/* 0x4c8110, handler of type 0x19: field data[4] (size data[8]) of object data[0]: clears mask data[0x10], sets data[0xc] */

int ai_Unk4c8110(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *a;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    a = x;
    if (a == 0) {
        a = y;
        if (a == 0)
            a = z;
    }
    a += *(int *)&m->data[4];
    switch (*(int *)&m->data[8]) {
    case 1:
        *(unsigned char *)a &= ~*(unsigned char *)&m->data[0x10];
        *(unsigned char *)a |= *(unsigned char *)&m->data[0xc];
        break;
    case 2:
        *(unsigned short *)a &= ~*(unsigned short *)&m->data[0x10];
        *(unsigned short *)a |= *(unsigned short *)&m->data[0xc];
        break;
    case 4:
        *(int *)a &= ~*(int *)&m->data[0x10];
        *(int *)a |= *(int *)&m->data[0xc];
        break;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
