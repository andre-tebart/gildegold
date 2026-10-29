/* 0x5c2730, the second master letter window "berufe\meisterbrief_1" 0x62d2bc (124 B): the 0x5c26a0 body
   with the single caption 0x195e (0x52ea30 the screen, the pump 0x5309a0 (this, 0x677c7), 0x4758f0 win) */

void ai_Unk5c2730(void)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2bc);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x195e);
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk5c2730, 0x677c7) != 0);
    d2_CloseView(win);
}
