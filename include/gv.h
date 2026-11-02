struct D3PARTSYS;
struct D3COLOR;
struct D3SM;
struct D3VECTOR;
struct SNDHANDLE;
struct SHOTREC;
struct AIBLD;
struct AICHAR;
struct GVSHOTMSG;
struct VFSFILE;
struct D3OBJECT;
/* Declarations of gv.c (0x4ed170.., the avatar table 0x60f310 and the effect slots 0x6f1008). Names are ours. */
#ifndef GV_H
#define GV_H
#include "ai_types.h"

extern char unk_Avatars[0x20][0xf4];    /* 0x60f310 */
extern char gv_Effects[];             /* 0x6f1008, 8 slots of 0x170 bytes */
extern AITIME gv_LastTime;              /* 0x6f0e38, ai_Time at the last effect step (struct copies of it) */
typedef struct GVGRAVE {                /* 0x44 bytes, a dead person in a grave (GVYARD.graves; filled by gv_BuryNewDead) */
    int id;                             /* 0x00, the object id (ai_NextObjectId), -1 when free */
    char first[0x10];                   /* 0x04, the first name (txt_MaleFirstNames / txt_FemaleFirstNames by sex) */
    char family[0x10];                  /* 0x14, the family name (txt_FamilyNames) */
    unsigned char traits[5];            /* 0x24, five values 42 .. 189 (random 0 .. 7 * 21 + 42) */
    char pad29[3];
    int v2c;                            /* 0x2c, 1 .. 9 */
    int v30;                            /* 0x30, 1 .. 2 */
    int nameId;                         /* 0x34, 1 .. 0x5e, the id of its gm_NameRecord */
    int sex;                            /* 0x38, 0 male, 1 female (picks the first-name list and the gm_NameRecord table) */
    int icon;                           /* 0x3c, 0x639 + one of the name record's four words +0x34 (0xff: none) */
    int year;                           /* 0x40, a year 1 .. 4 before the burial (ai_Time.year), a guess: the year of death */
} GVGRAVE;
typedef struct GVENTRY {                /* 0x14 bytes, an entry of a graveyard's lists (gv_AllocGraveyard) */
    int size;                           /* 0x00, the list (grave size) 0 .. 3 it belongs to */
    int id;                             /* 0x04, -1 when free */
    unsigned short w08;                 /* 0x08, 0xffff when free (0x7fff for a new dead person) */
    unsigned short until;               /* 0x0a, ai_Time.year + 5 at the burial (a guess: the year the grave runs out) */
    unsigned char b0c;                  /* 0x0c, 0xff for a new dead person */
    char pad0d[3];
    GVGRAVE *grave;                     /* 0x10, its grave record (0x44 << size bytes) in GVYARD.graves */
} GVENTRY;
typedef struct GVYARD {                 /* 0x24 bytes, the graveyard record of a kind 0x1d building (AIBLD.yard; gv_AllocGraveyard) */
    int id;                             /* 0x00, the building's id */
    int kind;                           /* 0x04, 1 .. 3, the building type's level when allocated */
    GVENTRY *lists[4];                  /* 0x08, the entries (0x14 bytes) of the four grave sizes (0x44 << i bytes each) */
    unsigned char open[4];              /* 0x18, the size i is in use (open[0] is set at allocation; 0x19 .. 0x1b by expansion) */
    GVENTRY *entries;                   /* 0x1c, all entries ("gv_Alloc:_all_graves") */
    GVGRAVE *graves;                    /* 0x20, the grave records, first word -1 when free ("gv_Alloc:_all_zombies") */
} GVYARD;
#pragma pack(push, 1)
typedef struct GVORDER {                /* 0x1f bytes, a graveyard order: the data of message 0x60 (cm_SendGraveOrder), executed by type in cm.c's handler 0x4cce10 */
    int type;                           /* 0x00, 0 .. 6 (0 gv_BuryCharacter, 1 a grave reserved for a living character 0x4f0490, ..) */
    int chr;                            /* 0x04, the character's netId (AICHAR.netId), -1 for none */
    int bld;                            /* 0x08, the graveyard building's id (AIBLD.id) */
    int size;                           /* 0x0c, the grave size (GVYARD.lists index) */
    unsigned char entry;                /* 0x10, the entry of that list, 0xff for none */
    short years;                        /* 0x11, type 1: years added to ai_Time.year for the entry's GVENTRY.until */
    int price;                          /* 0x13, type 1: the price (> 0: the owner's money changes) */
    int objId;                          /* 0x17, ai_NextObjectId, filled by the handler when the game is networked */
    int seed;                           /* 0x1b, a rand() the handler fills otherwise */
} GVORDER;
#pragma pack(pop)
typedef struct GVCANNON {               /* 0x20 bytes, the state of the cannon scene (unk_575860 0x57e270 ..), saved with the effects */
    int bld;                            /* 0x00, the record id (rec + 1) the scene was last set up for, -1 after the reset */
    int spare;                          /* 0x04, no reader besides the save game */
    int camTarget[3];                   /* 0x08, the camera ints kept from the camera +0x84 .. +0x8c */
    int sliderA;                        /* 0x14, the first panel slider (0 .. 4) */
    int sliderB;                        /* 0x18, the second panel slider (0 .. 100) */
    int selected;                       /* 0x1c, the selected object (the scene's +0xa0) */
} GVCANNON;
extern GVCANNON gv_Cannon;              /* 0x925100 */
extern char ai_Time[];             /* 0x770ae0 */
extern int ai_TimeYear;            /* 0x770ae0, ai_Time's first int: the game year (AITIME.year in ai.h;
                                      the alias am.h / cm.h use for AITIME.hour is ai_TimeHour) */
