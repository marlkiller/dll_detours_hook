#include "DemoHook.h"
#include "Logger.h"
#include <Windows.h>
// #include "detours.h" // No longer directly needed here for transactions
#include "HookRegistry.h" // For REGISTER_HOOK macro
#include "utils/DetourUtils.h" // Include our new DetourUtils

// Original function pointer for MessageBoxW
static int (WINAPI * g_origMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;

// Our replacement function for MessageBoxW
int WINAPI MyMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    LOG_DEBUG("MessageBoxW hooked!");
    LOG_DEBUG("Original Caption: %ls", lpCaption);
    LOG_DEBUG("Original Text: %ls", lpText);

    // Modify the caption and text
    LPCWSTR newCaption = L"Hooked by DemoHook!";
    LPCWSTR newText = L"This message box has been hijacked.";

    // Call the original MessageBoxW with modified parameters
    return g_origMessageBoxW(hWnd, newText, newCaption, uType);
}

const char* DemoHook::GetProcessName() const
{
    return "c_bin_dev.exe";
}

bool DemoHook::InstallHook()
{
    LOG_DEBUG("Installing hooks for %s", GetProcessName());
    return DetourUtils::Hook(&(PVOID&)g_origMessageBoxW, MyMessageBoxW);
}

void DemoHook::UninstallHook()
{
    LOG_DEBUG("Uninstalling hooks for %s", GetProcessName());
    DetourUtils::UnHook(&(PVOID&)g_origMessageBoxW, MyMessageBoxW);
    g_origMessageBoxW = MessageBoxW; // Reset to original for safety/clarity
}

// Register this hook using the macro
REGISTER_HOOK(DemoHook, "c_bin_dev.exe");

