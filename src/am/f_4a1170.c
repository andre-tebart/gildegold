/* 0x4a1170, decide callback 0x3a: the oldest grown-up unmarried child (as in 0x4a1000) gets a building class
   (o2 kind 0x17): a random one the dynasty has none of (0x506430 + 0x74, kind 5), the family's (0x504ad0) or
   a random 1..15; the character pays 1.5 % of 0x50a640 and needs six times that */

int ai_Unk4a1170(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    unsigned char *fin;
    unsigned char k;
    int m, price, q, r, n;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            k = ((unsigned char *)c)[2];
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0
            && (float)(c->unk00a + 4) >= *(float *)((unsigned char *)c + 0x20)
            && ((unsigned char *)c)[0x164] == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    price = (int)(gm_GetWorth(p->id) * 0.015f);
    if (gm_Unk509ee0((AIPER *)p, ai_OwnPlayer) < price * 6)
        return 0;
    q = util_Random(0xf) + 1;
    if (((unsigned char *)p)[2] == 5) {
        fin = (unsigned char *)gm_PlayerRecord((AICHAR *)p);
        r = util_Random(0x10);
        for (n = 0x10; n != 0; n--) {
            if (r != 0 && fin[r + 0x74] == 0) {
                q = r;
                break;
            }
            r = (r + 1) % 0x10;
        }
    } else if (((unsigned char *)p)[0x164] != 0) {
        q = gm_TitleGroup(((unsigned char *)p)[0x164]);
    }
    o1->kind = 7;
    o1->unk04 = best->netId;
    o2->kind = 0x17;
    o2->unk04 = gm_TitleNumber(q, 1);
    o2->unk10 = price;
    return 0x3a;
}