extern char ai_Chars[][0x21c];      /* 0x770b80, the characters */

/* the 0x21c-byte character records (ai_Chars) as the order handlers read them */
typedef struct ORDCHAR {
    unsigned short id;              /* 0x000 */
    unsigned char kind;             /* 0x002, 6 = player */
    char pad003;
    unsigned int netId;             /* 0x004, the id the order events carry */
    unsigned char health;           /* 0x008, > 0 = the record is in use */
    unsigned char textVariant;      /* 0x009, nonzero: the alternate text block (level texts +10, job texts +0x28) */
    char pad00a[3];
    unsigned char level;            /* 0x00d, the level the level orders test */
    char pad00e[0x4e];
    int partner;                    /* 0x05c, the partner character id, -1 = none */
    char pad060[0x106];
    unsigned char job;              /* 0x166, the profession id the job orders test (text 0x253 + job) */
    char pad167[2];
    unsigned char job2;             /* 0x169, the second profession field, tested by the same orders */
    char pad16a[2];
    struct ORDBLD *bld;             /* 0x16c, the character's building (ai.h AICHAR.bld) */
    struct ORDBLD *bld2;            /* 0x170, the character's own building (its home), 0 when none */
    char pad174[0x54];
    int flags1c8;                   /* 0x1c8, order flags: 0x100 answer, 0x200 / 0x80000 / 0x100000 / 0x200000 / 0x400000 */
    int flags1cc;                   /* 0x1cc, privilege flags: 0xe00 */
    char pad1d0[0x21c - 0x1d0];
} ORDCHAR;
#define ORD_CHARS ((ORDCHAR *)ai_Chars)
#define ORD_CHAR(p) ((ORDCHAR *)(p))   /* a raw character record as the order handlers read it */

/* the wage record of gm_CharacterWages (0x4fa700): the two profession ids, the wage kind and the three amounts */
typedef struct ORDWAGES {
    unsigned char job;              /* 0x000, the profession id (character +0x166) */
    unsigned char job2;             /* 0x001, the second profession id (character +0x169) */
    unsigned char kind;             /* 0x002, the wage kind 0x23 .. 0x27 */
    char pad003;
    int amount;                     /* 0x004, the wage of the first profession */
    int amount2;                    /* 0x008, the wage of the second profession */
    int amount3;                    /* 0x00c, the wage of the kind */
} ORDWAGES;

/* the message record of an order (c = the character, m = this record): the handlers write their text at +0x14 */
typedef struct ORDMSG {
    int kind;                       /* 0x000, 1 do / 2 answer / 3 ask / 4 answer */
    char pad004[0x10];
    char text[0x200];               /* 0x014, the text the handlers fill */
    int answerFlag;                 /* 0x214, 1 = the answer handler picked this order; stored as an int */
    char pad218[4];
    ORDCHAR *partner;               /* 0x21c, the partner character of an answer */
} ORDMSG;
#define ORD_MSG(p) ((ORDMSG *)(p))

/* a cell of the statistics grid ai_Grid (ai.h, 0x91ae60) as ord.c reads it: the same 24-byte record, which the
   order statistics use beyond AIGRID's a / b / used (+8 the average value of the objects of the cell, +0xc /
   +0x10 / +0x14 the float sums 0x4f8920 rebuilds); kept here because gv.h and ai.h cannot be in one TU */
typedef struct ORDCELL {
    unsigned short a;               /* 0x00, the sum over the 3 x 3 neighbourhood */
    unsigned short b;               /* 0x02, the sum over the cell itself */
    unsigned char used;             /* 0x04, the number of objects of the cell (read unsigned: and eax,0xff) */
    char pad05[3];
    int average;                    /* 0x08 */
    float halfWeightSum;            /* 0x0c, the sum of half the weights w of 0x4f8920 (w * 0.5f) */
    float weightSum;                /* 0x10, the sum of the weights w of 0x4f8920 */
    float buildingCount;            /* 0x14, 0x4f8ae0 adds 1.0 per building object of the cell */
} ORDCELL;

#pragma pack(push, 1)
/* the building record ai.h's AIBLD, as ord.c reads it (gv.h and ai.h cannot be in one TU: both declare
   ai_Time / ai_Chars / ai_BTypes); the object list of gm_BuildQuery's records */
