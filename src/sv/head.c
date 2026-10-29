/* sv.c -- the screen / message box unit (0x520ec0 ..): the tag "sv_" ("sv_ChangePlayerHE" 0x624120, "sv_HandleObjects" 0x6242c4, "f3_sv:OpenBook" 0x624428, "sv_NetworkSync"), the message boxes ("misc\\Messagebox" 0x60ea68,
   "misc\\MessageBoxGreen" 0x623ed0), the groundplan bitmaps, the city / time panels, the pause / player bar, the book. Starts at 0x520ec0 (a tick callback, registered by 0x520ee0). Names are ours. ROUGH drafts.
   unit of rea 8cef7d5b (src/game/sv.c); boundaries in decl/text.dcl, notes in src/sv/open.md */
struct CHARACTER;
struct D2SHP;
struct D2SURF;
struct D3OBJECT;
struct D3TRAIN;
struct SBSAMPLE;
struct VFSENTRY;
struct VFSDIR;
extern char d2_Radios[];
extern char d2_Views[];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include <d3d.h>
#include "gv.h"
#include "d3.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "tb.h"
#include "evt.h"
#include "data_views.h"
#define ALIGN1 __declspec(align(1))   /* a 1-aligned container: the original packs the block at an odd address */
extern ALIGN1 char ai_Unk623ba9[277];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char d2_BackSurf;
extern unsigned char d2_CutsceneActive;
extern unsigned char d3_ViewW;
extern unsigned char d3_ViewX;
extern unsigned char d3_ViewH;
extern unsigned char d3_ViewY;
extern GAPDATA full_data_623e84[];
extern GAPDATA full_data_6f2354[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_76631c[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_75ec98[];
extern GAPDATA full_data_75ec9c[];
extern unsigned char ai_Unk623ac0;
extern char d3_AnimHead[];
extern GAPDATA full_data_6238e8[];
extern GAPDATA full_data_6238ec[];
extern GAPDATA full_data_6238f0[];
extern GAPDATA full_data_623934[];
extern GAPDATA full_data_623938[];
extern GAPDATA full_data_62393c[];
extern GAPDATA full_data_7604e8[];
extern const unsigned char ai_Unk5ebb74;
extern char ai_PlayerSlots[];

extern unsigned char ai_OptionPanelHelp;
extern char ai_PlayerSlots[];
extern unsigned char ai_TimeHour;
extern unsigned char d2_HitUnderPointer;
extern unsigned char d2_ShpH;
extern unsigned char d2_ShpW;

extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern char d2_Wins[];
extern GAPDATA full_data_623e90[];
extern GAPDATA full_data_625718[];
extern GAPDATA full_data_62571c[];
extern GAPDATA full_data_6f22dc[];
extern GAPDATA full_data_6f2364[];
extern GAPDATA full_data_6faa60[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_6f2338[];
extern GAPDATA full_data_76631c[];
extern unsigned char ai_Unk7603a0;
extern GAPDATA full_data_75ec80[];
extern GAPDATA full_data_75ec84[];
extern unsigned char ai_TimeHour;
extern unsigned char txt_Keys;
extern GAPDATA full_data_6f2294[];
extern GAPDATA full_data_6f2320[];
extern GAPDATA full_data_6f23bc[];
extern GAPDATA full_data_6f23c0[];
extern GAPDATA full_data_6f23c4[];
extern GAPDATA full_data_6f23c8[];
extern GAPDATA full_data_6f23cc[];
extern GAPDATA full_data_6f23d0[];
extern GAPDATA full_data_6f23d4[];
extern GAPDATA full_data_6f23d8[];

extern unsigned char ai_Unk6f22e8;
extern unsigned char ai_Unk6f22f8;
extern unsigned char ai_Unkf8aae0;
extern unsigned char d2_HitUnderPointer;
extern unsigned char d2_PointerSprite;



extern GAPDATA full_data_623e98[];
extern GAPDATA full_data_623e9c[];
extern GAPDATA full_data_6ed9c8[];
extern GAPDATA full_data_6f1c4c[];
extern GAPDATA full_data_6f22f0[];
extern GAPDATA full_data_6f22f4[];
extern GAPDATA full_data_6f2300[];
extern GAPDATA full_data_6f2320[];
extern GAPDATA full_data_6f2324[];
extern GAPDATA full_data_6f2328[];
extern GAPDATA full_data_6f232c[];
extern GAPDATA full_data_6f23dc[];
extern int ai_CameraStep;  /* 0x623814, the camera move step (100 - camera speed); sv.c converts it with fild at the float uses */
extern unsigned char ai_ClickedObject;
extern unsigned char ai_Unk6f22e8;
extern unsigned char ai_Unk6f22f8;
extern unsigned char d2_HitUnderPointer;
extern GAPDATA full_data_6f22a8[];
extern GAPDATA full_data_6f22ec[];
extern GAPDATA full_data_6f22f0[];
extern GAPDATA full_data_6f22f4[];
extern GAPDATA full_data_6f22fc[];
extern GAPDATA full_data_6f2300[];
extern GAPDATA full_data_6f2338[];
extern GAPDATA full_data_6f23e0[];
extern GAPDATA full_data_760458[];
extern GAPDATA full_data_76045c[];
extern GAPDATA full_data_760460[];
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk7603a0;
extern unsigned char d3_Unk6333fc;
extern GAPDATA full_data_6237c8[];
extern GAPDATA full_data_75ec84[];
extern char gm_PlayerRecords[];
extern unsigned char hst_KeyNext;
extern unsigned char txt_GroupLevel;

extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */

extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */

extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;



extern GAPDATA full_data_623240[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char d2_HalfScreenH;
extern unsigned char d2_HalfScreenW;
extern GAPDATA full_data_6f2320[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char d3t_CamMoving;
extern GAPDATA full_data_6f22a8[];
extern GAPDATA full_data_6f22ec[];
extern unsigned char ai_Unk6f2318;
extern GAPDATA full_data_6f22fc[];
extern GAPDATA full_data_6f2320[];
extern GAPDATA full_data_6f2338[];
extern GAPDATA full_data_6f23e4[];
extern GAPDATA full_data_6f23e8[];
extern int snd_SystemBank;
extern GAPDATA full_data_6f2410[];
extern GAPDATA full_data_6f262c[];
extern GAPDATA full_data_6f2630[];
extern GAPDATA full_data_6faa80[];
extern GAPDATA full_data_6faa84[];
extern unsigned char he_Events;
extern unsigned char ai_GameSwitches;
extern unsigned char ai_Unkf8aae0;
extern unsigned char d2_MouseX;
extern unsigned char d2_MouseY;
extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern unsigned char d2_TextLeft;
extern unsigned char d2_TextTop;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char ai_Unk5fe708;
extern unsigned char d2_HalfScreenW;
extern unsigned char d2_TextDrawnH;
extern unsigned char d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char d3s_QuadUV;
extern GAPDATA full_data_6f1c90[];
extern GAPDATA full_data_6f23ec[];
extern unsigned char d2_Pics;
extern GAPDATA full_data_760444[];
extern GAPDATA full_data_760448[];
extern GAPDATA full_data_76044c[];
extern GAPDATA full_data_760450[];
extern unsigned char ai_Unk7603a0;
extern GAPDATA full_data_6f240c[];
extern GAPDATA full_data_6f23f0[];
extern GAPDATA full_data_6f25b4[];
extern GAPDATA full_data_75ec40[];
extern unsigned char main_Window;
extern unsigned char ai_Unk75fba0;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_623ebc[];
extern unsigned char ai_Unk75fba0;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk623ac0;
extern unsigned char d3_Sky;
extern GAPDATA full_data_6239d8[];
extern unsigned char d3t_SunRays;
extern GAPDATA full_data_6f2408[];
extern GAPDATA full_data_7604e8[];
extern unsigned char ai_OptionSpeed;
extern unsigned char ch_Unk5f1290;
extern GAPDATA full_data_6f22ac[];
extern unsigned char d2_HalfScreenH;
extern unsigned char d2_HalfScreenW;
extern unsigned char ai_GameFlags;
extern unsigned char ai_GameOver;
extern unsigned char ai_MissionPartner;

extern unsigned char ch_CurrentGroup;
extern unsigned char ai_Unk633738;
extern unsigned char ai_Unk6f0cd4;
extern unsigned char ai_Unk6f0da4;
extern unsigned char ai_Unk6f0dbc;
extern unsigned char ai_Unk6f22e8;
extern unsigned char ai_Unk6f2448;
extern unsigned char ai_Unk75e130;
extern unsigned char ai_Unka687e4;
extern unsigned char ai_Unkf31144;
extern unsigned char cm_RequestedSaveName;
extern unsigned char cm_RequestedSaveSlot;
extern unsigned char d2_CutsceneActive;
extern unsigned char d2_FocusedInput;
extern unsigned char d2_HitUnderPointer;
extern unsigned char d2_MouseX;
extern unsigned char d2_MouseY;
extern unsigned char d3_FrameCount;


extern GAPDATA full_data_623eac[];
extern GAPDATA full_data_623eb0[];
extern GAPDATA full_data_623eb4[];
extern GAPDATA full_data_62457c[];
extern GAPDATA full_data_6f22ac[];
extern GAPDATA full_data_6f22f0[];
extern GAPDATA full_data_6f23f1[];
extern GAPDATA full_data_6f2580[];
extern GAPDATA full_data_6f2584[];
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f262c[];
extern unsigned char snd_MusicEnabled;
extern unsigned char snd_SfxEnabled;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern unsigned char ai_GameFlags;
extern unsigned char ai_OptionSpeed;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk75e130;
extern unsigned char ai_Unk75e134;
extern GAPDATA full_data_6248b4[];

extern int ai_Unk75f280[576];
int d2_CreateWinText(short x, short y, char *text, int win);
int d2_WinOpenPic(short x, short y, int pic, int win);
void d2_SetObjWidth(int obj, short w);
void d2_BlitPart();
void d2_EnableObject();
void d2_RemoveObj();
void d2_SetObjEnabled();
void d2_SetObjFont();
void d2_SetObjWidth(int obj, short w);
void sv_UpdateClock(void);
void sv_Unk52d010(char *part);
void sv_Unk52eed0(void);
void sv_Unk52ee50(void);
void sv_Unk52eb50(int x, int y, int a, int b, unsigned k);
void sv_Unk52eaa0(int x, int y, float len, int k);
void sv_ClearSelection(void);
void sv_Unk529fb0(void);
void sv_Unk522f10(unsigned char a, int b);

struct G16CHARCLICK;  /* file-scope tag for the prototypes below */
struct GCLOCK;        /* f3gm.c typedef; file-scope tag for the gm_AddTime / gm_SetTime prototypes */
struct D3SURF;        /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */

extern int sv_TickFrames;
extern float sv_FramesPerTick;
extern int sv_SelectionFlags[0xc0];
extern int cm_Bombs[0x40];
extern char ch_EmptyName[];             /* 0x631090 */
extern int di_Absolute, di_MinX, di_MinY, di_MaxX, di_MaxY;  /* 0x5f3240, 0x6310b0, 0x6310b4, 0x5f3238, 0x5f323c (di_input.c) */
extern short d2_ScreenW, d2_ScreenH;    /* 0xfb9072, 0xfb9070 (d2.h), the screen size */
extern unsigned sv_FrameMask;           /* 0x760638 */
extern char sv_MessageBoxOpen, sv_MessageBoxGreenOpen, sv_MessageBoxYesNoOpen;  /* 0x6f2350, 0x6f2351, 0x6f2352 (the yes / no box) */
extern int d2_LastWindow, sv_PopRequest;  /* 0x6ed938, 0x6f22a0 */
extern char *d2_LastWindowPtr;  /* 0x6ed980, window created last; @original: d2.h declares D2WIN *; the units add byte offsets to it and cannot include d2.h (vfs_Root / vfs_Tfopen clash with vfs.h) */
extern int d2_ClickedId, d2_HitWindow;  /* 0xf8a668, 0xf8aad0 */
extern unsigned char kb_Last;  /* 0x1468410 */
/* d2_RadioSel: view of d2_Radios (tools/data_views.py) */
extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and cannot include d2.h (vfs_Root / vfs_Tfopen clash with vfs.h) */

void d2_SetPointerSprite(int a, int pic);      /* 0x476f80, gfx.c */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
void sv_SetStatusText(char *s);                     /* 0x52e330 */
void d2_CopyCaption(char *caption);            /* 0x478870, gfx.c */
char *gm_FindCharacterByMaster(int master);    /* 0x50a810, f3gm.c */
int gm_SumSlotAmounts(void *list, unsigned char i);                     /* 0x508660 */
int d2_OpenForm(int x, int y, char *name);                     /* 0x4741f0 */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
void d2_CentreViewWindows(int view);            /* 0x475600 */
int d2_CreateRadioGroup(int n, ...);                     /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                     /* 0x46c560 */
void d2_RadioSelect(int g, int sel);                     /* 0x46c630 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);                     /* 0x46a960 */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */
void d2_MenuNavigate(int g);                     /* 0x46c6c0 */
void d2_RaiseViewWindows(int view);                     /* 0x4741a0 */
void d2_SetObjValue();                     /* 0x475cb0 */
void d2_RadioClear(int g);                     /* 0x46c600 */
void d2_CloseView(int view);            /* 0x4758f0 */
void sv_FitForm(int f);                     /* 0x52ea30 */
extern int ai_Unk623820, ai_Unk623824, ai_Unk623828, ai_Unk62382c, ai_Unk623830, ai_Unk623834, ai_Unk623838, ai_Unk62383c;  /* 0x623820, 0x623824, 0x623828, 0x62382c, 0x623830, 0x623834, 0x623838, 0x62383c */
extern int ai_Unk623840, ai_Unk623844, ai_Unk623848, ai_Unk62384c, ai_Unk623850, sv_PanelPic, sv_PanelForm, ai_Unk6238a4;  /* 0x623840, 0x623844, 0x623848, 0x62384c, 0x623850, 0x623854, 0x623860, 0x6238a4 */
extern int ai_Unk6237cc, ai_Unk623e88;  /* 0x6237cc, 0x623e88 */
extern int sv_PlayerBarForm;
extern int ai_Unk7662e0[15];
/* ai_Unk7e4048: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4058: view of ai_PlayerSlots (tools/data_views.py) */
extern int ai_Unk6f22c0, ai_Unk6f22c4, ai_Unk6f22c8, ai_FrameWait;  /* 0x6f22c0, 0x6f22c4, 0x6f22c8, 0x6f22bc */
extern int sv_FrameDepth;
extern int sv_FrameBase;
extern char ai_Unk6f0454;  /* 0x6f0454 */
extern int sv_ViewRectLeft, sv_ViewRectTop, sv_ViewRectRight, sv_ViewRectBottom, ai_Unk6ed92c;  /* 0x6248a4, 0x6248a8, 0x6248ac, 0x6248b0, 0x6ed92c */
extern int ai_Unkfb9060, ai_Unkfb9064, ai_Unkfb9068, ai_Unkfb906c;  /* 0xfb9060, 0xfb9064, 0xfb9068, 0xfb906c */
extern char ai_GamePanel;  /* 0x6f3668 */
extern char *ai_PlayerBuilding, *ai_FallbackBuilding;  /* 0x6f2308, 0x6f230c */
extern unsigned short * sv_PanelShown;
extern int sv_PanelFlag;
extern char * sv_PanelObject;
extern unsigned short * sv_PanelRecord;
extern char sv_PanelText[76];
extern short d2_PointerLook;
/* ai_Unk6f0455: view of ai_Unk6f0454 (tools/data_views.py) */
extern unsigned short ai_PlayerId;  /* 0x6f25b8 */
/* the object / character record fields sv.c reads (AIBLD / AICHAR in ai.h; ai.h cannot be included here: its
   declarations of ai_Time / ai_Chars / ai_BTypes / ai_Blds clash with gv.h's char views). Views, as gv.h's ORDCHAR */
#pragma pack(push, 1)
typedef struct SVOBJREC {
    unsigned char kind;                 /* +0x00, index into ai_BTypes (0x47: a building, 4: an object of the player) */
    char pad01[0x28];
    short sceneRec;                     /* +0x29, the scene record word the panel shows for the object */
    char pad2b[0x32];
    int objList;                        /* +0x5d, head of its object-instance list (gm_Query's list) */
    char pad61[4];
    int ownerId;                        /* +0x65, the character id of a type-4 object */
} SVOBJREC;
typedef struct SVPLATE {                /* 200 bytes, a name plate record: the object it belongs to */
    char *obj;                          /* +0x00, the object that shows the plate, 0 = free */
    char pad04[0xc4];
} SVPLATE;
typedef struct SVPLATEOBJ {             /* the 3D object fields the plate code clears */
    char pad00[0x224];
    int plate;                          /* +0x224, the name plate attached to the object */
} SVPLATEOBJ;
typedef struct SVGROUP {                 /* the group record fields sv.c reads (the 8 member slots) */
    char pad00[0x8c];
    int member[8];                      /* +0x8c, the character ids of the members, -1 = a free slot */
} SVGROUP;
typedef struct SVCHARREC {
    unsigned short id;                  /* +0x00 */
    char pad02[2];
    unsigned int netId;                 /* +0x04, the id the order events carry (AICHAR.netId) */
    char pad08[0x170];
    int inventory;                      /* +0x178, the character's goods inventory (AICHAR.inventory) */
    char pad17c[0x18];
    int money;                          /* +0x194, the character's money (AICHAR.money) */
    char pad198[0x19];
    unsigned char flag1b1;              /* +0x1b1, nonzero: the info panel stays closed for the character */
} SVCHARREC;
#pragma pack(pop)
extern int sv_BubbleObjects[4286];
extern SVPLATE sv_NamePlates[0x20];
extern char sv_StatusText[136];
extern int sv_StatusTime;

int gm_IsHomeType(unsigned char *b);     /* 0x502520, f3gm.c */
int gm_IsType10(unsigned char *b);       /* 0x5024f0, f3gm.c */
int init_ObjectSceneUniverse(unsigned short *p);     /* 0x511dc0, f3init.c */
void init_LoadObjectScene(int n);        /* 0x50d7f0, f3init.c */
void sv_PopTo(int v);                     /* 0x52fb50 */
void ai_GameFrame(void);                     /* 0x591520 */
int d2_DrawMousePointer(int x, int y);         /* 0x476f00, gfx.c */
void d2_SetClipRect(int x0, int y0, int x1, int y1);                     /* 0x47cb40 */
int d2_AddHitRect();                     /* 0x478890, @original: sv.c's callers were compiled against the non-prototype declaration (sv_BuildScreenLayout passes an int d2_ScreenW - 0x50 without a short conversion, 464 bytes vs 480) */
int d2_CreateWindow();                     /* 0x472390 */
void d2_PlaceWindowScaled();                     /* 0x475730 */
void mem_Free(void *p);                     /* 0x422620 */
void * d2_GetPic();                     /* 0x4690b0 */
int d2_LockSurface();                     /* 0x479020 */
int d2_ClearSurface();                     /* 0x4796c0 */
int d2_DrawShape(int x, int y, void *surf, void *pic, unsigned char i);                     /* 0x47c6f0 */
struct D2SHP * d2_GrabShape(int x, int y, int w, int h, struct D2SURF *surf, unsigned char mode, char grab8);                     /* 0x47b220 */
int d2_UnlockSurface();                     /* 0x4790e0 */
struct D2SURF * d2_MakeSurface(struct D2SURF *desc, int flags);                     /* 0x478cc0 */
int d2_CreatePicObj();                     /* 0x46cd80 */
void d2_EnableObject();                     /* 0x475aa0 */
int gm_TitleGroup();                     /* 0x504ad0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void he_Unk53dad0();                     /* 0x53dad0 */
void d2_RemoveObj();                     /* 0x46e710 */
int d2_FreeSurface(struct D2SURF *s);                     /* 0x478fc0 */
void d2_RemoveHitRect();                     /* 0x4788f0 */
int d2_RemoveWindow();                     /* 0x472ce0 */
int sv_MessageBox(int text, unsigned flags);    /* 0x521150 */
int sv_MessageBoxGreen();               /* 0x521410 */
int sv_MessageBoxYesNo();               /* 0x5216a0 */

#include "time_view.h"

