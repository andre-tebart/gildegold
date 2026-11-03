/* f3init.c -- the game initialisation of the town (0x50d100 ..): tag "f3_init:" ("f3_init:Gebaeude des Typs %s allokiert", 0x622ecc) and "init_" in the error strings ("init_LoadObjektScene:", "init_LoadGebaeudeScene:", "init_LocalData()"). The scene object names ("ob_NAME" / "gb_NAME" with the variant suffixes "_A" .. "_H") are mapped to the object / building prototypes, the object and building scenes "scenes/*ob_%s.ed3" / "scenes/*gb_%s.ed3" are loaded into universes of their own (tables 0x76faa0 per object prototype, 0x76fa60 per building class), the start objects are placed. The .data of this unit starts at 0x61adf8 (right after f3gm's last string 0x61ade0): its initialised globals (0x61adf8 / 0x61ae00 and the table of 19 records of 0x6c4 bytes at 0x61ae08) come first, then its strings from "KONTOR" (0x622e94) on. The unit ends at 0x5142e0, where io_text.c's text builders start. Names are ours.
   Open: type debt - the local views SCENEOBJ / INIT_* and the local canonical prototypes stand in for the types and
   prototypes of ch.h, ai.h, amt.h, d3_sm.h and rd.h, whose headers did not compile into this unit (gv.h conflicts).
   Open: not included yet (dcp headers conflicts f3init HEADER): ai.h amt.h cm.h d2.h f3snd.h gfx.h hmt.h hst.h
   sv.h unk_4b39f0.h unk_4b6480.h; held by own declarations that differ from the owner's: cm_SendExSellObject,
   amt_GetKind, amt_GetOffice, d2_ClearSurface, sv_PanelShown, amt_GetFirstOffice, gm_AssignBuilding.
   unit of rea 8cef7d5b (src/game/f3init.c); boundaries in decl/text.dcl */
struct VFSENTRY;
struct D3OBJECT;
struct D3SURF;
struct VFSDIR;
struct D3SM;
struct D3TERRAIN;
extern char ai_PlayerSlots[4][0x2f4];
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
#include "net.h"                     /* net_Conn: s is -1 (an invalid socket) when no network game runs */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_PlayerRecords[688];
#include "ai_types.h"
extern AISTORE gm_Offers[];            /* 0x7e4b20 */
extern float sv_SkyScale;

/* the scene object view of this unit: a CHARACTER (ch.h, 0x244 bytes, the list 0x1468ae0); the unit cannot
   include ch.h (gv.h's d3sm_GetVertex is a K&R declaration), so the fields this unit reads are declared here
   (type debt, see the lead comment) */
typedef struct SCENEOBJ {
    char pad000[0x28];              /* 0x00 .. 0x27 */
    int script;                     /* 0x28, the id of the event script running for the object (-1 = none; init_LoadShopScript) */
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
    char pad128[0xa4];              /* 0x128 .. 0x1cb */
    void *info;                     /* 0x1cc, D3OBJECT.info: the mesh data (0 until d3_InflateObject loads it) */
    char pad1d0[0x1c];              /* 0x1d0 .. 0x1eb */
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
   the field in use is declared (type debt, see the lead comment) */
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
    char pad06[0xd];                /* 0x06 .. 0x12 */
    unsigned char flags;            /* 0x13, 0x80 / 0x20 = the upgrade effect is due (init_SyncSceneObjects shows it once and clears them) */
    int list;                       /* 0x14, the object list the object holds (ai.h's AIOBJ.unk14 / list; gm_Query finds its goods) */
    struct SCENEOBJ *obj;           /* 0x18, the scene object showing the instance (0 = none) */
    char pad1c[0x27];               /* 0x1c .. 0x42 */
} INIT_OBJ;

/* the building type record (ai.h's AIBTYPE, 0x24d bytes, ai_BTypes) as the part order reads it */
typedef struct INIT_BTYPEREC {
    unsigned char kind;             /* 0x000, the class (init_OpenTown's cls[] index) */
    char pad001[0x21];              /* 0x001 .. 0x021 */
    unsigned char nParts;           /* 0x022, how many of parts[] are used */
    unsigned short parts[0x100];    /* 0x023, the object prototypes of the building's parts in build order (bit 15 a flag) */
    char pad223[0x2a];              /* 0x223 .. 0x24c (the jobs at 0x223, the person prototypes at 0x22f / 0x230) */
} INIT_BTYPEREC;
#pragma pack(pop)


#include "ai_types.h"                /* AITIME */
#pragma pack(push, 1)

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
    AITIME time;                    /* 0x28, the game clock the message was made at (ai.h's AIMSG.time) */
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
   here, so only the fields in use are declared (type debt, see the lead comment) */
typedef struct INIT_CHAR {
    unsigned short id;              /* 0x000, 0xffff = the slot is free */
    unsigned char kind;             /* 0x002 */
    char pad003[0x1];               /* 0x003 */
    unsigned int netId;             /* 0x004, the record's network id (the field block id) */
    unsigned char health;           /* 0x008, ai.h AICHAR.health (100 at creation, > 0 = alive) */
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

/* d3_sm.h's D3TERRAIN as far as this unit reads it (written before the unit included d3_sm.h; the uses can
   read D3TERRAIN now); type debt, see the lead comment */
typedef struct INIT_TERRAIN {
    unsigned int size;              /* 0x000, the cells per side (0x40 / 0x80 select the shadow map size) */
} INIT_TERRAIN;

/* stdlib min / max (the body ai.h defines; f3init does not include ai.h yet: its cm_SendExSellObject with an int
   third parameter, @original, supersedes ai.h's short) */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#include "data_views.h"
#include "d3_snd.h"
#include "sblib.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3snd_AttachI ((int (*)(int, char *, int, float))d3snd_Attach)  /* 0x417710 (d3_snd.h: D3SOUND *(SNDHANDLE *, D3OBJECT *, int, float)): this unit's int handle / char * object view */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */

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
#define INIT_SCENE_HEIGHTMAP_END ((unsigned)init_SceneHeightMaps + 0x40 * 0x3e0) /* one past the height map of the last scene: 0xfd7c50 + 0x40 * 0x3e0 (init_FreeScenes); anchored on the array, not on d3anim_Tail + 0x10 (their linked distance differs from the original's) */
#define INIT_CHARACTER_COUNT 0x300      /* ch.h's CH_MAX: the slots of the character list ch_List (0x1468ae0) */
#define INIT_START_CHARACTERS_END ((unsigned)&ai_Unk770d8c + 0x1fb4) /* one past the 16th character record: 0x770b80 + 16 * 0x21c (init_StartNewGame) */
#define INIT_CHARACTERS_END ((unsigned)ai_Chars + 0x300 * 0x21c)  /* one past the character table ai_Chars (0x770b80 + 0x300 * 0x21c = 0x7d5f80; the original's gm_PlayerRecords follows it there, not in our link) */
#define INIT_LAST_CHARACTER ((unsigned)&ai_Unk770d8c + 0x64fd8)  /* the last character record of ai_Chars (INIT_CHARACTERS_END - 0x21c; the greeting loop walks up to its +2) */
#define INIT_BTYPE(b) ((unsigned char *)ai_BTypes + ((INIT_BLD *)(b))->type * 0x24d)  /* the ai_BTypes record (0x24d bytes) of the building b's type; the original reads ai_BTypes anew at each use but where it keeps the record (cl) */
#define INIT_FIELD_MONEY 0x1ac          /* the character's money field (the field block of the start purse) */
#define INIT_FIELD_RELIGION 0x0c        /* the character's religion field: 0 catholic / 1 protestant (the two judges) */

/* the functions of f3gm.c this unit calls (declared by their addresses there) */
float gm_JobWage();
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
extern char * ai_PlayerBuilding;                /* 0x6f2308 */
extern char * ai_FallbackBuilding;                /* 0x6f230c */
int gm_IsBuildingKind(int n);           /* 0x4fe940, f3gm.c: 1 when the object prototype n is a building part */
int gm_FindBuildingKind(char *name);    /* 0x502060, f3gm.c: the building kind of the name */
unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);  /* 0x503530, f3gm.c: the permission bits of the character p in the building b (or the object o) */


struct D3SCENE;
/* init_SceneDirty: view of d3_Scenes (tools/data_views.py) */
extern unsigned char ai_GameState;      /* 0x6f25ac */
extern int sv_FrameMask;           /* 0x760638 */
extern short d2_ScreenW;  /* 0xfb9072 */
extern short d2_ScreenH;  /* 0xfb9070 */
struct D2FADE;
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */                     /* 0x4767d0 */
void d2_ReleaseFade(struct D2FADE *f); /* 0x476860, d2.h */                     /* 0x476860 */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */

/* the canonical declarations of the engine / other-unit functions the scene loaders call (their headers drag the
   gv.h conflicts in; the types match the canonical prototypes, the CHARACTER arguments are cast through the
   SCENEOBJ view - type debt, see the lead comment) */
struct CHARACTER;
int ch_PlaceOnGround(struct CHARACTER *ch, int force);                           /* 0x404a80, ch.h */
int tb_SetFunctionOff(void (*proc)(void), int off);                              /* 0x42ea10, tb.h */
void d2_ClearScreen(void);                                                       /* 0x46e990, d2.h */
void d2_ShowPicture(int pic);                                                    /* 0x46cfd0, d2.c */
int d2_ClearSurface(struct D2SURF *surf);                                        /* 0x4796c0, gfx.c */
void gm_RandomMarketShake(int unk);                                             /* 0x4ff450, f3gm.c */  /* the original pushes the player at both call sites; the function does not use it */
extern short d2_ScreenW;  /* 0xfb9072, d2.h */
extern short d2_ScreenH;  /* 0xfb9070 */
void sv_SceneLoadProgress(int kind, int cur, int total);                         /* 0x521c10, sv.c */
void sv_UpdateClock(void);                                                       /* 0x5315c0, sv.c */
extern char * cm_FieldObject;                /* 0xa68d2c */
extern struct AICHAR *ai_JudgeCatholic;  /* 0x6f1bfc, the two judges (0x6f1bfc Katholisch, 0x6f1c00 Evangelisch) */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 */
extern int ai_Opponents[];              /* 0x75e160, the difficulty level 0 .. 4 of each opponent (the seven globals ai_Opponents .. opponent_g of the options 0x5d4c70 / 0x5d5640 and of the selection menu 0x537040) */
void cm_BeginSendGroup(void);                     /* 0x4c3b30 */
void cm_EndSendGroup(void);                     /* 0x4c3b40 */
int cm_SendGroupState();                     /* 0x4c3b50 */
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);                     /* 0x4c4650 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
int cm_GetSlotValue(int h);                     /* 0x4c3b10 */
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3c00 */
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);                     /* 0x4c4880, @original: this unit's call sites push the third argument as an int - 0x50f42c zero-extends the unsigned short goods id (`and eax,0xffff`) where cm.h's short view would sign-extend */
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */
int amt_GetOffice(unsigned char i, struct AMTREC *out);  /* 0x4e6f30, amt.c: copies the office i to out (an AMTREC, 0x18 bytes) */
int amt_GetKind(int k, struct AMTKIND *out);              /* 0x4e6f80, amt.c: copies the office kind k to out (an AMTKIND, 0x18 bytes) */
int amt_GetFirstOffice(unsigned int k, struct AMTREC *out);  /* 0x4e6ec0, amt.c: copies the first office of the kind k to out (an AMTREC) */
int amt_SendOfficeOwner(char a, char *b, unsigned char c); /* 0x4e6650, amt.c */
void gm_AgeCharacter(char *c, unsigned short age);    /* 0x4fc6f0, f3gm.c: ages the character c to the age */
int ai_WorksForMaster(struct AICHAR *c);                 /* 0x47dd70, ai.c */
void hmt_Begin(void);                           /* 0x4ad0a0, hmt.c */
extern int sv_PanelFlag;                /* 0x75e8c0 */
void ai_ObjectAngle(void *out, int plot);                 /* 0x58fd90, unk_58bbd0.c: the angle of the object plot to out */
void cm_SendOwnership(int a, int b, int c);               /* 0x4c54a0, cm.c: message 0x38, the building a changes its person from b to c */
void cm_SendBoss(int a, int b);                            /* 0x4c5540, cm.c: message 0x39, the building a gets the character b as its boss */
int cm_RequestCreateGeb(int owner, int kind, D3VECTOR4 *pos, int *rot);  /* 0x4c5ae0, cm.c: message 0x4c, a building of kind k at pos */
unsigned char *gm_NearestFreeBuilding(unsigned char *b, unsigned kind); /* 0x501f60, f3gm.c */
void main_SetLoadProgress(int unused, int pct);          /* 0x537af0, main.c: the loading progress bar */
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);                     /* 0x4c55d0 */
int gv_SendGrave(unsigned char *o);                     /* 0x4f15e0 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);                     /* 0x4c51b0 */
void d2_DrawAll(void);                      /* 0x46e150, d2.c */
unsigned short *gm_CreateObject(int id, int proto, int n);  /* 0x500190, f3gm.c: the new object instance */
int d2_SetForm(int view, int n);                     /* 0x475f70 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
float gm_ItemPrice(int item, unsigned char n);
int gm_ListStock(short *l, int id);               /* 0x50ab50, f3gm.c */
unsigned int d3fl_DetailScale = 0;  /* 0x633748 */
unsigned char d3if_Unk6339db = 0;  /* 0x6339db */
/* init_StatusFormat: view of txt_Ptrs (build/mk_txt_views.py) */
int d2_CutsceneActive = 0;  /* 0x6ed9d4 */
char * ai_KindBChars[8] = {0};  /* 0x6f1c04, the characters of the kind */
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
extern int cm_NetFlags;  /* 0x6f2594 */
unsigned char init_TextureSet = 0;  /* 0x6f1c24 */
int d3_QuadTree = 0;  /* 0x6f1c28 */
int init_TownSoundCount = 0;  /* 0x6f1c34 */
int ai_FrameWait = 0;  /* 0x6f22bc */
int main_StartHouseCheat = 0;  /* 0x6f2430, no writer in the exe: nonzero gives the first kind 6 character its start house ( */
extern char ai_Unk7e3f40_[];


struct AIBLD; void sv_UpdateChimneySmoke(struct AIBLD *b);                     /* 0x528490 */
void ai_CheckBuildSites(void);                     /* 0x58fb60 */
void gm_UpdatePlayers(void);                 /* 0x4feb30, f3gm.c */
void gm_InitPlayerRecord(int p);                 /* 0x4feb60, f3gm.c */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
int sv_PumpMessages();                     /* 0x52fb80 */
void sv_NetworkSyncWait(unsigned fl);           /* 0x52fc10, sv.c */
struct G13MIS;
struct G13MIS *ai_NewRandomMission(int owner, int kind, int x);  /* 0x4b3c10, unk_4b39f0.h */
void ai_ShowMissions(int owner);                /* 0x4b5310, unk_4b39f0.c */
void cm_SetSpeedLevel(unsigned int v);          /* 0x4c3ef0, cm.h */
int cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2);  /* 0x4c46d0, cm.h */  /* @original: the callers store the returned slot, cm.c defines void */
void main_GiveStartBuildings(int p);                     /* 0x534040, main.c range: gm_BuildQuery(1, 0, 6) makes the "Home-Sweet" house and hands it to character c (0x502b60) */
struct D3FLMAP;
int d3fl_ReplaceTexture(struct D3FLMAP *map, char *name, int idx);  /* 0x442210, d3_fl.c */
int cm_SendExSysMessage(unsigned char id, void *msg);                     /* 0x4c4db0 */
extern INIT_OBJ *sv_PanelShown;          /* 0x6f2310, the room object the info panel shows (sv.c's unsigned short * view) */
/* the animation names of a building class (0x6c4 bytes each, in the table 0x61ae08): the class kind, then the
   0x30-byte name slots of which the first 16 are used (ch_PreloadAnims gets 0x10 of them) */
typedef struct INIT_ANIMSET {
    unsigned kind;                  /* 0x00, the building class (INIT_BLD.type) */
    char names[36][0x30];           /* 0x04 .. 0x6c3 */
} INIT_ANIMSET;
extern INIT_ANIMSET init_ClassAnims[19];
int ai_Unk6f2440 = 0;  /* 0x6f2440 */
int main_MoneyCheat = 0;  /* 0x6f2444, set from the command line switch "MO_MONEY" (main.c); nonzero gives 500000 start money instead of 2000 and the first kind 6 character its starter buildings */
#include "anm.h"
#include "rd.h"
int ai_MapSize = 0;  /* 0x6f4ee8 */
void sv_CollectGroupCharacters(char *b);                 /* 0x52b2a0, sv.c: the characters of the group b into 0x7603a0 */
void d3if_Prerender(char map);                     /* 0x447860, d3_interface.c */
void sv_UpdateSkyColors();                     /* 0x525940 */
void d3fl_SetShift(struct D3FLMAP *map, unsigned char shift, char light);   /* 0x446dc0, d3_fl.c */
void sv_Unk52bb70(int id);                     /* 0x52bb70 */
extern int __mb_cur_max;                /* 0x62e3a0, the CRT locale state */
extern unsigned short *_pctype;         /* 0x62e194, the CRT ctype table */
#include "unk_5d33b0_types.h"
#include "ch.h"
#include "f3gm.h"
extern OPTIONBYTES ai_GfxFloor;        /* 0x6f35c8 */
extern int snd_SfxEnabled;              /* 0x6f257c, sound on */
struct SBSAMPLE; struct SBBANK;
int _isctype(int c, int mask);
int init_StatusForm = 0;  /* 0x75df00, the panel form the load status is printed in */
/* 0x75e5a0, the player profile of the character creation: main_ChoosePlayer reads it from game.INI ([Network]
   Name, Familienname, Wappen, Geschlecht, Glauben) and writes the result of its stage 5, main_CreateStartCouple
   takes it for the start couple, 0x57bc30 is the default profile of the demo. Layout from those writers and
   from the registration message 0xc (0x4c46d0: short + 0xc, byte + 0xe, int + 0xf, sex + 0x13, faith + 0x14) */
#include "f3init_types.h"
INIT_PLAYER init_PlayerProfile = {0};  /* 0x75e5a0 */
int ai_SceneChars[32] = {0};  /* 0x7603a0 .. INIT_WINDOWS_END: the characters of the current scene (INIT_CHAR pointers; cm.c 0x4cf000 updates them each tick) */
#define INIT_WINDOWS_END (unsigned)&sv_SkyScale       /* one past the scene window table 0x7603a0 (0x20 ints) */
#include "rd_types.h"                 /* G15BK, the building sites ai_Bauplaetze (rd.h cannot be included here) */
#include "ls.h"
#include "f3init.h"
#include "ahm.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_t.h"
#include "d3_sm.h"
#include "d3_tools.h"
#include "d3_vec.h"
#include "err.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "tb.h"
#include "tr.h"
#include "ts_texture.h"
#include "unk_458810.h"
#include "unk_58bbd0.h"
#include "unk_5d33b0.h"
#include "vfs.h"
signed char init_BuildingUniverse[0x22] = {0};  /* 0x76fa60, the universe of each building class's scene */
unsigned short * ai_SceneObject = 0;  /* 0x76fa84 */
signed char init_ObjectUniverse[0x2dc] = {0};  /* 0x76faa0, the universe of each object prototype's scene (-1 = not loaded) */
int init_TownSounds[16] = {0};  /* 0x76fd80, the handles of the town's sounds (16 at most), stopped by */
unsigned char * ai_SceneBuilding = 0;  /* 0x76fdc0 */
int ai_NBauplaetze = 0;  /* 0xf812e0, the number of building sites */
G15BK * ai_Bauplaetze = 0;  /* 0xf812e4, the building sites */
int evt_NoGroup = -1;  /* 0x5f8430 */
float d3o_FogScale = 1.0f;  /* 0x5fbd04 */
/* init_SlotCurrency: view of ai_PlayerSlots (tools/data_views.py) */
/* 0x50a810, f3gm.c: the first kind 6 / 7 / 1 / 2 character with a job whose master is m */
void amt_InitTable(void);               /* 0x4e5cd0, amt.h */
void gv_ClearCrimes(void);           /* 0x4f4350, gv.c */
int cm_SendMessage10(unsigned char n, int b, void *p, short d);  /* 0x4c45c0, cm.h */
void gm_AssignBuilding(unsigned char *b, int place, int own);  /* 0x502b60, f3gm.c */  /* @original: f3gm.c defines `unsigned short place, unsigned own`; the caller below zero-extends the word load (xor/mov cx, no and mask) */
void gm_SyncMarket(unsigned p);         /* 0x4fec10, f3gm.c */
void ai_MsgNoOp(struct AICHAR *c);                     /* 0x4b70f0 */
struct G15PLANT;
void gm_ShowBuildingObjects(unsigned char *b);                     /* 0x502e30, f3gm.c */
void ai_FavourComment(char *a);

/* ---- the start-of-game setup rules 0x50e0d0 .. 0x50e5a0 (no strings; ROUGH drafts) ---- */

/* ---- the building scenes and the placement of the start objects 0x50dc30 .. 0x5124d0 (ROUGH drafts) ---- */

/* ---- the scene hooks and the local data of the game 0x511600 .. 0x512de0 (ROUGH drafts) ---- */

/* ---- the building scene setup 0x512e40 .. 0x514360 (ROUGH drafts) ---- */

/* ---- the town scene loading 0x5124f0 .. 0x5128d0, 0x510770, 0x510ad0, 0x513050 (ROUGH drafts) ---- */

extern OPTIONBYTES ai_GfxFog;        /* 0x6f35cc, the detail byte */
extern unsigned char d3fl_DetailSizeShift;      /* 0x633751 */
/* 0x50d100, maps the name of the scene object o (copied: "ob_NAME_X" / "gb_NAME") to a prototype: a variant suffix "_A" .. "_H" is cut off, "ob" looks the name up in the object prototypes (the result 0x1000000 | number at SCENEOBJ.proto), "gb" in the building prototypes (0x2000000 | number), a name containing "KONTOR" is the building 0x47 (0x2000047). Returns 1 when found. */

int init_SceneNameToPrototype(char *o)
{
    SCENEOBJ *obj = (SCENEOBJ *)o;
    char head[0x100], rest[0x100], tail[0x80];
    char *u, *t;
    unsigned n;
    int i;

    strcpy(head, o);
    u = strchr(head, '_');
    if (u == 0)
        return 0;
    strcpy(rest, u + 1);
    *u = 0;
    t = strrchr(rest, '_');
    if (t != 0) {
        strcpy(tail, t);
        if (strcmp(tail, "_A") == 0 || strcmp(tail, "_B") == 0 || strcmp(tail, "_C") == 0 || strcmp(tail, "_D") == 0 ||
            strcmp(tail, "_E") == 0 || strcmp(tail, "_F") == 0 || strcmp(tail, "_G") == 0 || strcmp(tail, "_H") == 0)
            *t = 0;
    }
    if (_strcmpi(head, "ob") == 0 || _strcmpi(head + 1, "ob") == 0) {
        n = 0;
        for (i = 0; i < 0xb9dc; i += 0x41) {
            if (_strcmpi(rest, ai_Goods + i + 1) == 0) {
                obj->proto = n | INIT_PROTO_OBJECT;
                return 1;
            }
            n++;
        }
        return 0;
    }
    if (_strcmpi(head, "gb") != 0)
        return 0;
    n = 0;
    for (i = 0; i < 0xca78; i += 0x24d) {
        if (_strcmpi(rest, ai_BTypes + i + 1) == 0) {
            obj->proto = n | INIT_PROTO_BUILDING;
            return 1;
        }
        n++;
    }
    if (strstr(rest, "KONTOR") != 0) {
        obj->proto = INIT_PROTO_BUILDING | INIT_KONTOR_BUILDING;   /* the Kontor building, 0x2000000 | 0x47 */
        return 1;
    }
    return 0;
}
/* 0x50d350, the destroy callback of a scene object: removes its gfx infos and it from the quad tree d3_QuadTree */


