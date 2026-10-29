/* 0x42c010 */

int sfx_PauseStream(HSTREAM s, int pause)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_pause_stream(s, pause);
    } else
        ret = -1;
    return ret;
}
