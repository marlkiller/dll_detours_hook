#include "RegistryUtils.h"
#include <vector>

namespace RegistryUtils {

RegistryKey::RegistryKey() : hKey_(NULL) {}

RegistryKey::~RegistryKey() {
    Close();
}

bool RegistryKey::Open(HKEY hRootKey, const std::wstring& subKey, REGSAM desiredAccess) {
    Close();
    LSTATUS status = RegOpenKeyExW(hRootKey, subKey.c_str(), 0, desiredAccess, &hKey_);
    return status == ERROR_SUCCESS;
}

bool RegistryKey::Create(HKEY hRootKey, const std::wstring& subKey, REGSAM desiredAccess) {
    Close();
    DWORD disposition;
    LSTATUS status = RegCreateKeyExW(hRootKey, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, desiredAccess, NULL, &hKey_, &disposition);
    return status == ERROR_SUCCESS;
}

void RegistryKey::Close() {
    if (hKey_ != NULL) {
        RegCloseKey(hKey_);
        hKey_ = NULL;
    }
}

bool RegistryKey::SetStringValue(const std::wstring& valueName, const std::wstring& value) {
    if (hKey_ == NULL) {
        return false;
    }
    LSTATUS status = RegSetValueExW(hKey_, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(value.c_str()), (DWORD)((value.length() + 1) * sizeof(wchar_t)));
    return status == ERROR_SUCCESS;
}

bool RegistryKey::GetStringValue(const std::wstring& valueName, std::wstring& value) {
    if (hKey_ == NULL) {
        return false;
    }
    DWORD valueSize;
    LSTATUS status = RegQueryValueExW(hKey_, valueName.c_str(), NULL, NULL, NULL, &valueSize);
    if (status != ERROR_SUCCESS) {
        return false;
    }

    std::vector<wchar_t> buffer(valueSize / sizeof(wchar_t));
    status = RegQueryValueExW(hKey_, valueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(buffer.data()), &valueSize);
    if (status != ERROR_SUCCESS) {
        return false;
    }

    value = buffer.data();
    return true;
}

} // namespace RegistryUtils
