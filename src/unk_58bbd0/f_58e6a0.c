/* 0x58e6a0, one goods entry of the production window (1936 B, ROUGH): the row param_3 (the id +0, the x / y
   +8 / +0xc, the state +0x20 / +0x24, the controls +0x10 .. +0x24): a zero id frees the five controls
   (0x46e710 / 0x472ce0); else the slider 0x46a960 (+0x20) and the icon 0x469ee0 / 0x4731d0 (+0x6, the goods
   + 0xdf), the two labels 0x4731d0 / 0x473650 (0x746 / 0x747 at +0x3a / +0x49), the amount texts 0x5089c0 /
  0x508a80 into 0x473650 / 0x4734d0 at +0x40 / +0x49, the carrier 0x53e400 (2, 0, 4 / 3, 3) and the time ratio
  (+0x1e4), the slider values 0x475cb0 (the available / the max), the text 0x5de652 ("%s (%li) %s" 0x62b098),
   the control positions 0x46cda0 and the window +0x1c0 = 0x22, +0x1cc = 4, +0x1d0 = 0. ROUGH: the layout
   offsets are transcribed from the pseudocode */

void ai_Unk58e6a0(int param_1, int param_2, int *param_3, int param_4, unsigned char param_5, unsigned char *param_6)
{
    char *p;
    char text[32];
    int scr;
    int n;
    int a;

    d2_SetForm(param_1, param_2);
    scr = U_I(((unsigned)&d2_Views + 0x4) + (param_2 + param_1 * 0xab) * 4);
    if (param_3[0] == 0) {
        for (n = 4; n <= 9; n++)
            if (param_3[n] != -1) {
                if (n == 9)
                    d2_RemoveWindow(param_3[n]);
                else
                    d2_RemoveObj(param_3[n]);
                param_3[n] = -1;
            }
        return;
    }
    if (param_3[8] == -1)
        param_3[8] = d2_WinOpenSlider(param_3[2] + 6, param_3[3], 0x30, 0, 100, 0x6a, 0x3d, U_I((unsigned)&d2_LastWindow));
    if (param_3[4] == -1) {
        if (param_4 == 0) {
            param_3[4] = d2_WinOpenPic((short)param_3[2] + 0xf, (short)param_3[3] + 6, *param_3 + 0xdf,
                                      U_I((unsigned)&d2_LastWindow));
            U_I(U_I((unsigned)&d2_Objs) + 0x4c + param_3[4] * 0x2ec) = 1;
        } else {
            a = 0x8a;
            if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0)
                a = 0xca;
            param_3[4] = d2_WinOpenInputIcon(param_3[2] + 0xf, param_3[3] + 6, *param_3 + 0xdf, a, U_I((unsigned)&d2_LastWindow));
            d2_SetObjFont(param_3[4], 0x54);
        }
    }
    if (param_3[5] == -1) {
        if ((param_5 & 1) != 0)
            param_3[5] = d2_WinOpenPic((short)param_3[2] + 0xf, (short)param_3[3] + 0x3a, 0x747, U_I((unsigned)&d2_LastWindow));
        else if ((param_5 & 2) != 0)
            param_3[5] = d2_WinOpenPic((short)param_3[2] + 0xf, (short)param_3[3] + 0x3a, 0x746, U_I((unsigned)&d2_LastWindow));
    }
    if ((param_5 & 2) != 0) {
        /* the amount text of the carrier's goods (0x5089c0 / 0x508a80) */
        gm_GoodsPrice(*param_3, param_6[0x65]);
        ((int (*)(int))gm_Unk508a80)(U_I((unsigned)&ai_OwnPlayer));
        if (param_3[6] == -1)
            param_3[6] = d2_CreateWinText((short)param_3[2] + 0x10, (short)param_3[3] + 0x40, text, U_I((unsigned)&d2_LastWindow));
        else
            d2_SetObjText(param_3[6], text);
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + param_3[6] * 0x2ec) = 0x2d;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x74 + param_3[6] * 0x2ec) = 0x54;
        U_I(U_I((unsigned)&d2_Objs) + 0x60 + param_3[6] * 0x2ec) = 1;
    }
    if ((param_5 & 1) != 0) {
        gm_GoodsPrice(*param_3, *param_6 == 0x47 ? param_6[0x65] : U_I((unsigned)&ai_OwnPlayer));
        ((int (*)(int))gm_Unk508a80)(U_I((unsigned)&ai_OwnPlayer));
        if (param_3[7] == -1)
            param_3[7] = d2_CreateWinText((short)param_3[2] + 0x10, (short)param_3[3] + 0x49, text, U_I((unsigned)&d2_LastWindow));
        else
            d2_SetObjText(param_3[7], text);
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + param_3[7] * 0x2ec) = 0x2d;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x74 + param_3[7] * 0x2ec) = 0x54;
        U_I(U_I((unsigned)&d2_Objs) + 0x60 + param_3[7] * 0x2ec) = 1;
    }
    p = (char *)he_FindEvent(2, 0, 4, 3, U_I(param_6 + 1));
    while (p != 0 && *(unsigned short *)(p + 0xac) != *param_3)
        p = (char *)he_NextEvent();
    if (p == 0) {
        p = (char *)he_FindEvent(2, 0, 3, 3, U_I(param_6 + 1));
        while (p != 0 && *(unsigned short *)(p + 0xac) != *param_3)
            p = (char *)he_NextEvent();
    }
    if (p != 0 && (U_I(p + 0x14) != 0 || U_I(p + 0x18) != 0)) {
        float r;

        if (*p == 4)
            r = *(float *)(p + 0xb4) / *(float *)(p + 0xb8);
        else if (*p == 3)
            r = (float)U_I(p + 0xb0) / *(float *)(p + 0xb4);
        else
            r = 0.0f;
        *(float *)(U_I((unsigned)&d2_Objs) + 0x1e4 + param_3[4] * 0x2ec) = 1.0f - r;
    } else if (param_3[4] != -1) {
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1e4 + param_3[4] * 0x2ec) = 0;
    }
    if (param_4 == 0) {
        U_I(U_I((unsigned)&d2_Objs) + 0x50 + param_3[4] * 0x2ec) = param_3[0xc] == 0;
        d2_SetObjValue(param_3[8], 0, param_3[0xd], param_3[0xc], param_3[0xb]);
    } else {
        d2_SetObjValue(param_3[4], 0, param_3[0xc] - param_3[0xb], param_3[0xc], 0);
        U_I(U_I((unsigned)&d2_Objs) + 0x50 + param_3[4] * 0x2ec) = param_3[0xc] == 0;
        d2_SetObjValue(param_3[8], 0, param_3[0xd], param_3[0xc], param_3[0xb]);
    }
    U_I(U_I((unsigned)&d2_Objs) + 0x1c0 + param_3[4] * 0x2ec) = 0x22;
    U_I(U_I((unsigned)&d2_Objs) + 0x1cc + param_3[4] * 0x2ec) = 4;
    U_I(U_I((unsigned)&d2_Objs) + 0x1d0 + param_3[4] * 0x2ec) = 0;
    sprintf((char *)(U_I((unsigned)&d2_Objs) + 0xbc + param_3[4] * 0x2ec), (char *)(unsigned)&full_data_62b098,
                 U_I((unsigned)&ai_Unk143c860 + *param_3 * 8), param_3[0xc], U_I((unsigned)&ai_Unk144a160));
    /* ROUGH: the control positions 0x46cda0 of the icon, the two labels, the two amounts and the slider */
    n = U_I(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4);
    a = *(short *)(((unsigned)&d2_Wins + 0x4) + scr * 0x3c4 + 2);
    if (param_3[4] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 0xf, a + (short)param_3[3] + 6, param_3[4]);
    if (param_3[5] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 0xf, a + (short)param_3[3] + 0x3a, param_3[5]);
    if (param_3[6] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 0x12, a + (short)param_3[3] + 0x3f, param_3[6]);
    if (param_3[7] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 0x12, a + (short)param_3[3] + 0x49, param_3[7]);
    if (param_3[8] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 6, a + (short)param_3[3], param_3[8]);
    if (param_3[9] >= 0)
        d2_MoveObj(n + (short)param_3[2] + 0xf, a + (short)param_3[3] + 6,
                     U_I(((unsigned)&ai_Unkf8f13c + 0x250) + param_3[9] * 0x3c4));
}
