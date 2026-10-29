/* 0x59ee40, the enter-building driver for the player's own / kind 0x1c buildings (1398 B, ROUGH: the view and
   device bookkeeping is condensed): 0x59e210 refuses -> 0x75e8c0 = 0; the room record 0x511e60 (param_1) is
   marked (0xfd7c58 + idx * 0xf8); 0x4c28a0 (param_1); the room 0x4ffe90 (param_1 + 0x5d, 2, 6, 0, 0xfd), else
   (2, 6, 4, 2); the camera matrices 0x43b790 / 0x43b860; the kind switch opens the window of the building:
   0x1e 0x593b00 / 0x4bcbe0 (param_1 + 0x71), 0x1f 0x593ca0, 0x20 0x593960, 0x27 0x594e50, 0x47 0x5950e0,
   0x48 .. 0x4a 0x586840, 0x4e .. 0x50 0x5d9c30, default 0x58f620; the exit clears the record, the sound and
   view state (0x4c2c00 / 0x5136b0 / 0x43db30 / 0x4478e0 / 0x513740) */

int init_BuildingSceneUniverse(); int init_LoadBuildingScene(); void d3fl_SetDetail(unsigned int n); void veg_HidePlants();
int ai_Unk586840(); int ai_GypsyProductionContact(); int ai_Unk587ad0();
int ai_Unk5876a0(); int ai_GypsyContact(); int ai_TheatreContact(); int ai_CurseContact();
int ai_Unk587560();

