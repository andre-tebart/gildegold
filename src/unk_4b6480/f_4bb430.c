/* 0x4bb430, message if 0x5056a0 of c (not with item 0x290) */

int ai_Unk4bb430(G14MSG *m)
{
    AICHAR *c;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (!gm_Unk5056a0(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
