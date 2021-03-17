#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NONE='\033[0m'

function test {
	ret=`../build/bant $1`
	if [ "$ret" -eq "$2" ]; then
		echo -e "${GREEN} PASSED ${NONE} $3"
	else
		echo -e "${RED} FAILED ${NONE} $3"
	fi
}

echo -e "${YELLOW} Arithmetic ${NONE}"
test "./tests/arith_tests/op_prec.bnt" "1" "Operator precedence"
test "./tests/arith_tests/op_prec_paren.bnt" "12" "Operator precedence, with parentheses"
test "./tests/arith_tests/unary_neg.bnt" "0" "Unary negative"
echo ""

echo -e "${YELLOW} Boolean ${NONE}"
test "./tests/bool_tests/eq_or.bnt" "2" "OR'ing equals"
test "./tests/bool_tests/unary_not.bnt" "1" "Unary NOT"
test "./tests/bool_tests/if_as_expr.bnt" "100" "If as expression"
test "./tests/bool_tests/or_eq_no_parens.bnt" "2" "OR'ing equals w/o parentheses"
echo ""

echo -e "${YELLOW} Functions ${NONE}"
test "./tests/func_tests/func_as_val.bnt" "3" "Function as value"
test "./tests/func_tests/list_func_as_val.bnt" "5" "Function and list as values"
test "./tests/func_tests/closure.bnt" "3" "Closure"
test "./tests/func_tests/currying.bnt" "6" "Curry"
test "./tests/func_tests/multi_func_def.bnt" "3" "Function definitons between vals"
test "./tests/func_tests/mut_rec_func.bnt" "-1" "Mutually recursive functions"
test "./tests/func_tests/generic_closure.bnt" "5" "Generic closure"
echo ""

echo -e "${YELLOW} Lists & Builtins ${NONE}"
test "./tests/list_tests/insert.bnt" "2" "Insert element"
test "./tests/list_tests/remove.bnt" "2" "Remove element"
test "./tests/list_tests/replace.bnt" "2" "Replace element"
test "./tests/list_tests/pushback.bnt" "2" "Append element to back"
test "./tests/list_tests/pushfront.bnt" "2" "Append element to front"
test "./tests/list_tests/insertinplace.bnt" "2" "Insert element in place"
test "./tests/list_tests/removeinplace.bnt" "2" "Remove element in place"
test "./tests/list_tests/replaceinplace.bnt" "2" "Replace element in place"
test "./tests/list_tests/pushbackinplace.bnt" "2" "Append element to back in place"
test "./tests/list_tests/pushfrontinplace.bnt" "2" "Append element to front in place"
test "./tests/list_tests/front.bnt" "1" "Get first element of list"
test "./tests/list_tests/back.bnt" "3" "Get last element of list"
test "./tests/list_tests/head.bnt" "2" "Get list minus last element"
test "./tests/list_tests/tail.bnt" "2" "Get list minus first element"
test "./tests/list_tests/combine.bnt" "6" "Combine lists"
test "./tests/list_tests/append.bnt" "6" "Append list to back of other list"
test "./tests/list_tests/size.bnt" "10" "Get list size"
test "./tests/list_tests/range.bnt" "2" "Get sublist"
test "./tests/list_tests/isempty.bnt" "2" "Check if list is empty"
test "./tests/list_tests/sum.bnt" "15" "Get sum of integer elements"
test "./tests/list_tests/product.bnt" "120" "Get product of integer elements"
test "./tests/list_tests/max.bnt" "94" "Get max integer element of list"
test "./tests/list_tests/min.bnt" "3" "Get min integer element of list"
test "./tests/list_tests/sorthl.bnt" "2" "Sort list in non-ascending order"
test "./tests/list_tests/sortlh.bnt" "2" "Sort list in non-descending order"
test "./tests/list_tests/contains.bnt" "2" "Check if element is in list"
test "./tests/list_tests/find.bnt" "3" "Get index of element"
test "./tests/list_tests/map.bnt" "2" "Create new list from mapping"
test "./tests/list_tests/filter.bnt" "2" "Create new list by filtering other list"
test "./tests/list_tests/fill.bnt" "2" "Create new list given element and size"
test "./tests/list_tests/reverse.bnt" "2" "Reverse list"
test "./tests/list_tests/foldl.bnt" "20" "Fold list linear left-associative"
test "./tests/list_tests/foldr.bnt" "14" "Fold list linear right-associative"
test "./tests/list_tests/zip.bnt" "2" "Zip 2-list"
test "./tests/list_tests/union.bnt" "2" "Union 2 lists"
test "./tests/list_tests/intersect.bnt" "2" "Intersect 2 lists"
test "./tests/list_tests/equals.bnt" "2" "Check if two lists are equal"
test "./tests/list_tests/inttochar.bnt" "2" "Int to char"
test "./tests/list_tests/chartoint.bnt" "2" "Char to int"
test "./tests/list_tests/halt.bnt" "25" "Print int and halt"
test "./tests/list_tests/builtins_test01.bnt" "12" "Combo of map, filter, closure from case switch"
echo ""

echo -e "${YELLOW} Miscellaneous ${NONE}"
test "./tests/list_tests/two_d_list.bnt" "6" "Create/access 2d list"
test "./tests/misc_tests/char.bnt" "50" "List of chars"
test "./tests/misc_tests/match.bnt" "3" "Pattern matching"
test "./tests/misc_tests/match_any.bnt" "6" "Any match"
test "./tests/misc_tests/match_func_val.bnt" "2" "Match returns function"
test "./tests/misc_tests/generic.bnt" "2" "Generic function"
test "./tests/misc_tests/generic_list.bnt" "2" "Generic list"
test "./tests/misc_tests/basic_comment.bnt" "5" "Single comment"
test "./tests/misc_tests/comment.bnt" "2" "Multiple comments"
test "./tests/misc_tests/max_rec.bnt" "0" "Max recursive depth"
test "./tests/misc_tests/char_lit_in_app.bnt" "2" "Char literals in function application"
echo ""

echo -e "${YELLOW} \"Real\" Programs ${NONE}"
test "./tests/real_tests/get_max.bnt" "94" "Get max element of list"
test "./tests/real_tests/doubling.bnt" "10" "Double each element in the list"
test "./tests/real_tests/doubling_closure.bnt" "15" "Multiply list using closure"
test "./tests/real_tests/trace.bnt" "15" "Get the trace of a matrix"
test "./tests/real_tests/matr_eq.bnt" "2" "Check if two matrices are equal"
test "./tests/real_tests/fib.bnt" "377" "N-th fibonacci number"
test "./tests/real_tests/mapping.bnt" "2" "Map call that takes in-place function and closure"
test "./tests/real_tests/coding_bat.bnt/square56.bnt" "2" "Map->map->filter, given functions are closures from a case switch"
test "./tests/real_tests/coding_bat.bnt/no14.bnt" "2" "List contains no 1's or 4's"
test "./tests/real_tests/coding_bat.bnt/no9.bnt" "3" "Remove numbers with last digit as 9 from list"
test "./tests/real_tests/coding_bat.bnt/noTeen.bnt" "2" "Filter out x-teen numbers"
test "./tests/real_tests/coding_bat.bnt/two2.bnt" "2" "Double list and filter out numbers ending in 2"
test "./tests/real_tests/coding_bat.bnt/scoresAverage.bnt" "4" "Combo of sum, range, and size: get higher of average scores between 2 halves"
test "./tests/real_tests/coding_bat.bnt/scores100.bnt" "2" "Check if 100s are adjacent"
echo ""
