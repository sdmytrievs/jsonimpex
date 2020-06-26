#!/bin/bash
# Installing dependencies needed to build jsonimpex14

if [ "$(uname)" == "Darwin" ]; then

    # Do under Mac OS X platform
    #Needs Xcode and ArangoDB server locally installed
    brew upgrade
    brew install cmake
    brew install lua
    
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then

    #Needs gcc v.5 or higher and ArangoDB server locally installed
    sudo apt-get update
        sudo apt-get install -y lua5.3-dev libcurl4-openssl-dev
fi

# Uncomment what is necessary to reinstall by force
#sudo rm -f /usr/local/lib/libvelocypack.*
#sudo rm -f /usr/local/lib/libjsonarango.*
#sudo rm -f /usr/local/lib/libjsonio14.*
#sudo rm -f /usr/local/lib/libpugixml.*
#sudo rm -f /usr/local/lib/libyaml-cpp.*
#sudo rm -f /usr/local/lib/libthrift.a

threads=3


# Velocypack from ArangoDB (added for installing ArangoDB database client)
# if no VPack installed in /usr/local/lib/libvelocypack.a (/usr/local/include/velocypack)
test -f /usr/local/lib/libvelocypack.a || {

        # Building velocypack library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/arangodb/velocypack.git && \
                cd velocypack && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DBuildVelocyPackExamples=OFF && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# JSONIO ArangoDB database client
# if no JSONIO installed in /usr/local/lib/libjsonarango.a
test -f /usr/local/lib/libjsonarango.a || {

        # Building jsonio library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://bitbucket.org/gems4/jsonarango.git -b $BRANCH_JSON && \
                cd jsonio && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# JSONIO14 database client
# if no JSONIO installed in /usr/local/lib/libjsonio14.a (/usr/local/include/jsonio14)
test -f /usr/local/lib/libjsonio14.a || {

        # Building jsonio library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://bitbucket.org/gems4/jsonio14.git -b $BRANCH_JSON && \
                cd jsonio && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_INSTALL_PREFIX=$INSTALL_JSON && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# pugixml
test -f /usr/local/lib/libpugixml.so || {

        # Building pugixml library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/zeux/pugixml.git && \
                cd pugixml && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# YAMLCPP
test -f /usr/local/lib/libyaml-cpp.so || {

        # Building yaml-cpp library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/jbeder/yaml-cpp.git && \
                cd yaml-cpp && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# Thrift
# if no Thrift installed in /usr/local/lib/libthrift.a (/usr/local/include/thrift)
test -f /usr/local/lib/libthrift.a || {

        # Building thrift library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone http://github.com/apache/thrift && \
                cd thrift && \
                ./bootstrap.sh && \
                ./configure --without-lua --without-qt5 && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}


if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
   sudo ldconfig
fi
