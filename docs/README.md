# Bant (WORK IN PROGRESS)

### Build: **REQUIRES C++17**
Simply clone and run the ```./scripts/makeBant.sh``` script. Run a Bant program using ```[bant directory]/build/bant -f [source file].bnt```. To see debug output use the ```-d``` flag

# Features
_Bant_ is a strongly, statically typed, interpreted, pure functional programming language that supports the following features:
- Types:
  * _int_
  * _char_
  * _string_
  * _bool_
  * _null_
  * _List_
  * _Tuple_
- first-class, polymorphic functions
- "product types" (non-recursive)
- standard arithmetic and boolean operations
- built-in functions for: _List_ and _Tuple_ manipulation, basic I/O, conversions, etc.
- control-flow statements
- value case-switching
- comments
- external file importing
