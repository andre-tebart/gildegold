/* 0x5c1130, the special-amount menu "special_amt1" 0x62d224 (2384 B, ROUGH): the nine buttons of the table
   (the order 0, 8, 6, 4, 5, 1, 2, 3, 7 and the texts 0x589 / 0x599 / 0x58d / 0x590 / 0x58c / 0x58a / 0x58f /
   0x58b / 0x58e), the images 0x143ec88, the captions 0x5bd0c0 (200), the list flag of the last button
   0x4e86e0, the click dispatch on the buttons (the list 0x5bea50 with the page index) and the detail
   0x5c0970 (param_1, param_2, -1, param_4); 0x4758f0 (win) and the chosen value at the end */

int ai_AmountMenu(int param_1, int param_2, int param_3, int param_4)
{
    int win;
    int text;
    int r;
    int i;
    int ctrl;
    int btn[9];
    int order[9];
    int list;

    order[0] = 0;
    order[1] = 8;
    order[2] = 6;
    order[3] = 4;
    order[4] = 5;
    order[5] = 1;
    order[6] = 2;
    order[7] = 3;
    order[8] = 7;
    r = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d224);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    text = param_2 == 0 ? 0xa1 : param_2;
    io_PrintText(text);
    d2_SetForm(win, 3);
    list = io_PrintText(0x1687);
    ctrl = d2_GetWinObj(win, 3, list);
    d2_SetForm(win, 1);
    btn[0] = d2_WinOpenPic(0x45, 0x3c, 0x589, U_I((unsigned)&d2_LastWindow));
    btn[1] = d2_WinOpenPic(0x10f, 0x25, 0x599, U_I((unsigned)&d2_LastWindow));
    btn[2] = d2_WinOpenPic(0x10f, 0x93, 0x58d, U_I((unsigned)&d2_LastWindow));
    btn[3] = d2_WinOpenPic(0x69, 0xab, 0x590, U_I((unsigned)&d2_LastWindow));
    btn[4] = d2_WinOpenPic(0x1b5, 0xab, 0x58c, U_I((unsigned)&d2_LastWindow));
    btn[5] = d2_WinOpenPic(0x45, 0x125, 0x58a, U_I((unsigned)&d2_LastWindow));
    btn[6] = d2_WinOpenPic(0x10f, 0x125, 0x58f, U_I((unsigned)&d2_LastWindow));
    btn[7] = d2_WinOpenPic(0x1d9, 0x125, 0x58b, U_I((unsigned)&d2_LastWindow));
    btn[8] = d2_WinOpenPic(0x1d9, 0x3c, 0x58e, U_I((unsigned)&d2_LastWindow));
    for (i = 0; i < 9; i++) {
        if (i != 1 || U_I(param_1 + 0x18) == 0) {
            U_I((unsigned)&d2_Objs + btn[i] * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + btn[i] * 0x2ec + 0x1c0) = 3;
        }
        *(unsigned short *)((unsigned)&d2_Objs + btn[i] * 0x2ec + 0x74) = 0x55;
        ai_Unk5bd0c0(U_I((unsigned)&full_data_143ec88 + order[i] * 4),
                     (*(short *)((unsigned)&d2_Objs + btn[i] * 0x2ec + 0x10) - *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 4)) + 0x20,
                     (*(short *)((unsigned)&d2_Objs + btn[i] * 0x2ec + 0x12) - *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 6)) - 0x19,
                     200, 0x55);
    }
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        r = 0;
        if (amt_HasColumn7Holder() == 0) {
            U_I((unsigned)&d2_Objs + btn[8] * 0x2ec + 0x4c) = 0;
            U_I((unsigned)&d2_Objs + btn[8] * 0x2ec + 0x1c0) = 0;
        } else {
            U_I((unsigned)&d2_Objs + btn[8] * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + btn[8] * 0x2ec + 0x1c0) = 3;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 9; i++) {
                if (btn[i] == U_I((unsigned)&ai_Unk5fe710)) {
                    d2_SetViewEnabled(win, 0);
                    r = ai_Unk5bea50(order[i], param_1, param_2, 0, param_4);
                    d2_SetViewEnabled(win, 1);
                    goto done;
                }
            }
            if (U_I((unsigned)&ai_Unk5fe710) == ctrl) {
                d2_SetViewEnabled(win, 0);
                r = ai_Unk5c0970(param_1, param_2, -1, param_4);
                d2_SetViewEnabled(win, 1);
            }
        }
done:
        if (r != 0 && param_4 == 0 && (*(unsigned char *)(param_1 + 0x14) & 4) == 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_AmountMenu, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    return r;
}
