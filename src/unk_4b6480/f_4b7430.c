/* 0x4b7430, message: v = 1..4 % (only with officeKind or secondOffice, not with item 0x290) */

int ai_Unk4b7430(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0 && c->secondOffice == 0)
        return 0x400;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_Random(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
