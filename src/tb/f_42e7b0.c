/* 0x42e7b0, the multimedia timer callback. @deep Q2: tb_Unk6333d4 stays unnamed - it is write-only in the whole exe (one store site 0x42e837/0x42e83d, no reader anywhere, no string or text id); the role is unclear, no name without inventing one. tb_Unk6333d0 became tb_AnimTick (gfx/shp step animations by it). behaviour byte */
void CALLBACK tb_TimerProc(UINT id, UINT msg, DWORD user, DWORD dw1, DWORD dw2)
{
    int i;

    if (tb_Busy)
        return;
    tb_Busy = 1;
    tb_Calls++;
    if (!tb_Paused) {
        for (i = 0; i < 32; i++)
            if (tb_Funcs[i].proc && d3_Time % tb_Funcs[i].period == 0 && !tb_Funcs[i].off)
                tb_Funcs[i].proc();
        if (d3_Time % 3 == 0)
            tb_AnimTick++;
        if (d3_Time & 1)
            tb_Unk6333d4++;
        if (tb_Slow) {
            if (++tb_SlowCount % 20 == 0)
                d3_Time++;
        } else
            d3_Time++;
    }
    if (tb_OneShot)
        tb_Timer = timeSetEvent(tb_Delay, tb_Caps.wPeriodMin + 5, tb_TimerProc, 0, TIME_ONESHOT);
    tb_Busy = 0;
}
