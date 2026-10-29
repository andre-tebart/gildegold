/* 0x410040, script command PlaySample: sets the loop count of the named sample of the default sample bank
   and plays it, only while the running script is in no group; returns the sample, 0 when it was not started
   (the lookup is repeated in the original's null path) */

struct SBSAMPLE *sndc_PlaySample(char **name, int *loops)
{
    struct SBSAMPLE *s;

    if (evt_Cur->group != evt_NoGroup)
        return 0;
    s = sb_GetSample(0, *name);
    if (s) {
        sb_SetLoops(s, *loops);
        sb_Play(s, 0, 0);
    } else
        sb_GetSample(0, *name);
    return s;
}
