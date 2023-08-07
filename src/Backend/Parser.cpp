#include "Core/Log.h"
#include "Parser.h"

#include <iostream>
#include <iomanip>

namespace Iridis
{
    std::unordered_map<Token::Type, int> Parser::BinaryOperatorPrecedence =
    {
        { Token::Type::RAngleBracket, 10 },
        { Token::Type::LAngleBracket, 10 },
        { Token::Type::Plus,          20 },
        { Token::Type::Minus,         20 },
        { Token::Type::Asterisk,      40 },
        { Token::Type::Slash,         40 },
    };

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

    std::unique_ptr<ExpressionAST> Parser::ParseBinaryOperatorRHS(
            int execPrecedence,
            std::unique_ptr<ExpressionAST> LHS)
    {
        while (true)
        {
            int precidence = BinaryOperatorPrecedence[currentToken.GetType()];

            if (precidence < execPrecedence)
                return LHS;

            Token::Type binaryOperator = currentToken.GetType();
            ReadNextToken();

            auto RHS = HandleExpression();
            if (!RHS)
                return nullptr;

            int nextPrecidence = BinaryOperatorPrecedence[currentToken.GetType()];
            if (precidence < nextPrecidence)
            {
                RHS = ParseBinaryOperatorRHS(precidence+1, std::move(RHS));
                if (!RHS)
                    return nullptr;
            }

            LHS = std::make_unique<BinaryExpressionAST>(binaryOperator, std::move(LHS), std::move(RHS));
        }
    }

    std::unique_ptr<ExpressionAST> Parser::HandleExpression()
    {
        auto LHS = ParseExpression();
        if (!LHS)
            return nullptr;
        return ParseBinaryOperatorRHS(0, std::move(LHS));
    }

    std::unique_ptr<ExpressionAST> Parser::HandleNumberExpression()
    {
        // TODO(Hachem): Support other types
        auto result = std::make_unique<NumberExpressionAST<int>>(*currentToken.GetNumber());
        ReadNextToken();
        return std::move(result);
    }

    std::unique_ptr<ExpressionAST> Parser::HandleParenExpression()
    {
        ReadNextToken();
        auto expression = HandleExpression();
        if (!expression)
            return nullptr;
        if (currentToken.GetType() != Token::Type::LParen)
        {
            int line = currentToken.GetLine();
            int column = currentToken.GetColumn();

            IRIDIS_ERROR("Expected `)` after expression @ Line: {}, Column: {}", line, column);
            ShowErrorLocation();
            std::cout << ": Did you mean to write a `)`" << std::endl;
            return nullptr;
        }
        ReadNextToken();
        return expression;
    }

    std::unique_ptr<ExpressionAST> Parser::HandleIdentifierExpression()
    {
        std::wstring identifier = *currentToken.GetIdentifier();
        ReadNextToken();

        if (currentToken.GetType() != Token::Type::RParen)
            return std::make_unique<VariableExpressionAST>(identifier);

        ReadNextToken();
        std::vector<std::unique_ptr<ExpressionAST>> arguments;

        if (currentToken.GetType() != Token::Type::LParen)
            while (true)
            {
                if (auto argument = HandleExpression())
                    arguments.push_back(std::move(argument));
                else
                    return nullptr;

                if (currentToken.GetType() == Token::Type::LParen)
                    break;

                if (currentToken.GetType() != Token::Type::Comma)
                {
                    int line = currentToken.GetLine();
                    int column = currentToken.GetColumn();
                    IRIDIS_ERROR("Expected `)` or ',' in argument list @ Line: {}, Column: {}", line, column);
                    ShowErrorLocation();
                    std::cout << ": Did you mean to write a `,` to continue your argument list or maybe a `)` to end it?" << std::endl;
                    return nullptr;
                }

                ReadNextToken();
            }
        
        ReadNextToken();
        return std::make_unique<CallProcedureAST>(identifier, std::move(arguments));
    }

    std::unique_ptr<ExpressionAST> Parser::ParseExpression()
    {
        if (currentToken.GetType() == Token::Type::Number)
            return HandleNumberExpression();
        if (currentToken.GetType() == Token::Type::RParen)
            return HandleParenExpression();

        return HandleIdentifierExpression();
    }

    int Parser::Parse()
    {
        int result = 0;

        for (; currentTokenIndex < (int) tokens.size(); currentTokenIndex++)
        {
            currentToken = tokens[currentTokenIndex];
            ParseExpression();
        }

        return result;
    }
};