typedef struct ORDBLD {
    unsigned char type;             /* 0x00, index into ai_BTypes */
    char pad01[0x26];
    unsigned short owner;           /* 0x27, index into ai_Chars, 0xffff = none */
    char pad29[0x33];
    char level;                     /* 0x5c, the building's level 0 .. 100 */
    int objList;                    /* 0x5d */
    char *object;                   /* 0x61, its object in the scene */
    char pad65[0x44];
} ORDBLD;
#pragma pack(pop)

/* a record of the event list (he_FindEvent / he_NextEvent) as the order handlers read it */
typedef struct ORDEVENT {
    char pad000[0xac];
    int ownerId;                    /* 0x0ac, the character the event belongs to */
    int targetId;                   /* 0x0b0, the event's other character */
} ORDEVENT;
extern int ai_ClockMs;                /* 0x6333cc */
extern int d3_CurrentScene;                /* 0x633444 */

int util_Random(unsigned short n);      /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
int gm_MinutesBetween();                     /* 0x4fe280 */
struct D3PARTSYS *d3t_CreateExplosion(void *parent, struct D3VECTOR *pos, int count, float radius, float height, float z, float size, unsigned int duration); /* 0x416040, d3_tools.c */
struct SBSAMPLE; struct SBBANK;
struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);   /* 0x426030 */
int gm_FindById();                     /* 0x4fea50 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void cm_BeginFieldBlock(char *o, int id);                     /* 0x4c3ba0 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
void cm_SendFieldDeltas(void);                     /* 0x4c49a0 */
extern char *ai_Blds;                   /* 0x889f74, 256 records of 0xa9 bytes */
extern unsigned short *ai_PlayerChar;    /* 0x6f1bf8, the player's character (ai.c; this header's units add byte offsets through ai_PlayerCharC) */
extern const float ai_Unk5ebba4[];          /* 0x5ebba4, d3_Origin (as cm.h) */
extern const char ai_ZeroRot[];                 /* 0x5ebbb4 */

