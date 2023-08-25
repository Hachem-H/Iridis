#pragma once

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define internal static
#define global   static

#define streql(str1, str2) (strcmp(str1, str2) == 0)

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef float  f32;
typedef double f64;

typedef size_t    usize;
typedef ptrdiff_t isize;

typedef uint8_t       byte;
typedef unsigned char uchar;
