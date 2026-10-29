/* 0x4277c0 @deep B: hunks code 93, fill 8, slots 4. Read every hunk in asmdiff: original keeps count=1 in ebx and the error return 0 in edi (mov ebx,1; xor edi,edi; mov eax,edi), ours uses edi for count and xor eax,eax; original merges both error exits into one shared evt_Error/evt_Stop tail (add esp,0xc before the pushes). Every constant (2, 0x1b) and every statement is present - only allocation and block layout differ. Nested-if / goto-tail / message-variable variants recorded in notes/units/evt.md did not change the layout (best 206 B). Same operations spelled differently. */

int evt_DeclareLocal(EVTSCRIPT *sc, int type)
{
    EVTTOKEN t;
    char name[32];
    int count = 1;

    evt_GetToken(sc->pc, &t);
    if (t.type != EVT_IDENT) {
        if (t.type == EVT_VAR) {
            if (((EVTVAR *)t.u.p)->local) {
                evt_Error(sc, sc->pc, "Syntax error: Propably duplicate symbol...");
                evt_Stop(sc);
                return 0;
            }
        } else {
            evt_Error(sc, sc->pc, "Syntax error: unknown");
            evt_Stop(sc);
            return 0;
        }
    }
    if (t.type == EVT_IDENT)
        strcpy(name, t.u.s);
    else
        strcpy(name, ((EVTVAR *)t.u.p)->name);
    evt_GetToken(sc->pc, &t);
    if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACKET) {
        evt_GetToken(sc->pc, &t);
        if (t.type == EVT_INT)
            count = t.u.i;
    }
    return (int)evt_AddLocal(sc, type, count, name);
}
