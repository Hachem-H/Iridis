# Interoperability with C

Iridis is meant to serve as an alternative or a replacement for C, but that does not mean that it cannot interact with already existing C code. For example, we may find ourselves in a large C project where we would like to use Iridis to implement certain features. Consider I had this code base for instance:

```c
// SomeFile.h

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

```c
#include "SomeFile.h"

void SomeObscureFunction()
{
    puts("I am some weird function with no declaration");
}

void PrintPersonData(Person person)
{
    printf("Name: %s\n", person.name);
    printf("Age: %d\n", person.age);
}
```

There is 2 ways we could use this in Iridis

## Manual Interoperability

### Calling C from Iridis

The same way you would write custom bindings to link your source code with a C library, you could redefine the functions yourself in Iridis this way:

```
// SomeFile.iridis

Person :: struct
{
	name : cstring,
	age  : u32,
}

PrintPersonData     :: extern proc (Person)
SomeObscureFunction :: extern proc ()
```

The `extern` keyword automatically changes the procedure to the C calling conventions (so no name mangling). It is also important that `cstring` is just `^u8`under the hood, though it is preferred to use `cstring` instead since the built-in `len!` will work in a similar manner to `strlen`. 

```iridis
// main.iridis

SomeFile :: import!("SomeFile")

main :: proc()
{
    hachem := SomeFile.Person
    {
        name = "Hachem",
        age  = 16, // As of the day of writing 
    }

    SomeFile.PrintPersonData(hachem)
    SomeFile.SomeObscureFunction()
}
```

And all that would be left would be linking, since in this case the main unit is the Iridis program, we should link the C code into the Iridis executable. 

```
$ clang -o SomeFile.o -c SomeFile.c
$ iridis compile -o main.o -i main.iridis
$ iridis link main.o SomeFile.o -o Application
$ ./Application
Name: Hachem
Age: 16
I am some weird function with no declaration
```

### Calling Iridis from C

The inverse passage is also possible, and the process is also rather similar, we can use the same example that we used before

```main.iridis
// SomeFile.iridis

io :: import("std.io")

Person :: struct
{
	name : cstring, // NOTE: Using cstring
	age  : u32,
}

#[export]
PrintPersonData :: proc(person: Person)
{
	io.println("Name: {}", person.name)
	io.println("Age: {}", person.age)
}

#[export]
SomeObscureProcedure :: proc()
{
    io.println("I am some other function with a definition")
}
```

Using this in C would be rather easy, all we have to do is similarly redefine everything

```c
// main.c

#include <stdint.h> 

typedef struct Person_t
{
	char* name;
    uint32_t age;
} Person;

extern void PrintPersonData(Person);
extern void SomeObscureProcedure();

int main()
{
	Person hachem = { "Hachem", 16 };
    PrintPersonData(hachem);
    SomeObscureProcedure():
}
```

Linking also works the same way

```
$ iridis compile -i SomeFile.iridis -o SomeFile.o
$ clang -o main.o -c main.c
$ clang -o Application main.o SomeFile.o
$ ./Application
Name: Hachem
Age: 16
I am some other function with a declaration
```

## Automatic Interoperability

### Calling C from Iridis

Similarly to how C has the `#include` directive, Iridis has one specifically for header files, it automatically converts the types as needed. It can be used simply as

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
        name = "Hachem",
        age  = 16
    }

    SomeFile.PrintPersonData(hachem)
    SomeFile.SomeObscureFunction()
}
```

Notice how I had to call the `SomeObscureFunction` from an extern? The same way you would do per translation unit in C._Just noting that i placed in a module so it would be considered the same-ish library_.

We compile this the same exact way.

### Calling Iridis from C

Now, instead of manually having to write the bindings ourselves, the `iridis` compiler comes with a nice little command called `genbind` or **Gen**erate **Bind**ing. It basically does all of the grunt work for you:

```
$ iridis genbind -o SomeFile.h -i SomeFile.iridis
```

```c
// SomeFile.h

#include <stdint.h>

typedef struct Person
{
    char* name;
    uint32_t age;
} Person;

extern void PrintPersonData(Person);
extern void SomeObscureProcedure();
```

And we can use it in the same exact way that you would expect. Compiling is also identical.
