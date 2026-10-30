/* 0x4b76b0, message: v = 1..2 % of 0x50a640 (not with item 0x290; types 0x12 and 0x13) */

int ai_Unk4b76b0(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(2) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
