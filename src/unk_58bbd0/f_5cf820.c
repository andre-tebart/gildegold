/* 0x5cf820, the "panel_useobj" text window 0x62d010 (128 B): 0x4741f0 (0, 0, 0x62d010) page 1 with
   0x515960 (the "%s" 0x5f8318, param_1), 0x476f80 (0, 0), the pump 0x5309a0 (0x677c7) and the exit
   0x4758f0. */

int ai_Unk5cf820(int param_1)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d010);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText("%s", param_1);
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    while (sv_RunFrame(ai_Unk5cf820, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return 0;
}
