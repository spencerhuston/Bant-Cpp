#include "../includes/interpreter.hpp"

Interpreter::Interpreter(const ExpPtr & rootExpression)
: rootExpression(rootExpression) { }

void
Interpreter::run() {
    Values::Environment environment{};
    auto resultValue = std::static_pointer_cast<Values::IntValue>(interpret(rootExpression, environment));
    std::cout << std::to_string(resultValue->data) << std::endl;
    // print here or something
}

Values::ValuePtr
Interpreter::interpret(const ExpPtr & expression, Values::Environment & environment) {
    if (expression->expType == ExpressionTypes::PROG)
        return interpretProgram(expression, environment);
    else if (expression->expType == ExpressionTypes::LIT)
        return interpretLiteral(expression, environment);
    else if (expression->expType == ExpressionTypes::PRIM)
        return interpretPrimitive(expression, environment);
    else if (expression->expType == ExpressionTypes::LET)
        return interpretLet(expression, environment);
    else if (expression->expType == ExpressionTypes::REF)
        return interpretReference(expression, environment);
    else if (expression->expType == ExpressionTypes::BRANCH)
        return interpretBranch(expression, environment);
    else if (expression->expType == ExpressionTypes::ARG)
        return interpretArgument(expression, environment);
    else if (expression->expType == ExpressionTypes::TYPECLASS)
        return interpretTypeclass(expression, environment);
    else if (expression->expType == ExpressionTypes::APP)
        return interpretApplication(expression, environment);
    else if (expression->expType == ExpressionTypes::LIST_DEF)
        return interpretListDefinition(expression, environment);
    else if (expression->expType == ExpressionTypes::TUPLE_DEF)
        return interpretTupleDefinition(expression, environment);
    else if (expression->expType == ExpressionTypes::BLOCK_GET)
        return interpretBlockGet(expression, environment);
    else if (expression->expType == ExpressionTypes::MATCH)
        return interpretMatch(expression, environment);
    else if (expression->expType == ExpressionTypes::END)
        return nullptr;

    //printError(expression->token, std::string("Unknown expression type: ") + expression->token.text);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretProgram(const ExpPtr & expression, Values::Environment & environment) {
    auto program = std::static_pointer_cast<Program>(expression);

    Values::Environment programEnvironment;

    for (auto & function : program->functions) {
        Values::Environment functionInnerEnvironment;

        std::vector<std::string> parameterNames;
        for (auto & parameter : function->parameters) {
            parameterNames.push_back(parameter->name);
            //auto defaultValue = interpret(parameter->defaultValue);
        }
        auto functionValue = std::make_shared<Values::FunctionValue>(function->returnType, parameterNames, function->functionBody, functionInnerEnvironment);
        addName(functionInnerEnvironment, function->name, functionValue);
        addName(programEnvironment, function->name, functionValue);
    }

    for (auto & environmentVariable : environment) {
        addName(programEnvironment, environmentVariable.first, environmentVariable.second);
    }

    return interpret(program->body, programEnvironment);
}

Values::ValuePtr
Interpreter::interpretLiteral(const ExpPtr & expression, Values::Environment & environment) {
    auto literal = std::static_pointer_cast<Literal>(expression);

    if (literal->returnType->dataType == Types::DataTypes::INT) {
        return std::make_shared<Values::IntValue>(literal->returnType, std::get<int>(literal->data));
    } else if (literal->returnType->dataType == Types::DataTypes::CHAR) {
        return std::make_shared<Values::CharValue>(literal->returnType, std::get<char>(literal->data));
    } else if (literal->returnType->dataType == Types::DataTypes::STRING) {
        return std::make_shared<Values::StringValue>(literal->returnType, std::get<std::string>(literal->data));
    } else if (literal->returnType->dataType == Types::DataTypes::BOOL) {
        return std::make_shared<Values::BoolValue>(literal->returnType, std::get<bool>(literal->data));
    } else if (literal->returnType->dataType == Types::DataTypes::NULLVAL) {
        return std::make_shared<Values::NullValue>(literal->returnType);
    }

    // err for unknown literal type
    return nullptr;
}

