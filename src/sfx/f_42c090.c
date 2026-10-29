/* 0x42c090 */

int sfx_SetStreamLoopCount(HSTREAM s, int count)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_loop_count(s, count);
    } else
        ret = -1;
    return ret;
}
