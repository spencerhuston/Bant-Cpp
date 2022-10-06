# Bant

### Build: **REQUIRES C++17**
Simply clone and run the ```./scripts/makeBant.sh``` script. Run a Bant program using ```[bant directory]/build/bant -f [source file].bnt```. To see debug output use the ```-d``` flag

# Features
_Bant_ is a strongly, statically typed, interpreted, pure functional programming language that supports the following features:
- [Types](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/BantPrimitives.md):
  * _int_
  * _char_
  * _string_
  * _bool_
  * _null_
  * [_List_](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/ListType.md)
  * [_Tuple_](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/TupleType.md)
- [first-class, polymorphic functions](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/Functions.md)
- "product types" (non-recursive)
- standard [arithmetic](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/Arithmetic.md) and [boolean](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/Boolean.md) operations
- [built-in functions](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/BantBuiltins.md) for: _List_ and _Tuple_ manipulation, basic I/O, conversions, etc.
- [control-flow statements](https://github.com/spencerhuston/Bant/blob/main/docs/BantFeatures/ControlFlow.md)
- value case-switching
- comments
- external file importing
