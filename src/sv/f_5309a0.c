/* 0x5309a0, "MASTER": one frame of the game screen loop, called by every screen with its own callback (the top of the callback stack 0x75f780, 8 bytes per entry, depth 0x6f229c; 0 = nothing to register) and the
   mask of the parts to run (param_2: 1 input, 4 resize, 0x10 objects, 0x20 speech, 0x40 world, 0x100 left click, 0x1000 names, 0x2000 main buttons, 0x4000 weather, 0x10000 paused, 0x100000 modal, ..); returns 1
   while the caller's screen is the top screen, 0 when it must end. ROUGH (the float operands of the last mouse block are lost) */

int cm_PartyById();                     /* 0x4e50a0 */
void main_GameLoop(int ctl);                     /* 0x53bb20 */
void sv_UpdateClock(void);

int sv_RunFrame(int cb, unsigned mask)
{
    int r, top, c, i, it;
    char cv;
    unsigned u5;
    char name[0x100], tmp[0x104];
    int *p;

    SV_U((unsigned)&sv_FrameMask) = mask;
    sv_PumpMessages();
    di_GetMouse();
    if (cb != 0) {
        if (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) != cb)
            di_ClearButtons();
        if (ai_PlayerId != 0xffff) {
            i = ai_PlayerId * 0x21c;
            if ((SV_B(((unsigned)&ai_Chars + 0x2) + i) == 6 || SV_B(((unsigned)&ai_Chars + 0x2) + i) == 7) && SV_B(((unsigned)&ai_CharInts + 0x4) + i) == 0 && SV_I((unsigned)&sv_FrameBase) < SV_I((unsigned)&sv_FrameDepth)
                && (it = cm_PartyById(SV_I((unsigned)&ai_Unk770d8c + ai_PlayerId * 0x21c))) != 0 && *(char *)(it + 8) == 8 && (mask & 0x10000) == 0) {
                SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase);
                sv_SetStatusText(SV_C((unsigned)&full_data_143a3e4));
            }
        }
    }
    if (SV_I((unsigned)&main_Inactive) != 0)
        return 1;
    cv = (char)mask;
    if ((mask & 4) != 0 && (d2_MouseUpdate(), SV_I((unsigned)&main_Inactive) != 0))
        SV_I((unsigned)&ai_FrameWait) = 1;
    if (SV_I((unsigned)&sv_PopRequest) == 0 && (mask & 0x100) != 0 && (mask & 0x100000) == 0)
        sv_Unk52d940(mask);
    if (cb == 0) {
        r = 1;
    } else {
        if (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == cb) {
            r = 1;
        } else {
            if (SV_I((unsigned)&sv_PopRequest) == 0) {
                p = (int *)((unsigned)&ai_Unk75f280 + 0x500);
                r = 1;
                do {
                    if (*p == cb) {
                        err_Message("MASTER(): Function already registrated!");
                        goto l21;
                    }
                    p += 2;
                } while ((int)p < (unsigned)((unsigned)ai_Unk75f280 + 0x900));
                SV_I((unsigned)&sv_FrameDepth)++;
                SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) = cb;
                goto l64;
            }
l21:
            r = 0;
        }
        if (SV_I((unsigned)&sv_PopRequest) != 0 && r != 0) {
            SV_I((unsigned)&sv_PopRequest)--;
            if (SV_I((unsigned)&sv_FrameDepth) != 0) {
                SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) = 0;
                SV_I((unsigned)&sv_FrameDepth)--;
            }
            r = 0;
        }
    }