void init_SceneObjectDestroyCB(int o)
{
    he_RemoveGfxInfos(o);
    if (d3_QuadTree != 0)
        d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)d3_QuadTree);
}
/* 0x50d380, names the scene object o ("ob_NAME" for kind 1 = an object prototype p, "gb_NAME" for kind 2 = a building p) and hooks it up with p (+0x200 / the building's +0x61) @deep byte; only Q4 is left (8 raw *(T*) accesses). o is a D3OBJECT: +0x218 type / +0x220 kind / +0x224 mask exist in d3.h, but +0x200 (the owner record, here written with the prototype p) is untyped (d3.h: char pad200[0x8]) and the +0x1ec material ops[0] destroy-callback slot has no named field either - both are shared-header decisions (ls.c's 0x51d4f0 escalated the same +0x200 field). p is an AIBLD building record: +0x4 the prototype index, +0x5a the flags, +0x61 the scene object need ai.h, which neither f3init.c nor ls.c can include (gv.h declares ai_Blds / ai_Chars / gm_BuildQuery with types different from ai.h, 30 compile errors). No local view clears the AIBLD accesses. All other rules pass: Q1 init_HookObject (the name cm.h and ls.c already use for 0x50d380), Q2/Q7 cleared by folding the d3if_ShowTree aliases, Q6 described. Tried: the switch form (111 -> 8 code hunks) and the if(*b != 10) polarity (byte). */


void init_HookObject(char *o, unsigned short *p, int kind)
{
    char *b;

    switch (kind) {
    case 2:
        b = ai_BTypes + (unsigned)(unsigned char)*p * 0x24d;
        sprintf(o, "gb_%s", b + 1);
        *(char **)((char *)p + 0x61) = o;
        if (*b != 10) {
            *(int *)(o + 0x220) = 3;
            if (*(int *)(o + 0x218) == 1 && (p[0x2d] & 1) == 0)
                d3if_ShowTree(o, 1);
        } else {
            *(int *)(o + 0x220) = 0;
            *(int *)(o + 0x224) = 0;
        }
        break;
    case 1:
        sprintf(o, "ob_%s", ai_Goods + *p * 0x41 + 1);
        *(int *)(o + 0x220) = 4;
        break;
    }
    init_SceneNameToPrototype(o);
    *(unsigned short **)(o + 0x200) = p;
    if (*(int **)(o + 0x1ec) != 0)
        **(void (***)())(o + 0x1ec) = (void (*)())init_SceneObjectDestroyCB;
}
/* 0x50d460, marks every scene universe as not loaded; returns 0 */


char init_ResetSceneUniverses(void)
{
    memset(init_ObjectUniverse, -1, 0x2dc);
    memset(init_BuildingUniverse, -1, 0x22);
    return 0;
}
/* 0x50d490, frees the loaded scenes of the objects and building classes and the height maps of the 64 scenes */


void init_FreeScenes(void)
{
    int i;
    int *p;

    d3if_SelectScene(0, 0);
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = 0;
    }
    d3if_Reset();
    i = 0;
    do {
        if (init_ObjectUniverse[i] > 0) {
            d3if_SelectScene(init_ObjectUniverse[i], 0);
            d3if_Reset();
            init_ObjectUniverse[i] = -1;
        }
        i++;
    } while (i < 0x2dc);
    i = 0;
    do {
        if (init_BuildingUniverse[i] > 0) {
            d3if_SelectScene(init_BuildingUniverse[i], 0);
            d3if_Reset();
            init_BuildingUniverse[i] = -1;
        }
        i++;
    } while (i < 0x22);
    p = (int *)init_SceneHeightMaps;
    do {
        if (*p != 0) {
            d3sm_Destroy(*p);
            *p = 0;
        }
        p += 0xf8;
    } while ((int)p < (int)INIT_SCENE_HEIGHTMAP_END);
}
/* 0x50d550, the scene object callback (a function start Ghidra missed) that allocates the building for a "gb_NAME" scene object o (0x50d100 resolved the name to the building type, bits 0x2000000 in +0x48): a new instance of the type without an owner (0x501580), for the KONTOR (type 0x47) the office of the player slot whose name is the part after the last '_' of the object name (the slot number goes to +0x65, its name to +5); the object and the building are linked (+0x61 / +0x200) and "f3_init:Gebaeude des Typs %s allokiert" is the (unused) log text. An object without a state of its own takes its parent's record. @deep draft, 90 code hunks / 114 vs 106 instructions, every one read against the original 0x50d550..0x50d6a0: no missing / extra statement, constant or callee. What is left is allocation / scheduling: the original hoists the id (and eax,0xffffff before the tests + movsx of the 0x4b byte, ours compares the byte directly), swaps the loop induction registers (original i=esi / slot pointer=edi, ours the reverse), spills id at [esp+0xc] vs [esp+0x14] and places the inlined strcpy byte loops differently. spell.py: 50d5d3 reload - tried using the masked id expression directly at the sprintf use (90 code hunks unchanged, reverted); 50d6a8 dup-tail - tried the parent-record tail as if (par == 0) return (unchanged, reverted); 50d57d / 50d65c have no menu rule and the meaning is equal. Readability: Q1 init_AllocSceneBuilding, Q2 cleared (gm_CreateBuilding / ai_PlayerSlots / ai_BuildingNames named), Q3 cleared (INIT_PROTO_MASK / INIT_PLAYER_SLOTS_END), Q6 done. Q4 (11 raw accesses) is the same shared-header blocker as init_HookObject and ls.c 0x51d4f0: o's D3OBJECT has no field at 0x200 (d3.h char pad200[0x8]) and the building instance b is an AIBLD from ai.h, which this unit does not include yet (its @original int-parameter cm_SendExSellObject supersedes ai.h's; the gv.h clash this note named once is gone). */


void init_AllocSceneBuilding(int o)
{
    char buf[0x100];
    char tail[0x80];
    unsigned id;
    unsigned char *b;
    int i;
    char *u;
    int par;

    if (init_SceneNameToPrototype((char *)o) != 0) {
        if (*(int *)(o + 0x48) != -1 && *(char *)(o + 0x4b) == 2 && *(int *)(o + 0x218) != 0) {
            id = *(unsigned *)(o + 0x48) & INIT_PROTO_MASK;
            if (id == 0x47) {
                b = (unsigned char *)gm_CreateBuilding(0x47, 0xffff);
                u = strrchr((char *)o, '_') + 1;
                strcpy(tail, u);
                for (i = 1; (int)ai_PlayerSlots[i] < INIT_PLAYER_SLOTS_END; i++) {
                    if (ai_PlayerSlots[i][0] == 0)
                        break;
                    if (_stricmp(tail, ai_PlayerSlots[i]) == 0) {
                        *(int *)(b + 0x65) = i;
                        strcpy((char *)b + 5, ai_PlayerSlots[i]);
                        break;
                    }
                }
            } else
                b = (unsigned char *)gm_CreateBuilding(id, 0xffff);
            if (b != 0) {
                sprintf(buf, "f3_init:Geb\xe4ude des Typs %s allokiert", (char *)ai_BuildingNames[id * 0xe - 2]);
                *(int *)(b + 0x61) = o;
                *(int *)(o + 0x200) = (int)b;
            }
        }
        if (*(int *)(o + 0x218) == 0) {
            par = *(int *)(o + 0x1f8);
            if (par != 0)
                *(int *)(o + 0x200) = *(int *)(par + 0x200);
        }
    }
}
/* 0x50d6a0, the scene object callback (a function start Ghidra missed): an object without an owner record (SCENEOBJ.owner) takes the one of its closest ancestor (SCENEOBJ.parent) that has one */


void init_InheritOwner(char *o)
{
    SCENEOBJ *obj = (SCENEOBJ *)o;
    SCENEOBJ *par, *own;

    own = obj->owner;
    par = obj->parent;
    while (own == 0 && par != 0) {
        own = par->owner;
        obj->owner = own;
        par = par->parent;
    }
}
/* 0x50d6d0, loads the town scene "scenes/*stadt_%s.ed3" (name n) into the current scene; the callback 0x50d730 adjusts its lights. 0 when it does not exist */


char init_LoadTownScene(char *n)
{
    char buf[0x100];

    sprintf(buf, "scenes/*stadt_%s.ed3", n);
    if (d3_LoadScene(buf, 0, 0) == 0)
        return 0;
    d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
    return 1;
}
/* 0x50d730, the callback for the light objects (type 6) of a loaded scene: a light without a third value gets one (the sum of the two others times 0.05) and its colours are scaled (0.8 / 0.5) @deep far, 61 vs 59 instructions, 22 differing; every hunk was read against the original 0x50d730..0x50d7e3 and none is a meaning error: the only differences are the addressing / scheduling forms - q = l + i is built with add vs lea and placed after the fcomp, and the original reloads *(o+0x1e8) between the three float stores (our source reloads it too, VC6 keeps the first load). The o+0x64 block is identical instruction for instruction. spell.py: no hunk matched the menu (50d744 / 50d7ee have no rule), so nothing in the last-bytes pass is left to try. Q4 is all that stays: 22 raw accesses - D3OBJECT +0x218 (typed in d3.h) and +0x5c / +0x60 / +0x64 (d3.h D3OBJECT.unk05c, a D3VECTOR whose three floats this light code reads as colour components), +0x94 (unk090.z) and +0x1e8 (untyped pad1d8, a pointer to a light record whose floats at 0x18 / 0x1c / 0x20 / 0x24 have no type anywhere). Naming them is a shared-header decision (d3.h D3OBJECT / a light record type) of the same kind as init_HookObject's +0x200, and the roles are not evidenced beyond this one callback. Q1 init_AdjustSceneLights (0x50d6d0's comment: it adjusts the lights of a loaded scene), Q2/Q3/Q6 pass, Q5 ROUGH removed. */


void init_AdjustSceneLights(int o)
{
    int i, q;
    int *l;

    if (*(int *)(o + 0x218) == 6) {
        i = 0;
        do {
            q = *(int *)(o + 0x1e8) + i;
            if (*(float *)(q + 0x20) == 0.0f) {
                *(float *)(q + 0x20) = (*(float *)(q + 0x1c) + *(float *)(q + 0x18)) * 0.05f;
                *(float *)(*(int *)(o + 0x1e8) + 0x1c + i) = *(float *)(*(int *)(o + 0x1e8) + 0x1c + i) * 0.8f;
                *(float *)(*(int *)(o + 0x1e8) + 0x24 + i) = *(float *)(*(int *)(o + 0x1e8) + 0x24 + i) * 0.5f;
            }
            i += 0x38;
        } while (i < 0x188);
        if (*(float *)(o + 100) == 0.0f) {
            *(float *)(o + 100) = (*(float *)(o + 0x5c) + *(float *)(o + 0x60)) * 0.05f;
            *(float *)(o + 0x60) = *(float *)(o + 0x60) * 0.8f;
            *(float *)(o + 0x94) = *(float *)(o + 0x94) * 0.5f;
        }
    }
}
/* 0x50d7f0, loads (or reloads) the scene of the object prototype n ("scenes/*ob_%s.ed3") into a free scene slot
   (d3t_FindFreeScene), registers the callbacks 0x50d100 / 0x50d730 on it; for an already loaded slot that is
   flagged dirty (D3SCENE.dirty) it rebuilds the scene and re-places its characters: at their stored position when
   that is the origin, else at the dummy of their transport (or the object's own dummy, "dummy_EINGANG" /
   "dummy_TRANSPORTER_ENTRY" as the fallbacks). While ai_GameState is on and the frame wait is over, a "BLACK"
   fade box covers the load.
   @deep behaviour draft, 181 code hunks, every hunk read; the rest is allocation / scheduling. (1) changed: the original stores a DWORD at [esp+0x10], ours a BYTE at [esp+0x13]; int changed tried twice (214->218 hunks before the polarity fix, 181->198 after; unitdiff 138->136) - the frame shift costs more code hunks than it gains, a frame/slot job. (2) uni*0x3e0: we keep the product in ebx across the d3_LoadScene calls, the original keeps uni in ebx and recomputes shl/sub in the loop; a separate dirty-flag symbol view (0xfd7f75) tried, no change. (3) the outer reload branch's then-block is out of line in the original (the inner if/else polarity swap was kept: 201->181). (4) register roles in the fade block (FrameMask esi vs edi). spell.py: reload 50d824 fixed by !(ai_FrameWait > 0) (214->201); code 50d824/50d8ba = the changed width; dup-tails 50d97c/50db62 = (2)/(1). Readability done: name from the init_LoadObjektScene error string, canonical callees/prototypes, SCENEOBJ.scene/transport, INIT_D3SCENE view, d3_Origin for 0x5ebba4, ch_List walk. */


