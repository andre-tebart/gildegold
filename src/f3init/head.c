/* f3init.c -- the game initialisation of the town (0x50d100 ..): tag "f3_init:" ("f3_init:Gebaeude des Typs %s allokiert", 0x622ecc) and "init_" in the error strings ("init_LoadObjektScene:", "init_LoadGebaeudeScene:", "init_LocalData()"). The scene object names ("ob_NAME" / "gb_NAME" with the variant suffixes "_A" .. "_H") are mapped to the object / building prototypes, the object and building scenes "scenes/*ob_%s.ed3" / "scenes/*gb_%s.ed3" are loaded into universes of their own (tables 0x76faa0 per object prototype, 0x76fa60 per building class), the start objects are placed. The .data of this unit starts at 0x61adf8 (right after f3gm's last string 0x61ade0): its initialised globals (0x61adf8 / 0x61ae00 and the table of 19 records of 0x6c4 bytes at 0x61ae08) come first, then its strings from "KONTOR" (0x622e94) on. Drafts of layout 2026-10-23, names are ours; the unit end is open.
   unit of rea 8cef7d5b (src/game/f3init.c); boundaries in decl/text.dcl, notes in src/f3init/open.md */
struct VFSENTRY;
struct D3OBJECT;
struct D3SURF;
struct VFSDIR;
struct D3SM;
struct D3TERRAIN;
extern char ai_PlayerSlots[][0x2f4];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#include "gv.h"
#include "d3.h"
#include "d3_quad.h"
#include "txt.h"
#include "evt.h"
#include "d3_light.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char d3_AnimHead[];
extern char gm_PlayerRecords[];
extern unsigned char gm_Offers;
extern unsigned char ai_Unk760420;
extern unsigned char ch_Motions;

/* the scene object view of this unit: a CHARACTER (ch.h, 0x244 bytes, the list 0x1468ae0); the unit cannot
   include ch.h (gv.h's d3sm_GetVertex is a K&R declaration), so the fields this unit reads are declared here
   (type debt, see notes/units/f3init.md) */
typedef struct SCENEOBJ {
    char pad000[0x2c];              /* 0x00 .. 0x2b */
    int group;                      /* 0x2c, ch.h CHARACTER.group: the group id the character belongs to (compared with
                                       ch_CurrentGroup / evt_NoGroup; here with the building's id) */
    int scriptGroup;                /* 0x30, ch.h CHARACTER.scriptGroup: the group of the script that created / moved it
                                       (here compared with the room object's instance id) */
    char pad034[0x14];              /* 0x34 .. 0x47 */
    unsigned proto;                 /* 0x48, the prototype reference: 0x1000000 | object index (ai_Goods), 0x2000000 | building index (ai_BTypes) */
    char pad04c[0x1c];              /* 0x4c .. 0x67 */
    float soundRange;               /* 0x68, the radius of the object's sound (0x5126a0: + 50 = the range) */
    char pad06c[0x1c];              /* 0x6c .. 0x87 */
    void *scene;                    /* 0x88, CHARACTER.scene: the scene slot the character is in (a D3SCENE *) */
    char pad08c[0x98];              /* 0x8c .. 0x123 */
    void *transport;                /* 0x124, CHARACTER.transport: the character's transport record, 0 = none */
    char pad128[0xc4];              /* 0x128 .. 0x1eb */
    void *mesh;                     /* 0x1ec, D3OBJX.mesh: the mesh driver (its texture-set list is at +0xf4) */
    char pad1f0[0x8];               /* 0x1f0 .. 0x1f7 */
    struct SCENEOBJ *parent;        /* 0x1f8, D3OBJECT.parent */
    char pad1fc[0x4];               /* 0x1fc, D3OBJECT.child */
    struct SCENEOBJ *owner;         /* 0x200, the owner record (the prototype instance of an object or building) */
    char pad204[0x8];               /* 0x204 .. 0x20b */
    void *sound;                    /* 0x20c, D3OBJX.sound: the sound attached to the object (0x5128a0 attaches, 0x5127c0 frees) */
    char pad210[0x8];               /* 0x210 .. 0x217 */
    int type;                       /* 0x218, D3OBJECT.type */
    char pad21c[0x4];               /* 0x21c, D3OBJECT.type0 */
    int kind;                       /* 0x220, D3OBJECT.kind */
    int mask;                       /* 0x224, D3OBJECT.mask */
} SCENEOBJ;

/* the chain of the character records the info panel walks (gm_Query(0, 2, 7, 4, 0x1d)): the next record at +0x3b,
   which is not 4-byte aligned, so this view is packed (no other function uses it) */
#pragma pack(push, 1)
typedef struct INIT_PANELCHAR {
    char pad000[0x3b];              /* 0x00 .. 0x3a */
    struct INIT_PANELCHAR *next;    /* 0x3b */
} INIT_PANELCHAR;
#pragma pack(pop)

/* the office record (amt.h's AMTREC, 0x18 bytes) as this unit reads it; amt.h cannot be included here, so only
   the field in use is declared (type debt, see notes/units/f3init.md) */
typedef struct INIT_OFFICE {
    unsigned idWord;                /* 0x00, AMTREC.idx in the low byte (the original loads the whole dword) */
    char pad04[0x4];                /* 0x04 .. 0x07 */
    unsigned kindWord;              /* 0x08, AMTREC.kind in the low byte (the original loads the whole dword) */
    char pad0c[0xc];                /* 0x0c .. 0x17 */
} INIT_OFFICE;

