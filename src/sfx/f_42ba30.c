/* 0x42ba30 */

int sfx_Release(SFXDRIVER *d)
{
    int ret = 0;

    if (!sfx_Initialized || sfx_FindDriver(d) < 0 || !AIL_digital_handle_release(d->drv))
        ret = -1;
    return ret;
}
