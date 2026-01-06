#include "dll_detours_hook.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm> // For std::transform and std::tolower
#include <filesystem> // For std::filesystem::path
#include "Logger.h"
#include "HookAdapter.h"
#include "HookRegistry.h" // Include the new registry header
#include <tlhelp32.h>


// Global vector to hold all active hook adapters
std::vector<HookAdapter*> g_hooks;

// Exported function implementation
extern "C" DLL_DETOURS_HOOK_API int dll_detours_hook_export() {
    return 521;
}

std::string GetCurrentProcessExeName() {
    WCHAR wProcessPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, wProcessPath, MAX_PATH) == 0) {
        LOG_DEBUG("Failed to get current process path.");
        return "";
    }

    std::filesystem::path fullPath(wProcessPath);
    std::string processName = fullPath.filename().string();
    
    // Convert to lowercase for consistent comparison
    std::transform(processName.begin(), processName.end(), processName.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    return processName;
}

// Function to get the parent process ID
DWORD GetParentProcessId() {
    DWORD ppid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            DWORD pid = GetCurrentProcessId();
            do {
                if (pe32.th32ProcessID == pid) {
                    ppid = pe32.th32ParentProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return ppid;
}

// Function to log detailed process information
void LogProcessInfo() {
    LOG_DEBUG("---------------- Process Information ----------------");

    // 1. Process ID and Parent Process ID
    DWORD pid = GetCurrentProcessId();
    DWORD ppid = GetParentProcessId();
    LOG_DEBUG("  Process ID (PID): %lu", pid);
    LOG_DEBUG("  Parent PID (PPID): %lu", ppid);

    // 2. Process Executable Path
    WCHAR processPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, processPath, MAX_PATH) == 0) {
        LOG_DEBUG("  Executable Path: Failed to get path.");
    } else {
        LOG_DEBUG_W(L"  Executable Path: %ls", processPath);
    }

    // 3. Command Line
    LPWSTR cmdLine = GetCommandLineW();
    if (cmdLine == NULL) {
        LOG_DEBUG("  Command Line: Failed to get command line.");
    } else {
        LOG_DEBUG_W(L"  Command Line: %ls", cmdLine);
    }

    // 4. Process Architecture
    BOOL isWow64 = FALSE;
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);

    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        LOG_DEBUG("  Process Architecture: x86 (32-bit)");
    } else if (IsWow64Process(GetCurrentProcess(), &isWow64) && isWow64) {
        LOG_DEBUG("  Process Architecture: x86 (32-bit) running on x64 OS");
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        LOG_DEBUG("  Process Architecture: x64 (64-bit)");
    } else {
        LOG_DEBUG("  Process Architecture: Unknown");
    }


    // 5. List of Loaded Modules
    LOG_DEBUG("  Loaded Modules:");
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W me32;
        me32.dwSize = sizeof(MODULEENTRY32W);
        if (Module32FirstW(hSnapshot, &me32)) {
            do {
                LOG_DEBUG_W(L"    - %ls (Base: 0x%p, Size: %lu bytes)", me32.szExePath, me32.modBaseAddr, me32.modBaseSize);
            } while (Module32NextW(hSnapshot, &me32));
        }
        CloseHandle(hSnapshot);
    } else {
        LOG_DEBUG("    Failed to create module snapshot.");
    }
    LOG_DEBUG("----------------------------------------------------");
}

// New central function to attach all hooks
extern "C" DLL_DETOURS_HOOK_API void AttachAllHooks() {
	// LogProcessInfo();
    std::string processName = GetCurrentProcessExeName();

    if (processName.empty()) {
        LOG_DEBUG("Could not determine current process executable name.");
        return;
    }

    LOG_DEBUG("DLL attached to process: %s", processName.c_str());

    HookAdapter* matchedHook = nullptr;

    for (const auto& entry : g_hookCreators) {
        HookCreatorFn creator = entry.second;
        // Instantiate the hook to check its process name.
        // The HookAdapter::GetProcessName() method is const.
        // We create a temporary instance just to call GetProcessName().
        // If it matches, we recreate it to install.
        std::unique_ptr<HookAdapter> tempHook(creator());
        if (tempHook && tempHook->ShouldHook(processName)) {
            // Found a match, now create the actual hook instance for installation
            matchedHook = creator();
            if (!matchedHook) {
                LOG_DEBUG("Failed to create hook instance for process: %s", entry.first.c_str());
                continue; // Try next hook creator
            }
            matchedHook->CheckFirstLaunch();
            LOG_DEBUG("Found matching hook for process: %s, type: %s", processName.c_str(), matchedHook->GetProcessName());
            if (!matchedHook->InstallHook()) {
                LOG_DEBUG("Failed to install hook for process: %s", matchedHook->GetProcessName());
                delete matchedHook; // Cleanup if install fails
                matchedHook = nullptr; // Mark as failed
            } else {
                g_hooks.push_back(matchedHook); // Only add to global list if successfully installed
            }
            break; // Found and handled the matching hook, stop iterating
        }
    }

    if (!matchedHook) {
        LOG_DEBUG("No suitable hook found for process: %s", processName.c_str());
    }
}

// New central function to detach all hooks and clean up
extern "C" DLL_DETOURS_HOOK_API void DetachAllHooks() {
    LOG_DEBUG("Cleaning up hook objects.");
    for (HookAdapter* hook : g_hooks) {
        hook->UninstallHook(); // Call UninstallHook() before deleting
        delete hook;
    }
    g_hooks.clear();
}


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications
            DisableThreadLibraryCalls((HMODULE)hModule);
            // Install hooks
            AttachAllHooks(); // Call the new function
            break;
        case DLL_PROCESS_DETACH:
            // Uninstall hooks
            DetachAllHooks(); // Call the new function
            break;
    }
    return TRUE;
}