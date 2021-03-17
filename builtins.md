## Builtin functions
- Insert: Insert element into list at the specified index, returns new list<br>
  ````insert[T](l: List[T], element: T, index: int) -> List[T]````
- Remove: Remove element from list at the specified index, returns new list<br>
  ````remove[T](l: List[T], index: int) -> List[T]````
- Replace: Replace element in list at the specified index, returns new list<br>
  ````replace[T](l: List[T], element: T, index: int) -> List[T]````
- PushFront: Insert element to front of list, returns new list<br>
  ````pushFront[T](l: List[T], element: T) -> List[T]````
- PushBack: Remove element from front of list, returns new list<br>
  ````pushBack[T](l: List[T], e: T) -> List[T]````
- InsertInPlace: Insert element into list at the specified index, returns same list<br>
  ````insertInPlace[T](l: List[T], element: T, index: int) -> List[T]````
- RemoveInPlace: Remove element from list at the specified index, returns same list<br>
  ````removeInPlace[T](l: List[T], index: int) -> List[T]````
- ReplaceInPlace: Remove element from list at the specified index, returns same list<br>
  ````replaceInPlace[T](l: List[T], element: T, index: int) -> List[T]````
- PushFrontInPlace: Insert element to front of list, returns same list<br>
  ````pushFrontInPlace[T](l: List[T], element: T) -> List[T]````
- PushBackInPlace: Remove element from front of list, returns same list<br>
  ````pushBackInPlace[T](l: List[T], e: T) -> List[T]````
- Front: Get first element of list<br>
  ````front[T](l: List[T]) -> T````
- Back: Get last element of list<br>
  ````back[T](l: List[T]) -> T````
- Head: Get all elements but the last from a list<br>
  ````head[T](l: List[T]) -> List[T]````
- Tail: Get all elements but the first from a list<br>
  ````tail[T](l: List[T]) -> List[T]````
- Combine: Return new list consisting of two appended lists<br>
  ````combine[T](l1: List[T], l2: List[T]) -> List[T]````
- Append: Append second list to back of first list<br>
  ````combine[T](l1: List[T], l2: List[T]) -> List[T]````
- Size: Get size of list<br>
  ````size[T](l: List[T]) -> int````
- Range: Get sublist from [i, j]<br>
  ````range[T](l: List[T], i: int, j: int) -> List[T]````
- IsEmpty: Check if list is empty or not (applied to lists of lists will only check the top-level list)<br>
  ````isEmpty[T](l: List[T]) -> bool````
- Sum: Get sum of elements of a list of integers<br>
  ````sum(l: List[int]) -> int````
- Product: Get product of elements of a list of integers<br>
  ````product(l: List[int]) -> int````
- Max: Get maximum value in list of integers<br>
  ````max(l: List[int]) -> int````
- Min: Get minimum value in list of integers<br>
  ````min(l: List[int]) -> int````
- SortHL: Sort integer list in non-ascending order (high to low)<br>
  ````sortHigh(l: List[int]) -> List[int]````
- SortLH: Sort integer list in non-descending order (low to high)<br>
  ````sortLow(l: List[int]) -> List[int]````
- Contains: Check if element exists<br>
  ````contains[T](l: List[T], element: T) -> bool````
- Find: Get index of element, if it exists, returns -1 otherwise<br>
  ````find[T](l: List[T], element: T) -> int````
- Map: Return list where function _f_ is applied to each element of list _l_<br>
  ````map[T, U](l: List[T], f: U -> T) -> List[T]````
- Filter: Return list where each element satisfies a boolean function _f_<br>
  ````filter[T, U](l: List[T], element: T, f: U -> bool) -> List[T]````
- Fill: Create new list of the specified size with each element set to _element_<br>
  ````fill[T](element: T, size: int) -> List[T]````
- Reverse: Reverse the list<br>
  ````reverse[T](l: List[T]) -> List[T]````
- FoldL: Left-associative linear fold<br>
  ````foldl[T](l: List[T], f: (T, T) -> T) -> T````
- FoldR: Right-associative linear fold<br>
  ````foldr[T](l: List[T], f: (T, T) -> T) -> T````
- Zip: Combine two lists into a list of 2 tuples of corresponding elements<br>
  ````zip[T, U](l1: List[T], l2: List[U]) -> List[Tuple[T, U]]````
- Union: Return list containing all elements of both lists, without duplicating common ones<br>
  ````union[T](l1: List[T], l2: List[T]) -> List[T]````
- Intersect: Return list containing only common elements between two lists<br>
  ````intersect[T](l1: List[T], l2: List[T]) -> List[T]````
- IntToChar: Convert integer to char<br>
  ````intToChar(i: int) -> char````
- CharToInt: Convert char to integer<br>
  ````charToInt(c: char) -> int````
- StringToCharList: Convert string to char list<br>
  ````stringToCharList(s: string) -> List[char]````
- CharListToString: Convert char list to string<br>
  ````charListToString(l: List[char]) -> string````
- PrintInt: Print out integer<br>
  ````printInt(i: int) -> null````
- PrintBool: Print out boolean value as true or false<br>
  ````printBool(b: bool) -> null````
- PrintList: Print out list as: (e1, e2, ..., eN)<br>
  ````printList[T](l: List[T]) -> null````
- Print2Tuple: Print out 2 tuple as: (e1, e2)<br>
  ````print2Tuple[T, U](t: Tuple[T, U]) -> null````
- Print3Tuple: Print out 3 tuple as: (e1, e2, e3)<br>
  ````print3Tuple[T, U, V](t: Tuple[T, U, V]) -> null````
- Print4Tuple: Print out 4 tuple as: (e1, e2, e3, e4)<br>
  ````print4Tuple[T, U, V, W](t: Tuple[T, U, V, W]) -> null````
- ReadChar: Read a char from input<br>
  ````readChar() -> char````
- PrintChar: Output a char to console<br>
  ````printChar(c: char) -> null````
- ReadString: Read a string from input<br>
  ````readString() -> string````
- PrintString: Print a string<br>
  ````printString(s: string) -> null````
- Halt: Exit program immediately<br>
  ````halt() -> null````
