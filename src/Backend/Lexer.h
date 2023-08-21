#pragma once

#include "Common.h"
#include "Token.h"

#define MAX_BUFFER_SIZE 1024

typedef struct TokenInfo_t
{
    u32 line;
    u32 column;
    char* identifier;
} TokenInfo;

typedef struct Lexer_t
{
    TokenInfo* tokenInfos;
    char* buffer;
    
    i32 currentLine;
    i32 currentColumn;
    i32 tokenStartLine;
    i32 floatStartIndex;
    i32 tokenStartColumn;

    bool insideSingleLineComment;
    bool insideMultilineComment;
    bool insideFloat;
    bool insideQuote;
} Lexer;

TokenInfo CreateTokenInfo(const u32 line, const u32 column, const char* identifier);
Token* Tokenize(const char* source);
