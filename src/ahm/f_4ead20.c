/* 0x4ead20, seeds the generator (init: from the time) and shuffles the table; with init the walk is restarted: start positions random on the grid, the steps taken
   from the table 0x60f1b0 */

void ahm_ShufflePlayers(int init)
{
    unsigned int t;

    if (init != 0)
        ai_WalkSeed.seed16 = (unsigned short)(timeGetTime() % 0x7fff) + 1;
    srand(ai_WalkSeed.seed & 0xffff);
    ahm_ShuffleTable();
    if (init != 0) {
        ai_WalkX = util_Random(0x19) & 0xffff;
        ai_WalkDX = ahm_Tab[util_Random(8) & 0xffff];
        ai_WalkY = util_Random(0x19) & 0xffff;
        t = util_Random(8);
        ai_WalkIndex = 0;
        ai_WalkDY = ahm_Tab[t];
    }
}
