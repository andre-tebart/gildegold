
extern TIMECAPS tb_Caps;
extern UINT tb_Delay;
extern TBFUNC tb_Funcs[32];
MMRESULT tb_Timer = {0};  /* 0x6333c8 */
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
