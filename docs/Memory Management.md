# Portability

Iridis is advertised as a system's programming language. So memory management (allocation and freeing) is left to the programmer. Now this is slightly of an issue for portability. Lets say that Iridis has predefined Allocate and Free procedures that it uses through out the entire standard library. If i wrote my custom kernel or if this is running on bare metal. I would have my own custom allocation methods. Intern I would not be able to use any of the standard library functions as they use their own allocators. This is the issue for example in C or Rust, where in C you would have to re-implement the entire C standard library from scratch (It is quite small, though it is still an annoying task), and in Rust you would have to compile without linking to the standard library in the first place.

Iridis solves this by the idea of a Allocators. Simply put, an allocator is a structure with 2 callbacks, `Alloc` and `Free`. Meaning you can pass in your own custom allocate and free methods. For example, lets say that we were in a custom bare-metal environment, I could implement a custom allocator like this 

```iridis
memory :: import!("std.memory")

CustomAlloc<T> :: proc(count: i32) -> ^T
{
    // ...
}

CustomFree<T> :: proc(buffer: ^T)
{
    // ...
}

main :: proc()
{
    customAllocator := memory.Allocator
    {
        Alloc = CustomAlloc,
        Free  = CustomFree,
    }
}
```

Now how is this useful? Well, simply let the standard library not use a pre-defined custom allocator. When ever you have to deal with a data structure which deals with memory allocation. Simply pass it one. For example consider dynamic arrays, if I had my kernel, I could use the standard library one with my allocator like this:

```iridis
// ...

types :: import!("std.types")

main :: proc()
{
    customAllocator := memory.Allocator
    {
        Alloc = CustomAlloc,
        Free  = CustomFree,
    }

    dynamicArray := types.CreateDynamicArray<i32>(customAllocator)
    defer types.DestroyDynamicArray(dynamicArray)
    
    types.PushToDynamicArray(&dynamicArray, 1, 2, 3)
}
```

Now, what if I was on Windows and Linux or whatever, some platform that natively is supported by Iridis? well simply, the memory module has a `DefaultAllocator` which you could use like so:

```iridis
main :: proc()
{
    dynamicArray := types.CreateDynamicArray<i32>(memory.GetDefaultAllocator())
    defer types.DestroyDynamicArray(dynamicArray)
    
    types.PushToDynamicArray(&dynamicArray, 1, 2, 3)

}
```

# Memory Safety

Now, its not enough that I should be able to allocate memory. I should be able to do so safely. Mainly, avoiding memory leaks. So when I allocate a buffer, I want to make sure that it is eventually freed. *Now how can I do this in Iridis?* Of course this will lie in the standard library implementation of the `DefaultAllocator`

We can start by defining the `DefaultAlloc`/`DefaultFree` methods like this, such as `InternalAlloc`/`InternalFree` are operating system dependent:

```iridis
#[private]
InternalAlloc<T> :: proc(count: i32) -> ^T
{
	// ...
}

#[private]
InternalFree<T> :: proc(buffer: ^T)
{
	// ...
}

#[private]
DefaultAlloc<T> :: proc(count: i32) -> ^T
{
    buffer := InternalAlloc<T>(count)
    return buffer
}

#[private]
DefaultFree<T> :: proc(buffer: ^T)
{
    InternalFree(buffer)
}

GetDefaultAllocator :: proc() -> Allocator
{
    return Allocator
    {
        Alloc = DefaultAlloc,
        Free  = DefaultFree,
    }
}
```

Now you might be wondering: *why doesn't the `DefaultAlloc`/`DefaultFree` just implement the native procedures directly instead of making a different procedure for it?* Well my friend, because this is not going to **ONLY** be the implementation. What I can start by doing, is defining a simple `MemoryTracker` which is a dynamic array. But wait, *don't dynamic arrays use allocators to do stuff?* but if this is the default allocator, what will it use? Well, this is why I did the 2 internal procedures

```iridis
types :: import!("std.types")

#[private]
InternalAllocator :: Allocator
{
    Alloc = InternalAlloc
    Free  = InternalFree,
}

#[private]
MemoryTracker := types.CreateDynamicArray<(runtime.StackTrace, i32)>(InternalAllocator)
// The Runtime Stacktrace is a type which holds information about where the functions were called. We can use this determine where an allocation happened.
// The i32 is the allocated buffer address.
```

meaning we can modify the `DefaultAlloc`/`DefaultFree` functions to push allocation and free data to the  memory tracker like this

```
#[private]
DefaultAlloc<T> :: proc(count: i32) -> ^T
{
    buffer := InternalAlloc<T>(count)
    types.PushToDynamicArray(&MemoryTracker, (runtime.GetStackTrace(), buffer));
    return buffer
}

#[private]
DefaultFree<T> :: proc(buffer: ^T)
{
    bufferIndex := 0
    for i in 0..<MemoryTracker.size
    {
        (_, address) := types.DynamicArrayAt(MemoryTracker, i)
        if address == buffer; do
            bufferIndex = i
    }
    types.DynamicArrayPopAt(&MemoryTracker, bufferIndex)
    
    InternalFree(buffer)
}
```

