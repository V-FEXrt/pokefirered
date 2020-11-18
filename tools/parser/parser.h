#ifndef INCLUDE_CORE_PARSER_H
#define INCLUDE_CORE_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "lexer.h"

namespace core
{
    class DefineStatement 
    {
    public:
        DefineStatement(std::string name, int value) : name_(name), value_(value) {}

        const std::string &name() const { return name_; }
        int value() const { return value_; }

    private:
        std::string name_;
        int value_;
    };

    class Parser
    {
    public:
        Parser() = default;

        std::vector<DefineStatement> Parse(std::vector<Token> tokens);
    private:
        int EvaluateExpression(std::vector<Token> tokens);
        int ResolveIdentifier();
        int GetPrecedence(const Token& token);
        bool IsOperator(const Token& token);
        std::vector<Token> ToPostfix();
        DefineStatement ParseDefine();

        Token Peek();
        Token Next();

        unsigned long index_;
        std::vector<Token> tokens_;

        std::map<std::string, int> top_level_;
    };
} // namespace core

#endif // INCLUDE_CORE_PARSER_H