/* the building record view (ai.h's AIBLD, 0xa9 bytes, ai_Blds): the fields this unit reads */
#pragma pack(push, 1)
typedef struct INIT_BLD {
    unsigned char type;             /* 0x00, the index into ai_BTypes */
    int id;                         /* 0x01, the building's id (0x500190 / gm_Query use it) */
    char pad05[0x20];               /* 0x05 .. 0x24 (ai.h's AIBLD.name) */
    unsigned short masterId;        /* 0x25, the character id of the building's master (ai.h's AIBLD.masterId;
                                       the new-town message 0x1b asks for every building with 0xffff here) */
    unsigned short owner;           /* 0x27, the character id of the building's person, 0xffff = none
                                       (ai.h's AIBLD.owner; init_OpenBuilding reads its kind) */
    char pad29[0x33];               /* 0x29 .. 0x5b */
    unsigned char level;            /* 0x5c, the building's level 0 .. 100 (ai.h's AIBLD.level; the value the
                                       building's worth 0x509b10 is weighted by) */
    int objList;                    /* 0x5d, the head of the building's object-instance list (gm_Query 0x4ffe90) */
    char pad61[4];                  /* 0x61 .. 0x64 */
    int player;                     /* 0x65, the city the building belongs to: an index into the player slots
                                       ai_PlayerSlots (init_StockMarketStalls reads the city's import / export
                                       goods through it, he.c / sv.c its name) */
    char pad69[0x8];                /* 0x69 .. 0x70 */
    int plants;                     /* 0x71, the 64 G15PLANT records (0x18 bytes each, ai.h's AIBLD.plants) of the
                                       building's vegetation map (veg_ShowPlants) */
    char pad75[0x34];               /* 0x75 .. 0xa8 */
} INIT_BLD;

/* the object instance view (ai.h's AIOBJ, 0x43 bytes, what gm_Query finds): the fields this unit reads */
typedef struct INIT_OBJ {
    unsigned short proto;           /* 0x00, the object prototype (an ai_Goods index) */
    int objId;                      /* 0x02, the instance's own id (the object owner / field id) */
    char pad06[0xe];                /* 0x06 .. 0x13 */
    int list;                       /* 0x14, the object list the object holds (ai.h's AIOBJ.unk14 / list; gm_Query finds its goods) */
    char pad18[0x2b];               /* 0x18 .. 0x42 */
} INIT_OBJ;
#pragma pack(pop)

/* the player's stock record (hst.c's G12STOCK, 0x80 bytes: init_StockList[player][0x55]); the fields this unit
   reads, named after f3gm.c's 0x4ffa10 / 0x4ff550 (the record's wanted amount at 0x20, the stored price 0x38) */
typedef struct INIT_STOCK {
    unsigned short item;            /* 0x00, the goods prototype (an ai_Goods index) */
    char pad02[2];                  /* 0x02 */
    int amount;                     /* 0x04, the amount the player holds */
    char pad08[0x18];               /* 0x08 .. 0x1f */
    float demand;                   /* 0x20, the amount the market wants */
    float mean;                     /* 0x24, the mean amount (f3gm.c 0x4ffa10) */
    char pad28[0x10];               /* 0x28 .. 0x37 */
    float price;                    /* 0x38, the stored price */
    char pad3c[0x44];               /* 0x3c .. 0x7f */
} INIT_STOCK;

/* the stock block of one player (0x2a90 bytes): four header ints, then the 0x55 goods records */
typedef struct INIT_STOCKBLOCK {
    int priceIndex[4];              /* 0x00, the player's two price indices and their factors (f3gm.c 0x4ff550) */
    INIT_STOCK rec[0x55];           /* 0x10 */
} INIT_STOCKBLOCK;

/* the game clock (ai.h's AITIME, 14 bytes at 0x770ae0) as the long message copies it */
#pragma pack(push, 1)
typedef struct INIT_TIME {
    int year;                       /* 0x0, the year (0x578 + it is the calendar year) */
    unsigned short hour;            /* 0x4, the hour */
    short unk6;                     /* 0x6 */
    int unk8;                       /* 0x8 */
    short unkc;                     /* 0xc */
} INIT_TIME;

/* the long-message record (ai.h's AIMSG, 0xf8 bytes, cm_SendLongMessage's argument); only the fields this unit
   writes are declared (type debt: ai.h cannot be included here) */
typedef struct INIT_AIMSG {
    char pad000[0x4];               /* 0x00 */
    unsigned char type;             /* 0x04, the message kind (0x1b / 0x4c) */
    char pad005[3];                 /* 0x05 .. 0x07 */
    int to;                         /* 0x08, the receiving player's netId (ai.h's AIMSG.to) */
    int fromId;                     /* 0x0c, the sending building's id (0xffffffff = none) */
    int nIds;                       /* 0x10, how many of ids[] are set (ai.h's AIMSG.nIds) */
    int reserved14;                 /* 0x14, only ever written 0 (ai.h's AIMSG.unk14) */
    char pad018[0x10];              /* 0x18 .. 0x27 */
    INIT_TIME time;                 /* 0x28, the game clock the message was made at (ai.h's AIMSG.time) */
    unsigned char flags;            /* 0x36, the message flags (2 = a new-town building, 1 = the mission mode) */
    char pad037;                    /* 0x37 */
    int ids[8];                     /* 0x38 */
    int arg[11];                    /* 0x58 */
    char pad084[0x74];              /* 0x84 .. 0xf7 */
} INIT_AIMSG;
#pragma pack(pop)

