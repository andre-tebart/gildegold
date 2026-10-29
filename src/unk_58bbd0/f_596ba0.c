/* 0x596ba0, the mistress window "locations\wohnsitz\geliebte" (1007 B, ROUGH SKELETON from the Ghidra
   pseudocode): the 8 slots (the ids at +0, the buildings +8, the flags +0x30), the window 0x4741f0 / 0x475f70 /
   0x52ea30 / 0x475600 / 0x476f80, the text 0x515960 (0x1a50 / 0x1a51) and the control 0x475b80; per frame the
   4 mistresses of the player (0x53e400 (1, 0, 0x6f), +0xac, +0x70), the texts 0x515960 (0x1a4e / 0x1a4f /
   0x1a52), the selection 0x5c1130 (the config 0x2040a / 0xa7fdfbb6 / 1 / 6, the texts 0x1a53 / 0x1a54 /
   0x1a55 / 0x1a56), the message record (type 0x6f, the character, the building) 0x4c4b70, and 0x5967c0 for
   the chosen slot; the pump 0x5309a0 (this, 0x67fff) and the exit 0x4758f0 / 0x52d7a0 */

void ai_MistressWindow(int param_1)
{
    int win;
    int ctl;
    int b;
    int i;
    int n;
    int ids[16];
    int cfg[0x14];
    char text1[0x40];
    char text2[0x40];
    char text3[0x200];

    memset(ids, 0, sizeof(ids));
    sv_ReleaseNamePlates();
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\geliebte");
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    ctl = d2_GetWinObj(win, 2, io_PrintText(*(unsigned char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0
                                                ? 0x1a50 : 0x1a51));
    U_I((unsigned)&d2_ClickedId) = -1;
    while (sv_RunFrame(ai_MistressWindow, 0x67fff) != 0) {
        for (i = 0; i < 16; i++)
            ids[i] = -1;
        n = 0;
        for (b = he_FindEvent(1, 0, 0x6f); b != 0 && n < 4; b = he_NextEvent()) {
            if (*(int *)(b + 0xac) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87)) {
                ids[n + 8] = U_I(b + 4);
                ids[n + 0xc] = U_I(b + 0x70) == 0 ? 1 : 0;
                ids[n] = U_I(b + 0xb0);
                n++;
            }
        }
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText("_Z_  %s   %N", 0x1a4e);
        io_PrintText(0x1a4f);
        for (i = 0; i < 4; i++) {
            unsigned short *p = (unsigned short *)gm_FindChar(ids[i]);
            int c;
            if (p == 0)
                break;
            c = d2_GetWinObj(win, 1, io_PrintText(0x1a52, *p));
            ids[i + 4] = c;   /* ROUGH: the control ids live at +0x10 in the original */
            d2_SetObjEnabled(c, ids[i + 0xc]);
        }
        d2_SetObjEnabled(ctl, n < 4);
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == ctl) {
                    /* ROUGH: the config 0x2040a - (the gender flag), the texts 0x1a53 / 0x1a54, 0x5c1130,
                       the texts 0x1a55 / 0x1a56, the message record (type 0x6f, the character, the building)
                       through 0x4c4b70 */
                } else {
                    for (i = 0; i < 4; i++) {
                        if (U_I((unsigned)&ai_Unk5fe710) == ids[i + 4]) {
                            ai_Unk5967c0(param_1, (unsigned short *)gm_FindChar(ids[i]),
                                         he_FindEvent(1, 1, ids[i + 8]));
                            break;
                        }
                    }
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