char *gv_AllocAvatar(void);              /* 0x4ed170 */
int gv_WriteAvatars(void *f);             /* 0x4ed1e0 */
int gv_ReadAvatars(void *f);             /* 0x4ed270 */
typedef struct GVEFFPART {              /* 0x68 bytes, one of an effect slot's three parts (saved field by field) */
    float v0[4];                        /* 0x00 */
    float v1[4];                        /* 0x10 */
    float v2[4];                        /* 0x20 */
    float v3[4];                        /* 0x30, not saved */
    float v4[4];                        /* 0x40 */
    int a, b, c;                        /* 0x50 */
    int state;                          /* 0x5c, the part is a SHOTREC (unk_5d33b0.h): this is its state, 0 idle, 1 in flight, 2 landed, 3 left the map (gv_UpdateEffects) */
    int time;                           /* 0x60, not saved: the reader sets it to ai_ClockMs */
    int steps;                          /* 0x64, the flight steps made (SHOTREC.steps) */
} GVEFFPART;
typedef struct GVEFFREC {               /* 16 bytes, a record of an effect slot's argument */
    int bldId;                          /* 0x00, a building id (gm_BuildQuery(1, 1, id)), -1: none */
    float dist;                         /* 0x04, the building's distance from the explosion (gv_ExplodeShot) */
    int mode;                           /* 0x08, 1: the explosion lay inside the building's box, 2: near it (gv_ExplodeShot); 1: the message depends on value */
    float value;                        /* 0x0c, the explosion's strength at the building (0 .. 100), compared with 25 */
} GVEFFREC;
typedef struct GVEFFARG {               /* 0x38 bytes, the argument of an effect slot's default callback (gv_EffectArgs, one per slot) */
    int cur;                            /* 0x00, the record at hand (0 .. 2) */
    int effectId;                       /* 0x04, the effect slot's id (gv_ExplodeShot) */
    GVEFFREC recs[3];                   /* 0x08 */
} GVEFFARG;
typedef struct GVEFFECT {               /* 0x170 bytes, an effect slot of gv_Effects (8) */
    int id;                             /* 0x000, -1: free */
    int flags;                          /* 0x004, 4: to be released (gv_ReleaseEffect) */
    union {
        struct {
            float f08, f0c, f10;        /* 0x008, 1.0 after the reset; the per-frame step adds dt up to 1.0 */
        };
        float charge[3];                /* 0x008, by the salvo's mode (0 .. 2): a received salvo takes its value off (0x4eee00, not below 0) */
    };
    int f14;                            /* 0x014 */
    GVEFFPART parts[3];                 /* 0x018 */
    int tail[3];                        /* 0x150, 0: the part is free (gv_UpdateEffects clears it when the part ends) */
    struct D3OBJECT *objs[3];           /* 0x15c, the parts' "kugel" objects (gv_UpdateEffects creates them, gv_FreeEffectObjects frees them) */
    int (*cb)(GVEFFARG *arg);           /* 0x168, the slot's callback (gv_EffectNotifyOwner by default), called when a part ends */
    GVEFFARG *arg;                      /* 0x16c, its argument: cur = the part that ended */
} GVEFFECT;
typedef char gv_AssertGVEFFECT[sizeof(GVEFFECT) == 0x170 ? 1 : -1];
extern GVEFFARG gv_EffectArgs[8];      /* 0x6f0e48, one per effect slot */
int gv_EffectNotifyOwner(GVEFFARG *a);  /* 0x4ed3f0, the default effect slot callback: tells the owner of the current record's building */
int gv_WriteEffects(void *f);             /* 0x4ed480 */
int gv_ReadEffects(void *f, int v);      /* 0x4ed910 */
void unk_ResetEffectSlots(void);               /* 0x4eddf0 */
void gv_FreeEffectObjects(void);               /* 0x4ede70 */
char *gv_FindEffect(int id);            /* 0x4eded0 */
void gv_ReleaseEffect(int id);             /* 0x4edf10 */
void gv_ExplodeShot(int damage, int effectId, struct SHOTREC *shot, GVEFFARG *out);  /* 0x4ee360 */
void gv_UpdateEffects(void);               /* 0x4edf80 */
void unk_Unk4eea70(char *o);             /* 0x4eea70 */
int gv_ReserveEffect(int id);              /* 0x4eeaa0 */
int gv_StopEffect(int id);              /* 0x4eeb00 */
int gv_FireShots(struct AIBLD *bld, struct AICHAR *ch, struct SHOTREC *a, struct SHOTREC *b, struct SHOTREC *d, float v, int cost, int x);  /* 0x4eeb70 */
char *gv_GetEffect(int id);            /* 0x4eedd0 */
int gv_StartShot(struct GVSHOTMSG *m, int w);       /* 0x4eee00 */
void unk_Unk4eeee0(int id, void *cb, void *arg);  /* 0x4eeee0 */
float gv_SalvoCharge(int a, int b, int c);  /* 0x4eef50 */
void gv_CannonKind(int n, int *out);    /* 0x4ef000 */
int gv_Accuse();                     /* 0x4f2e30 */
void cm_SendMoneyDelta(int a, int b);                     /* 0x4c5f60 */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
void gv_BuildingCannon(char *o, unsigned *balls, unsigned *kind);  /* 0x4ef170 */
void gv_CannonScatter(char *o, float *spread, float *speedSpread);  /* 0x4ef260 */
float unk_Unk4ef330(char *o);           /* 0x4ef330 */
int gv_GraveCount(unsigned char *o);    /* 0x4ef3c0 */
int unk_Unk4ef440(unsigned char *o);    /* 0x4ef440 */
int gv_EntryCount(unsigned char *o);    /* 0x4ef470 */
int gv_SizeOpen(char *r, int i);      /* 0x4ef4f0 */
extern char *ai_BTypes;              /* 0x7e3f30 */
int gv_ListEntry(char *r, int i, unsigned char n);  /* 0x4ef570 */
int gv_EntryIndex(char *r, int *e);     /* 0x4ef6b0 */
int unk_Unk4ef710(char *a, char *b);    /* 0x4ef710 */
int gv_EntrySameId(int *a, int *b);      /* 0x4ef740 */
int gv_EntryIsFree(int *a, int *b);      /* 0x4ef760 */
int gv_EntryIsUsed(int *a, int *b);      /* 0x4ef770 */
int gv_EntryHasId(int *a, int *b);      /* 0x4ef780 */
unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l);  /* 0x4ef7c0 */
unsigned gv_LoadGraveyard(unsigned char *o, int *e);  /* 0x4ef900 */
GVGRAVE *gv_RandomGrave(unsigned char *o);/* 0x4ef930 */
extern const int gv_EntryCounts[];             /* 0x5ee054, entries per kind */
extern const int gv_ListEntries[4][4];         /* 0x5ee084 */
int gv_BuryNewDead(unsigned char *o, int n);  /* 0x4ef9c0 */
GVYARD *gv_AllocGraveyard(unsigned char *o);  /* 0x4efc10, allocates the graveyard record of the building o ("gv_AllocGraveyardField", at +0x65 of o) */
extern const int gv_EntryTotals[];             /* 0x5ee064 */
extern const int gv_GraveTotals[];             /* 0x5ee074 */
extern int ai_NextObjectId;                /* 0x61a654 */