/* the office kind record (amt.h's AMTKIND, 0x18 bytes) as this unit reads it */
typedef struct INIT_KIND {
    char pad00[0x8];                /* 0x00 .. 0x07 */
    unsigned levelWord;             /* 0x08, AMTKIND.level in the low byte (the original loads the whole dword) */
    char pad0c[0xc];                /* 0x0c .. 0x17 */
} INIT_KIND;

/* the character record prefix the start of the game reads (ai.h's AICHAR, 0x21c bytes); ai.h cannot be included
   here, so only the fields in use are declared (type debt, see notes/units/f3init.md) */
typedef struct INIT_CHAR {
    unsigned short id;              /* 0x000, 0xffff = the slot is free */
    unsigned char kind;             /* 0x002 */
    char pad003[0x1];               /* 0x003 */
    unsigned int netId;             /* 0x004, the record's network id (the field block id) */
    char pad008[0x1];               /* 0x008 (ai.h health) */
    unsigned char female;           /* 0x009, 0 male / 1 female (unk_4a7140.h's female, he.c's title texts by +9) */
    unsigned short age;             /* 0x00a, the age in years (am.c: "grown up (0x0a + 4 >= 0x20)") */
    char pad00c[0x20];              /* 0x00c .. 0x02b */
    int group;                      /* 0x02c, the id of the building the character belongs to (-1 = none, sv.c 0x52b2a0) */
    char name[0x2c];                /* 0x030, the start of ai.h's AICHAR.name (0x58 bytes; this view stops at
                                       the partner view below) */
    int partner;                    /* 0x05c, the spouse's character id, -1 = none (am.c: gm_FindChar(+0x5c)) */
    char pad060[0x20];              /* 0x060 .. 0x07f */
    unsigned char skillBytes[5];    /* 0x080, the five bytes the opponent difficulty moves (inside ai.h's name / skill union) */
    char pad085[0xdf];              /* 0x085 .. 0x163 */
    unsigned char jobId;            /* 0x164, the character's job / title group (ai.h's jobId, gm_TitleGroup's argument) */
    char pad165[0x1];               /* 0x165 (ai.h title) */
    unsigned char officeKind;       /* 0x166, the kind of the office held, 0 = none (amt.h's officeKind) */
    char pad167[0x5];               /* 0x167 .. 0x16b */
    INIT_BLD *bld;                  /* 0x16c, the building the character is in / belongs to (ai.h's AICHAR.bld) */
    char pad170[0x14];              /* 0x170 .. 0x183 */
    SCENEOBJ *model;                /* 0x184, the character's scene record (ai.h's AICHAR.model; ch_SetVisible /
                                       ch_PlaceOnGround take it as a CHARACTER) */
    char pad188[0x94];              /* 0x188 .. 0x21b */
} INIT_CHAR;

/* d3_sm.h's D3TERRAIN as far as this unit reads it (the unit cannot include d3_sm.h: gv.h's
   d3sm_GetVertex is a K&R declaration); type debt, see notes/units/f3init.md */
typedef struct INIT_TERRAIN {
    unsigned int size;              /* 0x000, the cells per side (0x40 / 0x80 select the shadow map size) */
} INIT_TERRAIN;

/* stdlib min / max (the body ai.h defines; f3init cannot include ai.h, its AICHAR / AIBLD views differ) */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#include "data_views.h"

/* the object list of init_LoadSiteScene (0x510770): the count, the objects by name and the name substring the
   d3_ForEachObject callback init_CollectSiteObject keeps (a D3OBJECT's name is at its +0x00) */
typedef struct INIT_SITELIST {
    int n;                          /* 0x000, the number of collected objects */
    D3OBJECT *obj[0x200];           /* 0x004, the collected objects */
    char key[0x80];                 /* 0x804, the name substring the collector searches (the site name) */
} INIT_SITELIST;                    /* 0x884 bytes */
#define INIT_PROTO_OBJECT   0x1000000u   /* SCENEOBJ.proto: an index into the object prototypes ai_Goods */
#define INIT_PROTO_BUILDING 0x2000000u   /* SCENEOBJ.proto: an index into the building prototypes ai_BTypes */
#define INIT_KONTOR_BUILDING 0x47        /* the building prototype index the special name "KONTOR" maps to */
#define INIT_PROTO_MASK ((unsigned)&txt_Keys + 0x181bf)        /* the prototype reference's index bits (o + 0x48) */
#define INIT_PLAYER_SLOTS_END ((unsigned)&init_SlotCurrency + 0x888)   /* one past the player slot table 0x7e3f40 (0x2f4 bytes per slot) */
#define INIT_SLOT_CURRENCY_END ((unsigned)&gm_Offers + 0x44)  /* one past the currency words of the player slots 1 .. 3 (0x7e4288 + 3 * 0x2f4) */
#define INIT_SCENE_HEIGHTMAP_END ((unsigned)init_SceneHeightMaps + 0x40 * 0x3e0) /* one past the height map of the last scene: 0xfd7c50 + 0x40 * 0x3e0 (init_FreeScenes); anchored on the array, not on d3_AnimHead's 0x10 (their linked distance differs from the original's) */
#define INIT_CHARACTER_COUNT 0x300      /* ch.h's CH_MAX: the slots of the character list ch_List (0x1468ae0) */
#define INIT_START_CHARACTERS_END ((unsigned)&ai_Unk770d8c + 0x1fb4) /* one past the 16th character record: 0x770b80 + 16 * 0x21c (init_StartNewGame) */
#define INIT_CHARACTERS_END (unsigned)&gm_PlayerRecords  /* one past the character table ai_Chars (0x770b80 + 0x300 * 0x21c) */
#define INIT_LAST_CHARACTER ((unsigned)&ai_Unk770d8c + 0x64fd8)  /* the last character record of ai_Chars (INIT_CHARACTERS_END - 0x21c; the greeting loop walks up to its +2) */
#define INIT_FIELD_MONEY 0x1ac          /* the character's money field (the field block of the start purse) */
#define INIT_FIELD_RELIGION 0x0c        /* the character's religion field: 0 catholic / 1 protestant (the two judges) */

