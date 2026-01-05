#pragma once
#include "HookAdapter.h"

class DemoHook : public HookAdapter {
public:
    const char* GetProcessName() const override;
    bool InstallHook() override;
    void UninstallHook() override;
};
