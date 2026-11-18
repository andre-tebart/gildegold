/* lcmap -- the town map window and the messages after it (0x5a6b50 .. 0x5a9e90), split from lc; the name is ours.
   The city name text and map mark helpers, the town map window (ai_OpenTownMap, ai_BuildTownMap, ai_UpdateTownMap
   with the drag / key scroll, its close), the production settings ("hp_" chain ai_Settings, ai_FindSetting ..),
   the item group lookup ai_FindItemGroup and the plague / gypsy / graveyard / partner / Domizil news (their sound
   keys (string literals), the text buffers ai_NewsText / ai_NewsSecond).
   Boundary: lc's .data strings end with "contact_RAZZIA" 0x62c25c (user 0x5a6995 in 0x5a6920); this object's plain
   .data block is ai_MapScrollEnabled 0x62c26c (1) and ai_MapCellScale 0x62c270 (users 0x5a6b8c ..), its .bss starts
   at 0x6f268c (after lc's ai_BauplatzCount 0x6f2688) and holds rt_MapSurface 0x6f26ac, its .rdata starts with the
   named masks 0x5eed18 .. 0x5eed88 (ai_KindMaskTable, gypsyFilter, ai_BuildingTypeMasks) after lc's last literal
   1600000.0 0x5eed10 (user 0x5a5698), then its literals 52.0f, 0.435f, 0.48f. The first function 0x5a6b50 is
   called only by ai_BuildTownMap; the one before, 0x5a6a90, is lc's training room listener.
   Open: the head declarations are lc.c's as the split copied them (types, views, prototypes); trim them to what
   the unit uses once lc.h / lcmap.h carry the shared ones.
   Open: data: gypsyFilter and ai_BuildingTypeMasks keep their declarations in menu.h / ai.h (their users include
   those); move them to lcmap.h when the users can include it. */
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
#include "lcmap.h"
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
#include "lcdom.h"
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

/* dcp unit split: the definitions of lc the functions of lcmap use */
void ai_LayoutProdGoodsList(int view, int part, int pointer, int bld);
void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld);
void lc_PlayerInJail(void);
int ai_SearchResourceWindow(AIBLD *bld);
int ai_ExtractResourceWindow(AIBLD *bld);
void ai_Top5Window(void);

/* 0x5a6b50, the text of a city name (24 B): 0x5176d0 formats the value with "%1G" 0x61168c into buf */

void ai_FormatCityName(int value, char *buf)
{
    txt_Format(buf, "%1G", value);
}
/* 0x5a6b70, the town map position of the map mark m: its scene cell (cellX, cellZ) taken around the centre of the
   town scene's height map, scaled by ai_MapCellScale and pushed outwards by up to 51 pixels towards the map edges
   (by its distance over half the map bitmap's width / height), plus the map origin originX / originY and half the
   bitmap, into m->x / m->y */
void ai_MapCellToScreen(LCMAPMARK *m, int originX, int originY)
{
    float half;
    float x;
    float y;
    float edgeX;
    float edgeY;

    half = ((D3SM *)d3_Scenes[0].hmap)->size * 0.5;
    x = (m->cellX - half) * ai_MapCellScale;
    y = (m->cellZ - (double)half) * ai_MapCellScale;    /* @original y alone is taken in double (half popped by fisubr) */
    edgeX = fabs(x) / (ai_MapSize * 0.5f);
    edgeY = fabs(y) / (ai_MapHeight * 0.5f);
    if (x < 0.0f)
        edgeX = x - edgeX * 51.0f;
    else
        edgeX = edgeX * 51.0f + x;
    if (y < 0.0f)
        edgeY = y - edgeY * 51.0f;
    else
        edgeY = edgeY * 51.0f + y;
    m->x = originX + ai_MapSize * 0.5f + edgeX;
    m->y = ai_MapHeight * 0.5f + originY + edgeY;
}

/* 0x5a6c40, the four control pictures of the map window (102 B): on the screen window at (0, 0) the picture
   id, at (0, 0x1a2) id + 1, at (0, 0x78) id + 2 and at (0x243, 0x78) id + 3, each raised (d2_WinOpenPic,
   d2_RaiseObject); its caller ai_OpenTownMap passes the map's child window and the base id (0x727 / 0x72b / 0x723 /
   0x71f, by the record's flags) */

void ai_PlaceMapControls(int screen, int id)
{
    d2_RaiseObject(d2_WinOpenPic(0, 0, id, screen));
    d2_RaiseObject(d2_WinOpenPic(0, 0x1a2, id + 1, screen));
    d2_RaiseObject(d2_WinOpenPic(0, 0x78, id + 2, screen));
    d2_RaiseObject(d2_WinOpenPic(0x243, 0x78, id + 3, screen));
}

char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py); void d2_RaiseObject(); void d2_ScrollWindow();

/* 0x5a6cb0, places the marks of the town map window m: the player's buildings (gm_BuildQuery (1, 6); not the kinds
   'D' / 'E' nor one with flags5a bit 0, a foreign city 'G' only while the player has a town hall) become marks;
   a building with a scene object is mapped from its scene cell (ai_MapCellToScreen), one without (a foreign city)
   is placed at once on the map edge in its side's direction (0.48 / 0.435 of the map width out from the centre)
   with its name and the owner's crest (ref54) when the owner is of kind 5 .. 7. The marks are sorted by their
   scene row and the rest placed (the picture kind + 0x47f, named by the city or player slot); a mark that fails
   the request filter (ai_PassesMapPickFilter) is greyed out; then the crests. With the flag 4 the ambush spots
   (picture 0x550) and with 8 the building placement sites (0x54f) are added; the window scrolls to the player's
   building (else the fallback building, else the camera), clamped to the map.
   @deep Reconstructed against the original (all 901 instructions read): the mark collection (gm_BuildQuery
   1/6, the 'D'/'E'/flags5a/'G'-needs-a-hall filter), the scene-cell marks via
   d3_PointToWorld/d3_GetHeightAt/ai_MapCellToScreen, the foreign city marks on the map edge (dirs table,
   0.48 / 0.435), the row sort, the placement pass with captionX/Y -1 and the request filter, the crests,
   the ambush spots (flag 4, packed ctrls) and the placement sites (flag 8), the clamped scroll. Remaining:
   (1) VC6 factors our foreign-city coordinate (float)dirs[s][k] * size * scale * K + (float)size * scale *
   0.5f into (dir*K + 0.5) * size * scale (fild dir; fmul K; fadd 0.5; fimul size; fmul scale) where the
   original keeps the expanded form (fild size; fild dir; fmul st,st(1); fmul scale; fmul K; fxch; fmul
   scale; fmul 0.5; faddp) - tried reordering the terms, casting either factor, (float)(dir*size), /2, a
   memory constant, volatile scale and double: VC6 factors every spelling; (2) the original keeps m in ecx
   through the first mark loop and lays the player-building scroll path out of line at the end (+0xbcf)
   with one shared epilogue (returns 1 vs ours 2); (3) frame 0x8c vs 0x88 (slot sharing), regs/slots
   elsewhere. (references: M10 at +0x209: our constant 0.48 (__real@3ef5c28f) where the original's field
   refers to 5.33 at 0x62c270 (and 3 more), explained: the M8/M10 ref lines (0.48 / 0.435 / ai_MapCellScale
   against the original's 5.33 / 0.5 fields) are the same four constants in the factored order: every
   constant and object the original multiplies by appears in ours, aligned to other instructions) (size:
   887 vs 901 instructions, explained: ours 887 vs 901: the factored coordinate is 5 instructions where the
   original's expanded form is 9, four times (minus the original's shared y tail); the rest is allocation) */
