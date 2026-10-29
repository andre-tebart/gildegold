/* 0x4284e0 */

EVTSCRIPT *evt_FindByName(char *name)
{
    int i;

    for (i = 0; i < 128; i++)
        if (!_strcmpi(evt_Scripts[i].name, name))
            return &evt_Scripts[i];
    return 0;
}
