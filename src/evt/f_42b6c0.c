/* 0x42b6c0 */

int evt_StartScript(int unused1, int unused2, char *name)
{
    EVTSCRIPT *sc;

    sc = evt_Load(name);
    if (sc) {
        evt_RunScript(sc);
        return evt_LastId = sc->id;
    }
    return evt_LastId = -1;
}