void ai_BuildTownMap(LCMAPWIN *m)
{
    struct D3SM *hm;
    int x;
    int y;
    int i;
    int originX;
    int originY;
    float py;
    LCMAPMARK tmp;
    D3VECTOR pos;
    D2WIN *win;
    int saved;
    int dirs[8][2] = { { 0, -1 }, { 1, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
    AIBLD *bld;
    AIBLD *hall;
    AICHARREC *owner;
    D3OBJECT *obj;
    char *spot;
    int *ctl;
    char *src;
    char *dst;
    char c;
    int j;
    int n;

    hm = (struct D3SM *)d3_Scenes[0].hmap;
    originX = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townX;
    originY = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townY;
    for (i = 0; i < 0x100; i++) {
        m->marks[i].ctrl = -1;
        m->marks[i].bld = 0;
    }
    for (i = 0; i < 0x80; i++)
        m->siteCtrls[i] = -1;
    for (i = 0; i < 0x300; i++)
        m->controls[i] = -1;
    m->nMarks = 0;

    /* the player's buildings */
    hall = (AIBLD *)gm_BuildQuery(2, 5, 9, 4, ai_PlayerId);
    bld = (AIBLD *)gm_BuildQuery(1, 6);
    n = 0;
    while (bld != 0) {
        if (n >= 0x100)
            break;
        if (bld->type != 'D' && bld->type != 'E' && (bld->flags5a & 1) == 0 && (bld->type != 'G' || hall != 0)) {
            m->marks[n].bld = bld;
            n++;
        }
        bld = (AIBLD *)gm_BuildNext();
        m->nMarks = n;
    }
    d2_LastWindow = m->screen;
    for (i = 0; i < m->nMarks; i++) {
        if (m->marks[i].bld->object != 0) {
            obj = (D3OBJECT *)m->marks[i].bld->object;
            d3_PointToWorld(obj, &obj->pos, &pos);
            d3_GetHeightAt(hm, &pos, &x, &y, &py);
            m->marks[i].cellX = x;
            m->marks[i].cellZ = y;
            ai_MapCellToScreen(&m->marks[i], originX, originY);
        } else {
            d2_LastWindow = m->screen;
            d2_LastWindowPtr = &d2_Wins[d2_LastWindow];
            if (m->marks[i].bld->type == 'G') {
                x = (int)((float)dirs[m->marks[i].bld->side][0] * hm->size * ai_MapCellScale * 0.48f + (float)hm->size * ai_MapCellScale * 0.5f);
                y = (int)((float)dirs[m->marks[i].bld->side][1] * hm->size * ai_MapCellScale * 0.48f + (float)hm->size * ai_MapCellScale * 0.5f);
            } else {
                x = (int)((float)dirs[m->marks[i].bld->side][0] * hm->size * ai_MapCellScale * 0.435f + (float)hm->size * ai_MapCellScale * 0.5f);
                y = (int)((float)dirs[m->marks[i].bld->side][1] * hm->size * ai_MapCellScale * 0.435f + (float)hm->size * ai_MapCellScale * 0.5f);
            }
            m->marks[i].ctrl = d2_WinOpenPic(x, y, m->marks[i].bld->type + 0x47f, d2_LastWindow);
            d2_Objs[m->marks[i].ctrl].interactive = 1;
            d2_Objs[m->marks[i].ctrl].style = 0x43;
            d2_Objs[m->marks[i].ctrl].capfont = 0x56;
            x -= d2_Objs[m->marks[i].ctrl].w / 2;
            y -= d2_Objs[m->marks[i].ctrl].h / 2;
            m->marks[i].cellX = x;
            m->marks[i].cellZ = y;
            d2_MoveObj(d2_LastWindowPtr->x + x, d2_LastWindowPtr->y + y, m->marks[i].ctrl);
            if (m->marks[i].bld->type == 'G') {
                src = ((GMPLAYERSLOT *)ai_PlayerSlots)[m->marks[i].bld->playerSlot].name;
                dst = d2_Objs[m->marks[i].ctrl].caption;
                do {
                    c = *src++;
                    *dst++ = c;
                } while (c != 0);
            } else {
                ai_FormatCityName((int)m->marks[i].bld, d2_Objs[m->marks[i].ctrl].caption);
            }
            if (m->marks[i].bld->owner != 0xffff) {
                owner = &((AICHARREC *)ai_Chars)[m->marks[i].bld->owner];
                if (owner->kind == 5 || owner->kind == 6 || owner->kind == 7)
                    d2_WinOpenPic(x - 8, y - 8, owner->ref54, d2_LastWindow);
            }
        }
    }

    /* sorted by the scene row */
    for (i = 0; i < m->nMarks - 1; i++) {
        for (j = i + 1; j < m->nMarks; j++) {
            if (m->marks[j].cellZ < m->marks[i].cellZ) {
                tmp = m->marks[i];
                m->marks[i] = m->marks[j];
                m->marks[j] = tmp;
            }
        }
    }

    d2_LastWindow = m->screen;
    win = d2_LastWindowPtr = &d2_Wins[d2_LastWindow];
    for (i = 0; i < m->nMarks; i++) {
        x = (int)m->marks[i].x;
        y = (int)m->marks[i].y;
        if (m->marks[i].ctrl == -1) {
            m->marks[i].ctrl = d2_WinOpenPic(x, y, m->marks[i].bld->type + 0x47f, d2_LastWindow);
            d2_Objs[m->marks[i].ctrl].interactive = 1;
            d2_Objs[m->marks[i].ctrl].captionX = -1;
            d2_Objs[m->marks[i].ctrl].captionY = -1;
            d2_Objs[m->marks[i].ctrl].style = 0x43;
            d2_Objs[m->marks[i].ctrl].capfont = 0x56;
            if (m->marks[i].bld->type == 'G') {
                src = ((GMPLAYERSLOT *)ai_PlayerSlots)[m->marks[i].bld->playerSlot].name;
                dst = d2_Objs[m->marks[i].ctrl].caption;
                do {
                    c = *src++;
                    *dst++ = c;
                } while (c != 0);
            } else {
                ai_FormatCityName((int)m->marks[i].bld, d2_Objs[m->marks[i].ctrl].caption);
            }
            x -= d2_Objs[m->marks[i].ctrl].w / 2;
            y -= d2_Objs[m->marks[i].ctrl].h / 2;
            m->marks[i].cellX = x;
            m->marks[i].cellZ = y;
            d2_MoveObj(d2_LastWindowPtr->x + x, d2_LastWindowPtr->y + y, m->marks[i].ctrl);
            if (ai_PassesMapPickFilter((AICHARREC *)ai_Chars[ai_PlayerId], m->req, m->marks[i].bld) == 0) {
                d2_Objs[m->marks[i].ctrl].interactive = 0;
                d2_Objs[m->marks[i].ctrl].style = 0;
            }
        }
    }

    /* the owners' crests */
    for (i = 0; i < m->nMarks; i++) {
        if (m->marks[i].ctrl != -1 && m->marks[i].bld->owner != 0xffff) {
            owner = &((AICHARREC *)ai_Chars)[m->marks[i].bld->owner];
            if (owner->kind == 5 || owner->kind == 6 || owner->kind == 7)
                d2_WinOpenPic(m->marks[i].cellX - 8, m->marks[i].cellZ - 8, owner->ref54, d2_LastWindow);
        }
    }

    /* the ambush spots of the town scene */
    if (m->flags & 4) {
        saved = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        d2_LastWindow = m->screen;
        d2_LastWindowPtr = &d2_Wins[d2_LastWindow];
        for (i = 0; i < 0x10; i++)
            m->spotCtrls[i] = -1;
        n = 0;
        for (spot = ai_AmbushSpots[0]; (int)spot < (int)ai_AmbushSpots[8]; spot += sizeof(ai_AmbushSpots[0])) {  /* a signed compare (jl) */
            obj = d3_FindObject(0, 0x100, 0, spot);
            if (obj != 0) {
                d3_PointToWorld(obj, &obj->pos, &pos);
                d3_GetHeightAt(hm, &pos, &x, &y, &py);
                tmp.cellX = x;
                tmp.cellZ = y;
                ai_MapCellToScreen(&tmp, originX, originY);
                x = (int)tmp.x;
                y = (int)tmp.y;
                i = d2_WinOpenPic(x, y, 0x550, d2_LastWindow);
                d2_Objs[i].interactive = 1;
                d2_Objs[i].style = 0x43;
                d2_Objs[i].capfont = 0x56;
                d2_Objs[i].user = obj;
                m->spotCtrls[n++] = i;
            }
        }
        d3if_SelectScene(saved, 1);
    }

    /* the sites of the running building placement */
    if (m->flags & 8) {
        d2_LastWindow = m->screen;
        ctl = m->siteCtrls;
        for (i = 0; i < 0x100; i++, ctl++) {
            obj = (D3OBJECT *)ai_PlacementSites[i];
            if (obj != 0) {
                d3_PointToWorld(obj, &obj->pos, &pos);
                d3_GetHeightAt(hm, &pos, &x, &y, &py);
                tmp.cellX = x;
                tmp.cellZ = y;
                ai_MapCellToScreen(&tmp, originX, originY);
                x = (int)tmp.x;
                y = (int)tmp.y;
                *ctl = d2_WinOpenPic(x, y, 0x54f, d2_LastWindow);
                d2_Objs[*ctl].interactive = 1;
                d2_Objs[*ctl].style = 0x43;
            }
        }
    }

    /* scrolled to the player's building */
    if ((bld = (AIBLD *)ai_PlayerBuilding) != 0 || (bld = (AIBLD *)ai_FallbackBuilding) != 0) {
        obj = (D3OBJECT *)bld->object;
        if (obj == 0)
            obj = d3_Camera;
    } else {
        obj = d3_Camera;
        if (obj == 0)
            return;
    }
    d3_PointToWorld(obj, &obj->pos, &pos);
    d3_GetHeightAt(hm, &pos, &x, &y, &py);
    tmp.cellX = x;
    tmp.cellZ = y;
    ai_MapCellToScreen(&tmp, originX, originY);
    d2_LastWindow = m->screen;
    x = (int)tmp.x - d2_Wins[d2_LastWindow].w / 2;
    y = (int)tmp.y - d2_Wins[d2_LastWindow].h / 2;
    x = x < 0 ? 0 : x;
    if (ai_MapSize - 0x200 < x)
        x = ai_MapSize - 0x200;
    y = y < 0 ? 0 : y;
    if (ai_MapHeight - 0x168 < y)
        y = ai_MapHeight - 0x168;
    win->scrollX = x;
    win->scrollY = y;
    d2_ScrollWindow(win->id);
}


int d2_OpenForm(); void d2_RaiseWindow(); int io_PrintText(char *fmt, ...);
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.h: int view): @original ai_SwitchRoomI pushes the flags too (0x5a78cf) */
int d2_SetForm(int view, int n); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_CloseView(int view); void d2_RemoveObj(); void d2_ScrollWindowBy();
int d2_CreatePicObj();

/* 0x5a78a0, opens the town map window "Misc\Landkarte2" for the record m: the town map bitmap becomes the background
   tile (the old one is kept in m->tile), the map child window (0x4a, 0x4c, 0x206 x 0x164) gets the tiled background,
   the control bar ai_PlaceMapControls the request's own base id or by the flags 0x727 (4), 0x72b (2), 0x723 (1 or 8
   with a title) or 0x71f; form 1 shows the title, flag 0x10 form 2. Returns the view */
int ai_OpenTownMap(LCMAPWIN *m)
{
    m->tile = d2_BackgroundTile;
    d2_BackgroundTile = rt_MapSurface;
    m->view = d2_OpenForm(0, 0, "Misc\\Landkarte2");
    d2_PlaceMainWindowF(m->view, 3);
    d2_SetForm(m->view, 0);
    m->screen = d2_CreateChildWindow(0x4a, 0x4c, 0x206, 0x164, 0, d2_LastWindow);
    d2_LastWindow = m->screen;
    d2_LastWindowPtr->tileBackground = 1;
    d2_SetForm(m->view, 0);
    if (m->req[0x13] == 0) {
        if (m->flags & 4)
            ai_PlaceMapControls(d2_LastWindow, 0x727);
        else if (m->flags & 2)
            ai_PlaceMapControls(d2_LastWindow, 0x72b);
        else if ((m->flags & 1) && m->name != 0)
            ai_PlaceMapControls(d2_LastWindow, 0x723);
        else if ((m->flags & 8) && m->name != 0)
            ai_PlaceMapControls(d2_LastWindow, 0x723);
        else
            ai_PlaceMapControls(d2_LastWindow, 0x71f);
    } else {
        ai_PlaceMapControls(d2_LastWindow, m->req[0x13]);
    }
    if (m->name != 0) {
        d2_SetForm(m->view, 1);
        d2_RaiseWindow(d2_LastWindow);
        io_PrintText(m->name);
    }
    if (m->flags & 0x10) {
        d2_SetForm(m->view, 2);
        d2_RaiseWindow(d2_LastWindow);
    }
    return m->view;
}

/* 0x5a79e0, drags the town map window win with the right mouse button: when the drag begins the mouse bounds
   (di_MinX .. di_MaxY) and the window's scroll are kept and the bounds narrowed to the map (ai_MapSize x
   ai_MapHeight less the 0x200 x 0x168 window) around the start position; while it is held the scroll follows the
   relative mouse position dm_P; on release the bounds come back and the pointer is put back at d2_MouseX /
   d2_MouseY. Returns 1 when the scroll changed */
int ai_DragTownMap(D2WIN *win)
{
    float dx;
    int changed;
    int x;
    int y;

    changed = 0;
    if (dm_L.rhold != 0) {
        if (ai_Dragging == 0) {
            di_Absolute = 0;
            ai_Dragging = 1;
            ai_DragStartX = dm_L.x;
            ai_DragStartY = dm_L.y;
            ai_DragScrollX = win->scrollX;
            ai_DragScrollY = win->scrollY;
            ai_DragSavedMaxX = di_MaxX;
            ai_DragSavedMaxY = di_MaxY;
            ai_DragSavedMinX = di_MinX;
            ai_DragSavedMinY = di_MinY;
            di_MinX = ai_DragStartX - ai_DragScrollX;
            di_MaxX = ai_MapSize - ai_DragScrollX + ai_DragStartX - 0x200;
            di_MinY = ai_DragStartY - ai_DragScrollY;
            di_MaxY = ai_MapHeight - ai_DragScrollY + ai_DragStartY - 0x168;
        }
        dx = (float)(dm_P.x - ai_DragStartX);
        x = (int)dx;
        dx = (float)(dm_P.y - ai_DragStartY);
        y = (int)dx;
        if (win->scrollX != x + ai_DragScrollX || win->scrollY != ai_DragScrollY + y)
            changed = 1;
        win->scrollX = x + ai_DragScrollX;
        win->scrollY = ai_DragScrollY + y;
    } else if (ai_Dragging != 0) {
        di_MaxY = ai_DragSavedMaxY;
        di_MinX = ai_DragSavedMinX;
        di_MaxX = ai_DragSavedMaxX;
        ai_DragStartX = 0;
        ai_DragStartY = 0;
        di_MinY = ai_DragSavedMinY;
        di_Absolute = 1;
        di_SetMousePos(d2_MouseX, d2_MouseY);
        ai_Dragging = 0;
        return 0;
    }
    return changed;
}

/* 0x5a7bc0, scrolls the town map window win by 10 pixels when the pointer (dm_L) reaches the mouse bounds (di_MinX ..
   di_MaxY) or by the arrow keys (up, down, left, right: DIK 0xc8, 0xd0, 0xcb, 0xcd), clamped to the map (ai_MapSize x
   ai_MapHeight less the 0x200 x 0x168 window); the new scroll is kept in ai_MapScrollX / Y. Returns 1 when it
   changed */
int ai_ScrollTownMap(D2WIN *win)
{
    int y;
    int changed;

    changed = 0;
    ai_MapScrollX = win->scrollX;
    y = win->scrollY;
    if (dm_L.x <= di_MinX)
        ai_MapScrollX -= 10;
    else if (dm_L.x >= di_MaxX)
        ai_MapScrollX += 10;
    if (dm_L.y <= di_MinY)
        y -= 10;
    else if (dm_L.y >= di_MaxY)
        y += 10;
    if (kb_Down[0xc8])
        y -= 10;
    else if (kb_Down[0xd0])
        y += 10;
    else if (kb_Down[0xcb])
        ai_MapScrollX -= 10;
    else if (kb_Down[0xcd])
        ai_MapScrollX += 10;
    ai_MapScrollX = ai_MapScrollX < 0 ? 0 : ai_MapScrollX;
    if (ai_MapSize - 0x200 < ai_MapScrollX)
        ai_MapScrollX = ai_MapSize - 0x200;
    ai_MapScrollY = y < 0 ? 0 : y;
    if (ai_MapHeight - 0x168 < ai_MapScrollY)
        ai_MapScrollY = ai_MapHeight - 0x168;
    if (ai_MapScrollX != win->scrollX || ai_MapScrollY != win->scrollY) {
        win->scrollX = ai_MapScrollX;
        win->scrollY = ai_MapScrollY;
        changed = 1;
    }
    return changed;
}

/* 0x5a7cd0, the town map window's per-frame update (1556 B): while the map has the input (ai_MapScrollEnabled) its drag
   and key scroll (ai_DragTownMap / ai_ScrollTownMap) set m->scrolled and stamp m->scrollTime; 75 ms after the last
   scroll, with the right button held, the mark under the pointer opens the info window of its building's owner
   (ai_CharInfoWindow, kinds below 10). Then every character of scene 0 (ch_List) is placed on the map as
   ai_MapCellToScreen places a mark: a body standing for a character in a building owned by a player (kind 5 .. 7),
   or a cart (transport) whose owner (gm_ObjectBuildingOwner) is one, gets that player's crest (ref54 - 0x53) as a
   picture control clipped to the window; a placed control follows the window's scroll; the control of a character
   that left the scene, or whose body is of type 1 without a transport, is removed. The arrow keys scroll the
   window by 4 pixels */

void ai_UpdateTownMap(LCMAPWIN *m)
{
    struct D3SM *hm;
    D2WIN *win;
    CHARACTER *ch;
    AIBLD *bld;
    AICHARREC *owner;
    AICHARREC *rec;
    D3VECTOR4 pos;
    float half;
    float x;
    float y;
    float edgeX;
    float edgeY;
    int originX;
    int originY;
    int px;
    int py;
    int pic;
    int i;
    unsigned char kind;

    hm = (struct D3SM *)d3_Scenes[0].hmap;
    originX = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townX;
    originY = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townY;
    d2_LastWindow = m->screen;
    d2_LastWindowPtr = &d2_Wins[m->screen];
    win = d2_LastWindowPtr;
    if (ai_MapScrollEnabled != 0) {
        m->scrolled = 0;
        m->scrolled = ai_DragTownMap(&d2_Wins[m->screen]);
        m->scrolled |= ai_ScrollTownMap(&d2_Wins[m->screen]);
        if (m->scrolled != 0)
            m->scrollTime = d3_Time;
    }
    if (d2_RightHeldObj != -1 && d3_Time > m->scrollTime + 0x4b && dm_L.rhold != 0 && ai_DragWindow == -1) {
        for (i = 0; i < m->nMarks; i++) {
            if (d2_PointerObj != -1 && d2_PointerObj == m->marks[i].ctrl && m->marks[i].bld->owner != 0xffff &&
                ((AICHARREC *)ai_Chars)[m->marks[i].bld->owner].kind < 10)
                ai_CharInfoWindow(&((AICHARREC *)ai_Chars)[m->marks[i].bld->owner]);
        }
    }
    d2_LastWindow = m->screen;
    for (i = 0; i < 0x300; i++) {
        ch = ch_List[i];
        if (ch == 0)
            continue;
        if (ch->scene == &d3_Scenes[0]) {
            d3_PointToWorld(ch->obj, &ch->obj->pos, (D3VECTOR *)&pos);
            half = ((struct D3SM *)d3_Scenes[0].hmap)->size * 0.5;
            x = ((pos.x - hm->origin.x) / hm->cell.x - half) * ai_MapCellScale;
            y = ((pos.z - hm->origin.z) / hm->cell.z - (double)half) * ai_MapCellScale;   /* @original y alone in double (half popped by fsubrp) */
            edgeX = fabs(x) / (ai_MapSize * 0.5f);
            edgeY = fabs(y) / (ai_MapHeight * 0.5f);
            if (x < 0.0f)
                edgeX = x - edgeX * 51.0f;
            else
                edgeX = edgeX * 51.0f + x;
            if (y < 0.0f)
                edgeY = y - edgeY * 51.0f;
            else
                edgeY = edgeY * 51.0f + y;
            px = (int)((float)(ai_MapSize / 2) + originX + edgeX);
            py = (int)((float)(ai_MapHeight / 2) + originY + edgeY);
            if (m->controls[i] == -1) {
                if (ch->obj->type != 1 || ch->transport != 0) {
                    if ((owner = (AICHARREC *)ch->obj->owner) != 0 && ch->transport == 0) {
                        bld = owner->bld;
                        if (bld == 0 || bld->owner == 0xffff)
                            goto removed;
                        kind = ((AICHARREC *)ai_Chars)[bld->owner].kind;
                        if (kind != 6 && kind != 7 && kind != 5)
                            goto removed;
                        pic = ((AICHARREC *)ai_Chars)[bld->owner].ref54 - 0x53;
                    } else if (ch->obj->owner != 0 && ch->transport != 0 &&
                               (rec = (AICHARREC *)gm_ObjectBuildingOwner((unsigned short *)ch->obj->owner)) != 0 &&
                               (rec->kind == 6 || rec->kind == 7 || rec->kind == 5))
                        pic = rec->ref54 - 0x53;
                    else
                        goto removed;
                    m->controls[i] = d2_CreatePicObjS(d2_LastWindowPtr->x + px, d2_LastWindowPtr->y + py, pic);
                    if (m->controls[i] == -1)
                        continue;
                    d2_Objs[m->controls[i]].clipx0 = d2_LastWindowPtr->x;
                    d2_Objs[m->controls[i]].clipx1 = d2_LastWindowPtr->w + d2_LastWindowPtr->x;
                    d2_Objs[m->controls[i]].clipy0 = d2_LastWindowPtr->y;
                    d2_Objs[m->controls[i]].clipy1 = d2_LastWindowPtr->h + d2_LastWindowPtr->y;
                    d2_Objs[m->controls[i]].buf = (D2CTRL *)&d2_Views[m->view];
                }
            } else if (ch->obj->type != 1 || ch->transport != 0) {
                d2_MoveObj(d2_LastWindowPtr->x - d2_LastWindowPtr->scrollx + px,
                           d2_LastWindowPtr->y - win->scrolly + py, m->controls[i]);
                d2_Objs[m->controls[i]].clipx0 = d2_LastWindowPtr->x;
                d2_Objs[m->controls[i]].clipx1 = d2_LastWindowPtr->w + d2_LastWindowPtr->x;
                d2_Objs[m->controls[i]].clipy0 = d2_LastWindowPtr->y;
                d2_Objs[m->controls[i]].clipy1 = d2_LastWindowPtr->h + d2_LastWindowPtr->y;
            }
        }
    removed:
        if (m->controls[i] != -1 && (ch->obj->type == 1 || ch->scene != &d3_Scenes[0]) && ch->transport == 0) {
            d2_RemoveObj(m->controls[i]);
            m->controls[i] = -1;
        }
    }
    if (kb_Down[0xc8] != 0) {               /* DIK_UP */
        d2_ScrollWindowBy(0, -4, d2_LastWindow);
        return;
    }
    if (kb_Down[0xd0] != 0) {               /* DIK_DOWN */
        d2_ScrollWindowBy(0, 4, d2_LastWindow);
        return;
    }
    if (kb_Down[0xcb] != 0) {               /* DIK_LEFT */
        d2_ScrollWindowBy(-4, 0, d2_LastWindow);
        return;
    }
    if (kb_Down[0xcd] != 0)                 /* DIK_RIGHT */
        d2_ScrollWindowBy(4, 0, d2_LastWindow);
}

/* 0x5a82f0, the town map window's close (78 B): the background tile saved at +0x27a8 goes back to
   d2_BackgroundTile, d2_CloseView closes the window, the 0x300 control objects of +0x18dc are removed
   (d2_RemoveObj) and set to -1, and the window is set to -1 */

void ai_CloseMapWindow(int *m_)
{
    LCMAPWIN *m = (LCMAPWIN *)m_;
    int i;

    d2_BackgroundTile = m->tile;
    d2_CloseView(m->view);
    for (i = 0; i < 0x300; i++) {
        if (m->controls[i] != -1) {
            d2_RemoveObj(m->controls[i]);
            m->controls[i] = -1;
        }
    }
    m->view = -1;
}
#pragma pack(push, 1)
typedef struct G8BLD {                  /* a local view of gv.h's ORDBLD (gv.h cannot be included here yet): the
                                           fields the production settings read */
    unsigned char type;                 /* +0x00, the building kind */
    int id;                             /* +0x01, the building's id (the production settings chain's key) */
} G8BLD;
#pragma pack(pop)

/* 0x5a8340, the record's random in [0, 1) (68 B): the LCG at +4 (0x41c64e6d, 0x3039) advanced, then its high
   word modulo 0x7fff scaled by 1 / 0x7fff (0x5eb8f8) */

float ai_SettingRandom(G8SETTING *rec)
{
    rec->seed = rec->seed * 0x41c64e6d + 0x3039;
    return (float)((unsigned int)(unsigned short)(rec->seed >> 16) % 0x7fff) * 0.00003051851f;
}
/* 0x5a8390, the recursive free of a production setting chain (32 B): the next setting at +0x48 is freed first,
   then the record (mem_Free) */

void ai_FreeSettingChain(G8SETTING *rec)
{
    if (rec->next != 0)
        ai_FreeSettingChain(rec->next);
    mem_Free(rec);
}
/* 0x5a83b0, the goods weights of the production setting rec of the building bld (256 B): each of the ten goods of
   the building's item group (ai_FindItemGroup) that no slot holds yet takes the first free slot with the weight
   14; then the weights of the used slots are stepped by one, slot after slot round the seven, until they sum to
   100 (a weight stays within 0 .. 100) */

void ai_FillSettingWeights(G8BLD *bld, G8SETTING *rec)
{
    G7GRP *grp;
    unsigned int sum;
    int count;
    int left;
    int step;
    int found;
    int i;
    int j;

    grp = (G7GRP *)ai_FindItemGroup((char *)bld);
    if (grp == 0)
        return;
    for (i = 0; i < 10; i++) {
        if (grp->items[i] != 0) {
            found = 0;
            for (j = 0; j < 7; j++)
                if (grp->items[i] == rec->slots[j].good)
                    found = 1;
            if (!found) {
                for (j = 0; j < 7; j++) {
                    if (rec->slots[j].good == 0) {
                        rec->slots[j].good = grp->items[i];
                        rec->slots[j].weight = 14;
                        break;
                    }
                }
            }
        }
    }
    sum = 0;
    count = 0;
    for (j = 0; j < 7; j++) {
        if (rec->slots[j].good != 0) {
            count++;
            sum += rec->slots[j].weight;
        }
    }
    if (sum == 100)
        return;
    if (sum > 100) {
        left = sum - 100;
        step = -1;
    } else {
        left = 100 - sum;
        step = 1;
    }
    j = 0;
    while (left != 0 && count != 0) {
        if (rec->slots[j].good != 0 && rec->slots[j].weight + step > -1 && rec->slots[j].weight + step < 101) {
            rec->slots[j].weight += step;
            left--;
        }
        j = (j + 1) % 7;
    }
}
/* 0x5a84b0, the production setting of an id (27 B): the chain ai_Settings (0x6f26c0) walked by the next at
   +0x48, the record whose id at +0 is the argument, else 0 */

int *ai_FindSetting(int id)
{
    G8SETTING *s;

    s = ai_Settings;
    while (s != 0) {
        if (s->bldId == id)
            return (int *)s;
        s = s->next;
    }
    return 0;
}
/* 0x5a84d0, the item group of a good (102 B): the good's first byte is looked up as the key of a group (a
   G7GRP: the key byte and ten good ids) in the three special groups gm_SpecialItemGroups (the kinds ' ', 0x1e
   and 0x1f) first, then in the 37 groups of gm_BuildingItemGroups; the group or 0 */

unsigned char *ai_FindItemGroup(char *good)
{
    char c;
    unsigned char *q;
    int i;

    c = *good;
    if (c == ' ' || c == 0x1e || c == 0x1f) {
        i = 0;
        for (q = (unsigned char *)(unsigned)&gm_SpecialItemGroups; (int)q < (int)((unsigned)&gm_SpecialItemGroups + 0x3f); q += 0x15, i++)
            if ((char)*q == c)
                return (unsigned char *)(unsigned)&gm_SpecialItemGroups + i * 0x15;
    }
    i = 0;
    for (q = (unsigned char *)(unsigned)&gm_BuildingItemGroups; (int)q < (int)((unsigned)&gm_BuildingItemGroups + 0x309); q += 0x15, i++)
        if ((char)*q == c)
            return (unsigned char *)(unsigned)&gm_BuildingItemGroups + i * 0x15;
    return 0;
}

int ls_Read(void *buf, int size, int count, int f); void * mem_Alloc(int size, char *tag);
unsigned vfs_fwrite();

/* The production settings (the "hp_" module with the map window: the literals "hp_AddProdSetting" 0x62c284 and
   "hp_LoadProdSetting" 0x62c298 climb right after the map's "Misc\Landkarte2" 0x62c274): the linked list
   0x6f26c0 of 0x4c-byte records {the building id +0, the seed +4, the rotation index byte +8, seven {good, weight, lock}
   slots at +0xc (stride 8), the factor +0x44 (1.0f), next +0x48}; 0x5a83b0 / 0x5a84d0 fill the weights. */

/* 0x5a8540, the slot table of a building's setting (31 B): the chain ai_Settings (0x6f26c0) walked by the next at
   +0x48, the record whose building id at +0 is the argument: its seven slots at +0xc; else 0 */

G8SLOT *ai_FindSettingSlots(int bldId)
{
    G8SETTING *s;

    s = ai_Settings;
    while (s != 0) {
        if (s->bldId == bldId)
            return s->slots;
        s = s->next;
    }
    return 0;
}
/* 0x5a8560, the production setting of the building bld (a G8BLD, gv.h's ORDBLD), created on demand (338 B): its
   slots when the chain ai_Settings has one for the building's id (ai_FindSettingSlots); else, for a building with an
   item group (ai_FindItemGroup), a zeroed record ("hp_AddProdSetting") keyed by the id with a random seed, the
   factor 1.0 and the weights of its goods (ai_FillSettingWeights) is put at the head of the chain and its slots
   returned; 0 without a group or memory */

void *ai_ProdSetting(int bld)
{
    G8SETTING *rec;
    G8SLOT *slots;

    if (ai_Settings == 0) {
        if (ai_FindItemGroup((char *)bld) == 0)
            return 0;
        rec = (G8SETTING *)mem_Alloc(0x4c, "hp_AddProdSetting");
        if (rec != 0) {
            D3_ZERO_AT(rec, sizeof(G8SETTING));
            rec->bldId = ((G8BLD *)bld)->id;
            rec->seed = rand();
            rec->factor = 1.0f;
            ai_FillSettingWeights((G8BLD *)bld, rec);
            ai_Settings = rec;
            return rec->slots;
        }
        return 0;
    }
    slots = ai_FindSettingSlots(((G8BLD *)bld)->id);
    if (slots != 0)
        return slots;
    if (ai_FindItemGroup((char *)bld) == 0)
        return 0;
    rec = (G8SETTING *)mem_Alloc(0x4c, "hp_AddProdSetting");
    if (rec == 0)
        return 0;
    D3_ZERO_AT(rec, sizeof(G8SETTING));
    rec->bldId = ((G8BLD *)bld)->id;
    rec->seed = rand();
    rec->factor = 1.0f;
    ai_FillSettingWeights((G8BLD *)bld, rec);
    rec->next = ai_Settings;
    ai_Settings = rec;
    return rec->slots;
}
/* 0x5a86c0, the setting of the building id bldId removed (75 B): the list 0x6f26c0 is relinked and the record
   freed (mem_Free); -1 when the building has none */

int ai_FreeProdSetting(int bldId)
{
    G8SETTING *prev;
    G8SETTING *s;

    prev = 0;
    s = ai_Settings;
    while (s != 0) {
        if (s->bldId == bldId)
            break;
        prev = s;
        s = s->next;
    }
    if (s == 0)
        return -1;
    if (prev != 0)
        prev->next = s->next;
    else
        ai_Settings = s->next;
    mem_Free(s);
    return 0;
}
/* 0x5a8710, sets the weight of the good good in the production setting of the building bld (306 B): the free total
   is 100 less the weights of the locked slots; when the new weight fits it and more than one slot is unlocked,
   the slot of the good (with another weight than the new one) takes it, and the difference is spread one point at
   a time over the other used unlocked slots, round the seven from the setting's rotation index (each weight kept
   within 0 .. the free total), where the rotation then stays */

void ai_SetSettingWeight(int bld, int good, unsigned char weight)
{
    G8SETTING *rec;
    int total;
    int count;
    int slot;
    int step;
    int rest;
    int i;

    rec = (G8SETTING *)ai_FindSetting(((G8BLD *)bld)->id);
    total = 100;
    if (good == 0)
        return;
    count = 0;
    for (i = 0; i < 7; i++)
        if (rec->slots[i].good != 0)
            count++;
    for (i = 0; i < 7; i++) {
        if (rec->slots[i].good != 0 && rec->slots[i].lock != 0) {
            total -= rec->slots[i].weight;
            count--;
        }
    }
    if (weight > total || count < 2)
        return;
    for (slot = 0; slot < 7; slot++) {
        if (rec->slots[slot].good == good && weight != rec->slots[slot].weight) {
            step = rec->slots[slot].weight < weight ? -1 : 1;
            rest = abs(rec->slots[slot].weight - weight);
            i = rec->rotate;
            while (rest != 0) {
                if (slot != i && rec->slots[i].good != 0 && rec->slots[i].lock == 0 &&
                    rec->slots[i].weight + step > -1 && rec->slots[i].weight + step < total + 1) {
                    rec->slots[i].weight += step;
                    rest--;
                }
                i = (i + 1) % 7;
            }
            rec->slots[slot].weight = weight;
            rec->rotate = i;
            return;
        }
    }
}
/* 0x5a8850, the weights of a building's production setting refreshed (31 B): the setting of the building's id
   (ai_FindSetting, 0x5a84b0) has its slots refilled from the building's goods (ai_FillSettingWeights), if it exists */

void ai_RefreshProdSetting(G8BLD *b)
{
    G8SETTING *rec;

    rec = (G8SETTING *)ai_FindSetting(b->id);
    if (rec != 0)
        ai_FillSettingWeights((int)b, (char *)rec);
}
/* 0x5a8870, a good drawn by the weights of a building's setting (111 B): the setting of the building's id
   (ai_FindSetting), the random of 0x5a8340 * 100.0f (0x5ebb40) against the running sum of the slot weights, the
   good of the first slot over it; 0 when the building has no setting */

int ai_DrawSettingGood(int bld)
{
    G8SETTING *rec;
    G8SLOT *p;
    G8BLD *b;
    int i;
    int draw;
    int sum;
    int pick;

    b = (G8BLD *)bld;
    rec = (G8SETTING *)ai_FindSetting(b->id);
    if (rec == 0)
        return 0;
    draw = (int)((float)ai_SettingRandom(rec) * 100.0f);
    i = 0;
    pick = 0;
    sum = 0;
    p = rec->slots;
    while (i < 7 && pick == 0) {
        if (p->good != 0) {
            sum += p->weight;
            if (p->weight != 0 && draw < sum)
                pick = p->good;
        }
        i++;
        p++;
    }
    return pick;
}

/* 0x5a88e0, the factor of a building's production setting (31 B): the +0x38 of the slot table ai_FindSettingSlots
   returns (= the record's +0x44), else the shared 1.0f (ai_FloatOne, 0x5eb780) */

float ai_SettingFactor(int bld)
{
    G8SETTAIL *t;
    G8BLD *b;

    b = (G8BLD *)bld;
    t = (G8SETTAIL *)ai_FindSettingSlots(b->id);
    if (t != 0)
        return t->factor;
    return 1.0f;
}
/* 0x5a8900, the factor of a building's production setting set (28 B): the +0x44 of the record ai_FindSetting
   returns, when the building has a setting */

void ai_SetSettingFactor(int bld, float factor)
{
    G8SETTING *rec;
    G8BLD *b;

    b = (G8BLD *)bld;
    rec = (G8SETTING *)ai_FindSetting(b->id);
    if (rec != 0)
        rec->factor = factor;
}
/* 0x5a8920, the production settings freed (29 B): ai_FreeSettingChain over the chain ai_Settings (0x6f26c0),
   then the head cleared */

void ai_FreeSettings(void)
{
    if (ai_Settings != 0)
        ai_FreeSettingChain(ai_Settings);
    ai_Settings = 0;
}
/* 0x5a8940, the production settings written to the save f (213 B): the chain ai_Settings (0x6f26c0) counted,
   then each record's building id, seed and factor and its seven slots' good and weight written in chain order;
   1 done, 0 on a write error */

int ai_SaveSettings(void *f)
{
    G8SETTING *s;
    int n;
    int i;

    n = 0;
    s = ai_Settings;
    while (s != 0) {
        s = s->next;
        n++;
    }
    if (vfs_fwrite(&n, 4, 1, f) == 0)
        return 0;
    for (s = ai_Settings; s != 0; s = s->next) {
        if (vfs_fwrite(&s->bldId, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&s->seed, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&s->factor, 4, 1, f) == 0)
            return 0;
        for (i = 0; i < 7; i++) {
            if (vfs_fwrite(&s->slots[i].good, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&s->slots[i].weight, 1, 1, f) == 0)
                return 0;
        }
    }
    return 1;
}
/* 0x5a8a20, the production settings loaded from the save f (347 B, the reader of ls_LoadGame from 0x10047): the
   chain is freed (ai_FreeSettings), the count read, that many zeroed records ("hp_LoadProdSetting", the building
   id -1) put at the head of ai_Settings, then each record's building id, seed and factor and its seven slots'
   good and weight read back in chain order; 1 done, 0 on a read or memory error (the chain freed again) */

int ai_LoadSettings(int f)
{
    G8SETTING *rec;
    G8SETTING *s;
    int n;
    int i;

    ai_FreeSettings();
    if (ls_Read(&n, 4, 1, f) == 0)
        return 0;
    for (i = 0; i < n; i++) {
        rec = (G8SETTING *)mem_Alloc(0x4c, "hp_LoadProdSetting");
        if (rec == 0) {
            ai_FreeSettings();
            return 0;
        }
        D3_ZERO_AT(rec, sizeof(G8SETTING));
        rec->bldId = -1;
        if (ai_Settings != 0)
            rec->next = ai_Settings;
        ai_Settings = rec;
    }
    for (s = ai_Settings; s != 0; s = s->next) {
        if (ls_Read(&s->bldId, 4, 1, f) == 0 || ls_Read(&s->seed, 4, 1, f) == 0 ||
            ls_Read(&s->factor, 4, 1, f) == 0) {
            ai_FreeSettings();
            return 0;
        }
        for (i = 0; i < 7; i++) {
            if (ls_Read(&s->slots[i].good, 4, 1, f) == 0 || ls_Read(&s->slots[i].weight, 1, 1, f) == 0) {
                ai_FreeSettings();
                return 0;
            }
        }
    }
    return 1;
}
/* 0x5a8b80, the news of an indulgence purchase with two values (123 B): for the owner character of the kinds
   6 / 7 the text 0x20b6 (0x20b7 when the owner's kind byte +0xc is 0, else 0x20b8 / 0x20b9 by the clock
   ai_Time.year > 0x74) with the buyer's id, the building, the amount and the owner's share into ai_NewsText, shown
   to the owner's netId (he_ShowNews, kind 0x5ff); nothing for another owner kind */

void ai_ShowIndulgenceNews(unsigned short *buyer, char *ownerRec, int bld, int share, int amount)
{
    G8CHARREC *owner;
    int text;

    owner = (G8CHARREC *)ownerRec;
    if (owner->kind == 6 || owner->kind == 7) {
        if (owner->b00c == 0)
            text = 0x20b7;
        else
            text = (0x75 <= ai_Time.year) + 0x20b8;
        txt_Format(ai_NewsText, 0x20b6, *buyer, bld, amount, share, text);
        he_ShowNews(owner->netId, -1, ai_NewsText, 0, 0x5ff, ch_EmptyName);
    }
}
/* 0x5a8c00, the news of a payment to a building's owner (79 B): for an owner character of the kinds 6 / 7 the
   text 0x20ba with the payer's id and the building into ai_NewsText, shown to the owner's netId (he_ShowNews,
   kind 0x5ec); nothing for another owner kind. Both callers pay money to the building's owner
   (ai_ExecuteBuyIndulgence 0x49cb60 on taking over the rooms, ai_ConfessionWindow 0x5a2b60 at the church) */

void ai_ShowPaymentNews(unsigned short *buyer, char *ownerRec, int bld)
{
    G8CHARREC *owner;

    owner = (G8CHARREC *)ownerRec;
    if (owner->kind == 6 || owner->kind == 7) {
        txt_Format(ai_NewsText, 0x20ba, *buyer, bld);
        he_ShowNews(owner->netId, -1, ai_NewsText, 0, 0x5ec, ch_EmptyName);
    }
}
/* 0x5a8c50, the news of a purchase in a building (84 B): for the building owner of the kinds 6 / 7 the text
   0x20bc with the buyer's id, the building and the paid value into ai_NewsText, shown to the owner's netId
   (he_ShowNews, kind 0x5ff); nothing for another owner kind */

void ai_ShowPurchaseNews(unsigned short *buyer, char *ownerRec, int bld, int value)
{
    G8CHARREC *owner;

    owner = (G8CHARREC *)ownerRec;
    if (owner->kind == 6 || owner->kind == 7) {
        txt_Format(ai_NewsText, 0x20bc, *buyer, bld, value);
        he_ShowNews(owner->netId, -1, ai_NewsText, 0, 0x5ff, ch_EmptyName);
    }
}
/* 0x5a8cb0, the news an office's new owner gets about its holder (74 B, a reasoned guess at the text's meaning):
   for a character of the kinds 6 / 7 the text 0x20bb with the holder's id into ai_NewsText, shown to its netId
   (he_ShowNews, kind 0x5fd); nothing for another kind. The only caller is the office message
   amt_ExecuteOfficeMessage (0x4e67a0), which announces its x14 character to the new office owner */

void ai_ShowOfficeNews(unsigned short *holder, char *ownerRec)
{
    G8CHARREC *owner;

    owner = (G8CHARREC *)ownerRec;
    if (owner->kind == 6 || owner->kind == 7) {
        txt_Format(ai_NewsText, 0x20bb, *holder);
        he_ShowNews(owner->netId, -1, ai_NewsText, 0, 0x5fd, ch_EmptyName);
    }
}
/* 0x5a8d00, the tribute event's news that it has to wait (64 B, a Ghidra-missed start, called from 0x544070;
   a reasoned guess at the text's meaning): for a character of the kinds 6 / 7 the text 0x20bd (no values) into
   ai_NewsText, shown to its netId (he_ShowNews, kind 0x5ec); nothing for another kind. Its only caller is the
   tribute / levy event's init when more than 4 of its events already run and the event is posted again */

void ai_ShowTributeDelayedNews(int rec)
{
    G8CHARREC *c;

    c = (G8CHARREC *)rec;
    if (c->kind == 6 || c->kind == 7) {
        txt_Format(ai_NewsText, 0x20bd);
        he_ShowNews(c->netId, -1, ai_NewsText, 0, 0x5ec, ch_EmptyName);
    }
}
/* 0x5a8d40, the tribute collector's news (31 B, a reasoned guess at the text's meaning): for a character of the
   kinds 6 / 7 the text 0x20be (no values) into ai_NewsText, shown to its netId (he_ShowNews, kind 0x5fb);
   nothing for another kind. Its only caller is the tribute / levy event's init (0x544070), for the collector when
   it lacks the 0x15c item */

void ai_ShowTributeCollectNews(int rec)
{
    G8CHARREC *c;

    c = (G8CHARREC *)rec;
    if (c->kind == 6 || c->kind == 7) {
        txt_Format(ai_NewsText, 0x20be);
        he_ShowNews(c->netId, -1, ai_NewsText, 0, 0x5fb, ch_EmptyName);
    }
}
/* 0x5a8d80, the tribute collector's two messages (133 B, a Ghidra-missed start from 0x544070; a reasoned guess at
   the texts' meaning): the body runs for a kind (+2) 6 and for every kind except 7 - the original's redundant
   `== 6 || != 7`, needed for the bytes; the text 0x20bf (no values) to the collector's netId, then the text
   0x20c0 with the collector's id to the event character's netId (both kind 0x5fb) */

void ai_ShowTributeClaimNews(int collectorRec, int payerRec)
{
    G8CHARREC *collector;
    G8CHARREC *payer;

    collector = (G8CHARREC *)collectorRec;
    payer = (G8CHARREC *)payerRec;
    if (collector->kind == 6 || collector->kind != 7) {
        txt_Format(ai_NewsText, 0x20bf);
        he_ShowNews(collector->netId, -1, ai_NewsText, 0, 0x5fb, ch_EmptyName);
    }
    if (payer->kind == 6 || payer->kind != 7) {
        txt_Format(ai_NewsText, 0x20c0, collector->id);
        he_ShowNews(payer->netId, -1, ai_NewsText, 0, 0x5fb, ch_EmptyName);
    }
}
/* 0x5a8e10, the tavern helper's news (201 B, a reasoned guess at the texts' meaning): the helper gets the text
   0x20c1 with the owner's id and the building (event 0x5fb) when it is a kind 6 / 7 character; then the text
   0x20c2 with the building, the owner's id, the owner's sex variant (0x20c3 / 0x20c4, from the byte +9) and the
   helper's id goes to every other kind 6 / 7 character of the 0x300 ai_Chars records (event 0x5ec). Its only
   caller is the tavern event's run handler (0x542590) when the tavern record's helper figure +0x1e is let go */

void ai_ShowTavernHelperNews(char *ownerRec, char *helperRec, int bld)
{
    G8CHARREC *owner;
    G8CHARREC *helper;
    G8CHARREC *c;
    char *p;

    owner = (G8CHARREC *)ownerRec;
    helper = (G8CHARREC *)helperRec;
    if (helper->kind == 6 || helper->kind == 7) {
        txt_Format(ai_NewsText, 0x20c1, owner->id, bld);
        he_ShowNews(helper->netId, -1, ai_NewsText, 0, 0x5fb, ch_EmptyName);
    }
    txt_Format(ai_NewsText, 0x20c2, bld, owner->id, (owner->sex != 0) + 0x20c3, helper->id);
    p = (char *)((unsigned)&ai_Chars + 0x2);
    do {
        c = (G8CHARREC *)(p - 2);
        if (c != owner && c != helper && (c->kind == 6 || c->kind == 7))
            he_ShowNews(c->netId, -1, ai_NewsText, 0, 0x5ec, ch_EmptyName);
        p += 0x21c;
    } while ((int)p < (int)((unsigned)&ai_Chars + 0x65402));
}
/* 0x5a8ee0, the parting news of a character pair (162 B, a reasoned guess at the texts' meaning): for a kind
   6 / 7 second character the text 0x132f is formatted with the first character's id, the text 0x1330 + 3 * (the
   second character's sex != 0) + r, the second character's id, the text 0x1336 + r (each r = util_Random (3))
   and the local player's slot ai_PlayerSlots[ai_OwnPlayer], and shown to the second character's netId
   (he_ShowNews 1, 0x5ec). Its only caller is ai_FindPartner (0x47ddf0) on the path that drops the partner */

void ai_ShowPartingNews(unsigned short *cRec, unsigned short *pRec)
{
    G8CHARREC *c;
    G8CHARREC *p;

    c = (G8CHARREC *)cRec;
    p = (G8CHARREC *)pRec;
    if (p->kind == 6 || p->kind == 7) {
        txt_Format(ai_NewsText, 0x132f, c->id, (util_Random(3) & 0xffff) + (p->sex != 0 ? 0x1333 : 0x1330), p->id,
                   (util_Random(3) & 0xffff) + 0x1336, ai_PlayerSlots[ai_OwnPlayer]);
        he_ShowNews(p->netId, -1, ai_NewsText, 1, 0x5ec, 0);
    }
}
/* 0x5a8f90, the feud news of a character pair (180 B, a reasoned guess at the texts' meaning): for a kind 6 / 7
   second character the text 0x1340 + r is formatted with the first character's id into a local buffer, then the
   text 0x1339 with the first character's id, the text 0x133a + 3 * (the second character's sex != 0) + r, the
   second character's id and that buffer (each r = util_Random (3)), shown to the second character's netId
   (he_ShowNews 1, 0x5ec). Its only caller is ai_FindPartner (0x47ddf0), on the branch that gives the character
   a hated candidate as its partner */

void ai_ShowFeudNews(unsigned short *cRec, unsigned short *pRec)
{
    G8CHARREC *c;
    G8CHARREC *p;
    char local[512];

    c = (G8CHARREC *)cRec;
    p = (G8CHARREC *)pRec;
    if (p->kind == 6 || p->kind == 7) {
        txt_Format(local, (util_Random(3) & 0xffff) + 0x1340, c->id);
        txt_Format(ai_NewsText, 0x1339, c->id, (util_Random(3) & 0xffff) + (p->sex != 0 ? 0x133d : 0x133a), p->id,
                   local);
        he_ShowNews(p->netId, -1, ai_NewsText, 1, 0x5ec, 0);
    }
}
/* 0x5a9050, the partner news of a character pair (162 B, a reasoned guess at the texts' meaning): as ai_ShowFeudNews
   with the texts 0x134a + r, 0x1343 and 0x1344 + 3 * sex + r; its only caller is ai_FindPartner (0x47ddf0) on
   the path where the courtship completes and the pair becomes partners */

void ai_ShowPartnerNews(unsigned short *cRec, unsigned short *pRec)
{
    G8CHARREC *c;
    G8CHARREC *p;
    char local[512];

    c = (G8CHARREC *)cRec;
    p = (G8CHARREC *)pRec;
    if (p->kind == 6 || p->kind == 7) {
        txt_Format(local, (util_Random(3) & 0xffff) + 0x134a, c->id);
        txt_Format(ai_NewsText, 0x1343, c->id, (util_Random(3) & 0xffff) + (p->sex != 0 ? 0x1347 : 0x1344), p->id,
                   local);
        he_ShowNews(p->netId, -1, ai_NewsText, 1, 0x5ec, 0);
    }
}
/* 0x5a9110, shows the local player a character's text (95 B): unless the record's id is the local player's own
   (ai_PlayerId), the text 0x20c5 with the id and the given text is formatted into ai_NewsText and shown to the
   local player's netId (event 0x605) */

void ai_ShowCharacterNews(unsigned short *cRec, char *text)
{
    G8CHARREC *c;

    c = (G8CHARREC *)cRec;
    if (c->id != ai_PlayerId) {
        txt_Format(ai_NewsText, 0x20c5, c->id, text);
        he_ShowNews(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -1, ai_NewsText, 0, 0x605, ch_EmptyName);
    }
}
/* 0x5a9170, tells a player character about another character (74 B): for a kind 6 / 7 character the text 0x20c6
   with the other character's id, shown to its netId (event 0x5fa, the second text "_NACHRICHTEN_HS_46"); called by
   cm_ExAddStraftat with the culprit and the character to tell */

void ai_NotifyPlayerOfChar(unsigned short *cRec, unsigned short *pRec)
{
    G8CHARREC *c;
    G8CHARREC *p;

    c = (G8CHARREC *)cRec;
    p = (G8CHARREC *)pRec;
    if (p->kind == 6 || p->kind == 7) {
        txt_Format(ai_NewsText, 0x20c6, c->id);
        he_ShowNews(p->netId, -1, ai_NewsText, 0, 0x5fa, "_NACHRICHTEN_HS_46");
    }
}
/* 0x5a91c0, the plague news to every player character (91 B): a random 0 .. 2 picks the news text 0x20c7 + r and
   the sound key "_KATASTROPHE_PEST_HS_0%i" (r + 0), formatted with r into the 256-byte buffer; every kind 6 / 7
   character gets the text and the sound with the event 0x600 to its netId. Called by the plague event's init
   (he_BeginPlague, "HE_EVT_PEST") after its message */

void ai_PlagueNews(void)
{
    char local[256];
    G8CHARREC *c;
    int v;

    v = util_RandomS(3);
    sprintf(local, "_KATASTROPHE_PEST_HS_0%i", v);
    txt_Format(ai_NewsText, v + 0x20c7);
    c = (G8CHARREC *)ai_Chars;
    do {
        if (c->kind == 6 || c->kind == 7)
            he_ShowNews(c->netId, -1, ai_NewsText, 0, 0x600, local);
        c++;
    } while ((int)&c->kind < (int)&gm_PlayerRecordBytes);
}
/* 0x5a9240, a plague message (112 B): util_Random (2) picks the sound "_KATASTROPHE_PEST_HS_0<v + 3>" and the
   text 0x20ca + the value; a character of the kind 6 / 7 sends the text and the sound with the news 0x600 to its
   netId */

void ai_PlagueMessage(G8CHARREC *c)
{
    char local[64];
    int v;

    v = util_RandomS(2);
    if (c->kind == 6 || c->kind == 7) {
        sprintf(local, "_KATASTROPHE_PEST_HS_0%i", v + 3);
        txt_Format(ai_NewsText, v + 0x20ca);
        he_ShowNews(c->netId, -1, ai_NewsText, 0, 0x600, local);
    }
}
/* 0x5a92b0, a plague message to every kind 6 character (119 B): util_Random (2) picks the sound
   "_KATASTROPHE_PEST_HS_<v + 0x12>" and the text 0x20cd + the value; every character of the 0x300 records
   with the kind 6 (+2) sends it with the news 0x600 to its netId (+4) */

void ai_PlagueMessageAll(void)
{
    char local[64];
    int v;
    int i;

    v = util_RandomS(2);
    sprintf(local, "_KATASTROPHE_PEST_HS_%i", v + 0x12);
    txt_Format(ai_NewsText, v + 0x20cd);
    for (i = 0; i < 0x300; i++) {
        if (ai_Chars[i][2] == 6)
            he_ShowNews(ai_CharInts[i][0], -1, ai_NewsText, 0, 0x600, local);
    }
}
/* 0x5a9330, the swap text of an office swap (253 B; its only caller is amt.c amt_ExecuteSwapMessage 0x4e6c50,
   message 0x5c): the text 0x1dcc with the first and the third character's ids for a kind 6 (+2) second character
   (sent to its +4 netId, 0x5fd), the same with the first and the second character's ids for a kind 6 third
   character, else the text 0x1dcd with the three ids to the local player's character (0x770b84 + the player *
   0x21c, 0x5fe) */

void ai_SendSwapText(unsigned short *cRec, unsigned short *o1Rec, unsigned short *o2Rec)
{
    G8CHARREC *c;
    G8CHARREC *o1;
    G8CHARREC *o2;

    c = (G8CHARREC *)cRec;
    o1 = (G8CHARREC *)o1Rec;
    o2 = (G8CHARREC *)o2Rec;
    if (o1->kind == 6) {
        txt_Format(ai_NewsText, 0x1dcc, c->id, o2->id);
        he_ShowNews(o1->netId, -1, ai_NewsText, 0, 0x5fd, ch_EmptyName);
        return;
    }
    if (o2->kind == 6) {
        txt_Format(ai_NewsText, 0x1dcc, c->id, o1->id);
        he_ShowNews(o2->netId, -1, ai_NewsText, 0, 0x5fd, ch_EmptyName);
        return;
    }
    if (c->kind != 6) {
        txt_Format(ai_NewsText, 0x1dcd, c->id, o1->id, o2->id);
        he_ShowNews(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -1, ai_NewsText, 0, 0x5fe, ch_EmptyName);
    }
}
/* 0x5a9430, tells a character that its evidence against a culprit is void (61 B): the text 0x1747 with the
   culprit's id, shown to the told character's netId (event 0x5fa) */

void ai_ShowEvidenceVoidNews(unsigned short *toldRec, unsigned short *culpritRec)
{
    G8CHARREC *told;
    G8CHARREC *culprit;

    told = (G8CHARREC *)toldRec;
    culprit = (G8CHARREC *)culpritRec;
    txt_Format(ai_NewsText, 0x1747, culprit->id);
    he_ShowNews(told->netId, -1, ai_NewsText, 0, 0x5fa, ch_EmptyName);
}
/* 0x5a9470, the gypsys' delivery news (536 B): for one to three target characters to[0 .. n - 1], the news 0x41bf
   naming the sender from is shown to each target of the kinds 6 / 7 ("_NACHRICHTEN_ZIGEUNER_06"); then the
   sender's report is built in ai_NewsText from the text 0x41ba, the targets' names (0x41bb each, the last of two
   or more with 0x41bc) and the closing text (0x41be for several, 0x41bd for one), and shown to the sender
   ("_NACHRICHTEN_ZIGEUNER_05") */

void ai_GypsyDeliveryText(void *from, int n, void **to)
{
    char line[128];
    int i;
    int last;
    int done;

    if (n < 1 || n >= 4)
        return;
    txt_Format(ai_NewsText, 0x41bf, ((AICHARREC *)from)->id);
    for (i = 0; i < n; i++) {
        if (((AICHARREC *)to[i])->kind == 6 || ((AICHARREC *)to[i])->kind == 7)
            he_ShowNews(((AICHARREC *)to[i])->netId, -1, ai_NewsText, 1, 0x5fb, "_NACHRICHTEN_ZIGEUNER_06");
    }
    strcpy(ai_NewsText, txt_Ptrs[0x41ba]);
    if (n > 1) {
        last = n - 1;
        for (i = 0; i < 4; i++) {
            if (i >= last)
                break;
            txt_Format(line, 0x41bb, ((AICHARREC *)to[i])->id);
            strcat(ai_NewsText, line);
        }
        done = 0;
        for (; i < 4; i++) {
            if (done)
                break;
            txt_Format(line, 0x41bc, ((AICHARREC *)to[i])->id);
            strcat(ai_NewsText, line);
            done = 1;
        }
        strcat(ai_NewsText, txt_Ptrs[0x41be]);
    } else {
        for (i = 0; i < 4; i++) {
            if (i >= 1)
                break;
            txt_Format(line, 0x41bb, ((AICHARREC *)to[i])->id);
            strcat(ai_NewsText, line);
        }
        strcat(ai_NewsText, txt_Ptrs[0x41bd]);
    }
    he_ShowNews(((AICHARREC *)from)->netId, -1, ai_NewsText, 1, 0x5fb, "_NACHRICHTEN_ZIGEUNER_05");
}
/* 0x5a9690, tells a player character that relatives were angered by neglected graves (69 B): for a kind 6 / 7
   character the text 0x4154 with the building, shown to its netId (event 0x5fc); gv_GraveyardTurn sends it when
   the turn lowered the relations */

void ai_TellGraveNeglect(unsigned short *plRec, int bld)
{
    G8CHARREC *pl;

    pl = (G8CHARREC *)plRec;
    if (pl->kind == 6 || pl->kind == 7) {
        txt_Format(ai_NewsText, 0x4154, bld);
        he_ShowNews(pl->netId, -1, ai_NewsText, 0, 0x5fc, ch_EmptyName);
    }
}
/* 0x5a96e0, the news of a grave sale to the graveyard's owner (159 B): for an owner of the kinds 6 / 7, the text
   0x414e with the buyer's id, the graveyard bld, the grave size's name (ai_GraveNames), the price and the owner's
   share (the language 1 swaps the building and the size name), shown to the owner ("_NACHRICHTEN_FRIEDHOF_08") */

void ai_NotifySale(int owner, unsigned short *buyer, int bld, int size, int price, int share)
{
    if (((AICHARREC *)owner)->kind != 6 && ((AICHARREC *)owner)->kind != 7)
        return;
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(ai_NewsText, 0x414e, *buyer, bld, ai_GraveNames[size], price, share);
        break;
    case 1:
        txt_Format(ai_NewsText, 0x414e, *buyer, ai_GraveNames[size], bld, price, share);
        break;
    }
    he_ShowNews(((AICHARREC *)owner)->netId, -1, ai_NewsText, 0, 0x5fc, "_NACHRICHTEN_FRIEDHOF_08");
}
/* 0x5a9780, tells a player character that its graves were leased (94 B): for a kind 6 / 7 character the text
   0x4151 with the building, the number of paid leases, the singular / plural text (0x4152 for exactly one paid
   lease, 0x4153 otherwise) and the owner's total share, shown to its netId (event 0x5fc); its only caller is
   gv_GraveyardTurn, when it paid leases (paid > 0) */

