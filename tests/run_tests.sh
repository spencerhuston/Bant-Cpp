#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NONE='\033[0m'

BANT_PATH="../build/bant"
DEBUG=false

RUN_ARITH=false
RUN_BOOL=false
RUN_STRING_CHAR=false
RUN_LIST=false
RUN_BUILTINS=false
RUN_FUNCTIONS=false
RUN_TEMPLATES=false
RUN_TYPECLASS=false
RUN_ALL=false

NUM_SUCCESSES=0
NUM_FAILURES=0

function test {
	sourcePath="$1/$2"
	ret=$(echo -e "$($BANT_PATH $sourcePath)")
	exp=$(echo -e "$3")
	if [[ "$ret" == "$exp" ]] || [[ "$3" = "Error" && "${ret,,}" = *"${3,,}"* ]]; then
		echo -e "${GREEN}\tPASSED${NONE}  $4"
		((NUM_SUCCESSES++))
	else
		echo -e "${RED}\tFAILED${NONE}  $4"
		((NUM_FAILURES++))
	fi

	if [[ "$DEBUG" = true ]]; then
		echo -e "========================================"
		echo -e "EXPECTED:\n$exp\n"
		echo -e "RETURNED:\n$ret"
		echo -e "========================================\n"
	fi
}

function arith_tests {
	echo -e "${YELLOW}ARITHMETIC${NONE}"
	arithPath="./arith_tests"
	echo -e "${YELLOW}\tcorrect${NONE}"
	test $arithPath "op_prec.bnt" "1" "Operator precedence"
	test $arithPath "op_prec_paren.bnt" "12" "Operator precedence with parentheses"
	test $arithPath "unary_neg.bnt" "0" "Unary negate"
	echo ""
	echo -e "${YELLOW}\terror${NONE}"
	test $arithPath "div_by_zero.bnt" "Error" "reject division by zero"
	echo ""
}

function bool_tests {
	echo -e "${YELLOW}BOOLEAN${NONE}"
	boolPath="./bool_tests"
	test $boolPath "eq_or.bnt" "2" "OR'ing equals"
	test $boolPath "unary_not.bnt" "1" "Unary NOT"
	test $boolPath "not_literal_bool.bnt" "true" "Unary NOT on literal"
	test $boolPath "if_as_expr.bnt" "100" "If as expression"
	test $boolPath "or_eq_no_parens.bnt" "2" "OR'ing equals w/o parentheses"
	test $boolPath "int_bool_literal_mix.bnt" "true" "Int & Bool literals mix"
	echo ""
}

function string_char_tests {
	echo -e "${YELLOW}STRING & CHAR${NONE}"
	stringCharPath="./string_char_tests"
	test $stringCharPath "string.bnt" "test" "String variable"
	test $stringCharPath "string_with_space.bnt" "test space" "String variable with a space"
	test $stringCharPath "string_with_spaces.bnt" "test multi spaces" "String variable with multiple spaces"
	test $stringCharPath "string_literal.bnt" "stringliteral" "String literal"
	test $stringCharPath "string_literal_with_space.bnt" "string literal" "String literal with a space"
	test $stringCharPath "string_literal_with_spaces.bnt" "string literal spaces" "String literal with multiple spaces"
	test $stringCharPath "string_newline_escape.bnt" "newline\n\ntest" "String newline escape"
	test $stringCharPath "string_return_carriage_escape.bnt" "return\rtest" "String return carriage escape"
	test $stringCharPath "string_tab_escape.bnt" "tab\ttest" "String tab escape"
	test $stringCharPath "string_space_escape.bnt" "space test" "String space escape"
	test $stringCharPath "string_backspace_escape.bnt" "backspace\btest" "String backspace escape"
	test $stringCharPath "string_question_escape.bnt" "question?test" "String question escape"
	test $stringCharPath "string_normally_excluded_character.bnt" "~@$&|^," "Accept normally excluded characters"
	test $stringCharPath "string_comment_char.bnt" "#test" "Comment delimiter in string"
	echo ""
}

