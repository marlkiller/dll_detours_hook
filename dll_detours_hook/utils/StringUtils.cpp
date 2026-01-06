#include "StringUtils.h"
#include <cstdio>   // For sprintf_s, snprintf
#include <string>   // For std::string
#include <vector>   // For std::vector<char> for intermediate buffer
#include <codecvt> // For std::wstring_convert
#include <locale>  // For std::wstring_convert

namespace StringUtils {
	
// Helper to convert std::string to std::wstring
std::wstring s2ws(const std::string& str)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    try {
        return converter.from_bytes(str);
    }
    catch (const std::range_error& e) {
        // This can happen if the input string is not valid UTF-8.
        // For this project, process names are typically ASCII, so this is less likely.
        // You could fall back to a different conversion method if needed.
        (void)e;
        size_t convertedChars = 0;
        std::vector<wchar_t> wstr(str.length() + 1);
        mbstowcs_s(&convertedChars, wstr.data(), wstr.size(), str.c_str(), _TRUNCATE);
        return std::wstring(wstr.data());
    }
}

std::string FormatHexString(const unsigned char* data, int len) {
    if (!data || len <= 0) {
        return "";
    }
    // Each byte becomes 2 hex chars + 1 space = 3 chars, except for the last one.
    // Plus null terminator for safety if building c-string internally.
    std::string hex_str;
    hex_str.reserve(len * 3); // Pre-allocate memory
    for (int i = 0; i < len; ++i) {
        char buf[4]; // Enough for "XX " + null
        sprintf_s(buf, sizeof(buf), "%02X ", data[i]);
        hex_str.append(buf);
    }
    if (!hex_str.empty()) {
        hex_str.pop_back(); // Remove the last space
    }
    return hex_str;
}

std::string UTF16ToUTF8(const uint16_t* data, int64_t size) {
    if (!data || size <= 0) {
        return "(empty)";
    }

    // Estimate maximum UTF-8 size (up to 3 bytes per UTF-16 char for non-BMP, 4 for surrogate pairs)
    // For simplicity and common use cases, assume up to 3 bytes per char for now
    // A more precise calculation would involve iterating to count actual bytes.
    // Max size of 5000 from original code.
    int64_t buffer_max_size = size * 3 + 1; // 3 bytes per UTF-16 char + null terminator
    if (buffer_max_size > 5000) buffer_max_size = 5000; // Cap at original max

    std::vector<char> buffer(buffer_max_size);
    int out_pos = 0;
    for (int64_t i = 0; i < size && out_pos + 4 < buffer.size(); i++) { // Ensure space for 4 bytes (max for 1 UTF-16 char)
        uint16_t ch = data[i];
        if (ch < 0x80) {
            buffer[out_pos++] = (char)ch;
        } else if (ch < 0x800) {
            if (out_pos + 2 < buffer.size()) {
                buffer[out_pos++] = (char)(0xC0 | (ch >> 6));
                buffer[out_pos++] = (char)(0x80 | (ch & 0x3F));
            }
        } else {
            if (out_pos + 3 < buffer.size()) { // For BMP characters
                buffer[out_pos++] = (char)(0xE0 | (ch >> 12));
                buffer[out_pos++] = (char)(0x80 | ((ch >> 6) & 0x3F));
                buffer[out_pos++] = (char)(0x80 | (ch & 0x3F));
            }
            // Handling surrogate pairs (U+10000 to U+10FFFF) would add more complexity,
            // but is not present in the original code's UTF-8 conversion logic.
        }
    }
    buffer[out_pos] = '\0';
    return std::string(buffer.data());
}

bool UTF16StartsWith(const uint16_t* data, int64_t size, std::string_view pattern) {
    if (!data || size <= 0) return false;
    if (pattern.empty()) return true;

    // Convert the start of the UTF-16 data to UTF-8 for comparison with pattern (which is UTF-8)
    // We need to be careful with sizing here. We can't simply compare UTF-16 code units to UTF-8 bytes directly.
    // The safest way is to convert a prefix of the UTF-16 string to UTF-8 and then compare.
    // Let's take a substring of the UTF-16 data equivalent in length to the pattern's length
    // (assuming ASCII for simple comparison, otherwise full conversion needed).
    // Original logic: if ((uint16_t)pattern[i] != data[i]) return 0;
    // This implies pattern was treated as single-byte characters which map directly to UTF-16 code points
    // if they are ASCII.
    
    // Given the pattern "fuck" from original usage, it's ASCII.
    // The original logic compares uint16_t code points with char bytes.
    // This implies that pattern is expected to be ASCII or Latin-1 characters
    // that directly map to the lower 128/256 code points of UTF-16.
    
    // Let's replicate the original comparison behavior, which is flawed for non-ASCII patterns.
    // A proper UTF-16 starts with UTF-8 check would require converting both to a common encoding
    // or doing proper multi-byte comparison.
    // For the "fuck" case, this original logic works because 'f','u','c','k' are single byte in UTF-8
    // and directly correspond to their UTF-16 values.

    for (size_t i = 0; i < pattern.length(); ++i) {
        if (i >= size) { // UTF-16 data is shorter than pattern
            return false;
        }
        if ((uint16_t)pattern[i] != data[i]) {
            return false;
        }
    }
    return true; // All characters matched
}

void SetClipboardStr(const wchar_t* text)
{
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();

    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE,
        (wcslen(text) + 1) * sizeof(wchar_t));

    if (h) {
        memcpy(GlobalLock(h), text,
               (wcslen(text) + 1) * sizeof(wchar_t));
        GlobalUnlock(h);
        SetClipboardData(CF_UNICODETEXT, h);
    }

    CloseClipboard();
}
} // namespace StringUtils