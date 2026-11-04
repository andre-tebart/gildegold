/* sv.c -- the screen / message box unit (0x520ec0 ..): the tag "sv_" ("sv_ChangePlayerHE" 0x624120, "sv_HandleObjects" 0x6242c4, "f3_sv:OpenBook:b" 0x624428, "sv_NetworkSync"), the message boxes ("misc\\Messagebox" 0x60ea68,
   "misc\\MessageBoxGreen" 0x623ed0), the groundplan bitmaps, the city / time panels, the pause / player bar, the book. Starts at 0x520ec0 (a tick callback, registered by 0x520ee0); ends before
   0x531670, main.c's program level code. Names are ours.
   Open: the start and the end rest on the .data order only.
   Open: sv.c includes ai.h but not d2.h yet: its char * declarations of d2_Objs / d2_LastWindowPtr and its K&R
   d2_BlitPartMode supersede d2.h's, so the d2 records go through the SV* views and the SV_I / SV_W address macros.
   This blocks Q4 / Q7 of every function that touches widgets or calls a K&R d2 / gfx prototype.
   Open: head: include/gfx.h, include/d2.h unsettled (d2_Objs / d2_LastWindowPtr char *, d2_AddHitRect and
   d2_BlitPartMode K&R; 39 K&R d2 prototypes whose calls need a re-grade); also not included: cm.h (the same
   d2_Objs / d2_LastWindowPtr clash; the cm_SendChr* / cm_PartyById calls stay without a prototype), f3snd.h rt.h sblib.h
   (d2_Wins is declared in the head as D2WIN[], the D2OBJ fields through ((D2OBJ *)d2_Objs)[i]).
   Calls whose argument count differs from the definition (read the original's pushes): sb_PlaySample (sblib.h:
   6 calls to re-grade), d2_SetObjValue. The
   @original cast macros (d2_PutPixelRGBc, d2_CreatePicObjS, d2_WinOpenPicStyledS, d2_OpenFormS, d2_SetObjValue4,
   ai_SwitchRoomI, cm_SendExSetHEI, d2_PlaceMainWindowF) record the parameter types the original's calls show.
   Open: data: 0x6f23f1 sv_InMissionComplete, owner sv (only sv_RunFrame uses it; it follows sv_BookPhase) but kept
   typed in main.c's gap block. ai_GameState, main_GameEnded and sv_MenuOpen (0x6f25ac..ae) lie in main's block
   (0x6f25a4..): whose unit they are is open.
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
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern void *d2_ScreenSurf;  /* 0x6ed924 */

#include "unk_5d33b0_types.h"
#include "ai_types.h"
extern OPTIONBYTES ai_OptionPanelHelp;
extern char ai_PlayerSlots[4][0x2f4];
extern int d2_HitUnderPointer;
extern short d2_ShpH;
extern short d2_ShpW;

extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern D2WIN d2_Wins[];
extern GAPDATA full_data_623e90[];
extern GAPDATA full_data_6f2364[];
extern void *d2_ScreenSurf;  /* 0x6ed924 */

extern char * ai_PickedObject;
extern int ai_PickedChar;
extern char d2_CaptionCopy[0x80];
extern int d2_HitUnderPointer;
extern int d2_PointerSprite;



extern char * ai_PickedObject;
extern int ai_PickedChar;
extern int d2_HitUnderPointer;
extern int ai_SceneChars[32];
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
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern int ai_PermsDirty;
extern OPTIONBYTES ai_GameSwitches;
extern char d2_CaptionCopy[0x80];
extern short d2_MouseX;
extern short d2_MouseY;
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern int d2_TextLeft;
extern int d2_TextTop;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern float d2_ScreenScale;
extern int d2_HalfScreenW;
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern float d3s_QuadUV[6];
extern struct D2PIC *d2_Pics;
extern int ai_SceneChars[32];
extern GAPDATA full_data_75ec40[];
extern char cm_ChatLog[2048];

extern GAPDATA full_data_602568[];
extern char cm_ChatLog[2048];
extern int d2_HalfScreenH;
extern int d2_HalfScreenW;

extern int cm_NoticeCount;
extern unsigned int cm_FightStart;
extern int cm_SceneRain;
extern char * ai_PickedObject;
extern int ai_TimeCopy[4];
extern int cm_NextNetStep;
extern int cm_DayEndRequest;
extern char cm_RequestedSaveName[72];
extern int cm_RequestedSaveSlot;
extern int d2_CutsceneActive;
extern struct D2INPUT *d2_FocusedInput;
extern int d2_HitUnderPointer;
extern short d2_MouseX;
extern short d2_MouseY;


extern int main_Inactive;
extern int snd_MusicEnabled;
extern int snd_SfxEnabled;
extern void *d2_ScreenSurf;  /* 0x6ed924 */
extern int ai_OptionSpeed;
extern int ai_TimeCopy[4];
extern GAPDATA full_data_6248b4[];

int d2_CreateWinText(short x, short y, char *text, int win);
int d2_WinOpenPic(short x, short y, int pic, int win);
void d2_SetObjWidth(int obj, short w);
void d2_BlitPart();
void d2_EnableObject();
void d2_RemoveObj();
void d2_SetObjFont();
void d2_SetObjWidth(int obj, short w);

struct G16CHARCLICK;  /* file-scope tag for the prototypes below */
struct AITIME;        /* f3gm.c typedef; file-scope tag for the gm_AddTime / gm_SetTime prototypes */
struct D3SURF;        /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */

extern int di_Absolute, di_MinX, di_MinY, di_MaxX, di_MaxY;  /* 0x5f3240, 0x6310b0, 0x6310b4, 0x5f3238, 0x5f323c (di_input.c) */
extern short d2_ScreenW;  /* 0xfb9072 (d2.h), the screen size */
extern short d2_ScreenH;  /* 0xfb9070 */
extern char *d2_LastWindowPtr;  /* 0x6ed980, window created last; @original: d2.h declares D2WIN *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */
extern unsigned char d2_HoverBlocked;  /* 0x6ed9c8 (d2.h) */
extern unsigned char kb_Last;  /* 0x1468410 */
/* d2_RadioSel: view of d2_Radios (tools/data_views.py) */
extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */

void d2_SetPointerSprite(int a, int pic);      /* 0x476f80, gfx.c */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
void d2_CopyCaption(char *caption);            /* 0x478870, gfx.c */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
void d2_CentreViewWindows(int view);            /* 0x475600 */
void d2_RadioAdd(int g, int obj);                     /* 0x46c560 */
void d2_RadioSelect(int g, int sel);                     /* 0x46c630 */
void d2_RaiseViewWindows(int view);                     /* 0x4741a0 */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original sv_OpenBuildingPanel pushes 4 arguments (0x528a1e), as rt.c's calls */
extern D2WIN d2_Wins[];  /* 0xf8f120, d2.h */
void d2_CloseView(int view);            /* 0x4758f0 */
/* ai_SeasonCloudChance: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_SeasonClimate: view of ai_PlayerSlots (tools/data_views.py) */
extern int sv_ViewShape;  /* 0x6f22c0 */
extern struct D2SURF *sv_GroundplanSurf, *sv_GroundplanView;  /* 0x6f22c4, 0x6f22c8 */
extern void *d2_BackSurf;  /* 0x6ed92c */
extern OPTIONBYTES ai_GamePanel;  /* 0x6f3668 */
#define d2_TransparentBytes ((unsigned char *)&d2_TransparentColour)  /* 0x6f0454, @original: sv_BuildScreenLayout clears the key byte by byte (mov byte [0x6f0454] / [0x6f0455] / [0x6f0456], bl), not through the D2KEY word gb */
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
    char name[0x40];                    /* +0x04, the object's name (sv_RegisterNamePlate finds the plate by it) */
    int look;                           /* +0x44, the pointer look while the object is hovered (sv_HoverPick reads its
                                           low word; sv_RegisterNamePlate stores it whole) */
    char text[0x80];                    /* +0x48, the plate's text, the hover caption (sv_HoverPick) */
} SVPLATE;
typedef struct SVPLATEOBJ {             /* the 3D object fields the plate code clears */
    char pad00[0x224];
    int plate;                          /* +0x224, the name plate attached to the object */
} SVPLATEOBJ;
typedef struct SVGROUP {                 /* the group record fields sv.c reads (AICHAR.job, ai.h AIJOB) */
    unsigned char kind;                 /* +0x00, the job kind (sv_MoveToGroup's switch) */
    char pad01[0x13];
    int rank1Count;                     /* +0x14, members whose title is their building type's first rank (+0x22f;
                                           sv_CountGroupMembers, or of a kind 0x1e building accepted by ahm_HasRecord) */
    int rank2Count;                     /* +0x18, members whose title is the type's second rank (+0x230) */
    char pad1c[0x36];
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
    char pad8c[0x98];
    D3OBJECT **transport;               /* +0x124, the cart object the model shows (CHARACTER.transport) */
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
void d2_RemoveObj();                     /* 0x46e710 */
int d2_FreeSurface(struct D2SURF *s);                     /* 0x478fc0 */
void d2_RemoveHitRect();                     /* 0x4788f0 */
int d2_RemoveWindow();                     /* 0x472ce0 */

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
#include "f3gm.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
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
#include "ch.h"
#include "unk_5d33b0.h"
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
    AICHAR *p;
    char buf[0x100];

    p = (AICHAR *)gm_FindCharacterByMaster(id);
    if (p != 0 && (p->flags1b4 & 1) != 0) {
        txt_Format(buf, 0x78, p->id);
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
/* 0x521410, the green message box ("misc\\MessageBoxGreen" 0x623ed0) with an optional title text a and the text b; like 0x521150, a button returns its number + 1. Not reentrant (sv_MessageBoxGreenOpen, 0x6f2351) @deep behaviour: 73 code hunks, all read: the statement set is complete and matches 0x521150's twin (form, a/b texts, timed/quick/n, do-while frame loop); the residue is allocation - the original keeps the constant 1 in ebx (cmp eax,ebx / mov ds:X,ebx / cmp BYTE PTR ds:X,bl) and hoists bar = -1 into its own frame+0x10 slot with form in ebp, ours uses immediates and puts form in edi (the first attempts at the hoist made the count worse: 73 -> 267, reverted). Readability as far as base evidence goes: sv_FitForm (0x52ea30), d2_OpenForm / d2_SetForm / d2_RadioAdd / d2_RadioSelect / d2_RaiseViewWindows / d2_WinOpenSlider / d2_RadioSel / d2_ClickedId / d2_SetObjValue, kb_Last / d3_Time / d2_LastWindowPtr / d2_Objs and sv_MessageBoxGreenOpen named, marker dropped; open: Q4 6 raw accesses need the D2WIN / D2OBJ typed views (sv.c does not include d2.h yet: its char * views of d2_Objs / d2_LastWindowPtr supersede d2.h's); Q7 d2_SetObjValue K&R (2..5 argument callers, no canonical prototype). */

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

/* 0x521c10, the progress callback of the scene load (a Ghidra miss inside the span of 0x521ba0): kind is the load
   stage, cur / total the step. The first call creates the progress slider sv_LoadSlider (d2_OpenSlider, 0x190 long,
   just below the screen centre) and blits the back surface onto the screen; kind 0 sets it to cur * 200 / total,
   kind 1 to 200 + cur * 200 / total and at the last step removes it and rebuilds the city panel; every 10th frame
   (sv_LoadFrame) the slider and the panel pointer are drawn and the screen is flipped. */
void sv_SceneLoadProgress(int kind, int cur, int total)
{
    int h;

    if (sv_LoadSlider == -1) {
        d2_CutsceneActive = 1;
        sv_LoadFrame = 0;
        h = d2_OpenSlider((int)(d3_ViewW * 0.5f + d3_ViewX - 205.0f), d2_ScreenH - 0x40, 0x190, 0, 0x190, 0x83, 0x42);
        sv_LoadSlider = h;
        d2_SetObjValue(h, 0, 0x190, 0);
        d2_BlitPart(0, 0, ((D2SURF *)d2_BackSurf)->w, ((D2SURF *)d2_BackSurf)->h, d2_ScreenSurf, 0, 0, d2_BackSurf);
        if (sv_LoadSlider == -1)
            goto lend;
    }
    if (kind == 0 || sv_LoadFrame % 10 == 0)
        d2_BlitPart(0, 0, ((D2SURF *)d2_BackSurf)->w, ((D2SURF *)d2_BackSurf)->h, d2_BackSurf, 0, 0, d2_ScreenSurf);
    if (kind == 0) {
        d2_SetObjValue(sv_LoadSlider, 0, 0x190, cur * 200 / total);
    } else if (kind == 1) {
        d2_SetObjValue(sv_LoadSlider, 0, 0x190, cur * 200 / total + 200);
        if (cur >= total - 1) {
            d2_RemoveObj(sv_LoadSlider);
            sv_LoadSlider = -1;
            d2_CutsceneActive = 0;
            if (sv_PanelPic != -1) {
                d2_ClearSurface(d2_ScreenSurf);
                d2_DrawPic(0, 0, sv_PanelPic, d2_ScreenSurf);
                d2_LockSurface(d2_ScreenSurf);
                sv_UpdateCityPanel(1, 0);
                d2_UnlockSurface(d2_ScreenSurf);
                d2_BlitPart(0, 0, ((D2SURF *)d2_BackSurf)->w, ((D2SURF *)d2_BackSurf)->h, d2_ScreenSurf, 0, 0, d2_BackSurf);
            }
            ai_FrameWait = 1;
        }
    }
    if (sv_LoadFrame % 10 == 0 && sv_LoadSlider != -1 && d2_LockSurface(d2_ScreenSurf) != 0) {
        d2_DrawSlider(sv_LoadSlider, d2_ScreenSurf);
        sv_DrawPanelPointer();
        d2_UnlockSurface(d2_ScreenSurf);
        dd_Flip();
    }
lend:
    sv_LoadFrame = sv_LoadFrame + 1;
}
/* 0x521e70, builds the main screen layout for the scene s: clears the clip and the 0x10 groundplan buttons, creates
   the player bar window sv_BarWindow (0x288 x 0x30) and its toggle button sv_BarToggle (a picture object the bar
   collapses with), redraws the background bitmap over the view rectangle sv_ViewRect* into d2_BackSurf, grabs that
   rectangle as sv_ViewShape (so the city panel can restore it), points the dirty rectangle at the view and creates
   the groundplan surface sv_GroundplanView (d2_MakeSurface, 0x68 x 0x6e). */

void sv_BuildScreenLayout(int s)
{
    int t, i;
    int tab[16];
    int *q;

    d2_SetClipRect(0, 0, (int)d2_ScreenW, (int)d2_ScreenH);
    sv_AdvanceHit = d2_AddHitRect(d2_ScreenW - 0x50, 0x11, 0x30, 0x30, 0);
    sv_BarWindow = d2_CreateWindow(4, 10, 0x288, 0x30, 0);
    d2_PlaceWindowScaled(sv_BarWindow, 1);
    if (sv_ViewShape != 0) {
        mem_Free(sv_ViewShape);
        sv_ViewShape = 0;
    }
    t = d2_GetPic(s);
    if (d2_LockSurface(d2_BackSurf) != 0) {
        d2_ClearSurface(d2_BackSurf);
        d2_DrawShape(0, 0, d2_BackSurf, t, 0);
        d2_TransparentBytes[0] = 0;
        d2_TransparentBytes[1] = 0;
        d2_TransparentBytes[2] = 0;
        sv_ViewShape = d2_GrabShape(sv_ViewRectLeft, sv_ViewRectTop, (sv_ViewRectRight - sv_ViewRectLeft) + 1, sv_ViewRectBottom - sv_ViewRectTop, d2_BackSurf, 0, 1);
        d2_ClearSurface(d2_BackSurf);
        d2_UnlockSurface(d2_BackSurf);
    }
    q = sv_GroundplanButtons;
    for (i = 0xf; i != 0; i--)
        *q++ = -1;
    d2_DirtyRectRight = sv_ViewRectRight;
    d2_DirtyRectLeft[0] = sv_ViewRectLeft;
    d2_DirtyRectTop = sv_ViewRectTop;
    q = tab;
    for (i = 0x10; i != 0; i--)
        *q++ = 0;
    sv_PanelPic = s;
    d2_DirtyRectBottom = sv_ViewRectBottom;
    tab[1] = 0x68;
    tab[2] = 0x6e;
    sv_GroundplanView = d2_MakeSurface(tab, 3);
    d2_ClearSurface(sv_GroundplanView);
    sv_BarToggle = d2_CreatePicObj(0x286, 0x23c, 0x544);
    ((D2OBJ *)d2_Objs)[sv_BarToggle].interactive = 1;
    d2_EnableObject(sv_BarToggle, 0);
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
/* 0x522100, enables (n != 0) or disables (n == 0) the panel widgets: sv_PanelEnabled = n, the building panel
   view (sv_PanelForm, twice, around the event bar via he_SetBarEnabled), the player bar's buttons and its window
   object (d2_Wins[sv_BarWindow].parent), the toggle button (off, never on), the city panel's icons and money
   text, and the 0x10 groundplan buttons. */
void sv_SetPanelEnabled(int n)
{
    int *q;

    sv_PanelEnabled = n;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    he_SetBarEnabled(n);
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    if (sv_ButtonFamilyTree != -1)
        d2_EnableObject(sv_ButtonFamilyTree, n);
    if (sv_ButtonCompare != -1)
        d2_EnableObject(sv_ButtonCompare, n);
    if (sv_ButtonCityStats != -1)
        d2_EnableObject(sv_ButtonCityStats, n);
    if (sv_ButtonInventory != -1)
        d2_EnableObject(sv_ButtonInventory, n);
    if (sv_ButtonPlayerStats != -1)
        d2_EnableObject(sv_ButtonPlayerStats, n);
    if (sv_ButtonMessages != -1)
        d2_EnableObject(sv_ButtonMessages, n);
    if (sv_PanelUnusedObj != -1)
        d2_EnableObject(sv_PanelUnusedObj, n);
    if (sv_PanelBuildingIcon != -1)
        d2_EnableObject(sv_PanelBuildingIcon, n);
    if (sv_PanelCompass != -1)
        d2_EnableObject(sv_PanelCompass, n);
    if (sv_BarToggle != -1 && n == 0)
        d2_EnableObject(sv_BarToggle, 0);
    if (sv_ButtonPrivileges != -1)
        d2_EnableObject(sv_ButtonPrivileges, n);
    if (sv_PanelMoneyText != -1)
        d2_EnableObject(sv_PanelMoneyText, n);
    if (sv_BarWindow != -1)
        d2_EnableObject(d2_Wins[sv_BarWindow].parent, n);
    q = sv_GroundplanButtons;
    do {
        if (*q != -1)
            d2_EnableObject(*q, n);
        q++;
    } while ((int)q < (int)((unsigned)&sv_GroundplanButtons + 0x3c));
}
/* 0x522280, closes the panel widgets (0x46e710, id := -1) and hides the rest */

void sv_ClosePanelWidgets(void)
{
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    if (sv_ButtonFamilyTree != -1) {
        d2_RemoveObj(sv_ButtonFamilyTree);
        sv_ButtonFamilyTree = -1;
    }
    if (sv_ButtonCompare != -1) {
        d2_RemoveObj(sv_ButtonCompare);
        sv_ButtonCompare = -1;
    }
    if (sv_ButtonCityStats != -1) {
        d2_RemoveObj(sv_ButtonCityStats);
        sv_ButtonCityStats = -1;
    }
    if (sv_PanelUnusedObj != -1) {
        d2_RemoveObj(sv_PanelUnusedObj);
        sv_PanelUnusedObj = -1;
    }
    if (sv_ButtonInventory != -1) {
        d2_RemoveObj(sv_ButtonInventory);
        sv_ButtonInventory = -1;
    }
    if (sv_ButtonPlayerStats != -1) {
        d2_RemoveObj(sv_ButtonPlayerStats);
        sv_ButtonPlayerStats = -1;
    }
    if (sv_ButtonMessages != -1) {
        d2_RemoveObj(sv_ButtonMessages);
        sv_ButtonMessages = -1;
    }
    if (sv_PanelBuildingIcon != -1) {
        d2_RemoveObj(sv_PanelBuildingIcon);
        sv_PanelBuildingIcon = -1;
    }
    if (sv_PanelCompass != -1) {
        d2_RemoveObj(sv_PanelCompass);
        sv_PanelCompass = -1;
    }
    if (sv_ButtonPrivileges != -1) {
        d2_RemoveObj(sv_ButtonPrivileges);
        sv_ButtonPrivileges = -1;
    }
    if (sv_PanelMoneyText != -1) {
        d2_RemoveObj(sv_PanelMoneyText);
        sv_PanelMoneyText = -1;
    }
    if (sv_BarToggle != -1)
        d2_EnableObject(sv_BarToggle, 0);
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
    if (sv_ViewShape != 0) {
        mem_Free(sv_ViewShape);
        sv_ViewShape = 0;
    }
    if (sv_BarToggle != -1) {
        d2_RemoveObj(sv_BarToggle);
        sv_BarToggle = -1;
    }
    if (sv_AdvanceHit != -1)
        d2_RemoveHitRect(sv_AdvanceHit);
    sv_ClosePanelWidgets();
    sv_PanelPic = -1;
    if (sv_BarWindow != -1) {
        d2_RemoveWindow(sv_BarWindow);
        sv_BarWindow = -1;
    }
    sv_ClosePanel();
    sv_PanelRebuild = 1;
}

extern char *ai_Goods;              /* 0x91aaf0 */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define sv_ClickedObjectT (*(D3OBJECT **)&ai_ClickedObject)  /* 0x6f22e4, ai_ClickedObject (cm.h: int) as the scene object picked (d3if_Pick) */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
int d2_BlitPartMode();                     /* 0x479150 */  /* @original: sv.c's call at 1099 passes 1 argument to d2.h's 8-parameter prototype */
extern int sv_ResearchClass, sv_ScreenDirty, sv_GroundplanBuilding, sv_GroundplanRoom, sv_GroundplanScene;  /* 0x6f233c, 0x6f2358, 0x6f235c, 0x6f2360, 0x623e8c */
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
    sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->name);
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
                sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->name);
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
            sprintf(name, "%sbmp\groundplans\riss_%s.bmp", d2_GfxDir, bt->name);
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
                            && !(ai_CharsT[ai_PlayerId].heldRounds > 0)) {
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
                            && !(ai_CharsT[ai_PlayerId].heldRounds > 0)) {
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
char * cm_CharWeapon(char *c);                     /* 0x4cda50 */

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
    d2_BlitPart(0, 0, ((D2SURF *)d2_BackSurf)->w, ((D2SURF *)d2_BackSurf)->h, d2_ScreenSurf, 0, 0, d2_BackSurf);
    r = d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0, 1);
    sv_RunFrame(0, 0x1c6);
    d2_ReleaseFade(r);
    d2_PlaceWindow(sv_BarWindow, 1);
    d2_BlitPart(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, d2_BackSurf, 0, 0, d2_ScreenSurf);
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
    p = ((SVGROUP *)g)->member;
    i = 8;
    do {
        if (*p != -1)
            n++;
        p++;
        i--;
    } while (i != 0);
    return n;
}
/* 0x5241d0, counts the members of the group record g by their home building: rank1Count those whose title is the
   first title rank of their building's type (+0x22f; for a kind 0x1e building, one accepted by ahm_HasRecord),
   rank2Count those of the second rank (+0x230). */

