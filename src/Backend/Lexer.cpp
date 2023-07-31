#include "Lexer.h"

#include <iostream>
#include <cwctype>

namespace Iridis
{
    namespace Lexer
    {
        std::vector<Token> Tokenize(std::wstring& source)
        {
            std::vector<std::wstring> stringTokens;
            std::wstring buffer;

            for (wchar_t character : source)
            {
                if (std::iswspace(character) || std::iswpunct(character))
                {
                    if (!buffer.empty())
                    {
                        stringTokens.push_back(buffer);
                        buffer.clear();
                    }

                    if (std::iswpunct(character))
                        stringTokens.push_back(std::wstring(1, character));
                } 
                else
                    buffer += character;
            }

            if (!buffer.empty())
                stringTokens.push_back(buffer);

            std::vector<Token> tokens;
            for (std::wstring stringToken : stringTokens)
                tokens.push_back(Token::ToToken(stringToken));

            return tokens;
        }
    };
};
