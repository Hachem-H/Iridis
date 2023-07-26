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
