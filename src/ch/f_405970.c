/* 0x405970 */

CHACTION *ch_SitDown(CHARACTER *ch, char *name)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (!a)
        return 0;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActSitDown;
    a->type = 0x2f;
    a->next = 0;
    a->unk10 = 0;
    strcpy(a->u.name, name);
    return a;
}
