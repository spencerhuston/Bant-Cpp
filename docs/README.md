# Bant (WORK IN PROGRESS)
Bant is an interpreted, statically typed, toy functional programming language made purely for my own entertainment

**REQUIRES C++17**

### Build:
Simply clone and run the ```makeBant.sh``` script. Run a Bant program using ```[bant directory]/build/bant source_file.bnt```

### Features:
- Types:
  * _int_
  * _char_
  * _string_
  * _bool_
  * _null_
  * _List_
  * _Tuple_
- first-class functions
- polymorphic functions
- "product types" (non-recursive)
- standard arithmetic and boolean operations
- built-in functions for: _List_ and _Tuple_ manipulation, basic I/O
- control-flow statements
- value case-switching
- static type-checking
- comments
- external file importing

### Features TODO:
- Built-in _string_ functions
- Type inference
- Typeclasses

# Crash Course
**NOTE**: the examples in this guide will use the following format: ```code example``` => ```console output```

TODO

### Arithmetic Operations
Bant supports primitive integer types, with the following operations:
- Addition ```+```
- Subtraction ```-```
- Multiplication ```*```
- Division ```/```
- Modulus ```%```

Regular operator precedence follows, with use of parentheses available to break it up.

Examples:

1.0)
```
printInt(1 + 2 - 3 * 4 / 5)
``` 
=> ```1```

1.1)
```
printInt((1 + (2 - 3)) * 4 / 5)
``` 
=> ```0```

---

### Boolean Operations
Bant also supports boolean types, ```true``` and ```false```, with the following operations:
- Less than ```<```
- Greater than ```>```
- Not ```!```
- Equals ```==```
- Not equal ```!=```
- Less than or equal to ```<=```
- Greater than or equal to ```>=```
- And ```&&```
- Or ```||```

Examples:

2.0)
```
printBool(!true || !false && !(!true))
``` 
=> ```true```

2.1)
```
printBool((5 > 4) != false || (3 + 4 < 5))
``` 
=> ```true```

---

### Control Flow
Bant supports the following conditional statements:

```if```

```else if```

```else```

**NOTE**: An _if_-statement's conditonal expression must be wrapped in parentheses, as the examples will show

Examples:

3.0) (**NOTE**: _if_-statements with no _else_-statement will implicitly return _null_)
```
if (5 > 4) printBool(true)
``` 
=> ```true```

3.1)
```
printInt(if (3 != 4) 3 else 4)
``` 
=> ```3```

3.2)
```
printInt(if (3 != 4 && 4 == 5) 3 else if (3 != 4 && 4 != 5) 4 else 5)
``` 
=> ```4```

---

### Declaring Variables
A variable declaration requires the: name of the variable, type of the variable, and the expression to bind to the name.

You can declare one as such:

```val [name] : [type] = [expression]```

Examples:

4.0)
```
val a : int = 5 + 3;
printInt(a)
``` 
=> ```8```

4.1)
```
val eight : int = 5 + 3;
val not_true : bool = !true;
printInt(eight);
printBool(not_true)
```
=> 
```
8
false
```
**NOTES**:
- Variable names can only contain alphanumeric characters and underscore, and must start with an alphabetic character
- Being a functional language, all variables are immutable (minus _List_ types when used with _[function]InPlace_ builtin functions)

---

### Bant Primitive Types
#### int
Signed 32-bit integers, range of -2147483648 to 2147483647.

#### bool
As shown, Bant's _bool_ type can be either _true_ or _false_

#### char
ASCII code characters denoted with single quotation marks.

