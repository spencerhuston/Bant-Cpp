#pragma once

#include "types.hpp"
#include "token.hpp"

#include <vector>
#include <string>
#include <variant>

namespace Expressions {
    enum class ExpressionTypes {
        PROG, LIT, PRIM, LET, 
        REF, BRANCH, ARG, FUN_DEF, 
        APP, LIST_DEF, BLOCK_GET,
        CASE, MATCH, END
    };

    class Expression {
        private:
            Expression()
            : token(Token(Token::TokenType::DELIM,
                          FilePosition(-1, -1, "END"),
                          std::string({}))),
              expType(ExpressionTypes::END),
              returnType(Types::NullType()) { }

        public:
            Token token;
            ExpressionTypes expType;
            Types::Type returnType;

            Expression(const Token & token, 
                       const ExpressionTypes expType, 
                       const Types::Type & returnType)
            : token(token), 
              expType(expType), 
              returnType(returnType) { }
            
            static Expression End() {
                return Expression{};
            }
    };

    class Program : public Expression {
        public:
            std::vector<Expression> functions;
            Expression body;

            Program(const Token & token,
                    const std::vector<Expression> & functions,
                    Expression body)
            : Expression(token, ExpressionTypes::PROG, body.returnType),
              functions(functions), 
              body(body) { }

    };

    class Function : public Expression {
        public:
            std::string name{};
            std::vector<Types::GenType> genericParameters{};
            std::vector<Types::Type> parameters{};

            Expression functionBody;

            Function(const Token & token, 
                     const Types::Type & returnType,
                     const std::string name, 
                     const std::vector<Types::GenType> & genericParameters,
                     const std::vector<Types::Type> & parameters,
                     const Expression & functionBody)
            : Expression(token, ExpressionTypes::FUN_DEF, returnType),
              name(name),
              genericParameters(genericParameters),
              parameters(parameters),
              functionBody(functionBody) { }
    };

    class Literal : public Expression {
        public:
            std::variant<int, bool, char> data;

            template<typename T>
            Literal(const Token & token,
                    const Types::Type & returnType,
                    const T data)
            : Expression(token, ExpressionTypes::LIT, returnType),
              data(data) { }
            
            Literal(const Token & token,
                    const Types::Type & returnType)
            : Expression(token, ExpressionTypes::LIT, returnType) { }

            template<typename T>
            T getData() {
                return std::get<T>(data);
            }
    };

    class Primitive : public Expression {
        public:
            Operator::OperatorTypes op;
            Expression leftSide, rightSide;

            Primitive(const Token & token,
                      const Types::Type & returnType,
                      const Expression & leftSide,
                      const Expression & rightSide)
            : Expression(token, ExpressionTypes::PRIM, returnType),
              leftSide(leftSide),
              rightSide(rightSide) { }
    };

    class Let : public Expression {
        public:
            std::string ident;
            Types::Type valueType; // return type would be for the afterLet
            Expression value, afterLet;

            Let(const Token & token,
                const std::string & ident,
                const Types::Type & valueType,
                const Expression & value,
                const Expression & afterLet)
            : Expression(token, ExpressionTypes::LET, afterLet.returnType),
              ident(ident),
              valueType(valueType),
              value(value),
              afterLet(afterLet) { }
    };

    class Reference : public Expression {
        public:
            std::string ident;

            Reference(const Token & token,
                      const Types::Type & returnType,
                      const std::string & ident)
            : Expression(token, ExpressionTypes::REF, returnType),
              ident(ident) { }
    };

    class Branch : public Expression {
        public:
            Expression condition, ifBranch, elseBranch;

            Branch(const Token & token,
                   const Expression & condition,
                   const Expression & ifBranch,
                   const Expression & elseBranch)
            : Expression(token, ExpressionTypes::BRANCH, ifBranch.returnType),
              condition(condition),
              ifBranch(ifBranch),
              elseBranch(elseBranch) { }
    };

    class Argument : public Expression {
        public:
            std::string name;
    };

    class Application : public Expression {
        public:
            Expression function;
            std::vector<Expression> arguments;
    };

    class ListDefinition : public Expression {
        public:
            std::vector<Expression> values;

            ListDefinition(const Token & token,
                           const std::vector<Expression> values)
            : Expression(token, ExpressionTypes::LIST_DEF, Types::NullType()),
              values(values) { }
    };

    class BlockGet : public Expression {
        public:
            Expression reference, index;
    };

    class Case : public Expression {
        public:
            Expression ident, body;

            Case(const Token & token,
                 const Expression & ident,
                 const Expression & body)
            : Expression(token, ExpressionTypes::CASE, body.returnType),
              ident(ident),
              body(body) { }
    };

    class Match : public Expression {
        public:
            std::string ident;
            std::vector<Case> cases;

            Match(const Token & token,
                  const std::string & ident,
                  const std::vector<Case> cases)
            : Expression(token, ExpressionTypes::MATCH, cases.at(0).returnType),
              ident(ident),
              cases(cases) { }
    };
}