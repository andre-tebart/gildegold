/* 0x593150, the production window of a type 4 building (1894 B, ROUGH SKELETON from the Ghidra pseudocode): as
   0x5923c0 (the screen 0x62b038, 0x58bbd0, the frames, the skin 0xe, 0x52e6d0 / 0x475ff0 / 0x52e860, the word
   0x54), the text 0x515960 (0xcb); the click opens the type 4 building (0x53e400 (2, 0, 4, 3)) and the carrier
   loop 0x524260 / 0x770d08; the four selection columns 0x6f5320 + 0x58 and 0x6f5a80 (the chosen index 0x6f5a60);
   the goods offer 0x500b90, the goods list 0x889400 / 0x76fdc4 (kind 0x17 skipped, the four needs at +0x2e), the
   price / amount loop (0x91aaf0, 0x50aaf0), 0x58cc10 (win, 2, cols, the record, 2), 0x58d720, 0x58dd00; the
   pump 0x5309a0 (this, 0x677c7) */

int ai_StorageWindow(); int ai_Unk5b9f30(); int ai_Unk5bba00(); int gm_BuildKindObjects(char *b);
int ai_Unk5e56c4();

int ai_Unk593150(int param_1)
{
    int win;
    int cols;
    int col;
    int i;
    int n;
    int v;
    int rec1;
    int rec2;
    int b;
    int id;
    int add;
    int *p;
    int w;
    int a[3];

    if (sv_CheckCharacterFlag(param_1) == 0) {
        win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
        d2_CentreViewWindows(win);
        sv_Unk525220(param_1, 0);
        d2_SetForm(win, 3);
        io_PrintText(0xcb);
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
            rec2 = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x2a);
            if (rec2 == 0)
                return 0;
            if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_Unkf8aac4) == win) {
                b = 0;
                for (p = (int *)((unsigned)&ai_Unk75f280 + 0x20); (int)p <= ((unsigned)&ai_Unk75f280 + 0x51f) && p[-4] != 0xffff; p += 0xa) {
                    if (*p != 0) {
                        b = 1;
                        break;
                    }
                }
                for (col = 0; col < cols; col++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f5354 + col * 0x58)) {
                        if (b) {
                            /* ROUGH: find the type 4 building 0x53e400 (2, 0, 4, 3) with the column's id,
                               else post the type 4 message (0x4c2d20, the 0x3e-dword record with the kind 4,
                               the character 0x770b84 (param_1 + 0x27 or 0x6f25b8), the clock 0x770ae0.., the
                               building id, the text rec2 + 2, 0x4c4b70 / 0x4c3ae0 / 0x4c3b10); the carriers
                               0x75f290 (0x6f2310 or the record 0xfd) get 0x524260, 0x770d08 = 0, the +0x70
                               flag */
                            if (U_I(b + 0x70) == 0)
                                U_I(b + 0x70) = 1;
                        } else {
                            for (p = (int *)((unsigned)&ai_Unk75f280 + 0x10); *p != 0xffff && (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 0xa) {
                                id = *p;
                                if (*(char **)(((unsigned)&ai_Unk770cec + 0x10) + id * 0x21c) != 0 &&
                                    **(char **)(((unsigned)&ai_Unk770cec + 0x10) + id * 0x21c) == 4 &&
                                    *(int *)(*(char **)(((unsigned)&ai_Unk770cec + 0x10) + id * 0x21c) + 0x10) ==
                                        U_I(param_1 + 1) &&
                                    *(unsigned short *)(*(char **)(((unsigned)&ai_Unk770cec + 0x10) + id * 0x21c) + 0xac) ==
                                        U_I((unsigned)&full_data_6f5320 + col * 0x58))
                                    U_I((unsigned)&ai_Selected + id * 0x21c) = 1;
                            }
                        }
                    }
                    /* the four selection values of the column (0x6f534c, the chosen 0x6f5a60) */
                    for (i = 0; i < 4; i++) {
                        if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f5324 + col * 0x58 + i * 4)) {
                            if (U_I((unsigned)&full_data_6f534c + col * 0x58) == i)
                                U_I((unsigned)&full_data_6f5a60) = 0;
                            else
                                U_I((unsigned)&full_data_6f5a60) = U_I((unsigned)&full_data_6f5320 + col * 0x58 + i * 4);
                        }
                    }
                }
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                /* ROUGH: no lit carrier -> 0x6f22a0 = 1, 0x52d7a0; the carriers off */
                sv_CameraFollow();
            }
            gm_BuildKindObjects(param_1);
            if (cols < 0x10) {
                /* ROUGH: the goods list 0x889400 (0x76fdc4 entries): the kind 0x17 is skipped, the four needs
                   at +0x2e must be available (0x4ffe90, the record +0xe == 2 or the building kind 0x1e); the
                   accepted ids become the new columns 0x6f5320 + cols * 0x58 */
                for (n = 0; n < U_I((unsigned)&ai_Unk76fdc4) && cols < 0x10; n++) {
                    id = U_I((unsigned)&ai_Unk889400 + n * 4);
                    if (*(char *)(U_I((unsigned)&ai_Goods) + id * 0x40) == 0x17)
                        continue;
                    add = 1;
                    for (i = 0; i < 4; i++) {
                        w = *(unsigned short *)(U_I((unsigned)&ai_Goods) + id * 0x40 + 0x2e + i * 2);
                        if (w != 0) {
                            b = gm_Query(*(int *)(rec1 + 0x14), 1, 0, w);
                            if (b == 0 || (U_I(b + 0xe) != 2 && *(char *)param_1 != 0x1e)) {
                                add = 0;
                                break;
                            }
                        }
                    }
                    if (add) {
                        U_I((unsigned)&full_data_6f5320 + cols * 0x58) = id;
                        cols++;
                    }
                }
            }
            if (cols > 0) {
                /* ROUGH: the prices / amounts per column as in 0x5923c0 (0x91aaf0, the words +8 / +0,
                   0x50aaf0, +0x36), stored at +0x50 / +0x54 */
                for (col = 0; col < cols; col++) {
                    U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x50) = 0x7fffffff;
                    for (i = 0; i < 4; i++) {
                        v = *(unsigned short *)(0x26 + i * 2 + U_I((unsigned)&full_data_6f5320 + col * 0x58) * 0x41 +
                                                8 + U_I((unsigned)&ai_Goods));
                        U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x14 + i * 4) = v;
                        U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x24 + i * 4) =
                            *(unsigned short *)(0x26 + i * 2 + U_I((unsigned)&full_data_6f5320 + col * 0x58) * 0x41 +
                                                U_I((unsigned)&ai_Goods));
                    }
                    b = gm_Query(*(int *)(rec2 + 0x14), 1, 0, U_I((unsigned)&full_data_6f5320 + col * 0x58));
                    U_I((unsigned)&full_data_6f5320 + col * 0x58 + 0x54) = b == 0 ? 0 : gm_ListAmount(rec2, b);
                }
            }
            ai_Unk5e56c4(cols, a, 10);
            d2_SetObjText(a[2], a);
            ai_Unk58cc10(win, 2, cols, rec2, 2);
            ai_Unk58d720(param_1, rec2);
            ai_Unk58dd00(win, 1, 0, param_1);
            n = sv_RunFrame(ai_Unk593150, 0x677c7);
        } while (n != 0);
        d2_CloseView(win);
        sv_ClosePlayerBar();
    }
    return 0;
}
