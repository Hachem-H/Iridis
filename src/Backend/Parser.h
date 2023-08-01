#pragma once

#include "Token.h"
#include "AST.h"

#include <sstream>
#include <memory>
#include <map>

namespace Iridis
{
    class Parser
    {
    public:
        Parser(const std::wstring& sourceCode, std::vector<Token> tokens);

        bool HandleIdentifier();

        void ShowErrorLocation();
        void Parse();
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