void ai_Unk59ee40(unsigned char *param_1)
{
    unsigned short *room;
    int idx;
    int kind;
    int saved1, saved2, saved3, saved4, saved5, saved6;

    if (ai_Unk59e210(param_1) == 0) {
        U_I((unsigned)&sv_PanelFlag) = 0;
        return;
    }
    U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth) + 1;
    U_I((unsigned)&full_data_6237c8) = 0;
    sv_Unk52adb0();
    sv_ClearSelection();
    saved1 = U_I((unsigned)&ai_Unk75fb80);
    saved2 = U_I((unsigned)&ai_Unk75fb84);
    saved3 = U_I((unsigned)&ai_Unk75fb88);
    saved4 = U_I((unsigned)&ai_Unk75fb90);
    saved5 = U_I((unsigned)&ai_Unk75fb94);
    saved6 = U_I((unsigned)&ai_Unk75fb98);
    kind = U_I((unsigned)&d3_CurrentScene);
    strcpy((char *)(unsigned)&ch_StepSample, "normal_s");
    idx = init_BuildingSceneUniverse(param_1);
    U_I(((unsigned)&ai_Unkfd7c50 + 0x8) + idx * 0xf8) = ((unsigned)&ai_Unkfd7c50 + 0x8);
    if (idx != 0) {
        snd_LoadBuildingVoices(param_1);
        if (idx == -1 || *(unsigned char *)((unsigned)&init_SceneDirty + idx * 0x3e0) != 0) {
            U_I((unsigned)&d2_CutsceneActive) = 1;
            sv_RunFrame(0, 0x603c7);
            sv_RunFrame(0, 0x603c7);
            init_LoadBuildingScene(*param_1);
            U_I((unsigned)&d2_CutsceneActive) = 0;
        }
        U_I((unsigned)&ai_FallbackBuilding) = (int)param_1;
        U_I((unsigned)&evt_NoGroup) = U_I(param_1 + 1);
        U_I((unsigned)&ch_CurrentGroup) = U_I(param_1 + 1);
        room = (unsigned short *)gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0xfd);
        if (room == 0)
            room = (unsigned short *)gm_Query(U_I(param_1 + 0x5d), 2, 6, 4, 2);
        U_I((unsigned)&ch_CurrentScriptGroup) = U_I(room + 1);
        d3if_SelectScene(idx, 0);
        d3_InflateScene(3);
        if (U_I((unsigned)&ai_Unk633738) != 0) {
            if (*(unsigned char *)((unsigned)&ai_GfxFloor + 2) == 2)
                *(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) = (*(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) & 0xe7) | 4;
            else
                *(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) = (*(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) & 0xeb) | 8;
            d3fl_SetDetail(0x40);
        }
        init_ShowBuildingScene(U_I((unsigned)&ai_FallbackBuilding), *room);
        init_FinishBuildingScene(U_I((unsigned)&ai_FallbackBuilding), room);
        /* ROUGH: the device callback (0x5fb5b0 + 0x3c8 -> +0x210) */
        if (*(unsigned short *)(param_1 + 0x27) == U_W((unsigned)&ai_PlayerId)) {
            d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x5c));
            d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x90));
        } else {
            kind = gm_BuildingClass(*param_1);
            if (kind == 2) {
                if (d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0 &&
                    d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0) {
                    d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114));
                    d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120));
                }
            } else if (*(char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d) == 0x1c) {
                d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x5c));
                d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x90));
            } else {
                if (d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0 &&
                    d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120), (float *)(unsigned)&ai_Unk5ebba4, 0x3dcccccd) == 0) {
                    d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x114));
                    d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x120));
                }
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
    switch (*param_1) {
    case 0x1e:
        ai_Unk593b00(U_I((unsigned)&ai_FallbackBuilding));
        veg_HidePlants(U_I(param_1 + 0x71));
        break;
    case 0x1f:
        ai_Unk593ca0(U_I((unsigned)&ai_FallbackBuilding));
        break;
    case 0x20:
        ai_Unk593960(U_I((unsigned)&ai_FallbackBuilding));
        break;
    case 0x27:
        ai_Unk594e50(U_I((unsigned)&ai_FallbackBuilding));
        break;
    case 0x47:
        ai_Unk5950e0(U_I((unsigned)&ai_FallbackBuilding));
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        ai_Unk586840(U_I((unsigned)&ai_FallbackBuilding));
        break;
    case 0x4e:
    case 0x4f:
    case 0x50:
        ai_GypsyProductionContact(U_I((unsigned)&ai_FallbackBuilding));
        break;
    default:
        ai_PumpFrames(0);
        break;
    }
    if (idx != -1) {
        U_I(((unsigned)&ai_Unkfd7c50 + 0x8) + idx * 0xf8) = 0;
        U_I((unsigned)&d3_Sky) = 0;
    }
    sv_Unk52adb0();
    sv_ClearSelection();
    U_I((unsigned)&ai_Unk6f22cc) = 0;
    U_I((unsigned)&ai_Unk6f22d0) = 0;
    /* ROUGH: 0x4284a0 / 0x429e90 / 0x52bb70 of the device record and 0x5fb5b0 callback +0x294 */
    if (U_I((unsigned)&ai_Unk5f8438) != -1)
        sv_Unk52bb70(U_I((unsigned)&ai_Unk5f8438));
    snd_UnloadBuildingVoices();
    unk_Unk5136b0(U_I((unsigned)&ai_FallbackBuilding));
    d3_DeflateScene(3);
    d3if_SelectScene(kind, 0);
    U_I((unsigned)&ai_Unk7662c0) = 0;
    U_I((unsigned)&ai_FallbackBuilding) = 0;
    U_I((unsigned)&evt_NoGroup) = 0;
    U_I((unsigned)&ch_CurrentGroup) = 0xffffffff;
    U_I((unsigned)&ch_CurrentScriptGroup) = 0xffffffff;
    unk_Unk513740();
    U_I((unsigned)&ai_Unk75fb80) = saved1;
    U_I((unsigned)&ai_Unk75fb84) = saved2;
    U_I((unsigned)&ai_Unk75fb88) = saved3;
    U_I((unsigned)&ai_Unk75fb90) = saved4;
    U_I((unsigned)&ai_Unk75fb94) = saved5;
    U_I((unsigned)&ai_Unk75fb98) = saved6;
}
