/* 0x586e40, the graveyard "ALTERN_LASSEN" contact (842 B): the sibling of 0x586ab0; rec = the contact record
   (+0x27 the owner word, +1 a dword). The target picked by 0x5c1130 (the 0x50-byte query: 1, 0x400, 6 at
   +0x10, -1 at +0x1c .. +0x38, U_I (0x144a444)); none -> 0x52d7a0 (). 0x525220 (rec, 0), the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x40a4 /
   0x40a3 (the target's word, 4) and 0x40a5 / 0x40a5 + 1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump
   loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x520f90 (the
   player's character, 4) == 0 -> skip, the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c,
   i = 0 .. 0x2ff) or none -> 0x521150 (U_I (0x144a40c), 0); else the message (type 0x89 at +4, 1 at +0x36, 4
   at +0x64, the player's character word at +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 /
   word 0x770aec at +0x28, the target +4 at +0x5c, the found character +4 at +0x38 / +0x60, the level at +0x58:
   the target's +0x170 / +0x16c record + 1 or 0x4a63f0 (the player's character, 0, 0x7b4c4a0, 0, 0, 100.0f,
   &lv) with lv = -1) through 0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, the
   line 0x40a7 (the found character's word) in 0x5176d0 and 0x521150 (the line, 0), 0x6f22a0 = 1; the second
   entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 () */

void ai_Unk586e40(char *rec)
{
    char msg[0xf8];
    char buf[0x200];
    char q[0x50];
    char *p;
    char *f;
    int win;
    int b1;
    int b2;
    int v;
    int i;
    int h;
    int lv;

    memset(q, 0, 0x50);
    for (i = 0; i < 8; i++)
        *(int *)(q + 0x1c + i * 4) = -1;
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a444), 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_Unk525220(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    i = io_PrintText(0x40a4, 0x40a3, *(unsigned short *)p, 4);
    d2_SetForm(win, 2);
    i = io_PrintText(0x40a5);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                v = 0;
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4) == 0)
                    v = 1;
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (v == 0) {
                    if (f == 0) {
                        sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                    } else {
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x89;
                        *(unsigned char *)(msg + 0x36) = 1;
                        *(int *)(msg + 0x64) = 4;
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(int *)(msg + 0x60) = *(int *)(f + 4);
                        *(int *)(msg + 0x38) = *(int *)(f + 4);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x5c) = *(int *)(p + 4);
                        lv = -1;
                        if (*(int *)(p + 0x170) == 0) {
                            if (*(int *)(p + 0x16c) == 0)
                                AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                            else
                                lv = *(int *)(*(int *)(p + 0x16c) + 1);
                        } else {
                            lv = *(int *)(*(int *)(p + 0x170) + 1);
                        }
                        *(int *)(msg + 0x58) = lv;
                        h = cm_SendLongMessage(msg);
                        if (h != -1) {
                            while (cm_GetSlotState(h) == 0)
                                sv_RunFrame(0, 0x20007);
                        }
                        U_I((unsigned)&sv_FrameMask) = 0x677c7;
                        txt_Format(buf, (char *)0x40a7, *(unsigned short *)f);
                        sv_MessageBox(buf, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk586e40, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_Unk52ae40();
            return;
        }
    }
}
