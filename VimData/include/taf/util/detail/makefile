
TOPDIR	:= ../../..

include ${TOPDIR}/make.rules

HFILES = $(wildcard *.h)

all:
	echo "do nothing"

clean:
	echo "do nothing"

cleanall:
	echo "do nothing"

install:
	if [ ! -d ${INSTALL_PATH}/include/util/detail ]; then mkdir -p ${INSTALL_PATH}/include/util/detail; fi
	cp -rf ${HFILES} ${INSTALL_PATH}/include/util/detail

uninstall:
	echo "do nothing"
