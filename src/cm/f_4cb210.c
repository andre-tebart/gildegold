/* 0x4cb210, handler of type 0x3c */

int ai_Unk4cb210(G17MSG *m, G17STAT *s)
{
    char r = 2;

    if (unk_Unk4f3910(G17_I(*m, 0), G17_I(*m, 4), G17_I(*m, 8), G17_I(*m, 0xc)) > 0)
        r = 1;
    if (s != 0) {
        s->state = r;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