void ai_TellGravesPaid(int plRec, int bld, int paid, int sum)
{
    G8CHARREC *pl;

    pl = (G8CHARREC *)plRec;
    if (pl->kind == 6 || pl->kind == 7) {
        txt_Format(ai_NewsText, 0x4151, bld, paid, (paid != 1) + 0x4152, sum);
        he_ShowNews(pl->netId, -1, ai_NewsText, 0, 0x5fc, ch_EmptyName);
    }
}
/* 0x5a97e0, the news of a grave lease extended, to the graveyard's owner (159 B, he_AnswerGraveLease): as
   ai_NotifySale with the text 0x414f: the lessee's id, the graveyard bld, the grave size's name (ai_GraveNames),
   the price and the owner's share (the language 1 swaps the building and the size name), without a news key */

void ai_ShowGraveOrderExtended(int owner, unsigned short *lessee, int bld, int size, int price, int share)
{
    if (((AICHARREC *)owner)->kind != 6 && ((AICHARREC *)owner)->kind != 7)
        return;
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(ai_NewsText, 0x414f, *lessee, bld, ai_GraveNames[size], price, share);
        break;
    case 1:
        txt_Format(ai_NewsText, 0x414f, *lessee, ai_GraveNames[size], bld, price, share);
        break;
    }
    he_ShowNews(((AICHARREC *)owner)->netId, -1, ai_NewsText, 0, 0x5fc, ch_EmptyName);
}
/* 0x5a9880, the news of a grave order cancelled, to the graveyard's owner (139 B, he_AnswerGraveLease): as
   ai_ShowGraveOrderExtended with the text 0x4150: the character's id, the graveyard bld and the grave size's
   name (ai_GraveNames), the language 1 swapping the building and the size name */

