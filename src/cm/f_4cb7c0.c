/* 0x4cb7c0, handler of type 0x41 */

int ai_Unk4cb7c0(G17MSG *m, G17STAT *s)
{
    ord_LoadStatistics(&m->data[0]);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
