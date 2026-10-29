/* 0x4c6ec0, handler of type 0xe: removes the object id */

int ai_Unk4c6ec0(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z;

    *(unsigned short *)&m->data[0] = (unsigned short)ai_Unk4c4450((int)m, *(unsigned short *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(unsigned short *)&m->data[0]) == 0)
        return 1;
    if (x != 0) {
        if (gm_FreeBuilding(x) != 0)
            return 1;
    } else {
        if (y != 0)
            return 1;
        unk_Unk503c60(*(unsigned short *)z, 1);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
