/* 0x4c81d0, handler of type 0x1a: adds the float data[8] to the field data[4] of object data[0] */

int ai_Unk4c81d0(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *a;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    a = z;
    if (a == 0) {
        a = y;
        if (a == 0)
            a = x;
    }
    *(float *)(a + *(int *)&m->data[4]) += *(float *)&m->data[8];
    if (s != 0)
        s->state = 1;
    return 0;
}
