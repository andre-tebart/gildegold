/* 0x42be80 */

int sfx_SampleStatus(HSAMPLE s)
{
    int ret = -1;

    if (sfx_Initialized && sfx_FindSample(s) >= 0)
        ret = AIL_sample_status(s);
    return ret;
}
