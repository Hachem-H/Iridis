#pragma once

typedef enum NodeType_t
{
    IntegerDeclaration,
    StringDeclaration,
    FloatDeclaration,

    ProcedureNode,
    StructureNode,
} NodeType;

typedef struct Node_t
{
    union
    {
        struct
        {
            char* name;
            int value;
        } integerDeclaration;

        struct
        {
            char* name;
            float value;
        } floatDeclaration;

        struct
        {
            char* name;
            char* value;
        } stringDeclaration;

        struct
        {
            char* name;
        } procedureDeclaration;

        struct
        {
            char* name;
        } structDeclaration;
    };
    
    NodeType type;
} Node;

void DestroyNode(Node* node);
