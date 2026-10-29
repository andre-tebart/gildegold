/* 0x4c63e0, check of message 0x11 (trade): 1 if the buyer lacks room / the seller lacks the goods or the money */

int ai_Unk4c63e0(G17MSG *m)
{
    char *x, *y, *z, *o, *r;
    int *v;

    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_FindById(&x, &y, &z, *(int *)&m->data[4]) == 0)
            return 1;
        if (x != 0)
            v = (int *)(x + 0x5d);
        else if (y != 0)
            v = (int *)(y + 0x14);
        else
            v = (int *)(z + 0x178);
        o = gm_Query(*v, 1, 0, *(unsigned short *)&m->data[8]);
        if (o == 0)
            return 1;
        if (*(int *)(o + 0xe) < *(int *)&m->data[0xf])
            return 1;
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
            return 1;
        if (x != 0) {
            if (*(unsigned short *)(x + 0x27) != 0xffff)
                r = ai_Chars[*(unsigned short *)(x + 0x27)];
            else
                r = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x116 || *(short *)y == 0x1dd) {
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            }
            r = gm_Unk50a050(y);
        } else {
            if (gm_Unk50ad70(z, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                return 1;
            r = z;
        }
        if (*(int *)&m->data[0x13] != 0 && r != 0) {
            if (gm_SumSlotAmounts(*(int *)(r + 0x178), m->data[0xe]) < *(int *)&m->data[0xf] * *(int *)&m->data[0x13])
                return 1;
        }
    }
    return 0;
}
