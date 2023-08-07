#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Token.h"

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
    
    template<typename T>
    struct NumberExpressionAST 
        : public ExpressionAST
    {
        NumberExpressionAST(const T value) 
            : value(value) { }

        T value;
    };

    struct VariableExpressionAST
        : public ExpressionAST
    {
        VariableExpressionAST(const std::wstring& name)
            : name(name) { }

        std::wstring name;
    };

    struct BinaryExpressionAST
        : public ExpressionAST
    {
        BinaryExpressionAST(const Token::Type op,
                            std::unique_ptr<ExpressionAST> leftHandSide,
                            std::unique_ptr<ExpressionAST> rightHandSide)
            : op(op), 
              leftHandSide(std::move(leftHandSide)),
              rightHandSide(std::move(rightHandSide)) { }
        
        Token::Type op;
        std::unique_ptr<ExpressionAST> leftHandSide;
        std::unique_ptr<ExpressionAST> rightHandSide;
    };

    struct StructureAST
        : public ExpressionAST
    {
        StructureAST(const std::wstring& name,
                     std::vector<BasicArgument> members)
            : name(name), members(std::move(members)) { }

        std::wstring name;
        std::vector<BasicArgument> members;
    };

    struct ProcedurePrototypeAST
        : public ExpressionAST
    {
        ProcedurePrototypeAST(const std::wstring& name,
                              const std::vector<std::wstring> args)
            : name(name), args(std::move(args)) { }

        std::wstring name;
        std::vector<std::wstring> args;
    };

    struct ProcedureAST
        : public ExpressionAST
    {
        ProcedureAST(const std::wstring& name, 
                     std::vector<BasicArgument> arguments)
            : name(name), arguments(std::move(arguments)) { }

        std::wstring name;
        std::vector<BasicArgument> arguments;
    };

    struct CallProcedureAST
        : public ExpressionAST
    {
        CallProcedureAST(const std::wstring& callee,
                         const std::vector<std::unique_ptr<ExpressionAST>> args)
            : callee(callee), args(std::move(args)) { }

        std::wstring callee;
        std::vector<std::unique_ptr<ExpressionAST>> args;
    };
};
