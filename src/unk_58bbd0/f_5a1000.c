/* 0x5a1000, the guild level 1 window "LOCATIONS\ZUNFTHAUS\LEVEL1" (1968 B, ROUGH): 0x4e94a0 (the player) == 1
   opens it (else 0x520f90 (the player, 3) or the text 0x143fbd8); the window 0x4741f0, the text 0x1643, the
   seven icon buttons 0x4731d0 at the fixed positions (0x40 / 0xc0 / 0x140 / 0x1c0 x 0x50, 0x80 / 0x100 /
   0x180 x 0xb8, each -0x28 / -0x46) with the text ids 0x5a2 + i, the current one flagged at 0xf8f080 + 0x2c;
   the text 0x1644 (the office id 0x253 / 0x27b + the byte 0x770ce9 + slot * 0x21c, the level 3 and the text
   0x1649 + the current icon); the three buttons 0x52e350 (0x143fad4 / d8 / dc): the third only 0x6f22a0, the
   first / second 0x4e9660 (the player, {3, the level, 1 / -1}); an icon button moves the flag and the text
   0x1644 (the "$C" form 0x602568); the pump 0x5309a0 (0x5a1000, 0x677c7), 0x4758f0 / 0x52d7a0 at the end */

int amt_Unk4e94a0(); int amt_CanUseMayorPower3(); int amt_ExecutePriceMessage(char *c, int *m); int ai_Unk5c1b40();
int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();

void ai_Unk5a1000(int param_1)
{
    short pos[7][2] = { {0x40, 0x50}, {0xc0, 0x50}, {0x140, 0x50}, {0x1c0, 0x50},
                        {0x80, 0xb8}, {0x100, 0xb8}, {0x180, 0xb8} };
    int ctls[7];
    int outs[3];
    int req[3];
    char *per;
    int win;
    int cur;
    int i;
    int id;
    int r;

    per = (char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c;
    r = amt_Unk4e94a0(per);
    if (r != 1) {
        if (r != -1) {
            sv_CheckMoney(per, 3);
            return;
        }
        sv_MessageBox((char *)U_I((unsigned)&full_data_143fbd8), 0);
        return;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL1");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x1643);
    d2_SetForm(win, 2);
    for (i = 0; i < 7; i++) {
        ctls[i] = d2_WinOpenPic(pos[i][0] - 0x28, pos[i][1] - 0x46, i + 0x5a2, U_I((unsigned)&d2_LastWindow));
        U_I(U_I((unsigned)&d2_Objs) + 0x4c + ctls[i] * 0x2ec) = 1;
    }
    cur = 0;
    U_I(U_I((unsigned)&d2_Objs) + 0x2c + ctls[0] * 0x2ec) = 1;
    d2_SetForm(win, 3);
    if (*(unsigned char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x253;
    else
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x27b;
    io_PrintText(0x1644, id, 3, 0x1649);
    d2_SetForm(win, 4);
    sv_Unk52e350(U_I((unsigned)&d2_LastWindow), 0, outs, 3, U_I((unsigned)&full_data_143fad4), U_I((unsigned)&full_data_143fad8), U_I((unsigned)&full_data_143fadc));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == outs[2]) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == outs[0]) {
                req[0] = 3;
                req[1] = cur + 1;
                req[2] = 1;
                amt_ExecutePriceMessage((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, req);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == outs[1]) {
                req[0] = 3;
                req[1] = cur + 1;
                req[2] = -1;
                amt_ExecutePriceMessage((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, req);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                for (i = 0; i < 7; i++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == ctls[i] && cur != i) {
                        U_I(U_I((unsigned)&d2_Objs) + 0x2c + ctls[cur] * 0x2ec) = 0;
                        U_I(U_I((unsigned)&d2_Objs) + 0x2c + ctls[i] * 0x2ec) = 1;
                        d2_SetForm(win, 3);
                        io_PrintText("$C");
                        if (*(unsigned char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
                            id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x253;
                        else
                            id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x27b;
                        io_PrintText(0x1644, id, 3, i + 0x1649);
                        cur = i;
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_Unk5a1000, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
