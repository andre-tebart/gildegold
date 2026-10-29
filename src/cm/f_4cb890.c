/* 0x4cb890, handler of type 0x44 (the result of amt_AcceptApplication is also the return value, inverted) */

int ai_Unk4cb890(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_AcceptApplication(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
