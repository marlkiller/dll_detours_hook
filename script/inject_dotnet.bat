@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM ============================================================
REM inject_dotnet.bat
REM
REM .NET Harmony hook injection helper (AppDomainManager method)
REM
REM Behavior:
REM - Can be executed from any working directory
REM - Resolves paths relative to inject_dotnet.bat
REM - Copies DotnetHarmonyHook.dll into the target exe directory
REM - Launches target with APPDOMAIN_MANAGER env vars set
REM - Restores the original working directory on exit
REM
REM Usage:
REM   inject_dotnet.bat <target_exe_path>
REM
REM Example:
REM   inject_dotnet.bat "C:\Path\To\Listary.exe"
REM ============================================================

REM ------------------------------------------------------------
REM Argument validation
REM ------------------------------------------------------------
if "%~1"=="" (
echo Usage:
echo     inject_dotnet.bat ^<target_exe_path^>
echo.
echo Example:
echo     inject_dotnet.bat "C:\Path\To\Listary.exe"
exit /b 1
)

REM ------------------------------------------------------------
REM Resolve script directory (location-independent)
REM ------------------------------------------------------------
set SCRIPT_DIR=%~dp0

REM ------------------------------------------------------------
REM DLL path (relative to inject_dotnet.bat)
REM ------------------------------------------------------------
set DLL_SRC=%SCRIPT_DIR%..\release\dotnet_harmony_hook\DotnetHarmonyHook.dll

REM ------------------------------------------------------------
REM Resolve target executable
REM ------------------------------------------------------------
set TARGET_EXE_PATH=%~1
set TARGET_DIR=%~dp1
set TARGET_EXE=%~nx1

REM ------------------------------------------------------------
REM Validation checks
REM ------------------------------------------------------------
if not exist "%TARGET_EXE_PATH%" (
echo [!] Error: Target executable not found:
echo     %TARGET_EXE_PATH%
exit /b 1
)

if not exist "%DLL_SRC%" (
echo [!] Error: DotnetHarmonyHook.dll not found:
echo     %DLL_SRC%
echo [!] Build it first: dotnet_harmony_hook\build.cmd
exit /b 1
)

REM ------------------------------------------------------------
REM Status output
REM ------------------------------------------------------------
echo.
echo [*] .NET Harmony Hook Injection (AppDomainManager)
echo [*] Target directory : %TARGET_DIR%
echo [*] Target executable: %TARGET_EXE%
echo [*] Source DLL       : %DLL_SRC%
echo.

REM ------------------------------------------------------------
REM Enter target directory
REM ------------------------------------------------------------
pushd "%TARGET_DIR%"
if errorlevel 1 (
echo [!] Error: Failed to change to target directory
exit /b 1
)

REM ------------------------------------------------------------
REM Copy DLL into target directory
REM ------------------------------------------------------------
echo [*] Copying DotnetHarmonyHook.dll to target directory...
copy /Y "%DLL_SRC%" ".\DotnetHarmonyHook.dll" >nul
if errorlevel 1 (
echo [!] Error: Failed to copy DLL
popd
exit /b 1
)

REM ------------------------------------------------------------
REM Launch target with AppDomainManager injection
REM ------------------------------------------------------------
echo [*] Launching target with CLR hook injection...
echo.
set "APPDOMAIN_MANAGER_ASM=DotnetHarmonyHook"
set "APPDOMAIN_MANAGER_TYPE=DotnetHarmonyHook.HookAppDomainManager"
start "" "%TARGET_EXE_PATH%"

REM ------------------------------------------------------------
REM Restore original working directory
REM ------------------------------------------------------------
popd

echo.
echo [+] Injection completed. Target launched with hook.
exit /b 0
