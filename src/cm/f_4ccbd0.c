/* 0x4ccbd0, handler of type 0x5c */

int ai_Unk4ccbd0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = amt_ExecuteSwapMessage(&m->data[0]);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r == 0)
        return 1;
    return 0;
}
