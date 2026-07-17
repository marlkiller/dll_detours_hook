using System.Reflection;

namespace DotnetHarmonyHook;

public static class HookRegistry
{
    private static readonly List<(string ProcessName, Func<HarmonyHookAdapter> Creator)> _hookCreators = [];

    public static IReadOnlyList<(string ProcessName, Func<HarmonyHookAdapter> Creator)> HookCreators => _hookCreators;

    public static void RegisterHook(string processName, Func<HarmonyHookAdapter> creator)
    {
        _hookCreators.Add((processName, creator));
    }

    public static void AutoRegisterHooks()
    {
        var assembly = Assembly.GetCallingAssembly();
        foreach (var type in assembly.GetTypes())
        {
            var attr = type.GetCustomAttribute<HookRegistrationAttribute>();
            if (attr is null || !typeof(HarmonyHookAdapter).IsAssignableFrom(type))
                continue;

            RegisterHook(attr.ProcessName, () => (HarmonyHookAdapter)Activator.CreateInstance(type)!);
            HarmonyLogger.Debug($"Registered hook: {type.Name} -> {attr.ProcessName}");
        }
    }
}
