#include "Parser.h"
#include "AST.h"

#include <stdio.h>
#include <stb_ds.h>

#include <llvm-c/ExecutionEngine.h>

static Token* GetToken(Parser* parser)
{
    return &parser->tokens[parser->currentTokenIndex];
}

static Token* GetNextToken(Parser* parser)
{
    if (parser->currentTokenIndex+1 >= stbds_arrlen(parser->tokens))
        return NULL;

    parser->currentTokenIndex++;
    return GetToken(parser);
}

static void PrintErrorLocation(Parser* parser)
{
    uint32_t line   = GetToken(parser)->line;
    uint32_t column = GetToken(parser)->column;

    int numDigitsInLine = 0;
    for (int number = line; number != 0;)
    {
        number /= 10;
        numDigitsInLine++;
    }

    printf("%s%d%s: %s\n", CONSOLE_COLORS_GREEN, line, CONSOLE_MODE_RESET, parser->sourceLines[line-1]);
    for (uint32_t i = 0; i < numDigitsInLine+column+1; i++)
        printf(" ");
    printf("^ %shere%s", CONSOLE_MODE_UNDERLINE, CONSOLE_MODE_RESET);
}

static void HandleIdentifier(Parser* parser)
{
    char* identifierName = strdup(GetToken(parser)->literal.identifier);
    GetNextToken(parser);

    if (GetNextToken(parser)->type != TokenType_Colon &&
        GetNextToken(parser)->type != TokenType_Equal)
    {
        uint32_t line   = GetToken(parser)->line;
        uint32_t column = GetToken(parser)->column;

        LOG_ERROR("Expected `=` or `:` after type declaration @ Line: %d, Column: %d", line, column);
        PrintErrorLocation(parser);
        printf(": Did you mean `=` or `:` instead of `%s`?\n", GetToken(parser)->representation);
        return;
    }
     
    TokenType currentTokenType = GetNextToken(parser)->type;

    switch (currentTokenType)
    {
    case TokenType_Procedure:
    {
        ExpressionAST expression;
        expression.procedure.name = strdup(identifierName);
        
        LLVMTypeRef returnType = LLVMVoidType();

        LLVMTypeRef functionType = LLVMFunctionType(returnType, NULL, 0, 0);
        LLVMValueRef entryPointFunction = LLVMAddFunction(parser->module, expression.procedure.name, functionType);
         
        LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(entryPointFunction, "entry");
        LLVMPositionBuilderAtEnd(parser->builder, entryBlock);
        free(expression.procedure.name);
    } break;


    case TokenType_Structure:
    {
        ExpressionAST expression;
        expression.structure.name = strdup(identifierName);
        LLVMStructSetBody(LLVMStructCreateNamed(LLVMGetGlobalContext(), expression.structure.name), NULL, 0, false);
        free(expression.structure.name);
    } break;

    default: 
    {
        LOG_INFO("%s", StringFromTokenType(currentTokenType));
    } break;
    }

    free(identifierName);
}

void Parse(char* sourceCode)
{
    // TODO(Hachem): Proper backend
    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    LLVMInitializeX86AsmParser();
    LLVMInitializeX86AsmPrinter();

    Parser parser;
    parser.tokens            = Tokenize(sourceCode);
    parser.sourceLines       = NULL;
    parser.currentTokenIndex = 0;

    parser.builder            = LLVMCreateBuilder();
    parser.module             = LLVMModuleCreateWithName("Test");
    
    char* sourceLine = strtok(sourceCode, "\n");
    while (sourceLine != NULL)
    {
        stbds_arrpush(parser.sourceLines, sourceLine);
        sourceLine = strtok(NULL, "\n");
    }

    for (Token* token = GetToken(&parser); token != NULL;)
    {
        if (token->type == TokenType_Identifier)
            HandleIdentifier(&parser);

        token = GetNextToken(&parser);
    }

    char* generatedIR = LLVMPrintModuleToString(parser.module);
    printf("%s\n", generatedIR);
    LLVMDisposeMessage(generatedIR);
    
    stbds_arrfree(parser.sourceLines);
    DestroyTokens(parser.tokens);

    LLVMDisposeModule(parser.module);
    LLVMDisposeBuilder(parser.builder);
    LLVMShutdown();
}

