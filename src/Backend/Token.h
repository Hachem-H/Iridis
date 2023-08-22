#pragma once

#include "Common.h"

typedef enum TokenType_t
{
    TokenType_Identifier,
    TokenType_Integer,
    TokenType_String,
    TokenType_Float,
    TokenType_Bool,

    TokenType_Enumeration,
    TokenType_Procedure,
    TokenType_Structure,
    TokenType_External,
    TokenType_Module,

    TokenType_RBracket,
    TokenType_LBracket,
    TokenType_RBrace,
    TokenType_LBrace,
    TokenType_RParen,
    TokenType_LParen,
    TokenType_RAngle,
    TokenType_LAngle,

    TokenType_Colon,
    TokenType_SemiColon,
    TokenType_Caret,
    TokenType_Ampersand,
    TokenType_Bang,
    TokenType_Quote,
    TokenType_DoubleQuote,
    TokenType_Equal,
    TokenType_Comma,
    TokenType_Period,

    TokenType_Plus,
    TokenType_Minus,
    TokenType_Asterisk,
    TokenType_Slash,
} TokenType;

typedef struct Token_t
{
    char* representation;
    TokenType type;

    u32 line;
    u32 column;

    union
    {
        char*  identifier;
        double floating;
        i32    integer;
        bool   boolean;
    } literal;
} Token;

Token TokenFromString(i32 line, i32 column, const char* buffer);
char* StringFromTokenType(TokenType type);

void DestroyTokens(Token* tokens);
void DestroyToken(Token* token);
