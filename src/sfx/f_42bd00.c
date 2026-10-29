/* 0x42bd00 */

int sfx_InitSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_init_sample(s);
    } else
        ret = -1;
    return ret;
}
