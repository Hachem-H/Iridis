#include "Parser.h"

#include <stdio.h>
#include <stb_ds.h>

internal Token* GetToken(Parser* parser)
{
    return &parser->tokens[parser->currentTokenIndex];
}

internal Token* GetNextToken(Parser* parser)
{
    if (parser->currentTokenIndex+1 >= stbds_arrlen(parser->tokens))
        return NULL;

    parser->currentTokenIndex++;
    return GetToken(parser);
}

internal void PrintErrorLocation(Parser* parser)
{
    u32 line   = GetToken(parser)->line;
    u32 column = GetToken(parser)->column;

    i32 numDigitsInLine = 0;
    for (i32 number = line; number != 0;)
    {
        number /= 10;
        numDigitsInLine++;
    }

    printf("%s%d%s: %s\n", CONSOLE_COLORS_GREEN, line, CONSOLE_MODE_RESET, parser->sourceLines[line-1]);
    for (u32 i = 0; i < numDigitsInLine+column+1; i++)
        printf(" ");
    printf("^ %shere%s", CONSOLE_MODE_UNDERLINE, CONSOLE_MODE_RESET);
}

internal void HandleIdentifier(Parser* parser)
{
    char* identifierName = strdup(GetToken(parser)->literal.identifier);
    GetNextToken(parser);

    if (GetNextToken(parser)->type != TokenType_Colon &&
        GetNextToken(parser)->type != TokenType_Equal)
    {
        u32 line   = GetToken(parser)->line;
        u32 column = GetToken(parser)->column;

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
        Node node;
        node.type = ProcedureNode;
        node.procedureDeclaration.name = strdup(identifierName);
        stbds_arrpush(parser->nodes, node);
    } break;
    
    case TokenType_Structure:
    {
        Node node;
        node.type = StructureNode;
        node.structDeclaration.name = strdup(identifierName);
        stbds_arrpush(parser->nodes, node);
    } break;

    case TokenType_Integer:
    {
        Node node;
        node.type = IntegerDeclaration;
        node.integerDeclaration.name  = strdup(identifierName);
        node.integerDeclaration.value = GetToken(parser)->literal.integer;
        stbds_arrpush(parser->nodes, node);
    } break;

    case TokenType_Float:
    {
        Node node;
        node.type = FloatDeclaration;
        node.floatDeclaration.name  = strdup(identifierName);
        node.floatDeclaration.value = GetToken(parser)->literal.floating;
        stbds_arrpush(parser->nodes, node);
    } break;

    case TokenType_String:
    {
        Node node;
        node.type = StringDeclaration;
        node.stringDeclaration.name  = strdup(identifierName);
        node.stringDeclaration.value = GetToken(parser)->literal.identifier;
        stbds_arrpush(parser->nodes, node);
    } break;

    default: 
    {
        LOG_INFO("%s", StringFromTokenType(currentTokenType));
    } break;
    }

    free(identifierName);
}

Parser Parse(char* sourceCode)
{
    Parser parser;
    parser.tokens            = Tokenize(sourceCode);
    parser.sourceLines       = NULL;
    parser.nodes             = NULL;
    parser.currentTokenIndex = 0;

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

    return parser;
}

void DestroyParser(Parser* parser)
{
    stbds_arrfree(parser->sourceLines);
    DestroyTokens(parser->tokens);
    stbds_arrfree(parser->nodes);
}
