#pragma once

#include <string>
#include <optional>

namespace Iridis
{
    class Token
    {
    public:
        // NOTE(Hachem): This enum is going to grow indefintely, for now I'm only going to support basic Token types.
        // TODO(Hachem): Implement the other types.
        enum class Type
        {
            Identifier,
            Procedure,
            Structure,
            Extern,
            Module,
            Enum,
            
            In,
            Do,
            If,
            For,
            Else,
            While,
            Using,
            Return,

            RCurlyBrace,
            LCurlyBrace,
            RParen,
            LParen,
            RBracket,
            LBracket,

            Colon,      // NOTE(Hachem): Haha colon
            SemiColon,
            Caret,
            Ampersand,
            Exclamation,
            Equal,
            Comma,
            Dot,

            Plus,
            Minus,
            Asterisk,
            Slash,

            String,
            Number,
        };
        
        Token(const int& number)
            : type(Type::Number),
              numberValue(number) { }

        Token(const Type& type, const std::wstring& value)
            : type(type)
        {
            if (type == Type::Identifier)
                identifier = value;
            if (type == Type::String)
                stringValue = value;
        }

        Token(const Type& type)
            : type(type) { }


        inline const Type GetType() const { return type; }

        inline std::optional<int> GetNumber() const
        {
            if (type == Type::Number)
                return numberValue;
            else
                return { };
        }

        inline std::optional<std::wstring> GetIdentifier() const
        {
            if (type == Type::Identifier)
                return identifier;
            else
                return { };
        }

        inline std::optional<std::wstring> GetStringValue() const
        {
            if (type == Type::String)
                return identifier;
            else
                return { };
        }

        static Token ToToken(const std::wstring& buffer);
        std::wstring ToString();
    private:
        Type type;

        int numberValue          = 0;
        std::wstring identifier  = L"";
        std::wstring stringValue = L"";
    };
};
