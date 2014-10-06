add_subdirectory(clang-inspect)
add_subdirectory(clang-begin)

if(CLANG_ENABLE_STATIC_ANALYZER)
add_subdirectory(unittests)
endif()
