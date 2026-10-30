/* unk_5d33b0.c -- the screen / front-end module (0x5d33b0 .. 0x5dd3e0, the end of the game code; the
   0x5dd3e0 .. 0x5de450 tail is data): the
   screen file list 0x5d33b0 (the file extension pick over vfs_FindDir (0x42f7e0, vfs_Root 0x6333fc), the
   entries "%s_%s" 0x62d7d8), the screen windows 0x5d34b0 .., the graphics options writer 0x5d4c70 (the
   INI keys texture_scale .. brightness of the profile 0x75e480), the loading screen 0x5d7320 and the
   game start / frame callbacks 0x5d7530 ..
   Unit start (layout 2026-10-24 session 13): 0x5d33b0 is the first function of the screen-file family;
   its first literal "%s/%s" 0x62d7d8 follows the previous unit's last literal "special_uni" 0x62d6bc
   (+0x11c, data order D8; no literal belongs to the trial / action callbacks 0x5d0f20 .. 0x5d3270), and
   the subject changes from the action records of the table 0x62c420 to the screen definitions.
   (start: the literal order 0x62d7d8 after the previous unit's "special_uni" 0x62d6bc and the subject
   change; end: 0x5dd3e0, the end of the game code: the last function is 0x5dd100 (ret at 0x5dd3d6, nop
   fill to 0x5dd3e0); dinput.lib's c_dfDIKeyboard / c_dfDIMouse object tables follow, library data,
   see config/symbols/game_range.toml)
   unit of rea 8cef7d5b (src/game/unk_5d33b0.c); boundaries in decl/text.dcl, notes in src/unk_5d33b0/open.md */
extern char gm_PlayerRecords[];
#include "txt_views.h"
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
#include "vfs.h"
#include "di_input.h"
#include "f3snd.h"
#include "tb.h"
#include "d3.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "surf.h"
#include "unk_5d33b0.h"
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.c: int view): @original the callers pass the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "data_views.h"
#include "time_view.h"
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define sv_CheckGoodAmountB ((int (*)(char *, int, unsigned char))sv_CheckGoodAmount)  /* 0x521040 (sv.c: unsigned t): @original ai_CurseWindow (0x5d8c90) passes the good as a byte (mov cl, [ai_OwnPlayer], no zero extension) */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */

typedef struct D2OBJVIEW {                  /* 0x2ec, d2.h's D2OBJ: only the fields this unit reads */
    char pad0[0x10];
    short x;                                /* +0x10, D2OBJ.x */
    short y;                                /* +0x12, D2OBJ.y */
    short w;                                /* +0x14, D2OBJ.w (the frame width) */
    char pad16[0x12];
    int checked;                            /* +0x28, D2OBJ.checked: the checkbox / radio state */
    int highlighted;                        /* +0x2c, D2OBJ.highlighted: drawn in the highlight colour */
    char *win;                              /* +0x30, D2OBJ.win: the window the object belongs to */
    char pad34[0x14];
    int checkable;                          /* +0x48, D2OBJ.checkable: a checkbox / radio */
    int interactive;                        /* +0x4c, D2OBJ.interactive */
    int greyed;                             /* +0x50, D2OBJ.greyed: drawn in the inactive colour */
    char pad54[8];
    int centred;                           /* +0x5c, D2OBJ.centred */
    char pad60[0x18];
    int input;                              /* +0x78, D2OBJ.input */
    char pad7c[0x144];
    int style;                              /* +0x1c0, D2OBJ.style */
    int picW;                               /* +0x1c4, D2OBJ.picW */
    char pad1c8[4];
    int captionX;                           /* +0x1cc, D2OBJ.captionX */
    int captionY;                           /* +0x1d0, D2OBJ.captionY */
    char pad1d4[0x114];
    int owner;                              /* +0x2e8, D2OBJ.pad2e8: unused by d2; the gypsy windows keep the row's record here */
} D2OBJVIEW;

typedef struct D2WINVIEW {
    char pad00[0x08];
    short w;                            /* +0x08, the window width */
    short h;                            /* +0x0a, the window height */
    char pad0c[0x08];
    short mt;                           /* +0x14, the top text margin */
    char pad16[0x22e];
    int contentHeight;                  /* +0x244, height of the content */
    int scrollY;                        /* +0x248, scroll offset y */
    char pad24c[4];
    int scrollRemY;                     /* +0x250, scroll y still to go */
    char pad254[0x18];
    int parent;                         /* +0x26c, the object of the window */
    char pad270[0xc];
    short font;                         /* +0x27c, the window font */
    char pad27e[0x146];
} D2WINVIEW;

typedef union OPTIONBYTES {
    int word;                               /* the whole dword */
    unsigned char byte[4];                  /* the four option bytes */
} OPTIONBYTES;
extern OPTIONBYTES ai_GfxDetail, ai_GfxFloor, ai_GfxSet, ai_GfxFog, ai_GfxResolution, ai_GameSwitches, ai_GamePanel, ai_OptionPanelHelp;
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int ai_Unk5fe710;

extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern D2WINVIEW *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int ai_Unk6333cc;
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern D2WINVIEW *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern D2WINVIEW *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2WINVIEW *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern unsigned sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern D2OBJVIEW *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf (no CRT symbol available) */

