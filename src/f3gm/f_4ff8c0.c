/* 0x4ff8c0, the amount of the goods record i (+8 a float rate) for the player p: 0 for the goods kinds 0x17 / 0x25; for player 0 the float 0x6f1b9c * the rate * 0.0025 + 0.5, for the others the player's first int (price index, 0x7d6a60) * the rate * 0.0005 */

int gm_Unk4ff8c0(int p, int i)
{
    char *g;
    char k;

    g = GM_GOODS(p, i);
    k = ai_Goods[*(unsigned short *)g * 0x41];
    if (k != 0x17 && k != 0x25) {
        if (p == 0)
            return (int)(*(float *)&ai_RatingBase * *(float *)(g + 8) * 0.0025 + 0.5);
        return (int)((float)ai_Unk7d6a60[p * 0xaa4] * *(float *)(g + 8) * 0.0005);
    }
    return 0;
}
