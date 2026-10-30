/* d2.c -- 0x4687a0..0x473c10.., probably D:\develop\Units\d2.v1\D2.C (memory tag "d2:d2_LoadObj", messages
   "d2_Show", "d2_OpenInputIcon()", "d2_OpenSlider"; GFX.C of the same directory follows at 0x478ee1): the 2D
   GUI layer. The picture cache (pictures are read on demand from the archive and dropped again, oldest first,
   when the memory limit is reached), the object list and the dirty rectangle lists; input fields (number /
   text) and sliders; slider and button drawing; radio groups and their keyboard navigation, picture objects and
   reference counts, drawing of all objects, hit tests, object removal, text width and text drawing. Joined in
   round 16 from unk_4687a0.c, unk_4694c0.c and unk_46aa00.c, in round 17 from unk_46c6c0.c, unk_46d3d0.c and
   unk_46e710.c, in round 18 from unk_46fba0.c (text layout of a window, window background), unk_471a90.c
   (window drawing, creation, resizing, scrolling) and unk_472e00.c (window destruction, draw order list,
   objects on windows). Names are ours.
   unit of rea 8cef7d5b (src/game/d2.c); boundaries in decl/text.dcl, notes in src/d2/open.md */
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "di_input.h"
#include "txt.h"
#include "dd_vesa.h"
#include "d2.h"
#include "unk_47c5e0.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */

#endif
#include "data_views.h"
#define d2_MoveObjI ((void (*)(int, int, int))d2_MoveObj)  /* 0x46cda0 (d2.c: short x, short y): @original this unit passes the coordinates as ints (d2_CreatePicObj) */
extern char ai_Unkfa5f04[];

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char d3s_Lights;

char d2_FormsDir[] = "forms\\";  /* 0x5fe608, "forms\\", prefix of a form file name */
unsigned int d2_PicLimit = 8000000;          /* 0x5fe70c, bytes of pictures kept in memory */
int d2_HitObjId = -1;  /* 0x5fe714 */
int d2_CurrentFont = -1;  /* 0x5fe718 */
char d2_NoBackgroundRestore = 1;  /* 0x5fe71c, 1: the alternate background path - captions are drawn without saving / restoring the background underneath (d2_DrawWindowBackground, d2_DrawObjects); it is only compared to 1, no writer is drafted */
char d2_Unk5fe71d = 2;  /* 0x5fe71d, a second 1-byte flag in the original's slot next to d2_NoBackgroundRestore: the value 2, never referenced by the code (no absolute address operand; the pad after d2_NoBackgroundRestore in our object would be zero) */
int d2_PixelFormat = 655;  /* 0x5fe720, display colour format code copied from dd_PixelFormat (888 / 565 / 555) */
int d2_SpaceWidth = 8;  /* 0x5fe724, width of a space */
int d2_LetterGap = 2;  /* 0x5fe728, gap between letters */
int d2_PointerWin = -1;  /* 0x5fe72c, window id under the pointer (d2_FindWindowAt scan) */
int d2_PointerWinVisible = -1;  /* 0x5fe730, visible window under the pointer (d2_HitTest object scan) */
int d2_ScrollMarkerPic = -1;              /* 0x5fe734 */
int d2_FramePic = -1;              /* 0x5fe738 */
int d2_DefaultFont = -1;              /* 0x5fe73c */
int d2_ButtonPulldownPic = -1;  /* 0x5fe740, "_BUTTON_PULLDOWN" picture */
int d2_WinBorderPic = -1;              /* 0x5fe744 */
int d2_WinBorderPergaPic = -1;              /* 0x5fe748 */
int d2_MiscPic = -1;  /* 0x5fe74c, _MISC picture, its frames are the 'i' command icons */
int d2_PointerPicIndex = -1;  /* 0x5fe750, "_MOUSE_CURSOR" */
int d2_ExtraSymbolsPic = -1;  /* 0x5fe754, "_EXTRA_SYMBOLS" */
int d2_TextCursorPic = -1;  /* 0x5fe758, "_TEXT_CURSOR" */
int d2_PointerMoved = 1;  /* 0x5fe75c */
int d2_ClipRight = 640;  /* 0x5fed7c, width of the screen */
int d2_ClipBottom = 480;  /* 0x5fed80, height of the screen */

