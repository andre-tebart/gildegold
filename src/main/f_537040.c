/* 0x537040, the opponent selection ("menu\choose_opponents"): n rows (0x46a960 list boxes "<name> <letter>") with the entries of 0x75e160, the ok button (text 0x7d); the chosen entries are stored back
   in 0x75e160; ROUGH (the row height is a lost float) */

void main_Unk537040(int n)
{
    int f, h, y, i, ok, run;
    int ids[16];
    char buf[256];

    f = d2_OpenForm(0, 0, "menu\\choose_opponents");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 1);
    io_PrintText(0x42c5);
    d2_SetForm(f, 3);
    h = (int)0.0f;
    i = 0;
    y = ((int)*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 10) - h * n) / 2 + 0xe;
    if (0 < n) {
        do {
            crt_Sprintf(buf, "%s %c", U_I((unsigned)&full_data_144acd8), i + 0x41);
            ids[i] = d2_WinOpenSlider(0x80, y, 200, 0, 100, 0x7b, 0x802, d2_LastWindow);
            d2_SetObjValue(ids[i], 0, 4, U_I((unsigned)&ai_Opponents + i * 4));
            d2_SetSliderFlags(ids[i], 0x82);
            d2_SetLabel(ids[i], buf);
            d2_SetList(ids[i], 5, U_I((unsigned)&full_data_144acdc), U_I((unsigned)&full_data_144ace0), U_I((unsigned)&full_data_144ace4), U_I((unsigned)&full_data_144ace8), U_I((unsigned)&full_data_144acec));
            i++;
            y += h;
        } while (i < n);
    }
    d2_SetForm(f, 2);
    ok = d2_GetWinObj(f, 2, io_PrintText(0x7d));
    run = sv_RunFrame((int)main_Unk537040, 0xc6);
    while (run != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1 || (U_I((unsigned)&d2_ClickedId) == 0 && ai_Unk5fe710 == ok))
            U_I((unsigned)&sv_PopRequest) = 1;
        run = sv_RunFrame((int)main_Unk537040, 0xc6);
    }
    i = 0;
    if (0 < n) {
        do {
            U_I((unsigned)&ai_Opponents + i * 4) = d2_GetObjValue(ids[i]);
            i++;
        } while (i < n);
    }
    d2_CloseView(f);
}
