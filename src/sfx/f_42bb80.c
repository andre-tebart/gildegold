/* 0x42bb80 */

int sfx_AllocSample(SFXDRIVER *d, HSAMPLE *out)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            k = sfx_FreeSampleSlot(d);
            if (k >= 0) {
                *out = AIL_allocate_sample_handle(d->drv);
                if (*out) {
                    d->used++;
                    sfx_Samples[i][k] = *out;
                } else
                    ret = -1;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
