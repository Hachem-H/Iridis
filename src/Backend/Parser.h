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
    Node** nodes;
} Parser;

Node* ParseIdentifierExpression(Parser* parser);
Node* ParseNumberExpression(Parser* parser);
Node* ParseParenExpression(Parser* parser);

Node* ParseProcedure(Parser* parser, char* identifier);
Node* ParsePrototype(Parser* parser, char* identifier);

Node* ParseBinaryExpression(Parser* parser, i32 expressionPrecedence, Node* left);

Node* ParseExpression(Parser* parser);
Node* ParsePrimary(Parser* parser);

Parser Parse(char* sourceCode);

void DestroyParser(Parser* parser);