void sv_CountGroupMembers(char *g)
{
    int i;
    int *p;
    char *c;
    char *b;

    ((SVGROUP *)g)->rank1Count = 0;
    ((SVGROUP *)g)->rank2Count = 0;
    p = ((SVGROUP *)g)->member;
    i = 8;
    do {
        if (*p != -1) {
            c = (char *)gm_FindChar(*p);
            if (c != 0 && ((SVCHARREC *)c)->bld != 0) {
                b = ai_BTypes + *((SVCHARREC *)c)->bld * 0x24d;
                /* b: the building's type record (AIBTYPE, 0x24d bytes): +0x22f / +0x230 its two title ranks, +0x00 the kind */
                if (c[0x165] == b[0x22f]) {
                    ((SVGROUP *)g)->rank1Count++;
                } else if (c[0x165] == b[0x230]) {
                    ((SVGROUP *)g)->rank2Count++;
                } else if (b[0] == 0x1e) {
                    if (ahm_HasRecord(c) != 0)
                        ((SVGROUP *)g)->rank1Count++;
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
int gm_ClockSeason(AITIME *c);         /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
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
        k = gm_ClockSeason(&ai_Time);
        if ((((float)ai_Time.hour < gm_DayEndHours[k] && gm_DayStartHours[k] <= (float)ai_Time.hour) || bt->kind == 4 || bt->kind == 0x13 || bt->kind == 0x1d)
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
                old->time = ai_Time;
                old->state = -1;
            }
            break;
        case 0x14:
        case 0x15:
        case 0x8c:
            sv_CountGroupMembers((char *)old);
            if (sv_UsedGroupSlots((char *)old) <= 0) {
                old->time = ai_Time;
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
/* 0x524900, collects the state icon numbers of the character c into out[]: the icons sv_Activities holds for the
   kind of c's job (SVGROUP.kind: the entry's icon, and, when the entry's valueOff is not -1, a short of the job
   record at that offset + 0xdf), then cm_CharWeapon's image + 0xdf; returns the count. */

int sv_CharStateIcons(char *c, int *out)
{
    SVGROUP *job;
    unsigned short *word;
    int n, i, *p;
    int *q;

    job = ((SVCHARREC *)c)->group;
    n = 0;
    if (job != 0 && job->kind != 0) {
        i = 0;
        p = (int *)(unsigned)&sv_Activities[0].icon;
        for (; (int)p < (int)((unsigned)&sv_Activities[0].icon + 0x600); p += 8, i++) {
            if (p[-6] == job->kind) {
                out[n++] = *p;
                if (p[1] != -1) {
                    /* the entry's valueOff: the offset of a short of the job record (0xac / 0xae / 0xbc) */
                    word = (unsigned short *)((char *)job + ((int *)(unsigned)&sv_Activities[0].valueOff)[i * 8]);
                    out[n++] = *word + 0xdf;
                    break;
                }
            }
        }
    }
    q = (int *)cm_CharWeapon(c);
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
extern int sv_SkyScene;  /* 0x75ec20 */
extern struct D3ANIM *d3t_CamMove;  /* 0x6310e8 */
extern int sv_LampPhase;  /* 0x623e80 */
extern int sv_DayPhase;  /* 0x6f236c */
extern char sv_LampSwitches;
extern float sv_SkyScale;  /* 0x760420 */
extern float sv_DayPhaseFrac;  /* 0x623e94 */
/* ai_MapRefs: view of d3_Scenes (tools/data_views.py) */
const int sv_PhaseMinutes[24] = { 390, 510, 630, 1050, 1170, 1290, 300, 420, 540, 1140, 1260, 1380, 420, 540, 660, 1020, 1140, 1260, 480, 600, 690, 960, 1020, 1140 };  /* 0x5eeab8 */

extern int ai_Unk623cc0[], ai_Unk623e10[];  /* 0x623cc0, 0x623e10 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void d2_ClearPointerItems(void);                     /* 0x476c70 */
void sv_UpdatePanel(void);                     /* 0x52a050 */
int d2_WinOpenPicStyled();                     /* 0x473350 */
void gm_ClockToDate(struct AITIME *c, unsigned char *out);                     /* 0x4fe2c0 */
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
/* 0x525510, sets up the day's weather and sky for the clock t: the season's cloud chance and climate from the own player's record; with
   reroll the hours are rolled anew (winter weather in season 3: snow with rain at half the cloud chance, else snow; the clouds of each hour
   by the chance, their thunder above 999 out of winter, the wind rose as sin / cos of a random walk of the angle by +-0.175); the wind
   state, rain or snow by the winter weather and the strongest hour, the sky with its three layers (two fair skies and the dark one), the
   light colours of every scene with a map and the sky phase for t */
void sv_SetupWeather(struct AITIME *t, int reroll)
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
    ai_InitWind(season, ai_Time.year, sv_Weather.windX, sv_Weather.windY);
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
    d3_SkyLayers[2] = (int)d3sky_CreateLayer(d3_Sky, 0, sv_SkyHeavy[2].name, flags, sv_SkyHeavy[2].scale, 0, sv_SkyHeavy[2].vscale, sv_SkyHeavy[2].voffset);
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

void sv_FillDayPhases(struct AITIME *t)
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
/* 0x525a80, a d3_ForEachObject callback (the object o, the flag f): sets the brightness ramp of the object's light
   (0x1e8, a D3LDATA) to a random 0.3 .. 0.6 + 0.3 (f = 0) or -0.3 - 0.6 * random (f != 0); returns 1 */

char sv_SetLampRamp(char *o, int f)
{
    double v;

    if (f == 0)
        v = util_RandomF() * 0.6 + 0.3;
    else
        v = -0.3 - util_RandomF() * 0.6;
    if (D3_L(o)->light != 0)
        D3_L(o)->light->ramp = (float)v;
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

extern float sv_CameraHeightFactor, sv_CameraAngleFactor, sv_CameraHeightStep, sv_ScrollVelX, sv_ScrollVelY, sv_ScrollVelXPrev, sv_ScrollVelYPrev;  /* 0x6f22d4, 0x6f22d8, 0x6f2374, 0x6f2388, 0x6f2390, 0x6f238c, 0x6f1c44 */
extern float sv_CameraHeightNear, sv_CameraAngleNear, sv_CameraHeightFar, sv_CameraAngleFar, sv_ScrollSpeed;  /* 0x623800, 0x623804, 0x623808, 0x62380c, 0x623818 */
extern int sv_BorderMoved;  /* 0x6f2378 */
extern int sv_BorderViewX;  /* 0x6f22cc */
extern int sv_BorderViewY;  /* 0x6f22d0 */
extern int d3t_CamMoving;  /* 0x6310e4 */
extern struct D3ANIM *d3t_CamMove;  /* 0x6310e8 */
extern int sv_ScrollTime;  /* 0x6f2394 */
extern float sv_ScrollLastMove[3];
float d3t_GetMeanHeight();                   /* 0x412550 */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
void d3_MatTranspose();                     /* 0x454040 */
float d3_VecAngle2();                   /* 0x453b40 */

/* 0x525dd0, in scene 0 sets both camera blend factors to f and places the camera: its height is the near / far height blended by
   f over the terrain's mean height under it, its x angle the near / far angle blended by f; stores the camera's rotation and
   position as the view (sv_ViewRot / sv_ViewPos) */
void sv_PlaceCamera(float f)
{
    D3VECTOR4 pos;
    D3VECTOR rot;
    double h;

    if (d3_CurrentScene != 0)
        return;
    sv_CameraHeightFactor = f;
    sv_CameraAngleFactor = f;
    h = d3t_GetMeanHeight(&d3_Camera->pos);
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    pos.y = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + h);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    rot.x = (sv_CameraAngleFar - sv_CameraAngleNear) * sv_CameraAngleFactor + sv_CameraAngleNear;
    d3_SetObjectRot(d3_Camera, &rot);
    sv_ViewRot[0] = d3_Camera->rotation.x;
    sv_ViewRot[1] = d3_Camera->rotation.y;
    sv_ViewRot[2] = d3_Camera->rotation.z;
    sv_ViewPos[0] = d3_Camera->pos.x;
    sv_ViewPos[1] = d3_Camera->pos.y;
    sv_ViewPos[2] = d3_Camera->pos.z;
}
/* 0x525ee0, eases the camera's height toward the wanted one (the near / far height blended by sv_CameraHeightFactor over the
   terrain's mean height, against the camera's height with its offset): more than 5 off, a fifteenth of the difference per call,
   at most 10, moves it (not while a camera move runs); the step is kept in sv_CameraHeightStep, 0 when within 5 */

void sv_EaseCameraHeight(void)
{
    float d, step;
    D3VECTOR4 pos;

    if (d3_Camera == 0)
        return;
    d = d3t_GetMeanHeight(&d3_Camera->pos);
    d = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + d - (d3_Camera->posOffset.y + d3_Camera->pos.y);
    if (5.0 < fabs(d)) {
        step = d * 0.06666667f;
        if (step > 0.0f)
            step = step < 10.0f ? step : 10.0f;
        else if (step < 0.0f)
            step = step > -10.0f ? step : -10.0f;
        pos.x = d3_Camera->pos.x;
        pos.y = d3_Camera->pos.y;
        pos.z = d3_Camera->pos.z;
        pos.y = pos.y + step;
        if (d3t_CamMove == 0)
            d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        sv_CameraHeightStep = step;
        return;
    }
    sv_CameraHeightStep = 0.0f;
}
/* 0x525ff0, the mouse at the screen border moves the camera to one of its preset views: the border states sv_BorderViewX /
   sv_BorderViewY (-1, 0, 1) step by the mouse position against the window di_MinX .. di_MaxX / di_MinY .. di_MaxY (x: left
   views[3], right views[4]; y: above views[1], below views[2]; both 0: the saved view savedPos / savedRot), a view whose position
   is all zero is not set; the view is stored (sv_ViewPos / sv_ViewRot) and the camera moved there (d3t_PlaceObjectV), then
   sv_BorderMoved holds until the mouse is back inside the window */
void sv_BorderCameraView(void)
{
    int left, right, top, bottom;
    int y;
    D3VECTOR4 dir, pos;
    D3VECTOR4 none;

    left = di_MinX + 1;
    right = di_MaxX - 1;
    top = di_MinY + 1;
    bottom = di_MaxY - 1;
    none.x = 0.0f;
    none.y = 0.0f;
    none.z = 0.0f;
    none.w = 0.0f;
    if (sv_BorderMoved != 0) {
        if (dm_L.x >= left && dm_L.x <= right && dm_L.y >= top && dm_L.y <= bottom)
            sv_BorderMoved = 0;
        return;
    }
    y = dm_L.y;
    if (y > top && y < bottom) {
        if (dm_L.x < left) {
            if (sv_BorderViewX <= -1)
                return;
            sv_BorderViewX--;
        } else {
            if (dm_L.x <= right)
                return;
            if (sv_BorderViewX >= 1)
                return;
            sv_BorderViewX++;
        }
        sv_BorderViewY = 0;
    } else {
        if (y < top) {
            if (sv_BorderViewY <= -1)
                return;
            sv_BorderViewY--;
        } else if (y > bottom) {
            if (sv_BorderViewY >= 1)
                return;
            sv_BorderViewY++;
        }
        sv_BorderViewX = 0;
    }
    if (sv_BorderViewX == -1) {
        if (d3_VecEqual(&d3_Camera->views[3].pos, (D3VECTOR *)&none, 0.2f) != 0)
            return;
        pos.x = d3_Camera->views[3].pos.x;
        pos.y = d3_Camera->views[3].pos.y;
        pos.z = d3_Camera->views[3].pos.z;
        dir.x = d3_Camera->views[3].dir.x;
        dir.y = d3_Camera->views[3].dir.y;
        dir.z = d3_Camera->views[3].dir.z;
    } else if (sv_BorderViewX == 0 && sv_BorderViewY == 0) {
        pos.x = d3_Camera->savedPos.x;
        pos.y = d3_Camera->savedPos.y;
        pos.z = d3_Camera->savedPos.z;
        dir.x = d3_Camera->savedRot.x;
        dir.y = d3_Camera->savedRot.y;
        dir.z = d3_Camera->savedRot.z;
    } else if (sv_BorderViewX == 1) {
        if (d3_VecEqual(&d3_Camera->views[4].pos, (D3VECTOR *)&none, 0.2f) != 0)
            return;
        pos.x = d3_Camera->views[4].pos.x;
        pos.y = d3_Camera->views[4].pos.y;
        pos.z = d3_Camera->views[4].pos.z;
        dir.x = d3_Camera->views[4].dir.x;
        dir.y = d3_Camera->views[4].dir.y;
        dir.z = d3_Camera->views[4].dir.z;
    } else if (sv_BorderViewY == -1) {
        if (d3_VecEqual(&d3_Camera->views[1].pos, (D3VECTOR *)&none, 0.2f) != 0)
            return;
        pos.x = d3_Camera->views[1].pos.x;
        pos.y = d3_Camera->views[1].pos.y;
        pos.z = d3_Camera->views[1].pos.z;
        dir.x = d3_Camera->views[1].dir.x;
        dir.y = d3_Camera->views[1].dir.y;
        dir.z = d3_Camera->views[1].dir.z;
    } else if (sv_BorderViewY == 1) {
        if (d3_VecEqual(&d3_Camera->views[2].pos, (D3VECTOR *)&none, 0.2f) != 0)
            return;
        pos.x = d3_Camera->views[2].pos.x;
        pos.y = d3_Camera->views[2].pos.y;
        pos.z = d3_Camera->views[2].pos.z;
        dir.x = d3_Camera->views[2].dir.x;
        dir.y = d3_Camera->views[2].dir.y;
        dir.z = d3_Camera->views[2].dir.z;
    }
    sv_ViewPos[0] = pos.x;
    sv_ViewPos[1] = pos.y;
    sv_ViewPos[2] = pos.z;
    sv_ViewRot[0] = dir.x;
    sv_ViewRot[1] = dir.y;
    sv_ViewRot[2] = dir.z;
    d3anim_FreePath(d3_Camera);
    d3t_CamMoving = 0;
    d3t_CamMove = 0;
    d3t_PlaceObjectV(d3_Camera, (D3VECTOR *)&pos, (D3VECTOR *)&dir, ai_CameraStep, 8);
    sv_BorderMoved = 1;
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
    const float *ax = (const float *)&d3_ZAxis;

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
            a = -d3_VecAngle2((char *)&d3_ZAxis, fwd);
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
extern int sv_BandActive;  /* 0x760440 */
extern int ai_DragWindow;  /* 0x62a8cc */
extern int sv_CameraScrolled;  /* 0x6f22b4 */
extern int sv_FrameDepth;  /* 0x6f229c */
extern int sv_FrameBase;  /* 0x6f22a4 */
extern char * sv_SelectedObject;  /* 0x75ec60 */
extern int sv_SelectedModel;  /* 0x6f2304 */
extern char sv_CameraEaseOn;
int d3t_GetHeightRange();                     /* 0x411c70 */
short * gm_PlayerRecord(char *ch);                   /* 0x506430 */
void sv_PlaceCamera(float f);
void sv_EaseCameraHeight(void);
int sv_ScrollCamera(void);
int sv_MouseCamera(void);

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
    const float *ax = (const float *)&d3_ZAxis;

    if (d3t_CamMoving != 0)
        return 0;
    my = dm_P.y;
    cam = (char *)d3_Camera;
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
                    ang = -d3_VecAngle2((char *)&d3_ZAxis, fwd);
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
/* 0x527640, the per-frame camera update: with a camera, not paused, no rubber band and no item drag: the keyboard / border
   scroll (sv_ScrollCamera, unless the right button is held; its result is sv_CameraScrolled), the mouse control (unless bit
   0x8000 of sv_FrameMask), the height easing (sv_CameraEaseOn and key 0xf up) and, when the camera scrolled at the base frame
   without a selected building, the player's camera record (position, rotation, angle factor); then the camera's rotation and
   position become the view (sv_ViewRot / sv_ViewPos). Returns sv_CameraScrolled. Without a camera it puts the mouse back to
   the absolute screen window once and returns 0 */
int sv_UpdateCamera(void)
{
    AIFIN *rec;

    if (d3_Camera != 0 && tb_Paused == 0) {
        if (sv_BandActive == 0 && ai_DragWindow == -1) {
            if (dm_L.rhold == 0)
                sv_CameraScrolled = sv_ScrollCamera();
            if ((sv_FrameMask & 0x8000) == 0)
                sv_MouseCamera();
            if (sv_CameraEaseOn != 0 && kb_Down[0xf] == 0)
                sv_EaseCameraHeight();
            if (sv_CameraScrolled != 0 && sv_FrameDepth == sv_FrameBase && ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0
                && (rec = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId])) != 0) {
                SV_CAMREC(rec);
                rec->camAngle = sv_CameraAngleFactor;
            }
            sv_ViewRot[0] = d3_Camera->rotation.x;
            sv_ViewRot[1] = d3_Camera->rotation.y;
            sv_ViewRot[2] = d3_Camera->rotation.z;
            sv_ViewPos[0] = d3_Camera->pos.x;
            sv_ViewPos[1] = d3_Camera->pos.y;
            sv_ViewPos[2] = d3_Camera->pos.z;
        }
        return sv_CameraScrolled;
    }
    sv_CameraScrolled = 0;
    if (di_Absolute == 0) {
        di_MinX = 0;
        di_MinY = 0;
        di_MaxX = d2_ScreenW - 8;
        di_MaxY = d2_ScreenH;
        di_Absolute = 1;
        di_SetMousePos((int)dm_L.x, (int)dm_L.x);
    }
    return 0;
}
/* 0x527840, moves the camera (animated, d3t_PlaceObjectV) to 600 units back along its view from the building
   b's 3D object, like sv_CameraToObject: the move takes the camera step ai_CameraStep scaled by the distance
   (0.0005 per unit), at least one step and at most three; the stored view sv_ViewPos / sv_ViewRot and the
   player's camera record get the target position, b becomes sv_SelectedObject
   @deep read against the original (238 vs 238 ins): no code hunk; frame 0x8c vs the original's 0x9c (16
   bytes more locals in the original, a slot layout question) and slot hunks at 0x527963 / 0x52798b /
   0x527ae3..0x527b07: the final sv_ViewPos / sv_ViewRot stores take the same slots in another order
   (references: M8 at +0x2b0: ours refers to sv_ViewPos+0x4 (0x75fb84), the original's field to 0x75fb80
   (sv_ViewPos, sv_ViewPosX) (and 3 more), explained: the four M8 lines pair reordered stores crosswise:
   ours ViewPos+0/+4/+8 <- [esp+0x20/0x24/0x28], ViewRot+0/+4/+8 <- [esp+0x34/0x38/0x3c]; the original
   stores the same slots to the same fields (ViewPosX/Y/Z, ViewRotY/Z) in another order (recorded as debt
   at 15bda65)) */

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
    const float *ax = (const float *)&d3_ZAxis;
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

/* 0x528180, the d3_ForEachObject callback of 0x5282f0 over the model parts of the building b: for the part named
   "dummy_FAHNE" it creates the banner object "sp_WIMPEL" in b's model scene at the dummy's world position, turned by pi
   around the y axis, coloured for the owner like 0x528290 (texture set 'f' + the owner's ref54 byte, kind 4), starts its
   animation "sonstiges\sp_WIMPEL.baf" and sets its flags (stateFlags 0x44 on, 8 off; objectFlags 2 off); returns 1 */
char sv_CreateBannerCB(D3OBJECT *part, AIBLD *b)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3MATRIX m;
    D3OBJECT *o;

    if (_stricmp(part->name, "dummy_FAHNE") == 0) {
        rot.x = 0;
        rot.y = 0;
        rot.z = 0;
        d3_PointToWorld(part, &part->pos, (D3VECTOR *)&pos);
        o = d3_CreateObject((int)b->object, (D3VECTOR *)&pos, &rot, "sp_WIMPEL");
        rot.x = 0;
        rot.y = 3.1415927f;
        rot.z = 0;
        d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
        d3_SetWorldPosRot(o, &pos, &m);
        if (b->owner != 0xffff) {
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, (unsigned char)((char)ai_CharsT[b->owner].ref54 + 'f'), 1);
            o->kind = 4;
        }
        d3t_PlayAnim(o, "sonstiges\\sp_WIMPEL.baf", 1);
        o->stateFlags = (o->stateFlags & 0xf7) | 0x44;
        o->objectFlags &= 0xfd;
    }
    return 1;
}
/* 0x527bb0, puts the camera directly 600 units back along its view from the building b's 3D object (its centre,
   the terrain's mean height plus the zoom height, pulled in by the object's height range): the stored view
   sv_ViewPos / sv_ViewRot and the player's camera record get the new position, b becomes sv_SelectedObject;
   sv_FlyCameraToObject does the same with an animated move
   @deep read against the original (188 vs 188 ins): no code hunk; frame 0x88 vs the original's 0x94 and 19
   slot hunks (locals laid out 4 bytes apart, temporaries at 0xb4 / 0xc4 in the original), the final
   sv_ViewPos / sv_ViewRot stores load the same slots in another order (0x527d86..0x527db7) (references: M8
   at +0x1ed: ours refers to sv_ViewPos (0x75fb80), the original's field to 0x75fb98 (sv_ViewRotZ) (and 2
   more), explained: the three M8 lines pair reordered stores crosswise: ours ViewPos <- [esp+0x40],
   ViewRot+4 <- [esp+0x54], ViewRot+8 <- [esp+0x58]; the original ViewPosX <- [esp+0x40], ViewRotY <-
   [esp+0x54], ViewRotZ <- [esp+0x58]: the same values to the same fields (recorded as debt at 15bda65)) */
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
    const float *ax = (const float *)&d3_ZAxis;
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
    const float *ax = (const float *)&d3_ZAxis;
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
/* 0x528140, moves the camera (0x411680) to the stored view (sv_ViewPos / sv_ViewRot) with the step ai_CameraStep / 8 */

void sv_MoveCameraToView(void)
{
    d3t_PlaceObject(d3_Camera, sv_ViewPos[0], sv_ViewPos[1], sv_ViewPos[2], sv_ViewRot[0], sv_ViewRot[1], sv_ViewRot[2], ai_CameraStep, 8);
}
/* 0x528290, an object callback (the object o, the building b): for the model part named "sp_WIMPEL" (the banner)
   sets its texture set to 'f' + the low byte of its owner's character ref54 (the owner's colour) when the owner
   is valid; returns 1 */

char sv_ColourBannerCB(D3OBJECT *o, AIBLD *b)
{
    if (_stricmp(o->name, "sp_WIMPEL") == 0) {
        if (b->owner != 0xffff)
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, (unsigned char)((char)ai_CharsT[b->owner].ref54 + 'f'), 1);
    }
    return 1;
}
/* 0x5282f0, colours the banner of the building b (+0x61 its model) for a character owner (type 5 / 6 / 7, +0x27): the model's parts are walked (0x438d40, 0x100, the callback 0x528180) with the model's +0x1f0 cleared
   when its flag +0x210 & 1 is not set */

void sv_CreateBanners(char *b)
{
    AIBLD *bld;
    D3OBJECT *m;
    D3OBJECT *save;
    char c;

    bld = (AIBLD *)b;
    m = (D3OBJECT *)bld->object;
    if (m != 0 && bld->owner != 0xffff) {
        c = ai_CharsT[bld->owner].kind;
        if (c == 6 || c == 7 || c == 5) {
            if ((m->state & 1) == 0) {
                save = m->next;
                m->next = 0;
                d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)bld->object, 0x100, sv_CreateBannerCB, b);
                ((D3OBJECT *)bld->object)->next = save;
                return;
            }
            d3_ForEachObject(d3_DefaultScene, m, 0x100, sv_CreateBannerCB, b);
        }
    }
}
/* 0x528390, colours the banners and carts of the character c (index): in scene 0 the model of every building of c
   (gm_BuildQuery(1, 4, c)) is walked with sv_ColourBannerCB; then each cart of c (gm_Query kind 0x1d) shown by a scene
   character gets, in that character's scene, the texture set ref54 - 0x599 (when c is a kind 5 / 6 / 7 character with a
   ref54 and the set is not 0); the current scene is restored */
void sv_ColourBanners(int c)
{
    int scene;
    AICHAR *ch;
    AIBLD *b;
    AIOBJ *o;
    SVMODEL *m;
    int set;

    scene = d3_CurrentScene;
    ch = &ai_CharsT[c];
    d3if_SelectScene(0, 1);
    for (b = (AIBLD *)gm_BuildQuery(1, 4, c); b != 0; b = (AIBLD *)gm_BuildNext()) {
        if (b->object != 0)
            d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)b->object, 0x340, sv_ColourBannerCB, b);
    }
    for (o = (AIOBJ *)gm_Query(0, 3, 7, 3, c, 4, 0x1d); o != 0; o = (AIOBJ *)gm_QueryNext()) {
        m = (SVMODEL *)o->sceneObj;
        if (m != 0) {
            d3if_SelectScene(d3t_SceneIndex(m->scene), 1);
            if (ch != 0 && ch->ref54 != 0 && (ch->kind == 5 || ch->kind == 6 || ch->kind == 7)) {
                set = ch->ref54 - 0x599;
                if (set != 0)
                    d3if_SelectTextureSet(*m->transport, (struct D3SURF *)(*m->transport)->info, set, 1);
            }
        }
    }
    d3if_SelectScene(scene, 1);
}
/* 0x528490, the chimney smoke of the building b (not of kind 7, with a model): when its first worker (a living character
   of job kind 4 in b, not of kind 10) has no job or a job without argc8, and the hour lies inside the season's day hours
   (gm_DayStartHours .. gm_DayEndHours), the smoke effect "effekte\Schornstein_dunkel_esc" is started at the model's
   "dummy_RAUCH_0" dummy (in scene 0) unless one runs; its id is kept in b->smokeEffect and its group cleared. Otherwise a
   running smoke effect is stopped and b->smokeEffect set to -1
   @deep read whole against 0x528490..0x52865c and rewritten typed (AIBLD.smokeEffect 0x95, AIJOB.argc8,
   AICHAR walk, EVTSCRIPT id/group): the type-7 / model test, the worker scan with the index i reused for
   ai_Chars[i].job, the smokeEffect / day-hour window (one fcom/fcomp: hour >= start && hour < end, fixed
   from two returns), the effect start in scene 0 and the stop branch all match (143 vs 144 instructions).
   Left is allocation in the scan loop: ours keeps the constant 0xffff in esi (cmp word ptr [eax-2], si;
   reused for the hour mask: and ecx, esi) and the walking pointer biased to +2 (cmp byte ptr [eax], 0xa)
   with health loaded into bl (mov bl,[eax+6]; test bl,bl), the original keeps 0xa in bl (mov bl, 0xa; cmp
   byte ptr [eax+2], bl), compares cmp byte ptr [eax+8], 0 / cmp word ptr [eax], 0xffff directly and masks
   with the immediate; season in edi vs esi. Tried: pointer loop with (int) compare, pointer-first, index
   loop, health != 0, (short)id != -1 (all equal or worse) */
void sv_UpdateChimneySmoke(AIBLD *b)
{
    int season, i, scene;
    AICHAR *ch;
    AIJOB *job;
    D3OBJECT *d;
    EVTSCRIPT *e;
    D3VECTOR4 pos;

    season = gm_ClockSeason(&ai_Time);
    if (ai_BTypesT[b->type].kind != 7 && b->object != 0) {
        for (i = 0; i < 0x300; i++) {
            ch = &ai_CharsT[i];
            if (ch->health > 0 && ch->id != 0xffff && ch->kind != 10 && ch->bld == b && ch->jobKind == 4) {
                job = ai_CharsT[i].job;
                if (job == 0 || job->argc8 == 0) {
                    if (b->smokeEffect != -1)
                        return;
                    if (!((float)ai_Time.hour >= gm_DayStartHours[season] && (float)ai_Time.hour < gm_DayEndHours[season]))
                        return;
                    scene = d3_CurrentScene;
                    d3if_SelectScene(0, 1);
                    d = d3_FindObject((int)b->object, 0x100, 0, "dummy_RAUCH_0");
                    if (d != 0 && (e = evt_Load("effekte\\Schornstein_dunkel_esc")) != 0) {
                        d3_PointToWorld(d, &d->pos, (D3VECTOR *)&pos);
                        if (evt_RunScriptArgs(e, 3, (int)pos.x, (int)pos.y, (int)pos.z) != 0)
                            b->smokeEffect = e->id;
                        e->group = 0;
                    }
                    d3if_SelectScene(scene, 1);
                    return;
                }
                break;
            }
        }
        if (b->smokeEffect != -1) {
            e = evt_FindById(b->smokeEffect);
            if (e != 0)
                evt_Stop(e);
            b->smokeEffect = -1;
        }
    }
}
/* 0x528660, a callback collecting the names o that contain "sp_WIMPEL" into the list p (p[0] the count, then the pointers); 1 while below 0x20 */

char sv_CollectBannerCB(char *o, int *p)
{
    if (strstr(o, "sp_WIMPEL") != 0) {
        p[p[0] + 1] = (int)o;
        p[0]++;
    }
    return p[0] < 0x20;
}

extern int d3_QuadTree, sv_PanelBldPic, sv_PanelHealthBar, sv_PanelCartWin, sv_PanelItemBtn, sv_PanelTransportBtn, sv_PanelCartBtn, sv_PanelActionBtn, sv_PanelCartChar;  /* 0x6f1c28, 0x623864, 0x623868, 0x62387c, 0x623888, 0x62388c, 0x623890, 0x623894, 0x631078 */
extern int sv_PanelWidgets[4];  /* 0x62386c */

int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjFont();                     /* 0x4760b0 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);                     /* 0x46a960 */
int sv_AddPanelButton(int x, int y, int text);

/* 0x5286a0, tints the banners of the building b: its model's parts named "sp_WIMPEL" (0x438d40, 0x40, the collector 0x528660; the model's +0x1f0 cleared when its flag +0x210 & 1 is not set) are recoloured
   (0x458780 with the colour 0x6f1c28, when set) and updated (0x447190) */
void sv_RemoveBanners(char *b)
{
    int list[33];
    int i;
    AIBLD *bld;
    D3OBJECT *m;
    D3OBJECT *save;

    bld = (AIBLD *)b;
    m = (D3OBJECT *)bld->object;
    if (m != 0) {
        for (i = 0; i < 0x21; i++)
            list[i] = 0;
        if ((m->state & 1) == 0) {
            save = m->next;
            m->next = 0;
            d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)bld->object, 0x40, sv_CollectBannerCB, list);
            ((D3OBJECT *)bld->object)->next = save;
        } else
            d3_ForEachObject(d3_DefaultScene, m, 0x40, sv_CollectBannerCB, list);
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

void sv_RefreshBanners(void)
{
    int scene;
    AIBLD *o;
    AICHAR *c;
    char t;

    scene = d3_CurrentScene;
    c = 0;
    d3if_SelectScene(0, 1);
    o = (AIBLD *)gm_BuildQuery(1, 6);
    while (o != 0) {
        if (o->owner != 0xffff)
            c = &ai_CharsT[o->owner];
        if (c != 0 && (char *)c != ai_PlayerCharC && ((t = c->kind) == 6 || t == 7 || t == 5) && (o->flags5a & 1) == 0) {
            sv_RemoveBanners((char *)o);
            sv_CreateBanners((char *)o);
        }
        o = (AIBLD *)gm_BuildNext();
    }
    d3if_SelectScene(scene, 1);
}
/* 0x528800, creates a panel button at (x + 3, y) from the image 0xc5 (0x46bf10) with the text, the state 0x54, the flag +0x5c = 1 and the frame 0x60; returns the widget id */

int sv_AddPanelButton(int x, int y, int text)
{
    int w;

    w = d2_WinCreateSlider(x + 3, y, 0xc5, text, d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    *(int *)(d2_Objs + 0x5c + w * 0x2ec) = 1;
    d2_SetObjWidth(w, 0x60);
    return w;
}
/* 0x528860, opens the building info panel (sv_PanelForm, "panel\infopanel_gebaeude") for the building b unless one is open:
   the title (kind 0x47: text 0x870 and the name of the player slot b->unk65; else the owner's name markup, the kind's
   name text 0x48e + 14 * kind and, when b has a name of its own, "$A>name<"), the building picture 0x427 + kind centred in
   the window (sv_PanelBldPic, interactive; its user record is the owner's character unless that is the player), the
   health bar (sv_PanelHealthBar, the level 0 .. 100; not for the kinds 0x47 / 0x1d / 0x57) and, unless flag 1 of
   b->flags5a is set or for those kinds, the action button 0x8c (sv_PanelActionBtn: txt_PanelBtnOwn for the player's own
   or permitted building, else txt_PanelBtnForeign) and for the goods carrying classes the "Transport" button
   (sv_PanelTransportBtn, not with a fallback building); the unused widget ids are set to -1
   @deep read whole against 0x528860..0x528b74 and rewritten typed (AIBLD / AIBTYPE / D2WIN.w / D2OBJ
   interactive+user, d2_OpenFormS 16-bit x, title branch order != 0x47 first, the label as one ?:
   argument): every statement matches, 244 vs 247 instructions. Left is the constant -1: the original keeps
   it in ebp for the whole function and re-materialises it after ebp served as the AIBTYPE pointer (or
   ebp,-1 at 0x528b28 / 0x528b3b, then mov [sv_PanelWidgets], ebp and the else-path stores via ebp), ours
   stores immediates / eax (mov dword ptr [X], 0xffffffff; or eax,-1; mov [X], eax) and lays the three
   tails out in another order. Tried: the tail as if/else without returns and with a shared widget store
   (both 39 ins / 140 bytes, unchanged) */
void sv_OpenBuildingPanel(AIBLD *b)
{
    int level, cls;
    char name[128];
    AIBTYPE *bt;
    unsigned perm;

    level = gm_BuildingLevel((char *)b);
    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
    d2_SetForm(sv_PanelForm, 1);
    if (b->type != 0x47) {
        io_CharacterNameMarkup(b->owner, 2, 1, name);
        if (b->name[0] != 0)
            io_PrintText("$Z%s$A%s$A>%s<", name, b->type * 0xe + 0x48e, b->name);
        else
            io_PrintText("$Z%s$A%s", name, b->type * 0xe + 0x48e);
    } else {
        io_PrintText("$Z%s$A%s", 0x870, ai_PlayerSlots[b->playerSlot]);
    }
    d2_SetForm(sv_PanelForm, 0);
    sv_PanelBldPic = d2_WinOpenPic((d2_Wins[d2_LastWindow].w - 0x30) >> 1, 0x48, b->type + 0x427, d2_LastWindow);
    ((D2OBJ *)d2_Objs)[sv_PanelBldPic].interactive = 1;
    if (b->owner != 0xffff && b->owner != *ai_PlayerChar)
        ((D2OBJ *)d2_Objs)[sv_PanelBldPic].user = ai_Chars[b->owner];
    if (b->type == 0x47 || b->type == 0x1d || b->type == 0x57)
        sv_PanelHealthBar = -1;
    else {
        sv_PanelHealthBar = d2_WinOpenSlider(0x15, 0x7b, 0x38, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue4(sv_PanelHealthBar, 0, 100, level);
    }
    if ((b->flags5a & 1) == 0 && b->type != 0x47 && b->type != 0x1d && b->type != 0x57) {
        cls = gm_BuildingClass(b->type);
        bt = &ai_BTypesT[b->type];
        perm = gm_PermissionBits(ai_PlayerId, (unsigned char *)b, 0, 0);
        sv_PanelActionBtn = sv_AddPanelButton(0, 0x8c, b->owner != ai_PlayerId && b->masterId != ai_PlayerId && (perm & 0x10) == 0
                                                       ? txt_PanelBtnForeign : txt_PanelBtnOwn);
        if ((cls == 1 || cls == 2 || cls == 4 || cls == 7 || cls == 8 || bt->kind == 0x10 || bt->kind == 0x1e || bt->kind == 0x1f
             || bt->kind == 0x1d) && bt->kind != 5) {
            if (ai_FallbackBuilding == 0)
                sv_PanelTransportBtn = sv_AddPanelButton(0, 0xa6, (int)"Transport");
            sv_PanelWidgets[0] = -1;
            return;
        }
        sv_PanelWidgets[0] = -1;
        sv_PanelTransportBtn = -1;
        return;
    }
    sv_PanelActionBtn = -1;
    sv_PanelTransportBtn = -1;
    sv_PanelWidgets[0] = -1;
}
/* 0x528b80, opens the info panel (sv_PanelForm, "panel\infopanel_gebaeude") for the item object p unless one is open or p
   is one of the goods 0x92 .. 0x97 / 0x2db: the title is the item's name text 0x9a8 + 2 * id, after the kind name and the
   own name of the selected building (ai_PlayerBuilding, else ai_FallbackBuilding) when there is one; the item picture
   0xdf + id (interactive) one pixel left of the window's centre; for a goods type 2 / 6 item in a building the buttons:
   0x8c (sv_PanelActionBtn, txt_PanelBtnItemOwn) when the player owns the item's building or has permission bit 0x10 and
   the building lists objects after it (gm_ListObjectsAfter), and always 0xa6 (sv_PanelItemBtn, txt_PanelBtnItem) */
void sv_OpenItemPanel(unsigned short *p)
{
    AIBLD *b;
    unsigned short *owner;
    unsigned perm;
    AIGOOD *g;

    b = 0;
    if (sv_PanelForm == -1 && (*p < 0x92 || *p > 0x97) && *p != 0x2db) {
        sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 1);
        if (ai_PlayerBuilding != 0)
            b = (AIBLD *)ai_PlayerBuilding;
        else if (ai_FallbackBuilding != 0)
            b = (AIBLD *)ai_FallbackBuilding;
        if (b != 0) {
            if (b->name[0] != 0)
                io_PrintText("$Z%s$A>%s<$A%s", b->type * 0xe + 0x48e, b->name, *p * 2 + 0x9a8);
            else
                io_PrintText("$Z%s$A%s", b->type * 0xe + 0x48e, *p * 2 + 0x9a8);
        } else
            io_PrintText("$Z%s", *p * 2 + 0x9a8);
        d2_SetForm(sv_PanelForm, 0);
        ((D2OBJ *)d2_Objs)[d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) - 1, 0x48, *p + 0xdf, d2_LastWindow)].interactive = 1;
        g = &ai_GoodsT[*p];
        if (b != 0 && (g->type == 2 || g->type == 6)) {
            owner = gm_ObjectBuildingOwner(p);
            perm = gm_PermissionBits(ai_PlayerId, (unsigned char *)b, 0, 0);
            if (*owner == ai_PlayerId || (perm & 0x10) != 0) {
                if (gm_ListObjectsAfter((unsigned char *)b, p, 0) != 0)
                    sv_PanelActionBtn = sv_AddPanelButton(0, 0x8c, txt_PanelBtnItemOwn);
                sv_PanelItemBtn = sv_AddPanelButton(0, 0xa6, txt_PanelBtnItem);
            } else
                sv_PanelItemBtn = sv_AddPanelButton(0, 0xa6, txt_PanelBtnItem);
        }
    }
}
/* 0x528d90, opens the info panel (sv_PanelForm, "panel\infopanel_transporter") for the cart c unless one is open or its
   building has no owner: the title ("$Z%2N1$A%s": the owner character's id and the goods name text 0x9a8 + 2 * id), the
   cart picture 0xdf + id (interactive), for the player's own cart the button 0xac (sv_PanelCartBtn, txt_PanelBtnCart),
   the load bar (sv_PanelHealthBar, the fill percent); then notes the panel's window (sv_PanelCartWin) and the scene
   character that shows the cart (sv_PanelCartChar) */
void sv_OpenCartPanel(AIOBJ *c)
{
    unsigned short *owner;
    int x;

    if (sv_PanelForm != -1)
        return;
    owner = gm_ObjectBuildingOwner((unsigned short *)c);
    if (owner == 0)
        return;
    sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_transporter");
    d2_SetForm(sv_PanelForm, 1);
    io_PrintText("$Z%2N1$A%s", *owner, c->id * 2 + 0x9a8);
    d2_SetForm(sv_PanelForm, 0);
    x = ((d2_Wins[d2_LastWindow].w - 0x30) >> 1) - 1;
    ((D2OBJ *)d2_Objs)[d2_WinOpenPic(x, 0x30, c->id + 0xdf, d2_LastWindow)].interactive = 1;
    if (*owner == ai_PlayerId)
        sv_PanelCartBtn = sv_AddPanelButton(x, 0xac, txt_PanelBtnCart);
    sv_PanelHealthBar = d2_WinOpenSlider(0x18, 0x66, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
    d2_SetObjValue4(sv_PanelHealthBar, 0, 100, c->fill);
    d2_SetForm(sv_PanelForm, 2);
    sv_PanelCartWin = d2_LastWindow;
    sv_PanelCartChar = (int)c->sceneObj;
}

extern int sv_PanelBuildBtn;  /* 0x623884 */
extern int sv_PanelKind1eBtn;  /* 0x623898 */
extern int sv_PanelPortrait;  /* 0x623880 */
extern int ai_SelectedCount;  /* 0x6f2334 */
extern unsigned char ai_GameState;  /* 0x6f25ac */
extern int d2_PointerObj;  /* 0x5fe710 */
extern char *ai_Unk7662c0p;
extern unsigned short * sv_SelectedItem;
extern int ai_BuildingPerms;  /* 0x6f231c */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
int gm_MinutesBetween();                     /* 0x4fe280 */
void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);                     /* 0x4fe140 */
void rt_EditCartRoute();                     /* 0x4bf5b0 */
int d2_RemoveWindowObjects();                     /* 0x472d20 */
int gm_ListAmount();                     /* 0x50aaf0 */
int ai_RunAllows();                     /* 0x5763c0 */
void ai_CampaignStep(int a, int b, int c, int d);                     /* 0x576040 */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
int sv_ReturnZero(void);
void sv_OpenDefaultPanel(void);
void sv_OpenPersonPanel(AICHAR *c);
void sv_RefreshPanel(void);
void sv_OpenItemPanel(unsigned short *p);

/* 0x528ee0, opens the default info panel when no panel is open (sv_PanelForm): without a selected building the
   "standard" panel with the construction button 0x8c (sv_PanelBuildBtn) and the item button 0xa6; for the player's
   own fallback building (ai_FallbackBuilding, owner = ai_PlayerId) the building panel with the owner and kind name,
   the building picture, the health bar (its level), the button 0x8c and either the kind 0x1e button
   (sv_PanelKind1eBtn) or the item button 0xa6 */
void sv_OpenDefaultPanel(void)
{
    int level;
    char name[128];

    if (sv_PanelForm != -1)
        return;
    if (ai_FallbackBuilding == 0) {
        if (ai_PlayerBuilding == 0) {
            sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_standard");
            d2_SetForm(sv_PanelForm, 0);
            sv_PanelBuildBtn = sv_AddPanelButton(0, 0x8c, txt_PanelBtnBuild);
            sv_PanelItemBtn = sv_AddPanelButton(0, 0xa6, txt_PanelBtnItem);
            return;
        }
    } else if (((AIBLD *)ai_FallbackBuilding)->owner == (unsigned short)ai_PlayerId) {
        level = gm_BuildingLevel((char *)ai_FallbackBuilding);
        sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 0);
        d2_SetForm(sv_PanelForm, 1);
        io_CharacterNameMarkup(((AIBLD *)ai_FallbackBuilding)->owner, 2, 1, name);
        io_PrintText("$Z%s$A%s$A%s", name, ((AIBLD *)ai_FallbackBuilding)->type * 0xe + 0x48e, ((AIBLD *)ai_FallbackBuilding)->name);
        d2_SetForm(sv_PanelForm, 0);
        d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) - 1, 0x4c, ((AIBLD *)ai_FallbackBuilding)->type + 0x427, d2_LastWindow);
        sv_PanelHealthBar = d2_WinOpenSlider(0x15, 0x7a, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue4(sv_PanelHealthBar, 0, 100, level);
        sv_PanelActionBtn = sv_AddPanelButton(0, 0x8c, txt_PanelBtnOwn);
        if (((AIBLD *)ai_FallbackBuilding)->type == 0x1e) {
            sv_PanelKind1eBtn = sv_AddPanelButton(0, 0xa6, txt_PanelBtnKind1e);
            return;
        }
        sv_PanelItemBtn = sv_AddPanelButton(0, 0xa6, txt_PanelBtnItem);
    }
}
/* 0x5290f0, opens the person info panel ("panel\\infopanel_personen") unless a panel is open: for no character or
   several selected the text "%i %s" (ai_SelectedCount, txt_PanelPersons); for the character c its name markup and
   health in percent ("%s %i%%"), the opinion bar (sv_PanelHealthBar: gm_Opinion of the player for c, -127 .. 127),
   the standing picture c->ref54 for the kinds 5 / 6 / 7, for the title job groups 10 .. 12 the pictures 0x5df / 0x5e0
   with the value bars of the skills sk82 / sk83; then the portrait (sv_PanelPortrait: c->picture, interactive, its
   user record c; the picture 0x521 for none) */
