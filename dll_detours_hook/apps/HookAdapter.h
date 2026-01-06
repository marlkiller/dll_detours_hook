#pragma once

#include <string>

class HookAdapter {
public:
    virtual ~HookAdapter() = default;

    // Returns the name of the process this adapter is for.
    virtual const char* GetProcessName() const = 0;

    // Checks if this adapter should be used for the current process.
    virtual bool ShouldHook(const std::string& processName) const {
        return _stricmp(processName.c_str(), GetProcessName()) == 0;
    }

    // Installs the actual hooks.
    virtual bool InstallHook() = 0;

    // Uninstalls the hooks.
    virtual void UninstallHook() {}

    virtual void OnFirstLaunch();

    // Checks if this is the first launch and calls OnFirstLaunch if it is.
    void CheckFirstLaunch();

};
