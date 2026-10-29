/* 0x551d00, the allocation handler of the event with the six ids at +0xb0 (ROUGH): 0x505ca0 (6, +0xb0) fills six ids, each is incremented, the due clock is now, state 0xc, minute word 9 */

void he_Unk551d00(int ev)
{
    int i;
    int *p = (int *)(ev + 0xb0);

    gm_Shuffled(6, p);
    for (i = 0; i < 6; i++)
        p[i] = p[i] + 1;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x70) = 0xc;
    *(unsigned short *)(ev + 0x56) = 9;
    *(int *)(ev + 0x58) = 0;
}
