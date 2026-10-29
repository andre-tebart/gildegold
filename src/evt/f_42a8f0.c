/* 0x42a8f0 */

int evt_Statement(EVTSCRIPT *sc)
{
    EVTTOKEN t;
    char buf2[256];
    char buf[256];
    EVTFRAME *f;
    char *p;
    int l;

    evt_Parens = 0;
    do {
        evt_GetToken(sc->pc, &t);
        if (t.type == EVT_OP) {
            evt_Scope = 0;
            if (t.u.i == EVT_OP_RBRACE) {
                l = sc->frame->level;
                if (l > 0) {
                    sc->frame->level = l - 1;
                    f = sc->frame;
                    p = f->loops[f->level].start;
                    if (p && f->loops[f->level].type == 1)
                        sc->pc = p;
                } else if (!evt_LeaveFunction(sc)) {
                    evt_Stop(sc);
                }
                return 0;
            }
        } else if (t.type == EVT_KEYWORD) {
            evt_Scope = 0;
            switch (t.u.i) {
            case 1:
                evt_While();
                break;
            case 2:
                evt_If();
                break;
            case 4:
                evt_Else();
                break;
            case 5:
                evt_Include();
                break;
            case 6:
                evt_Mode1();
                break;
            case 8:
                evt_Mode2();
                break;
            case 7:
                evt_Mode0();
                break;
            case 3:
                evt_Return();
                break;
            default:
                sprintf(buf, "Keyword is not surported!");
                evt_Error(sc, sc->pc, buf);
                evt_Stop(sc);
                return 0;
            }
        } else if (t.type == EVT_BUILTIN) {
            evt_CallBuiltin(t.u.p);
        } else if (t.type == EVT_VAR) {
            evt_Assign(t.u.p);
        } else if (t.type == EVT_FUNC) {
            evt_EnterFunction(sc, t.u.p);
        } else if (t.type == EVT_TYPE) {
            evt_DeclareLocal(sc, t.u.i);
        } else if (t.type == EVT_SCRIPT) {
            evt_Scope = t.u.p;
            sc->pc++;
        } else if (t.type != EVT_END) {
            sprintf(buf2, "Unknown symbol : '%s'", t.u.s);
            evt_Error(sc, sc->pc, buf2);
            evt_Stop(sc);
        }
    } while ((sc->flags & 1) && t.type != EVT_END && (t.type != EVT_OP || t.u.i != EVT_OP_SEMI));
    if (t.type == EVT_OP && t.u.i == EVT_OP_SEMI) {
        f = sc->frame;
        l = f->level;
        if (l > 1 && f->loops[l - 1].start && f->loops[l - 1].braced) {
            f->level = l - 1;
            f = sc->frame;
            if (f->loops[f->level].type == 1)
                sc->pc = f->loops[f->level].start;
        } else {
            return 0;
        }
    }
    if (t.type == EVT_END)
        sc->flags &= ~1;
    return 0;
}
