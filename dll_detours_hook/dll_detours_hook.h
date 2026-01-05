#ifndef DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H
#define DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H

#include <string> // Required for std::string
#include "HookRegistry.h" // Include the new registry header

#ifdef dll_detours_hook_EXPORTS
#define DLL_DETOURS_HOOK_API  __declspec(dllexport)
#else
#define DLL_DETOURS_HOOK_API  __declspec(dllimport)
#endif

// 声明要导出的函数
extern "C" DLL_DETOURS_HOOK_API  int dll_detours_hook_export();
extern "C" DLL_DETOURS_HOOK_API void AttachAllHooks();
extern "C" DLL_DETOURS_HOOK_API void DetachAllHooks();

std::string GetCurrentProcessExeName();

void hello();

#endif //DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H
