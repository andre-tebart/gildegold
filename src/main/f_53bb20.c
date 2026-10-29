/* 0x53bb20, the main game loop of the process (the dialog callback of the process level, `ctl` = 0 on the first call): the start-up / load of the game by the mode flags 0x6f23f8 (1 new game, 2 load, 4 network, 8, 0x10, 0x40 .., 0x80), the "Init" export of the optional
   plug-in DLL (0x75dda0), the autosave, then the round loop: one player at a time (0x6f25b8) the day change 0x5384e0, the start of the round 0x53b2e0 ... until the game is left (0x6f259c); ends with the network error / player left messages (ROUGH) */

void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b);

void main_GameLoop(int ctl)
{
    int iv, i6, i5;
    int w;
    FARPROC pf;
    char *pc;
    int *pi;
    unsigned short us;
    unsigned u;
    int cl[0x3e];
    int ck[4];
    int clk[4];
    char ap[0x104], ab[0x104];
    int pl[0x45];
    int idx;
    int *pp;
    int fx, fy, fh;
    int dst;
    unsigned char *p3;
    unsigned short *p4;

    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && ctl == 0)
        amt_InitTable();
    U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
    if ((U_I((unsigned)&ai_GameFlags) & 4) == 0) {
        net_Connect(0, 0);
        iv = main_Unk53b830();
        if (iv != 1) {
            while (1) {
                U_I((unsigned)&d2_CutsceneActive) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x24087;
                txt_Format(cl, 0x79);
                iv = sv_MessageBox(cl, 1);
                if (iv == 0)
                    break;
                U_I((unsigned)&d2_CutsceneActive) = 1;
                d2_DrawAll();
                Sleep(1000);
            }
            U_I((unsigned)&ai_GameOver) = 1;
            U_I((unsigned)&ai_LeaveGame) = 0;
            return;
        }
    } else {
        w = -1;
        ai_Unk4b1f90();
        if ((U_I((unsigned)&ai_GameFlags) & 0x10) != 0) {
            iv = main_Unk53b830();
            if (iv != 1) {
                while (1) {
                    U_I((unsigned)&sv_FrameMask) = 0x24087;
                    txt_Format(cl, 0x79);
                    iv = sv_MessageBox(cl, 1);
                    if (iv == 0)
                        break;
                    d2_DrawAll();
                    Sleep(1000);
                }
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            U_I(((unsigned)&ls_SavePath + 0x80)) = (int)LoadLibraryA((char *)((unsigned)&ai_DataPath + 0x100));
            if (U_I(((unsigned)&ls_SavePath + 0x80)) == 0) {
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            w = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
            d2_CentreViewWindows(w);
            io_PrintText(0x3ff6);
            sv_RunFrame(0, 0x24087);
            pf = GetProcAddress((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)), "Init");
            if (pf == 0) {
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                d2_CloseView(w);
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            iv = ((int (*)(int))pf)(((unsigned)&ls_SavePath + 0xa0));
            if (iv == 0) {
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                d2_CloseView(w);
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            Sleep(0x9c4);
            udp_Init(0x3039);
            U_I((unsigned)&full_data_6f25c4) = 1;
        }
        net_Connect((unsigned)&full_data_75e1c0, U_I(((unsigned)&ls_SavePath + 0x84)));
        d2_CloseView(w);
    }
    ai_Unk4c3480();
    if ((U_I((unsigned)&ai_GameFlags) & 0x44) != 0) {
        U_I((unsigned)&d2_CutsceneActive) = 0;
        iv = ai_Unk589f10();
        if (iv != 0) {
            if (U_I(((unsigned)&ls_SavePath + 0x80)) != 0) {
                Sleep(1000);
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
            }
            U_I((unsigned)&ai_LeaveGame) = 0;
            U_I((unsigned)&ai_GameOver) = 1;
            return;
        }
    }
    U_I((unsigned)&d2_CutsceneActive) = 1;
    main_Unk537980();
    d2_SetForm(U_I((unsigned)&init_StatusForm), 1);
    w = U_I(((unsigned)&d2_Strings + 0x1b4));
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
        w = U_I(((unsigned)&ai_DifficultyTexts + 0x84));
    main_SetLoadProgress(w, 5);
    sv_PumpMessages();
    main_SetLoadProgress(U_I(((unsigned)&d2_Strings + 0x1b0)), 5);
    main_Unk537de0();
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && U_I((unsigned)&full_data_6f2428) != 0)
        main_Unk53b840();
    main_SetLoadProgress(0, 10);
    if ((U_I((unsigned)&ai_GameFlags) & 0x41) == 0) {
        if ((U_I((unsigned)&ai_GameFlags) & 0x40) == 0) {
            if ((U_I((unsigned)&ai_GameFlags) & 2) != 0) {
                U_I((unsigned)&ai_Difficulty) = *(unsigned char *)((unsigned)&ai_GamePanel + 0x2);
                iv = unk_Unk51c950((unsigned)&full_data_75e360, 1);
                if (iv == 0) {
                    U_I((unsigned)&sv_FrameMask) = 0x46086;
                    sv_MessageBox(U_I((unsigned)&full_data_14414d0), 0);
                    U_I((unsigned)&ai_GameOver) = 1;
                    main_Unk537f40();
                    main_Unk537be0();
                    return;
                }
                sv_Unk525a00((unsigned)&ai_Time);
                U_I((unsigned)&full_data_760454) = gm_ClockSeason((unsigned)&ai_Time);
            }
        } else {
            U_I((unsigned)&ai_Difficulty) = 2;
            if (U_I((unsigned)&ai_Unk1460ae0) == -1)
                goto L_fail;
            unk_Unk5112d0((unsigned)&full_data_75e360);
        }
    } else if ((U_I((unsigned)&ai_GameFlags) & 4) == 0) {
        sprintf(ap, "%s/%s.cty", "gamedata/cities", (char *)((unsigned)&ai_DataPath + 0x220));
        iv = unk_Unk51c950(ap, 2);
        if (iv == 0) {
            main_Exit();
            return;
        }
        main_Unk53b840();
        init_SetupNewGame(*(unsigned char *)(unsigned)&ai_OwnPlayer);
        init_LocalData();
        init_FillMarkets(*(unsigned char *)(unsigned)&ai_OwnPlayer);
        main_SetLoadProgress(0, 100);
        main_SetLoadProgress(0, 100);
        if (U_I((unsigned)&ai_Unk6f2440) != 0)
            unk_Unk511c70();
    } else {
        if (U_I((unsigned)&ai_Unk1460ae0) == -1) {
L_fail:
            sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x80)), 0);
            main_Unk537f40();
            main_Unk537be0();
            return;
        }
        if ((U_I((unsigned)&ai_GameFlags) & 0x40) == 0) {
            init_StartNetworkGame((unsigned)&full_data_75e360);
            U_I((unsigned)&ai_Difficulty) = 2;
        } else {
            unk_Unk5112d0(((unsigned)&ls_SavePath + 0xca));
            U_I((unsigned)&ai_Difficulty) = 2;
        }
    }
    unk_Unk5124d0();
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0) {
        if ((U_I((unsigned)&ai_GameFlags) & 4) == 0) {
            clk[0] = 0;
            gm_SetTime(clk, 6, 0, 0);
            w = cm_SendExSysMessage(3, clk);
            iv = cm_GetSlotState(w);
            while (iv == 0) {
                sv_PumpNet();
                iv = cm_GetSlotState(w);
            }
            if (U_I((unsigned)&main_MoneyCheat) == 0)
                iv = (5 - U_I((unsigned)&ai_Difficulty)) * 0xfa;
            else
                iv = 750000;
            pc = (char *)((unsigned)&ai_Chars + 0x2);
            do {
                if (pc[6] != 0 && (*pc == 6 || *pc == 7)) {
                    w = gm_PlayerFactor(iv, *(unsigned char *)(unsigned)&ai_OwnPlayer);
                    cm_SendMoveGoods(*(int *)(pc + 2), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, w);
                }
                pc += 0x21c;
            } while ((int)pc < (unsigned)((unsigned)ai_Chars + 0x65402));
        }
        if ((U_I((unsigned)&ai_GameFlags) & 1) != 0)
            ai_Unk4b1d20();
    }
    sv_PumpMessages();
    ai_Unk5b2090();
    sv_PumpMessages();
    main_Unk537be0();
    U_I((unsigned)&d2_CutsceneActive) = 0;
    w = sv_Unk522040();
    sv_Unk521e70(w);
    d3if_SelectScene(0, 0);
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && (U_I((unsigned)&ai_GameFlags) & 0x40) == 0) {
        unk_Unk511790();
        unk_Unk511840();
        ai_Unk589e50();
    }
    ai_ApplyOptions();
    d3_SetViewport((float)(U_I((unsigned)&sv_ViewRectRight) - U_I((unsigned)&sv_ViewRectLeft)), (float)(U_I((unsigned)&sv_ViewRectBottom) - U_I((unsigned)&sv_ViewRectTop)), (float)U_I((unsigned)&sv_ViewRectLeft), (float)U_I((unsigned)&sv_ViewRectTop),
                 *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH);
    if ((U_I((unsigned)&ai_GameFlags) & 2) != 0 && (sv_Unk525510((unsigned)&ai_Time, 0), U_I((unsigned)&full_data_76f990) != -1)) {
        p3 = (unsigned char *)gm_FindBuildingById(U_I((unsigned)&full_data_76f990));
        p4 = (unsigned short *)gm_FindObjectInst(U_I((unsigned)&full_data_76f994));
        U_I((unsigned)&ai_FrameWait) = 2;
        if (p3 == 0 || p4 == 0) {
            init_LoadBuildingScene(*p3);
            w = -1;
        } else {
            init_LoadObjectScene(*p4);
            w = *(int *)(p4 + 1);
        }
        sv_SelectPanelObject(p3, w, (unsigned)&ch_EmptyName);
    }
    us = *(unsigned short *)(unsigned)&ai_PlayerId;
    while (us < 0x300 && U_I((unsigned)&ai_GameOver) == 0 && *(char *)(((unsigned)&ai_Chars + 0x2) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) != 6) {
        us++;
        *(unsigned short *)(unsigned)&ai_PlayerId = us;
    }
    if ((char)U_I((unsigned)&ai_GameFlags) < 0) {
        ai_Unk578180();
        memset(cl, 0, 0xf8);
        /* `cl` is an int array, so the byte offsets must not pick up the int scaling of MSG_B / MSG_I. */
        MSG_B((char *)cl, 4) = 0x87;
        MSG_I((char *)cl, 0xc) = -1;
        MSG_B((char *)cl, 0x36) = 9;
        MSG_I((char *)cl, 8) = U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        cm_SendLongMessage(cl);
    }
    while (U_I((unsigned)&ai_GameOver) == 0 && (iv = sv_RunFrame((int)main_GameLoop, 0x20007)) != 0 && U_I((unsigned)&full_data_6f2598) < 800) {
        iv = 0;
        U_I((unsigned)&ai_CurrentPlayer) = 0;
        if ((U_I((unsigned)&ai_GameFlags) & 0x42) == 0) {
            *(unsigned short *)(unsigned)&ai_PlayerId = 0;
            U_I((unsigned)&ai_CurrentPlayer) = 0;
            main_Unk538140();
        } else {
            u = U_I((unsigned)&ai_PlayerId) & 0xffff;
            if (u != 0) {
                pc = (char *)((unsigned)&ai_Chars + 0x2);
                do {
                    if (*pc == 6)
                        iv++;
                    pc += 0x21c;
                    u--;
                    U_I((unsigned)&ai_CurrentPlayer) = iv;
                } while (u != 0);
            }
        }
        us = *(unsigned short *)(unsigned)&ai_PlayerId;
        while (us < 0x300 && U_I((unsigned)&ai_GameOver) == 0 && *(char *)(((unsigned)&ai_Chars + 0x2) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) != 6) {
            us++;
            *(unsigned short *)(unsigned)&ai_PlayerId = us;
        }
        if (*(char *)(((unsigned)&ai_Chars + 0x2) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) == 6) {
            if ((U_I((unsigned)&ai_GameFlags) & 0x46) == 0) {
                clk[1] = U_I((unsigned)&ai_TimeHour);
                clk[0] = U_I((unsigned)&ai_Time);
                clk[2] = U_I(((unsigned)&ai_Time + 0x8));
                *(short *)(clk + 3) = *(short *)((unsigned)&ai_Time + 0xc);
                gm_SetTime(clk, 6, 0, 0);
                w = cm_SendExSysMessage(3, clk);
                iv = cm_GetSlotState(w);
                while (iv == 0) {
                    sv_PumpNet();
                    iv = cm_GetSlotState(w);
                }
            }
            if ((U_I((unsigned)&ai_GameFlags) & 0x42) == 0) {
                main_Unk5384e0();
            } else {
                unk_Unk513740();
                sv_RedrawScreen();
                if ((U_I((unsigned)&ai_GameFlags) & 4) != 0) {
                    cm_SendExSysMessage(1, 0);
                    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
                        sv_NetworkSyncWait(0x67fff);
                }
            }
            u = U_I((unsigned)&ai_GameFlags) & 0xff;
            if ((u & 2) == 0) {
                cm_BeginSendGroup();
                if ((U_I((unsigned)&ai_GameFlags) & 8) != 0 || (U_I((unsigned)&ai_Unk1460ae0) != -1 && (U_I((unsigned)&cm_NetFlags) & 8) != 0)) {
                    he_Unk541010();
                    he_Unk541060();
                    he_Unk541170();
                }
                he_Unk5410f0();
                cm_EndSendGroup();
                iv = cm_SendGroupState();
                while (iv == 0) {
                    sv_PumpNet();
                    iv = cm_SendGroupState();
                }
                u = U_I((unsigned)&ai_GameFlags) & 0xff;
            }
            if ((u & 0x42) == 0 && U_I((unsigned)&full_data_62456c) != 0 && U_I((unsigned)&ai_GameOver) == 0 && (char)u >= 0) {
                U_I((unsigned)&ls_SaveSlot) = 0;
                if ((u & 4) == 0) {
                    sprintf(ap, "%s\\gamedata\\saves\\autosave.sav", "\\project\\gfx\\");
                    i5 = fopen(ap, "rb");
                    if (i5 != 0) {
                        sprintf(ab, "%s\\gamedata\\saves\\autosave.bak", "\\project\\gfx\\");
                        fseek(i5, 0, 2);
                        w = ftell(i5);
                        fseek(i5, 0, 0);
                        i6 = mem_Alloc(w, (unsigned)&ai_Unk60c1ec);
                        iv = fopen(ab, "wb");
                        if (i6 != 0) {
                            if (iv != 0) {
                                fread(i6, w, 1, i5);
                                fwrite(i6, w, 1, iv);
                                fclose(iv);
                            }
                            mem_Free(i6);
                        }
                        fclose(i5);
                    }
                    sprintf(ap, "%s/standard.scr", "gamedata/cities");
                    ai_LoadScreenImage(ap);
                    ls_SaveGame("Gamedata\\Saves\\Autosave.SAV", "Autosave", 1);
                } else if ((u & 0x10) != 0) {
                    memset(cl, 0, 0x7c);
                    strcpy((char *)cl + 4, "AUTOSAVE");
                    cl[0] = 0;
                    w = cm_SendExSysMessage(0xf, cl);
                    iv = cm_GetSlotState(w);
                    while (iv == 0) {
                        sv_PumpNet();
                        iv = cm_GetSlotState(w);
                    }
                }
            }
            he_Unk53d9f0(U_I((unsigned)&ai_PlayerId) & 0xffff);
            U_I((unsigned)&ai_Unkf31144) = 0;
            U_I((unsigned)&full_data_6f25a8) = 0;
            ai_GameFrame();
            if (U_I((unsigned)&ai_GameOver) == 0) {
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 1);
                if ((U_I((unsigned)&ai_GameFlags) & 8) == 0) {
                    ck[0] = U_I((unsigned)&ai_Time);
                    ck[1] = U_I((unsigned)&ai_TimeHour);
                    ck[2] = U_I(((unsigned)&ai_Time + 0x8));
                    *(short *)(ck + 3) = *(short *)((unsigned)&ai_Time + 0xc);
                    us = (unsigned short)U_I((unsigned)&ai_TimeHour);
                    while (us < 0x17) {
                        gm_AddTime(ck, 0, 0x1e, 0);
                        w = ai_Unk4c4d40(ck);
                        iv = cm_GetSlotState(w);
                        while (iv == 0) {
                            sv_PumpNet();
                            iv = cm_GetSlotState(w);
                        }
                        us = (unsigned short)ck[1];
                    }
                }
                main_Unk539870();
                if (U_I((unsigned)&ai_GameOver) == 0 && U_I((unsigned)&ai_LeaveGame) == 0) {
                    U_I((unsigned)&ai_GameFlags) &= 0xffffffbd;
                    U_I((unsigned)&ai_CurrentPlayer)++;
                    goto L_next;
                }
            }
            U_I((unsigned)&ai_CurrentPlayer)++;
        }
