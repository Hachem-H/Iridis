#pragma once

#include "Common.h"

typedef enum TypeID_t
{
    TypeID_i8,
    TypeID_i16,
    TypeID_i32,
    TypeID_i64,

    TypeID_u8,
    TypeID_u16,
    TypeID_u32,
    TypeID_u64,

    TypeID_c32,
    TypeID_c64,
    TypeID_c128,

    TypeID_qat64,
    TypeID_qat128,
    TypeID_qat256,

    TypeID_Vector,
    TypeID_Matrix,

    TypeID_f32,
    TypeID_f64,

    TypeID_char,
    TypeID_bool,
    TypeID_string,
    TypeID_cstring,

    TypeID_mod,
    TypeID_enum,
    TypeID_proc,
    TypeID_struct,
    
    TypeID_Type,
    TypeID_Array,
    TypeID_Pointer,
    TypeID_Possible,
} TypeID;

typedef struct Value Value;
struct Value
{
    TypeID typeID;
    usize  size;
    
    // TODO(Hachem): Other types
    union
    {
        struct { i8  value; } int8;
        struct { i16 value; } int16;
        struct { i32 value; } int32;
        struct { i64 value; } int64;

        struct { u8  value; } uint8;
        struct { u16 value; } uint16;
        struct { u32 value; } uint32;
        struct { u64 value; } uint64;

        struct { f32 value; } float32;
        struct { f64 value; } float64;

        struct { char value;  } character;
        struct { i8 value:1;  } boolean;
        struct { char* value; } cstring;
        struct { char* value; 
                 i32   size;  } string;
    
        struct { Value* value;  } type;
        struct { void* pointer; } pointer;
        struct { void* pointer;
                 i32   size;    } array;
        struct { i32 posssible;
                 bool ok;
                 Value* some;   } possible;
    };
};
