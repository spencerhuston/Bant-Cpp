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
