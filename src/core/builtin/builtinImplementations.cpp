#include "builtinImplementations.hpp"

Interpreter BuiltinImplementations::interpreter{nullptr};

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
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::INSERTINPLACE) {
        return insertInPlaceBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::REMOVEINPLACE) {
        return removeInPlaceBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::REPLACEINPLACE) {
        return replaceInPlaceBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::FRONT) {
        return frontBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::BACK) {
        return backBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::HEAD) {
        return headBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::TAIL) {
        return tailBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::COMBINE) {
        return combineBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::APPEND) {
        return appendBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::SIZE) {
        return sizeBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::RANGE) {
        return rangeBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::ISEMPTY) {
        return isEmptyBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::SUM) {
        return sumBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::PRODUCT) {
        return productBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::MAX) {
        return maxBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::MIN) {
        return minBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::SORTLH) {
        return sortlhBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::SORTHL) {
        return sorthlBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::CONTAINS) {
        return containsBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::FIND) {
        return findBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::MAP) {
       return mapBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::FILTER) {
       return filterBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::FILL) {
        return fillBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::REVERSE) {
        return reverseBuiltin(functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::ZIP) {
        return zipBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::EQUALS) {
        return equalsBuiltin(token, functionValue, environment);
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
        return printStringBuiltin(token, functionValue, environment);
    } else if (functionValue->builtinEnum == BuiltinDefinitions::BuiltinEnums::HALT) {
        return haltBuiltin(functionValue, environment);
    }
    return nullValue;
}

template<class ValueType>
std::shared_ptr<ValueType>
BuiltinImplementations::getArgumentValue(const int & index, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return std::static_pointer_cast<ValueType>(environment->at(functionValue->parameterNames.at(index)));
}

Values::ListValuePtr
BuiltinImplementations::makeListType(Values::ListValuePtr listValue, std::vector<Values::ValuePtr> listData) {
    return std::make_shared<Values::ListValue>(std::make_shared<Types::ListType>(std::static_pointer_cast<Types::ListType>(listValue->type)->listType), listData);
}

Values::ValuePtr
BuiltinImplementations::insertBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    unsigned int index = getArgumentValue<Values::IntValue>(2, functionValue, environment)->data;
    
    if (!listValue->listData.empty() && index >= listValue->listData.size()) {
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

    auto listData = listValue->listData;
    listData.at(index) = elementValue;

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::pushFrontBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    auto listData = listValue->listData;
    listData.insert(listData.begin(), elementValue);
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::pushBackBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    auto listData = listValue->listData;
    listData.insert(listData.begin() + listData.size(), elementValue);
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::insertInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    unsigned int index = getArgumentValue<Values::IntValue>(2, functionValue, environment)->data;
    
    if (!listValue->listData.empty() && index >= listValue->listData.size()) {
        printError(token, "Error: Out of bounds list access: " + token.position.currentLineText);
        return nullValue;
    }

    listValue->listData.insert(listValue->listData.begin() + index, elementValue);

    return listValue;
}

Values::ValuePtr
BuiltinImplementations::removeInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
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

    listValue->listData.erase(listValue->listData.begin() + index);

    return listValue;
}

Values::ValuePtr
BuiltinImplementations::replaceInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
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

    listValue->listData.at(index) = elementValue;

    return listValue;
}

Values::ValuePtr
BuiltinImplementations::pushFrontInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    listValue->listData.insert(listValue->listData.begin(), elementValue);
    return listValue;
}

Values::ValuePtr
BuiltinImplementations::pushBackInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto elementValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    listValue->listData.insert(listValue->listData.begin() + listValue->listData.size(), elementValue);
    return listValue;
}

Values::ValuePtr
BuiltinImplementations::frontBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot get element from empty list: " + token.position.currentLineText);
        return nullValue;
    }

    return listValue->listData.at(0);
}

Values::ValuePtr
BuiltinImplementations::backBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot get element from empty list: " + token.position.currentLineText);
        return nullValue;
    }

    return listValue->listData.at(listValue->listData.size() - 1);
}

Values::ValuePtr
BuiltinImplementations::headBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    
    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot get sublist from empty list: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.pop_back();
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::tailBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    
    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot get sublist from empty list: " + token.position.currentLineText);
        return nullValue;
    }

    auto listData = listValue->listData;
    listData.erase(listData.begin());
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::combineBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue1 = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listValue2 = getArgumentValue<Values::ListValue>(1, functionValue, environment);

    auto combinedListData = listValue1->listData;
    combinedListData.insert(combinedListData.end(), listValue2->listData.begin(), listValue2->listData.end());
    return makeListType(listValue1, combinedListData);
}

