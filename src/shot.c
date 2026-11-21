/* shot.c -- the wind and the cannon ball (0x5dbc10 .. 0x5dd3e0, the end of the game code): the default
   effect record, the wind state (init / update / save / load), the ball's sphere volume, the shot start, its
   flight steps and ground test, the aim solver. Names are ours; the file name is a guess.
   Start: menu.c's .data strings end with 0x5db110's "Locations\zigeuner\poker2" (0x62de3c), then a new plain
   .data block starts at 0x62de58 (ai_ShotDrag, ai_ShotHitHeight, ai_DefaultEffect); in .rdata the unit's named
   cannon constants 0x5eedf4..0x5eee1c follow menu.c's last literal 0x5eedf0.
   Open: the head region is menu.c's (dcp unit split copied the declarations the functions use).
   unit split from menu (dcp unit split); boundaries in decl/text.dcl */
extern int gm_PlayerRecords[688];
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
/* the unit's first .rdata constant (before the header's, in address order): the filter of the gypsy windows, the
   flags and kind he and lc pass to ai_PickActionBuilding too */
extern const unsigned __int64 gypsyFilter;
extern const unsigned int ai_BuildingTypeMasks[6];
#include "menu.h"
#include "shot.h"
#include "sv_t.h"
#include "lc.h"
#include "data_views.h"
#include "d2_types.h"                 /* D2OBJ, D2WIN, D2PIC: the records of d2_Objs, d2_LastWindowPtr, d2_Pics */
#include "ls.h"                      /* ls_SavePath */
#include "ahm.h"
#include "ai.h"
#include "amt.h"
#include "anm.h"
#include "bio.h"
#include "d3_interface.h"
#include "d3s.h"
#include "f3gm.h"
#include "f3init.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "msx.h"
#include "sblib.h"
#include "sv.h"
#include "tr.h"
#include "poker.h"
#include "mission.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "dd_vesa.h"
#include "ch.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */
#define ai_CharsK ((AICHARKINDVIEW *)((char *)ai_Chars + 2))  /* 0x770b82, the records from their kind byte (ai_HasQuicksavePlayer walks them so) */

extern OPTIONBYTES ai_GfxResolution;
/* full rebuild: the symbols whose original addresses the code above spells literally */

extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf (no CRT symbol available) */

/* dcp unit split: the declarations of menu before the moved functions */
void d2_CentreViewWindows(int view); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
void d2_CloseView(int view);
int io_PrintText(char *fmt, ...); int d2_GetWinObj(int view, int idx, int k);
int sv_CreateScrollButtons(int x, int y, int f, int g, int p5, int *b, int *a, int *c); /* 0x52e860, sv.c:
                                           the slider of the form f at (x, y) */
int d2_GetViewWin(int view, int i);         /* 0x475ff0, d2.h: window i of a view, -1 if out of range */
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag); /* 0x5092e0, f3gm.c: the building skill (ai.h's
                                               prototype; f3snd.h's building view is unsigned char *) */
char *gv_FindAccused(char *c, int max, int *ids, int *cnt);  /* 0x4f41b0, gv.c: the character with the
                                               most records of its table not yet paired with c */
void cm_SendProofExecuted(int record, int character); /* 0x4c4ed0, cm.c: message 0x24, the pair (character,
                                               record) into the executed table 0x91b6e0 */
void cm_SendMoneyDelta(int character, int delta); /* 0x4c5f60, cm.c: message 0x5a, adds delta to the
                                               character's money (+0x194), clamped 0 .. 0x32 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2); /* 0x53dda0, he.c */
void d2_SetObjValue(int obj, ...); int d2_GetObjValue(int obj); int d2_CreateWinText(short x, short y, char *text, int win); void d2_SetObjText(int obj, char *text);
char *itoa(int, char *, int); void d2_SetObjFont(int obj, int font); int sv_RunFrame(int cb, unsigned mask);
int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj); void d2_MenuNavigate(int g); void d2_RaiseViewWindows(int view); void d2_RadioClear(int g);
#pragma pack(push, 1)
typedef struct SAVEREQUEST {                /* 0x210, a save file entry (ai_ListFiles) or an empty slot's request */
    char pad00[9];
    char name[0x80];                        /* +0x09, the slot caption (the file name without ".SAV") */
    char title[0x80];                       /* +0x89, the save's title */
    char path[0x107];                       /* +0x109, the file path (ai_FillSaveSlots opens it) */
} SAVEREQUEST;                              /* 0x210 */
typedef struct SAVESLOTVIEW {               /* 0x220, one slot of the save / load window */
    unsigned char kind;                     /* +0x00, the slot kind (ls_SaveSlot for a registered save) */
    char pad01[3];
    int obj;                                /* +0x04, the slot's caption text object, -1 = none */
    int win;                                /* +0x08, the slot's child window, -1 = free */
    unsigned char used;                     /* +0x0c, 1: a filled slot, 0: an empty one */
    char pad0d[3];
    union {
        SAVEREQUEST req;                    /* +0x10, the request ai_RegisterSaveSlot copied */
        struct {
            char reqpad[9];
            char name[0x80];                /* +0x19, the caption (the request's name) */
            char title[0x80];               /* +0x99, the save's title (the header's +0x9c) */
        };
    };
} SAVESLOTVIEW;                             /* 0x220 */
#pragma pack(pop)
typedef struct D2INPUTVIEW { char pad0[0x10]; int w; char pad1[0x148]; int maxw; } D2INPUTVIEW;
extern D2INPUTVIEW d2_Inputs[];         /* 0xfa6020, d2.h: D2INPUT [0x80] */
extern int d2_FontHeight;               /* 0xf8f084, d2.h */
extern struct D2INPUT *d2_FocusedInput;           /* 0x6ed9dc, d2.h: D2INPUT * */
extern unsigned char kb_Last;           /* 0x1468410, d2.h */
int d2_WinCreateSlider(int x, int y, int pic, int unk, int win);     /* 0x46bf10, d2.h: D2OBJ *d2_Objs */
void d2_CloseView(int view);                                         /* 0x4758f0, d2.h */
void d2_SetObjWidth(int obj, short w);                               /* 0x46c260, d2.h */
int d2_OpenInput(int x, int y, int w, int h, unsigned char flags);   /* 0x46a110 */
void d2_RemoveObj(int obj);                                          /* 0x46e710 */
void d2_SetObjFont(int obj, int font);                               /* 0x4760b0 */
void d2_SetObjValue(int obj, ...);                                               /* 0x475cb0, d2.c (varargs: the call sites pass 1, 3 or 4 values) */
void d2_SetObjMax(int obj, int v);                                   /* 0x475f40 */
typedef struct D2FADEVIEW { unsigned char flags; char pad1[0x63]; } D2FADEVIEW;
extern D2WIN d2_Wins[];             /* 0xf8f120, d2.h: D2WIN d2_Wins[0x61] */
extern float sv_FramesPerTick;          /* 0x62378c, sv.c: the frames drawn since the last timer tick
                                           (the frame loop counts them in 0x623790, the tick callback
                                           0x520ec0 copies the count as a float and clears it) */
