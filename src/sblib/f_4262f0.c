/* 0x4262f0, the first sample slot from index *idx on that plays entry e (*idx is updated to it);
   returns NULL when no slot does, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_FindSampleOf(SBENTRY *e, int *idx)
{
    SBSAMPLE *samples;
    SBSAMPLE *s = NULL;
    int i;

    if (!sb_Driver)
        return &sb_Dummy;
    samples = sb_Samples;
    if (samples) {
        for (i = *idx; i < sb_Count; i++) {
            if (samples[i].info == e) {
                *idx = i;
                s = &samples[i];
                break;
            }
        }
    }
    return s;
}
