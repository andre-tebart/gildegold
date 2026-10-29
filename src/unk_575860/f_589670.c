/* 0x589670, the item-slot window (1066 B, ROUGH SKELETON: the window, every call and the widget ids are on the
   disassembly; the slot grid arithmetic is summarised): 0x589450 () first; the window 0x62aa38 with 0x475600,
   0x475f70 (win, 0), the sound 0x52ea30, 0x476f80 (0, 0), the section 2, the text 0x205b, the section 0; the
   rectangles 0x475ff0 / 0x52e860 (0x214, 0x168, 0x38, ...) and the buttons 0x52e350 (0, 2, the list); 0x476b70 ();
   the drag window 0xfa5aa8 in 0x46c600, 0x472d20 (U_I (0x6ed938), 1). The slots: 0x4731d0 (0x10, the slot + 4),
   0x502020 (the id) / 0x4fe980, 0x4731d0 (0x60, i + 0x427), 0x473650 (0x9c, ...), 0x473650 (0x60, ...,
   0x1442334), 0x4731d0 (0x128, ...), 0x473650 (0x164, ...), 0x589400 (the slot 0x6f5ec0 + i * 0xc). The pump
   loop 0x5309a0 (this window, 0x657c7): 0x589570 (the selected slot) when 0xfa5aa8 is not -1, 0x46c6c0,
   0x475e40 (the buttons), 0x5894d0 (); the exit 0x46c600 and 0x4758f0 (the window) */

int d2_GetViewWin(); int sv_CreateFormSlider(); void d2_RadioClear(int g); int d2_RemoveWindowObjects(); void d2_MenuNavigate(int g);

void ai_Unk589670(void)
{
    char line[0x100];
    int win;
    int i;
    int id;
    int e;
    int p;

    ai_Unk589450();
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62aa38);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    io_PrintText(0x205b);
    d2_SetForm(win, 0);
    d2_ClearInput();
    for (i = 0; i < 0xb; i++) {
        p = d2_WinOpenPic(0x10, 0, (char *)(i + 4), U_I((unsigned)&d2_LastWindow));
        id = *(int *)((unsigned)&full_data_6f5ec4 + i * 0xc);
        if (id != -1)
            e = gm_FindBuildingById(id);
        else
            e = 0;
        d2_WinOpenPic(0x60, *(unsigned char *)((unsigned)&full_data_6f5ec0 + i * 0xc) + 0x427, (char *)win, U_I((unsigned)&d2_LastWindow));
        d2_CreateWinText(0x9c, i, line, U_I((unsigned)&ai_Unk143c860));
        d2_CreateWinText(0x60, i, line, U_I((unsigned)&full_data_1442334));
        d2_WinOpenPic(0x128, 0, (char *)(i + 0xdf), U_I((unsigned)&d2_LastWindow));
        d2_CreateWinText(0x164, i, line, U_I((unsigned)&full_data_1442334));
        (void)e;
        (void)p;
    }
    while (sv_RunFrame(ai_Unk589670, 0x657c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (*(int *)((unsigned)&d2_RadioSel) != -1)
            ai_Unk589570(*(int *)((unsigned)&d2_RadioSel));
        d2_MenuNavigate(0);
        d2_SetObjEnabled(0, 0);
        ai_Unk5894d0();
    }
    d2_RadioClear(0);
    d2_CloseView(win);
}
