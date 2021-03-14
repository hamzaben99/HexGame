SRC=./src
TST=./test
BUILD=./build
INSTALL=./install
CYOUNIT_SRC=./lib/cyounit-src
CYOUNIT_LIB=./lib/cyounit
DOC=./build/doc
ifdef GSL_PATH
INC_LIB_PATH=-DGSL_PATH=
LIB_PATH=${GSL_PATH}
endif

all: server test random_player connex_player

cmake-build:
	mkdir -p ${BUILD}
	cd ${BUILD} && cmake ${INC_LIB_PATH}${LIB_PATH} ..

server: cmake-build
	cd ${BUILD} && make server

test: cyounit cmake-build
	cd ${BUILD} && make alltests_project alltests_random_player alltests_connex_player 

random_player: cmake-build
	cd ${BUILD} && make random_player

connex_player: cmake-build
	cd ${BUILD} && make connex_player0
	cd ${BUILD} && make connex_player1
	cd ${BUILD} && make connex_player2

minimax_player: cmake-build
	cd ${BUILD} && make minimax_player

install: server test random_player connex_player #minimax_player
	cp ${BUILD}/${SRC}/server ${INSTALL}/
	cp ${BUILD}/${TST}/alltests* ${INSTALL}/
	cp ${BUILD}/${SRC}/*.so ${INSTALL}/

clean: cyounit-clean
	rm -rf ${INSTALL}/* ${BUILD}/ ${CYOUNIT_LIB}/ *.gcno *.gcda *.gcov

cyounit:
	cd ${CYOUNIT_SRC} && mkdir -p build && cd build && \
	cmake -DCMAKE_INSTALL_PREFIX="${CURDIR}/${CYOUNIT_LIB}" .. && make install

cyounit-clean:
	rm -rf ${CYOUNIT_SRC}/build

cyounit-update:
	git subtree pull --prefix ${CYOUNIT_SRC} cyounit-src master --squash

doc: clean install
	mkdir ${DOC} && cd ${DOC} && doxygen ../../doc/Doxyfile
