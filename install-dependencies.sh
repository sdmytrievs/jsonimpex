#!/bin/bash
# Installing dependencies needed to build jsonimpex

if [ "$(uname)" == "Darwin" ]; then
    # Do under Mac OS X platform
    #Needs Xcode and ArangoDB server locally installed
    #brew upgrade
    brew install lua
    EXTN=dylib

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    #Needs gcc v.5 or higher and ArangoDB server locally installed
    #sudo apt-get update
    sudo apt-get install -y libcurl4-openssl-dev
    EXTN=so

    #sudo rm -f /usr/local/lib/liblua.a
    # Lua is a powerful, efficient, lightweight, embeddable scripting language
    # if no lua installed in /usr/local/lib/liblua.a (/usr/local/include)
    test -f /usr/local/lib/liblua.a || {

        # Building velocypack library
        mkdir -p ~/code && \
                cd ~/code && \
                curl -R -O http://www.lua.org/ftp/lua-5.4.2.tar.gz && \
                tar zxf lua-5.4.2.tar.gz && \
                cd lua-5.4.2 && \
                make "MYCFLAGS=-fPIC" linux && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
   }

fi

# Uncomment what is necessary to reinstall by force
#sudo rm -rf /usr/local/include/spdlog
#sudo rm -f /usr/local/lib/libvelocypack.a
#sudo rm -f /usr/local/lib/libarango-cpp.$EXTN
#sudo rm -f /usr/local/lib/libjsonio.$EXTN
#sudo rm -f /usr/local/lib/libpugixml.$EXTN
#sudo rm -f /usr/local/lib/libyaml-cpp.$EXTN
#sudo rm -f /usr/local/lib/libthrift.$EXTN

threads=3
BRANCH_JSON=master
BuildType=Release

# spdlog
# if no spdlog installed in /usr/local/include/spdlog (copy only headers)
test -d /usr/local/include/spdlog || {

        # Building spdlog library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/gabime/spdlog -b v1.11.0  && \
                cd spdlog/include && \
                sudo cp -r spdlog /usr/local/include

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# Velocypack from arango-cpp (added for installing arango-cpp database client)
# Installed as part arango-cpp
# arango-cpp database client
# if no arango-cpp installed in /usr/local/lib/libarango-cpp.a
test -f /usr/local/lib/libarango-cpp.$EXTN || {

        # Building arango-cpp library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone  --recurse-submodules https://github.com/sdmytrievs/arango-cpp.git && \
                cd arango-cpp && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release && \
                make -j $threads && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# JSONIO database client
# if no JSONIO installed in /usr/local/lib/libjsonio.$EXTN (/usr/local/include/jsonio)
test -f /usr/local/lib/libjsonio.$EXTN || {

        # Building jsonio library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/sdmytrievs/jsonio.git -b $BRANCH_JSON && \
                cd jsonio && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=$BuildType && \
                make && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# pugixml
test -f /usr/local/lib/libpugixml.$EXTN || {

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
test -f /usr/local/lib/libyaml-cpp.$EXTN || {

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

## Thrift
## if no Thrift installed in /usr/local/lib/libthrift.a (/usr/local/include/thrift)
#test -f /usr/local/lib/libthrift.$EXTN || {
#
#        # Building thrift library
#        mkdir -p ~/code && \
#                cd ~/code && \
#                git clone http://github.com/apache/thrift && \
#                cd thrift && \
#                ./bootstrap.sh && \
#                ./configure  CXXFLAGS='-fPIC' --without-lua --without-qt5 && \
#                make -j $threads && \
#                sudo make install
#
#        # Removing generated build files
#        cd ~ && \
#                 rm -rf ~/code
#}


if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
   sudo ldconfig
fi
