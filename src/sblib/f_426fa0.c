/* 0x426fa0 */

SBSAMPLE *sb_PlaySample(SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay)
{
    SBSAMPLE *s;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        s->info = sb_FindEntry(bank, name);
        if (s->info) {
            s->loops = loops;
            s->pan = pan;
            s->volume = volume;
            sb_Play(s, fade, delay);
        } else
            s = NULL;
    }
    return s;
}
