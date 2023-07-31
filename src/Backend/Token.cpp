#include "Token.h"

#include <iostream>
#include <string>

namespace Iridis
{
    Token Token::ToToken(const std::wstring& buffer)
    {
             if (buffer == L"{")      return Token(Type::RCurlyBrace);
        else if (buffer == L"}")      return Token(Type::LCurlyBrace);
        else if (buffer == L"[")      return Token(Type::RBracket);
        else if (buffer == L"]")      return Token(Type::LBracket);
        else if (buffer == L"(")      return Token(Type::RParen);
        else if (buffer == L")")      return Token(Type::LParen);

        else if (buffer == L":")      return Token(Type::Colon);
        else if (buffer == L";")      return Token(Type::SemiColon);
        else if (buffer == L"^")      return Token(Type::Caret);
        else if (buffer == L"&")      return Token(Type::Ampersand);
        else if (buffer == L"!")      return Token(Type::Exclamation);
        else if (buffer == L",")      return Token(Type::Comma);
        else if (buffer == L".")      return Token(Type::Dot);

        else if (buffer == L"+")      return Token(Type::Plus);
        else if (buffer == L"-")      return Token(Type::Minus);
        else if (buffer == L"*")      return Token(Type::Asterisk);
        else if (buffer == L"/")      return Token(Type::Slash);
        else if (buffer == L"=")      return Token(Type::Equal);

        else if (buffer == L"in")     return Token(Type::In);
        else if (buffer == L"do")     return Token(Type::Do);
        else if (buffer == L"if")     return Token(Type::If);
        else if (buffer == L"for")    return Token(Type::For);
        else if (buffer == L"else")   return Token(Type::Else);
        else if (buffer == L"while")  return Token(Type::While);
        else if (buffer == L"using")  return Token(Type::Using);
        else if (buffer == L"return") return Token(Type::Return);

        else if (buffer == L"proc")   return Token(Type::Procedure);
        else if (buffer == L"struct") return Token(Type::Structure);
        else if (buffer == L"enum")   return Token(Type::Enum);
        else if (buffer == L"extern") return Token(Type::Extern);
        else if (buffer == L"mod")    return Token(Type::Module);

        // TODO(Hachem): Strings and Numbers
        return Token(Type::Identifier, buffer);
    }

    std::wstring Token::ToString()
    {
             if (type == Type::RCurlyBrace) return L"RCurlyBrace";
        else if (type == Type::LCurlyBrace) return L"LCurlyBrace";
        else if (type == Type::RBracket)    return L"RBracket";
        else if (type == Type::LBracket)    return L"LBracket";
        else if (type == Type::RParen)      return L"RParen";
        else if (type == Type::LParen)      return L"LParen";
  
        else if (type == Type::Colon)       return L"Colon";
        else if (type == Type::SemiColon)   return L"SemiColon";
        else if (type == Type::Caret)       return L"Caret";
        else if (type == Type::Ampersand)   return L"Ampersand";
        else if (type == Type::Exclamation) return L"Exclamation";
        else if (type == Type::Comma)       return L"Comma";
        else if (type == Type::Dot)         return L"Dot";

        else if (type == Type::In)          return L"In";
        else if (type == Type::Do)          return L"Do";
        else if (type == Type::If)          return L"If";
        else if (type == Type::For)         return L"For";
        else if (type == Type::Else)        return L"Else";
        else if (type == Type::While)       return L"While";
        else if (type == Type::Using)       return L"Using";
        else if (type == Type::Return)      return L"Return";

        else if (type == Type::Plus)        return L"Plus";
        else if (type == Type::Minus)       return L"Minus";
        else if (type == Type::Asterisk)    return L"Asterisk";
        else if (type == Type::Slash)       return L"Slash";
        else if (type == Type::Equal)       return L"Equal";
  
        else if (type == Type::Procedure)   return L"Procedure";
        else if (type == Type::Structure)   return L"Structure";
        else if (type == Type::Enum)        return L"Enum";
        else if (type == Type::Extern)      return L"Extern";
        else if (type == Type::Module)      return L"Module";

        else if (type == Type::Identifier)
            return L"Identifier(" + identifier + L")";
        
        else return L"Unknown Token";
    }
};
