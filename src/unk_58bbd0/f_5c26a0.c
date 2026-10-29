/* 0x5c26a0, the master letter window "berufe\meisterbrief_1" 0x62d2bc (176 B): 0x52ea30 (screen), the
   caption 0x515960 (0x1949 / 0x194e), the pump 0x5309a0 (this, 0x677c7) and 0x4758f0 (win) */

void ai_Unk5c26a0(void)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2bc);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1949);
    io_PrintText(0x194e);
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk5c26a0, 0x677c7) != 0);
    d2_CloseView(win);
}
