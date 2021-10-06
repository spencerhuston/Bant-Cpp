### Declaring Variables
A variable declaration requires the: name of the variable, type of the variable, and the expression to bind to the name.

You can declare one as such:

```val [name] : [type] = [expression]```

**NOTES**:
* Variable names can only contain alphanumeric characters and underscore, and must start with an alphabetic character
* All variables are immutable (minus _List_ types when used with _[function]InPlace_ builtin functions)

Examples:

```
val a : int = 5 + 3;
printInt(a)
``` 
=> ```8```

<br>

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
