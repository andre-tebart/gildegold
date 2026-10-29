/* 0x40b7b0, stops the other running scripts of the caller's group */

int evtc_StopOthers(void)
{
    int i;

    for (i = 0; i < 128; i++) {
        if (evt_Scripts[i].id != -1 && (evt_Scripts[i].flags & 1) && evt_Scripts[i].group == evt_Cur->group
            && evt_Scripts[i].id != evt_Cur->id)
            evt_Stop(&evt_Scripts[i]);
    }
    return 0;
}
