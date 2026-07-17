# DLL Detours Hook & .NET Harmony Hook

[![Telegram](https://img.shields.io/badge/Join%20our%20Telegram-blue?logo=telegram)](https://t.me/dylib_dobby_hook_chat)

A Windows DLL injection project for enhancing software functionality through API hooking, supporting both **native (C++)** and **managed (.NET)** applications.

## Development Environment

- Windows
- Visual Studio / CLion / JetBrains Rider
- CMake / .NET SDK
- Debugging Tools: x64dbg, IDA Pro, DebugView

## Project Structure

```
├── dll_detours_hook/          Native C++ hook DLL (Microsoft Detours)
│   ├── apps/                  Hook implementations (Snipaste, Demo, etc.)
│   └── utils/                 Common utilities (Logger, etc.)
├── dotnet_harmony_hook/       Managed .NET hook DLL (Lib.Harmony)
│   └── src/
│       ├── hooks/             .NET hook implementations (Listary, Demo, etc.)
│       ├── HarmonyUtils.cs    Harmony patching wrapper
│       └── HookAppDomainManager.cs  CLR injection entry
├── script/
│   ├── inject.bat             Native DLL injection (setdll.exe / IAT)
│   └── inject_dotnet.bat      .NET DLL injection (AppDomainManager)
├── libs/                      detours.lib
├── tools/                     setdll.exe, withdll.exe, DebugView, Xenos
└── release/                   Build output
```

See [supported-apps.md](./supported-apps.md) for the list of supported apps.

## Build

### Native (C++ Detours)

```cmd
build.cmd
```

Output: `release/dll_detours_hook.dll`

### Managed (.NET Harmony)

```cmd
cd dotnet_harmony_hook
build.cmd
```

Output: `release/dotnet_harmony_hook/DotnetHarmonyHook.dll` (targets .NET Framework 4.8 + .NET 8.0)

## Injection

### Native (C++ Detours)

#### Dynamic Injection
Launches target with DLL injected at startup — no disk modification.

```cmd
withdll.exe /d:release\dll_detours_hook.dll C:\Path\To\app_demo.exe
```

#### Static Injection (IAT Modification) — Recommended
Permanently adds the DLL to the target's import table.

```cmd
script\inject.bat "C:\Path\To\app_demo.exe"
```

Or with a custom DLL path:

```cmd
script\inject.bat "C:\Path\To\app_demo.exe" "D:\path\to\dll_detours_hook.dll"
```

⚠️ **This permanently modifies the executable. Always back up the original.**

### Managed (.NET Harmony)

Uses the AppDomainManager technique — no disk modification required.

```cmd
script\inject_dotnet.bat "C:\Path\To\TargetApp.exe"
```

The script copies `DotnetHarmonyHook.dll` into the target directory and launches the process with `APPDOMAIN_MANAGER` environment variables set.

## View Logs

Run `tools/DebugView/DebugView.exe` to monitor debug output from the hooks.

## Develop

### Adding a Native Hook (C++ / Detours)

1. Create `MyHook.h` and `MyHook.cpp` in `dll_detours_hook/apps/`
2. Inherit from `HookAdapter`, implement `InstallHook()` and `GetProcessName()`
3. Register with `REGISTER_HOOK(MyHook, "target.exe")`

Example: see [DemoHook](dll_detours_hook/apps/DemoHook.cpp)

### Adding a Managed Hook (C# / Harmony)

1. Create `MyHook.cs` in `dotnet_harmony_hook/src/hooks/`
2. Use `HarmonyUtils.PatchMethod()` to apply Harmony patches
3. Register via `HookRegistry.Register<MyHook>("TargetApp.exe")`

Example: see [DemoNetHook](dotnet_harmony_hook/src/hooks/DemoNetHook.cs)

## Powered by

[![JetBrains logo.](https://resources.jetbrains.com/storage/products/company/brand/logos/jetbrains.svg)](https://jb.gg/OpenSource)

## Warning

For research and learning purposes only. Do not use for illegal purposes.