Supported escape characters are:
- ```?``` = Single question mark
- ```\``` = Backslash
- ```b``` = Backspace
- ```n``` = Newline
- ```r``` = Return carriage
- ```t``` = Tab
- ```s``` = Space

#### string
Denoted with double quotes and, like _char_ types, supports ASCII characters and the previously listed escape characters 

---

### Lists
Bant's _List_ types can be declared as ```List { value1, value2, ..., valueN }```

When listed as a type, it is denoted ```List[type]``` such as:
- _List_ of _int_: ```List[int]```
- _List_ of list of _char_: ```List[List[char]]```
- _List_ of function type that returns an integer, with an integer argument: ```List[(int) -> int]```

_List_ access looks like function application, being ```listName(index)```

---

### Tuples
While _List_ types can only hold a single type, _Tuple_ types can hold multiple types

_Tuple_ declaration is very similar to _List_ declaration, being: ```Tuple { value1, value2, ... valueN }```

_Tuple_ type definitions must list the type of each element by order of occurrence:
- _Tuple_ of _int_ and _char_: ```Tuple[int, char]```
- _Tuple_ of _Tuple_ of _int_ and _char_, and of _string_: ```Tuple[Tuple[int, char], string]```

_Tuple_ access is done by using the dot operator followed by the index of the element you wish to access:

5.0)
```
val t : Tuple[int, char, string] = Tuple { 5, 'a', "hello" };
printChar(t.1)
```
=> ```a```

**NOTE**: There is currently no support for directly accessing a nested _Tuple_, such as ```t.0.0```. Instead one should create a function to extract the nested _Tuple_ and then access the element from that value

---

### Functions
TODO
#### Functions as values
TODO

#### Closures
TODO

#### Partial Application
TODO

---

### Product Types
TODO

---

### Case-switching
TODO
#### "any" case
TODO

---

### Builtin Functions
Bant supports a wide array of builtin function types, from _List_ manipulation to I/O. The supported functions are defined below.

#### Lists
- insert: Insert element into list at the specified index, returns new list<br>
  ````insert[T](l: List[T], element: T, index: int) -> List[T]````
- remove: Remove element from list at the specified index, returns new list<br>
  ````remove[T](l: List[T], index: int) -> List[T]````
- replace: Replace element in list at the specified index, returns new list<br>
  ````replace[T](l: List[T], element: T, index: int) -> List[T]````
- pushFront: Insert element to front of list, returns new list<br>
  ````pushFront[T](l: List[T], element: T) -> List[T]````
- pushBack: Remove element from front of list, returns new list<br>
  ````pushBack[T](l: List[T], e: T) -> List[T]````
- insertInPlace: Insert element into list at the specified index, returns same list<br>
  ````insertInPlace[T](l: List[T], element: T, index: int) -> List[T]````
- removeInPlace: Remove element from list at the specified index, returns same list<br>
  ````removeInPlace[T](l: List[T], index: int) -> List[T]````
- replaceInPlace: Remove element from list at the specified index, returns same list<br>
  ````replaceInPlace[T](l: List[T], element: T, index: int) -> List[T]````
- pushFrontInPlace: Insert element to front of list, returns same list<br>
  ````pushFrontInPlace[T](l: List[T], element: T) -> List[T]````
- pushBackInPlace: Remove element from front of list, returns same list<br>
  ````pushBackInPlace[T](l: List[T], e: T) -> List[T]````
- front: Get first element of list<br>
  ````front[T](l: List[T]) -> T````
- back: Get last element of list<br>
  ````back[T](l: List[T]) -> T````
- head: Get all elements but the last from a list<br>
  ````head[T](l: List[T]) -> List[T]````
- tail: Get all elements but the first from a list<br>
  ````tail[T](l: List[T]) -> List[T]````
- combine: Append second list to back of first list (returns new list)<br>
  ````combine[T](l1: List[T], l2: List[T]) -> List[T]````
- append: Append second list to back of first list (returns first list)<br>
  ````append[T](l1: List[T], l2: List[T]) -> List[T]````
- size: Get size of list<br>
  ````size[T](l: List[T]) -> int````
- range: Get sublist from [i, j]<br>
  ````range[T](l: List[T], i: int, j: int) -> List[T]````
- isEmpty: Check if list is empty or not (applied to lists of lists will only check the top-level list)<br>
  ````isEmpty[T](l: List[T]) -> bool````
- contains: Check if element exists<br>
  ````contains[T](l: List[T], element: T) -> bool````
- find: Get index of element, if it exists, returns -1 otherwise<br>
  ````find[T](l: List[T], element: T) -> int````
- fill: Create new list of the specified size with each element set to _element_<br>
  ````fill[T](element: T, size: int) -> List[T]````
- reverse: Reverse the list<br>
  ````reverse[T](l: List[T]) -> List[T]````
- union: Return list containing all elements of both lists, without duplicating common ones<br>
  ````union[T](l1: List[T], l2: List[T]) -> List[T]````
- intersect: Return list containing only common elements between two lists<br>
  ````intersect[T](l1: List[T], l2: List[T]) -> List[T]````

#### Integer Lists
- sum: Get sum of elements of a list of integers<br>
  ````sum(l: List[int]) -> int````
- product: Get product of elements of a list of integers<br>
  ````product(l: List[int]) -> int````
- max: Get maximum value in list of integers<br>
  ````max(l: List[int]) -> int````
- min: Get minimum value in list of integers<br>
  ````min(l: List[int]) -> int````
- sortHL: Sort integer list in non-ascending order (high to low)<br>
  ````sortHigh(l: List[int]) -> List[int]````
- sortLH: Sort integer list in non-descending order (low to high)<br>
  ````sortLow(l: List[int]) -> List[int]````

#### Functional
- map: Return list where function _f_ is applied to each element of list _l_<br>
  ````map[T, U](l: List[T], f: (T) -> U) -> List[U]````
- filter: Return list where each element satisfies a boolean function _f_<br>
  ````filter[T, U](l: List[T], element: T, f: U -> bool) -> List[T]````
- foldL: Left-associative linear fold on list _l_, initial value _i_, and function _f_<br>
  ````foldl[T](l: List[T], i: T, f: (T, T) -> T) -> T````
- foldR: Right-associative linear fold on list _l_, initial value _i_, and function _f_<br>
  ````foldr[T](l: List[T], i: T, f: (T, T) -> T) -> T````
- zip: Combine two lists into a list of 2 tuples of corresponding elements<br>
  ````zip[T, U](l1: List[T], l2: List[U]) -> List[Tuple[T, U]]````

#### I/O
- printInt: Print out integer<br>
  ````printInt(i: int) -> null````
- printBool: Print out boolean value as true or false<br>
  ````printBool(b: bool) -> null````
- printList: Print out list as: (e1, e2, ..., eN)<br>
  ````printList[T](l: List[T]) -> null````
- print2Tuple: Print out 2 tuple as: (e1, e2)<br>
  ````print2Tuple[T, U](t: Tuple[T, U]) -> null````
- print3Tuple: Print out 3 tuple as: (e1, e2, e3)<br>
  ````print3Tuple[T, U, V](t: Tuple[T, U, V]) -> null````
- print4Tuple: Print out 4 tuple as: (e1, e2, e3, e4)<br>
  ````print4Tuple[T, U, V, W](t: Tuple[T, U, V, W]) -> null````
- readChar: Read a char from input<br>
  ````readChar() -> char````
- printChar: Output a char to console<br>
  ````printChar(c: char) -> null````
- readString: Read a string from input<br>
  ````readString() -> string````
- printString: Print a string<br>
  ````printString(s: string) -> null````

#### Conversion
- intToString: Convert integer to string representation<br>
  ````intToString(i: int) -> string````
- stringToInt: Convert numeric string to integer<br>
  ````stringToInt(s: string) -> int````
- stringToCharList: Convert string to char list<br>
  ````stringToCharList(s: string) -> List[char]````
- charListToString: Convert char list to string<br>
  ````charListToString(l: List[char]) -> string````

#### String
- concat: Concatenate two strings _s1_ and _s2_ together<br>
  ````concat(s1: string, s2: string) -> string````
- substr: Get substring from string _s_ starting at index _start_ and up to but not including index _end_<br>
  ````substr(s: string, start: int, end: int) -> string````
- charAt: Get the char at index _i_ of string _s_<br>
  ````charAt(s: string, i: int) -> char````

#### Other
- halt: Exit program immediately<br>
  ````halt() -> null````
- equals: Check if value 1 equals value 2, applies to all types but functions and product-types<br>
  ````equals[T](v1: T, v2: T) -> bool````
- rand: Returns a random integer between an lowerbound _l_ and an upperbound _u_<br>
  ````rand(l: int, u: int) -> int````

---

### Comments
TODO

---

### File Importing
TODO
