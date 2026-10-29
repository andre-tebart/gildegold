/* 0x42bf80 */

int sfx_CloseAllStreams(void)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                for (k = 0; k < sfx_Drivers[i]->max; k++)
                    if (sfx_Streams[i][k])
                        if (sfx_CloseStream(sfx_Streams[i][k]))
                            ret = -1;
    } else
        ret = -1;
    return ret;
}
