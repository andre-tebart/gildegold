/* 0x42c050 */

int sfx_SetStreamVolume(HSTREAM s, int vol)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_volume(s, vol);
    } else
        ret = -1;
    return ret;
}
