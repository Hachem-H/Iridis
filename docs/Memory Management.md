# Memory Management

Memory Management probably has to be one of the important topics when discussing a low-level programming language. The question lies in "How do I allocate/free memory safely?". Well, the heart of the memory manager lies in the `Memory.Allocator` structure, defined as:

```iridis
// std/Memory.iridis

MemoryError :: enum
{
    InsufficientMemory,
    Fragmentation,
    BufferTooLarge,
	
    DoubleFree,
    InvalidPointer,
	
    // ...
}

Allocator<T> :: struct
{
    Allocate<T> : proc(^Allocator, u32) -> MemoryError?^T,
    Free<T>     : proc(^Allocator, ^T)  -> MemoryError?,
}

Allocate<T> :: proc(allocator: ^Allocator, count: u32) -> MemoryError?^T
{
     return allocator.Allocate<T>(allocator, count)?
}

Free<T> :: proc(allocator: ^Allocator, buffer: ^T)  -> MemoryError?
{
    try allocator.Free<T>(allocator, buffer)?
    temp := buffer
}

UnsafeFree<T> :: proc(allocator: ^Allocator, buffer: ^T)  -> MemoryError?
{
    try allocator.Free<T>(allocator, buffer)?
}
```

We notice that the `Allocator` struct contains two simple callbacks, `Allocate` and `Free`. Creating an allocator involves simply implementing both of these functions. We also note the usage of the helper functions `Memory.Allocate`,  `Memory.Free` and  `Memory.Unsafe`, which simplify the usage of clearing up the ownership after a buffer has been freed.

## Portability
Why the custom `Allocator` structure? Can't you have a simple `Allocate` and `Free`?  Lets say that Iridis has predefined Allocate and Free procedures that it uses through out the entire standard library. If i wrote my custom kernel or if this is running on bare metal. I would have my own custom allocation methods. In turn I would not be able to use any of the standard library functions as they use their own allocators. This is the issue for example in C or Rust, where in C you would have to re-implement the entire C standard library from scratch (It is quite small, though it is still an annoying task), and in Rust you would have to compile without linking to the standard library in the first place.

On the other hand, since I have custom allocators, I can build the entire standard library in such a way that it depends on passing in an allocator. For example consider this code being some custom kernel code:

```
Memory :: import!("std.Memory")
Types  :: import!("std.Types")

CustomAlloc<T> :: proc(context: ^Allocator, count: i32) -> MemoryError?^T
{
    // ...
}

CustomFree<T> :: proc(context: ^Allocator, buffer: ^T) -> MemoryError?
{
    // ...
}

main :: proc()
{
    customAllocator := Memory.Allocator
    {
        Alloc = CustomAlloc,
        Free  = CustomFree,
    }
    
    dynamicArray := Types.CreateDynamicArray<i32>(&customAllocator)
    defer Types.DestroyDynamicArray(dynamicArray)
    
    Types.PushToDynamicArray(&dynamicArray, 1, 2, 3)
}
```

If I wanted to use the built-in implementation of the dynamic array, I can simply just pass in my custom allocator to the `CreateDynamicArray`, which lets the standard library use this particular implementation of the allocator. This also means that you can use multiple types of allocators at the same time, unlike Odin where you would have to set the context per-scope.

## Allocation Trackers

Since I pass in context through every single allocation, we can draft a simple implementation of the `DefaultAllocator` which can track if all memory allocated is freed. We can first implement the procedures used for the supported operating systems as so.

```iridis
#[private]
InternalAlloc<T> :: proc(context: ^Allocator, count: i32) -> MemoryError?^T
{
	// OS dependent
}

#[private]
InternalFree<T> :: proc(context: ^Allocator, buffer: ^T) -> MemoryError?
{
	// OS dependent
}

#[private]
InternalAllocator := Allocator
{
    Allocate = InternalAlloc,
    Free     = InternalFree,
}
```

