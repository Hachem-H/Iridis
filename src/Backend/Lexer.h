#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "Token.h"

#define MAX_BUFFER_SIZE 1024

typedef struct TokenInfo_t
{
    uint32_t line;
    uint32_t column;
    char* identifier;
} TokenInfo;

typedef struct Lexer_t
{
    TokenInfo* tokenInfos;
    char* buffer;
    
    int currentLine;
    int currentColumn;
    int tokenStartLine;
    int floatStartIndex;
    int tokenStartColumn;

    bool insideSingleLineComment;
    bool insideMultilineComment;
    bool insideFloat;
    bool insideQuote;
} Lexer;

TokenInfo CreateTokenInfo(const uint32_t line, const uint32_t column, const char* identifier);
Token* Tokenize(const char* source);
