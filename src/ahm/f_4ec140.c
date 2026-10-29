/* 0x4ec140, reads the walk state written by ahm_SaveWalk (0x4ebfb0, the load game 0x51c950 in ls.c calls it from version 0x10049) and reshuffles the table
   without restarting the walk (ahm_ShufflePlayers(0)); 1 when all was read, 0 when a read failed */


int ahm_LoadWalk(void *f)
{
    int i;

    if (ls_Read(&ai_WalkX, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkDX, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkY, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkDY, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkIndex, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkSeed, 2, 1, f) == 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (ls_Read(&ai_WalkSteps[i].on, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].x, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].y, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[0], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[1], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[2], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[3], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[4], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.id, 4, 1, f) == 0)
            return 0;
    }
    ahm_ShufflePlayers(0);
    return 1;
}
