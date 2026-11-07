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
struct CHARACTER;
struct G15PLANT;
/* Declarations of gv.c (0x4ed170.., the avatar table 0x60f310 and the effect slots 0x6f1008). Names are ours. */
#ifndef GV_H
#define GV_H
#include "ai_types.h"
#include "cm_types.h"                /* CRIME */
#include "gv_types.h"                /* GVENTRY, AIDEC, ORDSTATS, ORDSTATROUND, ORDMSGREC */

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
typedef struct GVMOVE {                 /* 0x19 bytes, a grave move: the data of message 0x61 (cm_SendGraveMove), run by gv_MoveGrave */
    int chr;                            /* 0x00, the paying character's netId (AICHAR.netId) */
    int bld;                            /* 0x04, the graveyard building's id (AIBLD.id) */
    int from;                           /* 0x08, the source grave size (GVYARD.lists index) */
    unsigned char entry;                /* 0x0c, the entry of that list */
    int to;                             /* 0x0d, the target grave size */
    int price;                          /* 0x11, > 0: the character pays it */
    int objId;                          /* 0x15, ai_NextObjectId when the game is networked */
} GVMOVE;
typedef struct GVRAISE {                /* 0x19 bytes, a raising of a dead person: the data of message 0x62 (cm_SendRaiseDead), run by gv_RaiseDead */
    int seed;                           /* 0x00, a rand() the handler 0x4ccf10 fills (util_Seed) */
    int objId;                          /* 0x04, ai_NextObjectId when the game is networked */
    int bld;                            /* 0x08, the graveyard building's id (AIBLD.id) */
    int size;                           /* 0x0c, the grave size (GVYARD.lists index) */
    unsigned char entry;                /* 0x10, the entry of that list */
    int grave;                          /* 0x11, the dead person's GVGRAVE.id */
    int kind;                           /* 0x15, 0x1b or 0x1c (the handler refuses others) */
} GVRAISE;
#pragma pack(pop)
typedef struct GVCANNON {               /* 0x20 bytes, the state of the cannon scene (sv_t 0x57e270 ..), saved with the effects */
    int bld;                            /* 0x00, the record id (rec + 1) the scene was last set up for, -1 after the reset */
    int spare;                          /* 0x04, no reader besides the save game */
    float camTarget[3];                 /* 0x08, the aim angles (pitch, yaw, roll) the scene keeps between visits: the camera's rotation (+0x84 .. +0x8c) at the first, the scene's rot after */
    int sliderA;                        /* 0x14, the first panel slider (0 .. 4) */
    int sliderB;                        /* 0x18, the second panel slider (0 .. 100) */
    int selected;                       /* 0x1c, the selected object (the scene's +0xa0) */
} GVCANNON;
extern GVCANNON gv_Cannon;              /* 0x925100 */
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern char ai_Chars[][0x21c];      /* 0x770b80, the characters */

