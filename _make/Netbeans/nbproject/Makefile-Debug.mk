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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1404715117/ArtDatabase.o \
	${OBJECTDIR}/_ext/1445274692/LibSUNAI.o \
	${OBJECTDIR}/_ext/1404715117/LabelDictionary.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../lib -Wl,-rpath ../../bin

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSUNAI.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSUNAI.so: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSUNAI.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1404715117/ArtDatabase.o: ../../src/ArtDatabase/ArtDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1404715117
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1404715117/ArtDatabase.o ../../src/ArtDatabase/ArtDatabase.cpp

${OBJECTDIR}/_ext/1445274692/LibSUNAI.o: ../../src/LibSUNAI.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445274692/LibSUNAI.o ../../src/LibSUNAI.cpp

${OBJECTDIR}/_ext/1404715117/LabelDictionary.o: ../../src/ArtDatabase/LabelDictionary.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1404715117
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1404715117/LabelDictionary.o ../../src/ArtDatabase/LabelDictionary.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSUNAI.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
