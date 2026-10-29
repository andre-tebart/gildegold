/* 0x428eb0 */

EVTVAR *evt_NewVar(EVTSCRIPT *sc, int type, int count)
{
    int size = 0;

    if (type == 1)
        size = 4;
    else if (type == 2)
        size = 1;
    else if (type == 6)
        size = 0x60;
    else if (type == 7)
        size = 4;
    else
        __asm int 3
    sc->vars[sc->nvars].type = type;
    sc->vars[sc->nvars].data.p = mem_Alloc(size * count, "evt:var");
    memset(sc->vars[sc->nvars].data.p, 0, size * count);
    sc->vars[sc->nvars].count = count;
    return &sc->vars[sc->nvars++];
}
