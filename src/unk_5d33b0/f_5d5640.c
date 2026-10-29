/* 0x5d5640, the options reader (1523 B), the twin of ai_SaveOptions: every option is read with
   GetPrivateProfileIntA from the INI ai_OptionsFile (the sections "Gfx", "Sound", "Game") into the
   option globals: texture_scale / details / lod_handling / shadow_detail into the bytes 1 / 0 / 2 / 3
   of ai_GfxDetail, floor_mipmapping / camera_limits / floor_lod / character_detail into the bytes of
   ai_GfxFloor (character_detail defaults to 1), fog_plane, the 12 screen tuning floats (the key is the
   value * 100, brightness r / g / b default 0 and a 0x32, contrast and gamma 100), gfx_set, cur_res
   (it also picks the screen mode record: ai_ScreenModeW / H), the sound volumes and msx_freq, the
   speed keys (invert_mouse is forced to 0), the town name ("Augsburg" when the key is missing), the
   historic / mission numbers, the panel options and the seven opponents (default 2).
   @deep the body is transcribed from build/ghidra/5d5640.c and the disassembly (every GetPrivateProfileIntA key, its default and its target byte/word, the 12 tuning floats * 0.01f, the town string, invert_mouse forced to 0, the mode table); readable, 15 code + 30 order hunks left. Two roots: (1) the cur_res mode table: the original digits i = (v & 0xff) * 8 as an integer and loads [eax+0x62d7b8] / [eax+0x62d7bc], i.e. a char-offset index into the table; a typed SCREENMODE index compiles to [eax*8+base] instead, and the char-offset spelling needs a dereferenced pointer cast (Q4) - a shared-header view decision. Tried: SCREENMODE ai_ScreenModes[i] (this), an int * view with i*2. (2) the rest: the store of each GetPrivateProfileIntA result is scheduled before / after the call's stack cleanup (mov ds:X,eax|al), ours consistently after. Tried: v = call; target = v (this, 15 hunks) vs target = call inlined (20 hunks, reverted). spell.py: no menu rule; the writer ai_SaveOptions (the twin) is byte */


