# dll_detours_hook




## 动态注入
withdll.exe /d:..\release\dll_detours_hook.dll "D:\tools\app_demo.exe"

## 静态注入
### 绝对路径
setdll.exe /d:..\release\dll_detours_hook.dll "D:\tools\app_demo.exe"
### 相对路径
"D:\workspace\code\c\dll_detours_hook\tools\setdll.exe" /d:dll_detours_hook.dll "app_demo.exe"

TODO....
1. 配置 github action 生成 release
2. Hack some app

