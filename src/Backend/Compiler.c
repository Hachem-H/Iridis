#include "Compiler.h"

#include "Token.h"
#include "Lexer.h"

#include "Core/Log.h"
#include <stb_ds.h>

int CompileSourceCode(const char* sourceCode)
{
    Token* tokens = Tokenize(sourceCode);
    for (int i = 0; i < arrlen(tokens); i++) 
    {
        TokenType type = tokens[i].type;
        if (type == TokenType_Identifier || type == TokenType_String)
        {
            LOG_INFO("%s(%s)", StringFromTokenType(type), tokens[i].literal.identifier)
        }
        else if (type == TokenType_Integer)
        {
            LOG_INFO("%s(%d)", StringFromTokenType(type), tokens[i].literal.integer);
        }
        else if (type == TokenType_Float)
        {
            LOG_INFO("%s(%f)", StringFromTokenType(type), tokens[i].literal.floating)
        }
        else LOG_INFO("%s", StringFromTokenType(type));
    }

    DestroyTokens(tokens);

    return 0;
}
