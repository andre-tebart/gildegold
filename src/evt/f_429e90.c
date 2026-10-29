/* 0x429e90 */

int evt_Stop(EVTSCRIPT *sc)
{
    unsigned int flags;
    EVTFUNC *f;

    if (!sc)
        return 0;
    flags = sc->flags;
    sc->call = 0;
    sc->waitfn = 0;
    if (!(flags & 2)) {
        f = evt_FindFunc(sc, "exit");
        if (f) {
            sc->flags = flags | 3;
            evt_EnterFunction(sc, f);
            sc->mode = 2;
            evt_Step(sc);
            return 0;
        }
    }
    sc->id = 0;
    sc->flags = 0;
    if (sc->character)
        sc->character->scriptId = -1;
    evt_Error(sc, 0, "Script done...");
    evt_Unload(sc);
    return 1;
}
