/* 0x4bdae0, puts the marks into the radio group rt_MarkGroup */

void rt_BuildMarkGroup(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1) {
            if (rt_MarkGroup == -1)
                rt_MarkGroup = d2_CreateRadioGroup(1, m->obj);
            else
                d2_RadioAdd(rt_MarkGroup, m->obj);
        }
    }
}