int d2_CreateWindow(short x, short y, short w, short h, unsigned int flags); /* 0x472390, d2.c */
void d2_PlaceWindowScaled(int win, int flags); /* 0x475730, d2.c */
int d2_RemoveWindow(int win);           /* 0x472ce0, d2.c */
int io_PrintText(char *fmt, ...);          /* 0x515960, io_text.c: prints a text (id or literal) with its
                                           tokens into the current window and redraws it */
extern short d2_ScreenW;                /* 0xfb9072, d2.h */
extern short d2_ScreenH;                /* 0xfb9070, d2.h */
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
D2FADEVIEW *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d2_ReleaseFade(D2FADEVIEW *f);        /* 0x476860, d2.h */
void d2_ClearScreen(void);              /* 0x46e990, d2.h */
void d2_ShowPicture(int pic);           /* 0x46cfd0, d2.h */
void sv_SetPanelEnabled(int on);        /* 0x522100, sv.c: disables the panel widgets when on is 0 */
void sv_DisableVisibleWidgets(int on);  /* 0x52e600, sv.c: flags the widgets of the visible forms */
void sv_RedrawScreen(void);             /* 0x524010, sv.c */
extern int ai_GameOver;                 /* 0x6f259c, cm.h: nonzero when the game is over / left */
extern int ai_FrameWait;                /* 0x6f22bc, sv.c: frames still to wait (sv_RunFrame counts it down) */
extern int sv_PopRequest;               /* 0x6f22a0, cm.h / gv.h: the dialog close request */
extern int d2_CutsceneActive;           /* 0x6ed9d4, cm.h / gfx.h */
extern unsigned char kb_Last;           /* 0x1468410, d2.h: the last key (di_input.c) */
void d2_FocusInput(int obj);                                           /* 0x478320 */
#pragma pack(push, 1)
typedef struct SAVEHEADERVIEW {             /* 0xfc */
    int version;                            /* +0x00, the save version (also ls_Version) */
    unsigned char flags;                    /* +0x04, bit 2: a map only save */
    char player[0x20];                       /* +0x05, the player's name */
    char pad25[0x0d];
    int year;                               /* +0x32, the game clock's year (0x578 + it is the calendar year) */
    unsigned short hour;                    /* +0x36, its hour */
    int minute;                             /* +0x38, its minute */
    char pad3c[4];
    char scenario[0x10];                    /* +0x40, the character's scenario record */
    char character[0x0c];                   /* +0x50, the character's saved bytes */
    char pad5c[4];
    unsigned char nameTextOffset;           /* +0x60, added to the text id 0x130 (0x18f compact) of the character line */
    unsigned char officeTextOffset;         /* +0x61, added to the text id 0x253 (0x27b compact) of the character line */
    unsigned char traitTextOffset;          /* +0x62, added to the text id 0x114 (0x11e compact) of the character line */
    char pad63[5];
    int money;                              /* +0x68, the character's money ("%i%c") */
    unsigned char compactTexts;             /* +0x6c, nonzero: the three text ids shift by +10 / +0x5f / +0x28 */
    unsigned char slot;                     /* +0x6d, ls_SaveSlot of the save */
    short pad6e;
    int playerBuilding;                     /* +0x70, the player's building id, -1 = none */
    int otherBuilding;                      /* +0x74, the second building id, -1 = none */
    int party[8];                           /* +0x78, the table 0x76fa24 */
    int difficulty;                         /* +0x98, ai_Difficulty (2 for a save older than 0x10038) */
    char title[0x60];                       /* +0x9c, the title shown in the slot */
} SAVEHEADERVIEW;                           /* 0xfc */
#pragma pack(pop)
extern int ls_Version;                      /* 0x623650, ls.c: the version of the save read last */
int ls_ReadSaveHeader(int f, int *h);      /* 0x51cee0, ls.c: reads a save header into h (no header there) */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win); /* 0x472aa0, d2.h */
int d2_WinOpenRaw(int x, int y, int w, int h, void *src, int win);  /* 0x46c390, d2.h */
int d2_WinOpenPic(short x, short y, int pic, int win); /* 0x4731d0, d2.h: creates a picture object */
extern int d2_HitWindow;                    /* 0xf8aad0, the window the pointer hit (gfx.c sets it from
                                               d2_PointerWin; also the "double click" id of the slots) */