/* the 0x21c-byte character records (ai_Chars) as the order handlers read them */
typedef struct ORDCHAR {
    unsigned short id;              /* 0x000 */
    unsigned char kind;             /* 0x002, 6 = player */
    char pad003;
    unsigned int netId;             /* 0x004, the id the order events carry */
    unsigned char health;           /* 0x008, > 0 = the record is in use */
    unsigned char textVariant;      /* 0x009, the sex (ai.h AICHAR.sex: 0 male, 1 female); nonzero: the alternate text block (level texts +10, job texts +0x28) */
    unsigned short w00a;            /* 0x00a, ai.h AICHAR.w00a; gm_ShiftLoyalty and gm_HandOutMoney skip the characters below 12 (reasoned guess: the age) */
    unsigned char b00c;             /* 0x00c, a kind (ai.h AICHAR.b00c); gm_HandOutMoney compares it between two characters and with the decision 2's value (reasoned guess: the faith) */
    unsigned char level;            /* 0x00d, the level the level orders test */
    char pad00e[2];
    float f010;                     /* 0x010, ai.h AICHAR.f010; the life value the daily step moves f01c toward while w00a is below f020 */
    float f014;                     /* 0x014, ai.h AICHAR.f014; the base of the daily life loss beyond f020 */
    float f018;                     /* 0x018, ai.h AICHAR.f018; gm_AgeCharacter ages the life from it */
    float f01c;                     /* 0x01c, ai.h AICHAR.f01c, the life value: the character dies when f01c + v024 is used up (gm_DailyCharacterStep) */
    float f020;                     /* 0x020, ai.h AICHAR.f020; a child (jobId 0) whose w00a is below it gets the child head (gm_CharNameRecord) */
    int v024;                       /* 0x024, ai.h AICHAR.v024, added to f01c by the daily step's death test */
    short w028;                     /* 0x028, ai.h AICHAR.w028 */
    char pad02a[2];
    unsigned int v02c;              /* 0x02c, ai.h AICHAR.v02c: nine counters of 2 .. 4 bits (bits 0, 4, 8, 12, 14, 17, 20, 23, 25) the daily step halves */
    union {
        char first[0x10];           /* 0x030, the first name (ai.h AICHAR.first) */
        unsigned int nameKey;       /* 0x030, its first four bytes as one number: gm_BirthDate's seed for the birthday */
    };
    char family[0x10];              /* 0x040, the family name (ai.h AICHAR.family) */
    unsigned short w050;            /* 0x050, ai.h AICHAR.w050; a player's character has the top bit set and its
                                       gm_PlayerRecords slot in the low 4 bits (gm_PlayerRecord) */
    char pad052[2];
    int v054;                       /* 0x054, gm_CreateObject: nonzero for a character of the kinds 5 .. 7, its low byte + 0x67 is the texture set of the character's carts */
    unsigned char generation;       /* 0x058, 1 for a character created without parents, else the parent's + 1
                                       (gm_CreateCharacter; reasoned guess: the generation) */
    char pad059[3];
    union {
        int partner;                /* 0x05c, the partner character id, -1 = none */
        int relatives[8];           /* 0x05c, the netIds of the relatives (ai.h AICHAR.relatives), -1 = none: [0] the
                                       partner, [1] / [2] the parents (gm_PopulationStep), [3..7] the children
                                       (reasoned guess: gm_PartnerStep counts them for the chance of another birth) */
    };
    float pregnancy;                /* 0x07c, above 1.0 the mother gives birth (gm_PartnerStep), set from the birth
                                       chance, reset below 0 after the birth (reasoned guess: the pregnancy) */
    unsigned char skills[5];        /* 0x080, the five skill bytes (ai.h AICHAR.skills; gm_GetSkill reads them as the base value) */
    char pad085[3];
    float mood[14][3];              /* 0x088, the mood triples (a, b, value) (ai.h AICHAR.mood) */
    char pad130[0x34];
    unsigned char jobId;            /* 0x164, the title / job id gm_TitleGroup groups (ai.h AICHAR.jobId) */
    char title;                     /* 0x165, the title rank (ai.h AICHAR.title) */
    unsigned char job;              /* 0x166, the profession id the job orders test (text 0x253 + job) */
    unsigned char prevOffice;       /* 0x167, ai.h AICHAR.prevOffice; gm_PopulationStep gives a newcomer 10 .. 21 */
    unsigned char appliedOffice;    /* 0x168, ai.h AICHAR.appliedOffice, 0 = none */
    unsigned char job2;             /* 0x169, the second profession field, tested by the same orders */
    char pad16a[2];
    struct ORDBLD *bld;             /* 0x16c, the character's building (ai.h AICHAR.bld) */
    struct ORDBLD *bld2;            /* 0x170, the character's own building (its home), 0 when none */
    int jobDays;                    /* 0x174, the days served in the current job: a job change clears it (gm_SetJob 0x504790), gm_TitlePeriodServed weighs it against the title period */
    int inventory;                  /* 0x178, the character's goods inventory, an object list (ai.h AICHAR.inventory) */
    char pad17c[8];
    struct CHARACTER *model;        /* 0x184, the character's model (ai.h AICHAR.model, ch.h CHARACTER) */
    char pad188[4];
    int picture;                    /* 0x18c, the portrait picture (ai.h AICHAR.picture), -1 = none */
    int v190;                       /* 0x190, 4 at creation, 10 with gm_AllowAll (ai.h AICHAR.v190) */
    int v194;                       /* 0x194, gm_TakeOverCharacter sets it to 4 for the character that takes over */
    int v198;                       /* 0x198, cleared with wage at the end of every day (f3gm.c gm_EndDayIncomes) */
    int wage;                       /* 0x19c, the wage paid in the last round settlement (ai.h AICHAR.wage) */
    int v1a0;                       /* 0x1a0, a trade sum cm.c's goods move adds to (price x amount) for an object of the type 0x1dc (seller) / 0x1db (buyer); the income kind 4 taxes v1a0 + v1a4 */
    int v1a4;                       /* 0x1a4, the same for the types 0x1db (seller) / 0x1dc (buyer) */
    int v1a8;                       /* 0x1a8, an amount the income kind 3 (f3gm.c gm_IncomeKind3) taxes at the decision 0xd's rate and clears once paid */
    int donated;                    /* 0x1ac, the sum the character gave the church (ai_IndulgenceWindow shows it with its rank, gm_DonationRank) */
    unsigned char b1b0;             /* 0x1b0, scales the chance that gm_HandOutMoney takes money from the character (x 0.25) */
    unsigned char b1b1;             /* 0x1b1, gm_PartnerStep takes no step unless it is 0 */
    char pad1b2[2];
    unsigned int flags1b4;          /* 0x1b4, bit 0 is cleared when the character's master building changes hands (gm_AssignBuilding) */
    char pad1b8[0x10];
    int flags1c8;                   /* 0x1c8, order flags: 0x100 answer, 0x200 / 0x80000 / 0x100000 / 0x200000 / 0x400000 */
    int flags1cc;                   /* 0x1cc, privilege flags: 0xe00 */
    float loyalty;                  /* 0x1d0, 0 .. 1, starts at 0.25 .. 0.75; gm_ShiftLoyalty / gm_HandOutMoney move it (reasoned guess: the loyalty) */
    int worth[4];                   /* 0x1d4, the character's worth (gm_GetWorth) of the last rounds, worth[3] the newest (gm_UpdateWorthMood shifts it) */
    float mood3Rate;                /* 0x1e4, scales the base growth of mood need 3 (ai.h AICHAR.mood3Rate) */
    int v1e8;                       /* 0x1e8, 1 at creation for the kinds 3 / 0x10 / 0x13 (gm_CreateCharacter) */
    char pad1ec[4];
    int v1f0;                       /* 0x1f0, gm_PopulationStep counts the working characters with it set; below 6, a
                                       newcomer of kind 0x10 may arrive */
    union {
        char ownName[0x1c];         /* 0x1f4, the character's own head name (ai.h AICHAR.ownName), empty: the job's / title's head record */
        struct {
            char padOwnName[0x18];
            int partyId;            /* 0x20c, the scene party (ai.h AICHAR.partyId), -1 at creation */
        };
    };
    int v210;                       /* 0x210, ai.h AICHAR.partner, -1 at creation */
    signed char unk214;             /* 0x214 */
    unsigned char b215;             /* 0x215, ai.h AICHAR.b215, the partner search's step counter */
    unsigned char b216;             /* 0x216, ai.h AICHAR.b216, the steps the partner search needs */
    unsigned char b217;             /* 0x217, ai.h AICHAR.b217: one of 0x3c .. 0x5c (or 0xc2) by the kind, the title group and chance (gm_CreateCharacter), a child takes a parent's */
    unsigned char mask;             /* 0x218, ai.h AICHAR.mask, 1 at creation */
    char pad219[3];
} ORDCHAR;
typedef char gv_AssertORDCHAR[sizeof(ORDCHAR) == 0x21c ? 1 : -1];

