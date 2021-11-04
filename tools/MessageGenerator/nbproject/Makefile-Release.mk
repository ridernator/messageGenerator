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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/src/CppGenerator.o \
	${OBJECTDIR}/src/Definitions.o \
	${OBJECTDIR}/src/FileParser.o \
	${OBJECTDIR}/src/Generator.o \
	${OBJECTDIR}/src/JavaGenerator.o \
	${OBJECTDIR}/src/MessageGenerator.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=--std=c++20 -Wextra
CXXFLAGS=--std=c++20 -Wextra

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lxerces-c

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/messagegenerator

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/messagegenerator: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/messagegenerator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/CppGenerator.o: src/CppGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CppGenerator.o src/CppGenerator.cpp

${OBJECTDIR}/src/Definitions.o: src/Definitions.cxx
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Definitions.o src/Definitions.cxx

${OBJECTDIR}/src/FileParser.o: src/FileParser.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/FileParser.o src/FileParser.cpp

${OBJECTDIR}/src/Generator.o: src/Generator.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Generator.o src/Generator.cpp

${OBJECTDIR}/src/JavaGenerator.o: src/JavaGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/JavaGenerator.o src/JavaGenerator.cpp

${OBJECTDIR}/src/MessageGenerator.o: src/MessageGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageGenerator.o src/MessageGenerator.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
