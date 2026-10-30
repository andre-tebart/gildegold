/* 0x4b6ef0, message 11: c pays (2..4) / 120 */

int ai_MsgPayPctOver120(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.008333333333333333);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
