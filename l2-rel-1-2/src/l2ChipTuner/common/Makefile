PROJECT_DIR:=l2ChipTuner/

MODULE_TYPE=dll
MODULE_NAME=common

include ${L2MAKE_DIR}config/config.mk

INCLUDE_PATH += \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}ecu/device/src \
   -I${L2PROJECTS_DIR}l2maxlib/registry/engine/src \
   -I${L2PROJECTS_DIR}l2maxlib/tools/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/io/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/exception/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/tools/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/thread/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/trace/src \
   -I${L2PROJECTS_DIR}l2maxlib/core/context/src

LIBRARY_PATH += \
   -L${L2PROJECTS_DIR}l2maxlib/registry/engine/${TMP_SLIB_DIR} \
   -L${L2PROJECTS_DIR}l2maxlib/tools/${TMP_SLIB_DIR} \
   -L${L2PROJECTS_DIR}l2maxlib/core/${TMP_SLIB_DIR}

LIBS+= \
	-ll2maxRegistry \
	-ll2maxTools \
	-ll2maxCore

include $(L2MAKE_DIR)proj.mk