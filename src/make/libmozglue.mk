LIBMOZGLUE_DYLIB=$(1)/libmozglue.dylib
LIBMOZGLUE_A=$(1)/libmozglue.a

LIBMOZGLUE=$(shell if [ -e $(call LIBMOZGLUE_DYLIB, $(1)) ]; then echo $(call LIBMOZGLUE_DYLIB, $(1)); else echo "-Wl,--whole-archive $(call LIBMOZGLUE_A, $(1)) -Wl,--no-whole-archive"; fi)
