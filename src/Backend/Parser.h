#pragma once

#include "Token.h"
#include "AST.h"

#include <memory>
#include <map>

namespace Iridis
{
    class Parser
    {
    public:
        Parser(std::vector<Token> tokens)
            : tokens(tokens), currentToken(tokens[currentTokenIndex]) { }

        std::unique_ptr<ProcedureAST> ParseProcedure();

        void Parse();
    private:
        bool ReadNextToken();
        bool ReadPreviousToken();
    private:
        int currentTokenIndex = 0;

        std::vector<Token> tokens;
        Token currentToken;

        const static std::map<Token::Type, int> BinaryOperatorPrecedence;
    };
};
