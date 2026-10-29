/* 0x4d3ea0, the loot of the carts of a won fight: for each of the 3 cart ids at +0x100 of a, the goods of the cart
   (0x4ffe90 chains) go to the winner (0x4c4880, at least 1, f of the count; a free pick-up with b == 0) and their
   price is added; with no winner record half of the value goes to the money message 0x4c47e0; the value goes to
   the record of the attacker side (0x506430) */

int ai_Unk4d3ea0(char *a, char *b, char *d, int unused, float f)
{
    char *ob, *q, *p;
    int cnt, i;
    int dmg = 0;
    float x;

    if (ai_Unk6f0cec == 0 || ai_Unk6f0ce8 == 0)
        return 0;
    {
        ob = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x116);
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1) {
                p = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100 + i * 4));
                p = gm_Query(*(int *)(p + 0x14), 1, 0, 0x1dd);
                for (q = gm_Query(*(int *)(p + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
                    gm_Unk50a050(q);
                    x = (float)*(int *)(q + 0xe) * f;
                    if (1.0f > x)
                        x = 1.0f;
                    cnt = (int)x;
                    if (ob != 0)
                        G19_4C4880U(*(int *)(ob + 2), *(int *)(q + 6), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
                    else
                        G19_4C4880U(-1, *(int *)(q + 6), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
                    dmg = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * cnt + dmg);
                }
                if (ob == 0) {
                    dmg = (int)(dmg * 0.5);
                    cm_SendMoveGoods(*(int *)(d + 4), -1, ai_OwnPlayer, dmg);
                }
                q = gm_PlayerRecord((char *)ai_Unk6f0cec);
                if (q != 0)
                    *(int *)(q + 0x1c) += dmg;
            }
        }
    }
    return dmg;
}
