/* 0x59d2d0, the money lender's exchange fees window "..geldleihe_WechselGebuehren" (320 B): the window, the
   text 0x1908, the two counters 0x475b80 (the text 0x1907 and the next text id) and the sliders 0x475cb0
   (3, 0xf, param_1 + 0x69 / + 0x6d); the pump 0x5309a0 (0x677c7); on select 0x4c3ba0 (param_1, param_1 + 1),
   0x4c3c00 (4, 1, 0x69, &value), 0x4c3c00 (4, 1, 0x6d, &value2), 0x4c49a0; 0x4758f0 at the end */

void ai_Unk59d2d0(int param_1)
{
    int win;
    int ctl1;
    int ctl2;
    int text;
    int v1;
    int v2;
    int r;

    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_WechselGebuehren");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText(0x1908);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1907);
    ctl1 = d2_GetWinObj(win, 1, text);
    ctl2 = d2_GetWinObj(win, 1, text + 1);
    d2_SetObjValue(ctl2, 3, 0xf, U_I(param_1 + 0x69));
    d2_SetObjValue(ctl1, 3, 0xf, U_I(param_1 + 0x6d));
    for (;;) {
        if (sv_RunFrame(ai_Unk59d2d0, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            v1 = d2_GetObjValue(ctl2);
            v2 = d2_GetObjValue(ctl1);
            cm_BeginFieldBlock(param_1, U_I(param_1 + 1));
            cm_AddFieldDelta(4, 1, 0x69, &v1);
            cm_AddFieldDelta(4, 1, 0x6d, &v2);
            cm_SendFieldDeltas();
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
}
