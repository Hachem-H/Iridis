#include "Lexer.h"

#include <iostream>
#include <cwctype>

namespace Iridis
{
    namespace Lexer
    {
        std::vector<Token> Tokenize(const std::wstring& source)
        {
            struct TokenInfo
            {
                int line = 0;
                int column = 0;
                std::wstring identifier;
            };

            std::vector<TokenInfo> tokenInfo;
            std::wstring buffer;

            int currentLine = 1;
            int currentColumn = 1;
            int tokenStartLine = 1;
            int tokenStartColumn = 1;

            for (wchar_t character : source)
            {
                if (character == L'\n')
                {
                    currentLine++;
                    currentColumn = 1;
                    continue;
                }

                if (std::iswspace(character) || std::iswpunct(character))
                {
                    if (!buffer.empty())
                    {
                        tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });
                        buffer.clear();
                    }

                    if (std::iswpunct(character))
                        tokenInfo.push_back({ currentLine, currentColumn, std::wstring(1, character) });

                    tokenStartLine = currentLine;
                    tokenStartColumn = currentColumn + 1;
                }
                else
                {
                    if (buffer.empty())
                    {
                        tokenStartLine = currentLine;
                        tokenStartColumn = currentColumn;
                    }

                    buffer += character;
                }

                ++currentColumn;
            }

            if (!buffer.empty())
                tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });

            std::vector<Token> tokens;
            for (TokenInfo tokenInfo : tokenInfo)
                tokens.push_back(Token::ToToken(tokenInfo.line, tokenInfo.column, tokenInfo.identifier));

            return tokens;
        }
    };
};
