#### Functional
- map: Return list where function _f_ is applied to each element of list _l_<br>
  ````map[T, U](l: List[T], f: (T) -> U) -> List[U]````
- filter: Return list where each element satisfies a boolean function _f_<br>
  ````filter[T, U](l: List[T], element: T, f: U -> bool) -> List[T]````
- foreach: Perform some non-returning operation on each element of a list<br>
  ```foreach[T](l: List[T], f: (T) -> null) -> null```
- foldL: Left-associative linear fold on list _l_, initial value _i_, and function _f_<br>
  ````foldl[T](l: List[T], i: T, f: (T, T) -> T) -> T````
- foldR: Right-associative linear fold on list _l_, initial value _i_, and function _f_<br>
  ````foldr[T](l: List[T], i: T, f: (T, T) -> T) -> T````
- zip: Combine two lists into a list of 2 tuples of corresponding elements<br>
  ````zip[T, U](l1: List[T], l2: List[U]) -> List[Tuple[T, U]]````
