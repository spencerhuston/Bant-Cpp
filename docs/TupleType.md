### Tuples
While _List_ types can only hold a single type, _Tuple_ types can hold multiple types

_Tuple_ declaration is very similar to _List_ declaration, being: ```Tuple { value1, value2, ... valueN }```

_Tuple_ type definitions must list the type of each element by order of occurrence:
- _Tuple_ of _int_ and _char_: ```Tuple[int, char]```
- _Tuple_ of _Tuple_ of _int_ and _char_, and of _string_: ```Tuple[Tuple[int, char], string]```

_Tuple_ access is done by using the dot operator followed by the index of the element you wish to access (**NOTE**: There is currently no support for directly accessing a nested _Tuple_, such as ```t.0.0```. Instead one should create a function to extract the nested _Tuple_ and then access the element from that value

```
val t : Tuple[int, char, string] = Tuple { 5, 'a', "hello" };
printChar(t.1)
```
=> ```a```
