/* 0x4bb150, message for c with field 0x2c set: c pays 1..4 % of 0x50a640 */

int ai_Unk4bb150(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (*(int *)((char *)c + 0x2c) == 0)
        return 0x400;
    k = util_Random(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
