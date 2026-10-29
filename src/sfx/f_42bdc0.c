/* 0x42bdc0 */

int sfx_SetSampleVolume(HSAMPLE s, int vol)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_volume(s, vol);
    } else
        ret = -1;
    return ret;
}
