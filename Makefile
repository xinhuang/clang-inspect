CLANG_LEVEL := ../..

include $(CLANG_LEVEL)/../../Makefile.config

PARALLEL_DIRS := 
DIRS := clang-inspect unittests

include $(CLANG_LEVEL)/Makefile

###
# Handle the nested test suite.

ifneq ($(PROJ_SRC_ROOT),$(PROJ_OBJ_ROOT))
$(RecursiveTargets)::
	$(Verb) for dir in test; do \
	  if [ -f $(PROJ_SRC_DIR)/$${dir}/Makefile ] && [ ! -f $${dir}/Makefile ]; then \
	    $(MKDIR) $${dir}; \
	    $(CP) $(PROJ_SRC_DIR)/$${dir}/Makefile $${dir}/Makefile; \
	  fi \
	done
endif

test::
	@ $(MAKE) -C test

report::
	@ $(MAKE) -C test report

clean::
	@ $(MAKE) -C test clean

.PHONY: test report clean
