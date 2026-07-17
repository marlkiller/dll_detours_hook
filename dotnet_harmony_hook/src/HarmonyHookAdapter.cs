namespace DotnetHarmonyHook;

public abstract class HarmonyHookAdapter
{
    public abstract string GetProcessName();

    public virtual bool ShouldHook(string processName)
    {
        return string.Equals(processName, GetProcessName(), StringComparison.OrdinalIgnoreCase);
    }

    public abstract bool InstallHook();

    public virtual void UninstallHook()
    {
    }

    public virtual void OnFirstLaunch()
    {
    }

    public void CheckFirstLaunch()
    {
        OnFirstLaunch();
    }
}
