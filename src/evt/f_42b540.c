/* 0x42b540 */

EVTBUILTIN *evt_FindBuiltin(char *name)
{
    int i;

    for (i = 0; i < 256; i++)
        if (!strcmp(evt_Builtins[i].name, name))
            break;
    if (i < 256)
        return &evt_Builtins[i];
    return 0;
}
