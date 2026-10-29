/* 0x5be720, one compact row of the special-amount list (416 B): the control 0x4731d0 (0x5da), the icon, the
   four texts, the 0x46a960 slider (0x442) with 0x50c550, the 0x6d3 page mark of the winner +0x210, the
   window list +0x4c / +0x1c0 / +0x2e8 and the icon 0x54 of the kinds 5 / 6 / 7 at x - 0x20 */

int ai_Unk5be720(int param_1, int param_2, int param_3, int *param_4)
{
    int ix, kind;
    char *chr;

    ix = *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) / 2 + param_1;
    param_4[1] = d2_WinOpenPic(param_1, param_2, 0x5da, U_I((unsigned)&d2_LastWindow));
    param_4[2] = -1;
    if (param_4[0] == 0) {
        d2_WinOpenPic((ix - *(short *)(U_I((unsigned)&d2_Pics) + 0x1a2ac) / 2) - 3, param_2 + 1, 0x4fb,
                     U_I((unsigned)&d2_LastWindow));
    } else {
        chr = (char *)param_4[0];
        param_4[6] = d2_WinOpenPic((ix - *(short *)(U_I((unsigned)&d2_Pics) + 0x50 + U_I(chr + 0x18c) * 0x54) / 2) - 3,
                                  param_2 + 1, U_I(chr + 0x18c), U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(param_1 + 3, param_2 + 3, param_4[7], U_I((unsigned)&d2_LastWindow));
        if (param_4[8] != 0)
            d2_WinOpenPic(param_1 + 3, param_2 + 3 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, param_4[8],
                         U_I((unsigned)&d2_LastWindow));
        if (param_4[9] != 0)
            d2_WinOpenPic(param_1 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10, param_2 + 3, param_4[9],
                         U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(param_1 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10,
                     param_2 + 3 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, param_4[10], U_I((unsigned)&d2_LastWindow));
        if (*(char *)(chr + 2) != 6) {
            param_4[2] = d2_WinOpenSlider(ix - 0x23, *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) + 2 + param_2, 0x40, 0,
                                      100, 0x75, 0x442, U_I((unsigned)&d2_LastWindow));
            if (*(char *)(chr + 2) == 7)
                d2_SetObjValue(param_4[2], 0, 100, 0);
            else
                d2_SetObjValue(param_4[2], 0, 100, (int)gm_Liking(U_W((unsigned)&ai_PlayerId), *(unsigned short *)chr, 1));
            d2_SetObjEnabled(param_4[2], 0);
        }
        if (U_I(chr + 0x210) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c))
            d2_WinOpenPic(param_1 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8), param_2, 0x6d3, U_I((unsigned)&d2_LastWindow));
        if (param_4[0xc] != 0) {
            U_I((unsigned)&d2_Objs + param_4[1] * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + param_4[1] * 0x2ec + 0x1c0) = 3;
            U_I((unsigned)&d2_Objs + param_4[1] * 0x2ec + 0x2e8) = param_4[0];
        }
        kind = *(char *)(chr + 2);
        if (kind == 5 || kind == 6 || kind == 7)
            d2_WinOpenPic(param_1 - 0x20, param_2, U_I(chr + 0x54), U_I((unsigned)&d2_LastWindow));
    }
    return 0;
}
