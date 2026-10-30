/* 0x5384e0, the round / day change of the player (0x6f25b8): fade out (BLACK), the clock 0x770ae0 (+1 when 0x6f23f8 bit 2 and 0x6f2598 > 1), the announcement messages of the kinds 0x66 / 0x70 / 0x68 / 0x85 / 0x7c / 0x93 / 0x84 /
   0x4c queued when the character has none (0x53e400), the summary page of the balance of the player (0x515960 texts 0x1ef9 .. 0x1eff, the table 0x758ca4 of 0x100 records of 0x50 B), the renovation check of the
   buildings (kind 0x1c message "renovieren") and the distribution of the work load (+0x39 of the buildings), the figure of the character (0x770d04) at the end; ROUGH (the float expressions
   of the renovation, the __ftol arguments, are guesses) */

/* message record (0xf8 bytes) of 0x4c4b70 / 0x4e4060: kind byte +4, owner id +8, target +0xc, time stamp (the 0x770ae0 clock, 14 bytes) +0x28 .. +0x36, flag +0x36 = 1 */
#define MSG_B(m, o) (*(unsigned char *)((m) + (o)))
#define MSG_I(m, o) (*(int *)((m) + (o)))
#define MSG_CLOCK(m) do { MSG_I(m, 0x28) = U_I((unsigned)&ai_Time); MSG_I(m, 0x2c) = U_I((unsigned)&ai_TimeHour); MSG_I(m, 0x30) = U_I(((unsigned)&ai_Time + 0x8)); *(short *)((m) + 0x34) = *(short *)((unsigned)&ai_Time + 0xc); } while (0)
#define MAIN_MSG(m, kind) do { memset(m, 0, 0xf8); MSG_B(m, 4) = (kind); MSG_I(m, 0xc) = -1; MSG_B(m, 0x36) = 1; MSG_CLOCK(m); MSG_I(m, 8) = U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c); } while (0)