void sv_OpenPersonPanel(AICHAR *c)
{
    char text[512];
    int opinion, group, health, x, skill;
    unsigned char kind;

    if (sv_PanelForm != -1)
        return;
    sv_PanelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_personen");
    d2_SetForm(sv_PanelForm, 1);
    if (c == 0 || ai_SelectedCount >= 2) {
        sprintf(text, "%i %s", ai_SelectedCount, txt_PanelPersons);
        io_PrintText(text);
    } else {
        opinion = gm_Opinion(ai_PlayerId, c->id);
        group = 0;
        if (c->title != 0)
            group = gm_TitleJobGroup((unsigned char)c->title);
        health = (int)(gm_HealthRatio(c) * 100.0);
        io_CharacterNameMarkup(c->id, 1, 4, text);
        io_PrintText("%s %i%%", text, health);
        d2_SetForm(sv_PanelForm, 0);
        x = (d2_Wins[d2_LastWindow].w - 0x30) >> 1;
        sv_PanelHealthBar = d2_WinOpenSlider(0x18, 0x6c, 0x30, 0, 100, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue4(sv_PanelHealthBar, -127, 0x7f, opinion);
        kind = c->kind;
        if (kind == 5 || kind == 6 || kind == 7)
            d2_WinOpenPic(x - 8, 0x30, c->ref54, d2_LastWindow);
        if (group == 0xb || group == 0xc || group == 10) {
            skill = c->sk82;
            d2_WinOpenPic(0x19, 0x7e, 0x5df, d2_LastWindow);
            sv_ValueBar(0x29, 0x7e, skill, 0x4f1);
            d2_WinOpenPic(0x19, 0x8c, 0x5e0, d2_LastWindow);
            sv_ValueBar(0x29, 0x8c, c->sk83, 0x4f1);
        }
    }
    d2_SetForm(sv_PanelForm, 0);
    x = (d2_Wins[d2_LastWindow].w - 0x30) >> 1;
    if (c != 0) {
        sv_PanelPortrait = d2_WinOpenPic(x, 0x38, c->picture, d2_LastWindow);
        ((D2OBJ *)d2_Objs)[sv_PanelPortrait].user = c;
        ((D2OBJ *)d2_Objs)[sv_PanelPortrait].interactive = 1;
        return;
    }
    d2_WinOpenPic(x, 0x38, 0x521, d2_LastWindow);
}
/* the panel's widget row sv_PanelWidgets[0..3] removed (sv_RefreshPanel writes the loop out at every use) */
#define SV_CLEAR_PANEL_WIDGETS(i) \
    for (i = 0; i < 4; i++) \
        if (sv_PanelWidgets[i] != -1) { \
            d2_RemoveObj(sv_PanelWidgets[i]); \
            sv_PanelWidgets[i] = -1; \
        }
#define SV_SELBLD ((AIBLD *)sv_SelectedObject)  /* the selected building, re-read at every use */
#define SV_SELCART ((AIOBJ *)sv_SelectedItem)  /* the selected cart, re-read at every use */

/* 0x529380, the per-frame refresh of the open info panel (sv_PanelForm). For the selected building
   (sv_SelectedObject with its health bar and picture): the title as sv_OpenBuildingPanel writes it, the level
   bar, and the progress of its current job on the picture (sv_PanelBldPic fillFraction: the job found by
   he_FindEvent for the flags5a bits 4 (0x1c), 1 (0xd: argc8 / workTotal, at least 0.01, 0.05 without a job),
   0x40 (0x1e), 0x80 (0x1f); the time from start to end against the time from start to now) with the busy
   picture 0x4ff (sv_PanelWidgets[0]); without a busy bit the busy picture is removed. For the selected character:
   its name and health ("$C%s %i%%", one selected only); when it works in a building of the player, its state
   pictures (sv_CharStateIcons, styled for 0xdf .. 0x3ba) and the progress of its job on the portrait
   (sv_PanelPortrait) by the job kind: 0x8f / 0x16 (start .. 0xb8), 0x40 (stage 2: start .. end), 0x61 (eight
   hours from 0xc4), 0x43 (a day), 0x62 (stage 4: four days), 0x65 (from stage 2: a day), 0x48 (stage 1:
   start .. 0xcc), 0x6c (start .. 0xb4); 0.01 before the timed stage. For the selected cart (goods kind
   0x1d, its panel window open): the route button opens rt_EditCartRoute, the fill bar, the name of the
   building of its job and one picture with its amount per goods of its load, at most four
   @deep Reconstructed whole from 0x529380..0x529fa8 (941 -> 3140 bytes; 927 vs 919 instructions): building
   title / level / job progress for the flags5a bits 4/1/0x40/0x80, the character name+health, state icons
   and the job-kind progress chain (0x8f,0x16,0x40,0x61,0x43,0x62,0x65,0x48,0x6c, read case by case incl.
   the AITIME copies and gm_AddTime hours), and the cart branch (route button, fill bar, job building name,
   goods pictures/amounts loop); every statement compared with the original, the char and cart paths align
   except regs/order/sib. Left are two code-generation decisions, no meaning: (1) the flags5a&1 path: the
   original keeps prog in st(0) (fld 0.05f at 0x52951c, fld st(0)/fcomp, fst [esp+0x14] before the widget
   calls, fld after) and cross-jumps its store into the portrait store at 0x529cc1; ours keeps prog in its
   slot (mov [esp+0x14],0x3d4ccccd / 0x3c23d70a immediates, fld at the end) with its own store+epilogue.
   (2) tail merging: the original's flags5a&4 path jumps into the 0x80 path at its fdiv (jmp 0x52969b) and
   the 0x40 path into the shared d2_SetForm call (jmp 0x5296d7); ours emits the 4 path's fdiv/store/widget
   tail in full (0x529501..0x529582). Tried: a separate float for the &1 path, a single exit (if/else chain
   without returns), the widget check shared vs per branch (934 -> 929 -> 927), the j==0 block as else (now
   placed at the end like the original). */
void sv_RefreshPanel(void)
{
    AICHAR *c;
    AIJOB *j;
    AITIME *from;
    AITIME t;
    float prog, total;
    int *q;
    int level, n, i, x, icon, o;
    int icons[8];
    unsigned char kind;
    unsigned short fl;
    unsigned char *bld;
    AIOBJ *load, *goods;
    char amount[64];
    char text[256];

    prog = 0.0f;
    if (sv_PanelForm == -1)
        return;
    if (sv_SelectedObject != 0 && sv_PanelHealthBar != -1 && sv_PanelBldPic != -1) {
        level = gm_BuildingLevel(sv_SelectedObject);
        d2_SetForm(sv_PanelForm, 1);
        io_PrintText("$C");
        if (SV_SELBLD->type != 0x47) {
            io_CharacterNameMarkup(SV_SELBLD->owner, 2, 1, text);
            if (SV_SELBLD->name[0] != 0)
                io_PrintText("$Z%s$A%s$A>%s<", text, SV_SELBLD->type * 0xe + 0x48e, SV_SELBLD->name);
            else
                io_PrintText("$Z%s$A%s", text, SV_SELBLD->type * 0xe + 0x48e);
        } else {
            io_PrintText("$Z%s$A%s", 0x870, ai_PlayerSlots[SV_SELBLD->playerSlot]);
        }
        d2_SetObjValue4(sv_PanelHealthBar, 0, 100, level);
        fl = SV_SELBLD->flags5aw;
        if ((fl & 4) != 0) {
            j = (AIJOB *)he_FindEvent(2, 0, 0x1c, 3, SV_SELBLD->id);
            if (j == 0)
                return;
            total = (float)gm_MinutesBetween(&j->start, &j->end8f);
            ((D2OBJ *)d2_Objs)[sv_PanelBldPic].fillFraction = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
            if (sv_PanelWidgets[0] == -1) {
                d2_SetForm(sv_PanelForm, 0);
                sv_PanelWidgets[0] = d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
            }
            return;
        } else if ((fl & 1) != 0) {
            j = (AIJOB *)he_FindEvent(2, 0, 0xd, 3, SV_SELBLD->id);
            prog = 0.05f;
            if (j != 0) {
                if (j->stage >= 4)
                    prog = (float)j->argc8 / j->workTotal;
                if (prog < 0.01)
                    prog = 0.01;
                if (sv_PanelWidgets[0] == -1) {
                    d2_SetForm(sv_PanelForm, 0);
                    sv_PanelWidgets[0] = d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
                }
            }
            ((D2OBJ *)d2_Objs)[sv_PanelBldPic].fillFraction = prog;
            return;
        } else if ((fl & 0x40) != 0) {
            j = (AIJOB *)he_FindEvent(2, 0, 0x1e, 3, SV_SELBLD->id);
            if (j == 0)
                return;
            total = (float)gm_MinutesBetween(&j->start, &j->end);
            ((D2OBJ *)d2_Objs)[sv_PanelBldPic].fillFraction = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
            if (sv_PanelWidgets[0] == -1) {
                d2_SetForm(sv_PanelForm, 0);
                sv_PanelWidgets[0] = d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
            }
            return;
        } else if ((fl & 0x80) != 0) {
            j = (AIJOB *)he_FindEvent(2, 0, 0x1f, 3, SV_SELBLD->id);
            if (j == 0)
                return;
            total = (float)gm_MinutesBetween(&j->start, &j->end);
            ((D2OBJ *)d2_Objs)[sv_PanelBldPic].fillFraction = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
            if (sv_PanelWidgets[0] == -1) {
                d2_SetForm(sv_PanelForm, 0);
                sv_PanelWidgets[0] = d2_WinOpenPic(((d2_Wins[d2_LastWindow].w - 0x30) >> 1) + 0x2f, 0x4c, 0x4ff, d2_LastWindow);
            }
            return;
        } else {
            if (sv_PanelWidgets[0] != -1) {
                d2_RemoveObj(sv_PanelWidgets[0]);
                sv_PanelWidgets[0] = -1;
            }
            return;
        }
    }
    if (sv_SelectedChar != 0) {
        c = (AICHAR *)sv_SelectedChar;
        if (ai_SelectedCount < 2 && c != 0) {
            d2_SetForm(sv_PanelForm, 1);
            n = (int)(gm_HealthRatio(c) * 100.0);
            io_CharacterNameMarkup(c->id, 1, 4, text);
            io_PrintText("$C%s %i%%", text, n);
        }
        if (c->bld == 0 || c->bld->owner != (unsigned short)ai_PlayerId || ai_SelectedCount >= 2) {
            SV_CLEAR_PANEL_WIDGETS(i)
            return;
        }
        SV_CLEAR_PANEL_WIDGETS(i)
        d2_SetForm(sv_PanelForm, 0);
        x = (d2_Wins[d2_LastWindow].w - 0x30) >> 1;
        n = sv_CharStateIcons((char *)sv_SelectedChar, icons);
        for (i = 0; i < n; i++) {
            icon = icons[i];
            if (icon >= 0xdf && icon < 0x3bb)
                sv_PanelWidgets[i] = d2_WinOpenPicStyledS(x + 0x32, i * 0x1a + 0x38, icon, d2_LastWindow, -2);
            else
                sv_PanelWidgets[i] = d2_WinOpenPic(x + 0x32, i * 0x1a + 0x38, icon, d2_LastWindow);
        }
        j = c->job;
        if (j != 0) {
            if (sv_PanelPortrait == -1)
                return;
            kind = j->kind;
        if (kind == 0x8f) {
            total = (float)gm_MinutesBetween(&j->start, &j->end8f);
            if (total != 0.0f)
                prog = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
        } else if (kind == 0x16) {
            total = (float)gm_MinutesBetween(&j->start, &j->end8f);
            if (total != 0.0f)
                prog = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
        } else if (kind == 0x40) {
            if (j->stage < 2)
                prog = 0.01f;
            else if (j->stage == 2) {
                total = (float)gm_MinutesBetween(&j->start, &j->end);
                if (total != 0.0f)
                    prog = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
            }
        } else if (kind == 0x61) {
            from = &j->start61;
            t = *from;
            gm_AddTime((struct AITIME *)&t, 8, 0, 0);
            total = (float)gm_MinutesBetween(from, &t);
            if (total != 0.0f)
                prog = (float)gm_MinutesBetween(from, &ai_Time) / total;
        } else if (kind == 0x43) {
            from = &j->start;
            t = *from;
            gm_AddTime((struct AITIME *)&t, 0x18, 0, 0);
            total = (float)gm_MinutesBetween(from, &t);
            if (total != 0.0f)
                prog = (float)gm_MinutesBetween(from, &ai_Time) / total;
        } else if (kind == 0x62) {
            if (j->stage < 4)
                prog = 0.01f;
            else if (j->stage == 4) {
                from = &j->start;
                t = *from;
                gm_AddTime((struct AITIME *)&t, 0x60, 0, 0);
                total = (float)gm_MinutesBetween(from, &t);
                if (total != 0.0f)
                    prog = (float)gm_MinutesBetween(from, &ai_Time) / total;
            }
        } else if (kind == 0x65) {
            if (j->stage < 2)
                prog = 0.01f;
            else {
                from = &j->start;
                t = *from;
                gm_AddTime((struct AITIME *)&t, 0x18, 0, 0);
                total = (float)gm_MinutesBetween(from, &t);
                if (total != 0.0f)
                    prog = (float)gm_MinutesBetween(from, &ai_Time) / total;
            }
        } else if (kind == 0x48) {
            if (j->stage < 1)
                prog = 0.01f;
            else if (j->stage == 1) {
                total = (float)gm_MinutesBetween(&j->start, &j->end48);
                if (total != 0.0f)
                    prog = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
            }
        } else if (kind == 0x6c) {
            total = (float)gm_MinutesBetween(&j->start, &j->end6c);
            if (total != 0.0f)
                prog = (float)gm_MinutesBetween(&j->start, &ai_Time) / total;
        }
            ((D2OBJ *)d2_Objs)[sv_PanelPortrait].fillFraction = prog;
        } else if (sv_PanelPortrait != -1)
            ((D2OBJ *)d2_Objs)[sv_PanelPortrait].fillFraction = 0;
        return;
    }
    if (sv_PanelCartWin == -1 || sv_SelectedItem == 0) {
        SV_CLEAR_PANEL_WIDGETS(i)
        return;
    }
    SV_CLEAR_PANEL_WIDGETS(i)
    if (sv_SelectedItem == 0 || ai_Goods[*sv_SelectedItem * 0x41] != 0x1d)
        return;
    if (sv_PanelCartBtn != -1 && d2_ClickedId != -1 && d2_PointerObj == sv_PanelCartBtn && (sv_FrameMask & 0x2000) != 0)
        rt_EditCartRoute((char *)sv_SelectedItem);
    if (sv_PanelHealthBar != -1)
        d2_SetObjValue4(sv_PanelHealthBar, 0, 100, SV_SELCART->fill);
    d2_SetForm(sv_PanelForm, 2);
    SV_CLEAR_PANEL_WIDGETS(i)
    d2_RemoveWindowObjects(d2_LastWindow, 1);
    if (SV_SELCART->job24 != 0 && (bld = gm_BuildQuery(1, 1, SV_SELCART->job24->cartBld)) != 0)
        io_PrintText("$Y$Z%s", *bld * 0xe + 0x48e);
    load = (AIOBJ *)gm_Query((void *)SV_SELCART->list, 1, 0, 0x1dd);
    if (load == 0)
        return;
    goods = (AIOBJ *)gm_Query((void *)load->list, 1, 5);
    if (goods == 0)
        return;
    q = sv_PanelWidgets;
    x = 3;
    do {
        if (x >= 0x83)
            return;
        *q = d2_WinOpenPicStyledS(x, 0x28, goods->id + 0xdf, d2_LastWindow, -2);
        sprintf(amount, "%i", gm_ListAmount(load, goods));
        o = d2_CreateWinText(x - 2, 0x1c, amount, d2_LastWindow);
        ((D2OBJ *)d2_Objs)[o].interactive = 1;
        ((D2OBJ *)d2_Objs)[o].centred = 1;
        ((D2OBJ *)d2_Objs)[o].w = 0x20;
        goods = (AIOBJ *)gm_QueryNext();
        x += 0x20;
        q++;
    } while (goods != 0);
}
/* 0x529fb0, closes the info panel (0x623860): frees the widgets 0x62386c .. (4 ids), destroys the form and clears all the panel's widget ids (0x623864 .. 0x623898) */

void sv_ClosePanel(void)
{
    int *q;

    if (sv_PanelForm != -1) {
        q = sv_PanelWidgets;
        do {
            if (*q != -1) {
                d2_RemoveObj(*q);
                *q = -1;
            }
            q++;
        } while ((int)q < (int)((unsigned)sv_PanelWidgets + 0x10));
        d2_CloseView(sv_PanelForm);
        sv_PanelForm = -1;
        sv_PanelBldPic = -1;
        sv_PanelHealthBar = -1;
        sv_PanelKind1eBtn = -1;
        sv_PanelActionBtn = -1;
        sv_PanelTransportBtn = -1;
        sv_PanelCartBtn = -1;
        sv_PanelBuildBtn = -1;
        sv_PanelItemBtn = -1;
        sv_PanelPortrait = -1;
        sv_PanelCartWin = -1;
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
void gm_ClockToDate(struct AITIME *c, unsigned char *out);                     /* 0x4fe2c0 */
int gm_Season(char *d);                     /* 0x4fe330, f3gm.h */
int gm_ClockSeason(AITIME *c);         /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
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
       sv_ViewShape. */
    if (SV_I((unsigned)&sv_ViewShape) != 0)
        d2_ShpBlit(sv_ViewRectLeft, sv_ViewRectTop, (struct D2SURF *)d2_ScreenSurf, (struct D2SHP *)sv_ViewShape);
    if (a & 1) {
        if (sv_ButtonFamilyTree != -1) { d2_RemoveObj(sv_ButtonFamilyTree); sv_ButtonFamilyTree = -1; }
        if (sv_ButtonCompare != -1) { d2_RemoveObj(sv_ButtonCompare); sv_ButtonCompare = -1; }
        if (sv_ButtonCityStats != -1) { d2_RemoveObj(sv_ButtonCityStats); sv_ButtonCityStats = -1; }
        if (sv_ButtonPrivileges != -1) { d2_RemoveObj(sv_ButtonPrivileges); sv_ButtonPrivileges = -1; }
        if (sv_PanelMoneyText != -1) { d2_RemoveObj(sv_PanelMoneyText); sv_PanelMoneyText = -1; }
        if (sv_ButtonInventory != -1) { d2_RemoveObj(sv_ButtonInventory); sv_ButtonInventory = -1; }
        if (sv_ButtonPlayerStats != -1) { d2_RemoveObj(sv_ButtonPlayerStats); sv_ButtonPlayerStats = -1; }
        if (sv_ButtonMessages != -1) { d2_RemoveObj(sv_ButtonMessages); sv_ButtonMessages = -1; }
        if (sv_PanelBuildingIcon != -1) { d2_RemoveObj(sv_PanelBuildingIcon); sv_PanelBuildingIcon = -1; }
        if (sv_PanelCompass != -1) { d2_RemoveObj(sv_PanelCompass); sv_PanelCompass = -1; }
        sv_ButtonFamilyTree = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), sv_BarWindow);
        SV_WI(sv_ButtonFamilyTree, 0x1c0) = 0x43;
        SV_WI(sv_ButtonFamilyTree, 0x4c) = 1;
        sv_ButtonCompare = d2_WinOpenPic(0x23a, 0xd, 0x522, sv_BarWindow);
        SV_WI(sv_ButtonCompare, 0x1c0) = 0x43;
        SV_WI(sv_ButtonCompare, 0x4c) = 1;
        sv_ButtonPrivileges = d2_CreateWinText(0x39, 0xd, ch_EmptyName, sv_BarWindow);
        SV_WI(sv_ButtonPrivileges, 0x1c0) = 0x43;
        SV_WI(sv_ButtonPrivileges, 0x4c) = 1;
        SV_WS(sv_ButtonPrivileges, 0x74) = 0x55;
        sv_ButtonPlayerStats = d2_CreatePicObjS(d2_ScreenW - 0x8d, d2_ScreenH - 0x73, 0x583);
        SV_WI(sv_ButtonPlayerStats, 0x1c0) = 0x43;
        SV_WI(sv_ButtonPlayerStats, 0x4c) = 1;
        sv_ButtonMessages = d2_CreatePicObjS(d2_ScreenW - 0x57, d2_ScreenH - 0x4f, 0x7d9);
        SV_WI(sv_ButtonMessages, 0x1c0) = 3;
        SV_WI(sv_ButtonMessages, 0x4c) = 1;
        sv_ButtonInventory = d2_CreatePicObjS(d2_ScreenW - 0x2a, d2_ScreenH - 0x66, 0x584);
        SV_WI(sv_ButtonInventory, 0x1c0) = 0x43;
        SV_WI(sv_ButtonInventory, 0x4c) = 1;
        sv_PanelBuildingIcon = d2_CreatePicObjS(d2_ScreenW - 0x4a, 0xd6, 0x585);
        SV_WI(sv_PanelBuildingIcon, 0x1c0) = 0x43;
        SV_WI(sv_PanelBuildingIcon, 0x4c) = 1;
        sv_PanelCompass = d2_CreatePicObjS(d2_ScreenW - 0x48, 0xd7, 0x551);
        SV_WI(sv_PanelCompass, 0x1c0) = 0x43;
        SV_WI(sv_PanelCompass, 0x4c) = 1;
        SV_WI(sv_PanelCompass, 0x18) = 8;
        sv_PanelMoneyText = d2_CreateScreenText(d2_ScreenW - 0x6a, d2_ScreenH - 0x7e, ch_EmptyName);
        SV_WI(sv_PanelMoneyText, 0x5c) = 1;
        SV_WS(sv_PanelMoneyText, 0x74) = 0x54;
        SV_WS(sv_PanelMoneyText, 0x14) = 0x6b;
        SV_WS(sv_PanelMoneyText, 0x16) = 0x20;
    } else {
        /* the texts of the six plates come from the text lists 0x1441ec8 .. 0x1441ee0 (or are cleared) */
        if (SV_B((unsigned)&ai_OptionPanelHelp) != 0) {
            if (sv_ButtonFamilyTree != -1) strcpy(SV_W(sv_ButtonFamilyTree) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d4)));
            if (sv_ButtonCompare != -1) strcpy(SV_W(sv_ButtonCompare) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9e0)));
            if (sv_ButtonPrivileges != -1) strcpy(SV_W(sv_ButtonPrivileges) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d0)));
            if (sv_ButtonPlayerStats != -1) strcpy(SV_W(sv_ButtonPlayerStats) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9cc)));
            if (sv_ButtonInventory != -1) strcpy(SV_W(sv_ButtonInventory) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9c8)));
            if (sv_PanelCompass != -1) strcpy(SV_W(sv_PanelCompass) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9d8)));
        } else {
            if (sv_ButtonFamilyTree != -1) SV_WB(sv_ButtonFamilyTree, 0xbc) = ch_EmptyName[0];
            if (sv_ButtonCompare != -1) SV_WB(sv_ButtonCompare, 0xbc) = ch_EmptyName[0];
            if (sv_ButtonPrivileges != -1) SV_WB(sv_ButtonPrivileges, 0xbc) = ch_EmptyName[0];
            if (sv_ButtonPlayerStats != -1) SV_WB(sv_ButtonPlayerStats, 0xbc) = ch_EmptyName[0];
            if (sv_ButtonInventory != -1) SV_WB(sv_ButtonInventory, 0xbc) = ch_EmptyName[0];
            if (sv_PanelCompass != -1) SV_WB(sv_PanelCompass, 0xbc) = ch_EmptyName[0];
        }
        sv_UpdateGroundplan(b);
        d2_SetFont(0x55);
        gm_ClockToDate((struct AITIME *)&ai_Time, (unsigned char *)&t);
        if (ls_GetLanguage() == 2) {
            sprintf(buf, "%s A.D.%i", d2_Strings[0x4c + gm_Season((char *)&t)], t.year);
            d2_DrawText(d2_Wins[sv_BarWindow].x + 0x181, 0x17, d2_ScreenSurf, buf, 0);
        } else {
            sprintf(buf, "%s A.D.%i", d2_Strings[0x4c + gm_Season((char *)&t)], t.year);
            d2_DrawText(d2_Wins[sv_BarWindow].x + 0x195, 0x17, d2_ScreenSurf, buf, 0);
        }
        h = (int)((SV_I(((unsigned)&ai_Time + 0x6)) % 0x3c) * 0.8);
        shape = d2_GetPic(0x581);
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, shape, h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        h = (int)((float)((SV_US((unsigned)&ai_Time.hour) * 0x3c + SV_I(((unsigned)&ai_Time + 0x6))) * 4.0) / 60.0) % 0x30;
        shape = d2_GetPic(0x582);
        d2_DrawShape(d2_ScreenW - 0x50, 0x11, d2_ScreenSurf, shape, h);
        d2_AddDirty(d2_ScreenW - 0x50, 0x11, SV_S((unsigned)&d2_ShpW), SV_S((unsigned)&d2_ShpH));
        if (SV_I((unsigned)&d2_HitUnderPointer) == sv_AdvanceHit) {
            sprintf(buf, "%02i:%02i", SV_US((unsigned)&ai_Time.hour), SV_I(((unsigned)&ai_Time + 0x6)));
            d2_SetFont(0x54);
            d2_DrawTextBox(d2_ScreenW - 0x50, 2, 0x2e, 0x14, d2_ScreenSurf, buf, 0x28);
            d2_AddDirty(d2_ScreenW - 0x50, 2, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
            d2_SetFont(0x55);
        }
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&ai_PlayerBuilding) == 0 && sv_PanelCompass != -1) {
            if (sv_PanelBuildingIcon != -1) {
                d2_EnableObject(sv_PanelBuildingIcon, 0);
                d2_EnableObject(sv_PanelCompass, 1);
                h = (int)((float)fmod(d3_Camera->rotation.y, 6.2831853f) * 0.15915494f * 48.0 + 48.5) % 0x30;
                SV_WI(sv_PanelCompass, 0x78) = h;
            } else {
                d2_EnableObject(sv_PanelCompass, 0);
            }
        } else {
            if (sv_PanelBuildingIcon != -1)
                d2_EnableObject(sv_PanelBuildingIcon, 1);
            if (sv_PanelCompass != -1)
                d2_EnableObject(sv_PanelCompass, 0);
        }
        d2_SetFont(0x55);
        if (sv_ButtonPrivileges != -1) {
            io_CharacterName(ai_PlayerId, 1, 2, name);
            strcpy(buf, name);
            d2_SetObjText(sv_ButtonPrivileges, buf);
        }
        sprintf(buf, "%i", SV_I(((unsigned)&ai_Selected + 0xc) + ai_PlayerId * 0x21c));
        d2_DrawText(d2_Wins[sv_BarWindow].x + 0x23a, 0x17, d2_ScreenSurf, buf, 0);
        if (sv_ButtonCompare != -1)
            d2_MoveObj(d2_Wins[sv_BarWindow].x + SV_S((unsigned)&d2_TextDrawnW) + 0x23d, 0x18, sv_ButtonCompare);
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0) {
            if (sv_ButtonCityStats == -1) {
                strcpy(name, (char *)(unsigned)&ai_PlayerSlots + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
                _strupr(name);
                sprintf(arms, "_STADTWAPPEN_%s", name);
                pic = d2_FindPic(arms);
                if (pic != -1)
                    sv_ButtonCityStats = d2_CreatePicObjS(d2_ScreenW - 0x6e, 0x5b, pic);
                else
                    sv_ButtonCityStats = d2_CreatePicObjS(d2_ScreenW - 0x6e, 0x5b, 0x5b1);
                SV_WI(sv_ButtonCityStats, 0x1c0) = 0x43;
                SV_WI(sv_ButtonCityStats, 0x4c) = 1;
            }
            if (SV_B((unsigned)&ai_OptionPanelHelp) != 0) {
                if (sv_ButtonCityStats != -1) strcpy(SV_W(sv_ButtonCityStats) + 0xbc, SV_C(((unsigned)&ai_MessageFilterTexts + 0x9dc)));
            } else if (sv_ButtonCityStats != -1)
                SV_WB(sv_ButtonCityStats, 0xbc) = ch_EmptyName[0];
            d2_SetFont(0x56);
            strcpy(name, (char *)((unsigned)&ai_PlayerSlots + 0x20) + SV_B((unsigned)&ai_OwnPlayer) * 0x2f4);
            d2_DrawTextBox(d2_ScreenW - 0x70, 0x5d, 0x68, 0x14, d2_ScreenSurf, name, 8);
            d2_AddDirty(d2_ScreenW - 0x70, 0x5d, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        } else if (sv_ButtonCityStats != -1) {
            d2_RemoveObj(sv_ButtonCityStats);
            sv_ButtonCityStats = -1;
        }
        d2_SetFont(0x54);
        if (SV_I((unsigned)&sv_ResearchClass) == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_B(SV_I((unsigned)&ai_FallbackBuilding)) == 0x47)
                who = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x65);
            else
                who = SV_B((unsigned)&ai_OwnPlayer);
            n = gm_SumSlotAmounts(SV_I(((unsigned)&ai_Unk770cec + 0xc) + ai_PlayerId * 0x21c), who) + sv_PendingMoney;
        } else {
            n = ((int (*)(void *))gm_SumListAmounts)(*(int *)(gm_PlayerSeat(SV_I((unsigned)&sv_ResearchClass)) + 0x14)) + sv_PendingMoney;
            who = SV_B((unsigned)&ai_OwnPlayer);
        }
        if (sv_PanelMoneyText != -1) {
            if (n != 0) {
                h = gm_ConvertMoney(n, who);
                txt_Format(buf, "%I~ %w", (int)(float)h, who);   /* 0x523d17: the second argument (esi) is
                                                                    the same owner index, for the %w city name */
                d2_SetObjText(sv_PanelMoneyText, buf);
            } else
                d2_SetObjText(sv_PanelMoneyText, SV_C(((unsigned)&ai_MessageFilterTexts + 0x8c0)));
        }
    }
    if (sv_ButtonFamilyTree != -1 && SV_WI(sv_ButtonFamilyTree, 8) != SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c)) {
        d2_RemoveObj(sv_ButtonFamilyTree);
        sv_ButtonFamilyTree = -1;
        sv_ButtonFamilyTree = d2_WinOpenPic(0x10, 8, SV_I(((unsigned)&ai_Unk770b89 + 0x4b) + ai_PlayerId * 0x21c), sv_BarWindow);
        SV_WI(sv_ButtonFamilyTree, 0x1c0) = 3;
        SV_WI(sv_ButtonFamilyTree, 0x4c) = 1;
    }
    if (he_BarShown >= 0) {
        if (he_LastSlot > 3) {
            d2_EnableObject(sv_BarToggle, 1);
        } else {
            d2_EnableObject(sv_BarToggle, 0);
            SV_I((unsigned)&full_data_6f2364) = 0;
        }
        if (d2_ClickedId != -1 && d2_PointerObj == sv_BarToggle)
            SV_I((unsigned)&full_data_6f2364) = SV_I((unsigned)&full_data_6f2364) == 0;
        if (SV_I((unsigned)&full_data_623e90) != he_LastSlot || (d2_ClickedId != -1 && d2_PointerObj == sv_BarToggle)) {
            if (SV_I((unsigned)&full_data_6f2364) != 0) {
                d2_SetForm(he_BarView, 0);
                d2_MoveObj(d2_ScreenW - (he_LastSlot << 5) - 0x1d, d2_Wins[d2_LastWindow].y, d2_Wins[d2_LastWindow].parent);
                d2_MoveObj(d2_ScreenW - (he_LastSlot << 5) - 0x4d, d2_ScreenH - 0x21, sv_BarToggle);
            } else {
                d2_SetForm(he_BarView, 0);
                d2_MoveObj(d2_ScreenW - 0x7d, d2_Wins[d2_LastWindow].y, d2_Wins[d2_LastWindow].parent);
                d2_MoveObj(d2_ScreenW - 0xad, d2_ScreenH - 0x21, sv_BarToggle);
            }
            SV_I((unsigned)&full_data_623e90) = he_LastSlot;
        }
        if (SV_I((unsigned)&full_data_6f2364) != 0) {
            d2_SetClipRect(0, 0, d2_ScreenW - 0x8c, d2_ScreenH);
            shape = d2_GetPic(sv_PanelPic + 3);
            d2_DrawShape(d2_ScreenW - (he_LastSlot << 5) - 0x37, d2_ScreenH - 0x37, d2_ScreenSurf, shape, 0);
            d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        }
    }
}

unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */

/* 0x52adb0, resets the selection (item, object, model, sv_SelectedExtra) and clears the bit 2 of the carts (kind 0x1d) of the current building: the player's building, else the fallback one, else the first gm_BuildQuery(1, 0, 0x44) finds
   @deep read the whole function against the original: stores, the building choice (player's building ->
   fallback, else gm_BuildQuery(1,0,0x44)), the gm_Query(objList,1,5) cart loop with AIOBJ.flags20 &= ~2
   all match. Left: the original tests b twice on the ai_PlayerBuilding != 0 path (+0x3c cmp eax,ebx; je
   ret, then +0x40 cmp eax,ebx; je ret, reached by jmp from the gm_BuildQuery call), ours merges them (one
   cmp/je, no jmp): 3 ins. Tried: one common test after if/else, return inside each branch, '&&' condition
   form, b loaded before/after the stores; the flow is equal, only VC6's tail merge of the redundant test
   differs */
