#pragma once

#include "Token.h"
#include "Lexer.h"

#include "Core/Log.h"

typedef struct Parser_t
{
    int currentTokenIndex;
    char** sourceLines;
    Token* tokens;
} Parser;

void Parse(char* sourceCode);
