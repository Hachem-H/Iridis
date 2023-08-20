#include "Compiler.h"

#include "AST.h"
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include "Core/Log.h"
#include "Core/Utils.h"

#include <llvm-c/Core.h>
#include <stb_ds.h>

int CompileSourceCode(const char* outputPath, const char* sourcePath)
{
    LLVMContextRef context = LLVMGetGlobalContext();
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMModuleRef module   = LLVMModuleCreateWithNameInContext(GetFileName(sourcePath), context);

    char* sourceCode = ReadFile(sourcePath);
    if (!sourceCode)
        return -1;
    
    Parser parser = Parse(sourceCode);
    free(sourceCode);

    for (int i = 0; i < stbds_arrlen(parser.nodes); i++)
    {
        Node node = parser.nodes[i];
    
        switch (node.type)
        {
        case IntegerDeclaration:
        {
            LLVMValueRef value = LLVMAddGlobal(module, LLVMInt32TypeInContext(context), node.integerDeclaration.name);
            LLVMSetInitializer(value, LLVMConstInt(LLVMInt32TypeInContext(context), node.integerDeclaration.value, false));
        } break;

        case StringDeclaration:
        {
            LLVMValueRef value = LLVMAddGlobal(module, LLVMArrayType(LLVMInt8TypeInContext(context), strlen(node.stringDeclaration.value)+1), node.stringDeclaration.name);
            LLVMSetInitializer(value, LLVMConstString(node.stringDeclaration.value, strlen(node.stringDeclaration.value), false));
        } break;

        case FloatDeclaration:
        {
            LLVMValueRef value = LLVMAddGlobal(module, LLVMFloatTypeInContext(context), node.floatDeclaration.name);
            LLVMSetInitializer(value, LLVMConstReal(LLVMFloatTypeInContext(context), node.floatDeclaration.value));
        } break;

        case ProcedureNode:
        {
            LLVMTypeRef functionType = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
            LLVMValueRef value = LLVMAddFunction(module, node.procedureDeclaration.name, functionType);
            LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(value, "entry");
            LLVMPositionBuilderAtEnd(builder, entryBlock);
        } break;

        case StructureNode:
        {
            LLVMStructSetBody(LLVMStructCreateNamed(context, node.structDeclaration.name), NULL, 0, false);
        } break;

        default: break;
        }

        DestroyNode(&node);
    }

    DestroyParser(&parser);

    char* generatedIR = LLVMPrintModuleToString(module);
    FILE* output = fopen(outputPath, "w");
    fputs(generatedIR, output);
    puts(generatedIR);
    fclose(output);

    LLVMDisposeMessage(generatedIR);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMShutdown();

    return 0;
}
