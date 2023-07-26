# Basic Example

In this comprehensive guide, we will delve into the basics of the Iridis, a language designed to be expressive, efficient, and developer-friendly.

## Getting Started

In Iridis, like any C/C++ application, the program's entry point is defined as a procedure called `main`:

```iridis
main :: proc()
{
    
}
```

The `main` procedure serves as the starting point for the program, and all code execution begins from this point. To indicate that `main` is a global static procedure that cannot be modified, it is declared as a constant using the `::` operator.

Similarly, constants can be defined in Iridis using the following syntax:

```iridis
SomeConstant :: 123456789            // With type inference
SomeOtherConstant : i32 : 1234566789 // With explicit typing
```

You can create constants using either type inference or explicit type declaration.

## Printing Text Output

To display text output, Iridis provides the `io` module from the standard library. To use it, you can import the module using the built-in `import!` directive:

```iridis
io :: import!("std.io")

main :: proc()
{
    io.println("Hello, World!")
}
```

In the `main` procedure, we use `io.println()` to print the text "Hello, World!" followed by a new line. The `println` procedure can also perform text formatting, similar to Rust's string formatting.

```iridis
io :: import!("std.io")

main :: proc()
{
    world :: "World!"
    io.println("Hello, {}", world)
}
```

In this example, we define a variable `world` with the value "World!" and use it to print the message "Hello, World!" with the help of string formatting.

## Working with Structures

Iridis supports defining structures using the `struct` keyword using the `::` operator like procedures:

```iridis
HumanBeing :: struct
{
    name: string,
    age: u8,
    birthYear: u32
}
```

In this example, we define a `HumanBeing` struct with three fields: `name`, `age`, and `birthYear`. Each field has a specific data type associated with it.

To create an instance of the `HumanBeing` struct, you can use the following syntax:

```iridis
human := HumanBeing
{
    name: "Hachem",
    age: 17,
    birthYear: 2006,
}
```

This creates a new `human` instance with the name "Hachem," age 17, and birth year 2006.

# Conditional Statements

Conditional statements in Iridis work similarly to other programming languages:

```iridis
someValue := someProc()

if someValue
{
    io.println("Some value was true")
    io.println("Who knows what someProc() is")
}
```

Here, we use an `if` statement to check the value returned by the `someProc()` procedure. If the value is true, the specified code block will be executed.

## Loops

Iridis supports both traditional and range-based loops:

Traditional loop:

```iridis
someArray := [1, 2, 3, 4]

for i in someArray
{
    io.println("{}", i)
}
```

This loop iterates through the elements of the `someArray` and prints each element.

Loop using `do` syntax:

```iridis
for i in someArray; do
    io.println("{}", i)
```

The `do` syntax is a shorthand for writing single statement scopes since some people might find single statements in curly braces jarring, commonly used in `if` statements, and loops.

Range-based for loops:

```iridis
for i in 0..<len!(someArray); do // Equivalent to 'for (int i = 0; i < someArrayLength; i++)'
    io.print("{} ", someArray[i])
for i in 0..=len!(someArray); do // Equivalent to 'for (int i = 0; i <= someArrayLength; i++)'
    io.print("{} ", someArray[i])
```

These examples demonstrate how to iterate over elements in an array using different loop styles.

## Conclusion

In this guide, we have covered the basics of the  language. To explore further features and examples, refer to the `main.iridis` file. Happy coding!
