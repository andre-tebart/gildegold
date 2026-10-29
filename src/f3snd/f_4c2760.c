/* 0x4c2760, plays the voice sample of speaker id in bank now (snd_FindVoiceSample) at the voice volume,
   unless a queued sound is playing; returns the sample handle or 0 */

int snd_PlayVoice(int id, int bank, char *text, int var)
{
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    if (snd_Queue != 0 && snd_Queue->playing != 0)
        return 0;
    s = snd_FindVoiceSample(id, bank, text, var);
    if (s != 0) {
        sb_Play((char *)s, 0, 0);
        sb_SetVolume(s, (int)(snd_VoiceVolume * 127.0f));
    }
    return s;
}
