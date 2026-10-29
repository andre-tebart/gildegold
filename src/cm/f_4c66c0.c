/* 0x4c66c0, check of message 0x19: 1 if it sets flag 0x800000 of field 0x1c8 of a character that has it */

int ai_Unk4c66c0(G17MSG *m)
{
    char *x, *y, *z, *base;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    base = x;
    if (base == 0) {
        base = y;
        if (base == 0)
            base = z;
    }
    if (z != 0 && base + *(int *)&m->data[4] == z + 0x1c8 && (*(int *)&m->data[0xc] & ((unsigned)&ai_Relations + 0x1aca0)) && (*(int *)(z + 0x1c8) & ((unsigned)&ai_Relations + 0x1aca0)))
        return 1;
    return 0;
}
