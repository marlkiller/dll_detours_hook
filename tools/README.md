
编译 Detours 需要 **Visual Studio**（自带 C++ 开发组件）。

### 核心工具集：
* **setdll.exe**: 静态注入工具（修改 IAT 导入表）。
* **withdll.exe**: 动态注入启动器（不修改文件，仅在运行时注入）。

---

## 2. 编译步骤 (CLI 方式)

虽然 Detours 使用 `nmake` 构建，但只要安装了 VS，环境就是现成的。

1.  **打开开发者命令提示符**：
    * 在 Windows 搜索框输入 `Native Tools`。
    * **注入 64 位程序**：选择 `x64 Native Tools Command Prompt for VS 2022`。
    * **注入 32 位程序**：选择 `x86 Native Tools Command Prompt for VS 2022`。

2.  **执行编译**：
    ```cmd
    # 进入源码目录
    cd /d C:\path\to\Detours
    
    # 执行 VS 自带的编译命令
    nmake
    ```

3.  **获取成品**：
    * 编译完成后，在 `bin.X64` 或 `bin.X86` 目录下即可找到 `setdll.exe` 和 `withdll.exe`。