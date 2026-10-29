/* 0x536e80, the difficulty choice (same form, five buttons from the text 0x1abf: the chosen level 0 .. 4 goes to 0x6f366a through 0x6f23fc); returns 1 when confirmed */

int main_Unk536e80(void)
{
    int r, f, t, b[5], b6, g, run;

    r = 0;
    f = d2_OpenForm(0, 0, "menu\\choosecharacter_intro");
    d2_CentreViewWindows(f);
    U_I((unsigned)&ai_Difficulty) = *(unsigned char *)((unsigned)&ai_GamePanel + 0x2);
    t = io_PrintText(0x1abf);
    b[0] = d2_GetWinObj(f, 0, t);
    b[1] = d2_GetWinObj(f, 0, t + 1);
    b[2] = d2_GetWinObj(f, 0, t + 2);
    b[3] = d2_GetWinObj(f, 0, t + 3);
    b[4] = d2_GetWinObj(f, 0, t + 4);
    b6 = d2_GetWinObj(f, 0, t + 5);
    g = d2_CreateRadioGroup(6, b[0], b[1], b[2], b[3], b[4], b6);
    d2_RadioSelect(g, U_I((unsigned)&ai_Difficulty));
    run = sv_RunFrame((int)main_Unk536e80, 0xc6);
    do {
        if (run == 0) {
            *(unsigned char *)((unsigned)&ai_GamePanel + 0x2) = (unsigned char)U_I((unsigned)&ai_Difficulty);
            d2_RadioClear(g);
            d2_CloseView(f);
            return r;
        }
        d2_MenuNavigate(g);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1) {
            r = 0;
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c) {
            if (ai_Unk5fe710 == b[0])
                U_I((unsigned)&ai_Difficulty) = 0;
            else if (ai_Unk5fe710 == b[1])
                U_I((unsigned)&ai_Difficulty) = 1;
            else if (ai_Unk5fe710 == b[2])
                U_I((unsigned)&ai_Difficulty) = 2;
            else if (ai_Unk5fe710 == b[3])
                U_I((unsigned)&ai_Difficulty) = 3;
            else if (ai_Unk5fe710 == b[4])
                U_I((unsigned)&ai_Difficulty) = 4;
            r = 1;
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        run = sv_RunFrame((int)main_Unk536e80, 0xc6);
    } while (1);
}