/* the functions of f3gm.c this unit calls (declared by their addresses there) */
char *gm_Query(void *list, int n, ...);
unsigned short *gm_QueryNext(void);
unsigned char *gm_BuildQuery(int n, ...);
unsigned char *gm_BuildNext(void);
int gm_PlayerFactor(int a, unsigned char k);
float gm_ItemPrice(int item, unsigned char n);
int gm_ListStock(short *l, int id);               /* 0x50ab50, f3gm.c */
unsigned char *gm_FirstBuilding(int n);
float gm_Unk508740();
int gm_IsHomeType(unsigned char *b);
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, the player slots */
extern INIT_STOCKBLOCK init_StockBlock[];  /* 0x7d6a60, one 0x2a90-byte stock block per player (the records start at +0x10) */
extern char ch_EmptyName[];             /* 0x631090 */
extern int d3_CurrentScene;                /* 0x633444 */
extern int ai_NextObjectId;                /* 0x61a654, the next object id */
extern int ai_PlayerBuilding;                /* 0x6f2308 */
extern int ai_FallbackBuilding;                /* 0x6f230c */
char d3t_GetBoundingSphere(struct D3OBJECT *obj, struct D3OBJECT *camera, struct D3VECTOR *center, float *radius);  /* 0x411eb0, d3_tools.c: the center and radius of the mesh's 8-corner box */
int gm_IsBuildingKind(int n);           /* 0x4fe940, f3gm.c: 1 when the object prototype n is a building part */
int gm_FindObjectProto(char *name);     /* 0x4fe900, f3gm.c: the object prototype of the name */
int gm_FindBuildingKind(char *name);    /* 0x502060, f3gm.c: the building kind of the name */
int unk_Unk503530(int p, void *b, void *o, int c);  /* 0x503530, f3gm.c: the permission bits of the character p in the building b (or the object o) */
void init_LocalData(void);
unsigned char * gm_CreateBuilding(unsigned char kind, unsigned short owner);                     /* 0x501580 */


extern char *ai_Goods;              /* 0x91aaf0, the object prototypes (0x2dc of 0x41 bytes) */
struct D3SCENE;
extern struct D3SCENE *d3_DefaultScene;  /* 0x5fb5b0, the current scene (d3.h: D3SCENE *) */
extern int d3_QuadTree;
extern signed char init_ObjectUniverse[0x2dc];
extern signed char init_BuildingUniverse[0x22];
/* init_SceneDirty: view of d3_Scenes (tools/data_views.py) */
extern char init_SceneName[];             /* 0xfd7ba0, the name of each scene record (0x3e0 bytes apart) */
extern int ai_Unk1468ae0[];             /* 0x1468ae0, the scene objects (0x100) */
extern int d3_CurrentScene, ai_Unk633738;  /* 0x633444, 0x633738 */
extern unsigned char ai_GameState;      /* 0x6f25ac */
extern int ai_FrameWait;
extern unsigned sv_FrameMask;           /* 0x760638 */
extern short d2_ScreenW, d2_ScreenH;/* 0xfb9072 / 0xfb9070 */
extern int ai_GameFlags;      /* 0x6f23f8 */
void he_RemoveGfxInfos(int obj);            /* 0x53ebe0, he.h */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                     /* 0x447300 */
char d3if_SelectScene(int index, char keep);   /* 0x4478e0, d3_interface.c */
void d3if_Reset(void);                         /* 0x4473f0, d3_interface.c */
void d3sm_Destroy(struct D3SM *h);             /* 0x45ef60, rd.h */
char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));  /* 0x44be60, cm.h */
char d3_ForEachObject2(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, void (*cb)(struct D3OBJECT *obj));  /* 0x438e60, d3_engine.h */
struct D2FADE;
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */                     /* 0x4767d0 */
void d2_ReleaseFade(struct D2FADE *f); /* 0x476860, d2.h */                     /* 0x476860 */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */
int ai_Unk42ea50();                     /* 0x42ea50 */
int tb_SetFunctionOff();                     /* 0x42ea10 */
int ai_Unk402220();                     /* 0x402220 */
struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);                     /* 0x453e80 */
int ch_PlaceOnGround();                     /* 0x404a80 */

/* the canonical declarations of the engine / other-unit functions the scene loaders call (their headers drag the
   gv.h conflicts in; the types match the canonical prototypes, the CHARACTER arguments are cast through the
   SCENEOBJ view - type debt, see notes/units/f3init.md) */
