#pragma once

typedef struct ProcedureAST_t
{
    char* name;
} ProcedureAST;

typedef struct StructureAST_t
{
    char* name;
} StructureAST;

typedef struct ExpressionAST_t
{
    union
    {
        ProcedureAST procedure;
        StructureAST structure;
    };
} ExpressionAST;
