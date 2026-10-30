/* 0x4bacc0, message 0x38: unless a 0..255 roll is below c's luck (skill 0x82) and unless c holds item 0x290, one
   of up to 6 goods of c's inventory (gm_Query (1, 5), not the goods of type 9) is lost: cm_SendExSellObject with
   -1 as the buyer (cm.c 0x4c70b0 moves the item from data[4] = c to data[0] = -1) and the news text 0x9a8 + 2 *
   the good names it */

int ai_MsgLoseGood(G14MSG *m)
{
    AICHAR *c;
    AIOBJ *e;
    int n;
    void *a[6];
    char buf[0x100];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (util_RandomU(0x100) < (unsigned short)c->sk82)
        return 0x400;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (e = gm_Query(c->inventory, 1, 5); e != 0 && n < 6; e = gm_QueryNext()) {
        if (ai_GoodsT[e->id].type != 9)
            a[n++] = e;
    }
    if (n == 0)
        return 0x400;
    e = a[util_RandomU(n)];
    cm_SendExSellObjectI(-1, c->netId, e->id, 1, ai_OwnPlayer, 0);
    txt_Format(buf, m->type + 0x12e7, e->id * 2 + 0x9a8);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
