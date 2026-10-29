/* 0x42c300 */

int sfx_SampleDriver(HSAMPLE s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Samples[i][k] == s)
                    return i;
    return -1;
}
