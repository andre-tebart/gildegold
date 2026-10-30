/* 0x4b6d30, message: c pays 2..4 % */

int ai_MsgPayPct2to4(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
