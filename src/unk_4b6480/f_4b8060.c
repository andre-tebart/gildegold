/* 0x4b8060, message: as 0x4b78c0, negative (0..100, -10..-38; not with item 0x290) */

int ai_Unk4b8060(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    fl = (unsigned)&full_data_600c00;
    if (!ai_Unk4a5b50((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    if (p == 0)
        return 0x400;
    n = util_Random(15) * 2 + 10;
    cm_SendRelations(c->netId, p->netId, -n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
