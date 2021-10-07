Literal:
==========
l
=>
val l$x : t = l;

Primitive:
x prim y
=>
val l$x : t = x;
val l$x+1 : t = y;
l$x prim l$x+1

Branch:
==========
if (x bool-prim y) [true-branch] else [false-branch]
=>
func cf$() -> null = { true-branch };
func ct$() -> null = { false-branch };
if (x bool-prim y) ct$() else  cf$()

Program:
==========
func name1(arg1, arg2, ..., argn) -> type = {
	body
};

func ...

programBody
=>
func name1(c$, arg1, arg2, ..., argn) -> type = {
	body;
	c$(return)
};

func ...

func c$() -> null = { programBody }

FuncApp:
==========
funcName(arg1, arg2, ..., argN)
=>
val l$x : t = value1;
val l$x+1 : t = value2;
...
val l$x+n : t = valueN;
funcName(c$, l$x, l$x+1, ..., l$x+n)

ListDef:
==========
List { value1, value2, ..., valueN };
=>
val l$x : t = value1;
val l$x+1 : t = value2;
...
val l$x+n : t = valueN;
List { l$x, l$x+1, ..., l$x+n }

TupleDef:
==========
Tuple { value1, value2, ..., valueN };
=>
val l$x : t = value1;
val l$x+1 : t = value2;
...
val l$x+n : t = valueN;
Tuple { l$x, l$x+1, ..., l$x+n }

Match:
==========
match (ref) {
	case exp = { case1 body };
	case exp2 = { case2 body };
	...
	case expN = { caseN body };
	case any = { case any body };
}
=>
func c$1() -> null = { case1 body; c(val) };
func c$2() -> null = { case2 body; c(val) };
...
func c$N() -> null = { caseN body; c(val) };
func c$Any$0() -> null = { case any body; c(val) };
func c$(val: t) -> null = { val }
match (ref) {
	case exp = { c1$() };
	case exp2 = { c2$() };
	...
	case expN = { cN() };
	case any = { c$Any$0() };
}

Unchanged:
- TypeclassDef
- Let
- Reference