L_next:
        if (U_I((unsigned)&ai_GameOver) == 0) {
            if (U_I((unsigned)&ai_Unk1460ae0) == -1) {
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 1);
                ck[0] = U_I((unsigned)&ai_Time);
                ck[1] = U_I((unsigned)&ai_TimeHour);
                ck[2] = U_I(((unsigned)&ai_Time + 0x8));
                *(short *)(ck + 3) = *(short *)((unsigned)&ai_Time + 0xc);
                us = (unsigned short)U_I((unsigned)&ai_TimeHour);
                while (us < 0x17) {
                    gm_AddTime(ck, 0, 0x1e, 0);
                    w = ai_Unk4c4d40(ck);
                    iv = cm_GetSlotState(w);
                    while (iv == 0) {
                        sv_PumpNet();
                        iv = cm_GetSlotState(w);
                    }
                    us = (unsigned short)ck[1];
                }
                gm_AddTime(ck, 0x18, 0, 0);
                gm_SetTime(ck, 6, 0, 0);
                ai_Unk4c4d40(ck);
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 0);
            }
            if ((U_I((unsigned)&cm_NetFlags) & 8) != 0 || U_I((unsigned)&ai_Unk1460ae0) == -1) {
                iv = amt_CheckOfficeColumns();
                if (iv == 0) {
                    amt_UpdateOffices();
                } else {
                    memset(pl, 0, 0x114);
                    idx = 0;
                    pp = (int *)(unsigned)&ai_CharInts;
                    do {
                        if (*((char *)pp - 2) == 6) {
                            w = *pp;
                            pl[3] = w;
                            pl[0x0d + idx] = w;
                            idx++;
                        } else if (*((char *)pp - 2) == 7) {
                            pl[0x0d + idx] = *pp;
                            idx++;
                        }
                        pp += 0x87;
                    } while ((int)pp < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
                    *((char *)pl + 0x30) = (char)idx;
                    pl[7] = U_I((unsigned)&ai_TimeHour);
                    pl[6] = U_I((unsigned)&ai_Time);
                    U_I((unsigned)&sv_FrameMask) = 0x24087;
                    *((char *)pl + 8) = 1;
                    *((char *)pl + 0x26) = 0x61;
                    pl[5] = -1;
                    pl[8] = U_I(((unsigned)&ai_Time + 0x8));
                    *(short *)(pl + 9) = *(short *)((unsigned)&ai_Time + 0xc);
                    w = ai_Unk4c4fa0(pl);
                    iv = cm_GetSlotState(w);
                    while (iv == 0) {
                        sv_PumpNet();
                        iv = cm_GetSlotState(w);
                    }
                }
            }
            if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
                sv_NetworkSyncWait(0x24087);
            main_Unk53b2e0();
        }
        U_I((unsigned)&full_data_6f2598)++;
    }
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0) {
        if (U_I((unsigned)&ai_Unk60bd94) == -1) {
            pc = "$3AEs trat ein Netzwerkfehler auf! Das Spiel wurde beendet...$2A%ia[Okay]";
        } else {
            if (U_I((unsigned)&ai_Unk60bd94) == U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c))
                goto L_end;
            pc = "$3AEin Spieler hat die Partie verlassen. Das Spiel wurde beendet...$2A%ia[Okay]";
        }
        sv_MessageBox(pc, 0);
    }
L_end:
    if ((char)U_I((unsigned)&ai_GameFlags) < 0)
        ai_Unk578370();
    main_Unk537f40();
}