void ai_ShowGraveOrderCancelled(int owner, unsigned short *lessee, int bld, int size)
{
    if (((AICHARREC *)owner)->kind != 6 && ((AICHARREC *)owner)->kind != 7)
        return;
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(ai_NewsText, 0x4150, *lessee, bld, ai_GraveNames[size]);
        break;
    case 1:
        txt_Format(ai_NewsText, 0x4150, *lessee, ai_GraveNames[size], bld);
        break;
    }
    he_ShowNews(((AICHARREC *)owner)->netId, -1, ai_NewsText, 0, 0x5fc, ch_EmptyName);
}
/* 0x5a9910, the haunting text of a grave owner's event 0x88 to its victim (124 B, he_RunHaunting): the
   character's id (twice) and the event's end time around a random variant of the text 0x409c .. 0x409e in
   0x409b, the language 1 putting the end time last; shown to the victim's netId (1, 0x5fb,
   "_NACHRICHTEN_FRIEDHOF_11") */

void ai_HauntingText(unsigned short *chrId, int victim, unsigned int end)
{
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(ai_NewsText, 0x409b, (util_Random(3) & 0xffff) + 0x409c, *chrId, end, *chrId);
        break;
    case 1:
        txt_Format(ai_NewsText, 0x409b, (util_Random(3) & 0xffff) + 0x409c, *chrId, *chrId, end);
        break;
    }
    he_ShowNews(((AICHARREC *)victim)->netId, -1, ai_NewsText, 1, 0x5fb, "_NACHRICHTEN_FRIEDHOF_11");
}
/* 0x5a9990, tells a character of the graveyard work's outcome (69 B): the text 0x40a8 with one of the three
   sub-texts 0x40a9 .. 0x40ab (util_Random(3)) into ai_NewsText, shown to its netId (1, event 0x5fb, the sound
   "_NACHRICHTEN_FRIEDHOF_12"); he_RunDecay sends it after he_SetLifeByAge set the second character's value */

