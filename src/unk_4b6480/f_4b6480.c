/* 0x4b6480, message 0: c gets a reward of 1..3 % of 0x50a640 */

int ai_MsgRewardPct1to3(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_Random(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100 ? 0x100 < strlen(buf) : 0, 0x603, 0);
    return 0;
}
