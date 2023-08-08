#include "Backend/Token.h"
#include "Backend/Lexer.h"

#include <stb_ds.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char* ReadFile(const char* path)
{
    FILE* file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    size_t bufferSize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(bufferSize);
    fread(buffer, bufferSize, 1, file);
    buffer[bufferSize] = 0;

    fclose(file);
    return buffer;
}

int main(void)
{
    char* sourceCode = ReadFile("Source.iridis");
    Token* tokens = Tokenize(sourceCode);
    for (int i = 0; i < stbds_arrlen(tokens); i++)
        printf("%s : %s\n", tokens[i].representation, StringFromTokenType(tokens[i].type));
    DestroyTokens(tokens);
    free(sourceCode);
}
