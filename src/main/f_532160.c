/* 0x532160, "main_OpenEngine": loads the text definitions ("<path>die_gilde_add_on_text_def.." 0x42c9c0 / 0x42cbc0), shows the loading screen "Misc\\Loading_main" while the data files, the sound / music
   (0x425bc0, "system.sbf", "include\\sfx.ini") and all game modules (0x520ee0 registers sv's tick callback, 0x50d460 ..) are initialised, then waits for the black fade (0x4767d0 "BLACK") by running 0x5309a0 and
   clears the run state; returns 1; ROUGH */

int main_Unk532160(void)
{
    char tmp[260], tmp2[260];
    int f, k;
    unsigned char *fade;

    U_I((unsigned)&d2_CutsceneActive) = 1;
    if (U_I((unsigned)&full_data_6f2460) == 0) {
        crt_Sprintf(tmp, (char *)(unsigned)"%s%s", (char *)(unsigned)&full_data_624690, "die_gilde_add_on_text.def");
        if (txt_LoadIncludes(tmp) == 0)
            err_Message("main_OpenEngine(): could not open txt-engine!");
    } else {
        crt_Sprintf(tmp, (char *)(unsigned)"%s%s", (char *)(unsigned)&full_data_624690, "die_gilde_add_on_text.def");
        txt_OpenText(tmp, 0);
    }
    ai_Unk4b1f30();
    d2_ClearScreen();
    f = d2_OpenForm(0, 0, "Misc\\Loading_main");
    k = f * 0x2ac;
    d2_PlaceWindow(U_I(((unsigned)&d2_Views + 0x4) + k), 3);
    d2_MoveObj(0, *(short *)(unsigned)&d2_ScreenH - *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I(((unsigned)&d2_Views + 0x8) + k) * 0x3c4) - 0x30, U_I(((unsigned)&ai_Unkf8f13c + 0x250) + U_I(((unsigned)&d2_Views + 0x8) + k) * 0x3c4));
    d2_PlaceWindow(U_I(((unsigned)&d2_Views + 0x8) + k), 1);
    io_PrintText("$ADie Gilde - Gold Edition - Version 2.05$X");
    net_Connect(0, 0);
    ai_Unk4c3480();
    ai_Unk587de0();
    crt_Sprintf(tmp, "%sdata\\", (char *)(unsigned)&full_data_624690);
    gm_LoadTables(tmp);
    unk_Unk4eddf0();
    unk_Unk4fde90();
    ai_FreeSettings();
    gm_ItemPrice(0x1d5, 100);
    gm_ItemPrice(0x1d7, 100);
    gm_ItemPrice(0x1d4, 100);
    gm_ItemPrice(0x1da, 100);
    gm_ItemPrice(0x1d9, 100);
    gm_ItemPrice(0x1d6, 100);
    gm_ItemPrice(0x1d8, 100);
    if (snd_SfxEnabled != 0) {
        sfx_Startup();
        if (sb_Init(((unsigned)&ai_Unk9367e0 + 0x52ea0), 0x30, 2, 0x10, 0xac44) != 0) {
            snd_SfxEnabled = 0;
            U_I((unsigned)&snd_MusicEnabled) = 0;
        }
        d3snd_Init(0x40);
        d3sndw_Init(0x30);
        crt_Sprintf(tmp, "%ssfx\\", (char *)(unsigned)&full_data_624690);
        sb_SetPath(tmp);
        U_I((unsigned)&full_data_6f2400) = sb_LoadSampleBank("system.sbf", 0);
        crt_Sprintf(tmp2, "%s\\include_sfx.ini", (char *)(unsigned)&full_data_75e240);
        main_Unk537c70(tmp2);
    }
    if (U_I((unsigned)&snd_MusicEnabled) != 0) {
        msx_Init(2, 0x10, 0xac44);
        crt_Sprintf(tmp, "%smsx\\", (char *)(unsigned)&full_data_624690);
        msx_SetPath(tmp);
    }
    ai_ApplyVolumes();
    sv_InitTick();
    init_ResetSceneUniverses();
    ch_Open();
    d3t_CreateSnowMipmaps();
    ai_Init();
    ahm_LoadPrototypes(U_I((unsigned)&full_data_6f2468));
    if (U_I((unsigned)&full_data_6f246c) != 0)
        hmt_CreateBinaryFile();
    U_I((unsigned)&full_data_6f246c) = 0;
    hmt_Init();
    ai_Unk4d27a0();
    d2_CloseView(f);
    fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x5a, 1);
    while ((*fade & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_ReleaseFade(fade);
    memset((char *)(unsigned)&init_PlayerProfile, 0, 0x8c);
    U_I((unsigned)&main_ScreenOpen) = 1;
    evt_Init();
    evtc_Register();
    ocmd_Register();
    chc_Register();
    sndc_Register();
    U_I((unsigned)&d3e_Resolve) = (unsigned)&evt_GetStartScript;
    ai_ApplyOptions();
    sv_PumpMessages();
    return 1;
}
