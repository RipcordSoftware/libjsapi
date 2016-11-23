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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/application.o \
	${OBJECTDIR}/builder.o \
	${OBJECTDIR}/button.o \
	${OBJECTDIR}/check_button.o \
	${OBJECTDIR}/drawing_area.o \
	${OBJECTDIR}/entry.o \
	${OBJECTDIR}/image_surface.o \
	${OBJECTDIR}/label.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/widget.o \
	${OBJECTDIR}/window.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config gtkmm-3.0 --cflags` 
CXXFLAGS=`pkg-config gtkmm-3.0 --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a ../../externals/installed/lib/libjs_static.ajs -Wl,--whole-archive ../../externals/installed/lib/libmozglue.a -Wl,--no-whole-archive `pkg-config gtkmm-3.0 --libs` -lz  $(LDLIBS)  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlibjsapi_gtkmm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlibjsapi_gtkmm: ../libjsapi/dist/Release/GNU-Linux-x86/libjsapi.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlibjsapi_gtkmm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlibjsapi_gtkmm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/application.o: application.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/application.o application.cpp

${OBJECTDIR}/builder.o: builder.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/builder.o builder.cpp

${OBJECTDIR}/button.o: button.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/button.o button.cpp

${OBJECTDIR}/check_button.o: check_button.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/check_button.o check_button.cpp

${OBJECTDIR}/drawing_area.o: drawing_area.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/drawing_area.o drawing_area.cpp

${OBJECTDIR}/entry.o: entry.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/entry.o entry.cpp

${OBJECTDIR}/image_surface.o: image_surface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/image_surface.o image_surface.cpp

${OBJECTDIR}/label.o: label.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/label.o label.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/widget.o: widget.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/widget.o widget.cpp

${OBJECTDIR}/window.o: window.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../libjsapi -I../../externals/installed/include/mozjs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/window.o window.cpp

# Subprojects
.build-subprojects:
	cd ../libjsapi && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlibjsapi_gtkmm

# Subprojects
.clean-subprojects:
	cd ../libjsapi && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
