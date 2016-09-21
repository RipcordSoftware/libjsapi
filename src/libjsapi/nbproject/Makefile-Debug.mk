#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/context.o \
	${OBJECTDIR}/context_state.o \
	${OBJECTDIR}/dynamic_array.o \
	${OBJECTDIR}/dynamic_object.o \
	${OBJECTDIR}/function_arguments.o \
	${OBJECTDIR}/global.o \
	${OBJECTDIR}/object.o \
	${OBJECTDIR}/runtime.o \
	${OBJECTDIR}/script.o \
	${OBJECTDIR}/value.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f6 \
	${TESTDIR}/TestFiles/f12 \
	${TESTDIR}/TestFiles/f13 \
	${TESTDIR}/TestFiles/f8 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f11 \
	${TESTDIR}/TestFiles/f9 \
	${TESTDIR}/TestFiles/f7 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f10

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=$(COVERAGE_FLAGS)
CXXFLAGS=$(COVERAGE_FLAGS)

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a

${OBJECTDIR}/context.o: context.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/context.o context.cpp

${OBJECTDIR}/context_state.o: context_state.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/context_state.o context_state.cpp

${OBJECTDIR}/dynamic_array.o: dynamic_array.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_array.o dynamic_array.cpp

${OBJECTDIR}/dynamic_object.o: dynamic_object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_object.o dynamic_object.cpp

${OBJECTDIR}/function_arguments.o: function_arguments.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/function_arguments.o function_arguments.cpp

${OBJECTDIR}/global.o: global.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/global.o global.cpp

${OBJECTDIR}/object.o: object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/object.o object.cpp

${OBJECTDIR}/runtime.o: runtime.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/runtime.o runtime.cpp

${OBJECTDIR}/script.o: script.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script.o script.cpp

${OBJECTDIR}/value.o: value.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/value.o value.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f5: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f5: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f5: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/call_js_function_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f6: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f6: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f6: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/call_native_function_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f12: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f12: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f12: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f12: ${TESTDIR}/tests/function_arguments_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f13: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f13: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f13: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f13: ${TESTDIR}/tests/gc_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f13 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f8: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f8: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f8: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f8: ${TESTDIR}/tests/global_property_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f3: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f3: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f3: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/multi_context_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f4: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f4: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f4: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/multi_runtime_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f2: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f2: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f2: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/script_exception_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f11: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f11: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f11: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f11: ${TESTDIR}/tests/simple_dynamic_array_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f9: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f9: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f9: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f9: ${TESTDIR}/tests/simple_dynamic_object_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f7: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f7: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f7: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/simple_object_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f1: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f1: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f1: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/simple_script_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   

${TESTDIR}/TestFiles/f10: ../../externals/installed/lib/libjs_static.ajs

${TESTDIR}/TestFiles/f10: ../../externals/installed/lib/libgtest.a

${TESTDIR}/TestFiles/f10: ../../externals/installed/lib/libgtest_main.a

${TESTDIR}/TestFiles/f10: ${TESTDIR}/tests/simple_value_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} $(COVERAGE_FLAGS)  -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} -lpthread `pkg-config --libs zlib` $(LDLIBS)   


${TESTDIR}/tests/call_js_function_tests.o: tests/call_js_function_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/call_js_function_tests.o tests/call_js_function_tests.cpp


${TESTDIR}/tests/call_native_function_tests.o: tests/call_native_function_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/call_native_function_tests.o tests/call_native_function_tests.cpp


${TESTDIR}/tests/function_arguments_tests.o: tests/function_arguments_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/function_arguments_tests.o tests/function_arguments_tests.cpp


${TESTDIR}/tests/gc_tests.o: tests/gc_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/gc_tests.o tests/gc_tests.cpp


${TESTDIR}/tests/global_property_tests.o: tests/global_property_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/global_property_tests.o tests/global_property_tests.cpp


${TESTDIR}/tests/multi_context_tests.o: tests/multi_context_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/multi_context_tests.o tests/multi_context_tests.cpp


