/* 0x599e00, one goods row of the city info window (240 B): the three labels 0x4731d0 (x, y, 0x525, screen),
   (x + 2, y + 2, param_4 + 0xdf, screen) and (x + 3, y + 0x35, 0x746, screen); the second control is marked
   owned (0xf8f080 + 0x4c); the price 0x5089c0 (param_4, param_5) formatted by 0x508a80 (amount, param_5,
   text) and 0x473650 (x + 7, y + 0x3a, text, screen) puts the value label (width 0x24, height 0x54, flag at
   +0x60); returns the first control id */

int ai_Unk599e00(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    char text[0x40];
    int id;

    d2_WinOpenPic(param_1, param_2, 0x525, param_3);
    id = d2_WinOpenPic(param_1 + 2, param_2 + 2, param_4 + 0xdf, param_3);
    d2_WinOpenPic(param_1 + 3, param_2 + 0x35, 0x746, param_3);
    U_I(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    gm_Unk508a80((int)gm_GoodsPrice(param_4, param_5), param_5, text);
    id = d2_CreateWinText(param_1 + 7, param_2 + 0x3a, text, U_I((unsigned)&d2_LastWindow));
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = 0x24;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x74 + id * 0x2ec) = 0x54;
    U_I(U_I((unsigned)&d2_Objs) + 0x60 + id * 0x2ec) = 1;
    return id;
}
