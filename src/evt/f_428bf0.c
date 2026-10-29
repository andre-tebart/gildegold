/* 0x428bf0 */

void evt_Include(void)
{
    EVTTOKEN t;
    EVTSCRIPT *sc;
    char *msg;
    int i;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_STRING) {
        sc = evt_Load(t.u.s);
        if (sc) {
            evt_Compile(sc);
            for (i = 0; i < 8; i++)
                if (!evt_Cur->includes[i]) {
                    evt_Cur->includes[i] = sc;
                    return;
                }
            return;
        }
        evt_Error(evt_Cur, evt_Cur->pc, "Error including the script!");
        evt_Stop(evt_Cur);
    } else {
        evt_Error(evt_Cur, evt_Cur->pc, "Wrong include parameter!");
        evt_Stop(evt_Cur);
    }
}
