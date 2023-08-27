#include "Compiler.h"

#include "AST.h"
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include "Core/Log.h"
#include "Core/FS.h"

#include <stb_ds.h>

i32 CompileSourceCode(const char* outputPath, const char* sourcePath)
{
    char* sourceCode = ReadFileContents(sourcePath);
    if (!sourceCode)
        return -1;
    
    Parser parser = Parse(sourceCode);
    DestroyParser(&parser);

    free(sourceCode);
    return 0;
}
