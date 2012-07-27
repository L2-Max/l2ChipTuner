# MAKE proj
# Copyright (c) 2006 Arshinkin Maksim
# All rights reserved.

platform:=win32
#platform:=unix
#platform:=aix

CPU_TYPE:=x86
#CPU_TYPE:=POWERPC

BUILD_CONFIG_DEBUG=d
BUILD_CONFIG_RELEASE=r

BUILD_CONFIG_LIST=\
	${BUILD_CONFIG_DEBUG}\
	${BUILD_CONFIG_RELEASE}

BUILD_CONFIG?=${BUILD_CONFIG_DEBUG}

BUILD_LOG_LEVEL:=1

ERROR_CODE:=0

ifeq (${platform},)
   ERROR_MSG:=Platform not specified.
   ERROR_CODE=1
endif

ifeq ($(MAKECMDGOALS),)
	ERROR_MSG:=Target not specified.
	ERROR_CODE=1
endif

ifeq (${BUILD_CONFIG},)
	ERROR_MSG:=BUILD_CONFIG not specified.
	ERROR_CODE=1
endif

ifeq ($(filter ${BUILD_CONFIG},${BUILD_CONFIG_DEBUG} ${BUILD_CONFIG_RELEASE}),)
	ERROR_MSG:=Build config '${BUILD_CONFIG}' not exist.
	ERROR_CODE=1
endif

fail::
ifneq (${ERROR_CODE},0)
	@echo --- Error ---------------------------------------;
	@echo ${ERROR_MSG};
	@echo -------------------------------------------------;
	@exit 1
endif

.PHONY:build clean distclean

include $(L2MAKE_DIR)config/defs.mk

ifneq (${platform},)
	include $(L2MAKE_DIR)platforms/${platform}.mk
endif

##############################################
# Common definitions
##############################################
HAS_CORBA=n

MAKEFILE=Makefile

ifeq (${MODULE_TYPE},dll)
	MODULE_DIR:=${BUILD_LIB_DIR}
	MODULE_PATH:=${MODULE_DIR}${MODULE_NAME}${DLL_MODULE_SUFF}
else
ifeq (${MODULE_TYPE},lib)
	MODULE_DIR:=${BUILD_BIN_DIR}
	MODULE_PATH:=${MODULE_DIR}${MODULE_NAME}${LIB_MODULE_SUFF}
else
ifeq (${MODULE_TYPE},arc)
	MODULE_DIR:=${ARC_LIB_DIR}
	MODULE_PATH:=${MODULE_DIR}${ARC_MODULE_PREF}${MODULE_NAME}${ARC_MODULE_SUFF}
else
	MODULE_DIR:=${BUILD_BIN_DIR}
	MODULE_PATH:=${MODULE_DIR}${MODULE_NAME}${EXE_MODULE_SUFF}
endif
endif
endif
ifeq (${MAKECMDGOALS},build)
	ifeq (${BUILD_CONFIG},d)
		QT_TARGET:=debug
	else
		QT_TARGET:=release
	endif
else
	QT_TARGET:=${MAKECMDGOALS}
endif

##############################################
# C/C++ compiler definitions
##############################################
ifeq ( ${CXX}, )
   CXX=gcc
endif

CFLAGS+=-fexceptions

ifeq (${CPU_TYPE},x86)
   CFLAGS+=-mthreads
endif

ifeq (${CPU_TYPE},POWERPC)
#   CFLAGS+=-pthread
endif

ifeq (${BUILD_CONFIG},d)
	CFLAGS+=-g
endif

ifeq (${BUILD_CONFIG},r)
	CFLAGS+=-O3
endif

INCLUDE_PATH+=\
	-I${TMP_IDL_DIR}

##############################################
# Linker definitions
##############################################
ifeq (${CPU_TYPE},x86)
   LFLAGS+=-mthreads
endif

ifeq (${BUILD_CONFIG},r)
	LFLAGS+=-s
endif

ifeq (${MODULE_TYPE},dll)
	LFLAGS+=-shared -Wl,--out-implib,${TMP_SLIB_DIR}${LIB_MODULE_PREF}${MODULE_NAME}${LIB_MODULE_SUFF}
endif



##############################################
# Archiver definitions
##############################################
ARC:=ar
ARCFLAGS:=r

##############################################
# LEX parser definitions
##############################################
LXFLAGS=-d

##############################################
# YACC parser definitions
##############################################
YFLAGS=-d

##############################################
# IDL parser definitions
##############################################
IDLC=omniidl
IFLAGS=-bcxx
