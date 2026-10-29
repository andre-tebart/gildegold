/* 0x5923c0, the production window of a building (1998 B, ROUGH SKELETON from the Ghidra pseudocode): the screen
   "Handel\Handel_PRODUKTION" 0x62b038 opens (0x520fe0, 0x4741f0 / 0x475600, 0x525220, the frames 0x475f70
   (3 / 0 / 2), the text 0x515960 (199), the table init 0x58bbd0, 0x6f5320 = 0, the skin 0xf8f4b8.. = 0xe, the
   controls 0x52e6d0 / 0x475ff0 / 0x52e860 with the word 0x54 at +0x74, 0x476f80 (0, 0)); the loop reads the
   three records 0x4ffe90 (0xff, 0x2a, 0xfe), the click 0xf8a668 / 0xf8aac4 opens the building (0x53e400
   (2, 0, 9, 3) / (2, 0, 8, 3), the message record 0x4c4b70, the carriers 0x75f290 / 0x75f2a0, 0x524260,
   0x770d08), the goods columns (0x6f5320 / 0x6f5354, 0x58 bytes each) and the item availability (0x4ffe90
   (+, 1, 4, 0x17) / 0x4ffcc0, the prices 0x50aaf0 and the amounts 0x91aaf0); the texts 0x5e56c4 / 0x4734d0,
   the columns 0x58c680 / 0x58d720 / 0x58dd00; the pump 0x5309a0 (0x58bd60, 0x677c7); the exit 0x4758f0 /
   0x525440 */

void sv_CameraFollow();

int ai_Unk5923c0(int param_1)
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
    int ctl;
    char text[10];

    if (sv_CheckCharacterFlag(param_1) == 0) {
        win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
        d2_CentreViewWindows(win);
        sv_Unk525220(param_1, 0);
        d2_SetForm(win, 3);
        io_PrintText(199);
        ai_Unk58bbd0();
        U_I((unsigned)&full_data_6f5320) = 0;
        cols = 1;
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
        d2_SetPointerSprite(0, 0);
        do {
            d2_SetForm(win, 2);
            rec1 = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0xff);
            if (rec1 == 0)
                return 0;
            rec2 = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x2a);
            if (rec2 == 0)
                return 0;
            rec3 = gm_Query(*(int *)(param_1 + 0x5d), 1, 0, 0xfe);
            if (rec3 == 0)
                return 0;
            if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_Unkf8aac4) == win) {
                /* the click: a carrier of the table 0x75f2a0 (id at -0x10, the flag at +0)? */
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
                            if (col == 0) {
                                snd_PlayFavourComment();
                                b = he_FindEvent(2, 0, 9, 3, U_I(param_1 + 1));
                                if (b == 0) {
                                    /* the "9" message: 0x3e-dword record, 0x4c4b70, the confirmation
                                       0x4c3ae0 / 0x5309a0 (0, 0x20007) */
                                    goto next;
                                }
                                /* ROUGH: the record fields (local_f4 = 9, the clock 0x770ae0.., the
                                   character 0x770b84, the building, the two texts rec1 / rec3) and the
                                   0x4c4b70 / 0x4c3ae0 loop */
                            } else {
                                snd_PlayFavourComment();
                                b = he_FindEvent(2, 0, 8, 3, U_I(param_1 + 1));
                                while (b != 0) {
                                    if (*(unsigned short *)(b + 0xac) ==
                                        U_I((unsigned)&full_data_6f5320 + col * 0x58))
                                        goto found;
                                    b = he_NextEvent();
                                }
                                /* ROUGH: the "8" message record with the column's building */
                                goto next;
                            }
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
                                if (col == 0) {
                                    if (pc != 0 && *pc == 9 &&
                                        *(int *)(pc + 0x10) == U_I(param_1 + 1))
                                        U_I((unsigned)&ai_Selected + *p * 0x21c) = 0;
                                } else if (pc != 0 && *pc == 8 &&
                                           *(int *)(pc + 0x10) == U_I(param_1 + 1) &&
                                           *(unsigned short *)(pc + 0xac) ==
                                               U_I((unsigned)&full_data_6f5320 + col * 0x58)) {
                                    U_I((unsigned)&ai_Selected + *p * 0x21c) = 1;
                                }
                            }
                        }
                    }
                next:
                    ;
                }
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                /* ROUGH: the carrier check again; when no carrier is lit, 0x6f22a0 = 1 and 0x52d7a0 */
                sv_CameraFollow();
            }
            /* the building's goods offer: the record +0x14, the kinds 0x17 walk the table 0x6f5320 */
            item = (unsigned short *)gm_Query(*(int *)(rec1 + 0x14), 1, 4, 0x17);
            while (item != 0) {
                if (*(int *)(item + 7) > 1) {
                    /* ROUGH: the new columns (0x6f5320 + cols * 0x16, the cap 0x6f58a0, the record's
                       ushort at +0, skip the already present 0x6f5378 entries) */
                    U_I((unsigned)&full_data_6f5320 + cols * 0x58) = *item;
                    cols++;
                }
                item = (unsigned short *)gm_QueryNext();
            }
            if (cols > 1) {
                /* ROUGH: the price / amount columns: for every column after the first the goods record
                   0x91aaf0 + id * 0x41 gives the words at +8 / +0, the price 0x50aaf0 and the amount
                   +0x36; the column's +0x50 / +0x54 / +0x58 */
                for (col = 1; col < cols; col++) {
                    n = U_I((unsigned)&ai_Goods);
                    U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x2c) = 0x7fffffff;
                    for (i = 0; i < 4; i++) {
                        v = *(unsigned short *)(0x26 + i * 2 + U_I((unsigned)&full_data_6f5320 + col * 0x58) * 0x41 + 8 + n);
                        U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x10 + i * 4) = v;
                        U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x20 + i * 4) =
                            *(unsigned short *)(0x26 + i * 2 + U_I((unsigned)&full_data_6f5320 + col * 0x58) * 0x41 + n);
                    }
                    /* ROUGH: the per-good price minimum and the amount of the record */
                }
            }
            _itoa(cols - 1, a, 10);
            d2_SetObjText(a[2], a);
            ai_Unk58c680(win, 2, cols, rec3);
            ai_Unk58d720(param_1, rec1);
            ai_Unk58dd00(win, 1, 0, param_1);
            n = sv_RunFrame(ai_ProductionWindow, 0x677c7);
        } while (n != 0);
        d2_CloseView(win);
        sv_ClosePlayerBar();
    }
    return 0;
}