struct CHARACTER;
extern struct SCENEOBJ *ch_List[INIT_CHARACTER_COUNT];   /* 0x1468ae0, ch.h's CHARACTER *ch_List[CH_MAX] */
struct D3HMAP *ch_GetSceneHMap(struct D3SCENE *scene);                           /* 0x402220, ch.h */
void ch_SetPos(struct CHARACTER *ch, D3VECTOR *pos, int unk);                    /* 0x4025c0, ch.h */
int ch_PlaceOnGround(struct CHARACTER *ch, int force);                           /* 0x404a80, ch.h */
struct D3OBJECT *ch_FindDummy(void);                                             /* 0x406790, ch.h */
int d3t_FindFreeScene(void);                                                     /* 0x411bb0, d3_tools.c */
void err_Message(char *text);                                                    /* 0x4222b0, err.c */
int tb_SetFunctionOff(void (*proc)(void), int off);                              /* 0x42ea10, tb.h */
int tb_IsFunctionOff(void (*proc)(void));                                        /* 0x42ea50, tb.h */
void d2_ClearScreen(void);                                                       /* 0x46e990, d2.h */
void d2_ShowPicture(int pic);                                                    /* 0x46cfd0, d2.c */
int d2_ClearSurface(struct D2SURF *surf);                                        /* 0x4796c0, gfx.c */
void gm_RandomMarketShake(int unk);                                             /* 0x4ff450, f3gm.c */  /* @original: the original pushes one argument at both call sites; f3gm.c defines no parameter */
extern short d2_ScreenW, d2_ScreenH;                                             /* 0xfb9072 / 0xfb9070, d2.h */
void d3sm_Build(void *h, void *t);                                               /* 0x45e620, d3_engine.h */
void sv_SceneLoadProgress(int kind, int cur, int total);                         /* 0x521c10, sv.c */
void sv_UpdateClock(void);                                                       /* 0x5315c0, sv.c */
void init_SceneObjectDestroyCB(int o);
void init_AdjustSceneLights(int o);
int init_SceneNameToPrototype(char *o);
extern int cm_FieldObject;                /* 0xa68d2c */
extern char *ai_JudgeCatholic, *ai_JudgeProtestant;  /* 0x6f1bfc, 0x6f1c00, the two judges (0x6f1bfc Katholisch, 0x6f1c00 Evangelisch) */
extern int ai_Opponents[];              /* 0x75e160, the difficulty level 0 .. 4 of each opponent (the seven globals ai_Opponents .. opponent_g of the options 0x5d4c70 / 0x5d5640 and of the selection menu 0x537040) */
extern int ai_Unk6f2440;
void cm_BeginSendGroup(void);                     /* 0x4c3b30 */
void cm_EndSendGroup(void);                     /* 0x4c3b40 */
int cm_SendGroupState();                     /* 0x4c3b50 */
void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);                     /* 0x4c4650 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
int cm_GetSlotValue(int h);                     /* 0x4c3b10 */
int cm_GetSlotState(int h);                     /* 0x4c3ae0, cm.h (gv.h has the K&R form) */
int cm_AddFieldDelta(int size, int n, short field, void *v);                     /* 0x4c3c00 */
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);                     /* 0x4c4880, @original: this unit's call sites push the third argument as an int - 0x50f42c zero-extends the unsigned short goods id (`and eax,0xffff`) where cm.h's short view would sign-extend */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */
int amt_GetOffice(unsigned char i, struct AMTREC *out);  /* 0x4e6f30, amt.c: copies the office i to out (an AMTREC, 0x18 bytes) */
int amt_GetKindLevel(int k);                    /* 0x4e7c40, amt.c: the level of the office kind k */
int amt_GetKind(int k, struct AMTKIND *out);              /* 0x4e6f80, amt.c: copies the office kind k to out (an AMTKIND, 0x18 bytes) */
int amt_GetFirstOffice(unsigned int k, struct AMTREC *out);  /* 0x4e6ec0, amt.c: copies the first office of the kind k to out (an AMTREC) */
int amt_SendOfficeOwner(int a, char *b, int c); /* 0x4e6650, amt.c */
void gm_AgeCharacter(char *c, unsigned age);    /* 0x4fc6f0, f3gm.c: ages the character c to the age */
int gm_TitleGroup(int k);                       /* 0x504ad0, f3gm.c */
int gm_TitleNumber(int group, int rank);        /* 0x5043d0, f3gm.c */
void gm_Shuffle(unsigned n, int *out);                /* 0x505d30, f3gm.c */
int ai_WorksForMaster(struct AICHAR *c);                 /* 0x47dd70, ai.c */
void hmt_Begin(void);                           /* 0x4ad0a0, hmt.c */
extern int sv_PanelFlag;                /* 0x75e8c0 */
extern int init_StatusForm;
int gm_BuildingClass(int type);                     /* 0x501d80 */
int gm_BuildingMainObject(unsigned char *b);                     /* 0x501b50 */
int ai_BuildingName(int id, char *out);                  /* 0x58f640, unk_58bbd0.c: the name of a building kind id */
int ai_FindBauplaetze(char *prefix, int *list);           /* 0x58f7a0, unk_58bbd0.c: the free placement candidates of a name prefix */
void ai_ObjectAngle(void *out, int plot);                 /* 0x58fd90, unk_58bbd0.c: the angle of the object plot to out */
void cm_SendOwnership(int a, int b, int c);               /* 0x4c54a0, cm.c: message 0x38, the building a changes its person from b to c */
void cm_SendBoss(int a, int b);                            /* 0x4c5540, cm.c: message 0x39, the building a gets the character b as its boss */
int cm_RequestCreateGeb(int a, int k, float *pos, int *v);/* 0x4c5ae0, cm.c: message 0x4c, a building of kind k at pos */
unsigned char *gm_NearestFreeBuilding(unsigned char *b, unsigned kind); /* 0x501f60, f3gm.c */
int gm_FirstTitleNumber(int x);                          /* 0x504290, f3gm.c: the first title number of the job group of x */
int gm_BuildingKindValue(int kind);                      /* 0x509d30, f3gm.c: the sum of the prices of the kind's negative object words */
void main_SetLoadProgress(int unused, int pct);          /* 0x537af0, main.c: the loading progress bar */
void he_PostEvent6d(void);                                /* 0x53efe0, he.c: posts the event 0x6d (once) */
void he_PostEvent6b(void);                                /* 0x53f070, he.c: posts the event 0x6b (once) */
void he_PostEvent7a(void);                                /* 0x53f100, he.c: posts the event 0x7a (once) */
void he_PostEvent2b(void);                                /* 0x53f190, he.c: posts the event 0x2b for every figure of kind 5 / 7 without one */
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);                     /* 0x4c55d0 */
int gv_SendGrave(unsigned char *o);                     /* 0x4f15e0 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);                     /* 0x4c51b0 */
void d2_DrawAll(void);                      /* 0x46e150, d2.c */
int ai_Unk511810();                     /* 0x511810 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
unsigned short *gm_CreateObject(int id, unsigned proto, int n);  /* 0x500190, f3gm.c: the new object instance */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
int init_BuildingKindName();                     /* 0x50dba0 */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
float gm_ItemPrice(int item, unsigned char n);
int gm_ListStock(short *l, int id);               /* 0x50ab50, f3gm.c */
int ai_Unk633748 = 0;  /* 0x633748 */
char ai_Unk6339db = 0;  /* 0x6339db */
/* init_StatusFormat: view of txt_Ptrs (build/mk_txt_views.py) */
int d2_CutsceneActive = 0;  /* 0x6ed9d4 */
char * ai_KindBChars[8] = {0};  /* 0x6f1c04, the characters of the kind */
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, the player slots */
extern INIT_STOCKBLOCK init_StockBlock[];  /* 0x7d6a60, one 0x2a90-byte stock block per player (the records start at +0x10) */
extern int cm_NetFlags, d2_CutsceneActive, init_TownSoundCount;  /* 0x6f2594, 0x6ed9d4, 0x6f1c34 */
unsigned char init_TextureSet = 0;  /* 0x6f1c24 */
/* net.h's NETCONN (hst.c's view): the fields this unit reads; 0x1460ae0 is the network connection of a
   running network game, -1 (an invalid socket) when there is none */
