/* di_input.c -- original file name unknown: see include/di_input.h. Names are ours.
   unit of rea 8cef7d5b (src/game/di_input.c); boundaries in decl/text.dcl, notes in src/di_input/open.md */
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <dinput.h>
#include "di_input.h"

int kb_Base = 0x200;                    /* 0x5f3234 */
int di_MaxX = 790;                      /* 0x5f3238 mouse limits (as in the exe image) */
int di_MaxY = 599;                      /* 0x5f323c */
int di_Absolute = 1;                    /* 0x5f3240 mouse position from GetCursorPos */
unsigned short kb_Map[256];             /* 0x1467dc0 */
unsigned char kb_Down[256];             /* 0x14681c0 */
unsigned char kb_Pressed[256];          /* 0x1468420 */
DIMOUSEST dm_L;                         /* 0x1467380 */
DIMOUSEST dm_S;                         /* 0x14673e0 */
DIMOUSEST dm_P;                         /* 0x14683c0 */
DIMOUSEST dm_Q[32];                     /* 0x1467440 */
LPDIRECTINPUT di_Main;                  /* 0x1468414 */
LPDIRECTINPUTDEVICE di_Mouse;           /* 0x1467360 */
LPDIRECTINPUTDEVICE di_Keyboard;        /* 0x14673cc */
HANDLE di_Event;                        /* 0x146840c */
HWND di_Hwnd;                           /* 0x146735c */
HINSTANCE di_Hinst;                     /* 0x1467364 */
unsigned int kb_RepeatTime = 0;         /* 0x631098 */
int di_MinX = 0;                        /* 0x6310b0 */
int di_MinY = 0;                        /* 0x6310b4 */
unsigned int di_Flags = 0;              /* 0x6310b8 */
int di_GuiClick = 0;                    /* 0x6310bc, set by the GUI (gfx.c) when a left click hit a control: the button release is then swallowed */
int di_Disabled = 0;                    /* 0x6310c0 */
int di_SwapLR = 0;                      /* 0x6310c4 */
int di_LDownTime = 0;                   /* 0x6310c8 */
int di_RDownTime = 0;                   /* 0x6310cc */
int di_MDownTime = 0;                   /* 0x6310d0 */
int di_PressX = 0;                      /* 0x6310d4 */
int di_PressY = 0;                      /* 0x6310d8 */
int di_LClickTime = 0;                  /* 0x6310dc (.bss: 0 at start) */
union { int i; unsigned char b; } kb_RepeatKey = {0};     /* 0x6310e0 */
unsigned char kb_Last;                  /* 0x1468410 */