The start by implementing the default allocator as follows:

```iridis
Runtime :: import!("std.Runtime")

DefaultAllocator :: struct using Allocator
{
     tracker : Types.DynamicArray<(Runtime.CallTrace, u32)>
}

#[private]
DefaultAlloc<T> :: proc(context: ^Allocator, count: u32) -> MemoryError?^T
{
    allocator := context as ^DefaultAllocator

    buffer := try InternalAlloc<T>(count)
    address := unsafe buffer
    tracked := (Runtime.GetStackTrace(), address as u32)
    Types.PushToDynamicArray(&allocator.tracker, tracked)

    return buffer
}

#[private]
DefaultFree<T> :: proc(context: ^Allocator, buffer: ^T) -> MemoryError?
{
    allocator := context as ^DefaultAllocator
    bufferIndex := 0

   for i in 0..<allocator.tracker.size
   {
        (_, address) := Types.DynamicArrayAt(allocator.tracker, i)
        if address == buffer as u32; do
            bufferIndex = i
   }
        
    Types.DynamicArrayRemoveAt(&allocator.tracker, bufferIndex)!
    try InternalFree(buffer)
}
```

The `DefaultAlloc` and `DefaultFree` procedures track the memory allocations by storing the runtime stack-trace (where the allocation has happened) and the buffer address. So we can simply implement the `CreateDefaultAllocator` and `DestroyDefaultAllocator` in such a way that it checks if all the memory blocks have been freed.

```iridis
CreateDefaultAllocator :: proc() -> DefaultAllocator
{
    return DefaultAllocator
    {
        Allocate = DefaultAllocate,
        Free     = DefaultFree,
        tracker  = Types.CreateDynamicArray(&InternalAllocator)
    }
}

DestroyDefaultAllocator :: proc(allocator: ^DefaultAllocator)
{
    if allocator.tracker.size != 0; do
    for i in allocator.tracker.size
    {
        (callTrace, address) = allocator.tracker.buffer[i]
        IO.ErrorLine("Unfreed buffer @ {}: ", address)

        for entry in callTrace; do
            io.Error("{}:{} in {}", entry.line, entry.file, entry.name)

        Types.DestroyDynamicArray(&allocator.tracker)
    }
    
    temp := allocator
}
```

Although this implementation is rather slow, the actual implementation in the standard library will be signifcantly faster, and the 'memory tracker' will only be enabled in the `debug` and `release` profiles.

## Other types of allocators
### Global Allocator
Simply put, the global allocator is a default allocator which runs in the global scope. The `DestroyDefaultAllocator` is ran after the program ends using the global defer.
```iridis
OS :: import!("std.OS")

comptime if OS.IsSupported()
{
    GlobalAllocator := CreateDefaultAllocator()
    defer DestroyDefaultAllocator(&GlobalAllocator)
}
```

Its worth noting that comptime works a lot like `#ifdef` in C.

### Fixed Buffer Allocator
A fixed buffer allocator simply allocates memory to a buffer instead of a heap. For instance:
```iridis
Memory :: import!("std.Memory")

main :: proc()
{
    buffer := [1000; 0] // A buffer of 1000x zeros
    allocator := CreateFixedBufferAllocator(&buffer)
    
    ints := Memory.Allocate<i32>(&allocator, 5)
    // ... 
    Memory.Free(&allocator, ints)
}
```

### Residue Allocator
This allocator should never be used and is only implemented as a joke. What a residue allocator does is simply frees all the memory up-front. This would mean that using the this comes with reduced performance to maintain a log of allocated buffers. Which is why when you use the Resiude Allocator, when the program exit, you will have a duck yelling at you:
```
      __
  __( o)>  A real programmer
  \ <_ )      manages his own memory.
   `---'              - A wise duck
     ||     
   ==^^==
```

---
_It is very important to note that these are how the allocators will work in practice and will probably end up having completely different internal implementations._