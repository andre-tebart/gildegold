/* 0x5335b0, the network main menu ("Menu\CHOOSENETWORK", three buttons: host 0x5328f0 (0x6f23f8 = 5), load a network game 0x532da0 (0) (5), join 0x533470 (4)); returns 1 when a game was started */

int main_Unk5335b0(void)
{
    int r, f, b1, b2, b3, g, run;

    r = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    io_PrintText(0x1ca5);
    b1 = d2_GetWinObj(f, 0, io_PrintText(0x1ca6));
    b2 = d2_GetWinObj(f, 0, io_PrintText(0x1ca7));
    b3 = d2_GetWinObj(f, 0, io_PrintText(0x1ca8));
    g = d2_CreateRadioGroup(3, b1, b2, b3);
    *(unsigned char *)(unsigned)&ai_MissionMode = 0xfe;
    run = sv_RunFrame((int)main_Unk5335b0, 0x24087);
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
                U_I((unsigned)&ai_GameFlags) = 5;
                d2_SetViewEnabled(f, 0);
                run = main_HostNetworkGame();
            } else if (ai_Unk5fe710 == b2) {
                U_I((unsigned)&ai_GameFlags) = 5;
                d2_SetViewEnabled(f, 0);
                run = main_Unk532da0(0);
            } else {
                if (ai_Unk5fe710 != b3)
                    goto lnext;
                U_I((unsigned)&ai_GameFlags) = 4;
                d2_SetViewEnabled(f, 0);
                run = main_Unk533470();
            }
            if (run != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                r = 1;
            }
            d2_SetViewEnabled(f, 1);
        }
lnext:
        run = sv_RunFrame((int)main_Unk5335b0, 0x24087);
    } while (1);
}
