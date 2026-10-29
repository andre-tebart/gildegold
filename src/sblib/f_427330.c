/* 0x427330, sets the sample volume (0..0x7f) and passes it to the driver when it differs; values out of
   range, a closed driver and a NULL sample are ignored */

void sb_SetVolume(SBSAMPLE *s, int volume)
{
    if (sb_Driver && s && volume >= 0 && volume <= 0x7f && s->volume != volume) {
        s->volume = volume;
        sfx_SetSampleVolume(s->handle, volume);
    }
}
