#include "AST.h"

#include <stdlib.h>
#include <string.h>

#include <stb_ds.h>

Node* CreateNumber(double number)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_Number;
    node->number.value = number;
    return node;
}

Node* CreateString(char* string)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_String;
    node->string.value = strdup(string);
    return node;
}

Node* CreateVarible(char* name)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_Variable;
    node->variable.name = strdup(name);
    return node;
}

Node* CreateBinaryExpression(TokenType operator, Node* right, Node* left)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_BinaryExpression;
    node->binaryExpression.operator = operator;
    node->binaryExpression.right    = right;
    node->binaryExpression.left     = left;
    return node;
}

Node* CreateProcedure(Node* prototype, Node** body)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_Procedure;
    node->procedure.body      = body;
    node->procedure.prototype = prototype;
    return node;
}

Node* CreateProcedureCall(char* callee, Node** arguments)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_ProcedureCall;
    node->procedureCall.callee    = strdup(callee);
    node->procedureCall.arguments = arguments;
    return node;
}

Node* CreateProcedurePrototype(char* name, char** arguments)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = NodeType_ProcedurePrototype;
    node->procedurePrototype.arguments = arguments;
    node->procedurePrototype.name      = strdup(name);
    return node;
}

void DestroyNode(Node* node)
{
    switch (node->type)
    {
    case NodeType_Program: break;
    case NodeType_Number:  break;

    case NodeType_String:
    {
        free(node->string.value);
    } break;

    case NodeType_Variable:
    {
        free(node->variable.name);
    } break;

    case NodeType_BinaryExpression:
    {
        DestroyNode(node->binaryExpression.left);
        DestroyNode(node->binaryExpression.right);
    } break;

    case NodeType_Procedure:
    {
        for (i32 i = 0; i < stbds_arrlen(node->procedure.body); i++)
            DestroyNode(node->procedure.body[i]);
        stbds_arrfree(node->procedure.body);
        DestroyNode(node->procedure.prototype);
    } break;

    case NodeType_ProcedureCall:
    {
        for (i32 i = 0; i < stbds_arrlen(node->procedureCall.arguments); i++)
            DestroyNode(node->procedureCall.arguments[i]);
        stbds_arrfree(node->procedureCall.arguments);
        free(node->procedureCall.callee);
    } break;

    case NodeType_ProcedurePrototype:
    {
        for (i32 i = 0; i < stbds_arrlen(node->procedurePrototype.arguments); i++)
            free(node->procedurePrototype.arguments[i]);
        stbds_arrfree(node->procedurePrototype.arguments);
        free(node->procedurePrototype.name);
    } break;

    default: break;
    };

    free(node);
}
