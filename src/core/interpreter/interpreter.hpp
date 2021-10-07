#pragma once

#include "../../utils/operator.hpp"

#include "../../defs/token.hpp"
#include "../../defs/values.hpp"
#include "../builtin/builtinDefinitions.hpp"
#include "../builtin/builtinImplementations.hpp"

#include "../typeChecker/typeChecker.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <exception>

class RuntimeException : public std::exception {
    public:
        const char * what() const noexcept override {
            return "RuntimeException";
        }
};

class Interpreter {
    private:
        ExpPtr rootExpression;
        bool error = false;

        std::shared_ptr<Values::NullValue> errorNullValue;

        std::vector<std::pair<std::string, Token>> callStack;

        Values::ValuePtr interpretProgram(const ExpPtr & expression, const Values::Environment & environment);
        Values::ValuePtr interpretLiteral(const ExpPtr & expression, const Values::Environment & environment);
        Values::ValuePtr interpretPrimitive(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretLet(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretReference(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretBranch(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretTypeclass(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretApplication(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretListDefinition(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretTupleDefinition(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretMatch(const ExpPtr & expression, Values::Environment & environment);

        template<typename PrimitiveType>
        Values::ValuePtr doOperation(Token token, Operator::OperatorTypes op, const Values::ValuePtr & leftSide, const Values::ValuePtr & rightSide);

        Values::ValuePtr getName(const Token & token, Values::Environment & environment, std::string name);

        const std::string getStackTraceString();
        void printError(const Token & token, const std::string & errorMessage);

    public:
        explicit Interpreter(const ExpPtr & rootExpression);

        void run();
        Values::ValuePtr interpret(const ExpPtr & expression, Values::Environment & environment);
        
        void addName(Values::Environment & environment, std::string name, Values::ValuePtr value);
        bool errorOccurred() { return error; }
};