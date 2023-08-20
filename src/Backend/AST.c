#include "AST.h"

#include <stdlib.h>

void DestroyNode(Node* node)
{
    switch (node->type)
    {
    case IntegerDeclaration: 
    {
        free(node->integerDeclaration.name); 
    } break;

    case FloatDeclaration: 
    {
        free(node->floatDeclaration.name); 
    } break;

    case StringDeclaration:
    {
        free(node->stringDeclaration.name);
        free(node->stringDeclaration.value);
    } break;

    case ProcedureNode:
    {
        free(node->procedureDeclaration.name);
    } break;

    case StructureNode:
    {
        free(node->structDeclaration.name);
    } break;

    default: break;
    }
}
