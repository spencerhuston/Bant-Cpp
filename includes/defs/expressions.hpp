#pragma once

#include "../utils/operator.hpp"

#include "types.hpp"
#include "token.hpp"
#include "builtin/builtinDefinitions.hpp"

#include <vector>
#include <string>
#include <variant>
#include <memory>

namespace Expressions {
    enum class ExpressionTypes {
        PROG, LIT, PRIM, LET, 
        REF, BRANCH, ARG, FUN_DEF, TYPECLASS,
        APP, LIST_DEF, TUPLE_DEF, BLOCK_GET,
        CASE, MATCH, END
    };

    class Expression {
        public:
            Token token;
            ExpressionTypes expType;
            Types::TypePtr returnType;

            Expression(const Token & token, 
                       const ExpressionTypes expType, 
                       const Types::TypePtr & returnType)
            : token(token),
              expType(expType), 
              returnType(returnType) { }
            
            Expression()
            : token(Token(Token::TokenType::DELIM,
                          FilePosition(-1, -1, "END"),
                          std::string({}))),
              expType(ExpressionTypes::END),
              returnType(std::make_shared<Types::NullType>()) { }
            
            static std::shared_ptr<Expression> End() {
                return std::make_shared<Expression>();
            }
    };

    using ExpPtr = std::shared_ptr<Expression>;

    class Argument : public Expression {
        public:
            std::string name;
            ExpPtr defaultValue;

            Argument(const Token & token,
                     const Types::TypePtr & returnType,
                     const std::string & name,
                     const ExpPtr & defaultValue)
            : Expression(token, ExpressionTypes::ARG, returnType),
              name(name),
              defaultValue(defaultValue) { }
    };
    
    class Function : public Expression {
        public:
            std::string name{};
            std::vector<Types::GenTypePtr> genericParameters{};
            std::vector<std::shared_ptr<Argument>> parameters{};

            ExpPtr functionBody;

            bool isBuiltin = false;
            BuiltinDefinitions::BuiltinEnums builtinEnum;

            Function(const Token & token, 
                     const Types::TypePtr & returnType,
                     const std::string name, 
                     const std::vector<Types::GenTypePtr> & genericParameters,
                     const std::vector<std::shared_ptr<Argument>> & parameters,
                     const ExpPtr & functionBody)
            : Expression(token, ExpressionTypes::FUN_DEF, returnType),
              name(name),
              genericParameters(genericParameters),
              parameters(parameters),
              functionBody(functionBody) { }
    };

    class Program : public Expression {
        public:
            std::vector<std::shared_ptr<Function>> functions;
            ExpPtr body;

            Program(const Token & token,
                    const std::vector<std::shared_ptr<Function>> & functions,
                    const ExpPtr & body)
            : Expression(token, ExpressionTypes::PROG, body->returnType),
              functions(functions), 
              body(body) { }

    };

    class Typeclass : public Expression {
        public:
            const std::string ident;
            std::vector<std::shared_ptr<Argument>> fields{};
            std::vector<ExpPtr> fieldValues{};

            Typeclass(const Token & token,
                      const std::string ident,
                      std::vector<std::shared_ptr<Argument>> fields,
                      const Types::TypePtr & typeclassType)
            : Expression(token, ExpressionTypes::TYPECLASS, typeclassType),
              ident(ident),
              fields(fields) {
                  for (auto & field : fields) {
                      fieldValues.push_back(field->defaultValue);
                  }
              }
    };

    class Literal : public Expression {
        public:
            std::variant<int, bool, char, std::string> data;

            Literal(const Token & token,
                    const Types::TypePtr & returnType,
                    const int data)
            : Expression(token, ExpressionTypes::LIT, returnType),
              data(std::variant<int, bool, char, std::string>(data)) { }

            Literal(const Token & token,
                    const Types::TypePtr & returnType,
                    const bool data)
            : Expression(token, ExpressionTypes::LIT, returnType),
              data(std::variant<int, bool, char, std::string>(data)) { }

            Literal(const Token & token,
                    const Types::TypePtr & returnType,
                    const char data)
            : Expression(token, ExpressionTypes::LIT, returnType),
              data(std::variant<int, bool, char, std::string>(data)) { }

              Literal(const Token & token,
                      const Types::TypePtr & returnType,
                      const std::string data)
            : Expression(token, ExpressionTypes::LIT, returnType),
              data(std::variant<int, bool, char, std::string>(data)) { }
            
            Literal(const Token & token)
            : Expression(token, ExpressionTypes::LIT, std::make_shared<Types::NullType>()) { }

