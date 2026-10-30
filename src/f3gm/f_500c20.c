/* 0x500c20, collects the offers for the goods id from all buildings (the first 0x58 of the table; each entry of 6 ints at 0x7e4b20: building, market object, goods object, the supply (0x50aaf0 minus a quarter of 0x50ab50 * the object's int +0x1c, not below 0), the price float, flag) and sorts them by price, highest first (the price = 0x5089c0 (goods, 0x6f1bc5) * the float at +0x49 of the building). Count in 0x7e3f3c. Returns 0, 1 without the tables. ROUGH */

int gm_CollectOffers(int goods)
{
    int n, k, i, j, cnt;
    char *b;
    int *e;
    int mk, s, o, a, q, cc;
    unsigned v;
    int f;
    unsigned id;
    int tmp[6];

    if (ai_BTypes == 0)
        return 1;
    n = 0;
    gm_NOffers = 0;
    b = ai_Blds;
    for (k = 0x58; k != 0; k--, b += 0xa9) {
        if (*b != 0) {
            f = 0;
            e = ((int *)gm_Offers) + n * 6;
            e[0] = 0;
            e[2] = 0;
            e[5] = 0;
            if (b == (char *)ai_MarketBuildingI) {
                mk = gm_FindMarketObject(goods);
                n = gm_NOffers;
                if (mk != 0 && (o = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, goods)) != 0) {
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[1] = mk;
                    e[2] = o;
                    id = *(unsigned short *)o;
                    goto add;
                }
            } else {
                s = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xff);
                n = gm_NOffers;
                if (s != 0 && (o = (int)gm_Query(*(int *)(s + 0x14), 1, 0, goods)) != 0) {
                    f = 1;
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[5] = 1;
                }
                mk = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x2a);
                n = gm_NOffers;
                if (mk != 0 && (o = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, goods)) != 0) {
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[1] = mk;
                    e[2] = o;
                    id = *(unsigned short *)o;
                add:
                    a = gm_ListStock(mk, id);
                    q = *(int *)(o + 0x1c);
                    cc = gm_ListAmount(mk, o);
                    v = cc - ((unsigned)(a * q) >> 2);
                    ((int *)gm_Offers)[gm_NOffers * 6 + 3] = v;
                    ((int *)gm_Offers)[gm_NOffers * 6 + 3] = v & ((int)v < 0) - 1;
                    n = gm_NOffers + 1;
                    gm_NOffers++;
                } else if (f) {
                    gm_NOffers++;
                    n = gm_NOffers;
                }
            }
        }
    }
    cnt = n;
    for (i = 0; i < cnt; i++)
        *(float *)(((int *)gm_Offers) + i * 6 + 4) = (float)(gm_GoodsPrice(goods, ai_OwnPlayer) * *(float *)(((int *)gm_Offers)[i * 6] + 0x49));
    cnt = gm_NOffers;
    for (i = 1; i <= cnt; i++) {
        for (j = i; j < cnt; j++) {
            if (*(float *)(((int *)gm_Offers) + (i - 1) * 6 + 4) < *(float *)(((int *)gm_Offers) + j * 6 + 4)) {
                memcpy(tmp, ((int *)gm_Offers) + (i - 1) * 6, 24);
                memcpy(((int *)gm_Offers) + (i - 1) * 6, ((int *)gm_Offers) + j * 6, 24);
                memcpy(((int *)gm_Offers) + j * 6, tmp, 24);
            }
        }
    }
    return 0;
}