extern struct {
    int s;                              /* 0x00, the socket */
    int pad04[3];
    int closed;                         /* 0x10 */
} net_Conn;                             /* 0x1460ae0 */
int d3_QuadTree = 0;  /* 0x6f1c28 */
int init_TownSoundCount = 0;  /* 0x6f1c34 */
int ai_FrameWait = 0;  /* 0x6f22bc */
int main_StartHouseCheat = 0;  /* 0x6f2430, no writer in the exe: nonzero gives the first kind 6 character its start house ( */
extern int init_TownSounds[16];
extern float init_SoundVolume;
extern char ai_Unk7e3f40_[];


int sv_Unk528490();                     /* 0x528490 */
void ai_CheckBuildSites(void);                     /* 0x58fb60 */
void gm_UpdatePlayers(void);                 /* 0x4feb30, f3gm.c */
void gm_InitPlayerRecord(int p);                 /* 0x4feb60, f3gm.c */
void gm_ResetStock(void);                    /* 0x50cc60, f3gm.h: the stock of the type 0x1e stalls to 2 */
int gm_FindMarketObject(int id);                     /* 0x500ec0 */
int gm_ClockSeason(void *t);                     /* 0x4fe350, f3gm.c: the season of the clock (day % 4) */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
int sv_PumpMessages();                     /* 0x52fb80 */
int ls_SendGame(char *name);               /* 0x520a30, ls.c */
int unk_Unk51c950();                     /* 0x51c950 */
void sv_NetworkSyncWait(unsigned fl);           /* 0x52fc10, sv.c */
struct G13MIS;
struct G13MIS *ai_NewRandomMission(int owner, int kind, int x);  /* 0x4b3c10, unk_4b39f0.h */
void ai_ShowMissions(int owner);                /* 0x4b5310, unk_4b39f0.c */
int ls_ReceiveGame(void);                       /* 0x520b80, ls.c */
void cm_SetSpeedLevel(unsigned int v);          /* 0x4c3ef0, cm.h */
int cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2);  /* 0x4c46d0, cm.h */  /* @original: the callers store the returned slot, cm.c defines void */
void d2_SetViewEnabled(int view, int on);       /* 0x475590, d2.h */
int init_SetupNewGame(int n);                   /* 0x511400 */
void init_FillMarkets(int p);                   /* 0x511bc0 */
int main_Unk534040();                     /* 0x534040, main.c range: gm_BuildQuery(1, 0, 6) makes the "Home-Sweet" house and hands it to character c (0x502b60) */
void d3snd_Free(void *snd);             /* 0x4179d0, d3_engine.h */
struct SBSAMPLE * sb_PlaySample(int bank, char *name, int a, int b, int vol, int c, int d);  /* 0x426fa0, f3snd.h */
struct D3FLMAP;
char *tex_FindFile(char *name);                                     /* 0x45f9f0, d3_fl.h */
int d3fl_ReplaceTexture(struct D3FLMAP *map, char *name, int idx);  /* 0x442210, d3_fl.c */
void sb_Stop();                     /* 0x427290 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);   /* 0x446f30, ecmd.c */
void cm_SendExSysMessage(unsigned char id, void *msg);                     /* 0x4c4db0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);  /* 0x4c4830, cm.h */
int init_NameBuildings(void);
extern INIT_OBJ *sv_PanelShown;          /* 0x6f2310, the room object the info panel shows (sv.c's unsigned short * view) */
extern char ai_Time[];  /* 0x770ae0 */
void init_LoadObjectScene(int n);
/* the animation names of a building class (0x6c4 bytes each, in the table 0x61ae08): the class kind, then the
   0x30-byte name slots of which the first 16 are used (ch_PreloadAnims gets 0x10 of them) */
