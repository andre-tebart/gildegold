/* 0x59f410, the room switch of the other buildings (1424 B, ROUGH): the room 0x76faa0 (the index word) and its
   callback : the room record 0x6f2310 = param_1, 0x5f8430 / 0x5f1298 = param_1 + 1, 0x6f22a4 = 0x6f229c + 1,
   0x52adb0 / 0x52ae40; 0x50d7f0 (the room), 0x4478e0 (the callback), 0x513050 / 0x513400, the matrices
   0x43b790 / 0x43b860 (+0x5c / +0x90), the saved view; then the room handler switch (0x2c / 0x109 0x592290,
   0x2d 0x59290, 0x33 0x592320, 0x37 0x5938c0, 0x107 0x593e80, 0x205 0x587560, 0x211 0x587ad0, 0x21b 0x5876a0,
   0x276 0x5d9a80, 0x27f 0x5d9b90, 0x28a 0x5d9b20, default 0x58f620); the exit restores the view / device and
   switches back to the room 0xfd (0x4ffe90, 0x5f1298) */

void ai_Unk59f410(unsigned short *param_1)
{
    unsigned short *room;
    int idx;
    int kind;
    int saved1, saved2, saved3, saved4, saved5, saved6;

    saved1 = U_I((unsigned)&ai_Unk75fb80);
    saved2 = U_I((unsigned)&ai_Unk75fb84);
    saved3 = U_I((unsigned)&ai_Unk75fb88);
    saved4 = U_I((unsigned)&ai_Unk75fb90);
    saved5 = U_I((unsigned)&ai_Unk75fb94);
    saved6 = U_I((unsigned)&ai_Unk75fb98);
    kind = U_I((unsigned)&d3_CurrentScene);
    U_I((unsigned)&sv_PanelShown) = (int)param_1;
    U_I((unsigned)&evt_NoGroup) = U_I(param_1 + 1);
    U_I((unsigned)&ch_CurrentScriptGroup) = U_I(param_1 + 1);
    U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth) + 1;
    U_I((unsigned)&full_data_6237c8) = 0;
    sv_Unk52adb0();
    sv_Unk52ae40();
    idx = -1;
    room = param_1;
    if (*(signed char *)((unsigned)&init_ObjectUniverse + *param_1) != -2) {
        if (*(signed char *)((unsigned)&init_ObjectUniverse + *param_1) == -1 ||
            *(unsigned char *)((unsigned)&init_SceneDirty + *(signed char *)((unsigned)&init_ObjectUniverse + *param_1) * 0x3e0) != 0) {
            U_I((unsigned)&d2_CutsceneActive) = 1;
            init_LoadObjectScene(*param_1);
            U_I((unsigned)&d2_CutsceneActive) = 0;
        }
        *(unsigned short *)(U_I((unsigned)&ai_FallbackBuilding) + 0x29) = *param_1;
        d3if_SelectScene(*(signed char *)((unsigned)&init_ObjectUniverse + *param_1), 0);
        d3_InflateScene(3);
        init_ShowBuildingScene(U_I((unsigned)&ai_FallbackBuilding), *param_1);
        init_FinishBuildingScene(U_I((unsigned)&ai_FallbackBuilding), param_1);
        /* ROUGH: the device callback (0x5fb5b0 + 0x3c8 -> +0x210) */
        idx = U_I((unsigned)&ai_Unk5f8438);
        d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x5c));
        d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x90));
        U_I((unsigned)&ai_Unk75fb80) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x4c);
        U_I((unsigned)&ai_Unk75fb84) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x50);
        U_I((unsigned)&ai_Unk75fb88) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x54);
        U_I((unsigned)&ai_Unk75fb90) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x84);
        U_I((unsigned)&ai_Unk75fb94) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x88);
        U_I((unsigned)&ai_Unk75fb98) = U_I(U_I((unsigned)&ai_Unkfc2dec) + 0x8c);
        U_I((unsigned)&ai_Unk6f22cc) = 0;
        U_I((unsigned)&ai_Unk6f22d0) = 0;
    }
    U_I((unsigned)&snd_AmbientOn) &= 0xffffff00;
    switch (*param_1) {
    case 0x2c:
    case 0x109: ai_UpgradeContact(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x2d: ai_Unk592290(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x33: ai_Unk592320(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x37: ai_Unk5938c0(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x107: ai_Unk593e80(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x205: ai_Unk587560(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x211: ai_Unk587ad0(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x21b: ai_Unk5876a0(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x276: ai_GypsyContact(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x27f: ai_TheatreContact(U_I((unsigned)&ai_FallbackBuilding)); break;
    case 0x28a: ai_CurseContact(U_I((unsigned)&ai_FallbackBuilding)); break;
    default: ai_PumpFrames(param_1); break;
    }
    U_I((unsigned)&snd_AmbientOn) |= 1;
    sv_Unk52adb0();
    sv_Unk52ae40();
    sv_ReleaseNamePlates();
    U_I((unsigned)&ai_Unk7662c0) = 0;
    /* ROUGH: the device callbacks as in 0x59ee40, then the room 0xfd restored (0x4ffe90, 0x5f1298) */
    if (U_I((unsigned)&ai_Unk5f8438) != -1)
        sv_Unk52bb70(U_I((unsigned)&ai_Unk5f8438));
    d3_DeflateScene(3);
    d3if_SelectScene(kind, 0);
    U_I((unsigned)&sv_PanelShown) = 0;
    *(unsigned short *)(U_I((unsigned)&ai_FallbackBuilding) + 0x29) = 0xfd;
    U_I((unsigned)&evt_NoGroup) = 0;
    U_I((unsigned)&ch_CurrentGroup) = U_I(U_I((unsigned)&ai_FallbackBuilding) + 1);
    room = (unsigned short *)gm_Query(U_I(U_I((unsigned)&ai_FallbackBuilding) + 0x5d), 2, 6, 0, 0xfd);
    U_I((unsigned)&ch_CurrentScriptGroup) = U_I(room + 1);
    U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth);
    U_I((unsigned)&ai_Unk6f22d0) = (int)param_1;
    U_I((unsigned)&ai_Unk75fb80) = saved1;
    U_I((unsigned)&ai_Unk75fb84) = saved2;
    U_I((unsigned)&ai_Unk75fb88) = saved3;
    U_I((unsigned)&ai_Unk6f22cc) = idx;
    U_I((unsigned)&ai_Unk75fb90) = saved4;
    U_I((unsigned)&ai_Unk75fb94) = saved5;
    U_I((unsigned)&ai_Unk75fb98) = saved6;
}
