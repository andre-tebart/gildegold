/* 0x40af50, action callback 0x3b: waits args[0] milliseconds */

void ch_ActWait(CHACTION *a)
{
    if (!a->count)
        a->args[1] = d3_Time;
    if (d3_Time > (unsigned long)(a->args[1] + a->args[0]) || a->stop)
        ch_RemoveAction(a);
}
