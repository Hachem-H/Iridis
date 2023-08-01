#include "Token.h"

#include <iostream>
#include <sstream>
#include <string>

namespace Iridis
{
    Token Token::ToToken(int line, int column, const std::wstring& buffer)
    {
        bool isNumber = true;
        for (wchar_t character : buffer)
            if (!std::isdigit(character))
            {
                isNumber = false;
                break;
            }

        if (isNumber)
        {
            std::wistringstream stream(buffer);
            int value = 0;
            if (stream >> value)
                return Token(line, column, value);
        }

             if (buffer == L"{")      return Token(line, column, Type::RCurlyBrace);
        else if (buffer == L"}")      return Token(line, column, Type::LCurlyBrace);
        else if (buffer == L"[")      return Token(line, column, Type::RBracket);
        else if (buffer == L"]")      return Token(line, column, Type::LBracket);
        else if (buffer == L"(")      return Token(line, column, Type::RParen);
        else if (buffer == L")")      return Token(line, column, Type::LParen);
        else if (buffer == L"<")      return Token(line, column, Type::RAngleBracket);
        else if (buffer == L">")      return Token(line, column, Type::LAngleBracket);

        else if (buffer == L":")      return Token(line, column, Type::Colon);
        else if (buffer == L";")      return Token(line, column, Type::SemiColon);
        else if (buffer == L"^")      return Token(line, column, Type::Caret);
        else if (buffer == L"&")      return Token(line, column, Type::Ampersand);
        else if (buffer == L"!")      return Token(line, column, Type::Exclamation);
        else if (buffer == L",")      return Token(line, column, Type::Comma);
        else if (buffer == L".")      return Token(line, column, Type::Dot);

        else if (buffer == L"+")      return Token(line, column, Type::Plus);
        else if (buffer == L"-")      return Token(line, column, Type::Minus);
        else if (buffer == L"*")      return Token(line, column, Type::Asterisk);
        else if (buffer == L"/")      return Token(line, column, Type::Slash);
        else if (buffer == L"=")      return Token(line, column, Type::Equal);

        else if (buffer == L"in")     return Token(line, column, Type::In);
        else if (buffer == L"do")     return Token(line, column, Type::Do);
        else if (buffer == L"if")     return Token(line, column, Type::If);
        else if (buffer == L"for")    return Token(line, column, Type::For);
        else if (buffer == L"else")   return Token(line, column, Type::Else);
        else if (buffer == L"while")  return Token(line, column, Type::While);
        else if (buffer == L"using")  return Token(line, column, Type::Using);
        else if (buffer == L"return") return Token(line, column, Type::Return);

        else if (buffer == L"proc")   return Token(line, column, Type::Procedure);
        else if (buffer == L"struct") return Token(line, column, Type::Structure);
        else if (buffer == L"enum")   return Token(line, column, Type::Enum);
        else if (buffer == L"extern") return Token(line, column, Type::Extern);
        else if (buffer == L"mod")    return Token(line, column, Type::Module);

        return Token(line, column, Type::Identifier, buffer);
    }

    std::wstring Token::ToString()
    {
             if (type == Type::RCurlyBrace)   return L"RCurlyBrace";
        else if (type == Type::LCurlyBrace)   return L"LCurlyBrace";
        else if (type == Type::RBracket)      return L"RBracket";
        else if (type == Type::LBracket)      return L"LBracket";
        else if (type == Type::RParen)        return L"RParen";
        else if (type == Type::LParen)        return L"LParen";
        else if (type == Type::RAngleBracket) return L"RAngleBracket";
        else if (type == Type::LAngleBracket) return L"LAngleBracket";

        else if (type == Type::Colon)         return L"Colon";
        else if (type == Type::SemiColon)     return L"SemiColon";
        else if (type == Type::Caret)         return L"Caret";
        else if (type == Type::Ampersand)     return L"Ampersand";
        else if (type == Type::Exclamation)   return L"Exclamation";
        else if (type == Type::Comma)         return L"Comma";
        else if (type == Type::Dot)           return L"Dot";

        else if (type == Type::In)            return L"In";
        else if (type == Type::Do)            return L"Do";
        else if (type == Type::If)            return L"If";
        else if (type == Type::For)           return L"For";
        else if (type == Type::Else)          return L"Else";
        else if (type == Type::While)         return L"While";
        else if (type == Type::Using)         return L"Using";
        else if (type == Type::Return)        return L"Return";

        else if (type == Type::Plus)          return L"Plus";
        else if (type == Type::Minus)         return L"Minus";
        else if (type == Type::Asterisk)      return L"Asterisk";
        else if (type == Type::Slash)         return L"Slash";
        else if (type == Type::Equal)         return L"Equal";
  
        else if (type == Type::Procedure)     return L"Procedure";
        else if (type == Type::Structure)     return L"Structure";
        else if (type == Type::Enum)          return L"Enum";
        else if (type == Type::Extern)        return L"Extern";
        else if (type == Type::Module)        return L"Module";

        else if (type == Type::Identifier)
            return L"Identifier(" + identifier + L")";
        else if (type == Type::Number)
            return L"Number(" + std::to_wstring(numberValue) + L")";
        
        else return L"Unknown Token";
    }
};
