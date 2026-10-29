/* 0x585750, the graveyard "Seance" window (613 B): the first argument is unused, ev the event record, out the
   result record. out[0] / out[1] = 0, out+8 = ev+0x20, out+0xc = ev+0x24, out+0x10 = **(int **)(ev + 0x2c +
   *(int *)(ev + 0x88) * 0xc) and its 0x11 dwords copied to out+0x14; when the player's character byte +0x1cc
   has bit 8 -> the message box 0x521410 (U_I (0x144a4f0), U_I (0x144a4f4), 0) and 0. Else the window
   "Locations_Friedhof_Seance" (0x62a680) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x40ce
   (0x40cc), the four entries 0x40cf (4) / 0x40d0 (6) / 0x40d1 (8) / 0x40d2 (cancel) in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1;
   the cancel -> 0x6f22a0 = 1; the first entry -> 0x520f90 (the character, 4) * 1, the second -> * 2, the third
   -> * 3; a non-zero result -> out[0] = the index, out[1] = 4 / 6 / 8, 0x6f22a0 = 1 and 1; the pump 0 ->
   0x4758f0 (window), the result */

int ai_Unk585750(char *unused, char *ev, int *out)
{
    int amt[4];
    int *p;
    int *q;
    int win;
    int b1;
    int b2;
    int b3;
    int b4;
    int v;
    int i;
    int r;

    r = 0;
    out[1] = 0;
    out[0] = 0;
    *(int *)((char *)out + 0x08) = *(int *)(ev + 0x20);
    *(unsigned char *)((char *)out + 0x0c) = *(unsigned char *)(ev + 0x24);
    amt[0] = 0;
    amt[1] = 4;
    amt[2] = 6;
    amt[3] = 8;
    p = *(int **)(ev + 0x2c + *(int *)(ev + 0x88) * 0xc);
    *(int *)((char *)out + 0x10) = *p;
    q = (int *)((char *)out + 0x14);
    for (i = 0; i < 0x11; i++)
        q[i] = p[i];
    if (*(unsigned char *)((unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c + 0x1cc) & 8) {
        sv_MessageBoxGreen(U_I((unsigned)&full_data_144a4f0), U_I((unsigned)&full_data_144a4f4), 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a680);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x40ce, 0x40cc);
    d2_SetForm(win, 2);
    b1 = d2_GetWinObj(win, 2, io_PrintText(0x40cf, 4));
    b2 = d2_GetWinObj(win, 2, io_PrintText(0x40d0, 6));
    b3 = d2_GetWinObj(win, 2, io_PrintText(0x40d1, 8));
    b4 = d2_GetWinObj(win, 2, io_PrintText(0x40d2));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b4) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4);
                } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 6) * 2;
                } else if (U_I((unsigned)&ai_Unk5fe710) == b3) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 8) * 3;
                } else {
                    goto pump;
                }
                if (v != 0) {
                    out[0] = v;
                    out[1] = amt[v];
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
        }
pump:
        if (sv_RunFrame(ai_Unk585750, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
