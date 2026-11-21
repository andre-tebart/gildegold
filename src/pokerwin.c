/* pokerwin -- the gypsy's poker windows (0x5d9de0 .. 0x5dbc10), split from menu; the name is ours.
   Start: this object's plain .data block ai_PokerUi 0x62dda0 follows menu's last string (ai_GypsyProductionContact's
   "contact_ZUTATEN_SUCHEN_ZIGEUNER" 0x62dd7c); its first reader ai_OpenPokerWindow. Its common group is ai_PokerState
   (0x6f3500, between shot's ai_EffectState 0x6f3300 and menu's ai_Options 0x6f35c0: the groups in reverse link order).
   bprobe at 0x5d9c30 / 0x5d9de0 / 0x5d9f50: no change.
   unit 0x005d9de0..0x005dbc10; boundaries in decl/text.dcl */
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
#include "menu.h"
#include "pokerwin.h"
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
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
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
void d2_PlaceWindowScaled(int win, int flags); /* 0x475730, d2.c */
int d2_RemoveWindow(int win);           /* 0x472ce0, d2.c */
int io_PrintText(char *fmt, ...);          /* 0x515960, io_text.c: prints a text (id or literal) with its
                                           tokens into the current window and redraws it */
extern short d2_ScreenW;                /* 0xfb9072, d2.h */
extern short d2_ScreenH;                /* 0xfb9070, d2.h */
extern struct D3RECT d2_DirtyRect;         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
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
extern int ai_Opponents[7];
extern char ai_OptionsFile[288];
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
void d3_ApplyGraphicsOptions(unsigned char a, unsigned char b, unsigned char c, unsigned char mode, int e); /* 0x43c4e0 */
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

POKERGAME ai_PokerState;  /* 0x6f3500 .. 0x6f35c0, the poker game state (poker.c steps it) */
/* ai_TextLoadGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextSaveGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGameOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGfxOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextSfxOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextLeaveGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextBack: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_QuickSaveNoPlayerText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_QuickSaveDisabledText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextBloodRevenge: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextCurse: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTheatre: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextHomage: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextChat: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TailorBlockedText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TailorAmountText: view of txt_Ptrs (build/mk_txt_views.py) */
POKERUI ai_PokerUi = { -1 };  /* 0x62dda0, the poker window state (window -1 when closed, the rest zero) */

/* 0x5d9de0, opens the poker table window "Locations\zigeuner\poker" 0x62de10 (273 B): returns 0 when a window is open
   already (ai_PokerUi.window != -1) or the form cannot be opened, else 1. The pointer sprite is cleared, the view
   centred, and the panels are initialised from the windows the forms 1, 2 and 3 made (each takes d2_LastWindow and clears its
   five slots to -1; the form 3 panel, first in memory, is marked shown); the form 6 row of five buttons (the texts 0x144aae8 ..
   0x144aaf8, ids into ai_PokerUi.buttons) gets its third button disabled. The poker family 0x5d9de0 ..
   0x5db110 (the data block 0x62dda0 .. 0x62de10 is one POKERUI); betSlider (0x62ddb8) starts at -1. */

