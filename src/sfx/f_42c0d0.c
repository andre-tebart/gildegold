/* 0x42c0d0 */

int sfx_SetStreamPos(HSTREAM s, int ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_ms_position(s, ms);
    } else
        ret = -1;
    return ret;
}
