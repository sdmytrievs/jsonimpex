#!/bin/bash

# Uncomment what is necessary to reinstall by force 
rm -f ${CONDA_PREFIX}/lib/libjsonio.a

threads=3
BRANCH_JSON=master


# JSONIO database client (added for building ThermoMatch code)
# if no JSONIO installed in ${CONDA_PREFIX}/lib/libjsonio.a (${CONDA_PREFIX}/include/jsonio)
test -f ${CONDA_PREFIX}/lib/libjsonio.a || {

	# Building jsonio library
	mkdir -p ~/code && \
		cd ~/code && \
		git clone https://bitbucket.org/gems4/jsonio.git -b $BRANCH_JSON && \
		cd jsonio && \
		mkdir -p build && \
		cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DBuildJsonioExamples=OFF && \
		make -j $threads && \
		make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

