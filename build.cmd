@echo off
rmdir /s /q build

rem MinGW 似乎不支持 32 位
rem cmake -B build -G "MinGW Makefiles"
rem cmake --build build

cmake -G "Visual Studio 17 2022" -A x64 -B build .
@REM cmake -G "Visual Studio 17 2022" -A Win32 -B build .
@REM cmake --build ./build --config Release
cmake --build ./build --config Release --target install