void sv_ResetSelection(void)
{
    AIBLD *b;
    AIOBJ *o;

    sv_SelectedItem = 0;
    sv_SelectedObject = 0;
    sv_SelectedExtra = 0;
    sv_SelectedModel = 0;
    if (ai_PlayerBuilding != 0) {
        b = (AIBLD *)ai_FallbackBuilding;
        if (b == 0)
            return;
    } else {
        b = (AIBLD *)ai_FallbackBuilding;
        if (b == 0)
            b = (AIBLD *)gm_BuildQuery(1, 0, 0x44);
    }
    if (b == 0)
        return;
    sv_OtherSelected = 0;
    o = (AIOBJ *)gm_Query((void *)b->objList, 1, 5);
    while (o != 0) {
        if (ai_GoodsT[o->id].type == 0x1d)
            o->flags20 &= ~2u;
        o = (AIOBJ *)gm_QueryNext();
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
    SV_I((unsigned)&ai_SelectedCount) = 0;
}
/* 0x52b2a0, collects into ai_SceneChars the characters whose scene character (AICHAR.model, its +0x2c) belongs to the
   building b (b == 0: the ones in no building, -1), at most 31; past the eighth one of a building the scene character
   is hidden */
void sv_CollectGroupCharacters(char *b)
{
    int n, i;

    memset(ai_SceneChars, 0, sizeof(ai_SceneChars));
    if (b != 0) {
        n = 0;
        for (i = 0; i < 768; i++) {
            if (n >= 31)
                return;
            if (ai_CharsT[i].model != 0 && ai_CharsT[i].model->bldId == ((AIBLD *)b)->id) {
                ai_SceneChars[n++] = (int)&ai_CharsT[i];
                if (n > 8)
                    ch_SetVisible((CHARACTER *)ai_CharsT[i].model, 0);
            }
        }
    } else {
        n = 0;
        for (i = 0; i < 768; i++) {
            if (n >= 31)
                return;
            if (ai_CharsT[i].model != 0 && ai_CharsT[i].model->bldId == -1)
                ai_SceneChars[n++] = (int)&ai_CharsT[i];
        }
    }
}
/* 0x52b350, registers the activity of the character job kind (name, dayOnly, icon, valueOff: SVACTIVITY) in the
   first free entry of sv_Activities; returns 1, or 0 when the table is full */
int sv_RegisterActivity(int kind, char *name, int dayOnly, int icon, int valueOff)
{
    int i;

    for (i = 0; i < 0x30; i++) {
        if (sv_Activities[i].kind == 0) {
            if (name != 0)
                strcpy(sv_Activities[i].name, name);
            sv_Activities[i].kind = kind;
            sv_Activities[i].dayOnly = dayOnly;
            sv_Activities[i].icon = icon;
            sv_Activities[i].valueOff = valueOff;
            return 1;
        }
    }
    return 0;
}
/* 0x52b3c0, copies to out the location name of the activity (sv_Activities) of c's job kind when c is not flagged
   0x1000000, is in the building b, and the activity is not day-only or the hour lies in the season's day window
   (gm_DayStartHours / gm_DayEndHours); returns 1, else 0 */
int sv_ActivityName(AICHAR *c, AIBLD *b, char *out)
{
    int season, i;

    season = gm_ClockSeason(&ai_Time);
    for (i = 0; sv_Activities[i].kind != 0; i++) {
        if (sv_Activities[i].kind == c->jobKind) {
            if ((c->flags1cc & (1 << 24)) == 0 && c->bld == b
                && (sv_Activities[i].dayOnly == 0
                    || ((float)ai_Time.hour < gm_DayEndHours[season] && gm_DayStartHours[season] <= (float)ai_Time.hour))) {
                strcpy(out, sv_Activities[i].name);
                return 1;
            }
            return 0;
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
int ai_RunAllows();                     /* 0x5763c0 */
void ai_CampaignStep(int a, int b, int c, int d);                     /* 0x576040 */
int gm_BuildingClass(int kind);                /* 0x501d80, f3gm.c, the class of a building kind */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
void d2_FreeAnim(int n);                     /* 0x47ce30 */
int d3t_PlaceAtObject();                     /* 0x4118a0 */
void snd_PlayWorkerClick(struct G16CHARCLICK *c);                     /* 0x4c2c70 */

/* 0x52a050, the per-frame update of the info panel (not while the player bar is open or before the game runs):
   enables the transport button unless the selected building holds the objects 0x2a / 0x116; with the 0x2000 frame
   flag a click on the action button opens the tech tree of the player's (or the fallback building's shown room)
   building when the player holds the permission 0x10, else the building window (the managed building window for
   the classes 3 / 5); a click on the transport button opens ai_TransportWindow for a goods building, one on the
   item button the amount menu. Then sv_RefreshPanel; when the selection differs from what the panel was built for
   (sv_Shown*) or sv_PanelRebuild is set, the panel is closed and opened anew for the cart, the persons, the item /
   room or the building, and the sv_Shown* state is stored */
void sv_UpdatePanel(void)
{
    AIBLD *b, *sel;
    AIBTYPE *bt;
    int cls, i, perms, room;
    int st[0x14];

    b = 0;
    if (sv_PlayerBarForm != -1)
        return;
    if (ai_GameState == 0)
        return;
    if (sv_SelectedObject != 0 && gm_Query(SV_SELBLD->objList, 2, 6, 0, 0x2a) == 0 && gm_Query(SV_SELBLD->objList, 2, 6, 0, 0x116) == 0)
        d2_SetObjEnabled(sv_PanelTransportBtn, 0);
    else
        d2_SetObjEnabled(sv_PanelTransportBtn, 1);
    if ((sv_FrameMask & 0x2000) != 0 && d2_ClickedId != -1 && sv_PanelActionBtn != -1 && d2_PointerObj == sv_PanelActionBtn) {
        if (ai_PlayerBuilding != 0)
            perms = gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_PlayerBuilding, 0, 0);
        else if (ai_FallbackBuilding != 0) {
            if (sv_PanelShown == 0)
                perms = gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_FallbackBuilding, 0, 0);
            else {
                room = (int)gm_Query(((AIBLD *)ai_FallbackBuilding)->objList, 1, 0, ((AIBLD *)ai_FallbackBuilding)->mainRoom);
                perms = gm_PermissionBits(ai_PlayerId, 0, room, 0);
            }
        } else {
            if (sv_SelectedObject == 0)
                goto refresh;
            perms = gm_PermissionBits(ai_PlayerId, (unsigned char *)sv_SelectedObject, 0, 0);
        }
        sv_PanelPerms = perms;
        if (ai_PlayerBuilding != 0 && (sv_PanelPerms & 0x10) != 0)
            ai_TechTreeWindow((int)ai_PlayerBuilding, ((AIBLD *)ai_PlayerBuilding)->mainRoom);
        else if (ai_FallbackBuilding != 0 && sv_PanelShown == 0)
            ai_BuildingWindow((int)ai_FallbackBuilding);
        else if (ai_FallbackBuilding != 0 && (sv_PanelPerms & 0x10) != 0)
            ai_TechTreeWindow((int)ai_FallbackBuilding, *sv_PanelShown);
        else if (sv_SelectedObject != 0) {
            cls = gm_BuildingClass(SV_SELBLD->type);
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)sv_SelectedObject, 0, 0);
            if ((cls == 3 || cls == 5) && (sv_PanelPerms & 0x10) != 0)
                ai_ManagedBuildingWindow((int)sv_SelectedObject);
            else
                ai_BuildingWindow((int)sv_SelectedObject);
        }
    } else if (d2_ClickedId != -1) {
        if (sv_PanelTransportBtn != -1 && d2_PointerObj == sv_PanelTransportBtn && (sv_FrameMask & 0x2000) != 0) {
            sel = SV_SELBLD;
            if (sel != 0 && sel->type != 0x47) {
                cls = gm_BuildingClass(sel->type);
                bt = &ai_BTypesT[sel->type];
                if ((cls == 1 || cls == 2 || cls == 4 || cls == 7 || cls == 8 || bt->kind == 0x10 || bt->kind == 0x1e) && ai_RunAllows(0x400) == 1) {
                    ai_BuildingPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)sel, 0, 0);
                    ai_CampaignStep(0x12, (int)sel, 0, 0);
                    ai_TransportWindow((int)sel, 1);
                    ai_CampaignStep(0x12, (int)sel, 1, 0);
                    sv_SelectedObject = (char *)sel;
                    sv_ResetScreenClip();
                }
            }
        } else if (sv_PanelItemBtn != -1 && d2_PointerObj == sv_PanelItemBtn && (sv_FrameMask & 0x2000) != 0 && ai_RunAllows(0x80) == 1) {
            for (i = 0; i < 0x14; i++)
                st[i] = 0;
            st[1] = 0;
            st[5] = 4;
            ai_CampaignStep(0xf, 0, 0, 0);
            ai_AmountMenu((int)st, (int)ch_EmptyName, (int)ch_EmptyName, (int)sv_ReturnZero);
            ai_CampaignStep(0xf, 0, 1, 0);
        }
    }
refresh:
    sv_RefreshPanel();
    if ((int)sv_SelectedObject == sv_ShownObject && (int)ai_FallbackBuilding == sv_ShownFallback && (int)ai_PlayerBuilding == sv_ShownBuilding
        && (int)sv_PanelShown == sv_ShownRoom && (int)sv_SelectedItem == sv_ShownItem && sv_SelectedChar == sv_ShownChar
        && sv_OtherSelected == sv_ShownOther && ai_SelectedCount == sv_ShownCount && sv_PanelRebuild == 0)
        return;
    sv_PanelRebuild = 0;
    sv_ClosePanel();
    if (sv_SelectedItem != 0 && ai_Goods[*sv_SelectedItem * 0x41] == 0x1d && ai_SelectedCount == 0 && sv_SelectedChar == 0)
        sv_OpenCartPanel((AIOBJ *)sv_SelectedItem);
    else if (sv_SelectedModel == 0 && sv_OtherSelected == 0 && ai_SelectedCount == 0) {
        if (sv_PanelShown == 0 && sv_SelectedChar == 0)
            sv_OpenDefaultPanel();
    } else if (ai_SelectedCount != 0) {
        if (sv_SelectedChar != sv_ShownChar || sv_ShownCount != ai_SelectedCount) {
            if (ai_SelectedCount > 1)
                sv_OpenPersonPanel(0);
            else {
                sv_OpenPersonPanel((AICHAR *)sv_SelectedChar);
                sv_ShownChar = sv_SelectedChar;
                sv_RefreshPanel();
            }
        }
    }
    sv_ShownOther = sv_OtherSelected;
    sv_ShownCount = ai_SelectedCount;
    if (sv_SelectedModel == 0 && ai_FallbackBuilding == 0 && ai_PlayerBuilding == 0) {
        sv_ShownItem = (int)sv_SelectedItem;
        sv_ShownFallback = 0;
        sv_ShownBuilding = 0;
        sv_ShownModel = 0;
        sv_ShownRoom = (int)sv_PanelShown;
        sv_ShownChar = sv_SelectedChar;
        sv_ShownObject = (int)sv_SelectedObject;
        return;
    }
    sv_ShownFallback = (int)ai_FallbackBuilding;
    sv_ShownBuilding = (int)ai_PlayerBuilding;
    sv_ShownRoom = (int)sv_PanelShown;
    sv_ShownChar = sv_SelectedChar;
    sv_ShownItem = (int)sv_SelectedItem;
    sv_ShownObject = (int)sv_SelectedObject;
    if ((sv_SelectedItem != 0 || sv_PanelShown != 0) && sv_PanelForm == -1) {
        sv_ShownFallback = (int)ai_FallbackBuilding;
        sv_ShownBuilding = (int)ai_PlayerBuilding;
        sv_ShownModel = sv_SelectedModel;
        sv_ShownRoom = (int)sv_PanelShown;
        if (sv_SelectedItem != 0) {
            sv_ShownModel = sv_SelectedModel;
            sv_ShownItem = (int)sv_SelectedItem;
            sv_OpenItemPanel(sv_SelectedItem);
            sv_RefreshPanel();
        } else if (sv_PanelShown != 0) {
            sv_ShownRoom = (int)sv_PanelShown;
            sv_OpenItemPanel(sv_PanelShown);
            sv_RefreshPanel();
        }
        return;
    }
    if (sv_SelectedObject == 0 && ai_FallbackBuilding == 0) {
        sv_ShownModel = 0;
        return;
    }
    sv_ShownRoom = (int)sv_PanelShown;
    sv_ShownFallback = (int)ai_FallbackBuilding;
    sv_ShownBuilding = (int)ai_PlayerBuilding;
    sv_ShownModel = sv_SelectedModel;
    sv_ShownObject = (int)sv_SelectedObject;
    if (sv_SelectedObject != 0)
        b = SV_SELBLD;
    else if (ai_FallbackBuilding != 0)
        b = (AIBLD *)ai_FallbackBuilding;
    sv_OpenBuildingPanel(b);
    sv_RefreshPanel();
}

struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);   /* 0x448630 */

/* 0x52a680, the hover pick of the town view: unless the pointer shows a look 1 .. 8, picks the scene object under
   the pointer inside the view rectangle (d3if_Pick kind 0x20, then 0x40; none, or a window / widget under the
   pointer: clears the caption unless the pointed widget keeps it, frees the pointer sprite and forgets the hover
   state). With the frame flag 0x10 the object's kind +0x4b names what it is: 1 an item (sv_HoverItem: its name,
   with the 0x4000 permission for the goods kinds 6 / 2 the use text and look 0xb, else look 0x16), 2 a building
   (sv_HoverBuilding: its kind name and its own name), 3 a character (ai_PickedChar: its name, for a title 0x1c
   with flag 1 and the permission 0x800 the text 0x414c and look 0x2e); the selected, permitted building gets look
   0xb. A "contact_gate_<goods>" object of the player's / fallback building resolves to that goods item (or the
   item 0xfd), a "tp_TUER" door to its building's name, any other object to its name plate; the caption goes to
   d2_CaptionCopy and the picked object is kept in sv_HoverLast
   @deep Rewritten whole against 0x52a680..0x52ada8 (505 vs 508 instructions): the look 1..8 exit, pick
   x/y, the view-rect / window / widget tests, the two d3if_Pick kinds, the switch on the object kind +0x4b
   (item / building / character with their captions, looks and permission words, the fallback-or-0
   PermissionBits as two calls), the selected-building look, the contact_gate resolve loop, the tp_TUER
   door captions, the name plate loop and the sprite reset; every statement compared. Left, no meaning: (1)
   block layout: the original places the nothing-picked block (0x52a76d..0x52a7dc, own epilogue) inline
   after the second pick and lets the main path fall into the shared epilogue; ours moves that block to the
   end (0x52ad37) and gives the main path its own epilogue (31+31 lines); tried then-with-return, if/else
   and the inverted && / || condition, all identical code. (2) memcmp(o, "tp_TUER", 8): the original emits
   mov ecx,8 / repe cmpsb, ours mov ecx,2 / repe cmpsd. (3) addressing / allocation: mov ecx,eax; add
   ecx,edx; mov cl,[ecx] vs mov cl,[eax+edx] (ai_Goods kind), lea edx,[eax+ecx+1] vs add/inc (_strcmpi
   arg), eax vs edx for sv_HoverContact. */
void sv_HoverPick(void)
{
    char *o;
    AIBLD *b;
    unsigned short *it;
    int x, y, n;
    char goods[0x60];

    ai_PickedObject = 0;
    if ((unsigned short)d2_PointerLook >= 1 && (unsigned short)d2_PointerLook <= 8)
        return;
    x = dm_L.x;
    y = dm_L.y;
    sv_PickX = x;
    sv_PickY = y;
    if (x > sv_ViewRectLeft && x < sv_ViewRectRight && y > sv_ViewRectTop && y < sv_ViewRectBottom && d2_HitWindow == -1
        && d2_HitUnderPointer == -1
        && ((ai_PickedObject = d3if_Pick((float)x, (float)y, 0x20, 1)) != 0
            || (ai_PickedObject = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x40, 1)) != 0)) {
        if ((sv_FrameMask & 0x10) == 0 || d2_HoverBlocked != 0 || ai_PickedObject == 0 || d2_HitWindow != -1 || d2_HitUnderPointer != -1)
            return;
        sv_PanelPerms = 0;
        sv_HoverItemPerms = 0;
        sv_HoverBldPerms = 0;
        sv_HoverCharPerms = 0;
        d2_PointerLook = 0;
        sv_HoverItem = 0;
        sv_HoverBuilding = 0;
        ai_PickedChar = 0;
        sv_HoverContact = 0;
        switch (ai_PickedObject[0x4b]) {
        case 1:
            sv_HoverItem = *(int *)(ai_PickedObject + 0x200);
            if (ai_FallbackBuilding != 0)
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_FallbackBuilding, sv_HoverItem, 0);
        else
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, 0, sv_HoverItem, 0);
            if (sv_HoverItem != 0) {
                n = *(unsigned short *)sv_HoverItem;
                if ((ai_Goods[n * 0x41] == 6 || ai_Goods[n * 0x41] == 2) && (sv_PanelPerms & 0x4000) != 0) {
                    sprintf(d2_CaptionCopy, "%s %s", txt_Ptrs[0x9a8 + n * 2], txt_Ptrs[0x83]);
                    d2_PointerLook = 0xb;
                } else {
                    sprintf(d2_CaptionCopy, "%s", txt_Ptrs[0x9a8 + n * 2]);
                    d2_PointerLook = 0x16;
                }
            }
            sv_HoverItemPerms = sv_PanelPerms;
            break;
        case 2:
            sv_HoverBuilding = *(int *)(ai_PickedObject + 0x200);
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)sv_HoverBuilding, 0, 0);
            b = (AIBLD *)sv_HoverBuilding;
            if (b != 0 && (b->flags5a & 1) == 0) {
                if (b->name[0] != 0)
                    sprintf(d2_CaptionCopy, "%s >%s<", txt_Ptrs[0x48e + b->type * 14], b->name);
                else
                    strcpy(d2_CaptionCopy, txt_Ptrs[0x48e + b->type * 14]);
            }
            sv_HoverBldPerms = sv_PanelPerms;
            break;
        case 3:
            ai_PickedChar = *(int *)(ai_PickedObject + 0x200);
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, 0, 0, ai_PickedChar);
            if (ai_PickedChar != 0) {
                io_CharacterName(((AICHAR *)ai_PickedChar)->id, 1, 4, d2_CaptionCopy);
                if (((AICHAR *)ai_PickedChar)->title == 0x1c && (((AICHAR *)ai_PickedChar)->flags1b4 & 1) != 0 && (sv_PanelPerms & 0x800) != 0
                    && d3_CurrentScene != 0) {
                    txt_Format(d2_CaptionCopy, 0x414c, ((AICHAR *)ai_PickedChar)->id);
                    d2_PointerLook = 0x2e;
                }
            }
            sv_HoverCharPerms = sv_PanelPerms;
            break;
        default:
            d2_CaptionCopy[0] = 0;
        }
        if (sv_SelectedObject != 0 && sv_HoverBuilding == (int)sv_SelectedObject && (sv_HoverBldPerms & 1) != 0
            && (((AIBLD *)sv_HoverBuilding)->flags5a & 1) == 0)
            d2_PointerLook = 0xb;
        b = (AIBLD *)ai_FallbackBuilding;
        if ((b != 0 || (b = (AIBLD *)ai_PlayerBuilding) != 0) && ai_PickedObject != 0 && strncmp(ai_PickedObject, "contact_gate_", 0xd) == 0) {
            strcpy(goods, ai_PickedObject + 0x10);
            if (ai_FallbackBuilding != 0)
                it = (unsigned short *)gm_Query(b->objList, 1, 4, 6);
            else
                it = (unsigned short *)gm_Query(b->objList, 1, 4, 2);
            for (; it != 0; it = gm_QueryNext())
                if (_strcmpi(goods, ai_Goods + *it * 0x41 + 1) == 0)
                    break;
            if (it != 0)
                sv_HoverContact = (int)it;
            else if (ai_FallbackBuilding != 0)
                sv_HoverContact = (int)gm_Query(b->objList, 1, 0, 0xfd);
            if (sv_HoverContact != 0) {
                sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)b, sv_HoverContact, 0);
                if ((sv_PanelPerms & 1) != 0) {
                    d2_PointerLook = 0xb;
                    if (*(unsigned short *)sv_HoverContact == 0xfd) {
                        if (sv_PanelShown != 0)
                            sprintf(d2_CaptionCopy, "%s %s", txt_Ptrs[0x9a8 + *sv_PanelShown * 2], txt_Ptrs[0x84]);
                        else
                            sprintf(d2_CaptionCopy, "%s %s", txt_Ptrs[0x48e + b->type * 14], txt_Ptrs[0x84]);
                    } else
                        sprintf(d2_CaptionCopy, "%s %s", txt_Ptrs[0x9a8 + *(unsigned short *)sv_HoverContact * 2], txt_Ptrs[0x83]);
                } else
                    sv_HoverContact = 0;
            }
        }
        o = ai_PickedObject;
        if (memcmp(o, "tp_TUER", 8) == 0 && (b = *(AIBLD **)(o + 0x200)) != 0) {
            if ((sv_PanelPerms & 1) != 0) {
                d2_PointerLook = 0xb;
                if (b->name[0] != 0)
                    sprintf(d2_CaptionCopy, "%s >%s< %s", txt_Ptrs[0x48e + b->type * 14], b->name, txt_Ptrs[0x83]);
                else
                    sprintf(d2_CaptionCopy, "%s %s", txt_Ptrs[0x48e + b->type * 14], txt_Ptrs[0x83]);
            } else if (b->name[0] != 0)
                sprintf(d2_CaptionCopy, "%s >%s<", txt_Ptrs[0x48e + b->type * 14], b->name);
            else
                strcpy(d2_CaptionCopy, txt_Ptrs[0x48e + b->type * 14]);
        } else {
            for (n = 0; n < 0x20; n++)
                if (sv_NamePlates[n].obj == o) {
                    strcpy(d2_CaptionCopy, sv_NamePlates[n].text);
                    d2_PointerLook = sv_NamePlates[n].look;
                }
        }
        if (d2_PointerSprite != -1) {
            d2_FreeAnim(d2_PointerSprite);
            d2_PointerSprite = -1;
            sv_PointerSpriteFor = 0;
        }
        sv_HoverLast = (int)ai_PickedObject;    } else {
        if (d2_PointerObj == -1 || (((D2OBJ *)d2_Objs)[d2_PointerObj].style & 0x40) == 0)
            d2_CaptionCopy[0] = 0;
        if (d2_PointerSprite != -1) {
            d2_FreeAnim(d2_PointerSprite);
            d2_PointerSprite = -1;
            sv_PointerSpriteFor = -1;
        }
        sv_HoverLast = 0;
        sv_HoverItem = 0;
        sv_HoverBuilding = 0;
        ai_PickedChar = 0;
        d2_PointerLook = 0;
    }
}
/* 0x52ae70, "sv_HandleObjects": turns a click into the selection. A pending quick jump (sv_PanelFlag) looks up its
   contact name (sv_PanelText) as an object when the panel shows the current building's remembered room; a left
   click inside the view (or that jump) copies the hover state into the sv_Clicked* state, focuses the camera on a
   zoom target object that is no door / goods / contact (calling its event 2), selects the clicked character when
   the player may command him (0x800) and sets the selected object, item and model; a click on nothing resets the
   selection
   @deep read the whole function against 0x52ae70..0x52b29d (297 vs 298 ins): the clicked-state clears, the
   quick-jump lookups (two blocks, each FindObject / sprintf then sv_PanelFlag = 0), the click test
   (dm_L.lup, CamMove, view rect, hit window / widget), the nothing-picked reset, the sv_Clicked* copies,
   the door test (strcmp "tp_TUER" gives the original's mov ecx,8 / repe cmpsb; memcmp gives cmpsd), the
   camera focus with events[2], the character selection and the selected object / item / model: all match.
   Left, allocation / scheduling only: (1) at +0x31d the original pushes the snd_PlayWorkerClick argument
   first (push eax; mov cx,[eax]; mov [sv_SelectedChar],eax) and stores ai_SelectedCount = 1 after the
   selected byte, ours stores sv_SelectedChar / ai_SelectedCount first and pushes late; (2) the last goods
   kind test: original mov esi,eax; shl esi,6; add ecx,esi; add eax,ecx; cmp byte [eax],0x1d, ours mov
   edx,eax; shl edx,6; add ecx,edx; cmp byte [eax+ecx],0x1d (tried the expression direct and through cls) */
void sv_HandleObjects(void)
{
    char *subj;
    AIGOOD *cls;
    D3OBJECT *o;
    int found, door, perms;
    char msg[0x80];

    found = 0;
    door = 0;
    cls = 0;
    sv_ClickedBuilding = 0;
    sv_ClickedItem = 0;
    ai_ClickedObject = 0;
    sv_ClickedChar = 0;
    sv_ClickedContact = 0;
    if (sv_SelectedChar != 0 && ((AICHAR *)sv_SelectedChar)->selected == 0)
        sv_SelectedChar = 0;
    if (sv_PanelFlag != 0) {
        subj = ai_PlayerBuilding != 0 ? ai_PlayerBuilding : ai_FallbackBuilding;
        if (subj == sv_PanelObject
            && (sv_PanelShown == sv_PanelRecord
                || (ai_FallbackBuilding != 0 && sv_PanelShown == 0 && sv_PanelRecord != 0 && *sv_PanelRecord == 0xfd))) {
            if (sv_PanelText[0] != 0) {
                o = d3_FindObject(0, 0x60, 0, sv_PanelText);
                if (o != 0) {
                    ai_PickedObject = (char *)o;
                    found = 1;
                } else
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", sv_PanelText);
            }
            sv_PanelFlag = 0;
        }
        if (subj == sv_PanelObject && (sv_PanelRecord == 0 || *sv_PanelRecord == 0xfd)) {
            if (sv_PanelText[0] != 0) {
                o = d3_FindObject(0, 0x60, 0, sv_PanelText);
                if (o != 0) {
                    ai_PickedObject = (char *)o;
                    found = 1;
                } else
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", sv_PanelText);
            }
            sv_PanelFlag = 0;
        }
    }
    if ((dm_L.lup != 0 && d3t_CamMove == 0 && dm_L.x > sv_ViewRectLeft && dm_L.x < sv_ViewRectRight
         && dm_L.y > sv_ViewRectTop && dm_L.y < sv_ViewRectBottom && d2_HitWindow == -1 && d2_HitUnderPointer == -1)
        || found != 0) {
        if (ai_PickedObject != 0) {
            sv_ClickedBuilding = sv_HoverBuilding;
            sv_ClickedItem = sv_HoverItem;
            ai_ClickedObject = (int)ai_PickedObject;
            sv_ClickedChar = ai_PickedChar;
            sv_ClickedContact = sv_HoverContact;
            if (sv_HoverItem != 0)
                cls = &ai_GoodsT[((AIOBJ *)sv_HoverItem)->id];
            if (strcmp(ai_PickedObject, "tp_TUER") == 0 || (cls != 0 && cls->type == 6) || sv_HoverContact != 0)
                door = 1;
            if ((((D3OBJECT *)ai_PickedObject)->objectFlags & 1) != 0 && door == 0) {
                d3t_PlaceAtObject((D3OBJECT *)ai_PickedObject, ai_CameraStep);
                sv_FocusObject = ai_PickedObject;
                if (ai_PickedObject != 0 && ((D3OBJECT *)ai_PickedObject)->events != 0 && ((D3OBJECT *)ai_PickedObject)->events[2].fn != 0)
                    ((D3OBJECT *)ai_PickedObject)->events[2].fn((D3OBJECT *)ai_PickedObject, 2, ((D3OBJECT *)ai_PickedObject)->events[2].name, 0);
                sv_FocusDepth = sv_FrameDepth;
            }
            sv_LastPicked = ai_PickedObject;
            if (ai_PickedChar != 0 && ((AICHAR *)ai_PickedChar)->health != 0) {
                perms = gm_PermissionBits(ai_PlayerId, 0, 0, ai_PickedChar);
                sv_ClearSelection();
                if ((perms & 0x800) != 0 && (((AICHAR *)ai_PickedChar)->title != 0x1c || (((AICHAR *)ai_PickedChar)->flags1b4 & 1) == 0)) {
                    sv_SelectedChar = ai_PickedChar;
                    ai_CharsT[((AICHAR *)ai_PickedChar)->id].selected = 1;
                    ai_SelectedCount = 1;
                    sv_PanelCartChar = (int)ai_CharsT[((AICHAR *)ai_PickedChar)->id].model;
                    snd_PlayWorkerClick((struct G16CHARCLICK *)ai_PickedChar);
                }
                sv_PanelCartChar = (int)ai_CharsT[((AICHAR *)ai_PickedChar)->id].model;
            }
            if (ai_SelectedCount == 0 && sv_OtherSelected == 0 && sv_HoverBuilding != 0 && sv_SelectedItem == 0)
                sv_SelectedModel = (int)ai_PickedObject;
            sv_SelectedObject = (char *)sv_HoverBuilding;
            sv_SelectedItem = (unsigned short *)sv_HoverItem;
            if (sv_HoverBuilding != 0)
                sv_SelectedModel = (int)ai_PickedObject;
            if (sv_HoverItem != 0) {
                sv_SelectedModel = (int)ai_PickedObject;
                cls = &ai_GoodsT[((AIOBJ *)sv_HoverItem)->id];
                if (cls->type == 0x1d)
                    sv_ClearSelection();
            }
        } else if (sv_OtherSelected == 0)
            sv_ResetSelection();
    }
}

int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */

/* 0x52b500, runs the location scripts of the characters in ai_SceneChars that stand in the building a (their scene
   character's group is a's id, alive, in the default scene): a character gets the script of its activity
   (sv_ActivityName) or of its role ("besucher" for a visitor of a type 0x16 building's room 0x120, "bewohner" at
   home, "meister" / "pause" / "faulenzen" for an idle one at work), loaded from
   "<evt_Path>locations\<class>\<name><n>_<class>.esc" (n the first number no running script of the scene uses,
   else 0) and started with "sp_<netId>"; a character whose activity ended (the day window passed or its job's
   argc8 is set) stops its activity script. Every 500 ms it fires event 6 of the default scene first */
void sv_RunLocationScripts(AIBLD *a)
{
    AIGOOD *cls;
    AIBTYPE *kind;
    AICHAR *c, *e;
    AITASK *m;
    EVTSCRIPT *s;
    char *r;
    int season, i, j, ok, n, d;
    VFSDIR *found;
    char name[0x60];
    char cn[0x80];
    int used[8];
    char path[0x80];
    char sp[0x80];

    cls = 0;
    if (sv_PanelShown != 0 && *sv_PanelShown != 0xfd)
        cls = &ai_GoodsT[*sv_PanelShown];
    kind = &ai_BTypesT[a->type];
    if (d3_Time > sv_SceneEventTime + 500) {
        if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[6].fn != 0)
            d3_DefaultScene->events[6].fn(d3_DefaultScene, 6, d3_DefaultScene->events[6].name, 0);
        sv_SceneEventTime = d3_Time;
    }
    season = gm_ClockSeason(&ai_Time);
    for (i = 0; i < 0x20; i++) {
        c = (AICHAR *)ai_SceneChars[i];
        if (c == 0 || c->model == 0 || c->model->bldId != a->id || c->health <= 0 || c->model->scene != d3_DefaultScene)
            continue;
        ok = 0;
        name[0] = 0;
        if (sv_ActivityName(c, a, name) != 0) {
            if (kind->kind == 0xd) {
                if (sv_PanelShown != 0) {
                    if (*sv_PanelShown == 0x37 && c->model->scriptGroup == ((AIOBJ *)sv_PanelShown)->objId) {
                        ok = 1;
                        goto lstart;
                    }
                    if (*sv_PanelShown != 0xfd || c->model->scriptGroup != ((AIOBJ *)sv_PanelShown)->objId)
                        goto lstart;
                }
                ok = 1;
                if (c->job != 0 && c->job->argc8 != 0)
                    ok = 0;
            } else if (kind->kind == 8 && *sv_PanelShown == 0xdd && c->jobKind == 0x15) {
                ok = 1;
            } else {
                ok = 1;
                if (c->job != 0 && c->job->kind == 4 && c->job->argc8 != 0)
                    ok = 0;
            }
        } else if (kind->kind == 0x16 && sv_PanelShown != 0 && *sv_PanelShown == 0x120
                   && c->model->scriptGroup == ((AIOBJ *)sv_PanelShown)->objId && (c->flags1c8 & 0x1000) == 0) {
            strcpy(name, "besucher");
            ok = 1;
        } else if (kind->kind == 1 && c->bld2 == a && (c->flags1c8 & 0x1000) == 0) {
            strcpy(name, "bewohner");
            ok = 1;
        } else if (c->jobKind == 0 && c->bld == a && c->model->scriptId == -1) {
            if (c->jobId != 0)
                strcpy(name, "meister");
            else if (c->title == 0x1c && (c->flags1b4 & 1) != 0)
                strcpy(name, "faulenzen");
            else
                strcpy(name, "pause");
            ok = 1;
        }
lstart:
        if (c->title == 0x1c && (c->flags1b4 & 1) != 0 && c->model->scriptId == -1)
            strcpy(name, "faulenzen");
        else if (!ok)
            goto lstop;
        if (name[0] != 0) {
            m = c->model;
            if (m->scriptId != -1 && evt_FindById(m->scriptId) != 0)
                continue;
            if (m->scriptId != -1)
                continue;
            for (n = 0; n < 8; n++)
                used[n] = 1;
            for (j = 0; j < 0x20; j++) {
                e = (AICHAR *)ai_SceneChars[j];
                if (e != 0 && e->model != 0 && e->model->scriptId != -1 && (s = evt_FindById(e->model->scriptId)) != 0
                    && (r = strstr(s->name, name)) != 0) {
                    d = r[strlen(name)] - '0';
                    if (d < 0 || d > 9)
                        break;
                    used[d] = 0;
                }
            }
            for (n = 0; n < 8; n++)
                if (used[n] != 0)
                    break;
            if (c->jobKind == 0x16)
                strcpy(cn, "Training_Allgemein");
            else if (cls == 0)
                init_BuildingKindName(a->type, cn);
            else
                strcpy(cn, cls->name);
            sprintf(path, "%slocations\\%s\\%s%i_%s.esc", evt_Path, cn, name, n, cn);
            if (vfs_FindFile(path, vfs_Root, &found) == 0)
                sprintf(path, "%slocations\\%s\\%s0_%s.esc", evt_Path, cn, name, cn);
            if (vfs_FindFile(path, vfs_Root, &found) != 0) {
                sprintf(sp, "sp_%i", c->netId);
                s = evt_LoadScript(path);
                if (s != 0) {
                    evt_RunScriptArgs(s, 1, sp);
                    s->character = (CHARACTER *)m;
                    m->scriptId = s->id;
                }
            }
            continue;
        }
lstop:
        ok = 0;
        if (c->bld != a || c->model == 0 || c->model->scriptId == -1)
            continue;
        if (c->jobKind == 0 || (float)ai_Time.hour > gm_DayEndHours[season])
            ok = 1;
        if ((c->job != 0 && c->job->kind == 4 && c->job->argc8 != 0) || ok) {
            ok = 0;
            s = evt_FindById(c->model->scriptId);
            if (s != 0) {
                for (j = 0; j < 0x30; j++) {
                    if (ok)
                        break;
                    if (sv_Activities[j].kind != 0 && sv_Activities[j].name[0] != 0 && strstr(s->name, sv_Activities[j].name) != 0)
                        ok = 1;
                }
                if (ok)
                    evt_Stop(s);
            }
        }
    }
}
/* 0x52bb70, runs the key handler 0x5309a0 (0, keys & ~0xc0 | 0x100200) for as long as the object id (0x4284a0) is flagged +0x124 & 1 */

