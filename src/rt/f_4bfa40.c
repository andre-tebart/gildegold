/* 0x4bfa40, cart c unloads at its current route point (c->unk2d): the goods of the point's unloadSlots slots go from the
   cart into the building's stock (market 0x1c: its stock 0x1db, warehouse 10: 0x500ec0), bought by player pl at
   the building's price when it is not the cart owner's
   @deep readability only, byte now: Q4 (10 raw accesses - AIBLD +0x27/+0x49/+0x5d/+0x65, AIOBJ +0x14, RTINV stock, d2_Objs view need typed fields; rt.h's views and the ai.h AIBLD pads) and Q2 (gm_Query/4ffcc0 stock lists, gm_Unk50a050/50aa00/50ab80/50aaf0 stock and cart accessors, gm_GoodsPrice price, gm_Unk509ee0 money, cm_SendExSellObject transfer) - the stock/cart API needs names first */

void rt_UnloadAtPoint(char *c, int pl)
{
    RTPOINT *q;
    char *b;
    char *t;
    short *o;
    char *st;
    unsigned short *g;
    int *s;
    int k;
    int n;
    int price;
    RTINV ci;
    RTINV bi;

    if (*(RTPOINT **)(c + 0x29) == 0)
        return;
    q = rt_PointAt(*(RTPOINT **)(c + 0x29), *(int *)(c + 0x2d));
    b = gm_FindBuildingById(q->buildingId);
    if (b == 0)
        return;
    t = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (*t != 10 && *t != 0x1c && !gm_BuildingGoodsList(b, &bi, 0))
        return;
    o = gm_Unk50a050(c);
    if (!gm_StallGoodsList(c, &ci))
        return;
    s = q->unloadSlots;
    for (k = 3; k != 0; k--) {
        if (s[0] != 0) {
            if (*t == 10)
                st = gm_FindMarketObject(s[0]);
            else if (*t == 0x1c)
                st = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1db, 6);
            else
                st = bi.stock;
            g = gm_Query(*(int *)(ci.stock + 0x14), 1, 0, s[0]);
            if (g != 0) {
                n = s[1];
                if (n >= gm_ListAmount(ci.stock, g))
                    n = gm_ListAmount(ci.stock, g);
                n = gm_FreeCapacity(st, s[0], n);
                if (n > 0) {
                    price = 0;
                    if (*(short *)(b + 0x27) != *o) {
                        if (*t == 10)
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * 1.1f);
                        else
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * *(float *)(b + 0x49));
                    }
                    if (gm_Unk509ee0(o, pl) >= price * n)
                        cm_SendExSellObjectI(*(int *)(st + 2), *(int *)(ci.stock + 2), s[0], n, pl, price);
                }
            }
        }
        s += 2;
    }
}