typedef struct GVAVATAR {               /* 0xf4 bytes, a record of the avatar table unk_Avatars (32) */
    unsigned id;                        /* 0x00, four characters ("neVm"), the key of the save game's (id, used) pairs */
    char name[0x10];                    /* 0x04, "Maven" (the read report prints it): the first name of the character made from it */
    char title[0x10];                   /* 0x14, "of Reading": its family name */
    unsigned char skills[5];            /* 0x24, the five skill bytes (ORDCHAR.skills) */
    char pad29[3];
    float mood[14][3];                  /* 0x2c, the mood triples (ORDCHAR.mood; gv_AllocAvatar sets the second of each to the first * 0.001) */
    unsigned char jobId;                /* 0xd4, ORDCHAR.jobId */
    unsigned char sex;                  /* 0xd5, ORDCHAR.textVariant */
    unsigned char b00c;                 /* 0xd6, ORDCHAR.b00c */
    char ownName[0x1b];                 /* 0xd7, ORDCHAR.ownName, the head name */
    unsigned char used;                 /* 0xf2, 1 while the record is in use (gv_AllocAvatar), 0 free */
    char padf3;
} GVAVATAR;
typedef char gv_AssertGVAVATAR[sizeof(GVAVATAR) == 0xf4 ? 1 : -1];
#define ORD_CHARS ((ORDCHAR *)ai_Chars)
#define ORD_CHAR(p) ((ORDCHAR *)(p))   /* a raw character record as the order handlers read it */

/* the message record of an order (c = the character, m = this record): the handlers write their text at +0x14 */
typedef struct ORDMSG {
    int kind;                       /* 0x000, 1 do / 2 answer / 3 ask / 4 answer */
    int law;                        /* 0x004, of the law orders: the decision record (AIDEC.id) the office may change */
    int value;                      /* 0x008, of the law orders asked of the AI: the value it wants (lc.c
                                       ai_LawPrivilegeWindow) */
    char pad00c[8];
    char text[0x200];               /* 0x014, the text the handlers fill */
    union {
        int answerFlag;             /* 0x214, 1 = the answer handler picked this order; stored as an int */
        int target;                 /* 0x214, of an ask that names a character (the courtship, lc.c ai_CourtshipWindow):
                                       its netId */
    };
    char pad218[4];
    ORDCHAR *partner;               /* 0x21c, the partner character of an answer */
} ORDMSG;
#define ORD_MSG(p) ((ORDMSG *)(p))

#pragma pack(push, 1)
/* the building record ai.h's AIBLD, as ord.c reads it (gv.h and ai.h cannot be in one TU: both declare
   ai_Time / ai_Chars / ai_BTypes); the object list of gm_BuildQuery's records */
