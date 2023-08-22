# The Ownership system

The main goal of Iridis is to have a rather safe manual memory management, which is why (even though I dreaded accepting the neccesity of this), Iridis is going to have an Ownership System. Now the way that this ownership system is going to work is slightly different than the way the Rust borrow checker works. Iridis, to avoid unnecessary complexity, will not have the idea of a 'reference'. Instead, pointers themselves will have an owner, and given one pointer/memory address, only one owner can exist at a given time. The way it works is the this.

```iridis
Mem :: import!("std.Memory")

main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    buffer := Mem.Allocate<i32>(&allocator, 1000)!
    Mem.Free(&allocator, buffer)!
}
```

Consider we had a simple buffer of 1000 integers allocated on the heap. the `Memory.Allocate` procedure will return a pointer to said buffer, ONLY, the `buffer` variable holds ownership to that pointer. Setting a different pointer equal to the buffer will move ownership from `buffer` to that new variable.

```iridis
main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    buffer := Mem.Allocate<i32>(&allocator, 1000)!
    newOwner := buffer
    
    Mem.Free(&allocator, buffer)!
    // ^ This line will not compile anymore
}
```

The ownership has moved from `buffer` to `newOwner`. Meaning, I simply cannot use the `buffer` variable at all until ownership is passed to it.  I.E. I can move use the `newOwner` to free the buffer.

```iridis
buffer := Mem.Allocate<i32>(&allocator, 1000)!
newOwner := buffer
    
allocator.Free(newOwner)!
```
Or simply give `buffer` ownership back
```iridis
buffer := Mem.Allocate<i32>(&allocator, 1000)!
newOwner := buffer
// ...
buffer := newOwner
Mem.Free(buffer)!
```

## Scoping
The reason that the ownership system works this way is to avoid stuff like this:
```iridis
Mem :: import!("std.Memory")

main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    someBuffer := Mem.Allocate<i32>(&allocator, 1000)!
    {
        someCopy := someBuffer
        Mem.Free(&allocator, someCopy)!
    }
    
    // ...
    Mem.Free(&allocator, someBuffer)!
}
```
This code will simply not compile, there is a rather visible double free situation, though in a larger code base this might not be very visible. If we run through the code really quickly, we can see that ownership of `someBuffer` is passed over to `someCopy`, meaning freeing `someBuffer` will not even work since ownership is passed over to `someCopy` which is now out of scope.

## Procedures

In procedures, since Iridis does not have the idea of a reference, it might seem annoying to constantly move ownership between procedure argument and procedure return. This is what Odin does and what Rust uses (if you for some reason don't use references). Instead, Iridis decided to go the simple route, procedure arguments simply return ownership. For example:

```iridis
IO :: import!("std.IO")
Mem :: import!("std.Memory")

PrintSomeBuffer :: proc(buffer: ^char, size: u32)
{
    for i in 0..<size; do
    	IO.Print("{}", buffer[i])
    IO.PrintLine()
    // Here, `buffer` is not re-assigned anywhere, meaning it is still the owner,
    // in this case, simply the ownership is passed back to the argument
}

main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    bufferSize :: 1000
    buffer := Mem.Allocate<i32>(&allocator, bufferSize)!
    PrintSomeBuffer(buffer, bufferSize)
    // `buffer` got ownership back after the procedure return
    Mem.Free(&allocator, buffer)!
}
```

## Uninitialized data

What about some uninitialized pointer, who owns it? Well in Iridis, there is no `nil` type, so you cannot even have an uninitialized pointer in the first place (This includes default values, if I had some structure which was set to default, and that structure had a pointer, the compiler will throw an error). Each pointer created will either get ownership or have its own owner. This way you can avoid using lifetimes. This applies to structures too.

```iridis
Mem :: import!("std.Memory")

Human :: struct
{
	name     : ^char,
	nameSize : u32,
}

main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    bufferSize :: 7
    hachem := Human
    {
        name     = Mem.Allocate<char>(&allocator, bufferSize)!,
        nameSize = bufferSize,
    }
    
    // ...
    Mem.Copy("Hachem", hachem.name, hachem.nameSize)!
    hachem.name[hachem.nameSize] = 0
    // ...
    
    Mem.Free(&allocator, human.name)!
}
```

## But I know what I am doing...

Let's face it...no you don't. But if you happen to, you can unsafely create a copy of your pointer without moving ownership by using the `unsafe` keyword after assignment (unlike Rust, you don't need an entire scope).

```iridis
main :: proc()
{
    allocator := Mem.GetGlobalAllocator()
    
    buffer := Mem.Allocate<i32>(&allocator, 1000)!
    newOwner := unsafe buffer
    
    Mem.Free(&allocator, buffer)!
}
```

In this case, `buffer` has 2 owners, it self and `newOwner`. The compiler treats them as if they each had their own independent ownership. As in, if you copy `newOwner` to some `newerOwner`, ownership will be copied.

## Free
It is important to note that `Memory.Free` removes ownership completely so a buffer cannot be used after it has been freed. The unsafe equivalent of this is `Memory.UnsafeFree`.
