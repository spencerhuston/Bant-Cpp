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

    printError(expression->token, std::string("Unknown expression type: ") + expression->token.text);

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
        printMismatchError(literal->token, literal->returnType, expectedType);
    }
    
    return literal;
}

ExpPtr
TypeChecker::evalPrimitive(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto primitive = std::static_pointer_cast<Primitive>(expression);

    // unary op:
    //  - NOT for bool only
    //  - PLUS/MINUS for int only
    // 
    // binary op:
    //  - AND/OR for bool only
    //  - ARITH for int only
    //  - COMP for primitives

    if (Operator::isUnaryOperator(primitive->op)) {
        if (primitive->op == Operator::OperatorTypes::NOT) {
            eval(primitive->rightSide, environment, std::make_shared<Types::BoolType>());
            primitive->returnType = std::make_shared<Types::BoolType>();
        } else if (primitive->op == Operator::OperatorTypes::PLUS ||
                   primitive->op == Operator::OperatorTypes::MINUS) {
            eval(primitive->rightSide, environment, std::make_shared<Types::IntType>());
            primitive->returnType = std::make_shared<Types::IntType>();
        }
    } else if (Operator::isComparisonOperator(primitive->op) ||
               Operator::isArithmeticOperator(primitive->op)) {
        if (primitive->op == Operator::OperatorTypes::AND ||
            primitive->op == Operator::OperatorTypes::OR) {
            eval(primitive->leftSide, environment, std::make_shared<Types::BoolType>());
            eval(primitive->rightSide, environment, std::make_shared<Types::BoolType>());
            primitive->returnType = std::make_shared<Types::BoolType>();
        } else if (Operator::isArithmeticOperator(primitive->op)) {
            eval(primitive->leftSide, environment, std::make_shared<Types::IntType>());
            eval(primitive->rightSide, environment, std::make_shared<Types::IntType>());
            primitive->returnType = std::make_shared<Types::IntType>();
        } else { // is comparison operator
            eval(primitive->leftSide, environment, std::make_shared<Types::GenType>("$"));

            if (!Types::isPrimitiveType(primitive->leftSide->returnType)) {
                printError(primitive->token, "Binary operators can only be used on primitive types");
            }

            eval(primitive->rightSide, environment, primitive->leftSide->returnType);
            primitive->returnType = primitive->leftSide->returnType;
        }
    }

    return primitive;
}

ExpPtr
TypeChecker::evalLet(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto let = std::static_pointer_cast<Let>(expression);
    
    eval(let->value, environment, let->valueType);

    Environment afterLetEnvironment = environment;
    addName(afterLetEnvironment, let->ident, let->valueType);

    return eval(let->afterLet, afterLetEnvironment, expectedType);
}

ExpPtr
TypeChecker::evalReference(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto reference = std::static_pointer_cast<Reference>(expression);

    auto referenceType = environment.find(reference->ident);
    if (referenceType == environment.end()) {
        printError(reference->token, std::string("Error: ") + reference->ident + 
                           std::string(" does not exist in this scope"));
        return reference;
    }

    if (!referenceType->second->compare(expectedType)) {
        printMismatchError(reference->token, referenceType->second, expectedType);
    } else if (expectedType->dataType == Types::DataTypes::GEN) {
        reference->returnType = referenceType->second;
    }

    return reference;
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

// TODO
// - addName for fields
ExpPtr
TypeChecker::evalTypeclass(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);
    auto type = std::static_pointer_cast<Types::TypeclassType>(typeclass->returnType);
    if (!type->compare(expectedType)) {
        printMismatchError(typeclass->token, type, expectedType);
        return typeclass;
    }

    for (auto & field : typeclass->fields) {
        evalArgument(field, environment, std::make_shared<Types::NullType>());
    }

    return typeclass;
}

ExpPtr
TypeChecker::evalApplication(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto application = std::static_pointer_cast<Application>(expression);

    auto ident = eval(application->ident, environment, std::make_shared<Types::GenType>("$"));

    auto functionType = std::static_pointer_cast<Types::FuncType>(ident->returnType);
    application->returnType = functionType->returnType;
    
    if (application->arguments.size() != functionType->argumentTypes.size()) {
        printError(application->token, "Function application does not match signature");
    }

    for (unsigned int argumentIndex = 0; argumentIndex < application->arguments.size(); ++argumentIndex) {
        eval(application->arguments.at(argumentIndex), environment, functionType->argumentTypes.at(argumentIndex));
    }

    return application;
}

ExpPtr
TypeChecker::evalListDefinition(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    if (!listDefinition->returnType->compare(expectedType)) {
        printMismatchError(listDefinition->token, listDefinition->returnType, expectedType);
    }

    return listDefinition;
}

ExpPtr
TypeChecker::evalTupleDefinition(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    if (!tupleDefinition->returnType->compare(expectedType)) {
        printMismatchError(tupleDefinition->token, tupleDefinition->returnType, expectedType);
    }

    return tupleDefinition;
}

ExpPtr
TypeChecker::evalBlockGet(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    auto blockGet = std::static_pointer_cast<BlockGet>(expression);

    eval(blockGet->index, environment, std::make_shared<Types::IntType>());
    return eval(blockGet->reference, environment, expectedType);
}

// TODO
ExpPtr
TypeChecker::evalMatch(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    return nullptr;
}

// TODO
ExpPtr
TypeChecker::evalCase(const ExpPtr & expression, Environment & environment, const Types::TypePtr & expectedType) {
    return nullptr;
}

void
TypeChecker::addName(Environment & environment, std::string name, Types::TypePtr type) {
    if (environment.count(name)) {
		environment.erase(name);
	}
	environment.insert(std::pair<std::string, Types::TypePtr>(name, type));
}

void
TypeChecker::printMismatchError(const Token & token, const Types::TypePtr & type, const Types::TypePtr & expectedType) {
    error = true;

    std::stringstream errorStream;
    errorStream << "Line: " << token.position.fileLine
                << ", Column: " << token.position.fileColumn << std::endl
                << "Mismatched type: " << type->toString()
                << ", Expected: " << expectedType->toString()
                << std::endl << token.position.currentLineText << std::endl;
    Format::printError(errorStream.str());
}

void
TypeChecker::printError(const Token & token, const std::string & errorMessage) {
    error = true;

    std::stringstream errorStream;
    errorStream << "Line: " << token.position.fileLine
                << ", Column: " << token.position.fileColumn << std::endl
                << errorMessage << std::endl 
                << token.position.currentLineText << std::endl;
    Format::printError(errorStream.str());
}