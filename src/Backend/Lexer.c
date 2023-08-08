#include "Lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <stb_ds.h>

static Lexer LexingState = (Lexer)
{
    .tokenInfos = NULL,
    .buffer     = NULL,

    .currentLine      = 0,
    .currentColumn    = 0,
    .tokenStartLine   = 0,
    .floatStartIndex  = 0,
    .tokenStartColumn = 0,

    .insideSingleLineComment = false,
    .insideMultilineComment  = false,
    .insideQuote             = false,
    .insideFloat             = false,
};

TokenInfo CreateTokenInfo(const uint32_t line, const uint32_t column, const char* identifier)
{
    TokenInfo tokenInfo;
    tokenInfo.line   = line;
    tokenInfo.column = column;
    tokenInfo.identifier = (char*) malloc(sizeof(identifier));
    strcpy(tokenInfo.identifier, identifier);
    return tokenInfo;
}

Token* Tokenize(const char* source)
{
    for (size_t i = 0; i < strlen(source); i++)
    {
        char character = source[i];
        char nextCharacter = (i+1 < strlen(source)) ? source[i+1] : 0x00;

        if (isdigit(character) || (character == '.' && isdigit(nextCharacter)))
        {
            if (!LexingState.insideFloat)
            {
                LexingState.insideFloat = true;
                LexingState.floatStartIndex = i;
            }
        }
        else
        {
            if (LexingState.insideFloat)
            {
                LexingState.insideFloat = false;
                size_t floatLength = i - LexingState.floatStartIndex;
                char* floatBuffer = (char*)malloc(floatLength + 1);
                strncpy(floatBuffer, source + LexingState.floatStartIndex, floatLength);
                floatBuffer[floatLength] = '\0';
                stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.tokenStartLine, LexingState.tokenStartColumn, floatBuffer));
                free(floatBuffer);
            }

            if (character == '\n')
            {
                LexingState.currentLine++;
                LexingState.currentColumn = 1;

                if (stbds_arrlen(LexingState.buffer) != 0)
                {
                    stbds_arrpush(LexingState.buffer, 0);
                    stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, LexingState.buffer));
                    stbds_arrsetlen(LexingState.buffer, 0);
                    LexingState.buffer[0] = 0;
                }

                if (LexingState.insideSingleLineComment)
                    LexingState.insideSingleLineComment = false;
                continue;
            }

            if (LexingState.insideMultilineComment)
            {
                if (character == '*' && nextCharacter == '/')
                {
                    LexingState.insideMultilineComment = false;
                    LexingState.currentColumn += 2;
                    i++;
                }
                else
                    LexingState.currentColumn++;
                continue;
            }

            if (LexingState.insideSingleLineComment)
                continue;

            if (character == '/' && nextCharacter == '*')
            {
                LexingState.insideMultilineComment = true;
                LexingState.currentColumn += 2;
                i++;
                continue;
            }

            if (character == '/' && nextCharacter == '/')
            {
                LexingState.insideSingleLineComment = true;
                LexingState.currentColumn += 2;
                i++;
                continue;
            }

            if (LexingState.insideQuote)
            {
                stbds_arrpush(LexingState.buffer, character);

                if (character == '"' && nextCharacter != '\\')
                {
                    stbds_arrpush(LexingState.buffer, 0);
                    stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, LexingState.buffer));
                    stbds_arrsetlen(LexingState.buffer, 0);
                    LexingState.buffer[0] = 0;
                    LexingState.insideQuote = false;
                }
                else if (character == '\\' && nextCharacter == '"')
                {
                    stbds_arrpush(LexingState.buffer, nextCharacter);
                    i++;
                }

                LexingState.currentColumn++;
                continue;
            }

            if (character == '"')
            {
                LexingState.insideQuote = true;

                if (stbds_arrlen(LexingState.buffer) != 0)
                {
                    stbds_arrpush(LexingState.buffer, 0);
                    stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, LexingState.buffer));
                    stbds_arrsetlen(LexingState.buffer, 0);
                    LexingState.buffer[0] = 0;
                }

                stbds_arrpush(LexingState.buffer, character);
                LexingState.currentColumn++;
                continue;
            }

            if (isspace(character) || ispunct(character))
            {
                if (stbds_arrlen(LexingState.buffer) != 0)
                {
                    stbds_arrpush(LexingState.buffer, 0);
                    stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, LexingState.buffer));
                    stbds_arrsetlen(LexingState.buffer, 0);
                    LexingState.buffer[0] = 0;
                }

                if (ispunct(character))
                {
                    char* toPush = (char*) malloc(2);
                    toPush[0] = character;
                    toPush[1] = 0;
                    stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, toPush));
                    free(toPush);
                }

                LexingState.tokenStartLine = LexingState.currentLine;
                LexingState.tokenStartColumn = LexingState.currentColumn+1;
            }
            else
            {
                if (stbds_arrlen(LexingState.buffer) != 0)
                {
                    LexingState.tokenStartLine = LexingState.currentLine;
                    LexingState.tokenStartColumn = LexingState.currentColumn;
                }

                stbds_arrpush(LexingState.buffer, character);
            }
        }
        LexingState.currentColumn++;
    }

    if (stbds_arrlen(LexingState.buffer) != 0)
    {
        stbds_arrpush(LexingState.buffer, 0);
        stbds_arrpush(LexingState.tokenInfos, CreateTokenInfo(LexingState.currentLine, LexingState.currentColumn, LexingState.buffer));
    }
    
    Token* tokens = NULL;
    for (int i = 0; i < stbds_arrlen(LexingState.tokenInfos); i++)
    {
        uint32_t line   = LexingState.tokenInfos[i].line;
        uint32_t column = LexingState.tokenInfos[i].column;
        char* buffer    = LexingState.tokenInfos[i].identifier;
        Token token = TokenFromString(line, column, buffer);
        stbds_arrpush(tokens, token);
    }

    for (int i = 0; i < stbds_arrlen(LexingState.tokenInfos); i++)
        free(LexingState.tokenInfos[i].identifier);

    stbds_arrfree(LexingState.tokenInfos);
    stbds_arrfree(LexingState.buffer);

    return tokens;
}
