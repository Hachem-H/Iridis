#include "Token.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stb_ds.h>

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

static void ProcessStringLiteral(char* string)
{
    char* source = string;
    char* destination = string;

    while (*source)
    {
        if (*source == '\\' && *(source + 1))
        {
            source++;
            switch (*source)
            {
                case 'n':
                    *destination = '\n';
                    break;
                case 't':
                    *destination = '\t';
                    break;
                case 'b':
                    *destination = '\b';
                    break;
                case 'r':
                    *destination = '\r';
                    break;
                case '"':
                    *destination = '\"';
                    break;
                case '\'':
                    *destination = '\'';
                    break;
                default:
                    *destination = '\\';
                    destination++;
                    continue;
            }
        } else
            *destination = *source;
        source++;
        destination++;
    }

    *destination = '\0'; 
}

Token TokenFromString(int line, int column, const char* buffer)
{
    Token token;
    token.line   = line;
    token.column = column;
    token.representation = strdup(buffer);

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

    if      (strcmp(buffer, "[")       == 0) token.type = TokenType_RBracket;
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
    else if (strcmp(buffer, "=")       == 0) token.type = TokenType_Equal;
    else if (strcmp(buffer, ",")       == 0) token.type = TokenType_Comma;
    else if (strcmp(buffer, ".")       == 0) token.type = TokenType_Period;
    else if (strcmp(buffer, "'")       == 0) token.type = TokenType_Quote;
    else if (strcmp(buffer, "\"")      == 0) token.type = TokenType_DoubleQuote;

    else if (strcmp(buffer, "enum")    == 0) token.type = TokenType_Enumeration;
    else if (strcmp(buffer, "struct")  == 0) token.type = TokenType_Structure;
    else if (strcmp(buffer, "proc")    == 0) token.type = TokenType_Procedure;
    else if (strcmp(buffer, "extern")  == 0) token.type = TokenType_External;
    else if (strcmp(buffer, "mod")     == 0) token.type = TokenType_Module;

    else if (buffer[0] == '"')
    {
        token.type = TokenType_String;
        token.literal.identifier = strdup(token.representation+1);
        ProcessStringLiteral(token.literal.identifier);
    }

    else
    {
        token.type = TokenType_Identifier;
        token.literal.identifier = token.representation;
    }

    return token;
}

char* StringFromTokenType(TokenType type)
{
    switch (type)
    {
    case TokenType_Identifier:  return "Identifier";
    case TokenType_Integer:     return "Integer";
    case TokenType_String:      return "String";
    case TokenType_Float:       return "Float";

    case TokenType_Enumeration: return "Enumeration";
    case TokenType_Procedure:   return "Procedure";
    case TokenType_Structure:   return "Structure";
    case TokenType_External:    return "External";
    case TokenType_Module:      return "Module";

    case TokenType_RBracket:    return "RBracket";
    case TokenType_LBracket:    return "LBracket";
    case TokenType_RBrace:      return "RBrace";
    case TokenType_LBrace:      return "LBrace";
    case TokenType_RParen:      return "RParen";
    case TokenType_LParen:      return "LParen";
    case TokenType_RAngle:      return "RAngle";
    case TokenType_LAngle:      return "LAngle";

    case TokenType_Colon:       return "Colon";
    case TokenType_SemiColon:   return "SemiColon";
    case TokenType_Caret:       return "Caret";
    case TokenType_Ampersand:   return "Ampersand";
    case TokenType_Bang:        return "Bang";
    case TokenType_Quote:       return "Quote";
    case TokenType_DoubleQuote: return "DoubleQuote";
    case TokenType_Equal:       return "Equal";
    case TokenType_Comma:       return "Comma";
    case TokenType_Period:      return "Period";

    case TokenType_Plus:        return "Plus";
    case TokenType_Minus:       return "Minus";
    case TokenType_Asterisk:    return "Asterisk";
    case TokenType_Slash:       return "Slash";
    }
}

void DestroyToken(Token* token)
{
    free(token->representation);
}

void DestroyTokens(Token* tokens)
{
    for (int i = 0; i < stbds_arrlen(tokens); i++)
        DestroyToken(&tokens[i]);
    stbds_arrfree(tokens);
}

