/* 0x42bb00 */

void sfx_ReacquireAll(void *hwnd, unsigned int msg)
{
    int i;

    if (sfx_Initialized)
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                sfx_Reacquire(sfx_Drivers[i], hwnd, msg);
}
