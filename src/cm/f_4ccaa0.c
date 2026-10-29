/* 0x4ccaa0, handler of type 0x5a: adds data[4] to +0x194 of the character data[0] (0..0x32) */

int ai_Unk4ccaa0(G17MSG *m, G17STAT *s)
{
    char *c;
    int v;

    c = gm_FindChar(G17_I(*m, 0));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = G17_I(*m, 4) + *(int *)(c + 0x194);
    v = v < 0 ? 0 : v;
    if (v >= 0x32)
        v = 0x32;
    *(int *)(c + 0x194) = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