void ai_TellGraveWorkSuccess(int chRec)
{
    unsigned int v;
    G8CHARREC *c;

    c = (G8CHARREC *)chRec;
    v = util_Random(3);
    txt_Format(ai_NewsText, 0x40a8, (v & 0xffff) + 0x40a9);
    he_ShowNews(c->netId, -1, ai_NewsText, 1, 0x5fb, "_NACHRICHTEN_FRIEDHOF_12");
}
/* 0x5a99e0, tells a character that the graveyard work failed (69 B): the text 0x40ac with one of the three
   sub-texts 0x40a9 .. 0x40ab (util_Random(3)) into ai_NewsText, shown to its netId (1, event 0x5fb, the sound
   "_NACHRICHTEN_FRIEDHOF_13"); he_RunDecay sends it in the work's other branch (no value set, its +0xbc = 0) */

void ai_TellGraveWorkFailure(int chRec)
{
    unsigned int v;
    G8CHARREC *c;

    c = (G8CHARREC *)chRec;
    v = util_Random(3);
    txt_Format(ai_NewsText, 0x40ac, (v & 0xffff) + 0x40a9);
    he_ShowNews(c->netId, -1, ai_NewsText, 1, 0x5fb, "_NACHRICHTEN_FRIEDHOF_13");
}
/* 0x5a9a30, the graveyard news of the two characters worker / other to `to` (188 B): the text 0x40ad (0x40a3,
   worker's id, its building +0x16c, other's id); with detail the text 0x40ae (0x230 for a feminine worker, else 0x20d,
   + its title +0x165) is formatted into ai_NewsSecond and appended; the news 0x5fa goes to to->netId with the
   sound "_NACHRICHTEN_FRIEDHOF_00" */