extern int ai_GameFlags;                /* 0x6f23f8, the game flags: the load window writes the load mode 10 whole */
void d2_EnableObject(int obj, int on);      /* 0x475aa0, d2.h: enables / disables an object and its items */
int sv_OpenMessageBox(int text, unsigned char flags); /* 0x521900, sv.c: opens the waiting box with text */
typedef struct SCREENMODE {                 /* 8 bytes, a screen mode of the table 0x62d7b8 */
    int w;                                  /* +0x0, the width */
    int h;                                  /* +0x4, the height */
} SCREENMODE;
char *itoa(int value, char *buf, int base); /* 0x5e56c4, the library's integer to text (base 10 here) */
extern unsigned int d3s_TexSize;         /* 0xfb9260, d3s.c */
extern unsigned int d3s_MaxTexSize;      /* 0xfb91dc, d3s.c */
extern float d3o_FogScale;               /* 0x5fbd04, d3_interface.h */
extern float sv_CameraAngleFactor;               /* 0x6f22d8, sv.c */
extern unsigned char ai_GameState;       /* 0x6f25ac */
extern int d3_CurrentScene;              /* 0x633444 */
extern D3SCENE *d3_DefaultScene;         /* 0x5fb5b0 */
extern D3SCENE d3_Scenes[];              /* 0xfd7ba0 */
extern char d3fl_Mips;                   /* 0x63373c, d3_fl.h */
extern char ch_ShadowEnabled;            /* 0x5f122c, the shadow flag ch.c copies into the bodies' +0x211 bit 3 */
extern float sv_CameraLimits[12];        /* 0x6237d0, the camera limit table (height / angle pairs, sv.c) */
extern float sv_CameraHeightNear;        /* 0x623800, the camera height at the near / far blend (sv.c) */
extern float sv_CameraAngleNear;         /* 0x623804, the camera angle at the near / far blend */
extern float sv_CameraHeightFar;         /* 0x623808 */
extern float sv_CameraAngleFar;          /* 0x62380c */
void d3s_ResetShadows(struct D3OBJECT *obj);    /* 0x45ce00, d3s.c */
void d3s_FreeShadows(struct D3OBJECT *obj);     /* 0x45ccd0, d3s.c */
void d3_UpdateAll(unsigned char full);          /* 0x43b6e0, d3_engine.c */
char d3if_SelectScene(int index, char keep);    /* 0x4478e0, d3_interface.c */
void d3_ReloadMeshes(unsigned int mask, int type, void (*progress)(int a, int i, int n));  /* 0x43e090, d3_engine.c */
void d3_ApplyGraphicsOptions(int a, unsigned char b, unsigned char c, unsigned char mode, int e); /* 0x43c4e0 */
void ch_SetLowPoly(unsigned char mode);         /* 0x402af0, ch.c */
void d3fl_SetMips(char a);                      /* 0x43f930, d3_fl.c */
void ai_CheckBuildSites(void);                  /* 0x58fb60, lc.c */
void d3_SetRLights(unsigned char on);           /* 0x43e180, d3_engine.c */
struct D3FLMAP;
void init_SetDetailLevel(struct D3FLMAP *s, char f);       /* 0x513590, f3init.c */
void sv_PlaceCamera(float f);                   /* 0x525dd0, sv.c */
char d3_ForEachObject2(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, void (*cb)(struct D3OBJECT *obj)); /* 0x438e60, d3_engine.h */
typedef union INIT_LOD {
    unsigned int all;               /* the whole dword, which is what the original compares and stores */
    struct {
        unsigned int mode : 7;      /* 1: all three LODs, 2: the best one found */
        unsigned int shadow : 1;    /* the "_s" shadow variant */
    } bits;
} INIT_LOD;
extern INIT_LOD d3io_Lod;          /* 0x5fbf78 */
typedef struct D2SURFVIEW {
    char pad0[0x10];
    int pitch;                          /* +0x10, in pixels */
    char pad1[8];
    unsigned short *bits;               /* +0x1c, pixels (system memory, or while locked) */
} D2SURFVIEW;
extern D2SURFVIEW *d2_ScreenSurf;       /* 0x6ed924, d2.h: D2SURF * */
int d2_LockSurface(D2SURFVIEW *surf);   /* 0x479020, d2.h: D2SURF *, locks the surface (pixels in bits) */
int d2_UnlockSurface(D2SURFVIEW *surf); /* 0x4790e0, d2.h: D2SURF * */
void *mem_Alloc(int size, char *tag);   /* 0x4223c0, d3.h */
void mem_Free(void *p);                 /* 0x422620, d3.h */
extern unsigned int dd_RMask;           /* 0x1464fa4, dd_vesa.h: the red channel mask of the display */
extern unsigned int dd_GMask;           /* 0x1464fb0, dd_vesa.h */
extern unsigned int dd_BMask;           /* 0x146359c, dd_vesa.h */
int ls_Read(void *buf, int size, int count, int f);   /* 0x51c920, ls.c: vfs_fread items */
void sv_SetStatusText(char *s);   /* 0x52e330, sv.c */
int cm_SendExSysMessage(unsigned char id, void *msg);   /* 0x4c4db0, as in cm.h */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);   /* 0x41e670, as in dd_vesa.h */
unsigned int util_Hash(unsigned char *buf, unsigned int len);      /* 0x401000, as in bio.h */
void sv_PumpNet(void);                                             /* 0x52fe00, sv.c */
void sv_BroadcastText(unsigned fl, char *t);  /* 0x52fcf0, sv.c: sends t as the network message 0xe and
                                                 shows it until the answer, then clears the status line */
int ls_WriteSaveHeader(int f, unsigned flags); /* 0x5194c0, ls.c: writes the save header (the flag word) */
int ls_ReadSaveHeader(int f, int *h);         /* 0x51cee0, ls.c: reads the save header into h */
extern int cm_RequestedSaveSlot;        /* 0x6f25c8, cm.h: the save slot the request asked for */
extern char cm_RequestedSaveName[];     /* 0x6f25d0, cm.h: the save name the request carried */
extern int cm_SaveAcks[][2];            /* 0x76fa20, cm.h: 8 slots; [0] = the player's netId, [1] = the
                                           save name hash, -1 when empty (message 0x10 fills the first free) */
extern unsigned char ai_NumPlayers;     /* 0x62489c, the player count (ai.c / cm.c / main.c) */
extern unsigned short ai_PlayerId;      /* 0x6f25b8, the local player (cm.h / am.h / gv.h) */
typedef struct AICHARVIEW {
    unsigned short id;                  /* +0x000, 0xffff when free */
    unsigned char kind;                 /* +0x002, 6 / 7 = a player */
    char pad003;
    unsigned int netId;                 /* +0x004 */
    char pad008[0x15e];
    unsigned char officeKind;           /* +0x166, the kind of the office held (amt.h AMTCHARVIEW.officeKind) */
    char pad167[0x21];
    char selected;                      /* +0x188, the group selection flag (sv.c ai_Selected, cleared by sv_ClearSelection) */
    char pad189[3];
    int pic;                            /* +0x18c, the picture of the character the poker panel shows (reasoned: the picture index) */
    char pad190[0x3c];
    unsigned int flags1cc;             /* +0x1cc, ai.h's flags1cc (bit 0x20000 = the blood revenge) */
    char pad1d0[0x3c];
    int partyId;                        /* +0x20c, the scene party the character belongs to, -1 = none (ai_char.h AICHAR.partyId) */
    char pad210[0xc];
} AICHARVIEW;
#pragma pack(push, 1)
typedef struct AICHARKINDVIEW {
    unsigned char kind;                 /* +0x000 (record +2), 6 / 7 = a player */
    char pad001[5];
    unsigned char health;               /* +0x006 (record +8), > 0 = alive (ai_char.h AICHAR.health) */
    char pad007[0x203];
    int partyId;                        /* +0x20a (record +0x20c), -1 = none (ai_char.h AICHAR.partyId) */
    char pad20e[0xe];
} AICHARKINDVIEW;
#pragma pack(pop)
struct AIBLD; int ai_ProductionWindow(struct AIBLD *bld); /* 0x58bd60, lc.c: "Handel\Handel_PRODUKTION" */
int ai_OpenTransport(int rec);             /* 0x5af1a0, lc.c */
void ai_TradeSearchWindow(unsigned char *cfg, int arg); /* 0x595480, lc.c: "HANDEL\HANDEL_SUCHEN" */
void ai_MasterLetterWindow(struct AIBLD *bld); /* 0x5c3200, lc.c: "ob_MEISTERBRIEF" */
void ai_MarketContact(struct AIBLD *rec);  /* 0x58f510, lc.c */
int sv_CheckMoney(char *o, int n);         /* 0x520f90, sv.c: 1 when the object's money (+0x194)
                                              covers n, else a message box with the text 0x74 */
