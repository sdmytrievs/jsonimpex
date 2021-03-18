# jsonio17


## Build and Run jsonio17 on Windows 10

* Make sure you have git installed. If not, install it on Windows: https://git-scm.com/download/win.
* To download jsonio17 source code, using Windows Command Prompt go to C:/gitJsonio and execute

```sh
git clone https://bitbucket.org/gems4/jsonio17.git
```

## Prepare building tools mingw


## Prepare building tools MSVC

* jsonio17 dependencies will be compiled using MSVC 2017 64 bit compiler. For this Visual Studio Community (2017) needs to be installed: 
https://docs.microsoft.com/en-us/visualstudio/install/install-visual-studio?view=vs-2017
At Step 4 - Select workloads, select Desktop development with C++ to be installed. On the individual components page check that also Windows 10 SDK is selected to be installed.

* In addition to MSVC 2017, Qt needs to be installed: https://www.qt.io/download in C:/Qt folder (Qt installation folder is used in further scripts, please use C:/Qt)!
Select with Qt 5.12.0 MSVC 2017 64-bit with Qt Charts, and Qt WebEngine.

### Install Dependencies

* For compiling the libraries that jsonio17 is dependent on, .bat script can be found in /win-make. The process will several minutes. In a windows Command Prompt terminal go to ~jsonarango/win-make and run:

```
jsonio17/win-make>install-dependencies-Win-msvc.bat
```

This script builds curl and velocypack libraries, copies then in the C:\usr\local folder.

### Compiling and the jsonio17 test in Qt Creator

* In Qt Creator open ~\jsonio17\jsonio17_test.pro

### lua54

http://luabinaries.sourceforge.net/

https://sourceforge.net/projects/luabinaries/files/5.4.2/

https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%20Libraries/Static/

https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%20Libraries/Static/lua-5.4.2_Win64_vc16_lib.zip/download


## thrift



 http://archive.apache.org/dist/thrift/0.10.0/
 
http://www.apache.org/dyn/closer.cgi?path=/thrift/0.14.1/thrift-0.14.1.exe

https://thrift.apache.org/tutorial/

http://mohamednadjibmami.com/apache-thrift-for-c-on-visual-studio-2015/

C++11 is required at a minimum. C++03/C++98 are not supported after version 0.12.0.

Boost is required to run the C++ unit tests. It is not necessary to link against the runtime library.

libevent (for libthriftnb only) - most linux distributions have dev packages for this: http://monkey.org/~provos/libevent/

https://www.programmersought.com/article/63691428329/

Thrift installation under Windows
I am compiling on Windows 7 64bit, VS2010. Compiling under Windows is not too much trouble, as briefly introduced as follows:

Unzip the source code, go to the lib\cpp directory, open Thrift.sln, there are two projects libthrift and libthriftnb, which libthrift project is a regular blocking server (single-threaded server, one connected to a thread server, thread pool Server), libthriftnb project is non-blocking (non-blocking) mode server side, only need to rely on libevent library when compiling libthriftnb, otherwise you can not compile libevent library;
Set up dependent library header files and library files, which will not be introduced anymore;
Compile, if it is successful, it will be OK, libthrift.lib and libthriftnb.lib will be generated in the lib\cpp\Debug directory (if compiled);
Description: The release of thrift-0.9.3 is actually compiled under Windows. Because the Thrift.cpp to be compiled in the vs project does not exist anymore, it can be compiled successfully after removing it from the project.thrift-pull-739。

In addition, you can also compile the thrift file generation tool yourself, of course, you can also directlyOfficial website downloadHere are the compilation steps:

Copy the compiler\cpp\src\windows\version.h.in file to the compiler\cpp\src\ directory and rename it to version.h;
Go to the compiler\cpp directory, open compiler.sln, compile

https://docs.microsoft.com/en-us/visualstudio/msbuild/how-to-build-specific-targets-in-solutions-by-using-msbuild-exe?view=vs-2019

msbuild "libthrift.vcxproj" /fl /flp:logfile=MyProjectOutput.log;verbosity=diagnostic


https://cognitivewaves.wordpress.com/apache-thrift-on-windows/#source-cmake




D:\thrift> set BOOST_ROOT=D:\thrift\3rdparty\boost_1_53_0\_install
D:\thrift> mkdir _build
D:\thrift> cd _build
D:\thrift\_build> cmake .. -G "Visual Studio 10 Win64" -DCMAKE_INSTALL_PREFIX=D:/thrift/_install -DCMAKE_PREFIX_PATH=D:/thrift/3rdparty/flex_bison -DWITH_SHARED_LIB=off -DWITH_CPP=on -DWITH_BOOSTTHREADS=on -DWITH_PLUGIN=off -DBUILD_TESTING=off -DBUILD_TUTORIALS=on 