typedef struct ORDBLD {
    unsigned char type;             /* 0x00, index into ai_BTypes */
    int id;                         /* 0x01, ai.h AIBLD.id */
    char name[0x20];                /* 0x05, the building's name (ai.h AIBLD.name) */
    unsigned short masterId;        /* 0x25, the character id of the building's master (ai.h AIBLD.masterId) */
    unsigned short owner;           /* 0x27, index into ai_Chars, 0xffff = none */
    unsigned short mainRoom;        /* 0x29, the room the building's panel opens with (ai.h AIBLD.mainRoom; gm_CreateBuilding) */
    int v2b;                        /* 0x2b, cleared by gm_CreateBuilding */
    unsigned char b2f;              /* 0x2f, cleared by gm_CreateBuilding */
    int v30;                        /* 0x30, set by gm_CreateBuilding: 0x1388 for a home, 100 for the kinds 0x1e .. 0x20, 0x13880 for the office 0x47, else 0 */
    char pad34[5];
    int budget;                     /* 0x39, ai.h AIBLD.budget, 0x7d00 when created */
    int wageRate;                   /* 0x3d, ai.h AIBLD.wageRate, 100 when created */
    int workRate;                   /* 0x41, ai.h AIBLD.workRate, 2 when created */
    int v45;                        /* 0x45, 100 when created */
    float priceFactor;              /* 0x49, scales the goods price of the building's offers (f3gm.c gm_CollectOffers) */
    int roundSum;                   /* 0x4d, ai.h AIBLD.roundSums[0], cleared at the end of every day (gm_EndDayIncomes) */
    char pad51[4];
    int v55;                        /* 0x55, of a kind 5 building an income per round (gm_BuildingBalance out[10]) */
    char pad59;
    union {
        unsigned char flags5a;      /* 0x5a, bit 0: takes no new inhabitants (gm_FindHome) */
        unsigned short flags5aw;    /* 0x5a, read as a word (ai.h AIBLD.flags5aw): 0x1000 adds 0.1 to the building's
                                       staff factor (gm_StaffFactor) */
    };
    char level;                     /* 0x5c, the building's level 0 .. 100 */
    int objList;                    /* 0x5d */
    struct D3OBJECT *object;        /* 0x61, its object in the scene */
    union {
        unsigned char people;       /* 0x65, the number of its inhabitants, below the kind's capacity
                                       (ord_GetKindRecord [0]) while it has room (gm_FindHome) */
        int block65;                /* 0x65, of the kinds 0x48 .. 0x4a a graveyard record (gv_FreeGraveyard), of
                                       0x4b .. 0x4d a heap block (gm_FreeBuilding frees both; ai.h AIBLD.yard) */
        int v65;                    /* 0x65, set by gm_CreateBuilding per kind: 4 .. 7 the owner's netId (-1 without
                                       one), 0xb .. 0xd -1, 0xe .. 0x10 10, 0x35 .. 0x37 0 */
    };
    union {
        struct {
            int daySum;             /* 0x69, ai.h AIBLD.unk69, cleared for the kinds 11 .. 13 at the end of every day (reasoned guess: a per-day sum, as roundSum) */
            int v6d;                /* 0x6d, gm_CreateBuilding: 5 for the kinds 0xe .. 0x10, 0 for 0x35 .. 0x37 */
            struct G15PLANT *plants; /* 0x71, of a kind 0x1e building its plant records (ai.h AIBLD.plants), freed by gm_FreeBuilding */
        };
        AITIME created;             /* 0x69, of the kinds 4 .. 7 the game time it was created (gm_CreateBuilding) */
        unsigned short w69[2];      /* 0x69, the kinds 0x35 .. 0x37 clear it as two words */
        struct {
            char pad69[8];
            ORDCREDIT credits[3];   /* 0x71, of a bank the credit offers kept for the player (gm_GetCreditOffers) */
        };
        char u69[0x2c];             /* 0x69 .. 0x95 */
    };
    int smokeEffect;                /* 0x95, ai.h AIBLD.smokeEffect, -1 none */
    int flags99;                    /* 0x99, bit 0 set when created */
    char pad9d[8];
    int va5;                        /* 0xa5, -1 when created */
} ORDBLD;

