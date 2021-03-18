#include "../includes/defs/builtin/builtinDefinitions.hpp"

const std::vector<std::string> BuiltinDefinitions::builtinNames{
    "insert", "remove", "replace", 
    "pushFront", "pushBack",
    "insertInPlace", "removeInPlace", "replaceInPlace",
    "pushFrontInPlace", "pushBackInPlace",
    "front", "back",
    "head", "tail",
    "combine", "append", "size",
    "range", "isEmpty",
    "sum", "product", "max", "min", "sortlh", "sorthl",
    "contains", "find",
    "map", "filter",
    "fill", "reverse",
    "foldl", "foldr",
    "zip", "union", "intersect", "equals",
    "intToChar", "charToInt", "stringToCharList", "charListToString",
    "printInt",
    "printBool",
    "printList", "print2Tuple", "print3Tuple", "print4Tuple",
    "readChar", "printChar",
    "readString", "printString",
    "halt"
};

int
BuiltinDefinitions::builtinNumber() {
    return static_cast<int>(BuiltinEnums::BUILTINNUM);
}

const std::string BuiltinDefinitions::builtinDefinitions = 
R"(func insert[T](l: List[T], e: T, index: int) -> List[T] = { 0 };
func remove[T](l: List[T], index: int) -> List[T] = { 0 };
func replace[T](l: List[T], e: T, index: int) -> List[T] = { 0 };
func pushFront[T](l: List[T], e: T) -> List[T] = { 0 };
func pushBack[T](l: List[T], e: T) -> List[T] = { 0 };
func insertInPlace[T](l: List[T], e: T, index: int) -> List[T] = { 0 };
func removeInPlace[T](l: List[T], index: int) -> List[T] = { 0 };
func replaceInPlace[T](l: List[T], e: T, index: int) -> List[T] = { 0 };
func pushFrontInPlace[T](l: List[T], e: T) -> List[T] = { 0 };
func pushBackInPlace[T](l: List[T], e: T) -> List[T] = { 0 };
func front[T](l: List[T]) -> T = { 0 };
func back[T](l: List[T]) -> T = { 0 };
func head[T](l: List[T]) -> List[T] = { 0 };
func tail[T](l: List[T]) -> List[T] = { 0 };
func combine[T](l1: List[T], l2: List[T]) -> List[T] = { 0 };
func append[T](l1: List[T], l2: List[T]) -> List[T] = { 0 };
func size[T](l: List[T]) -> int = { 0 };
func range[T](l: List[T], i: int, j: int) -> List[T] = { 0 };
func isEmpty[T](l: List[T]) -> bool = { 0 };
func sum(l: List[int]) -> int = { 0 };
func product(l: List[int]) -> int = { 0 };
func max(l: List[int]) -> int = { 0 };
func min(l: List[int]) -> int = { 0 };
func sortlh(l: List[int]) -> List[int] = { 0 };
func sorthl(l: List[int]) -> List[int] = { 0 };
func contains[T](l: List[T], e: T) -> bool = { 0 };
func find[T](l: List[T], e: T) -> int = { 0 };
func map[T](l: List[T], f: (T) -> T) -> List[T] = { 0 };
func filter[T](l: List[T], f: (T) -> bool) -> List[T] = { 0 };
func fill[T](e: T, s: int) -> List[T] = { 0 };
func reverse[T](l: List[T]) -> List[T] = { 0 };
func foldl[T](l: List[T], f: (T, T) -> T) -> T = { 0 };
func foldr[T](l: List[T], f: (T, T) -> T) -> T = { 0 };
func zip[T, U](l1: List[T], l2: List[U]) -> List[Tuple[T, U]] = { 0 };
func union[T](l1: List[T], l2: List[T]) -> List[T] = { 0 };
func intersect[T](l1: List[T], l2: List[T]) -> List[T] = { 0 };
func equals[T](l1: List[T], l2: List[T]) -> bool = { 0 };
func intToChar(i: int) -> char = { 0 };
func charToInt(c: char) -> int = { 0 };
func stringToCharList(s: string) -> List[char] = { 0 };
func charListToString(l: List[char]) -> string = { 0 };
func printInt(i: int) -> null = { 0 };
func printBool(b: bool) -> null = { 0 };
func printList[T](l: List[T]) -> null = { 0 };
func print2Tuple[T, U](t: Tuple[T, U]) -> null = { 0 };
func print3Tuple[T, U, V](t: Tuple[T, U, V]) -> null = { 0 };
func print4Tuple[T, U, V, W](t: Tuple[T, U, V, W]) -> null = { 0 };
func readChar() -> char = { 0 };
func printChar(c: char) -> null = { 0 };
func readString() -> string = { 0 };
func printString(s: string) -> null = { 0 };
func halt() -> null = { 0 };
)";

bool
BuiltinDefinitions::isBuiltin(const std::string & functionIdent) {
    return (std::find(builtinNames.begin(), builtinNames.end(), functionIdent) != builtinNames.end());
}

BuiltinDefinitions::BuiltinEnums
BuiltinDefinitions::getBuiltin(const std::string & builtinName) {
    return (static_cast<BuiltinEnums>(std::distance(builtinNames.begin(), std::find(builtinNames.begin(), builtinNames.end(), builtinName))));
}