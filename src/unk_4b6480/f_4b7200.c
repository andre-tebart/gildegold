/* 0x4b7200, message: v = 1..2 % (not with item 0x290, nothing if v is above 0x50a640) */

int ai_Unk4b7200(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_Random(2) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    if (v > gm_GetWorth(c->id))
        return 0;
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
