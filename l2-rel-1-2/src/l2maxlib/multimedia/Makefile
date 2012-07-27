PROJECT_DIR:=l2maxlib/

MODULE_TYPE=dll
MODULE_NAME=l2maxMultimedia

include $(L2MAKE_DIR)config/config.mk

INCLUDE_PATH += \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}tools/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}core/exception/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}core/io/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/trace/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/tools/src \
	-I${SDK_DIR}MPlayer \
   -I${SDK_DIR}MPlayer/libavutil \
	-I${SDK_DIR}xvidcore/src
	
LIBRARY_PATH += \
   -L$${L2PROJECTS_DIR}${PROJECT_DIR}tools/${TMP_SLIB_DIR} \
   -L$${L2PROJECTS_DIR}${PROJECT_DIR}core/${TMP_SLIB_DIR} \
   -L${SDK_DIR}MPlayer \
	-I${SDK_DIR}xvidcore/build/generic/build

LIBS += \
   -ll2maxTools \
   -ll2maxCore \
	-lmplayer \
	-lWs2_32 \
	-lVfw32 \
	-lxvidcore


include $(L2MAKE_DIR)proj.mk