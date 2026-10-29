/* 0x5c2b90, the master letter window "berufe\meisterbrief_2" 0x62d2d4 (1648 B, ROUGH): the production
   settings record 0x5a8540 (param_1 + 1) or 0x5a8560 (param_1) of the character, else 0x6f2f90 = 0, and
   0x5a8850 (param_1); the window 0x4741f0 (0, 0, 0x62d2d4), 0x52ea30 (screen), the percent 0x5085b0
   (param_1 + 0x39, 0x6f1bc5); the page 0: the checkboxes 0x5c2490 (win, &a, &b, aiStack_48, param_1,
   *param_2, the flag word of param_2 + 0xda, &master, recs), the page 1: the seven rows 0x5c2a10 (win,
   recs, local_38, the sliders 0x5c2a1c); the two page tabs 0x475f70 (win, 3) / 0x52e350 (screen, 0,
   &tab1, 2, 0x143df40, 0x144a714). The frame loop 0x5309a0 (0x5c2b90, 0x677c7): the row clicks
   (0x475980 the value, 0x5a8710 the weight, 0x475cb0 (ctrl, 0, 100, the byte +4)), the master slider
   (0x5a8900 (param_1, the percent * 0.01)), the checkbox clicks (the byte +5 toggled, the control record
   0xf8f080 + id * 0x2ec + 0x28, 0x475e40), the page switches (0x6f2f90 0 / 1) and the four order flags
   (0x4c3ba0 / 0x4c3c00 / 0x4c49a0 on param_1 + 0x39 and param_2 + 0x1b4 / + 0x1b8); the exit writes
   0x5085f0 (b, 0x6f1bc5) back into param_1 + 0x39 and the delta into param_2 + 0x1b8, then 0x4758f0
   ROUGH: the 0x5c2490 call passes four flag words in the original (the callee reads one) */