void init_LoadObjectScene(int n)
{
    int k;
    char *p;
    int uni, save;
    char buf[0x100];
    union {
        D3VECTOR vec;
        char raw[0x10];
    } tmp;
    struct SCENEOBJ **q;
    struct SCENEOBJ *o;
    struct D3OBJECT *d;
    void (*cb)(int, int, int);
    unsigned fl;
    char *bx;
    char changed;

    save = d3_CurrentScene;
    p = ai_Goods + n * 0x41;
    cb = 0;
    changed = 0;
    if (ai_GameState != 0 && !(ai_FrameWait > 0)) {
        fl = sv_FrameMask & 0xfffffff7;
        bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
        k = (unsigned char)*bx;
        while ((k & 4) == 0) {
            sv_RunFrame(0, fl | 0x100080);
            k = (unsigned char)*bx;
        }
        d2_ReleaseFade((struct D2FADE *)bx);
        d2_ClearScreen();
        cb = sv_SceneLoadProgress;
    }
    if ((ai_GameFlags & 8) != 0 && tb_IsFunctionOff(sv_UpdateClock) == 0) {
        tb_SetFunctionOff(sv_UpdateClock, 1);
        changed = 1;
    }
    if (init_ObjectUniverse[n] == -1) {
        uni = d3t_FindFreeScene();
        if (uni == -1)
            err_Message("init_LoadObjektScene:Could not find a free universe...");
        d3if_SelectScene(uni, 0);
        sprintf(buf, "scenes/*ob_%s.ed3", p + 1);
        _strupr(buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            d3if_SelectScene(d3_CurrentScene, 0);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            init_ObjectUniverse[n] = uni;
            d3if_SelectScene(save, 0);
        }
    } else {
        uni = init_ObjectUniverse[n];
        if (d3_Scenes[uni].dirty != 0) {
            d3if_SelectScene(uni, 0);
            sprintf(buf, "scenes/*ob_%s.ed3", p + 1);
            _strupr(buf);
            if (d3_LoadScene(buf, 0, cb) != 0) {
                d3if_SelectScene(d3_CurrentScene, 0);
                d3sm_Destroy(d3_DefaultScene->hmap);
                d3_DefaultScene->hmap = 0;
                ch_GetSceneHMap(d3_DefaultScene);
                d3sm_Build(d3_DefaultScene->hmap, d3_Terrain);
                d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SceneNameToPrototype);
                d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
                /* the original compares the walk pointer as an int (jl): a plain pointer compare would be jb */
                for (q = (struct SCENEOBJ **)ch_List; (int)q < (int)(ch_List + INIT_CHARACTER_COUNT); q++) {
                    o = *q;
                    if (o != 0 && o->scene == &d3_Scenes[uni]) {
                        /* 0x38: the character's stored position, a D3VECTOR the sources do not name yet */
                        if (d3_VecEqual((D3VECTOR *)((char *)o + 0x38), &d3_Origin, 0.5f) != 0) {
                            if ((o->transport != 0 && (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER")) != 0) || (d = ch_FindDummy()) != 0 ||
                                ((d = d3_FindObject(0, 0x100, 0, "dummy_EINGANG")) != 0 || (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY")) != 0)) {
                                /* d + 0x4c: D3OBJECT.pos, the dummy's position */
                                d3_PointToWorld(d, (D3VECTOR *)((char *)d + 0x4c), &tmp.vec);
                                ch_SetPos((struct CHARACTER *)o, &tmp.vec, 1);
                            }
                        } else {
                            ch_SetPos((struct CHARACTER *)o, (D3VECTOR *)((char *)o + 0x38), 1);
                            ch_PlaceOnGround((struct CHARACTER *)o, 1);
                        }
                    }
                }
                d3_Scenes[init_ObjectUniverse[n]].dirty = 0;
                d3if_SelectScene(save, 0);
            }
        }
    }
    if ((ai_GameFlags & 8) != 0 && changed)
        tb_SetFunctionOff(sv_UpdateClock, 0);
}
/* 0x50dba0, the name of the building kind k into out ("FRIEDHOF" for the type 0x1d, "ZIGEUNERLAGER" for 0x1f, else the prototype's name) @deep close (47/47 instructions, 12 differing, 1 code hunk at 50dc18): the only difference is the inline strcpy form - the original copies with inc eax / sub edx,eax / mov cl,[eax] / mov [edx+eax],cl / inc eax / test cl,cl / jne (one running index, the dest base folded into edx), ours with lea ecx,[eax+1] / mov al,[ecx] / inc ecx / mov [edx],al / inc edx / test al,al (two increments). The meaning was read: out gets the record name at ai_BTypes + k*0x24d + 1, the same string, same result 1; the local src was removed and the expression written directly in the call (no change). spell.py: no hunk matched the menu; diffclass: code 12, meaning checked. */


int init_BuildingKindName(int k, char *out)
{
    char t;

    t = ai_BTypes[k + k * 0x24c];
    if (t == 0x1d) {
        strcpy(out, "FRIEDHOF");
        return 1;
    }
    if (t == 0x1f) {
        strcpy(out, "ZIGEUNERLAGER");
        return 1;
    }
    strcpy(out, ai_BTypes + k + k * 0x24c + 1);
    return 1;
}
/* 0x50dc30, loads (or reloads) the scene of the building kind k ("scenes/*gb_%s.ed3", the name
   init_BuildingKindName) into a free scene slot (kept per building class in init_BuildingUniverse); the callbacks
   0x50d100 / 0x50d730 are registered on it. Like init_LoadObjectScene for the object scenes, plus the detail level
   update of the current terrain (init_SetDetailLevel).
   @deep far draft after the base rewrites (195 code hunks), every hunk read; readability done (draft -> readable). Gains kept: ai_FrameWait <= 0 in the new-slot branch (the original's test eax,eax / jle; 220 -> 215 code hunks) and the polarity swap of the position / dummy block (the original's then-block is out of line; 215 -> 195). Tried and reverted: int changed (the dword store at instructions 10 / 21 is the original's, but the frame shift costs more code hunks - third attempt). spell.py menu left: 50dc9d slots [+0x14]/[+0x18] (local frame slots), 50dcd4 / 50dd9e order (register permutation and call-block order), 50ddc3 width (no unsigned short local in the reading; the diff there is the d3if_SelectScene / init_SetDetailLevel call order), 50e070 dup-tail (the walk's loop tail). The rest is allocation / scheduling. */


void init_LoadBuildingScene(int k)
{
    int save, uni, n, o, cls;
    struct D3OBJECT *d;
    char changed;
    char *bx;
    char *slot;
    void (*cb)(int, int, int);
    unsigned fl;
    char buf[0x104], name[0x200], name2[0x80];
    union {
        D3VECTOR vec;
        char raw[0x10];
    } tmp;
    struct SCENEOBJ **q;
    struct SCENEOBJ *co;

    save = d3_CurrentScene;
    cb = 0;
    changed = 0;
    if ((ai_GameFlags & 8) != 0 && tb_IsFunctionOff(sv_UpdateClock) == 0) {
        tb_SetFunctionOff(sv_UpdateClock, 1);
        changed = 1;
    }
    slot = (char *)init_BuildingUniverse + (unsigned char)ai_BTypes[k * 0x24d];
    if (*slot == -1) {
        if (ai_GameState != 0 && (ai_FrameWait <= 0 || sv_PanelFlag != 0)) {
            fl = sv_FrameMask & 0xfffffff7;
            bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
            n = (unsigned char)*bx;
            while ((n & 4) == 0) {
                sv_RunFrame(0, fl | 0x80);
                n = (unsigned char)*bx;
            }
            d2_ClearScreen();
            d2_ReleaseFade((struct D2FADE *)bx);
            cb = sv_SceneLoadProgress;
        }
        uni = d3t_FindFreeScene();
        if (uni == -1)
            err_Message("init_LoadGebaeudeScene:Could not find a free universe...");
        d3if_SelectScene(uni, 0);
        init_BuildingKindName(k, name);
        sprintf(buf, "scenes/*gb_%s.ed3", name);
        _strupr(buf);
        sprintf(name, "init_LoadGebaeudeScene: %s", buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            d3if_SelectScene(uni, 0);
            if (d3_Terrain != 0)
                init_SetDetailLevel((int *)d3_Terrain, 1);
            d3if_SelectScene(d3_CurrentScene, 0);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x160, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            *slot = uni;
            d3if_SelectScene(save, 0);
            goto done;
        }
    } else {
        if (d3_Scenes[*slot].dirty == 0)
            goto done;
        if (ai_GameState != 0 && !(ai_FrameWait > 0)) {
            fl = sv_FrameMask & 0xfffffff7;
            bx = (char *)d2_StartFade(0, 0, (int)d2_ScreenW, (int)d2_ScreenH, "BLACK", 0x32, 1);
            n = (unsigned char)*bx;
            while ((n & 4) == 0) {
                sv_RunFrame(0, fl | 0x80);
                n = (unsigned char)*bx;
            }
            d2_ClearScreen();
            d2_ReleaseFade((struct D2FADE *)bx);
            cb = sv_SceneLoadProgress;
        }
        cls = *slot;
        d3if_SelectScene(cls, 0);
        init_BuildingKindName(k, name2);
        sprintf(buf, "scenes/*gb_%s.ed3", name2);
        _strupr(buf);
        if (d3_LoadScene(buf, 0, cb) != 0) {
            if (d3_Terrain != 0)
                init_SetDetailLevel((int *)d3_Terrain, 1);
            d3if_SelectScene(d3_CurrentScene, 0);
            d3sm_Destroy(d3_DefaultScene->hmap);
            d3_DefaultScene->hmap = 0;
            ch_GetSceneHMap(d3_DefaultScene);
            d3sm_Build(d3_DefaultScene->hmap, d3_Terrain);
            d3_ForEachObject2(d3_DefaultScene, 0, 0x160, init_SceneNameToPrototype);
            d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
            for (q = (struct SCENEOBJ **)ch_List; (int)q < (int)(ch_List + INIT_CHARACTER_COUNT); q++) {
                co = *q;
                if (co != 0 && co->scene == &d3_Scenes[cls]) {
                    if (d3_VecEqual((D3VECTOR *)((char *)co + 0x38), &d3_Origin, 0.5f) != 0) {
                        if ((co->transport != 0 && (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER")) != 0) || (d = ch_FindDummy()) != 0 ||
                            ((d = d3_FindObject(0, 0x100, 0, "dummy_EINGANG")) != 0 || (d = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY")) != 0)) {
                            /* d + 0x4c: D3OBJECT.pos, the dummy's position */
                            d3_PointToWorld(d, (D3VECTOR *)((char *)d + 0x4c), &tmp.vec);
                            ch_SetPos((struct CHARACTER *)co, &tmp.vec, 1);
                        }
                    } else {
                        ch_SetPos((struct CHARACTER *)co, (D3VECTOR *)((char *)co + 0x38), 1);
                    }
                }
            }
            d3_Scenes[*slot].dirty = 0;
        }
    }
    d3if_SelectScene(save, 0);
done:
    if ((ai_GameFlags & 8) != 0 && changed)
        tb_SetFunctionOff(sv_UpdateClock, 0);
}
/* 0x50e0d0, the start of a new game: the first 16 character records are opened in the game - every occupied
   record of the kinds 0 .. 4 gets a start purse (the field INIT_FIELD_MONEY: rand() & 0xffff + 10 through the
   player factor) as a field block of its own. Then the two judges are created (kind 0xc, money 0x12) in one
   send group and fetched back through their send slots as ai_JudgeCatholic / ai_JudgeProtestant, whose
   religion field INIT_FIELD_RELIGION is set to 0 / 1 in a second group; eight characters of the kind 0
   (money rand(8) + 0x10, the 6th argument rand(3) + 4) follow in a third and eight of the kind 0xb (the 7th
   argument 0x13, each one also announced as a trade message from object -2) in a fourth; every group is
   waited for by pumping the queue. The message senders leave their send slot in eax where cm.h's prototypes
   say void, so the calls are made through 8-argument int function pointers. No caller in the exe.
   @deep behaviour draft, 27 code hunks, every hunk read against tools/disasm.py 0x50e0d0 0x160: no missing / extra statement, constant or callee (the 16-record purse loop, the two 8-argument sender calls, the four send groups, the judge religion fields 0 / 1) - the meaning is equal. Left is allocation / scheduling: the original keeps the two judge send slots in esi / edi across the wait group (push edi at 0x50e143, a mid-function live range), ours spills the first to [esp+0x44] and keeps the second in esi. spell.py: 50e10e slots (w1 / w2 frame slots) and 50e1a0 reload - the values are already int locals kept across the calls, no source spelling moved them; 50e143 / 50e16c / 50e1db / 50e244 / 50e32d have no menu rule. Caller: 0x511400 at 0x511423 (the new-game setup). Readability done: Q1 init_StartNewGame, Q2 cleared, Q3 INIT_START_CHARACTERS_END, Q4 INIT_CHAR view, Q5 marker removed, Q6 described. */


void init_StartNewGame(void)
{
    INIT_CHAR *c;
    int i;
    int rec[16];                /* the send slots of the created characters (never read) */
    int m;
    int w1, w2;

    rand();                     /* the first random value of the game is dropped */
    c = (INIT_CHAR *)ai_Chars[0];
    do {
        if (c->id != 0xffff && c->kind < 5) {
            w1 = gm_PlayerFactor((util_RandomU(0x122) & 0xffff) + 10, ai_OwnPlayer);
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValues(4, 1, INIT_FIELD_MONEY, &w1);
            cm_SendFieldDeltas();
        }
        c++;
    } while ((int)c < INIT_START_CHARACTERS_END);
    cm_BeginSendGroup();
    w1 = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xc, -1, -1, 0x12, -1, 0, 0, 0);
    w2 = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xc, -1, -1, 0x12, -1, 0, 0, 0);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    ai_JudgeCatholic = (struct AICHAR *)cm_GetSlotValue(w1);
    ai_JudgeProtestant = (struct AICHAR *)cm_GetSlotValue(w2);
    cm_BeginSendGroup();
    m = 0;
    cm_BeginFieldBlock((char *)ai_JudgeCatholic, ((INIT_CHAR *)ai_JudgeCatholic)->netId);
    cm_AddFieldDelta(1, 1, ((char *)ai_JudgeCatholic - cm_FieldObject) + INIT_FIELD_RELIGION, &m);
    cm_SendFieldDeltas();
    m = 1;
    cm_BeginFieldBlock((char *)ai_JudgeProtestant, ((INIT_CHAR *)ai_JudgeProtestant)->netId);
    cm_AddFieldDelta(1, 1, ((char *)ai_JudgeProtestant - cm_FieldObject) + INIT_FIELD_RELIGION, &m);
    cm_SendFieldDeltas();
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    i = 0;
    do {
        rec[i] = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0, -1, -1, util_RandomU(8) + 0x10, -1, (char)util_RandomU(3) + 4, 0, 0);
        i++;
    } while (i < 8);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    i = 0;
    do {
        rec[i] = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xb, -1, -1, util_RandomU(8) + 0x10, -1, 0, 0x13, 0);
        cm_SendExSellObject(-2, -1, 0x154, 1, ai_OwnPlayer, 0);
        i++;
    } while (i < 8);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
}
/* 0x50e330, creates the n opponents of a new game (the count is the player slots minus the human players, the
   list holds m free object prototypes; its caller 0x511400 (at 0x511547) clears the prototypes the existing characters already
   use): per opponent a prototype prm is taken from the list (an entry of 0 is skipped, the walk starts at a
   random index, steps by 1 or 3 and clears the entry it uses), the opponent's character record is created as
   message 0xb kind 5 (0x4c4650, money 0x10), its difficulty k is ai_Opponents[i] (0 .. 4) and it is paid with the
   difficulty's price tab[k] (500 / 750 / 1000 / 20000 / 40000) through the player factor (0x4c4880), plus a
   random 5000 .. 9999 when 0x6f2440 is set. After the group came through, the five skill bytes at +0x80 of every
   created record are moved by the difficulty's value tab[k + 5] (-31 / -21 / 0 / +42 / +84, the upper clamp
   through a float compare against 252.0f, the lower one through the byte guard) as single field values
   (0x4c3d90) of the message 0x17 block; the difficulty 2 opponents are left alone and every opponent's block is
   sent on its own.
   @deep behaviour draft, 108 differing instructions in 162 code hunks, every hunk read against tools/disasm.py 0x50e330 0x270: the meaning is now equal (the skip walk with its own counter and break, the hoisted step = sw != 0 ? 1 : 3, the two-sided clamp 252.0f / 0, the four send groups). Left is allocation: the original has m in ebp and prm in the stack slot [esp+0x10] (stored and reloaded), ours the reverse, and the tab[] / ids[] arrays sit 0x20 apart from the original's frame slots (swapping the declarations moves nothing); the ids / tab / __ftol temp slots are all shifted by that. Readability: Q1 init_CreateOpponents, Q3 / Q4 / Q5 / Q6 done (INIT_CHAR view, ai_Opponents, the clamp constants); Q2 left on ai_Unk6f2440 alone - its only two readers are this function (the extra random 5000 .. 9999 payment) and main.c's new-game block (0x511c70, the starter buildings of the first kind 6 character); no string, no third reader, so the name has no evidence. */


void init_CreateOpponents(int n, int m, int *list)
{
    int i, j, k, v, id, cnt, idx, step;
    unsigned short r;
    short sw;
    int tab[10];
    int ids[8];
    int prm;
    INIT_CHAR *pc;
    unsigned char b;

    i = 0;
    prm = 0;
    r = util_RandomU(m);
    idx = r;
    sw = util_RandomU(2);
    step = sw != 0 ? 1 : 3;
    tab[0] = 500;
    tab[1] = 0x2ee;
    tab[2] = 1000;
    tab[3] = 20000;
    tab[4] = 40000;
    tab[5] = -31;
    tab[6] = -21;
    tab[7] = 0;
    tab[8] = 0x2a;
    tab[9] = 0x54;
    cm_BeginSendGroup();
    if (0 < n) {
        do {
            if (m != 0) {
                cnt = m;
                do {
                    if (list[idx] != 0) {
                        prm = list[idx];
                        list[idx] = 0;
                        break;
                    }
                    idx = (idx + step) % m;
                } while (--cnt != 0);
            }
            id = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(5, -1, -1, 0x10, -1, prm, 0, 2);
            k = ai_Opponents[i];
            ids[i] = id;
            v = gm_PlayerFactor(tab[k], ai_OwnPlayer);
            cm_SendMoveGoods(-2, -1, ai_OwnPlayer, v);
            if (ai_Unk6f2440 != 0) {
                r = util_RandomU(5000);
                v = gm_PlayerFactor((r & 0xffff) + 5000, ai_OwnPlayer);
                cm_SendMoveGoods(-2, -1, ai_OwnPlayer, v);
            }
            i++;
        } while (i < n);
    }
    cm_EndSendGroup();
    k = cm_SendGroupState();
    while (k == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        k = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    for (i = 0; i < n; i++) {
        pc = (INIT_CHAR *)cm_GetSlotValue(ids[i]);
        if (ai_Opponents[i] != 2) {
            cm_BeginFieldBlock((char *)pc, pc->netId);
            for (j = 0; j < 5; j++) {
                v = pc->skillBytes[j] + tab[ai_Opponents[i] + 5];
                v = (int)(252.0f < v ? 252.0f : v);
                b = v < 0 ? 0 : v;
                cm_AddFieldValue(1, 1, (char *)pc + j - cm_FieldObject + 0x80, &b);
            }
            cm_SendFieldValues();
        }
    }
    cm_EndSendGroup();
    k = cm_SendGroupState();
    while (k == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        k = cm_SendGroupState();
    }
}
/* 0x50e5a0, lays out the n offices of the list ids (the office indexes, a byte each) in a row: every office x
   is the slot table tab[] (24 39 51 63 69 18 57 6 12 30 75 94 88 81 45, a back-and-forth of the 232-wide row)
   minus half the level of the office's kind (amt_GetOffice -> the AMTREC kind at +8, amt_GetKindLevel), written
   to out[i]; the slot index starts at a random one and wraps through the table. Returns the number of wraps
   (the last (s + 1) / 15), i.e. 0 when n <= 15.
   @deep behaviour draft, 3 differing asm lines (2 code hunks + 1 fill) after the whole original 0x50e5a0..0x50e690 was read: the only real difference is the store of the random slot index - the original does a 16-bit store (mov si,ax) then zero-extends once before the loop (and esi,0xffff), ours does mov esi,eax with the same extension (the compiler knows util_Random's unsigned short return). Tried one edit one build each: r/s both unsigned short (the mov matches but the extension moves into the loop, 39 lines), s = util_Random(0xf) with r assigned only in the loop (mov matches, the extension is not hoisted, 19 lines), r = util_Random(0xf) & 0xffff (no change), the call through a (int (*)(unsigned short)) cast of util_Random (no change). The rest is equal, including the frame (rec[0x18] + tab[15]) and the amt_GetOffice byte argument. Readability done: Q1 init_PlaceOffices, Q2 cleared (amt_GetOffice, amt_GetKindLevel), Q3 none, Q4 cleared by the INIT_OFFICE view, Q5 draft marker removed, Q6 described. */


unsigned short init_PlaceOffices(int n, int *out, int *ids)
{
    int tab[15];
    unsigned short r;
    unsigned s;
    int *q;
    int w;
    INIT_OFFICE rec;
    unsigned u;


    tab[0] = 0x18;
    tab[1] = 0x27;
    tab[2] = 0x33;
    tab[3] = 0x3f;
    tab[4] = 0x45;
    tab[5] = 0x12;
    tab[6] = 0x39;
    tab[7] = 6;
    tab[8] = 0xc;
    tab[9] = 0x1e;
    tab[10] = 0x4b;
    tab[11] = 0x5e;
    tab[12] = 0x58;
    tab[13] = 0x51;
    tab[14] = 0x2d;
    r = util_RandomU(0xf);
    s = r;
    if (0 < n) {
        q = out;
        do {
            amt_GetOffice(((unsigned char *)ids)[(char *)q - (char *)out], &rec);
            u = rec.kindWord;
            w = amt_GetKindLevel(u & 0xff);
            *q = tab[s] - w / 2;
            r = (int)(s + 1) / 0xf;
            s = (int)(s + 1) % 0xf;
            q++;
        } while (--n != 0);
    }
    return r;
}
/* 0x50e690, the start population of a new town: the ages t[0..29] (the six groups of 4 / 6 shuffled by
   gm_Shuffle, each plus a random 0 .. 2), the characters already there (the first 12 who ai_WorksForMaster and
   are not kind 5) into f0, the free slots filled through cm_SendCreateCharacter (kind 3, up to three of the kind
   0x10 beyond the 13th), then for each character its age (t[k], through gm_AgeCharacter), job and money (the
   office of ids[k]: amt_GetOffice -> amt_GetKind, the age / money tables t[0x1e..0x27] / t[0x28..0x31]:
   1 3 3 4 4 5 6 7 7 7 and 1000 10000 10000 20000 20000 40000 50000 60000 80000 100000, the money through
   gm_PlayerFactor), the office flags (field 0x1cc) and the first office of the kinds 0x23 .. 0x25, then the
   spouses of the unmarried (cm_SendRelations both ways, a child of kind 8 when the age gap is above 5), and
   eight extra pairs of the kind 3 characters. Sends the field blocks / characters over the network; returns 1.
   @deep read against the whole original 0x50e690..0x50f0a1 and corrected: signed level /3 and int money
   (msg3), loop bounds n>=30/n>=12 and the signed table end, wait loops as while (j != -1 &&
   cm_GetSlotState(j) == 0) (the byte-matched amt_CheckOffices shape), one shared SetField wait (else j =
   -1), the spouse loop as do .. while (u && v) with the job ternary u-first, the 8-pair loop's create
   arguments inline (util_Random(4) + 0xf unmasked), gm_PlayerFactor's first argument inline, the true
   parameter types of amt_SendOfficeOwner (char, char *, unsigned char) and gm_AgeCharacter (unsigned
   short); 458 -> 217 differing, 758 vs 761 instructions, frame equal. Left: allocation and scheduling -
   the constants of the t[] init in other registers (ours ebp=0x10/ecx=0x18/edx=0x19, original ebx/edx/ecx)
   and cnt in ebx vs ebp; the t[] add loop indexed i*4 (inc esi, [esp+esi*4]) where the original keeps a
   byte offset (add esi,4; cmp esi,0x78) - tried a separate loop variable, no change; ids / rec / ob slots
   placed 0x30 apart (lea [esp+0x108] vs [esp+0x138]) - tried the declaration order of the original's
   frame, no change; the office kind level kept in esi across the calls (ours) vs reloaded from [esp+0x110]
   (original); the money bonus product ordered r*(msg3*0.01)*fl (ours) vs (r*fl)*(msg3*0.01) - tried fl
   left and inline, no match; the msg4 = 2 store scheduled before the pushes */


int init_CreateStartCharacters(void)
{
    int t[50];
    int ids[30];
    int out[30];
    INIT_CHAR *f0[30];
    INIT_CHAR *p, *ch, *q, *u, *v;
    INIT_OFFICE rec, ob1, ob2, ob3;
    INIT_KIND kind;
    int cnt, i, n, k, j, h, w;
    unsigned msg0, msg1, msg4;
    int msg3;
    char msg5;
    char c;
    float fl;

    t[0] = 0x10; t[1] = 0x12; t[2] = 0x14; t[3] = 0x18;
    t[4] = 0x10; t[5] = 0x12; t[6] = 0x14; t[7] = 0x18;
    t[8] = 0x10; t[9] = 0x12; t[10] = 0x14; t[11] = 0x18;
    t[12] = 0x16; t[13] = 0x18; t[14] = 0x19; t[15] = 0x1b; t[16] = 0x1d; t[17] = 0x1d;
    t[18] = 0x16; t[19] = 0x18; t[20] = 0x19; t[21] = 0x1b; t[22] = 0x1d; t[23] = 0x1d;
    t[24] = 0x19; t[25] = 0x1b; t[26] = 0x1d; t[27] = 0x20; t[28] = 0x22; t[29] = 0x25;
    cnt = 0;
    gm_Shuffle(4, t);
    gm_Shuffle(4, t + 4);
    gm_Shuffle(4, t + 8);
    gm_Shuffle(6, t + 12);
    gm_Shuffle(6, t + 18);
    gm_Shuffle(6, t + 24);
    memset(f0, 0, sizeof(f0));
    for (i = 0; i < 30; i++)
        t[i] += util_RandomU(3) & 0xffff;
    n = 0;
    p = (INIT_CHAR *)ai_Chars[0];
    do {
        if (n >= 30)
            break;
        if (n >= 12)
            break;
        if (ai_WorksForMaster((char *)p) != 0 && p->kind != 5) {
            f0[n] = p;
            n++;
        }
        p = (INIT_CHAR *)((char *)p + 0x21c);
    } while ((int)p < (int)INIT_CHARACTERS_END);
    for (i = 0; i < 30; i++)
        ids[i] = i;
    gm_Shuffle(4, ids);
    gm_Shuffle(4, ids + 4);
    gm_Shuffle(4, ids + 8);
    gm_Shuffle(6, ids + 12);
    gm_Shuffle(6, ids + 18);
    gm_Shuffle(6, ids + 24);
    init_PlaceOffices(30 - n, out + n, ids + n);
    for (i = n; i < 30; i++) {
        cm_BeginSendGroup();
        if (cnt < 3 && i * 4 > 0x30 && util_RandomU(4) == 0) {
            h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0x10, -1, -1, 0x10, -1, 0, 0, 0);
            cnt++;
        } else {
            h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, 0x10, -1, (unsigned char)out[i], 0, 2);
        }
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
        f0[i] = (INIT_CHAR *)cm_GetSlotValue(h);
    }
    t[0x1e] = 1; t[0x1f] = 3; t[0x20] = 3; t[0x21] = 4; t[0x22] = 4; t[0x23] = 5;
    t[0x24] = 6; t[0x25] = 7; t[0x26] = 7; t[0x27] = 7;
    t[0x28] = 1000; t[0x29] = 10000; t[0x2a] = 10000; t[0x2b] = 20000; t[0x2c] = 20000;
    t[0x2d] = 40000; t[0x2e] = 50000; t[0x2f] = 60000; t[0x30] = 80000; t[0x31] = 100000;
    for (k = 0; k < 30; k++) {
        unsigned short age;

        msg0 = 2;
        amt_GetOffice((unsigned char)ids[k], &rec);
        amt_GetKind(rec.kindWord & 0xff, &kind);
        age = (unsigned short)t[k];
        msg5 = (char)((util_RandomU(0x2a) & 0xffff) + ((int)(kind.levelWord & 0xff) / 3 + 1) * 21.0f);
        msg0 = util_RandomU(2) + t[(kind.levelWord & 0xff) + 0x1e];
        w = t[(kind.levelWord & 0xff) + 0x28];
        msg3 = gm_PlayerFactor(w - (int)(util_RandomF() * w * -0.5f), ai_OwnPlayer);
        ch = f0[k];
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, msg3);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        if (ch->kind == 3) {
            cm_AddFieldDelta(1, 1, 0x84, &msg5);
            if (util_RandomU(2) != 0) {
                fl = util_RandomF() + 1.0f;
                msg4 = (unsigned)(((util_RandomU(8) & 0xffff) + 1) * fl * (msg3 * 0.01f));
                cm_AddFieldDelta(4, 1, 0x1ac, &msg4);
            }
        }
        cm_SendFieldDeltas();
        gm_AgeCharacter((char *)ch, age);
        j = amt_SendOfficeOwner((unsigned char)ids[k], (char *)ch, 0xff);
        while (j != -1 && cm_GetSlotState(j) == 0)
            sv_PumpNet();
        c = ch->officeKind;
        if (c == 0x18 || c == 0x1a || c == 0x1b) {
            j = ((int (*)(void *, int, int, int, int))cm_SendSetField)(ch->netId, 0x1cc, 4, 0x80, 0);
        } else if (c == 0x16 || c == 0x17 || c == 0x19) {
            j = ((int (*)(void *, int, int, int, int))cm_SendSetField)(ch->netId, 0x1cc, 4, 0x40, 0);
        } else {
            j = -1;
        }
        while (j != -1 && cm_GetSlotState(j) == 0)
            sv_PumpNet();
        c = ch->officeKind;
        if (c == 0x1b) {
            amt_GetFirstOffice(0x25, &ob1);
            amt_SendOfficeOwner(ob1.idWord, (char *)ch, 0xff);
        } else if (c == 0x1a) {
            amt_GetFirstOffice(0x24, &ob2);
            amt_SendOfficeOwner(ob2.idWord, (char *)ch, 0xff);
        } else if (c == 0x19) {
            amt_GetFirstOffice(0x23, &ob3);
            amt_SendOfficeOwner(ob3.idWord, (char *)ch, 0xff);
        }
    }
    do {
        u = 0;
        v = 0;
        for (k = 0; k < 30 && !(u && v); k++) {
            q = f0[k];

            if (q->age >= 0x17) {
                if (u == 0 && q->female == 0 && q->partner == -1)
                    u = q;
                else if (v == 0 && q->female == 1 && q->partner == -1)
                    v = q;
            }
        }
        if (u != 0 && v != 0) {
            n = v->age - 0x12;
            cm_BeginFieldBlock((char *)u, u->netId);
            cm_AddFieldDelta(4, 1, 0x5c, &v->netId);
            cm_SendFieldDeltas();
            cm_BeginFieldBlock((char *)v, v->netId);
            cm_AddFieldDelta(4, 1, 0x5c, &u->netId);
            cm_SendFieldDeltas();
            if (n > 5) {
                w = gm_TitleNumber(gm_TitleGroup(util_RandomU(2) != 0 ? u->jobId : v->jobId), 1);
                msg4 = 2;
                j = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(8, u->netId, v->netId, n, -1, w, 0, 2);
                while (j != -1 && cm_GetSlotState(j) == 0)
                    sv_PumpNet();
                ch = (INIT_CHAR *)cm_GetSlotValue(j);
                cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_RandomU(0x400) & 0xffff) + 0x200) * 0x20);
                cm_BeginFieldBlock((char *)ch, ch->netId);
                cm_AddFieldDelta(1, 1, 0xd, &msg4);
                cm_SendFieldDeltas();
            }
            ((int (*)(int, int, int, int))cm_SendRelations)(u->netId, v->netId, (util_RandomU(0x20) & 0xffff) + 0x60, 0);
            j = ((int (*)(int, int, int, int))cm_SendRelations)(v->netId, u->netId, (util_RandomU(0x20) & 0xffff) + 0x60, 0);
            while (j != -1 && cm_GetSlotState(j) == 0)
                sv_PumpNet();
        }
    } while (u != 0 && v != 0);
    for (i = 8; i != 0; i--) {
        msg0 = 2;
        msg1 = (util_RandomU(9) & 0xffff) + 1;
        h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, util_Random(4) + 0xf, -1,
            gm_TitleNumber((util_RandomU(0xc) & 0xffff) + 1, (util_RandomU(2) & 0xffff) + 1), 0, 0);
        while (h != -1 && cm_GetSlotState(h) == 0)
            sv_PumpNet();
        ch = (INIT_CHAR *)cm_GetSlotValue(h);
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_RandomU(0x400) & 0xffff) + 0x200) * 0x20);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        cm_AddFieldDelta(1, 1, 0x167, &msg1);
        cm_SendFieldDeltas();
        h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, util_Random(4) + 0xe, -1,
            gm_TitleNumber((util_RandomU(0xc) & 0xffff) + 1, (util_RandomU(2) & 0xffff) + 1), 0, 1);
        while (h != -1 && cm_GetSlotState(h) == 0)
            sv_PumpNet();
        ch = (INIT_CHAR *)cm_GetSlotValue(h);
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_RandomU(0x400) & 0xffff) + 0x200) * 0x20);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        cm_AddFieldDelta(1, 1, 0x167, &msg1);
        cm_SendFieldDeltas();
    }
    hmt_Begin();
    return 1;
}
/* 0x50f0b0, the opening objects of the buildings of the types 0x20 / 0x1f / 0x1e: for each building the objects 0xff / 0xfe are made (0x500190) and the goods of the type (0x1b7 .. 0x1c0) are put into them (0x4c4880), the first one with a random extra amount */


void init_CreateOpeningObjects(void)
{
    int types[4];
    int *tp;
    int t;
    INIT_BLD *b;
    INIT_OBJ *o1, *o2;
    int i, g, k;
    int goods[8];

    types[0] = 0x20;
    t = 0x20;
    types[1] = 0x1f;
    types[2] = 0x1e;
    types[3] = 0;
    tp = types;
    do {
        b = (INIT_BLD *)gm_BuildQuery(1, 0, t);
        while (b != 0) {
            o1 = (INIT_OBJ *)gm_Query(b->objList, 1, 0, 0xff);
            o2 = (INIT_OBJ *)gm_Query(b->objList, 1, 0, 0xfe);
            if (o1 == 0)
                o1 = (INIT_OBJ *)gm_CreateObject(b->id, 0xff, 1);
            if (o2 == 0)
                o2 = (INIT_OBJ *)gm_CreateObject(b->id, 0xfe, 1);
            for (i = 0; i < 8; i++)
                goods[i] = 0;
            switch (t) {
            case 0x1e:
                goods[1] = 0x1b8;
                goods[0] = 0x1b7;
                goods[2] = 0x1b9;
                break;
            case 0x1f:
                goods[1] = 0x1bb;
                goods[0] = 0x1ba;
                goods[2] = 0x1bc;
                break;
            case 0x20:
                goods[1] = 0x1bf;
                goods[0] = 0x1bd;
                goods[2] = 0x1be;
                goods[3] = 0x1c0;
                break;
            }
            i = 0;
            do {
                g = goods[i];
                if (g != 0) {
                    if (b->type == 0x1e) {
                        cm_SendExSellObject(o1->objId, -1, g, 2, ai_OwnPlayer, 0);
                    } else {
                        cm_SendExSellObject(o1->objId, -1, g, 2 - (i != 0), ai_OwnPlayer, 0);
                        if (i == 0) {
                            cm_SendExSellObject(o2->objId, -1, goods[0], (util_RandomU(0x14) & 0xffff) + 0x14, ai_OwnPlayer, 0);
                        }
                    }
                    switch (g) {
                    case 0x1bd:
                        k = 0x1d0;
                        break;
                    case 0x1bf:
                        k = 0x1d2;
                        break;
                    case 0x1be:
                        k = 0x1d1;
                        break;
                    case 0x1c0:
                        k = 0x1d3;
                        break;
                    case 0x1b7:
                        k = 0x1ca;
                        break;
                    case 0x1b8:
                        k = 0x1cb;
                        break;
                    case 0x1b9:
                        k = 0x1cc;
                        break;
                    case 0x1bb:
                        k = 0x1ce;
                        break;
                    case 0x1bc:
                        k = 0x1cf;
                        break;
                    default:
                        goto next;
                    }
                    cm_SendExSellObject(o1->objId, -1, k, 2 - (i != 0), ai_OwnPlayer, 0);
                }
            next:
                i++;
            } while (i < 8);
            b = (INIT_BLD *)gm_BuildNext();
        }
        tp++;
        t = *tp;
    } while (t != 0);
}
/* 0x50f340, the market stalls of the town: for every market building (prototype 0x47) the two stall objects
   of its object list - 0x1db the sale stall, 0x1dc the purchase stall - get the number of goods in the
   city's import list (0x1db) / export list (0x1dc) (the 16 words at +0x2ac / +0x2cc of the owner city's
   player slot) as their count field, the capacity 4, and both directions of the goods are sent to the
   clients (0x4c4880)
   @deep B3 draft, 203/204 instructions: only 2 code hunks left, the count loops' pointer chain (0x50f39a / 0x50f48d = lea ecx,[eax+eax*2] + lea edi,[ecx*4+base] + mov ecx,0x10; the original uses eax as the lea scratch and puts the counter init first) - allocation, not meaning. All 67 differing instructions were read against the original 0x50f340..0x50f56f: the meaning is equal; the meaning leads found are applied (the third cm_SendExSellObject argument is an int per ai.h - the original zero-extends the unsigned short goods id with and eax,0xffff - 28 -> 12 code hunks) and the rest is regs (edx/ecx for the objId argument, ebp vs ebx for cm_FieldObject in the second loop). spell.py: no hunk matches the menu; the counter-before-pointer spelling was tried, no change. Readability done: Q1 init_StockMarketStalls, Q2 ai_Unk7e41ec -> ai_PlayerSlots + 0x2ac / 0x2cc, Q4 through INIT_BLD.player (0x65) / INIT_OBJ.objId, Q6. */


