#pragma once

#include "Token.h"
#include "Lexer.h"

#include "Core/Log.h"

#include <llvm-c/Core.h>

typedef struct Parser_t
{
    int currentTokenIndex;
    char** sourceLines;
    Token* tokens;

    LLVMBuilderRef builder;
    LLVMModuleRef module;
} Parser;

void Parse(char* sourceCode);
