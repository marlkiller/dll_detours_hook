namespace DotnetHarmonyHook;

[AttributeUsage(AttributeTargets.Class, Inherited = false)]
public class HookRegistrationAttribute : Attribute
{
    public string ProcessName { get; }

    public HookRegistrationAttribute(string processName)
    {
        ProcessName = processName;
    }
}
