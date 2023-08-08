#include "Backend/Token.h"
#include "Backend/Lexer.h"

#include <stb_ds.h>
#include <stdio.h>

static const char* const TestSourceCode = "main :: proc()";

int main(void)
{
    Token* tokens = Tokenize(TestSourceCode);
    for (int i = 0; i < stbds_arrlen(tokens); i++)
    {
        printf("%s END\n", tokens[i].representation);
        DestroyToken(&tokens[i]);
    }
}
