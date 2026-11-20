/* lcamt -- the special-amount lists and the panels after them (0x5bd060 .. 0x5d33b0), split from lc; the name is
   ours. The row helpers (ai_AmountLimit, the text controls, ai_ShowWindowRow, the loan / building rows), the
   special-amount list and tabbed window (ai_AmountTabs: ai_Fill*Tab / ai_Show*Tab), the amount menus, the master
   letter and work settings, the law book, the statistics charts and panels (ai_CityStatsPanel), the privilege,
   espionage, interrogation and courtship asks, and the use callbacks of the Domizil table (ai_UseGood*).
   Boundary: lc's .data strings end with 0x5bcc70's 0x62d134; this object's plain .data block starts with
   ai_AmountTabs 0x62d148, its .bss with ai_StatsChartFlags 0x6f2f88 (after lc's 0x6f2f84), its .rdata with the named
   int ai_AmountTabCount 0x5eede0 after lc's literal 128.0 (named constants come first in an object). The first
   function is a reasoned guess: 0x5bd060 is the first after the stats panel 0x5bcc70 (lc's last string user) and
   below 0x5bd470 (the first user of this object's .data); ai_CityStatsPanel's dword load of the short
   ai_StatsChartFlags shows this unit defines it.
   Open: layout: the first function within (0x5bcc70, 0x5bd470]: none of 0x5bd060 .. 0x5bd330 has data of its own (only
   0x5bd250's pooled svgrave string); bprobe at 0x5bd0c0 .. 0x5bd470 drops no byte function (ai_CharInfoPanel and
   ai_ChangeProfessionWindow move one instruction further away when the helpers are unknown: weak, for 0x5bd060).
   The callers do not decide: ai_CentredText only from this unit, ai_AmountLimit / ai_ShowWindowRow / ai_ShowBuildingRows
   only from lc / lcdom, the text controls from both.
   Open: the head declarations are lc.c's as the split copied them (types, views, prototypes); trim them to what
   the unit uses once lc.h / lcamt.h carry the shared ones. */
#define GAPDATA __declspec(align(1)) unsigned char

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
#include "lcamt.h"
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
#define PF_KIND67   0x100000             /* the partner filter's one-of bits (AMOUNTCFG.mode, ai_PassesPartnerFilter): kind 6 / 7 */
#define PF_KIND5    0x200000             /* kind 5 */
#define PF_SERVANT  0x400000             /* not kind 5, working for a master (ai_WorksForMaster) */
#define PF_KIND0    0x800000             /* kind 0 */
#define PF_ONE_OF   0xf00000             /* one of these must hold */
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
void d2_RemoveObj(); int gm_HeadIconNumber(); int gm_ObjectBuilding(int y);
void d2_SetObjFont(); void d2_SetCaption();
void d2_RadioSelect(int g, int sel);
void d2_CenterObj(); void gm_FreeObject(int o); int gm_LevelPayout(int v, int kind);
#pragma pack(push, 1)
typedef struct G8SCENECHAR {            /* the scene character fields this unit reads (ch.h CHARACTER) */
    char pad00[0x34];
    D3OBJECT *body;                     /* 0x34, the body (CHARACTER.obj) */
    char pad38[0xf0];
    void *actions;                      /* 0x128, the action queue (CHARACTER.actions) */
} G8SCENECHAR;
#pragma pack(pop)
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
int gv_HeldCrimesOf(int culprit, int *out, unsigned short t);  /* 0x4f3fe0, gv.h's prototype (gv.h is not included yet: Open: head) */
void gv_SortCrimesByLaw(int n, CRIME *r);  /* 0x4f4390, gv.h's prototype (gv.h is not included yet: Open: head) */
int gv_RankCulpritsAll(int *ids, int *open, int *total, unsigned short t);  /* 0x4f3dd0, gv.h's prototype (gv.h is not included yet: Open: head) */
void ai_RightAlignedText(char *text, int x, int y, int w, int font);
int ai_BribeAmountCB(AICHARREC *ch);
int gv_HeldCrimeCount(int culprit, unsigned short t);  /* 0x4f4130, gv.h's prototype (gv.h is not included yet: Open: head) */
G11REC *hmt_GetRecord(unsigned short k);                    /* 0x4ac1e0, hmt.h's prototypes (hmt.h is not included yet: Open: head) */
unsigned short hmt_RecordTextId(unsigned short k, unsigned short n); /* 0x4ac1a0 */
int hmt_GetPrice(struct AICHAR *p);                         /* 0x4ac200 */
void hmt_ApplyRecord(struct AICHAR *p, int d, unsigned short k); /* 0x4ac2f0 */
int ai_EvalFest(AIGRP *g, int send);   /* 0x494ac0, am.h's prototype (am.h is not included yet: Open: head) */
#define lc_CharRecs ((AICHARREC *)ai_Chars)  /* the character records as AICHARREC */
typedef struct LCTRAINROW {             /* 0xc bytes, a row of the training window (ai_TrainingWindow keeps three) */
    int pic;                            /* 0x00, the training's picture (0x762 + the row), interactive */
    int frame;                          /* 0x04, the row's frame picture 0x74c */
    int window;                         /* 0x08, the row's child window (font 0x54) */
} LCTRAINROW;
int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win);  /* 0x46a360, d2.h's prototype (d2.h is not included yet: Open: head) */
void * d2_SetFont(int font); void d2_DrawTextBox(); void d2_FillPaintbox(int x, int y, int w, int h); int gv_GetTableRecord();
int gm_EquipmentValue(); void d2_RadioSelect(int g, int sel);
int ai_PassesPartnerFilter(unsigned short me, unsigned short id, AMOUNTCFG *cfg, int unused);

/* dcp unit split: the definitions of lc the functions of lcamt use */
void ai_LayoutProdGoodsList(int view, int part, int pointer, int bld);
void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld);
void lc_PlayerInJail(void);
int ai_SearchResourceWindow(AIBLD *bld);
int ai_ExtractResourceWindow(AIBLD *bld);
void ai_Top5Window(void);
G8SLOT *ai_FindSettingSlots(int bldId);
void *ai_ProdSetting(int bld);
void ai_SetSettingWeight(int bld, int good, unsigned char weight);
void ai_RefreshProdSetting(G8BLD *b);
void ai_SetSettingFactor(int bld, float factor);
void ai_ShowTributeClaimNews(int collectorRec, int payerRec);
void ai_TalentWindow(struct AIBLD *bld, int room, int talent);
void ai_EvidenceBookWindow(AIBLD *bld);
int ai_BribeAmountCB(AICHARREC *ch);
void ai_DenounceWindow(AIBLD *bld);
void ai_PamphletWindow(int bld);
void ai_NoticeBoardWindow(void);
void ai_BardWindow(void);
void ai_FeastWindow(AIBLD *bld, AIOBJ *table);
void ai_TechEffectsWindow(AIBLD *bld);
void ob_HandleBepflanzung(AIBLD *bld);
void ai_HandlePickedPlant(AIBLD *bld, int plants);
int ai_FindUsableGoodIndex(int id, int *index);
void ai_ChooseCoatOfArms(void);

/* 0x5bd060, the upper limit of an amount box (88 B): with from (the lower end) and steps (of 10000) positive,
   *out = the larger of from and min(money, steps * 10000 + from); returns 1, else 0. The original computes the
   min twice (once for the comparison and once for the value) */

int ai_AmountLimit(int from, int money, int steps, int *out)
{
    int limit;

    if (from < 1 || steps < 1)
        return 0;
    limit = steps * 10000 + from;
    if ((money < limit ? money : limit) > from)
        limit = money < limit ? money : limit;
    else
        limit = from;
    *out = limit;
    return 1;
}
/* 0x5bd0c0, a centred text control of width w in the current window (128 B): sets the font, and when the window
   has flags draws the text box (d2_DrawTextBox, centred on x, flags 8) and paints the measured bounds, then
   creates the centred text control (d2_CreateWinText) at x - w / 2 with its width and font */

void ai_CentredText(char *text, int x, int y, int w, int font)
{
    int ctrl;

    d2_SetFont(font);
    if (d2_Wins[d2_LastWindow].surf2 != 0) {
        d2_DrawTextBox(x - w / 2, y, w, 0x140, 0, text, 8);
        d2_FillPaintbox(d2_TextLeft, d2_TextTop, d2_TextDrawnW, d2_TextDrawnH);
    }
    ctrl = d2_CreateWinText(x - w / 2, y, text, d2_LastWindow);
    d2_Objs[ctrl].centred = 1;
    d2_Objs[ctrl].w = w;
    d2_SetObjFont(ctrl, font);
}
/* 0x5bd190, a right-aligned text control of width w in the current window (96 B): creates it at x - w
   (d2_CreateWinText), sets its right alignment and its width, then its font */

void ai_RightAlignedText(char *text, int x, int y, int w, int font)
{
    int ctrl;

    ctrl = d2_CreateWinText(x - w, y, text, d2_LastWindow);
    d2_Objs[ctrl].rightAligned = 1;
    d2_Objs[ctrl].w = w;
    d2_SetObjFont(ctrl, font);
}
/* 0x5bd1f0, a left-aligned text control of width w at x, y in the current window (96 B): creates it
   (d2_CreateWinText), clears its right alignment, sets its width, then its font */

void ai_LeftAlignedText(char *text, int x, int y, int w, int font)
{
    int ctrl;

    ctrl = d2_CreateWinText(x, y, text, d2_LastWindow);
    d2_Objs[ctrl].rightAligned = 0;
    d2_Objs[ctrl].w = w;
    d2_SetObjFont(ctrl, font);
}
#pragma pack(push, 1)

#pragma pack(pop)

/* 0x5bd250, the pictures and text block of a window row (192 B): with the window set, the gv record (0x4f2720
   fills AIPOSVIEW: the picture count at +0xc, the text count at +0x18) fills the picture page of the row with
   0x4731d0 (0x526); with an entry at most 4 the text page gets "$C$Z%s" (0x62a578, entry + 0x16b7) and
   0x52eb50 (0, 6, the player's character, the entry, 0x4df) */

void ai_ShowWindowRow(AIWINDOWROW *row)
{
    int i;
    AIPOSVIEW pos;

    if (row->win != -1) {
        if (gv_GetTableRecord(row->record, (int *)&pos) != 0 &&
            pos.texts != 0 && pos.pics != 0) {
            d2_SetForm(row->win, row->pageA);
            for (i = 0; i < pos.pics; i++)
                d2_WinOpenPic(i * 0x10 + 5, 5, 0x526, d2_LastWindow);
        }
        if (row->entry <= 4) {
            d2_SetForm(row->win, row->pageB);
            io_PrintText("$C$Z%s", row->entry + 0x16b7);
            d2_SetForm(row->win, row->pageC);
            sv_DrawBalanceBar(0, 6, (char *)((unsigned)&ai_Chars + ai_PlayerId * 0x21c), row->entry, 0x4df);
        }
    }
}
/* 0x5bd330, the loan rows of the money lender's info window (316 B): the loans of the character ch (gm_CreditEvents),
   at most max (capped at 9), each a row of rows in the form form of win: the frame picture 0x75a centred in the
   window, a child window (font 0x56, left margin 0x10) over it with the text 0x1f41 (the loan's bank, amount,
   interest and due date) when the bank is found; returns the number of rows */

int ai_ShowLoanRows(AICHARREC *ch, int max, CREDITLISTROW *rows, int win, int form)
{
    HELOAN *loans[9];
    int n;
    int w;
    int i;
    int bank;

    if (max >= 9)
        max = 9;
    n = gm_CreditEvents(ch->id, 0, loans);
    w = d2_Pics[0x75a].w;
    if (n >= max)
        n = max;
    for (i = 0; i < n; i++) {
        d2_SetForm(win, form);
        rows[i].pic = d2_WinOpenPic((d2_LastWindowPtr->w - w) >> 1, i * 0x80, 0x75a, d2_LastWindow);
        rows[i].child = d2_CreateChildWindow((d2_LastWindowPtr->w - w) >> 1, i * 0x80 + 4, w, 0x72, 0x10,
                                             d2_LastWindow);
        rows[i].loan = loans[i];
        d2_Wins[rows[i].child].font = 0x56;
        d2_Wins[rows[i].child].ml = 0x10;
        bank = gm_FindBuildingById(((AIJOB *)rows[i].loan)->bldId);
        if (bank != 0)
            io_PrintText(0x1f41, bank, rows[i].loan->amount, rows[i].loan->interest, &rows[i].loan->due);
    }
    return n;
}
/* 0x5bd470, the building rows of the character ch (362 B), the stats panel's building page and the money lender's
   first page: with a ctrls array (max entries, set to -1 first) per building of ch (gm_BuildQuery(1, 4, id)) that
   may be entered from the panel (AIBLD.flags5a bit 0 clear) a row in the current window: the type picture
   0x427 + kind, the name ("%G") and the equipment value at its level ("%T", gm_EquipmentValue); while there is
   room in ctrls the picture becomes clickable (style 3, the building as its user record) and is kept there;
   returns the number of buildings */

int ai_ShowBuildingRows(AICHARREC *ch, int max, int *ctrls)
{
    char text[0x100];
    int filled;
    int n;
    int value;
    int pic;
    int y;
    AIBLD *bld;

    filled = 0;
    if (max > 0 && ctrls != 0) {
        filled = 1;
        memset(ctrls, -1, max * 4);
    }
    n = 0;
    for (bld = (AIBLD *)gm_BuildQuery(1, 4, ch->id); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
        if ((bld->flags5a & 1) == 0) {
            value = gm_EquipmentValue(bld, bld->level);
            y = n << 6;
            pic = d2_WinOpenPic(0x20, y, bld->type + 0x427, d2_LastWindow);
            txt_Format(text, "%G", bld);
            d2_CreateWinText(0x80, y, text, d2_LastWindow);
            txt_Format(text, "%T", value);
            d2_CreateWinText(0x80, y + 0x1b, text, d2_LastWindow);
            if (filled == 1 && n < max) {
                d2_Objs[pic].interactive = 1;
                d2_Objs[pic].style = 3;
                d2_Objs[pic].user = bld;
                ctrls[n] = pic;
            }
            n++;
        }
    }
    return n;
}
/* 0x5bd5e0, the rows of the special-amount list (1374 B, ai_SpecialAmountListWindow for the kinds 0 / 7): lays out
   the n rows in columns by n (1 .. 3: one column of n, 4 .. 6: two, 7 / 8: three; a column of three spaced 0xca
   apart, of two centred in pairs, of one at 0xca; the columns 0x80 apart), then per row the frame picture 0x5da
   (clickable with the character as its user record when the filter passes it, ai_PassesPartnerFilter), and for a
   character: the family picture (AICHARREC.ref54) of the kinds 5 .. 7, the portrait, the four icons in the
   corners, the title and name ("%s %1N7") and below it the office of kind 7 ("%s", the deputy office text) or the
   name ("%1N5"); a character other than viewer and not of kind 6 gets the liking slider (gm_Liking of the viewer,
   0 for kind 7); a viewer of kind 6 marks its partner with the picture 0x6d3; returns 0 */

int ai_ShowAmountRows(int win, int kind, unsigned int *filter, int n, LCAMOUNTROW *rows, AICHARREC *viewer)
{
    int cols[3];
    int col;
    int colY;
    char text[0x100];
    int ncols;
    int step;
    int i;
    int j;
    int off;
    int x;
    int y;
    int left;
    LCAMOUNTROW *row;

    cols[0] = -1;
    cols[1] = -1;
    cols[2] = -1;
    ncols = 0;
    step = 0;
    switch (n) {
    case 1:
        ncols = 1;
        step = 0x80;
        cols[0] = 1;
        break;
    case 2:
        ncols = 1;
        step = 0x80;
        cols[0] = 2;
        break;
    case 3:
        ncols = 1;
        step = 0x80;
        cols[0] = 3;
        break;
    case 4:
        ncols = 2;
        step = 0x40;
        cols[0] = 2;
        cols[1] = 2;
        break;
    case 5:
        ncols = 2;
        step = 0x40;
        cols[0] = 3;
        cols[1] = 2;
        break;
    case 6:
        ncols = 2;
        step = 0x40;
        cols[0] = 3;
        cols[1] = 3;
        break;
    case 7:
        ncols = 3;
        cols[2] = 2;
        cols[1] = 2;
        cols[0] = 3;
        break;
    case 8:
        ncols = 3;
        cols[2] = 3;
        cols[1] = 2;
        cols[0] = 3;
        break;
    }
    i = 0;
    for (col = 0; col < ncols; col++) {
        for (j = 0; j < cols[col] && i < n; j++, i++) {
            rows[i].y = col * 0x80 + step;
            if (cols[col] == 3)
                rows[i].x = j * 0xca;
            else if (cols[col] == 2)
                rows[i].x = j * 0xca + 0x65;
            else
                rows[i].x = 0xca;
        }
    }
    for (i = 0; i < n; i++) {
        row = &rows[i];
        x = row->x + 0x65;
        y = row->y + 0x23;
        left = x - d2_Pics[0x5da].w / 2;
        row->ctl[0] = d2_WinOpenPic(left, y, 0x5da, d2_LastWindow);
        if (row->rec == 0)
            continue;
        if (row->rec->kind == 6 || row->rec->kind == 7 || row->rec->kind == 5)
            d2_WinOpenPic(left - 0x20, y, row->rec->ref54, d2_LastWindow);
        row->portrait = d2_WinOpenPic(x - d2_Pics[row->rec->picture].w / 2 - 3, y + 1, row->rec->picture,
                                         d2_LastWindow);
        d2_WinOpenPic(left + 3, y + 3, row->icons[0], d2_LastWindow);
        if (row->icons[1] != 0)
            d2_WinOpenPic(left + 3, d2_Pics[0x5da].h + y - 0x13, row->icons[1], d2_LastWindow);
        if (row->icons[2] != 0)
            d2_WinOpenPic(d2_Pics[0x5da].w + left - 0x10, y + 3, row->icons[2], d2_LastWindow);
        d2_WinOpenPic(d2_Pics[0x5da].w + left - 0x10, d2_Pics[0x5da].h + y - 0x13, row->icons[3], d2_LastWindow);
        if (kind == 7) {
            txt_Format(text, "%s %1N7", (row->rec->sex ? 0x11e : 0x114) + row->rec->b00d, row->rec->id);
            ai_CentredText(text, x, row->y, 0xca, 0x56);
            txt_Format(text, "%s", (row->rec->sex ? 0x27b : 0x253) + row->rec->secondOffice);
            ai_CentredText(text, x, row->y + 0xf, 0xca, 0x56);
        } else {
            txt_Format(text, "%s %1N7", (row->rec->sex ? 0x11e : 0x114) + row->rec->b00d, row->rec->id);
            ai_CentredText(text, x, row->y, 0xca, 0x56);
            txt_Format(text, "%1N5", row->rec->id);
            ai_CentredText(text, x, row->y + 0xf, 0xca, 0x56);
        }
        if (row->rec != viewer && row->rec->kind != 6) {
            row->ctl[1] = d2_WinOpenSlider(x - 0x23, d2_Pics[0x5da].h + y + 5, 0x40, 0, 100, 0x75, 0x442,
                                              d2_LastWindow);
            if (row->rec->kind == 7)
                d2_SetObjValue(row->ctl[1], 0, 100, 0, 1);
            else
                d2_SetObjValue4(row->ctl[1], 0, 100, (int)gm_Liking(viewer->id, row->rec->id, 1));
            d2_SetObjEnabled(row->ctl[1], 1);
            d2_Objs[row->ctl[1]].interactive = 1;
        }
        if (viewer != 0 && viewer->kind == 6 && row->rec->partner == viewer->netId)
            d2_WinOpenPic(d2_Pics[0x5da].w + left, y, 0x6d3, d2_LastWindow);
        if (filter == 0 || ai_PassesPartnerFilter(ai_PlayerId, row->rec->id, filter, 0) != 0) {
            d2_Objs[row->ctl[0]].interactive = 1;
            d2_Objs[row->ctl[0]].style = 3;
            d2_Objs[row->ctl[0]].user = row->rec;
        }
    }
    return 0;
}
/* 0x5bdb60, the seat rows of the special-amount list (1633 B, ai_SpecialAmountListWindow for the kinds 1 .. 6 and 8;
   the seats as offices is a reasoned guess): the kind places the rows on fixed seats (1 .. 3: four, 4 .. 6: six,
   8: five; another kind shows nothing), then per row the frame picture 0x5da and for a character the pictures and
   the liking slider as ai_ShowAmountRows, the texts the title and name ("%s %1N7") and the name ("%1N5"), or for
   kind 8 the name ("%1N7") and the office of AICHARREC.flags1cc (0x1000: 0x26, 0x2000: 0x27, 0x800: 0x25,
   0x400: 0x24, 0x200: 0x23); a vacant row shows the picture 0x4fb and the text 0x253 + its office, its frame
   disabled; returns 0 */

int ai_ShowOfficeRows(int win, int kind, unsigned int *filter, int n, LCAMOUNTROW *rows, AICHARREC *viewer)
{
    int valid;
    int left;
    int i;
    char text[0x100];
    int x;
    int y;
    int office;
    unsigned int flags;

    if (kind == 1 || kind == 2 || kind == 3) {
        rows[0].x = 0xca;
        rows[1].x = 0xca;
        rows[2].x = 0x65;
        rows[3].x = 0x12f;
        rows[0].y = 0;
        rows[1].y = 0x80;
        rows[2].y = 0x100;
        rows[3].y = 0x100;
        valid = 1;
    } else if (kind == 4 || kind == 5 || kind == 6) {
        rows[0].x = 0xca;
        rows[1].x = 0x65;
        rows[2].x = 0x12f;
        rows[3].x = 0;
        rows[4].x = 0xca;
        rows[5].x = 0x194;
        rows[0].y = 0;
        rows[1].y = 0x80;
        rows[2].y = 0x80;
        rows[3].y = 0x100;
        rows[4].y = 0x100;
        rows[5].y = 0x100;
        valid = 1;
    } else if (kind == 8) {
        rows[0].x = 0x12f;
        rows[1].x = 0x65;
        rows[2].x = 0xca;
        rows[3].x = 0x12f;
        rows[4].x = 0x65;
        rows[0].y = 0;
        rows[1].y = 0;
        rows[2].y = 0x80;
        rows[3].y = 0x100;
        rows[4].y = 0x100;
        valid = 1;
    } else {
        valid = 0;
    }
    for (i = 0; i < n && valid; i++) {
        x = rows[i].x + 0x65;
        y = rows[i].y + 0x23;
        left = x - d2_Pics[0x5da].w / 2;
        rows[i].ctl[0] = d2_WinOpenPic(left, y, 0x5da, d2_LastWindow);
        rows[i].ctl[1] = -1;
        if (rows[i].rec != 0) {
            if (rows[i].rec->kind == 6 || rows[i].rec->kind == 7 || rows[i].rec->kind == 5)
                d2_WinOpenPic(left - 0x20, y, rows[i].rec->ref54, d2_LastWindow);
            rows[i].portrait = d2_WinOpenPic(x - d2_Pics[rows[i].rec->picture].w / 2 - 3, y + 1, rows[i].rec->picture,
                                             d2_LastWindow);
            d2_WinOpenPic(left + 3, y + 3, rows[i].icons[0], d2_LastWindow);
            if (rows[i].icons[1] != 0)
                d2_WinOpenPic(left + 3, d2_Pics[0x5da].h + y - 0x13, rows[i].icons[1], d2_LastWindow);
            if (rows[i].icons[2] != 0)
                d2_WinOpenPic(d2_Pics[0x5da].w + left - 0x10, y + 3, rows[i].icons[2], d2_LastWindow);
            d2_WinOpenPic(d2_Pics[0x5da].w + left - 0x10, d2_Pics[0x5da].h + y - 0x13, rows[i].icons[3], d2_LastWindow);
            if (kind != 8) {
                txt_Format(text, "%s %1N7", (rows[i].rec->sex ? 0x11e : 0x114) + rows[i].rec->b00d, rows[i].rec->id);
                ai_CentredText(text, x, rows[i].y, 0xca, 0x56);
                txt_Format(text, "%1N5", rows[i].rec->id);
                ai_CentredText(text, x, rows[i].y + 0xf, 0xca, 0x56);
            } else {
                flags = rows[i].rec->flags1cc;
                if (flags & 0x1000)
                    office = 0x26;
                else if (flags & 0x2000)
                    office = 0x27;
                else if (flags & 0x800)
                    office = 0x25;
                else if (flags & 0x400)
                    office = 0x24;
                else
                    office = (flags & 0x200) ? 0x23 : 0;
                txt_Format(text, "%1N7", rows[i].rec->id);
                ai_CentredText(text, x, rows[i].y, 0xca, 0x56);
                txt_Format(text, "%s", (rows[i].rec->sex ? 0x27b : 0x253) + office);
                ai_CentredText(text, x, rows[i].y + 0xf, 0xca, 0x56);
            }
            if (rows[i].rec != viewer && rows[i].rec->kind != 6) {
                rows[i].ctl[1] = d2_WinOpenSlider(x - 0x23, d2_Pics[0x5da].h + y + 5, 0x40, 0, 100, 0x75, 0x442,
                                                  d2_LastWindow);
                if (rows[i].rec->kind == 7)
                    d2_SetObjValue(rows[i].ctl[1], 0, 100, 0, 1);
                else
                    d2_SetObjValue4(rows[i].ctl[1], 0, 100, (int)gm_Liking(viewer->id, rows[i].rec->id, 1));
                d2_SetObjEnabled(rows[i].ctl[1], 1);
                d2_Objs[rows[i].ctl[1]].interactive = 1;
            }
            if (viewer != 0 && viewer->kind == 6 && rows[i].rec->partner == viewer->netId)
                d2_WinOpenPic(d2_Pics[0x5da].w + left, y, 0x6d3, d2_LastWindow);
            if (filter == 0 || ai_PassesPartnerFilter(ai_PlayerId, rows[i].rec->id, filter, 0) != 0) {
                d2_Objs[rows[i].ctl[0]].interactive = 1;
                d2_Objs[rows[i].ctl[0]].style = 3;
                d2_Objs[rows[i].ctl[0]].user = rows[i].rec;
            }
        } else {
            d2_WinOpenPic(x - d2_Pics[0x4fb].w / 2 - 3, y + 1, 0x4fb, d2_LastWindow);
            txt_Format(text, "%s", rows[i].office + 0x253);
            ai_CentredText(text, x, rows[i].y, 0xca, 0x56);
            d2_Objs[rows[i].ctl[0]].interactive = 1;
            d2_Objs[rows[i].ctl[0]].user = 0;
            d2_SetObjEnabled(rows[i].ctl[0], 0);
        }
    }
    return 0;
}

float gm_CharacterHealth(); int amt_HasColumn7Holder(); int d2_WinCreateSlider();
void d2_SetObjWidth(int obj, short w); int amt_GetColumnOffices(int col, int max, struct AMTREC *out);

#define LC_LOVER_FLAG 0x800000  /* AICHAR.flags1c8 bit 23: has a keeper (he.c HE_LOVER_FLAG, cm.c CM_KEEPER_FLAG) */

/* 0x5be1d0, the four corner icons of a special-amount row (477 B): the faith (0x6c9 / 0x6ca), the lover (0x6cc: a
   kept character (LC_LOVER_FLAG) of the other sex whose pair event 0x6f the player keeps), the
   crimes (0x6cd: gv_HeldCrimeCount against the player, else 0x6ce: a crime event 0x18 the player was told of about
   the character), and the pair event 0x45 with the player (0x6cb) or else the health (gm_CharacterHealth below
   100 / 350 / 650: 0x6d2 / 0x6d1 / 0x6d0, else 0x6cf); returns 1, 0 for a row without a character */
int ai_AmountRowStatus(LCAMOUNTROW *row)
{
    AIJOB *ev;
    float health;

    if (row->rec == 0)
        return 0;
    row->icons[0] = row->rec->faith ? 0x6c9 : 0x6ca;
    row->icons[1] = 0;
    if (lc_CharRecs[ai_PlayerId].sex != row->rec->sex && (row->rec->powerFlags & LC_LOVER_FLAG) != 0) {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x6f); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && row->rec->netId == ev->partner2) {
                row->icons[1] = 0x6cc;
                break;
            }
        }
    }
    row->icons[2] = 0;
    if (gv_HeldCrimeCount(row->rec->netId, ai_PlayerId) > 0) {
        row->icons[2] = 0x6cd;
    } else {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->chr == ai_PlayerId && row->rec->netId == ev->id4) {
                row->icons[2] = 0x6ce;
                break;
            }
        }
    }
    row->icons[3] = 0;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x45); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && row->rec->netId == ev->partner2) {
            row->icons[3] = 0x6cb;
            break;
        }
    }
    if (row->icons[3] == 0) {
        health = gm_CharacterHealth(row->rec);
        if (health < 100.0f)
            row->icons[3] = 0x6d2;
        else if (health < 350.0f)
            row->icons[3] = 0x6d1;
        else if (health < 650.0f)
            row->icons[3] = 0x6d0;
        else
            row->icons[3] = 0x6cf;
    }
    return 1;
}
/* 0x5be3b0, one row of a special-amount list at (x, y) in the current window (872 B): the frame picture 0x5da; for
   a character the portrait, the corner icons of ai_AmountRowStatus, the title and name ("%s %1N7", only with titled
   and a title) or the name ("%1N7") and below it "%1N5" in the font font, the liking slider of the player
   (disabled; 0 for kind 7, none for kind 6), the frame clickable with the character as its user record when the
   row is pickable, and the family picture (AICHARREC.ref54) of the kinds 5 .. 7; a vacant row shows the picture
   0x4fb; returns 0 */

int ai_AmountRow(int x, int y, int win, int font, LCAMOUNTROW *row, int titled)
{
    char text[0x100];
    int frameY;
    int cx;

    cx = d2_Pics[0x5da].w / 2 + x;
    frameY = y + 0x1c;
    row->ctl[0] = d2_WinOpenPic(x, frameY, 0x5da, d2_LastWindow);
    row->ctl[1] = -1;
    if (row->rec != 0) {
        row->portrait = d2_WinOpenPic(cx - d2_Pics[row->rec->picture].w / 2 - 3, y + 0x1d, row->rec->picture,
                                      d2_LastWindow);
        d2_WinOpenPic(x + 3, y + 0x1f, row->icons[0], d2_LastWindow);
        if (row->icons[1] != 0)
            d2_WinOpenPic(x + 3, d2_Pics[0x5da].h + y + 9, row->icons[1], d2_LastWindow);
        if (row->icons[2] != 0)
            d2_WinOpenPic(d2_Pics[0x5da].w + x - 0x10, y + 0x1f, row->icons[2], d2_LastWindow);
        d2_WinOpenPic(d2_Pics[0x5da].w + x - 0x10, d2_Pics[0x5da].h + y + 9, row->icons[3], d2_LastWindow);
        if (row->rec->b00d != 0 && titled != 0)
            txt_Format(text, "%s %1N7", (row->rec->sex ? 0x11e : 0x114) + row->rec->b00d, row->rec->id);
        else
            txt_Format(text, "%1N7", row->rec->id);
        ai_CentredText(text, cx, row->y, 0xca, font);
        txt_Format(text, "%1N5", row->rec->id);
        ai_CentredText(text, cx, row->y + 0xe, 0xca, font);
        if (row->rec->kind != 6) {
            row->ctl[1] = d2_WinOpenSlider(cx - 0x23, d2_Pics[0x5da].h + y + 0x1e, 0x40, 0, 100, 0x75, 0x442,
                                           d2_LastWindow);
            if (row->rec->kind == 7)
                d2_SetObjValue(row->ctl[1], 0, 100, 0, 1);
            else
                d2_SetObjValue4(row->ctl[1], 0, 100, (int)gm_Liking(ai_PlayerId, row->rec->id, 1));
            d2_SetObjEnabled(row->ctl[1], 0);
        }
        if (row->pickable != 0) {
            d2_Objs[row->ctl[0]].interactive = 1;
            d2_Objs[row->ctl[0]].style = 3;
            d2_Objs[row->ctl[0]].user = row->rec;
        }
        if (row->rec->kind == 5 || row->rec->kind == 6 || row->rec->kind == 7)
            d2_WinOpenPic(x - 0x20, frameY, row->rec->ref54, d2_LastWindow);
    } else {
        d2_WinOpenPic(cx - d2_Pics[0x4fb].w / 2 - 3, y + 0x1d, 0x4fb, d2_LastWindow);
    }
    return 0;
}
/* 0x5be720, one compact special-amount row at (x, y) in the current window (734 B, ai_AmountRow without the texts):
   the frame picture 0x5da; for a character the portrait, the corner icons (ai_AmountRowStatus), the liking slider of
   the player (disabled; 0 for kind 7, none for kind 6), the mark 0x6d3 right of the frame when the character is the
   player's partner, the frame clickable with the character as its user record when the row is pickable, and the
   family picture (AICHARREC.ref54) of the kinds 5 .. 7 left of it; a vacant row shows the picture 0x4fb; win is
   not read (the pictures go to d2_LastWindow); returns 0 */

int ai_DrawAmountRow(int x, int y, int win, LCAMOUNTROW *row)
{
    int cx;

    cx = d2_Pics[0x5da].w / 2 + x;
    row->ctl[0] = d2_WinOpenPic(x, y, 0x5da, d2_LastWindow);
    row->ctl[1] = -1;
    if (row->rec != 0) {
        row->portrait = d2_WinOpenPic(cx - d2_Pics[row->rec->picture].w / 2 - 3, y + 1, row->rec->picture,
                                      d2_LastWindow);
        d2_WinOpenPic(x + 3, y + 3, row->icons[0], d2_LastWindow);
        if (row->icons[1] != 0)
            d2_WinOpenPic(x + 3, d2_Pics[0x5da].h + y - 0x13, row->icons[1], d2_LastWindow);
        if (row->icons[2] != 0)
            d2_WinOpenPic(d2_Pics[0x5da].w + x - 0x10, y + 3, row->icons[2], d2_LastWindow);
        d2_WinOpenPic(d2_Pics[0x5da].w + x - 0x10, d2_Pics[0x5da].h + y - 0x13, row->icons[3], d2_LastWindow);
        if (row->rec->kind != 6) {
            row->ctl[1] = d2_WinOpenSlider(cx - 0x23, d2_Pics[0x5da].h + y + 2, 0x40, 0, 100, 0x75, 0x442,
                                           d2_LastWindow);
            if (row->rec->kind == 7)
                d2_SetObjValue(row->ctl[1], 0, 100, 0, 1);
            else
                d2_SetObjValue4(row->ctl[1], 0, 100, (int)gm_Liking(ai_PlayerId, row->rec->id, 1));
            d2_SetObjEnabled(row->ctl[1], 0);
        }
        if (row->rec->partner == lc_CharRecs[ai_PlayerId].netId)
            d2_WinOpenPic(d2_Pics[0x5da].w + x, y, 0x6d3, d2_LastWindow);
        if (row->pickable != 0) {
            d2_Objs[row->ctl[0]].interactive = 1;
            d2_Objs[row->ctl[0]].style = 3;
            d2_Objs[row->ctl[0]].user = row->rec;
        }
        if (row->rec->kind == 5 || row->rec->kind == 6 || row->rec->kind == 7)
            d2_WinOpenPic(x - 0x20, y, row->rec->ref54, d2_LastWindow);
    } else {
        d2_WinOpenPic(cx - d2_Pics[0x4fb].w / 2 - 3, y + 1, 0x4fb, d2_LastWindow);
    }
    return 0;
}
/* 0x5bea00, the special-amount list window over a caller record (79 B): the local 0x50-byte config is
   zeroed and gets +4 = 0x204, +8 = 0x5eed68, +0xc = 0x5eed6c and +0x10 = 6, then 0x5c0970 (the config, the
   second argument, 0, 0) runs the tabbed list; its result is returned */

int ai_SpecialAmountList(G8CHARREC *ch, char *text)
{
    AMOUNTCFG cfg;

    (void)ch;
    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x204;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    return ai_SpecialAmountWindow((unsigned int *)&cfg, text, 0, 0);
}
/* 0x5bea50, the special-amount list window "special\amt2" (1655 B): the caption (the kind's name 0x12b2 + kind, or
   the text 0xa0 with the viewing character), the subcaption caption or caption2, and up to 8 rows: the kind 0 the
   first characters of the kinds 6 / 7 / 5, the kinds 1 .. 6 the six offices of the column kind, 7 the six of
   amt_GetColumn7Offices, 8 the five of the column 8 (another kind or no row: the window closes at once and returns
   0). Each row gets its corner icons (ai_AmountRowStatus), then ai_ShowAmountRows (kinds 0 / 7) or ai_ShowOfficeRows
   draws them from the viewpoint of the viewing character or the player. Holding the right button on a row's slider
   views the list as that character; a click on a row's frame chooses its character: without pick the window closes
   with it, else pick (character, 0) decides (the window disabled meanwhile); a vacant seat ends the viewing. Returns
   the chosen character record (0: none) */

int ai_SpecialAmountListWindow(int kind, unsigned int *filter, char *caption, char *caption2,
                               int (*pick)(AICHARREC *, int))
{
    int viewing;
    LCAMOUNTROW rows[8];
    AMTREC seats8[5];
    AMTREC seats[6];
    AMTREC seats7[6];
    AICHARREC *chosen;
    AICHARREC *viewer;
    AICHARREC *c;
    AIJOB *ev;
    float health;
    int redraw;
    int win;
    int n;
    int i;
    int hit;

    n = 0;
    chosen = 0;
    viewer = 0;
    win = d2_OpenForm(0, 0, "special\\amt2");
    d2_CentreViewWindows(win);
    viewing = 0;
    redraw = 1;
    do {
        if (redraw) {
            d2_SetForm(win, 2);
            io_PrintText("$C");
            if (viewing != 0)
                io_PrintText(0xa0, viewer->id);
            else
                io_PrintText("$Z$[%s$]", kind + 0x12b2);
            d2_SetForm(win, 3);
            io_PrintText("$C");
            if (caption != 0)
                io_PrintText(caption);
            else if (caption2 != 0)
                io_PrintText(caption2);
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            for (i = 0; i < 8; i++) {
                rows[i].rec = 0;
                rows[i].ctl[0] = -1;
                rows[i].office = 0;
            }
            if (kind == 0) {
                n = 0;
                for (i = 0; i < 0x300 && n < 8; i++) {
                    c = &lc_CharRecs[i];
                    if (c->kind == 6 || c->kind == 7 || c->kind == 5)
                        rows[n++].rec = c;
                }
            } else if (kind > 0 && kind < 7) {
                n = amt_GetColumnOffices(kind, 6, seats);
                for (i = 0; i < n; i++) {
                    rows[i].rec = (AICHARREC *)gm_FindChar(seats[i].owner);
                    rows[i].office = seats[i].kind;
                }
            } else if (kind == 7) {
                n = amt_GetColumn7Offices(0, 6, seats7);
                for (i = 0; i < n; i++) {
                    rows[i].rec = (AICHARREC *)gm_FindChar(seats7[i].owner);
                    rows[i].office = seats7[i].kind;
                }
            } else if (kind == 8) {
                n = amt_GetColumnOffices(kind, 5, seats8);
                for (i = 0; i < n; i++) {
                    rows[i].rec = (AICHARREC *)gm_FindChar(seats8[i].owner);
                    rows[i].office = seats8[i].kind;
                }
            } else {
                n = 0;
            }
            if (n == 0) {
                sv_PopRequest = 1;
                sv_RunFrame(ai_SpecialAmountListWindow, 0x657c7);
                d2_CloseView(win);
                return 0;
            }
            for (i = 0; i < n; i++) {
                /* the corner icons, as ai_AmountRowStatus (written out here: the original has the code in line) */
                if (rows[i].rec == 0)
                    continue;
                rows[i].icons[0] = rows[i].rec->faith ? 0x6c9 : 0x6ca;
                rows[i].icons[1] = 0;
                if (lc_CharRecs[ai_PlayerId].sex != rows[i].rec->sex && (rows[i].rec->powerFlags & LC_LOVER_FLAG) != 0) {
                    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x6f); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && rows[i].rec->netId == ev->partner2) {
                            rows[i].icons[1] = 0x6cc;
                            break;
                        }
                    }
                }
                rows[i].icons[2] = 0;
                if (gv_HeldCrimeCount(rows[i].rec->netId, ai_PlayerId) > 0) {
                    rows[i].icons[2] = 0x6cd;
                } else {
                    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->chr == ai_PlayerId && rows[i].rec->netId == ev->id4) {
                            rows[i].icons[2] = 0x6ce;
                            break;
                        }
                    }
                }
                rows[i].icons[3] = 0;
                for (ev = (AIJOB *)he_FindEvent(1, 0, 0x45); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                    if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && rows[i].rec->netId == ev->partner2) {
                        rows[i].icons[3] = 0x6cb;
                        break;
                    }
                }
                if (rows[i].icons[3] == 0) {
                    health = gm_CharacterHealth(rows[i].rec);
                    if (health < 100.0f)
                        rows[i].icons[3] = 0x6d2;
                    else if (health < 350.0f)
                        rows[i].icons[3] = 0x6d1;
                    else if (health < 650.0f)
                        rows[i].icons[3] = 0x6d0;
                    else
                        rows[i].icons[3] = 0x6cf;
                }
            }
            if (kind == 0 || kind == 7)
                ai_ShowAmountRows(d2_LastWindow, kind, filter, n, rows, viewer ? viewer : &lc_CharRecs[ai_PlayerId]);
            else
                ai_ShowOfficeRows(d2_LastWindow, kind, filter, n, rows, viewer ? viewer : &lc_CharRecs[ai_PlayerId]);
            redraw = 0;
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (dm_L.rhold != 0 && viewing == 0) {
            chosen = 0;
            viewer = 0;
            hit = -1;
            for (i = 0; i < n; i++) {
                if (rows[i].ctl[1] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit > -1 && hit < n && rows[hit].rec != 0) {
                viewer = 0;
                if (rows[hit].rec->id != 0xffff) {
                    viewing = 1;
                    viewer = rows[hit].rec;
                    redraw = 1;
                } else
                    redraw = 1;
            }
        } else if (dm_L.rhold == 0 && viewing != 0) {
            viewing = 0;
            viewer = 0;
            redraw = 1;
        } else if (dm_L.lclick != 0 && d2_ClickedId != -1) {
            chosen = 0;
            hit = -1;
            for (i = 0; i < n; i++) {
                if (rows[i].ctl[0] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit > -1 && hit < n) {
                c = rows[hit].rec;
                if (c->id == 0xffff) {
                    viewer = 0;
                    viewing = 0;
                    redraw = 1;
                } else if (pick == 0) {
                    sv_PopRequest = 1;
                    chosen = c;
                    redraw = 1;
                } else {
                    d2_SetViewEnabled(win, 0);
                    chosen = pick(c, 0) ? c : 0;
                    d2_SetViewEnabled(win, 1);
                    redraw = 1;
                }
            } else
                redraw = 1;
        }
    } while (sv_RunFrame(ai_SpecialAmountListWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return (int)chosen;
}
/* 0x5bf0d0, the fill of the special-amount window's family tab (tab 0, 551 B): the characters other than me, alive
   (id != 0xffff) and of a kind below 10 that are one of my relatives (the relation class 4 / 5 for relatives[0],
   0 for [1], 1 for [2], 6 / 7 for the others; the odd class: a female character) or share my w050 and b058 (2 / 3),
   then the partner of my running pair event 0x41 (8 / 9, unless it is relatives[0]). With max 0 it only counts
   them; else it clears max rows of rows and fills up to max of them (LCAMOUNTROW.rec, x2c the class), gives each
   its corner icons (ai_AmountRowStatus) and makes it pickable when it passes the partner filter filter
   (ai_PassesPartnerFilter); returns the count */

unsigned int ai_FillFamilyTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    AICHARREC *c;
    AIJOB *ev;
    int found;
    int j;
    int i;
    unsigned int n;
    unsigned int cnt;

    cnt = 0;
    if (max == 0) {
        for (i = 0; i < 0x300; i++) {
            c = &lc_CharRecs[i];
            if (c == me || c->id == 0xffff || c->kind >= 10)
                continue;
            found = 0;
            for (j = 0; j < 8; j++) {
                if (me->relatives[j] == c->netId) {
                    found = 1;
                    break;
                }
            }
            if (found || (c->w050 == me->w050 && c->b058 == me->b058))
                cnt++;
        }
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x41); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == me->netId) {
                cnt++;
                break;
            }
        }
        return cnt;
    }
    memset(rows, 0, max * sizeof(LCAMOUNTROW));
    n = 0;
    for (i = 0; i < 0x300; i++) {
        c = &lc_CharRecs[i];
        if (n >= max)
            break;
        if (c == me || c->id == 0xffff || c->kind >= 10)
            continue;
        found = 0;
        for (j = 0; j < 8; j++) {
            if (me->relatives[j] == c->netId) {
                found = 1;
                break;
            }
        }
        if (found) {
            rows[n].rec = c;
            switch (j) {
            case 0:
                rows[n].x2c = (c->sex != 0) + 4;
                break;
            case 1:
                rows[n].x2c = 0;
                break;
            case 2:
                rows[n].x2c = 1;
                break;
            default:
                rows[n].x2c = (c->sex != 0) + 6;
                break;
            }
            n++;
        } else if (c->w050 == me->w050 && c->b058 == me->b058) {
            rows[n].rec = c;
            rows[n].x2c = (c->sex != 0) + 2;
            n++;
        }
    }
    if (n < max) {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x41); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == me->netId) {
                c = (AICHARREC *)gm_FindChar(ev->partner2);
                if (c != 0 && c->netId != me->relatives[0]) {
                    rows[n].rec = c;
                    rows[n].x2c = (c->sex != 0) + 8;
                    n++;
                }
                break;
            }
        }
    }
    for (i = 0; i < (int)n; i++) {
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
    }
    return n;
}
/* 0x5bf300, the fill of the special-amount window's friends tab (tab 1, 488 B): the characters other than me,
   alive and of the kinds 2 .. 7 whom me likes at least 75 (gm_Liking) and who are not me's partner. With max 0 it
   only counts them; else it clears max rows of rows and keeps the max best liked: a character liked more than the
   last row takes it (LCAMOUNTROW.rec, x2c the liking) and bubbles up the list; the count (at most max) gets the
   corner icons (ai_AmountRowStatus) and the partner filter filter (ai_PassesPartnerFilter, pickable); returns the
   count */

unsigned int ai_FillFriendsTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    unsigned int n;
    AICHARREC *c;
    AICHARREC *t;
    float like;
    unsigned int k;
    int i;

    n = 0;
    if (max == 0) {
        for (i = 0; i < 0x300; i++) {
            c = &lc_CharRecs[i];
            if (c != me && c->id != 0xffff && c->kind >= 2 && c->kind <= 7 &&
                gm_Liking(me->id, c->id, 1) >= 75.0f && c->partner != me->netId)
                n++;
        }
        return n;
    }
    memset(rows, 0, max * sizeof(LCAMOUNTROW));
    for (i = 0; i < 0x300; i++) {
        c = &lc_CharRecs[i];
        if (c == me || c->id == 0xffff || c->kind < 2 || c->kind > 7)
            continue;
        like = gm_Liking(me->id, c->id, 1);
        if (like >= 75.0f && c->partner != me->netId && rows[max - 1].x2c < like) {
            rows[max - 1].rec = c;
            rows[max - 1].x2c = (int)like;
            n++;
            for (k = max - 1; k != 0; k--) {
                if (rows[k - 1].x2c < rows[k].x2c) {
                    t = rows[k - 1].rec;
                    like = rows[k - 1].x2c;
                    rows[k - 1].rec = rows[k].rec;
                    rows[k - 1].x2c = rows[k].x2c;
                    rows[k].rec = t;
                    rows[k].x2c = (int)like;
                }
            }
        }
    }
    if (n >= max)
        n = max;
    for (i = 0; i < (int)n; i++) {
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
    }
    return n;
}
/* 0x5bf4f0, the fill of the special-amount window's enemies tab (tab 2, 501 B): the characters other than me,
   alive and of the kinds 2 .. 7 whom me likes at most 33 (gm_Liking), and me's partner (taken as liking 0). With
   max 0 it only counts them; else it sets max rows of rows to the liking 100 and keeps the max least liked: a
   character liked less than the last row takes it (LCAMOUNTROW.rec, x2c the liking) and bubbles up the list; the
   count (at most max) gets the corner icons (ai_AmountRowStatus) and the partner filter filter
   (ai_PassesPartnerFilter, pickable); returns the count */

unsigned int ai_FillEnemiesTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    unsigned int n;
    AICHARREC *c;
    AICHARREC *t;
    float like;
    unsigned int k;
    int i;

    n = 0;
    if (max == 0) {
        for (i = 0; i < 0x300; i++) {
            c = &lc_CharRecs[i];
            if (c != me && c->id != 0xffff && c->kind >= 2 && c->kind <= 7 &&
                (gm_Liking(me->id, c->id, 1) <= 33.0f || c->partner == me->netId))
                n++;
        }
        return n;
    }
    memset(rows, 0, max * sizeof(LCAMOUNTROW));
    for (k = 0; k < max; k++)
        rows[k].x2c = 100;
    for (i = 0; i < 0x300; i++) {
        c = &lc_CharRecs[i];
        if (c == me || c->id == 0xffff || c->kind < 2 || c->kind > 7)
            continue;
        if (c->partner == me->netId)
            like = 0.0f;
        else
            like = gm_Liking(me->id, c->id, 1);
        if (like <= 33.0f && like < rows[max - 1].x2c) {
            rows[max - 1].rec = c;
            rows[max - 1].x2c = (int)like;
            n++;
            for (k = max - 1; k != 0; k--) {
                if (rows[k - 1].x2c > rows[k].x2c) {
                    t = rows[k - 1].rec;
                    like = rows[k - 1].x2c;
                    rows[k - 1].rec = rows[k].rec;
                    rows[k - 1].x2c = rows[k].x2c;
                    rows[k].rec = t;
                    rows[k].x2c = (int)like;
                }
            }
        }
    }
    if (n >= max)
        n = max;
    for (i = 0; i < (int)n; i++) {
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
    }
    return n;
}
/* 0x5bf6f0, the fill of the special-amount window's lovers tab (tab 3, 195 B): the partners (gm_FindChar) of up to
   four pair events 0x6f of me (AIJOB.partner1 me's netId); with max set it clears max rows of rows and fills the
   first min(max, count) of them (LCAMOUNTROW.rec) with the corner icons (ai_AmountRowStatus) and the partner
   filter filter (ai_PassesPartnerFilter, pickable); returns the count
   @deep deep s34: c2trace --prio: n#1 9 against max#1 14 (max +16 in the memset block, P 4); n is
   live-through in the gm_FindChar block (-6) and referenced only in the store block (+16, P 2). The scan
   written as lovers[n] = gm_FindChar(..); if (lovers[n] != 0) n++; puts an n reference in the call block
   (+16 there): n 19 > max 14, n = ebx / max = ebp, and with the min as max < n ? max : n (the original's
   cmp ebp,ebx / mov eax,ebp / jb / mov eax,ebx) the variant is 2 ins (order 6): the only hunk left is the
   store, which the original makes after the test (test eax,eax / je / mov [esp+ebx*4+0xc],eax / inc ebx) -
   so that form is provably not the original's, not kept. = control: initgen (n i c ev), an early return
   for max == 0, lovers[n] = c; n++ in braces; worse: n >= 4 break in the body (22), max < n ? max : n
   alone (26). climb round 1 and climb --base on the 2-ins variant (2 rounds): nothing. Open: an n
   reference in the gm_FindChar / test block that keeps the store behind the test. prior: match-deep
   2026-11-17 (s8): draft -> shape, size 195 = 195: the fill loop walks rows (i++, rows++; rows->rec,
   ai_AmountRowStatus(rows)): the original's esi row pointer and frame 0x10 (n in a register all through,
   filter and me reloaded). Left: n and max swapped (the original n = ebx, max = ebp; ours n = ebp, max =
   ebx): c2trace --prio: max#1 prio 14 (memset's max * 56 counts 2 refs in a P = 4 block) > n#1 prio 9
   (lives unreferenced through the scan's call blocks 3/4 and the fill body 14), so max is served first and
   takes ebx. With the regs swapped the min reads n < max ? n : max (20 ins, kept); max < n ? max : n gives
   the same allocation plus an order hunk (26). Rounds: index form with count local 63, unsigned n 57,
   while scan = control, row local = control, row-walk (row = rows; row++) 59, lovers[n] stored before the
   test (no c) 26 but the store order is wrong, split n++ / nested if = control; climb --base rowp 2
   rounds: nothing. Next: what raises n above max (an n reference the original has, or max's memset refs). */

unsigned int ai_FillLoversTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    int n;
    unsigned int i;
    AICHARREC *lovers[4];
    AICHARREC *c;
    AIJOB *ev;

    n = 0;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x6f); ev != 0 && n < 4; ev = (AIJOB *)he_NextEvent()) {
        if (ev->partner1 == me->netId) {
            c = (AICHARREC *)gm_FindChar(ev->partner2);
            if (c != 0)
                lovers[n++] = c;
        }
    }
    if (max != 0) {
        memset(rows, 0, max * sizeof(LCAMOUNTROW));
        for (i = 0; i < (n < max ? n : max); i++, rows++) {
            rows->rec = lovers[i];
            ai_AmountRowStatus(rows);
            rows->pickable = ai_PassesPartnerFilter(me->id, rows->rec->id, filter, 0);
        }
    }
    return n;
}
#pragma pack(push, 1)
typedef struct LCOFFEREVENT {           /* the wedding offer event 0x2c's words over AIJOB's union (he.c HEOFFER, same
                                           layout) */
    char pad00[0xac];
    int who[6];                         /* 0xac, the characters of the wedding: the suitor, the partner, then four
                                           guests (the last -1 for none) */
    char kind;                          /* 0xc4, the offer kind */
    AITIME clock;                       /* 0xc5, when the wedding takes place */
} LCOFFEREVENT;
#pragma pack(pop)
/* 0x5bf7c0, the fill of the special-amount window's wedding tab (tab 4, 717 B): the weddings (the offer events 0x2c)
   me takes part in whose six characters exist (the last guest may be missing). With max 0 it counts their
   characters (5 or 6 per wedding); else it fills rows with the characters of the soonest such wedding at most 200
   years ahead (LCAMOUNTROW.rec, x2c the role: 0 the suitor, 1 the partner, 2, 3, 3, 4 the guests; x34 the event's
   id), up to max of them, each with the corner icons (ai_AmountRowStatus) and the partner filter filter
   (ai_PassesPartnerFilter, pickable); returns the count */

unsigned int ai_FillWeddingTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    AICHARREC *guests[6];
    int roles[6];
    LCOFFEREVENT *ev;
    AICHARREC *c;
    unsigned int n;
    int j;
    int soonest;
    int mine;
    int k;

    soonest = 200;
    n = 0;
    if (max == 0) {
        for (ev = (LCOFFEREVENT *)he_FindEvent(1, 0, 0x2c); ev != 0; ev = (LCOFFEREVENT *)he_NextEvent()) {
            mine = 0;
            c = (AICHARREC *)gm_FindChar(ev->who[0]);
            if (c == 0)
                continue;
            if (c == me)
                mine = 1;
            c = (AICHARREC *)gm_FindChar(ev->who[1]);
            if (c == 0)
                continue;
            if (c == me)
                mine = 1;
            c = (AICHARREC *)gm_FindChar(ev->who[2]);
            if (c == 0)
                continue;
            if (c == me)
                mine = 1;
            c = (AICHARREC *)gm_FindChar(ev->who[3]);
            if (c == 0)
                continue;
            if (c == me)
                mine = 1;
            c = (AICHARREC *)gm_FindChar(ev->who[4]);
            if (c == 0)
                continue;
            k = 5;
            if (c == me)
                mine = 1;
            c = (AICHARREC *)gm_FindChar(ev->who[5]);
            if (c != 0) {
                k = 6;
                if (c == me)
                    mine = 1;
            }
            if (mine)
                n += k;
        }
        return n;
    }
    for (ev = (LCOFFEREVENT *)he_FindEvent(1, 0, 0x2c); ev != 0; ev = (LCOFFEREVENT *)he_NextEvent()) {
        mine = 0;
        if (ev->clock.year - ai_Time.year > soonest)
            continue;
        guests[0] = (AICHARREC *)gm_FindChar(ev->who[0]);
        if (guests[0] == 0)
            continue;
        roles[0] = 0;
        if (guests[0] == me)
            mine = 1;
        guests[1] = (AICHARREC *)gm_FindChar(ev->who[1]);
        if (guests[1] == 0)
            continue;
        roles[1] = 1;
        if (guests[1] == me)
            mine = 1;
        guests[2] = (AICHARREC *)gm_FindChar(ev->who[2]);
        if (guests[2] == 0)
            continue;
        roles[2] = 2;
        if (guests[2] == me)
            mine = 1;
        guests[3] = (AICHARREC *)gm_FindChar(ev->who[3]);
        if (guests[3] == 0)
            continue;
        roles[3] = 3;
        if (guests[3] == me)
            mine = 1;
        guests[4] = (AICHARREC *)gm_FindChar(ev->who[4]);
        if (guests[4] == 0)
            continue;
        roles[4] = 3;
        if (guests[4] == me)
            mine = 1;
        guests[5] = (AICHARREC *)gm_FindChar(ev->who[5]);
        if (guests[5] != 0) {
            roles[5] = 4;
            if (guests[5] == me)
                mine = 1;
        }
        if (!mine)
            continue;
        soonest = ev->clock.year - ai_Time.year;
        for (j = 0, k = 0; j < 6 && j < max; j++) {
            if (guests[j] != 0) {
                rows[k].rec = guests[j];
                rows[k].x2c = roles[j];
                rows[k].x34 = ((AIJOB *)ev)->id;
                ai_AmountRowStatus(&rows[k]);
                rows[k].pickable = ai_PassesPartnerFilter(me->id, rows[k].rec->id, filter, 0);
                k++;
            }
        }
        n = k;
    }
    return n;
}
/* 0x5bfa90, the fill of the special-amount window's quarrel tab (tab 5, 428 B): while me is in a quarrel
   (AICHARREC.powerFlags 0x400) the other side of its insult event 0x47 (role 0), then the characters (id4) of the
   crime events 0x18 of me (role 1). With max 0 it only counts them; else it fills rows with them (LCAMOUNTROW.rec,
   x2c the role; the crimes up to max rows) and gives each the corner icons (ai_AmountRowStatus) and the partner
   filter filter (ai_PassesPartnerFilter, pickable); returns the count */

unsigned int ai_FillQuarrelTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    AIJOB *ev;
    AICHARREC *c;
    unsigned int count;
    unsigned int n;
    int i;

    count = 0;
    if (max == 0) {
        if ((me->powerFlags & 0x400) != 0) {
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x47); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                if (ev->partner1 == me->netId) {
                    if (gm_FindChar(ev->partner2) != 0)
                        count = 1;
                    break;
                }
                if (ev->partner2 == me->netId) {
                    if (gm_FindChar(ev->partner1) != 0)
                        count = 1;
                    break;
                }
            }
        }
        for (ev = (AIJOB *)he_FindEvent(2, 2, me->id, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (gm_FindChar(ev->id4) != 0)
                count++;
        }
        return count;
    }
    n = 0;
    c = 0;
    if ((me->powerFlags & 0x400) != 0) {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x47); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == me->netId) {
                c = (AICHARREC *)gm_FindChar(ev->partner2);
                break;
            }
            if (ev->partner2 == me->netId) {
                c = (AICHARREC *)gm_FindChar(ev->partner1);
                break;
            }
        }
    }
    if (c != 0) {
        rows[0].rec = c;
        rows[0].x2c = 0;
        n = 1;
    }
    for (ev = (AIJOB *)he_FindEvent(2, 2, me->id, 0, 0x18); ev != 0 && n < max; ev = (AIJOB *)he_NextEvent()) {
        c = (AICHARREC *)gm_FindChar(ev->id4);
        if (c != 0) {
            rows[n].rec = c;
            rows[n].x2c = 1;
            n++;
        }
    }
    for (i = 0; i < (int)n; i++) {
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
    }
    return n;
}
/* 0x5bfc40, the fill of the special-amount window's candidacy tab (tab 6, 335 B): while me applies for an office
   (AICHARREC.appliedOffice), the office's applicants (amt_GetOfficeApplicants, up to 6) and up to three other
   characters applying for the same office. With max 0 it only counts them; else rows[0].x34 is the office and the
   rows take the applicants (x2c 0, up to min(max, count)) then the other characters (x2c 1, up to max rows in all),
   each with the corner icons (ai_AmountRowStatus) and the partner filter filter (ai_PassesPartnerFilter, pickable);
   returns the rows filled */

unsigned int ai_FillCandidacyTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    char *applicants[6];
    AICHARREC *rivals[3];
    AICHARREC *c;
    int nApplicants;
    int nRivals;
    unsigned int i;
    int j;

    if (me->appliedOffice == 0)
        return 0;
    nApplicants = amt_GetOfficeApplicants(me->appliedOffice, 6, applicants);
    nRivals = 0;
    for (c = (AICHARREC *)ai_Chars; (int)c < (int)ai_Chars[768]; c++) {  /* a signed compare (jl) in the original */
        if (nRivals >= 3)
            break;
        if (c->id != 0xffff && c->id != me->id && c->appliedOffice == me->appliedOffice)
            rivals[nRivals++] = c;
    }
    if (max == 0)
        return nRivals + nApplicants;
    rows[0].x34 = me->appliedOffice;
    for (i = 0; i < (max < (unsigned int)nApplicants ? max : nApplicants); i++) {
        rows[i].rec = (AICHARREC *)applicants[i];
        rows[i].x2c = 0;
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
    }
    for (j = 0; j < nRivals; j++) {
        if (i >= max)
            break;
        rows[i].rec = rivals[j];
        rows[i].x2c = 1;
        ai_AmountRowStatus(&rows[i]);
        rows[i].pickable = ai_PassesPartnerFilter(me->id, rows[i].rec->id, filter, 0);
        i++;
    }
    return i;
}
/* 0x5bfd90, the fill of the special-amount window's office tab (tab 7, 314 B): while me holds an office
   (AICHARREC.officeKind), the characters applying (AICHARREC.appliedOffice) for one of the open offices me is
   eligible for (amt_GetEligibleOffices, up to 4). With max 0 it only counts them; else it fills up to max rows
   with them, office by office (LCAMOUNTROW.rec, x34 the office), each with the corner icons (ai_AmountRowStatus)
   and the partner filter filter (ai_PassesPartnerFilter, pickable); returns the count */

unsigned int ai_FillOfficeTab(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter)
{
    int offices[4];
    AICHARREC *c;
    unsigned int n;
    int nOffices;
    int k;
    int i;

    n = 0;
    if (me->officeKind == 0)
        return 0;
    nOffices = amt_GetEligibleOffices((char *)me, 4, offices);
    if (nOffices == 0)
        return 0;
    if (max == 0) {
        for (i = 0; i < 768; i++) {
            if (((AICHARREC *)ai_Chars)[i].appliedOffice != 0) {
                for (k = 0; k < nOffices; k++) {
                    if (offices[k] == ((AICHARREC *)ai_Chars)[i].appliedOffice) {
                        n++;
                        break;
                    }
                }
            }
        }
        return n;
    }
    for (k = 0; k < nOffices; k++) {
        if (n >= max)
            break;
        for (c = (AICHARREC *)ai_Chars; (int)c < (int)ai_Chars[768]; c++) {
            if (offices[k] == c->appliedOffice) {
                rows[n].rec = c;
                rows[n].x34 = offices[k];
                ai_AmountRowStatus(&rows[n]);
                rows[n].pickable = ai_PassesPartnerFilter(me->id, rows[n].rec->id, filter, 0);
                n++;
                if (n >= max)
                    break;
            }
        }
    }
    return n;
}
/* 0x5bfed0, prepares a page of the special-amount window (144 B): selects the page param_2 of the view
   param_1, saves the page window's scroll offset, clears its text ("$C") and removes its objects. With the
   scroll page param_3 (not -1) the page's scroll offset is cleared, the scroll page's text is cleared and
   its scroll buttons (0x767) are created, then the page is selected again; returns 0. Without it the saved
   scroll offset is returned. */

int ai_AmountPageSwitch(int view, int page, int scrollPage)
{
    int saved;
    int win;

    d2_SetForm(view, page);
    saved = d2_Wins[d2_LastWindow].scrollY;
    io_PrintText("$C");
    d2_RemoveWindowObjects(d2_LastWindow, 1);
    if (scrollPage != -1) {
        d2_Wins[d2_LastWindow].scrollY = 0;
        win = d2_LastWindow;
        d2_SetForm(view, scrollPage);
        io_PrintText("$C");
        d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
        d2_SetForm(view, page);
        return 0;
    }
    return saved;
}
/* 0x5bff80, enables the slider (LCAMOUNTROW.ctl[1], the object of d2_WinOpenSlider that ai_AmountRow makes) of
   every one of the count rows of a special-amount row list */

void ai_EnableAmountRows(int count, LCAMOUNTROW *rows)
{
    for (; count > 0; count--) {
        d2_SetObjEnabled(rows->ctl[1], 1);
        rows++;
    }
}
/* 0x5bffb0, the rows of the special-amount window's family tab (tab 0, 249 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x1690] at the top and draws each
   row (ai_AmountRow at 0x4b, 0x28 + 0x70 per row, titled) with its relation text txt_Ptrs[0x1691 + x2c] right
   aligned beside it; enables the rows' sliders, restores the page's scroll offset and sets its scroll step 0x70;
   returns 1 (0 without rows) */

int ai_ShowFamilyTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x1690], half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = 0x4b;
        rows[i].y = y;
        ai_AmountRow(0x4b, y, d2_LastWindow, 0x56, &rows[i], 1);
        ai_RightAlignedText(txt_Ptrs[0x1691 + rows[i].x2c], 0x14a, y + 0x1e, 0xc8, 0x55);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c00b0, the rows of the special-amount window's friends tab (tab 1, 259 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x169b] at the top and draws each
   row centred (ai_AmountRow, the frame picture 0x5da's width, 0x28 + 0x70 per row, titled); enables the rows'
   sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1 (0 without rows) */

int ai_ShowFriendsTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x169b], half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 2;
        rows[i].y = y;
        ai_AmountRow(rows[i].x, y, d2_LastWindow, 0x56, &rows[i], 1);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c01c0, the rows of the special-amount window's enemies tab (tab 2, 259 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x169c] at the top and draws each
   row centred (ai_AmountRow, the frame picture 0x5da's width, 0x28 + 0x70 per row, titled); enables the rows'
   sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1 (0 without rows) */

int ai_ShowEnemiesTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x169c], half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 2;
        rows[i].y = y;
        ai_AmountRow(rows[i].x, y, d2_LastWindow, 0x56, &rows[i], 1);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c02d0, the rows of the special-amount window's lovers tab (tab 3, 259 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x169d] at the top and draws each
   row centred (ai_AmountRow, the frame picture 0x5da's width, 0x28 + 0x70 per row, titled); enables the rows'
   sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1 (0 without rows) */

int ai_ShowLoversTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x169d], half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 2;
        rows[i].y = y;
        ai_AmountRow(rows[i].x, y, d2_LastWindow, 0x56, &rows[i], 1);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c03e0, the rows of the special-amount window's wedding tab (tab 4, 367 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage) and centres the heading at the top: the suitor's name (rows[0],
   "%1N3"), the text txt_Ptrs[0x169e] and the partner's name (rows[1]); then draws each row (ai_AmountRow
   at 0x4b, 0x52 + 0x70 per row, titled) with its role text txt_Ptrs[0x169f + x2c] right aligned beside it;
   enables the rows' sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1 (0 without
   rows) */

int ai_ShowWeddingTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    int saved;
    char name[0x100];
    unsigned int i;
    int half;
    int y;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    txt_Format(name, (int)"%1N3", rows[0].rec->id);
    ai_CentredText(name, half, 8, half * 2, 0x55);
    ai_CentredText(txt_Ptrs[0x169e], half, 0x20, half * 2, 0x55);
    txt_Format(name, (int)"%1N3", rows[1].rec->id);
    ai_CentredText(name, half, 0x38, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x52 + i * 0x70;
        rows[i].x = 0x4b;
        rows[i].y = y;
        ai_AmountRow(0x4b, y, d2_LastWindow, 0x56, &rows[i], 1);
        ai_RightAlignedText(txt_Ptrs[0x169f + rows[i].x2c], 0x14a, y + 0x1e, 0xc8, 0x55);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c0550, the rows of the special-amount window's quarrel tab (tab 5, 249 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x16a4] at the top and draws each
   row (ai_AmountRow at 0x4b, 0x28 + 0x70 per row, titled) with its role text txt_Ptrs[0x16a5 + x2c] right
   aligned beside it; enables the rows' sliders, restores the page's scroll offset and sets its scroll step 0x70;
   returns 1 (0 without rows) */

int ai_ShowQuarrelTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x16a4], half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = 0x4b;
        rows[i].y = y;
        ai_AmountRow(0x4b, y, d2_LastWindow, 0x56, &rows[i], 1);
        ai_RightAlignedText(txt_Ptrs[0x16a5 + rows[i].x2c], 0x14a, y + 0x1e, 0xc8, 0x55);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c0650, the rows of the special-amount window's candidacy tab (tab 6, 312 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x16a7] (formatted with the office's
   text id 0x253 + rows[0].x34) at the top and draws each row (ai_AmountRow at 0x4b, 0x28 + 0x70 per row, titled)
   with its role text txt_Ptrs[0x16a8 + x2c] (applicant or rival) right aligned beside it; enables the rows'
   sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1 (0 without rows) */

int ai_ShowCandidacyTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    char caption[0x200];
    unsigned int i;
    int half;
    int y;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    txt_Format(caption, 0x16a7, rows[0].x34 + 0x253);
    ai_CentredText(caption, half, 8, half * 2, 0x55);
    for (i = 0; i < n; i++) {
        y = 0x28 + i * 0x70;
        rows[i].x = 0x4b;
        rows[i].y = y;
        ai_AmountRow(0x4b, y, d2_LastWindow, 0x56, &rows[i], 1);
        ai_RightAlignedText(txt_Ptrs[0x16a8 + rows[i].x2c], 0x14a, y + 0x1e, 0xc8, 0x55);
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
/* 0x5c0790, the rows of the special-amount window's office tab (tab 7, 277 B): with n rows, switches to the page
   (ai_AmountPageSwitch view, page, scrollPage), centres the caption txt_Ptrs[0x16aa] at the top and draws the rows
   from y 0x28 down, 0x70 apart (ai_AmountRow at 0x4b, titled); before the first row of each office (x34, filled
   office by office by ai_FillOfficeTab) it centres the office's name txt_Ptrs[0x253 + office] as a heading 0x19
   high; enables the rows' sliders, restores the page's scroll offset and sets its scroll step 0x70; returns 1
   (0 without rows) */

int ai_ShowOfficeTab(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows)
{
    unsigned int i;
    int half;
    int y;
    int office;
    int saved;

    if (n == 0)
        return 0;
    saved = ai_AmountPageSwitch(view, page, scrollPage);
    half = d2_Wins[d2_LastWindow].w / 2;
    ai_CentredText(txt_Ptrs[0x16aa], half, 8, half * 2, 0x55);
    y = 0x28;
    office = 0x28;  /* the original starts the last office at the first row's y (ebp = esi = 0x28) */
    for (i = 0; i < n; i++) {
        if (rows[i].x34 != office) {
            office = rows[i].x34;
            ai_CentredText(txt_Ptrs[0x253 + office], half, y, half * 2, 0x55);
            y += 0x19;
        }
        rows[i].x = 0x4b;
        rows[i].y = y;
        ai_AmountRow(0x4b, y, d2_LastWindow, 0x56, &rows[i], 1);
        y += 0x70;
    }
    ai_EnableAmountRows(n, rows);
    d2_Wins[d2_LastWindow].scrollY = saved;
    d2_Wins[d2_LastWindow].visible = 0x70;
    return 1;
}
typedef struct AIAMOUNTTAB {            /* 0x14 bytes, one tab of the special-amount list window's tab table:
                                           {id, count, control, fill function of the page, row builder} */
    int id;                             /* +0x00, 0 .. 7 */
    unsigned int count;                 /* +0x04, the rows of the page */
    int control;                        /* +0x08, the tab's button (ai_SpecialAmountWindow), -1: none yet */
    unsigned int (*fill)(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter);
                                        /* +0x0c, the page fill (ai_FillFamilyTab .. ai_FillOfficeTab) */
    int (*show)(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows);
                                        /* +0x10, the page's rows (ai_ShowFamilyTab .. ai_ShowOfficeTab) */
} AIAMOUNTTAB;
extern AIAMOUNTTAB ai_AmountTabs[];     /* 0x62d148, the eight tabs */

/* 0x5c08b0, clears the count and the control of the eight tabs of the special-amount list window (the ids and
   the two handlers stay): the count (+4) and the control (+8) of every tab */

void ai_ClearAmountTabs(void)
{
    int *p;

    for (p = &ai_AmountTabs[0].control; (int)p < (int)&ai_AmountTabs[8].control; p += 5) {
        p[-1] = 0;
        p[0] = 0;
    }
}
/* 0x5c08d0, the liking sliders of the special-amount list (144 B): for every row (0xe dwords: the character
   at +0, the slider at +8) whose character and slider exist the slider is set to gm_Liking(me's id, the row
   character's id, 1), 0 when me and the row character are both of the kinds 6 / 7; me is the player's
   character, the rows of one tab */

void ai_AmountLikingRows(AICHARREC *me, unsigned count, int *rows)
{
    AICHARREC *chr;

    if (me == 0 || me->id == 0xffff)
        return;
    for (; count > 0; count--) {
        chr = (AICHARREC *)*rows;
        if (chr != 0 && chr->id != 0xffff && rows[2] != -1) {
            if ((me->kind == 6 || me->kind == 7) && (chr->kind == 6 || chr->kind == 7))
                d2_SetObjValue4(rows[2], 0, 100, 0);
            else
                d2_SetObjValue4(rows[2], 0, 100, (int)gm_Liking(me->id, chr->id, 1));
        }
        rows += 0xe;
    }
}
/* 0x5c0970, the tabbed special-amount window "special\amt3" (1972 B): counts the rows of the eight tabs
   (ai_AmountTabs fill with max 0) and opens tab (0 .. 7; else the last one, ai_AmountTab), or the first tab with
   rows when it has none; prints caption, makes one tab button per tab down the side (d2_WinCreateSlider, the tab
   name txt_Ptrs[0x1688 + id], the window's height shared out evenly) and shows the open tab's rows (up to 16).
   Each frame: a right release closes it; holding the right button on a row's slider shows the tab's likings
   (ai_AmountLikingRows) as that character, letting go returns to the player's; a click on a tab button opens
   that tab when it has rows (else disables its button), a click on a row's frame chooses its character (without
   pick the window closes with it, else pick (character, 0) decides, the window disabled meanwhile); every 0xd8
   ticks while the page is not scrolled the tabs are counted and the open one refilled again (state 3). Returns
   the chosen character record (0: none) */

int ai_SpecialAmountWindow(unsigned int *filter, char *caption, int tab, int (*pick)(AICHARREC *, int))
{
    LCAMOUNTROW rows[16];
    int page;
    int hit;
    unsigned long refresh;
    AICHARREC *chosen;
    int state;
    AICHARREC *viewer;
    int win;
    unsigned int prev;
    int i;
    int k;
    int step;
    int rest;
    int y;

    hit = 0;
    refresh = d3_Time;
    ai_ClearAmountTabs();
    if (tab > -1 && tab < 8)
        ai_AmountTab = tab;
    if (ai_AmountTab >= 8)
        ai_AmountTab = 0;
    for (i = 0; i < 8; i++)
        ai_AmountTabs[i].count = ai_AmountTabs[i].fill(&lc_CharRecs[ai_PlayerId], 0, rows, filter);
    if (ai_AmountTabs[ai_AmountTab].count == 0) {
        ai_AmountTab = 8;
        for (i = 0; i < 8; i++) {
            if (ai_AmountTabs[i].count != 0) {
                ai_AmountTab = i;
                break;
            }
        }
    }
    win = d2_OpenForm(0, 0, "special\\amt3");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    page = d2_LastWindow;
    d2_SetForm(win, 1);
    io_PrintText(caption);
    d2_SetForm(win, 4);
    y = d2_Wins[d2_LastWindow].h - 0x107;
    step = y / 7;
    rest = y % 7;
    y = 0;
    for (i = 0; i < 8; i++, y += step + (--rest >= 0) + 0x21) {
        ai_AmountTabs[i].control = d2_WinCreateSlider((d2_Wins[d2_LastWindow].w - 0xb9) / 2, y, 0xbf,
                                                      txt_Ptrs[0x1688 + ai_AmountTabs[i].id], d2_LastWindow);
        d2_SetObjWidth(ai_AmountTabs[i].control, 0xb9);
    }
    if (ai_AmountTab < 8) {
        d2_SetForm(win, 2);
        ai_AmountTabs[ai_AmountTab].count =
            ai_AmountTabs[ai_AmountTab].fill(&lc_CharRecs[ai_PlayerId], 16, rows, filter);
        ai_AmountTabs[ai_AmountTab].show(win, 2, 3, ai_AmountTabs[ai_AmountTab].count, rows);
    }
    d2_ClickedId = -1;
    state = 3;
    viewer = &lc_CharRecs[ai_PlayerId];
    do {
        if (state == 3) {
            prev = -1;
            if (ai_AmountTab >= 8)
                ai_AmountTab = 0;
            else
                prev = ai_AmountTabs[ai_AmountTab].count;
            for (i = 0; i < 8; i++)
                ai_AmountTabs[i].count = ai_AmountTabs[i].fill(&lc_CharRecs[ai_PlayerId], 0, rows, filter);
            if (ai_AmountTabs[ai_AmountTab].count == 0) {
                ai_AmountTab = 8;
                for (i = 0; i < 8; i++) {
                    if (ai_AmountTabs[i].count != 0) {
                        ai_AmountTab = i;
                        break;
                    }
                }
            }
            for (i = 0; i < 8; i++) {
                d2_SetObjEnabled(ai_AmountTabs[i].control, ai_AmountTabs[i].count > 0);
                d2_Objs[ai_AmountTabs[i].control].checked = i == ai_AmountTab;
            }
            if (ai_AmountTab < 8) {
                ai_AmountTabs[ai_AmountTab].count =
                    ai_AmountTabs[ai_AmountTab].fill(&lc_CharRecs[ai_PlayerId], 16, rows, filter);
                if (ai_AmountTabs[ai_AmountTab].count != prev)
                    ai_AmountTabs[ai_AmountTab].show(win, 2, 3, ai_AmountTabs[ai_AmountTab].count, rows);
                else  /* the same number of rows: keep the page's scroll offset */
                    ai_AmountTabs[ai_AmountTab].show(win, 2, -1, ai_AmountTabs[ai_AmountTab].count, rows);
                ai_AmountLikingRows(viewer, ai_AmountTabs[ai_AmountTab].count, (int *)rows);
            } else {
                ai_AmountPageSwitch(win, 2, 3);
                viewer = &lc_CharRecs[ai_PlayerId];
            }
        } else if (state == 1) {
            ai_AmountTabs[hit].count = ai_AmountTabs[hit].fill(&lc_CharRecs[ai_PlayerId], 0, 0, filter);
            if (ai_AmountTabs[hit].count != 0) {
                d2_Objs[ai_AmountTabs[ai_AmountTab].control].checked = 0;
                ai_AmountTab = hit;
                d2_Objs[ai_AmountTabs[hit].control].checked = 1;
                ai_AmountTabs[ai_AmountTab].count =
                    ai_AmountTabs[ai_AmountTab].fill(&lc_CharRecs[ai_PlayerId], 16, rows, filter);
                ai_AmountTabs[ai_AmountTab].show(win, 2, 3, ai_AmountTabs[ai_AmountTab].count, rows);
            } else {
                d2_SetObjEnabled(ai_AmountTabs[hit].control, 0);
            }
        } else if (state == 4) {
            ai_AmountLikingRows(viewer, ai_AmountTabs[ai_AmountTab].count, (int *)rows);
        } else if (state == 2) {
            refresh = 0;
        }
        chosen = 0;
        state = 0;
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (dm_L.rhold != 0 && viewer == &lc_CharRecs[ai_PlayerId]) {
            chosen = 0;
            viewer = 0;
            k = -1;
            for (i = 0; i < ai_AmountTabs[ai_AmountTab].count; i++) {
                if (rows[i].ctl[1] == d2_PointerObj) {
                    k = i;
                    break;
                }
            }
            if (k > -1 && k < ai_AmountTabs[ai_AmountTab].count && rows[k].rec != 0 && rows[k].rec->id != 0xffff) {
                state = 4;
                viewer = rows[k].rec;
            }
        } else if (dm_L.rhold == 0 && viewer != &lc_CharRecs[ai_PlayerId]) {
            state = 4;
            viewer = &lc_CharRecs[ai_PlayerId];
        } else if (d2_ClickedId != -1) {
            for (i = 0; i < 8; i++) {
                if (ai_AmountTabs[i].control == d2_PointerObj) {
                    state = 1;
                    hit = i;
                    goto next;
                }
            }
            for (i = 0; i < ai_AmountTabs[ai_AmountTab].count; i++) {
                if (rows[i].ctl[0] == d2_PointerObj) {
                    state = 2;
                    hit = i;
                    if (pick == 0) {
                        sv_PopRequest = 1;
                        chosen = rows[i].rec;
                    } else {
                        d2_SetViewEnabled(win, 0);
                        chosen = pick(rows[i].rec, 0) ? rows[i].rec : 0;
                        d2_SetViewEnabled(win, 1);
                    }
                }
            }
        } else if (refresh <= d3_Time && d2_Wins[page].scrollRemY == 0) {
            state = 3;
            refresh = d3_Time + 0xd8;
        }
next:;
    } while (sv_RunFrame((int)ai_SpecialAmountWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return (int)chosen;
}
/* 0x5c1130, the special-amount menu "special\amt1" (975 B): the caption (caption, else the text 0xa1), the
   tabbed window's button (the text 0x1687, its object in form 3) and nine picture buttons, each named by its
   kind's text txt_Ptrs[0x12b2 + kind] in the order 0, 8, 6, 4, 5, 1, 2, 3, 7; the second (kind 8) takes no
   clicks when cfg->lockSecond is set, the last (kind 7) only while someone holds a column 7 office
   (amt_HasColumn7Holder). A click on a picture opens the kind's list (ai_SpecialAmountListWindow), on the
   button the tabbed window (ai_SpecialAmountWindow, the last tab), the menu disabled meanwhile; a character
   chosen there closes the menu too unless pick decides or cfg->option14 has bit 4; a right release closes it.
   Returns the chosen character record (0: none); prompt is not read */

int ai_AmountMenu(AMOUNTCFG *cfg, char *caption, int prompt, int (*pick)(AICHARREC *, int))
{
    int ctrl;
    int btn[9];
    int order[9];
    int win;
    int r;
    int i;
    int hit;

    (void)prompt;
    order[0] = 0;
    order[1] = 8;
    order[2] = 6;
    order[3] = 4;
    order[4] = 5;
    order[5] = 1;
    order[6] = 2;
    order[7] = 3;
    order[8] = 7;
    win = d2_OpenForm(0, 0, "special\\amt1");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    if (caption != 0)
        io_PrintText(caption);
    else
        io_PrintText((char *)0xa1);
    d2_SetForm(win, 3);
    ctrl = d2_GetWinObj(win, 3, io_PrintText((char *)0x1687));
    d2_SetForm(win, 1);
    btn[0] = d2_WinOpenPic(0x45, 0x3c, 0x589, d2_LastWindow);
    btn[1] = d2_WinOpenPic(0x10f, 0x25, 0x599, d2_LastWindow);
    btn[2] = d2_WinOpenPic(0x10f, 0x93, 0x58d, d2_LastWindow);
    btn[3] = d2_WinOpenPic(0x69, 0xab, 0x590, d2_LastWindow);
    btn[4] = d2_WinOpenPic(0x1b5, 0xab, 0x58c, d2_LastWindow);
    btn[5] = d2_WinOpenPic(0x45, 0x125, 0x58a, d2_LastWindow);
    btn[6] = d2_WinOpenPic(0x10f, 0x125, 0x58f, d2_LastWindow);
    btn[7] = d2_WinOpenPic(0x1d9, 0x125, 0x58b, d2_LastWindow);
    btn[8] = d2_WinOpenPic(0x1d9, 0x3c, 0x58e, d2_LastWindow);
    for (i = 0; i < 9; i++) {
        if (i != 1 || cfg->lockSecond == 0) {
            d2_Objs[btn[i]].interactive = 1;
            d2_Objs[btn[i]].style = 3;
        }
        d2_Objs[btn[i]].font = 0x55;
        ai_CentredText(txt_Ptrs[0x12b2 + order[i]], d2_Objs[btn[i]].x - d2_LastWindowPtr->x + 0x20,
                       d2_Objs[btn[i]].y - d2_LastWindowPtr->y - 0x19, 200, 0x55);
    }
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    do {
        if (amt_HasColumn7Holder() == 0) {
            d2_Objs[btn[8]].interactive = 0;
            d2_Objs[btn[8]].style = 0;
        } else {
            d2_Objs[btn[8]].interactive = 1;
            d2_Objs[btn[8]].style = 3;
        }
        r = 0;
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            hit = -1;
            for (i = 0; i < 9; i++) {
                if (btn[i] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit > -1 && hit < 9) {
                d2_SetViewEnabled(win, 0);
                r = ai_SpecialAmountListWindow(order[hit], (unsigned int *)cfg, caption, 0, pick);
                d2_SetViewEnabled(win, 1);
            } else if (d2_PointerObj == ctrl) {
                d2_SetViewEnabled(win, 0);
                r = ai_SpecialAmountWindow((unsigned int *)cfg, caption, -1, pick);
                d2_SetViewEnabled(win, 1);
            }
            if (r != 0 && pick == 0 && (cfg->option14 & 4) == 0)
                sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_AmountMenu, 0x657c7) != 0);
    d2_CloseView(win);
    return r;
}
/* 0x5c1500, the office list of one amount kind "special\amt2" (1394 B), the callback of the Rathaus office info
   menu (ai_OpenAmountKindMenu): the caption (the kind's name 0x12b2 + kind, or the text 0xa0 with the viewing
   character) and the offices of the column kind (1 .. 6 and 8: amt_GetColumnOffices, 7: amt_GetColumn7Offices,
   up to 6) as rows with their holders (gm_FindChar), the corner icons written in line as in
   ai_SpecialAmountListWindow, drawn by ai_ShowOfficeRows (ai_ShowAmountRows for the kinds 0 / 7) with their
   frames and sliders enabled. Another kind, or no office, returns at once (the window stays open). Holding the
   right button on a row's slider views the list as that character; a click on a row's frame opens the info box
   of the row's office (ai_AmountInfoWindow, the window disabled meanwhile; a vacant seat ends the viewing) */

void ai_AmountListWindow(int kind)
{
    AICHARREC *viewer;
    int viewing;
    int win;
    LCAMOUNTROW rows[8];
    AMTREC seats7[6];
    AMTREC seats[6];
    AICHARREC *c;
    AIJOB *ev;
    float health;
    int redraw;
    int n;
    int i;
    int hit;

    n = 0;
    viewer = 0;
    win = d2_OpenForm(0, 0, "special\\amt2");
    d2_CentreViewWindows(win);
    viewing = 0;
    redraw = 1;
    do {
        if (redraw) {
            d2_SetForm(win, 2);
            io_PrintText("$C");
            if (viewing != 0)
                io_PrintText(0xa0, viewer->id);
            else
                io_PrintText("$Z$[%s$]", kind + 0x12b2);
            d2_SetForm(win, 3);
            io_PrintText("$C");
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            for (i = 0; i < 8; i++) {
                rows[i].rec = 0;
                rows[i].ctl[0] = -1;
                rows[i].office = 0;
            }
            if ((kind > 0 && kind < 7) || kind == 8) {
                n = amt_GetColumnOffices(kind, 6, seats);
                for (i = 0; i < n; i++) {
                    rows[i].rec = (AICHARREC *)gm_FindChar(seats[i].owner);
                    rows[i].office = seats[i].kind;
                }
            } else if (kind == 7) {
                n = amt_GetColumn7Offices(0, 6, seats7);
                for (i = 0; i < n; i++) {
                    rows[i].rec = (AICHARREC *)gm_FindChar(seats7[i].owner);
                    rows[i].office = seats7[i].kind;
                }
            } else {
                return;
            }
            if (n == 0)
                return;
            for (i = 0; i < n; i++) {
                /* the corner icons, as ai_AmountRowStatus (written out here: the original has the code in line) */
                if (rows[i].rec == 0)
                    continue;
                rows[i].icons[0] = rows[i].rec->faith ? 0x6c9 : 0x6ca;
                rows[i].icons[1] = 0;
                if (lc_CharRecs[ai_PlayerId].sex != rows[i].rec->sex && (rows[i].rec->powerFlags & LC_LOVER_FLAG) != 0) {
                    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x6f); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && rows[i].rec->netId == ev->partner2) {
                            rows[i].icons[1] = 0x6cc;
                            break;
                        }
                    }
                }
                rows[i].icons[2] = 0;
                if (gv_HeldCrimeCount(rows[i].rec->netId, ai_PlayerId) > 0) {
                    rows[i].icons[2] = 0x6cd;
                } else {
                    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->chr == ai_PlayerId && rows[i].rec->netId == ev->id4) {
                            rows[i].icons[2] = 0x6ce;
                            break;
                        }
                    }
                }
                rows[i].icons[3] = 0;
                for (ev = (AIJOB *)he_FindEvent(1, 0, 0x45); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                    if (ev->partner1 == lc_CharRecs[ai_PlayerId].netId && rows[i].rec->netId == ev->partner2) {
                        rows[i].icons[3] = 0x6cb;
                        break;
                    }
                }
                if (rows[i].icons[3] == 0) {
                    health = gm_CharacterHealth(rows[i].rec);
                    if (health < 100.0f)
                        rows[i].icons[3] = 0x6d2;
                    else if (health < 350.0f)
                        rows[i].icons[3] = 0x6d1;
                    else if (health < 650.0f)
                        rows[i].icons[3] = 0x6d0;
                    else
                        rows[i].icons[3] = 0x6cf;
                }
            }
            if (kind == 0 || kind == 7)
                ai_ShowAmountRows(d2_LastWindow, kind, 0, n, rows, viewer ? viewer : &lc_CharRecs[ai_PlayerId]);
            else
                ai_ShowOfficeRows(d2_LastWindow, kind, 0, n, rows, viewer ? viewer : &lc_CharRecs[ai_PlayerId]);
            for (i = 0; i < n; i++) {
                d2_SetObjEnabled(rows[i].ctl[0], 1);
                if (rows[i].ctl[1] != -1)
                    d2_SetObjEnabled(rows[i].ctl[1], 1);
            }
            redraw = 0;
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (dm_L.rhold != 0 && viewing == 0) {
            viewer = 0;
            hit = -1;
            for (i = 0; i < n; i++) {
                if (rows[i].ctl[1] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit > -1 && hit < n && rows[hit].rec != 0) {
                viewer = 0;
                redraw = 1;
                if (rows[hit].rec->id != 0xffff) {
                    viewing = 1;
                    viewer = rows[hit].rec;
                }
            }
        } else if (dm_L.rhold == 0 && viewing != 0) {
            viewing = 0;
            viewer = 0;
            redraw = 1;
        } else if (dm_L.lclick != 0 && d2_ClickedId != -1) {
            hit = -1;
            for (i = 0; i < n; i++) {
                if (rows[i].ctl[0] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit > -1 && hit < n) {
                c = rows[hit].rec;
                if (c == 0 || c->id == 0xffff) {
                    viewer = 0;
                    viewing = 0;
                }
                d2_SetViewEnabled(win, 0);
                ai_AmountInfoWindow(rows[hit].office, c);
                d2_SetViewEnabled(win, 1);
            }
            redraw = 1;
        }
    } while (sv_RunFrame(ai_AmountListWindow, 0x657c7) != 0);
    d2_CloseView(win);
}
/* 0x5c1a80, the info box of an amount kind (the form "special\amtinfo"): the caption "$Z$[ %s $]$A$Z" over
   the kind's name text (0x253 + kind), the two texts 2 * kind + 0x134b / 0x134c, then the frame pump
   (0x5309a0, this, 0x657c7) with dm_L.rup ending it; closes the view. Both callers also pass a character
   (the row's holder, or 0), which the box does not read */

void ai_AmountInfoWindow(int kind, AICHARREC *ch)
{
    int win;

    (void)ch;
    win = d2_OpenForm(0, 0, "special\\amtinfo");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText("$Z$[ %s $]$A$Z", kind + 0x253);
    d2_SetForm(win, 1);
    io_PrintText(kind * 2 + 0x134b);
    io_PrintText(kind * 2 + 0x134c);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_AmountInfoWindow, 0x657c7) != 0);
    d2_CloseView(win);
}


/* 0x5c1b20, the office info entry of the Rathaus name plate: opens the seven-button amount kind menu
   (ai_AmountKindMenu) with the amount list window (ai_AmountListWindow) as its callback and the
   contact_AMT_INFO name plate text (txt_Ptrs[0x8c]) as its caption, then gives the camera back to the
   player */

void ai_OpenAmountKindMenu(void)
{
    ai_AmountKindMenu(ai_AmountListWindow, txt_Ptrs[0x8c], 1);
    sv_CameraFollow();
}
/* 0x5c1b40, the seven-button amount kind menu "special\amt1" (686 B): the caption (caption, else the text 0xa2)
   and the picture buttons of the kinds 8, 6, 4, 5, 1, 2, 3 (the pictures 0x599 / 0x58d / 0x590 / 0x58c / 0x58a /
   0x58f / 0x58b), each named by its kind's text txt_Ptrs[0x12b2 + kind]; the first (kind 8) takes clicks only
   with withKind8. A click on a button calls pick with its kind (the menu disabled meanwhile); a right release
   closes the menu */

void ai_AmountKindMenu(void (*pick)(int kind), char *caption, int withKind8)
{
    int win;
    int btn[7];
    int order[7];
    int i;
    int hit;

    order[0] = 8;
    order[1] = 6;
    order[2] = 4;
    order[3] = 5;
    order[4] = 1;
    order[5] = 2;
    order[6] = 3;
    win = d2_OpenForm(0, 0, "special\\amt1");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    if (caption != 0)
        io_PrintText(caption);
    else
        io_PrintText((char *)0xa2);
    d2_SetForm(win, 1);
    btn[0] = d2_WinOpenPic(0x10f, 0x25, 0x599, d2_LastWindow);
    btn[1] = d2_WinOpenPic(0x10f, 0x93, 0x58d, d2_LastWindow);
    btn[2] = d2_WinOpenPic(0x69, 0xab, 0x590, d2_LastWindow);
    btn[3] = d2_WinOpenPic(0x1b5, 0xab, 0x58c, d2_LastWindow);
    btn[4] = d2_WinOpenPic(0x45, 0x125, 0x58a, d2_LastWindow);
    btn[5] = d2_WinOpenPic(0x10f, 0x125, 0x58f, d2_LastWindow);
    btn[6] = d2_WinOpenPic(0x1d9, 0x125, 0x58b, d2_LastWindow);
    for (i = 0; i < 7; i++) {
        if (i != 0 || withKind8 != 0) {
            d2_Objs[btn[i]].interactive = 1;
            d2_Objs[btn[i]].style = 3;
        }
        d2_Objs[btn[i]].font = 0x55;
        ai_CentredText(txt_Ptrs[0x12b2 + order[i]], d2_Objs[btn[i]].x - d2_LastWindowPtr->x + 0x20,
                       d2_Objs[btn[i]].y - d2_LastWindowPtr->y - 0x19, 200, 0x55);
    }
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            hit = -1;
            for (i = 0; i < 7; i++) {
                if (btn[i] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (pick != 0 && hit > -1 && hit < 7) {
                d2_SetViewEnabled(win, 0);
                pick(order[hit]);
                d2_SetViewEnabled(win, 1);
            }
        }
    } while (sv_RunFrame((int)ai_AmountKindMenu, 0x657c7) != 0);
    d2_CloseView(win);
}
/* 0x5c1df0, the agenda entry of the Rathaus name plate: opens the seven-button amount kind menu
   (ai_AmountKindMenu) with the office list (ai_AgendaOfficeList) as its callback and the Tagesordnung
   caption text (txt_Ptrs[0x139c]) as its caption, then gives the camera back to the player */

void ai_OpenAgendaMenu(void)
{
    ai_AmountKindMenu(ai_AgendaOfficeList, txt_Ptrs[0x139c], 1);
    sv_CameraFollow();
}
/* 0x5c1e10, the agenda of one office column "special\tagesordnung" (846 B), the callback of the Rathaus agenda
   menu (ai_OpenAgendaMenu): nothing when the column has no offices (amt_GetColumnOffices, up to 6). The window
   (two scrolled pages, forms 2 and 3) is titled with the text 0x13a3 + kind; page 2 (0x139e) lists the offices
   of state 2 with their holder and the character in x14 (0x139f: x14's id, the holder's id, the office's name
   0x253 + kind, 0x27b + kind for a female holder); page 3 lists, for the columns below 8 (0x13a0), every office
   open for applications (state 3: 0x13a1 with its name) with up to four characters applying for it
   (AICHARREC.appliedOffice); for the column 8 (0x13a2) the pending election's office (hmt_GetElectionObjType:
   0x13a3 with its name) and up to four characters of its group (AICHARREC.flags1cc 0x8000 for 0x24, 0x10000
   for 0x25, else 0x4000). An empty page says 0x139d. A right release closes it */

void ai_AgendaOfficeList(int kind)
{
    int shown;
    int n;
    int count;
    AICHARREC *group[4];
    AMTREC seats[6];
    AICHARREC *holder;
    AICHARREC *other;
    AICHARREC *c;
    unsigned int mask;
    int listed;
    int found;
    int type;
    int win;
    int view;
    int i;
    int m;

    n = amt_GetColumnOffices(kind, 6, seats);
    if (n <= 0)
        return;
    win = d2_OpenForm(0, 0, "special\\tagesordnung");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    view = d2_LastWindow;
    d2_SetForm(win, 4);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, view, 0x767);
    d2_SetForm(win, 3);
    view = d2_LastWindow;
    d2_SetForm(win, 5);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, view, 0x767);
    d2_SetForm(win, 1);
    io_PrintText(kind + 0x13a3);
    d2_SetForm(win, 2);
    io_PrintText(0x139e);
    listed = 0;
    for (i = 0; i < n; i++) {
        if (seats[i].state != 2)
            continue;
        holder = (AICHARREC *)gm_FindChar(seats[i].owner);
        other = (AICHARREC *)gm_FindChar(seats[i].x14);
        if (holder != 0 && other != 0) {
            listed |= 1;
            io_PrintText(0x139f, other->id, holder->id, holder->sex == 0 ? seats[i].kind + 0x253 : seats[i].kind + 0x27b);
        }
    }
    if (listed == 0)
        io_PrintText(0x139d);
    d2_SetForm(win, 3);
    if (kind < 8) {
        io_PrintText(0x13a0);
        shown = 0;
        for (i = 0; i < n; i++) {
            if (seats[i].state != 3)
                continue;
            found = 0;
            shown |= 1;
            io_PrintText(0x13a1, seats[i].kind + 0x253);
            count = 0;
            for (c = lc_CharRecs; (int)c < (int)&lc_CharRecs[0x300]; c++) {  /* a signed compare (jl) in the original */
                if (count >= 4)
                    break;
                if (c->appliedOffice == seats[i].kind) {
                    found |= 1;
                    io_PrintText("$A$Z%1N2", c->id);
                    count++;
                }
            }
            if (found == 0)
                io_PrintText(0x139d);
        }
    } else {
        type = hmt_GetElectionObjType();
        group[0] = 0;
        group[1] = 0;
        group[2] = 0;
        group[3] = 0;
        mask = 0;
        if (type != 0) {
            if (type == 0x24)
                mask = 0x8000;
            else
                mask = type == 0x25 ? 0x10000 : 0x4000;
        }
        m = 0;
        for (c = lc_CharRecs; m < 4; c++) {
            if ((int)c >= (int)&lc_CharRecs[0x300])
                break;
            if (c->flags1cc & mask)
                group[m++] = c;
        }
        shown = m;
        io_PrintText(0x13a2);
        if (type != 0) {
            io_PrintText(0x13a3, type + 0x253);
            for (i = 0; i < m; i++) {
                if (i == 0)
                    io_PrintText("$2A$Z%1N2", group[0]->id);
                else
                    io_PrintText("$A$Z%1N2", group[i]->id);
            }
        }
    }
    if (shown == 0)
        io_PrintText(0x139d);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_OpenAgendaMenu, 0x657c7) != 0);
    d2_CloseView(win);
}

int gv_TableRecordText();

/* 0x5c2160, the law book (657 B): the book 0x111 (criminal law: the model "ob_GESETZBUCH_STRAFRECHT", the laws
   0x11 .. 0x1c, the section text 0x13f8, the info texts from 0x1452), 0x112 (taxes: "ob_GESETZBUCH_STEUERN",
   9 .. 0x10, 0x13f7, from 0x142a) or 0x113 (constitution: "ob_GESETZBUCH_VERFASSUNG", 0 .. 8, 0x13f6, from
   0x13fd) opens at "Buch" with the two pages "Buch\Gesetzbuch" (sv_OpenBook); another book does nothing. Page 1
   gets the section text and the first laws (6 / 4 / 5), page 2 the rest, each a line of its decision record's
   text (gv_TableRecordText, "%it$1>%s$N$0>"); the page numbers 0x1a38. A click on a law's line opens its info
   box (ai_LawBookInfoWindow: the info text base + 5 * i and the law's current value as a text, by
   AIDEC.valueText the value itself or a name of 0x13dd / 0x13e6 / 0x13ec + it). A right release closes the
   book; the camera goes back to the player */

void ai_OpenLawBook(int book)
{
    int base;
    int start;
    int split;
    int page;
    AIDEC rec;
    int ctrl[12];
    char text[0x200];
    SVBOOK *bk;
    int section;
    int end;
    int n;
    int i;
    int hit;

    switch (book) {
    case 0x111:
        bk = sv_OpenBook("ob_GESETZBUCH_STRAFRECHT", "Buch", 2, "Buch\\Gesetzbuch", "Buch\\Gesetzbuch");
        start = 0x11;
        end = 0x1d;
        base = 0x1452;
        section = 0x13f8;
        split = 6;
        break;
    case 0x112:
        bk = sv_OpenBook("ob_GESETZBUCH_STEUERN", "Buch", 2, "Buch\\Gesetzbuch", "Buch\\Gesetzbuch");
        start = 9;
        end = 0x11;
        base = 0x142a;
        section = 0x13f7;
        split = 4;
        break;
    case 0x113:
        bk = sv_OpenBook("ob_GESETZBUCH_VERFASSUNG", "Buch", 2, "Buch\\Gesetzbuch", "Buch\\Gesetzbuch");
        start = 0;
        end = 9;
        base = 0x13fd;
        section = 0x13f6;
        split = 5;
        break;
    default:
        return;
    }
    d2_SetForm(bk->views[0], 1);
    io_PrintText(section);
    d2_SetForm(bk->views[0], 2);
    page = bk->views[0];
    for (n = start, i = 0; n < end; n++, i++) {
        if (i == split) {
            d2_SetForm(bk->views[1], 2);
            page = bk->views[1];
        }
        gv_TableRecordText(text, n);
        ctrl[i] = d2_GetWinObj(page, 2, io_PrintText("%it$1>%s$N$0>", text));
    }
    d2_SetForm(bk->views[0], 3);
    io_PrintText(0x1a38, 1);
    d2_SetForm(bk->views[1], 3);
    io_PrintText(0x1a38, 2);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            hit = -1;
            for (i = 0; i < 12; i++) {
                if (ctrl[i] == d2_PointerObj) {
                    hit = i;
                    break;
                }
            }
            if (hit != -1) {
                gv_GetTableRecord(hit + start, &rec);
                switch (rec.valueText) {
                case 0:
                    ai_LawBookInfoWindow(base + hit * 5, rec.cur);
                    break;
                case 1:
                    ai_LawBookInfoWindow(base + hit * 5, rec.cur + 0x13dd);
                    break;
                case 2:
                    ai_LawBookInfoWindow(base + hit * 5, rec.cur + 0x13e6);
                    break;
                case 3:
                    ai_LawBookInfoWindow(base + hit * 5, rec.cur + 0x13ec);
                    break;
                }
            }
        }
        sv_UpdateBook(bk);
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_OpenLawBook, 0x677c7) != 0);
    sv_CloseBook(bk);
    sv_CameraFollow();
}
/* 0x5c2410, the law book info box "Gesetze\Gesetzbuch_Info" (176 B): the caption text id + 1 (with the id and
   the law text as arguments) and the body text id + 2 through 0x515960, the pump 0x5309a0 (this, 0x677c7) and
   0x4758f0 (win) */

void ai_LawBookInfoWindow(int textId, int lawText)
{
    int win;

    win = d2_OpenForm(0, 0, "Gesetze\\Gesetzbuch_Info");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(textId + 1, textId, lawText);
    d2_SetForm(win, 2);
    io_PrintText(textId + 2);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_LawBookInfoWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5c2490, fills the master letter window's work-settings page (464 B): the order checkbox (text 0x194f, on / off
   0x1951 + bit 0 of the flags), for a building of another class than 5 the three option checkboxes (texts 0x1954 /
   0x1957 / 0x195a, bits 1 .. 3; else those controls are -1), the budget counter (text 0x1953, 1000 .. 9000, the
   converted budget *budget) and, for a class other than 5 with a production setting, the weight factor slider
   (text 0x42c2, 50 .. 150, the setting's factor in percent) into *factorCtrl (else -1)
   @deep deep s38: left set.flags (the field web k#b, 8 refs, prio 186) in edi and checks (prio 198) in esi
   where the original has them the other way round (checks is served first and takes esi). flags as a named
   local (at the top or at the first use), checks[0] as *checks, the -1 stores first / reversed, (flags &
   4) != 0 spellings, every subset of the four on values inlined into their two uses (15 variants): =
   control or worse; climb round 1 (49 mutants): nothing. Open: a set.flags reference more in the first
   block (P 15, +30 each) or one checks reference fewer would flip them; the original's parameter may have
   been the four words of LCWORKSET as scalars (flags a parameter candidate). */

void ai_ShowWorkSettingsPage(int win, int *budgetCtrl, int *budget, int *checks, AIBLD *bld, int charId,
                             LCWORKSET set, int *factorCtrl, G8SETTAIL *setting)
{
    int ctrl;
    unsigned int on;
    AIBTYPE *type;

    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1949);
    d2_SetForm(win, 2);
    io_PrintText("$C$A");
    on = set.flags & 1;
    ctrl = d2_GetWinObj(win, 2, io_PrintText(0x194f, 0, charId, on + 0x1951));
    checks[0] = ctrl;
    d2_SetObjValue2(ctrl, on);
    checks[1] = -1;
    checks[2] = -1;
    checks[3] = -1;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (type->kind != 5) {
        on = (set.flags >> 1) & 1;
        ctrl = d2_GetWinObj(win, 2, io_PrintText(0x1954, 0, charId, on + 0x1955));
        checks[1] = ctrl;
        d2_SetObjValue2(ctrl, on);
        on = (set.flags >> 2) & 1;
        ctrl = d2_GetWinObj(win, 2, io_PrintText(0x1957, 0, charId, on + 0x1958));
        checks[2] = ctrl;
        d2_SetObjValue2(ctrl, on);
        on = (set.flags >> 3) & 1;
        ctrl = d2_GetWinObj(win, 2, io_PrintText(0x195a, 0, charId, on + 0x195b));
        checks[3] = ctrl;
        d2_SetObjValue2(ctrl, on);
    }
    ctrl = d2_GetWinObj(win, 2, io_PrintText(0x1953, charId, ai_OwnPlayer));
    *budgetCtrl = ctrl;
    d2_SetObjValue4(ctrl, 1000, 9000, *budget);
    if (type->kind != 5 && setting != 0) {
        io_PrintText(0x42c2);
        ctrl = d2_WinOpenSlider(100, 300, 200, 0x32, 0x96, 0x7b, 0x1002, d2_LastWindow);
        *factorCtrl = ctrl;
        d2_SetObjValue4(ctrl, 0x32, 0x96, (int)(setting->factor * 100.0f));
        return;
    }
    *factorCtrl = -1;
}
/* 0x5c26a0, the master letter notice window "berufe\_meisterbrief_1" (176 B): the form on the screen
   (0x52ea30), the master letter caption 0x1949 and the text 0x194e, the pump 0x5309a0 (this, 0x677c7) and
   0x4758f0 (win) */

void ai_MasterLetterNoticeWindow(AIBLD *bld)
{
    int win;

    win = d2_OpenForm(0, 0, "berufe\\_meisterbrief_1");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1949);
    io_PrintText(0x194e);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_MasterLetterNoticeWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5c2730, the master letter window of the building kinds 5 / 9 "berufe\meisterbrief_1" (124 B; the
   ai_MasterLetterWindow mode 0x40): the 0x5c26a0 body with the single caption 0x195e (0x52ea30 the screen,
   the pump 0x5309a0 (this, 0x677c7), 0x4758f0 win) */

void ai_MasterLetterKindWindow(AIBLD *bld)
{
    int win;

    win = d2_OpenForm(0, 0, "berufe\\_meisterbrief_1");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x195e);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_MasterLetterKindWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5c27b0, the master letter's hire window "berufe\_meisterbrief_1" (608 B; mode = ai_MasterLetterWindow's mode):
   for the mode 0x10 the texts 0x1949 / 0x194b and the hire button 0x194d, for 0x20 the two buttons of the text
   0x1938 (hire, close); any other mode closes the window at once. The hire button sends the master hire request
   (AIMSG type 7, hireMaster 1, the room the panel shows or -1 at home) unless a master hire event of the building
   (he_FindEvent 2, 0, 7, 3, its hireTitle 0) already runs, shows the text 0x1939 (the type's first job) and closes
   the window; the second button and a right click close it too */

void ai_MasterHireWindow(AIBLD *bld, int mode)
{
    int closeButton;
    int hireButton;
    AIMSG msg;
    char text[0x200];
    int t;
    AIJOB *job;
    int win;

    closeButton = -1;
    win = d2_OpenForm(0, 0, "berufe\\_meisterbrief_1");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    switch (mode) {
    case 0x10:
        io_PrintText(0x1949);
        io_PrintText(0x194b);
        hireButton = d2_GetWinObj(win, 1, io_PrintText(0x194d));
        break;
    case 0x20:
        t = io_PrintText(0x1938);
        hireButton = d2_GetWinObj(win, 1, t);
        closeButton = d2_GetWinObj(win, 1, t + 1);
        break;
    default:
        d2_CloseView(win);
        return;
    }
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == hireButton) {
                for (job = (AIJOB *)he_FindEvent(2, 0, 7, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                    if (job->hireTitle == 0)
                        break;
                if (job == 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 7;
                    msg.flags = 1;
                    msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.fromId = bld->id;
                    msg.time = ai_Time;
                    msg.hireMaster = 1;
                    if (gm_IsHomeType(bld) != 0)
                        msg.hireMasterRoom = -1;
                    else
                        msg.hireMasterRoom = ((AIROOM *)sv_PanelShown)->objId;
                    msg.hireTitle = 0;
                    cm_SendLongMessage((char *)&msg);
                    txt_Format(text, 0x1939, ((AIBTYPE *)ai_BTypes)[bld->type].jobs[0] + 0x130);
                    d2_ClearInput();
                    sv_MessageBox(text, 0);
                }
                sv_PopRequest = 1;
            } else if (d2_PointerObj == closeButton) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_MasterHireWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5c2a10, fills the master letter window's goods page (400 B): the title text 0x1949, then one row per slot of
   the production setting (seven, 0x30 pixels apart): the good's picture (d2 style 3, its AIGOOD record as the
   object's user data, interactive, checked while the slot is not locked) into locks[i] and the weight slider
   (0 .. 100, the slot's weight, disabled while locked) into sliders[i]; an empty slot gives -1 in both
   @deep deep s38: the empty-slot arm as the else and y, i stepped in the for's increment (y += 0x30 first)
   put y into win's dead parameter slot as in the original (32 -> 6 ins). Left: the latch's inc edi comes
   before add eax,0x30 in the original, and at the loop entry the slider cursor is stored to sliders' slot
   before y = 0xc. i++ written before y += 0x30 (in the increment or in the body) makes the cursor temp
   reach its count 4 before y in the walk and y loses win's slot (24 ins); 36 init x increment orders: 3
   codegens; i < 7 as the bound: 8 ins; climb 2 rounds (341 mutants): nothing. */

void ai_ShowSettingGoods(int win, G8SLOT *slots, int *sliders, int *locks)
{
    int i;
    int y;
    int pic;

    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1949);
    d2_SetForm(win, 2);
    io_PrintText("$C");
    for (i = 0, y = 0xc; y < 0x15c; y += 0x30, i++) {
        if (slots[i].good != 0) {
            pic = d2_WinOpenPic(0xf, i * 0x30, slots[i].good + 0xdf, d2_LastWindow);
            d2_Objs[pic].style = 3;
            d2_Objs[pic].user = &((AIGOOD *)ai_Goods)[slots[i].good];
            d2_SetObjEnabled(pic, 1);
            d2_Objs[pic].interactive = 1;
            d2_Objs[pic].checked = slots[i].lock == 0;
            locks[i] = pic;
            sliders[i] = d2_WinOpenSlider(100, y, 200, 0, 0x64, 0x7b, 0x802, d2_LastWindow);
            d2_SetObjValue4(sliders[i], 0, 0x64, slots[i].weight);
            d2_SetObjEnabled(sliders[i], slots[i].lock == 0);
        } else {
            sliders[i] = -1;
            locks[i] = -1;
        }
    }
}
/* 0x5c2b90, the master letter window "berufe\_meisterbrief_2" of a building's master (1609 B .. its switch tables
   0x5c31dc / 0x5c31ec): the building's production setting (ai_FindSettingSlots, else a new one by ai_ProdSetting,
   refreshed; none: only the work settings page, no page tabs), the budget converted for the player, and the page
   ai_MasterLetterPage shows: 0 the work settings (ai_ShowWorkSettingsPage), 1 the goods weights
   (ai_ShowSettingGoods), switched by the two tabs (txt_Ptrs 0xf60 / 0x4155). A goods slider sets the good's weight
   (ai_SetSettingWeight) and redraws the seven, a goods picture toggles the slot's lock, the factor slider sets the
   setting's factor (percent * 0.01), a checkbox of the work settings sets or clears its order flag in the master's
   and the building's work settings (field deltas). The exit writes the chosen budget into the building and adds
   the difference to the master's budget */

void ai_MasterLetterSettings(AIBLD *bld, AICHARREC *ch)
{
    int hasSetting;
    G8SLOT *slots;
    int budget;
    int budgetCtrl;
    int factorCtrl;
    int win;
    int workTab;
    int goodsTab;
    LCWORKSET set;
    int checks[4];
    int sliders[7];
    int locks[7];
    int i;

    hasSetting = 1;
    workTab = -1;
    goodsTab = -1;
    factorCtrl = -1;
    slots = ai_FindSettingSlots(bld->id);
    memset(sliders, -1, sizeof(sliders));
    if (slots == 0 && (slots = ai_ProdSetting((int)bld)) == 0) {
        ai_MasterLetterPage = 0;
        hasSetting = 0;
    } else {
        ai_RefreshProdSetting((G8BLD *)bld);
    }
    win = d2_OpenForm(0, 0, "berufe\\_meisterbrief_2");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    budget = gm_ConvertMoney(bld->budget, ai_OwnPlayer);
    set = ch->workSet;
    if (ai_MasterLetterPage == 0)
        ai_ShowWorkSettingsPage(win, &budgetCtrl, &budget, checks, bld, ch->id, ch->workSet, &factorCtrl,
                                (G8SETTAIL *)slots);
    else
        ai_ShowSettingGoods(win, slots, sliders, locks);
    if (hasSetting) {
        int tabs[2];

        d2_SetForm(win, 3);
        sv_ButtonRow(d2_LastWindow, 0, tabs, 2, txt_Ptrs[0xf60], txt_Ptrs[0x4155]);
        workTab = tabs[0];
        goodsTab = tabs[1];
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (dm_L.ldown != 0) {
            if (ai_MasterLetterPage == 1) {
                for (i = 0; i < 7; i++)
                    if (d2_PointerObj == sliders[i])
                        break;
                if (i < 7 && sliders[i] != -1) {
                    d2_ClickedId = -1;
                    ai_SetSettingWeight((int)bld, slots[i].good, d2_GetObjValue(sliders[i]));
                    for (i = 0; i < 7; i++)
                        if (sliders[i] != -1)
                            d2_SetObjValue4(sliders[i], 0, 100, slots[i].weight);
                }
            } else if (ai_MasterLetterPage == 0 && factorCtrl != -1 && d2_PointerObj == factorCtrl) {
                ai_SetSettingFactor((int)bld, d2_GetObjValue(factorCtrl) * 0.01f);
            }
        }
        if (dm_L.lclick != 0 && d2_PointerObj != -1) {
            for (i = 0; i < 7; i++) {
                if (d2_PointerObj == locks[i]) {
                    slots[i].lock = slots[i].lock == 0;
                    d2_Objs[locks[i]].checked = slots[i].lock == 0;
                    d2_SetObjEnabled(sliders[i], slots[i].lock == 0);
                }
            }
        }
        if (d2_ClickedId != -1) {
            if (ai_MasterLetterPage == 1) {
                if (d2_PointerObj == workTab) {
                    ai_ShowWorkSettingsPage(win, &budgetCtrl, &budget, checks, bld, ch->id, set, &factorCtrl,
                                            (G8SETTAIL *)slots);
                    ai_MasterLetterPage = 0;
                    d2_PointerObj = -1;
                } else {
                    for (i = 0; i < 7; i++)
                        if (d2_PointerObj == sliders[i])
                            break;
                    if (i < 7) {
                        ai_SetSettingWeight((int)bld, slots[i].good, d2_GetObjValue(sliders[i]));
                        for (i = 0; i < 7; i++)
                            if (sliders[i] != -1)
                                d2_SetObjValue4(sliders[i], 0, 100, slots[i].weight);
                    }
                }
            } else if (ai_MasterLetterPage == 0) {
                budget = d2_GetObjValue(budgetCtrl);
                if (d2_PointerObj == goodsTab) {
                    ai_ShowSettingGoods(win, slots, sliders, locks);
                    ai_MasterLetterPage = 1;
                    d2_PointerObj = -1;
                } else {
                    for (i = 0; i < 4; i++) {
                        if (d2_PointerObj == -1)
                            break;
                        if (d2_PointerObj == checks[i]) {
                            set = ch->workSet;
                            if (d2_GetObjValue(checks[i]) != 0) {
                                switch (i) {
                                case 0: set.flags |= 1; break;
                                case 1: set.flags |= 2; break;
                                case 2: set.flags |= 4; break;
                                case 3: set.flags |= 8; break;
                                }
                            } else {
                                switch (i) {
                                case 0: set.flags &= ~1; break;
                                case 1: set.flags &= ~2; break;
                                case 2: set.flags &= ~4; break;
                                case 3: set.flags &= ~8; break;
                                }
                            }
                            cm_BeginFieldBlock((char *)ch, ch->netId);
                            cm_AddFieldDelta(4, 1, (char *)&ch->workSet - cm_FieldObject, &set);
                            cm_SendFieldDeltas();
                            cm_BeginFieldBlock((char *)bld, bld->id);
                            cm_AddFieldDelta(4, 1, (char *)&bld->workSet - cm_FieldObject, &set);
                            cm_SendFieldDeltas();
                            ai_ShowWorkSettingsPage(win, &budgetCtrl, &budget, checks, bld, ch->id, set,
                                                    &factorCtrl, (G8SETTAIL *)slots);
                            d2_PointerObj = -1;
                        }
                    }
                }
            }
        }
    } while (sv_RunFrame(ai_MasterLetterSettings, 0x677c7) != 0);
    if (ai_MasterLetterPage == 0)
        budget = d2_GetObjValue(budgetCtrl);
    budget = gm_PlayerFactor(budget, ai_OwnPlayer);
    cm_BeginFieldBlock((char *)bld, bld->id);
    cm_AddFieldDelta(4, 1, (char *)&bld->budget - cm_FieldObject, &budget);
    cm_SendFieldDeltas();
    {
        int delta = budget - bld->budget;

        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldValues(4, 1, (char *)&ch->budget - cm_FieldObject, &delta);
        cm_SendFieldDeltas();
    }
    d2_CloseView(win);
}
/* 0x5c3200, opens the master letter of the player's building (448 B .. its switch tables 0x5c3360 / 0x5c3374):
   nothing unless the player owns it. The mode: 0x40 for the building classes 5 / 9 (ai_MasterLetterKindWindow),
   0x30 when the building has a master (gm_FindCharacterByMaster: ai_MasterLetterSettings), else 0x20, or 0x10 when
   the player's own job in the title group of the type's first job is one of the type's six jobs (the player is
   the master: ai_MasterHireWindow with that mode); a running master hire event (he_FindEvent 2, 0, 7, 3, its
   hireTitle 0) makes it 0x50 (ai_MasterLetterNoticeWindow). sv_CameraFollow after the window */

void ai_MasterLetterWindow(AIBLD *bld)
{
    AIBTYPE *type;
    AICHARREC *me;
    AICHARREC *master;
    AIJOB *job;
    int mode;
    int job2;
    int i;

    if (bld == 0 || ai_PlayerId != bld->owner)
        return;
    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    master = (AICHARREC *)gm_FindCharacterByMaster((int)bld);
    if (type->kind == 5 || type->kind == 9) {
        mode = 0x40;
    } else if (master != 0) {
        mode = 0x30;
    } else {
        mode = 0x20;
        job2 = gm_TitleJob(ai_PlayerId, gm_TitleGroup(type->jobs[0]));
        if (job2 != 0) {
            for (i = 0; i < 6; i++) {
                if (job2 == type->jobs[i]) {
                    master = me;
                    mode = 0x10;
                    break;
                }
            }
        }
    }
    for (job = (AIJOB *)he_FindEvent(2, 0, 7, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent()) {
        if (job->hireTitle == 0) {
            mode = 0x50;
            break;
        }
    }
    switch (mode) {
    case 0x10:
    case 0x20:
        ai_MasterHireWindow(bld, mode);
        break;
    case 0x30:
        ai_MasterLetterSettings(bld, master);
        break;
    case 0x40:
        ai_MasterLetterKindWindow(bld);
        break;
    case 0x50:
        ai_MasterLetterNoticeWindow(bld);
        break;
    }
    sv_CameraFollow();
}

int gm_IsUnderAge(unsigned short id); int ai_WorksForMaster();

/* 0x5c33c0, whether the character id passes the amount menu's partner filter cfg for the character me (982 B):
   never me itself, an empty record, a held character or the player's own; no filter or no filter bits pass all.
   Then a kind above 9 fails, with option14 bit 2 the ids listed in cfg->ids fail, and the bits of cfg->mode:
   1 / 2 not a woman / not a man, 0x1000 no living first relative, 4 of me's family (the same w050) or me's
   relative, 8 neither, 0x40 kind 1, 0x10 a title and kind 0, 0x100 an office or the kinds 5 .. 7, 0x200 / 0x400
   under age / not (gm_IsUnderAge), 0x2000 no model, 0x4000 not the kinds 5 .. 7, 0x8000 one of them, 0x800 an office,
   0x10000 an own building, 0x20000 / 0x20 not the power flags 0x800000 / 0x40000000, 0x80 the word +0x1f0 set,
   unless 0x40000 none of the flags1cc bits 0x3000; of the bits 0x100000 (kinds 6 / 7), 0x200000 (kind 5), 0x400000
   (not 5, working for a master: ai_WorksForMaster) and 0x800000 (kind 0) one must hold. With cfg->liking me's
   liking of id (gm_Liking) must lie in [likingMin, likingMax]. unused is never read */

int ai_PassesPartnerFilter(unsigned short me, unsigned short id, AMOUNTCFG *cfg, int unused)
{
    AICHARREC *rel;
    unsigned int mask;
    int i;
    int any;
    float liking;

    if (me == id)
        goto reject;
    if (lc_CharRecs[id].id == 0xffff)
        goto reject;
    if (lc_CharRecs[id].heldRounds > 0)
        goto reject;
    if (&lc_CharRecs[id] == (AICHARREC *)ai_PlayerChar)
        goto reject;
    if (cfg == 0)
        goto accept;
    mask = cfg->mode;
    if (mask == 0)
        goto accept;
    if (lc_CharRecs[id].kind >= 10)
        goto reject;
    if (cfg->option14 & 2) {
        for (i = 0; i < 4; i++)
            if (id == cfg->ids[i] && cfg->ids[i] != 0xffff)
                goto reject;
    }
    if (mask & 0x3ffff) {
        if ((mask & 1) && lc_CharRecs[id].sex == 1)
            goto reject;
        if ((mask & 2) && lc_CharRecs[id].sex == 0)
            goto reject;
        if ((mask & 0x1000) && lc_CharRecs[id].relatives[0] != -1) {
            rel = (AICHARREC *)gm_FindChar(lc_CharRecs[id].relatives[0]);
            if (rel != 0 && rel->health > 0)
                goto reject;
        }
        if ((cfg->mode & 4) && lc_CharRecs[id].w050 != lc_CharRecs[me].w050 &&
            lc_CharRecs[id].relatives[0] != lc_CharRecs[me].netId)
            goto reject;
        if (cfg->mode & 8) {
            if (lc_CharRecs[id].w050 == lc_CharRecs[me].w050)
                goto reject;
            if (lc_CharRecs[id].relatives[0] == lc_CharRecs[me].netId)
                goto reject;
        }
        if ((cfg->mode & 0x40) && lc_CharRecs[id].kind != 1)
            goto reject;
        if (cfg->mode & 0x10) {
            if (lc_CharRecs[id].title == 0)
                goto reject;
            if (lc_CharRecs[id].kind != 0)
                goto reject;
        }
        if ((cfg->mode & 0x100) && lc_CharRecs[id].kind != 6 && lc_CharRecs[id].kind != 7 && lc_CharRecs[id].kind != 5 && lc_CharRecs[id].officeKind == 0 &&
            lc_CharRecs[id].secondOffice == 0)
            goto reject;
        if ((cfg->mode & 0x200) && gm_IsUnderAge(id) == 0)
            goto reject;
        if ((cfg->mode & 0x400) && gm_IsUnderAge(id) != 0)
            goto reject;
        mask = cfg->mode;
        if ((mask & 0x2000) && lc_CharRecs[id].model != 0)
            goto reject;
        if ((mask & 0x4000) && (lc_CharRecs[id].kind == 6 || lc_CharRecs[id].kind == 7 || lc_CharRecs[id].kind == 5))
            goto reject;
        if ((mask & 0x8000) && lc_CharRecs[id].kind != 6 && lc_CharRecs[id].kind != 7 && lc_CharRecs[id].kind != 5)
            goto reject;
        if ((mask & 0x800) && lc_CharRecs[id].officeKind == 0 && lc_CharRecs[id].secondOffice == 0)
            goto reject;
        if ((mask & 0x10000) && lc_CharRecs[id].bld2 == 0)
            goto reject;
        if ((mask & 0x20000) && (lc_CharRecs[id].powerFlags & 1 << 23))
            goto reject;
        if ((mask & 0x20) && (lc_CharRecs[id].powerFlags & 0x40000000))
            goto reject;
        if ((mask & 0x80) && lc_CharRecs[id].w1f0 == 0)
            goto reject;
        if (!(mask & 0x40000) && (lc_CharRecs[id].flags1cc & 0x3000))
            goto reject;
    }
    if (mask & PF_ONE_OF) {
        any = 0;
        any |= (mask & PF_KIND67) && (lc_CharRecs[id].kind == 6 || lc_CharRecs[id].kind == 7);
        any |= (mask & PF_KIND5) && lc_CharRecs[id].kind == 5;
        any |= (mask & PF_SERVANT) && lc_CharRecs[id].kind != 5 && ai_WorksForMaster(&lc_CharRecs[id]);
        if (!(((cfg->mode & PF_KIND0) && lc_CharRecs[id].kind == 0) | any))
            goto reject;
    }
    if (cfg->liking != 0) {
        liking = gm_Liking(me, id, 1);
        if (!(liking < cfg->likingMin)) {
            if (liking > cfg->likingMax)
                goto reject;
        } else
            return 0;
    }
accept:         /* the tests share these two exits (a return at each test duplicates the epilogue: 437 vs 279 instructions) */
    return 1;
reject:
    return 0;
}


/* 0x5c37a0, whether the building bld passes the amount menu's building filter cfg for the character me (480 B
   .. its switch table 0x5c3980): no filter passes all, no building or character none, an empty filter (no kind
   bits, case 0) all. The building's class (AIBTYPE.buildingClass, 0 .. 63) must be one of cfg->filter's bits (classMask); bit 63
   also wants a building that can be entered (not flags5a bit 0) with a scene object. cfg->kind is the owner test:
   0 any, 1 no owner, 2 an owner, 3 me or none, 4 me, 5 not me, 6 another one, 7 another one not the player; with
   option14 bit 0 an owner listed in cfg->ids fails */

int ai_PassesBuildingFilter(AICHARREC *me, AMOUNTCFG *cfg, AIBLD *bld)
{
    AIBTYPE *type;
    unsigned int lo;
    unsigned int hi;
    int match;
    int ownerOk;
    int listed;
    int i;
    unsigned int mask;
    unsigned int mask0;

    if (cfg == 0)
        return 1;
    if (bld == 0 || me == 0)
        return 0;
    mask0 = cfg->classMask[0];
    if (mask0 == 0 && cfg->classMask[1] == 0 && cfg->kind == 0)
        return 1;
    match = 0;
    ownerOk = 0;
    listed = 1;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    mask = cfg->classMask[1];
    lo = 0;
    hi = 0;
    if (type->kind > 0x1f)
        hi = 1 << (type->kind - 0x20);
    else
        lo = 1 << type->kind;
    if (mask & 0x80000000) {
        if (bld->flags5a & 1)
            return 0;
        if (bld->object == 0)
            return 0;
    }
    if ((mask0 & lo) || (mask & hi))
        match = 1;
    if (cfg->kind == 0)
        ownerOk = 1;
    else switch (cfg->kind) {
    case 0:
        ownerOk = 1;
        break;
    case 1:
        ownerOk = bld->owner == 0xffff;
        break;
    case 2:
        ownerOk = bld->owner != 0xffff;
        break;
    case 3:
        ownerOk = bld->owner == me->id || bld->owner == 0xffff;
        break;
    case 4:
        ownerOk = bld->owner == me->id;
        break;
    case 5:
        ownerOk = bld->owner != me->id;
        break;
    case 6:
        ownerOk = bld->owner != me->id && bld->owner != 0xffff;
        break;
    case 7:
        ownerOk = bld->owner != me->id && bld->owner != *ai_PlayerChar && bld->owner != 0xffff;
        break;
    }
    if ((cfg->option14 & 1) && bld->owner != 0xffff) {
        for (i = 0; i < 4; i++) {
            if (bld->owner == cfg->ids[i] && cfg->ids[i] != 0xffff) {
                listed = 0;
                break;
            }
        }
    }
    return match && ownerOk && listed;
}
/* 0x5c39a0, whether the town map's mark of the building bld passes the request filter cfg for the character me
   (496 B .. its switch table 0x5c3b90; ai_BuildTownMap greys out the marks it fails): no filter passes all, no
   building or character none, a filter without class bits and owner test all. A building of the classes
   ai_BuildingTypeMasks[4..5] fails under classMask[1] bit 30, one that cannot be entered (flags5a bit 0) under
   bit 63. The class must be one of classMask's bits (or classMask empty); the owner test cfg->kind and the ids of
   option14 bit 0 as in ai_PassesBuildingFilter */

int ai_PassesMapPickFilter(AICHARREC *me, AMOUNTCFG *cfg, AIBLD *bld)
{
    AIBTYPE *type;
    unsigned int lo;
    unsigned int hi;
    int match;
    int ownerOk;
    int listed;
    int i;

    if (cfg == 0)
        return 1;
    if (bld == 0 || me == 0)
        return 0;
    if (cfg->classMask[0] == 0 && cfg->kind == 0)
        return 1;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    lo = 0;
    hi = 0;
    if (type->kind > 0x1f)
        hi = 1 << (type->kind - 0x20);
    else
        lo = 1 << type->kind;
    if (((ai_BuildingTypeMasks[4] & lo) || (ai_BuildingTypeMasks[5] & hi)) && (cfg->classMask[1] & 0x40000000))
        return 0;
    ownerOk = 0;
    match = 0;
    listed = 1;
    if ((cfg->classMask[1] & 0x80000000) && (bld->flags5a & 1))
        return 0;
    if ((cfg->classMask[0] == 0 && cfg->classMask[1] == 0) || (lo & cfg->classMask[0]) || (hi & cfg->classMask[1]))
        match = 1;
    if (cfg->kind == 0)
        ownerOk = 1;
    else switch (cfg->kind) {
    case 0:
        ownerOk = 1;
        break;
    case 1:
        ownerOk = bld->owner == 0xffff;
        break;
    case 2:
        ownerOk = bld->owner != 0xffff;
        break;
    case 3:
        ownerOk = bld->owner == me->id || bld->owner == 0xffff;
        break;
    case 4:
        ownerOk = bld->owner == me->id;
        break;
    case 5:
        ownerOk = bld->owner != me->id;
        break;
    case 6:
        ownerOk = bld->owner != me->id && bld->owner != 0xffff;
        break;
    case 7:
        ownerOk = bld->owner != me->id && bld->owner != *ai_PlayerChar && bld->owner != 0xffff;
        break;
    }
    if ((cfg->option14 & 1) && bld->owner != 0xffff) {
        for (i = 0; i < 4; i++) {
            if (bld->owner == cfg->ids[i] && cfg->ids[i] != 0xffff) {
                listed = 0;
                break;
            }
        }
    }
    return match && ownerOk && listed;
}
typedef struct LCPICKROW {         /* 0x20 bytes, one character row of ai_PickBuildingChar */
    int icon;                      /* 0x00, the row's picture object 0x5db, the click target */
    int portrait;                  /* 0x04, the character's portrait (AICHARREC.picture) */
    int name;                      /* 0x08, the name text */
    int bar;                       /* 0x0c, the liking slider (disabled: a bar) */
    int pad10;
    int id;                        /* 0x14, the character id */
    int pad18[2];
} LCPICKROW;

/* 0x5c3bb0, the character choice of a building, the window "Gesetze\Auswahl2" (1357 B): lists the building's master
   and owner, then (building class 1 / 2, a home) the characters living in it (AICHARREC.bld2) or (the classes 4 .. 9,
   0xb .. 0xe, 0x10, 0x12 .. 0x16) the ones working in it (AICHARREC.bld), up to 16 rows, the caption caption when
   given and the scroll buttons past two rows. Each row is the picture 0x5db, the portrait, the name centred below it
   and the player's liking of the character as a disabled bar; a row cfg's partner filter refuses is greyed and not
   clickable (or left out when cfg->flags is 0). A click on a row returns its character record, or with a callback cb
   calls cb (the record, bld) and redraws the rows; a right click closes the window. Returns the picked record or 0 */

AICHARREC *ai_PickBuildingChar(AIBLD *bld, AMOUNTCFG *cfg, char *caption, void (*cb)(AICHARREC *, AIBLD *))
{
    LCPICKROW rows[16];
    char text[32];
    AICHARREC *result;
    int win;
    int listWin;
    int radio;
    int n;
    int i;
    int y;
    int w;
    int h;
    int draw;
    int usable;
    int shown;
    int step;
    int liking;
    int label;
    int bar;
    int sel;
    AIBTYPE *bt;

    result = 0;
    radio = -1;
    n = 0;
    draw = 1;
    if (bld == 0)
        return 0;
    if (bld->masterId != 0xffff)
        rows[n++].id = bld->masterId;
    if (bld->owner != 0xffff && bld->masterId != bld->owner)
        rows[n++].id = bld->owner;
    bt = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (bt->kind == 1 || bt->kind == 2) {
        for (i = 0; i < 768; i++) {
            if (n >= 16)
                break;
            if (lc_CharRecs[i].bld2 == (int)bld && lc_CharRecs[i].id != bld->masterId && lc_CharRecs[i].id != bld->owner)
                rows[n++].id = lc_CharRecs[i].id;
        }
    }
    if ((bt->kind > 3 && bt->kind < 10) || (bt->kind > 10 && bt->kind < 15) ||
        bt->kind == 16 || (bt->kind > 17 && bt->kind < 23)) {
        for (i = 0; i < 768; i++) {
            if (n >= 16)
                break;
            if (lc_CharRecs[i].bld == bld && lc_CharRecs[i].id != bld->masterId && lc_CharRecs[i].id != bld->owner)
                rows[n++].id = lc_CharRecs[i].id;
        }
    }
    win = d2_OpenForm(0, 0, "Gesetze\\Auswahl2");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    if (caption != 0) {
        d2_SetForm(win, 1);
        io_PrintText(caption);
    }
    if (n > 2) {               /* the list part (2) scrolls by the buttons on the frame part (0) */
        d2_SetForm(win, 2);
        listWin = d2_LastWindow;
        d2_SetForm(win, 0);
        d2_CreateScrollButtons(-1, -1, d2_LastWindow, listWin, 0x767);
    }
    d2_ClickedId = -1;
    do {
        if (draw) {
            d2_SetForm(win, 2);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            w = d2_Pics[0x5db].w;
            h = d2_Pics[0x5db].h;
            if (n != 0) {
            y = 0x1e;
            for (i = 0; i < n; i++) {
                usable = 1;
                shown = 1;
                if (cfg != 0 && cfg->mode != 0) {
                    usable = ai_PassesPartnerFilter(ai_PlayerId, (unsigned short)rows[i].id, cfg, (int)bld);
                    shown = cfg->flags | usable;
                }
                if (shown != 0) {
                    rows[i].icon = d2_WinOpenPic(0x96, y, 0x5db, d2_LastWindow);
                    if (usable) {
                        if (radio == -1)
                            radio = d2_CreateRadioGroup(1, rows[i].icon);
                        else
                            d2_RadioAdd(radio, rows[i].icon);
                    }
                    liking = (int)gm_Liking(ai_PlayerId, rows[i].id, 1);
                    rows[i].portrait = d2_WinOpenPic(0x98, y + 1, lc_CharRecs[rows[i].id].picture, d2_LastWindow);
                    txt_Format(text, (int)"%1N3", rows[i].id);
                    label = d2_CreateWinText(0x96 - (0xa0 - w) / 2, h + y - 0x46, text, d2_LastWindow);
                    rows[i].name = label;
                    d2_Objs[label].centred = 1;
                    d2_Objs[label].w = 0xa0;
                    d2_SetObjFont(label, 0x56);
                    d2_Objs[rows[i].icon].interactive = usable;
                    d2_Objs[rows[i].icon].greyed = usable == 0;
                    bar = d2_WinOpenSlider(0x8c, h + y + 4, 0x40, 0, 100, 0x75, 0x442, d2_LastWindow);
                    rows[i].bar = bar;
                    d2_SetObjValue4(bar, 0, 100, liking);
                    d2_SetObjEnabled(bar, 0);
                    d2_MoveObj(d2_LastWindowPtr->x - (d2_Objs[bar].w - w) / 2 + 0x96, d2_LastWindowPtr->y + h + y + 4, bar);
                    step = 0x5a;
                } else {
                    step = 0;
                }
                y += step;
            }
            }
            draw = 0;
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == rows[i].icon) {
                    sel = i;
                    break;
                }
            }
            if (sel > -1) {
                result = &lc_CharRecs[rows[sel].id];
                if (cb != 0) {
                    cb(result, bld);
                    draw = 1;
                    if (radio != -1)
                        d2_RadioClear(radio);
                    radio = -1;
                } else {
                    sv_PopRequest = 1;
                }
            }
        }
    } while (sv_RunFrame((int)ai_PickBuildingChar, 0x677c7) != 0);
    if (radio != -1)
        d2_RadioClear(radio);
    d2_CloseView(win);
    return result;
}

/* unlinked: the selection entry of a building with a research class, an empty stub in this build that returns 0;
   LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040, where the original's tail jump in ai_LawSelectionEntry
   points. Defined above its caller as in the original: VC6 tail-calls it with the class stored over the first
   argument (mov [esp+4],edx; jmp), which it does only for a callee whose body it has seen */
int ai_ResearchSelectionEntry(int cls, AMOUNTCFG *filter, char *text, void (*cb)(AICHARREC *, AIBLD *))
{
    return 0;
}

/* 0x5c4100, the law selection entry of the map pick 0x5b2180: the picked building record; when its kind has a
   research class (0x50b090) it returns the research selection entry of that class (ai_ResearchSelectionEntry, 0
   in this build), else it opens the law selection window 0x5c3bb0 (param_2 the filter, param_3 the caption
   text, param_4 the pick callback) */
int ai_LawSelectionEntry(AIBLD *bld, AMOUNTCFG *filter, char *text, void (*cb)(AICHARREC *, AIBLD *))
{
    int cls;

    if (bld == 0)
        return 0;
    if (gm_ResearchClass(bld->type, &cls) != 0)
        return ai_ResearchSelectionEntry(cls, filter, text, cb);
    return (int)ai_PickBuildingChar(bld, filter, text, cb);
}

void d2_PlotLine(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);  /* 0x4764f0 */

typedef struct LCFAMILYBOX {        /* 0x10 bytes, one box of the family tree window */
    unsigned short id;             /* 0x00, the character id, 0xffff: the box is empty */
    short pad02;
    int ctrl;                      /* 0x04, the box picture object (d2_WinOpenPic), -1: no character in it */
    unsigned short x;              /* 0x08, the centre of the box */
    unsigned short y;              /* 0x0a, the top of the box */
    int pad0c;
} LCFAMILYBOX;

/* 0x5c4150, the family tree window "Special\Stammbaum": shows the character netId's parents (the father relatives[1] on the left,
   the mother relatives[2] on the right), its partner (unless
   one of the two has the powerFlags bit 0x40000), its siblings (the father's relatives[3..7] but itself, up to two on
   each side) and its children (relatives[3..7] of a kind below 10, up to five) as portrait boxes (the
   picture 0x5db, the portrait inside, an empty box 0x4fb) joined by lines, each box labelled with the
   character's name ("%1N7") and the character itself marked with its ref54 picture. The caption is the text
   caption (0x9f when 0). The flags pick which boxes can be clicked: the middle row and the children with 1 and
   without 0xc, or with 0x10; the parents without 0xc; a child also with 2 and 8 when it is older than 10. A
   click opens the info panel of the picked character (ai_CharInfoPanel: without its take-over button with 0x10
   or for the character itself, with it under 8), a click on no box redraws; the right button closes the window
   unless flags has 8. callId is the frame pump's call id; the campaign step 6 brackets the window.
   @deep Reconstructed in full against the original (all 1277 instructions read: the relatives walk, the
   parent / middle-row / sibling / child boxes with their d2_Objs style/user, the connecting PlotLines, the
   labels and the click dispatch); size now equal (4542 vs 4542, ours 1276 ins vs 1277), every call,
   constant and reference agrees. The remaining code hunks are allocation-driven instruction selection and
   store scheduling: +0x2b3 the original computes (half - w) + w/2 (ecx=ebx; sub ecx,edx; lea
   eax,[ecx+ebp]) where ours computes (w/2 - w) + half (mov ecx,ebp; sub ecx,edx; lea eax,[ecx+ebx]) -
   source variants (reordered terms, a left local) do not move it (a local adds a frame slot); the box
   loops store node.x with add eax,ebx / mov word [ebp+8],ax where ours uses lea ecx,[ebx+eax] / mov word
   [ebp+8],cx and stores ctrl first (+0x77c, +0x867, +0xa86, +0xb79); the line midpoints end in lea
   ebp,[eax+ebx] vs ours mov esi,eax; ...; add esi,ebx (+0xc68, +0xd52); the loop counters and 4W/5 sit in
   [esp+0x18]/[esp+0x10] swapped. proof stops at the sibling loop's store order (x before ctrl). */
void ai_FamilyTreeWindow(int netId, unsigned char flags, int caption, int callId)
{
    AICHARREC *ch;
    AICHARREC *rel;
    AICHARREC *pick;
    int win;
    int draw;
    int width;
    int half;
    int w;
    int h;
    int x;
    int mid;
    int lo;
    int hi;
    int i;
    int k;
    int found;
    int partner;
    int nSiblings;
    int nChildren;
    int siblingX[2];
    LCFAMILYBOX parents[2];
    LCFAMILYBOX row[6];             /* the siblings 0, 1, 4, 5, the character 2, its partner 3 */
    int evenX[4];
    int oddX[5];
    LCFAMILYBOX children[5];
    char rowText[256];
    char parentText[256];
    char childText[256];

    if (ai_RunAllows(1) == 0)
        return;
    ai_CampaignStep(6, 0, 0, 0);
    ch = (AICHARREC *)gm_FindChar(netId);
    if (ch == 0)
        return;
    win = d2_OpenForm(0, 0, "Special\\Stammbaum");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    if (caption != 0)
        io_PrintText(caption);
    else
        io_PrintText(0x9f);
    d2_SetForm(win, 1);
    draw = 1;
    d2_ClickedId = -1;
    width = d2_Wins[d2_LastWindow].w;
    do {
        if (draw) {
            half = width / 2;
            w = d2_Pics[0x5db].w;
            h = d2_Pics[0x5db].h;
            k = 0;
            nChildren = 0;
            memset(parents, 0xff, sizeof parents);
            nSiblings = 0;
            memset(row, 0xff, sizeof row);
            partner = 0;
            memset(children, 0xff, sizeof children);
            draw = 0;
            row[2].id = ch->id;
            rel = (AICHARREC *)gm_FindChar(ch->relatives[0]);
            if (rel != 0 && (ch->powerFlags & 0x40000) == 0 && (rel->powerFlags & 0x40000) == 0) {
                partner = 1;
                row[3].id = rel->id;
            }
            rel = (AICHARREC *)gm_FindChar(ch->relatives[2]);
            if (rel != 0)
                parents[1].id = rel->id;
            rel = (AICHARREC *)gm_FindChar(ch->relatives[1]);
            if (rel != 0) {
                parents[0].id = rel->id;
                for (i = 3; i < 8 && k < 6; i++) {
                    if (rel->relatives[i] != -1 && rel->relatives[i] != ch->netId) {
                        pick = (AICHARREC *)gm_FindChar(rel->relatives[i]);
                        if (pick != 0) {
                            row[k].id = pick->id;
                            k++;
                            nSiblings++;
                            if (k == 2)
                                k = 4;
                        }
                    }
                }
            }
            for (i = 3; i < 8 && nChildren < 5; i++) {
                if (ch->relatives[i] != -1) {
                    rel = (AICHARREC *)gm_FindChar(ch->relatives[i]);
                    if (rel != 0 && rel->kind < 10) {
                        children[nChildren].id = rel->id;
                        nChildren++;
                    }
                }
            }
            d2_SetForm(win, 1);
            d2_ClearPaintbox();
            d2_RemoveWindowObjects(d2_LastWindow, 1);

            /* the parents */
            parents[0].ctrl = d2_WinOpenPic(half - w - 0x5a, 10, 0x5db, d2_LastWindow);
            parents[0].x = half - w + w / 2 - 0x5a;
            parents[0].y = 10;
            if (parents[0].id != 0xffff) {
                d2_WinOpenPic(half - w - 0x58, 12, ((AICHARREC *)ai_Chars)[parents[0].id].picture, d2_LastWindow);
                d2_Objs[parents[0].ctrl].style = 3;
                d2_Objs[parents[0].ctrl].user = ai_Chars[parents[0].id];
            } else {
                d2_WinOpenPic(half - w - 0x58, 12, 0x4fb, d2_LastWindow);
                parents[0].ctrl = -1;
            }
            parents[1].ctrl = d2_WinOpenPic(half + 0x5a, 10, 0x5db, d2_LastWindow);
            parents[1].x = half + w / 2 + 0x5a;
            parents[1].y = 10;
            if (parents[1].id != 0xffff) {
                d2_WinOpenPic(half + 0x5c, 12, ((AICHARREC *)ai_Chars)[parents[1].id].picture, d2_LastWindow);
                d2_Objs[parents[1].ctrl].style = 3;
                d2_Objs[parents[1].ctrl].user = ai_Chars[parents[1].id];
            } else {
                d2_WinOpenPic(half + 0x5c, 12, 0x4fb, d2_LastWindow);
                parents[1].ctrl = -1;
            }

            /* the character and its partner */
            if (partner) {
                row[2].ctrl = d2_WinOpenPic(half - w - 0x10, 0x96, 0x5db, d2_LastWindow);
                row[2].x = half - w + w / 2 - 0x10;
                row[2].y = 0x96;
                if (row[2].id != 0xffff) {
                    d2_WinOpenPic(half - w - 0xe, 0x98, ((AICHARREC *)ai_Chars)[row[2].id].picture, d2_LastWindow);
                    d2_Objs[row[2].ctrl].style = 3;
                    d2_Objs[row[2].ctrl].user = ai_Chars[row[2].id];
                } else {
                    d2_WinOpenPic(half - w - 0xe, 0x98, 0x4fb, d2_LastWindow);
                    row[2].ctrl = -1;
                }
                row[3].ctrl = d2_WinOpenPic(half + 0x10, 0x96, 0x5db, d2_LastWindow);
                row[3].x = half + w / 2 + 0x10;
                row[3].y = 0x96;
                if (row[3].id != 0xffff) {
                    d2_WinOpenPic(half + 0x12, 0x98, ((AICHARREC *)ai_Chars)[row[3].id].picture, d2_LastWindow);
                    d2_Objs[row[3].ctrl].style = 3;
                    d2_Objs[row[3].ctrl].user = ai_Chars[row[3].id];
                } else {
                    d2_WinOpenPic(half + 0x12, 0x98, 0x4fb, d2_LastWindow);
                    row[3].ctrl = -1;
                }
            } else {
                row[2].ctrl = d2_WinOpenPic(half - w / 2, 0x96, 0x5db, d2_LastWindow);
                row[2].x = half;
                row[2].y = 0x96;
                if (row[2].id != 0xffff) {
                    d2_WinOpenPic(half - w / 2 + 2, 0x98, ((AICHARREC *)ai_Chars)[row[2].id].picture, d2_LastWindow);
                    d2_Objs[row[2].ctrl].style = 3;
                    d2_Objs[row[2].ctrl].user = ai_Chars[row[2].id];
                } else {
                    d2_WinOpenPic(half - w / 2 + 2, 0x98, 0x4fb, d2_LastWindow);
                    row[2].ctrl = -1;
                }
            }

            /* the siblings, two on each side */
            if (nSiblings != 0) {
                siblingX[0] = width / 5 + 0x10;
                siblingX[1] = width * 4 / 5 - w - 0x10;
                for (i = 0; i < 2; i++) {
                    if (row[i].id == 0xffff)
                        break;
                    row[i].ctrl = d2_WinOpenPic(siblingX[i], 0x96, 0x5db, d2_LastWindow);
                    row[i].x = w / 2 + siblingX[i];
                    row[i].y = 0x96;
                    d2_WinOpenPic(siblingX[i] + 2, 0x98, ((AICHARREC *)ai_Chars)[row[i].id].picture, d2_LastWindow);
                    d2_Objs[row[i].ctrl].style = 3;
                    d2_Objs[row[i].ctrl].user = ai_Chars[row[i].id];
                }
                siblingX[0] = width / 5 - w - 0x10;
                siblingX[1] = width * 4 / 5 + 0x10;
                for (i = 4; i < 6; i++) {
                    if (row[i].id == 0xffff)
                        break;
                    row[i].ctrl = d2_WinOpenPic(siblingX[i - 4], 0x96, 0x5db, d2_LastWindow);
                    row[i].x = siblingX[i - 4] + w / 2;
                    row[i].y = 0x96;
                    d2_WinOpenPic(siblingX[i - 4] + 2, 0x98, ((AICHARREC *)ai_Chars)[row[i].id].picture, d2_LastWindow);
                    d2_Objs[row[i].ctrl].style = 3;
                    d2_Objs[row[i].ctrl].user = ai_Chars[row[i].id];
                }
            }

            /* the lines from the parents down to the middle row */
            mid = (row[2].y - h - 10) / 2 + h + 10;
            for (i = 0; i < 2; i++)
                d2_PlotLine(parents[i].x, parents[i].y + h + 5, parents[i].x, mid, 0x10, 0x10, 0x10);
            lo = parents[0].x;
            hi = parents[1].x;
            for (i = 0; i < 6; i++) {
                if (row[i].id != 0xffff && i != 3) {
                    x = row[i].x;
                    d2_PlotLine(x, row[i].y, x, mid, 0x10, 0x10, 0x10);
                    if (x < lo)
                        lo = x;
                    if (x > hi)
                        hi = x;
                }
            }
            d2_PlotLine(lo, mid, lo != hi ? hi + 1 : hi, mid, 0x10, 0x10, 0x10);

            /* the children: five places for an odd count, four for an even one */
            children[0].y = 0x122;
            if (nChildren != 0) {
                if (nChildren & 1) {
                    oddX[0] = half - w / 2;
                    oddX[2] = oddX[0] + w + 0x28;
                    oddX[1] = oddX[0] - w - 0x28;
                    oddX[3] = oddX[1] - w - 0x28;
                    oddX[4] = oddX[2] + w + 0x28;
                    for (i = 0; i < 5; i++) {
                        if (children[i].id == 0xffff)
                            break;
                        children[i].ctrl = d2_WinOpenPic(oddX[i], 0x122, 0x5db, d2_LastWindow);
                        children[i].x = w / 2 + oddX[i];
                        children[i].y = 0x122;
                        d2_WinOpenPic(oddX[i] + 2, 0x124, ((AICHARREC *)ai_Chars)[children[i].id].picture, d2_LastWindow);
                        d2_Objs[children[i].ctrl].style = 3;
                        d2_Objs[children[i].ctrl].user = ai_Chars[children[i].id];
                    }
                } else {
                    evenX[1] = half + 0x14;
                    evenX[0] = half - w - 0x14;
                    evenX[2] = evenX[0] - w - 0x28;
                    evenX[3] = evenX[1] + w + 0x28;
                    for (i = 0; i < 4; i++) {
                        if (children[i].id == 0xffff)
                            break;
                        children[i].ctrl = d2_WinOpenPic(evenX[i], 0x122, 0x5db, d2_LastWindow);
                        children[i].x = w / 2 + evenX[i];
                        children[i].y = 0x122;
                        d2_WinOpenPic(evenX[i] + 2, 0x124, ((AICHARREC *)ai_Chars)[children[i].id].picture, d2_LastWindow);
                        d2_Objs[children[i].ctrl].style = 3;
                        d2_Objs[children[i].ctrl].user = ai_Chars[children[i].id];
                    }
                }
            }

            /* the lines from the character (and its partner) down to the children */
            lo = row[2].x;
            hi = row[2].x;
            if (partner) {
                hi = row[3].x;
                mid = (children[0].y - row[2].y - h) / 2 + row[2].y + h;
                for (i = 2; i < 4; i++)
                    d2_PlotLine(row[i].x, row[i].y + h + 5, row[i].x, mid, 0x10, 0x10, 0x10);
                d2_PlotLine(lo, mid, hi + 1, mid, 0x10, 0x10, 0x10);
            }
            for (i = 0; i < 5; i++) {
                if (children[i].id != 0xffff) {
                    x = children[i].x;
                    d2_PlotLine(x, children[i].y, x, (children[0].y - row[2].y - h) / 2 + row[2].y + h, 0x10, 0x10, 0x10);
                    if (x < lo)
                        lo = x;
                    if (x > hi)
                        hi = x;
                }
            }
            if (nChildren != 0) {
                mid = (children[0].y - row[2].y - h) / 2 + row[2].y + h;
                d2_PlotLine(lo, mid, hi + 1, mid, 0x10, 0x10, 0x10);
                if (!partner)
                    d2_PlotLine(row[2].x, row[2].y + h + 5, row[2].x, mid, 0x10, 0x10, 0x10);
            }

            /* the names, the clickable boxes and the mark of the character itself */
            for (i = 0; i < 6; i++) {
                if (row[i].id != 0xffff) {
                    d2_Objs[row[i].ctrl].interactive = (flags & 1) && (flags & 0xc) == 0 || (flags & 0x10) ? 1 : 0;
                    txt_Format(rowText, "%1N7", row[i].id);
                    ai_CentredText(rowText, row[i].x, row[i].y + h + 3, 0x50, 0x56);
                    if (row[i].id == ch->id)
                        d2_WinOpenPic(row[i].x - w / 2 - 10, row[i].y - 10, ch->ref54, d2_LastWindow);
                }
            }
            for (i = 0; i < 2; i++) {
                if (parents[i].id != 0xffff) {
                    d2_Objs[parents[i].ctrl].interactive = (flags & 4) == 0 && (flags & 8) == 0 ? 1 : 0;
                    txt_Format(parentText, "%1N7", parents[i].id);
                    ai_CentredText(parentText, parents[i].x, parents[i].y + h + 3, 0x50, 0x56);
                    if (parents[i].id == ch->id)
                        d2_WinOpenPic(parents[i].x - w / 2 - 10, parents[i].y - 10, ch->ref54, d2_LastWindow);
                }
            }
            for (i = 0; i < 5; i++) {
                if (children[i].id != 0xffff) {
                    d2_Objs[children[i].ctrl].interactive = (flags & 1) && (flags & 0xc) == 0
                        || (flags & 2) && (flags & 8) && ((AICHARREC *)ai_Chars)[children[i].id].w00a > 10 ? 1 : 0;
                    txt_Format(childText, "%1N7", children[i].id);
                    ai_CentredText(childText, children[i].x, children[i].y + h + 3, 0x50, 0x56);
                    if (children[i].id == ch->id)
                        d2_WinOpenPic(children[i].x - w / 2 - 10, children[i].y - 10, ch->ref54, d2_LastWindow);
                }
            }
        }
        if (dm_L.rup != 0 && (flags & 8) == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            found = -1;
            pick = 0;
            for (i = 0; i < 6; i++) {
                if (d2_PointerObj == row[i].ctrl) {
                    found = i;
                    pick = (AICHARREC *)gm_FindChar(((AICHARREC *)ai_Chars)[row[i].id].netId);
                    break;
                }
            }
            for (i = 0; i < 2 && found == -1; i++) {
                if (d2_PointerObj == parents[i].ctrl) {
                    found = i;
                    pick = (AICHARREC *)gm_FindChar(((AICHARREC *)ai_Chars)[parents[i].id].netId);
                    break;
                }
            }
            for (i = 0; i < 5 && found == -1; i++) {
                if (d2_PointerObj == children[i].ctrl) {
                    pick = (AICHARREC *)gm_FindChar(((AICHARREC *)ai_Chars)[children[i].id].netId);
                    break;
                }
            }
            if (pick != 0) {
                d2_SetViewEnabled(win, 0);
                if (flags & 0x10)
                    ai_CharInfoPanel(pick, 0);
                else if (flags & 8) {
                    if (ch != pick)
                        ai_CharInfoPanel(pick, 1);
                    else
                        ai_CharInfoPanel(pick, 0);
                }
                d2_SetViewEnabled(win, 1);
            } else {
                draw = 1;
            }
        }
    } while (sv_RunFrame(ai_FamilyTreeWindow, callId) != 0);
    d2_CloseView(win);
    ai_CampaignStep(6, 0, 1, 0);
}

int gm_GetGoodsValue();
void ai_ShowMissions(int owner);

/* 0x5c5310, the character info panel "panel\info_spieler" of the character chr (the family tree window opens it):
   page 1 the name (text 0x1f53), page 2 the birth date ("*d.m.y"), the death date ("+d.m.y") or the age, the
   profession or else the title, the offices (the office and the second office, "-" for none), the worth ("-" for a
   dead one), the partner (married, engaged by a pair event 0x41, widowed, or single 0x1f5d), the faith (its name
   changes in the year 0x75) and the children (relatives[3..7]); page 3 the five skill bars and one line per
   ailment field. other set: the take-over button (text 0x1f50: after the confirmation 0x1f51 the player takes over
   the character and the panel closes); else for the player's own child the gift button (unless the player's
   powerFlags has 0x20) and with it the profession choice for a jobless child (no event 0x5e and f020 at most the
   age + 4) or the university for one with a profession that has not studied (powerFlags 0x10000), and for the
   player's own character the missions button when it has missions. The right button closes it.
   @deep s48: proven byte under a declaration set (5 unused typedefs at the unit's start;
   ledger/tiebreak.tsv): the text is done, the unit's declaration set is the open part. Levers:
   professionBtn / universityBtn as scalars with a block-local int btns[2] per sv_ButtonRow arm; row = 0
   before d2_SetForm(win, 3); the skill loop's row++; y += 0x18; before the txt_Format and each header /
   ailment line's row++ after the .centred store; the job / title text base as a local set by if / else (if
   (chr->sex) base = 0x18f; else base = 0x130;) - a variable with two definitions is no forward-substituted
   ternary, so VC6 adds into the jobId register (add eax,ecx) as the original; ~70 ternary spellings never
   did. prior: Reconstructed in full against the original (all 999 instructions read: the dates, profession
   / title, offices, worth, partner / betrothal event 0x41 walk, faith, children, the skill bars and the
   nine ailment rows, the button ladder of page 4 and the click dispatch); all calls, literals and
   references agree, no M8/M9. Remaining: the original does not strength-reduce or constant-fold the row
   counter of page 3 - in the skill loop it keeps row in esi (inc esi; lea eax,[esi+esi*2]; lea
   ecx,[eax*8+1]) and computes the header / ailment rows at run time (lea ecx,[eax*8+0xa]; inc esi), where
   ours carries row*0x18+1 as a stepped counter and folds the later rows to constants (mov esi,7 / push
   0x9a / push 0xb2): tried ++row in the call, (row+1)*0x18+1 with a trailing row++, both keep the
   reduction (8 instructions fewer: ours 991 vs 999); the original keeps universityBtn[0] in ebp (cmp
   eax,ebp) where ours reloads [esp+0x24]; the rest is order / regs / slots; proof stops at the escaped
   frame layout (gift / professionBtn / universityBtn slots). */
void ai_CharInfoPanel(AICHARREC *chr, int other)
{
    int win;
    int y;
    int giftBtn;
    int professionBtn;
    int i;
    int dead;
    int width;
    int takeOverBtn;
    int universityBtn;
    int missionBtn;
    GMDATE date;
    char text[64];
    int callId;
    char text1[512];
    char text2[512];
    AICHARREC *rel;
    AICHARREC *betrothed;
    char *ev;
    int officeBase;
    int any;
    int row;
    int obj;
    int base;

    dead = 0;
    takeOverBtn = -1;
    missionBtn = -1;
    giftBtn = -1;
    universityBtn = -1;
    professionBtn = -1;
    callId = sv_FrameMask;
    win = d2_OpenForm(0, 0, "panel\\info_spieler");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f53, chr->id);

    /* page 2: the dates, the profession, the offices, the worth, the partner, the faith and the children */
    d2_SetForm(win, 2);
    gm_BirthDate(chr, &date);
    txt_Format(text, "*%i.%i.%i", date.day, date.month, date.year);
    io_PrintText("$3>$L%s$A$0>", text);
    if (gm_DeathDate(chr, &date) != 0) {
        txt_Format(text, "+%i.%i.%i", date.day, date.month, date.year);
        io_PrintText("$3>$L%s$A$0>", text);
        dead = 1;
    } else {
        io_PrintText("$5<$R%s$0<$3>$L%i$A$0>", 0x1f54, (int)gm_CharAge(chr->id));
    }
    if (chr->jobId != 0) {
        if (chr->sex)
            base = 0x18f;
        else
            base = 0x130;
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f55, chr->jobId + base);
    } else if (chr->title != 0) {
        if (chr->sex)
            base = 0x230;
        else
            base = 0x20d;
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f56, chr->title + base);
    }
    officeBase = chr->sex ? 0x27b : 0x253;
    if (chr->officeKind == 0 && chr->secondOffice == 0) {
        txt_Format(text, "%s", "-");
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f57, text);
    }
    if (chr->officeKind != 0 && chr->secondOffice != 0)
        io_PrintText("$5<$R%s$0<$3>$L%s$A%s$A$0>", 0x1f57, chr->officeKind + officeBase, chr->secondOffice + officeBase);
    else if (chr->officeKind != 0)
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f57, chr->officeKind + officeBase);
    else if (chr->secondOffice != 0)
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f57, chr->secondOffice + officeBase);
    if (!dead) {
        io_PrintText("$5<$R%s$0<$3>$L%T$A$0>", 0x1f58, gm_GetWorth(chr->id));
    } else {
        gm_GetWorth(chr->id);
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f58, "-");
    }
    rel = (AICHARREC *)gm_FindChar(chr->relatives[0]);
    if (dead) {
        strcpy(text, "-");
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", txt_Ptrs[0x1f59], text);
    } else if (rel != 0 && rel->health > 0) {
        txt_Format(text, 0x1f5f, rel->id);
        io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f59, text);
    } else {
        ev = he_FindEvent(1, 0, 0x41);
        while (ev != 0 && ((AIJOB *)ev)->partner1 != chr->netId)
            ev = he_NextEvent();
        if (ev != 0 && (betrothed = (AICHARREC *)gm_FindChar(((AIJOB *)ev)->partner2)) != 0) {
            txt_Format(text, 0x1f5e, betrothed->id);
            io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f59, text);
        } else if (rel != 0) {
            txt_Format(text, 0x1f60, rel->id);
            io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f59, text);
        } else {
            io_PrintText("$5<$R%s$0<$3>$L%s$A$0>", 0x1f59, 0x1f5d);
        }
    }
    io_PrintText("$5<$R%s $0<$3>$L%s$A$0>", 0x488, (ai_Time.year < 0x75 ? 0x486 : 0x483) + chr->faith);
    io_PrintText("$5<$R%s$0<", 0x1f5a);
    any = 0;
    for (i = 3; i < 8; i++) {
        rel = (AICHARREC *)gm_FindChar(chr->relatives[i]);
        if (rel != 0) {
            any = 1;
            io_PrintText("$3>$L%1N1$A$0>", rel->id);
        }
    }
    if (!any)
        io_PrintText("$3>$L-$A$0>");

    /* page 3: the skills and the ailments */
    row = 0;
    d2_SetForm(win, 3);
    width = d2_Wins[d2_LastWindow].w;
    txt_Format(text, 0x1f61);
    obj = d2_CreateWinText(0, 1, text, d2_LastWindow);
    y = 2;
    d2_Objs[obj].centred = 1;
    d2_Objs[obj].w = width;
    for (i = 0; i < 5; i++) {
        row++;
        y += 0x18;
        txt_Format(text, 0x16b7 + i);
        d2_CreateWinText(10, row * 0x18 + 1, text, d2_LastWindow);
        sv_DrawBalanceBar(0xaf, y, chr, i, 0x4e5);
    }
    row++;
    txt_Format(text, 0x1f63);
    obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
    d2_Objs[obj].centred = 1;
    row++;
    d2_Objs[obj].w = width;
    if (chr->ailments != 0) {
        if (chr->ailments & 0xf) {
            txt_Format(text, 0x161e);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0xf0) {
            txt_Format(text, 0x161f);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0xf00) {
            txt_Format(text, 0x1620);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0x3000) {
            txt_Format(text, 0x1621);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0x1c000) {
            txt_Format(text, 0x1622);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0xe0000) {
            txt_Format(text, 0x1623);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 7 << 20) {
            txt_Format(text, 0x1624);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0x1800000) {
            txt_Format(text, 0x1625);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
        if (chr->ailments & 0x1e000000) {
            txt_Format(text, 0x1626);
            obj = d2_CreateWinText(0, row * 0x18 + 10, text, d2_LastWindow);
            d2_Objs[obj].centred = 1;
            row++;
            d2_Objs[obj].w = width;
        }
    }

    /* page 4: the buttons */
    if (other != 0) {
        d2_SetForm(win, 4);
        takeOverBtn = d2_GetWinObj(win, 4, io_PrintText(0x1f50, chr->id));
    } else {
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0 && chr->relatives[1] == ((AICHARREC *)ai_Chars)[ai_PlayerId].netId
            || ((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 1 && chr->relatives[2] == ((AICHARREC *)ai_Chars)[ai_PlayerId].netId) {
            if (chr->jobId == 0) {
                if (he_FindEvent(2, 2, chr->id, 0, 0x5e) == 0 && chr->f020 <= chr->w00a + 4) {
                    if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x20) == 0) {
                        txt_Format(text1, 0x1f4b, chr->id);
                        txt_Format(text2, 0x1a96);
                        d2_SetForm(win, 4);
                        {
                            int btns[2];
                            sv_ButtonRow(d2_LastWindow, 0, btns, 2, text1, text2);
                            giftBtn = btns[0];
                            professionBtn = btns[1];
                        }
                    }
                } else if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x20) == 0) {
                    txt_Format(text1, 0x1f4b, chr->id);
                    d2_SetForm(win, 4);
                    giftBtn = d2_GetWinObj(win, 4, io_PrintText("%ia[%s]", text1));
                }
            } else if ((chr->powerFlags & 0x10000) == 0) {
                if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x20) == 0) {
                    txt_Format(text2, 0x1f4b, chr->id);
                    txt_Format(text1, 0x1a97);
                    d2_SetForm(win, 4);
                    {
                        int btns[2];
                        sv_ButtonRow(d2_LastWindow, 0, btns, 2, text2, text1);
                        giftBtn = btns[0];
                        universityBtn = btns[1];
                    }
                }
            } else if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x20) == 0) {
                txt_Format(text1, 0x1f4b, chr->id);
                d2_SetForm(win, 4);
                giftBtn = d2_GetWinObj(win, 4, io_PrintText("%ia[%s]", text1));
            }
        } else if (((AICHARREC *)ai_Chars)[ai_PlayerId].netId == chr->netId && ai_HasMission(chr->netId)) {
            d2_SetForm(win, 4);
            missionBtn = d2_GetWinObj(win, 4, io_PrintText(0x1f49));
        }
    }

    d2_ClickedId = -1;
    d2_PointerObj = -1;
    do {
        if (giftBtn != -1) {
            if (gm_GetGoodsValue(ai_Chars[ai_PlayerId]) <= 0)
                d2_SetObjEnabled(giftBtn, 0);
            else
                d2_SetObjEnabled(giftBtn, 1);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            d2_SetViewEnabled(win, 0);
            if (d2_PointerObj == takeOverBtn) {
                txt_Format(text2, 0x1f51, chr->id, chr->id);
                if (sv_MessageBox(text2, 1) != 0) {
                    cm_SendTakeOverCharacter(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, chr->netId);
                    sv_PopRequest = 2;
                }
            } else if (d2_PointerObj == giftBtn) {
                if (ai_GiftWindow(ai_Chars[ai_PlayerId], chr) != 0)
                    d2_SetObjEnabled(giftBtn, 0);
            } else if (d2_PointerObj == professionBtn) {
                if (ai_ChooseProfessionWindow(ai_Chars[ai_PlayerId], chr) != 0)
                    d2_SetObjEnabled(professionBtn, 0);
            } else if (d2_PointerObj == universityBtn) {
                if (ai_UniversityWindow(ai_Chars[ai_PlayerId], chr) != 0)
                    d2_SetObjEnabled(universityBtn, 0);
            } else if (d2_PointerObj == missionBtn) {
                ai_ShowMissions(chr->netId);
            }
            d2_SetViewEnabled(win, 1);
        }
    } while (sv_RunFrame(ai_CharInfoPanel, callId) != 0);
    d2_CloseView(win);
}
/* 0x5c6050, the gift window "misc\geschenck" (1133 B): the player (giver, the player's character record) gives chr
   money. The least gift is 0.5 % of the player's worth (gm_GetWorth), at least 0x640, the most 5 % of the worth or
   the player's money (gm_GetGoodsValue), whichever is less, at least 0xc80 (none: returns 0). A giver whose power
   bit 0x20 (AICHARREC.powerFlags, a gift given this round) is set sees the text 0x1f4f only; else the caption
   0x1f4c, the amount slider (text 0x1f4d with chr's name, in the player's money) and the buttons of the text 0x1f4e
   (give, close). Give: when the player has the amount (sv_CheckGoodAmount) the group "gift" moves it from the giver
   to chr and sets the giver's bit 0x20; the window closes and it returns 1. A right click or close returns 0 */

int ai_GiftWindow(AICHARREC *giver, AICHARREC *chr)
{
    int slider;
    int least;
    int giveBtn;
    int closeBtn;
    int most;
    int win;
    int given;
    int used;
    int text;
    int amount;

    giveBtn = -1;
    closeBtn = -1;
    slider = -1;
    least = max(0x640, (int)(gm_GetWorth(lc_CharRecs[ai_PlayerId].id) * 0.005f));
    most = max(0xc80, (int)min((float)(gm_GetWorth(lc_CharRecs[ai_PlayerId].id) * 0.05f),
                                (float)gm_GetGoodsValue((char *)&lc_CharRecs[ai_PlayerId])));
    if (most <= 0)
        return 0;
    used = (giver->powerFlags >> 5) & 1;
    win = d2_OpenForm(0, 0, "misc\\geschenck");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f4c);
    if (used) {
        io_PrintText(0x1f4f);
    } else {
        slider = d2_GetWinObj(win, 1, io_PrintText(0x1f4d, chr->id, ai_OwnPlayer));
        text = io_PrintText(0x1f4e);
        giveBtn = d2_GetWinObj(win, 1, text);
        closeBtn = d2_GetWinObj(win, 1, text + 1);
        d2_SetObjValue4(slider, gm_ConvertMoney(least, ai_OwnPlayer), gm_ConvertMoney(most, ai_OwnPlayer),
                       gm_ConvertMoney(least, ai_OwnPlayer));
    }
    given = 0;
    d2_ClickedId = -1;
    do {
        if (dm_L.rup)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && !used) {
            if (d2_PointerObj == giveBtn) {
                amount = gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer);
                if (sv_CheckGoodAmountB((char *)&lc_CharRecs[ai_PlayerId], amount, ai_OwnPlayer)) {
                    unsigned int flags = giver->powerFlags | 0x20;

                    cm_BeginGroup("gift");
                    cm_SendMoveGoods(chr->netId, giver->netId, ai_OwnPlayer, amount);
                    cm_BeginFieldBlock(giver, giver->netId);
                    cm_AddFieldDelta(4, 1, 0x1c8, &flags);
                    cm_SendFieldDeltas();
                    cm_EndGroup("gift");
                }
                given = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == closeBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GiftWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return given;
}

struct AIPER;
int gm_CharLevel(unsigned short id, int again); int ord_DispatchOrder();
unsigned int ai_FindCandidates(struct AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out);

#define AI_CANDIDATE_FLAGS_SEX0 0xc0540a    /* the candidate flags of the kind 9 (advertising) search for a character with sex 0 (the mask bits of ai_PassesPartnerFilter) */
#define AI_CANDIDATE_FLAGS_SEX1 0xc05409    /* ditto for sex != 0 */
/* 0x5c64c0, collects the candidate net ids of a kind 9 search into out[] and their count into *n: up to 10
   candidates of character p (ai_FindCandidates, the filter 0xc0540a, one less for a female character, sex +9);
   the ids are the characters' net ids, ai_CharInts[id][0] (the caller finds them again with gm_FindChar) */

void ai_FindCandidateIds(G8CHARREC *p, int *n, int *out)
{
    unsigned short ids[10];
    int i;

    unsigned int fl = p->sex != 0 ? AI_CANDIDATE_FLAGS_SEX1 : AI_CANDIDATE_FLAGS_SEX0;

    *n = ai_FindCandidates((struct AIPER *)p, 9, fl, 0.0f, 100.0f, ids);
    for (i = 0; i < *n; i++)
        out[i] = ai_CharInts[ids[i]][0];
}
/* 0x5c6530, the marriage test of two characters, idA / idB their net ids: gm_FindChar resolves both;
   the codes 0xfffffc00 (no record), 0xfffffbff / 0xfffffbfe (health 0), 0xfffffbfd / 0xfffffbfc (the partner
   link +0x5c is another net id), 0xfffffbfb / 0xfffffbfa (held); the level difference (gm_CharLevel) under
   6 -> 1 */

unsigned int ai_MarriageTest(int idA, int idB)
{
    G8CHARREC *a;
    G8CHARREC *b;
    int i;
    int j;

    a = (G8CHARREC *)gm_FindChar(idA);
    b = (G8CHARREC *)gm_FindChar(idB);
    if (a == 0 || b == 0)
        return 0xfffffc00;
    if (a->health <= 0)
        return 0xfffffbff;
    if (b->health <= 0)
        return 0xfffffbfe;
    if (b->partner != -1 && b->partner != idA)
        return 0xfffffbfd;
    i = a->partner;
    if (i != -1 && i != idB)
        return 0xfffffbfc;
    if (a->heldRounds > 0)
        return 0xfffffbfb;
    if (b->heldRounds > 0)
        return 0xfffffbfa;
    i = gm_CharLevel(a->id, 0);
    j = gm_CharLevel(b->id, 0);
    return (unsigned int)(abs(i - j) < 6);
}
/* 0x5c6620, the courtship rounds the suitor idA needs for idB (742 B; the netIds, 0 when either is not found):
   idB's standing is the larger of idB's worth (gm_GetWorth, at least 0) and half the worth of either parent
   (AICHARREC.relatives[1] / [2]); a suitor worth less than that pays (standing - worth) / standing * 5 + 1, else 1;
   to that come (100 - the liking of idA for idB) * 0.05 + 1, the age of idB clamped to 15 .. 30 as (1 - age / 15) * 4
   + 1, and idB's five skills / 252 each. An order handler 0x10 of the suitor (ord_DispatchOrder) with the bit 2 set
   halves it; the result is clamped to 2 .. 15 */

int ai_CourtshipRounds(int idA, int idB)
{
    float parentB;
    float standing;
    float parentA;
    int req[0x88];
    AICHARREC *a;
    AICHARREC *b;
    AICHARREC *p;
    int worth;
    float rate;
    float liking;
    float score;
    int i;

    parentA = parentB = 0.0f;
    memset(req, 0, sizeof(req));
    req[0] = 1;
    a = (AICHARREC *)gm_FindChar(idA);
    b = (AICHARREC *)gm_FindChar(idB);
    if (a == 0 || b == 0)
        return 0;
    p = (AICHARREC *)gm_FindChar(b->relatives[1]);
    if (p != 0)
        parentA = (float)(gm_GetWorth(p->id) / 2);
    p = (AICHARREC *)gm_FindChar(b->relatives[2]);
    if (p != 0)
        parentB = (float)(gm_GetWorth(p->id) / 2);
    standing = max((float)gm_GetWorth(b->id), 0.0f);
    standing = max(standing, max(parentA, parentB));
    worth = gm_GetWorth(a->id);
    if ((float)worth > standing)
        rate = 1.0f;
    else
        rate = (standing - (float)worth) / standing * 5.0f + 1.0f;
    liking = (100.0f - gm_Liking(a->id, b->id, 1)) * 0.05f + 1.0f;
    score = (1.0f - min(30.0f, max(gm_CharAge(b->id), 15.0f)) * 0.06666667f) * 4.0f + 1.0f;
    score += liking;
    score += rate;
    for (i = 0; i < 5; i++)
        score += b->skills[i] * 0.003968254f;
    if (ord_DispatchOrder(0x10, (char *)a, req) & 2)
        score *= 0.5f;
    score = min(max(score, 2.0f), 15.0f);
    return (int)score;
}

void d2_EnableViewObjects(); void gm_AddTime(struct AITIME *c, int hours, int mins, int secs); 
int cm_GetSlotState();

/* 0x5c6910, the confirm dialog of the courtship (advertising) list 0x5c6aa0: the rating 0x5c6620 of the own
   character (ai_CharInts[ai_PlayerId][0]) and the partner ch->netId; the window "privillegien\werbung2"
   "privillegien\werbung2" with the text 0x1ce5 (ch->id, the rating); the frame loop 0x5309a0 (this, 0x657c7): a click 0
   sends the marriage proposal message 0x41 (the own netId as to / fromId / arg[0], the partner's as arg[1], the
   clock, flags 2) through 0x4c4b70 and waits for its slot (0x4c3ae0 / 0x52fe00), the result 1; click 1 only
   pops; the exit 0x4758f0, d2_ClickedId = -1 and di_ClearEvents(0x11) */

int ai_CourtshipConfirm(AICHARREC *ch)
{
    AIMSG msg;
    int win;
    int v;
    int slot;

    v = ai_CourtshipRounds(ai_CharInts[ai_PlayerId][0], ch->netId);
    win = d2_OpenForm(0, 0, "privillegien\\werbung2");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1ce5, ch->id, v);
    d2_ClickedId = -1;
    v = 0;
    for (;;) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x41;
                msg.flags = 2;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.time = ai_Time;
                msg.arg[1] = ch->netId;
                msg.fromId = ai_CharInts[ai_PlayerId][0];
                msg.arg[0] = ai_CharInts[ai_PlayerId][0];
                slot = cm_SendLongMessage((char *)&msg);
                while (cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                v = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
            }
        }
        if (sv_RunFrame(ai_CourtshipConfirm, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    d2_ClickedId = -1;
    di_ClearEvents(0x11);
    return v;
}
typedef struct LCORDMSG {          /* the message record of an order (gv.h ORDMSG, which this unit cannot include): the
                                      order handlers get it with the character (ord_DispatchOrder) */
    int kind;                      /* 0x000, 1 do / 2 answer / 3 ask / 4 answer */
    union {
        int law;                   /* 0x004, of the law orders: the decision record (AIDEC.id) the office may change
                                      (gv.h ORDMSG.law) */
        int amount;                /* 0x004, of the bewitch order asked of the AI: what it pays, 1 .. 5 (ai_BewitchOrder);
                                      of the penance order: the crimes it buys off, 1 .. 9 (ai_PenanceWindow) */
    };
    int value;                     /* 0x008, of the law orders asked of the AI: the value it wants (ai_LawPrivilegeWindow) */
    char pad00c[8];
    char text[0x200];              /* 0x014, the text the handlers fill */
    union {
        int target;                /* 0x214, of an ask that names a character (the courtship): its netId (gv.h
                                      ORDMSG.target) */
        int answerFlag;            /* 0x214, of the law orders: which of the office's decisions, 1 / 2 (gv.h
                                      ORDMSG.answerFlag) */
    };
    int target2;                   /* 0x218, of the hatred order asked of the AI: the second character's netId
                                      (ai_HatredWindow; the first is target) */
    struct AICHARREC *partner;     /* 0x21c, the partner character (gv.h ORDMSG.partner; the privileges panel sets it
                                      to the player's partner of the pair event 0x45) */
} LCORDMSG;
/* 0x5c6aa0, the courtship list "privillegien\werbung1" (604 B): the caption 0x1ce3 and up to nine candidates of ch
   (ai_FindCandidateIds) as amount rows in a grid of three columns spread over the list part's width (the frame
   picture 0x5da), each with the text 0x1ce4 of the courtship rounds the player needs for them (ai_CourtshipRounds)
   below it. A click on a row opens the confirm ai_CourtshipConfirm (the list's objects disabled meanwhile); a
   confirmed courtship closes the list and returns 0x92, a right click returns 2 */

int ai_CourtshipListWindow(G8CHARREC *ch, LCORDMSG *m)
{
    int n;
    int result;
    int win;
    int margin;
    int ids[9];
    char text[128];
    LCAMOUNTROW rows[9];
    int gap;
    int i;

    result = 2;
    ai_FindCandidateIds(ch, &n, ids);
    win = d2_OpenForm(0, 0, "privillegien\\werbung1");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1ce3);
    d2_SetForm(win, 2);
    gap = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 3) / 4;
    margin = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 3) % 4 / 2;
    for (i = 0; i < n; i++) {
        rows[i].x = margin + (d2_Pics[0x5da].w + 10) * (i % 3) + (i % 3 + 1) * gap - 10;
        rows[i].y = i / 3 * 0x82 + 5;
        rows[i].rec = (AICHARREC *)gm_FindChar(ids[i]);
        rows[i].pickable = 1;
        ai_AmountRowStatus(&rows[i]);
        rows[i].x2c = ai_CourtshipRounds(lc_CharRecs[ai_PlayerId].netId, rows[i].rec->netId);
        txt_Format(text, 0x1ce4, rows[i].x2c);
        ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x56, &rows[i], 1);
        ai_CentredText(text, rows[i].x + d2_Pics[0x5da].w / 2, rows[i].y + d2_Pics[0x5da].h + 0x2b, 0xca, 0x56);
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1 && dm_L.lclick) {
            for (i = 0; i < n; i++) {
                if (rows[i].ctl[0] == d2_PointerObj) {
                    d2_EnableViewObjects(win, 0);
                    if (ai_CourtshipConfirm(rows[i].rec) != 0) {
                        result = 0x92;
                        sv_PopRequest = 1;
                        break;
                    }
                    d2_EnableViewObjects(win, 1);
                }
            }
        }
    } while (sv_RunFrame((int)ai_CourtshipListWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return result;
}
/* 0x5c6d00, the courtship order's ask (ord.c ord_ApplyForOffice, kind 3; 1479 B): ch the asking character, m the order
   message. ch's courtship event 0x41 (he_FindEvent, AIJOB.partner1 = ch) gives the state: none 1, running 2, done
   (pairDone) 3, a round to pay (pairDrawn) 4. A character that is no player (kind != 6) without a courtship posts
   the courtship message 0x41 for m->target when that character exists and lives (returns 0x10); else it returns 0x20.
   A player without one gets the candidate list (ai_CourtshipListWindow); else the window "privillegien\werbung2": the
   caption 0x1cf3 and 0x1ce7 (ch, the partner, the rounds left) while it runs, 0x1ce3 / 0x1d31 (the partner) once done,
   and in a round to pay 0x1cf4 (the partner, the rounds left) and the three gift buttons 0x1cf5 .. 0x1cf7 (the gift
   text 0x1d11 + pairPick, pairPrice) with the close button 0x1cf8. A gift the player can pay counts gain rounds,
   (pick - the partner's b00d) / 9 * 3 + 0.5 clamped to 0 .. 3 (a round due when the rounds are reached: an hour on),
   posts the event again (cm_SendExSetHE), pays pairPrice (cm_SendMoveGoods) and tells the partner's answer 0x1cf9
   (text 0x1d02 + a random pick by the gain); the close button tells 0x1cfa and posts the event. Returns 2 */

int ai_CourtshipWindow(AICHARREC *ch, LCORDMSG *m)
{
    int state;
    int partner;
    int win;
    int level;
    int price;
    int left;
    int picks[3];
    int prices[3];
    int btns[9];               /* the gift buttons and the close button in [0 .. 3]; the original's frame holds nine */
    AIJOB *ev;
    AICHARREC *p;
    char answer[0x200];
    char closing[0x200];
    int i;
    int gain;
    char rounds;
    int r;

    state = 1;
    level = 0;
    partner = -1;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x41); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->partner1 == ch->netId) {
            state = 2;
            partner = ev->partner2;
            if (ev->pairDone == 1)
                state = 3;
            if (ev->pairDrawn == 1)
                state = 4;
            break;
        }
    }
    if (ch->kind != 6) {
        if (ev != 0)
            return 0x20;
        p = (AICHARREC *)gm_FindChar(m->target);
        if (p != 0 && p->health != 0) {
            AIMSG msg;

            memset(&msg, 0, sizeof(msg));
            msg.type = 0x41;
            msg.flags = 2;
            msg.to = ch->netId;
            msg.fromId = ch->netId;
            msg.time = ai_Time;
            msg.arg[0] = ch->netId;
            msg.arg[1] = p->netId;
            cm_SendLongMessage((char *)&msg);
            return 0x10;
        }
        return 0x20;
    }
    if (state == 1)
        return ai_CourtshipListWindow((G8CHARREC *)ch, m);
    win = d2_OpenForm(0, 0, "privillegien\\werbung2");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1cf3);
    if (state == 2) {
        p = (AICHARREC *)gm_FindChar(partner);
        io_PrintText(0x1ce7, ch->id, p->id, max(0, ev->pairRounds - ev->pairRound), 0x5d);
    } else if (state == 3) {
        p = (AICHARREC *)gm_FindChar(partner);
        io_PrintText(0x1ce3);
        io_PrintText(0x1d31, p->id);
    } else if (state == 4) {
        p = (AICHARREC *)gm_FindChar(partner);
        level = p->b00d;
        for (i = 0; i < 3; i++) {
            prices[i] = ev->pairPrice[i];
            picks[i] = ev->pairPick[i];
        }
        io_PrintText(0x1cf4, p->id, 0x5d, ev->pairRounds - ev->pairRound, 0x5d);
        btns[0] = d2_GetWinObj(win, 1, io_PrintText(0x1cf5, picks[0] + 0x1d11, prices[0]));
        btns[1] = d2_GetWinObj(win, 1, io_PrintText(0x1cf6, picks[1] + 0x1d11, prices[1]));
        btns[2] = d2_GetWinObj(win, 1, io_PrintText(0x1cf7, picks[2] + 0x1d11, prices[2]));
        btns[3] = d2_GetWinObj(win, 1, io_PrintText(0x1cf8));
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && state == 4) {
            for (i = 0; i < 4; i++) {
                if (d2_PointerObj == btns[i])
                    break;
            }
            if (i < 3) {
                price = prices[i];
                if (sv_CheckGoodAmountB((char *)&lc_CharRecs[ai_PlayerId], price, ai_OwnPlayer)) {
                    rounds = 0;
                    p = (AICHARREC *)gm_FindChar(partner);
                    left = picks[i] - level;
                    gain = (int)(max(left * 0.11111111f, 0.0f) * 3.0f + 0.5f);
                    if (gain > 0)
                        rounds = min(gain, 3);
                    if (gain <= 0)
                        r = util_Random(5) & 0xffff;
                    else if (gain == 1)
                        r = (util_Random(5) & 0xffff) + 5;
                    else if (gain == 2)
                        r = (util_Random(4) & 0xffff) + 9;
                    else
                        r = (util_Random(3) & 0xffff) + 0xc;
                    txt_Format(answer, 0x1cf9, p->id, r + 0x1d02);
                    ev->pairRound += rounds;
                    if (ev->pairRound >= ev->pairRounds) {
                        ev->end = ai_Time;
                        gm_AddTime(&ev->end, 0, 1, 0);
                    }
                    ev->pairDrawn = 0;
                    ev->exSlot = cm_SendExSetHEI(0, ev);
                    cm_SendMoveGoods(-1, ch->netId, ai_OwnPlayer, price);
                    sv_MessageBox(answer, 0);
                    sv_PopRequest = 1;
                }
            } else if (i == 3) {
                txt_Format(closing, 0x1cfa, ((AICHARREC *)gm_FindChar(partner))->id);
                ev->pairDrawn = 0;
                ev->exSlot = cm_SendExSetHEI(0, ev);
                sv_MessageBox(closing, 0);
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_CourtshipWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return 2;
}
/* 0x5c72d0, the rating chart of the statistics window (ai_DrawStatisticsCharts, its flag 1; 393 B): the top value is the
   first of the thresholds 500 .. 20000 above the highest of the rounds' rating bases (ord_StatisticsHistory[].base)
   and the running ai_RatingBase (else that value); the rounds' bases are drawn as grey (0x33) line segments, scale
   = (width - 0x69) / 16 apart from x 0x2d, y from height - 0x16 up by (height - 0x1e) / top per unit, the last one
   to the running ai_RatingBase. The current statistics stats (by value) are not read. Returns the top value */

int ai_PlotRatingBaseChart(ORDSTATS stats, int width, int height)
{
    float bottom;
    int limits[9];
    int scale;
    int top;
    int x;
    int i;
    float unit;

    scale = (width - 0x69) / 16;
    top = -1;
    limits[0] = 500;
    limits[1] = 1000;
    limits[2] = 2000;
    limits[3] = 4000;
    limits[4] = 8000;
    limits[5] = 10000;
    limits[6] = 15000;
    limits[7] = 20000;
    limits[8] = 50000;
    for (i = 0; i < 16; i++) {
        if (ord_StatisticsHistory[i].base > top)
            top = ord_StatisticsHistory[i].base;
    }
    top = (int)max(ai_RatingBase, (float)top);
    for (i = 0; i < 8; i++) {
        if (top < limits[i]) {
            top = limits[i];
            break;
        }
    }
    unit = (float)(height - 0x1e) / top;
    bottom = (float)(height - 0x16);
    x = 0x2d;
    for (i = 0; i < 15; i++) {
        d2_PlotLine(x, (int)(bottom - ord_StatisticsHistory[i].base * unit), x + scale,
                    (int)(bottom - ord_StatisticsHistory[i + 1].base * unit), 0x33, 0x33, 0x33);
        x += scale;
    }
    d2_PlotLine(0x2d + scale * 15, (int)(bottom - ord_StatisticsHistory[15].base * unit), 0x2d + scale * 16,
                (int)(bottom - ai_RatingBase * unit), 0x33, 0x33, 0x33);
    return top;
}
/* 0x5c7460, the score chart of the statistics window (ai_DrawStatisticsCharts, its flag 2; 255 B): the rounds' overall scores
   (ord_StatisticsHistory[].score, 0 .. 1) as green (0x10, 0xee, 0x10) line segments, scale = (width - 0x69) / 16
   apart from x 0x2d, y from height - 0x20 + 10 up by height - 0x1e for a score of 1, the last one to the current
   score stats.score. The highest score is looked for and not used */

void ai_PlotScoreChart(ORDSTATS stats, int width, int height)
{
    float unit;
    float bottom;
    float top;
    int scale;
    int x;
    int i;
    float from;
    float to;

    scale = (width - 0x69) / 16;
    top = -1.0f;
    for (i = 0; i < 16; i++) {
        if (top < ord_StatisticsHistory[i].score)
            top = ord_StatisticsHistory[i].score;
    }
    unit = (float)(height - 0x1e);
    bottom = (float)(height - 0x20) + 10.0f;
    x = 0x2d;
    for (i = 0; i < 15; i++) {
        from = ord_StatisticsHistory[i].score;
        to = ord_StatisticsHistory[i + 1].score;
        d2_PlotLine(x, (int)(bottom - from * unit), x + scale, (int)(bottom - to * unit), 0x10, 0xee, 0x10);
        x += scale;
    }
    d2_PlotLine(0x2d + scale * 15, (int)(bottom - ord_StatisticsHistory[15].score * unit), 0x2d + scale * 16,
                (int)(bottom - stats.score * unit), 0x10, 0xee, 0x10);
}
/* 0x5c7560, the first-object chart of the statistics window (ai_DrawStatisticsCharts, its flag 8; 325 B): the rounds' shares
   of characters without the first object (ord_StatisticsHistory[].noFirst, 0 .. 1, below 0 drawn as 0) as yellow
   (0xee, 0xee, 0x10) line segments, scale = (width - 0x69) / 16 apart from x 0x2d, y from height - 0x16 up by
   height - 0x1e for a share of 1, the last one to the current share stats.col[1] */

void ai_PlotNoFirstChart(ORDSTATS stats, int width, int height)
{
    int scale;
    int x;
    int i;
    float bottom;
    float unit;
    float from;
    float to;

    scale = (width - 0x69) / 16;
    unit = (float)(height - 0x1e);
    bottom = (float)(height - 0x16);
    x = 0x2d;
    for (i = 1; i < 16; i++) {
        from = max(ord_StatisticsHistory[i - 1].noFirst, 0.0);
        to = max(ord_StatisticsHistory[i].noFirst, 0.0);
        d2_PlotLine(x, (int)(bottom - from * unit), x + scale, (int)(bottom - to * unit), 0xee, 0xee, 0x10);
        x += scale;
    }
    from = max(ord_StatisticsHistory[15].noFirst, 0.0);
    to = max(stats.col[1], 0.0);
    d2_PlotLine(0x2d + scale * 15, (int)(bottom - from * unit), 0x2d + scale * 16, (int)(bottom - to * unit), 0xee, 0xee,
                0x10);
}
/* 0x5c76b0, the second-object chart of the statistics window (ai_DrawStatisticsCharts, its flag 0x10; 325 B): the rounds'
   shares of characters without the second object (ord_StatisticsHistory[].noSecond, 0 .. 1, below 0 drawn as 0)
   as purple (0xee, 0x10, 0xee) line segments, scale = (width - 0x69) / 16 apart from x 0x2d, y from height - 0x16
   up by height - 0x1e for a share of 1, the last one to the current share stats.col[3] */

void ai_PlotNoSecondChart(ORDSTATS stats, int width, int height)
{
    int scale;
    int x;
    int i;
    float bottom;
    float unit;
    float from;
    float to;

    scale = (width - 0x69) / 16;
    unit = (float)(height - 0x1e);
    bottom = (float)(height - 0x16);
    x = 0x2d;
    for (i = 1; i < 16; i++) {
        from = max(ord_StatisticsHistory[i - 1].noSecond, 0.0);
        to = max(ord_StatisticsHistory[i].noSecond, 0.0);
        d2_PlotLine(x, (int)(bottom - from * unit), x + scale, (int)(bottom - to * unit), 0xee, 0x10, 0xee);
        x += scale;
    }
    from = max(ord_StatisticsHistory[15].noSecond, 0.0);
    to = max(stats.col[3], 0.0);
    d2_PlotLine(0x2d + scale * 15, (int)(bottom - from * unit), 0x2d + scale * 16, (int)(bottom - to * unit), 0xee, 0x10,
                0xee);
}
/* 0x5c7800, the combined rating chart of the statistics window (ai_DrawStatisticsCharts, its flag 4; 356 B): the rounds'
   combined ratings (ord_StatisticsHistory[].combined, 0 .. 1, below 0 drawn as 0) as red (0xee, 0x10, 0x10) line
   segments, scale = (width - 0x69) / 16 apart from x 0x2d, y from height - 0x16 up by height - 0x1e for a rating of
   1, the last one to the current rating stats.col[2]. The highest score is looked for and not used */

void ai_PlotCombinedChart(ORDSTATS stats, int width, int height)
{
    int scale;
    int x;
    int i;
    float top;
    float bottom;
    float unit;
    float from;
    float to;

    scale = (width - 0x69) / 16;
    top = -1.0f;
    for (i = 0; i < 16; i++) {
        if (top < ord_StatisticsHistory[i].score)
            top = ord_StatisticsHistory[i].score;
    }
    unit = (float)(height - 0x1e);
    bottom = (float)(height - 0x16);
    x = 0x2d;
    for (i = 1; i < 16; i++) {
        from = max(ord_StatisticsHistory[i - 1].combined, 0.0);
        to = max(ord_StatisticsHistory[i].combined, 0.0);
        d2_PlotLine(x, (int)(bottom - from * unit), x + scale, (int)(bottom - to * unit), 0xee, 0x10, 0x10);
        x += scale;
    }
    from = max(ord_StatisticsHistory[15].combined, 0.0);
    to = max(stats.col[2], 0.0);
    d2_PlotLine(0x2d + scale * 15, (int)(bottom - from * unit), 0x2d + scale * 16, (int)(bottom - to * unit), 0xee, 0x10,
                0x10);
}

int d2_TextWidth();

/* 0x5c7970, draws the statistics charts into the paintbox window win (1422 B): the flag mask selects the charts
   (1 the rating base, 2 the score, 4 the combined rating, 8 the share without the first object, 0x10 without the
   second), each drawn from the current statistics stats over the window's width - 1 and height; then the axes, the
   ticks and right-aligned percent labels 100% .. 25% of the four rows, the rating scale (the top value the rating
   base chart returned and its half) on the right, and along the bottom one tick per round back from the game clock
   ai_Time, a longer tick and the year at each round in the first month of a year. The pen size is restored */

void ai_DrawStatisticsCharts(unsigned short flags, ORDSTATS stats, int win)
{
    unsigned char old;
    int top;
    int height;
    int width;
    int scale;
    int i;
    int round;
    int pct;
    int y;
    int x;
    int mid;
    int obj;
    int textW;
    float range;
    AITIME clock;
    GMDATE date;
    char buf[16];

    top = 0;
    old = d2_PenSize;
    d2_ClearPaintbox();
    d2_RemoveWindowObjects(win, 1);
    d2_PenSize = 1;
    width = d2_Wins[win].w;
    height = d2_Wins[win].h;
    scale = (width - 0x69) / 16;
    if (flags & 1)
        top = ai_PlotRatingBaseChart(stats, width - 1, height);
    d2_PenSize = 1;
    if (flags & 2)
        ai_PlotScoreChart(stats, width - 1, height);
    if (flags & 4)
        ai_PlotCombinedChart(stats, width - 1, height);
    if (flags & 8)
        ai_PlotNoFirstChart(stats, width - 1, height);
    if (flags & 0x10)
        ai_PlotNoSecondChart(stats, width - 1, height);
    d2_PenSize = 0;
    d2_PlotLine(0x2d, 8, 0x2d, height - 0x13, 0x10, 0x10, 0x10);
    d2_PlotLine(0x2d, height - 0x13, width - 0x3b, height - 0x13, 0x10, 0x10, 0x10);
    range = (float)height - 30.0f;
    for (i = 0; i < 4; i++) {
        y = (int)(range * (i * 0.25f) + 8.0f);
        d2_PlotLine(0x28, y, 0x2d, y, 0x10, 0x10, 0x10);
    }
    i = 0;
    for (pct = 100; pct > 0; pct -= 25) {
        txt_Format(buf, "%i%", pct);
        obj = d2_CreateWinText(3, (int)(range * (i * 0.25f) + 5.0f), buf, d2_LastWindow);
        d2_Objs[obj].w = 0x23;
        d2_Objs[obj].rightAligned = 1;
        i++;
    }
    if (top != 0) {
        d2_PlotLine(width - 0x3c, 8, width - 0x3c, height - 0x13, 0x10, 0x10, 0x10);
        d2_PlotLine(width - 0x37, 8, width - 0x3c, 8, 0x10, 0x10, 0x10);
        mid = (height - 0x1e) / 2;
        d2_PlotLine(width - 0x37, mid + 8, width - 0x3c, mid + 8, 0x10, 0x10, 0x10);
        txt_Format(buf, "%i", top);
        textW = d2_TextWidth(buf, 0x55) + 2;
        obj = d2_CreateWinText(width - 0x3c, 5, buf, d2_LastWindow);
        d2_Objs[obj].w = textW;
        d2_Objs[obj].rightAligned = 1;
        txt_Format(buf, "%i", top / 2);
        obj = d2_CreateWinText(width - 0x3c, mid + 5, buf, d2_LastWindow);
        d2_Objs[obj].w = textW;
        d2_Objs[obj].rightAligned = 1;
    }
    clock = ai_Time;
    gm_ClockToDate(&clock, (unsigned char *)&date);
    if (date.month == 1) {
        txt_Format(buf, "%i", date.year);
        textW = d2_TextWidth(buf, 0x55);
        obj = d2_CreateWinText(0x2d + scale * 16, height - 0xd, buf, d2_LastWindow);
        d2_Objs[obj].w = textW;
    }
    clock.year--;
    gm_ClockToDate(&clock, (unsigned char *)&date);
    for (round = 15; round > 0; round--) {
        x = 0x2d + round * scale;
        d2_PlotLine(x, height - 0x13, x, height - 0x10, 0x10, 0x10, 0x10);
        if (date.month == 1) {
            txt_Format(buf, "%i", date.year);
            textW = d2_TextWidth(buf, 0x55);
            obj = d2_CreateWinText(0x2d + round * scale, height - 0xd, buf, d2_LastWindow);
            d2_Objs[obj].w = textW;
            d2_PlotLine(x, height - 0x13, x, height - 0xd, 0x10, 0x10, 0x10);
        }
        clock.year--;
        gm_ClockToDate(&clock, (unsigned char *)&date);
    }
    if (date.month == 1) {
        txt_Format(buf, "%i", date.year);
        textW = d2_TextWidth(buf, 0x55);
        obj = d2_CreateWinText(0x2d, height - 0xd, buf, d2_LastWindow);
        d2_Objs[obj].w = textW;
    }
    d2_PenSize = old;
}

float ord_GetStatistics(int *o);

/* 0x5c7f00, the statistics panel "panel_statistik_stadt" (995 B): when the run allows it (ai_RunAllows 0x10) and
   between the campaign steps 0xc, it opens the form and prints the town's name with the date (0x1f0e), the running
   rating, the rank texts of the overall score (0x1f16 + 0 .. 4) and the combined rating (0x1f1c + 0 .. 4) and the
   two shares without the first / second object in percent; page 3 holds the charts ai_DrawStatisticsCharts draws
   by ai_StatsChartFlags, page 4 the legend: five check boxes with their texts 0x1f25 .. and a colour bar of the
   chart. A click on the legend rebuilds the flags from the check boxes and redraws the charts; the right button
   closes the panel */
void ai_CityStatsPanel(void)
{
    unsigned char colors[15];
    ORDSTATS stats;
    int ids[5];
    char buf[0x100];
    int win;
    int i;
    int k;
    int obj;
    int checked;

    colors[0] = 0x33;
    colors[1] = 0x33;
    colors[2] = 0x33;
    colors[3] = 0x10;
    colors[4] = 0xee;
    colors[5] = 0x10;
    colors[6] = 0xee;
    colors[7] = 0x10;
    colors[8] = 0x10;
    colors[9] = 0xee;
    colors[10] = 0xee;
    colors[11] = 0;
    colors[12] = 0xee;
    colors[13] = 0x10;
    colors[14] = 0xee;
    if (ai_StatsChartFlags == 0)
        ai_StatsChartFlags = 1;
    if (!ai_RunAllows(0x10))
        return;
    ai_CampaignStep(0xc, 0, 0, 0);
    win = d2_OpenForm(0, 0, "panel\\statistik_stadt");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f0e, ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townName, &ai_Time);
    ord_GetStatistics((int *)&stats);
    d2_SetForm(win, 2);
    io_PrintText(0x1f14, (int)ai_RatingBase);
    io_PrintText(0x1f15, min(4, (int)((stats.score + 1.0) * 2.5)) + 0x1f16);
    io_PrintText(0x1f1b, min(4, (int)(stats.col[2] * 5.0f)) + 0x1f1c);
    io_PrintText(0x1f21, (int)max(stats.col[1] * 100.0f, 0.0));
    io_PrintText(0x1f22, (int)(max(stats.col[3], 0.0) * 100.0));
    d2_SetForm(win, 3);
    ai_DrawStatisticsCharts(ai_StatsChartFlags, stats, d2_LastWindow);
    d2_SetForm(win, 4);
    for (i = 0; i < 5; i++) {
        checked = (ai_StatsChartFlags & (1 << i)) != 0;
        obj = d2_WinOpenPic(5, i * 30 + 5, 0, d2_LastWindow);
        ids[i] = obj;
        txt_Format(buf, i + 0x1f25);
        d2_CreateWinText(0x19, i * 30 + 4, buf, d2_LastWindow);
        d2_Objs[obj].interactive = 1;
        d2_Objs[obj].checkable = 1;
        d2_SetObjValue2(obj, checked);
        for (k = 0; k < 10; k++)
            d2_PlotLine(d2_Wins[d2_LastWindow].w - 1, i * 30 + 9 + k, d2_Wins[d2_LastWindow].w - 0xb, i * 30 + 9 + k,
                        colors[i * 3], colors[i * 3 + 1], colors[i * 3 + 2]);
    }
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            ai_StatsChartFlags = 0;
            for (i = 0; i < 5; i++)
                ai_StatsChartFlags |= d2_GetObjValue(ids[i]) ? 1 << i : 0;
            d2_SetForm(win, 3);
            ai_DrawStatisticsCharts(ai_StatsChartFlags, stats, d2_LastWindow);
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_CityStatsPanel, 0x657c7) != 0);
    d2_CloseView(win);
    ai_CampaignStep(0xc, 0, 1, 0);
}


/* the x of a column of the player comparison panel (ai_PlayerComparePanel) */
#define LC_COMPARE_X(count, i) (((count) <= 4 ? 0x50 : 0x3a) * (i) + ((count) <= 4 ? 0x14 : 2))

/* 0x5c82f0, the player comparison panel (837 B): when there are ranked players (gm_RankPlayers, up to 8) and the run
   allows it (ai_RunAllows 4), between the campaign steps 10 it opens "panel\pcompare4plus6" (up to 6 players) or
   "panel\pcompare8" and prints the caption 0x1f35; per player a column: the dynasty picture of the character
   (AICHARREC.ref54, its object linked to the character), the name, a slider of the weighted sum (GMRANK.sum * 100) in
   the dynasty's colour and the points. The columns lie 0x50 apart from 0x14, or 0x3a apart from 2 for more than four
   players. The right button closes the panel */

void ai_PlayerComparePanel(void)
{
    int count;
    int sliderPics[8];
    char buf[0x100];
    GMRANK ranks[8];
    GMRANK *rank;
    int i;
    int value;
    int obj;
    int colour;
    int win;

    sliderPics[0] = 0x8f;
    sliderPics[1] = 0x95;
    sliderPics[2] = 0x9b;
    sliderPics[3] = 0xa1;
    sliderPics[4] = 0xa7;
    sliderPics[5] = 0xad;
    sliderPics[6] = 0xb3;
    sliderPics[7] = 0xb9;
    gm_RankPlayers(&count, ranks);
    if (count == 0 || ai_RunAllows(4) == 0)
        return;
    ai_CampaignStep(10, 0, 0, 0);
    if (count <= 6)
        win = d2_OpenForm(0, 0, "panel\\pcompare4plus6");
    else
        win = d2_OpenForm(0, 0, "panel\\pcompare8");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f35);
    d2_SetForm(win, 2);
    for (i = 0; i < count; i++) {
        rank = &ranks[i];
        value = (int)(rank->sum * 100.0f);
        obj = d2_WinOpenPic(0x28, LC_COMPARE_X(count, i), ((AICHARREC *)ai_Chars)[rank->id].ref54, d2_LastWindow);
        d2_Objs[obj].interactive = 1;
        d2_Objs[obj].style = 3;
        d2_Objs[obj].user = ai_Chars[rank->id];
        txt_Format(buf, (int)"%1N3", rank->id);
        ai_CentredText(buf, 0x36, LC_COMPARE_X(count, i) + 0x22, 300, 0x56);
        colour = min(max(((AICHARREC *)ai_Chars)[rank->id].ref54 - 0x59a, 0), 7);
        obj = d2_WinOpenSlider(0x5a, LC_COMPARE_X(count, i) + 7, 300, 0, 100, sliderPics[colour], 0x42, d2_LastWindow);
        d2_SetObjValue4(obj, 0, 100, value);
        txt_Format(buf, "%i", rank->points);
        ai_CentredText(buf, 500, LC_COMPARE_X(count, i), 200, 0x55);
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PlayerComparePanel, 0x657c7) != 0);
    d2_CloseView(win);
    ai_CampaignStep(10, 0, 1, 0);
}

int ord_PrivilegeDialog();

/* 0x5c8640, the privileges panel "panel_privilegien" (1403 B): a held player character gets the privilege dialog
   with the text 0x1cdf instead; else, when the run allows it (ai_RunAllows 2), between the campaign steps 8 it lists
   the orders (ord_DispatchOrder, the 0x3a handlers) the player's character may give: page 2 its own orders (those
   answering 2, and greyed the ones answering 9), page 10 the orders with its partner of the pair event 0x45 (found
   when the office power 0x100 is used or a partner was known), switched by the partner button (0x1cdc / 0x1cdb with
   the partner's id). Every 500 clock ticks or on a change the handlers are asked again and the list rebuilt; a click
   on an order runs it (kind 3 / 4) and redraws when it reports a change. The right button closes the panel */
void ai_PrivilegesPanel(void)
{
    int changed;
    int rows;
    AICHARREC *partner;
    int button;
    unsigned int next;
    char fmt[8] = "%ia[%s]";
    LCORDMSG msg;
    int answers[0x3a];
    int ids[0x3a];
    AICHARREC *old;
    AIJOB *job;
    int page;
    int win;
    int i;
    int v;
    int k;

    button = -1;
    page = 2;
    partner = 0;
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds != 0) {
        ord_PrivilegeDialog(txt_Ptrs[0x1cdf], 0, 0x657c7);
        return;
    }
    if (!ai_RunAllows(2))
        return;
    ai_CampaignStep(8, 0, 0, 0);
    memset(&msg, 0, sizeof(msg));
    msg.kind = 1;
    win = d2_OpenForm(0, 0, "panel\\privilegien");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    changed = 1;
    for (i = 0; i < 0x3a; i++)
        answers[i] = 1;
    d2_ClickedId = -1;
    next = d3_Time;
    do {
        rows = 0;
        if (next <= d3_Time || changed) {
            next = d3_Time + 500;
            old = partner;
            if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x100) != 0 || partner != 0) {
                for (job = (AIJOB *)he_FindEvent(1, 0, 0x45); job != 0; job = (AIJOB *)he_NextEvent()) {
                    if (job->partner1 == ((AICHARREC *)ai_Chars)[ai_PlayerId].netId)
                        break;
                }
                if (job != 0)
                    partner = (AICHARREC *)gm_FindChar(job->partner2);
                else
                    partner = 0;
            }
            msg.partner = partner;
            if (partner != old || (partner == 0 && (msg.kind == 2 || button != -1))) {
                changed = 1;
                page = 2;
            }
            if (page == 2)
                msg.kind = 1;
            else
                msg.kind = 2;
            for (i = 0; i < 0x3a; i++) {
                v = ord_DispatchOrder(i, ai_Chars[ai_PlayerId], (int *)&msg);
                if (v != answers[i])
                    changed = 1;
                answers[i] = v;
            }
            if (changed) {
                changed = 0;
                d2_SetForm(win, 2);
                io_PrintText("$C");
                button = -1;
                d2_Wins[d2_LastWindow].contentHeight = 0;
                d2_Wins[d2_LastWindow].scrollY = 0;
                if (page == 2) {
                    d2_SetForm(win, 1);
                    io_PrintText(0x1cdd);
                    if (partner != 0) {
                        d2_SetForm(win, 3);
                        button = d2_GetWinObj(win, 3, io_PrintText(0x1cdc, partner->id));
                    }
                } else if (page == 10) {
                    if (partner != 0) {
                        d2_SetForm(win, 1);
                        io_PrintText(0x1cde);
                        d2_SetForm(win, 3);
                        button = d2_GetWinObj(win, 3, io_PrintText(0x1cdb, partner->id));
                    } else {
                        d2_SetForm(win, 1);
                        io_PrintText(0x1cdd);
                        page = 2;
                    }
                }
                d2_SetForm(win, 2);
                msg.kind = (page != 2) + 1;
                for (i = 0; i < 0x3a; i++) {
                    ids[i] = -1;
                    v = ord_DispatchOrder(i, ai_Chars[ai_PlayerId], (int *)&msg);
                    answers[i] = v;
                    if (v == page) {
                        if (rows != 0)
                            io_PrintText("$A");
                        ids[i] = d2_GetWinObj(win, 2, io_PrintText(fmt, msg.text));
                        rows++;
                    } else if (page == 2 && v == 9) {
                        if (rows != 0)
                            io_PrintText("$A");
                        k = d2_GetWinObj(win, 2, io_PrintText(fmt, msg.text));
                        ids[i] = -1;
                        d2_Objs[k].clickDisabled = 1;
                        d2_Objs[k].greyed = 1;
                        rows++;
                    }
                }
                if (rows > 9) {
                    d2_SetForm(win, 2);
                    k = d2_LastWindow;
                    d2_SetForm(win, 4);
                    io_PrintText("$C");
                    d2_CreateScrollButtons(-1, -1, d2_LastWindow, k, 0x767);
                } else {
                    d2_SetForm(win, 4);
                    io_PrintText("$C");
                }
            }
        }
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == button) {
                changed = 1;
                page = page == 2 ? 10 : 2;
            } else {
                for (i = 0; i < 0x3a; i++) {
                    if (d2_PointerObj == ids[i]) {
                        d2_SetViewEnabled(win, 0);
                        msg.kind = (page != 2) + 3;
                        v = ord_DispatchOrder(i, ai_Chars[ai_PlayerId], (int *)&msg);
                        if (v & 0x80) {
                            changed = 1;
                            answers[i] = v & 10;
                        }
                        if (page == 2)
                            msg.kind = 1;
                        else
                            msg.kind = 2;
                        d2_SetViewEnabled(win, 1);
                        break;
                    }
                }
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PrivilegesPanel, 0x657c7) != 0);
    d2_CloseView(win);
    ai_CampaignStep(8, 0, 1, 0);
}

/* 0x5c8bc0, the medicus order's ask (ord_MedicusOrder, kind 3) on the hurt or sick character ch (803 B): a character
   of another kind than 6 / 7 with at least 2 in money pays the medicus 2 and gets the order's flag 0x40, and in
   one of two cases is healed by half its hurt (v024), cured of a disease and the player paid 2 % of its worth:
   0x11; else 0. A kind 6 / 7 character's player gets the window "privillegien\medicus" with the offer (0x1d3b,
   the price 2 % of the worth) and its two buttons: yes pays 2 (sv_CheckMoney / sv_CheckGoodAmount), and in one of
   two cases cures a disease and pays the price to the player (the texts 0x1d3c + 0x1d3d / 0x1d3e), waits for the
   flag 0x40 to be set and shows the text: 0x81; no or the right button closes the window: 2 */
int ai_MedicusHandler(AICHARREC *ch, LCORDMSG *m)
{
    int price;
    int ret;
    int win;
    char note[0x100];
    char text[0x400];
    int heal;
    int yes;
    int no;
    int slot;

    ret = 2;
    if (ch->kind != 6 && ch->kind != 7) {
        if (ch->money < 2)
            return 0;
        heal = abs(ch->v024) / 2;
        price = (int)(gm_GetWorth(ch->id) * 0.02f);
        cm_SendMoneyDelta(ch->netId, -2);
        cm_SendSetField(ch->netId, 0x1c8, 4, 0x40, 0);
        if ((short)util_Random(2) != 0) {
            cm_SendMoveGoods(-1, ch->netId, ai_OwnPlayer, price);
            gm_AddHealth(ch->id, heal);
            gm_CureDisease((char *)ch);
        }
        return 0x11;
    }
    price = (int)(gm_GetWorth(ch->id) * 0.02f);
    win = d2_OpenForm(0, 0, "privillegien\\medicus");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    slot = io_PrintText(0x1d3b, 2, price);
    yes = d2_GetWinObj(win, 1, slot);
    no = d2_GetWinObj(win, 1, slot + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                if (sv_CheckMoney(ch, 2) != 0 && sv_CheckGoodAmountB((char *)ch, price, ai_OwnPlayer) != 0) {
                    cm_SendMoneyDelta(ch->netId, -2);
                    txt_Format(text, 0x1d3c);
                    if ((short)util_Random(2) != 0) {
                        txt_Format(note, "%s", 0x1d3d);
                        strcat(text, note);
                        cm_SendMoveGoods(-1, ch->netId, ai_OwnPlayer, price);
                        gm_CureDisease((char *)ch);
                    } else {
                        txt_Format(note, "%s", 0x1d3e);
                        strcat(text, note);
                    }
                    yes = -1;
                    no = -1;
                    ret = 0x81;
                    slot = cm_SendSetField(ch->netId, 0x1c8, 4, 0x40, 0);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_MedicusHandler, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5c8ef0, the divorce order's ask (ord.c 0x4f5800, kind 3) on the character ch (927 B): of another kind than 6 / 7
   it needs 8 in money and goods of at least 0.12 of its worth (else 0x22), pays 8, clears its spouse
   (relatives[0], field 0x5c) and the spouse's spouse and family word (field 0x50) to -1 and pays the spouse 8 % of
   its worth: 0x11. A kind 6 / 7 character's player gets the window "privillegien\scheidung" with the offer
   (0x1d8f: the text 0x114 + b00d (+ 10 for sex), 8, the price 8 % of the worth plus 4 %, the spouse's id); its
   first button pays 8 and the price (sv_CheckMoney / sv_CheckGoodAmount), sends the same fields and the money,
   each waited for, and prints 0x1d90: 0x81; the second button or the right button close it: 2, else 0 */
int ai_DivorceDialog(AICHARREC *ch, LCORDMSG *m)
{
    int win;
    int text;
    AICHARREC *spouse;
    int slot;
    int none;
    int done;
    int price;
    float worth;

    none = -1;
    done = 0;
    if (ch->kind != 6 && ch->kind != 7) {
        if (ch->money < 8)
            return 0x22;
        worth = (float)gm_GetWorth(ch->id);
        if (gm_GetGoodsValue(ch) / worth < 0.12)
            return 0x22;
        spouse = (AICHARREC *)gm_FindChar(ch->relatives[0]);
        cm_SendMoneyDelta(ch->netId, -8);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldValue(4, 1, 0x5c, &none);
        cm_SendFieldDeltas();
        cm_BeginFieldBlock((char *)spouse, spouse->netId);
        cm_AddFieldValue(4, 1, 0x5c, &none);
        cm_AddFieldValue(2, 1, 0x50, &none);
        cm_SendFieldDeltas();
        slot = cm_SendMoveGoods(spouse->netId, ch->netId, ai_OwnPlayer, (int)(worth * 0.08));
        while (cm_GetSlotState(slot) == 0)
            sv_PumpNet();
        return 0x11;
    }
    worth = (float)gm_GetWorth(ch->id);
    price = (int)(worth * 0.08);
    spouse = (AICHARREC *)gm_FindChar(ch->relatives[0]);
    win = d2_OpenForm(0, 0, "privillegien\\scheidung");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    text = (ch->sex != 0 ? 10 : 0) + 0x114;
    slot = io_PrintText(0x1d8f, text + ch->b00d, 8, price - (int)(worth * -0.04), spouse->id,
                        spouse->id);
    d2_GetWinObj(win, 1, slot);
    d2_GetWinObj(win, 1, slot + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                if (sv_CheckMoney(ch, 8) != 0 && sv_CheckGoodAmountB((char *)ch, price, ai_OwnPlayer) != 0) {
                    cm_SendMoneyDelta(ch->netId, -8);
                    cm_BeginFieldBlock((char *)ch, ch->netId);
                    cm_AddFieldValue(4, 1, 0x5c, &none);
                    slot = cm_SendFieldValues();
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    cm_BeginFieldBlock((char *)spouse, spouse->netId);
                    cm_AddFieldValue(4, 1, 0x5c, &none);
                    cm_AddFieldValue(2, 1, 0x50, &none);
                    slot = cm_SendFieldValues();
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    slot = cm_SendMoveGoods(spouse->netId, ch->netId, ai_OwnPlayer, price);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    io_PrintText("$C");
                    io_PrintText(0x1d90);
                    sv_PopRequest = 1;
                    done = 0x81;
                }
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
                done = 2;
            }
        }
        if (dm_L.rup) {
            sv_PopRequest = 1;
            done = 2;
        }
    } while (sv_RunFrame((int)ai_DivorceDialog, 0x657c7) != 0);
    d2_CloseView(win);
    return done;
}

int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t);

/* 0x5c9290, the blackmail window "privillegien\erpressung2" (ai_BlackmailBuildingListener on a click): the
   player's character me blackmails the character victim with the evidence me holds against it
   (gv_HeldCrimeIdsOf: the count of those crimes). Null or me not of kind 6 / 7: 0. Button 0 draws util_Random(8):
   more than the evidence count fails (relations -26, me pays 2, the text 0x1d59), else the blackmail message 0x45
   (me to me, arg[0] me, arg[1] the victim, flags 2, sent now) goes out and waits for its event to finish
   (AIJOB.stage -1) or me's powerFlags 0x100 (the victim gave in: the text 0x1d5a with the clock 0x60 hours ahead),
   returns 1; button 1 or the right button close it */
int ai_BlackmailWindow(AICHARREC *me, AICHARREC *victim)
{
    char text[256];
    int crimes[0x20];
    AITIME due;
    AIMSG msg;
    AIJOB *job;
    int win;
    int h;
    int held;
    int ret;

    if (me == 0 || victim == 0)
        return 0;
    if (me->kind != 6 && me->kind != 7)
        return 0;
    held = gv_HeldCrimeIdsOf(victim->netId, crimes, me->id);
    win = d2_OpenForm(0, 0, "privillegien\\erpressung2");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d57, victim->id, victim->id, victim->id, victim->id, 2);
    io_PrintText(0x1d58);
    d2_ClickedId = -1;
    ret = 0;
    do {
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                if (held >= (int)(util_Random(8) & 0xffff)) {
                    due = ai_Time;
                    gm_AddTime(&due, 0x60, 0, 0);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x45;
                    msg.to = me->netId;
                    msg.fromId = me->netId;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    msg.arg[0] = me->netId;
                    msg.arg[1] = victim->netId;
                    h = cm_SendLongMessage((char *)&msg);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    job = (AIJOB *)cm_GetSlotValue(h);
                    while ((me->powerFlags & 0x100) == 0 && job->stage != -1)
                        sv_PumpNet();
                    if ((me->powerFlags & 0x100) != 0) {
                        txt_Format(text, 0x1d5a, victim->id, &due);
                        sv_MessageBox(text, 0);
                    }
                    ret = 1;
                    sv_PopRequest = 1;
                } else {
                    cm_SendRelations(me->netId, victim->netId, -26, 1);
                    cm_SendMoneyDelta(me->netId, -2);
                    txt_Format(text, 0x1d59, victim->id);
                    sv_MessageBox(text, 0);
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if (d2_ClickedId == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BlackmailWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}

/* 0x5c9550, the blackmail order's ask (ord_BlackmailBuildingOrder, kind 3) for the character me with the order
   message m. Of another kind than 6 / 7 (the AI): no character m->target: 0x60; else util_Random(8) above the
   count of the crimes me holds evidence of against it (gv_HeldCrimeIdsOf) lowers the relations by 26, else the
   blackmail message 0x45 goes out as ai_BlackmailWindow sends it (the clock 0x60 hours ahead is computed but not
   used); both 0x10. A player gets the window "privillegien\erpressung" (caption 0x1d55, a scrolled list): one
   amount row per culprit me holds evidence of (gv_RankCulpritsAll, as the evidence book ai_EvidenceBookWindow
   lists them, "%i (%i) %s"; none: the text 0x1d50). A clicked row, the player paying 2 (sv_CheckMoney): a
   culprit with powerFlags 0x200 gives the text 0x1d52, fewer than 3 open evidences the text 0x1d51, else the
   blackmail window ai_BlackmailWindow (closing this one once it sent) and the result 0x92; else 0x20 */

int ai_BlackmailBuildingListener(AICHARREC *me, LCORDMSG *m)
{
    int win;
    int list;
    int count;
    int n;
    int i;
    int ret;
    AICHARREC *ch;
    AICHARREC *victim;
    AITIME due;
    int total[0x20];
    char text[0x40];
    int open[0x20];
    int ids[0x20];
    char warn[0x200];
    LCAMOUNTROW rows[0x20];

    if (me->kind != 6 && me->kind != 7) {
        victim = (AICHARREC *)gm_FindChar(m->target);
        if (victim == 0)
            return 0x60;
        open[0] = gv_HeldCrimeIdsOf(victim->netId, ids, me->id);  /* the count in open[0]: the original's slot */
        if (open[0] >= (int)(util_Random(8) & 0xffff)) {
            AIMSG msg;

            due = ai_Time;
            gm_AddTime(&due, 0x60, 0, 0);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x45;
            msg.to = me->netId;
            msg.fromId = me->netId;
            msg.flags = 2;
            msg.time = ai_Time;
            msg.arg[0] = me->netId;
            msg.arg[1] = victim->netId;
            cm_SendLongMessage((char *)&msg);
            return 0x10;
        }
        cm_SendRelations(me->netId, victim->netId, -26, 1);
        return 0x10;
    }
    win = d2_OpenForm(0, 0, "privillegien\\erpressung");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d55);
    d2_SetForm(win, 2);
    list = d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, list, 0x767);
    count = gv_RankCulpritsAll(ids, open, total, me->id);
    for (i = 0, n = 0; i < count; i++) {
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
        io_PrintText(0x1d50);
    }
    ret = 0x20;
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == rows[i].ctl[0]) {
                    if (sv_CheckMoney(me, 2) != 0) {
                        if ((rows[i].rec->powerFlags & 0x200) != 0) {
                            txt_Format(warn, 0x1d52, rows[i].rec->id);
                            sv_MessageBox(warn, 0);
                        } else if ((unsigned int)rows[i].x34 < 3) {
                            sv_MessageBox(txt_Ptrs[0x1d51], 0);
                        } else {
                            if (ai_BlackmailWindow(me, (AICHARREC *)gm_FindChar(ids[i])) != 0)
                                sv_PopRequest = 1;
                            ret = 0x92;
                        }
                    }
                    break;
                }
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BlackmailBuildingListener, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original ai_BlackmailContactWindow reads the 16-bit result (mov si, ax at 0x5c9ac3), as ai.c / am.c / amt.c do */

/* 0x5c9990, the blackmail contact's letter window "misc\perga_rolle" (the pick callback ai_BlackmailContact hands
   ai_AmountMenu) for the picked character ch: one already marked (powerFlags 0x400) gives the text 0x1d7f and 0.
   Else the offer 0x1d67 (ch's id, 2, the word 0x1d68 / 0x1d69 by ch's sex) on the parchment (ai_ShowWindowRow).
   Button 0, the player paying 2 (sv_CheckMoney): the letter 0x47 (the player to the player, letter.victim ch, two
   random texts 0x1d6d + 0..8 / 0x1d76 + 0..8, flags 2, sent now), waited for, the player pays 2 and gets the text
   0x1d6a; sv_PopRequest 3, returns 1. Button 1: sv_PopRequest 2; the right button 1 */

int ai_BlackmailContactWindow(AICHARREC *ch)
{
    char text[0x100];
    AIMSG msg;
    AIWINDOWROW row;
    int win;
    int h;
    int ret;
    int word;
    unsigned short text1;
    unsigned short text2;

    ret = 0;
    if ((ch->powerFlags & 0x400) != 0) {
        txt_Format(text, 0x1d7f, ch->id);
        sv_MessageBox(text, 0);
        return 0;
    }
    word = (ch->sex != 0) + 0x1d68;
    win = d2_OpenForm(0, 0, "misc\\perga_rolle");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d67, ch->id, 2, ch->id, word, ch->id);
    memset(&row, 0xff, sizeof(row));
    row.win = win;
    row.pageA = 2;
    row.pageB = 3;
    row.pageC = 4;
    row.entry = 4;
    row.record = 4;
    ai_ShowWindowRow(&row);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                if (sv_CheckMoney(&((AICHARREC *)ai_Chars)[ai_PlayerId], 2) != 0) {
                    text2 = util_RandomU(9) + 0x1d76;
                    text1 = util_RandomU(9) + 0x1d6d;
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x47;
                    msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.fromId = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    msg.letter.from = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.letter.text1 = text1;
                    msg.letter.text2 = text2;
                    msg.letter.victim = ch->netId;
                    h = cm_SendLongMessage((char *)&msg);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_SendMoneyDelta(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -2);
                    txt_Format(text, 0x1d6a, text1, ch->id, text2);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 3;
                    ret = 1;
                }
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 2;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BlackmailContactWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5c9c50, the blackmail contact order's ask (ord_BlackmailContactOrder, kind 3) for the character me with the
   order message m. Of another kind than 6 / 7 (the AI): no character m->target: 0x60; else the letter 0x47 to it
   as ai_BlackmailContactWindow sends it (not waited for), me pays 2: 0x10. A player picks the victim in the amount
   menu (mode 0x400, kind 6, the gypsy filter, the caption 0x1d66) with the letter window ai_BlackmailContactWindow
   as its pick callback: 0x81 when the picked character is marked (powerFlags 0x400) afterwards, else 2
   @deep deep s29: the original emits the letter fill as: the ai_Time dwords, me->netId into to / fromId /
   letter.from, text1, the victim->netId load, lea / push &msg, and only then the type 0x47, flags 2, the
   time's last word, text2 and the victim store (five stores after the push); ours puts every store before
   the push (order 30). The byte sibling ai_BlackmailContactWindow has the same statement order (type, to,
   fromId, flags, time, from, text1, text2, victim) and keeps its stores before the push. Tried: msg at
   function scope / cfg in the else block (= control); victim->netId into a local before the memset (64);
   no memset (diagnostic, 85); 1500 sampled orders of the nine fill statements (permute.py, best 12 ins /
   63 bytes); climb 2 rounds: 10 ins / 71 bytes (victim, flags, text2 last; the same order 30 hunks, bytes
   worse, not kept). */

int ai_BlackmailContact(AICHARREC *me, LCORDMSG *m)
{
    int ret;
    AICHARREC *victim;
    AICHARREC *pick;
    unsigned short text1;
    unsigned short text2;
    AMOUNTCFG cfg;

    ret = 2;
    if (me->kind != 6 && me->kind != 7) {
        AIMSG msg;

        victim = (AICHARREC *)gm_FindChar(m->target);
        if (victim == 0)
            return 0x60;
        text2 = util_RandomU(9) + 0x1d76;
        text1 = util_RandomU(9) + 0x1d6d;
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x47;
        msg.to = me->netId;
        msg.fromId = me->netId;
        msg.flags = 2;
        msg.time = ai_Time;
        msg.letter.from = me->netId;
        msg.letter.text1 = text1;
        msg.letter.text2 = text2;
        msg.letter.victim = victim->netId;
        cm_SendLongMessage((char *)&msg);
        cm_SendMoneyDelta(me->netId, -2);
        return 0x10;
    } else {
        char text[0x40];

        memset(&cfg, 0, sizeof(cfg));
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        txt_Format(text, 0x1d66);
        pick = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, ai_BlackmailContactWindow);
        if (pick != 0 && (pick->powerFlags & 0x400) != 0)
            ret = 0x81;
    }
    return ret;
}
/* 0x5c9df0, the intimidation window "privillegien\furcht_einfloessen" (the pick callback of the intimidation order
   0x5c9ff0's amount menu) for the picked character ch: the offer 0x1d62 (ch's id twice, 2, its id). Button 0, the player paying 2
   (sv_CheckMoney): the player pays 2 and sends the message 0x4b (the player to the player, arg[0] the player,
   arg[1] ch, flags 2, sent now), then the text 0x1d63 with the clock 0x18 hours ahead and ch's id; sv_PopRequest 4,
   returns 1. Button 1: sv_PopRequest 2; the right button 1 */

int ai_IntimidateWindow(AICHARREC *ch)
{
    char text[0x100];
    AIMSG msg;
    AITIME due;
    int win;
    int ret;

    ret = 0;
    win = d2_OpenForm(0, 0, "privillegien\\furcht_einfloessen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d62, ch->id, ch->id, 2, ch->id);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                if (sv_CheckMoney(&((AICHARREC *)ai_Chars)[ai_PlayerId], 2) != 0) {
                    cm_SendMoneyDelta(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -2);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x4b;
                    msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.fromId = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    msg.arg[0] = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.arg[1] = ch->netId;
                    cm_SendLongMessage((char *)&msg);
                    due = ai_Time;
                    gm_AddTime(&due, 0x18, 0, 0);
                    txt_Format(text, 0x1d63, &due, ch->id);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 4;
                    ret = 1;
                }
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 2;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_IntimidateWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5c9ff0, the intimidation order's ask (ord_IntimidateOrder, kind 3) for the character me with the order message
   m. Of another kind than 6 / 7 (the AI): no character m->target: 0x60; else me pays 2 and sends the message 0x4b
   to it as ai_IntimidateWindow does: 0x10. A player picks the victim in the amount menu (mode 0x400, kind 6, the
   gypsy filter, the caption 0x1d61) with ai_IntimidateWindow as its pick callback: 1 when one was picked, else 0
   @deep deep s43: climb --ops move,swap 4 rounds ends at the s23 best (to, time, arg[1], type, fromId,
   flags, arg[0]: 4 ins), where only msg.to and msg.arg[1] are stored before the push of &msg (the original
   stores type / to / fromId / flags, then arg[1], arg[0] after the push). Chained stores: every order of
   the 3-chains of to / fromId / arg[0] (720, best 20) and of the 2-chains starting with to (1440, best 6):
   no lever. Open: what makes the push of &msg rank above the constant stores (an escape of &msg before the
   fill, another block extent). prior: deep s23: left four AIMSG stores (type 0x4b, to, fromId, flags 2)
   that the original schedules after the arg[1] load and the push of &msg, ours before the last time-copy
   store. All 5040 orders of the seven fill statements (best 4 ins / 39 bytes), climb 2 rounds (8 ins / 29
   bytes), msg at function scope (= control), the money call after the fill and arg[0] before arg[1] (far
   worse): no lever found. he_AllocMeeting's fill is scheduled the original's way with the same statements. */

int ai_IntimidateContact(AICHARREC *me, LCORDMSG *m)
{
    int ret;
    AICHARREC *victim;

    ret = 0;
    if (me->kind != 6 && me->kind != 7) {
        AIMSG msg;

        victim = (AICHARREC *)gm_FindChar(m->target);
        if (victim == 0)
            return 0x60;
        cm_SendMoneyDelta(me->netId, -2);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x4b;
        msg.to = me->netId;
        msg.fromId = me->netId;
        msg.flags = 2;
        msg.time = ai_Time;
        msg.arg[1] = victim->netId;
        msg.arg[0] = me->netId;
        cm_SendLongMessage((char *)&msg);
        return 0x10;
    } else {
        AMOUNTCFG cfg;
        char text[0x40];

        memset(&cfg, 0, sizeof(cfg));
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        txt_Format(text, 0x1d61);
        if (ai_AmountMenu(&cfg, text, 0, ai_IntimidateWindow) != 0)
            ret = 1;
    }
    return ret;
}

int gv_SetTableRecord(struct AICHAR *c, int i, int v);   /* 0x4f2750 */

/* 0x5ca150, the law order's ask (ord_LawOrder / ord_LawOrder2, kinds 3 / 4) "privillegien\gesetz": the character me
   changes the decision m->law (AIDEC) of an office, the office's character being me (kind 3) or m->partner (kind 4;
   none or another kind: 0x60). m->answerFlag 1 / 2 picks the office flag 0x80000 / 0x100000 set on it afterwards
   (else 0x60). The AI (not kind 6) needs 2 in money (else 0x20) and a value m->value within the record's bounds
   other than its current one (else 0x60): it sets it (gv_SetTableRecord), pays 2 and sets the flag: 0. A player gets
   the window (caption 0x1ba9) with the decision's text by its valueText (0: the number with a slider min .. max;
   1: the names 0x13dd + it; 2: a switch 0x13e7 - cur; 3: the names 0x13ec + it; others: 0 at once); the apply button
   is enabled once the value differs. Button 0, me paying 2 (sv_CheckMoney): the new value (a switch: 1 - cur), the
   set waited for, me pays 2 and the flag is set: 0x80; button 1 or the right button close it */

int ai_LawPrivilegeWindow(AICHARREC *me, LCORDMSG *m)
{
    int flag;
    AICHARREC *owner;
    int ret;

    ret = 0;
    if (m->kind == 3) {
        owner = me;
    } else if (m->kind == 4 && m->partner != 0) {
        owner = m->partner;
    } else {
        return 0x60;
    }
    if (m->answerFlag == 1)
        flag = 0x80000;
    else if (m->answerFlag == 2)
        flag = 0x100000;
    else
        return 0x60;
    if (me->kind != 6) {
        AIDEC rec;

        if (me->money < 2)
            return 0x20;
        if (gv_GetTableRecord(m->law, &rec) == 0)
            return 0x60;
        if (m->value == rec.cur || m->value > rec.max || m->value < rec.min)
            return 0x60;
        gv_SetTableRecord((struct AICHAR *)me, rec.id, m->value);
        cm_SendMoneyDelta(me->netId, -2);
        cm_SendSetField(owner->netId, 0x1c8, 4, flag, 0);
    }
    if (me->kind == 6) {
        int base = 0;
        int apply = 1;
        int ctrl2 = -1;
        int win;
        int word;
        int t;
        int ctrl;
        int h;
        int v;
        int from;
        AIDEC rec = {0};

        if (gv_GetTableRecord(m->law, &rec) == 0)
            return 0;
        word = rec.id * 5 + 0x13fd;
        win = d2_OpenForm(0, 0, "privillegien\\gesetz");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        io_PrintText(0x1ba9);
        switch (rec.valueText) {
        case 0:
            t = io_PrintText(0x13d0, word, 0x13d3);
            ctrl = d2_GetWinObj(win, 1, t);
            ctrl2 = d2_GetWinObj(win, 1, t + 1);
            d2_SetObjValue4(ctrl, rec.min, rec.max, rec.cur);
            base = 0;
            break;
        case 1:
            t = io_PrintText(0x13da, word, 0x13dd, rec.cur + 0x13dd);
            ctrl = d2_GetWinObj(win, 1, t);
            ctrl2 = d2_GetWinObj(win, 1, t + 1);
            base = 0x13dd;
            break;
        case 2:
            ctrl = d2_GetWinObj(win, 1, io_PrintText(0x13e3, word, word, 0x13e7 - rec.cur));
            apply = 0;
            break;
        case 3:
            t = io_PrintText(0x13e9, 0x13ec, rec.cur + 0x13ec);
            ctrl = d2_GetWinObj(win, 1, t);
            ctrl2 = d2_GetWinObj(win, 1, t + 1);
            base = 0x13ec;
            break;
        default:
            return 0;
        }
        d2_ClickedId = -1;
        do {
            if (apply) {
                if (d2_GetObjValue(ctrl) - base == rec.cur || ctrl2 == -1)
                    d2_SetObjEnabled(ctrl2, 0);
                else
                    d2_SetObjEnabled(ctrl2, 1);
            }
            if (dm_L.rup)
                sv_PopRequest = 1;
            if (d2_ClickedId != -1) {
                if (d2_ClickedId == 0) {
                    if (sv_CheckMoney(me, 2) != 0) {
                        if (rec.valueText == 2) {
                            v = 1;
                            from = rec.cur;
                        } else {
                            v = d2_GetObjValue(ctrl);
                            from = base;
                        }
                        ret = 0x80;
                        h = gv_SetTableRecord((struct AICHAR *)me, rec.id, v - from);
                        if (h != -1) {
                            while (cm_GetSlotState(h) == 0)
                                sv_PumpNet();
                            cm_SendMoneyDelta(me->netId, -2);
                            cm_SendSetField(owner->netId, 0x1c8, 4, flag, 0);
                        }
                        sv_PopRequest = 1;
                    }
                } else if (d2_ClickedId == 1) {
                    sv_PopRequest = 1;
                }
            }
        } while (sv_RunFrame((int)ai_LawPrivilegeWindow, 0x657c7) != 0);
        d2_CloseView(win);
    }
    return ret;
}

int amt_GetKind(); int amt_FindOffice(); int amt_SendOfficeCandidates(); int ai_RelationGain();

/* 0x5ca560, the dismissal order's ask (ord_DismissOfficeOrder, kinds 3 / 4): the office holder owner (me
   for kind 3, m->partner for kind 4; none or another kind: 0x60) has an office holder of the same office column
   (AMTKIND.col of owner's officeKind) removed ("absetzen"). The AI (not kind 6): the character m->target, its
   office found (amt_FindOffice) and of owner's column (else 0x60), is removed in the group "absetzen": its
   candidates (amt_SendOfficeCandidates), owner's flag 0x200000 (powerFlags) and the relation to it -40: 0x10. A
   player picks the character in the special-amount list of owner's column (mode 0x400, kind 6, the gypsy filter,
   option 3, leaving out me and, of a partner's ask, owner; the captions 0x1d98 / 0x1d99): an office in state 1 is
   removed as above with the relation -20, the flag's send and the group waited for; unless the group failed
   (state 2) the text 0x1d9b and 0x90, else the text 0x1d9a and 0x20; nothing picked: 0x20
   @original the refusal block is shared by the state test and the failed group (goto refused): written out at
   both or nested under if (office.state == 1) VC6 lays it out otherwise (42 / 40 instructions off) */

int ai_DismissOfficeAsk(AICHARREC *me, LCORDMSG *m)
{
    AICHARREC *owner;
    AICHARREC *ch;
    AMTKIND ownerKind;
    int officeKind;
    int flagSlot;
    int slot;

    if (m->kind == 3) {
        owner = me;
        officeKind = me->officeKind;
    } else if (m->kind == 4 && m->partner != 0) {
        owner = m->partner;
        officeKind = owner->officeKind;
    } else {
        return 0x60;
    }
    if (amt_GetKind(officeKind, &ownerKind) == 0)
        return 0x60;
    if (me->kind != 6) {
        AMTKIND kind;
        AMTREC office;

        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0)
            return 0x60;
        if (amt_FindOffice((char *)ch, &kind, &office) == 0)
            return 0x60;
        if (ownerKind.col != kind.col)
            return 0x60;
        cm_BeginGroup("absetzen");
        amt_SendOfficeCandidates((char *)ch, (char *)owner);
        cm_SendSetField(owner->netId, 0x1c8, 4, 0x200000, 0);
        cm_SendRelations(me->netId, ch->netId, -40, 1);
        cm_EndGroup("absetzen");
        return 0x10;
    } else {
        AMOUNTCFG cfg;
        AMTREC office;
        AMTKIND kind;
        char text[0x100];
        char caption2[0x100];
        char caption[0x100];

        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        cfg.ids[0] = me->id;
        if (m->partner == owner)
            cfg.ids[1] = owner->id;
        cfg.kind = 6;
        cfg.option14 = 3;
        cfg.mode = 0x400;
        cfg.classMask[0] = gypsyFilterWords[0];
        cfg.classMask[1] = gypsyFilterWords[1];
        txt_Format(caption, 0x1d98);
        txt_Format(caption2, 0x1d99);
        ch = (AICHARREC *)ai_SpecialAmountListWindow(ownerKind.col, (unsigned int *)&cfg, caption, caption2, 0);
        if (ch != 0) {
            if (amt_FindOffice((char *)ch, &kind, &office) == 0)
                return 0x60;
            if (office.state != 1)
                goto refused;
            cm_BeginGroup("absetzen");
            amt_SendOfficeCandidates((char *)ch, (char *)owner);
            flagSlot = cm_SendSetField(owner->netId, 0x1c8, 4, 0x200000, 0);
            cm_SendRelations(me->netId, ch->netId, -20, 1);
            slot = cm_EndGroup("absetzen");
            while (cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            if (cm_GetSlotState(slot) == 2) {
refused:
                txt_Format(text, 0x1d9a, ch->id);
                sv_MessageBox(text, 0);
                return 0x20;
            }
            while (cm_GetSlotState(flagSlot) == 0)
                sv_PumpNet();
            txt_Format(text, 0x1d9b, ch->id, ch->id);
            sv_MessageBox(text, 0);
            return 0x90;
        }
        return 0x20;
    }
}
#define LC_POWER_EMBEZZLED 0x400000  /* AICHARREC.powerFlags: the office's embezzlement used (ai_MisappropriationWindow) */

/* 0x5ca860, the embezzlement order's ask "privillegien\unterschlagung" (kinds 3 / 4) for the character me: the office
   holder owner (me for kind 3, m->partner for kind 4; none or another kind: 0x60) embezzles (25 .. 200) * 0.005 of
   its wage (gm_CharacterWages' ORDWAGES.amount). The AI (not kind 6) needs 4 in money (else 0x20): the sum is paid to
   me (cm_SendMoveGoodsUnchecked from -1), me pays 4 and owner's flag 0x400000 (powerFlags) is set: 0. A player gets the
   window with the offer 0x1dc2 (me's id); button 0, me paying 4 (sv_CheckMoney), does the same, waits for the flag's
   send and shows the text 0x1dc3 with the sum: 0x90; button 1 or the right button close it */

int ai_MisappropriationWindow(AICHARREC *me, LCORDMSG *m)
{
    ORDWAGES wages = {0};
    char text[0x100];
    AICHARREC *owner;
    int ret = 0;
    int sum;
    int win;
    int h;

    if (m->kind == 3) {
        owner = me;
    } else if (m->kind == 4 && m->partner != 0) {
        owner = m->partner;
    } else {
        return 0x60;
    }
    gm_CharacterWages(owner->id, 0, (unsigned char *)&wages);
    sum = (int)(wages.amount * ((float)((util_Random(0xb0) & 0xffff) + 0x19) * 0.005f));
    if (me->kind != 6) {
        if (me->money < 4)
            return 0x20;
        cm_SendMoveGoodsUnchecked(me->netId, -1, 0, sum);
        cm_SendMoneyDelta(me->netId, -4);
        cm_SendSetField(owner->netId, 0x1c8, 4, LC_POWER_EMBEZZLED, 0);
    } else {
        win = d2_OpenForm(0, 0, "privillegien\\unterschlagung");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        io_PrintText(0x1dc2, me->id);
        d2_ClickedId = -1;
        do {
            if (d2_ClickedId != -1) {
                if (d2_ClickedId == 0) {
                    if (sv_CheckMoney(me, 4) != 0) {
                        cm_SendMoveGoodsUnchecked(me->netId, -1, 0, sum);
                        cm_SendMoneyDelta(me->netId, -4);
                        h = cm_SendSetField(owner->netId, 0x1c8, 4, LC_POWER_EMBEZZLED, 0);
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        txt_Format(text, 0x1dc3, sum);
                        sv_MessageBox(text, 0);
                        ret = 0x90;
                        sv_PopRequest = 1;
                    }
                } else if (d2_ClickedId == 1) {
                    sv_PopRequest = 1;
                }
            }
            if (dm_L.rup)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_MisappropriationWindow, 0x657c7) != 0);
        d2_CloseView(win);
    }
    return ret;
}
/* 0x5caab0, the bewitch window "privillegien\becircen" (the amount menu callback of 0x5cacd0, with the picked
   character ch). A ch of the player's own sex gets the text 0x1db1 (with 0x1db3 for a woman, 0x1db2 for a man)
   and 0. Else the window with the text 0x1db4 (ch twice) and a slider 1 .. 5; its first button, when the player
   has the slider's amount (sv_CheckMoney), pays it for ch's relation (ai_RelationGain), takes it from the
   player (cm_SendMoneyDelta), sends the relation change to ch (cm_SendRelations), tells it (0x1db5, ch twice)
   and closes with 1; the second button or a right click close it. Returns 1 once ch was bewitched, else 0 */

int ai_BewitchWindow(AICHARREC *ch)
{
    char text[0x100];
    int win;
    int slider;
    int amount;
    int gain;
    int done;

    done = 0;
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == ch->sex) {
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
            txt_Format(text, 0x1db1, 0x1db2);
        else
            txt_Format(text, 0x1db1, 0x1db3);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "privillegien\\becircen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    slider = d2_GetWinObj(win, 1, io_PrintText(0x1db4, ch->id, ch->id));
    d2_SetObjValue4(slider, 1, 5, 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                amount = d2_GetObjValue(slider);
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], amount) != 0) {
                    gain = ai_RelationGain(ai_Chars[ai_PlayerId], ch, amount);
                    cm_SendMoneyDelta(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -amount);
                    cm_SendRelations(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ch->netId, gain, 1);
                    txt_Format(text, 0x1db5, ch->id, ch->id);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 4;
                    done = 1;
                }
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 2;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BewitchWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5cacd0, the bewitch order's ask (ord_BewitchOrder, kinds 3 / 4): a player (kind 6 / 7) picks the character in
   the amount menu (the partner filter 0x400, gypsyFilter, the text 0x1dae with 0x1daf / 0x1db0 by me's sex) whose
   callback ai_BewitchWindow does the rest: 1 when it bewitched someone, else 0. The AI pays m->amount (1 .. 5, not
   above its money) for the relation with the character m->target (ai_RelationGain), sends the payment and the
   relation change: 0x10; a wrong amount or an unknown target: 0x60 */

int ai_BewitchOrder(AICHARREC *me, LCORDMSG *m)
{
    AMOUNTCFG cfg;
    char text[0x40];
    AICHARREC *target;
    int gain;
    int ret;
    int n;

    ret = 0;
    if (me->kind == 6 || me->kind == 7) {
        memset(&cfg, 0, sizeof(cfg));
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.classMask[0] = gypsyFilterWords[0];
        cfg.classMask[1] = gypsyFilterWords[1];
        if (me->sex == 0)
            txt_Format(text, 0x1dae, 0x1daf);
        else
            txt_Format(text, 0x1dae, 0x1db0);
        if (ai_AmountMenu(&cfg, text, 0, ai_BewitchWindow) != 0)
            ret = 1;
        return ret;
    }
    if (me->money < m->amount || m->amount < 1 || m->amount > 5)
        return 0x60;
    target = (AICHARREC *)gm_FindChar(m->target);
    if (target == 0)
        return 0x60;
    n = m->amount;
    gain = ai_RelationGain(me, target, n);
    cm_SendMoneyDelta(me->netId, -n);
    cm_SendRelations(me->netId, target->netId, gain, 1);
    return 0x10;
}
/* 0x5cae00, the counter-espionage order's ask (ord_CounterEspionageOrder, kinds 3 / 4) "privillegien\gegenspionage":
   it costs me 4. Every running espionage event 0x18 on me (id4 is me's netId) ends now, and me sends an espionage
   message 0x18 of its own on the spy (the event's character). The AI (not kind 6 / 7) needs the 4 (else 0x20), also
   stamps the event's stopped18 and returns 2, | 0x10 when it caught a spy. A player gets the window (the text 0x1db7
   with the 4, two buttons): the first, when the player has the 4 (sv_CheckMoney), does it and tells the spies'
   names (0x1db8, then 0x1db9 per spy, or 0x1dba when there were none); either button or a right click close it,
   then 2 */

int ai_CounterEspionage(AICHARREC *me, LCORDMSG *m)
{
    AIMSG msg;
    char text[0x200];
    char *p;
    AIJOB *ev;
    int win;
    int t;
    int ok;
    int cancel;
    int caught;
    int ret;

    ret = 2;
    if (me->kind != 6 && me->kind != 7) {
        if (me->money < 4)
            return 0x20;
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x18;
        msg.to = me->netId;
        msg.fromId = me->netId;
        msg.flags = 2;
        msg.time = ai_Time;
        cm_SendMoneyDelta(me->netId, -4);
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->id4 == me->netId) {
                ret |= 0x10;
                ev->end = ai_Time;
                ev->stopped18 = ai_Time;
                cm_SendExSetHEI(ev->stage, ev);
                msg.espionage.target = ((AICHARREC *)ai_Chars)[ev->chr].netId;
                cm_SendLongMessage((char *)&msg);
            }
        }
        return ret;
    }
    win = d2_OpenForm(0, 0, "privillegien\\gegenspionage");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1db7, 4);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 4) != 0) {
                    caught = 0;
                    txt_Format(text, 0x1db8);
                    p = text + strlen(text);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x18;
                    msg.to = me->netId;
                    msg.fromId = me->netId;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    cm_SendMoneyDelta(me->netId, -4);
                    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                        if (ev->id4 == me->netId) {
                            caught |= 1;
                            ev->end = ai_Time;
                            cm_SendExSetHEI(0, ev);
                            msg.espionage.target = ((AICHARREC *)ai_Chars)[ev->chr].netId;
                            cm_SendLongMessage((char *)&msg);
                            txt_Format(p, 0x1db9, ev->chr);
                            p = text + strlen(text);
                        }
                    }
                    if (!caught)
                        txt_Format(text, 0x1dba);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_CounterEspionage, 0x657c7) != 0);
    d2_CloseView(win);
    return 2;
}

int amt_SendOfficeSwap();

/* 0x5cb1e0, the counter-espionage office swap (64 B): amt_SendOfficeSwap (0x4e6ba0) sends the requester's
   request for the two characters; -1 returns 0, another requester returns 1; a player (kind 6) waits on the
   message slot 0x4c3ae0 with the pump 0x52fe00 and returns whether the answer's status is not 2. */
int ai_SwapSeats(char *requester, char *b, char *c)
{
    int h;

    h = amt_SendOfficeSwap(requester, b, c);
    if (h != -1) {
        if (((AICHARREC *)requester)->kind == 6) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
            return cm_GetSlotState(h) != 2;
        }
        return 1;
    }
    return 0;
}
/* 0x5cb250, the seat swap order's ask (ord_SeatSwapOrder, kinds 3 / 4) "privillegien\sessel_vertauschen": the office
   holder (me for kind 3, m->partner for kind 4; none: 0x60) of the office 0xf / 0x15 / 0x1b swaps the two offices below
   it (0xd / 0xe, 0x13 / 0x14, 0x19 / 0x1a; another office: 0x20), whose holders it finds in ai_Chars. The AI needs 6 in
   money and both holders (else 0x20) and returns 0x10 when ai_SwapSeats went through, else 0x20. A player gets the
   text 0x1dcb when a seat is empty (0), else the window (0x1dc9: the 6, the two holders); the first button, when the
   player has the 6 (sv_CheckMoney), swaps them and tells 0x1dca (or 0x1dcb when it failed); either button or a right
   click close it, then 1 */

int ai_SeatSwapWindow(AICHARREC *me, LCORDMSG *m)
{
    char text[0x200];
    AICHARREC *first;
    AICHARREC *second;
    AICHARREC *c;
    int office;
    int k1;
    int k2;
    int n;
    int win;
    int t;
    int ok;
    int cancel;

    second = 0;
    n = 2;
    first = 0;
    if (m->kind == 3) {
        office = me->officeKind;
    } else if (m->kind == 4 && m->partner != 0) {
        office = m->partner->officeKind;
    } else {
        return 0x60;
    }
    if (office == 0xf) {
        k2 = 0xe;
        k1 = 0xd;
    } else if (office == 0x15) {
        k2 = 0x14;
        k1 = 0x13;
    } else if (office == 0x1b) {
        k2 = 0x1a;
        k1 = 0x19;
    } else {
        return 0x20;
    }
    for (c = (AICHARREC *)ai_Chars; (int)c < (int)((AICHARREC *)ai_Chars + 0x300); c++) {
        if (n == 0)
            break;
        if (c->officeKind == k1) {
            first = c;
            n--;
        } else if (c->officeKind == k2) {
            second = c;
            n--;
        }
    }
    if (me->kind != 6 && me->kind != 7) {
        if (me->money < 6 || n > 0)
            return 0x20;
        return ai_SwapSeats((char *)me, (char *)second, (char *)first) ? 0x10 : 0x20;
    }
    if (n > 0) {
        txt_Format(text, "$Z%s", 0x1dcb);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "privillegien\\sessel_vertauschen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1dc9, 6, second->id, first->id);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 6) != 0) {
                    if (ai_SwapSeats((char *)me, (char *)second, (char *)first) != 0)
                        txt_Format(text, 0x1dca);
                    else
                        txt_Format(text, "$Z%s", 0x1dcb);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_CounterEspionage, 0x657c7) != 0);   /* the original passes the counter-espionage window here */
    d2_CloseView(win);
    return 1;
}
/* 0x5cb4f0, the hatred order's ask (ord_HatredOrder, kinds 3 / 4) "privillegien\hass_erzeugen": me sets two
   characters against each other. The price is 2 % of their two worths (gm_GetWorth); each one's relation to the other
   drops by a random 3 .. 32 less its opinion of the other less 0x7f (cm_SendRelations both ways), me pays the price
   to the bank (cm_SendMoveGoods) and 4 (cm_SendMoneyDelta), and a player among the two gets the news 0x1dd3 naming
   the other ("_NACHRICHTEN_HS_20"). The AI (not kind 6) names them in m->target / m->target2: 0x10, or 0x20 when it
   has less than 4, one is unknown or its goods are worth less than the price. A player (kind 6) picks them in two
   amount menus (texts 0x1dcf, 0x1dd0; none: 0), then the window (0x1dd1: the 4, the price, the two) whose first
   button, when the player has the 4 (sv_CheckMoney) and the price (sv_CheckGoodAmount), does it and tells 0x1dd2;
   either button or a right click close it. Returns 1 once it was done, else 0 */

int ai_HatredWindow(AICHARREC *me, LCORDMSG *m)
{
    int done;
    int ok;
    int price;
    int cancel;
    int win;
    AMOUNTCFG cfg;
    char text[0x100];
    AICHARREC *a;
    AICHARREC *b;
    int t;

    if (me->kind != 6) {
        int r1, r2, o1, o2;
        int price = 0;      /* @original: the initializer (no code) moves VC6's first-reference tie key so that
                               both worth sums call gm_GetWorth(b) first, as the original (deep s29 lever) */

        if (me->money < 4)
            return 0x20;
        a = (AICHARREC *)gm_FindChar(m->target);
        if (a == 0)
            return 0x20;
        b = (AICHARREC *)gm_FindChar(m->target2);
        if (b == 0)
            return 0x20;
        price = (int)((float)(gm_GetWorth(b->id) + gm_GetWorth(a->id)) * 0.02f);
        if (gm_GetGoodsValue(me) < price)
            return 0x20;
        r1 = util_RandomU(0x1e) + 3;
        r2 = util_RandomU(0x1e) + 3;
        o1 = gm_Opinion(a->id, b->id);
        o2 = gm_Opinion(b->id, a->id);
        cm_SendRelations(a->netId, b->netId, r1 - o1 - 0x7f, 1);
        cm_SendRelations(b->netId, a->netId, r2 - o2 - 0x7f, 1);
        cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
        cm_SendMoneyDelta(me->netId, -4);
        if (a->kind == 6 || a->kind == 7) {
            txt_Format(text, 0x1dd3, b->id, b->id);
            he_ShowNews(a->netId, a->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
        }
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(text, 0x1dd3, a->id, a->id);
            he_ShowNews(b->netId, b->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
        }
        return 0x10;
    }
    done = 0;
    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.classMask[0] = gypsyFilterWords[0];
    cfg.classMask[1] = gypsyFilterWords[1];
    txt_Format(text, 0x1dcf);
    a = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    if (a == 0)
        return 0;
    txt_Format(text, 0x1dd0, a->id);
    b = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    if (b == 0)
        return 0;
    price = (int)((float)(gm_GetWorth(b->id) + gm_GetWorth(a->id)) * 0.02f);
    win = d2_OpenForm(0, 0, "privillegien\\hass_erzeugen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1dd1, 4, price, a->id, b->id);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 4) != 0 && sv_CheckGoodAmountB((char *)me, price, ai_OwnPlayer) != 0) {
                    int r1, r2, o1, o2;

                    r1 = util_RandomU(0x1e) + 3;
                    r2 = util_RandomU(0x1e) + 3;
                    o1 = gm_Opinion(a->id, b->id);
                    o2 = gm_Opinion(b->id, a->id);
                    cm_SendRelations(a->netId, b->netId, r1 - o1 - 0x7f, 1);
                    cm_SendRelations(b->netId, a->netId, r2 - o2 - 0x7f, 1);
                    cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
                    cm_SendMoneyDelta(me->netId, -4);
                    txt_Format(text, 0x1dd2, a->id, b->id);
                    sv_MessageBox(text, 0);
                    if (a->kind == 6 || a->kind == 7) {
                        txt_Format(text, 0x1dd3, b->id, b->id);
                        he_ShowNews(a->netId, a->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
                    }
                    if (b->kind == 6 || b->kind == 7) {
                        txt_Format(text, 0x1dd3, a->id, a->id);
                        he_ShowNews(b->netId, b->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
                    }
                    sv_PopRequest = 1;
                    done = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_HatredWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return done;
}
#define LC_POWER_HEARING 0x8000  /* AICHARREC.powerFlags: the office's hearing used (ai_HearingWindow) */
/* 0x5cba90, the hearing order's ask (ord_HearingOrder, kinds 3 / 4) "privillegien\verhoer": me summons a character
   to a hearing, once per office (LC_POWER_HEARING not yet set): me pays 2 and the character 3 (cm_SendMoneyDelta),
   the power is marked used on me's powerFlags (cm_SendSetField), and a player character summoned gets the news 0x1dda (me, the 3;
   "_NACHRICHTEN_HS_20"). The AI (not kind 6) names it in m->target: 0x10, 0x20 without the 2 or with the power used,
   0x60 for an unknown character. A player who used it gets the text 0x1ce0 and 0x81; else the amount menu (0x1dd7; none:
   2), then the window (0x1dd8) whose first button, when the player has the 2 (sv_CheckMoney), does it, waits for the
   field message, tells 0x1dd9 and returns 0x81; the second button or a right click close it with 2 */

int ai_HearingWindow(AICHARREC *me, LCORDMSG *m)
{
    int ret;
    int cancel;
    int ok;
    AMOUNTCFG cfg;
    char text[0x200];
    AICHARREC *target;
    int win;
    int t;
    int h;

    if (me->kind != 6) {
        if (me->money < 2)
            return 0x20;
        if (me->powerFlags & LC_POWER_HEARING)
            return 0x20;
        target = (AICHARREC *)gm_FindChar(m->target);
        if (target == 0)
            return 0x60;
        cm_SendMoneyDelta(me->netId, -2);
        cm_SendMoneyDelta(target->netId, -3);
        cm_SendSetField(me->netId, 0x1c8, 4, LC_POWER_HEARING, 0);
        if (target->kind == 6 || target->kind == 7) {
            txt_Format(text, 0x1dda, me->id, 3);
            he_ShowNews(target->netId, target->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
        }
        return 0x10;
    }
    ret = 2;
    memset(&cfg, 0, sizeof(cfg));
    if (me->powerFlags & LC_POWER_HEARING) {
        txt_Format(text, 0x1ce0);
        sv_MessageBox(text, 0);
        return 0x81;
    }
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(text, 0x1dd7);
    target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    if (target == 0)
        return 2;
    win = d2_OpenForm(0, 0, "privillegien\\verhoer");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1dd8, 2, target->id, target->id, 3, 2);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 2) != 0) {
                    cm_SendMoneyDelta(me->netId, -2);
                    cm_SendMoneyDelta(target->netId, -3);
                    h = cm_SendSetField(me->netId, 0x1c8, 4, LC_POWER_HEARING, 0);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    txt_Format(text, 0x1dd9, target->id, target->id, 3, target->id, 3);
                    sv_MessageBox(text, 0);
                    if (target->kind == 6 || target->kind == 7) {
                        txt_Format(text, 0x1dda, me->id, 3);
                        he_ShowNews(target->netId, target->netId, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
                    }
                    sv_PopRequest = 1;
                    ret = 0x81;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_HearingWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
#define LC_POWER_EXPEL 0x80  /* AICHARREC.powerFlags: the office's worker expulsion used (ai_ExpelWorkerWindow) */
/* 0x5cbe00, the expel-worker order's ask (ord_ExpelWorkerOrder, kinds 3 / 4) "privillegien\arbeiter_ausweisen": the
   office holder (me for kind 3, m->partner for kind 4; none: 0x60), once per office (LC_POWER_EXPEL not yet set), has
   a worker of a building removed: me pays 5, the power is marked used, the building's owner, when a player, gets the
   news 0x1ddf (me, the building, the worker; "_NACHRICHTEN_HS_20"), the player inherits the worker's estate
   (gm_InheritEstate) and the worker is killed (cm_SendKillCharacter). The AI (not kind 6) names the worker in
   m->target and the building in m->target2: 0x10, 0x20 without the 5, 0x60 when the power is used or one is
   unknown (its news goes to the owner's id, not its netId, as in the original). A player who used the power gets
   0x1ce0 and 0x81; else the map pick of a building (0x1ddc; none or no owner: 0), the window (0x1ddd: the 5, the
   owner, the building) whose first button, when the player has the 5 (sv_CheckMoney), does it for a kind 0 worker
   in that building from a random start (none: only the payment) and tells 0x1dde: 0x81; the second button or a
   right click close it with 0 */

int ai_ExpelWorkerWindow(AICHARREC *me, LCORDMSG *m)
{
    AICHARREC *holder;
    int cancel;
    AIBLD *bld;
    int ok;
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x200];
    AICHARREC *target;
    AICHARREC *owner;
    AICHARREC *worker;
    int ret;
    int win;
    int t;
    int h;
    int i;
    int n;

    if (me->kind != 6) {
        if (me->money < 5)
            return 0x20;
        /* @original the AI branch tests the bit as a signed int (0x80 in cl, test byte ptr [..],cl); the
           unsigned field gives the sign test the player branch has */
        if (m->kind == 3 && !((int)me->powerFlags & LC_POWER_EXPEL)) {
            holder = me;
        } else {
            if (m->kind != 4)
                return 0x60;
            holder = m->partner;
            if (holder == 0 || ((int)holder->powerFlags & LC_POWER_EXPEL))
                return 0x60;
        }
        target = (AICHARREC *)gm_FindChar(m->target);
        if (target == 0)
            return 0x60;
        bld = (AIBLD *)gm_BuildQuery(1, 1, m->target2);
        if (bld == 0)
            return 0x60;
        cm_SendMoneyDelta(me->netId, -5);
        cm_SendSetField(holder->netId, 0x1c8, 4, LC_POWER_EXPEL, 0);
        if (((AICHARREC *)ai_Chars)[bld->owner].kind == 6 || ((AICHARREC *)ai_Chars)[bld->owner].kind == 7) {
            txt_Format(news, 0x1ddf, me->id, bld, target->id);
            he_ShowNews(bld->owner, bld->owner, news, 0, 0x5fb, "_NACHRICHTEN_HS_20");
        }
        gm_InheritEstate((struct ORDCHAR *)target, (struct ORDCHAR *)ai_PlayerChar);
        cm_SendKillCharacter(target->netId, 1);
        return 0x10;
    }
    memset(&cfg, 0, sizeof(cfg));
    if (m->kind == 3) {
        holder = me;
    } else if (m->kind == 4 && m->partner != 0) {
        holder = m->partner;
    } else {
        return 0x60;
    }
    if (holder->powerFlags & LC_POWER_EXPEL) {
        txt_Format(text, 0x1ce0);
        sv_MessageBox(text, 0);
        return 0x81;
    }
    ret = 0;
    cfg.mode = 0x410;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    cfg.flags = 0;
    txt_Format(text, 0x1ddc);
    bld = (AIBLD *)ai_MapPick(1, &cfg, text, 0, 0);
    if (bld == 0)
        return 0;
    if (bld->owner == 0xffff)
        return 0;
    owner = &((AICHARREC *)ai_Chars)[bld->owner];
    win = d2_OpenForm(0, 0, "privillegien\\arbeiter_ausweisen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1ddd, 5, owner->id, bld);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 5) != 0) {
                    cm_SendMoneyDelta(me->netId, -5);
                    h = cm_SendSetField(holder->netId, 0x1c8, 4, LC_POWER_EXPEL, 0);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    txt_Format(news, 0x1dde);
                    sv_MessageBox(news, 0);
                    worker = 0;
                    i = util_RandomU(0x300);
                    for (n = 0x300; n != 0; n--, i = (i + 1) % 0x300) {
                        if (((AICHARREC *)ai_Chars)[i].id != 0xffff && ((AICHARREC *)ai_Chars)[i].bld == bld &&
                            ((AICHARREC *)ai_Chars)[i].kind == 0) {
                            worker = &((AICHARREC *)ai_Chars)[i];
                            break;
                        }
                    }
                    if (worker != 0) {
                        if (owner->kind == 6 || owner->kind == 7) {
                            txt_Format(news, 0x1ddf, me->id, bld, worker->id);
                            he_ShowNews(owner->netId, owner->netId, news, 0, 0x5fb, "_NACHRICHTEN_HS_20");
                        }
                        gm_InheritEstate((struct ORDCHAR *)worker, (struct ORDCHAR *)ai_PlayerChar);
                        cm_SendKillCharacter(worker->netId, 1);
                    }
                    ret = 0x81;
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_ExpelWorkerWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5cc2b0, the peace order's ask (ord_PeaceOrder, kinds 3 / 4) "privillegien\frieden_stiften": me reconciles two
   characters. Each one's relation to the other rises by a random 15 .. 54 (cm_SendRelations both ways), me pays 3
   (cm_SendMoneyDelta), and a player among the two gets the news 0x1de8 naming the other and me. The AI (not kind 6)
   names them in m->target / m->target2: 0x10, or 0x20 when it has less than 3, 0x60 when one is unknown. A player
   (kind 6) picks them in two amount menus (texts 0x1de4, 0x1de5; none: 0), then the window (0x1de6: the 3, the two)
   whose first button, when the player has the 3 (sv_CheckMoney), does it and tells 0x1de7; either button or a right
   click close it. Returns 1 once it was done, else 0 */

int ai_PeaceWindow(AICHARREC *me, LCORDMSG *m)
{
    int done;
    int ok;
    int cancel;
    int win;
    AMOUNTCFG cfg;
    char text[0x200];
    AICHARREC *a;
    AICHARREC *b;
    int r1;
    int r2;
    int t;

    if (me->kind != 6) {
        r1 = util_RandomU(0x28) + 0xf;
        r2 = util_RandomU(0x28) + 0xf;
        if (me->money < 3)
            return 0x20;
        a = (AICHARREC *)gm_FindChar(m->target);
        if (a == 0)
            return 0x60;
        b = (AICHARREC *)gm_FindChar(m->target2);
        if (b == 0)
            return 0x60;
        cm_SendRelations(a->netId, b->netId, r1, 1);
        cm_SendRelations(b->netId, a->netId, r2, 1);
        cm_SendMoneyDelta(me->netId, -3);
        if (a->kind == 6 || a->kind == 7) {
            txt_Format(text, 0x1de8, b->id, me->id);
            he_ShowNews(a->netId, a->netId, text, 0, 0x5ec, ch_EmptyName);
        }
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(text, 0x1de8, a->id, me->id);
            he_ShowNews(b->netId, b->netId, text, 0, 0x5ec, ch_EmptyName);
        }
        return 0x10;
    }
    done = 0;
    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(text, 0x1de4);
    a = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    if (a == 0)
        return 0;
    txt_Format(text, 0x1de5, a->id);
    b = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    if (b == 0)
        return 0;
    win = d2_OpenForm(0, 0, "privillegien\\frieden_stiften");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1de6, 3, a->id, b->id);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                if (sv_CheckMoney(me, 3) != 0) {
                    r1 = util_RandomU(0x28) + 0xf;
                    r2 = util_RandomU(0x28) + 0xf;
                    cm_SendRelations(a->netId, b->netId, r1, 1);
                    cm_SendRelations(b->netId, a->netId, r2, 1);
                    cm_SendMoneyDelta(me->netId, -3);
                    txt_Format(text, 0x1de7, a->id, b->id);
                    sv_MessageBox(text, 0);
                    if (a->kind == 6 || a->kind == 7) {
                        txt_Format(text, 0x1de8, b->id, me->id);
                        he_ShowNews(a->netId, a->netId, text, 0, 0x5ec, 0);
                    }
                    if (b->kind == 6 || b->kind == 7) {
                        txt_Format(text, 0x1de8, a->id, me->id);
                        he_ShowNews(b->netId, b->netId, text, 0, 0x5ec, 0);
                    }
                    sv_PopRequest = 1;
                    done = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PeaceWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5cc700, the conversion order's ask (ord_ConvertOrder, kinds 3 / 4) "privillegien\bekehren": me preaches to the
   town. The price is 6 % of me's worth (gm_GetWorth); me pays it to the bank (cm_SendMoveGoods) and 6
   (cm_SendMoneyDelta), and every living character (kind below 10) of the other faith whose skills[4] plus a random
   0 .. 125 stays below me's skills[4] takes me's faith (field 0xc). The AI (not kind 6) leaves the players (kinds
   6 / 7) alone: 0x10, or 0x20 when it has less than 6 or its goods are worth less than the price. A player (kind 6)
   gets the window (0x1de1: the 6, the price) whose first button, when the player has the 6 (sv_CheckMoney) and the
   price (sv_CheckGoodAmount), does it and tells 0x1de2 (the number converted); either button or a right click
   close it. Returns 1 once it was done, else 0 */

int ai_ConvertWindow(AICHARREC *me, LCORDMSG *m)
{
    int price;
    int done;
    int win;
    int ok;
    int cancel;
    int n;
    int i;
    int t;
    AICHARREC *c;
    char text[0x100];

    if (me->kind != 6) {
        int faith;
        unsigned char skill;

        skill = me->skills[4];
        faith = me->faith;
        if (me->money < 6)
            return 0x20;
        price = (int)((float)gm_GetWorth(me->id) * 0.06f);
        if (gm_GetGoodsValue(me) < price)
            return 0x20;
        cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
        cm_SendMoneyDelta(me->netId, -6);
        for (i = 0; i < 768; i++) {
            c = &((AICHARREC *)ai_Chars)[i];
            if (c->id != 0xffff && c->kind < 0xa && c->kind != 6 && c->kind != 7 && c->faith != me->faith &&
                skill > (int)(util_RandomU(0x7e) + c->skills[4])) {
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldDelta(1, 1, 0xc, &faith);
                cm_SendFieldDeltas();
            }
        }
        return 0x10;
    }
    done = 0;
    price = (int)((float)gm_GetWorth(me->id) * 0.06f);
    win = d2_OpenForm(0, 0, "privillegien\\bekehren");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1de1, 6, price);
    ok = d2_GetWinObj(win, 1, t);
    cancel = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                int faith;
                unsigned char skill;

                n = 0;
                skill = me->skills[4];
                faith = me->faith;
                if (sv_CheckMoney(me, 6) != 0 && sv_CheckGoodAmountB((char *)me, price, ai_OwnPlayer) != 0) {
                    cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
                    cm_SendMoneyDelta(me->netId, -6);
                    /* @original: the loop's strength-reduced pointer is based on c->kind (ai_Chars+2: cmp byte
                       [esi],0xa, faith at [esi+0xa]), which VC6 picks only with a second kind reference, as in
                       the AI loop above; the original had a kind test VC6 folds away (written twice here) */
                    for (i = 0; i < 768; i++) {
                        c = &((AICHARREC *)ai_Chars)[i];
                        if (c->id != 0xffff && c->kind < 0xa && c->kind < 0xa && c->faith != me->faith &&
                            skill > (int)(util_RandomU(0x7e) + c->skills[4])) {
                            n++;
                            cm_BeginFieldBlock((char *)c, c->netId);
                            cm_AddFieldDelta(1, 1, 0xc, &faith);
                            cm_SendFieldDeltas();
                        }
                    }
                    txt_Format(text, 0x1de2, n);
                    sv_MessageBox(text, 0);
                    done = 1;
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_ConvertWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5cca40, the penance order's ask (ord_PenanceOrder, kinds 3 / 4) "privillegien\abbitte": me buys off its crimes,
   one per money unit (1 .. 9): it pays them (cm_SendMoneyDelta) and its oldest open crimes are retired
   (gv_RetireCrimesOf). The AI (not kind 6) names the number in m->amount: 0, or 0x60 when it is outside 1 .. 9 or
   above its money. A player (kind 6) sets it on the window's slider (0x1dea) whose first button, when the player has
   the money (sv_CheckMoney), does it and tells 0x1deb (the number retired); either button or a right click close
   it. Returns 1 once it was done, else 0 */

int ai_PenanceWindow(AICHARREC *me, LCORDMSG *m)
{
    char text[0x100];
    int win;
    int ok;
    int cancel;
    int slider;
    int t;
    int n;
    int done;

    if (me->kind != 6) {
        if (m->amount < 1 || m->amount > 9 || m->amount > me->money)
            return 0x60;
        gv_RetireCrimesOf(me->netId, m->amount);
        cm_SendMoneyDelta(me->netId, -m->amount);
    }
    if (me->kind == 6) {
        done = 0;
        win = d2_OpenForm(0, 0, "privillegien\\abbitte");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        t = io_PrintText(0x1dea);
        ok = d2_GetWinObj(win, 1, t + 1);
        cancel = d2_GetWinObj(win, 1, t + 2);
        slider = d2_GetWinObj(win, 1, t);
        d2_SetObjValue4(slider, 1, 9, 1);
        d2_ClickedId = -1;
        do {
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == ok) {
                    n = d2_GetObjValue(slider);
                    if (sv_CheckMoney(me, n) != 0) {
                        cm_SendMoneyDelta(me->netId, -n);
                        txt_Format(text, 0x1deb, gv_RetireCrimesOf(me->netId, n));
                        sv_MessageBox(text, 0);
                        done = 1;
                        sv_PopRequest = 1;
                    }
                } else if (d2_PointerObj == cancel) {
                    sv_PopRequest = 1;
                }
            }
            if (dm_L.rup)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_PenanceWindow, 0x657c7) != 0);
        d2_CloseView(win);
        return done;
    }
    return 0;
}
int ai_FindPartnersRandomWalk(struct AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id); /* 0x4a5b50 (am.h, not included) */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, int d, unsigned char e);  /* 0x4e6680 (amt.h) */
#define LC_MIRACLE_PICK_FLAGS3 0x400800  /* ai_FindPartnersRandomWalk flags of the miracle that takes an office (bits not read) */
#define LC_MIRACLE_PICK_FLAGS4 0x300000  /* ai_FindPartnersRandomWalk flags of the miracle that raises a skill (bits not read) */
/* 0x5ccc10, a miracle (ai_MiracleWindow, the miracle order's ask, rolls which = 0 .. 5): it builds the news title 0x1df7
   (me) and tells every player (kinds 6 / 7) what happened (he_ShowNews 0x5fe; 1 tells each character on its own, 0x601).
   0: a building of the kind 0x80 found by ai_PickActionBuilding: its owner gives 2 .. 4 % of his worth
      (cm_SendMoveGoodsUnchecked, the text 0x1df8).
   1: the sick: every character whose health (v024) is below -20 is healed to half of it, with a chance of one in two
      (the field 0x24; the texts 0x1df9 .. 0x1dfb: the healed and the others hear their own news).
   2: a building found by ai_PickActionBuilding (the kinds 0x1a07c43b0) loses 10 .. 25 of its level, if it keeps some
      (the field 0x5c, the text 0x1dfc).
   3: a character of the random walk loses its office (amt_SendOfficeMessage 4, the text 0x1dfd).
   4: a character of the random walk gains 0 .. 20 in one of its skills below 231 (cm_AddFieldValues, the text 0x1dfe
      names the skill 0x16b7 + it).
   5: the poorest of the first 8 players (kinds 5 .. 7) gets 3 .. 7 % of his worth for a building found by
      ai_PickActionBuilding (the kinds 0x1b07d43b4; the text 0x1dff).
   @deep s48: kept (133 -> 77 ins): bld one function-scope local shared by the cases 0 / 2 / 5 and out one
   shared by 0 / 2 (case 5 keeps its own block-local out): c2frame then packs the case locals into the
   original's slots (0x10 n / worth0 / id3 / id4 / the case-5 fild temp, 0x14 the case-0 fild temp / v /
   flags3 / flags4 / worth5, 0x18 bld, 0x1c out, 0x20 case-5 out, 0x24 kind / chars, 0x44 office); slots 6
   left. Left: the scratch rotation: the original starts the cases 1 and 3 at eax (lea eax text / lea eax
   &id) where ours continues the previous case's rotation (ecx / edx), and case 5's chars loop / GetWorth
   push are two steps off; case 2 and 4 agree. flip.py: case 1 flips only by deleting case 0's tail or
   swapping its two last calls (meaning); case 3 flips by deleting / duplicating case 2 statements and case
   4 ones (m436 del gain = util_Random, m510 del cm_AddFieldValues) - so the rotation is not simply
   sequential in source order. No lever: the tail written in each case (not cross-jumped, 121+), a switch
   (jump table), kinds as __int64 / QuadPart / literal / function scope, worth function scope, the
   &ai_PlayerSlots argument spellings, climb 2 rounds. Also left: case 4's skill loop exit test (the
   original dec ecx; test ecx,ecx), and the common tail's xor eax,eax placement (strcat inline). prior:
   s47: read the diff again. Two independent things are left. (1) The frame packing (frame > 0x80: count *
   1000 / size order) puts the case locals into other slots of the shared dead-parameter area: case 0 bld
   at +0x14 / the rand fild temp at +0x30 where the original has +0x18 / +0x2c, case 1 v at +0x10 against
   +0x14, case 2 out at +0x10 against +0x1c (dcp frame: the original packs me / worth / v / out / flags and
   which / bld / n across arg+0..0xc in another order); c2frame --names would give the counts each needs.
   (2) From case 1 on, the scratch registers are one step behind (ours lea ecx / edx for the first
   txt_Format / sprintf arguments, the original eax / ecx), so one temporary more is handed out in ours
   before case 1 (case 0's tail or the 0x1df7 title). No variant round this session. */

void ai_Miracle(AICHARREC *me, int which)
{
    char text[0x400];
    char title[0x400];
    char line[0x400];
    char healed[0x400];
    char unhealed[0x400];
    AICHARREC *c;
    AIBLD *bld;
    int out;
    int price;
    int i;

    if (which < 0 || which > 5)
        return;
    txt_Format(title, 0x1df7, me->id);
    if (which == 0) {
        int worth;
        LARGE_INTEGER kinds;

        kinds.LowPart = 0x80;
        kinds.HighPart = 0;
        if (ai_PickActionBuilding((struct AIPER *)me, 2, kinds.QuadPart, 0.0f, 100.0f, &out) == 0)
            return;
        gm_FindById((int *)&bld, 0, 0, out);
        c = &((AICHARREC *)ai_Chars)[bld->owner];
        worth = gm_GetWorth(c->id);
        price = (int)((util_RandomU(3) + 2) * (worth * 0.01));
        cm_SendMoveGoodsUnchecked(c->netId, -1, ai_OwnPlayer, price);
        txt_Format(text, 0x1df8, c->id, bld, c->id, price, &ai_PlayerSlots[ai_OwnPlayer][0x20]);
    } else if (which == 1) {
        int v;

        txt_Format(text, 0x1df9);
        txt_Format(line, 0x1dfa);
        sprintf(healed, "%s%s%s", title, text, line);
        txt_Format(line, 0x1dfb);
        sprintf(unhealed, "%s%s", title, text, line);  /* sic: the original's format drops the line */
        strcat(title, text);
        for (i = 0; i < 768; i++) {
            c = &((AICHARREC *)ai_Chars)[i];
            if (c->id == 0xffff)
                continue;
            if (c->v024 < -20) {
                if (util_RandomU(2) != 0) {
                    v = abs(c->v024 / 2);
                    cm_BeginFieldBlock((char *)c, c->netId);
                    cm_AddFieldDelta(4, 1, 0x24, &v);
                    cm_SendFieldDeltas();
                    if (c->kind == 6 || c->kind == 7)
                        he_ShowNews(c->netId, -1, healed, 1, 0x601, 0);
                } else if (c->kind == 6 || c->kind == 7) {
                    he_ShowNews(c->netId, -1, unhealed, 1, 0x601, 0);
                }
            } else if (c->kind == 6 || c->kind == 7) {
                he_ShowNews(c->netId, -1, title, 1, 0x601, 0);
            }
        }
        return;
    } else if (which == 2) {
        char level;
        LARGE_INTEGER kinds;

        kinds.LowPart = 0xa07c43b0;
        kinds.HighPart = 1;
        if (ai_PickActionBuilding((struct AIPER *)me, 2, kinds.QuadPart, 0.0f, 100.0f, &out) == 0)
            return;
        gm_FindById((int *)&bld, 0, 0, out);
        c = &((AICHARREC *)ai_Chars)[bld->owner];
        level = bld->level - util_RandomU(0x10) - 10;
        if (level <= 0)
            return;
        cm_BeginFieldBlock((char *)bld, bld->id);
        cm_AddFieldDelta(1, 1, 0x5c, &level);
        cm_SendFieldDeltas();
        txt_Format(text, 0x1dfc, c->id, bld);
    } else if (which == 3) {
        int flags;
        int id;
        AMTKIND kind;
        AMTREC office;

        flags = LC_MIRACLE_PICK_FLAGS3;
        if (ai_FindPartnersRandomWalk((struct AIPER *)me, 1, &flags, 0.0f, 100.0f, (unsigned short *)&id) == 0)
            return;
        c = &((AICHARREC *)ai_Chars)[(unsigned short)id];
        if (amt_FindOffice((char *)c, &kind, &office) == 0)
            return;
        amt_SendOfficeMessage(office.idx, 0, 0, 4, 0xff);
        txt_Format(text, 0x1dfd, c->id, c->id, c->id);
    } else if (which == 4) {
        int flags;
        int id;
        char gain;
        int j;
        int skill;

        flags = LC_MIRACLE_PICK_FLAGS4;
        gain = util_Random(0x15);
        if (ai_FindPartnersRandomWalk((struct AIPER *)me, 1, &flags, 0.0f, 100.0f, (unsigned short *)&id) == 0)
            return;
        c = &((AICHARREC *)ai_Chars)[(unsigned short)id];
        skill = -1;
        i = util_RandomU(5);
        for (j = 5; j != 0; j--) {
            if (c->skills[i] < 0xe7) {
                skill = i;
                break;
            }
            i = (i + 1) % 5;
        }
        if (skill == -1)
            return;
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValues(1, 1, skill + 0x80, &gain);
        cm_SendFieldDeltas();
        txt_Format(text, 0x1dfe, c->id, c->id, skill + 0x16b7);
    } else if (which == 5) {
        AICHARREC *chars[8];
        int n;
        int worth;
        int v;
        int j;
        int out;
        LARGE_INTEGER kinds;

        n = 0;
        for (i = 0; i < 768; i++) {
            if (n >= 8)
                break;
            c = &((AICHARREC *)ai_Chars)[i];
            if (c->kind == 6 || c->kind == 7 || c->kind == 5)
                chars[n++] = c;
        }
        c = chars[0];
        worth = gm_GetWorth(c->id);
        for (j = 1; j < n; j++) {
            v = gm_GetWorth(chars[j]->id);
            if (v < worth) {
                worth = v;
                c = chars[j];
            }
        }
        kinds.LowPart = 0xb07d43b4;
        kinds.HighPart = 1;
        if (ai_PickActionBuilding((struct AIPER *)c, 4, kinds.QuadPart, 0.0f, 100.0f, &out) == 0)
            return;
        price = (int)((float)(util_RandomU(5) + 3) * worth * 0.01f);
        gm_FindById((int *)&bld, 0, 0, out);
        cm_SendMoveGoodsUnchecked(c->netId, -1, ai_OwnPlayer, price);
        txt_Format(text, 0x1dff, c->id, bld, c->id, price);
    } else {
        return;
    }
    strcat(title, text);
    for (i = 0; i < 768; i++) {
        c = &((AICHARREC *)ai_Chars)[i];
        if (c->kind == 6 || c->kind == 7)
            he_ShowNews(c->netId, -1, title, 1, 0x5fe, 0);
    }
}
/* 0x5cd2e0, the miracle order's ask (ord_MiracleOrder; 282 B): the character ch (its kind 6 / 7 at +2, its netId
   at +4, its money at +0x194). A kind 7 character returns 0; another kind: money < 4 returns 0x20, else the
   payment 0x4c5f60 (-4) and a random miracle ai_Miracle (the character, 0x42ec00 (6) & 0xffff), return 0x10.
   Kind 6: the window "privillegien\wunder", 0x476f80 / 0x475600 / 0x475f70 with the caption 0x515960
   (0x1df6, 4); click 0 (0x520f90 (4)) pays 0x4c5f60 (-4) and runs ai_Miracle (0x42ec00 (6) & 0xffff), return 1;
   click 1 sets 0x6f22a0; the pump 0x5309a0 (this, 0x657c7) and the end 0x4758f0. The second argument m is not
   read */

int ai_MiracleWindow(void *chp, int *m)
{
    AICHARREC *ch;
    int win;
    int ret;

    ch = (AICHARREC *)chp;

    if (ch->kind != 6 && ch->kind != 7) {
        if (ch->money < 4)
            return 0x20;
        cm_SendMoneyDelta(ch->netId, 0xfffffffc);
        ai_Miracle(ch, util_Random(6) & 0xffff);
        return 0x10;
    }
    if (ch->kind == 6) {
        ret = 0;
        win = d2_OpenForm(0, 0, "privillegien\\wunder");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        io_PrintText(0x1df6, 4);
        d2_ClickedId = -1;
        do {
            if (d2_ClickedId != -1) {
                if (d2_ClickedId == 0) {
                    if (sv_CheckMoney((char *)ch, 4) != 0) {
                        cm_SendMoneyDelta(ch->netId, 0xfffffffc);
                        ai_Miracle(ch, util_Random(6) & 0xffff);
                        ret = 1;
                        sv_PopRequest = 1;
                    }
                } else if (d2_ClickedId == 1) {
                    sv_PopRequest = 1;
                }
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (sv_RunFrame(ai_MiracleWindow, 0x657c7) == 0) {
                d2_CloseView(win);
                return ret;
            }
        } while (1);
    }
    return 0;
}
void cm_SendSetJob(int id, int job);  /* 0x4c59b0 (cm.h, not included) */
/* 0x5cd400, the profession change (ord.c's handler 0x4f4ff0, a player's ask) "privillegien\CHOOSEPROFESSION2": a
   grid of 15 professions, 5 to a row, the caption 0x1d36; a profession me holds a title in is labelled with it and
   the titles left (the format txt_Ptrs[0x1d39]), me's own one is disabled. While a running event 0x26 of the player
   blocks it (he_FindEvent) a click only tells 0x15bd; else a click on another profession asks 0x1d37 for its price
   (4 for one me holds a title in, else 8; sv_CheckMoney) and on yes sets me's job to its title (or the group's first,
   gm_TitleNumber) and pays (cm_SendSetJob, cm_SendMoneyDelta). The campaign step 9 brackets the window. Returns 0x10
   once me changed, else 0 (and 0 for any other kind)
   @deep deep s35: the caption copy is strcpy (the hand while ((*d++ = *s++)) loop scheduled inc ecx after
   the store; strcpy gives the original's order): 21 -> 19 ins, s / d dropped. Left: the 0x15bd arm's
   ternary: the original computes (sex ? 0x18f : 0x130) into edx, copies it to esi and loads jobId into edx
   (index in ecx); ours keeps the index in esi and the ternary in ecx. = (same code): the operand orders
   (lc.c ai_MasterExamWindow's jobId - 1 + (..) form, + .. - 1, (.. - 1) + ..), != 0, a block / function
   int t, job / w / h / dx / i / obj / cost / group for the ternary (all fold); base = the ternary gives
   the original's registers but stores base and moves two slots (17 ins, draft); win for it: 1 ins (its
   store), but win is read after the loop. ai_MasterExamWindow (lc.c) has the same arm left with a note. */

int ai_ChangeProfessionWindow(AICHARREC *me, LCORDMSG *m)
{
    int win;
    char **name;
    int group;
    int base;
    int ret;
    int w;
    int h;
    char caption[0x200];
    char text[0x400];
    int dx;
    int i;
    int job;
    int obj;
    int cost;

    if (me->kind == 6) {
        ret = 0;
        ai_CampaignStep(9, 0, 0, 0);
        win = d2_OpenForm(0, 0, "privillegien\\CHOOSEPROFESSION2");
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        w = (d2_Wins[d2_LastWindow].w - 0x40) / 5;
        h = (d2_Wins[d2_LastWindow].h - 0x84) / 3;
        dx = (w - d2_Pics[0x5a2].w) / 2;
        io_PrintText(0x1d36);
        base = me->sex ? 0x18f : 0x130;
        name = &txt_Ptrs[0x1ef];
        for (group = 1; group <= 15; group++) {
            i = group - 1;
            job = gm_TitleJob(me->id, group);
            obj = d2_WinOpenPic((i % 5) * w + dx + 0x20, (i / 5) * h + dx + 0x64, i + 0x5a2, d2_LastWindow);
            d2_Objs[obj].interactive = 1;
            d2_Objs[obj].style = 3;
            d2_SetObjFont(obj, 0x56);
            d2_Objs[obj].centred = 1;
            d2_Objs[obj].captionX = -50;
            d2_Objs[obj].captionY = 0x44;
            if (job != 0)
                sprintf(caption, txt_Ptrs[0x1d39], gm_TitlesLeft(job), txt_Ptrs[job + base]);
            else
                caption[0] = 0;
            d2_SetCaption(obj, caption);
            d2_Objs[obj].w = 0xa4;
            if (job == me->jobId) {
                d2_SetObjEnabled(obj, 0);
            } else if (job == 0) {
                d2_Objs[obj].capfont = 0x56;
                d2_Objs[obj].picW = 0xa4;
                strcpy(d2_Objs[obj].caption, *name);
            }
            name++;
        }
        d2_ClickedId = -1;
        while (sv_RunFrame((int)ai_ChangeProfessionWindow, 0x657c7) != 0) {
            if (dm_L.rup)
                sv_PopRequest = 1;
            if (dm_L.lclick && d2_PointerObj != -1) {
                if (he_FindEvent(2, 2, ai_PlayerId, 0, 0x26) != 0) {
                    txt_Format(caption, 0x15bd, ai_PlayerId,
                               (lc_CharRecs[ai_PlayerId].sex ? 0x18f : 0x130) + lc_CharRecs[ai_PlayerId].jobId - 1);
                    sv_MessageBox(caption, 4);
                } else {
                    d2_EnableViewObjects(win, 0);
                    for (i = 0; i <= 15; i++)
                        if (d2_ClickedId == i + 0x5a2)
                            break;
                    if (i <= 15) {
                        job = gm_TitleJob(me->id, i + 1);
                        cost = job != 0 ? -4 : -8;
                        if (job != me->jobId && sv_CheckMoney(me, -cost) != 0) {
                            for (i = 0; i <= 15; i++) {
                                if (d2_ClickedId == i + 0x5a2) {
                                    group = i + 1;
                                    job = gm_TitleJob(me->id, group);
                                    if (job == 0)
                                        job = gm_TitleNumber(group, 1);
                                    txt_Format(text, 0x1d37, -cost, job + base);
                                    if (sv_MessageBox(text, 1) != 0) {
                                        cm_SendSetJob(me->netId, job);
                                        cm_SendMoneyDelta(me->netId, cost);
                                        sv_PopRequest = 1;
                                        ret = 0x10;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    d2_EnableViewObjects(win, 1);
                }
            }
        }
        d2_CloseView(win);
        ai_CampaignStep(9, 0, 1, 0);
        return ret;
    }
    return 0;
}
int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 (am.h, not included) */
extern struct AICHAR *ai_JudgeCatholic;    /* 0x6f1bfc (gv.h), the catholic judge */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 (gv.h), the protestant judge */
#define LC_TRIAL_PICK_FLAGS 0xc00          /* the ai_PickPartnerRandomWalk flags of the trial's assessors (bits not read) */
/* 0x5cd8c0, opens a trial (the message 0x2c to the player, flags 2): me accuses target of n crimes. Only while a
   building of the class 0xf stands (gm_BuildQuery 1, 6); else nothing is sent. The judge is the holder of the office
   0xd, else one of the two judges at random; with byFaith the judge of me's faith. The holder of the office 0x11 sits
   in when neither side holds it (his seat only from three crimes on), and two assessors come from the random walk
   (ai_PickPartnerRandomWalk, the ids already seated left out): 0x40 when one is missing, else 0x10 */

int ai_OpenTrial(AICHARREC *me, AICHARREC *target, int n, int byFaith)
{
    unsigned short seated[4];
    int picked;
    AIMSG msg;
    unsigned char *bld;
    unsigned int i;

    seated[0] = 0xffff;
    seated[1] = 0xffff;
    seated[2] = 0xffff;
    seated[3] = 0xffff;
    for (bld = gm_BuildQuery(1, 6); bld != 0; bld = gm_BuildNext())
        if (((AIBTYPE *)ai_BTypes)[((AIBLD *)bld)->type].kind == 0xf)
            break;
    if (bld == 0)
        return 0x10;
    msg.type = 0x2c;
    msg.flags = 2;
    msg.to = me->netId;
    msg.fromId = lc_CharRecs[ai_PlayerId].netId;
    msg.arg[0] = me->netId;
    msg.arg[1] = target->netId;
    seated[0] = target->id;
    msg.arg[2] = -1;
    msg.arg[3] = -1;
    msg.arg[4] = -1;
    msg.arg[5] = -1;
    if (byFaith == 0) {
        for (i = 0; i < 768; i++) {
            if (lc_CharRecs[i].officeKind == 0xd) {
                msg.arg[2] = lc_CharRecs[i].netId;
                seated[1] = lc_CharRecs[i].id;
                break;
            }
        }
        if (msg.arg[2] == -1) {
            if (util_RandomU(2) != 0) {
                msg.arg[2] = ((AICHARREC *)ai_JudgeCatholic)->netId;
                seated[1] = ((AICHARREC *)ai_JudgeCatholic)->id;
            } else {
                msg.arg[2] = ((AICHARREC *)ai_JudgeProtestant)->netId;
                seated[1] = ((AICHARREC *)ai_JudgeProtestant)->id;
            }
        }
    } else {
        msg.arg[2] = me->faith == 0 ? ((AICHARREC *)ai_JudgeCatholic)->netId : ((AICHARREC *)ai_JudgeProtestant)->netId;
        seated[1] = me->faith == 0 ? ((AICHARREC *)ai_JudgeCatholic)->id : ((AICHARREC *)ai_JudgeProtestant)->id;
    }
    if (target->officeKind != 0x11 && me->officeKind != 0x11) {
        for (i = 0; i < 768; i++) {
            if (lc_CharRecs[i].officeKind == 0x11) {
                if (n > 2)
                    msg.arg[5] = lc_CharRecs[i].netId;
                seated[2] = lc_CharRecs[i].id;
                break;
            }
        }
    }
    if (ai_PickPartnerRandomWalk((struct AIPER *)me, LC_TRIAL_PICK_FLAGS, 4, seated, 0.0f, 100.0f,
                                 (unsigned short *)&picked) == 0)
        return 0x40;
    if (seated[2] == 0xffff)
        seated[2] = picked;
    else
        seated[3] = picked;
    msg.arg[3] = lc_CharRecs[(unsigned short)picked].netId;
    if (ai_PickPartnerRandomWalk((struct AIPER *)me, LC_TRIAL_PICK_FLAGS, 4, seated, 0.0f, 100.0f,
                                 (unsigned short *)&picked) == 0)
        return 0x40;
    msg.arg[4] = lc_CharRecs[(unsigned short)picked].netId;
    cm_SendLongMessage((char *)&msg);
    return 0x10;
}
/* 0x5cdb90, the accusation window "privillegien\Beweise_Details" (the accusation order's ask, ai_AccusationWindow /
   ai_FaithAccusationWindow, for me and a culprit of its list): as ai_EvidenceDetailsWindow the crimes of culprit
   that me holds (gv_HeldCrimesOf), sorted by law, each a row of its law's name and pictures, the caption 0x1742;
   when the order 0x16 reports the flag 2 for the culprit the text 0x174a, else the accuse button 0x1745, enabled
   only while one of the crimes is open (state 1) and culprit is not me. The button opens the trial (ai_OpenTrial)
   and closes the window; a right click closes it. Returns 0 */

int ai_AccuseWindow(AICHARREC *me, AICHARREC *culprit)
{
    int n;
    int i;
    int k;
    int win;
    int list;
    int accuse;
    int open;
    AIPOSVIEW pos;
    int ids[0x20];
    int arg[0x88];
    CRIME crimes[0x20];

    open = 0;
    accuse = -1;
    if (culprit->id == 0xffff)
        return 0;
    n = gv_HeldCrimesOf(culprit->netId, ids, me->id);
    if (n < 1)
        return 0x60;
    for (i = 0; i < n; i++) {
        crimes[i] = ((CRIME *)ai_Crimes)[ids[i]];
        open |= crimes[i].state == 1;
    }
    if (me == culprit)
        open = 0;
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
    if (ord_DispatchOrder(0x16, culprit, arg) & 2) {
        io_PrintText(0x174a);
    } else {
        accuse = d2_GetWinObj(win, 3, io_PrintText(0x1745));
        if (open == 0)
            d2_SetObjEnabled(accuse, 0);
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == accuse) {
            ai_OpenTrial(me, culprit, n, 0);
            sv_PopRequest = 3;
        }
    } while (sv_RunFrame((int)ai_AccuseWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return 0;
}
/* 0x5cde90, the accusation order's ask (ord_AccusationOrder, kind 3): me accuses a character it holds evidence of.
   The AI (not kind 6 / 7) names him in m->target: the trial opens (ai_OpenTrial) unless me or he holds the office
   0xd, he is unknown or me holds none of his crimes (0x60). A player gets the evidence book "privillegien\
   Beweise_Sichten" (as ai_EvidenceBookWindow: one amount row per culprit, "%i (%i) %s", or the text 0x1741); a
   clicked row opens its accusation window (ai_AccuseWindow), whose result it returns (0x20 when none was opened);
   a right click closes it */

int ai_AccusationWindow(AICHARREC *me, LCORDMSG *m)
{
    int n;
    int count;
    int i;
    int win;
    int list;
    int sel;
    int ret;
    AICHARREC *ch;
    char text[0x40];
    int open[0x20];
    int ids[0x20];
    int total[0x20];
    LCAMOUNTROW rows[0x20];

    ret = 0x20;
    if (me->kind != 6 && me->kind != 7) {
        if (me->officeKind == 0xd)
            return 0x60;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0 || ch->officeKind == 0xd)
            return 0x60;
        open[0] = gv_HeldCrimesOf(m->target, ids, me->id);    /* the original keeps the count in open[0] */
        if (open[0] == 0)
            return 0x60;
        return ai_OpenTrial(me, ch, open[0], 0);
    }
    win = d2_OpenForm(0, 0, "privillegien\\Beweise_Sichten");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x173f);
    d2_SetForm(win, 2);
    list = d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, list, 0x767);
    count = gv_RankCulpritsAll(ids, open, total, me->id);
    for (i = 0, n = 0; i < count; i++) {
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
                ret = ai_AccuseWindow(me, rows[sel].rec);
        }
    } while (sv_RunFrame((int)ai_AccusationWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5ce1a0, the accusation by faith (ord_SpyTargetFlagOrder, the jobs 0x12 / 0x16): ai_AccusationWindow's twin
   whose trial is held by the judge of me's faith (ai_OpenTrial byFaith 1). The AI (not kind 6 / 7) names the
   culprit in m->target: the trial opens unless me or he holds the office 0xd, he is unknown or me holds none of
   his crimes (0x60). A player gets the evidence book "privillegien\Beweise_Sichten" (one amount row per culprit,
   "%i (%i) %s", or the text 0x1741); a clicked row opens its accusation window (ai_AccuseWindow), whose result it
   returns (0x20 when none was opened); a right click closes it. The frame pump names ai_AccusationWindow as its
   owner (the original's copy) */

int ai_FaithAccusationWindow(AICHARREC *me, LCORDMSG *m)
{
    int n;
    int count;
    int i;
    int win;
    int list;
    int sel;
    int ret;
    AICHARREC *ch;
    char text[0x40];
    int open[0x20];
    int ids[0x20];
    int total[0x20];
    LCAMOUNTROW rows[0x20];

    ret = 0x20;
    if (me->kind != 6 && me->kind != 7) {
        if (me->officeKind == 0xd)
            return 0x60;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0 || ch->officeKind == 0xd)
            return 0x60;
        open[0] = gv_HeldCrimesOf(m->target, ids, me->id);    /* the original keeps the count in open[0] */
        if (open[0] == 0)
            return 0x60;
        return ai_OpenTrial(me, ch, open[0], 1);
    }
    win = d2_OpenForm(0, 0, "privillegien\\Beweise_Sichten");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x173f);
    d2_SetForm(win, 2);
    list = d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, list, 0x767);
    count = gv_RankCulpritsAll(ids, open, total, me->id);
    for (i = 0, n = 0; i < count; i++) {
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
                ret = ai_AccuseWindow(me, rows[sel].rec);
        }
    } while (sv_RunFrame((int)ai_AccusationWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5ce4b0, carries out the interrogation (ai_InterrogationWindow): me questions ch, whose office column holds up
   to 6 offices (amt_KindColumn, amt_GetColumnOffices). ch himself gains 100 relation to me and, a player, the news
   0x1e0a (me's id, "_NACHRICHTEN_NEU_05"); every other holder of the column loses 20 and, a player, gets the news
   0x1e0b (me, ch, me; "_NACHRICHTEN_NEU_06"). Then me's flag 0x80000 (AICHARREC.flags1cc, interrogated: the window
   refuses a second one) is set */

void ai_Interrogate(AICHARREC *me, AICHARREC *ch)
{
    AMTREC offices[6];
    char text[0x200];
    char news[0x200];
    AICHARREC *holder;
    int col;
    int n;
    int i;

    col = amt_KindColumn(ch->officeKind);
    n = amt_GetColumnOffices(col, 6, offices);
    for (i = 0; i < n; i++) {
        if (offices[i].owner == -1)
            continue;
        if (ch->netId == offices[i].owner) {
            cm_SendRelations(me->netId, offices[i].owner, 100, 1);
            if (ch->kind == 6 || ch->kind == 7) {
                txt_Format(text, 0x1e0a, me->id);
                he_ShowNews(ch->netId, -1, text, 0, 0x5fb, "_NACHRICHTEN_NEU_05");
            }
        } else {
            holder = (AICHARREC *)gm_FindChar(offices[i].owner);
            if (holder == 0)
                continue;
            cm_SendRelations(me->netId, offices[i].owner, -20, 1);
            if (holder->kind == 6 || holder->kind == 7) {
                txt_Format(news, 0x1e0b, me->id, ch->id, me->id);
                he_ShowNews(holder->netId, -1, news, 0, 0x5fb, "_NACHRICHTEN_NEU_06");
            }
        }
    }
    cm_SendSetField(me->netId, 0x1cc, 4, 0x80000, 0);
}

int amt_SendOfficeMessage(unsigned char a, char *b, char *c, int d, unsigned char e); int gm_TitleNumber();

/* 0x5ce610, the interrogation order's ask (ord.c, the callers push (c, m)): the AI (not kind 6) interrogates the
   character m->target unless me already did (AICHARREC.flags1cc 0x80000: 0x20), he is unknown or holds no office
   (0x60): ai_Interrogate, 0x10. A player who already did gets the message box txt_Ptrs[0x1ce0] (0x81); else he picks
   the character from the amount menu (mode 0xc00, the gypsyFilter classes, kind 6, the title txt_Ptrs[0x1e08]; none:
   2) and confirms in the window "privillegien\verhoer" (the text 0x1e09 with the character's id, its two buttons):
   the first runs ai_Interrogate (0x91), the second or a right click closes it (2) */

int ai_InterrogationWindow(AICHARREC *me, LCORDMSG *m)
{
    AMOUNTCFG cfg;
    AICHARREC *ch;
    int win;
    int text;
    int yes;
    int no;
    int ret;

    if (me->kind != 6) {
        if ((me->flags1cc & 0x80000) != 0)
            return 0x20;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0)
            return 0x60;
        if (ch->officeKind == 0)
            return 0x60;
        ai_Interrogate(me, ch);
        return 0x10;
    }
    ret = 2;
    memset(&cfg, 0, sizeof(cfg));
    if ((me->flags1cc & 0x80000) != 0) {
        sv_MessageBox(txt_Ptrs[0x1ce0], 0);
        return 0x81;
    }
    cfg.mode = 0xc00;
    cfg.filter = gypsyFilter;
    cfg.kind = 6;
    ch = (AICHARREC *)ai_AmountMenu(&cfg, txt_Ptrs[0x1e08], 0, 0);
    if (ch == 0)
        return 2;
    win = d2_OpenForm(0, 0, "privillegien\\verhoer");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1e09, ch->id);
    yes = d2_GetWinObj(win, 1, text);
    no = d2_GetWinObj(win, 1, text + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                ai_Interrogate(me, ch);
                ret = 0x91;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_InterrogationWindow, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5ce7f0, the worker dismissal window "privillegien_arbeiter_ausweisen" (352 B): the character ch, kinds
   6 / 7 only; its office (the byte +0x166) through amt_GetFirstOffice (the list out[0]); the window
   "privillegien\arbeiter_ausweisen" with the text 0x1e24 ((ch->sex != 0 ? 0x27b : 0x253) + the office) and the
   button 0x1e25 / d2_GetWinObj; click 0 -> amt_SendOfficeMessage (out[0], 0, 0, the hour > 0xd ? 0xff : 3, 0xff)
   and 0x90; click 1 closes; the pump 0x5309a0 (0x657c7) and the exit 0x4758f0. The return is 0x22 when nothing
   was clicked, 0x20 for another kind, 0x40 when the office has no list. The second argument m is not read */

int ai_WorkerDismissalWindow(void *chp, int *m)
{
    AICHARREC *ch;
    int out[6];
    int win;
    int text;
    int click1;
    int click2;
    int i;
    int ret;

    ch = (AICHARREC *)chp;
    ret = 0x22;
    if (ch->kind != 6 && ch->kind != 7)
        return 0x20;
    i = amt_GetFirstOffice(ch->officeKind, out);
    if (i == 0)
        return 0x40;
    win = d2_OpenForm(0, 0, "privillegien\\arbeiter_ausweisen");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1e24, (ch->sex != 0 ? 0x27b : 0x253) + ch->officeKind);
    text = io_PrintText(0x1e25);
    click1 = d2_GetWinObj(win, 1, text);
    click2 = d2_GetWinObj(win, 1, text + 1);
    d2_ClickedId = -1;
    for (;;) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == click1) {
                amt_SendOfficeMessage(out[0], 0, 0, (ai_Time.hour > 0xd ? 0xff : 3), 0xff);
                ret = 0x90;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == click2) {
                sv_PopRequest = 1;
            }
        }
        if (sv_RunFrame(ai_WorkerDismissalWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    }
}
/* 0x5ce950, the dismissal order's ask (ord.c, the callers push (c, m)): me has an office holder dismissed for 8
   (gm_SendMoneyDelta), once (AICHARREC.flags1cc 0x40000, set with cm_SendSetField). The dismissal is the office
   message of the holder's first office (amt_GetFirstOffice, amt_SendOfficeMessage(office, 0, 0, after 13 o'clock
   0xff else 3, 0xff)); a player holder gets the news 0x1e10 (me's id, 0x253 for a man, else 0x27b + the office).
   The AI (not kind 6) dismisses m->target: 0x20 when already done or me has less than 8, 0x60 when he is unknown
   or holds no office, 0x40 when the office has no record, else 0x10 ("_NACHRICHTEN_NEU_07"). A player who already
   did gets the message box txt_Ptrs[0x1ce0] (0x81); else he picks the holder from the amount menu (mode 0xc00,
   the gypsyFilter classes, kind 6, the title txt_Ptrs[0x1e0e]; none: 2, no office record: 0x40) and confirms in
   the window "privillegien\verhoer" (the text 0x1e0f with the holder's id): the first button, when me can pay 8
   (sv_CheckMoney), dismisses him and waits for the flag's slot (0x91); the second or a right click closes it (2) */

int ai_DismissalHandler(AICHARREC *me, LCORDMSG *m)
{
    AICHARREC *ch;
    AMOUNTCFG cfg;
    int office[6];
    char text[0x200];
    int win;
    int t;
    int yes;
    int no;
    int slot;
    int ret;

    if (me->kind != 6) {
        if ((me->flags1cc & 0x40000) != 0)
            return 0x20;
        if (me->money < 8)
            return 0x20;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0)
            return 0x60;
        if (ch->officeKind == 0)
            return 0x60;
        if (amt_GetFirstOffice(ch->officeKind, office) == 0)
            return 0x40;
        amt_SendOfficeMessage(office[0], 0, 0, (ai_Time.hour > 0xd ? 0xff : 3), 0xff);
        gm_SendMoneyDelta(me->netId, -8);
        cm_SendSetField(me->netId, 0x1cc, 4, 0x40000, 0);
        if (ch->kind == 6 || ch->kind == 7) {
            txt_Format(text, 0x1e10, me->id, ch->sex == 0 ? 0x253 : ch->officeKind + 0x27b);
            he_ShowNews(ch->netId, -1, text, 0, 0x5fb, "_NACHRICHTEN_NEU_07");
        }
        return 0x10;
    }
    ret = 2;
    memset(&cfg, 0, sizeof(cfg));
    if ((me->flags1cc & 0x40000) != 0) {
        sv_MessageBox(txt_Ptrs[0x1ce0], 0);
        return 0x81;
    }
    cfg.mode = 0xc00;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    ch = (AICHARREC *)ai_AmountMenu(&cfg, txt_Ptrs[0x1e0e], 0, 0);
    if (ch == 0)
        return 2;
    if (amt_GetFirstOffice(ch->officeKind, office) == 0)
        return 0x40;
    win = d2_OpenForm(0, 0, "privillegien\\verhoer");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1e0f, ch->id);
    yes = d2_GetWinObj(win, 1, t);
    no = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes && sv_CheckMoney(me, 8) != 0) {
                amt_SendOfficeMessage(office[0], 0, 0, (ai_Time.hour > 0xd ? 0xff : 3), 0xff);
                gm_SendMoneyDelta(me->netId, -8);
                slot = cm_SendSetField(me->netId, 0x1cc, 4, 0x40000, 0);
                while (slot != -1 && cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                sv_PopRequest = 1;
                ret = 0x91;
                if (ch->kind == 6 || ch->kind == 7) {
                    txt_Format(text, 0x1e10, me->id, ch->sex == 0 ? 0x253 : ch->officeKind + 0x27b);
                    he_ShowNews(ch->netId, -1, text, 0, 0x5fb, 0);
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_DismissalHandler, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5ced50, sends the AIMSG type 0x45 "the interrogation was refused" (144 B): to the interrogator ch
   (its netId at +4, as 'to' and arg[0]), about the interrogated target (its netId as arg[1]) and the
   interrogation window variant 1 / 2 (arg[2]); the clock, flags 2; cm_SendLongMessage (0x4c4b70) returns
   its handle. */

int ai_SendInterrogationRefusal(char *ch, char *target, int variant)
{
    AIMSG msg;

    memset(&msg, 0, sizeof msg);
    msg.type = 0x45;
    msg.to = ((AICHARREC *)ch)->netId;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 2;
    msg.arg[0] = ((AICHARREC *)ch)->netId;
    msg.arg[1] = ((AICHARREC *)target)->netId;
    msg.arg[2] = variant;
    return cm_SendLongMessage((char *)&msg);
}
/* 0x5cede0, the interrogation order's ask, variant 1 (ord_InterrogationOrder2, the callers push (c, m)): me has an
   office holder questioned for 4, once (AICHARREC.powerFlags 0x100): the message 0x45 of variant 1
   (ai_SendInterrogationRefusal). The AI (not kind 6) questions m->target: 0x20 when already done or me has less
   than 4, 0x60 when he is unknown or holds no office, else 0x10. A player who already did gets the message box
   txt_Ptrs[0x1ce0] (0x81); else he picks the holder from the office list ai_SpecialAmountListWindow (kind 4, mode
   0xc00, the gypsyFilter classes, kind 6, the caption txt_Ptrs[0x1e16]; none: 2) and confirms in the window
   "privillegien\verhoer" (the text 0x1e17 with the holder's id and the price 4): the first button, when me can
   pay 4 (sv_CheckMoney), sends the message and waits for its slot (0x91); the second or a right click closes it
   (2) */

int ai_InterrogationWindow1(AICHARREC *me, LCORDMSG *m)
{
    AICHARREC *ch;
    AMOUNTCFG cfg;
    int win;
    int t;
    int yes;
    int no;
    int slot;
    int ret;

    if (me->kind != 6) {
        if ((me->powerFlags & 0x100) != 0)
            return 0x20;
        if (me->money < 4)
            return 0x20;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0)
            return 0x60;
        if (ch->officeKind == 0)
            return 0x60;
        ai_SendInterrogationRefusal((char *)me, (char *)ch, 1);
        return 0x10;
    }
    ret = 2;
    memset(&cfg, 0, sizeof(cfg));
    if ((me->powerFlags & 0x100) != 0) {
        sv_MessageBox(txt_Ptrs[0x1ce0], 0);
        return 0x81;
    }
    cfg.mode = 0xc00;
    cfg.filter = gypsyFilter;
    cfg.kind = 6;
    ch = (AICHARREC *)ai_SpecialAmountListWindow(4, (unsigned int *)&cfg, txt_Ptrs[0x1e16], ch_EmptyName, 0);
    if (ch == 0)
        return 2;
    win = d2_OpenForm(0, 0, "privillegien\\verhoer");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1e17, ch->id, 4);
    yes = d2_GetWinObj(win, 1, t);
    no = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes && sv_CheckMoney(me, 4) != 0) {
                slot = ai_SendInterrogationRefusal((char *)me, (char *)ch, 1);
                while (slot != -1 && cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                sv_PopRequest = 1;
                ret = 0x91;
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_InterrogationWindow1, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5cf020, the interrogation order's ask, variant 2 (ord_InterrogationOrder3, the callers push (c, m)): the twin
   of ai_InterrogationWindow1 with the message 0x45 of variant 2, the office list of kind 5, the caption
   txt_Ptrs[0x1e1d] and the text 0x1e1e (the holder's id, the price 4) */

int ai_InterrogationWindow2(AICHARREC *me, LCORDMSG *m)
{
    AICHARREC *ch;
    AMOUNTCFG cfg;
    int win;
    int t;
    int yes;
    int no;
    int slot;
    int ret;

    if (me->kind != 6) {
        if ((me->powerFlags & 0x100) != 0)
            return 0x20;
        if (me->money < 4)
            return 0x20;
        ch = (AICHARREC *)gm_FindChar(m->target);
        if (ch == 0)
            return 0x60;
        if (ch->officeKind == 0)
            return 0x60;
        ai_SendInterrogationRefusal((char *)me, (char *)ch, 2);
        return 0x10;
    }
    ret = 2;
    memset(&cfg, 0, sizeof(cfg));
    if ((me->powerFlags & 0x100) != 0) {
        sv_MessageBox(txt_Ptrs[0x1ce0], 0);
        return 0x81;
    }
    cfg.mode = 0xc00;
    cfg.filter = gypsyFilter;
    cfg.kind = 6;
    ch = (AICHARREC *)ai_SpecialAmountListWindow(5, (unsigned int *)&cfg, txt_Ptrs[0x1e1d], ch_EmptyName, 0);
    if (ch == 0)
        return 2;
    win = d2_OpenForm(0, 0, "privillegien\\verhoer");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1e1e, ch->id, 4);
    yes = d2_GetWinObj(win, 1, t);
    no = d2_GetWinObj(win, 1, t + 1);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes && sv_CheckMoney(me, 4) != 0) {
                slot = ai_SendInterrogationRefusal((char *)me, (char *)ch, 2);
                while (slot != -1 && cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                sv_PopRequest = 1;
                ret = 0x91;
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_InterrogationWindow2, 0x657c7) != 0);
    d2_CloseView(win);
    return ret;
}
/* 0x5cf260, the profession chooser "privillegien\CHOOSEPROFESSION2" (ai_CharInfoPanel): parent apprentices child.
   The fee is 1.5 % of the parent's worth (gm_GetWorth); the caption 0x1a9a names the child (0x1aa0 / 0x1aa1 by
   sex, the id) and the fee. The fifteen profession pictures 0x5a2 .. 0x5b0 stand in a 5 x 3 grid over the form
   (columns of (width - 0x40) / 5, rows of (height - 0x84) / 3, centred by the picture 0x5a2's width), each
   interactive with the style 3 and the caption txt_Ptrs[0x1ef + i] in the font 0x56. A left click on a picture
   sends the apprenticeship message 0x5e to the child (flags 0x12; the parent, the building class gm_TitleNumber
   (profession, 1), the fee) and closes the window; a right click closes it. Returns 1 once the left button was
   clicked, else 0 */

int ai_ChooseProfessionWindow(AICHARREC *parent, AICHARREC *child)
{
    AIMSG msg;
    int fee;
    int win;
    int colW;
    int rowH;
    int dx;
    int i;
    int pic;
    int ret;

    fee = (int)((float)gm_GetWorth(parent->id) * 0.015f);
    win = d2_OpenForm(0, 0, "privillegien\\CHOOSEPROFESSION2");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    colW = (d2_Wins[d2_LastWindow].w - 0x40) / 5;
    rowH = (d2_Wins[d2_LastWindow].h - 0x84) / 3;
    dx = (colW - d2_Pics[0x5a2].w) / 2;
    io_PrintText(0x1a9a, (child->sex != 0) + 0x1aa0, child->id, fee);
    for (i = 1; i <= 15; i++) {
        pic = d2_WinOpenPic((i - 1) % 5 * colW + dx + 0x20, (i - 1) / 5 * rowH + dx + 0x64, i + 0x5a1, d2_LastWindow);
        d2_Objs[pic].interactive = 1;
        d2_Objs[pic].style = 3;
        strcpy(d2_Objs[pic].caption, txt_Ptrs[0x1ee + i]);
        d2_Objs[pic].capfont = 0x56;
    }
    ret = 0;
    while (sv_RunFrame((int)ai_ChooseProfessionWindow, sv_FrameMask) != 0) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (dm_L.lclick != 0) {
            ret = 1;
            for (i = 0; i <= 15; i++) {
                if (d2_ClickedId == i + 0x5a2) {
                    msg.flags = 0x12;
                    msg.to = child->netId;
                    msg.type = 0x5e;
                    msg.fromId = -1;
                    msg.apprenticeship.parent = parent->netId;
                    msg.apprenticeship.bldClass = gm_TitleNumber(i + 1, 1);
                    msg.apprenticeship.price = fee;
                    cm_SendLongMessage(&msg);
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(win);
    return ret;
}
/* 0x5cf4f0, the university window "special\uni" (ai_CharInfoPanel): the player sends child to study. The three
   offers come from ai_RollStudyOffers for the player's character (per offer: the fee, the subject, the level);
   the caption 0x1a9d names the child (0x1aa0 / 0x1aa1 by sex, the id, 5); each offer is a row (y = i * 0x5a + 10)
   of the picture 0x4c6 (interactive, style 3), the town of the player slot i + 1, the texts 0x16b7 + subject and
   0x16b7 + level and the fee ("%T"); the third form holds the close button 0x1ba8. A click on a row sends the
   study message 0x5f to the child (flags 0x12; the player's character, the fee, the subject, the level), sets
   the child's powerFlags 0x10000 and closes the window (1); the close button or a right click closes it (0).
   parent (the caller passes the player's character) is not read */

int ai_UniversityWindow(AICHARREC *parent, AICHARREC *child)
{
    int win;
    int ctrl[4];
    int offers[3][3];
    AIMSG msg;
    char text[0x100];
    int i;
    int ret;

    ai_RollStudyOffers(ai_Chars[ai_PlayerId], offers);
    win = d2_OpenForm(0, 0, "special\\uni");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1a9d, (child->sex != 0) + 0x1aa0, child->id, 5);
    d2_SetForm(win, 2);
    for (i = 0; i < 3; i++) {
        ctrl[i] = d2_WinOpenPic(0x14, i * 0x5a + 10, 0x4c6, d2_LastWindow);
        d2_Objs[ctrl[i]].interactive = 1;
        d2_Objs[ctrl[i]].style = 3;
        txt_Format(text, "%s", ((GMPLAYERSLOT *)ai_PlayerSlots)[i + 1].name);
        d2_CreateWinText(0xbe, i * 0x5a + 10, text, d2_LastWindow);
        txt_Format(text, "%s", offers[i][1] + 0x16b7);
        d2_CreateWinText(0xbe, i * 0x5a + 0x1e, text, d2_LastWindow);
        txt_Format(text, "%s", offers[i][2] + 0x16b7);
        d2_CreateWinText(0xbe, i * 0x5a + 0x32, text, d2_LastWindow);
        txt_Format(text, "%T", offers[i][0]);
        d2_CreateWinText(0xbe, i * 0x5a + 0x46, text, d2_LastWindow);
    }
    d2_SetForm(win, 3);
    ctrl[3] = d2_GetWinObj(win, 3, io_PrintText(0x1ba8));
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    ret = 0;
    while (sv_RunFrame((int)ai_UniversityWindow, sv_FrameMask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ctrl[3]) {
                sv_PopRequest = 1;
            } else {
                for (i = 0; i < 3; i++) {
                    if (d2_PointerObj == ctrl[i]) {
                        msg.flags = 0x12;
                        msg.type = 0x5f;
                        msg.to = child->netId;
                        msg.fromId = -1;
                        msg.study.ch = lc_CharRecs[ai_PlayerId].netId;
                        msg.study.subject = offers[i][1];
                        msg.study.level = offers[i][2];
                        msg.study.child = offers[i][0];
                        cm_SendLongMessage(&msg);
                        cm_SendSetField(child->netId, 0x1c8, 4, 0x10000, 0);
                        ret = 1;
                        sv_PopRequest = 1;
                        break;
                    }
                }
            }
        }
    }
    d2_CloseView(win);
    return ret;
}
/* 0x5cf820, the use-object panel (form "panel\useobj" 0x62d010): opens the form, page 1, prints
   text through the "%s" 0x5f8318, and pumps frames until the right mouse button (dm_L.rup) or a pop request
   closes it. */

void ai_UseObjPanel(char *text)
{
    int win;

    win = d2_OpenForm(0, 0, "panel\\useobj");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText("%s", text);
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_UseObjPanel, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5cf8a0, uses a Domizil good: ch the character, use the request (use->obj the inventory object). Errors: -1 no
   character, -2 no request, -3 no object, -4 the good has no Domizil entry (ai_FindUsableGood), -5 a good of the
   same class (G9DOMIZIL.mask against ch->goodsMask) is in use. use->effect is 1, but for the good 0x175 only when
   util_RandomF() >= 0.66; the entry's use callback runs (0 -> return 0); ai_CampaignStep 0x2d; the class mask
   is set on the character; a good with a duration (G9DOMIZIL.minutes) gets its object's minutesLeft, the
   two flags and the start time ai_Time as field deltas, one without is sold from the character
   (cm_SendExSellObject); a good with a law accuses the character (gv_Accuse, the home building's id). A player
   (kind 6) whose callback did not report the use sees the panel, with the text 0xfe8 (rank + 1) on effect.
   Returns the callback's result (1 without one). */

int ai_UseGood(AICHARREC *ch, G8USEVIEW *use)
{
    char text[0x800];
    int minutes;
    char flag;
    G9DOMIZIL *entry;
    G9DOMIZIL *found;
    int ret;

    if (ch == 0)
        return -1;
    if (use == 0)
        return -2;
    use->handled = 0;
    if (use->obj == 0)
        return -3;
    entry = (G9DOMIZIL *)ai_FindUsableGood(use->obj->id);
    if (entry == 0)
        return -4;
    if ((entry->mask & ch->goodsMask) != 0)
        return -5;
    use->effect = (use->obj->id != 0x175 || util_RandomF() >= 0.66) ? 1 : 0;
    if (entry->use != 0)
        ret = ((int (*)(AICHARREC *, G8USEVIEW *))entry->use)(ch, use);
    else
        ret = 1;
    if (ret != 0) {
        ai_CampaignStep(0x2d, 0, 0, use->obj->id);
        if (entry->mask != 0)
            cm_SendSetField(ch->netId, 0x1ec, 4, entry->mask, 0);
        if (entry->minutes != 0) {
            int one;

            minutes = entry->minutes;
            one = 1;
            flag = 1;
            cm_BeginFieldBlock((char *)use->obj, use->obj->objId);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->minutesLeft - cm_FieldObject), &minutes);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->active - cm_FieldObject), &one);
            cm_AddFieldDelta(1, 1, (unsigned short)((char *)&use->obj->inUse - cm_FieldObject), &flag);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->since.year - cm_FieldObject), &ai_Time.year);
            cm_AddFieldDelta(2, 1, (unsigned short)((char *)&use->obj->since.hour - cm_FieldObject), &ai_Time.hour);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->since.min - cm_FieldObject), &ai_Time.min);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->since.sec - cm_FieldObject), &ai_Time.sec);
            cm_SendFieldDeltas();
        }
        if (entry->minutes == 0)
            cm_SendExSellObject(-1, ch->netId, use->obj->id, 1, ai_OwnPlayer, 0);
        if (entry->law != -1) {
            if (ch->bld2 != 0)
                gv_Accuse(entry->law, 1, ch->netId, -1, ((AIBLD *)ch->bld2)->id);
            else
                gv_Accuse(entry->law, 1, ch->netId, -1, -1);
        }
        if (ch->kind == 6 && use->handled == 0) {
            int index;

            found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
            if (use->effect == 1)
                txt_Format(text, 0xfe8, found->rank + 1);
            ai_UseObjPanel(text);
        }
    }
    return ret;
}
/* 0x5cfb60, the use callback of the Domizil good 0x173 (the +0x14 pointer of the table entry 0x62c6c0):
   pays the character ch 2 money with gm_SendMoneyDelta (message 0x5a, the netId at +4); returns 1. */

int ai_UseGoodAddMoney(AICHARREC *ch)
{
    gm_SendMoneyDelta(ch->netId, 2);
    return 1;
}
/* 0x5cfb80, the use callback of the Domizil good 0x175 (the +0x14 pointer of the table entry 0x62c6dc;
   ai_UseGood gives it the effect 1 only one time in three): effect 1 adds 10% of the character's life
   (AICHAR.f01c) with cm_SendFloatDelta (0x4c4ac0); effect 0 for a player (kind 6) sets use->handled and
   shows the text 0xff0 (ai_UseObjPanel); returns 1. */

int ai_UseGoodHeal(AICHARREC *ch, G8USEVIEW *use)
{
    char text[0x800];

    if (use->effect == 1) {
        cm_SendFloatDelta(ch->netId, 0x1c, ch->f01c * 0.1f);
    } else if (use->effect == 0 && ch->kind == 6) {
        use->handled = 1;
        txt_Format(text, 0xff0);
        ai_UseObjPanel(text);
    }
    return 1;
}
/* 0x5cfc00, the use callback of the Domizil good 0x177 (reached through its table entry +0x14): picks the target
   building, for a player (kind 6) on the town map (ai_MapPick with the prompt 0x9a, building class 2 only), else
   the building use->target; none, or a building without owner -> 0. Sends the effect message 0x76 (to the user,
   the clock, flags 10, the building's id, the good 0x177), stores the building's id into the used object's bldId
   and infects the building's owner (gm_CatchDisease): always for a player, else unless the owner holds the good
   0x2c4 and util_RandomF() <= 0.75. A player owner (kind 6 / 7) gets the news 0xff1 (key 0x5fb). Returns 1.
   Both paths are written out in full, as the original's code has them. */

int ai_UseGoodInfectOwner(AICHARREC *ch, G8USEVIEW *use)
{
    AIBLD *bld;
    int *id;

    if (ch->kind == 6) {
        AMOUNTCFG cfg;
        char text[0x100];

        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9a);
        cfg.flags = 1;
        cfg.mode = -1;
        cfg.kind = 6;
        cfg.classMask[0] = 4;
        bld = (AIBLD *)ai_MapPick(1, &cfg, text, 0, 0);
        if (bld != 0 && bld->owner != 0) {
            AIMSG msg;

            id = &bld->id;
            msg.to = ch->netId;
            msg.type = 0x76;
            msg.fromId = -1;
            msg.time = ai_Time;
            msg.flags = 10;
            msg.effect.target = *id;
            msg.effect.good = 0x177;
            cm_SendLongMessage((char *)&msg);
            cm_BeginFieldBlock((char *)use->obj, use->obj->objId);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->bldId - cm_FieldObject), id);
            cm_SendFieldDeltas();
            gm_CatchDisease((struct ORDCHAR *)&lc_CharRecs[bld->owner], 0, 0);
            if (lc_CharRecs[bld->owner].kind == 6 || lc_CharRecs[bld->owner].kind == 7) {
                char news[0x800];

                txt_Format(news, 0xff1);
                he_ShowNews(lc_CharRecs[bld->owner].netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    } else {
        bld = (AIBLD *)gm_BuildQuery(1, 1, use->target);
        if (bld != 0 && bld->owner != 0) {
            AIMSG msg;

            id = &bld->id;
            msg.to = ch->netId;
            msg.type = 0x76;
            msg.fromId = -1;
            msg.time = ai_Time;
            msg.flags = 10;
            msg.effect.target = *id;
            msg.effect.good = 0x177;
            cm_SendLongMessage((char *)&msg);
            cm_BeginFieldBlock((char *)use->obj, use->obj->objId);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->bldId - cm_FieldObject), id);
            cm_SendFieldDeltas();
            if (ai_InventoryHas(&lc_CharRecs[bld->owner], 0x2c4) == 0 || util_RandomF() > 0.75f)
                gm_CatchDisease((struct ORDCHAR *)&lc_CharRecs[bld->owner], 0, 0);
            if (lc_CharRecs[bld->owner].kind == 6 || lc_CharRecs[bld->owner].kind == 7) {
                char news[0x800];

                txt_Format(news, 0xff1);
                he_ShowNews(lc_CharRecs[bld->owner].netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    }
    return 0;
}
/* 0x5cff70, the use callback of the Domizil good 0x178 (reached through its table entry +0x14): picks the target
   building, for a player (kind 6) on the town map (ai_MapPick with the prompt 0x9b, the building class filter
   0x1a0744300), else the building use->target; none -> 0. Sends the effect message 0x76 (to the user, the clock,
   flags 10, the building's id, the good 0x178: the points effect he_CreatePointsEffect), then the send-home
   message 0x77 (flags 1, the building's id; he_StartSendHomeEvent), stores the building's id into the used
   object's bldId and sets the bit 0x20 of the building's word 0x5a (cm_SendSetField). An owner (not 0xffff) of
   kind 6 / 7 gets the news 0xff2 about the building (key 0x5fb). Returns 1. The message is cleared with
   D3_ZERO before and after the 0x76 message in each path, as the original's code has it. Both paths are written
   out in full with the 0x77 message and the rest, as the original has them: VC6 cross-jumps the first copy
   into the second, and the first copy's temporaries set the second path's scratch registers. */

int ai_UseGoodSendHome(AICHARREC *ch, G8USEVIEW *use)
{
    AIBLD *bld;
    int *id;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        AMOUNTCFG cfg;
        char text[0x100];

        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9b);
        cfg.flags = 1;
        cfg.mode = -1;
        cfg.kind = 6;
        cfg.filter = 0x1a0744300;
        bld = (AIBLD *)ai_MapPick(1, &cfg, text, 0, 0);
        if (bld != 0) {
            {
                AIMSG msg;

                D3_ZERO(&msg, sizeof(msg));
                msg.to = ch->netId;
                msg.type = 0x76;
                msg.fromId = -1;
                msg.time = ai_Time;
                msg.flags = 10;
                id = &bld->id;
                msg.effect.target = *id;
                msg.effect.good = 0x178;
                cm_SendLongMessage((char *)&msg);
                D3_ZERO(&msg, sizeof(msg));
            }
            {
                /* @original a message of its own block: VC6 gives it the slot of the 0x76 messages, which the
                   D3_ZERO after each send has cleared, so its other fields are 0 */
                AIMSG msg;

                msg.to = ch->netId;
                msg.type = 0x77;
                msg.fromId = -1;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.effect.target = *id;
                cm_SendLongMessage((char *)&msg);
            }
            cm_BeginFieldBlock((char *)use->obj, use->obj->objId);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->bldId - cm_FieldObject), id);
            cm_SendFieldDeltas();
            cm_SendSetField(*id, 0x5a, 2, 0x20, 0);
            if (bld->owner != 0xffff && (lc_CharRecs[bld->owner].kind == 6 || lc_CharRecs[bld->owner].kind == 7)) {
                char news[0x800];

                txt_Format(news, 0xff2, bld);
                he_ShowNews(lc_CharRecs[bld->owner].netId, -1, news, 0, 0x5fb, 0);
            }
        } else {
            goto done;
        }
    } else {
        bld = (AIBLD *)gm_BuildQuery(1, 1, use->target);
        if (bld == 0)
            goto done;
        {
            AIMSG msg;

            D3_ZERO(&msg, sizeof(msg));
            msg.to = ch->netId;
            msg.type = 0x76;
            msg.fromId = -1;
            msg.time = ai_Time;
            msg.flags = 10;
            id = &bld->id;
            msg.effect.target = *id;
            msg.effect.good = 0x178;
            cm_SendLongMessage((char *)&msg);
            D3_ZERO(&msg, sizeof(msg));
        }
        {
            /* @original a message of its own block: VC6 gives it the slot of the 0x76 messages, which the
               D3_ZERO after each send has cleared, so its other fields are 0 */
            AIMSG msg;

            msg.to = ch->netId;
            msg.type = 0x77;
            msg.fromId = -1;
            msg.time = ai_Time;
            msg.flags = 1;
            msg.effect.target = *id;
            cm_SendLongMessage((char *)&msg);
        }
        cm_BeginFieldBlock((char *)use->obj, use->obj->objId);
        cm_AddFieldDelta(4, 1, (unsigned short)((char *)&use->obj->bldId - cm_FieldObject), id);
        cm_SendFieldDeltas();
        cm_SendSetField(*id, 0x5a, 2, 0x20, 0);
        if (bld->owner != 0xffff && (lc_CharRecs[bld->owner].kind == 6 || lc_CharRecs[bld->owner].kind == 7)) {
            char news[0x800];

            txt_Format(news, 0xff2, bld);
            he_ShowNews(lc_CharRecs[bld->owner].netId, -1, news, 0, 0x5fb, 0);
        }
    }
    ret = 1;
done:
    return ret;
}
/* 0x5d0320, the use callback of the Domizil good 0x17b (reached through its table entry +0x14): picks the target
   character, for a player (kind 6) with the special-amount menu (ai_AmountMenu, the gypsy filter, mode 0x400, the
   prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. Sends it the skill-bonus message 0x82:
   the skills 3 and 2 lose 1/6 each for 24 hours (flags 2). A player target (kind 6 / 7) gets the news 0xff8 (key
   0x5fb). Returns 1. Both paths are written out in full, as the original's code has them. */

int ai_UseGoodWeakenSkills(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x800];
    AICHARREC *target;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target != 0) {
            AIMSG msg;
            msg.type = 0x82;
            msg.to = target->netId;
            msg.fromId = -1;
            msg.flags = 2;
            msg.skillBonus.skill1 = 3;
            msg.skillBonus.bonus1 = -1.0f / 6.0f;
            msg.skillBonus.skill2 = 2;
            msg.skillBonus.bonus2 = -1.0f / 6.0f;
            msg.skillBonus.hours = 24;
            cm_SendLongMessage((char *)&msg);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xff8);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            AIMSG msg;
            msg.type = 0x82;
            msg.to = target->netId;
            msg.fromId = -1;
            msg.flags = 2;
            msg.skillBonus.skill1 = 3;
            msg.skillBonus.bonus1 = -1.0f / 6.0f;
            msg.skillBonus.skill2 = 2;
            msg.skillBonus.bonus2 = -1.0f / 6.0f;
            msg.skillBonus.hours = 24;
            cm_SendLongMessage((char *)&msg);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xff8);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    }
    return 0;
}
/* 0x5d0530, the use callback of the Domizil good 0x17d (reached through its table entry +0x14), the twin of
   ai_UseGoodWeakenSkills: a player (kind 6) picks the target character with the same special-amount menu (prompt
   0x9c) and sends it the skill-bonus message 0x82: skill 0 loses 1/6 and skill 4 loses 1/3 for 24 hours (flags
   2); for any other user the target is use->target (gm_FindChar) and no message is sent. A player target (kind
   6 / 7) gets the news 0xff6 (key 0x5fb). Returns 1, 0 without a target. */

int ai_UseGoodWeakenSkills2(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x800];
    AICHARREC *target;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target != 0) {
            AIMSG msg;
            msg.type = 0x82;
            msg.to = target->netId;
            msg.fromId = -1;
            msg.flags = 2;
            msg.skillBonus.skill1 = 0;
            msg.skillBonus.bonus1 = -1.0f / 6.0f;
            msg.skillBonus.skill2 = 4;
            msg.skillBonus.bonus2 = -1.0f / 3.0f;
            msg.skillBonus.hours = 24;
            cm_SendLongMessage((char *)&msg);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xff6);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xff6);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    }
    return 0;
}
/* 0x5d06e0, the use callback of the Domizil good 0x17e (the +0x14 pointer of the table entry 0x62c784;
   the "Unversehrtheit" good ai_DecideUseHealthObject wants while a character has an ailment): clears the
   low bits of the first set ailment field of ch->ailments (the mask of its field: 7, 0x30, 0x300, 0x1000,
   0xc000, 0x20000, 0x100000, 0x800000, 0x6000000) with cm_SendSetField; returns 1. */

int ai_UseGoodCureAilment(AICHARREC *ch)
{
    unsigned int f;
    int mask;

    f = ch->ailments;
    if (f != 0) {
        mask = 0;
        if (f & 0xf)
            mask = 7;
        else if (f & 0xf0)
            mask = 0x30;
        else if (f & 0xf00)
            mask = 0x300;
        else if (f & 0x3000)
            mask = 0x1000;
        else if (f & 0x1c000)
            mask = 0xc000;
        else if (f & 0xe0000)
            mask = 0x20000;
        else if (f & (7 << 20))
            mask = 0x100000;
        else if (f & 0x1800000)
            mask = 8 << 20;
        else if (f & 0x1e000000)
            mask = 0x6000000;
        cm_SendSetField(ch->netId, 0x2c, 4, 0, mask);
    }
    return 1;
}
/* 0x5d0780, the use callback of the Domizil good 0x161 (reached through its table entry +0x14): picks the target
   building, for a player (kind 6) on the town map (ai_MapPick with the prompt 0x9b, the class filter 0x90010,
   kind 4), else the building use->target; none -> 0. Every character in that building (AICHARREC.bld) has its
   b1c5 halved, rounded up, sent as a field delta. Returns 1. (am.c's AI uses the good where the workers' b1c5
   averages at least 6; what b1c5 means is not known.) */

int ai_UseGoodHalveWorkerValue(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    unsigned char value;
    AIBLD *bld;
    int i;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9b);
        cfg.flags = 1;
        cfg.mode = -1;
        cfg.kind = 4;
        cfg.filter = 0x90010;
        bld = (AIBLD *)ai_MapPick(1, &cfg, text, 0, 0);
        if (bld == 0)
            goto done;
        ret = 1;
    } else {
        bld = (AIBLD *)gm_BuildQuery(1, 1, use->target);
        if (bld == 0)
            goto done;
        ret = 1;
    }
    for (i = 0; i < 0x300; i++) {
        if (lc_CharRecs[i].bld == bld) {
            value = lc_CharRecs[i].b1c5 % 2 + lc_CharRecs[i].b1c5 / 2;
            cm_BeginFieldBlock((char *)&lc_CharRecs[i], lc_CharRecs[i].netId);
            cm_AddFieldDelta(1, 1, (unsigned short)((char *)&lc_CharRecs[i].b1c5 - cm_FieldObject), &value);
            cm_SendFieldDeltas();
        }
    }
done:
    return ret;
}
/* 0x5d08c0, raises a skill of the character ch by a trial step (he.c's trial event calls it for both of its
   skills): skill 0 .. 4 (>= 5 -> 0); a skill value n of 252 or more -> 0. With x = (252 - n) / 252 and
   y = 0.6x + 0.2 the amount is (1 - y + util_RandomF() * y) * 42x + 1, cut so that n + amount stays at 252; a
   nonzero amount goes out with cm_SendAddSkill (the netId, the skill, the amount). Returns 1. */

int ai_ApplyTrialStep(AICHARREC *ch, int skill)
{
    float x;
    float y;
    unsigned char amount;

    if (skill < 5 && !((float)ch->skills[skill] >= 252.0f)) {
        x = (252.0f - ch->skills[skill]) * (1.0f / 252.0f);
        y = x * 0.6f + 0.2f;
        amount = (unsigned char)(int)(x * (((1.0f - y) + util_RandomF() * y) * 42.0f) + 1.0f);
        if ((float)(ch->skills[skill] + amount) > 252.0f)
            amount = (unsigned char)(int)(252.0f - ch->skills[skill]);
        if (amount != 0)
            cm_SendAddSkill(ch->netId, skill, amount);
        return 1;
    }
    return 0;
}
/* 0x5d09d0, the use callback of the Domizil record 9 (the good 0x162, 0x62c51c): the character kind 6 picks
   the target in the special-amount list 0x5bea00 (the text 0x5176d0 0x9d), every other kind takes the use's
   target 0x505df0; the target's trial step 1 (0x5d08c0) runs and 1, 0 without a target. */

int ai_TrialStep1(G8CHARREC *ch, G8USEVIEW *use)
{
    char text[0x100];
    G8CHARREC *target;

    if (ch->kind == 6) {
        txt_Format(text, (char *)0x9d);
        target = (G8CHARREC *)ai_SpecialAmountList((int)ch, text);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 1);
            return 1;
        }
    } else {
        target = (G8CHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 1);
            return 1;
        }
    }
    return 0;
}
/* 0x5d0a40, the use callback of the Domizil record 10 (the good 0x163, 0x62c538): the character kind 6 picks
   the target in the special-amount list 0x5bea00 (the text 0x5176d0 0x9d), every other kind takes the use's
   target 0x505df0; the target's trial step 3 (0x5d08c0) runs and 1, 0 without a target. */

int ai_TrialStep3(G8CHARREC *ch, G8USEVIEW *use)
{
    char text[0x100];
    G8CHARREC *target;

    if (ch->kind == 6) {
        txt_Format(text, (char *)0x9d);
        target = (G8CHARREC *)ai_SpecialAmountList((int)ch, text);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 3);
            return 1;
        }
    } else {
        target = (G8CHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 3);
            return 1;
        }
    }
    return 0;
}
/* 0x5d0ab0, the use callback of the Domizil record 11 (the good 0x164, 0x62c554): the character kind 6 picks
   the target in the special-amount list 0x5bea00 (the text 0x5176d0 0x9d), every other kind takes the use's
   target 0x505df0; the target's trial step 4 (0x5d08c0) runs and 1, 0 without a target. */

int ai_TrialStep4(G8CHARREC *ch, G8USEVIEW *use)
{
    char text[0x100];
    G8CHARREC *target;

    if (ch->kind == 6) {
        txt_Format(text, (char *)0x9d);
        target = (G8CHARREC *)ai_SpecialAmountList((int)ch, text);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 4);
            return 1;
        }
    } else {
        target = (G8CHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            ai_ApplyTrialStep((int)target, 4);
            return 1;
        }
    }
    return 0;
}
/* 0x5d0b20, the use callback of the Domizil good 0x167 (reached through its table entry +0x14): picks the target
   character, for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400 with the partner
   bit 1 for a player whose sex byte is set, else 2; the prompt 0x9c), else the character use->target
   (gm_FindChar); none -> 0. The target's relation to the user rises by 25, 50 when the user holds the good 0x2c5
   (cm_SendRelations); a player target (kind 6 / 7) gets the news 0xfe9 naming the user (key 0x5ec). Returns 1.
   @deep deep s38: ret = 1 set in each arm after its null test (path 1 after if (target == 0) return 0;,
   path 2 as if (target != 0) ret = 1; in the else): the original's mov edi,1 positions (path 1 before the
   menu call with cfg.flags stored from edi, path 2 right after its test) and cm_SendRelations' last
   argument an immediate 1 (ret = 1 in the common block makes VC6 push edi): 31 -> 24 ins, draft -> shape.
   Left (regs / order): after ai_InventoryHas the original loads target->netId into ecx, then ch->netId
   into edx, and pushes both; ours hoists ch->netId into ecx and loads target->netId late into eax; the
   news block's scratch registers rotate behind it. Tried: amount inline in the call (pushes the 1 before
   the inventory call, pops split; climb's best at 12 ins / 198 bytes), (has != 0) + 1, ? 50 : 25, ret =
   target != 0, ret types: no better. climb 2 rounds from this text: nothing else. prior: Read whole
   against the original and rewritten typed (AMOUNTCFG, AICHARREC user / target); fixed two meaning errors
   of the draft: the partner bit is 1 for a set sex byte and 2 otherwise (neg / sbb / add 2 / or dh,4), and
   he_ShowNews gets (netId, -1, text, 0, 0x5ec, 0). The structure follows the original: ret 0 in edi, path
   2 joins at if (target != 0) { ret = 1; .. }, the amount in a local (the pops merge, add esp,0x18 as the
   original). Left: where VC6 materialises the constant 1 in edi: the original sets mov edi,1 in path 1
   before the menu call and stores cfg.flags from it (+0x53 mov [esp+0x1c],edi), path 2 sets it after the
   null check; ours stores cfg.flags as an immediate, sets edi after the 0x2c5 pushes and pushes edi for
   cm_SendRelations' last argument where the original pushes 1; the rest is register renaming and argument
   load order (regs 32, order 6). Tried: the inventory call inside the argument list (12 ins but its pops
   split, the original's merge), ret = 1 after the send. */

int ai_UseGoodRaiseRelation(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x400];
    AICHARREC *target;
    int amount;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 1;
        cfg.mode = 0x400 | (lc_CharRecs[ai_PlayerId].sex != 0 ? 1 : 2);
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target == 0)
            return 0;
        ret = 1;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target != 0)
            ret = 1;
    }
    if (target != 0) {
        amount = (ai_InventoryHas(ch, 0x2c5) != 0 ? 2 : 1) * 25;
        cm_SendRelations(ch->netId, target->netId, amount, 1);
        if (target->kind == 6 || target->kind == 7) {
            txt_Format(news, 0xfe9, ch->id);
            he_ShowNews(target->netId, -1, news, 0, 0x5ec, 0);
        }
    }
    return ret;
}
/* 0x5d0c70, a message callback (484 B): param_1 the message record (+2 the type, +4 the handle), param_2
   the request record (+4 / +8 the two target ids). Type 6: the config cfg (20 dwords, [1] = 0x400, [2] /
   [3] = U_I (0x5eed68) / U_I (0x5eed6c), [4] = 6, [7 .. 14] = -1) with the text 0x5176d0 (0x9c) opens
   the first window 0x5c1130 (cfg, text, 0, 0); then [5] = 3 and the word [9] = the first record's id,
   the text 0x5176d0 (0x9e) opens the second window with the same cfg; other types: the two targets
   0x505df0 (param_2 + 4 / + 8); a missing record -> 0. Then the amount -40 or -80 by 0x5ba6b0 (param_1,
   0x2c5) and two 0x4c4b00 transfers between the two records (the handles +4, the amount, 1). When the
   first record's type is 6 / 7 the text 0x5176d0 (0xfea, the second record's word) goes through
   0x53dda0 (the first + 4, -1, 1, 0x5fb); when the second's is, the text 0x5176d0 (0xfeb, the first
   word twice) goes through 0x53dda0 (the second + 4, -1, 1, 0x5fb). Returns 1 (0 without records). */

int ai_QuarrelCallback(AICHARREC *ch, int *req)
{
    AMOUNTCFG cfg;
    char text[0x100];
    char text2[0x400];
    AICHARREC *a;
    AICHARREC *b;
    int amount;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, (char *)0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        a = (AICHARREC *)ai_AmountMenu((int)&cfg, text, 0, 0);
        if (a == 0)
            goto done;
        cfg.option14 = 3;
        cfg.ids[0] = a->id;
        txt_Format(text, (char *)0x9e);
        b = (AICHARREC *)ai_AmountMenu((int)&cfg, text, 0, 0);
        if (b == 0)
            goto done;
    } else {
        /* @original ret set to 0 again in this arm (already 0): its reference in the else block
           ranks req above ch, so ch takes ebx and ret's kind-6 piece esi */
        ret = 0;
        a = (AICHARREC *)gm_FindChar(req[1]);
        b = (AICHARREC *)gm_FindChar(req[2]);
        if (a == 0 || b == 0)
            goto done;
    }
    amount = (ai_InventoryHas(ch, 0x2c5) != 0 ? 2 : 1) * -40;
    cm_SendRelations(a->netId, b->netId, amount, 1);
    cm_SendRelations(b->netId, a->netId, amount, 1);
    if (a->kind == 6 || a->kind == 7) {
        txt_Format(text2, (char *)0xfea, b->id);
        he_ShowNews(a->netId, -1, text2, 1, 0x5fb, 0);
    }
    if (b->kind == 6 || b->kind == 7) {
        txt_Format(text2, (char *)0xfeb, a->id, a->id);
        he_ShowNews(b->netId, -1, text2, 1, 0x5fb, 0);
    }
    ret = 1;
done:
    return ret;
}
/* 0x5d0e60, the use callback of the Domizil good 0x168 (the +0x14 pointer of the table entry 0x62c5e0):
   marks the use handled, then 0x505ca0 (5, pick) gives a shuffled 0 .. 4 order and the first pick whose
   skill counter ch->skills[pick[i]] is below 0xfc runs the trial step 0x5d08c0 (ch, the pick) and is
   remembered. A kind 6 character: 0x4c5870 (ch->netId) and the text 0x5176d0 (0xfe8, the used object's
   id * 2 + 0xd71, the pick + 0x16b7) through the use-object panel 0x5cf820. Returns 1. */

int ai_UseGoodTrialStep(AICHARREC *ch, G8USEVIEW *use)
{
    int pick[5];
    char text[0x800];
    int i;
    int n;
    int found = 0;

    use->handled = 1;
    gm_Shuffled(5, pick);
    for (i = 0, n = 0; n < 1 && i < 5; i++) {
        if (ch->skills[pick[i]] < 0xfc) {
            ai_ApplyTrialStep((int)ch, pick[i]);
            found = pick[i];
            n++;
        }
    }
    if (ch->kind == 6) {
        cm_SendSkillsChanged(ch->netId);
        txt_Format(text, 0xfe8, use->obj->id * 2 + 0xd71, found + 0x16b7);
        ai_UseObjPanel(text);
    }
    return 1;
}
/* 0x5d0f20, the use callback of the Domizil good 0x16a (the +0x14 pointer of the table entry 0x62c5fc): marks the
   use handled and picks the target character, for a player (kind 6) with the special-amount menu (the gypsy
   filter, mode 0x400, the prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. Three random
   office holders (ai_Chars from a random start, steps 1 / 7 / 13: a used record of kind below 10 with an office,
   not the target, the user or an earlier pick) each get the target's relation lowered by 20, 40 when the user
   holds the good 0x2c5 (cm_SendRelations). A player target (kind 6 / 7) gets the news 0xfec naming the three,
   each player pick the news 0xfed naming the target (key 0x5fb); a player user sees the text 0xfe8 (the good's
   rank + 1 and the three) in the use-object panel. Returns 1. */

int ai_UseGoodLowerOfficialRelations(AICHARREC *ch, G8USEVIEW *use)
{
    AICHARREC *pick[3];
    int index;
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x800];
    AICHARREC *target;
    G9DOMIZIL *found;
    int level;
    int amount;
    int ret;
    int id;
    int n;
    int i;

    use->handled = 1;
    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target != 0) {
        level = ai_InventoryHas(ch, 0x2c5) != 0 ? 2 : 1;
        id = util_Random(0x300) & 0xffff;
        for (n = 0x300; n != 0; n--, id = (id + 1) % 0x300) {
            if (id != target->id && id != ch->id && lc_CharRecs[id].id != 0xffff && lc_CharRecs[id].kind < 10 &&
                lc_CharRecs[id].officeKind != 0) {
                pick[0] = &lc_CharRecs[id];
                break;
            }
        }
        id = util_Random(0x300) & 0xffff;
        for (n = 0x300; n != 0; n--, id = (id + 7) % 0x300) {
            if (id != target->id && id != ch->id && id != pick[0]->id && lc_CharRecs[id].id != 0xffff &&
                lc_CharRecs[id].kind < 10 && lc_CharRecs[id].officeKind != 0) {
                pick[1] = &lc_CharRecs[id];
                break;
            }
        }
        id = util_Random(0x300) & 0xffff;
        for (n = 0x300; n != 0; n--, id = (id + 13) % 0x300) {
            if (id != target->id && id != ch->id && id != pick[0]->id && id != pick[1]->id &&
                lc_CharRecs[id].id != 0xffff && lc_CharRecs[id].kind < 10 && lc_CharRecs[id].officeKind != 0) {
                pick[2] = &lc_CharRecs[id];
                break;
            }
        }
        amount = level * -20;
        cm_SendRelations(target->netId, pick[0]->netId, amount, 1);
        cm_SendRelations(target->netId, pick[1]->netId, amount, 1);
        cm_SendRelations(target->netId, pick[2]->netId, amount, 1);
        if (target->kind == 6 || target->kind == 7) {
            txt_Format(news, 0xfec, pick[0]->id, pick[1]->id, pick[2]->id);
            he_ShowNews(target->netId, -1, news, 1, 0x5fb, 0);
        }
        txt_Format(news, 0xfed, target->id);
        for (i = 0; i < 3; i++) {
            if (pick[i]->kind == 6 || pick[i]->kind == 7)
                he_ShowNews(pick[i]->netId, -1, news, 0, 0x5fb, 0);
        }
        if (ch->kind == 6) {
            found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
            txt_Format(news, 0xfe8, found->rank + 1, pick[0]->id, pick[1]->id, pick[2]->id);
            ai_UseObjPanel(news);
            ret = 1;
        } else {
            /* @original ret = 1 written in both arms: the arms' two return copies keep ebp's push in
               the prologue and ch->kind loaded into cl at the top */
            ret = 1;
        }
    }
    return ret;
}
/* 0x5d1300, the use callback of the Domizil good 0x16c (the +0x14 pointer of the table entry 0x62c634): like
   ai_UseGoodTrialStep with two steps - marks the use handled, gm_Shuffled (5, pick) gives a shuffled 0 .. 4
   order and the first two picks whose skill counter ch->skills[pick[i]] is below 0xfc run the trial step
   ai_ApplyTrialStep (ch, the pick) and are remembered. A kind 6 character: cm_SendSkillsChanged (ch->netId)
   and the text 0xfe8 (the used object's id * 2 + 0xd71, the second and the first pick + 0x16b7) through the
   use-object panel. Returns 1. */

int ai_UseGoodTwoTrialSteps(AICHARREC *ch, G8USEVIEW *use)
{
    int first;
    int pick[5];
    char text[0x800];
    int second;
    int i;
    int n;

    second = 0;
    first = 0;
    use->handled = 1;
    gm_Shuffled(5, pick);
    for (i = 0, n = 0; n < 2 && i < 5; i++) {
        if (ch->skills[pick[i]] < 0xfc) {
            ai_ApplyTrialStep((int)ch, pick[i]);
            n++;
            if (n == 1)
                first = pick[i];
            else if (n == 2)
                second = pick[i];
        }
    }
    if (ch->kind == 6) {
        cm_SendSkillsChanged(ch->netId);
        txt_Format(text, 0xfe8, use->obj->id * 2 + 0xd71, second + 0x16b7, first + 0x16b7);
        ai_UseObjPanel(text);
    }
    return 1;
}
/* 0x5d13e0, the use callback of the Domizil good 0x170 (the +0x14 pointer of the table entry 0x62c688): marks the
   use handled; three random characters of the other sex (ai_Chars from a random start, steps 1 / 7 / 13: a
   used record of kind below 10, not the user or an earlier pick; the first one must hold an office) each get
   their relation to the user raised by 20 (cm_SendRelations). Each player pick (kind 6 / 7) gets the news
   0xfef naming the user (key 0x5ec); a player user sees the text 0xfe8 (the good's rank + 1 and the three) in
   the use-object panel. Returns 1. */

int ai_UseGoodCharmOtherSex(AICHARREC *ch, G8USEVIEW *use)
{
    AICHARREC *pick[3];
    int index;
    char news[0x800];
    G9DOMIZIL *found;
    int id;
    int n;
    int i;

    use->handled = 1;
    id = util_Random(0x300) & 0xffff;
    for (n = 0x300; n != 0; n--, id = (id + 1) % 0x300) {
        if (id != ch->id && lc_CharRecs[id].id != 0xffff && lc_CharRecs[id].kind < 10 &&
            lc_CharRecs[id].sex != ch->sex && lc_CharRecs[id].officeKind != 0) {
            pick[0] = &lc_CharRecs[id];
            break;
        }
    }
    id = util_Random(0x300) & 0xffff;
    for (n = 0x300; n != 0; n--, id = (id + 7) % 0x300) {
        if (id != ch->id && id != pick[0]->id && lc_CharRecs[id].id != 0xffff && lc_CharRecs[id].kind < 10 &&
            lc_CharRecs[id].sex != ch->sex) {
            pick[1] = &lc_CharRecs[id];
            break;
        }
    }
    id = util_Random(0x300) & 0xffff;
    for (n = 0x300; n != 0; n--, id = (id + 13) % 0x300) {
        if (id != ch->id && id != pick[0]->id && id != pick[1]->id && lc_CharRecs[id].id != 0xffff &&
            lc_CharRecs[id].kind < 10 && lc_CharRecs[id].sex != ch->sex) {
            pick[2] = &lc_CharRecs[id];
            break;
        }
    }
    cm_SendRelations(ch->netId, pick[0]->netId, 20, 1);
    cm_SendRelations(ch->netId, pick[1]->netId, 20, 1);
    cm_SendRelations(ch->netId, pick[2]->netId, 20, 1);
    txt_Format(news, 0xfef, ch->id);
    for (i = 0; i < 3; i++) {
        if (pick[i]->kind == 6 || pick[i]->kind == 7)
            he_ShowNews(pick[i]->netId, -1, news, 0, 0x5ec, 0);
    }
    if (ch->kind == 6) {
        found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
        txt_Format(news, 0xfe8, found->rank + 1, pick[0]->id, pick[1]->id, pick[2]->id);
        ai_UseObjPanel(news);
    }
    return 1;
}
/* 0x5d1680, the use callback of the Domizil good 0x16d (the +0x14 pointer of the table entry 0x62c66c): picks
   two characters, for a player (kind 6) with two special-amount menus (the gypsy filter, mode 0x400, the
   prompts 0x9c and 0x9e; the second leaves out the first pick: option14 3, ids[0]), else the characters
   use->target and use->arg (gm_FindChar); a missing one -> 0. Their relations to each other fall by 25 both
   ways (cm_SendRelations); each player of the two (kind 6 / 7) gets the news 0xfee naming the other (key
   0x5fb). Returns 1. */

int ai_UseGoodSowDiscord(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x400];
    AICHARREC *a;
    AICHARREC *b;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        a = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (a == 0)
            goto done;
        cfg.option14 = 3;
        cfg.ids[0] = a->id;
        txt_Format(text, 0x9e);
        b = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        a = (AICHARREC *)gm_FindChar(use->target);
        b = (AICHARREC *)gm_FindChar(use->arg);
        if (a == 0)
            goto done;
    }
    if (b != 0) {
        ret = 1;
        cm_SendRelations(a->netId, b->netId, -25, 1);
        cm_SendRelations(b->netId, a->netId, -25, 1);
        if (a->kind == 6 || a->kind == 7) {
            txt_Format(news, 0xfee, b->id);
            he_ShowNews(a->netId, -1, news, 0, 0x5fb, 0);
        }
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(news, 0xfee, a->id);
            he_ShowNews(b->netId, -1, news, 0, 0x5fb, 0);
        }
    }
done:
    return ret;
}
/* 0x5d1830, the use callback of the Domizil good 0x15c (the +0x14 pointer of the table entry 0x62c4ac):
   walks the running tribute events 0x35 (0x53e400 (1, 0, 0x35) / 0x53e300) and, for each whose collector
   ev->idbc is ch->netId, shows the tribute claim news 0x5a8d80 (ch, the payer record ai_Chars[ev->chr]),
   sets the event's end time to ai_Time and gives it its slot (0x4c4c60, -1). Returns 1. */

int ai_UseGoodTributeClaim(AICHARREC *ch)
{
    AIJOB *ev;

    ev = (AIJOB *)he_FindEvent(1, 0, 0x35);
    while (ev != 0) {
        if ((int)ch->netId == ev->idbc) {
            ai_ShowTributeClaimNews((int)ch, (int)ai_Chars[ev->chr]);
            ev->end = ai_Time;
            cm_SendExSetHEI(-1, (char *)ev);
        }
        ev = (AIJOB *)he_NextEvent();
    }
    return 1;
}
/* 0x5d18c0, the use callback of the Domizil good 0x2b8 (the +0x14 pointer of the table entry 0x62c998): picks the
   target character, for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400, the
   prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. Sets the bit 0x100000 of the
   target's flags1cc (cm_SendSetField): the next round start (main_RundenBeginn) charges it 3 action points
   (the AP event 0xf18) unless it holds the good 0x2c4, and clears the bit. Returns 1. */

int ai_UseGoodDrainActionPoints(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x100];
    AICHARREC *target;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    cm_SendSetField(target->netId, 0x1cc, 4, 0x100000, 0);
    return 1;
}
/* 0x5d1990, the use callback of the Domizil good 0x2bb (the +0x14 pointer of the table entry 0x62c9ec): picks the
   target character, for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400, the
   prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. In a shuffled 0 .. 4 order
   (gm_Shuffled) the first skill in which the user is above 21 is lowered by 21 for the target
   (cm_SendAddSkill); a player target (kind 6 / 7) gets the news 0x100a naming the skill (0x16b7 + it, key
   0x5fb), and a player user sees the text 0x1099 (the target, the skill) inside 0xfe8 in the use-object
   panel. Marks the use handled and returns 1. */

int ai_UseGoodDrainSkill(AICHARREC *ch, G8USEVIEW *use)
{
    int pick[5];
    AMOUNTCFG cfg;
    char text[0x200];
    char news[0x800];
    AICHARREC *target;
    int i;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    gm_Shuffled(5, pick);
    for (i = 0; i < 5; i++) {
        if (ch->skills[pick[i]] > 0x15) {
            cm_SendAddSkill(target->netId, pick[i], -0x15);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0x100a, pick[i] + 0x16b7);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            if (ch->kind == 6) {
                txt_Format(text, 0x1099, target->id, pick[i] + 0x16b7);
                txt_Format(news, 0xfe8, text);
                ai_UseObjPanel(news);
            }
            break;
        }
    }
    use->handled = 1;
    return 1;
}
/* 0x5d1b30, the use callback of the Domizil good 0x2bc (the +0x14 pointer of the table entry 0x62ca08): picks a
   building and a character, for a player (kind 6) on the town map (ai_MapPick with the prompt txt_Ptrs[0x9b],
   the class filter 0x1a0744300, kind 6) and then with the special-amount menu (the gypsy filter, mode 0x400,
   leaving out the building's owner: option14 3, ids[0]; the prompt txt_Ptrs[0x9c]), else the character
   use->target (gm_FindChar) and the building use->arg (gm_FindById); a missing one -> 0. The character's
   relation to the building's owner falls by 20 + a random 0 .. 31 (cm_SendRelations, handed the character's
   id), and every kind 0 character in the building loses 10 .. 30% of its life (gm_AddHealth). A player
   character (kind 6 / 7) gets the news 0x100b (the owner, the building; key 0x5fb), a player user sees the
   text 0x109b (the character, the owner, the building) inside 0xfe8 in the use-object panel. Marks the use
   handled and returns 1. */

int ai_UseGoodHarmBuilding(AICHARREC *ch, G8USEVIEW *use)
{
    AIBLD *bld;
    AMOUNTCFG cfg;
    char text[0x200];
    char news[0x800];
    AICHARREC *target;
    int i;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        cfg.flags = 1;
        cfg.mode = -1;
        cfg.kind = 6;
        cfg.filter = 0x1a0744300;
        bld = (AIBLD *)ai_MapPick(1, &cfg, txt_Ptrs[0x9b], 0, 0);
        if (bld == 0)
            return 0;
        cfg.option14 = 3;
        cfg.ids[0] = bld->owner;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, txt_Ptrs[0x9c], 0, 0);
        if (target == 0)
            return 0;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target == 0)
            return 0;
        gm_FindById((int *)&bld, 0, 0, use->arg);
        if (bld == 0)
            return 0;
    }
    cm_SendRelations(target->id, lc_CharRecs[bld->owner].netId, -20 - (util_Random(0x20) & 0xffff), 1);
    for (i = 0; i < 0x300; i++) {
        if (lc_CharRecs[i].kind == 0 && lc_CharRecs[i].bld == bld)
            gm_AddHealth(i, -((util_RandomF() * 0.2f + 0.1f) * lc_CharRecs[i].f01c));
    }
    if (target->kind == 6 || target->kind == 7) {
        txt_Format(news, 0x100b, bld->owner, bld->owner, bld);
        he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
    }
    if (ch->kind == 6) {
        txt_Format(text, 0x109b, target->id, bld->owner, bld);
        txt_Format(news, 0xfe8, text);
        ai_UseObjPanel(news);
    }
    use->handled = 1;
    return 1;
}
/* 0x5d1da0, the use callback of the Domizil good 0x2bd (the +0x14 pointer of the table entry 0x62ca24): picks a
   building, for a player (kind 6) on the town map (ai_MapPick with the prompt txt_Ptrs[0x9b], the class filter
   0x1a0744300, kind 6), else the building use->target (gm_FindById); none -> 0. In the building's store (the
   object kind 6 / 0x2a of its objList, gm_Query) every good held more than once loses 10 .. 30% of its
   count, at least 1 (cm_SendExSellObject from the store). A player owner (kind 6 / 7) gets the news 0x100c
   naming the building (key 0x5fb), a player user sees the text 0x109d (the owner, the building) inside 0xfe8
   in the use-object panel. Marks the use handled and returns 1. */

int ai_UseGoodSpoilStock(AICHARREC *ch, G8USEVIEW *use)
{
    AIBLD *bld;
    AMOUNTCFG cfg;
    char text[0x200];
    char news[0x800];
    AICHARREC *owner;
    AIOBJ *store;
    AIOBJ *item;
    int n;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        cfg.flags = 1;
        cfg.mode = -1;
        cfg.kind = 6;
        cfg.filter = 0x1a0744300;
        bld = (AIBLD *)ai_MapPick(1, &cfg, txt_Ptrs[0x9b], 0, 0);
        if (bld == 0)
            return 0;
    } else {
        gm_FindById((int *)&bld, 0, 0, use->target);
        if (bld == 0)
            return 0;
    }
    owner = &lc_CharRecs[bld->owner];
    store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
    if (store != 0) {
        gm_Query((void *)store->list, 1, 5);
        for (item = (AIOBJ *)gm_Query((void *)store->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
            if (item->count > 1) {
                n = (int)((util_RandomF() * 0.2f + 0.1f) * item->count) < 1
                        ? 1
                        : (int)((util_RandomF() * 0.2f + 0.1f) * item->count);
                cm_SendExSellObject(-1, store->objId, item->id, n, ai_OwnPlayer, 0);
            }
        }
    }
    if (owner->kind == 6 || owner->kind == 7) {
        txt_Format(news, 0x100c, bld);
        he_ShowNews(owner->netId, -1, news, 0, 0x5fb, 0);
    }
    if (ch->kind == 6) {
        txt_Format(text, 0x109d, bld->owner, bld);
        txt_Format(news, 0xfe8, text);
        ai_UseObjPanel(news);
    }
    use->handled = 1;
    return 1;
}
extern const int ai_CharWalkSteps[16];  /* 0x5ed100, the random walk steps (am.h, not included) */
/* 0x5d1fb0, the use callback of the Domizil record 57 (0x62ca5c, the text id 0x2bf): picks the target
   character, for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400, option14 3,
   the prompt txt_Ptrs[0x9c]), else the character use->target (gm_FindChar); none -> 0. Three office holders
   (ai_Chars from a random start with a random ai_CharWalkSteps step: a used record holding an office, not the
   user nor the target) each get the target's relation to them raised by 20 .. 51 (cm_SendRelations), and the
   target loses 50 .. 113 life (gm_AddHealth). The target, if a player (kind 6 / 7), gets the news 0x100d,
   each player office holder 0x100e / 0x100f (key 0x5fb); marks the use handled, and a player user sees the
   text 0x10a1 (the target, the three) inside 0xfe8 in the use-object panel. Returns 1. */

int ai_UseGoodCharmOfficials(AICHARREC *ch, G8USEVIEW *use)
{
    AICHARREC *pick[3];
    AMOUNTCFG cfg;
    char news[0x800];
    char text[0x200];
    AICHARREC *target;
    int id;
    int step;
    int n;
    int i;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        cfg.option14 = 3;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.classMask[0] = gypsyFilterWords[0];
        cfg.classMask[1] = gypsyFilterWords[1];
        target = (AICHARREC *)ai_AmountMenu(&cfg, txt_Ptrs[0x9c], 0, 0);
        if (target == 0)
            return 0;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target == 0)
            return 0;
    }
    id = util_RandomU(0x300);
    step = ai_CharWalkSteps[util_RandomU(0x10)];
    n = 3;
    do {
        if (lc_CharRecs[id].id != 0xffff && lc_CharRecs[id].officeKind != 0 && &lc_CharRecs[id] != ch &&
            &lc_CharRecs[id] != target)
            pick[--n] = &lc_CharRecs[id];
        id = (id + step) % 0x300;
    } while (n != 0);
    for (i = 0; i < 3; i++)
        cm_SendRelations(target->netId, pick[i]->netId, (util_Random(0x20) & 0xffff) + 20, 1);
    gm_AddHealth(target->id, -50 - (util_Random(0x40) & 0xffff));
    if (target->kind == 6 || target->kind == 7) {
        txt_Format(news, 0x100d, pick[0]->id, pick[0]->id, pick[1]->id, pick[2]->id);
        he_ShowNews(target->netId, -1, news, 1, 0x5fb, 0);
    }
    if (pick[0]->kind == 6 || pick[0]->kind == 7) {
        txt_Format(news, 0x100e, target->id, pick[1]->id, pick[2]->id);
        he_ShowNews(pick[0]->netId, -1, news, 1, 0x5fb, 0);
    }
    if (pick[1]->kind == 6 || pick[1]->kind == 7) {
        txt_Format(news, 0x100f, target->id, pick[0]->id, pick[2]->id);
        he_ShowNews(pick[1]->netId, -1, news, 1, 0x5fb, 0);
    }
    if (pick[2]->kind == 6 || pick[2]->kind == 7) {
        txt_Format(news, 0x100f, target->id, pick[0]->id, pick[1]->id);
        he_ShowNews(pick[2]->netId, -1, news, 1, 0x5fb, 0);
    }
    use->handled = 1;
    if (ch->kind == 6) {
        txt_Format(text, 0x10a1, target->id, pick[0]->id, pick[1]->id, pick[2]->id);
        txt_Format(news, 0xfe8, text);
        ai_UseObjPanel(news);
    }
    return 1;
}
/* 0x5d22e0, the use callback of the Domizil record 59 (0x62ca94, the text id 0x2c1, the mask 0x5a0): a
   character of kind 6 needs sv_CheckMoney (ch, 3) to pass (it shows the text 0x74 "not enough money"), every
   other kind needs 3 money silently; then the money delta -3 (0x50cc50, ch->netId) and 1, 0 otherwise. */

int ai_UsePayGood(G8CHARREC *ch)
{
    if (ch->kind == 6) {
        if (sv_CheckMoney((char *)ch, 3) == 0)
            return 0;
    } else if (ch->money < 3) {
        return 0;
    }
    gm_SendMoneyDelta(ch->netId, -3);
    return 1;
}
/* 0x5d2320, the use callback of the Domizil record 60 (0x62cab0, the text id 0x2c2): picks the target character,
   for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400, the prompt 0x9c), else the
   character use->target (gm_FindChar); none -> 0. Sets the bit 0x200000 of the target's flags1cc
   (cm_SendSetField): the next round start (main_RundenBeginn) charges it 8 action points (the AP event 0xf2c)
   unless it holds the good 0x2c4, and clears the bit. Marks the use handled; a player user sees the text 0x10a7
   (the target) inside 0xfe8 in the use-object panel. Returns 1. */

int ai_UseGoodDrainActionPoints8(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x200];
    char news[0x400];
    AICHARREC *target;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    cm_SendSetField(target->netId, 0x1cc, 4, 0x200000, 0);
    use->handled = 1;
    if (ch->kind == 6) {
        txt_Format(text, 0x10a7, target->id);
        txt_Format(news, 0xfe8, text);
        ai_UseObjPanel(news);
    }
    return 1;
}
/* 0x5d2440, the use callback of the Domizil record 61 (0x62cacc, the text id 0x2c3): picks the target character,
   for a player (kind 6) with the special-amount menu (the gypsy filter, mode 0x400, the prompt 0x9c), else the
   character use->target (gm_FindChar); none -> 0. Unless the target holds the good 0x2c4 and util_RandomF() <=
   0.75, it is held one round more (cm_SendMessage5b, heldRounds + 1). Marks the use handled; a player user sees
   the text 0x10a9 (the target) inside 0xfe8 in the use-object panel. Returns 1. */

int ai_UseGoodHoldRound(AICHARREC *ch, G8USEVIEW *use)
{
    AMOUNTCFG cfg;
    char text[0x200];
    char news[0x400];
    AICHARREC *target;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    if (ai_InventoryHas(target, 0x2c4) == 0 || util_RandomF() > 0.75f)
        cm_SendMessage5b(target->netId, 1, 0);
    use->handled = 1;
    if (ch->kind == 6) {
        txt_Format(text, 0x10a9, target->id);
        txt_Format(news, 0xfe8, text);
        ai_UseObjPanel(news);
    }
    return 1;
}
/* 0x5d2580, the use callback of the Domizil record 32 (0x62c7a0, the text id 0x229): picks the target character,
   for a player (kind 6) with the special-amount menu (flags 1, the gypsy filter, the prompt 0x9c, mode 0x400
   with the partner bit 1 or 2 by the player's sex), else the character use->target (gm_FindChar); none -> 0.
   The target's relation to the user rises by 25 (cm_SendRelations); a player target (kind 6 / 7) gets the news
   0xffa naming the user (key 0x5ec). A player user sees the text 0xfe8 (the good's rank + 1, the target) in the
   use-object panel, and the use is marked handled. Returns 1.
   @deep deep s26: climb round 1 (52 mutants): nothing. c2trace: the constant 6 (ch->kind test, cfg.kind,
   target->kind, the second ch->kind test; byte compares, prio 7) takes ebx for the whole function; the
   constant 1 (ret = 1 twice, cfg.flags, the relations argument, handled; prio 6) is cut by it and its
   pieces go to memory (savings -1 even with edi = ret available). The original: 6 in ebx only for the
   first test / cfg.kind, 1 in edi (ret's register) for the relations push and handled. = control: if (ret)
   / if (ret == 1) as the block test (VC6 threads both). ret = 1 as the block's first statement (and the
   else's ret = 1 dropped, with or without the first path's): 11 ins, push edi / handled edi right but the
   first path's mov edi,1 is then dead. Left: code 8 (the two immediates 6, push 1, handled = 1). prior:
   Read whole against the original and written typed (AMOUNTCFG with the sex-dependent partner bit,
   AICHARREC target, G9DOMIZIL found); every call, argument, constant, store and branch matches in order.
   Left is VC6's constant-in-register choice: the original holds ret (edi) = 1 and uses it for the 4th
   cm_SendRelations argument (push edi at +0xe3) and use->handled (mov [ebx+0x10], edi at +0x154), and
   compares kind with the immediate 6 (+0xf3, +0x133) where ours keeps 6 in ebx (cmp al, bl) and pushes 1;
   plus the filter's high dword stored after the call setup (+0x9e, order). Tried: handled = ret /
   relations(ret) (no change), ret = 1 placement in both paths and a goto done tail (worse, 15 / 36 ins),
   the mode / filter store order (fixed: 39 -> 12). */

int ai_UseGoodCharmPartner(AICHARREC *ch, G8USEVIEW *use)
{
    int index;
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x400];
    AICHARREC *target;
    G9DOMIZIL *found;
    int ret;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        ret = 1;
        cfg.flags = 1;
        cfg.kind = 6;
        cfg.mode = ((lc_CharRecs[ai_PlayerId].sex != 0) + 1) | 0x400;
        cfg.classMask[0] = gypsyFilterWords[0];
        cfg.classMask[1] = gypsyFilterWords[1];
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target == 0)
            return 0;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target != 0)
            ret = 1;
    }
    if (target != 0) {
        cm_SendRelations(ch->netId, target->netId, 25, 1);
        if (target->kind == 6 || target->kind == 7) {
            txt_Format(news, 0xffa, ch->id);
            he_ShowNews(target->netId, -1, news, 0, 0x5ec, 0);
        }
        if (ch->kind == 6) {
            found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
            use->handled = 1;
            txt_Format(news, 0xfe8, found->rank + 1, target->id);
            ai_UseObjPanel(news);
        }
    }
    return ret;
}
/* 0x5d2720, the use callback of the Domizil record 34 (0x62c7d8, the text id 0x22c), a twin of
   ai_UseGoodWeakenSkills: picks the target character, for a player (kind 6) with the special-amount menu (the
   gypsy filter, mode 0x400, the prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. Sends it
   the skill-bonus message 0x82: skill 3 loses 0.35 for 48 hours (flags 2; skill2 is 5, bonus2 is left unset as
   the original leaves it). A player target (kind 6 / 7) gets the news 0xffb (key 0x5fb); a player user sees the
   text 0xfe8 (the good's rank + 1, the target) in the use-object panel and the use is marked handled. Returns 1.
   Both paths are written out in full, as the original's code has them. */

int ai_UseGoodWeakenSkills3(AICHARREC *ch, G8USEVIEW *use)
{
    int index;
    AMOUNTCFG cfg;
    char text[0x100];
    char news[0x800];
    AICHARREC *target;
    G9DOMIZIL *found;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 0;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target != 0) {
            AIMSG msg;
            msg.type = 0x82;
            msg.to = target->netId;
            msg.fromId = -1;
            msg.flags = 2;
            msg.skillBonus.skill1 = 3;
            msg.skillBonus.bonus1 = -0.35f;
            msg.skillBonus.skill2 = 5;
            msg.skillBonus.hours = 48;
            cm_SendLongMessage((char *)&msg);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xffb);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
            use->handled = 1;
            txt_Format(news, 0xfe8, found->rank + 1, target->id);
            ai_UseObjPanel(news);
            return 1;
        }
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target != 0) {
            AIMSG msg;
            msg.type = 0x82;
            msg.to = target->netId;
            msg.fromId = -1;
            msg.flags = 2;
            msg.skillBonus.skill1 = 3;
            msg.skillBonus.bonus1 = -0.35f;
            msg.skillBonus.skill2 = 5;
            msg.skillBonus.hours = 48;
            cm_SendLongMessage((char *)&msg);
            if (target->kind == 6 || target->kind == 7) {
                txt_Format(news, 0xffb);
                he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
            }
            return 1;
        }
    }
    return 0;
}
extern AIDEC gv_Decisions[];  /* 0x611258, the 0x1d AI decision (law) records (gv.h, not included) */
/* 0x5d2960, the use callback of the Domizil record 36 (0x62c810, the text id 0x22b): picks the target character,
   for a player (kind 6) with the special-amount menu (flags 1, the gypsy filter, mode 0x400, the prompt 0x9c),
   else the character use->target (gm_FindChar); none -> 0. Collects the laws in force (the gv_Decisions records
   shown by name, valueText 2, with a rule and a nonzero value); none: the target gets the news 0xffe (key 0x5fa)
   and the use fails (0). Else the target is accused of one at random (gv_Accuse, the user as accuser, the user's
   home building), a second time when the user holds the good 0x2c5. A player target (kind 6 / 7) gets the news
   0xffe (key 0x5fb); a player user sees the text 0xfe8 (the good's rank + 1, the target) in the use-object panel
   and the use is marked handled. Returns 1. */

int ai_UseGoodFrameChar(AICHARREC *ch, G8USEVIEW *use)
{
    int ret;
    int index;
    AMOUNTCFG cfg;
    int laws[0x1d];
    char text[0x100];
    char news[0x400];
    AICHARREC *target;
    int i;
    G9DOMIZIL *found;
    int twice;
    int n;

    ret = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 1;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target == 0)
            goto done;
        ret = 1;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target == 0)
            goto done;
        ret = 1;
    }
    twice = ai_InventoryHas(ch, 0x2c5) != 0;
    n = 0;
    for (i = 0; i < 0x1d; i++) {
        if (gv_Decisions[i].valueText == 2 && (char)gv_Decisions[i].rule != 0 && gv_Decisions[i].cur != 0)
            laws[n++] = gv_Decisions[i].id;
    }
    if (n == 0) {
        txt_Format(news, 0xffe);
        he_ShowNews(target->netId, -1, news, 0, 0x5fa, 0);
        return 0;
    }
    i = rand() % n;
    gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
    if (twice) {
        i = rand() % n;
        gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
    }
    if (target->kind == 6 || target->kind == 7) {
        txt_Format(news, 0xffe);
        he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
    }
    if (ch->kind == 6) {
        found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
        use->handled = 1;
        txt_Format(news, 0xfe8, found->rank + 1, target->id);
        ai_UseObjPanel(news);
    }
done:
    return ret;
}
/* 0x5d2bc0, the use callback of the Domizil record 37 (0x62c82c, the text id 0x22d), the twin of
   ai_UseGoodFrameChar: the same target pick (flags 1, the gypsy filter, mode 0x400, the prompt 0x9c) and the
   laws in force (none: the news 0xfff, key 0x5fa, and 0); the target is accused twice of a law at random
   (gv_Accuse, the user as accuser, the user's home building), four times when the user holds the good 0x2c5.
   A player target (kind 6 / 7) gets the news 0xfff (key 0x5fb); a player user sees the text 0xfe8 (the good's
   rank + 1, the target) in the use-object panel and the use is marked handled. Returns 1. */

int ai_UseGoodFrameChar2(AICHARREC *ch, G8USEVIEW *use)
{
    int ret;
    int index;
    AMOUNTCFG cfg;
    int laws[0x1d];
    char text[0x100];
    char news[0x400];
    AICHARREC *target;
    int i;
    G9DOMIZIL *found;
    int twice;
    int n;

    ret = 0;
    n = 0;
    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 1;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
        if (target == 0)
            goto done;
        ret = 1;
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
        if (target == 0)
            goto done;
        ret = 1;
    }
    twice = ai_InventoryHas(ch, 0x2c5) != 0;
    for (i = 0; i < 0x1d; i++) {
        if (gv_Decisions[i].valueText == 2 && (char)gv_Decisions[i].rule != 0 && gv_Decisions[i].cur != 0)
            laws[n++] = gv_Decisions[i].id;
    }
    if (n == 0) {
        txt_Format(news, 0xfff);
        he_ShowNews(target->netId, -1, news, 0, 0x5fa, 0);
        return 0;
    }
    i = rand() % n;
    gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
    i = rand() % n;
    gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
    if (twice) {
        i = rand() % n;
        gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
        i = rand() % n;
        gv_Accuse(laws[i], 1, target->netId, ch->netId, ch->bld2 != 0 ? ((AIBLD *)ch->bld2)->id : -1);
    }
    if (target->kind == 6 || target->kind == 7) {
        txt_Format(news, 0xfff);
        he_ShowNews(target->netId, -1, news, 0, 0x5fb, 0);
    }
    if (ch->kind == 6) {
        found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
        use->handled = 1;
        txt_Format(news, 0xfe8, found->rank + 1, target->id);
        ai_UseObjPanel(news);
    }
done:
    return ret;
}
/* 0x5d2e80, the use callback of the Domizil good 0x2b2 (the heating, the +0x14 pointer of the table entry
   0x62c8f0): a kind 6 character picks the building on the map (0x5b2180 (1, cfg, txt_Ptrs[0x9b], 0, 0),
   cfg = 0x410 flags, the 0xa0400300 filter, mode 1, kind 4), every other kind takes the use's target
   (0x502020). Then the building's flags5aw bit 0x2000 (the heating, cleared at the round settlement) is set
   with 0x4c4a60 (id, 0x5a, 2, 0x2000, 0). Returns 1 (0 without a building). */

int ai_UseGoodHeatBuilding(AICHARREC *ch, G8USEVIEW *use)
{
    int cfg[20];
    int i;
    AIBLD *bld;

    if (ch->kind == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        cfg[1] = 0x410;
        cfg[4] = 4;
        cfg[2] = 0xa0400300;
        cfg[3] = 1;
        cfg[0] = 0;
        bld = (AIBLD *)ai_MapPick(1, cfg, txt_Ptrs[0x9b], 0, 0);
    } else {
        bld = (AIBLD *)gm_FindBuildingById(use->target);
    }
    if (bld == 0)
        return 0;
    cm_SendSetField(bld->id, 0x5a, 2, 0x2000, 0);
    return 1;
}
/* 0x5d2f20, the use callback of the Domizil record 49 (0x62c97c, the text id 0x2b7, the value 0x10e0):
   sends the message type 0xa4 (the event 0xa4, he.c 0x5753a0) to the character's netId, fromId -1, the
   current clock ai_Time and the flag 1 (0x4c4b70); returns 1. */

int ai_UseEventGood(G8CHARREC *ch)
{
    AIMSG msg;

    msg.to = ch->netId;
    msg.type = 0xa4;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 1;
    cm_SendLongMessage((char *)&msg);
    return 1;
}
/* 0x5d2f90, the use callback of the Domizil good 0x298 (the twin of the heating good 0x2b2, the +0x14
   pointer of the table entry 0x62c864): a kind 6 character picks the building on the map (0x5b2180 (1,
   cfg, ch_EmptyName, 0, 0), cfg = 0x410 flags, the 0xa0747980 filter, mode 1, kind 4), every other kind
   takes the use's target (0x502020). Then the building's flags5aw bit 0x1000 (0.1 more staff factor in
   gm_StaffFactor, cleared at the round settlement) is set with 0x4c4a60 (id, 0x5a, 2, 0x1000, 0). Returns
   1 (0 without a building). */

int ai_UseGoodStaffBonus(AICHARREC *ch, G8USEVIEW *use)
{
    int cfg[20];
    int i;
    AIBLD *bld;

    if (ch->kind == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        cfg[1] = 0x410;
        cfg[4] = 4;
        cfg[2] = 0xa0747980;
        cfg[3] = 1;
        cfg[0] = 0;
        bld = (AIBLD *)ai_MapPick(1, cfg, ch_EmptyName, 0, 0);
        if (bld == 0)
            return 0;
    } else {
        bld = (AIBLD *)gm_FindBuildingById(use->target);
        if (bld == 0)
            return 0;
    }
    cm_SendSetField(bld->id, 0x5a, 2, 0x1000, 0);
    return 1;
}
/* 0x5d3030, the use callback of the Domizil record 43 (0x62c8d4, the text id 0x29c): picks the target character,
   for a player (kind 6) with the special-amount menu (flags 1, the gypsy filter, mode 0x400, the prompt 0x9c),
   else the character use->target (gm_FindChar); none -> 0. Sends it the event message 0xa3 (the petty loss of
   he.c's he_AllocPettyLoss / he_RunPettyLoss: it loses money over the next rounds; now, flags 1). A player user
   sees, inside 0xfe8, the text whose id is the good's rank + 1 (formatted with the target) in the use-object
   panel. Marks the use handled and returns 1. */

int ai_UseGoodPettyLoss(AICHARREC *ch, G8USEVIEW *use)
{
    int index;
    AMOUNTCFG cfg;
    char text[0x100];
    char line[0x200];
    char news[0x400];
    AICHARREC *target;
    G9DOMIZIL *found;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 1;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    {
        AIMSG msg;

        msg.type = 0xa3;
        msg.to = target->netId;
        msg.fromId = -1;
        msg.time = ai_Time;
        msg.flags = 1;
        cm_SendLongMessage((char *)&msg);
    }
    if (ch->kind == 6) {
        found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
        txt_Format(line, found->rank + 1, target->id);
        txt_Format(news, 0xfe8, line);
        ai_UseObjPanel(news);
    }
    use->handled = 1;
    return 1;
}
/* 0x5d31c0, the use callback of the Domizil good 0x297 (one of am.c's education goods 0x168 / 0x16c /
   0x297 / 0x2b7, the +0x14 pointer of the table entry 0x62c848): posts two skill-bonus messages 0x82
   (0x4c4b70, an AIMSG) to ch->netId, one raising skills 0 and 4 by 1/6, one lowering skills 3 and 2 by
   1/6, each lasting 24 hours. Returns 1. */

int ai_UseGoodSkillBonus(AICHARREC *ch)
{
    union {
        AIMSG msg;
        struct {
            char pad58[0x58];
            unsigned char skill1;      /* 0x58, the first skill number 0 .. 4 */
            unsigned char skill2;      /* 0x59, the second skill number */
            char pad5a[2];
            float bonus1;              /* 0x5c, the bonus added for skill1 */
            float bonus2;              /* 0x60, the bonus added for skill2 */
            int hours;                 /* 0x64, the hours the bonus lasts */
        } skill;
    } m;

    m.msg.type = 0x82;
    m.msg.to = ch->netId;
    m.msg.fromId = -1;
    m.msg.flags = 2;
    m.skill.skill1 = 0;
    m.skill.bonus1 = 1.0f / 6.0f;
    m.skill.skill2 = 4;
    m.skill.bonus2 = 1.0f / 6.0f;
    m.skill.hours = 0x18;
    cm_SendLongMessage((char *)&m.msg);
    m.msg.type = 0x82;
    m.msg.to = ch->netId;
    m.msg.fromId = -1;
    m.msg.flags = 2;
    m.skill.skill1 = 3;
    m.skill.bonus1 = -1.0f / 6.0f;
    m.skill.skill2 = 2;
    m.skill.bonus2 = -1.0f / 6.0f;
    m.skill.hours = 0x18;
    cm_SendLongMessage((char *)&m.msg);
    return 1;
}
#define LC_F1CC_DISTRUST 0x400000  /* AICHARREC flags1cc: the distrust bit (poker.c AIPER_F1CC_DISTRUST: -0.2 trust) */
/* 0x5d3270, the use callback of the Domizil good 0x29a (record 41 at 0x62c89c of the table 0x62c420): picks the
   target character, for a player (kind 6) with the special-amount menu (flags 1, the gypsy filter, mode 0x400,
   the prompt 0x9c), else the character use->target (gm_FindChar); none -> 0. Sets the target's flags1cc bit
   0x400000 (the distrust bit poker.c's AIPER_F1CC_DISTRUST reads: -0.2 trust). A player user sees, inside 0xfe8,
   the text whose id is the good's rank + 1 (formatted with the target) in the use-object panel. Marks the use
   handled and returns 1. */

int ai_UseGoodDistrust(AICHARREC *ch, G8USEVIEW *use)
{
    int index;
    AMOUNTCFG cfg;
    char text[0x100];
    char line[0x200];
    char news[0x400];
    AICHARREC *target;
    G9DOMIZIL *found;

    if (ch->kind == 6) {
        memset(&cfg, 0, sizeof(cfg));
        memset(cfg.ids, 0xff, sizeof(cfg.ids));
        txt_Format(text, 0x9c);
        cfg.flags = 1;
        cfg.mode = 0x400;
        cfg.kind = 6;
        cfg.filter = gypsyFilter;
        target = (AICHARREC *)ai_AmountMenu(&cfg, text, 0, 0);
    } else {
        target = (AICHARREC *)gm_FindChar(use->target);
    }
    if (target == 0)
        return 0;
    cm_SendSetField(target->netId, 0x1cc, 4, LC_F1CC_DISTRUST, 0);
    if (ch->kind == 6) {
        found = (G9DOMIZIL *)ai_FindUsableGoodIndex(use->obj->id, &index);
        txt_Format(line, found->rank + 1, target->id);
        txt_Format(news, 0xfe8, line);
        ai_UseObjPanel(news);
    }
    use->handled = 1;
    return 1;
}

/* the unit's data in the original's order: .data ai_AmountTabs 0x62d148 (the first of the block, after lc.c's strings) and the
   form strings, .bss 0x6f2f88 .. 0x6f2f90 (after lc.c's 0x6f2f60 .. 0x6f2f84) */
const int ai_AmountTabCount = 8;  /* 0x5eede0, the unit's first .rdata (a named int after lc.c's 128.0): the number of tabs of
   ai_AmountTabs; no reader in the surviving code (the loops use the immediate 8). Name a guess from the value and the place */
AIAMOUNTTAB ai_AmountTabs[8] = {       /* 0x62d148, one table: 8 rows of 0x14 {id 0..7, count, control, two handlers}; ai_ClearAmountTabs clears count and control, the ids run 0..7 from 0x62d148 and row 7 ends where "%G" starts */
    { 0, 0, -1, ai_FillFamilyTab, ai_ShowFamilyTab },
    { 1, 0, -1, ai_FillFriendsTab, ai_ShowFriendsTab },
    { 2, 0, -1, ai_FillEnemiesTab, ai_ShowEnemiesTab },
    { 3, 0, -1, ai_FillLoversTab, ai_ShowLoversTab },
    { 4, 0, -1, ai_FillWeddingTab, ai_ShowWeddingTab },
    { 5, 0, -1, ai_FillQuarrelTab, ai_ShowQuarrelTab },
    { 6, 0, -1, ai_FillCandidacyTab, ai_ShowCandidacyTab },
    { 7, 0, -1, ai_FillOfficeTab, ai_ShowOfficeTab },
};
unsigned short ai_StatsChartFlags = 0;  /* 0x6f2f88, the charts the statistics panel shows (ai_CityStatsPanel: bit i the legend row i, 1 at the first open) */
unsigned int ai_AmountTab = 0;  /* 0x6f2f8c, the open tab of the special-amount window (ai_SpecialAmountWindow, 0 .. 7; 8: no tab has rows) */
int ai_MasterLetterPage = 0;  /* 0x6f2f90, the open page of the master letter window (ai_MasterLetterSettings): 0 the work settings, 1 the goods weights */