Values::ValuePtr
Interpreter::interpretPrimitive(const ExpPtr & expression, Values::Environment & environment) {
    auto primitive = std::static_pointer_cast<Primitive>(expression);
    auto leftValue = interpret(primitive->leftSide, environment);
    auto rightValue = interpret(primitive->rightSide, environment);

    if (leftValue->type->dataType == Types::DataTypes::INT) {
        return doOperation<Values::IntValue>(primitive->op, leftValue, rightValue);
    } else if (leftValue->type->dataType == Types::DataTypes::CHAR) {
        return doOperation<Values::CharValue>(primitive->op, leftValue, rightValue);
    } else if (leftValue->type->dataType == Types::DataTypes::STRING) {
        return doOperation<Values::StringValue>(primitive->op, leftValue, rightValue);
    } else if (leftValue->type->dataType == Types::DataTypes::BOOL) {
        return doOperation<Values::BoolValue>(primitive->op, leftValue, rightValue);
    }

    // err about cannot perform operation on these types
    return nullptr;
}

Values::ValuePtr
Interpreter::interpretLet(const ExpPtr & expression, Values::Environment & environment) {
    auto let = std::static_pointer_cast<Let>(expression);

    auto letValue = interpret(let->value, environment);
    Values::Environment afterLetEnvironment;
    addName(afterLetEnvironment, let->ident, letValue);
    return interpret(let->afterLet, afterLetEnvironment);
}

Values::ValuePtr
Interpreter::interpretReference(const ExpPtr & expression, Values::Environment & environment) {
    auto reference = std::static_pointer_cast<Reference>(expression);

    auto referenceValue = getName(reference->token, environment, reference->ident);
    if (referenceValue->type->dataType == Types::DataTypes::TUPLE && !reference->fieldIdent.empty()) {
        auto tupleValue = std::static_pointer_cast<Values::TupleValue>(referenceValue);
        int tupleIndex = std::stoi(reference->fieldIdent);
        return tupleValue->tupleData.at(tupleIndex);
    } else if (referenceValue->type->dataType == Types::DataTypes::TYPECLASS && !reference->fieldIdent.empty()) {
        auto typeclassValue = std::static_pointer_cast<Values::TypeclassValue>(referenceValue);
        auto fieldValue = typeclassValue->fields.find(reference->fieldIdent);
        if (fieldValue == environment.end()) {
            //printError(reference->token, std::string("Error: typeclass ") +
            //           typeclassType->ident + std::string(" has no field ") +
            //          reference->fieldIdent);
            return std::make_shared<Values::NullValue>(std::make_shared<Types::NullType>());
        }
        return fieldValue->second;
    }

    return referenceValue;
}

Values::ValuePtr
Interpreter::interpretBranch(const ExpPtr & expression, Values::Environment & environment) {
    auto branch = std::static_pointer_cast<Branch>(expression);

    auto conditionValue = std::static_pointer_cast<Values::BoolValue>(interpret(branch->condition, environment));
    if (conditionValue->data) {
        return interpret(branch->ifBranch, environment);
    } 

    return interpret(branch->elseBranch, environment);
}

Values::ValuePtr
Interpreter::interpretArgument(const ExpPtr & expression, Values::Environment & environment) {
    auto argument = std::static_pointer_cast<Argument>(expression);

    // For default values, do later

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretTypeclass(const ExpPtr & expression, Values::Environment & environment) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);

    std::map<std::string, Values::ValuePtr> fields;
    for (unsigned int fieldIndex = 0; fieldIndex < typeclass->fields.size(); ++fieldIndex) {
        addName(fields, typeclass->fields.at(fieldIndex)->name, interpret(typeclass->fieldValues.at(fieldIndex), environment));
    }

    auto typeclassValue = std::make_shared<Values::TypeclassValue>(typeclass->returnType, fields);
    addName(environment, typeclass->ident, typeclassValue);

    return typeclassValue;
}

Values::ValuePtr
Interpreter::interpretApplication(const ExpPtr & expression, Values::Environment & environment) {
    auto application = std::static_pointer_cast<Application>(expression);

    auto ident = interpret(application->ident, environment);
    if (ident->type->dataType == Types::DataTypes::TYPECLASS) {
        auto typeclassType = std::static_pointer_cast<Types::TypeclassType>(application->returnType);
        Values::Environment typeclassArguments;
        for (unsigned int argumentIndex = 0; argumentIndex < typeclassType->fieldTypes.size(); ++argumentIndex) {
            addName(typeclassArguments, typeclassType->fieldTypes.at(argumentIndex).first, interpret(application->arguments.at(argumentIndex), environment));
        }
        return std::make_shared<Values::TypeclassValue>(typeclassType, typeclassArguments);
    } 
    
    // else has to be a function type

    auto functionValue = std::static_pointer_cast<Values::FunctionValue>(ident);
    Values::Environment functionEnvironment;
    for (unsigned int argumentIndex = 0; argumentIndex < application->arguments.size(); ++argumentIndex) {
        addName(functionEnvironment, functionValue->parameterNames.at(argumentIndex), interpret(application->arguments.at(argumentIndex), environment));
    }

    for (auto & environmentExpression : functionValue->functionBodyEnvironment) {
        addName(functionEnvironment, environmentExpression.first, environmentExpression.second);
    }

    return interpret(functionValue->functionBody, functionEnvironment);
}

