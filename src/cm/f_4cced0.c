/* 0x4cced0, handler of type 0x61 */

int ai_Unk4cced0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = unk_Unk4f0ed0(&m->data[0]);
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
