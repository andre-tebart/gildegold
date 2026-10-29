/* 0x40b400, like ch_AddAction, with func2 called at the animation frame "frame"; no queue check */

CHACTION *ch_AddActionCB(CHARACTER *ch, CHACTFUNC func2, unsigned char frame, unsigned int type, ...)
{
    CHACTION *a;
    int i;
    va_list args;

    if (!ch)
        return 0;
    if (!ch_ActDefs[type].func)
        type = 0;
    a = ch_NewAction(ch);
    if (!a)
        return 0;
    a->func = ch_ActDefs[type].func;
    a->type = type;
    a->ch = ch;
    a->count = 0;
    va_start(args, type);
    for (i = 0; i < ch_ActDefs[a->type].nargs; i++)
        a->args[i] = va_arg(args, int);
    a->next = 0;
    a->unk10 = 0;
    a->func2 = func2;
    a->unk08 = frame;
    if (a->func == (CHACTFUNC)ch_ActAnimOnce)
        a->args[0] = a->type;
    return a;
}
