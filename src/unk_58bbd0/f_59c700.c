/* 0x59c700, the money lender's exchange window "..geldleihe_Wechsel" (3024 B, ROUGH SKELETON: the two record
   tables and the exchange texts are written, the money transfers are condensed): the window 0x4741f0, the five
   controls 0x475b80 (0 .. 4), the texts 0x18f5, 0x18f6 (the record +0x69 and +0x6d, the city factor), the two
   lists of 16 records (0x59c120 (param_1 + 0x5d / the player's goods 0x770cf8 + player * 0x21c, the list, the
   window, 5 / 4, 1)), the selected list entry (+5 / +3 / +6) texts 0x18fa / 0x18f8 / 0x18fd, the counts
   0x59c6e0, the buttons 0x475aa0 / 0x5e56c4, the panel 0x472760, the exchange of the two goods by 0x4c4540 /
   0x4c4880 ("kredit") and 0x4c4580; 0x46c70 / 0x46c90 / 0x477530 refresh the lists; 0x476c70 / 0x4758f0 /
   0x52d7a0 on exit */

void ai_Unk59c700(int param_1)
{
    int recsA[16 * 7];
    int recsB[16 * 7];
    char text[0x100];
    int win;
    int ctl[5];
    int i;
    int n;
    int m;
    int side;
    int sel;
    int idx;
    int v;
    int r;
    int a;
    int b;

    for (i = 0; i < 16 * 7; i++) {
        recsA[i] = 0;
        recsB[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        recsA[i * 7] = -1;
        recsB[i * 7] = -1;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_Wechsel");
    d2_CentreViewWindows(win);
    ctl[2] = d2_GetWinObj(win, 0, 2);
    ctl[3] = d2_GetWinObj(win, 0, 3);
    ctl[1] = d2_GetWinObj(win, 0, 1);
    ctl[0] = d2_GetWinObj(win, 0, 0);
    ctl[4] = d2_GetWinObj(win, 0, 4);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x73b, 0x73f);
    d2_SetForm(win, 0);
    /* ROUGH: the two header texts at the control rectangles (0x473650, +2 / +2) */
    d2_CreateWinText((short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x10) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 4)) + 2,
                 (short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x16) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + *(short *)(U_I((unsigned)&d2_Objs) + ctl[1] * 0x2ec + 0x12)) + 2,
                 (char *)(unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
    d2_CreateWinText((short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x10) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 4)) + 2,
                 (short)(*(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x16) -
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + *(short *)(U_I((unsigned)&d2_Objs) + ctl[2] * 0x2ec + 0x12)) + 2,
                 (char *)(unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xe;
    d2_SetForm(win, 2);
    io_PrintText(0x18f6, U_I(param_1 + 0x69),
                 (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8, U_I(param_1 + 0x6d));
    d2_CopyCaption((char *)U_I((unsigned)&full_data_144059c));
    d2_SetPointerSprite(0, 0);
    side = 0;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            if (d2_CountPointerItems() == 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                d2_EndInputEdit();
                for (i = 0; i < 16; i++) {
                    if (recsA[i * 7] != -1)
                        d2_SetObjValue(recsA[i * 7], 0, 0, 0);
                    if (recsB[i * 7] != -1)
                        d2_SetObjValue(recsB[i * 7], 0, 0, 0);
                }
            }
        }
        d2_ClearPointerItems();
        sel = ai_Unk59c120(U_I(param_1 + 0x5d), recsA, win, 5, 1);
        if (side == 2 || sel == -1) {
            sel = ai_Unk59c120(U_I(((unsigned)&ai_Unk770cec + 0xc) + U_W((unsigned)&ai_PlayerId) * 0x21c), recsB, win, 4, 1);
            if (sel != -1) {
                if (*(unsigned char *)(unsigned)&ai_Unkf8aae0 == 0 || side == 0) {
                    txt_Format(text, 0x18f8, recsB[sel * 7 + 5], recsB[sel * 7 + 3] * 2 + 0x9a8);
                    d2_CopyCaption(text);
                }
                side = 2;
                d2_SetObjPic(ctl[4], 0x4f9);
            } else {
                side = 0;
                d2_CopyCaption((char *)U_I((unsigned)&full_data_144059c));
                sel = -1;
                d2_SetObjPic(ctl[4], 0x4f7);
            }
        } else {
            if (*(unsigned char *)(unsigned)&ai_Unkf8aae0 == 0 || side == 0) {
                txt_Format(text, 0x18fa, recsA[sel * 7 + 5], recsA[sel * 7 + 3] * 2 + 0x9a8);
                d2_CopyCaption(text);
            }
            side = 1;
            d2_SetObjPic(ctl[4], 0x4f8);
        }
        n = ai_UsedRecordCount(recsA);
        m = ai_UsedRecordCount(recsB);
        if (n < 5) {
            d2_EnableObject(ctl[1], 0);
            d2_EnableObject(ctl[0], 0);
            d2_EnableObject(ctl[2], 0);
        } else {
            d2_EnableObject(ctl[1], 1);
            d2_EnableObject(ctl[0], 1);
            d2_EnableObject(ctl[2], 1);
            _itoa(n, text, 10);
            d2_SetObjText(ctl[2], text);
        }
        if (m < 5) {
            d2_EnableObject(ctl[2], 0);
            d2_EnableObject(ctl[3], 0);
            d2_EnableObject(ctl[0], 0);
        } else {
            d2_EnableObject(ctl[2], 1);
            d2_EnableObject(ctl[3], 1);
            d2_EnableObject(ctl[0], 1);
            _itoa(m, text, 10);
            d2_SetObjText(ctl[0], text);
        }
        if (U_I((unsigned)&d2_ClickedId) == 3 || U_I((unsigned)&d2_ClickedId) == 4) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl[1]) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, -0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[3]) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, -0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[2]) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, 0x4c, U_I((unsigned)&d2_LastWindow));
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[0]) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, 0x4c, U_I((unsigned)&d2_LastWindow));
            }
        }
        if (side == 1) {
            idx = ai_Unk59c6b0(recsB);
            if (idx != -1) {
                a = recsB[idx * 7 + 3];
                b = recsA[sel * 7 + 3];
                if (a == b) {
                    d2_CopyCaption((char *)U_I((unsigned)&full_data_14405b0));
                } else {
                    /* ROUGH: the rate checks (0x508620) and the two transfers of the goods a / b by
                       0x4c4540 / 0x4c4880 ("kredit") / 0x4c4580, then both lists are rebuilt */
                    cm_BeginGroup("kredit");
                    cm_SendExSellObject(-1, U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), a, 0, U_I((unsigned)&ai_OwnPlayer), 0);
                    cm_SendExSellObject(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), -1, b, 0, U_I((unsigned)&ai_OwnPlayer), 0);
                    r = ((int (*)(void *))cm_EndGroup)("kredit");
                    while (cm_GetSlotState(r) == 0)
                        sv_RunFrame(0, 0x677c7);
                    ai_Unk59c120(U_I(((unsigned)&ai_Unk770cec + 0xc) + U_W((unsigned)&ai_PlayerId) * 0x21c), recsB, win, 4, 1);
                    ai_Unk59c120(U_I(param_1 + 0x5d), recsA, win, 5, 1);
                    sv_SetStatusText((char *)U_I((unsigned)&full_data_14405b8));
                    d2_EndInputEdit();
                    for (i = 0; i < 16; i++) {
                        if (recsA[i * 7] != -1)
                            d2_SetObjValue(recsA[i * 7], 0, 0, 0);
                        if (recsB[i * 7] != -1)
                            d2_SetObjValue(recsB[i * 7], 0, 0, 0);
                    }
                }
            }
        } else if (side == 2) {
            idx = ai_Unk59c6b0(recsA);
            if (idx != -1) {
                a = recsA[idx * 7 + 3];
                b = recsB[sel * 7 + 3];
                if (a == b) {
                    sv_SetStatusText((char *)U_I((unsigned)&full_data_14405b0));
                } else {
                    /* ROUGH: the same exchange in the other direction */
                    cm_BeginGroup("kredit");
                    cm_SendExSellObject(-1, U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), a, 0, U_I((unsigned)&ai_OwnPlayer), 0);
                    cm_SendExSellObject(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), -1, b, 0, U_I((unsigned)&ai_OwnPlayer), 0);
                    r = ((int (*)(void *))cm_EndGroup)("kredit");
                    while (cm_GetSlotState(r) == 0)
                        sv_RunFrame(0, 0x677c7);
                    ai_Unk59c120(U_I(((unsigned)&ai_Unk770cec + 0xc) + U_W((unsigned)&ai_PlayerId) * 0x21c), recsB, win, 4, 1);
                    ai_Unk59c120(U_I(param_1 + 0x5d), recsA, win, 5, 1);
                    sv_SetStatusText((char *)U_I((unsigned)&full_data_14405b8));
                    d2_EndInputEdit();
                }
            }
        }
        if (sv_RunFrame(ai_Unk59c700, 0x677c7) == 0) {
            d2_ClearPointerItems();
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