int d2_LastPointerX = 0;  /* 0x633f10, last pointer x */
int d2_LastPointerY = 0;  /* 0x633f14, last pointer y */
D2FORM0 d2_Forms0[96] = {0};  /* 0x633f18 */
D2FORM2 d2_Forms2[96] = {0};  /* 0x68ce98 */
D2PIC *d2_Pics = 0;                 /* 0x6ed918 */
int d2_NPics = 0;                   /* 0x6ed91c */
int d2_PicBytes = 0;                /* 0x6ed920 */
void *d2_ScreenSurf = 0;               /* 0x6ed924 */
void *d2_FrontSurf = 0;               /* 0x6ed928 */
void *d2_BackSurf = 0;               /* 0x6ed92c, offscreen surface the layer draws into; blitted to d2_ScreenSurf */
D2SURF *d2_BackgroundTile = 0;          /* 0x6ed930 */
char d2_PicBase = 0;                /* 0x6ed934 */
int d2_LastWindow = 0;  /* 0x6ed938, window created last */
int d2_FrameCounter = 0;  /* 0x6ed940, frame counter */
D2PICHDR *d2_FontPic = 0;        /* 0x6ed948 */
int d2_HighestObj = 0;  /* 0x6ed94c, highest object index in use */
int d2_TextSegCount = 0;  /* 0x6ed954, number of text pieces */
int d2_CurrentView = 0;  /* 0x6ed958, the view a form was opened in */
D2TEX *d2_CaptionTex = 0;           /* 0x6ed960 */
D2OBJ **d2_DrawOrder = 0;          /* 0x6ed964, the objects in drawing order */
int d2_TextDrawnW = 0;  /* 0x6ed968 */
int d2_TextDrawnH = 0;  /* 0x6ed96c */
int d2_TextLeft = 0;  /* 0x6ed970, left edge of the text drawn last */
int d2_TextTop = 0;  /* 0x6ed974, top of the text drawn last */
unsigned int d2_PenSize = 0;  /* 0x6ed97c, pen size */
D2WIN * d2_LastWindowPtr = 0;  /* 0x6ed980, window created last */
int d2_FullFormPath = 0;  /* 0x6ed984, nonzero: the name given to d2_OpenForm is a full path (no d2_FormsDir prefix, no .form suffix); cleared after each load, nothing in the binary sets it */
FILE *d2_File = 0;                  /* 0x6ed988 */
int d2_PicShift = 0;                /* 0x6ed98c */
D2RECT *d2_Dirty[2] = {0};          /* 0x6ed994 */
void *d2_RedrawInfo2 = 0;           /* 0x6ed99c, third redraw info buffer ("d2:d2_RedrawInfo2") */
D2SHAPE *d2_Shapes = 0;             /* 0x6ed9a0 */
int d2_DirtyCount[2] = {0};         /* 0x6ed9a4 */
int d2_WindowsOpen = 0;  /* 0x6ed9b4, windows open */
D2WIN * d2_PendingCaptionWin = 0;  /* 0x6ed9b8, window of d2_PendingCaptionObj */
D2OBJ * d2_PendingCaptionObj = 0;  /* 0x6ed9bc, the object whose caption is drawn last
                                        (under the pointer / hit object): d2_DrawObjCaption, then 0 */
int d2_TextSegFlags = 0;  /* 0x6ed9c0, flags of the next text piece */
D2INPUT * d2_FocusedInput = 0;  /* 0x6ed9dc, the focused input */
short d2_ShpW = 0;  /* 0x6f044c */
short d2_ShpH = 0;  /* 0x6f044e */
int d2_ClipLeft = 0;  /* 0x6f0458, clip left */
int d2_ClipTop = 0;  /* 0x6f045c, clip top */
int d2_PaletteDepth = 0;  /* 0xf8a64c */
int d2_ClickedId = 0;  /* 0xf8a668 */
short d2_MouseX = 0;  /* 0xf8a674, mouse x */
short d2_MouseY = 0;  /* 0xf8a676, mouse y */
D2FRCT d2_PointerItems[6] = {0};  /* 0xf8ab80, the item list drawn next to the pointer */
D2DISSOLVE * d2_CurrentDissolve = 0;  /* 0xf8abc8 */
D2SEG d2_TextSegs[64] = {0};  /* 0xf8abe0 */
int d2_MainWindow = 0;  /* 0xf8eee0, the main window handle (HWND) given to d2_Open */
char d2_PointerTexName[128] = {0};  /* 0xf8ef00 */
D2OBJ * d2_Objs = 0;  /* 0xf8f080 */
int d2_FontHeight = 0;  /* 0xf8f084, font height */
int d2_HalfScreenH = 0;  /* 0xf8f088, half the screen height */
int d2_HalfScreenW = 0;  /* 0xf8f08c, half the screen width */
D2FADE * d2_Fades[32] = {0};  /* 0xf8f0a0, fades (d2_StartFade) */
D2WIN d2_Wins[0x60] = {0};  /* 0xf8f120, 96 windows: 0xfa5aa0 is d2_Radios */
D2RADIO d2_Radios[8] = {0};  /* 0xfa5aa0 */
/* d2_PointerX: view of ai_Unkfa5f04 (tools/data_views.py) */
int d2_PointerY = 0;  /* 0xfa5f0c, pointer y */
char d2_GfxPath[256] = {0};  /* 0xfa5f20 */
D2INPUT d2_Inputs[0x80] = {0};  /* 0xfa6020 */
D2VIEW d2_Views[0x30] = {0};  /* 0xfb1020 */
short d2_ScreenH = 0;  /* 0xfb9070 */
short d2_ScreenW = 0;  /* 0xfb9072 */
/* 0x143a1c0, the text pointer table: 0x4800 slots, the one block the text loader fills;
   d2_Strings (the first 122) and every named text pointer are views of it (txt_views.h) */
char *txt_Ptrs[0x4800] = {0};

