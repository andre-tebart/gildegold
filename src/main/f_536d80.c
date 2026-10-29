/* 0x536d80, the "menu\choosecharacter_intro" dialog with two buttons (texts 0x1adf .. 0x1ae1): returns 1 for the first button, -1 when cancelled, else 0 */

int main_Unk536d80(void)
{
    int r, f, t, b1, b2, g, run;

    r = 0;
    f = d2_OpenForm(0, 0, "menu\\choosecharacter_intro");
    d2_CentreViewWindows(f);
    io_PrintText(0x1adf);
    if (*(unsigned char *)(unsigned)&ai_MissionMode != 0xffffffff)
        io_PrintText(0x1ae0);
    t = io_PrintText(0x1ae1);
    b1 = d2_GetWinObj(f, 0, t);
    b2 = d2_GetWinObj(f, 0, t + 1);
    g = d2_CreateRadioGroup(2, b1, b2);
    run = sv_RunFrame((int)main_Unk536d80, 0xc6);
    do {
        if (run == 0) {
            d2_RadioClear(g);
            d2_CloseView(f);
            return r;
        }
        d2_MenuNavigate(g);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1) {
            r = -1;
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c) {
            r = 1;
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        run = sv_RunFrame((int)main_Unk536d80, 0xc6);
    } while (1);
}