void ai_LoadOptions(void)
{
    unsigned v;
    int i;

    v = GetPrivateProfileIntA("Gfx", "texture_scale", 0, ai_OptionsFile);
    ai_GfxDetail.byte[1] = v;
    v = GetPrivateProfileIntA("Gfx", "details", 0, ai_OptionsFile);
    ai_GfxDetail.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "lod_handling", 0, ai_OptionsFile);
    ai_GfxDetail.byte[2] = v;
    v = GetPrivateProfileIntA("Gfx", "shadow_detail", 0, ai_OptionsFile);
    ai_GfxDetail.byte[3] = v;
    v = GetPrivateProfileIntA("Gfx", "floor_mipmapping", 0, ai_OptionsFile);
    ai_GfxFloor.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "camera_limits", 0, ai_OptionsFile);
    ai_GfxFloor.byte[1] = v;
    v = GetPrivateProfileIntA("Gfx", "floor_lod", 0, ai_OptionsFile);
    ai_GfxFloor.byte[2] = v;
    v = GetPrivateProfileIntA("Gfx", "character_detail", 1, ai_OptionsFile);
    ai_GfxFloor.byte[3] = v;
    v = GetPrivateProfileIntA("Gfx", "fog_plane", 0, ai_OptionsFile);
    ai_GfxFog.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "brightness_r", 0, ai_OptionsFile);
    ai_ScreenTuning[0] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_g", 0, ai_OptionsFile);
    ai_ScreenTuning[1] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_b", 0, ai_OptionsFile);
    ai_ScreenTuning[2] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_a", 50, ai_OptionsFile);
    ai_ScreenTuning[3] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_r", 100, ai_OptionsFile);
    ai_ScreenTuning[4] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_g", 100, ai_OptionsFile);
    ai_ScreenTuning[5] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_b", 100, ai_OptionsFile);
    ai_ScreenTuning[6] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_a", 100, ai_OptionsFile);
    ai_ScreenTuning[7] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_r", 100, ai_OptionsFile);
    ai_ScreenTuning[8] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_g", 100, ai_OptionsFile);
    ai_ScreenTuning[9] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_b", 100, ai_OptionsFile);
    ai_ScreenTuning[10] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_a", 100, ai_OptionsFile);
    ai_ScreenTuning[11] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gfx_set", 0, ai_OptionsFile);
    ai_GfxSet.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "cur_res", 0, ai_OptionsFile);
    ai_GfxResolution.byte[0] = v;
    i = v & 0xff;
    ai_ScreenModeW = ai_ScreenModes[i].w;
    ai_ScreenModeH = ai_ScreenModes[i].h;
    v = GetPrivateProfileIntA("Sound", "master_vol", 0, ai_OptionsFile);
    ai_SoundOptions.masterVolume = v;
    v = GetPrivateProfileIntA("Sound", "sfx_vol", 0, ai_OptionsFile);
    ai_SoundOptions.sfxVolume = v;
    v = GetPrivateProfileIntA("Sound", "msx_vol", 0, ai_OptionsFile);
    ai_SoundOptions.msxVolume = v;
    v = GetPrivateProfileIntA("Sound", "speech_vol", 0, ai_OptionsFile);
    ai_SoundOptions.voiceVolume = v;
    v = GetPrivateProfileIntA("Sound", "msx_freq", 0, ai_OptionsFile);
    ai_SoundOptions.msxFreqByte[0] = v;
    v = GetPrivateProfileIntA("Game", "speed", 0, ai_OptionsFile);
    ai_OptionSpeed = v;
    v = GetPrivateProfileIntA("Game", "mouse_speed", 0, ai_OptionsFile);
    ai_OptionMouseSpeed = v;
    v = GetPrivateProfileIntA("Game", "camera_speed", 0, ai_OptionsFile);
    ai_OptionCameraSpeedByte = v;
    v = GetPrivateProfileIntA("Game", "scroll_speed", 0, ai_OptionsFile);
    ai_OptionScrollSpeed = v;
    v = GetPrivateProfileIntA("Game", "invert_mouse", 0, ai_OptionsFile);
    ai_GameSwitches.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "nachtwaechter", 1, ai_OptionsFile);
    ai_GameSwitches.byte[1] = v;
    v = GetPrivateProfileIntA("Game", "show_cursor_txt", 1, ai_OptionsFile);
    ai_GameSwitches.byte[2] = v;
    v = GetPrivateProfileIntA("Game", "show_geb_info", 1, ai_OptionsFile);
    ai_GameSwitches.byte[0] = 0;
    ai_GameSwitches.byte[3] = v;
    GetPrivateProfileStringA("Game", "stadt", "Augsburg", ai_TownName, 0x40, ai_OptionsFile);
    v = GetPrivateProfileIntA("Game", "historie", 0, ai_OptionsFile);
    ai_OptionHistorie = v;
    v = GetPrivateProfileIntA("Game", "mission", 1, ai_OptionsFile);
    ai_OptionMission = v;
    v = GetPrivateProfileIntA("Game", "net_mission", 1, ai_OptionsFile);
    ai_OptionNetMission = v;
    v = GetPrivateProfileIntA("Game", "panel_mode", 1, ai_OptionsFile);
    ai_GamePanel.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "help_events", 1, ai_OptionsFile);
    ai_GamePanel.byte[1] = v;
    v = GetPrivateProfileIntA("Game", "difficulty", 1, ai_OptionsFile);
    ai_GamePanel.byte[2] = v;
    v = GetPrivateProfileIntA("Game", "hints", 1, ai_OptionsFile);
    ai_GamePanel.byte[3] = v;
    v = GetPrivateProfileIntA("Game", "panel_help", 1, ai_OptionsFile);
    ai_OptionPanelHelp.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "opponent_a", 2, ai_OptionsFile);
    ai_Opponents[0] = v;
    v = GetPrivateProfileIntA("Game", "opponent_b", 2, ai_OptionsFile);
    ai_Opponents[1] = v;
    v = GetPrivateProfileIntA("Game", "opponent_c", 2, ai_OptionsFile);
    ai_Opponents[2] = v;
    v = GetPrivateProfileIntA("Game", "opponent_d", 2, ai_OptionsFile);
    ai_Opponents[3] = v;
    v = GetPrivateProfileIntA("Game", "opponent_f", 2, ai_OptionsFile);
    ai_Opponents[5] = v;
    v = GetPrivateProfileIntA("Game", "opponent_f", 2, ai_OptionsFile);
    ai_Opponents[5];
    v = GetPrivateProfileIntA("Game", "opponent_g", 2, ai_OptionsFile);
    ai_Opponents[6] = v;
}
