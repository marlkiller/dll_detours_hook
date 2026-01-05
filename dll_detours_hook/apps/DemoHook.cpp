#include "DemoHook.h"
#include "Logger.h"
#include <Windows.h>
#include "detours.h"
#include "HookRegistry.h" // For REGISTER_HOOK macro

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

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)g_origMessageBoxW, MyMessageBoxW);
    LONG error = DetourTransactionCommit();

    if (error == NO_ERROR) {
        LOG_DEBUG("MessageBoxW hook attached successfully.");
    } else {
        LOG_DEBUG("Error attaching MessageBoxW hook: %d", error);
        return false;
    }

    return true;
}

// Register this hook using the macro
REGISTER_HOOK(DemoHook, "c_bin_dev.exe");