void * mem_Alloc(int size, char *tag);        /* 0x4223c0, allocates zeroed memory */
void mem_Free(void *p);                 /* 0x422620 */
GVYARD *gv_GrowGraveyard(unsigned char *o);  /* 0x4eff50 */
void gv_FreeGraveyard(int *g);             /* 0x4f0180 */
int unk_Unk4f01d0(int *g, char *o, int v, int b);  /* 0x4f01d0 */
int gv_BuryCharacter(int *p);              /* 0x4f0240 */
int gv_OrderReserveGrave(int *c, char *o, int size, short years, int price);  /* 0x4f0430 */
int gv_ReserveGrave(GVORDER *m);          /* 0x4f0490 */
int unk_Unk4f0700(int *g, char *o, int v, int b, short s, int x);  /* 0x4f0700 */
int gv_ExtendGrave(GVORDER *m);           /* 0x4f0770 */
int unk_Unk4f0900(int *g, char *o, int v, int b);  /* 0x4f0900 */
int unk_Unk4f0970(unsigned char *p);    /* 0x4f0970 */
int unk_Unk4f0a90(int *g, char *o, int v);  /* 0x4f0a90 */
int gm_AddNegative();                     /* 0x500ae0 */
int gm_AddObjekt(int id, int proto, int n);                     /* 0x500970 */
int unk_Unk4f0af0(char *p);             /* 0x4f0af0 */
int unk_Unk4f0c20(char *o, int v, int b, int x);  /* 0x4f0c20 */
int unk_Unk4f0c80(unsigned char *p);    /* 0x4f0c80 */
int unk_Unk4f0d70(char *o, int x);      /* 0x4f0d70 */
int unk_Unk4f0dd0(unsigned char *p);    /* 0x4f0dd0 */
void unk_Unk4f0e80(int *g, int x, char *o, int v, char b, int y);  /* 0x4f0e80 */
int unk_Unk4f0ed0(int *p);              /* 0x4f0ed0 */
int unk_Unk4f1170(char *o, int v, int b, int x, int y);  /* 0x4f1170 */
int gv_SendGrave(unsigned char *o);    /* 0x4f15e0 */
int unk_Unk4f1660(char *c);             /* 0x4f1660 */
int unk_Unk4f1d50(unsigned char *o, int *n);  /* 0x4f1d50 */
float unk_Unk4f16f0(char *o);           /* 0x4f16f0 */
void ai_Unk4c4ac0(int a, int b, int c);                     /* 0x4c4ac0 */
extern int ai_Unk1460ae0;               /* 0x1460ae0 */
int unk_Unk4f17f0(void *f, unsigned char *o);  /* 0x4f17f0 */
char * gm_Query(void *list, int n, ...);                   /* 0x4ffe90 */
unsigned unk_Unk4f1200(char *p);        /* 0x4f1200 */
int gv_ReadGraveyard(void *f, unsigned char *o);  /* 0x4f1aa0, reads the graveyard record of o from the file f (allocated by 0x4efc10) */
float util_RandomF(void);               /* 0x42ec20 */
extern int ai_CharCount;                /* 0x6f1bc0, the number of characters in use (of ai_Chars[0x300]) */
int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5);                     /* 0x4fbd10 */
int gm_ChooseHeadIcon(unsigned short *c);                     /* 0x4fb890 */
int unk_Unk4f22a0(char *o, unsigned char m);  /* 0x4f22a0 */

int unk_Unk4f2540(void *a, unsigned char **o, int *e);  /* 0x4f2540 */
int unk_Unk4f25d0(void *a, unsigned char **o, int *e);  /* 0x4f25d0 */
float unk_Unk4f2660(char *o);           /* 0x4f2660 */
int gv_GetTableRecord(int i, int *out);     /* 0x4f2720 */
int gv_FindTableRecord(char *c, unsigned b, int *out);  /* 0x4f2870 */
extern char unk_Tab611258[];            /* 0x611258, 0x1d records of 0x24 bytes */

int gv_WriteGraveyard(void *f);             /* 0x4f28d0 */
float unk_Unk4f2da0(short *c);          /* 0x4f2da0 */
extern char unk_Var6f1b88[];            /* 0x6f1b88 */
extern char gv_Graveyard[];            /* 0x91f700 */
extern char unk_Tab91b6e0[];            /* 0x91b6e0 */
int gm_BuildingSkill();                     /* 0x5092e0 */

int unk_Unk4f2b30(void *f);             /* 0x4f2b30 */
int gv_TableRecordText(char *out, int i);    /* 0x4f2fe0 */

void unk_Unk4f3610(int v, int *out);    /* 0x4f3610 */
int unk_Unk4f3640(int b, int a);        /* 0x4f3640 */
int unk_Unk4f36b0(void);                /* 0x4f36b0 */
int unk_Unk4f36e0(int id);              /* 0x4f36e0 */
int unk_Unk4f3780(int a, int b);        /* 0x4f3780 */

int unk_Unk4f3910(int a, int b, int s, int m);  /* 0x4f3910 */
int gv_CountOwnedGraves(int c);               /* 0x4f39b0 */
int unk_Unk4f39e0(int c);               /* 0x4f39e0 */

int unk_Unk4f3fe0(int p, int *out, unsigned t);  /* 0x4f3fe0 */
int unk_Unk4f4080(int p, int *out, unsigned t);  /* 0x4f4080 */
int unk_Unk4f4130(int p, unsigned t);   /* 0x4f4130 */
void gv_ClearGraveyard(void);               /* 0x4f4350 */
int unk_Unk4f4420(char *c, unsigned k); /* 0x4f4420 */
short * gm_FindChar(int id);                   /* 0x505df0 */

