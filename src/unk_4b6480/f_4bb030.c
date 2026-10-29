/* 0x4bb030, message: c loses 6..17 of one of the skills 0x80..0x84 (not with item 0x290) */

int ai_Unk4bb030(G14MSG *m)
{
    AICHAR *c;
    int s, d;
    char buf[0x100];

    s = util_Random(5);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    d = ((unsigned char *)c)[0x80 + s];
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (d <= 0)
        return 0x400;
    ai_Unk4c6020(c->netId, s, -(char)min(d, util_Random(0xc) + 6));
    txt_Format(buf, m->type + 0x12e7, s + 0x16b7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