void ai_Unk5c2b90(char *param_1, short *param_2)
{
    int win;
    char *recs;
    int flags;
    int f2;
    int f3;
    int f4;
    int tab1;
    int tab2;
    int master;
    int percent;
    int a;
    int b;
    int v;
    int pics[7];
    int sliders[7];
    int checks[4];
    int i;
    int j;
    int mode;

    mode = 0;
    master = -1;
    tab2 = -1;
    flags = -1;
    recs = (char *)ai_Unk5a8540(*(int *)(param_1 + 1));
    for (i = 0; i < 7; i++)
        pics[i] = -1;
    if (recs == 0 && (recs = (char *)ai_Unk5a8560(param_1)) == 0) {
        mode = -1;
        U_I((unsigned)&full_data_6f2f90) = 0;
    } else {
        ai_Unk5a8850(param_1);
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2d4);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    a = gm_ConvertMoney(*(int *)(param_1 + 0x39), U_I((unsigned)&ai_OwnPlayer));
    flags = U_I((char *)param_2 + 0x1b4);
    f2 = U_I((char *)param_2 + 0x1b8);
    f3 = U_I((char *)param_2 + 0x1bc);
    f4 = U_I((char *)param_2 + 0x1c0);
    if (U_I((unsigned)&full_data_6f2f90) == 0) {
        ai_Unk5c2490(win, &b, &a, checks, (unsigned char *)param_1, *param_2, flags, &master, (int)recs);
    } else {
        ai_Unk5c2a10(win, recs, pics, sliders);
    }
    if (mode != -1) {
        d2_SetForm(win, 3);
        sv_Unk52e350(U_I((unsigned)&d2_LastWindow), 0, &tab1, 2, (void *)(unsigned)&full_data_143df40, (void *)(unsigned)&full_data_144a714);
        tab2 = tab1;
        /* ROUGH: local_64 = the second id the 0x52e350 block gives (the second tab control) */
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I(((unsigned)&dm_L + 0x10)) != 0) {
                if (U_I((unsigned)&full_data_6f2f90) == 1) {
                    for (i = 0; i < 7; i++) {
                        if (U_I((unsigned)&ai_Unk5fe710) == pics[i]) {
                            if (pics[i] != -1) {
                                U_I((unsigned)&d2_ClickedId) = -1;
                                v = d2_GetObjValue(pics[i]);
                                ai_Unk5a8710(param_1, *(int *)(recs + i * 8), v);
                                for (j = 0; j < 7; j++) {
                                    if (pics[j] != -1)
                                        d2_SetObjValue(pics[j], 0, 100, *(unsigned char *)(recs + j * 8 + 4));
                                }
                            }
                            break;
                        }
                    }
                } else if (U_I((unsigned)&full_data_6f2f90) == 0 && master != -1 && U_I((unsigned)&ai_Unk5fe710) == master) {
                    v = d2_GetObjValue(master);
                    ai_Unk5a8900(param_1, (float)v * 0.01);
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (U_I(((unsigned)&dm_L + 0x18)) != 0 && U_I((unsigned)&ai_Unk5fe710) != -1) {
            for (j = 0; j < 7; j++) {
                if (U_I((unsigned)&ai_Unk5fe710) == sliders[j]) {
                    unsigned char c = *(unsigned char *)(recs + j * 8 + 5);
                    *(unsigned char *)(recs + j * 8 + 5) = c == 0;
                    U_I(U_I((unsigned)&d2_Objs) + 0x28 + sliders[j] * 0x2ec) = c != 0;
                    d2_SetObjEnabled(pics[j], *(unsigned char *)(recs + j * 8 + 5) == 0);
                }
            }
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&full_data_6f2f90) == 1) {
                if (U_I((unsigned)&ai_Unk5fe710) == tab2) {
                    /* ROUGH: 0x5c2490 (win, &a, &b, checks, param_1, *param_2, flags, &master, recs) again;
                       then 0x6f2f90 = 0, 0x5fe710 = -1 */
                    ai_Unk5c2490(win, &b, &a, checks, (unsigned char *)param_1, *param_2, flags, &master, (int)recs);
                    U_I((unsigned)&full_data_6f2f90) = 0;
                    U_I((unsigned)&ai_Unk5fe710) = -1;
                } else {
                    for (i = 0; i < 7; i++) {
                        if (U_I((unsigned)&ai_Unk5fe710) == pics[i]) {
                            if (pics[i] != -1) {
                                v = d2_GetObjValue(pics[i]);
                                ai_Unk5a8710(param_1, *(int *)(recs + i * 8), v);
                                for (j = 0; j < 7; j++) {
                                    if (pics[j] != -1)
                                        d2_SetObjValue(pics[j], 0, 100, *(unsigned char *)(recs + j * 8 + 4));
                                }
                            }
                            break;
                        }
                    }
                }
            } else if (U_I((unsigned)&full_data_6f2f90) == 0) {
                a = d2_GetObjValue(b);
                if (U_I((unsigned)&ai_Unk5fe710) == tab1) {
                    ai_Unk5c2a10(win, recs, pics, sliders);
                    U_I((unsigned)&full_data_6f2f90) = 1;
                    U_I((unsigned)&ai_Unk5fe710) = -1;
                } else {
                    for (j = 0; j < 4; j++) {
                        if (U_I((unsigned)&ai_Unk5fe710) == -1)
                            break;
                        if (U_I((unsigned)&ai_Unk5fe710) == checks[j]) {
                            flags = U_I((char *)param_2 + 0x1b4);
                            f2 = U_I((char *)param_2 + 0x1b8);
                            f3 = U_I((char *)param_2 + 0x1bc);
                            f4 = U_I((char *)param_2 + 0x1c0);
                            if (d2_GetObjValue(checks[j]) == 0)
                                flags &= ~(1 << j);
                            else
                                flags |= 1 << j;
                            cm_BeginFieldBlock((char *)param_2, *(int *)((char *)param_2 + 2));
                            cm_AddFieldDelta(4, 1, (char *)param_2 + 0x1b4 - U_I((unsigned)&cm_FieldObject), &flags);
                            cm_SendFieldDeltas();
                            cm_BeginFieldBlock(param_1, *(int *)(param_1 + 1));
                            cm_AddFieldDelta(4, 1, param_1 + 0x99 - U_I((unsigned)&cm_FieldObject), &flags);
                            cm_SendFieldDeltas();
                            ai_Unk5c2490(win, &b, &a, checks, (unsigned char *)param_1, *param_2, flags,
                                         &master, (int)recs);
                            U_I((unsigned)&ai_Unk5fe710) = -1;
                        }
                    }
                }
            }
        }
        if (sv_RunFrame(ai_Unk5c2b90, 0x677c7) == 0)
            break;
    }
    if (U_I((unsigned)&full_data_6f2f90) == 0)
        a = d2_GetObjValue(b);
    a = gm_PlayerFactor(a, U_I((unsigned)&ai_OwnPlayer));
    cm_BeginFieldBlock(param_1, *(int *)(param_1 + 1));
    cm_AddFieldDelta(4, 1, param_1 + 0x39 - U_I((unsigned)&cm_FieldObject), &a);
    cm_SendFieldDeltas();
    v = a - *(int *)(param_1 + 0x39);
    cm_BeginFieldBlock((char *)param_2, *(int *)((char *)param_2 + 2));
    cm_AddFieldValues(4, 1, (char *)param_2 + 0x1b8 - U_I((unsigned)&cm_FieldObject), &v);
    cm_SendFieldDeltas();
    d2_CloseView(win);
}
