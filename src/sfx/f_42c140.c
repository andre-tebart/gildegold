/* 0x42c140 */

int sfx_GetStreamPos(HSTREAM s, int *ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_stream_ms_position(s, 0, ms);
    } else
        ret = -1;
    return ret;
}
