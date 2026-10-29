/* 0x4cd180, handler of type 0x66: 'nCTa' / 'nCTd' */

int ai_Unk4cd180(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x6154436e)
        r = gv_ReserveEffect(G17_I(*m, 4));
    else if (G17_I(*m, 0) == 0x6454436e)
        r = unk_Unk4eeb00(G17_I(*m, 4));
    else
        return -1;
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
