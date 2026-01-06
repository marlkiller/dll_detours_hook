#include "dll_detours_hook.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm> // For std::transform and std::tolower
#include <filesystem> // For std::filesystem::path
#include "Logger.h"
#include "HookAdapter.h"
#include "HookRegistry.h" // Include the new registry header

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

// New central function to attach all hooks
extern "C" DLL_DETOURS_HOOK_API void AttachAllHooks() {
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