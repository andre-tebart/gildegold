/* 0x4ac200, a price for character p: 500 * its worth / the richest character's worth, clamped to 1600..16000;
   0 if p's goods value does not exceed it */

int hmt_GetPrice(AICHAR *p)
{
    short id;
    int total;
    int goods;
    int v;

    gm_FindRichest(&id, &total);
    goods = gm_GetGoodsValue(p);
    v = (int)(min(16000.0f, max(1600.0f, (float)gm_GetWorth(p->id) * 500.0f / total)));
    if (goods > v)
        return v;
    return 0;
}
