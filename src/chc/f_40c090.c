/* 0x40c090, action callback: starts the script named in the action */

void chc_AniScriptCB(CHACTION *a)
{
    EVTSCRIPT *sc = evt_Load(a->scriptName);

    if (sc) {
        evt_RunScript(sc);
        sc->group = a->scriptAction.group;
    }
}
