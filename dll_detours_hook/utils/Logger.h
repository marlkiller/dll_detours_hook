#pragma once

#include <Windows.h>
#include <cstdio>
#include <cstdarg>
#include <cwchar>

#ifdef _DEBUG
    inline void __log_debug_impl(const char* func, int line, const char* fmt, ...) {
        char buf[2048];
        char user_buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(user_buf, sizeof(user_buf), fmt, args);
        va_end(args);
        sprintf_s(buf, sizeof(buf), "[HOOK_DEBUG] %s:%d: %s\n", func, line, user_buf);
        OutputDebugStringA(buf);
    }

    inline void __log_debug_impl_w(const char* func, int line, const wchar_t* fmt, ...) {
        wchar_t buf[2048];
        wchar_t user_buf[1024];
        va_list args;
        va_start(args, fmt);
        vswprintf(user_buf, sizeof(user_buf)/sizeof(wchar_t), fmt, args);
        va_end(args);
        swprintf_s(buf, sizeof(buf)/sizeof(wchar_t), L"[HOOK_DEBUG] %hs:%d: %ls\n", func, line, user_buf);
        OutputDebugStringW(buf);
    }

    // Use LOG_DEBUG for logging messages containing only ASCII characters.
    // It uses OutputDebugStringA.
    #define LOG_DEBUG(fmt, ...) __log_debug_impl(__func__, __LINE__, fmt, ##__VA_ARGS__)

    // Use LOG_DEBUG_W for logging messages that may contain non-ASCII characters (e.g., file paths).
    // It uses OutputDebugStringW to ensure proper display of Unicode characters in debug viewers like DbgView.
    // Remember to use wide string literals (L"...") and the %ls format specifier for wide strings.
	#define LOG_DEBUG_W(fmt, ...) __log_debug_impl_w(__func__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...) do {} while (0)
	#define LOG_DEBUG_W(fmt, ...) do {} while (0)
#endif