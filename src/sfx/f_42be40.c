/* 0x42be40 */

int sfx_SetSampleLoopCount(HSAMPLE s, int count)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_loop_count(s, count);
    } else
        ret = -1;
    return ret;
}
