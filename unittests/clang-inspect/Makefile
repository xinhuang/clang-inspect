CLANG_LEVEL = ../../../..
include $(CLANG_LEVEL)/../../Makefile.config

TESTNAME = ClangInspectTests
LINK_COMPONENTS := asmparser bitreader support MC MCParser option \
		 TransformUtils
USEDLIBS = clangInspect.a clangTooling.a clangFrontend.a clangSerialization.a \
	   clangDriver.a clangParse.a clangSema.a clangEdit.a clangAnalysis.a \
	   clangAST.a clangASTMatchers.a clangDynamicASTMatchers.a clangLex.a \
	   clangBasic.a

include $(CLANG_LEVEL)/Makefile
MAKEFILE_UNITTEST_NO_INCLUDE_COMMON := 1
CPP.Flags += -I$(PROJ_SRC_DIR)/../../clang-inspect
include $(LLVM_SRC_ROOT)/unittests/Makefile.unittest
