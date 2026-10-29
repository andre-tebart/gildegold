/* 0x58f290, the third column display (637 B, ROUGH): as 0x58e440 but the row stride is (the row height + 0x23)
   and the entry side comes from the record param_5 (+0): 0x1db -> 1, 0x1dc -> 2, else 3; the title controls
   0x6f4f10 (0x4731d0 (0, 0, 0x743)), their texts 0x469250 (id + 0x741), the positions (width - the icon width)
   / 2 and (the row height + 0x23) * i; the visibility 0x46cda0 (the screen 0xf8f124 + scr * 0x3c4); the rows
   0x6f4f60 through 0x58e6a0 (the offsets +8 / +0xc of the row); with param_3 the entry texts 0x476c00
   (0x475980, the goods + 0xdf) after 0x476c70 */

void ai_Unk58f290(int param_1, int param_2, int param_3, int param_4, short *param_5)
{
    int *p;
    int *q;
    int scr;
    int h;
    int i;
    int n;
    int side;

    scr = U_I(((unsigned)&d2_Views + 0x4) + (param_2 + param_1 * 0xab) * 4);
    h = *(short *)((unsigned)&ai_Unkf8f128 + scr * 0x3c4);
    d2_SetForm(param_1, param_2);
    for (p = (int *)(unsigned)&full_data_6f4f10; p < (int *)(unsigned)((unsigned)full_data_6f4f10 + 0x50); p += 5)
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
            p[-2] = (*(short *)(U_I((unsigned)&d2_Pics) + 0x2624e) + 0x23) * i;
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
    if (*param_5 == 0x1db)
        side = 1;
    else if (*param_5 == 0x1dc)
        side = 2;
    else
        side = 3;
    i = 0;
    n = 0;
    q = (int *)(unsigned)&full_data_6f4f60;
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
        q[2] = *(int *)((unsigned)&full_data_6f4f00 + n * 0x28) + -0x44 + i * 0x44;
        q[3] = *(int *)((unsigned)&full_data_6f4f04 + n * 0x28);
        ai_Unk58e6a0(param_1, param_2, q, param_3, side, (unsigned char *)param_4);
        q += 0xe;
    } while (q < (int *)(unsigned)((unsigned)full_data_6f4f60 + 0x380));
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