void main_Unk5384e0(void)
{
    char msg[0xf8];
    char clk[14];
    char ln[0x21c];   /* a whole character record: gm_Unk50af00 (0x50af00) memsets 0x21c bytes into it, so
                         0x100 zeroed the saved ebp / return address of this frame (the crash after the
                         round-change page); the original's frame is 0x3b8 and its buffer sits at +0x1a8 */
    char *c;
    unsigned char *bp;
    unsigned char *fade;
    unsigned char *o;
    int bld, r, w, uu, uv, i, k, n, id, q, u2;
    float fa, fb;

    id = U_I((unsigned)&ai_PlayerId) & 0xffff;
    fade = 0;
    bld = gm_PlayerRecord((unsigned)&ai_Chars + id * 0x21c);
    gm_ClockSeason((unsigned)&ai_Time);
    memcpy(clk, (char *)(unsigned)&ai_Time, 14);
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0 && 1 < U_I((unsigned)&full_data_6f2598))
        *(int *)clk = U_I((unsigned)&ai_Time) + 1;
    gm_ClockSeason(clk);
    ahm_FillSteps();
    *(float *)(bld + 0x58) = 0.5f;
    *(float *)(bld + 0x5c) = 1.0f;
    *(float *)(bld + 0x60) = 1.0f;
    if (U_I((unsigned)&full_data_624560) != 0) {
        U_I((unsigned)&ai_Unkfb9060) = (int)*(short *)(unsigned)&d2_ScreenW;
        U_I((unsigned)&ai_Unkfb906c) = (int)*(short *)(unsigned)&d2_ScreenH;
        U_I((unsigned)&ai_Unkfb9068) = 0;
        U_I((unsigned)&ai_Unkfb9064) = 0;
        if (U_I((unsigned)&full_data_6f25c0) == 0) {
            fade = (unsigned char *)d2_StartFade(0, 0, U_I((unsigned)&ai_Unkfb9060), U_I((unsigned)&ai_Unkfb906c), "BLACK", 0, 1);
            while ((*fade & 4) == 0)
                sv_RunFrame(0, 0x24087);
            d2_ShowPicture(0x79d);
            d2_ReleaseFade(fade);
            d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x32, 10);
        }
    }
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    gm_Unk50af00(id, ln);
    sv_Unk525a00(clk);
    U_I((unsigned)&full_data_760454) = gm_ClockSeason(clk);
    gm_FindRichest(ln + 0x2a, &k);
    k = 0;
    q = cm_SendExSysMessage(0, clk);
    while (cm_GetSlotState(q) == 0)
        sv_PumpNet();
    tb_SetFunctionOff((int)sv_UpdateClock, 1);
    if ((char)U_I((unsigned)&ai_GameFlags) >= 0) {
        if (he_FindEvent(1, 0, 0x66) == 0) {
            MAIN_MSG(msg, 0x66);
            cm_SendLongMessage(msg);
        }
        if (he_FindEvent(1, 0, 0x70) == 0) {
            MAIN_MSG(msg, 0x70);
            cm_SendLongMessage(msg);
        }
        if (he_FindEvent(1, 0, 0x68) == 0) {
            MAIN_MSG(msg, 0x68);
            cm_SendLongMessage(msg);
        }
    }
    if (U_I((unsigned)&ai_Unk6f2438) != 0 && he_FindEvent(1, 0, 0x85) == 0) {
        MAIN_MSG(msg, 0x85);
        cm_SendLongMessage(msg);
    }
    if (he_FindEvent(1, 0, 0x7c) == 0) {
        MAIN_MSG(msg, 0x7c);
        cm_SendLongMessage(msg);
    }
    if (he_FindEvent(1, 0, 0x93) == 0) {
        MAIN_MSG(msg, 0x93);
        cm_SendLongMessage(msg);
    }
    if (he_FindEvent(1, 0, 0x84) == 0 && *(char *)((unsigned)&ai_GamePanel + 0x3) != 0) {
        MAIN_MSG(msg, 0x84);
        cm_SendLongMessage(msg);
    }
    if (U_I((unsigned)&sv_PanelForm) != -1)
        d2_SetViewEnabled(U_I((unsigned)&sv_PanelForm), 0);
    he_Unk53d9f0(0xffff);
    if (U_I((unsigned)&full_data_624560) != 0) {
        int win = sv_Unk52fa10();
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), d2_GetViewWin(win, 1), 0x767);
        w = d2_GetViewWin(win, 1);
        U_I(((unsigned)&ai_Unkf8f13c + 0x248) + w * 0x3c4) = 0x18;
        d2_SetForm(win, 1);
        w = (int)(0.0f);
        io_PrintText(0x2051, clk, w % 0xc);
        io_PrintText(0x1ef9);
        io_PrintText(0x1eff, U_I(((unsigned)&ai_Selected + 0x8) + id * 0x21c));
        k = 0;
        do {
            bp = (unsigned char *)(((unsigned)&he_RunHandlers + 0x6a4) + k * 0x50);
            n = *(int *)(bp + 4);
            if ((n != 0 || *(int *)(bp + 8) != 0 || bp[0xc] != 0) && *(int *)bp == id) {
                if (n == 0) {
                    n = *(int *)(bp + 8);
                    if (n < 1) {
                        if (n < 0)
                            io_PrintText(0x1efd, bp + 0xc, n);
                        else
                            io_PrintText(0x1efb, bp + 0xc);
                    } else
                        io_PrintText(0x1efe, bp + 0xc, n);
                } else if (n < 0)
                    io_PrintText(0x1efb, bp + 0xc, n);
                else
                    io_PrintText(0x1efc, bp + 0xc, n);
            }
            k++;
        } while (k < 0x100);
        k = 0;
        while (ln[0x2e] != 0 && k < 9) {
            io_PrintText(0x1efd, (unsigned char)ln[0x2e + k * 5] + 0x253, *(int *)(ln + 0x2f + k * 5));
            k++;
            ln[0x2e] = ln[0x2e + k * 5];
        }
        io_PrintText(0x1efa, U_I(((unsigned)&ai_Selected + 0xc) + id * 0x21c));
        r = sv_RunFrame((int)main_Unk5384e0, 0x24087);
        while (r != 0) {
#ifdef REA_START
            /* REA_START=tutorial (`uv run play|dbg --start tutorial`): the first screen the started tutorial
               stops at is this round-change page with its "Weiter" button. The hook advances it the way a
               click does (sv_PopRequest, the only thing the button's handler leaves behind), so a run without
               GUI automation gets past the page. The frame above has drawn and registered it once. */
            {
                char *rea = getenv("REA_START");
                if (rea != 0 && strcmp(rea, "tutorial") == 0)
                    U_I((unsigned)&sv_PopRequest) = 1;
            }
#endif
            if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c)
                U_I((unsigned)&sv_PopRequest) = 1;
            r = sv_RunFrame((int)main_Unk5384e0, 0x24087);
        }
        fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 1);
        while ((*fade & 4) == 0)
            sv_RunFrame(0, U_I((unsigned)&sv_FrameMask) | 0x80);
        sv_Unk52fa90();
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    }
    r = he_Unk53efb0(id);
    cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + id * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, -r);
    ai_Unk4bb720(3, U_I((unsigned)&ai_CharInts + id * 0x21c));
    cm_SendExSysMessage(1, 0);
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    U_I((unsigned)&ai_Unkfb9068) = U_I((unsigned)&sv_ViewRectRight);
    U_I((unsigned)&ai_Unkfb9060) = U_I((unsigned)&sv_ViewRectLeft);
    U_I((unsigned)&ai_Unkfb9064) = U_I((unsigned)&sv_ViewRectTop);
    U_I((unsigned)&ai_Unkfb906c) = U_I((unsigned)&sv_ViewRectBottom);
    unk_Unk513740();
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    if (U_I((unsigned)&full_data_624560) != 0) {
        if (fade != 0)
            d2_ReleaseFade(fade);
        d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x32, 10);
    }
    if (U_I((unsigned)&sv_PanelForm) != -1)
        d2_SetViewEnabled(U_I((unsigned)&sv_PanelForm), 1);
    if (*(char *)(unsigned)&ai_MissionMode != 5 && *(char *)(unsigned)&ai_MissionMode >= 0 && he_FindEvent(2, 0, 0x4c, 2, id) == 0) {
        MAIN_MSG(msg, 0x4c);
        MSG_I(msg, 0x10) = 0;
        MSG_I(msg, 0x14) = 0;
        MSG_I(msg, 0xc) = MSG_I(msg, 8);
        cm_SendLongMessage(msg);
    }
    if ((U_I((unsigned)&ai_GameFlags) & 8) != 0) {
        o = (unsigned char *)gm_BuildQuery(1, 0, 0x44);
        o = (unsigned char *)gm_Query(*(int *)(o + 0x5d), 1, 4, 0x1d);
        while (o != 0) {
            u2 = *(int *)(o + 0x3b);
            o = (unsigned char *)gm_BuildQuery(1, 1, *(int *)(o + 0x1c));
            if (o == 0 || ai_CanAct((unsigned)&ai_Chars + (unsigned)*(unsigned short *)(o + 0x27) * 0x21c) == 0)
                i = 0;
            else
                i = 1;
            ch_SetVisible(u2, i);
            o = (unsigned char *)gm_QueryNext();
        }
    }
    k = 0;
    do {
        c = (char *)((unsigned)&ai_Chars + k * 0x21c);
        if (*(short *)c != -1 && c[2] != 6 && c[2] != 7) {
            uu = gm_GetGoodsValue(c) * 0x4b;
            uv = 0;
            o = (unsigned char *)gm_BuildQuery(1, 4, k);
            while (o != 0) {
                char kd = *(char *)(U_I((unsigned)&ai_BTypes) + (unsigned)*o * 0x24d);
                if (gm_BuildingClass(*o) == 1 || gm_BuildingClass(*o) == 4 || gm_BuildingClass(*o) == 2 || kd == 0x13 || kd == 4 || kd == 0x10 || kd == 5 || kd == 9) {
                    kd = *(char *)(U_I((unsigned)&ai_BTypes) + (unsigned)*o * 0x24d + 0x247);
                    if (kd == 1)
                        uv += 2;
                    else if (kd == 2)
                        uv += 4;
                    else if (kd == 3)
                        uv += 8;
                }
                if (ai_IsLocalMaster(c) != 0 || c[2] == 1) {
                    unsigned short lim = (unsigned short)util_RandomInt(0x5a);
                    if (gm_BuildingLevel(o) < (int)lim) {
                        fa = (float)gm_EquipmentValue(o, 100) * 0.005;
                        if (fa < 32.0f)
                            fa = 32.0f;
                        fb = (float)uu;
                        if (fa < fb * 0.75) {
                            memset(msg, 0, 0xf8);
                            MSG_B(msg, 4) = 0x1c;
                            MSG_I(msg, 0xc) = -1;
                            MSG_B(msg, 0x36) = 1;
                            MSG_I(msg, 0x58) = *(int *)(o + 1);
                            MSG_I(msg, 0x5c) = 10;
                            MSG_I(msg, 8) = U_I((unsigned)&ai_CharInts + k * 0x21c);
                            cm_BeginGroup("renovieren");
                            w = (int)fa;
                            cm_SendMoveGoods(-1, U_I((unsigned)&ai_CharInts + k * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, w);
                            cm_SendLongMessage(msg);
                            cm_EndGroup("renovieren");
                            bld = gm_PlayerRecord(c);
                            if (bld != 0)
                                *(int *)(bld + 0x44) = (int)fa;
                            uu = (int)fb;
                        }
                    }
                }
                o = (unsigned char *)gm_BuildNext();
            }
            o = (unsigned char *)gm_BuildQuery(1, 4, k);
            while (o != 0) {
                char kd = *(char *)(U_I((unsigned)&ai_BTypes) + (unsigned)*o * 0x24d);
                if (gm_BuildingClass(*o) == 1 || gm_BuildingClass(*o) == 4 || gm_BuildingClass(*o) == 2 || kd == 0x13 || kd == 4 || kd == 0x10 || kd == 5 || kd == 9) {
                    unsigned v;
                    int p = (unsigned)*o * 0x24d;
                    kd = *(char *)(p + 0x247 + U_I((unsigned)&ai_BTypes));
                    if (kd == 1) {
                        v = (uu * 2) / uv;
                        *(unsigned *)(o + 0x39) = v;
                        v = v & (((int)v < 0) - 1);
                        *(unsigned *)(o + 0x39) = v;
                        if (*(char *)(p + U_I((unsigned)&ai_BTypes)) == 5) {
                            if (0x2edff < (int)v)
                                v = 0x2ee00;
                        } else if (63999 < (int)v)
                            v = 64000;
                        *(unsigned *)(o + 0x39) = v;
                    } else if (kd == 2) {
                        v = (uu * 4) / uv;
                        *(unsigned *)(o + 0x39) = v;
                        v = v & (((int)v < 0) - 1);
                        *(unsigned *)(o + 0x39) = v;
                        if (*(char *)(p + U_I((unsigned)&ai_BTypes)) == 5) {
                            if (319999 < (int)v)
                                v = 320000;
                        } else if (0x1f3ff < (int)v)
                            v = 0x1f400;
                        *(unsigned *)(o + 0x39) = v;
                    } else if (kd == 3) {
                        v = (uu * 8) / uv;
                        *(unsigned *)(o + 0x39) = v;
                        v = v & (((int)v < 0) - 1);
                        *(unsigned *)(o + 0x39) = v;
                        if (*(char *)(p + U_I((unsigned)&ai_BTypes)) == 5) {
                            if (639999 < (int)v)
                                v = 640000;
                        } else if (0x3e7ff < (int)v)
                            v = 0x3e800;
                        *(unsigned *)(o + 0x39) = v;
                    }
                }
                o = (unsigned char *)gm_BuildNext();
            }
        }
        k++;
    } while (k < 0x300);
    for (k = 0; k < 0x300; k++) {
        c = (char *)((unsigned)&ai_Chars + k * 0x21c);
        if (*(short *)c != -1) {
            U_I(((unsigned)&ai_Selected + 0x40) + k * 0x21c) &= 0xffffe7ff;
            if (ai_IsLocalJob(c) != 0)
                U_I(((unsigned)&ai_Selected + 0x30) + k * 0x21c) = *(int *)(U_I((unsigned)&ai_Unk770cec + k * 0x21c) + 0x39);
        }
    }
    for (k = 0; k < 0x300; k++) {
        float px = -1.0f, py = -1.0f;
        int pz;
        c = (char *)((unsigned)&ai_Chars + k * 0x21c);
        if (U_I(((unsigned)&ai_Unk770cec + 0x18) + k * 0x21c) != 0 && he_Unk55eba0(c, &px, &py, &pz) != 0) {
            int fg = U_I(((unsigned)&ai_Unk770cec + 0x18) + k * 0x21c);
            if (px != *(float *)(fg + 0x2c) || py != *(float *)(fg + 0x30)) {
                if (*(int *)(fg + 0x28) != -1) {
                    int h = (int)evt_FindById(*(int *)(fg + 0x28));
                    evt_Stop((EVTSCRIPT *)h);
                }
                ch_StopActions(U_I(((unsigned)&ai_Unk770cec + 0x18) + k * 0x21c));
                if (py != -1.0f && (w = ch_AddAction(U_I(((unsigned)&ai_Unk770cec + 0x18) + k * 0x21c), 0x33, pz, px, py)) != 0) {
                    *(int *)(w + 0x70) = U_I((unsigned)&full_data_6253b0);
                    *(int *)(w + 0x74) = U_I((unsigned)&full_data_6253b4);
                    *(int *)(w + 0x78) = U_I((unsigned)&full_data_6253b8);
                }
            }
        }
        if ((U_I((unsigned)&ai_GameFlags) & 8) != 0 && U_I(((unsigned)&ai_Unk770cec + 0x18) + k * 0x21c) != 0 && *(char *)(((unsigned)&ai_Chars + 0x2) + k * 0x21c) == 0xd && U_I(((unsigned)&ai_Unk770cec + 0x10) + k * 0x21c) == 0)
            cm_SendKillCharacter(U_I((unsigned)&ai_CharInts + k * 0x21c), 1);
    }
    U_I((unsigned)&ai_Unkf83650) = 1;
    amt_Unk4ea8e0((unsigned)&ai_Chars + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
    snd_FadeMusic(0x3f800000, 2000);
}
