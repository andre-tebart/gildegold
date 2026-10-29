/* 0x46bdd0, sets the slider's flags (u.s.flags) */

void d2_SetSliderFlags(int id, int flags)
{
    d2_Objs[id].u.s.flags = flags;
}
