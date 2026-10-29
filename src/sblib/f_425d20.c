/* 0x425d20, per-frame update of all samples: fades in / out (sb_FadeTime), repeat delays with loop counts,
   restart bookkeeping and the done callback; while sb_Stopping is set it fades everything out over
   sb_StopTime and clears the flag once no sample plays any more */

void sb_Update(void)
{
    int i, done;
    unsigned int now, el, fl;
    SBSAMPLE *s;

    if (!sb_Driver)
        return;
    if (!sb_Stopping) {
        if (sb_FadeTime) {
            now = SB_TIME;
            for (i = 0; i < sb_Count; i++) {
                s = &sb_Samples[i];
                if (!s->info)
                    continue;
                if (s->flags & 0x10) {
                    s->info->used = now;
                    s->start = now;
                }
                if (sfx_SampleStatus(s->handle) == 4) {
                    if (s->flags & 3) {
                        if (now - s->fadestart < sb_FadeTime) {
                            if (s->flags & 1)
                                sfx_SetSampleVolume(s->handle, (int)((float)(now - s->fadestart) / sb_FadeTime * s->volume));
                            else
                                sfx_SetSampleVolume(s->handle, (int)((float)(s->fadestart - now + sb_FadeTime) / sb_FadeTime * s->volume));
                        } else if (s->flags & 2) {
                            sfx_EndSample(s->handle);
                            /* the original stores the flags twice; the volatile store keeps VC6 from merging them */
                            fl = s->flags | 8;
                            s->volatileFlags = fl;
                            s->fadestart = 0;
                            s->flags = fl & ~3;
                        } else {
                            sfx_SetSampleVolume(s->handle, s->volume);
                            s->flags &= ~3;
                            s->fadestart = 0;
                        }
                    }
                } else if (s->delay == 0 || s->loops == 1 || (s->flags & 0xe)) {
                    if (s->done) {
                        s->done(s);
                        s->done = NULL;
                    }
                } else if (s->last) {
                    if (s->last + s->delay < SB_TIME) {
                        if (s->loops)
                            s->loops--;
                        sb_Play(s, 0, s->delay);
                    }
                } else
                    s->last = SB_TIME;
            }
        }
    } else {
        now = SB_TIME;
        done = 1;
        for (i = 0; i < sb_Count; i++) {
            s = &sb_Samples[i];
            if (s->info && sfx_SampleStatus(s->handle) == 4) {
                done = 0;
                el = SB_TIME;
                el -= s->fadestart;
                if (el < sb_StopTime)
                    sfx_SetSampleVolume(s->handle, (int)((float)el / sb_StopTime * s->volume));
                else
                    sb_Stop(s, 0);
            }
        }
        sb_Stopping = done == 0;
    }
}
