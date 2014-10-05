# clang-inspect

To get identifier information at specific position.

## Usage

  clang-inspect <source files> -offset=<line>:<column> -p=compilation_database.json -- [-help]

## HowToBuild

1. Create directory `$(llvm_root)/tools/clang/tools/clang-inspect`
2. `git clone git://github.com/xinhuang/clang-inspect.git`
3. Add `clang-inspect` folder into `$(llvm_root)/tools/clang/tools/Makefile` or `$(llvm_root)/tools/clang/tools/CMakeLists.txt` (Depending on which one you use to build)
4. `make`

