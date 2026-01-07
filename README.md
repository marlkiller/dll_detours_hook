# DLL Detours Hook

## About

[![Telegram](https://img.shields.io/badge/Join%20our%20Telegram-blue?logo=telegram)](https://t.me/dylib_dobby_hook_chat)

This project is a Windows DLL project aiming to enhance software functionality through API hooking, using the **Microsoft Detours** library.

**Development Environment:**

- Windows
- Visual Studio | CLion
- CMake
- Debugging Tools: x64dbg, IDA Pro

**Directory Structure:**

- **dll_detours_hook**: Main source code for the hook DLL.
  - **apps**: Contains specific hook implementations for different applications (e.g., `DemoHook`).
  - **utils**: Common utilities for logging, string manipulation, etc.
- **libs**: Libraries the project depends on (i.e., `detours.lib` from [Microsoft Detours](https://github.com/microsoft/Detours)).
- **build**: Default directory for compiled binaries.
- **tools**:
  - `withdll.exe`: Dynamic Injection tool from [Microsoft Detours](https://github.com/microsoft/Detours).
  - `setdll.exe`: Static Injection tool from [Microsoft Detours](https://github.com/microsoft/Detours).
  - `DebugView`: A utility to view debug output messages from the hooks.

## Features

Check the full list of supported apps [here](./supported-apps.md).


## Usage

This section explains how to get the necessary files and inject the DLL into a target application.

### 1. Get the Required Files

You have two options to get the hook DLL and injection tools.

#### Option A: Build from Source
Run the build script from the root directory. This will compile the project and place the output (`dll_detours_hook.dll`) in the `release` folder. The injection tools are in the `tools` directory.
```shell
build.cmd
```

#### Option B: Use Pre-built Release
Download the latest `dll_detours_hook.zip` from the [Releases](https://github.com/marlkiller/dll_detours_hook/releases) page and unzip it. It contains:
- `release/dll_detours_hook.dll`: The hook library.
- `tools/`: The injection tools directory.

### 2. Inject the DLL

Once you have the DLL and tools, you can inject it.

#### Dynamic Injection
This method uses `withdll.exe` to **launch a target application with the DLL injected at startup**.  
The injection happens at runtime and **does not modify the target executable on disk**.

This is often the simplest and most reliable way to test DLL hooks or runtime behavior.

```cmd
# Example: Execute from the 'tools' directory.
# Adjust the path to your DLL (from build or release) and the target application.
.\withdll.exe /d:..\release\dll_detours_hook.dll C:\Windows\System32\notepad.exe
```

#### Static Injection
This method uses `setdll.exe` to perform **static DLL injection** by **modifying the target executable file**.  
It works by updating the **Import Address Table (IAT)** so that the DLL is loaded automatically when the application starts.

⚠️ **Warning:**  
This method **permanently changes the executable on disk**.  
Always keep a backup of the original file before using static injection.

### Steps

1. Navigate to the target application's directory (for example, where `app_demo.exe` is located).
   ```cmd
   cd C:\Path\To\Target\App
   ```

2. Copy `dll_detours_hook.dll` into the same directory.

3. Run `setdll.exe` (using its full path) to modify the executable and add the DLL to its import table.
   ```cmd
   "D:\workspace\code\c\dll_detours_hook\tools\setdll.exe" /d:dll_detours_hook.dll "app_demo.exe"
   ```

### 3. View Logs
Run `DebugView.exe` from the `tools/DebugView` directory to monitor the debug output from the hooks (e.g., logs created with the `LOG_DEBUG` macro).

## Develop

The framework is designed to be easily extendable by creating new hook implementations.

### 1. Define a New Hook Class

Create new `.h` and `.cpp` files in the `dll_detours_hook/apps/` directory for your hook. The class should implement the required hooking logic.

**Example: A simple `DemoHook`**

```cpp
// --- In DemoHook.h ---
#pragma once

class DemoHook {
public:
    void InstallHook();
    const char* GetProcessName() override; // Added GetProcessName
};


// --- In DemoHook.cpp ---
#include "DemoHook.h"
#include "detours.h"
#include "utils/Logger.h"

// Define a pointer for the original function
static int (WINAPI* OriginalMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT) = MessageBoxW;

// Your custom detoured function
int WINAPI DetouredMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
    LOG_DEBUG("Hooked MessageBoxW!");
    return OriginalMessageBoxW(hWnd, L"Hooked!", lpCaption, uType);
}

void DemoHook::InstallHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)OriginalMessageBoxW, DetouredMessageBoxW);
    DetourTransactionCommit();
}

const char* DemoHook::GetProcessName() {
    return "c_bin_dev.exe"; // Example process name
}

REGISTER_HOOK(DemoHook, "c_bin_dev.exe");

```

## Powered by

[![JetBrains logo.](https://resources.jetbrains.com/storage/products/company/brand/logos/jetbrains.svg)](https://jb.gg/OpenSource) 

## WARNING

For research and learning purposes only. Please do not use for illegal purposes.   
Note: If reprinted, please indicate the source (link to this post) and author information.
