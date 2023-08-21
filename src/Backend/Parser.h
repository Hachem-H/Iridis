#pragma once

#include "Common.h"

#include "AST.h"
#include "Token.h"
#include "Lexer.h"

#include "Core/Log.h"

typedef struct Parser_t
{
    i32 currentTokenIndex;
    char** sourceLines;

    Token* tokens;
    Node* nodes;
} Parser;

Parser Parse(char* sourceCode);
void DestroyParser(Parser* parser);
