/* tb.c -- original file name unknown: the time base ("tb_OpenFunction: Too many TimeBaseProcs!").
   A multimedia timer increments the millisecond counter d3_Time and calls registered functions every
   `period` ticks. Names are ours except tb_OpenFunction. */
#ifndef UNK_42E7B0_H
#define UNK_42E7B0_H

#include <windows.h>
#include <mmsystem.h>

typedef void (*TBPROC)(void);

typedef struct TBFUNC {                 /* 12 bytes */
    TBPROC proc;
    unsigned long period;               /* called when d3_Time % period == 0 */
    int off;                            /* nonzero: not called */
} TBFUNC;

extern MMRESULT tb_Timer;               /* 0x6333c8 */
extern volatile unsigned long d3_Time;  /* 0x6333cc, also in d3.h */
extern volatile int tb_AnimTick;        /* 0x6333d0, counted once per 3 ms; gfx/shp step animations by it */
extern volatile int tb_Unk6333d4;               /* 0x6333d4, incremented when d3_Time is odd */
extern volatile int tb_Calls;                 /* 0x6333d8, timer callbacks */
extern int tb_Busy;                     /* 0x6333dc */
extern int tb_Paused;                   /* 0x6333e0 */
extern int tb_OneShot;                  /* 0x6333e4, the callback restarts the timer */
extern char tb_Slow;                    /* 0x6333e8, d3_Time advances every 20th callback */
extern int tb_SlowCount;                /* 0x6333ec */
extern TIMECAPS tb_Caps;                /* 0xfe7a80 */
extern UINT tb_Delay;                   /* 0xfe7a88 */
extern TBFUNC tb_Funcs[32];             /* 0xfe7aa0 */

void err_Message(char *text);                                                           /* 0x4222b0 */

void CALLBACK tb_TimerProc(UINT id, UINT msg, DWORD user, DWORD dw1, DWORD dw2);       /* 0x42e7b0 */
int tb_Init(UINT delay, int oneshot);                                                   /* 0x42e8c0 */
void tb_Exit(void);                                                                     /* 0x42e940 */
int tb_OpenFunction(TBPROC proc, unsigned long period);                                 /* 0x42e970 */
int tb_CloseFunction(TBPROC proc);                                                      /* 0x42e9d0 */
int tb_SetFunctionOff(TBPROC proc, int off);                                            /* 0x42ea10 */
int tb_IsFunctionOff(TBPROC proc);                                                      /* 0x42ea50 */

#endif
