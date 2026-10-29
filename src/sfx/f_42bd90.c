/* 0x42bd90 */

int sfx_EndSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_end_sample(s);
    } else
        ret = -1;
    return ret;
}
