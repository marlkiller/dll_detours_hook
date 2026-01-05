//
// Created by EDY on 2024/5/31.
//
//

#include "binmain.h"
#include <iostream>
#include <Windows.h>
#include "detours.h"
#include "detver.h"

using namespace std;


int sum_(int a, int b) {
    return a + b;
}
static int (*original_sum_)(int a, int b) = sum_;
//static int (*original_sum_2)(int, int) = &sum_;

int mySum_(int a, int b) {
    return a * b;
}

int main()
{
    cout << "Hello CMake." << endl;

    printf("before %d\n", sum_(1,2));

    // install
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)original_sum_, (PVOID)mySum_);
    DetourTransactionCommit();
    
    printf("after %d\n", sum_(1,2));


    // uninstall
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)original_sum_, (PVOID)mySum_);
    DetourTransactionCommit();
    
    printf("after %d\n", sum_(1,2));

    return 0;
}
