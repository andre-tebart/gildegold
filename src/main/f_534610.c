/* 0x534610, the talent choice of the character creation ("menu\choosecharacter_talent"): five talents (0x75e180 = 11 ints: the start values 0x75e198 (6) and the talent points), each row with a label
   ("%s:$N" of the text 0x16b7 + n), a bar (0x52eaa0, picture 0x4d9) and two arrow buttons (0x519 / 0x51a) that move points between the talent and the pool (local_7c = 0x7e points); the ok button stores the
   five values (0x75e5f0) and the two start tools of the chosen talents (0x75e5a0 / 0x75e5a1); returns 1 when confirmed; ROUGH (the float conversions of the talent steps are lost: placeholders 0) */

#define SV_WI2(i, o) (*(int *)(U_I((unsigned)&d2_Objs) + (i) * 0x2ec + (o)))

int main_Unk534610(void)
{
    int pts, ok, i, r, row, run, w;
    int *p, *q;
    int st[11];
    int *vals;
    int btn[12];
    char again;
    int f, sel;

    r = 0;
    memset((char *)(unsigned)&full_data_75e198, 0, 0x18);
    p = (int *)(unsigned)&full_data_75e180;
    q = st;
    for (i = 0xb; i != 0; i--)
        *q++ = *p++;
    again = 1;
    pts = 0x7e;
    ok = -1;
    vals = st + 6;
    for (i = 4; i < 6; i++)
        vals[st[i]] = (int)0.0f;
    p = st;
    q = (int *)(unsigned)&full_data_75e180;
    for (i = 0xb; i != 0; i--)
        *q++ = *p++;
    f = d2_OpenForm(0, 0, "menu\choosecharacter_talent");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 2);
    io_PrintText(0x1ae2);
    run = sv_RunFrame((int)main_Unk534610, 0x12d6);
    do {
        if (run == 0) {
            d2_CloseView(f);
            return r;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (again) {
            d2_SetForm(f, 1);
            io_PrintText((char *)(unsigned)&full_data_624cf8);
            i = 0;
            w = 0x1c;
            do {
                io_PrintText("%s:$N", i + 0x16b7);
                sv_Unk52eaa0(0xd2, w, (float)vals[i], 0x4d9);
                row = i * 0x25 + 0x1c;
                btn[i * 2] = d2_WinOpenPic(*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - 0x2c, row, 0x519, d2_LastWindow);
                SV_WI2(btn[i * 2], 0x4c) = 1;
                SV_WI2(btn[i * 2], 0x1c0) = 3;
                btn[i * 2 + 1] = d2_WinOpenPic(*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - 0x18, row, 0x51a, d2_LastWindow);
                SV_WI2(btn[i * 2 + 1], 0x4c) = 1;
                SV_WI2(btn[i * 2 + 1], 0x1c0) = 3;
                w += 0x25;
                i++;
            } while (w < 0xd5);
            io_PrintText(0x1ae3);
            sv_Unk52eaa0(0xd2, 0xec, (float)pts, 0x4d9);
            sel = d2_GetWinObj(f, 1, io_PrintText(0x1ae4));
            for (i = 0; i < 5; i++) {
                if (U_I((unsigned)&full_data_75e198 + i * 4) < vals[i]) {
                    SV_WI2(btn[i * 2], 0x3c) = 0;
                    SV_WI2(btn[i * 2], 0x50) = 0;
                } else {
                    SV_WI2(btn[i * 2], 0x3c) = 1;
                    SV_WI2(btn[i * 2], 0x50) = 1;
                }
                if (pts < 1 || 126.0f <= (float)vals[i]) {
                    SV_WI2(btn[i * 2 + 1], 0x3c) = 1;
                    SV_WI2(btn[i * 2 + 1], 0x50) = 1;
                } else {
                    SV_WI2(btn[i * 2 + 1], 0x3c) = 0;
                    SV_WI2(btn[i * 2 + 1], 0x50) = 0;
                }
            }
            again = 0;
        }
        d2_SetObjEnabled(sel, pts == 0);
        r = U_I((unsigned)&d2_ClickedId);
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c) {
                r = 1;
                for (i = 0; i < 5; i++)
                    *(char *)((unsigned)&full_data_75e5f0 + i) = (char)vals[i];
                switch (st[4]) {
                case 0: *(unsigned char *)(unsigned)&init_PlayerProfile = 0x1c; break;
                case 1: *(unsigned char *)(unsigned)&init_PlayerProfile = 0x31; break;
                case 2: *(unsigned char *)(unsigned)&init_PlayerProfile = 4; break;
                case 3: *(unsigned char *)(unsigned)&init_PlayerProfile = 0x37; break;
                case 4: *(unsigned char *)(unsigned)&init_PlayerProfile = 0x10; break;
                }
                switch (st[5]) {
                case 0: *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0x1c; break;
                case 1: *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0x25; break;
                case 2: *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 4; break;
                case 3: *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0; break;
                case 4: *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0x49; break;
                }
                p = st;
                q = (int *)(unsigned)&full_data_75e180;
                for (i = 0xb; i != 0; i--)
                    *q++ = *p++;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
                goto lnext;
            }
            if (U_I((unsigned)&d2_ClickedId) == 0x519 || U_I((unsigned)&d2_ClickedId) == 0x51a) {
                again = 1;
                for (i = 0; i < 12; i++) {
                    if (ai_Unk5fe710 == btn[i]) {
                        row = i / 2;
                        if ((i & 1) == 0) {
                            if (U_I((unsigned)&full_data_75e198 + row * 4) < vals[row]) {
                                vals[row] = (int)0.0f;
                                pts = (int)0.0f;
                            }
                        } else if (0 < pts) {
                            vals[row] = (int)0.0f;
                            pts = (int)0.0f;
                        }
                    }
                }
            }
        }
lnext:
        run = sv_RunFrame((int)main_Unk534610, 0x12d6);
    } while (1);
}
