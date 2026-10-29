/* 0x5c2410, the law book info box "Gesetze\Gesetzbuch_Info" 0x62d2a4 (176 B): the caption 0x515960
   (param_1 + 1, param_1, param_2) and the text 0x515960 (param_1 + 2), the pump 0x5309a0 (this, 0x677c7)
   and 0x4758f0 (win) */

void ai_Unk5c2410(int param_1, int param_2)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2a4);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(param_1 + 1, param_1, param_2);
    d2_SetForm(win, 2);
    io_PrintText(param_1 + 2);
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk5c2410, 0x677c7) != 0);
    d2_CloseView(win);
}
