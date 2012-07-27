PROJECT_DIR:=l2maxlib/

MODULE_TYPE=dll
MODULE_NAME=l2maxTools

include ${L2MAKE_DIR}config/config.mk

INCLUDE_PATH+=\
	-I${L2PROJECTS_DIR}${PROJECT_DIR}3rdparty/libpng/include \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}3rdparty/zlib/include \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/exception/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/trace/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/io/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/tools/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/thread/src

LIBRARY_PATH+=\
	-L${L2PROJECTS_DIR}${PROJECT_DIR}3rdparty/libpng/lib \
	-L${L2PROJECTS_DIR}${PROJECT_DIR}3rdparty/zlib/lib \
	-L${L2PROJECTS_DIR}${PROJECT_DIR}core/${TMP_SLIB_DIR}

LIBS+=\
	-lpng \
	-lz \
	-ll2maxCore

include $(L2MAKE_DIR)proj.mk