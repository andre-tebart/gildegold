/* 0x59e420, the enter-building driver (2592 B, ROUGH: the entry / exit bookkeeping is condensed): when
   0x59e210 refuses, 0x75e8c0 = 0 and return; 0x520f20, 0x5309a0 (0x20007), 0x6237c8 = 0, 0x52adb0 /
   0x52ae40; the sound "EigenesGebaeudeBetreten_s" / "FremdesGeschaeftBetreten_s" (0x426fa0); "normal_s" copied
   to 0x5f1230; the building 0x4ffe90 (param_1 + 0x5d, 1, 0, param_1 + 0x29) and 0x4c28a0 (param_1); the pump
   0x5309a0 (0x59e420, 0x67fff) and per frame the room 0x4ffe90: the room callback 0x76faa0 (the index word),
   0x50d7f0 (the room), 0x4478e0 (the callback), 0x43deb0 (3), 0x438e60 (0x5fb5b0, 0, 0xe0, 0x45ce00),
   0x512f30 / 0x513050 / 0x513400 (param_1, the building), the matrices 0x43b790 / 0x43b860 (0xfc2dec + 0x5c /
   + 0x90), the first-room camera checks 0x453e80 (+0x114 / +0x120), the saved view 0x75fb80 ..; then
   0x576040 (0x19, the room, 0, 0), the room handlers switch on the word (0x120..0x29e: the contact listeners
   and windows of this unit and the following ones), 0x576040 (0x1a, the room, 0, 0), 0x6f2314 = the room,
   0x4284a0 / 0x429e90 / 0x52bb70, 0x5136b0 (param_1), 0x6f22bc = 1; when 0x6f2308 is null 0x6f22a0 = 1 and
   0x43db30 (3); on exit 0x52adb0 / 0x52ae40, 0x7662c0 = 0, 0x4c2c00, 0x4478e0, the globals cleared, 0x513740,
   the saved view restored and "GebaeudeVerlassen_s" */