void init_StockMarketStalls(void)
{
    INIT_BLD *b;
    INIT_OBJ *st;
    int owner, n, i;
    unsigned short *w, *w0;

    b = (INIT_BLD *)gm_BuildQuery(1, 0, INIT_KONTOR_BUILDING);
    while (b != 0) {
        owner = b->player;
        st = (INIT_OBJ *)gm_Query(b->objList, 2, 0, 0x1db, 6);
        if (st != 0) {
            n = 0;
            w0 = (unsigned short *)(ai_PlayerSlots[owner] + 0x2ac);
            w = w0;
            for (i = 0x10; i != 0; i--) {
                if (*w != 0)
                    n++;
                w++;
            }
            cm_BeginFieldBlock((char *)st, st->objId);
            cm_AddFieldDelta(1, 1, (char *)st - cm_FieldObject + 0x1c, &n);
            n = 4;
            cm_AddFieldDelta(4, 1, (char *)st - cm_FieldObject + 0xe, &n);
            cm_SendFieldDeltas();
            for (i = 0x10; i != 0; i--) {
                if (*w0 != 0) {
                    cm_SendExSellObject(st->objId, -1, *w0, 1, ai_OwnPlayer, 0);
                    cm_SendExSellObject(-1, st->objId, *w0, 1, ai_OwnPlayer, 0);
                }
                w0++;
            }
        }
        st = (INIT_OBJ *)gm_Query(b->objList, 2, 0, 0x1dc, 6);
        if (st != 0) {
            n = 0;
            w0 = (unsigned short *)(ai_PlayerSlots[owner] + 0x2cc);
            w = w0;
            for (i = 0x10; i != 0; i--) {
                if (*w != 0)
                    n++;
                w++;
            }
            cm_BeginFieldBlock((char *)st, st->objId);
            cm_AddFieldDelta(1, 1, (char *)st - cm_FieldObject + 0x1c, &n);
            n = 4;
            cm_AddFieldDelta(4, 1, (char *)st - cm_FieldObject + 0xe, &n);
            cm_SendFieldDeltas();
            for (i = 0x10; i != 0; i--) {
                if (*w0 != 0) {
                    cm_SendExSellObject(st->objId, -1, *w0, 1, ai_OwnPlayer, 0);
                    cm_SendExSellObject(-1, st->objId, *w0, 1, ai_OwnPlayer, 0);
                }
                w0++;
            }
        }
        b = (INIT_BLD *)gm_BuildNext();
    }
}
/* 0x50f570, init_Betrieb (the error text "init_Betrieb(): costs are zero" 0x622fe0): stocks a producer
   building b of the classes 1 / 4 (gm_BuildingClass). The first object 0xff of the building that is not
   a kind 0x25 good opens the recipe list: its object list (gm_Query kind 5) is walked while the good's
   prototype name starts with '%'; the four (weight, recipe) pairs at +0x26 / +0x2e of that prototype give the
   cost sum - the price of each recipe (gm_ItemPrice 0x5087f0) times its weight, truncated - and the amount is
   16000 / sum (a float division for the prototype kind 0x15, an integer one otherwise), at least 1. Each
   recipe is then sent into the stall (the object kind 4) in the amount times its weight, capped at the
   stall's stock (gm_ListStock 0x50ab50); 0xffff recipe slots are skipped, a 0 slot ends the list.
   The error text fires when the sum is zero.
   @deep B3 draft, 177/177 instructions, 38 differing: the rest is the sum / i register swap (ours sum=ebx i=ebp, the original sum=ebp i=ebx - spell.py's only menu entry, 50f6a6 regs ebp->ebx ebx->ebp) plus the error tail's block placement (the original's 'costs are zero' tail sits at the function end 0x50f73a behind a je, ours right after the test) and the add/lea fold of the recipe base (50f616 / 50f61a). All hunks read against 0x50f570..0x50f74e: no meaning error left - the two meaning errors found are fixed (the sum weights the price by rw[-4] through fimul, the stall clamp is if (amount < stock) n = amount; else n = stock) and took 152 -> 38 differing instructions. Tried, no change: the local declaration order, the (short*)+0x17 scaled recipe base, the while/for spelling of the recipe walk, -1 vs 0xffff (the latter fixed cmp ax,0xffff). Readability done: Q1 init_Betrieb (the error string 'init_Betrieb(): costs are zero', 0x622fe0, is the function's own), Q2 (gm_BuildingClass -> gm_BuildingClass project-wide, err_Message call gone, gm_ListStock prototype), Q4 (INIT_BLD parameter, INIT_OBJ objId / list), Q6. */


void init_Betrieb(INIT_BLD *b)
{
    int cls, i, sum, n, v;
    char *p;
    INIT_OBJ *o1, *st;
    unsigned short *rw, *rw0;

    cls = gm_BuildingClass(b->type);
    p = ai_BTypes + (unsigned)b->type * 0x24d;
    if (cls != 1 && cls != 4)
        return;
    o1 = (INIT_OBJ *)gm_Query((void *)b->objList, 1, 0, 0xff);
    if (o1 == 0)
        return;
    st = (INIT_OBJ *)gm_Query((void *)o1->list, 1, 5);
    if (st == 0)
        return;
    while (st != 0 && ai_Goods[st->proto * 0x41] == '%')
        st = (INIT_OBJ *)gm_QueryNext();
    if (st == 0)
        return;
    sum = 0;
    i = 0;
    rw0 = (unsigned short *)(ai_Goods + st->proto * 0x41 + 0x2e);
    rw = rw0;
    for (i = 0; i < 4; i++) {
        if (*rw == 0)
            break;
        if (*rw != 0xffff)
            sum += (int)(gm_ItemPrice(*rw, 100) * rw[-4]);
        rw++;
    }
    if (sum == 0) {
        err_Message("init_Betrieb(): costs are zero");
        return;
    }
    if (*p == 0x15)
        n = (int)(16000.0f / sum);
    else
        n = 16000 / sum;
    if (n <= 1)
        n = 1;
    st = (INIT_OBJ *)gm_Query((void *)b->objList, 1, 4, 5);
    if (st == 0)
        return;
    for (i = 0; i < 4; i++) {
        if (*rw0 == 0)
            return;
        if (*rw0 != 0xffff) {
            v = rw0[-4] * n;
            if (v < gm_ListStock((short *)st, *rw0))
                n = v;
            else
                n = gm_ListStock((short *)st, *rw0);
            cm_SendExSellObject(st->objId, -1, *rw0, n, ai_OwnPlayer, 0);
        }
        rw0++;
    }
}
/* 0x50f750, the opening of a new building b (gm_BuildingClass): its owner pays in the kind 0x136 good
   (the prototype kind 9, plus a 500 fee moved to its character through gm_PlayerFactor / cm_SendMoveGoods) or
   the 0x134 good (every other kind except 4 / 0x10 / 0x13); then the building's person is sent
   (cm_SendMessage3d, or a grave for the cemetery kind 0x1d) and - once the send slot is answered
   (cm_RequestChrMoveToUniverse) - placed at its "dummy_EINGANG" entrance object, except for the homes
   (gm_IsHomeType); finally the building is stocked (init_Betrieb)
   @deep B3 draft, 160/174 instructions, 16 differing: ONE code hunk left - the stack cleanup of the entrance message (ours merges add esp,0x10 with the init_Betrieb cleanup into add esp,0x14 after the call, the original restores 0x10 right after cm_RequestChrMoveToUniverse and adds 4 after init_Betrieb); the other 13 differing lines are the alignment fill. Everything else was read against 0x50f750..0x50f902 and fixed (33/432 vs 139/448 differing bytes): p[0x230] is an unsigned char (xor/mov dl), ai_Chars[b->owner][2] is indexed directly (the original folds 0x770b82 into the operand), and the typed views replaced the raw accesses. spell.py: 50f903 dup-tail (the only menu entry) - the two arms / pull-after / polarity prescriptions do not apply: the original HAS two tails (0x50f8d4 and the shared 0x50f8f7) and the else falls through into the shared one, which is exactly what the source writes; no gain from a restructure. Readability done: Q1 init_OpenBuilding, Q2 (cm_RequestChrMoveToUniverse, gv_SendGrave, gm_BuildingMainObject named project-wide), Q4 (INIT_BLD.id / owner, INIT_OBJ.objId, INIT_CHAR.netId), Q6. */


void init_OpenBuilding(INIT_BLD *b)
{
    unsigned char *p;
    char k;
    int h, r, v;
    INIT_CHAR *ch;
    INIT_OBJ *ent;

    gm_BuildingClass(b->type);
    p = (unsigned char *)ai_BTypes + (unsigned)b->type * 0x24d;
    k = *p;
    if (k == 9) {
        cm_SendExSellObject(b->id, -1, 0x136, 1, ai_OwnPlayer, 0);
        v = gm_PlayerFactor(500, ai_OwnPlayer);
        ch = (INIT_CHAR *)ai_Chars[b->owner];
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, v);
    } else if (k != 4 && k != 0x10 && k != 0x13) {
        cm_SendExSellObject(b->id, -1, 0x134, 1, ai_OwnPlayer, 0);
    }
    k = *p;
    if (k == 9)
        goto stock;
    if (k == 4 || k == 0x10 || k == 0x13) {
        if (k == 0x1d)
            goto cemetery;
        if (ai_Chars[b->owner][2] != 6 && ai_Chars[b->owner][2] != 7)
            cm_SendMessage3d((char *)b, 0, p[0x230], 0);
        h = cm_SendMessage3d((char *)b, 0, p[0x230], 0);
    } else {
        if (k != 0x1d)
            h = cm_SendMessage3d((char *)b, 0, p[0x230], 0);
        else
cemetery:
            h = gv_SendGrave((unsigned char *)b);
    }
    r = cm_GetSlotState(h);
    while (r == 0) {
        sv_PumpNet();
        r = cm_GetSlotState(h);
    }
    ch = (INIT_CHAR *)cm_GetSlotValue(h);
    if (gm_IsHomeType((unsigned char *)b) == 0) {
        ent = (INIT_OBJ *)gm_BuildingMainObject((unsigned char *)b);
        if (ent != 0) {
            cm_RequestChrMoveToUniverse(ch->netId, b->id, ent->objId, "dummy_EINGANG");
            init_Betrieb(b);
            return;
        }
    } else {
        cm_RequestChrMoveToUniverse(ch->netId, b->id, -1, "dummy_EINGANG");
    }
stock:
    init_Betrieb(b);
}



extern char *ai_Blds;              /* 0x889f74, the buildings (0xa9 bytes each, 0x100) */
unsigned int d3_LightKeyBudgetCulled = 128;  /* 0x5fcc6c */
unsigned int d3_LightKeyBudgetVisible = 512;  /* 0x5fcc70 */
unsigned char init_Detail = 2;  /* 0x61adf8, the detail level of the scene (4 = high / 2 / 1 = low, a byte) */
float init_SoundVolume = 0.699999988f;  /* 0x61adfc, 0.7f: the volume factor of a started town sound ( */
int init_ShopScript = -1;  /* 0x61ae00, the handle of the shopping script running in the town scene (-1 = none; */
INIT_ANIMSET init_ClassAnims[19] = { { 20, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "arbeit/mist_schaufeln", "arbeit/neufegen2", "arbeit/schweissab", "arbeit/schmieden", "", "", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/einhand_vom_tisch", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/manipulieren_stehend", "gestik/zeigen", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "spezial/auf_tisch_legen", "spezial/meckern", "", "", "", "", "", "", "", "", "" } }, { 5, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/haende_reiben", "arbeit/schreiben", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 7, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "gestik/manipulieren_stehend", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/bein_kratzen", "gestik/zeigen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "arbeit/neufegen2", "", "", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 6, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/aufstehen", "sitzend/hinsetzen", "sitzend/wuerfeln", "spezial/meckern", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 8, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/manipulieren_sitzend", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "spezial/meckern", "", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech3", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech5", "gestik/gespraech6", "gestik/zeigen", "", "", "", "", "", "", "", "", "" } }, { 14, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "spezial/meckern", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "arbeit/umruehren_auftisch", "", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/zeigen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/manipulieren_stehend", "", "", "", "", "", "", "", "", "" } }, { 15, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/steh_noise9", "gestik/durch_haar_fahr", "spezial/einhand_vom_tisch", "arbeit/neufegen2", "arbeit/schweissab", "", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/bein_kratzen", "gestik/steh_noise7", "gestik/steh_noise8", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 16, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "arbeit/schoepfen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 18, { "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/zeigen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech5", "gestik/gespraech6", "gestik/manipulieren_hockend", "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/auf_tisch_legen", "spezial/in_andere_hand_geben", "spezial/runterbeug_such", "spezial/einhaendig_vom_boden_nehmen", "arbeit/schweissab", "arbeit/schmieden", "arbeit/schoepfen", "arbeit/metall_erhitzen", "arbeit/neufegen2", "gestik/manipulieren_stehend", "spezial/meckern", "spezial/einhand_vom_tisch", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise3", "", "", "" } }, { 19, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech1", "sitzend/sitzgespraech2", "sitzend/sitzgespraech3", "sitzend/sitzgespraech4", "sitzend/sitzgespraech5", "sitzend/sitzgespraech6", "sitzend/sitzgespraech7", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/gespraech5", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 9, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "gestik/zeigen", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 13, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "arbeit/schreiben", "arbeit/schoepfen", "", "gestik/zeigen", "kampf/stechen", "kampf/streich", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 21, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/einhand_vom_tisch", "spezial/runterbeug_such", "arbeit/schweissab", "arbeit/schmieden", "arbeit/saegen", "arbeit/hobeln", "arbeit/neufegen2", "", "", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/zeigen", "gestik/kopf_nicken", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/steh_noise7", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/manipulieren_hockend", "spezial/meckern", "spezial/auf_tisch_legen", "", "", "", "", "", "" } }, { 11, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/einhand_vom_tisch", "arbeit/schreiben", "arbeit/schoepfen", "arbeit/schmieden", "arbeit/schweissab", "arbeit/baumfaellen", "gestik/kinn_kratzen", "gestik/kopf_kratzen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/jubeln", "spezial/meckern", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 22, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "gestik/bein_kratzen", "gestik/herranwinken", "gestik/zeigen", "spezial/meckern", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "spezial/auf_tisch_legen", "arbeit/zapfen", "arbeit/umruehren_auftisch", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "sitzend/sitzgespraech1", "sitzend/sitzgespraech2", "sitzend/sitzgespraech3", "sitzend/sitzgespraech4", "sitzend/sitzgespraech5", "sitzend/sitzgespraech6", "sitzend/sitzgespraech7", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/gespraech5", "gestik/gespraech6", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/kinn_kratzen", "gestik/kopf_kratzen" } }, { 1, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "sitzend/hinsetzen", "sitzend/aufstehen", "sitzend/sitz_noise2", "sitzend/sitz_noise3", "sitzend/sitz_noise4", "sitzend/sitz_noise5", "gestik/steh_noise7", "gestik/steh_noise8", "gestik/steh_noise9", "gestik/gespraech1", "gestik/kopf_nicken", "gestik/kopf_schuetteln", "gestik/durch_haar_fahr", "gestik/gespraech5", "gestik/haende_reiben", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 4, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 12, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "spezial/runterbeug_such", "spezial/einhand_vom_tisch", "arbeit/hacken", "arbeit/mist_schaufeln", "arbeit/schweissab", "", "gestik/manipulieren_stehend", "gestik/bein_kratzen", "gestik/manipulieren_hockend", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } }, { 0, { "bewegung/dreh_90_links", "bewegung/dreh_90_rechts", "bewegung/gehen", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" } } };  /* 0x61ae08 .. */
int init_AiTown = 1;  /* 0x624564: .data initial 1, no writer in the exe; nonzero = the opening also stocks the free buildings of the class-1 table and sends the computer players' mail (the AI part of the town) */
extern char ai_MissionMode;        /* 0x624580 */
/* ai_CharInts: view of ai_Chars (tools/data_views.py) */
/* 0x50f910, the opening of a new town's buildings (called by the new-game setup 0x511400; no return
   value, the caller ignores it). For each of up to eight player characters (kind 5 / 6 / 7) the first free building
   whose class lists the character's job (+0x164) is opened (0x50f750); a character without one gets a building of its
   job built from the prototypes (0x58f640 / 0x58f7a0, 0x4c5ae0). The characters are then sent into houses (0x501190 /
   0x501f60 / 0x4c5540 / 0x4c54a0), the buildings of the classes 0xb .. 0xd and 0x10 get their persons and goods, the
   other free buildings whose class is marked 1 in the table below are stocked (cls[]: 0 = nothing, 1 = one or two
   persons, 2 / 3 = special), the start characters (0x50e690) and the opening goods (0x4f8ed0) are made, a message
   0x1b goes to every building (types E / D / F skipped) and the mission-mode message 0x4c out, and finally the
   characters of the kinds 2 .. 7 greet each other (0x4c4b00 both ways).
   @deep read against the whole original 0x50f910..0x51072a and rewritten: the house choice as kind != 5 &&
   != 4 first and q != 0 first (both loops), the ncl bound without the n copy, the building-type record
   re-read from ai_BTypes where the original re-reads it (INIT_BTYPE) and kept in cl where it keeps it,
   gm_PlayerFactor / gm_BuildingKindValue inline in cm_SendMoveGoods, w and the class index as ints, nch =
   0 inside the AiTown branch, the missing cm_SendOwnership of the 0x1b message loop, the 4-argument
   cm_SendRelations of the greetings, the kind-pointer loops with the original's signed ends (ours walked
   one record past the table), the h != -1 wait of the build branch; 745 -> 145 differing, 1157 vs 1150
   instructions. Left: the frame is 0x408 vs 0x404 - one more spill slot, which shifts cls / cnt / pl / ch2
   / list200 by 4 (cmp byte [esp+eax+0x20] vs [esp+eax+0x1c], lea [esp+eax*4+0x80] vs +0x7c) and swaps ncl
   / k between [esp+0x10]/[esp+0x14]/[esp+0x18]; in the greetings the original keeps i at [esp+0x10] and
   tests i+1 < 0x300 with the inner loop ended by the pointer, ours keeps a down-counter 0x300-j (tried the
   pointer do-while with the i+1 guard: VC6 then starts i at 1, 178 differing, reverted to the for form) */
