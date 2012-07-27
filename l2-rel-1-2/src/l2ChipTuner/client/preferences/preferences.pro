include (projdefs.pri)

TARGET=ClientPreferences
TEMPLATE=lib

CONFIG += static qt thread

INCLUDEPATH += \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}common/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/defs/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/device/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/proto/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}client/mainapp/src \
   $${L2PROJECTS_DIR}l2maxlib/registry/engine/src \
   $${L2PROJECTS_DIR}l2maxlib/tools/src \
   $${L2PROJECTS_DIR}l2maxlib/core/io/src \
   $${L2PROJECTS_DIR}l2maxlib/core/exception/src \
   $${L2PROJECTS_DIR}l2maxlib/core/trace/src \
   $${L2PROJECTS_DIR}l2maxlib/core/thread/src \
   $${L2PROJECTS_DIR}l2maxlib/core/tools/src \
   $${L2PROJECTS_DIR}l2maxlib/core/context/src
