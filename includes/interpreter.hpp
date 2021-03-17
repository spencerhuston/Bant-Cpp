#pragma once

#include "utils/operator.hpp"
#include "defs/values.hpp"
#include "typeChecker.hpp"

#include <iostream>

class Interpreter {
    private:
        ExpPtr rootExpression;
        bool error = false;

        Values::ValuePtr interpret(const ExpPtr & expression, Values::Environment & environment);

        Values::ValuePtr interpretProgram(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretLiteral(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretPrimitive(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretLet(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretReference(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretBranch(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretArgument(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretTypeclass(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretApplication(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretListDefinition(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretTupleDefinition(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretBlockGet(const ExpPtr & expression, Values::Environment & environment);
        Values::ValuePtr interpretMatch(const ExpPtr & expression, Values::Environment & environment);

        template<typename PrimitiveType>
        Values::ValuePtr doOperation(Operator::OperatorTypes op, const Values::ValuePtr & leftSide, const Values::ValuePtr & rightSide);

        void addName(Values::Environment & environment, std::string name, Values::ValuePtr value);
        Values::ValuePtr getName(const Token & token, Values::Environment & environment, std::string name);

    public:
        explicit Interpreter(const ExpPtr & rootExpression);

        void run();
        bool errorOccurred() { return error; }
};