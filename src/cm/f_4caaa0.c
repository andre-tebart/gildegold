/* 0x4caaa0, handler of type 0x38 */

int ai_Unk4caaa0(G17MSG *m, G17STAT *s)
{
    char *b, *c1, *c2;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c1 = gm_FindChar(G17_I(*m, 8));
    if (c1 == 0 && G17_I(*m, 8) != -1)
        return 1;
    c2 = gm_FindChar(G17_I(*m, 4));
    if (c2 == 0 && G17_I(*m, 4) != -1)
        return 2;
    gm_AssignBuilding(b, *(unsigned short *)c2, *(unsigned short *)c1);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
