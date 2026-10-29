/* 0x4cd1e0, handler of type 0x67 */

int ai_Unk4cd1e0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = unk_Unk4eee00(&m->data[0], s);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