int sv_MessageBox(int text, unsigned flags); /* 0x521150, sv.c: the modal message box */
void sv_CameraFollow(void);                /* 0x52d7a0, sv.c: the per-frame camera follow */
int d2_RemoveWindow(); int io_PrintText(char *fmt, ...);
int d2_GetObjValue();
int he_IsMessageKindFiltered(int kind); /* 0x575640, he.c: nonzero when the kind is filtered */
void he_SetMessageKindFiltered(int kind, int filtered); /* 0x575710, he.c: sets / clears the kind's bit */
int cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60, cm.c: message 0x19 (the field block) */
void d2_SetCaption(int obj, char *text);           /* 0x473540, d2.h: sets the caption and its width */
typedef struct AIREQ {
    unsigned short id;                  /* +0x00 */
    unsigned char kind;                 /* +0x02 */
    char pad03;
    int handle;                         /* +0x04 */
    char pad08[0x54];
} AIREQ;
typedef struct CONTACTREQ {
    int type;                           /* +0x00 */
    int a;                              /* +0x04 */
    char pad08[8];
    int b;                              /* +0x10 */
    char pad14[0x3c];
} CONTACTREQ;
#pragma pack(push, 1)
typedef struct AIBLDVIEW {
    char pad0[0x5d];
    int objList;                        /* +0x5d, AIBLD.objList: the objects of the building */
} AIBLDVIEW;
#pragma pack(pop)
typedef struct GYPSYROW {               /* 0x38 bytes: one row of the special-amount list (ai_AmountRowStatus / ai_AmountRow read it as ints) */
    AICHARVIEW *rec;                    /* +0x00, the character the row shows (0 = empty) */
    int ctl;                            /* +0x04, the row's control object (d2_Objs index) */
    int pad08[2];
    int x;                              /* +0x10, the position of the row */
    int y;                              /* +0x14 */
    int pad18[8];
} GYPSYROW;
extern struct D2PIC *d2_Pics;              /* 0x6ed918, d2.h: D2PIC * */
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_CheckGoodAmount(char *o, int n, unsigned char t);
struct AITIME;
int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int ai_PokerStart();
unsigned short ai_PokerStep(); int gm_BuildingSkill(); float ai_NormalizeEffectScale(float x);
typedef struct PLAYERRECVIEW {          /* f3gm.h's player record (gm_PlayerRecord, 0x56 words): only the float the poker game reads */
    char pad00[0x60];
    float luck;                         /* +0x60, the chance that the player sits down to play (decays by * 0.66 - 0.1 per game; reasoned) */
} PLAYERRECVIEW;
/* the cannon constants, read-only, in the original's order (named .rdata 0x5eedf4..0x5eee1c, the unit's first
   .rdata; read from memory, not folded; 0.0254, 0.1, 4/3 pi and -2.5 are these objects at the sites that read them,
   the same values elsewhere are pooled literals) */
const float ai_InchPerMeter = 39.37008f;     /* 0x5eedf4, world units (inches) per meter */
const float ai_MeterPerInch = 0.0254f;       /* 0x5eedf8, meters per inch (ai_SolveCannonAim) */
const float ai_Gravity = 9.81f;              /* 0x5eedfc, m/s^2 */
const float ai_ShotStep = 0.013513514f;      /* 0x5eee00, 1/74, the time (s) of one flight step of ai_StepShot */
const float ai_ShotHalfStep = 0.0067567569f; /* 0x5eee04, 1/148, the step of ai_StepShotSlow */
const float ai_IronDensity = 7874.0f;        /* 0x5eee08, kg/m^3, the cannon balls' (am.c, sv_t.c) */
const float ai_AirDensity = 1.2f;            /* 0x5eee0c, kg/m^3 */
const float ai_ReachScale = 0.1f;            /* 0x5eee10, the factor of a value divided by the ball's reach
                                                (ai_PositionShare, ai_InitShot's start speed) */
const float ai_WindGain = 60.0f;             /* 0x5eee14, the wind scale (60): the factor of ai_InitWind's
                                                per-direction random values and the range ai_NormalizeEffectScale
                                                clamps to */
const float ai_SphereFactor = 4.1887903f;    /* 0x5eee18, 4/3 pi (ai_SphereVolume) */
const float ai_AimErrLow = -2.5f;            /* 0x5eee1c, the low bound of ai_SolveCannonAim's height error */

/* 0x5dbc10, the default effect record (5 B): returns the address of ai_DefaultEffect (0x62de60: an
   empty name, the scale 1.0, the lifetime 25.0, then the entry's ids / callbacks). The twin
   0x5dbc20 returns its scale from the same record. */

int ai_GetDefaultEffect(void)
{
    return (int)&ai_DefaultEffect;
}
/* 0x5dbc20, the default effect scale (13 B): the scale field of the default effect record
   (ai_DefaultEffect.scale 0x62de6c) through the 0 .. 60 -> 0 .. 1 clamp 0x5dd090
   (ai_NormalizeEffectScale); the result is the entry's scale (passed to the 0 .. 100 slider as
   100 * value, sv_t.c). The callers pass an ignored argument through a K&R declaration.
   @original the original TU called 0x5dd090 through an int K&R declaration: it pushes the float's
   bit pattern as an int (mov eax,ds:0x62de6c; push eax) and does not pop the float result, so the
   function returns the callee's st(0) (float) and the result is used as such by its callers. */

float ai_GetDefaultEffectScale(void)
{
    ((int (__cdecl *)())ai_NormalizeEffectScale)(ai_DefaultEffect.scaleBits); /* @original the float callee was called through an int K&R declaration (push the bits, no fstp) */
}

#include <math.h>

/* 0x5dbc30, builds the wind state of the weather (649 B; sv_SetupWeather calls it with the day phase, the game clock's first
   word and the two 24 entry wind rose tables sin / cos of the random wind directions): type (0 .. 3, else the default) is
   the day phase, id the clock word, from / to the sin and cos tables. The state record ai_EffectState gets the clock of
   its last update (year id, hour 6, minute 0; ai_UpdateWind advances from it), a copy of both tables, and the caller's two tables are negated in place (every
   entry * -1). Then for each of the 24 directions three random values are drawn by the phase's constants (base = random * 60 * k + 1, gust = random * 60 * k + c,
   phase = (random + n) * pi or 2 pi): type 0 k 0.25 / 0.3 + 0.2 / 5 * 2 pi, 1 k 0.05 / 0.4 + 0.1 / 3 * pi, 2 k 0.5 / 0.2 + 0.3 /
   5 * 2 pi, 3 k 0.25 / 0.5 / 3 * pi, else 0.5 / 0.5 / 2 * 2 pi. At the end a random angle (0 .. 2 pi) is stored and the
   default effect ai_DefaultEffect gets the vector (the sin table entry 6, 0, the cos table entry 6) and the scale
   |sin(angle) * gust[6]| + base[6]. Void. Reasoned: the wind meaning follows from the caller (the wind rose tables
   of sv_SetupWeather) and the sin / cos copies; the arrays base / gust / phase are named from their formulas. */
