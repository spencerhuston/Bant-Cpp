#pragma once

#include "defs/builtin/builtinDefinitions.hpp"
#include "defs/expressions.hpp"

#include "parser.hpp"

class TypeChecker {
    private:
        ExpPtr rootExpression;
        bool error = false;

        ExpPtr eval(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        
        ExpPtr evalProgram(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalLiteral(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalPrimitive(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalLet(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalReference(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalBranch(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalArgument(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalTypeclass(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalApplication(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalListDefinition(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalTupleDefinition(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalBlockGet(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);
        ExpPtr evalMatch(ExpPtr expression, Environment & environment, Types::TypePtr & expectedType);

        void resolveType(Types::TypePtr & returnType, Environment & environment);

        bool compare(Types::TypePtr & leftType, Types::TypePtr & rightType);

        void addName(Environment & environment, std::string name, Types::TypePtr type);
        Types::TypePtr getName(const Token & token, Environment & environment, std::string name);

        void printMismatchError(const Token & token, const Types::TypePtr & type, const Types::TypePtr & expectedType);
        void printError(const Token & token, const std::string & errorMessage);

    public:
        explicit TypeChecker(const ExpPtr & rootExpression);
        
        void check();
        bool errorOccurred() const { return error; }
};