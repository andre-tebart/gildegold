/* 0x4ca8a0, handler of type 0x36: the person "sp_<data[0]>" uses the gates of building data[4] (the room data[8]) */

int ai_Unk4ca8a0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    char gate2[0x80];
    char *p, *b, *q, *r;
    int k;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    b = gm_BuildQuery(1, 1, G17_I(*m, 4));
    q = gm_Query(*(int *)(b + 0x5d), 1, 1, G17_I(*m, 8));
    if (*(int *)(p + 0x30) != -1)
        r = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(p + 0x30));
    else
        r = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
    if (*(unsigned short *)q == 0xfd) {
        k = init_BuildingSceneUniverse(b);
        sprintf(gate2, "dummy_gate_ob_AUSSEN_RAUM");
    } else {
        k = init_ObjectSceneUniverse(q);
        sprintf(gate2, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
    }
    sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)r * 0x41 + 1));
    if (k >= 0)
        ch_UseGate(p, gate, gate2, k, *(int *)(b + 1), *(int *)(q + 2));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
