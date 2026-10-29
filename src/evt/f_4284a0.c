/* 0x4284a0, the script with this id among the 128 slots of evt_Scripts, 0 if none (-1 = no id) */

EVTSCRIPT *evt_FindById(int id)
{
    int i;

    if (id == -1)
        return 0;
    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].id == id)
            return &evt_Scripts[i];
    return 0;
}
