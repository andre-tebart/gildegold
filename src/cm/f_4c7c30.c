/* 0x4c7c30, handler of type 0x13: moves the object data[8] from the list of data[4] to the list of data[0] */

int ai_Unk4c7c30(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *o, *prev;
    int *p1, *p2;
    int c;

    *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[4]) == 0)
        return 1;
    if (x != 0)
        p1 = (int *)(x + 0x5d);
    else if (y != 0)
        p1 = (int *)(y + 0x14);
    else
        p1 = (int *)(z + 0x178);
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    if (x != 0)
        p2 = (int *)(x + 0x5d);
    else if (y != 0)
        p2 = (int *)(y + 0x14);
    else
        p2 = (int *)(z + 0x178);
    c = ai_Unk4c4450((int)m, *(int *)&m->data[8]);
    *(int *)&m->data[8] = c;
    prev = 0;
    for (o = (char *)*p1; o != 0; o = *(char **)(o + 0x3f)) {
        if (*(int *)(o + 2) == c) {
            if (prev == 0)
                *p1 = *(int *)(o + 0x3f);
            else
                *(int *)(prev + 0x3f) = *(int *)(o + 0x3f);
            break;
        }
        prev = o;
    }
    if (o == 0)
        return 1;
    *(int *)(o + 6) = *(int *)&m->data[0];
    *(int *)(o + 0x3f) = *p2;
    *p2 = (int)o;
    if (s != 0)
        s->state = 1;
    return 0;
}
