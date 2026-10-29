/* 0x4c7d80, handler of type 0x14: removes the single item data[4] from workshop data[0] */

int ai_Unk4c7d80(G17MSG *m, G17STAT *s)
{
    char *y, *o;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, &y, 0, *(int *)&m->data[0]) == 0)
        return 1;
    if (*(short *)y != 0x2a && *(short *)y != 0x116)
        return 1;
    o = gm_Query(*(int *)(y + 0x14), 1, 0, *(unsigned short *)&m->data[4]);
    if (o == 0)
        return 1;
    if (*(int *)(o + 0xe) != 1)
        return 1;
    gm_RemoveObject((int *)(y + 0x14), *(unsigned short *)&m->data[4]);
    if (s != 0)
        s->state = 1;
    return 0;
}
