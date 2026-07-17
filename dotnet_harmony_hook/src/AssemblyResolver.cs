using System.Reflection;

namespace DotnetHarmonyHook;

internal static class AssemblyResolver
{
    private static bool _attached;

    internal static void Attach()
    {
        if (_attached)
            return;
        _attached = true;
        AppDomain.CurrentDomain.AssemblyResolve += Resolve;
    }

    private static Assembly? Resolve(object sender, ResolveEventArgs args)
    {
        var name = new AssemblyName(args.Name).Name;
        if (name != "0Harmony")
            return null;

        var prefix = typeof(AssemblyResolver).Assembly.GetName().Name + ".";
        var resourceName = typeof(AssemblyResolver).Assembly.GetManifestResourceNames()
            .FirstOrDefault(n => n == prefix + "0Harmony.dll");

        if (resourceName is null)
            return null;

        using var stream = typeof(AssemblyResolver).Assembly.GetManifestResourceStream(resourceName);
        if (stream is null)
            return null;

        var data = new byte[stream.Length];
        _ = stream.Read(data, 0, data.Length);
        return Assembly.Load(data);
    }
}
