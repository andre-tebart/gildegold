/* 0x429ab0 */

int evt_RunScriptArgs(EVTSCRIPT *sc, int n, ...)
{
    EVTFUNC *f;
    int i;

    if (!sc)
        return 0;
    if (!evt_Compile(sc))
        return 0;
    f = evt_FindFunc(sc, "main");
    if (!f) {
        evt_Error(sc, 0, "evt_RunScript:No entrypoint(main) found in script...");
        return 0;
    }
    if (n != f->nargs) {
        evt_Error(sc, 0, "evt_RunScript:Wrong parameter count...");
        evt_Stop(sc);
        return 0;
    }
    {
        va_list ap;
        va_start(ap, n);
        for (i = 0; i < f->nargs; i++) {
            if (f->argtype[i] == 6)
                strcpy(evt_StrArgs[i], va_arg(ap, char *));
            else if (f->argtype[i] == 1)
                evt_Args[i] = va_arg(ap, int);
            else if (f->argtype[i] == 2)
                evt_Args[i] = va_arg(ap, char);
        }
    }
    evt_EnterFunction(sc, f);
    sc->flags |= 1;
    sc->frame = &sc->frames[0];
    sprintf(evt_Log[evt_LogPos].text, "Run script: %s", sc->name);
    evt_Log[evt_LogPos].line = 0;
    if (++evt_LogPos > 30)
        evt_LogPos = 0;
    return 1;
}
