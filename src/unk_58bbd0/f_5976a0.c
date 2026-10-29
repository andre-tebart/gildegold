/* 0x5976a0, the tavern rules window "locations\Wirtshaus_WS_regeln" (210 B): 0x4741f0 / 0x475600 / 0x475f70
   (0) / 0x52ea30 / 0x476f80, the picture 0x472270 (-1, -1, the screen, the screen, 0x767), the frames
   0x475f70 (3 / 1 / 2) and the texts 0x515960 (0x1889 / 0x188a); the pump 0x5309a0 (this, 0x677c7); the exit
   0x4758f0 */

void d2_CreateScrollButtons(); int d2_RemoveWindowObjects();

void ai_Unk5976a0(void)
{
    int win;

    win = d2_OpenForm(0, 0, "locations\\Wirtshaus_WS_regeln");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(0xffffffff, 0xffffffff, U_I((unsigned)&d2_LastWindow), U_I((unsigned)&d2_LastWindow), 0x767);
    d2_SetForm(win, 1);
    io_PrintText(0x1889);
    d2_SetForm(win, 2);
    io_PrintText(0x188a);
    while (sv_RunFrame(ai_Unk5976a0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
}
