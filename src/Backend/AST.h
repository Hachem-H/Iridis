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

struct Node
{
    NodeType type;

    union
    {
        struct { f64 value;   } number;
        struct { char* value; } string;
        struct { char* name;  } variable;

        struct { Node* left;
                 Node* right;
                 TokenType operator; } binaryExpression;
        
        struct { Node* prototype;
                 Node** body;      } procedure;
        struct { char* callee;
                 Node** arguments; } procedureCall;
        struct { char* name;
                 char** arguments; } procedurePrototype;
    };
};

Node* CreateNumber(f64 number);
Node* CreateString(char* string);
Node* CreateVarible(char* name);

Node* CreateBinaryExpression(TokenType operator, Node* right, Node* left);

Node* CreateProcedure(Node* prototype, Node** body);
Node* CreateProcedureCall(char* callee, Node** arguments);
Node* CreateProcedurePrototype(char* name, char** arguments);

void DestroyNode(Node* node);
