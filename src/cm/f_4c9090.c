/* 0x4c9090, handler of type 0x23 */

int ai_Unk4c9090(G17MSG *m, G17STAT *s)
{
    switch (unk_Unk4f37e0(G17_I(*m, 0), G17_I(*m, 4))) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
