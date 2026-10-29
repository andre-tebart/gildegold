/* 0x4eadc0, shuffles the player prototype table (2 * count random swaps of two entries) */

void ahm_ShuffleTable(void)
{
    AHMGRID t;
    int n, i, j;

    for (n = ai_PlayerPrototypeCount * 2; n > 0; n--) {
        i = util_Random(ai_PlayerPrototypeCount);
        j = util_Random(ai_PlayerPrototypeCount);
        if (i != j) {
            t = ai_PlayerPrototypes[i];
            ai_PlayerPrototypes[i] = ai_PlayerPrototypes[j];
            ai_PlayerPrototypes[j] = t;
        }
    }
}
