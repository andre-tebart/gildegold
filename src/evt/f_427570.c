/* 0x427570 */

EVTVAR *evt_AddGlobal(char *name, void *data, int type)
{
    EVTVAR *v;

    if (evt_MaxGlobals < evt_NGlobals + 0x30U) {
        v = mem_Alloc((evt_NGlobals + 16) * sizeof(EVTVAR), "evt:t");
        if (evt_Globals) {
            memcpy(v, evt_Globals, evt_NGlobals * sizeof(EVTVAR));
            mem_Free(evt_Globals);
        }
        evt_MaxGlobals += 0x300;
        evt_Globals = v;
    }
    strcpy(evt_Globals[evt_NGlobals].name, name);
    evt_Globals[evt_NGlobals].type = type;
    evt_Globals[evt_NGlobals].local = 0;
    evt_Globals[evt_NGlobals].data.p = data;
    evt_Globals[evt_NGlobals].count = 1;
    evt_NGlobals++;
    return &evt_Globals[evt_NGlobals - 1];
}
