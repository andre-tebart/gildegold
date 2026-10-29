/* 0x4ff950, the stock of the goods record i of the player p: the sum over the four buildings (the group (1, 5, number) of the bytes at +0x40 + 8k, each with the float at +4 of its slot added once per match) times 0.5, scaled by the float +0x24 / +0x20 when +0x24 is not 0; 0 when the first building byte is 0. ROUGH */

int gm_Unk4ff950(int p, int i)
{
    int sum;
    char *g;
    char *s;
    int k, o;

    sum = 0;
    g = GM_GOODS(p, i);
    s = g + 0x40;
    if (*s == 0)
        return 0;
    for (k = 4; k != 0; k--, s += 8) {
        if (*s != 0 && (o = gm_BuildQuery(1, 5, (unsigned char)*s)) != 0) {
            do {
                sum = (int)((float)sum + *(float *)(s + 4));
                o = gm_BuildNext();
            } while (o != 0);
        }
    }
    sum = (int)(sum * 0.5);
    if (*(float *)(g + 0x24) != 0.0f)
        sum = (int)(*(float *)(g + 0x24) / *(float *)(g + 0x20) * sum);
    return sum;
}
