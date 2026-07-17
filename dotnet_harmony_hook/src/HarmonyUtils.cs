using System.Reflection;
using HarmonyLib;

namespace DotnetHarmonyHook;

public static class HarmonyUtils
{
    private static readonly Harmony _harmony = new("com.dotnet-harmony-hook");

    public static bool Patch(MethodBase original, HarmonyMethod? prefix = null,
        HarmonyMethod? postfix = null, HarmonyMethod? transpiler = null)
    {
        try
        {
            _harmony.Patch(original, prefix, postfix, transpiler);
            HarmonyLogger.Debug($"Patched method: {original.DeclaringType?.Name}.{original.Name}");
            return true;
        }
        catch (Exception ex)
        {
            HarmonyLogger.Debug($"Failed to patch {original.Name}: {ex.Message}");
            return false;
        }
    }

    public static void Unpatch(MethodBase original, HarmonyPatchType patchType = HarmonyPatchType.All)
    {
        try
        {
            _harmony.Unpatch(original, patchType);
            HarmonyLogger.Debug($"Unpatched method: {original.DeclaringType?.Name}.{original.Name}");
        }
        catch (Exception ex)
        {
            HarmonyLogger.Debug($"Failed to unpatch {original.Name}: {ex.Message}");
        }
    }

    public static Harmony Instance => _harmony;
}
