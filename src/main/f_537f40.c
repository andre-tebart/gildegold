/* 0x537f40, "end of a game": the reverse of 0x537de0: plates / hints cleared, the tick callback removed, the sound banks and the weather emitters (0x76061c / 0x760618 / the sky layers 0x760620 ..) freed, the
   modules shut down (0x4b1e40, 0x4b39f0, 0x53d590 / 0x53d9b0 / 0x53d5d0, sv's 0x5223c0, 0x503e80, 0x4fe7f0 / 0x4fe830, the objects 0x1468ae0 ..), and the selection state cleared; ROUGH */

void main_Unk537f40(void)
{
    int *p;
    int sc;

    sv_ClearNamePlates();
    d2_CopyCaption((char *)(unsigned)&ch_EmptyName);
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    U_I((unsigned)&d2_CutsceneActive) = 1;
    tb_CloseFunction(sv_UpdateClock);
    U_I((unsigned)&ai_Unk6f0d8c) = 0;
    evt_UnloadAll();
    if (U_I(((unsigned)&cm_RequestedSaveName + 0x44)) != 0)
        d3io_ReleaseObject(U_I(((unsigned)&cm_RequestedSaveName + 0x44)));
    if (snd_SfxEnabled != 0) {
        snd_ClearQueue();
        d3snd_DetachAll();
        if (U_I((unsigned)&snd_Bank) != 0) {
            sb_UnloadSampleBank(U_I((unsigned)&snd_Bank));
            U_I((unsigned)&snd_Bank) = 0;
            sb_UnloadSampleBank(U_I((unsigned)&full_data_6f2408));
            U_I((unsigned)&full_data_6f2408) = 0;
            sb_UnloadSampleBank(U_I((unsigned)&full_data_6f240c));
            U_I((unsigned)&full_data_6f240c) = 0;
            sb_UnloadSampleBank(U_I((unsigned)&full_data_6f2410));
            U_I((unsigned)&full_data_6f2410) = 0;
            sb_UnloadSampleBank(U_I((unsigned)&full_data_6f2414));
            U_I((unsigned)&full_data_6f2414) = 0;
            sb_UnloadSampleBank(U_I((unsigned)&ch_SndLib));
            U_I((unsigned)&ch_SndLib) = 0;
        }
    }
    ai_Unk4b1e40();
    ai_ClearMissions();
    he_Unk53d590();
    he_Unk53d9b0();
    he_Unk53d5d0();
    sv_Unk5223c0();
    gm_Unk503e80();
    gm_ClearBuildings();
    gm_ClearObjects();
    p = (int *)(unsigned)&ai_Unk1468ae0;
    do {
        if (*p != 0)
            ch_Delete(*p);
        p++;
    } while ((int)p < (unsigned)((unsigned)ai_Unk1468ae0 + 0xc00));
    if (U_I((unsigned)&ai_Unk76061c) != 0) {
        d3t_FreeSnow(U_I((unsigned)&ai_Unk76061c));
        U_I((unsigned)&ai_Unk76061c) = 0;
    }
    if (U_I((unsigned)&ai_Unk760618) != 0) {
        d3t_DeleteRain(U_I((unsigned)&ai_Unk760618));
        U_I((unsigned)&ai_Unk760618) = 0;
    }
    if (U_I((unsigned)&d3_Sky) != 0) {
        p = (int *)(unsigned)&ai_Unk760620;
        sc = U_I((unsigned)&d3_Sky);
        do {
            if (*p != 0) {
                d3sky_DeleteLayer(sc, *p);
                sc = U_I((unsigned)&d3_Sky);
                *p = 0;
            }
            p++;
        } while ((int)p < (unsigned)((unsigned)ai_Unk760620 + 0x18));
        d3sky_Destroy(sc);
        U_I((unsigned)&d3_Sky) = 0;
    }
    *(char *)(unsigned)&d3_FogByDistance = 0;
    if (U_I((unsigned)&d3t_SunRays) != 0) {
        d3t_RemoveSunRays(U_I((unsigned)&d3t_SunRays));
        U_I((unsigned)&d3t_SunRays) = 0;
    }
    U_I((unsigned)&d3t_CamMove) = 0;
    init_FreeScenes();
    ai_Unk5bc0f0();
    anm_Free();
    ai_ResetMessages();
    udp_Exit();
    net_Close();
    U_I((unsigned)&full_data_6f25c4) = 0;
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&ai_Unk75ec60) = 0;
    U_I((unsigned)&sv_PanelShown) = 0;
    U_I((unsigned)&ai_PlayerBuilding) = 0;
    U_I((unsigned)&ai_FallbackBuilding) = 0;
}
