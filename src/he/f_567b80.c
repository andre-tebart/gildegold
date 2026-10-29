/* 0x567b80, the allocation handler of the tips (type 0x84; a Ghidra miss): the first run is today, 4 days later when a tutorial event 0x83 exists; +0x56 = 8, +0x58 = 0; 0x505ca0 (0x1a, 0x6fa1e0) fills the tip order table */

void he_Unk567b80(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (he_FindEvent(1, 0, 0x83) != 0)
        *(int *)(ev + 0x52) = U_I((unsigned)&ai_Time) + 4;
    else
        *(int *)(ev + 0x52) = U_I((unsigned)&ai_Time);
    *(short *)(ev + 0x56) = 8;
    *(int *)(ev + 0x58) = 0;
    gm_Shuffled(0x1a, (unsigned)&full_data_6fa1e0);
}
