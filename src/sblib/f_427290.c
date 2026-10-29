/* 0x427290, stops sample s: with fade, sets the fade-out flag and the fade start (sb_Update ramps the
   volume down); without, stops the sample at the driver now and sets the stopped flag. Does nothing when
   the driver is closed or s is NULL */

void sb_Stop(SBSAMPLE *s, int fade)
{
    if (sb_Driver && s) {
        if (fade) {
            s->flags |= 2;
            s->fadestart = SB_TIME;
        } else {
            sfx_EndSample(s->handle);
            s->flags |= 8;
        }
    }
}
