/* 0x4cd2f0, handler of type 0x69: frees the list +0x29 of the object data[0] */

int ai_Unk4cd2f0(G17MSG *m, G17STAT *s)
{
    char *q;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = gm_FindObjectInst(G17_I(*m, 0));
    if (q == 0)
        return -1;
    if (*(int *)(q + 0x29) != 0) {
        rt_FreePoints(*(int *)(q + 0x29));
        *(int *)(q + 0x29) = 0;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