void ai_GraveyardMessage(G8CHARREC *to, G8CHARREC *worker, G8CHARREC *other, int detail)
{
    txt_Format(ai_NewsText, 0x40ad, 0x40a3, worker->id, worker->bld, other->id);
    if (detail != 0) {
        txt_Format(ai_NewsSecond, 0x40ae,
                     (worker->sex ? 0x230 : 0x20d) + worker->title);
        strcat(ai_NewsText, ai_NewsSecond);
    }
    he_ShowNews(to->netId, -1, ai_NewsText, 0, 0x5fa, "_NACHRICHTEN_FRIEDHOF_00");
}
/* 0x5a9af0, the graveyard news of the worker / other characters to `to`, without a detail sentence (83 B):
   the text 0x40af (0x40a3, the worker's id, its building +0x16c, the other's id) into ai_NewsText, shown to
   to's netId (0, event 0x5fa, the sound "_NACHRICHTEN_FRIEDHOF_00"); he_RunDecay sends it for a work without a
   result (+0xbc = 0), in place of the detailed ai_GraveyardMessage */

void ai_GraveyardMessagePlain(int toRec, unsigned short *workerRec, unsigned short *otherRec)
{
    G8CHARREC *to;
    G8CHARREC *worker;
    G8CHARREC *other;

    to = (G8CHARREC *)toRec;
    worker = (G8CHARREC *)workerRec;
    other = (G8CHARREC *)otherRec;
    txt_Format(ai_NewsText, 0x40af, 0x40a3, worker->id, worker->bld, other->id);
    he_ShowNews(to->netId, -1, ai_NewsText, 0, 0x5fa, "_NACHRICHTEN_FRIEDHOF_00");
}

