@echo off
setlocal EnableDelayedExpansion 

set ROOT_DIR=%cd%
set LOCALINSTALL=D:/usr_vs/local
set PROGFILES=%ProgramFiles%
if not "%ProgramFiles(x86)%" == "" set PROGFILES=%ProgramFiles(x86)%

REM Check if Visual Studio 2017 comunity is installed
set MSVCDIR="%PROGFILES%\Microsoft Visual Studio\2017\Community"
set VCVARSALLPATH="%PROGFILES%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"        
if exist %MSVCDIR% (
  if exist %VCVARSALLPATH% (
   	set COMPILER_VER="2017"
        set COMPILER_VER_NAME="Visual Studio 15 2017"
        echo Using Visual Studio 2017 Community
	goto setup_env
  )
)

REM Check if Visual Studio 2019 comunity is installed
set MSVCDIR="%PROGFILES%\Microsoft Visual Studio\2019\Community"
set VCVARSALLPATH="%PROGFILES%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"        
if exist %MSVCDIR% (
  if exist %VCVARSALLPATH% (
   	set COMPILER_VER="2019"
        set COMPILER_VER_NAME="Visual Studio 16 2019"
        echo Using Visual Studio 2019 Community
	goto setup_env
  )
)


echo No compiler : Microsoft Visual Studio 2017 or 2019 Community is not installed.
goto end

:setup_env


echo "%MSVCDIR%\VC\Auxiliary\Build\vcvarsall.bat"
call %MSVCDIR%\VC\Auxiliary\Build\vcvarsall.bat x64

rem Set the standard INCLUDE search path
rem
rem set INCLUDE=%LOCALINSTALL%\include;%INCLUDE%
rem set LIB=%LOCALINSTALL%\lib;%LIB%
rem set PATH=%LOCALINSTALL%\bin;%PATH%
echo %INCLUDE%


mkdir tmp_json
cd tmp_json


echo
echo ******                		 ******
echo ****** Compiling pugixml         ******
echo ******                		 ******
echo

echo Get pugixml from git...
 git clone https://github.com/zeux/pugixml.git
cd pugixml

echo "Configuring..."
cmake -G%COMPILER_VER_NAME%  -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=%LOCALINSTALL% .. -S . -B build
echo "Building..."
cmake --build build  --target install --config Release
cd ..

echo
echo ******                		 ******
echo ****** Compiling yaml-cpp         ******
echo ******                		 ******
echo

echo Get yaml-cpp from git...
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp

echo "Configuring..."
cmake -G %COMPILER_VER_NAME% -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DCMAKE_INSTALL_PREFIX=%LOCALINSTALL% .. -S . -B build
echo "Building..."
cmake --build build  --target install --config Release
cd ..\..

REM Housekeeping
rem rd /s /q tmp_json



