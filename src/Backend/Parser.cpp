#include "Parser.h"

#include <iostream>

namespace Iridis
{
    const std::map<Token::Type, int> Parser::BinaryOperatorPrecedence =
        {
            {Token::Type::RAngleBracket, 10},
            {Token::Type::LAngleBracket, 10},
            {Token::Type::Plus, 20},
            {Token::Type::Minus, 20},
            {Token::Type::Asterisk, 40},
            {Token::Type::Slash, 40},
    };

    bool Parser::ReadNextToken()
    {
        if (currentTokenIndex + 1 < (int) tokens.size())
        {
            currentTokenIndex++;
            currentToken = tokens[currentTokenIndex];
            return true;
        }

        return false;
    }

    bool Parser::ReadPreviousToken()
    {
        if (currentTokenIndex - 1 >= 0)
        {
            currentTokenIndex--;
            currentToken = tokens[currentTokenIndex];
            return true;
        }

        return false;
    }

    std::unique_ptr<ProcedureAST> Parser::ParseProcedure()
    {
        std::wstring name = *currentToken.GetIdentifier();
        ReadNextToken();
        ReadNextToken();
        ReadNextToken();

        if (currentToken.GetType() != Token::Type::Procedure)
            return nullptr;

        std::vector<BasicArgument> empty = {};
        return std::make_unique<ProcedureAST>(name, empty);
    }

    void Parser::Parse()
    {
        for (;currentTokenIndex < (int) tokens.size(); currentTokenIndex++)
        {
            currentToken = tokens[currentTokenIndex];

            if (currentToken.GetType() == Token::Type::Identifier)
                if (auto procedure = ParseProcedure())
                    std::wcout << L"Got procedure: " << procedure->name << std::endl;
        }
    }
};