void init_OpenTown(void)
{
    char cls[0x22] = {0, 0, 3, 0, 1, 1, 0, 1, 1, 1, 0, 2, 2, 2, 1, 0, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 3, 3, 1, 0, 1, 1, 0};
    int cnt[0x22];
    int pl[8];
    int ch2[16];
    char vecA[16], vecB[12];
    char buf260[96];
    int list200[128];
    int ncl, i, j, k, r, q, h, n, nch, m, ok;
    char *p, *b, *c;
    unsigned char *cl;                 /* the ai_BTypes record's bytes: the original reads every one zero-extended */
    int bk;                            /* the class byte of the record (*cl) */

    memset(cnt, 0, sizeof(cnt));
    memset(pl, 0, sizeof(pl));
    memset(ch2, 0, sizeof(ch2));
    ncl = 0;
    p = (char *)ai_Chars + 2;
    do {
        if (ncl >= 8)
            break;
        if (((INIT_CHAR *)(p - 2))->id != 0xffff && (*p == 6 || *p == 7 || *p == 5)) {
            pl[ncl] = (int)(p - 2);
            ncl++;
        }
        p += 0x21c;
    } while ((int)p < (int)(INIT_CHARACTERS_END + 2));
    for (i = 0; i < ncl; i++) {
        c = (char *)pl[i];
        if (c == 0 || !(((INIT_CHAR *)c)->kind == 6 || ((INIT_CHAR *)c)->kind == 7 || ((INIT_CHAR *)c)->kind == 5))
            continue;
        ok = 0;
        for (k = 0; k < 0xa900 && !ok; k += 0xa9) {
            b = ai_Blds + k;
            if (((INIT_BLD *)b)->type != 0 && ((INIT_BLD *)b)->owner == 0xffff) {
                cl = INIT_BTYPE(b);
                for (j = 0; j < 6; j++) {
                    if (cl[0x223 + j] != 0 && cl[0x223 + j] == (unsigned char)c[0x164]) {
                        h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)b)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        init_OpenBuilding((INIT_BLD *)b);
                        ok = 1;
                        cnt[(unsigned char)*cl]++;
                        break;
                    }
                }
            }
        }
        if (!ok) {
            int proto = gm_FirstTitleNumber((unsigned char)c[0x164]);

            if (ai_BuildingName(proto, buf260) != 0 && (n = ai_FindBauplaetze(buf260, list200)) != 0) {
                m = 0;
                if (n > 0) {
                    do {
                        if (list200[m] != 0)
                            break;
                        m++;
                    } while (m < n);
                }
                d3_PointToWorld((struct D3OBJECT *)list200[m], (D3VECTOR *)(list200[m] + 0x4c), (D3VECTOR *)vecA);
                ai_ObjectAngle(vecB, list200[m]);
                h = cm_RequestCreateGeb(((INIT_CHAR *)c)->netId, proto, (D3VECTOR4 *)vecA, (int *)vecB);
                while (h != -1 && cm_GetSlotState(h) == 0)
                    sv_PumpNet();
                h = cm_GetSlotValue(h);
                if (h != 0)
                    init_OpenBuilding((INIT_BLD *)h);
            }
        }
    }
    main_SetLoadProgress(0, 0x55);
    for (i = 0; i < ncl; i++) {
        c = (char *)pl[i];
        if (c == 0)
            continue;
        q = gm_BuildQuery(1, 4, ((INIT_CHAR *)c)->id);
        if (q != 0 && gm_IsHomeType(q) != 0)
            q = 0;
        if (((INIT_CHAR *)c)->kind != 5 && ((INIT_CHAR *)c)->kind != 4) {
            if (q != 0) {
                r = gm_NearestFreeBuilding(q, 4);
                if (r == 0)
                    r = gm_NearestFreeBuilding(q, 5);
            } else {
                r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
                if (r == 0)
                    r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
            }
        } else if (q != 0) {
            r = gm_NearestFreeBuilding(q, 5);
            if (r == 0)
                r = gm_NearestFreeBuilding(q, 4);
        } else {
            r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
            if (r == 0)
                r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
        }
        if (r == 0)
            break;
        cm_SendBoss(((INIT_CHAR *)c)->netId, ((INIT_BLD *)r)->id);
        h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)r)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
        while (cm_GetSlotState(h) == 0)
            sv_PumpNet();
    }
    main_SetLoadProgress(0, 0x5a);
    for (i = 0xb; i <= 0xd; i++) {
        b = (char *)gm_BuildQuery(2, 4, 0xffff, 5, i);
        if (b != 0) {
            n = gm_EquipmentValue(b, 100);
            cm_BeginFieldBlock(b, ((INIT_BLD *)b)->id);
            cm_AddFieldDelta(4, 1, 0x65, &n);
            n = n * 2;
            cm_AddFieldDelta(4, 1, 0x69, &n);
            cm_SendFieldDeltas();
            cl = INIT_BTYPE(b);
            cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)ai_PlayerCharC)->netId, ((INIT_CHAR *)ai_PlayerCharC)->netId);
            cm_SendMessage3d(b, 1, INIT_BTYPE(b)[0x230], cl[0x223]);
            cm_SendMessage3d(b, 0, INIT_BTYPE(b)[0x22f], 0);
            cm_SendMessage3d(b, 0, INIT_BTYPE(b)[0x22f], 0);
            cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
            cnt[i]++;
        }
    }
    main_SetLoadProgress(0, 0x5f);
    b = (char *)gm_BuildQuery(2, 4, 0xffff, 5, 0x10);
    if (b != 0) {
        h = cm_SendMessage3d(b, 2, INIT_BTYPE(b)[0x230], 0x2d);
        while (cm_GetSlotState(h) == 0)
            sv_PumpNet();
        h = cm_GetSlotValue(h);
        cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)h)->netId, ((INIT_CHAR *)h)->netId);
        cm_SendMessage3d(b, 0, INIT_BTYPE(b)[0x22f], 0);
        cm_SendMessage3d(b, 0, INIT_BTYPE(b)[0x230], 0);
        cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
        cnt[0x10]++;
    }
    if (init_AiTown != 0) {
        nch = 0;
        for (k = 0; k < 0xa900; k += 0xa9) {
            b = ai_Blds + k;
            if (((INIT_BLD *)b)->type != 0 && ((INIT_BLD *)b)->owner == 0xffff) {
                cl = INIT_BTYPE(b);
                bk = *cl;
                if (cls[bk] == 1 && cnt[bk] < 2) {
                    int w = 0;

                    cm_BeginSendGroup();
                    for (j = 5; j >= 0; j--) {
                        if (cl[j + 0x223] != 0) {
                            w = cl[j + 0x223];
                            break;
                        }
                    }
                    h = cm_SendMessage3d(b, 2, INIT_BTYPE(b)[0x230], w);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    h = cm_GetSlotValue(h);
                    cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)h)->netId, ((INIT_CHAR *)h)->netId);
                    cm_SendMoveGoods(((INIT_CHAR *)h)->netId, -1, ai_OwnPlayer, gm_PlayerFactor(3000, ai_OwnPlayer));
                    if (nch < 0x10) {
                        ch2[nch] = h;
                        nch++;
                    }
                    if (*cl != 9 && *cl != 5) {
                        if (*cl == 4 || *cl == 0x10 || *cl == 0x13) {
                            cm_SendMessage3d(b, 0, cl[0x22f], 0);
                            cm_SendMessage3d(b, 0, cl[0x230], 0);
                        } else {
                            cm_SendMessage3d(b, 0, cl[0x22f], 0);
                        }
                    }
                    cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
                    if (*cl == 5)
                        cm_SendMoveGoods(((INIT_CHAR *)h)->netId, -1, ai_OwnPlayer, gm_PlayerFactor(6000, ai_OwnPlayer));
                    init_Betrieb((INIT_BLD *)b);
                    cm_EndSendGroup();
                    cnt[bk]++;
                }
            }
        }
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
        for (i = 0; i < nch; i++) {
            c = (char *)ch2[i];
            if (c == 0)
                continue;
            q = gm_BuildQuery(1, 4, ((INIT_CHAR *)c)->id);
            if (q != 0 && gm_IsHomeType(q) == 0) {
                r = gm_NearestFreeBuilding(q, 5);
                if (r == 0)
                    r = gm_NearestFreeBuilding(q, 4);
            } else {
                r = gm_BuildQuery(2, 0, 5, 4, 0xffff);
                if (r == 0)
                    r = gm_BuildQuery(2, 0, 4, 4, 0xffff);
            }
            if (r == 0)
                break;
            cm_SendBoss(((INIT_CHAR *)c)->netId, ((INIT_BLD *)r)->id);
            h = ((int (*)(void *, void *, void *))cm_SendOwnership)(((INIT_BLD *)r)->id, ((INIT_CHAR *)c)->netId, ((INIT_CHAR *)c)->netId);
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        for (; i < nch; i++) {
            if (ch2[i] != 0)
                cm_SendMoveGoods(((INIT_CHAR *)ch2[i])->netId, -1, ai_OwnPlayer, gm_BuildingKindValue(4));
        }
    }
    cm_BeginSendGroup();
    for (k = 0; k < 0xa900; k += 0xa9) {
        unsigned short o;

        b = ai_Blds + k;
        cl = INIT_BTYPE(b);
        o = ((INIT_BLD *)b)->owner;
        if (o != 0xffff && (cls[*cl] == 2 || cls[*cl] == 1) && (ai_Chars[o][2] == 5 || ai_Chars[o][2] == 4)) {
            if (init_AiTown != 0) {
                if (*cl != 9 && *cl != 5) {
                    if (*cl == 4 || *cl == 0x10 || *cl == 0x13) {
                        cm_SendMessage3d(b, 0, cl[0x22f], 0);
                        cm_SendMessage3d(b, 0, cl[0x230], 0);
                    } else {
                        cm_SendMessage3d(b, 0, cl[0x22f], 0);
                    }
                }
                cm_SendMessage3d(b, 1, INIT_BTYPE(b)[0x230], INIT_BTYPE(b)[0x223]);
                cm_SendExSellObject(((INIT_BLD *)b)->id, -1, 0x135, 1, ai_OwnPlayer, 0);
                init_Betrieb((INIT_BLD *)b);
            }
        } else if (cls[*cl] == 0 && o == 0xffff) {
            cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)ai_PlayerCharC)->netId, ((INIT_CHAR *)ai_PlayerCharC)->netId);
        }
    }
    cm_EndSendGroup();
    while (cm_SendGroupState() == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
    }
    ((int (*)(int))init_CreateStartCharacters)(0);   /* @original: the original call pushes one argument (0x510494) although the callee reads none */
    ord_UpdateStatistics();
    b = (char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (((INIT_BLD *)b)->masterId == 0xffff && ((INIT_BLD *)b)->type != 'E' && ((INIT_BLD *)b)->type != 'D' && ((INIT_BLD *)b)->type != 'F') {
            INIT_AIMSG m2;

            m2.type = 0x1b;
            m2.to = ((INIT_CHAR *)ai_PlayerCharC)->netId;
            m2.fromId = -1;
            m2.flags = 2;
            m2.arg[0] = ((INIT_BLD *)b)->id;
            m2.arg[1] = gm_EquipmentValue((unsigned char *)b, (char)((INIT_BLD *)b)->level);
            cm_SendOwnership(((INIT_BLD *)b)->id, ((INIT_CHAR *)ai_PlayerCharC)->netId, ((INIT_CHAR *)ai_PlayerCharC)->netId);
            cm_SendLongMessage((char *)&m2);
        }
        b = (char *)gm_BuildNext();
    }
    if (ai_MissionMode == 5) {
        INIT_AIMSG m2;
        AITIME *clock = &ai_Time;

        i = 0;
        p = (char *)ai_Chars + 2;
        do {
            if (*p == 6)
                break;
            p += 0x21c;
            i++;
        } while ((int)p < (int)(INIT_CHARACTERS_END + 2));
        memset(&m2, 0, sizeof(m2));
        m2.type = 0x4c;
        m2.to = ai_CharInts[i][0];
        m2.fromId = ai_CharInts[i][0];
        m2.nIds = 0;
        m2.reserved14 = 0;
        m2.time = *clock;
        m2.flags = 1;
        cm_SendLongMessage((char *)&m2);
    }
    he_PostEvent6d();
    he_PostEvent6b();
    he_PostEvent7a();
    he_PostEvent2b();
    p = (char *)ai_Chars + 2;
    for (i = 0; i < 0x2ff; i++, p += 0x21c) {
        if (*p == 6 || *p == 7 || *p == 5 || *p == 3 || *p == 4 || *p == 2) {
            char *p2;

            m = 0;
            for (j = i + 1, p2 = p + 0x21c; j < 0x300; j++, p2 += 0x21c) {
                if (*p2 == 6 || *p2 == 7 || *p2 == 5 || *p2 == 3 || *p2 == 4 || *p2 == 2) {
                    ((int (*)(int, int, int, int))cm_SendRelations)(((INIT_CHAR *)(p - 2))->netId, ((INIT_CHAR *)(p2 - 2))->netId, (util_RandomU(0x40) & 0xffff) - 0x20, 0);
                    h = ((int (*)(int, int, int, int))cm_SendRelations)(((INIT_CHAR *)(p2 - 2))->netId, ((INIT_CHAR *)(p - 2))->netId, (util_RandomU(0x40) & 0xffff) - 0x20, 0);
                    m += 2;
                    while (h != -1 && m % 10 == 0 && cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                }
            }
        }
    }
}
/* 0x510730, d3_ForEachObject callback of init_LoadSiteScene: keeps the object o when its name contains the
   group key of the list (ctx->key, the site name); returns 0 once 0x200 objects are collected */


char init_CollectSiteObject(D3OBJECT *o, void *ctx)
{
    INIT_SITELIST *list = (INIT_SITELIST *)ctx;

    if (strstr(o->name, list->key) != 0) {
        list->obj[list->n] = o;
        list->n++;
    }
    return list->n < 0x200;
}


/* 0x510770, merges the scene objects of one building site group ("bk_BRUNNEN" .. "bk_STATUEN", the names of the
   caller 0x510ad0): the objects whose name contains the site name are collected (init_CollectSiteObject); a
   site name of its own ("bk_" / "!bk_" prefix) is recorded in the building sites ai_Bauplaetze with the
   bounding box of the 8 corners of its first object (min at +0x40, max at +0x50) unless it is known already;
   then every collected object is replaced by ONE object spawned from the loaded site scene (d3_SpawnObject
   type 2, named like the site) that takes over its parent, position, rotation and children, and the old
   objects are disposed. The site name is the collector key. */
/* 0x510770 */
void init_LoadSiteScene(char *name)
{
    INIT_SITELIST list;
    D3MATRIX cam;
    D3OBJECT *o, *p, *next, *id;
    D3BOXVERT *box;
    float *v;
    int i;

    memset(&list, 0, sizeof list);
    strcpy(list.key, name);
    d3_ForEachObject(d3_DefaultScene, 0, 0x1c0, init_CollectSiteObject, &list);
    if (list.n != 0) {
        o = list.obj[0];
        if (_strnicmp(name, "bk_", 3) == 0 || _strnicmp(name, "!bk_", 4) == 0) {
            i = 0;
            if (0 < ai_NBauplaetze) {
                do {
                    if (_strcmpi(name, ai_Bauplaetze[i].name) == 0) {
                        if (i < ai_NBauplaetze)
                            goto place;
                        break;
                    }
                    i++;
                } while (i < ai_NBauplaetze);
            }
            d3_MatIdentity(&cam);
            box = ((D3MESHOPS *)o->info->mesh)->GetBox(o, o->info, &cam);
            ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, (ai_NBauplaetze * 3 + 3) * 0x20);
            strcpy(ai_Bauplaetze[ai_NBauplaetze].name, name);
            ai_Bauplaetze[ai_NBauplaetze].x0 = box[0].x;
            ai_Bauplaetze[ai_NBauplaetze].y0 = box[0].y;
            ai_Bauplaetze[ai_NBauplaetze].z0 = box[0].z;
            ai_Bauplaetze[ai_NBauplaetze].x1 = box[0].x;
            ai_Bauplaetze[ai_NBauplaetze].y1 = box[0].y;
            ai_Bauplaetze[ai_NBauplaetze].z1 = box[0].z;
            v = (float *)box + 0x16;            /* the 7 remaining corners: 20 floats (0x50) apart */
            i = 7;
            do {
                ai_Bauplaetze[ai_NBauplaetze].x0 = min(v[-2], ai_Bauplaetze[ai_NBauplaetze].x0);
                ai_Bauplaetze[ai_NBauplaetze].y0 = min(v[-1], ai_Bauplaetze[ai_NBauplaetze].y0);
                ai_Bauplaetze[ai_NBauplaetze].z0 = min(v[0], ai_Bauplaetze[ai_NBauplaetze].z0);
                ai_Bauplaetze[ai_NBauplaetze].x1 = max(v[-2], ai_Bauplaetze[ai_NBauplaetze].x1);
                ai_Bauplaetze[ai_NBauplaetze].y1 = max(v[-1], ai_Bauplaetze[ai_NBauplaetze].y1);
                ai_Bauplaetze[ai_NBauplaetze].z1 = max(v[0], ai_Bauplaetze[ai_NBauplaetze].z1);
                v += 0x14;
                i--;
            } while (i != 0);
            ai_NBauplaetze++;
        }
place:
        for (i = 0; i < list.n; i++) {
            o = list.obj[i];
            id = d3_SpawnObject(2, name);
            d3_LinkTop(id);
            d3_SetParent(id, o->parent);
            d3_SetObjectPos(id, &o->pos);
            d3_SetObjectRot(id, &o->rotation);
            p = o->child;
            while (p != 0) {
                next = p->next;
                d3_SetParent(p, id);
                p = next;
            }
            d3if_Delete(o);
        }
    }
}
/* 0x510ad0, initialises the town of the city file `city` (the start of a game): reads the city INI into the
   player slot table (init_LoadCityINI), loads the town scene "scenes/*stadt_%s.ed3" (init_LoadTownScene) and
   returns 1 when it does not exist, empties the offices (amt_InitTable) and the crimes (gv_ClearCrimes),
   allocates the buildings and the site names of the scene objects (init_AllocSceneBuilding /
   init_SceneNameToPrototype), inherits the owners (init_InheritOwner) and merges the 16 building site groups
   (init_LoadSiteScene). Then it creates the bank (character kind 10, named "STADT", 200000000 for every player
   slot in use), the opening objects and stalls (init_CreateOpeningObjects / init_StockMarketStalls), hands the
   type 0x1c buildings to the players (gm_AssignBuilding), updates and initialises the player records
   (gm_UpdatePlayers / gm_InitPlayerRecord) and syncs the market (gm_SyncMarket). At the end the display scene
   is selected again and the shadow map of the terrain (0x40 / 0x80 cells) is built (d3sm_Create) and stored in
   the scene (hmap). Every step waits for the network group. Returns 0. */


int init_InitTown(char *city)
{
    char ok;
    int h, i;
    short *p;
    INIT_CHAR *ch;
    char *pc;
    int bank;

    ai_NextObjectId = 1;
    init_LoadCityINI(city, 0);
    d3if_SelectScene(0, 0);
    ok = init_LoadTownScene(city);
    if (ok == 0)
        return 1;
    amt_InitTable();
    gv_ClearCrimes();
    d3_ForEachObject2(d3_DefaultScene, 0, 0x140, init_AllocSceneBuilding);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, init_SceneNameToPrototype);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_InheritOwner);
    init_LoadSiteScene("bk_BRUNNEN");
    init_LoadSiteScene("bk_GEFAENGNIS");
    init_LoadSiteScene("bk_HANDWERK");
    init_LoadSiteScene("bk_KIRCHE_KL");
    init_LoadSiteScene("bk_KIRCHE_MT");
    init_LoadSiteScene("bk_KIRCHE_GR");
    init_LoadSiteScene("bk_RATHAUS");
    init_LoadSiteScene("bk_STADTWACHE");
    init_LoadSiteScene("bk_WIRTSHAUS_KL");
    init_LoadSiteScene("bk_WIRTSHAUS_GR");
    init_LoadSiteScene("bk_WOHNSITZ_KL");
    init_LoadSiteScene("bk_WOHNSITZ_GR");
    init_LoadSiteScene("bk_ZUNFTHAUS");
    init_LoadSiteScene("bk_RAUBRITTERFESTE");
    init_LoadSiteScene("bk_DOMIZIL");
    init_LoadSiteScene("bk_STATUEN");
    cm_BeginSendGroup();
    cm_SendMessage10(0x44, -1, 0, 0);
    cm_SendMessage10(0x45, -1, 0, 0);
    bank = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(10, -1, -1, 0x12, -1, 0, 0, 1);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    ch = (INIT_CHAR *)cm_GetSlotValue(bank);
    ai_PlayerCharC = (char *)ch;
    strcpy(ch->name, "STADT");
    cm_SendMoveGoods(((INIT_CHAR *)ai_PlayerCharC)->netId, -1, ai_OwnPlayer, 200000000);
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_EndSendGroup();
    i = 1;
    p = init_SlotCurrency;
    do {
        if (*p != 0)
            cm_SendMoveGoods(((INIT_CHAR *)ai_PlayerCharC)->netId, -1, i, 200000000);
        p += 0x17a;
        i++;
    } while ((int)p < (int)INIT_SLOT_CURRENCY_END);
    init_CreateOpeningObjects();
    cm_BeginSendGroup();
    init_StockMarketStalls();
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    h = gm_BuildQuery(1, 5, 0x1c);
    while (h != 0) {
        gm_AssignBuilding(h, ((INIT_CHAR *)ai_PlayerCharC)->id, ((INIT_CHAR *)ai_PlayerCharC)->id);
        h = gm_BuildNext();
    }
    gm_UpdatePlayers();
    i = 0;
    pc = ai_PlayerSlots[0];
    do {
        if (*pc == 0)
            break;
        gm_InitPlayerRecord(i);
        pc += 0x2f4;
        i++;
    } while ((int)pc < (int)INIT_PLAYER_SLOTS_END);
    cm_BeginSendGroup();
    gm_SyncMarket(ai_OwnPlayer);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    d3if_SelectScene(1, 0);
    d3if_SelectScene(0, 0);
    if (((INIT_TERRAIN *)d3_Terrain)->size != 0x80 && ((INIT_TERRAIN *)d3_Terrain)->size == 0x40) {
        d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create(d3_Terrain, 0x80, 2);
        return 0;
    }
    d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create(d3_Terrain, 0x100, 2);
    return 0;
}
/* 0x510e20, prints the title ("$C") and the text t in the form 0x75df00 */


void init_ShowStatus(int t)
{
    d2_SetForm(init_StatusForm, 2);
    io_PrintText("$C");
    io_PrintText(t);
    d2_DrawAll();
}


/* 0x75e5a0, the player profile of the character creation: main_ChoosePlayer reads it from game.INI ([Network]
   Name, Familienname, Wappen, Geschlecht, Glauben) and writes the result of its stage 5, main_CreateStartCouple
   takes it for the start couple, 0x57bc30 is the default profile of the demo. Layout from those writers and
   from the registration message 0xc (0x4c46d0: short + 0xc, byte + 0xe, int + 0xf, sex + 0x13, faith + 0x14) */

/* 0x510e50, the start of a network game for the joining or hosting player (strings "%s/AUGSBURG.NET", "scenes/*network.ed3", "\\game.INI", section "Network"): waits for the flags in 0x6f2594 (1 = host, 4 = ready), loads the city file `name` (default "gamedata/cities/AUGSBURG.NET"), writes the player names to game.INI, registers the player profile (0x4c46d0), sets up the network scene, gives each player slot its start objects (0x4fec10 / 0x4ff450). Returns 1 when the city could not be loaded, 2 when 0x520b80 fails, else 0.
   The remaining difference is the frame (0x310 against the original's 0x384): 0x74 bytes of locals are missing, every local below ini is shifted. @deep frame job */
extern void * main_Instance;          /* 0x75e140 */
extern unsigned ai_MissionSeed;           /* 0x75e120 */
extern char ai_MissionMode;        /* 0x624580 */
extern void *d2_ScreenSurf;                /* 0x6ed924 */

/* 0x510e50 @deep behaviour: 87 code hunks, all read against 0x510e50..0x5112d0. Meaning fixes done: the game.INI literal is \game.INI (10 bytes: the original copies dword+dword+word at 0x510f48..0x510f60; the old 9-byte 'game.INI' gave a byte store), the player profile is the INIT_PLAYER view (status 0x02, job 0x03, coat 0x04 as an int, sex 0x08, faith 0x09, names 0x0a/0x2a: the original loads the dword at 0x75e5a4 and the two bytes at 0x75e5a8/0x75e5a9 separately, so the old unsigned short + >>8 pair was the meaning error; 183 -> 87 code hunks). Left: (1) the frame 0x310 against the original's 0x384 - 0x74 bytes of locals are missing between the scalars and ini (esp+0x24 against esp+0x98), every local below ini and the two returns' add esp shift; no statement in 0x510e50..0x5112d0 reads the missing region, so no source form reaches it. (2) init_StatusFormat's sprintf: the original's strrchr args are cleaned only at 0x510fb9 (add esp,0x34), ours cleans them at the strcpy (add esp,0x8 then 0x2c) - scheduling of the stack cleanup. (3) the start-money select: the original branches (cmp eax,edi / je / push 0x7a120 / jmp / push 0x7d0) where 'main_MoneyCheat == 0 ? 2000 : 500000' compiles neg/sbb/and/add; the if/else-into-a-local spelling was tried too (worse: 165 hunks, reverted) - VC6 constant-select codegen. (4) register allocation of the ai_NewRandomMission / init_SetupNewGame / d3if_SelectScene sites (regs/order hunks). */
int init_StartNetworkGame(char *name)
{
    char c[0x200];
    char ini[0x104];
    unsigned flags;
    unsigned args[2];
    int h, r, i;
    int saved;
    INIT_CHAR *ch;
    char *p;
    short s;

    sprintf(c, (char *)init_StatusFormat, ai_NumPlayers);
    init_ShowStatus((int)c);
    cm_NetFlags = 0;
    do {
        sv_PumpMessages();
        sv_PumpNet();
    } while (cm_NetFlags == 0);
    if ((cm_NetFlags & 1) != 0) {
        if (*name == 0)
            sprintf(name, "%s/AUGSBURG.NET", "gamedata/cities");
        init_ShowStatus(init_StatusSendGame);
        if (ls_SendGame(name) == 0)
            return 1;
    }
    flags = cm_NetFlags & 1;
    init_ShowStatus(init_StatusReceiveGame);
    if (ls_ReceiveGame() == 0)
        return 2;
    GetModuleFileNameA(main_Instance, ini, 0x104);
    p = strrchr(ini, 0x5c);
    strcpy(p, "\\game.INI");
    saved = d3_CurrentScene;
    d2_CutsceneActive = 0;
    d3if_SelectScene(d3t_FindFreeScene(), 0);
    d3_LoadScene("scenes/*network.ed3", 0, 0);
    d2_SetViewEnabled(init_StatusForm, 0);
    d2_CutsceneActive = 1;
    d3if_Reset();
    d3if_SelectScene(saved, 0);
    d2_SetViewEnabled(init_StatusForm, 1);
    WritePrivateProfileStringA("Network", "Name", init_PlayerProfile.firstName, ini);
    WritePrivateProfileStringA("Network", "Familienname", init_PlayerProfile.familyName, ini);
    d2_ClearSurface((struct D2SURF *)d2_ScreenSurf);
    s = util_RandomU(2);
    d2_ShowPicture((s != 1 ? 3 : 0) + 0x78e);
    d2_DrawAll();
    if ((cm_NetFlags & 1) != 0) {
        args[0] = ai_NumPlayers;
        cm_SendExSysMessage(6, args);
    }
    h = cm_SendMessage0c(0xffff, 0xffff, init_PlayerProfile.status, init_PlayerProfile.job, init_PlayerProfile.coat + 0x59a, init_PlayerProfile.sex, init_PlayerProfile.faith, init_PlayerProfile.firstName, init_PlayerProfile.familyName);
    while (h == -1) {
        sv_PumpMessages();
        sv_PumpNet();
        h = cm_SendMessage0c(0xffff, 0xffff, init_PlayerProfile.status, init_PlayerProfile.job, init_PlayerProfile.coat + 0x59a, init_PlayerProfile.sex, init_PlayerProfile.faith, init_PlayerProfile.firstName, init_PlayerProfile.familyName);
    }
    r = cm_GetSlotState(h);
    while (r == 0) {
        sv_PumpMessages();
        sv_PumpNet();
        r = cm_GetSlotState(h);
    }
    ch = (INIT_CHAR *)cm_GetSlotValue(h);
    if ((cm_NetFlags & 1) != 0) {
        ai_MissionMode = -1;
        args[0] = 0xffffffff;
        ai_MissionSeed = timeGetTime();
        args[1] = ai_MissionSeed;
        cm_SendExSysMessage(9, args);
    }
    while (ai_MissionMode == -2) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    if (ai_MissionMode != -1) {
        if (ai_MissionMode != 5)
            ai_MissionSeed = timeGetTime();
        ai_NewRandomMission(ch->netId, (int)ai_MissionMode, 1);
    }
    i = gm_PlayerFactor(main_MoneyCheat == 0 ? 2000 : 500000, ai_OwnPlayer);
    cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, i);
    while ((cm_NetFlags & 4) == 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    if (flags != 0) {
        init_ShowStatus(init_StatusSetupGame);
        init_SetupNewGame(ai_OwnPlayer);
    }
    sv_NetworkSyncWait(0x24087);
    init_ShowStatus(init_StatusLocalData);
    init_LocalData();
    sv_NetworkSyncWait(0x24087);
    if (flags != 0) {
        init_FillMarkets(ai_OwnPlayer);
        i = 0;
        p = ai_PlayerSlots[0];
        do {
            if (*p == 0)
                break;
            gm_SyncMarket(i);
            gm_RandomMarketShake(i);
            p += 0x2f4;
            i++;
        } while ((int)p < INIT_PLAYER_SLOTS_END);
        cm_SetSpeedLevel(2);
    }
    saved = d3_CurrentScene;
    d2_CutsceneActive = 0;
    if (ai_MissionMode > -1 && ai_MissionMode <= 5) {
        d3if_SelectScene(d3t_FindFreeScene(), 0);
        d3_LoadScene("scenes/*network.ed3", 0, 0);
        ai_ShowMissions(ch->netId);
        d3if_Reset();
        d3if_SelectScene(saved, 0);
    }
    return 0;
}
/* 0x5112d0, the start of a network game: the file names "gamedata/network/%s.SAV" / ".SRV" of the game n, the waiting loop for the flags in 0x6f2594, loading the save (0x51c950), the local data (0x511ad0). Returns 1 when the server file could not be read, else 0. ROUGH */