l64:
    if ((mask & 1) != 0 && SV_I((unsigned)&ai_GameOver) == 0) {
        if ((mask & 0x100) != 0)
            he_Unk53daf0();
        he_Unk53edf0();
        if (SV_I((unsigned)&ai_Unk1460ae0) == -1 && SV_I((unsigned)&ai_Unka687e4) < SV_I((unsigned)&ai_Unk6333cc) && SV_B((unsigned)&ai_GameState) != 0)
            ai_Unk4c4d40((unsigned)&ai_Unk75e130);
        if ((mask & 0x10000) == 0 && SV_B((unsigned)&ai_GameState) != 0 && SV_I((unsigned)&sv_PanelFlag) == 0 && ai_Unk4e4cc0() != 0 && SV_B((unsigned)&full_data_6f22ac) != 0)
            SV_I((unsigned)&sv_PopRequest) = 1;
    }
    if ((mask & 0x20000) != 0 && SV_I((unsigned)&ai_Unka687e4) < SV_I((unsigned)&ai_Unk6333cc)) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        SV_I((unsigned)&ai_Unka687e4) = SV_I((unsigned)&ai_Unk6333cc) + 7;
    }
    if (r == 0 || SV_I((unsigned)&sv_PanelFlag) != 0)
        SV_I((unsigned)&ai_FrameWait)++;
    if ((mask & 0x200) != 0)
        evt_StepAll();
    if (SV_I((unsigned)&ai_FrameWait) == 0 && (cv >= 0 || (d2_FlushDirty(), SV_I((unsigned)&ai_FrameWait) == 0)) && (mask & 0x40) != 0 && SV_I((unsigned)&full_data_623eb0) != 0) {
        if ((mask & 0x10000) == 0 && (mask & 0x40000) != 0)
            sv_Unk525ad0((unsigned)&ai_Time);
        if (SV_I((unsigned)&d3_CurrentScene) == 0)
            d3q_Mark(SV_I((unsigned)&d3_QuadTree), SV_I((unsigned)&ai_Unkfc2dec), SV_I((unsigned)&d3_FrameCount));
        d3t_UpdateCamera();
        d3if_ClearView();
    }
    c = SV_I((unsigned)&ai_PlayerBuilding);
    if (c == 0)
        c = SV_I((unsigned)&ai_FallbackBuilding);
    u5 = mask & 0x8000;
    if (u5 == 0 && SV_B((unsigned)&ai_GameState) != 0)
        sv_CollectGroupCharacters((char *)c);
    if (c == 0 || (mask & 0x10000) != 0) {
        SV_I((unsigned)&ch_CurrentGroup) = -1;
    } else {
        sv_Unk52b500((unsigned char *)c);
        sv_Unk524990(c);
        SV_I((unsigned)&ch_CurrentGroup) = *(int *)(c + 1);
    }
    if (SV_B((unsigned)&ai_GameState) != 0) {
        ai_UpdateWind();
        unk_Unk4edf80();
    }
    if (SV_I((unsigned)&full_data_623eac) != 0) {
        ch_UpdatePositions();
        ch_Update();
        ch_Unk4027b0();
    }
    if ((mask & 0x4000) != 0 && SV_US((unsigned)&ai_TimeHour) != SV_I((unsigned)&ai_Unk760614))
        sv_Unk5301a0();
    if (SV_I((unsigned)&full_data_6f2580) != 0)
        snd_UpdateRain();
    if (SV_I((unsigned)&snd_SfxEnabled) != 0) {
        ai_Unk4c0e60();
        snd_UpdateQueue();
    }
    if (SV_I((unsigned)&snd_MusicEnabled) != 0 && (mask & 0x40000) != 0)
        ai_Unk4c2000();
    if (SV_I((unsigned)&ai_FrameWait) == 0
        && ((SV_I((unsigned)&ai_Unk633738) == 0 || (mask & 0x4000) == 0 || (mask & 0x40) == 0 || (sv_Unk530670(), SV_I((unsigned)&ai_FrameWait) == 0))
            && ((mask & 0x10000) == 0 || SV_I((unsigned)&ai_Unk6f0dbc) == 0 || (d3t_DrawRain(SV_I((unsigned)&ai_Unk6f0dbc)), SV_I((unsigned)&ai_FrameWait) == 0)) && SV_I((unsigned)&ai_Unk760440) != 0)) {
        if (u5 == 0)
            sv_Unk52eee0();
        else
            sv_Unk52f0d0();
        sv_Unk52f2c0();
    }
    if (SV_I((unsigned)&snd_SfxEnabled) != 0) {
        d3snd_Update();
        if (SV_I((unsigned)&full_data_6f2580) != 0)
            d3sndw_Update();
        sb_Update();
    }
    if (SV_I((unsigned)&sv_PopRequest) == 0 && (mask & 0x100000) == 0) {
        if (u5 == 0)
            sv_Unk52a680();
        if ((mask & 0x10) == 0) {
            if (SV_I(((unsigned)&dm_L + 0x28)) != 0)
                goto l2;
            if (ai_Unk5fe710 == -1 || (SV_B(SV_I((unsigned)&d2_Objs) + 0x1c0 + ai_Unk5fe710 * 0x2ec) & 0x40) == 0)
                d2_CopyCaption(ch_EmptyName);
            SV_I((unsigned)&ai_Unk6f22e8) = 0;
            SV_I((unsigned)&full_data_6f22f0) = 0;
            if (u5 == 0)
                d2_SetPointerSprite(0, 0);
        }
        if (SV_I(((unsigned)&dm_L + 0x28)) == 0) {
            if ((mask & 0x10) != 0)
                sv_Unk52ae70();
            if ((mask & 0x20) != 0) {
                sv_Unk52bec0();
                sv_Unk52c5c0();
            }
        }
    }