void sv_RunFramesUntilStopped(int id)
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
/* 0x52bbc0, sends the selected characters of the first selected one's group job ('C') to the clicked building
   (sv_ClickedBuilding): a member already belonging to it is moved in at once (sv_MoveToGroup, its actions stopped and
   a goto message to the building's room object 0x146), the others (up to four) are collected into one 'C' message to
   the player with the clicked and the owning building's ids; each leaves the job's member slots and the selection.
   The job is announced (cm_SendExSetHE) afterwards
   @deep read the whole function against 0x52bbc0..0x52beb5 (218 vs 217 ins): the selected-character loop
   (signed i < 768, the done test first), the first member fixing the group job and its kind 'C', the
   not-in-the-building branch first (the 'C' AIMSG: type, flags 2, to = the player's netId, arg[0] / arg[1]
   the building ids, the ids from arg[6] with the -1 fill as rep stosd), the in-the-building branch
   (gm_Query room 0x146, member slot cleared, sv_MoveToGroup, cm_SendChrStop, cm_SendChrGotoBuilding, cur
   reloaded), the second-member branch with done after four, the selected flag cleared, cm_SendExSetHE /
   cm_SendLongMessage: all equal. Left, allocation only (asmdiff --regs ebx=ebp,edx=ebx,ebp=edx leaves only
   these): the original keeps cur in ebx and the job in ebp (ours edx / ebx, so ours zeroes ebx for
   msg.type), keeps out in edi across the loop (ours reloads [esp+0x14] in the second-member branch) and
   loads the job's kind into dl before the 0x43 store; the netId register (edx vs ebx / edi) */
void sv_SendSelectedToBuilding(void)
{
    AIJOB *job;
    AICHAR *c;
    AIBLD *owner, *cur;
    AIOBJ *room;
    int *out, *p;
    int n, done, kind, i, k;
    AIMSG msg;

    n = 0;
    done = 0;
    kind = 0;
    job = 0;
    owner = 0;
    cur = (AIBLD *)sv_ClickedBuilding;
    msg.type = 0;
    out = &msg.arg[6];
    for (i = 0; i < 768; i++) {
        if (done)
            break;
        c = &ai_CharsT[i];
        if (c->selected == 0)
            continue;
        if (kind == 0) {
            if (c->job != 0 && c->bld != 0) {
                kind = 'C';
                job = c->job;
                if (job->kind == 'C') {
                    if (c->bld != cur) {
                        msg.type = 'C';
                        msg.flags = 2;
                        owner = c->bld;
                        msg.to = ai_CharsT[ai_PlayerId].netId;
                        msg.arg[0] = cur->id;
                        msg.arg[1] = owner->id;
                        *out++ = c->netId;
                        n++;
                        for (p = out, k = n; k < 4; k++)
                            *p++ = -1;
                        for (k = 0; k < 4; k++) {
                            if (job->members[k] == c->netId) {
                                job->members[k] = -1;
                                cur = (AIBLD *)sv_ClickedBuilding;
                                break;
                            }
                        }
                    } else {
                        room = (AIOBJ *)gm_Query((void *)cur->objList, 1, 0, 0x146);
                        for (k = 0; k < 4; k++) {
                            if (job->members[k] == c->netId) {
                                job->members[k] = -1;
                                break;
                            }
                        }
                        sv_MoveToGroup((unsigned char *)sv_ClickedBuilding, 0, c->id, 0);
                        cm_SendChrStop(c->netId);
                        cm_SendChrGotoBuilding(c->netId, ((AIBLD *)sv_ClickedBuilding)->id, room->objId, 0, 0, 0);
                        cur = (AIBLD *)sv_ClickedBuilding;
                    }
                }
            }
        } else if (kind == 'C' && c->bld == owner && c->job == job) {
            if (c->bld != cur) {
                *out++ = c->netId;
                n++;
                for (k = 0; k < 4; k++) {
                    if (job->members[k] == c->netId) {
                        job->members[k] = -1;
                        cur = (AIBLD *)sv_ClickedBuilding;
                        break;
                    }
                }
                if (n > 3)
                    done = 1;
            } else {
                room = (AIOBJ *)gm_Query((void *)cur->objList, 1, 0, 0x146);
                for (k = 0; k < 4; k++) {
                    if (job->members[k] == c->netId) {
                        job->members[k] = -1;
                        break;
                    }
                }
                sv_MoveToGroup((unsigned char *)sv_ClickedBuilding, 0, c->id, 0);
                cm_SendChrStop(c->netId);
                cm_SendChrGotoBuilding(c->netId, ((AIBLD *)sv_ClickedBuilding)->id, room->objId, 0, 0, 0);
                cur = (AIBLD *)sv_ClickedBuilding;
            }
        }
        c->selected = 0;
    }
    if (job != 0)
        cm_SendExSetHE(job->stage, (char *)job);
    if (msg.type != 0)
        cm_SendLongMessage((char *)&msg);
}
#define sv_SelectionBytes (*(unsigned char (*)[0x300])&sv_SelectionFlags)  /* 0x75e920, the flags as the bytes they are, one per character */
/* 0x52bec0, the selection's mouse handling (every frame): a held left button ends a rubber band (sv_EndBand) or,
   with the right button up, starts one in the player's own building (sv_StartBand). With characters selected a
   release sends them: with nothing clicked to the map cell under the pointer (d3sm_Pick on the default scene's
   height map; the first of nine neighbour cells inside the map whose ground is neither 0 nor the border 0xd, else
   the cell itself), each through cm_SendChrWalkTo after leaving its group and stopping its script; with a clicked
   contact the selected scene characters to it (cm_SendChrStop, cm_SendChrUseGates of the current building and the
   contact's room); with a clicked building through sv_SendSelectedToBuilding. Then it rebuilds the selection
   state: the grey look of the characters whose flag changed, sv_SelectedChar, ai_SelectedCount and the pointer
   items of those in their own building; a right release drops the selection
   @deep read the whole function against 0x52bec0..0x52c413 (396 vs 391 ins): the band end / start (lhold,
   rdown, sv_BandActive, di_Absolute, the own building), the four clicked-state tests with the original's
   threading (contact -> group, object / item -> building test, building -> send), the map-cell send over
   ai_CharsT (selected, alive, permission 0x800, model; the off[9][2] initializer per character, the spiral
   index i kept across characters and cell set once, the 50.0 / cell size scaling, the fallback to the
   picked cell after 8, sv_MoveToGroup, the script stop, cm_SendChrWalkTo), the contact send over
   ai_SceneChars (the dummy_ name, the 0xfd room via init_BuildingSceneUniverse, the building choice,
   cm_SendChrUseGates), sv_SendSelectedToBuilding, and the rebuild loop (the grey look by the CHARACTER
   fading bit, sv_SelectedChar, the pointer items, the byte flags): all equal. Left, allocation: the
   original holds the constants 0 / -1 / 1 in ebx / edi / ebp all through (cmp [mem],ebx; rematerialized
   after each send) and keeps map in [esp+0x14], cell in ebx; ours holds 0 in edi, map in ebx and cell in a
   slot (frame 0x98 vs 0x94), so the spiral loop stores cell and jumps once more and the character pointer
   is biased by 8 */
void sv_HandleSelectionMouse(void)
{
    D3SM *map;
    AIBLD *b;
    AICHAR *c;
    int cx, cy, i, j, ix, iy, size, cell, t;

    cx = -1;
    cy = -1;
    cell = 0;
    map = (D3SM *)d3_DefaultScene->hmap;
    b = 0;
    if (ai_PlayerBuilding != 0)
        b = (AIBLD *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        b = (AIBLD *)ai_FallbackBuilding;
    if (dm_L.lhold == 0 && sv_BandActive != 0) {
        snd_PlaySelectedClick();
        sv_EndBand();
    }
    if (dm_L.lhold != 0 && dm_L.rdown == 0) {
        if (sv_BandActive == 0 && di_Absolute != 0 && d2_HitWindow == -1 && b != 0 && b->owner == ai_PlayerId)
            sv_StartBand();
    } else if (ai_SelectedCount != 0) {
        if (sv_ClickedContact == 0 && ai_ClickedObject == 0 && sv_ClickedItem == 0 && sv_ClickedBuilding == 0
            && map != 0 && b != 0 && dm_L.lup != 0 && d2_HitWindow == -1) {
            i = 0;
            for (j = 0; j < 768; j++) {
                c = &ai_CharsT[j];
                if (c->selected != 0 && c->health != 0 && (gm_PermissionBits(ai_PlayerId, 0, 0, (int)c) & 0x800) != 0
                    && c->model != 0) {
                    int off[9][2] = { {0, 0}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };

                    if (d3sm_Pick(map, dm_L.x, dm_L.y, &cx, &cy) != 0) {
                        size = map->size;
                        do {
                            ix = (int)(off[i][0] * (50.0 / map->cell.x) + cx);
                            iy = (int)(off[i][1] * (50.0 / map->cell.z) + cy);
                            if (ix < size && iy < size)
                                cell = map->entries[size * iy + ix].type;
                            if (++i >= 8) {
                                ix = cx;
                                iy = cy;
                                break;
                            }
                        } while (ix < 0 || ix >= size || iy < 0 || iy >= size || cell == 0 || cell == 0xd);
                        if (c->bld != 0)
                            sv_MoveToGroup((unsigned char *)c->bld, 0, c->id, 0);
                        if (c->model->scriptId != -1 && (t = (int)evt_FindById(c->model->scriptId)) != 0)
                            evt_Stop((EVTSCRIPT *)t);
                        cm_SendChrWalkTo(c->netId, ix, iy, d3_CurrentScene);
                    }
                }
            }
        }
        if (sv_ClickedContact != 0 && map != 0 && dm_L.lup != 0 && d2_HitWindow == -1) {
            char name[0x80];

            t = -1;
            sprintf(name, "dummy_%s", sv_ClickedObjectT->name + 8);
            if (d3_FindObject((int)sv_ClickedObjectT, 0x100, 0, name) == 0)
                strcpy(name, sv_ClickedObjectT->name);
            for (j = 0; j < 0x20; j++) {
                c = (AICHAR *)ai_SceneChars[j];
                b = 0;
                if (c != 0 && c->selected != 0) {
                    cm_SendChrStop(c->netId);
                    if (c->bld != 0)
                        sv_MoveToGroup((unsigned char *)c->bld, 0, c->id, 0);
                    if (((AIOBJ *)sv_ClickedContact)->id == 0xfd) {
                        if (ai_FallbackBuilding != 0) {
                            t = init_BuildingSceneUniverse((unsigned char *)ai_FallbackBuilding);
                            if (ai_FallbackBuilding != 0)
                                b = (AIBLD *)ai_FallbackBuilding;
                            else if (ai_PlayerBuilding != 0)
                                b = (AIBLD *)ai_PlayerBuilding;
                        } else if (ai_PlayerBuilding != 0) {
                            t = 0;
                            if (ai_FallbackBuilding != 0)
                                b = (AIBLD *)ai_FallbackBuilding;
                            else if (ai_PlayerBuilding != 0)
                                b = (AIBLD *)ai_PlayerBuilding;
                        }
                    } else {
                        t = init_ObjectSceneUniverse((unsigned short *)sv_ClickedContact);
                        if (ai_FallbackBuilding != 0)
                            b = (AIBLD *)ai_FallbackBuilding;
                        else if (ai_PlayerBuilding != 0)
                            b = (AIBLD *)ai_PlayerBuilding;
                    }
                    if (t >= 0)
                        cm_SendChrUseGates(c->netId, b->id, ((AIOBJ *)sv_ClickedContact)->objId);
                }
            }
            sv_ClearSelection();
            sv_ClickedContact = 0;
        }
        if (sv_ClickedBuilding != 0)
            sv_SendSelectedToBuilding();
    }
    ai_SelectedCount = 0;
    d2_ClearPointerItems();
    for (j = 0; j < 768; j++) {
        c = &ai_CharsT[j];
        if (c->model != 0) {
            if (c->selected != sv_SelectionBytes[j] && ((CHARACTER *)c->model)->flags.fading == 0) {
                if (c->selected != 0)
                    d3t_SetGrey(((CHARACTER *)c->model)->obj, 1);
                else
                    d3t_SetGrey(((CHARACTER *)c->model)->obj, 0);
            }
            if (c->selected != 0) {
                if (ai_SelectedCount == 0)
                    sv_SelectedChar = (int)c;
                ai_SelectedCount++;
                if (c->bld != 0 && c->bld->id == c->model->bldId
                    && (ai_FallbackBuilding != 0 || (sv_PanelShown != 0 && *sv_PanelShown != 0x120)))
                    d2_AddPointerItem(c->picture, 1);
            }
        }
        sv_SelectionBytes[j] = c->selected;
    }
    if (dm_L.rup != 0 && ai_SelectedCount != 0)
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
/* 0x52c4e0, shows a speech bubble over the character ch (only one of the current scene, and one bubble per character): the text id
   text, or the own text name, goes into a free sv_Bubbles record, else into the last one stamped before now; ch's delete
   callback becomes sv_ReleaseBubble
   @deep read whole against the original: the three loops (own entry, free entry with the cmp 0x40 after
   the break, last entry stamped before d3_Time), the stores time/ch/text, onDelete, strcpy/empty name
   all match; SVBUBBLE typed. Left: VC6 keeps d3_Time in esi from inside loop 2 (0x52c524 mov
   esi,[d3_Time]) through the time store (mov [eax+4],esi), saving esi in the prologue; the original
   loads it in the expiry block only (push esi; mov esi,[d3_Time] at 0x52c53c .. pop esi 0x52c562) and
   reloads it for the store (0x52c572 mov ecx,[d3_Time]). Tried: a local now, indexed stores without b,
   goto found, i == 0x40: all equal or worse. The branch-target hunks follow from the 5 byte
   prologue/epilogue size. */

void sv_ShowBubble(CHARACTER *ch, int text, char *name)
{
    int i, j;
    SVBUBBLE *b;

    if (ch == 0 || ch->scene != d3_DefaultScene)
        return;
    for (i = 0; i < 0x40; i++) {
        if (sv_Bubbles[i].ch == (struct CHARACTER *)ch)
            return;
    }
    for (i = 0; i < 0x40; i++) {
        if (sv_Bubbles[i].ch == 0)
            break;
    }
    if (i >= 0x40) {
        i = -1;
        for (j = 0; j < 0x40; j++) {
            if (sv_Bubbles[j].time < d3_Time)
                i = j;
        }
        if (i == -1)
            return;
    }
    b = &sv_Bubbles[i];
    b->time = d3_Time;
    b->ch = (struct CHARACTER *)ch;
    b->text = text;
    ch->onDelete = (void (*)(struct CHARACTER *))sv_ReleaseBubble;
    if (name != 0)
        strcpy(b->name, name);
    else
        b->name[0] = 0;
}
/* 0x52c5c0, on a left click lets the character under the pointer speak (ai_PickedChar, else the character of a picked
   character object, a person only; with a building shown only in its kind 0x0f, or in kind 0x16 on room 0x120): a
   member of a job says a line on the job (txt_Format by the job kind, with the character or building it names), the
   others one of four topics 0x11e9 + kind * 15 (a title group 0xb / 0xc never topic 3) graded by a rating of the
   statistics record or ord_BuildingCapacityRatio (5 grades of 3 lines); with an active history key event (0x52) every
   second time one of the lines of the history entry before hst_KeyNext
   @deep reconstructed and read whole against the original (the pick checks, the job-kind switch in the
   original's case order with txt_Format varargs, the topic grading 3.0*(x*5.0)+id, the history key lines,
   the bubble calls); instruction count equal (633), switch tables equal. Left is block placement only: the
   plain-topic block 'kind = util_RandomU(4) + 1' of a character without a title group 0xb/0xc lies in the
   original out of line after the job switch dispatch (0x52c77c push 4 .. jmp 0x52c73f), ours inline before
   the 0xb/0xc block (0x52c71f .. jmp 0x52c755); plus an order hunk at 0x52c6d8 (the id store vs stosw).
   Tried: inverted condition, goto into the else, a switch on v: same or worse. */

void sv_SpeakPicked(void)
{
    AIBTYPE *cls;
    AIBLD *b;
    AIBLD *r;
    AICHAR *c;
    AIJOB *job;
    CHARACTER *ch;
    int v, kind, id, n;
    char plain;

    if (ai_PickedChar == 0) {
        if (ai_PickedObject == 0 || ((D3OBJECT *)ai_PickedObject)->kind != 2 || sv_HoverItem != 0)
            return;
    }
    if (dm_L.lup == 0)
        return;
    b = 0;
    cls = 0;
    if (ai_PlayerBuilding != 0)
        b = (AIBLD *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        b = (AIBLD *)ai_FallbackBuilding;
    if (b != 0)
        cls = &ai_BTypesT[b->type];
    if (ai_PickedChar != 0)
        gm_PermissionBits(ai_PlayerId, 0, 0, ai_PickedChar);
    if (ai_PickedChar != 0)
        ch = (CHARACTER *)((AICHAR *)ai_PickedChar)->model;
    else if (ai_PickedObject != 0)
        ch = ch_FindByObject((D3OBJECT *)ai_PickedObject);
    else
        ch = 0;
    if (ch != 0 && (ch->kind & 1) == 0)
        return;
    if (b != 0 && (cls->kind != 0x16 || *sv_PanelShown != 0x120) && cls->kind != 0x0f)
        return;
    util_Random(4);
    {
        char text[0x200] = "";
        float st[10];

        id = -1;
        if (ai_PickedChar != 0) {
            c = (AICHAR *)ai_PickedChar;
            plain = 1;
            v = 0;
            if (c->jobId != 0)
                v = gm_TitleGroup(c->jobId);
            else if (c->title != 0)
                v = gm_TitleJobGroup((unsigned char)c->title);
            if (v == 0xb || v == 0xc) {
                plain = 0;
                kind = util_RandomU(4) + 1;
                if (kind == 3)
                    kind = 4;
            } else {
                kind = util_RandomU(4) + 1;
            }
            c = (AICHAR *)ai_PickedChar;
            job = c->job;
            if (job != 0) {
                kind = -1;
                switch (c->jobKind) {
                case 0x49:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->u.id)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x124a, r->owner, r);
                    break;
                case 0x44:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->u.id)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1259, r->owner, r);
                    break;
                case 0x28:
                    txt_Format(text, util_RandomU(3) + 0x124d);
                    break;
                case 0x3c:
                    if ((c = (AICHAR *)gm_FindChar(job->u.id)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1253, c->id);
                    break;
                case 0x14:
                    if (c->bld != 0) {
                        if (ai_BTypesT[c->bld->type].kind == ' ')
                            txt_Format(text, util_RandomU(3) + 0x1286);
                        else
                            txt_Format(text, util_RandomU(3) + 0x1256);
                    }
                    break;
                case 0x19:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->u.id)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x125c, r->owner, r);
                    break;
                case 0x62:
                    txt_Format(text, util_RandomU(3) + 0x125f);
                    break;
                case 0x18:
                    if ((c = (AICHAR *)gm_FindChar(job->id4)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1262, c->id);
                    break;
                case 0x40:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->u.id)) != 0 && r->owner != 0xffff)
                        txt_Format(text, util_RandomU(3) + 0x1265, r->owner, r);
                    break;
                case 0x43:
                    txt_Format(text, util_RandomU(3) + 0x1268);
                    break;
                case 0x61:
                    txt_Format(text, util_RandomU(3) + 0x126b);
                    break;
                case 0x48:
                    txt_Format(text, util_RandomU(3) + 0x126e);
                    break;
                case 100:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->id2)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1271, r->owner, r);
                    break;
                case 0x65:
                    txt_Format(text, util_RandomU(3) + 0x1274);
                    break;
                case 0x1a:
                    if ((c = (AICHAR *)gm_FindChar(job->id3)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1277, c->id);
                    break;
                case 0x3f:
                case 0x75:
                    if ((r = (AIBLD *)gm_FindBuildingById(job->u.id)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x1250, r->owner, r);
                    break;
                case 0x89:
                    if ((c = (AICHAR *)gm_FindChar(job->id2)) != 0)
                        txt_Format(text, util_RandomU(3) + 0x127a, c->id);
                    break;
                case 0x8e:
                    txt_Format(text, util_RandomU(3) + 0x127d);
                    break;
                case 0xa1:
                    txt_Format(text, util_RandomU(3) + 0x1280);
                    break;
                case 0x8c:
                    if (c->bld != 0) {
                        cls = &ai_BTypesT[c->bld->type];
                        if (cls->kind == 0x1d)
                            txt_Format(text, util_RandomU(3) + 0x1283);
                        else if (cls->kind == 0x1f)
                            txt_Format(text, util_RandomU(3) + 0x1289);
                    }
                    break;
                default:
                    kind = util_RandomU(4) + 1;
                    if (!plain && kind == 3)
                        kind = 4;
                    break;
                }
            }
        } else {
            kind = util_RandomU(4) + 1;
        }
        if (kind >= 0 && kind <= 4)
            id = kind * 15 + 0x11e9;
        if (util_RandomU(2) == 0 && he_FindEvent(1, 0, 0x52) != 0)
            kind = 5;
        ord_GetStatistics((int *)st);
        switch (kind) {
        case 1:
            id = (int)(3.0 * (st[1] * 5.0) + id);
            id += util_RandomU(3);
            break;
        case 2:
            id = (int)(3.0 * (ord_BuildingCapacityRatio() * 5.0) + id);
            id += util_RandomU(3);
            break;
        case 3:
            id = (int)(3.0 * (st[2] * 5.0) + id);
            id += util_RandomU(3);
            break;
        case 4:
            id = (int)(3.0 * (st[0] * 5.0) + id);
            id += util_RandomU(3);
            break;
        case 5:
            if (hst_KeyNext != -1) {
                for (n = 1; n < 5; n++) {
                    if (*txt_Ptrs[hst_KeyNext - 5 + n] == 0)
                        break;
                }
                id = util_RandomU(n - 1) + hst_KeyNext - 5;
            }
            break;
        }
        if (ai_PickedChar != 0) {
            if (id != -1) {
                sv_ShowBubble((CHARACTER *)((AICHAR *)ai_PickedChar)->model, id, 0);
                return;
            }
            if (text[0] != 0)
                sv_ShowBubble((CHARACTER *)((AICHAR *)ai_PickedChar)->model, 0, text);
        } else if (ai_PickedObject != 0) {
            sv_ShowBubble(ch_FindByObject((D3OBJECT *)ai_PickedObject), id, 0);
        }
    }
}
/* 0x52cef0, frees the speech bubbles shown more than 300 ms ago and draws the others as text boxes (font 0x54, 0xa0 x 0x60)
   above the screen box of their character's body (its low-poly body while the body is hidden, type 1), for the
   characters of the current scene; the text is the bubble's own or the d2_Strings line
   @deep read whole against the original: the expiry loop (time + 300u unsigned against d3_Time), the
   draw loop with the body / low-poly body screen box (two ScreenBox calls, cross-jumped as in the
   original), the two DrawTextBox calls (own text / d2_Strings), AddDirty; instruction count 91/92. Left:
   the draw loop's induction pointer is sv_Bubbles+0xc (name) in ours, +8 (ch) in the original (mov
   eax,[ebx-4] vs [ebx]; mov al,[ebx] vs mov cl,[ebx+4] / lea eax,[ebx+4]), d3_Time hoisted out of the
   expiry loop in ours (0x52cef0) and loaded per entry in the original (0x52cf00), d2_ScreenSurf loaded
   before the text branch in ours. Tried: a SVBUBBLE pointer per entry, a pointer walk (counter loop:
   worse), a text pointer variable, a ternary condition: same or worse. d2_AddDirty is K&R in sv's head
   (Open: d2.h unsettled), M7 stays open. */

void sv_DrawBubbles(void)
{
    int i, x, y;
    CHARACTER *ch;
    D3OBJECT *low;
    D3RECT box;

    for (i = 0; i < 0x40; i++) {
        if (sv_Bubbles[i].ch != 0 && d3_Time > sv_Bubbles[i].time + 300u)
            sv_Bubbles[i].ch = 0;
    }
    d2_SetFont(0x54);
    for (i = 0; i < 0x40; i++) {
        ch = (CHARACTER *)sv_Bubbles[i].ch;
        if (ch == 0 || ch->obj->scene != d3_DefaultScene)
            continue;
        if (ch->obj->type != 1) {
            if (d3if_ScreenBox(ch->obj, &box) == 0)
                continue;
        } else {
            low = ch->lowPolyBody;
            if (low == 0 || low->type == 1 || d3if_ScreenBox(low, &box) == 0)
                continue;
        }
        y = box.top - 0x40;
        x = (box.right - box.left) / 2 - 0x50 + box.left;
        if (sv_Bubbles[i].name[0] != 0)
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, sv_Bubbles[i].name, 0x28);
        else
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, d2_Strings[sv_Bubbles[i].text], 0x28);
        d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
    }
}
/* 0x52d1e0, draws the text of every name plate (sv_NamePlates) above the screen box of its object (one with mesh data
   or of type 0), then the names of the contact gates of the current scene (sv_DrawGateName)
   @deep read whole against the original: font 0x54, the 32 plates (object with mesh data or of type 0,
   screen box), the text box and dirty rect, then d3_ForEachObject2(d3_DefaultScene, 0, 0x20,
   sv_DrawGateName); 65 vs 66 instructions. Left is the loop's induction pointer: ours walks
   sv_NamePlates+0x48 (the text, mov ebx,sv_NamePlates+0x48 / mov eax,[ebx-0x48] / push ebx), the original
   sv_NamePlates (the object, mov ebx,sv_NamePlates / lea edx,[ebx+0x48]), as in sv_DrawBubbles; plus the
   register rotation of x / y / TextDrawnW,H (regs 18) and the pushed ebx. d2_AddDirty is K&R in sv's head
   (Open: d2.h unsettled), M7 stays open. */

void sv_DrawNamePlates(void)
{
    int i, x, y;
    D3OBJECT *o;
    D3RECT box;

    d2_SetFont(0x54);
    for (i = 0; i < 0x20; i++) {
        o = (D3OBJECT *)sv_NamePlates[i].obj;
        if (o != 0 && (o->info != 0 || o->type == 0) && d3if_ScreenBox(o, &box) != 0) {
            y = box.top;
            x = (box.right - box.left) / 2 - 0x50 + box.left;
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, sv_NamePlates[i].text, 0x28);
            d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
        }
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0x20, sv_DrawGateName);
}
/* 0x52d2a0, draws the names of the objects in the shown room (sv_PanelShown's list; without one the list of the room
   object 0xfd of ai_FallbackBuilding, else the building's own) above their scene objects: first the things, then
   the persons (goods types 6 / 2) */

void sv_DrawItemNames(void)
{
    int list, x, y;
    AIGOOD *g;
    AIOBJ *q;
    D3RECT box;

    if (sv_PanelShown != 0) {
        list = ((AIOBJ *)sv_PanelShown)->list;
    } else {
        if (ai_FallbackBuilding == 0)
            return;
        q = (AIOBJ *)gm_Query((void *)((AIBLD *)ai_FallbackBuilding)->objList, 1, 0, 0xfd);
        if (q != 0)
            list = q->list;
        else
            list = ((AIBLD *)ai_FallbackBuilding)->objList;
    }
    d2_SetFont(0x54);
    for (q = (AIOBJ *)gm_Query((void *)list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        g = &ai_GoodsT[q->id];
        if (q->obj != 0 && g->type != 6 && g->type != 2 && d3if_ScreenBox(q->obj, &box) != 0) {
            y = box.top;
            x = (box.right - box.left) / 2 - 0x50 + box.left;
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, (char *)txt_ObjectNames[q->id][0], 0x28);
            d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
        }
    }
    if (sv_PanelShown != 0)
        list = ((AIOBJ *)sv_PanelShown)->list;
    else if (ai_FallbackBuilding != 0)
        list = ((AIBLD *)ai_FallbackBuilding)->objList;
    for (q = (AIOBJ *)gm_Query((void *)list, 2, 5, 6); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        g = &ai_GoodsT[q->id];
        if (q->obj != 0 && (g->type == 6 || g->type == 2) && d3if_ScreenBox(q->obj, &box) != 0) {
            y = box.top;
            x = (box.right - box.left) / 2 - 0x50 + box.left;
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, (char *)txt_ObjectNames[q->id][0], 0x28);
            d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
        }
    }
}
/* 0x52d010, the d3_ForEachObject2 callback of 0x52d1e0 (scene objects of mask 0x20): for a "gate" part of the shown
   building (ai_FallbackBuilding, else ai_PlayerBuilding) finds the item named by the part's name from +0x10
   ("contact_gate_<item>") among the building's objects (kind 6 in a fallback building, else 2), else the room
   object 0xfd of a fallback building, and draws its name above the part's screen box: the item's name with
   txt_GateItemSuffix, for the room object the shown room's or the building kind's name with txt_GatePlaceSuffix
   @deep read whole against the original: the building choice, strstr "gate", the item search over gm_Query
   kind 6 / 2 with goto found onto the it == 0 test, the room object 0xfd of a fallback building, the three
   cross-jumped sprintf "%s %s" (txt_ObjectNames / ai_BuildingNames[type*14-2] with txt_GatePlaceSuffix /
   txt_GateItemSuffix), the screen box text (buf 0x100, frame equal 0x170); 148 vs 151 instructions. Left
   is allocation: the building pointer stays in eax until after strstr in the original (mov edi,eax at
   0x52d03c), ours loads edi directly; after the found label 'it' lives in eax in the original (stub
   0x52d11f mov eax,esi; jmp 0x52d0e2), in esi in ours (no stub), which moves the three sprintf blocks (eax
   vs esi / ecx vs edx); the goods name address lea edx,[eax+ecx+1] vs add eax,ecx; inc eax; the y /
   d2_ScreenSurf / buf loads around DrawTextBox in other registers. d2_AddDirty is K&R in sv's head (Open:
   d2.h unsettled), M7 stays open. */

void sv_DrawGateName(D3OBJECT *part)
{
    AIBLD *b;
    unsigned short *it;
    char name[0x60];
    char buf[0x100];
    D3RECT box;
    int x, y;

    b = (AIBLD *)ai_FallbackBuilding;
    if (b == 0) {
        b = (AIBLD *)ai_PlayerBuilding;
        if (b == 0)
            return;
    }
    if (strstr(part->name, "gate") == 0)
        return;
    strcpy(name, part->name + 0x10);
    if (ai_FallbackBuilding != 0)
        it = (unsigned short *)gm_Query((void *)b->objList, 1, 4, 6);
    else
        it = (unsigned short *)gm_Query((void *)b->objList, 1, 4, 2);
    for (; it != 0; it = gm_QueryNext()) {
        if (_strcmpi(name, ai_GoodsT[*it].name) == 0)
            goto found;
    }
    if (ai_FallbackBuilding == 0)
        return;
    it = (unsigned short *)gm_Query((void *)b->objList, 1, 0, 0xfd);
found:
    if (it == 0)
        return;
    if (*it == 0xfd) {
        if (sv_PanelShown != 0)
            sprintf(buf, "%s %s", (char *)txt_ObjectNames[*sv_PanelShown][0], txt_GatePlaceSuffix);
        else
            sprintf(buf, "%s %s", (char *)ai_BuildingNames[b->type * 0xe - 2], txt_GatePlaceSuffix);
    } else {
        sprintf(buf, "%s %s", (char *)txt_ObjectNames[*it][0], txt_GateItemSuffix);
    }
    if (d3if_ScreenBox(part, &box) != 0) {
        y = box.top;
        x = (box.right - box.left) / 2 - 0x50 + box.left;
        d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, buf, 0x28);
        d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
    }
}
/* 0x52d480, with the frame flag 8 and the panel permission 0x1000 (of the shown room, and ai_BuildingPerms): draws the
   border arrows of sv_BorderCameraView (picture 0x52c) for the border view sv_BorderViewX / sv_BorderViewY: at the
   bottom (frames 0 / 1), left (2 / 3), right (4 / 5) and top (6 / 7) border, the first frame of a pair where the
   camera's preset view on that side is set (its position not d3_Origin), the second where it is not; on the
   middle row / column only the set views get an arrow (the second frame) */

