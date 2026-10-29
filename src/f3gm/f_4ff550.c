/* 0x4ff550, the daily price index update of the player p at the clock t: the calendar date of 0x4fe2c0 (year, month number) is looked up in the two piecewise linear tables of the player (9 points of (value, month) at +0x60 / +0x50 of the slot 0x7e3f40 + p * 0x2f4 each), interpolated into the indices [0] / [2] of the player's record (0x7d6a60, times the factor [1] / [3] / 1000); then the 0x55 goods records: the float +0x20 moves halfway to 0x4ff8c0, the own player gets 0x4ff950 into +0x34, others copy +0x30; +0x38 = the price 0x4ffa10; the counters +0x10 .. +0x1c are cleared; at the end of the calendar the market (0x4fec10, 0x4ff450) runs. ROUGH */

void gm_SyncMarket(unsigned p);
int gm_Unk4ff8c0();
int gm_Unk4ff950();

void gm_UpdatePrices(unsigned p, void *t)
{
    unsigned char d[10];   /* gm_ClockToDate writes 10 bytes: out[0..5] and the int at +6 */
    int month, n, k, a, x0, x1;
    int *pl = (int *)ai_PlayerSlots[p];
    int *rec = ai_Unk7d6a60 + p * 0xaa4;
    int i;
    char *g;
    float f;

    gm_ClockToDate(t, d);
    month = *(short *)(d + 2);
    for (n = 0; n < 9; n++)
        if (pl[0x64 / 4 + 2 * n + 2] <= pl[0x64 / 4 + 2 * n])
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (month <= pl[0x64 / 4 + 2 * k])
            break;
    if (k < n - 1) {
        x0 = pl[0x64 / 4 + 2 * k];
        if (month == x0)
            rec[0] = (((int)(char)d[1] + (month - x0) * 0xc) * (pl[0x70 / 4 + 2 * k] - pl[0x68 / 4 + 2 * k])) / ((pl[0x6c / 4 + 2 * k] - x0) * 0xc) + pl[0x68 / 4 + 2 * k];
        else if (k < 1)
            rec[0] = pl[0x68 / 4];
        else {
            x1 = pl[0x5c / 4 + 2 * k];
            rec[0] = (((int)(char)d[1] + (month - x1) * 0xc) * (pl[0x68 / 4 + 2 * k] - pl[0x60 / 4 + 2 * k])) / ((x0 - x1) * 0xc) + pl[0x60 / 4 + 2 * k];
        }
    } else
        rec[0] = pl[0x60 / 4 + 2 * n];
    rec[0] = (rec[1] * rec[0]) / 1000;
    for (n = 0; n < 9; n++)
        if (pl[0xb4 / 4 + 2 * n + 2] <= pl[0xb4 / 4 + 2 * n])
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (month <= pl[0xb4 / 4 + 2 * k])
            break;
    if (k < n - 1) {
        x0 = pl[0xb4 / 4 + 2 * k];
        if (month == x0)
            a = (((int)(char)d[1] + (month - x0) * 0xc) * (pl[0xc0 / 4 + 2 * k] - pl[0xb8 / 4 + 2 * k])) / ((pl[0xbc / 4 + 2 * k] - x0) * 0xc) + pl[0xb8 / 4 + 2 * k];
        else if (k < 1)
            a = pl[0xb8 / 4];
        else {
            x1 = pl[0xac / 4 + 2 * k];
            a = (((int)(char)d[1] + (month - x1) * 0xc) * (pl[0xb8 / 4 + 2 * k] - pl[0xb0 / 4 + 2 * k])) / ((x0 - x1) * 0xc) + pl[0xb0 / 4 + 2 * k];
        }
        rec[2] = a;
    } else
        rec[2] = pl[0xb0 / 4 + 2 * n];
    rec[2] = (rec[3] * rec[2]) / 1000;
    for (i = 0; i < 0x55; i++) {
        g = GM_GOODS(p, i);
        if (ai_Goods[*(unsigned short *)g * 0x41] != 0x17 && ai_Goods[*(unsigned short *)g * 0x41] != 0x25) {
            a = gm_Unk4ff8c0(p, i);
            *(float *)(g + 0x20) = ((float)a - *(float *)(g + 0x20)) * 0.5f + *(float *)(g + 0x20);
        }
        if (p == ai_OwnPlayer)
            *(int *)(g + 0x34) = gm_Unk4ff950(p, i);
        else
            *(int *)(g + 0x34) = *(int *)(g + 0x30);
        f = gm_Unk4ffa10(p, i);
        *(float *)(g + 0x38) = f;
        if (g[0x40] != 0)
            *(float *)(g + 0x20) = (float)*(int *)(g + 0x34);
        *(int *)(g + 0x10) = 0;
        *(int *)(g + 0x14) = 0;
        *(int *)(g + 0x1c) = 0;
        *(int *)(g + 0x18) = 0;
    }
    if (ai_Unk1460ae0 == -1) {
        gm_SyncMarket(p);
        gm_RandomMarketShake();
    }
}
