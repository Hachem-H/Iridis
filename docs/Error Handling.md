# Error Handling
Errors are an innate part of writing code. You cannot write an entire program without having at least one place where your program errors out. It's just how programing goes. Unlike certain other programming languages, wherein error categories and optional types are included as components of the language's standard library (e.g., `std::optional` or `Result<T, E>`/`Option<T>`), these kinds of types are integrated directly into Iridis as intrinsic elements of the language. For the sake of simplicity, both error and optional types are represented as a single type. Consider the following highly complicated procedure as an example:

```iridis
DivideNumbers :: proc(a: i32, b: i32) -> i32
{
    return a/b
}
```

We can simply call it and while storing the result in some `value` variable:

```iridis
SomeProc :: proc()
{
    value := DivideNumbers(123, 456)
}
```

This seems fine and all, but there are some obvious errors that might problematic, for example, division by zero. Well, we can account for that with the `Possible` type notated with `?T`

```iridis
DivideNumbers :: proc(a: i23, b: i32) -> ?i32
{
    if b == 0; do
        return // An empty return
    return a/b
}
```

Now, when we want to call it, all we have to do is check of a value has been returned.
```iridis
IO :: import!("std.IO")

SomeProc :: proc()
{
    value := DivideNumbers(123, 0)? // Note the presence of ?

    if value; do
        IO.PrintLine("Got a value of {}", value!) // unwrap the value
    else; do
        IO.PrintLine("Got a possible error")
}
```

Or you could also give a potential default value instead if a return was not provided
```iridis
SomeProc :: proc()
{
    value := DivideNumbers(123, 0) ? 0
    // In this case value is 0
}
```

## Error Types
Let's say for sake of argument that we had several possible errors, we can put them all in an error enum like this:
```
PossibleErrors :: enum
{
    DivideByZero,
    IllegalNumber,
}
```

We would modify the `DivideNumbers` procedure to return this enum by simply changing the declaration of the `Possible` type, notation wise, the Rust equivalent of `Result<T, E>` would be a `E?T`
```iridis
DivideNumbers :: proc(a: i32, b: i32) -> PossibleErrors?i32
{
    if b == 0; do
        return .DivideByZero

    result := a/b
    if result == 420; do
        return .IllegalNumber
    return result
}
```

And now we can handle each error separately (default values still work):
```iridis
SomeProc :: proc()
{
    value := DivideNumbers(123, 0)
    match value
    {
        .DivideByZero  => IO.PrintLine("Division by zero error")
        .IllegalNumber => IO.PrintLine("Got the forbidden number")

        else => IO.PrintLine("Got value of {}", value)
    }
}
```

## Ignoring or forwarding the error
Sometimes, we can be sure that an error wont happen, _or we feel too lazy to handle the error ourselves_. In those types of cases, we can simply force unwrap the procedure result directly, like so:
```iridis
SomeProc :: proc()
{
    value1 := DivideNumbers(1, 2)!
    value2 := DivideNumbers(1, 0)!
    // ^ This crashes your program
}
```

We can also forward the error to the calling procedure like this:
```iridis
SomeProc :: proc() -> PossibleErrors?
{
    value := try DivideNumbers(1, 0)
}
```

In this particular example, `SomeProc` will return an error of `PossibleErrors.DivideByZero` with a void return. For more complicated function where multiple errors can happen, you can simply add your second error enum in the possible declaration, _(This might change in future iterations)_:

```
SomeErrors :: enum
{
    DivideByZero,
    IllegalNumber,
}

SomeMoreErrors :: enum
{
	EmptyString,
	StringTooLong,
}

SomeProc      :: proc() -> SomeErrors?     { /* ... */ }
SomeOtherProc :: proc() -> SomeMoreErrors? { /* ... */ }

Calling :: proc() -> SomeErrors, SomeMoreErrors?
{
	try SomeProc()
	try SomeOtherProc()
}
```

