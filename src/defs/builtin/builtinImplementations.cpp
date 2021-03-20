#include "../../../includes/defs/builtin/builtinImplementations.hpp"

Values::ValuePtr
BuiltinImplementations::runBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::INSERT) {
        return insertBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::REMOVE) {
        return removeBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::REPLACE) {
        return replaceBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PUSHFRONT) {
        return pushFrontBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PUSHBACK) {
        return pushBackBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINTLIST) {
        return printListBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINT2TUPLE) {
        return print2TupleBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINT3TUPLE) {
        return print3TupleBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINT4TUPLE) {
        return print4TupleBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::INTTOCHAR) {
        return intToCharBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::CHARTOINT) {
        return charToIntBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::STRINGTOCHARLIST) {
        return stringToCharListBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::CHARLISTTOSTRING) {
        return charListToStringBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINTINT) {
        return printIntBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINTBOOL) {
        return printBoolBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::READCHAR) {
        return readCharBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINTCHAR) {
        return printCharBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::READSTRING) {
        return readStringBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRINTSTRING) {
        return printStringBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::HALT) {
        return haltBuiltin(functionValue, environment);
    }
    return nullValue;
}

template<class ValueType>
std::shared_ptr<ValueType>
BuiltinImplementations::getArgumentValue(const int & index, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return std::static_pointer_cast<ValueType>(environment.at(functionValue->parameterNames.at(index)));
}

Values::ListValuePtr
BuiltinImplementations::makeListType(Values::ListValuePtr listValue, std::vector<Values::ValuePtr> listData) {
    return std::make_shared<Values::ListValue>(std::make_shared<Types::ListType>(std::static_pointer_cast<Types::ListType>(listValue->type)->listType), listData);
}

Values::ValuePtr
BuiltinImplementations::insertBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    if (!elementValue->type->compare(std::static_pointer_cast<Types::ListType>(listValue->type)->listType)) {
        printError(token, "Error: Element type must match list type: " + token.position.currentLineText);
        return nullValue;
    }

    unsigned int index = getArgumentValue<Values::IntValue>(2, functionValue, environment)->data;
    
    if (index >= listValue->listData.size()) {
        printError(token, "Error: Out of bounds list access: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.insert(listData.begin() + index, elementValue);

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::removeBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot remove from empty list: " + token.position.currentLineText);
        return nullValue;
    }

    unsigned int index = getArgumentValue<Values::IntValue>(1, functionValue, environment)->data;
    
    if (index >= listValue->listData.size()) {
        printError(token, "Error: Out of bounds list access: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.erase(listData.begin() + index);

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::replaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot replace with element in empty list: " + token.position.currentLineText);
        return nullValue;
    }

    unsigned int index = getArgumentValue<Values::IntValue>(2, functionValue, environment)->data;
    
    if (index >= listValue->listData.size()) {
        printError(token, "Error: Out of bounds list access: " + token.position.currentLineText);
        return nullValue;
    }

    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    if (!elementValue->type->compare(std::static_pointer_cast<Types::ListType>(listValue->type)->listType)) {
        printError(token, "Error: Element type must match list type: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.at(index) = elementValue;

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::pushFrontBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);
    
    if (!elementValue->type->compare(std::static_pointer_cast<Types::ListType>(listValue->type)->listType)) {
        printError(token, "Error: Element type must match list type: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.insert(listData.begin(), elementValue);
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::pushBackBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);
    
    if (!elementValue->type->compare(std::static_pointer_cast<Types::ListType>(listValue->type)->listType)) {
        printError(token, "Error: Element type must match list type: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.insert(listData.begin() + listData.size(), elementValue);
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::insertInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::removeInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::replaceInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::pushFrontInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::pushBackInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::frontBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::backBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::headBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::tailBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::combineBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::appendBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::sizeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::rangeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::isEmptyBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::sumBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::productBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::maxBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::minBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::sortlhBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::sorthlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::containsBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::findBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::mapBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::filterBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::fillBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::reverseBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::foldlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::foldrBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::zipBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::unionBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::intersectBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::equalsBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::intToCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto intValue = getArgumentValue<Values::IntValue>(0, functionValue, environment)->data;
    return std::make_shared<Values::CharValue>(std::make_shared<Types::CharType>(), (char)intValue);
}

Values::ValuePtr
BuiltinImplementations::charToIntBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto charValue = getArgumentValue<Values::CharValue>(0, functionValue, environment)->data;
    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), (int)charValue);
}

Values::ValuePtr
BuiltinImplementations::stringToCharListBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto stringValue = getArgumentValue<Values::StringValue>(0, functionValue, environment)->data;
    std::vector<Values::ValuePtr> listData;
    for (auto & character : stringValue) {
        listData.push_back(std::make_shared<Values::CharValue>(std::make_shared<Types::CharType>(), character));
    }
    return std::make_shared<Values::ListValue>(std::make_shared<Types::ListType>(std::make_shared<Types::CharType>()), listData);
}

Values::ValuePtr
BuiltinImplementations::charListToStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    std::string stringValue;
    for (auto & value : listValue->listData) {
        stringValue += std::string(1, std::static_pointer_cast<Values::CharValue>(value)->data);
    }
    return std::make_shared<Values::StringValue>(std::make_shared<Types::StringType>(), stringValue);
}

