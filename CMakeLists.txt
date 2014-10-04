add_subdirectory(clang-inspect)

if(CLANG_ENABLE_STATIC_ANALYZER)
add_subdirectory(unittests)
endif()
