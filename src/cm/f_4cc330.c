/* 0x4cc330, handler of type 0x52 */

int ai_Unk4cc330(G17MSG *m, G17STAT *s)
{
    char *r;

    r = ai_Unk4ce130(G17_I(*m, 0));
    if (r != 0) {
        ai_Unk4cedc0(r, G17_I(*m, 4), 0);
        *(int *)(r + 0x24) -= G17_I(*m, 4);
        r[8] = m->data[8];
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
