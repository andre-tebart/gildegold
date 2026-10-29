/* 0x4cb8d0, handler of type 0x45 */

int ai_Unk4cb8d0(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_ExecuteOfficeMessage(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
