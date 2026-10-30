/* 0x5a6270, the city watch search window (519 B; a Ghidra-missed start, the callback of 0x5b2180 in 0x5a6480):
   the name param_2 (0 returns 0), the scan of the crew byte at 0x770d08, the window
   "LOCATIONS\STADTWACHE\STANDARD" with two controls from 0x515960 (0x1a70) / 0x475b80; the pump 0x5309a0
   (this, 0x677c7): a click on the first control sends the AIMSG type 0x65 (+8 the player's character, the clock
   0x770ae0 .. +0x34, 2 at +0x36, the name copied to +0x58, -1 at +0x38, the building +1 at +0x78, the 0x6f2310
   +2 at +0x7c and the up to six wanted ids (the live table 0x7603a0 .. 0x760420, byte +0x188) at +0x38) and
   0x4c2d20, 0x6f22a0 = 2; the second control sets 0x6f22a0 = 1; the exit 0x4758f0, returns 0 */

int ai_Unk5a6270(int param_1, char *param_2)
{
    int list[6];
    int win;
    int ctl1;
    int ctl2;
    int id;
    int n;
    int i;
    int e;
    int *tp;
    char msg[0xf4];

    if (param_2 == 0)
        return 0;
    for (i = 0; i < 0x300; i++) {
        if (*(char *)((unsigned)&ai_Selected + i * 0x21c) != 0)
            break;
    }
    if (i == 0x300) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_1440b38), 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\STADTWACHE\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    id = io_PrintText(0x1a70);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                *(unsigned char *)(msg + 4) = 0x65;
                U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                U_I(msg + 0x28) = U_I((unsigned)&ai_Time);
                U_I(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                U_I(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                U_W(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
                *(unsigned char *)(msg + 0x36) = 2;
                U_I(msg + 0x78) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
                U_I(msg + 0x7c) = U_I(U_I((unsigned)&sv_PanelShown) + 2);
                for (i = 0; param_2[i] != 0; i++)
                    *(char *)(msg + 0x58 + i) = param_2[i];
                *(char *)(msg + 0x58 + i) = 0;
                for (i = 0; i < 6; i++)
                    U_I(msg + 0x38 + i * 4) = -1;
                n = 0;
                for (tp = (int *)(unsigned)&ai_Unk7603a0; tp < (int *)(unsigned)((unsigned)ai_Unk7603a0V + 0x80) && n < 6; tp++) {
                    e = *tp;
                    if (e != 0 && *(char *)(e + 0x188) != 0) {
                        U_I(msg + 0x38 + n * 4) = U_I(e + 4);
                        n++;
                    }
                }
                if (n != 0) {
                    cm_SendLongMessage(msg);
                    snd_PlayFavourComment();
                }
                U_I((unsigned)&sv_PopRequest) = 2;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a6270, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    return 0;
}
