/* 0x42b9f0 */

int sfx_CloseAll(void)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                if (sfx_CloseDigOutput(sfx_Drivers[i]) == -1)
                    ret = -1;
    } else
        ret = -1;
    return ret;
}
