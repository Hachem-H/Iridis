#include "Core/Log.h"
#include "Parser.h"

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

    bool Parser::HandleIdentifier()
    {
        const std::wstring name = *currentToken.GetIdentifier();
        ReadNextToken(); ReadNextToken();
        if (currentToken.GetType() != Token::Type::Colon &&
            currentToken.GetType() != Token::Type::Equal)
        {
            IRIDIS_ERROR("Expected `=` or `:` after type declaration.");
            return false;
        }

        ReadNextToken();

        if (currentToken.GetType() == Token::Type::Structure)
        {
            std::vector<BasicArgument> empty = {};
            if (auto ast = std::make_unique<StructureAST>(name, empty))
            {
                IRIDIS_CORE_INFO("Got structure called {}", WideToNarrow(name));
                return true;
            }
        }

        if (currentToken.GetType() == Token::Type::Procedure)
        {
            std::vector<BasicArgument> empty = {};
            if (auto ast = std::make_unique<ProcedureAST>(name, empty))
            {
                IRIDIS_CORE_INFO("Got procedure called {}", WideToNarrow(name));
                return true;
            }
        }

        IRIDIS_ERROR("Unexpected symbol: {}", WideToNarrow(currentToken.ToString()));
        return false;
    }

    void Parser::Parse()
    {
        for (;currentTokenIndex < (int) tokens.size(); currentTokenIndex++)
        {
            currentToken = tokens[currentTokenIndex];

            if (currentToken.GetType() == Token::Type::Identifier)
                if (!HandleIdentifier())
                    break;
        }
    }
};
