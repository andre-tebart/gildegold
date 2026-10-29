/* 0x5967c0, the mistress window "locations\wohnsitz\geliebte2" (990 B, ROUGH SKELETON from the Ghidra
   pseudocode): param_2 the character record, param_3 the building record; 0x4741f0 / 0x475f70 (0) / 0x52ea30 /
   0x475600 / 0x476f80 / 0x475f70 (1), the text 0x515960 ("_C_Z_ _1N3_", *param_2), the screen 0x52e350, the
   list 0x5be1d0 / 0x5be3b0; per frame 0x505df0 (the character), 0x53e400 (1, 1, the building), the daily flag
   (the record +0xbc against the day 0x770ae0), the texts 0x5176d0 (0x1a57 / 0x1a58), the selection control,
   the demand (0x5c1130 with the config 0x20400 / 0xa7fdfbb6 / 1 / 6, the texts 0x1a53 / 0x1a54 / 0x1a5b) and
   the 0x4c4c60 / 0x4c3ae0 / 0x4c5f60 / 0x4c4b00 message; the pump 0x5309a0 (this, 0x677c7) and the exit
   0x4758f0 */

void ai_Unk5967c0(int param_1, unsigned short *param_2, int param_3)
{
    int win;
    int ctl;
    int ui;
    int bld;
    int b;
    int i;
    int cfg[0x14];
    unsigned short *list[4];
    char text1[0x40];
    char text2[0x200];
    char text3[0x200];

    ui = U_I(param_2 + 2);
    bld = U_I(param_3 + 4);
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\geliebte2");
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText("_C_Z_ _1N3_", *param_2);
    d2_SetForm(win, 3);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, &ctl, 2, U_I((unsigned)&full_data_1440b30), U_I((unsigned)&full_data_1440b34));
    d2_SetForm(win, 2);
    list[0] = param_2;
    ai_AmountRowStatus(list);
    ai_AmountRow(0x19, 0x19, U_I((unsigned)&d2_LastWindow), 0x54, list, 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    while (sv_RunFrame(ai_Unk5967c0, 0x677c7) != 0) {
        i = gm_FindChar(ui);
        if (i == 0 || (b = he_FindEvent(1, 1, bld), b == 0)) {
            d2_SetObjEnabled(ctl, 0);
        } else {
            d2_SetObjEnabled(ctl, U_I((unsigned)&ai_Time) > U_I(param_3 + 0xbc) ? 1 : 0);
        }
        if (U_I(((unsigned)&dm_L + 0x20)) == 0 && i != 0 && b != 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == 0 /* the screen control */) {
                    /* ROUGH: the demand text 0x1a57 / 0x1a58, the confirmation 0x521150, the daily stamp
                       (param_3 + 0x52 .., +0xbc), the message 0x4c4c60 (1, param_3) and 0x4c3ae0 */
                } else if (U_I((unsigned)&ai_Unk5fe710) == ctl) {
                    /* ROUGH: the config 0x20400 / 0xa7fdfbb6 / 1 / 6, the texts 0x1a53 / 0x1a54, 0x5c1130,
                       the text 0x1a5b, the daily stamp, 0x4c4c60 (3, param_3), 0x4c5f60 and 0x4c4b00 */
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
}
