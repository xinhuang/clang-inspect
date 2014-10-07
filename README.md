# clang-refactor tool set

A place to play with the ClangTooling.

## clang-inspect

To get identifier information at specific position.

## clang-begin

Use `std::begin` `std::end` instead of member function `begin` `end`.

This transformation will only be applied to classes that has both member `begin` `end` defined.

## HowToBuild

1. Create directory `$(llvm_root)/tools/clang/tools/clang-inspect`
2. `git clone git://github.com/xinhuang/clang-inspect.git`
3. Add `clang-inspect` folder into `$(llvm_root)/tools/clang/tools/Makefile` 
   or `$(llvm_root)/tools/clang/tools/CMakeLists.txt`   
   (Depending on which one you use to build)
4. `make`