            template<typename T>
            T getData() {
                return std::get<T>(data);
            }
    };

    class Primitive : public Expression {
        public:
            Operator::OperatorTypes op;
            ExpPtr leftSide, rightSide;

            Primitive(const Token & token,
                      const Types::TypePtr & returnType,
                      const Operator::OperatorTypes op,
                      const ExpPtr & leftSide,
                      const ExpPtr & rightSide)
            : Expression(token, ExpressionTypes::PRIM, returnType),
              op(op),
              leftSide(leftSide),
              rightSide(rightSide) { }
    };

    class Let : public Expression {
        public:
            std::string ident;
            Types::TypePtr valueType; // return type would be for the afterLet
            ExpPtr value, afterLet;

            Let(const Token & token,
                const std::string & ident,
                const Types::TypePtr & valueType,
                const ExpPtr & value,
                const ExpPtr & afterLet)
            : Expression(token, ExpressionTypes::LET, afterLet->returnType),
              ident(ident),
              valueType(valueType),
              value(value),
              afterLet(afterLet) { }
    };

    class Reference : public Expression {
        public:
            std::string ident;
            std::string fieldIdent;

            Reference(const Token & token,
                      const Types::TypePtr & returnType,
                      const std::string & ident)
            : Expression(token, ExpressionTypes::REF, returnType),
              ident(ident),
              fieldIdent("") { }
            
            Reference(const Token & token,
                      const Types::TypePtr & returnType,
                      const std::string & ident,
                      const std::string & fieldIdent)
            : Expression(token, ExpressionTypes::REF, returnType),
              ident(ident),
              fieldIdent(fieldIdent) { }
    };

    class Branch : public Expression {
        public:
            ExpPtr condition, ifBranch, elseBranch;

            Branch(const Token & token,
                   const ExpPtr & condition,
                   const ExpPtr & ifBranch,
                   const ExpPtr & elseBranch)
            : Expression(token, ExpressionTypes::BRANCH, ifBranch->returnType),
              condition(condition),
              ifBranch(ifBranch),
              elseBranch(elseBranch) { }
    };

    class Application : public Expression {
        public:
            ExpPtr ident;
            std::vector<ExpPtr> arguments;

            Application(const Token & token,
                        const ExpPtr & ident,
                        const std::vector<ExpPtr> arguments)
            : Expression(token, ExpressionTypes::APP, std::make_shared<Types::UnknownType>()),
              ident(ident),
              arguments(arguments) { }
    };

    class ListDefinition : public Expression {
        public:
            std::vector<ExpPtr> values;

            ListDefinition(const Token & token,
                           const std::vector<ExpPtr> values)
            : Expression(token, ExpressionTypes::LIST_DEF, std::make_shared<Types::ListType>()),
              values(values) { }

            ListDefinition(const Token & token,
                           const std::vector<ExpPtr> values,
                           const Types::TypePtr returnType)
            : Expression(token, ExpressionTypes::LIST_DEF, returnType),
              values(values) { }
    };

    class TupleDefinition : public Expression {
        public:
            std::vector<ExpPtr> values;

            TupleDefinition(const Token & token,
                            const std::vector<ExpPtr> values)
            : Expression(token, ExpressionTypes::TUPLE_DEF, std::make_shared<Types::TupleType>()),
              values(values) { }
            
            TupleDefinition(const Token & token,
                            const Types::TypePtr & returnType,
                            const std::vector<ExpPtr> values)
            : Expression(token, ExpressionTypes::TUPLE_DEF, returnType),
              values(values) { }
    };

    class BlockGet : public Expression {
        public:
            ExpPtr reference, index;

            BlockGet(const Token & token,
                     const ExpPtr & reference,
                     const ExpPtr & index)
            : Expression(token, ExpressionTypes::BLOCK_GET, std::make_shared<Types::UnknownType>()),
              reference(reference),
              index(index) { }
    };

    class Case : public Expression {
        public:
            ExpPtr ident, body;

            Case(const Token & token,
                 const ExpPtr & ident,
                 const ExpPtr & body)
            : Expression(token, ExpressionTypes::CASE, body->returnType),
              ident(ident),
              body(body) { }
    };

    class Match : public Expression {
        public:
            std::string ident;
            std::vector<std::shared_ptr<Case>> cases;

            Match(const Token & token,
                  const std::string & ident,
                  const std::vector<std::shared_ptr<Case>> cases)
            : Expression(token, ExpressionTypes::MATCH, std::make_shared<Types::UnknownType>()),
              ident(ident),
              cases(cases) { }
    };
}