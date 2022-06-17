#!/bin/bash

# Uncomment what is necessary to reinstall by force 
rm -f ${CONDA_PREFIX}/lib/libjsonio17.a

BRANCH_JSON=tests

# if no JSONIO installed in ${CONDA_PREFIX}/lib/libjsonio17.a (${CONDA_PREFIX}/include/jsonio17)
test -f ${CONDA_PREFIX}/lib/libjsonio17.a || {

	# Building jsonio library
	mkdir -p ~/code && \
		cd ~/code && \
		git clone https://bitbucket.org/gems4/jsonio17.git -b $BRANCH_JSON && \
		cd jsonio17 && \
		mkdir -p build && \
		cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DBuildExamples=OFF -DBuildTests=OFF && \
		make  && \
		make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

