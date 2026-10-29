/* 0x42b410 */

int evt_Unload(EVTSCRIPT *sc)
{
    int i;

    if (!sc)
        return 0;
    if (sc->vars) {
        for (i = 0; i < sc->nvars; i++) {
            if (sc->vars[i].data.p) {
                mem_Free(sc->vars[i].data.p);
                sc->vars[i].data.p = 0;
                sc->vars[i].data.p = 0;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (sc->includes[i]) {
            evt_Unload(sc->includes[i]);
            sc->includes[i] = 0;
        }
    }
    if (sc->vars) {
        mem_Free(sc->vars);
        sc->vars = 0;
    }
    if (sc->funcs) {
        mem_Free(sc->funcs);
        sc->funcs = 0;
    }
    if (sc->source) {
        mem_Free(sc->source);
        sc->source = 0;
    }
    if (sc->lines) {
        mem_Free(sc->lines);
        sc->lines = 0;
    }
    if (sc->stack) {
        mem_Free(sc->stack);
        sc->stack = 0;
    }
    memset(sc, 0, sizeof(EVTSCRIPT));
    sc->id = -1;
    return 1;
}