int ai_OpenPokerWindow(void)
{
    if (ai_PokerUi.window != -1)
        return 0;
    ai_PokerUi.window = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker");
    if (ai_PokerUi.window == -1)
        return 0;
    ai_PokerUi.betSlider = -1;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(ai_PokerUi.window);
    d2_SetForm(ai_PokerUi.window, 1);
    ai_PokerUi.panel[1].win = d2_LastWindow;
    memset(ai_PokerUi.panel[1].slot, 0xff, sizeof(ai_PokerUi.panel[1].slot));
    ai_PokerUi.panel[1].shown = 0;
    d2_SetForm(ai_PokerUi.window, 2);
    ai_PokerUi.panel[2].win = d2_LastWindow;
    memset(ai_PokerUi.panel[2].slot, 0xff, sizeof(ai_PokerUi.panel[2].slot));
    ai_PokerUi.panel[2].shown = 0;
    d2_SetForm(ai_PokerUi.window, 3);
    ai_PokerUi.panel[0].win = d2_LastWindow;
    memset(ai_PokerUi.panel[0].slot, 0xff, sizeof(ai_PokerUi.panel[0].slot));
    ai_PokerUi.panel[0].shown = 1;
    d2_SetForm(ai_PokerUi.window, 6);
    sv_ButtonRow(d2_LastWindow, 0, ai_PokerUi.buttons, 5, txt_Ptrs[0x424a], txt_Ptrs[0x424b], txt_Ptrs[0x424c], txt_Ptrs[0x424d], txt_Ptrs[0x424e]);
    d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
    return 1;
}
/* 0x5d9f50, shows the poker bet slider (159 B): arg on, nothing happens for 0. The slider is made once, on the first
   call, in the form 5 of the poker window (ai_PokerUi.window): d2_WinOpenSlider, as wide as the window less 0xb4, 0x7b
   picture, flags 2, scale 0 .. 100) and stored in ai_PokerUi.betSlider; its range is the bet limits converted to the
   player's money (gm_ConvertMoney of ai_PokerState.minBet, ai_PokerState.maxBet and ai_PokerState.minBet again as the start value, step 1).
   Later calls enable / disable the slider (d2_EnableObject). The limits are the poker game's POKERGAME.minBet / maxBet (0x6f35a4 / 0x6f35a8). */

void ai_PokerBetControl(int on)
{
    int slider;

    if (on != 0) {
        if (ai_PokerUi.betSlider == -1) {
            d2_SetForm(ai_PokerUi.window, 5);
            slider = d2_WinOpenSlider(0, 0, d2_Wins[d2_LastWindow].w - 0xb4, 0, 100, 0x7b, 2, d2_LastWindow);
            ai_PokerUi.betSlider = slider;
            d2_SetObjValue(ai_PokerUi.betSlider, gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.maxBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), 1);
        } else {
            d2_EnableObject(ai_PokerUi.betSlider, on);
        }
    }
}
/* 0x5da010, closes the poker window (24 B): closes the window's view (ai_PokerUi) and clears
   the handle (-1). */

void ai_ClosePokerWindow(void)
{
    d2_CloseView(ai_PokerUi.window);
    ai_PokerUi.window = -1;
}

typedef struct PLAYERRECVIEW {          /* f3gm.h's player record (gm_PlayerRecord, 0x56 words): only the float the poker game reads */
    char pad00[0x60];
    float luck;                         /* +0x60, the chance that the player sits down to play (decays by * 0.66 - 0.1 per game; reasoned) */
} PLAYERRECVIEW;

/* 0x5da030, the poker game (the card game against two opponents) of the gypsy poker table (2752 B). The player's record
   (gm_PlayerRecord) decides whether he plays: the record's float at +0x60 must exceed a random number, it must be past
   11 o'clock, and the player's worth (gm_GetWorth, converted to the player's money) at least 10000. Then the float is
   decayed (* 0.66 - 0.1), the key state 0x1e is cleared, the poker window (ai_OpenPokerWindow) opened and the game started
   (ai_PokerStart fills ai_PokerState; 0 closes the window and ends). Otherwise the form "Misc\Messagebox" shows the text
   0x4239 (with 0x4238) until the right button or the key 1 pops it. Returns 1 when a game was played, else 0.
   The pump runs sv_RunFrame (this function, 0x777c7); every frame the buttons follow the mask the game step
   (ai_PokerStep) returns: bit 1 the first button (ai_PokerUi.buttons[0]), bit 8 the second, bit 0x10 the fourth; bits 1 and
   0x20 show the bet slider (ai_PokerBetControl). The second button asks (text 0x4263, message box 0x101) and leaves, the
   fifth opens the rules window ai_PokerWindow2 with the poker view disabled. Then ai_PokerState.phase (1 .. 11) selects the
   step: the seats are redrawn with ai_PokerSeatPanel (mode 0 .. 5) when the phase changes, the pot text 0x4252 and a random
   remark 0x4253 .. 0x4255 (with the hand text 0x4256 / 0x4257 / 0x4258) are printed, the first button takes the bet from the
   slider (gm_PlayerFactor of its value) and steps the game; phase 6 lets the player mark one of the first two cards to
   exchange (the objects' checked flag, ai_Unk4a8630), phase 7 shows the two opponents' texts (0x425c when the seat put a card away in the exchange, else 0x425d: seat dropped),
   phase 10 shows the result (the winner's names and the hand text 0x4266 + the rank), phase 11 waits for the fourth button.
   Reasoned: the button roles and the phase meanings follow from the texts and the mask bits; the text ids are not read
   from the text files. */

