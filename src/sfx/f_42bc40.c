/* 0x42bc40 */

int sfx_ReleaseSample(HSAMPLE s)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_SampleDriver(s);
        if (i >= 0) {
            k = sfx_FindSample(s);
            if (k >= 0) {
                AIL_release_sample_handle(s);
                sfx_Samples[i][k] = 0;
                sfx_Drivers[i]->used--;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
