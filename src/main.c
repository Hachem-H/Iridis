#include "Backend/Token.h"

#include <stdio.h>

static void PrintToken(Token* token)
{
    if (token->type == TokenType_Identifier)
        printf("%s\n", token->literal.identifier);
    else if (token->type == TokenType_Integer)
        printf("%d\n", token->literal.integer);
    else if (token->type == TokenType_Float)
        printf("%f\n", token->literal.floating);
    else
        puts("");
}

int main()
{
    Token tokens[5] = 
    {
        TokenFromString(0, 0, "{"),
        TokenFromString(0, 0, "proc"),
        TokenFromString(0, 0, "1234"),
        TokenFromString(0, 0, "1.234"),
        TokenFromString(0, 0, "something")
    };

    for (int i = 0 ; i < 5; i++)
    {
        printf("%d, %s, ", tokens[i].type, tokens[i].representation);
        PrintToken(&tokens[i]);
        DestroyToken(&tokens[i]);
    }
}
