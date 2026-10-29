/* 0x5c9990, a blackmail payment window (694 B, a Ghidra-missed start): param_1 is the message record (the
   AIMSG: +0 the id, +2 the type, +4 the handle, +9 a flag, +0x1c8 the flags). The flag 0x400 -> the text
   0x5176d0 (0x1d7f) / 0x521150 and return 0. Else the window 0x4741f0 (0x62cd48) with the texts 0x515960
   (0x1d67, id, 2, id, flag ? 0x1d69 : 0x1d68, id) and the sub-window 0x5bd250 of the six values {win, 2,
   3, 4, 4, 4}. Click 0: 0x520f90 (the character 0x770b80 + 0x6f25b8 * 0x21c, 2), the two rolls 0x42ec00
   (9), the message record (the clock 0x770ae0 .., the character 0x770b84 + 0x6f25b8 * 0x21c, the kinds
   0x47 / 2, the two texts 0x1d6d / 0x1d76 + the rolls), 0x4c4b70 / 0x4c3ae0 / 0x52fe00, the close 0x4c5f60
   (-2) and the text 0x1d6a / 0x521150, 0x6f22a0 = 3, return 1. Click 1: 0x6f22a0 = 2. The pump 0x5309a0
   and the end 0x4758f0. ROUGH: the record fields by offset. */

int ai_Unk5c9990(unsigned short *param_1)
{
    int rec[0x3e];
    int tbl[6];
    char text[0x100];
    int win;
    int h;
    int i;
    int ret;
    int charid;
    unsigned short r1;
    unsigned short r2;

    ret = 0;
    if ((*(unsigned int *)((char *)param_1 + 0x1c8) & 0x400) != 0) {
        txt_Format(text, (char *)0x1d7f, *param_1);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62cd48);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d67, *param_1, 2, *param_1, *(char *)((char *)param_1 + 9) ? 0x1d69 : 0x1d68,
                 *param_1);
    for (i = 0; i < 6; i++)
        tbl[i] = -1;
    tbl[0] = win;
    tbl[1] = 2;
    tbl[2] = 3;
    tbl[3] = 4;
    tbl[4] = 4;
    tbl[5] = 4;
    ai_Unk5bd250(tbl);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                if (sv_CheckMoney((char *)((unsigned)&ai_Chars + *(int *)(unsigned)&ai_PlayerId * 0x21c), 2) != 0) {
                    r1 = util_Random(9);
                    r2 = util_Random(9);
                    for (i = 0; i < 0x3e; i++)
                        rec[i] = 0;
                    charid = *(int *)((unsigned)&ai_CharInts + *(int *)(unsigned)&ai_PlayerId * 0x21c);
                    *(char *)((char *)rec + 4) = 0x47;
                    rec[2] = charid;
                    rec[3] = charid;
                    rec[0x16] = charid;
                    rec[10] = *(int *)(unsigned)&ai_Time;
                    rec[11] = *(int *)(unsigned)&ai_TimeHour;
                    rec[12] = *(int *)((unsigned)&ai_Time + 0x8);
                    *(short *)((char *)rec + 0x38) = *(short *)((unsigned)&ai_Time + 0xc);
                    *(char *)((char *)rec + 0x36) = 2;
                    rec[0x17] = *(int *)((char *)param_1 + 4);
                    *(short *)((char *)rec + 0x60) = r2 + 0x1d6d;
                    *(short *)((char *)rec + 0x62) = r1 + 0x1d76;
                    h = cm_SendLongMessage(rec);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(int *)(unsigned)&ai_PlayerId * 0x21c), 0xfffffffe);
                    txt_Format(text, (char *)0x1d6a, r2 + 0x1d6d, *param_1, r1 + 0x1d76);
                    sv_MessageBox(text, 0);
                    *(int *)(unsigned)&sv_PopRequest = 3;
                    ret = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                *(int *)(unsigned)&sv_PopRequest = 2;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Unk5c9990, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
