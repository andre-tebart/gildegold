/* 0x427a90 */

EVTFUNC *evt_FindFunc(EVTSCRIPT *sc, char *name)
{
    EVTSCRIPT *s = evt_Scope;
    int i;

    if (!s)
        s = sc;
    if (sc->funcs)
        for (i = 0; i < s->nfuncs; i++)
            if (!strcmp(s->funcs[i].name, name))
                return &s->funcs[i];
    return 0;
}
