/* 0x4cb910, handler of type 0x46 */

int ai_Unk4cb910(G17MSG *m, G17STAT *s)
{
    int r;

    r = unk_Unk4f27d0(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->value = 0;
        s->state = (r == 0) + 1;
    }
    return 0;
}
