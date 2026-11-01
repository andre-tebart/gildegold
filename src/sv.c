/* sv.c -- the screen / message box unit (0x520ec0 ..): the tag "sv_" ("sv_ChangePlayerHE" 0x624120, "sv_HandleObjects" 0x6242c4, "f3_sv:OpenBook" 0x624428, "sv_NetworkSync"), the message boxes ("misc\\Messagebox" 0x60ea68,
   "misc\\MessageBoxGreen" 0x623ed0), the groundplan bitmaps, the city / time panels, the pause / player bar, the book. Starts at 0x520ec0 (a tick callback, registered by 0x520ee0); ends before
   0x531670, main.c's program level code. Names are ours.
   Open: the start and the end rest on the .data order only.
   Open: sv.c includes ai.h but not d2.h yet: its char * declarations of d2_Objs / d2_LastWindowPtr and its K&R
   d2_BlitPartMode supersede d2.h's, so the d2 records go through the SV* views and the SV_I / SV_W address macros.
   This blocks Q4 / Q7 of every function that touches widgets or calls a K&R d2 / gfx prototype.
   Open: head: include/gfx.h, include/d2.h unsettled (d2_Objs / d2_LastWindowPtr char *, d2_AddHitRect and
   d2_BlitPartMode K&R; 39 K&R d2 prototypes whose calls need a re-grade); also not included: ch.h cm.h
   f3gm.h f3snd.h rt.h sblib.h unk_5d33b0.h. Calls whose argument count differs from the definition (read
   the original's pushes): sb_PlaySample, d2_SetObjValue, d2_PlaceMainWindow, ch_FindByObject. The
   @original cast macros (d2_PutPixelRGBc, d2_CreatePicObjS, d2_WinOpenPicStyledS, ai_SwitchRoomI,
   cm_SendExSetHEI) record the parameter types the original's calls show.
   Open: the camera 0xfc2dec has two names: sv_CameraObj (char *, read by raw offsets in most functions) and
   d3.h's typed d3_Camera (the camera functions); likewise ai_Unk5fb5b0 (char *) for d3_DefaultScene.
   unit of rea 8cef7d5b (src/game/sv.c); boundaries in decl/text.dcl */
struct CHARACTER;
struct D2SHP;
struct D2SURF;
struct D3OBJECT;
struct D3TRAIN;
struct SBSAMPLE;
struct VFSENTRY;
struct VFSDIR;
#include "d2_types.h"
extern D2RADIO d2_Radios[];
extern D2VIEW d2_Views[];
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
#include "sv.h"
#define ALIGN1 __declspec(align(1))   /* a 1-aligned container: the original packs the block at an odd address */
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern void *d2_BackSurf;
extern GAPDATA full_data_623e84[];
extern GAPDATA full_data_6f2354[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_76631c[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_75ec98[];
extern GAPDATA full_data_75ec9c[];
extern char sv_SkyHeavy[];
extern const char ai_AxisPair[];

#include "unk_5d33b0_types.h"
#include "ai_types.h"
extern OPTIONBYTES ai_OptionPanelHelp;
extern char ai_PlayerSlots[4][0x2f4];
extern unsigned char ai_TimeHour;
extern int d2_HitUnderPointer;
extern short d2_ShpH;
extern short d2_ShpW;

extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern D2WIN d2_Wins[];
extern GAPDATA full_data_623e90[];
extern GAPDATA full_data_625718[];
extern GAPDATA full_data_62571c[];
extern GAPDATA full_data_6f22dc[];
extern GAPDATA full_data_6f2364[];
extern GAPDATA full_data_6faa60[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_6f2338[];
extern GAPDATA full_data_76631c[];
extern GAPDATA full_data_75ec80[];
extern GAPDATA full_data_75ec84[];
extern unsigned char ai_TimeHour;
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

extern char * ai_Unk6f22e8;
extern int ai_Unk6f22f8;
extern char ai_Unkf8aae0[0x80];
extern int d2_HitUnderPointer;
extern int d2_PointerSprite;



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
extern char * ai_Unk6f22e8;
extern int ai_Unk6f22f8;
extern int d2_HitUnderPointer;
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
extern int ai_SceneChars[32];
extern GAPDATA full_data_6237c8[];
extern GAPDATA full_data_75ec84[];
extern int gm_PlayerRecords[688];
extern int hst_KeyNext;

extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */

extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */

extern int d2_TextDrawnH;
extern int d2_TextDrawnW;



extern GAPDATA full_data_623240[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern int d2_HalfScreenH;
extern int d2_HalfScreenW;
extern GAPDATA full_data_6f2320[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern GAPDATA full_data_6f22a8[];
extern GAPDATA full_data_6f22ec[];
extern int ai_Unk6f2318;
extern GAPDATA full_data_6f22fc[];
extern GAPDATA full_data_6f2320[];
extern GAPDATA full_data_6f2338[];
extern GAPDATA full_data_6f23e4[];
extern GAPDATA full_data_6f23e8[];
extern GAPDATA full_data_6f262c[];
extern GAPDATA full_data_6f2630[];
extern GAPDATA full_data_6faa80[];
extern OPTIONBYTES ai_GameSwitches;
extern char ai_Unkf8aae0[0x80];
extern short d2_MouseX;
extern short d2_MouseY;
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern int d2_TextLeft;
extern int d2_TextTop;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern float ai_Unk5fe708;
extern int d2_HalfScreenW;
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern float d3s_QuadUV[6];
extern GAPDATA full_data_6f1c90[];
extern GAPDATA full_data_6f23ec[];
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_760444[];
extern GAPDATA full_data_760448[];
extern GAPDATA full_data_76044c[];
extern GAPDATA full_data_760450[];
extern int ai_SceneChars[32];
extern GAPDATA full_data_6f23f0[];
extern GAPDATA full_data_75ec40[];
extern char ai_Unk75fba0[2048];

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_623ebc[];
extern char ai_Unk75fba0[2048];
extern unsigned char ai_TimeHour;
extern char sv_SkyHeavy[];
extern int ai_OptionSpeed;
extern float ch_Unk5f1290;
extern GAPDATA full_data_6f22ac[];
extern int d2_HalfScreenH;
extern int d2_HalfScreenW;
extern int ai_GameFlags;

extern int ch_CurrentGroup;
extern int ai_Unk6f0cd4;
extern unsigned int ai_Unk6f0da4;
extern int ai_Unk6f0dbc;
extern char * ai_Unk6f22e8;
extern int ai_Unk6f2448;
extern int ai_TimeCopy[4];
extern int ai_Unka687e4;
extern int ai_Unkf31144;
extern char cm_RequestedSaveName[72];
extern int cm_RequestedSaveSlot;
extern int d2_CutsceneActive;
extern struct D2INPUT *d2_FocusedInput;
extern int d2_HitUnderPointer;
extern short d2_MouseX;
extern short d2_MouseY;


extern GAPDATA full_data_623eac[];
extern GAPDATA full_data_623eb0[];
extern GAPDATA full_data_623eb4[];
extern GAPDATA full_data_62457c[];
extern GAPDATA full_data_6f22ac[];
extern GAPDATA full_data_6f22f0[];
extern GAPDATA full_data_6f23f1[];
extern GAPDATA full_data_6f2580[];
extern int main_Inactive;
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f262c[];
extern int snd_MusicEnabled;
extern int snd_SfxEnabled;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern int ai_GameFlags;
extern int ai_OptionSpeed;
extern unsigned char ai_TimeHour;
extern int ai_TimeCopy[4];
extern GAPDATA full_data_6248b4[];

int d2_CreateWinText(short x, short y, char *text, int win);
int d2_WinOpenPic(short x, short y, int pic, int win);
void d2_SetObjWidth(int obj, short w);
void d2_BlitPart();
void d2_EnableObject();
void d2_RemoveObj();
void d2_SetObjEnabled();
void d2_SetObjFont();
void d2_SetObjWidth(int obj, short w);

struct G16CHARCLICK;  /* file-scope tag for the prototypes below */
struct GCLOCK;        /* f3gm.c typedef; file-scope tag for the gm_AddTime / gm_SetTime prototypes */
struct D3SURF;        /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */

extern char ch_EmptyName[];             /* 0x631090 */
extern int di_Absolute, di_MinX, di_MinY, di_MaxX, di_MaxY;  /* 0x5f3240, 0x6310b0, 0x6310b4, 0x5f3238, 0x5f323c (di_input.c) */
extern short d2_ScreenW;  /* 0xfb9072 (d2.h), the screen size */
extern short d2_ScreenH;  /* 0xfb9070 */
extern int d2_LastWindow;  /* 0x6ed938 */
extern char *d2_LastWindowPtr;  /* 0x6ed980, window created last; @original: d2.h declares D2WIN *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */
extern int d2_ClickedId;  /* 0xf8a668 */
extern unsigned char kb_Last;  /* 0x1468410 */
/* d2_RadioSel: view of d2_Radios (tools/data_views.py) */
extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */

void d2_SetPointerSprite(int a, int pic);      /* 0x476f80, gfx.c */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
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
void d2_MenuNavigate(int g);                     /* 0x46c6c0 */
void d2_RaiseViewWindows(int view);                     /* 0x4741a0 */
void d2_SetObjValue();                     /* 0x475cb0 */
void d2_RadioClear(int g);                     /* 0x46c600 */
void d2_CloseView(int view);            /* 0x4758f0 */
/* ai_SeasonCloudChance: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_SeasonClimate: view of ai_PlayerSlots (tools/data_views.py) */
extern int ai_Unk6f22c0;  /* 0x6f22c0 */
extern struct D2SURF *sv_GroundplanSurf, *sv_GroundplanView;  /* 0x6f22c4, 0x6f22c8 */
extern char ai_Unk6f0454[3];  /* 0x6f0454 */
extern char *ai_Unk6ed92c;  /* 0x6ed92c */
extern OPTIONBYTES ai_GamePanel;  /* 0x6f3668 */
/* ai_Unk6f0455: view of ai_Unk6f0454 (tools/data_views.py) */
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
typedef struct SVGROUP {                 /* the group record fields sv.c reads (AICHAR.job, ai.h AIJOB) */
    unsigned char kind;                 /* +0x00, the job kind (sv_MoveToGroup's switch) */
    char pad01[0x51];
    AITIME time;                        /* +0x52, when the job was left empty (kinds 0x14 / 0x15 / 0x28 / 0x8c) */
    char pad60[0x10];
    int state;                          /* +0x70, of a kind 0x28 job: below 3 the job is reset (-1) when a member leaves */
    char pad74[0x18];
    int member[8];                      /* +0x8c, the character ids of the members, -1 = a free slot */
} SVGROUP;
typedef struct SVMODEL {                /* the CHARACTER fields (ch.h) sv.c reads of a character's model */
    char pad00[0x28];
    int scriptId;                       /* +0x28, the event script the model runs, -1 = none */
    int group;                          /* +0x2c, the group id (compared with a building's id) */
    char pad30[4];
    D3OBJECT *obj;                      /* +0x34, the body */
    char pad38[0x50];
    D3SCENE *scene;                     /* +0x88, the scene the model is in */
    char pad8c[0x9c];
    void *actions;                      /* +0x128, the action queue, 0 = idle */
} SVMODEL;
typedef struct SVCHARREC {
    unsigned short id;                  /* +0x00 */
    char pad02[2];
    unsigned int netId;                 /* +0x04, the id the order events carry (AICHAR.netId) */
    unsigned char health;               /* +0x08, > 0 = alive (AICHAR.health) */
    char pad09[0x27];
    char name[0x58];                    /* +0x30, AICHAR.name */
    char pad88[0xdc];
    unsigned char jobId;                /* +0x164, AICHAR.jobId */
    char title;                         /* +0x165, the title rank (AICHAR.title) */
    char pad166[6];
    unsigned char *bld;                 /* +0x16c, the building the character is in (AICHAR.bld) */
    char pad170[8];
    int inventory;                      /* +0x178, the character's goods inventory (AICHAR.inventory) */
    SVGROUP *group;                     /* +0x17c, the job the character is a member of (AICHAR.job) */
    unsigned char groupKind;            /* +0x180, its kind (AICHAR.jobKind) */
    char pad181[3];
    SVMODEL *model;                     /* +0x184, the character's model (a ch.h CHARACTER) */
    unsigned char selected;             /* +0x188, selected in the member list */
    char pad189[3];
    int picture;                        /* +0x18c, the portrait picture of the member list */
    char pad190[4];
    int money;                          /* +0x194, the character's money (AICHAR.money) */
    char pad198[0x19];
    unsigned char flag1b1;              /* +0x1b1, nonzero: the info panel stays closed for the character */
    char pad1b2[0x16];
    unsigned int flags;                 /* +0x1c8, AICHAR.flags1c8 (0x800: not available for the member list) */
} SVCHARREC;
typedef struct SVMEMBERROW {            /* 0x28 bytes, a row of the member list (32 rows at 0x75f280) */
    int check;                          /* +0x00, the selection button (the character's portrait) */
    int frame;                          /* +0x04, the frame picture 0x5dc */
    int icon[2];                        /* +0x08, the two state pictures (sv_CharStateIcons), -1 = none */
    int chr;                            /* +0x10, the character index, 0xffff = a free row */
    int text;                           /* +0x14, the name / health text */
    int window;                         /* +0x18, the child window with the two bars, -1 = none */
    int cleared1c;                      /* +0x1c, cleared when the row is made (its use is open) */
    int selected;                       /* +0x20, the character's selection at the last update */
    int pad24;
} SVMEMBERROW;
#pragma pack(pop)
extern SVPLATE sv_NamePlates[0x20];

int gm_IsType10(unsigned char *b);       /* 0x5024f0, f3gm.c */
int d2_DrawMousePointer(int x, int y);         /* 0x476f00, gfx.c */
int d2_AddHitRect();                     /* 0x478890, @original: sv.c's callers were compiled against the non-prototype declaration (sv_BuildScreenLayout passes an int d2_ScreenW - 0x50 without a short conversion, 464 bytes vs 480) */
int d2_CreateWindow();                     /* 0x472390 */
void d2_PlaceWindowScaled();                     /* 0x475730 */
void * d2_GetPic();                     /* 0x4690b0 */
int d2_LockSurface();                     /* 0x479020 */
int d2_ClearSurface();                     /* 0x4796c0 */
int d2_UnlockSurface();                     /* 0x4790e0 */
struct D2SURF * d2_MakeSurface(struct D2SURF *desc, int flags);                     /* 0x478cc0 */
int d2_CreatePicObj();                     /* 0x46cd80 */
void d2_EnableObject();                     /* 0x475aa0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void d2_RemoveObj();                     /* 0x46e710 */
int d2_FreeSurface(struct D2SURF *s);                     /* 0x478fc0 */
void d2_RemoveHitRect();                     /* 0x4788f0 */
int d2_RemoveWindow();                     /* 0x472ce0 */

#include "time_view.h"
#include "io_text.h"
#include "ls.h"
#include "ahm.h"
#include "ai.h"
#include "d3_tools.h"
#include "d3_sm.h"
#include "anm.h"
#include "bio.h"
#include "d3_anim.h"
#include "d3_d3d.h"
#include "d3_engine.h"
#include "d3_interface.h"
#include "d3_io_t.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_snd.h"
#include "d3_vec.h"
#include "d3s.h"
#include "dd_vesa.h"
#include "err.h"
#include "f3init.h"
#include "he.h"
#include "hst.h"
#include "main.h"
#include "mem.h"
#include "msx.h"
#include "tr.h"
#include "ts_texture.h"
#include "txt.h"
#include "unk_458810.h"
#include "unk_4b39f0.h"
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "vfs.h"
#include "shp.h"
#define d3_PointToWorldK ((void (*)())d3_PointToWorld)  /* 0x458a20 (d3.h, 3 parameters): @original sv_Unk528180 0x5281b5 pushes 4 arguments (the 4th an uninitialised register), compiled without a prototype */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
/* 0x520ec0, the tick callback: turns the frame counter sv_TickFrames into the float sv_FramesPerTick and clears it */

void sv_UpdateFramesPerTick(void)
{
    sv_FramesPerTick = (float)sv_TickFrames;
    sv_TickFrames = 0;
}
/* 0x520ee0, registers sv_UpdateFramesPerTick as the tick callback (the period 0x47) and clears the selection flags
   sv_SelectionFlags (0xc0 words) and the bomb records cm_Bombs (0x40 words) */

void sv_InitTick(void)
{
    int i;

    tb_OpenFunction(sv_UpdateFramesPerTick, 0x47);
    for (i = 0; i < 0xc0; i++)
        sv_SelectionFlags[i] = 0;
    for (i = 0; i < 0x40; i++)
        cm_Bombs[i] = 0;
}
/* 0x520f10, unregisters the tick callback */

void sv_UnregisterTick(void)
{
    tb_CloseFunction(sv_UpdateFramesPerTick);
}
/* 0x520f20, resets the screen clip and the scene state 0x631090 and the window metrics 0x5f3238 / 0x5f323c (screen height - 8, width) */

void sv_ResetScreenClip(void)
{
    d2_SetPointerSprite(0, 0);
    d2_ClearPointerItems();
    di_ClearButtons();
    sv_SetStatusText(ch_EmptyName);
    d2_CopyCaption(ch_EmptyName);
    di_Absolute = 1;
    di_MinX = 0;
    di_MinY = 0;
    di_MaxX = d2_ScreenW - 8;
    di_MaxY = (int)d2_ScreenH;
}
/* 0x520f90, checks that the object o has room for n slots (+0x194); otherwise a message box with the text 0x74 (n); 1 when ok */

int sv_CheckMoney(char *o, int n)
{
    char buf[0x80];

    if (((SVCHARREC *)o)->money < n && n != 0) {
        txt_Format(buf, 0x74, n);
        sv_MessageBox(buf, 0);
        return 0;
    }
    return 1;
}
/* 0x520fe0, checks the character that works for the master id: when its flag +0x1b4 & 1 is set (gm_FindCharacterByMaster,
   0x50a810) a message box with the text 0x78 (the short id) in the pergament style; 1 when shown */

int sv_CheckCharacterFlag(int id)
{
    unsigned short *p;
    char buf[0x100];

    p = (unsigned short *)gm_FindCharacterByMaster(id);
    if (p != 0 && (*((unsigned char *)p + 0x1b4) & 1) != 0) {
        txt_Format(buf, 0x78, *p);
        sv_MessageBox(buf, 0x100);
        return 1;
    }
    return 0;
}
/* 0x521040, checks that the stock of the character o (gm_SumSlotAmounts of its inventory +0x178 and the good t) covers n;
   otherwise a message box with the text 0x75 (t); 1 when ok */

int sv_CheckGoodAmount(char *o, int n, unsigned t)
{
    char buf[0x40];

    if (gm_SumSlotAmounts(((SVCHARREC *)o)->inventory, t) < n && n != 0) {
        txt_Format(buf, 0x75, t & 0xff);
        sv_MessageBox(buf, 0);
        return 0;
    }
    return 1;
}
/* 0x5210a0, sv_CheckStock: checks that the stock list holds at least n of the good record p (its prototype word; 0x508660) and shows a message box with the text by the prototype (0x142 / 0x115, the two money seats, pick 0x76 / 0x77, else 0x75); flag 4 auto-closes the box. 1 when the stock covers n, 0 after the box */

int sv_CheckStock(short *p, int stock, int n, unsigned t)
{
    int v;
    char buf[0x200];

    v = gm_SumSlotAmounts(stock, t);
    if (v >= n || n == 0)
        return 1;
    if (p != 0) {
        if (*p == 0x142) {
            txt_Format(buf, 0x76, t & 0xff);
            goto show;
        }
        if (*p == 0x115) {
            txt_Format(buf, 0x77, t & 0xff);
            goto show;
        }
    }
    txt_Format(buf, 0x75, t & 0xff);
show:
    sv_MessageBox(buf, 4);
    return 0;
}
/* 0x521150, a modal message box (the form "misc\\Messagebox" in the sizes by the flags 2 Autosize / 0x10 BIG / 0x20 VERY_BIG, 0x100 the pergament "NONE_PERGA", 8 all messages): shows the text, loops
   sv_RunFrame until a button, waits at most 500 ms with flag 4, a countdown bar with flag 0x80 and returns the button + 1 (0 for none / cancel). Not reentrant (sv_MessageBoxOpen, 0x6f2350)
   @deep behaviour: 133 code hunks, every hunk read against the original; the statement set is complete (form call per branch, timed/quick/n, do-while frame loop, unsigned t0 timeout) and the rest is the allocation cascade: the original holds the widget-table base in ebx, so the flags param slot stays live and form/ret/bar take the slots frame+0x00/+0x08/+0x10 with bar in esi; ours puts the table in edx, form in the flags slot, bar in a slot and keeps a zero register in esi (the cmp eax,esi / test eax,eax / cmp DWORD PTR ds:X,esi hunks follow from that, not from a meaning error). Readability as far as base evidence goes: callees d2_OpenForm / d2_SetForm / d2_RadioAdd / d2_RadioSelect / d2_RaiseViewWindows / d2_WinOpenSlider / d2_RadioSel / d2_ClickedId / d2_SetObjValue and globals kb_Last / d3_Time / d2_LastWindowPtr / d2_Objs / sv_MessageBoxOpen named, body complete and described; open: Q2 d2_HitWindow (0xf8aad0, gfx.h calls it d2_Unk8aad0, still a placeholder; role = the window under the pointer copied from d2_PointerWin in d2_MouseUpdate, a d2.h naming decision); Q4 7 raw accesses need D2WIN / D2OBJ typed views (sv.c does not include d2.h yet: its char * views of d2_Objs / d2_LastWindowPtr supersede d2.h's); Q7 d2_SetObjValue is K&R in d2.h and cm.h because the callers pass 2..5 arguments, no canonical prototype. */

int sv_MessageBox(int text, unsigned flags)
{
    int ret, form, bar, idx, i, k, scene, timed, quick, n;
    unsigned mask;
    unsigned t0;

    t0 = d3_Time;
    ret = 0;
    i = 0;
    bar = -1;
    if (sv_MessageBoxOpen != 0)
        return 0;
    mask = sv_FrameMask & 0xffffffc7;
    if (flags & 2)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if (flags & 0x10)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if (flags & 0x20)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else if (flags & 0x100) {
        form = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_SetForm(form, 1);
    } else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    if (flags & 8)
        mask = 0x717c6;
    io_PrintText(text);
    scene = d2_LastWindow;
    d2_CentreViewWindows(form);
    sv_MessageBoxOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    if (*(short *)(k + 0x1c) > 0) {
        do {
            int w = *(int *)(*(int *)(k + 0x18) + i * 4);
            if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40)
                d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
            i++;
        } while (i < *(short *)(k + 0x1c));
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    timed = flags & 0x80;
    if (timed != 0)
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x18, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    if (sv_RunFrame(sv_MessageBox, mask) != 0) {
        quick = flags & 0x40;
        n = flags & 4;
        do {
            d2_MenuNavigate(idx);
            if (quick != 0)
                d2_RaiseViewWindows(form);
            if (d2_ClickedId == 0) {
                if (d2_HitWindow == scene) {
                    ret = d2_RadioSel[idx * 0x23] + 1;
                    sv_PopRequest = 1;
                } else if (kb_Last == 1) {
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if ((d2_ClickedId == 1 && d2_HitWindow == scene) || kb_Last == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (timed != 0 && bar != -1)
                d2_SetObjValue(bar, 0, 500, d3_Time - t0);
            if (n != 0 && d3_Time > t0 + 500)
                sv_PopRequest = 1;
        } while (sv_RunFrame(sv_MessageBox, mask) != 0);
    }
    sv_MessageBoxOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
/* 0x521410, the green message box ("misc\\MessageBoxGreen" 0x623ed0) with an optional title text a and the text b; like 0x521150, a button returns its number + 1. Not reentrant (sv_MessageBoxGreenOpen, 0x6f2351) @deep behaviour: 73 code hunks, all read: the statement set is complete and matches 0x521150's twin (form, a/b texts, timed/quick/n, do-while frame loop); the residue is allocation - the original keeps the constant 1 in ebx (cmp eax,ebx / mov ds:X,ebx / cmp BYTE PTR ds:X,bl) and hoists bar = -1 into its own frame+0x10 slot with form in ebp, ours uses immediates and puts form in edi (the first attempts at the hoist made the count worse: 73 -> 267, reverted). Readability as far as base evidence goes: sv_FitForm (0x52ea30), d2_OpenForm / d2_SetForm / d2_RadioAdd / d2_RadioSelect / d2_RaiseViewWindows / d2_WinOpenSlider / d2_RadioSel / d2_ClickedId / d2_SetObjValue, kb_Last / ai_ClockMs / d2_LastWindowPtr / d2_Objs and sv_MessageBoxGreenOpen named, marker dropped; open: Q4 6 raw accesses need the D2WIN / D2OBJ typed views (sv.c does not include d2.h yet: its char * views of d2_Objs / d2_LastWindowPtr supersede d2.h's); Q7 d2_SetObjValue K&R (2..5 argument callers, no canonical prototype). */

int sv_MessageBoxGreen(int a, int b, unsigned flags)
{
    int ret, form, bar, idx, i, k;
    unsigned mask;
    unsigned t0;

    t0 = d3_Time;
    i = 0;
    ret = 0;
    if (sv_MessageBoxGreenOpen != 0)
        return 0;
    mask = sv_FrameMask & 0xffffffc7;
    form = d2_OpenForm(0, 0, "misc\\MessageBoxGreen");
    if ((flags & 8) != 0)
        mask = 0x717c6;
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    if (a != 0)
        io_PrintText(a);
    d2_SetForm(form, 2);
    io_PrintText(b);
    d2_CentreViewWindows(form);
    sv_MessageBoxGreenOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    while (i < *(short *)(k + 0x1c)) {
        int w = *(int *)(*(int *)(k + 0x18) + i * 4);
        if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40) {
            d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
        }
        i++;
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    if ((flags & 0x80) == 0)
        bar = -1;
    else
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x18, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    while (sv_RunFrame(sv_MessageBoxGreen, mask) != 0) {
        d2_MenuNavigate(idx);
        if ((flags & 0x40) != 0)
            d2_RaiseViewWindows(form);
        if (d2_ClickedId == 0) {
            sv_PopRequest = 1;
            ret = d2_RadioSel[idx * 0x23] + 1;
        } else if (d2_ClickedId == 1 || kb_Last == 1) {
            sv_PopRequest = 1;
            ret = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if ((flags & 0x80) != 0 && bar != -1)
            d2_SetObjValue(bar, 0, 500, d3_Time - t0);
        if ((flags & 4) != 0 && t0 + 500 < d3_Time)
            sv_PopRequest = 1;
    }
    sv_MessageBoxGreenOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
/* 0x5216a0, the yes / no message box (the form by the size flags like 0x521150: 2 Autosize, 0x10 BIG, 0x20 VERY_BIG):
   the text, a radio button per non-static window of the form, the optional slider 0x80 and the expiry check 4; returns 1
   for the first button (yes), 0 for the second. Not reentrant: it holds sv_MessageBoxYesNoOpen (0x6f2352) while it runs, and
   returns 0 at once when the box is already open
   @deep B3: code hunks remain (grade: 114 code, 24 slots, 2 fill; 90 differing asm lines, 394/608 bytes, our size 608 vs the original 607). The whole original 0x5216a0..0x5218fe was read (disasm) and the statement sequence matches: the non-reentrancy guard, mask = sv_FrameMask & 0xffffffc7 with the 8 -> 0x717c6 override, the form per size flag, io_PrintText / d2_CentreViewWindows / d2_CreateRadioGroup, the radio-button walk over the non-static windows, d2_RadioSelect / d2_SetForm, the optional slider 0x80, and the do-while sv_RunFrame body (d2_MenuNavigate, 0x40 raise, clicked 0 -> ret 1 / clicked 1 -> ret 0, the mouse-event pop, the slider update and the 500 ms expiry). Fixes this session: the form name is one d2_OpenForm call per arm (if ((flags & 2) != 0) ... else if 0x10 ... else if 0x20 ...; the original pushes the string per arm and shares the push 0 / push 0 / call tail, 246 -> 114 code hunks), the clock is d3_Time (the canonical 0x6333cc name; the volatile re-read matches the original's re-reads), sv_MessageBoxYesNoOpen (0x6f2352) named after its twin flags sv_MessageBoxOpen / sv_MessageBoxGreenOpen, Q6 comment, ROUGH dropped. Left, all allocation: (1) the shared constant 0x40 - ours hoists mov edi,0x40 and reuses it for the window style compare AND the slider's height - 0x40, and reuses edi for the constant 1 later; the original uses the immediates in all three places and never touches edi (0x52174d / 0x521772 / 0x5217dd / 0x521840); (2) the slot pair: ours bar at frame+0x10 and t0 at frame+0xc, the original bar at frame+0xc and t0 at frame+0x10 (0x5216a9 / 0x5216b4), and the push edi shifts every other slot by 4; declaration order tried (bar before t0, t0 first, w at the top) with no change; (3) bar lives in esi in ours, in a slot in the original (0x521814 / 0x521840 region). Q1-Q3, Q5, Q6 pass; Q4 (6 raw *(T *) in the d2 window / obj reads) and Q7 (d2_SetObjValue K&R) stay the documented type debt of the whole sv.c message-box family (sv.c does not include d2.h yet: its char * views of d2_Objs / d2_LastWindowPtr supersede d2.h's), same as the note of 0x521150 / 0x521410. */

int sv_MessageBoxYesNo(int text, unsigned flags)
{
    int t0, ret, form, bar, idx, i, k, w;
    unsigned mask;

    t0 = d3_Time;
    i = 0;
    bar = -1;
    ret = 0;
    if (sv_MessageBoxYesNoOpen != 0)
        return 0;
    mask = sv_FrameMask & 0xffffffc7;
    if ((flags & 2) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if ((flags & 0x10) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if ((flags & 0x20) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    if ((flags & 8) != 0)
        mask = 0x717c6;
    io_PrintText(text);
    d2_CentreViewWindows(form);
    sv_MessageBoxYesNoOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    while (i < *(short *)(k + 0x1c)) {
        w = *(int *)(*(int *)(k + 0x18) + i * 4);
        if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40) {
            d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
        }
        i++;
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    if ((flags & 0x80) != 0)
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x20, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    while (sv_RunFrame(sv_MessageBoxYesNo, mask) != 0) {
        d2_MenuNavigate(idx);
        if ((flags & 0x40) != 0)
            d2_RaiseViewWindows(form);
        if (d2_ClickedId == 0) {
            sv_PopRequest = 1;
            ret = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
            ret = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if ((flags & 0x80) != 0 && bar != -1)
            d2_SetObjValue(bar, 0, 500, d3_Time - t0);
        if ((flags & 4) != 0 && t0 + 500 < d3_Time)
            sv_PopRequest = 1;
    }
    sv_MessageBoxYesNoOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
/* 0x521900, opens a non-modal message box (the form by the flags 0x10 BIG / 0x20 VERY_BIG), shows the text, pumps
   the messages once and returns the form */

int sv_OpenMessageBox(int text, unsigned char flags)
{
    int form;

    if ((flags & 0x10) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if ((flags & 0x20) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    d2_CentreViewWindows(form);
    io_PrintText(text);
    sv_RunFrame(0, sv_FrameMask);
    return form;
}
/* 0x521960, selects the object o (of the player or a pause object) for the info panel: stores it in sv_PanelObject, its
   scene record of the id b in sv_PanelRecord (gm_Query of the object id +0x5d; the kind 0x47 uses the record 0xfd), copies
   the text c into sv_PanelText and raises sv_PanelFlag. When the record is the one shown (sv_PanelShown) the redraw counter
   sv_PopRequest is set to the frame difference (sv_FrameDepth - sv_FrameBase), else the record is taken over and shown
   (init_LoadObjectScene for a scene universe that is still dirty); finally sv_PopTo(0x591520)
   @deep behaviour draft after the base rewrites (256 -> 202 code hunks), readable done (Q1..Q7 pass). The statement set is complete; the reading matches the original instruction for instruction. Gains kept: (1) sv_PanelRecord is not copied into a local - the original reloads the global at every use (this alone 256 -> 202); (2) 'cur = 0; if (b) cur = b; else if (f) cur = f;' reproduces the original's shared conditional assign (cmp eax,ebp / jne / mov eax,[ai_FallbackBuilding] / cmp / je / mov edi,eax); (3) *sv_PanelRecord read unsigned (the original's xor edx,edx / mov dx into init_LoadObjectScene); (4) the record views SVOBJREC/SVCHARREC cost no hunk (202 with and without). What is left is one allocation cascade over four saved registers: the original pushes ebx/ebp/esi/edi with o=esi, cur=edi, the constant 0=ebp and the constant 1=ebx (mov ebx,0x1 before the first gm_Query, kept across both calls and both sv_PanelFlag = 1 stores); ours pushes ebp/esi/edi only, folds cur into ebp (so the one xor serves both cur = 0 and the zero constant), tests instead of cmp-with-zero and uses immediates for 1. Tried, same 202: cur as a declaration initializer, the nested if/else polarity swap, the inline ((SVOBJREC *)o)-> cast instead of the bld local. spell.py: 521965 reload (the original reloads the o parameter from its stack slot at one use; the bld local is not the cause - dropping it changes nothing), 5219db / 521b5a dup-tail (the cur tail and the sv_PopTo tail: both spellings give 202), 5219a1 code (the ai_BTypes address: the original lea's it into eax, ours folds it into the mov's SIB - sib/add-lea, no source form found). Remaining leads for deep: the constant-register CSE (0 in ebp, 1 in ebx) and cur in edi vs ebp, the ai_BTypes temp ebx vs edi, the lea vs SIB load - allocation, i.e. vargen territory. */

void sv_SelectPanelObject(unsigned char *o, int b, char *c)
{
    char k;
    unsigned char *cur = 0;
    int w;
    unsigned short *old;
    int oldv;
    SVOBJREC *bld;

    bld = (SVOBJREC *)o;
    if (sv_PanelFlag != 0)
        return;
    if (!(ai_PlayerId == 0xffff || ai_Chars[ai_PlayerId][0x1b1] == 0 || (k = ai_BTypes[bld->kind * 0x24d]) == 6 || (k == 4 && bld->ownerId == ((SVCHARREC *)ai_Chars[ai_PlayerId])->netId)))
        return;
    if (ai_PlayerBuilding != 0)
        cur = (unsigned char *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        cur = (unsigned char *)ai_FallbackBuilding;
    sv_PanelObject = (char *)o;
    sv_PanelRecord = (unsigned short *)gm_Query(bld->objList, 1, 1, b);
    if (bld->kind == 0x47)
        sv_PanelRecord = (unsigned short *)gm_Query(bld->objList, 1, 0, 0xfd);
    strcpy(sv_PanelText, c);
    sv_PanelFlag = 1;
    if (sv_PanelRecord == 0 || *sv_PanelRecord == 0) {
        if (cur == o && ai_FallbackBuilding == 0) {
            sv_PanelFlag = 0;
            return;
        }
    } else {
        if (sv_PanelRecord == sv_PanelShown) {
            if (sv_FrameDepth <= sv_FrameBase) {
                sv_PanelFlag = 0;
                return;
            }
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            return;
        }
        if (o == cur && gm_IsHomeType(cur) != 0) {
            bld->sceneRec = *sv_PanelRecord;
            if (sv_PanelShown != 0) {
                sv_PopRequest = (sv_FrameDepth - sv_FrameBase) + 1;
                return;
            }
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            return;
        }
        old = sv_PanelShown;
        oldv = ai_FrameWait;
        bld->sceneRec = *sv_PanelRecord;
        if (gm_IsHomeType(o) == 0 && gm_IsType10(o) == 0) {
            sv_PanelShown = sv_PanelRecord;
            sv_PanelFlag = 0;
            ai_FrameWait = 0;
            w = init_ObjectSceneUniverse(sv_PanelRecord);
            if (init_SceneDirty[w * 0x3e0] != 0)
                init_LoadObjectScene(*sv_PanelRecord);
            sv_PanelFlag = 1;
            ai_FrameWait = oldv;
            sv_PanelShown = old;
        }
    }
    sv_PopTo(ai_GameFrame);
}
/* 0x521ba0, draws the mouse pointer for the panel layout d2_PointerLook: the scroll arrows 3 / 6 show it at (-0x28, 0),
   8 at (-0x28, -0x30), the bar layouts 1 / 7 at (0, -0x30) and every other layout at (0, 0)
   @deep behaviour draft, readable done. 65 -> 19 code hunks this session: the width was the whole first half (d2_PointerLook is a short at 0x6ed9d0, the original loads a word and cmp ax; the int declaration made dword compares) and the shape is one if/else-if chain in the original's test order 1 / 3 / 6 / 8 / 7 with x = -0x28 in the 3 / 6 / 8 arms and y = -0x30 in 1 / 7, each of 3 / 6 / 8 calling and returning. Tried, all 19 code hunks: x, y locals assigned at the top; the same with declaration initializers; 'x = y = 0'; literal args in the arms (-0x28, y) / (-0x28, -0x30) / (0, y); an explicit 'x = 0' in the 1-arm; a switch over the 5 cases (a jump table, 77 hunks). Left: the original keeps BOTH x = 0 (edx) and y = 0 (ecx) live from the entry (xor edx,edx / xor ecx,ecx) and pushes them (mov edx,-0x28; push ecx; push edx), where VC6 folds ours to 'push 0 / push -0x28' and drops x's register - a constant-fold / peephole choice (spell: 521ba6 dup-tail, the 4-instruction 3-arm tail, hints tried as listed). vargen / perm is what is left. */

void sv_DrawPanelPointer(void)
{
    int x, y;

    x = 0;
    y = 0;
    if (d2_PointerLook == 1)
        y = -0x30;
    else if (d2_PointerLook == 3) {
        x = -0x28;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 6) {
        x = -0x28;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 8) {
        x = -0x28;
        y = -0x30;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 7)
        y = -0x30;
    d2_DrawMousePointer(x, y);
}

#define SV_I(a) (*(int *)(a))

/* 0x521c10, a Ghidra miss (inside the span of 0x521ba0): the progress callback of the scene load (kind, cur, total): the first call creates the bar object 0x623e84 (0x46a4c0, 0x190 wide, below the screen
   centre: 0xfe73c8 * 0.5 + 0xfe73a8 - 205), kind 0 sets it to cur * 200 / total, kind 1 to 200 + cur * 200 / total and at the last step destroys it (0x46e710) and redraws the screen (0x4796c0 .. 0x479480,
   0x522f10 (1, 0), 0x6f22bc = 1); every 10th frame (0x6f2354) the screen is blitted (0x479020 / 0x46aa00 / 0x521ba0 / 0x4790e0 / 0x41e390) (ROUGH) */
void sv_SceneLoadProgress(int kind, int cur, int total)
{
    int h;

    if (SV_I((unsigned)&full_data_623e84) == -1) {
        SV_I((unsigned)&d2_CutsceneActive) = 1;
        SV_I((unsigned)&full_data_6f2354) = 0;
        h = d2_OpenSlider((int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX - 205.0f), *(short *)(unsigned)&d2_ScreenH - 0x40, 0x190, 0, 0x190, 0x83, 0x42);
        SV_I((unsigned)&full_data_623e84) = h;
        d2_SetObjValue(h, 0, 0x190, 0);
        d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
        if (SV_I((unsigned)&full_data_623e84) == -1)
            goto lend;
    }
    if (kind == 0 || SV_I((unsigned)&full_data_6f2354) % 10 == 0)
        d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
    if (kind == 0) {
        d2_SetObjValue(SV_I((unsigned)&full_data_623e84), 0, 0x190, cur * 200 / total);
    } else if (kind == 1) {
        d2_SetObjValue(SV_I((unsigned)&full_data_623e84), 0, 0x190, cur * 200 / total + 200);
        if (cur >= total - 1) {
            d2_RemoveObj(SV_I((unsigned)&full_data_623e84));
            SV_I((unsigned)&full_data_623e84) = -1;
            SV_I((unsigned)&d2_CutsceneActive) = 0;
            if (SV_I((unsigned)&sv_PanelPic) != -1) {
                d2_ClearSurface(SV_I((unsigned)&d2_ScreenSurf));
                d2_DrawPic(0, 0, SV_I((unsigned)&sv_PanelPic), SV_I((unsigned)&d2_ScreenSurf));
                d2_LockSurface(SV_I((unsigned)&d2_ScreenSurf));
                sv_UpdateCityPanel(1, 0);
                d2_UnlockSurface(SV_I((unsigned)&d2_ScreenSurf));
                d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
            }
            SV_I((unsigned)&ai_FrameWait) = 1;
        }
    }
    if (SV_I((unsigned)&full_data_6f2354) % 10 == 0 && SV_I((unsigned)&full_data_623e84) != -1 && d2_LockSurface(SV_I((unsigned)&d2_ScreenSurf)) != 0) {
        d2_DrawSlider(SV_I((unsigned)&full_data_623e84), SV_I((unsigned)&d2_ScreenSurf));
        sv_DrawPanelPointer();
        d2_UnlockSurface(SV_I((unsigned)&d2_ScreenSurf));
        dd_Flip();
    }
lend:
    SV_I((unsigned)&full_data_6f2354) = SV_I((unsigned)&full_data_6f2354) + 1;
}
/* 0x521e70, builds the main screen layout for the scene s: clears the clip, creates the bar widgets 0x623850 / 0x623840 (a 0x288 x 0x30 strip), redraws the background bitmap (0x6ed92c, the rectangle 0x6248a4 ..
   0x6248b0) and sets up the groundplan view 0x6f22c8 (0x478cc0 with a table of 0x10 words: 0x68 / 0x6e) and the button 0x6238a4 (0x46cd80 (0x286, 0x23c, 0x544)). ROUGH */

void sv_BuildScreenLayout(int s)
{
    int t, i;
    int tab[16];
    int *q;

    d2_SetClipRect(0, 0, (int)d2_ScreenW, (int)d2_ScreenH);
    ai_Unk623850 = d2_AddHitRect(d2_ScreenW - 0x50, 0x11, 0x30, 0x30, 0);
    ai_Unk623840 = d2_CreateWindow(4, 10, 0x288, 0x30, 0);
    d2_PlaceWindowScaled(ai_Unk623840, 1);
    if (ai_Unk6f22c0 != 0) {
        mem_Free(ai_Unk6f22c0);
        ai_Unk6f22c0 = 0;
    }
    t = d2_GetPic(s);
    if (d2_LockSurface(ai_Unk6ed92c) != 0) {
        d2_ClearSurface(ai_Unk6ed92c);
        d2_DrawShape(0, 0, ai_Unk6ed92c, t, 0);
        ai_Unk6f0454[0] = 0;
        ai_Unk6f0454[1] = 0;
        ai_Unk6f0454[2] = 0;
        ai_Unk6f22c0 = d2_GrabShape(sv_ViewRectLeft, sv_ViewRectTop, (sv_ViewRectRight - sv_ViewRectLeft) + 1, sv_ViewRectBottom - sv_ViewRectTop, ai_Unk6ed92c, 0, 1);
        d2_ClearSurface(ai_Unk6ed92c);
        d2_UnlockSurface(ai_Unk6ed92c);
    }
    ai_Unkfb9068 = sv_ViewRectRight;
    q = sv_GroundplanButtons;
    for (i = 0xf; i != 0; i--)
        *q++ = -1;
    ai_Unkfb9060 = sv_ViewRectLeft;
    ai_Unkfb9064 = sv_ViewRectTop;
    q = tab;
    for (i = 0x10; i != 0; i--)
        *q++ = 0;
    sv_PanelPic = s;
    ai_Unkfb906c = sv_ViewRectBottom;
    tab[1] = 0x68;
    tab[2] = 0x6e;
    sv_GroundplanView = d2_MakeSurface(tab, 3);
    d2_ClearSurface(sv_GroundplanView);
    ai_Unk6238a4 = d2_CreatePicObj(0x286, 0x23c, 0x544);
    *(int *)(d2_Objs + 0x4c + ai_Unk6238a4 * 0x2ec) = 1;
    d2_EnableObject(ai_Unk6238a4, 0);
}
/* 0x522040, the image number of the panel for the player's group (0x6f3668, or when 0: the kind number 0x504ad0 of the first player character): 0x534 (1, 2, 10), 0x538 (3, 4), 0x53c (0xb, 0xc, 6), 0x540 (5, 7, 8, 9); 0x534 else */

int sv_PanelImage(void)
{
    int i, k;
    char *p;
    int img;

    img = 0x534;
    if (ai_GamePanel.byte[0] == 0) {
        i = 0;
        for (p = &ai_Chars[0][2]; (int)p < (int)&ai_Chars[768][2]; p += 0x21c) {
            if (*p == 6 || *p == 7)
                break;
            i++;
        }
        k = gm_TitleGroup((unsigned char)ai_Chars[i][0x164]);
        if (k == 1 || k == 2 || k == 10)
            img = 0x534;
        else if (k == 3 || k == 4)
            img = 0x538;
        else if (k == 0xb || k == 0xc || k == 6)
            img = 0x53c;
        else if (k == 5 || k == 7 || k == 8 || k == 9)
            img = 0x540;
    } else if (ai_GamePanel.byte[0] == 1) {
        img = 0x534;
    } else if (ai_GamePanel.byte[0] == 2) {
        img = 0x538;
    } else if (ai_GamePanel.byte[0] == 3) {
        img = 0x53c;
    } else if (ai_GamePanel.byte[0] == 4) {
        img = 0x540;
    }
    return img;
}
/* 0x522100, enables (n != 0) or disables (n == 0) the panel widgets (0x475aa0 marks an object disabled
   when its on argument is 0): the flag 0x623e88 = n, and every widget id of the panel set (0x623860 by
   0x475590, 0x53dad0 for the list, the others by 0x475aa0, the strip 0x623840 through
   its record of 0xf8f128, the table 0x7662e0) that is not -1 */

void sv_SetPanelEnabled(int n)
{
    int *q;

    ai_Unk623e88 = n;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    he_Unk53dad0(n);
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    if (ai_Unk623820 != -1)
        d2_EnableObject(ai_Unk623820, n);
    if (ai_Unk623824 != -1)
        d2_EnableObject(ai_Unk623824, n);
    if (ai_Unk623828 != -1)
        d2_EnableObject(ai_Unk623828, n);
    if (ai_Unk62382c != -1)
        d2_EnableObject(ai_Unk62382c, n);
    if (ai_Unk623830 != -1)
        d2_EnableObject(ai_Unk623830, n);
    if (ai_Unk623834 != -1)
        d2_EnableObject(ai_Unk623834, n);
    if (ai_Unk623844 != -1)
        d2_EnableObject(ai_Unk623844, n);
    if (ai_Unk623838 != -1)
        d2_EnableObject(ai_Unk623838, n);
    if (ai_Unk62383c != -1)
        d2_EnableObject(ai_Unk62383c, n);
    if (ai_Unk6238a4 != -1 && n == 0)
        d2_EnableObject(ai_Unk6238a4, 0);
    if (ai_Unk623848 != -1)
        d2_EnableObject(ai_Unk623848, n);
    if (ai_Unk62384c != -1)
        d2_EnableObject(ai_Unk62384c, n);
    if (ai_Unk623840 != -1)
        d2_EnableObject(*(int *)(ai_Unkf8f128 + 0x264 + ai_Unk623840 * 0x3c4), n);
    q = sv_GroundplanButtons;
    do {
        if (*q != -1)
            d2_EnableObject(*q, n);
        q++;
    } while ((int)q < (int)((unsigned)&sv_GroundplanButtons + 0x3c));
}
/* 0x522280, closes the panel widgets (0x46e710, id := -1) and hides the rest */

void sv_Unk522280(void)
{
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    if (ai_Unk623820 != -1) {
        d2_RemoveObj(ai_Unk623820);
        ai_Unk623820 = -1;
    }
    if (ai_Unk623824 != -1) {
        d2_RemoveObj(ai_Unk623824);
        ai_Unk623824 = -1;
    }
    if (ai_Unk623828 != -1) {
        d2_RemoveObj(ai_Unk623828);
        ai_Unk623828 = -1;
    }
    if (ai_Unk623844 != -1) {
        d2_RemoveObj(ai_Unk623844);
        ai_Unk623844 = -1;
    }
    if (ai_Unk62382c != -1) {
        d2_RemoveObj(ai_Unk62382c);
        ai_Unk62382c = -1;
    }
    if (ai_Unk623830 != -1) {
        d2_RemoveObj(ai_Unk623830);
        ai_Unk623830 = -1;
    }
    if (ai_Unk623834 != -1) {
        d2_RemoveObj(ai_Unk623834);
        ai_Unk623834 = -1;
    }
    if (ai_Unk623838 != -1) {
        d2_RemoveObj(ai_Unk623838);
        ai_Unk623838 = -1;
    }
    if (ai_Unk62383c != -1) {
        d2_RemoveObj(ai_Unk62383c);
        ai_Unk62383c = -1;
    }
    if (ai_Unk623848 != -1) {
        d2_RemoveObj(ai_Unk623848);
        ai_Unk623848 = -1;
    }
    if (ai_Unk62384c != -1) {
        d2_RemoveObj(ai_Unk62384c);
        ai_Unk62384c = -1;
    }
    if (ai_Unk6238a4 != -1)
        d2_EnableObject(ai_Unk6238a4, 0);
}
/* 0x5223c0, destroys the screen: the widgets of the table 0x7662e0, the groundplan views 0x6f22c4 / 0x6f22c8 (0x478fc0), the background 0x6f22c0, the button 0x6238a4, the strips 0x623850 / 0x623840, the panel
   widgets (0x522280), 0x529fb0, and sets the flag 0x6237cc */

void sv_DestroyScreen(void)
{
    int *q;

    q = sv_GroundplanButtons;
    do {
        if (*q != -1) {
            d2_RemoveObj(*q);
            *q = -1;
        }
        q++;
    } while ((int)q < (int)((unsigned)&sv_GroundplanButtons + 0x3c));
    if (sv_GroundplanSurf != 0) {
        d2_FreeSurface(sv_GroundplanSurf);
        sv_GroundplanSurf = 0;
    }
    if (sv_GroundplanView != 0) {
        d2_FreeSurface(sv_GroundplanView);
        sv_GroundplanView = 0;
    }
    if (ai_Unk6f22c0 != 0) {
        mem_Free(ai_Unk6f22c0);
        ai_Unk6f22c0 = 0;
    }
    if (ai_Unk6238a4 != -1) {
        d2_RemoveObj(ai_Unk6238a4);
        ai_Unk6238a4 = -1;
    }
    if (ai_Unk623850 != -1)
        d2_RemoveHitRect(ai_Unk623850);
    sv_Unk522280();
    sv_PanelPic = -1;
    if (ai_Unk623840 != -1) {
        d2_RemoveWindow(ai_Unk623840);
        ai_Unk623840 = -1;
    }
    sv_Unk529fb0();
    ai_Unk6237cc = 1;
}

extern char *ai_Goods;              /* 0x91aaf0 */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
int d2_BlitPartMode();                     /* 0x479150 */  /* @original: sv.c's call at 1099 passes 1 argument to d2.h's 8-parameter prototype */
extern int ai_Unk6f22dc, sv_ResearchClass, sv_ScreenDirty, sv_GroundplanBuilding, sv_GroundplanRoom, sv_GroundplanScene;  /* 0x6f22dc, 0x6f233c, 0x6f2358, 0x6f235c, 0x6f2360, 0x623e8c */
extern int sv_ResearchClass;  /* 0x6f233c, the class number of the research list the panel shows (gm_BuildingClass) */
extern char d2_GfxDir[];             /* 0xf8ef80, the data directory */
extern char sv_GroundplanColours[];
extern int d3_CurrentScene;                /* 0x633444 */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */
struct D2SURF * d2_LoadBmpSurface();                     /* 0x478b50 */
void d2_GetPixelColour();                     /* 0x4798f0 */
int d2_PutPixelRGB();                     /* 0x4799e0 */
void ai_SwitchRoom(int param_1, unsigned short param_2);                     /* 0x59e0a0 */
unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);                     /* 0x503530 */
int d2_FindPic();                     /* 0x468890 */
char *_strupr(char *);                     /* 0x5ea986 */
#define ai_SwitchRoomI ((void (*)(int, int))ai_SwitchRoom)  /* @original: sv_UpdateGroundplan zero-extends the room word into an int argument (xor edx,edx / mov dx,[esi]) */
extern int d2_PicShift;                   /* 0x6ed98c, d2.h (not includable here) */
#define d2_PutPixelRGBc ((int (*)(int, int, unsigned char, unsigned char, unsigned char, struct D2SURF *))d2_PutPixelRGB)  /* @original: sv_LoadGroundplan pushes the colour bytes as dwords loaded from the pixel buffer (char parameters, gfx.c masks them) */
int d2_CompareColour();                     /* 0x478930 */
int gm_BuildingClass(int kind);                /* 0x501d80, f3gm.c, the class of a building kind */
int gm_GoodPicture();                     /* 0x502570 */
int d2_CreatePicObj();                     /* 0x46cd80 */
#define d2_CreatePicObjS ((int (*)(short, short, int))d2_CreatePicObj)  /* @original: sv_LoadGroundplan adds d2_ScreenW + x in 16 bits (a short x, as d2.c's callers) */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
int gm_ResearchClass(int k, int *out);         /* 0x50b090, f3gm.c, the research class of a building kind */

/* 0x5224a0, loads the groundplan of the selected building (ai_PlayerBuilding, else ai_FallbackBuilding):
   bmp\groundplans\riss_<type name>.bmp, else one by the building class (gm_BuildingClass 1: wirtshaus /
   parfuemerie / tinkturei / the name / handwerksbetrieb, 3: rathaus / Arbeiterunterkunft, 5: zunfthaus, other:
   geldleihe / lagerhaus / kirche / stadtwache / the name / handwerksbetrieb); copies it into sv_GroundplanView,
   then reads the cell map <name>_C.bmp: a white pixel marks a cell, the pixel right of it one of the 15
   sv_GroundplanColours, the n-th colour the n-th good of the building's type (kinds 2 / 6, its slot list) or
   the n-th offered object (type kind 2); one picture button per cell into sv_GroundplanButtons
   @deep read whole against the original and reconstructed (strings, per-branch sprintf, class/kind chains,
   char-parameter PutPixelRGB, short CreatePicObj x, strcat _C.bmp, kind-2 query branch first, slot loop);
   411 vs 412 ins. Left: the cell map's y loop is top-tested in the original (0x5227d1 cmp ebx,[eax+8]; jge
   0x522974 ... 0x522913 jmp 0x5227d1, exit block xor ebp,ebp re-zeroing the cached 0) where ours is
   rotated (cmp [eax+8],ebp; jle / bottom mov ecx,[eax+8]; inc ebx; cmp ebx,ecx; jl) and our epilogue
   follows the loop; for / while / for(;;)+break / goto loop all compile identically, so the form is the
   optimizer's (the 0-register ebp reused as i in the loop), not the source. Also 0x5228c4 mov cl,[eax+ebx]
   vs mov ecx,eax; add ecx,ebx; mov cl,[ecx] (addressing choice of ai_GoodsT[id].type, same with a char*
   index) */
void sv_LoadGroundplan(void)
{
    AIBLD *b;
    AIBTYPE *bt;
    int x, y, i, n, slot, cnt, pic, cls;
    unsigned id;
    char kind;
    unsigned short *obj;
    unsigned char white[3];
    unsigned char rgb[4];
    char name[0x100];

    white[0] = 0xff;
    white[1] = 0xff;
    white[2] = 0xff;
    if (ai_PlayerBuilding != 0)
        b = (AIBLD *)ai_PlayerBuilding;
    else
        b = (AIBLD *)ai_FallbackBuilding;
    if (sv_GroundplanSurf != 0) {
        d2_FreeSurface(sv_GroundplanSurf);
        sv_GroundplanSurf = 0;
    }
    bt = &ai_BTypesT[b->type];
    sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->pad001);
    sv_GroundplanSurf = d2_LoadBmpSurface(name);
    if (sv_GroundplanSurf == 0) {
        cls = gm_BuildingClass(b->type);
        if (cls == 1) {
            if (bt->kind == 0x16)
                sprintf(name, "%sbmp\groundplans\riss_wirtshaus.bmp", d2_GfxDir);
            else if (bt->kind == 0x0e)
                sprintf(name, "%sbmp\groundplans\riss_parfuemerie.bmp", d2_GfxDir);
            else if (bt->kind == 8)
                sprintf(name, "%sbmp\groundplans\riss_tinkturei.bmp", d2_GfxDir);
            else {
                sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->pad001);
                sv_GroundplanSurf = d2_LoadBmpSurface(name);
                if (sv_GroundplanSurf != 0)
                    goto loaded;
                sprintf(name, "%sbmp\groundplans\riss_handwerksbetrieb.bmp", d2_GfxDir);
            }
        } else if (cls == 3) {
            if (bt->kind == 0x0f)
                sprintf(name, "%sbmp\groundplans\riss_rathaus.bmp", d2_GfxDir);
            else if (bt->kind == 1)
                sprintf(name, "%sbmp\groundplans\Riss_Arbeiterunterkunft.bmp", d2_GfxDir);
        } else if (cls == 5)
            sprintf(name, "%sbmp\groundplans\riss_zunfthaus.bmp", d2_GfxDir);
        else if (bt->kind == 5)
            sprintf(name, "%sbmp\groundplans\riss_geldleihe.bmp", d2_GfxDir);
        else if (bt->kind == 9)
            sprintf(name, "%sbmp\groundplans\riss_lagerhaus.bmp", d2_GfxDir);
        else if (bt->kind == 7)
            sprintf(name, "%sbmp\groundplans\riss_kirche.bmp", d2_GfxDir);
        else if (bt->kind == 0x13)
            sprintf(name, "%sbmp\groundplans\riss_stadtwache.bmp", d2_GfxDir);
        else {
            sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->pad001);
            sv_GroundplanSurf = d2_LoadBmpSurface(name);
            if (sv_GroundplanSurf != 0)
                goto loaded;
            sprintf(name, "%sbmp\groundplans\riss_handwerksbetrieb.bmp", d2_GfxDir);
        }
        if (sv_GroundplanSurf == 0) {
            sv_GroundplanSurf = d2_LoadBmpSurface(name);
            if (sv_GroundplanSurf == 0)
                return;
        }
    }
loaded:
    d2_LockSurface(sv_GroundplanSurf);
    d2_LockSurface(sv_GroundplanView);
    for (y = 0; y < sv_GroundplanSurf->h; y++)
        for (x = 0; x < sv_GroundplanSurf->w; x++) {
            d2_GetPixelColour(x, y, sv_GroundplanSurf, rgb);
            d2_PutPixelRGBc(x, y, rgb[0], rgb[1], rgb[2], sv_GroundplanView);
        }
    d2_UnlockSurface(sv_GroundplanView);
    if (sv_GroundplanSurf != 0) {
        d2_FreeSurface(sv_GroundplanSurf);
        sv_GroundplanSurf = 0;
    }
    *strrchr(name, '.') = 0;
    strcat(name, "_C.bmp");
    sv_GroundplanSurf = d2_LoadBmpSurface(name);
    n = 0;
    d2_LockSurface(sv_GroundplanSurf);
    y = 0;
    if (sv_GroundplanSurf != 0) {
        while (y < sv_GroundplanSurf->h) {
            for (x = 0; x < sv_GroundplanSurf->w; x++) {
                d2_GetPixelColour(x, y, sv_GroundplanSurf, rgb);
                if (d2_CompareColour(rgb, white) != 0)
                    continue;
                d2_GetPixelColour(x + 1, y, sv_GroundplanSurf, rgb);
                for (i = 0; i < 15; i++)
                    if (d2_CompareColour(rgb, sv_GroundplanColours + i * 3) == 0)
                        break;
                if (i >= 15)
                    continue;
                cnt = 0;
                if (bt->kind == 2) {
                    for (obj = (unsigned short *)gm_Query((void *)b->objList, 1, 4, 2); obj != 0; obj = gm_QueryNext())
                        if (*obj != 0xfd) {
                            if (cnt == i)
                                break;
                            cnt++;
                        }
                } else {
                    obj = 0;
                    for (slot = 0; slot < 0x40; slot++) {
                        if (bt->slot[slot] == 0xffff)
                            continue;
                        id = bt->slot[slot] & 0x7fff;
                        kind = ai_GoodsT[id].type;
                        if ((kind == 2 || kind == 6) && id != 0xfd) {
                            if (cnt == i) {
                                obj = (unsigned short *)gm_Query((void *)b->objList, 1, 0, id);
                                break;
                            }
                            cnt++;
                        }
                    }
                }
                if (obj != 0 && (pic = gm_GoodPicture(obj)) != -1) {
                    sv_GroundplanButtons[n] = d2_CreatePicObjS(d2_ScreenW + x - 0x7e, y + 0x4c, pic);
                    n++;
                }
            }
            y++;
        }
    }
    if (sv_GroundplanView != 0)
        d2_UnlockSurface(sv_GroundplanView);
    if (sv_GroundplanSurf != 0)
        d2_UnlockSurface(sv_GroundplanSurf);
}
/* 0x5229b0, the per-frame groundplan of the panel: rebuilds the cell buttons (sv_LoadGroundplan) when the scene,
   the player's building or its room changed since the last build (or the screen is dirty); without a building
   draws the arms of the own city (_STADTWAPPEN_<slot name>, else _STADTWAPPEN_BERLIN), else blits the groundplan;
   with flags 0x2000 and the pointer on a cell (sv_GroundplanColours index n) finds the n-th good of the player's
   building (or the n-th offered object) and of the fallback building: on a left click one the player may use
   (ai_ScriptQuery 0x19, gm_PermissionBits 0x4000) is entered (ai_SwitchRoom) or selected for the panel
   (sv_SelectPanelObject), otherwise its caption is shown; clears sv_ScreenDirty
   @deep reconstructed whole against the original (city-arms branch, the pointer-cell lookup, both building
   parts with the script / permission checks, sv_ScreenDirty cleared on every path); 407 vs 411 ins. Left
   only the goods-kind load in the two slot loops: 0x522c86 / 0x522db4 ours mov cl,[eax+ebp] where the
   original materializes the address (mov ecx,eax; add ecx,ebp; mov cl,[ecx]) of ai_GoodsT[id].type; tried
   a pointer local (AIGOOD *good), a char* index (ai_Goods[id * 0x41]), id as int / unsigned / unsigned
   short (the last breaks the frame): all the same or worse, so it is instruction selection, not the
   source. sv_LoadGroundplan has the same pair */
void sv_UpdateGroundplan(unsigned flags)
{
    int i, x, y, cnt, slot, pic;
    void *shape;
    unsigned id;
    char kind;
    AIBLD *b;
    AIBTYPE *bt;
    AIOBJ *obj;
    unsigned char rgb[4];
    char name[0x80];
    char path[0x80];

    if (d3_CurrentScene != sv_GroundplanScene || (int)sv_PanelShown != sv_GroundplanRoom || (int)ai_PlayerBuilding != sv_GroundplanBuilding || sv_ScreenDirty != 0) {
        sv_GroundplanBuilding = (int)ai_PlayerBuilding;
        sv_GroundplanRoom = (int)sv_PanelShown;
        for (i = 0; i < 15; i++) {
            if (sv_GroundplanButtons[i] != -1) {
                d2_RemoveObj(sv_GroundplanButtons[i]);
                sv_GroundplanButtons[i] = -1;
            }
        }
        if (ai_PlayerBuilding != 0 || ai_FallbackBuilding != 0)
            sv_LoadGroundplan();
        sv_GroundplanScene = d3_CurrentScene;
    }
    if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0) {
        strcpy(name, ai_PlayerSlots[ai_OwnPlayer]);
        _strupr(name);
        sprintf(path, "_STADTWAPPEN_%s", name);
        pic = d2_FindPic(path);
        if (pic == -1)
            pic = d2_FindPic("_STADTWAPPEN_BERLIN");
        shape = d2_GetPic(pic);             /* sets d2_PicShift */
        d2_DrawShape(d2_ScreenW - 0x6e, 0x5b, d2_ScreenSurf, shape, d2_PicShift + 1);
    } else {
        if (sv_GroundplanView != 0)
            d2_BlitPartMode(0, 0, sv_GroundplanView->w, sv_GroundplanView->h, sv_GroundplanView, d2_ScreenW - 0x6e, 0x5b, d2_ScreenSurf, 1);
        if (flags & 0x2000) {
            x = dm_L.x - d2_ScreenW + 0x72;
            y = dm_L.y - 0x59;
            if (sv_GroundplanSurf != 0 && x >= 0 && x < sv_GroundplanSurf->w && y >= 0 && y < sv_GroundplanSurf->h) {
                d2_LockSurface(sv_GroundplanSurf);
                d2_GetPixelColour(x, y, sv_GroundplanSurf, rgb);
                for (i = 0; i < 15; i++)
                    if (d2_CompareColour(rgb, sv_GroundplanColours + i * 3) == 0)
                        break;
                d2_UnlockSurface(sv_GroundplanSurf);
                if (i < 15) {
                    b = (AIBLD *)ai_PlayerBuilding;
                    if (b != 0) {
                        obj = 0;
                        bt = &ai_BTypesT[b->type];
                        cnt = 0;
                        if (bt->kind == 2) {
                            for (obj = (AIOBJ *)gm_Query((void *)b->objList, 1, 4, 2); obj != 0; obj = (AIOBJ *)gm_QueryNext())
                                if (obj->id != 0xfd) {
                                    if (cnt == i)
                                        break;
                                    cnt++;
                                }
                        } else {
                            for (slot = 0; slot < 0x40; slot++) {
                                if (bt->slot[slot] == 0xffff)
                                    continue;
                                id = bt->slot[slot] & 0x7fff;
                                kind = ai_GoodsT[id].type;
                                if ((kind == 2 || kind == 6) && id != 0xfd) {
                                    if (cnt == i) {
                                        obj = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, id);
                                        break;
                                    }
                                    cnt++;
                                }
                            }
                        }
                        if (dm_L.lclick != 0 && obj != 0 && obj->id != *sv_PanelShown
                            && ai_ScriptQuery(0x19, (int)ai_PlayerBuilding, 0, obj->id) == 1
                            && !(ai_CharsT[ai_PlayerId].unk1b1 > 0)) {
                            if (gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_PlayerBuilding, &obj->id, 0) & 0x4000)
                                ai_SwitchRoomI(-1, obj->id);
                        } else if (obj != 0)
                            d2_CopyCaption((char *)txt_ObjectNames[obj->id][0]);
                    }
                    b = (AIBLD *)ai_FallbackBuilding;
                    if (b != 0) {
                        bt = &ai_BTypesT[b->type];
                        obj = 0;
                        cnt = 0;
                        for (slot = 0; slot < 0x40; slot++) {
                            if (bt->slot[slot] == 0xffff)
                                continue;
                            id = bt->slot[slot] & 0x7fff;
                            kind = ai_GoodsT[id].type;
                            if ((kind == 2 || kind == 6) && id != 0xfd) {
                                if (cnt == i) {
                                    obj = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, id);
                                    break;
                                }
                                cnt++;
                            }
                        }
                        if (dm_L.lclick != 0 && obj != 0
                            && (sv_PanelShown == 0 || (obj->id != *sv_PanelShown && ai_ScriptQuery(0x19, (int)ai_FallbackBuilding, 0, obj->id) == 1))
                            && !(ai_CharsT[ai_PlayerId].unk1b1 > 0)) {
                            if (gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_FallbackBuilding, &obj->id, 0) & 0x4000)
                                sv_SelectPanelObject((unsigned char *)ai_FallbackBuilding, obj->objId, ch_EmptyName);
                        } else if (obj != 0)
                            d2_CopyCaption((char *)txt_ObjectNames[obj->id][0]);
                    }
                }
            }
        }
    }
    sv_ScreenDirty = 0;
}
/* 0x522ec0, remembers the research list of the building o for the panel (sv_ResearchClass): the category 3 (0x501d80,
   the research buildings) sets it to 1, the category 5 takes the class number of the building kind (gm_BuildingClass) */

void sv_SetResearchClass(unsigned char *o)
{
    int k;
    int v;

    k = gm_BuildingClass(*o);
    if (k == 3) {
        sv_ResearchClass = 1;
        return;
    }
    if (k == 5) {
        if (gm_ResearchClass(*o, &v) != 0)
            sv_ResearchClass = v;
    }
}

/* 0x46d040 */
void d2_BlitPart();                     /* 0x479480 */
struct D2FADE;
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */                     /* 0x4767d0 */
void d2_ReleaseFade(struct D2FADE *f); /* 0x476860, d2.h */                     /* 0x476860 */
void d2_PlaceWindow(int win, unsigned char flags);  /* 0x4756b0 */
int ahm_HasRecord();                     /* 0x4eaad0 */
struct D3SKY; struct D3SKYLAYER; struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
char * ai_Unk4cda50(char *c);                     /* 0x4cda50 */

/* 0x524010, redraws the whole screen: the background view 0x6ed924 is filled (0x46d040), the panel 0x522f10 (1, 0) built into it, the view blitted (0x479480) to 0x6ed92c, a black clip rectangle ("BLACK") over the
   whole screen shown for one message round (0x5309a0 (0, 0x1c6)), the strip 0x623840 shown (0x4756b0) and the background copied back */
void sv_RedrawScreen(void)
{
    int r;

    d2_ClearSurface(d2_ScreenSurf);
    d2_DrawPic(0, 0, sv_PanelPic, d2_ScreenSurf);
    if (d2_LockSurface(d2_ScreenSurf) != 0) {
        sv_UpdateCityPanel(1, 0);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_BlitPart(0, 0, *(int *)(ai_Unk6ed92c + 4), *(int *)(ai_Unk6ed92c + 8), d2_ScreenSurf, 0, 0, ai_Unk6ed92c);
    r = d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0, 1);
    sv_RunFrame(0, 0x1c6);
    d2_ReleaseFade(r);
    d2_PlaceWindow(ai_Unk623840, 1);
    d2_BlitPart(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, ai_Unk6ed92c, 0, 0, d2_ScreenSurf);
}
/* 0x524100, enters the character c (index) into the first free (-1) of the 8 member slots of the group record
   g, unless he is a member already; a member slot holds the character's AICHAR.netId */

void sv_AddGroupMember(int c, char *g)
{
    AICHAR *ch;
    int i;

    ch = &ai_CharsT[c];
    for (i = 0; i < 8; i++)
        if (((SVGROUP *)g)->member[i] == ch->netId)
            break;
    if (i < 8)
        return;
    for (i = 0; i < 8; i++)
        if (((SVGROUP *)g)->member[i] == -1) {
            ((SVGROUP *)g)->member[i] = ch->netId;
            return;
        }
}
/* 0x524160, removes the character c (index) from the member slots (+0x8c) of the group record g @deep @deep: behaviour draft, readable done. 11 code hunks left, two causes, both entry scheduling / dead values: (1) the original materialises the character base (lea ecx,[eax*4+0x770b80]) and then never loads through it - it loads the id with the SIB form mov edx,[eax*4+0x770b84]; ours emits only the load. Tried: 'ch = ai_Chars[c]; id = *(int *)(ch + 4)' (CSE, no lea), the typed ((SVCHARREC *)ai_Chars[c])->netId (no lea), the char[0x21c] index form - all 11; (2) the original computes p = g+0x8c after 'i = 0' (xor eax,eax; lea ecx,[esi+0x8c]), ours hoists the lea above the id computation - the statement order is already id / i = 0 / p = ..., a scheduler choice; spelling p as grp->member + i changes the lea to a SIB form (worse). Kept: id via SVCHARREC.netId, the scan through SVGROUP.member (the pointer scan + indexed store, the original's add ecx,4 loop), which made it readable (Q4 clear). Nothing else in the body differs: the do-while, the -1 store and the 8 bound are the original's. */

void sv_RemoveGroupMember(int c, char *g)
{
    int i, id;
    int *p;
    SVGROUP *grp;

    id = ((SVCHARREC *)ai_Chars[c])->netId;
    i = 0;
    grp = (SVGROUP *)g;
    p = grp->member;
    do {
        if (*p == id) {
            grp->member[i] = -1;
            return;
        }
        i++;
        p++;
    } while (i < 8);
}
/* 0x5241b0, the number of used member slots (+0x8c, 8 of them) of the group record g */

int sv_UsedGroupSlots(char *g)
{
    int n, i;
    int *p;

    n = 0;
    p = (int *)(g + 0x8c);
    i = 8;
    do {
        if (*p != -1)
            n++;
        p++;
        i--;
    } while (i != 0);
    return n;
}
/* 0x5241d0, counts the members of the group record g by their home building (+0x16c): +0x14 those whose title byte (+0x165) is the building's byte +0x22f (or, for kind 0x1e, accepted by 0x4eaad0), +0x18 those with +0x230 */

void sv_CountGroupMembers(char *g)
{
    int i;
    int *p;
    char *c;
    char *b;

    *(int *)(g + 0x14) = 0;
    *(int *)(g + 0x18) = 0;
    p = (int *)(g + 0x8c);
    i = 8;
    do {
        if (*p != -1) {
            c = gm_FindChar(*p);
            if (c != 0 && *(unsigned char **)(c + 0x16c) != 0) {
                b = ai_BTypes + **(unsigned char **)(c + 0x16c) * 0x24d;
                if (c[0x165] == b[0x22f]) {
                    (*(int *)(g + 0x14))++;
                } else if (c[0x165] == b[0x230]) {
                    (*(int *)(g + 0x18))++;
                } else if (b[0] == 0x1e) {
                    if (ahm_HasRecord(c) != 0)
                        (*(int *)(g + 0x14))++;
                }
            }
        }
        p++;
        i--;
    } while (i != 0);
}

extern float gm_DayStartHours[], gm_DayEndHours[];   /* 0x617830 / 0x617840 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
int init_ObjectSceneUniverse();                     /* 0x511dc0 */
int gm_ClockSeason(void *t);           /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
struct CHACTION * ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);                   /* 0x40b2f0 */
char *strstr();                   /* 0x5de6b0, strstr (LIBC; as ls.c) */
void cm_SendExSetHE(unsigned char k, char *p);                     /* 0x4c4c60 */
#define cm_SendExSetHEI ((void (*)(int, char *))cm_SendExSetHE)  /* @original: sv_MoveToGroup pushes -1, not 0xff (an int / signed parameter at its calls) */
void he_FreeEvent();                     /* 0x53e1f0 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */

/* 0x524260, moves the character c (index) from his current job (the group record AICHAR.job) to the job g;
   o is the building and it the room the character goes to (o defaults to his building): when the room is in
   another scene, it is day time (gm_DayStartHours .. gm_DayEndHours of the season) or the building is of kind
   4 / 0x13 / 0x1d, his model is sent there (the actions 0x33 / 0x39 tagged "sv_ChangePlayerHE") and its script
   stopped. He leaves the old job (sv_RemoveGroupMember) and the job is finished by its kind (the member count,
   the time an emptied job was left, he_FreeEvent, the pause script, cm_SendExSetHE for an empty job); without
   an old job a running "Pause" / "pause" script of the model is stopped. Then he joins g (sv_AddGroupMember)
   and the new job kind (field 0x180) is sent and the job set when it changed
   @deep read against the original and fixed: room (*it != 0xfd) polarity, w >= 0, the action arguments
   o->id / it->objId, old != 0 first, the case bodies in the original's order (0x28 before 0x14; tables now
   equal), <= 0 slot tests, 'pause' before 'Pause', g != 0 first, newkind = 0 at entry, the job time as an
   AITIME struct copy (memcpy took esi/edi), cm_SendExSetHE(-1) as an int push; typed SVCHARREC / SVGROUP /
   SVMODEL. Instruction count equal (287). Left is allocation only: with --regs ebp=esi,esi=ebx,ebx=ebp 16
   lines differ, the callee-save push order (ours push ebp first / ebx later, the original push ebx first /
   ebp later), and ev taking ebp where the original reuses old's esi in the no-old path; ch / old / it
   rotate over ebx / esi / ebp */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g)
{
    SVCHARREC *ch;
    SVGROUP *old;
    AIBTYPE *bt;
    char *msg;
    EVTSCRIPT *ev;
    int w, k;
    char newkind;

    newkind = 0;
    if (c >= 0x300)
        return;
    ch = (SVCHARREC *)ai_Chars[c];
    old = ch->group;
    if ((char *)old == g)
        return;
    if (o == 0)
        o = ch->bld;
    if (it != 0) {
        bt = &ai_BTypesT[*o];
        if (*it != 0xfd)
            w = init_ObjectSceneUniverse(it);
        else
            w = init_BuildingSceneUniverse(o);
        k = gm_ClockSeason(ai_Time);
        if ((((float)ai_TimeT.hour < gm_DayEndHours[k] && gm_DayStartHours[k] <= (float)ai_TimeT.hour) || bt->kind == 4 || bt->kind == 0x13 || bt->kind == 0x1d)
            && w >= 0 && ch->model != 0 && &d3_Scenes[w] != ch->model->scene) {
            msg = (char *)ch_AddAction((struct CHARACTER *)ch->model, 0x33, w, ((AIBLD *)o)->id, ((AIOBJ *)it)->objId);
            strcpy(msg + 0x70, "sv_ChangePlayerHE");
            msg = (char *)ch_AddAction((struct CHARACTER *)ch->model, 0x39, w);
            strcpy(msg + 0x70, "sv_ChangePlayerHE");
            if (ch->model->scriptId != -1) {
                ev = evt_FindById(ch->model->scriptId);
                if (ev != 0)
                    evt_Stop(ev);
            }
        }
    }
    if (old != 0) {
        sv_RemoveGroupMember(ch->id, (char *)old);
        switch (old->kind) {
        case 3:
        case 4:
        case 8:
        case 9:
            sv_CountGroupMembers((char *)old);
            break;
        case 0x28:
            if (ch->model != 0 && ch->model->scriptId != -1)
                evt_Stop(evt_FindById(ch->model->scriptId));
            if (old->state < 3) {
                old->time = ai_TimeT;
                old->state = -1;
            }
            break;
        case 0x14:
        case 0x15:
        case 0x8c:
            sv_CountGroupMembers((char *)old);
            if (sv_UsedGroupSlots((char *)old) <= 0) {
                old->time = ai_TimeT;
                cm_SendExSetHEI(-1, (char *)old);
            }
            break;
        case 0x48:
        case 0x49:
        case 0x75:
        case 0xa1:
            if (sv_UsedGroupSlots((char *)old) <= 0)
                cm_SendExSetHEI(-1, (char *)old);
            break;
        case 0x16:
        case 0x88:
        case 0x89:
        case 0x8d:
        case 0x8f:
            he_FreeEvent(old);
        }
    } else if (ch->model != 0 && ch->model->scriptId != -1) {
        ev = evt_FindById(ch->model->scriptId);
        if (ev != 0 && (strstr((char *)ev, "pause") != 0 || strstr((char *)ev, "Pause") != 0))
            evt_Stop(ev);
    }
    if (g != 0) {
        sv_AddGroupMember(ch->id, g);
        switch ((unsigned char)g[0]) {
        case 3:
        case 4:
        case 8:
        case 9:
        case 0x14:
        case 0x15:
        case 0x8c:
            sv_CountGroupMembers(g);
        }
        newkind = g[0];
    } else
        newkind = 0;
    if ((ch->group != 0 && g == 0) || newkind != 0) {
        cm_BeginFieldBlock(ch, ch->netId);
        cm_AddFieldValue(1, 1, 0x180, &newkind);
        cm_SendFieldValues();
        ch->group = (SVGROUP *)g;
    }
}
/* 0x524770, 1 when the character g's model stands at the door of the building o: the door ("dummy_TUER" of
   the building's object, else the object itself) is within 750.0 of the model's body and the model is idle (no
   queued action); or when the model's group is the building's id */

int sv_IsAtDoor(char *g, char *o)
{
    SVMODEL *dst;
    D3OBJECT *door;
    float pos[4];

    dst = ((SVCHARREC *)g)->model;
    if (((AIBLD *)o)->object != 0) {
        if (dst == 0)
            return 0;
        door = d3_FindObject((int)((AIBLD *)o)->object, 0x100, 0, "dummy_TUER");
        if (door != 0)
            d3_PointToWorld(door, &door->pos, (D3VECTOR *)pos);
        else
            d3_PointToWorld((D3OBJECT *)((AIBLD *)o)->object, &((D3OBJECT *)((AIBLD *)o)->object)->pos, (D3VECTOR *)pos);
        if (d3_VecEqual(&dst->obj->pos, (D3VECTOR *)pos, 750.0f) != 0 && dst->actions == 0)
            return 1;
    }
    if (dst != 0 && dst->group == ((AIBLD *)o)->id)
        return 1;
    return 0;
}
/* 0x524810, sv_IsAtDoor for a cart g (AIOBJ, its scene character +0x3b), only while that character is in the
   first scene: within 650.0 of the building o's door (1950.0 for a building type of kind 10, 3250.0 from the
   building's object when it has no "dummy_TUER"); or when the character's group is the building's id */

int sv_IsCartAtDoor(char *g, unsigned char *o)
{
    float r;
    SVMODEL *dst;
    D3OBJECT *door;
    float pos[4];

    r = 650.0f;
    if (ai_BTypesT[*o].kind == 10)
        r = 1950.0f;
    dst = (SVMODEL *)((AIOBJ *)g)->sceneObj;
    if (((AIBLD *)o)->object != 0) {
        if (dst == 0)
            return 0;
        if (dst->scene == &d3_Scenes[0]) {
            door = d3_FindObject((int)((AIBLD *)o)->object, 0x100, 0, "dummy_TUER");
            if (door != 0)
                d3_PointToWorld(door, &door->pos, (D3VECTOR *)pos);
            else {
                d3_PointToWorld((D3OBJECT *)((AIBLD *)o)->object, &((D3OBJECT *)((AIBLD *)o)->object)->pos, (D3VECTOR *)pos);
                r = 3250.0f;
            }
            if (d3_VecEqual(&dst->obj->pos, (D3VECTOR *)pos, r) != 0)
                return 1;
        }
    }
    if (dst != 0 && dst->group == ((AIBLD *)o)->id)
        return 1;
    return 0;
}
/* 0x524900, collects the image numbers for the character c into out[]: from the table 0x75ec98 (records of 8 words: [-6] the kind, [0] the first image, [1] the index of a short in the character's +0x17c record + 0xdf)
   and the image of 0x4cda50 (+ 0xdf); returns the count */

int sv_CharStateIcons(char *c, int *out)
{
    unsigned char *r;
    int n, i, *p;
    int *q;

    r = *(unsigned char **)(c + 0x17c);
    n = 0;
    if (r != 0 && *r != 0) {
        i = 0;
        p = (int *)(unsigned)&full_data_75ec98;
        for (; (int)p < (int)((unsigned)&full_data_75ec98 + 0x600); p += 8, i++) {
            if (p[-6] == *r) {
                out[n++] = *p;
                if (p[1] != -1) {
                    out[n++] = *(unsigned short *)(r + ((int *)(unsigned)&full_data_75ec9c)[i * 8]) + 0xdf;
                    break;
                }
            }
        }
    }
    q = (int *)ai_Unk4cda50(c);
    if (q != 0 && *q != 0)
        out[n++] = *q + 0xdf;
    return n;
}

extern int d2_PointerObj;  /* 0x5fe710 */
extern int sv_SelectedChar;
extern char sv_PlayerBarKind;
#define sv_MemberRows (*(SVMEMBERROW (*)[0x20])&ai_Unk75f280)  /* 0x75f280, the member list rows (he.c and sv.c declare int[576]) */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);                     /* 0x472aa0 */
int d2_WinOpenPicStyled();                     /* 0x473350 */
#define d2_OpenFormS ((int (*)(short, short, char *))d2_OpenForm)  /* @original: sv_OpenPlayerBar computes x = d2_ScreenW - 0x68 in 16 bits (sub cx, 0x68; push ecx) */
#define d2_WinOpenPicStyledS ((int (*)(short, short, int, int, int))d2_WinOpenPicStyled)  /* @original: sv_UpdateMemberList computes the y of each call afresh (a short y, as d2_WinOpenPic's) */
int d2_CreateWinText(short x, short y, char *text, int win);                     /* 0x473650 */
void d2_SetObjText();                     /* 0x4734d0 */
int gm_TitleJobGroup();                     /* 0x504c00 */
void snd_PlayWorkerClick(struct G16CHARCLICK *c);                     /* 0x4c2c70 */
void d2_SetObjEnabled();                     /* 0x475e40 */
void d2_AddPointerItem(int id, int amount);                     /* 0x476b90 */
float gm_HealthRatio();                     /* 0x4fc8e0 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */

/* 0x524990, rebuilds the member list of the player bar for the building g: rows of characters that are gone
   (no model, dead) are freed; every living character in g (by sv_PlayerBarKind: 0 those of the building type's two
   titles without a job, 1 those of the job groups 10 .. 12, else all) gets a row (portrait button, frame, text,
   and for building kinds 0x10 / 0x13 / 4 a child window with two bars sv_DrawBalanceBar); a click on a row's button
   toggles the character's selection, a row is enabled while its character's model is sent to g, is not busy
   (flag 0x800) and has no job of kind 0x14; each row shows "name health%" and up to two state pictures
   (sv_CharStateIcons, styled for the pictures 0xdf .. 0x3bb); with the button 0x51d the selected characters leave
   their job (sv_MoveToGroup) */
void sv_UpdateMemberList(unsigned char *g)
{
    char *bt;
    SVMEMBERROW *r;
    int i, n, cnt, y;
    int im[8];
    int im0, im1;
    char text[0x80];
    char name[0x100];

    if (sv_PlayerBarForm == -1)
        return;
    bt = ai_BTypes + *g * sizeof(AIBTYPE);
    d2_SetForm(sv_PlayerBarForm, 1);
    d2_ClearPointerItems();
    for (n = 0; n < 0x20; n++) {
        r = &sv_MemberRows[n];
        if (r->chr != 0xffff && (((SVCHARREC *)ai_Chars[r->chr])->model == 0 || ((SVCHARREC *)ai_Chars[r->chr])->health == 0)) {
            if (r->icon[0] != -1) {
                d2_RemoveObj(r->icon[0]);
                r->icon[0] = -1;
            }
            if (r->icon[1] != -1) {
                d2_RemoveObj(r->icon[1]);
                r->icon[1] = -1;
            }
            d2_RemoveObj(r->check);
            r->check = -1;
            d2_RemoveObj(r->text);
            r->text = -1;
            d2_RemoveObj(r->frame);
            r->frame = -1;
            if (r->window != -1)
                d2_RemoveWindow(r->window);
            r->window = -1;
            r->chr = 0xffff;
        }
    }
    for (i = 0; i < 0x300; i++) {
        if (((SVCHARREC *)ai_Chars[i])->bld != g || ((SVCHARREC *)ai_Chars[i])->model == 0 || ((SVCHARREC *)ai_Chars[i])->health == 0)
            continue;
        if (sv_PlayerBarKind == 0) {
            if (!((((SVCHARREC *)ai_Chars[i])->title == bt[0x22f] || ((SVCHARREC *)ai_Chars[i])->title == bt[0x230]) && ((SVCHARREC *)ai_Chars[i])->jobId == 0))
                continue;
        } else if (sv_PlayerBarKind == 1) {
            n = gm_TitleJobGroup((unsigned char)((SVCHARREC *)ai_Chars[i])->title);
            if (((SVCHARREC *)ai_Chars[i])->jobId != 0)
                continue;
            if (n != 10 && n != 0xb && n != 0xc)
                continue;
        }
        for (n = 0; n < 0x20; n++)
            if (sv_MemberRows[n].chr == i)
                break;
        if (n < 0x20)
            continue;
        for (n = 0; n < 0x20; n++)
            if (sv_MemberRows[n].chr == 0xffff)
                break;
        if (n > 0x20)
            break;
        io_CharacterName(i, 1, 1, name);
        d2_SetForm(sv_PlayerBarForm, 1);
        y = n * 0x4e;
        sv_MemberRows[n].frame = d2_WinOpenPic(6, y, 0x5dc, d2_LastWindow);
        sv_MemberRows[n].check = d2_WinOpenPic(8, y + 0x11, ((SVCHARREC *)ai_Chars[i])->picture, d2_LastWindow);
        sv_MemberRows[n].text = d2_CreateWinText(0, y + 4, ch_EmptyName, d2_LastWindow);
        *(int *)(d2_Objs + 0x5c + sv_MemberRows[n].text * 0x2ec) = 1;
        *(short *)(d2_Objs + 0x14 + sv_MemberRows[n].text * 0x2ec) = 0x5f;
        *(int *)(d2_Objs + 0x48 + sv_MemberRows[n].check * 0x2ec) = 1;
        *(SVCHARREC **)(d2_Objs + 0x2e8 + sv_MemberRows[n].check * 0x2ec) = (SVCHARREC *)ai_Chars[i];
        if (sv_MemberRows[n].window != -1) {
            d2_RemoveWindow(sv_MemberRows[n].window);
            sv_MemberRows[n].window = -1;
        }
        sv_MemberRows[n].chr = i;
        sv_MemberRows[n].cleared1c = 0;
        if (bt[0] == 0x10 || bt[0] == 0x13 || bt[0] == 4) {
            d2_SetForm(sv_PlayerBarForm, 1);
            sv_MemberRows[n].window = d2_CreateChildWindow(6, y + 0x3f, 0x50, 0xf, 0, d2_LastWindow);
            d2_WinOpenPic(4, 1, 0x5df, d2_LastWindow);
            sv_DrawBalanceBar(0x10, 4, ai_Chars[sv_MemberRows[n].chr], 2, 0x4f1);
            d2_WinOpenPic(0x28, 1, 0x5e0, d2_LastWindow);
            sv_DrawBalanceBar(0x34, 4, ai_Chars[sv_MemberRows[n].chr], 3, 0x4f1);
        }
        if (((SVCHARREC *)ai_Chars[i])->selected != 0)
            d2_SetObjValue(sv_MemberRows[n].check, 1);
        else
            d2_SetObjValue(sv_MemberRows[n].check, 0);
    }
    for (n = 0; n < 0x20; n++) {
        r = &sv_MemberRows[n];
        if (d2_ClickedId != -1 && r->chr != 0xffff && d2_PointerObj == r->check) {
            if (*(int *)(d2_Objs + 0x28 + r->check * 0x2ec) != 0) {
                ((SVCHARREC *)ai_Chars[r->chr])->selected = 1;
                snd_PlayWorkerClick((struct G16CHARCLICK *)ai_Chars[r->chr]);
            } else
                ((SVCHARREC *)ai_Chars[r->chr])->selected = 0;
        }
        if (r->chr != 0xffff && ((SVCHARREC *)ai_Chars[r->chr])->model->group == ((AIBLD *)g)->id
            && (((SVCHARREC *)ai_Chars[r->chr])->flags & 0x800) == 0 && ((SVCHARREC *)ai_Chars[r->chr])->groupKind != 0x14)
            d2_SetObjEnabled(r->check, 1);
        else
            d2_SetObjEnabled(r->check, 0);
    }
    for (n = 0; n < 0x20; n++) {
        if (sv_MemberRows[n].chr == 0xffff)
            continue;
        if (((SVCHARREC *)ai_Chars[sv_MemberRows[n].chr])->selected != 0) {
            d2_AddPointerItem(*(int *)(d2_Objs + 8 + sv_MemberRows[n].check * 0x2ec), 1);
            d2_SetObjValue(sv_MemberRows[n].check, 1);
        } else
            d2_SetObjValue(sv_MemberRows[n].check, 0);
        im1 = -1;
        if (sv_MemberRows[n].icon[0] != -1) {
            d2_RemoveObj(sv_MemberRows[n].icon[0]);
            sv_MemberRows[n].icon[0] = -1;
        }
        if (sv_MemberRows[n].icon[1] != -1) {
            d2_RemoveObj(sv_MemberRows[n].icon[1]);
            sv_MemberRows[n].icon[1] = -1;
        }
        d2_SetForm(sv_PlayerBarForm, 1);
        if (sv_MemberRows[n].text != -1) {
            sprintf(text, "%s %i%%", ((SVCHARREC *)ai_Chars[sv_MemberRows[n].chr])->name, (int)(gm_HealthRatio(ai_Chars[sv_MemberRows[n].chr]) * 100.0));
            d2_SetObjText(sv_MemberRows[n].text, text);
            *(short *)(d2_Objs + 0x14 + sv_MemberRows[n].text * 0x2ec) = 0x5f;
        }
        cnt = sv_CharStateIcons(ai_Chars[sv_MemberRows[n].chr], im);
        if (cnt == 0)
            continue;
        if (cnt > 1)
            im1 = im[1];
        im0 = im[0];
        if (im0 != -1) {
            if (sv_MemberRows[n].icon[0] == -1) {
                if (im0 >= 0xdf && im0 <= 0x3bb)
                    sv_MemberRows[n].icon[0] = d2_WinOpenPicStyledS(0x38, n * 0x4e + 0x10, im0, d2_LastWindow, -2);
                else
                    sv_MemberRows[n].icon[0] = d2_WinOpenPic(0x38, n * 0x4e + 0x10, im0, d2_LastWindow);
            } else if (im0 != *(int *)(d2_Objs + 8 + sv_MemberRows[n].icon[0] * 0x2ec)) {
                d2_RemoveObj(sv_MemberRows[n].icon[0]);
                sv_MemberRows[n].icon[0] = -1;
                if (im0 >= 0xdf && im0 <= 0x3bb)
                    sv_MemberRows[n].icon[0] = d2_WinOpenPicStyledS(0x38, n * 0x4e + 0x10, im0, d2_LastWindow, -2);
                else
                    sv_MemberRows[n].icon[0] = d2_WinOpenPic(0x38, n * 0x4e + 0x10, im0, d2_LastWindow);
            }
        }
        if (im1 != -1) {
            if (sv_MemberRows[n].icon[1] == -1) {
                if (im1 >= 0xdf && im1 <= 0x3bb)
                    sv_MemberRows[n].icon[1] = d2_WinOpenPicStyledS(0x38, n * 0x4e + 0x28, im1, d2_LastWindow, -2);
                else
                    sv_MemberRows[n].icon[1] = d2_WinOpenPic(0x38, n * 0x4e + 0x28, im1, d2_LastWindow);
            } else if (im1 != *(int *)(d2_Objs + 8 + sv_MemberRows[n].icon[1] * 0x2ec)) {
                d2_RemoveObj(sv_MemberRows[n].icon[1]);
                sv_MemberRows[n].icon[1] = -1;
                if (im1 != 0)                   /* the original tests im1 != 0 here, not the range */
                    sv_MemberRows[n].icon[1] = d2_WinOpenPicStyledS(0x38, n * 0x4e + 0x28, im1, d2_LastWindow, -2);
                else
                    sv_MemberRows[n].icon[1] = d2_WinOpenPic(0x38, n * 0x4e + 0x28, im1, d2_LastWindow);
            }
        }
    }
    for (n = 0; n < 0x20; n++) {
        if (sv_MemberRows[n].chr != 0xffff) {
            i = ((SVCHARREC *)ai_Chars[sv_MemberRows[n].chr])->selected != 0;
            sv_MemberRows[n].selected = i;
            *(int *)(d2_Objs + 0x28 + sv_MemberRows[n].check * 0x2ec) = i;
            *(int *)(d2_Objs + 0x2c + sv_MemberRows[n].check * 0x2ec) = i;
        }
    }
    if (d2_ClickedId == 0x51d) {
        for (n = 0; n < 0x20; n++)
            if (sv_MemberRows[n].selected != 0)
                sv_MoveToGroup(g, 0, sv_MemberRows[n].chr, 0);
        sv_ClearSelection();
    }
    if (sv_SelectedChar != 0 && ((SVCHARREC *)sv_SelectedChar)->selected == 0)
        sv_SelectedChar = 0;
}

extern OPTIONBYTES ai_GfxResolution;        /* 0x6f2f94, the player bar variant */
extern int sv_WinterWeather;  /* 0x760610 */
extern int sv_CloudChance;  /* 0x760480 */
extern int sv_SeasonClimate;  /* 0x760484 */
extern int sv_WeatherHour;  /* 0x760614 */
extern int * d3t_Rain;  /* 0x760618 */
extern int d3t_Snow;  /* 0x76061c */
extern int sv_SkyPhase;  /* 0x760464 */
extern int sv_ThunderFog;  /* 0x6f2370 */
extern int d3_SkyLayers[6];  /* 0x760620 */
extern int ai_Unk633dd4;  /* 0x633dd4 */
extern int sv_SkyScene;  /* 0x75ec20 */
extern struct D3ANIM *d3t_CamMove;  /* 0x6310e8 */
extern int sv_LampPhase;  /* 0x623e80 */
extern int sv_DayPhase;  /* 0x6f236c */
extern char sv_LampSwitches;
extern float sv_SkyScale;  /* 0x760420 */
extern float sv_DayPhaseFrac;  /* 0x623e94 */
/* ai_MapRefs: view of d3_Scenes (tools/data_views.py) */
const int sv_PhaseMinutes[24] = { 390, 510, 630, 1050, 1170, 1290, 300, 420, 540, 1140, 1260, 1380, 420, 540, 660, 1020, 1140, 1260, 480, 600, 690, 960, 1020, 1140 };  /* 0x5eeab8 */

extern char *ai_Unk5fb5b0;              /* 0x5fb5b0 */
extern int ai_Unk623cc0[], ai_Unk623e10[];  /* 0x623cc0, 0x623e10 */
extern int ai_Unkfba9c0[], ai_Unk5fcc78, ai_Unk5fcc7c, ai_Unk5fcc80;  /* 0xfba9c0, 0x5fcc78, 0x5fcc7c, 0x5fcc80 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
void sv_UpdatePanel(void);                     /* 0x52a050 */
int d2_WinOpenPicStyled();                     /* 0x473350 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */
void ai_InitWind(int type, int id, float *from, float *to);                     /* 0x5dbc30 */
void d3t_FreeSnow();                     /* 0x414b50 */
struct D3TRAIN * d3t_CreateRain();                     /* 0x413b30 */
struct D3TRAIN; void d3t_SetRain(struct D3TRAIN *rain, int n, ...);                     /* 0x413d30 */
void d3t_DeleteRain();                     /* 0x413d00 */
struct D3TSNOW *d3t_CreateSnow();                     /* 0x4148f0 */
struct D3TSNOW; void d3t_SetSnow(struct D3TSNOW *snow, int n, ...);                     /* 0x414c60 */
struct D3SKY; struct D3SKY *d3sky_Create(float horizon, float fogstart, float fogscale);                     /* 0x45da30 */
struct D3SKY; struct D3SKYLAYER; void d3sky_DeleteLayer(struct D3SKY *s, struct D3SKYLAYER *l);                     /* 0x45d800 */
struct D3SKYLAYER; struct D3SKY; struct D3SKYLAYER *d3sky_CreateLayer(struct D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset, float vscale, float voffset);                     /* 0x45d880 */
void d3sky_SetLayerSpeed(struct D3SKY *s, struct D3SKYLAYER *l, float speed);  /* 0x45d960 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);                     /* 0x44a170 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);                     /* 0x449de0 */
void sv_UpdateSkyColors(void);
void sv_UpdateSkyPhase(AITIME *t);

/* 0x525220, opens the player bar ("Misc\\PlayerBar", or "Misc\\PlayerBar_1" at the other resolution) of the kind k at the right screen edge: its two scroll
   buttons (style 3) go to the window, the panel form is disabled, the 0x20 member rows are cleared, the button picture 0x51d is created and every character's
   member list selection is dropped */
void sv_OpenPlayerBar(int a, char k)
{
    int s, f, i;

    if (sv_PlayerBarForm != -1)
        return;
    sv_PlayerBarKind = k;
    if (ai_GfxResolution.byte[0] == 0)
        sv_PlayerBarForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "Misc\\PlayerBar");
    else
        sv_PlayerBarForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "Misc\\PlayerBar_1");
    d2_SetForm(sv_PlayerBarForm, 1);
    s = d2_LastWindow;
    d2_SetForm(sv_PlayerBarForm, 2);
    f = d2_GetWinObj(sv_PlayerBarForm, 2, 0);
    ((D2OBJ *)d2_Objs)[f].style = 3;
    d2_Wins[d2_LastWindow].btnup = f;
    ((D2OBJ *)d2_Objs)[f].winIndex = s;
    f = d2_GetWinObj(sv_PlayerBarForm, 2, 1);
    ((D2OBJ *)d2_Objs)[f].style = 3;
    d2_Wins[d2_LastWindow].btndown = f;
    ((D2OBJ *)d2_Objs)[f].winIndex = s;
    d2_Wins[s].visible = 0x4e;
    if (sv_PlayerBarForm != -1) {
        if (sv_PanelForm != -1)
            d2_SetViewEnabled(sv_PanelForm, 0);
        d2_SetForm(sv_PlayerBarForm, 1);
        for (i = 0; i < 0x20; i++) {
            sv_MemberRows[i].check = -1;
            sv_MemberRows[i].icon[0] = -1;
            sv_MemberRows[i].icon[1] = -1;
            sv_MemberRows[i].chr = 0xffff;
            sv_MemberRows[i].text = -1;
            sv_MemberRows[i].window = -1;
            sv_MemberRows[i].cleared1c = 0;
        }
        d2_SetForm(sv_PlayerBarForm, 2);
        f = d2_WinOpenPicStyled(0x11, 4, 0x51d, d2_LastWindow, -2);
        ((D2OBJ *)d2_Objs)[f].interactive = 1;
        ((D2OBJ *)d2_Objs)[f].style = 3;
        for (i = 0; i < 768; i++)
            if (((SVCHARREC *)ai_Chars[i])->selected != 0 && ((SVCHARREC *)ai_Chars[i])->jobId != 0)
                ((SVCHARREC *)ai_Chars[i])->selected = 0;
    }
}
/* 0x525440, closes the player bar (d2_CloseView, sv_PlayerBarForm), shows the info panel (sv_PanelForm) again and
   refreshes it (sv_UpdatePanel) */

void sv_ClosePlayerBar(void)
{
    d2_CloseView(sv_PlayerBarForm);
    sv_PlayerBarForm = -1;
    d2_ClearPointerItems();
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 1);
    sv_UpdatePanel();
}
/* 0x525480, the season of the clock/sky record t, forwarded to gm_ClockSeason (0x4fe350) as a callback */

int sv_ClockSeason(void *t)
{
    return gm_ClockSeason(t);
}
typedef struct SVSKYKEY {               /* 0x10 bytes, a light key's colour in the sky tables (D3LKEY.rgb / val) */
    float rgb[3];
    float val;
} SVSKYKEY;
#define sv_DayPhases     (*(SVDAYPHASE (*)[6])&sv_DayTables)  /* 0x623ba8, the day phases */
#define sv_SkyKeys       (*(SVSKYKEY (*)[7])((char *)&sv_DayTables + 0x38))  /* 0x623be0, the sky lights' keys */
#define sv_SkyKeysAlt    (*(SVSKYKEY (*)[7])((char *)&sv_DayTables + 0xa8))  /* 0x623c50, the same for the lights with objectFlags 0x10 */
#define sv_SkyKeysWinter (*(SVSKYKEY (*)[7])((char *)&ai_Unk623cc0 + 0x70))  /* 0x623d30, the keys in winter weather */
#define sv_SkyKeysWinterAlt (*(SVSKYKEY (*)[7])((char *)&ai_Unk623cc0 + 0xe0))  /* 0x623da0 */
/* 0x525490, the d3_ForEachObject callback over the sky scene's lights: copies the colours of the first 6 keys from the sky table
   (by sv_WinterWeather and the light's objectFlags 0x10) into the light's keys; returns 1 (go on) */

char sv_SetSkyLightColors(D3OBJECT *o, void *ctx)
{
    SVSKYKEY *t;
    int i;

    if (sv_WinterWeather == 0)
        t = (o->objectFlags & 0x10) ? sv_SkyKeysAlt : sv_SkyKeys;
    else
        t = (o->objectFlags & 0x10) ? sv_SkyKeysWinterAlt : sv_SkyKeysWinter;
    for (i = 0; i < 6; i++) {
        D3_L(o)->light->keys[i].rgb[0] = t[i].rgb[0];
        D3_L(o)->light->keys[i].rgb[1] = t[i].rgb[1];
        D3_L(o)->light->keys[i].rgb[2] = t[i].rgb[2];
        D3_L(o)->light->keys[i].val = t[i].val;
    }
    return 1;
}
#define sv_SkyHeavyT (*(SVSKYREC (*)[3])&sv_SkyHeavy)  /* 0x623ac0, sv_SkyHeavy (cm.c: char[]) as its sky records */
/* 0x525510, sets up the day's weather and sky for the clock t: the season's cloud chance and climate from the own player's record; with
   reroll the hours are rolled anew (winter weather in season 3: snow with rain at half the cloud chance, else snow; the clouds of each hour
   by the chance, their thunder above 999 out of winter, the wind rose as sin / cos of a random walk of the angle by +-0.175); the wind
   state, rain or snow by the winter weather and the strongest hour, the sky with its three layers (two fair skies and the dark one), the
   light colours of every scene with a map and the sky phase for t */
void sv_SetupWeather(struct GCLOCK *t, int reroll)
{
    int season, i, mx;
    float ang, step;
    int scene;
    D3TEXFLAGS flags;
    char date[12];

    gm_ClockToDate(t, date);
    season = sv_ClockSeason(t);
    sv_CloudChance = ai_SeasonCloudChance[ai_OwnPlayer * 0xbd + season];
    sv_SeasonClimate = ai_SeasonClimate[ai_OwnPlayer * 0xbd + season];
    if (reroll != 0) {
        if (season == 3) {
            sv_WinterWeather = (util_RandomU(100) < sv_CloudChance / 2) + 1;
        } else
            sv_WinterWeather = 0;
        for (i = 0; i < 24; i++)
            sv_Weather.clouds[i] = 0;
        for (i = 0; i < 24; i++) {
            if (util_RandomU(100) < sv_CloudChance)
                sv_Weather.clouds[i] = (unsigned short)util_Random(sv_CloudChance * 10);
            if ((int)sv_Weather.clouds[i] > 700)
                sv_Weather.clouds[i] = (sv_Weather.clouds[i] & 1) + 999;
            else if ((int)sv_Weather.clouds[i] > 500)
                sv_Weather.clouds[i] = (sv_Weather.clouds[i] & 1) + 499;
            else if ((int)sv_Weather.clouds[i] > 250)
                sv_Weather.clouds[i] = (sv_Weather.clouds[i] & 1) + 0x95;
            else
                sv_Weather.clouds[i] = 0;
            if (sv_WinterWeather == 0 && (int)sv_Weather.clouds[i] >= 999)
                sv_Weather.storm[i] = (unsigned short)util_Random(3);
            else
                sv_Weather.storm[i] = 0;
        }
        ang = (float)(util_RandomF() * 6.2831853);
        step = rand() > 0x3fff ? -0.175f : 0.175f;
        for (i = 0; i < 24; i++) {
            sv_Weather.windX[i] = (float)sin(ang);
            sv_Weather.windY[i] = (float)cos(ang);
            ang = (float)(util_RandomF() * step + ang);
        }
    }
    ai_InitWind(season, ai_TimeT.year, sv_Weather.windX, sv_Weather.windY);
    sv_Weather.thunderStart = 0;
    sv_Weather.thunderLen = 0;
    sv_WeatherHour = -1;
    mx = 0;
    for (i = 0; i < 24; i++)
        if (mx <= (int)sv_Weather.clouds[i])
            mx = sv_Weather.clouds[i];
    if (sv_WinterWeather != 0) {
        if (sv_WinterWeather == 1) {
            if (d3t_Rain != 0) {
                d3t_DeleteRain(d3t_Rain);
                d3t_Rain = 0;
            }
        } else if (d3t_Rain == 0)
            d3t_Rain = d3t_CreateRain(mx);
        if (d3t_Snow == 0)
            d3t_Snow = d3t_CreateSnow(mx);
        d3t_SetSnow(d3t_Snow, 2, 0, 0, 3, 0xff);
        d3t_SetSnow(d3t_Snow, 1, 0, 0);
    } else {
        if (d3t_Snow != 0) {
            d3t_FreeSnow(d3t_Snow);
            d3t_Snow = 0;
        }
        if (d3t_Rain == 0)
            d3t_Rain = d3t_CreateRain(mx);
        d3t_SetRain(d3t_Rain, 1, 0, 0);
    }
    if (d3_Sky == 0)
        d3_Sky = d3sky_Create(-0.95f, -0.2f, 600.0f);
    for (i = 0; i < 6; i++) {
        if (d3_SkyLayers[i] != 0) {
            d3sky_DeleteLayer(d3_Sky, (struct D3SKYLAYER *)d3_SkyLayers[i]);
            d3_SkyLayers[i] = 0;
        }
    }
    flags.all = 0xff;
    flags.b[2] = 0;
    d3_SkyLayers[0] = (int)d3sky_CreateLayer(d3_Sky, 0, sv_SkyFair[0].name, flags, sv_SkyFair[0].scale, 0, sv_SkyFair[0].vscale, sv_SkyFair[0].voffset);
    d3sky_SetLayerSpeed(d3_Sky, (struct D3SKYLAYER *)d3_SkyLayers[0], 0);
    flags.all = 0;
    flags.b[2] = 0;
    d3_SkyLayers[1] = (int)d3sky_CreateLayer(d3_Sky, 0, sv_SkyFair[1].name, flags, sv_SkyFair[1].scale, 0, sv_SkyFair[1].vscale, sv_SkyFair[1].voffset);
    d3sky_SetLayerSpeed(d3_Sky, (struct D3SKYLAYER *)d3_SkyLayers[1], 0);
    flags.all = 0;
    flags.b[2] = 0;
    d3_SkyLayers[2] = (int)d3sky_CreateLayer(d3_Sky, 0, sv_SkyHeavyT[2].name, flags, sv_SkyHeavyT[2].scale, 0, sv_SkyHeavyT[2].vscale, sv_SkyHeavyT[2].voffset);
    d3sky_SetLayerSpeed(d3_Sky, (struct D3SKYLAYER *)d3_SkyLayers[2], 0);
    scene = d3_CurrentScene;
    for (i = 0; i < 64; i++) {
        if (d3_Scenes[i].map != 0) {
            d3if_SelectScene(i, 1);
            sv_UpdateSkyColors();
        }
    }
    d3if_SelectScene(scene, 1);
    sv_SkyPhase = -100;
    sv_LampSwitches = 0;
    sv_UpdateSkyPhase((AITIME *)t);
}
typedef struct SVLEVELCOLOR {           /* 0x10 bytes, the ambient colour of a light level in the sky tables */
    D3VECTOR amb;
    float pad;                          /* 0 in every record */
} SVLEVELCOLOR;
#define sv_LevelAmbient       (*(SVLEVELCOLOR (*)[7])&ai_Unk623cc0)  /* 0x623cc0, the ambient colours of the 7 light levels */
#define sv_LevelAmbientWinter (*(SVLEVELCOLOR (*)[7])&ai_Unk623e10)  /* 0x623e10, the same in winter weather */
/* 0x525940, colours the default scene's sky for the weather: its lights' keys (sv_SetSkyLightColors), then the ambient colour of
   each of the 7 light levels from the level table (the winter one in winter weather) into the scene's levels, the current ambient
   colour and the engine's levels; only when the scene has a map */

void sv_UpdateSkyColors(void)
{
    SVLEVELCOLOR *p;
    int i;

    if (d3_DefaultScene->map == 0)
        return;
    d3_ForEachObject(d3_DefaultScene, 0, 8, sv_SetSkyLightColors, 0);
    p = sv_WinterWeather == 0 ? sv_LevelAmbient : sv_LevelAmbientWinter;
    for (i = 0; i < 7; i++) {
        ((D3IFSCENE *)d3_DefaultScene)->levels[i].amb.x = p[i].amb.x;
        ((D3IFSCENE *)d3_DefaultScene)->levels[i].amb.y = p[i].amb.y;
        ((D3IFSCENE *)d3_DefaultScene)->levels[i].amb.z = p[i].amb.z;
        d3_AmbientColor.x = p[i].amb.x;
        d3_AmbientColor.y = p[i].amb.y;
        d3_AmbientColor.z = p[i].amb.z;
        d3if_Levels[i].amb.x = p[i].amb.x;
        d3if_Levels[i].amb.y = p[i].amb.y;
        d3if_Levels[i].amb.z = p[i].amb.z;
    }
}
/* 0x525a00, fills the day phases sv_DayPhases (hour, minute, second of the day) from the start minutes of the season of the
   clock t (sv_PhaseMinutes)
   @deep Read against the original 0x525a00..0x525a6f: the statement set matches (gm_ClockSeason, the
   6-phase index loop over sv_DayPhases with hour = minutes / 60, min = (char)minutes - hour * 60, sec =
   (min + hour * 60) * 60; the jl index loop and the stride-10 pointer at the min field are the
   original's). Only allocation is left: the original saves ebx and keeps the walk pointer in ecx, the
   season row pointer in esi, the minute value in edi and the 0x3c multiplier / minute byte in bl; ours
   keeps the walk pointer in esi, the row in edi, the value in ecx and the byte ops in cl, so it saves no
   ebx (asmdiff --regs esi=ecx,edi=esi,ecx=edi,cl=bl leaves only push ebx / pop ebx and the value register
   edi vs ebx: '+ push ebx', '+ mov edi, dword ptr [esi]', '+ imul edi'). Tried: a walking const int *q,
   swapped declaration orders of h / m and season / i, h * 60 + m: all the same 40 vs 42 instructions. */

void sv_FillDayPhases(struct GCLOCK *t)
{
    int season, i;
    char h, m;

    season = gm_ClockSeason(t);
    for (i = 0; i < 6; i++) {
        h = (char)(sv_PhaseMinutes[season * 6 + i] / 60);
        sv_DayPhases[i].hour = h;
        m = (char)sv_PhaseMinutes[season * 6 + i] - h * 60;
        sv_DayPhases[i].min = m;
        sv_DayPhases[i].sec = (m + h * 60) * 60;
    }
}
/* 0x525a80, a callback (the object o, a flag f): sets the float +0x1a4 of the record +0x1e8 of o to a random 0.3 .. 0.9 (f = 0) or -0.3 + 0.6 * ... (f != 0); returns 1 */

char sv_SetLampRamp(char *o, int f)
{
    double v;

    if (f == 0)
        v = util_RandomF() * 0.6 + 0.3;
    else
        v = -0.3 - util_RandomF() * 0.6;
    if (*(int *)(o + 0x1e8) != 0)
        *(float *)(*(int *)(o + 0x1e8) + 0x1a4) = (float)v;
    return 1;
}
/* 0x525ad0, applies the sky phase of the clock t: the second of the day is mapped through the day phases to the sky phase k
   (0 .. 600: 100 per phase, 200 for the midday phase); the dark sky layer's alpha is the fog strength; while a thunder flash
   lasts (in a scene with a map) the thunder fog set 2 is on; when the phase, the scale and the scene hardly changed only the fog
   is renewed; otherwise the light set k / 100 mod 7 and its fraction go to d3_SetLightSet (changed when the phase moved by 100 or
   the scene changed), the lamps get their ramps at the first two light sets of the day */
void sv_UpdateSkyPhase(AITIME *t)
{
    int s, k, day, d, sc;
    float a, frac;
    int changed;

    a = 0.0f;
    s = (t->min + t->hour * 60) * 60;
    if (s < sv_DayPhases[0].sec)
        k = 0;
    else if (s < sv_DayPhases[1].sec)
        k = ((s - sv_DayPhases[0].sec) * 100) / (sv_DayPhases[1].sec - sv_DayPhases[0].sec);
    else if (s < sv_DayPhases[2].sec)
        k = ((s - sv_DayPhases[1].sec) * 100) / (sv_DayPhases[2].sec - sv_DayPhases[1].sec) + 100;
    else if (s < sv_DayPhases[3].sec)
        k = ((s - sv_DayPhases[2].sec) * 200) / (sv_DayPhases[3].sec - sv_DayPhases[2].sec) + 200;
    else if (s < sv_DayPhases[4].sec)
        k = ((s - sv_DayPhases[3].sec) * 100) / (sv_DayPhases[4].sec - sv_DayPhases[3].sec) + 400;
    else if (s < sv_DayPhases[5].sec)
        k = ((s - sv_DayPhases[4].sec) * 100) / (sv_DayPhases[5].sec - sv_DayPhases[4].sec) + 500;
    else
        k = 600;
    if (d3_SkyLayers[2] != 0)
        a = ((D3SKYLAYER *)d3_SkyLayers[2])->alpha * 0.003921569f;
    if (d3_Scenes[d3_CurrentScene].map != 0 && (unsigned)d3_Time < (unsigned)(sv_Weather.thunderStart + sv_Weather.thunderLen)) {
        d3_SetFogSet(0, 2, 1.0f);
        sv_ThunderFog = 1;
        return;
    }
    if (sv_ThunderFog != 0) {
        d3_SetFogSet(0, 1, a);
        sv_ThunderFog = 0;
    }
    sc = d3_CurrentScene;
    if (((fabs(sv_SkyScale - 1.0f) < 0.1 && abs(sv_SkyPhase - k) < 10) && sv_SkyScene == d3_CurrentScene) || d3t_CamMove != 0) {
        d3_SetFogSet(0, 1, a);
        return;
    }
    if (fabs(sv_SkyScale - 1.0f) > 0.2 || abs(sv_SkyPhase - k) >= 100)
        changed = 1;
    else
        changed = sv_SkyScene != d3_CurrentScene;
    sv_SkyScene = d3_CurrentScene;
    sv_SkyPhase = k;
    sv_SkyScale = 1.0f;
    frac = k * 0.01;
    day = (int)frac;
    d = day % 7;
    if (sv_LampPhase != d && d3_Scenes[sc].map != 0) {
        if (d >= 0 && sv_LampSwitches == 0) {
            d3_ForEachObject(d3_DefaultScene, 0, 6, sv_SetLampRamp, 0);
            sv_LampSwitches++;
        } else if (d >= 3 && sv_LampSwitches == 1) {
            d3_ForEachObject(d3_DefaultScene, 0, 6, sv_SetLampRamp, 1);
            sv_LampSwitches++;
        }
        sv_LampPhase = d;
    }
    sv_DayPhaseFrac = frac = frac - (float)day;
    sv_DayPhase = d;
    d3_SetLightSet(d, frac, 1.0f, changed);
    d3_SetFogSet(0, 1, a);
}

extern char * sv_CameraObj;
extern float sv_CameraHeightFactor, sv_CameraAngleFactor, ai_Unk6f2374, sv_ScrollVelX, sv_ScrollVelY, sv_ScrollVelXPrev, sv_ScrollVelYPrev;  /* 0x6f22d4, 0x6f22d8, 0x6f2374, 0x6f2388, 0x6f2390, 0x6f238c, 0x6f1c44 */
extern float sv_CameraHeightNear, sv_CameraAngleNear, sv_CameraHeightFar, sv_CameraAngleFar, sv_ScrollSpeed;  /* 0x623800, 0x623804, 0x623808, 0x62380c, 0x623818 */
extern int ai_Unk6f2378;  /* 0x6f2378 */
extern int ai_Unk6f22cc;  /* 0x6f22cc */
extern int ai_Unk6f22d0;  /* 0x6f22d0 */
extern int d3t_CamMoving;  /* 0x6310e4 */
extern struct D3ANIM *d3t_CamMove;  /* 0x6310e8 */
extern int sv_ScrollTime;  /* 0x6f2394 */
extern float sv_ScrollLastMove[3];
float d3t_GetMeanHeight();                   /* 0x412550 */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
void d3_MatTranspose();                     /* 0x454040 */
float d3_VecAngle2();                   /* 0x453b40 */

/* 0x525dd0, in scene 0 sets the camera height factors 0x6f22d4 / 0x6f22d8 to f and moves the camera (0x43b790: the x / z of its position and the height 0x623800 .. 0x623808 blended by f over the terrain height
   0x412550) and the camera angle (0x43b860: +0x88 / +0x8c and 0x623804 .. 0x62380c blended); stores the camera position and direction (0x75fb80 / 0x75fb90) */
void sv_PlaceCamera(float f)
{
    float pos[3], ang[3];
    double h;

    if (d3_CurrentScene != 0)
        return;
    sv_CameraHeightFactor = f;
    sv_CameraAngleFactor = f;
    h = d3t_GetMeanHeight(sv_CameraObj + 0x4c);
    pos[0] = *(float *)(sv_CameraObj + 0x4c);
    pos[2] = *(float *)(sv_CameraObj + 0x54);
    pos[1] = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + h);
    d3_SetObjectPos(sv_CameraObj, pos);
    ang[1] = *(float *)(sv_CameraObj + 0x88);
    ang[2] = *(float *)(sv_CameraObj + 0x8c);
    ang[0] = (sv_CameraAngleFar - sv_CameraAngleNear) * sv_CameraAngleFactor + sv_CameraAngleNear;
    d3_SetObjectRot(sv_CameraObj, ang);
    sv_ViewRotX = *(int *)(sv_CameraObj + 0x84);
    sv_ViewRotY = *(int *)(sv_CameraObj + 0x88);
    sv_ViewRotZ = *(int *)(sv_CameraObj + 0x8c);
    sv_ViewPosX = *(int *)(sv_CameraObj + 0x4c);
    sv_ViewPosY = *(int *)(sv_CameraObj + 0x50);
    sv_ViewPosZ = *(int *)(sv_CameraObj + 0x54);
}
/* 0x525ee0, eases the camera height (+0x50) toward the wanted one (the terrain height + 0x623800 .. 0x623808 blend by 0x6f22d4, minus the camera offset +0x7c): a difference above 5.0 is closed by 1/15 per call, at most 10.0;
   the step is kept in 0x6f2374 (not applied while the cutscene flag 0x6310e8 is set) */

void sv_Unk525ee0(void)
{
    double d;
    float step, pos[3];

    if (sv_CameraObj == 0)
        return;
    d = d3t_GetMeanHeight(sv_CameraObj + 0x4c);
    d = ((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + d) - (*(float *)(sv_CameraObj + 0x7c) + *(float *)(sv_CameraObj + 0x50));
    if (5.0 < fabs(d)) {
        d = d * 0.06666667;
        step = (float)d;
        if (d <= 0.0) {
            if (step < 0.0f && step <= -10.0f)
                step = -10.0f;
        } else if (10.0f <= step)
            step = 10.0f;
        pos[0] = *(float *)(sv_CameraObj + 0x4c);
        pos[2] = *(float *)(sv_CameraObj + 0x54);
        pos[1] = *(float *)(sv_CameraObj + 0x50) + step;
        if (d3t_CamMove == 0)
            d3_SetObjectPos(sv_CameraObj, pos);
        ai_Unk6f2374 = step;
        return;
    }
    ai_Unk6f2374 = 0.0f;
}
/* 0x525ff0, the mouse at the screen border starts a camera move to one of the 5 preset views (+0x5c / 0x90 the default, +0xb4 / +0xcc left / right of the window, +0xe4 / +0xfc above / below, each position + direction
   triple): the border state 0x6f22cc / 0x6f22d0 (-1, 0, 1) steps by the mouse position against the window 0x6310b0 / 0x6310b4 .. 0x5f3238 / 0x5f323c; the view is applied (0x75fb80 / 0x75fb90, 0x433b10, 0x411860 (cam,
   pos, dir, 0x623814, 8)) unless the camera is already within 0.2 of it (0x453e80) */

void sv_Unk525ff0(void)
{
    int top, bot;
    int pos[3], dir[3];
    int zero[4];
    int my, off;

    bot = di_MaxY - 1;
    top = di_MinY + 1;
    zero[0] = zero[1] = zero[2] = zero[3] = 0;
    if (ai_Unk6f2378 != 0) {
        if (dm_L.x < di_MinX + 1)
            return;
        if (di_MaxX - 1 < dm_L.x)
            return;
        if (dm_L.y < top)
            return;
        if (bot < dm_L.y)
            return;
        ai_Unk6f2378 = 0;
        return;
    }
    my = dm_L.y;
    if (top < my && my < bot) {
        if (dm_L.x < di_MinX + 1) {
            if (ai_Unk6f22cc < 0)
                return;
            ai_Unk6f22cc--;
        } else {
            if (dm_L.x <= di_MaxX - 1)
                return;
            if (0 < ai_Unk6f22cc)
                return;
            ai_Unk6f22cc++;
        }
        ai_Unk6f22d0 = 0;
        if (ai_Unk6f22cc == -1) {
            off = 0xe4;
            if (d3_VecEqual(sv_CameraObj + off, zero, 0.2f) != 0)
                return;
            goto load;
        }
        if (ai_Unk6f22cc != 0) {
            if (ai_Unk6f22cc == 1) {
                off = 0xfc;
                if (d3_VecEqual(sv_CameraObj + off, zero, 0.2f) != 0)
                    return;
                goto load;
            }
            goto apply;
        }
    } else {
        if (my < top) {
            if (ai_Unk6f22d0 < 0)
                return;
            ai_Unk6f22d0--;
        } else if (bot < my) {
            if (0 < ai_Unk6f22d0)
                return;
            ai_Unk6f22d0++;
        }
        ai_Unk6f22cc = 0;
        if (ai_Unk6f22d0 != 0) {
            if (ai_Unk6f22d0 == -1) {
                off = 0xb4;
                if (d3_VecEqual(sv_CameraObj + off, zero, 0.2f) != 0)
                    return;
                goto load;
            } else if (ai_Unk6f22d0 == 1) {
                off = 0xcc;
                if (d3_VecEqual(sv_CameraObj + off, zero, 0.2f) != 0)
                    return;
                goto load;
            }
            goto apply;
        }
    }
    off = 0x5c;
    memcpy(pos, sv_CameraObj + 0x5c, 12);
    memcpy(dir, sv_CameraObj + 0x90, 12);
    goto apply;
load:
    memcpy(pos, sv_CameraObj + off, 12);
    memcpy(dir, sv_CameraObj + off + 0xc, 12);
apply:
    sv_ViewPosX = pos[0];
    sv_ViewPosY = pos[1];
    sv_ViewPosZ = pos[2];
    sv_ViewRotX = dir[0];
    sv_ViewRotY = dir[1];
    sv_ViewRotZ = dir[2];
    d3anim_FreePath(sv_CameraObj);
    d3t_CamMoving = 0;
    d3t_CamMove = 0;
    d3t_PlaceObjectV(sv_CameraObj, pos, dir, ai_CameraStep, 8);
    ai_Unk6f2378 = 1;
}
/* 0x5263a0, one frame of the camera scroll by the screen border (a 0x40-pixel zone of the mouse window) or the
   arrow keys: the x / y scroll velocities sv_ScrollVelX / Y start at +-10 and grow by 5 * dt (dt = 0.4 * the
   elapsed ms, at most 4) up to +-50, decay to 0 otherwise; d2_PointerLook gets the direction (1 .. 8, 5 .. 8
   the corners). The camera moves by the velocities rotated into the view (by the view's heading, or by the
   matrix rows when it looks straight down), slower inside a building, clamped to the height map 20 cells in,
   unless the move equals the last one (sv_ScrollLastMove). Returns 1 when the camera moved @deep read whole against 0x5263a0..0x526ce2 (571 vs 579 instructions): the look 1..8 test, dt from the clock (1.0f first frame, min(dt, 4.0)), the border/key/corner speed ramps with their double 50/-50 bounds, both decays (x > 0 .. else x < 0), the building slow-down, k = speed * dt * (factor*1.5 + 0.6), the AxisPair rows, both branches with their dead dst stores, the clamp (x: < lo / else > hi; z: max(hi) / min(lo)) all match. Left, allocation and layout: the angle a kept in a register where the original stores and reloads it (0x526b36 fstp/fld, fld [esp] before fcos), fwd[2] kept with fst where the original stores and reloads (0x526aaa), the 0.1 bound compared as a double literal vs the original's float at 0x5eb91c, the x-decay block placed before the left/right bodies (ours 0x526820) where the original puts it after them (0x5268c4), one register (ecx vs edx at 0x526ad6), frame 0x74 vs 0x80 */

int sv_ScrollCamera(void)
{
    int moved, my;
    float dt;                       /* the frame time factor, at most 4 */
    float k, f;
    float pos[3], dst[3], v[3], right[16], fwd[3];   /* right is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    float a, sn, cs;
    D3SM *hm;                       /* the scene's height map */
    float lo, hi;
    int dirs;
    const float *ax = (const float *)ai_AxisPair;

    moved = 0;
    dirs = 0;
    if ((unsigned short)d2_PointerLook >= 1 && (unsigned short)d2_PointerLook <= 8)
        d2_PointerLook = 0;
    if (d3t_CamMove != 0 || d3t_CamMoving != 0)
        return 0;
    if (sv_ScrollTime != 0)
        dt = (unsigned)(d3_Time - sv_ScrollTime) * 0.4;
    else
        dt = 1.0f;
    dt = min(dt, 4.0);
    sv_ScrollTime = d3_Time;
    my = dm_L.y;
    f = (float)(5.0 * dt);
    if ((my == di_MinY && dm_L.x < di_MinX + 0x40) || (my < di_MinY + 0x40 && dm_L.x == di_MinX)) {
        if (sv_ScrollVelY == 0.0f)
            sv_ScrollVelY = 10.0f;
        if (sv_ScrollVelY < 50.0)
            sv_ScrollVelY += f;
        if (sv_ScrollVelX == 0.0f)
            sv_ScrollVelX = -10.0f;
        if (-50.0 < sv_ScrollVelX)
            sv_ScrollVelX -= f;
        d2_PointerLook = 5;
    } else if ((my == di_MinY && dm_L.x > di_MaxX - 0x40) || (my < di_MinY + 0x40 && dm_L.x == di_MaxX)) {
        if (sv_ScrollVelY == 0.0f)
            sv_ScrollVelY = 10.0f;
        if (sv_ScrollVelY < 50.0)
            sv_ScrollVelY += f;
        if (sv_ScrollVelX == 0.0f)
            sv_ScrollVelX = 10.0f;
        if (sv_ScrollVelX < 50.0)
            sv_ScrollVelX += f;
        d2_PointerLook = 6;
    } else if ((my == di_MaxY && dm_L.x > di_MaxX - 0x40) || (my > di_MaxY - 0x40 && dm_L.x == di_MaxX)) {
        if (sv_ScrollVelY == 0.0f)
            sv_ScrollVelY = -10.0f;
        if (-50.0 < sv_ScrollVelY)
            sv_ScrollVelY -= f;
        if (sv_ScrollVelX == 0.0f)
            sv_ScrollVelX = 10.0f;
        if (sv_ScrollVelX < 50.0)
            sv_ScrollVelX += f;
        d2_PointerLook = 8;
    } else if ((my == di_MaxY && dm_L.x < di_MinX + 0x40) || (my > di_MaxY - 0x40 && dm_L.x == di_MinX)) {
        if (sv_ScrollVelY == 0.0f)
            sv_ScrollVelY = -10.0f;
        if (-50.0 < sv_ScrollVelY)
            sv_ScrollVelY -= f;
        if (sv_ScrollVelX == 0.0f)
            sv_ScrollVelX = -10.0f;
        if (-50.0 < sv_ScrollVelX)
            sv_ScrollVelX -= f;
        d2_PointerLook = 7;
    } else {
        if (dm_L.x == di_MinX || kb_Down[0xcb] != 0) {
            if (sv_ScrollVelX == 0.0f)
                sv_ScrollVelX = -10.0f;
            if (-50.0 < sv_ScrollVelX)
                sv_ScrollVelX -= f;
            d2_PointerLook = 4;
        } else if (dm_L.x == di_MaxX || kb_Down[0xcd] != 0) {
            if (sv_ScrollVelX == 0.0f)
                sv_ScrollVelX = 10.0f;
            if (sv_ScrollVelX < 50.0)
                sv_ScrollVelX += f;
            d2_PointerLook = 3;
        } else {
            if (my == di_MaxY || kb_Down[0xd0] != 0) {
                if (sv_ScrollVelY == 0.0f)
                    sv_ScrollVelY = -10.0f;
                if (-50.0 < sv_ScrollVelY)
                    sv_ScrollVelY -= f;
                dirs = 1;
                d2_PointerLook = 1;
            } else if (my == di_MinY || kb_Down[0xc8] != 0) {
                if (sv_ScrollVelY == 0.0f)
                    sv_ScrollVelY = 10.0f;
                if (sv_ScrollVelY < 50.0)
                    sv_ScrollVelY += f;
                dirs = 1;
                d2_PointerLook = 2;
            }
            if (sv_ScrollVelX > 0.0f) {          /* the x speed decays to 0 */
                sv_ScrollVelX -= f;
                if (sv_ScrollVelX < 0.0f)
                    sv_ScrollVelX = 0.0f;
            } else if (sv_ScrollVelX < 0.0f) {
                sv_ScrollVelX += f;
                if (sv_ScrollVelX > 0.0f)
                    sv_ScrollVelX = 0.0f;
            }
            if (dirs)
                goto scroll;
        }
        if (sv_ScrollVelY > 0.0f) {              /* the y speed decays to 0 */
            sv_ScrollVelY -= f;
            if (sv_ScrollVelY < 0.0f)
                sv_ScrollVelY = 0.0f;
        } else if (sv_ScrollVelY < 0.0f) {
            sv_ScrollVelY += f;
            if (sv_ScrollVelY > 0.0f)
                sv_ScrollVelY = 0.0f;
        }
    }
scroll:
    if (d3_Camera != 0 && (sv_ScrollVelX != 0.0f || sv_ScrollVelY != 0.0f)) {
        if (ai_FallbackBuilding != 0)
            dt *= 0.5;
        else if (ai_PlayerBuilding != 0)
            dt *= 0.25;
        k = (float)(sv_ScrollSpeed * dt * (sv_CameraHeightFactor * 1.5 + 0.6000000238418579));
        d3_MatTranspose(&d3_Camera->matrix, right);
        v[0] = sv_ScrollVelX * k * 1.1;
        fwd[1] = 0.0f;
        v[1] = sv_ScrollVelY * k * 1.1;
        fwd[0] = right[0] * ax[0] + right[4] * ax[1] + right[8] * ax[2];
        fwd[2] = right[2] * ax[0] + right[6] * ax[1] + right[10] * ax[2];
        if (sqrt(fwd[0] * fwd[0] + fwd[2] * fwd[2]) < 0.1f) {   /* the view looks straight down: the matrix rows (0x526ac2) */
            dst[1] = v[1];
            dst[2] = 0.0f;
            fwd[0] = 0.0f * right[8] + v[0] * right[0] + v[1] * right[4];
            fwd[2] = 0.0f * right[10] + v[0] * right[2] + v[1] * right[6];
        } else {                                                /* else rotated by the view's heading */
            a = -d3_VecAngle2((char *)(unsigned)&ai_AxisPair, fwd);
            sn = (float)sin(a);
            dst[1] = 0.0f;
            dst[2] = v[1];
            cs = (float)cos(a);
            fwd[0] = sn * v[1] + cs * v[0];
            fwd[2] = cs * v[1] - sn * v[0];
        }
        fwd[1] = 0.0f;
        dst[0] = fwd[0] + d3_Camera->pos.x;
        dst[1] = d3_Camera->pos.y;
        dst[2] = fwd[2] + d3_Camera->pos.z;
        if (d3_VecEqual(sv_ScrollLastMove, dst, 0.1f) == 0) {
            hm = (D3SM *)d3_DefaultScene->hmap;
            if (hm != 0) {                                       /* clamped to the height map, 20 cells in */
                lo = hm->cell.x * 20.0f + hm->origin.x;
                if (dst[0] < lo)
                    dst[0] = lo;
                else {
                    hi = (float)(hm->size - 20) * hm->cell.x + hm->origin.x;
                    if (dst[0] > hi)
                        dst[0] = hi;
                }
                lo = hm->cell.z * 20.0f + hm->origin.z;
                hi = (float)(hm->size - 20) * hm->cell.z + hm->origin.z;
                dst[2] = max(dst[2], hi);           /* z runs from the far edge (a negative cell): hi is the lower bound */
                dst[2] = min(dst[2], lo);
            }
            d3_SetObjectPos(d3_Camera, (struct D3VECTOR *)dst);
            moved = 1;
        }
        sv_ScrollLastMove[0] = fwd[0];
        sv_ScrollLastMove[1] = fwd[1];
        sv_ScrollLastMove[2] = fwd[2];
    }
    sv_ScrollVelXPrev = sv_ScrollVelX;
    sv_ScrollVelYPrev = sv_ScrollVelY;
    return moved;
}

extern int sv_MouseDragOn, sv_ZoomTurnOn, sv_MouseCamActive, sv_DragStartX, sv_DragStartY, sv_DragLastX, sv_DragLastY, sv_SavedMaxX, sv_SavedMaxY, sv_SavedMinX, sv_SavedMinY, sv_ZoomStartX, sv_ZoomStartY;  /* 0x6f23b8, 0x6f23a0, 0x6f23a4, 0x6f2398, 0x6f239c, 0x6f1c88, 0x6f1c48, 0x6f1c50, 0x6f1c40, 0x6f1c84, 0x6f2298, 0x6f2290, 0x6f1c8c */
extern int ai_OptionScrollSpeed;
extern float sv_ZoomStartHeight, sv_ZoomStartAngle, sv_ZoomStartTurn, sv_ZoomBaseY, sv_DragLastPos[3];  /* 0x6f23a8, 0x6f23ac, 0x6f23b0, 0x6f23b4, 0x6f1c64 */
extern int tb_Paused;  /* 0x6333e0 */
extern int ai_Unk760440;  /* 0x760440 */
extern int ai_Unk62a8cc;  /* 0x62a8cc */
extern int ai_Unk6f22b4;  /* 0x6f22b4 */
extern int sv_FrameDepth;  /* 0x6f229c */
extern int sv_FrameBase;  /* 0x6f22a4 */
extern char * sv_SelectedObject;  /* 0x75ec60 */
extern int sv_SelectedModel;  /* 0x6f2304 */
extern char ai_Unk62381c;
int d3t_GetHeightRange();                     /* 0x411c70 */
short * gm_PlayerRecord(char *ch);                   /* 0x506430 */
void sv_PlaceCamera(float f);
void sv_Unk525ee0(void);
int sv_ScrollCamera(void);
int sv_MouseCamera(void);
#define CAMLOG(r) do { \
        *(int *)((r) + 0x8c) = *(int *)(sv_CameraObj + 0x4c); \
        *(int *)((r) + 0x90) = *(int *)(sv_CameraObj + 0x50); \
        *(int *)((r) + 0x94) = *(int *)(sv_CameraObj + 0x54); \
        *(int *)((r) + 0x9c) = *(int *)(sv_CameraObj + 0x84); \
        *(int *)((r) + 0xa0) = *(int *)(sv_CameraObj + 0x88); \
        *(int *)((r) + 0xa4) = *(int *)(sv_CameraObj + 0x8c); \
    } while (0)

/* the player's camera record gets the camera's position and rotation */
#define SV_CAMREC(r) do { \
        (r)->camPos[0] = d3_Camera->pos.x; \
        (r)->camPos[1] = d3_Camera->pos.y; \
        (r)->camPos[2] = d3_Camera->pos.z; \
        (r)->camRot[0] = d3_Camera->rotation.x; \
        (r)->camRot[1] = d3_Camera->rotation.y; \
        (r)->camRot[2] = d3_Camera->rotation.z; \
    } while (0)

/* 0x526cf0, the camera by the mouse. The right button frees the mouse (the clip window opened to +-32000, the
   cursor put back on release). With both buttons the drag zooms (the height factor, 0.0015 per pixel of y and 5
   per wheel notch) and turns (the angle factor with it, rotation.y by 0.0035 per pixel of x); the right button
   alone drags the map by the mouse delta scaled by the scroll speed option, rotated into the view and clamped to
   the height map 20 cells in. The wheel alone zooms by 0.1 a notch. Each change goes into the player's camera
   record. Returns the activity: sv_MouseCamActive - wheel down + wheel up + sv_ZoomTurnOn @deep read whole against 0x526cf0..0x527640 (556 vs 565 instructions): the free-mouse start (saved clip window, cursor), the zoom/turn start values and their y range (1 - angle) / -0.0015 and -angle / -0.0015 (was a 0 * y0 leftover), the release (0.004 per pixel), the both-button zoom/turn with the copied position / rotation, the wheel (0.1f a notch, max(min(z, 1), 0)), the drag (scroll speed * 0.0606, AxisPair rows, both branches with their dead dst stores, the clamp x < lo / > hi, z max / min) and the return all match. Left: allocation (register choice of the camera / my / constants 32000 / -32000 shifts with statement order: ours ebx/esi vs the original's esi/ebx/edi, frame 0x7c vs 0x90) and the -666.6667 drag-range constant, a float object in the original (0x5eeb78, fmul dword after fchs) that a literal folds into a double / positive constant */
int sv_MouseCamera(void)
{
    short my;
    char *cam;
    AIFIN *rec;
    float z, f2, ang, sn, cs, lo, hi;
    double k;                       /* the drag scale: the scroll speed option / 16.5 */
    D3VECTOR zpos, zrot;            /* the camera position / rotation of the zoom and turn */
    float dst[3], right[16], fwd[3], v[2];   /* right is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    D3SM *hm;                       /* the scene's height map */
    const float *ax = (const float *)ai_AxisPair;

    if (d3t_CamMoving != 0)
        return 0;
    my = dm_P.y;
    cam = sv_CameraObj;
    if (dm_L.rhold == 0) {
    chk:
        if (dm_L.ldown != 0 && dm_L.rhold != 0)
            goto go;
    } else {
        if (sv_MouseDragOn == 0) {                /* the first frame of the right button: free the mouse */
            sv_DragLastX = sv_DragStartX = dm_L.x;
            sv_DragLastY = sv_DragStartY = dm_L.y;
            sv_SavedMaxX = di_MaxX;
            sv_MouseDragOn = 1;
            di_Absolute = 0;
            sv_SavedMaxY = di_MaxY;
            sv_SavedMinX = di_MinX;
            sv_SavedMinY = di_MinY;
            di_MaxX = 32000;
            di_MaxY = 32000;
            di_MinX = -32000;
            di_MinY = -32000;
        }
        if (dm_L.ldown != 0) {
            if (sv_ZoomTurnOn == 0) {
                sv_ZoomTurnOn = 1;                   /* the zoom / turn starts: its start values */
                sv_ZoomStartX = dm_P.x;
                sv_ZoomStartHeight = sv_CameraHeightFactor;
                sv_ZoomStartY = my;
                sv_ZoomBaseY = ((D3OBJECT *)cam)->pos.y - ((sv_CameraHeightFar - sv_CameraHeightNear) * sv_ZoomStartHeight + sv_CameraAngleNear);
                sv_ZoomStartAngle = sv_CameraAngleFactor;
                di_MaxY = sv_ZoomStartY - (int)((1.0 - sv_ZoomStartAngle) * -666.6667f);   /* the drag range of the angle 0 .. 1 at 0.0015 per pixel */
                di_MinY = sv_ZoomStartY - (int)(-sv_ZoomStartAngle * -666.6667f);
                sv_ZoomStartTurn = ((D3OBJECT *)cam)->rotation.y;
            }
            goto chk;
        }
    }
    if (sv_ZoomTurnOn != 0) {
        sv_ZoomTurnOn = 0;
        sv_DragLastY = (int)my;
        di_MaxX = 32000;
        di_MaxY = 32000;
        sv_DragLastX = (int)dm_P.x;
        di_MinX = -32000;
        di_MinY = -32000;
        sv_ZoomStartHeight = (sv_DragLastY - sv_ZoomStartY) * 0.004 + sv_ZoomStartHeight;
    }
    if (dm_L.rhold == 0 && sv_MouseDragOn != 0) {
        di_MaxY = sv_SavedMaxY;
        di_MinX = sv_SavedMinX;
        di_MaxX = sv_SavedMaxX;
        di_MinY = sv_SavedMinY;
        di_Absolute = 1;
        sv_MouseDragOn = 0;
        di_SetMousePos(sv_DragStartX, sv_DragStartY);
        sv_DragStartX = 0;
        sv_DragStartY = 0;
        sv_ZoomTurnOn = 0;
        return 0;
    }
go:
    cam = (char *)d3_Camera;
    if (cam != 0) {
        if (dm_L.rhold != 0 && dm_L.ldown != 0 && sv_ZoomTurnOn != 0) {     /* both buttons: zoom and turn by the drag */
            f2 = ((dm_L.wup - dm_L.wdown) * 5.0f + my - sv_ZoomStartY) * 0.0015f;
            sv_CameraHeightFactor = sv_ZoomStartHeight + f2;
            zpos = ((D3OBJECT *)cam)->pos;
            zpos.y = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_ZoomBaseY;
            sv_CameraAngleFactor = f2 + sv_ZoomStartAngle;
            zrot = ((D3OBJECT *)cam)->rotation;
            zrot.x = (sv_CameraAngleFar - sv_CameraAngleNear) * sv_CameraAngleFactor + sv_CameraAngleNear;
            zrot.y = sv_ZoomStartTurn - (dm_P.x - sv_ZoomStartX) * 0.0035f;
            if (d3_VecEqual(&((D3OBJECT *)cam)->pos, &zpos, 0.1f) == 0 || d3_VecEqual(&d3_Camera->rotation, &zrot, 0.001f) == 0) {
                d3_SetObjectPos(d3_Camera, &zpos);
                d3_SetObjectRot(d3_Camera, &zrot);
                rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                if (rec != 0) {
                    rec->camAngle = sv_CameraAngleFactor;
                    SV_CAMREC(rec);
                }
            }
        }
        cam = (char *)d3_Camera;
        if (cam != 0) {
            if (sv_ZoomTurnOn == 0 && dm_L.wup - dm_L.wdown != 0) {   /* the wheel: the zoom by 0.1 a notch */
                z = sv_CameraHeightFactor = (dm_L.wup - dm_L.wdown) * 0.1f + sv_CameraHeightFactor;
                sv_PlaceCamera((float)max(min(z, 1.0), 0.0));
                rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                if (rec != 0) {
                    rec->camAngle = sv_CameraAngleFactor;
                    SV_CAMREC(rec);
                }
            }
            cam = (char *)d3_Camera;
            if (cam != 0 && dm_L.rhold != 0 && dm_L.ldown == 0) {    /* the right button alone: drag the map */
                d3_MatTranspose(&d3_Camera->matrix, right);
                k = ai_OptionScrollSpeed * 0.0606060606060606;
                v[0] = (dm_P.x - sv_DragLastX) * k;
                fwd[1] = 0.0f;
                v[1] = -(dm_P.y - sv_DragLastY) * k;
                fwd[0] = right[0] * ax[0] + right[4] * ax[1] + right[8] * ax[2];
                fwd[2] = right[2] * ax[0] + right[6] * ax[1] + right[10] * ax[2];
                if (sqrt(fwd[0] * fwd[0] + fwd[2] * fwd[2]) < 0.1f) {   /* the view looks straight down: the matrix rows (0x527386) */
                    dst[1] = v[1];
                    dst[2] = 0.0f;
                    fwd[0] = 0.0f * right[8] + v[0] * right[0] + v[1] * right[4];
                    fwd[2] = 0.0f * right[10] + v[0] * right[2] + v[1] * right[6];
                } else {                                                /* else rotated by the view's heading */
                    ang = -d3_VecAngle2((char *)(unsigned)&ai_AxisPair, fwd);
                    sn = (float)sin(ang);
                    dst[1] = 0.0f;
                    dst[2] = v[1];
                    cs = (float)cos(ang);
                    fwd[0] = sn * v[1] + cs * v[0];
                    fwd[2] = cs * v[1] - sn * v[0];
                }
                fwd[1] = 0.0f;
                dst[0] = fwd[0] + d3_Camera->pos.x;
                dst[1] = d3_Camera->pos.y;
                dst[2] = fwd[2] + d3_Camera->pos.z;
                if (d3_VecEqual(sv_DragLastPos, dst, 0.1f) == 0) {
                    hm = (D3SM *)d3_DefaultScene->hmap;
                    if (hm != 0) {                                      /* clamped to the height map, 20 cells in */
                        lo = hm->cell.x * 20.0f + hm->origin.x;
                        if (dst[0] < lo)
                            dst[0] = lo;
                        else {
                            hi = (float)(hm->size - 20) * hm->cell.x + hm->origin.x;
                            if (dst[0] > hi)
                                dst[0] = hi;
                        }
                        lo = hm->cell.z * 20.0f + hm->origin.z;
                        hi = (float)(hm->size - 20) * hm->cell.z + hm->origin.z;
                        dst[2] = max(dst[2], hi);           /* z runs from the far edge (a negative cell) */
                        dst[2] = min(dst[2], lo);
                    }
                    d3_SetObjectPos(d3_Camera, (struct D3VECTOR *)dst);
                    rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                    if (rec != 0)
                        SV_CAMREC(rec);
                }
                sv_DragLastPos[0] = dst[0];
                sv_DragLastPos[1] = dst[1];
                sv_DragLastPos[2] = dst[2];
                sv_DragLastX = (int)dm_P.x;
                sv_DragLastY = (int)dm_P.y;
                sv_MouseCamActive = 1;
            }
        }
    }
    return (sv_MouseCamActive - dm_L.wdown) + dm_L.wup + sv_ZoomTurnOn;
}
/* 0x527640, the per-frame camera update: with a camera and no cutscene (0x6333e0, 0x760440, 0x62a8cc = -1) 0x5263a0 (when no mouse button), the mouse control 0x526cf0 (unless flag 0x8000 of 0x760638), the
   height easing 0x525ee0 (option 0x62381c) and, when the camera moved without a selection, the stored camera record of the player (0x506430: position, direction, zoom); returns 0x6f22b4; without a camera it
   restores the clip window to the screen (0x5f3238 / 0x5f323c, 0x4113f0 (mouse x, mouse x)) once */

int sv_Unk527640(void)
{
    char *rec;

    if (sv_CameraObj != 0 && tb_Paused == 0) {
        if (ai_Unk760440 == 0 && ai_Unk62a8cc == -1) {
            if (dm_L.rhold == 0)
                ai_Unk6f22b4 = sv_ScrollCamera();
            if (-1 < (char)(sv_FrameMask >> 8))
                sv_MouseCamera();
            if (ai_Unk62381c != 0 && kb_Down[0xf] == 0)
                sv_Unk525ee0();
            if (ai_Unk6f22b4 != 0 && sv_FrameDepth == sv_FrameBase && ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0
                && (rec = gm_PlayerRecord(ai_Chars[ai_PlayerId])) != 0) {
                CAMLOG(rec);
                *(float *)(rec + 0x88) = sv_CameraAngleFactor;
            }
            sv_ViewRotX = *(int *)(sv_CameraObj + 0x84);
            sv_ViewRotY = *(int *)(sv_CameraObj + 0x88);
            sv_ViewRotZ = *(int *)(sv_CameraObj + 0x8c);
            sv_ViewPosX = *(int *)(sv_CameraObj + 0x4c);
            sv_ViewPosY = *(int *)(sv_CameraObj + 0x50);
            sv_ViewPosZ = *(int *)(sv_CameraObj + 0x54);
        }
        return ai_Unk6f22b4;
    }
    ai_Unk6f22b4 = 0;
    if (di_Absolute == 0) {
        di_MaxY = (int)d2_ScreenH;
        di_MaxX = d2_ScreenW - 8;
        di_MinX = 0;
        di_MinY = 0;
        di_Absolute = 1;
        di_SetMousePos((int)dm_L.x, (int)dm_L.x);
    }
    return 0;
}
/* 0x527840, moves the camera (animated, d3t_PlaceObjectV) to 600 units back along its view from the building
   b's 3D object, like sv_CameraToObject: the move takes the camera step ai_CameraStep scaled by the distance
   (0.0005 per unit), at least one step and at most three; the stored view sv_ViewPos / sv_ViewRot and the
   player's camera record get the target position, b becomes sv_SelectedObject */

void sv_FlyCameraToObject(int b)
{
    float pos[4];                   /* the target camera position (a padded vector) */
    float dir[3];                   /* the camera rotation, kept */
    float lo, hi;                   /* the object's height range */
    float back[4];                  /* 600 units back along the view */
    float cen[4];                   /* the object's centre in the world */
    float rot[16];                  /* the camera matrix, transposed (a 4x4 D3MATRIX) */
    float h;                        /* the camera height above the centre, then the pull-in factor */
    double t;                       /* the mean terrain height there */
    float step, dx, dy, dz, len;
    char *obj;
    D3OBJECT *cam;
    AIFIN *rec;
    const float *ax = (const float *)ai_AxisPair;
    float vx, vy, vz;               /* the view axis (0, 0, 1) scaled to -600 */

    obj = ((AIBLD *)b)->object;
    d3t_GetHeightRange(obj, &lo, &hi);
    d3_MatTranspose(&d3_Camera->matrix, rot);
    vx = ax[0] * -600.0f;
    vy = ax[1] * -600.0f;
    vz = ax[2] * -600.0f;
    back[0] = rot[8] * vz + rot[4] * vy + rot[0] * vx;
    back[1] = rot[9] * vz + rot[5] * vy + rot[1] * vx;
    back[2] = rot[10] * vz + rot[6] * vy + rot[2] * vx;
    d3_PointToWorld(obj, &((D3OBJECT *)obj)->pos, cen);
    pos[0] = cen[0] + back[0];
    pos[1] = cen[1] + back[1];
    pos[2] = cen[2] + back[2];
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    h = pos[1] - cen[1];
    d3t_GetHeightRange(obj, &lo, &hi);
    h = back[1] / ((hi - lo) * 0.5f + h);
    pos[0] = pos[0] - (h * back[0] - back[0] * 1.8);
    pos[2] = pos[2] - (h * back[2] - back[2] * 1.8);
    dir[0] = d3_Camera->rotation.x;
    dir[1] = d3_Camera->rotation.y;
    dir[2] = d3_Camera->rotation.z;
    d3anim_FreePath(d3_Camera);
    d3t_CamMove = 0;
    d3t_CamMoving = 0;
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    cam = d3_Camera;
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    dx = pos[0] - cam->pos.x;
    dy = pos[1] - cam->pos.y;
    dz = pos[2] - cam->pos.z;
    step = (float)ai_CameraStep;
    len = (float)sqrt(dz * dz + dy * dy + dx * dx) * step * 0.0005f;   /* the steps for the distance */
    d3t_PlaceObjectV(cam, (struct D3VECTOR *)pos, (struct D3VECTOR *)dir, (int)min(ai_CameraStep * 3, max(step, len)), 0x68);
    sv_ViewRot[0] = dir[0];
    sv_ViewPos[0] = pos[0];
    sv_ViewRot[1] = dir[1];
    sv_ViewPos[1] = pos[1];
    sv_ViewRot[2] = dir[2];
    sv_ViewPos[2] = pos[2];
    rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
    if (rec != 0) {
        rec->camPos[0] = pos[0];
        rec->camPos[1] = pos[1];
        rec->camPos[2] = pos[2];
        rec->camRot[0] = dir[0];
        rec->camRot[1] = dir[1];
        rec->camRot[2] = dir[2];
    }
    sv_SelectedObject = (char *)b;
    sv_SelectedModel = (int)((AIBLD *)b)->object;
}

int d3t_PlaceObject(struct D3OBJECT *camera, float px, float py, float pz, float dx, float dy, float dz, int unk, char flags);                     /* 0x411680 */
int _stricmp();                     /* 0x5eaa30 (_strcmpi) */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */
unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
unsigned char * gm_BuildNext(void);                     /* 0x501030 */
int d3t_SceneIndex();                     /* 0x411be0 */
void d3_SetWorldPosRot();                     /* 0x449920 */
int d3t_PlayAnim();                     /* 0x411960 */

/* 0x528180, the callback of 0x5282f0 over the model parts of the building b (+0x61 its model): for the part named "dummy_FAHNE" it creates the banner object "sp_WIMPEL" (model "sonstiges\sp_WIMPEL.baf") at the
   dummy's position (0x458a20), turned by pi around the y axis (0x4545c0), coloured for the owner like 0x528290, and flags it (+0x211 bit 1 cleared, +0x212 = +0x212 & 0xf7 | 0x44); returns 1; ROUGH (the 4th argument of
   0x458a20 is an uninitialised register in the original) */
char sv_Unk528180(char *part, char *b)
{
    float z[3], w[3], m[16];   /* m is a D3MATRIX: d3_MatFromAngles writes 0x40 bytes (m[3] overflowed the stack) */
    char *o;
    int junk;

    if (_stricmp(part, "dummy_FAHNE") == 0) {
        z[0] = 0;
        z[1] = 0;
        z[2] = 0;
        d3_PointToWorldK(part, part + 0x4c, w, junk);   /* @original: the 4th argument is an uninitialised register in the original (0x5281b5) */
        o = (char *)d3_CreateObject(*(int *)(b + 0x61), w, z, "sp_WIMPEL");
        z[0] = 0;
        z[1] = 3.1415927f;
        z[2] = 0;
        d3_MatFromAngles(z, m);
        d3_SetWorldPosRot(o, w, m);
        if (*(unsigned short *)(b + 0x27) != 0xffff) {
            d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), (char)(*(char *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x54) + 'f'), 1);
            *(int *)(o + 0x220) = 4;
        }
        d3t_PlayAnim(o, "sonstiges\\sp_WIMPEL.baf", 1);
        o[0x212] = (o[0x212] & 0xf7) | 0x44;
        o[0x211] &= 0xfd;
    }
    return 1;
}
/* 0x527bb0, puts the camera directly 600 units back along its view from the building b's 3D object (its centre,
   the terrain's mean height plus the zoom height, pulled in by the object's height range): the stored view
   sv_ViewPos / sv_ViewRot and the player's camera record get the new position, b becomes sv_SelectedObject;
   sv_FlyCameraToObject does the same with an animated move */
void sv_CameraToObject(int b)
{
    float pos[4];                   /* the new camera position (a padded vector) */
    float dir[3];                   /* the camera rotation, kept */
    float lo, hi;                   /* the object's height range */
    float back[4];                  /* 600 units back along the view */
    float cen[4];                   /* the object's centre in the world */
    float rot[16];                  /* the camera matrix, transposed (a 4x4 D3MATRIX) */
    float h;                        /* the camera height above the centre, then the pull-in factor */
    double t;                       /* the mean terrain height there */
    char *obj;
    AIFIN *rec;
    const float *ax = (const float *)ai_AxisPair;
    float vx, vy, vz;               /* the view axis (0, 0, 1) scaled to -600 */

    obj = ((AIBLD *)b)->object;
    d3t_GetHeightRange(obj, &lo, &hi);
    d3_MatTranspose(&d3_Camera->matrix, rot);
    vx = ax[0] * -600.0f;
    vy = ax[1] * -600.0f;
    vz = ax[2] * -600.0f;
    back[0] = rot[8] * vz + rot[4] * vy + rot[0] * vx;
    back[1] = rot[9] * vz + rot[5] * vy + rot[1] * vx;
    back[2] = rot[10] * vz + rot[6] * vy + rot[2] * vx;
    d3_PointToWorld(obj, &((D3OBJECT *)obj)->pos, cen);
    pos[0] = cen[0] + back[0];
    pos[1] = cen[1] + back[1];
    pos[2] = cen[2] + back[2];
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    h = pos[1] - cen[1];
    d3t_GetHeightRange(obj, &lo, &hi);
    h = back[1] / ((hi - lo) * 0.5f + h);
    pos[0] = pos[0] - (h * back[0] - back[0] * 1.8);
    pos[2] = pos[2] - (h * back[2] - back[2] * 1.8);
    dir[0] = d3_Camera->rotation.x;
    dir[1] = d3_Camera->rotation.y;
    dir[2] = d3_Camera->rotation.z;
    d3anim_FreePath(d3_Camera);
    d3t_CamMove = 0;
    d3t_CamMoving = 0;
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    sv_ViewRot[0] = dir[0];
    sv_ViewRot[2] = dir[2];
    sv_ViewPos[0] = pos[0];
    sv_ViewRot[1] = dir[1];
    sv_ViewPos[2] = pos[2];
    sv_ViewPos[1] = pos[1];
    d3_SetObjectPos(d3_Camera, pos);
    d3_SetObjectRot(d3_Camera, dir);
    rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
    if (rec != 0) {
        rec->camPos[0] = pos[0];
        rec->camPos[1] = pos[1];
        rec->camPos[2] = pos[2];
        rec->camRot[0] = dir[0];
        rec->camRot[1] = dir[1];
        rec->camRot[2] = dir[2];
    }
    sv_SelectedObject = (char *)b;
    sv_SelectedModel = (int)((AIBLD *)b)->object;
}
/* 0x527e90, moves the camera (animated, d3t_PlaceObjectV) to 600 units back along its view from the 3D object
   m, like sv_FlyCameraToObject for a bare object: no stored view, no selection @deep read whole against 0x527e90..0x528130: the statements, the AxisPair*-600 back vector through the 4x4 rows, the two mean-height placements, the distance-scaled step min(3*step, max(step, len)) and the 5-argument d3t_PlaceObjectV all match (192 vs 192 instructions). Left: allocation only - the height-above-centre temp h gets a low slot ([esp+0x2c]) where the original keeps it at the frame top ([esp+0xb8], frame 0x9c vs ours 0x8c), and with it the second d3t_GetHeightRange's push ecx is scheduled before the pos[1] store (ours 0x527f97) instead of after (0x527fbe); tried h declared first: no change */

void sv_FlyCameraToModel(int m)
{
    float h;                        /* the camera height above the centre, then the pull-in factor */
    float pos[4];                   /* the target camera position (a padded vector) */
    float dir[3];                   /* the camera rotation, kept */
    float lo, hi;                   /* the object's height range */
    float back[4];                  /* 600 units back along the view */
    float cen[4];                   /* the object's centre in the world */
    float rot[16];                  /* the camera matrix, transposed (a 4x4 D3MATRIX) */
    double t;                       /* the mean terrain height there */
    float step, dx, dy, dz, len;
    D3OBJECT *cam;
    const float *ax = (const float *)ai_AxisPair;
    float vx, vy, vz;               /* the view axis (0, 0, 1) scaled to -600 */

    d3t_GetHeightRange((char *)m, &lo, &hi);
    d3_MatTranspose(&d3_Camera->matrix, rot);
    vx = ax[0] * -600.0f;
    vy = ax[1] * -600.0f;
    vz = ax[2] * -600.0f;
    back[0] = rot[8] * vz + rot[4] * vy + rot[0] * vx;
    back[1] = rot[9] * vz + rot[5] * vy + rot[1] * vx;
    back[2] = rot[10] * vz + rot[6] * vy + rot[2] * vx;
    d3_PointToWorld((char *)m, &((D3OBJECT *)m)->pos, cen);
    pos[0] = cen[0] + back[0];
    pos[1] = cen[1] + back[1];
    pos[2] = cen[2] + back[2];
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    h = pos[1] - cen[1];
    d3t_GetHeightRange((char *)m, &lo, &hi);
    h = back[1] / ((hi - lo) * 0.5f + h);
    pos[0] = pos[0] - (h * back[0] - back[0] * 1.8);
    pos[2] = pos[2] - (h * back[2] - back[2] * 1.8);
    dir[0] = d3_Camera->rotation.x;
    dir[1] = d3_Camera->rotation.y;
    dir[2] = d3_Camera->rotation.z;
    d3anim_FreePath(d3_Camera);
    d3t_CamMove = 0;
    d3t_CamMoving = 0;
    t = d3t_GetMeanHeight((struct D3VECTOR *)pos);
    cam = d3_Camera;
    pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t;
    dx = pos[0] - cam->pos.x;
    dy = pos[1] - cam->pos.y;
    dz = pos[2] - cam->pos.z;
    step = (float)ai_CameraStep;
    len = (float)sqrt(dz * dz + dy * dy + dx * dx) * step * 0.0005f;   /* the steps for the distance */
    d3t_PlaceObjectV(cam, (struct D3VECTOR *)pos, (struct D3VECTOR *)dir, (int)min(ai_CameraStep * 3, max(step, len)), 0x68);
}
/* 0x528140, moves the camera (0x411680) to the stored view (0x75fb80 position, 0x75fb90 direction) with the step 0x623814 / 8 */

void sv_Unk528140(void)
{
    d3t_PlaceObject(sv_CameraObj, *(float *)&sv_ViewPosX, *(float *)&sv_ViewPosY, *(float *)&sv_ViewPosZ, *(float *)&sv_ViewRotX, *(float *)&sv_ViewRotY, *(float *)&sv_ViewRotZ, ai_CameraStep, 8);
}
/* 0x528290, a callback over the objects of a model (name o): for the one named "sp_WIMPEL" (the banner) sets its colour (0x446f30 (o, o +0x1cc, 0x66 + the byte +0x54 of the character record (0x770bd4) of the owner, 1))
   when the owner (+0x27) is valid; returns 1 */

char sv_Unk528290(char *o, char *b)
{
    if (_stricmp(o, "sp_WIMPEL") == 0) {
        if (*(unsigned short *)(b + 0x27) != 0xffff)
            d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), (char)(*(char *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x54) + 'f'), 1);
    }
    return 1;
}
/* 0x5282f0, colours the banner of the building b (+0x61 its model) for a character owner (type 5 / 6 / 7, +0x27): the model's parts are walked (0x438d40, 0x100, the callback 0x528180) with the model's +0x1f0 cleared
   when its flag +0x210 & 1 is not set */

void sv_Unk5282f0(char *b)
{
    char *m;
    char c;
    int save;

    m = *(char **)(b + 0x61);
    if (m != 0 && *(unsigned short *)(b + 0x27) != 0xffff) {
        c = ai_Chars[*(unsigned short *)(b + 0x27)][2];
        if (c == 6 || c == 7 || c == 5) {
            if ((*(unsigned char *)(m + 0x210) & 1) == 0) {
                save = *(int *)(m + 0x1f0);
                *(int *)(m + 0x1f0) = 0;
                d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x100, sv_Unk528180, b);
                *(int *)(*(int *)(b + 0x61) + 0x1f0) = save;
                return;
            }
            d3_ForEachObject(ai_Unk5fb5b0, m, 0x100, sv_Unk528180, b);
        }
    }
}
/* 0x528390, colours the banners of all buildings of the character c (index): in scene 0 every object (0x501190 (1, 4, c) / 0x501030) with a model is walked (0x438d40, 0x340, 0x528290), and the buildings of the
   list 0x4ffe90 (0, 3, 7, 3, c, 4, 0x1d) get the model scene (0x411be0) and the banner number (the byte +0x54 of the character's record - 0x599) through 0x446f30; the scene is restored */

void sv_Unk528390(int c)
{
    int scene, b, n;
    char *o;
    char *ch;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    o = (char *)gm_BuildQuery(1, 4, c);
    while (o != 0) {
        if (*(int *)(o + 0x61) != 0)
            d3_ForEachObject(ai_Unk5fb5b0, *(int *)(o + 0x61), 0x340, sv_Unk528290, o);
        o = (char *)gm_BuildNext();
    }
    o = gm_Query(0, 3, 7, 3, c, 4, 0x1d);
    while (o != 0) {
        b = *(int *)(o + 0x3b);
        if (b != 0) {
            d3if_SelectScene(d3t_SceneIndex(*(int *)(b + 0x88)), 1);   /* the 1 is SelectScene's (0x528418 is pushed before the inner call) */
            ch = ai_Chars[c];
            if (c * 0x21c != -(unsigned)&ai_Chars && *(int *)(ch + 0x54) != 0 && (ch[2] == 5 || ch[2] == 6 || ch[2] == 7)) {
                n = *(int *)(ch + 0x54) - 0x599;
                if (n != 0)
                    d3if_SelectTextureSet(**(int **)(b + 0x124), *(int *)(**(int **)(b + 0x124) + 0x1cc), n, 1);
            }
        }
        o = gm_QueryNext();
    }
    d3if_SelectScene(scene, 1);
}
/* 0x528490, the chimney smoke of the building o (not kind 7): when one of its characters (type 4 at +0x180, home +0x16c = o) is at work (no group or a group without +0xc8) and the time of day is inside the window of
   0x617830 / 0x617840, an effect "effekte\\Schornstein_dunkel_esc" (0x4287b0) is started at the "dummy_RAUCH_0" dummy of the model (0x429ab0) and its handle stored at +0x95; otherwise a running effect is stopped
   (0x429e90) and the handle cleared. ROUGH */

void sv_Unk528490(unsigned char *o)
{
    int k, i, scene, d, e;
    char *c;
    float pos[3];

    k = gm_ClockSeason(ai_Time);
    scene = d3_CurrentScene;
    if (ai_BTypes[*o * 0x24d] != 7 && *(int *)(o + 0x61) != 0) {
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (c[8] != 0 && *(short *)c != -1 && c[2] != 10 && *(unsigned char **)(c + 0x16c) == o && c[0x180] == 4) {
                if (*(int *)(c + 0x17c) == 0 || *(int *)(*(int *)(c + 0x17c) + 200) == 0) {
                    if (*(int *)(o + 0x95) != -1)
                        return;
                    if ((float)(*(unsigned short *)(ai_Time + 4)) < gm_DayStartHours[k])
                        return;
                    if (gm_DayEndHours[k] <= (float)(*(unsigned short *)(ai_Time + 4)))
                        return;
                    d3if_SelectScene(0, 1);
                    d = d3_FindObject(*(int *)(o + 0x61), 0x100, 0, "dummy_RAUCH_0");
                    if (d != 0 && (e = (int)evt_Load("effekte\\Schornstein_dunkel_esc")) != 0) {
                        d3_PointToWorld(d, d + 0x4c, pos);
                        if (evt_RunScriptArgs((EVTSCRIPT *)e, 3, (int)pos[0], (int)pos[1], (int)pos[2]) != 0)
                            *(int *)(o + 0x95) = *(int *)(e + 0x100);
                        *(int *)(e + 0x104) = 0;
                    }
                    d3if_SelectScene(scene, 1);
                    return;
                }
                break;
            }
        }
        if (*(int *)(o + 0x95) != -1) {
            e = (int)evt_FindById(*(int *)(o + 0x95));
            if (e != 0)
                evt_Stop((EVTSCRIPT *)e);
            o[0x95] = 0xff;
            o[0x96] = 0xff;
            o[0x97] = 0xff;
            o[0x98] = 0xff;
        }
    }
}
/* 0x528660, a callback collecting the names o that contain "sp_WIMPEL" into the list p (p[0] the count, then the pointers); 1 while below 0x20 */

char sv_Unk528660(char *o, int *p)
{
    if (strstr(o, "sp_WIMPEL") != 0) {
        p[p[0] + 1] = (int)o;
        p[0]++;
    }
    return p[0] < 0x20;
}

extern int d3_QuadTree, ai_Unk623864, ai_Unk623868, ai_Unk62387c, ai_Unk623888, ai_Unk62388c, ai_Unk623890, ai_Unk623894, ai_Unk631078;  /* 0x6f1c28, 0x623864, 0x623868, 0x62387c, 0x623888, 0x62388c, 0x623890, 0x623894, 0x631078 */
extern int ai_Unk62386c[4];  /* 0x62386c */

int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjFont();                     /* 0x4760b0 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */
int gm_BuildingLevel();                     /* 0x508d70 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);                     /* 0x46a960 */
unsigned short * gm_Unk50a050(unsigned short *p);                     /* 0x50a050 */
int gm_Unk508cc0();                     /* 0x508cc0 */
int sv_Unk528800(int x, int y, int text);

/* 0x5286a0, tints the banners of the building b: its model's parts named "sp_WIMPEL" (0x438d40, 0x40, the collector 0x528660; the model's +0x1f0 cleared when its flag +0x210 & 1 is not set) are recoloured
   (0x458780 with the colour 0x6f1c28, when set) and updated (0x447190) */
void sv_Unk5286a0(char *b)
{
    int list[33];
    int i, m, save;

    m = *(int *)(b + 0x61);
    if (m != 0) {
        for (i = 0; i < 0x21; i++)
            list[i] = 0;
        if ((*(unsigned char *)(m + 0x210) & 1) == 0) {
            save = *(int *)(m + 0x1f0);
            *(int *)(m + 0x1f0) = 0;
            d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x40, sv_Unk528660, list);
            *(int *)(*(int *)(b + 0x61) + 0x1f0) = save;
        } else
            d3_ForEachObject(ai_Unk5fb5b0, m, 0x40, sv_Unk528660, list);
        i = 0;
        if (0 < list[0]) {
            do {
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)list[i + 1], (D3QNODE *)d3_QuadTree);
                d3if_Delete((struct D3OBJECT *)list[i + 1]);
                i++;
            } while (i < list[0]);
        }
    }
}
/* 0x528770, in scene 0 goes over the objects of kind 6 (0x501190 (1, 6)): for those whose owner (+0x27) is another character of type 5 / 6 / 7 and which are not flagged (+0x5a & 1) the banner is tinted (0x5286a0) and coloured (0x5282f0) */

void sv_Unk528770(void)
{
    int scene;
    char *o;
    char *c;
    char t;

    scene = d3_CurrentScene;
    c = 0;
    d3if_SelectScene(0, 1);
    o = (char *)gm_BuildQuery(1, 6);
    while (o != 0) {
        if (*(unsigned short *)(o + 0x27) != 0xffff)
            c = ai_Chars[*(unsigned short *)(o + 0x27)];
        if (c != 0 && c != ai_PlayerCharC && ((t = c[2]) == 6 || t == 7 || t == 5) && (o[0x5a] & 1) == 0) {
            sv_Unk5286a0(o);
            sv_Unk5282f0(o);
        }
        o = (char *)gm_BuildNext();
    }
    d3if_SelectScene(scene, 1);
}
/* 0x528800, creates a panel button at (x + 3, y) from the image 0xc5 (0x46bf10) with the text, the state 0x54, the flag +0x5c = 1 and the frame 0x60; returns the widget id */

int sv_Unk528800(int x, int y, int text)
{
    int w;

    w = d2_WinCreateSlider(x + 3, y, 0xc5, text, d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    *(int *)(d2_Objs + 0x5c + w * 0x2ec) = 1;
    d2_SetObjWidth(w, 0x60);
    return w;
}
/* 0x528860, opens the building info panel ("panel\\infopanel_gebaeude", 0x623860) for the object o: the title text ("$Z%s$A%s", with the owner name "$A>%s<" when o+5 is set; kind 0x47 uses the player's name 0x870),
   the building image (0x427 + kind), the health bar 0x623868 (0x46a960) except for the kinds 0x47 / 0x1d / 0x57, and for owned buildings the buttons (0x528800: 0x8c at 0x623894, "Transport" 0xa6 at 0x62388c for
   kinds that carry goods). ROUGH */

void sv_Unk528860(unsigned char *o)
{
    int hp, k, v;
    char text[128];
    char *b;
    unsigned short owner;
    unsigned fl;
    char kind;

    hp = gm_BuildingLevel(o);
    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
    d2_SetForm(sv_PanelForm, 1);
    if (*o == 0x47) {
        io_PrintText("$Z%s$A%s", 0x870, (char *)(unsigned)&ai_PlayerSlots + *(int *)(o + 0x65) * 0x2f4);
    } else {
        io_CharacterNameMarkup(*(unsigned short *)(o + 0x27), 2, 1, text);
        if (o[5] != 0)
            io_PrintText("$Z%s$A%s$A>%s<", text, *o * 0xe + 0x48e, o + 5);
        else
            io_PrintText("$Z%s$A%s", text, *o * 0xe + 0x48e);
    }
    d2_SetForm(sv_PanelForm, 0);
    ai_Unk623864 = d2_WinOpenPic((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1, 0x48, *o + 0x427, d2_LastWindow);
    *(int *)(d2_Objs + 0x4c + ai_Unk623864 * 0x2ec) = 1;
    owner = *(unsigned short *)(o + 0x27);
    if (owner != 0xffff && owner != *(unsigned short *)ai_PlayerCharC)
        *(char **)(d2_Objs + 0x2e8 + ai_Unk623864 * 0x2ec) = ai_Chars[owner];
    kind = *o;
    if (kind == 0x47 || kind == 0x1d || kind == 0x57)
        ai_Unk623868 = -1;
    else {
        ai_Unk623868 = d2_WinOpenSlider(0x15, 0x7b, 0x38, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, 0, 100, hp);
    }
    if ((o[0x5a] & 1) == 0 && *o != 0x47 && *o != 0x1d && *o != 0x57) {
        k = gm_BuildingClass(*o);
        b = ai_BTypes + *o * 0x24d;
        fl = gm_PermissionBits(ai_PlayerId, o, 0, 0);
        v = ai_Unk1441f6c;
        if (*(short *)(o + 0x27) != (short)ai_PlayerId && *(short *)(o + 0x25) != (short)ai_PlayerId && (fl & 0x10) == 0)
            v = ai_Unk1441f74;
        ai_Unk623894 = sv_Unk528800(0, 0x8c, v);
        if ((k == 1 || k == 2 || k == 4 || k == 7 || k == 8 || *b == 0x10 || *b == 0x1e || *b == 0x1f || *b == 0x1d) && *b != 5) {
            if (ai_FallbackBuilding == 0)
                ai_Unk62388c = sv_Unk528800(0, 0xa6, (int)"Transport");
            ai_Unk62386c[0] = -1;
            return;
        }
        ai_Unk62386c[0] = -1;
        ai_Unk62388c = -1;
        return;
    }
    ai_Unk623894 = -1;
    ai_Unk62388c = -1;
    ai_Unk62386c[0] = -1;
}
/* 0x528b80, opens the info panel for the item p (not the ids 0x92 .. 0x97 and 0x2db): the title text with the item's name (0x9a8 + 2 * id; the selected building's name and owner before it), the image (0xdf + id), and for items of the class 2 / 6 the owner's buttons
   (0x528800: 0x8c at 0x623894 when the item may be changed (0x508cc0), 0xa6 at 0x623888) */

void sv_Unk528b80(unsigned short *p)
{
    unsigned char *b, *sel;
    short *owner;
    int w;
    unsigned fl;
    char c;

    if (sv_PanelForm == -1 && (*p < 0x92 || 0x97 < *p) && *p != 0x2db) {
        sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 1);
        sel = (unsigned char *)ai_PlayerBuilding;
        b = sel;
        if (sel == 0) {
            b = (unsigned char *)ai_FallbackBuilding;
            sel = 0;
        }
        if (b == 0)
            io_PrintText("$Z%s", *p * 2 + 0x9a8);
        else if (b[5] == 0)
            io_PrintText("$Z%s$A%s", *b * 0xe + 0x48e, *p * 2 + 0x9a8);
        else
            io_PrintText("$Z%s$A>%s<$A%s", *b * 0xe + 0x48e, b + 5, *p * 2 + 0x9a8);
        d2_SetForm(sv_PanelForm, 0);
        w = d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1, 0x48, *p + 0xdf, d2_LastWindow);
        *(int *)(d2_Objs + 0x4c + w * 0x2ec) = 1;
        if (b != 0 && ((c = ai_Goods[*p * 0x41]) == 2 || c == 6)) {
            owner = (short *)gm_Unk50a050(p);
            fl = gm_PermissionBits(ai_PlayerId, b, 0, 0);
            if ((*owner == (short)ai_PlayerId || (fl & 0x10) != 0) && gm_Unk508cc0(b, p, 0) != 0)
                ai_Unk623894 = sv_Unk528800(0, 0x8c, ai_Unk1441f78);
            ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
        }
    }
}
/* 0x528d90, opens the info panel for the transporter p ("panel\\infopanel_transporter"): the title ("$Z%2N1$A%s": the owner's id and the name 0x9a8 + 2 * id), the image 0xdf + id, for the own transporter the button 0xac
   (0x623890), the load bar 0x623868 (+0x12 byte) and notes the panel's form 0x62387c and the position record 0x631078 */

void sv_Unk528d90(unsigned short *p)
{
    short *owner;
    int x, w;

    if (sv_PanelForm != -1)
        return;
    owner = (short *)gm_Unk50a050(p);
    if (owner == 0)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_transporter");
    d2_SetForm(sv_PanelForm, 1);
    io_PrintText("$Z%2N1$A%s", *owner, *p * 2 + 0x9a8);
    d2_SetForm(sv_PanelForm, 0);
    x = ((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1;
    w = d2_WinOpenPic(x, 0x30, *p + 0xdf, d2_LastWindow);
    *(int *)(d2_Objs + 0x4c + w * 0x2ec) = 1;
    if (*owner == (short)ai_PlayerId)
        ai_Unk623890 = sv_Unk528800(x, 0xac, ai_Unk144ad24);
    ai_Unk623868 = d2_WinOpenSlider(0x18, 0x66, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
    d2_SetObjValue(ai_Unk623868, 0, 100, (char)p[9]);
    d2_SetForm(sv_PanelForm, 2);
    ai_Unk62387c = d2_LastWindow;
    ai_Unk631078 = *(int *)((char *)p + 0x3b);
}

extern int ai_Unk623884;  /* 0x623884 */
extern int ai_Unk623898;  /* 0x623898 */
extern int ai_Unk623880;  /* 0x623880 */
extern int ai_Unk6f2334;  /* 0x6f2334 */
extern unsigned char ai_GameState;  /* 0x6f25ac */
extern char ai_Unk6f2338;  /* 0x6f2338 */
extern int d2_PointerObj;  /* 0x5fe710 */
extern char *ai_Unk7662c0p;
extern unsigned short * ai_Unk75ec5c;
extern int ai_Unk6f23bc, ai_Unk6f23c0, ai_Unk6f23c4, ai_Unk6f23c8, ai_Unk6f23cc, ai_Unk6f23d0, ai_Unk6f23d4, ai_Unk6f23d8, ai_Unk6f2294, ai_BuildingPerms, ai_Unk6f2320;  /* 0x6f23bc, 0x6f23c0, 0x6f23c4, 0x6f23c8, 0x6f23cc, 0x6f23d0, 0x6f23d4, 0x6f23d8, 0x6f2294, 0x6f231c, 0x6f2320 */
extern int ai_Unk5fe7d0;  /* 0x5fe7d0 */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
int gm_MinutesBetween();                     /* 0x4fe280 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
void rt_Unk4bf5b0();                     /* 0x4bf5b0 */
int d2_RemoveWindowObjects();                     /* 0x472d20 */
int gm_ListAmount();                     /* 0x50aaf0 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
int ai_AmountMenu();                     /* 0x5c1130 */
int ai_TransportWindow();                     /* 0x5af1d0 */
int ai_Unk5b8fc0();                     /* 0x5b8fc0 */
int ai_Unk5b84f0();                     /* 0x5b84f0 */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
int sv_ReturnZero(void);
void sv_Unk528ee0(void);
void sv_Unk5290f0(unsigned short *p);
void sv_Unk529380(void);
void sv_Unk528860(unsigned char *o);
void sv_Unk528b80(unsigned short *p);
void sv_Unk528d90(unsigned short *p);

/* 0x528ee0, opens the default info panel when no panel is open (0x623860): without a selection the "standard" panel with the buttons 0x8c / 0xa6; for an own building (0x6f230c with the owner +0x27 = the player) the
   building panel with the name, the image, the health bar and the buttons 0x8c, 0xa6 (0x623898 for kind 0x1e) */
void sv_Unk528ee0(void)
{
    int hp;
    char text[128];

    if (sv_PanelForm != -1)
        return;
    if (ai_FallbackBuilding == 0) {
        if (ai_PlayerBuilding == 0) {
            sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_standard");
            d2_SetForm(sv_PanelForm, 0);
            ai_Unk623884 = sv_Unk528800(0, 0x8c, ai_Unk1441f68);
            ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
            return;
        }
    } else if (*(short *)(ai_FallbackBuilding + 0x27) == (short)ai_PlayerId) {
        hp = gm_BuildingLevel(ai_FallbackBuilding);
        sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 0);
        d2_SetForm(sv_PanelForm, 1);
        io_CharacterNameMarkup(*(unsigned short *)(ai_FallbackBuilding + 0x27), 2, 1, text);
        io_PrintText("$Z%s$A%s$A%s", text, *(unsigned char *)ai_FallbackBuilding * 0xe + 0x48e, ai_FallbackBuilding + 5);
        d2_SetForm(sv_PanelForm, 0);
        d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1, 0x4c, *(unsigned char *)ai_FallbackBuilding + 0x427, d2_LastWindow);
        ai_Unk623868 = d2_WinOpenSlider(0x15, 0x7a, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, 0, 100, hp);
        ai_Unk623894 = sv_Unk528800(0, 0x8c, ai_Unk1441f6c);
        if (*(unsigned char *)ai_FallbackBuilding == 0x1e) {
            ai_Unk623898 = sv_Unk528800(0, 0xa6, ai_Unk1441f70);
            return;
        }
        ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
    }
}
/* 0x5290f0, opens the person info panel ("panel\\infopanel_personen") for the character p (0 for none or several selected: the text "%i %s" with the count 0x6f2334 and 0x144adb0): the name and age
   (0x514e90, "%s (%i)"), the health bar 0x623868 (0x50c520 of the character, -127 .. 127), the standing image for types 5 / 6 / 7 (+0x54), the two small buttons 0x5df / 0x5e0 (0x52eaa0) for kinds 10 .. 12, and the portrait
   (+0x18c, or 0x521 for none) with the character noted in its widget */

void sv_Unk5290f0(unsigned short *p)
{
    char text[512];
    int hp, kind, age, x;
    short w;
    char t;

    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_personen");
    d2_SetForm(sv_PanelForm, 1);
    if (p == 0 || 1 < ai_Unk6f2334) {
        sprintf(text, "%i %s", ai_Unk6f2334, ai_Unk144adb0);
        io_PrintText(text);
    } else {
        hp = gm_Opinion(ai_PlayerId, *p);
        kind = 0;
        if (*((char *)p + 0x165) != 0)
            kind = gm_TitleJobGroup(*((char *)p + 0x165));
        gm_HealthRatio(p);
        age = (int)0;
        io_CharacterNameMarkup(*p, 1, 4, text);
        io_PrintText("%s (%i)", text, age);
        d2_SetForm(sv_PanelForm, 0);
        w = *(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4);
        ai_Unk623868 = d2_WinOpenSlider(0x18, 0x6c, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(ai_Unk623868, -127, 0x7f, hp);
        t = *((char *)p + 2);
        if (t == 5 || t == 6 || t == 7)
            d2_WinOpenPic(((w - 0x30) >> 1) - 8, 0x30, *(int *)(p + 0x2a), d2_LastWindow);
        if (kind == 0xb || kind == 0xc || kind == 10) {
            d2_WinOpenPic(0x19, 0x7e, 0x5df, d2_LastWindow);
            sv_ValueBar(0x29, 0x7e, *((char *)p + 0x82), 0x4f1);
            d2_WinOpenPic(0x19, 0x8c, 0x5e0, d2_LastWindow);
            sv_ValueBar(0x29, 0x8c, *((char *)p + 0x83), 0x4f1);
        }
    }
    d2_SetForm(sv_PanelForm, 0);
    x = (*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1;
    if (p != 0) {
        ai_Unk623880 = d2_WinOpenPic(x, 0x38, *(int *)(p + 0xc6), d2_LastWindow);
        *(unsigned short **)(d2_Objs + 0x2e8 + ai_Unk623880 * 0x2ec) = p;
        *(int *)(d2_Objs + 0x4c + ai_Unk623880 * 0x2ec) = 1;
        return;
    }
    d2_WinOpenPic(x, 0x38, 0x521, d2_LastWindow);
}
/* 0x529380, the per-frame refresh of the info panel 0x623860 (the progress bar +0x1e4 of the widget 0x623864, the owner's tools row 0x62386c ..): for a selected building (0x75ec60) the title and the progress of its
   current job (the object lists 0x53e400 (2, 0, kind 0x1c / 0x1d / 0x1e / 0x1f / 2, 3, owner) the elapsed fraction by 0x4fe280 of the start +0x44 against the end), without one the selected character 0x7662c0 (or
   the transporter 0x75ec5c) is shown: its name, the images of 0x524900, and the progress of its action by the action byte (+0x2f8: 0x8f / 0x16 timers, 0x40 (state +0x70), 0x61, 0x43, 0x62, 0x65, 0x48, 0x6c). ROUGH */

void sv_Unk529380(void)
{
    int i;
    int *q;
    float prog;
    unsigned short *tr;
    char *b;
    char *job;
    int n, a, d;
    char text[256];
    unsigned short fl;

    prog = 0.0f;
    if (sv_PanelForm == -1)
        return;
    b = (char *)sv_SelectedObject;
    if (b == 0 || ai_Unk623868 == -1 || ai_Unk623864 == -1) {
        if (sv_SelectedChar == 0) {
            q = ai_Unk62386c;
            for (i = 0; i < 4; i++, q++)
                if (*q != -1) {
                    d2_RemoveObj(*q);
                    *q = -1;
                }
            if (ai_Unk62387c == -1 || ai_Unk75ec5c == 0)
                return;
            tr = ai_Unk75ec5c;
            if (ai_Goods[*tr * 0x41] != 0x1d)
                return;
            if (ai_Unk623890 != -1 && d2_ClickedId != -1 && d2_PointerObj == ai_Unk623890 && (sv_FrameMask & 0x2000) != 0) {
                rt_Unk4bf5b0(tr);
                tr = ai_Unk75ec5c;
            }
            if (ai_Unk623868 != -1)
                d2_SetObjValue(ai_Unk623868, 0, 100, (char)tr[9]);
            d2_SetForm(sv_PanelForm, 2);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            return;
        }
        if (ai_Unk6f2334 < 2 && sv_SelectedChar != 0) {
            d2_SetForm(sv_PanelForm, 1);
            gm_HealthRatio(sv_SelectedChar);
            io_CharacterNameMarkup(*(unsigned short *)sv_SelectedChar, 1, 4, text);
            io_PrintText("$C%s (%i)", text, 0);
        }
        return;
    }
    n = gm_BuildingLevel(b);
    d2_SetForm(sv_PanelForm, 1);
    io_PrintText("$Z%s$A%s", (unsigned char)*b * 0xe + 0x48e, "");
    d2_SetObjValue(ai_Unk623868, 0, 100, n);
    fl = *(unsigned short *)(b + 0x5a);
    if ((fl & 4) == 0) {
        if ((fl & 1) != 0) {
            job = (char *)he_FindEvent(2, 0, 0xd, 3, *(int *)(b + 1));
            prog = 0.05f;
            if (job != 0) {
                if (3 < *(int *)(job + 0x70))
                    prog = (float)*(int *)(job + 200) / (float)*(int *)(job + 0xc4);
                if (prog < 0.01f)
                    prog = 0.01f;
            }
        } else if ((fl & 0x40) != 0) {
            job = (char *)he_FindEvent(2, 0, 0x1e, 3, *(int *)(b + 1));
            if (job == 0)
                return;
            a = gm_MinutesBetween(job + 0x44, job + 0x52);
            d = gm_MinutesBetween(job + 0x44, ai_Time);
            prog = (float)d / (float)a;
        } else if (!(fl & 0x80)) {
            if (ai_Unk62386c[0] == -1)
                return;
            d2_RemoveObj(ai_Unk62386c[0]);
            ai_Unk62386c[0] = -1;
            return;
        } else {
            job = (char *)he_FindEvent(2, 0, 0x1f, 3, *(int *)(b + 1));
            if (job == 0)
                return;
            a = gm_MinutesBetween(job + 0x44, job + 0x52);
            d = gm_MinutesBetween(job + 0x44, ai_Time);
            prog = (float)d / (float)a;
        }
    } else {
        job = (char *)he_FindEvent(2, 0, 0x1c, 3, *(int *)(b + 1));
        if (job == 0)
            return;
        a = gm_MinutesBetween(job + 0x44, job + 0xb8);
        d = gm_MinutesBetween(job + 0x44, ai_Time);
        prog = (float)d / (float)a;
    }
    *(float *)(d2_Objs + 0x1e4 + ai_Unk623864 * 0x2ec) = prog;
    if (ai_Unk62386c[0] == -1 && (fl & 1) != 0) {
        d2_SetForm(sv_PanelForm, 0);
        ai_Unk62386c[0] = d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
    }
}
/* 0x529fb0, closes the info panel (0x623860): frees the widgets 0x62386c .. (4 ids), destroys the form and clears all the panel's widget ids (0x623864 .. 0x623898) */

void sv_Unk529fb0(void)
{
    int *q;

    if (sv_PanelForm != -1) {
        q = ai_Unk62386c;
        do {
            if (*q != -1) {
                d2_RemoveObj(*q);
                *q = -1;
            }
            q++;
        } while ((int)q < (int)((unsigned)ai_Unk62386c + 0x10));
        d2_CloseView(sv_PanelForm);
        sv_PanelForm = -1;
        ai_Unk623864 = -1;
        ai_Unk623868 = -1;
        ai_Unk623898 = -1;
        ai_Unk623894 = -1;
        ai_Unk62388c = -1;
        ai_Unk623890 = -1;
        ai_Unk623884 = -1;
        ai_Unk623888 = -1;
        ai_Unk623880 = -1;
        ai_Unk62387c = -1;
    }
}
/* 0x52a040, an empty callback returning 0 */

int sv_ReturnZero(void)
{
    return 0;
}

/* ---- layout 2026-10-23 (session 5): drafts of the functions after 0x52a050 (ROUGH: absolute-address macros for the globals not declared above) ---- */
#define SV_I(a) (*(int *)(a))
#define SV_U(a) (*(unsigned *)(a))
#define SV_S(a) (*(short *)(a))
#define SV_US(a) (*(unsigned short *)(a))
#define SV_B(a) (*(unsigned char *)(a))
#define SV_C(a) (*(char **)(a))
#define SV_W(i) (d2_Objs + (i) * 0x2ec)            /* widget i */
#define SV_WI(i, o) (*(int *)(SV_W(i) + (o)))
#define SV_WS(i, o) (*(short *)(SV_W(i) + (o)))
#define SV_WB(i, o) (*(char *)(SV_W(i) + (o)))

void d2_RemoveObj();                     /* 0x46e710 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */
int d2_CreateWinText(short x, short y, char *text, int win);                     /* 0x473650 */
int d2_CreateScreenText(short x, short y, char *text);                     /* 0x473860 */
int d2_CreatePicObj();                     /* 0x46cd80 */
void d2_MoveObj(short x, short y, int obj);                     /* 0x46cda0 */
void d2_DrawText(int x, int y, void *surf, char *text, unsigned char flags);                     /* 0x46f000 */
void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags);                     /* 0x46f1a0 */
void d2_AddDirty();                     /* 0x468eb0 */
int d2_ShpBlit();                     /* 0x47bd70 */
void d2_SetClipRect(int x0, int y0, int x1, int y1);                     /* 0x47cb40 */
void * d2_SetFont(int font);                     /* 0x476030 */
void d2_EnableObject();                     /* 0x475aa0 */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */
int gm_Season(char *d);                     /* 0x4fe330, f3gm.h */
int gm_ClockSeason(void *t);           /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
int ls_GetLanguage();                     /* 0x5190a0 */
char * gm_PlayerSeat(int p);                     /* 0x504a80 */
int gm_SumListAmounts(void *list, unsigned char p);                     /* 0x509e30 */
int gm_ConvertMoney(int a, unsigned char k);                     /* 0x5085b0 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */

/* 0x522f10, the city / time panel (the big panel at the top of the screen), only while the player's character
   is in state 6: blits the saved view shape back; a & 1 rebuilds the widgets (the shield, the money and name
   texts, the clock / compass pictures, the help captions from the text list), then every call draws the date
   ("<season> A.D.<year>"), the sun and hour hands, the time under the pointer, the compass (camera angle) or
   the groundplan, the player's name, number and city arms, the money (the research class's list or the slots
   of the player), re-opens the shield when its picture changed and moves the side panel by 0x625718
   @deep read against the original and fixed (the date: gm_Season takes one argument and the year is
   sprintf's, the 12-byte calendar record; UpdateGroundplan gets b; the help-text polarity; FindPic fails
   with -1; short CreatePicObj x; three 0x100 buffers; the unfolded float constants of the clock and
   compass; side panel threshold 3 and the MoveObj argument order; GetPic before the pushes): frame and
   instruction count equal (1233 vs 1231). Left: the side-panel MoveObj block 0x523e98: the original hoists
   only push 0 of SetForm(0x6faa60, 0) before je (its branches load 0x6faa60 into eax / ecx) where ours
   hoists both pushes, and it reads d2_Wins[d2_LastWindow] as parent [ecx*4+0xf8f38c] plus y through lea
   eax,[ecx*4+d2_Wins] / [eax+6] where ours shl eax,2 and uses [eax+disp] twice; tried (&d2_Wins[i])->y,
   same. Allocation / address CSE. Readability still needs the d2.h head (SV_W widget macros, the 0x6238xx
   widget ids) */
void sv_UpdateCityPanel(unsigned char a, int b)
{
    struct {                            /* the calendar record gm_ClockToDate fills */
        unsigned char day, season;
        short year;
        unsigned char hour, min;
        char sec[4], pad[2];
    } t;
    char buf[0x100], name[0x100], arms[0x100];
    void *shape;
    int pic;
    int h, n, who;

    if (SV_B(((unsigned)&ai_Chars + 0x2) + ai_PlayerId * 0x21c) != 6)
        return;
    /* 0x522f42: the grabbed view shape back onto the screen at the view rectangle. The skeleton called
       d2_ShpBlit(0, 0, 0), so the shape was whatever the stack held (a click on "Weiter" then crashed in
       d2_ShpBlit with esi = 1); the original pushes sv_ViewRectLeft, sv_ViewRectTop, d2_ScreenSurf,
       ai_Unk6f22c0. */
    if (SV_I((unsigned)&ai_Unk6f22c0) != 0)
        d2_ShpBlit(sv_ViewRectLeft, sv_ViewRectTop, (struct D2SURF *)d2_ScreenSurf, (struct D2SHP *)ai_Unk6f22c0);
    if (a & 1) {
        if (ai_Unk623820 != -1) { d2_RemoveObj(ai_Unk623820); ai_Unk623820 = -1; }
        if (ai_Unk623824 != -1) { d2_RemoveObj(ai_Unk623824); ai_Unk623824 = -1; }
        if (ai_Unk623828 != -1) { d2_RemoveObj(ai_Unk623828); ai_Unk623828 = -1; }
        if (ai_Unk623848 != -1) { d2_RemoveObj(ai_Unk623848); ai_Unk623848 = -1; }
        if (ai_Unk62384c != -1) { d2_RemoveObj(ai_Unk62384c); ai_Unk62384c = -1; }
        if (ai_Unk62382c != -1) { d2_RemoveObj(ai_Unk62382c); ai_Unk62382c = -1; }
        if (ai_Unk623830 != -1) { d2_RemoveObj(ai_Unk623830); ai_Unk623830 = -1; }
        if (ai_Unk623834 != -1) { d2_RemoveObj(ai_Unk623834); ai_Unk623834 = -1; }
        if (ai_Unk623838 != -1) { d2_RemoveObj(ai_Unk623838); ai_Unk623838 = -1; }
        if (ai_Unk62383c != -1) { d2_RemoveObj(ai_Unk62383c); ai_Unk62383c = -1; }
        ai_Unk623820 = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), ai_Unk623840);
        SV_WI(ai_Unk623820, 0x1c0) = 0x43;
        SV_WI(ai_Unk623820, 0x4c) = 1;
        ai_Unk623824 = d2_WinOpenPic(0x23a, 0xd, 0x522, ai_Unk623840);
        SV_WI(ai_Unk623824, 0x1c0) = 0x43;
        SV_WI(ai_Unk623824, 0x4c) = 1;
        ai_Unk623848 = d2_CreateWinText(0x39, 0xd, ch_EmptyName, ai_Unk623840);
        SV_WI(ai_Unk623848, 0x1c0) = 0x43;
        SV_WI(ai_Unk623848, 0x4c) = 1;
        SV_WS(ai_Unk623848, 0x74) = 0x55;
        ai_Unk623830 = d2_CreatePicObjS(d2_ScreenW - 0x8d, d2_ScreenH - 0x73, 0x583);
        SV_WI(ai_Unk623830, 0x1c0) = 0x43;
        SV_WI(ai_Unk623830, 0x4c) = 1;
        ai_Unk623834 = d2_CreatePicObjS(d2_ScreenW - 0x57, d2_ScreenH - 0x4f, 0x7d9);
        SV_WI(ai_Unk623834, 0x1c0) = 3;
        SV_WI(ai_Unk623834, 0x4c) = 1;
        ai_Unk62382c = d2_CreatePicObjS(d2_ScreenW - 0x2a, d2_ScreenH - 0x66, 0x584);
        SV_WI(ai_Unk62382c, 0x1c0) = 0x43;
        SV_WI(ai_Unk62382c, 0x4c) = 1;
        ai_Unk623838 = d2_CreatePicObjS(d2_ScreenW - 0x4a, 0xd6, 0x585);
        SV_WI(ai_Unk623838, 0x1c0) = 0x43;
        SV_WI(ai_Unk623838, 0x4c) = 1;
        ai_Unk62383c = d2_CreatePicObjS(d2_ScreenW - 0x48, 0xd7, 0x551);
        SV_WI(ai_Unk62383c, 0x1c0) = 0x43;
        SV_WI(ai_Unk62383c, 0x4c) = 1;
        SV_WI(ai_Unk62383c, 0x18) = 8;
        ai_Unk62384c = d2_CreateScreenText(d2_ScreenW - 0x6a, d2_ScreenH - 0x7e, ch_EmptyName);
        SV_WI(ai_Unk62384c, 0x5c) = 1;
        SV_WS(ai_Unk62384c, 0x74) = 0x54;
        SV_WS(ai_Unk62384c, 0x14) = 0x6b;
        SV_WS(ai_Unk62384c, 0x16) = 0x20;
    } else {
        /* the texts of the six plates come from the text lists 0x1441ec8 .. 0x1441ee0 (or are cleared) */
        if (SV_B((unsigned)&ai_OptionPanelHelp) != 0) {
            if (ai_Unk623820 != -1) strcpy(SV_W(ai_Unk623820) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d4)));
            if (ai_Unk623824 != -1) strcpy(SV_W(ai_Unk623824) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9e0)));
            if (ai_Unk623848 != -1) strcpy(SV_W(ai_Unk623848) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d0)));
            if (ai_Unk623830 != -1) strcpy(SV_W(ai_Unk623830) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9cc)));
            if (ai_Unk62382c != -1) strcpy(SV_W(ai_Unk62382c) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9c8)));
            if (ai_Unk62383c != -1) strcpy(SV_W(ai_Unk62383c) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d8)));
        } else {
            if (ai_Unk623820 != -1) SV_WB(ai_Unk623820, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623824 != -1) SV_WB(ai_Unk623824, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623848 != -1) SV_WB(ai_Unk623848, 0xbc) = ch_EmptyName[0];
            if (ai_Unk623830 != -1) SV_WB(ai_Unk623830, 0xbc) = ch_EmptyName[0];
            if (ai_Unk62382c != -1) SV_WB(ai_Unk62382c, 0xbc) = ch_EmptyName[0];
            if (ai_Unk62383c != -1) SV_WB(ai_Unk62383c, 0xbc) = ch_EmptyName[0];
        }
        sv_UpdateGroundplan(b);
        d2_SetFont(0x55);
        gm_ClockToDate((struct GCLOCK *)&ai_Time, (unsigned char *)&t);
        if (ls_GetLanguage() == 2) {
            sprintf(buf, "%s A.D.%i", d2_Strings[0x4c + gm_Season((char *)&t)], t.year);
            d2_DrawText(d2_Wins[ai_Unk623840].x + 0x181, 0x17, d2_ScreenSurf, buf, 0);
        } else {
            sprintf(buf, "%s A.D.%i", d2_Strings[0x4c + gm_Season((char *)&t)], t.year);
            d2_DrawText(d2_Wins[ai_Unk623840].x + 0x195, 0x17, d2_ScreenSurf, buf, 0);
        }
        h = (int)((SV_I(((unsigned)&ai_Time + 0x6)) % 0x3c) * 0.8);
        shape = d2_GetPic(0x581);
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, shape, h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        h = (int)((float)((SV_US((unsigned)&ai_TimeHour) * 0x3c + SV_I(((unsigned)&ai_Time + 0x6))) * 4.0) / 60.0) % 0x30;
        shape = d2_GetPic(0x582);
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, shape, h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        if (SV_I((unsigned)&d2_HitUnderPointer) == ai_Unk623850) {
            sprintf(buf, "%02i:%02i", SV_US((unsigned)&ai_TimeHour), SV_I(((unsigned)&ai_Time + 0x6)));
            d2_SetFont(0x54);
            d2_DrawTextBox(d2_ScreenW - 0x50, 2, 0x2e, 0x14, d2_ScreenSurf, buf, 0x28);
            d2_AddDirty(d2_ScreenW - 0x50, 2, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
            d2_SetFont(0x55);
        }
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&ai_PlayerBuilding) == 0 && ai_Unk62383c != -1) {
            if (ai_Unk623838 != -1) {
                d2_EnableObject(ai_Unk623838, 0);
                d2_EnableObject(ai_Unk62383c, 1);
                h = (int)((float)fmod(d3_Camera->rotation.y, 6.2831853f) * 0.15915494f * 48.0 + 48.5) % 0x30;
                SV_WI(ai_Unk62383c, 0x78) = h;
            } else {
                d2_EnableObject(ai_Unk62383c, 0);
            }
        } else {
            if (ai_Unk623838 != -1)
                d2_EnableObject(ai_Unk623838, 1);
            if (ai_Unk62383c != -1)
                d2_EnableObject(ai_Unk62383c, 0);
        }
        d2_SetFont(0x55);
        if (ai_Unk623848 != -1) {
            io_CharacterName(ai_PlayerId, 1, 2, name);
            strcpy(buf, name);
            d2_SetObjText(ai_Unk623848, buf);
        }
        sprintf(buf, "%i", SV_I(((unsigned)&ai_Selected + 0xc) + ai_PlayerId * 0x21c));
        d2_DrawText(d2_Wins[ai_Unk623840].x + 0x23a, 0x17, d2_ScreenSurf, buf, 0);
        if (ai_Unk623824 != -1)
            d2_MoveObj(d2_Wins[ai_Unk623840].x + SV_S((unsigned)&d2_TextDrawnW) + 0x23d, 0x18, ai_Unk623824);
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0) {
            if (ai_Unk623828 == -1) {
                strcpy(name, (char *)(unsigned)&ai_PlayerSlots + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
                _strupr(name);
                sprintf(arms, "_STADTWAPPEN_%s", name);
                pic = d2_FindPic(arms);
                if (pic != -1)
                    ai_Unk623828 = d2_CreatePicObjS(d2_ScreenW - 0x6e, 0x5b, pic);
                else
                    ai_Unk623828 = d2_CreatePicObjS(d2_ScreenW - 0x6e, 0x5b, 0x5b1);
                SV_WI(ai_Unk623828, 0x1c0) = 0x43;
                SV_WI(ai_Unk623828, 0x4c) = 1;
            }
            if (SV_B((unsigned)&ai_OptionPanelHelp) != 0) {
                if (ai_Unk623828 != -1) strcpy(SV_W(ai_Unk623828) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9dc)));
            } else if (ai_Unk623828 != -1)
                SV_WB(ai_Unk623828, 0xbc) = ch_EmptyName[0];
            d2_SetFont(0x56);
            strcpy(name, (char *)((unsigned)&ai_PlayerSlots + 0x20) + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
            d2_DrawTextBox(d2_ScreenW - 0x70, 0x5d, 0x68, 0x14, d2_ScreenSurf, name, 8);
            d2_AddDirty(d2_ScreenW - 0x70, 0x5d, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        } else if (ai_Unk623828 != -1) {
            d2_RemoveObj(ai_Unk623828);
            ai_Unk623828 = -1;
        }
        d2_SetFont(0x54);
        if (SV_I((unsigned)&sv_ResearchClass) == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_B(SV_I((unsigned)&ai_FallbackBuilding)) == 0x47)
                who = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x65);
            else
                who = SV_B((unsigned)&ai_OwnPlayer);
            n = gm_SumSlotAmounts(SV_I(((unsigned)&ai_Unk770cec + 0xc) + ai_PlayerId * 0x21c), who) + SV_I((unsigned)&full_data_6f22dc);
        } else {
            n = ((int (*)(void *))gm_SumListAmounts)(*(int *)(gm_PlayerSeat(SV_I((unsigned)&sv_ResearchClass)) + 0x14)) + SV_I((unsigned)&full_data_6f22dc);
            who = SV_B((unsigned)&ai_OwnPlayer);
        }
        if (ai_Unk62384c != -1) {
            if (n != 0) {
                h = gm_ConvertMoney(n, who);
                txt_Format(buf, "%I~ %w", (int)(float)h, who);   /* 0x523d17: the second argument (esi) is
                                                                    the same owner index, for the %w city name */
                d2_SetObjText(ai_Unk62384c, buf);
            } else
                d2_SetObjText(ai_Unk62384c, SV_C(((unsigned)&ai_MessageFilterTexts + 0x8c0)));
        }
    }
    if (ai_Unk623820 != -1 && SV_WI(ai_Unk623820, 8) != SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c)) {
        d2_RemoveObj(ai_Unk623820);
        ai_Unk623820 = -1;
        ai_Unk623820 = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), ai_Unk623840);
        SV_WI(ai_Unk623820, 0x1c0) = 3;
        SV_WI(ai_Unk623820, 0x4c) = 1;
    }
    if (SV_I((unsigned)&full_data_62571c) >= 0) {
        if (SV_I((unsigned)&full_data_625718) > 3) {
            d2_EnableObject(ai_Unk6238a4, 1);
        } else {
            d2_EnableObject(ai_Unk6238a4, 0);
            SV_I((unsigned)&full_data_6f2364) = 0;
        }
        if (d2_ClickedId != -1 && d2_PointerObj == ai_Unk6238a4)
            SV_I((unsigned)&full_data_6f2364) = SV_I((unsigned)&full_data_6f2364) == 0;
        if (SV_I((unsigned)&full_data_623e90) != SV_I((unsigned)&full_data_625718) || (d2_ClickedId != -1 && d2_PointerObj == ai_Unk6238a4)) {
            if (SV_I((unsigned)&full_data_6f2364) != 0) {
                d2_SetForm(SV_I((unsigned)&full_data_6faa60), 0);
                d2_MoveObj(d2_ScreenW - (SV_I((unsigned)&full_data_625718) << 5) - 0x1d, d2_Wins[d2_LastWindow].y, d2_Wins[d2_LastWindow].parent);
                d2_MoveObj(d2_ScreenW - (SV_I((unsigned)&full_data_625718) << 5) - 0x4d, d2_ScreenH - 0x21, ai_Unk6238a4);
            } else {
                d2_SetForm(SV_I((unsigned)&full_data_6faa60), 0);
                d2_MoveObj(d2_ScreenW - 0x7d, d2_Wins[d2_LastWindow].y, d2_Wins[d2_LastWindow].parent);
                d2_MoveObj(d2_ScreenW - 0xad, d2_ScreenH - 0x21, ai_Unk6238a4);
            }
            SV_I((unsigned)&full_data_623e90) = SV_I((unsigned)&full_data_625718);
        }
        if (SV_I((unsigned)&full_data_6f2364) != 0) {
            d2_SetClipRect(0, 0, d2_ScreenW - 0x8c, d2_ScreenH);
            shape = d2_GetPic(sv_PanelPic + 3);
            d2_DrawShape(d2_ScreenW - (SV_I((unsigned)&full_data_625718) << 5) - 0x37, d2_ScreenH - 0x37, d2_ScreenSurf, shape, 0);
            d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        }
    }
}

unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
void ch_SetVisible();                     /* 0x402480 */

/* 0x52adb0, resets the selection (0x75ec5c / 0x75ec60, 0x6f2304) and clears the "selected" bit 2 of the group members (class 0x1d records) of the object under the cursor */
void sv_Unk52adb0(void)
{
    int o;
    unsigned short *p;

    SV_I((unsigned)&ai_Unk75ec5c) = 0;
    SV_I((unsigned)&sv_SelectedObject) = 0;
    SV_I((unsigned)&full_data_76631c) = 0;
    SV_I((unsigned)&sv_SelectedModel) = 0;
    o = SV_I((unsigned)&ai_FallbackBuilding);
    if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        if (o == 0) {
            o = gm_BuildQuery(1, 0, 0x44);
            if (o == 0)
                return;
        }
    } else if (o == 0) {
        SV_I((unsigned)&sv_SelectedModel) = 0;
        SV_I((unsigned)&ai_Unk75ec5c) = 0;
        SV_I((unsigned)&sv_SelectedObject) = 0;
        SV_I((unsigned)&full_data_76631c) = 0;
        return;
    }
    SV_B((unsigned)&full_data_6f2338) = 0;
    p = (unsigned short *)((char *(*)())gm_Query)(*(int *)(o + 0x5d), 1, 5);
    while (p != 0) {
        if (ai_Goods[*p * 0x41] == 0x1d)
            *(unsigned *)(p + 0x10) &= 0xfffffffd;
        p = (unsigned short *)gm_QueryNext();
    }
}
/* 0x52ae40, clears the group selection: the focus 0x7662c0, 0x6f2304, the selected flag (+0x21c stride: +0x188 of the character records at 0x770d08) and 0x6f2334 */

void sv_ClearSelection(void)
{
    char *q;

    q = (char *)(unsigned)&ai_Selected;
    SV_I((unsigned)&sv_SelectedChar) = 0;
    SV_I((unsigned)&sv_SelectedModel) = 0;
    do {
        *q = 0;
        q += 0x21c;
    } while ((int)q < (int)((unsigned)&ai_Selected + 0x65400));
    SV_I((unsigned)&ai_Unk6f2334) = 0;
}
/* 0x52b2a0, collects the (up to 0x20) characters 0x770d04 + n * 0x21c that belong to the group b (b == 0: the ones with no group, +0x2c == -1) into the table 0x7603a0 */

void sv_CollectGroupCharacters(char *b)
{
    int i, *p, *q;

    p = (int *)(unsigned)&ai_SceneChars;
    for (i = 0x20; i != 0; i--)
        *p++ = 0;
    if (b == 0) {
        p = (int *)(unsigned)&ai_SceneChars;
        q = (int *)((unsigned)&ai_Unk770cec + 0x18);
        do {
            if ((int)p > ((unsigned)&ai_SceneChars + 0x7b))
                return;
            if (*q != 0 && *(int *)(*q + 0x2c) == -1)
                *p++ = (int)(q - 0x61);
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182));
    } else {
        p = (int *)(unsigned)&ai_SceneChars;
        q = (int *)((unsigned)&ai_Unk770cec + 0x18);
        while ((int)p < ((unsigned)&ai_SceneChars + 0x7c)) {
            i = *q;
            if (i != 0 && *(int *)(i + 0x2c) == *(int *)(b + 1)) {
                *p++ = (int)(q - 0x61);
                if ((int)p > ((unsigned)&ai_SceneChars + 0x20))
                    ch_SetVisible(i, 0);
            }
            q += 0x87;
            if ((int)q > ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x181))
                return;
        }
    }
}
/* 0x52b350, adds a record (a, name[0x10], c, d, e) to the 0x20-byte table 0x75ec80 (0x20 entries; a == 0 marks a free entry); returns 1 or 0 when full */

int sv_Unk52b350(int a, char *name, int c, int d, int e)
{
    int i;
    int *p;

    i = 0;
    p = (int *)(unsigned)&full_data_75ec80;
    do {
        if (*p == 0) {
            if (name != 0)
                strcpy((char *)((unsigned)&full_data_75ec84 + i * 0x20), name);
            ((int *)(unsigned)&full_data_75ec80)[i * 8] = a;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 5] = c;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 6] = d;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 7] = e;
            return 1;
        }
        p += 8;
        i++;
    } while ((int)p < (unsigned)((unsigned)full_data_75ec80 + 0x600));
    return 0;
}
/* 0x52b3c0, finds the 0x75ec80 record of the character kind +0x180 of c (when c is not flagged 0x1000000, +0x16c == b, and the record is not time-limited or the day time (0x770ae4)
   lies in the window 0x617830 / 0x617840 of the current hour slot) and copies its name to out; returns 1 or 0 */

int sv_Unk52b3c0(char *c, int b, char *out)
{
    int h, i;
    unsigned *p, u;

    h = gm_ClockSeason((unsigned)&ai_Time);
    i = 0;
    if (SV_I((unsigned)&full_data_75ec80) != 0) {
        p = (unsigned *)(unsigned)&full_data_75ec80;
        u = SV_U((unsigned)&full_data_75ec80);
        while (u != *(unsigned char *)(c + 0x180)) {
            u = p[8];
            p += 8;
            i++;
            if (u == 0)
                return 0;
        }
        if ((*(unsigned *)(c + 0x1cc) & ((unsigned)&txt_Keys + 0x181c0)) == 0 && *(int *)(c + 0x16c) == b
            && (((int *)(unsigned)&full_data_75ec80)[i * 8 + 5] == 0
                || ((float)SV_US((unsigned)&ai_TimeHour) < *(float *)((unsigned)&gm_DayEndHours + h * 4) && *(float *)((unsigned)&gm_DayStartHours + h * 4) <= (float)SV_US((unsigned)&ai_TimeHour)))) {
            strcpy(out, (char *)((unsigned)&full_data_75ec84 + i * 0x20));
            return 1;
        }
    }
    return 0;
}
/* 0x52b490, creates the "sp_<id>" object of a character record a (+4 id, +0x184 the character) from the template b and links it back (+0xc94 / +0x28) */

int sv_Unk52b490(char *a, int b)
{
    int o;
    char buf[0x80];

    sprintf(buf, "sp_%i", *(int *)(a + 4));
    o = (int)evt_Load(b);
    if (o != 0) {
        evt_RunScriptArgs((EVTSCRIPT *)o, 1, buf);
        *(int *)(o + 0xc94) = *(int *)(a + 0x184);
        *(int *)(*(int *)(a + 0x184) + 0x28) = *(int *)(o + 0x100);
    }
    return o;
}

void d2_SetObjEnabled();                     /* 0x475e40 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
int ai_AmountMenu();                     /* 0x5c1130 */
int gm_BuildingClass(int kind);                /* 0x501d80, f3gm.c, the class of a building kind */
int ai_TransportWindow();                     /* 0x5af1d0 */
int ai_Unk5b8fc0();                     /* 0x5b8fc0 */
int ai_Unk5b84f0();                     /* 0x5b84f0 */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
void d2_FreeAnim(int n);                     /* 0x47ce30 */
int d3t_PlaceAtObject();                     /* 0x4118a0 */
void snd_PlayWorkerClick(struct G16CHARCLICK *c);                     /* 0x4c2c70 */

/* 0x52a050, the per-frame refresh of the info panel: the right mouse button / the 0x2000 key flag act on the object under the cursor (selection, jump to a contact), then the panel
   is rebuilt when the shown object / group / item changed (0x6f23bc .. 0x6f23d8 hold the last shown state); ROUGH */
void sv_UpdatePanel(void)
{
    unsigned char *o, *p;
    unsigned fl;
    int c, k, f, v;
    int st[0x14];
    int *q;

    if (SV_I((unsigned)&sv_PlayerBarForm) != -1)
        return;
    if (SV_B((unsigned)&ai_GameState) == 0)
        return;
    o = (unsigned char *)SV_I((unsigned)&sv_SelectedObject);
    if (o == 0 || gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a) != 0 || gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x116) != 0)
        f = 1;
    else
        f = 0;
    d2_SetObjEnabled(SV_I((unsigned)&ai_Unk62388c), f);
    p = o;
    fl = SV_U((unsigned)&sv_FrameMask) & 0x2000;
    if (fl == 0) {
l260:
        if (d2_ClickedId != -1) {
            if (SV_I((unsigned)&ai_Unk62388c) == -1 || d2_PointerObj != SV_I((unsigned)&ai_Unk62388c) || fl == 0) {
                if (SV_I((unsigned)&ai_Unk623888) != -1 && d2_PointerObj == SV_I((unsigned)&ai_Unk623888) && fl != 0 && ai_Unk5763c0(0x80) == 1) {
                    q = st;
                    for (c = 0x14; c != 0; c--)
                        *q++ = 0;
                    st[1] = 0;
                    st[5] = 4;
                    ai_Unk576040(0xf, 0, 0, 0);
                    ai_AmountMenu(st, ch_EmptyName, ch_EmptyName, sv_ReturnZero);
                    ai_Unk576040(0xf, 0, 1, 0);
                }
            } else if (SV_I((unsigned)&sv_SelectedObject) != 0 && *(unsigned char *)SV_I((unsigned)&sv_SelectedObject) != 0x47) {
                c = gm_BuildingClass(*(unsigned char *)SV_I((unsigned)&sv_SelectedObject));
                if ((c == 1 || c == 2 || c == 4 || c == 7 || c == 8 || (k = ai_BTypes[*p * 0x24d]) == 0x10 || k == 0x1e) && ai_Unk5763c0(0x400) == 1) {
                    SV_I((unsigned)&ai_BuildingPerms) = gm_PermissionBits(ai_PlayerId, p, 0, 0);
                    ai_Unk576040(0x12, p, 0, 0);
                    ai_TransportWindow(p, 1);
                    ai_Unk576040(0x12, p, 1, 0);
                    SV_I((unsigned)&sv_SelectedObject) = (int)p;
                    sv_ResetScreenClip();
                }
            }
        }
    } else if (d2_ClickedId != -1) {
        if (SV_I((unsigned)&ai_Unk623894) == -1 || d2_PointerObj != SV_I((unsigned)&ai_Unk623894))
            goto l260;
        p = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
        if (p == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
                p = (unsigned char *)SV_I((unsigned)&sv_SelectedObject);
                if (p == 0)
                    goto l3b8;
                goto l188;
            }
            p = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
            if (SV_I((unsigned)&sv_PanelShown) == 0)
                goto l188;
            v = gm_Query(*(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d), 1, 0, *(unsigned short *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x29));
            SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, 0, v, 0);
        } else {
l188:
            SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, p, 0, 0);
        }
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 || (SV_I((unsigned)&full_data_6f2320) & 0x10) == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
l1f2:
                if (SV_I((unsigned)&sv_SelectedObject) != 0) {
                    c = gm_BuildingClass(*(unsigned char *)SV_I((unsigned)&sv_SelectedObject));
                    SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, SV_I((unsigned)&sv_SelectedObject), 0, 0);
                    if ((c == 3 || c == 5) && (SV_I((unsigned)&full_data_6f2320) & 0x10) != 0)
                        ai_Unk5b8fc0(SV_I((unsigned)&sv_SelectedObject));
                    else
                        ai_Unk5b84f0(SV_I((unsigned)&sv_SelectedObject));
                }
            } else if (SV_I((unsigned)&sv_PanelShown) == 0) {
                ai_Unk5b84f0(SV_I((unsigned)&ai_FallbackBuilding));
            } else {
                if ((SV_I((unsigned)&full_data_6f2320) & 0x10) == 0)
                    goto l1f2;
                ai_TechTreeWindow(SV_I((unsigned)&ai_FallbackBuilding), *(unsigned short *)SV_I((unsigned)&sv_PanelShown));
            }
        } else {
            ai_TechTreeWindow(SV_I((unsigned)&ai_PlayerBuilding), *(unsigned short *)(SV_I((unsigned)&ai_PlayerBuilding) + 0x29));
        }
    }
l3b8:
    sv_Unk529380();
    if (SV_I((unsigned)&sv_SelectedObject) == SV_I((unsigned)&full_data_6f23c4) && SV_I((unsigned)&ai_FallbackBuilding) == SV_I((unsigned)&full_data_6f23c0) && SV_I((unsigned)&ai_PlayerBuilding) == SV_I((unsigned)&full_data_6f23bc)
        && SV_I((unsigned)&sv_PanelShown) == SV_I((unsigned)&full_data_6f23c8) && SV_I((unsigned)&ai_Unk75ec5c) == SV_I((unsigned)&full_data_6f23cc) && SV_I((unsigned)&sv_SelectedChar) == SV_I((unsigned)&full_data_6f23d0)
        && SV_B((unsigned)&full_data_6f2338) == SV_I((unsigned)&full_data_6f23d4) && SV_I((unsigned)&ai_Unk6f2334) == SV_I((unsigned)&full_data_6f23d8) && SV_I((unsigned)&ai_Unk6237cc) == 0)
        return;
    SV_I((unsigned)&ai_Unk6237cc) = 0;
    sv_Unk529fb0();
    if (SV_I((unsigned)&ai_Unk75ec5c) == 0 || ai_Goods[*(unsigned short *)SV_I((unsigned)&ai_Unk75ec5c) * 0x41] != 0x1d || SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_I((unsigned)&sv_SelectedChar) != 0) {
        if (SV_I((unsigned)&sv_SelectedModel) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
            if (SV_I((unsigned)&ai_Unk6f2334) == 0) {
                if (SV_I((unsigned)&sv_PanelShown) == 0 && SV_I((unsigned)&sv_SelectedChar) == 0)
                    sv_Unk528ee0();
                goto l510;
            }
        } else if (SV_I((unsigned)&ai_Unk6f2334) == 0)
            goto l510;
        if (SV_I((unsigned)&sv_SelectedChar) != SV_I((unsigned)&full_data_6f23d0) || SV_I((unsigned)&full_data_6f23d8) != SV_I((unsigned)&ai_Unk6f2334)) {
            if (SV_I((unsigned)&ai_Unk6f2334) < 2) {
                sv_Unk5290f0(SV_I((unsigned)&sv_SelectedChar));
                SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&sv_SelectedChar);
                sv_Unk529380();
            } else
                sv_Unk5290f0(0);
        }
    } else
        sv_Unk528d90(SV_I((unsigned)&ai_Unk75ec5c));
l510:
    SV_I((unsigned)&full_data_6f23d4) = SV_B((unsigned)&full_data_6f2338);
    SV_I((unsigned)&full_data_6f23c0) = SV_I((unsigned)&ai_FallbackBuilding);
    SV_I((unsigned)&full_data_6f23bc) = SV_I((unsigned)&ai_PlayerBuilding);
    SV_I((unsigned)&full_data_6f23d8) = SV_I((unsigned)&ai_Unk6f2334);
    if (SV_I((unsigned)&sv_SelectedModel) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        SV_I((unsigned)&full_data_6f23c0) = 0;
        SV_I((unsigned)&full_data_6f23bc) = 0;
        SV_I((unsigned)&full_data_6f2294) = 0;
    } else {
        SV_I((unsigned)&full_data_6f23c8) = SV_I((unsigned)&sv_PanelShown);
        SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&sv_SelectedChar);
        SV_I((unsigned)&full_data_6f23cc) = SV_I((unsigned)&ai_Unk75ec5c);
        SV_I((unsigned)&full_data_6f23c4) = SV_I((unsigned)&sv_SelectedObject);
        if ((SV_I((unsigned)&ai_Unk75ec5c) == 0 && SV_I((unsigned)&sv_PanelShown) == 0) || SV_I((unsigned)&sv_PanelForm) != -1) {
            if (SV_I((unsigned)&sv_SelectedObject) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0) {
                SV_I((unsigned)&full_data_6f2294) = 0;
                return;
            }
            SV_I((unsigned)&full_data_6f23c0) = SV_I((unsigned)&ai_FallbackBuilding);
            SV_I((unsigned)&full_data_6f23bc) = SV_I((unsigned)&ai_PlayerBuilding);
            SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&sv_SelectedModel);
            p = (unsigned char *)SV_I((unsigned)&sv_SelectedObject);
            if (p == 0 && (p = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0)
                p = 0;
            sv_Unk528860(p);
            sv_Unk529380();
            return;
        }
        SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&sv_SelectedModel);
        if (SV_I((unsigned)&ai_Unk75ec5c) != 0) {
            SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&sv_SelectedModel);
            sv_Unk528b80(SV_I((unsigned)&ai_Unk75ec5c));
            sv_Unk529380();
            return;
        }
        if (SV_I((unsigned)&sv_PanelShown) != 0) {
            sv_Unk528b80(SV_I((unsigned)&sv_PanelShown));
            sv_Unk529380();
            return;
        }
    }
    SV_I((unsigned)&full_data_6f23c4) = SV_I((unsigned)&sv_SelectedObject);
    SV_I((unsigned)&full_data_6f23c8) = SV_I((unsigned)&sv_PanelShown);
    SV_I((unsigned)&full_data_6f23cc) = SV_I((unsigned)&ai_Unk75ec5c);
    SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&sv_SelectedChar);
    SV_I((unsigned)&full_data_6f23d8) = SV_I((unsigned)&ai_Unk6f2334);
}

struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);   /* 0x448630 */

/* 0x52a680, finds the object under the mouse cursor (0x448630 at the cursor position, kind 0x20 then 0x40) and builds the hint text 0xf8aae0 for it: the object kind +0x4b is 1 (item
   of a building, 0x6f22f0), 2 (a building, 0x6f22f4) or 3 (a character, 0x6f22f8); contact gates ("contact_gate_*") resolve to the contact item; ROUGH */
void sv_Unk52a680(void)
{
    char *o, *s, *t;
    unsigned char *b;
    unsigned short *it;
    int i, k, msg, lastarg;
    char buf[0x60];

    SV_I((unsigned)&ai_Unk6f22e8) = 0;
    if (SV_US((unsigned)&d2_PointerLook) != 0 && SV_US((unsigned)&d2_PointerLook) < 9) {
        SV_I((unsigned)&ai_Unk6f22e8) = 0;
        return;
    }
    SV_I((unsigned)&full_data_623e98) = SV_S((unsigned)&dm_L);
    SV_I((unsigned)&full_data_623e9c) = SV_S(((unsigned)&dm_L + 0x2));
    if (SV_I((unsigned)&full_data_623e98) <= SV_I((unsigned)&sv_ViewRectLeft) || SV_I((unsigned)&sv_ViewRectRight) <= SV_I((unsigned)&full_data_623e98) || SV_I((unsigned)&full_data_623e9c) <= SV_I((unsigned)&sv_ViewRectTop)
        || SV_I((unsigned)&sv_ViewRectBottom) <= SV_I((unsigned)&full_data_623e9c) || d2_HitWindow != -1 || SV_I((unsigned)&d2_HitUnderPointer) != -1
        || ((SV_C((unsigned)&ai_Unk6f22e8) = d3if_Pick((float)SV_I((unsigned)&full_data_623e98), (float)SV_I((unsigned)&full_data_623e9c), 0x20, 1)) == 0
            && (SV_C((unsigned)&ai_Unk6f22e8) = d3if_Pick((float)SV_S((unsigned)&dm_L), (float)SV_S(((unsigned)&dm_L + 0x2)), 0x40, 1)) == 0)) {
        if (d2_PointerObj == -1 || (SV_B(SV_I((unsigned)&d2_Objs) + 0x1c0 + d2_PointerObj * 0x2ec) & 0x40) == 0)
            SV_B((unsigned)&ai_Unkf8aae0) = 0;
        if (SV_I((unsigned)&d2_PointerSprite) != -1) {
            d2_FreeAnim(SV_I((unsigned)&d2_PointerSprite));
            SV_I((unsigned)&d2_PointerSprite) = -1;
            SV_I((unsigned)&full_data_6f23dc) = -1;
        }
        SV_I((unsigned)&full_data_6f1c4c) = 0;
        SV_I((unsigned)&full_data_6f22f0) = 0;
        SV_I((unsigned)&full_data_6f22f4) = 0;
        SV_I((unsigned)&ai_Unk6f22f8) = 0;
        SV_US((unsigned)&d2_PointerLook) = 0;
        return;
    }
    if ((SV_B((unsigned)&sv_FrameMask) & 0x10) == 0)
        return;
    if (SV_B((unsigned)&full_data_6ed9c8) != 0)
        return;
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (o == 0)
        return;
    if (d2_HitWindow != -1)
        return;
    if (SV_I((unsigned)&d2_HitUnderPointer) != -1)
        return;
    SV_I((unsigned)&full_data_6f2320) = 0;
    SV_I((unsigned)&full_data_6f2328) = 0;
    SV_I((unsigned)&full_data_6f2324) = 0;
    SV_I((unsigned)&full_data_6f232c) = 0;
    SV_US((unsigned)&d2_PointerLook) = 0;
    SV_I((unsigned)&full_data_6f22f0) = 0;
    SV_I((unsigned)&full_data_6f22f4) = 0;
    SV_I((unsigned)&ai_Unk6f22f8) = 0;
    SV_I((unsigned)&full_data_6f2300) = 0;
    k = o[0x4b];
    if (k == 1) {
        SV_I((unsigned)&full_data_6f22f0) = *(int *)(o + 0x200);
        b = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
        if (b == 0)
            b = 0;
        SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, b, SV_I((unsigned)&full_data_6f22f0), 0);
        if (SV_I((unsigned)&full_data_6f22f0) != 0) {
            i = *(unsigned short *)SV_I((unsigned)&full_data_6f22f0);
            k = ai_Goods[i * 0x41];
            if ((k == 6 || k == 2) && (SV_I((unsigned)&full_data_6f2320) & 0x4000) != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", SV_I((unsigned)&txt_ObjectNames + i * 8), SV_I((unsigned)&full_data_143a3cc));
                SV_US((unsigned)&d2_PointerLook) = 0xb;
            } else {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s", SV_I((unsigned)&txt_ObjectNames + i * 8));
                SV_US((unsigned)&d2_PointerLook) = 0x16;
            }
        }
        SV_I((unsigned)&full_data_6f2328) = SV_I((unsigned)&full_data_6f2320);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
    } else if (k == 2) {
        SV_I((unsigned)&full_data_6f22f4) = *(int *)(o + 0x200);
        SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, SV_I((unsigned)&full_data_6f22f4), 0, 0);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
        if (b != 0 && (b[0x5a] & 1) == 0) {
            if (b[5] != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s<", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5);
                SV_I((unsigned)&full_data_6f2324) = SV_I((unsigned)&full_data_6f2320);
                b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
                goto la9a;
            }
            strcpy((char *)(unsigned)&ai_Unkf8aae0, (char *)SV_I((unsigned)&full_data_143b3f8 + *b * 0x38));
        }
        SV_I((unsigned)&full_data_6f2324) = SV_I((unsigned)&full_data_6f2320);
    } else if (k == 3) {
        SV_I((unsigned)&ai_Unk6f22f8) = *(int *)(o + 0x200);
        SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, 0, 0, SV_I((unsigned)&ai_Unk6f22f8));
        if (SV_I((unsigned)&ai_Unk6f22f8) != 0) {
            io_CharacterName(*(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8), 1, 4, (unsigned)&ai_Unkf8aae0);
            if (*(char *)(SV_I((unsigned)&ai_Unk6f22f8) + 0x165) == 0x1c && (*(unsigned char *)(SV_I((unsigned)&ai_Unk6f22f8) + 0x1b4) & 1) != 0 && (SV_I((unsigned)&full_data_6f2320) & 0x800) != 0
                && SV_I((unsigned)&d3_CurrentScene) != 0) {
                txt_Format((char *)(unsigned)&ai_Unkf8aae0, 0x414c, *(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8));
                SV_US((unsigned)&d2_PointerLook) = 0x2e;
            }
        }
        SV_I((unsigned)&full_data_6f232c) = SV_I((unsigned)&full_data_6f2320);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
    } else {
        SV_B((unsigned)&ai_Unkf8aae0) = 0;
        b = 0;
    }
la9a:
    if (SV_I((unsigned)&sv_SelectedObject) != 0 && (int)b == SV_I((unsigned)&sv_SelectedObject) && (SV_I((unsigned)&full_data_6f2324) & 1) != 0 && (b[0x5a] & 1) == 0)
        SV_US((unsigned)&d2_PointerLook) = 0xb;
    b = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
    if ((b != 0 || (b = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding)) != 0) && SV_C((unsigned)&ai_Unk6f22e8) != 0 && strncmp(SV_C((unsigned)&ai_Unk6f22e8), "contact_gate_", 0xd) == 0) {
        strcpy(buf, SV_C((unsigned)&ai_Unk6f22e8) + 0x10);
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
            msg = *(int *)(b + 0x5d);
            lastarg = 2;
        } else {
            msg = *(int *)(b + 0x5d);
            lastarg = 6;
        }
        it = (unsigned short *)gm_Query(msg, 1, 4, lastarg);
        while (it != 0) {
            if (_strcmpi(buf, ai_Goods + *it * 0x41 + 1) == 0)
                goto lb9e;
            it = (unsigned short *)gm_QueryNext();
        }
        it = (unsigned short *)SV_I((unsigned)&full_data_6f2300);
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0)
            it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
lb9e:
        SV_I((unsigned)&full_data_6f2300) = (int)it;
        if (SV_I((unsigned)&full_data_6f2300) != 0) {
            SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, b, SV_I((unsigned)&full_data_6f2300), 0);
            if ((SV_I((unsigned)&full_data_6f2320) & 1) == 0) {
                SV_I((unsigned)&full_data_6f2300) = 0;
            } else {
                SV_US((unsigned)&d2_PointerLook) = 0xb;
                if (*(unsigned short *)SV_I((unsigned)&full_data_6f2300) == 0xfd) {
                    t = (char *)SV_I((unsigned)&full_data_143a3d0);
                    if (SV_I((unsigned)&sv_PanelShown) == 0)
                        msg = SV_I((unsigned)&full_data_143b3f8 + *b * 0x38);
                    else
                        msg = SV_I((unsigned)&txt_ObjectNames + *(unsigned short *)SV_I((unsigned)&sv_PanelShown) * 8);
                } else {
                    msg = SV_I((unsigned)&txt_ObjectNames + *(unsigned short *)SV_I((unsigned)&full_data_6f2300) * 8);
                    t = (char *)SV_I((unsigned)&full_data_143a3cc);
                }
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", msg, t);
            }
        }
    }
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (memcmp(o, "tp_TUER", 8) == 0 && (b = *(unsigned char **)(o + 0x200)) != 0) {
        if ((SV_I((unsigned)&full_data_6f2320) & 1) == 0) {
            if (b[5] == 0) {
                strcpy((char *)(unsigned)&ai_Unkf8aae0, (char *)SV_I((unsigned)&full_data_143b3f8 + *b * 0x38));
                goto lad73;
            }
            sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s<", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5);
        } else {
            SV_US((unsigned)&d2_PointerLook) = 0xb;
            if (b[5] != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s< %s", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5, SV_I((unsigned)&full_data_143a3cc));
                o = SV_C((unsigned)&ai_Unk6f22e8);
                goto lad73;
            }
            sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), SV_I((unsigned)&full_data_143a3cc));
        }
        o = SV_C((unsigned)&ai_Unk6f22e8);
    } else {
        s = (char *)((unsigned)&sv_NamePlates + 0x48);
        for (i = 0x20; i != 0; i--) {
            if (*(char **)(s - 0x48) == o) {
                strcpy((char *)(unsigned)&ai_Unkf8aae0, s);
                SV_US((unsigned)&d2_PointerLook) = *(unsigned short *)(s - 4);
            }
            s += 200;
        }
    }
lad73:
    if (SV_I((unsigned)&d2_PointerSprite) != -1) {
        d2_FreeAnim(SV_I((unsigned)&d2_PointerSprite));
        SV_I((unsigned)&d2_PointerSprite) = -1;
        SV_I((unsigned)&full_data_6f23dc) = 0;
        o = SV_C((unsigned)&ai_Unk6f22e8);
    }
    SV_C((unsigned)&full_data_6f1c4c) = o;
}
/* 0x52ae70, "sv_HandleObjects": turns the object under the cursor (0x6f22e8, found by 0x52a680) into the selection state: the quick-jump contact name 0x75e8d4 (panel flag 0x75e8c0) is looked
   up as an object when the panel shows the remembered subject, the hint objects 0x760458 .. 0x760460, the highlight (0x4118a0, 0x6f22ec) and the group selection (0x7662c0 / 0x6f2334) are
   set; ROUGH */

void sv_Unk52ae70(void)
{
    unsigned short *g;
    char *o, *cls, *s, *t;
    int subj, n, found;
    char msg[0x80];

    found = 0;
    cls = 0;
    SV_I((unsigned)&full_data_760458) = 0;
    SV_I((unsigned)&full_data_76045c) = 0;
    SV_I((unsigned)&ai_ClickedObject) = 0;
    SV_I((unsigned)&full_data_760460) = 0;
    SV_I((unsigned)&full_data_6f22fc) = 0;
    if (SV_I((unsigned)&sv_SelectedChar) != 0 && *(char *)(SV_I((unsigned)&sv_SelectedChar) + 0x188) == 0)
        SV_I((unsigned)&sv_SelectedChar) = 0;
    if (SV_I((unsigned)&sv_PanelFlag) != 0) {
        subj = SV_I((unsigned)&ai_PlayerBuilding);
        if (subj == 0)
            subj = SV_I((unsigned)&ai_FallbackBuilding);
        if (subj == SV_I((unsigned)&sv_PanelObject)) {
            if (SV_I((unsigned)&sv_PanelShown) == SV_I((unsigned)&sv_PanelRecord)
                || (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) == 0 && SV_I((unsigned)&sv_PanelRecord) != 0 && *(short *)SV_I((unsigned)&sv_PanelRecord) == 0xfd)) {
                if (SV_B((unsigned)&sv_PanelText) != 0) {
                    o = (char *)d3_FindObject(0, 0x60, 0, (unsigned)&sv_PanelText);
                    if (o != 0) {
                        found = 1;
                        SV_I((unsigned)&sv_PanelFlag) = 0;
                        SV_C((unsigned)&ai_Unk6f22e8) = o;
                        goto lf73;
                    }
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", (char *)(unsigned)&sv_PanelText);
                }
                SV_I((unsigned)&sv_PanelFlag) = 0;
            }
lf73:
            if (SV_I((unsigned)&sv_PanelRecord) == 0 || *(short *)SV_I((unsigned)&sv_PanelRecord) == 0xfd) {
                if (SV_B((unsigned)&sv_PanelText) != 0) {
                    o = (char *)d3_FindObject(0, 0x60, 0, (unsigned)&sv_PanelText);
                    if (o != 0) {
                        found = 1;
                        SV_I((unsigned)&sv_PanelFlag) = 0;
                        SV_C((unsigned)&ai_Unk6f22e8) = o;
                        goto lfe4;
                    }
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", (char *)(unsigned)&sv_PanelText);
                }
                SV_I((unsigned)&sv_PanelFlag) = 0;
            }
        }
    }
lfe4:
    if ((SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d3t_CamMove) == 0 && SV_S((unsigned)&dm_L) > SV_I((unsigned)&sv_ViewRectLeft) && SV_S((unsigned)&dm_L) < SV_I((unsigned)&sv_ViewRectRight)
         && SV_I((unsigned)&sv_ViewRectTop) < SV_S(((unsigned)&dm_L + 0x2)) && SV_S(((unsigned)&dm_L + 0x2)) < SV_I((unsigned)&sv_ViewRectBottom) && d2_HitWindow == -1 && SV_I((unsigned)&d2_HitUnderPointer) == -1)
        || found) {
        if (SV_C((unsigned)&ai_Unk6f22e8) == 0) {
            if (SV_B((unsigned)&full_data_6f2338) == 0) {
                sv_Unk52adb0();
                return;
            }
        } else {
            SV_I((unsigned)&full_data_760458) = SV_I((unsigned)&full_data_6f22f4);
            SV_I((unsigned)&full_data_76045c) = SV_I((unsigned)&full_data_6f22f0);
            SV_I((unsigned)&full_data_760460) = SV_I((unsigned)&ai_Unk6f22f8);
            SV_I((unsigned)&full_data_6f22fc) = SV_I((unsigned)&full_data_6f2300);
            if (SV_I((unsigned)&full_data_6f22f0) != 0)
                cls = ai_Goods + *(unsigned short *)SV_I((unsigned)&full_data_6f22f0) * 0x41;
            if (memcmp(SV_C((unsigned)&ai_Unk6f22e8), "tp_TUER", 8) == 0 || (cls != 0 && *cls == 6) || SV_I((unsigned)&full_data_6f2300) != 0)
                found = 1;
            else
                found = 0;
            SV_C((unsigned)&ai_ClickedObject) = SV_C((unsigned)&ai_Unk6f22e8);
            if ((SV_C((unsigned)&ai_Unk6f22e8)[0x211] & 1) != 0 && !found) {
                d3t_PlaceAtObject(SV_C((unsigned)&ai_Unk6f22e8), SV_I((unsigned)&ai_CameraStep));
                SV_C((unsigned)&full_data_6f22ec) = SV_C((unsigned)&ai_Unk6f22e8);
                if (SV_C((unsigned)&ai_Unk6f22e8) != 0 && (n = *(int *)(SV_C((unsigned)&ai_Unk6f22e8) + 0x1d4)) != 0 && *(int *)(n + 0x108) != 0)
                    ((int (*)())*(int *)(n + 0x108))(SV_C((unsigned)&ai_Unk6f22e8), 2, n + 0x10c, 0);
                SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth);
            }
            SV_C((unsigned)&full_data_6f23e0) = SV_C((unsigned)&ai_Unk6f22e8);
            if (SV_I((unsigned)&ai_Unk6f22f8) != 0 && *(char *)(SV_I((unsigned)&ai_Unk6f22f8) + 8) != 0) {
                g = (unsigned short *)SV_I((unsigned)&ai_Unk6f22f8);
                n = gm_PermissionBits(ai_PlayerId, 0, 0, g);
                sv_ClearSelection();
                if ((n & 0x800) != 0 && (*(char *)((char *)g + 0x165) != 0x1c || (g[0xda] & 1) == 0)) {
                    SV_I((unsigned)&sv_SelectedChar) = (int)g;
                    SV_B((unsigned)&ai_Selected + *g * 0x21c) = 1;
                    SV_I((unsigned)&ai_Unk6f2334) = 1;
                    SV_I((unsigned)&ai_Unk631078) = SV_I(((unsigned)&ai_Unk770cec + 0x18) + *g * 0x21c);
                    snd_PlayWorkerClick(g);
                }
                SV_I((unsigned)&ai_Unk631078) = SV_I(((unsigned)&ai_Unk770cec + 0x18) + *(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8) * 0x21c);
            }
            if (SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0 && SV_I((unsigned)&full_data_6f22f4) != 0 && SV_I((unsigned)&ai_Unk75ec5c) == 0)
                SV_I((unsigned)&sv_SelectedModel) = SV_I((unsigned)&ai_Unk6f22e8);
            SV_I((unsigned)&sv_SelectedObject) = SV_I((unsigned)&full_data_6f22f4);
            if (SV_I((unsigned)&full_data_6f22f4) != 0)
                SV_I((unsigned)&sv_SelectedModel) = SV_I((unsigned)&ai_Unk6f22e8);
            SV_I((unsigned)&ai_Unk75ec5c) = SV_I((unsigned)&full_data_6f22f0);
            if (SV_I((unsigned)&full_data_6f22f0) != 0) {
                SV_I((unsigned)&sv_SelectedModel) = SV_I((unsigned)&ai_Unk6f22e8);
                if (ai_Goods[*(unsigned short *)SV_I((unsigned)&full_data_6f22f0) * 0x41] == 0x1d)
                    sv_ClearSelection();
            }
        }
    }
}

int init_BuildingKindName();                     /* 0x50dba0 */
struct VFSENTRY * vfs_FindFile(char *path, struct VFSDIR *d, struct VFSDIR **found);                     /* 0x42f8f0 */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */

/* 0x52b500, keeps the "extras" (script objects "sp_<id>") of the characters of the group of the building a (the table 0x7603a0 = the characters of 0x52b2a0): a character gets the object of its role (names "bewohner",
   "besucher", "meister", "pause", "faulenzen" or the one from the 0x75ec80 table of 0x52b3c0), created from the script "<scripts>locations\<class>\<name><n>_<class>.esc" (n = the first free number among the
   objects already using the name) and linked back (+0xc94 / +0x28); a character whose role ended loses the object (0x429e90); ROUGH */
void sv_Unk52b500(unsigned char *a)
{
    char *cls, *kind, *c, *g, *w, *o4, *r;
    short *it;
    int h, i, ok, k, n, d, v, len, *p;
    char name[0x20], cn[0x60], path[0x80], sp[0x80];
    int used[8];

    cls = 0;
    it = (short *)SV_I((unsigned)&sv_PanelShown);
    if (it != 0 && *it != 0xfd)
        cls = ai_Goods + *(unsigned short *)it * 0x41;
    kind = ai_BTypes + *a * 0x24d;
    if (SV_I((unsigned)&full_data_6237c8) + 500 < SV_I((unsigned)&ai_ClockMs)) {
        if (ai_Unk5fb5b0 != 0 && (k = *(int *)(ai_Unk5fb5b0 + 0x3c8)) != 0 && *(int *)(k + 0x318) != 0)
            ((int (*)())*(int *)(k + 0x318))(ai_Unk5fb5b0, 6, k + 0x31c, 0);
        SV_I((unsigned)&full_data_6237c8) = SV_I((unsigned)&ai_ClockMs);
    }
    h = gm_ClockSeason((unsigned)&ai_Time);
    for (i = 0; i < 0x20; i++) {
        c = (char *)SV_I((unsigned)&ai_SceneChars + i * 4);
        if (c == 0 || (g = *(char **)(c + 0x184)) == 0)
            continue;
        if (*(int *)(g + 0x2c) != *(int *)(a + 1) || *(char *)(c + 8) == 0 || *(char **)(g + 0x88) != ai_Unk5fb5b0)
            continue;
        ok = 0;
        name[0] = 0;
        if (sv_Unk52b3c0(c, a, name) == 0) {
            k = *kind;
            if (k == 0x16 && it != 0 && *it == 0x120 && *(int *)(g + 0x30) == *(int *)((char *)it + 2) && (*(unsigned *)(c + 0x1c8) & 0x1000) == 0) {
                strcpy(name, "besucher");
                ok = 1;
            } else if (k == 1 && *(unsigned char **)(c + 0x170) == a && (*(unsigned *)(c + 0x1c8) & 0x1000) == 0) {
                strcpy(name, "bewohner");
                ok = 1;
            } else if (*(char *)(c + 0x180) == 0 && *(unsigned char **)(c + 0x16c) == a && *(int *)(g + 0x28) == -1) {
                if (*(char *)(c + 0x164) == 0) {
                    if (*(char *)(c + 0x165) == 0x1c && (*(unsigned char *)(c + 0x1b4) & 1) != 0)
                        strcpy(name, "faulenzen");
                    else
                        strcpy(name, "pause");
                } else
                    strcpy(name, "meister");
                ok = 1;
            }
        } else {
            k = *kind;
            if (k == 0xd) {
                if (it != 0) {
                    if (*it == 0x37 && *(int *)(g + 0x30) == *(int *)((char *)it + 2)) {
                        ok = 1;
                        goto l7f0;
                    }
                    if (*it != 0xfd || *(int *)(g + 0x30) != *(int *)((char *)it + 2))
                        goto l7f0;
                }
                w = *(char **)(c + 0x17c);
                ok = 1;
                if (w != 0 && *(int *)(w + 200) != 0)
                    ok = 0;
            } else {
                if (k == 8 && *it == 0xdd && *(char *)(c + 0x180) == 0x15) {
                    ok = 1;
                    goto l7f0;
                }
                w = *(char **)(c + 0x17c);
                ok = 1;
                if (w != 0 && *w == 4 && *(int *)(w + 200) != 0)
                    ok = 0;
            }
        }
l7f0:
        if (*(char *)(c + 0x165) == 0x1c && (*(unsigned char *)(c + 0x1b4) & 1) != 0 && *(int *)(g + 0x28) == -1) {
            strcpy(name, "faulenzen");
        } else if (!ok) {
            goto lsecond;
        }
        if (name[0] != 0) {
            if (*(int *)(g + 0x28) == -1) {
                for (n = 0; n < 8; n++)
                    used[n] = 1;
                for (p = (int *)(unsigned)&ai_SceneChars; (int)p < (unsigned)((unsigned)ai_SceneChars + 0x80); p++) {
                    if (*p != 0 && (w = *(char **)(*p + 0x184)) != 0 && (v = *(int *)(w + 0x28)) != -1
                        && (o4 = (char *)evt_FindById(v)) != 0 && (r = (char *)strstr(o4, name)) != 0) {
                        len = strlen(name);
                        d = r[len] - '0';
                        if (d < 0 || d > 9)
                            break;
                        used[d] = 0;
                    }
                }
                n = 0;
                do {
                    if (used[n] != 0)
                        break;
                    n++;
                } while (n < 8);
                if (*(char *)(c + 0x180) == 0x16)
                    strcpy(cn, "Training_Allgemein");
                else if (cls == 0)
                    init_BuildingKindName(*a, cn);
                else
                    strcpy(cn, cls + 1);
                sprintf(path, "%slocations\\%s\\%s%i_%s.esc", evt_Path, cn, name, n, cn);
                if (vfs_FindFile(path, SV_I((unsigned)&d3_Unk6333fc), &d) == 0)
                    sprintf(path, "%slocations\\%s\\%s0_%s.esc", evt_Path, cn, name, cn);
                if (vfs_FindFile(path, SV_I((unsigned)&d3_Unk6333fc), &d) != 0) {
                    sprintf(sp, "sp_%i", *(int *)(c + 4));
                    o4 = (char *)evt_LoadScript(path);
                    if (o4 != 0) {
                        evt_RunScriptArgs((EVTSCRIPT *)o4, 1, sp);
                        *(char **)(o4 + 0xc94) = g;
                        *(int *)(g + 0x28) = *(int *)(o4 + 0x100);
                    }
                }
            }
            continue;
        }
lsecond:
        ok = 0;
        if (*(unsigned char **)(c + 0x16c) == a && g != 0 && (v = *(int *)(g + 0x28)) != -1) {
            if (*(char *)(c + 0x180) == 0 || *(float *)((unsigned)&gm_DayEndHours + h * 4) < (float)SV_US((unsigned)&ai_TimeHour))
                ok = 1;
            w = *(char **)(c + 0x17c);
            if ((w != 0 && *w == 4 && *(int *)(w + 200) != 0) || ok) {
                ok = 0;
                o4 = (char *)evt_FindById(v);
                if (o4 != 0) {
                    w = (char *)(unsigned)&full_data_75ec84;
                    do {
                        if (ok)
                            goto lfree;
                        if (*(int *)(w - 4) != 0 && *w != 0 && strstr(o4, w) != 0)
                            ok = 1;
                        w += 0x20;
                    } while ((int)w < ((unsigned)((unsigned)full_data_75ec84 + 0x5fc) + 0x4));
                    if (ok)
lfree:
                        evt_Stop((EVTSCRIPT *)o4);
                }
            }
        }
    }
}
/* 0x52bb70, runs the key handler 0x5309a0 (0, keys & ~0xc0 | 0x100200) for as long as the object id (0x4284a0) is flagged +0x124 & 1 */

void sv_Unk52bb70(int id)
{
    unsigned fl;
    char *o;

    fl = (SV_U((unsigned)&sv_FrameMask) & 0xffffff3f) | 0x100200;
    o = (int)evt_FindById(id);
    while (o != 0 && (*(unsigned char *)(o + 0x124) & 1) != 0) {
        sv_RunFrame(0, fl);
        o = (int)evt_FindById(id);
    }
}
/* 0x52bbc0, moves the selected characters (the table 0x770cec, stride 0x21c, the selected flag byte at +0x1c of the entry) into the group (object kind 'C') of the first one's object: the first selected character fixes
   the object (+0x10) and the owner (+0); the entries that belong to the current group 0x760458 are taken out (0x4ffe90 (+0x5d, 1, 0, 0x146) / 0x524260 / 0x4c5230 / 0x4c52e0), the others are collected (up to four ids)
   into a message ('C', +8 the player record, +0x36 = 2, +0x58 / +0x5c the two group handles, +0x70 the four ids) that 0x4c4b70 sends; the group slot list (+0xc4, 4 ints) loses the moved id; the selected flags are cleared
   (ROUGH) */

void sv_Unk52bbc0(void)
{
    char *obj, *o;
    int *rec, *out;
    int state, owner, cur, n, done, k, g, id;
    unsigned char m[0xf8];

    obj = 0;
    state = 0;
    owner = 0;
    n = 0;
    done = 0;
    cur = SV_I((unsigned)&full_data_760458);
    m[4] = 0;
    out = (int *)(m + 0x70);
    for (rec = (int *)(unsigned)&ai_Unk770cec; !done && (int)rec < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a); rec += 0x87) {
        if ((char)rec[7] == 0)
            continue;
        id = rec[-0x5a];
        if (state == 0) {
            o = (char *)rec[4];
            if (o != 0 && rec[0] != 0) {
                owner = rec[0];
                state = 0x43;
                obj = o;
                if (*o == 'C') {
                    if (owner == cur) {
                        g = gm_Query(*(int *)(cur + 0x5d), 1, 0, 0x146);
                        for (k = 0; k < 4; k++) {
                            if (*(int *)(o + 0xc4 + k * 4) == id) {
                                *(int *)(o + 0xc4 + k * 4) = -1;
                                break;
                            }
                        }
                        sv_MoveToGroup(SV_I((unsigned)&full_data_760458), 0, (short)rec[-0x5b], 0);
                        ai_Unk4c5230(id);
                        ai_Unk4c52e0(id, *(int *)(SV_I((unsigned)&full_data_760458) + 1), *(int *)(g + 2), 0, 0, 0);
                        cur = SV_I((unsigned)&full_data_760458);
                    } else {
                        m[4] = 'C';
                        m[0x36] = 2;
                        *(int *)(m + 8) = SV_I((unsigned)&ai_CharInts + SV_U((unsigned)&ai_PlayerId) * 0x21c);
                        *(int *)(m + 0x58) = *(int *)(cur + 1);
                        *(int *)(m + 0x5c) = *(int *)(owner + 1);
                        *out++ = id;
                        n++;
                        for (k = n; k < 4; k++)
                            *out++ = -1;
                        out = (int *)(m + 0x70) + n;
                        for (k = 0; k < 4; k++) {
                            if (*(int *)(o + 0xc4 + k * 4) == id) {
                                *(int *)(o + 0xc4 + k * 4) = -1;
                                cur = SV_I((unsigned)&full_data_760458);
                                break;
                            }
                        }
                    }
                }
            }
        } else if (state == 0x43 && rec[0] == owner && (char *)rec[4] == obj) {
            if (rec[0] == cur) {
                g = gm_Query(*(int *)(cur + 0x5d), 1, 0, 0x146);
                for (k = 0; k < 4; k++) {
                    if (*(int *)(obj + 0xc4 + k * 4) == id) {
                        *(int *)(obj + 0xc4 + k * 4) = -1;
                        break;
                    }
                }
                sv_MoveToGroup(SV_I((unsigned)&full_data_760458), 0, (short)rec[-0x5b], 0);
                ai_Unk4c5230(id);
                ai_Unk4c52e0(id, *(int *)(SV_I((unsigned)&full_data_760458) + 1), *(int *)(g + 2), 0, 0, 0);
                cur = SV_I((unsigned)&full_data_760458);
            } else {
                *out++ = id;
                n++;
                for (k = 0; k < 4; k++) {
                    if (*(int *)(obj + 0xc4 + k * 4) == id) {
                        *(int *)(obj + 0xc4 + k * 4) = -1;
                        cur = SV_I((unsigned)&full_data_760458);
                        break;
                    }
                }
                if (n > 3)
                    done = 1;
            }
        }
        *(char *)(rec + 7) = 0;
    }
    if (obj != 0)
        cm_SendExSetHE(*(int *)(obj + 0x70), obj);
    if (m[4] != 0)
        cm_SendLongMessage(m);
}
/* 0x52bec0, the mouse release of the selection (called every frame with 0x6f2334 counting the selected characters): with the selection mode 0x1467394 (button released 0x14673a4): the click without a pick sends
   the selected characters of the player (flag +0x188 / kind from 0x503530 & 0x800) to the clicked map cell (0x45f2b0 (map +0xb0 of the scene, x, y, &cx, &cy), the cell types 0 / 0xd are skipped over the nine
   neighbour offsets off[9], 0x4c5100 (id, cx, cy, 0x633444)); with a picked group 0x6f22fc the group members (0x7603a0, 0x10 slots) are sent to it (0x4c5230, 0x4c5460 (id, target +1, pick +1)); 0x52bbc0 moves
   the selection into the picked object when 0x760458 is set; then the selected flags are mirrored (0x75e920) and the focus / hilight of each (0x4134a0 (+0x34, flag), 0x476b90) updated (ROUGH: the offset
   table off[] is the neighbour spiral, the __ftol operands (off * 50.0 / cell size + c) are decoded from the disassembly) */

void sv_Unk52bec0(void)
{
    int off[9][2] = { {0, 0}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
    char *pk; int map, v, cx, cy, ix, iy, size, i, cell, t, tgt;
    int *p;
    char *pc;
    char name[0x80];
    unsigned short *ch;
    unsigned *pp;

    cx = -1;
    cy = -1;
    map = *(int *)(SV_I((unsigned)&ai_Unk5fb5b0) + 0xb0);
    v = SV_I((unsigned)&ai_PlayerBuilding);
    if (v == 0)
        v = SV_I((unsigned)&ai_FallbackBuilding);
    if (SV_I(((unsigned)&dm_L + 0x14)) == 0) {
        if (SV_I((unsigned)&ai_Unk760440) != 0) {
            snd_PlaySelectedClick();
            sv_Unk52eed0();
            if (SV_I(((unsigned)&dm_L + 0x14)) != 0)
                goto lrel;
        }
        goto lend;
    }
lrel:
    if (SV_I(((unsigned)&dm_L + 0x24)) == 0) {
        if (SV_I((unsigned)&ai_Unk760440) == 0 && SV_I((unsigned)&di_Absolute) != 0 && SV_I((unsigned)&d2_HitWindow) == -1 && v != 0 && *(short *)(v + 0x27) == (short)SV_I((unsigned)&ai_PlayerId))
            sv_Unk52ee50();
        goto lend;
    }
    if (SV_I((unsigned)&ai_Unk6f2334) == 0)
        goto lend;
    if (SV_I((unsigned)&full_data_6f22fc) == 0) {
        if (SV_I((unsigned)&ai_ClickedObject) != 0 || SV_I((unsigned)&full_data_76045c) != 0)
            goto lafter;
        if (SV_I((unsigned)&full_data_760458) == 0) {
            if (map == 0 || v == 0 || SV_I(((unsigned)&dm_L + 0xc)) == 0 || SV_I((unsigned)&d2_HitWindow) != -1)
                goto lend;
            i = 0;
            ch = (unsigned short *)(unsigned)&ai_Chars;
            do {
                cell = 0;
                if (*(char *)(ch + 0xc4) != 0 && *(char *)(ch + 4) != 0 && (gm_PermissionBits(SV_I((unsigned)&ai_PlayerId), 0, 0, ch) & 0x800) != 0 && *(int *)(ch + 0xc2) != 0) {
                    if (d3sm_Pick(map, (int)*(short *)(unsigned)&dm_L, (int)*(short *)((unsigned)&dm_L + 0x2), &cx, &cy) != 0) {
                        size = *(int *)(map + 0x20);
                        do {
                            ix = (int)((float)off[i][0] * (50.0 / *(float *)(map + 0x10)) + cx);
                            iy = (int)((float)off[i][1] * (50.0 / *(float *)(map + 0x18)) + cy);
                            if (ix < size && iy < size)
                                cell = *(int *)(*(int *)(map + 0x24) + (size * iy + ix) * 0x1c);
                            i++;
                            t = cx;
                            tgt = cy;
                        } while (i < 8 && (ix < 0 || size <= ix || iy < 0 || size <= iy || cell == 0 || (t = ix, tgt = iy, cell == 0xd)));
                        if (*(int *)(ch + 0xb6) != 0)
                            sv_MoveToGroup(*(int *)(ch + 0xb6), 0, *ch, 0);
                        if (*(int *)(*(int *)(ch + 0xc2) + 0x28) != -1 && (pk = (int)evt_FindById(*(int *)(*(int *)(ch + 0xc2) + 0x28))) != 0)
                            evt_Stop((EVTSCRIPT *)pk);
                        ai_Unk4c5100(*(int *)(ch + 2), t, tgt, SV_I((unsigned)&d3_CurrentScene));
                    }
                }
                ch += 0x10e;
            } while ((int)ch < (unsigned)((unsigned)ai_Chars + 0x65400));
            if (SV_I((unsigned)&full_data_6f22fc) != 0)
                goto lgroup;
            goto lafter;
        }
    } else {
lgroup:
        if (map != 0 && SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d2_HitWindow) == -1) {
            t = -1;
            sprintf(name, "dummy_%s", (char *)SV_I((unsigned)&ai_ClickedObject) + 8);
            if (d3_FindObject(SV_I((unsigned)&ai_ClickedObject), 0x100, 0, name) == 0)
                strcpy(name, (char *)SV_I((unsigned)&ai_ClickedObject));
            pp = (unsigned *)(unsigned)&ai_SceneChars;
            do {
                ch = (unsigned short *)*pp;
                v = 0;
                if (ch != 0 && *(char *)(ch + 0xc4) != 0) {
                    ai_Unk4c5230(*(int *)(ch + 2));
                    if (*(int *)(ch + 0xb6) != 0)
                        sv_MoveToGroup(*(int *)(ch + 0xb6), 0, *ch, 0);
                    if (**(short **)(unsigned)&full_data_6f22fc == 0xfd) {
                        if (SV_I((unsigned)&ai_FallbackBuilding) != 0) {
                            t = init_BuildingSceneUniverse(SV_I((unsigned)&ai_FallbackBuilding));
                            v = SV_I((unsigned)&ai_FallbackBuilding);
                            if (v == 0)
                                v = SV_I((unsigned)&ai_PlayerBuilding);
                        } else if (SV_I((unsigned)&ai_PlayerBuilding) != 0) {
                            t = 0;
                            v = SV_I((unsigned)&ai_PlayerBuilding);
                        }
                    } else {
                        t = init_ObjectSceneUniverse(SV_I((unsigned)&full_data_6f22fc));
                        v = SV_I((unsigned)&ai_FallbackBuilding);
                        if (v == 0)
                            v = SV_I((unsigned)&ai_PlayerBuilding);
                    }
                    if (-1 < t)
                        ai_Unk4c5460(*(int *)(ch + 2), *(int *)(v + 1), *(int *)(SV_I((unsigned)&full_data_6f22fc) + 1));
                }
                pp++;
            } while ((int)pp < (unsigned)((unsigned)ai_SceneChars + 0x80));
            sv_ClearSelection();
            SV_I((unsigned)&full_data_6f22fc) = 0;
        }
lafter:
        if (SV_I((unsigned)&full_data_760458) == 0)
            goto lend;
    }
    sv_Unk52bbc0();
lend:
    SV_I((unsigned)&ai_Unk6f2334) = 0;
    d2_ClearPointerItems();
    i = 0;
    p = (int *)((unsigned)&ai_Unk770cec + 0x18);
    do {
        cell = *p;
        if (cell != 0) {
            pc = (char *)p;
            if ((char)p[1] != *(char *)((unsigned)&sv_SelectionFlags + i) && (*(unsigned char *)(cell + 0x8c) & 0x40) == 0)
                d3t_SetGrey(*(int *)(cell + 0x34), (char)p[1] != 0);
            if ((char)p[1] != 0) {
                if (SV_I((unsigned)&ai_Unk6f2334) == 0)
                    SV_I((unsigned)&sv_SelectedChar) = (int)(p - 0x61);
                SV_I((unsigned)&ai_Unk6f2334) = SV_I((unsigned)&ai_Unk6f2334) + 1;
                if (p[-6] != 0 && *(int *)(p[-6] + 1) == *(int *)(*p + 0x2c) && (SV_I((unsigned)&ai_FallbackBuilding) != 0 || (SV_I((unsigned)&sv_PanelShown) != 0 && **(short **)(unsigned)&sv_PanelShown != 0x120)))
                    d2_AddPointerItem(p[2], 1);
            }
        }
        t = p[1];
        p += 0x87;
        *(char *)((unsigned)&sv_SelectionFlags + i) = (char)t;
        i++;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182));
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && SV_I((unsigned)&ai_Unk6f2334) != 0)
        sv_ClearSelection();
}
/* 0x52c420, for every group record (0x4ffe90 (0, 2, 7, 4, 0x1d)) sets bit 1 of +0x20 when some character (0x770cec stride 0x21c: kind 1, owner +0x1c matching, flag +0x48 & 1) belongs to it */

void sv_Unk52c420(void)
{
    char *g;
    int *p;

    g = (char *)gm_Query(0, 2, 7, 4, 0x1d);
    while (g != 0) {
        *(int *)(g + 0x20) = 0;
        p = (int *)(unsigned)&ai_Unk770cec;
        do {
            if ((unsigned short)p[-0x5b] != 0xffff && *(char *)((int)p - 0x16a) == 1 && *p != 0 && *(int *)(*p + 1) == *(int *)(g + 0x1c) && (*(unsigned char *)(p + 0x12) & 1) != 0)
                *(unsigned *)(g + 0x20) |= 1;
            p += 0x87;
        } while ((int)p < (int)((unsigned)&ai_Unk770cec + 0x65400));
        g = gm_QueryNext();
    }
}
/* 0x52c4a0, releases the speech bubble of the object o (the object field sv_BubbleObjects of its bubble record) */

void sv_ReleaseBubble(int o)
{
    int i;
    int *p;

    i = 0;
    p = sv_BubbleObjects;
    do {
        if (*p == o) {
            sv_BubbleObjects[i * 0x43] = 0;
            return;
        }
        p += 0x43;
        i++;
    } while ((int)p < (int)(sv_BubbleObjects + 0x40 * 0x43));
}
/* 0x52c4e0, registers the object o (it must belong to the current scene: +0x88 == 0x5fb5b0) with the value b and the optional name in the table 0x761fc0 (a free entry, else the oldest expired one,
   time 0x761fc4 < 0x6333cc ms); the object's +0x228 callback becomes 0x52c4a0 */

void sv_Unk52c4e0(int o, int b, char *name)
{
    int i, j, *p;
    int *e;

    if (o != 0 && *(char **)(o + 0x88) == ai_Unk5fb5b0) {
        p = (int *)(unsigned)&sv_BubbleObjects;
        do {
            if (*p == o)
                return;
            p += 0x43;
        } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
        i = 0;
        p = (int *)(unsigned)&sv_BubbleObjects;
        do {
            if (*p == 0) {
                if (i < 0x40)
                    goto lfound;
                break;
            }
            p += 0x43;
            i++;
        } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
        i = -1;
        j = 0;
        p = (int *)((unsigned)&sv_Bubbles + 0x4);
        do {
            if (*p < SV_I((unsigned)&ai_ClockMs))
                i = j;
            p += 0x43;
            j++;
        } while ((int)p < ((unsigned)&sv_Bubbles + 0x4304));
        if (i != -1) {
lfound:
            e = (int *)(i * 0x10c + (unsigned)&sv_Bubbles);
            ((int *)((unsigned)&sv_Bubbles + 0x4))[i * 0x43] = SV_I((unsigned)&ai_ClockMs);
            ((int *)(unsigned)&sv_BubbleObjects)[i * 0x43] = o;
            *e = b;
            *(int *)(o + 0x228) = (int)sv_ReleaseBubble;
            if (name != 0) {
                strcpy((char *)e + 0xc, name);
                return;
            }
            *(char *)(i * 0x10c + ((unsigned)&sv_BubbleObjects + 0x4)) = 0;
        }
    }
}

char d3if_ScreenBox(D3OBJECT *obj, D3RECT *r);                     /* 0x4486e0 */
/* 0x42ec00 */
struct CHARACTER * ch_FindByObject();                     /* 0x402ac0 */
int gm_TitleJobGroup();                     /* 0x504c00 */
int gm_TitleGroup();                     /* 0x504ad0 */
char * gm_FindBuildingById(int id);                     /* 0x502020 */
float unk_Unk4f9e00(int *o);                     /* 0x4f9e00 */
float ord_BuildingCapacityRatio();                     /* 0x4f97c0 */

/* 0x52c5c0, picks the speech bubble for the character under the cursor (0x6f22f8) or the object (0x6f22e8) and shows it through 0x52c4e0: a text id (0x11e9 + n * 0xf, or by the role +0x180 of the
   character: 0x1250 ..) or a text built into a buffer; ROUGH (the switch cases are read from the Ghidra output, the 5 kinds of the final switch are mostly lost) */
void sv_Unk52c5c0(void)
{
    char *cls, *g, *o, *b, *r;
    int v, id, k, bv;
    unsigned kind;
    unsigned short *q;
    char text[0x200];
    char tmp[0x28];

    g = SV_C((unsigned)&ai_Unk6f22f8);
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (g == 0) {
        if (o == 0)
            return;
        if (*(int *)(o + 0x220) != 2)
            return;
        if (SV_I((unsigned)&full_data_6f22f0) != 0)
            return;
    }
    if (SV_I(((unsigned)&dm_L + 0xc)) == 0)
        return;
    cls = 0;
    b = SV_C((unsigned)&ai_PlayerBuilding);
    if (b == 0)
        b = SV_C((unsigned)&ai_FallbackBuilding);
    if (b != 0)
        cls = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (g == 0 || (gm_PermissionBits(ai_PlayerId, 0, 0, g), (g = SV_C((unsigned)&ai_Unk6f22f8)) == 0)) {
        if (o != 0) {
            v = ch_FindByObject(o);
            goto l67e;
        }
    } else {
        v = *(int *)(g + 0x184);
l67e:
        if (v != 0 && (*(unsigned char *)(v + 4) & 1) == 0)
            return;
    }
    if (b != 0 && ((*cls != 0x16 || *(unsigned short *)SV_I((unsigned)&sv_PanelShown) != 0x120) && *cls != 0x0f))
        return;
    util_RandomU(4);
    text[0] = ch_EmptyName[0];
    memset(text + 1, 0, 0x1ff);
    id = -1;
    if (g == 0) {
        kind = (unsigned short)util_RandomU(4) + 1;
lcc95:
        if (kind < 5)
            goto lcc9e;
    } else {
        int plain = 1;

        if (*(char *)(g + 0x164) == 0) {
            if (*(char *)(g + 0x165) != 0) {
                v = gm_TitleJobGroup(*(char *)(g + 0x165));
                goto l712;
            }
l77c:
            kind = (unsigned short)util_RandomU(4) + 1;
        } else {
            v = gm_TitleGroup(*(char *)(g + 0x164));
l712:
            if (v != 0xb && v != 0xc)
                goto l77c;
            plain = 0;
            kind = (unsigned short)util_RandomU(4) + 1;
            if (kind == 3)
                kind = 4;
        }
        v = *(int *)(g + 0x17c);
        if (v == 0)
            goto lcc95;
        kind = 0xffffffff;
        switch (*(unsigned char *)(g + 0x180)) {
        case 0x14:
            if (*(char **)(g + 0x16c) != 0) {
                if (ai_BTypes[**(unsigned char **)(g + 0x16c) * 0x24d] == ' ')
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1286);
                else
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1256);
            }
            break;
        default:
            kind = (unsigned short)util_RandomU(4) + 1;
            if (plain || kind != 3)
                goto lcc95;
            kind = 4;
            goto lcc9e;
        case 0x18:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xc4));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1262);
            break;
        case 0x19:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x125c);
            break;
        case 0x1a:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xb4));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1277);
            break;
        case 0x28:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x124d);
            break;
        case 0x3c:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xac));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1253);
            break;
        case 0x3f:
        case 0x75:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1250);
            break;
        case 0x40:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0 && *(short *)(r + 0x27) != -1)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(short *)(r + 0x27), r) + 0x1265);
            break;
        case 0x43:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1268);
            break;
        case 0x44:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1259);
            break;
        case 0x48:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x126e);
            break;
        case 0x49:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x124a);
            break;
        case 0x61:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x126b);
            break;
        case 0x62:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x125f);
            break;
        case 100:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xb0));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1271);
            break;
        case 0x65:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1274);
            break;
        case 0x89:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xb0));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x127a);
            break;
        case 0x8c:
            if (*(char **)(g + 0x16c) != 0) {
                k = ai_BTypes[**(unsigned char **)(g + 0x16c) * 0x24d];
                if (k == 0x1d)
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1283);
                else if (k == 0x1f)
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1289);
            }
            break;
        case 0x8e:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x127d);
            break;
        case 0xa1:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1280);
        }
    }
    goto lcca;
lcc9e:
    id = kind * 0xf + 0x11e9;
lcca:
    if ((short)util_RandomU(2) == 0 && he_FindEvent(1, 0, 0x52) != 0)
        kind = 5;
    unk_Unk4f9e00(tmp);
    switch (kind) {
    case 2:
        ord_BuildingCapacityRatio();
        goto lftol;
    case 1:
    case 3:
    case 4:
lftol:
        id = (int)(0.0) + (unsigned short)util_RandomU(3);       /* the float operand of the first term is lost in the pseudocode */
        break;
    case 5:
        if (SV_I((unsigned)&hst_KeyNext) != -1) {
            id = 1;
            for (q = (unsigned short *)(((unsigned)&txt_GroupLevel + 0x47f0) + SV_I((unsigned)&hst_KeyNext) * 4); id < 5; id++, q = (unsigned short *)((char *)q + 4)) {
                if (**(char **)q == 0)
                    break;
            }
            id = ((unsigned short)util_RandomU(id - 1) - 5) + SV_I((unsigned)&hst_KeyNext);
        }
    default:
        break;
    }
    if (g == 0) {
        if (o != 0)
            sv_Unk52c4e0(ch_FindByObject(o, id, 0), 0, 0);
    } else {
        if (id != -1) {
            sv_Unk52c4e0(*(int *)(g + 0x184), id, 0);
            return;
        }
        if (text[0] != 0) {
            sv_Unk52c4e0(*(int *)(g + 0x184), 0, text);
            return;
        }
    }
}
/* 0x52cef0, ages the speech bubble table 0x761fc0 (entries older than 300 ms are freed) and draws the rest above their objects (0x4486e0 gives the screen box; text: the entry's own or the default line
   0x143a1c0[+ -8]) */

void sv_Unk52cef0(void)
{
    int *p, o, ob, t[3], x, y;
    char *s;

    p = (int *)(unsigned)&sv_BubbleObjects;
    do {
        if (*p != 0 && (unsigned)(p[-1] + 300) < SV_U((unsigned)&ai_ClockMs))
            *p = 0;
        p += 0x43;
    } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
    d2_SetFont(0x54);
    p = (int *)(unsigned)&sv_BubbleObjects;
    do {
        o = *p;
        if (o != 0 && *(char **)((ob = *(int *)(o + 0x34)) + 0x208) == ai_Unk5fb5b0
            && (*(int *)(ob + 0x218) != 1 || ((ob = *(int *)(o + 0x22c)) != 0 && *(int *)(ob + 0x218) != 1))
            && (char)d3if_ScreenBox(ob, t) != 0) {
            y = t[1] - 0x40;
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            s = (char *)(p + 1);
            if (*s == 0)
                s = (char *)SV_I((unsigned)&d2_Strings + p[-2] * 4);
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, s, 0x28);
            d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        p += 0x43;
    } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
}
/* 0x52d1e0, draws the name plates of the objects of the table 0x760640 (0x32 ints each, name at +0x48) above their screen boxes, then walks the scene objects of kind 0x20 with the callback 0x52d010 */

void sv_Unk52d1e0(void)
{
    int *p, o, t[3], x;

    d2_SetFont(0x54);
    p = (int *)(unsigned)&sv_NamePlates;
    do {
        o = *p;
        if (o != 0 && (*(int *)(o + 0x1cc) != 0 || *(int *)(o + 0x218) == 0)) {
            if ((char)d3if_ScreenBox(o, t) != 0) {
                x = (t[2] - t[0]) / 2 - 0x50 + t[0];
                d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, (char *)(p + 0x12), 0x28);
                d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
            }
        }
        p += 0x32;
    } while ((int)p < (unsigned)((unsigned)sv_NamePlates + 0x1900));
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0x20, sv_Unk52d010);
}
/* 0x52d2a0, draws the names of the items (0x4ffe90 lists of the shown building / group: the not-person items first, then the persons (class 6 / 2)) above their screen boxes */

void sv_Unk52d2a0(void)
{
    int sub, t[3], x;
    unsigned short *q;
    char c;

    if (SV_I((unsigned)&sv_PanelShown) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
            return;
        q = (unsigned short *)gm_Query(*(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d), 1, 0, 0xfd);
        if (q == 0)
            sub = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d);
        else
            sub = *(int *)((char *)q + 0x14);
    } else
        sub = *(int *)(SV_I((unsigned)&sv_PanelShown) + 0x14 * 2);
    d2_SetFont(0x54);
    q = (unsigned short *)((char *(*)())gm_Query)(sub, 1, 5);
    while (q != 0) {
        if (*(int *)(q + 0xc) != 0 && (c = ai_Goods[*q * 0x41]) != 6 && c != 2 && (char)d3if_ScreenBox(*(int *)(q + 0xc), t) != 0) {
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, SV_I((unsigned)&txt_ObjectNames + *q * 8), 0x28);
            d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        q = (unsigned short *)gm_QueryNext();
    }
    if (SV_I((unsigned)&sv_PanelShown) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sub = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d);
    } else
        sub = *(int *)(SV_I((unsigned)&sv_PanelShown) + 0x14 * 2);
    q = (unsigned short *)gm_Query(sub, 2, 5, 6);
    while (q != 0) {
        if (*(int *)(q + 0xc) != 0 && ((c = ai_Goods[*q * 0x41]) == 6 || c == 2) && (char)d3if_ScreenBox(*(int *)(q + 0xc), t) != 0) {
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, SV_I((unsigned)&txt_ObjectNames + *q * 8), 0x28);
            d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        q = (unsigned short *)gm_QueryNext();
    }
}
/* 0x52d010, the callback of 0x52d1e0 over the scene objects of kind 0x20: for a "gate" part of the shown building (0x6f230c / 0x6f2308) it finds the item of that name (the contact gates "contact_gate_<name>")
   in the building's lists (or the 0xfd item) and draws its name above the part's screen box; a missed start inside 0x52cef0 for Ghidra; ROUGH */

void sv_Unk52d010(char *part)
{
    char *b;
    unsigned short *it;
    char name[0x60], buf[0x80];
    int t[3], x, y, lst;

    b = SV_C((unsigned)&ai_FallbackBuilding);
    if (b == 0) {
        b = SV_C((unsigned)&ai_PlayerBuilding);
        if (b == 0)
            return;
    }
    if (strstr(part, (char *)(unsigned)"gate") == 0)
        return;
    strcpy(name, part + 0x10);
    lst = SV_I((unsigned)&ai_FallbackBuilding) != 0 ? 6 : 2;
    it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 4, lst);
    while (it != 0) {
        if (_strcmpi(name, ai_Goods + *it * 0x41 + 1) == 0)
            goto lfound;
        it = (unsigned short *)gm_QueryNext();
    }
    if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
        return;
    it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
lfound:
    if (it == 0)
        return;
    if (*it == 0xfd) {
        if (SV_I((unsigned)&sv_PanelShown) != 0)
            sprintf(buf, "%s %s", SV_I((unsigned)&txt_ObjectNames + *(unsigned short *)SV_I((unsigned)&sv_PanelShown) * 8), SV_I((unsigned)&full_data_143a3d0));
        else
            sprintf(buf, "%s %s", SV_I((unsigned)&full_data_143b3f8 + *(unsigned char *)b * 0x38), SV_I((unsigned)&full_data_143a3d0));
    } else
        sprintf(buf, "%s %s", SV_I((unsigned)&txt_ObjectNames + *it * 8), SV_I((unsigned)&full_data_143a3cc));
    if ((char)d3if_ScreenBox(part, t) != 0) {
        x = (t[2] - t[0]) / 2 - 0x50 + t[0];
        y = t[1];
        d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, buf, 0x28);
        d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
    }
}

struct SBSAMPLE * sb_PlaySample();                     /* 0x426fa0 */
int ai_ScriptQuery();                     /* 0x576340 */
int ai_Unk575860();                     /* 0x575860 */
void ai_Unk591170(void);                     /* 0x591170 */
void anm_Update(void);                     /* 0x4bb860 */
void sv_ReleaseNamePlates(void);                /* 0x52e190 */
void gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */

/* 0x52d480, with the key flag 8 set and the camera rotated away from the stored views (0x5ebba4 zero vector, tolerance 0.2): draws the arrows (picture 0x52c, frames 0 .. 7) at the screen border that show
   the way back to the object's view: the pairs 0xfc2dec +0xe4 / +0xfc (position side) and +0xb4 / +0xcc (direction side) decide which of two arrows per side is shown; ROUGH */
void sv_Unk52d480(void)
{
    int pic, n3, n5, o;
    char a, b;
    char *o1;

    a = 0;
    b = 0;
    n3 = 0;
    o = 0;
    if ((SV_B((unsigned)&sv_FrameMask) & 8) == 0)
        return;
    o1 = SV_C((unsigned)&ai_PlayerBuilding);
    if ((o1 != 0 || (SV_C((unsigned)&ai_FallbackBuilding) != 0 && (o1 = SV_C((unsigned)&ai_FallbackBuilding)) != 0)) && (SV_I((unsigned)&sv_PanelShown) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
        && (SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, o1, SV_I((unsigned)&sv_PanelShown), 0), (SV_I((unsigned)&full_data_6f2320) & 0x1000) == 0))
        return;
    if ((SV_I((unsigned)&ai_BuildingPerms) & 0x1000) == 0)
        return;
    pic = d2_GetPic(0x52c);
    if (SV_I((unsigned)&ai_Unk6f22cc) == -1) {
        n3 = 1;
        if (d3_VecEqual(sv_CameraObj + 0xfc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            n3 = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22cc) == 1) {
        b = 1;
        if (d3_VecEqual(sv_CameraObj + 0xe4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            b = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22cc) == 0) {
        b = d3_VecEqual(sv_CameraObj + 0xe4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0;
        if (d3_VecEqual(sv_CameraObj + 0xfc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            n3 = 1;
    }
    if (SV_I((unsigned)&ai_Unk6f22d0) == -1) {
        a = 1;
        if (d3_VecEqual(sv_CameraObj + 0xb4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            a = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22d0) == 1) {
        o = SV_I((unsigned)&ai_Unk6f22d0);
        if (d3_VecEqual(sv_CameraObj + 0xcc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            o = 2;
    } else if (SV_I((unsigned)&ai_Unk6f22d0) == 0) {
        a = d3_VecEqual(sv_CameraObj + 0xb4, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0;
        if (d3_VecEqual(sv_CameraObj + 0xcc, (char *)(unsigned)&ai_Unk5ebba4, 0.2f) == 0)
            o = 1;
    }
    d2_SetClipRect(0, 0, *(int *)((char *)d2_ScreenSurf + 4), *(int *)((char *)d2_ScreenSurf + 8));
    if (a == 2)
        d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, SV_S((unsigned)&d2_ScreenH) - 0x20, d2_ScreenSurf, pic, 0);
    else if (a == 1)
        d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, SV_S((unsigned)&d2_ScreenH) - 0x20, d2_ScreenSurf, pic, 1);
    if (b == 2)
        d2_DrawShape(4, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, 2);
    else if (b == 1)
        d2_DrawShape(4, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, 3);
    if (n3 == 2)
        n5 = 4;
    else if (n3 == 1)
        n5 = 5;
    else
        n5 = -1;
    if (n5 != -1)
        d2_DrawShape(SV_S((unsigned)&d2_ScreenW) - 0x96, SV_I((unsigned)&d2_HalfScreenH), d2_ScreenSurf, pic, n5);
    if (o != 2) {
        if (o == 1)
            d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, 0x38, d2_ScreenSurf, pic, 7);
        return;
    }
    d2_DrawShape(SV_I((unsigned)&d2_HalfScreenW) - 0x10, 0x38, d2_ScreenSurf, pic, 6);
}
/* 0x52d7a0, per-frame camera follow of the stored view (0x75fb80 / 0x75fb90): when the camera shows a different view than the stored one it is reset (0x433b10) or moved (0x528140); ends the highlight
   (0x6f22ec, via its +0x18c callback) and clears the selection (0x52adb0) when a group is selected */

void sv_CameraFollow(void)
{
    int fl, n;

    fl = 0;
    if (SV_I((unsigned)&d3t_CamMove) != 0) {
        if (d3_VecEqual((char *)(unsigned)&d3_Unk1466620, (char *)(unsigned)&sv_ViewPosX, 0.1f) == 0) {
            d3anim_FreePath(sv_CameraObj);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
        } else {
            d3anim_FreePath(sv_CameraObj);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
            fl = 1;
            d3_SetObjectPos(sv_CameraObj, (unsigned)&sv_ViewPosX);
            d3_SetObjectRot(sv_CameraObj, (unsigned)&sv_ViewRotX);
        }
    }
    if (SV_I((unsigned)&sv_FrameDepth) <= SV_I((unsigned)&sv_FrameBase) || fl) {
        if ((d3_VecEqual(sv_CameraObj + 0x84, (char *)(unsigned)&sv_ViewRotX, 0.1f) == 0 || d3_VecEqual(sv_CameraObj + 0x4c, (char *)(unsigned)&sv_ViewPosX, 0.1f) == 0)
            && (!fl && (sv_Unk528140(), SV_I((unsigned)&full_data_6f22ec) != 0))) {
            n = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
            if (n != 0 && *(int *)(n + 0x18c) != 0)
                ((int (*)())*(int *)(n + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, n + 400, 0);
            SV_I((unsigned)&full_data_6f22ec) = 0;
        }
    } else if (SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest) <= SV_I((unsigned)&full_data_6f22a8)) {
        sv_Unk528140();
        SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest);
        if (SV_I((unsigned)&full_data_6f22ec) != 0) {
            n = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
            if (n != 0 && *(int *)(n + 0x18c) != 0)
                ((int (*)())*(int *)(n + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, n + 400, 0);
            SV_I((unsigned)&full_data_6f22ec) = 0;
        }
        if (SV_I((unsigned)&ai_Unk75ec5c) == 0)
            return;
        sv_Unk52adb0();
    }
    if (SV_I((unsigned)&ai_Unk75ec5c) == 0)
        return;
    sv_Unk52adb0();
}
/* 0x52d940, the mouse click handler of the game screen (fl: 0x800 right button, 0x100 left button, 8 = key flag): plays "MausclickRechts_s_01" for a right click that changes something, closes a
   dialog (0x6f262c: the kinds 0x11 / 0x26 / 0x69 set 0x6f2630, 0x89 goes to 0x575860, the others to 0x53dcb0), routes a click on a shown widget (0x6faa84 table of 3 ints: widget, +0x70 of the object),
   and for the right button resets the selection / the view to the stored one (the "RaumWechseln_s" sound when a room is entered through the 0xfd item); the left button hands over to 0x52a050
   and updates the shown subject (0x6f23e4 / 0x6f23e8) through 0x52e190; ROUGH */

void sv_Unk52d940(unsigned fl)
{
    char c;
    int ok, i, *p, v, who;
    short *it;

    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) != (int)ai_GameFrame || SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_I((unsigned)&ai_Unk75ec5c) != 0 || SV_I((unsigned)&sv_SelectedChar) != 0 || SV_I((unsigned)&sv_SelectedObject) != 0))
        sb_PlaySample(SV_I((unsigned)&snd_SystemBank), "MausclickRechts_s_01", 1, 0x3f, 0x7f, 0, 0);
    if (SV_I((unsigned)&full_data_6f262c) != 0) {
        if (SV_I(((unsigned)&dm_L + 0x20)) == 0 && *(char *)(unsigned)&kb_Last != 0x1c)
            return;
        if (*(char **)SV_I((unsigned)&full_data_6f262c) != 0) {
            c = **(char **)SV_I((unsigned)&full_data_6f262c);
            if (c == 0x11 || c == 0x26 || c == 0x69) {
                SV_I((unsigned)&full_data_6f2630) = 1;
                goto l9f2;
            }
            if (c == (char)-0x79) {
                ai_Unk575860();
                goto l9f2;
            }
        }
        he_Unk53dcb0(0);
l9f2:
        di_ClearEvents(0x10);
        if (*(char *)(unsigned)&kb_Last != 0x1c)
            return;
        *(char *)(unsigned)&kb_Last = 0;
        return;
    }
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && d2_PointerObj != -1) {
        i = 0;
        p = (int *)(unsigned)&full_data_6faa84;
        do {
            if (*p != -1 && p != (int *)4 && d2_PointerObj == *(int *)(SV_I((unsigned)&d2_Objs) + *p * 0x2ec)) {
                *(int *)(SV_I((unsigned)&full_data_6faa80 + i * 12) + 0x70) = -1;
                SV_I((unsigned)&full_data_6f2630) = 0;
                di_ClearButtons();
                break;
            }
            p += 3;
            i++;
        } while ((int)p < ((unsigned)((unsigned)full_data_6faa84 + 0xbc) + 0x4));
    }
    if ((fl & 0x800) == 0)
        goto l72;
    ai_Unk591170();
    anm_Update();
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && SV_I((unsigned)&sv_SelectedObject) != 0 && SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)ai_GameFrame) {
        sv_Unk52adb0();
        goto l72;
    }
    if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) != 0 && SV_I((unsigned)&full_data_6f22fc) != 0 && SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
            SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase) + 1;
            d3_SetObjectPos(sv_CameraObj, (unsigned)&sv_ViewPosX);
            d3_SetObjectRot(sv_CameraObj, (unsigned)&sv_ViewRotX);
            goto lbb6;
        }
    } else if (SV_I((unsigned)&full_data_6f22fc) != 0 && SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
        sb_PlaySample(snd_RoomBank, "RaumWechseln_s", 1, 0x3f, 0x7f, 0, 0);
        *(short *)(SV_I((unsigned)&ai_PlayerBuilding) + 0x29) = *(short *)SV_I((unsigned)&full_data_6f22fc);
        SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase) + 1;
        d3_SetObjectPos(sv_CameraObj, (unsigned)&sv_ViewPosX);
        d3_SetObjectRot(sv_CameraObj, (unsigned)&sv_ViewRotX);
lbb6:
        SV_I((unsigned)&ai_Unk6f22cc) = 0;
        SV_I((unsigned)&ai_Unk6f22d0) = 0;
    }
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && (SV_B((unsigned)&full_data_6f2338) != 0 || (SV_I((unsigned)&ai_Unk75ec5c) != 0 && ai_Goods[*(unsigned short *)SV_I((unsigned)&ai_Unk75ec5c) * 0x41] == 0x1d))) {
        sv_Unk52adb0();
        di_ClearEvents(0x10);
    }
    if ((SV_I((unsigned)&ai_PlayerBuilding) != 0 && ai_ScriptQuery(0x1a, SV_I((unsigned)&ai_PlayerBuilding), 0, *(unsigned short *)SV_I((unsigned)&sv_PanelShown)) == 0)
        || (SV_I((unsigned)&ai_FallbackBuilding) != 0 && ai_ScriptQuery(0x1c, SV_I((unsigned)&ai_FallbackBuilding), 0, 0) == 0))
        di_ClearAxes();
    if (SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_B((unsigned)&full_data_6f2338) != 0 || SV_I(((unsigned)&dm_L + 0x20)) == 0)
        goto l72;
    ok = 0;
    if (SV_I((unsigned)&d3t_CamMove) != 0) {
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0)
            goto l72;
        if (d3_VecEqual((char *)(unsigned)&d3_Unk1466620, (char *)(unsigned)&sv_ViewPosX, 0.1f) == 0) {
            d3anim_FreePath(sv_CameraObj);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
        } else {
            d3anim_FreePath(sv_CameraObj);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
            ok = 1;
            d3_SetObjectPos(sv_CameraObj, (unsigned)&sv_ViewPosX);
            d3_SetObjectRot(sv_CameraObj, (unsigned)&sv_ViewRotX);
        }
    }
    if (SV_I((unsigned)&sv_FrameDepth) <= SV_I((unsigned)&sv_FrameBase) || ok) {
        if (d3_VecEqual(sv_CameraObj + 0x84, (char *)(unsigned)&sv_ViewRotX, 0.1f) != 0 || d3_VecEqual(sv_CameraObj + 0x4c, (char *)(unsigned)&sv_ViewPosX, 0.1f) != 0 || ok) {
            if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&sv_PanelShown) == 0) {
                if (SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase) && SV_I((unsigned)&ai_Unk75ec5c) != 0)
                    sv_Unk52adb0();
                goto l72;
            }
            it = 0;
            if (SV_I((unsigned)&sv_PanelShown) == 0) {
                if (SV_I((unsigned)&ai_FallbackBuilding) != 0 || SV_I((unsigned)&ai_PlayerBuilding) != 0) {
                    SV_I((unsigned)&ai_FallbackBuilding) = 0;
                    SV_I((unsigned)&ai_PlayerBuilding) = 0;
                    goto l1a;
                }
            } else {
                if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
                    if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
                        goto l1a;
                    if (ai_ScriptQuery(0x1c, SV_I((unsigned)&ai_FallbackBuilding), 0, 0) == 1) {
                        c = SV_C(((unsigned)&ai_Selected + 0x29))[ai_PlayerId * 0x21c];
                        v = SV_I((unsigned)&ai_FallbackBuilding);
                        goto lbd;
                    }
                } else {
                    if (ai_ScriptQuery(0x1a, SV_I((unsigned)&ai_PlayerBuilding), 0, *(unsigned short *)SV_I((unsigned)&sv_PanelShown)) == 1) {
                        c = SV_C(((unsigned)&ai_Selected + 0x29))[ai_PlayerId * 0x21c];
                        v = SV_I((unsigned)&ai_PlayerBuilding);
lbd:
                        if (c == 0 && v != 0 && *(unsigned short *)SV_I((unsigned)&sv_PanelShown) == 0xf2)
                            it = (short *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(v);
                    }
                }
                if (SV_I((unsigned)&ai_PlayerBuilding) == 0 || it == 0 || (int)it == SV_I((unsigned)&sv_PanelShown)) {
l1a:
                    SV_I((unsigned)&sv_PanelShown) = 0;
                } else
                    ai_SwitchRoom(-1, *it);
            }
            SV_I((unsigned)&sv_PopRequest) = 1;
            if (SV_I((unsigned)&ai_PlayerBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) == 0)
                SV_I((unsigned)&ai_PlayerBuilding) = 0;
            d3_SetObjectPos(sv_CameraObj, (unsigned)&sv_ViewPosX);
            d3_SetObjectRot(sv_CameraObj, (unsigned)&sv_ViewRotX);
            SV_I((unsigned)&ai_Unk6f22cc) = 0;
            SV_I((unsigned)&ai_Unk6f22d0) = 0;
            goto l72;
        }
        if (SV_I((unsigned)&ai_PlayerBuilding) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sv_Unk528140();
    } else {
        SV_I((unsigned)&sv_PopRequest) = 1;
        if (SV_I((unsigned)&full_data_6f22a8) < SV_I((unsigned)&sv_FrameDepth) - 1)
            goto l72;
        if (SV_I((unsigned)&ai_PlayerBuilding) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sv_Unk528140();
        SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest);
    }
    if (SV_I((unsigned)&full_data_6f22ec) != 0) {
        v = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
        if (v != 0 && *(int *)(v + 0x18c) != 0)
            ((int (*)())*(int *)(v + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, v + 400, 0);
        SV_I((unsigned)&full_data_6f22ec) = 0;
    }
    sv_Unk52adb0();
l72:
    if ((fl & 0x100) != 0)
        sv_UpdatePanel();
    who = SV_I((unsigned)&ai_PlayerBuilding);
    if (who == 0 && (who = SV_I((unsigned)&ai_FallbackBuilding)) == 0)
        who = 0;
    if ((fl & 8) != 0 && who != 0) {
        it = (short *)SV_I((unsigned)&sv_PanelShown);
        if (it == 0)
            it = 0;
        SV_I((unsigned)&full_data_6f2320) = gm_PermissionBits(ai_PlayerId, who, it, 0);
        if (*(unsigned short *)(who + 0x27) == ai_PlayerId || (SV_I((unsigned)&full_data_6f2320) & 0x1000) != 0)
            sv_Unk525ff0();
    }
    if (SV_I((unsigned)&full_data_6f23e4) != who || SV_I((unsigned)&full_data_6f23e8) != SV_I((unsigned)&sv_PanelShown) || SV_I((unsigned)&ai_Unk6f2318) != 0) {
        SV_I((unsigned)&ai_BuildingPerms) = gm_PermissionBits(ai_PlayerId, who, SV_I((unsigned)&sv_PanelShown), 0);
        sv_ReleaseNamePlates();
        SV_I((unsigned)&full_data_6f23e8) = SV_I((unsigned)&sv_PanelShown);
        SV_I((unsigned)&ai_Unk6f2318) = 0;
        SV_I((unsigned)&full_data_6f23e4) = who;
    }
}

#include <stdarg.h>
extern unsigned char ai_GameState;
extern unsigned char d3_MipSkip;

struct D3OBJECT * d3t_FindObject(char *name, char flags);                     /* 0x411900 */
int d2_TextWidth();                     /* 0x46ea10 */
int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */

/* 0x52e050, draws the hint text 0xf8aae0 at the mouse position when the hint flag (0x6f3618 +2) is set */
void sv_Unk52e050(void)
{
    if (SV_B((unsigned)&ai_Unkf8aae0) != 0 && SV_B(((unsigned)&ai_GameSwitches + 0x2)) != 0) {
        d2_SetFont(0x54);
        d2_DrawTextBox(SV_S((unsigned)&d2_MouseX) - 0x36, SV_S((unsigned)&d2_MouseY) + 0x34, 0xa0, 0x60, d2_ScreenSurf, (char *)(unsigned)&ai_Unkf8aae0, 0xa8);
        d2_AddDirty(SV_I((unsigned)&d2_TextLeft), SV_I((unsigned)&d2_TextTop), SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
    }
}
/* 0x52e0c0, draws the status line 0x761f40 (screen centre, y by a float of the clock) until it is 0x15e ms old */

void sv_Unk52e0c0(void)
{
    int x, y;

    if (SV_B((unsigned)&sv_StatusText) != 0) {
        d2_SetFont(0x53);
        if ((SV_U((unsigned)&sv_FrameMask) & 0x8000) != 0) {
            x = SV_I((unsigned)&d2_HalfScreenW) - 300;
            y = (int)(*(float *)(unsigned)&ai_Unk5fe708 * 505.0f);
        } else {
            x = SV_I((unsigned)&d2_HalfScreenW) - 300;
            y = (int)(*(float *)(unsigned)&ai_Unk5fe708 * 575.0f);
        }
        d2_DrawTextBox(x, y, 600, 0x18, d2_ScreenSurf, (char *)(unsigned)&sv_StatusText, 0x28);
        d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        if (SV_U((unsigned)&sv_StatusTime) + 0x15e <= SV_U((unsigned)&ai_ClockMs))
            SV_B((unsigned)&sv_StatusText) = 0;
    }
}
/* 0x52e170, clears the name plate records sv_NamePlates */

void sv_ClearNamePlates(void)
{
    SVPLATE *p;

    p = sv_NamePlates;
    do {
        p->obj = 0;
        p++;
    } while ((int)p < (int)(sv_NamePlates + 0x20));
}
/* 0x52e190, releases the name plates: the objects lose their plate hook (+0x224), the records are cleared */

void sv_ReleaseNamePlates(void)
{
    SVPLATE *p;

    p = sv_NamePlates;
    do {
        if (p->obj != 0)
            ((SVPLATEOBJ *)p->obj)->plate = 0;
        p->obj = 0;
        p++;
    } while ((int)p < (int)(sv_NamePlates + 0x20));
}
/* 0x52e1c0, registers (or finds) the plate for the object called name (entry: +0 the object, +4 its name, +0x44 b, +0x48 the text, 200 bytes each); returns the object or 0 */

int sv_RegisterNamePlate(char *name, char *text, int b)
{
    int i, o;
    char *e;
    char msg[0x100];

    i = 0;
    e = (char *)((unsigned)&sv_NamePlates + 0x4);
    do {
        if (*(int *)(e - 4) != 0 && strcmp(e, name) == 0) {
            *(int *)(SV_I((unsigned)&sv_NamePlates + i * 200) + 0x224) = 1;
            return SV_I((unsigned)&sv_NamePlates + i * 200);
        }
        e += 200;
        i++;
    } while ((int)e < ((unsigned)((unsigned)sv_NamePlates + 0x1900) + 0x4));
    i = 0;
    e = (char *)(unsigned)&sv_NamePlates;
    do {
        if (*(int *)e == 0) {
            if (i < 0x20) {
                o = d3t_FindObject(name, 2);
                SV_I((unsigned)&sv_NamePlates + i * 200) = o;
                if (o != 0) {
                    SV_I(((unsigned)&sv_NamePlates + 0x44) + i * 200) = b;
                    strncpy((char *)(((unsigned)&sv_NamePlates + 0x48) + i * 200), text, 0x80);
                    strncpy((char *)(((unsigned)&sv_NamePlates + 0x4) + i * 200), name, 0x40);
                    *(char *)(((unsigned)&sv_NamePlates + 0xc7) + i * 200) = 0;
                    *(char *)(((unsigned)&sv_NamePlates + 0x43) + i * 200) = 0;
                    *(int *)(SV_I((unsigned)&sv_NamePlates + i * 200) + 0x224) = 1;
                    return SV_I((unsigned)&sv_NamePlates + i * 200);
                }
                sprintf(msg, "RegisterStatusText(): Object not found: %s", name);
                return 0;
            }
            break;
        }
        e += 200;
        i++;
    } while ((int)e < (unsigned)((unsigned)sv_NamePlates + 0x1900));
    sprintf(msg, "RegisterStatusText(): Too many Objects: %s", name);
    return 0;
}
/* 0x52e330, sets the status line text sv_StatusText and its time stamp sv_StatusTime (d3_Time) */

void sv_SetStatusText(char *s)
{
    strcpy(sv_StatusText, s);
    sv_StatusTime = d3_Time;
}
/* 0x52e350, creates n buttons (widget kind 0xbf, picture 0x53) in a row in the form f at height y, the labels are the n string arguments; the widths are the widest label + 0x20, ids returned in out[];
   the row is centred (or the labels' width is used as the step when the form is too narrow) */

void sv_ButtonRow(int f, int y, int *out, int n, ...)
{
    char labels[0x20][0x40];
    int wmax, w, step, x, i, id;
    char *s;
    va_list ap;

    wmax = 0;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        s = va_arg(ap, char *);
        strcpy(labels[i], s);
        w = d2_TextWidth(s, (int)*(short *)((unsigned)&ai_Unkf8f39c + f * 0x3c4)) + 0x20;
        if (wmax < w)
            wmax = w;
    }
    w = *(short *)((unsigned)&ai_Unkf8f128 + f * 0x3c4);
    step = w / (n + 1);
    x = step;
    if (step < wmax) {
        step = wmax + 8;
        x = step * n;
        if (x - w == 0 || x < w)
            x = w - x;
        else
            x = x - w;
        x = x / 2 + step / 2;
    }
    x -= wmax / 2;
    for (i = 0; i < n; i++) {
        id = d2_WinCreateSlider(x, y, 0xbf, labels[i], f);
        out[i] = id;
        SV_WS(id, 0x74) = 0x53;
        d2_SetObjWidth(id, wmax);
        x += step;
    }
}
/* 0x52e4b0, like 0x52e350 with the picture 0x54 / widget kind 0xc5 and the label width measured with the font 0x54 */

void sv_Unk52e4b0(int f, int y, int *out, int n, ...)
{
    char labels[0x20][0x40];
    int wmax, w, step, x, i, id;
    char *s;
    va_list ap;

    wmax = 0;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        s = va_arg(ap, char *);
        strcpy(labels[i], s);
        w = d2_TextWidth(s, 0x54);
        if (wmax < w + 0x20)
            wmax = w + 0x20;
    }
    w = *(short *)((unsigned)&ai_Unkf8f128 + f * 0x3c4);
    step = w / (n + 1);
    x = step;
    if (step < wmax) {
        step = wmax + 8;
        x = step * n;
        if (x - w == 0 || x < w)
            x = w - x;
        else
            x = x - w;
        x = x / 2 + step / 2;
    }
    x -= wmax / 2;
    for (i = 0; i < n; i++) {
        id = d2_WinCreateSlider(x, y, 0xc5, labels[i], f);
        out[i] = id;
        SV_WS(id, 0x74) = 0x54;
        d2_SetObjWidth(id, wmax);
        x += step;
    }
}

int gm_Unk504e00();                     /* 0x504e00 */
int ai_Unk4ce170();                     /* 0x4ce170 */

/* 0x52e600, a != 0: collects the widgets (0x2ec records of the table 0xf8f080) whose +0x40 points into a visible form record of 0xfb1158 (stride 0x2ac) and that are not yet flagged +0x38, flags them and
   remembers their index (0x6f1c90, count 0x6f23ec); a == 0: clears the flags again */
void sv_DisableVisibleWidgets(int a)
{
    int i, n, *p, w;
    short idx;

    if (a != 0) {
        SV_I((unsigned)&full_data_6f23ec) = 0;
        p = (int *)((unsigned)&ai_Unkfb1030 + 0x190);
        w = SV_I((unsigned)&d2_Objs);
        do {
            if (p[-4] != 0 && *p != 0) {
                idx = 0;
                n = 0;
                do {
                    if (*(int *)(n + 0x18 + w) != 0 && *(int **)(n + 0x40 + w) == p - 0x68 && *(int *)(n + 0x38 + w) == 0) {
                        ((unsigned short *)(unsigned)&full_data_6f1c90)[SV_I((unsigned)&full_data_6f23ec)] = idx;
                        SV_I((unsigned)&full_data_6f23ec)++;
                        *(int *)(n + 0x38 + w) = 1;
                        w = SV_I((unsigned)&d2_Objs);
                    }
                    n += 0x2ec;
                    idx++;
                } while (n < 0x8c400);
            }
            p += 0xab;
        } while ((int)p < (unsigned)((unsigned)ai_Unkfb1030 + 0x81d0));
        return;
    }
    i = 0;
    if (0 < SV_I((unsigned)&full_data_6f23ec)) {
        do {
            n = ((unsigned short *)(unsigned)&full_data_6f1c90)[i];
            i++;
            *(int *)(SV_I((unsigned)&d2_Objs) + 0x38 + n * 0x2ec) = 0;
        } while (i < SV_I((unsigned)&full_data_6f23ec));
    }
}
/* 0x52e6d0, builds the picture frames of the dialog form f (0x4731d0 (x, y, picture, form)): the title bar 0x520 / 0x427 + the building kind, the corners; the picture b = 0x73b selects the
   wide variant; sets the four borders of the form (+0x1fc .. 0x20c) to 0x18 */

void sv_Unk52e6d0(int f, int b, int c)
{
    unsigned char *o;
    int pic, x, y;

    d2_WinOpenPic(599, 9, 0x520, f);
    if (c == 0) {
        o = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
        if (o == 0 && (o = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0)
            c = 0x520;
        else
            c = *o + 0x427;
    }
    d2_WinOpenPic(0xb, 9, c, f);
    o = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
    if (o == 0 && (o = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0) {
        pic = 0x444;
        goto l748;
    }
    if (b != 0x73b) {
        pic = *o + 0x427;
l748:
        d2_WinOpenPic(599, 9, pic, f);
        if (b != 0x73b) {
            d2_WinOpenPic(0, 0, b, f);
            d2_WinOpenPic(0, 400, b + 1, f);
            d2_WinOpenPic(0, 0x4d, b + 2, f);
            b = b + 3;
            y = 0x4d;
            x = 0x247;
            goto l815;
        }
    } else {
        d2_WinOpenPic(0x1eb, 9, 0x520, f);
        d2_WinOpenPic(0x1eb, 9, *o + 0x427, f);
    }
    d2_WinOpenPic(0, 0, 0x73b, f);
    d2_WinOpenPic(0, 0x43, 0x73d, f);
    d2_WinOpenPic(0x1f7, 0x43, 0x73e, f);
    b = 0x73c;
    y = 0x1a8;
    x = 0;
l815:
    d2_WinOpenPic(x, y, b, f);
    pic = SV_I((unsigned)&d2_LastWindow) * 0x3c4;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x11c) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x120) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x124) + pic) = 0x18;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x128) + pic) = 0x18;
}
/* 0x52e860, builds the slider of the form f at (x, y): the two bars (pictures 3 / 4, widgets *a / *b shown, kind 3, +0x1e0 = form index), the grip button (0x473650, picture 0x54) in *c; stores them in the form
   record (0xf8f4c8 ..) and the callback p5 in the form record of g (+0x384) */

int sv_CreateFormSlider(int x, int y, int f, int g, int p5, int *b, int *a, int *c)
{
    int id, fr;

    id = d2_WinOpenPic(x, y, 3, f);
    *a = id;
    fr = f * 0x3c4;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x12c) + fr) = id;
    SV_WI(*a, 0x4c) = 1;
    SV_WI(*a, 0x1c0) = 3;
    id = d2_WinOpenPic(x, y + 0x2a, 4, f);
    *b = id;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x130) + fr) = id;
    SV_WI(*b, 0x4c) = 1;
    SV_WI(*b, 0x1c0) = 3;
    SV_WI(*b, 0x1e0) = g;
    SV_WI(*a, 0x1e0) = g;
    id = d2_CreateWinText(x - 0x13, y + 0x19, ch_EmptyName, f);
    *c = id;
    SV_I(((unsigned)&ai_Unkf8f39c + 0x134) + fr) = id;
    SV_WS(*c, 0x74) = 0x54;
    SV_WI(*c, 0x5c) = 1;
    SV_WS(*c, 0x14) = 0x30;
    SV_I(((unsigned)&ai_Unkf8f13c + 0x22c) + g * 0x3c4) = 0;
    SV_I(((unsigned)&ai_Unkf8f13c + 0x248) + g * 0x3c4) = p5;
    return 1;
}
/* 0x52ea30, resizes the form f to the size of its background picture (0x6ed918 + index * 0x54, +0x50 / +0x52) when it has one (0xf8f4a8 != -1) and sets its borders 0x18 / 0x18 / 0x1b / 0xb */

void sv_FitForm(int f)
{
    int p;

    f = f * 0x3c4;
    if (SV_I(((unsigned)&ai_Unkf8f39c + 0x10c) + f) != -1) {
        p = SV_I((unsigned)&d2_Pics) + SV_I(((unsigned)&ai_Unkf8f39c + 0x10c) + f) * 0x54;
        SV_S((unsigned)&ai_Unkf8f128 + f) = *(short *)(p + 0x50);
        SV_S(((unsigned)&ai_Unkf8f128 + 0x2) + f) = *(short *)(p + 0x52);
        SV_I(((unsigned)&ai_Unkf8f39c + 0x11c) + f) = 0x18;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x120) + f) = 0x18;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x124) + f) = 0x1b;
        SV_I(((unsigned)&ai_Unkf8f39c + 0x128) + f) = 0xb;
    }
}
/* 0x52eaa0, draws the value as a row of picture tiles at (x, y) in the window d2_LastWindow: n = value / 21 (one
   talent step; the original multiplies by the float 1/21 at 0x5eeb88), n / 2 whole tiles of the picture k (width
   +0x50 + 2) and, when n is odd, a half tile (picture k + 1). The value is an int (the original's fild). */

void sv_ValueBar(int x, int y, int value, int k)
{
    unsigned n, odd;
    int w, i;

    w = *(short *)(SV_I((unsigned)&d2_Pics) + 0x50 + k * 0x54) + 2;
    n = (int)(value / 21.0f);
    odd = n & 0x80000001;
    if ((int)odd < 0)
        odd = (odd - 1 | 0xfffffffe) + 1;
    i = 0;
    if (0 < (int)n / 2) {
        do {
            d2_WinOpenPic(w * i + x, y, k, SV_I((unsigned)&d2_LastWindow));
            i++;
        } while (i < (int)n / 2);
    }
    if (odd != 0)
        d2_WinOpenPic(w * i + x, y, k + 1, SV_I((unsigned)&d2_LastWindow));
}
/* 0x52eb50, draws the two-sided bar (current value 0x504e00 (a, b) against the byte at b + 0x80 + a): equal (within 1 %) a single bar, the picture k (+1 for an odd tile), smaller / larger the base bar
   in k and the difference in k + 2 / k + 3 or k + 4 / k + 5 (the float operands deciding the tile counts are lost in the pseudocode: n = placeholders); ROUGH */

void sv_DrawBalanceBar(int x, int y, int a, int b, unsigned k)
{
    unsigned u1, u2, n, odd;
    int pic, w, i;
    double dd;

    pic = k;
    w = *(short *)(SV_I((unsigned)&d2_Pics) + 0x50 + k * 0x54) + 2;
    u1 = gm_Unk504e00(a, b);
    u2 = *(unsigned char *)(b + 0x80 + a);
    k = u1;
    if ((int)u1 <= (int)u2)
        k = u2;
    dd = (double)(int)(u1 - u2);
    if (dd < 0)
        dd = -dd;
    if (dd / (double)(int)k <= 0.01) {
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
        pic++;
    } else if ((int)u1 < (int)u2) {
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic + 2, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd != 0)
            d2_WinOpenPic(w * i + x, y, pic + 3, SV_I((unsigned)&d2_LastWindow));
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
        pic++;
    } else {
        if ((int)u1 <= (int)u2)
            return;
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic + 4, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd != 0)
            d2_WinOpenPic(w * i + x, y, pic + 5, SV_I((unsigned)&d2_LastWindow));
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
    }
    d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
}
/* 0x52ee50, starts the rubber band selection (0x760440 = 1): both corners (0x760444 / 0x760448 and 0x76044c / 0x760450) are the mouse position clamped to the view rectangle 0xfc2930 .. 0xfc293c */

void sv_Unk52ee50(void)
{
    int mx, my, m;

    mx = SV_S((unsigned)&dm_L);
    m = SV_I((unsigned)&d3_Viewport);
    if (m <= mx)
        m = mx;
    SV_I((unsigned)&full_data_760444) = SV_I(((unsigned)&d3_Viewport + 0x8)) - 1;
    if (m < SV_I(((unsigned)&d3_Viewport + 0x8)) - 1 && (SV_I((unsigned)&full_data_760444) = mx, mx < SV_I((unsigned)&d3_Viewport)))
        SV_I((unsigned)&full_data_760444) = SV_I((unsigned)&d3_Viewport);
    my = SV_S(((unsigned)&dm_L + 0x2));
    m = SV_I(((unsigned)&d3_Viewport + 0x4));
    if (m <= my)
        m = my;
    SV_I((unsigned)&full_data_760448) = SV_I(((unsigned)&d3_Viewport + 0xc)) - 1;
    if (m < SV_I((unsigned)&full_data_760448) && (SV_I((unsigned)&full_data_760448) = my, my < SV_I(((unsigned)&d3_Viewport + 0x4))))
        SV_I((unsigned)&full_data_760448) = SV_I(((unsigned)&d3_Viewport + 0x4));
    SV_I((unsigned)&full_data_76044c) = SV_I((unsigned)&full_data_760444);
    SV_I((unsigned)&ai_Unk760440) = 1;
    SV_I((unsigned)&full_data_760450) = SV_I((unsigned)&full_data_760448);
}
/* 0x52eed0, ends the rubber band selection */

void sv_Unk52eed0(void)
{
    SV_I((unsigned)&ai_Unk760440) = 0;
}
/* 0x52eee0, during the rubber band: moves the second corner with the mouse (clamped to the view) and flags (+0x188) the characters of the table 0x7603a0 whose screen position (three float conversions, the
   operands are lost: placeholders) lies inside the band; ROUGH */

void sv_Unk52eee0(void)
{
    unsigned fl;
    int mx, my, m, x0, x1, y0, y1, px, py, c, *p, g;

    if (SV_I((unsigned)&ai_Unk760440) != 0) {
        mx = SV_S((unsigned)&dm_L);
        m = SV_I((unsigned)&d3_Viewport);
        if (m <= mx)
            m = mx;
        SV_I((unsigned)&full_data_76044c) = SV_I(((unsigned)&d3_Viewport + 0x8)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0x8)) - 1 && (SV_I((unsigned)&full_data_76044c) = mx, mx < SV_I((unsigned)&d3_Viewport)))
            SV_I((unsigned)&full_data_76044c) = SV_I((unsigned)&d3_Viewport);
        my = SV_S(((unsigned)&dm_L + 0x2));
        m = SV_I(((unsigned)&d3_Viewport + 0x4));
        if (m <= my)
            m = my;
        SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0xc)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0xc)) - 1 && (SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0x4)), SV_I(((unsigned)&d3_Viewport + 0x4)) <= my))
            SV_I((unsigned)&full_data_760450) = my;
        x0 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_76044c) <= SV_I((unsigned)&full_data_760444))
            x0 = SV_I((unsigned)&full_data_76044c);
        y0 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760450) <= SV_I((unsigned)&full_data_760448))
            y0 = SV_I((unsigned)&full_data_760450);
        x1 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_760444) <= SV_I((unsigned)&full_data_76044c))
            x1 = SV_I((unsigned)&full_data_76044c);
        y1 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760448) <= SV_I((unsigned)&full_data_760450))
            y1 = SV_I((unsigned)&full_data_760450);
        p = (int *)(unsigned)&ai_SceneChars;
        do {
            c = *p;
            if (c != 0 && *(char *)(c + 8) != 0) {
                fl = gm_PermissionBits(ai_PlayerId, 0, 0, c);
                *(char *)(c + 0x188) = 0;
                if ((fl & 0x800) != 0 && (g = *(int *)(c + 0x184)) != 0 && *(char **)(g + 0x88) == ai_Unk5fb5b0 && *(int *)(*(int *)(g + 0x34) + 0x1cc) != 0) {
                    px = (int)0.0f;             /* the screen position of the character (float operands lost) */
                    py = (int)0.0f;
                    if (x0 <= px && px <= x1 && y0 <= py && py <= y1)
                        *(char *)(c + 0x188) = 1;
                }
            }
            p++;
        } while ((int)p < (unsigned)((unsigned)ai_SceneChars + 0x80));
    }
}
/* 0x52f0d0, like 0x52eee0 (the flags come from 0x4ce170): moves the second corner with the mouse (clamped to the view) and flags (+0x188) the characters of the table 0x7603a0 whose screen position (three float conversions, the
   operands are lost: placeholders) lies inside the band; ROUGH */

void sv_Unk52f0d0(void)
{
    unsigned fl;
    int mx, my, m, x0, x1, y0, y1, px, py, c, *p, g;

    if (SV_I((unsigned)&ai_Unk760440) != 0) {
        mx = SV_S((unsigned)&dm_L);
        m = SV_I((unsigned)&d3_Viewport);
        if (m <= mx)
            m = mx;
        SV_I((unsigned)&full_data_76044c) = SV_I(((unsigned)&d3_Viewport + 0x8)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0x8)) - 1 && (SV_I((unsigned)&full_data_76044c) = mx, mx < SV_I((unsigned)&d3_Viewport)))
            SV_I((unsigned)&full_data_76044c) = SV_I((unsigned)&d3_Viewport);
        my = SV_S(((unsigned)&dm_L + 0x2));
        m = SV_I(((unsigned)&d3_Viewport + 0x4));
        if (m <= my)
            m = my;
        SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0xc)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0xc)) - 1 && (SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0x4)), SV_I(((unsigned)&d3_Viewport + 0x4)) <= my))
            SV_I((unsigned)&full_data_760450) = my;
        x0 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_76044c) <= SV_I((unsigned)&full_data_760444))
            x0 = SV_I((unsigned)&full_data_76044c);
        y0 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760450) <= SV_I((unsigned)&full_data_760448))
            y0 = SV_I((unsigned)&full_data_760450);
        x1 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_760444) <= SV_I((unsigned)&full_data_76044c))
            x1 = SV_I((unsigned)&full_data_76044c);
        y1 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760448) <= SV_I((unsigned)&full_data_760450))
            y1 = SV_I((unsigned)&full_data_760450);
        p = (int *)(unsigned)&ai_SceneChars;
        do {
            c = *p;
            if (c != 0 && *(char *)(c + 8) != 0) {
                fl = ai_Unk4ce170(ai_PlayerId, c);
                *(char *)(c + 0x188) = 0;
                if ((fl & 0x800) != 0 && (g = *(int *)(c + 0x184)) != 0 && *(char **)(g + 0x88) == ai_Unk5fb5b0 && *(int *)(*(int *)(g + 0x34) + 0x1cc) != 0) {
                    px = (int)0.0f;             /* the screen position of the character (float operands lost) */
                    py = (int)0.0f;
                    if (x0 <= px && px <= x1 && y0 <= py && py <= y1)
                        *(char *)(c + 0x188) = 1;
                }
            }
            p++;
        } while ((int)p < (unsigned)((unsigned)ai_SceneChars + 0x80));
    }
}

void dd_Line();                     /* 0x41e8f0 */
void * mem_Alloc(int size, char *tag);                      /* 0x4223c0 */
void mem_Free(void *p);                     /* 0x422620 */
int d3t_ReleaseObjectTextures();                     /* 0x411b70 */
char d3if_Show(struct D3OBJECT *obj, char show);                     /* 0x4471b0 */
int d2_LoadView();                     /* 0x474b30 */
void d2_FillViewTextures(int view, char *tex1, char *tex2); /* 0x474d90 */
void d2_RedrawView(int view);                     /* 0x474e80 */
int d2_SetViewTextures(int view, char *tex1, char *tex2);  /* 0x474ca0 */
void d2_ClearViewTextures(int view);                      /* 0x474d10 */
void d2_EnableViewObjects();                     /* 0x4754d0 */
void d3d_SetWBuffer(char w);                     /* 0x437060 */
char *util_BaseName();                   /* 0x401040 */
int sv_Unk52f710(int *book, char dir);
void sv_Unk52f9f0(int *book);
void sv_Unk52fa00(int *book);

/* 0x52f2c0, draws the rubber band rectangle (0x41e8f0, colour -1) between the corners 0x760444 / 0x760448 and 0x76044c / 0x760450 while 0x760440 is set */
void sv_Unk52f2c0(void)
{
    if (SV_I((unsigned)&ai_Unk760440) != 0) {
        dd_Line(SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760448), SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760448), -1);
        dd_Line(SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760448), SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760450) + 1, -1);
        dd_Line(SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760450), SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760450), -1);
        dd_Line(SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760450), SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760448), -1);
    }
}
/* 0x52f340, the page keys of the book: the button 0x767 turns forward (0x52f9f0), 0x768 backward (0x52fa00) */

void sv_Unk52f340(int *book)
{
    if (SV_I((unsigned)&d2_ClickedId) == 0x767) {
        sv_Unk52f9f0(book);
        return;
    }
    if (SV_I((unsigned)&d2_ClickedId) == 0x768)
        sv_Unk52fa00(book);
}
/* 0x52f360, opens a book ("f3_sv:OpenBook", 0x274 bytes: +0 the book object, +4 its parent, +0x228 the page widgets (n of them, +0x271), +0x270 the first shown page, +0x268 / +0x26c the page button ids, +0x272 the
   saved 0x63342d): the book model a (0x411900), optionally put at the scene object b (0x446e10); n page widgets (0x474b30 (0, 0, text)) with the "Buch_Seite<i>_oben / unten_ns_nm" pictures; the n texts follow
   as variable arguments; plays "Aufklappen_s"; ROUGH */

int *sv_Unk52f360(int a, int b, char n, ...)
{
    int *bk;
    int i, w, *p;
    char top[0x60], bottom[0x60];
    int *arg;

    bk = (int *)mem_Alloc(0x274, "f3_sv:OpenBook");
    if (bk == 0)
        return 0;
    w = d3t_FindObject(a, 1);
    bk[0] = w;
    if (w == 0) {
lfail:
        mem_Free(bk);
        return 0;
    }
    if (b == 0) {
        bk[1] = 0;
    } else {
        d3if_Show(w, 0);
        w = bk[0];
        bk[1] = w;
        w = d3_CreateObject(*(int *)(w + 0x1f8), w + 0x4c, w + 0x84, b);
        bk[0] = w;
    }
    *(unsigned char *)(bk[0] + 0x210) &= 0xdf;
    d3_RelightObject(bk[0]);
    *((char *)bk + 0x271) = n;
    w = (int)n;
    *((char *)(bk + 0x9c)) = 0;
    bk[0x9a] = -1;
    bk[0x9b] = -1;
    if (0 < w) {
        arg = (int *)(&n + 4);
        p = bk + 0x8a;
        i = 0;
        do {
            *p = d2_LoadView(0, 0, *arg++);
            if (*p == 0)
                goto lfail;
            sprintf(top, "Buch_Seite%i_oben_ns_nm", i + 1);
            sprintf(bottom, "Buch_Seite%i_unten_ns_nm", i + 1);
            d2_FillViewTextures(*p, top, bottom);
            d2_RedrawView(*p);
            if (2 < i)
                d2_EnableViewObjects(*p, 0);
            p++;
            i++;
        } while (i < w);
    }
    if (SV_B((unsigned)&d3d_ZEnabled) != 0) {
        *((char *)bk + 0x272) = SV_B((unsigned)&d3d_ZEnabled);
        d3d_SetWBuffer(0);
    }
    sb_PlaySample(snd_BookBank, "Aufklappen_s", 1, 0x3f, 0x7f, 0, 0);
    return bk;
}
/* 0x52f530, closes a book: frees the model (0x411b70, back to the parent 0x447190 when it had one), the page widgets (0x474d10 / 0x4758f0), the two buttons, restores 0x437060 and plays "Zuklappen_s" */

void sv_Unk52f530(int *bk)
{
    int i, *p;

    if (bk != 0) {
        d3t_ReleaseObjectTextures(bk[0]);
        if (bk[1] != 0) {
            d3if_Show(bk[1], 1);
            d3if_Delete((struct D3OBJECT *)bk[0]);
            bk[0] = bk[1];
        }
        i = 0;
        if (0 < *((char *)bk + 0x271)) {
            p = bk + 0x8a;
            do {
                d2_ClearViewTextures(*p);
                d2_CloseView(*p);
                i++;
                p++;
            } while (i < *((char *)bk + 0x271));
        }
        if (bk[0x9a] != -1)
            d2_RemoveObj(bk[0x9a]);
        if (bk[0x9b] != -1)
            d2_RemoveObj(bk[0x9b]);
        if (*((char *)bk + 0x272) != 0)
            d3d_SetWBuffer(1);
        mem_Free(bk);
        sb_PlaySample(snd_BookBank, "Zuklappen_s", 1, 0x3f, 0x7f, 0, 0);
    }
}
/* 0x52f600, per frame update of an open book: hides the page widgets outside the shown pair (+0x270 .. +0x270 + 1), turns the page when a page button (+0x268 / +0x26c) is the focus, and shows the shown
   pair (blink 0x6f23f0) */

void sv_Unk52f600(int bk)
{
    int *p, i;

    if (bk != 0) {
        i = 0;
        if (*(char *)(bk + 0x271) > 0) {
            p = (int *)(bk + 0x228);
            do {
                if (i < *(char *)(bk + 0x270) || *(char *)(bk + 0x270) + 1 < i)
                    d2_EnableViewObjects(*p, 0);
                i++;
                p++;
            } while (i < *(char *)(bk + 0x271));
        }
        if (*(int *)(bk + 0x26c) != -1 && d2_PointerObj == *(int *)(bk + 0x26c) && SV_I(((unsigned)&dm_L + 0xc)) != 0)
            sv_Unk52fa00((int *)bk);
        if (*(int *)(bk + 0x268) != -1 && d2_PointerObj == *(int *)(bk + 0x268) && SV_I(((unsigned)&dm_L + 0xc)) != 0)
            sv_Unk52f9f0((int *)bk);
        i = (int)*(char *)(bk + 0x270) + SV_B((unsigned)&full_data_6f23f0);
        if (i < *(char *)(bk + 0x271)) {
            p = (int *)(bk + 0x228 + i * 4);
            do {
                if ((int)*(char *)(bk + 0x270) + SV_B((unsigned)&full_data_6f23f0) < i)
                    break;
                d2_RedrawView(*p);
                i++;
                p++;
            } while (i < *(char *)(bk + 0x271));
        }
        SV_I((unsigned)&full_data_6f23f0) ^= 1;
    }
}
/* 0x52f710, turns the book bk by dir pages (2 forward / -2 backward): checks the limits and that the page animations of the book model (+0x1ec) are finished ("buch\Buch_vorblaettern.baf" /
   "buch\Buch_zurueckblaettern.baf"), sets the page pictures of the n page widgets, starts the animation, advances the first page (+0x270), plays "Umblaettern_s"; returns 1 when turned; ROUGH */

int sv_Unk52f710(int *bk, char dir)
{
    char *s, *anim;
    int off, a, i, rec, *p, pg, lim;
    char fwd[0x1c], back[0x1c];
    char top[0x40], bottom[0x40];

    if (bk == 0)
        return 0;
    if (dir < 0) {
        if ((char)bk[0x9c] - 2 < 0)
            return 0;
    } else if ((int)*((char *)bk + 0x271) <= (char)bk[0x9c] + 2)
        return 0;
    off = 0;
    do {
        strcpy(fwd, "buch\\Buch_vorblaettern.baf");
        s = util_BaseName(fwd);
        anim = *(char **)(*(int *)(bk[0] + 0x1ec) + 0x178 + off);
        rec = *(int *)(bk[0] + 0x1ec) + 0x110 + off;
        if (anim != 0 && _strcmpi(anim, s) == 0 && dir > 0) {
            if ((*(unsigned char *)(rec + 0x6d) & 0x20) == 0)
                return 0;
            d3_ReleaseTexture(*(int *)(bk[0] + 0x1ec) + 0xf4, *(int *)(off + 0x178 + *(int *)(bk[0] + 0x1ec)));
        }
        strcpy(back, "buch\\Buch_zurueckblaettern.baf");
        s = util_BaseName(back);
        if (*(char **)(rec + 0x68) != 0 && _strcmpi(*(char **)(rec + 0x68), s) == 0) {
            if ((*(unsigned char *)(rec + 0x6d) & 0x20) == 0 && dir > 0)
                return 0;
            d3_ReleaseTexture(*(int *)(bk[0] + 0x1ec) + 0xf4, *(int *)(off + 0x178 + *(int *)(bk[0] + 0x1ec)));
        }
        off += 0x7c;
    } while (off < 0x174);
    i = 0;
    s = (char *)(*(int *)(bk[0] + 0x1ec) + 0x17d);
    do {
        if (*(int *)(s - 5) != 0 && (*(unsigned char *)s & 0x20) == 0)
            return 0;
        i++;
        s += 0x7c;
    } while (i < 3);
    pg = 0;
    if (*((char *)bk + 0x271) > 0) {
        p = bk + 0x8a;
        do {
            a = (int)(char)bk[0x9c];
            lim = a;
            if (dir <= 0)
                lim = a - 2;
            if (pg < lim || (dir <= 0 ? a + 2 : a + 4) < pg) {
                d2_SetViewTextures(*p, ch_EmptyName, ch_EmptyName);
                d2_EnableViewObjects(*p, 0);
            } else {
                lim = (pg & 3) + 1;
                sprintf(top, "Buch_Seite%i_oben_ns_nm", lim);
                sprintf(bottom, "Buch_Seite%i_unten_ns_nm", lim);
                d2_SetViewTextures(*p, top, bottom);
                d2_EnableViewObjects(*p, 1);
            }
            pg++;
            p++;
        } while (pg < (int)*((char *)bk + 0x271));
    }
    if (dir <= 0)
        d3t_PlayAnim(bk[0], "buch\\Buch_zurueckblaettern.baf", 0x12);
    else
        d3t_PlayAnim(bk[0], "buch\\Buch_vorblaettern.baf", 0x12);
    *(char *)(bk + 0x9c) = (char)bk[0x9c] + dir;
    sv_Unk52f340(bk);
    sb_PlaySample(snd_BookBank, "Umblaettern_s", 1, 0x3f, 0x7f, 0, 0);
    return 1;
}
/* 0x52f9f0 / 0x52fa00, turn the book forward / backward by two pages */

void sv_Unk52f9f0(int *bk)
{
    sv_Unk52f710(bk, 2);
}
/* 0x52fa00 */

void sv_Unk52fa00(int *bk)
{
    sv_Unk52f710(bk, -2);
}

extern int ai_Unk623ea0, ai_Unk623ea4, ai_Unk623ea8;  /* 0x623ea0, 0x623ea4, 0x623ea8 */
/* 0x52fa10, creates the scroll parchment form "misc\scroll_perga" (0x623ea0) with its two end pieces (0x623ea4 / 0x623ea8, picture 0x6e0) in the current form; returns the form or -1 */
int sv_CreateScrollForm(void)
{
    if (ai_Unk623ea0 != -1)
        return -1;
    ai_Unk623ea0 = d2_OpenForm(0, 0, "misc\\scroll_perga");
    d2_CentreViewWindows(ai_Unk623ea0);
    if (ai_Unk623ea0 == -1)
        return -1;
    d2_SetForm(ai_Unk623ea0, 0);
    ai_Unk623ea4 = d2_WinOpenPic(0, 0, 0x6e0, d2_LastWindow);
    ai_Unk623ea8 = d2_WinOpenPic(0, 0x1d1, 0x6e0, d2_LastWindow);
    return ai_Unk623ea0;
}
/* 0x52fa90, destroys the scroll form */

void sv_DestroyScrollForm(void)
{
    if (ai_Unk623ea0 != -1) {
        d2_CloseView(ai_Unk623ea0);
        ai_Unk623ea0 = -1;
        ai_Unk623ea4 = -1;
        ai_Unk623ea8 = -1;
    }
}
/* 0x52fad0, shows the scroll and sets the animation frame (the clock 0x6ed980 +0x248 / 6 % 12) of both end pieces */

void sv_Unk52fad0(void)
{
    int f;

    if (SV_I((unsigned)&ai_Unk623ea0) != -1) {
        d2_SetForm(SV_I((unsigned)&ai_Unk623ea0), 1);
        f = (*(int *)(SV_I((unsigned)&d2_LastWindowPtr) + 0x248) / 6) % 0xc;
        if (f < 0)
            f = 0;
        SV_WI(SV_I((unsigned)&ai_Unk623ea4), 0x78) = f;
        SV_WI(SV_I((unsigned)&ai_Unk623ea8), 0x78) = f;
    }
}
/* 0x52fb50, sets 0x6f22a0 to the distance (from the top of the 0x75f780 stack of 8-byte records, 0x6f229c = top) to the record whose first word is v */

void sv_PopTo(int v)
{
    int i;

    i = sv_FrameDepth;
    if (sv_FrameDepth >= 0) {
        while (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + i * 8) != v) {
            i--;
            if (i < 0)
                return;
        }
        sv_PopRequest = sv_FrameDepth - i;
    }
}
/* 0x52fb80, pumps the window messages of 0x6f2588 (the message struct 0x75ec40); WM_QUIT sets 0x6f25b4; returns 1 */

int sv_PumpMessages(void)
{
    int r;

    r = PeekMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0, 0);
    while (r != 0) {
        if (GetMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0) == 0)
            SV_I((unsigned)&main_QuitProgram) = 1;
        else {
            TranslateMessage((MSG *)(unsigned)&full_data_75ec40);
            DispatchMessageA((MSG *)(unsigned)&full_data_75ec40);
        }
        r = PeekMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0, 0);
    }
    return 1;
}

extern int ai_Unk623ec4, ai_Unk623ec8;  /* 0x623ec4, 0x623ec8 */
extern int ai_Unk60d360;  /* 0x60d360 */
int cm_SendExSysMessage(unsigned char id, void *msg);                     /* 0x4c4db0 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
void d2_PlaceMainWindow();                     /* 0x475850 */
void d2_SetObjValue();                     /* 0x475cb0 */
void d2_SetObjFont();                     /* 0x4760b0 */
int io_PrintText(char *fmt, ...);                     /* 0x515960 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetObjMax();                     /* 0x475f40 */
int d2_GetObjValue();                     /* 0x475980 */
void d2_FocusInput();                     /* 0x478320 */
void ai_Unk4c5aa0(void *p);                     /* 0x4c5aa0 */
int sv_Unk52fe10(void);

/* 0x52fc10, the network sync wait screen: sends the "sv_NetworkSync" message (0x4c4db0 (0xe, text)) and runs the message loop (0x5309a0 (0, fl | 0x300000)) until the answer arrives (0x4c3ae0); after 1.5 s
   (0x5dc ms) it shows the waiting dialog (0x521900 (0x143a3a8 = the text)); fl is the key mask kept in 0x760638 */
void sv_NetworkSyncWait(unsigned fl)
{
    unsigned t0;
    char msg[0x7c];
    int m, dlg;

    t0 = timeGetTime();
    memset(msg, 0, sizeof(msg));
    strcpy(msg, "sv_NetworkSync");
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    sv_FrameMask = fl;
    dlg = sv_OpenMessageBox(ai_Unk143a3a8, 0);
    d2_SetViewEnabled(dlg, 0);
    while (cm_GetSlotState(m) == 0) {
        if (t0 + 0x5dc < timeGetTime())
            d2_SetViewEnabled(dlg, 1);
        sv_RunFrame(0, fl | 0x300000);
    }
    d2_CloseView(dlg);
}
/* 0x52fcf0, sends the text t as a network message (0x4c4db0 (0xe, copy)) and shows it as the status line (0x52e330) until the answer arrives; clears the status line afterwards */

void sv_BroadcastText(unsigned fl, char *t)
{
    char msg[0x7c];
    int m;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, t);
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, fl | 0x300000);
    }
    sv_SetStatusText(ch_EmptyName);
}
/* 0x52fd80, like 0x52fcf0 with the key mask 0x60d360 | 0x210000 and no clearing of the status line */

void sv_Unk52fd80(char *t)
{
    char msg[0x7c];
    int m;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, t);
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, ai_Unk60d360 | 0x210000);
    }
}
/* 0x52fe00, the three network shutdown steps */

void sv_PumpNet(void)
{
    cm_FlushSendQueue();
    cm_ReceiveMessages();
    cm_ExecCommands();
}
/* 0x52fe10, the chat console form "misc\chatconsole" (also the message loop callback of 0x5309a0 (0x52fe10, 0x657c7)): one button per human player (the characters 0x770bd4 stride 0x21c with kind 7 at -0x52;
   the buttons remember their state in 0x623ebc), the input line (0x475b80 form text) with the "$<n>FF " colour prefix and the player's text, sent through 0x4c5aa0; ROUGH */

int sv_Unk52fe10(void)
{
    int ids[16];
    char line[0x60];
    int i, f, ed, w, *p, r, k;
    char *s;

    for (i = 0; i < 8; i++)
        ids[i] = -1;
    ai_Unk623ec4 = 0;
    f = d2_OpenForm(0, 0, "misc\\chatconsole");
    d2_PlaceMainWindow(f, 3);
    d2_SetForm(f, 3);
    i = 0;
    p = (int *)((unsigned)&ai_Unk770b89 + 0x4b);
    do {
        if (*(char *)((int)p - 0x52) == 7) {
            w = d2_WinOpenPic(i * 0x30 + 100, 4, *p, d2_LastWindow);
            ids[i] = w;
            SV_WI(w, 0x48) = 1;
            SV_WI(w, 0x2e8) = (int)(p - 0x15);
            d2_SetObjValue(w, *(char *)((unsigned)&full_data_623ebc + i));
            i++;
        }
        p += 0x87;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk770b89 + 0x653f9) + 0x52));
    w = d2_WinCreateSlider(0, 0, 0xc5, SV_I((unsigned)&full_data_14414c8), d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    d2_SetForm(f, 2);
    ed = d2_GetWinObj(f, 2, io_PrintText(0x1cc3, ch_EmptyName));
    d2_SetObjMax(ed, 0x60);
    r = sv_RunFrame(sv_Unk52fe10, 0x657c7);
    do {
        if (r == 0) {
            for (i = 0; i < 8; i++) {
                if (ids[i] != -1)
                    *(char *)((unsigned)&full_data_623ebc + i) = d2_GetObjValue(ids[i]);
            }
            d2_CloseView(f);
            *(char *)(unsigned)&kb_Last = 0;
            return 0;
        }
        d2_SetForm(f, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText((char *)(unsigned)&ai_Unk75fba0);
        if (SV_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1)
            SV_I((unsigned)&sv_PopRequest) = 1;
        d2_FocusInput(ed);
        if ((SV_I((unsigned)&d2_ClickedId) != -1 || *(char *)(unsigned)&kb_Last == 0x1c) && (d2_PointerObj == w || *(char *)(unsigned)&kb_Last == 0x1c)) {
            for (k = 0; k < 8; k++) {
                if (ids[k] != -1 && d2_GetObjValue(ids[k]) != 0)
                    ids[k + 8] = *(int *)(*(int *)(SV_I((unsigned)&d2_Objs) + 0x2e8 + ids[k] * 0x2ec) + 4);
            }
            sprintf(line, "$%iFF ", SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c) - 0x59a);
            s = (char *)d2_GetObjValue(ed);
            strcat(line, s);
            strcat(line, "$A");
            d2_SetObjValue(ed, ch_EmptyName);
            ai_Unk4c5aa0(ids + 8);
            SV_I((unsigned)&sv_PopRequest) = 1;
        }
        r = sv_RunFrame(sv_Unk52fe10, 0x657c7);
    } while (1);
}
/* 0x530100, shows / hides the quick chat strip "misc\quickchat" (0x623ec8): visible while a chat line is fresh (0x623ec4 + 0x23a ms) and the console 0x52fe10 is not the top of the 0x75f780 stack */

void sv_Unk530100(void)
{
    if (SV_I((unsigned)&ai_Unk75f280 + 0x500 + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)sv_Unk52fe10
            || SV_U((unsigned)&ai_Unk623ec4) <= SV_U((unsigned)&ai_ClockMs) - 0x23a
            || SV_U((unsigned)&ai_Unk623ec4) == 0xffffffff) {
        if (ai_Unk623ec8 != -1) {
            d2_CloseView(ai_Unk623ec8);
            ai_Unk623ec8 = -1;
            return;
        }
    } else {
        if (ai_Unk623ec8 == -1) {
            ai_Unk623ec8 = d2_OpenForm(0, 0, "misc\\quickchat");
            d2_PlaceMainWindow(ai_Unk623ec8, 3);
            if (ai_Unk623ec8 == -1)
                return;
        }
        d2_SetForm(ai_Unk623ec8, 0);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText((char *)(unsigned)&ai_Unk75fba0);
    }
}

struct D3TSNOW; void d3t_SetSnow(struct D3TSNOW *snow, int n, ...);                     /* 0x414c60 */
struct D3TRAIN; void d3t_SetRain(struct D3TRAIN *rain, int n, ...);                     /* 0x413d30 */
struct D3SKYLAYER; void d3sky_SetLayerTex(struct D3SKYLAYER *l, char *name);                     /* 0x45d9f0 */
void d3sky_FadeLayer(struct D3SKY *s, struct D3SKYLAYER *l, unsigned char alpha, float time);   /* 0x45d9a0 */
void d3sky_SetLayerSpeed(struct D3SKY *s, struct D3SKYLAYER *l, float speed);        /* 0x45d960 */
void d3t_DrawRain();                     /* 0x414590 */
void d3t_DrawSnow();                     /* 0x415380 */
struct D3OBJECT *d3t_CreateSunRays();                     /* 0x417190 */
void d3t_FadeOutSunRays();                     /* 0x417150 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
extern unsigned ai_Unk623ecc;

/* 0x5301a0, the weather of the current hour h (the clock 0x770ae4): the rain / snow emitters (0x76061c, 0x760618) get the cloud counter of the hour (0x760488, 24 counters), the wind strength follows the three
   hours around h, and the sky layers 0x760620 / 0x760624 (fading in and out 0x45d9a0) get a new sky model of the group "Sky_Schoen_01 .. " (0x6238a8, 0x4c bytes per entry) / "Sky_Mittel_01" 0x6239d8 /
   "sky_schwer_01" 0x623ac0 chosen by the strongest of the three counters (< 0x32, < 0x96, more); the float operands of the emitter calls are lost in the pseudocode (placeholders 0); ROUGH */
void sv_Unk5301a0(void)
{
    float f1, f2, w;
    int h, c, k, cl;
    char *name;
    unsigned u;
    int *cnt;

    cnt = (int *)sv_Weather.clouds;
    SV_I((unsigned)&sv_WeatherHour) = SV_US((unsigned)&ai_TimeHour);
    h = SV_I((unsigned)&sv_WeatherHour);
    if (SV_I((unsigned)&d3t_Snow) != 0)
        d3t_SetSnow(SV_I((unsigned)&d3t_Snow), 1, 1, cnt[h], 1000);
    if (SV_I((unsigned)&d3t_Rain) != 0) {
        u = cnt[h];
        if (SV_I((unsigned)&d3t_Snow) != 0) {
            if ((u & 1) == 0)
                u = 0;
            else
                u = (int)u / 5;
        }
        d3t_SetRain(SV_I((unsigned)&d3t_Rain), 1, 1, u, 1000);
    }
    f1 = (float)((int *)sv_Weather.windX)[h];
    f2 = (float)((int *)sv_Weather.windY)[h];
    c = cnt[(h + 0x17) % 0x18];
    if (c <= cnt[h])
        c = cnt[h];
    if (c <= cnt[(h + 1) % 0x18])
        c = cnt[(h + 1) % 0x18];
    if (SV_I((unsigned)&d3t_Snow) != 0)
        d3t_SetSnow(SV_I((unsigned)&d3t_Snow), 1, 2, (int)(int)0.0f);
    if (SV_I((unsigned)&d3t_Rain) != 0)
        d3t_SetRain(SV_I((unsigned)&d3t_Rain), 1, 2, (int)(int)0.0f);
    if (SV_I((unsigned)&d3_Sky) != 0) {
        w = (float)(c + 0x96) * (float)sqrt(f1 * f1 + f2 * f2) * 0.125f;
        if (*(char *)(d3_SkyLayers[1] + 0x26) == 0) {
            if (c < 0x32) {
                k = util_RandomU(4) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[1] + 0x1c), sv_SkyFair[k].name) == 0)
                    k++;
                name = sv_SkyFair[k].name;
            } else if (c < 0x96) {
                k = util_RandomU(3) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[1] + 0x1c), sv_SkyMedium[k].name) == 0)
                    k++;
                name = sv_SkyMedium[k].name;
            } else {
                k = util_RandomU(2) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[1] + 0x1c), (char *)(unsigned)&sv_SkyHeavy + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&sv_SkyHeavy + k * 0x4c;
            }
            d3sky_SetLayerTex(d3_SkyLayers[1], name);
            *(int *)d3_SkyLayers[1] = *(int *)(name + 0x40);
            *(int *)(d3_SkyLayers[1] + 0x18) = *(int *)(name + 0x44);
            *(int *)(d3_SkyLayers[1] + 0x14) = *(int *)(name + 0x48);
            d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), d3_SkyLayers[1], 0xff, 1500.0f);
        }
        if (*(char *)(d3_SkyLayers[1] + 0x26) == -1) {
            if (c < 0x32) {
                k = util_RandomU(4) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[0] + 0x1c), sv_SkyFair[k].name) == 0)
                    k++;
                name = sv_SkyFair[k].name;
            } else if (c < 0x96) {
                k = util_RandomU(3) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[0] + 0x1c), sv_SkyMedium[k].name) == 0)
                    k++;
                name = sv_SkyMedium[k].name;
            } else {
                k = util_RandomU(2) & 0xffff;
                if (_strcmpi(*(char **)(d3_SkyLayers[0] + 0x1c), (char *)(unsigned)&sv_SkyHeavy + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&sv_SkyHeavy + k * 0x4c;
            }
            d3sky_SetLayerTex(d3_SkyLayers[0], name);
            *(int *)d3_SkyLayers[0] = *(int *)(name + 0x40);
            *(int *)(d3_SkyLayers[0] + 0x18) = *(int *)(name + 0x44);
            *(int *)(d3_SkyLayers[0] + 0x14) = *(int *)(name + 0x48);
            d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), d3_SkyLayers[1], 0, 1500.0f);
        }
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), d3_SkyLayers[0], w * 0.75f);
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), d3_SkyLayers[1], w);
        if (c == 0) {
            u = 0;
            w = 2500.0f;
        } else {
            u = ((c << 6) / 1000) & 0xff | 0x60;
            w = 1000.0f;
        }
        d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), d3_SkyLayers[2], u, w);
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), d3_SkyLayers[2], w * 1.5f);
    }
}
/* 0x530670, the thunder / sun glow update of an hour: stops the emitters (0x414590 / 0x415380), a random thunder (sound "DonnerMittel_s", 0x760548 time, 0x76054c length) when the hour's flag 0x7604e8 is set, and
   the light flash 0x417190 / 0x417150 around the hours 11 .. 17 when 0x633444 is 0; ROUGH */

void sv_Unk530670(void)
{
    unsigned short hh;
    int v;

    if (SV_I((unsigned)&d3t_Rain) != 0)
        d3t_DrawRain(SV_I((unsigned)&d3t_Rain));
    if (SV_I((unsigned)&d3t_Snow) != 0)
        d3t_DrawSnow(SV_I((unsigned)&d3t_Snow));
    hh = SV_US((unsigned)&ai_TimeHour);
    if (((int *)sv_Weather.storm)[hh] != 0 && ((short)util_RandomU(300) == 0 || (((int *)sv_Weather.storm)[SV_US((unsigned)&ai_TimeHour)] == 3 && (short)util_RandomU(100) == 0))) {
        sv_Weather.thunderStart = SV_I((unsigned)&ai_ClockMs);
        sv_Weather.thunderLen = (util_RandomU(8) & 0xffff) + 8;
        sb_PlaySample(snd_ThunderBank, "DonnerMittel_s", 1, 0x3f, (int)0.0f);
    }
    if (SV_I((unsigned)&d3_CurrentScene) == 0) {
        if (10 < SV_US((unsigned)&ai_TimeHour) && SV_US((unsigned)&ai_TimeHour) < 0x12 && *(int *)((unsigned)&sv_SeasonClimate + SV_US((unsigned)&ai_TimeHour) * 4) != 0
            && ((int *)sv_Weather.clouds)[SV_US((unsigned)&ai_TimeHour)] == 0 && 0x1e < (int)((unsigned)SV_US(((unsigned)&ai_Time + 0x6)) | ((unsigned)SV_US(((unsigned)&ai_Time + 0x8)) << 16))) {
            hh = (unsigned short)util_RandomU(0x80);
            if (0x60 < hh && SV_I((unsigned)&d3t_SunRays) == 0)
                d3t_CreateSunRays();
            ai_Unk623ecc = SV_US((unsigned)&ai_TimeHour);
        }
        if (ai_Unk623ecc != 0xffffffff && (int)ai_Unk623ecc < (int)SV_US((unsigned)&ai_TimeHour) && 0x1e < (int)((unsigned)SV_US(((unsigned)&ai_Time + 0x6)) | ((unsigned)SV_US(((unsigned)&ai_Time + 0x8)) << 16))) {
            if (SV_I((unsigned)&d3t_SunRays) != 0)
                d3t_FadeOutSunRays(SV_I((unsigned)&d3t_SunRays), 1);
            ai_Unk623ecc = 0xffffffff;
        }
    }
}
/* 0x5307f0, the "advance game" callback / screen (form "misc\advancegame", 0x6f3608 = 0x2ee, 0x5f1290 = 4.9375 the game speed): runs the message loop 0x5309a0 (0x5307f0, mask) while it returns non zero and
   restores the speed (0x6f3608 * 0.00625 + 0.25) afterwards */

void sv_Unk5307f0(void)
{
    unsigned mask;
    int sp, r, f;

    mask = SV_U((unsigned)&sv_FrameMask);
    sp = SV_I((unsigned)&ai_OptionSpeed);
    if (ai_Unk5763c0(8) != 0) {
        ai_Unk576040(0xb, 0, 0, 0);
        mask = mask & 0xffffc3e7 | 0xc0;
        SV_I((unsigned)&full_data_6f22ac) = 1;
        SV_I((unsigned)&ai_OptionSpeed) = 0x2ee;
        *(float *)(unsigned)&ch_Unk5f1290 = 4.9375f;
        f = d2_OpenForm(0, 0, "misc\\advancegame");
        d2_CentreViewWindows(f);
        io_PrintText(0x82);
        r = sv_RunFrame(sv_Unk5307f0, mask);
        while (r != 0) {
            if (*(char *)(unsigned)&kb_Last != 0 || SV_I(((unsigned)&dm_L + 0xc)) != 0 || SV_I(((unsigned)&dm_L + 0x20)) != 0)
                SV_I((unsigned)&sv_PopRequest) = 1;
            r = sv_RunFrame(sv_Unk5307f0, mask);
        }
        SV_I((unsigned)&full_data_6f22ac) = 0;
        *(float *)(unsigned)&ch_Unk5f1290 = (float)sp * 0.00625f + 0.25f;
        SV_I((unsigned)&ai_OptionSpeed) = sp;
        d2_CloseView(f);
        ai_Unk576040(0xb, 0, 1, 0);
    }
}
/* 0x530900, sets the screen centre 0xf8f08c / 0xf8f088: with mask bit 0x40 the centre of the 3D viewport
   (d3_ViewW * 0.5f + d3_ViewX, d3_ViewH * 0.5f + d3_ViewY), shifted by -6 / +10 screen-scale pixels while the
   left button is held (mask bit 0x100); else half the screen size (0xfb9072 / 0xfb9070). Read from
   0x530900..0x530992: the two floats at 0xfe73c8 / 0xfe73c4 times the 0.5f at 0x5eb7ac, plus the view origins
   0xfe73a8 / 0xfc5328, and the 6.0f (0x5ebb1c) / 10.0f (0x5ebb68) terms times d2_ScreenScale (0x5fe708) in
   the bit 0x100 arm. The ftol of the x float is hoisted above the y computation by the original's scheduler. */

void sv_SetScreenCenter(unsigned fl)
{
    if ((fl & 0x40) != 0) {
        int w, h;

        if ((fl & 0x100) != 0) {
            w = (int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX
                      - *(float *)(unsigned)&ai_Unk5fe708 * 6.0f);
            h = (int)(*(float *)(unsigned)&d3_ViewH * 0.5f + *(float *)(unsigned)&ai_Unk5fe708 * 10.0f
                      + *(float *)(unsigned)&d3_ViewY);
        } else {
            w = (int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX);
            h = (int)(*(float *)(unsigned)&d3_ViewH * 0.5f + *(float *)(unsigned)&d3_ViewY);
        }
        SV_I((unsigned)&d2_HalfScreenW) = w;
        SV_I((unsigned)&d2_HalfScreenH) = h;
        return;
    }
    SV_I((unsigned)&d2_HalfScreenW) = (int)SV_S((unsigned)&d2_ScreenW) >> 1;
    SV_I((unsigned)&d2_HalfScreenH) = (int)SV_S((unsigned)&d2_ScreenH) >> 1;
}

int cm_PartyById();                     /* 0x4e50a0 */
void main_GameLoop(int ctl);                     /* 0x53bb20 */
void sv_UpdateClock(void);

/* 0x5309a0, "MASTER": one frame of the game screen loop, called by every screen with its own callback (the top of the callback stack 0x75f780, 8 bytes per entry, depth 0x6f229c; 0 = nothing to register) and the
   mask of the parts to run (param_2: 1 input, 4 resize, 0x10 objects, 0x20 speech, 0x40 world, 0x100 left click, 0x1000 names, 0x2000 main buttons, 0x4000 weather, 0x10000 paused, 0x100000 modal, ..); returns 1
   while the caller's screen is the top screen, 0 when it must end. ROUGH (the float operands of the last mouse block are lost) */
int sv_RunFrame(int cb, unsigned mask)
{
    int r, top, c, i, it;
    char cv;
    unsigned u5;
    char name[0x100], tmp[0x104];
    int *p;

    SV_U((unsigned)&sv_FrameMask) = mask;
    sv_PumpMessages();
    di_GetMouse();
    if (cb != 0) {
        if (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) != cb)
            di_ClearButtons();
        if (ai_PlayerId != 0xffff) {
            i = ai_PlayerId * 0x21c;
            if ((SV_B(((unsigned)&ai_Chars + 0x2) + i) == 6 || SV_B(((unsigned)&ai_Chars + 0x2) + i) == 7) && SV_B(((unsigned)&ai_CharInts + 0x4) + i) == 0 && SV_I((unsigned)&sv_FrameBase) < SV_I((unsigned)&sv_FrameDepth)
                && (it = cm_PartyById(SV_I((unsigned)&ai_Unk770d8c + ai_PlayerId * 0x21c))) != 0 && *(char *)(it + 8) == 8 && (mask & 0x10000) == 0) {
                SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase);
                sv_SetStatusText(SV_C((unsigned)&full_data_143a3e4));
            }
        }
    }
    if (SV_I((unsigned)&main_Inactive) != 0)
        return 1;
    cv = (char)mask;
    if ((mask & 4) != 0 && (d2_MouseUpdate(), SV_I((unsigned)&main_Inactive) != 0))
        SV_I((unsigned)&ai_FrameWait) = 1;
    if (SV_I((unsigned)&sv_PopRequest) == 0 && (mask & 0x100) != 0 && (mask & 0x100000) == 0)
        sv_Unk52d940(mask);
    if (cb == 0) {
        r = 1;
    } else {
        if (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == cb) {
            r = 1;
        } else {
            if (SV_I((unsigned)&sv_PopRequest) == 0) {
                p = (int *)((unsigned)&ai_Unk75f280 + 0x500);
                r = 1;
                do {
                    if (*p == cb) {
                        err_Message("MASTER(): Function already registrated!");
                        goto l21;
                    }
                    p += 2;
                } while ((int)p < (unsigned)((unsigned)ai_Unk75f280 + 0x900));
                SV_I((unsigned)&sv_FrameDepth)++;
                SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) = cb;
                goto l64;
            }
l21:
            r = 0;
        }
        if (SV_I((unsigned)&sv_PopRequest) != 0 && r != 0) {
            SV_I((unsigned)&sv_PopRequest)--;
            if (SV_I((unsigned)&sv_FrameDepth) != 0) {
                SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) = 0;
                SV_I((unsigned)&sv_FrameDepth)--;
            }
            r = 0;
        }
    }
l64:
    if ((mask & 1) != 0 && SV_I((unsigned)&ai_GameOver) == 0) {
        if ((mask & 0x100) != 0)
            he_Unk53daf0();
        he_Unk53edf0();
        if (SV_I((unsigned)&ai_Unk1460ae0) == -1 && SV_I((unsigned)&ai_Unka687e4) < SV_I((unsigned)&ai_ClockMs) && SV_B((unsigned)&ai_GameState) != 0)
            ai_Unk4c4d40((unsigned)&ai_TimeCopy);
        if ((mask & 0x10000) == 0 && SV_B((unsigned)&ai_GameState) != 0 && SV_I((unsigned)&sv_PanelFlag) == 0 && ai_Unk4e4cc0() != 0 && SV_B((unsigned)&full_data_6f22ac) != 0)
            SV_I((unsigned)&sv_PopRequest) = 1;
    }
    if ((mask & 0x20000) != 0 && SV_I((unsigned)&ai_Unka687e4) < SV_I((unsigned)&ai_ClockMs)) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        SV_I((unsigned)&ai_Unka687e4) = SV_I((unsigned)&ai_ClockMs) + 7;
    }
    if (r == 0 || SV_I((unsigned)&sv_PanelFlag) != 0)
        SV_I((unsigned)&ai_FrameWait)++;
    if ((mask & 0x200) != 0)
        evt_StepAll();
    if (SV_I((unsigned)&ai_FrameWait) == 0 && (cv >= 0 || (d2_FlushDirty(), SV_I((unsigned)&ai_FrameWait) == 0)) && (mask & 0x40) != 0 && SV_I((unsigned)&full_data_623eb0) != 0) {
        if ((mask & 0x10000) == 0 && (mask & 0x40000) != 0)
            sv_UpdateSkyPhase((unsigned)&ai_Time);
        if (SV_I((unsigned)&d3_CurrentScene) == 0)
            d3q_Mark(SV_I((unsigned)&d3_QuadTree), SV_I((unsigned)&sv_CameraObj), SV_I((unsigned)&d3_FrameCount));
        d3t_UpdateCamera();
        d3if_ClearView();
    }
    c = SV_I((unsigned)&ai_PlayerBuilding);
    if (c == 0)
        c = SV_I((unsigned)&ai_FallbackBuilding);
    u5 = mask & 0x8000;
    if (u5 == 0 && SV_B((unsigned)&ai_GameState) != 0)
        sv_CollectGroupCharacters((char *)c);
    if (c == 0 || (mask & 0x10000) != 0) {
        SV_I((unsigned)&ch_CurrentGroup) = -1;
    } else {
        sv_Unk52b500((unsigned char *)c);
        sv_UpdateMemberList(c);
        SV_I((unsigned)&ch_CurrentGroup) = *(int *)(c + 1);
    }
    if (SV_B((unsigned)&ai_GameState) != 0) {
        ai_UpdateWind();
        unk_Unk4edf80();
    }
    if (SV_I((unsigned)&full_data_623eac) != 0) {
        ch_UpdatePositions();
        ch_Update();
        ch_Unk4027b0();
    }
    if ((mask & 0x4000) != 0 && SV_US((unsigned)&ai_TimeHour) != SV_I((unsigned)&sv_WeatherHour))
        sv_Unk5301a0();
    if (SV_I((unsigned)&full_data_6f2580) != 0)
        snd_UpdateRain();
    if (SV_I((unsigned)&snd_SfxEnabled) != 0) {
        ai_Unk4c0e60();
        snd_UpdateQueue();
    }
    if (SV_I((unsigned)&snd_MusicEnabled) != 0 && (mask & 0x40000) != 0)
        ai_Unk4c2000();
    if (SV_I((unsigned)&ai_FrameWait) == 0
        && ((SV_I((unsigned)&ai_Unk633738) == 0 || (mask & 0x4000) == 0 || (mask & 0x40) == 0 || (sv_Unk530670(), SV_I((unsigned)&ai_FrameWait) == 0))
            && ((mask & 0x10000) == 0 || SV_I((unsigned)&ai_Unk6f0dbc) == 0 || (d3t_DrawRain(SV_I((unsigned)&ai_Unk6f0dbc)), SV_I((unsigned)&ai_FrameWait) == 0)) && SV_I((unsigned)&ai_Unk760440) != 0)) {
        if (u5 == 0)
            sv_Unk52eee0();
        else
            sv_Unk52f0d0();
        sv_Unk52f2c0();
    }
    if (SV_I((unsigned)&snd_SfxEnabled) != 0) {
        d3snd_Update();
        if (SV_I((unsigned)&full_data_6f2580) != 0)
            d3sndw_Update();
        sb_Update();
    }
    if (SV_I((unsigned)&sv_PopRequest) == 0 && (mask & 0x100000) == 0) {
        if (u5 == 0)
            sv_Unk52a680();
        if ((mask & 0x10) == 0) {
            if (SV_I(((unsigned)&dm_L + 0x28)) != 0)
                goto l2;
            if (d2_PointerObj == -1 || (SV_B(SV_I((unsigned)&d2_Objs) + 0x1c0 + d2_PointerObj * 0x2ec) & 0x40) == 0)
                d2_CopyCaption(ch_EmptyName);
            SV_I((unsigned)&ai_Unk6f22e8) = 0;
            SV_I((unsigned)&full_data_6f22f0) = 0;
            if (u5 == 0)
                d2_SetPointerSprite(0, 0);
        }
        if (SV_I(((unsigned)&dm_L + 0x28)) == 0) {
            if ((mask & 0x10) != 0)
                sv_Unk52ae70();
            if ((mask & 0x20) != 0) {
                sv_Unk52bec0();
                sv_Unk52c5c0();
            }
        }
    }
l2:
    if ((mask & 0x400) != 0)
        ai_Unk587e00();
    if (SV_I((unsigned)&ai_FrameWait) == 0 && cv < 0 && d2_LockSurface(d2_ScreenSurf) != 0) {
        if (sv_PanelPic != -1 && (mask & 0x100) != 0 && SV_I((unsigned)&full_data_623eb4) != 0)
            sv_UpdateCityPanel(2, mask);
        if (SV_I((unsigned)&full_data_62457c) != 0)
            d2_DrawObjects();
        d2_UnlockSurface(d2_ScreenSurf);
        d2_DrawFades();
    } else if (SV_I((unsigned)&sv_PanelFlag) != 0)
        d2_FinishFades();
    di_PollMouse();
    if (SV_I((unsigned)&di_Absolute) != 0) {
        SV_S((unsigned)&d2_MouseX) = SV_S((unsigned)&dm_P);
        SV_S((unsigned)&d2_MouseY) = SV_S(((unsigned)&dm_P + 0x2));
    }
    sv_SetScreenCenter(mask);
    if (SV_I((unsigned)&ai_FrameWait) == 0 && cv < 0) {
        if (d2_LockSurface(d2_ScreenSurf) != 0) {
            if (SV_I((unsigned)&ai_Unk6f0da4) != 0)
                ai_Unk4d8b50();
            if ((mask & 0x1000) != 0 && SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase) && SV_I((unsigned)&full_data_6f262c) == 0) {
                sv_Unk52cef0();
                if (SV_B(((unsigned)&kb_Down + 0x1d)) == 0) {
                    if (SV_B(((unsigned)&kb_Down + 0x38)) != 0)
                        sv_Unk52d2a0();
                } else
                    sv_Unk52d1e0();
            }
            if ((mask & 0x80000) == 0) {
                if (SV_I((unsigned)&sv_PanelShown) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
                    sv_Unk52d480();
            } else
                ai_Unk4cf370();
            if ((mask & 0x1000) != 0 && SV_I(((unsigned)&dm_L + 0x28)) == 0 && SV_I((unsigned)&ai_Unk6f22b4) == 0 && SV_I((unsigned)&d3t_CamMove) == 0 && SV_I((unsigned)&ai_Unk760440) == 0 && SV_I((unsigned)&d2_CutsceneActive) == 0 && SV_I((unsigned)&full_data_6f262c) == 0)
                sv_Unk52e050();
            sv_Unk52e0c0();
            sv_DrawPanelPointer();
            d2_UnlockSurface(d2_ScreenSurf);
        }
        dd_Flip();
    }
    if (SV_B((unsigned)&kb_Last) == 1 && (mask & 0x2000) != 0) {
        msx_SetVolume((int)0.0f);                /* the float operands of this block are lost in the pseudocode */
        sb_SetMasterVolume((int)0.0f);
        ai_OptionsMenu();
        msx_SetVolume((int)0.0f);
        sb_SetMasterVolume((int)0.0f);
    }
    sv_Unk530100();
    if (SV_B((unsigned)&ai_GameState) != 0) {
        if ((SV_U((unsigned)&ai_GameFlags) & 4) != 0 && SV_I((unsigned)&d2_FocusedInput) == 0 && SV_B((unsigned)&kb_Last) == 0x1c && (mask & 0x2000) != 0)
            sv_Unk52fe10();
        if (SV_B((unsigned)&ai_GameState) != 0) {
            if ((SV_B((unsigned)&kb_Last) == 0xcf || (SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d2_HitUnderPointer) == ai_Unk623850)) && (mask & 0x10000) == 0 && SV_B((unsigned)&full_data_6f22ac) == 0 && (SV_U((unsigned)&ai_GameFlags) & 4) == 0) {
                if (SV_I((unsigned)&ai_Unk6f2448) == 0)
                    sv_Unk5307f0();
                else {
                    SV_I((unsigned)&full_data_6f25a8) = 1;
                    sv_PopTo((int)main_GameLoop);
                }
            }
            if (SV_B((unsigned)&ai_GameState) != 0) {
                if (SV_B((unsigned)&sv_MenuOpen) == 0 && (SV_U((unsigned)&ai_GameFlags) & 4) == 0 && u5 == 0 && SV_B((unsigned)&kb_Last) == '9')
                    ai_PauseGame();
                if (SV_B((unsigned)&ai_GameState) != 0 && (mask & 0x300000) == 0)
                    ai_Unk589cf0();
            }
        }
    }
    if (sv_PanelPic != -1 && (mask & 0x2000) != 0 && (mask & 0x100000) == 0 && SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase)) {
        if (SV_B((unsigned)&ai_GameState) != 0)
            ai_Unk589c90();
        if (ai_PlayerId == 0xffff || SV_B(((unsigned)&ai_CharInts + 0x4) + ai_PlayerId * 0x21c) == 0) {
            if (SV_I((unsigned)&d2_ClickedId) != -1
                && (d2_PointerObj == ai_Unk623848 || d2_PointerObj == ai_Unk623824 || d2_PointerObj == ai_Unk62382c || d2_PointerObj == ai_Unk623830 || d2_PointerObj == ai_Unk623828
                    || d2_PointerObj == ai_Unk623820))
                sv_SetStatusText(SV_C((unsigned)&full_data_143a3e4));
        } else if (SV_I((unsigned)&d2_ClickedId) != -1) {
            if (d2_PointerObj == ai_Unk623848)
                ai_Unk5c8640();
            else if (d2_PointerObj == ai_Unk623824)
                ai_Unk5c82f0();
            else if (d2_PointerObj == ai_Unk62382c)
                ai_Unk5bac10();
            else if (d2_PointerObj == ai_Unk623830)
                ai_Unk5bcc70();
            else if (d2_PointerObj == ai_Unk623828)
                ai_Unk5c7f00();
            else if (d2_PointerObj == ai_Unk623834)
                ai_MessageFilterScreen();
            else if (d2_PointerObj == ai_Unk623820)
                ai_Unk5c4150(SV_I((unsigned)&ai_CharInts + ai_PlayerId * 0x21c), 0x11, 0, 0x657c7);
        }
    }
    sv_TickFrames++;
    if ((mask & 0x10000) == 0) {
        if (SV_I((unsigned)&sv_PopRequest) == 0 && SV_I((unsigned)&sv_PanelFlag) == 0 && SV_I((unsigned)&ai_Unkf31144) != 0 && SV_I((unsigned)&full_data_6f25a8) == 0) {
l3f5:
            if ((char)SV_B((unsigned)&ai_GameFlags) >= 0) {
                SV_I((unsigned)&full_data_6f25a8) = 1;
                tb_SetFunctionOff(sv_UpdateClock, 1);
                SV_U((unsigned)&sv_FrameMask) |= 0x100000;
                sv_MessageBoxYesNo(SV_I((unsigned)&full_data_143a3c4), 0xc4);
                sv_PopTo((int)main_GameLoop);
            }
            goto l444;
        }
        if (SV_I((unsigned)&ai_Unk1460ae0) != -1 || SV_US((unsigned)&ai_TimeHour) < 0x17)
            goto l444;
        if (SV_I((unsigned)&full_data_6f25a8) == 0)
            goto l3f5;
    } else {
l444:
        if (SV_I((unsigned)&full_data_6f25a8) == 0)
            goto l473;
    }
    if (SV_I((unsigned)&sv_PopRequest) == 0 && SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)ai_GameFrame)
        SV_I((unsigned)&sv_PopRequest) = 1;
l473:
    if ((mask & 0x10000) == 0 && SV_B((unsigned)&full_data_6f23f1) == 0 && SV_I((unsigned)&ai_MissionPartner) != -1 && SV_B((unsigned)&ai_GameState) != 0) {
        SV_B((unsigned)&full_data_6f23f1) = 1;
        tb_SetFunctionOff(sv_UpdateClock, 1);
        ai_MissionCompleteSequence(0x65fff);
        tb_SetFunctionOff(sv_UpdateClock, 0);
        SV_B((unsigned)&full_data_6f23f1) = 0;
    }
    if (SV_B((unsigned)&cm_RequestedSaveName) != 0 && (SV_U((unsigned)&sv_FrameMask) & 0x200000) == 0) {
        if (SV_I((unsigned)&cm_RequestedSaveSlot) == 1)
            strcpy(name, "QUICKSAVE");
        else if (SV_I((unsigned)&cm_RequestedSaveSlot) == 0)
            strcpy(name, "AUTOSAVE");
        else
            sprintf(name, "GILDE_SAVEGAME_%i", SV_I((unsigned)&cm_RequestedSaveSlot));
        strcpy(tmp, (char *)(unsigned)&cm_RequestedSaveName);
        ai_SaveNetworkGame(name, tmp);
    }
    if (SV_I((unsigned)&ai_Unk6f0cd4) != 0 && (SV_U((unsigned)&sv_FrameMask) & 0x200000) == 0) {
        SV_I((unsigned)&ai_Unk6f0cd4)--;
        sv_BroadcastText(mask, SV_C((unsigned)&ai_Unk1441488));
    }
    if (0 < SV_I((unsigned)&ai_FrameWait))
        SV_I((unsigned)&ai_FrameWait)--;
    return r;
}
/* 0x5315c0, a callback of 0x42ea10 (the screens' wait loops): while the clock is shown by a game speed (0x6f3608: seconds = (speed * 0.00625 + 0.5) * 0x6248b4) it stores that time of day (hours / minutes /
   seconds) in the clock record 0x75e130 (0x4fe140); skipped in the day phase 0x17 and later unless the flag bit 7 of 0x6f23f8 is set; a start Ghidra merged into 0x5309a0 */

void sv_UpdateClock(void)
{
    int t, h, m;

    if ((SV_US((unsigned)&ai_Unk75e134) >= 0x17 || SV_US((unsigned)&ai_TimeHour) >= 0x17) && (SV_B((unsigned)&ai_GameFlags) & 0x80) == 0)
        return;
    t = (int)(((float)SV_I((unsigned)&ai_OptionSpeed) * 0.00625f + 0.5) * SV_I((unsigned)&full_data_6248b4));
    h = t / 3600;
    m = (t - h * 3600) / 60;
    gm_AddTime((unsigned)&ai_TimeCopy, h, m, t - (m + h * 60) * 60);
}

extern int sv_SkyScene;
extern float sv_SkyScale;
extern int ai_Unk760440;
extern int sv_SkyPhase;
extern int sv_CloudChance;
extern int sv_SeasonClimate;
extern int sv_WinterWeather;
extern int sv_WeatherHour;
extern int d3t_Snow;
extern int d3_SkyLayers[6];
extern int d2_HitWindow;
extern int ai_Unkfb9060;
extern int ai_Unkfb9064;
extern int ai_Unkfb9068;
extern int ai_Unkfb906c;
extern int ai_Unkfba9c0[168];
/* ai_Unk143a3a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f68: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f6c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f70: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f74: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f78: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f7c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144adb0: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_Unk5fcc78 = 1128792064;  /* 0x5fcc78 */
int ai_Unk5fcc7c = 1128792064;  /* 0x5fcc7c */
int ai_Unk5fcc80 = 1128792064;  /* 0x5fcc80 */
int d2_PointerObj = -1;  /* 0x5fe710 */
float sv_FramesPerTick = 30.0f;  /* 0x62378c */
int sv_TickFrames = 30;  /* 0x623790, the frames drawn since the last timer tick (counted by the frame loop) */
char sv_GroundplanColours[] = "\377\376\005\005%\377\377/\021\005\377\030\3746\343\377\231\005\005\377\366z\005\005\266W\377<\005P\031H\005\013\304^\305\005g\357\234\231\005\010r";  /* 0x623794, the table of */
int sv_StatusTime = -1;  /* 0x6237c4, when the status line was set (d3_Time) */
int ai_Unk6237cc = 1;  /* 0x6237cc */
/* 0x6237d0, the camera limits: three settings of (height, angle), first the near view pair and then (+3 pairs)
   the far one. ai_ApplyGfxOptions (0x5d5c40) copies the pair of the camera_limits option (ai_GfxFloor.byte[1])
   into sv_CameraHeightNear .. sv_CameraAngleFar; setting 0 has the same near and far pair (no zoom) */
float sv_CameraLimits[12] = {
    1200.0f, -0.9f,
    650.0f, -0.471238911f,
    190.0f, -0.104719758f,
    1200.0f, -0.9f,
    1800.0f, -1.08210409f,
    2400.0f, -1.04719758f,
};  /* 0x6237d0 */
float sv_CameraHeightNear = 450.0f;  /* 0x623800 */
float sv_CameraAngleNear = -0.471238911f;  /* 0x623804 */
float sv_CameraHeightFar = 1600.0f;  /* 0x623808 */
float sv_CameraAngleFar = -1.08210409f;  /* 0x62380c */
float sv_ScrollSpeed = 1.0f;  /* 0x623818 */
char ai_Unk62381c = 1;  /* 0x62381c */
int ai_Unk623820 = -1;  /* 0x623820 */
int ai_Unk623824 = -1;  /* 0x623824 */
int ai_Unk623828 = -1;  /* 0x623828 */
int ai_Unk62382c = -1;  /* 0x62382c */
int ai_Unk623830 = -1;  /* 0x623830 */
int ai_Unk623834 = -1;  /* 0x623834 */
int ai_Unk623838 = -1;  /* 0x623838 */
int ai_Unk62383c = -1;  /* 0x62383c */
int ai_Unk623840 = -1;  /* 0x623840 */
int ai_Unk623844 = -1;  /* 0x623844 */
int ai_Unk623848 = -1;  /* 0x623848 */
int ai_Unk62384c = -1;  /* 0x62384c */
int ai_Unk623850 = -1;  /* 0x623850 */
int sv_PanelPic = -1;  /* 0x623854 */
int sv_PlayerBarForm = -1;  /* 0x62385c, the player bar's form, -1 = closed */
int sv_PanelForm = -1;  /* 0x623860 */
int ai_Unk623864 = -1;  /* 0x623864 */
int ai_Unk623868 = -1;  /* 0x623868 */
int ai_Unk62386c[4] = {-1, -1, -1, -1};  /* 0x62386c, the info panel's 4 widget ids (freed by one loop: 0x62386c .. 0x62387c) */
int ai_Unk62387c = -1;  /* 0x62387c */
int ai_Unk623880 = -1;  /* 0x623880 */
int ai_Unk623884 = -1;  /* 0x623884 */
int ai_Unk623888 = -1;  /* 0x623888 */
int ai_Unk62388c = -1;  /* 0x62388c */
int ai_Unk623890 = -1;  /* 0x623890 */
int ai_Unk623894 = -1;  /* 0x623894 */
int ai_Unk623898 = -1;  /* 0x623898 */
int ai_Unk6238a4 = -1;  /* 0x6238a4 */
/* 0x6238a8, the skies of fair weather (sv_Unk5301a0 picks one below 50 clouds, cm picks one for the fair cutscene) */
SVSKYREC sv_SkyFair[4] = {
    { "Sky_Schoen_01", 3.0f, 0.8f, -0.25f },
    { "Sky_Schoen_02", 3.0f, 0.8f, -0.25f },
    { "Sky_Leicht_01", 3.0f, 0.8f, -0.25f },
    { "Sky_Leicht_02", 3.0f, 0.8f, -0.25f },
};
/* 0x6239d8, the skies of medium cloud (50 .. 149 clouds) */
SVSKYREC sv_SkyMedium[3] = {
    { "Sky_Mittel_01", 3.0f, 0.8f, -0.25f },
    { "Sky_Mittel_02", 3.0f, 0.8f, -0.25f },
    { "Sky_Mittel_03", 3.0f, 0.8f, -0.25f },
};
/* 0x623ba8, the day tables: the 6 day phases (SVDAYPHASE, 10 bytes each, sv_FillDayPhases writes them from the season's
   minutes; their int seconds sit at odd addresses, so the object is packed) and the sky key colours sv_SkyKeys (+0x38) /
   sv_SkyKeysAlt (+0xa8); one object, the last phase's 4 pad bytes are the first key's colour */
ALIGN1 char sv_DayTables[278] = { 4, 0, 64, 56, 0, 0, 0, 0, 0, 0, 6, 0, 96, 84, 0, 0, 0, 0, 0, 0, 12, 0, -64, -88, 0, 0, 0, 0, 0, 0, 16, 0, 0, -31, 0, 0, 0, 0, 0, 0, 18, 0, 32, -3, 0, 0, 0, 0, 0, 0, 19, 0, 48, 11, 1, 0, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, -18, 66, 0, 0, 15, 67, 0, 0, 90, 67, 0, 0, -81, 67, 0, 0, 85, 67, 0, 0, 52, 67, 0, 0, 32, 67, 0, -128, -69, 67, 0, 0, 127, 67, 0, 0, 78, 67, 0, 0, -10, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 57, 67, 0, 0, -84, 66, 0, -128, -1, 67, 0, 0, 34, 67, 0, 0, 1, 67, 0, 0, 69, 67, 0, 0, -93, 67, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0, -20, 67, 0, 0, -34, 66, 0, 0, 13, 67, 0, 0, 53, 67, 0, -128, -30, 67, 0, 0, 59, 67, 0, 0, 83, 67, 0, 0, 26, 67, 0, 0, -71, 67, 0, 0, 127, 67, 0, 0, 66, 67, 0, 0, -22, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 47, 67, 0, 0, 104, 66, 0, 0, -49, 67, 0, 0, 76, 67, 0, 0, 64, 67, 0, 0, 53, 67, 0, -128, -47, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0 };
int ai_Unk623cc0[] = { 1104150528, 1097859072, 1116602368, 0, 1102577664, 1107820544, 1113325568, 0, 1116209152, 1112276992, 1110179840, 0, 1120272384, 1117519872, 1109393408, 0, 1116602368, 1113587712, 1108606976, 0, 1107820544, 1102053376, 1112539136, 0, 1104150528, 1097859072, 1116602368, 0, 1122500608, 1124270080, 1132396544, 1133019136, 1121452032, 1125056512, 1128792064, 1133969408, 1123680256, 1127284736, 1126170624, 1134592000, 1124990976, 1128726528, 1130168320, 1135247360, 1126629376, 1127153664, 1126957056, 1135411200, 1124990976, 1124728832, 1128792064, 1135706112, 1122107392, 1124270080, 1132396544, 1132691456, 1124401152, 1121976320, 1132396544, 1134592000, 1122893824, 1125384192, 1125122048, 1138425856, 1123287040, 1125253120, 1125122048, 1136361472, 1125318656, 1127612416, 1128595456, 1135706112, 1124597760, 1124139008, 1125122048, 1135083520, 1125777408, 1128267776, 1129185280, 1134264320, 1124401152, 1121976320, 1132396544, 1134428160 };  /* 0x623cc0 */
int ai_Unk623e10[] = { 1099431936, 1097859072, 1116602368, 0, 1096810496, 1104674816, 1114112000, 0, 1096810496, 1102053376, 1114112000, 0, 1101004800, 1107296256, 1113325568, 0, 1101004800, 1103101952, 1114898432, 0, 1101004800, 1094713344, 1110966272, 0, 1099431936, 1097859072, 1116602368, 0 };  /* 0x623e10 */
int sv_LampPhase = -1;  /* 0x623e80 */
int ai_Unk623e88 = 1;  /* 0x623e88 */
int sv_GroundplanScene = -1;  /* 0x623e8c */
float sv_DayPhaseFrac = 0.5f;  /* 0x623e94 */
int ai_Unk623ea0 = -1;  /* 0x623ea0 */
int ai_Unk623ea4 = -1;  /* 0x623ea4 */
int ai_Unk623ea8 = -1;  /* 0x623ea8 */
int ai_Unk623ec4 = -1;  /* 0x623ec4 */
int ai_Unk623ec8 = -1;  /* 0x623ec8 */
int sv_ViewRectLeft = 0;  /* 0x6248a4 */
int sv_ViewRectTop = 0;  /* 0x6248a8 */
unsigned ai_Unk623ecc = 4294967295;  /* 0x623ecc */
int sv_ViewRectRight = 694;  /* 0x6248ac */
int sv_ViewRectBottom = 600;  /* 0x6248b0 */
int ai_Unk62a8cc = -1;  /* 0x62a8cc */
int ai_Unk631078 = 0;  /* 0x631078 */
short d2_PointerLook = 0;  /* 0x6ed9d0, the pointer direction / panel layout code (1 .. 8) */
int sv_SavedMaxY = 0;  /* 0x6f1c40 */
float sv_ScrollVelYPrev = 0;  /* 0x6f1c44 */
int sv_DragLastY = 0;  /* 0x6f1c48 */
int sv_SavedMaxX = 0;  /* 0x6f1c50 */
float sv_DragLastPos[3] = {0};  /* 0x6f1c64 */
float sv_ScrollLastMove[3] = {0};  /* 0x6f1c74 */
int sv_SavedMinX = 0;  /* 0x6f1c84 */
int sv_DragLastX = 0;  /* 0x6f1c88 */
int sv_ZoomStartY = 0;  /* 0x6f1c8c */
int sv_ZoomStartX = 0;  /* 0x6f2290 */
int sv_SavedMinY = 0;  /* 0x6f2298 */
int sv_FrameDepth = 0;  /* 0x6f229c, the depth of the frame callback stack */
int sv_PopRequest = 0;  /* 0x6f22a0 */
int sv_FrameBase = 0;  /* 0x6f22a4, the depth a screen was entered at (the base of the pop count) */
int ai_Unk6f22b4 = 0;  /* 0x6f22b4 */
int ai_Unk6f22c0 = 0;  /* 0x6f22c0 */
struct D2SURF *sv_GroundplanSurf = 0;  /* 0x6f22c4, the groundplan bitmap while sv_LoadGroundplan reads it */
struct D2SURF *sv_GroundplanView = 0;  /* 0x6f22c8, the groundplan copy the panel blits */
int ai_Unk6f22cc = 0;  /* 0x6f22cc */
int ai_Unk6f22d0 = 0;  /* 0x6f22d0 */
float sv_CameraHeightFactor = 0;  /* 0x6f22d4 */
float sv_CameraAngleFactor = 0;  /* 0x6f22d8 */
int sv_SelectedModel = 0;  /* 0x6f2304 */
char * ai_PlayerBuilding = 0;  /* 0x6f2308 */
char * ai_FallbackBuilding = 0;  /* 0x6f230c */
unsigned short * sv_PanelShown = 0;  /* 0x6f2310, the scene record (room) the info panel shows now */
int ai_Unk6f2334 = 0;  /* 0x6f2334 */
int sv_ResearchClass = 0;  /* 0x6f233c */
char sv_LampSwitches = 0;  /* 0x6f2340 */
char sv_MessageBoxOpen = 0;  /* 0x6f2350 */
char sv_MessageBoxGreenOpen = 0;  /* 0x6f2351 */
char sv_MessageBoxYesNoOpen = 0;  /* 0x6f2352 (the yes / no box) */
int sv_ScreenDirty = 0;  /* 0x6f2358 */
int sv_GroundplanBuilding = 0;  /* 0x6f235c */
int sv_GroundplanRoom = 0;  /* 0x6f2360 */
char sv_PlayerBarKind = 0;  /* 0x6f2368 */
int sv_DayPhase = 0;  /* 0x6f236c */
int sv_ThunderFog = 0;  /* 0x6f2370 */
float ai_Unk6f2374 = 0;  /* 0x6f2374 */
int ai_Unk6f2378 = 0;  /* 0x6f2378 */
float sv_ScrollVelX = 0;  /* 0x6f2388 */
float sv_ScrollVelXPrev = 0;  /* 0x6f238c */
float sv_ScrollVelY = 0;  /* 0x6f2390 */
int sv_ScrollTime = 0;  /* 0x6f2394 */
int sv_DragStartX = 0;  /* 0x6f2398 */
int sv_DragStartY = 0;  /* 0x6f239c */
int sv_ZoomTurnOn = 0;  /* 0x6f23a0 */
int sv_MouseCamActive = 0;  /* 0x6f23a4 */
float sv_ZoomStartHeight = 0;  /* 0x6f23a8 */
float sv_ZoomStartAngle = 0;  /* 0x6f23ac */
float sv_ZoomStartTurn = 0;  /* 0x6f23b0 */
float sv_ZoomBaseY = 0;  /* 0x6f23b4 */
int sv_MouseDragOn = 0;  /* 0x6f23b8 */
unsigned char ai_GameState = 0;  /* 0x6f25ac */
int ai_OptionScrollSpeed = 0;  /* 0x6f3614 */
int sv_PanelFlag = 0;  /* 0x75e8c0, 1 when the panel is up (an int; cm.h / f3init.c) */
char * sv_PanelObject = 0;  /* 0x75e8c4, the object the info panel shows */
unsigned short * sv_PanelRecord = 0;  /* 0x75e8c8, its scene record */
char sv_PanelText[76] = {0};  /* 0x75e8d4, the panel text */
int sv_SelectionFlags[0xc0] = {0};  /* 0x75e920, the mirror of the character selection flags (byte i per character) */
int sv_SkyScene = 0;  /* 0x75ec20 */
unsigned short * ai_Unk75ec5c = 0;  /* 0x75ec5c */
int ai_Unk75f280[576] = {0};  /* 0x75f280, the member panel: */
float sv_ViewPos[4] = {0};  /* 0x75fb80, the stored camera position (a padded vector): set by the camera moves, restored when the player leaves a building */
float sv_ViewRot[4] = {0};  /* 0x75fb90, its rotation (D3OBJECT.rotation) */
float sv_SkyScale = 0;  /* 0x760420 */
int ai_Unk760440 = 0;  /* 0x760440 */
int sv_SkyPhase = 0;  /* 0x760464 */
int sv_CloudChance = 0;  /* 0x760480 */
int sv_SeasonClimate = 0;  /* 0x760484 */
SVWEATHER sv_Weather = {0};  /* 0x760488 */
int sv_WinterWeather = 0;  /* 0x760610 */
int sv_WeatherHour = 0;  /* 0x760614 */
int d3t_Snow = 0;  /* 0x76061c */
int d3_SkyLayers[6] = {0};  /* 0x760620, the sky layer handles: sv creates 0 .. 2, sv and main_EndGame delete all six */
SVPLATE sv_NamePlates[0x20] = {0};  /* 0x760640, the name plate records (200 bytes each) */
char sv_StatusText[128] = {0};  /* 0x761f40, the status line text */
int sv_Bubbles[0x40][0x43] = {0};  /* 0x761fc0, the 64 speech bubble records (0x10c bytes): +0 the bubble, +4 its time
                                      stamp (ai_ClockMs), +8 its object (sv_BubbleObjects), +0xc the name */
/* sv_BubbleObjects: view of sv_Bubbles (+0x8) */
int sv_SelectedChar = 0;  /* 0x7662c0 */
int sv_GroundplanButtons[15] = {0};  /* 0x7662e0, 15 widget ids */
int cm_Bombs[0x40] = {0};  /* 0x935c00, cm's 32 bomb records (object, time), cm.h */
int d2_HitWindow = 0;  /* 0xf8aad0 */
int ai_Unkfb9060 = {0}; /* 0xfb9060 */
int ai_Unkfb9064 = {0}; /* 0xfb9064 */
int ai_Unkfb9068 = {0}; /* 0xfb9068 */
int ai_Unkfb906c = {0}; /* 0xfb906c */
int ai_Unkfba9c0[168] = {0}; /* 0xfba9c0 */
char * sv_CameraObj = {0}; /* 0xfc2dec, the camera object */
