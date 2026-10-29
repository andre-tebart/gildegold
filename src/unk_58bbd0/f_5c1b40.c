/* 0x5c1b40, the seven-button special-amount menu "special_amt1" 0x62d224 (820 B): the texts 0x599 / 0x58d /
   0x590 / 0x58c / 0x58a / 0x58f / 0x58b, the image order 8, 6, 4, 5, 1, 2, 3 (the images 0x143ec88), the
   captions 0x5bd0c0 (200) and the callback param_1 (called with the order value of the clicked button);
   0x4758f0 (win) at the end */

int ai_Unk5c1b40(int param_1, int param_2, int param_3)
{
    int win;
    int i;
    int found;
    int ctrl;
    int btn[14];

    btn[7] = 8;
    btn[8] = 6;
    btn[9] = 4;
    btn[10] = 5;
    btn[0xb] = 1;
    btn[0xc] = 2;
    btn[0xd] = 3;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d224);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    if (param_2 == 0)
        param_2 = 0xa2;
    io_PrintText(param_2);
    d2_SetForm(win, 1);
    btn[0] = d2_WinOpenPic(0x10f, 0x25, 0x599, U_I((unsigned)&d2_LastWindow));
    btn[1] = d2_WinOpenPic(0x10f, 0x93, 0x58d, U_I((unsigned)&d2_LastWindow));
    btn[2] = d2_WinOpenPic(0x69, 0xab, 0x590, U_I((unsigned)&d2_LastWindow));
    btn[3] = d2_WinOpenPic(0x1b5, 0xab, 0x58c, U_I((unsigned)&d2_LastWindow));
    btn[4] = d2_WinOpenPic(0x45, 0x125, 0x58a, U_I((unsigned)&d2_LastWindow));
    btn[5] = d2_WinOpenPic(0x10f, 0x125, 0x58f, U_I((unsigned)&d2_LastWindow));
    btn[6] = d2_WinOpenPic(0x1d9, 0x125, 0x58b, U_I((unsigned)&d2_LastWindow));
    for (i = 0; i < 7; i++) {
        if (i != 0 || param_3 != 0) {
            ctrl = btn[i];
            U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 3;
        }
        ctrl = btn[i];
        *(unsigned short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x74) = 0x55;
        ai_Unk5bd0c0(U_I((unsigned)&full_data_143ec88 + btn[i + 7] * 4),
                     (*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x10) - *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 4)) + 0x20,
                     (*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x12) - *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 6)) - 0x19,
                     200, 0x55);
    }
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            found = -1;
            for (i = 0; i < 7; i++) {
                if (btn[i] == U_I((unsigned)&ai_Unk5fe710)) {
                    found = i;
                    break;
                }
            }
            if (param_1 != 0 && found >= 0 && found < 7) {
                d2_SetViewEnabled(win, 0);
                ((void (*)(int))param_1)(btn[found + 7]);
                d2_SetViewEnabled(win, 1);
            }
        }
        if (sv_RunFrame(ai_Unk5c1b40, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    return 0;
}
