/* 0x4cb940, handler of type 0x47, "cm_ExChangePlayerHe" */

int ai_Unk4cb940(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        err_Message("cm_ExChangePlayerHe(): invalid sp->id");
    if (*(unsigned short *)c != 0xffff)
        sv_MoveToGroup(*(int *)(c + 0x16c), G17_I(*m, 4), *(unsigned short *)c, 0);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
