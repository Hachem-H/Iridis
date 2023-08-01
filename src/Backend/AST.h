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

    class ProcedureCallAST
        : public ExpressionAST
    {
    public:
        ProcedureCallAST(const std::string& callee,
                         std::vector<std::unique_ptr<ExpressionAST>> arguments)
            : callee(callee), arguments(std::move(arguments)) { }

    private:
        std::string callee;
        std::vector<std::unique_ptr<ExpressionAST>> arguments;
    };

    class ProcedureAST
    {
    public:
        ProcedureAST(const std::wstring& name, 
                     std::vector<BasicArgument> arguments)
            : name(name), arguments(std::move(arguments)) { }
    public:
        std::wstring name;
        std::vector<BasicArgument> arguments;
    };
};
