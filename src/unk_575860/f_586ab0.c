/* 0x586ab0, the graveyard "HEIMSUCHUNG" contact (902 B): rec = the contact record (+0x27 the owner word, +1 a
   dword). The target picked by 0x5c1130 (the 0x50-byte query: 1, 0x400, 6 at +0x10, -1 at +0x1c .. +0x38,
   U_I (0x144a414)); none -> 0x52d7a0 (). 0x525220 (rec, 0), the window "Locations_Friedhof_Friedhof"
   (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x4098 / 0x4097 (the target's word twice)
   in 0x475b80 (window, 1, text) = the slider, 0x475cb0 (slider, 4, 0xc, 4), the texts 0x4099 / 0x4099 + 1 in
   0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape
   0x14673a0 -> 0x6f22a0 = 1; the first entry: n = 0x475980 (slider), 0x520f90 (the player's character, n) == 0
   -> skip, the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c, i = 0 .. 0x2ff) or none ->
   0x521150 (U_I (0x144a40c), 0); else the message (type 0x88 at +4, 2 at +0x36, n at +0x64, the player's
   character word at +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the
   target +4 at +0x5c, the found character +4 at +0x38 / +0x60, the level at +0x58: the target's +0x170 / +0x16c
   record + 1 or 0x4a63f0 (the player's character, 0, 0x7b4c4a0, 0, 0, 100.0f, &lv) with lv = -1) through
   0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, 0x6f22a0 = 1, 0x521150
   (U_I (0x144a428), 0); the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (),
   0x52ae40 () */

int ai_AmountMenu(); int sv_CheckMoney();

void ai_Unk586ab0(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    char *f;
    int win;
    int slider;
    int b1;
    int b2;
    int n;
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
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a414), 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    n = *(unsigned short *)p;
    slider = d2_GetWinObj(win, 1, io_PrintText(0x4098, 0x4097, n, n));
    d2_SetForm(win, 2);
    n = io_PrintText(0x4099);
    b1 = d2_GetWinObj(win, 2, n);
    b2 = d2_GetWinObj(win, 2, n + 1);
    d2_SetObjValue(slider, 4, 0xc, 4);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                v = 0;
                n = d2_GetObjValue(slider);
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, n) == 0)
                    v = 1;
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else if (v == 0) {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x88;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = n;
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(int *)(msg + 0x5c) = *(int *)(p + 4);
                    *(int *)(msg + 0x60) = *(int *)(f + 4);
                    *(int *)(msg + 0x38) = *(int *)(f + 4);
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
                    U_I((unsigned)&sv_PopRequest) = 1;
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a428), 0);
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk586ab0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return;
        }
    }
}