void unk_Unk4f4d10(void);               /* 0x4f4d10 */
int gv_GetGraveInfo(int id, short *a, int *v, short *c, unsigned *k);  /* 0x4f4d30 */
void unk_Unk4f4970(unsigned i);              /* 0x4f4970 */
int gm_Unk505e60(unsigned id, int again);                     /* 0x505e60 */
float gm_Liking();                   /* 0x50c550 */

void unk_Unk4f3700(void);               /* 0x4f3700 */
int unk_Unk4f37e0(int id, int m);       /* 0x4f37e0 */
void unk_Unk4f4390(int n, char *r);     /* 0x4f4390 */
char * he_FindEvent(int n, ...);         /* 0x53e400, as he.h */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void sv_PumpNet();                    /* 0x52fe00 */
extern unsigned short ai_PlayerId;     /* 0x6f25b8 */

int unk_Unk4f3a30(int p, int n);        /* 0x4f3a30 */
int unk_Unk4f3b40(int p, int n, int *ids);  /* 0x4f3b40 */

int gv_RankRecordOwners(int *ids, int *cnt, unsigned short t);/* 0x4f3c50 */

int unk_Unk4f3dd0(int *ids, int *c1, int *c2, unsigned t);  /* 0x4f3dd0 */

extern const int unk_Tab5ee0f8[];             /* 0x5ee0f8, experience thresholds per level */
extern unsigned char ai_NumPlayers;     /* 0x62489c, the number of players (as ai.h / cm.h / unk_4b39f0.c) */
int gm_GetWorth(unsigned short id);    /* 0x50a640 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void cm_BeginFieldBlock(char *o, int id);                     /* 0x4c3ba0 */
int cm_AddFieldDelta(int size, int n, short field, void *v);                     /* 0x4c3c00 */
void cm_SendFieldDeltas(void);                     /* 0x4c49a0 */
struct GCLOCK;  /* f3gm.h typedef (gm_AddTime); file-scope so the gm_AddTime prototype below is stable */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */

typedef struct { char b[0x2d]; } UNKREC;      /* a 0x2d-byte record of 0x91f700, passed by value */
int gv_CannonCost(char *o, int a, int b, int d, int *n, int *price);  /* 0x4ef090 */
int unk_Unk4f2440(unsigned short *c, int k1, int k2, int *out);  /* 0x4f2440 */
int unk_Unk4f24a0(unsigned short *c);  /* 0x4f24a0, returns gm_PlayerFactor's value (0x4f2532 ret after the 0x5085f0 call) */
int unk_Unk4f30a0(char *out, UNKREC r, int mode);  /* 0x4f30a0 */
int unk_Unk4f31d0(char *out, UNKREC r, int mode);  /* 0x4f31d0 */
int unk_Unk4f3270(char *out, UNKREC r, int mode);  /* 0x4f3270 */
int unk_Unk4f3580(char *out, UNKREC r, int mode);  /* 0x4f3580 */
char *gv_FindAccused(char *c, int max, int *ids, int *cnt);  /* 0x4f41b0 */
void unk_Unk4f4460(unsigned short *c, int a);  /* 0x4f4460 */
void unk_Unk4f48f0(unsigned short *c, int a, char *out);  /* 0x4f48f0 */

unsigned ord_ApplyForOffice(ORDCHAR *c, int *m);  /* 0x4f4e20 */
char * he_NextEvent(void);                   /* 0x53e300 */

int ord_LevelOrder(char *c, int *m);     /* 0x4f4f20 */
int ord_EspionageResultOrder(char *c, int *m);     /* 0x4f4ff0 */
int ord_PrivilegeDialog(char *text, int mode, int arg);  /* 0x4f8200, the privilege dialog */

int gv_SetTableRecord(short *c, int i, int v);  /* 0x4f2750 */
int unk_Unk4f27d0(int *p);              /* 0x4f27d0 */

int gv_Accuse(int i, int v, int a, int b, int obj);  /* 0x4f2e30 */
float util_RandomF(void);               /* 0x42ec20 */

void unk_Unk4f1de0(char *o);            /* 0x4f1de0 */
int unk_Unk4f2330(unsigned short *c, int a, int kind, int flag, int *out);  /* 0x4f2330 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
void cm_SendRelations(int a, int b, int c, int d);                     /* 0x4c4b00 */
int gm_Unk509ee0(char *c, char p);                   /* 0x509ee0 */

int unk_Unk4f5070(char *c, int *m);     /* 0x4f5070 */

int ord_SpyTargetOrder(char *c, int *m);     /* 0x4f50f0 */

