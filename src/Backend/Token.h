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
            Character,
            String,
            Number,

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
            RAngleBracket,
            LAngleBracket,

            Colon,      // NOTE(Hachem): Haha colon
            SemiColon,
            Caret,
            Ampersand,
            Exclamation,
            Quote,
            DoubleQuote,
            Equal,
            Comma,
            Dot,

            Plus,
            Minus,
            Asterisk,
            Slash,

        };
        
        Token(const int line, 
              const int column,
              const int& number)
            : line(line),
              column(column),
              type(Type::Number),
              repr(std::to_wstring(number)),
              numberValue(number) { }

        Token(const int line, 
              const int column,
              const Type& type, 
              const std::wstring& value)
            : line(line),
              column(column),
              type(type),
              repr(value)
        {
            if (type == Type::Identifier)
                identifier = value;
        }

        Token(const int line, 
              const int column,
              const std::wstring& value,
              const std::wstring& repr)
            : line(line),
              column(column),
              type(Type::String),
              repr(repr),
              stringValue(value) { }

        Token(const int line, 
              const int column,
              const std::wstring& representation,
              const Type& type)
            : line(line),
              column(column),
              type(type),
              repr(representation) { }


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

        inline const int GetLine()                    const { return line;   }
        inline const int GetColumn()                  const { return column; }
        inline const std::wstring GetRepresentation() const { return repr;   }

        static Token ToToken(int line, int column, const std::wstring& buffer);
        std::wstring ToString();
    private:
        int line;
        int column;

        Type type;
        std::wstring repr = L"";
        int numberValue = 0;

        std::wstring identifier  = L"";
        std::wstring stringValue = L"";
    };
};
