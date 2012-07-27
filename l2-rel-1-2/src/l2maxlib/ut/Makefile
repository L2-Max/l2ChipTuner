PROJECT_DIR:=l2maxlib/

MODULE_TYPE=app
MODULE_NAME=ut

include $(L2MAKE_DIR)config/config.mk

CFLAGS +=

INCLUDE_PATH+=\
	-I${L2PROJECTS_DIR}${PROJECT_DIR}tools/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}registry/engine/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}multimedia/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}core/exception/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}core/io/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/trace/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/thread/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/tools/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/context/src

LIBRARY_PATH+=\
	-L${L2PROJECTS_DIR}${PROJECT_DIR}tools/${TMP_SLIB_DIR} \
	-L${L2PROJECTS_DIR}${PROJECT_DIR}registry/engine/${TMP_SLIB_DIR} \
   -L${L2PROJECTS_DIR}${PROJECT_DIR}multimedia/${TMP_SLIB_DIR} \
   -L${L2PROJECTS_DIR}${PROJECT_DIR}core/${TMP_SLIB_DIR}


LIBS+=\
	-ll2maxTools \
	-ll2maxRegistry \
   -ll2maxMultimedia \
   -ll2maxCore

include $(L2MAKE_DIR)proj.mk
