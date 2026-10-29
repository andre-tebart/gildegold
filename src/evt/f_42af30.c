/* 0x42af30 */

void evt_Shutdown(void)
{
    int i;

    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].name[0])
            evt_Unload(&evt_Scripts[i]);
    for (i = 0; i < 256; i++)
        evt_Builtins[i].name[0] = 0;
    if (evt_Globals) {
        mem_Free(evt_Globals);
        evt_Globals = 0;
    }
    if (evt_Builtins) {
        mem_Free(evt_Builtins);
        evt_Builtins = 0;
    }
    if (evt_Args) {
        mem_Free(evt_Args);
        evt_Args = 0;
    }
    if (evt_Console) {
        mem_Free(evt_Console);
        evt_Console = 0;
    }
    if (evt_Log) {
        mem_Free(evt_Log);
        evt_Log = 0;
    }
    if (evt_Scripts) {
        mem_Free(evt_Scripts);
        evt_Scripts = 0;
    }
}
