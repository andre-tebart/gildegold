/* 0x42c2c0 */

int sfx_FindStream(HSTREAM s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Streams[i][k] == s)
                    return k;
    return -1;
}
