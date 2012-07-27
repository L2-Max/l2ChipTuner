# MAKE proj
# Copyright (c) 2006 Arshinkin Maksim 
# All rights reserved.

include $(L2MAKE_DIR)config/config.mk

.PHONY:${SUBDIRS}

build distclean clean: fail ${SUBDIRS}
${SUBDIRS}::
	@echo ;
	@echo ------ Subdirectory project $@ ---------;

	@$(L2MAKE_DIR)config/projconfig ${PROJECT_DIR} '${BUILD_CONFIG_LIST}' $@ ${BOOST_DIR} ${SDK_DIR}; \
	if [ $$? -eq 0 ]; \
	then \
		cd $@;\
		qmake -makefile ; \
		${MAKE} ${QT_TARGET} ; \
	else \
		${MAKE} -C $@ -e BUILD_CONFIG=${BUILD_CONFIG} ${MAKECMDGOALS} ; \
	fi
