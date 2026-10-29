/* 0x42e8c0, starts the timer with the given delay in milliseconds. */

int tb_Init(UINT delay, int oneshot)
{
    timeGetDevCaps(&tb_Caps, sizeof(TIMECAPS));
    timeBeginPeriod(tb_Caps.wPeriodMin + 5);
    if (delay < tb_Caps.wPeriodMin)
        return 0;
    tb_Delay = delay;
    tb_OneShot = oneshot;
    if (oneshot)
        tb_Timer = timeSetEvent(delay, tb_Caps.wPeriodMin + 5, tb_TimerProc, 0, TIME_ONESHOT);
    else
        tb_Timer = timeSetEvent(delay, tb_Caps.wPeriodMin + 5, tb_TimerProc, 0, TIME_PERIODIC);
    if (!tb_Timer)
        return 0;
    srand(timeGetTime());
    return 1;
}
