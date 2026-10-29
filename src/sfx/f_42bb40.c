/* 0x42bb40 */

int sfx_SetNumSamples(int n)
{
    int ret = 0;

    if (sfx_Initialized && sfx_NumDrivers() <= 1) {
        sfx_NumSamples = n;
        AIL_set_preference(1, n);
    } else
        ret = -1;
    return ret;
}
