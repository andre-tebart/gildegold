/* 0x5a5120, the kidnapping window "locations\diebesgilde\diebesgilde_entfuehrung" (698 B): param_1 the building
   (byte +0x1b1 a running attempt -> the text 0x19ba), the live character 0x520f90 (3, 0x770b80 + slot * 0x21c),
   the own-building gate +0x16c / +0x27 and the action 0x4a63f0 (param_1, 0, 0x4080a0, 0, 0, 100.0f, &ctrl3)
   only without a +0x170 record, 0x501190 (1, 1, ctrl3); the window 0x4741f0 / 0x475600 / 0x476f80 / 0x475f70
   (1) with two controls from 0x515960 (0x19b9, the word, 3, the word) and 0x475b80; the pump 0x5309a0 (this,
   0x677c7): a click (0xf8a668) on the first control builds the AIMSG type 0x3c (+4, the player character id at
   +8, 1 at +0x36, -1 in the eight crew slots +0x38, the owner at +0x58, the control +1 at +0x5c, the current
   building +1 at +0x60) from the crew flags +0x184 of the 0x300 character records 0x770b84 and sends
   0x4c4b70 / 0x4c5f60 / 0x4f2e30 (0x1a, 1, the id, the owner, -1) / 0x4c2d20 / 0x52ae40 with 0x6f22a0 = 3, or
   shows the text 0x144094c when no crew is free; the second control only sets 0x6f22a0 = 1; the exit 0x4758f0,
   returns 1 when a crew was hired */

int ai_Unk4a63f0(struct AIPER *p, int a, __int64 mask, int c, float f, int *out); int unk_Unk4f2e30();

int ai_Unk5a5120(int param_1)
{
    int hire;
    int ret = 0;
    int ctrl1;
    int ctrl2;
    int ctrl3;
    int win;
    int id;
    int n;
    int i;
    char msg[0xf8];
    char buf[0x7f8];
    char *p;

    if (param_1 == 0)
        return 0;
    if (sv_CheckMoney(3, (char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c)) == 0)
        return 0;
    if (U_I(param_1 + 0x16c) != 0 && U_W(U_I(param_1 + 0x16c) + 0x27) == U_W(param_1))
        return 0;
    if (*(char *)(param_1 + 0x1b1) != 0) {
        txt_Format(buf, 0x19ba, U_W(param_1));
        sv_MessageBox(buf, 0);
        return 0;
    }
    if (U_I(param_1 + 0x170) == 0) {
        if (AI_4A63F0_7(param_1, 0, 0x4080a0, 0, 0, 100.0f, &ctrl3) == 0)
            return 0;
        hire = gm_BuildQuery(1, 1, ctrl3);
    }
    win = d2_OpenForm(0, 0, "locations\diebesgilde\diebesgilde_entfuehrung");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19b9, U_W(param_1), 3, U_W(param_1));
    ctrl1 = d2_GetWinObj(win, 1, id);
    ctrl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctrl1) {
                for (i = 0; i < 0x3e; i++)
                    U_I(msg + i * 4) = 0;
                *(unsigned char *)(msg + 4) = 0x3c;
                *(unsigned char *)(msg + 0x36) = 1;
                U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                U_I(msg + 0x58) = U_I(param_1 + 4);
                U_I(msg + 0x5c) = U_I(ctrl1 + 1);
                U_I(msg + 0x60) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
                for (i = 0; i < 8; i++)
                    U_I(msg + 0x38 + i * 4) = -1;
                n = 0;
                for (p = (char *)(unsigned)&ai_CharInts; p < (char *)((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); p += 0x21c) {
                    if (*(char *)(p + 0x184) != 0) {
                        U_I(msg + 0x38 + n * 4) = U_I(p);
                        n++;
                    }
                }
                if (n == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144094c), 0);
                    U_I((unsigned)&d2_ClickedId) = -1;
                } else {
                    cm_SendLongMessage(msg);
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), -3);
                    unk_Unk4f2e30(0x1a, 1, U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), U_I(param_1 + 4), -1);
                    snd_PlayFavourComment();
                    sv_Unk52ae40();
                    ret = 1;
                    U_I((unsigned)&sv_PopRequest) = 3;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctrl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a5120, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    return ret;
}
