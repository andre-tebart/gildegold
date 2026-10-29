/* 0x42bf20 */

int sfx_CloseStream(HSTREAM s)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_StreamDriver(s);
        if (i >= 0) {
            k = sfx_FindStream(s);
            if (k >= 0) {
                AIL_close_stream(s);
                sfx_Drivers[i]->used--;
                sfx_Streams[i][k] = 0;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