Values::ValuePtr
BuiltinImplementations::appendBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue1 = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listValue2 = getArgumentValue<Values::ListValue>(1, functionValue, environment);

    listValue1->listData.insert(listValue1->listData.end(), listValue2->listData.begin(), listValue2->listData.end());
    return listValue1;
}

Values::ValuePtr
BuiltinImplementations::sizeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), listValue->listData.size());
}

Values::ValuePtr
BuiltinImplementations::rangeBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto startValue = getArgumentValue<Values::IntValue>(1, functionValue, environment);
    auto endValue = getArgumentValue<Values::IntValue>(2, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: Cannot get sublist from empty list: " + token.position.currentLineText);
        return nullValue;
    }
    
    unsigned int startIndex = std::static_pointer_cast<Values::IntValue>(startValue)->data;
    unsigned int endIndex = std::static_pointer_cast<Values::IntValue>(endValue)->data;

    if (startIndex > endIndex || 
        startIndex >= listValue->listData.size() || endIndex >= listValue->listData.size() ||
        startIndex < 0 || endIndex < 0) {
        printError(token, "Error: Invalid range: " + token.position.currentLineText);
        return nullValue;
    }

    std::vector<Values::ValuePtr> listData;
    for (unsigned int index = startIndex; index <= endIndex; ++index) {
        listData.push_back(listValue->listData.at(index));
    }
    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::isEmptyBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), listValue->listData.empty());
}

Values::ValuePtr
BuiltinImplementations::sumBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    int sum = 0;
    for (auto & value : listValue->listData) {
        sum += std::static_pointer_cast<Values::IntValue>(value)->data;
    }

    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), sum);
}

Values::ValuePtr
BuiltinImplementations::productBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), 0);
    }

    int product = 1;
    for (auto & value : listValue->listData) {
        product *= std::static_pointer_cast<Values::IntValue>(value)->data;
    }

    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), product);
}

Values::ValuePtr
BuiltinImplementations::maxBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: List[int] passed to max cannot be empty: " + token.position.currentLineText);
        return nullValue;
    }

    int max = INT_MIN;
    for (auto & value : listValue->listData) {
        auto intValue = std::static_pointer_cast<Values::IntValue>(value)->data;

        if (max < intValue) {
            max = intValue;
        }
    }

    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), max);
}

Values::ValuePtr
BuiltinImplementations::minBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        printError(token, "Error: List[int] passed to min cannot be empty: " + token.position.currentLineText);
        return nullValue;
    }

    int min = INT_MAX;
    for (auto & value : listValue->listData) {
        auto intValue = std::static_pointer_cast<Values::IntValue>(value)->data;

        if (min > intValue) {
            min = intValue;
        }
    }

    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), min);
}

Values::ValuePtr
BuiltinImplementations::sortlhBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        return listValue;
    }

    std::sort(listValue->listData.begin(), listValue->listData.end(), 
              [](const Values::ValuePtr & lhs, const Values::ValuePtr & rhs) {
                    return std::static_pointer_cast<Values::IntValue>(lhs)->data < std::static_pointer_cast<Values::IntValue>(rhs)->data;
                });
    return listValue;
}

Values::ValuePtr
BuiltinImplementations::sorthlBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        return listValue;
    }

    std::sort(listValue->listData.begin(), listValue->listData.end(), 
              [](const Values::ValuePtr & lhs, const Values::ValuePtr & rhs) {
                    return std::static_pointer_cast<Values::IntValue>(lhs)->data > std::static_pointer_cast<Values::IntValue>(rhs)->data;
                });
    return listValue;
}