int ord_BlackmailBuildingOrder(char *c, int *m);     /* 0x4f5190 */
int ord_BlackmailContactVariantOrder(char *c, int *m);     /* 0x4f52e0 */
int ord_BlackmailContactOrder(char *c, int *m);     /* 0x4f5350 */
int ord_EmbezzlementOrder(char *c, int *m);     /* 0x4f5c30 */
int ord_LawOrder(char *c, int *m);     /* 0x4f5d50 */
int ord_LawOrder2(char *c, int *m);     /* 0x4f5f00 */
int ord_Level3Order(char *c, int *m);     /* 0x4f5200 */
int ord_Level4Order(char *c, int *m);     /* 0x4f53c0 */
int ord_Level4Order2(char *c, int *m);     /* 0x4f54a0 */
int ord_Level4To5Order(char *c, int *m);     /* 0x4f5580 */
int ord_Level5UpOrder(char *c, int *m);     /* 0x4f5660 */
int ord_Level5UpOrder2(char *c, int *m);     /* 0x4f5730 */
int ord_Level6UpOrder(char *c, int *m);     /* 0x4f58e0 */
int ord_Level6UpOrder2(char *c, int *m);     /* 0x4f59b0 */

int ord_Job11Order(char *c, int *m);     /* 0x4f60b0 */
int ord_JobMultiOrder(char *c, int *m);     /* 0x4f6180 */
int ord_Job28Order(char *c, int *m);     /* 0x4f6270 */
int ord_Job29Order(char *c, int *m);     /* 0x4f6340 */
int ord_Job34Order(char *c, int *m);     /* 0x4f6410 */
int ord_JobRangeOrder(char *c, int *m);     /* 0x4f64e0 */
int ord_JobMultiOrder2(char *c, int *m);     /* 0x4f65c0 */

int ord_EmbezzlementContractOrder(char *c, int *m);     /* 0x4f66b0 */

int ord_Job17Order(char *c, int *m);     /* 0x4f68d0 */
int ord_Job17Order2(char *c, int *m);     /* 0x4f69a0 */
int ord_JobMultiOrder4(char *c, int *m);     /* 0x4f6a70 */
int ord_Job10Order(char *c, int *m);     /* 0x4f6c60 */
int ord_Job15Order(char *c, int *m);     /* 0x4f6d30 */
int ord_CounterEspionageOrder(char *c, int *m);     /* 0x4f67c0 */
int ord_MisappropriationOrder(char *c, int *m);     /* 0x4f6b40 */

int ord_CounterEspionageWindowOrder(char *c, int *m);     /* 0x4f6e00 */
int ord_EspionageWindowOrder(char *c, int *m);     /* 0x4f6f10 */
int ord_EspionagePaymentOrder(char *c, int *m);     /* 0x4f70f0 */
int ord_EspionageOrder(char *c, int *m);     /* 0x4f7200 */
int ord_EspionagePaymentOrder2(char *c, int *m);     /* 0x4f7300 */
int ord_EspionageRelationOrder(char *c, int *m);     /* 0x4f7400 */
int ord_EspionageAmountOrder(char *c, int *m);     /* 0x4f74f0 */
int ord_SpyTargetFlagOrder(char *c, int *m);     /* 0x4f75e0 */
int ord_JobMultiOrder3(char *c, int *m);     /* 0x4f7000 */

int ord_EspionageOverviewOrder(char *c, int *m);     /* 0x4f76e0 */
int ord_Level7Order(char *c, int *m);     /* 0x4f77d0 */
int ord_Level8Order(char *c, int *m);     /* 0x4f78b0 */
int ord_Level9UpOrder(char *c, int *m);     /* 0x4f7990 */
int ord_Flag800Order(char *c, int *m);     /* 0x4f7ca0 */
int ord_Flag800Order2(char *c, int *m);     /* 0x4f7d70 */
int ord_Flag200Order(char *c, int *m);     /* 0x4f7ef0 */
int ord_Flag400Order(char *c, int *m);     /* 0x4f8070 */
int ord_InterrogationOrder(char *c, int *m);     /* 0x4f7a60 */
int ord_DismissalOrder(char *c, int *m);     /* 0x4f7b80 */
int ord_InterrogationOrder2(char *c, int *m);     /* 0x4f7e40 */
int ord_InterrogationOrder3(char *c, int *m);     /* 0x4f7fc0 */
int ord_DismissWorkerOrder(char *c, int *m);     /* 0x4f8140 */

extern int (*ord_OrderHandlers[0x3a])(char *c, int *m);    /* 0x611768, the order handler of each order number */
int d2_OpenForm(int x, int y, char *name);  /* 0x4741f0, d2.h: loads the view, returns its number */
void d2_CentreViewWindows(int view);                     /* 0x475600 */
int d2_SetForm(int view, int n);                       /* 0x475f70 */
void d2_CloseView(int view);                     /* 0x4758f0 */
void d2_SetPointerSprite(int a, int pic);            /* 0x476f80, d2.h / gfx.h */
int sv_RunFrame(int cb, unsigned mask);                     /* 0x5309a0 */
extern int sv_PopRequest;                /* 0x6f22a0 */
extern int d2_ClickedId;                /* 0xf8a668 */
int ord_DispatchOrder(int id, char *c, int *m);  /* 0x4f81c0 */