typedef struct INIT_ANIMSET {
    unsigned kind;                  /* 0x00, the building class (INIT_BLD.type) */
    char names[36][0x30];           /* 0x04 .. 0x6c3 */
} INIT_ANIMSET;
extern INIT_ANIMSET init_ClassAnims[19];
extern unsigned char init_Detail;
extern int init_ShopScript;
extern unsigned short ai_PlayerId;              /* 0x6f25b8 */
extern unsigned int d3_LightKeyBudgetVisible;
extern int ai_Unk5fcc6c;
int ai_Unk6f2440 = 0;  /* 0x6f2440 */
int main_MoneyCheat = 0;  /* 0x6f2444, set from the command line switch "MO_MONEY" (main.c); nonzero gives 500000 start money instead of 2000 and the first kind 6 character its starter buildings */
int ai_MapSize = 0;  /* 0x6f4ee8 */
extern int ch_CurrentGroup;                /* 0x5f1294 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);  /* 0x404200, ch.h (varargs) */
void sv_CollectGroupCharacters(char *b);                 /* 0x52b2a0, sv.c: the characters of the group b into 0x7603a0 */
char d3if_Show(struct D3OBJECT *obj, char show);                     /* 0x4471b0 */
void d3if_Prerender(char map);                     /* 0x447860, d3_interface.c */
void sv_UpdateSkyColors();                     /* 0x525940 */
void d3fl_SetDetail(unsigned int n);                     /* 0x43f8a0 */
void d3fl_SetShift(struct D3FLMAP *map, unsigned char shift, char light);   /* 0x446dc0, d3_fl.c */
int sv_Unk52bb70();                     /* 0x52bb70 */
void ch_ClearSceneCallbacks();                     /* 0x40b4d0 */
extern int __mb_cur_max;                /* 0x62e3a0, the CRT locale state */
extern unsigned short *_pctype;         /* 0x62e194, the CRT ctype table */
extern int ai_GfxFloor;                /* 0x6f35c8 */
void init_SoundObjectHook(char *o);
char init_BuildingTextureSetHook(int unused, unsigned char *b, int o);
extern int snd_SfxEnabled;              /* 0x6f257c, sound on */
struct SBSAMPLE; struct SBBANK;
struct SBSAMPLE *sb_GetSample(struct SBBANK *bank, char *name);  /* 0x426030, d3_snd.c: the sample of the name */
int d3snd_Attach(int smp, char *obj, int vol, float r);      /* 0x417710, d3_snd.c: attaches an opened sound to the object */
void d3snd_Play(struct D3SOUND *s, int a, int loops, int delay);  /* 0x417cf0, d3_snd.c */
int _isctype(int c, int mask);
void init_SetDetailLevel(int *s, char f);
int init_StatusForm = 0;  /* 0x75df00, the panel form the load status is printed in */
/* 0x75e5a0, the player profile of the character creation: main_Unk5361d0 reads it from game.INI ([Network]
   Name, Familienname, Wappen, Geschlecht, Glauben) and writes the result of its stage 5, main_Unk53b840
   takes it for the start couple, 0x57bc30 is the default profile of the demo. Layout from those writers and
   from the registration message 0xc (0x4c46d0: short + 0xc, byte + 0xe, int + 0xf, sex + 0x13, faith + 0x14) */
typedef struct INIT_PLAYER {
    unsigned char tool1;            /* 0x00, the first start tool (main_Unk534610) */
    unsigned char tool2;            /* 0x01, the second start tool */
    unsigned char status;           /* 0x02, 0x10 in the default profile, 0x12 when the creation is done */
    unsigned char job;              /* 0x03, the job / title group (model chooser main_Unk534b30's p, gm_TitleGroup's argument) */
    int coat;                       /* 0x04, the coat of arms as an object id (Wappen: 0x59a + the chosen index) */
    unsigned char sex;              /* 0x08, 0 male / 1 female (Geschlecht) */
    unsigned char faith;            /* 0x09, 0 / 1 (Glauben) */
    char firstName[0x20];           /* 0x0a, the first name (Name) */
    char familyName[0x20];          /* 0x2a, the family name (Familienname) */
} INIT_PLAYER;
INIT_PLAYER init_PlayerProfile = {0};  /* 0x75e5a0 */
int ai_Unk7603a0[32] = {0};  /* 0x7603a0 .. INIT_WINDOWS_END: the */
#define INIT_WINDOWS_END (unsigned)&ai_Unk760420       /* one past the scene window table 0x7603a0 (0x20 ints) */
/* ai_NBauplaetze / ai_Bauplaetze: the building sites (rd.h's G15BK; rd.h cannot be included here, gv.h's
   d3sm_GetVertex collides, so the record is declared locally - type debt, see notes/units/f3init.md) */
