/* 0x5d5f10, applies the sound option volumes to the mixer (192 B): the master volume scales the sound
   driver (sb_SetMasterVolume, sfx * master / 127) and the music volume (msx_SetVolume, msx * master /
   127); the ambient driver gets the sfx volume (msx_SetAmbVolume). snd_VoiceVolume = speech / 127 and
   snd_MusicVolume = the low byte of the msx_freq option * 0.25. Called on every option apply. */

void ai_ApplyVolumes(void)
{
    float master;

    master = (float)ai_SoundOptions.masterVolume * (1.0f / 127.0f);
    sb_SetMasterVolume((int)((float)ai_SoundOptions.sfxVolume * master));
    msx_SetVolume((int)((float)ai_SoundOptions.msxVolume * master));
    msx_SetAmbVolume((int)((float)ai_SoundOptions.sfxVolume * master));
    snd_VoiceVolume = (float)ai_SoundOptions.voiceVolume * (1.0f / 127.0f);
    snd_MusicVolume = (float)(ai_SoundOptions.msxFreq & 0xff) * 0.25f;
}
