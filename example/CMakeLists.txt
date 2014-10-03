set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangFrontend clangSerialization clangDriver
    clangTooling clangParse clangSema clangAnalysis clangewriteFrontend
    clangRewrite clangEdit clangAST clangLex clangBasic)

add_clang_executable(example
    Example.cpp
)

target_link_libraries(example
    asmparse
    bitreader
    support
    mc
    option
)

