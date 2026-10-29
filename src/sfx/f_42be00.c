/* 0x42be00 */

int sfx_SetSamplePan(HSAMPLE s, int pan)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_pan(s, pan);
    } else
        ret = -1;
    return ret;
}
