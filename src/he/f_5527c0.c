/* 0x5527c0, the allocation handler: unless flag 4 of +0x78 is set the event is posted +0xb8 units later (due clock now, 0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5527c0(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, *(int *)(ev + 0xb8), 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
