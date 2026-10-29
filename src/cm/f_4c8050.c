/* 0x4c8050, handler of type 0x18: adds the (k, value) pairs of the second field block to the floats at 0x90 + 12 k
   of character data[0], each kept in 0..1000 */

int ai_Unk4c8050(G17MSG *m, G17STAT *s)
{
    char *z;
    unsigned char *p;
    unsigned int i;
    float *f;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, 0, &z, *(int *)&m->data[0]) == 0)
        return 1;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        f = (float *)(z + 0x90 + p[0] * 12);
        *f += *(float *)(p + 1);
        p += 5;
        *f = min(max(*f, 0.0), 1000.0);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
