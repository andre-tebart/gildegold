/* 0x42bca0 */

int sfx_ReleaseAllSamples(void)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                for (k = 0; k < sfx_Drivers[i]->max; k++)
                    if (sfx_Samples[i][k])
                        if (sfx_ReleaseSample(sfx_Samples[i][k]) == -1)
                            ret = -1;
    } else
        ret = -1;
    return ret;
}
