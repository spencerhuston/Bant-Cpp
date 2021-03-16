#include "../includes/interpreter.hpp"

Interpreter::Interpreter(const ExpPtr & rootExpression)
: rootExpression(rootExpression) { }

void
Interpreter::run() {

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

    return nullptr;
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

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretReference(const ExpPtr & expression, Values::Environment & environment) {
    auto reference = std::static_pointer_cast<Reference>(expression);



    return nullptr;
}

Values::ValuePtr
Interpreter::interpretBranch(const ExpPtr & expression, Values::Environment & environment) {
    auto branch = std::static_pointer_cast<Branch>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretArgument(const ExpPtr & expression, Values::Environment & environment) {
    auto argument = std::static_pointer_cast<Argument>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretTypeclass(const ExpPtr & expression, Values::Environment & environment) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretApplication(const ExpPtr & expression, Values::Environment & environment) {
    auto application = std::static_pointer_cast<Application>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretListDefinition(const ExpPtr & expression, Values::Environment & environment) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretTupleDefinition(const ExpPtr & expression, Values::Environment & environment) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    return nullptr;
}

Values::ValuePtr
Interpreter::interpretBlockGet(const ExpPtr & expression, Values::Environment & environment) {
    auto blockGet = std::static_pointer_cast<BlockGet>(expression);

    return nullptr;
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