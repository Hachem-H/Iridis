#include "Token.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool IsInteger(const char* buffer)
{
    for (int i = 0; buffer[i] != 0x00; i++)
        if (!isdigit(buffer[i]))
            return false;
    return true; 
}

static bool IsFloat(const char* buffer)
{
    char* endPointer;
    strtod(buffer, &endPointer);

    if (*endPointer != 0x00)
        return false;
    return true;
}

Token TokenFromString(int line, int column, const char* buffer)
{
    Token token;
    token.line   = line;
    token.column = column;

    token.representation = (char*) malloc(strlen(buffer)+1);
    token.representation[strlen(buffer)] = 0;
    strcpy(token.representation, buffer);

    if (IsInteger(buffer))
    {
        token.type =  TokenType_Integer;
        token.literal.integer = atoi(buffer);
        return token;
    }

    if (IsFloat(buffer))
    {
        token.type = TokenType_Float;
        token.literal.floating = atof(buffer);
        return token;
    }

         if (strcmp(buffer, "[")       == 0) token.type = TokenType_RBracket;
    else if (strcmp(buffer, "]")       == 0) token.type = TokenType_LBracket;
    else if (strcmp(buffer, "{")       == 0) token.type = TokenType_RBrace;
    else if (strcmp(buffer, "}")       == 0) token.type = TokenType_LBrace;
    else if (strcmp(buffer, "(")       == 0) token.type = TokenType_RParen;
    else if (strcmp(buffer, ")")       == 0) token.type = TokenType_LParen;
    else if (strcmp(buffer, "<")       == 0) token.type = TokenType_RAngle;
    else if (strcmp(buffer, ">")       == 0) token.type = TokenType_LAngle;
    else if (strcmp(buffer, "+")       == 0) token.type = TokenType_Plus;
    else if (strcmp(buffer, "-")       == 0) token.type = TokenType_Minus;
    else if (strcmp(buffer, "*")       == 0) token.type = TokenType_Asterisk;
    else if (strcmp(buffer, "/")       == 0) token.type = TokenType_Slash;
    else if (strcmp(buffer, ":")       == 0) token.type = TokenType_Colon;
    else if (strcmp(buffer, ";")       == 0) token.type = TokenType_SemiColon;
    else if (strcmp(buffer, "^")       == 0) token.type = TokenType_Caret;
    else if (strcmp(buffer, "&")       == 0) token.type = TokenType_Ampersand;
    else if (strcmp(buffer, "!")       == 0) token.type = TokenType_Bang;
    else if (strcmp(buffer, ",")       == 0) token.type = TokenType_Comma;
    else if (strcmp(buffer, ".")       == 0) token.type = TokenType_Period;
    else if (strcmp(buffer, "enum")    == 0) token.type = TokenType_Enumeration;
    else if (strcmp(buffer, "struct")  == 0) token.type = TokenType_Structure;
    else if (strcmp(buffer, "proc")    == 0) token.type = TokenType_Procedure;
    else if (strcmp(buffer, "extern")  == 0) token.type = TokenType_External;
    else if (strcmp(buffer, "mod")     == 0) token.type = TokenType_Module;
    else
    {
        token.type = TokenType_Identifier;
        token.literal.identifier = token.representation;
    }

    return token;
}

void DestroyToken(Token* token)
{
    free(token->representation);
}
