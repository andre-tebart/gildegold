/* 0x4c9d00, handler of type 0x33 */

int ai_Unk4c9d00(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    ch_AddAction(p, 0x38, G17_I(*m, 4));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
