#ifndef INCLUDE_CORE_LEXER_H
#define INCLUDE_CORE_LEXER_H

#include <string>
#include <vector>

namespace core
{
    class Token
    {
    public:
        enum class Type
        {
            // Macros
            kIfDef,
            kIfNDef,
            kDefine,
            kEndIf,
            kInclude,

            // Identifiers
            kIdentifier,

            // Literals
            kNumber,
            kString,

            // Symbols
            kOpenParen,
            kCloseParen,
            kLessThan,
            kGreaterThan,
            kLeftShift,
            kRightShift,
            kPlus,
            kMinus,
            kTimes,
            kDivide,
            kBitXor,
            kBitAnd,
            kBitOr,
            kQuote,
            kComma,

        };

        Token(Type type) : type_(type) {}
        Token(Type type, std::string string_value) : type_(type), string_value_(string_value) {}
        Token(Type type, int int_value) : type_(type), int_value_(int_value) {}

        Type type() const { return type_; }
        const std::string &string_value() const { return string_value_; }
        int int_value() const { return int_value_; }

        std::string ToString() const;

    private:
        Type type_;
        std::string string_value_;
        int int_value_;
    };

    class Lexer
    {
    public:
        Lexer() = default;
        ~Lexer() = default;

        std::vector<Token> LexFile(const std::string &path);
        std::vector<Token> LexString(const std::string &data);
        void LexFileDumpTokens(const std::string &path, const std::string &out);

    private:
        std::vector<Token> Lex();
        char Peek();
        char Next();
        bool IsNumber();
        bool IsAlpha();
        bool IsHexAlpha();
        bool IsAlphaNumber();
        bool IsWhitespace();

        Token ConsumeIdentifier();
        Token ConsumeNumber();
        Token ConsumeString();
        Token ConsumeMacro();

        std::string ReadIdentifier();

        std::string data_ = "";
        uint32_t index_ = 0;
    };
} // namespace core

#endif // INCLUDE_CORE_LEXER_H
