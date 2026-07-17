using System.Reflection;
using System.Windows.Forms;
using HarmonyLib;

namespace DotnetHarmonyHook.Hooks;

[HookRegistration("demo_app.exe")]
public class DemoNetHook : HarmonyHookAdapter
{
    private static MethodInfo? _originalMessageBoxShow;

    public override string GetProcessName() => "demo_app.exe";

    public override bool InstallHook()
    {
        var original = typeof(MessageBox).GetMethod("Show", [typeof(string), typeof(string)]);
        if (original is null)
        {
            HarmonyLogger.Debug("Failed to find MessageBox.Show(string, string)");
            return false;
        }

        _originalMessageBoxShow = original;

        var prefix = new HarmonyMethod(typeof(DemoNetHook), nameof(Prefix_MessageBoxShow));
        return HarmonyUtils.Patch(original, prefix: prefix);
    }

    public override void UninstallHook()
    {
        if (_originalMessageBoxShow is not null)
        {
            HarmonyUtils.Unpatch(_originalMessageBoxShow);
            _originalMessageBoxShow = null;
        }
    }

    public static bool Prefix_MessageBoxShow(ref string text, ref string caption)
    {
        HarmonyLogger.Debug("MessageBox.Show hooked!");
        HarmonyLogger.Debug("Original caption: {0}", caption);
        HarmonyLogger.Debug("Original text: {0}", text);

        caption = "Hooked by DemoNetHook!";
        text = "This message box has been hijacked by Harmony.";

        return true;
    }
}
