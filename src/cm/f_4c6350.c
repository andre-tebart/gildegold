/* 0x4c6350, check of message 0xf: 1 if the owner of id has less than the amount of item k */

int ai_Unk4c6350(G17MSG *m)
{
    char *x, *y, *z;
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
        if (gm_SumSlotAmounts(*v, m->data[0xc]) - *(int *)&m->data[0xd] < 0)
            return 1;
    }
    return 0;
}
