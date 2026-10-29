/* 0x4c01b0, stops the queue: restores the music after a fanfare (fade back to 1.0), frees every queued sound */
void snd_ClearQueue(void)
{
    G16SND *n;

    if (snd_FanfareFade != 0) {
        snd_FanfareFade = 0;
        snd_FadeMusic(1.0f, 2000);
    }
    if (snd_Queue != 0) {
        do {
            n = snd_Queue->next;
            sb_SetRestart(snd_Queue->smp, 0);
            mem_Free(snd_Queue);
            snd_Queue = n;
        } while (n != 0);
    }
}
