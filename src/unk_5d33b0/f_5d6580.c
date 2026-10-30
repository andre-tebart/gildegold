/* 0x5d6580, the sound options screen "menu\options_sfx" 0x62db1c (974 B): the same window family as
   0x5d5fd0. Fills five controls of the view 1 from ai_SoundOptions: four volume sliders (0 .. 0x7f: master, sfx, msx,
   speech) and the msx_freq list (5 items), then pumps the screen (sv_RunFrame, this function as the callback): a click or the
   right button pops it, the first button of the row applies. While the left button is down on a volume slider the new
   values are previewed live (sb_SetMasterVolume of sfx * master / 100, msx_SetVolume of msx * master / 100; master is the
   slider * 0.01) and the first press on the sfx slider plays the sample "NeuesUpgrade_s" 0x62db0c of the system bank
   (once per visit). On apply the five controls are read back into ai_SoundOptions, the INI is written (ai_SaveOptions)
   and the volumes applied (ai_ApplyVolumes). Void; opened by the options menu 0x5d7770.
   Text slots (txt_Ptrs): the labels 0x1c91 .. 0x1c95, the msx_freq items 0x1c87 .. 0x1c8b, the buttons 0x1ca2 / 0x1ca3. */

void ai_OptionsSfxScreen(void)
{
    unsigned mask;
    int form, apply, sample;
    int master, sfx, msx, speech, freq;
    int ids[2];
    int okId, cancelId;
    float fMaster, fSfx, fMsx;

    apply = 0;
    sample = 0;
    mask = sv_FrameMaskU & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_sfx");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindowF(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c5b);
    d2_SetForm(form, 1);
    master = d2_GetWinObj(form, 1, 0);
    sfx = d2_GetWinObj(form, 1, 1);
    msx = d2_GetWinObj(form, 1, 2);
    speech = d2_GetWinObj(form, 1, 3);
    freq = d2_GetWinObj(form, 1, 4);
    d2_SetObjValue4(master, 0, 0x7f, ai_SoundOptions.masterVolume);
    d2_SetSliderFlags(master, 0x882);
    d2_SetLabel(master, txt_Ptrs[0x1c91]);
    d2_SetObjValue4(sfx, 0, 0x7f, ai_SoundOptions.sfxVolume);
    d2_SetSliderFlags(sfx, 0x882);
    d2_SetLabel(sfx, txt_Ptrs[0x1c92]);
    d2_SetObjValue4(msx, 0, 0x7f, ai_SoundOptions.msxVolume);
    d2_SetSliderFlags(msx, 0x882);
    d2_SetLabel(msx, txt_Ptrs[0x1c93]);
    d2_SetObjValue4(speech, 0, 0x7f, ai_SoundOptions.voiceVolume);
    d2_SetSliderFlags(speech, 0x882);
    d2_SetLabel(speech, txt_Ptrs[0x1c94]);
    d2_SetObjValue4(freq, 0, 4, ai_SoundOptions.msxFreq & 0xff);
    d2_SetSliderFlags(freq, 0x82);
    d2_SetLabel(freq, txt_Ptrs[0x1c95]);
    d2_SetList(freq, 5, txt_Ptrs[0x1c87], txt_Ptrs[0x1c88], txt_Ptrs[0x1c89], txt_Ptrs[0x1c8a], txt_Ptrs[0x1c8b]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsSfxScreen, mask) != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == okId) {
                apply = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancelId)
                sv_PopRequest = 1;
        }
        if (dm_L.ldown != 0 && (d2_PointerObj == master || d2_PointerObj == sfx || d2_PointerObj == msx
                                || d2_PointerObj == speech)) {
            fMaster = (float)d2_GetObjValue(master) * 0.01f;
            fSfx = (float)d2_GetObjValue(sfx);
            fMsx = (float)d2_GetObjValue(msx);
            sb_SetMasterVolume((int)(fSfx * fMaster));
            msx_SetVolume((int)(fMsx * fMaster));
            if (d2_PointerObj == sfx && sample == 0)
                sample = (int)sb_PlaySampleI(snd_SystemBank, "NeuesUpgrade_s", 1, 0x3f, 0x7f, 0, 0);
        }
    }
    if (apply != 0) {
        ai_SoundOptions.masterVolume = d2_GetObjValue(master);
        ai_SoundOptions.sfxVolume = d2_GetObjValue(sfx);
        ai_SoundOptions.msxVolume = d2_GetObjValue(msx);
        ai_SoundOptions.voiceVolume = d2_GetObjValue(speech);
        ai_SoundOptions.msxFreqByte[0] = d2_GetObjValue(freq);
        ai_SaveOptions();
        ai_ApplyVolumes();
    }
    d2_CloseView(form);
}
