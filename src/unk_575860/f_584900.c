/* 0x584900, the graveyard grave action with a character (615 B, a start Ghidra missed: the callback is reached
   only through a table; the ledger found the dword 0x584b31): rec / ev the event record with +4 (a dword) and
   +8 (a byte). When 0x573200 (rec, ev+4, ev+8) finds a running event of the type 0x8d, the green message box
   0x521410 (U_I (0x144a638), U_I (0x144a63c), 0) and 0; else the player bar 0x525220 (rec, 0), the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x4120
   (0x411e, 0x228) and 0x4121 / +1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this
   window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: the first selected character (the
   byte +0x188 of the records 0x770b80 + i * 0x21c, i = 0 .. 0x2ff), none -> 0x521150 (U_I (0x144a40c), 0),
   else the message (type 0x8d at +4, 1 at +0x36, the character 0x770b84 + rec's owner word +0x27 * 0x21c at
   +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the character record
   +4 at +0x38, ev +4 at +0x5c, ev +8 at +0x60) through 0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007),
   and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 (), the
   result */

int sv_MessageBoxGreen(); int sv_OpenPlayerBar(); int sv_ClosePlayerBar();

int ai_Unk584900(char *rec, char *ev)
{
    char msg[0xf8];
    char *p;
    int win;
    int b1;
    int b2;
    int v;
    int i;
    int r;
    int h;

    r = 0;
    if (he_Unk573200(rec, *(int *)(ev + 4), *(unsigned char *)(ev + 8)) != 0) {
        sv_MessageBoxGreen(U_I((unsigned)&full_data_144a638), U_I((unsigned)&full_data_144a63c), 0);
        return 0;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4120, 0x411e, 0x228);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4121);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                p = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        p = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (p == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x8d;
                    *(unsigned char *)(msg + 0x36) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x38) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = *(int *)(ev + 4);
                    *(unsigned char *)(msg + 0x60) = *(unsigned char *)(ev + 8);
                    h = cm_SendLongMessage(msg);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk584900, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return r;
        }
    }
}
