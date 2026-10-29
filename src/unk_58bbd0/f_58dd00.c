/* 0x58dd00, the column display of the production window (1058 B, ROUGH): the building record param_1 / the
   column set param_2 (the screen 0xfb1024 + (param_2 + param_1 * 0xab) * 4, the height 0xf8f128); the title
   controls 0x6f4f10 / 0x6f52f0 (0x4731d0 (0, 0, 0x743)) and their texts 0x469250 (id + 0x741), the positions
   (the width - the icon width) / 2 and (the row height + 0x16) * i (the second set + 0xb6); the visibility
   0x46cda0 (icon / text at the screen's +0xf124 / +0x126); the goods columns 0x6f4f60 / 0x6f58a0 laid out by
   the counts (i / the entry counts) through 0x58e6a0; then the same title pass for 0x6f52ec and the second grid
   (0x6f58ac), and with param_3 the texts of the entries 0x476c00 (0x475980 (id), the goods + 0xdf) after
   0x476c70 */

void ai_Unk58dd00(int param_1, int param_2, int param_3, int param_4)
{
    int *p;
    int *q;
    int scr;
    int h;
    int i;
    int n;
    int x;

    scr = U_I(((unsigned)&d2_Views + 0x4) + (param_2 + param_1 * 0xab) * 4);
    h = *(short *)((unsigned)&ai_Unkf8f128 + scr * 0x3c4);
    d2_SetForm(param_1, param_2);
    for (p = (int *)(unsigned)&full_data_6f4f10; p < (int *)(unsigned)((unsigned)full_data_6f4f10 + 0x50); p += 5)
        if (*p == -1)
            *p = d2_WinOpenPic(0, 0, 0x743, U_I((unsigned)&d2_LastWindow));
    d2_SetForm(param_1, param_2);
    for (p = (int *)(unsigned)&full_data_6f52f0; p < (int *)(unsigned)((unsigned)full_data_6f52f0 + 0x28); p += 5)
        if (*p == -1)
            *p = d2_WinOpenPic(0, 0, 0x743, U_I((unsigned)&d2_LastWindow));
    i = 0;
    for (p = (int *)(unsigned)&full_data_6f4f0c; p < (int *)(unsigned)((unsigned)full_data_6f4f0c + 0x50); p += 5, i++) {
        if (p[-1] != *p)
            d2_SetObjPic(p[1], *p + 0x741);
        n = *p;
        p[-1] = n;
        if (n > 1) {
            p[-3] = (h - *(short *)(U_I((unsigned)&d2_Pics) + 0x261a4 + n * 0x54)) / 2;
            p[-2] = (*(short *)(U_I((unsigned)&d2_Pics) + 0x2624e) + 0x16) * i;
        }
    }
    i = 0;
    for (p = (int *)(unsigned)&full_data_6f52ec; p < (int *)(unsigned)((unsigned)full_data_6f52ec + 0x28); p += 5, i++) {
        if (p[-1] != *p)
            d2_SetObjPic(p[1], *p + 0x741);
        n = *p;
        p[-1] = n;
        if (n > 1) {
            p[-3] = (h - *(short *)(U_I((unsigned)&d2_Pics) + 0x261a4 + n * 0x54)) / 2;
            p[-2] = (*(short *)(U_I((unsigned)&d2_Pics) + 0x2624e) + 0x16) * i + 0xb6;
        }
    }
    d2_SetForm(param_1, param_2);
    for (p = (int *)(unsigned)&full_data_6f4f10; p < (int *)(unsigned)((unsigned)full_data_6f4f10 + 0x50); p += 5) {
        n = *p;
        if (p[-2] < 2) {
            U_I(U_I((unsigned)&d2_Objs) + 0x38 + n * 0x2ec) = 1;
        } else {
            U_I(U_I((unsigned)&d2_Objs) + 0x38 + n * 0x2ec) = 0;
            d2_MoveObj(*(short *)(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4) + (short)p[-4],
                         *(short *)(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4 + 2) + (short)p[-3], n);
        }
    }
    i = 0;
    n = 0;
    q = (int *)(unsigned)&full_data_6f4f6c;
    do {
        i++;
        p = (int *)((unsigned)&full_data_6f4f08 + n * 0x14);
        if (p[0] < i) {
            i = 1;
            do {
                p += 5;
                n++;
            } while (*p < 1);
        }
        q[-1] = *(int *)((unsigned)&full_data_6f4f00 + n * 0x28) + -0x44 + i * 0x44;
        *q = *(int *)((unsigned)&full_data_6f4f04 + n * 0x28);
        ai_Unk58e6a0(param_1, param_2, q - 3, param_3, 2, param_4);
        q += 0xe;
    } while (q < (int *)(unsigned)((unsigned)full_data_6f4f6c + 0x380));
    d2_SetForm(param_1, param_2);
    for (p = (int *)(unsigned)&full_data_6f52ec; p < (int *)(unsigned)((unsigned)full_data_6f52ec + 0x28); p += 5) {
        if (p[-1] != *p)
            d2_SetObjPic(p[1], *p + 0x741);
        p[-1] = *p;
        if (*p < 2) {
            U_I(U_I((unsigned)&d2_Objs) + 0x38 + p[1] * 0x2ec) = 1;
        } else {
            U_I(U_I((unsigned)&d2_Objs) + 0x38 + p[1] * 0x2ec) = 0;
            d2_MoveObj((short)p[-3] + *(short *)(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4),
                         (short)p[-2] + *(short *)(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4 + 2), p[1]);
        }
    }
    i = 0;
    n = 0;
    q = (int *)(unsigned)&full_data_6f58ac;
    do {
        i++;
        p = (int *)((unsigned)&full_data_6f52e8 + n * 0x14);
        if (p[0] < i) {
            i = 1;
            do {
                p += 5;
                n++;
            } while (*p < 1);
        }
        q[-1] = *(int *)((unsigned)&full_data_6f52e0 + n * 0x28) + -0x44 + i * 0x44;
        *q = *(int *)((unsigned)&full_data_6f52e4 + n * 0x28);
        ai_Unk58e6a0(param_1, param_2, q - 3, param_3, 2, param_4);
        q += 0xe;
    } while (q < (int *)(unsigned)((unsigned)full_data_6f58ac + 0x1c0));
    if (param_3 != 0) {
        d2_ClearPointerItems();
        for (p = (int *)(unsigned)&full_data_6f4f70; p < (int *)(unsigned)((unsigned)full_data_6f4f70 + 0x380); p += 0xe)
            if (*p != -1)
                d2_SetPointerItem(p[-4] + 0xdf, d2_GetObjValue(*p));
        for (p = (int *)(unsigned)&full_data_6f58b0; p < (int *)(unsigned)((unsigned)full_data_6f58b0 + 0x1c0); p += 0xe)
            if (*p != -1)
                d2_SetPointerItem(p[-4] + 0xdf, d2_GetObjValue(*p));
    }
}
