#pragma once

#include "../../defs/values.hpp"
#include "../../defs/token.hpp"
#include "../../utils/format.hpp"
#include "builtinDefinitions.hpp"
#include "../interpreter/interpreter.hpp"

#include <climits>
#include <algorithm>
#include <random>

class Interpreter;

class BuiltinImplementations {
    private:
        template<class ValueType>
        static std::shared_ptr<ValueType> getArgumentValue(const int & index, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        
        static Values::ListValuePtr makeListType(Values::ListValuePtr listValue, std::vector<Values::ValuePtr> listData);

        static Values::ValuePtr insertBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr removeBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr replaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushFrontBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushBackBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr insertInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr removeInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr replaceInPlaceBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushFrontInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushBackInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr frontBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr backBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr headBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr tailBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr combineBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr appendBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sizeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr rangeBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr isEmptyBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sumBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr productBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr maxBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr minBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sortlhBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sorthlBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr containsBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr findBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr mapBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr filterBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr fillBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr reverseBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr foldlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr foldrBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr zipBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr setOperation(Values::FunctionValuePtr functionValue, Values::Environment & environment, bool unionFlag);
        static Values::ValuePtr unionBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr intersectBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr equalsBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr intToStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr stringToIntBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr stringToCharListBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr charListToStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printIntBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printBoolBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printListBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print2TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print3TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print4TupleBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr readCharBuiltin(Values::FunctionValuePtr functionValue);
        static Values::ValuePtr printCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr readStringBuiltin(Values::FunctionValuePtr functionValue);
        static Values::ValuePtr printStringBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr concatBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr substrBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr charAtBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr randBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printTypeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr haltBuiltin(Values::FunctionValuePtr functionValue);

        static std::shared_ptr<Values::NullValue> nullValue;
        static bool error;

        static bool valuesEqual(Values::ValuePtr value1, Values::ValuePtr value2);

        static void printTuple(const Token & token, const std::vector<Values::ValuePtr> & tupleData, const std::string & collectionType);
        static void printValue(const Token & token, Values::ValuePtr value, const std::string & collectionType);
        
        static void printError(const Token & token, const std::string & errorMessage);

    public:
        static Interpreter interpreter;
        static Values::ValuePtr runBuiltin(const Token & token, Values::FunctionValuePtr functionValue, Values::Environment & environment);
};