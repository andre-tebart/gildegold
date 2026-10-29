/* 0x42c180 */

int sfx_GetStreamLength(HSTREAM s, int *ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_stream_ms_position(s, ms, 0);
    } else
        ret = -1;
    return ret;
}
