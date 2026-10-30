/* 0x591520, the main game frame callback (1445 B, ROUGH SKELETON from the Ghidra pseudocode): the clock index
   0x4fe350 (0x770ae0), the globals saved / restored around the loop (0x5f3238 / 0x5f323c / 0x6310b0 /
   0x6310b4), the network branch (0x6f23f8 & 4: 0x5bc110); the character query (0x6f23f8 & 1) + 0x527bb0; the
   per-frame work: 0x4c30e0 / 0x42ea10, the pump 0x5309a0 (this, 0x67fff); 0x591ad0 for the flagged characters,
   0x527640, the day-phase switch on the clock 0x770ae0 (mode 0 / 1 / 2, the character +0x95 handles 0x4284a0 /
   0x429e90 / 0x528490), 0x591480, 0x527840, the construction window 0x5909a0, the contact checks (0x502520 /
   0x576340 / 0x5024f0 / 0x59e420 / 0x59ee40) and the selection 0x75e8c0 / 0x75e8c4 (0x527840, 0x527bb0);
   the exit restores the globals and returns */

void ai_GameFrame(void)
{
    int n;
    int mode;
    int it;
    int r;
    int v;
    unsigned int u;
    char *p;
    int s0;
    int s1;
    int s2;
    int s3;

    n = gm_ClockSeason((void *)(unsigned)&ai_Time);
    U_I((unsigned)&full_data_5f3418) = -1;
    unk_Unk513740();
    if (U_I((unsigned)&full_data_6f1c2c) != 0) {
        d3fl_MakeDivide(U_I((unsigned)&ai_Unk633738));
        U_I((unsigned)&full_data_6f1c2c) = 0;
    }
    s0 = U_I((unsigned)&di_MaxX);
    s1 = U_I((unsigned)&di_MaxY);
    s2 = U_I((unsigned)&di_MinX);
    s3 = U_I((unsigned)&di_MinY);
    *(char *)(unsigned)&snd_AmbientOn = 1;
    U_I((unsigned)&ai_GameState) = 1;
    if (U_I((unsigned)&ai_GameFlags) & 4) {
        for (p = (char *)((unsigned)&ai_Chars + 0x2); p < (char *)(unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            if (*(unsigned short *)(p - 2) != 0xffff &&
                *(int *)(p + 0x52) == U_I(((unsigned)&ai_Unk770b89 + 0x4b) + U_W((unsigned)&ai_PlayerId) * 0x87) &&
                *(unsigned short *)(p - 2) != U_W((unsigned)&ai_PlayerId) &&
                (*p == 6 || *p == 7 || *p == 5) &&
                U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87) < *(unsigned int *)(p + 2)) {
                ai_Unk5bc110();
                break;
            }
        }
    }
    if ((U_I((unsigned)&ai_GameFlags) & 1) && U_I((unsigned)&full_data_6f2598) == 1) {
        for (it = gm_BuildQuery(1, 4, U_W((unsigned)&ai_PlayerId)); it != 0; it = gm_BuildNext()) {
            if (it != U_I(((unsigned)&ai_Unk770cec + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x87)) {
                sv_Unk527bb0(it);
                break;
            }
        }
    }
    snd_StartMarketAmbience();
    mode = 0;
    tb_SetFunctionOff((void *)(unsigned)&sv_UpdateClock, 0);
    r = sv_RunFrame(ai_GameFrame, 0x67fff);
    while (r != 0) {
        U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth);
        if (*(char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
            ai_Unk591ad0();
        sv_Unk527640();
        if (mode == 0) {
            if ((float)U_W((unsigned)&ai_TimeHour) < *(float *)((unsigned)&ai_Unk617830 + n * 4) ||
                *(float *)((unsigned)&ai_Unk617840 + n * 4) <= (float)U_W((unsigned)&ai_TimeHour)) {
                for (it = gm_BuildQuery(1, 6); it != 0; it = gm_BuildNext()) {
                    if (*(int *)(it + 0x95) != -1) {
                        v = (int)evt_FindById(*(int *)(it + 0x95));
                        if (v != 0)
                            evt_Stop((EVTSCRIPT *)v);
                        *(int *)(it + 0x95) = -1;
                    }
                }
                mode = 1;
            }
        } else if (mode == 1) {
            if (*(float *)((unsigned)&ai_Unk617830 + n * 4) <= (float)U_W((unsigned)&ai_TimeHour) &&
                (float)U_W((unsigned)&ai_TimeHour) < *(float *)((unsigned)&ai_Unk617840 + n * 4)) {
                for (it = gm_BuildQuery(1, 6); it != 0; it = gm_BuildNext())
                    if (*(int *)(it + 0x95) == -1)
                        sv_Unk528490(it);
                mode = 2;
            }
        } else if (mode == 2 &&
                   ((float)U_W((unsigned)&ai_TimeHour) < *(float *)((unsigned)&ai_Unk617830 + n * 4) ||
                    *(float *)((unsigned)&ai_Unk617840 + n * 4) <= (float)U_W((unsigned)&ai_TimeHour))) {
            for (it = gm_BuildQuery(1, 6); it != 0; it = gm_BuildNext()) {
                if (*(int *)(it + 0x95) != -1) {
                    v = (int)evt_FindById(*(int *)(it + 0x95));
                    if (v != 0)
                        evt_Stop((EVTSCRIPT *)v);
                    *(int *)(it + 0x95) = -1;
                }
            }
            mode = 3;
        }
        ai_Unk591480();
        if (U_I(((unsigned)&dm_L + 0x14)) != 0 && U_I(((unsigned)&dm_L + 0x24)) == 0 && U_I((unsigned)&full_data_6f22f4) != 0 &&
            U_I((unsigned)&d3t_CamMove) == 0)
            sv_Unk527840(U_I((unsigned)&full_data_6f22f4));
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&ai_Unk623884))
            ai_Unk5909a0();
        if (U_I((unsigned)&ai_Unk75ec60) != 0 &&
            (u = unk_Unk503530(U_W((unsigned)&ai_PlayerId), U_I((unsigned)&ai_Unk75ec60), 0, 0), U_I((unsigned)&ai_Unk75ec60) != 0) &&
            U_I((unsigned)&full_data_6f22f4) == U_I((unsigned)&ai_Unk75ec60) &&
            ((U_I(((unsigned)&dm_L + 0x1c)) != 0 || (U_I(((unsigned)&dm_L + 0xc)) != 0 && U_I((unsigned)&d2_PointerLook) == 0xb))) &&
            (u & 1) != 0 && (*(unsigned char *)(U_I((unsigned)&ai_Unk75ec60) + 0x5a) & 1) == 0) {
            v = gm_IsHomeType(U_I((unsigned)&ai_Unk75ec60));
            if (v == 0 || (v = ai_Unk576340(0x1b, U_I((unsigned)&ai_Unk75ec60), 0, 0), v != 1)) {
                v = gm_IsType10(U_I((unsigned)&ai_Unk75ec60));
                if (v == 0 &&
                    (v = ai_Unk576340(0x19, U_I((unsigned)&ai_Unk75ec60), 0,
                                      *(unsigned short *)(U_I((unsigned)&ai_Unk75ec60) + 0x29)), v == 1))
                    ai_Unk59e420(U_I((unsigned)&ai_Unk75ec60), *(unsigned short *)(U_I((unsigned)&ai_Unk75ec60) + 0x29));
            } else {
                ai_Unk59ee40(U_I((unsigned)&ai_Unk75ec60));
            }
        }
        if (U_I((unsigned)&ai_MarketBuilding) != 0)
            ai_Unk59a800(U_I((unsigned)&ai_MarketBuilding));
        /* 0x591986 .. 0x591a14: the object the info panel selected resolves here, before the frame is pumped:
           without a scene record the camera is focused on it (0x527840) and the panel flag dropped; with one the
           object takes the record word (+0x29) and then, by its +0x5a bit 0 / a home type / the type 0x10, the
           building is entered (0x59ee40 / 0x59e420) or the panel is only redrawn (0x527bb0 / 0x527840); the
           record-less way and the +0x5a bit 0 clear sv_PanelFlag */
        if (U_I((unsigned)&sv_PanelFlag) != 0) {
            if (U_I((unsigned)&sv_PanelRecord) == 0) {
                sv_Unk527840(U_I((unsigned)&sv_PanelObject));
                U_I((unsigned)&sv_PanelFlag) = 0;
            } else {
                p = (char *)U_I((unsigned)&sv_PanelObject);
                *(unsigned short *)(p + 0x29) = *(unsigned short *)U_I((unsigned)&sv_PanelRecord);
                if ((*(unsigned char *)(p + 0x5a) & 1) != 0) {
                    U_I((unsigned)&sv_PanelFlag) = 0;
                } else {
                    if (*(int *)(p + 0x61) != 0 && gm_IsType10((unsigned char *)p) == 0)
                        sv_Unk527bb0(p);
                    if (gm_IsHomeType(p) != 0)
                        ai_Unk59ee40((unsigned char *)p);
                    else if (gm_IsType10((unsigned char *)p) == 0)
                        ai_Unk59e420((unsigned char *)p, *(unsigned short *)(p + 0x29));
                    else {
                        sv_Unk527840(p);
                        U_I((unsigned)&sv_PanelFlag) = 0;
                    }
                }
            }
        }
        r = sv_RunFrame(ai_GameFrame, 0x67fff);
    }
    *(char *)(unsigned)&snd_AmbientOn = 0;
    U_I((unsigned)&ai_GameState) = 0;
    snd_StopMarketAmbience();
    d2_CopyCaption((void *)(unsigned)&ch_EmptyName);
    sv_SetStatusText((void *)(unsigned)&ch_EmptyName);
    sv_Unk52adb0();
    U_I((unsigned)&d2_PointerLook) = 0;
    U_I((unsigned)&d2_CutsceneActive) = 0;
    U_I((unsigned)&di_Absolute) = 1;
    di_SetMousePos(U_I((unsigned)&d2_MouseX), U_I((unsigned)&d2_MouseY));
    U_I((unsigned)&di_MaxX) = s0;
    U_I((unsigned)&di_MaxY) = s1;
    U_I((unsigned)&di_MinX) = s2;
    U_I((unsigned)&di_MinY) = s3;
    sv_RunFrame(0, 0x67fff);
}
