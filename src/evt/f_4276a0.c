/* 0x4276a0 */

EVTVAR *evt_AddLocal(EVTSCRIPT *sc, int type, int count, char *name)
{
    int size = 0;
    EVTVAR *v = 0;
    int i;

    for (i = 0; i < 8; i++) {
        if (!sc->frame->locals[i]) {
            if (type == 1)
                size = 4;
            else if (type == 2)
                size = 1;
            else if (type == 7)
                size = 4;
            else if (type == 6)
                size = 0x60;
            else
                __asm int 3
            v = (EVTVAR *)sc->sp;
            v->count = count;
            v->local = 1;
            v->type = type;
            v->data.p = v + 1;
            strcpy(v->name, name);
            sc->sp += size * count + sizeof(EVTVAR);
            if (sc->sp > sc->stack + 0x400) {
                evt_Error(sc, sc->pc, "Stack Overflow!");
                evt_Stop(sc);
            }
            sc->frame->locals[i] = v;
            break;
        }
    }
    if (!v)
        evt_Error(sc, sc->pc - 1, "Too many locals!");
    return v;
}
