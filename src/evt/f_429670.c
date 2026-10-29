/* 0x429670 */

int evt_Compile(EVTSCRIPT *sc)
{
    EVTTOKEN t;
    int depth = 0;
    EVTFUNC *funcs;
    EVTVAR *vars;
    EVTSCRIPT *old;

    funcs = mem_Alloc(0x5300, "evt_t:funcs");
    vars = mem_Alloc(0x1800, "evt_t:vars");
    funcs->nameWord = 0;
    vars->typeWord = 0;
    sc->vars = mem_Alloc(0x1800, "evt_t:vars");
    sc->funcs = mem_Alloc(0x5300, "evt_t:funcs");
    old = evt_Cur;
    evt_Cur = sc;
    sc->pc = sc->source;
    do {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_TYPE && depth == 0) {
            if (!evt_Declare(sc, t.u.i)) {
                mem_Free(funcs);
                mem_Free(vars);
                mem_Free(sc->vars);
                sc->vars = 0;
                mem_Free(sc->funcs);
                sc->funcs = 0;
                return 0;
            }
        } else {
            if (t.type == EVT_OP) {
                if (t.u.i == EVT_OP_LBRACE)
                    depth++;
                else if (t.u.i == EVT_OP_RBRACE)
                    depth--;
            }
            if (depth < 0) {
                evt_Error(sc, sc->pc, "Syntax Error: too many '}'...");
                mem_Free(funcs);
                mem_Free(vars);
                mem_Free(sc->vars);
                sc->vars = 0;
                mem_Free(sc->funcs);
                sc->funcs = 0;
                return 0;
            }
            if (t.type == EVT_KEYWORD && t.u.i == 5)
                evt_Include();
        }
    } while (t.type != EVT_END);
    if (depth != 0) {
        evt_Error(sc, 0, "Syntax Error: Wrong braces-count...");
        mem_Free(funcs);
        mem_Free(vars);
        mem_Free(sc->vars);
        sc->vars = 0;
        mem_Free(sc->funcs);
        sc->funcs = 0;
        return 0;
    }
    if (sc->nvars != 0) {
        memcpy(vars, sc->vars, sc->nvars * sizeof(EVTVAR));
        mem_Free(sc->vars);
        sc->vars = 0;
        sc->vars = mem_Alloc(sc->nvars * sizeof(EVTVAR), "evt:sc->Vars");
        memcpy(sc->vars, vars, sc->nvars * sizeof(EVTVAR));
    } else {
        mem_Free(sc->vars);
        sc->vars = 0;
    }
    if (sc->nfuncs != 0) {
        memcpy(funcs, sc->funcs, sc->nfuncs * sizeof(EVTFUNC));
        mem_Free(sc->funcs);
        sc->funcs = 0;
        sc->funcs = mem_Alloc(sc->nfuncs * sizeof(EVTFUNC), "evt:sc->Functions");
        memcpy(sc->funcs, funcs, sc->nfuncs * sizeof(EVTFUNC));
    } else {
        mem_Free(sc->funcs);
        sc->funcs = 0;
    }
    evt_Cur = old;
    mem_Free(funcs);
    mem_Free(vars);
    return 1;
}
