# jsonimpex17


## Build and Run jsonimpex17 on Windows 10

* Make sure you have git installed. If not, install it on Windows: https://git-scm.com/download/win.
* To download jsonio17 source code, using Windows Command Prompt go to C:/gitJsonio and execute

```sh
git clone https://bitbucket.org/gems4/jsonimpex17.git
```

## Prepare building tools mingw


## Prepare building tools MSVC

* jsonimpex17 dependencies will be compiled using MSVC 2017 64 bit compiler. For this Visual Studio Community (2017) needs to be installed: 
https://docs.microsoft.com/en-us/visualstudio/install/install-visual-studio?view=vs-2017
At Step 4 - Select workloads, select Desktop development with C++ to be installed. On the individual components page check that also Windows 10 SDK is selected to be installed.

* In addition to MSVC 2017, Qt needs to be installed: https://www.qt.io/download in C:/Qt folder (Qt installation folder is used in further scripts, please use C:/Qt)!
Select with Qt 5.12.0 MSVC 2017 64-bit with Qt Charts, and Qt WebEngine.

### Install Dependencies

* For compiling the libraries that jsonimpex17 is dependent on, .bat script can be found in /win-make. The process will several minutes. In a windows Command Prompt terminal go to ~jsonimpex17/win-make and run:

```
jsonimpex17/win-make>install-dependencies-Win-msvc.bat
```

This script builds curl, velocypack, lua, jsonio17, yaml-cpp and pugixml libraries, copies then in the C:\usr\local folder.

### Compiling and the jsonimpex17 test in Qt Creator

* In Qt Creator open ~\jsonimpex17\jsonimpex17_test.pro

### lua54

http://luabinaries.sourceforge.net/

https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%20Libraries/Static/lua-5.4.2_Win64_vc16_lib.zip/download


