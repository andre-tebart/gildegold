/* 0x5c1a80, the special-amount info box "special_amtinfo" 0x62d244 (240 B): the caption "%s: %s (%A %Z)"
   (0x62d234) over param_1 + 0x253, the two texts param_1 * 2 + 0x134b / 0x134c, the pump 0x5309a0 (this,
   0x657c7) and 0x4758f0 (win) */

int ai_Unk5c1a80(int param_1)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d244);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText((int)(unsigned)&full_data_62d234, param_1 + 0x253);
    d2_SetForm(win, 1);
    io_PrintText(param_1 * 2 + 0x134b);
    io_PrintText(param_1 * 2 + 0x134c);
    U_I((unsigned)&d2_ClickedId) = -1;
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk5c1a80, 0x657c7) != 0);
    d2_CloseView(win);
    return 0;
}
