# MAKE proj
# Copyright (c) 2006 Arshinkin Maksim 
# All rights reserved.

# Subprogram for making temporary sources

####################################################################
# LEX_MAKE
####################################################################
LEX_TARGETS:=

define LEX_SRC_MAKE

LEX_TARGETS+=${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc
LEX_OUT+=${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc

${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc: $(1)
	@if [ ! -d ${TMP_LEX_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${TMP_LEX_DIR:%/=%} ; \
	fi

ifeq (${BUILD_LOG_LEVEL},0)
	@echo --- Parsing ---------------------------;
	@echo Module: ${MODULE_NAME};
	@echo   Unit: $(1);
	@echo Config: ${BUILD_CONFIG};
	
	${LEX} ${LXFLAGS} -o${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc $(1)

else
	@echo Parsing: $(notdir $(1));
	@${LEX} ${LXFLAGS} -o${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc $(1)

endif

CXX_SRC+=${TMP_LEX_DIR}$(notdir $(basename $(1))).yy.cc

endef

####################################################################
# YACC_MAKE
####################################################################
define YACC_SRC_MAKE

YACC_OUT+=\
	${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.hh \
	${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.cc \
	location.hh position.hh stack.hh

${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.hh \
${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.cc: ${LEX_TARGETS} $(1)
	@if [ ! -d ${TMP_YACC_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${TMP_YACC_DIR:%/=%} ; \
	fi

ifeq (${BUILD_LOG_LEVEL},0)
	@echo --- Parsing ---------------------------;
	@echo Module: ${MODULE_NAME};
	@echo   Unit: $(1);
	@echo Config: ${BUILD_CONFIG};

	${YACC} ${YFLAGS} -o ${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.cc $(1)

else
	@echo Parsing: $(notdir $(1));
	@${YACC} ${YFLAGS} -o ${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.cc $(1)

endif

HXX_SRC+=${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.hh
CXX_SRC+=${TMP_YACC_DIR}$(notdir $(basename $(1))).tab.cc

endef

####################################################################
# IDL_MAKE
####################################################################
define IDL_SRC_MAKE

IDL_OUT+=${TMP_IDL_DIR}$(notdir $(basename $(1))).hh ${TMP_IDL_DIR}$(notdir $(basename $(1)))SK.cc

${TMP_IDL_DIR}$(notdir $(basename $(1))).hh \
${TMP_IDL_DIR}$(notdir $(basename $(1)))SK.cc: $(1)
	@if [ ! -d ${TMP_IDL_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${TMP_IDL_DIR:%/=%} ; \
	fi

ifeq (${BUILD_LOG_LEVEL},0)
	@echo --- Parsing ---------------------------;
	@echo Module: ${MODULE_NAME};
	@echo   Unit: $(1);
	@echo Config: ${BUILD_CONFIG};
	
	${IDLC} ${IFLAGS} -C${TMP_IDL_DIR:%/=%} $(1)

else
	@echo Parsing: $(notdir $(1));
	@${IDLC} ${IFLAGS} -C${TMP_IDL_DIR:%/=%} $(1)
	
endif

HXX_SRC+=${TMP_IDL_DIR}$(notdir $(basename $(1))).hh
CXX_SRC+=${TMP_IDL_DIR}$(notdir $(basename $(1)))SK.cc

endef

####################################################################
# OBJ_MAKE
####################################################################
define OBJ_MAKE

${TMP_OBJ_DIR}$(notdir $(basename $(1))).o: ${HXX_SRC} $(1)
	@if [ ! -d ${TMP_OBJ_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${TMP_OBJ_DIR:%/=%} ; \
	fi

ifeq (${BUILD_LOG_LEVEL},0)
	@echo --- Compiling -------------------------;
	@echo Module: ${MODULE_NAME};
	@echo   Unit: $(1);
	@echo Config: ${BUILD_CONFIG};

	${CXX} -c ${CFLAGS} ${DEFINES} ${INCLUDE_PATH} $(1) -o ${TMP_OBJ_DIR}$(notdir $(basename $(1))).o

else
	@echo Compiling: $(notdir $(1));
	@${CXX} -c ${CFLAGS} ${DEFINES} ${INCLUDE_PATH} $(1) -o ${TMP_OBJ_DIR}$(notdir $(basename $(1))).o

endif

OBJECTS+=${TMP_OBJ_DIR}$(notdir $(basename $(1))).o

endef

# Find any prmanent souce files and making rules
LEX_SRC=$(wildcard ${SRC_DIR}*.l)
YACC_SRC=$(wildcard ${SRC_DIR}*.y)
IDL_SRC=$(wildcard ${SRC_DIR}*.idl)
HXX_SRC=$(wildcard ${SRC_DIR}*.h*)
CXX_SRC=$(wildcard ${SRC_DIR}*.c*)

ifneq (,${LEX_SRC})
#   DEFINES += -DYYLMAX=0x6400000 -DYY_BUF_SIZE=0x6400000
endif

ifneq (,${YACC_SRC})
	INCLUDE_PATH+=-I${TMP_YACC_DIR}
endif

ifneq (,${IDL_SRC})
	HAS_CORBA=y
endif

ifeq (${HAS_CORBA},y)

INCLUDE_PATH+=\
	${CORBA_INCLUDE_DIR}

LIBRARY_PATH+=\
	${CORBA_LIB_DIR}

LIBS+=\
	${CORBA_LIBS}

DEFINES+=\
	-D__x86__

endif

$(foreach lex_src,$(LEX_SRC),$(eval $(call LEX_SRC_MAKE,$(lex_src))))
$(foreach yacc_src,$(YACC_SRC),$(eval $(call YACC_SRC_MAKE,$(yacc_src))))
$(foreach idl_src,$(IDL_SRC),$(eval $(call IDL_SRC_MAKE,$(idl_src))))
$(foreach cxx_src,$(CXX_SRC),$(eval $(call OBJ_MAKE,$(cxx_src))))

# Detecting what souce is found and what object should be built
build: fail ${MODULE_PATH}

${MODULE_PATH}: ${OBJECTS}
ifeq (${OBJECTS},)
	@echo --- PROJECT HAS NO SOURCES -----------;
else
ifneq ($(findstring --out-implib,${LFLAGS}),)
	@if [ ! -d ${TMP_SLIB_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${TMP_SLIB_DIR:%/=%} ; \
	fi
endif

	@if [ ! -d ${MODULE_DIR:%/=%} ] ; \
	then \
		${MKDIR} ${MODULE_DIR:%/=%} ; \
	fi

ifeq (${MODULE_TYPE},arc)
	@echo --- Archiving ---------------------------;
	@echo Module: ${MODULE_NAME};

ifeq (${BUILD_LOG_LEVEL},0)
	@echo   Type: ${MODULE_TYPE};
	@echo Config: ${BUILD_CONFIG};

	${ARC} ${ARCFLAGS} ${MODULE_PATH} ${OBJECTS}
else
	@${ARC} ${ARCFLAGS} ${MODULE_PATH} ${OBJECTS}
endif

else
	@echo --- Linking ---------------------------;
	@echo Module: ${MODULE_NAME};

ifeq (${BUILD_LOG_LEVEL},0)
	@echo   Unit: ${MODULE_PATH};
	@echo   Type: ${MODULE_TYPE};
	@echo Config: ${BUILD_CONFIG};
	
	${CXX} ${LFLAGS} -o ${MODULE_PATH} ${OBJFLAGS_START} ${OBJECTS} ${OBJFLAGS_END} ${SLIBS} ${LIBRARY_PATH} ${LIBS}

else
	@${CXX} ${LFLAGS} -o ${MODULE_PATH} ${OBJFLAGS_START} ${OBJECTS} ${OBJFLAGS_END} ${SLIBS} ${LIBRARY_PATH} ${LIBS}

endif
endif
	@echo --- Done ------------------------------;
endif

distclean:clean
	@if [ -d  ${MODULE_DIR} ]; \
	then \
		if [ -e ${MODULE_PATH} ]; \
		then \
			echo ${MODULE_PATH}; \
			${RMFILE} ${MODULE_PATH}; \
		fi; \
		if [ -z "`${LS} ${MODULE_DIR}`" ]; \
		then \
			${RMDIR} ${MODULE_DIR}; \
		fi \
	fi

clean:fail
	@echo -- Cleaning ---------------------------;
	@echo Module: ${MODULE_NAME};
	@echo Config: ${BUILD_CONFIG};
	@if [ -d ${TMP_DIR} ] ; \
	then \
		if [ -d ${TMP_LEX_DIR} ] ; \
		then \
			$(foreach lex,${LEX_OUT},${FIND} ${TMP_LEX_DIR} -name '$(notdir $(lex))' -print -exec ${RMFILE} {} \; ; ) \
			if [ -z "`${LS} ${TMP_LEX_DIR}`" ] ; \
			then \
				${RMDIR} ${TMP_LEX_DIR}; \
			fi; \
		fi; \
		if [ -d ${TMP_YACC_DIR} ] ; \
		then \
			$(foreach yacc,${YACC_OUT},${FIND} ${TMP_YACC_DIR} -name '$(notdir $(yacc))' -print -exec ${RMFILE} {} \; ; ) \
			if [ -z "`${LS} ${TMP_YACC_DIR}`" ] ; \
			then \
				${RMDIR} ${TMP_YACC_DIR}; \
			fi; \
		fi; \
		if [ -d ${TMP_IDL_DIR} ] ; \
		then \
			$(foreach idl,${IDL_OUT},${FIND} ${TMP_IDL_DIR} -name '$(notdir $(idl))' -print -exec ${RMFILE} {} \; ; ) \
			if [ -z "`${LS} ${TMP_IDL_DIR}`" ] ; \
			then \
				${RMDIR} ${TMP_IDL_DIR}; \
			fi; \
		fi; \
		if [ -d ${TMP_OBJ_DIR} ] ; \
		then \
			$(foreach obj,$(OBJECTS),${FIND} ${TMP_OBJ_DIR} -name '$(notdir $(obj))' -print -exec ${RMFILE} {} \; ; ) \
			if [ -z "`${LS} ${TMP_OBJ_DIR}`" ] ; \
			then \
				${RMDIR} ${TMP_OBJ_DIR}; \
			fi; \
		fi; \
		if [ -d ${TMP_SLIB_DIR} ] ; \
		then \
			if [ -f ${TMP_SLIB_DIR}${LIB_MODULE_PREF}${MODULE_NAME}${LIB_MODULE_SUFF} ];\
			then\
				${RMFILE} ${TMP_SLIB_DIR}${LIB_MODULE_PREF}${MODULE_NAME}${LIB_MODULE_SUFF};\
			fi;\
			${RMDIR} ${TMP_SLIB_DIR};\
		fi; \
		if [ -z "`${LS} ${TMP_DIR}`" ] ; \
		then \
			${RMDIR} ${TMP_DIR}; \
		fi; \
	fi
