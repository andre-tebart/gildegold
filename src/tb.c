/* tb.c -- original file name unknown: the time base ("tb_OpenFunction" string).
   Extent in GildeGold.exe (inferred): 0x42e7b0 (after txt_FindKey, the end of txt.c) up to
   0x42ea81; its .data is the one string at 0x5f8fa4. Functions are kept in their original order.
   Names are ours except tb_OpenFunction.
   unit of rea 8cef7d5b (src/game/tb.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "tb.h"
#include "err.h"

/* 0x42e7b0, the multimedia timer callback. */
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
/* 0x42e940, kills the timer event (if it still exists) and ends the timer period it raised. */

void tb_Exit(void)
{
    if (tb_Timer && timeKillEvent(tb_Timer) == TIMERR_NOERROR)
        tb_Timer = 0;
    timeEndPeriod(tb_Caps.wPeriodMin + 5);
}
/* 0x42e970, registers a function called every `period` ticks. */

int tb_OpenFunction(TBPROC proc, unsigned long period)
{
    int i = 0;

    while (tb_Funcs[i++].proc && i < 32)
        ;
    if (i > 32) {
        err_Message("tb_OpenFunction: Too many TimeBaseProcs!");
        return 0;
    }
    i--;
    tb_Funcs[i].proc = proc;
    tb_Funcs[i].period = period;
    tb_Funcs[i].off = 0;
    return 1;
}
/* 0x42e9d0, unregisters proc from the timer; 1 when it was registered, else 0. */

int tb_CloseFunction(TBPROC proc)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc) {
            tb_Funcs[i].proc = NULL;
            return 1;
        }
    return 0;
}
/* 0x42ea10, switches the timer callback of proc off (off != 0) or on; 1 when proc is registered. */

int tb_SetFunctionOff(TBPROC proc, int off)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc) {
            tb_Funcs[i].off = off;
            return 1;
        }
    return 0;
}
/* 0x42ea50, tells whether proc is switched off in the timer; 0 for an unregistered proc. */

int tb_IsFunctionOff(TBPROC proc)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc)
            return tb_Funcs[i].off != 0;
    return 0;
}

extern TIMECAPS tb_Caps;
extern UINT tb_Delay;
extern TBFUNC tb_Funcs[32];
MMRESULT tb_Timer = {0};  /* 0x6333c8 */
volatile unsigned long d3_Time = 0;  /* 0x6333cc, the millisecond counter tb_TimerProc advances */
volatile int tb_AnimTick = {0};  /* 0x6333d0, counted once per 3 ms; gfx/shp step animations by it */
volatile int tb_Unk6333d4 = {0};  /* 0x6333d4, incremented when d3_Time is odd */
volatile int tb_Calls = {0};  /* 0x6333d8, timer callbacks */
int tb_Busy = 0;  /* 0x6333dc */
int tb_Paused = 0;  /* 0x6333e0 */
int tb_OneShot = 0;  /* 0x6333e4, the callback restarts the timer */
char tb_Slow = 0;  /* 0x6333e8, d3_Time advances every 20th callback */
int tb_SlowCount = 0;  /* 0x6333ec */
TIMECAPS tb_Caps = {0};  /* 0xfe7a80 */
UINT tb_Delay = 0;  /* 0xfe7a88 */
TBFUNC tb_Funcs[32] = {0};  /* 0xfe7aa0 */