Values::ValuePtr
BuiltinImplementations::containsBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listType = std::static_pointer_cast<Types::ListType>(listValue->type);

    if (listValue->listData.empty()) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), false);
    } else if (!Types::isPrimitiveType(listType->listType) || listType->listType->dataType == Types::DataTypes::GEN) {
        printError(token, "Error: contains requires list of non-generic primitives: " + token.position.currentLineText);
        return nullValue;
    }
    
    int typeEnum = static_cast<int>(listType->listType->dataType);
    auto searchValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    if (typeEnum != static_cast<int>(searchValue->type->dataType)) {
        printError(token, "Error: contains: mistmatched types: " + token.position.currentLineText);
        return nullValue;
    }

    for (auto & value : listValue->listData) {
        switch (typeEnum) {
            case 0: { // INT
                auto intValue = std::static_pointer_cast<Values::IntValue>(value)->data;
                auto searchIntValue = std::static_pointer_cast<Values::IntValue>(searchValue)->data;

                if (intValue == searchIntValue) {
                    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), true);
                }
            }
                break;
            case 1: { // CHAR
                auto charValue = std::static_pointer_cast<Values::CharValue>(value)->data;
                auto searchCharValue = std::static_pointer_cast<Values::CharValue>(searchValue)->data;

                if (charValue == searchCharValue) {
                    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), true);
                }
            }
                break;
            case 2: { // STRING
                auto stringValue = std::static_pointer_cast<Values::StringValue>(value)->data;
                auto searchStringValue = std::static_pointer_cast<Values::StringValue>(searchValue)->data;

                if (stringValue == searchStringValue) {
                    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), true);
                }
            }
                break;
            case 3: { // BOOL
                auto boolValue = std::static_pointer_cast<Values::BoolValue>(value)->data;
                auto searchBoolValue = std::static_pointer_cast<Values::BoolValue>(searchValue)->data;

                if (boolValue == searchBoolValue) {
                    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), true);
                }
            }
                break;
        }
    }

    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), false);
}

Values::ValuePtr
BuiltinImplementations::findBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listType = std::static_pointer_cast<Types::ListType>(listValue->type);

    if (listValue->listData.empty()) {
        return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), false);
    }
    
    int typeEnum = static_cast<int>(listType->listType->dataType);
    auto searchValue = getArgumentValue<Values::Value>(1, functionValue, environment);

    for (unsigned int index = 0; index < listValue->listData.size(); ++index) {
        switch (typeEnum) {
            case 0: { // INT
                auto intValue = std::static_pointer_cast<Values::IntValue>(listValue->listData.at(index))->data;
                auto searchIntValue = std::static_pointer_cast<Values::IntValue>(searchValue)->data;

                if (intValue == searchIntValue) {
                    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), index);
                }
            }
                break;
            case 1: { // CHAR
                auto charValue = std::static_pointer_cast<Values::IntValue>(listValue->listData.at(index))->data;
                auto searchCharValue = std::static_pointer_cast<Values::IntValue>(searchValue)->data;

                if (charValue == searchCharValue) {
                    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), index);
                }
            }
                break;
            case 2: { // STRING
                auto stringValue = std::static_pointer_cast<Values::IntValue>(listValue->listData.at(index))->data;
                auto searchStringValue = std::static_pointer_cast<Values::IntValue>(searchValue)->data;

                if (stringValue == searchStringValue) {
                    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), index);
                }
            }
                break;
            case 3: { // BOOL
                auto boolValue = std::static_pointer_cast<Values::IntValue>(listValue->listData.at(index))->data;
                auto searchBoolValue = std::static_pointer_cast<Values::IntValue>(searchValue)->data;

                if (boolValue == searchBoolValue) {
                    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), index);
                }
            }
                break;
        }
    }

    return std::make_shared<Values::IntValue>(std::make_shared<Types::IntType>(), -1);
}

Values::ValuePtr
BuiltinImplementations::mapBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto funcValue = getArgumentValue<Values::FunctionValue>(1, functionValue, environment);

    std::vector<Values::ValuePtr> listData;
    for (const auto value : listValue->listData) {
        auto funcEnvironment = funcValue->functionBodyEnvironment;
        interpreter.addName(funcEnvironment, funcValue->parameterNames.at(0), value);
        listData.push_back(interpreter.interpret(funcValue->functionBody, funcEnvironment));
    }

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::filterBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto funcValue = getArgumentValue<Values::FunctionValue>(1, functionValue, environment);
    
    std::vector<Values::ValuePtr> listData;
    for (const auto value : listValue->listData) {
        auto funcEnvironment = funcValue->functionBodyEnvironment;
        interpreter.addName(funcEnvironment, funcValue->parameterNames.at(0), value);
        auto result = std::static_pointer_cast<Values::BoolValue>(interpreter.interpret(funcValue->functionBody, funcEnvironment));
        
        if (result->data)
            listData.push_back(value);
    }

    return makeListType(listValue, listData);
}

Values::ValuePtr
BuiltinImplementations::fillBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto fillValue = getArgumentValue<Values::Value>(0, functionValue, environment);
    auto fillAmountValue = getArgumentValue<Values::IntValue>(1, functionValue, environment);

    std::vector<Values::ValuePtr> listData;
    for (int i = 0; i < fillAmountValue->data; ++i) {
        listData.push_back(fillValue);
    }

    return std::make_shared<Values::ListValue>(std::make_shared<Types::ListType>(fillValue->type), listData);
}

