/* 0x4bb330, message if 0x5054f0 of c: c pays 0x5085f0(1..5, ai_OwnPlayer) (not with item 0x290) */

int ai_Unk4bb330(G14MSG *m)
{
    AICHAR *c;
    int v;
    char buf[0x400];

    v = gm_PlayerFactor(util_RandomU(5) + 1, ai_OwnPlayer);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (!gm_Unk5054f0(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
