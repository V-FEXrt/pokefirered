#include "parser.h"

#include <iostream>
#include <vector>

namespace core
{

    int Parser::GetPrecedence(const Token &token)
    {
        switch (token.type())
        {
        case Token::Type::kTimes:
            return 3;
        case Token::Type::kDivide:
            return 3;
        case Token::Type::kPlus:
            return 4;
        case Token::Type::kMinus:
            return 4;
        case Token::Type::kLeftShift:
            return 5;
        case Token::Type::kRightShift:
            return 5;
        case Token::Type::kBitAnd:
          return 8;
        case Token::Type::kBitXor:
          return 9;
        case Token::Type::kBitOr:
          return 10;

        default: {
            std::cout << "Asked for precedence of unmapped token: " << token.ToString() << std::endl;
            return 0;
        }
        }
    }

    std::vector<Token> Parser::ToPostfix()
    {
        std::vector<Token::Type> types = {
            Token::Type::kNumber,
            Token::Type::kIdentifier,
            Token::Type::kOpenParen,
            Token::Type::kCloseParen,
            Token::Type::kLeftShift,
            Token::Type::kRightShift,
            Token::Type::kPlus,
            Token::Type::kMinus,
            Token::Type::kTimes,
            Token::Type::kDivide,
            Token::Type::kBitXor,
            Token::Type::kBitAnd,
            Token::Type::kBitOr,
        };

        std::vector<Token> output;
        std::vector<Token> stack;

        while (std::find(types.begin(), types.end(), Peek().type()) != types.end())
        {
            Token token = Next();
            if (token.type() == Token::Type::kNumber || token.type() == Token::Type::kIdentifier)
            {
                output.push_back(token);
            }
            else if (token.type() == Token::Type::kOpenParen)
            {
                stack.push_back(token);
            }
            else if (token.type() == Token::Type::kCloseParen)
            {
                while (!stack.empty() && stack.back().type() != Token::Type::kOpenParen)
                {
                    Token back = stack.back();
                    stack.pop_back();
                    output.push_back(back);
                }

                // Next();

                if (!stack.empty())
                {
                    // pop the left parenthesis token
                    stack.pop_back();
                }
                else
                {
                    std::cout << "Mismatched parentheses detected in expression!" << std::endl;
                }
            }
            else
            {
                // token is an operator
                while (!stack.empty() && stack.back().type() != Token::Type::kOpenParen && GetPrecedence(stack.back()) <= GetPrecedence(token))
                {
                    Token back = stack.back();
                    stack.pop_back();
                    output.push_back(back);
                }
                stack.push_back(token);
            }
        }

        while (!stack.empty())
        {
            if (stack.back().type() == Token::Type::kOpenParen || stack.back().type() == Token::Type::kCloseParen)
            {
                std::cout << "Mismatched parentheses detected in expression!" << std::endl;
            }
            else
            {
                Token back = stack.back();
                stack.pop_back();
                output.push_back(back);
            }
        }

        return output;
    }

    Token Parser::Peek() { return tokens_[index_]; }

    Token Parser::Next()
    {
        Token t = Peek();
        index_++;
        return t;
    }

    int Parser::ResolveIdentifier()
    {
        std::string iden_val = Next().string_value();
        // error if iden_val not in top level
        return top_level_[iden_val];
    }

    bool Parser::IsOperator(const Token &token)
    {
        std::vector<Token::Type> types = {
            Token::Type::kLeftShift,
            Token::Type::kRightShift,
            Token::Type::kPlus,
            Token::Type::kMinus,
            Token::Type::kTimes,
            Token::Type::kDivide,
            Token::Type::kBitXor,
            Token::Type::kBitAnd,
            Token::Type::kBitOr,
        };
        return std::find(types.begin(), types.end(), token.type()) != types.end();
    }

    int Parser::EvaluateExpression(std::vector<Token> tokens)
    {
        std::vector<Token> stack;
        for (Token token : tokens)
        {
            if (IsOperator(token) && stack.size() > 1)
            {
                int op2 = stack.back().int_value();
                stack.pop_back();
                int op1 = stack.back().int_value();
                stack.pop_back();
                int result = 0;
                if (token.type() == Token::Type::kTimes)
                {
                    result = op1 * op2;
                }
                if (token.type() == Token::Type::kDivide)
                {
                    result = op1 / op2;
                }
                if (token.type() == Token::Type::kPlus)
                {
                    result = op1 + op2;
                }
                if (token.type() == Token::Type::kMinus)
                {
                    result = op1 - op2;
                }
                if (token.type() == Token::Type::kLeftShift)
                {
                    result = op1 << op2;
                }
                if (token.type() == Token::Type::kRightShift)
                {
                    result = op1 >> op2;
                }
                if (token.type() == Token::Type::kBitAnd) {
                    result = op1 & op2;
                }
                if (token.type() == Token::Type::kBitXor) {
                    result = op1 ^ op2;
                }
                if (token.type() == Token::Type::kBitOr) {
                    result = op1 | op2;
                }

                stack.push_back(Token(Token::Type::kNumber, result));
            }

            if (token.type() == Token::Type::kNumber)
            {
                stack.push_back(token);
            }

            if (token.type() == Token::Type::kIdentifier)
            {
                // error if iden_val not in top level
                std::string iden_val = token.string_value();
                stack.push_back(Token(Token::Type::kNumber, top_level_[iden_val]));
            }
        }
        return stack.size() ? stack.back().int_value() : 0;
    }

    DefineStatement Parser::ParseDefine()
    {
        if (Next().type() != Token::Type::kDefine)
        {
            // error
        }

        if (Peek().type() != Token::Type::kIdentifier)
        {
            // error
        }

        std::string identifer = Next().string_value();

        // Ambiguous. Need to forward look for the replacement list
        // if (Peek().type() == Token::Type::kOpenParen)
        // {
        //     // Parameters (x, y, x) Expression
        //     Next();

        //     while (Peek().type() != Token::Type::kCloseParen)
        //     {
        //         std::cout << Next().string_value() << std::endl;
        //         if (Peek().type() == Token::Type::kComma) { Next(); }
        //     }

        //     Next();
        //     // parse expression
        // }

        int value = EvaluateExpression(ToPostfix());

        top_level_[identifer] = value;
        return DefineStatement(identifer, value);
    }

    std::vector<DefineStatement> Parser::Parse(std::vector<Token> tokens)
    {
        index_ = 0;
        tokens_ = std::move(tokens);
        std::vector<DefineStatement> statements;

        while (index_ < tokens_.size())
        {
            switch (Peek().type())
            {
            case Token::Type::kDefine:
                statements.push_back(ParseDefine());
                break;

            default:
                Next();
                break;
            }
        }

        return statements;
    }

} // namespace core