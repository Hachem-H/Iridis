# Error Handling
Errors are an inate part of writing code. You cannot write an entire program without encountering at least a place where your program errors out. It's just programing goes. Unlike other languages where the error is part of the language library (like `std::optional`, or `Result<T>`), error handling and optional values are one, and are built in as parts of the language. For example, consider this extremely complicated procedure:

```iridis
DivideNumbers :: proc(a: i32, b: i32) -> i32
{
    return a/b
}
```

We can simply call it and storing the reuslt in some `value` variable:

```iridis
SomeProc :: proc()
{
    value := DivideNumbers(123, 456)
}
```

This seems fine and all, but there are some obvious errors that might problematic, for example, division by zero. Well we can account for that with the error type, (In Iridis, the error and optional types are one).

```iridis
DivideNumbers :: proc(a: i23, b: i32) -> ?i32
{
    if b == 0; do
        return // An empty return
    return a/b
}
```

Now, when we want to call it, all we have to do is check for the error.
```iridis
IO :: import!("std.IO")

SomeProc :: proc()
{
    value := DivideNumbers(123, 0)? // Note the presence of ?

    if value; do
        IO.PrintLine("Got a value of {}", value^) // De-reference to get the result
    else; do
        IO.PrintLine("Got a possible error")
}
```

Or you could also give a potential default value instead
```iridis
SomeProc :: proc()
{
    value := DivideNumbers(123, 0) ? 0
    // In this case value is 0
}
```

## Error enums
Let's say for sake of argument that we had several possible errors, we can put them all in an error enum like this:
```
PossibleErrors :: enum
{
    DivideByZero,
    IllegalNumber,
}
```

We would modify the `DivideNumbers` procedure to return this enum by simply changing the declaration
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

And now we can handle each error seperatly (default values still work):
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

## Ignoring or fowarding the error
Sometimes, we can be sure that an error wont happen, _or we feel too lazy to handle the error ourselves_. So, I added the ability to ignore the error completly.
```iridis
SomeProc :: proc()
{
    value := DivideNumbers(1, 2)!
}
```

We can also forward the error to the calling function like this
```iridis
SomeProc :: proc() -> PossibleErrors?
{
    value := try DivideNumbers(1, 0)
}
```

In this particular example, `SomeProc` will return an error of `PossibleErrors.DivideByZero` with a void return.
