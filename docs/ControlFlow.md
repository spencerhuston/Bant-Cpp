### Control Flow
Bant supports the following conditional statements:

```if```

```else if```

```else```

**NOTES**: 
* An _if_-statement's conditonal expression must be wrapped in parentheses
* An _if_-statement with no _else_-statement will implicitly return _null_

Examples:

```
if (5 > 4) printBool(true)
``` 
=> ```true```

<br>

```
printInt(if (5 > 4) 5)
``` 
=> 
```
Line: 1, Column: 15
Mismatched type: null, Expected: int
printInt(if (5

Line: 1, Column: 22
Mismatched type: int, Expected: null
printInt(if (5 > 4) 5) 

One or more errors occurred during type checking, exiting
```

<br>

```
printInt(if (3 != 4) 3 else 4)
``` 
=> ```3```

<br>

```
printInt(if (3 != 4 && 4 == 5) 3 else if (3 != 4 && 4 != 5) 4 else 5)
``` 
=> ```4```
