/* 0x426f20, nonzero when sample s is playing (its handle's status is 4); 1 when the driver is closed and
   0 for an empty or finished slot */

int sb_IsPlaying(SBSAMPLE *s)
{
    int ret = 0;

    if (!sb_Driver)
        return 1;
    if (s && s->handle && sfx_SampleStatus(s->handle) == 4)
        ret = -1;
    return ret;
}
