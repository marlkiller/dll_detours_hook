@echo off
setlocal

set "build_type=Release"

rem --- Simple Argument Parsing ---
:arg_loop
if "%~1"=="" goto :end_arg_loop
if /i "%~1"=="-t" (set "build_type=%~2" & shift & shift & goto :arg_loop)
shift & goto :arg_loop
:end_arg_loop

echo =================================================
echo  Auto-detecting Visual Studio Environment...
echo =================================================

rem 1. Use vswhere to find the path to VsDevCmd.bat
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Workload.NativeDesktop -find Common7\Tools\VsDevCmd.bat`) do (
    set "VSDevCmd=%%i"
)

if "%VSDevCmd%"=="" (
    echo [ERROR] Visual Studio C++ workload not found.
    exit /b 1
)

rem 2. Initialize VS environment (This sets up PATH, INCLUDE, LIB, etc.)
echo Loading environment from: %VSDevCmd%
call "%VSDevCmd%" -arch=x64 >nul

rem 3. Clean and Build
if exist "build" rmdir /s /q build

echo Configuring and Building (%build_type%)...

rem Because the environment is loaded, CMake "just works" with the default generator
cmake -B build -DCMAKE_BUILD_TYPE=%build_type% .
if %errorlevel% neq 0 exit /b %errorlevel%

cmake --build build --config %build_type% --target install
if %errorlevel% neq 0 exit /b %errorlevel%

echo =================================================
echo  Build successful!
echo =================================================
endlocal