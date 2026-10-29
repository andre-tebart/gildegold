/* 0x4cb100, handler of type 0x3b: removes building data[0] (its jobs, character references, effect) */

int ai_Unk4cb100(G17MSG *m, G17STAT *s)
{
    int save = d3_CurrentScene;
    char *b, *o, *p;
    int e;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    if (b == ai_Unk75ec60)
        sv_Unk52adb0();
    for (o = gm_Query(0, 2, 7, 4, 0x1d); o != 0; o = gm_QueryNext()) {
        if (*(int *)(o + 0x1c) == *(int *)(b + 1))
            gm_Unk5034b0(o);
    }
    for (p = ai_Unk770cec; (int)p < (int)(ai_Unk770cec + 0x300 * 0x21c); p += 0x21c) {
        if (*(char **)p == b)
            *(char **)p = 0;
    }
    d3if_SelectScene(0, 1);
    if (d3_QuadTree != 0)
        d3q_RemoveObject((D3OBJECT *)*(int *)(b + 0x61), (D3QNODE *)d3_QuadTree);
    d3if_Delete((struct D3OBJECT *)*(int *)(b + 0x61));
    if (*(int *)(b + 0x95) != -1) {
        o = (char *)evt_FindById(*(int *)(b + 0x95));
        if (o != 0)
            evt_Stop(o);
        *(int *)(b + 0x95) = -1;
    }
    d3if_SelectScene(save, 1);
    gm_FreeBuilding(b);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