l2:
    if ((mask & 0x400) != 0)
        ai_Unk587e00();
    if (SV_I((unsigned)&ai_FrameWait) == 0 && cv < 0 && d2_LockSurface(d2_ScreenSurf) != 0) {
        if (sv_PanelPic != -1 && (mask & 0x100) != 0 && SV_I((unsigned)&full_data_623eb4) != 0)
            sv_Unk522f10(2, mask);
        if (SV_I((unsigned)&full_data_62457c) != 0)
            d2_DrawObjects();
        d2_UnlockSurface(d2_ScreenSurf);
        d2_DrawFades();
    } else if (SV_I((unsigned)&sv_PanelFlag) != 0)
        d2_FinishFades();
    di_PollMouse();
    if (SV_I((unsigned)&di_Absolute) != 0) {
        SV_S((unsigned)&d2_MouseX) = SV_S((unsigned)&dm_P);
        SV_S((unsigned)&d2_MouseY) = SV_S(((unsigned)&dm_P + 0x2));
    }
    sv_SetScreenCenter(mask);
    if (SV_I((unsigned)&ai_FrameWait) == 0 && cv < 0) {
        if (d2_LockSurface(d2_ScreenSurf) != 0) {
            if (SV_I((unsigned)&ai_Unk6f0da4) != 0)
                ai_Unk4d8b50();
            if ((mask & 0x1000) != 0 && SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase) && SV_I((unsigned)&full_data_6f262c) == 0) {
                sv_Unk52cef0();
                if (SV_B(((unsigned)&kb_Down + 0x1d)) == 0) {
                    if (SV_B(((unsigned)&kb_Down + 0x38)) != 0)
                        sv_Unk52d2a0();
                } else
                    sv_Unk52d1e0();
            }
            if ((mask & 0x80000) == 0) {
                if (SV_I((unsigned)&sv_PanelShown) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
                    sv_Unk52d480();
            } else
                ai_Unk4cf370();
            if ((mask & 0x1000) != 0 && SV_I(((unsigned)&dm_L + 0x28)) == 0 && SV_I((unsigned)&ai_Unk6f22b4) == 0 && SV_I((unsigned)&d3t_CamMove) == 0 && SV_I((unsigned)&ai_Unk760440) == 0 && SV_I((unsigned)&d2_CutsceneActive) == 0 && SV_I((unsigned)&full_data_6f262c) == 0)
                sv_Unk52e050();
            sv_Unk52e0c0();
            sv_DrawPanelPointer();
            d2_UnlockSurface(d2_ScreenSurf);
        }
        dd_Flip();
    }
    if (SV_B((unsigned)&kb_Last) == 1 && (mask & 0x2000) != 0) {
        msx_SetVolume((int)0.0f);                /* the float operands of this block are lost in the pseudocode */
        sb_SetMasterVolume((int)0.0f);
        ai_OptionsMenu();
        msx_SetVolume((int)0.0f);
        sb_SetMasterVolume((int)0.0f);
    }
    sv_Unk530100();
    if (SV_B((unsigned)&ai_GameState) != 0) {
        if ((SV_U((unsigned)&ai_GameFlags) & 4) != 0 && SV_I((unsigned)&d2_FocusedInput) == 0 && SV_B((unsigned)&kb_Last) == 0x1c && (mask & 0x2000) != 0)
            sv_Unk52fe10();
        if (SV_B((unsigned)&ai_GameState) != 0) {
            if ((SV_B((unsigned)&kb_Last) == 0xcf || (SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d2_HitUnderPointer) == ai_Unk623850)) && (mask & 0x10000) == 0 && SV_B((unsigned)&full_data_6f22ac) == 0 && (SV_U((unsigned)&ai_GameFlags) & 4) == 0) {
                if (SV_I((unsigned)&ai_Unk6f2448) == 0)
                    sv_Unk5307f0();
                else {
                    SV_I((unsigned)&full_data_6f25a8) = 1;
                    sv_PopTo((int)main_GameLoop);
                }
            }
            if (SV_B((unsigned)&ai_GameState) != 0) {
                if (SV_B((unsigned)&sv_MenuOpen) == 0 && (SV_U((unsigned)&ai_GameFlags) & 4) == 0 && u5 == 0 && SV_B((unsigned)&kb_Last) == '9')
                    ai_PauseGame();
                if (SV_B((unsigned)&ai_GameState) != 0 && (mask & 0x300000) == 0)
                    ai_Unk589cf0();
            }
        }
    }
    if (sv_PanelPic != -1 && (mask & 0x2000) != 0 && (mask & 0x100000) == 0 && SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase)) {
        if (SV_B((unsigned)&ai_GameState) != 0)
            ai_Unk589c90();
        if (ai_PlayerId == 0xffff || SV_B(((unsigned)&ai_CharInts + 0x4) + ai_PlayerId * 0x21c) == 0) {
            if (SV_I((unsigned)&d2_ClickedId) != -1
                && (ai_Unk5fe710 == ai_Unk623848 || ai_Unk5fe710 == ai_Unk623824 || ai_Unk5fe710 == ai_Unk62382c || ai_Unk5fe710 == ai_Unk623830 || ai_Unk5fe710 == ai_Unk623828
                    || ai_Unk5fe710 == ai_Unk623820))
                sv_SetStatusText(SV_C((unsigned)&full_data_143a3e4));
        } else if (SV_I((unsigned)&d2_ClickedId) != -1) {
            if (ai_Unk5fe710 == ai_Unk623848)
                ai_Unk5c8640();
            else if (ai_Unk5fe710 == ai_Unk623824)
                ai_Unk5c82f0();
            else if (ai_Unk5fe710 == ai_Unk62382c)
                ai_Unk5bac10();
            else if (ai_Unk5fe710 == ai_Unk623830)
                ai_Unk5bcc70();
            else if (ai_Unk5fe710 == ai_Unk623828)
                ai_Unk5c7f00();
            else if (ai_Unk5fe710 == ai_Unk623834)
                ai_MessageFilterScreen();
            else if (ai_Unk5fe710 == ai_Unk623820)
                ai_Unk5c4150(SV_I((unsigned)&ai_CharInts + ai_PlayerId * 0x21c), 0x11, 0, 0x657c7);
        }
    }
    sv_TickFrames++;
    if ((mask & 0x10000) == 0) {
        if (SV_I((unsigned)&sv_PopRequest) == 0 && SV_I((unsigned)&sv_PanelFlag) == 0 && SV_I((unsigned)&ai_Unkf31144) != 0 && SV_I((unsigned)&full_data_6f25a8) == 0) {
l3f5:
            if ((char)SV_B((unsigned)&ai_GameFlags) >= 0) {
                SV_I((unsigned)&full_data_6f25a8) = 1;
                tb_SetFunctionOff(sv_UpdateClock, 1);
                SV_U((unsigned)&sv_FrameMask) |= 0x100000;
                sv_MessageBoxYesNo(SV_I((unsigned)&full_data_143a3c4), 0xc4);
                sv_PopTo((int)main_GameLoop);
            }
            goto l444;
        }
        if (SV_I((unsigned)&ai_Unk1460ae0) != -1 || SV_US((unsigned)&ai_TimeHour) < 0x17)
            goto l444;
        if (SV_I((unsigned)&full_data_6f25a8) == 0)
            goto l3f5;
    } else {
l444:
        if (SV_I((unsigned)&full_data_6f25a8) == 0)
            goto l473;
    }
    if (SV_I((unsigned)&sv_PopRequest) == 0 && SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)ai_GameFrame)
        SV_I((unsigned)&sv_PopRequest) = 1;
