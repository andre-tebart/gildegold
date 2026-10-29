/* 0x42e940, kills the timer event (if it still exists) and ends the timer period it raised. */

void tb_Exit(void)
{
    if (tb_Timer && timeKillEvent(tb_Timer) == TIMERR_NOERROR)
        tb_Timer = 0;
    timeEndPeriod(tb_Caps.wPeriodMin + 5);
}
