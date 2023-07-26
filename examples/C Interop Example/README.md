# C Interop Example

Iridis is a high-performance programming language, and sometimes, we may find ourselves in a large C project where we would like to use Iridis to implement certain features. In such scenarios, we can use Iridis's C interop capabilities to seamlessly integrate with existing C code. This example demonstrates how to use the `include!` directive to work with C types and functions in Iridis.

## C Header File - SomeFile.h

```c
#pragma once

#include <stdio.h>
#include <stdint.h>

typedef struct Person_t
{
    char* name;
    uint32_t age;
} Person;

void PrintPersonData(Person person);
```

## C Source File - SomeFile.c

```c
#include "SomeFile.h"

void SomeObscureFunction()
{
    puts("I am some weird function with no declaration");
    puts("Surely Iridis wont be able to find me");
}

void PrintPersonData(Person person)
{
    printf("Name: %s\n", person.name);
    printf("Age: %d\n", person.age);
}
```

To use these functions in Iridis, we can include the `SomeFile.h` using the `include!` directive, which automatically transforms C types into Iridis types:

```iridis
SomeFile :: include!("SomeFile.h")
```

Now we can easily use the functions and the `Person` struct defined in `SomeFile.h`:

```iridis
SomeFile :: include!("SomeFile.h")

main :: proc()
{
    hachem := SomeFile.Person
    {
        name: "Hachem",
        age: 17
    }

    SomeFile.PrintPersonData(hachem)
}
```

Additionally, for functions with no declaration like `SomeObscureFunction`, we can use `extern` to inform Iridis that it's a C function:

```iridis
SomeFile :: include!("SomeFile.h")
SomeFile :: mod
{
    SomeObscureFunction :: extern proc (Person)
}

main :: proc()
{
    hachem := SomeFile.Person
    {
        name: "Hachem",
        age: 17
    }

    SomeFile.PrintPersonData(hachem)
    SomeFile.SomeObscureFunction()
}
```

By doing this, we can successfully use C functions in Iridis without redefining C structs or functions. The `include!` directive makes the integration seamless, allowing us to combine the power and expressiveness of Iridis with existing C code. Happy coding!

**Note: Additionally, the Iridis compiler provides the ability to generate header bindings for Iridis code. You can achieve this using the following command:**

```sh
$ iridis genbind main.iridis -o main.h
```

This command instructs the Iridis compiler to generate header bindings for the `main.iridis` file and output them to the file `main.h`. The generated header will contain the necessary C declarations for any external functions and types defined in `main.iridis`, making it easier to include the Iridis code as part of a C project. With this capability, the Iridis language becomes even more accessible and flexible, allowing developers to combine the strengths of both Iridis and C to create high-performance and feature-rich applications.
