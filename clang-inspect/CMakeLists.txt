set(LLVM_LINK_COMPONENTS
  lineeditor
  support
  )

add_clang_library(clangInspect
  USRFinder.cpp
  USRFindingAction.cpp
  Util.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangDynamicASTMatchers
  clangFrontend
  )

add_subdirectory(tool)

