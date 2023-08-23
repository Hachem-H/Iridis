#include "Compiler.h"

#include "AST.h"
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include "Core/Log.h"
#include "Core/Utils.h"

#include <llvm-c/Core.h>
#include <stb_ds.h>

i32 CompileSourceCode(const char* outputPath, const char* sourcePath)
{
    char* sourceCode = ReadFile(sourcePath);
    if (!sourceCode)
        return -1;
    
    Parser parser = Parse(sourceCode);
    DestroyParser(&parser);

    free(sourceCode);
    return 0;
}
