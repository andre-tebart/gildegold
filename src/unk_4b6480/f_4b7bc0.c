/* 0x4b7bc0, message: the holders of up to 16 buildings of the 0x501190(1, 5, 7) list (each once) get 0x4c4b00
   6..12 from c; v = 2..5 % of 0x50a640 (not with item 0x290) */

int ai_Unk4b7bc0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, i, j, r, v;
    double k;
    unsigned int ids[16];
    char buf[0x400];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 5, 7); b != 0 && n < 16; b = gm_BuildNext()) {
        if (*(unsigned short *)((char *)b + 0x27) != 0xffff)
            ids[n++] = ai_CharsT[*(unsigned short *)((char *)b + 0x27)].netId;
    }
    if (n == 0)
        return 0x400;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ids[i] == ids[j])
                ids[j] = -1;
        }
    }
    for (i = 0; i < n; i++) {
        if (ids[i] != -1) {
            r = util_RandomU(4) * 2 + 6;
            cm_SendRelations(c->netId, ids[i], r, 1);
        }
    }
    k = util_RandomU(4) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
