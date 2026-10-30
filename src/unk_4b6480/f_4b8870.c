/* 0x4b8870, message: c (money at least 2..4) loses that much 0x50cc50 (not with item 0x290) */

int ai_Unk4b8870(G14MSG *m)
{
    AICHAR *c;
    int s;
    char buf[0x200];

    s = util_RandomU(3) + 2;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (c->money < s)
        return 0x400;
    txt_Format(buf, m->type + 0x12e7, s);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    gm_SendMoneyDelta(c->netId, -s);
    return 0;
}
