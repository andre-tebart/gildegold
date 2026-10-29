/* 0x5c9df0, the blackmail payment window (511 B, a Ghidra-missed start): param_1 is the message record
   (AIMSG: +0 the id, +4 the handle). The window 0x4741f0 (0x62d4f8) with 0x476f80 / 0x475600 and the text
   0x515960 (0x1d62, id, 2, id, id). Click 0: 0x520f90 (the character 0x770b80 + 0x6f25b8 * 0x21c, 2), the
   close 0x4c5f60 (the character 0x770b84 + 0x6f25b8 * 0x21c, -2), the message record (kind byte 0x4b /
   type 2, the clock 0x770ae0 .., the character 0x770b84 + .., the handle) sent by 0x4c4b70, the date
   0x4fe140 (0x18) and the text 0x5176d0 (0x1d63, the date, id) / 0x521150, 0x6f22a0 = 4, return 1. Click 1:
   0x6f22a0 = 2. The pump 0x5309a0 and the end 0x4758f0. ROUGH: the record fields by offset. */

int ai_Unk5c9df0(unsigned short *param_1)
{
    int rec[0x3e];
    int date[6];
    char text[0x100];
    int win;
    int i;
    int ret;
    int charid;

    ret = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d4f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d62, *param_1, 2, *param_1, *param_1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                if (sv_CheckMoney((char *)((unsigned)&ai_Chars + *(int *)(unsigned)&ai_PlayerId * 0x21c), 2) != 0) {
                    charid = *(int *)((unsigned)&ai_CharInts + *(int *)(unsigned)&ai_PlayerId * 0x21c);
                    cm_SendMoneyDelta(charid, 0xfffffffe);
                    for (i = 0; i < 0x3e; i++)
                        rec[i] = 0;
                    *(char *)((char *)rec + 4) = 0x4b;
                    rec[2] = charid;
                    rec[3] = charid;
                    rec[0x16] = charid;
                    rec[10] = *(int *)(unsigned)&ai_Time;
                    rec[11] = *(int *)(unsigned)&ai_TimeHour;
                    rec[12] = *(int *)((unsigned)&ai_Time + 0x8);
                    *(short *)((char *)rec + 0x38) = *(short *)((unsigned)&ai_Time + 0xc);
                    *(char *)((char *)rec + 0x36) = 2;
                    rec[0x17] = *(int *)((char *)param_1 + 4);
                    cm_SendLongMessage(rec);
                    date[0] = *(int *)(unsigned)&ai_Time;
                    date[1] = *(int *)(unsigned)&ai_TimeHour;
                    date[2] = *(int *)((unsigned)&ai_Time + 0x8);
                    *(short *)&date[3] = *(short *)((unsigned)&ai_Time + 0xc);
                    gm_AddTime(date, 0x18, 0, 0);
                    txt_Format(text, (char *)0x1d63, date, *param_1);
                    sv_MessageBox(text, 0);
                    *(int *)(unsigned)&sv_PopRequest = 4;
                    ret = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                *(int *)(unsigned)&sv_PopRequest = 2;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Unk5c9df0, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
