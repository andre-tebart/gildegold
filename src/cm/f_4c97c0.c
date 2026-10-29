/* 0x4c97c0, handler of type 0x2e */

int ai_Unk4c97c0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p != 0) {
        ch_AddAction(p, 0x2d, G17_I(*m, 4), G17_I(*m, 8), G17_I(*m, 0xc));
        if (s != 0) {
            s->state = 2;
            s->unk01 = 0;
            s->value = 0;
        }
    }
    return 0;
}
