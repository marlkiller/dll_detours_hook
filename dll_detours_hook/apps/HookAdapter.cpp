#include "HookAdapter.h"
#include <Windows.h>
#include <ShlObj.h>
#include <string>
#include "Logger.h"
#include "RegistryUtils.h"
#include "StringUtils.h"
#include <vector>
#include <algorithm>
#pragma comment(lib, "Version.lib")

void HookAdapter::OnFirstLaunch() {
     MessageBoxW(
        nullptr,
        L"当前为免费开源的破解版本。如您是付费购买所得，请及时举报售卖渠道并给予差评，避免他人受骗。\n\n免责声明：本版本仅供学习与研究用途，开发者不对由此产生的任何后果承担责任。",
        L"免责声明",
        MB_OK | MB_ICONINFORMATION
    );
}

std::string GetInjectVersion()
{
    wchar_t exePath[MAX_PATH] = {0};
    if (GetModuleFileNameW(nullptr, exePath, MAX_PATH) == 0) {
        return "0.0.0.0";
    }

    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeW(exePath, &handle);
    if (size == 0) {
        return "0.0.0.0";
    }

    std::vector<BYTE> buffer(size);
    if (!GetFileVersionInfoW(exePath, 0, size, buffer.data())) {
        return "0.0.0.0";
    }

    VS_FIXEDFILEINFO* fileInfo = nullptr;
    UINT len = 0;
    if (!VerQueryValueW(buffer.data(), L"\\", reinterpret_cast<LPVOID*>(&fileInfo), &len)) {
        return "0.0.0.0";
    }

    if (fileInfo == nullptr) {
        return "0.0.0.0";
    }

    DWORD major = HIWORD(fileInfo->dwFileVersionMS);
    DWORD minor = LOWORD(fileInfo->dwFileVersionMS);
    DWORD build = HIWORD(fileInfo->dwFileVersionLS);
    DWORD revision = LOWORD(fileInfo->dwFileVersionLS);

    char version[64];
    snprintf(version, sizeof(version), "%u.%u.%u.%u",
             major, minor, build, revision);

    return version;
}

void HookAdapter::CheckFirstLaunch() {
    // const std::string appName = GetProcessName();
    const std::string currentVersion = GetInjectVersion();
	
    WCHAR processPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, processPath, MAX_PATH) == 0) {
        LOG_DEBUG("Failed to get current process path for registry key.");
        return;
    }
    std::wstring appKey(processPath);
    std::replace(appKey.begin(), appKey.end(), L'\\', L'_');


    const std::wstring subKeyPath = L"Software\\DllDetoursHook\\" + appKey;
    const std::wstring valueName = L"inject_version";

    LOG_DEBUG_W(L"Checking for first launch for app: %ls, version: %hs", appKey.c_str(), currentVersion.c_str());

    RegistryUtils::RegistryKey regKey;
    std::wstring storedVersion;
    bool needsUpdate = true;

    if (regKey.Open(HKEY_CURRENT_USER, subKeyPath, KEY_READ)) {
        if (regKey.GetStringValue(valueName, storedVersion)) {
            if (StringUtils::s2ws(currentVersion) == storedVersion) {
                needsUpdate = false;
                LOG_DEBUG("Stored version matches current version.");
            } else {
                LOG_DEBUG_W(L"Stored version (%ls) differs from current version (%hs).", storedVersion.c_str(), currentVersion.c_str());
            }
        } else {
            LOG_DEBUG("inject_version value not found.");
        }
        regKey.Close();
    } else {
        LOG_DEBUG("Registry key not found, assuming first launch.");
    }

    if (needsUpdate) {
        LOG_DEBUG("First launch detected or version changed. Calling OnFirstLaunch() and updating registry.");
        OnFirstLaunch();
        if (regKey.Create(HKEY_CURRENT_USER, subKeyPath, KEY_WRITE)) {
            if (regKey.SetStringValue(valueName, StringUtils::s2ws(currentVersion))) {
                LOG_DEBUG("Successfully updated registry with version: %s", currentVersion.c_str());
            } else {
                LOG_DEBUG("Failed to write version to registry.");
            }
            regKey.Close();
        } else {
            LOG_DEBUG("Failed to create registry key.");
        }
    }
}
