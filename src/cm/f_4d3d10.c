/* 0x4d3d10, loot of a won fight: a random share (0.07 + 0.001 * 0 .. 49 of the winner's strength 0x509e60, at most 0x9c400)
   goes to the money message 0x4c47e0, then each object of the loser's stock (0x4ffe90 list 1) is moved to the winner
   (0x4c4880, 80 .. 99 % of its count) and its price added; the value goes to the loser's record (0x506430) */

#define G19_4C4880U ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)

int ai_Unk4d3d10(char *a, char *b, char *d, char *t)
{
    char *oa, *ob, *q;
    int cnt, v;
    float *fp;

    oa = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x116);
    ob = gm_Query(*(int *)(a + 0x5d), 2, 6, 0, 0x2a);
    v = gm_GetGoodsValue(t);
    v = v < 0 ? 0 : v;
    v = v < 0x9c400 ? v : 0x9c400;
    v = (int)(((double)(unsigned int)ai_Unk4e52f0(0x32) * 0.001 + 0.07) * v);
    cm_SendMoveGoods(*(int *)(d + 4), *(int *)(t + 4), ai_OwnPlayer, v);
    if (ob != 0 && oa != 0) {
        for (q = gm_Query(*(int *)(ob + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
            cnt = (int)(((double)(unsigned int)(ai_Unk4e52f0(0x14) + 0x50) * 0.01) * (double)*(int *)(q + 0xe));
            if (cnt < 1)
                cnt = 1;
            G19_4C4880U(*(int *)(oa + 2), *(int *)(ob + 2), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
            v = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * cnt + v);
        }
    }
    q = gm_PlayerRecord(d);
    if (q != 0)
        *(int *)(q + 0x1c) += v;
    return v;
}
