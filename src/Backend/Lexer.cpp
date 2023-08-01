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

            bool insideSingleLineComment = false;
            bool insideMultiLineComment = false;

            for (size_t i = 0; i < source.size(); i++)
            {
                wchar_t character = source[i];
                wchar_t nextCharacter = (i + 1 < source.size()) ? source[i + 1] : L'\0';

                if (character == L'\n')
                {
                    currentLine++;
                    currentColumn = 1;
                    if (insideSingleLineComment)
                    {
                        insideSingleLineComment = false;
                        buffer.clear(); // Discard the comment content
                    }
                    continue;
                }

                if (insideMultiLineComment)
                {
                    if (character == L'*' && nextCharacter == L'/')
                    {
                        insideMultiLineComment = false;
                        i++; // Skip the next character '/'
                        currentColumn += 2;
                    }
                    else
                    {
                        currentColumn++;
                    }
                    continue; // Ignore characters inside the multi-line comment
                }

                if (insideSingleLineComment)
                {
                    continue; // Ignore characters after "//" until the end of line
                }

                if (character == L'/' && nextCharacter == L'*')
                {
                    insideMultiLineComment = true;
                    i++; // Skip the next character '*'
                    currentColumn += 2;
                    continue; // Ignore the '/*' characters
                }

                if (character == L'/' && nextCharacter == L'/')
                {
                    insideSingleLineComment = true;
                    i++; // Skip the next character '/'
                    currentColumn += 2;
                    continue; // Ignore the '//' characters
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

                currentColumn++;
            }

            if (!buffer.empty())
                tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });

            std::vector<Token> tokens;
            for (const TokenInfo& tokenInfo : tokenInfo)
                tokens.push_back(Token::ToToken(tokenInfo.line, tokenInfo.column, tokenInfo.identifier));

            return tokens;
        }
    };
};