/* the head of an object record ai.h's AIOBJ, as gm_Query returns it (gv.h and ai.h cannot be in one TU) */
typedef struct ORDOBJ {
    unsigned short type;            /* 0x00, the prototype id */
    int objId;                      /* 0x02, the object's own id (messages name the object by it) */
    int ownerId;                    /* 0x06, the id of the building / object / character holding it (gm_ClearObjects finds it by 0x4fea50) */
    int holderId;                   /* 0x0a, gm_CreateObject: the holder's building id, the holding object's holderId or the holding character's netId (a cart: its owner character's netId) */
    int count;                      /* 0x0e, the amount of goods it stands for (ai.h AIOBJ.count) */
    unsigned char fill;             /* 0x12, how full a cart is in percent (ai.h AIOBJ.fill), 100 when created */
    unsigned char b13;              /* 0x13 */
    int list;                       /* 0x14, the object list it holds (ai.h AIOBJ.list) */
    struct D3OBJECT *obj;           /* 0x18, the scene object that shows it (ai.h AIOBJ.obj) */
    union {                         /* 0x1c, gm_CreateObject clears 0x1f bytes from here (to 0x3b) and sets them per prototype */
        unsigned char lvl1c;        /* 0x1c, a workshop's level (ai.h AIOBJ.lvl1c); gm_SyncMarket raises it to the market's goods count */
        int v1c[4];                 /* 0x1c */
        unsigned char cap[2];       /* 0x1c, of the objects 0x2a / 0x116 two capacities from the building kind (gm_SetCapacities) */
        struct {                    /* 0x1c, of the object 0x1b5 of the building kinds 0x14 .. 0x16: -1, -1, -1, 0 when created (gm_CreateBuilding) */
            char v1b5a;             /* 0x1c */
            char v1b5b;             /* 0x1d */
            int v1b5c;              /* 0x1e */
            unsigned char v1b5d;    /* 0x22 */
        };
        struct {
            int cartBld;            /* 0x1c, of a cart: the id of its building, -1 = none */
            int cart20;             /* 0x20 */
            int cart24;             /* 0x24 */
            unsigned char cart28;   /* 0x28 */
        };
    };
    char pad2c[5];
    unsigned char b31;              /* 0x31, a cart: 2 when created */
    char pad32[9];
    struct CHARACTER *model;        /* 0x3b, a cart's model (the cart or the horse, ch_Create) */
    struct ORDOBJ *next;            /* 0x3f, the next object of its list (ai.h AIOBJ.next) */
} ORDOBJ;                           /* 0x43 bytes, the records of the instance table ai_Objects */
#pragma pack(pop)

/* a record of the event list (he_FindEvent / he_NextEvent) as the order handlers read it */
typedef struct ORDEVENT {
    char pad000[0xac];
    int ownerId;                    /* 0x0ac, the character the event belongs to */
    int targetId;                   /* 0x0b0, the event's other character */
} ORDEVENT;
extern volatile unsigned long d3_Time;  /* 0x6333cc, milliseconds (tb.c) */
extern int d3_CurrentScene;                /* 0x633444 */

