/* 0x42b780 */

int sfx_SetMasterVolume(SFXDRIVER *d, int vol)
{
    int ret = 0;

    if (sfx_FindDriver(d) >= 0 && vol >= 0 && vol <= 127) {
        AIL_set_digital_master_volume(d->drv, vol);
    } else
        ret = -1;
    return ret;
}