int unk_Unk5112d0(int n)
{
    int r;
    char a[0x104], b[0x104], c[0x200];

    sprintf(a, "gamedata/network/%s.SAV", (char *)n);
    sprintf(c, (char *)init_StatusFormat, ai_NumPlayers);
    init_ShowStatus((int)c);
    cm_NetFlags = 0;
    do {
        sv_PumpMessages();
        sv_PumpNet();
    } while (cm_NetFlags == 0);
    if ((cm_NetFlags & 1) != 0) {
        sprintf(b, "gamedata/network/%s.SRV", (char *)n);
        init_ShowStatus(init_StatusSendGame);
        r = ls_SendGame(b);
        if (r == 0)
            return 1;
        if ((cm_NetFlags & 1) != 0) {
            int args[31];

            args[0] = ai_NumPlayers;
            cm_SendExSysMessage(6, args);
        }
    }
    while ((cm_NetFlags & 4) == 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    ls_LoadGame(a, 5);
    sv_NetworkSyncWait(0x24087);
    init_ShowStatus(init_StatusLocalData);
    init_LocalData();
    d2_CutsceneActive = 0;
    return 0;
}
/* 0x511400, the setup of a new game for the player slot n: seeds the random generator once (timeGetTime; the
   only place that seeds it, so every new game starts the same), init_StartNewGame (0x50e0d0), with the cheat
   flag 0x6f2430 the start house of the first kind 6 character (0x534040), hands out the opponent slots for the
   players the city supports beyond the first (init_CreateOpponents with a table of eight job kinds
   0x18 0x27 0x33 0x3f 0x45 0x4b 0x12 0x06, without the kinds the persons already have; with the flag 0x80
   of ai_GameFlags only the kind 0x12), init_OpenTown (0x50f910), and gives the first building of the class 1 the
   480000 start money. Returns 1 */


int init_SetupNewGame(int n)
{
    unsigned long t;
    char *p;
    int i, d;
    unsigned tab[8];
    unsigned *q;
    INIT_OBJ *o;
    INIT_BLD *b;
    int *pi;

    if (net_Conn.s == -1) {
        t = timeGetTime();
        srand(t);
    }
    ((void (*)(int))init_StartNewGame)(0);   /* @original: the original call pushes one argument (0x511422) although the callee reads none */
    if (main_StartHouseCheat != 0) {
        i = 0;
        p = ai_Chars[0] + 2;
        do {
            if (*p == 6) {
                main_GiveStartBuildings(i);
                break;
            }
            p += 0x21c;
            i++;
        } while ((int)p < (int)(INIT_CHARACTERS_END + 2));
    }
    if ((int)ai_PlayerSlots[n][0x48] - ai_NumPlayers > 0) {
        tab[0] = 0x18;
        tab[1] = 0x27;
        tab[2] = 0x33;
        tab[3] = 0x3f;
        tab[4] = 0x45;
        tab[5] = 0x4b;
        tab[6] = 0x12;
        tab[7] = 6;
        if (ai_GameFlags & 0x80) {
            D3_ZERO(tab, sizeof(tab));
            tab[0] = 0x12;
        } else {
            p = ai_Chars[0] + 2;
            do {
                if (*p == 6 || *p == 7) {
                    i = 0;
                    do {
                        if ((unsigned char)p[0x162] == tab[i]) {
                            tab[i] = 0;
                            break;
                        }
                        i++;
                    } while (i < 8);
                }
                p += 0x21c;
            } while ((int)p < (int)(INIT_CHARACTERS_END + 2));
        }
        init_CreateOpponents((int)ai_PlayerSlots[n][0x48] - ai_NumPlayers, 8, tab);
    }
    init_OpenTown();
    b = (INIT_BLD *)gm_FirstBuilding(1);
    if (b != 0) {
        o = (INIT_OBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
        if (o != 0)
            cm_SendMoveGoodsUnchecked(o->objId, -1, ai_OwnPlayer, 480000);
    }
    return 1;
}
/* 0x5115b0, collects the name s into the list r (count at r + 0x80, at most 0x20) when it contains "ub_FACKEL_" */


char unk_Unk5115b0(int s, int r)
{
    if (strstr((char *)s, "ub_FACKEL_") != 0) {
        *(int *)(r + *(int *)(r + 0x80) * 4) = s;
        *(int *)(r + 0x80) = *(int *)(r + 0x80) + 1;
    }
    return *(int *)(r + 0x80) < 0x20;
}
/* 0x511600, in the scene s: for every "ub_FACKEL_" object (0x5115b0 collects them, up to 0x20) the children whose kind is above 4 and whose name contains "rLicht" are handed on (0x449960), then the object is released (0x447190) */


void init_StripTorches(int s)
{
    int list[33];
    int i, c, next;

    i = 0;
    list[32] = 0;
    d3_ForEachObject(d3_DefaultScene, s, 0x2c0, unk_Unk5115b0, list);
    if (0 < list[32]) {
        do {
            c = *(int *)(list[i] + 0x1fc);
            while (c != 0) {
                next = *(int *)(c + 0x1f0);
                if (*(int *)(c + 0x218) >= 5 && strstr((char *)c, "rLicht") != 0)
                    d3_SetParent(c, s);
                c = next;
            }
            d3if_Delete((struct D3OBJECT *)list[i]);
            i++;
        } while (i < list[32]);
    }
}
/* 0x5116b0, collects the torch objects of the scene (0x511600) and re-creates the quad tree 0x6f1c28 */


void unk_Unk5116b0(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_StripTorches);
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = (int)d3q_Create(0, 0x40, 8, 7);
    }
    d3_LightInvalidate(1);
}
/* 0x511700, the scene object callback of the town scene (0x511790 hooks every scene object with it): a
   "gb_" building object (kind byte 2 at +0x4b) gets the flag 4 in +0x212 (bit 8 cleared; the bits 4 and 8
   for the building type 0x1f), objects with "_TOR_" in the name are left alone; returns 1
   @deep behaviour: 47 code hunks, meaning equal - the whole original 0x511700..0x51178d was read. Meaning fixes: the return type is char (the original sets only al at 0x511763 / 0x511789, it leaves eax dirty; int gave 50 code hunks / 89 differing bytes, char 47 / 68), Q4 through SCENEOBJ.proto (0x48) instead of a raw cast, Q3 through INIT_PROTO_MASK. Left (allocation / layout): the two flag blocks are swapped - the original keeps the strstr("_TOR_") arm inline at 0x511741 and jumps to the building arm at 0x511767, ours emits the building arm inline and jumps to the strstr arm; the p test's branch polarity (je vs jne) follows. Register allocation inside the arms: the original compares *p in memory (cmp BYTE [eax],0x1f) and keeps the flag in cl with al = 1 set before the store, ours loads *p into cl and the flag into al and sets al after. Spellings tried, all identical code: the early 'if (strncmp != 0) return 1' shape, the 'if (o[4b] != 2 || (p = ...) == 0)' inverted shape, and the if/else shape with both arms returning. */


char init_TownBuildingHook(char *o)
{
    SCENEOBJ *so;
    char *p;

    so = (SCENEOBJ *)o;
    if (strncmp(o, "gb_", 3) != 0)
        return 1;
    if (o[0x4b] != 2 || (p = ai_BTypes + (so->proto & INIT_PROTO_MASK) * 0x24d) == 0) {
        if (strstr(o, "_TOR_") != 0)
            return 1;
        o[0x212] = (o[0x212] & 0xf7) | 4;
        return 1;
    }
    if (*p != 0x1f) {
        o[0x212] = (o[0x212] & 0xf7) | 4;
        return 1;
    }
    o[0x212] = o[0x212] | 0xc;
    return 1;
}
/* 0x511790, prepares the town scene: the flag 4 on every building scene object of the class 6, the hook 0x511700 on all "gb_" objects, the torches (0x5116b0), and the scene's tree is rebuilt */


void unk_Unk511790(void)
{
    char *b, *o;

    b = (char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        o = *(char **)(b + 0x61);
        if (o != 0) {
            o[0x212] = (o[0x212] & 0xf7) | 4;
            sv_UpdateChimneySmoke(b);
        }
        b = (char *)gm_BuildNext();
    }
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, init_TownBuildingHook, 0);
    unk_Unk5116b0();
    ai_CheckBuildSites();
    d3sm_Build(*(int *)((char *)d3_DefaultScene + 0xb0), d3_Terrain);
}
/* 0x511810, scene object callback: an object of the type 0 / 1 / 3 / 4 (+0x220) gets the flag 4 in +0x213 */


int ai_Unk511810(char *o)
{
    int t;

    t = *(int *)(o + 0x220);
    if (t == 3 || t == 4 || t == 1 || t == 0)
        o[0x213] |= 4;
    return t;
}
/* 0x511840, frees and re-creates the quad tree 0x6f1c28 from the scene's objects (0x511810) */

void unk_Unk511840(void)
{
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = 0;
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, ai_Unk511810);
    d3_QuadTree = (int)d3q_Create(0, 0x40, 8, 7);
}
/* 0x511890, gives every building of the class 6 (not the type 0x1c) a name: the names of its kind (the 0xc names at 0x143b400 + 0x38 per kind) that no building uses yet, up to 0x20 characters ("Building name too long: %s" is the error text), one picked at random into the building's name field (+5) @deep behaviour: 178 code hunks / 404 of 544 bytes, meaning read whole against the original 0x511890..0x511ad0 (both gm_BuildQuery(1,6) walks, the 0xc x 0xe name table walk, the 0x100 x 0xa9 ai_Blds compare, the 0x20 length check, the random pick, the two error formats) - no meaning difference left; the frame now matches 0x810 exactly. Frame finding of this session: the original has TWO 0x200-byte error buffers, one per "Building name too long" format (0x120..0x320 for the arm that calls err_Message, 0x620..0x810 for the arm that drops the text); our single err[0x200] left the frame 0x610 and every buffer 0x200 too low. After adding the second (char text[0x200]) the anchors match: one at 0x20, names at 0x320, the sprintf/err_Message pair at 0x124/0x12c, the dead format at 0x620. Left (allocation only): the original pushes ebx/ebp/edi mid-function (at 0x5118ee, the second walk) where ours pushes them in the prologue, the first walk keeps cnt/zero in esi vs the original's [esp+0xc] + test eax, and the register assignment of the strcpy/strlen blocks (109 +/- lines, every one read). Also tried and reverted: util_Random((unsigned char)cnt) - the original zero-extends the count with xor dx,dx / mov dl,al (a char parameter in its TU), our cast gives and eax,0xff and 179 code hunks, plain cnt gives 178. */


int init_NameBuildings(void)
{
    unsigned char *b;
    char names[0xc][0x40];
    int cnt, i, j, k;
    char *src, *u;
    char one[0x100];
    char err[0x200];            /* the "Building name too long" text handed to err_Message */
    char text[0x200];           /* the same text of the first (dead) check: the original formats it and drops it */
    unsigned r;
    char *bl;

    b = (unsigned char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (ai_BTypes[*b * 0x24d] != 0x1c)
            b[5] = ch_EmptyName[0];
        b = (unsigned char *)gm_BuildNext();
    }
    b = (unsigned char *)gm_BuildQuery(1, 6);
    while (1) {
        if (b == 0)
            return 0;
        if (ai_BTypes[*b * 0x24d] != 0x1c) {
            cnt = 0;
            b[5] = ch_EmptyName[0];
            for (i = 0; i < 0xc; i++) {
                src = ai_BuildingNames[i + *b * 0xe];
                if (*src != 0) {
                    strcpy(one, src);
                    bl = ai_Blds;
                    for (j = 0; j < 0x100; j++, bl += 0xa9) {
                        if (*bl != 0 && strcmp(bl + 5, one) == 0)
                            break;
                    }
                    if (j < 0x100)
                        continue;
                    if (strlen(one) < 0x20) {
                        strcpy(names[cnt], one);
                        cnt++;
                    } else
                        sprintf(text, "Building name too long: %s", one);
                }
            }
            if (cnt != 0) {
                r = util_RandomU(cnt);
                u = names[r & 0xffff];
                if (strlen(u) < 0x20)
                    strcpy((char *)b + 5, u);
                else {
                    sprintf(err, "Building name too long: %s", u);
                    err_Message(err);
                }
            }
        }
        b = (unsigned char *)gm_BuildNext();
    }
}
/* 0x511ad0, "init_LocalData": selects the town universe, makes the player records (0x4feb30 / 0x4feb60) unless the flag 0x40 is set, finds the two judges (the characters of the kind 0xc with the byte 0 / 1 at +0x228: the "Richter Katholisch" / "Richter Evangelisch", stored in 0x6f1bfc / 0x6f1c00) and the list of the characters of the kind 0xb (0x6f1c04); a missing judge is an error; names the buildings (0x511890) unless the flags 0x44 are set; then 0x50cc60 @deep behaviour: 102 code hunks / 124 of 256 bytes, meaning read whole against 0x511ad0..0x511bc0 (the scene select, the player-record loop over the slot table, the two kind 0xc judge scans with the +0x228 byte 0 / 1, the kind 0xb collector walk, the two error texts, the name pass, the final call) - no meaning error left. Fixed this session: Q3 (0x7e4b10 -> INIT_PLAYER_SLOTS_END, 0x7d5f8c -> INIT_CHARACTERS_END + 0xc, 0x7d5f80 -> INIT_CHARACTERS_END) and Q7 with the real void gm_ResetStock(void) prototype, which is also the behaviour fix of the tail: the original ends in a tail jump (jmp 0x50cc60 at 0x511bb6), which VC6 only emits with the prototype (the implicit int declaration gave call + ret). Left (allocation / codegen): the judge scan's registers - the original loads only ai_JudgeProtestant into edi before the loop and keeps n in edx (test edx,edx / dec edx), ours keeps n in ebp (push ebp/pop ebp) and both snapshots inside the loop (mov ecx,ds:X / mov edx,esi), and the original tests p[-10] in memory (cmp BYTE [eax-0xa],0xc) where ours loads it into dl first; the kind 0xb walk's collector pointer: the original keeps q in ecx with the kind 0xb in dl, ours uses esi; the pop edi / pop esi placement before the two judge tests. 82 +/- lines in the diff, every one read - all register or slot, same instructions. */


void init_LocalData(void)
{
    char *p;
    struct AICHAR *j1, *j2;
    int n, i;
    char **q;
    char *c;

    d3if_SelectScene(0, 0);
    if ((ai_GameFlags & 0x40) == 0) {
        gm_UpdatePlayers();
        i = 0;
        p = ai_PlayerSlots[0];
        do {
            if (*p == 0)
                break;
            gm_InitPlayerRecord(i);
            p += 0x2f4;
            i++;
        } while ((int)p < INIT_PLAYER_SLOTS_END);
    }
    n = 2;
    p = ai_Chars[0] + 0x228;
    do {
        j1 = ai_JudgeCatholic;
        j2 = ai_JudgeProtestant;
        if (n == 0)
            break;
        if (p[-10] == 0xc) {
            if (*p == 0)
                ai_JudgeCatholic = (struct AICHAR *)(p - 0xc);
            else {
                if (*p != 1)
                    goto next;
                ai_JudgeProtestant = (struct AICHAR *)(p - 0xc);
            }
            n--;
        }
    next:
        p += 0x21c;
        j1 = ai_JudgeCatholic;
        j2 = ai_JudgeProtestant;
    } while ((int)p < INIT_CHARACTERS_END + 0xc);
    q = ai_KindBChars;
    c = ai_Chars[0];
    ai_JudgeCatholic = j1;
    ai_JudgeProtestant = j2;
    do {
        if (c[2] == 0xb) {
            *q = c;
            q++;
        }
        c += 0x21c;
    } while ((int)c < INIT_CHARACTERS_END);
    if (j1 == 0) {
        err_Message("init_LocalData(): gm_RichterKatholisch==NULL");
        j2 = ai_JudgeProtestant;
    }
    if (j2 == 0)
        err_Message("init_LocalData(): gm_RichterEvangelisch==NULL");
    if ((ai_GameFlags & 0x44) == 0)
        init_NameBuildings();
    gm_ResetStock();
}
/* 0x511bc0, fills the markets of the player p: for every one of the 0x55 goods records of the player's stock
   (0x7d6a70 + p * 0x2a90) that has a market object (gm_FindMarketObject) a random part of the record's demand
   is sent to the market (cm_SendExSellObject): 0.5 .. 1.5 of it for the goods kinds 0x17 / 0x25, 0.2 .. 1.2
   for the others. 0x55 fills all of the player's slots at the start of a game */


void init_FillMarkets(int p)
{
    int i;
    char *pr;
    GMSTOCK *g;
    INIT_OBJ *m;

    i = 0x55;
    g = &gm_StockBlocks[p].goods[0];
    do {
        pr = ai_Goods + g->item * 0x41;
        m = (INIT_OBJ *)gm_FindMarketObject(g->item);
        if (m != 0) {
            if (*pr == 0x17 || *pr == 0x25)
                cm_SendExSellObject(m->objId, -1, g->item, (int)((util_RandomF() + 0.5) * g->demand), ai_OwnPlayer, 0);
            else
                cm_SendExSellObject(m->objId, -1, g->item, (int)((util_RandomF() + 0.2) * g->demand), ai_OwnPlayer, 0);
        }
        g++;
        i--;
    } while (i != 0);
}
/* 0x511c70, gives the buildings of the classes 1 / 4 / 2 and of the types 0x13 / 4 / 0x10 / 5 / 9 (not 0xb) to the first character of the kind 6: 64000 in the building's money word (+0x39), the ownership (0x4c54a0), the staff (0x4c55d0) and the object 0x135 */


void unk_Unk511c70(void)
{
    char *c;
    int i;
    unsigned char *b;
    char k;
    int v;

    c = ai_Chars[0];
    for (i = 0; i < 0x300; i++) {
        if (c[2] == 6)
            break;
        c += 0x21c;
    }
    b = (unsigned char *)ai_Blds;
    while (1) {
        if (*b != 0 &&
            ((gm_BuildingClass(*b) == 1 || gm_BuildingClass(*b) == 4 || gm_BuildingClass(*b) == 2 ||
              ((k = ai_BTypes[*b * 0x24d]) == 0x13 || k == 4)) || (k == 0x10 || (k == 5 || k == 9))) &&
            ai_BTypes[*b * 0x24d] != 0xb) {
            v = 64000;
            cm_BeginFieldBlock(b, *(int *)(b + 1));
            cm_AddFieldDelta(4, 1, (char *)b - cm_FieldObject + 0x39, &v);
            cm_SendFieldDeltas();
            cm_SendOwnership(*(int *)(b + 1), *(int *)(c + 4), *(int *)(c + 4));
            cm_SendMessage3d(b, 1, 0, (unsigned char)ai_BTypes[*b * 0x24d + 0x223]);
            cm_SendExSellObject(*(int *)(b + 1), -1, 0x135, 1, ai_OwnPlayer, 0);
        }
        b += 0xa9;
    }
}
/* 0x511dc0, the universe of the scene of the object prototype p (a new one marked dirty (0xfd7f75) is only reserved, named after the prototype, and loaded later; the current prototype 0x6f2310 is loaded at once by 0x50d7f0) */


int init_ObjectSceneUniverse(unsigned short *p)
{
    int uni;
    char *d, *s;
    char c;

    if (init_ObjectUniverse[*p] == -1) {
        if (p == (unsigned short *)sv_PanelShown) {
            init_LoadObjectScene(*p);
            return init_ObjectUniverse[*p];
        }
        uni = d3t_FindFreeScene();
        if (uni == -1)
            return -1;
        init_ObjectUniverse[*p] = uni;
        init_SceneDirty[uni * 0x3e0] = 1;
        strcpy(d3_Scenes[uni].name, ai_Goods + *p * 0x41 + 1);
    }
    return init_ObjectUniverse[*p];
}
/* 0x511e60, the universe of the scene of the building b's type (0x76fa60): reserved (marked dirty, named after the type) when it is not the current building, else loaded at once by 0x50dc30; -1 when no universe is free @deep behaviour: 35 code hunks / 54 of 176 bytes, meaning read whole against 0x511e60..0x511f11 (the k = ai_BTypes[*b * 0x24d] lookup, the -1 test on init_BuildingUniverse[k], the fallback building / type 0x1e case that loads at once, the free scene, the dirty flag, the name strcpy, the two returns) - no meaning difference left. Meaning/type fixes this session: the t local removed so *b is read where the original re-reads it (0x511ec9), and k as unsigned with a (unsigned char) cast on the table byte: that is what the original does (xor ebx,ebx / mov bl,BYTE / mov esi,ebx, the byte compare cmp BYTE [esi+0x76fa60],0xff) and it took 144 -> 54 differing bytes / 84 -> 35 code hunks. Left (allocation): the inlined strcpy's registers - the original computes the name destination into ecx (lea ecx,[ecx+0xfd7ba0]) before re-reading *b and walks the source in eax/edx, ours keeps the destination in esi after re-reading *b; and the two returns read the byte from memory (movsx eax,BYTE [esi+0x76fa60]) where ours reuses al. Same instructions, only the register assignment and the block order. */


int init_BuildingSceneUniverse(unsigned char *b)
{
    unsigned k;
    int uni;

    k = (unsigned char)ai_BTypes[*b * 0x24d];
    if (init_BuildingUniverse[k] == -1) {
        if (b != (unsigned char *)ai_FallbackBuilding && *b != 0x1e) {
            uni = d3t_FindFreeScene();
            if (uni == -1)
                return -1;
            init_BuildingUniverse[k] = uni;
            init_SceneDirty[uni * 0x3e0] = 1;
            strcpy(d3_Scenes[uni].name, ai_BTypes + *b * 0x24d + 1);
            return init_BuildingUniverse[k];
        }
        init_LoadBuildingScene(*b);
    }
    return init_BuildingUniverse[k];
}


