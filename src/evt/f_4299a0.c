/* 0x4299a0 */

int evt_RunScript(EVTSCRIPT *sc)
{
    char buf[512];
    EVTFUNC *f;

    if (!sc)
        return 0;
    if (!evt_Compile(sc)) {
        evt_Unload(sc);
        return 0;
    }
    f = evt_FindFunc(sc, "main");
    if (!f) {
        evt_Error(sc, 0, "evt_RunScript:No entrypoint(main) found in script...");
        return 0;
    }
    evt_EnterFunction(sc, f);
    sc->flags |= 1;
    sc->mode = 0;
    sc->frame = &sc->frames[0];
    sprintf(buf, "Run script: %s", sc->name);
    strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
    evt_Log[evt_LogPos].line = 0;
    if (++evt_LogPos > 30)
        evt_LogPos = 0;
    return 1;
}
