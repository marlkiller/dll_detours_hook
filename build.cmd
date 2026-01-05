@echo off
setlocal

rem Default build type
set "build_type=Release"

rem --- Argument Parsing ---
:arg_loop
if "%1"=="" goto :end_arg_loop
if /i "%1"=="-t" (
    if not "%2"=="" (
        set "build_type=%2"
        shift
    )
    shift
    goto :arg_loop
)
shift
goto :arg_loop
:end_arg_loop
rem --- End Argument Parsing ---

echo =================================================
echo  Starting build with configuration: %build_type%
echo =================================================

rem Clean previous build
if exist "build" (
    echo "Removing previous build directory..."
    rmdir /s /q build
)

rem Configure
echo "Configuring with CMake..."
cmake -G "Visual Studio 17 2022" -A x64 -B build .
if %errorlevel% neq 0 (
    echo "CMake configuration failed."
    goto :eof
)


rem Build
echo "Building with CMake..."
cmake --build ./build --config %build_type% --target install
if %errorlevel% neq 0 (
    echo "CMake build failed."
    goto :eof
)

echo "Build completed successfully."

endlocal
