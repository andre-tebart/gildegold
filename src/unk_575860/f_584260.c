/* 0x584260, the rent-a-grave window (427 B): rec = the record, a / b the values passed on; the window
   "Locations_Friedhof_RentAGrave" (0x62a5d8) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x4113
   (0x4112) and 0x625150 (0x4119) in the entry 0x475b80 (window, 3), the four buttons 0x583c50 (rec, window, 2,
   the buttons, the selection), the confirm enabled by 0x583ef0 (rec, a, b, the buttons, the selection),
   0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the
   confirm -> 0x475590 (window, 0), 0x584010 (rec, a, b, the selection) == 1 -> 1 and 0x6f22a0 = 1,
   0x475590 (window, 1); a clicked button != the selection -> the selection, 0x583c50 again and 0x583ef0 again;
   the pump 0 -> 0x4758f0 (window), the result */

int ai_Unk584260(char *rec, int a, int b)
{
    int win;
    int btn;
    int sel;
    int r;
    int i;
    int btns[4];

    sel = 0;
    r = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5d8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4113, 0x4112);
    ai_Unk583c50(rec, win, 2, btns, &sel);
    d2_SetForm(win, 3);
    i = io_PrintText((char *)(unsigned)&full_data_625150, 0x4119);
    btn = d2_GetWinObj(win, 3, i);
    d2_SetObjEnabled(btn, ai_Unk583ef0(rec, a, b, btns, &sel));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == btn) {
                d2_SetViewEnabled(win, 0);
                if (ai_Unk584010(rec, a, b, sel) == 1) {
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
                d2_SetViewEnabled(win, 1);
            } else {
                for (i = 0; i < 4; i++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == btns[i] && i != sel) {
                        sel = i;
                        ai_Unk583c50(rec, win, 2, btns, &sel);
                        d2_SetObjEnabled(btn, ai_Unk583ef0(rec, a, b, btns, &sel));
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_Unk584260, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
