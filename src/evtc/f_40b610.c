/* 0x40b610, the same with two arguments */

int evtc_RunScript2(char **name, int *a, int *b)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScriptArgs(sc, 2, *a, *b);
    return (int)sc;
}
