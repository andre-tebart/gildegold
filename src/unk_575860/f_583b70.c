/* 0x583b70, opens the graveyard detail window (209 B): w = the window list (7 the selected entry record), title
   = its title; 0 without a selected entry, else "Locations_Friedhof_RentAGrave2" (0x62a59c) with 0x476f80
   (0, 0) / 0x475600, the sound 0x52ea30, the texts 0x62a578 ("%C Z %s", title) and 0x602568, the sections
   1 / 2 / 3 / 5 / 4 (0x6ed938 kept in w[1] / w[2] / w[4] / w[5] / w[6]), then 0x583700 (w); 1 */

int ai_Unk583b70(int *w, char *title)
{
    int win;

    if (w[7] == 0)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a59c);
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(w[0]);
    d2_SetForm(w[0], 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(w[0], 1);
    w[1] = U_I((unsigned)&d2_LastWindow);
    io_PrintText((char *)(unsigned)&full_data_62a578, title);
    d2_SetForm(w[0], 2);
    w[2] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 3);
    w[4] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 5);
    w[5] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 4);
    w[6] = U_I((unsigned)&d2_LastWindow);
    io_PrintText((char *)(unsigned)"$C");
    ai_Unk583700(w);
    return 1;
}
