/* 0x49c790, callback 0x0c: buy the good s1->unk04 at the place that sells it, if the character can pay
   (s1->unk14 in (0, 1) is the share of the money to use) */

int ai_Unk49c790(int a, G5CH *p, int f, AIARG *s1, AIARG *o)
{
    unsigned char *bld;
    AIOBJ *obj;
    int money;

    if (f == 2 && s1->kind == 2) {
        bld = (unsigned char *)gm_FindMarketObject(s1->unk04);
        if (bld != 0) {
            obj = gm_Query(*(int *)(bld + 0x14), 1, 0, s1->unk04);
            if (obj != 0 && gm_ListAmount(bld, obj) >= 1) {
                money = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
                if (s1->unk14 > 0.0f && s1->unk14 < 1.0f)
                    money = (int)(money * s1->unk14);
                if (money >= (int)gm_GoodsPrice(s1->unk04, ai_OwnPlayer)) {
                    o->kind = 1;
                    o->unk04 = *(int *)(bld + 2);
                    o->unk08 = 1;
                    return 0xc;
                }
            }
        }
    }
    return 0;
}
