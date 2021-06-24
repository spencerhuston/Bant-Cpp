#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NONE='\033[0m'

echo -e "\n${YELLOW}RUNNING TEST SCRIPT...${NONE}\n"

function test {
	sourcePath="$1/$2"
	ret=`../build/bant $sourcePath`
	if [ "$ret" = "$3" ] || [[ "$3" = "Error" && "$ret" = *"$3"* ]]; then
		echo -e "${GREEN}\tPASSED ${NONE} $4"
	else
		echo -e "${RED}\tFAILED ${NONE} $4"
	fi
}

echo -e "${YELLOW}ARITHMETIC${NONE}"
arithPath="./arith_tests"
test $arithPath "op_prec.bnt" "1" "Operator precedence"
test $arithPath "op_prec_paren.bnt" "12" "Operator precedence with parentheses"
test $arithPath "unary_neg.bnt" "0" "Unary negative"
echo ""

echo -e "${YELLOW}BOOLEAN${NONE}"
boolPath="./bool_tests"
test $boolPath "eq_or.bnt" "2" "OR'ing equals"
test $boolPath "unary_not.bnt" "1" "Unary NOT"
test $boolPath "if_as_expr.bnt" "100" "If as expression"
test $boolPath "or_eq_no_parens.bnt" "2" "OR'ing equals w/o parentheses"
echo ""

echo -e "${YELLOW}STRING & CHAR${NONE}"
boolPath="./string_char_tests"
test $boolPath "string.bnt" "test" "String variable"
test $boolPath "string_with_space.bnt" "test space" "String variable with a space"
test $boolPath "string_with_spaces.bnt" "test multi spaces" "String variable with multiple spaces"
test $boolPath "string_literal.bnt" "stringliteral" "String literal"
test $boolPath "string_literal_with_space.bnt" "string literal" "String literal with a space"
test $boolPath "string_literal_with_spaces.bnt" "string literal spaces" "String literal with multiple spaces"
echo ""

echo -e "${YELLOW}BUILTINS${NONE}"
boolPath="./builtin_tests"
echo -e "${YELLOW}\tprint2Tuple - correct${NONE}"
test $boolPath "print2Tuple_int_bool_literals.bnt" "(0, true)" "Print 2 tuple - int and bool literals"
test $boolPath "print2Tuple_int_char_literals.bnt" "(0, 'a')" "Print 2 tuple - int and char literals"
test $boolPath "print2Tuple_int_string_literals.bnt" "(0, \"test\")" "Print 2 tuple - int and string literals"
test $boolPath "print2Tuple_int_bool_variables.bnt" "(0, true)" "Print 2 tuple - int and bool variables"
test $boolPath "print2Tuple_int_char_variables.bnt" "(0, 'a')" "Print 2 tuple - int and char variables"
test $boolPath "print2Tuple_int_string_variables.bnt" "(0, \"test\")" "Print 2 tuple - int and string variables"
echo ""

echo -e "${YELLOW}\tprint2Tuple - error${NONE}"
test $boolPath "print2Tuple_no_typeclass.bnt" "Error" "Print 2 tuple - reject typeclass"
test $boolPath "print2Tuple_no_function.bnt" "Error" "Print 2 tuple - reject function"
test $boolPath "print2Tuple_no_null.bnt" "Error" "Print 2 tuple - reject null"
test $boolPath "print2Tuple_no_nested_tuple.bnt" "Error" "Print 2 tuple - reject nested tuple"
test $boolPath "print2Tuple_no_list.bnt" "Error" "Print 2 tuple - reject list"
echo ""