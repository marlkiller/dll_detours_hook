#pragma once

#include <Windows.h>
#include <vector>

// Forward declaration for DetourAttach/DetourDetach
// These are external symbols from detours.lib
extern "C" {
    LONG WINAPI DetourTransactionBegin();
    LONG WINAPI DetourTransactionCommit();
    LONG WINAPI DetourUpdateThread(_In_ HANDLE hThread);
    LONG WINAPI DetourAttach(_Inout_ PVOID* ppPointer, _In_ PVOID pDetour);
    LONG WINAPI DetourDetach(_Inout_ PVOID* ppPointer, _In_ PVOID pDetour);
}

namespace DetourUtils {

// Helper function to attach a single hook with transaction management
bool Hook(PVOID* ppOriginal, PVOID pDetour);

// Helper function to detach a single hook with transaction management
bool UnHook(PVOID* ppOriginal, PVOID pDetour);

} // namespace DetourUtils