int util_Random(unsigned short n);      /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, cm_FightLoop ..) call util_RandomU, a cast in their head */
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
int cm_SendFieldDeltas(void);                     /* 0x4c49a0 */
extern char *ai_Blds;                   /* 0x889f74, 256 records of 0xa9 bytes */
extern unsigned short *ai_PlayerChar;    /* 0x6f1bf8, the player's character (ai.c; this header's units add byte offsets through ai_PlayerCharC) */
extern const struct D3VECTOR d3_Origin;     /* 0x5ebba4, (0, 0, 0), d3_vec.c */
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
    int state;                          /* 0x5c, the part is a SHOTREC (menu.h): this is its state, 0 idle, 1 in flight, 2 landed, 3 left the map (gv_UpdateEffects) */
    int time;                           /* 0x60, not saved: the reader sets it to d3_Time */
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
int gv_RequestEffectLoad(struct AIBLD *bld);            /* 0x4eea70 */
int gv_ReserveEffect(int id);              /* 0x4eeaa0 */
int gv_StopEffect(int id);              /* 0x4eeb00 */
int gv_FireShots(struct AIBLD *bld, struct AICHAR *ch, struct SHOTREC *a, struct SHOTREC *b, struct SHOTREC *d, float v, int cost, int x);  /* 0x4eeb70 */
char *gv_GetEffect(int id);            /* 0x4eedd0 */
int gv_StartShot(struct GVSHOTMSG *m, int w);       /* 0x4eee00 */
void gv_SetEffectCallback(int id, void *cb, void *arg);  /* 0x4eeee0 */
float gv_SalvoCharge(int a, int b, int c);  /* 0x4eef50 */
void gv_CannonKind(int n, int *out);    /* 0x4ef000 */
int gv_Accuse();                     /* 0x4f2e30 */
void cm_SendMoneyDelta(int a, int b);                     /* 0x4c5f60 */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
void gv_BuildingCannon(char *o, unsigned *balls, unsigned *kind);  /* 0x4ef170 */
void gv_CannonScatter(char *o, float *spread, float *speedSpread);  /* 0x4ef260 */
float gv_CannonItemFactor(char *o);           /* 0x4ef330 */
int gv_GraveCount(unsigned char *o);    /* 0x4ef3c0 */
int gv_GravesInUse(unsigned char *o);    /* 0x4ef440 */
int gv_EntryCount(unsigned char *o);    /* 0x4ef470 */
int gv_SizeOpen(char *r, int i);      /* 0x4ef4f0 */
extern char *ai_BTypes;              /* 0x7e3f30 */
int gv_ListEntry(char *r, int i, unsigned char n);  /* 0x4ef570 */
int gv_EntryIndex(char *r, int *e);     /* 0x4ef6b0 */
int gv_EntryHeldBy(char *a, char *b);    /* 0x4ef710 */
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
int gv_OrderBury(int *g, char *o, int v, int b);  /* 0x4f01d0 */
int gv_BuryCharacter(int *p);              /* 0x4f0240 */
int gv_OrderReserveGrave(int *c, char *o, int size, short years, int price);  /* 0x4f0430 */
int gv_ReserveGrave(GVORDER *m);          /* 0x4f0490 */
int gv_OrderExtendGrave(int *g, char *o, int v, int b, short s, int x);  /* 0x4f0700 */
int gv_ExtendGrave(GVORDER *m);           /* 0x4f0770 */
int gv_OrderSetGraveHolder(int *c, char *o, int size, int entry);  /* 0x4f0900 */
int gv_SetGraveHolder(GVORDER *m);         /* 0x4f0970 */
int gv_OrderOpenSize(int *c, char *o, int size);  /* 0x4f0a90 */
int gm_AddNegative();                     /* 0x500ae0 */
int gm_AddObjekt(int id, int proto, int n);                     /* 0x500970 */
int gv_OpenSize(GVORDER *m);           /* 0x4f0af0 */
int gv_OrderAdjustEntry(char *o, int v, int b, int x);  /* 0x4f0c20 */
int gv_AdjustEntry(GVORDER *m);          /* 0x4f0c80 */
int gv_OrderNewDead(char *o, int x);      /* 0x4f0d70 */
int gv_AddNewDead(unsigned char *p);    /* 0x4f0dd0 */
int gv_OrderMoveGrave(int *c, int to, char *o, int from, int entry, int price);  /* 0x4f0e80 */
int gv_MoveGrave(GVMOVE *m);           /* 0x4f0ed0 */
int gv_OrderRaiseDead(char *o, int size, int entry, int grave, int kind);  /* 0x4f1170 */
int gv_SendGrave(unsigned char *o);    /* 0x4f15e0 */
int gv_DecayRaisedDead(struct AICHAR *c);  /* 0x4f1660 */
int gv_RoomForRaised(unsigned char *o, int *n);  /* 0x4f1d50 */
float gv_PreserveRate(char *o);           /* 0x4f16f0 */
void cm_SendFloatDelta(int id, int field, float delta);  /* 0x4c4ac0 */
int gv_WriteGraveyard(void *f, unsigned char *o);  /* 0x4f17f0 */
char * gm_Query(void *list, int n, ...);                   /* 0x4ffe90 */
unsigned gv_RaiseDead(GVRAISE *m);     /* 0x4f1200 */
int gv_ReadGraveyard(void *f, unsigned char *o);  /* 0x4f1aa0, reads the graveyard record of o from the file f (allocated by 0x4efc10) */
float util_RandomF(void);               /* 0x42ec20 */
extern int ai_CharCount;                /* 0x6f1bc0, the number of characters in use (of ai_Chars[0x300]) */
int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5);                     /* 0x4fbd10 */
void gm_ChooseHeadIcon(unsigned short *c);                     /* 0x4fb890 */
GVENTRY *gv_FindWornEntry(struct AIBLD *bld, unsigned char m);  /* 0x4f22a0 */

int gv_FindHeldEntry(void *a, unsigned char **o, int *e);  /* 0x4f2540 */
int gv_FindReservedEntry(void *a, unsigned char **o, int *e);  /* 0x4f25d0 */
float gv_SeanceChance(char *o);           /* 0x4f2660 */
int gv_GetTableRecord(int i, int *out);     /* 0x4f2720 */
int gv_FindTableRecord(char *c, unsigned b, int *out);  /* 0x4f2870 */
extern AIDEC gv_Decisions[];           /* 0x611258, 0x1d AIDEC records of 0x24 bytes (the AI decision table) */

int gv_WriteCrimes(void *f);             /* 0x4f28d0 */
float gv_SchoolLevelFactor(unsigned short *c);          /* 0x4f2da0 */
int gm_BuildingSkill();                     /* 0x5092e0 */

int gv_ReadCrimes(int f);             /* 0x4f2b30 */
int gv_TableRecordText(char *out, int i);    /* 0x4f2fe0 */

int gv_FindCrimesByState(int state, int *out);    /* 0x4f3610 */
int gv_AddEvidence(int crime, int owner);        /* 0x4f3640 */
int gv_FreeCrimeSlot(void);                /* 0x4f36b0 */
int gv_FindCrime(int id);              /* 0x4f36e0 */
int gv_EvidenceSlot(int a, int b);        /* 0x4f3780 */

int gv_HandOverCrimes(int owner, int culprit, int state, int hand);  /* 0x4f3910 */
int gv_CountOwnedGraves(int c);               /* 0x4f39b0 */
int gv_CulpritGuilt(int c);               /* 0x4f39e0 */

