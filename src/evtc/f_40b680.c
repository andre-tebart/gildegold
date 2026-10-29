/* 0x40b680, the same with one argument */

int evtc_RunScript1(char **name, int *a)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScriptArgs(sc, 1, *a);
    return (int)sc;
}
