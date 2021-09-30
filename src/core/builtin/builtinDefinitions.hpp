#pragma once

#include <string>
#include <vector>
#include <algorithm>

class BuiltinDefinitions {
    private:
        static const std::vector<std::string> builtinNames;

    public:
        enum class BuiltinEnums {
            INSERT, REMOVE, REPLACE,
            PUSHFRONT, PUSHBACK,
            INSERTINPLACE, REMOVEINPLACE, REPLACEINPLACE,
            PUSHFRONTINPLACE, PUSHBACKINPLACE, 
            FRONT, BACK,
            HEAD, TAIL,
            COMBINE, APPEND, SIZE,
            RANGE, ISEMPTY,
            SUM, PRODUCT, MAX, MIN, SORTLH, SORTHL,
            CONTAINS, FIND,
            MAP, FILTER, FOREACH,
            FILL, REVERSE,
            FOLDL, FOLDR,
            ZIP, UNION, INTERSECT, EQUALS,
            INTTOSTRING, STRINGTOINT, STRINGTOCHARLIST, CHARLISTTOSTRING,
            PRINTINT,
            PRINTBOOL,
            PRINTLIST, PRINT2TUPLE, PRINT3TUPLE, PRINT4TUPLE,
            READCHAR, PRINTCHAR,
            READSTRING, PRINTSTRING,
            CONCAT, SUBSTR, CHARAT,
            RAND, PRINTTYPE, HALT,
            BUILTINNUM = HALT + 1
        };

        static int builtinNumber();

        static const std::string builtinDefinitions;

        static bool isBuiltin(const std::string & functionIdent);
        static BuiltinEnums getBuiltin(const std::string & builtinName);
};