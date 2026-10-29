/* 0x40c1d0, the same with an int argument for the script */

void chc_AniScriptIntCB(CHACTION *a)
{
    EVTSCRIPT *sc = evt_Load(a->scriptName);

    if (sc) {
        evt_RunScriptArgs(sc, 1, a->scriptAction.arg);
        sc->group = a->scriptAction.group;
    }
}
