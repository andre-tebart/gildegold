/* 0x5be3b0, one row of the special-amount list at (x, y) (464 B): the control 0x4731d0 (0x5da), the icon
   (the id +0x18c and its glyph width), four texts 0x4731d0, the two captions 0x5176d0 (0x62d1fc / 0x62d1f4
   and 0x62d1ec) with 0x5bd0c0, the 0x46a960 slider (0x442) with the 0x50c550 percent for the kinds other
   than 6 and the window list fields +0x4c / +0x1c0 / +0x2e8; the kinds 5 / 6 / 7 get the icon 0x54 at
   x - 0x20 */

int ai_AmountRow(int param_1, int param_2, int param_3, int param_4, int *param_5, int param_6)
{
    int x, y, ix, iy, t, kind;
    char *chr;
    char buf[256];

    x = param_1;
    y = param_2;
    ix = *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) / 2 + x;
    iy = y + 0x1c;
    param_5[1] = d2_WinOpenPic(x, iy, 0x5da, U_I((unsigned)&d2_LastWindow));
    param_5[2] = -1;
    if (param_5[0] != 0) {
        chr = (char *)param_5[0];
        param_5[6] = d2_WinOpenPic((ix - *(short *)(U_I((unsigned)&d2_Pics) + 0x50 + U_I(chr + 0x18c) * 0x54) / 2) - 3,
                                  y + 0x1d, U_I(chr + 0x18c), U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(x + 3, y + 0x1f, param_5[7], U_I((unsigned)&d2_LastWindow));
        if (param_5[8] != 0)
            d2_WinOpenPic(x + 3, y + 0x1f + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, param_5[8],
                         U_I((unsigned)&d2_LastWindow));
        if (param_5[9] != 0)
            d2_WinOpenPic(x + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10, y + 0x1f, param_5[9],
                         U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(x + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10,
                     y + 0x1f + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, param_5[10], U_I((unsigned)&d2_LastWindow));
        if (*(unsigned char *)(chr + 0xd) == 0 || param_6 == 0)
            txt_Format(buf, (char *)(unsigned)&full_data_62d1fc, *(unsigned short *)chr);
        else
            txt_Format(buf, (char *)(unsigned)&full_data_62d1f4,
                         (-(*(char *)(chr + 9) != 0) & 10) + 0x114 + *(unsigned char *)(chr + 0xd),
                         *(unsigned short *)chr);
        ai_Unk5bd0c0((int)buf, ix, param_5[5], 0xca, param_4);
        txt_Format(buf, (char *)(unsigned)&full_data_62d1ec, *(unsigned short *)chr);
        ai_Unk5bd0c0((int)buf, ix, param_5[5] + 0xe, 0xca, param_4);
        if (*(char *)(chr + 2) != 6) {
            param_5[2] = d2_WinOpenSlider(ix - 0x23, *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) + 0x1e + y, 0x40, 0,
                                      100, 0x75, 0x442, U_I((unsigned)&d2_LastWindow));
            if (*(char *)(chr + 2) == 7)
                d2_SetObjValue(param_5[2], 0, 100, 0);
            else
                d2_SetObjValue(param_5[2], 0, 100, (int)gm_Liking(U_W((unsigned)&ai_PlayerId), *(unsigned short *)chr, 1));
            d2_SetObjEnabled(param_5[2], 0);
        }
        if (param_5[0xc] != 0) {
            U_I((unsigned)&d2_Objs + param_5[1] * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + param_5[1] * 0x2ec + 0x1c0) = 3;
            U_I((unsigned)&d2_Objs + param_5[1] * 0x2ec + 0x2e8) = param_5[0];
        }
        kind = *(char *)(chr + 2);
        if (kind != 5 && kind != 6 && kind != 7)
            return 0;
        t = U_I(chr + 0x54);
        x = x - 0x20;
    } else {
        iy = y + 0x1d;
        t = 0x4fb;
        x = (ix - *(short *)(U_I((unsigned)&d2_Pics) + 0x1a2ac) / 2) - 3;
    }
    d2_WinOpenPic(x, iy, t, U_I((unsigned)&d2_LastWindow));
    return 0;
}
