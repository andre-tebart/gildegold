/* 0x42ba70 */

int sfx_ReleaseAll(void)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                if (sfx_Release(sfx_Drivers[i]))
                    ret = -1;
    } else
        ret = -1;
    return ret;
}
