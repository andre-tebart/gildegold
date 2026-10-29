/* 0x42c380 */

int sfx_NumDrivers(void)
{
    int i;
    int n = 0;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            n++;
    return n;
}