function list_tests {
	echo -e "${YELLOW}LIST${NONE}"
	listPath="./list_tests"
	echo -e "${YELLOW}\tcorrect${NONE}"
	test $listPath "list_of_ints.bnt" "(5, 6)" "List of ints"
	test $listPath "list_of_bool.bnt" "(true, false)" "List of bool"
	test $listPath "list_of_char.bnt" "('a', 'b')" "List of char"
	test $listPath "list_of_string.bnt" "(\"test\", \"test2\")" "List of string"
	test $listPath "list_of_null.bnt" "" "List of null"
	test $listPath "nested_list.bnt" "" "Nested list"
	test $listPath "list_of_tuples.bnt" "" "List of tuples"
	test $listPath "list_of_tuple_of_list.bnt" "" "List of tuple of list"
	test $listPath "list_of_function.bnt" "" "List of functions"
	test $listPath "function_list_param.bnt" "true" "Function with list parameter"
	test $listPath "list_of_typeclass.bnt" "2" "List of typeclass"
	test $listPath "list_return_type.bnt" "(5)" "List return type"
	test $listPath "nested_list_access.bnt" "2" "Nested list access"
	echo ""
	echo -e "${YELLOW}\terror${NONE}"
	test $listPath "nested_list_differing_types.bnt" "Error" "reject nested list of differing types"
	test $listPath "nested_no_top_level_primitive.bnt" "Error" "reject nested list with top-level primitive value"
	test $listPath "different_primitive_types.bnt" "Error" "reject different primitive types"
	test $listPath "no_nested_list_for_primitives.bnt" "Error" "reject nested list of same type in place of actual primitive value"
	test $listPath "function_list_param_no_differing_types.bnt" "Error" "reject differing list type passed to function"
	test $listPath "nested_list_access_differing_types.bnt" "Error" "reject access to nested list of differing types"
	echo ""
}

