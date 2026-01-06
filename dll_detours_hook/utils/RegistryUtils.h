#pragma once

#include <string>
#include <Windows.h>

namespace RegistryUtils {

class RegistryKey {
public:
    RegistryKey();
    ~RegistryKey();

    bool Open(HKEY hRootKey, const std::wstring& subKey, REGSAM desiredAccess);
    bool Create(HKEY hRootKey, const std::wstring& subKey, REGSAM desiredAccess);
    void Close();

    bool SetStringValue(const std::wstring& valueName, const std::wstring& value);
    bool GetStringValue(const std::wstring& valueName, std::wstring& value);

private:
    HKEY hKey_;
};

} // namespace RegistryUtils
