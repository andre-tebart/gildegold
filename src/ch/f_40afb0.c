/* 0x40afb0, allocates the action queue and declares the actions */

int ch_Open(void)
{
    ch_Queue = mem_Alloc(CH_QUEUE * sizeof(CHACTION), "ch:Open:ActionQueue");
    if (!ch_Queue)
        return 0;
    memset(ch_ActDefs, 0, sizeof(ch_ActDefs));
    memset(ch_List, 0, sizeof(ch_List));
    ch_DeclareAction(7, (CHACTFUNC)ch_ActTurn, 1, "bewegung/dreh_90_rechts", 2);
    ch_DeclareAction(0, (CHACTFUNC)ch_ActAnimOnce, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x17, (CHACTFUNC)ch_ActPlay, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x2d, (CHACTFUNC)ch_ActWalk, 1, "bewegung/gehen", 3);
    ch_DeclareAction(0x3a, (CHACTFUNC)ch_ActWalkCells, 1, "bewegung/gehen", 3);
    ch_DeclareAction(0x39, (CHACTFUNC)ch_ActWalk2RndDummy, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x2e, (CHACTFUNC)ch_ActPlay, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x2f, (CHACTFUNC)ch_ActSitDown, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x30, (CHACTFUNC)ch_ActStandUp, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x31, (CHACTFUNC)ch_ActTakingObject, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x32, (CHACTFUNC)ch_ActDroppingObject, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x33, (CHACTFUNC)ch_ActMove2Universe, 1, ch_EmptyName, 3);
    ch_DeclareAction(0x35, (CHACTFUNC)ch_ActTurnTo, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x36, (CHACTFUNC)ch_ActPlayOnce, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x37, (CHACTFUNC)ch_ActSetVisible, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x38, (CHACTFUNC)ch_ActFade, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x3b, (CHACTFUNC)ch_ActWait, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x34, (CHACTFUNC)ch_ActUsingGate, 1, ch_EmptyName, 5);
    memset(ch_Fades, 0, sizeof(ch_Fades));
    ch_PathMap = ch_MapAllocF(60.0f);
    return 1;
}
