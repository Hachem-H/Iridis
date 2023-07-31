#pragma once

#include <vector>
#include <memory>
#include <string>

namespace Iridis
{
    struct ExpressionAST
    {
        virtual ~ExpressionAST() = default;
    };

    class NumberAST
        : public ExpressionAST
    {
    public:
        NumberAST(double value)
            : value(value) { }
    private:
        double value;
    };

    class VariableAST
        : public ExpressionAST
    {
    public:
        VariableAST(const std::wstring& name)
            : name(name) { }
    private:
        std::wstring name;
    };

    class BinaryExpressionAST
        : public ExpressionAST
    {
    public:
        BinaryExpressionAST(char op, std::unique_ptr<ExpressionAST> leftHandSide,
                            std::unique_ptr<ExpressionAST> rightHandSide) 
            : op(op), leftHandSide(std::move(leftHandSide)),
                      rightHandSide(std::move(rightHandSide)) { }
    private:
        char op;
        std::unique_ptr<ExpressionAST> leftHandSide;
        std::unique_ptr<ExpressionAST> rightHandSide;
    };

    class ProcedureCallAST
        : public ExpressionAST
    {
    public:
        ProcedureCallAST(const std::string& callee,
                         std::vector<std::unique_ptr<ExpressionAST>> arguments)
            : callee(callee), arguments(std::move(arguments)) { }

    private:
        std::vector<std::unique_ptr<ExpressionAST>> arguments;
        std::string callee;
    };

    class ProcedurePrototypeAST
    {
    public:
        ProcedurePrototypeAST(const std::wstring& name,
                              const std::vector<std::wstring> arguments)
            : name(name), arguments(std::move(arguments)) { }

        inline const std::wstring& GetName() const { return name; }
    private:
        std::wstring name;
        std::vector<std::wstring> arguments;
    };

    class ProcedureAST
    {
    public:
        ProcedureAST(std::unique_ptr<ProcedurePrototypeAST> prototype,
                     std::unique_ptr<ExpressionAST> body)
            : prototype(std::move(prototype)), body(std::move(body)) { }
    private:
        std::unique_ptr<ProcedurePrototypeAST> prototype;
        std::unique_ptr<ExpressionAST> body;
    };
};
