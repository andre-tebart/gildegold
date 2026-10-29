/* 0x54a6f0, a Ghidra miss: the init of the plague event: counts the figures of the six slots (+0x8c .. , count in the byte +0xd8; none: the event is freed), clears +0xac .. (6 dwords) to -1, +0xd2 = 0, the clock +0xc4 = now, due now and posted 2 units ahead */

void he_Unk54a6f0(int ev)
{
    int i;
    int *p;

    *(unsigned char *)(ev + 0xd8) = 0;
    p = (int *)(ev + 0x8c);
    for (i = 0; i < 6; i++) {
        if (p[i] != -1)
            *(unsigned char *)(ev + 0xd8) = *(unsigned char *)(ev + 0xd8) + 1;
    }
    if (*(unsigned char *)(ev + 0xd8) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    memset((void *)(ev + 0xac), -1, 6 * 4);
    *(unsigned char *)(ev + 0xd2) = 0;
    *(HETIME *)(ev + 0xc4) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
}
