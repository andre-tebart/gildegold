/* 0x40b5a0, script command GetTime: the running time in milliseconds (d3_Time ticks * tb_Delay) */

int evtc_GetTime(void)
{
    return d3_Time * tb_Delay;
}
