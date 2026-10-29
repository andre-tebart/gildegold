/* 0x4bdac0, clears the radio group rt_MarkGroup */

void rt_ClearMarkGroup(void)
{
    if (rt_MarkGroup != -1)
        d2_RadioClear(rt_MarkGroup);
    rt_MarkGroup = -1;
}
