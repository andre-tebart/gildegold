/* 0x4b6bf0, message: one of up to 12 buildings of the 0x501190(1, 5, 0x16) list is chosen (0x501190(1, 1, id)),
   c pays 2..6 % */

int ai_MsgPayForBuilding(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, v;
    double k;
    int ids[12];
    char buf[0x400];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 5, 0x16); b != 0 && n < 12; b = gm_BuildNext())
        ids[n++] = b->id;
    if (n == 0)
        return 0x400;
    gm_BuildQuery(1, 1, ids[util_Random(n)]);
    k = util_Random(5) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
