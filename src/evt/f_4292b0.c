/* 0x4292b0 */

int evt_LeaveFunction(EVTSCRIPT *sc)
{
    int i, j;

    if (strcmp(sc->frame->func->name, "exit")) {
        for (i = 15; i >= 0; i--)
            if (sc->frames[i].func)
                break;
        if (i >= 0) {
            if (i >= 1) {
                for (j = 0; j < 8; j++)
                    sc->frame->locals[j] = 0;
                sc->sp = sc->frame->sp;
                sc->frame = &sc->frames[i - 1];
                sc->pc = sc->frame->ret;
                sc->frames[i].func = 0;
                sc->frames[i].ret = 0;
                return 1;
            }
            for (j = 0; j < 8; j++)
                sc->frame->locals[j] = 0;
            sc->sp = sc->frame->sp;
            sc->frame = 0;
            sc->flags &= ~1;
            sc->frames[i].func = 0;
            sc->frames[i].ret = 0;
        }
    }
    return 0;
}