int gv_HeldCrimesOf(int culprit, int *out, unsigned short t);  /* 0x4f3fe0 */
int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t);  /* 0x4f4080 */
int gv_HeldCrimeCount(int culprit, unsigned short t);   /* 0x4f4130 */
void gv_ClearCrimes(void);               /* 0x4f4350 */
int gv_CheckSentence(struct AICHAR *c, unsigned k); /* 0x4f4420 */
short * gm_FindChar(int id);                   /* 0x505df0 */

void gv_CheckRankUps(void);               /* 0x4f4d10 */
int gv_EvidencePrice(int id, struct AICHAR *seller, int *price, short *culprit, unsigned *law);  /* 0x4f4d30 */
void gv_CheckRankUp(unsigned i);              /* 0x4f4970 */
int gm_CharLevel(unsigned short id, int again);                     /* 0x505e60 */
float gm_Liking();                   /* 0x50c550 */

void gv_SendCrimes(void);               /* 0x4f3700 */
int gv_RetireCrime(int id, int m);       /* 0x4f37e0 */
void gv_SortCrimesByLaw(int n, CRIME *r);     /* 0x4f4390 */
char * he_FindEvent(int n, ...);         /* 0x53e400, as he.h */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void sv_PumpNet();                    /* 0x52fe00 */
extern unsigned short ai_PlayerId;     /* 0x6f25b8 */

int gv_RetireCrimesOf(int p, int n);        /* 0x4f3a30 */
int gv_RetireCrimeIds(int p, int n, int *ids);  /* 0x4f3b40 */

int gv_RankCulprits(int *ids, int *cnt, unsigned short t);/* 0x4f3c50 */

int gv_RankCulpritsAll(int *ids, int *open, int *total, unsigned short t);  /* 0x4f3dd0 */

extern const int gv_RankWorth[];             /* 0x5ee0f8, experience thresholds per level */
extern unsigned char ai_NumPlayers;     /* 0x62489c, the number of players (as ai.h / cm.h / mission.c) */
int gm_GetWorth(unsigned short id);    /* 0x50a640 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void cm_BeginFieldBlock(char *o, int id);                     /* 0x4c3ba0 */
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3c00 */
int cm_SendFieldDeltas(void);                     /* 0x4c49a0 */
struct AITIME;  /* f3gm.h typedef (gm_AddTime); file-scope so the gm_AddTime prototype below is stable */
void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);                     /* 0x4fe140 */

int gv_CannonCost(char *o, int a, int b, int d, int *n, int *price);  /* 0x4ef090 */
int gv_MovePrice(unsigned short *c, int k1, int k2, int *out);  /* 0x4f2440 */
int gv_ProtectionPrice(unsigned short *c);  /* 0x4f24a0, returns gm_PlayerFactor's value (0x4f2532 ret after the 0x5085f0 call) */
int gv_CrimeTextLaw2to8(char *out, CRIME r, int mode);  /* 0x4f30a0 */
int gv_CrimeTextLaw9to16(char *out, CRIME r, int mode);  /* 0x4f31d0 */
int gv_CrimeTextLaw17to28(char *out, CRIME r, int mode);  /* 0x4f3270 */
int gv_CrimeText(char *out, CRIME r, int mode);  /* 0x4f3580 */
char *gv_FindAccused(char *c, int max, int *ids, int *cnt);  /* 0x4f41b0 */
void gv_PassSentence(struct AICHAR *c, int a);  /* 0x4f4460 */
void gv_SentenceText(unsigned short *c, int a, char *out);  /* 0x4f48f0 */

unsigned ord_ApplyForOffice(ORDCHAR *c, int *m);  /* 0x4f4e20 */
char * he_NextEvent(void);                   /* 0x53e300 */

int ord_LevelOrder(char *c, int *m);     /* 0x4f4f20 */
int ord_ChangeProfessionOrder(char *c, int *m);     /* 0x4f4ff0 */
int ord_PrivilegeDialog(char *text, int mode, int arg);  /* 0x4f8200, the privilege dialog */

int gv_SetTableRecord(struct AICHAR *c, int i, int v);  /* 0x4f2750 */
int gv_OnDecisionValue(GVDECMSG *p);              /* 0x4f27d0 */

int gv_Accuse(int i, int v, int a, int b, int obj);  /* 0x4f2e30 */
float util_RandomF(void);               /* 0x42ec20 */

void gv_GraveyardTurn(char *o);            /* 0x4f1de0 */
int gv_LeasePrice(unsigned short *c, unsigned char care, int kind, int flag, int *out);  /* 0x4f2330 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
int cm_SendRelations(int a, int b, int c, int d);                     /* 0x4c4b00 */
int gm_CharCurrency(char *c, char p);                   /* 0x509ee0 */

int ord_MedicusOrder(char *c, int *m);     /* 0x4f5070 */

int ord_AccusationOrder(char *c, int *m);     /* 0x4f50f0 */

