/* 0x42c1e0 */

int sfx_FindDriver(SFXDRIVER *d)
{
    int i;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i] == d)
            return i;
    return -1;
}
