/* di_input.c -- original file name unknown: registry helpers ("Software\Ahead Entertainment\"), the
   keyboard scan code map and DirectInput (mouse device 0x1467360, keyboard device 0x14673cc). Starts at
   0x4103a0 (after the sound command registration, sndc.c) and ends at 0x411680 (d3_tools.c).
   Names are ours. */
#ifndef UNK_4103A0_H
#define UNK_4103A0_H

#include <windows.h>
#include <dinput.h>

/* DirectInput mouse state, 0x4c bytes: the working state dm_S (0x14673e0), the latched state dm_L
   (0x1467380), the previous state dm_P (0x14683c0) and the 32-entry event queue dm_Q (0x1467440). */
typedef struct {
    union {
        struct {
            short x, y, z;                      /* +0x00 */
        };
        struct {
            volatile short vx, vy, vz;          /* +0x00: volatile view: di_UpdateButtons must reload the
                                                   position at every use instead of keeping it in a
                                                   register (the original reloads dm_S.x / dm_S.y) */
        };
    };
    short dx, dy, dz;                           /* +0x06 */
    int lup, ldown, lhold, lclick, lextra;      /* +0x0c: left button */
    int rup, rdown, rhold, rclick;              /* +0x20: right button */
    int mup, mdown, mhold, mclick;              /* +0x30: middle button */
    int wdown, wup;                             /* +0x40: wheel */
    int valid;                                  /* +0x48: queue entry in use */
} DIMOUSEST;

extern int kb_Base;                     /* 0x5f3234, initial 0x200 (mouse speed) */
extern int di_SwapLR;                   /* 0x6310c4, swap the left / right buttons; cleared on every option apply */
extern unsigned short kb_Map[256];      /* 0x1467dc0 */
extern DIMOUSEST dm_L;                  /* 0x1467380 */
extern DIMOUSEST dm_S;                  /* 0x14673e0 */
extern DIMOUSEST dm_P;                  /* 0x14683c0 */
extern DIMOUSEST dm_Q[32];              /* 0x1467440 */
extern unsigned char kb_Down[256];      /* 0x14681c0 */
extern unsigned char kb_Pressed[256];   /* 0x1468420 */
extern volatile int tb_Calls;           /* 0x6333d8, timer callbacks */

HKEY reg_OpenKey(char *name, int mode);                                 /* 0x4103a0, mode 1 read, 2 create; -1 on failure */
void reg_CloseKey(HKEY key);                                            /* 0x410430 */
int reg_GetString(HKEY key, char *name, char *out);                     /* 0x410440, out needs 128 bytes */
void reg_SetString(HKEY key, char *name, char *value);                  /* 0x4104b0 */
int kb_BuildMap(void);                                                  /* 0x4104e0 */
unsigned char kb_FindKey(int shift, int ctrl, int alt, char code);                /* 0x410560 */
void kb_SetBase(int v);                                                 /* 0x410610 */
void di_ClearButtons(void);                                             /* 0x410620 */
void di_ClearAxes(void);                                                /* 0x410690 */
void di_Error(int unk, int hr, char *fmt, ...);                         /* 0x4106d0 */
int di_AcquireMouse(int on);                                            /* 0x410710 */
int di_AcquireKeyboard(int on);                                         /* 0x410740 */
int di_Init(HWND hwnd, HINSTANCE hinst, unsigned int flags);            /* 0x410770, flags 1 kbd exclusive, 2 mouse background, 4 cursor */
void di_Exit(void);                                                     /* 0x410a50 */
void di_UpdateButtons(void);                                            /* 0x410ac0 */
void di_SwapButtons(void);                                              /* 0x410ed0 */
void di_PollMouse(void);                                                /* 0x410f20 */
void di_PollKeyboard(void);                                             /* 0x4112e0 */
void di_SetMousePos(int x, int y);                                      /* 0x4113f0 */
void di_GetMouse(void);                                                 /* 0x411440 */
void di_ClearEvents(unsigned char mask);                                /* 0x4115b0 */

#endif
