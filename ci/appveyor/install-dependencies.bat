if "%APPVEYOR_BUILD_WORKER_IMAGE%"=="Visual Studio 2017" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
)
if "%APPVEYOR_BUILD_WORKER_IMAGE%"=="Visual Studio 2015" (
    call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
)

set FART="%CD%\tools\fart\fart.exe"

mkdir tmp_velo
cd tmp_velo

echo
echo ******                  ******
echo ****** Compiling JSONIO ******
echo ******                  ******
echo

echo git clone jsonio17...
git clone https://bitbucket.org/gems4/jsonio17.git
cd jsonio17

echo "Configuring..."
cmake -G"Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH="%CONDA_PREFIX%\Library" .. -A x64 -S . -B build
echo "Building..."
cmake --build build --config %CONFIGURATION% --target install

cd ..\..

REM Housekeeping
rd /s /q tmp_velo
