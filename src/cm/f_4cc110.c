/* 0x4cc110, handler of type 0x4f: two bytes of the table +0xf4 of the action record (type 0x3a) of person data[0] */

int ai_Unk4cc110(G17MSG *m, G17STAT *s)
{
    char *p, *a, *q;
    int idx, i;

    p = ch_FindByName((char *)&m->data[0]);
    if (p == 0)
        return 1;
    a = *(char **)(p + 0x128);
    if (a == 0 || a[9] != 0x3a)
        return 2;
    if (*(int *)(a + 0xf4) != 0) {
        *(int *)(a + 0xf0) = G17_I(*m, 0x1c);
        idx = G17_I(*m, 0x18);
        q = (char *)&m->data[0x21];
        for (i = 0; i < 2 && idx < *(int *)(a + 0xf0); i++) {
            (*(char **)(a + 0xf4))[idx * 2] = q[-1];
            (*(char **)(a + 0xf4))[idx * 2 + 1] = q[0];
            q += 2;
            idx++;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
