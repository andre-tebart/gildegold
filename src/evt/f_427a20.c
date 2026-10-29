/* 0x427a20 */

EVTSCRIPT *evt_FindInclude(EVTSCRIPT *sc, char *name)
{
    int i;

    for (i = 0; i < 8; i++)
        if (sc->includes[i] && !strcmp(sc->includes[i]->name, name))
            return sc->includes[i];
    return 0;
}
