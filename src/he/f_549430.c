/* 0x549430, a Ghidra miss (disassembled): the run handler of the trial of the figure +0xac for the player (word +8): state 0 counts the rounds (+0xb8, +0x56 += 0x18 units, the news 0x1a9b, the player pays the cost -+0xb4 with 0x53eec0; the 3rd round goes to the state 1), state 1 picks two random abilities of the player (0x504ad0 / 0x504100, +0x10 random bytes each at +0x80 by 0x4c3d00), sets the party flags 0x4c59b0 (+0xb0) and tells the figure the news 0x1a9c (ROUGH) */

void he_Unk549430(int ev)
{
    char text[0x100];
    int st, rec, o, v, x10 = 0, x14 = 0;
    unsigned char b1, b2;
    int cnt;

    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    if (st == 0) {
        rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
        o = gm_FindChar(*(int *)(ev + 0xac));
        if (o == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, 0x1a9b, *(unsigned short *)rec);
        *(unsigned char *)(ev + 0xb8) = *(unsigned char *)(ev + 0xb8) + 1;
        *(unsigned short *)(ev + 0x56) = *(unsigned short *)(ev + 0x56) + 0x18;
        cnt = *(unsigned char *)(ev + 0xb8);
        cm_SendExSetHE(cnt >= 3, ev);
        he_Unk53eec0(*(unsigned short *)o, text, 0, -*(int *)(ev + 0xb4));
        return;
    }
    if (st != 1)
        return;
    rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    v = gm_TitleGroup(*(unsigned char *)(ev + 0xb0), &x10, &x14);
    gm_JobSlots(v, &x10, &x14);
    cm_BeginFieldBlock(rec, *(int *)(rec + 4));
    b1 = (unsigned char)(util_Random(0x10) + 0x10);
    cm_AddFieldValues(1, 1, x10 + 0x80, &b1);
    b2 = (unsigned char)(util_Random(0x10) + 0x10);
    cm_AddFieldValues(1, 1, x14 + 0x80, &b2);
    cm_SendFieldDeltas();
    ai_Unk4c59b0(*(int *)(rec + 4), *(unsigned char *)(ev + 0xb0));
    o = gm_FindChar(*(int *)(ev + 0xac));
    if (o == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
    txt_Format(text, 0x1a9c, *(unsigned short *)rec, ((*(char *)(rec + 9) != 0) ? 0x5f : 0) + 0x130 + *(unsigned char *)(ev + 0xb0));
    he_ShowNews(*(int *)(o + 4), *(int *)(o + 4), text, 0, 0x5f4, (char *)0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    cm_SendExSetHE(-1, ev);
}
