#pragma once

#include "Token.h"
#include "AST.h"

#include <unordered_map>
#include <sstream>
#include <memory>

namespace Iridis
{
    class Parser
    {
    public:
        Parser(const std::wstring& sourceCode, std::vector<Token> tokens);

        std::unique_ptr<ExpressionAST> HandleIdentifierExpression();
        std::unique_ptr<ExpressionAST> HandleNumberExpression();
        std::unique_ptr<ExpressionAST> HandleParenExpression();

        std::unique_ptr<ExpressionAST> ParseBinaryOperatorRHS(
                int execPrecedence,
                std::unique_ptr<ExpressionAST> LHS);
        std::unique_ptr<ExpressionAST> HandleExpression();

        void ShowErrorLocation();

        std::unique_ptr<ExpressionAST> ParseExpression();
        int Parse();
    public:
        static std::unordered_map<Token::Type, int> BinaryOperatorPrecedence;
    private:
        bool ReadNextToken();
        bool ReadPreviousToken();
    private:
        std::vector<std::wstring> sourceLines;
        std::vector<Token> tokens;
        Token currentToken;

        int currentTokenIndex = 0;
    };
};
