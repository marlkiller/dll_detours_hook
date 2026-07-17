@echo off
setlocal enabledelayedexpansion

set "build_config=Release"

:arg_loop
if "%~1"=="" goto :end_arg_loop
if /i "%~1"=="-c" (set "build_config=%~2" & shift & shift & goto :arg_loop)
shift & goto :arg_loop
:end_arg_loop

echo =================================================
echo  Building DotnetHarmonyHook (%build_config%)...
echo =================================================

where dotnet >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] .NET SDK not found. Please install from https://dotnet.microsoft.com/download
    exit /b 1
)

dotnet restore DotnetHarmonyHook.csproj
if %errorlevel% neq 0 exit /b %errorlevel%

dotnet build DotnetHarmonyHook.csproj -c %build_config% -f net48 -o ..\release\dotnet_harmony_hook\
if %errorlevel% neq 0 exit /b %errorlevel%

echo =================================================
echo  Build successful!
echo  Output: ..\release\dotnet_harmony_hook\
echo =================================================
endlocal
