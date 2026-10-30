/* 0x4b7f40, message: as 0x4b78c0, negative (0..100, -6..-14; not with item 0x290) */

int ai_Unk4b7f40(G14MSG *m)
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
    n = util_Random(5) * 2 + 6;
    cm_SendRelations(c->netId, p->netId, -n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
