#include "HookRegistry.h"
#include "HookAdapter.h" // Needed for HookAdapter definition

// Define the global hook creators registry
std::vector<std::pair<std::string, HookCreatorFn>> g_hookCreators;

// Implement the RegisterHookCreator function
extern "C" void RegisterHookCreator(const std::string& processName, HookCreatorFn creator) {
    g_hookCreators.push_back({processName, creator});
}
