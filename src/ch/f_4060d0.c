/* 0x4060d0 */

int ch_DropObject(CHARACTER *ch, char *name, char *obj, int frame)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (!a)
        return 0;
    a->ch = ch;
    a->args[0] = frame;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActDroppingObject;
    a->type = 0x32;
    a->next = 0;
    a->unk10 = 0;
    a->args[1] = 0;
    a->args[2] = 2;
    if (name)
        strcpy(a->u.name, name);
    else
        a->u.name[0] = 0;
    if (obj)
        strcpy(CH_ACTOBJ(a)->name2, obj);
    else
        CH_ACTOBJ(a)->name2[0] = 0;
    return 1;
}
