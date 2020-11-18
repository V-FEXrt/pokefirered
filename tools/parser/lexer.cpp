#include "lexer.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace core
{

    bool Lexer::IsNumber()
    {
        char c = Peek();
        return (c >= '0' && c <= '9');
    }

    bool Lexer::IsWhitespace()
    {
        char c = Peek();
        return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
    }

    bool Lexer::IsHexAlpha()
    {
        char c = Peek();
        return ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
    }

    bool Lexer::IsAlpha()
    {
        char c = Peek();
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    bool Lexer::IsAlphaNumber()
    {
        return IsAlpha() || IsNumber();
    };

    char Lexer::Peek()
    {
        return data_[index_];
    }

    char Lexer::Next()
    {
        char c = Peek();
        index_++;
        return c;
    }

    Token Lexer::ConsumeIdentifier()
    {
        std::string identifer = "";

        while (IsAlphaNumber() || Peek() == '_')
        {
            identifer += Next();
        }

        return Token(Token::Type::kIdentifier, identifer);
    }

    Token Lexer::ConsumeNumber()
    {
        std::string identifer = "";

        if (Peek() == '0')
        {
            identifer += Next();
            if (Peek() == 'x')
            {
                identifer += Next();
            }

            while (IsNumber() || IsHexAlpha())
            {
                identifer += Next();
            }

            return Token(Token::Type::kNumber, std::stoi(identifer, nullptr, 16));
        }

        while (IsNumber())
        {
            identifer += Next();
        }

        return Token(Token::Type::kNumber, std::stoi(identifer));
    }

    // TODO: Doesn't currently support escape characters
    Token Lexer::ConsumeString()
    {
        std::string value = "";
        if (Next() != '\"')
        {
            // Error
        }

        // TODO: error if we never see a quote
        while (Peek() != '\"')
        {
            value += Next();
        }
        Next(); // Consume final quote
        return Token(Token::Type::kString, value);
    }

    Token Lexer::ConsumeMacro()
    {
        Token id = ConsumeIdentifier();

        if (id.string_value() == "ifdef")
        {
            return Token(Token::Type::kIfDef);
        }
        if (id.string_value() == "ifndef")
        {
            return Token(Token::Type::kIfNDef);
        }
        if (id.string_value() == "define")
        {
            return Token(Token::Type::kDefine);
        }
        if (id.string_value() == "endif")
        {
            return Token(Token::Type::kEndIf);
        }

        if (id.string_value() == "include")
        {
            return Token(Token::Type::kInclude);
        }

        return Token(Token::Type::kDefine);
    }

    std::vector<Token> Lexer::LexString(const std::string &data)
    {
        index_ = 0;
        data_ = data;

        return Lex();
    }

    std::vector<Token> Lexer::LexFile(const std::string &path)
    {
        std::ifstream file;
        file.open(path);

        std::stringstream stream;
        stream << file.rdbuf();

        index_ = 0;
        data_ = stream.str();

        file.close();

        return Lex();
    }

    void Lexer::LexFileDumpTokens(const std::string &path, const std::string &out)
    {
        std::ofstream file;
        file.open(out);

        for (Token token : LexFile(path))
        {
            file << token.ToString() << std::endl;
        }

        file.close();
    }

    std::vector<Token> Lexer::Lex()
    {
        std::vector<Token> tokens;

        while (index_ < data_.length())
        {
            while (IsWhitespace())
            {
                Next();
            }

            if (IsAlpha())
            {
                tokens.push_back(ConsumeIdentifier());
                continue;
            }

            if (IsNumber())
            {
                tokens.push_back(ConsumeNumber());
                continue;
            }

            switch (Peek())
            {
            case '*':
                Next();
                tokens.push_back(Token(Token::Type::kTimes));
                break;
            case '-':
                Next();
                tokens.push_back(Token(Token::Type::kMinus));
                break;
            case '+':
                Next();
                tokens.push_back(Token(Token::Type::kPlus));
                break;
            case '(':
                Next();
                tokens.push_back(Token(Token::Type::kOpenParen));
                break;
            case ')':
                Next();
                tokens.push_back(Token(Token::Type::kCloseParen));
                break;
            case '&':
                Next();
                tokens.push_back(Token(Token::Type::kBitAnd));
                break;
            case '|':
                Next();
                tokens.push_back(Token(Token::Type::kBitOr));
                break;
            case '^':
                Next();
                tokens.push_back(Token(Token::Type::kBitXor));
                break;
            case ',':
                Next();
                tokens.push_back(Token(Token::Type::kComma));
                break;
            case '#':
                Next();
                tokens.push_back(ConsumeMacro());
                break;
            case '\"':
                tokens.push_back(ConsumeString());
                break;
            case '<':
                Next();
                if (Peek() == '<')
                {
                    Next();
                    tokens.push_back(Token(Token::Type::kLeftShift));
                    break;
                }
                tokens.push_back(Token(Token::Type::kLessThan));
                break;
            case '>':
                Next();
                if (Peek() == '>')
                {
                    Next();
                    tokens.push_back(Token(Token::Type::kRightShift));
                    break;
                }
                tokens.push_back(Token(Token::Type::kGreaterThan));
                break;

            case '/':
                Next();
                switch (Peek())
                {
                case '/':
                    while (Next() != '\n')
                        ;
                    continue;
                case '*':
                    while (Next() != '*')
                        ;
                    Next(); // last /
                    continue;
                default:
                    tokens.push_back(Token(Token::Type::kDivide));
                    continue;
                }

            default:
                std::cout << "Unknown char:" << Next() << std::endl;
                break;
            }
        }

        return tokens;
    }

    std::string Token::ToString() const
    {
        switch (type())
        {
        case core::Token::Type::kIfDef:
            return "Macro: IfDef";
        case core::Token::Type::kIfNDef:
            return "Macro: IfNDef";
        case core::Token::Type::kDefine:
            return "Macro: Define";
        case core::Token::Type::kEndIf:
            return "Macro: EndIf";
        case core::Token::Type::kInclude:
            return "Macro: Include";
        case core::Token::Type::kNumber:
            return "Number: " + std::to_string(int_value());
        case core::Token::Type::kString:
            return "String: " + string_value();
        case core::Token::Type::kIdentifier:
            return "Identifier: " + string_value();
        case core::Token::Type::kOpenParen:
            return "Symbol: (";
        case core::Token::Type::kCloseParen:
            return "Symbol: )";
        case core::Token::Type::kLessThan:
            return "Symbol: <";
        case core::Token::Type::kGreaterThan:
            return "Symbol: >";
        case core::Token::Type::kLeftShift:
            return "Symbol: <<";
        case core::Token::Type::kRightShift:
            return "Symbol: >>";
        case core::Token::Type::kPlus:
            return "Symbol: +";
        case core::Token::Type::kMinus:
            return "Symbol: -";
        case core::Token::Type::kTimes:
            return "Symbol: *";
        case core::Token::Type::kDivide:
            return "Symbol: /";
        case core::Token::Type::kBitXor:
            return "Symbol: ^";
        case core::Token::Type::kBitAnd:
            return "Symbol: &";
        case core::Token::Type::kBitOr:
            return "Symbol: |";
        case core::Token::Type::kQuote:
            return "Symbol: \"";
        case core::Token::Type::kComma:
            return "Symbol: ,";
        }
    }

} // namespace core