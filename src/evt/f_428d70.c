/* 0x428d70 */

int evt_DefineFunc(EVTSCRIPT *sc, int unk)
{
    EVTTOKEN t;
    EVTFUNC *f = &sc->funcs[sc->nfuncs];

    f->nargs = 0;
    memset(&t, 0, sizeof(t));
    do {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_TYPE && ((t.u.i >= 1 && t.u.i <= 2) || t.u.i == 6)) {
            f->argtype[f->nargs] = t.u.i;
            evt_GetToken(evt_Cur->pc, &t);
            if (t.type != EVT_IDENT) {
                if (t.type == EVT_VAR) {
                    evt_Error(sc, sc->pc, "Dublicate symbolname!");
                    evt_Stop(sc);
                } else {
                    evt_Error(sc, sc->pc, "Missing operand!");
                    evt_Stop(sc);
                }
                return 0;
            }
            strcpy(f->argname[f->nargs], t.u.s);
            f->nargs++;
        }
        evt_GetToken(evt_Cur->pc, &t);
    } while (t.type != EVT_OP || t.u.i != EVT_OP_RPAREN);
    sc->nfuncs++;
    return unk;
}
