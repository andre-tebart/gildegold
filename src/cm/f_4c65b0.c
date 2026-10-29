/* 0x4c65b0, check of message 0x16 (field deltas): 1 if a field is the id (+4) or a character reference (+0x5c) that
   does not belong to the object */

int ai_Unk4c65b0(G17MSG *m)
{
    char *x, *y, *z, *base, *a, *c;
    unsigned char *p;
    unsigned int i, j, n;
    int size;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
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
        a = base + *(unsigned short *)(p + 2);
        p += 4;
        if (z != 0) {
            if (a == z + 4)
                return 1;
            if (a == z + 0x5c) {
                c = gm_FindChar(*(int *)p + *(int *)(z + 0x5c));
                if (c == 0)
                    return 1;
                if (*(int *)(c + 0x5c) != -1 && *(int *)(c + 0x5c) != *(int *)(z + 4))
                    return 1;
            }
        }
        switch (size) {
        case 1:
            for (j = 0; j < n; j++)
                p += 1;
            break;
        case 2:
            for (j = 0; j < n; j++)
                p += 2;
            break;
        case 4:
            for (j = 0; j < n; j++)
                p += 4;
            break;
        }
    }
    return 0;
}
