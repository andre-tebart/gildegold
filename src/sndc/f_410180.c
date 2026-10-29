/* 0x410180, script command SpeechQueued: queues the spoken text (speaker id, bank, text, variation, delay)
   with the voice queue, only while the running script is in no group; returns the voice sample or 0 */

int sndc_SpeechQueued(int *id, int *bank, char **name, int *var, int *delay)
{
    if (evt_Cur->group != evt_NoGroup)
        return 0;
    return snd_QueueVoice(*id, *bank, *name, *var, *delay);
}