typedef struct G15BK {
    char name[0x40];                /* 0x00 */
    float x0, y0, z0;               /* 0x40, the minimum corner */
    char pad4c[4];
    float x1, y1, z1;               /* 0x50, the maximum corner */
    char pad5c[4];
} G15BK;
signed char init_BuildingUniverse[0x22] = {0};  /* 0x76fa60, the universe of each building class's scene */
unsigned short * ai_SceneObject = 0;  /* 0x76fa84 */
signed char init_ObjectUniverse[0x2dc] = {0};  /* 0x76faa0, the universe of each object prototype's scene (-1 = not loaded) */
int init_TownSounds[16] = {0};  /* 0x76fd80, the handles of the town's sounds (16 at most), stopped by */
unsigned char * ai_SceneBuilding = 0;  /* 0x76fdc0 */
int ai_NBauplaetze = 0;  /* 0xf812e0, the number of building sites */
G15BK * ai_Bauplaetze = 0;  /* 0xf812e4, the building sites */
extern char ai_GfxDetail;               /* 0x6f35c4 */
int evt_NoGroup = -1;  /* 0x5f8430 */
float d3o_FogScale = 1.0f;  /* 0x5fbd04 */
/* init_SlotCurrency: view of ai_PlayerSlots (tools/data_views.py) */
/* 0x50a810, f3gm.c: the first kind 6 / 7 / 1 / 2 character with a job whose master is m */
struct VFSENTRY * vfs_FindFile(char *path, struct VFSDIR *d, struct VFSDIR **found);  /* 0x42f8f0, sv.c: the path lookup
int ai_Unk454510();                     /* 0x454510 */
int ai_Unk43c570();                     /* 0x43c570 */
int ai_Unk43ca20();                     /* 0x43ca20 */
void amt_InitTable(void);               /* 0x4e5cd0, amt.h */
void gv_ClearGraveyard(void);           /* 0x4f4350, gv.c */
int cm_SendMessage10(unsigned char n, int b, void *p, short d);  /* 0x4c45c0, cm.h */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);  /* 0x45eeb0, d3_sm.h */
void gm_AssignBuilding(unsigned char *b, int place, int own);  /* 0x502b60, f3gm.c */  /* @original: f3gm.c defines `unsigned short place, unsigned own`; the caller below zero-extends the word load (xor/mov cx, no and mask) */
void gm_SyncMarket(unsigned p);         /* 0x4fec10, f3gm.c */
int init_LoadCityINI(char *city, int slot);   /* 0x513a20, the city INI loader, returns 1 */
void ai_MsgNoOp(struct AICHAR *c);                     /* 0x4b70f0 */
int d3t_SceneIndex(struct D3SCENE *scene);                     /* 0x411be0 */
void ch_SetVisible(struct CHARACTER *ch, int show);                     /* 0x402480 */
void ch_PlaceTransport(struct CHARACTER *ch, char now);                     /* 0x403620 */
struct G15PLANT;
void veg_ShowPlants(struct G15PLANT *p);                     /* 0x4bcb90, rd.h: the plants of a building's vegetation map */
void gm_SelectModelVariant(char *o);                     /* 0x4fb5a0, f3gm.c: the model variant of the object o */
int gm_ChooseHeadIcon(unsigned short *c);                     /* 0x4fb890 */
void gm_ShowBuildingObjects(unsigned char *b);                     /* 0x502e30, f3gm.c */
int ai_Unk45f9f0();                     /* 0x45f9f0 */
int ai_Unk442210();                     /* 0x442210 */
void init_AllocSceneBuilding(int o);    /* 0x50d550 (a scene callback, a start Ghidra missed) */
void init_InheritOwner(char *o);        /* 0x50d6a0 (a scene callback, a start Ghidra missed) */
int ai_Unk4c2f30();
void init_LoadSiteScene(char *name);
char init_CollectSiteObject(D3OBJECT *o, void *ctx);
D3OBJECT *d3_SpawnObject(int type, char *name);                     /* 0x43c570, d3_engine.h */
void d3_LinkTop(D3OBJECT *obj);                                     /* 0x43ca20, d3_engine.h */
void d3_MatIdentity(D3MATRIX *m);                                   /* 0x454510, d3_vec.h */

/* ---- the start-of-game setup rules 0x50e0d0 .. 0x50e5a0 (no strings; ROUGH drafts) ---- */

/* ---- the building scenes and the placement of the start objects 0x50dc30 .. 0x5124d0 (ROUGH drafts) ---- */

/* ---- the scene hooks and the local data of the game 0x511600 .. 0x512de0 (ROUGH drafts) ---- */

/* ---- the building scene setup 0x512e40 .. 0x514360 (ROUGH drafts) ---- */

/* ---- the town scene loading 0x5124f0 .. 0x5128d0, 0x510770, 0x510ad0, 0x513050 (ROUGH drafts) ---- */

extern int ai_GfxFog;                /* 0x6f35cc, the detail byte */
extern unsigned char d3_FogByDistance;               /* 0x6334a5 */
extern unsigned char ai_Unk633751;      /* 0x633751 */
extern unsigned int d3fl_DetailScale;   /* 0x633748 */
extern unsigned char d3fl_DetailSizeShift;  /* 0x633751 */
extern int evt_NoGroup;                /* 0x5f8430 */
extern float d3o_FogScale;              /* 0x5fbd04 */


