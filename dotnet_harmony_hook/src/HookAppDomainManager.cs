namespace DotnetHarmonyHook;

#if NETFRAMEWORK
public class HookAppDomainManager : AppDomainManager
{
    public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
    {
        base.InitializeNewDomain(appDomainInfo);

        try
        {
            AssemblyResolver.Attach();
            HookLoader.Initialize();
        }
        catch
        {
        }
    }
}
#endif
