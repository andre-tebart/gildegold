/* 0x4c27d0, queues a voice sample (snd_FindVoiceSample) with delay */

int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay)
{
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    s = snd_FindVoiceSample(id, bank, text, var);
    if (s != 0) {
        sb_SetVolume(s, (int)(snd_VoiceVolume * 127.0f));
        snd_AddToQueue((char *)s, delay);
    }
    return s;
}
