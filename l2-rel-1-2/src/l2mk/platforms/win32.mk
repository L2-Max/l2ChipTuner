# MAKE proj
# Copyright (c) 2006 Arshinkin Maksim 
# All rights reserved.

#################################################
# Paths definitions
#################################################
SDK_DIR:=D:/workspace/SDKs/win/

CORBA_DIR=${SDK_DIR}omniORB/4.1.0/
CORBA_BIN_DIR=${CORBA_DIR}bin/x86_win32/
CORBA_INCLUDE_DIR=\
	-I${CORBA_DIR}include

CORBA_LIB_DIR=\
	-L${CORBA_DIR}lib/x86_win32/


MSYS_DIR:=${SDK_DIR}MSYS/
MSYS_BIN_DIR:=${MSYS_DIR}bin/
MSYS_INCLUDE_DIR:=${MSYS_DIR}include/

BOOST_DIR:=${SDK_DIR}boost/

SHELL:=${MSYS_BIN_DIR}sh.exe

BUILD_BIN_DIR=${BUILD_DIR}
BUILD_LIB_DIR=${BUILD_DIR}
BUILD_SLIB_DIR=${TMP_SLIB_DIR}

#################################################
# Commands definitions
#################################################
MAKE:=mingw32-make

FIND:=${MSYS_BIN_DIR}find
MKDIR:=${MSYS_BIN_DIR}mkdir -p
RMDIR:=${MSYS_BIN_DIR}rmdir
RMFILE:=${MSYS_BIN_DIR}rm
CP:=${MSYS_BIN_DIR}cp
LEX=${MSYS_BIN_DIR}flex
YACC=${MSYS_BIN_DIR}bison
LS=${MSYS_BIN_DIR}ls

EXE_MODULE_SUFF=.exe
DLL_MODULE_SUFF=.dll

LIB_MODULE_PREF=
LIB_MODULE_SUFF=.lib

ARC_MODULE_PREF=lib
ARC_MODULE_SUFF=.a

#################################################
# C/C++ compiler definitions
#################################################
#CFLAGS+=-shared-libgcc

DEFINES+=\
   -D__WIN32__\
   -D_WIN32_WINNT=0x0500

INCLUDE_PATH+=\
	-I${MSYS_INCLUDE_DIR}
#	-I${SDK_DIR}STLPort/stlport \

#################################################
# Linker definitions
#################################################
LFLAGS+=-Wl,--enable-auto-import

ifeq (${MODULE_TYPE},app_windows)
	LFLAGS+=-Wl,--subsystem=windows
else ifeq (${MODULE_TYPE},app_console)
	LFLAGS+=-Wl,--subsystem=console
else ifeq (${MODULE_TYPE},lib)
   LFLAGS+=-static
endif

#SLIBS += \
#	${SDK_DIR}STLPort/build/lib/obj/gcc/so/libstlport.a


LIBS:=\
	-lstdc++ \
   -lgdi32
   

################################################
# Additional libraries
################################################
CORBA_LIBS:=\
	-lomniORB410_rtd\
	-lomnithread33_rtd
