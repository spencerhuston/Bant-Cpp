### Functions
Bant supports polymorphic first-class functions, allowing for: functions as values, nested functions, closures, currying, and partial application.

You can declare one as: 

```func functionName(parameter1: type, ..., parameterN: type) -> returnType = { function body };```

Function application works like any other language, being ```functionName(argument1, ..., argumentN)```

**NOTES**:
* All arguments are passed by value (for user-defiend functions)
* Mutual recursion is supported
* Currently, tail-call optimization/trampolining is NOT implemented, so the interpreter runs out of stack space at about 12k nested calls. There is a planned optimization phase to fix this using continuations.

Examples:

Simple function that accepts an integer and returns it added with the value 2
```
func addTwo(i: int) -> int = x + 2;
printInt(addTwo(3))
```
=> ```5```

<br>

Function that takes an _int_ and a _char_ and returns a _Tuple_ made of the two values
```
func makeTuple(i: int, c: char) -> Tuple[int, char] = {
    Tuple { i, c }
};

print2Tuple[int, char](makeTuple(5, 'a'))
```
=> ```(5, 'a')```

<br>

Recursive function (fibonacci)
```
func fib(n: int) -> int = {
	if (n <= 1)
		n
	else 
		fib(n - 1) + fib(n - 2)
};

printInt(fib(9))
```
=> ```34```

#### Functions as values
Being first-class citizens, functions can be stored in variables, passed to other functions, and returned from functions

Example:
```
val h : (int) -> int = {
    func f(x: int) -> int = x + 2;
    f
};
printInt(h(3)) 
```
=> ```5```

<br>

#### Closures, Currying, and Partial Application
Functions can be returned from other functions with certain variables permanently set to a value from outside the function scope that is deduced during their construction. Note that Bant currently does not support recursive closures.

In this example, we construct an instance of the function _g_ inside of _f_ where _x_ will always be equal to the value 2. This instance is then stored in _h_
```
func f(x: int) -> (int) -> int = {
    func g(y: int) -> int = {
        x + y
    };
    g
};
val h : (int) -> int = f(2);
printInt(h(3))
```
=> ```5```

<br>

Here we directly call the instance of _g_ that is returned from _f_ with _x_ set to 2.
```
func f(x: int) -> (int) -> int = {
    func g(y: int) -> int = {
        x + y
    };
    g
};
printInt(f(2)(3))
```
=> ```5```

#### Polymorphic Functions
One of Bant's most powerful tools is polymorphic, or generic, functions. Most of the builtin functions rely on this and it makes code far more reusable and composable. Note that Bant currently does not support recursive polymorphic functions.

To define a polymorphic function, simply denote the generic type names in square brackets after the function name:
```
func functionName[genericTypeName1, ..., genericTypeNameN](parameter1: type, ..., parameterN: type) -> returnType = { 
    function body 
};
```

A function application to a polymorphic function must include the types to use for that call:

```functionName[type1, ..., typeN](...)```

Examples:
TODO
