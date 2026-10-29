/* 0x5caab0, the bewitch window "privillegien\becircen" 0x62d554 (544 B, a Ghidra-missed start: reached only
   as the callback of 0x5cacd0's menu 0x5c1130): param_1 is the message record (the AIMSG: +0 the id, +9 the
   flag). The player's character (0x770b80 + U_W(0x6f25b8) * 0x21c) byte +9 against the record's +9 first:
   equal shows the text 0x5176d0 (0x1db1 and 0x1db3 when the byte is set, else 0x1db2) and returns 0.
   Otherwise the window 0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 with the text 0x515960 (0x1db4, the id
   twice), the button 0x475b80 and the slider 0x475cb0 (1, 5, 1); a click (0x475980 and 0x520f90 of the
   character with the value) pays 0x494d40 (the character, the record, the value), closes 0x4c5f60 (-the
   value), sends 0x4c4b00 (the handle, the character +4, the payment, 1), shows 0x5176d0 (0x1db5, the id
   twice) / 0x521150 and sets 0x6f22a0 = 4, return 1; the second click sets 2. The pump 0x5309a0 (this,
   0x657c7) and the end 0x4758f0. ROUGH: the record by offset. */

int ai_Unk5caab0(int param_1)
{
    char text[256];
    unsigned short *chr;
    int win;
    int btn;
    int v;
    int r;
    int ret;

    chr = (unsigned short *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    if (*(char *)((char *)chr + 9) == *(char *)(param_1 + 9)) {
        if (*(char *)((char *)chr + 9) != 0)
            txt_Format(text, (char *)0x1db1, 0x1db3);
        else
            txt_Format(text, (char *)0x1db1, 0x1db2);
        sv_MessageBox(text, 0);
        return 0;
    }
    ret = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d554);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    btn = d2_GetWinObj(win, 1, io_PrintText(0x1db4, *(unsigned short *)param_1,
                                            *(unsigned short *)param_1));
    d2_SetObjValue(btn, 1, 5, 1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                v = d2_GetObjValue(btn);
                if (sv_CheckMoney(chr, v) != 0) {
                    r = ai_Unk494d40(chr, (void *)param_1, v);
                    cm_SendMoneyDelta(U_I((char *)chr + 4), -v);
                    cm_SendRelations(*(int *)(param_1 + 4), U_I((char *)chr + 4), r, 1);
                    txt_Format(text, (char *)0x1db5, *(unsigned short *)param_1,
                                 *(unsigned short *)param_1);
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
        if (sv_RunFrame(ai_Unk5caab0, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
