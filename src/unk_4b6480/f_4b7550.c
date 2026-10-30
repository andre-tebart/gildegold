/* 0x4b7550, message: v = 1..3 % of 0x50a640, with the 0x4fea50 value of the 0x4a63f0 hit (mask 0x400000, 100.0) in
   the text (not with item 0x290, not without a hit) */

int ai_Unk4b7550(G14MSG *m)
{
    AICHAR *c;
    double k;
    LARGE_INTEGER mask;
    int v, r, o;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    mask.LowPart = 0x400000;
    mask.HighPart = 0;
    if (!ai_Unk4a63f0((AIPER *)c, 6, mask.QuadPart, 0, 100.0f, &o))
        return 0x400;
    if (!AI_4FEA50_4(&r, 0, 0, o))
        return 0x400;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, r, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
