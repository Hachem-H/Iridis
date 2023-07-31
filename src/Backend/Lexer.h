#pragma once

#include "Token.h"

#include <vector>

namespace Iridis
{
    namespace Lexer
    {
        std::vector<Token> Tokenize(std::wstring& source);
    };
};
