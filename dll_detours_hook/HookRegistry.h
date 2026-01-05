#ifndef DLL_DETOURS_HOOK_HOOK_REGISTRY_H
#define DLL_DETOURS_HOOK_HOOK_REGISTRY_H

#include <string>
#include <functional>
#include <vector>
#include <utility>

// Forward declaration
class HookAdapter;

// Type definition for a function that creates a HookAdapter instance
using HookCreatorFn = std::function<HookAdapter*()>;

// Global registry for hook creators
extern std::vector<std::pair<std::string, HookCreatorFn>> g_hookCreators;

// Function to register a hook creator
// Defined as extern "C" for potential cross-language compatibility if ever needed,
// but primarily to ensure consistent linkage across compilation units.
extern "C" void RegisterHookCreator(const std::string& processName, HookCreatorFn creator);

// Macro to simplify hook registration in each hook implementation file
#define REGISTER_HOOK(HookClassName, processName) \
namespace { \
    struct HookClassName ## Registrar { \
        HookClassName ## Registrar() { \
            RegisterHookCreator(processName, []() -> HookAdapter* { return new HookClassName(); }); \
        } \
    }; \
    static HookClassName ## Registrar g_ ## HookClassName ## _registrar; \
}

#endif // DLL_DETOURS_HOOK_HOOK_REGISTRY_H
