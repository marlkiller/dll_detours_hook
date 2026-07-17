using System.Runtime.InteropServices;

namespace DotnetHarmonyHook;

public static class HarmonyLogger
{
    [DllImport("kernel32.dll", EntryPoint = "OutputDebugStringW", CharSet = CharSet.Unicode)]
    private static extern void OutputDebugStringW(string message);

    public static void Debug(string message)
    {
        OutputDebugStringW($"[HarmonyHook] {message}\n");
    }

    public static void Debug(string format, params object?[] args)
    {
        OutputDebugStringW($"[HarmonyHook] {string.Format(format, args)}\n");
    }
}