void ai_InitWind(int type, int id, float *from, float *to)
{
    int i;

    ai_EffectState.year = id;
    ai_EffectState.hour = 6;
    ai_EffectState.min = 0;
    memcpy(ai_EffectState.sinTab, from, sizeof(ai_EffectState.sinTab));
    memcpy(ai_EffectState.cosTab, to, sizeof(ai_EffectState.cosTab));
    for (i = 0; i < 24; i++) {
        from[i] = from[i] * -1.0f;
        to[i] = to[i] * -1.0f;
    }
    switch (type) {
    case 0:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.3f + 0.2f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 1:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.05f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.4f + 0.1f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    case 2:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.2f + 0.3f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 3:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    default:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 2.0f) * 6.2831855f;
        }
        break;
    }
    ai_EffectState.angle = util_RandomF() * 6.2831855f;
    ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[6];
    ai_DefaultEffect.pos[1] = 0;
    ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[6];
    ai_DefaultEffect.scale = (float)(fabs(sin(ai_EffectState.angle) * ai_EffectState.gust[6]) + ai_EffectState.base[6]);
}
/* 0x5dbed0, advances the wind (235 B; sv_RunFrame calls it every frame): the hours since the state's last
   update (gm_MinutesBetween of the stored clock and ai_Time, / 60); when not negative, the angle of
   ai_EffectState grows by hours * phase[hour], the default effect ai_DefaultEffect gets the hour's wind
   vector (sin, 0, cos) and the scale (sin(angle) + 1) * gust[hour] / 2 + base[hour], and the clock is
   stored as the last update. */

void ai_UpdateWind(void)
{
    AITIME last;
    AITIME *now = &ai_Time;
    float hours;
    int hour;

    last.year = ai_EffectState.year;
    last.hour = (unsigned short)ai_EffectState.hour;
    last.min = ai_EffectState.min;
    last.sec = 0;
    hours = gm_MinutesBetween((struct AITIME *)&last, (struct AITIME *)&ai_Time) * (1.0f / 60.0f);
    if (!(hours < 0.0f)) {
        hour = now->hour;
        ai_DefaultEffect.pos[1] = 0;
        ai_EffectState.angle += hours * ai_EffectState.phase[hour];
        ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[hour];
        ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[hour];
        ai_DefaultEffect.scale = ((float)sin(ai_EffectState.angle) + 1.0f) * ai_EffectState.gust[hour] * 0.5f
            + ai_EffectState.base[hour];
        ai_EffectState.year = now->year;
        ai_EffectState.hour = hour;
        ai_EffectState.min = now->min;
    }
}
/* 0x5dbfc0, writes the wind state to the save game (241 B; the save writer 0x51c870 calls it): the fields of
   ai_EffectState one by one with vfs_fwrite, the clock of the last update, the sin / cos / base / gust /
   phase tables and the angle. 0 as soon as a write fails, else 1. */

int ai_SaveWind(struct VFSFILE *f)
{
    if (vfs_fwrite(&ai_EffectState.year, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.hour, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.min, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f) <= 0)
        return 0;
    return vfs_fwrite(&ai_EffectState.angle, 4, 1, f) > 0;
}
/* 0x5dc0c0, reads the wind state from the save game (223 B; the save reader 0x5205f0 calls it from version
   0x10050): the fields of ai_EffectState in ai_SaveWind's order with ls_Read. 0 as soon as a read fails,
   else 1. */

int ai_LoadWind(int f)
{
    if (!ls_Read(&ai_EffectState.year, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.hour, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.min, 4, 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f))
        return 0;
    return ls_Read(&ai_EffectState.angle, 4, 1, f) != 0;
}
/* 0x5dc1a0, the sphere volume record (89 B): radius and size plus the metrics derived from the size
   into out (+0 = radius, +4 = size, +8 = size^2 * 2 pi, +0xc = size^3 * 4/3 pi, +0x10 = that *
   radius). Returns 0 when a negative value comes in, 1 otherwise. */

int ai_SphereVolume(float radius, float size, float *out)
{
    float volume;

    if (radius < 0.0f || size < 0.0f)
        return 0;
    out[0] = radius;
    out[1] = size;
    out[2] = size * size * 6.2831855f;
    volume = ai_SphereFactor * (size * size * size);
    out[3] = volume;
    out[4] = volume * radius;
    return 1;
}
/* 0x5dc210, the two-axis position share (35 B): the two values a / b divided by the sphere volume
   record's reach (+0x10, volume * radius - ai_SphereVolume's record, am.c 0x4a67a0 passes it as
   int *in) and scaled by 0.1; the pair goes into out (+0, +4) as the ballistic range rng[0] / rng[1].
   Void: the caller ignores the result (am.h declares it void). The division is written value / field
   (no parentheses): the parenthesised form makes VC6 emit fld divisor; fdivr value instead. */

