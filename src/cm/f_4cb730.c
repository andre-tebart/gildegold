/* 0x4cb730, handler of type 0x40: the same record gets deltas added (3 ints, 4 floats) */

int ai_Unk4cb730(G17MSG *m, G17STAT *s)
{
    char *r;

    r = gm_FindGoods(m->data[0], *(unsigned short *)&m->data[1]);
    if (r == 0)
        return 1;
    *(int *)(r + 0x10) += G17_I(*m, 3);
    *(int *)(r + 0x14) += G17_I(*m, 7);
    *(int *)(r + 0x18) += G17_I(*m, 0xb);
    *(float *)(r + 0x20) += *(float *)&m->data[0xf];
    *(float *)(r + 0x24) += *(float *)&m->data[0x13];
    *(float *)(r + 0x2c) += *(float *)&m->data[0x17];
    *(float *)(r + 0x38) += *(float *)&m->data[0x1b];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
