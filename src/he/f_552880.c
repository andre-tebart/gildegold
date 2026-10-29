/* 0x552880, the allocation handler of an event with a day timer (ROUGH): unless flag 4 the due clock is now (posted 0x18 later when the minute word was already >= 0x11), the minute word is set to 0x11, +0xac = 0, +0xb0 = 0x1b, +0xb4 = +0xb8 = -1 and the event posted (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk552880(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        if (*(unsigned short *)(ev + 0x56) >= 0x11)
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 0x11;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0x1b;
        *(int *)(ev + 0xb4) = -1;
        *(int *)(ev + 0xb8) = -1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