function builtins_tests {
	echo -e "${YELLOW}BUILTINS${NONE}"
	builtinsPath="./builtin_tests"
	echo -e "${YELLOW}\tprint2Tuple - correct${NONE}"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_bool_literals.bnt" "(0, true)" "int and bool literals"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_char_literals.bnt" "(0, 'a')" "int and char literals"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_string_literals.bnt" "(0, \"test\")" "int and string literals"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_bool_variables.bnt" "(0, true)" "int and bool variables"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_char_variables.bnt" "(0, 'a')" "int and char variables"
	test "${builtinsPath}/print2Tuple" "print2Tuple_int_string_variables.bnt" "(0, \"test\")" "int and string variables"
	test "${builtinsPath}/print2Tuple" "nested_2_tuple.bnt" "((5, 'a'), ((true, 6), (\"test\", \"test2\")))" "nested tuples"
	echo ""
	#echo -e "${YELLOW}\tprint2Tuple - error${NONE}"
	#test $builtinsPath "print2Tuple_no_typeclass.bnt" "Error" "reject typeclass"
	#test $builtinsPath "print2Tuple_no_function.bnt" "Error" "reject function"
	#test $builtinsPath "print2Tuple_no_null.bnt" "Error" "reject null"
	#test $builtinsPath "print2Tuple_no_nested_tuple.bnt" "Error" "reject nested tuple"
	#test $builtinsPath "print2Tuple_no_list.bnt" "Error" "reject list"
	#echo ""
	echo -e "${YELLOW}\tprintList - correct${NONE}"
	test $builtinsPath "print_nested_list.bnt" "(((1, 2), (3, 4, 5)), ((6), (7)))" "nested list"
	echo ""
	echo -e "${YELLOW}\tzip - correct${NONE}"
	test $builtinsPath "zip_int_char.bnt" "((1, 'a'), (2, 'b'), (3, 'c'))" "int and char"
	echo ""
	echo -e "${YELLOW}\tequals - correct${NONE}"
	test "${builtinsPath}/equals" "equals_list_int.bnt" "false\ntrue" "int lists"
	test "${builtinsPath}/equals" "equals_tuple.bnt" "false\nfalse\ntrue" "Tuples"
	test "${builtinsPath}/equals" "equals_bad_type_func.bnt" "false" "Bad type - function"
	echo ""
}

function func_tests {
	echo -e "${YELLOW}FUNCTIONS${NONE}"
	functionPath="./func_tests"
	echo -e "${YELLOW}\tcorrect${NONE}"
	test $functionPath "recursive_func.bnt" "0" "Recursive function"
	test $functionPath "mutually_recursive.bnt" "-1" "Mutually recursive functions"
	test $functionPath "mutually_recursive_separate_scope.bnt" "-1" "Mutually recursive functions, in separate program expression blocks"
	echo ""
	#echo -e "${YELLOW}\terror${NONE}"
	#echo ""
}

function template_tests {
	echo -e "${YELLOW}TEMPLATES${NONE}"
	templatesPath="./template_tests"
	echo -e "${YELLOW}\tcorrect${NONE}"
	test $templatesPath "identity_template.bnt" "" "Template identity function"
	test $templatesPath "nested_template_correct_return.bnt" "" "Nested template function, correct return from application"
	test $templatesPath "return_template_function.bnt" "" "Return nested template function"
	test $templatesPath "template_tuple.bnt" "" "Return tuples made from template"
	test $templatesPath "partial_function.bnt" "" "Partial function"
	test $templatesPath "builtin_no_retain_type_info.bnt" "2\n1" "Builtin does not keep old generic replacement info"
	echo ""
	echo -e "${YELLOW}\terror${NONE}"
	test $templatesPath "nested_template_incorrect_return.bnt" "Error" "Nested template function, incorrect return from application"
	echo ""
}

function typeclass_tests {
	echo -e "${YELLOW}TYPECLASS${NONE}"
	typeclassPath="./typeclass_tests"
	echo -e "${YELLOW}\tcorrect${NONE}"
	test $typeclassPath "car.bnt" "2020\nHonda Civic" "Car"
	test $typeclassPath "point.bnt" "2" "Point"
	echo ""
	#echo -e "${YELLOW}\terror${NONE}"
	#echo ""
}

for var in "$@"
do
    if [[ "$var" == "-d" ]]; then
		DEBUG=true
	fi
	
	if [[ "$var" == "-a" ]]; then
		RUN_ARITH=true
	fi

	if [[ "$var" == "-b" ]]; then
		RUN_BOOL=true
	fi
	
	if [[ "$var" == "-s" ]]; then
		RUN_STRING_CHAR=true
	fi

	if [[ "$var" == "-l" ]]; then
		RUN_LIST=true
	fi
	
	if [[ "$var" == "-i" ]]; then
		RUN_BUILTINS=true
	fi

	if [[ "$var" == "-f" ]]; then
		RUN_BUILTINS=true
	fi

	if [[ "$var" == "-g" ]]; then
		RUN_TEMPLATES=true
	fi

	if [[ "$var" == "-c" ]]; then
		RUN_TYPECLASS=true
	fi
done

if [[ "$RUN_ARITH" = false && 
		"$RUN_BOOL" = false && 
		"$RUN_STRING_CHAR" = false && 
		"$RUN_LIST" = false && 
		"$RUN_BUILTINS" = false && 
		"$RUN_TEMPLATES" = false && 
		"$RUN_FUNCTIONS" = false && 
		"$RUN_TYPECLASS" = false ]]; then
	RUN_ALL=true
fi

echo -e "\n${YELLOW}RUNNING TEST SCRIPT...${NONE}\n"

if [[ "$RUN_ALL" = true ]]; then
	arith_tests
	bool_tests
	string_char_tests
	list_tests
	builtins_tests
	func_tests
	template_tests
	typeclass_tests
fi

if [[ "$RUN_ARITH" = true ]]; then
	arith_tests
fi

if [[ "$RUN_BOOL" = true ]]; then
	bool_tests
fi

if [[ "$RUN_STRING_CHAR" = true ]]; then
	string_char_tests
fi

if [[ "$RUN_LIST" = true ]]; then
	list_tests
fi

if [[ "$RUN_BUILTINS" = true ]]; then
	builtins_tests
fi

if [[ "$RUN_FUNCTIONS" = true ]]; then
	func_tests
fi

if [[ "$RUN_TEMPLATES" = true ]]; then
	template_tests
fi

if [[ "$RUN_TYPECLASS" = true ]]; then
	typeclass_tests
fi

if [ "$NUM_FAILURES" -gt "0" ]; then
	echo -e "${RED}ONE OR MORE TESTS FAILED${NONE}\n"
fi

echo -e "${YELLOW}NUMBER PASSED: $NUM_SUCCESSES${NONE}"
echo -e "${YELLOW}NUMBER FAILED: $NUM_FAILURES${NONE}"
PERCENTAGE_PASSED=$((100*$NUM_SUCCESSES/($NUM_SUCCESSES + $NUM_FAILURES)))
echo -e "${YELLOW}PERCENTAGE PASSED: $PERCENTAGE_PASSED%${NONE}"
