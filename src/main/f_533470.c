/* 0x533470, the client side network dialog ("Menu\CHOOSENETWORK", two buttons: join a saved game 0x532a70 / enter an address and join 0x532da0 (1)); returns 1 when a game was chosen */

int main_Unk533470(void)
{
    int r, f, b1, b2, g, run;

    r = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    io_PrintText(0x1ca5);
    b1 = d2_GetWinObj(f, 0, io_PrintText(0x1ca9));
    b2 = d2_GetWinObj(f, 0, io_PrintText(0x1caa));
    g = d2_CreateRadioGroup(2, b1, b2);
    run = sv_RunFrame((int)main_Unk533470, 0x24087);
    do {
        if (run == 0) {
            d2_RadioClear(g);
            d2_CloseView(f);
            return r;
        }
        d2_MenuNavigate(g);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
            r = 0;
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (ai_Unk5fe710 == b1) {
                d2_SetViewEnabled(f, 0);
                run = main_Unk532a70();
            } else {
                if (ai_Unk5fe710 != b2)
                    goto lnext;
                d2_SetViewEnabled(f, 0);
                run = main_Unk532da0(1);
            }
            if (run != 0) {
                r = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            }
            d2_SetViewEnabled(f, 1);
        }
lnext:
        run = sv_RunFrame((int)main_Unk533470, 0x24087);
    } while (1);
}