int ai_PokerGame(void)
{
    char msg[0x100];
    char text[0x100];
    int lastPhase;
    unsigned short mask;
    PLAYERRECVIEW *rec;
    int worth, form;
    char *s, c;
    int d;

    lastPhase = -1;
    rec = (PLAYERRECVIEW *)gm_PlayerRecord(ai_CharsC + ai_PlayerId * 0x21c);
    if (rec == 0)
        return 0;
    worth = gm_ConvertMoney(gm_GetWorth(ai_PlayerId), ai_OwnPlayer);
    if (util_RandomF() < rec->luck && ai_Time.hour > 11 && worth >= 10000) {
        rec->luck = rec->luck * 0.66f - 0.1f;
        kb_Down[0x1e] = 0;
        if (ai_OpenPokerWindow() == 0)
            return 0;
        if (ai_PokerStart((struct AIPER *)(ai_CharsC + ai_PlayerId * 0x21c), (struct G10GAME *)&ai_PokerState) == 0) {
            ai_ClosePokerWindow();
            return 0;
        }
        mask = ai_PokerStep();
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            d2_SetObjEnabled(ai_PokerUi.buttons[0], mask & 1);
            d2_SetObjEnabled(ai_PokerUi.buttons[1], mask & 8);
            d2_SetObjEnabled(ai_PokerUi.buttons[3], mask & 0x10);
            ai_PokerBetControl((mask & 1) != 0 && (mask & 0x20) != 0);
            if (d2_ClickedId != -1) {
                if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                    txt_Format(msg, 0x4263);
                    if (sv_MessageBox((int)msg, 0x101) != 0)
                        sv_PopRequest = 1;
                }
                if (ai_PokerUi.buttons[4] == d2_PointerObj) {
                    d2_SetViewEnabled(ai_PokerUi.window, 0);
                    ai_PokerWindow2();
                    d2_SetViewEnabled(ai_PokerUi.window, 1);
                }
            }
            switch (ai_PokerState.phase) {
            case 1:
                if (lastPhase != 1) {
                    lastPhase = 1;
                    ai_PokerSeatPanel(1, 0, 0);
                    ai_PokerSeatPanel(2, 0, 0);
                    ai_PokerSeatPanel(0, 0, 0);
                }
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.actor == 0) {
                    d = text - (s = txt_Ptrs[0x4250]);
                    do {
                        c = *s;
                        s[d] = c;
                        s++;
                    } while (c != 0);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[0] == d2_PointerObj) {
                        ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                    }
                } else {
                    sprintf(text, txt_Ptrs[0x424f], ai_PokerState.seat[ai_PokerState.actor].who->vname, ai_PokerState.seat[ai_PokerState.actor].who->nname);
                    io_PrintText(text);
                    txt_Format(text, 0x4251, ai_PokerState.minBet);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                        mask = ai_PokerStep();
                }
                break;
            case 2:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_Random(3) + 0x4253, 0x4256);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 3, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 3, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 3, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 3:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 4:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_Random(3) + 0x4253, 0x4257);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                if (lastPhase != 4) {
                    lastPhase = 4;
                    ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                    ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                }
                mask = ai_PokerStep();
                break;
            case 5:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 6:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                io_PrintText(txt_Ptrs[0x425b]);
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 3;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 3;
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.panel[0].slot[0] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = d2_Objs[ai_PokerUi.panel[0].slot[0]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.panel[0].slot[1] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = d2_Objs[ai_PokerUi.panel[0].slot[1]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.buttons[2] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            ai_PokerExchange(0);
                        else if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            ai_PokerExchange(1);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[3] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 7:
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 0;
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                if (ai_PokerState.seat[1].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                }
                if (ai_PokerState.seat[2].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                }
                if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                    mask = ai_PokerStep();
                break;
            case 8:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_Random(3) + 0x4253, 0x4258);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 9:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 10:
                ai_PokerStep();
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.winner == 0)
                    txt_Format(text, 0x4265, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                else
                    txt_Format(text, 0x4264, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->vname, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->nname, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                io_PrintText(text);
                break;
            case 11:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[3] == d2_PointerObj)
                        sv_PopRequest = 1;
                }
                break;
            }
        } while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0);
        ai_ClosePokerWindow();
        return 1;
    } else {
        form = d2_OpenForm(0, 0, "Misc\\Messagebox");
        if (form == 0)
            return 0;
        d2_CentreViewWindows(ai_PokerUi.window);
        txt_Format(text, 0x4239, 0x4238);
        io_PrintText(text);
        while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0) {
            if (dm_L.rup != 0 || kb_Down[1] != 0)
                sv_PopRequest = 1;
        }
        d2_CloseView(form);
        return 0;
    }
}
/* 0x5dadf0, builds the panel of one poker seat (445 B): seat 0 is the player, 1 and 2 the opponents; n the number of
   cards to show, cards the seat's card array (ai_PokerState.seat[seat].card, indices into the deck). The panel's window
   (ai_PokerUi.panel[seat].win) becomes the current one and its text is cleared; the seat's title and names are printed
   with "%s %s %s" (the title text txt_Ptrs[0x114 + 10 * female + title], the first and the family name) into a centred
   label (font 0x54) at the window's top; the character's picture (the player's own, or the seat's model) is placed left
   of the cards. Then each of the n cards becomes a picture object 0x20 apart, stored in the panel's slot i: the card
   back (picture 0x775 + ai_CardBack) when the seat has folded (foldPhase >= 0), or while an opponent's first two
   cards are shown at the end of the game (winner 4); else the card's face (0x775 + the card index), and the player's
   first two cards are greyed (they are chosen for the exchange in phase 6). Void. Reasoned: the picture and text
   roles follow from the offsets (0x775 + index into d2_Pics, the label with the centred flag). */

