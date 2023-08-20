#pragma once

#include "AST.h"
#include "Token.h"
#include "Lexer.h"

#include "Core/Log.h"

typedef struct Parser_t
{
    int currentTokenIndex;
    char** sourceLines;

    Token* tokens;
    Node* nodes;
} Parser;

Parser Parse(char* sourceCode);
void DestroyParser(Parser* parser);
