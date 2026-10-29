/* 0x4c7e90, handler of type 0x16: adds the field block (deltas) to the object data[0] */

int ai_Unk4c7e90(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *base;
    unsigned char *p;
    unsigned int i, n;
    int size, id;

    id = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, id) == 0)
        return 1;
    *(int *)&m->data[0] = id;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        size = (char)p[0];
        n = (char)p[1];
        if (size == 1) {
            unsigned char *d = (unsigned char *)base + *(unsigned short *)(p + 2);
            p += 4;
            for (; n != 0; n--)
                *d++ += *p++;
        } else if (size == 2) {
            short *d = (short *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ += *(short *)p;
                p += 2;
            }
        } else if (size == 4) {
            int *d = (int *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ += *(int *)p;
                p += 4;
            }
        } else {
            p += 4;
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
