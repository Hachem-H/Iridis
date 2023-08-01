#pragma once

#include <vector>
#include <memory>
#include <string>

namespace Iridis
{
    enum class BuiltinType
    {
        I64, I32, I16, I8,
        U64, U32, U16, U8,

        String, Character, Boolean,
    };

    struct BasicArgument
    {
        std::string name;
        BuiltinType type;
    };

    struct ExpressionAST
    {
        virtual ~ExpressionAST() = default;
    };


    struct StructureAST
    {
        StructureAST(const std::wstring& name,
                     std::vector<BasicArgument> members)
            : name(name), members(std::move(members)) { }

        std::wstring name;
        std::vector<BasicArgument> members;
    };

    struct ProcedureAST
    {
        ProcedureAST(const std::wstring& name, 
                     std::vector<BasicArgument> arguments)
            : name(name), arguments(std::move(arguments)) { }

        std::wstring name;
        std::vector<BasicArgument> arguments;
    };
};