void ai_Unk59e420(unsigned char *param_1, int param_2)
{
    int i;
    unsigned short *room;
    int cb;
    int kind;
    int v;
    unsigned short vid;
    int saved1, saved2, saved3, saved4, saved5, saved6;

    if (ai_Unk59e210(param_1) == 0) {
        U_I((unsigned)&sv_PanelFlag) = 0;
        return;
    }
    sv_ResetScreenClip();
    sv_RunFrame(ai_Unk59e420, 0x20007);
    U_I((unsigned)&full_data_6237c8) = 0;
    sv_Unk52adb0();
    sv_ClearSelection();
    kind = U_I((unsigned)&d3_CurrentScene);
    U_I((unsigned)&full_data_6f2314) = 0;
    *(short *)(param_1 + 0x29) = (short)param_2;
    saved1 = U_I((unsigned)&ai_Unk75fb80);
    saved2 = U_I((unsigned)&ai_Unk75fb84);
    saved3 = U_I((unsigned)&ai_Unk75fb88);
    saved4 = U_I((unsigned)&ai_Unk75fb90);
    saved5 = U_I((unsigned)&ai_Unk75fb94);
    saved6 = U_I((unsigned)&ai_Unk75fb98);
    if (param_2 == 0)
        U_I((unsigned)&ai_PlayerBuilding) = 0;
    if (*(unsigned short *)(param_1 + 0x27) == U_W((unsigned)&ai_PlayerId))
        sb_PlaySample(U_I((unsigned)&full_data_6f2410), "EigenesGebaeudeBetreten_s", 1, 0x3f, 0x7f, 0, 0);
    else
        sb_PlaySample(U_I((unsigned)&full_data_6f2410), "FremdesGeschaeftBetreten_s", 1, 0x3f, 0x7f, 0, 0);
    strcpy((char *)(unsigned)&ch_StepSample, "normal_s");
    gm_Query(U_I(param_1 + 0x5d), 1, 0, *(unsigned short *)(param_1 + 0x29));
    snd_LoadBuildingVoices(param_1);
    for (;;) {
        U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth) + 1;
        room = (unsigned short *)gm_Query(U_I(param_1 + 0x5d), 1, 0, *(unsigned short *)(param_1 + 0x29));
        if (room != 0) {
            cb = *(signed char *)((unsigned)&init_ObjectUniverse + *room);
            if (cb != -2) {
                if (cb == -1 || *(unsigned char *)((unsigned)&init_SceneDirty + cb * 0x3e0) != 0) {
                    U_I((unsigned)&d2_CutsceneActive) = 1;
                    sv_RunFrame(0, 0x603c7);
                    sv_RunFrame(0, 0x603c7);
                    init_LoadObjectScene(*room);
                    U_I((unsigned)&d2_CutsceneActive) = 0;
                }
                if (U_I((unsigned)&d3_CurrentScene) != 0)
                    d3_DeflateScene(3);
                U_I((unsigned)&ai_PlayerBuilding) = param_1;
                U_I((unsigned)&evt_NoGroup) = U_I(room + 1);
                U_I((unsigned)&ch_CurrentGroup) = U_I(param_1 + 1);
                U_I((unsigned)&ch_CurrentScriptGroup) = U_I(room + 1);
                U_I((unsigned)&sv_PanelShown) = room;
                *(unsigned short *)(param_1 + 0x29) = *room;
                d3if_SelectScene(*(signed char *)((unsigned)&init_ObjectUniverse + *room), 0);
                d3_InflateScene(3);
                d3_ForEachObject2((void *)U_I((unsigned)&ai_Unk5fb5b0), 0, 0xe0, (void *)(unsigned)&d3s_ResetShadows);
                init_SetCharacterAnimations(param_1);
                init_ShowBuildingScene(U_I((unsigned)&ai_PlayerBuilding), *(unsigned short *)(U_I((unsigned)&ai_PlayerBuilding) + 0x29));
                init_FinishBuildingScene(U_I((unsigned)&ai_PlayerBuilding), U_I((unsigned)&sv_PanelShown));
                d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x5c));
                d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x90));
                /* ROUGH: the device callback (0x5fb5b0 + 0x3c8 -> +0x210) */
                if (*(unsigned short *)(param_1 + 0x27) != U_W((unsigned)&ai_PlayerId)) {
                    v = gm_BuildingClass(*param_1);
                    if ((v == 1 || v == 8) &&
                        d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0 &&
                        d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0) {
                        d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114));
                        d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120));
                    }
                }
                U_I((unsigned)&ai_Unk75fb80) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x4c);
                U_I((unsigned)&ai_Unk75fb84) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x50);
                U_I((unsigned)&ai_Unk75fb88) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x54);
                U_I((unsigned)&ai_Unk75fb90) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x84);
                U_I((unsigned)&ai_Unk75fb94) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x88);
                U_I((unsigned)&ai_Unk75fb98) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x8c);
                U_I((unsigned)&ai_Unk6f22cc) = 0;
                U_I((unsigned)&ai_Unk6f22d0) = 0;
            }
            ai_Unk576040(0x19, room, 0, 0);
            vid = *room;
            switch (vid) {
            case 0x13: ai_Unk597500(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x14: ai_Unk596400(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x15: ai_Unk5965f0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x16: ai_WineCellarContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x17: ai_Unk5966f0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x18: ai_MistressContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x1f: ai_Unk59a560(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x4d: ai_ThievesGuildEmptyListener(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x54: ai_Unk5a4d40(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x5c: ai_Unk5a4f70(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x60: ai_Unk5a5080(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x65: ai_Unk5a5bc0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x74: ai_Unk595170(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x85: ai_Unk595330(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x9b: ai_Unk5962b0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xa5: ai_Unk59df80(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xb7: ai_DungeonContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xba: ai_Unk5a3df0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xbb: ai_DungeonEmptyListener(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xcf: ai_Unk596120(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xd6: ai_IdleContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xdd: ai_AquariumContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xe5:
            case 0xe6: ai_Unk5a3810(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xf0: ai_ChurchContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xf2: ai_Unk5a2de0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0xf7: ai_Unk5a36c0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x10e: ai_Unk59fd30(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x110: ai_Unk5a07d0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x114: ai_Unk59fca0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x118: ai_PerfumeryContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x120: ai_Unk599af0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x12e: ai_Unk599cb0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x137: ai_Unk5a18d0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x138: ai_Unk5a1a60(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x139: ai_Unk5a1960(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x145: ai_Unk5a6a90(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x146: ai_Unk5a67a0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x14c: ai_Unk5a6920(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x24b: ai_Unk5821d0(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x256: ai_Unk582560(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x266: ai_Unk582600(U_I((unsigned)&ai_PlayerBuilding), room); break;
            case 0x29e: ai_ProductionContact(U_I((unsigned)&ai_PlayerBuilding), room); break;
            default: ai_PumpFrames(room); break;
            }
            ai_Unk576040(0x1a, room, 0, 0);
            U_I((unsigned)&full_data_6f2314) = (int)room;
            /* ROUGH: 0x4284a0 / 0x429e90 / 0x52bb70 of the device record and 0x5fb5b0 callback +0x294 */
            if (U_I((unsigned)&ai_Unk5f8438) != -1)
                sv_Unk52bb70(U_I((unsigned)&ai_Unk5f8438));
            unk_Unk5136b0K(param_1);
            U_I((unsigned)&ai_FrameWait) = 1;
            if (U_I((unsigned)&ai_PlayerBuilding) == 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                d3_DeflateScene(3);
            }
        }
        if (sv_RunFrame(ai_Unk59e420, 0x67fff) == 0) {
            sv_Unk52adb0();
            sv_ClearSelection();
            U_I((unsigned)&ai_Unk7662c0) = 0;
            snd_UnloadBuildingVoices();
            d3if_SelectScene(kind, 0);
            U_I((unsigned)&evt_NoGroup) = 0;
            U_I((unsigned)&ch_CurrentGroup) = 0xffffffff;
            U_I((unsigned)&ch_CurrentScriptGroup) = 0xffffffff;
            U_I((unsigned)&ai_PlayerBuilding) = 0;
            U_I((unsigned)&sv_PanelShown) = 0;
            unk_Unk513740();
            U_I((unsigned)&ai_Unk75fb80) = saved1;
            U_I((unsigned)&ai_Unk75fb84) = saved2;
            U_I((unsigned)&ai_Unk75fb88) = saved3;
            U_I((unsigned)&ai_Unk75fb90) = saved4;
            U_I((unsigned)&ai_Unk75fb94) = saved5;
            U_I((unsigned)&ai_Unk75fb98) = saved6;
            sb_PlaySample(U_I((unsigned)&full_data_6f2410), "GebaeudeVerlassen_s", 1, 0x3f, 0x7f, 0, 0);
            return;
        }
    }
}
