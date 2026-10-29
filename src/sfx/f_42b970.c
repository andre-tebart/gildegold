/* 0x42b970 */

int sfx_CloseDigOutput(SFXDRIVER *d)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            AIL_waveOutClose(d->drv);
            if (sfx_Samples[i]) {
                mem_Free(sfx_Samples[i]);
                sfx_Samples[i] = 0;
            }
            if (sfx_Streams[i]) {
                mem_Free(sfx_Streams[i]);
                sfx_Streams[i] = 0;
            }
            mem_Free(d);
            sfx_Drivers[i] = 0;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
