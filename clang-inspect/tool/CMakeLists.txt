include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_clang_executable(clang-inspect ClangInspect.cpp)
target_link_libraries(clang-inspect
  clangAST
  clangBasic
  clangFrontend
  clangIndex
  clangRewrite
  clangTooling
  )

install(TARGETS clang-inspect RUNTIME DESTINATION bin)
