//
// Created by EDY on 2024/5/31.
//
// CMakeProject1.cpp: 定义应用程序的入口点。
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

    //开始事务
    DetourTransactionBegin();
    //更新线程信息
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)original_sum_, (PVOID)mySum_);
    //结束事务
    DetourTransactionCommit();
    
    printf("after %d\n", sum_(1,2));


    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //将拦截函数从原函数的地址上解除
    DetourDetach(&(PVOID&)original_sum_, (PVOID)mySum_);
    DetourTransactionCommit();
    
    printf("after %d\n", sum_(1,2));

    return 0;
}
