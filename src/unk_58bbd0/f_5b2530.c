/* 0x5b2530, a two-choice window (384 B): the kind record 0x7e3f30 + kind * 0x24d, the window "0x62ccfc"
   (0x4741f0 / 0x475600), the caption 0x10b; the kind 0x116 uses the +0x242 byte (option 0x10e against the
   text 0x111), the other kinds the +0x240 byte for the first option (0x10c / 0x110) and the +0x241 byte for
   the second (0x10d / 0x111) against param_4; the control ids come from 0x475b80 (win, 0, 0x515960 (id, 0x1f40))
   and the tail 0x515960 (0x10f, 1); the pump 0x5309a0 (this, 0x677c7) drives the click dispatch (0xf8a668,
   0x5fe710) and 0x14673a0; returns 1 / 2 for the two options, 0 otherwise; 0x4758f0 closes */

int ai_Unk5b2530(unsigned short *param_1, unsigned char *param_2, int param_3, int param_4)
{
    char *kind;
    int win;
    int ctrl;
    int ctrl1;
    int r;

    kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_2 * 0x24d);
    r = 0;
    ctrl = -1;
    win = d2_OpenForm((char *)(unsigned)&full_data_62ccfc, 0, 0);
    d2_CentreViewWindows(win);
    io_PrintText(0x10b);
    if (*param_1 == 0x116) {
        if (param_3 >= *(unsigned char *)(kind + 0x242))
            io_PrintText(0x111);
        else
            ctrl1 = d2_GetWinObj(win, 0, io_PrintText(0x10e, 0x1f40));
    } else {
        if (param_3 >= *(unsigned char *)(kind + 0x240))
            io_PrintText(0x110);
        else
            ctrl = d2_GetWinObj(win, 0, io_PrintText(0x10c, 0x1f40));
        if (param_4 >= *(unsigned char *)(kind + 0x241))
            io_PrintText(0x111);
        else
            ctrl1 = d2_GetWinObj(win, 0, io_PrintText(0x10d, 0x1f40));
    }
    io_PrintText(0x10f, 1);
    do {
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctrl1) {
                r = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctrl) {
                r = 2;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk5b2530, 0x677c7) != 0);
    d2_CloseView(win);
    return r;
}
