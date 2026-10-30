/* 0x4b6fd0, message: c pays 1..3 % (not for officeKind 0x0e; with item 0x290 only 0x4b70f0, which does nothing) */

void ai_MsgNoOp(struct AICHAR *c);

int ai_Unk4b6fd0(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0x0e)
        return 0x400;
    if (ai_InventoryHas(c, 0x290)) {
        ai_MsgNoOp(c);
        return 0x400;
    }
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
