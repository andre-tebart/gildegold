/* 0x4b6570, message 1: as 0x4b6480 (2..4 %), not for c->officeKind == 0x0e */

int ai_MsgRewardPct2to4(G14MSG *m)
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
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
