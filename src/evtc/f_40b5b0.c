/* 0x40b5b0, starts a script in the caller's context */

int evtc_RunScript(char **name)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScript(sc);
    return (int)sc;
}
