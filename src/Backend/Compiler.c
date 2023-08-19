#include "Compiler.h"

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include "Core/Log.h"
#include "Core/Utils.h"

#include <stb_ds.h>

int CompileSourceCode(const char* outputPath, const char* sourcePath)
{
    LOG_INFO("%s -> %s", sourcePath, outputPath);
    return 0;
}