char d3t_GetBoundingSphere(struct D3OBJECT *obj, struct D3OBJECT *camera, struct D3VECTOR *center, float *radius);  /* 0x411eb0, d3_tools.c: the center and radius of the mesh's 8-corner box */
int gm_IsBuildingKind(int n);           /* 0x4fe940, f3gm.c: 1 when the object prototype n is a building part */
int gm_FindObjectProto(char *name);     /* 0x4fe900, f3gm.c: the object prototype of the name */
extern unsigned char init_TextureSet;      /* 0x6f1c24 */
/* 0x511f20, a scene callback of the object o (a function start Ghidra missed, registered with 0x438e60 (0x1e0) by
   0x513050; base is the object list of the scene: the scene object's +0x14, else the scene building's list): links an
   object of the object prototypes (kind byte +0x4b = 1) with its instance record and shows / hides it.
   An object whose goods class is none of 2 / 0x1d / 0x21 / 6 and that has an instance in the scene list: with a
   parent of the object prototypes it is shown only when the parent has an instance too, and a building part is then
   hidden when a part of the same class precedes it in the scene building's part list (ai_BTypes +0x22 / +0x23) or
   comes after the parent's; without a parent (or with another parent) it is hidden behind later parts of its class.
   A class 2 / 0x21 / 6 object takes its instance from the scene building (gm_Query 2, 6), its mask 1 when the player
   may use it (gm_PermissionBits 0x4000); an object without an instance is hidden (classes 0x1d / 0x21 excepted).
   An instance flagged 0xa0 (+0x13) gets the effect "Effekte/upgrade2.esc" at its bounding sphere once (the flags are
   cleared). The objects named "no_*" are hidden when the prototype after the prefix has an instance, "pfl_*" /
   "vg_*" select the texture set init_TextureSet.
   @deep reconstructed from the whole original 0x511f20..0x5124c9 (the old draft summarised the record
   walks): the class chain as one if / else-if over g[0] with the instance queries in the conditions, the
   part-order walk over the scene building's ai_BTypes part list (nParts +0x22, parts +0x23, & 0x7fff,
   first / second index) and the parent's later parts, the part queries as gm_Query(base, 1, 4, class) (the
   draft had 4, 1), gm_PermissionBits on ai_SceneBuilding itself (not its list), the effect script
   Effekte/upgrade2.esc (the draft had a cut string) with its 8 arguments (center x/y/z, radius,
   radius*1.5, o), d3_Camera, the no_ query with the ternary list argument; Q2/Q4/Q6 cleared through
   SCENEOBJ.info / INIT_OBJ.flags / obj / INIT_BTYPEREC. 519 -> 283 differing, 465 vs 465 instructions,
   size equal. Left: allocation - the original keeps rec in esi across branch D and the A branch (ours edi
   / [esp+0x18] and a reload after d3if_ShowTree), a / b of the part walk at [esp+0x18]/[esp+0x1c] with ia
   in esi (ours ebx and [esp+0x1c]/[esp+0x20]), the part id masked in a 16-bit register (and cx,0x7fff;
   ours and eax / test ax - a short v gave the and but sign extensions, 471 instructions, reverted), base
   at [esp+0x34] vs [esp+0x30] (the original's slot is the dead argument slot above o), the no_ branch's
   two returns tail-merged in ours */
void init_SyncSceneObjects(char *o)
{
    SCENEOBJ *so = (SCENEOBJ *)o;
    char *base;                        /* the object list of the scene */
    char *g;                           /* the ai_Goods record (0x41 bytes) of o's prototype, its class at 0 */
    INIT_OBJ *rec, *s, *e;
    SCENEOBJ *parent;
    INIT_BTYPEREC *bt;
    EVTSCRIPT *sc;
    D3VECTOR center;
    float radius;
    unsigned id;
    unsigned short a, b, v;
    int kind, fx, i, n, ia, ib;

    rec = 0;
    id = so->proto & INIT_PROTO_MASK;
    kind = o[0x4b];
    base = ai_SceneObject != 0 ? (char *)((INIT_OBJ *)ai_SceneObject)->list : (char *)((INIT_BLD *)ai_SceneBuilding)->objList;
    if (kind == 1) {
        g = ai_Goods + id * 0x41;
        if (g[0] != 2 && g[0] != 0x1d && g[0] != 0x21 && g[0] != 6 && (rec = (INIT_OBJ *)gm_Query(base, 1, 0, id)) != 0) {
            rec->obj = 0;
            parent = so->parent;
            if (parent != 0) {
                unsigned pid = parent->proto & INIT_PROTO_MASK;

                if (((char *)parent)[0x4b] == 1) {
                    s = (INIT_OBJ *)gm_Query(base, 1, 0, pid);
                    if (s != 0) {
                        char *sg;

                        d3if_ShowTree((struct D3OBJECT *)o, 1);
                        so->owner = (SCENEOBJ *)rec;
                        so->mask = 0;
                        rec->obj = so;
                        if (gm_IsBuildingKind(rec->proto) != 0) {
                            for (e = (INIT_OBJ *)gm_Query(base, 1, 4, (unsigned char)g[0]); e != 0; e = (INIT_OBJ *)gm_QueryNext()) {
                                a = e->proto;
                                b = rec->proto;
                                if (a != b) {
                                    ia = -1;
                                    ib = -1;
                                    bt = (INIT_BTYPEREC *)(ai_BTypes + *ai_SceneBuilding * 0x24d);
                                    n = bt->nParts;
                                    for (i = 0; i < n; i++) {
                                        v = bt->parts[i] & 0x7fff;
                                        if (v != 0) {
                                            if (a == v)
                                                ia = i;
                                            else if (b == v)
                                                ib = i;
                                        }
                                    }
                                    if (ia != -1 && ia > ib)
                                        d3if_ShowTree((struct D3OBJECT *)o, 0);
                                }
                            }
                        }
                        sg = ai_Goods + s->proto * 0x41;
                        if (gm_IsBuildingKind(rec->proto) != 0) {
                            for (e = (INIT_OBJ *)gm_Query(base, 1, 4, (unsigned char)sg[0]); e != 0; e = (INIT_OBJ *)gm_QueryNext()) {
                                if (e->proto > s->proto)
                                    d3if_ShowTree((struct D3OBJECT *)o, 0);
                            }
                        }
                    } else {
                        d3if_ShowTree((struct D3OBJECT *)o, 0);
                        so->owner = 0;
                    }
                } else {
                    d3if_ShowTree((struct D3OBJECT *)o, 1);
                    so->owner = (SCENEOBJ *)rec;
                    so->mask = 0;
                    rec->obj = so;
                    if (gm_IsBuildingKind(rec->proto) != 0) {
                        for (e = (INIT_OBJ *)gm_Query(base, 1, 4, (unsigned char)g[0]); e != 0; e = (INIT_OBJ *)gm_QueryNext()) {
                            if (e->proto > rec->proto)
                                d3if_ShowTree((struct D3OBJECT *)o, 0);
                        }
                    }
                }
            } else {
                g = ai_Goods + id * 0x41;
                d3if_ShowTree((struct D3OBJECT *)o, 1);
                so->owner = (SCENEOBJ *)rec;
                if (g[0] != 0x1d)
                    so->mask = 0;
                rec->obj = so;
                if (gm_IsBuildingKind(rec->proto) != 0) {
                    for (e = (INIT_OBJ *)gm_Query(base, 1, 4, (unsigned char)g[0]); e != 0; e = (INIT_OBJ *)gm_QueryNext()) {
                        if (e->proto > rec->proto)
                            d3if_ShowTree((struct D3OBJECT *)o, 0);
                    }
                }
            }
        } else if (g[0] != 0x1d && (rec = (INIT_OBJ *)gm_Query((void *)((INIT_BLD *)ai_SceneBuilding)->objList, 2, 6, 0, id)) != 0) {
            d3if_ShowTree((struct D3OBJECT *)o, 1);
            so->owner = (SCENEOBJ *)rec;
            if ((g[0] == 6 || g[0] == 2) && ai_FallbackBuilding != 0) {
                if (gm_PermissionBits(ai_PlayerId, ai_SceneBuilding, (unsigned short *)rec, 0) & 0x4000)
                    so->mask = 1;
            } else {
                so->mask = 0;
            }
            rec->obj = so;
        } else if (ai_Goods[id * 0x41] != 0x1d && ai_Goods[id * 0x41] != 0x21) {
            so->owner = 0;
            if (so->type != 1)
                d3if_ShowTree((struct D3OBJECT *)o, 0);
        }
        if (rec != 0 && (rec->flags & 0xa0) != 0) {
            if (so->info == 0)
                d3_InflateObject((D3OBJECT *)o);
            if (so->mesh != 0 && d3t_GetBoundingSphere((struct D3OBJECT *)o, d3_Camera, &center, &radius) != 0) {
                fx = (int)(radius * 1.5);
                sc = evt_Load("Effekte/upgrade2.esc");
                if (sc != 0)
                    evt_RunScriptArgs(sc, 6, (int)center.x, (int)center.y, (int)center.z, (int)radius, fx, o);
            }
            rec->flags &= 0x5f;
        }
    }
    if (strncmp(o, "no_", 3) == 0) {
        i = gm_FindObjectProto(o + 6);
        if (i != -1) {
            s = (INIT_OBJ *)gm_Query(ai_Goods[i * 0x41] == 2 ? (char *)((INIT_BLD *)ai_SceneBuilding)->objList : base, 1, 0, i);
            if (s != 0)
                d3if_ShowTree((struct D3OBJECT *)o, 0);
            else
                d3if_ShowTree((struct D3OBJECT *)o, 1);
        }
        return;
    }
    if (strncmp(o, "pfl_", 4) == 0 || strncmp(o, "vg_", 3) == 0) {
        if (so->info != 0)
            d3if_SelectTextureSet((struct D3OBJECT *)o, (struct D3SURF *)so->info, init_TextureSet, 1);
        else
            d3if_SelectTextureSet((struct D3OBJECT *)o, (struct D3SURF *)((char *)so->mesh + 0xf4), init_TextureSet, 1);
    }
}
/* 0x5124d0, the first building of the type 0xb: 0x511e60 */


void unk_Unk5124d0(void)
{
    int b;

    b = (int)gm_BuildQuery(1, 5, 0xb);
    if (b != 0)
        init_BuildingSceneUniverse(b);
}
/* 0x5124f0, loads the shopping script of the building b for the character of b's master (gm_FindCharacterByMaster)
   when it is among the scene characters ai_SceneChars with its byte +8 above 1 and, with an object o, its model
   belongs to o's group (+0x30): "%slocations\%s\Einkauf_%s.esc" in the script folder evt_Path with the name of b's
   type for a home, else of o's prototype. When the file exists, an earlier script of the character's model (+0x28)
   is stopped, the new one runs with the model and ai_FavourComment, the script records the model and its id goes to
   the model and to init_ShopScript (-1 when the script does not load).
   @deep read against the whole original 0x5124f0..0x5126a0 and rewritten: the window walk as a for loop
   with the break at its head (no goto), the later reads through c (the last candidate, esi) as the
   original does, the home-type record first, name initialised 0 and taken from the goods record only when
   that is nonzero, the old script id read once; Q2 (ai_SceneChars, ai_FavourComment), Q4 (INIT_CHAR.health
   / model, SCENEOBJ.script / scriptGroup, INIT_OBJ, EVTSCRIPT.id / character), Q5, Q7
   (gm_FindCharacterByMaster in f3gm.h) cleared. 99 -> 95 differing, 132 vs 139 instructions. Left:
   allocation - the original keeps b in ebp and name in a frame slot ([esp+0x10], frame 0x13c vs 0x138) and
   so duplicates the sprintf tail for the goods path (0x5125be) with the null path reading the slot (mov
   eax,[esp+0x10]); ours keeps name in ebp, reloads b from its argument slot and joins the paths before one
   sprintf (lea eax,[ebp+1]); the rest are the follow-on register choices (eax/ecx/edx around
   evt_RunScriptArgs and the id stores) */
void init_LoadShopScript(unsigned char *b, unsigned short *o)
{
    INIT_CHAR *c, *w, *m;
    INIT_CHAR **q;
    EVTSCRIPT *h, *old;
    char *name;
    int id;
    char buf[0x104];
    char s1[4], s2[0x30];

    name = 0;
    w = 0;
    c = 0;
    m = (INIT_CHAR *)gm_FindCharacterByMaster((int)b);
    for (q = (INIT_CHAR **)ai_SceneChars; (int)q < (int)INIT_WINDOWS_END; q++) {
        if (w != 0)
            break;
        c = *q;
        if (c != 0 && c == m)
            w = c;
    }
    if (w != 0 && w->health > 1 && (o == 0 || w->model->scriptGroup == ((INIT_OBJ *)o)->objId)) {
        if (gm_IsHomeType(b) != 0)
            name = (char *)INIT_BTYPE(b);
        else if (ai_Goods + ((INIT_OBJ *)o)->proto * 0x41 != 0)
            name = ai_Goods + ((INIT_OBJ *)o)->proto * 0x41;
        sprintf(buf, "%slocations\\%s\\Einkauf_%s.esc", evt_Path, name + 1, name + 1);
        if (vfs_FindFile(buf, vfs_Root, s1) != 0) {
            sprintf(s2, "sp_%i", c->netId);
            h = evt_LoadScript(buf);
            if (h != 0) {
                id = c->model->script;
                if (id != -1 && (old = evt_FindById(id)) != 0)
                    evt_Stop(old);
                evt_RunScriptArgs(h, 2, c->model, ai_FavourComment);
                h->character = (struct CHARACTER *)c->model;
                c->model->script = h->id;
                init_ShopScript = h->id;
                return;
            }
            init_ShopScript = -1;
        }
    }
}
/* 0x5126a0, the scene callback for the "sfx_" objects (a function start Ghidra missed): starts the sound
   named after the prefix (the name's "_l" / "_r" suffixes decide on looping and a 2000 ms delay) at the
   object's position (d3snd_Attach, volume 0x7f, range the object's sound radius + 50) when sound is on
   (snd_SfxEnabled), then clears the object's state (0x224)
   @deep behaviour: 8 code hunks / 12 of 224 bytes, meaning read whole (the sfx_ prefix, the strcpy of the rest, the _l / _r loop and delay, the sound-on gate, the sample lookup, the attach with the volume 0x7f and the radius +50, the play with loop/delay, the mask clear) - no meaning difference left. This session: Q1 init_SoundObjectHook, Q2 through the canonical names of three callees (d3snd_Attach -> d3snd_Attach, d3snd_Play -> d3snd_Play, sb_GetSample -> sb_GetSample, from d3_snd.c / f3snd.h), Q7 with their prototypes, Q4 through the new SCENEOBJ.soundRange (0x68) and SCENEOBJ.mask (0x224) - 48 -> 8 code hunks. Left (allocation): the frame is 0x108 against the original's 0x104 (one local too many on the stack, name at the same [esp+0x1c]); the original keeps loop in edi where ours uses esi and the sample pointer in esi where ours uses edi (the two are swapped through the three calls). */


void init_SoundObjectHook(char *o)
{
    char name[0x104];
    int loop, delay, h;
    struct SBSAMPLE *s;

    if (strncmp(o, "sfx_", 4) == 0) {
        strcpy(name, o + 4);
        loop = strstr(name, "_l") == 0;
        if (strstr(name, "_r") != 0) {
            delay = 2000;
            loop = 0;
        } else
            delay = 0;
        if (snd_SfxEnabled != 0) {
            s = sb_GetSample(0, name);
            if (s != 0) {
                h = d3snd_AttachI((int)s, o, 0x7f, (float)((int)((SCENEOBJ *)o)->soundRange + 0x32));
                if (h != 0)
                    d3snd_Play((struct D3SOUND *)h, 0, loop, delay);
            }
        }
        ((SCENEOBJ *)o)->mask = 0;
    }
}
/* 0x512780, the scene object callback for "gate" objects: sets the state (+0x224 = 1, the flag 1 of +0x211 cleared) when the name contains "gate", else the state 0 */


void init_GateObjectHook(int o)
{
    if (strstr((char *)o, "gate") != 0) {
        *(int *)(o + 0x224) = 1;
        *(unsigned char *)(o + 0x211) &= 0xfe;
        return;
    }
    *(int *)(o + 0x224) = 0;
}
/* 0x5127c0, the sound hook of the "sfx_" scene objects: frees the sound at SCENEOBJ.sound (0x4179d0) */


void init_SoundObjectFreeHook(char *o)
{
    if (strncmp(o, "sfx_", 4) == 0 && ((SCENEOBJ *)o)->sound != 0)
        d3snd_Free(((SCENEOBJ *)o)->sound);
}
/* 0x5127f0, starts the looped sound name once (its volume scaled by the 0.7 of 0x61adfc) and keeps its handle in
   the table 0x76fd80 (16 entries) */


void init_PlayTownSound(char *name, int volume, int delay)
{
    int h;

    h = sb_PlaySample(0, name, 0, 0x3f, (int)(volume * init_SoundVolume), 1, delay);
    if (init_TownSoundCount < 0x10) {
        init_TownSounds[init_TownSoundCount] = h;
        init_TownSoundCount++;
    }
}
/* 0x512840, the ambient sound of a graveyard building b of the type 0x1e on the map (the word +0x29 == 0xfd): "AesteKnacken_s" */


void init_PlayGraveyardSound(char *b)
{
    gm_ClockSeason(&ai_Time);
    if (b != 0) {
        if (gm_IsHomeType(b) != 0 && *(short *)(b + 0x29) == 0xfd && *b == 0x1e)
            init_PlayTownSound("AesteKnacken_s", 0x50, 8000);
    }
}
/* 0x512890, stops all sounds of the town table (0x427290) */


void unk_Unk512890(void)
{
    int i, n;

    i = 0;
    if (0 < init_TownSoundCount) {
        do {
            sb_Stop(init_TownSounds[i], 1);
            n = init_TownSoundCount;
            init_TownSounds[i] = 0;
            i++;
        } while (i < n);
    }
    init_TownSoundCount = 0;
}
/* 0x5128d0, switches the seasonal textures of the town scene (d3_Terrain, the names of 0x40 bytes at +0x1a64) to
   the current season (gm_ClockSeason): when no name carries the season's suffix yet, each is cut at its old suffix
   ("_FRUEHLING" / "_HERBST" / "_SNOW") and gets the season's suffix when that texture exists (tex_FindFile), then
   the tile is set again (d3fl_ReplaceTexture). The summer (1) only cuts the suffixes (a name without one stays as
   it is); the four seasons are four near copies in the original. */


void init_SetSeasonTextures(void)
{
    int season, i, j, found;
    char *nm, *u;
    char a[0x80], b[0x80];

    season = gm_ClockSeason(&ai_Time);
    if (d3_Terrain == 0)
        return;
    if (season == 3) {
        found = 0;
        i = 0;
        while (!found) {
            if (i >= 0x200)
                break;
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_SNOW") != 0)
                found = 1;
            i += 0x40;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_SNOW", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 2) {
        found = 0;
        i = 0;
        while (!found) {
            if (i >= 0x200)
                break;
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_HERBST") != 0)
                found = 1;
            i += 0x40;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_HERBST", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 0) {
        found = 0;
        i = 0;
        while (!found) {
            if (i >= 0x200)
                break;
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_FRUEHLING") != 0)
                found = 1;
            i += 0x40;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_FRUEHLING", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 1) {
        found = 0;
        i = 0;
        while (!found) {
            if (i >= 0x200)
                break;
            if (*((char *)d3_Terrain + 0x1a64 + i) != 0
                && (strstr((char *)d3_Terrain + 0x1a64 + i, "_FRUEHLING") != 0
                    || strstr((char *)d3_Terrain + 0x1a64 + i, "_HERBST") != 0
                    || strstr((char *)d3_Terrain + 0x1a64 + i, "_SNOW") != 0))
                found = 1;
            i += 0x40;
        }
        if (!found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0) {
                    *u = 0;
                    if (tex_FindFile(a) != 0)
                        d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
                }
            }
        }
    }
}
/* 0x512de0, for the scene objects "pfl_" / "vg_" / "!vg_": registers them (0x446f30) with their flags (+0x1cc) in the list 0x6f1c24 */


void init_PlantTextureSetHook(char *o)
{
    if (strncmp(o, "pfl_", 4) != 0 && strncmp(o, "vg_", 3) != 0 && strncmp(o, "!vg_", 4) != 0)
        return;
    d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), init_TextureSet, 1);
}
/* 0x512e40, the scene object callback: unless the object is of the kind 2 / 4 it switches the textures of the
   object's mesh (SCENEOBJ.mesh, the set list at +0xf4) to the current set (init_TextureSet, 0x446f30)
   @deep meaning fixed: the 2nd argument is the mesh + 0xf4 (ours dereferenced it), the typed views SCENEOBJ.kind/SCENEOBJ.mesh replace the raw casts, the return is char (the original ends mov al,1), d3if_SelectTextureSet -> d3if_SelectTextureSet (ecmd.c's void *list prototype, which also clears the cross-TU conflict) and ai_Unk6f1c24 -> init_TextureSet (written from gm_ClockSeason in cm.c, the set d3if_SelectTextureSet switches to): 9 -> 6 differing instructions. Remaining: the original's dead cmp ecx,0x3 between add edx,0xf4 and mov cl,[0x6f1c24] (its flags are overwritten by the next mov; tried s != 3 as a third && term (extra je), a switch with case 3/default merged (sub eax,2 chain), the tautology s != 3 || s == 3), plus ours loading the byte before push 0x1/add edx,0xf4 */


char init_TextureSetHook(int o)
{
    int s;

    s = ((SCENEOBJ *)o)->kind;
    if (s != 2 && s != 4)
        d3if_SelectTextureSet((D3OBJECT *)o, (char *)((SCENEOBJ *)o)->mesh + 0xf4, init_TextureSet, 1);
    return 1;
}
/* 0x512e80, the scene callback (a function start Ghidra missed) for the objects of a building b whose state is not 2 / 3 / 4 and whose name does not start with "pfl_" / "vg_" / "!vg_": registers (0x446f30) the object with its list for every slot 0 .. (the building kind's rank byte (+0x247) - 1, + 1 for the type 2) */


char init_BuildingTextureSetHook(D3OBJECT *o, void *ctx)
{
    unsigned char *b = ctx;
    char *p;
    int n, i;
    int st;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    n = (unsigned char)p[0x247] - 1;
    if (*p == 2)
        n++;
    st = o->kind;
    if (st == 2 || st == 4 || st == 3)
        return 1;
    if (strncmp(o->name, "pfl_", 4) != 0 && strncmp(o->name, "vg_", 3) != 0 && strncmp(o->name, "!vg_", 4) != 0) {
        for (i = 0; i <= n; i++)
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, i, 1);
    }
    return 1;
}
/* 0x512f30, sets the 16 character animations (ch_PreloadAnims, 0x404200) of the characters that live in or belong
   to the building b (its scene object +0x16c or the group id +0x2c), from the class animation record of the table
   0x61ae08 whose kind is b's class (init_ClassAnims)
   @deep read the whole original 0x512f30..0x513040 against the source: the class lookup in the 19-record table 0x61ae08 (kind = ai_BTypes[b[0]*0x24d]), sv_CollectGroupCharacters(b), the walk of ai_Chars + 0x184 in steps of 0x21c up to +0x184+0x300*0x21c, the q[-6] == b / group == b->id test and the 18-argument ch_PreloadAnims call (r->names 0..15, 0x10) all correspond - meaning equal. Readability done: Q1 init_SetCharacterAnimations, Q2 (ch_PreloadAnims 0x404200, sv_CollectGroupCharacters 0x52b2a0 renamed in sv.c, init_ClassAnims), Q3 (the end is (char *)ai_Chars + 0x184 + 0x300*0x21c), Q4 (new INIT_ANIMSET view, INIT_CHAR.group at 0x2c, INIT_BLD.id), Q7 (varargs prototype). Tried: dropping the redundant i (same code). Left (allocation): ours keeps b in ebx where the original uses ebp and the class-byte address in edi vs edx, idx in esi vs ecx - the prologue (512f30) and the character loop (512f9e) hunks */


