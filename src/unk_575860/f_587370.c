/* 0x587370, the graveyard "VERSICHERUNG" contact (485 B): rec = the contact record (+0x27 the owner word, +1 a
   dword). 0x525220 (rec, 0), the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the
   sound 0x52ea30, the texts 0x40f6 / 0x40f5 (0x228) and 0x40f7 / 0x40f7 + 1 in 0x475b80 (window, 2, text),
   0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first
   entry: the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c, i = 0 .. 0x2ff) or none ->
   0x521150 (U_I (0x144a40c), 0); else the message (type 0x8a at +4, 1 at +0x36, the player's character word at
   +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the found character +4
   at +0x38 / +0x58) through 0x4c4b70 and 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), 0x525440 (), 0x52ae40 () */

void ai_Unk587370(char *rec)
{
    char msg[0xf8];
    char *f;
    int win;
    int b1;
    int b2;
    int i;

    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x40f6, 0x40f5, 0x228);
    d2_SetForm(win, 2);
    i = io_PrintText(0x40f7);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x8a;
                    *(unsigned char *)(msg + 0x36) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x38) = *(int *)(f + 4);
                    *(int *)(msg + 0x58) = *(int *)(f + 4);
                    cm_SendLongMessage(msg);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk587370, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return;
        }
    }
}