l473:
    if ((mask & 0x10000) == 0 && SV_B((unsigned)&full_data_6f23f1) == 0 && SV_I((unsigned)&ai_MissionPartner) != -1 && SV_B((unsigned)&ai_GameState) != 0) {
        SV_B((unsigned)&full_data_6f23f1) = 1;
        tb_SetFunctionOff(sv_UpdateClock, 1);
        ai_MissionCompleteSequence(0x65fff);
        tb_SetFunctionOff(sv_UpdateClock, 0);
        SV_B((unsigned)&full_data_6f23f1) = 0;
    }
    if (SV_B((unsigned)&cm_RequestedSaveName) != 0 && (SV_U((unsigned)&sv_FrameMask) & 0x200000) == 0) {
        if (SV_I((unsigned)&cm_RequestedSaveSlot) == 1)
            strcpy(name, "QUICKSAVE");
        else if (SV_I((unsigned)&cm_RequestedSaveSlot) == 0)
            strcpy(name, "AUTOSAVE");
        else
            sprintf(name, "GILDE_SAVEGAME_%i", SV_I((unsigned)&cm_RequestedSaveSlot));
        strcpy(tmp, (char *)(unsigned)&cm_RequestedSaveName);
        ai_SaveNetworkGame(name, tmp);
    }
    if (SV_I((unsigned)&ai_Unk6f0cd4) != 0 && (SV_U((unsigned)&sv_FrameMask) & 0x200000) == 0) {
        SV_I((unsigned)&ai_Unk6f0cd4)--;
        sv_BroadcastText(mask, SV_C((unsigned)&ai_Unk1441488));
    }
    if (0 < SV_I((unsigned)&ai_FrameWait))
        SV_I((unsigned)&ai_FrameWait)--;
    return r;
}
