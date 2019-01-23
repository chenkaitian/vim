
TOPDIR	:= ../..

include ${TOPDIR}/make.rules

HFILES = $(wildcard *.h)

DIRS = detail 

all:
	echo "do nothing"

clean:
	echo "do nothing"

cleanall:
	echo "do nothing"

install:
	if [ ! -d ${INSTALL_PATH}/include/promise ]; then mkdir -p ${INSTALL_PATH}/include/promise; fi
	cp -rf ${HFILES} ${INSTALL_PATH}/include/promise
	
	@for dir in $(DIRS); do make -C $$dir install; echo; done

uninstall:
	echo "do nothing"
