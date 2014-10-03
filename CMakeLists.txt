add_subdirectory(clang-inspect)

if(CLANG_ENABLE_STATIC_ANALYZER)
add_subdirectory(test)
add_subdirectory(unittests)
endif()
