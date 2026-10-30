/* 0x4bfc60, cart c loads at its current route point: the goods of the point's loadSlots slots go from the building's
   stock (market: 0x1dc) into the cart, paid by player pl (a message when pl cannot afford it)
   @deep behaviour: the price factors are fixed (1.1f / *(float*)(b+0x49)); what remains is the local frame: the original zeroes each RTINV local as rep stos 0x2d dwords (0xb4) plus a separate dword store at +0x15c/+0x24 where we zero 0x2e dwords, and the stores/registers differ (67 code hunks, slots 16, order 8). The RTINV zeroing seam suggests the original's local is not the plain RTINV view (rt.h type debt). Q2/Q4 stay open */

void rt_Unk4bfc60(char *c, char pl)
{
    RTPOINT *q;
    short *o;
    char *b;
    char *t;
    char *st;
    unsigned short *g;
    int *s;
    int k;
    int n;
    int price;
    RTINV ci;
    char name[0x80];
    RTINV bi;
    char buf[0x400];

    memset(&bi, 0, sizeof(bi));
    memset(&ci, 0, sizeof(ci));
    if (*(RTPOINT **)(c + 0x29) == 0)
        return;
    q = rt_PointAt(*(RTPOINT **)(c + 0x29), *(int *)(c + 0x2d));
    o = gm_Unk50a050(c);
    b = gm_FindBuildingById(q->buildingId);
    if (b == 0)
        return;
    t = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (*t != 10 && *t != 0x1c && !gm_BuildingGoodsList(b, &bi, 0) || !gm_StallGoodsList(c, &ci))
        return;
    s = q->loadSlots;
    for (k = 3; k != 0; k--) {
        if (s[0] != 0) {
            price = 0;
            if (*t == 10)
                st = gm_FindMarketObject(s[0]);
            else if (*t == 0x1c)
                st = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1dc, 6);
            else
                st = bi.stock;
            g = gm_Query(*(int *)(st + 0x14), 1, 0, s[0]);
            if (g != 0) {
                n = s[1];
                if (n >= gm_ListAmount(st, g))
                    n = gm_ListAmount(st, g);
                n = gm_FreeCapacity(ci.stock, s[0], n);
                if (*(short *)(b + 0x27) != *o) {
                    if (*t == 10)
                        price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * 1.1f);
                    else if (*t == 0x1c)
                        price = (int)(gm_GoodsPrice(*g, b[0x65]) * 1.1f);
                    else
                        price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * *(float *)(b + 0x49));
                }
                if (n > 0) {
                    if (gm_Unk509ee0(o, pl) < n * price) {
                        io_TownDisplayName((char *)ai_PlayerSlots + pl * 0x2f4, name);
                        txt_Format(buf, 0x42f0, name, 0xc14, name, pl);
                        he_ShowNews(*(int *)(o + 2), -1, buf, 0, 0x5ef, ch_EmptyName);
                    }
                    cm_SendExSellObjectI(*(int *)(ci.stock + 2), *(int *)(st + 2), s[0], n, pl, price);
                }
            }
        }
        s += 2;
    }
}
