/* 0x4cc8e0, handler of type 0x56 */

int ai_Unk4cc8e0(G17MSG *m, G17STAT *s)
{
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ord_HandleMessageRecord(&m->data[0]) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
