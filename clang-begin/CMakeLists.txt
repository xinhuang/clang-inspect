add_clang_executable(clang-begin
  ClangBegin.cpp
  UseBeginAction.cpp
  UseBeginMatcher.cpp
  IncludeFinder.cpp)
target_link_libraries(clang-begin
  clangASTMatchers
  clangBasic
  clangTooling
  )

install(TARGETS clang-begin RUNTIME DESTINATION bin)
