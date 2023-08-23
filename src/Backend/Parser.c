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

internal i32 ReadNextToken(Parser* parser, i32 advance)
{
    if (parser->currentTokenIndex+advance >= stbds_arrlen(parser->tokens))
        return -1;
    return parser->tokens[parser->currentTokenIndex+advance].type;
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
    printf("^ %shere%s ", CONSOLE_MODE_UNDERLINE, CONSOLE_MODE_RESET);
}

Node* ParseNumberExpression(Parser* parser)
{
    Node* result = CreateNumber(GetToken(parser)->literal.number);
    GetNextToken(parser);
    return result;
}

Node* ParseParenExpression(Parser* parser)
{
    GetNextToken(parser);
    Node* expression = ParseExpression(parser); 
    if (!expression)
        return NULL;
    if (GetToken(parser)->type != TokenType_RParen)
    {
        LOG_ERROR("Unexpected `%s` in expression.", GetToken(parser)->representation);
        PrintErrorLocation(parser);
        puts("Did you mean to close the expression with a `)`?");
        return NULL;
    }
    GetNextToken(parser);
    return expression;
}

Node* ParseIdentifierExpression(Parser* parser)
{
    char* name = GetToken(parser)->literal.identifier;
    GetNextToken(parser);

    if (GetToken(parser)->type != TokenType_LParen)
        return CreateVarible(name);
    GetNextToken(parser);

    Node** arguments = NULL;
    if (GetToken(parser)->type != TokenType_RParen)
    {
        while (true)
        {
            Node* argument = ParseExpression(parser);
            if (argument != NULL)
                stbds_arrpush(arguments, argument);
            else
                return NULL;

            if (GetToken(parser)->type == TokenType_LParen)
                break;

            if (GetToken(parser)->type != TokenType_Comma)
            {
                LOG_ERROR("Unexpected `%s` in argument list.", GetToken(parser)->representation);
                PrintErrorLocation(parser);
                puts("You can either close the argument list with `)` or add to it using `,`");
                return NULL;
            }

            GetNextToken(parser);
        }
    }

    GetNextToken(parser);
    return CreateProcedureCall(name, arguments);
}

Node* ParseBinaryExpression(Parser* parser, i32 expressionPrecedence, Node* left)
{
    while (true)
    {
        i32 precedence = GetTokenPrecedence(GetToken(parser)->type);
        if (precedence < expressionPrecedence)
            return left;
    
        TokenType operator = GetToken(parser)->type;
        GetNextToken(parser);

        Node* right = ParsePrimary(parser);
        if (!right)
            return NULL;

        i32 nextPrecedence = GetTokenPrecedence(GetToken(parser)->type);
        if (precedence < nextPrecedence)
        {
            right = ParseBinaryExpression(parser, precedence+1, right);
            if (!right)
                return NULL;
        }

        left = CreateBinaryExpression(operator, right, left);
    }
}

Node* ParsePrimary(Parser* parser)
{
    switch (GetToken(parser)->type)
    {
    case TokenType_LParen:     return ParseParenExpression(parser);
    case TokenType_Number:     return ParseNumberExpression(parser);
    case TokenType_Identifier: return ParseIdentifierExpression(parser);

    default: 
    {
        LOG_ERROR("Unexpected token `%s`.", GetToken(parser)->representation);
        PrintErrorLocation(parser);
        return NULL;
    };
    }
}

Node* ParseExpression(Parser* parser)
{
    Node* left = ParsePrimary(parser);
    if (!left)
        return NULL;
    return ParseBinaryExpression(parser, 0, left);
}

Node* ParsePrototype(Parser* parser, char* name)
{
    GetNextToken(parser);
    if (GetToken(parser)->type != TokenType_LParen)
    {
        LOG_ERROR("Unexpected `%s` in procedure prototype.", GetToken(parser)->representation);
        PrintErrorLocation(parser);
        puts("You have to give the procedure arguments, you do so by stating them inside the prototype.");
        return NULL;
    }
    
    char** arguments = NULL;
    while (GetNextToken(parser)->type == TokenType_Identifier)
        stbds_arrpush(arguments, GetToken(parser)->literal.identifier);

    if (GetToken(parser)->type != TokenType_LParen)
    {
        LOG_ERROR("Unexpected `%s` in procedure prototype.", GetToken(parser)->representation);
        PrintErrorLocation(parser);
        puts("You opened an argument list in procedure prototype, you have to close it with `)`");
        return NULL;
    }

    GetNextToken(parser);
    Node* prototype = CreateProcedurePrototype(name, arguments);
    return prototype;
}

Node* ParseProcedure(Parser* parser, char* name)
{
    GetNextToken(parser);
    Node* prototype = ParsePrototype(parser, name);
    if (prototype == NULL)
        return NULL;

    if (GetToken(parser)->type != TokenType_LBrace)
    {
        LOG_ERROR("Missing procedure body");
        PrintErrorLocation(parser);
        puts("A procedure needs a body, you can give it one by opening a scope.");
        return NULL;
    }

    Node** body = NULL;
    while (GetNextToken(parser)->type != TokenType_RBrace)
    {
        Node* expression = ParsePrimary(parser);
        stbds_arrpush(body, expression);
    }

    Node* procedure = CreateProcedure(prototype, body);
    return procedure;
}

internal void HandleIdentifier(Parser* parser)
{
    char* identifier = strdup(GetToken(parser)->literal.identifier);

    if (ReadNextToken(parser, 1) != TokenType_Colon)
    {
        Node* expression = ParseIdentifierExpression(parser);
        if (expression)
        {
            LOG_DEBUG("Variable");
            DestroyNode(expression);
        }
        else
            GetNextToken(parser);
    }

    if (GetToken(parser)->type != TokenType_Colon &&
        GetToken(parser)->type != TokenType_Equal)
    {
        LOG_ERROR("Expected `:` or `=` after identifer.");
        PrintErrorLocation(parser);
        puts("You should add `:` to define a constant or a `=` for a variable.");
    }

    if (GetToken(parser)->type == TokenType_Procedure)
    {
        Node* expression = ParseProcedure(parser, identifier);
        if (expression)
        {
            LOG_DEBUG("Procedure Declaration");
            DestroyNode(expression);
        }
        else
            GetNextToken(parser);

    }

    free(identifier);
    GetNextToken(parser);
}

Parser Parse(char* sourceCode)
{
    Parser parser;
    parser.tokens = Tokenize(sourceCode);
    parser.sourceLines = NULL;
    parser.currentTokenIndex = 0;

    char* sourceLine = strtok(sourceCode, "\n");
    while (sourceLine != NULL)
    {
        stbds_arrpush(parser.sourceLines, sourceLine);
        sourceLine = strtok(NULL, "\n");
    }

    for (Token* token = GetToken(&parser); token != NULL;)
    {
        switch (token->type)
        {
        case TokenType_Identifier:
        {
            HandleIdentifier(&parser);
        } break;

        default: break;
        }

        token = GetNextToken(&parser);
    }

    return parser;
}

void DestroyParser(Parser* parser)
{
    stbds_arrfree(parser->sourceLines);
    DestroyTokens(parser->tokens);
}

