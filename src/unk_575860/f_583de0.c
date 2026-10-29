/* 0x583de0, a graveyard confirmation window (272 B): rec / the two values passed on to 0x4f0900; the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x410f /
   0x410e and the entries 0x4111 / +1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0
   (this window, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the first entry -> 0x4f0900 (0, rec, the two
   values), 1; the second -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the result */

int unk_Unk4f0900(); int unk_Unk4ef4f0(); void sv_ButtonRow(int f, int y, int *out, int n, ...);

int ai_Unk583de0(char *rec, int a, unsigned char b)
{
    int win;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x410f, 0x410e);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4111);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                unk_Unk4f0900(0, rec, a, b);
                done = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk583de0, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
