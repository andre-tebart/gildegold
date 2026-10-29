/* 0x426540, the sample slot to reuse: the first idle slot, then the one with the lowest start time among
   the idle slots; ends its sample and resets flags / delay / loops. Returns NULL when there is no slot
   array, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_AllocSample(void)
{
    SBSAMPLE *s = NULL;
    int i;

    if (!sb_Driver)
        return &sb_Dummy;
    if (sb_Samples) {
        for (i = 0; i < sb_Count; i++) {
            if (sfx_SampleStatus(sb_Samples[i].handle) != 4 && !(sb_Samples[i].flags & 0x10)) {
                s = &sb_Samples[i++];
                break;
            }
        }
        if (s) {
            for (; i < sb_Count; i++)
                if (sb_Samples[i].start < s->start && sfx_SampleStatus(sb_Samples[i].handle) != 4
                    && !(sb_Samples[i].flags & 0x10))
                    s = &sb_Samples[i];
            sfx_InitSample(s->handle);
            s->flags = 0;
            s->delay = 0;
            s->loops = 1;
        }
    }
    return s;
}
