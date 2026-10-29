/* 0x4293b0 */

void evt_EnterFunction(EVTSCRIPT *sc, EVTFUNC *f)
{
    int vals[8];
    char strs[8][0x60];
    EVTVAR *v;
    char *s;
    int i = 0;
    int k;

    if (!f) {
        evt_Error(sc, sc->pc, "evt_EnterFunction: Unknown function...");
        return;
    }
    for (; i < 16; i++)
        if (!sc->frames[i].func)
            break;
    if (i >= 16) {
        evt_Error(sc, sc->pc, "evt_EnterFunction: Callstack overflow...");
        return;
    }
    if (i > 0) {
        sc->pc++;
        for (k = 0; k < f->nargs; k++) {
            s = (char *)evt_Eval(0);
            if (f->argtype[k] == 6)
                strcpy(strs[k], s);
            else
                vals[k] = (int)s;
        }
        if (f->nargs == 0)
            sc->pc++;
        sc->frame->ret = sc->pc;
    } else {
        for (k = 0; k < f->nargs; k++)
            if (f->argtype[k] == 6)
                strcpy(strs[k], evt_StrArgs[k]);
            else
                vals[k] = evt_Args[k];
    }
    sc->frame = &sc->frames[i];
    sc->frames[i].sp = sc->sp;
    sc->frames[i].func = f;
    for (k = 0; k < f->nargs; k++) {
        v = evt_AddLocal(sc, f->argtype[k], 1, f->argname[k]);
        if (!v) {
            evt_Error(sc, sc->pc, "Could not allocate symbol...out of memory!");
            evt_Stop(sc);
            return;
        }
        switch (v->type) {
        case 2:
            *v->data.b = vals[k];
            break;
        case 1:
            *v->data.i = vals[k];
            break;
        case 7:
            *v->data.f = (float)vals[k];
            break;
        case 6:
            strcpy(v->data.p, strs[k]);
            break;
        }
    }
    sc->pc = f->body;
    sc->frame->ret = f->body;
    sc->frame->level = 0;
}
