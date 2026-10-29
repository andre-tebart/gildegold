/* 0x4bae20, message (if 0..255 is not above c's skill 0x82): c gets 1..3 % of 0x50a640 */

int ai_Unk4bae20(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x800];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (util_Random(0x100) > (unsigned short)c->sk82)
        return 0x400;
    k = util_Random(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    return 0;
}