void ai_PokerSeatPanel(int seat, int n, int *cards)
{
    char text[0x100];
    POKERWHO *who;
    int o, pic, x, w, h, i;

    d2_LastWindow = ai_PokerUi.panel[seat].win;
    io_PrintText("$C");
    w = d2_Wins[d2_LastWindow].w;
    h = d2_Wins[d2_LastWindow].h;
    x = (w - 0xc4) / 2;
    who = ai_PokerState.seat[seat].who;
    sprintf(text, "%s %s %s", txt_Ptrs[(who->female != 0 ? 0x11e : 0x114) + who->title], who->vname, who->nname);
    o = d2_CreateWinText(0, 5, text, ai_PokerUi.panel[seat].win);
    d2_Objs[o].w = w;
    d2_Objs[o].centred = 1;
    d2_SetObjFont(o, 0x54);
    if (seat != 0) {
        pic = ai_PokerState.seat[seat].model;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_Pics[pic].h / 2, pic, ai_PokerUi.panel[seat].win);
    } else {
        pic = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->pic;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_Pics[pic].h / 2, pic, ai_PokerUi.panel[0].win);
    }
    for (i = 0; i < n; i++, x += 0x20) {
        if (ai_PokerState.seat[seat].foldPhase >= 0) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else if (seat != 0 && i < 2 && ai_PokerState.winner == 4) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[cards[i] + 0x775].h / 2, cards[i] + 0x775, ai_PokerUi.panel[seat].win);
            if (seat == 0 && i < 2)
                d2_Objs[ai_PokerUi.panel[0].slot[i]].greyed = 1;
        }
    }
}
/* 0x5db110, the second poker window "Locations\\zigeuner\\poker2" (the poker hands table, opened by the fifth button of the poker
   table 0x5da030 with its view disabled; 2806 B of straight-line picture rows). Opens the form (-1: nothing), clears
   the pointer sprite, centres the view and shows the title text 0x423e; the form 3 gets a button row of one (the text
   0x4249) whose id ends the window; the form 4 gets a slider (sv_CreateScrollButtons, 0x35) in the form 2 window. In the form 2 the
   texts 0x4247 / 0x4248 are printed and the table of the eight poker hands (the texts 0x423f .. 0x4246, the hand names
   "Paar" .. "Royal Flush") is built straight-line: row i at y = 0xa0 + 0x6d * i has five card pictures 0x20 apart (x = 5,
   0x25, 0x45, 0x65, 0x85; the pictures 0x775 + n: the ones that make the hand are highlighted, the others greyed) and the
   hand's name as a centred text label at x = 0xcb, 0x29 below the row (font 0x54, as wide as the window less 0xd2). The
   window's content height is raised by 8, then the pump (this function, 0x777c7) waits for the right button or the button
   and closes the view. Void. Reasoned: the rows are read off the pictures and texts (the hand rows follow the text order
   0x423f .. 0x4246); which picture number is which card is not decoded. */

