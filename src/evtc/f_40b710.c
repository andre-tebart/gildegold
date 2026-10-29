/* 0x40b710, script command Sleep: suspends the script for *t milliseconds (*t / 14 ticks, -1: forever); the
   waitStart tick is taken on the first call, the resumed calls re-arm the wait until it has passed */

int evtc_Wait(int *t)
{
    EVTSCRIPT *sc;

    if (evt_CurCall && evt_CurCall->fn == evtc_Wait) {
        sc = evt_Cur;
        if (*t != -1 && d3_Time >= (unsigned long)(*t / 14 + sc->waitStart))
            return 0;
        sc->waitfn = evtc_Wait;
        return 0;
    }
    evt_Cur->waitfn = evtc_Wait;
    evt_Cur->waitStart = d3_Time;
    return 1;
}
