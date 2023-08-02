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
            bool insideQuote = false;

            for (size_t i = 0; i < source.size(); i++)
            {
                wchar_t character = source[i];
                wchar_t nextCharacter = (i + 1 < source.size()) ? source[i + 1] : L'\0';

                if (character == L'\n')
                {
                    currentLine++;
                    currentColumn = 1;

                    if (!buffer.empty())
                    {
                        tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });
                        buffer.clear();
                    }

                    if (insideSingleLineComment)
                        insideSingleLineComment = false;
                    continue;
                }

                if (insideMultiLineComment)
                {
                    if (character == L'*' && nextCharacter == L'/')
                    {
                        insideMultiLineComment = false;
                        i++;
                        currentColumn += 2;
                    }
                    else
                        currentColumn++;
                    continue;
                }

                if (insideSingleLineComment)
                    continue;

                if (character == L'/' && nextCharacter == L'*')
                {
                    insideMultiLineComment = true;
                    i++;
                    currentColumn += 2;
                    continue;
                }

                if (character == L'/' && nextCharacter == L'/')
                {
                    insideSingleLineComment = true;
                    i++;
                    currentColumn += 2;
                    continue;
                }

                if (insideQuote)
                {
                    buffer += character;

                    if (character == L'"' && nextCharacter != L'\\')
                    {
                        insideQuote = false;
                        tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });
                        buffer.clear();
                    }
                    else if (character == L'\\' && nextCharacter == L'"')
                    {
                        buffer += nextCharacter;
                        i++;
                    }

                    currentColumn++;
                    continue;
                }

                if (character == L'"')
                {
                    insideQuote = true;
                    if (!buffer.empty())
                    {
                        tokenInfo.push_back({ tokenStartLine, tokenStartColumn, buffer });
                        buffer.clear();
                    }
                    buffer += character;
                    currentColumn++;
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
