#pragma once

#include <stdio.h>
#include <stdint.h>

typedef struct Person_t
{
    char* name;
    uint32_t age;
} Person;

void PrintPersonData(Person person);
