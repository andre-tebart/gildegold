/* 0x4ab080, decide callback: at certain hours p (not flagged 0x2000000) picks one of the goods of another
   owner's shop (type cap > 1, objects 0x120 / 300) that costs at most 5 percent of its money, into a (kind
   0x13: item, price) and b (kind 4: the building); half of the time it only looks (a and b cleared) */

int ai_Unk4ab080(AIPER *p, int *a, int *b)
{
    int list[16];
    G10DIST info;
    int money, n, tries, item;
    int price, id2, x;
    unsigned int r, k;
    AIBLD *bld;
    AIOBJ *o;
    int i;

    if ((p->flags1cc & 0x2000000) == 0 && ai_TimeT.hour > 0x14
        && ai_TimeT.hour % 3 == (unsigned int)p->netId % 3) {
        money = gm_GetGoodsValue((AICHAR *)p);
        memset(list, 0, sizeof(list));
        n = 0;
        bld = gm_BuildQuery(1, 5, 0x16);
        if (bld != 0) {
            do {
                if (bld->owner != 0xffff && bld->owner != p->id && ai_BTypesT[bld->type].cap247 > 1
                    && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
                    && (o = gm_Query(o->list, 1, 0, 300)) != 0) {
                    list[n * 2] = (int)bld;
                    list[n * 2 + 1] = (int)o;
                    n++;
                }
                bld = gm_BuildNext();
            } while (bld != 0);
            if (n != 0) {
                r = util_RandomU(n);
                k = util_RandomU(3);
                bld = (AIBLD *)list[r * 2];
                o = (AIOBJ *)list[r * 2 + 1];
                tries = 3;
                do {
                    item = *(int *)((unsigned char *)o + 0x1c + k * 4);
                    if (unk_Unk4f4d30(item, &((AIPER *)ai_CharsT)[bld->owner], &price, &id2, &x) != 0
                        && (float)price <= money * 0.05f) {
                        amt_GetKind(((AIPER *)ai_CharsT)[(unsigned short)id2].unk166, &info);
                        if (info.lvl < util_RandomU(12))
                            return 0;
                        if (util_RandomF() >= 0.5) {
                            *(unsigned char *)a = 0x13;
                            a[1] = item;
                            a[4] = price;
                            *(unsigned char *)b = 4;
                            b[1] = bld->id;
                            return 1;
                        }
                        for (i = 6; i != 0; i--)
                            *a++ = 0;
                        for (i = 6; i != 0; i--)
                            *b++ = 0;
                        return 1;
                    }
                    k = (int)(k + 1) % 3;
                } while (--tries);
            }
        }
    }
    return 0;
}
