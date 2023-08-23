#pragma once

#include "Common.h"
#include "Token.h"

typedef struct Node Node;

typedef enum NodeType_t
{
    NodeType_Program,

    NodeType_Number,
    NodeType_String,
    NodeType_Variable,

    NodeType_BinaryExpression,

    NodeType_Procedure,
    NodeType_ProcedureCall,
    NodeType_ProcedurePrototype,
} NodeType;

typedef struct NumberNode
{
    double value;
} NumberNode;

typedef struct StringNode
{
    char* value;
} StringNode;

typedef struct VariableNode
{
    char* name;
} VariableNode;

typedef struct BinaryExpressionNode_t
{
    Node* left;
    Node* right;
    TokenType operator;
} BinaryExpressionNode;

typedef struct ProcedureCallNode_t
{
    char* callee;
    Node** arguments;
} ProcedureCallNode;

typedef struct ProcedurePrototypeNode_t
{
    char* name;
    char** arguments;
} ProcedurePrototypeNode;

typedef struct ProcedureNode_t
{
    Node* prototype;
    Node** body;
} ProcedureNode;

struct Node
{
    NodeType type;

    union
    {
        NumberNode   number;
        StringNode   string;
        VariableNode variable;

        BinaryExpressionNode binaryExpression;
        
        ProcedureNode          procedure;
        ProcedureCallNode      procedureCall;
        ProcedurePrototypeNode procedurePrototype;
    };
};

Node* CreateNumber(double number);
Node* CreateString(char* string);
Node* CreateVarible(char* name);

Node* CreateBinaryExpression(TokenType operator, Node* right, Node* left);

Node* CreateProcedure(Node* prototype, Node** body);
Node* CreateProcedureCall(char* callee, Node** arguments);
Node* CreateProcedurePrototype(char* name, char** arguments);

void DestroyNode(Node* node);
