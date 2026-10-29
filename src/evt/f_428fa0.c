/* 0x428fa0 */

int evt_Declare(EVTSCRIPT *sc, int type)
{
    char name[32];
    EVTTOKEN t;
    EVTVAR *v;
    char *pos;
    int n;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_IDENT) {
        strcpy(name, t.u.s);
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_OP) {
            if (t.u.i == EVT_OP_SEMI) {
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, 1);
                    strcpy(v->name, name);
                    return 1;
                }
            } else if (t.u.i == EVT_OP_ASSIGN) {
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, 1);
                    strcpy(v->name, name);
                } else
                    __asm int 3
                evt_GetToken(evt_Cur->pc, &t);
                if (t.type == EVT_INT) {
                    *v->data.i = t.u.i;
                    return 1;
                }
            } else if (t.u.i == EVT_OP_LPAREN) {
                pos = evt_Cur->pc;
                if (!evt_DefineFunc(sc, (int)&t.u))
                    return 0;
                sc->funcs[sc->nfuncs - 1].body = strchr(pos, '{');
                strcpy(sc->funcs[sc->nfuncs - 1].name, name);
                evt_Cur->pc = pos;
                return 1;
            } else if (t.u.i == EVT_OP_LBRACKET) {
                evt_GetToken(evt_Cur->pc, &t);
                if (t.type == EVT_INT) {
                    n = t.u.i;
                } else {
                    n = 1;
                    evt_Error(sc, sc->pc, "Illegal array size...");
                }
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, n);
                    strcpy(v->name, name);
                }
                evt_GetToken(evt_Cur->pc, &t);
                if (t.u.i != EVT_OP_RBRACKET)
                    evt_Error(sc, sc->pc, "Missing ']'!");
                evt_GetToken(evt_Cur->pc, &t);
                if (t.u.i != EVT_OP_SEMI)
                    evt_Error(sc, sc->pc, "Expecting ';'");
            }
        }
    }
    return 1;
}
