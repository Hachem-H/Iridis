#include "Compiler.h"

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include "Core/Log.h"
#include <stb_ds.h>

int CompileSourceCode(const char* sourceCode)
{
    Parse((char*) sourceCode);
    return 0;
}
