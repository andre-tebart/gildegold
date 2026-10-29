/* 0x405ad0 */

int ch_StandUp(CHARACTER *ch, char *name)
{
    CHACTION *a;

    if (!(ch->flags.all & 0x10))
        return 0;
    a = ch_Unk40b270(ch);
    if (!a)
        return 0;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActStandUp;
    a->type = 0x30;
    a->next = 0;
    a->unk10 = 0;
    strcpy(a->u.name, name);
    return 1;
}