int d2_WinOpenPic(short x, short y, int pic, int win); int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out); 
/* the ai.h view of 0x50c520 (the opinion of two characters, word arguments as the original pushes them) */
int gm_Opinion(int a, int b);

/* 0x5a9b50, the Domizil message of a finished research (108 B): for a kind 6 / 7 character the text 0x4074 with
   the researcher's name and family (+0x30 / +0x40) and the name of its study (0x4172 + kind) into ai_NewsText,
   then the quick-jump message 0x5ec to its netId with its building's id (+0x16c, +1), the object's id (+2) and
   the sound "_NACHRICHTEN_DOMIZIL_03"; he_RunTrainShare sends it when the research event 0x97 hands the result over */

void ai_DomizilMessageResearch(int toRec, int cRec, int objRec, int kind)
{
    G8CHARREC *to;
    G8CHARREC *c;
    AIROOM *obj;

    to = (G8CHARREC *)toRec;
    if (to->kind != 6 && to->kind != 7)
        return;
    c = (G8CHARREC *)cRec;
    obj = (AIROOM *)objRec;
    txt_Format(ai_NewsText, 0x4074, c->name, c->family, kind + 0x4172);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 0, 0x5ec, c->bld->id, obj->objId, 0,
                 "_NACHRICHTEN_DOMIZIL_03");
}
/* 0x5a9bc0, the Domizil message of a researched kind (110 B): as ai_DomizilMessageResearch with the text 0x4075
   and the kind text 0x9a8 + 2 * kind in place of the study name; he_LabIncident sends it when the research event
   0x98 sets the kind's place value */

