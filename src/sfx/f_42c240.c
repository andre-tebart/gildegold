/* 0x42c240 */

int sfx_FreeStreamSlot(SFXDRIVER *d)
{
    int i;
    int k;

    i = sfx_FindDriver(d);
    if (i >= 0 && d->used < d->max)
        for (k = 0; k < d->max; k++)
            if (!sfx_Streams[i][k])
                return k;
    return -1;
}
