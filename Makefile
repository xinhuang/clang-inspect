CLANG_LEVEL := ../..

include $(CLANG_LEVEL)/../../Makefile.config

PARALLEL_DIRS := clang-begin
DIRS := clang-inspect unittests

include $(CLANG_LEVEL)/Makefile

