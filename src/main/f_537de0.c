/* 0x537de0, "start of a game": resets the global state (0x6f2358 = 1, 0x6f2598 = 1 the round number, the day 0x770ae0 = 2, the quit flags 0x6f259c / 0x6f25a0 / 0x6f25ad), registers the tick callback 0x5315c0
   (0x42e970 (cb, 0x47) / 0x42ea10), starts the sound banks (natur\regen, wind, blitzunddonner, objekte\buecher, TuerenUndRaeume, Transporter, Lebewesen\schritte) when the sound is on, and
   clears the selection (0x6f2308 .. 0x75ec60) before starting the modules (0x4bb810 anm, 0x50d460, 0x4e5770 ..) */

void main_Unk537de0(void)
{
    he_Unk53d4b0();
    U_I((unsigned)&sv_ScreenDirty) = 1;
    U_I((unsigned)&full_data_6f2598) = 1;
    U_I((unsigned)&ai_Time) = 2;
    U_I((unsigned)&ai_GameOver) = 0;
    U_I((unsigned)&ai_LeaveGame) = 0;
    *(char *)((unsigned)&ai_GameState + 0x1) = 0;
    U_I((unsigned)&cm_SpeedLevel) = 2;
    tb_OpenFunction(sv_UpdateClock, 0x47);
    tb_SetFunctionOff(sv_UpdateClock, 1);
    he_Unk53eea0();
    gv_ClearGraveyard();
    ord_ResetStatistics();
    U_I(((unsigned)&cm_RequestedSaveName + 0x44)) = d3io_LoadObjectRef("Buch", "Buch");
    if (snd_SfxEnabled != 0) {
        U_I((unsigned)&snd_Bank) = sb_LoadSampleBank("natur\\regen.sbf", 0);
        U_I((unsigned)&full_data_75e630) = sb_LoadSampleBank("natur\\wind.sbf", 0);
        U_I((unsigned)&full_data_6f2408) = sb_LoadSampleBank("natur\\blitzunddonner.sbf", 0);
        U_I((unsigned)&full_data_6f240c) = sb_LoadSampleBank("objekte\\buecher.sbf", 0);
        U_I((unsigned)&full_data_6f2410) = sb_LoadSampleBank("objekte\\TuerenUndRaeume.sbf", 0);
        U_I((unsigned)&full_data_6f2414) = sb_LoadSampleBank("objekte\\Transporter.sbf", 0);
        U_I((unsigned)&ch_SndLib) = sb_LoadSampleBank("Lebewesen\\schritte.sbf", 1);
    }
    msx_SetFadeTime(3000);
    he_Unk53d930();
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    U_I((unsigned)&full_data_6f25c0) = 0;
    U_I((unsigned)&ai_Unk75ec60) = 0;
    U_I((unsigned)&sv_PanelShown) = 0;
    U_I((unsigned)&ai_PlayerBuilding) = 0;
    U_I((unsigned)&ai_FallbackBuilding) = 0;
    ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    anm_Alloc();
    ch_Reset();
    ai_Unk589410();
    init_ResetSceneUniverses();
    ai_Unk4e5770();
}