Values::ValuePtr
BuiltinImplementations::reverseBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);

    if (listValue->listData.empty()) {
        return listValue;
    }

    std::reverse(listValue->listData.begin(), listValue->listData.end());
    return listValue;
}

// TODO
Values::ValuePtr
BuiltinImplementations::foldlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto funcValue = getArgumentValue<Values::FunctionValue>(1, functionValue, environment);

    /*for (unsigned int foldIndex = 2; foldIndex < listValue->listData.size(); ++foldIndex) {
        auto funcEnvironment = funcValue->functionBodyEnvironment;
        interpreter.addName(funcEnvironment, funcValue->parameterNames.at(0), value);
        interpreter.addName(funcEnvironment, funcValue->parameterNames.at(1), value);

        auto result = std::static_pointer_cast<Values::Value>(interpreter.interpret(funcValue->functionBody, funcEnvironment));
    }*/

    return nullptr;
}

// TODO
Values::ValuePtr
BuiltinImplementations::foldrBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto funcValue = getArgumentValue<Values::FunctionValue>(1, functionValue, environment);

    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::zipBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue1 = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listValue2 = getArgumentValue<Values::ListValue>(1, functionValue, environment);

    if (listValue1->listData.size() != listValue2->listData.size()) {
        printError(token, "Error: zip: differing list sizes: " + token.position.currentLineText);
        return nullValue;
    }

    auto tupleType = std::make_shared<Types::TupleType>(std::vector<Types::TypePtr>{
                                        std::static_pointer_cast<Types::ListType>(listValue1->type)->listType, 
                                        std::static_pointer_cast<Types::ListType>(listValue2->type)->listType
                                    });
                            
    std::vector<Values::ValuePtr> listData;
    for (unsigned int zipIndex = 0; zipIndex < listValue1->listData.size(); ++zipIndex) {
        listData.push_back(std::make_shared<Values::TupleValue>(tupleType, std::vector<Values::ValuePtr>{listValue1->listData.at(zipIndex), listValue2->listData.at(zipIndex)}));
    }

    return std::make_shared<Values::ListValue>(std::make_shared<Types::ListType>(tupleType), listData);
}

// TODO
Values::ValuePtr
BuiltinImplementations::unionBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

// TODO
Values::ValuePtr
BuiltinImplementations::intersectBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    return nullptr;
}

Values::ValuePtr
BuiltinImplementations::equalsBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto listValue1 = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    auto listValue2 = getArgumentValue<Values::ListValue>(1, functionValue, environment);

    return std::make_shared<Values::BoolValue>(std::make_shared<Types::BoolType>(), valuesEqual(listValue1, listValue2));
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
    auto listValue = getArgumentValue<Values::ListValue>(0, functionValue, environment);
    printValue(token, listValue, "printList");
    std::cout << std::endl;

    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print2TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printValue(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment), "print2Tuple");
    std::cout << std::endl;
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print3TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printValue(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment), "print3Tuple");
    std::cout << std::endl;
    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::print4TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    printValue(token, getArgumentValue<Values::TupleValue>(0, functionValue, environment), "print4Tuple");
    std::cout << std::endl;
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
BuiltinImplementations::printStringBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    auto stringValue = getArgumentValue<Values::StringValue>(0, functionValue, environment)->data;

    for (unsigned int i = 0; i < stringValue.length(); ++i) {
        if (stringValue.at(i) == '\\' && i <= stringValue.length() - 1) {
            switch (stringValue.at(i + 1)) {
                case '?':
                    std::cout << "?";
                    break;
                case '\\':
                    std::cout << "\\";
                    break;
                case 'b':
                    std::cout << "\b";
                    break;
                case 'n':
                    std::cout << "\n";
                    break;
                case 'r':
                    std::cout << "\r";
                    break;
                case 't':
                    std::cout << "\t";
                    break;
                case 's':
                    std::cout << " ";
                    break;
                default:
                    printError(token, std::string("Error: invalid escape sequence: ") + std::string({stringValue.at(i + 1)}));
            }
            i++;
        } 
        else if (stringValue.at(i) == '\\' && i == stringValue.length()) {
            printError(token, "Error: escape slash requires escape character");
            return nullValue;
        }
        else {
            std::cout << std::string({stringValue.at(i)});
        }
    }

    std::cout << std::endl;

    return nullValue;
}

