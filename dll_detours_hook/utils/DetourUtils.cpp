#include "DetourUtils.h"
#include "Logger.h" // Assuming LOG_DEBUG is defined here
#include <detours.h> // Include the actual detours header for library functions

namespace DetourUtils {

bool Hook(PVOID* ppOriginal, PVOID pDetour) {
    if (DetourTransactionBegin() != NO_ERROR) {
        LOG_DEBUG("DetourTransactionBegin failed for attach.");
        return false;
    }

    DetourUpdateThread(GetCurrentThread());

    if (DetourAttach(ppOriginal, pDetour) != NO_ERROR) {
        DetourTransactionAbort();
        LOG_DEBUG("DetourAttach failed for hook %p -> %p.", ppOriginal, pDetour);
        return false;
    }

    LONG error = DetourTransactionCommit();
    if (error == NO_ERROR) {
        LOG_DEBUG("Hook %p -> %p attached successfully.", ppOriginal, pDetour);
        return true;
    } else {
        LOG_DEBUG("DetourTransactionCommit failed for attach: %d.", error);
        return false;
    }
}

bool UnHook(PVOID* ppOriginal, PVOID pDetour) {
    if (DetourTransactionBegin() != NO_ERROR) {
        LOG_DEBUG("DetourTransactionBegin failed for detach.");
        return false;
    }

    DetourUpdateThread(GetCurrentThread());

    if (DetourDetach(ppOriginal, pDetour) != NO_ERROR) {
        DetourTransactionAbort();
        LOG_DEBUG("DetourDetach failed for hook %p -> %p.", ppOriginal, pDetour);
        return false;
    }

    LONG error = DetourTransactionCommit();
    if (error == NO_ERROR) {
        LOG_DEBUG("Hook %p -> %p detached successfully.", ppOriginal, pDetour);
        return true;
    } else {
        LOG_DEBUG("DetourTransactionCommit failed for detach: %d.", error);
        return false;
    }
}

} // namespace DetourUtils