int ord_BlackmailBuildingOrder(char *c, int *m);     /* 0x4f5190 */
int ord_IntimidateOrder(char *c, int *m);     /* 0x4f52e0 */
int ord_BlackmailContactOrder(char *c, int *m);     /* 0x4f5350 */
int ord_DismissOfficeOrder(char *c, int *m);     /* 0x4f5c30 */
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

int ord_BewitchOrder(char *c, int *m);     /* 0x4f66b0 */

int ord_Job17Order(char *c, int *m);     /* 0x4f68d0 */
int ord_Job17Order2(char *c, int *m);     /* 0x4f69a0 */
int ord_JobMultiOrder4(char *c, int *m);     /* 0x4f6a70 */
int ord_Job10Order(char *c, int *m);     /* 0x4f6c60 */
int ord_Job15Order(char *c, int *m);     /* 0x4f6d30 */
int ord_CounterEspionageOrder(char *c, int *m);     /* 0x4f67c0 */
int ord_MisappropriationOrder(char *c, int *m);     /* 0x4f6b40 */

int ord_SeatSwapOrder(char *c, int *m);     /* 0x4f6e00 */
int ord_HatredOrder(char *c, int *m);     /* 0x4f6f10 */
int ord_HearingOrder(char *c, int *m);     /* 0x4f70f0 */
int ord_ExpelWorkerOrder(char *c, int *m);     /* 0x4f7200 */
int ord_ConvertOrder(char *c, int *m);     /* 0x4f7300 */
int ord_PeaceOrder(char *c, int *m);     /* 0x4f7400 */
int ord_PenanceOrder(char *c, int *m);     /* 0x4f74f0 */
int ord_SpyTargetFlagOrder(char *c, int *m);     /* 0x4f75e0 */
int ord_JobMultiOrder3(char *c, int *m);     /* 0x4f7000 */

int ord_MiracleOrder(char *c, int *m);     /* 0x4f76e0 */
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
extern ORDMSGREC ord_MessageRecord;     /* 0x91b480, the record the messages carry */
extern char gm_IncomeHistory[];         /* 0x91b580, the 17-round history of the five income sums (rows of
                                            0x44 bytes, the running sums behind them) */
extern char gm_CharacterStatistics[];            /* 0x91b500 */
extern ORDSTATROUND ord_StatisticsHistory[16]; /* 0x91ab00, the statistics of the last 16 rounds */
extern ORDSTATS ord_Statistics;         /* 0x91b4c0, the last statistics (ord_UpdateStatistics) */
extern float ai_RatingBase;              /* 0x6f1b9c, the running rating: it moves towards ai_Rating (ai.h, 0x6f1ba0) by ord_RatingRate */
extern float ord_RatingRate;             /* 0x91aaf4, the change rate of the rating (ord.c 0x4f8ed0; ls.c saves it) */
#ifndef DATA_VIEWS_H
extern char *unk_Ptrfd7c50;             /* 0xfd7c50, the scene */
#endif
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);                    /* 0x45f170 */
void ord_ResetStatistics(void);               /* 0x4f82a0 */
int ord_GetGridCell(char *p, int *x, int *y);  /* 0x4f8300, the grid cell (x, y) of the position p */

unsigned char * gm_BuildNext(void);                     /* 0x501030 */
int gm_FindById(int *bld, int *obj, int *chr, int id);  /* 0x4fea50, fills *bld with the object of the id (ai.h's prototype) */
int gm_EquipmentValue(unsigned char *b, int level);                     /* 0x509b10 */
void ord_CountGridObjects(void);               /* 0x4f8360 */

char d3sm_GetVertex(struct D3SM *h, int x, int z, struct D3VECTOR *out);                              /* 0x45f100, d3_sm.c (this unit's callers read an int: d3sm_GetVertexI) */
extern char ord_KindTable[];            /* 0x617468, 0x58 pairs of ints */
int ord_RandomCellPoint(int x, int y, int z);  /* 0x4f86a0 */
int ord_GetKindRecord(int i, int *out);     /* 0x4f8890 */

void ord_AddGridAmount(int who, unsigned char kind, int *px, int *py);  /* 0x4f8460, kind: a law record (CRIME.law; the caller cm_ExAddCrime pushes the byte unwidened) */
void ord_SubGridAmount(int who, unsigned char kind);  /* 0x4f8590 */

int ord_RandomObjectId(char *c);             /* 0x4f8700 */

void ord_AppendStatisticsRound(void);               /* 0x4f88c0 */

extern char ai_Crimes[];  /* 0x91f700, 0x200 records of 0x2d bytes (to 0x925100) */
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

extern AITIME ord_StatisticsClock;      /* 0x91b470, the clock of the last rating step (ord_AdvanceRatingBase) */
int gm_MinutesBetween();                     /* 0x4fe280 */
void ord_AdvanceRatingBase(void);               /* 0x4f9c00 */

/* declared in other units' headers before (moved to the owner) */
extern struct AICHAR *ai_JudgeCatholic;  /* 0x6f1bfc */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 */

#endif
