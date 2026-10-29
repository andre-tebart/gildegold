/* 0x5316d0, shuts the game modules down in reverse order of their start (0x520f10 unregisters the tick callback of the sv screens) */

void main_ShutdownModules(void)
{
    ai_SaveOptions();
    gm_FreeIconNames();
    main_FreeSoundBanks();
    evt_Shutdown();
    sv_UnregisterTick();
    if (snd_SfxEnabled != 0) {
        sb_Exit();
        d3snd_Exit();
    }
    if (snd_MusicEnabled != 0)
        msx_Exit();
    if (snd_SfxEnabled != 0) {
        sfx_Shutdown();
        d3sndw_Exit();
    }
    he_SaveMessageFilter();
    txt_FreeFiles();
    gm_Free();
    ai_FreeSettings();
    ahm_FreePrototypes();
    hmt_Exit();
    gv_FreeEffectObjects();
    ch_Close();
    ai_ResetMessages();
}
