include (projdefs.pri)

TARGET=l2ChipTuner
TEMPLATE=app

QT += opengl
CONFIG += qt thread

PRE_TARGETDEPS += \
   $${DESTDIR}libClientMeter.a \
   $${DESTDIR}libClientPreferences.a

INCLUDEPATH += \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}common/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/defs/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/device/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/proto/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}client/meter/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}client/preferences/src \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}client/preferences/$${UI_DIR} \
   $${L2PROJECTS_DIR}$${PROJECT_DIR}client/mainapp/src \
   $${L2PROJECTS_DIR}l2maxlib/registry/engine/src \
   $${L2PROJECTS_DIR}l2maxlib/tools/src \
   $${L2PROJECTS_DIR}l2maxlib/core/io/src \
   $${L2PROJECTS_DIR}l2maxlib/core/exception/src \
   $${L2PROJECTS_DIR}l2maxlib/core/trace/src \
   $${L2PROJECTS_DIR}l2maxlib/core/thread/src \
   $${L2PROJECTS_DIR}l2maxlib/core/tools/src \
   $${L2PROJECTS_DIR}l2maxlib/core/context/src

LIBS += \
   $${DESTDIR}libClientMeter.a \
   $${DESTDIR}libClientPreferences.a \
   -L$${L2PROJECTS_DIR}l2maxlib/registry/engine/$${TMP_SLIB_DIR} -ll2maxRegistry \
   -L$${L2PROJECTS_DIR}l2maxlib/tools/$${TMP_SLIB_DIR} -ll2maxTools \
   -L$${L2PROJECTS_DIR}l2maxlib/core/$${TMP_SLIB_DIR} -ll2maxCore \
   -L$${L2PROJECTS_DIR}$${PROJECT_DIR}common/$${TMP_SLIB_DIR} -lcommon \
   -L$${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/defs/$${TMP_SLIB_DIR} -lECUDefs \
   -L$${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/proto/$${TMP_SLIB_DIR} -lECUProto \
   -L$${L2PROJECTS_DIR}$${PROJECT_DIR}ecu/device/$${TMP_SLIB_DIR} -lECUDevice \
   -lopengl32 -lglu32
   
RESOURCES +=\
	res/mainWindow.qrc

win32{
	RC_FILE	= mainapp.rc
}