Noe that we stored them, *then what?* Well, let me introduce you to a keyword called `defer`. What `defer` usually does, is that it executes a line of code when a scope ends. But when you do it globally, it would run this line of code at the end of the program execution. So what I could do, is check if all the buffers have been freed like this:

```iridis
io :: import!("std.io")

defer
{
    if MemoryTracker.size != 0
    {
        for (location, address) in MemoryTracker; do
            io.error("You allocated a buffer at address {} in {} but it wasn't freed", address, location)
    }
}
```

This way, we can inform the programmer that they didn't free their memory without forcing them into weird programming paradigms, (like the rust borrow checker). But I hear you saying: *Wouldn't this make allocation in general slower?* And the answer to that is... yes. Smart-ass.

## Optimizing safe memory allocation

I to introduce you the `comptime` keyword, which turns anything into a compile time statement (this includes `if`, `for` and`while`). Basically, Iridis can compile into 3 different profiles;

- Debug: "Compile with no optimizations and debug symbols"
- Release: "Compile with optimizations and debug symbols"
- Distribution: "Compile with max optimizations and no debug symbols"

Well, iridis has access to the runtime information from the compiler. So, we can use when to check what profile we are using. And ONLY check for non-freed blocks in the distribution build like this:

```
runtime :: import!("std.memory")

comptime if runtime.profile != .Distribution
{
    #[private]
    MemoryTracker := types.CreateDynamicArray<...>(InternalAllocator) 

    defer
    {
        // ...
    }
}

// ...

#[private]
DefaultAlloc<T> :: proc(count: i32) -> ^T
{
    comptime if runtime.profile != .Distribution
    {
        buffer := InternalAlloc<T>(count)
        types.PushToDynamicArray(&MemoryTracker, (runtime.GetStackTrace(), buffer));
    }

    return buffer
}

#[private]
DefaultFree<T> :: proc(buffer: ^T)
{
    comptime if runtime.profile != .Distribution
    {
        bufferIndex := 0
        for i in 0..<MemoryTracker.size
        {
            (_, address) := types.DynamicArrayAt(MemoryTracker, i)
            if address == buffer; do
                bufferIndex = i
        }

        types.DynamicArrayPopAt(&MemoryTracker, bufferIndex)
    }

    InternalFree(buffer)
}
```

## Local Allocators

Realistically, I will probably split this into a `GlobalAllocator` and a `DefaultAllocator`, as it would be practicall for game-developpment. Consider this example:

```iridis
memory :: import!("std.memory")

// ...

UpdateEntities :: proc()
{
    allocator := memory.GetGlobalAllocator()
    entities := allocator.Allocate<Entity>(100)
    UpdateEntity(entities)
}

// ...

main :: proc()
{
    while GameIsRunning(); do
        UpdateEntites()
}
```

This is obviously a memory leak, but we would get a 100 error messages per game cycle about the memory not being freed and that's simply not ideal. What would be better is to a have a local allocator where it would be instant.

``` iridis
// ...

UpdateEntities :: proc()
{
    allocator := memory.CreateDefaultAllocator()
    defer memory.DestroyDefaultAllocator(allocator)
    
    entities := allocator.Allocate<Entity>(100)
    UpdateEntity(entities)
}

// ...
```

When the `DestroyDefaultAllocator` method is called, we would immediately get information about the memory leak and cause a crash possibly. And so the `GlobalAllocator` would simply be a default one where the global `defer` would call the destroy procedure.

# Garbage Collection

Now due to the way that we wrote our `GlobalAllocator` we can simply use the same exact method to implement a basic garbage collector which frees the buffers for you like so:

```iridis
// In this case, the MemoryTracker doesn't need to store the stack trace since we aren't warning the programmer

defer
{
    for address in MemoryTracker; do
    	InternalFree(address)
}
```

 Though, yes, this would mean that using the garbage collector will come with reduced performance to maintain memory. Which is why when you use the garbage collector, when the program exit, you will have a duck being mean to you:

```
      __
  __( o)>  A real programmer
  \ <_ )    manages his own memory.
   `---'   
     ||     
   ==^^==
```

# Conclusion

The way that Iridis handles memory management through the use of `comptime` and the `defer` keyword allows it to be quite portable and makes it quite useful in terms of kernel design. It also means that it could potentially be used in various fields of application where memory management isn't the main goal through the "Garbage Collector".

---

*It is very important to note that this is not and probably will not be the actual implementation as it is quite inefficient. In reality, I will probably use the concept of memory arenas and better heap block searching*.
