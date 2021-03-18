#pragma once

#include "../values.hpp"
#include "builtinDefinitions.hpp"

class BuiltinImplementations {
    private:
        static Values::ValuePtr insertBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr removeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr replaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushFrontBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushBackBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr insertInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr removeInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr replaceInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushFrontInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr pushBackInPlaceBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr frontBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr backBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr headBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr tailBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr combineBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr appendBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sizeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr rangeBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr isEmptyBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sumBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr productBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr maxBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr minBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sortlhBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr sorthlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr containsBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr findBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr mapBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr filterBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr fillBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr reverseBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr foldlBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr foldrBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr zipBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr unionBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr intersectBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr equalsBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr intToCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr charToIntBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr stringToCharListBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr charListToStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printIntBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printBoolBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printListBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print2TupleBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print3TupleBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr print4TupleBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr readCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printCharBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr readStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr printStringBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
        static Values::ValuePtr haltBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);

    public:
        static Values::ValuePtr runBuiltin(Values::FunctionValuePtr functionValue, Values::Environment & environment);
};