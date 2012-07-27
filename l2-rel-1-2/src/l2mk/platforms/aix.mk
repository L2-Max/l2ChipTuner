# MAKE proj
# Copyright (c) 2006 Arshinkin Maksim 
# All rights reserved.

#################################################
# Paths definitions
#################################################
SDK_DIR:=~/workspace/SDKs/unix/

CORBA_DIR=${SDK_DIR}
CORBA_BIN_DIR=${CORBA_DIR}bin/
CORBA_INCLUDE_DIR=\
	-I${CORBA_DIR}include

CORBA_LIB_DIR=

BOOST_DIR:=${SDK_DIR}boost/

SHELL:=sh

BUILD_BIN_DIR=${BUILD_DIR}
BUILD_LIB_DIR=${BUILD_DIR}
BUILD_SLIB_DIR=${TMP_SLIB_DIR}

#################################################
# Commands definitions
#################################################
MAKE:=gmake

FIND:=find
MKDIR:=mkdir -p
RMDIR:=rmdir
RMFILE:=rm
CP:=cp
LEX=flex
YACC=bison
LS=ls

EXE_MODULE_SUFF=
DLL_MODULE_SUFF=.a

LIB_MODULE_PREF=lib
LIB_MODULE_SUFF=.a

ARC_MODULE_PREF=lib
ARC_MODULE_SUFF=.a

#################################################
# C/C++ compiler definitions
#################################################
CXX=gxlc++

CFLAGS+=-Wx,-qrtti=all -Wx,-qsrcmsg

DEFINES+=\
   -D__UNIX__

INCLUDE_PATH+=

#################################################
# Linker definitions
#################################################
LFLAGS+=

ifeq (${MODULE_TYPE},lib)
   LFLAGS+=-static
endif

LIBS:=

################################################
# Additional libraries
################################################
CORBA_LIBS:=