${TESTDIR}/tests/multi_runtime_tests.o: tests/multi_runtime_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/multi_runtime_tests.o tests/multi_runtime_tests.cpp


${TESTDIR}/tests/script_exception_tests.o: tests/script_exception_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/script_exception_tests.o tests/script_exception_tests.cpp


${TESTDIR}/tests/simple_dynamic_array_tests.o: tests/simple_dynamic_array_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/simple_dynamic_array_tests.o tests/simple_dynamic_array_tests.cpp


${TESTDIR}/tests/simple_dynamic_object_tests.o: tests/simple_dynamic_object_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/simple_dynamic_object_tests.o tests/simple_dynamic_object_tests.cpp


${TESTDIR}/tests/simple_object_tests.o: tests/simple_object_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/simple_object_tests.o tests/simple_object_tests.cpp


${TESTDIR}/tests/simple_script_tests.o: tests/simple_script_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/simple_script_tests.o tests/simple_script_tests.cpp


${TESTDIR}/tests/simple_value_tests.o: tests/simple_value_tests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -I../../externals/installed/include -I. -std=c++11 $(COVERAGE_FLAGS) -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/simple_value_tests.o tests/simple_value_tests.cpp


${OBJECTDIR}/context_nomain.o: ${OBJECTDIR}/context.o context.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/context.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/context_nomain.o context.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/context.o ${OBJECTDIR}/context_nomain.o;\
	fi

${OBJECTDIR}/context_state_nomain.o: ${OBJECTDIR}/context_state.o context_state.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/context_state.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/context_state_nomain.o context_state.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/context_state.o ${OBJECTDIR}/context_state_nomain.o;\
	fi

${OBJECTDIR}/dynamic_array_nomain.o: ${OBJECTDIR}/dynamic_array.o dynamic_array.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/dynamic_array.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_array_nomain.o dynamic_array.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/dynamic_array.o ${OBJECTDIR}/dynamic_array_nomain.o;\
	fi

${OBJECTDIR}/dynamic_object_nomain.o: ${OBJECTDIR}/dynamic_object.o dynamic_object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/dynamic_object.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_object_nomain.o dynamic_object.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/dynamic_object.o ${OBJECTDIR}/dynamic_object_nomain.o;\
	fi

${OBJECTDIR}/function_arguments_nomain.o: ${OBJECTDIR}/function_arguments.o function_arguments.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/function_arguments.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/function_arguments_nomain.o function_arguments.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/function_arguments.o ${OBJECTDIR}/function_arguments_nomain.o;\
	fi

${OBJECTDIR}/global_nomain.o: ${OBJECTDIR}/global.o global.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/global.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/global_nomain.o global.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/global.o ${OBJECTDIR}/global_nomain.o;\
	fi

${OBJECTDIR}/object_nomain.o: ${OBJECTDIR}/object.o object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/object.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/object_nomain.o object.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/object.o ${OBJECTDIR}/object_nomain.o;\
	fi

${OBJECTDIR}/runtime_nomain.o: ${OBJECTDIR}/runtime.o runtime.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/runtime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/runtime_nomain.o runtime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/runtime.o ${OBJECTDIR}/runtime_nomain.o;\
	fi

${OBJECTDIR}/script_nomain.o: ${OBJECTDIR}/script.o script.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/script.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/script_nomain.o script.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/script.o ${OBJECTDIR}/script_nomain.o;\
	fi

${OBJECTDIR}/value_nomain.o: ${OBJECTDIR}/value.o value.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/value.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../externals/installed/include/mozjs- -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/value_nomain.o value.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/value.o ${OBJECTDIR}/value_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
	    ${TESTDIR}/TestFiles/f12 || true; \
	    ${TESTDIR}/TestFiles/f13 || true; \
	    ${TESTDIR}/TestFiles/f8 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f11 || true; \
	    ${TESTDIR}/TestFiles/f9 || true; \
	    ${TESTDIR}/TestFiles/f7 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f10 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjsapi.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
