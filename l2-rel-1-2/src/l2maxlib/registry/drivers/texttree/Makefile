PROJECT_DIR:=l2maxlib/

MODULE_TYPE=dll
MODULE_NAME=l2maxRegTreeDrv

include $(L2MAKE_DIR)config/config.mk

INCLUDE_PATH+=\
	-I${L2PROJECTS_DIR}${PROJECT_DIR}config \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/exception/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/io/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/trace/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/tools/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/thread/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}registry/engine/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}registry/drivers/common/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}tools/src

OBJFLAGS_START:=-Wl,--whole-archive

OBJECTS+=\
	${ARC_LIB_DIR}${ARC_MODULE_PREF}l2maxRegistryDriversCommon${ARC_MODULE_SUFF}

OBJFLAGS_END:=-Wl,--no-whole-archive

LIBRARY_PATH+=\
	-L${L2PROJECTS_DIR}${PROJECT_DIR}core/${TMP_SLIB_DIR} \
	-L${L2PROJECTS_DIR}${PROJECT_DIR}tools/${TMP_SLIB_DIR}

LIBS+=\
	-ll2maxCore \
	-ll2maxTools

include $(L2MAKE_DIR)proj.mk