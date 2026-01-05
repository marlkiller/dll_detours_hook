#pragma once

#include <Windows.h>
#include <cstdio>
#include <cstdarg>

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
    #define LOG_DEBUG(fmt, ...) __log_debug_impl(__func__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...) do {} while (0)
#endif