void ai_PokerWindow2(void)
{
    int ids[1];
    int a, b, c;
    int form, win, o;
    int w;

    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker2");
    if (form == -1)
        return;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 1);
    io_PrintText((char *)0x423e);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 1, txt_Ptrs[0x4249]);
    d2_SetForm(form, 2);
    win = d2_LastWindow;
    w = d2_Wins[d2_LastWindow].w - 0xd2;
    d2_SetForm(form, 4);
    sv_CreateScrollButtons(0, 0, d2_LastWindow, win, 0x35, &c, &b, &a);
    d2_SetForm(form, 2);
    io_PrintText((char *)0x4247);
    io_PrintText((char *)0x4248);
    o = d2_WinOpenPic(0x5, 0xa0, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0xa0, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0xa0, 0x77f, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x65, 0xa0, 0x784, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0xa0, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0xc9, txt_Ptrs[0x423f], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x10d, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x10d, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x10d, 0x785, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x10d, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x10d, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x136, txt_Ptrs[0x4240], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x17a, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x17a, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x17a, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x17a, 0x777, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0x17a, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x1a3, txt_Ptrs[0x4241], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x1e7, 0x775, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x1e7, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x1e7, 0x77f, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x1e7, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x1e7, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x210, txt_Ptrs[0x4242], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x254, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x254, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x254, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x254, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x254, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x27d, txt_Ptrs[0x4243], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x2c1, 0x777, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x2c1, 0x77b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x2c1, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x2c1, 0x787, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x2c1, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x2ea, txt_Ptrs[0x4244], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x32e, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x32e, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x32e, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x32e, 0x78c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x32e, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x357, txt_Ptrs[0x4245], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x39b, 0x778, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x39b, 0x77c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x39b, 0x780, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x39b, 0x784, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x39b, 0x788, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x3c4, txt_Ptrs[0x4246], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    d2_ClickedId = 1;
    d2_Wins[d2_LastWindow].contentHeight += 8;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == ids[0])
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PokerWindow2, 0x777c7) != 0);
    d2_CloseView(form);
}
