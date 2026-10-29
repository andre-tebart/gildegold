/* 0x42bab0 */

void sfx_Reacquire(SFXDRIVER *d, void *hwnd, unsigned int msg)
{
    if (sfx_Initialized && msg >= WM_USER && sfx_FindDriver(d) >= 0)
        if (!AIL_digital_handle_reacquire(d->drv))
            PostMessageA(hwnd, msg, (WPARAM)d, 0);
}
