#include "Core/Log.h"
#include "Parser.h"

#include <iostream>
#include <iomanip>

namespace Iridis
{
    Parser::Parser(const std::wstring& sourceCode, std::vector<Token> tokens)
        : tokens(tokens), currentToken(tokens[0])
    {
        std::wstringstream ss(sourceCode);
        std::wstring line;

        while (std::getline(ss, line))
            sourceLines.push_back(line);
    }

    void Parser::ShowErrorLocation()
    {
        using namespace ConsoleColors;

        int line = currentToken.GetLine();
        int column = currentToken.GetColumn();

        int numDigits = 0;
        for (int number = line; number != 0;)
        {
            number /= 10;
            numDigits++;
        }

        std::wcout << GREEN << line << RESET << ": " << sourceLines[line-1] << std::endl;
        std::cout << std::string(numDigits+column+1, ' ') << "^ " << UNDERLINE << "here" << RESET;
    }

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
        std::cout << std::endl;

        const std::wstring name = *currentToken.GetIdentifier();
        ReadNextToken(); ReadNextToken();
        if (currentToken.GetType() != Token::Type::Colon &&
            currentToken.GetType() != Token::Type::Equal)
        {
            int line = currentToken.GetLine();
            int column = currentToken.GetColumn();

            IRIDIS_ERROR("Expected `=` or `:` after type declaration @ Line: {}, Column: {}", line, column);
            ShowErrorLocation(); 
            if (auto identifierName = currentToken.GetIdentifier())
                std::wcout << ": Did you mean `=` or `:` instead of `" << *identifierName << "`?";
            else
                std::wcout << ": Did you mean `=` or `:` instead of `" << currentToken.GetRepresentation() << "`?";
            std::cout << std::endl;
            return false;
        }

        ReadNextToken();

        if (currentToken.GetType() == Token::Type::Structure)
        {
            // TODO(Hachem): Parse structure
            std::vector<BasicArgument> empty = {};
            if (auto ast = std::make_unique<StructureAST>(name, empty))
            {
                IRIDIS_CORE_INFO("Got structure called {}", WideToNarrow(name));
                return true;
            }
        }

        if (currentToken.GetType() == Token::Type::Procedure)
        {
            // TODO(Hachem): Parse procedure
            std::vector<BasicArgument> empty = {};
            if (auto ast = std::make_unique<ProcedureAST>(name, empty))
            {
                IRIDIS_CORE_INFO("Got procedure called {}", WideToNarrow(name));
                return true;
            }
        }

        IRIDIS_ERROR("Unexpected symbol @ Line: {}, Column {}", 
                     currentToken.GetLine(), currentToken.GetColumn());
        ShowErrorLocation();
        if (auto identifierName = currentToken.GetIdentifier())
            std::wcout << L": `" << *identifierName << "` doesn't seem to be a valid symbol";
        std::cout << '.' << std::endl;
        return false;
    }

    int Parser::Parse()
    {
        int result = 0;

        for (; currentTokenIndex < (int) tokens.size(); currentTokenIndex++)
        {
            currentToken = tokens[currentTokenIndex];

            if (currentToken.GetType() == Token::Type::Identifier)
                if (!HandleIdentifier())
                    result = -1;
        }

        return result;
    }
};
