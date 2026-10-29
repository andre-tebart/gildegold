/* 0x42bfe0 */

int sfx_StartStream(HSTREAM s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_start_stream(s);
    } else
        ret = -1;
    return ret;
}