Values::ValuePtr
Interpreter::interpretListDefinition(const ExpPtr & expression, Values::Environment & environment) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    std::vector<Values::ValuePtr> listData;
    for (auto & element : listDefinition->values) {
        listData.push_back(interpret(element, environment));
    }

    return std::make_shared<Values::ListValue>(listDefinition->returnType, listData);
}

Values::ValuePtr
Interpreter::interpretTupleDefinition(const ExpPtr & expression, Values::Environment & environment) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    std::vector<Values::ValuePtr> tupleData;
    for (auto & element : tupleDefinition->values) {
        tupleData.push_back(interpret(element, environment));
    }

    return std::make_shared<Values::TupleValue>(tupleDefinition->returnType, tupleData);
}

Values::ValuePtr
Interpreter::interpretBlockGet(const ExpPtr & expression, Values::Environment & environment) {
    auto blockGet = std::static_pointer_cast<BlockGet>(expression);

    unsigned int index = std::static_pointer_cast<Values::IntValue>(interpret(blockGet->index, environment))->data;
    auto listValue = std::static_pointer_cast<Values::ListValue>(interpret(blockGet->reference, environment));
    if (index > listValue->listData.size()) {
        // fatal error, out of bounds access
    }
    return listValue->listData.at(index);
}

Values::ValuePtr
Interpreter::interpretMatch(const ExpPtr & expression, Values::Environment & environment) {
    auto match = std::static_pointer_cast<Match>(expression);

    return nullptr;
}

void
Interpreter::addName(Values::Environment & environment, std::string name, Values::ValuePtr value) {
    if (environment.count(name)) {
		environment.erase(name);
	}
	environment.insert(std::pair<std::string, Values::ValuePtr>(name, value));
}

Values::ValuePtr
Interpreter::getName(const Token & token, Values::Environment & environment, std::string name) {
    auto value = environment.find(name);
    if (value == environment.end()) {
        //printError(token, std::string("Error: ") + name + 
        //                  std::string(" does not exist in this scope"));
        //return std::make_shared<Types::NullType>();
    }
    return value->second;
}

template<typename PrimitiveType>
Values::ValuePtr
Interpreter::doOperation(Operator::OperatorTypes op, const Values::ValuePtr & leftSide, const Values::ValuePtr & rightSide) {
    if (op == Operator::OperatorTypes::PLUS) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(),
                                                  std::static_pointer_cast<Values::IntValue>(leftSide)->data +
                                                  std::static_pointer_cast<Values::IntValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::MINUS) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(),
                                                  std::static_pointer_cast<Values::IntValue>(leftSide)->data -
                                                  std::static_pointer_cast<Values::IntValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::TIMES) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(),
                                                  std::static_pointer_cast<Values::IntValue>(leftSide)->data *
                                                  std::static_pointer_cast<Values::IntValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::DIV) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(),
                                                  std::static_pointer_cast<Values::IntValue>(leftSide)->data /
                                                  std::static_pointer_cast<Values::IntValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::MOD) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(),
                                                  std::static_pointer_cast<Values::IntValue>(leftSide)->data %
                                                  std::static_pointer_cast<Values::IntValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::GRT) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data >
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::LST) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data <
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::EQ) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data ==
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::NOTEQ) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data !=
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::GRTEQ) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data >=
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::LSTEQ) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<PrimitiveType>(leftSide)->data <=
                                                   std::static_pointer_cast<PrimitiveType>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::AND) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<Values::BoolValue>(leftSide)->data &&
                                                   std::static_pointer_cast<Values::BoolValue>(rightSide)->data);
    } else if (op == Operator::OperatorTypes::OR) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(),
                                                   std::static_pointer_cast<Values::BoolValue>(leftSide)->data ||
                                                   std::static_pointer_cast<Values::BoolValue>(rightSide)->data);
    }

    // err here about bad operator type
    return std::make_shared<Values::NullValue>(std::make_shared<Types::NullType>());
}