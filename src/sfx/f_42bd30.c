/* 0x42bd30 */

int sfx_StartSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_start_sample(s);
    } else
        ret = -1;
    return ret;
}
