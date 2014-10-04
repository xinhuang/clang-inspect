set(LLVM_LINK_COMPONENTS
  support
  )

get_filename_component(CLANG_INSPECT_SOURCE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/../../clang-inspect REALPATH)
include_directories(
  ${CLANG_INSPECT_SOURCE_DIR}
  )

add_extra_unittest(ClangInspectTests
  UtilTest.cpp
  USRFindingTest.cpp
  ${CLANG_INSPECT_SOURCE_DIR}/Util.cpp
  )

target_link_libraries(ClangInspectTests
  clangInspect
  clangAST
  clangBasic
  clangFrontend
  clangIndex
  clangTooling
  )