void ai_PositionShare(float a, float b, SPHEREVOLUME *rec, float *out)
{
    out[0] = a / rec->reach * ai_ReachScale;
    out[1] = b / rec->reach * ai_ReachScale;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* 0x5dc240, builds a cannon ball in flight (341 B; am.c's shot callback 0x4a6ae0 calls it per ball): the
   ball's sphere (ai_SphereVolume of density and size: mass = volume * density, am.c passes 7874, the density
   of iron) gives the speed charge / mass * 0.1; the start velocity is the barrel direction dir times it (also
   the current velocity), the start and the current position are pos, the wind is ai_DefaultEffect with
   its scale clamped to 0 .. 60 (ai_WindGain) and the vector scaled by it; the surface and the mass, state 1
   (in flight), the clock d3_Time (ms) and 0 steps complete the record. Returns 1. */
int ai_InitShot(SHOTREC *shot, D3VECTOR *pos, D3VECTOR *dir, float charge, float size, float density)
{
    /* @original the call goes through a pointer: the original calls ai_GetDefaultEffect first and keeps the
       record in edx across ai_SphereVolume; a direct call VC6 sinks to the wind copy (it knows the callee) */
    EFFECTREC *(*get)(void) = (EFFECTREC *(*)(void))ai_GetDefaultEffect;
    EFFECTREC *wind = get();
    SPHEREVOLUME ball;

    ai_SphereVolume(density, size, (float *)&ball);
    shot->velStart.v = *dir;
    shot->velStart.speed = charge / ball.reach * ai_ReachScale;
    shot->velStart.v.x = shot->velStart.speed * shot->velStart.v.x;
    shot->velStart.v.y = shot->velStart.speed * shot->velStart.v.y;
    shot->velStart.v.z = shot->velStart.speed * shot->velStart.v.z;
    shot->vel = shot->velStart;
    shot->wind = *wind;
    shot->wind.scale = MAX(MIN(shot->wind.scale, ai_WindGain), 0.0f);
    shot->wind.pos[0] = shot->wind.scale * shot->wind.pos[0];
    shot->wind.pos[1] = shot->wind.scale * shot->wind.pos[1];
    shot->wind.pos[2] = shot->wind.scale * shot->wind.pos[2];
    shot->posStart = *pos;
    shot->pos = *pos;
    shot->mass = ball.reach;
    shot->area = ball.surface;
    shot->state = 1;
    shot->time = d3_Time;
    shot->steps = 0;
    return 1;
}
/* 0x5dc3a0, scatters a cannon ball's start velocity (254 B; am.c's shot callback 0x4a6ae0 calls it after
   ai_InitShot for each ball): 0 once the ball made a flight step. The start velocity and speed are scaled by
   (1.5 r - 1) * speedSpread + 1, then two random deviations are added to the velocity: (2 r - 1) * speed *
   sideSpread along side (am.c passes the vector the balls are set apart along) and ((2 r - 1) * speed *
   otherSpread + 1) along other (a second vector of the planned shot); the result is also the current
   velocity. Returns 1. r is util_RandomF each time. Reasoned: the spread names follow the formulas,
   am.c passes the same spread value for both deviations. The 1.0s are the shared ai_FloatOne the original
   loads. */

int ai_ScatterShot(SHOTREC *shot, float sideSpread, D3VECTOR *side, float otherSpread, D3VECTOR *other,
                   float speedSpread)
{
    float k, x, y, z;
    D3VECTOR dside, dother;

    if (shot->steps != 0)
        return 0;
    k = (util_RandomF() * 1.5f - 1.0f) * speedSpread + 1.0f;
    shot->velStart.v.x = k * shot->velStart.v.x;
    shot->velStart.v.y = k * shot->velStart.v.y;
    shot->velStart.v.z = k * shot->velStart.v.z;
    shot->velStart.speed = k * shot->velStart.speed;
    k = sideSpread * (shot->velStart.speed * (util_RandomF() * 2.0f - 1.0f));
    dside.x = k * side->x;
    dside.y = k * side->y;
    dside.z = k * side->z;
    k = otherSpread * (shot->velStart.speed * (util_RandomF() * 2.0f - 1.0f)) + 1.0f;
    dother.x = k * other->x;
    dother.y = k * other->y;
    dother.z = k * other->z;
    x = dother.x + shot->velStart.v.x + dside.x;
    shot->velStart.v.x = x;
    y = dside.y + (dother.y + shot->velStart.v.y);
    shot->velStart.v.y = y;
    z = dside.z + (dother.z + shot->velStart.v.z);
    shot->velStart.v.z = z;
    shot->vel.v.x = x;
    shot->vel.v.y = y;
    shot->vel.v.z = z;
    shot->vel.speed = shot->velStart.speed;
    return 1;
}

/* ai_StepShot and ai_StepShotSlow: the drag 0.5 * density * coefficient * area * v^2 signed with v, written as
   plain products: VC6 orders an unparenthesised float product itself (the two globals by which it met first,
   so the wind's x drag, the function's first product, names the density before the coefficient) and a
   parenthesis costs a node in the scheduler's 81-instruction region (the flight step's last region ends
   exactly behind the fourth argument push of ai_ShotHitsGround).
   @original the wind's y drag keeps its two multiplications by the wind apart (fmul [wind.y] twice; the plain
   product squares a loaded copy: fld st(0) / fmul st,st(1)): the parentheses of WINDDRAGY, 8 ins without */
#define SIGN(v) ((v) > 0.0f ? 1.0f : -1.0f)
#define DRAG(v) SIGN(v) * shot->area * ai_AirDensity * ai_ShotDrag * (v) * (v) * 0.5f
#define WINDDRAG(v) shot->area * v * v * SIGN(v) * ai_AirDensity * ai_ShotDrag * 0.5f
#define WINDDRAGY(v) ((SIGN(v) * shot->area * ai_AirDensity * ai_ShotDrag) * (v)) * (v) * 0.5f

/* 0x5dc4a0, moves a cannon ball (1318 B; gv.c 0x4edf80 calls it every frame): 0 unless the ball is in flight
   (state 1). Off the height map (d3_GetHeightAt fails) the state becomes 3 and the energy 0. Otherwise one
   step of ai_ShotStep (1/74 s) is made per millisecond since the last one: the force on each axis is the
   drag 0.5 * ai_AirDensity * ai_ShotDrag * area * v^2 (signed with v) plus the same drag of the wind, and
   gravity pulls y; the velocity takes Heun's average of the change at the old and the predicted velocity,
   the position moves by velocity * step in inches. When ai_ShotHitsGround finds the move from the old to the
   new position hits the ground and the ball made more than 50 steps, it lands there: state 2, the energy
   0.5 * mass * v^2, returns 0. Returns 1 while it flies. */
int ai_StepShot(SHOTREC *shot)
{
    float fall = ai_Gravity * ai_ShotStep;
    float perMass = ai_ShotStep / shot->mass;
    D3SM *map = (D3SM *)init_SceneHeightMaps[0];
    int ticks, i, px, pz, npx, npz;
    float height;
    D3VECTOR4 v, next, wind, d, a, b, m;


    if (shot->state != 1)
        return 0;
    ticks = d3_Time - shot->time;
    if (!d3_GetHeightAt(map, &shot->pos, &px, &pz, &height)) {
        shot->state = 3;
        shot->energy = 0;
        return 0;
    }
    wind.x = WINDDRAG(shot->wind.pos[0]);
    wind.y = WINDDRAGY(shot->wind.pos[1]);
    wind.z = WINDDRAG(shot->wind.pos[2]);
    for (i = 0; i < ticks; i++) {
        v.x = shot->vel.v.x;
        v.y = shot->vel.v.y;
        v.z = shot->vel.v.z;
        d.x = DRAG(v.x);
        d.y = DRAG(v.y);
        d.z = DRAG(v.z);
        a.x = d.x + wind.x;
        a.y = d.y + wind.y;
        a.z = d.z + wind.z;
        a.x *= perMass;
        a.y *= perMass;
        a.z *= perMass;
        a.y -= fall;
        v.x = a.x + v.x;
        v.y = a.y + v.y;
        v.z = a.z + v.z;
        d.x = DRAG(v.x);
        d.y = DRAG(v.y);
        d.z = DRAG(v.z);
        b.x = d.x + wind.x;
        next.x = shot->pos.x;
        next.y = shot->pos.y;
        next.z = shot->pos.z;
        shot->time++;
        shot->steps++;
        b.y = d.y + wind.y;
        b.z = d.z + wind.z;
        b.x *= perMass;
        b.y *= perMass;
        b.z *= perMass;
        b.y -= fall;
        v.x = b.x + a.x;
        v.y = b.y + a.y;
        v.z = b.z + a.z;
        v.x *= 0.5f;
        v.y *= 0.5f;
        v.z *= 0.5f;
        shot->vel.v.x += v.x;
        shot->vel.v.y += v.y;
        shot->vel.v.z += v.z;
        m.x = shot->vel.v.x * ai_InchPerMeter * ai_ShotStep;
        m.y = shot->vel.v.y * ai_InchPerMeter * ai_ShotStep;
        m.z = shot->vel.v.z * ai_InchPerMeter * ai_ShotStep;
        next.x += m.x;
        next.y += m.y;
        next.z += m.z;
        if (ai_ShotHitsGround(map, &shot->pos, (D3VECTOR *)&next, px, pz, &npx, &npz) && shot->steps > 50) {
            shot->pos.x = next.x;
            shot->state = 2;
            shot->pos.y = next.y;
            shot->pos.z = next.z;
            shot->energy = (shot->vel.v.x * shot->vel.v.x + shot->vel.v.y * shot->vel.v.y
                + shot->vel.v.z * shot->vel.v.z) * shot->mass * 0.5f;
            return 0;
        }
        shot->pos.x = next.x;
        shot->pos.y = next.y;
        shot->pos.z = next.z;
        px = npx;
        pz = npz;
    }
    return 1;
}

/* 0x5dc9d0, moves a cannon ball in slow motion (1318 B; gv.c 0x4edf80 calls it instead of ai_StepShot when the
   effect's +0x14 flag is set): ai_StepShot instruction for instruction with the step ai_ShotHalfStep (1/148 s)
   for ai_ShotStep, in the gravity, the per-mass factor and the position move. */
int ai_StepShotSlow(SHOTREC *shot)
{
    float fall = ai_Gravity * ai_ShotHalfStep;
    float perMass = ai_ShotHalfStep / shot->mass;
    D3SM *map = (D3SM *)init_SceneHeightMaps[0];
    int ticks, i, px, pz, npx, npz;
    float height;
    D3VECTOR4 v, next, wind, d, a, b, m;


    if (shot->state != 1)
        return 0;
    ticks = d3_Time - shot->time;
    if (!d3_GetHeightAt(map, &shot->pos, &px, &pz, &height)) {
        shot->state = 3;
        shot->energy = 0;
        return 0;
    }
    wind.x = WINDDRAG(shot->wind.pos[0]);
    wind.y = WINDDRAGY(shot->wind.pos[1]);
    wind.z = WINDDRAG(shot->wind.pos[2]);
    for (i = 0; i < ticks; i++) {
        v.x = shot->vel.v.x;
        v.y = shot->vel.v.y;
        v.z = shot->vel.v.z;
        d.x = DRAG(v.x);
        d.y = DRAG(v.y);
        d.z = DRAG(v.z);
        a.x = d.x + wind.x;
        a.y = d.y + wind.y;
        a.z = d.z + wind.z;
        a.x *= perMass;
        a.y *= perMass;
        a.z *= perMass;
        a.y -= fall;
        v.x = a.x + v.x;
        v.y = a.y + v.y;
        v.z = a.z + v.z;
        d.x = DRAG(v.x);
        d.y = DRAG(v.y);
        d.z = DRAG(v.z);
        b.x = d.x + wind.x;
        next.x = shot->pos.x;
        next.y = shot->pos.y;
        next.z = shot->pos.z;
        shot->time++;
        shot->steps++;
        b.y = d.y + wind.y;
        b.z = d.z + wind.z;
        b.x *= perMass;
        b.y *= perMass;
        b.z *= perMass;
        b.y -= fall;
        v.x = b.x + a.x;
        v.y = b.y + a.y;
        v.z = b.z + a.z;
        v.x *= 0.5f;
        v.y *= 0.5f;
        v.z *= 0.5f;
        shot->vel.v.x += v.x;
        shot->vel.v.y += v.y;
        shot->vel.v.z += v.z;
        m.x = shot->vel.v.x * ai_InchPerMeter * ai_ShotHalfStep;
        m.y = shot->vel.v.y * ai_InchPerMeter * ai_ShotHalfStep;
        m.z = shot->vel.v.z * ai_InchPerMeter * ai_ShotHalfStep;
        next.x += m.x;
        next.y += m.y;
        next.z += m.z;
        if (ai_ShotHitsGround(map, &shot->pos, (D3VECTOR *)&next, px, pz, &npx, &npz) && shot->steps > 50) {
            shot->pos.x = next.x;
            shot->state = 2;
            shot->pos.y = next.y;
            shot->pos.z = next.z;
            shot->energy = (shot->vel.v.x * shot->vel.v.x + shot->vel.v.y * shot->vel.v.y
                + shot->vel.v.z * shot->vel.v.z) * shot->mass * 0.5f;
            return 0;
        }
        shot->pos.x = next.x;
        shot->pos.y = next.y;
        shot->pos.z = next.z;
        px = npx;
        pz = npz;
    }
    return 1;
}
#undef SIGN
#undef DRAG
#undef WINDDRAG
#undef WINDDRAGY
/* 0x5dcf00, the ground test of a cannon ball's flight step (390 B; ai_StepShot / ai_StepShotSlow): the cell
   of to (d3_GetHeightAt into npx / npz). When it is at most one cell from the cell px / pz of from, the
   step hits when to lies less than ai_ShotHitHeight above the ground there. A longer step is walked from
   from to to in steps of 1 / (the cell distance): the first point less than ai_ShotHitHeight above the
   ground is the hit, to is moved there. 1 on a hit, else 0. */

int ai_ShotHitsGround(D3SM *map, D3VECTOR *from, D3VECTOR *to, int px, int pz, int *npx, int *npz)
{
    float height, t, step;
    int dx, dz, cx, cz;
    D3VECTOR4 p, d;                     /* the engine's padded vectors (the original's frame holds 16 bytes each) */

    d3_GetHeightAt(map, to, npx, npz, &height);
    dx = abs(*npx - px);
    dz = abs(*npz - pz);
    if (dx <= 1 && dz <= 1) {
        if (ai_ShotHitHeight + height > to->y)
            return 1;
        return 0;
    }
    step = 1.0f / (float)sqrt((float)(dz * dz) + (float)(dx * dx));
    t = 0;
    d.x = to->x - from->x;
    d.y = to->y - from->y;
    d.z = to->z - from->z;
    p.x = from->x;
    p.y = from->y;
    p.z = from->z;
    d.x *= step;
    d.y *= step;
    d.z *= step;
    do {
        p.x += d.x;
        p.y += d.y;
        p.z += d.z;
        d3_GetHeightAt(map, (D3VECTOR *)&p, &cx, &cz, &height);
        if (ai_ShotHitHeight + height > p.y) {
            to->x = p.x;
            to->y = p.y;
            to->z = p.z;
            return 1;
        }
        t += step;
    } while (t < 1.0f);
    return 0;
}
/* 0x5dd090, the 0 .. ai_WindGain (60) clamp divided by ai_WindGain (69 B): 0 when <= 0, 1 when >= 60,
   else value / 60. The original loads 60 from its .rdata constant (ai_WindGain), so neither the
   division nor 60 / 60 is folded; max(min(..)) expands min twice (the second min is the selected
   value kept on the FPU stack) and VC6 copies the division into each arm. */

float ai_NormalizeEffectScale(float value)
{
    return max(min(value, ai_WindGain), 0.0f) / ai_WindGain;
}

#include <math.h>

/* 0x5dd100, solves the cannon aim (727 B; am.c's 0x4a67a0 calls it): the ball flies from `from` to `to`
   (world units, 0.0254 m each) under gravity; bisects the launch speed (range[0] .. range[1]) and the
   elevation angle (range[2] .. range[3], radians) in 16 steps until the landing point is within 2.5 m of the
   target. The error is the flight time (vertical speed / g plus the fall from the height difference) times the
   horizontal speed minus the horizontal distance; too short raises the angle (the speed once the angle is at
   its maximum), too far lowers the speed (the angle once the speed is at its minimum). Returns 0 with the
   speed in *outSpeed, the elevation in *outElev, the heading angle (d3_VecAngle2 against d3_ZAxis) in
   *outYaw and three vectors: outDir the horizontal direction tilted by the elevation, outUp d3_YAxis tilted
   the same way, outSide their cross product; -1 when no setting in the range hits (reasoned: the names follow
   the use in am.c).
   @deep deep s29: climb round 1 again: only the old err-inlined row (10 ins elsewhere, not kept). The
   first-reference tie key (a local declared with an initializer, the lever that put cm_TrialAskPlayer's
   two calls and ai_DonationWindow's reloads in the original's order): horiz / drop / speed / ang / i =
   control; s / c / h / t / err with = 0 emit a store and are worse (22..30 ins). Hunks unchanged: hdir.y's
   mov [..],ebp before the speedStep fstp, the d3_YAxis.y / .z products loading c first, outSide[0]'s
   second product operand order. prior: size queue s16: 208 -> 14 ins, raw size 727 = the original (padded
   size). Kept: sign = 0 at its declaration (the original zeroes ebp first and stores hdir.y from it), ang
   = range[3] before speed = range[0] (the FPU stack order), 1.0f / g written inline at both uses instead
   of an inv local (VC6 hoists g * 2 first, then 1 / g, as the original), g declared before speedStep /
   angStep (with equal counts the symbol order decides that speedStep, not g, takes range's dead slot).
   Left: hdir.y's mov [..],ebp scheduled before the speedStep fstp (the original after the angStep fsub),
   and in the success arm the c products of outUp[1] / outUp[2] load c first (the original the d3_YAxis
   component). No lever for those: all 1023 operand swaps of the five tail lines, 144 declaration orders of
   s / c, d3_YAxis through a pointer / array view / local copies of c and s (one code each); climb round 1
   best 10 ins by inlining err in both tests (another place differs, not kept). */
int ai_SolveCannonAim(float *from, float *to, float *range, float *outElev, float *outYaw, float *outSpeed,
                      float *outDir, float *outUp, float *outSide)
{
    float g = ai_Gravity;
    float speedStep = (range[1] - range[0]) * 0.5f;
    float angStep = (range[3] - range[2]) * 0.5f;
    D3VECTOR4 dir, hdir;
    float horiz, drop, s, c, h, t, err;
    float speed, ang;
    int sign = 0, i;

    dir.x = to[0] - from[0];
    dir.y = to[1] - from[1];
    dir.z = to[2] - from[2];
    hdir.x = dir.x;
    hdir.y = 0.0f;
    hdir.z = dir.z;
    horiz = (float)sqrt(dir.x * dir.x + dir.z * dir.z);
    d3_Normalize(&dir);
    d3_Normalize(&hdir);
    *outYaw = d3_VecAngle2((D3VECTOR *)&d3_ZAxis, (D3VECTOR *)&dir);
    drop = (float)fabs(to[1] - from[1]) * ai_MeterPerInch;
    horiz = ai_MeterPerInch * horiz;
    if (from[1] > to[1])
        sign = 1;
    else if (from[1] < to[1])
        sign = -1;
    ang = range[3];
    speed = range[0];
    for (i = 0; i < 16; i++) {
        s = (float)sin(ang);
        c = (float)cos(ang);
        h = s * s * speed * speed / (g * 2.0f);
        t = 1.0f / g * s * speed;
        if (sign == 1) {
            h = h + drop;
            t = (float)sqrt((h * (1.0f / g) + h * (1.0f / g))) + t;
        } else if (sign == -1) {
            h = h - drop;
            t = (float)sqrt((h * (1.0f / g) + h * (1.0f / g))) + t;
        }
        err = t * c * speed - horiz;
        if (err > 2.5f) {
            if (speed > range[0]) {
                speed = speed - speedStep;
                speedStep = speedStep * 0.5f;
            } else if (ang > range[2]) {
                ang = ang - angStep;
                angStep = angStep * 0.5f;
            } else
                return -1;
        } else if (err < ai_AimErrLow) {
            if (ang < range[3]) {
                ang = ang + angStep;
                angStep = angStep * 0.5f;
            } else if (speed < range[1]) {
                speed = speed + speedStep;
                speedStep = speedStep * 0.5f;
            } else
                return -1;
        } else {
            *outSpeed = speed;
            *outElev = ang;
            outDir[0] = hdir.x;
            outDir[1] = c * hdir.y - s * hdir.z;
            outDir[2] = s * hdir.y + c * hdir.z;
            outUp[0] = d3_YAxis.x;
            outUp[1] = d3_YAxis.y * c - d3_YAxis.z * s;
            outUp[2] = d3_YAxis.z * c + d3_YAxis.y * s;
            outSide[0] = outUp[2] * outDir[1] - outDir[2] * outUp[1];
            outSide[1] = outDir[2] * outUp[0] - outUp[2] * outDir[0];
            outSide[2] = outDir[0] * outUp[1] - outDir[1] * outUp[0];
            return 0;
        }
    }
    return -1;
}
float ai_ShotDrag = 1.5f;  /* 0x62de58, the drag coefficient of a cannon ball (ai_StepShot) */
float ai_ShotHitHeight = 98.4252014f;  /* 0x62de5c, 2.5 m in inches: a ball within this height above the ground hits it (ai_ShotHitsGround) */
EFFECTREC ai_DefaultEffect = { { 0.0f, 0.0f, 1.0f }, { 25.0f } };  /* 0x62de60, the default effect: the vector (0, 0, 1) and the scale 25.0 (0x62de6c) */
EFFECTSTATE ai_EffectState;  /* 0x6f3300 .. 0x6f34f0, the wind state ai_InitWind builds */