void sv_DrawBorderArrows(void)
{
    void *pic;
    char *b;
    int bottom, left, right, top;

    right = 0;
    left = 0;
    bottom = 0;
    top = 0;
    if ((sv_FrameMask & 8) == 0)
        return;
    b = ai_PlayerBuilding;
    if ((b != 0 || (ai_FallbackBuilding != 0 && (b = ai_FallbackBuilding) != 0)) && (sv_PanelShown != 0 || ai_FallbackBuilding != 0)) {
        sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)b, sv_PanelShown, 0);
        if ((sv_PanelPerms & 0x1000) == 0)
            return;
    }
    if ((ai_BuildingPerms & 0x1000) == 0)
        return;
    pic = d2_GetPic(0x52c);
    if (sv_BorderViewX == -1) {
        right = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            right = 2;
    } else if (sv_BorderViewX == 1) {
        left = 1;
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            left = 2;
    } else if (sv_BorderViewX == 0) {
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            left = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            right = 1;
    }
    if (sv_BorderViewY == -1) {
        bottom = 1;
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            bottom = 2;
    } else if (sv_BorderViewY == 1) {
        top = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            top = 2;
    } else if (sv_BorderViewY == 0) {
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            bottom = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            top = 1;
    }
    d2_SetClipRect(0, 0, ((int *)d2_ScreenSurf)[1], ((int *)d2_ScreenSurf)[2]);
    if (bottom == 2)
        d2_DrawShape(d2_HalfScreenW - 0x10, d2_ScreenH - 0x20, d2_ScreenSurf, pic, 0);
    else if (bottom == 1)
        d2_DrawShape(d2_HalfScreenW - 0x10, d2_ScreenH - 0x20, d2_ScreenSurf, pic, 1);
    if (left == 2)
        d2_DrawShape(4, d2_HalfScreenH, d2_ScreenSurf, pic, 2);
    else if (left == 1)
        d2_DrawShape(4, d2_HalfScreenH, d2_ScreenSurf, pic, 3);
    if (right == 2)
        d2_DrawShape(d2_ScreenW - 0x96, d2_HalfScreenH, d2_ScreenSurf, pic, 4);
    else if (right == 1)
        d2_DrawShape(d2_ScreenW - 0x96, d2_HalfScreenH, d2_ScreenSurf, pic, 5);
    if (top == 2)
        d2_DrawShape(d2_HalfScreenW - 0x10, 0x38, d2_ScreenSurf, pic, 6);
    else if (top == 1)
        d2_DrawShape(d2_HalfScreenW - 0x10, 0x38, d2_ScreenSurf, pic, 7);
}
/* 0x52d7a0, per frame: ends a running camera placement (d3t_CamMove), the camera put straight onto the stored view
   (sv_ViewPos / sv_ViewRot) when the placement was heading there; outside a nested frame (or after that) a camera
   off the stored view is moved back to it (sv_MoveCameraToView) and the focus object released (its event 3); in a
   nested frame the camera returns once the frame depth reaches sv_FocusDepth again, which releases the focus
   object too; then the selection is reset when an item is selected */

void sv_CameraFollow(void)
{
    int placed;
    D3EVENT *ev;

    placed = 0;
    if (d3t_CamMove != 0) {
        if (d3_VecEqual(&d3t_PlaceTarget, (D3VECTOR *)sv_ViewPos, 0.1f) != 0) {
            d3anim_FreePath(d3_Camera);
            d3t_CamMove = 0;
            d3t_CamMoving = 0;
            placed = 1;
            d3_SetObjectPos(d3_Camera, (D3VECTOR *)sv_ViewPos);
            d3_SetObjectRot(d3_Camera, (D3VECTOR *)sv_ViewRot);
        } else {
            d3anim_FreePath(d3_Camera);
            d3t_CamMove = 0;
            d3t_CamMoving = 0;
        }
    }
    if (sv_FrameDepth <= sv_FrameBase || placed) {
        if ((d3_VecEqual(&d3_Camera->rotation, (D3VECTOR *)sv_ViewRot, 0.1f) == 0 || d3_VecEqual(&d3_Camera->pos, (D3VECTOR *)sv_ViewPos, 0.1f) == 0)
            && !placed) {
            sv_MoveCameraToView();
            if (sv_FocusObject != 0) {
                ev = ((D3OBJECT *)sv_FocusObject)->events;
                if (ev != 0 && ev[3].fn != 0)
                    ev[3].fn(sv_FocusObject, 3, ev[3].name, 0);
                sv_FocusObject = 0;
            }
        }
    } else if (sv_FocusDepth >= sv_FrameDepth - sv_PopRequest) {
        sv_MoveCameraToView();
        sv_FocusDepth = sv_FrameDepth - sv_PopRequest;
        if (sv_FocusObject != 0) {
            ev = ((D3OBJECT *)sv_FocusObject)->events;
            if (ev != 0 && ev[3].fn != 0)
                ev[3].fn(sv_FocusObject, 3, ev[3].name, 0);
            sv_FocusObject = 0;
        }
        if (sv_SelectedItem != 0)
            sv_ResetSelection();
    }
    if (sv_SelectedItem != 0)
        sv_ResetSelection();
}
typedef struct SVFRAME {                /* 8 bytes, an entry of the frame stack sv_Frames (sv_RunFrame: "MASTER()") */
    int cb;                             /* 0x00, the frame callback registered at this depth (sv_RunFrame's cb) */
    char pad04[4];
} SVFRAME;
#define sv_Frames (*(SVFRAME (*)[0x80])((char *)&ai_Unk75f280 + 0x500))  /* 0x75f780, the frame stack, indexed by sv_FrameDepth (sv_RunFrame pushes and pops it) */

/* 0x52d940, the per-frame input step of the game screen (sv_RunFrame, mask 0x100; fl is the frame mask): a right
   click (dm_L.rup) that has something to undo clicks ("MausclickRechts_s_01"); while an event window is active
   (he_ActiveSlot) only a right click or Enter acts on it: the ceremony kinds 0x11 / 0x26 / 0x69 are confirmed
   (he_SlotConfirmed), the tutorial's 0x87 ticks, the others close; a right click on an event bar button resets
   its event's stage. Mask 0x800: the map key, the animals, then the right click leaves the current view step by
   step: the selection, a room entered through a contact ("RaumWechseln_s", the contact becomes the building's
   main room), a running camera placement, a nested frame, the room (back to the main room 0xf2 or out of the
   building), and puts the camera back on the stored view. Mask 0x100: the panel update; mask 8: the border
   camera of an own (or 0x1000-permitted) building. The building permissions are recomputed when the building or
   room changed (sv_PermsBuilding / sv_PermsRoom) or ai_PermsDirty is set
   @deep Rewritten whole against the original 0x52d940..0x52e049 (every block read and reconciled: the
   event-window close, the event bar loop, the 0x800 right-click cascade, the camera / frame / room exit,
   the 0x100 / 8 tails and the permission refresh); 353 -> 3 differing instructions, ours 503 vs the
   original's 506. The one hunk left: the original tests the slot address inside the event bar loop, 'lea
   eax,[edx-4]; test eax,eax; je' at 0x52da38 (edx = &he_EventSlots[i].obj, the null test of
   &he_EventSlots[i]); our source has the test (s != 0) but VC6 folds it in every spelling tried: s =
   &he_EventSlots[i], a pointer walk s++ with an unsigned and a signed (int) end compare, (int)s != 0, char
   * / unsigned address arithmetic, &s->ev != 0, &s->obj - 1, s != he_ActiveSlot (0 there), s != the zero
   local bld, he.h's array without its size. A variant round has to find the spelling (an inline helper,
   another declaration of the table) that keeps the test. */

void sv_HandleRightClick(unsigned fl)
{
    char *bld;
    int placed, i;
    AIJOB *job;
    HEEVSLOT *s;
    char *rb;
    unsigned short *room;
    D3EVENT *ev;

    bld = 0;
    if (dm_L.rup != 0 && (sv_Frames[sv_FrameDepth].cb != (int)ai_GameFrame || ai_SelectedCount != 0 || sv_SelectedItem != 0 || sv_SelectedChar != 0 || sv_SelectedObject != 0))
        sb_PlaySample(snd_SystemBank, "MausclickRechts_s_01", 1, 0x3f, 0x7f, 0, 0);
    if (he_ActiveSlot != 0) {
        if (dm_L.rup == 0 && kb_Last != 0x1c)
            return;
        job = he_ActiveSlot->ev;
        if (job != 0 && (job->kind == 0x11 || job->kind == 0x26 || job->kind == 0x69))
            he_SlotConfirmed = 1;
        else if (job != 0 && job->kind == (char)0x87)
            ai_TutorialTick();
        else
            he_ActivateSlot(0);
        di_ClearEvents(0x10);
        if (kb_Last == 0x1c)
            kb_Last = 0;
        return;
    }
    if (dm_L.rup != 0 && d2_PointerObj != -1) {
        for (i = 0; i < 16; i++) {
            s = &he_EventSlots[i];
            if (s->obj != -1 && s != 0 && d2_PointerObj == ((D2OBJ *)d2_Objs)[s->obj].id) {
                he_EventSlots[i].ev->stage = -1;
                he_SlotConfirmed = 0;
                di_ClearButtons();
                break;
            }
        }
    }
    if ((fl & 0x800) == 0)
        goto done;
    ai_HandleMapKey();
    anm_Update();
    if (dm_L.rup != 0 && sv_SelectedObject != 0 && sv_Frames[sv_FrameDepth].cb == (int)ai_GameFrame)
        sv_ResetSelection();
    else {
        if (ai_PlayerBuilding != 0) {
            if (sv_ClickedContact != 0 && ai_SelectedCount == 0 && sv_OtherSelected == 0) {
                sb_PlaySample(snd_RoomBank, "RaumWechseln_s", 1, 0x3f, 0x7f, 0, 0);
                ((AIBLD *)ai_PlayerBuilding)->mainRoom = ((AIOBJ *)sv_ClickedContact)->id;
                sv_PopRequest = sv_FrameDepth - sv_FrameBase + 1;
                d3_SetObjectPos(d3_Camera, (D3VECTOR *)sv_ViewPos);
                d3_SetObjectRot(d3_Camera, (D3VECTOR *)sv_ViewRot);
                sv_BorderViewX = 0;
                sv_BorderViewY = 0;
            }
        } else if (ai_FallbackBuilding != 0 && sv_PanelShown != 0 && sv_ClickedContact != 0 && ai_SelectedCount == 0 && sv_OtherSelected == 0) {
            sv_PopRequest = sv_FrameDepth - sv_FrameBase + 1;
            d3_SetObjectPos(d3_Camera, (D3VECTOR *)sv_ViewPos);
            d3_SetObjectRot(d3_Camera, (D3VECTOR *)sv_ViewRot);
            sv_BorderViewX = 0;
            sv_BorderViewY = 0;
        }
        if (dm_L.rup != 0 && (sv_OtherSelected != 0 || (sv_SelectedItem != 0 && ai_GoodsT[*sv_SelectedItem].type == 0x1d))) {
            sv_ResetSelection();
            di_ClearEvents(0x10);
        }
        if ((ai_PlayerBuilding != 0 && ai_ScriptQuery(0x1a, (int)ai_PlayerBuilding, 0, *sv_PanelShown) == 0)
            || (ai_FallbackBuilding != 0 && ai_ScriptQuery(0x1c, (int)ai_FallbackBuilding, 0, 0) == 0))
            di_ClearAxes();
        if (ai_SelectedCount != 0 || sv_OtherSelected != 0 || dm_L.rup == 0)
            goto done;
        placed = 0;
        if (d3t_CamMove != 0) {
            if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0)
                goto done;
            if (d3_VecEqual(&d3t_PlaceTarget, (D3VECTOR *)sv_ViewPos, 0.1f) != 0) {
                d3anim_FreePath(d3_Camera);
                d3t_CamMove = 0;
                d3t_CamMoving = 0;
                placed = 1;
                d3_SetObjectPos(d3_Camera, (D3VECTOR *)sv_ViewPos);
                d3_SetObjectRot(d3_Camera, (D3VECTOR *)sv_ViewRot);
            } else {
                d3anim_FreePath(d3_Camera);
                d3t_CamMove = 0;
                d3t_CamMoving = 0;
            }
        }
        if (sv_FrameDepth > sv_FrameBase && !placed) {
            /* a nested frame: pop it, the camera returns once the depth is back at sv_FocusDepth */
            sv_PopRequest = 1;
            if (sv_FocusDepth < sv_FrameDepth - 1)
                goto done;
            if (ai_PlayerBuilding != 0 || ai_FallbackBuilding != 0)
                sv_MoveCameraToView();
            sv_FocusDepth = sv_FrameDepth - sv_PopRequest;
            if (sv_FocusObject != 0) {
                ev = ((D3OBJECT *)sv_FocusObject)->events;
                if (ev != 0 && ev[3].fn != 0)
                    ev[3].fn(sv_FocusObject, 3, ev[3].name, 0);
                sv_FocusObject = 0;
            }
            sv_ResetSelection();
        } else if (d3_VecEqual(&d3_Camera->rotation, (D3VECTOR *)sv_ViewRot, 0.1f) == 0 && d3_VecEqual(&d3_Camera->pos, (D3VECTOR *)sv_ViewPos, 0.1f) == 0 && !placed) {
            /* the camera is off the stored view: back to it */
            if (ai_PlayerBuilding != 0 || ai_FallbackBuilding != 0)
                sv_MoveCameraToView();
            if (sv_FocusObject != 0) {
                ev = ((D3OBJECT *)sv_FocusObject)->events;
                if (ev != 0 && ev[3].fn != 0)
                    ev[3].fn(sv_FocusObject, 3, ev[3].name, 0);
                sv_FocusObject = 0;
            }
            sv_ResetSelection();
        } else {
            /* the camera is at the stored view: leave the room, or the building */
            if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0 && sv_PanelShown == 0) {
                if (sv_FrameDepth == sv_FrameBase && sv_SelectedItem != 0)
                    sv_ResetSelection();
                goto done;
            }
            room = 0;
            if (sv_PanelShown != 0) {
                rb = 0;
                if (ai_PlayerBuilding != 0) {
                    if (ai_ScriptQuery(0x1a, (int)ai_PlayerBuilding, 0, *sv_PanelShown) == 1 && !(ai_CharsT[ai_PlayerId].heldRounds > 0))
                        rb = ai_PlayerBuilding;
                } else if (ai_FallbackBuilding != 0) {
                    if (ai_ScriptQuery(0x1c, (int)ai_FallbackBuilding, 0, 0) == 1 && !(ai_CharsT[ai_PlayerId].heldRounds > 0))
                        rb = ai_FallbackBuilding;
                }
                if (rb != 0 && *sv_PanelShown == 0xf2)
                    room = (unsigned short *)gm_GetGebaeudeHauptraum((unsigned char *)rb);
                if (ai_PlayerBuilding == 0 || room == 0 || room == sv_PanelShown)
                    sv_PanelShown = 0;
                else
                    ai_SwitchRoomI(-1, *room);
            } else if (ai_FallbackBuilding != 0 || ai_PlayerBuilding != 0) {
                ai_FallbackBuilding = 0;
                ai_PlayerBuilding = 0;
                sv_PanelShown = 0;
            }
            sv_PopRequest = 1;
            if (ai_PlayerBuilding != 0 && sv_PanelShown == 0)
                ai_PlayerBuilding = 0;
            d3_SetObjectPos(d3_Camera, (D3VECTOR *)sv_ViewPos);
            d3_SetObjectRot(d3_Camera, (D3VECTOR *)sv_ViewRot);
            sv_BorderViewX = 0;
            sv_BorderViewY = 0;
        }
    }
done:
    if ((fl & 0x100) != 0)
        sv_UpdatePanel();
    if (ai_PlayerBuilding != 0)
        bld = ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        bld = ai_FallbackBuilding;
    if ((fl & 8) != 0 && bld != 0) {
        if (sv_PanelShown != 0)
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)bld, sv_PanelShown, 0);
        else
            sv_PanelPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)bld, 0, 0);
        if (((AIBLD *)bld)->owner == ai_PlayerId || (sv_PanelPerms & 0x1000) != 0)
            sv_BorderCameraView();
    }
    if (sv_PermsBuilding != bld || sv_PermsRoom != sv_PanelShown || ai_PermsDirty != 0) {
        ai_BuildingPerms = gm_PermissionBits(ai_PlayerId, (unsigned char *)bld, sv_PanelShown, 0);
        sv_ReleaseNamePlates();
        sv_PermsBuilding = bld;
        sv_PermsRoom = sv_PanelShown;
        ai_PermsDirty = 0;
    }
}

#include <stdarg.h>
#include "cm_types.h"
#include "net.h"
extern unsigned char ai_GameState;
extern unsigned char sv_MenuOpen;
extern unsigned char d3_MipSkip;
extern char d3d_ZEnabled;

struct D3OBJECT * d3t_FindObject(char *name, char flags);                     /* 0x411900 */
int d2_TextWidth();                     /* 0x46ea10 */
int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */

/* 0x52e050, draws the hint text 0xf8aae0 at the mouse position when the hint flag (0x6f3618 +2) is set */
void sv_DrawHint(void)
{
    if (SV_B((unsigned)&d2_CaptionCopy) != 0 && SV_B(((unsigned)&ai_GameSwitches + 0x2)) != 0) {
        d2_SetFont(0x54);
        d2_DrawTextBox(SV_S((unsigned)&d2_MouseX) - 0x36, SV_S((unsigned)&d2_MouseY) + 0x34, 0xa0, 0x60, d2_ScreenSurf, (char *)(unsigned)&d2_CaptionCopy, 0xa8);
        d2_AddDirty(SV_I((unsigned)&d2_TextLeft), SV_I((unsigned)&d2_TextTop), SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
    }
}
/* 0x52e0c0, draws the status line sv_StatusText (font 0x53, 600 wide, centred, at y 505 (the 0x8000 frame mask) or
   575 scaled by d2_ScreenScale) and clears it 350 ms after sv_SetStatusText set it */

void sv_DrawStatusText(void)
{
    int x, y;

    if (sv_StatusText[0] != 0) {
        d2_SetFont(0x53);
        if ((sv_FrameMask & 0x8000) != 0) {
            x = d2_HalfScreenW - 300;
            y = (int)(d2_ScreenScale * 505.0f);
        } else {
            x = d2_HalfScreenW - 300;
            y = (int)(d2_ScreenScale * 575.0f);
        }
        d2_DrawTextBox(x, y, 600, 0x18, d2_ScreenSurf, sv_StatusText, 0x28);
        d2_AddDirty(x, y, d2_TextDrawnW, d2_TextDrawnH);
        if (d3_Time >= sv_StatusTime + 0x15e)
            sv_StatusText[0] = 0;
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
/* 0x52e1c0, registers the name plate of the object called name (sv_NamePlates, 0x20 records): an existing plate of that
   name is kept, else the first free record gets the object (d3t_FindObject), the pointer look and the text; the
   object becomes pickable (mask 1). Returns the object, 0 when it is not found or all records are used (the
   "RegisterStatusText()" message is formatted but not shown) */

int sv_RegisterNamePlate(char *name, char *text, int look)
{
    int i;
    char msg[0x100];

    for (i = 0; i < 0x20; i++) {
        if (sv_NamePlates[i].obj != 0 && strcmp(sv_NamePlates[i].name, name) == 0) {
            ((D3OBJECT *)sv_NamePlates[i].obj)->mask = 1;
            return (int)sv_NamePlates[i].obj;
        }
    }
    for (i = 0; i < 0x20; i++) {
        if (sv_NamePlates[i].obj == 0)
            break;
    }
    if (i >= 0x20) {
        sprintf(msg, "RegisterStatusText(): Too many Objects:%s", name);
        return 0;
    }
    sv_NamePlates[i].obj = (char *)d3t_FindObject(name, 2);
    if (sv_NamePlates[i].obj == 0) {
        sprintf(msg, "RegisterStatusText(): Object not found: %s", name);
        return 0;
    }
    sv_NamePlates[i].look = look;
    strncpy(sv_NamePlates[i].text, text, 0x80);
    strncpy(sv_NamePlates[i].name, name, 0x40);
    sv_NamePlates[i].text[0x7f] = 0;
    sv_NamePlates[i].name[0x3f] = 0;
    ((D3OBJECT *)sv_NamePlates[i].obj)->mask = 1;
    return (int)sv_NamePlates[i].obj;
}
/* 0x52e330, sets the status line text sv_StatusText and its time stamp sv_StatusTime (d3_Time) */

void sv_SetStatusText(char *s)
{
    strcpy(sv_StatusText, s);
    sv_StatusTime = d3_Time;
}
/* 0x52e350, creates n buttons (d2_WinCreateSlider kind 0xbf, font 0x53) in a row in the window f at the height y, the
   labels are the n string arguments; every button is as wide as the widest label + 0x20, the ids go to out[]. The
   buttons are spread evenly over the window's width, or, when they do not fit, put side by side 8 apart and centred
   @deep Read whole against the original 0x52e350..0x52e4af and rewritten: D2WIN *win = &d2_Wins[f] (font
   +0x27c, w +8), the va_arg label loop with the inline strcpy, two copies of the widest width (wmax in a
   register for the step / centring, bw in a slot for d2_SetObjWidth, as the original keeps it), the spread
   / side-by-side branch with its if/else difference and the shared halving, the slider x as x - wmax / 2
   (VC6's induction variable in the preheader), D2OBJ.font = 0x53. Same size (352 = 352), 122 = 122
   instructions; diffclass: slots 26, regs 34, order 6 and one 'code' hunk at the prologue (0x52e35f) that
   is the same rotation: with --regs edi=ebx,ebx=edi it is the push order (push ebx/ebp/esi/edi vs ours
   edi/ebp/esi/ebx), the zero test of n (the original 'test eax,eax', ours 'cmp eax,ebx' against the zero
   wmax register) and the slot swap win [esp+0x18] / count [esp+0x14] / bw [esp+0x10] (ours win 0x10, count
   0x18, bw 0x14). Declaration orders tried (bw first, win last, i with bw): no change. */

void sv_ButtonRow(int f, int y, int *out, int n, ...)
{
    char labels[0x20][0x40];
    D2WIN *win;
    int wmax, bw, w, step, x, i, id, row;
    char *s;
    va_list ap;

    wmax = 0;
    bw = 0;
    win = &d2_Wins[f];
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        s = va_arg(ap, char *);
        strcpy(labels[i], s);
        w = d2_TextWidth(s, win->font) + 0x20;
        if (w > wmax) {
            bw = w;
            wmax = w;
        }
    }
    va_end(ap);
    w = win->w;
    x = w / (n + 1);
    if (wmax > x) {
        step = wmax + 8;
        row = step * n;
        if (row > w)
            x = (row - w) / 2 + step / 2;
        else
            x = (w - row) / 2 + step / 2;
    } else
        step = x;
    for (i = 0; i < n; i++) {
        id = d2_WinCreateSlider(x - wmax / 2, y, 0xbf, labels[i], f);
        out[i] = id;
        ((D2OBJ *)d2_Objs)[id].font = 0x53;
        d2_SetObjWidth(id, bw);
        x += step;
    }
}
/* 0x52e4b0, sv_ButtonRow with the font 0x54 and the button kind 0xc5 (rt_EditCartRoute's tab row): n buttons in a
   row in the window f at the height y, labelled with the n string arguments, as wide as the widest label + 0x20,
   spread evenly or put side by side 8 apart and centred; the ids go to out[]
   @deep Read whole against the original 0x52e4b0..0x52e5fc and rewritten as sv_ButtonRow's twin: D2WIN
   *win (w +8), the va_arg label loop with the inline strcpy, d2_TextWidth(s, 0x54) + 0x20, one widest
   width wmax (a slot, as the original), the spread / side-by-side branch with the if/else difference, the
   slider x as x - wmax / 2, D2OBJ.font = 0x54, kind 0xc5. Same size (333 = 333); the whole diff is the
   ebx/edi rotation: with --regs ebx=edi,edi=ebx only the prologue's push order / the load of n (ours mov
   edi,[esp+0x81c] before push ebp, the original after push edi), the reload of n after the label loop
   scheduled one line later, the 'add ebx,ebp' line order and the pop order remain. */

void sv_TabButtonRow(int f, int y, int *out, int n, ...)
{
    char labels[0x20][0x40];
    D2WIN *win;
    int wmax, w, step, x, i, id, row;
    char *s;
    va_list ap;

    wmax = 0;
    win = &d2_Wins[f];
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        s = va_arg(ap, char *);
        strcpy(labels[i], s);
        w = d2_TextWidth(s, 0x54) + 0x20;
        if (w > wmax)
            wmax = w;
    }
    va_end(ap);
    w = win->w;
    x = w / (n + 1);
    if (wmax > x) {
        step = wmax + 8;
        row = step * n;
        if (row > w)
            x = (row - w) / 2 + step / 2;
        else
            x = (w - row) / 2 + step / 2;
    } else
        step = x;
    for (i = 0; i < n; i++) {
        id = d2_WinCreateSlider(x - wmax / 2, y, 0xc5, labels[i], f);
        out[i] = id;
        ((D2OBJ *)d2_Objs)[id].font = 0x54;
        d2_SetObjWidth(id, wmax);
        x += step;
    }
}

int ai_LedSelectBits(int idx, char *c);                     /* 0x4ce170, cm.h (not included: its d2_Objs / d2_LastWindowPtr clash) */

/* 0x52e600, on != 0: disables (D2OBJ.disabled) every used d2 object that draws into an active, enabled view (d2_Views)
   and is not disabled yet, remembering it in sv_DisabledWidgets; on == 0: enables the remembered objects again */
void sv_DisableVisibleWidgets(int on)
{
    D2VIEW *v;
    D2OBJ *o;
    int i, k;

    if (on != 0) {
        sv_DisabledCount = 0;
        for (k = 0; k < 0x30; k++) {
            v = &d2_Views[k];
            if (v->active != 0 && v->enabled != 0) {
                for (i = 0; i < 0x300; i++) {
                    o = &((D2OBJ *)d2_Objs)[i];
                    if (o->type != 0 && o->buf == (D2CTRL *)v && o->disabled == 0) {
                        sv_DisabledWidgets[sv_DisabledCount++] = i;
                        o->disabled = 1;
                    }
                }
            }
        }
        return;
    }
    for (i = 0; i < sv_DisabledCount; i++)
        ((D2OBJ *)d2_Objs)[sv_DisabledWidgets[i]].disabled = 0;
}
/* 0x52e6d0, puts the frame pictures into the building window f (ai_ProductionWindow, ai_TradeSearchWindow, ..): the
   title c at (0xb, 9) (0: the current building's kind picture 0x427 + type, 0x520 without a building), the kind
   picture at the right (0x444 without a building; the wide frame b = 0x73b puts the plain title 0x520 and the kind
   picture at 0x1eb instead), then the frame: b, b + 1, b + 2, b + 3 (left, bottom, top, right) or the wide frame's
   0x73b / 0x73d / 0x73e / 0x73c; the last window's paddings become 0x18 */

void sv_OpenWindowFrame(int f, int b, int c)
{
    char *bld;

    d2_WinOpenPic(0x257, 9, 0x520, f);
    if (c != 0)
        d2_WinOpenPic(0xb, 9, c, f);
    else if ((bld = ai_PlayerBuilding) == 0 && (bld = ai_FallbackBuilding) == 0)
        d2_WinOpenPic(0xb, 9, 0x520, f);
    else
        d2_WinOpenPic(0xb, 9, ((AIBLD *)bld)->type + 0x427, f);
    if ((bld = ai_PlayerBuilding) == 0 && (bld = ai_FallbackBuilding) == 0)
        d2_WinOpenPic(0x257, 9, 0x444, f);
    else if (b != 0x73b)
        d2_WinOpenPic(0x257, 9, ((AIBLD *)bld)->type + 0x427, f);
    else {
        d2_WinOpenPic(0x1eb, 9, 0x520, f);
        d2_WinOpenPic(0x1eb, 9, ((AIBLD *)bld)->type + 0x427, f);
    }
    if (b != 0x73b) {
        d2_WinOpenPic(0, 0, b, f);
        d2_WinOpenPic(0, 400, b + 1, f);
        d2_WinOpenPic(0, 0x4d, b + 2, f);
        d2_WinOpenPic(0x247, 0x4d, b + 3, f);
    } else {
        d2_WinOpenPic(0, 0, 0x73b, f);
        d2_WinOpenPic(0, 0x43, 0x73d, f);
        d2_WinOpenPic(0x1f7, 0x43, 0x73e, f);
        d2_WinOpenPic(0, 0x1a8, 0x73c, f);
    }
    d2_Wins[d2_LastWindow].padl = 0x18;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0x18;
    d2_Wins[d2_LastWindow].padb = 0x18;
}
/* 0x52e860, gives the window g scroll controls in the window f: the up button (picture 3) at (x, y) and the down button
   (picture 4) at (x, y + 0x2a), both style 3, interactive, owned by g, and the empty text between them (font 0x54,
   centred, 0x30 wide) at (x - 0x13, y + 0x19); their ids go to *up / *down / *bar and to f's btnup / btndown / bar;
   g's scroll position is reset and step becomes its scroll step (visible). Returns 1 */

int sv_CreateScrollButtons(int x, int y, int f, int g, int step, int *down, int *up, int *bar)
{
    D2WIN *w;
    int id;

    w = &d2_Wins[g];
    id = d2_WinOpenPic(x, y, 3, f);
    *up = id;
    d2_Wins[f].btnup = id;
    ((D2OBJ *)d2_Objs)[*up].interactive = 1;
    ((D2OBJ *)d2_Objs)[*up].style = 3;
    id = d2_WinOpenPic(x, y + 0x2a, 4, f);
    *down = id;
    d2_Wins[f].btndown = id;
    ((D2OBJ *)d2_Objs)[*down].interactive = 1;
    ((D2OBJ *)d2_Objs)[*down].style = 3;
    ((D2OBJ *)d2_Objs)[*down].winIndex = g;
    ((D2OBJ *)d2_Objs)[*up].winIndex = g;
    id = d2_CreateWinText(x - 0x13, y + 0x19, ch_EmptyName, f);
    *bar = id;
    d2_Wins[f].bar = id;
    ((D2OBJ *)d2_Objs)[*bar].font = 0x54;
    ((D2OBJ *)d2_Objs)[*bar].centred = 1;
    ((D2OBJ *)d2_Objs)[*bar].w = 0x30;
    w->scrollY = 0;
    w->visible = step;
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
    int n, odd, w, i;

    w = d2_Pics[k].w + 2;
    n = (int)(value / 21.0f);
    odd = n % 2;
    for (i = 0; i < n / 2; i++)
        d2_WinOpenPic(i * w + x, y, k, d2_LastWindow);
    if (odd != 0)
        d2_WinOpenPic(i * w + x, y, k + 1, d2_LastWindow);
}
/* 0x52eb50, draws the skill skill of the character c as a row of tiles at (x, y) in d2_LastWindow, 21 points a
   tile (sv_ValueBar's tiles): the current value gm_SkillPoints against the base byte c[0x80 + skill]. Within 1 %
   of each other one bar in the picture k (k + 1 the half tile); a current value below the base draws the base in
   k + 2 / k + 3 and the current value over it in k / k + 1; above the base the current value in k + 4 / k + 5 and
   the base over it in k, its half tile in k too (the original pushes k there). */

void sv_DrawBalanceBar(int x, int y, char *c, int skill, int k)
{
    int cur, base, top, n, odd, w, i;

    w = d2_Pics[k].w + 2;
    cur = gm_SkillPoints(c, skill);
    base = (unsigned char)c[0x80 + skill];
    top = cur;
    if (cur <= base)
        top = base;
    if (fabs((double)(cur - base)) / top > 0.01f) {
        if (cur < base) {
            n = (int)(base / 21.0f);
            odd = n % 2;
            for (i = 0; i < n / 2; i++)
                d2_WinOpenPic(i * w + x, y, k + 2, d2_LastWindow);
            if (odd != 0)
                d2_WinOpenPic(i * w + x, y, k + 3, d2_LastWindow);
            n = (int)(cur / 21.0f);
            odd = n % 2;
            for (i = 0; i < n / 2; i++)
                d2_WinOpenPic(i * w + x, y, k, d2_LastWindow);
            if (odd != 0)
                d2_WinOpenPic(i * w + x, y, k + 1, d2_LastWindow);
        } else if (cur > base) {
            n = (int)(cur / 21.0f);
            odd = n % 2;
            for (i = 0; i < n / 2; i++)
                d2_WinOpenPic(i * w + x, y, k + 4, d2_LastWindow);
            if (odd != 0)
                d2_WinOpenPic(i * w + x, y, k + 5, d2_LastWindow);
            n = (int)(base / 21.0f);
            odd = n % 2;
            for (i = 0; i < n / 2; i++)
                d2_WinOpenPic(i * w + x, y, k, d2_LastWindow);
            if (odd != 0)
                d2_WinOpenPic(i * w + x, y, k, d2_LastWindow);    /* the original's: k, not k + 1 */
        }
    } else {
        n = (int)(base / 21.0f);
        odd = n % 2;
        for (i = 0; i < n / 2; i++)
            d2_WinOpenPic(i * w + x, y, k, d2_LastWindow);
        if (odd != 0)
            d2_WinOpenPic(i * w + x, y, k + 1, d2_LastWindow);
    }
}
/* 0x52ee50, starts the rubber band selection (sv_BandActive = 1): both corners of sv_Band are the mouse position
   dm_L clamped to the view rectangle d3_Viewport (right / bottom exclusive). */

void sv_StartBand(void)
{
    int x, y;

    x = min(max(d3_Viewport.left, dm_L.x), d3_Viewport.right - 1);
    y = min(max(d3_Viewport.top, dm_L.y), d3_Viewport.bottom - 1);
    sv_Band.x0 = x;
    sv_Band.x1 = x;
    sv_BandActive = 1;
    sv_Band.y0 = y;
    sv_Band.y1 = y;
}
/* 0x52eed0, ends the rubber band selection */

void sv_EndBand(void)
{
    SV_I((unsigned)&sv_BandActive) = 0;
}
/* 0x52eee0, during the rubber band: moves sv_Band's mouse corner (clamped to d3_Viewport like sv_StartBand) and
   selects the characters of ai_SceneChars the band holds: a live character the player may select
   (gm_PermissionBits 0x800) whose body stands in d3_DefaultScene and has mesh data; its screen position is the
   centre of the body's first 8 transformed vertices (the bounding box corners, camera space) projected with
   d3_ScaleX / d3_ScaleY around d3_CenterX / d3_CenterY (the 1 / z truncated to an int, as the original does).
   @deep read whole against the original and reconstructed (band clamp via min/max, the per-character
   permission / scene / info tests, the 8-vertex centre in a padded D3VECTOR4 (frame matches 0x20), the 1/z
   projection); all hunks equal but the allocation of info vs. the loop counter: ours 'mov eax,
   [eax+0x1cc]; test eax, eax ... mov ecx, 8', the original's 'mov ecx, [eax+0x1cc]; test ecx, ecx ... mov
   eax, ecx; mov ecx, 8' (one extra move). Tried: info local, v set before the inits, an indexed loop
   (worse). */

void sv_UpdateBand(void)
{
    int left, right, top, bottom, i, n, iz, sx, sy;
    unsigned bits;
    D3VECTOR4 p;
    AICHAR *c;
    D3PVERT *v;
    D3OBJINFO *info;

    if (sv_BandActive == 0)
        return;
    sv_Band.x1 = min(max(d3_Viewport.left, dm_L.x), d3_Viewport.right - 1);
    sv_Band.y1 = min(max(d3_Viewport.top, dm_L.y), d3_Viewport.bottom - 1);
    left = min(sv_Band.x0, sv_Band.x1);
    top = min(sv_Band.y0, sv_Band.y1);
    right = max(sv_Band.x0, sv_Band.x1);
    bottom = max(sv_Band.y0, sv_Band.y1);
    for (i = 0; i < 0x20; i++) {
        c = (AICHAR *)ai_SceneChars[i];
        if (c == 0 || c->health == 0)
            continue;
        bits = gm_PermissionBits(ai_PlayerId, 0, 0, (int)c);
        c->selected = 0;
        if ((bits & 0x800) == 0 || c->model == 0 || c->model->scene != d3_DefaultScene || (info = c->model->body->info) == 0)
            continue;
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
        v = (D3PVERT *)info->keys;
        for (n = 8; n != 0; n--, v++) {
            p.x += v->x;
            p.y += v->y;
            p.z += v->z;
        }
        p.x *= 0.125;
        p.y *= 0.125;
        p.z *= 0.125;
        iz = (int)(1.0 / p.z);
        sx = (int)(d3_ScaleX * iz * p.x + d3_CenterX);
        sy = (int)(d3_ScaleY * iz * p.y + d3_CenterY);
        if (sx >= left && sx <= right && sy >= top && sy <= bottom)
            c->selected = 1;
    }
}
/* 0x52f0d0, sv_UpdateBand for the 0x8000 mode of sv_RunFrame: the same band and projection, but a character is
   selectable when ai_LedSelectBits gives 0x800 (led by the player's character) instead of gm_PermissionBits.
   @deep read whole against the original: it is sv_UpdateBand with ai_LedSelectBits(ai_PlayerId, c) in
   place of gm_PermissionBits; reconstructed the same way (min/max clamp, D3VECTOR4 centre, frame matches).
   The one remaining hunk is the same allocation as sv_UpdateBand's: ours 'mov eax, [eax+0x1cc]; test eax,
   eax ... mov ecx, 8', the original's 'mov ecx, [eax+0x1cc]; test ecx, ecx ... mov eax, ecx; mov ecx, 8'. */

void sv_UpdateBandLed(void)
{
    int left, right, top, bottom, i, n, iz, sx, sy;
    unsigned bits;
    D3VECTOR4 p;
    AICHAR *c;
    D3PVERT *v;
    D3OBJINFO *info;

    if (sv_BandActive == 0)
        return;
    sv_Band.x1 = min(max(d3_Viewport.left, dm_L.x), d3_Viewport.right - 1);
    sv_Band.y1 = min(max(d3_Viewport.top, dm_L.y), d3_Viewport.bottom - 1);
    left = min(sv_Band.x0, sv_Band.x1);
    top = min(sv_Band.y0, sv_Band.y1);
    right = max(sv_Band.x0, sv_Band.x1);
    bottom = max(sv_Band.y0, sv_Band.y1);
    for (i = 0; i < 0x20; i++) {
        c = (AICHAR *)ai_SceneChars[i];
        if (c == 0 || c->health == 0)
            continue;
        bits = ai_LedSelectBits(ai_PlayerId, (char *)c);
        c->selected = 0;
        if ((bits & 0x800) == 0 || c->model == 0 || c->model->scene != d3_DefaultScene || (info = c->model->body->info) == 0)
            continue;
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
        v = (D3PVERT *)info->keys;
        for (n = 8; n != 0; n--, v++) {
            p.x += v->x;
            p.y += v->y;
            p.z += v->z;
        }
        p.x *= 0.125;
        p.y *= 0.125;
        p.z *= 0.125;
        iz = (int)(1.0 / p.z);
        sx = (int)(d3_ScaleX * iz * p.x + d3_CenterX);
        sy = (int)(d3_ScaleY * iz * p.y + d3_CenterY);
        if (sx >= left && sx <= right && sy >= top && sy <= bottom)
            c->selected = 1;
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
#define SV_ANIMCTL(o) ((D3ANIMCTL *)(o)->material->animCtl)  /* the animation control of an object's material (D3MATERIAL.animCtl at 0xf4) */

/* 0x52f2c0, draws the rubber band rectangle (0x41e8f0, colour -1) between the corners 0x760444 / 0x760448 and 0x76044c / 0x760450 while 0x760440 is set */
void sv_DrawRubberBand(void)
{
    if (SV_I((unsigned)&sv_BandActive) != 0) {
        dd_Line(sv_Band.x0, sv_Band.y0, sv_Band.x1, sv_Band.y0, -1);
        dd_Line(sv_Band.x1, sv_Band.y0, sv_Band.x1, sv_Band.y1 + 1, -1);
        dd_Line(sv_Band.x1, sv_Band.y1, sv_Band.x0, sv_Band.y1, -1);
        dd_Line(sv_Band.x0, sv_Band.y1, sv_Band.x0, sv_Band.y0, -1);
    }
}
/* 0x52f340, the page keys of the book: the button 0x767 turns forward (sv_BookForward), 0x768 backward (sv_BookBack) */

void sv_BookKeys(SVBOOK *bk)
{
    if (SV_I((unsigned)&d2_ClickedId) == 0x767) {
        sv_BookForward(bk);
        return;
    }
    if (SV_I((unsigned)&d2_ClickedId) == 0x768)
        sv_BookBack(bk);
}
/* 0x52f360, opens a book: the model named model (d3t_FindObject), or, given the scene object name at, a copy of it
   created there (the model hidden until the close); n page views follow as variable arguments (the d2_LoadView
   names), each with the textures "Buch_Seite<i>_oben_ns_nm" / "Buch_Seite<i>_unten_ns_nm", the pages past the
   first pair disabled; the w-buffer is switched off while the book is open; plays "Aufklappen_s". Returns the
   record, 0 when the model or a view is missing. */

SVBOOK *sv_OpenBook(char *model, char *at, char n, ...)
{
    SVBOOK *bk;
    int i, count;
    char top[0x60], bottom[0x60];
    va_list ap;

    bk = (SVBOOK *)mem_Alloc(sizeof(SVBOOK), "f3_sv:OpenBook:b");
    if (bk == 0)
        return 0;
    bk->obj = d3t_FindObject(model, 1);
    if (bk->obj == 0) {
        mem_Free(bk);
        return 0;
    }
    if (at != 0) {
        d3if_Show(bk->obj, 0);
        bk->hidden = bk->obj;
        bk->obj = d3_CreateObject((int)bk->obj->parent, &bk->obj->pos, &bk->obj->rotation, at);
    } else {
        bk->hidden = 0;
    }
    bk->obj->state &= ~0x20;
    d3_RelightObject(bk->obj);
    bk->npages = n;
    bk->page = 0;
    bk->btnNext = -1;
    bk->btnPrev = -1;
    count = n;
    va_start(ap, n);
    for (i = 0; i < count; i++) {
        bk->views[i] = d2_LoadView(0, 0, va_arg(ap, char *));
        if (bk->views[i] == 0) {
            mem_Free(bk);
            return 0;
        }
        sprintf(top, "Buch_Seite%i_oben_ns_nm", i + 1);
        sprintf(bottom, "Buch_Seite%i_unten_ns_nm", i + 1);
        d2_FillViewTextures(bk->views[i], top, bottom);
        d2_RedrawView(bk->views[i]);
        if (i > 2)
            d2_EnableViewObjects(bk->views[i], 0);
    }
    if (d3d_ZEnabled != 0) {
        bk->zbuffer = d3d_ZEnabled;
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
/* 0x52f600, the per frame update of an open book: disables the page views outside the shown pair (page, page + 1),
   turns the book on a click (dm_L.lup) on its backward / forward button, redraws one page of the shown pair (the
   one sv_BookPhase picks; the phase flips every frame). */

void sv_UpdateBook(SVBOOK *bk)
{
    int i;

    if (bk == 0)
        return;
    for (i = 0; i < bk->npages; i++) {
        if (i < bk->page || i > bk->page + 1)
            d2_EnableViewObjects(bk->views[i], 0);
    }
    if (bk->btnPrev != -1 && d2_PointerObj == bk->btnPrev && dm_L.lup != 0)
        sv_BookBack(bk);
    if (bk->btnNext != -1 && d2_PointerObj == bk->btnNext && dm_L.lup != 0)
        sv_BookForward(bk);
    for (i = bk->page + sv_BookPhase; i < bk->npages; i++) {
        if (i > bk->page + sv_BookPhase)
            break;
        d2_RedrawView(bk->views[i]);
    }
    sv_BookPhase ^= 1;
}
/* 0x52f710, turns the book bk by dir pages (2 forward / -2 backward): within the pages, and only when the page
   animations of the book model (the three slots of its material's D3ANIMCTL) have finished (a finished turn
   animation is released); sets the page textures of the views around the new pair ("Buch_Seite<n>_oben_ns_nm" /
   "_unten_ns_nm", n = page % 4 + 1, the others emptied and disabled), plays the turn animation, moves the page,
   runs sv_BookKeys and plays "Umblaettern_s". Returns 1 when turned. */

int sv_TurnBook(SVBOOK *bk, char dir)
{
    char top[0x40], bottom[0x40], path[0x200];
    char *name;
    int i, pg, a;
    D3ANIMSLOT *slot;

    if (bk == 0)
        return 0;
    if (dir >= 0) {
        if (bk->page + 2 >= bk->npages)
            return 0;
    } else if (bk->page - 2 < 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        strcpy(path, "buch\\Buch_vorblaettern.baf");
        name = util_BaseName(path);
        slot = &SV_ANIMCTL(bk->obj)->slots[i];
        if (slot->data != 0 && _strcmpi(slot->data->name, name) == 0 && dir > 0) {
            if ((slot->flags.b[1] & 0x20) == 0)
                return 0;
            d3_ReleaseTexture(SV_ANIMCTL(bk->obj), SV_ANIMCTL(bk->obj)->slots[i].data->name);
        }
        strcpy(path, "buch\\Buch_zurueckblaettern.baf");
        name = util_BaseName(path);
        if (slot->data != 0 && _strcmpi(slot->data->name, name) == 0) {
            if ((slot->flags.b[1] & 0x20) == 0 && dir > 0)
                return 0;
            d3_ReleaseTexture(SV_ANIMCTL(bk->obj), SV_ANIMCTL(bk->obj)->slots[i].data->name);
        }
    }
    for (i = 0; i < 3; i++) {
        slot = &SV_ANIMCTL(bk->obj)->slots[i];
        if (slot->data != 0 && (slot->flags.b[1] & 0x20) == 0)
            return 0;
    }
    for (pg = 0; pg < bk->npages; pg++) {
        a = bk->page;
        if (pg >= (dir > 0 ? a : a - 2) && pg <= (dir > 0 ? a + 4 : a + 2)) {
            sprintf(top, "Buch_Seite%i_oben_ns_nm", (pg & 3) + 1);
            sprintf(bottom, "Buch_Seite%i_unten_ns_nm", (pg & 3) + 1);
            d2_SetViewTextures(bk->views[pg], top, bottom);
            d2_EnableViewObjects(bk->views[pg], 1);
        } else {
            d2_SetViewTextures(bk->views[pg], ch_EmptyName, ch_EmptyName);
            d2_EnableViewObjects(bk->views[pg], 0);
        }
    }
    if (dir > 0)
        d3t_PlayAnim(bk->obj, "buch\\Buch_vorblaettern.baf", 0x12);
    else
        d3t_PlayAnim(bk->obj, "buch\\Buch_zurueckblaettern.baf", 0x12);
    bk->page += dir;
    sv_BookKeys(bk);
    sb_PlaySample(snd_BookBank, "Umblaettern_s", 1, 0x3f, 0x7f, 0, 0);
    return 1;
}
/* 0x52f9f0 / 0x52fa00, turn the book forward / backward by two pages */

void sv_BookForward(SVBOOK *bk)
{
    sv_TurnBook(bk, 2);
}
/* 0x52fa00, turns the book backward by two pages */

void sv_BookBack(SVBOOK *bk)
{
    sv_TurnBook(bk, -2);
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

void sv_ScrollFormAnimate(void)
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
/* 0x52fb50, requests popping the frame stack down to the frame whose callback is cb: sets sv_PopRequest to its distance from the top sv_FrameDepth (nothing when cb is not on the stack) */

void sv_PopTo(int cb)
{
    int i;

    for (i = sv_FrameDepth; i >= 0; i--) {
        if (sv_Frames[i].cb == cb) {
            sv_PopRequest = sv_FrameDepth - i;
            return;
        }
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

extern unsigned sv_ChatTime;  /* 0x623ec4 */
extern int sv_QuickChatForm;  /* 0x623ec8 */
extern int cm_DialogFrameFlags;  /* 0x60d360 */
int cm_SendExSysMessage(unsigned char id, void *msg);                     /* 0x4c4db0 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.c: int view): @original the callers pass the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */
void d2_SetObjValue();                     /* 0x475cb0 */
void d2_SetObjFont();                     /* 0x4760b0 */
int io_PrintText(char *fmt, ...);                     /* 0x515960 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetObjMax();                     /* 0x475f40 */
int d2_GetObjValue();                     /* 0x475980 */
void d2_FocusInput();                     /* 0x478320 */
void cm_SendChat(void *p);                     /* 0x4c5aa0 */
void sv_ChatConsole(void);

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
    fl |= 0x300000;
    dlg = sv_OpenMessageBox(txt_NetworkSyncWait, 0);
    d2_SetViewEnabled(dlg, 0);
    while (cm_GetSlotState(m) == 0) {
        if (timeGetTime() > t0 + 0x5dc)
            d2_SetViewEnabled(dlg, 1);
        sv_RunFrame(0, fl);
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
    fl |= 0x300000;
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, fl);
    }
    sv_SetStatusText(ch_EmptyName);
}
/* 0x52fd80, like 0x52fcf0 with the key mask 0x60d360 | 0x210000 and no clearing of the status line */

void sv_SyncText(char *t)
{
    char msg[0x7c];
    int m;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, t);
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, cm_DialogFrameFlags | 0x210000);
    }
}
/* 0x52fe00, the three network shutdown steps */

void sv_PumpNet(void)
{
    cm_FlushSendQueue();
    cm_ReceiveMessages();
    cm_ExecCommands();
}
typedef struct SVCHATMSG {              /* 0x80 bytes, the data of message 0x4b (cm_SendChat; cm.c's handler appends text to cm_ChatLog) */
    int to[8];                          /* +0x00, the netIds of the checked recipients (AICHAR.netId) */
    char text[0x60];                    /* +0x20, "$<colour>FF " + the typed line + "$A" */
} SVCHATMSG;

/* 0x52fe10, the chat console form "misc\chatconsole", also its own sv_RunFrame callback: one recipient button per human player (AICHAR.kind 7, the portrait ref54,
   the state kept in sv_ChatRecipients), the chat log cm_ChatLog and an input line; Enter or the send button sends the line coloured by the player's ref54
   to the checked players (cm_SendChat) and closes the form, the right button or Escape closes it */
void sv_ChatConsole(void)
{
    int ids[8];
    SVCHATMSG msg;
    int i, k, f, ed, w;

    ids[0] = -1;
    ids[1] = -1;
    ids[2] = -1;
    ids[3] = -1;
    ids[4] = -1;
    ids[5] = -1;
    ids[6] = -1;
    ids[7] = -1;
    sv_ChatTime = 0;
    f = d2_OpenForm(0, 0, "misc\\chatconsole");
    d2_PlaceMainWindowF(f, 3);
    d2_SetForm(f, 3);
    i = 0;
    for (k = 0; k < 768; k++) {
        if (ai_CharsT[k].kind == 7) {
            w = d2_WinOpenPic(i * 0x30 + 100, 4, ai_CharsT[k].ref54, d2_LastWindow);
            ids[i] = w;
            ((D2OBJ *)d2_Objs)[w].checkable = 1;
            ((D2OBJ *)d2_Objs)[w].user = &ai_CharsT[k];
            d2_SetObjValue(w, sv_ChatRecipients[i]);
            i++;
        }
    }
    w = d2_WinCreateSlider(0, 0, 0xc5, txt_ChatConsoleSlider, d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    d2_SetForm(f, 2);
    ed = d2_GetWinObj(f, 2, io_PrintText(0x1cc3, ch_EmptyName));
    d2_SetObjMax(ed, 0x60);
    while (sv_RunFrame((int)sv_ChatConsole, 0x657c7) != 0) {
        d2_SetForm(f, 1);
        io_PrintText("$C");
        io_PrintText(cm_ChatLog);
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        d2_FocusInput(ed);
        if ((d2_ClickedId != -1 || kb_Last == 0x1c) && (d2_PointerObj == w || kb_Last == 0x1c)) {
            for (i = 0; i < 8; i++) {
                if (ids[i] != -1 && d2_GetObjValue(ids[i]) != 0)
                    msg.to[i] = ((AICHAR *)((D2OBJ *)d2_Objs)[ids[i]].user)->netId;
            }
            sprintf(msg.text, "$%iFF ", ai_CharsT[ai_PlayerId].ref54 - 0x59a);
            strcat(msg.text, (char *)d2_GetObjValue(ed));
            strcat(msg.text, "$A");
            d2_SetObjValue(ed, ch_EmptyName);
            cm_SendChat(&msg);
            sv_PopRequest = 1;
        }
    }
    for (i = 0; i < 8; i++) {
        if (ids[i] != -1)
            sv_ChatRecipients[i] = d2_GetObjValue(ids[i]);
    }
    d2_CloseView(f);
    kb_Last = 0;
}
/* 0x530100, shows / hides the quick chat strip "misc\quickchat" (sv_QuickChatForm) with the chat log: visible while a chat line is fresh (sv_ChatTime + 0x23a ms)
   and the chat console sv_ChatConsole is not the top frame */

void sv_UpdateQuickChat(void)
{
    if (sv_Frames[sv_FrameDepth].cb == (int)sv_ChatConsole || sv_ChatTime <= d3_Time - 0x23a || sv_ChatTime == 0xffffffff) {
        if (sv_QuickChatForm != -1) {
            d2_CloseView(sv_QuickChatForm);
            sv_QuickChatForm = -1;
        }
    } else {
        if (sv_QuickChatForm == -1) {
            sv_QuickChatForm = d2_OpenForm(0, 0, "misc\\quickchat");
            d2_PlaceMainWindowF(sv_QuickChatForm, 3);
            if (sv_QuickChatForm == -1)
                return;
        }
        d2_SetForm(sv_QuickChatForm, 0);
        io_PrintText("$C");
        io_PrintText(cm_ChatLog);
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
int ai_RunAllows();                     /* 0x5763c0 */
void ai_CampaignStep(int a, int b, int c, int d);                     /* 0x576040 */
extern int sv_SunRaysHour;  /* 0x623ecc */

#define sv_SkyLayersT (*(D3SKYLAYER *(*)[6])&d3_SkyLayers)  /* 0x760620, d3_SkyLayers (int[6]) as the sky layers they point to */

/* 0x5301a0, applies the weather of the clock's hour (sv_WeatherHour) from sv_Weather: the snow / rain emitters get the hour's clouds (rain under snow only on odd
   clouds, a fifth), the strongest clouds c of the hour and its two neighbours drive the emitters' drift against the wind and, with the wind strength, the sky:
   a new texture for the fading cloud layer (sv_SkyFair below 50, sv_SkyMedium below 150, sv_SkyHeavy above; never the one it shows), the layer speeds and
   the alpha of the overcast layer 2 */
void sv_UpdateWeather(void)
{
    float wx, wy, w;
    int c, k;
    SVSKYREC *sky;

    sv_WeatherHour = ai_Time.hour;
    if (d3t_Snow != 0)
        d3t_SetSnow((struct D3TSNOW *)d3t_Snow, 1, 1, sv_Weather.clouds[sv_WeatherHour], 1000);
    if (d3t_Rain != 0) {
        if (d3t_Snow != 0) {
            if (sv_Weather.clouds[sv_WeatherHour] & 1)
                d3t_SetRain((struct D3TRAIN *)d3t_Rain, 1, 1, (int)sv_Weather.clouds[sv_WeatherHour] / 5, 1000);
            else
                d3t_SetRain((struct D3TRAIN *)d3t_Rain, 1, 1, 0, 1000);
        } else
            d3t_SetRain((struct D3TRAIN *)d3t_Rain, 1, 1, sv_Weather.clouds[sv_WeatherHour], 1000);
    }
    wx = sv_Weather.windX[sv_WeatherHour];
    wy = sv_Weather.windY[sv_WeatherHour];
    c = sv_Weather.clouds[(sv_WeatherHour + 23) % 24];
    if (c <= (int)sv_Weather.clouds[sv_WeatherHour])
        c = sv_Weather.clouds[sv_WeatherHour];
    if (c <= (int)sv_Weather.clouds[(sv_WeatherHour + 1) % 24])
        c = sv_Weather.clouds[(sv_WeatherHour + 1) % 24];
    if (d3t_Snow != 0)
        d3t_SetSnow((struct D3TSNOW *)d3t_Snow, 1, 2, (int)(-wx * (double)c * 2.0), (int)(-wy * (double)c * 2.0), 1000);
    if (d3t_Rain != 0)
        d3t_SetRain((struct D3TRAIN *)d3t_Rain, 1, 2, (int)(-wx * (double)c * 0.5), (int)(-wy * (double)c * 0.5), 1000);
    if (d3_Sky != 0) {
        w = (float)(sqrt(wy * wy + wx * wx) * (c + 0x96) * 0.125);
        if (sv_SkyLayersT[1]->alpha == 0) {
            if (c < 0x32) {
                k = util_RandomU(4);
                if (_strcmpi(sv_SkyLayersT[1]->t->name, sv_SkyFair[k].name) == 0)
                    k++;
                sky = &sv_SkyFair[k];
            } else if (c < 0x96) {
                k = util_RandomU(3);
                if (_strcmpi(sv_SkyLayersT[1]->t->name, sv_SkyMedium[k].name) == 0)
                    k++;
                sky = &sv_SkyMedium[k];
            } else {
                k = util_RandomU(2);
                if (_strcmpi(sv_SkyLayersT[1]->t->name, sv_SkyHeavy[k].name) == 0)
                    k++;
                sky = &sv_SkyHeavy[k];
            }
            d3sky_SetLayerTex(sv_SkyLayersT[1], sky->name);
            sv_SkyLayersT[1]->scale = sky->scale;
            sv_SkyLayersT[1]->vscale = sky->vscale;
            sv_SkyLayersT[1]->voffset = sky->voffset;
            d3sky_FadeLayer(d3_Sky, sv_SkyLayersT[1], 0xff, 1500.0f);
        }
        if (sv_SkyLayersT[1]->alpha == 0xff) {
            if (c < 0x32) {
                k = util_RandomU(4);
                if (_strcmpi(sv_SkyLayersT[0]->t->name, sv_SkyFair[k].name) == 0)
                    k++;
                sky = &sv_SkyFair[k];
            } else if (c < 0x96) {
                k = util_RandomU(3);
                if (_strcmpi(sv_SkyLayersT[0]->t->name, sv_SkyMedium[k].name) == 0)
                    k++;
                sky = &sv_SkyMedium[k];
            } else {
                k = util_RandomU(2);
                if (_strcmpi(sv_SkyLayersT[0]->t->name, sv_SkyHeavy[k].name) == 0)
                    k++;
                sky = &sv_SkyHeavy[k];
            }
            d3sky_SetLayerTex(sv_SkyLayersT[0], sky->name);
            sv_SkyLayersT[0]->scale = sky->scale;
            sv_SkyLayersT[0]->vscale = sky->vscale;
            sv_SkyLayersT[0]->voffset = sky->voffset;
            d3sky_FadeLayer(d3_Sky, sv_SkyLayersT[1], 0, 1500.0f);
        }
        d3sky_SetLayerSpeed(d3_Sky, sv_SkyLayersT[0], w * 0.75);
        d3sky_SetLayerSpeed(d3_Sky, sv_SkyLayersT[1], w);
        if (c == 0)
            d3sky_FadeLayer(d3_Sky, sv_SkyLayersT[2], 0, 2500.0f);
        else
            d3sky_FadeLayer(d3_Sky, sv_SkyLayersT[2], (c << 6) / 1000 + 0x60, 1000.0f);
        d3sky_SetLayerSpeed(d3_Sky, sv_SkyLayersT[2], w * 1.5);
    }
}
/* 0x530670, the weather of a frame (sv_RunFrame): draws the rain / snow emitters; in a stormy hour (sv_Weather.storm) a thunder now and then (1 in 300 frames, at storm 3
   another 1 in 100: sv_Weather.thunderStart / thunderLen and the sample "DonnerMittel_s"); in the town (d3_CurrentScene 0), when the clouds clear in an hour from 11 to 17
   o'clock past its 30th minute, the sun rays (3 in 8) and their hour sv_SunRaysHour; past the 30th minute of a later hour they fade out
   @deep Read whole against 0x530670..0x5307eb and rewritten: rain/snow draw, the thunder roll (storm[h],
   1/300 or storm 3 and 1/100, thunderStart/thunderLen, sb_PlaySample with the 7 arguments the original
   pushes, volume init_SoundVolume*127.0f), the sun rays when clouds[h-1] != 0 and clouds[h] == 0 between
   11 and 17 past minute 30 (ai_Time.min), sv_SunRaysHour int (the signed jle). Every statement matches;
   diffclass: regs 8/4/20 (eax<->ecx: the original keeps ai_Time.hour in ecx and zero-extends into eax, ours
   the reverse) and one code hunk at 0x530734 that is the same cascade: the reload of ai_Time.hour before
   d3_CurrentScene (+ mov ecx,[ai_Time.hour] / mov eax,[d3_CurrentScene] vs ours mov eax,[ai_Time+4] / mov
   ecx,[d3_CurrentScene]) and the clouds[h-1]/clouds[h] addressing (+ shl eax,2 / mov
   edx,[eax+sv_Weather-4] / mov edx,[eax+sv_Weather] vs ours lea ecx,[edx*4+sv_Weather] / cmp [ecx],0).
   Tried (&clouds[-1])[h] and clouds[(int)h-1]: same 47. Left: allocation / addressing selection. */

void sv_WeatherFrame(void)
{
    if (d3t_Rain != 0)
        d3t_DrawRain(d3t_Rain);
    if (d3t_Snow != 0)
        d3t_DrawSnow(d3t_Snow);
    if (sv_Weather.storm[ai_Time.hour] != 0 && (util_RandomU(300) == 0 || (sv_Weather.storm[ai_Time.hour] == 3 && util_RandomU(100) == 0))) {
        sv_Weather.thunderStart = d3_Time;
        sv_Weather.thunderLen = util_RandomU(8) + 8;
        sb_PlaySample(snd_ThunderBank, "DonnerMittel_s", 1, 0x3f, (int)(init_SoundVolume * 127.0f), 0, 0);
    }
    if (d3_CurrentScene == 0) {
        if (ai_Time.hour >= 11 && ai_Time.hour <= 17 && sv_Weather.clouds[ai_Time.hour - 1] != 0 && sv_Weather.clouds[ai_Time.hour] == 0 && ai_Time.min > 30) {
            if (util_RandomU(0x80) > 0x60 && d3t_SunRays == 0)
                d3t_CreateSunRays();
            sv_SunRaysHour = ai_Time.hour;
        }
        if (sv_SunRaysHour != -1 && ai_Time.hour > sv_SunRaysHour && ai_Time.min > 30) {
            if (d3t_SunRays != 0)
                d3t_FadeOutSunRays(d3t_SunRays, 1);
            sv_SunRaysHour = -1;
        }
    }
}
/* 0x5307f0, the advance game screen, also its own sv_RunFrame callback: when ai_RunAllows(8), the form "misc\advancegame" with text 0x82 while the game runs at
   option speed 0x2ee (cm_SpeedFactor 4.9375) with the frame mask's bits 3, 4 and 10..13 cleared and 6, 7 set, until a key or a mouse button; then the speed option
   and its factor (speed * 0.00625 + 0.25) come back; ai_CampaignStep(0xb, ..) brackets it */

void sv_AdvanceGame(void)
{
    unsigned mask;
    int sp, f;

    mask = sv_FrameMask;
    sp = ai_OptionSpeed;
    if (ai_RunAllows(8) != 0) {
        ai_CampaignStep(0xb, 0, 0, 0);
        mask = mask & 0xffffc3e7 | 0xc0;
        sv_Advancing = 1;
        ai_OptionSpeed = 0x2ee;
        cm_SpeedFactor = 4.9375f;
        f = d2_OpenForm(0, 0, "misc\\advancegame");
        d2_CentreViewWindows(f);
        io_PrintText(0x82);
        while (sv_RunFrame((int)sv_AdvanceGame, mask) != 0) {
            if (kb_Last != 0 || dm_L.lup != 0 || dm_L.rup != 0)
                sv_PopRequest = 1;
        }
        sv_Advancing = 0;
        cm_SpeedFactor = (float)(sp * 0.00625f + 0.25);
        ai_OptionSpeed = sp;
        d2_CloseView(f);
        ai_CampaignStep(0xb, 0, 1, 0);
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
        if ((fl & 0x100) != 0) {
            d2_HalfScreenW = (int)(d3_ViewW * 0.5f + d3_ViewX - d2_ScreenScale * 6.0f);
            d2_HalfScreenH = (int)(d3_ViewH * 0.5f + d2_ScreenScale * 10.0f + d3_ViewY);
        } else {
            d2_HalfScreenW = (int)(d3_ViewW * 0.5f + d3_ViewX);
            d2_HalfScreenH = (int)(d3_ViewH * 0.5f + d3_ViewY);
        }
        return;
    }
    d2_HalfScreenW = d2_ScreenW >> 1;
    d2_HalfScreenH = d2_ScreenH >> 1;
}

int cm_PartyById();                     /* 0x4e50a0 */
void main_GameLoop(int ctl);                     /* 0x53bb20 */
void sv_UpdateClock(void);

/* 0x5309a0, "MASTER": one frame of the game screen loop, called by every screen with its own callback (the top of the callback stack 0x75f780, 8 bytes per entry, depth 0x6f229c; 0 = nothing to register) and the
   mask of the parts to run (param_2: 1 input, 4 resize, 0x10 objects, 0x20 speech, 0x40 world, 0x100 left click, 0x1000 names, 0x2000 main buttons, 0x4000 weather, 0x10000 paused, 0x100000 modal, ..); returns 1
   while the caller's screen is the top screen, 0 when it must end. The Esc key (mask 0x2000) opens the options menu with the music / sfx volumes lowered to a third
   and restores them after it.
   @deep Read in full against the original 0x5309a0..0x5315bc; all statements reconciled (frame stack
   push/pop, the unsigned clock compares, the mask & 0x80 tests, the band / name-plate / scroll-arrow
   branch orders, the Esc volume block master*1/127 with the 1/3 terms, the save-name clear, name buffer
   0x104, frame 0x214 now equal, 830 vs 830 instructions). One hunk left: ours CSEs sv_FrameDepth across
   the merge into the pop arm (- 0x530b21 mov ecx,[sv_FrameDepth] on the err/PopRequest edge, then test ecx
   / [ecx*8+sv_Frames] / dec ecx), the original reloads it inside the arm (+ 0x530b36 mov
   eax,[sv_FrameDepth], test eax / [eax*8+sv_Frames] / dec eax). Tried: sv_Frames[sv_FrameDepth--].cb = 0,
   sv_Frames[++sv_FrameDepth].cb = cb - same. A load placement / register choice: allocation. */
int sv_RunFrame(int cb, unsigned mask)
{
    int r, i;
    AIBLD *c;
    G28REC *party;
    float master;
    unsigned u5;
    char name[0x104], tmp[0x104];

    sv_FrameMask = mask;
    sv_PumpMessages();
    di_GetMouse();
    if (cb != 0) {
        if (sv_Frames[sv_FrameDepth].cb != cb)
            di_ClearButtons();
        if (ai_PlayerId != 0xffff) {
            if ((ai_CharsT[ai_PlayerId].kind == 6 || ai_CharsT[ai_PlayerId].kind == 7) && ai_CharsT[ai_PlayerId].health == 0 && sv_FrameDepth > sv_FrameBase
                && (party = (G28REC *)cm_PartyById(ai_CharsT[ai_PlayerId].partyId)) != 0 && party->kind == 8 && (mask & 0x10000) == 0) {
                sv_PopRequest = sv_FrameDepth - sv_FrameBase;
                sv_SetStatusText(txt_Ptrs[0x89]);
            }
        }
    }
    if (main_Inactive != 0)
        return 1;
    if ((mask & 4) != 0 && (d2_MouseUpdate(), main_Inactive != 0))
        ai_FrameWait = 1;
    if (sv_PopRequest == 0 && (mask & 0x100) != 0 && (mask & 0x100000) == 0)
        sv_HandleRightClick(mask);
    if (cb != 0) {
        if (sv_Frames[sv_FrameDepth].cb == cb) {
            r = 1;
        } else {
            if (sv_PopRequest == 0) {
                r = 1;
                for (i = 0; i < 0x80; i++) {
                    if (sv_Frames[i].cb == cb) {
                        err_Message("MASTER(): Function already registrated!");
                        goto l21;
                    }
                }
                sv_FrameDepth++;
                sv_Frames[sv_FrameDepth].cb = cb;
                goto l64;
            }
l21:
            r = 0;
        }
        if (sv_PopRequest != 0 && r != 0) {
            sv_PopRequest--;
            if (sv_FrameDepth != 0) {
                sv_Frames[sv_FrameDepth].cb = 0;
                sv_FrameDepth--;
            }
            r = 0;
        }
    } else {
        r = 1;
    }
l64:
    if ((mask & 1) != 0 && ai_GameOver == 0) {
        if ((mask & 0x100) != 0)
            he_PickActiveEvent();
        he_RunQuickList();
        if (net_Conn.s == (unsigned)-1 && (unsigned)d3_Time > (unsigned)cm_NextNetStep && ai_GameState != 0)
            cm_SendTime((unsigned)&ai_TimeCopy);
        if ((mask & 0x10000) == 0 && ai_GameState != 0 && sv_PanelFlag == 0 && cm_UpdateParties() != 0 && sv_Advancing != 0)
            sv_PopRequest = 1;
    }
    if ((mask & 0x20000) != 0 && (unsigned)d3_Time > (unsigned)cm_NextNetStep) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        cm_NextNetStep = d3_Time + 7;
    }
    if (r == 0 || sv_PanelFlag != 0)
        ai_FrameWait++;
    if ((mask & 0x200) != 0)
        evt_StepAll();
    if (ai_FrameWait == 0 && ((mask & 0x80) == 0 || (d2_FlushDirty(), ai_FrameWait == 0)) && (mask & 0x40) != 0 && sv_WorldOn != 0) {
        if ((mask & 0x10000) == 0 && (mask & 0x40000) != 0)
            sv_UpdateSkyPhase((unsigned)&ai_Time);
        if (d3_CurrentScene == 0)
            d3q_Mark(d3_QuadTree, d3_Camera, d3_FrameCount);
        d3t_UpdateCamera();
        d3if_ClearView();
    }
    c = (AIBLD *)ai_PlayerBuilding;
    if (c == 0)
        c = (AIBLD *)ai_FallbackBuilding;
    u5 = mask & 0x8000;
    if (u5 == 0 && ai_GameState != 0)
        sv_CollectGroupCharacters((char *)c);
    if (c == 0 || (mask & 0x10000) != 0) {
        ch_CurrentGroup = -1;
    } else {
        sv_RunLocationScripts(c);
        sv_UpdateMemberList((unsigned char *)c);
        ch_CurrentGroup = c->id;
    }
    if (ai_GameState != 0) {
        ai_UpdateWind();
        gv_UpdateEffects();
    }
    if (sv_CharsOn != 0) {
        ch_UpdatePositions();
        ch_Update();
        ch_SyncObjectOffsets();
    }
    if ((mask & 0x4000) != 0 && ai_Time.hour != sv_WeatherHour)
        sv_UpdateWeather();
    if (main_WeatherSound != 0)
        snd_UpdateRain();
    if (snd_SfxEnabled != 0) {
        snd_UpdateAnimals();
        snd_UpdateQueue();
    }
    if (snd_MusicEnabled != 0 && (mask & 0x40000) != 0)
        snd_UpdateMusic();
    if (ai_FrameWait == 0
        && ((d3_Terrain == 0 || (mask & 0x4000) == 0 || (mask & 0x40) == 0 || (sv_WeatherFrame(), ai_FrameWait == 0))
            && ((mask & 0x10000) == 0 || cm_SceneRain == 0 || (d3t_DrawRain(cm_SceneRain), ai_FrameWait == 0)) && sv_BandActive != 0)) {
        if (u5 != 0)
            sv_UpdateBandLed();
        else
            sv_UpdateBand();
        sv_DrawRubberBand();
    }
    if (snd_SfxEnabled != 0) {
        d3snd_Update();
        if (main_WeatherSound != 0)
            d3sndw_Update();
        sb_Update();
    }
    if (sv_PopRequest == 0 && (mask & 0x100000) == 0) {
        if (u5 == 0)
            sv_HoverPick();
        if ((mask & 0x10) == 0) {
            if (dm_L.rclick != 0)
                goto l2;
            if (d2_PointerObj == -1 || (((D2OBJ *)d2_Objs)[d2_PointerObj].style & 0x40) == 0)
                d2_CopyCaption(ch_EmptyName);
            ai_PickedObject = 0;
            sv_HoverItem = 0;
            if (u5 == 0)
                d2_SetPointerSprite(0, 0);
        }
        if (dm_L.rclick == 0) {
            if ((mask & 0x10) != 0)
                sv_HandleObjects();
            if ((mask & 0x20) != 0) {
                sv_HandleSelectionMouse();
                sv_SpeakPicked();
            }
        }
    }
l2:
    if ((mask & 0x400) != 0)
        ai_HandleItemDrag();
    if (ai_FrameWait == 0 && (mask & 0x80) != 0 && d2_LockSurface(d2_ScreenSurf) != 0) {
        if (sv_PanelPic != -1 && (mask & 0x100) != 0 && sv_CityPanelOn != 0)
            sv_UpdateCityPanel(2, mask);
        if (main_ObjectsOn != 0)
            d2_DrawObjects();
        d2_UnlockSurface(d2_ScreenSurf);
        d2_DrawFades();
    } else if (sv_PanelFlag != 0)
        d2_FinishFades();
    di_PollMouse();
    if (di_Absolute != 0) {
        d2_MouseX = dm_P.x;
        d2_MouseY = dm_P.y;
    }
    sv_SetScreenCenter(mask);
    if (ai_FrameWait == 0 && (mask & 0x80) != 0) {
        if (d2_LockSurface(d2_ScreenSurf) != 0) {
            if (cm_FightStart != 0)
                cm_DrawFightTime();
            if ((mask & 0x1000) != 0 && sv_FrameDepth == sv_FrameBase && he_ActiveSlot == 0) {
                sv_DrawBubbles();
                if (kb_Down[0x1d] != 0)
                    sv_DrawNamePlates();
                else if (kb_Down[0x38] != 0)
                    sv_DrawItemNames();
            }
            if ((mask & 0x80000) != 0)
                cm_DrawScrollArrows();
            else if (sv_PanelShown != 0 || ai_FallbackBuilding != 0)
                sv_DrawBorderArrows();
            if ((mask & 0x1000) != 0 && dm_L.rclick == 0 && sv_CameraScrolled == 0 && d3t_CamMove == 0 && sv_BandActive == 0 && d2_CutsceneActive == 0 && he_ActiveSlot == 0)
                sv_DrawHint();
            sv_DrawStatusText();
            sv_DrawPanelPointer();
            d2_UnlockSurface(d2_ScreenSurf);
        }
        dd_Flip();
    }
    if (kb_Last == 1 && (mask & 0x2000) != 0) {
        master = (float)ai_SoundOptions.masterVolume * (1.0f / 127.0f);
        msx_SetVolume((int)((float)ai_SoundOptions.msxVolume * master * (1.0f / 3.0f)));
        sb_SetMasterVolume((int)((float)ai_SoundOptions.sfxVolume * master * (1.0f / 3.0f)));
        ai_OptionsMenu();
        master = (float)ai_SoundOptions.masterVolume * (1.0f / 127.0f);
        msx_SetVolume((int)((float)ai_SoundOptions.msxVolume * master));
        sb_SetMasterVolume((int)((float)ai_SoundOptions.sfxVolume * master));
    }
    sv_UpdateQuickChat();
    if (ai_GameState != 0) {
        if ((ai_GameFlags & 4) != 0 && d2_FocusedInput == 0 && kb_Last == 0x1c && (mask & 0x2000) != 0)
            sv_ChatConsole();
        if (ai_GameState != 0) {
            if ((kb_Last == 0xcf || (dm_L.lup != 0 && d2_HitUnderPointer == sv_AdvanceHit)) && (mask & 0x10000) == 0 && sv_Advancing == 0 && (ai_GameFlags & 4) == 0) {
                if (gm_AllowAll == 0)
                    sv_AdvanceGame();
                else {
                    main_DayEnding = 1;
                    sv_PopTo((int)main_GameLoop);
                }
            }
            if (ai_GameState != 0) {
                if (sv_MenuOpen == 0 && (ai_GameFlags & 4) == 0 && u5 == 0 && kb_Last == '9')
                    ai_PauseGame();
                if (ai_GameState != 0 && (mask & 0x300000) == 0)
                    ai_SpeedKeys();
            }
        }
    }
    if (sv_PanelPic != -1 && (mask & 0x2000) != 0 && (mask & 0x100000) == 0 && sv_FrameDepth == sv_FrameBase) {
        if (ai_GameState != 0)
            ai_SlotKeys();
        if (ai_PlayerId == 0xffff || ai_CharsT[ai_PlayerId].health <= 0) {
            if (d2_ClickedId != -1
                && (d2_PointerObj == sv_ButtonPrivileges || d2_PointerObj == sv_ButtonCompare || d2_PointerObj == sv_ButtonInventory || d2_PointerObj == sv_ButtonPlayerStats || d2_PointerObj == sv_ButtonCityStats
                    || d2_PointerObj == sv_ButtonFamilyTree))
                sv_SetStatusText(txt_Ptrs[0x89]);
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == sv_ButtonPrivileges)
                ai_PrivilegesPanel();
            else if (d2_PointerObj == sv_ButtonCompare)
                ai_PlayerComparePanel();
            else if (d2_PointerObj == sv_ButtonInventory)
                ai_InventoryPanel();
            else if (d2_PointerObj == sv_ButtonPlayerStats)
                ai_PlayerStatsPanel();
            else if (d2_PointerObj == sv_ButtonCityStats)
                ai_CityStatsPanel();
            else if (d2_PointerObj == sv_ButtonMessages)
                ai_MessageFilterScreen();
            else if (d2_PointerObj == sv_ButtonFamilyTree)
                ai_FamilyTreeWindow(ai_CharsT[ai_PlayerId].netId, 0x11, 0, 0x657c7);
        }
    }
    sv_TickFrames++;
    if ((mask & 0x10000) == 0) {
        if (sv_PopRequest == 0 && sv_PanelFlag == 0 && cm_DayEndRequest != 0 && main_DayEnding == 0) {
l3f5:
            if ((ai_GameFlags & 0x80) == 0) {
                main_DayEnding = 1;
                tb_SetFunctionOff(sv_UpdateClock, 1);
                sv_FrameMask |= 0x100000;
                sv_MessageBoxYesNo((int)txt_Ptrs[0x81], 0xc4);
                sv_PopTo((int)main_GameLoop);
            }
            goto l444;
        }
        if (net_Conn.s != (unsigned)-1 || ai_Time.hour < 0x17)
            goto l444;
        if (main_DayEnding == 0)
            goto l3f5;
    } else {
l444:
        if (main_DayEnding == 0)
            goto l473;
    }
    if (sv_PopRequest == 0 && sv_Frames[sv_FrameDepth].cb == (int)ai_GameFrame)
        sv_PopRequest = 1;
l473:
    if ((mask & 0x10000) == 0 && sv_InMissionComplete == 0 && ai_MissionPartner != -1 && ai_GameState != 0) {
        sv_InMissionComplete = 1;
        tb_SetFunctionOff(sv_UpdateClock, 1);
        ai_MissionCompleteSequence(0x65fff);
        tb_SetFunctionOff(sv_UpdateClock, 0);
        sv_InMissionComplete = 0;
    }
    if (cm_RequestedSaveName[0] != 0 && (sv_FrameMask & 0x200000) == 0) {
        if (cm_RequestedSaveSlot == 1)
            strcpy(name, "QUICKSAVE");
        else if (cm_RequestedSaveSlot == 0)
            strcpy(name, "AUTOSAVE");
        else
            sprintf(name, "GILDE_SAVEGAME_%i", cm_RequestedSaveSlot);
        strcpy(tmp, cm_RequestedSaveName);
        cm_RequestedSaveName[0] = 0;
        ai_SaveNetworkGame(name, tmp);
    }
    if ((unsigned)cm_NoticeCount > 0 && (sv_FrameMask & 0x200000) == 0) {
        cm_NoticeCount--;
        sv_BroadcastText(mask, txt_Ptrs[0x1cb2]);
    }
    if (0 < ai_FrameWait)
        ai_FrameWait--;
    return r;
}
/* 0x5315c0, a callback of 0x42ea10 (the screens' wait loops): while the clock is shown by a game speed (0x6f3608: seconds = (speed * 0.00625 + 0.5) * 0x6248b4) it stores that time of day (hours / minutes /
   seconds) in the clock record 0x75e130 (0x4fe140); skipped in the day phase 0x17 and later unless the flag bit 7 of 0x6f23f8 is set; a start Ghidra merged into 0x5309a0 */

void sv_UpdateClock(void)
{
    int t, h, m;

    if ((SV_US((unsigned)&ai_Unk75e134) >= 0x17 || SV_US((unsigned)&ai_Time.hour) >= 0x17) && (SV_B((unsigned)&ai_GameFlags) & 0x80) == 0)
        return;
    t = (int)(((float)SV_I((unsigned)&ai_OptionSpeed) * 0.00625f + 0.5) * SV_I((unsigned)&full_data_6248b4));
    h = t / 3600;
    m = (t - h * 3600) / 60;
    gm_AddTime((unsigned)&ai_TimeCopy, h, m, t - (m + h * 60) * 60);
}

extern int sv_SkyScene;
extern float sv_SkyScale;
extern int sv_BandActive;
extern int sv_SkyPhase;
extern int sv_CloudChance;
extern int sv_SeasonClimate;
extern int sv_WinterWeather;
extern int sv_WeatherHour;
extern int d3t_Snow;
extern int d3_SkyLayers[6];
extern int d2_HitWindow;
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
/* txt_NetworkSyncWait: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnBuild: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnOwn: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnKind1e: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnForeign: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnItemOwn: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnItem: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelPersons: view of txt_Ptrs (build/mk_txt_views.py) */
int d2_PointerObj = -1;  /* 0x5fe710 */
float sv_FramesPerTick = 30.0f;  /* 0x62378c */
int sv_TickFrames = 30;  /* 0x623790, the frames drawn since the last timer tick (counted by the frame loop) */
char sv_GroundplanColours[] = "\377\376\005\005%\377\377/\021\005\377\030\3746\343\377\231\005\005\377\366z\005\005\266W\377<\005P\031H\005\013\304^\305\005g\357\234\231\005\010r";  /* 0x623794, the table of */
int sv_StatusTime = -1;  /* 0x6237c4, when the status line was set (d3_Time) */
unsigned sv_SceneEventTime = 0xffffffff;  /* 0x6237c8, d3_Time when sv_RunLocationScripts last fired event 6 of the default scene (every 500 ms; the scene changes of unk_58bbd0.c reset it to 0) */
int sv_PanelRebuild = 1;  /* 0x6237cc */
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
char sv_CameraEaseOn = 1;  /* 0x62381c */
int sv_ButtonFamilyTree = -1;  /* 0x623820 */
int sv_ButtonCompare = -1;  /* 0x623824 */
int sv_ButtonCityStats = -1;  /* 0x623828 */
int sv_ButtonInventory = -1;  /* 0x62382c */
int sv_ButtonPlayerStats = -1;  /* 0x623830 */
int sv_ButtonMessages = -1;  /* 0x623834 */
int sv_PanelBuildingIcon = -1;  /* 0x623838 */
int sv_PanelCompass = -1;  /* 0x62383c */
int sv_BarWindow = -1;  /* 0x623840 */
int sv_PanelUnusedObj = -1;  /* 0x623844 */
int sv_ButtonPrivileges = -1;  /* 0x623848 */
int sv_PanelMoneyText = -1;  /* 0x62384c */
int sv_AdvanceHit = -1;  /* 0x623850 */
int sv_PanelPic = -1;  /* 0x623854 */
int sv_PlayerBarForm = -1;  /* 0x62385c, the player bar's form, -1 = closed */
int sv_PanelForm = -1;  /* 0x623860 */
int sv_PanelBldPic = -1;  /* 0x623864 */
int sv_PanelHealthBar = -1;  /* 0x623868 */
int sv_PanelWidgets[4] = {-1, -1, -1, -1};  /* 0x62386c, the info panel's 4 widget ids (freed by one loop: 0x62386c .. 0x62387c) */
int sv_PanelCartWin = -1;  /* 0x62387c */
int sv_PanelPortrait = -1;  /* 0x623880 */
int sv_PanelBuildBtn = -1;  /* 0x623884 */
int sv_PanelItemBtn = -1;  /* 0x623888 */
int sv_PanelTransportBtn = -1;  /* 0x62388c */
int sv_PanelCartBtn = -1;  /* 0x623890 */
int sv_PanelActionBtn = -1;  /* 0x623894 */
int sv_PanelKind1eBtn = -1;  /* 0x623898 */
int sv_BarToggle = -1;  /* 0x6238a4 */
/* 0x6238a8, the skies of fair weather (sv_UpdateWeather picks one below 50 clouds, cm picks one for the fair cutscene) */
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
/* 0x623ac0, the skies of heavy cloud (150 clouds and above; cm picks one for the stormy cutscene) */
SVSKYREC sv_SkyHeavy[3] = {
    { "sky_schwer_01", 3.0f, 0.8f, -0.25f },
    { "sky_schwer_02", 3.0f, 0.8f, -0.25f },
    { "sky_dunkel_01", 3.0f, 0.8f, -0.25f },
};
/* 0x623ba8, the day tables: the 6 day phases (SVDAYPHASE, 10 bytes each, sv_FillDayPhases writes them from the season's
   minutes; their int seconds sit at odd addresses, so the object is packed) and the sky key colours sv_SkyKeys (+0x38) /
   sv_SkyKeysAlt (+0xa8); one object, the last phase's 4 pad bytes are the first key's colour */
ALIGN1 char sv_DayTables[278] = { 4, 0, 64, 56, 0, 0, 0, 0, 0, 0, 6, 0, 96, 84, 0, 0, 0, 0, 0, 0, 12, 0, -64, -88, 0, 0, 0, 0, 0, 0, 16, 0, 0, -31, 0, 0, 0, 0, 0, 0, 18, 0, 32, -3, 0, 0, 0, 0, 0, 0, 19, 0, 48, 11, 1, 0, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, -18, 66, 0, 0, 15, 67, 0, 0, 90, 67, 0, 0, -81, 67, 0, 0, 85, 67, 0, 0, 52, 67, 0, 0, 32, 67, 0, -128, -69, 67, 0, 0, 127, 67, 0, 0, 78, 67, 0, 0, -10, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 57, 67, 0, 0, -84, 66, 0, -128, -1, 67, 0, 0, 34, 67, 0, 0, 1, 67, 0, 0, 69, 67, 0, 0, -93, 67, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0, -20, 67, 0, 0, -34, 66, 0, 0, 13, 67, 0, 0, 53, 67, 0, -128, -30, 67, 0, 0, 59, 67, 0, 0, 83, 67, 0, 0, 26, 67, 0, 0, -71, 67, 0, 0, 127, 67, 0, 0, 66, 67, 0, 0, -22, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 47, 67, 0, 0, 104, 66, 0, 0, -49, 67, 0, 0, 76, 67, 0, 0, 64, 67, 0, 0, 53, 67, 0, -128, -47, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0 };
int ai_Unk623cc0[] = { 1104150528, 1097859072, 1116602368, 0, 1102577664, 1107820544, 1113325568, 0, 1116209152, 1112276992, 1110179840, 0, 1120272384, 1117519872, 1109393408, 0, 1116602368, 1113587712, 1108606976, 0, 1107820544, 1102053376, 1112539136, 0, 1104150528, 1097859072, 1116602368, 0, 1122500608, 1124270080, 1132396544, 1133019136, 1121452032, 1125056512, 1128792064, 1133969408, 1123680256, 1127284736, 1126170624, 1134592000, 1124990976, 1128726528, 1130168320, 1135247360, 1126629376, 1127153664, 1126957056, 1135411200, 1124990976, 1124728832, 1128792064, 1135706112, 1122107392, 1124270080, 1132396544, 1132691456, 1124401152, 1121976320, 1132396544, 1134592000, 1122893824, 1125384192, 1125122048, 1138425856, 1123287040, 1125253120, 1125122048, 1136361472, 1125318656, 1127612416, 1128595456, 1135706112, 1124597760, 1124139008, 1125122048, 1135083520, 1125777408, 1128267776, 1129185280, 1134264320, 1124401152, 1121976320, 1132396544, 1134428160 };  /* 0x623cc0 */
int ai_Unk623e10[] = { 1099431936, 1097859072, 1116602368, 0, 1096810496, 1104674816, 1114112000, 0, 1096810496, 1102053376, 1114112000, 0, 1101004800, 1107296256, 1113325568, 0, 1101004800, 1103101952, 1114898432, 0, 1101004800, 1094713344, 1110966272, 0, 1099431936, 1097859072, 1116602368, 0 };  /* 0x623e10 */
int sv_LampPhase = -1;  /* 0x623e80 */
int sv_LoadSlider = -1;  /* 0x623e84, the object handle of the scene load's progress slider (sv_SceneLoadProgress), -1: none */
int sv_PanelEnabled = 1;  /* 0x623e88 */
int sv_GroundplanScene = -1;  /* 0x623e8c */
float sv_DayPhaseFrac = 0.5f;  /* 0x623e94 */
int sv_PickX = -1;  /* 0x623e98, the pointer x of the last hover pick (sv_HoverPick) */
int sv_PickY = -1;  /* 0x623e9c, the pointer y of the last hover pick */
int ai_Unk623ea0 = -1;  /* 0x623ea0 */
int ai_Unk623ea4 = -1;  /* 0x623ea4 */
int ai_Unk623ea8 = -1;  /* 0x623ea8 */
int sv_CharsOn = 1;  /* 0x623eac, nonzero: sv_RunFrame updates the characters (ch_UpdatePositions, ch_Update, ..) */
int sv_WorldOn = 1;  /* 0x623eb0, nonzero: sv_RunFrame steps the 3D world (sky phase, quad tree, camera, view clear) */
int sv_CityPanelOn = 1;  /* 0x623eb4, nonzero: sv_RunFrame redraws the city panel */
unsigned char sv_ChatRecipients[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };  /* 0x623ebc, the chat console's recipient buttons, one per human player in ai_Chars order (1 = checked), kept between openings */
unsigned sv_ChatTime = 0xffffffff;  /* 0x623ec4, d3_Time when the last chat line arrived (cm.c's 0x4b handler), 0 / -1: none */
int sv_QuickChatForm = -1;  /* 0x623ec8 */
int sv_ViewRectLeft = 0;  /* 0x6248a4 */
int sv_ViewRectTop = 0;  /* 0x6248a8 */
int sv_SunRaysHour = -1;  /* 0x623ecc, the hour the sun rays started (sv_WeatherFrame), -1: none */
int sv_ViewRectRight = 694;  /* 0x6248ac */
int sv_ViewRectBottom = 600;  /* 0x6248b0 */
int ai_DragWindow = -1;  /* 0x62a8cc */
int sv_PanelCartChar = 0;  /* 0x631078 */
short d2_PointerLook = 0;  /* 0x6ed9d0, the pointer direction / panel layout code (1 .. 8) */
int sv_SavedMaxY = 0;  /* 0x6f1c40 */
float sv_ScrollVelYPrev = 0;  /* 0x6f1c44 */
int sv_DragLastY = 0;  /* 0x6f1c48 */
int sv_HoverLast = 0;  /* 0x6f1c4c, the scene object sv_HoverPick picked last (0: none) */
int sv_SavedMaxX = 0;  /* 0x6f1c50 */
float sv_DragLastPos[3] = {0};  /* 0x6f1c64 */
float sv_ScrollLastMove[3] = {0};  /* 0x6f1c74 */
int sv_SavedMinX = 0;  /* 0x6f1c84 */
int sv_DragLastX = 0;  /* 0x6f1c88 */
int sv_ZoomStartY = 0;  /* 0x6f1c8c */
unsigned short sv_DisabledWidgets[0x300] = {0};  /* 0x6f1c90, the d2 objects sv_DisableVisibleWidgets disabled (one per d2_Objs record at most) */
int sv_ZoomStartX = 0;  /* 0x6f2290 */
int sv_ShownModel = 0;  /* 0x6f2294, sv_SelectedModel when sv_UpdatePanel last built the panel */
int sv_SavedMinY = 0;  /* 0x6f2298 */
int sv_FrameDepth = 0;  /* 0x6f229c, the depth of the frame callback stack */
int sv_PopRequest = 0;  /* 0x6f22a0 */
int sv_FrameBase = 0;  /* 0x6f22a4, the depth a screen was entered at (the base of the pop count) */
int sv_FocusDepth = 0;  /* 0x6f22a8, sv_FrameDepth when sv_FocusObject was focused (sv_HandleObjects); the camera code releases the focus when the frame stack drops below it */
unsigned char sv_Advancing = 0;  /* 0x6f22ac, nonzero while the advance game screen runs (0x5307f0, form "misc\advancegame"); sv_RunFrame tests it */
int sv_CameraScrolled = 0;  /* 0x6f22b4 */
int sv_ViewShape = 0;  /* 0x6f22c0 */
struct D2SURF *sv_GroundplanSurf = 0;  /* 0x6f22c4, the groundplan bitmap while sv_LoadGroundplan reads it */
struct D2SURF *sv_GroundplanView = 0;  /* 0x6f22c8, the groundplan copy the panel blits */
int sv_BorderViewX = 0;  /* 0x6f22cc */
int sv_BorderViewY = 0;  /* 0x6f22d0 */
float sv_CameraHeightFactor = 0;  /* 0x6f22d4 */
float sv_CameraAngleFactor = 0;  /* 0x6f22d8 */
char * sv_FocusObject = 0;  /* 0x6f22ec, the clicked object the camera was placed at (d3t_PlaceAtObject): its handler (+0x1d4) gets message 2 at the focus (+0x108) and 3 at the release (+0x18c) */
int sv_HoverItem = 0;  /* 0x6f22f0, the item (AIOBJ) under the pointer, from the picked object's +0x200 (kind 1) */
int sv_HoverBuilding = 0;  /* 0x6f22f4, the building (AIBLD) under the pointer (kind 2) */
int sv_ClickedContact = 0;  /* 0x6f22fc, sv_HoverContact at the last click (sv_HandleObjects) */
int sv_HoverContact = 0;  /* 0x6f2300, the goods item a contact gate under the pointer resolves to */
int sv_SelectedModel = 0;  /* 0x6f2304 */
char * ai_PlayerBuilding = 0;  /* 0x6f2308 */
char * ai_FallbackBuilding = 0;  /* 0x6f230c */
unsigned short * sv_PanelShown = 0;  /* 0x6f2310, the scene record (room) the info panel shows now */
int sv_PanelPerms = 0;  /* 0x6f2320, the player's permission bits (gm_PermissionBits) for the building / room the panel acts on, tested for 0x10 / 0x1000 */
int sv_HoverBldPerms = 0;  /* 0x6f2324, the player's permission bits for sv_HoverBuilding */
int sv_HoverItemPerms = 0;  /* 0x6f2328, the player's permission bits for sv_HoverItem */
int sv_HoverCharPerms = 0;  /* 0x6f232c, the player's permission bits for the character under the pointer */
int ai_SelectedCount = 0;  /* 0x6f2334 */
unsigned char sv_OtherSelected = 0;  /* 0x6f2338, nonzero: a selection of another kind is held (reasoned: like sv_SelectedModel it keeps sv_UpdatePanel from the default panel, and it blocks the object pick 0x52adb0) */
int sv_ResearchClass = 0;  /* 0x6f233c */
char sv_LampSwitches = 0;  /* 0x6f2340 */
char sv_MessageBoxOpen = 0;  /* 0x6f2350 */
char sv_MessageBoxGreenOpen = 0;  /* 0x6f2351 */
char sv_MessageBoxYesNoOpen = 0;  /* 0x6f2352 (the yes / no box) */
int sv_LoadFrame = 0;  /* 0x6f2354, the frame counter of the scene load (sv_SceneLoadProgress): every 10th frame redraws the bar */
int sv_ScreenDirty = 0;  /* 0x6f2358 */
int sv_GroundplanBuilding = 0;  /* 0x6f235c */
int sv_GroundplanRoom = 0;  /* 0x6f2360 */
char sv_PlayerBarKind = 0;  /* 0x6f2368 */
int sv_DayPhase = 0;  /* 0x6f236c */
int sv_ThunderFog = 0;  /* 0x6f2370 */
float sv_CameraHeightStep = 0;  /* 0x6f2374 */
int sv_BorderMoved = 0;  /* 0x6f2378 */
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
int sv_ShownBuilding = 0;  /* 0x6f23bc, ai_PlayerBuilding when sv_UpdatePanel last built the panel */
int sv_ShownFallback = 0;  /* 0x6f23c0, ai_FallbackBuilding when sv_UpdatePanel last built the panel */
int sv_ShownObject = 0;  /* 0x6f23c4, sv_SelectedObject when sv_UpdatePanel last built the panel */
int sv_ShownRoom = 0;  /* 0x6f23c8, sv_PanelShown when sv_UpdatePanel last built the panel */
int sv_ShownItem = 0;  /* 0x6f23cc, sv_SelectedItem when sv_UpdatePanel last built the panel */
int sv_ShownChar = 0;  /* 0x6f23d0, sv_SelectedChar when sv_UpdatePanel last built the panel */
int sv_ShownOther = 0;  /* 0x6f23d4, sv_OtherSelected when sv_UpdatePanel last built the panel */
int sv_ShownCount = 0;  /* 0x6f23d8, ai_SelectedCount when sv_UpdatePanel last built the panel */
int sv_PointerSpriteFor = 0;  /* 0x6f23dc, reset with the pointer sprite (-1 / 0 when sv_HoverPick frees d2_PointerSprite; reasoned: what the sprite was made for) */
char * sv_LastPicked = 0;  /* 0x6f23e0, ai_PickedObject at the last click (sv_HandleObjects; nothing reads it) */
char *sv_PermsBuilding = 0;  /* 0x6f23e4, the building ai_BuildingPerms was last computed for (the right-click handler 0x52d940) */
unsigned short *sv_PermsRoom = 0;  /* 0x6f23e8, the room (sv_PanelShown) it was computed for */
int sv_DisabledCount = 0;  /* 0x6f23ec, the entries of sv_DisabledWidgets */
unsigned char sv_BookPhase = 0;  /* 0x6f23f0, toggled 0 / 1 each frame by the open book's update: which page of the shown pair is redrawn this frame */
unsigned char ai_GameState = 0;  /* 0x6f25ac */
/* the two flags after ai_GameState are objects of their own (the original reads each as a byte: no views of it) */
unsigned char main_GameEnded = 0;  /* 0x6f25ad, nonzero when the player has reached the game's goal: the round start plays the fanfare and the outro (cleared at the game start) */
unsigned char sv_MenuOpen = 0;  /* 0x6f25ae, 1 while a menu screen runs */
int ai_OptionScrollSpeed = 0;  /* 0x6f3614 */
int sv_PanelFlag = 0;  /* 0x75e8c0, 1 when the panel is up (an int; cm.h / f3init.c) */
char * sv_PanelObject = 0;  /* 0x75e8c4, the object the info panel shows */
unsigned short * sv_PanelRecord = 0;  /* 0x75e8c8, its scene record */
char sv_PanelText[76] = {0};  /* 0x75e8d4, the panel text */
int sv_SelectionFlags[0xc0] = {0};  /* 0x75e920, the mirror of the character selection flags (byte i per character) */
int sv_SkyScene = 0;  /* 0x75ec20 */
unsigned short * sv_SelectedItem = 0;  /* 0x75ec5c */
SVACTIVITY sv_Activities[0x30] = {0};  /* 0x75ec80, the character activities the he.c scripts register (sv_RegisterActivity) */
int ai_Unk75f280[576] = {0};  /* 0x75f280, the member panel: */
float sv_ViewPos[4] = {0};  /* 0x75fb80, the stored camera position (a padded vector): set by the camera moves, restored when the player leaves a building */
float sv_ViewRot[4] = {0};  /* 0x75fb90, its rotation (D3OBJECT.rotation) */
float sv_SkyScale = 0;  /* 0x760420 */
int sv_BandActive = 0;  /* 0x760440 */
SVBAND sv_Band = {0};  /* 0x760444, the rubber band's corners */
int sv_ClickedBuilding = 0;  /* 0x760458, sv_HoverBuilding at the last click (sv_HandleObjects) */
int sv_ClickedItem = 0;  /* 0x76045c, sv_HoverItem at the last click */
int sv_ClickedChar = 0;  /* 0x760460, ai_PickedChar at the last click */
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
SVBUBBLE sv_Bubbles[0x40] = {0};  /* 0x761fc0, the 64 speech bubble records */
/* sv_BubbleObjects: view of sv_Bubbles (+0x8) */
int sv_SelectedChar = 0;  /* 0x7662c0 */
int sv_GroundplanButtons[15] = {0};  /* 0x7662e0, 15 widget ids */
int sv_SelectedExtra = 0;  /* 0x76631c, a fourth selection word, cleared with sv_SelectedItem / sv_SelectedObject / sv_SelectedModel (its only use; name and int type are a guess) */
int cm_Bombs[0x40] = {0};  /* 0x935c00, cm's 32 bomb records (object, time), cm.h */
int d2_HitWindow = 0;  /* 0xf8aad0 */
int d2_DirtyRectLeft[4] = {0};  /* 0xfb9060, left, top, right, bottom (d2_DirtyRectTop .. Bottom are its elements) */