Values::ValuePtr
BuiltinImplementations::printIntBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto intValue = getArgumentValue<Values::IntValue>(0, functionValue, environment)->data;
    std::cout << intValue << std::endl;
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::printBoolBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto boolValue = getArgumentValue<Values::BoolValue>(0, functionValue, environment)->data;
    std::cout << ((boolValue) ? std::string("true") : std::string("false")) << std::endl;
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::printListBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listData = getArgumentValue<Values::ListValue>(0, functionValue, environment)->listData;

    const std::string collectionType = "printList";
    
    std::cout << "(";
    if (listData.empty()) {
        std::cout << ")" << std::endl;
        return nullValue;
    }

    for (unsigned int listIndex = 0; listIndex < listData.size() - 1; ++listIndex) {
        printValue(token, listData.at(listIndex), collectionType);
        std::cout << ", ";
    }
    printValue(token, listData.at(listData.size() - 1), collectionType);
    std::cout << ")" << std::endl;

    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print2TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printTuple(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment)->tupleData, "print2Tuple");
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print3TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printTuple(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment)->tupleData, "print3Tuple");
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print4TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printTuple(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment)->tupleData, "print4Tuple");
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::readCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    char charValue;
    std::cin >> charValue;
    return std::make_shared<Values::CharValue>(std::make_shared<Types::CharType>(), charValue);
}

Values::ValuePtr
BuiltinImplementations::printCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto charValue = getArgumentValue<Values::CharValue>(0, functionValue, environment)->data;
    std::cout << charValue << std::endl;
    return nullValue;
}
Values::ValuePtr
BuiltinImplementations::readStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    std::string stringValue;
    std::cin >> stringValue;
    return std::make_shared<Values::StringValue>(std::make_shared<Types::StringType>(), stringValue);
}

Values::ValuePtr
BuiltinImplementations::printStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto stringValue = getArgumentValue<Values::StringValue>(0, functionValue, environment)->data;
    std::cout << stringValue << std::endl;
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::haltBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    exit(0);
    return nullValue;
}

std::shared_ptr<Values::NullValue> BuiltinImplementations::nullValue = std::make_shared<Values::NullValue>(std::make_shared<Types::NullType>());
bool BuiltinImplementations::error = false;

void
BuiltinImplementations::printTuple(const Token & token, const std::vector<Values::ValuePtr> & tupleData, const std::string & collectionType) {
    std::cout << "(";
    for (unsigned int tupleIndex = 0; tupleIndex < tupleData.size() - 1; ++tupleIndex) {
        printValue(token, tupleData.at(tupleIndex), collectionType);
        std::cout << ", ";
    }
    printValue(token, tupleData.at(tupleData.size() - 1), collectionType);
    std::cout << ")" << std::endl;
}


void
BuiltinImplementations::printValue(const Token & token, Values::ValuePtr value, const std::string & collectionType) {
    if (!Types::isPrimitiveType(value->type) || value->type->dataType == Types::DataTypes::GEN) {
        printError(token, std::string("Error: ") + collectionType + 
                          std::string(" only takes non-generic primitives"));
    }

    if (value->type->dataType == Types::DataTypes::INT) {
        std::cout << std::static_pointer_cast<Values::IntValue>(value)->data;
    } else if (value->type->dataType == Types::DataTypes::CHAR) {
        std::cout << std::string("'") << std::string(1, std::static_pointer_cast<Values::CharValue>(value)->data)
                  << std::string("'");
    } else if (value->type->dataType == Types::DataTypes::STRING) {
        std::cout << std::string("\"") << std::static_pointer_cast<Values::StringValue>(value)->data
                  << std::string("\"");
    } else if (value->type->dataType == Types::DataTypes::BOOL) {
        std::cout << ((std::static_pointer_cast<Values::BoolValue>(value)->data) ? std::string("true") : std::string("false"));
    }
}

void
BuiltinImplementations::printError(const Token & token, const std::string & errorMessage) {
    error = true;

    std::stringstream errorStream;
    errorStream << "Line: " << token.position.fileLine - BuiltinDefinitions::builtinNumber()
                << ", Column: " << token.position.fileColumn << std::endl
                << errorMessage << std::endl 
                << token.position.currentLineText << std::endl;
    Format::printError(errorStream.str());
}