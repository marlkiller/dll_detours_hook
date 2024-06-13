#include "dll_detours_hook.h"
#include "stdlib.h"
#include <iostream>
#include <Windows.h>
#include "detours.h"
#include "detver.h"
//#pragma comment(lib,"detours.lib")
//#pragma comment (lib,"libs\detours.lib")
//#pragma comment (lib,"libs/detours.lib")


// ʵ�ֵ����ĺ���
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
////������غ���
//void EndHook() {
//    //��ʼ����
//    DetourTransactionBegin();
//    //�����߳���Ϣ 
//    DetourUpdateThread(GetCurrentThread());
//    //�����غ�����ԭ�����ĵ�ַ�Ͻ��
//    DetourDetach(&(PVOID&)original_sum, (PVOID)mySum);
//    //��������
//    DetourTransactionCommit();
//}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    std::cout << "Hello, dll_detours_hook!" << std::endl;

    printf("before %d\n", sum(1,2));
    // DobbyHook(sum, mySum, (void *)&sum_p);

    //��ʼ����
    DetourTransactionBegin();
    //�����߳���Ϣ
    DetourUpdateThread(GetCurrentThread());
    //�����غ������ӵ�ԭ�����ĵ�ַ��
    DetourAttach(&(PVOID&)original_sum, (PVOID)mySum);
    //��������
    DetourTransactionCommit();
    
    
    printf("after %d\n", sum(1,2));
    
    
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            // ���̱����غ�ִ��
            break;
        case DLL_THREAD_ATTACH:
            // �̱߳����������
            break;
        case DLL_THREAD_DETACH:
            // �����˳�ִ�еĴ���
            break;
        case DLL_PROCESS_DETACH:
            // ����ж�ر�Dll��ִ�еĴ���
            break;
    }
    return TRUE;
}