/* 0x42c110 */

int sfx_StreamStatus(HSTREAM s)
{
    if (sfx_Initialized && sfx_FindStream(s) >= 0)
        return AIL_stream_status(s);
    return -1;
}
