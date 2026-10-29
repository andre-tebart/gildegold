/* 0x43e6b0, default for d3e_Resolve: the handler of an event, by name */

D3EVHANDLER d3e_DefaultResolve(void *obj, int id, char *name)
{
    return (D3EVHANDLER)ai_AlwaysTrue;
}
