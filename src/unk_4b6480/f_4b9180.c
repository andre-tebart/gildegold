/* 0x4b9180, message: c pays 1..2 % of 0x50a640; then as 0x4b8f70, but the text names the player of the second end
   (not with item 0x290) */

int ai_Unk4b9180(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, v;
    float k;
    AIJOB *js[8];
    char *os[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    k = (util_Random(2) + 1.0f) * 0.01f;
    v = (int)(gm_GetWorth(c->id) * k);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    n = util_Random(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        if (*(char *)w == 0x47)
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0]);
        else
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)u + 0x65)], v);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
