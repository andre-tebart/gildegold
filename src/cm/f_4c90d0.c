/* 0x4c90d0, handler of type 0x24 */

int ai_Unk4c90d0(G17MSG *m, G17STAT *s)
{
    int i;

    i = unk_Unk4f3780(G17_I(*m, 4), G17_I(*m, 0));
    if (i > -1) {
        ai_Unk91b6e0[i][0] = G17_I(*m, 4);
        ai_Unk91b6e0[i][1] = G17_I(*m, 0);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
