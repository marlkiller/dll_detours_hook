# DotnetHarmonyHook

.NET method hooking framework based on **Harmony (Lib.Harmony)**. Mirrors the C++ `dll_detours_hook` project architecture.

## Build

```cmd
cd dotnet_harmony_hook
build.cmd
```

Requires [.NET SDK](https://dotnet.microsoft.com/download). Output: `release/dotnet_harmony_hook/DotnetHarmonyHook.dll`

## Injection

### AppDomainManager (for .NET Framework apps)

1. Copy `DotnetHarmonyHook.dll` to the target app's directory
2. Open cmd in that directory and run:

```cmd
set APPDOMAIN_MANAGER_ASM=DotnetHarmonyHook
set APPDOMAIN_MANAGER_TYPE=DotnetHarmonyHook.HookAppDomainManager
target.exe
```

## Creating a New Hook

```csharp
[HookRegistration("target.exe")]
class MyHook : HarmonyHookAdapter
{
    public override string GetProcessName() => "target.exe";

    public override bool InstallHook()
    {
        var method = typeof(TargetClass).GetMethod("TargetMethod");
        var prefix = new HarmonyMethod(typeof(MyHook), nameof(Prefix));
        return HarmonyUtils.Patch(method, prefix: prefix);
    }

    static bool Prefix(ref bool __result)
    {
        __result = true;
        return false;
    }
}
```

See `src/hooks/ListaryHook.cs` for a real example.