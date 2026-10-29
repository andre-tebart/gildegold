/* 0x426030 */

SBSAMPLE *sb_GetSample(SBBANK *bank, char *name)
{
    SBSAMPLE *s;

    if (sb_Driver) {
        s = sb_AllocSample();
        if (s) {
            s->info = sb_FindEntry(bank, name);
            if (s->info) {
                s->done = NULL;
                s->loops = 1;
                s->pan = abs(0x7f) / 2;
                s->volume = 0x7f;
                sfx_SetSampleVolume(s->handle, 0x7f);
                sfx_SetSamplePan(s->handle, s->pan);
                sfx_SetSampleLoopCount(s->handle, s->loops);
            } else
                s = NULL;
        }
        return s;
    }
    return &sb_Dummy;
}