int unk_Unk4f5800(char *c, int *m);     /* 0x4f5800 */
int ord_WageReportOrder(char *c, int *m);     /* 0x4f5a80 */
int gm_CharacterWages(unsigned short id, int flag, unsigned char *out);  /* 0x4fa700, the wage record (ORDWAGES) of the character id for the report */

/* the tables 0x4f82a0 clears */
extern char ord_MessageRecord[0x24];        /* 0x91b480, the 9-int record the messages carry, then the tag */
extern int ord_MessageRecordEnabled;    /* 0x91b480, the first int of ord_MessageRecord: 0x4f9710 sets it 1 / 0
                                           on the 'enbl' / 'dsbl' message, 0x4f9220 runs the event only at 1 (a
                                           separate name, so the compiler does not kill the record's clear) */
extern char gm_IncomeHistory[];         /* 0x91b580, the 17-round history of the five income sums (rows of
                                            0x44 bytes, the running sums behind them) */
extern char gm_CharacterStatistics[];            /* 0x91b500 */
extern char ord_StatisticsHistory[];            /* 0x91ab00 */
extern char ord_StatisticsRecord[];            /* 0x91b4c0 */
extern ORDCELL ord_GridCells[8][8];     /* 0x91ae60, ai.h's ai_Grid (the same address); see ORDCELL */
extern int ai_RatingBase;                /* 0x6f1b9c, the rating's old value (stored as bits; ai.h's ai_Rating follows it at 0x6f1ba0) */
extern float ord_RatingRate;             /* 0x91aaf4, the change rate of the rating (ord.c 0x4f8ed0; ls.c saves it) */
extern float ai_RatingBaseF;             /* 0x6f1b9c, ai_RatingBase read as the float its bits hold */
#ifndef DATA_VIEWS_H
extern char *unk_Ptrfd7c50;             /* 0xfd7c50, the scene */
#endif
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);                    /* 0x45f170 */
void ord_ResetStatistics(void);               /* 0x4f82a0 */
int ord_GetGridCell(char *p, int *x, int *y);  /* 0x4f8300, the grid cell (x, y) of the position p */

unsigned char * gm_BuildNext(void);                     /* 0x501030 */
int gm_FindById(int *bld, int *obj, int *chr, int id);  /* 0x4fea50, fills *bld with the object of the id (ai.h's prototype) */
int gm_EquipmentValue(unsigned char *b, int unused);                     /* 0x509b10 */
void ord_CountGridObjects(void);               /* 0x4f8360 */

char d3sm_GetVertex(struct D3SM *h, int x, int z, struct D3VECTOR *out);                              /* 0x45f100, d3_sm.c (this unit's callers read an int: d3sm_GetVertexI) */
extern char ord_KindTable[];            /* 0x617468, 0x58 pairs of ints */
int unk_Unk4f86a0(int x, int y, int z);  /* 0x4f86a0 */
int ord_GetKindRecord(int i, int *out);     /* 0x4f8890 */

void ord_AddGridAmount(int who, int kind, int *px, int *py);  /* 0x4f8460 */
void ord_SubGridAmount(int who, int kind);  /* 0x4f8590 */

int unk_Unk4f8700(char *c);             /* 0x4f8700 */

void unk_Unk4f88c0(void);               /* 0x4f88c0 */

extern char gv_Graveyard[], gv_GraveyardEnd[];  /* 0x91f700, 0x925100: 0x200 records of 0x2d bytes, and their end */
void ord_RebuildGridSums(void);               /* 0x4f8920 */

void ord_GetMessageRecord(int *out);           /* 0x4f9170 */
void ord_InitMessageRecord(int a, float b, float c, float d);  /* 0x4f9190 */

extern char ord_RatingBytes[];            /* 0x91b460, 5 + 1 bytes */
extern char *ai_Blds;             /* 0x889f74 */
void ord_ComputeStatistics(float *out);         /* 0x4f8ae0 */
void ord_UpdateStatistics(void);               /* 0x4f8ed0 */
void ord_LoadStatistics(void *p);       /* 0x4f9020 */
void ord_UpdateMessageEvent(void);               /* 0x4f9220 */
void ord_ReportStatistics(void);        /* 0x4f9850 */
int ord_HandleMessageRecord(int *m);              /* 0x4f9710 */
unsigned char ord_GetRatingByte(int i);              /* 0x4f97a0 */
float ord_BuildingCapacityRatio(void);              /* 0x4f97c0 */

float ord_StatisticsRating(void);              /* 0x4f9ce0 */
float ord_WeightedRecordSum(int *out);          /* 0x4f9d40 */
float ord_GetStatistics(int *o);            /* 0x4f9e00 */

extern char ord_StatisticsTime[];            /* 0x91b470, a clock stamp (14 bytes) */
int gm_MinutesBetween();                     /* 0x4fe280 */
void ord_AdvanceRatingBase(void);               /* 0x4f9c00 */

/* declared in other units' headers before (moved to the owner) */
extern struct AICHAR *ai_JudgeCatholic;  /* 0x6f1bfc */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 */

#endif
