/* 0x42abc0 */

int evt_ResumeCall(EVTSCRIPT *sc)
{
    EVTBUILTIN *f;
    int r;
    int i;

    f = evt_CurCall = sc->call;
    sc->call = 0;
    evt_Cur->waitfn = 0;
    switch (f->nargs) {
    case 0:
        r = f->fn0();
        break;
    case 1:
        r = f->fn1(&sc->args[0]);
        break;
    case 2:
        r = f->fn2(&sc->args[0], &sc->args[1]);
        break;
    case 3:
        r = f->fn3(&sc->args[0], &sc->args[1], &sc->args[2]);
        break;
    case 4:
        r = f->fn4(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3]);
        break;
    case 5:
        r = f->fn5(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4]);
        break;
    case 6:
        r = f->fn6(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4], &sc->args[5]);
        break;
    case 7:
        r = f->fn7(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4], &sc->args[5],
                   &sc->args[6]);
        break;
    }
    if (!sc->waitfn) {
        sc->call = 0;
        evt_CurCall = 0;
        for (i = 0; i < f->nargs; i++) {
            if (f->argtype[i] == 6)
                free((void *)sc->args[i]);
            sc->args[i] = 0;
        }
    } else {
        sc->call = f;
    }
    evt_CurCall = 0;
    return r;
}
