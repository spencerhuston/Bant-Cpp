### Lists
Bant's _List_ types can be declared as ```List { value1, value2, ..., valueN }```

When listed as a type, it is denoted ```List[type]``` such as:
- _List_ of _int_: ```List[int]```
- _List_ of list of _char_: ```List[List[char]]```
- _List_ of function type that returns an integer, with an integer argument: ```List[(int) -> int]```

_List_ access looks like function application, being ```listName(index)```. Nested access would be successive applications, like ```listName(index1)(index2)```.
