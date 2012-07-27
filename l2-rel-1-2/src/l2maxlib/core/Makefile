PROJECT_DIR:=l2maxlib/

MODULE_TYPE=dll
MODULE_NAME=l2maxCore

SUBDIRS:=\
   context \
   exception \
   thread \
   event \
   trace \
   io \
   tools

include $(L2MAKE_DIR)subdirs.mk

OBJFLAGS_START:=-Wl,--whole-archive

OBJECTS += \
   ${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreContext${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreException${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreThread${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreEvent${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreTrace${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreIo${ARC_MODULE_SUFF} \
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxCoreTools${ARC_MODULE_SUFF}

OBJFLAGS_END:=-Wl,--no-whole-archive

include $(L2MAKE_DIR)proj.mk
