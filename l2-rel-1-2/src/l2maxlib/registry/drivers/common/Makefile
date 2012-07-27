PROJECT_DIR:=l2maxlib/

MODULE_TYPE=arc
MODULE_NAME=l2maxRegistryDriversCommon

include ${L2MAKE_DIR}config/config.mk

INCLUDE_PATH+= \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/exception/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/tools/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/trace/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}core/thread/src \
	-I${L2PROJECTS_DIR}${PROJECT_DIR}tools/src \
   -I${L2PROJECTS_DIR}${PROJECT_DIR}registry/engine/src

include $(L2MAKE_DIR)proj.mk
