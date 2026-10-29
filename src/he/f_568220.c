/* 0x568220, a Ghidra miss: the run handler of the event 0xa6, the patrol of the guards (only with the sound / game flag 0x6f25ac; waits for the sound handle +0x84): state 0 puts the player 0x501190 (1, 0, 0x44)
   in 0x6f2638 and calls 0x567f50 (ev, g) for every guard group (0x4ffe90 (0, 2, 7, 4, 0x1d) / 0x4ffcc0) with a route (+0x29), not flagged (+0x20 & 1, +0x13 & 0x20) and with a pause counter (+0x33); the next run is 15
   units ahead; -1 / -2 free */

void he_Unk568220(int ev)
{
    int st, g;

    if (*(char *)(unsigned)&ai_GameState == 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    st = *(int *)(ev + 0x70);
    *(int *)(ev + 0x84) = -1;
    if (st < -2)
        return;
    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    U_I((unsigned)&full_data_6f2638) = gm_BuildQuery(1, 0, 0x44);
    for (g = gm_Query(0, 2, 7, 4, 0x1d); g != 0; g = gm_QueryNext()) {
        if (*(int *)(g + 0x29) != 0 && (*(unsigned char *)(g + 0x20) & 1) == 0 && (*(unsigned char *)(g + 0x13) & 0x20) == 0 && *(char *)(g + 0x33) != 0)
            he_Unk567f50(ev, (unsigned short *)g);
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0xf, 0);
}
