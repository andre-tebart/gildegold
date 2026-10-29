/* 0x42beb0 */

int sfx_OpenStream(SFXDRIVER *d, char *file, HSTREAM *out)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            k = sfx_FreeStreamSlot(d);
            if (k >= 0) {
                *out = AIL_open_stream(d->drv, file, ret);
                if (*out) {
                    sfx_Streams[i][k] = *out;
                    d->used++;
                } else {
                    AIL_last_error();
                    ret = -1;
                }
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