void init_SetCharacterAnimations(unsigned char *b)
{
    int idx;
    unsigned k;
    int *q;
    int ch;
    INIT_ANIMSET *r;

    idx = 0;
    if (init_ClassAnims[0].kind != 0) {
        k = init_ClassAnims[0].kind;
        do {
            if (k == (unsigned char)ai_BTypes[*b * 0x24d])
                break;
            k = init_ClassAnims[idx + 1].kind;
            idx++;
        } while (k != 0);
    }
    r = init_ClassAnims + idx;
    sv_CollectGroupCharacters((char *)b);
    q = (int *)((char *)ai_Chars + 0x184);
    do {
        ch = *q;
        if (ch != 0 && (q[-6] == (int)b || ((INIT_CHAR *)ch)->group == ((INIT_BLD *)b)->id)) {
            ch_PreloadAnims((struct CHARACTER *)ch, 0x10, r->names[0], r->names[1], r->names[2], r->names[3],
                            r->names[4], r->names[5], r->names[6], r->names[7], r->names[8], r->names[9],
                            r->names[10], r->names[11], r->names[12], r->names[13], r->names[14], r->names[15]);
        }
        q += 0x87;
    } while ((int)q < (int)((char *)ai_Chars + 0x184 + 0x300 * 0x21c));
}
/* 0x513050, enters the scene of the building b (init_ShowBuildingScene): the room object o (the slot of b's object
   list, or 0xfd = the building's own scene) selects the universe (init_ObjectUniverse / init_BuildingUniverse),
   the scene object ai_SceneObject / the building ai_SceneBuilding are set, the detail tree "ordner_DETAILS" is
   shown, the group / dialog state (ch_CurrentGroup / evt_NoGroup) and the fog / light / season defaults are set
   (d3o_FogScale = 1.0, d3_LightKeyBudgetVisible = 0x800, init_TextureSet = season), the scene callbacks are
   registered for home-type buildings (init_PlantTextureSetHook / init_TextureSetHook, mask 0xc0), for the gates
   (init_GateObjectHook, 0x20) and for the scene records (init_SyncSceneObjects, 0x1e0), the objects of the
   building / of its person are shown and the characters near it placed (at most 4, or 8 for the class 0x16;
   ch_List first, then the models of the ai_Chars records)
   @deep behaviour + readable, meaning read whole against 0x513050..0x5133f6 and fixed: the room object's proto is read once (an unsigned short local from a plain unsigned short *o; the old char temp made the table byte sign-extend after the merged call), the room record's own id is at +2 not +4 (*(INIT_OBJ *)o)->objId), the ai_Chars model walk re-reads *q before each use, the two loop ends are signed int compares, b is an INIT_BLD (id +1, objList +0x5d, plants +0x71), sv_PanelShown is an INIT_OBJ * (objId), and the 3 loops use SCENEOBJ (group 0x2c / scriptGroup 0x30) + the INIT_PANELCHAR chain view (+0x3b). Left: allocation / scheduler shape - 13 hunks / 94 differing instructions, none a meaning difference: (1) VC6 merges the valid room branch's ai_MsgNoOp() (0x4b70f0, a bare ret) with the invalid branch's, the original calls it twice (tried: statement order, polarity, goto, call before the load - always merged); (2) d3if_ShowTree(d, ai_GfxDetail != 0) - the original materializes the char bool with test/jne + push 0/push 1, ours with setne cl/push ecx (tried ternary, !!, & 1, > 0, a cast, an int parameter - no change); (3) the else arm's redundant t->scene == d3_DefaultScene re-test, which VC6 folds for every cast form tried; (4) register allocation of the d3_ForEachObject2 pair (edx/eax vs ecx/edx) and of the two character walks. */


void init_ShowBuildingScene(unsigned char *b, int slot)
{
    INIT_BLD *bd = (INIT_BLD *)b;
    unsigned short *o;
    SCENEOBJ *t;
    SCENEOBJ **q;
    struct D3OBJECT *d;
    int season, cls, i, uni, limit;
    unsigned short proto;
    char *p;

    season = gm_ClockSeason(&ai_Time);
    cls = gm_BuildingClass(*b);
    p = ai_BTypes + (unsigned)*b * 0x24d;
    o = (unsigned short *)gm_Query(bd->objList, 1, 0, slot);
    if (o == 0 || (proto = *o) == 0xfd) {
        uni = init_BuildingUniverse[(unsigned char)ai_BTypes[*b * 0x24d]];
        ai_SceneObject = 0;
        ((int (*)())ai_MsgNoOp)();
    } else {
        ai_SceneObject = o;
        uni = init_ObjectUniverse[proto];
        ((int (*)())ai_MsgNoOp)();
    }
    if (d3_CurrentScene == uni && (ai_PlayerBuilding == 0 || ai_PlayerBuilding == (int)b) && (ai_FallbackBuilding == 0 || ai_FallbackBuilding == (int)b)) {
        d = d3_FindObject(0, 0x180, 0, "ordner_DETAILS");
        if (d != 0)
            d3if_ShowTree(d, ai_GfxDetail != 0);
        ch_CurrentGroup = bd->id;
        if (sv_PanelShown == 0)
            evt_NoGroup = bd->id;
        else
            evt_NoGroup = sv_PanelShown->objId;
        if (season == 1)
            init_TextureSet = 1;
        else if (season == 0)
            init_TextureSet = 0;
        else if (season == 2)
            init_TextureSet = 2;
        else if (season == 3)
            init_TextureSet = 3;
        d3_FogByDistance = 0;
        d3o_FogScale = 1.0f;
        d3_LightKeyBudgetVisible = 0x800;
        if (gm_IsHomeType(b) != 0) {
            d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_PlantTextureSetHook);
            d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_TextureSetHook);
        }
        if (*b == 0x1e && slot == 0xfd)
            veg_ShowPlants((struct G15PLANT *)bd->plants);
        ai_SceneBuilding = b;
        sv_CollectGroupCharacters((char *)b);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SyncSceneObjects);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_GateObjectHook);
        gm_ShowBuildingObjects(b);
        i = 0;
        if (*p == 0x16)
            limit = 8;
        else
            limit = (*p != 1) * 4 + 4;
        q = (SCENEOBJ **)ch_List;
        do {
            t = *q;
            if (t != 0 && d3t_SceneIndex(t->scene) == uni) {
                if (t->group == bd->id || t->scriptGroup == ((INIT_OBJ *)o)->objId) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround((struct CHARACTER *)t, 0);
                } else
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            q++;
        } while ((int)q < (int)(ch_List + INIT_CHARACTER_COUNT));
        q = &((INIT_CHAR *)ai_Chars)->model;
        do {
            if (*q != 0 && d3t_SceneIndex((*q)->scene) == uni) {
                t = *q;
                if ((t->group == bd->id || t->scriptGroup == ((INIT_OBJ *)o)->objId) && i < limit) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround(*q, 0);
                    gm_SelectModelVariant((char *)q - 0x184);
                    gm_ChooseHeadIcon((unsigned short *)((char *)q - 0x184));
                    i++;
                } else
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            q += 0x87;
        } while ((int)q < (int)((char *)ai_Chars + 0x184 + INIT_CHARACTER_COUNT * 0x21c));
        t = (SCENEOBJ *)gm_Query(0, 2, 7, 4, 0x1d);
        while (t != 0) {
            t = ((INIT_PANELCHAR *)t)->next;
            if (t->scene == d3_DefaultScene) {
                if (t->group == bd->id) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround((struct CHARACTER *)t, 0);
                    ch_PlaceTransport((struct CHARACTER *)t, 1);
                } else if ((int)t->scene == (int)d3_DefaultScene)
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            t = (SCENEOBJ *)gm_QueryNext();
        }
    }
}
/* 0x513400, finishes the scene of the building b (init_FinishBuildingScene): one of the "contact_EINKAUF*" shop
   counter dummies ("contact_EINKAUF", then _METALL / _SKRIPTE / _HOLZ / _STEIN) is shown for the local player and
   its state (+0x224) set, else the shopping script of the building is loaded (init_LoadShopScript, also for the
   class 5 buildings without a contact dummy); then the sound hook of the "sfx_" objects is registered (mask 0x20),
   the graveyard ambience played, the texture-set slot hook registered for the buildings that are no home type
   (mask 0xc0), the detail level set (d3_Terrain), the season textures switched and the sky colours updated */


void init_FinishBuildingScene(unsigned char *b, unsigned short *o)
{
    INIT_BLD *bd = (INIT_BLD *)b;
    SCENEOBJ *d;
    char *p;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    if (o == 0)
        o = (unsigned short *)gm_Query(bd->objList, 2, 6, 0, 0xfd);
    d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_METALL");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_SKRIPTE");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_HOLZ");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_STEIN");
    if (d != 0) {
        if (bd->owner == ai_PlayerId)
            d3if_Show((struct D3OBJECT *)d, 0);
        else {
            d3if_Show((struct D3OBJECT *)d, 1);
            init_LoadShopScript(b, o);
        }
        d->mask = 1;
    } else if (*p == 5 && bd->owner != ai_PlayerId)
        init_LoadShopScript(b, o);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_SoundObjectHook);
    init_PlayGraveyardSound((char *)b);
    if (gm_IsHomeType(b) == 0)
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, init_BuildingTextureSetHook, b);
    init_SetDetailLevel((int *)d3_Terrain, 0);
    init_SetSeasonTextures();
    d3if_Prerender(0);
    sv_UpdateSkyColors();
}
/* 0x513590, sets the detail level of the scene s (the flags in the byte at +0x71c * 4: 0x10 = high, 8 = middle, 4 = low) from the setting byte 0x6f35c8 + 2 and the frame limits, and tells the renderer (0x43f8a0 the size 0x80 / 0x40, 0x446dc0 the level); the flag f forces the update @deep behaviour + readable, meaning read whole against 0x513590..0x5136a2 and fixed: init_Detail is an unsigned char (the original stores mov BYTE ds:0x61adf8,4/2/1 - the draft's int gave a dword store; ls.c 0x51d2d0 also loads it as a byte, so its extern is unsigned char now), the size is d3fl_DetailScale << d3fl_DetailSizeShift with no & 0x1f (the original shl ebx,cl has no and), and the final test compares the old level computed once (oldlvl = (old>>2)&7) with ((new>>2)&7) as the original's cmp ebp,edx - the draft recomputed it and VC6 emitted an xor/test. Q2 via the d3fl_ aliases, Q7 via the canonical void d3fl_SetShift(D3FLMAP *, unsigned char, char) in cm.h/cm.c. Left: 4 hunks / 60 differing instructions, all register allocation: ours spills oldlvl to [esp+0x14] and keeps lvl in ebp, size in edi, a in ebx where the original keeps lvl=edi, oldlvl=ebp, size=ebx, a=edx (and the set byte in cl vs dl). Tried: declaration orders (lvl/a/size/oldlvl), oldlvl computed after size, oldlvl as unsigned char, the no-fl ((unsigned char *)s)[0x1c70] form - all identical or worse. */


void init_SetDetailLevel(int *s, char f)
{
    unsigned char old;
    int a, lvl, size, oldlvl;
    unsigned char *fl;
    unsigned char set;

    a = 0;
    lvl = 0;
    if (s == 0)
        return;
    fl = (unsigned char *)(s + 0x71c);
    old = *fl;
    oldlvl = (old >> 2) & 7;
    set = ai_GfxFloor.byte[2];
    size = d3fl_DetailScale << d3fl_DetailSizeShift;
    if (d3_CurrentScene == 0) {
        if (*s != 0x40 && ai_MapSize < 0x780) {
            if (set == 0) {
                lvl = 4;
                a = 0x80;
                *fl = (old & 0xf3) | 0x10;
                init_Detail = 4;
                goto upd;
            }
            if (set == 1) {
                *fl = (old & 0xeb) | 8;
                init_Detail = 2;
                goto mid;
            }
            if (set != 2)
                goto upd;
        }
        lvl = 1;
        *fl = (old & 0xe7) | 4;
        init_Detail = 1;
    } else if (set == 2) {
        lvl = 1;
        *fl = (old & 0xe7) | 4;
    } else {
        *fl = (old & 0xeb) | 8;
mid:
        lvl = 2;
    }
    a = 0x40;
upd:
    if (a != size || f != 0)
        d3fl_SetDetail(a);
    if (oldlvl != ((*fl >> 2) & 7) || f != 0)
        d3fl_SetShift((struct D3FLMAP *)s, lvl >> 1, 1);
}
/* 0x5136b0, leaves the town scene: releases the hooks (0x5127c0), the sounds (0x512890), resets the frame limits and frees the scene handle 0x61ae00 */


void unk_Unk5136b0(void)
{
    int h;

    d3_ForEachObject2(d3_DefaultScene, 0, 0xa0, init_SoundObjectFreeHook);
    unk_Unk512890();
    d3_LightKeyBudgetVisible = 0x400;
    d3_LightKeyBudgetCulled = 0x40;
    if (init_ShopScript != -1) {
        h = (int)evt_FindById(init_ShopScript);
        if (h != 0) {
            evt_Stop((EVTSCRIPT *)h);
            sv_Unk52bb70(*(int *)(h + 0x100));
            init_ShopScript = -1;
        }
    }
    ch_ClearSceneCallbacks(d3_DefaultScene);
    ch_CurrentGroup = -1;
}
/* 0x513740, after loading a town: the "now" of the game clock (0x4fe350 of the clock 0x770ae0, kept as the season / day part 0..3 in 0x6f1c24), the camera of the player's character (0x506430, 0x43b790 / 0x525dd0 / 0x43b860, the default 0.33 when it has none), the characters without a house (0x2c = -1) that are not in the shown scene are hidden (0x402540 / 0x402480 / 0x4fb5a0), the scene callbacks 0x512de0 (0xe0) / 0x512e40 (0xc0), the scene's own hook (+0x210 of its parent), the frame limits (0x5128d0 / 0x513590) and the light factor 0x5fbd04 = 1 - 0.01 * the detail byte 0x6f35cc; then 0x525ad0 and the scene switch 0x447860 (0 when 0x6339db is set, else 1). ROUGH */


void init_AfterTownLoad(void)
{
    int phase, c;
    int *p;
    char *e;

    phase = gm_ClockSeason(&ai_Time);
    if (d3_CurrentScene == 0) {
        c = (int)gm_PlayerRecord(ai_Chars[(unsigned short)ai_PlayerId]);
        if (c == 0 || *(int *)(c + 0x88) == -0x40800000) {
            sv_PlaceCamera(0x3ea8f5c3);
        } else {
            d3_SetObjectPos(d3_Camera, c + 0x8c);
            sv_PlaceCamera(*(int *)(c + 0x88));
            *(int *)&d3_Camera->rotation.y = *(int *)(c + 0xa0);
            d3_SetObjectRot(d3_Camera, &d3_Camera->rotation);
        }
        for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < (int)((unsigned)&ai_Unk770cec + 0x65418); p += 0x87) {
            if (*p != 0) {
                if (d3t_SceneIndex(*(int *)(*p + 0x88)) == 0 && *(int *)(*p + 0x2c) == -1) {
                    int o = *p;

                    if (*(int *)(*(int *)(o + 0x34) + 0x1cc) == 0
                        || (*(int *)(o + 0x22c) != 0 && *(int *)(*(int *)(o + 0x22c) + 0x1cc) == 0))
                        ch_InflateModel(o);
                    ch_SetVisible(*p, 1);
                    gm_SelectModelVariant(p - 0x61);
                }
            }
        }
        if (phase == 1)
            init_TextureSet = 1;
        else if (phase == 0)
            init_TextureSet = 0;
        else if (phase == 2)
            init_TextureSet = 2;
        else if (phase == 3)
            init_TextureSet = 3;
        d3_ForEachObject2(d3_DefaultScene, 0, 0xe0, init_PlantTextureSetHook);
        d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_TextureSetHook);
        if (d3_DefaultScene != 0) {
            int hook = *(int *)((char *)d3_DefaultScene + 0x3c8);

            if (hook != 0 && *(void (**)())(hook + 0x210) != 0)
                (*(void (**)())(hook + 0x210))(d3_DefaultScene, 4, hook + 0x214, 0);
        }
        init_SetSeasonTextures();
        init_SetDetailLevel((int)d3_Terrain, 0);
        d3_FogByDistance = 1;
        evt_NoGroup = 0;
        d3o_FogScale = 1.0f - (float)(ai_GfxFog.word & 0xff) * 0.01f;
        sv_UpdateSkyPhase(&ai_Time);
        if (d3if_Unk6339db != 0) {
            d3if_Prerender(0);
            return;
        }
        d3if_Prerender(1);
    }
}
/* 0x513970, splits the comma separated list s into at most n numbers (atoi of each part) in out; returns the count */


int init_ParseNumberList(char *s, int *out, int n)
{
    int done, cnt;
    char *c;
    char buf[0x100];

    cnt = 0;
    done = 0;
    c = strchr(s, ',');
    if (0 < n) {
        do {
            memset(buf, 0, 0x100);
            strncpy(buf, s, c - s);
            out[cnt] = atoi(buf);
            cnt++;
            if (done)
                return cnt;
            s = c + 1;
            c = strchr(s, ',');
            if (c == 0) {
                c = strchr(s, 0);
                done = 1;
            }
        } while (cnt < n);
    }
    return cnt;
}
/* 0x513a20, reads the city file "<gfx dir><cities dir>/<name>.ini" (sections "A - ALLGEMEIN", "A - EINWOHNER", "A - PRUNK", "B - WETTER", "C - PRIVILEGIEN", "E - UMLAND", "F - GESETZE", "KONTOR") into the town slot `slot` of the table 0x7e3f40 (0x2f4 bytes each): the name "Stadtname" (+0), the map position "KartenPosition" (+0x40, two numbers), the faith "Glaube" (+0x49), the history "HistorieStart" / "HistorieEnde" (+0x4c / +0x50), the currency "Waehrung" (+0x54, an object name 3 characters in), "Land" (+0x60) and "Sprache" (+0x61), the map offset "KartenOffset" (+0x2ec), "MaxPlayer" (+0x48), the ten "Einwohner_%i" / "Prunk_%i" pairs (+0x64 / +0xb4), the weather chances (+0x108 / +0x118 four numbers, +0x128 the freezing chance), the eleven privileges (+0x12c), the eight "Umland%i" entries (+0x145: type, number, up to eight object ids), the six law lists of 12 numbers (+0x1dc .. +0x290), and the "Import" / "Export" lists of up to 16 goods (+0x2ac / +0x2cc); slot 0 additionally clears the whole table first and loads the neighbour cities "NachbarStadt" (up to 8, recursive with the slot numbers 1 ..). Returns 1. @deep readability advanced, Q4 open, far draft: Q2 (unk_Unk513970 -> init_ParseNumberList), Q5 (the ROUGH marker replaced by the state) and Q7 (int gm_FindBuildingKind(char *) 0x502060) cleared. The body was read against the section list: GetPrivateProfileStringA of A - ALLGEMEIN (Stadtname / KartenPosition / Glaube / HistorieStart / HistorieEnde / Waehrung / Land / Sprache / KartenOffset / NachbarStadt / MaxPlayer), A - EINWOHNER + A - PRUNK (10 pairs + 10 pairs), B - WETTER (regen / schnee / zufrieren), C - PRIVILEGIEN (11), E - UMLAND (8 entries of kind / number / 8 ids), F - GESETZE (6 lists of 12) and KONTOR (import / export, 16 shorts) matches the draft's calls and keys. Left: 537 code hunks / 292 slot hunks / 425 differing instructions - the draft's city record offsets overlap in the six law lists (0x1dc, 0x20c, 0x22c, 0x25c, 0x274, 0x290: finLaw 0x20c+0x30 crosses penalLaw 0x22c), so the record must be re-read from the disassembly before an INIT_CITY view can replace the 9 raw accesses - Q4 is the typed-view work on that verified layout. */


int init_LoadCityINI(char *city, int slot)
{
    char ini[260], val[256], key[512];
    int nums[64];
    char *s, *p, *dst;
    int i, k, j, n;
    char names[8][0x40];
    char sep[4];

    s = ai_PlayerSlots[slot];
    if (slot == 0)
        memset(ai_PlayerSlots[0], 0, 0x2f4 * 4);
    sprintf(ini, "%s%s/%s.ini", "\\project\\gfx\\", "gamedata/cities", city);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", val, 0x100, ini);
    strcpy(s, val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "KartenPosition", "0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x40), 2);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Glaube", "0", val, 0x100, ini);
    s[0x49] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "HistorieStart", "1400", val, 0x100, ini);
    *(int *)(s + 0x4c) = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "HistorieEnde", "1400", val, 0x100, ini);
    *(int *)(s + 0x50) = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Waehrung", "ob_GOLDGULDEN", val, 0x100, ini);
    *(short *)(s + 0x54) = gm_FindObjectProto(val + 3);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Land", "0", val, 0x100, ini);
    s[0x60] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Sprache", "0", val, 0x100, ini);
    s[0x61] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "KartenOffset", "0, 0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x2ec), 2);
    if (slot == 0) {
        GetPrivateProfileStringA("A - ALLGEMEIN", "NachbarStadt", "", val, 0x100, ini);
        p = strtok(val, ",");
        n = 0;
        while (p != 0 && n <= 7) {
            while (*p == ' ')
                p++;
            strcpy(names[n], p);
            p = strtok(0, ",");
            n++;
        }
        for (i = 0; i < n; i++)
            init_LoadCityINI(names[i], i + 1);
    }
    GetPrivateProfileStringA("A - ALLGEMEIN", "MaxPlayer", "4", val, 0x100, ini);
    s[0x48] = atoi(val);
    for (i = 0; i < 10; i++) {
        sprintf(key, "Einwohner_%i", i);
        GetPrivateProfileStringA("A - EINWOHNER", key, "1400,0", val, 0x100, ini);
        init_ParseNumberList(val, nums, 2);
        *(int *)(s + 0x64 + i * 8) = nums[0];
        *(int *)(s + 0x68 + i * 8) = nums[1];
    }
    for (i = 0; i < 10; i++) {
        sprintf(key, "Prunk_%i", i);
        GetPrivateProfileStringA("A - PRUNK", key, "1400,0", val, 0x100, ini);
        init_ParseNumberList(val, nums, 2);
        *(int *)(s + 0xb4 + i * 8) = nums[0];
        *(int *)(s + 0xb8 + i * 8) = nums[1];
    }
    GetPrivateProfileStringA("B - WETTER", "Regenwahrscheinlichkeit", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x108), 4);
    GetPrivateProfileStringA("B - WETTER", "Schneewahrscheinlichkeit", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x118), 4);
    GetPrivateProfileStringA("B - WETTER", "Zufrierenwahrscheinlichkeit", "0", val, 0x100, ini);
    *(int *)(s + 0x128) = atoi(val);
    GetPrivateProfileStringA("C - PRIVILEGIEN", "Privilegien", "0,0,0,0,0,0,0,0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, nums, 0xb);
    for (i = 0; i < 0xb; i++)
        s[0x12c + i] = ((char *)nums)[i];
    dst = s + 0x145;
    k = 0;
    j = 0xa1;
    do {
        sprintf(key, "Umland%i", k);
        GetPrivateProfileStringA("E - UMLAND", key, "", val, 0x100, ini);
        p = strtok(val, ",");
        for (i = 0; p != 0 && i < 10; i++) {
            while (*p == ' ')
                p++;
            if (i == 0)
                dst[-1] = gm_FindBuildingKind(p + 3);
            else if (i == 1)
                *dst = atoi(p);
            else
                *(short *)(s + (i + j) * 2) = gm_FindObjectProto(p + 3);
            p = strtok(0, ",");
        }
        j += 9;
        k++;
        dst += 0x12;
    } while (j < 0xe9);
    GetPrivateProfileStringA("F - GESETZE", "Verfassungsgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x1dc), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Finanzgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x20c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Strafgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x22c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Gildengesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x25c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Kirchengesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x274), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "DiebeRaeubergesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x290), 0xc);
    strcpy(sep, ",");
    GetPrivateProfileStringA("KONTOR", "Import", "", val, 0x100, ini);
    p = strtok(val, sep);
    i = 0;
    if (p != 0) {
        short *q = (short *)(s + 0x2ac);

        do {
            if (i > 0xf)
                break;
            while (isspace((unsigned char)*p))
                p++;
            *q = gm_FindObjectProto(p + 3);
            p = strtok(0, sep);
            i++;
            q++;
        } while (p != 0);
    }
    strcpy(sep, ",");
    i = 0;
    GetPrivateProfileStringA("KONTOR", "Export", "", val, 0x100, ini);
    p = strtok(val, sep);
    if (p != 0) {
        short *q = (short *)(s + 0x2cc);

        do {
            if (i > 0xf)
                return 1;
            while (isspace((unsigned char)*p))
                p++;
            *q = gm_FindObjectProto(p + 3);
            p = strtok(0, sep);
            i++;
            q++;
        } while (p != 0);
    }
    return 1;
}

/* init_StatusSendGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* init_StatusReceiveGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* init_StatusSetupGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* init_StatusLocalData: view of txt_Ptrs (build/mk_txt_views.py) */
