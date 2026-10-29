/* 0x4ff450, a random market shake: with chance 1 / 2, 0 .. 1 times (0x2 draws) a random goods record of the player 0x6f1bc5 (0x54 + 1 records) that is in use gets a copy (0x80 bytes) with the float at +0x20 copied to +0x24 halved or raised by 1.5 (and the value 0x5087f0 (id, 100) likewise at +0x38), handed to 0x4c5710 (copy, player). ROUGH */

void gm_RandomMarketShake(void)
{
    int rec[0x20];
    unsigned n, k;
    int *g;
    float v;
    int i;

    if (util_Random(2) != 0) {
        n = util_Random(2) & 0xffff;
        for (; n != 0; n--) {
            k = util_Random(0x54) & 0xffff;
            g = (int *)(init_StockBlocks + ai_OwnPlayer * 0x2a90 + k * 0x80);
            if (*(short *)g != 0 && g[1] != 0) {
                for (i = 0; i < 0x20; i++)
                    rec[i] = g[i];
                if (util_Random(1) == 0) {
                    *(float *)(rec + 9) = *(float *)(rec + 8) * 0.5f;
                    v = (float)(gm_ItemPrice(*(short *)g, 100) * 0.5);
                } else {
                    *(float *)(rec + 9) = *(float *)(rec + 8) * 1.5f;
                    v = (float)(gm_ItemPrice(*(short *)g, 100) * 1.5);
                }
                *(float *)(rec + 0xe) = v;
                cm_SendGoodsDelta(rec, ai_OwnPlayer);
            }
        }
    }
}
