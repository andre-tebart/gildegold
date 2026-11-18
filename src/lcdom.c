/* lcdom -- the Domizil table and the windows after it (0x5a9e90 .. 0x5bd060), split from lc; the name is ours. The
   Domizil goods lookups (ai_RandomDomizilGoodNotIn, ai_DomizilGoodInRange) over ai_DomizilTable, the bonus amount
   asks, the personnel / mercenary / helper books, the transport, cart, storage and offer windows, the inventory
   child window, the thieves' guild equip and training windows and the player stats panel (0x5bcc70).
   Boundary: lc's .data strings end with 0x5a9e40's "_NACHRICHTEN_DOMIZIL_12" 0x62c400; this object's plain .data
   block starts with ai_ChildWindowId 0x62c418 and ai_DomizilTable 0x62c420 (first users 0x5a9e93 / 0x5a9ecb in
   0x5a9e90), its .bss with ai_InvChar 0x6f2f60 (after lc's ai_NewsSecond), its .rdata with the named
   gm_SpecialItemGroups 0x5eed94 and its count 0x5eedd4 after lc's literals 52.0f / 0.435f / 0.48f (named constants
   come first in an object), then its literal 128.0 (users 0x5aa47d ..).
   Open: the head declarations are lc.c's as the split copied them (types, views, prototypes); trim them to what
   the unit uses once lc.h / lcdom.h carry the shared ones. */
#define GAPDATA __declspec(align(1)) unsigned char

#define sv_ReturnZeroV ((int (*)(int))sv_ReturnZero)  /* @original 0x5c4100 passes the research class as an argument (sv.h declares the function void) */
#define ai_MapPickCb(cb) ((void (*)(void *, void *))(cb))  /* @original: ai_MapPick's cb is void (*)() (lc.h, its callbacks take differing pointer types); both indirect calls push two dwords and read no result (0x5b22f6/0x5b22f7 push ebp twice; 0x5b248f/0x5b2490 push the picked building and picked) */
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original the plague messages (ai_PlagueNews, ai_PlagueMessage, ai_PlagueMessageAll) read the 16-bit result (mov si,ax / and esi,0xffff), as f3gm.c's gm_Shuffled */
struct SBSAMPLE;
struct D3SM;
#include "txt_views.h"
#include <windows.h>
#include <d3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lc.h"
#include "lcdom.h"
#include "di_input.h"
#include "evt.h"
#include "d3.h"
#include "d3_light.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
#define gypsyFilterWords (*(int (*)[2])&gypsyFilter)  /* 0x5eed68, the filter's two 32-bit words */
extern GAPDATA full_data_62b038[];
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
#include "d2_types.h"


extern struct D2OBJ *d2_Objs;
extern D2VIEW d2_Views[];
extern D2WIN d2_Wins[];
extern char * cm_FieldObject;
extern struct D2PIC *d2_Pics;
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern unsigned char kb_Last;
extern char * ai_BTypes;
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern char * sv_SelectedObject;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern int d3t_BoxCornerIdx;  /* 0x5f3418 (d3_tools.c) */
extern int init_TerrainDivide;  /* 0x6f1c2c (f3init.h) */
extern int gm_TreeNodeCount;



extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

















extern int d2_ClickedId;




extern int d2_ClickedId;














extern char * ai_BTypes;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;


extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;






extern int ai_SabotageCallback();
extern int ai_BeatingCallback();



extern unsigned char kb_Last;



extern int d2_ClickedId;
extern int d2_LastWindow;


extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern int ai_EspionageAmountCB();







extern GAPDATA full_data_6f2450[];
extern unsigned char kb_Last;
extern int d2_LastWindow;
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern GAPDATA full_data_602568[];
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;



extern GAPDATA full_data_6f2450[];
#define ai_DebugHotkeys (*(int *)&full_data_6f2450)  /* 0x6f2450, a debug switch (no writer in the exe): set, the '-' key opens the mistress window in the study (ai_StudyContact) and the enter key posts the tavern message 0x74 (ai_TavernContact) */
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern D2VIEW d2_Views[];
extern D2WIN d2_Wins[];
extern char * ai_PickedObject;

extern struct AIBLD *ai_MarketBuilding;
extern int ai_PermsDirty;
extern char * sv_SelectedObject;

extern char ai_PlayerSlots[4][0x2f4];









extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;


extern int d2_LastWindow;
extern struct D2PIC *d2_Pics;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern unsigned char ai_OwnPlayer;

extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;

extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern char ai_PlayerSlots[4][0x2f4];

extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;



extern int d2_ClickedId;
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;


#include "ai_types.h"
extern int d2_ClickedId;
extern int d2_LastWindow;



extern char * ai_BTypes;

extern int d2_CutsceneActive;
extern int snd_RoomBank;
extern char * ai_BTypes;
#include "menu_types.h"
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;




extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;



extern int gm_PlayerRecords[688];
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;









extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;




extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;




extern char ai_Chars[768][0x21c];

extern char ai_Chars[768][0x21c];


extern char ai_Chars[768][0x21c];

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern char ai_Chars[768][0x21c];


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_602568[];





extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern int gm_PlayerRecords[688];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;


extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;

extern char * ai_BTypes;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern int ai_SceneChars[32];


extern int ai_SceneChars[32];
extern int d2_ClickedId;










extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;


extern char * cm_FieldObject;
extern int d2_ClickedId;


extern char ai_Chars[768][0x21c];
extern char * cm_FieldObject;
extern GAPDATA full_data_627978[];




extern int ai_CityWatchWindow();

extern int ai_CityWatchCrewWindow();

extern int ai_CityWatchWindow();
extern int ai_WatchPlanWindow();
extern int d2_ClickedId;

extern int ai_CityWatchWindow();
extern int ai_CityWatchCrewWindow();
extern int d2_ClickedId;

extern int ai_CityWatchCrewWindow();
extern int ai_WatchPlanWindow();
extern int ai_SceneChars[32];
extern int d2_ClickedId;

extern int ai_WatchPlanWindow();
extern int ai_SceneChars[32];
extern int d2_ClickedId;














extern char ai_Chars[768][0x21c];
extern int ai_MapSize;
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern char cm_EventNames[115][0x20];
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern D2WIN d2_Wins[];
extern struct D2SURF *rt_MapSurface;
extern short d2_MouseX;
extern short d2_MouseY;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern int ai_MapScrollEnabled;
extern D2VIEW d2_Views[];
#include "am_types.h"
extern char ch_EmptyName[1];



extern char ch_EmptyName[1];
extern int ai_DomizilCount;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;

extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern char * ai_BTypes;
extern GAPDATA full_data_60d568[];

extern char ch_EmptyName[1];
extern struct D2OBJ *d2_Objs;

extern struct AIBLD *ai_MarketBuilding;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_602568[];
extern struct AIBLD *ai_MarketBuilding;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;
extern int sv_FrameMask;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
#pragma pack(push, 1)
typedef struct G8INVOBJ {               /* an object of a character's inventory (AIOBJ, ai_types.h, read as a held good:
                                           ai_InventoryHas, ai_BuildingHasItem, the inventory window, the Domizil
                                           goods' clock lc_UpdateGoodsReserves); AIOBJ's union does not reach +0x2d */
    unsigned short id;                  /* +0x00, the object's type id (AIOBJ.id) */
    int objId;                          /* +0x02, AIOBJ.objId */
    int ownerId;                        /* +0x06, the id of the character holding it (AIOBJ.ownerId; the Domizil clock
                                           sells the used-up good from it) */
    char pad0a[4];
    int count;                          /* +0x0e, how many (AIOBJ.count; the inventory window's amount control) */
    char pad12[0xa];
    int minutesLeft;                    /* +0x1c, of a Domizil good in use: the minutes its effect still runs
                                           (lc_UpdateGoodsReserves counts it down from G9DOMIZIL.minutes) */
    char inUse;                         /* +0x20, of a Domizil good: set to 1 by ai_UseGood with minutesLeft (the name is
                                           a reasoned guess) */
    int active;                         /* +0x21, nonzero when the object counts as held, of a Domizil good while its
                                           effect runs (ai_InventoryHas tests it; lc_UpdateGoodsReserves clears it
                                           when minutesLeft runs out; the name is a reasoned guess) */
    int bldId;                          /* +0x25, the id of the building the object stands in (reasoned guess:
                                           ai_BuildingHasItem compares it with AIBLD.id) */
    char pad29[4];
    AITIME since;                       /* +0x2d, of a Domizil good in use: when minutesLeft was last counted down */
} G8INVOBJ;
#pragma pack(pop)
#define d2_OpenFormS ((int (*)(short, short, char *))d2_OpenForm)  /* @original: the transport window computes x = d2_ScreenW - 0x68 in 16 bits (0x5af336 mov ax, [d2_ScreenW]; sub ax, 0x68; push eax), as sv_OpenPlayerBar */
extern char * ai_BTypes;
extern GAPDATA full_data_60c28c[];
extern int ai_MapSize;
extern char ai_PlayerSlots[4][0x2f4];
extern struct D2SURF *rt_MapSurface;
extern char ch_EmptyName[1];
extern int ai_SabotageCallback();
extern int ai_EspionageAmountCB();
extern int ai_BeatingCallback();
extern char * ai_PickedObject;
extern char * cm_FieldObject;
extern struct D2OBJ *d2_Objs;



extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern char ai_Chars[768][0x21c];
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_60d224[];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern int d2_ClickedId;

extern int d2_ClickedId;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;


extern int d2_ClickedId;
extern GAPDATA full_data_625380[];
extern GAPDATA full_data_62bdc8[];
extern GAPDATA full_data_62bde4[];
extern GAPDATA full_data_62be00[];
extern char * ai_BTypes;
extern unsigned char ai_OwnPlayer;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern struct D2OBJ *d2_Objs;
extern char * cm_FieldObject;


extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern signed char ai_Relations[768][768];
#include "cm_types.h"
extern G17MSG cm_SendRing[];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern int sv_FrameMask;
extern unsigned char ai_OwnPlayer;
extern unsigned gm_OwnNameHeads[136];
#include "gv_types.h"
#include "ai_bld.h"                /* AIBLD, AIBTYPE */
extern GAPDATA full_data_5fe7d0[];
extern char ai_PlayerSlots[4][0x2f4];

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern unsigned char ai_OwnPlayer;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern char ai_Chars[768][0x21c];
extern GAPDATA full_data_624204[];
extern int gm_PlayerRecords[688];
extern char cm_EventNames[115][0x20];
extern unsigned char ai_OwnPlayer;
extern char * ai_BTypes;
extern char ai_Crimes[];

extern int d2_ClickedId;

extern int d2_ClickedId;

extern char ch_EmptyName[1];
extern int d2_ClickedId;

extern char ch_EmptyName[1];
extern int d2_ClickedId;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern struct D2PIC *d2_Pics;
extern int sv_FrameMask;
extern char ai_Chars[768][0x21c];
extern GAPDATA full_data_60d224[];
extern unsigned char ai_OwnPlayer;
extern char * cm_FieldObject;
extern signed char ai_Relations[768][768];

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf */

struct AMTREC;  /* file-scope tag for the prototypes below */

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

struct AITIME;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */


#define U_I(a) (*(int *)(a))
#define U_W(a) (*(unsigned short *)(a))

#pragma pack(push, 1)
typedef struct AITASKVIEW {       /* the character's model record fields this unit reads (ai_char.h AITASK) */
    char pad000[0x2c];
    int bldId;                     /* 0x2c, the building the character was sent to (ai_char.h AITASK.bldId) */
    int scriptGroup;               /* 0x30, the group of the script that created / moved it (the room object's id) */
} AITASKVIEW;

typedef struct LCWORKSET {       /* the work settings a building keeps at AIBLD +0x99 (ai_bld.h AIBLD.workSet) and a
                                      hired worker takes over into AICHAR.flags1b4 .. target (ai_char.h; cm_ExHire
                                      copies them as 16 bytes); the master letter window passes them by value */
    unsigned int flags;            /* +0x0, the order flags (bit 0 .. 3: the page's four checkboxes) */
    int budget;                    /* +0x4, AICHAR.budget */
    int prod;                      /* +0x8, AICHAR.prod (the item the workshop makes) */
    int target;                    /* +0xc, AICHAR.target (a building id or -1) */
} LCWORKSET;
typedef struct AICHARREC {         /* the character record fields this unit reads (ai_char.h AICHAR, same layout;
                                      G8CHARREC below is the view of the news messages) */
    unsigned short id;             /* 0x000, the character's id (AIBLD.owner refers to it) */
    unsigned char kind;            /* 0x002, the character kind (ai_char.h AICHAR.kind; 6 / 7 need no cell model) */
    char pad003;
    unsigned int netId;            /* 0x004, the character's network id (ai_char.h AICHAR.netId) */
    unsigned char health;          /* 0x008 */
    unsigned char sex;             /* 0x009, 0 / non-0 (ai_char.h AICHAR.sex; it picks the male / female name table) */
    unsigned short w00a;           /* 0x00a, the age in years (ai_char.h AICHAR.w00a, gm_CharAge; the family tree
                                      window lets a child above 10 be picked) */
    unsigned char faith;           /* 0x00c, the faith, 0 / 1 (ai_char.h AICHAR.b00c): the baptism window sets it to
                                      1 - it (or to the church owner's) and names it by the text 0x197a + it; the
                                      church donation reaches the characters of the owner's faith */
    unsigned char b00d;            /* 0x00d, 1 .. 2 (ai_char.h AICHAR.b00d, gv.c copies it to the grave's v30) */
    char pad00e[0xe];
    float f01c;                    /* 0x01c, the life value (ai_char.h AICHAR.f01c: 1000.0 at creation, aged
                                      by gm_AgeCharacter, the character dies when it is used up) */
    float f020;                    /* 0x020, ai_char.h AICHAR.f020 (the character info panel offers a jobless child
                                      the profession choice when it is at most the age + 4) */
    int v024;                      /* 0x024, ai_char.h AICHAR.v024: below 0 the character is hurt (ord.c's medicus
                                      order tests it below -20); the medicus heals half of it (gm_AddHealth) */
    char pad028[4];
    unsigned int ailments;         /* 0x02c, the ailment counters (am.h G5CH.ailments; ai_char.h AICHAR.v02c):
                                      the fields at bits 0, 4, 8, 12, 14, 17, 20, 23, 25 (4, 4, 4, 2, 3, 3, 3, 2, 4
                                      bits), the daily step 0x4fbe90 halves each, the diseases 2 .. 9 sit in the
                                      field pairs of gm_CatchMinorDisease / gm_CatchUpperDisease */
    char name[0x20];               /* 0x030, the character's name (ai_char.h AICHAR.name) */
    unsigned short w050;           /* 0x050, the family's grave word (ai_char.h AICHAR.w050); the family tab takes the
                                      characters with the same w050 and b058 */
    char pad052[2];
    int ref54;                     /* 0x054, a reference word the related characters share (ai_char.h AICHAR.ref54;
                                      the game frame compares it with the player's); a picture id, 0x59a + the
                                      dynasty's colour 0 .. 7 (ai_PlayerComparePanel shows it and colours the
                                      slider by it) */
    unsigned char b058;            /* 0x058, compared with w050 by the family tab (ai_FillFamilyTab) */
    char pad059[3];
    unsigned int relatives[8];     /* 0x05c, the netIds of the character's relatives (ai_char.h AICHAR.relatives;
                                      the baptism window converts [3..7] with the player) */
    char pad07c[4];
    unsigned char skills[5];       /* 0x080, the five skill values (ai_char.h AICHAR.skills; the personnel book shows
                                      [3] and [2] as bars) */
    char pad085[0xdf];
    unsigned char jobId;           /* 0x164, the character's job (ai_char.h AICHAR.jobId; the construction window's group 7
                                      needs 0x34 .. 0x39) */
    unsigned char title;           /* 0x165, the title rank (ai_char.h AICHAR.title; text 0x20d / 0x230 + it) */
    unsigned char officeKind;      /* 0x166, the office the character holds, 0 = none (ai_char.h AICHAR.officeKind) */
    char pad167;
    unsigned char appliedOffice;   /* 0x168, the office kind applied for, 0 = none (ai_char.h AICHAR.appliedOffice) */
    unsigned char secondOffice;    /* 0x169, the second office kind, the deputy one (ai_char.h AICHAR.secondOffice) */
    char pad16a[2];
    struct AIBLD *bld;             /* 0x16c, the building the character is in (ai_char.h AICHAR.bld) */
    int bld2;                      /* 0x170, the character's own building, 0 when none (ai_char.h AICHAR.bld2) */
    int v174;                      /* 0x174, 0 when hired (ai_char.h AICHAR.v174); the personnel book shows it with
                                      the texts 0x1a39 / 0x1a3a */
    int inventory;                 /* 0x178, the character's goods inventory, an object list (ai_char.h AICHAR.inventory) */
    struct AIJOB *job;             /* 0x17c, the job the character works in, 0 = none (ai_char.h AICHAR.job) */
    char pad180[4];
    struct AITASKVIEW *model;      /* 0x184, the character's model record, 0 when none (ai_char.h AICHAR.model) */
    unsigned char selected;        /* 0x188, nonzero: in the player's selection (ai_char.h AICHAR.selected) */
    char pad189[3];
    int picture;                   /* 0x18c, the portrait picture (ai_char.h AICHAR.picture) */
    char pad190[4];
    int money;                     /* 0x194, the character's money (ai_char.h AICHAR.money) */
    char pad198[0x14];
    int donated;                   /* 0x1ac, the money the character gave the church: ai_IndulgenceWindow adds the price
                                      (cm_AddFieldValues 0x1ac) and shows it with the text 0x1964 */
    char pad1b0;
    unsigned char heldRounds;      /* 0x1b1, the rounds the character is held (ai_char.h AICHAR.heldRounds) */
    char pad1b2[2];
    union {
        LCWORKSET workSet;         /* 0x1b4, the work settings the character took over from its building (ai_char.h
                                      AICHAR.flags1b4 .. target; the master letter window edits them) */
        struct {
            unsigned int flags1b4; /* 0x1b4, the order flags (ai_char.h AICHAR.flags1b4) */
            int budget;            /* 0x1b8, the money the character may spend (ai_char.h AICHAR.budget; main_EndRound
                                      copies the building's budget to its jobs; the master list page shows it) */
        };
    };
    char pad1c4;
    unsigned char b1c5;            /* 0x1c5, ai_char.h AICHAR.b1c5 (a worker value am.c averages per building; the
                                      Domizil good 0x161 halves it, rounded up, for everyone in the building) */
    char pad1c6[2];
    unsigned int powerFlags;       /* 0x1c8, the office-power bits already used (ai_char.h AICHAR.powerFlags) */
    unsigned int flags1cc;         /* 0x1cc, ai_char.h AICHAR.flags1cc (0x800: a bribe raises the relation by 6 instead of 4) */
    char pad1d0[0x1c];
    unsigned int goodsMask;        /* 0x1ec, the classes of the Domizil goods in use (G9DOMIZIL.mask; ai_UseGood sets them) */
    int w1f0;                      /* 0x1f0, ai_char.h AICHAR.v1f0 (0 at creation; the partner filter bit 0x80 wants it set) */
    char pad1f4[0x1c];
    int partner;                   /* 0x210, the netId of the partner (ai_char.h AICHAR.partner; the amount rows mark
                                      a kind 6 viewer's partner) */
    char pad214[8];
} AICHARREC;                       /* 0x21c bytes, ai_Chars[768] */
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct LCFIREEVENT {       /* the fire event 0x3d's word over AIJOB's union (its alloc 0x564860; the jail check
                                      0x591ad0 reads the burning building here) */
    char pad00[0xac];
    int bld;                       /* 0xac, the building that burns, -1 / 0: none */
} LCFIREEVENT;
typedef struct LCAMBUSHEVENT {     /* the ambush event 0x48's word over AIJOB's union (he.c HEAMBUSH; ai_BreakInCallback
                                      looks for one at a place) */
    char pad00[0xac];
    char place[0x20];              /* 0xac, the name of the scene object the gang lies in wait at */
} LCAMBUSHEVENT;
typedef struct LCSEARCHEVENT {     /* the search event 0x14's words over AIJOB's union (ai_TradeSearchWindow counts them) */
    char pad00[0xac];
    unsigned char count;           /* 0xac, how many characters search */
    char padad;
    unsigned short good;           /* 0xae, the good searched for */
} LCSEARCHEVENT;
typedef struct LCSEARCHWORDS {     /* the search message 0x14's words at AIMSG +0x58 (msg.arg) */
    unsigned char count;           /* 0x58, how many of ids[] are set */
    char pad59;
    unsigned short good;           /* 0x5a, the good searched for */
    int room;                      /* 0x5c, the objId of the room the search starts from */
} LCSEARCHWORDS;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct AIROOM {            /* the room object sv_PanelShown / the driver's room parameter points to (ai_types.h AIOBJ) */
    unsigned short id;             /* 0x00, the room (scene record) id */
    int objId;                     /* 0x02, the object's own id (AIOBJ.objId) */
    char pad006[0xe];
    int list;                      /* 0x14, its object list (ai_types.h AIOBJ.list; ai_StorageWindow / gm_SumListAmounts) */
} AIROOM;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct G8USEVIEW {              /* a local view of am.h's G8USE (am.h cannot be included here yet): the
                                           record ai_UseGood hands to a Domizil record's use callback (+0x14) */
    G8INVOBJ *obj;                      /* 0x00, the inventory object used (G8USE.obj; its +0 id) */
    int target;                         /* 0x04, the target reference (G8USE.target) */
    int arg;                            /* 0x08 */
    int effect;                         /* 0x0c, 1 the use took effect, 0 not, -1 backfired (G8USE.effect) */
    int handled;                        /* 0x10, set by a callback that reported the use itself (G8USE.handled) */
} G8USEVIEW;
#pragma pack(pop)


#include "ls.h"
#include "f3init.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "d3s.h"
#include "err.h"
#include "f3gm.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "sv_types.h"
#include "amt_types.h"
#include "ch.h"
#include "hmt_types.h"
#include "msx.h"
#include "rd.h"
#include "am.h"
#include "amt.h"
#include "gv.h"
#include "hst.h"
#include "hmt.h"
#include "mission.h"
#include "ai.h"
#include "anm.h"
#include "chc.h"
#include "d3_int_io.h"
#include "d3_tools.h"
#include "f3snd.h"
#include "vfs.h"
#include "d2.h"
#include "rt.h"
#include "gfx.h"
#include "cm.h"
#include "ahm.h"
#include "menu.h"
#include "sv.h"
#include "sv_quickjump.h"
#define d2_CreatePicObjS ((int (*)(short, short, int))d2_CreatePicObj)  /* 0x46cd80 (d2.h: int x, y): @original the town map update adds the window position to the mark pixel in 16 bits (0x5a8052 mov dx, word ptr [eax + 4] / add dx, di / push edx), as d2.c's callers */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original the checkbox / route-box calls push only the object and the value (0x5c81ab ai_CityStatsPanel: push esi; push edi; 0x5b0a1b ai_TransportWindow: push 0; push esi) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original the range calls push four (0x5c2610 ai_ShowWorkSettingsPage: push *budget; push 0x2328; push 0x3e8; push eax; 0x5c2675: push eax; push 0x96; push 0x32; push esi) */

/* dcp unit split: the declarations of lc before the moved functions */
struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); int d2_OpenForm(); void d2_CentreViewWindows(int view);
float gm_GoodsPrice(unsigned id, unsigned char p);   /* 0x5089c0, the price (ai.h / cm.h / rt.h: float) */
void gm_FormatAmount(int v, unsigned char k, char *out);  /* 0x508a80, above its first call (line 1119) */
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...);
int d2_GetViewWin(); void d2_SetPointerSprite(int a, int pic); char * gm_Query(void *list, int n, ...);
int gm_FreeCapacity(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int ai_ScriptQuery(); int cm_SendLongMessage(char *msg); int cm_GetSlotState(); int cm_GetSlotValue(int h);
void snd_PlayFavourComment(void); void d2_CloseView(int view);
int gm_ListAmount(); char * gm_Query(void *list, int n, ...);
void ai_LayoutProdGoodsList(int param_1, int param_2, int param_3, int param_4); char *itoa(int, char *, int); void d2_SetObjText();
int gm_BuildingGoodsList(char *b, unsigned *out, int kind); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win); struct AIPER; struct D3VECTOR; int d2_WinOpenPic(short x, short y, int pic, int win); int d2_WinOpenPicStyled();
void d2_RemoveObj(); int gm_MinutesBetween(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...);
int gm_BuildingClass(); int d2_WinOpenPicStyled(); void d2_SetObjFont(); void d2_SetCaption();
int gm_ClockSeason(); int ls_GetLanguage(); int d2_CountPointerItems(); void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldValues(void); void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld); void d2_MoveObj(short x, short y, int obj);
void d2_SetPointerItem(int id, int amount); void d2_ClearPointerItems(void);
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);  /* 0x46a960 (d2.h) */
int d2_RemoveWindow(int win);                                   /* 0x472ce0 (d2.h) */
char * gm_FindGoods(int p, unsigned id);
struct D3OBJECT;
struct D3OBJECT * d3_FindObject(int a, int flags, int c, char *name); /* 0x449690 (d3_interface.c, d3.h) */
int sv_RegisterNamePlate(char *name, char *text, int b);         /* 0x52e1c0 (sv.c) */
int ai_OpenTransport(int rec);                                   /* 0x5af1a0, below */
char *strstr(); unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void);
struct D3OBJECT;
struct D3VECTOR;
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out); /* 0x458a20 (d3_pose.c) */
unsigned char *gm_BuildQuery(int n, ...);                  /* 0x501190 (f3gm.c) */
unsigned char *gm_BuildNext(void);                         /* 0x501030 (f3gm.c) */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); /* 0x453e80 (d3_vec.c) */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
#pragma pack(push, 1)
typedef struct AIOBJVIEW {         /* the object fields the vg_ / build-site callbacks read (d3.h D3OBJECT, same layout) */
    char pad0[0x1f0];
    struct AIOBJVIEW *next;        /* 0x1f0, the sibling list */
    struct AIOBJVIEW *prev;        /* 0x1f4 */
    struct AIOBJVIEW *parent;      /* 0x1f8, the parent object (the owner of a building site) */
    struct AIOBJVIEW *child;       /* 0x1fc, the first child (the "bk_" site markers live here) */
    char pad200[0x20];
    int kind;                      /* 0x220, 1 = per-vertex lit */
} AIOBJVIEW;
#pragma pack(pop)
char d3if_ShowTree(struct D3OBJECT *obj, char show);
char d3_ForEachObject(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, char (*cb)(struct D3OBJECT *, void *), void *ctx);  /* 0x438d40 (d3.h) */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                /* 0x447300 (d3_interface.c) */
char d3if_SelectScene(int index, char keep);  /* 0x4478e0 (d3_interface.c) */
int d3t_DropObject(); 
struct XOBJ;
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);  /* 0x458c10 (d3_pose.c) */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);    /* 0x458810 (d3_pose.c) */
float d3_VecAngle(struct D3VECTOR *a, struct D3VECTOR *b);      /* 0x4539f0 (d3_vec.c) */
extern const struct D3VECTOR4 d3_ZAxis;                          /* 0x5ebb74, (0, 0, 1) */
unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void); int gm_MayTakeJob(int k, int c);
char *strupr(char *); int sprintf(char *buf, const char *fmt, ...); char * vfs_GetPath();
struct D3OBJECT * d3_LoadGroup(); void d3t_InflateObjectTree(D3OBJECT *obj); int d3t_DropObject();
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
int gm_BuildingKindCost(int c, int kind); void txt_Format(char *out, int id0, ...);
char d3sm_GetVertex();  /* 0x45f100: this caller tests the char result (al) */
char d3sm_Pick();  /* 0x45f2b0, d3_sm.h: the cell under a screen point, 0 = none */
void sv_SetStatusText(char *s);  /* 0x52e330, sv.h */
extern char ai_DataPath[];  /* 0x75dca0, the data path (am.h) */
int ai_RunAllows(); int d2_OpenForm();
int d2_SetForm(int view, int n); int d2_GetViewWin(); int io_PrintText(char *fmt, ...);
int d2_WinOpenPic(short x, short y, int pic, int win); void d2_SetObjFont(); void d2_SetObjText();  /* d2_CreateWinText: d2.h's prototype; ai_ShowProdSlot's 16-bit calls go through d2_CreateWinTextS */
void d2_SetViewEnabled(); void d2_CloseView(int view); int cm_SendLongMessage(char *msg);
void txt_Format(char *out, int id0, ...);
int gm_CharCurrency(char *c, char p); int gm_BuildingClass(); int gm_MayTakeJob(int k, int c);
#define sv_CheckStockB ((int (*)(AIROOM *, int, int, unsigned char))sv_CheckStock)  /* @original ai_ConstructionWindow pushes the money kind as a byte (only cl loaded) */
#define sv_CheckGoodAmountB ((int (*)(char *, int, unsigned char))sv_CheckGoodAmount)  /* @original as sv_CheckStockB */
int gm_IsHomeType();
unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c); void init_ShowBuildingScene(unsigned char *b, int slot); void init_FinishBuildingScene(unsigned char *b, unsigned short *o);
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags); int gm_ClockSeason(); void init_AfterTownLoad(void); void d3fl_MakeDivide();
void ai_ChooseCoatOfArms(void); void snd_StartMarketAmbience(void); int tb_SetFunctionOff(); void snd_StopMarketAmbience(void);
void d2_CopyCaption(char *caption);
struct AIBLD; int ai_ScriptQuery();
int gm_IsType10(unsigned char *b);
int d2_GetWinObj(); void ai_TechEffectsWindow(AIBLD *bld);
float gm_ItemPrice(int item, unsigned char n);
void cm_BeginGroup(const char *s); void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); short * gm_PlayerRecord(char *c);
int cm_SendLongMessage(char *msg); int cm_EndGroup(const char *s); int cm_GetSlotState(); void cm_SendMoneyDelta(int a, int b);
char *strupr(char *); void gm_CloseTechTree(void);
void ai_MasterLetterWindow();
void ai_GameFrame(void);
void lc_PlayerInJail(void);
typedef struct SVFRAME {                /* 8 bytes, an entry of the frame stack 0x75f780 (sv.c sv_RunFrame: "MASTER()") */
    int cb;                             /* 0x00, the frame callback registered at this depth */
    char pad04[4];
} SVFRAME;
typedef struct LCTREENODE {        /* a node of f3gm's research tree layout gm_TreeNodes (f3gm.c RTNODE, 0x34 bytes) */
    char pad00[0x24];
    int obj;                       /* 0x24, the node's button object (RTNODE.l[2]), -1 = none */
    char pad28[0xc];
} LCTREENODE;
extern char gm_TreeNodes[3328];    /* 0x76fde0, LCTREENODE[64] (f3gm.c) */
extern int ai_Difficulty;          /* 0x6f23fc, the game difficulty 0 .. 4 (ai.h) */
int gm_BuildKindObjects(char *b);
char *itoa(int, char *, int);
int ai_SearchResourceWindow(AIBLD *bld);
int ai_ExtractResourceWindow(AIBLD *bld);
void ob_HandleBepflanzung(AIBLD *bld);
void ai_HandlePickedPlant(AIBLD *bld, int plants);
int he_NoProtectionEvent();
unsigned short * gm_QueryNext(void); void txt_Format(char *out, int id0, ...);
void gm_SendMoneyDelta(int a, int b);
int ai_RunAllowsContact();
void d2_SetObjEnabled(); void d2_SetCaption(); int d2_WinOpenPic(short x, short y, int pic, int win);
int ai_AmountMenu(); void ai_TalentWindow(struct AIBLD *bld, int room, int talent);
void ai_PamphletWindow(int bld);
void ai_FeastWindow(AIBLD *bld, AIOBJ *table); void ai_DenounceWindow(AIBLD *bld);
short * gm_FindChar(int id);
#define cm_SendExSetHEI ((int (*)(int, char *))cm_SendExSetHE)  /* @original: the callers here push the key as an int and read the return (byte-matched ai_TransportOrderDue), against cm.h's (unsigned char, char *) prototype */
int cm_SendRelations(int a, int b, int c, int d); void cm_SendMoneyDelta(int a, int b); void d2_SetObjEnabled();
typedef struct LCAMOUNTROW {            /* 0x38 bytes, one row of the special-amount list (ai_AmountRowStatus /
                                           ai_AmountRow; cm.c CMAMOUNTROW) */
    AICHARREC *rec;                     /* 0x00, the character the row shows */
    int ctl[2];                         /* 0x04, the row's controls (ai_AmountRow): the frame picture, the liking slider */
    int office;                         /* 0x0c, of a vacant seat: its office, the text 0x253 + it (ai_ShowOfficeRows) */
    int x;                              /* 0x10 */
    int y;                              /* 0x14 */
    int portrait;                       /* 0x18, the portrait picture (ai_ShowAmountRows) */
    int icons[4];                       /* 0x1c, the icons in the frame's corners (ai_ShowAmountRows) */
    int x2c;                            /* 0x2c, a value the row's text shows (the evidence book: the evidence count) */
    int pickable;                       /* 0x30, nonzero: the row's frame takes clicks (ai_AmountRow) */
    int x34;                            /* 0x34, a second value (the evidence book: the open evidence) */
} LCAMOUNTROW;
int gm_TitlesLeft(); float gm_TitlePeriodServed(int n, int *out); int d2_WinOpenSlider();
int gm_TitleGroup(); void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);
int gm_HasAdultRelative(); float gm_AddHealth(unsigned short id, int d); int ai_TutorialOff(); void ai_EvidenceBookWindow(struct AIBLD *bld);
void d2_CreateScrollButtons(); int d2_RemoveWindowObjects();
typedef struct LCDUELSIDE {             /* one side of a dice duel: am.h AGDUEL's side1 (+0x0c) or side2 (+0x30) block
                                           (am.h cannot be included here yet) */
    AICHARREC *ch;                      /* 0x00, the character throwing (AGDUEL.side1) */
    unsigned char n;                    /* 0x04, the dice thrown (AGDUEL.n1) */
    char pad05[3];
    unsigned int stats;                 /* 0x08 (AGDUEL.stats1) */
    unsigned char t[0x10];              /* 0x0c, the thrown values 1 .. 6 (AGDUEL.t1) */
} LCDUELSIDE;
typedef struct LCDUEL {                 /* a dice duel (am.h AGDUEL, same layout; am.h cannot be included here yet) */
    int stake;                          /* 0x00, the first throw's stake (AGDUEL.stake) */
    int raiseAmount;                    /* 0x04, a tenth of the stake, paid on the throws of kind 2 / 3 (AGDUEL.raiseAmount) */
    int phase;                          /* 0x08, 5 / 6 at the end: side1 / side2 won (AGDUEL.phase) */
    LCDUELSIDE side1;                   /* 0x0c (AGDUEL.side1 .. t1) */
    int kind1;                          /* 0x28, the action side1 chose, 4 answer (AGDUEL.kind1) */
    int reserved2c;                     /* 0x2c */
    LCDUELSIDE side2;                   /* 0x30 (AGDUEL.side2 .. t2) */
    int kind2;                          /* 0x4c (AGDUEL.kind2) */
    int reserved50;                     /* 0x50 */
    int total;                          /* 0x54, the stakes paid so far (AGDUEL.total) */
} LCDUEL;
int gm_GetWorth(unsigned short id); int gm_ConvertMoney(int a, unsigned char k); short * gm_PlayerRecord(char *c); int ai_DuelStart();
int ai_DuelAct(); void ai_DuelNextPhase(); int ai_DuelOpponentAction(); int ai_DuelAcceptAction();
typedef struct LCFIN {                  /* what gm_PlayerRecord finds for a character (ai.h AIFIN; ai.h cannot be included here yet) */
    char pad00[0x28];
    int spyPaid;                        /* 0x28, AIFIN.spyPaid: the money paid to spies and bribed characters (ai_BribeAmountCB) */
    char pad2c[0x18];
    int renovationSpent;                /* 0x44, AIFIN.renovationSpent: the money paid for renovations (ai_RenovationWindow) */
    int loansRepaid;                    /* 0x48, AIFIN.loansRepaid: the loan sums paid back at a money lender (ai_RepayLoan) */
    char pad4c[4];
    int cartSpent;                      /* 0x50, AIFIN.cartSpent: the money paid for new carts (ai_BuyCartWindow) */
    char pad54[4];
    float duelBias;                     /* 0x58, AIFIN.roundFactors[0]: the bias for who starts a dice duel (am.h AGFIN.firstBias) */
    float tavernChance;                 /* 0x5c, AIFIN.roundFactors[1]: the chance of being invited to the dice game; each visit
                                           takes it to * 0.66 - 0.1 (invited) or * 0.66 - 0.02 (not invited, after 11 o'clock) */
} LCFIN;
typedef struct LCDUELBTN {              /* an action button of the dice game window */
    int obj;                            /* 0x000, its window object, -1 while the action is not allowed (ai_DuelAcceptAction) */
    int kind;                           /* 0x004, the action: 2 another die, 3 stop, 4 answer (AGDUEL.kind1) */
    int unused;                         /* 0x008, set to 0, never read */
    char text[0x100];                   /* 0x00c, its caption (0x1880 / 0x1881 with the raise amount, 0x1882) */
} LCDUELBTN;
int cm_SendTableRequest(G17TABLEMSG *p); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s); unsigned char * gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);
int gv_EvidencePrice(); void cm_SendScriptMessage(void *p); void cm_SendProofExecuted(int a, int b);
int d3t_PlaceAtObject(); void d2_SetBackgroundTexture(); 
struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask); int gm_GetGoodsValue(); int gm_SumListAmounts(void *list, unsigned char p);
int gm_PlayerFactor(int a, unsigned char k); int gm_GetCreditOffers(); void d2_EnableViewObjects();
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_FreeEvent(); void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...);
void ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
void ai_BardWindow(void); void ai_NoticeBoardWindow(void);
void cm_BeginGroup(const char *s); void cm_SendScriptMessage(void *p); void cm_SendProofExecuted(int a, int b);
int d3t_PlayAnim(); void d2_SetBackgroundTexture();
char d3if_ShowTree(struct D3OBJECT *obj, char show); int ls_GetLanguage(); struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);
unsigned char * gm_BuildNext(void); void ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
int ai_TransportWindow(); 
typedef struct CREDITROW {          /* a row of the new credit window (ai_NewCreditWindow): an offer and its object */
    ORDCREDIT offer;                /* 0x00, the bank's offer (gm_GetCreditOffers) */
    int obj;                        /* 0x0c, the row's window object (d2_GetWinObj) */
    int index;                      /* 0x10, the offer's slot in the bank's credits[] */
} CREDITROW;
short * gm_FindChar(int id); void txt_Format(char *out, int id0, ...);
short * gm_PlayerRecord(char *c); void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_FreeEvent();
void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...); void d2_SetObjText(); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
int d2_RemoveWindow(); void d2_RemoveObj(); int gm_SumSlotAmounts(void *list, unsigned char i);
void gm_SortCharsByName(int n, int *ids, int flag); void d2_AddPointerItem(int id, int amount); void d2_ClearPointerItems(void); int d2_CountPointerItems();
void d2_EndInputEdit(void); void d2_EnableObject(); void d2_ScrollWindowBy(); void d2_MoveObj(short x, short y, int obj);
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont(); void d2_CopyCaption(char *caption);
void d2_SetObjPic(); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void); void cm_BeginFieldBlock(char *o, int id);
char *itoa(int, char *, int); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s);
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k);
typedef struct CREDITLISTROW {      /* a row of the credit list window (ai_CreditListWindow) */
    int child;                      /* 0x0, the row's child window (d2_CreateChildWindow), -1: none */
    int pic;                        /* 0x4, its frame picture 0x75a (d2_WinOpenPic), -1: none; the radio group selects it */
    HELOAN *loan;                   /* 0x8, the loan it shows (gm_CreditEvents) */
} CREDITLISTROW;
int gm_SumSlotAmounts(void *list, unsigned char i); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
void d2_SetObjEnabled(); void d2_EnableViewObjects(); void d2_EnableObject();
void gm_SortCharsByName(int n, int *ids, int flag); int d2_RemoveWindow(); void d2_RemoveObj();
void d2_SetObjText(); void d2_MoveObj(short x, short y, int obj); int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont();
int gm_SumListAmounts(void *list, unsigned char p); char * he_FindEvent(int n, ...); char * he_NextEvent(void); int ai_AmountMenu();
int gm_BuildingClass(); unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);
void init_LoadObjectScene();
char d3_DeflateScene(unsigned int flags); char d3_InflateScene(unsigned int flags); void init_SetCharacterAnimations(unsigned char *b);
void init_ShowBuildingScene(unsigned char *b, int slot); void init_FinishBuildingScene(unsigned char *b, unsigned short *o); 
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);
void snd_UnloadBuildingVoices(void); void init_AfterTownLoad(void); 
struct SBSAMPLE * sb_PlaySample();
void ai_AlchemistLabContact(struct AIBLD *rec, struct AIOBJ *room); void ai_CannonContact(struct AIBLD *rec, struct AIOBJ *room); void ai_CabinetContact(struct AIBLD *rec, struct AIOBJ *room); void ai_ProductionContact(int building, int arg);
int gm_GetWorth(unsigned short id); void d2_ClearPaintbox(void);
int ai_ShowBuildingRows(AICHARREC *ch, int max, int *ctrls);  /* 0x5bd470, defined below */
int ai_ShowLoanRows(AICHARREC *ch, int max, CREDITLISTROW *rows, int win, int form);  /* 0x5bd330, defined below */
typedef struct SVSAVEDVIEW {
    D3VECTOR4 pos;
    D3VECTOR rot;
} SVSAVEDVIEW;
int init_BuildingSceneUniverse(); void init_LoadBuildingScene(int k); void d3fl_SetDetail(unsigned int n); void veg_HidePlants();
void ai_GraveyardContact(char *rec); void ai_GypsyProductionContact(int building); void ai_GraveyardFogContact(char *rec);
void ai_GraveyardSeanceContact(char *rec); void ai_GypsyContact(int building); void ai_TheatreContact(int building); void ai_CurseContact(int buildingArg);
void ai_GraveyardTradeContact(char *rec);
typedef struct LCSALEROW {          /* a row of ai_BuyBuildingWindow's list: a building offered for sale */
    int control;                    /* 0x0, the row's text control (d2_GetWinObj) */
    AIJOB *sale;                    /* 0x4, the building's sale event (kind 0x1b) */
    AIBLD *bld;                     /* 0x8, the building */
    int flags;                      /* 0xc, its flags5aw when the row was built (a change rebuilds the list) */
} LCSALEROW;
void ai_OpenAmountKindMenu(void);
void ai_OpenAgendaMenu(void);
int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); int amt_ApplyForOffice(); int amt_KindColumn();
void ai_AmountInfoWindow(); int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();
int ai_AmountLimit();
int amt_CanUseMayorPower2(); void d2_SetObjEnabled();
int amt_ExecuteShareMessage(short *c, int *m);
int amt_MayorBonusStep(); int amt_CanUseMayorPower3(); int amt_ExecutePriceMessage(char *c, int *m);
int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();
int amt_ExecuteMayorPowerMessage(char *c, int *m);
typedef struct AMTPICK {           /* the row record the special-amount list hands its callback (8 bytes): the
                                      office index and the owner (0x5bea50 compares the word with 0xffff) */
    unsigned short office;         /* 0x00, the office index, 0xffff = an empty row */
    unsigned short pad02;
    int charId;                    /* 0x04, the office owner's character id (amt_ExecuteMayorPowerMessage's m[1]) */
} AMTPICK;
char * gm_Query(void *list, int n, ...); int gm_GetWorth(unsigned short id); int gm_GetGoodsValue(); int ai_AmountLimit();
int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
int io_PrintText(char *fmt, ...); int d2_GetWinObj(); int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d); int cm_SendRelations(int a, int b, int c, int d); void txt_Format(char *out, int id0, ...);
void d2_CloseView(int view); int _ftol();
int gv_CountOwnedGraves(); int gv_CulpritGuilt(int c); int gm_GetWorth(unsigned short id); float gm_Liking();
int ai_InventoryHas(void *ch, int id); int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); 
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...); int d2_GetWinObj();
void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void);
int gv_RetireCrimesOf(int p, int n); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); void ai_ShowIndulgenceNews(); int cm_GetSlotState();
void txt_Format(char *out, int id0, ...);
void d2_CloseView(int view); int _ftol();
void ai_Top5Window();
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); short * gm_FindChar(int id); int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);
void gm_DonationTop5(unsigned *ids, int *sc); int ai_DrawAmountRow();
int d2_RemoveWindowObjects();
void cm_SendMoneyDelta(int a, int b); void ai_ShowPaymentNews(); 
int cm_SendObject1b5(int owner, G17B7 value); int cm_GetSlotValue(int h); int cm_SendGroupState(); int cm_FlushSendQueue();
int cm_ReceiveMessages(); int cm_ExecCommands(); int cm_SendLongMessage(char *msg); int cm_GetSlotState();
int ai_AmountMenu(); void d2_SetObjEnabled(); char *itoa(int, char *, int);
void ai_StorageWindow(); int ai_ProductionWindow();
int ai_OpenTransport(); void ai_MasterLetterWindow();
#pragma pack(push, 1)
typedef struct G8CHARREC {              /* a character record of ai_Chars (AICHAR, ai_char.h): the fields the
                                           news messages of this unit read */
    unsigned short id;                  /* +0x000, the character's id */
    unsigned char kind;                 /* +0x002, the character kind */
    char pad03;
    int netId;                          /* +0x004, the id the news events carry */
    unsigned char health;               /* +0x008, > 0 = alive */
    unsigned char sex;                  /* +0x009, 0 / 1 (the text's feminine form) */
    char pad0a[2];                      /* +0x00a .. 0x00b */
    unsigned char b00c;                 /* +0x00c, a kind the person tooltip names (AICHAR.b00c); picks the news text */
    char pad0d[0x23];                   /* +0x00d .. 0x02f */
    union {
        char name[0x58];                /* +0x030, the character's name (AICHAR.name) */
        struct {
            char first[0x10];           /* +0x030 */
            char family[0x10];          /* +0x040 */
        };
        struct {
            char padname5c[0x2c];       /* +0x030 .. 0x05b */
            int partner;                /* +0x05c, the netId of the character's partner (AICHAR.relatives[0]) */
            int relatives[7];           /* +0x060 .. 0x07b */
        };
    };
    char pad88[0xdd];                   /* +0x088 .. 0x164 */
    unsigned char title;                /* +0x165, the title rank */
    char pad166[6];                     /* +0x166 .. 0x16b */
    AIBLD *bld;                         /* +0x16c, the building the character is in */
    char pad170[0x24];                  /* +0x170 .. 0x193 */
    int money;                          /* +0x194, the character's money (AICHAR.money) */
    char pad198[0x19];                  /* +0x198 .. 0x1b0 */
    unsigned char heldRounds;           /* +0x1b1, the rounds the character is held */
    char pad1b2[0x6a];                  /* +0x1b2 .. 0x21b */
} G8CHARREC;
#pragma pack(pop)
int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s);
int he_NoProtectionEvent(); void snd_PlayFavourComment(void); 
char * he_NextEvent(void);
unsigned char * gm_BuildQuery(int n, ...); int gm_BuildingValues(); int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj);
void d2_RadioClear(int g); int d2_RadioGetSel(); int cm_SendMoveObject(int a, int b, int c); void cm_SendDeleteObject(int id);
int d2_GetViewWin(); void d2_CreateScrollButtons(); void d2_ClearInput(void);
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int gv_Accuse();
void cm_SendMessage5b(int a, int b, int c);
typedef struct LCKIDNAPEVENT {          /* the kidnap event 0x3c's words over AIJOB's union (he.c HEEV3C, same layout) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the victim */
    int target;                         /* 0xb0, the id of the building the gang goes to for the victim */
    int guild;                          /* 0xb4, the thieves' guild's id */
} LCKIDNAPEVENT;
typedef struct LCRANSOMEVENT {          /* the ransom event 0x3d's words over AIJOB's union (he.c HEEV3D, same layout) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the kidnapped character */
    int guild;                          /* 0xb0, the id of the thieves' guild that holds it */
    AITIME since;                       /* 0xb4, the time of the kidnap */
} LCRANSOMEVENT;
float gm_GetSkill(char *c, int skill); int cm_SendChrGotoBuilding(); 
int gm_CreateCharModel(); void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s); int ch_Delete();
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py); void d2_RaiseObject(); void d2_ScrollWindow();
int d2_OpenForm(); void d2_RaiseWindow(); int io_PrintText(char *fmt, ...);
int d2_SetForm(int view, int n); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_CloseView(int view); void d2_RemoveObj(); void d2_ScrollWindowBy();
int d2_CreatePicObj();
#pragma pack(push, 1)
typedef struct G8BLD {                  /* a local view of gv.h's ORDBLD (gv.h cannot be included here yet): the
                                           fields the production settings read */
    unsigned char type;                 /* +0x00, the building kind */
    int id;                             /* +0x01, the building's id (the production settings chain's key) */
} G8BLD;
#pragma pack(pop)
int ls_Read(void *buf, int size, int count, int f); void * mem_Alloc(int size, char *tag);
unsigned vfs_fwrite();
int d2_WinOpenPic(short x, short y, int pic, int win); int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out); 
int gm_Opinion(int a, int b);
#pragma pack(push, 1)
typedef struct G8OBJ {                  /* an object record of the ai_ lists (AIOBJ, ai_types.h) */
    unsigned short id;                  /* +0x00, the object's type id */
    int objId;                          /* +0x02, the object's own id */
} G8OBJ;
#pragma pack(pop)

/* dcp unit split: the definitions of lc the functions of lcdom use */
void ai_LayoutProdGoodsList(int view, int part, int pointer, int bld);
int ai_UpdateMarketGoodsList(AIOBJ *stall, AIOBJ *market);
void ai_LayoutMarketGoodsList(int view, int part, int pointer, int bld);
void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld);
int ai_UpdateStoreGoods(AIBLD *bld, AIOBJ *store);
void ai_LayoutStoreGoods(int view, int part, int pointer, AIBLD *bld, unsigned short *obj);
void lc_PlayerInJail(void);
void ai_TechTreeWindow(int bld, int room);
int ai_SearchResourceWindow(AIBLD *bld);
int ai_ExtractResourceWindow(AIBLD *bld);
int ai_FillExchangeRows(int list, LCEXCHROW *rows, int view, int form, char input);
void ai_Top5Window(void);

/* 0x5a9e90, a free entry of the Domizil table (127 B, he_CircusShowIncome): util_Random (ai_DomizilCount,
   0x62cb58) picks an index into the 0x42 entries of 0x1c bytes at 0x62c420; each try returns the entry's id
   when it is not among the nObjs words of objs, else the index advances modulo the count; 0 when every id is
   taken
   @deep s25: the hunk is n's decrement scheduled before the modulo (orig mov ecx,[n]; dec ecx; inc eax;
   cdq; idiv; test ecx,ecx). Every form that puts n-- before i = .. in the list (while + n--; for (..; n--,
   i = ..); do { n--; i = ..} while (n != 0); for (;;) + if (n == 0) break; i++ / n-- / i %=) gets the
   order right but gives n a register (30 ins): c2trace --prio n: the loop block then references n three
   times (u d u, P 4: +32, prio 12) and n outranks entry (10), which loses edi to nObjs. In the original n
   stays at -8 (control: block 8 u d only, the test reuses the dec). = control: unsigned n, --n in the for,
   do { .. } while (--n != 0), while (n != 0) + i = .. + n--. Product exits (found flag / k >= nObjs / k ==
   nObjs / flag without break) x loops: none closer. flip.py --want dec..inc eax: no flips; climb 3 rounds
   from the n-first base: back to control. Left: one candidate ranking (n under entry with n-- first), code
   11. prior: size fixed s4 (125 bytes, padded 128 = original), shape 5 ins: a found flag set at the break
   (the original's decided test: the break goes straight to the advance, no cmp k,nObjs) and the count loop
   as for (n = count; n != 0; n--) (one code with the guard + while form). Left: the original schedules n's
   load and dec before the modulo's inc / idiv and tests it again (mov ecx,[n]; dec ecx; inc eax; cdq;
   idiv; test ecx,ecx), ours decrements after the idiv. No lever: n-- before the modulo (30 ins, n into a
   register), do-while forms, while (n--), i++ / i %= split (30). */

unsigned short ai_RandomDomizilGoodNotIn(int nObjs, unsigned short *objs)
{
    int n;
    int i;
    int k;
    int found;
    G9DOMIZIL *entry;

    i = util_Random((unsigned short)ai_DomizilCount) & 0xffff;
    for (n = ai_DomizilCount; n != 0; n--) {
        entry = &ai_DomizilTable[i];
        found = 0;
        for (k = 0; k < nObjs; k++) {
            if (objs[k] == entry->id) {
                found = 1;
                break;
            }
        }
        if (!found)
            return entry->id;
        i = (i + 1) % ai_DomizilCount;
    }
    return 0;
}
/* 0x5a9f10, a Domizil entry in a range (191 B): util_Random (the count) picks an index into the 0x42 entries of
   the Domizil table 0x62c420; each try returns the entry's +0 id when its +0x18 rank word lies in from .. to, else
   the index advances modulo the count; 0 when no entry does */

unsigned short ai_DomizilGoodInRange(int from, int to)
{
    int n;
    int i;
    G9DOMIZIL *entry;

    i = util_Random((unsigned short)ai_DomizilCount) & 0xffff;
    n = ai_DomizilCount;
    if (ai_DomizilCount == 0)
        return 0;
    while (n != 0) {
        entry = &ai_DomizilTable[i];
        if ((int)entry->rank >= from && (int)entry->rank <= to)
            return (unsigned short)entry->id;
        n--;
        i = (i + 1) % ai_DomizilCount;
    }
    return 0;
}

/* 0x5a9f80, asks the player for a bonus amount in the "misc\Messagebox" form with a percent slider (287 B):
   sv_CheckGoodAmount gates the box for the player (good 0x140); the window's text 0x1a34, the player's goods
   value (clamped at 0x12c0) / 32 as the slider, then the frame pump sv_RunFrame (sv_FrameMask): a click 0
   confirms (the message box of txt_Ptrs[0x1a35], the slider's value), 1 or a pressed key cancels; a right
   button release (dm_L.rup) pops it; returns the click result */

int ai_AskBonusAmount(void)
{
    int mask;
    int ret;
    int win;
    int ctrl;
    int v;

    ret = 0;
    mask = sv_FrameMask;
    if (sv_CheckGoodAmountB((char *)&ai_Chars[ai_PlayerId], 0x140, ai_OwnPlayer) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "misc\\Messagebox");
    ctrl = d2_GetWinObj(win, 0, io_PrintText(0x1a34, ai_OwnPlayer));
    v = gm_GetGoodsValue((char *)&ai_Chars[ai_PlayerId]);
    if (v >= 0x12c0)
        v = 0x12c0;
    d2_SetObjValue4(ctrl, 10, v / 32, 10);
    d2_CentreViewWindows(win);
    while (sv_RunFrame(ai_AskBonusAmount, mask) != 0) {
        if (d2_ClickedId != 0) {
            if (d2_ClickedId == 1 || kb_Last == 1) {
                sv_PopRequest = 1;
                ret = 0;
            }
        } else {
            sv_MessageBox(txt_Ptrs[0x1a35], 4);
            sv_PopRequest = 1;
            ret = d2_GetObjValue(ctrl);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
    return ret;
}
#pragma pack(push, 1)
typedef struct G9DOMIZILROW {           /* a row of the Domizil personnel book: 0x5aa0d0 creates its controls,
                                           0x5aa1f0 / 0x5aa520 fill them, ai_DomizilFreeRow frees them */
    int x;                              /* 0x00, the row's position (0x5aa0d0's arguments) */
    int y;                              /* 0x04 */
    AICHARREC *ch;                      /* 0x08, the character record it shows */
    int pic;                            /* 0x0c, the picture control (d2_WinOpenPic, 0x5aa0d0) */
    int slider;                         /* 0x10, the value slider (d2_WinOpenSlider, 0x5aa1f0) */
    int text;                           /* 0x14, the text control (0x5aa1f0 / 0x5aa520) */
    int name;                           /* 0x18, the name control (0x5aa0d0) */
    int icon;                           /* 0x1c, the character's icon control (d2_WinOpenPic, 0x5aa1f0) */
    unsigned char state;                /* 0x20, 0: vacant, 1: a character, 2: a hire running (ai_FillVacantPersonnelRow) */
    char pad21[3];                      /* 0x21 .. 0x23 */
    int title;                          /* 0x24, the title rank of the character the row was filled with (ai_PersonnelBook
                                           refills a row whose character or title changed); cleared by 0x5aa0d0 */
    int ctrl28[6];                      /* 0x28 .. 0x3f, the six entries' controls (their roles are not recovered:
                                           the book window creates them, its body is not written yet) */
    int ctrl40[6];                      /* 0x40 .. 0x57, the second control of each entry */
    int view;                           /* 0x58, the view (d2_CurrentView, 0x5aa0d0) */
} G9DOMIZILROW;
#pragma pack(pop)

/* 0x5aa0d0, the Domizil personnel book row at x / y (232 B, ai_PersonnelBook): its picture 0x5db, the two texts
   (txt_NoCharacter at y - 0x11, ch_EmptyName at y + 0x44) at x - 0x36, and the record: the picture at +0xc, the
   texts at +0x14 / +0x18 (their width 0xa0, centred), the ids -1 at +0x10 / +0x1c and the six pairs at +0x28 /
   +0x40, the title 0, the view d2_CurrentView at +0x58 */

void ai_CreatePersonnelRow(int x, int y, G9DOMIZILROW *row)
{
    int id;
    int i;

    id = d2_WinOpenPic(x, y, 0x5db, d2_LastWindow);
    row->pic = id;
    d2_Objs[id].interactive = 1;
    id = d2_CreateWinText(x - 0x36, y - 0x11, txt_NoCharacter, d2_LastWindow);
    row->text = id;
    d2_Objs[id].w = 0xa0;
    d2_Objs[row->text].centred = 1;
    id = d2_CreateWinText(x - 0x36, y + 0x44, ch_EmptyName, d2_LastWindow);
    row->name = id;
    d2_Objs[id].w = 0xa0;
    d2_Objs[row->name].centred = 1;
    row->icon = -1;
    row->slider = -1;
    row->ch = 0;
    row->title = 0;
    row->view = d2_CurrentView;
    row->x = x;
    row->y = y;
    for (i = 0; i < 6; i++) {
        row->ctrl28[i] = -1;
        row->ctrl40[i] = -1;
    }
}
/* 0x5aa1f0, fills the personnel book row row with the worker ch (700 B): the worker's portrait (+0x18c) as the
   icon, its name in the first text control, the opinion slider; a worker without a job (jobId 0) gets the status
   line by the owner's title against the building type's two job titles (AIBTYPE.title2 / title): with v174 the
   texts 0x1a39 / 0x1a3a after it, else 0x1a3b / 0x1a3c; with bars the skills [3] / [2] as value bars; the slider
   shows the player's opinion of the owner (gm_Opinion, -128 .. 127 as 0 .. 100) */

void ai_FillPersonnelRow(AICHARREC *ch, G9DOMIZILROW *row, int bars)
{
    char text[128];
    AIBTYPE *bt;
    int v;
    double f;

    bt = &((AIBTYPE *)ai_BTypes)[ch->bld->type];
    d2_SetForm(row->view, 3);
    row->icon = d2_WinOpenPic((short)row->x + 2, (short)row->y, ch->picture, d2_LastWindow);
    io_CharacterName(ch->id, 1, 1, text);
    d2_SetObjText(row->text, text);
    d2_Objs[row->text].centred = 1;
    d2_Objs[row->text].w = 0xa0;
    row->slider = d2_WinOpenSlider(row->x - 8, row->y + 0x32, 0x40, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_Objs[row->slider].clickDisabled = 1;
    row->state = 1;
    d2_Objs[row->pic].user = ch;
    v = ch->v174;
    if (v != 0 && ch->jobId == 0) {
        if (row->ch->title == bt->title2)
            txt_Format(text, "%i %s", v, 0x1a39);
        else if (row->ch->title == bt->title)
            txt_Format(text, "%i %s", v, 0x1a3a);
        d2_SetObjText(row->name, text);
        d2_Objs[row->name].centred = 1;
        d2_Objs[row->name].w = 0xa0;
    } else if (ch->jobId == 0) {
        if (row->ch->title == bt->title2)
            txt_Format(text, "%s", 0x1a3b);
        else if (row->ch->title == bt->title)
            txt_Format(text, "%s", 0x1a3c);
        d2_SetObjText(row->name, text);
        d2_Objs[row->name].centred = 1;
        d2_Objs[row->name].w = 0xa0;
    }
    if (bars != 0) {
        sv_ValueBar(row->x, row->y + 100, ch->skills[3], 0x4df);
        sv_ValueBar(row->x, row->y + 0x74, ch->skills[2], 0x4df);
    }
    if (row->slider != -1) {
        v = gm_Opinion(ai_PlayerId, row->ch->id);
        f = (v + 128.0) * 100.0;
        d2_SetObjValue4(row->slider, 0, 100, (int)(f / 255.0));
    }
}

void d2_RemoveObj(); int gm_HeadIconNumber(); int gm_ObjectBuilding(int y);
void d2_SetObjFont(); void d2_SetCaption();


/* 0x5aa4b0, frees the controls of a Domizil row (106 B): the character's icon (+0x1c) and the slider (+0x10), then
   both six-control entry arrays (+0x28 .. +0x57), each set to -1; the book window's free path calls it */

void ai_DomizilFreeRow(G9DOMIZILROW *row)
{
    int i;

    if (row->icon != -1) {
        d2_RemoveObj(row->icon);
        row->icon = -1;
    }
    if (row->slider != -1) {
        d2_RemoveObj(row->slider);
        row->slider = -1;
    }
    for (i = 0; i < 6; i++) {
        if (row->ctrl28[i] != -1) {
            d2_RemoveObj(row->ctrl28[i]);
            row->ctrl28[i] = -1;
        }
        if (row->ctrl40[i] != -1) {
            d2_RemoveObj(row->ctrl40[i]);
            row->ctrl40[i] = -1;
        }
    }
}
/* 0x5aa520, fills the personnel book row row of an empty or touring place (469 B): when bld's sales-tour
   event of kind (6 / 7, he_FindEvent by AIJOB.kind and the building id) counts a ware on the row's counter
   (AIJOB.tourCounters[slot]), the text 0x1a24 and the state 2; else a living character on the row: its name and
   the state 1; else the row's controls are freed (ai_DomizilFreeRow), the text "no character" with the state 0,
   the name control emptied and the picture's user record cleared */

void ai_FillVacantPersonnelRow(AIBLD *bld, G9DOMIZILROW *row, int kind, int slot)
{
    char text[256];
    AIJOB *job;
    AICHARREC *ch;

    job = (AIJOB *)he_FindEvent(2, 0, kind, 3, bld->id);
    if (job != 0 && job->tourCounters[slot] != 0) {
        strcpy(text, txt_Ptrs[0x1a24]);
        row->state = 2;
        d2_SetObjText(row->text, text);
        d2_Objs[row->text].w = 0xa0;
        d2_Objs[row->text].h = 0x28;
        row->state = 2;
        return;
    }
    ch = row->ch;
    if (ch != 0 && ch->health != 0) {
        io_CharacterName(ch->id, 1, 1, text);
        d2_SetObjText(row->text, text);
        d2_Objs[row->text].centred = 1;
        d2_Objs[row->text].w = 0xa0;
        row->state = 1;
        return;
    }
    ai_DomizilFreeRow(row);
    strcpy(text, txt_NoCharacter);
    row->state = 0;
    d2_SetObjText(row->text, text);
    d2_Objs[row->text].w = 0xa0;
    d2_Objs[row->text].h = 0x28;
    d2_SetObjText(row->name, ch_EmptyName);
    row->state = 0;
    d2_Objs[row->pic].user = 0;
}
/* 0x5aa700, the cart list of the personnel book's building bld on its page view 3 (657 B): the heading text
   0x1a1f, then up to three of the building's carts (gm_BuildingCarts into ai_BldCarts): a frame picture 0x5db,
   the carter's head (0x639 + the head icon of the four carter models by the cart's object id & 3), the goods'
   picture (0x4b1 + the goods id) and the text 0x1a3e with the cart's destination: the building of the job it
   runs (AIOBJ.job24's cartBld), or without a job the building it stands at (the market for a 'D' building);
   a destination other than bld gets a button (picture 5, its user record the building) captioned 0x1a3d */

void ai_ShowBldCarts(SVBOOK *book, AIBLD *bld)
{
    int heads[4];
    char text[512];
    AIBLD *dest;
    int *p;
    int i;

    ai_BldCartCount = 0;
    for (p = ai_BldCartTexts; (int)p < (int)(ai_BldCartTexts + 3); p++) {
        p[-3] = -1;
        *p = -1;
    }
    d2_SetForm(book->views[3], 1);
    io_PrintText("$C");
    io_PrintText(0x1a1f);
    d2_SetForm(book->views[3], 3);
    ai_BldCartCount = gm_BuildingCarts(bld, ai_BldCarts);
    if (ai_BldCartCount > 3)
        ai_BldCartCount = 3;
    for (i = 0; i < ai_BldCartCount; i++) {
        heads[0] = gm_HeadIconNumber("arbeiter_MANN");
        heads[1] = gm_HeadIconNumber("handwerker_MANN");
        heads[2] = gm_HeadIconNumber("handwerker2_MANN");
        heads[3] = gm_HeadIconNumber("handwerker3_MANN");
        d2_WinOpenPic(0xe, i * 0x6e + 8, 0x5db, d2_LastWindow);
        d2_WinOpenPic(0x10, i * 0x6e + 10, heads[ai_BldCarts[i]->objId & 3] + 0x639, d2_LastWindow);
        d2_WinOpenPic(0x50, i * 0x78, ai_BldCarts[i]->id + 0x4b1, d2_LastWindow);
        if (ai_BldCarts[i]->job24 != 0) {
            dest = (AIBLD *)gm_BuildQuery(1, 1, ai_BldCarts[i]->job24->cartBld);
            if (dest != 0)
                txt_Format(text, 0x1a3e, dest);
        } else {
            dest = (AIBLD *)gm_ObjectBuilding((int)ai_BldCarts[i]);
            if (dest->type == 'D')
                dest = ai_MarketBuilding;
            txt_Format(text, 0x1a3e, dest);
        }
        ai_BldCartTexts[i] = d2_CreateWinText(0, 0x48 + i * 0x78, text, d2_LastWindow);
        d2_SetObjFont(ai_BldCartTexts[i], 0x56);
        if (bld != dest) {
            ai_BldCartButtons[i] = d2_WinOpenPic(0, i * 0x78 + 0x5a, 5, d2_LastWindow);
            d2_Objs[ai_BldCartButtons[i]].interactive = 1;
            d2_Objs[ai_BldCartButtons[i]].user = dest;
            d2_SetObjFont(ai_BldCartButtons[i], 0x56);
            d2_SetCaption(ai_BldCartButtons[i], txt_Ptrs[0x1a3d]);
            d2_Objs[ai_BldCartButtons[i]].style = 3;
        }
    }
}
/* 0x5aa9a0, refreshes the cart list ai_ShowBldCarts made for the building bld (476 B): per cart the text of its
   destination (0x1a3e: the building of the job it runs; 0x1a3f: without a job the building it stands at, the
   market for a 'D' building) into its text control; a destination other than bld without a button gets one
   (picture 5, its user record the building), each button its caption 0x1a3d; on the book's page 2 a click on
   a button (d2_ClickedId 5 on the pointer's object) selects its building in the panel */

void ai_UpdateBldCarts(SVBOOK *book, AIBLD *bld)
{
    char text[512];
    AIBLD *dest;
    int i;

    for (i = 0; i < ai_BldCartCount; i++) {
        if (ai_BldCarts[i]->job24 != 0) {
            dest = (AIBLD *)gm_BuildQuery(1, 1, ai_BldCarts[i]->job24->cartBld);
            if (dest != 0)
                txt_Format(text, 0x1a3e, dest);
        } else {
            dest = (AIBLD *)gm_ObjectBuilding((int)ai_BldCarts[i]);
            if (dest->type == 'D')
                dest = ai_MarketBuilding;
            txt_Format(text, 0x1a3f, dest);
        }
        if (ai_BldCartTexts[i] != -1) {
            d2_SetObjText(ai_BldCartTexts[i], text);
            if (bld != dest && ai_BldCartButtons[i] == -1) {
                ai_BldCartButtons[i] = d2_WinOpenPic(0, i * 0x78 + 0x5a, 5, d2_LastWindow);
                d2_Objs[ai_BldCartButtons[i]].interactive = 1;
                d2_Objs[ai_BldCartButtons[i]].user = dest;
                d2_SetObjFont(ai_BldCartButtons[i], 0x56);
                d2_Objs[ai_BldCartButtons[i]].style = 3;
            }
            if (ai_BldCartButtons[i] != -1)
                d2_SetCaption(ai_BldCartButtons[i], txt_Ptrs[0x1a3d]);
        }
    }
    if (book->page == 2 && d2_ClickedId == 5) {
        for (i = 0; i < 3; i++) {
            if (ai_BldCartButtons[i] != -1 && d2_PointerObj == ai_BldCartButtons[i])
                sv_SelectPanelObject((unsigned char *)d2_Objs[ai_BldCartButtons[i]].user, -1, ch_EmptyName);
        }
    }
}
void d2_RadioSelect(int g, int sel);
/* 0x5aab80, the personnel book of the building bld (ob_PERSONALBUCH, the book model model; 5511 B): four pages
   ("Buch\Personalbuch_1" .. _4, the thieves' guild classes 4 / 0x13 / 0x10 their own first page). Page 1 holds
   the wage slider (AIBLD.wageRate 50 .. 150), the working-hours list (AIBLD.workRate, the texts 0x1a2e ..
   0x1a32; not for the guild classes), the bonus button 0x1a20 and, but for the classes 5 / 9, the master's row
   (the text 0x1a2d above it); pages 2 .. 4 hold up to 12 worker rows, two columns and two rows a page, as many as
   the type has places (AIBTYPE.k231 + k232), all of them in one radio group. Then the cart list
   (ai_ShowBldCarts) and the frame pump (sv_RunFrame, this, 0x677c7): every frame the carts, the book and its
   keys (right / left turn the page); on page 1 a changed wage is sent as the field 0x3d, the balance line 0x1a2c
   is printed and the master's row follows the building's master (gm_FindCharacterByMaster). A click on a row
   offers its action under it: an empty row the hire button 0x1a22 (the job's cost, the job or title name), a
   filled one the fire button 0x1a21 and, for a worker of the second title, the promotion 0x1a25. The bonus
   (ai_AskBonusAmount's amount) is split over the workers and the master (the groups "pay_worker" / "pay_meister":
   money and relations); a hire sends the request 6 (a worker for the row, or marks it in the running request)
   or 7 (the master, with the text 0x1939), a fire the message 0x31 after the question 0x1a36, a promotion the
   message 0x32 after the question 0x1a28 / 0x1a29 with the two wages (refused with 0x42c4 for a character
   working elsewhere, 0x1a27 below four years of service). The worker rows are refilled every frame from
   gm_ListWorkers (both title lists; a row whose character or title changed is freed first). At the end the
   work rate (field 0x41) and the wage (0x3d) are sent, the book closed and the camera follows again
   @deep frame sweep s21: 748 -> 606 ins: wage (escaped: the slider value compared and sent) a local of the
   page-0 block. Left: code 342, slots 282, regs 216 (an ebp frame: dcp frame names nothing); msg as a
   block local = control. prior: Written whole from the original (was a stub): the four-page book, the wage
   slider / work list, the bonus button and its pay_worker / pay_meister groups, the master row, the 12
   worker rows in one radio group, the per-frame refill from gm_ListWorkers with the old-row compare, the
   click handling (hire 6 / 7 with the new AIMSG hire members, fire 0x31, promote 0x32 with the two wages),
   the exit field deltas 0x41 / 0x3d. 1579 vs 1567 instructions, every call the same number of times (no
   calls: line), no branch target differences. Left is allocation and layout: the scalar frame has one slot
   more than the original's (masterRow at frame 0x158 vs 0x154, msg at 0x60 vs 0x5c; the original packs
   halfH / master / the list counter in 0x2c and pg / n in 0x30), so the prover stops at the first fill
   ('ours fill4 [(args + 0x14c)] the original fill4 [(args + 0x148)]'); the message bodies' stores are
   scheduled otherwise (the three M8 lines are the AITIME copy msg.time = ai_Time stored in another word
   order, '+0xaa3 ai_Time | ai_Time+0x8'), and the slots / regs hunks follow the frame. Tried: the
   declarations in the original's address order (frame start now matches but for the one slot), a local for
   the owner's AICHAR (the wage calls), the row column / row split (r = j / 2). (references: M8 at +0xaa3:
   ours refers to ai_Time (0x770ae0), the original's field to 0x770ae8 (ai_Time+0x8) (and 2 more),
   explained: the M8 refs are the words of one struct copy (msg.time = ai_Time) stored in another order, no
   other object) */

void ai_PersonnelBook(AIBLD *bld, char *model)
{
    int i;
    SVBOOK *book;
    int sel;
    int fireButton;
    int promoteButton;
    AIBTYPE *bt;
    int hireButton;
    AICHARREC *master;
    int n;
    int fireMasterButton;
    int nWorkers;
    int radio;
    int workList;
    int hireMasterButton;
    int slot;
    int wageSlider;
    int halfW;
    int bonusButton;
    AIMSG msg;
    G9DOMIZILROW masterRow = { 0 };
    int balance[21];
    char masterText[0x60];
    G9DOMIZILROW rows[12];
    int workers[38];
    char masterName[0x60];
    char ownerName[0x60];
    char text1[0x200];
    char text2[0x400];
    G9DOMIZILROW oldRows[12];
    char text3[0x400];
    char text4[1008];
    int halfH;
    int pg;
    int j;
    int r;
    int *vp;
    int k;
    int a;
    int b;
    int per;
    int rel;
    int amount;
    int cost;
    int v;
    AICHARREC *c;
    char *owner;
    AIJOB *job;

    nWorkers = 0;
    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    fireMasterButton = -1;
    hireMasterButton = -1;
    hireButton = -1;
    fireButton = -1;
    promoteButton = -1;
    workList = -1;
    sel = -1;
    radio = -1;
    slot = -1;
    if (bt->kind != 4 && bt->kind != 0x13 && bt->kind != 0x10)
        book = sv_OpenBook(model, "Buch", 4, "Buch\\Personalbuch_1", "Buch\\Personalbuch_2", "Buch\\Personalbuch_3",
                           "Buch\\Personalbuch_4");
    else
        book = sv_OpenBook(model, "Buch", 4, "Buch\\Personalbuch_1_Diebesgilde", "Buch\\Personalbuch_2",
                           "Buch\\Personalbuch_3", "Buch\\Personalbuch_4");
    sv_UpdateBook(book);
    ai_CampaignStep(0x21, (int)book, 0, 0);
    io_CharacterName(bld->owner, 1, 2, ownerName);
    io_CharacterName(bld->masterId, 1, 2, masterName);
    master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
    if (master == 0)
        strcpy(masterText, txt_NoCharacter);
    else
        io_CharacterName(master->id, 1, 1, masterText);
    d2_SetForm(book->views[0], 3);
    wageSlider = d2_GetWinObj(book->views[0], 3, 2);
    d2_SetObjValue4(wageSlider, 0x32, 0x96, bld->wageRate);
    if (bt->kind != 4 && bt->kind != 0x13 && bt->kind != 0x10) {
        workList = d2_GetWinObj(book->views[0], 3, 3);
        d2_SetObjValue4(workList, 0, 4, bld->workRate);
        d2_SetList(workList, 5, txt_Ptrs[0x1a2e], txt_Ptrs[0x1a2f], txt_Ptrs[0x1a30], txt_Ptrs[0x1a31], txt_Ptrs[0x1a32]);
    }
    d2_SetForm(book->views[0], 3);
    io_PrintText("$8A");
    io_PrintText("$8A");
    bonusButton = d2_GetWinObj(book->views[0], 3, io_PrintText(0x1a20));
    if (bt->kind != 5 && bt->kind != 9) {
        d2_SetForm(book->views[0], 3);
        ai_CreatePersonnelRow(d2_LastWindowPtr->w / 2 - 0x18, 0x20, (int *)&masterRow);
        v = d2_CreateWinText(masterRow.x - 0x38, masterRow.y - 0x20, txt_Ptrs[0x1a2d], d2_LastWindow);
        d2_Objs[v].centred = 1;
        d2_Objs[v].w = 0xa0;
    }
    for (i = 0; i < 4; i++) {
        d2_SetForm(book->views[i], 1);
        io_PrintText(0x1a1d);
        if (i == 0)
            d2_SetForm(book->views[0], 5);
        else
            d2_SetForm(book->views[i], 4);
        io_PrintText("%s %i", 0x1a37, i + 1);
    }
    if (bt->kind == 4 || bt->kind == 0x13 || bt->kind == 0x10) {
        a = d2_GetWinObj(book->views[0], 3, 3);
        b = d2_GetWinObj(book->views[0], 3, 1);
        d2_RemoveObj(a);
        d2_RemoveObj(b);
        workList = -1;
    }
    d2_SetForm(book->views[2], 3);
    halfW = d2_Wins[d2_LastWindow].w / 2;
    halfH = d2_Wins[d2_LastWindow].h / 2;
    for (i = 0; i < 12; i++) {
        rows[i].ch = 0;
        rows[i].pic = -1;
    }
    j = 0;
    pg = 1;
    vp = &book->views[1];
    for (i = 0; i < 12; i++) {
        if (i >= bt->k232 + bt->k231)
            break;
        r = j / 2;
        if (r >= 2) {
            j = 0;
            r = 0;
            pg++;
            vp++;
            if (pg > 3)
                break;
        }
        d2_SetForm(*vp, 3);
        rows[i].view = *vp;
        ai_CreatePersonnelRow(j % 2 * halfW + 0x28, r * halfH + 0x11, (int *)&rows[i]);
        if (i == 0 && radio == -1) {
            radio = d2_CreateRadioGroup(0);
            if (masterRow.pic != -1)
                d2_RadioAdd(radio, masterRow.pic);
            d2_RadioAdd(radio, rows[0].pic);
        } else {
            d2_RadioAdd(radio, rows[i].pic);
        }
        j++;
    }
    d2_RadioSelect(radio, -1);
    n = i;
    ai_ShowBldCarts(book, bld);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame((int)ai_PersonnelBook, 0x677c7) != 0) {
        if (dm_L.rup != 0 && ai_RunAllowsWindow() == 1)
            sv_PopRequest = 1;
        ai_UpdateBldCarts(book, bld);
        sv_UpdateBook(book);
        sv_BookKeys(book);
        if (kb_Last == 0xcd)
            sv_BookForward(book);
        if (kb_Last == 0xcb)
            sv_BookBack(book);
        if (book->page == 0) {
            int wage;

            if (wageSlider != -1 && (wage = d2_GetObjValue(wageSlider)) != bld->wageRate &&
                (slot == -1 || cm_GetSlotState(slot) != 0)) {
                cm_BeginFieldBlock((char *)bld, bld->id);
                cm_AddFieldDelta(4, 1, 0x3d, &wage);
                slot = cm_SendFieldDeltas();
            }
            d2_SetForm(book->views[0], 4);
            gm_BuildingBalance((unsigned char *)bld, ai_PlayerId, balance);
            io_PrintText(0x1a2c, balance[3]);
            master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
            if (master != 0 && master->id != ai_PlayerId && master->health != 0 && masterRow.icon == -1) {
                masterRow.ch = master;
                ai_FillPersonnelRow(master, &masterRow, 0);
            } else if ((master == 0 || master->health == 0) && masterRow.icon != -1) {
                masterRow.ch = 0;
                ai_DomizilFreeRow(&masterRow);
                fireButton = -1;
            }
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == masterRow.pic && masterRow.pic != -1) {
                    for (k = 0; k < 3; k++) {
                        d2_SetForm(book->views[k], 2);
                        io_PrintText("$C");
                    }
                    hireButton = -1;
                    fireButton = -1;
                    promoteButton = -1;
                    sel = i;
                    if (masterRow.ch == 0 && masterRow.state != 2) {
                        cost = (int)gm_JobCost(ai_Chars[bld->owner], bld->type, 0, -1);
                        d2_SetForm(masterRow.view, 2);
                        hireMasterButton = d2_GetWinObj(masterRow.view, 2, io_PrintText(0x1a22, cost, bt->jobs[0] + 0x130));
                    } else if (masterRow.state == 1) {
                        d2_SetForm(masterRow.view, 2);
                        io_PrintText("$C");
                        fireMasterButton = d2_GetWinObj(masterRow.view, 2, io_PrintText(0x1a21, masterRow.ch->id));
                    }
                }
                if (d2_ClickedId != -1) {
                    if (d2_PointerObj == bonusButton && nWorkers != 0 && (ai_GameFlags & 0x80) == 0) {
                        amount = ai_AskBonusAmount();
                        if (amount != 0) {
                            per = amount / nWorkers;
                            rel = per / 3;
                            if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], gm_PlayerFactor(amount, ai_OwnPlayer), ai_OwnPlayer) != 0) {
                                for (k = 0; k < n; k++) {
                                    if (rows[k].ch != 0) {
                                        cm_BeginGroup("pay_worker");
                                        cm_SendMoveGoods(rows[k].ch->netId, -1, ai_OwnPlayer, gm_PlayerFactor(per, ai_OwnPlayer));
                                        cm_SendRelations(ai_CharInts[ai_PlayerId][0], rows[k].ch->netId, rel, 0);
                                        cm_EndGroup("pay_worker");
                                    }
                                }
                                if (masterRow.ch != 0) {
                                    cm_BeginGroup("pay_meister");
                                    cm_SendMoveGoods(masterRow.ch->netId, -1, ai_OwnPlayer, gm_PlayerFactor(per, ai_OwnPlayer));
                                    cm_SendRelations(ai_CharInts[ai_PlayerId][0], masterRow.ch->netId, rel, 0);
                                    cm_EndGroup("pay_meister");
                                }
                                cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, gm_PlayerFactor(amount, ai_OwnPlayer));
                            }
                        }
                    }
                    if (d2_ClickedId != -1) {
                        if (d2_PointerObj == hireMasterButton && hireMasterButton != -1 && (ai_GameFlags & 0x80) == 0) {
                            if (master == 0 || master->health == 0) {
                                for (job = (AIJOB *)he_FindEvent(2, 0, 7, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                                    if (job->hireTitle == 0)
                                        break;
                                if (job == 0) {
                                    memset(&msg, 0, sizeof(msg));
                                    msg.type = 7;
                                    msg.flags = 1;
                                    msg.hireMaster = 1;
                                    msg.fromId = bld->id;
                                    msg.to = ai_CharInts[ai_PlayerId][0];
                                    msg.time = ai_Time;
                                    if (gm_IsHomeType(bld) != 0)
                                        msg.hireMasterRoom = -1;
                                    else
                                        msg.hireMasterRoom = ((AIROOM *)sv_PanelShown)->objId;
                                    msg.hireTitle = 0;
                                    cm_SendLongMessage((char *)&msg);
                                    txt_Format(text1, 0x1939, ((AIBTYPE *)ai_BTypes)[bld->type].jobs[0] + 0x130);
                                    sv_MessageBox(text1, 0);
                                }
                                for (k = 0; k < 3; k++) {
                                    d2_SetForm(book->views[k], 2);
                                    io_PrintText("$C");
                                }
                                masterRow.state = 2;
                                hireButton = -1;
                                fireButton = -1;
                                promoteButton = -1;
                                fireMasterButton = -1;
                            }
                        } else if (d2_PointerObj == fireMasterButton && masterRow.ch != 0 &&
                                   sv_MessageBox(txt_Ptrs[0x1a36], 1) != 0) {
                            for (k = 0; k < 3; k++) {
                                d2_SetForm(book->views[k], 2);
                                io_PrintText("$C");
                            }
                            memset(&msg, 0, sizeof(msg));
                            msg.type = 0x31;
                            msg.flags = 1;
                            msg.to = ai_CharInts[ai_PlayerId][0];
                            msg.fromId = bld->id;
                            msg.time = ai_Time;
                            msg.arg[0] = masterRow.ch->netId;
                            cm_SendLongMessage((char *)&msg);
                            for (k = 0; k < 3; k++) {
                                d2_SetForm(book->views[k], 2);
                                io_PrintText("$C");
                            }
                            sel = -1;
                            hireButton = -1;
                            fireButton = -1;
                            promoteButton = -1;
                            fireMasterButton = -1;
                        }
                    }
                }
            }
        }
        memcpy(oldRows, rows, sizeof(rows));
        for (k = 0; k < n; k++)
            rows[k].ch = 0;
        nWorkers = gm_ListWorkers((unsigned char *)bld, workers);
        for (a = 0; a < 2; a++) {
            for (b = 0; b < 12; b++) {
                v = workers[1 + a * 12 + b];
                if (v != 0xffff) {
                    for (k = 0; k < n; k++)
                        if (rows[k].ch == (AICHARREC *)ai_Chars[v])
                            break;
                    if (k >= n) {
                        for (k = 0; k < n; k++) {
                            if (rows[k].ch == 0) {
                                c = (AICHARREC *)ai_Chars[v];
                                rows[k].ch = c;
                                rows[k].title = c->title;
                                break;
                            }
                        }
                    }
                }
            }
        }
        for (k = 0; k < n; k++)
            if (rows[k].ch != oldRows[k].ch || rows[k].title != oldRows[k].title)
                ai_DomizilFreeRow(&rows[k]);
        he_FindEvent(2, 0, 6, 3, bld->id);
        for (i = 0; i < n; i++) {
            if (rows[i].ch != 0) {
                if (rows[i].icon == -1)
                    ai_FillPersonnelRow(rows[i].ch, &rows[i], 0);
            } else {
                ai_FillVacantPersonnelRow(bld, &rows[i], 6, i);
            }
        }
        ai_FillVacantPersonnelRow(bld, &masterRow, 7, 0);
        if (fireButton != -1 && sel != -1 && rows[sel].ch == 0) {
            sel = -1;
            hireButton = -1;
            fireButton = -1;
            promoteButton = -1;
            fireMasterButton = -1;
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
        }
        if (d2_ClickedId == -1)
            continue;
        for (i = 0; i < 12; i++) {
            if (rows[i].pic == d2_PointerObj) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                hireButton = -1;
                fireButton = -1;
                promoteButton = -1;
                fireMasterButton = -1;
                sel = i;
                if (rows[i].ch == 0) {
                    cost = (int)gm_JobCost(ai_Chars[bld->owner], bld->type, -1, 1);
                    d2_SetForm(rows[i].view, 2);
                    hireButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a22, cost, bt->title2 + 0x20d));
                } else if (rows[i].state == 1) {
                    d2_SetForm(rows[i].view, 2);
                    io_PrintText("$C");
                    fireButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a21, rows[i].ch->id));
                    if (rows[i].ch->title == bt->title2)
                        promoteButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a25, rows[i].ch->id));
                }
            }
        }
        if (d2_PointerObj == hireButton && sel != -1) {
            job = (AIJOB *)he_FindEvent(2, 0, 6, 3, bld->id);
            if (job != 0) {
                job->tourCounters[sel] = 1;
            } else {
                memset(&msg, 0, sizeof(msg));
                msg.type = 6;
                msg.flags = 1;
                msg.fromId = bld->id;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.time = ai_Time;
                msg.hireRows[sel] = 1;
                if (gm_IsHomeType(bld) != 0)
                    msg.hireRoom = -1;
                else
                    msg.hireRoom = ((AIROOM *)sv_PanelShown)->objId;
                msg.hireFlag = 1;
                msg.hireTitle = bt->title2;
                cm_SendLongMessage((char *)&msg);
            }
            sel = -1;
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
            ai_CampaignStep(0x22, 0, 0, 0);
        } else if (d2_PointerObj == fireButton && sel != -1 && (ai_GameFlags & 0x80) == 0) {
            if (sv_MessageBox(txt_Ptrs[0x1a36], 1) != 0) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x31;
                msg.flags = 1;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.arg[0] = rows[sel].ch->netId;
                cm_SendLongMessage((char *)&msg);
                sel = -1;
            }
        } else if (d2_PointerObj == promoteButton && sel != -1) {
            c = rows[sel].ch;
            if (c->model != 0 && c->model->bldId != bld->id) {
                txt_Format(text3, 0x42c4, c->id);
                sv_MessageBox(text3, 4);
            } else if (gm_AllowAll == 0 && c->v174 < 4) {
                txt_Format(text4, 0x1a27, 4);
                sv_MessageBox(text4, 4);
            } else {
                owner = ai_Chars[bld->owner];
                a = (int)(gm_JobWage(owner, bld->type, -1, 1) * (bld->wageRate * 0.01));
                b = (int)(gm_JobWage(owner, bld->type, -1, 0) * (bld->wageRate * 0.01));
                if (c->sex == 0)
                    txt_Format(text2, 0x1a28, c->id, bt->title + 0x20d, a, b);
                else
                    txt_Format(text2, 0x1a29, c->id, bt->title + 0x230, a, b);
                if (sv_MessageBox(text2, 0) != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x32;
                    msg.flags = 2;
                    msg.fromId = bld->id;
                    msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.time = ai_Time;
                    msg.arg[0] = c->netId;
                    cm_SendLongMessage((char *)&msg);
                    for (k = 0; k < 3; k++) {
                        d2_SetForm(book->views[k], 2);
                        io_PrintText("$C");
                    }
                    sel = -1;
                    promoteButton = -1;
                }
            }
        }
    }
    if (radio != -1)
        d2_RadioClear(radio);
    cm_BeginFieldBlock((char *)bld, bld->id);
    if (workList != -1) {
        v = d2_GetObjValue(workList);
        cm_AddFieldDelta(4, 1, 0x41, &v);
    }
    if (wageSlider != -1) {
        v = d2_GetObjValue(wageSlider);
        cm_AddFieldDelta(4, 1, 0x3d, &v);
    }
    cm_SendFieldDeltas();
    ai_CampaignStep(0x21, 0, 1, 0);
    sv_CloseBook(book);
    sv_CameraFollow();
}
/* 0x5ac110, the mercenary book of the building bld (ob_SOELDNERBUCH, the book model model; 3819 B): the
   personnel book's sibling for the mercenaries (the contact 0x592320): four pages ("Buch\Soeldnerbuch_1",
   "Buch\Personalbuch_2" .. _4), the bonus button 0x1a20 on page 1, up to 12 mercenary rows on pages 2 .. 4 (two
   columns, two rows a page, as many as AIBTYPE.k231 + k232), their pictures in one radio group; the rows'
   controls are made and freed here (only the first six entry controls). Every frame (sv_RunFrame, this,
   0x677c7) the book and its keys, the bonus on page 1 (ai_AskBonusAmount split over the mercenaries, the group
   "pay_worker"), and the rows refilled from gm_ListWorkers' third list (the characters without a job): a new
   mercenary's portrait, name and opinion slider (gm_Opinion), a row with a hire running (the request 6 of the
   title 0x13) the text 0x1a24, an empty row "no character". A click on a row offers the hire button 0x1a22
   (the job's cost, the text 0x220), the fire button 0x1a21 and the promotion 0x1a25; hire sends the request 6,
   fire the message 0x31 after the question 0x1a36, promotion the message 0x32 (refused with 0x1a27 below
   four years of service). At the end the radio group is cleared, the book closed and the camera follows
   @deep Written whole from the original (was a stub): the four-page mercenary book, the bonus split
   (pay_worker), the inline row creation (picture 0x5db, the two texts, the first six entry controls), the
   per-frame refill from gm_ListWorkers' third list (jobless characters), the inline fill (portrait, name,
   opinion slider; the hire text 0x1a24 for a running request 6 of title 0x13; 'no character' with the
   controls freed), the clicks (hire request 6 with hireTitle 0x13, fire 0x31, promotion 0x32 or the
   refusal 0x1a27). 1075 vs 1077 instructions, every call the same number of times. Left is allocation /
   scheduling: in the row setup ours keeps the picture x (px) in ebx and y in a slot, the original y in ebx
   and px in a slot ('+ add ebx, 0x44' against '- mov eax, [esp + 0x24] / add eax, 0x44'), which moves the
   d2_Objs / rows[i].name stores (the two M8 lines at +0x368 / +0x384 are those crossed stores) and the
   msg.time = ai_Time copy is stored in another word order (M8 at +0xc31 / +0xc3b). The prover stops at
   gm_PlayerFactor's byte argument (ai_OwnPlayer pushed through dl with other upper bits). Tried: the
   declaration order of y / px; the opinion's (v + 128.0) * 100.0 kept in a double (that settled the M10
   fold of 100 / 255). (references: M8 at +0x368: ours refers to d2_LastWindow (0x6ed938), the original's
   field to 0xf8f080 (d2_Objs, d2_ObjsC) (and 3 more), explained: the M8 refs are crossed stores of the
   same objects (rows[i].name / d2_Objs) and the words of one struct copy (msg.time = ai_Time) in another
   order) */

void ai_MercenaryBook(AIBLD *bld, char *model)
{
    int *vp;
    int pg;
    int j;
    int sel;
    SVBOOK *book;
    int y;
    int fireButton;
    int hireButton;
    int halfW;
    int promoteButton;
    AIBTYPE *bt;
    int radio;
    int nWorkers;
    int halfH;
    int bonusButton;
    AIMSG msg;
    G9DOMIZILROW rows[12];
    char masterText[0x60];
    char hireText[0x80];
    char emptyText[0x80];
    char nameText[0x80];
    int workers[38];
    char masterName[0x60];
    char ownerName[0x60];
    char text[0x400];
    G9DOMIZILROW oldRows[12];
    int n;
    int i;
    int r;
    int k;
    int px;
    int pic;
    int t;
    int per;
    int rel;
    int amount;
    int cost;
    int v;
    double f;
    AICHARREC *master;
    AICHARREC *c;
    AIJOB *job;

    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    nWorkers = 0;
    hireButton = -1;
    fireButton = -1;
    promoteButton = -1;
    sel = -1;
    radio = -1;
    book = sv_OpenBook(model, "Buch", 4, "Buch\\Soeldnerbuch_1", "Buch\\Personalbuch_2", "Buch\\Personalbuch_3",
                       "Buch\\Personalbuch_4");
    sv_UpdateBook(book);
    io_CharacterName(bld->owner, 1, 2, ownerName);
    io_CharacterName(bld->masterId, 1, 2, masterName);
    master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
    if (master == 0)
        strcpy(masterText, txt_NoCharacter);
    else
        io_CharacterName(master->id, 1, 1, masterText);
    d2_SetForm(book->views[0], 3);
    io_PrintText("$7A");
    io_PrintText("$6A");
    bonusButton = d2_GetWinObj(book->views[0], 3, io_PrintText(0x1a20));
    d2_SetForm(book->views[0], 3);
    for (i = 0; i < 4; i++) {
        d2_SetForm(book->views[i], 1);
        io_PrintText(0x1a1d);
        if (i == 0)
            d2_SetForm(book->views[0], 5);
        else
            d2_SetForm(book->views[i], 4);
        io_PrintText("%s %i", 0x1a37, i + 1);
    }
    d2_SetForm(book->views[2], 3);
    halfW = d2_Wins[d2_LastWindow].w / 2;
    halfH = d2_Wins[d2_LastWindow].h / 2;
    for (i = 0; i < 12; i++) {
        rows[i].ch = 0;
        rows[i].pic = -1;
    }
    j = 0;
    pg = 1;
    vp = &book->views[1];
    for (i = 0; i < 12; i++) {
        if (i >= bt->k232 + bt->k231)
            break;
        r = j / 2;
        if (r >= 2) {
            pg++;
            vp++;
            r = 0;
            j = 0;
            if (pg > 3)
                break;
        }
        d2_SetForm(*vp, 3);
        rows[i].view = *vp;
        rows[i].x = j % 2 * halfW + 0x10;
        y = r * halfH + 0x11;
        rows[i].y = y;
        px = rows[i].x + 0x18;
        pic = d2_WinOpenPic(px, y, 0x5db, d2_LastWindow);
        rows[i].pic = pic;
        if (i == 0)
            radio = d2_CreateRadioGroup(1, pic);
        else
            d2_RadioAdd(radio, pic);
        px -= 0x36;
        t = d2_CreateWinText(px, y - 0x11, txt_NoCharacter, d2_LastWindow);
        rows[i].text = t;
        y += 0x44;
        d2_Objs[pic].interactive = 1;
        d2_Objs[t].w = 0xa0;
        d2_Objs[t].centred = 1;
        t = d2_CreateWinText(px, y, ch_EmptyName, d2_LastWindow);
        rows[i].name = t;
        rows[i].ch = 0;
        d2_Objs[t].w = 0xa0;
        d2_Objs[t].centred = 1;
        j++;
        rows[i].icon = -1;
        rows[i].slider = -1;
        for (k = 0; k < 6; k++)
            rows[i].ctrl28[k] = -1;
    }
    n = i;
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame((int)ai_MercenaryBook, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        sv_UpdateBook(book);
        sv_BookKeys(book);
        if (kb_Last == 0xcd)
            sv_BookForward(book);
        if (kb_Last == 0xcb)
            sv_BookBack(book);
        if (book->page == 0 && d2_ClickedId != -1 && d2_PointerObj == bonusButton && nWorkers != 0) {
            amount = ai_AskBonusAmount();
            if (amount != 0) {
                per = amount / nWorkers;
                rel = per / 3;
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], gm_PlayerFactor(amount, ai_OwnPlayer), ai_OwnPlayer) != 0) {
                    for (k = 0; k < n; k++) {
                        if (rows[k].ch != 0) {
                            cm_BeginGroup("pay_worker");
                            cm_SendMoveGoods(rows[k].ch->netId, -1, ai_OwnPlayer, gm_PlayerFactor(per, ai_OwnPlayer));
                            cm_SendRelations(ai_CharInts[ai_PlayerId][0], rows[k].ch->netId, rel, 0);
                            cm_EndGroup("pay_worker");
                        }
                    }
                    cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, gm_PlayerFactor(amount, ai_OwnPlayer));
                }
            }
        }
        memcpy(oldRows, rows, sizeof(rows));
        for (k = 0; k < n; k++)
            rows[k].ch = 0;
        nWorkers = gm_ListWorkers((unsigned char *)bld, workers);
        for (i = 0; i < 12; i++) {
            v = workers[0x19 + i];
            if (v != 0xffff && ((AICHARREC *)ai_Chars[v])->jobId == 0) {
                for (k = 0; k < n; k++)
                    if (rows[k].ch == (AICHARREC *)ai_Chars[v])
                        break;
                if (k >= n) {
                    for (k = 0; k < n; k++) {
                        if (rows[k].ch == 0) {
                            rows[k].ch = (AICHARREC *)ai_Chars[v];
                            break;
                        }
                    }
                }
            }
        }
        for (k = 0; k < n; k++) {
            if (rows[k].ch != oldRows[k].ch) {
                if (rows[k].icon != -1) {
                    d2_RemoveObj(rows[k].icon);
                    rows[k].icon = -1;
                }
                if (rows[k].slider != -1) {
                    d2_RemoveObj(rows[k].slider);
                    rows[k].slider = -1;
                }
                for (i = 0; i < 6; i++) {
                    if (rows[k].ctrl28[i] != -1) {
                        d2_RemoveObj(rows[k].ctrl28[i]);
                        rows[k].ctrl28[i] = -1;
                    }
                }
            }
        }
        job = (AIJOB *)he_FindEvent(2, 0, 6, 3, bld->id);
        for (i = 0; i < n; i++) {
            c = rows[i].ch;
            if (c != 0) {
                if (rows[i].icon == -1) {
                    d2_SetForm(rows[i].view, 3);
                    rows[i].icon = d2_WinOpenPic((short)rows[i].x + 0x1a, (short)rows[i].y, c->picture, d2_LastWindow);
                    io_CharacterName(c->id, 1, 1, nameText);
                    d2_SetObjText(rows[i].text, nameText);
                    d2_Objs[rows[i].text].centred = 1;
                    d2_Objs[rows[i].text].w = 0xa0;
                    rows[i].slider = d2_WinOpenSlider(rows[i].x + 0xc, rows[i].y + 0x32, 0x40, 0, 100, 0x75, 0x42, d2_LastWindow);
                    rows[i].state = 1;
                    d2_Objs[rows[i].slider].clickDisabled = 1;
                }
                if (rows[i].slider != -1) {
                    v = gm_Opinion(ai_PlayerId, c->id);
                    f = (v + 128.0) * 100.0;
                    d2_SetObjValue4(rows[i].slider, 0, 100, (int)(f / 255.0));
                }
            } else if (job != 0 && job->hireTitle == 0x13 && job->tourCounters[i] != 0) {
                strcpy(hireText, txt_Ptrs[0x1a24]);
                rows[i].state = 2;
                d2_SetObjText(rows[i].text, hireText);
                d2_Objs[rows[i].text].w = 0xa0;
                d2_Objs[rows[i].text].h = 0x28;
            } else {
                if (rows[i].icon != -1) {
                    d2_RemoveObj(rows[i].icon);
                    rows[i].icon = -1;
                }
                if (rows[i].slider != -1) {
                    d2_RemoveObj(rows[i].slider);
                    rows[i].slider = -1;
                }
                for (k = 0; k < 6; k++) {
                    if (rows[i].ctrl28[k] != -1) {
                        d2_RemoveObj(rows[i].ctrl28[k]);
                        rows[i].ctrl28[k] = -1;
                    }
                }
                strcpy(emptyText, txt_NoCharacter);
                rows[i].state = 0;
                d2_SetObjText(rows[i].text, emptyText);
                d2_Objs[rows[i].text].w = 0xa0;
                d2_Objs[rows[i].text].h = 0x28;
                d2_SetObjText(rows[i].name, ch_EmptyName);
            }
        }
        if (fireButton != -1 && sel != -1 && rows[sel].ch == 0) {
            sel = -1;
            hireButton = -1;
            fireButton = -1;
            promoteButton = -1;
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
        }
        if (d2_ClickedId == -1)
            continue;
        for (i = 0; i < 12; i++) {
            if (rows[i].pic == d2_PointerObj) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                sel = i;
                hireButton = -1;
                fireButton = -1;
                promoteButton = -1;
                if (rows[i].ch == 0) {
                    cost = (int)gm_JobCost(ai_Chars[bld->owner], bld->type, -1, 1);
                    d2_SetForm(rows[i].view, 2);
                    hireButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a22, cost, 0x220));
                } else if (rows[i].state == 1) {
                    d2_SetForm(rows[i].view, 2);
                    io_PrintText("$C");
                    fireButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a21, rows[i].ch->id));
                    if (rows[i].ch->title == bt->title2)
                        promoteButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x1a25, rows[i].ch->id));
                }
            }
        }
        if (d2_PointerObj == hireButton && sel != -1) {
            job = (AIJOB *)he_FindEvent(2, 0, 6, 3, bld->id);
            if (job != 0) {
                job->tourCounters[sel] = 1;
            } else {
                memset(&msg, 0, sizeof(msg));
                msg.fromId = bld->id;
                msg.type = 6;
                msg.flags = 1;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.time = ai_Time;
                msg.hireRows[sel] = 1;
                if (gm_IsHomeType(bld) != 0)
                    msg.hireRoom = -1;
                else
                    msg.hireRoom = ((AIROOM *)sv_PanelShown)->objId;
                msg.hireFlag = 0;
                msg.hireTitle = 0x13;
                cm_SendLongMessage((char *)&msg);
            }
            sel = -1;
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
        } else if (d2_PointerObj == fireButton && sel != -1) {
            if (sv_MessageBox(txt_Ptrs[0x1a36], 1) != 0) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x31;
                msg.flags = 1;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.arg[0] = rows[sel].ch->netId;
                cm_SendLongMessage((char *)&msg);
                sel = -1;
            }
        } else if (d2_PointerObj == promoteButton && sel != -1) {
            c = rows[sel].ch;
            if (c->v174 < 4) {
                txt_Format(text, 0x1a27, 4);
                sv_MessageBox(text, 0);
            } else {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x32;
                msg.flags = 2;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.arg[0] = c->netId;
                cm_SendLongMessage((char *)&msg);
                sel = -1;
            }
        }
    }
    if (radio != -1)
        d2_RadioClear(radio);
    sv_CloseBook(book);
    sv_CameraFollow();
}
/* 0x5ad000, asks the player for a helper's bonus amount in the "misc\Messagebox" form with a percent slider
   (336 B, ai_HelperBook): as ai_AskBonusAmount with the text 0x4141 and the message box of txt_Ptrs[0x4142];
   sv_CheckGoodAmount gates the box for the player (good 0x140), the window's text 0x4141, the player's goods
   value (clamped at 0x12c0) / 32 as the slider, the frame pump sv_RunFrame (sv_FrameMask): a click 0 confirms
   (the slider's value), 1 or a pressed key cancels; a right button release (dm_L.rup) pops it; returns the click
   result */

int ai_AskHelperBonus(void)
{
    int mask;
    int ret;
    int win;
    int ctrl;
    int v;

    ret = 0;
    mask = sv_FrameMask;
    if (sv_CheckGoodAmountB((char *)&ai_Chars[ai_PlayerId], 0x140, ai_OwnPlayer) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "misc\\Messagebox");
    ctrl = d2_GetWinObj(win, 0, io_PrintText(0x4141, ai_OwnPlayer));
    v = gm_GetGoodsValue((char *)&ai_Chars[ai_PlayerId]);
    if (v >= 0x12c0)
        v = 0x12c0;
    d2_SetObjValue4(ctrl, 10, v / 32, 10);
    d2_CentreViewWindows(win);
    while (sv_RunFrame(ai_AskHelperBonus, mask) != 0) {
        if (d2_ClickedId != 0) {
            if (d2_ClickedId == 1 || kb_Last == 1) {
                sv_PopRequest = 1;
                ret = 0;
            }
        } else {
            sv_MessageBox(txt_Ptrs[0x4142], 4);
            sv_PopRequest = 1;
            ret = d2_GetObjValue(ctrl);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
    return ret;
}
/* 0x5ad150, the helpers' book of the building bld (ob_GEHILFENBUCH, the book model model; 4322 B; the
   graveyard's trade contact opens it): the mercenary book's layout ("Buch\Soeldnerbuch_1", "Buch\Personalbuch_2"
   .. _4, the page texts 0x413d / 0x4143, the bonus button 0x413e) with the personnel book's master row (the text
   0x1a2d above it) and cart list. Every frame (sv_RunFrame, this, 0x677c7) the book, its keys and the carts; on
   page 1 the bonus (ai_AskHelperBonus split over the helpers, the group "pay_worker"), the master row following
   the building's master and a click on it (the hire button 0x1a22 sends the master request 7 with the text
   0x1939, the fire button 0x1a21 the message 0x31 after the question 0x1a36). The helper rows are refilled from
   gm_ListWorkers' third list (the characters without a job) as in the mercenary book; a click on a helper offers
   the dismissal 0x413f, which after the question 0x4140 takes 1000 of its health (gm_AddHealth). At the end the
   radio group is cleared, the book closed and the camera follows
   @deep Written whole from the original (was a stub): the helpers' book (ob_GEHILFENBUCH), the mercenary
   book's rows (inline creation, refill from gm_ListWorkers' jobless list, the hire text of a running
   request 6 of title 0x13, the inline free) with the personnel book's master row and cart list; the bonus
   (ai_AskHelperBonus, pay_worker), the master hire request 7 / fire 0x31, the helper dismissal 0x413f /
   0x4140 with gm_AddHealth(id, -1000). 1220 vs 1221 instructions, every call the same number of times, no
   branch target differences. Left is allocation / layout: the text buffers lie elsewhere in the frame (the
   prover stops at the first io_CharacterName: '(args + 0x8b8)' vs '(args + 0x858)'; the declaration order
   does not move them), the sel / dismiss slots are swapped ([0x24] / [0x20]), and the message bodies'
   stores are scheduled otherwise (the M8 lines: msg.time = ai_Time's words and the rows[i].name / d2_Objs
   stores of the row setup crossing, as in ai_MercenaryBook). (references: M8 at +0x368: ours refers to
   d2_LastWindow (0x6ed938), the original's field to 0xf8f080 (d2_Objs, d2_ObjsC) (and 4 more), explained:
   the M8 refs are crossed stores of the same objects and the words of one struct copy (msg.time = ai_Time)
   in another order) */

void ai_HelperBook(AIBLD *bld, char *model)
{
    int i;
    int pg;
    int *vp;
    SVBOOK *book;
    int dismissButton;
    int sel;
    int n;
    int halfW;
    int radio;
    int fireMasterButton;
    AIBTYPE *bt;
    int nWorkers;
    int hireMasterButton;
    int bonusButton;
    G9DOMIZILROW masterRow;
    AIMSG msg;
    G9DOMIZILROW rows[12];
    char masterText[0x60];
    char nameText[0x80];
    char hireText[0x80];
    char emptyText[0x80];
    int workers[38];
    char masterName[0x60];
    char ownerName[0x60];
    char text1[0x200];
    char text2[0x400];
    G9DOMIZILROW oldRows[12];
    int halfH;
    int j;
    int r;
    int k;
    int y;
    int px;
    int pic;
    int t;
    int per;
    int rel;
    int amount;
    int cost;
    int v;
    double f;
    AICHARREC *master;
    AICHARREC *c;
    AIJOB *job;

    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    nWorkers = 0;
    dismissButton = -1;
    sel = -1;
    radio = -1;
    fireMasterButton = -1;
    hireMasterButton = -1;
    book = sv_OpenBook(model, "Buch", 4, "Buch\\Soeldnerbuch_1", "Buch\\Personalbuch_2", "Buch\\Personalbuch_3",
                       "Buch\\Personalbuch_4");
    sv_UpdateBook(book);
    io_CharacterName(bld->owner, 1, 2, ownerName);
    io_CharacterName(bld->masterId, 1, 2, masterName);
    master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
    if (master == 0)
        strcpy(masterText, txt_NoCharacter);
    else
        io_CharacterName(master->id, 1, 1, masterText);
    d2_SetForm(book->views[0], 3);
    io_PrintText("$7A");
    io_PrintText("$6A");
    bonusButton = d2_GetWinObj(book->views[0], 3, io_PrintText(0x413e));
    d2_SetForm(book->views[0], 3);
    for (i = 0; i < 4; i++) {
        d2_SetForm(book->views[i], 1);
        io_PrintText(0x413d);
        if (i == 0)
            d2_SetForm(book->views[0], 5);
        else
            d2_SetForm(book->views[i], 4);
        io_PrintText("%s %i", 0x4143, i + 1);
    }
    d2_SetForm(book->views[2], 3);
    halfW = d2_Wins[d2_LastWindow].w / 2;
    halfH = d2_Wins[d2_LastWindow].h / 2;
    for (i = 0; i < 12; i++) {
        rows[i].ch = 0;
        rows[i].pic = -1;
    }
    j = 0;
    pg = 1;
    vp = &book->views[1];
    for (i = 0; i < 12; i++) {
        if (i >= bt->k232 + bt->k231)
            break;
        r = j / 2;
        if (r >= 2) {
            pg++;
            vp++;
            r = 0;
            j = 0;
            if (pg > 3)
                break;
        }
        d2_SetForm(*vp, 3);
        rows[i].view = *vp;
        rows[i].x = j % 2 * halfW + 0x10;
        y = r * halfH + 0x11;
        rows[i].y = y;
        px = rows[i].x + 0x18;
        pic = d2_WinOpenPic(px, y, 0x5db, d2_LastWindow);
        rows[i].pic = pic;
        if (i == 0)
            radio = d2_CreateRadioGroup(1, pic);
        else
            d2_RadioAdd(radio, pic);
        px -= 0x36;
        t = d2_CreateWinText(px, y - 0x11, txt_NoCharacter, d2_LastWindow);
        rows[i].text = t;
        y += 0x44;
        d2_Objs[pic].interactive = 1;
        d2_Objs[t].w = 0xa0;
        d2_Objs[t].centred = 1;
        t = d2_CreateWinText(px, y, ch_EmptyName, d2_LastWindow);
        rows[i].name = t;
        rows[i].ch = 0;
        d2_Objs[t].w = 0xa0;
        d2_Objs[t].centred = 1;
        j++;
        rows[i].icon = -1;
        rows[i].slider = -1;
        for (k = 0; k < 6; k++)
            rows[i].ctrl28[k] = -1;
    }
    n = i;
    d2_SetForm(book->views[0], 3);
    ai_CreatePersonnelRow(d2_LastWindowPtr->w / 2 - 0x18, 0x20, (int *)&masterRow);
    i = d2_CreateWinText(masterRow.x - 0x38, masterRow.y - 0x20, txt_Ptrs[0x1a2d], d2_LastWindow);
    d2_Objs[i].centred = 1;
    d2_Objs[i].w = 0x64;
    ai_ShowBldCarts(book, bld);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame((int)ai_HelperBook, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        sv_UpdateBook(book);
        sv_BookKeys(book);
        ai_UpdateBldCarts(book, bld);
        if (kb_Last == 0xcd)
            sv_BookForward(book);
        if (kb_Last == 0xcb)
            sv_BookBack(book);
        if (book->page == 0) {
            if (d2_ClickedId != -1 && d2_PointerObj == bonusButton && nWorkers != 0) {
                amount = ai_AskHelperBonus();
                if (amount != 0) {
                    per = amount / nWorkers;
                    rel = per / 3;
                    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], gm_PlayerFactor(amount, ai_OwnPlayer), ai_OwnPlayer) != 0) {
                        for (i = 0; i < n; i++) {
                            if (rows[i].ch != 0) {
                                cm_BeginGroup("pay_worker");
                                cm_SendMoveGoods(rows[i].ch->netId, -1, ai_OwnPlayer, gm_PlayerFactor(per, ai_OwnPlayer));
                                cm_SendRelations(ai_CharInts[ai_PlayerId][0], rows[i].ch->netId, rel, 0);
                                cm_EndGroup("pay_worker");
                            }
                        }
                        cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, gm_PlayerFactor(amount, ai_OwnPlayer));
                    }
                }
            }
            master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
            if (master != 0 && master->id != ai_PlayerId && master->health != 0 && masterRow.icon == -1) {
                masterRow.ch = master;
                ai_FillPersonnelRow(master, &masterRow, 0);
            } else if ((master == 0 || master->health == 0) && masterRow.icon != -1) {
                masterRow.ch = 0;
                ai_DomizilFreeRow(&masterRow);
                dismissButton = -1;
            }
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == masterRow.pic && masterRow.pic != -1) {
                    for (k = 0; k < 3; k++) {
                        d2_SetForm(book->views[k], 2);
                        io_PrintText("$C");
                    }
                    dismissButton = -1;
                    sel = i;
                    if (masterRow.ch == 0 && masterRow.state != 2) {
                        cost = (int)gm_JobCost(ai_Chars[bld->owner], bld->type, 0, -1);
                        d2_SetForm(masterRow.view, 2);
                        hireMasterButton = d2_GetWinObj(masterRow.view, 2, io_PrintText(0x1a22, cost, bt->jobs[0] + 0x130));
                    } else if (masterRow.state == 1) {
                        d2_SetForm(masterRow.view, 2);
                        io_PrintText("$C");
                        fireMasterButton = d2_GetWinObj(masterRow.view, 2, io_PrintText(0x1a21, masterRow.ch->id));
                    }
                }
                if (d2_ClickedId != -1 && d2_PointerObj == hireMasterButton && master == 0) {
                    for (job = (AIJOB *)he_FindEvent(2, 0, 7, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                        if (job->hireTitle == 0)
                            break;
                    if (job == 0) {
                        memset(&msg, 0, sizeof(msg));
                        msg.fromId = bld->id;
                        msg.type = 7;
                        msg.flags = 1;
                        msg.hireMaster = 1;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.time = ai_Time;
                        if (gm_IsHomeType(bld) != 0)
                            msg.hireMasterRoom = -1;
                        else
                            msg.hireMasterRoom = ((AIROOM *)sv_PanelShown)->objId;
                        msg.hireTitle = 0;
                        cm_SendLongMessage((char *)&msg);
                        txt_Format(text1, 0x1939, ((AIBTYPE *)ai_BTypes)[bld->type].jobs[0] + 0x130);
                        sv_MessageBox(text1, 0);
                    }
                }
            }
        }
        if (d2_ClickedId != -1 && d2_PointerObj == fireMasterButton && masterRow.ch != 0 &&
            sv_MessageBox(txt_Ptrs[0x1a36], 1) != 0) {
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x31;
            msg.flags = 1;
            msg.fromId = bld->id;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.time = ai_Time;
            msg.arg[0] = masterRow.ch->netId;
            cm_SendLongMessage((char *)&msg);
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
            dismissButton = -1;
            fireMasterButton = -1;
            sel = -1;
        }
        memcpy(oldRows, rows, sizeof(rows));
        for (k = 0; k < n; k++)
            rows[k].ch = 0;
        nWorkers = gm_ListWorkers((unsigned char *)bld, workers);
        for (i = 0; i < 12; i++) {
            v = workers[0x19 + i];
            if (v != 0xffff && ((AICHARREC *)ai_Chars[v])->jobId == 0) {
                for (k = 0; k < n; k++)
                    if (rows[k].ch == (AICHARREC *)ai_Chars[v])
                        break;
                if (k >= n) {
                    for (k = 0; k < n; k++) {
                        if (rows[k].ch == 0) {
                            rows[k].ch = (AICHARREC *)ai_Chars[v];
                            break;
                        }
                    }
                }
            }
        }
        for (k = 0; k < n; k++) {
            if (rows[k].ch != oldRows[k].ch) {
                if (rows[k].icon != -1) {
                    d2_RemoveObj(rows[k].icon);
                    rows[k].icon = -1;
                }
                if (rows[k].slider != -1) {
                    d2_RemoveObj(rows[k].slider);
                    rows[k].slider = -1;
                }
                for (i = 0; i < 6; i++) {
                    if (rows[k].ctrl28[i] != -1) {
                        d2_RemoveObj(rows[k].ctrl28[i]);
                        rows[k].ctrl28[i] = -1;
                    }
                }
            }
        }
        job = (AIJOB *)he_FindEvent(2, 0, 6, 3, bld->id);
        for (i = 0; i < n; i++) {
            c = rows[i].ch;
            if (c != 0) {
                if (rows[i].icon == -1) {
                    d2_SetForm(rows[i].view, 3);
                    rows[i].icon = d2_WinOpenPic((short)rows[i].x + 0x1a, (short)rows[i].y, c->picture, d2_LastWindow);
                    io_CharacterName(c->id, 1, 1, nameText);
                    d2_SetObjText(rows[i].text, nameText);
                    d2_Objs[rows[i].text].centred = 1;
                    d2_Objs[rows[i].text].w = 0xa0;
                    rows[i].slider = d2_WinOpenSlider(rows[i].x + 0xc, rows[i].y + 0x32, 0x40, 0, 100, 0x75, 0x42, d2_LastWindow);
                    rows[i].state = 1;
                    d2_Objs[rows[i].slider].clickDisabled = 1;
                }
                if (rows[i].slider != -1) {
                    v = gm_Opinion(ai_PlayerId, c->id);
                    f = (v + 128.0) * 100.0;
                    d2_SetObjValue4(rows[i].slider, 0, 100, (int)(f / 255.0));
                }
            } else if (job != 0 && job->hireTitle == 0x13 && job->tourCounters[i] != 0) {
                strcpy(hireText, txt_Ptrs[0x1a24]);
                rows[i].state = 2;
                d2_SetObjText(rows[i].text, hireText);
                d2_Objs[rows[i].text].w = 0xa0;
                d2_Objs[rows[i].text].h = 0x28;
            } else {
                if (rows[i].icon != -1) {
                    d2_RemoveObj(rows[i].icon);
                    rows[i].icon = -1;
                }
                if (rows[i].slider != -1) {
                    d2_RemoveObj(rows[i].slider);
                    rows[i].slider = -1;
                }
                for (k = 0; k < 6; k++) {
                    if (rows[i].ctrl28[k] != -1) {
                        d2_RemoveObj(rows[i].ctrl28[k]);
                        rows[i].ctrl28[k] = -1;
                    }
                }
                strcpy(emptyText, txt_NoCharacter);
                rows[i].state = 0;
                d2_SetObjText(rows[i].text, emptyText);
                d2_Objs[rows[i].text].w = 0xa0;
                d2_Objs[rows[i].text].h = 0x28;
                d2_SetObjText(rows[i].name, ch_EmptyName);
            }
        }
        if (dismissButton != -1 && sel != -1 && rows[sel].ch == 0) {
            sel = -1;
            dismissButton = -1;
            for (k = 0; k < 3; k++) {
                d2_SetForm(book->views[k], 2);
                io_PrintText("$C");
            }
        }
        if (d2_ClickedId == -1)
            continue;
        for (i = 0; i < 12; i++) {
            if (rows[i].pic == d2_PointerObj) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                sel = i;
                dismissButton = -1;
                if (rows[i].ch != 0 && rows[i].state == 1) {
                    d2_SetForm(rows[i].view, 2);
                    io_PrintText("$C");
                    dismissButton = d2_GetWinObj(rows[i].view, 2, io_PrintText(0x413f, rows[i].ch->id));
                }
            }
        }
        if (d2_PointerObj == dismissButton && sel != -1) {
            c = rows[sel].ch;
            txt_Format(text2, 0x4140, c->id);
            if (sv_MessageBox(text2, 1) != 0) {
                for (k = 0; k < 3; k++) {
                    d2_SetForm(book->views[k], 2);
                    io_PrintText("$C");
                }
                gm_AddHealth(c->id, -1000);
                sel = -1;
            }
        }
    }
    if (radio != -1)
        d2_RadioClear(radio);
    sv_CloseBook(book);
    sv_CameraFollow();
}


void d2_CenterObj(); void gm_FreeObject(int o); int gm_LevelPayout(int v, int kind);

/* 0x5ae240, the new cart purchase window "Handel\Handel_Transport_neuer_Karren" of the building bld (808 B;
   ai_TransportWindow opens it): with three carts already (gm_BuildingCarts) only the text 0xf1 (3); else the three
   cart kinds as a radio group (the texts 0xd8, the frame 0x70b / 0x741) and the buy button 0xdb, captioned 0xda
   with the chosen kind's name (0xc10 + 2 * kind) and price (gm_ItemPrice of the item 0x134 + kind). Buying asks
   0xd9 (the price and the name in the order of the language), pays the price (cm_SendMoveGoodsUnchecked), counts
   it in the player's record (LCFIN.cartSpent) and moves the item to the building (cm_SendExSellObject), then
   closes; the frame pump is sv_RunFrame (this, 0x677c7) */

void ai_BuyCartWindow(AIBLD *bld)
{
    int buyButton;
    int list;
    int ids[3];
    char text[0x200];
    char text2[0x400];
    int win;
    int price;
    int item;
    int kind;
    int k;
    LCFIN *fin;

    kind = 0;
    if (gm_BuildingCarts(bld, 0) >= 3) {
        txt_Format(text, 0xf1, 3);
        sv_MessageBox(text, 0);
        return;
    }
    win = d2_OpenForm(0, 0, "Handel\\Handel_Transport_neuer_Karren");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText(0xd8);
    ids[0] = d2_GetWinObj(win, 1, 0);
    ids[1] = d2_GetWinObj(win, 1, 1);
    ids[2] = d2_GetWinObj(win, 1, 2);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x741);
    d2_SetForm(win, 3);
    buyButton = d2_GetWinObj(win, 3, io_PrintText(0xdb));
    list = d2_CreateRadioGroup(3, ids[0], ids[1], ids[2]);
    d2_RadioSelect(list, 0);
    do {
        if (d2_ClickedId >= 0x5e2 && d2_ClickedId <= 0x5e4) {
            for (k = 0; k < 3; k++)
                if (d2_PointerObj == ids[k])
                    kind = k;
        } else if (d2_ClickedId == 0 && d2_PointerObj == buyButton) {
            item = kind + 0x134;
            price = (int)gm_ItemPrice(item, 100);
            if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0) {
                switch (ls_GetLanguage()) {
                case 0:
                    txt_Format(text2, 0xd9, price, kind * 2 + 0xc10);
                    break;
                case 1:
                    txt_Format(text2, 0xd9, kind * 2 + 0xc10, price);
                    break;
                }
                if (sv_MessageBox(text2, 0x101) != 0) {
                    cm_SendMoveGoodsUnchecked(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, price);
                    fin = (LCFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                    if (fin != 0)
                        fin->cartSpent += price;
                    cm_SendExSellObject(bld->id, -1, item, 1, ai_OwnPlayer, 0);
                    sv_PopRequest = 1;
                }
            }
        }
        if (d2_Radios[list].sel != -1) {
            price = (int)gm_ItemPrice(kind + 0x134, 100);
            txt_Format(text, 0xda, kind * 2 + 0xc10, price);
            d2_SetCaption(buyButton, text);
            d2_CenterObj(buyButton);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BuyCartWindow, 0x677c7) != 0);
    d2_CloseView(win);
    d2_RadioClear(list);
}
/* 0x5ae570, sells the n carts carts selected in the transport window (317 B): none or more than 0x20: nothing;
   the price is 40 % of the sum of their item prices (gm_ItemPrice by the goods id and the fill); the question
   0xdc names the count and, for one cart, its goods (0x9a8 + 2 * id), for more the text 0xc10, with the price
   in the order of the language (ls_GetLanguage); confirmed (0x101), the carts are freed (gm_FreeObject) and the
   price is paid to the player (cm_SendMoveGoods from the bank) */

void ai_SellCarts(int n, AIOBJ **carts)
{
    char text[0x400];
    int sum;
    int price;
    int i;

    if (n > 0x20 || n == 0)
        return;
    sum = 0;
    for (i = 0; i < n; i++)
        sum += gm_ItemPrice(carts[i]->id, carts[i]->fill);
    price = (int)(sum * 0.4);
    switch (ls_GetLanguage()) {
    case 0:
        if (n > 1)
            txt_Format(text, 0xdc, price, n, 0xc10);
        else
            txt_Format(text, 0xdc, price, n, carts[0]->id * 2 + 0x9a8);
        break;
    case 1:
        if (n > 1)
            txt_Format(text, 0xdc, n, 0xc10, price);
        else
            txt_Format(text, 0xdc, n, carts[0]->id * 2 + 0x9a8, price);
        break;
    }
    if (sv_MessageBox(text, 0x101) != 0) {
        for (i = 0; i < n; i++)
            gm_FreeObject((int)carts[i]);
        cm_SendMoveGoods(ai_CharInts[ai_PlayerId][0], -1, ai_OwnPlayer, price);
    }
}
/* 0x5ae6b0, sends the transport order of a cart from one building to another (440 B, ai_TransportWindow per
   picked cart): the cart's carrier events (kind 0x11, key 3 = the cart's objId) end now (the game clock, stage
   -1); the script must confirm the order (ai_ScriptQuery 0x24 on the destination); the message 0xf to the
   character who (its netId; flags 2, the clock + 1 s) names the cart, the source and the destination building ids
   and the escort; an escort is paid for (gm_LevelPayout of the goods' value, cm_SendMoveGoodsUnchecked) and the
   fee returned; the player's own order steps the campaign (event 0x24); 0 when the script refuses */

int ai_SendTransportOrder(unsigned int who, AIOBJ *cart, AIBLD *from, AIBLD *to)
{
    int fee = 0;
    AIMSG msg;
    AIRECIPE goods;
    AIJOB *ev;

    ev = (AIJOB *)he_FindEvent(2, 3, cart->objId, 0, 0x11);
    while (ev != 0) {
        ev->end = ai_Time;
        ev->stage = -1;
        ev = (AIJOB *)he_NextEvent();
    }
    if (ai_ScriptQuery(0x24, to, 0, 0) == 1) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0xf;
        msg.to = ((AICHARREC *)ai_Chars)[who].netId;
        msg.fromId = -1;
        msg.flags = 2;
        msg.time = ai_Time;
        gm_AddTime(&msg.time, 0, 0, 1);
        msg.car = cart->objId;
        msg.n = from->id;
        msg.ch = to->id;
        msg.dst[5] = cart->escort;
        lc_Settings.escort = (char)msg.dst[5];
        cm_SendLongMessage((char *)&msg);
        if (cart->escort != 0) {
            gm_StallGoodsList((char *)cart, (unsigned *)&goods);
            fee = gm_LevelPayout(goods.pada8[2], cart->escort);
            cm_SendMoveGoodsUnchecked(-1, ((AICHARREC *)ai_Chars)[who].netId, ai_OwnPlayer, fee);
        }
        if (who == ai_PlayerId)
            ai_CampaignStep(0x24, to, 0, 0);
    }
    return fee;
}
/* 0x5ae870, makes the transport order of a cart due now (100 B): walks the events of the kind 0xf (he_FindEvent
   (1, 0, 0xf)); the one whose +0xac is the cart's object id (+2) gets the game clock as its end time (+0x52 ..
   0x5e) and is posted (cm_SendExSetHE (-1, it)); the transport window calls it for each cart of the picked
   building before it sends the new order (ai_SendTransportOrder) */

void ai_TransportOrderDue(AIOBJ *cart)
{
    AIJOB *ev;

    ev = (AIJOB *)he_FindEvent(1, 0, 0xf);
    while (ev != 0) {
        if (ev->u.id == cart->objId) {
            ev->end = ai_Time;
            cm_SendExSetHEI(-1, ev);
            return;
        }
        ev = (AIJOB *)he_NextEvent();
    }
}
#pragma pack(push, 1)
typedef struct G8SCENECHAR {            /* the scene character fields this unit reads (ch.h CHARACTER) */
    char pad00[0x34];
    D3OBJECT *body;                     /* 0x34, the body (CHARACTER.obj) */
    char pad38[0xf0];
    void *actions;                      /* 0x128, the action queue (CHARACTER.actions) */
} G8SCENECHAR;
#pragma pack(pop)

/* 0x5ae8e0, whether a transport cart is at its building (192 B, he_RunTrade / ai_TransportWindow): the cart's
   scene character (AIOBJ.sceneObj) whose body stands within 1350.0 of the building's scene object (AIBLD.object)
   and which runs no action (CHARACTER.actions 0) is there; else the cart's ownerId chain is resolved with
   gm_FindById: a building answers whether it is bld, an object follows its ownerId, a character ends it */

int ai_CartAtBuilding(AIBLD *bld, AIOBJ *cart)
{
    G8SCENECHAR *rec;
    D3OBJECT *obj;
    float v[4];
    AIOBJ *hitObj;
    AIBLD *hitBld;
    int out;
    int id;

    rec = (G8SCENECHAR *)cart->sceneObj;
    if (rec == 0)
        return 0;
    obj = (D3OBJECT *)bld->object;
    if (obj != 0) {
        d3_PointToWorld(obj, &obj->pos, (struct D3VECTOR *)v);
        if (d3_VecEqual(&rec->body->pos, (struct D3VECTOR *)v, 1350.0f) != 0) {
            if (rec->actions == 0)
                return 1;
        }
    }
    id = cart->ownerId;
    while (id != -1) {
        if (gm_FindById((int *)&hitObj, (int *)&hitBld, &out, id) == 0)
            return 0;
        if (hitObj != 0)
            return (int)hitObj == (int)bld;
        if (hitBld != 0)
            id = ((AIOBJ *)hitBld)->ownerId;
        if (out != 0)
            return 0;
    }
    return 0;
}
/* 0x5ae9a0, the transport of the picked goods between a warehouse and the building bld ("ob_TransportFromLager",
   1643 B, ai_TransportWindow): without a source object the warehouse lagerId (gm_Query kind 7 by id) takes
   the goods (gm_FreeStallCapacity: what fits), else the source gives them (gm_TakeableCount); the goods grids
   ai_ProdGoods2 then ai_ProdGoods are walked, each slot with a picked amount (its check control) is priced
   when priced is set (the goods price at the market class 0xa * 1.1, else * bld->priceFactor; the modes 2 / 4
   at the building's player slot), checked against the player's money (sv_CheckGoodAmount of the total over
   factor), and a factor whose change to the total is below gm_PlayerFactor (1, player) becomes 1.0; the mode
   bits 8 / 1 need the script's consent (ai_ScriptQuery 0x29 / 0x27, 0x2a / 0x25 for a source or the warehouse,
   then ai_CampaignStep); the move goes out as cm_SendExSellObject (lagerId, the cart, the goods, n, player,
   the price over factor), a changed price shows the text 0xec with the difference and plays
   "RollenKurz_s_01", and the slot's control drops by n. The moves are one send group, waited for; 1 */

int ai_TransportFromLager(int lagerId, AIOBJ *cart, AIBLD *bld, int priced, char *source, float factor,
                          unsigned char player, unsigned int mode)
{
    float total;
    float price;
    char *lager;
    AIBTYPE *bt;
    char text[256];
    LCPRODSLOT *slot;
    int n;
    int limit;
    int i;

    lager = 0;
    bt = 0;
    if (bld != 0)
        bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (source == 0) {
        lager = gm_Query(0, 2, 7, 1, lagerId);
        if (lager == 0) {
            err_Message("ob_TransportFromLager(): Could not find lager by id...");
            return 0;
        }
    }
    cm_BeginSendGroup();
    for (i = 0; i < 8; i++) {
        slot = &ai_ProdGoods2[i];
        if (slot->check == -1 || d2_GetObjValue(slot->check) == 0)
            continue;
        n = d2_GetObjValue(slot->check);
        if (source == 0) {
            if (lager != 0)
                n = gm_FreeStallCapacity(lager, slot->good, n);
        } else {
            n = gm_TakeableCount(source, slot->good, n);
        }
        if (n <= 0)
            continue;
        if (priced != 0) {
            if (bt->kind == 0xa)
                price = gm_GoodsPrice(slot->good, player) * 1.1;
            else
                price = gm_GoodsPrice(slot->good, player) * bld->priceFactor;
            if (mode == 2 || mode == 4)
                price = gm_GoodsPrice(slot->good, (unsigned char)bld->playerSlot);
        } else {
            price = 0.0f;
        }
        total = n * price;
        if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], (int)(1.0f / factor * total), player) == 0)
            continue;
        if (slot->amount < 0)
            continue;
        limit = gm_PlayerFactor(1, player);
        if (abs((int)(1.0f / factor * n * price - total)) < limit)
            factor = 1.0f;
        if (mode & 8) {
            if (source != 0) {
                if (ai_ScriptQuery(0x29, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x29, bld, 0, slot->good);
            } else {
                if (ai_ScriptQuery(0x27, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x27, bld, 0, slot->good);
            }
        } else if (mode & 1) {
            if (source != 0) {
                if (ai_ScriptQuery(0x2a, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x2a, bld, 0, slot->good);
            } else {
                if (ai_ScriptQuery(0x25, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x25, bld, 0, slot->good);
            }
        }
        cm_SendExSellObject(lagerId, cart->objId, slot->good, n, player, (int)(price / factor));
        if (price != 0.0f && factor != 1.0) {
            txt_Format(text, 0xec, (int)(total * factor - total));
            sv_SetStatusText(text);
            sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        }
        d2_SetObjValue(slot->check, 0, slot->amount, d2_GetObjValue(slot->check) - n, 0);
    }
    for (i = 0; i < 16; i++) {
        slot = &ai_ProdGoods[i];
        if (slot->check == -1 || d2_GetObjValue(slot->check) == 0)
            continue;
        n = d2_GetObjValue(slot->check);
        if (source == 0)
            n = gm_FreeStallCapacity(lager, slot->good, n);
        else
            n = gm_TakeableCount(source, slot->good, n);
        if (n <= 0)
            continue;
        if (priced != 0)
            price = gm_GoodsPrice(slot->good, player) * bld->priceFactor;
        else
            price = 0.0f;
        total = n * price;
        if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], (int)(1.0f / factor * total), player) == 0)
            continue;
        if (slot->amount < 0)
            continue;
        limit = gm_PlayerFactor(1, player);
        if (abs((int)(1.0f / factor * n * price - total)) < limit)
            factor = 1.0f;
        if (mode & 8) {
            if (source != 0) {
                if (ai_ScriptQuery(0x29, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x29, bld, 0, slot->good);
            } else {
                if (ai_ScriptQuery(0x27, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x27, bld, 0, slot->good);
            }
        } else if (mode & 1) {
            if (source != 0) {
                if (ai_ScriptQuery(0x2a, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x2a, bld, 0, slot->good);
            } else {
                if (ai_ScriptQuery(0x25, bld, 0, slot->good) == 0)
                    continue;
                ai_CampaignStep(0x25, bld, 0, slot->good);
            }
        }
        cm_SendExSellObject(lagerId, cart->objId, slot->good, n, player, (int)(price / factor));
        if (price != 0.0f && factor != 1.0) {
            txt_Format(text, 0xec, (int)(total * factor - total));
            sv_SetStatusText(text);
            sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        }
        d2_SetObjValue(slot->check, 0, slot->amount, d2_GetObjValue(slot->check) - n, 0);
    }
    cm_EndSendGroup();
    while (cm_SendGroupState() == 0)
        sv_PumpNet();
    return 1;
}
/* 0x5af010, the transport window's goods total (388 B): every picked slot of the goods grids ai_ProdGoods2 and
   ai_ProdGoods adds its amount (its check control) times the goods price at the owner (the building's player
   slot for the kind 0x47, else ai_OwnPlayer), times 1.1 at the market class 0xa, else times bld->priceFactor;
   the modes 2 / 4 take the plain price at the building's player slot instead. sv_PendingMoney becomes the
   negated total. The second argument is not read */

void ai_TransportGoodsTotal(AIBLD *bld, float factor, int mode)
{
    int n;
    LCPRODSLOT *slot;
    int i;
    float price;
    int owner;
    AIBTYPE *bt;
    float sum;

    sum = 0.0f;
    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (bld->type == 0x47)
        owner = bld->playerSlot;
    else
        owner = ai_OwnPlayer;
    for (i = 0; i < 8; i++) {
        slot = &ai_ProdGoods2[i];
        if (slot->check == -1 || d2_GetObjValue(slot->check) == 0)
            continue;
        n = d2_GetObjValue(slot->check);
        if (n == 0)
            continue;
        if (bt->kind == 0xa)
            price = gm_GoodsPrice(slot->good, owner) * 1.1;
        else
            price = gm_GoodsPrice(slot->good, owner) * bld->priceFactor;
        if (mode == 2 || mode == 4)
            price = gm_GoodsPrice(slot->good, (unsigned char)bld->playerSlot);
        sum += n * price;
    }
    for (i = 0; i < 16; i++) {
        slot = &ai_ProdGoods[i];
        if (slot->check == -1 || d2_GetObjValue(slot->check) == 0)
            continue;
        n = d2_GetObjValue(slot->check);
        if (n == 0)
            continue;
        if (bt->kind == 0xa)
            price = gm_GoodsPrice(slot->good, owner) * 1.1;
        else
            price = gm_GoodsPrice(slot->good, owner) * bld->priceFactor;
        if (mode == 2 || mode == 4)
            price = gm_GoodsPrice(slot->good, (unsigned char)bld->playerSlot);
        sum += n * price;
    }
    sv_PendingMoney = -sum;
}
/* 0x5af1a0, a forwarder of 0x5af1d0 with the mode 1 (16 B) */

int ai_OpenTransport(int building)
{
    return ai_TransportWindow((AIBLD *)building, 1);
}
/* 0x5af1b0, a forwarder of 0x5af1d0 with the mode 2 (16 B) */

int ai_OpenTransportSell(int building)
{
    return ai_TransportWindow((AIBLD *)building, 2);
}
/* 0x5af1c0, a forwarder of 0x5af1d0 with the mode 4 (16 B) */

int ai_OpenTransportBuy(int building)
{
    return ai_TransportWindow((AIBLD *)building, 4);
}
typedef struct LCCARTROW {              /* 0xf0 bytes, a cart row of the transport window (ai_TransportWindow's 16) */
    AIOBJ *cart;                        /* 0x00, the cart, 0: a free row */
    AIJOB *job;                         /* 0x04, its running transport order (event 0xf, AIJOB.u.id the cart's objId) */
    int selected;                       /* 0x08, picked for the buttons (a click on the cart picture toggles it) */
    int pic;                            /* 0x0c, the cart picture (0x4ae + the cart's kind) */
    int win;                            /* 0x10, the child window its goods sit in */
    int slider;                         /* 0x14, the fill bar (AIOBJ.fill) */
    int escortPic;                      /* 0x18, the escort picture 0x5e8 + AIOBJ.escort, its caption the escort's fee */
    int label;                          /* 0x1c, the route label (patrol on / off text); a click edits the route */
    AIRECIPE goods;                     /* 0x20, not read by the window */
    int goodsCtl[3];                    /* 0xd8, per goods slot its input icon (the goods' icon + 0xdf), -1: none */
    int goodsPic[3];                    /* 0xe4, per goods slot its stock slider, -1: none */
} LCCARTROW;

/* 0x5af1d0, the transport window "Handel\Handel_Transport" of the building bld (11961 B; ai_OpenTransport /
   ai_OpenTransportSell / ai_OpenTransportBuy pass the modes 1 / 2 / 4, sv_UpdatePanel 1, the market stand 8 |
   1). Mode 8 first opens the market stand's info panel with its seven panel buttons (a click on one picks that
   building and closes the window) and works on ai_MarketBuilding. The goods store the window trades with is
   found in the shown panel object's list or the building's (gm_Query kinds 0xff / 5 / 0x11 / 0x2a / 0x116, the
   modes 2 / 4: 0x1db / 0x1dc); mode 1 has the four buttons send / repair / buy carts / sell carts, the others
   one. Each frame the player's carts at the building (gm_Query kind 0x1d of the cart building src, the market
   class: the building of kind 0x44) fill the 16 rows (LCCARTROW: the cart picture, its fill slider, the escort
   picture whose click cycles AIOBJ.escort 0 .. 2 and the route label that opens rt_EditCartRoute), a row per
   cart with its goods (gm_StallGoodsList) as input icons and sliders; the running transport events 0xf mark
   their carts. A click on a cart picture selects it (or, with the goods list shown, moves the picked goods
   into it, ai_TransportFromLager); dropping goods on the window or the inventory child window moves them
   between the cart, the store and the player's inventory (cm_SendExSellObject, the price at the market class
   0.9, else the building's priceFactor, a random 1 in 4 times the health factor). The send button picks the
   destination on the map (ai_MapPick 0x11) and orders every selected cart there (ai_TransportOrderDue,
   ai_SendTransportOrder; their escort fees summed in the text 0xe3), the repair button refills the selected
   carts (gm_ItemPrice of the cart * 0.008 per missing percent, the message 0x10), the others buy or sell
   carts; 0x527 empties the goods back. sv_PendingMoney follows the goods total (ai_TransportGoodsTotal).
   @deep Reconstructed whole from the stub against the original (3426 instructions read end to end): the
   mode 8 info panel, the store queries per mode, the 16 cart rows (LCCARTROW), the row cleanup / add /
   goods-control sync, the transport events, the inventory child window (ai_InvSlots, ai_InvChar), the
   button enabling, the right-button reset, the click / drop branches (ai_TransportFromLager,
   cm_SendExSellObject, gm_FreeCapacity / gm_TakeableCount), the 0x527 clear, send (ai_MapPick 0x11,
   ai_SendTransportOrder), repair (message 0x10), buy / sell carts, the goods total and the close. 3417 vs
   3426 instructions, frame equal (0x1848). Fixed on the way: the own branches inverted (je to own=1),
   btns[8], two drop prices, the row y in an int (32-bit as the original), the goods slider x / y / stock
   as statements. Left: block layout and allocation: VC6 keeps ebp = -1 and the cart cursor in a slot in
   the original (ours ebx), the row walk pointer based at the row start in the original vs a field inside
   the row in ours ([esi] vs [esi-0xc], [ebx] vs [ebx-8]), selected tested as 'mov eax,[ebx+8]; test' vs
   'cmp [ebx],0', the escort / message field stores scheduled in another order (0x5b0161, 0x5b0197,
   0x5b1d55), the click branch's C block placed after A in the original (0x5b0b1c) vs after the drop
   branches in ours, and the merged sv_SetStatusText tail of the two 0xee status texts (references: M8 at
   +0xad8: ours refers to d2_RemoveObj (0x46e710), the original's field to 0x46c9e0 (d2_DeleteObject) (and
   1 more), explained: the two ref lines (d2_RemoveObj | d2_DeleteObject at +0xad8, ai_Time+8 | +0xc at
   +0x2be3) are alignment artifacts: the cleanup calls d2_RemoveObj / d2_DeleteObject in the original's
   order (goods icon, goods slider, row slider, escort picture, label) and the message copies all of
   ai_Time; the call count sv_SetStatusText 8 vs 7 is the original's cross-jumped tail of the two
   'txt_Format(buffer, 0xee, b); sv_SetStatusText(buffer)' paths) */

int ai_TransportWindow(AIBLD *bld, unsigned int mode)
{
    AIRECIPE goods = { 0 };
    AIOBJ *stall = 0;
    AIOBJ *marketObj = 0;
    char *goodsRec = 0;
    AIBLD *src = bld;
    AIOBJ *picked = 0;
    int slotX[3][3] = { { 0x51, 0, 0 }, { 0x30, 0x71, 0 }, { 0x11, 0x52, 0x93 } };
    int listShown = 0;
    int market = 0;
    int priced = 0;
    int canTrade = 0;
    int own = 0;
    int dropActive = 0;
    float factor = 1.0f;
    int boxValue = -1;
    int panelBtns[7] = { -1, -1, -1, -1, -1, -1, -1 };
    int panelIds[7] = { 0x95, 0x92, 0x93, 0x94, 0x97, 0x96, 0x2db };
    int panelForm = -1;
    AIBLD *b = bld;
    unsigned int mode8 = mode & 8;
    unsigned int mode1;
    AIBTYPE *bt;
    AIBLD *home;
    AIBLD *dest;
    AIOBJ *cart;
    AIOBJ *c;
    AIOBJ *sobj;
    AIOBJ *found;
    AIJOB *ev;
    LCINVSLOT *s;
    LCCARTROW rows[16];
    AIOBJ *pickCarts[32];
    AIOBJ *sellCarts[32];
    AMOUNTCFG cfg;
    AIMSG msg;
    AITIME due;
    GMDATE date;
    int btns[8];
    int btn0, btn1, btn2, btn3;
    int scrollUp, scrollDown, scrollBar;
    int routeBox, clearBtn;
    int win;
    int owner;
    int pic;
    int i, j, k, n, v, ok;
    int changed;
    int x, y, stock;
    int match, freeRow, notFull, item, ctl, max, price, fee, cost, empty, maxEmpty, limit;
    float applied, total, sellPrice, loadPrice, unit;
    char esc;
    char text1[0x100];
    char text2[0x100];
    char text3[0x100];
    char text4[0x100];
    char text5[0x100];

    if (mode8) {
        panelForm = d2_OpenFormS(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_marktstand");
        for (i = 0; i < 7; i++) {
            panelBtns[i] = sv_AddPanelButton(0, i * 0x18 + 0xc, txt_InfoPanelButtons[i]);
            if (*sv_PanelShown == panelIds[i]) {
                d2_Objs[panelBtns[i]].checked = 1;
                d2_Objs[panelBtns[i]].highlighted = 1;
            }
        }
        b = ai_MarketBuilding;
    }
    lc_Settings.escort = 0;
    bt = &((AIBTYPE *)ai_BTypes)[b->type];
    if (sv_PanelShown != 0) {
        mode1 = mode & 1;
        if (mode1) {
            marketObj = (AIOBJ *)gm_Query((void *)((AIOBJ *)sv_PanelShown)->list, 2, 0, 0xff, 6);
            if ((stall = (AIOBJ *)gm_Query((void *)((AIOBJ *)sv_PanelShown)->list, 2, 4, 5, 6)) == 0
                && (stall = (AIOBJ *)gm_Query((void *)((AIOBJ *)sv_PanelShown)->list, 1, 4, 0x11)) == 0
                && (stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x2a, 6)) == 0)
                stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x116, 6);
        } else if (mode == 2) {
            stall = (AIOBJ *)gm_Query((void *)((AIOBJ *)sv_PanelShown)->list, 2, 0, 0x1db, 6);
        } else if (mode == 4) {
            stall = (AIOBJ *)gm_Query((void *)((AIOBJ *)sv_PanelShown)->list, 2, 0, 0x1dc, 6);
        }
    } else {
        mode1 = mode & 1;
        if (mode1) {
            if ((stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x2a, 6)) == 0)
                stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x116, 6);
        } else if (mode == 2) {
            stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x1db, 6);
        } else if (mode == 4) {
            stall = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x1dc, 6);
        }
    }
    if (stall != 0)
        goodsRec = ai_Goods + stall->id * 0x41;
    for (i = 0; i < 16; i++) {
        rows[i].pic = -1;
        rows[i].win = -1;
        rows[i].slider = -1;
        rows[i].escortPic = -1;
        rows[i].cart = 0;
        rows[i].job = 0;
        rows[i].selected = 0;
        for (j = 0; j < 3; j++) {
            rows[i].goodsCtl[j] = -1;
            rows[i].goodsPic[j] = -1;
        }
    }
    ai_CampaignStep(0x23, bld, 0, 0);
    win = d2_OpenForm(0, 0, "Handel\\Handel_Transport");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 4);
    if (mode1) {
        sv_ButtonRow(d2_LastWindow, 0, btns, 4, txt_Ptrs[0xd3], txt_Ptrs[0xd2], txt_Ptrs[0xd1], txt_Ptrs[0xd4]);
        btn2 = btns[2];
        btn3 = btns[3];
        owner = ai_OwnPlayer;
        btn1 = btns[1];
    } else {
        sv_ButtonRow(d2_LastWindow, 0, btns, 1, txt_Ptrs[0xd3]);
        btn1 = -1;
        btn2 = -1;
        btn3 = -1;
        owner = b->playerSlot;
    }
    btn0 = btns[0];
    d2_SetForm(win, 0);
    if (mode == 2) {
        priced = 1;
        factor = gm_HealthFactor(ai_PlayerId);
    } else if (mode == 4) {
        canTrade = 1;
        factor = gm_HealthFactor(ai_PlayerId);
    } else if (b->owner != ai_PlayerId) {
        if (bt->kind == 0xa) {
            market = 1;
            src = (AIBLD *)gm_BuildQuery(1, 0, 0x44);
            priced = 1;
            factor = gm_HealthFactor(ai_PlayerId);
        }
        canTrade = 1;
        if (btn1 != -1)
            d2_EnableObject(btn1, 0);
        if (btn2 != -1)
            d2_EnableObject(btn2, 0);
        if (btn3 != -1)
            d2_EnableObject(btn3, 0);
    } else {
        own = 1;
    }
    d2_SetForm(win, 0);
    if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0) {
        sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x73f);
    } else {
        pic = 0x70b;
        if (bt->kind == 0x13 || bt->kind == 4 || bt->kind == 0x10)
            pic = 0x72f;
        else if (bt->kind == 8 || bt->kind == 0xe)
            pic = 0x70f;
        else if (bt->kind == 6)
            pic = 0x713;
        else if (bt->kind == 0x16)
            pic = 0x717;
        else if (bt->kind == 0xd || bt->kind == 0xc || bt->kind == 0xb)
            pic = 0x71b;
        else if (bt->kind == 2 || bt->kind == 7)
            pic = 0x737;
        if (canTrade == 0)
            sv_OpenWindowFrame(d2_LastWindow, pic, 0x741);
        else
            sv_OpenWindowFrame(d2_LastWindow, pic, 0x73f);
    }
    sv_CreateScrollButtons(0x270, 0x15c, d2_LastWindow, d2_GetViewWin(win, 2), 0x5e2, &scrollUp, &scrollDown,
                           &scrollBar);
    d2_Objs[scrollBar].font = 0x54;
    d2_SetForm(win, 3);
    if (mode == 2)
        io_PrintText(0xd0, ai_PlayerSlots[b->playerSlot]);
    else if (mode == 4)
        io_PrintText(0xcf, ai_PlayerSlots[b->playerSlot]);
    else if (sv_PanelShown != 0 && ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0)
        io_PrintText(*sv_PanelShown * 2 + 0x9a8);
    else if (canTrade == 0)
        io_PrintText(0xcc);
    else
        io_PrintText(0xcd);
    d2_SetForm(win, 2);
    ai_InitProductionTables();
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    ai_OpenInventoryWindow(d2_LastWindow, 0x172, 0x64, ai_PlayerId, 0x1f3c);
    d2_SetForm(win, 0);
    routeBox = d2_WinOpenPic(0x261, 0xd2, 0x528, d2_LastWindow);
    d2_Objs[routeBox].style = 3;
    d2_Objs[routeBox].checkable = 1;
    clearBtn = d2_WinOpenPic(0x261, 0x104, 0x527, d2_LastWindow);
    d2_Objs[clearBtn].style = 3;
    sv_ResetScreenClip();
    do {
        if ((ai_BuildingPerms & 0x200) == 0 && ai_IsBuildingOpen(b, 0, 0) == 0) {
            if (ai_PlayerId != b->owner) {
                priced = 0;
                canTrade = 0;
                own = 0;
            } else {
                own = 1;
            }
        } else if (b->owner != ai_PlayerId) {
            if (bt->kind == 0xa) {
                market = 1;
                priced = 1;
            }
            canTrade = 1;
        } else {
            own = 1;
        }
        if (ai_InventoryHas(ai_Chars[ai_PlayerId], 0x299) != 0)
            canTrade = 1;
        if (mode8 && d2_ClickedId != -1) {
            for (i = 0; i < 7; i++) {
                if (d2_PointerObj == panelBtns[i]) {
                    picked = (AIOBJ *)gm_Query(0, 2, 7, 0, panelIds[i]);
                    sv_PopRequest = 1;
                }
            }
        }
        if (bt->kind == 0xa) {
            if (stall != 0) {
                listShown = ai_UpdateMarketGoodsList(stall, marketObj);
                ai_LayoutMarketGoodsList(win, 1, 1, b);
            }
        } else if (stall != 0) {
            if (*goodsRec == 5) {
                listShown = ai_UpdateProdGoodsList(b, stall);
                ai_LayoutProdGoodsList(win, 1, 1, b);
            } else if (*goodsRec == 0x11) {
                listShown = ai_UpdateStoreGoods(b, stall);
                ai_LayoutStoreGoods(win, 1, 1, b, stall);
            }
        }

        /* the rows whose cart left (or runs an order, or stands elsewhere) are cleared */
        for (i = 0; i < 16; i++) {
            c = rows[i].cart;
            if (c == 0)
                continue;
            if (gm_Query((void *)src->objList, 2, 1, c->objId, 3, ai_PlayerId) != 0 && rows[i].job == 0
                && (gm_IsType10((unsigned char *)bld) == 0 || ai_CartAtBuilding(bld, c) != 0))
                continue;
            if (rows[i].pic != -1) {
                rows[i].cart = 0;
                d2_RemoveObj(rows[i].pic);
                rows[i].pic = -1;
                rows[i].job = 0;
            }
            for (j = 0; j < 3; j++) {
                if (rows[i].goodsCtl[j] != -1) {
                    d2_RemoveObj(rows[i].goodsCtl[j]);
                    d2_DeleteObject(rows[i].goodsPic[j]);
                    rows[i].goodsCtl[j] = -1;
                    rows[i].goodsPic[j] = -1;
                }
            }
            d2_DeleteObject(rows[i].slider);
            rows[i].slider = -1;
            d2_RemoveObj(rows[i].escortPic);
            rows[i].escortPic = -1;
            d2_RemoveObj(rows[i].label);
            rows[i].label = -1;
            if (rows[i].win != -1) {
                d2_RemoveWindow(rows[i].win);
                rows[i].win = -1;
            }
        }

        /* the player's carts at the building get a row each */
        cart = (AIOBJ *)gm_Query((void *)src->objList, 2, 4, 0x1d, 3, ai_PlayerId);
        for (k = 0; k < 16; k++) {
            if (cart == 0)
                break;
            d2_SetForm(win, 2);
            if (gm_IsType10((unsigned char *)bld) == 0 || ai_CartAtBuilding(bld, cart) != 0) {
                for (i = 0; i < 16; i++)
                    if (rows[i].cart != 0 && rows[i].cart->objId == cart->objId)
                        break;
                if (i >= 16) {
                    for (freeRow = 0; freeRow < 16; freeRow++)
                        if (rows[freeRow].cart == 0)
                            break;
                    ev = (AIJOB *)he_FindEvent(3, 0, 0xf, 2, ai_PlayerId, 3, cart->objId);
                    if (freeRow < 16 && ev == 0) {
                        y = (d2_Pics[0x5e2].h + 0x16) * freeRow;
                        rows[freeRow].cart = cart;
                        rows[freeRow].pic = d2_WinOpenPic(0x10, y + 0x16, cart->id + 0x4ae, d2_LastWindow);
                        rows[freeRow].escortPic = d2_WinOpenPic(0x18, y + 0x54, 0x5e8, d2_LastWindow);
                        rows[freeRow].label = d2_WinOpenPic(0xac, y + 0x54, 0x5eb, d2_LastWindow);
                        d2_Objs[rows[freeRow].escortPic].interactive = 1;
                        d2_Objs[rows[freeRow].escortPic].style = 0x43;
                        d2_Objs[rows[freeRow].escortPic].caption[0] = ch_EmptyName[0];
                        d2_Objs[rows[freeRow].label].interactive = 1;
                        d2_Objs[rows[freeRow].label].style = 0x43;
                        strcpy(d2_Objs[rows[freeRow].label].caption, txt_PanelBtnCart);
                        d2_SetObjFont(rows[freeRow].label, 0x54);
                        rows[freeRow].slider = d2_WinOpenSlider(0x4b, (d2_Pics[0x5e2].h + 0x16) * freeRow + 0x59, 0x50, 0,
                                                                0x64, 0x65, 0x2a, d2_LastWindow);
                        rows[freeRow].win = d2_CreateChildWindow(0x10, y + 0x26, 0xc8,
                                                                 0x40, 0x10, d2_LastWindow);
                        d2_SetObjFont(d2_Wins[rows[freeRow].win].parent, 0x53);
                        d2_Objs[rows[freeRow].pic].interactive = 1;
                        for (ev = (AIJOB *)he_FindEvent(2, 3, cart->objId, 0, 0x11); ev != 0;
                             ev = (AIJOB *)he_NextEvent()) {
                            ev->end = ai_Time;
                            ev->stage = -1;
                        }
                    }
                }
            }
            cart = (AIOBJ *)gm_QueryNext();
        }

        /* the running transport orders 0xf of the player mark their carts */
        for (i = 0; i < 16; i++)
            rows[i].job = 0;
        for (ev = (AIJOB *)he_FindEvent(2, 0, 0xf, 2, ai_PlayerId); ev != 0; ev = (AIJOB *)he_NextEvent())
            for (i = 0; i < 16; i++)
                if (rows[i].cart != 0 && ev->u.id == rows[i].cart->objId)
                    rows[i].job = ev;

        /* each row: its window, the goods controls, the escort and route controls */
        changed = 0;
        for (i = 0; i < 16; i++) {
            if (rows[i].win != -1) {
                d2_LastWindow = rows[i].win;
                io_PrintText("$C");
            }
            c = rows[i].cart;
            if (c == 0)
                continue;
            gm_StallGoodsList((char *)c, (unsigned *)&goods);
            for (j = 0; j < goods.n; j++) {
                if (rows[i].goodsCtl[j] != -1) {
                    if (listShown)
                        d2_Objs[rows[i].goodsCtl[j]].clickDisabled = 1;
                    else
                        d2_Objs[rows[i].goodsCtl[j]].clickDisabled = 0;
                }
            }
            if (c->flags13 & 0x20)
                io_PrintText("$Z%s", 0xde);
            if (rows[i].slider != -1)
                d2_SetObjValue4(rows[i].slider, 0, 100, c->fill);
            if (rows[i].escortPic != -1) {
                if (d2_Objs[rows[i].escortPic].pic != c->escort + 0x5e8)
                    d2_SetObjPic(rows[i].escortPic, c->escort + 0x5e8);
                txt_Format(d2_Objs[rows[i].escortPic].caption, c->escort + 0xe4, gm_LevelPayout(goods.pada8[2], c->escort));
                if (d2_ClickedId != -1 && d2_PointerObj == rows[i].escortPic) {
                    if ((c->flags20 & 1) == 0) {
                        esc = c->escort = (c->escort + 1) % 3;
                        cm_BeginFieldBlock((char *)c, c->objId);
                        cm_AddFieldValue(1, 1, 0x28, &esc);
                        cm_SendFieldValues();
                    } else {
                        sv_SetStatusText(txt_Ptrs[0xf0]);
                    }
                }
            }
            if (rows[i].label != -1)
                d2_SetCaption(rows[i].label, c->patrolOn ? txt_Ptrs[0x42eb] : txt_Ptrs[0x42ea]);
            if (d2_ClickedId != -1 && d2_PointerObj == rows[i].label && (c->flags20 & 1) == 0) {
                d2_SetViewEnabled(win, 0);
                rt_EditCartRoute(c);
                d2_SetViewEnabled(win, 1);
            }
            if ((c->flags13 & 0x20) == 0 && (c->flags20 & 1) == 0) {
                d2_Objs[rows[i].pic].greyed = 0;
            } else {
                rows[i].selected = 0;
                d2_Objs[rows[i].pic].greyed = 1;
            }
            if (rows[i].selected)
                d2_Objs[rows[i].pic].highlighted = 1;
            else
                d2_Objs[rows[i].pic].highlighted = 0;
            gm_StallGoodsList((char *)c, (unsigned *)&goods);
            d2_SetForm(win, 2);
            for (j = 0; j < goods.n; j++) {
                match = -1;
                for (k = 0; k < goods.count; k++) {
                    if (rows[i].goodsCtl[j] != -1 && goods.item[k] != 0
                        && goods.item[k] + 0xdf == d2_Objs[rows[i].goodsCtl[j]].pic) {
                        match = k;
                        break;
                    }
                }
                if (match == -1 && rows[i].goodsCtl[j] != -1) {
                    d2_RemoveObj(rows[i].goodsCtl[j]);
                    if (rows[i].goodsPic[j] != -1)
                        d2_DeleteObject(rows[i].goodsPic[j]);
                    rows[i].goodsCtl[j] = -1;
                    rows[i].goodsPic[j] = -1;
                }
            }
            for (k = 0; k < goods.count; k++) {
                item = goods.item[k];
                if (item == 0)
                    continue;
                for (j = 0; j < goods.n; j++) {
                    if (rows[i].goodsCtl[j] != -1 && d2_Objs[rows[i].goodsCtl[j]].pic == item + 0xdf) {
                        ctl = rows[i].goodsCtl[j];
                        v = d2_GetObjValue(ctl);
                        if (v > 0)
                            changed = 1;
                        max = goods.max[k];
                        d2_SetObjValue4(ctl, 0, goods.amount[k], v);
                        d2_SetObjValue4(rows[i].goodsPic[j], 0, max, goods.amount[k] - v);
                        d2_Objs[ctl].style = 0x22;
                        d2_Objs[ctl].captionX = -1;
                        d2_Objs[ctl].captionY = 0;
                        d2_Objs[ctl].centred = 1;
                        sprintf(d2_Objs[ctl].caption, "%s~ %li %s", txt_ObjectNames[goods.item[k]][0], goods.amount[k] - v,
                                txt_AmountUnit);
                        break;
                    }
                }
                if (j < goods.n)
                    continue;
                for (j = 0; j < goods.n; j++) {
                    if (rows[i].goodsCtl[j] == -1) {
                        stock = gm_ListStock((short *)goods.pada8[3], goods.item[k]);
                        x = slotX[rows[i].cart->id - 0x134][j] - d2_Wins[d2_LastWindow].x + d2_Objs[rows[i].pic].x;
                        y = (d2_Pics[0x5e2].h + 0x16) * i + 0x1e;
                        rows[i].goodsPic[j] = d2_WinOpenSlider(x - 8, y - 5, 0x2f, 0, stock, 0x6a, 0x2d, d2_LastWindow);
                        rows[i].goodsCtl[j] = d2_WinOpenInputIcon(x, y, goods.item[k] + 0xdf, 0x8a, d2_LastWindow);
                        d2_SetObjFont(rows[i].goodsCtl[j], 0x54);
                        break;
                    }
                }
            }
        }

        /* the inventory child window follows the route box */
        if (ai_ChildWindowId != -1) {
            if (listShown == 0) {
                dropActive = ai_UpdateInventoryWindow();
                v = d2_GetObjValue(routeBox);
                if (boxValue != v) {
                    if (v != 0) {
                        d2_EnableObject(d2_Wins[ai_ChildWindowId].parent, 1);
                        d2_SetForm(win, 2);
                        d2_EnableObject(d2_Wins[d2_LastWindow].parent, 0);
                    } else {
                        d2_EnableObject(d2_Wins[ai_ChildWindowId].parent, 0);
                        d2_SetForm(win, 2);
                        d2_EnableObject(d2_Wins[d2_LastWindow].parent, 1);
                    }
                }
                boxValue = v;
            } else {
                ai_UpdateInventoryWindow();
            }
        }

        /* the buttons need a selected cart; send needs one that is not full */
        notFull = 0;
        n = 0;
        for (i = 0; i < 16; i++) {
            if (rows[i].cart != 0 && rows[i].selected != 0) {
                if (rows[i].cart->fill < 100)
                    notFull = 1;
                n++;
            }
        }
        if (n == 0) {
            d2_Objs[btn0].clickDisabled = 1;
            d2_Objs[btn0].greyed = 1;
            if (mode1) {
                if (btn3 != -1) {
                    d2_Objs[btn3].clickDisabled = 1;
                    d2_Objs[btn3].greyed = 1;
                }
                if (btn1 != -1) {
                    d2_Objs[btn1].clickDisabled = 1;
                    d2_Objs[btn1].greyed = 1;
                }
            }
        } else {
            d2_Objs[btn0].clickDisabled = 0;
            d2_Objs[btn0].greyed = 0;
            if (btn3 != -1) {
                d2_Objs[btn3].clickDisabled = 0;
                d2_Objs[btn3].greyed = 0;
            }
            if (mode1) {
                if (notFull && btn1 != -1) {
                    d2_Objs[btn1].clickDisabled = 0;
                    d2_Objs[btn1].greyed = 0;
                } else {
                    d2_Objs[btn1].clickDisabled = 1;
                    d2_Objs[btn1].greyed = 1;
                }
            }
        }
        if (listShown == 0 && dropActive == 0)
            d2_ClearPointerItems();
        if (changed) {
            for (i = 0; i < 16; i++)
                if (rows[i].cart != 0)
                    for (j = 0; j < 3; j++)
                        if (rows[i].goodsPic[j] != -1)
                            d2_AddPointerItem(d2_Objs[rows[i].goodsCtl[j]].pic, d2_GetObjValue(rows[i].goodsCtl[j]));
        }

        /* the right button drops the picked goods, closes the route box or the window */
        if (dm_L.rup != 0) {
            if (d2_CountPointerItems() != 0) {
                d2_EndInputEdit();
                for (i = 0; i < 16; i++)
                    if (rows[i].cart != 0)
                        for (j = 0; j < 3; j++)
                            if (rows[i].goodsCtl[j] != -1)
                                d2_SetObjValue4(rows[i].goodsCtl[j], 0, 0, 0);
                for (k = 0; k < 8; k++)
                    if (ai_ProdGoods2[k].check != -1)
                        d2_SetObjValue4(ai_ProdGoods2[k].check, 0, 0, 0);
                for (k = 0; k < 16; k++)
                    if (ai_ProdGoods[k].check != -1)
                        d2_SetObjValue4(ai_ProdGoods[k].check, 0, 0, 0);
                for (k = 0; k < 6; k++)
                    if (ai_InvSlots[k].obj != 0)
                        d2_SetObjValue4(ai_InvSlots[k].ctl, 0, 0, 0);
                changed = 0;
                listShown = 0;
                dropActive = 0;
                d2_ClearPointerItems();
            } else if (d2_GetObjValue(routeBox) != 0) {
                d2_SetObjValue2(routeBox, 0);
                boxValue = 0;
            } else if (ai_RunAllowsWindow() == 1) {
                sv_PopRequest = 1;
                if (picked == 0 && sv_SelectedObject == 0)
                    sv_CameraFollow();
            }
        }

        if (d2_ClickedId == -1 || changed != 0 || listShown == 0) {
            if (d2_ClickedId != -1 && changed == 0 && dropActive == 0) {
                /* a click on a cart selects it */
                for (i = 0; i <= 15; i++) {
                    c = rows[i].cart;
                    if (c != 0 && d2_PointerObj == rows[i].pic) {
                        if (c->flags20 & 1)
                            sv_SetStatusText(txt_Ptrs[0xf0]);
                        else
                            rows[i].selected = !rows[i].selected;
                    }
                }
            } else if (d2_HitWindow == d2_Views[win].wins[1] && dropActive != 0 && dm_L.lup != 0) {
                /* inventory goods dropped on the window go into the store */
                if (own != 0) {
                    for (k = 0; k < 6; k++) {
                        s = &ai_InvSlots[k];
                        if (s->obj == 0)
                            continue;
                        item = s->obj->id;
                        ok = 1;
                        v = d2_GetObjValue(s->ctl);
                        if (v == 0)
                            continue;
                        if (priced) {
                            if (market)
                                sellPrice = gm_GoodsPrice(item, owner) * 0.9;
                            else
                                sellPrice = gm_GoodsPrice(item, owner) * b->priceFactor;
                        } else {
                            sellPrice = 0.0f;
                        }
                        if (marketObj != 0 && gm_Query((void *)marketObj->list, 1, 0, item) == 0)
                            ok = 0;
                        n = gm_FreeCapacity(stall, item, v);
                        if (ok && n) {
                            cm_SendExSellObject(stall->objId, ai_InvChar->netId, item, n, owner, (int)sellPrice);
                            d2_SetObjValue4(s->ctl, 0, 100, 0);
                        }
                    }
                } else {
                    txt_Format(text3, 0xef);
                    sv_MessageBox(text3, 0x100);
                }
            } else if (d2_HitWindow == ai_ChildWindowId && dm_L.lup != 0 && dropActive == 0 && listShown != 0) {
                /* goods of the list dropped on the inventory window go to the player */
                if (canTrade == 0 && own == 0) {
                    txt_Format(text5, 0xee, b);
                    sv_SetStatusText(text5);
                } else {
                    d2_EndInputEdit();
                    if (util_RandomF() < 0.25f)
                        ai_TransportFromLager(ai_InvChar->netId, stall, b, canTrade, ai_Chars[ai_PlayerId], factor, owner, mode);
                    else
                        ai_TransportFromLager(ai_InvChar->netId, stall, b, canTrade, ai_Chars[ai_PlayerId], 1.0f, owner, mode);
                    for (k = 0; k < 6; k++)
                        if (ai_InvSlots[k].obj != 0)
                            d2_SetObjValue4(ai_InvSlots[k].ctl, 0, 0, 0);
                    di_ClearButtons();
                }
            } else if (d2_ClickedId != -1 && dropActive != 0 && dm_L.lclick != 0) {
                /* inventory goods dropped on a cart go into it */
                found = 0;
                for (i = 0; i < 16; i++)
                    if (rows[i].cart != 0 && rows[i].pic == d2_PointerObj)
                        break;
                if (i < 16) {
                    c = rows[i].cart;
                    if (c->flags20 & 1) {
                        sv_SetStatusText(txt_Ptrs[0xf0]);
                    } else {
                        found = c;
                        rows[i].selected = 1;
                    }
                }
                if (found != 0) {
                    for (k = 0; k < 6; k++) {
                        s = &ai_InvSlots[k];
                        sobj = (AIOBJ *)gm_Query((void *)found->list, 1, 0, 0x1dd);
                        if (s->obj == 0)
                            continue;
                        item = s->obj->id;
                        v = d2_GetObjValue(s->ctl);
                        if (v == 0)
                            continue;
                        if (priced) {
                            if (market)
                                loadPrice = gm_GoodsPrice(item, owner) * 0.9;
                            else
                                loadPrice = gm_GoodsPrice(item, owner) * b->priceFactor;
                        } else {
                            loadPrice = 0.0f;
                        }
                        n = gm_FreeCapacity(sobj, item, v);
                        if (n == 0)
                            continue;
                        cm_SendExSellObject(sobj->objId, ai_InvChar->netId, item, n, owner, (int)loadPrice);
                        d2_SetObjValue4(s->ctl, 0, 100, 0);
                    }
                }
            }
        } else {
            /* a click on a cart moves the goods picked in the list into it */
            for (i = 0; i < 16; i++)
                if (rows[i].cart != 0 && rows[i].pic == d2_PointerObj)
                    break;
            if (i < 16) {
                c = rows[i].cart;
                if (c->flags20 & 1) {
                    sv_SetStatusText(txt_Ptrs[0xf0]);
                } else {
                    rows[i].selected = 1;
                    if (c != 0) {
                        if (canTrade == 0 && own == 0) {
                            txt_Format(text4, 0xee, b);
                            sv_SetStatusText(text4);
                        } else {
                            sobj = (AIOBJ *)gm_Query((void *)c->list, 1, 0, 0x1dd);
                            d2_EndInputEdit();
                            if (util_RandomF() < 0.25f)
                                ai_TransportFromLager(sobj->objId, stall, b, canTrade, 0, factor, owner, mode);
                            else
                                ai_TransportFromLager(sobj->objId, stall, b, canTrade, 0, 1.0f, owner, mode);
                        }
                    }
                }
            }
        }

        if (d2_HitWindow == d2_Views[win].wins[1] && changed != 0 && dm_L.lclick != 0) {
            /* the goods picked from the carts dropped on the window go into the store */
            if (priced != 0 || own != 0) {
                applied = 1.0f;
                if (util_RandomF() < 0.25f)
                    applied = factor;
                for (i = 0; i <= 15; i++) {
                    c = rows[i].cart;
                    if (c == 0 || (c->flags20 & 1))
                        continue;
                    sobj = (AIOBJ *)gm_Query((void *)c->list, 1, 0, 0x1dd);
                    for (j = 0; j < 3; j++) {
                        ctl = rows[i].goodsCtl[j];
                        if (ctl == -1)
                            continue;
                        item = d2_Objs[ctl].pic - 0xdf;
                        ok = 1;
                        if (priced) {
                            if (market)
                                price = (int)(gm_GoodsPrice(item, owner) * 0.9);
                            else
                                price = (int)(gm_GoodsPrice(item, owner) * b->priceFactor);
                            if (mode == 2 || mode == 4)
                                price = (int)gm_GoodsPrice(item, (unsigned char)b->playerSlot);
                        } else {
                            price = 0;
                        }
                        if (marketObj != 0 && gm_Query((void *)marketObj->list, 1, 0, item) == 0)
                            ok = 0;
                        v = d2_GetObjValue(ctl);
                        if (ok == 0) {
                            if (v > 0) {
                                txt_Format(text2, 0xeb, item * 2 + 0x9a8);
                                sv_MessageBox(text2, 0x100);
                            }
                        } else if (v > 0) {
                            n = gm_FreeCapacity(stall, item, v);
                            total = n * price;
                            limit = gm_PlayerFactor(1, ai_OwnPlayer);
                            if ((int)(total * applied - total) < limit)
                                applied = 1.0f;
                            if (mode8) {
                                if (ai_ScriptQuery(0x28, b, 0, item) == 0)
                                    continue;
                                ai_CampaignStep(0x28, b, 0, item);
                            } else if (mode1) {
                                if (ai_ScriptQuery(0x26, b, 0, item) == 0)
                                    continue;
                                ai_CampaignStep(0x26, b, 0, item);
                            }
                            if (n > 0)
                                cm_SendExSellObject(stall->objId, sobj->objId, item, n, owner, (int)(price * applied));
                            if (applied != 1.0 && n > 0) {
                                txt_Format(text2, 0xed, (int)(total * applied - total));
                                sv_SetStatusText(text2);
                                sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
                            }
                            if (n > 0)
                                d2_SetObjValue4(ctl, 0, 100, 0);
                        }
                    }
                }
            }
            d2_ClearPointerItems();
            for (k = 0; k < 8; k++)
                if (ai_ProdGoods2[k].check != -1)
                    d2_SetObjValue4(ai_ProdGoods2[k].check, 0, 0, 0);
            for (k = 0; k < 16; k++)
                if (ai_ProdGoods[k].check != -1)
                    d2_SetObjValue4(ai_ProdGoods[k].check, 0, 0, 0);
            di_ClearButtons();
        } else if (d2_HitWindow == ai_ChildWindowId && dm_L.lclick != 0 && dropActive == 0 && changed != 0
                   && (priced != 0 || own != 0)) {
            /* the goods picked from the carts dropped on the inventory window go to the player */
            for (i = 0; i <= 15; i++) {
                c = rows[i].cart;
                if (c == 0 || (c->flags20 & 1))
                    continue;
                sobj = (AIOBJ *)gm_Query((void *)c->list, 1, 0, 0x1dd);
                for (j = 0; j < 3; j++) {
                    ctl = rows[i].goodsCtl[j];
                    if (ctl == -1)
                        continue;
                    item = d2_Objs[ctl].pic - 0xdf;
                    ok = 1;
                    if (marketObj != 0 && gm_Query((void *)marketObj->list, 1, 0, item) == 0)
                        ok = 0;
                    v = d2_GetObjValue(ctl);
                    if (ok && v > 0) {
                        n = gm_TakeableCount(ai_Chars[ai_PlayerId], item, v);
                        if (n > 0) {
                            cm_SendExSellObject(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, sobj->objId, item, n, owner, 0);
                            d2_SetObjValue4(ctl, 0, 100, 0);
                        }
                    }
                }
            }
            d2_ClearPointerItems();
            for (k = 0; k < 8; k++)
                if (ai_ProdGoods2[k].check != -1)
                    d2_SetObjValue4(ai_ProdGoods2[k].check, 0, 0, 0);
            for (k = 0; k < 16; k++)
                if (ai_ProdGoods[k].check != -1)
                    d2_SetObjValue4(ai_ProdGoods[k].check, 0, 0, 0);
            di_ClearButtons();
        }

        if (clearBtn != -1) {
            if (changed == 0 ? (listShown != 0 && market == 0 && b->owner == ai_PlayerId) : listShown == 0) {
                d2_Objs[clearBtn].interactive = 1;
                d2_Objs[clearBtn].greyed = 0;
                d2_Objs[clearBtn].style = 3;
            } else {
                d2_Objs[clearBtn].interactive = 0;
                d2_Objs[clearBtn].greyed = 1;
            }
        }

        if (d2_ClickedId != 0) {
            /* 0x527 empties the picked goods back */
            if (d2_ClickedId == 0x527 && sv_MessageBox(txt_Ptrs[0xf9], 0x101) != 0) {
                if (listShown != 0) {
                    if (priced == 0) {
                        for (k = 0; k < 8; k++) {
                            if (ai_ProdGoods2[k].check == -1)
                                continue;
                            v = d2_GetObjValue(ai_ProdGoods2[k].check);
                            if (v > 0) {
                                cm_SendExSellObject(-1, stall->objId, ai_ProdGoods2[k].good, v, ai_OwnPlayer, 0);
                                d2_SetObjValue4(ai_ProdGoods2[k].check, 0, ai_ProdGoods2[k].amount, 0);
                            } else if (v < 0) {
                                cm_SendRemoveGoods(stall->objId, ai_ProdGoods2[k].good);
                            }
                        }
                        for (k = 0; k < 16; k++) {
                            if (ai_ProdGoods[k].check == -1)
                                continue;
                            v = d2_GetObjValue(ai_ProdGoods[k].check);
                            if (v > 0) {
                                cm_SendExSellObject(-1, stall->objId, ai_ProdGoods[k].good, v, ai_OwnPlayer, 0);
                                d2_SetObjValue4(ai_ProdGoods[k].check, 0, ai_ProdGoods[k].amount, 0);
                            } else if (v < 0) {
                                cm_SendRemoveGoods(stall->objId, ai_ProdGoods[k].good);
                            }
                        }
                    }
                } else if (changed != 0) {
                    for (i = 0; i <= 15; i++) {
                        c = rows[i].cart;
                        if (c == 0)
                            continue;
                        sobj = (AIOBJ *)gm_Query((void *)c->list, 1, 0, 0x1dd);
                        for (j = 0; j < 3; j++) {
                            ctl = rows[i].goodsCtl[j];
                            if (ctl == -1)
                                continue;
                            item = d2_Objs[ctl].pic - 0xdf;
                            v = d2_GetObjValue(ctl);
                            if (v > 0) {
                                cm_SendExSellObject(-1, sobj->objId, item, v, ai_OwnPlayer, 0);
                                d2_SetObjValue4(ctl, 0, goods.amount[i], 0);
                            }
                        }
                    }
                }
            }
        } else if (d2_PointerObj == btn0) {
            /* send: pick the destination on the map and order the selected carts there */
            memset(&cfg, 0, sizeof(cfg));
            cfg.mode = 0x400;
            cfg.kind = 0;
            cfg.filter = 0x40000001f07d7fb0;
            d2_SetViewEnabled(win, 0);
            n = 0;
            for (i = 0; i < 16; i++)
                if (rows[i].cart != 0 && rows[i].selected != 0)
                    pickCarts[n++] = rows[i].cart;
            txt_Format(text1, 0xce, n, 0xc10);
            dest = (AIBLD *)ai_MapPick(0x11, &cfg, text1, 0, 0);
            if (dest != 0 && dest != bld) {
                fee = 0;
                if (gm_IsHomeType(dest) == 0 && gm_IsHomeType(bld) == 0) {
                    for (i = 0; i <= 15; i++) {
                        c = rows[i].cart;
                        if (c == 0 || rows[i].selected == 0)
                            continue;
                        if (dest->type == 0x47 && c->id != 0x136) {
                            txt_Format(text1, 0xe9);
                            sv_MessageBox(text1, 0x100);
                            continue;
                        }
                        home = 0;
                        if (ai_PlayerBuilding != 0)
                            home = (AIBLD *)ai_PlayerBuilding;
                        else if (ai_FallbackBuilding != 0)
                            home = (AIBLD *)ai_FallbackBuilding;
                        if (home == 0 || (((AIBTYPE *)ai_BTypes)[home->type].kind != 0x1e
                                          && ((AIBTYPE *)ai_BTypes)[home->type].kind != 0x1d))
                            snd_PlayFavourComment();
                        ai_TransportOrderDue(c);
                        fee += ai_SendTransportOrder(ai_PlayerId, c, bld, dest);
                    }
                } else {
                    for (i = 0; i <= 15; i++) {
                        c = rows[i].cart;
                        if (c == 0 || rows[i].selected == 0)
                            continue;
                        if (dest->type == 0x47 && c->id != 0x136) {
                            txt_Format(text1, 0xe9);
                            sv_MessageBox(text1, 0x100);
                            continue;
                        }
                        home = 0;
                        if (ai_PlayerBuilding != 0)
                            home = (AIBLD *)ai_PlayerBuilding;
                        else if (ai_FallbackBuilding != 0)
                            home = (AIBLD *)ai_FallbackBuilding;
                        if (home == 0 || (((AIBTYPE *)ai_BTypes)[home->type].kind != 0x1e
                                          && ((AIBTYPE *)ai_BTypes)[home->type].kind != 0x1d))
                            snd_PlayFavourComment();
                        ai_TransportOrderDue(c);
                        fee += ai_SendTransportOrder(ai_PlayerId, c, bld, dest);
                    }
                }
                if (fee > 0) {
                    txt_Format(text1, 0xe3, fee);
                    sv_SetStatusText(text1);
                }
            }
            d2_SetViewEnabled(win, 1);
        } else if (d2_PointerObj == btn1 && (ai_GameFlags & 0x80) == 0) {
            /* repair: refill the selected carts, 0.008 of the cart's price per missing percent, 4 minutes each */
            n = 0;
            cost = 0;
            maxEmpty = 0;
            for (i = 0; i <= 15; i++) {
                c = rows[i].cart;
                if (c == 0 || rows[i].selected == 0)
                    continue;
                unit = gm_ItemPrice(c->id, 100) * 0.008f;
                empty = 100 - c->fill;
                if (empty > maxEmpty)
                    maxEmpty = empty;
                cost += empty * unit;
                n++;
            }
            if (n != 0) {
                due = ai_Time;
                gm_AddTime(&due, 0, maxEmpty * 4, 0);
                gm_ClockToDate(&due, (unsigned char *)&date);
                txt_Format(text3, 0xdd, cost, n, date.day, date.month, date.year);
                if (sv_MessageBox(text3, 0x101) != 0
                    && sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer) != 0) {
                    cm_SendMoveGoods(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, owner, cost);
                    for (i = 0; i <= 15; i++) {
                        c = rows[i].cart;
                        if (c == 0 || rows[i].selected == 0)
                            continue;
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 0x10;
                        msg.flags = 1;
                        msg.fromId = bld->id;
                        msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                        msg.time = ai_Time;
                        gm_AddTime(&msg.time, 0, 0, 10);
                        msg.car = c->objId;
                        msg.dst[0] = 4;
                        msg.u.s.cost = 1;
                        msg.n = 100 - c->fill;
                        cm_SendLongMessage((char *)&msg);
                    }
                }
            }
        } else if (d2_PointerObj == btn2 && (ai_GameFlags & 0x80) == 0) {
            /* buy a cart */
            d2_SetViewEnabled(win, 0);
            ai_BuyCartWindow(b);
            d2_SetViewEnabled(win, 1);
            if (goodsRec != 0) {
                if (*goodsRec == 5) {
                    listShown = ai_UpdateProdGoodsList(b, stall);
                    ai_LayoutProdGoodsList(win, 1, 1, b);
                } else if (*goodsRec == 0x11) {
                    listShown = ai_UpdateStoreGoods(b, stall);
                    ai_LayoutStoreGoods(win, 1, 1, b, stall);
                }
            }
        } else if (d2_PointerObj == btn3 && (ai_GameFlags & 0x80) == 0) {
            /* sell the selected carts */
            n = 0;
            for (i = 0; i <= 15; i++) {
                if (n >= 0x20)
                    break;
                if (rows[i].cart != 0 && rows[i].selected != 0)
                    sellCarts[n++] = rows[i].cart;
            }
            ai_SellCarts(n, sellCarts);
        }

        if (b->owner != ai_PlayerId) {
            if (btn1 != -1)
                d2_EnableObject(btn1, 0);
            if (btn2 != -1)
                d2_EnableObject(btn2, 0);
            if (btn3 != -1)
                d2_EnableObject(btn3, 0);
        }
        if (d2_GetObjValue(routeBox) != 0) {
            d2_EnableObject(d2_Wins[ai_ChildWindowId].parent, 1);
            d2_SetForm(win, 2);
            d2_EnableObject(d2_Wins[d2_LastWindow].parent, 0);
        } else {
            d2_EnableObject(d2_Wins[ai_ChildWindowId].parent, 0);
            d2_SetForm(win, 2);
            d2_EnableObject(d2_Wins[d2_LastWindow].parent, 1);
        }
        if (canTrade != 0 && listShown != 0)
            ai_TransportGoodsTotal(b, factor, mode);
        else
            sv_PendingMoney = 0;
    } while (sv_RunFrame((int)ai_TransportWindow, 0x657c7) != 0);
    ai_CloseChildWindow();
    d2_ClearPointerItems();
    d2_CloseView(win);
    sv_PendingMoney = 0;
    if (mode8) {
        d2_CloseView(panelForm);
        sv_PanelShown = (unsigned short *)picked;
    }
    ai_CampaignStep(0x23, 0, 1, 0);
    return 0;
}
/* 0x5b2090, loads the town map picture "<gfx>bmp\maps\<player slot name>.bmp" of the own player (235 B,
   main_GameLoop): its pixels are converted into a new surface rt_MapSurface of the bitmap's size (d2_MakeSurface
   flags 0xb, d2_ConvertRgbTo16), ai_MapSize / ai_MapHeight take the size and ai_MapCellScale the pixels per
   map cell: 7.5, 5.33 for a map narrower than 0x780; the bitmap is freed */

void ai_LoadTownMap(void)
{
    char path[0x104];
    D2SURF *bmp;

    sprintf(path, "%sbmp\\maps\\%s.bmp", d2_GfxDir, ai_PlayerSlots[ai_OwnPlayer]);
    bmp = d2_LoadBmpSurface(path);
    if (bmp != 0) {
        D2SURF desc;

        memset(&desc, 0, sizeof(desc));
        desc.w = bmp->w;
        desc.h = bmp->h;
        rt_MapSurface = d2_MakeSurface(&desc, 0xb);
        d2_LockSurface(rt_MapSurface);
        d2_ConvertRgbTo16(0, 0, bmp->w, bmp->h, (unsigned char *)bmp->bits, 0, 0, rt_MapSurface);
        ai_MapSize = bmp->w;
        ai_MapHeight = bmp->h;
        if (ai_MapSize >= 0x780)
            ai_MapCellScale = 7.5f;
        else
            ai_MapCellScale = 5.33f;
        d2_UnlockSurface(rt_MapSurface);
        d2_FreeSurface(bmp);
    }
}
/* 0x5b2180, the town map pick (936 B): opens the map window "Misc\Landkarte2" over its record (the request req
   copied in as its filter, flags, the title name or ch_EmptyName) and runs it until the pick: flags 4 offers the
   ambush spots (a click on a spot control hands its object to cb, or ends the window without one), flags 8 the
   building placement sites (a click picks the ai_PlacementSites entry and ends the window), and a click on a
   building mark picks the building (flags 1) or runs ai_LawSelectionEntry on it (flags 2, which calls cb
   itself); with flags 1 the picked building goes to cb (with the window disabled meanwhile), without a cb the
   pick ends the window. The right button closes it. Returns the pick (0: none) after the campaign step 0xd
   and ai_CloseMapWindow. The record is 0x27ac bytes in the original's 0x27b0-byte frame (ai_OpenTownMap stores
   the background tile at +0x27a8). The fifth argument is not read */

int ai_MapPick(unsigned int flags, int *req, char *name, void (*cb)(), int unused)
{
    LCMAPWIN map;
    int picked;
    int i;

    picked = 0;
    memset(map.req, 0, sizeof(map.req));
    if (req != 0)
        memcpy(map.req, req, sizeof(map.req));
    map.flags = flags;
    if (name != 0)
        strcpy(map.name, name);
    else
        map.name[0] = ch_EmptyName[0];
    ai_OpenTownMap(&map);
    ai_BuildTownMap(&map);
    while (sv_RunFrame((int)ai_MapPick, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        ai_MapScrollEnabled = d2_FocusedInput == 0;
        ai_UpdateTownMap(&map);
        if (flags & 4) {
            if (dm_L.lclick != 0 && d2_PointerObj != -1) {
                for (i = 0; i < 0x10; i++) {
                    if (map.spotCtrls[i] != -1 && d2_PointerObj == map.spotCtrls[i]) {
                        picked = (int)d2_Objs[map.spotCtrls[i]].user;
                        if (picked != 0 && cb != 0) {
                            d2_SetViewEnabled(map.view, 0);
                            d2_EnableObject(d2_Wins[map.screen].parent, 0);
                            ai_MapPickCb(cb)((void *)picked, (void *)picked);
                            d2_SetViewEnabled(map.view, 1);
                            d2_EnableObject(d2_Wins[map.screen].parent, 1);
                        } else {
                            sv_PopRequest = 1;
                        }
                        break;
                    }
                }
            }
        } else if ((flags & 8) && dm_L.lclick != 0 && d2_PointerObj != -1) {
            for (i = 0; i < 0x100; i++) {
                if (map.siteCtrls[i] != -1 && map.siteCtrls[i] == d2_PointerObj) {
                    picked = ai_PlacementSites[i];
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
        for (i = 0; i < map.nMarks; i++) {
            if (d2_PointerObj == -1 || d2_PointerObj != map.marks[i].ctrl || dm_L.lclick == 0)
                continue;
            picked = 0;
            if (flags & 1) {
                picked = (int)map.marks[i].bld;
            } else if (flags & 2) {
                d2_SetViewEnabled(map.view, 0);
                d2_EnableObject(d2_Wins[map.screen].parent, 0);
                picked = ai_LawSelectionEntry((unsigned char *)map.marks[i].bld, req, (int)name, cb);
                d2_EnableObject(d2_Wins[map.screen].parent, 1);
                d2_SetViewEnabled(map.view, 1);
            } else {
                continue;
            }
            if (picked == 0)
                continue;
            if (cb == 0) {
                sv_PopRequest = 1;
                break;
            }
            if (flags & 2)
                continue;
            d2_SetViewEnabled(map.view, 0);
            d2_EnableObject(d2_Wins[map.screen].parent, 0);
            ai_MapPickCb(cb)((void *)picked, (void *)map.marks[i].bld);
            d2_SetViewEnabled(map.view, 1);
            d2_EnableObject(d2_Wins[map.screen].parent, 1);
            break;
        }
    }
    ai_CampaignStep(0xd, picked, 1, 0);
    ai_CloseMapWindow((int *)&map);
    return picked;
}
/* 0x5b2530, the new storage slot window "Handel\Handel_Lager_Neuer_Slot" of the store obj in the building bld
   (384 B, ai_StorageWindow): the caption 0x10b; a store of type 0x116 offers one upgrade (0x10e) while level is
   below the type's max116, else the text 0x111; the others offer the first slot (0x10c) while level is below
   min0 (else 0x110) and the second (0x10d) while level2 is below min1 (else 0x111); the closing text 0x10f.
   Returns 1 for the first offer, 2 for the second, 0 when closed (the button 1 or the right button) */

int ai_NewStorageSlotWindow(AIOBJ *obj, AIBLD *bld, int level, int level2)
{
    int r;
    int first;
    int second;
    int win;
    AIBTYPE *bt;

    r = 0;
    second = -1;
    first = -1;
    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    win = d2_OpenForm(0, 0, "Handel\\Handel_Lager_Neuer_Slot");
    d2_CentreViewWindows(win);
    io_PrintText(0x10b);
    if (obj->id != 0x116) {
        if (level >= bt->min0)
            io_PrintText(0x110);
        else
            first = d2_GetWinObj(win, 0, io_PrintText(0x10c, 0x1f40));
        if (level2 >= bt->min1)
            io_PrintText(0x111);
        else
            second = d2_GetWinObj(win, 0, io_PrintText(0x10d, 0x1f40));
    } else if (level >= bt->max116) {
        io_PrintText(0x111);
    } else {
        first = d2_GetWinObj(win, 0, io_PrintText(0x10e, 0x1f40));
    }
    io_PrintText(0x10f, 1);
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == first) {
                r = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == second) {
                r = 2;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_NewStorageSlotWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return r;
}
/* 0x5b26b0, the store options window "Handel\Handel_Lageroptionen" of the building bld (2685 B; the contact
   listeners of the production buildings, kontors and stores): the store is the list's object of type 0x2a or
   0x116 (the list: the given one, else the list of the room object 2 of the building, else its objList), else
   the building's own 0x2a. Each frame it shows the store's two slot levels (lvl1c / lvl1d) against the type's
   limits (min0 / min1, max116 for a 0x116 store) with the slot orders running (event 0x12: slotOrder1c /
   slotOrder1d), the price 0x1f40 and the store's capacity level (count, text 0x107 + it, the price 0x3200).
   The first button opens ai_NewStorageSlotWindow: a bought slot (0x1f40, cm_SendMoveGoods with the slot wait)
   counts into the player record (+0x50) and the running order, or posts the order message 0x12 (n = 1: the
   first slot, ch = 1: the second); the second button buys the capacity upgrade (0x3200, message 0x13) after
   the confirmation 0x113. The buttons grey out at the limits; at capacity 3 with both slots full the text
   0x112 replaces them. The tutorial steps 0x1d frame the window
   @deep size queue s16: the three slot / capacity message fills in one order, type, to, fromId, flags, n,
   (ch,) time, car (a 3000-sample permutation of msg2's fill, then the same order for msg1 / msg3, which
   climb.py's move round pointed at): 200 -> 117 ins; raw 2693 -> 2695 (padded 2704 vs the original's 2688:
   the size moves away, the 64 slot hunks of the frame are left). prior: Reconstructed whole from the stub
   against the original (759 instructions read end to end): the store lookup (0x2a / 0x116 via the list or
   the room object 2), the window, the two slot-order events 0x12 and the capacity event 0x13, the level
   texts 0xfe..0x107, the slot purchase through ai_NewStorageSlotWindow (0x1f40, the cm_GetSlotState wait,
   the player record +0x50, message 0x12 n=1 / ch=1), the capacity upgrade (0x3200, confirmation 0x113,
   message 0x13), the button limits and the 0x112 replacement, the goods lists. 765 vs 759 instructions;
   frame equal (0x724: btns[8], text[0x400]). Left, allocation and layout only: the first event search
   keeps its cursor in eax and copies it to edi / [esp+0x34] after the loop in the original, ours walks it
   in edi with a store per step (0x5b2834); the three message builds store the same fields in another
   schedule (0x5b2b33, 0x5b2c96, 0x5b2e61); the origi... */

void ai_StorageWindow(AIBLD *bld, int list)
{
    AIBTYPE *bt;
    AIOBJ *store;
    AIOBJ *room;
    AIJOB *slots1;
    AIJOB *slots2;
    AIJOB *upgrade;
    AIJOB *ev;
    int *prec;
    int btns[8];
    int btn0, btn1;
    int win;
    int choice;
    int slot;
    int lim1, lim2;
    int level, level2;

    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (list == 0) {
        room = (AIOBJ *)gm_Query((void *)bld->objList, 1, 4, 2);
        if (room != 0)
            list = room->list;
        else
            list = bld->objList;
    }
    if ((store = (AIOBJ *)gm_Query((void *)list, 2, 0, 0x2a, 6)) == 0
        && (store = (AIOBJ *)gm_Query((void *)list, 2, 0, 0x116, 6)) == 0
        && (store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 0, 0x2a, 6)) == 0)
        return;
    ai_CampaignStep(0x1d, bld, 0, 0);
    win = d2_OpenForm(0, 0, "Handel\\Handel_Lageroptionen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 3);
    io_PrintText(0xfa);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x740);
    d2_SetForm(win, 4);
    sv_ButtonRow(d2_LastWindow, 0, btns, 2, txt_Ptrs[0xfb], txt_Ptrs[0xfc]);
    btn0 = btns[0];
    btn1 = btns[1];
    if (ai_GameFlags & 0x80)
        d2_SetObjEnabled(btn1, 0);
    ai_InitProductionTables();
    d2_SetPointerSprite(0, 0);
    do {
        AIMSG msg1;

        for (ev = (AIJOB *)he_FindEvent(2, 0, 0x12, 3, bld->id); ev != 0 && ev->slotOrder1c == 0;
             ev = (AIJOB *)he_NextEvent())
            ;
        slots1 = ev;
        for (slots2 = (AIJOB *)he_FindEvent(2, 0, 0x12, 3, bld->id); slots2 != 0 && slots2->slotOrder1d == 0;
             slots2 = (AIJOB *)he_NextEvent())
            ;
        upgrade = (AIJOB *)he_FindEvent(2, 0, 0x13, 3, bld->id);
        d2_SetForm(win, 2);
        if (store->id != 0x116) {
            io_PrintText(0xfe, store->lvl1c, bt->min0);
            io_PrintText(0xff, 0x1f40);
            if (slots1 != 0)
                io_PrintText(0x100, slots1->slotOrder1c);
            else
                io_PrintText(0x101);
            io_PrintText(0x102, store->lvl1d, bt->min1);
            io_PrintText(0xff, 0x1f40);
            if (slots2 != 0)
                io_PrintText(0x100, slots2->slotOrder1d);
            else
                io_PrintText(0x101);
        } else {
            io_PrintText(0x104, store->lvl1c, bt->max116);
            io_PrintText(0x105, 0x1f40);
            if (slots1 != 0)
                io_PrintText(0x106, slots1->slotOrder1c);
            else
                io_PrintText(0x107);
        }
        io_PrintText(0x103, store->count + 0x107, 0x3200);
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == btn0) {
                level = store->lvl1c;
                if (slots1 != 0)
                    level += slots1->slotOrder1c;
                level2 = store->lvl1d;
                if (slots2 != 0)
                    level2 += slots2->slotOrder1d;
                choice = ai_NewStorageSlotWindow(store, bld, level, level2);
                gm_SumListAmounts((void *)((AICHARREC *)ai_Chars)[ai_PlayerId].inventory, ai_OwnPlayer);
                if (choice == 1) {
                    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], 0x1f40, ai_OwnPlayer) != 0) {
                        slot = cm_SendMoveGoods(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, 0x1f40);
                        while (cm_GetSlotState(slot) == 0)
                            sv_PumpNet();
                        if (cm_GetSlotState(slot) == 1) {
                            prec = (int *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                            if (prec != 0)
                                prec[0x14] += 0x1f40;
                            if (slots1 != 0) {
                                slots1->slotOrder1c++;
                            } else {
                                memset(&msg1, 0, sizeof(msg1));
                                msg1.type = 0x12;
                                msg1.to = ((AICHARREC *)ai_Chars)[bld->owner].netId;
                                msg1.fromId = bld->id;
                                msg1.flags = 1;
                                msg1.n = 1;
                                msg1.ch = 0;
                                msg1.time = ai_Time;
                                msg1.car = store->objId;
                                cm_SendLongMessage((char *)&msg1);
                            }
                        }
                    }
                } else if (choice == 2) {
                    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], 0x1f40, ai_OwnPlayer) != 0) {
                        slot = cm_SendMoveGoods(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, 0x1f40);
                        while (cm_GetSlotState(slot) == 0)
                            sv_PumpNet();
                        if (cm_GetSlotState(slot) == 1) {
                            prec = (int *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                            if (prec != 0)
                                prec[0x14] += 0x1f40;
                            if (slots2 != 0) {
                                slots2->slotOrder1d++;
                            } else {
                                AIMSG msg2;

                                memset(&msg2, 0, sizeof(msg2));
                                msg2.type = 0x12;
                                msg2.to = ((AICHARREC *)ai_Chars)[bld->owner].netId;
                                msg2.fromId = bld->id;
                                msg2.flags = 1;
                                msg2.n = 0;
                                msg2.ch = 1;
                                msg2.time = ai_Time;
                                msg2.car = store->objId;
                                cm_SendLongMessage((char *)&msg2);
                            }
                        }
                    }
                }
            } else if (d2_PointerObj == btn1 && (ai_GameFlags & 0x80) == 0) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], 0x3200, ai_OwnPlayer) != 0) {
                    char text[0x400];

                    txt_Format(text, 0x113, 0x3200);
                    if (sv_MessageBox(text, 0x101) != 0) {
                        slot = cm_SendMoveGoods(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, 0x3200);
                        while (cm_GetSlotState(slot) == 0)
                            sv_PumpNet();
                        if (cm_GetSlotState(slot) == 1) {
                            prec = (int *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                            if (prec != 0)
                                prec[0x14] += 0x3200;
                            if (upgrade != 0) {
                                upgrade->capacityOrder++;
                            } else {
                                AIMSG msg3;

                                memset(&msg3, 0, sizeof(msg3));
                                msg3.type = 0x13;
                                msg3.to = ((AICHARREC *)ai_Chars)[bld->owner].netId;
                                msg3.fromId = bld->id;
                                msg3.flags = 1;
                                msg3.n = 1;
                                msg3.time = ai_Time;
                                msg3.car = store->objId;
                                cm_SendLongMessage((char *)&msg3);
                            }
                        }
                    }
                }
            }
        }
        if (store->id != 0x116) {
            lim1 = bt->min0;
            if (slots1 != 0)
                lim1 -= slots1->slotOrder1c;
            lim2 = bt->min1;
            if (slots2 != 0)
                lim2 -= slots2->slotOrder1d;
            if (store->lvl1c >= lim1 && store->lvl1d >= lim2 && btn0 != -1) {
                d2_Objs[btn0].greyed = 1;
                d2_Objs[btn0].clickDisabled = 1;
            }
        } else {
            lim2 = 0;
            lim1 = bt->max116;
            if (slots1 != 0)
                lim1 -= slots1->slotOrder1c;
            if (store->lvl1c >= lim1 && btn0 != -1) {
                d2_Objs[btn0].greyed = 1;
                d2_Objs[btn0].clickDisabled = 1;
            }
        }
        if (store->count >= 3 || upgrade != 0) {
            if (btn1 != -1) {
                d2_Objs[btn1].greyed = 1;
                d2_Objs[btn1].clickDisabled = 1;
                if (upgrade == 0)
                    d2_SetCaption(btn1, txt_Ptrs[0xfc]);
                else
                    d2_SetCaption(btn1, txt_Ptrs[0xfd]);
            }
        } else if (btn1 != -1) {
            d2_Objs[btn1].greyed = 0;
            d2_Objs[btn1].clickDisabled = 0;
            d2_SetCaption(btn1, txt_Ptrs[0xfc]);
        }
        if (store->count >= 3 && store->lvl1c >= lim1 && store->lvl1d >= lim2) {
            if (btn1 != -1)
                d2_EnableObject(btn1, 0);
            d2_SetForm(win, 4);
            io_PrintText(0x112);
            btn1 = -1;
            btn0 = -1;
        }
        if (store->id == 0x116) {
            ai_UpdateStoreGoods(bld, store);
            ai_LayoutStoreGoods(win, 1, 1, bld, (unsigned short *)store);
        } else {
            ai_UpdateProdGoodsList(bld, store);
            ai_LayoutProdGoodsList(win, 1, 0, (int)bld);
        }
        if (dm_L.rup != 0 && ai_RunAllowsWindow() == 1)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_StorageWindow, 0x657c7) != 0);
    sv_CameraFollow();
    d2_CloseView(win);
    ai_CampaignStep(0x1d, bld, 1, 0);
}
/* 0x5b3130, the talent window "Special\Talente" (the robber / rhetoric / tavern contact listeners): the player's
   talent `talent` (0 .. 4, AICHARREC.skills) with its picture (pics + level / 42, at most 5) and value bar, and the
   offer to train it for a price by its level (1 / 2 / 4 / 6 / 9 / 12, one less when the order 0x3a / 0x1a / 0x17 /
   0x18 / 0x19 of the talent reports the flag 2); a running training event 0x60 of the player shows its progress
   instead, a talent of 252 or more cannot be trained. OK pays the price (cm_SendMoneyDelta) and sends the
   training message 0x60 (AIMSG.training, as am.c's ai_ExecuteTrainSkill) */

void ai_TalentWindow(struct AIBLD *bld, int room, int talent)
{
    AIMSG msg;
    int pics[5];
    int orders[5];
    int discount;
    int canTrain;
    int form;
    int bar;
    int w;
    int text;
    unsigned char price;
    float level;
    float total;
    AIJOB *ev;
    int arg[0x88];

    pics[0] = 0x25e;
    pics[1] = 0x264;
    pics[2] = 0x26a;
    pics[3] = 0x270;
    pics[4] = 0x276;
    ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x60);
    canTrain = 0;
    memset(arg, 0, sizeof(arg));
    orders[0] = 0x3a;
    orders[1] = 0x1a;
    orders[2] = 0x17;
    orders[3] = 0x18;
    orders[4] = 0x19;
    arg[0] = 1;
    if (talent >= 5)
        return;
    discount = (ord_DispatchOrder(orders[talent], ai_Chars[ai_PlayerId], arg) & 2) ? -1 : 0;
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] < 0x2a)
        price = 1;
    else if (((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] < 0x54)
        price = 2;
    else if (((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] < 0x7e)
        price = 4;
    else if (((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] < 0xa8)
        price = 6;
    else
        price = ((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] < 0xd2 ? 9 : 12;
    price += discount;
    form = d2_OpenForm(0, 0, "Special\\Talente");
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 1);
    d2_SetPointerSprite(0, 0);
    text = talent + 0x16b7;
    io_PrintText(0x16af, text);
    io_PrintText(talent + 0x16c3);
    d2_SetForm(form, 2);
    w = d2_Wins[d2_LastWindow].w;
    level = ((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] * 0.023809524f;
    if (!(level < 5.0f))
        level = 5.0f;
    bar = d2_WinOpenPic(w / 2 - 0x18, 4, (int)(pics[talent] + level), d2_LastWindow);
    sv_ValueBar(w / 2 - 0x23, 0x3c, ((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent], 0x4df);
    d2_SetForm(form, 3);
    if (ev != 0) {
        io_PrintText(0x16b3, ev->training.talent + 0x16b7, text);
    } else if (((AICHARREC *)ai_Chars)[ai_PlayerId].skills[talent] >= 0xfc) {
        io_PrintText(0x16b2);
    } else {
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].money >= price) {
            if (discount == 0) {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0x16b1, price, text);
                    break;
                case 1:
                    io_PrintText(0x16b1, price, text);
                    break;
                }
            } else {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0x16b0, (((AICHARREC *)ai_Chars)[ai_PlayerId].sex ? 0x27b : 0x253) + ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice,
                                 price, text);
                    break;
                case 1:
                    io_PrintText(0x16b0, (((AICHARREC *)ai_Chars)[ai_PlayerId].sex ? 0x27b : 0x253) + ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice,
                                 text, price);
                    break;
                }
            }
            canTrain = 1;
        } else {
            io_PrintText(0x16b4, price, text);
        }
    }
    do {
        if (ev != 0 && bar != -1) {
            AITIME due;

            due = ev->start;
            gm_AddTime((struct AITIME *)&due, 5, 0, 0);
            total = (float)gm_MinutesBetween(&ev->start, &ev->end);
            d2_Objs[bar].fillFraction = (float)gm_MinutesBetween(&ev->start, &ai_Time) / total;
        }
        if (dm_L.rup != 0 && d2_FocusedInput == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            if (canTrain) {
                cm_SendMoneyDelta(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -(int)price);
                msg.flags = 1;
                msg.type = 0x60;
                msg.training.talent = (unsigned char)talent;
                msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                msg.fromId = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                cm_SendLongMessage((char *)&msg);
                ai_CampaignStep(0x2c, 0, 0, talent);
                sv_PopRequest = 1;
            }
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_TalentWindow, 0x677c7) != 0);
    sv_CameraFollow();
    d2_CloseView(form);
}
#pragma pack(push, 1)
typedef struct AIWINDOWROW {            /* the row record the window page functions take (ai_BlackmailContactWindow builds it) */
    int win;                            /* 0x00, the window, -1 none */
    int pageA;                          /* 0x04, the page of the pictures */
    int pageB;                          /* 0x08, the page of the text */
    int pageC;                          /* 0x0c, the page of the balance bar */
    int entry;                          /* 0x10, the page entry index */
    int record;                         /* 0x14, the gv table record id */
} AIWINDOWROW;

typedef struct AIPOSVIEW {              /* 0x24 bytes, the record gv_GetTableRecord (0x4f2720) fills (AIPOS, ai.h) */
    char pad00[0xc];
    int pics;                           /* 0x0c, the page's picture count */
    char pad10[8];
    int texts;                          /* 0x18, the text count (AIPOS.unk18) */
    char pad1c[8];
} AIPOSVIEW;
#pragma pack(pop)

void ai_ShowWindowRow(AIWINDOWROW *row);

/* 0x5b3630, the sabotage window "misc\perga_rolle" (ai_RobberContact's map pick callback, with the picked mark's
   character ch and building target; nothing for none or an ownerless one). The price is 2 % of the player's worth
   plus 5 % of the building's equipment value (gm_EquipmentValue 100), halved when the order 0x1b reports the flag
   2; the player must have it (sv_CheckGoodAmount). A running sabotage event 0x19 of the player on the building
   gives the text 0x1702, an insult running from ch to the player (he_NoEvent4bPair) 0x1703, a quarrel between
   them (he_NoEvent2ePair) 0x1704, a building under protection (he_NoProtectionEvent) 0x1705 and a haunting of the
   player by ch (he_NoEvent88Pair) 0x409f. Else the window (texts 0x1700 / 0x1711: the price, the owner, the
   building kind's name; the row of the player's talent 2) asks: its button sends the sabotage message 0x19 (the
   building, the price; in the cm group "sabotage") and tells it with the text 0x1701, a right click or the
   button 1 close it. Returns 1 once the sabotage was ordered, else 0 */

int ai_SabotageCallback(char *ch, AIBLD *target)
{
    int ordered;
    int price;
    int win;
    int ok;
    AIJOB *ev;
    AIWINDOWROW row;
    union {
        AIMSG msg;
        char text[0x100];               /* a short note reuses the message's buffer */
    } buf;
    char note[0x80];
    char text[0x400];
    int arg[0x88];

    ordered = 0;
    if (target == 0 || target->owner == 0xffff)
        return 0;
    memset(arg, 0, sizeof(arg));
    arg[0] = 1;
    price = (int)(gm_GetWorth(((AICHARREC *)ai_Chars)[ai_PlayerId].id) * 0.02f +
                  gm_EquipmentValue((unsigned char *)target, 100) * 0.05f);
    if (ord_DispatchOrder(0x1b, ai_Chars[ai_PlayerId], arg) & 2)
        price /= 2;
    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) == 0)
        return 0;
    for (ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x19); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->bldId == target->id) {
            txt_Format(text, 0x1702, target->owner, target);
            sv_MessageBox(text, 0);
            return 0;
        }
    }
    if (he_NoEvent4bPair((int)ch, (int)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(text, 0x1703, target->owner, target);
        sv_MessageBox(text, 0);
        return 0;
    }
    if (he_NoEvent2ePair((int)ch, (int)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(text, 0x1704, ((AICHARREC *)ch)->id);
        sv_MessageBox(text, 0);
        return 0;
    }
    if (he_NoProtectionEvent((int)target) == 0) {
        txt_Format(buf.text, 0x1705, target);
        sv_MessageBox(buf.text, 0);
        return 0;
    }
    if (he_NoEvent88Pair((struct AICHAR *)ch, (struct AICHAR *)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(text, 0x409f, ((AICHARREC *)ch)->id);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    ok = d2_GetWinObj(win, 1, io_PrintText(0x1700, 0x1711, price, target->owner, target->type * 14 + 0x48e));
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.entry = 2;
    row.record = 0x18;
    ai_ShowWindowRow(&row);
    while (sv_RunFrame((int)ai_SabotageCallback, 0x677c7) != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0 && ok != -1 && d2_PointerObj == ok) {
            buf.msg.type = 0x19;
            buf.msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
            buf.msg.fromId = ((AIBLD *)ai_PlayerBuilding)->id;
            buf.msg.flags = 1;
            buf.msg.sabotage.price = price;
            buf.msg.sabotage.bld = target->id;
            cm_BeginGroup("sabotage");
            cm_SendLongMessage((char *)&buf.msg);
            cm_EndGroup("sabotage");
            txt_Format(note, 0x1701, target->masterId, target->type * 14 + 0x48e);
            sv_MessageBox(note, 0);
            sv_PopRequest = 1;
            ordered = 1;
        }
    }
    d2_CloseView(win);
    return ordered;
}
/* 0x5b3ad0, the espionage window "locations\\wohnsitz\\spy" (ai_EspionageAmountMenu's amount menu callback, with the
   picked character ch). An espionage event 0x18 of the player already on ch asks (text 0x42c1) to end it: yes
   sets its end time to now and sends it (cm_SendExSetHE). Five running ones give the text 0x42c0. Else the window:
   the caption 0x42be, a slider of three lengths (texts 0x42bb .. 0x42bd), and every frame the price 0x42b8 for ch:
   0.5 % of the player's worth plus 0.5 % of ch's, times 1 + 0.33 * the length. Its first button (0x42b9) sends
   the espionage message 0x18 (the player's building, ch, the price, the length; in the cm group "spionage") when
   the player has the price, tells it with the text 0x42bf and closes; the second button (0x42ba), a right click or
   an empty pick close it. Returns 1 once the espionage was ordered, else 0 */

int ai_EspionageAmountCB(AICHARREC *ch)
{
    int ordered;
    int running;
    int win;
    int caption;
    int slider;
    int base;
    int price;
    int buttons[2];
    float factor;
    float own;
    AIJOB *ev;
    AIMSG msg;
    char text[0x400];

    ordered = 0;
    running = 0;
    for (ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->id4 == ch->netId) {
            txt_Format(text, 0x42c1, ch->id);
            if (sv_MessageBox(text, 0x101) != 0) {
                ev->end = ai_Time;
                ev->exSlot = cm_SendExSetHEI(-1, ev);
            }
            return 0;
        }
        running++;
    }
    if (running >= 5) {
        txt_Format(text, 0x42c0, 5);
        sv_MessageBox(text, 0x104);
        return 0;
    }
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\spy");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 2);
    caption = d2_CreateWinText(0, 3, txt_Ptrs[0x42be], d2_LastWindow);
    d2_Objs[caption].centred = 1;
    d2_Objs[caption].w = d2_Wins[d2_LastWindow].w;
    d2_SetObjFont(caption, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 0xc8, 0, 100, 0x7b, 0x82, d2_LastWindow);
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, txt_Ptrs[0x42bb], txt_Ptrs[0x42bc], txt_Ptrs[0x42bd]);
    d2_SetForm(win, 3);
    sv_ButtonRow(d2_LastWindow, 0, buttons, 2, txt_Ptrs[0x42b9], txt_Ptrs[0x42ba]);
    while (sv_RunFrame((int)ai_EspionageAmountCB, 0x677c7) != 0) {
        if (dm_L.rup != 0 || ch->id == 0xffff) {
            sv_PopRequest = 1;
            continue;
        }
        factor = d2_GetObjValue(slider) * 0.33f + 1.0f;
        own = gm_GetWorth(((AICHARREC *)ai_Chars)[ai_PlayerId].id) * 0.005f;
        base = (int)(own + gm_GetWorth(ch->id) * 0.005f);
        price = (int)(base * factor);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText(0x42b8, price, ch->id);
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == buttons[0] && sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0) {
            msg.type = 0x18;
            msg.flags = 2;
            msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
            msg.fromId = -1;
            msg.espionage.target = ch->netId;
            msg.espionage.bld = ((AICHARREC *)ai_Chars)[ai_PlayerId].bld2 != 0 ?
                                ((AIBLD *)((AICHARREC *)ai_Chars)[ai_PlayerId].bld2)->id : -1;
            msg.espionage.price = price;
            cm_BeginGroup("spionage");
            msg.espionage.length = d2_GetObjValue(slider);
            cm_SendLongMessage((char *)&msg);
            cm_EndGroup("spionage");
            txt_Format(text, 0x42bf, ch->id);
            sv_MessageBox(text, 0x100);
            sv_PopRequest = 1;
            ordered = 1;
        } else if (d2_PointerObj == buttons[1]) {
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(win);
    sv_CameraFollow();
    return ordered;
}
/* 0x5b3f30, the beating window "misc\perga_rolle" (ai_RobberContact's amount menu callback, with the picked
   character ch). The price is 0.85 % of the player's worth, halved when the order 0x1b reports the flag 2. A
   running beating event 0x1a of the player on ch gives the text 0x170a; the player must have the price
   (sv_CheckGoodAmount); an insult running from ch to the player (he_NoEvent4bPair) gives 0x170b, a quarrel between
   them (he_NoEvent2ePair) 0x170c, a haunting of the player by ch (he_NoEvent88Pair) 0x409f. Else the window
   (texts 0x1708 / 0x1711: the price, ch; the row of the player's talent 3) asks: its button sends the beat-up
   message 0x1a (ch, a random object of ch's, the price; in the cm group "beatup", as am.c's ai_ExecuteBeatUp) and
   tells it with the text 0x1709, a right click or the button 1 close it. Returns 1 once the beating was ordered,
   else 0 */

int ai_BeatingCallback(AICHARREC *ch)
{
    int ordered;
    int ok;
    int price;
    int win;
    AIJOB *ev;
    AIWINDOWROW row;
    union {
        AIMSG msg;
        char text[0x100];               /* the short notes reuse the message's buffer */
    } buf;
    char text[0x400];
    int arg[0x88];

    ok = -1;
    ordered = 0;
    memset(arg, 0, sizeof(arg));
    arg[0] = 1;
    price = (int)(gm_GetWorth(((AICHARREC *)ai_Chars)[ai_PlayerId].id) * 0.0085f);
    if (ord_DispatchOrder(0x1b, ai_Chars[ai_PlayerId], arg) & 2)
        price /= 2;
    for (ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x1a); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->id3 == ch->netId) {
            txt_Format(text, 0x170a, ch->id);
            sv_MessageBox(text, 4);
            return 0;
        }
    }
    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) == 0)
        return 0;
    if (he_NoEvent4bPair((int)ch, (int)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(buf.text, 0x170b, ch->id);
        sv_MessageBox(buf.text, 0);
        return 0;
    }
    if (he_NoEvent2ePair((int)ch, (int)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(buf.text, 0x170c, ch->id);
        sv_MessageBox(buf.text, 0);
        return 0;
    }
    if (he_NoEvent88Pair((struct AICHAR *)ch, (struct AICHAR *)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(buf.text, 0x409f);
        sv_MessageBox(buf.text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    d2_SetPointerSprite(0, 0);
    if (ch != 0) {
        d2_SetForm(win, 1);
        ok = d2_GetWinObj(win, 1, io_PrintText(0x1708, 0x1711, price, ch->id));
    }
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.entry = 3;
    row.record = 0x15;
    ai_ShowWindowRow(&row);
    while (sv_RunFrame((int)ai_BeatingCallback, 0x677c7) != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0 && ok != -1 && d2_PointerObj == ok) {
            buf.msg.type = 0x1a;
            buf.msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
            buf.msg.fromId = ((AIBLD *)ai_PlayerBuilding)->id;
            buf.msg.flags = 1;
            buf.msg.u.s.cost = price;
            buf.msg.ch = ch->netId;
            buf.msg.arg[0] = ord_RandomObjectId((char *)ch);
            cm_BeginGroup("beatup");
            cm_SendLongMessage((char *)&buf.msg);
            cm_EndGroup("beatup");
            txt_Format(text, 0x1709, ch->id);
            sv_MessageBox(text, 0);
            sv_PopRequest = 1;
            ordered = 1;
        }
    }
    d2_CloseView(win);
    return ordered;
}
int gv_HeldCrimesOf(int culprit, int *out, unsigned short t);  /* 0x4f3fe0, gv.h's prototype (gv.h is not included yet: Open: head) */
void gv_SortCrimesByLaw(int n, CRIME *r);  /* 0x4f4390, gv.h's prototype (gv.h is not included yet: Open: head) */

/* 0x5b4340, the evidence details window "privillegien\Beweise_Details" (ai_EvidenceBookWindow, for the player's
   character me and a character of the book): the crimes of culprit that me holds (gv_HeldCrimesOf, up to 32),
   copied from ai_Crimes and sorted by law (gv_SortCrimesByLaw), each a row of its law's name (txt_Ptrs[law * 5 +
   0x13fd]) and the law's pictures (gv_GetTableRecord); the caption 0x1742 names the culprit, the text 0x174a
   follows when the order 0x16 reports the flag 2 for the culprit. A right click closes it. Nothing for an empty
   culprit or none held */

void ai_EvidenceDetailsWindow(AICHARREC *me, AICHARREC *culprit)
{
    int n;
    int i;
    int k;
    int win;
    int list;
    AIPOSVIEW pos;
    int ids[0x20];
    int arg[0x88];
    CRIME crimes[0x20];

    if (culprit->id == 0xffff)
        return;
    n = gv_HeldCrimesOf(culprit->netId, ids, me->id);
    if (n < 1)
        return;
    for (i = 0; i < n; i++)
        crimes[i] = ((CRIME *)ai_Crimes)[ids[i]];
    gv_SortCrimesByLaw(n, crimes);
    win = d2_OpenForm(0, 0, "privillegien\\Beweise_Details");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    list = d2_LastWindow;
    d2_SetForm(win, 4);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, list, 0x767);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1742, culprit->id);
    d2_SetForm(win, 2);
    for (i = 0; i < n; i++) {
        ai_LeftAlignedText(txt_Ptrs[crimes[i].law * 5 + 0x13fd], 10, i * 0x28 + 10, 0x13b, 0x55);
        if (gv_GetTableRecord(crimes[i].law, (int *)&pos) != 0 && pos.pics != 0)
            for (k = 0; k < pos.pics; k++)
                d2_WinOpenPic(k * 0x10 + 0x154, i * 0x28 + 10, 0x526, d2_LastWindow);
    }
    memset(arg, 0, sizeof(arg));
    arg[0] = 1;
    d2_SetForm(win, 3);
    if (ord_DispatchOrder(0x16, culprit, arg) & 2)
        io_PrintText(0x174a);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_EvidenceDetailsWindow, 0x657c7) != 0);
    d2_CloseView(win);
}
int gv_RankCulpritsAll(int *ids, int *open, int *total, unsigned short t);  /* 0x4f3dd0, gv.h's prototype (gv.h is not included yet: Open: head) */
void ai_RightAlignedText(char *text, int x, int y, int w, int font);

/* 0x5b4590, the evidence book "privillegien\Beweise_Sichten" (the study's plate, ai_StudyContact): the caption 0x173f,
   then one amount row per culprit the player holds evidence of (gv_RankCulpritsAll, up to 32; gm_FindChar), with
   "%i (%i) %s": the evidence count, the open ones and the word 0x1748 / 0x1749 (one / more); none gives the text
   0x1741. A clicked row opens its culprit's details (ai_EvidenceDetailsWindow); a right click closes the book and
   the camera follows. bld (the study) is not read */

void ai_EvidenceBookWindow(AIBLD *bld)
{
    int n;
    int count;
    int i;
    int win;
    int list;
    int sel;
    AICHARREC *me;
    AICHARREC *ch;
    char text[0x40];
    int ids[0x20];
    int open[0x20];
    int total[0x20];
    LCAMOUNTROW rows[0x20];

    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    win = d2_OpenForm(0, 0, "privillegien\\Beweise_Sichten");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x173f);
    d2_SetForm(win, 2);
    list = d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, list, 0x767);
    count = gv_RankCulpritsAll(ids, open, total, me->id);
    n = 0;
    for (i = 0; i < count; i++) {
        ch = (AICHARREC *)gm_FindChar(ids[i]);
        if (ch != 0) {
            rows[n].rec = ch;
            rows[n].x2c = total[i];
            rows[n].x34 = open[i];
            ai_AmountRowStatus(&rows[n]);
            rows[n].pickable = 1;
            n++;
        }
    }
    d2_SetForm(win, 2);
    if (n != 0) {
        for (i = 0; i < n; i++) {
            txt_Format(text, "%i (%i) %s", rows[i].x2c, rows[i].x34, (rows[i].x2c != 1) + 0x1748);
            rows[i].x = 0x32;
            rows[i].y = i * 0x69 + 10;
            ai_AmountRow(0x32, i * 0x69 + 10, d2_LastWindow, 0x56, &rows[i], 1);
            ai_RightAlignedText(text, d2_Wins[d2_LastWindow].w - 0xf, i * 0x69 + 10 + 0x2d, 200, 0x55);
        }
    } else {
        io_PrintText(0x1741, ai_PlayerId);
    }
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == rows[i].ctl[0]) {
                    sel = i;
                    break;
                }
            }
            if (sel > -1)
                ai_EvidenceDetailsWindow((AICHARREC *)ai_Chars[ai_PlayerId], rows[sel].rec);
        }
    } while (sv_RunFrame((int)ai_EvidenceBookWindow, 0x657c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
int ai_BribeAmountCB(AICHARREC *ch);

/* 0x5b4820, the bribe window "misc\perga_rolle" (ai_OpenRobberBribe's amount menu callback, with the picked
   character ch): the text 0x174d (ch, the player's currency) with a money slider (up to 0.078 % of the two
   characters' worth together, ai_AmountLimit against the player's goods value, in the player's currency) and the
   give button, the text 0x174e with the cancel button, the row of the player's talent 2. Give, when the player
   has the amount: it goes on the player's record (LCFIN.spyPaid) and to ch (cm_SendMoveGoods), the relation of ch
   to the player rises by amount * (6 with the flag 0x800 of the player's flags1cc, else 4) * 500 / the two worths
   (cm_SendRelations, the slot waited for), a player character ch (kind 6 / 7) gets the news 0x1751, and the
   bribe is a crime (gv_Accuse law 0x10). Returns 1 once the bribe was given, else 0
   @deep Reconstructed whole against the original 0x5b4820..0x5b4be8 (bribe window: slider, give/cancel,
   LCFIN.spyPaid, cm_SendMoveGoods, cm_SendRelations with the slot wait, the 0x1751 news for kinds 6/7,
   gv_Accuse law 0x10): every callee, argument, constant and store matches, 293 vs 293 instructions. Left
   only the evaluation order of the two gm_GetWorth calls in the relation divisor amount * (flags1cc &
   0x800 ? 6 : 4) * 500 / (gm_GetWorth(ai_PlayerId) + gm_GetWorth(ch->id)): the original calls the player's
   first (+0x288 'mov cx,[ai_PlayerId]' CSE'd from the flags1cc index, push ecx, call; then 'mov ax,[esi];
   push eax; call'), ours calls ch's first (+0x284 'mov ax,[esi]; push eax; call', then 'mov
   cx,[ai_PlayerId]; push ecx; call'); the rest are the register renamings that follow (regs 54: eax->edx
   ecx->eax edx->ecx at +0x4ae6/+0x4b32/+0x4b84). Tried both operand orders (VC6 canonicalizes them), a
   player-worth temp before the expression (fixes the call order but then the 6/4 ternary goes to a memory
   slot: frame 0x434 vs 0x430), the ternary as if/else, double constants, a register float: the same or
   worse. The max computation got the original's order with the worth temp (byte there) */

int ai_BribeAmountCB(AICHARREC *ch)
{
    int given;
    int win;
    int text;
    int slider;
    int ok;
    int cancel;
    int max;
    int limit;
    int goods;
    int value;
    int amount;
    int slot;
    int relation;
    int worth;
    LCFIN *rec;
    AIWINDOWROW row;
    char news[0x400];

    given = 0;
    if (ch == 0)
        return 0;
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    text = io_PrintText(0x174d, ch->id, ai_OwnPlayer);
    slider = d2_GetWinObj(win, 1, text);
    ok = d2_GetWinObj(win, 1, text + 1);
    worth = gm_GetWorth(ai_PlayerId);
    max = (int)(abs(worth + gm_GetWorth(ch->id)) * 0.00078431371f);
    goods = gm_GetGoodsValue(ai_Chars[ai_PlayerId]);
    ai_AmountLimit(max, goods, 0x20, &limit);
    d2_SetObjValue4(slider, gm_ConvertMoney(max, ai_OwnPlayer), gm_ConvertMoney(limit, ai_OwnPlayer),
                   gm_ConvertMoney(max, ai_OwnPlayer));
    cancel = d2_GetWinObj(win, 1, io_PrintText(0x174e));
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.entry = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.record = 0x10;
    ai_ShowWindowRow(&row);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            value = d2_GetObjValue(slider);
            amount = gm_PlayerFactor(value, ai_OwnPlayer);
            if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], amount, ai_OwnPlayer) == 0)
                continue;
            rec = (LCFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
            if (rec != 0)
                rec->spyPaid += amount;
            cm_SendMoveGoods(ch->netId, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, amount);
            relation = (int)(amount * ((((AICHARREC *)ai_Chars)[ai_PlayerId].flags1cc & 0x800) ? 6.0f : 4.0f) * 500.0f /
                             (gm_GetWorth(ai_PlayerId) + gm_GetWorth(ch->id)));
            slot = cm_SendRelations(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ch->netId, relation, 1);
            while (cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            if (ch->kind == 6 || ch->kind == 7) {
                txt_Format(news, 0x1751, ai_PlayerId, amount);
                he_ShowNews(ch->netId, -1, news, 0, 0x5fb, "_NACHRICHTEN_HS_22");
            }
            gv_Accuse(0x10, 1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ch->netId, -1);
            sv_PopRequest = 1;
            given = 1;
        } else if (d2_PointerObj == cancel) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_BribeAmountCB, 0x677c7) != 0);
    d2_CloseView(win);
    return given;
}
int gv_HeldCrimeCount(int culprit, unsigned short t);  /* 0x4f4130, gv.h's prototype (gv.h is not included yet: Open: head) */

/* 0x5b4bf0, the bribe amount menu of the robbers' room (105 B, ai_RobberContact): the AMOUNTCFG with mode
   0x40400, the filter's high word from gypsyFilter (its low word stored 0) and kind 6, the prompt 0x174f, the menu with the callback ai_BribeAmountCB; the
   camera follows again */

void ai_OpenRobberBribe(int bld)
{
    AMOUNTCFG cfg;
    char text[0x100];

    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x40400;
    cfg.kind = 6;
    cfg.classMask[1] = gypsyFilterWords[1];
    cfg.classMask[0] = 0;
    txt_Format(text, 0x174f);
    ai_AmountMenu(&cfg, text, 0, ai_BribeAmountCB);
    sv_CameraFollow();
}
/* 0x5b4c60, the threatening letter window "misc\perga_rolle" (ai_DrohbriefWindow's amount menu callback, with the
   picked character ch; nothing for none, an empty pick, the player, or a player without 2 money). A haunting of
   the player by ch (he_NoEvent88Pair) gives the text 0x409f, no evidence of ch's crimes (gv_HeldCrimeCount) the
   text 0x1756. Else the window (text 0x1759: the evidence count with the word 0x1752 / 0x1753, ch; the row of
   the player's talent 4) asks: its button takes the 2 (cm_SendMoneyDelta) and, when a random number plus the
   mix of the two characters (gm_Mix 4) reaches 1, sends the quarrel message 0x2e from ch to the player (text
   0x175b), else tells the letter failed (0x175c); the second button or a right click close it. Returns 1 once
   the letter went out, else 0
   @deep s13 (size queue): the original stores sent = 0 to its slot [esp+0xc] in the first block (test esi;
   mov [esp+0xc],0; jne), ours sinks the store past the guards into the window path; with the store at the
   top VC6 gives every early return its own inline epilogue as the original: the probe { static int *probe;
   probe = &sent; } after sent = 0 removes all tail hunks (50 -> 34 ins, the rest the probe's own frame
   slot). So the open question is what keeps the original's zero store at the top (sent is memory-homed in
   both: sent = 1 stored from the eax shared with sv_PopRequest = 1). No lever (dcp try, = control or
   worse): return sent at the guards / the message exits / everywhere, int sent = 0 initializer, sent = 0
   after the first guard (52), char sent (62), the three id guards as one || (57). Next: what else reads or
   takes sent before the guards in the original (a second use, a call taking &sent), or sent as another
   variable's reuse. prior: Reconstructed whole against the original 0x5b4c60..0x5b4fab (threatening
   letter: the checks, he_NoEvent88Pair 0x409f, gv_HeldCrimeCount 0x1756, the window 0x1759 with the talent
   row 4, the 2 money, gm_Mix + util_RandomF >= 1 sending the quarrel message 0x2e (to ch, from -1, flags
   2, arg[0] ch, arg[1] the player) with 0x175b, else 0x175c): from +0xf1 on every instruction of the
   window, the loop and the message equals the original. Left only the block layout of the early exits: the
   original keeps a separate epilogue (xor eax,eax / pop edi / pop esi / pop ebx / add esp,0x51c / ret)
   after each of the 7 early returns and its own txt_Format/sv_MessageBox sequence for 0x409f and 0x1756,
   ours jumps to one shared epilogue at +0xf1 and cross-jumps the two message sequences (+0x96 'push
   0x409f; jmp +0xd2'); diffclass: tail 32, code 28 (+0x5f the sv_CheckMoney exit, +0x8d the merged message
   call), o... */

int ai_DrohbriefAmountCB(AICHARREC *ch)
{
    int sent;
    int n;
    int win;
    int text;
    int ok;
    int cancel;
    float chance;
    AICHARREC *me;
    AIWINDOWROW row;
    AIMSG msg;
    char note[0x400];

    sent = 0;
    if (ch == 0)
        return 0;
    if (ch->id == 0xffff)
        return 0;
    if (ch->id == ai_PlayerId)
        return 0;
    if (sv_CheckMoney(ai_Chars[ai_PlayerId], 2) == 0)
        return 0;
    if (he_NoEvent88Pair((struct AICHAR *)ch, (struct AICHAR *)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(note, 0x409f, ch->id);
        sv_MessageBox(note, 0);
        return 0;
    }
    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    n = gv_HeldCrimeCount(ch->netId, ai_PlayerId);
    if (n <= 0) {
        txt_Format(note, 0x1756, ch->id);
        sv_MessageBox(note, 0);
        return 0;
    }
    if (sv_CheckMoney(ai_Chars[ai_PlayerId], 2) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1759, n, (n != 1) + 0x1752, ch->id, ch->id);
    ok = d2_GetWinObj(win, 1, text);
    cancel = d2_GetWinObj(win, 1, text + 1);
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.entry = 4;
    row.record = 0x13;
    ai_ShowWindowRow(&row);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                cm_SendMoneyDelta(me->netId, -2);
                chance = gm_Mix((int)me, (int)ch, 4);
                if (util_RandomF() + chance >= 1.0f) {
                    msg.type = 0x2e;
                    msg.to = ch->netId;
                    msg.fromId = -1;
                    msg.flags = 2;
                    msg.arg[1] = me->netId;
                    msg.arg[0] = ch->netId;
                    cm_SendLongMessage((char *)&msg);
                    txt_Format(note, 0x175b, ch->id);
                } else {
                    txt_Format(note, 0x175c, ch->id, ch->id);
                }
                sv_MessageBox(note, 0);
                sv_PopRequest = 1;
                sent = 1;
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_DrohbriefAmountCB, 0x677c7) != 0);
    d2_CloseView(win);
    return sent;
}
/* 0x5b4fb0, the threatening-letter ("Drohbrief") window of the rhetoric contact 0x5965f0 (83 B): the amount
   menu 0x5c1130 with the config (mode 0x400, kind 6, the filter gypsyFilter, the rest cleared), the contact's
   text 0x1758 and the callback 0x5b4c60, whose result it returns; bld is not read */

int ai_DrohbriefWindow(int bld)
{
    AMOUNTCFG cfg;

    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    return ai_AmountMenu(&cfg, txt_Ptrs[0x1758], 0, (void *)ai_DrohbriefAmountCB);
}
/* 0x5b5010, the denounce window "misc\perga_rolle" (the ANSCHWAERZEN plate of ai_FeastContact; nothing for no
   building or a player without 2 money): the amount menu (mode 0x400, kind 6, the gypsy filter) picks the
   culprit (text 0x175f), a second one with the same config the character to denounce him to (0x1760). No
   evidence of the culprit's crimes held by the player (gv_HeldCrimeCount) gives the text 0x1756, a player
   without 3 money nothing; else the window (text 0x1761: the evidence count with the word 0x1752 / 0x1753, the
   two characters) asks: its button sends the denunciation message 0x2f to the second character (flags 9,
   arg[0] the culprit, arg[1] the player, arg[2] the receiver), takes the 3 (cm_SendMoneyDelta) and tells it
   (0x1762); the second button or a right click close it. The camera follows after the menus
   @deep Reconstructed whole against the original 0x5b5010..0x5b530b (both amount menus 0x175f / 0x1760
   with the one config, gv_HeldCrimeCount 0x1756, the 3-money check, the window 0x1761 with 0x1752+(n!=1),
   the message 0x2f flags 9 to/fromId/arg[2] the receiver, arg[0] the culprit, arg[1] the player,
   cm_SendMoneyDelta -3, 0x1762, the sv_CameraFollow tail calls); every instruction outside the message
   block equals the original. Left only the scheduling of the message stores: the original loads
   judge->netId into ecx and stores to/fromId/arg[2] first, then lea/push &msg, then the byte stores 0x2f /
   9 and arg[1] / arg[0]; ours hoists the constant byte stores (mov byte [esp+0xac],0x2f first, flags 9
   next) and pushes &msg late (asmdiff +0x21a..+0x26c), and the cm_SendMoneyDelta / 0x1762 block after it
   takes edx/eax for ecx/edx (regs). Tried the store orders: chained to=fromId=arg[2] both ways, type/flags
   first or last, arg[0] before arg[1], fromId=msg.to copies: none moves the push or the byte stores */

void ai_DenounceWindow(AIBLD *bld)
{
    int n;
    int win;
    int text;
    int ok;
    int cancel;
    AICHARREC *culprit;
    AICHARREC *judge;
    AMOUNTCFG cfg;
    AIMSG msg;
    char title[0x40];
    char note[0x200];

    if (sv_CheckMoney(ai_Chars[ai_PlayerId], 2) == 0 || bld == 0)
        return;
    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(title, 0x175f);
    culprit = (AICHARREC *)ai_AmountMenu(&cfg, title, 0, 0);
    if (culprit == 0)
        return;
    txt_Format(title, 0x1760, culprit->id);
    judge = (AICHARREC *)ai_AmountMenu(&cfg, title, 0, 0);
    if (judge == 0)
        return;
    n = gv_HeldCrimeCount(culprit->netId, ai_PlayerId);
    if (n <= 0) {
        txt_Format(note, 0x1756, culprit->id);
        sv_MessageBox(note, 0);
        sv_CameraFollow();
        return;
    }
    if (sv_CheckMoney(ai_Chars[ai_PlayerId], 3) == 0) {
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1761, n, (n != 1) + 0x1752, judge->id, culprit->id);
    ok = d2_GetWinObj(win, 1, text);
    cancel = d2_GetWinObj(win, 1, text + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                msg.arg[2] = msg.fromId = msg.to = judge->netId;
                msg.type = 0x2f;
                msg.flags = 9;
                msg.arg[1] = ai_CharInts[ai_PlayerId][0];
                msg.arg[0] = culprit->netId;
                cm_SendLongMessage((char *)&msg);
                cm_SendMoneyDelta(ai_CharInts[ai_PlayerId][0], -3);
                txt_Format(note, 0x1762, judge->id);
                sv_MessageBox(note, 0);
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_DenounceWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x5b5310, the pamphlet window "misc\perga_rolle" (ai_PamphletWindow's amount menu callback, with the picked
   character ch; nothing for none, an empty pick or the player). A running pamphlet of the player about ch
   (he_NoEvent35Pair) gives the text 0x190f, a haunting of the player by ch (he_NoEvent88Pair) 0x409f, more
   than 3 pamphlets of the player running 0x190e, a player without 1 money nothing. Else the window (text
   0x1910 about ch; the row of the player's talent 4) asks: its first button sends the pamphlet message 0x35
   (from and to the player, flags 2, the game time, ch's netId and a random text variant 0..0x1c), the second
   button or a right click close it. Returns 1 once the pamphlet went out, else 0 */

int ai_PamphletAmountCB(AICHARREC *ch)
{
    int sent;
    int count;
    int win;
    AIWINDOWROW row;
    AIMSG msg;
    char note[0x400];

    sent = 0;
    if (ch == 0)
        return 0;
    if (ch->id == 0xffff)
        return 0;
    if (ch->id == ai_PlayerId)
        return 0;
    if (he_NoEvent35Pair((unsigned short *)ai_Chars[ai_PlayerId], (int)ch, &count) == 0) {
        txt_Format(note, 0x190f, ch->id);
        sv_MessageBox(note, 0);
        return 0;
    }
    if (he_NoEvent88Pair((struct AICHAR *)ch, (struct AICHAR *)ai_Chars[ai_PlayerId]) == 0) {
        txt_Format(note, 0x409f, ch->id);
        sv_MessageBox(note, 0);
        return 0;
    }
    if (count > 3) {
        txt_Format(note, 0x190e);
        sv_MessageBox(note, 0);
        return 0;
    }
    if (sv_CheckMoney(ai_Chars[ai_PlayerId], 1) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1910, ch->id, ch->id, 1, ch->id);
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.entry = 4;
    row.record = 3;
    ai_ShowWindowRow(&row);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                gm_BuildQuery(1, 5, 10);
                msg.type = 0x35;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = ai_CharInts[ai_PlayerId][0];
                msg.flags = 2;
                msg.pamphlet.when = ai_Time;
                msg.pamphlet.variant = (unsigned char)util_Random(0x1d);
                msg.pamphlet.about = ch->netId;
                cm_SendLongMessage((char *)&msg);
                sent = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_PamphletAmountCB, 0x677c7) != 0);
    d2_CloseView(win);
    return sent;
}
/* 0x5b5610, the pamphlet window of the rhetoric contact 0x5965f0: the pamphlet text 0x190d (the plate text
   0x190c) formatted into a buffer, then the amount menu 0x5c1130 with the config (mode 0x400, kind 6, the
   filter gypsyFilter, the rest cleared) and the callback 0x5b5310; the camera follows. bld == 0 returns */

void ai_PamphletWindow(int bld)
{
    AMOUNTCFG cfg;
    char text[0x40];

    if (bld == 0)
        return;
    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(text, 0x190d);
    ai_AmountMenu(&cfg, text, 0, (void *)ai_PamphletAmountCB);
    sv_CameraFollow();
}
/* 0x5b5690, the notice board window "misc\pamphlet_brett" (the SCHWARZES_BRETT plate of the market,
   ai_MarketStandListener; text 0x1912): every half second (d3_Time + 500) it lists up to four running pamphlet
   events 0x35 whose stage is set and whose character exists, two forms each (text 0x1913 with the variant's word
   0x1916 + AIJOB.pamphletVariant and the character, and the tear-down button 0x1914), and hides the forms of the
   empty rows. A clicked button tears that pamphlet down for 2 money: the event's end is set to now
   (cm_SendExSetHE, waiting for its slot) and on success the 2 go off (gm_SendMoneyDelta); a right click closes
   the window. The camera follows
   @deep size queue s17 (102 -> 59 ins, raw 889 -> 912 vs 900: padded size, frame 0x2c as the original):
   the row loop keeps n (the form 2n-1 / 2n, the loop bound n < 5) and its own row index i for ctl[i] /
   evIds[i] (VC6 strength-reduces i to the byte offset ebx and keeps n in memory as the original); i with
   the bound i < 4 = control (merged into n), i < 4 && ev first 109. Left: the original caches -1 in ebx
   (or ebx,-1; the memsets' eight stores, d2_ClickedId = ebx, the compares, push ebx to cm_SendExSetHEI,
   re-made after the fill loop) where ours materializes it per use: c2trace const#0#1 (10 refs, lines
   5879-5928) prio -236, cut where ev#1 takes ebp. memset(.., -1, ..) at either site = control; climb round
   1: refresh = d3_Time + 500 between the two memsets 55 ins (not kept, order only). prior: Reconstructed
   whole against the original 0x5b5690..0x5b5a0f (the window misc\pamphlet_brett, text 0x1912, the
   half-second refresh d3_Time+500, the pamphlet events 0x35 with stage > -1 and gm_FindChar(idbc), the
   forms 2n-1 / 2n with 0x1913 (0x1916 + AIJOB.pamphletVariant at 0xc1) and the button 0x1914,
   d2_EnableObject of d2_Wins[d2_Views[win].wins[2n-1]].parent, the fill of the empty rows up to n < 5, the
   click search over ctl[4], he_FindEvent(1,1,evId), sv_CheckMoney 2, end = ai_Time, cm_SendExSetHE(-1, ev)
   with the slot wait, gm_SendMoneyDelta -2, the sv_CameraFollow tail call); every call, constant and
   reference equals the original. Left is allocation: the original caches -1 in ebx through the function
   (or ebx,-1; the eight memset stores of ctl/evIds, d2_ClickedId = ebx, cmp [d2_ClickedId], ebx, push ebx
   to cm_SendExSetHE, cmp edi, ebx) where ours materializes -1 at each use (or ecx/ed... */

void ai_NoticeBoardWindow(void)
{
    int win;
    int n;
    int i;
    int slot;
    unsigned long refresh;
    AIJOB *ev;
    AICHARREC *ch;
    int ctl[4];                         /* the rows' tear-down buttons */
    int evIds[4];                       /* the rows' pamphlet events */

    refresh = d3_Time;
    memset(ctl, 0xff, sizeof(ctl));
    memset(evIds, 0xff, sizeof(evIds));
    win = d2_OpenForm(0, 0, "misc\\pamphlet_brett");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    io_PrintText(0x1912);
    d2_ClickedId = -1;
    do {
        if (refresh <= d3_Time) {
            memset(ctl, 0xff, sizeof(ctl));
            memset(evIds, 0xff, sizeof(evIds));
            refresh = d3_Time + 500;
            n = 1;
            i = 0;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x35); ev != 0 && n < 5; ev = (AIJOB *)he_NextEvent()) {
                if (ev->stage > -1) {
                    ch = (AICHARREC *)gm_FindChar(ev->idbc);
                    if (ch != 0) {
                        d2_SetForm(win, n * 2 - 1);
                        io_PrintText("$C");
                        io_PrintText(0x1913, 0x1916 + ev->pamphletVariant, ch->id);
                        d2_SetForm(win, n * 2);
                        io_PrintText("$C");
                        ctl[i] = d2_GetWinObj(win, n * 2, io_PrintText(0x1914, 2));
                        evIds[i] = ev->id;
                        d2_EnableObject(d2_Wins[d2_Views[win].wins[n * 2 - 1]].parent, 1);
                        n++;
                        i++;
                    }
                }
            }
            for (; n < 5; n++) {
                d2_SetForm(win, n * 2 - 1);
                io_PrintText("$C");
                d2_EnableObject(d2_Wins[d2_Views[win].wins[n * 2 - 1]].parent, 0);
            }
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1 && d2_PointerObj != -1) {
            for (i = 0; i < 4; i++)
                if (ctl[i] == d2_PointerObj)
                    break;
            if (i < 4) {
                ev = (AIJOB *)he_FindEvent(1, 1, evIds[i]);
                if (ev != 0 && sv_CheckMoney(ai_Chars[ai_PlayerId], 2) != 0) {
                    ev->end = ai_Time;
                    slot = cm_SendExSetHEI(-1, ev);
                    while (slot != -1 && cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x677c7);
                    if (cm_GetSlotState(slot) == 1)
                        gm_SendMoneyDelta(ai_CharInts[ai_PlayerId][0], -2);
                    refresh = d3_Time;
                }
            }
        }
    } while (sv_RunFrame((int)ai_NoticeBoardWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
G11REC *hmt_GetRecord(unsigned short k);                    /* 0x4ac1e0, hmt.h's prototypes (hmt.h is not included yet: Open: head) */
unsigned short hmt_RecordTextId(unsigned short k, unsigned short n); /* 0x4ac1a0 */
int hmt_GetPrice(struct AICHAR *p);                         /* 0x4ac200 */
void hmt_ApplyRecord(struct AICHAR *p, int d, unsigned short k); /* 0x4ac2f0 */

/* 0x5b5a20, the bard's window "special\barde" (the TRIBUENE plate of the market, ai_MarketStandListener): without
   a bard event 0x6b at its stage 2 the text 0x1e26; else the song's hmt record (AIJOB.bardRec), the player's
   waiting event 0x11 for it is dropped (stage -1), a player who already bought a song (powerFlags 0x20000) gets
   the text 0x1e27, one too poor for it (hmt_GetPrice 0) 0x1e28. Else the offer 0x1e2a (the record's text + 1,
   the price) asks: the first button buys the song (hmt_ApplyRecord), then the window "special\barde2" shows the
   record's title and its poem (text + 2) with scroll buttons, and after a right click its effect (0x1e5c, then
   by the record's kind 0x1e5e with the skill, 0x1e5d with the profession or 0x1e5f). The two mp3 paths of the
   announcement and the poem are formatted but not played. The camera follows */

void ai_BardWindow(void)
{
    int win;
    int win2;
    int text;
    int price;
    int bought;
    AIJOB *ev;
    AIJOB *wait;
    G11REC *rec;
    char path[0x100];

    ev = (AIJOB *)he_FindEvent(1, 0, 0x6b);
    win = d2_OpenForm(0, 0, "special\\barde");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    d2_SetForm(win, 1);
    if (ev == 0 || ev->stage != 2) {
        io_PrintText(0x1e26);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    rec = hmt_GetRecord(ev->bardRec);
    hmt_RecordTextId(ev->bardRec, ev->bardTextOff);
    wait = (AIJOB *)he_FindEvent(3, 0, 0x11, 3, ev->id, 2, ai_PlayerId);
    if (wait != 0)
        wait->stage = -1;
    if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x20000) != 0) {
        io_PrintText(0x1e27);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    price = hmt_GetPrice((struct AICHAR *)ai_Chars[ai_PlayerId]);
    if (price == 0) {
        io_PrintText(0x1e28);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    msx_SetFadeTime(1000);
    io_PrintText(0x1e2a, rec->textId + 1, price);
    sprintf(path, "Gedichte_Announcement\\%s.mp3", rec->name);
    bought = 0;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                hmt_ApplyRecord((struct AICHAR *)ai_Chars[ai_PlayerId], price, ev->bardRec);
                bought = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
    if (bought == 0) {
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    d2_CloseView(win);
    win2 = d2_OpenForm(0, 0, "special\\barde2");
    d2_SetForm(win2, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win2);
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    d2_SetForm(win2, 1);
    io_PrintText(rec->textId);
    d2_SetForm(win2, 2);
    text = d2_LastWindow;
    d2_SetForm(win2, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, text, 0x767);
    d2_Wins[text].visible = 0x30;
    d2_SetForm(win2, 2);
    io_PrintText(rec->textId + 2);
    d2_ClickedId = -1;
    sprintf(path, "gedichte\\%s.mp3", rec->name);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
    d2_SetForm(win2, 3);
    io_PrintText("$C");
    d2_SetForm(win2, 1);
    io_PrintText("$C");
    io_PrintText(0x1e5c);
    d2_SetForm(win2, 2);
    io_PrintText("$C");
    d2_Wins[d2_LastWindow].scrollY = 0;
    if (rec->kind == 0)
        io_PrintText(0x1e5e, rec->id + 0x16b7);
    else if (rec->kind == 1)
        io_PrintText(0x1e5d, rec->id + 0x1ee);
    else
        io_PrintText(0x1e5f);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BardWindow, 0x677c7) != 0);
    d2_CloseView(win2);
    sv_CameraFollow();
}
int ai_EvalFest(AIGRP *g, int send);   /* 0x494ac0, am.h's prototype (am.h is not included yet: Open: head) */

/* 0x5b5e80, the feast window of the residence's social room (the FESTGEBEN plate of ai_FeastContact, with the
   building and its feast table object). With no feast event 0x37 of the building running, the player plans one
   for tomorrow at 20 o'clock: the window "locations\wohnsitz\fest2" picks the menu (text 0x1772, the six rows
   0x1773 + i, sent as the table's field 0x2c), then, when the building has a stage (gm_Query kind 0x18), the
   musicians (0x1779, the rows 0x177a + i, the field 0x2d; else the field gets 5, none). The window
   "locations\wohnsitz\fest" then lists the guests (0x1769 with the time, 0x176a per guest: a click drops him)
   and the cost (ai_EvalFest, 0x176b: the button invites one more through the amount menu); its first button
   sends the feast message 0x37 and to every guest the invitation 0x36 (flags 9 for the kinds 6 / 7, else 1) and
   writes the guests' netIds into the table (-1 for the empty seats), the second or a right click cancel. A
   running feast (or a planned one) shows its summary in "fest2": the time (0x1780), the guests (0x1781), the
   menu (0x1782), the musicians (0x1783, with a stage) and the cost (0x1784). The camera follows
   @deep frame sweep s21: 202 -> 108 ins: none (the -1 sent by address with the feast message) a local of
   the send block. Left: frame 0x3e8 vs 0x3e4 (one slot more: the original packs ok / cancel into one slot
   and i / refresh), two of the original's write-once slots ([esp+0xbc], [esp+0x164]) without a partner,
   code 70. Lead: ok / cancel as locals of the blocks that create and test them. prior: Reconstructed whole
   against the original 0x5b5e80..0x5b693c (feast planning: the clock + 24 h at 20:00, the feast event 0x37
   search by AIJOB.from, the stage query gm_Query kind 0x18, the menu window fest2 with the rows 0x1773+i
   and the table field 0x2c, the musician rows 0x177a+i and field 0x2d or 5, the guest window fest with
   0x1769 / 0x176a / ai_EvalFest / 0x176b / sv_ButtonRow 0x176c 0x176d, the invite amount menu with the ids
   filled and option14 |= 2, the messages 0x37 and 0x36 with flags 9 / 1, the table's guest fields 0x1c +
   4k with -1 for empty seats, the summary 0x1780..0x1784, one shared close tail): same instruction count
   (739 / 739), every call, constant and reference equal. Left is frame and allocation: the original packs
   two pairs of locals into one slot each (the menu index byte i and the guest window's refresh flag at
   frame+0x10, the done flag and the -1 of the empty seats at frame+0x14), ours gives each its own slot
   (frame 0x3ec vs 0x3e4), and keeps 0 in edi where the original keeps it in ebp at the start (the time
   copy and the cfg / guests stores scheduled around it, asmdiff +0x0..+0x80) plus the store order of the
   AIGRP fields at +0x348. Tried: done reused for the -1 (puts done in memory everywhere, worse),
   block-scoped i / refresh (no slot sharing), memset vs single stores of guests (single stores give the
   original's per-slot stores) */

void ai_FeastWindow(AIBLD *bld, AIOBJ *table)
{
    int k;
    int n;
    int win;
    int done;
    int essen;
    int musik;
    int hasStage;
    int invite;
    int ok;
    int cancel;
    AIJOB *ev;
    AICHARREC *ch;
    AICHARREC *pick;
    unsigned short *id;
    AITIME when;
    AICHARREC *guests[4];
    AIGRP g;
    int ctl[6];                         /* the pick rows, then the guests' rows */
    int btns[2];
    AMOUNTCFG cfg;
    AIMSG msg;
    char okText[0x100];
    char cancelText[0x100];

    when = ai_Time;
    memset(&cfg, 0, sizeof(cfg));
    essen = 6;
    cfg.kind = 6;
    done = 0;
    guests[0] = 0;
    guests[1] = 0;
    guests[2] = 0;
    guests[3] = 0;
    musik = 5;
    cfg.mode = 0x400;
    cfg.filter = gypsyFilter;
    gm_AddTime(&when, 0x18, 0, 0);
    when.hour = 0x14;
    when.min = 0;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x37); ev != 0; ev = (AIJOB *)he_NextEvent())
        if (ev->from == bld->id)
            break;
    hasStage = gm_Query((void *)bld->objList, 1, 0, 0x18) != 0;
    if (ev == 0) {
        unsigned char i;
        unsigned char noMusik;
        int refresh;

        win = d2_OpenForm(0, 0, "locations\\wohnsitz\\fest2");
        d2_SetForm(win, 0);
        sv_FitForm(d2_LastWindow);
        d2_CentreViewWindows(win);
        d2_SetPointerSprite(0, 0);
        d2_ClickedId = -1;
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText(0x1772);
        for (i = 0; i < 5; i++)
            ctl[i] = d2_GetWinObj(win, 1, io_PrintText("$L%ia[%s]$A", 0x1773 + i));
        ctl[i] = d2_GetWinObj(win, 1, io_PrintText("$L%in[%s]", 0x1773 + i));
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0) {
                done = 0;
                sv_PopRequest = 1;
            }
            if (d2_ClickedId != -1) {
                for (i = 0; i < 6; i++) {
                    if (d2_PointerObj == ctl[i]) {
                        cm_BeginFieldBlock((char *)table, table->objId);
                        cm_AddFieldDelta(1, 1, 0x2c, &i);
                        cm_SendFieldDeltas();
                        sv_PopRequest = 1;
                        done = 1;
                        essen = i;
                        break;
                    }
                }
            }
        } while (sv_RunFrame((int)ai_FeastWindow, 0x677c7) != 0);
        if (done == 0)
            goto close;
        if (hasStage != 0) {
            d2_SetForm(win, 1);
            io_PrintText("$C");
            io_PrintText(0x1779);
            for (i = 0; i < 5; i++)
                ctl[i] = d2_GetWinObj(win, 1, io_PrintText("$L%ia[%s]$A", 0x177a + i));
            ctl[i] = d2_GetWinObj(win, 1, io_PrintText("$L%in[%s]", 0x177a + i));
            d2_ClickedId = -1;
            do {
                if (dm_L.rup != 0) {
                    done = 0;
                    sv_PopRequest = 1;
                }
                if (d2_ClickedId != -1) {
                    for (i = 0; i < 6; i++) {
                        if (d2_PointerObj == ctl[i]) {
                            cm_BeginFieldBlock((char *)table, table->objId);
                            cm_AddFieldDelta(1, 1, 0x2d, &i);
                            cm_SendFieldDeltas();
                            sv_PopRequest = 1;
                            done = 1;
                            musik = i;
                            break;
                        }
                    }
                }
            } while (sv_RunFrame((int)ai_FeastWindow, 0x677c7) != 0);
        } else {
            noMusik = (unsigned char)musik;
            cm_BeginFieldBlock((char *)table, table->objId);
            cm_AddFieldDelta(1, 1, 0x2d, &noMusik);
            cm_SendFieldDeltas();
        }
        if (done == 0)
            goto close;
        d2_CloseView(win);
        invite = -1;
        refresh = 1;
        ok = -1;
        cancel = -1;
        win = d2_OpenForm(0, 0, "locations\\wohnsitz\\fest");
        d2_SetForm(win, 0);
        sv_FitForm(d2_LastWindow);
        d2_CentreViewWindows(win);
        d2_SetPointerSprite(0, 0);
        d2_ClickedId = -1;
        do {
            if (refresh != 0) {
                refresh = 0;
                d2_SetForm(win, 1);
                io_PrintText("$C");
                io_PrintText(0x1769, &when);
                g.cost = 0;
                n = 0;
                g.n = 0;
                g.musik = musik;
                g.leader = (struct AIC4 *)ai_Chars[ai_PlayerId];
                g.essen = essen;
                for (k = 0; k < 4; k++) {
                    ch = guests[k];
                    if (ch != 0) {
                        ctl[k] = d2_GetWinObj(win, 1, io_PrintText(0x176a, ch->id));
                        g.mem[g.n].ch = (struct AIC4 *)ch;
                        g.n++;
                        n++;
                    } else {
                        ctl[k] = -1;
                    }
                }
                ai_EvalFest(&g, 0);
                d2_SetForm(win, 2);
                io_PrintText("$C");
                invite = d2_GetWinObj(win, 2, io_PrintText(0x176b, g.cost));
                if (n == 4)
                    d2_SetObjEnabled(invite, 0);
                d2_SetForm(win, 3);
                io_PrintText("$C");
                txt_Format(okText, 0x176c);
                txt_Format(cancelText, 0x176d);
                sv_ButtonRow(d2_LastWindow, 0, btns, 2, okText, cancelText);
                ok = btns[0];
                cancel = btns[1];
                if (n == 0)
                    d2_SetObjEnabled(ok, 0);
            }
            if (d2_ClickedId != -1) {
                if (dm_L.rup != 0) {
                    done = 0;
                    sv_PopRequest = 1;
                }
                if (d2_PointerObj == invite) {
                    memset(cfg.ids, 0xff, sizeof(cfg.ids));
                    cfg.option14 |= 2;
                    id = cfg.ids;
                    for (k = 0; k < 4; k++)
                        if (guests[k] != 0)
                            *id++ = guests[k]->id;
                    pick = (AICHARREC *)ai_AmountMenu(&cfg, "$ZZum Fest einladen", 0, 0);
                    if (pick != 0) {
                        for (k = 0; k < 4; k++) {
                            if (guests[k] == 0) {
                                guests[k] = pick;
                                refresh = 1;
                                break;
                            }
                        }
                    }
                } else if (d2_PointerObj == ok) {
                    int none;

                    none = -1;
                    msg.type = 0x37;
                    msg.arg[5] = 0;
                    msg.flags = 2;
                    msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.fromId = bld->id;
                    cm_SendLongMessage((char *)&msg);
                    n = 0;
                    for (k = 0; k < 4; k++) {
                        ch = guests[k];
                        if (ch != 0) {
                            cm_BeginFieldBlock((char *)table, table->objId);
                            cm_AddFieldDelta(4, 1, 0x1c + n * 4, &ch->netId);
                            cm_SendFieldDeltas();
                            msg.type = 0x36;
                            msg.arg[4] = 0;
                            msg.to = ch->netId;
                            msg.fromId = bld->id;
                            msg.inviteWhen = when;
                            if (ch->kind == 6 || ch->kind == 7)
                                msg.flags = 9;
                            else
                                msg.flags = 1;
                            cm_SendLongMessage((char *)&msg);
                            n++;
                        }
                    }
                    for (k = n; k < 4; k++) {
                        cm_BeginFieldBlock((char *)table, table->objId);
                        cm_AddFieldDelta(4, 1, 0x1c + k * 4, &none);
                        cm_SendFieldDeltas();
                    }
                    done = 1;
                    sv_PopRequest = 1;
                } else if (d2_PointerObj == cancel) {
                    done = 0;
                    sv_PopRequest = 1;
                } else {
                    for (k = 0; k < 4; k++) {
                        if (d2_PointerObj == ctl[k]) {
                            guests[k] = 0;
                            refresh = 1;
                        }
                    }
                }
            }
        } while (sv_RunFrame((int)ai_FeastWindow, 0x677c7) != 0);
        if (done == 0)
            goto close;
        d2_CloseView(win);
    } else {
        when = ev->end;
        for (k = 0; k < 4; k++)
            guests[k] = (AICHARREC *)gm_FindChar(table->festGuests[k]);
        essen = table->festEssen;
        musik = table->festMusik;
    }
    g.musik = musik;
    g.essen = essen;
    g.cost = 0;
    g.leader = (struct AIC4 *)ai_Chars[ai_PlayerId];
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\fest2");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1780, &when);
    g.n = 0;
    for (k = 0; k < 4; k++) {
        if (guests[k] != 0) {
            g.mem[g.n].ch = (struct AIC4 *)guests[k];
            g.n++;
            io_PrintText(0x1781, guests[k]->id);
        }
    }
    io_PrintText(0x1782, essen + 0x1773);
    if (hasStage != 0)
        io_PrintText(0x1783, musik + 0x177a);
    ai_EvalFest(&g, 0);
    io_PrintText(0x1784, g.cost);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_FeastWindow, 0x677c7) != 0);
close:
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x5b6950, the building sale window "Panel\Gebaeude_Verkaufen_pergament" (ai_BuildingWindow, with the player's
   building): the text 0x17c5 (the building, the currency name 0x9a8 + 2 * the city's money good) and a price
   slider from half to one and a half of the building's equipment value (gm_EquipmentValue at its level, in the
   player's currency). The first button puts the building up for sale: the message 0x1b (from -1, flags 2,
   arg[0] the building, arg[1] the price in the base currency, gm_PlayerFactor) and the note txt_Ptrs[0x17c6];
   the second button, a right click, the building gone or no longer enterable (flags5aw 0x400) close it */

void ai_SellBuildingWindow(AIBLD *bld)
{
    int bldId;
    int win;
    int value;
    int slider;
    AIBLD *found;
    AIMSG msg;

    bldId = bld->id;
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Verkaufen_pergament");
    d2_CentreViewWindows(win);
    value = gm_EquipmentValue(bld, bld->level);
    value = gm_ConvertMoney(value, ai_OwnPlayer);
    d2_SetForm(win, 2);
    slider = d2_GetWinObj(win, 2, io_PrintText(0x17c5, bld, ai_PlayerCity[ai_OwnPlayer][0] * 2 + 0x9a8));
    d2_SetObjValue4(slider, (int)(value * 0.5), (int)(value * 1.5), value);
    d2_SetPointerSprite(0, 0);
    sv_RunFrame(0, 0x677c7);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x1b;
            msg.fromId = -1;
            msg.flags = 2;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.arg[0] = bld->id;
            msg.arg[1] = gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer);
            cm_SendLongMessage((char *)&msg);
            sv_MessageBox(txt_Ptrs[0x17c6], 0);
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_SellBuildingWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b6b20, the renovation window "Panel\Gebaeude_Renovieren" (ai_BuildingWindow, ai_ManagedBuildingWindow): a
   building at its full level 100 gets the note txt_Ptrs[0x17ca]. Else a level point costs 0.5 % of the
   building's equipment value, at least 128; the text 0x17c8 offers a quarter, half or all of the missing points
   (the three buttons, their costs) and a fourth button closes. A pick sends the renovation message 0x1c (from
   -1, flags 1, arg[0] the building, arg[1] the points) in the group "renovieren" with the payment: from the
   building's class treasury when it is the player character's (sv_CheckStock), else from the player's own
   money (sv_CheckGoodAmount, counted in the player record's renovationSpent). A right click, the building gone
   or no longer enterable (flags5aw 0x400) close it */

void ai_RenovationWindow(AIBLD *bld)
{
    int bldId;
    int missing;
    float perPoint;
    float total;
    int win;
    int text;
    int costAll;
    int costHalf;
    int costQuarter;
    int btnQuarter;
    int btnHalf;
    int btnAll;
    int btnClose;
    int cost;
    int points;
    AIBLD *found;
    AIROOM *treasury;
    LCFIN *fin;
    AIMSG msg;

    bldId = bld->id;
    perPoint = (float)gm_EquipmentValue(bld, 100);
    missing = 100 - bld->level;
    if (missing <= 0) {
        sv_MessageBox(txt_Ptrs[0x17ca], 4);
        return;
    }
    perPoint *= 0.005f;
    if (128.0f > perPoint)
        perPoint = 128.0f;
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Renovieren");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    total = missing * perPoint;
    costAll = (int)total;
    costHalf = (int)(total * 0.5);
    costQuarter = (int)(total * 0.25);
    text = io_PrintText(0x17c8, bld, costQuarter, costHalf, costAll);
    btnQuarter = d2_GetWinObj(win, 2, text);
    btnHalf = d2_GetWinObj(win, 2, text + 1);
    btnAll = d2_GetWinObj(win, 2, text + 2);
    btnClose = d2_GetWinObj(win, 2, text + 3);
    d2_SetPointerSprite(0, 0);
    sv_RunFrame(0, 0x677c7);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnQuarter || d2_PointerObj == btnHalf || d2_PointerObj == btnAll) {
                cost = 0;
                points = 0;
                msg.type = 0x1c;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.arg[0] = bld->id;
                msg.fromId = -1;
                msg.flags = 1;
                if (d2_PointerObj == btnQuarter) {
                    points = (int)(missing * 0.25);
                    cost = costQuarter;
                } else if (d2_PointerObj == btnHalf) {
                    points = (int)(missing * 0.5);
                    cost = costHalf;
                } else if (d2_PointerObj == btnAll) {
                    points = missing;
                    cost = costAll;
                }
                if (bld->owner == *ai_PlayerChar) {
                    treasury = (AIROOM *)gm_ClassTreasury(bld);
                    if (treasury == 0 || sv_CheckStockB(treasury, treasury->list, cost, ai_OwnPlayer) == 0)
                        continue;
                    msg.arg[1] = points;
                    cm_BeginGroup("renovieren");
                    cm_SendMoveGoods(-1, treasury->objId, ai_OwnPlayer, cost);
                    cm_SendLongMessage((char *)&msg);
                    cm_EndGroup("renovieren");
                    sv_PopRequest = 1;
                } else {
                    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer) == 0)
                        continue;
                    fin = (LCFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                    if (fin != 0)
                        fin->renovationSpent += cost;
                    msg.arg[1] = points;
                    cm_BeginGroup("renovieren");
                    cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, cost);
                    cm_SendLongMessage((char *)&msg);
                    cm_EndGroup("renovieren");
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnClose) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_RenovationWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b6eb0, the window of a building that is up for sale (ai_BuildingWindow): the sale window
   "Panel\Gebaeude_Verkaufen_pergament" shows the text 0x17cc (the building, the asking price of its sale event
   0x1b, AIJOB.salePrice); its first button withdraws the sale (the building's field 0x5a loses the flag 2,
   cm_SendSetField, and the event is freed), the second button, a right click, the building gone or no longer
   enterable (flags5aw 0x400) close it */

void ai_WithdrawSaleWindow(AIBLD *bld)
{
    int bldId;
    int win;
    int withdraw;
    AIBLD *found;
    AIJOB *ev;

    bldId = bld->id;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x1b); ev != 0; ev = (AIJOB *)he_NextEvent())
        if (ev->bldId == bld->id)
            break;
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Verkaufen_pergament");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    withdraw = d2_GetWinObj(win, 2, io_PrintText(0x17cc, bld, ev->salePrice));
    d2_SetPointerSprite(0, 0);
    sv_RunFrame(0, 0x677c7);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == withdraw) {
                cm_SendSetField(bld->id, 0x5a, 2, 0, 2);
                he_FreeEvent(ev);
                sv_PopRequest = 1;
            }
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_WithdrawSaleWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b6fe0, the room upgrade window of a building (ai_BuildingWindow, ai_ManagedBuildingWindow):
   "Panel\Gebaeude_Raum_Erweitern" lists the upgrade goods its type can hold (AIBTYPE.objs of ai_Goods type 2 or 6,
   not 0xfd) with their price (text 0x17d3: the goods price times 0.5 + difficulty / 4); a click on one buys it
   (message 0x1f "room_upgrade", the price moved from the player) while the built and running upgrades stay below
   AIBTYPE.extensions (else the message box 0x17d0); a running upgrade event 0x1f fills its picture by its progress
   (text 0x17d1), a built one is marked (text 0x17d2); text 0x17cf shows the upgrades left */

void ai_RoomUpgradeWindow(AIBLD *bld)
{
    struct {
        int good;                       /* the upgrade good */
        int pic;                        /* its picture (the button) */
        int text;                       /* its price / state text */
    } rows[32];
    int nRows;
    int built;
    int running;
    int bldId;
    AIBTYPE *type;
    int win;
    int i;
    int k;
    int good;
    AIGOOD *g;
    int price;
    int down;
    int up;
    int bar;
    unsigned short *obj;
    AIJOB *ev;
    float total;
    AIBLD *found;

    nRows = 0;
    built = 0;
    running = 0;
    bldId = bld->id;
    for (i = 0; i < 32; i++) {
        rows[i].pic = -1;
        rows[i].text = -1;
        rows[i].good = 0;
    }
    for (ev = (AIJOB *)he_FindEvent(2, 0, 0x1f, 3, bldId); ev != 0; ev = (AIJOB *)he_NextEvent())
        running++;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    for (i = 0; i < 0x40; i++) {
        if (nRows >= 32)
            break;
        if (type->objs[i] != 0xffff) {
            good = type->objs[i] & 0x7fff;
            g = &((AIGOOD *)ai_Goods)[good];
            if ((g->type == 2 || g->type == 6) && good != 0xfd) {
                rows[nRows].good = good;
                nRows++;
            }
        }
    }
    for (obj = (unsigned short *)gm_Query((void *)bld->objList, 1, 5); obj != 0; obj = gm_QueryNext()) {
        g = &((AIGOOD *)ai_Goods)[*obj];
        if ((g->type == 2 || g->type == 6) && *obj != 0xfd)
            built++;
    }
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Raum_Erweitern");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x17ce);
    d2_SetForm(win, 0);
    if (nRows > 4) {
        sv_CreateScrollButtons(d2_LastWindowPtr->w - 0x20, d2_LastWindowPtr->h - 0x4c, d2_LastWindow,
                               d2_GetViewWin(win, 2), 0x3c, &down, &up, &bar);
        d2_Objs[bar].font = 0x56;
    }
    d2_SetForm(win, 2);
    for (i = 0; i < nRows; i++) {
        char text[0x100];
        float factor;

        factor = ai_Difficulty * 0.25 + 0.5;
        price = (int)(gm_ItemPrice(rows[i].good, 100) * factor);
        rows[i].pic = d2_WinOpenPic(0x10, i * 0x3c, rows[i].good + 0xdf, d2_LastWindow);
        d2_Objs[rows[i].pic].interactive = 1;
        d2_Objs[rows[i].pic].style = 3;
        d2_Objs[rows[i].pic].icon = 0x520;
        d2_Objs[rows[i].pic].iconx = -2;
        d2_Objs[rows[i].pic].icony = -2;
        txt_Format(text, 0x17d3, txt_ObjectNames[rows[i].good][0], price);
        rows[i].text = d2_CreateWinText(0x60, i * 0x3c + 10, text, d2_LastWindow);
    }
    d2_SetPointerSprite(0, 0);
    d2_ClearInput();
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < nRows; i++) {
                if (d2_PointerObj == rows[i].pic) {
                    AIMSG msg;
                    float factor;

                    factor = ai_Difficulty * 0.25 + 0.5;
                    price = (int)(gm_ItemPrice(rows[i].good, 100) * factor);
                    if (running + built >= type->extensions) {
                        sv_MessageBox(txt_Ptrs[0x17d0], 0);
                    } else if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0) {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 0x1f;
                        msg.flags = 1;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.fromId = bld->id;
                        msg.time = ai_Time;
                        msg.roomUpgrade.bld = bld->id;
                        msg.roomUpgrade.good = (unsigned short)rows[i].good;
                        cm_BeginGroup("room_upgrade");
                        cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, price);
                        cm_SendLongMessage((char *)&msg);
                        cm_EndGroup("room_upgrade");
                    }
                    break;
                }
            }
        }
        running = 0;
        for (ev = (AIJOB *)he_FindEvent(2, 0, 0x1f, 3, bld->id); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            for (k = 0; k < nRows; k++) {
                d2_Objs[rows[k].pic].fillFraction = 0;
                if (ev->upgradeGood == rows[k].good) {
                    d2_Objs[rows[k].pic].interactive = 0;
                    d2_SetObjText(rows[k].text, txt_Ptrs[0x17d1]);
                    total = (float)gm_MinutesBetween(&ev->start, &ev->end);
                    d2_Objs[rows[k].pic].fillFraction = (float)gm_MinutesBetween(&ev->start, &ai_Time) / total;
                    break;
                }
            }
            running++;
        }
        built = 0;
        for (k = 0; k < nRows; k++) {
            if (gm_Query((void *)bld->objList, 1, 0, rows[k].good) != 0) {
                d2_Objs[rows[k].pic].interactive = 0;
                d2_SetObjText(rows[k].text, txt_Ptrs[0x17d2]);
                built++;
            }
        }
        d2_SetForm(win, 3);
        io_PrintText(0x17cf, bld, type->extensions - running - built, 0x17d4);
    } while (sv_RunFrame((int)ai_RoomUpgradeWindow, 0x657c7) != 0);
    d2_CloseView(win);
}
/* 0x5b7690, the upgrade of a building to the next type of its line (ai_BuildingWindow, ai_ManagedBuildingWindow):
   refused at the line's highest level (text 0x17d7), without the right to the next type (gm_MayTakeJob: text 0x17d6
   for the classes 1 / 4 / 8, else 0x17d9; gm_AllowAll skips the check) or when the building site does not fit the
   next type's name (text 0x17d8); else asks (text 0x17d5) for the price, 0.3 of the next type's value, and sends the
   message 0x1e "building_upgrade": paid by the player (then text 0x17da), of the classes 3 / 5 from the treasury
   @deep reconstructed from the stub against the whole original 0x5b7690..0x5b7b7c: all checks (maxLevel,
   gm_MayTakeJob 0x17d6/0x17d9, the site name strstr/_strcmpi 0x17d8), the price 0.3 *
   gm_BuildingKindValue, the player path and the class 3/5 treasury path with both 0x1e building_upgrade
   messages; ours 378 vs 377 ins, the first 0x290 bytes identical. Left is scheduling: the treasury path's
   AIMSG fill (+0xad9..+0xb45): the original loads ai_Time.year before the rep stosd and copies the time
   words interleaved with type/arg/flags/fromId/to, ours copies the 14-byte struct in another dword order
   (tried 8 statement orders: every one leaves the copy scheduled differently); the gm_ClassTreasury result
   move (+0xa98: mov ebx,eax before vs after the lea/push); the regs hunks eax/ecx/edx swaps around
   cm_SendMoveGoods / sv_CheckStock. The prover stops at the cross-jumped je of the class 3 path (a
   condition on flags set in another block) (references: M8 at +0x454: ours refers to ai_Time+0x4
   (0x770ae4), the original's field to 0x770ae0 (ai_Time), explained: the M8 at +0x454 is the struct copy
   msg.time = ai_Time scheduled in another dword order: both sides copy all four words of ai_Time
   (0x770ae0/4/8/c) into msg+0x28..0x35, only the load order differs) */

void ai_UpgradeBuilding(AIBLD *bld)
{
    char text[0x200];
    AIBUILDSITE site;
    AIBTYPE *type;
    AIROOM *treasury;
    AIBLD *found;
    AIBLD *first;
    int bldId;
    int cls;
    int price;

    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    bldId = bld->id;
    if (type->level >= type->maxLevel) {
        txt_Format(text, 0x17d7);
        sv_MessageBox(text, 0);
        return;
    }
    cls = gm_BuildingClass(bld->type);
    if (gm_AllowAll == 0) {
        if (cls != 1 && cls != 4 && cls != 8) {
            if (gm_MayTakeJob(bld->type + 1, ai_PlayerId) != 1) {
                txt_Format(text, 0x17d9);
                sv_MessageBox(text, 0);
                return;
            }
        } else if (gm_MayTakeJob(bld->type + 1, ai_PlayerId) != 1) {
            txt_Format(text, 0x17d6);
            sv_MessageBox(text, 0);
            return;
        }
    }
    {
        char name[0xf8];

        lc_GetGebaeudeBauplatz(&site, bld);
        if (site.object != 0) {
            ai_BuildingName(bld->type + 1, name);
            if (strstr(name, (char *)site.object) == 0) {
                txt_Format(text, 0x17d8, bld->type * 0xe + 0x49c);
                sv_MessageBox(text, 0);
                return;
            }
        } else {
            ai_BuildingName(bld->type + 1, name);
            if (strcmpi(name, site.name) != 0) {
                txt_Format(text, 0x17d8, bld->type * 0xe + 0x49c);
                sv_MessageBox(text, 0);
                return;
            }
        }
    }
    price = (int)(gm_BuildingKindValue(bld->type + 1) * 0.3);
    txt_Format(text, 0x17d5, price, bld, bld->type * 0xe + 0x49c);
    if (cls != 3 && cls != 5) {
        AIMSG msg;
        char done[0x400];

        if (sv_MessageBox(text, 0x11) == 0)
            return;
        if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) == 0)
            return;
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            return;
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.flags = 1;
        msg.to = ai_CharInts[ai_PlayerId][0];
        msg.fromId = bld->id;
        msg.time = ai_Time;
        msg.arg[0] = bld->id;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, price);
        cm_SendLongMessage((char *)&msg);
        cm_EndGroup("building_upgrade");
        txt_Format(done, 0x17da, bld->type * 0xe + 0x49c);
        sv_MessageBox(done, 4);
        return;
    }
    if (cls == 3) {
        first = (AIBLD *)gm_FirstBuilding(1);
        treasury = (AIROOM *)gm_Query((void *)first->objList, 2, 6, 0, 0x115);
        if (sv_MessageBox(text, 0x11) == 0)
            return;
    } else {
        treasury = (AIROOM *)gm_ClassTreasury(bld);
        if (sv_MessageBox(text, 0x11) == 0 || treasury == 0)
            return;
    }
    if (sv_CheckStockB(treasury, treasury->list, price, ai_OwnPlayer) != 0) {
        AIMSG msg;

        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.flags = 1;
        msg.to = ai_CharInts[ai_PlayerId][0];
        msg.fromId = bld->id;
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.flags = 1;
        msg.fromId = bld->id;
        msg.to = ai_CharInts[ai_PlayerId][0];
        msg.type = 0x1e;
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.flags = 1;
        msg.fromId = bld->id;
        msg.to = ai_CharInts[ai_PlayerId][0];
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.flags = 1;
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.flags = 1;
        msg.fromId = bld->id;
        msg.type = 0x1e;
        msg.arg[0] = bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.fromId = bld->id;
        msg.to = ai_CharInts[ai_PlayerId][0];
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, treasury->objId, ai_OwnPlayer, price);
        cm_SendLongMessage((char *)&msg);
        cm_EndGroup("building_upgrade");
    }
}
/* 0x5b7b80, the offer to buy a building from its master (ai_BuildingWindow, another's building): refused with the
   note txt_Ptrs[0x17df] when the player may not hold the kind (gm_MayTakeJob); else "Panel\Gebaeude_Verkaufen_pergament"
   shows the text 0x17de (the master, the building, the player's currency) and a price slider from half to ten
   times the building's equipment value. The first button, when the player's money covers the price, sends the
   offer message 0x21 to the master's player (from -1, flags 9, arg[0] the building, arg[1] the price in the base
   currency, arg[2] the buyer) and marks the building (field 0x5a, flag 0x200); it, the second button, a right
   click, the building gone or no longer enterable (flags5aw 0x400) close it */

void ai_BuyBuildingOfferWindow(AIBLD *bld)
{
    int bldId;
    int value;
    AIMSG msg;
    int win;
    int slider;
    AIBLD *found;
    char *text;

    bldId = bld->id;
    if (gm_MayTakeJob(bld->type, ai_PlayerId) != 1) {
        sv_MessageBox(txt_Ptrs[0x17df], 0);
        return;
    }
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Verkaufen_pergament");
    d2_CentreViewWindows(win);
    value = gm_EquipmentValue(bld, bld->level);
    value = gm_ConvertMoney(value, ai_OwnPlayer);
    d2_SetForm(win, 2);
    text = io_PrintText(0x17de, bld->masterId, bld, ai_OwnPlayer);
    slider = d2_GetWinObj(win, 2, text);
    d2_SetObjValue4(slider, (int)(value * 0.5), (int)(value * 10.0), value);
    d2_SetPointerSprite(0, 0);
    sv_RunFrame(0, 0x677c7);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer),
                                    ai_OwnPlayer) != 0) {
                msg.type = 0x21;
                msg.fromId = -1;
                msg.flags = 9;
                msg.to = ai_CharInts[bld->masterId][0];
                msg.arg[0] = bld->id;
                msg.arg[1] = gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer);
                msg.arg[2] = ai_CharInts[ai_PlayerId][0];
                cm_SendLongMessage((char *)&msg);
                cm_SendSetField(bld->id, 0x5a, 2, 0x200, 0);
            }
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_BuyBuildingOfferWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b7dd0, the effects window of a building's techniques "techtree\auswirkungen" (ai_TechTreeWindow,
   ai_BuildingWindow): the text 0x17e0 (the building), a close button (text 0x17e1) and per skill 1 .. 36 the
   building kind can reach (gm_BuildingKindSkill) a caption (txt_Ptrs[0xf98 + 2 * skill]) and a slider up to that
   level (6 for skill 3, 4 for skill 0x24) showing the building's levels (gm_BuildingSkill 1 / 0); the rows share the
   window's height. A click on a caption shows the skill's text pair 0xf98 / 0xf99 + 2 * skill in a message box
   @deep reconstructed from the stub against the whole original 0x5b7dd0..0x5b8293 (the order table with
   its 18-value initializer then 1..36, the sliders memset -1, the kind-skill count, step = (int)(10.0f / n
   * 24.0f), the caption / slider rows, the close button, the caption click message box $Z$[%s$]$N%s, the
   per-frame slider refresh with the original's unused first gm_BuildingSkill call); ours 341 vs 342 ins.
   Left is register allocation: the original keeps win in ebp through the frame loop and reloads bld from
   its argument slot at every use (mov edx,[esp+0x5ec]), ours keeps bld in ebp/ebx and win in edi, so the
   loop head is entered twice in ours (+0xd1: or ebx,-1 / mov [d2_ClickedId],ebx duplicated, the -1
   compares against ebx instead of an immediate +0x115/+0x13b/+0x14b), the slider refresh keeps the slider
   in edi (+0x1dc) where the original compares the slot in memory, and the step / y / top slots are
   assigned other registers (+0x51..+0x9b: step in edi vs a slot). The prover stops at the d2_Objs reload
   scheduled before / after the pics[i] store (+0x1da) */

void ai_TechEffectsWindow(AIBLD *bld)
{
    int n;
    int y;
    int step;
    int win;
    int bldId;
    int top;
    int close;
    int order[37] = { 1, 2, 0xb, 0xc, 9, 5, 6, 3, 4, 0xa, 0xd, 0xe, 0xf, 0x10, 0x11, 7, 8, 0 };
    int sliders[37];
    int pics[37];
    char text[0x400];
    int i;
    int skill;
    int max;
    int pic;

    bldId = bld->id;
    for (i = 0; i < 0x24; i++)
        order[i] = i + 1;
    memset(sliders, 0xff, sizeof(sliders));
    order[0x24] = 0;
    win = d2_OpenForm(0, 0, "techtree\\auswirkungen");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x17e0, bld);
    d2_SetForm(win, 3);
    close = d2_GetWinObj(win, 3, io_PrintText(0x17e1));
    d2_SetForm(win, 2);
    n = 0;
    for (i = 0; order[i] != 0; i++)
        if (gm_BuildingKindSkill(bld->type, order[i]) != 0)
            n++;
    step = (int)(10.0f / n * 24.0f);
    top = (d2_LastWindowPtr->h - step * n) / 2 + 0xe;
    n = 0;
    y = top;
    for (i = 0; (skill = order[i]) != 0; i++) {
        if (gm_BuildingKindSkill(bld->type, skill) != 0) {
            pic = d2_WinOpenPic(0, step * n + top, 0, d2_LastWindow);
            pics[i] = pic;
            d2_Objs[pic].style = 3;
            d2_Objs[pic].font = 0x55;
            txt_Format(text, "%s:", txt_Ptrs[0xf98 + skill * 2]);
            d2_SetCaption(pic, text);
            d2_Objs[pic].interactive = 1;
            if (skill == 3)
                max = 6;
            else if (skill == 0x24)
                max = 4;
            else
                max = gm_BuildingKindSkill(bld->type, skill);
            sliders[i] = d2_WinOpenSlider(0x140, y, 0x82, 0, max, 0x6f, 0x56, d2_LastWindow);
            d2_SetObjValue(sliders[i], 0, max, 0, 0);
            n++;
            y += step;
        } else {
            pics[i] = -1;
        }
    }
    d2_ClickedId = -1;
    do {
        if ((bld->flags5aw & 0x400) != 0 || gm_FindBuildingById(bldId) == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0 || (d2_ClickedId != -1 && d2_PointerObj == close))
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; (skill = order[i]) != 0; i++) {
                if (pics[i] != -1 && d2_PointerObj == pics[i]) {
                    txt_Format(text, "$Z$[%s$]$N%s", skill * 2 + 0xf98, skill * 2 + 0xf99);
                    d2_EnableViewObjects(win, 0);
                    sv_MessageBox(text, 0x10);
                    d2_EnableViewObjects(win, 1);
                }
            }
        }
        for (i = 0; (skill = order[i]) != 0; i++) {
            if (gm_BuildingKindSkill(bld->type, skill) != 0 && sliders[i] != -1) {
                if (skill == 3)
                    max = 6;
                else if (skill == 0x24)
                    max = 4;
                else
                    max = gm_BuildingKindSkill(bld->type, skill);
                gm_BuildingSkill((unsigned char *)bld, skill, 0);    /* its value unused (the original calls it too) */
                d2_SetObjValue(sliders[i], 0, max, gm_BuildingSkill((unsigned char *)bld, skill, 1),
                               gm_BuildingSkill((unsigned char *)bld, skill, 0));
            }
        }
    } while (sv_RunFrame((int)ai_TechEffectsWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b82a0, the fire fighting window of a burning building "Panel\Gebaeude_Brand_loeschen" (ai_BuildingWindow):
   the text 0x17e4 (the building) and three offers (text 0x17e5: 1, 2 or 4 money for a chance of 0.2, 0.5 or 0.8).
   A pick the player's money covers is paid (gm_SendMoneyDelta); on success the building's burning event 0x4f is
   handed to cm_SendExSetHE (its exSlot) with the note txt_Ptrs[0x17f7] and the window closes, else the note
   txt_Ptrs[0x17f8]. The second button, a right click, the building gone or no longer enterable close it */

void ai_FireFightingWindow(AIBLD *bld)
{
    int bldId;
    int win;
    int text;
    int low;
    int mid;
    int high;
    int cost;
    float chance;
    AIBLD *found;
    AIJOB *ev;

    bldId = bld->id;
    win = d2_OpenForm(0, 0, "Panel\\Gebaeude_Brand_loeschen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x17e4, bld);
    d2_SetForm(win, 2);
    text = io_PrintText(0x17e5, 1, 2, 4);
    low = d2_GetWinObj(win, 2, text);
    mid = d2_GetWinObj(win, 2, text + 1);
    high = d2_GetWinObj(win, 2, text + 2);
    d2_SetPointerSprite(0, 0);
    sv_RunFrame(0, 0x677c7);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            cost = 1;
            if (d2_PointerObj == low) {
                chance = 0.2f;
            } else if (d2_PointerObj == mid) {
                chance = 0.5f;
                cost = 2;
            } else if (d2_PointerObj == high) {
                chance = 0.8f;
                cost = 4;
            } else {
                continue;
            }
            if (sv_CheckMoney(ai_Chars[ai_PlayerId], cost) == 0)
                continue;
            gm_SendMoneyDelta(ai_CharInts[ai_PlayerId][0], -cost);
            d2_SetViewEnabled(win, 0);
            if (util_RandomF() < chance) {
                for (ev = (AIJOB *)he_FindEvent(1, 0, 0x4f); ev != 0; ev = (AIJOB *)he_NextEvent())
                    if (ev->bldId == bld->id)
                        break;
                if (ev != 0) {
                    ev->exSlot = cm_SendExSetHEI(-1, ev);
                    sv_MessageBox(txt_Ptrs[0x17f7], 4);
                }
                sv_PopRequest = 1;
            } else
                sv_MessageBox(txt_Ptrs[0x17f8], 4);
            d2_SetViewEnabled(win, 1);
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_FireFightingWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5b84f0, the options window of a building "Panel\gebaeude_optionen_pergament" (sv_UpdatePanel: the panel's
   building when no room of it is shown). Run by ai_RunAllows (0x100 for the player's own building, 0x4000 else);
   rebuilt whenever the building's flags 0x5a or its burning event change. Another's building shows its state
   (text 0x17a0 / 0x17fb: the master, the condition texts 0x17b1 / 0x17bb by the level, the equipment value), a
   home its residents (0x17a2) and, but for the classes 3 / 5 and homes, the buy button of a running sale (0x17aa:
   gm_SellBuilding) or the buy offer (0x17ad: ai_BuyBuildingOfferWindow, off while an offer runs, flag 0x200).
   The player's own building (master or owner) shows its state (0x179e / 0x17f9), the master's name input (0x17a1 /
   0x17fc, sent as the field 5 at the end), the sale (0x17a4: ai_SellBuildingWindow) or its withdrawal (0x17a7)
   or of the class 2 the buy-out (0x17af: half of buyOutBase + buyOutExtra to the owner, then the ownership), the renovation
   (0x17a5), the message 0x20 (0x17a6, confirmed by 0x17c7; a reasoned guess: the demolition), the technique tree
   (0x17ac), the room upgrades (0x17a8, when the type takes more than one), the upgrade (0x17ab, off with flag
   0x40), the fire fighting of a burning building (0x17b0) and for its owner the boss of a workshop (0x17a9:
   cm_SendBoss); gm_AllowAll adds the take-over (0x1ba7). Flag 4 disables the technique, renovation, 0x20 and sale
   buttons; ai_GameFlags 0x80 blocks the sub windows
   @deep reconstructed from the stub against the whole original 0x5b84f0..0x5b8fc0: the run masks
   0x100/0x4000 and the campaign steps 0x10, the sale event 0x1b lookup, the per-frame rebuild on flags
   0x5a / the burning event 0x4f, both state texts with the condition texts by the level, every button
   (0x17aa/0x17ad foreign, 0x17a1/0x17fc name input, 0x17af/0x17a4/0x17a7, 0x17a5, 0x17a6, 0x17ac, 0x17a8,
   0x17ab, 0x17b0, 0x17a9 boss, 0x1ba7 take-over), the flag 4/0x40/0x200 enables, the whole click dispatch
   (the sub windows behind ai_GameFlags 0x80, the 0x20 message, gm_SellBuilding, the buy-out with
   cm_SendMoveGoods/cm_SendOwnership/field 0x65, the never-set effects button folded to -1) and the name
   field 5 on close; ours 853 vs 858 ins. Left is the frame layout and allocation: the original's 26 locals
   sit in another slot order (F00 shared by the level and the buy-out amount, frame 0x9d0 vs 0x9d4), the 15
   -1 initialisations are scheduled before push edi in the original, after it in ours, and every button /
   text access differs only by its slot (slots 216, regs 72, frame 48); the code hunks are those slot
   shifts with the instruction order around them (e.g. +0x502..+0x576 the init block, +0x746 fild level
   from another slot, +0xe8f the cm_SendMoveGoods argument order). Tried: the declaration order of the
   original's layout, one variable for level and amount (frame 0x9d0 but code 377) (references: M8 at
   +0x7d3: ours refers to ai_Time+0x4 (0x770ae4), the original's field to 0xc0, explained: the M8 at +0x7d3
   pairs our msg.time copy (ai_Time+4) with the original's store of msg.arg[0] at [esp+0xc0]: an alignment
   artefact of the scheduled AIMSG fill of the 0x20 message; both sides copy ai_Time into msg+0x28 and the
   building id into +0xc / +0x58) */

void ai_BuildingWindow(AIBLD *bld)
{
    int level;
    AIJOB *saleEv;
    int upgradeBtn;
    int sellBtn;
    int offerBtn;
    int cls;
    int roomBtn;
    AIBTYPE *type;
    int demolishBtn;
    int techBtn;
    int nameInput;
    int renovateBtn;
    int value;
    int withdrawBtn;
    int buyOutBtn;
    int bossBtn;
    AIJOB *fireEv;
    int buyBtn;
    int fireBtn;
    int takeOverBtn;
    int lastFlags;
    int bldId;
    int effectsBtn;
    AIMSG msg;
    char name[0x80];
    char text[0x200];
    char buyText[0x200];
    char buyOutText[0x400];
    int win;
    int redraw;
    int cond;
    int price;
    int amount;
    AIBLD *found;
    AIJOB *ev;

    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    lastFlags = -1;
    techBtn = -1;
    renovateBtn = -1;
    sellBtn = -1;
    demolishBtn = -1;
    buyBtn = -1;
    withdrawBtn = -1;
    offerBtn = -1;
    bossBtn = -1;
    roomBtn = -1;
    upgradeBtn = -1;
    nameInput = -1;
    takeOverBtn = -1;
    buyOutBtn = -1;
    fireBtn = -1;
    effectsBtn = -1;
    redraw = 1;
    saleEv = 0;
    fireEv = 0;
    bldId = bld->id;
    if ((bld->flags5aw & 0x400) != 0)
        return;
    if (bld->masterId == ai_PlayerId) {
        if (ai_RunAllows(0x100) == 0)
            return;
    } else if (ai_RunAllows(0x4000) == 0) {
        return;
    }
    if (bld->masterId == ai_PlayerId)
        ai_CampaignStep(0x10, bld, 0, 0);
    cls = gm_BuildingClass(bld->type);
    win = d2_OpenForm(0, 0, "Panel\\gebaeude_optionen_pergament");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    value = gm_EquipmentValue(bld, bld->level);
    gm_MayTakeJob(bld->type, ai_PlayerId);
    if ((bld->flags5a & 2) != 0 && bld->masterId != ai_PlayerId) {
        for (saleEv = (AIJOB *)he_FindEvent(1, 0, 0x1b); saleEv != 0; saleEv = (AIJOB *)he_NextEvent())
            if (saleEv->bldId == bld->id)
                break;
    }
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (bld->flags5aw != lastFlags)
            redraw = 1;
        if (bld->owner == ai_PlayerId) {
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x4f); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->bldId == bld->id)
                    break;
            if (ev != fireEv) {
                redraw = 1;
                fireEv = ev;
            }
        }
        lastFlags = bld->flags5aw;
        if (redraw != 0) {
            techBtn = -1;
            renovateBtn = -1;
            sellBtn = -1;
            demolishBtn = -1;
            buyBtn = -1;
            withdrawBtn = -1;
            offerBtn = -1;
            bossBtn = -1;
            roomBtn = -1;
            upgradeBtn = -1;
            nameInput = -1;
            buyOutBtn = -1;
            fireBtn = -1;
            d2_SetForm(win, 1);
            level = gm_BuildingLevel((char *)bld);
            if (bld->owner != ai_PlayerId && bld->masterId != ai_PlayerId) {
                cond = (int)(9.0f - level * 0.1f + 0.5);
                io_PrintText(type->kind == 0x21 ? 0x17fb : 0x17a0, bld->masterId, bld, cond + 0x17b1,
                             cond + 0x17bb, value);
                if (type->kind == 1)
                    io_PrintText(0x17a2, bld->residents);
                if (cls != 3 && cls != 5 && type->kind != 1) {
                    if (saleEv != 0)
                        buyBtn = d2_GetWinObj(win, 1, io_PrintText(0x17aa));
                    else if (cls != 2 || type->kind == 0x10)
                        offerBtn = d2_GetWinObj(win, 1, io_PrintText(0x17ad));
                    if ((bld->flags5aw & 0x200) != 0)
                        d2_SetObjEnabled(offerBtn, 0);
                }
            }
            if (bld->masterId == ai_PlayerId || bld->owner == ai_PlayerId) {
                cond = (int)(9.0f - level * 0.1f + 0.5);
                io_PrintText(type->kind == 0x21 ? 0x17f9 : 0x179e, bld, cond + 0x17b1, cond + 0x17bb, value);
                if (bld->masterId == ai_PlayerId) {
                    nameInput = d2_GetWinObj(win, 1, io_PrintText(type->kind == 0x21 ? 0x17fc : 0x17a1,
                                                                  bld->name));
                    d2_SetObjMax(nameInput, 0x10);
                }
                if (type->kind == 1)
                    io_PrintText(0x17a2, bld->residents);
                if (cls == 2)
                    buyOutBtn = d2_GetWinObj(win, 1, io_PrintText(0x17af));
                else if ((bld->flags5a & 2) == 0)
                    sellBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a4));
                else
                    withdrawBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a7));
                renovateBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a5));
                if ((bld->flags5a & 0x18) == 0 && cls != 2 && type->kind != 0x10)
                    demolishBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a6));
                if (type->kind != 3 && type->kind != 0x21)
                    techBtn = d2_GetWinObj(win, 1, io_PrintText(0x17ac));
                if (type->extensions > 1)
                    roomBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a8));
                else
                    roomBtn = -1;
                if (cls != 2)
                    upgradeBtn = d2_GetWinObj(win, 1, io_PrintText(0x17ab));
                if (fireEv != 0)
                    fireBtn = d2_GetWinObj(win, 1, io_PrintText(0x17b0));
                if (bld->owner == ai_PlayerId) {
                    d2_SetForm(win, 1);
                    if (type->kind == 2 && ((AICHARREC *)ai_Chars[ai_PlayerId])->bld2 != (int)bld)
                        bossBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a9));
                }
            }
            if (gm_AllowAll != 0)
                takeOverBtn = d2_GetWinObj(win, 1, io_PrintText(0x1ba7));
            redraw = 0;
        }
        if ((bld->flags5a & 4) != 0) {
            d2_SetObjEnabled(techBtn, 0);
            d2_SetObjEnabled(renovateBtn, 0);
            d2_SetObjEnabled(demolishBtn, 0);
            d2_SetObjEnabled(sellBtn, 0);
        }
        if (upgradeBtn != -1) {
            if ((bld->flags5a & 0x40) != 0)
                d2_SetObjEnabled(upgradeBtn, 0);
            else
                d2_SetObjEnabled(upgradeBtn, 1);
        }
        if (dm_L.rup != 0 && d2_FocusedInput == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == techBtn) {
            d2_SetViewEnabled(win, 0);
            ai_TechTreeWindow((int)bld, 0xfd);
            d2_SetViewEnabled(win, 1);
            continue;
        }
        if (d2_PointerObj == sellBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_SellBuildingWindow(bld);
        } else if (d2_PointerObj == renovateBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_RenovationWindow(bld);
        } else if (d2_PointerObj == offerBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_BuyBuildingOfferWindow(bld);
        } else if (d2_PointerObj == withdrawBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_WithdrawSaleWindow(bld);
        } else if (d2_PointerObj == roomBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_RoomUpgradeWindow(bld);
        } else if (d2_PointerObj == bossBtn) {
            d2_SetViewEnabled(win, 0);
            cm_SendBoss(ai_CharInts[ai_PlayerId][0], bld->id);
            d2_SetViewEnabled(win, 1);
            redraw = 1;
            continue;
        } else if (d2_PointerObj == upgradeBtn && (ai_GameFlags & 0x80) == 0) {
            d2_SetViewEnabled(win, 0);
            ai_UpgradeBuilding(bld);
        } else if (d2_PointerObj == demolishBtn && (ai_GameFlags & 0x80) == 0) {
            txt_Format(text, 0x17c7, bld);
            d2_SetViewEnabled(win, 0);
            if (sv_MessageBox(text, 1) != 0) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x20;
                msg.flags = 1;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.arg[0] = bld->id;
                cm_SendLongMessage((char *)&msg);
                sv_PopRequest = 1;
            }
            d2_SetViewEnabled(win, 1);
            redraw = 1;
            continue;
        } else if (d2_PointerObj == buyBtn && (ai_GameFlags & 0x80) == 0) {
            if (gm_MayTakeJob(bld->type, ai_PlayerId) != 1) {
                sv_MessageBox(txt_Ptrs[0x17dd], 0);
                continue;
            }
            d2_SetViewEnabled(win, 0);
            price = saleEv->salePrice;
            if (gm_GetGoodsValue(ai_Chars[ai_PlayerId]) > price) {
                txt_Format(buyText, 0x17db, bld->masterId, bld, price);
                if (sv_MessageBox(buyText, 1) != 0)
                    gm_SellBuilding(bld->id, ai_CharInts[ai_PlayerId][0], saleEv->salePrice, (int)saleEv, 0);
            }
            d2_SetViewEnabled(win, 1);
            redraw = 1;
            continue;
        } else if (d2_PointerObj == effectsBtn) {
            d2_SetViewEnabled(win, 0);
            ai_TechEffectsWindow(bld);
        } else if (d2_PointerObj == takeOverBtn) {
            cm_SendOwnership(bld->id, ai_CharInts[ai_PlayerId][0], ai_CharInts[ai_PlayerId][0]);
            redraw = 1;
            continue;
        } else if (d2_PointerObj == buyOutBtn) {
            amount = bld->buyOutBase + bld->buyOutExtra;
            amount = (int)(amount * 0.5);
            d2_SetViewEnabled(win, 0);
            txt_Format(buyOutText, 0x17e3, bld, bld, amount);
            if (sv_MessageBox(buyOutText, 1) != 0) {
                cm_SendMoveGoods(ai_CharInts[bld->owner][0], -1, ai_OwnPlayer, amount);
                cm_SendOwnership(bld->id, ((AICHARREC *)ai_PlayerChar)->netId, ((AICHARREC *)ai_PlayerChar)->netId);
                cm_BeginFieldBlock((char *)bld, bld->id);
                cm_AddFieldValue(4, 1, (unsigned short)((char *)&bld->buyOutBase - cm_FieldObject), &amount);
                cm_SendFieldDeltas();
                sv_PopRequest = 1;
            }
            d2_SetViewEnabled(win, 1);
            redraw = 1;
            continue;
        } else if (d2_PointerObj == fireBtn) {
            d2_SetViewEnabled(win, 0);
            ai_FireFightingWindow(bld);
        } else {
            continue;
        }
        d2_SetViewEnabled(win, 1);
        redraw = 1;
    } while (sv_RunFrame((int)ai_BuildingWindow, 0x657c7) != 0);
    if (nameInput != -1) {
        strcpy(name, (char *)d2_GetObjValue(nameInput));
        cm_BeginFieldBlock((char *)bld, bld->id);
        cm_AddFieldValue(1, 0x20, 5, name);
        cm_SendFieldValues();
    }
    d2_CloseView(win);
    if (bld->masterId == ai_PlayerId)
        ai_CampaignStep(0x10, bld, 1, 0);
}
/* 0x5b8fc0, the options window of a managed building (sv_UpdatePanel: a building of the classes 3 / 5 with the
   permission 0x10) "Panel\gebaeude_optionen_pergament": sets the research class (sv_SetResearchClass, cleared on
   close) and, rebuilt whenever the building's flags 0x5a change, shows its state (text 0x179e, for another's
   building first 0x17a0: the condition texts 0x17b1 / 0x17bb by the level, the equipment value; a home's residents
   0x17a2) with the renovation (0x17a5), technique tree (0x17ac, not for the type class 3), room upgrade (0x17a8,
   when the type takes more than one) and upgrade (0x17ab, off with flag 0x40) buttons; flag 4 disables the first
   two */

void ai_ManagedBuildingWindow(AIBLD *bld)
{
    int techBtn;
    int renovateBtn;
    int lastFlags;
    int value;
    int roomBtn;
    int bldId;
    AIBTYPE *type;
    int upgradeBtn;
    int redraw;
    int win;
    int cond;
    AIBLD *found;

    upgradeBtn = -1;
    lastFlags = -1;
    redraw = 1;
    techBtn = -1;
    renovateBtn = -1;
    roomBtn = -1;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    bldId = bld->id;
    win = d2_OpenForm(0, 0, "Panel\\gebaeude_optionen_pergament");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    value = gm_EquipmentValue(bld, bld->level);
    sv_SetResearchClass((unsigned char *)bld);
    do {
        found = (AIBLD *)gm_FindBuildingById(bldId);
        if ((bld->flags5aw & 0x400) != 0 || found == 0)
            sv_PopRequest = 1;
        if (bld->flags5aw != lastFlags)
            redraw = 1;
        lastFlags = bld->flags5aw;
        if (redraw != 0) {
            d2_SetForm(win, 1);
            cond = (int)(9 - gm_BuildingLevel((char *)bld) / 10 + 0.5);
            if (bld->owner != ai_PlayerId && bld->masterId != ai_PlayerId) {
                io_PrintText(0x17a0, bld->masterId, bld, cond + 0x17b1, cond + 0x17bb, value);
                if (type->kind == 1)
                    io_PrintText(0x17a2, bld->residents);
            }
            io_PrintText(0x179e, bld, cond + 0x17b1, cond + 0x17bb, value);
            if (type->kind == 1)
                io_PrintText(0x17a2, bld->residents);
            renovateBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a5));
            if (type->kind != 3)
                techBtn = d2_GetWinObj(win, 1, io_PrintText(0x17ac));
            if (type->extensions > 1)
                roomBtn = d2_GetWinObj(win, 1, io_PrintText(0x17a8));
            upgradeBtn = d2_GetWinObj(win, 1, io_PrintText(0x17ab));
            redraw = 0;
        }
        if ((bld->flags5a & 4) != 0) {
            if (techBtn != -1)
                d2_SetObjEnabled(techBtn, 0);
            if (renovateBtn != -1)
                d2_SetObjEnabled(renovateBtn, 0);
        }
        if (upgradeBtn != -1) {
            if ((bld->flags5a & 0x40) != 0)
                d2_SetObjEnabled(upgradeBtn, 0);
            else
                d2_SetObjEnabled(upgradeBtn, 1);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == techBtn) {
            d2_SetViewEnabled(win, 0);
            ai_TechTreeWindow((int)bld, 0xfd);
            d2_SetViewEnabled(win, 1);
        } else if (d2_PointerObj == renovateBtn) {
            d2_SetViewEnabled(win, 0);
            ai_RenovationWindow(bld);
            d2_SetViewEnabled(win, 1);
            redraw = 1;
        } else if (d2_PointerObj == roomBtn) {
            d2_SetViewEnabled(win, 0);
            ai_RoomUpgradeWindow(bld);
            d2_SetViewEnabled(win, 1);
            redraw = 1;
        } else if (d2_PointerObj == upgradeBtn) {
            d2_SetViewEnabled(win, 0);
            ai_UpgradeBuilding(bld);
            d2_SetViewEnabled(win, 1);
            redraw = 1;
        }
    } while (sv_RunFrame((int)ai_ManagedBuildingWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ResearchClass = 0;
}
/* 0x5b92e0, collects a debt of `amount` from the character ch for the netId `to` (f3gm.c gm_IsBankrupt, he.c's
   partnership events): first the character's money as far as it reaches (cm_SendMoveGoods), then sells goods from
   the stores (0x2a, else 0x116) of its buildings of the classes 1 / 2 / 8 at half their price (cm_SendExSellObject),
   all goods but the types 0x17 / 0x25 first and then, from the 0x2a stores only, those two, round after round
   while something sells. The sale proceeds go to `to`, noted in `note` (text 0x18eb appended); without any sale
   the rest is moved unchecked. Returns what was paid
   @deep size queue s16: the two goto exits of the sale rounds (remaining <= 0 inside the building loop)
   are a plain break (the while test ends the round): the same code, kept as the plainer form. The original
   calls cm_SendExSellObject right after the n clamp (as ours) and keeps n in esi (mov eax,esi; cdq; idiv
   ebp: remaining's register reused), so climb.py's best (the call after the sums: 111 ins, raw 1088) is
   not it; both loops' call moved gives raw 1092 (padded right) at 116 ins with another order, not kept.
   Left: register allocation as before. prior: reconstructed from the stub against the whole original
   0x5b92e0..0x5b9727: the money first (min of gm_GetGoodsValue and the amount, cm_SendMoveGoods), the two
   sale rounds over the class 1/2/8 buildings' stores (0x2a else 0x116 for the other goods, 0x2a only for
   the types 0x17/0x25) at half gm_GoodsPrice with the n <= 1 / gm_ListAmount clamps and
   cm_SendExSellObject, the stop flag per round, the proceeds message 0x18eb strcat into the note and the
   rest through cm_SendMoveGoodsUnchecked; ours 359 vs 366 ins. Left is register allocation: the original
   keeps the ai_Goods base in ebp before reusing ebp for the price (+0x406..+0x41c: mov ebp,[ai_Goods]; add
   ebp,edx; add ecx,ebp; mov cl,[ecx] vs ours mov ecx,[ai_Goods]; add ecx,edx; mov cl,[eax+ecx]) and so
   assigns eax/ecx/edx otherwise around cm_SendExSellObject (+0x479..+0x4c0, +0x61d.., the same five pushes
   from other registers) and the secon... */

int ai_LiquidateAssets(int to, AICHARREC *ch, int amount, char *note)
{
    int remaining;
    int paid;
    int stop;
    int sold;
    char text[0x100];
    int value;
    int take;
    int got;
    int n;
    int price;
    char kind;
    int cls;
    unsigned char *bld;
    AIROOM *store;
    unsigned short *item;

    stop = 0;
    paid = 0;
    sold = 0;
    value = gm_GetGoodsValue((char *)ch);
    remaining = amount;
    take = value < amount ? value : amount;
    if (take > 0) {
        cm_SendMoveGoods(to, ch->netId, ai_OwnPlayer, take);
        remaining = amount - take;
        paid = take;
    }
    while (remaining > 0 && stop == 0) {
        bld = gm_BuildQuery(1, 4, ch->id);
        stop = 1;
        for (; bld != 0; bld = gm_BuildNext()) {
            if (remaining <= 0)
                break;
            cls = gm_BuildingClass(*bld);
            if (cls != 1 && cls != 2 && cls != 8)
                continue;
            store = (AIROOM *)gm_Query((void *)((AIBLD *)bld)->objList, 2, 6, 0, 0x2a);
            if (store == 0) {
                store = (AIROOM *)gm_Query((void *)((AIBLD *)bld)->objList, 2, 6, 0, 0x116);
                if (store == 0)
                    continue;
            }
            for (item = (unsigned short *)gm_Query((void *)store->list, 1, 5); item != 0;
                 item = gm_QueryNext()) {
                if (remaining <= 0)
                    break;
                kind = ((AIGOOD *)ai_Goods)[*item].type;
                if (kind == 0x17 || kind == 0x25)
                    continue;
                price = (int)(gm_GoodsPrice(*item, ai_OwnPlayer) * 0.5);
                if (price == 0)
                    continue;
                n = remaining / price;
                if (n <= 1)
                    n = 1;
                if (n >= gm_ListAmount((short *)store, (char *)item))
                    n = gm_ListAmount((short *)store, (char *)item);
                cm_SendExSellObject(-1, store->objId, *item, n, ai_OwnPlayer, 0);
                got = n * price;
                sold += got;
                remaining -= got;
                paid += got;
                if (got != 0)
                    stop = 0;
            }
        }
    }
    stop = 0;
    while (remaining > 0 && stop == 0) {
        bld = gm_BuildQuery(1, 4, ch->id);
        stop = 1;
        for (; bld != 0; bld = gm_BuildNext()) {
            if (remaining <= 0)
                break;
            cls = gm_BuildingClass(*bld);
            if (cls != 1 && cls != 2 && cls != 8)
                continue;
            store = (AIROOM *)gm_Query((void *)((AIBLD *)bld)->objList, 2, 6, 0, 0x2a);
            if (store == 0)
                continue;
            for (item = (unsigned short *)gm_Query((void *)store->list, 1, 5); item != 0;
                 item = gm_QueryNext()) {
                if (remaining <= 0)
                    break;
                kind = ((AIGOOD *)ai_Goods)[*item].type;
                if (kind != 0x17 && kind != 0x25)
                    continue;
                price = (int)(gm_GoodsPrice(*item, ai_OwnPlayer) * 0.5);
                if (price == 0)
                    continue;
                n = remaining / price;
                if (n <= 1)
                    n = 1;
                if (n >= gm_ListAmount((short *)store, (char *)item))
                    n = gm_ListAmount((short *)store, (char *)item);
                cm_SendExSellObject(-1, store->objId, *item, n, ai_OwnPlayer, 0);
                got = n * price;
                paid += got;
                remaining -= got;
                sold += got;
                if (got != 0)
                    stop = 0;
            }
        }
    }
    if (sold != 0) {
        cm_SendMoveGoods(to, -1, ai_OwnPlayer, sold);
        txt_Format(text, 0x18eb, sold);
        strcat(note, text);
        return paid;
    }
    if (remaining > 0)
        cm_SendMoveGoodsUnchecked(to, ch->netId, ai_OwnPlayer, remaining);
    return paid;
}
/* 0x5b9730, the plant bar of a building with a vegetation map (ai_WoodKontorContact's panel button): the ground
   type named "PFLANZBAR" of the terrain map and the plant goods (type 0x17) of the building's main room give the
   bar "Misc\PlantBar" one picture per plant (a radio group, keys 'N' / 'J' step it); the chosen plant's model
   "<data>vegetation/*pfl_<NAME>_04.ogr", half transparent, follows the mouse over the terrain (map_PickScreen),
   uncoloured where it fits on that ground (veg_Fits), red where not; a left release there plants it (cm_SetPlant,
   waiting for the answer), pays half its price for 100 (at least 0x400) and drops the new plant's model onto the
   ground; a right release closes the bar */

void ob_HandleBepflanzung(AIBLD *bld)
{
    char path[0x100];
    char msg[0x100];
    int types[8];                   /* the plant goods of the main room */
    int pics[8];                    /* their pictures on the bar */
    D3MATRIX m;
    D3VECTOR4 above;                /* the engine's padded vectors */
    D3VECTOR4 pos;
    struct { float x, y, z; int w; } pick;  /* the picked point (map_PickScreen fills x / y / z); @original the
                                               fourth word is cleared with the integer zero (esi), the three
                                               floats with the float zero (ebp): not a float as D3VECTOR4's w */
    D3TEXFLAGS tf;
    G15PLANT *plants;
    G15PLANT *plant;
    G15TREE *type;
    D3OBJECT *model;
    D3OBJECT *obj;
    unsigned short *item;
    char *room;
    int ground;                     /* the PFLANZBAR ground type */
    int view;
    int radio;
    int count;
    int sel;
    int last;
    int hit;
    int ok;
    int slot;
    int price;
    int cost;
    int x;
    int z;
    int i;

    model = 0;
    pick.x = 0.0f;
    pick.y = 0.0f;
    pick.z = 0.0f;
    pick.w = 0;
    z = 0;
    x = 0;
    ok = 1;
    ground = 0;
    plants = (G15PLANT *)bld->plants;
    sel = 0;
    type = 0;
    count = 0;
    last = -1;
    radio = -1;
    if (d3_Terrain == 0)
        return;
    for (i = 0; i < 8; i++) {
        if (strstr(((G15MAP *)d3_Terrain)->ground[i], "PFLANZBAR"))
            ground = i;
    }
    room = gm_Query((void *)bld->objList, 1, 0, 0xff);
    if (room == 0)
        return;
    for (item = (unsigned short *)gm_Query((void *)((AIROOM *)room)->list, 1, 4, 0x17); item != 0;
         item = gm_QueryNext())
        types[count++] = *item;
    view = d2_OpenFormS(d2_ScreenW - 0x5f, 0x10a, "Misc\\PlantBar");
    if (view == -1)
        return;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    d2_SetForm(view, 1);
    for (i = 0; i < count; i++) {
        int pic;

        pic = d2_WinOpenPic(0x1a, i * 0x32, types[i] + 0xdf, d2_LastWindow);
        pics[i] = pic;
        if (radio == -1) {
            radio = d2_CreateRadioGroup(1, pic);
            d2_Objs[pic].checked = 1;
            d2_Objs[pic].highlighted = 1;
        } else {
            d2_RadioAdd(radio, pic);
        }
    }
    do {
        hit = map_PickScreen(d3_Terrain, dm_L.x, dm_L.y, 30.0f, (D3VECTOR *)&pick);
        if (hit != -1 && model != 0) {
            /* the picked point from the camera's space into the world */
            x = hit & 0xffff;
            z = hit >> 16;
            d3_MatTranspose(&d3_Camera->matrix, &m);
            pos.x = m._11 * pick.x + m._21 * pick.y + m._31 * pick.z;
            pos.y = m._12 * pick.x + m._22 * pick.y + m._32 * pick.z;
            pos.z = m._13 * pick.x + m._23 * pick.y + m._33 * pick.z;
            pos.x += d3_Camera->pos.x;
            pos.y += d3_Camera->pos.y;
            pos.z += d3_Camera->pos.z;
            d3_SetObjectPos(model, (D3VECTOR *)&pos);
            if (((G15MAP *)d3_Terrain)->cells[((G15MAP *)d3_Terrain)->w * z + x] == ground
                && veg_Fits(plants, x, z, type->size))
                ok = 1;
            else
                ok = 0;
            d3_RelightObject(model);
        }
        if (ok)
            d3t_SetTreeColor(model, 0, 0, 0);
        else
            d3t_SetTreeColor(model, 0xc0, 0x40, 0x40);
        if (dm_L.lup != 0 && hit != -1 && ok) {
            slot = cm_SetPlant(bld->id, x, z, type->id, 0, -1);
            while (cm_GetSlotState(slot) == 0)
                sv_RunFrame(0, 0x677c7);
            plant = (G15PLANT *)cm_GetSlotValue(slot);
            if (plant != 0) {
                price = (int)gm_ItemPrice(type->id, 100);
                cost = (int)(price * 0.5);
                if (cost <= 0x400)
                    cost = 0x400;
                cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, cost);
                veg_LoadModel(plant, 0);
                obj = (D3OBJECT *)plant->obj;
                if (obj != 0) {
                    above.x = pos.x;
                    above.y = pos.y - 60.0;
                    above.z = pos.z;
                    d3_SetObjectPos(obj, (D3VECTOR *)&above);
                    d3t_PlaceObjectV(obj, (D3VECTOR *)&pos, &obj->rotation, 0x32, 0x20);
                    d3_RelightObject(obj);
                }
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lup != 0) {
            for (i = 0; i < count; i++) {
                if (d2_PointerObj == pics[i]) {
                    sel = d2_Radios[radio].sel;
                    break;
                }
            }
        }
        if (kb_Last == 'N') {
            if (++sel > count - 1)
                sel = 0;
            d2_RadioSelect(radio, sel);
        } else if (kb_Last == 'J') {
            if (--sel < 0)
                sel = count - 1;
            d2_RadioSelect(radio, sel);
        }
        sel = d2_Radios[radio].sel;
        if (sel != last) {
            /* the model of the newly chosen plant */
            type = veg_FindType(types[sel]);
            if (model != 0)
                d3if_Delete(model);
            sprintf(path, "%svegetation/*pfl_%s_04.ogr", ai_DataPath, type->name);
            model = d3_LoadGroup(path, 0, 0);
            if (model == 0) {
                sprintf(msg, "ob_HandleBepflanzung(): Could not load 3D-Objekt-roup '%s'...", path);
                err_Message(msg);
                sv_PopRequest = 1;
            } else {
                tf.all = 0;
                tf.b[2] = 1;
                tf.b[0] = 0xdc;
                model->stateFlags &= ~2;
                d3_SetAlpha(model, &tf);
                model->stateFlags |= 2;
            }
            last = sel;
        }
    } while (sv_RunFrame((int)ob_HandleBepflanzung, 0x677cf) != 0);
    if (model != 0)
        d3if_Delete(model);
    d2_RadioClear(radio);
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 1);
    d2_CloseView(view);
}
/* 0x5b9db0, the plant under the pointer (ai_PickedObject) of the building's vegetation map plants, every frame of
   the wood-kontor listener: with characters selected, a left release on a ripe plant (growth state above 1, or
   gm_AllowAll) sends each selected character the harvest event 0x28 (HE_ERNTEN of cm_EventNames: the building,
   the character's netId, the plant's id); else the caption names the plant (text 0x8f with its object name) or
   says it is not ripe yet (text 0x8e) */

void ai_HandlePickedPlant(AIBLD *bld, int plants)
{
    G15PLANT *plant;
    int i;

    if (ai_PickedObject == 0)
        return;
    plant = veg_FindPlantObject((G15PLANT *)plants, ai_PickedObject);
    if (ai_SelectedCount != 0) {
        if (plant == 0)
            return;
        if ((plant->state > 1 || gm_AllowAll != 0) && dm_L.lup != 0) {
            for (i = 0; i < 768; i++) {
                if (((AICHARREC *)ai_Chars)[i].selected != 0) {
                    AIMSG msg;

                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x28;
                    msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.fromId = bld->id;
                    msg.nIds = 1;
                    msg.reserved14 = 0;
                    msg.time = ai_Time;
                    msg.flags = 1;
                    msg.arg[3] = plant->id;
                    msg.arg[0] = bld->id;
                    msg.arg[1] = ((AICHARREC *)ai_Chars)[i].netId;
                    cm_SendLongMessage((char *)&msg);
                    snd_PlayFavourComment();
                }
            }
            return;
        }
    }
    if (plant == 0)
        return;
    if (plant->state > 1) {
        char text[0xf8];

        sprintf(text, txt_Ptrs[0x8f], txt_ObjectNames[plant->type][0]);
        d2_CopyCaption(text);
    } else {
        d2_CopyCaption(txt_Ptrs[0x8e]);
    }
}
/* 0x5b9f30, the feast window of a kontor (contact GELAGE): the form "special\gelage", centred and framed, prints
   the title (text 0x19a9) and the amount (text 0x19aa); its slider runs from 100 to the player's money, at most
   1000; the OK object pays the chosen amount (gm_PlayerFactor of the slider) and sends the feast event 0x38
   (HE_GELAGE of cm_EventNames: the building, the amount) in the message group "party", then closes; the second
   click id or a right release closes it too */

void ai_GelageWindow(AIBLD *bld)
{
    AIMSG msg;
    int win;
    int text;
    int slider;
    int ok;
    int cost;

    win = d2_OpenForm(0, 0, "special\\gelage");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0xe;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x19a9);
    d2_SetForm(win, 2);
    text = io_PrintText(0x19aa, ai_OwnPlayer);
    slider = d2_GetWinObj(win, 2, text);
    ok = d2_GetWinObj(win, 2, text + 1);
    d2_SetObjValue4(slider, 100, min(1000, gm_ConvertMoney(gm_GetGoodsValue(ai_Chars[ai_PlayerId]), ai_OwnPlayer)), 100);
    while (sv_RunFrame((int)ai_GelageWindow, 0x677c7) != 0) {
        if (d2_ClickedId == 0) {
            if (ok != -1 && d2_PointerObj == ok) {
                cost = gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer);
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x38;
                msg.flags = 1;
                msg.arg[0] = cost;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                cm_BeginGroup("party");
                cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, cost);
                cm_SendLongMessage((char *)&msg);
                cm_EndGroup("party");
                sv_PopRequest = 1;
            }
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
/* 0x5ba1c0, opens the inventory child window of the character charId: only while ai_ChildWindowId is -1; clears
   the six ai_InvSlots rows (-1, 0), points ai_InvChar at ai_Chars[charId], creates the child window (x, y, 0xcd,
   0xc4, 0x10, win) as ai_ChildWindowId with the picture 0x587, an inner child window (0, 0x2c, its width, 0x1e,
   0x110) with the font 0x54, and prints the text there */

void ai_OpenInventoryWindow(int win, int x, int y, unsigned short charId, int text)
{
    int i;

    if (ai_ChildWindowId != -1)
        return;
    for (i = 0; i < 6; i++) {
        ai_InvSlots[i].ctl = -1;
        ai_InvSlots[i].obj = 0;
    }
    ai_InvChar = (AICHARREC *)ai_Chars[charId];
    ai_ChildWindowId = d2_CreateChildWindow(x, y, 0xcd, 0xc4, 0x10, win);
    d2_WinOpenPic(0, 0, 0x587, ai_ChildWindowId);
    d2_CreateChildWindow(0, 0x2c, d2_LastWindowPtr->w, 0x1e, 0x110, d2_LastWindow);
    d2_Wins[d2_LastWindow].font = 0x54;
    io_PrintText(text);
}
/* 0x5ba290, the inventory child window's frame: for ai_InvChar, each of the six ai_InvSlots rows whose object
   left the inventory loses its control; the others get their amount (the control's range 0 .. count) and caption
   ("<name>~ <count> <text 0x3fe9>"), a Domizil good in use (active) its control disabled and filled by the part
   of its effect used up (1 - minutesLeft / G9DOMIZIL.minutes), else enabled and empty; an amount picked in a control goes to the pointer (good + 0xdf).
   Every inventory good not of type 9 and not shown yet gets the first free row: an input icon of the child window
   at column k % 3, row k / 3, styled 0x23, with the caption "<name>~ <count> Stück", and the same Domizil state.
   Returns 1 when some amount was picked */

int ai_UpdateInventoryWindow(void)
{
    AICHARREC *ch;
    G8INVOBJ *o;
    G9DOMIZIL *use;
    int picked;
    int value;
    int i;
    int k;

    ch = ai_InvChar;
    picked = 0;
    if (ch == 0)
        return 0;
    for (i = 0; i < 6; i++) {
        if (ai_InvSlots[i].obj == 0)
            continue;
        if (gm_Query((void *)ch->inventory, 1, 0, ai_InvSlots[i].obj->id) == 0) {
            /* the object left the inventory */
            ai_InvSlots[i].obj = 0;
            d2_RemoveObj(ai_InvSlots[i].ctl);
            ai_InvSlots[i].ctl = -1;
            continue;
        }
        value = d2_GetObjValue(ai_InvSlots[i].ctl);
        d2_SetObjValue4(ai_InvSlots[i].ctl, 0, ai_InvSlots[i].obj->count, value);
        sprintf(d2_Objs[ai_InvSlots[i].ctl].caption, "%s~ %li %s", txt_ObjectNames[ai_InvSlots[i].obj->id][0],
                ai_InvSlots[i].obj->count, txt_Ptrs[0x3fe9]);
        use = (G9DOMIZIL *)ai_FindUsableGood(ai_InvSlots[i].obj->id);
        if (use != 0) {
            if (ai_InvSlots[i].obj->active != 0) {
                d2_SetObjEnabled(ai_InvSlots[i].ctl, 0);
                d2_Objs[ai_InvSlots[i].ctl].fillFraction = 1.0 - (double)ai_InvSlots[i].obj->minutesLeft / use->minutes;
            } else {
                d2_SetObjEnabled(ai_InvSlots[i].ctl, 1);
                d2_Objs[ai_InvSlots[i].ctl].greyed = 0;
                d2_Objs[ai_InvSlots[i].ctl].fillFraction = 0;
            }
        }
        if (value != 0) {
            d2_SetPointerItem(ai_InvSlots[i].obj->id + 0xdf, value);
            picked = 1;
        }
    }
    for (o = (G8INVOBJ *)gm_Query((void *)ch->inventory, 1, 5); o != 0; o = (G8INVOBJ *)gm_QueryNext()) {
        if (((AIGOOD *)ai_Goods)[o->id].type == 9)
            continue;
        for (k = 0; k < 6; k++)
            if (ai_InvSlots[k].obj == o)
                break;
        if (k < 6)
            continue;
        for (k = 0; k < 6; k++) {
            if (ai_InvSlots[k].obj == 0) {
                /* the first free row takes the good */
                ai_InvSlots[k].obj = o;
                ai_InvSlots[k].ctl = d2_WinOpenInputIcon(k % 3 * 0x38 + 0x15, k / 3 * 0x36 + 0x4a, o->id + 0xdf, 10,
                                                         ai_ChildWindowId);
                d2_Objs[ai_InvSlots[k].ctl].style = 0x23;
                d2_Objs[ai_InvSlots[k].ctl].captionX = -1;
                d2_Objs[ai_InvSlots[k].ctl].captionY = 0;
                sprintf(d2_Objs[ai_InvSlots[k].ctl].caption, "%s~ %li St\xfc" "ck", txt_ObjectNames[ai_InvSlots[k].obj->id][0],
                        ai_InvSlots[k].obj->count);
                use = (G9DOMIZIL *)ai_FindUsableGood(ai_InvSlots[k].obj->id);
                if (use != 0) {
                    if (ai_InvSlots[k].obj->active != 0) {
                        d2_SetObjEnabled(ai_InvSlots[k].ctl, 0);
                        d2_Objs[ai_InvSlots[k].ctl].fillFraction =
                            1.0 - (double)ai_InvSlots[k].obj->minutesLeft / use->minutes;
                    } else {
                        d2_SetObjEnabled(ai_InvSlots[k].ctl, 1);
                    }
                }
                break;
            }
        }
    }
    return picked;
}
/* 0x5ba600, closes the panel's child window (32 B): clears the pointer items (d2_ClearPointerItems), removes the
   window ai_ChildWindowId and sets it to -1; the transport window and ai_InventoryPanel call it at their exit */

void ai_CloseChildWindow(void)
{
    d2_ClearPointerItems();
    d2_RemoveWindow(ai_ChildWindowId);
    ai_ChildWindowId = -1;
}
/* 0x5ba620, the Domizil table lookup (64 B): walks the 0x1c-byte entries of 0x62c420 while the id at +0 is not
   0; returns the entry of the id, 0 when absent */

int ai_FindUsableGood(int id)
{
    int i;

    for (i = 0; ai_DomizilTable[i].id != 0; i++)
        if (ai_DomizilTable[i].id == id)
            return (int)&ai_DomizilTable[i];
    return 0;
}
/* 0x5ba660, the Domizil table lookup with the index out (80 B): like 0x5ba620, and the index of the entry is
   stored at *index when that is not 0 */

int ai_FindUsableGoodIndex(int id, int *index)
{
    int i;

    for (i = 0; ai_DomizilTable[i].id != 0; i++)
        if (ai_DomizilTable[i].id == id) {
            if (index != 0)
                *index = i;
            return (int)&ai_DomizilTable[i];
        }
    return 0;
}
/* 0x5ba6b0, whether the character's ch inventory holds an object of the type id: 0x4ffe90 (the inventory
   +0x178, 1, 0, id) must find one, its type must have a table record (0x5ba620, the 0x62c420 table) and the
   object's +0x21 must be set. The callers read it as "the character has the skill" (cm.c: "the skill 0x16f" /
   0x2b5) or "has the good" (the item 0x290 gate of the ai message callbacks, the office bonus of f3gm.c) */

int ai_InventoryHas(void *ch, int id)
{
    G8INVOBJ *o;

    o = (G8INVOBJ *)gm_Query((void *)((G8INVCHAR *)ch)->inventory, 1, 0, id);
    if (o != 0 && ai_FindUsableGood(o->id) != 0 && o->active != 0)
        return 1;
    return 0;
}
/* 0x5ba700, whether the building bld holds an item of the type id: 0x4ffe90 (the whole instance table, scan
   all, the prototype id) must find one, its type must have a Domizil record (0x5ba620, the 0x62c420 table), the
   object's +0x21 must be set and its +0x25 must be the building's id. he_RunProduktion halves the production
   factor with the item 0x178 */

int ai_BuildingHasItem(AIBLD *bld, int id)
{
    G8INVOBJ *o;

    o = (G8INVOBJ *)gm_Query(0, 2, 7, 0, id);
    if (o != 0 && ai_FindUsableGood(o->id) != 0 && o->active != 0 && o->bldId == bld->id)
        return 1;
    return 0;
}
/* 0x5ba750, the clock of the Domizil goods in use (cm's frame, while the player is a character): 32 characters
   per call from lc_Settings.reserveCursor on; each Domizil good with a duration (G9DOMIZIL.minutes) that is active loses
   the minutes since its own stamp; when its time runs out it turns inactive, the character of kind 6 gets the
   news 0x1bc9 (_NACHRICHTEN_HS_37, the good's name text), the good 0x178 clears the flag 0x20 of its building's
   word 0x5a, and a character that can act (ai_CanAct) and is not of kind 7 sells it (one, cm_SendExSellObject);
   a good without a duration is set inactive. Stamps lc_Settings.reserveTime and moves the cursor on
   @deep Reconstructed in full from 0x5ba750..0x5ba979: the 32-character window from lc_Settings.reserveCursor, the
   inventory walk, the Domizil duration count-down with the stamp copy, the run-out branch (news 0x1bc9 for
   kind 6, the 0x5a flag 0x20 of the 0x178 good's building, the sale when ai_CanAct and kind != 7), the
   inactive branch, the end stamp and cursor; every call, argument, constant and string read against the
   original; ours 159 vs 160 instructions, frame equal. Left: register rotation only (asmdiff: eax/ecx/edx
   swapped through the body, e.g. +0x86 xor edx,edx / push edx vs xor eax,eax / push eax; the cursor reload
   +0x1c2 mov edx,[cursor]; add edx,0x20 vs mov eax,[cursor]; lea ecx,[eax+0x20]); the two AITIME copies
   (o->since = ai_Time, lc_Settings.reserveTime = ai_Time) load and store the same four parts in another register
   order. A variant round. (references: M8 at +0xd2: ours refers to ai_Time+0x4 (0x770ae4), the original's
   field to 0x770ae0 (ai_Time) (and 5 more), explained: the M8 ref lines are the two 14-byte AITIME copies:
   the same loads and stores of ai_Time and lc_Settings.reserveTime in another order (the alignment pairs ai_Time+4
   with ai_Time); no other object is read or written; the prover stops only at the byte push of
   ai_OwnPlayer through another register (Ins8Lo of a different upper half)) */

void lc_UpdateGoodsReserves(void)
{
    char text[0x400];
    G8INVOBJ *o;
    G9DOMIZIL *use;
    AIBLD *bld;
    int i;

    if (ai_PlayerId >= 0x300)
        return;
    gm_MinutesBetween(&lc_Settings.reserveTime, &ai_Time);
    for (i = lc_Settings.reserveCursor; i < lc_Settings.reserveCursor + 0x20 && i < 0x300; i++) {
        if (((AICHARREC *)ai_Chars)[i].id == 0xffff)
            continue;
        for (o = (G8INVOBJ *)gm_Query((void *)((AICHARREC *)ai_Chars)[i].inventory, 1, 5); o != 0;
             o = (G8INVOBJ *)gm_QueryNext()) {
            use = (G9DOMIZIL *)ai_FindUsableGood(o->id);
            if (use == 0)
                continue;
            if (use->minutes > 0) {
                if (o->active == 0)
                    continue;
                o->minutesLeft -= gm_MinutesBetween(&o->since, &ai_Time);
                o->since = ai_Time;
                if (o->minutesLeft >= 0)
                    continue;
                /* the effect ran out */
                o->active = 0;
                if (((AICHARREC *)ai_Chars)[i].kind == 6) {
                    txt_Format(text, 0x1bc9, o->id * 2 + 0x9a8);
                    he_ShowNews(ai_CharInts[i][0], -1, text, 0, 0x5ec, "_NACHRICHTEN_HS_37");
                }
                if (o->id == 0x178) {
                    bld = (AIBLD *)gm_BuildQuery(1, 1, o->bldId);
                    if (bld != 0)
                        cm_SendSetField(bld->id, 0x5a, 2, 0, 0x20);
                }
                if (ai_CanAct(ai_Chars[i]) && ((AICHARREC *)ai_Chars)[i].kind != 7)
                    cm_SendExSellObject(-1, o->ownerId, o->id, 1, ai_OwnPlayer, 0);
            } else if (o->active != 0) {
                o->active = 0;
            }
        }
    }
    lc_Settings.reserveTime = ai_Time;
    lc_Settings.reserveCursor = (i + 1) % 0x300;
}
/* 0x5ba980, asks whether the player uses the inventory good o (the inventory panel's click on a Domizil good):
   the form "panel\useobj" prints text 0xfe3 (its name and rank) when the good's mask leaves the player's
   goodsMask free, else 0xfe4 (it is in use); the first click id uses it: ai_UseGood for the player with the
   request {o, -1, -1}, returning its effect; the second asks text 0xfe6 and sells every amount picked in the
   inventory rows (cm_SendExSellObject) unless ai_GameFlags has 0x80; a right release closes it. Returns 0 when
   nothing was used */

int ai_UseGoodWindow(G8INVOBJ *o)
{
    char text[0x400];
    G8USEVIEW use = {0};
    int index;
    G9DOMIZIL *rec;
    int win;
    int used;
    int n;
    int i;

    used = 0;
    rec = (G9DOMIZIL *)ai_FindUsableGoodIndex(o->id, &index);
    if (rec != 0) {
        win = d2_OpenForm(0, 0, "panel\\useobj");
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        if ((rec->mask & ((AICHARREC *)ai_Chars)[ai_PlayerId].goodsMask) == 0 && ai_FindUsableGood(o->id) != 0)
            io_PrintText(0xfe3, o->id * 2 + 0x9a8, rec->rank, o->id * 2 + 0x9a8);
        else
            io_PrintText(0xfe4, o->id * 2 + 0x9a8);
        d2_SetPointerSprite(0, 0);
        sv_ResetScreenClip();
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                used = 0;
            }
            if (d2_ClickedId != -1) {
                if (d2_ClickedId == 0) {
                    used = 1;
                    sv_PopRequest = 1;
                } else if (d2_ClickedId == 1 && !(ai_GameFlags & 0x80)) {
                    /* sell the amounts picked in the inventory rows */
                    txt_Format(text, 0xfe6, o->id * 2 + 0x9a8, o->id * 2 + 0x9a8);
                    d2_SetViewEnabled(win, 0);
                    if (sv_MessageBox(text, 1) != 0) {
                        for (i = 0; i < 6; i++) {
                            if (ai_InvSlots[i].obj != 0 && (n = d2_GetObjValue(ai_InvSlots[i].ctl)) != 0) {
                                cm_SendExSellObject(-1, ai_InvChar->netId, ai_InvSlots[i].obj->id, n, ai_OwnPlayer, 0);
                                d2_SetObjValue4(ai_InvSlots[i].ctl, 0, 999, 0);
                            }
                        }
                        d2_ClearPointerItems();
                    }
                    d2_SetViewEnabled(win, 1);
                    used = 0;
                    sv_PopRequest = 1;
                }
            }
        } while (sv_RunFrame((int)ai_UseGoodWindow, 0x677c7) != 0);
        d2_CloseView(win);
        if (!used)
            return 0;
        use.obj = o;
        use.target = -1;
        use.arg = -1;
        use.effect = 0;
        use.handled = 0;
        ai_UseGood((int)ai_Chars[ai_PlayerId], (int *)&use);
    }
    return use.effect;
}
/* 0x5bac10, the player's inventory panel (sv's panel button; only while no child window is open and the run
   allows 0x1000): campaign step 0x14 in and out; the form "panel\inventory_2" at the lower right corner, framed,
   holds the inventory child window of the player (ai_OpenInventoryWindow, text 0x1f3b) and runs it every frame;
   a click on a row's control (or a drop of picked goods on it) opens ai_UseGoodWindow for a Domizil good, else
   says text 0xfe7 that it cannot be used; a right release puts the picked amounts back, or closes the panel */

void ai_InventoryPanel(void)
{
    int win;
    int i;

    if (ai_ChildWindowId != -1)
        return;
    if (ai_RunAllows(0x1000) == 0)
        return;
    ai_CampaignStep(0x14, 0, 0, 0);
    win = d2_OpenFormS(d2_ScreenW - 0x158, d2_ScreenH - 0x108, "panel\\inventory_2");
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0x18;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0x18;
    d2_Wins[d2_LastWindow].padb = 0x18;
    d2_SetForm(win, 0);
    d2_SetPointerSprite(0, 0);
    ai_OpenInventoryWindow(d2_LastWindow, 0, 0, ai_PlayerId, 0x1f3b);
    while (sv_RunFrame((int)ai_InventoryPanel, 0x657c7) != 0) {
        ai_UpdateInventoryWindow();
        if (d2_ClickedId != -1 || d2_CountPointerItems() != 0) {
            for (i = 0; i < 6; i++) {
                if (ai_InvSlots[i].obj == 0 || d2_PointerObj != ai_InvSlots[i].ctl)
                    continue;
                if (ai_FindUsableGood(ai_InvSlots[i].obj->id) != 0) {
                    d2_SetViewEnabled(win, 0);
                    ai_UseGoodWindow(ai_InvSlots[i].obj);
                    d2_SetObjValue4(ai_InvSlots[i].ctl, 0, 999, 0);
                    d2_ClearPointerItems();
                    d2_SetViewEnabled(win, 1);
                } else {
                    sv_MessageBox(txt_Ptrs[0xfe7], 0);
                }
            }
        }
        if (dm_L.rup != 0) {
            if (d2_CountPointerItems() != 0) {
                /* put the picked amounts back */
                for (i = 0; i < 6; i++) {
                    if (ai_InvSlots[i].obj != 0 && d2_GetObjValue(ai_InvSlots[i].ctl) != 0)
                        d2_SetObjValue4(ai_InvSlots[i].ctl, 0, 999, 0);
                }
                d2_ClearPointerItems();
            } else {
                sv_PopRequest = 1;
            }
        }
    }
    ai_CloseChildWindow();
    d2_CloseView(win);
    ai_CampaignStep(0x14, 0, 1, 0);
}

/* 0x5bae10, the init of the equip window's row table (64 B): every row of ai_EquipRows is freed (the character 0,
   the objects -1, the six slots -1 / 0), so that ai_UpdateEquipRows finds no row in use */

void ai_EquipRowsInit(AIBLD *bld)  /* bld: the equip window passes its building; the table is the same for all */
{
    int i, k;
    AIEQUIPROW *row;

    for (i = 0; i < 0x20; i++) {
        row = &ai_EquipRows[i];
        row->ch = 0;
        row->portrait = -1;
        row->frame = -1;
        row->window = -1;
        for (k = 0; k < 6; k++) {
            row->slot[k].pic = -1;
            row->slot[k].obj = 0;
        }
    }
}
#define ai_EquipRowTab ((AIEQUIPROW *)ai_EquipRows)  /* the row table as its rows */
/* 0x5bae50, the per-frame update of the equip window's rows (ai_EquipWindow): a row whose character left the
   scene (ai_SceneChars) is freed with its objects; a shown row drops the slots of goods the character no longer
   holds, prints the name and health ("$C%1N2 %i%%") and the two skill bars (texts 0x16ac / 0x16ad, skills[3] /
   [2]) into its window, and shows each good of the inventory (but those of type 9) in a slot as an input icon with
   the held count; a scene character of the job groups 10 .. 12 without a row gets the first free one (frame,
   portrait, window, text 0x16ab). Returns 1 when a slot holds a picked amount (it goes to the pointer) */


int ai_UpdateEquipRows(int view, int form)
{
    int *sc;
    AIEQUIPROW *row;
    AIOBJ *item;
    AICHARREC *ch;
    int r;
    int i;
    int group;
    int picked;

    picked = 0;
    d2_SetForm(view, form);
    lc_Settings.equipRowCount = 0;
    for (r = 0; r < 0x20; r++) {
        if (ai_EquipRowTab[r].ch == 0)
            continue;
        lc_Settings.equipRowCount++;
        for (sc = ai_SceneChars; (int)sc < (int)&ai_SceneChars[0x20]; sc++)
            if ((int)ai_EquipRowTab[r].ch == *sc)
                goto shown;
        /* the character left the scene: free the row */
        ai_EquipRowTab[r].ch = 0;
        if (ai_EquipRowTab[r].portrait != -1) {
            d2_RemoveObj(ai_EquipRowTab[r].portrait);
            ai_EquipRowTab[r].portrait = -1;
        }
        if (ai_EquipRowTab[r].frame != -1) {
            d2_RemoveObj(ai_EquipRowTab[r].frame);
            ai_EquipRowTab[r].frame = -1;
        }
        if (ai_EquipRowTab[r].window != -1) {
            d2_RemoveObj(ai_EquipRowTab[r].window);
            ai_EquipRowTab[r].window = -1;
        }
        continue;
shown:
        d2_LastWindow = ai_EquipRowTab[r].window;
        for (i = 0; i < 6; i++) {
            if (ai_EquipRowTab[r].slot[i].obj != 0
                && gm_Query((void *)ai_EquipRowTab[r].ch->inventory, 1, 0, ai_EquipRowTab[r].slot[i].obj) == 0) {
                d2_RemoveObj(ai_EquipRowTab[r].slot[i].pic);
                ai_EquipRowTab[r].slot[i].pic = -1;
                ai_EquipRowTab[r].slot[i].obj = 0;
            }
        }
        d2_LastWindow = ai_EquipRowTab[r].window;
        io_PrintText("$C%1N2 %i%%", ai_EquipRowTab[r].ch->id,
                     (int)(gm_HealthRatio((short *)ai_EquipRowTab[r].ch) * 100.0));
        io_PrintText(0x16ac);
        sv_ValueBar(0x50, 0x16, ai_EquipRowTab[r].ch->skills[3], 0x4f1);
        io_PrintText(0x16ad);
        sv_ValueBar(0x50, 0x25, ai_EquipRowTab[r].ch->skills[2], 0x4f1);
        for (item = (AIOBJ *)gm_Query((void *)ai_EquipRowTab[r].ch->inventory, 1, 5); item != 0;
             item = (AIOBJ *)gm_QueryNext()) {
            if (((AIGOOD *)ai_Goods)[item->id].type == 9)
                continue;
            for (i = 0; i < 6; i++)
                if (ai_EquipRowTab[r].slot[i].obj == item->id)
                    goto have;
            /* a new good: the first free slot shows it */
            d2_SetForm(view, form);
            for (i = 0; i < 6; i++) {
                if (ai_EquipRowTab[r].slot[i].obj == 0) {
                    ai_EquipRowTab[r].slot[i].pic = d2_WinOpenInputIcon(i * 0x36 + 0x50, r * 0x78 + 0x28,
                                                                        item->id + 0xdf, 10, d2_LastWindow);
                    ai_EquipRowTab[r].slot[i].obj = item->id;
                    break;
                }
            }
have:
            d2_SetObjValue4(ai_EquipRowTab[r].slot[i].pic, 0, item->count, d2_GetObjValue(ai_EquipRowTab[r].slot[i].pic));
        }
    }
    /* a scene character of the job groups 10 .. 12 without a row gets the first free one */
    for (sc = ai_SceneChars; (int)sc < (int)&ai_SceneChars[0x20]; sc++) {
        if (*sc == 0)
            continue;
        group = gm_TitleJobGroup(((AICHARREC *)*sc)->title);
        if (group < 10 || group > 12)
            continue;
        for (row = ai_EquipRowTab; (int)row < (int)(ai_EquipRowTab + 0x20); row++)
            if ((int)row->ch == *sc)
                goto next;
        for (r = 0, row = ai_EquipRowTab; (int)row < (int)(ai_EquipRowTab + 0x20); row++, r++) {
            if (row->ch == 0) {
                d2_SetForm(view, form);
                ai_EquipRowTab[r].ch = (AICHARREC *)*sc;
                ai_EquipRowTab[r].frame = d2_WinOpenPic(0, r * 0x78, 0x74c, d2_LastWindow);
                d2_Objs[ai_EquipRowTab[r].frame].interactive = 1;
                ai_EquipRowTab[r].portrait = d2_WinOpenPic(0x10, r * 0x78 + 5, ai_EquipRowTab[r].ch->picture, d2_LastWindow);
                ai_EquipRowTab[r].window = d2_CreateChildWindow(0x40, r * 0x78, 0xb4, 0x50, 0x10, d2_LastWindow);
                d2_SetObjFont(d2_Wins[ai_EquipRowTab[r].window].parent, 0x54);
                io_PrintText(0x16ab, ai_EquipRowTab[r].ch->id, ai_EquipRowTab[r].ch->skills[3]);
                break;
            }
        }
next:;
    }
    /* the picked amounts go to the pointer */
    for (r = 0; r < 0x20; r++) {
        if (ai_EquipRowTab[r].ch == 0)
            continue;
        for (i = 0; i < 6; i++) {
            if (ai_EquipRowTab[r].slot[i].pic != -1 && d2_GetObjValue(ai_EquipRowTab[r].slot[i].pic) != 0) {
                d2_AddPointerItem(ai_EquipRowTab[r].slot[i].obj + 0xdf, d2_GetObjValue(ai_EquipRowTab[r].slot[i].pic));
                picked = 1;
            }
        }
    }
    return picked;
}
/* 0x5bb2d0, whether a good fits the building class of a character (112 B): the character's building (+0x16c,
   0 when absent) gives its kind byte into the building-type records 0x7e3f30; their class byte must be 4 for the
   goods 0x172 / 0x174, 0x13 for 0x158 / 0x160 and 0x10 for 0x16e / 0x176; 1 otherwise */

int ai_GoodFitsBuilding(void *ch, int good)
{
    G8CHARREC *chrec;
    AIBTYPE *type;
    AIBLD *bld;

    if (ch != 0 && (bld = (chrec = (G8CHARREC *)ch)->bld) != 0) {
        type = (AIBTYPE *)(ai_BTypes + bld->type * 0x24d);
        if (((good == 0x172 || good == 0x174) && type->kind != 4) ||
            ((good == 0x158 || good == 0x160) && type->kind != 0x13) ||
            ((good == 0x16e || good == 0x176) && type->kind != 0x10))
            return 0;
        return 1;
    }
    return 0;
}
/* 0x5bb350, the equip window "locations\diebesgilde\diebesgilde_equipment" of the building bld (the AUSRUESTEN /
   ub_STAENDER_WAFFEN plates of the thieves' guild and mercenary contacts): nothing when sv_CheckCharacterFlag
   refuses; the building's store (gm_Query kind 0x2a, else 0x116; none: text 0x19de) shows its goods list (a store
   0x116 or goods of type 0x11: ai_UpdateStoreGoods, type 5: ai_UpdateProdGoodsList) and ai_UpdateEquipRows the
   rows of the scene's fighters. A click on a row's frame hands the first goods picked in the first grid to its
   character (only a weapon, ai_FindWeaponRecord, else text 0x19dd; not when the character holds one, 0x19df;
   not when it does not fit the building, ai_GoodFitsBuilding, 0x19e0), then the first one of the second grid;
   goods picked from the rows and dropped on the list go back to the store. A right click puts the picked
   amounts back or closes the window; the row count shows on the scroll bar */

void ai_EquipWindow(AIBLD *bld)
{
    int listShown;
    int bar;
    int win;
    int mask;
    int down;
    int up;
    int picked;
    int i;
    int k;
    AIOBJ *store;
    AIGOOD *g;
    AICHARREC *ch;
    char count[0x100];

    listShown = 0;
    bar = -1;
    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    mask = (sv_FrameMask & 0x10000) ? 0x7d6c6 : 0x677c7;
    store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 0, 0x2a, 6);
    if (store == 0) {
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 0, 0x116, 6);
        if (store == 0) {
            sv_MessageBox(txt_Ptrs[0x19de], 0);
            return;
        }
    }
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_equipment");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 3);
    io_PrintText(0x19db);
    ai_EquipRowsInit(bld);
    d2_SetPointerSprite(0, 0);
    ai_InitProductionTables();
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x72f, 0);
    sv_CreateScrollButtons(0x270, 0x15c, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Objs[bar].font = 0x54;
    g = &((AIGOOD *)ai_Goods)[store->id];
    do {
        if (store->id == 0x116 || g->type == 0x11) {
            listShown = ai_UpdateStoreGoods(bld, store);
            ai_LayoutStoreGoods(win, 1, 1, bld, (unsigned short *)store);
        } else if (g->type == 5) {
            listShown = ai_UpdateProdGoodsList(bld, store);
            ai_LayoutProdGoodsList(win, 1, 1, (int)bld);
        }
        if (d2_ClickedId != -1 && listShown != 0) {
            /* a click on a row's frame: the picked goods go to its character */
            for (i = 0; i < 0x20; i++)
                if (ai_EquipRowTab[i].ch != 0 && ai_EquipRowTab[i].frame == d2_PointerObj)
                    goto clicked;
            goto rows;
clicked:
            ch = ai_EquipRowTab[i].ch;
            if (ch != 0) {
                cm_BeginSendGroup();
                d2_EndInputEdit();
                for (k = 0; k < 16; k++) {
                    if (ai_ProdGoods[k].check == -1 || d2_GetObjValue(ai_ProdGoods[k].check) == 0)
                        continue;
                    if (ai_FindWeaponRecord(ai_ProdGoods[k].good) != 0) {
                        if (((AIWEAPON *)cm_CharWeapon(ch))->id != 0) {
                            /* the character holds a weapon already */
                            sv_MessageBox(txt_Ptrs[0x19df], 4);
                            continue;
                        }
                        if (ai_GoodFitsBuilding(ch, ai_ProdGoods[k].good) != 0) {
                            cm_SendExSellObject(ch->netId, store->objId, ai_ProdGoods[k].good, 1, ai_OwnPlayer, 0);
                            d2_SetObjValue(ai_ProdGoods[k].check, 0, ai_ProdGoods[k].amount,
                                           d2_GetObjValue(ai_ProdGoods[k].check) - 1, 0);
                        } else {
                            sv_MessageBox(txt_Ptrs[0x19e0], 4);
                        }
                    } else {
                        sv_MessageBox(txt_Ptrs[0x19dd], 0);
                    }
                    break;
                }
                for (k = 0; k < 8; k++) {
                    if (ai_ProdGoods2[k].check == -1 || d2_GetObjValue(ai_ProdGoods2[k].check) == 0)
                        continue;
                    if (ai_FindWeaponRecord(ai_ProdGoods2[k].good) != 0) {
                        if (ai_GoodFitsBuilding(ch, ai_ProdGoods2[k].good) != 0) {
                            cm_SendExSellObject(ch->netId, store->objId, ai_ProdGoods2[k].good, 1, ai_OwnPlayer, 0);
                            d2_SetObjValue(ai_ProdGoods2[k].check, 0, ai_ProdGoods2[k].amount,
                                           d2_GetObjValue(ai_ProdGoods2[k].check) - 1, 0);
                        } else {
                            sv_MessageBox(txt_Ptrs[0x19e0], 0);
                        }
                    } else {
                        sv_MessageBox(txt_Ptrs[0x19dd], 0);
                    }
                    break;
                }
                cm_EndSendGroup();
                while (cm_SendGroupState() == 0)
                    sv_PumpNet();
                cm_UpdateHandObject(ch);
            }
        }
rows:
        picked = ai_UpdateEquipRows(win, 2);
        if (d2_HitWindow == d2_Views[win].wins[1] && picked != 0 && dm_L.lup != 0) {
            /* goods picked from the rows and dropped on the list go back to the store */
            cm_BeginSendGroup();
            for (i = 0; i < 0x20; i++) {
                if (ai_EquipRowTab[i].ch == 0)
                    continue;
                for (k = 0; k < 6; k++)
                    if (ai_EquipRowTab[i].slot[k].pic != -1 && d2_GetObjValue(ai_EquipRowTab[i].slot[k].pic) != 0)
                        cm_SendExSellObject(store->objId, ai_EquipRowTab[i].ch->netId, ai_EquipRowTab[i].slot[k].obj,
                                            1, ai_OwnPlayer, 0);
            }
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
            for (i = 0; i < 0x20; i++)
                if (ai_EquipRowTab[i].ch != 0)
                    cm_UpdateHandObject(ai_EquipRowTab[i].ch);
        }
        itoa(lc_Settings.equipRowCount, count, 10);
        d2_SetObjText(bar, count);
        if (dm_L.rup != 0) {
            if (d2_CountPointerItems() == 0) {
                sv_PopRequest = 1;
            } else {
                /* put the picked amounts back */
                d2_ClearPointerItems();
                for (k = 0; k < 16; k++)
                    if (ai_ProdGoods[k].check != -1 && d2_GetObjValue(ai_ProdGoods[k].check) != 0)
                        d2_SetObjValue(ai_ProdGoods[k].check, 0, ai_ProdGoods[k].amount, 0, 0);
                for (k = 0; k < 8; k++)
                    if (ai_ProdGoods2[k].check != -1 && d2_GetObjValue(ai_ProdGoods2[k].check) != 0)
                        d2_SetObjValue(ai_ProdGoods2[k].check, 0, ai_ProdGoods2[k].amount, 0, 0);
                for (i = 0; i < 0x20; i++) {
                    if (ai_EquipRowTab[i].ch == 0)
                        continue;
                    for (k = 0; k < 6; k++)
                        if (ai_EquipRowTab[i].slot[k].pic != -1)
                            d2_SetObjValue4(ai_EquipRowTab[i].slot[k].pic, 0, 0, 0);
                }
            }
        }
    } while (sv_RunFrame((int)ai_EquipWindow, mask) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
#define lc_CharRecs ((AICHARREC *)ai_Chars)  /* the character records as AICHARREC */

typedef struct LCTRAINROW {             /* 0xc bytes, a row of the training window (ai_TrainingWindow keeps three) */
    int pic;                            /* 0x00, the training's picture (0x762 + the row), interactive */
    int frame;                          /* 0x04, the row's frame picture 0x74c */
    int window;                         /* 0x08, the row's child window (font 0x54) */
} LCTRAINROW;

/* 0x5bb950, opens a row of the training window in win (ai_TrainingWindow, the rows 0 .. 2 with the pictures 0x762 ..
   0x764): the frame 0x74c at the row's height (0x78 per row), the picture beside it (interactive, style 3) and a
   child window right of it; returns the frame */

int ai_OpenTrainingRow(int n, int pic, LCTRAINROW *row, int win)
{
    int y;

    y = n * 0x78;
    row->frame = d2_WinOpenPic(0, y, 0x74c, win);
    row->pic = d2_WinOpenPic(0xd, y + 5, pic, win);
    d2_Objs[row->pic].interactive = 1;
    d2_Objs[row->pic].style = 3;
    row->window = d2_CreateChildWindow(0x40, y, 0xcd, 0x57, 0x10, win);
    d2_Wins[row->window].font = 0x54;
    return row->frame;
}
/* 0x5bba00, the training window "special\training" of the building bld (2368 B; the TRAINING plate, panel text
   0x1a46): three rows (ai_OpenTrainingRow, the pictures 0x762 .. 0x764) on the background "Training_DIEB" (the
   building classes 4 / 0x10) or "Training_SOLDAT", fading in over the first frames. The rows 0 / 1 train thieves of
   the classes 2 / 3 (the thief message 0x16, enabled by the rooms 0x35 / 0x34), the row 2 the characters of the
   event 0x6c (the class 4 needs the room 0x5d, the class 0x10 the room 0x5e); each row shows its count. A row
   clicked with characters selected sends them (sv_MoveToGroup, cm_SendLongMessage) up to three thieves in all
   (text 0x1a4a past it, 0x1a49 when none went); clicked without, it selects the row's characters
   @deep size queue s20: the frame's extra slot (ours 0x93c, the original 0x938) is a strength-reduced
   pointer temp of the row click loop (c2frame: tmp@8845 = &rows[row], stored at [esp+0x18] and reloaded
   per pass); the original indexes rows[row].pic from row each pass (lea edx,[edi+edi*2]; mov
   eax,[esp+edx*4+0x24]). = control: count = n2 + n3 / count = n3; count += n2 / count = 0; count += ..;
   separate en0 / en1 enable flags instead of row / n2 reuse (136, slots +4); the click search as a break
   loop + if (row < 3) (a meaning change, not kept). climb.py round 1: nothing (every mutant changes the
   frame). The M8s at +0x3e6 / +0x4e9 are the ai_Time struct copy scheduled in another order (all four
   parts are read on both sides). prior: Reconstructed whole from 0x5bba00..0x5bc0eb (523 vs 517
   instructions): the form, the three ai_OpenTrainingRow rows, the DIEB / SOLDAT background, the
   room-enable queries 0x35 / 0x34 / 0x5d / 0x5e, the alpha fade (min(elapsed/750, 0.4), test ah,5 / jnp
   matched), the right click, the 0x16 / 0x6c event counts and captions, the send loops (thief message 0x16
   with memset, 0x6c without), the selection branch, the closing calls; every callee, constant and string
   reads equal. Left: (1) the click loop over rows: ours strength-reduces rows[row].pic into a pointer kept
   in a slot (0x5bbd52 '- lea edx,[esp+0x28]; mov [esp+0x18],edx; mov eax,[esp+0x18]; mov eax,[eax]' | '+
   lea edx,[edi+edi*2]; mov eax,[esp+edx*4+0x24]'; 0x5bbf94 '- mov ecx,[esp+0x18]; add ecx,0xc') which
   costs the extra slot (frame 0x93c vs 0x938, all frame/slot hunks); tried the if-block form and the
   operand order: no change. (2) the msg.time struct copy scheduling in both send loops (ours loads ai_Time
   / +4 / +0xc in another order: 0x5bbde8 '- mov eax,[ai_Time]' | '+ mov eax,[ai_TimeHourW]'); tried 8
   statement orders of type/to/fromId/time/flags/arg0: none better. Allocation and scheduling only.
   (references: M8 at +0x3e6: ours refers to ai_Time (0x770ae0), the original's field to 0x770ae4
   (ai_TimeHourW) (and 1 more), explained: the two M8 lines are the 14-byte AITIME copy msg.time = ai_Time
   read in another order (crosswise loads of the same struct's words into the same msg words); every
   store's target and source match) */

void ai_TrainingWindow(AIBLD *bld)
{
    int row;                            /* first the row 0's enable flag, then the row clicked */
    int n2;                             /* first the row 1's enable flag, then the thieves of the class 2 */
    int win;
    int n3;
    int start;
    LCTRAINROW rows[3];
    __int64 elapsed;
    AIMSG msg;
    char text[0x800];
    AIBTYPE *type;
    int parent;
    int en2;
    int n6c;
    int i;
    int count;
    int cls;
    double alpha;
    AICHARREC *ch;
    AIJOB *ev;

    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    en2 = 0;
    row = 0;
    n2 = 0;
    win = d2_OpenForm(0, 0, "special\\training");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x72f, 0);
    d2_SetForm(win, 3);
    io_PrintText(0x1a46);
    sv_OpenPlayerBar((int)bld, 1);
    d2_SetForm(win, 2);
    parent = d2_LastWindow;
    ai_OpenTrainingRow(0, 0x762, &rows[0], parent);
    ai_OpenTrainingRow(1, 0x763, &rows[1], parent);
    ai_OpenTrainingRow(2, 0x764, &rows[2], parent);
    d2_SetForm(win, 1);
    if (type->kind == 4 || type->kind == 0x10)
        d2_SetBackgroundTexture("Training_DIEB", d2_LastWindowPtr->id);
    else
        d2_SetBackgroundTexture("Training_SOLDAT", d2_LastWindowPtr->id);
    d2_LastWindowPtr->alpha = 0;
    start = d3_Time;
    if (gm_Query((void *)bld->objList, 2, 6, 0, 0x35) != 0)
        row = 1;
    if (gm_Query((void *)bld->objList, 2, 6, 0, 0x34) != 0)
        n2 = 1;
    if (type->kind == 4) {
        if (gm_Query((void *)bld->objList, 2, 6, 0, 0x5d) != 0)
            en2 = 1;
    } else if (type->kind == 0x10) {
        if (gm_Query((void *)bld->objList, 2, 6, 0, 0x5e) != 0)
            en2 = 1;
    } else {
        en2 = 1;
    }
    if (row != 0)
        d2_SetObjEnabled(rows[0].pic, 1);
    else
        d2_SetObjEnabled(rows[0].pic, 0);
    if (n2 != 0)
        d2_SetObjEnabled(rows[1].pic, 1);
    else
        d2_SetObjEnabled(rows[1].pic, 0);
    if (en2 != 0)
        d2_SetObjEnabled(rows[2].pic, 1);
    else
        d2_SetObjEnabled(rows[2].pic, 0);
    do {
        d2_SetForm(win, 1);
        elapsed = (unsigned)(d3_Time - start);
        alpha = elapsed * 0.0013333333333333333;
        d2_LastWindowPtr->alpha = (float)(alpha < 0.4 ? alpha : 0.4);
        if (dm_L.rup != 0) {
            if (d2_CountPointerItems() == 0)
                sv_PopRequest = 1;
            else
                sv_ClearSelection();
        }
        /* the thieves of the classes 2 / 3 and the characters of the event 0x6c on their way */
        n2 = 0;
        n3 = 0;
        for (ev = (AIJOB *)he_FindEvent(2, 0, 0x16, 3, bld->id); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->thiefClass == 2)
                n2++;
            else if (ev->thiefClass == 3)
                n3++;
        }
        n6c = 0;
        for (ev = (AIJOB *)he_FindEvent(2, 0, 0x6c, 3, bld->id); ev != 0; ev = (AIJOB *)he_NextEvent())
            n6c++;
        d2_LastWindow = rows[0].window;
        io_PrintText("$C %i %ii %1s$A %1S", n2, 0x51e, 0x1a47, 0x16b9);
        d2_LastWindow = rows[1].window;
        io_PrintText("$C %i %ii %1s$A %1S", n3, 0x51e, 0x1a47, 0x16ba);
        d2_LastWindow = rows[2].window;
        io_PrintText("$C %i %ii %1s", n6c, 0x51e, 0x1a48);
        if (d2_ClickedId != -1 && d2_CountPointerItems() != 0) {
            /* a row clicked with characters selected: send them */
            for (row = 0; row < 3; row++) {
                if (rows[row].pic == -1 || d2_PointerObj != rows[row].pic)
                    continue;
                count = n3 + n2;
                if (row == 0 || row == 1) {
                    for (i = 0; i < 0x300; i++) {
                        if (lc_CharRecs[i].selected == 0)
                            continue;
                        memset(&msg, 0, sizeof(msg));
                        if (lc_CharRecs[i].job != 0 && lc_CharRecs[i].job->kind == 0x16)
                            count--;
                        if (count >= 3) {
                            count = 4;
                            goto sent;
                        }
                        msg.type = 0x16;
                        msg.flags = 1;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.fromId = bld->id;
                        msg.time = ai_Time;
                        if (sv_PanelShown != 0)
                            msg.arg[2] = ((AIOBJ *)sv_PanelShown)->objId;
                        else
                            msg.arg[2] = -1;
                        msg.arg[1] = lc_CharRecs[i].netId;
                        msg.thiefClass = (row != 0) + 2;
                        sv_MoveToGroup((unsigned char *)bld, 0, lc_CharRecs[i].id, 0);
                        cm_SendLongMessage((char *)&msg);
                        count++;
                    }
                } else {
                    for (i = 0; i < 0x300; i++) {
                        if (count >= 3)
                            break;
                        if (lc_CharRecs[i].selected == 0)
                            continue;
                        msg.type = 0x6c;
                        msg.to = ai_CharInts[bld->owner][0];
                        msg.fromId = bld->id;
                        msg.time = ai_Time;
                        msg.flags = 1;
                        msg.arg[0] = lc_CharRecs[i].netId;
                        if (sv_PanelShown != 0 && *sv_PanelShown != 0xfd)
                            msg.arg[6] = ((AIOBJ *)sv_PanelShown)->objId;
                        else
                            msg.arg[6] = -1;
                        sv_MoveToGroup((unsigned char *)bld, 0, lc_CharRecs[i].id, 0);
                        cm_SendLongMessage((char *)&msg);
                        count++;
                    }
                }
                if (count != 0) {
sent:
                    snd_PlayFavourComment();
                    sv_ClearSelection();
                    if (count > 3) {
                        txt_Format(text, 0x1a4a, 3);
                        sv_MessageBox(text, 0);
                    }
                    break;
                }
                sv_MessageBox(txt_Ptrs[0x1a49], 0);
            }
        } else if (d2_ClickedId != -1 && d2_CountPointerItems() == 0) {
            /* a row clicked without a selection: select its characters */
            for (row = 0; row < 3; row++)
                if (rows[row].pic != -1 && d2_PointerObj == rows[row].pic)
                    goto select;
            continue;
select:
            {
                if (row != 2) {
                    cls = (row != 0) + 2;
                    for (ev = (AIJOB *)he_FindEvent(2, 0, 0x16, 3, bld->id); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->thiefClass == cls && (ch = (AICHARREC *)gm_FindChar(ev->thief)) != 0)
                            ch->selected = 1;
                    }
                } else {
                    for (ev = (AIJOB *)he_FindEvent(2, 0, 0x6c, 3, bld->id); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if ((ch = (AICHARREC *)gm_FindChar(ev->ownerCh)) != 0)
                            ch->selected = 1;
                    }
                }
            }
        }
    } while (sv_RunFrame((int)ai_TrainingWindow, 0x677c7) != 0);
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
    sv_ClearSelection();
}
/* 0x5bc0f0, the release of the picture 0x6f26ac (32 B): 0x478fc0 frees it, the id is reset */

void ai_FreePicture(void)
{
    if (rt_MapSurface != 0)
        d2_FreeSurface(rt_MapSurface);
    rt_MapSurface = 0;
}
/* 0x5bc110, the coat of arms choice "misc\choosewappen" (ai_GameFrame, when a relative of the player took the
   player's crest): the eight crests 0x59a .. 0x5a1 as a radio group, each disabled while another player character
   (kind 5 .. 7) bears it (AICHARREC.ref54); the OK button (text 0x7f) is enabled with a crest picked. OK sets the
   player's crest (cm field block on ref54) and, when no other player character bears it, closes the window and
   sends the system message 0xd with the player's id */

void ai_ChooseCoatOfArms(void)
{
    int ok;
    int group;
    int value;
    int win;
    int arms[8];
    int msg[0x1f];
    int i;
    int k;
    int taken;
    int slot;
    int c;
    AICHARREC *ch;

    win = d2_OpenForm(0, 0, "misc\\choosewappen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x85);
    d2_SetForm(win, 2);
    group = d2_CreateRadioGroup(0);
    for (i = 0; i < 8; i++) {
        arms[i] = d2_WinOpenPic(i * 0x30 + 0xa, 0x78, i + 0x59a, d2_LastWindow);
        d2_RadioAdd(group, arms[i]);
    }
    /* the crests other player characters bear (the original counts the crest down against the button up) */
    for (i = 0; i < 8; i++) {
        for (c = 0; c < 0x300; c++) {
            if (lc_CharRecs[c].id != 0xffff && lc_CharRecs[c].ref54 == 0x59a - i && lc_CharRecs[c].id != ai_PlayerId
                && (lc_CharRecs[c].kind == 6 || lc_CharRecs[c].kind == 7 || lc_CharRecs[c].kind == 5))
                d2_SetObjEnabled(arms[i], 0);
        }
    }
    d2_SetForm(win, 3);
    ok = d2_GetWinObj(win, 3, io_PrintText(0x7f));
    d2_Radios[group].sel = -1;
    sv_ResetScreenClip();
    do {
        for (i = 0; i < 8; i++) {
            d2_SetObjEnabled(arms[i], 1);
            for (c = 0; c < 0x300; c++) {
                if (lc_CharRecs[c].id != 0xffff && lc_CharRecs[c].ref54 == i + 0x59a && lc_CharRecs[c].id != ai_PlayerId
                    && (lc_CharRecs[c].kind == 6 || lc_CharRecs[c].kind == 7 || lc_CharRecs[c].kind == 5))
                    d2_SetObjEnabled(arms[i], 0);
            }
        }
        if (d2_Radios[group].sel != -1)
            d2_SetObjEnabled(ok, 1);
        else
            d2_SetObjEnabled(ok, 0);
        if (d2_ClickedId == 0 && d2_Radios[group].sel != -1) {
            ch = &lc_CharRecs[ai_PlayerId];
            value = d2_Radios[group].sel + 0x59a;
            taken = 0;
            cm_BeginFieldBlock((char *)ch, ch->netId);
            cm_AddFieldValue(4, 1, (unsigned short)((char *)&ch->ref54 - cm_FieldObject), &value);
            slot = cm_SendFieldValues();
            while (cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            for (k = 0; k < 8; k++) {
                for (c = 0; c < 0x300; c++) {
                    if (lc_CharRecs[c].id != 0xffff && lc_CharRecs[c].ref54 == d2_Radios[group].sel + 0x59a && lc_CharRecs[c].id != ai_PlayerId
                        && (lc_CharRecs[c].kind == 6 || lc_CharRecs[c].kind == 7 || lc_CharRecs[c].kind == 5))
                        taken = 1;
                }
            }
            if (taken == 0) {
                sv_PopRequest = 1;
                memset(msg, 0, sizeof(msg));
                msg[0] = ai_PlayerId;
                cm_SendExSysMessage(0xd, msg);
            }
        }
    } while (sv_RunFrame((int)ai_ChooseCoatOfArms, 0x677c7) != 0);
    d2_RadioClear(group);
    d2_CloseView(win);
}

/* 0x5bc420, opens the money page "Panel\geld_info_2" of the player stats panel (ai_PlayerStatsPanel's first
   entry): the captions 0x1f3e / 0x1f3f / 0x1f40, the player's currencies (ai_FillExchangeRows of the inventory
   into ai_StatsMoneyRows) and the player's loans (gm_CreditEvents, the text 0x18cb at nine) as rows of a frame
   0x75a and a child window with the text 0x1f41 (the bank, the amount, the interest, the due date) under scroll
   buttons; returns the form */

int ai_OpenMoneyInfoPage(void)
{
    int rowW;
    int down;
    int n;
    int up;
    int bar;
    HELOAN *loans[9];
    CREDITLISTROW rows[9];
    int win;
    int i;
    char *bank;

    up = -1;
    down = -1;
    bar = -1;
    for (i = 0; i < 16; i++) {
        ai_StatsMoneyRows[i].icon = -1;
        ai_StatsMoneyRows[i].good = 0;
    }
    for (i = 0; i < 9; i++) {
        rows[i].child = -1;
        rows[i].pic = -1;
        rows[i].loan = 0;
    }
    win = d2_OpenForm(0, 0, "Panel\\geld_info_2");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 2);
    io_PrintText(0x1f3e);
    d2_SetForm(win, 4);
    io_PrintText(0x1f3f);
    d2_SetForm(win, 5);
    io_PrintText(0x1f40);
    d2_SetForm(win, 1);
    ai_FillExchangeRows(lc_CharRecs[ai_PlayerId].inventory, ai_StatsMoneyRows, win, 1, 0);
    for (i = 0; i < 9; i++) {
        if (rows[i].child != -1) {
            d2_RemoveWindow(rows[i].child);
            d2_RemoveObj(rows[i].pic);
            rows[i].child = -1;
            rows[i].pic = -1;
            rows[i].loan = 0;
        }
    }
    gm_CreditEvents(ai_PlayerId, 0, loans);
    n = gm_CreditEvents(ai_PlayerId, 0, loans);
    d2_SetForm(win, 3);
    io_PrintText("$C");
    if (n >= 9)
        io_PrintText(0x18cb);
    rowW = d2_Pics[0x75a].w;
    d2_SetForm(win, 0);
    sv_CreateScrollButtons(0x215, 0x16b, d2_LastWindow, d2_GetViewWin(win, 3), 0x6a, &down, &up, &bar);
    for (i = 0; i < n; i++) {
        d2_SetForm(win, 3);
        rows[i].pic = d2_WinOpenPic((d2_LastWindowPtr->w - rowW) >> 1, i * 0x6a, 0x75a, d2_LastWindow);
        rows[i].child = d2_CreateChildWindow((d2_LastWindowPtr->w - rowW) >> 1, i * 0x6a + 4, rowW, 0x62, 0x10,
                                             d2_LastWindow);
        rows[i].loan = loans[i];
        d2_Wins[rows[i].child].font = 0x56;
        d2_Wins[rows[i].child].ml = 0x10;
        bank = gm_FindBuildingById(((AIJOB *)rows[i].loan)->bldId);
        if (bank != 0)
            io_PrintText(0x1f41, bank, rows[i].loan->amount, rows[i].loan->interest, &rows[i].loan->due);
    }
    return win;
}

/* 0x5bc6d0, opens the building list page "panel\geb_liste_2" of the player stats panel (its second entry): the
   caption 0xb2, the player's rows (ai_ShowBuildingRows into ai_StatsBldRows, their count in ai_StatsBldCount) under
   scroll buttons; returns the form */

int ai_OpenBuildingListPage(void)
{
    int bar;
    int up;
    int down;
    int win;
    AICHARREC *ch;

    ch = &lc_CharRecs[ai_PlayerId];
    win = d2_OpenForm(0, 0, "panel\\geb_liste_2");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0xb2);
    d2_SetForm(win, 2);
    ai_StatsBldCount = ai_ShowBuildingRows(ch, 0x80, ai_StatsBldRows);
    d2_SetForm(win, 0);
    sv_CreateScrollButtons(0x206, 0x167, d2_LastWindow, d2_GetViewWin(win, 2), 0x40, &down, &up, &bar);
    return win;
}
/* 0x5bc790, the building detail window "panel\player_stats_geb_detail" (a row of the stats panel's building page
   clicked): the building's name, its level (text 0xa3) and the lines of its balance (gm_BuildingBalance) that are
   not zero: 0xa4, 0xa5, the master's wage 0xa6 (with the first job's name), the title wages 0xa7, then
   0xa8 .. 0xad, the round sum 0xae (0xb3 for the building classes 4 / 0x10 / 0x13), 0xaf, and the result as a
   profit 0xb0 or a loss 0xb1; it stays until a right click */

void ai_BuildingDetailWindow(AIBLD *bld)
{
    int balance[0x15];
    AIBTYPE *type;
    int win;

    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    win = d2_OpenForm(0x3c, 0, "panel\\player_stats_geb_detail");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText("$[%1G$]", bld);
    d2_SetForm(win, 1);
    gm_BuildingClass(bld->type);
    gm_BuildingBalance((unsigned char *)bld, ai_PlayerId, balance);
    io_PrintText(0xa3, gm_BuildingLevel((char *)bld));
    if (balance[0] != 0)
        io_PrintText(0xa4, balance[0]);
    if (balance[2] != 0)
        io_PrintText(0xa5, balance[2]);
    if (balance[6] != 0)
        io_PrintText(0xa6, type->jobs[0] + 0x130, balance[6]);
    if (balance[4] + balance[5] != 0)
        io_PrintText(0xa7, balance[4] + balance[5]);
    if (balance[8] != 0)
        io_PrintText(0xa8, balance[8]);
    if (balance[0xc] != 0)
        io_PrintText(0xa9, balance[0xc]);
    if (balance[0xf] != 0)
        io_PrintText(0xaa, balance[0xf]);
    if (balance[0xd] != 0)
        io_PrintText(0xab, balance[0xd]);
    if (balance[0xe] != 0)
        io_PrintText(0xac, balance[0xe]);
    if (balance[0x13] != 0)
        io_PrintText(0xad, balance[0x13]);
    if (balance[0x12] != 0) {
        if (type->kind == 4 || type->kind == 0x10 || type->kind == 0x13)
            io_PrintText(0xb3, balance[0x12]);
        else
            io_PrintText(0xae, balance[0x12]);
    }
    if (balance[1] != 0)
        io_PrintText(0xaf, balance[1]);
    if (balance[0x14] > 0)
        io_PrintText(0xb0, balance[0x14]);
    else if (balance[0x14] < 0)
        io_PrintText(0xb1, balance[0x14] + balance[0x12]);
    while (sv_RunFrame((int)ai_BuildingDetailWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
}
int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win);  /* 0x46a360, d2.h's prototype (d2.h is not included yet: Open: head) */

/* 0x5bc9c0, opens the master list page "panel\meister_liste_2" of the player stats panel (its third entry): the
   captions 0x1934 / 0x1935 / 0x1936 and per building of the player (gm_BuildQuery(1, 4, id)) whose master is not
   the player a row of ai_MasterRows: the type's picture, the type and name ("%s~ %s"), the master's budget ("%T")
   and an input box 1000 .. 9000 on the building's budget; returns the form, or -1 (closed) without rows */

int ai_OpenMasterListPage(void)
{
    int count;
    AICHARREC *master;
    char text[0x100];
    int i;
    AIBLD *bld;
    int win;

    count = 0;
    win = d2_OpenForm(0, 0, "panel\\meister_liste_2");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1934);
    for (i = 0; i < 0x80; i++) {
        ai_MasterRows[i].pic = -1;
        ai_MasterRows[i].input = -1;
        ai_MasterRows[i].budgetText = -1;
        ai_MasterRows[i].nameText = -1;
        ai_MasterRows[i].budget = 0;
    }
    d2_SetForm(win, 0);
    sv_CreateScrollButtons(0x206, 0x167, d2_LastWindow, d2_GetViewWin(win, 2), 0x3e, &ai_MasterDown, &ai_MasterUp,
                           &ai_MasterBar);
    d2_Wins[d2_LastWindow].btnup = -1;
    d2_Wins[d2_LastWindow].btndown = -1;
    d2_SetForm(win, 3);
    io_PrintText(0x1935);
    d2_SetForm(win, 4);
    io_PrintText(0x1936);
    d2_SetForm(win, 2);
    bld = (AIBLD *)gm_BuildQuery(1, 4, ai_PlayerId);
    if (bld != 0) {
        do {
            master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
            if (master != 0 && master->id != ai_PlayerId) {
                d2_SetForm(win, 2);
                ai_MasterRows[count].pic = d2_WinOpenPic(0, count * 0x3e, bld->type + 0x427, d2_LastWindow);
                txt_Format(text, "%s~ %s", bld->type * 14 + 0x48e, bld->name);
                ai_MasterRows[count].nameText = d2_CreateWinText(0x40, count * 0x3e + 8, text, d2_LastWindow);
                d2_Objs[ai_MasterRows[count].nameText].w = 0xa0;
                d2_SetObjFont(ai_MasterRows[count].nameText, 0x54);
                txt_Format(text, "%T", master->budget);
                ai_MasterRows[count].budgetText = d2_CreateWinText(0x180, count * 0x3e + 0x14, text, d2_LastWindow);
                ai_MasterRows[count].input = d2_WinOpenInput(0xea, count * 0x3e + 0x14, 0x40, 0x40, 0x82, d2_LastWindow);
                d2_SetObjValue4(ai_MasterRows[count].input, 1000, 9000, gm_ConvertMoney(bld->budget, ai_OwnPlayer));
                ai_MasterRows[count].budget = d2_GetObjValue(ai_MasterRows[count].input);
                ai_MasterRows[count].bld = bld;
                count++;
            }
        } while ((bld = (AIBLD *)gm_BuildNext()) != 0);
    }
    if (count == 0) {
        d2_CloseView(win);
        return -1;
    }
    return win;
}

void * d2_SetFont(int font); void d2_DrawTextBox(); void d2_FillPaintbox(int x, int y, int w, int h); int gv_GetTableRecord();
int gm_EquipmentValue(); void d2_RadioSelect(int g, int sel);
int ai_PassesPartnerFilter(unsigned short me, unsigned short id, AMOUNTCFG *cfg, int unused);

/* 0x5bcc70, the player stats panel "Panel\player_stats" (1000 B, run by ai_RunAllows(0x800), campaign step 0x13):
   the button row of the texts 0x3fdd .. 0x3fdf over its three pages, the money page (ai_OpenMoneyInfoPage), the
   building list page (ai_OpenBuildingListPage) and the master list page (ai_OpenMasterListPage, its button off
   without masters) in a radio group that starts on the page picked last (ai_StatsPage). Per frame: shows the
   picked page, refills the money rows, opens the building detail window for a clicked building row, scrolls the
   master list by its buttons and sends a changed master budget input to the building (AIBLD.budget, in the
   player's money: gm_PlayerFactor) and the change to its master (AICHARREC.budget)
   @deep near-byte sweep s22: left win in the slot above the one the original gives it ([esp+0xc] vs
   [esp+0x8]) and two reloads after the master loop in swapped registers. value as a local of the if / the
   loop body = control (frame sweep lever does not reach it). */
void ai_PlayerStatsPanel(void)
{
    int out[3];
    int value;
    int win;
    int moneyPage;
    int bldPage;
    int masterPage;
    int radio;
    int i;
    int down;
    int list;
    AICHARREC *master;

    if (ai_RunAllows(0x800) == 0)
        return;
    ai_CampaignStep(0x13, 0, 0, 0);
    win = d2_OpenForm(0, 0, "Panel\\player_stats");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 3);
    sv_ButtonRow(d2_LastWindow, 0, out, 3, txt_Ptrs[0x3fdd], txt_Ptrs[0x3fde], txt_Ptrs[0x3fdf]);
    moneyPage = ai_OpenMoneyInfoPage();
    bldPage = ai_OpenBuildingListPage();
    masterPage = ai_OpenMasterListPage();
    radio = d2_CreateRadioGroup(3, out[0], out[1], out[2]);
    d2_Radios[radio].flags |= 1;
    d2_RadioSelect(radio, ai_StatsPage);
    if (masterPage == -1)
        d2_SetObjEnabled(out[2], 0);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_Radios[radio].sel == 0) {
            d2_SetViewEnabled(moneyPage, 1);
            d2_SetViewEnabled(bldPage, 0);
            d2_SetViewEnabled(masterPage, 0);
        } else if (d2_Radios[radio].sel == 1) {
            d2_SetViewEnabled(bldPage, 1);
            d2_SetViewEnabled(moneyPage, 0);
            d2_SetViewEnabled(masterPage, 0);
        } else if (d2_Radios[radio].sel == 2) {
            d2_SetViewEnabled(masterPage, 1);
            d2_SetViewEnabled(bldPage, 0);
            d2_SetViewEnabled(moneyPage, 0);
        }
        if (moneyPage != -1) {
            d2_SetForm(moneyPage, 1);
            ai_FillExchangeRows(lc_CharRecs[ai_PlayerId].inventory, ai_StatsMoneyRows, moneyPage, 1, 0);
        }
        if (bldPage != -1 && d2_ClickedId != -1) {
            for (i = 0; i < ai_StatsBldCount; i++) {
                if (d2_PointerObj == ai_StatsBldRows[i]) {
                    d2_EnableViewObjects(win, 0);
                    ai_BuildingDetailWindow((AIBLD *)d2_Objs[d2_PointerObj].user);
                    d2_EnableViewObjects(win, 1);
                    break;
                }
            }
        }
        if (masterPage != -1) {
            d2_SetForm(masterPage, 0);
            if (d2_ClickedId != -1) {
                down = d2_GetViewWin(masterPage, 3);
                list = d2_GetViewWin(masterPage, 2);
                if (d2_PointerObj == ai_MasterDown) {
                    d2_ScrollWindowBy(0, 0x3e, down);
                    d2_ScrollWindowBy(0, 0x3e, list);
                } else if (d2_PointerObj == ai_MasterUp) {
                    d2_ScrollWindowBy(0, -0x3e, down);
                    d2_ScrollWindowBy(0, -0x3e, list);
                }
            }
            for (i = 0; i < 0x80; i++) {
                if (ai_MasterRows[i].input != -1 && ai_MasterRows[i].budget != d2_GetObjValue(ai_MasterRows[i].input)) {
                    value = gm_PlayerFactor(d2_GetObjValue(ai_MasterRows[i].input), ai_OwnPlayer);
                    cm_BeginFieldBlock((char *)ai_MasterRows[i].bld, ai_MasterRows[i].bld->id);
                    cm_AddFieldDelta(4, 1, (unsigned short)((char *)&ai_MasterRows[i].bld->budget - cm_FieldObject), &value);
                    cm_SendFieldDeltas();
                    master = (AICHARREC *)gm_FindCharacterByMaster((int)ai_MasterRows[i].bld);
                    value -= ai_MasterRows[i].bld->budget;
                    cm_BeginFieldBlock((char *)master, master->netId);
                    cm_AddFieldValues(4, 1, (char *)&master->budget - cm_FieldObject, &value);
                    cm_SendFieldDeltas();
                    ai_MasterRows[i].budget = d2_GetObjValue(ai_MasterRows[i].input);
                }
            }
        }
    } while (sv_RunFrame((int)ai_PlayerStatsPanel, 0x657c7) != 0);
    ai_StatsPage = d2_Radios[radio].sel;
    d2_RadioClear(radio);
    d2_CloseView(win);
    if (moneyPage != -1)
        d2_CloseView(moneyPage);
    if (bldPage != -1)
        d2_CloseView(bldPage);
    if (masterPage != -1)
        d2_CloseView(masterPage);
    ai_CampaignStep(0x13, 0, 1, 0);
}

/* the use callbacks of the Domizil table (lcamt.c defines them; prototypes here until lcamt.h can carry the
   AICHARREC / G8USEVIEW views) */
int ai_UseGoodAddMoney(AICHARREC *ch);
int ai_UseGoodHeal(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodInfectOwner(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodSendHome(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodWeakenSkills(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodWeakenSkills2(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodCureAilment(AICHARREC *ch);
int ai_UseGoodHalveWorkerValue(AICHARREC *ch, G8USEVIEW *use);
int ai_TrialStep1(G8CHARREC *ch, G8USEVIEW *use);
int ai_TrialStep3(G8CHARREC *ch, G8USEVIEW *use);
int ai_TrialStep4(G8CHARREC *ch, G8USEVIEW *use);
int ai_UseGoodRaiseRelation(AICHARREC *ch, G8USEVIEW *use);
int ai_QuarrelCallback(AICHARREC *ch, int *req);
int ai_UseGoodTrialStep(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodLowerOfficialRelations(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodTwoTrialSteps(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodCharmOtherSex(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodSowDiscord(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodTributeClaim(AICHARREC *ch);
int ai_UseGoodDrainActionPoints(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodDrainSkill(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodHarmBuilding(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodSpoilStock(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodCharmOfficials(AICHARREC *ch, G8USEVIEW *use);
int ai_UsePayGood(G8CHARREC *ch);
int ai_UseGoodDrainActionPoints8(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodHoldRound(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodCharmPartner(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodWeakenSkills3(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodFrameChar(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodFrameChar2(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodHeatBuilding(AICHARREC *ch, G8USEVIEW *use);
int ai_UseEventGood(G8CHARREC *ch);
int ai_UseGoodStaffBonus(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodPettyLoss(AICHARREC *ch, G8USEVIEW *use);
int ai_UseGoodSkillBonus(AICHARREC *ch);
int ai_UseGoodDistrust(AICHARREC *ch, G8USEVIEW *use);
/* the unit's data in the original's order: .rdata gm_SpecialItemGroups 0x5eed94 and its count (this object's named
   constants, after lc.c's literals 52.0f / 0.435f / 0.48f; its literal 128.0 follows), .data ai_ChildWindowId 0x62c418
   (the first of the block, after lc.c's strings) .. "panel\useobj", .bss 0x6f2f60 .. 0x6f2f84 and the common rows */
const G7GRP gm_SpecialItemGroups[3] = { { 0x1f, { 0x1cd, 0x1ce, 0x1cf, 0, 0, 0, 0, 0, 0, 0 } }, { 0x20, { 0x1d0, 0x1d1, 0x1d2, 0x1d3, 0, 0, 0, 0, 0, 0 } }, { 0x1e, { 0x1ca, 0x1cb, 0x1cc, 0, 0, 0, 0, 0, 0, 0 } } };  /* 0x5eed94, the item groups of the good kinds 0x1f, ' ' and 0x1e (3 x 0x15 bytes; lc.c's ai_FindItemGroup walks them to 0x5eedd3) */
const int gm_SpecialItemGroupCount = 3;  /* 0x5eedd4, the number of gm_SpecialItemGroups; no reader in the surviving code (ai_FindItemGroup compares to the end address). Name a guess from the value and the place */
int ai_ChildWindowId = -1;  /* 0x62c418, the child window ai_OpenInventoryWindow creates (d2_CreateChildWindow over its character list), -1: none; ai_CloseChildWindow removes it, the transport window and ai_InventoryPanel compare it with the clicked id */
G9DOMIZIL ai_DomizilTable[0x42] = {  /* 0x62c420, the 0x42 entries of 0x1c: id +0, minutes +4, law +8, weight +0xc, mask +0x10, use +0x14, rank +0x18; ai_FindUsableGood / ai_FindUsableGoodIndex walk it to the id 0, lc_UpdateGoodsReserves counts the minutes down and ai_UseGood calls the use callback and prints the rank text */
    { 0x155, 4320, -1, 2.0f, 0x00000000, 0, 0x101a, { 1, 0 } },
    { 0x157, 4320, -1, 3.0f, 0x00000000, 0, 0x101e, { 2, 0 } },
    { 0x159, 4320, -1, 4.0f, 0x00000000, 0, 0x1022, { 3, 0 } },
    { 0x15a, 4320, -1, 4.0f, 0x00000000, 0, 0x1024, { 3, 0 } },
    { 0x15b, 5760, -1, 3.0f, 0x00000000, 0, 0x1026, { 1, 0 } },
    { 0x15c, 4320, -1, 2.5f, 0x00000000, (void *)ai_UseGoodTributeClaim, 0x1028, { 1, 0 } },
    { 0x15d, 7200, -1, 2.5f, 0x00000000, 0, 0x102a, { 2, 0 } },
    { 0x15f, 5760, -1, 2.5f, 0x00000000, 0, 0x102e, { 3, 0 } },
    { 0x161, 0, -1, 2.0f, 0x00000001, (void *)ai_UseGoodHalveWorkerValue, 0x1032, { 1, 0 } },
    { 0x162, 0, -1, 1.0f, 0x00000002, (void *)ai_TrialStep1, 0x1034, { 1, 0 } },
    { 0x163, 0, -1, 1.0f, 0x00000004, (void *)ai_TrialStep3, 0x1036, { 2, 0 } },
    { 0x164, 0, -1, 1.0f, 0x00000008, (void *)ai_TrialStep4, 0x1038, { 2, 0 } },
    { 0x165, 4320, -1, 5.0f, 0x00000000, 0, 0x103a, { 3, 0 } },
    { 0x166, 1440, -1, 1.0f, 0x00000000, 0, 0x103c, { 3, 0 } },
    { 0x167, 0, -1, 2.0f, 0x00000010, (void *)ai_UseGoodRaiseRelation, 0x103e, { 1, 0 } },
    { 0x169, 0, 3, 6.0f, 0x00000020, (void *)ai_QuarrelCallback, 0x1042, { 2, 0 } },
    { 0x168, 0, -1, 2.5f, 0x00000040, (void *)ai_UseGoodTrialStep, 0x1040, { 1, 0 } },
    { 0x16a, 0, 3, 3.5f, 0x00000080, (void *)ai_UseGoodLowerOfficialRelations, 0x1044, { 2, 0 } },
    { 0x16b, 4320, -1, 3.0f, 0x00000000, 0, 0x1046, { 3, 0 } },
    { 0x16c, 0, -1, 3.8f, 0x00000100, (void *)ai_UseGoodTwoTrialSteps, 0x1048, { 3, 0 } },
    { 0x16f, 1440, 17, 1.5f, 0x00000000, 0, 0x104e, { 2, 0 } },
    { 0x16d, 0, 17, 2.0f, 0x00040000, (void *)ai_UseGoodSowDiscord, 0x104a, { 1, 0 } },
    { 0x170, 0, 17, 1.5f, 0x00000200, (void *)ai_UseGoodCharmOtherSex, 0x1050, { 2, 0 } },
    { 0x171, 1440, 17, 1.5f, 0x00000000, 0, 0x1052, { 3, 0 } },
    { 0x173, 0, 18, 1.0f, 0x00000400, (void *)ai_UseGoodAddMoney, 0x1056, { 1, 0 } },
    { 0x175, 0, 18, 4.0f, 0x00000800, (void *)ai_UseGoodHeal, 0x105a, { 2, 0 } },
    { 0x177, 180, 24, 1.2f, 0x00001000, (void *)ai_UseGoodInfectOwner, 0x105e, { 3, 0 } },
    { 0x178, 180, 24, 1.9f, 0x00002000, (void *)ai_UseGoodSendHome, 0x1060, { 3, 0 } },
    { 0x17b, 1440, 18, 1.5f, 0x00004000, (void *)ai_UseGoodWeakenSkills, 0x1066, { 2, 0 } },
    { 0x17c, 1400, 18, 1.5f, 0x00000000, (void *)ai_AlwaysTrue, 0x1068, { 2, 0 } },
    { 0x17d, 1440, 18, 1.5f, 0x00010000, (void *)ai_UseGoodWeakenSkills2, 0x106a, { 3, 0 } },
    { 0x17e, 0, 18, 1.5f, 0x00020000, (void *)ai_UseGoodCureAilment, 0x106c, { 3, 0 } },
    { 0x229, 0, -1, 1.5f, 0x00080000, (void *)ai_UseGoodCharmPartner, 0x106e, { 1, 0 } },
    { 0x22a, 2880, -1, 1.5f, 0x00000000, 0, 0x1070, { 1, 0 } },
    { 0x22c, 2880, -1, 1.5f, 0x00000000, (void *)ai_UseGoodWeakenSkills3, 0x1074, { 2, 0 } },
    { 0x22e, 1440, -1, 1.5f, 0x00000000, 0, 0x1078, { 3, 0 } },
    { 0x22b, 0, -1, 1.5f, 0x00100000, (void *)ai_UseGoodFrameChar, 0x1072, { 2, 0 } },
    { 0x22d, 0, -1, 1.5f, 0x00200000, (void *)ai_UseGoodFrameChar2, 0x1076, { 3, 0 } },
    { 0x297, 1440, -1, 1.5f, 0x00000000, (void *)ai_UseGoodSkillBonus, 0x107a, { 1, 0 } },
    { 0x298, 0, -1, 1.5f, 0x00400000, (void *)ai_UseGoodStaffBonus, 0x107c, { 1, 0 } },
    { 0x299, 90, -1, 1.5f, 0x00000000, 0, 0x107e, { 2, 0 } },
    { 0x29a, 1440, -1, 1.5f, 0x00000000, (void *)ai_UseGoodDistrust, 0x1080, { 2, 0 } },
    { 0x29b, 4320, -1, 1.5f, 0x00000000, 0, 0x1082, { 3, 0 } },
    { 0x29c, 1440, 28, 1.5f, 0x00000000, (void *)ai_UseGoodPettyLoss, 0x1084, { 3, 0 } },
    { 0x2b2, 0, -1, 1.5f, 0x01000000, (void *)ai_UseGoodHeatBuilding, 0x1086, { 1, 0 } },
    { 0x2b3, 2880, -1, 1.5f, 0x00000000, 0, 0x1088, { 1, 0 } },
    { 0x2b4, 7200, -1, 1.5f, 0x00000000, 0, 0x108a, { 2, 0 } },
    { 0x2b5, 8640, -1, 1.5f, 0x00000000, 0, 0x108c, { 2, 0 } },
    { 0x2b6, 4320, -1, 1.5f, 0x00000000, 0, 0x108e, { 3, 0 } },
    { 0x2b7, 4320, -1, 1.5f, 0x00000000, (void *)ai_UseEventGood, 0x1090, { 3, 0 } },
    { 0x2b8, 0, -1, 1.5f, 0x00800000, (void *)ai_UseGoodDrainActionPoints, 0x1092, { 4, 0 } },
    { 0x2b9, 1440, -1, 1.5f, 0x00000000, 0, 0x1094, { 4, 0 } },
    { 0x2ba, 1440, 18, 1.5f, 0x00000000, 0, 0x1096, { 4, 0 } },
    { 0x2bb, 0, -1, 1.5f, 0x04000000, (void *)ai_UseGoodDrainSkill, 0x1098, { 4, 0 } },
    { 0x2bc, 0, -1, 1.5f, 0x08000000, (void *)ai_UseGoodHarmBuilding, 0x109a, { 4, 0 } },
    { 0x2bd, 0, -1, 1.5f, 0x10000000, (void *)ai_UseGoodSpoilStock, 0x109c, { 5, 0 } },
    { 0x2be, 1440, 17, 1.5f, 0x00000000, 0, 0x109e, { 5, 0 } },
    { 0x2bf, 0, 17, 1.5f, 0x20000000, (void *)ai_UseGoodCharmOfficials, 0x10a0, { 5, 0 } },
    { 0x2c0, 2880, 17, 1.5f, 0x00000000, 0, 0x10a2, { 5, 0 } },
    { 0x2c1, 1440, 18, 1.5f, 0x00000000, (void *)ai_UsePayGood, 0x10a4, { 5, 0 } },
    { 0x2c2, 0, -1, 1.5f, 0x40000000, (void *)ai_UseGoodDrainActionPoints8, 0x10a6, { 6, 0 } },
    { 0x2c3, 0, -1, 1.5f, 0x80000000, (void *)ai_UseGoodHoldRound, 0x10a8, { 6, 0 } },
    { 0x2c4, 2880, -1, 1.5f, 0x00000000, 0, 0x10aa, { 6, 0 } },
    { 0x2c5, 2880, -1, 1.5f, 0x00000000, 0, 0x10ac, { 6, 0 } },
    { 0x2c6, 1440, 18, 1.5f, 0x00000000, 0, 0x10ae, { 6, 0 } },
    { 0x0, 0, -1, 0.0f, 0x00000000, 0, 0x0, { 0, 0 } },
};
int ai_DomizilCount = 0x42;  /* 0x62cb58, the number of 0x1c-byte entries of the Domizil table ai_DomizilTable (0x62c420) */
GAPDATA full_data_62cb5c[88] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x01, 0x01, 0xff, 0x01, 0xff, 0xff, 0x00, 0x00, 0x01, 0x02, 0x00, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0x00, 0x03, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0xff, 0x01, 0x04, 0x00, 0xff, 0x00, 0x00, 0xff, 0x01, 0x00, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 };  /* 0x62cb5c, the rest of the 92-byte block at 0x62cb58 (no reader; lcdom's .data after the count) */
struct AICHARREC *ai_InvChar = 0;  /* 0x6f2f60, the character whose inventory the inventory child window (ai_OpenInventoryWindow) lists: ai_OpenInventoryWindow points it at ai_Chars[id]; the transport window sends its goods by its netId */
LCSETTINGS lc_Settings = { 0 };  /* 0x6f2f64, the escort kind of the last transport order, the goods-reserve clock and cursor and the equip window's row count (see LCSETTINGS) */
int ai_StatsBldCount = 0;  /* 0x6f2f80, the rows of the stats panel's building page ai_ShowBuildingRows filled (ai_OpenBuildingListPage stores it, ai_PlayerStatsPanel loops over it) */
int ai_StatsPage = 0;  /* 0x6f2f84, the stats panel's page selected last (ai_PlayerStatsPanel selects the radio group with it and stores the radio's sel back) */
LCEXCHROW ai_StatsMoneyRows[16];  /* 0x6f3680, the currency rows of the stats panel's money page (ai_FillExchangeRows fills them, ai_OpenMoneyInfoPage) */
LCINVSLOT ai_InvSlots[6];  /* 0x6f3840, the six rows of the inventory child window (ai_OpenInventoryWindow clears them: -1, 0, and fills them from the character's inventory) */
int ai_MasterDown;  /* 0x6f3870, the master list page's scroll down button (ai_PlayerStatsPanel passes it to sv_CreateScrollButtons and compares d2_PointerObj with it) */
AIEQUIPROW ai_EquipRows[0x20];  /* 0x6f3880, the equip window's rows: ai_EquipRowsInit frees all 32, ai_UpdateEquipRows fills and updates them, ai_EquipWindow lays them out; 32 x 0x40 runs to 0x6f4080 */
int ai_MasterUp;  /* 0x6f4080, the master list page's scroll up button (sv_CreateScrollButtons) */
LCBLDCARTS ai_BldCartList;  /* 0x6f40a0, the building window's cart list: count, carts, buttons, texts (lc.h) */
LCMASTERROW ai_MasterRows[0x80];  /* 0x6f40e0, the master list page's rows: ai_PlayerStatsPanel fills one per building of the player whose master is not the player (128 rows of 0x18) */
int ai_StatsBldRows[0x80];  /* 0x6f4ce0, the control of each row of the stats panel's building page (ai_ShowBuildingRows fills it, at most ai_StatsBldCount; ai_PlayerStatsPanel marks the clicked one) */
int ai_MasterBar;  /* 0x6f4ee0, the master list page's scroll bar (ai_PlayerStatsPanel passes it to sv_CreateScrollButtons and compares d2_PointerObj with it) */
int ai_MapHeight;  /* 0x6f4ee4, the height of the town map bitmap (ai_MapSize its width), set by the map loader 0x5b2090; owner by place: the commons between lcdom's group and lc's (lcdom or lcmap, both before lc) */
int ai_MapSize;  /* 0x6f4ee8 */
