/* 0x549770, a Ghidra miss (disassembled): the run handler of the next trial step for the player (word +8): state 0 shows the news 0x1a9f to the figure +0xac and raises the 0x506430 counter (+0x64); state 1 spends the 0x5d08c0 of +0xb8 / +0xb9, shows 0x1a9e, counts +0xba (5 rounds end it) and pays the cost -+0xb4 (0x53eec0) (ROUGH) */

void he_Unk549770(int ev)
{
    char text[0x100];
    int st, rec, o, r, cnt;

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
    rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    o = gm_FindChar(*(int *)(ev + 0xac));
    if (st == 0) {
        if (o != 0) {
            txt_Format(text, 0x1a9f, *(unsigned short *)rec);
            he_ShowNews(*(int *)(o + 4), *(int *)(o + 4), text, 0, 0x5f4, (char *)0);
            r = gm_PlayerRecord(o);
            if (r != 0)
                *(int *)(r + 0x64) = *(int *)(r + 0x64) + 1;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
    if (st != 1)
        return;
    if (o == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
    ai_Unk5d08c0(rec, *(unsigned char *)(ev + 0xb8));
    ai_Unk5d08c0(rec, *(unsigned char *)(ev + 0xb9));
    txt_Format(text, 0x1a9e, *(unsigned short *)rec);
    *(unsigned char *)(ev + 0xba) = *(unsigned char *)(ev + 0xba) + 1;
    cnt = *(unsigned char *)(ev + 0xba);
    *(unsigned short *)(ev + 0x56) = *(unsigned short *)(ev + 0x56) + 0x18;
    *(int *)(ev + 0x70) = cnt < 5;
    he_Unk53eec0(*(unsigned short *)o, text, 0, -*(int *)(ev + 0xb4));
}
