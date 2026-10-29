/* 0x42c200 */

int sfx_FreeSampleSlot(SFXDRIVER *d)
{
    int i;
    int k;

    i = sfx_FindDriver(d);
    if (i >= 0 && d->used < d->max)
        for (k = 0; k < d->max; k++)
            if (!sfx_Samples[i][k])
                return k;
    return -1;
}
