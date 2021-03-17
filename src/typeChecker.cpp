#include "../includes/typeChecker.hpp"

TypeChecker::TypeChecker(const ExpPtr & rootExpression)
: rootExpression(rootExpression) { }

void
TypeChecker::check() {
    Format::printDebugHeader("Type checking/inference");
    Environment environment{};
    Types::GenTypePtr programReturnType = std::make_shared<Types::GenType>("$");
    eval(rootExpression, environment, programReturnType);
    Format::printDebugHeader("Type checking/inference Done");
}

ExpPtr
TypeChecker::eval(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
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
    else if (expression->expType == ExpressionTypes::END)
        return expression;

    printError(expression->token, std::string("Unknown expression type: ") + expression->token.text);

    return nullptr;
}

ExpPtr
TypeChecker::evalProgram(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
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
TypeChecker::evalLiteral(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto literal = std::static_pointer_cast<Literal>(expression);
    
    if (!compare(literal->returnType, expectedType)) {
        printMismatchError(literal->token, literal->returnType, expectedType);
    }
    
    return literal;
}

ExpPtr
TypeChecker::evalPrimitive(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
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
    } else if (Operator::isBinaryBooleanOperator(primitive->op) ||
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
TypeChecker::evalLet(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto let = std::static_pointer_cast<Let>(expression);
    
    eval(let->value, environment, let->valueType);

    Environment afterLetEnvironment = environment;
    addName(afterLetEnvironment, let->ident, let->valueType);

    return eval(let->afterLet, afterLetEnvironment, expectedType);
}

ExpPtr
TypeChecker::evalReference(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto reference = std::static_pointer_cast<Reference>(expression);

    auto referenceType = getName(reference->token, environment, reference->ident);
    reference->returnType = referenceType;

    if (referenceType->dataType == Types::DataTypes::TUPLE && !reference->fieldIdent.empty()) {
        auto tupleType = std::static_pointer_cast<Types::TupleType>(referenceType);

        int tupleIndex = -1;
        try {
            tupleIndex = std::stoi(reference->fieldIdent);
        } catch (...) {
            printError(reference->token, std::string("Error: Tuple requires valid index: ") + reference->fieldIdent);
            return reference;
        }
        
        Types::TypePtr tupleElementType;
        try {
            tupleElementType = tupleType->tupleTypes.at(tupleIndex);
        } catch (...) {
            printError(reference->token, std::string("Error: Index not in range of tuple: ") + std::to_string(tupleIndex));
            return reference;
        }

        if (!compare(tupleElementType, expectedType)) {
            printMismatchError(reference->token, tupleElementType, expectedType);
        }   
        
        reference->returnType = tupleElementType;
    } else if (referenceType->dataType == Types::DataTypes::TYPECLASS && !reference->fieldIdent.empty()) {
        auto typeclassIdent = std::static_pointer_cast<Types::TypeclassType>(referenceType)->ident;
        auto typeclassType = std::static_pointer_cast<Types::TypeclassType>(getName(reference->token, environment, typeclassIdent));

        Types::TypePtr fieldType{nullptr};
        for (const auto & field : typeclassType->fieldTypes) {
            if (reference->fieldIdent == field.first) {
                fieldType = field.second;
                break;
            }
        }

        if (fieldType == nullptr) {
            printError(reference->token, std::string("Error: typeclass ") +
                       typeclassType->ident + std::string(" has no field ") +
                       reference->fieldIdent);
            return reference;
        }
        
        if (!compare(fieldType, expectedType)) {
            printMismatchError(reference->token, fieldType, expectedType);
        }   
        
        reference->returnType = fieldType;
    }

    if (!compare(reference->returnType, expectedType)) {
        printMismatchError(reference->token, referenceType, expectedType);
    }

    return reference;
}

ExpPtr
TypeChecker::evalBranch(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto branch = std::static_pointer_cast<Branch>(expression);

    eval(branch->condition, environment, std::make_shared<Types::BoolType>());

    Types::TypePtr elseType = eval(branch->elseBranch, environment, expectedType)->returnType;
    return eval(branch->ifBranch, environment, elseType);
}

ExpPtr
TypeChecker::evalArgument(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto argument = std::static_pointer_cast<Argument>(expression);

    if (argument->defaultValue) {
        eval(argument->defaultValue, environment, argument->returnType);
    }

    return nullptr;
}

ExpPtr
TypeChecker::evalTypeclass(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);
    auto type = std::static_pointer_cast<Types::TypeclassType>(typeclass->returnType);
    
    if (!compare(type, expectedType)) {
        printMismatchError(typeclass->token, type, expectedType);
        return typeclass;
    }

    for (auto & field : typeclass->fields) {
        evalArgument(field, environment, std::make_shared<Types::GenType>("$")); // deduce default values
    }

    addName(environment, typeclass->ident, type);

    return typeclass;
}

ExpPtr
TypeChecker::evalApplication(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto application = std::static_pointer_cast<Application>(expression);

    auto ident = eval(application->ident, environment, std::make_shared<Types::GenType>("$"));

    if (ident->returnType->dataType == Types::DataTypes::FUNC) {
        auto functionType = std::static_pointer_cast<Types::FuncType>(ident->returnType);
        application->returnType = functionType->returnType;
        
        if (application->arguments.size() != functionType->argumentTypes.size()) {
            printError(application->token, "Function application does not match signature");
        }

        for (unsigned int argumentIndex = 0; argumentIndex < application->arguments.size(); ++argumentIndex) {
            eval(application->arguments.at(argumentIndex), environment, functionType->argumentTypes.at(argumentIndex));
        }

        return application;
    } else if (ident->returnType->dataType == Types::DataTypes::TYPECLASS) {
        auto typeclassType = std::static_pointer_cast<Types::TypeclassType>(ident->returnType);
        application->returnType = typeclassType;
        
        if (application->arguments.size() != typeclassType->fieldTypes.size()) {
            printError(application->token, "Typeclass construction does not match signature");
        }

        for (unsigned int argumentIndex = 0; argumentIndex < application->arguments.size(); ++argumentIndex) {
            eval(application->arguments.at(argumentIndex), environment, typeclassType->fieldTypes.at(argumentIndex).second);
        }

        return application;
    }
    printError(application->token, "Bad function or typeclass application");
    return application;
}

ExpPtr
TypeChecker::evalListDefinition(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    if (!compare(listDefinition->returnType, expectedType)) {
        printMismatchError(listDefinition->token, listDefinition->returnType, expectedType);
    }

    return listDefinition;
}

ExpPtr
TypeChecker::evalTupleDefinition(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    if (!compare(tupleDefinition->returnType, expectedType)) {
        printMismatchError(tupleDefinition->token, tupleDefinition->returnType, expectedType);
    }

    return tupleDefinition;
}

ExpPtr
TypeChecker::evalBlockGet(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto blockGet = std::static_pointer_cast<BlockGet>(expression);

    eval(blockGet->index, environment, std::make_shared<Types::IntType>());
    eval(blockGet->reference, environment, std::make_shared<Types::ListType>(expectedType));

    if (blockGet->reference->returnType->dataType == Types::DataTypes::LIST) {
        blockGet->returnType = std::static_pointer_cast<Types::ListType>(blockGet->reference->returnType)->listType;
    }

    return blockGet;
}

ExpPtr
TypeChecker::evalMatch(ExpPtr expression, Environment & environment, Types::TypePtr expectedType) {
    auto match = std::static_pointer_cast<Match>(expression);

    auto caseType = getName(match->token, environment, match->ident);

    for (auto & casePtr : match->cases) {
        eval(casePtr->ident, environment, caseType);
        eval(casePtr->body, environment, expectedType);
    }

    return match;
}

bool 
TypeChecker::compare(Types::TypePtr leftType, Types::TypePtr rightType) {
    if (leftType->dataType == Types::DataTypes::UNKNOWN) {
        leftType = rightType;
        return true;
    } else if (rightType->dataType ==  Types::DataTypes::UNKNOWN) {
        rightType = leftType;
        return true;
    }
    return leftType->compare(rightType);
}

void
TypeChecker::addName(Environment & environment, std::string name, Types::TypePtr type) {
    if (environment.count(name)) {
		environment.erase(name);
	}
	environment.insert(std::pair<std::string, Types::TypePtr>(name, type));
}

Types::TypePtr
TypeChecker::getName(const Token & token, Environment & environment, std::string name) {
    auto type = environment.find(name);
    if (type == environment.end()) {
        printError(token, std::string("Error: ") + name + 
                          std::string(" does not exist in this scope"));
        return std::make_shared<Types::UnknownType>();
    }
    return type->second;
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