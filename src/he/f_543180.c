/* 0x543180, a Ghidra miss (disassembled): the follow-up of the quarrel: tells b (news 0x175d, message 0x5fa) when flag 2 is set, then re-posts in 1 unit (state 0 only) */

void he_Unk543180(int ev)
{
    int a, b;
    char buf[0x100];

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    if (b == 0 || a == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(int *)(ev + 0x70) == 0) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            txt_Format(buf, 0x175d, *(unsigned short *)a);
            he_ShowNews(*(int *)(b + 4), -1, buf, 0, 0x5fa, (char *)0);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
}
