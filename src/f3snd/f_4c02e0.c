/* 0x4c02e0, the queue step: starts the first sound once its delay is over (a "Fanfare" fades the ambience out),
   drops it when it has ended ("MausclickLinks" after a fanfare fades the ambience back in)
   @deep B draft, 75 code hunks, no meaning error: read the whole body against the original 0x4c02e0..0x4c0462. Every statement, constant, callee and branch corresponds (paused gate, delay < t - snd_QueueTime with t = F3SND_TIME's three volatile reads now equal, the Fanfare fade-out, sb_Play, playing=1/snd_QueueTime=t, sb_IsPlaying, the free and the MausclickLinks fade-back). What is left is scheduling and register allocation: ours loads delay before computing t - snd_QueueTime, hoists mov esi,1 above the sample/entry test, re-reads snd_Queue into another register after sb_Play, and jumps straight to the epilogue where the original merges through a second test ecx,ecx. Same operations spelled/ordered differently; readable (names, the G16SMP/G16SMPENTRY union view). */

void snd_UpdateQueue(void)
{
    G16SND *n;
    unsigned int t;

    if (snd_QueuePaused != 0)
        return;
    t = F3SND_TIME;
    if (snd_Queue == 0)
        return;
    if (snd_Queue->playing == 0) {
        if (snd_Queue->delay < t - snd_QueueTime) {
            if (snd_Queue->sample != 0 && snd_Queue->sample->info != 0) {
                if (strstr(snd_Queue->sample->info->name, "Fanfare") && snd_FanfareFade == 0) {
                    snd_FadeMusic(0.0f, 2000);
                    snd_FanfareFade = 1;
                }
                sb_Play(snd_Queue->smp, 0, 0);
            }
            snd_Queue->playing = 1;
            snd_QueueTime = t;
        }
    } else if (!sb_IsPlaying(snd_Queue->smp)) {
        n = snd_Queue->next;
        sb_SetRestartC(snd_Queue->smp, 0);
        mem_Free(snd_Queue);
        snd_Queue = n;
        snd_QueueTime = t;
        if (n != 0 && n->sample != 0 && n->sample->info != 0 && snd_FanfareFade != 0
            && strstr(n->sample->info->name, "MausclickLinks")) {
            n = snd_Queue->next;
            sb_SetRestartC(snd_Queue->smp, 0);
            mem_Free(snd_Queue);
            snd_Queue = n;
            snd_FadeMusic(1.0f, 2000);
            snd_FanfareFade = 0;
        }
    }
}
