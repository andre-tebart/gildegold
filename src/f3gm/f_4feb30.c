/* 0x4feb30, runs 0x4ff550 (player i, the clock 0x770ae0) for every used player slot (0x2f4 bytes each, from 0x7e3f40) */

void gm_UpdatePlayers(void)
{
    char *p;
    int i;

    i = 0;
    p = ai_PlayerSlots[0];
    do {
        if (*p == 0)
            return;
        gm_UpdatePrices(i, ai_Time);
        p += 0x2f4;
        i++;
    } while ((int)p < (int)((unsigned)ai_PlayerSlots + 0xbd0));
}
