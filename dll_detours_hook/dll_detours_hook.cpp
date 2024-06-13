#include "dll_detours_hook.h"
#include "stdlib.h"
#include <iostream>
#include <Windows.h>
#include "detours.h"
#include "detver.h"
//#pragma comment(lib,"detours.lib")
//#pragma comment (lib,"libs\detours.lib")
//#pragma comment (lib,"libs/detours.lib")


// 实现导出的函数
int dll_detours_hook_export() {
    return 521;
}

// Original function
int sum(int a, int b) {
    return a + b;
}

// Function pointer to hold the address of the original function
// This allows calling the original function even after it has been hooked
static int (*original_sum)(int a, int b) = sum;

// Replacement function
int mySum(int a, int b) {
    // If needed, you can call the original function via original_sum
    // Uncomment the line below to see the original result
    // std::cout << "Result before replacement: " << original_sum(a, b) << std::endl;
    return a * b;
}
//
//

//
////解除拦截函数
//void EndHook() {
//    //开始事务
//    DetourTransactionBegin();
//    //更新线程信息 
//    DetourUpdateThread(GetCurrentThread());
//    //将拦截函数从原函数的地址上解除
//    DetourDetach(&(PVOID&)original_sum, (PVOID)mySum);
//    //结束事务
//    DetourTransactionCommit();
//}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    std::cout << "Hello, dll_detours_hook!" << std::endl;

    printf("before %d\n", sum(1,2));
    // DobbyHook(sum, mySum, (void *)&sum_p);

    //开始事务
    DetourTransactionBegin();
    //更新线程信息
    DetourUpdateThread(GetCurrentThread());
    //将拦截函数附加到原函数的地址上
    DetourAttach(&(PVOID&)original_sum, (PVOID)mySum);
    //结束事务
    DetourTransactionCommit();
    
    
    printf("after %d\n", sum(1,2));
    
    
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            // 进程被加载后执行
            break;
        case DLL_THREAD_ATTACH:
            // 线程被创建后加载
            break;
        case DLL_THREAD_DETACH:
            // 正常退出执行的代码
            break;
        case DLL_PROCESS_DETACH:
            // 进程卸载本Dll后执行的代码
            break;
    }
    return TRUE;
}