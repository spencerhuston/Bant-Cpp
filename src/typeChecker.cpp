#include "../includes/typeChecker.hpp"

TypeChecker::TypeChecker(const ExpPtr & rootExpression)
: rootExpression(rootExpression) { }

ExpPtr
TypeChecker::check() {
    Format::printDebugHeader("Type checking/inference");
    Environment environment{};
    Types::GenTypePtr programReturnType = std::make_shared<Types::GenType>("$");
    ExpPtr typedRootExpression = eval(rootExpression, environment, programReturnType);
    Format::printDebugHeader("Type checking/inference Done");
    return typedRootExpression;
}

ExpPtr
TypeChecker::eval(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    if (expression->expType == ExpressionTypes::PROG)
        return evalProgram(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::LIT)
        return evalLiteral(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::PRIM)
        return evalPrimitive(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::LET)
        return evalLet(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::REF)
        return evalReference(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::BRANCH)
        return evalBranch(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::ARG)
        return evalArgument(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::TYPECLASS)
        return evalTypeclass(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::APP)
        return evalApplication(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::LIST_DEF)
        return evalListDefinition(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::TUPLE_DEF)
        return evalTupleDefinition(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::BLOCK_GET)
        return evalBlockGet(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::MATCH)
        return evalMatch(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::CASE)
        return evalCase(expression, environment, expectedType);
    else if (expression->expType == ExpressionTypes::END)
        return expression;

    error = true;

    std::stringstream errorStream;
    errorStream << "Unknown expression type: " << expression->token.text 
                << std::endl << expression->token.toString() << std::endl;

    Format::printError(errorStream.str());

    return nullptr;
}

ExpPtr
TypeChecker::evalProgram(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto program = std::static_pointer_cast<Program>(expression);

    for (auto & function : program->functions) {
        addName(environment, function->name, function->returnType);
    }

    for (auto & function : program->functions) {
        Environment functionInnerEnvironment = environment;

        for (auto & genericParameter : function->genericParameters) {
            addName(functionInnerEnvironment, genericParameter->identifier, genericParameter);
        }

        for (auto & parameter : function->parameters) {
            eval(parameter, functionInnerEnvironment, parameter->returnType);
            addName(functionInnerEnvironment, parameter->name, parameter->returnType);
        }

        auto functionReturnType = std::static_pointer_cast<Types::FuncType>(function->returnType);
        eval(function->functionBody, functionInnerEnvironment, functionReturnType->returnType);
    }

    return eval(program->body, environment, expectedType);
}

ExpPtr
TypeChecker::evalLiteral(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto literal = std::static_pointer_cast<Literal>(expression);
    
    if (!literal->returnType->compare(expectedType)) {
        printError(literal->token, literal->returnType, expectedType);
    }
    
    return literal;
}

// TODO
ExpPtr
TypeChecker::evalPrimitive(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto primitive = std::static_pointer_cast<Primitive>(expression);

    return nullptr;
}

ExpPtr
TypeChecker::evalLet(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto let = std::static_pointer_cast<Let>(expression);
    
    eval(let->value, environment, let->valueType);

    Environment afterLetEnvironment = environment;
    addName(afterLetEnvironment, let->ident, let->valueType);

    return eval(let->afterLet, afterLetEnvironment, expectedType);
}

// TODO
ExpPtr
TypeChecker::evalReference(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto reference = std::static_pointer_cast<Reference>(expression);
    return nullptr;
}

ExpPtr
TypeChecker::evalBranch(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto branch = std::static_pointer_cast<Branch>(expression);

    eval(branch->condition, environment, std::make_shared<Types::BoolType>());

    Types::TypePtr elseType = eval(branch->elseBranch, environment, expectedType)->returnType;
    return eval(branch->ifBranch, environment, elseType);
}

ExpPtr
TypeChecker::evalArgument(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto argument = std::static_pointer_cast<Argument>(expression);

    if (argument->defaultValue) {
        eval(argument->defaultValue, environment, argument->returnType);
    }

    return nullptr;
}

ExpPtr
TypeChecker::evalTypeclass(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);
    auto type = std::static_pointer_cast<Types::TypeclassType>(typeclass->returnType);
    if (!type->compare(expectedType)) {
        printError(typeclass->token, type, expectedType);
        return typeclass;
    }

    for (auto & field : typeclass->fields) {
        eval(field, environment, std::make_shared<Types::NullType>());
    }

    return typeclass;
}

// TODO
ExpPtr
TypeChecker::evalApplication(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    return nullptr;
}

ExpPtr
TypeChecker::evalListDefinition(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    if (!listDefinition->returnType->compare(expectedType)) {
        printError(listDefinition->token, listDefinition->returnType, expectedType);
    }

    return listDefinition;
}

ExpPtr
TypeChecker::evalTupleDefinition(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    if (!tupleDefinition->returnType->compare(expectedType)) {
        printError(tupleDefinition->token, tupleDefinition->returnType, expectedType);
    }

    return tupleDefinition;
}

ExpPtr
TypeChecker::evalBlockGet(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto blockGet = std::static_pointer_cast<BlockGet>(expression);

    if (eval(blockGet->index, environment, std::make_shared<Types::IntType>())) {
        printError(blockGet->token, blockGet->returnType, expectedType);
        return blockGet;
    }

    return eval(blockGet->reference, environment, expectedType);
}

// TODO
ExpPtr
TypeChecker::evalCase(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    return nullptr;
}


// TODO
ExpPtr
TypeChecker::evalMatch(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    return nullptr;
}

void
TypeChecker::addName(Environment environment, std::string name, Types::TypePtr type) {
    if (environment.count(name)) {
		environment.erase(name);
	}
	environment.insert(std::pair<std::string, Types::TypePtr>(name, type));
}

void
TypeChecker::printError(const Token & token, const Types::TypePtr & type, const Types::TypePtr & expectedType) {
    error = true;

    std::stringstream errorStream;
    errorStream << "Line: " << token.position.fileLine
                << ", Column: " << token.position.fileColumn << std::endl
                << "Mismatched type: " << type->toString()
                << ", Expected: " << expectedType->toString()
                << std::endl << token.position.currentLineText << std::endl;
    Format::printError(errorStream.str());
}