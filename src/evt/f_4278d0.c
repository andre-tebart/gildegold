/* 0x4278d0 */

EVTVAR *evt_FindVar(EVTSCRIPT *sc, char *name)
{
    EVTSCRIPT *s = evt_Scope ? evt_Scope : sc;
    EVTFRAME *f = sc->frame;
    int i;

    for (i = 0; f && i < 8; i++)
        if (f->locals[i] && !strcmp(f->locals[i]->name, name))
            return f->locals[i];
    for (i = 0; i < s->nvars; i++)
        if (!strcmp(s->vars[i].name, name))
            return &s->vars[i];
    for (i = 0; i < evt_NGlobals; i++)
        if (!strcmp(evt_Globals[i].name, name))
            return &evt_Globals[i];
    return 0;
}