Values::ValuePtr
BuiltinImplementations::haltBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment) {
    exit(0);
    return nullValue;
}

// private
std::shared_ptr<Values::NullValue> BuiltinImplementations::nullValue = std::make_shared<Values::NullValue>(std::make_shared<Types::NullType>());
bool BuiltinImplementations::error = false;

bool
BuiltinImplementations::valuesEqual(Values::ValuePtr value1, Values::ValuePtr value2) {
    auto typeEnum = static_cast<int>(value1->type->dataType);
    switch (typeEnum) {
        case 0: { // INT
            auto intValue1 = std::static_pointer_cast<Values::IntValue>(value1)->data;
            auto intValue2 = std::static_pointer_cast<Values::IntValue>(value2)->data;

            return (intValue1 == intValue2);
        }
            break;
        case 1: { // CHAR
            auto charValue1 = std::static_pointer_cast<Values::CharValue>(value1)->data;
            auto charValue2 = std::static_pointer_cast<Values::CharValue>(value2)->data;

            return (charValue1 == charValue2);
        }
            break;
        case 2: { // STRING
            auto stringValue1 = std::static_pointer_cast<Values::StringValue>(value2)->data;
            auto stringValue2 = std::static_pointer_cast<Values::StringValue>(value2)->data;

            return (stringValue1 == stringValue2);
        }
            break;
        case 3: { // BOOL
            auto boolValue1 = std::static_pointer_cast<Values::BoolValue>(value2)->data;
            auto boolValue2 = std::static_pointer_cast<Values::BoolValue>(value2)->data;

            return (boolValue1 == boolValue2);
        }
            break;
        case 4: { // NULLVAL
            return true;
        }
            break;
        case 5: { // LIST
            auto listData1 = std::static_pointer_cast<Values::ListValue>(value1)->listData;
            auto listData2 = std::static_pointer_cast<Values::ListValue>(value2)->listData;

            if (listData1.size() != listData2.size()) {
                return false;
            }

            for (unsigned int listIndex = 0; listIndex < listData1.size(); ++listIndex) {
                if (!valuesEqual(listData1.at(listIndex), listData2.at(listIndex))) {
                    return false;
                }
            }

            return true;
        }
            break;
        case 6: { // TUPLE
            auto tupleData1 = std::static_pointer_cast<Values::TupleValue>(value1)->tupleData;
            auto tupleData2 = std::static_pointer_cast<Values::TupleValue>(value2)->tupleData;

            if (tupleData1.size() != tupleData2.size()) {
                return false;
            }

            for (unsigned int tupleIndex = 0; tupleIndex < tupleData1.size(); ++tupleIndex) {
                if (!valuesEqual(tupleData1.at(tupleIndex), tupleData2.at(tupleIndex))) {
                    return false;
                }
            }

            return true;
        }
            break;
        default: // function or typeclass equality support for now
            return false;
            break;
    }

    return false;
}

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
    } else if (value->type->dataType == Types::DataTypes::NULLVAL) {
        std::cout << "null";
    } else if (value->type->dataType == Types::DataTypes::LIST) {
        auto listData = std::static_pointer_cast<Values::ListValue>(value)->listData;

        std::cout << "(";
        if (listData.empty()) {
            std::cout << ")";
        }

        for (unsigned int listIndex = 0; listIndex < listData.size() - 1; ++listIndex) {
            printValue(token, listData.at(listIndex), collectionType);
            std::cout << ", ";
        }
        printValue(token, listData.at(listData.size() - 1), collectionType);
        std::cout << ")";
    } else if (value->type->dataType == Types::DataTypes::TUPLE) {
        auto tupleData = std::static_pointer_cast<Values::TupleValue>(value)->tupleData;

        std::cout << "(";
        for (unsigned int tupleIndex = 0; tupleIndex < tupleData.size() - 1; ++tupleIndex) {
            printValue(token, tupleData.at(tupleIndex), collectionType);
            std::cout << ", ";
        }
        printValue(token, tupleData.at(tupleData.size() - 1), collectionType);
        std::cout << ")";
    } else if (value->type->dataType == Types::DataTypes::FUNC) {
        auto funcType = std::static_pointer_cast<Types::FuncType>(value->type);

        std::cout << funcType->toString();
    } /* else if (value->type->dataType == Types::DataTypes::TYPECLASS) {
        auto typeclassValue = std::static_pointer_cast<Values::TypeclassValue>(value);
        // TODO
    } */
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