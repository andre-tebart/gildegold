/* 0x427370, sets the master volume of the sound driver; ignored when the library is not initialized */

void sb_SetMasterVolume(int volume)
{
    if (sb_Driver)
        sfx_SetMasterVolume(sb_Driver, volume);
}
