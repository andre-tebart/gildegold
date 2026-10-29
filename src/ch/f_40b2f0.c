/* 0x40b2f0, appends action type to the character's queue; the action's arguments follow */

CHACTION *ch_AddAction(CHARACTER *ch, unsigned int type, ...)
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
    if (type == 0x33) {
        if (a->args[1] != -1 && a->args[2] == -1)
            __asm int 3
        if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
            ch_RemoveAction(a);
            return 0;
        }
    }
    ch_CheckQueue(ch, a);
    if (a->func == (CHACTFUNC)ch_ActAnimOnce)
        a->args[0] = a->type;
    return a;
}
