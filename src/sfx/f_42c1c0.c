/* 0x42c1c0 */

int sfx_FreeDriverSlot(void)
{
    int i;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (!sfx_Drivers[i])
            return i;
    return -1;
}
