#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
    EXTN=dylib
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    EXTN=so
fi

# Uncomment what is necessary to reinstall by force 
# rm -f ${CONDA_PREFIX}/lib/libjsonarango.$EXTN
# rm -f ${CONDA_PREFIX}/lib/libjsonio17.a

BRANCH_JSON=master

# jsonArango database client
# if no jsonArango installed in ${CONDA_PREFIX}/lib/libjsonarango.a
test -f ${CONDA_PREFIX}/lib/libjsonarango.$EXTN || {

        # Building jsonio library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone  --recurse-submodules https://bitbucket.org/gems4/jsonarango.git && \
                cd jsonarango && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX} -DBUILD_EXAMPLES=OFF  -DBULID_LOCAL_TESTS=OFF -DBULID_REMOTE_TESTS=OFF && \
                make && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# if no JSONIO installed in ${CONDA_PREFIX}/lib/libjsonio17.a (${CONDA_PREFIX}/include/jsonio17)
test -f ${CONDA_PREFIX}/lib/libjsonio17.a || {

	# Building jsonio library
	mkdir -p ~/code && \
		cd ~/code && \
		git clone https://bitbucket.org/gems4/jsonio17.git -b $BRANCH_JSON && \
		cd jsonio17 && \
		mkdir -p build && \
		cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX} -DBuildExamples=OFF -DBuildTests=OFF && \
		make  && \
		make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

