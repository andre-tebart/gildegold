/* 0x4272f0 */

void sb_SetPan(SBSAMPLE *s, int pan)
{
    if (sb_Driver && s && pan >= 0 && pan <= 0x7f && s->pan != pan) {
        s->pan = pan;
        sfx_SetSamplePan(s->handle, pan);
    }
}
