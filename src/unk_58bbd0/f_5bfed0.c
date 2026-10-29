/* 0x5bfed0, the page switch of the special-amount window (144 B): 0x475f70 (win, page), the saved +0x2e8
   control of the screen 0xf8f368 + screen * 0x3c4, the caption 0x515960 (0x602568) and 0x472d20
   (screen, 1); with param_3 != -1 the screen control is cleared and the 0x472270 (-1, -1, screen, screen,
   0x767) page follows; returns the saved control */

int ai_Unk5bfed0(int param_1, int param_2, int param_3)
{
    int saved;

    d2_SetForm(param_1, param_2);
    saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    io_PrintText((unsigned)"$C");
    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    if (param_3 != -1) {
        U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4) = 0;
        d2_SetForm(param_1, param_3);
        io_PrintText((unsigned)"$C");
        d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), U_I((unsigned)&d2_LastWindow), 0x767);
        d2_SetForm(param_1, param_2);
        return 0;
    }
    return saved;
}
