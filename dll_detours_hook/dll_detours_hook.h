#ifndef DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H
#define DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H

#ifdef MYDLL_EXPORTS
#define DLL_DETOURS_HOOK_API  __declspec(dllexport)
#else
#define DLL_DETOURS_HOOK_API  __declspec(dllimport)
#endif

// ����Ҫ�����ĺ���
extern "C" DLL_DETOURS_HOOK_API  int dll_detours_hook_export();


void hello();

#endif //DLL_DETOURS_HOOK_DLL_DETOURS_HOOK_H