void ai_DomizilMessageResearchKind(int toRec, int cRec, int objRec, int kind)
{
    G8CHARREC *to;
    G8CHARREC *c;
    AIROOM *obj;

    to = (G8CHARREC *)toRec;
    if (to->kind != 6 && to->kind != 7)
        return;
    c = (G8CHARREC *)cRec;
    obj = (AIROOM *)objRec;
    txt_Format(ai_NewsText, 0x4075, c->name, c->family, kind * 2 + 0x9a8);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 0, 0x5ec, c->bld->id, obj->objId, 0,
                 "_NACHRICHTEN_DOMIZIL_03");
}
#pragma pack(push, 1)
typedef struct G8OBJ {                  /* an object record of the ai_ lists (AIOBJ, ai_types.h) */
    unsigned short id;                  /* +0x00, the object's type id */
    int objId;                          /* +0x02, the object's own id */
} G8OBJ;
#pragma pack(pop)

/* 0x5a9c30, the Domizil message of the character c to `to` (108 B): for a kind 6 / 7 character the text 0x4077
   (c's name, its family, 0x9a8 + 2 * level) into ai_NewsText, then the quick-jump message 0x5ec to to->netId with
   the id of c's building, the object's id and the sound "_NACHRICHTEN_DOMIZIL_01" */

void ai_DomizilMessage(G8CHARREC *to, G8CHARREC *c, G8OBJ *obj, int level)
{
    if (to->kind != 6 && to->kind != 7)
        return;
    txt_Format(ai_NewsText, 0x4077, c->name, c->family, level * 2 + 0x9a8);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 0, 0x5ec, c->bld->id, obj->objId, 0,
                 "_NACHRICHTEN_DOMIZIL_01");
}
/* 0x5a9ca0, the Domizil news _NACHRICHTEN_DOMIZIL_04 of the building bld to the character to (98 B): for a
   kind 6 / 7 character, the text 0x407a with the first and family names of c and bld as its third argument
   (the original pushes the record pointer), then the quick-jump message 0x5ec with the building's id
   (he_LabIncident case 2 sends it after the building's level +0x5c fell) */

void ai_DomizilMessage04(G8CHARREC *to, G8CHARREC *c, AIBLD *bld)
{
    if (to->kind != 6 && to->kind != 7)
        return;
    txt_Format(ai_NewsText, 0x407a, c->first, c->family, bld);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 1, 0x5ec, bld->id, -1, 0,
                 "_NACHRICHTEN_DOMIZIL_04");
}
/* 0x5a9d00, the Domizil message of the age research (94 B): for a kind 6 / 7 character the text 0x407b with the
   character's name and family (+0x30 / +0x40) into ai_NewsText, then the quick-jump message 0x5ec to its netId
   with its building's id (+1) and the sound "_NACHRICHTEN_DOMIZIL_04"; he_LabIncident sends it for the pick that
   sets the character's life by age (he_SetLifeByAge) */

void ai_DomizilMessageAge(int toRec, int cRec, int bldRec)
{
    G8CHARREC *to;
    G8CHARREC *c;
    AIBLD *bld;

    to = (G8CHARREC *)toRec;
    if (to->kind != 6 && to->kind != 7)
        return;
    c = (G8CHARREC *)cRec;
    bld = (AIBLD *)bldRec;
    txt_Format(ai_NewsText, 0x407b, c->name, c->family);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 1, 0x5ec, bld->id, -1, 0,
                 "_NACHRICHTEN_DOMIZIL_04");
}
/* 0x5a9d60, the Domizil message of a task (114 B): for a kind 6 / 7 character the text 0x407c with the
   character's name and family (+0x30 / +0x40), the building and the task kind text 0x9a8 + 2 * the object's
   type id into ai_NewsText, then the quick-jump message 0x5ec to its netId with the building's id (+1), the
   object's id (+2) and the sound "_NACHRICHTEN_DOMIZIL_04"; he_Unk56e070's pick 1 sets the task's +0x1c /
   +0x20 fields and sends it */

void ai_DomizilMessageTask(G8CHARREC *to, G8CHARREC *c, AIBLD *bld, G8OBJ *obj)
{
    if (to->kind != 6 && to->kind != 7)
        return;
    txt_Format(ai_NewsText, 0x407c, c->name, c->family, bld, obj->id * 2 + 0x9a8);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 1, 0x5ec, bld->id, obj->objId, 0,
                 "_NACHRICHTEN_DOMIZIL_04");
}
/* 0x5a9dd0, the Domizil message of a place's lowered value (98 B): for a kind 6 / 7 character the text 0x407f
   with the character's name and family (+0x30 / +0x40) and the place's goods name text 0x9a8 + 2 * id into
   ai_NewsText, then the quick-jump message 0x5ec to its netId with the building's id (+1) and the sound
   "_NACHRICHTEN_DOMIZIL_04"; he_LabIncident's pick 4 sends it after ahm_SendSetPlaceValue lowered the place's value */

void ai_DomizilMessagePlace(G8CHARREC *to, G8CHARREC *c, AIBLD *bld, int placeId)
{
    if (to->kind != 6 && to->kind != 7)
        return;
    txt_Format(ai_NewsText, 0x407f, c->name, c->family, placeId * 2 + 0x9a8);
    he_MessageBoxQuickjump(to->netId, -1, ai_NewsText, 1, 0x5ec, bld->id, -1, 0,
                 "_NACHRICHTEN_DOMIZIL_04");
}
/* 0x5a9e40, the Domizil message 12 (76 B): the text 0x4070 with the building and the text 0x4071 + level into
   ai_NewsText, then the quick-jump message 0x5ec to the character's netId (+4) with the building's id (+1) and
   the sound "_NACHRICHTEN_DOMIZIL_12"; no kind 6 / 7 test like the other Domizil messages (gv_EffectNotifyOwner
   sends it only for a kind 6 character, level 0 / 1 / 2 by the effect's mode and value) */

void ai_DomizilMessage12(char *chRec, char *bldRec, int level)
{
    G8CHARREC *ch;
    AIBLD *bld;

    ch = (G8CHARREC *)chRec;
    bld = (AIBLD *)bldRec;
    txt_Format(ai_NewsText, 0x4070, bld, level + 0x4071);
    he_MessageBoxQuickjump(ch->netId, -1, ai_NewsText, 1, 0x5ec, bld->id, -1, 0,
                 "_NACHRICHTEN_DOMIZIL_12");
}

/* the unit's data in the original's order. .rdata: its named constants 0x5eed18 .. 0x5eed88 (after lc.c's literals,
   1600000.0 0x5eed10 the last), then its literals 52.0f, 0.435f, 0.48f; .data: ai_MapScrollEnabled 0x62c26c and
   ai_MapCellScale (the plain block after lc.c's strings), the news keys among its strings; .bss: 0x6f268c .. 0x6f2ac8
   (after lc.c's ai_BauplatzCount 0x6f2688) */
const unsigned __int64 ai_KindMaskTable[10] = { 0x13800, 0x10000000, 0x10013800, 0x7800000, 0x344000, 0x1a0400300, 0x6, 0x10010, 0x7808000, 0x800a0 };  /* 0x5eed18, ten 64-bit building kind masks before gypsyFilter (the same form as ai_BuildingTypeMasks); no reader in the surviving code (the removed functions' or the folded immediates'). Name and extent a guess: one object of 80 bytes (8-aligned as the original's) */
const volatile unsigned __int64 gypsyFilter = 0x1a7fdfbb6;  /* 0x5eed68, the filter of the gypsy windows, the flags and kind he and lc pass to ai_PickActionBuilding too (volatile: VC6 would fold it into immediates here; the original reads it from memory) */
const unsigned int ai_BuildingTypeMasks[6] = { 2692498336, 1, 3766305712, 1, 0xf82806f, 2 };  /* 0x5eed70, bit masks of building classes (defined beside gypsyFilter: ai.c reads it through the extern, from memory) (three 64 bit masks: [2..3] the house-owner classes, [4..5] the classes lc.c's ai_PassesMapPickFilter greys out under its 0x40000000 flag) */
int ai_MapScrollEnabled = 1;  /* 0x62c26c, 1 while the town map window has the input (ai_UpdateTownMap scrolls it; rt_EditCartRoute and ai_MapPick set it): the first of the unit's .data block */
float ai_MapCellScale = 5.33f;  /* 0x62c270, the town map pixels per scene cell: 7.5 for a map bitmap 0x780 wide or wider, else 5.33 (the map loader 0x5b2090 sets it) */
int ai_BauplatzCount = 0;  /* 0x6f2688 */
int ai_DragSavedMaxY = 0;  /* 0x6f268c, the mouse bounds di_MaxY / di_MinY / di_MaxX / di_MinX kept while the town map is */
int ai_DragSavedMinY = 0;  /* 0x6f2690, dragged with the right button (ai_DragTownMap) */
int ai_MapScrollY = 0;  /* 0x6f2694, the town map's scroll the key / edge scroll last computed (ai_ScrollTownMap) */
int ai_DragSavedMaxX = 0;  /* 0x6f2698 */
int ai_DragSavedMinX = 0;  /* 0x6f269c */
int ai_MapScrollX = 0;  /* 0x6f26a0 */
int ai_DragScrollX = 0;  /* 0x6f26a4, the map window's scroll when the drag began */
int ai_DragScrollY = 0;  /* 0x6f26a8 */
struct D2SURF *rt_MapSurface = 0;  /* 0x6f26ac, the town map's screen surface (in this unit's .bss block; rt.c's rt_EditCartRoute draws its routes on it) */
int *ai_PlacementSites = 0;  /* 0x6f26b0, the candidate site list of the running building placement (lc_GebaeudeAusrichten points it at its sites[]); the map window marks each entry */
int ai_DragStartX = 0;  /* 0x6f26b4, the mouse position (dm_L) the drag began at */
int ai_DragStartY = 0;  /* 0x6f26b8 */
int ai_Dragging = 0;  /* 0x6f26bc, 1 while the town map is dragged */
G8SETTING *ai_Settings = 0;  /* 0x6f26c0, the head of the production settings chain (the "hp_" records, 0x4c each), 0: none; ai_FindSetting / ai_FindSettingSlots walk it, ai_ProdSetting links and unlinks, ai_SaveSettings / ai_LoadSettings write and read it */
char ai_NewsText[1024] = { 0 };  /* 0x6f26c8, the shared text buffer of the unit's news messages: txt_Format writes the text into it, he_ShowNews shows it */
char ai_NewsSecond[1176] = { 0 };  /* 0x6f2ac8, the second text buffer of the news messages that append a part (txt_Format into it, strcat onto ai_NewsText); its size is the room up to ai_InvChar */
