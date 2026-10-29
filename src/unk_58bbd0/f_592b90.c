/* 0x592b90, the second production window (1457 B, ROUGH SKELETON from the Ghidra pseudocode): the same screen
   and setup as 0x5923c0, but the click opens the building type 3 (0x53e400 (2, 0, 3, 3)) and the character
   record's +0 byte must be 3 (not 9 / 8), the columns come from the goods offer without the > 1 filter, the
   prices / amounts are stored at +0x50 / +0x54, and the goods rows are 0x58cc10 (win, 2, cols, rec1, 1) */

int ai_Unk592b90(int param_1)
{
    int win;
    int cols;
    int col;
    int i;
    int n;
    int v;
    int rec1;
    int rec2;
    int rec3;
    int b;
    int *p;
    unsigned short *item;
    char *pc;
    int flag;
    int msg[0x3e];
    int a[3];
    char text[10];

    if (sv_CheckCharacterFlag(param_1) == 0) {
        win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
        d2_CentreViewWindows(win);
        sv_Unk525220(param_1, 0);
        d2_SetForm(win, 3);
        io_PrintText(0xca);
        ai_Unk58bbd0();
        cols = 0;
        d2_SetForm(win, 0);
        n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
        U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0xe;
        U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0xe;
        U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0xe;
        U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xe;
        sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70b, 0x742);
        v = d2_GetViewWin(win, 2, 0x78, a, &a[1], &a[2]);
        sv_CreateFormSlider(0x270, 0x15e, U_I((unsigned)&d2_LastWindow), v);
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x74 + a[2] * 0x2ec) = 0x54;
        do {
            d2_SetForm(win, 2);
            rec1 = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0xff);
            if (rec1 == 0)
                return 0;
            rec3 = gm_Query(*(int *)(param_1 + 0x5d), 1, 0, 0xfe);
            if (rec3 == 0)
                return 0;
            rec2 = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x2a);
            if (rec2 == 0)
                return 0;
            if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_Unkf8aac4) == win) {
                flag = 0;
                for (p = (int *)((unsigned)&ai_Unk75f280 + 0x20); (int)p <= ((unsigned)&ai_Unk75f280 + 0x51f) && p[-4] != 0xffff; p += 0xa) {
                    if (*p != 0) {
                        flag = 1;
                        break;
                    }
                }
                for (col = 0; col < cols; col++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f5354 + col * 0x58)) {
                        if (flag) {
                            snd_PlayFavourComment();
                            b = he_FindEvent(2, 0, 3, 3, U_I(param_1 + 1));
                            while (b != 0) {
                                if (*(unsigned short *)(b + 0xac) == U_I((unsigned)&full_data_6f5320 + col * 0x58))
                                    goto found;
                                b = he_NextEvent();
                            }
                            /* ROUGH: the type 3 message record (0x4c4b70 / 0x4c3ae0 / 0x5309a0) */
                        found:
                            for (p = (int *)((unsigned)&ai_Unk75f280 + 0x10); *p != 0xffff && (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 0xa) {
                                if (p[4] != 0) {
                                    sv_MoveToGroup(param_1, U_I((unsigned)&sv_PanelShown), *p, b);
                                    U_I((unsigned)&ai_Selected + *p * 0x21c) = 0;
                                }
                            }
                            if (U_I(b + 0x70) == 0)
                                U_I(b + 0x70) = 1;
                        } else {
                            for (p = (int *)((unsigned)&ai_Unk75f280 + 0x10); *p != 0xffff && (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 0xa) {
                                pc = (char *)U_I(((unsigned)&ai_Unk770cec + 0x10) + *p * 0x21c);
                                if (pc != 0 && *pc == 3 &&
                                    *(int *)(pc + 0x10) == U_I(param_1 + 1) &&
                                    *(unsigned short *)(pc + 0xac) ==
                                        U_I((unsigned)&full_data_6f5320 + col * 0x58))
                                    U_I((unsigned)&ai_Selected + *p * 0x21c) = 1;
                            }
                        }
                    }
                }
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                /* ROUGH: as 0x5923c0: no lit carrier -> 0x6f22a0 = 1, 0x52d7a0 */
                sv_CameraFollow();
            }
            /* the goods offer columns (the table 0x6f5320, the cap 0x6f58a0) */
            item = (unsigned short *)gm_Query(*(int *)(rec1 + 0x14), 1, 4, 0x17);
            while (item != 0) {
                if (*(int *)(item + 7) > 1) {
                    U_I((unsigned)&full_data_6f5320 + cols * 0x58) = *item;
                    cols++;
                }
                item = (unsigned short *)gm_QueryNext();
            }
            if (cols > 0) {
                /* ROUGH: the prices / amounts per column (the goods 0x91aaf0, 0x50aaf0, the words +0xe /
                   +0x36), stored at the column +0x50 / +0x54 */
                for (col = 0; col < cols; col++) {
                    b = gm_Query(*(int *)(rec2 + 0x14), 1, 0, U_I((unsigned)&full_data_6f5320 + col * 0x58));
                    U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x50) = b == 0 ? 0 : U_I(b + 0xe);
                    b = gm_Query(*(int *)(rec2 + 0x14), 1, 0, U_I((unsigned)&full_data_6f5320 + col * 0x58));
                    U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x54) = b == 0 ? 0 : gm_ListAmount(rec2, b);
                }
            }
            ai_Unk5e56c4(cols, a, 10);
            d2_SetObjText(a[2], a);
            ai_Unk58cc10(win, 2, cols, rec2, 1);
            ai_Unk58d720(param_1, rec1);
            ai_Unk58dd00(win, 1, 0, param_1);
            n = sv_RunFrame(ai_Unk592b90, 0x677c7);
        } while (n != 0);
        d2_CloseView(win);
        sv_ClosePlayerBar();
    }
    return 0;
}
