# Attributes

Attributes play a crucial role in Iridis by providing additional information to the compiler about types and procedures. Essentially, attributes act as markers that supply contextual details.
## Using Attributes
### Global Attributes

Global attributes give more information mainly to the linker instead of the compiler. You can mark variables/containers as `#[private]` or `#[export]` _(which means that the linking symbol will not be mangled, this is equivalent to C++'s `extern "C"`')_.

For instance:

```iridis
// SomeFile.iridis

IO :: import!("std.IO")
// ^ imports are always private so you can't call SomeFile.IO

#[private]
Relative :: struct
{
    name       : string,
    generation : u32
}

Human :: struct
{
    relatives : [3; Relative],
    name      : string,
    age       : u8,
}

#[private]
PrintRelative :: proc(relative: Relative)
{
    IO.PrintLine("Name: {}",        relative.name)
    IO.PrintLine("Generation:  {}", relative.generation)
}

#[export] // This will use the C calling convention
PrintHuman :: proc(human: Human)
{
    IO.PrintLine("Name: {}", human.name)
    IO.PrintLine("Age:  {}", human.age)
}
```



### Typed Attributes

Typed attributes constrain generic types. For instance, you can limit types to those that can interact with built-in procedures.

```iridis
SomeStruct<T: #Lengthable, V: #Stringable> :: struct
{
    something : T,
    someone   : V
}

// ...

SomeProc :: proc(some: SomeStruct)
{
    IO.PrintLine("Someone: {}", tostring!(someone))
    IO.PrintLine("Something's length: {}", len!(someone))
}
```

## Implementing Attributes for custom types

Consider a custom type, say, `Robot`:

```iridis
Robot :: struct
{
    codeName : string,
    name     : string,
    size     : u32,
}
```
To enable calling `len!(robot)` and obtaining the robot's size, define an attribute for the robot:

```iridis
#Lengthable :: (self: Robot) -> u32
{
    return self.u32
}
```

*This concept is akin to Rust traits.*
