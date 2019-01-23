
TOPDIR	:= ../..

include ${TOPDIR}/make.rules

HFILES = $(wildcard *.h)

all:
	echo "do nothing"

clean:
	echo "do nothing"

cleanall:
	echo "do nothing"

install:
	if [ ! -d ${INSTALL_PATH}/include/property ]; then mkdir -p ${INSTALL_PATH}/include/property; fi
	cp -rf ${HFILES} ${INSTALL_PATH}/include/property

uninstall:
	echo "do nothing"
