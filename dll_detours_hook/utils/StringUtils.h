#pragma once

#include <string>    // For std::string
#include <string_view> // For std::string_view
#include <cstdint>   // For uint16_t, int64_t
#include <windows.h>

namespace StringUtils {

// Helper to convert std::string to std::wstring
std::wstring s2ws(const std::string& str);

// Formats a byte array as a hexadecimal string.
std::string FormatHexString(const unsigned char* data, int len);

// Converts a UTF-16 string to a UTF-8 string.
std::string UTF16ToUTF8(const uint16_t* data, int64_t size);

// Checks if a UTF-16 string starts with a given UTF-16 text and a UTF-8 pattern.
bool UTF16StartsWith(const uint16_t* data, int64_t size, std::string_view pattern);

void SetClipboardStr(const wchar_t* text);
} // namespace StringUtils
