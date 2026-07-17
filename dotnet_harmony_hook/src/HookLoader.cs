namespace DotnetHarmonyHook;

public static class HookLoader
{
    private static readonly List<HarmonyHookAdapter> _activeHooks = [];
    private static Func<HarmonyHookAdapter>? _pendingHookCreator;
    private static bool _initialized;
    private static bool _installing;

    public static void Initialize()
    {
        if (_initialized)
            return;
        _initialized = true;

#if NETFRAMEWORK
        var processPath = System.Diagnostics.Process.GetCurrentProcess().MainModule?.FileName;
#else
        var processPath = Environment.ProcessPath;
#endif
        var processName = processPath is not null
            ? Path.GetFileName(processPath)
            : "unknown";
        HarmonyLogger.Debug("DLL loaded into process: {0}", processName);

        HookRegistry.AutoRegisterHooks();

        foreach (var (targetProcess, creator) in HookRegistry.HookCreators)
        {
            var tempHook = creator();
            if (!tempHook.ShouldHook(processName))
                continue;

            _pendingHookCreator = creator;
            AppDomain.CurrentDomain.AssemblyLoad += (_, _) => InstallPendingHook();
            InstallPendingHook();
            break;
        }
    }

    private static void InstallPendingHook()
    {
        if (_pendingHookCreator is null || _installing)
            return;

        _installing = true;
        try
        {
            var hook = _pendingHookCreator();
            hook.CheckFirstLaunch();
            HarmonyLogger.Debug("Attempting to install hook for process: {0}",
                hook.GetProcessName());

            if (hook.InstallHook())
            {
                _activeHooks.Add(hook);
                _pendingHookCreator = null;
            }
        }
        finally
        {
            _installing = false;
        }
    }

    public static void Cleanup()
    {
        HarmonyLogger.Debug("Cleaning up hook objects.");
        foreach (var hook in _activeHooks)
        {
            hook.UninstallHook();
        }
        _activeHooks.Clear();
        _pendingHookCreator = null;
    }
}
