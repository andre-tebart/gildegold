struct GCLOCK;
/* ai.h -- the AI unit (ai.c, 0x47d9e0..0x48e070; "ai_" strings: ai_CalcMeisterWache, ai_CalcAngriff,
   ai_CalcMeisterDiebe, ai_MeisterTrade, ai_MeisterCollectTrans, ai_MeisterTradeFernEinkauf ...). Original file name
   unknown. Names are ours except the ones taken from the strings; the layouts only hold the fields the AI code
   touches. Unified in round 21 from the headers of the round 19 / 20 files and in round 22 (ai2) from the views of
   the round 21 files (AIEN / AIS / AIENX / AISLX / AICHX / AIBLDX / AIJOBX / AIBTYPEX / AIOBJT / AIWATCHW / AIINFOW
   are now fields of the types below). Where two files named the same field differently, an anonymous union keeps
   both names (one offset, one type). */
#ifndef AI_H
#define AI_H

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#pragma pack(push, 1)
typedef struct AIBLD {                  /* 0xa9 bytes, ai_Blds[256] */
    unsigned char type;                 /* 0x00, index into ai_BTypes */
    int id;                             /* 0x01 */
    union {
        char name[0x22];                /* 0x05 */
        struct {
            char padname[0x20];
            unsigned short masterId;    /* 0x25, the character id of the building's master (the AI looks its
                                           buildings up by it, am.c 0x497cf0; the upgrade message goes to it) */
        };
    };
    unsigned short owner;               /* 0x27, index into ai_Chars */
    unsigned short mainRoom;            /* 0x29, the room (scene record) the building's panel opens with: sv_UpdatePanel passes it to ai_TechTreeWindow where it passes *sv_PanelShown for a shown room (reasoned) */
    char pad2b[0xe];
    int budget;                         /* 0x39, the money the building may spend this round: main_EndRound spreads its owner's goods value (x 75) over the buildings by AIBTYPE.level (2 / 4 / 8 shares, capped) and copies it to the AICHAR.budget of the local jobs; ai.c buys up to it */
    int wageRate;                       /* 0x3d, the percent of the job wages its workers get (main_SettleRound pays wage * rate / 100; below 100 lowers their opinion of the owner, above raises it) */
    int workRate;                       /* 0x41, a percent setting whose distance from 100 moves the workers' opinion the other way (main_SettleRound; reasoned guess: the working hours) */
    char pad45[8];
    int roundSums[3];                   /* 0x4d, cleared at every round settlement (main_SettleRound); reasoned guess: per-round sums */
    char pad59;
    union {
        unsigned char flags5a;          /* 0x5a */
        unsigned short flags5aw;        /* 0x5a, read as a word: bits 0x3800 are cleared at the round settlement (cm_SendSetField(id, 0x5a, 2, 0, 0x3800)) */
    };
    char level;                         /* 0x5c, the building's level 0 .. 100 (renovation below a rolled
                                           threshold, 100 - level in the renovation message, am.c) */
    int objList;                        /* 0x5d, head of the building's object-instance list (gm_Query 0x4ffe90,
                                           released with gm_RemoveObject) */
    char *object;                       /* 0x61, its 3D object in the scene */
    union {
        int unk65;                      /* 0x65, moved by +-1 per event towards AIPOS.unk18 (ai.c 0x4836a0) */
        int playerSlot;                 /* 0x65, of a kind 0x47 building: its ai_PlayerSlots index (sv_OpenBuildingPanel) */
        struct GVYARD *yard;            /* 0x65, of a kind 0x1d building: its graveyard record (gv.h, gv_AllocGraveyard) */
    };
    int unk69;                          /* 0x69, cleared for the kind 7 buildings at every round start (main.c 0x53b2e0) */
    char pad6d[4];
    int plants;                         /* 0x71, the 64 G15PLANT records (0x18 bytes each, rd.h) of the
                                           building's vegetation map (rd.c: veg_Fits / veg_FindPlantId) */
    char pad75[0x20];
    int smokeEffect;                    /* 0x95, the id of the running chimney smoke effect (evt_FindById), -1 none (sv.c 0x528490) */
    char pad99[0x10];
} AIBLD;

typedef struct AIOBJ {                  /* 0x43 bytes, an object (table behind ai_Objects): carts, stores, the
                                           goods in a store, a workshop's level object; what gm_Query finds */
    union {
        unsigned short id;              /* 0x00, index into ai_Goods */
        unsigned short type;            /* 0x00, 0x116: ... */
    };
    int objId;                          /* 0x02, the object's own id (0x500190 gives a new one; messages name the
                                           object by it, 0x4fea50 finds it by it) */
    int unk06;                          /* 0x06 */
    char pad0a[4];
    union {
        int unk0e;                      /* 0x0e */
        int count;                      /* 0x0e */
    };
    unsigned char fill;                 /* 0x12, how full the cart is in percent (msg.n = 100 - fill; below 0x21 a cart is free for a load, ai_MeisterTrade / ai_UnloadCarts) */
    char pad13;
    union {
        int unk14;                      /* 0x14, the object list it holds */
        int list;                       /* 0x14 */
    };
    struct D3OBJECT *obj;               /* 0x18, the scene object that shows it (sv_DrawItemNames draws its name above it) */
    union {
        int unk1c;                      /* 0x1c, a building id / a state / a count */
        int bldId;                      /* 0x1c, of a cart (gm_Query kind 0x1d): the id of its building (main_EndRound looks it up with gm_BuildQuery(1, 1, id)) */
        int cnt;                        /* 0x1c */
        struct {
            unsigned char lvl1c;        /* 0x1c, a workshop's levels, compared with AIENTRY need1c / need20 */
            unsigned char lvl1d;        /* 0x1d */
        };
        struct {
            int pad1c20;
            unsigned int flags20;       /* 0x20, of a cart (kind 0x1d): bit 1 (2) is cleared for the carts of the current building when the selection resets (sv_ResetSelection) */
        };
        struct {
            int pad1c24[2];
            struct AIJOB *job24;        /* 0x24, of a cart: a job whose cartBld names a building (sv_RefreshPanel prints that building's kind; reasoned: the job the cart runs) */
        };
        int recs[3];                    /* 0x1c..0x27, of a gm_Query kind 300 object in a shop: three record ids for gv_EvidencePrice (0x4ab080 tries them in turn) */
        struct {                        /* of a room of a building (the objects of AIBLD.objList the enter-building driver walks) */
            int locked;                 /* 0x1c, nonzero: the room's contacts open ai_UnlockRoomWindow, which clears it when paid */
            int unlockPrice;            /* 0x20, the money ai_UnlockRoomWindow asks; cleared with locked */
        };
    };
    char pad28[2];
    int unk2a;                          /* 0x2a */
    char pad2e[8];
    unsigned char unk36;                /* 0x36 */
    unsigned char unk37;                /* 0x37 */
    char pad38[3];
    void *sceneObj;                     /* 0x3b, the scene character (ch.h CHARACTER) that shows a cart */
    struct AIOBJ *next;                 /* 0x3f, the next object of its list (an inventory, a store) */
} AIOBJ;

typedef struct AIGOOD {                 /* 0x41 bytes, ai_Goods[] */
    char type;                          /* 0x00 */
    union {
        char name[0x25];                /* 0x01 */
        struct {
            char padname[0x21];
            unsigned int unk22;         /* 0x22 */
        };
    };
    unsigned short need[4];             /* 0x26 */
    unsigned short in[4];               /* 0x2e, the goods it is made of */
    unsigned short batch;               /* 0x36, the amount one production run makes (the store needs room for
                                           it and the market that much plus the stock; he.c 0x5690c0 hands it out) */
    char pad38[9];
} AIGOOD;

typedef struct AIBTYPE {                /* 0x24d bytes, ai_BTypes[] */
    char kind;                          /* 0x000 */
    char pad001[0x21];
    unsigned char nslots;               /* 0x022 */
    unsigned short slot[0x100];         /* 0x023, needed goods (bit 15 = flag) */
    unsigned char unk223;               /* 0x223 */
    char pad224[0xc];
    union {
        unsigned char unk230;           /* 0x230 */
        unsigned char k230;             /* 0x230 */
    };
    unsigned char k231;                 /* 0x231 */
    unsigned char k232;                 /* 0x232 */
    char pad233[0xd];
    unsigned char min0;                 /* 0x240 */
    unsigned char min1;                 /* 0x241 */
    char pad242[5];
    union {
        char level;                     /* 0x247, the type's level 1 .. 3 (am.h AIBTYPEX: a count (level)); main_EndRound weighs a building's budget share 2 / 4 / 8 by it */
        unsigned char cap247;           /* 0x247, read as unsigned (0x489cb0) */
    };
    unsigned char maxLevel;             /* 0x248, the highest level of the type's line (reasoned: ai_DecideUpgradeGuild builds the next type only while cap247 is below it) */
    char pad249[4];
} AIBTYPE;
#pragma pack(pop)

#include "ai_types.h"                /* AITIME, AISTORE */

typedef struct AIJOB {                  /* a job (he_FindEvent / he_NextEvent) */
    char kind;                          /* 0x00 */
    char pad01[0x43];
    AITIME start;                       /* 0x44, the start time (sv_RefreshPanel measures the progress from it) */
    AITIME end;                         /* 0x52, the end time of the kinds whose length is fixed at the start (0x40, the building jobs 0x1e / 0x1f) */
    char pad60[0x10];
    int stage;                          /* 0x70, the stage the job has reached (sv_RefreshPanel shows progress only from a kind's timed stage on) */
    char pad74[0x18];
    int helpers[4];                     /* 0x8c, of a helper job 0x8c the characters on it, -1: a free place
                                           (ai_GypsyHelperWindow counts them) */
    char pad9c[0x10];
    union {
        int unkac;                      /* 0xac, a building id */
        int partner1;                   /* 0xac, of a pair event (kinds 0x41 / 0x6f / 0x45, he_Unk5538b0) the first partner's netId */
        int ownerCh;                    /* 0xac, of an event 0x24 the netId of the character it is for (ai_DecideChangeProfession skips a character that has one) */
        union {
            int id;                     /* 0xac */
            struct {
                unsigned short lo;      /* 0xac */
                unsigned short hi;      /* 0xae */
            } w;
        } u;
    };
    union {
        int unkb0;                      /* 0xb0 */
        int partner2;                   /* 0xb0, of a pair event the second partner's netId */
        int id2;                        /* 0xb0, a second id: a building (kind 100) or a character (kind 0x89) as
                                           sv_SpeakPicked looks them up */
    };
    union {                             /* 0xb4..0xe7: the words, or the kind-specific times sv_RefreshPanel measures the progress to / from */
        struct {
            int unkb4;                          /* 0xb4 */
            int unkb8;                          /* 0xb8, a job-kind specific payload word: of a harvest job (0x28) the
                                                   plant it works (veg_FindPlantId, ai_SendJobOrders), else a counter / an
                                                   id / a news id (he.c per kind); no single role name */
            char padbc[8];
            int workTotal;                      /* 0xc4, of a building job 0xd the total that argc8 counts up to (sv_RefreshPanel: progress argc8 / workTotal) */
            int argc8;                          /* 0xc8, a job-kind specific word (he.c: a random table entry, a task, an
                                                   object); a workshop's chimney smokes only while its job has none
                                                   (sv.c 0x528490) */
            char padcc[4];
            unsigned short in[3];               /* 0xd0, the workshop kinds */
            unsigned short out[3];              /* 0xd6 */
            int amount[3];                      /* 0xdc */
        };
        AITIME end6c;                   /* 0xb4, of a job kind 0x6c: the end time */
        int cartBld;                    /* 0xb4, of the job a cart runs (AIOBJ.job24): the building (sv_RefreshPanel prints its kind) */
        int id3;                        /* 0xb4, a third id: of a kind 0x1a the character sv_SpeakPicked looks up */
        struct { char padid4[0x10]; int id4; };       /* 0xc4, of a kind 0x18: the character sv_SpeakPicked looks up */
        struct { char padt8f[4]; AITIME end8f; };     /* 0xb8, of the kinds 0x8f / 0x16 and the building job 0x1c: the end time */
        struct { char padt61[0x10]; AITIME start61; }; /* 0xc4, of a job kind 0x61: the start of its eight hours */
        struct { char padt48[0x18]; AITIME end48; };  /* 0xcc, of a job kind 0x48 (stage 1): the end time */
        struct { char padgc4[0x10]; int members[4]; };  /* 0xc4, of a group job 'C' (0x43): the netIds of its up to four
                                                          members, -1 = free (sv_SendSelectedToBuilding) */
    };
    char pade8[9];
    char flags;                         /* 0xf1, a flags byte (nonzero defers the trade message posting; the bits are unknown) */
} AIJOB;

typedef struct AITASK {                 /* the character's scene character (ch.h CHARACTER: scriptId, group, scriptGroup, scene) as ai.h reads it */
    char pad00[0x28];
    int scriptId;                       /* 0x28, the event script it runs, -1 = none (CHARACTER.scriptId) */
    int bldId;                          /* 0x2c, the building the character was sent to (f3gm.c 0x4fbd10; CHARACTER.group) */
    int scriptGroup;                    /* 0x30, the group of the script that created / moved it (CHARACTER.scriptGroup) */
    struct D3OBJECT *body;              /* 0x34, its body object (CHARACTER.obj) */
    char pad38[0x50];
    struct D3SCENE *scene;              /* 0x88, the scene it stands in (CHARACTER.scene) */
} AITASK;

typedef struct AICHAR {                 /* 0x21c bytes, ai_Chars[768] */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002 */
    char pad003;
    unsigned int netId;                /* 0x004 */
    unsigned char health;               /* 0x008, 100 at creation (ahm.c), > 0 = alive (0x4adff0 'lives') */
    unsigned char sex;                  /* 0x009, 0 / non-0 (reasoned: 0x4a8ad0 asks the partner search for flags 0xf00502 / 0xf00501 by it, the other sex) */
    unsigned short w00a;                /* 0x00a, 0x10 for a raised dead person (gv_RaiseDead), a value of the order for the alchemist (ahm_CreateAlchemist) */
    char pad00c;
    unsigned char b00d;                 /* 0x00d, 1 .. 2 (gv: kept in a grave as GVGRAVE.v30) */
    char pad00e[2];
    float f010;                         /* 0x010, start values set by the character creators (ahm_CreateAlchemist, gv_RaiseDead) */
    float f014;                         /* 0x014, the step gm_AgeCharacter ages f01c by; a raised dead person loses it daily (gv_DecayRaisedDead) */
    float f018;                         /* 0x018 */
    float f01c;                         /* 0x01c, a life value: 1000.0 at creation, aged by gm_AgeCharacter, the character dies when it is used up (f3gm.c 0x4fbe90, gv_DecayRaisedDead) */
    float f020;                         /* 0x020 */
    int v024;                           /* 0x024, a value ai.c decays every frame (sent as message field 0x24), 0 at creation */
    short w028;                         /* 0x028, 0 at creation */
    char pad02a[2];
    int v02c;                         /* 0x02c, 0 at creation */
    union {
        char name[0x58];                /* 0x030 */
        struct {
            char first[0x10];           /* 0x030, the first name (gv: copied to the grave, GVGRAVE.first) */
            char family[0x10];          /* 0x040, the family name (GVGRAVE.family) */
            unsigned short w050;        /* 0x050, must equal the grave entry's GVENTRY.w08 to be buried there (gv_BuryCharacter) */
        };
        struct {
            char padname80[0x50];
            unsigned char skills[5];    /* 0x080, five values (gv: copied to the grave, GVGRAVE.traits) */
        };
        struct {
            char padname[0x51];
            unsigned char sk81;         /* 0x081, skills */
            unsigned char sk82;         /* 0x082 */
            unsigned char sk83;         /* 0x083 */
            unsigned char sk84;         /* 0x084 */
        };
        struct {
            char padname54[0x24];
            int ref54;                  /* 0x054, a reference word (the save adds the base 0x59a back,
                                           ls_ReadCharacters); its low byte picks a cart's texture set
                                           ('g' + it, ls_ReadSceneRecords) */
        };
        struct {
            char padname5c[0x2c];
            int relatives[8];           /* 0x05c, the netIds of the character's relatives (gm_KillSpieler clears
                                           them when one dies, ai_DecideUseDynastyObject walks [3..7]); relatives[0] is never
                                           picked as a partner (ai_DecideOfficePartner) */
        };
    };
    float mood[14][3];                  /* 0x088, 13 triples (a, b, value), the current one at 0x124 */
    unsigned char sel;                  /* 0x130 */
    char pad131;
    unsigned char act;                  /* 0x132, the current action */
    char pad133[0x31];
    unsigned char jobId;               /* 0x164 */
    char title;                         /* 0x165, the title rank (f3gm.c 0x4fda70 / 0x504840) */
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none (amt.h AMTCHARVIEW.officeKind;
                                           amt_KindColumn / amt_GetKind take it) */
    unsigned char prevOffice;           /* 0x167, the office kind held before the current one (amt.h AMTCHARVIEW.prevOffice) */
    unsigned char appliedOffice;        /* 0x168, the office kind applied for, 0 = none (amt.h AMTCHARVIEW.appliedOffice) */
    unsigned char secondOffice;         /* 0x169, the second office kind, the deputy one (amt.h AMTCHARVIEW.secondOffice) */
    char pad16a[2];
    AIBLD *bld;                         /* 0x16c */
    AIBLD *bld2;                        /* 0x170, the character's own building (the f3gm.c 0x4fdcb0 comment: its home), 0 when none */
    char pad174[4];
    int inventory;                      /* 0x178, the character's goods inventory (a G8 object list of item ids,
                                           next at +0x3f): gm_Query finds a prototype kind in it, gm_SumSlotAmounts
                                           sums a need slot over it (am.c 0x4a5760) */
    AIJOB *job;                       /* 0x17c */
    unsigned char jobKind;              /* 0x180, the kind (first byte) of job, refreshed on load */
    char pad181[3];
    AITASK *model;                      /* 0x184, the character's model record; its +0x2c is the building the
                                           character was sent to (f3gm.c 0x4fbd10) */
    unsigned char selected;             /* 0x188, nonzero: the character is in the player's selection (sv_ClearSelection clears it, sv_HandleObjects sets it) */
    char pad189[3];
    int picture;                        /* 0x18c, the portrait picture (sv.c: the person panel, the member list) */
    int v190;                         /* 0x190, a sum the round change prints on the player's balance page (text 0x1eff, main.c 0x5384e0) */
    int money;                          /* 0x194, the character's money (amt.c) */
    char pad198[4];
    int wage;                           /* 0x19c, the wage the character was paid in the last round settlement (main_SettleRound sends it as a field delta) */
    char pad1a0[0x11];
    unsigned char heldRounds;           /* 0x1b1, rounds the character is held: message 0x5b (cm.c 0x4ccb10) adds to it, each
                                           round adds 0xff (main.c); nonzero: the AI chooses no new action (ai.c
                                           0x47e360), the character takes no office (amt.c), the player's room click
                                           does not switch rooms (sv.c); reasoned (a jail term or the like) */
    char pad1b2[2];
    unsigned int flags1b4;              /* 0x1b4 */
    int budget;                         /* 0x1b8, money */
    union {
        int unk1bc;                     /* 0x1bc */
        unsigned short prod;            /* 0x1bc, the item the workshop makes */
    };
    union {
        int target;                     /* 0x1c0, a building id or -1 */
        int unk1c0;                     /* 0x1c0 */
    };
    char pad1c4;
    unsigned char b1c5;                 /* 0x1c5, a per-worker value am.c 0x49a390 averages over a building's workers (at least 6 with a random 0..3 added lets the AI use the good 0x161 there); the meaning is not known */
    char pad1c6[2];
    unsigned int flags1c8;              /* 0x1c8 */
    unsigned int flags1cc;              /* 0x1cc */
    char pad1d0[0x14];
    float mood3Rate;                    /* 0x1e4, scales the base growth of mood need 3; message 0x1a keeps it in sync */
    char pad1e8[4];
    int unk1ec;                         /* 0x1ec, cleared for every character at the round start (main.c 0x53b2e0) */
    int v1f0;                         /* 0x1f0, 0 at creation */
    union {
        char ownName[0x1c];             /* 0x1f4, the character's own name, empty: the job's name record
                                           (gm_CharNameRecord) */
        struct {
            char padOwnName[0x18];
            int partyId;                /* 0x20c, the scene party the character belongs to (sv_RunFrame passes it to
                                           cm_PartyById); reasoned: it overlays the last 4 bytes of ownName */
        };
    };
    int partner;                        /* 0x210 */
    signed char unk214;                 /* 0x214 */
    unsigned char b215;                 /* 0x215, a step counter of the partner search (ai_FindPartner: 0 when a partner is picked, advanced towards b216); reasoned */
    unsigned char b216;                 /* 0x216, the steps the partner search needs (ai_FindPartner: from the liking, clamped by ai_Difficulty); reasoned */
    unsigned char b217;                 /* 0x217, sent as field 0x218 when the partner search is done (ai_FindPartner) */
    unsigned char mask;                 /* 0x218, the action groups it may use */
    char pad219[3];
} AICHAR;

typedef char ai_AssertAICHAR[sizeof(AICHAR) == 0x21c ? 1 : -1];

typedef struct AIENTRY {                /* 0x58 bytes, ai_Entries[32]: a good the workshop makes */
    int item;                           /* 0x00, index into ai_Goods */
    int slot[4];                        /* 0x04, indices into ai_Slots */
    AIBLD *bld;                         /* 0x14, where to buy it */
    void *store;                        /* 0x18 */
    union {
        int need1c;                     /* 0x1c */
        int unk1c;                      /* 0x1c */
    };
    union {
        int need20;                     /* 0x20 */
        int unk20;                      /* 0x20 */
    };
    float cost;                         /* 0x24, production cost per unit: materialCost + objectValue (ai_CalcEntryCost) */
    float materialCost;                 /* 0x28, cost of the needed goods per unit: their slot prices * need / batch */
    float objectValue;                  /* 0x2c, gm_ObjectValue(item) */
    float price;                        /* 0x30, the good's sale price (gm_GoodsPrice) */
    float margin;                       /* 0x34, sort key and profit per unit: (price - cost) / AIGOOD.unk22 */
    int pending;                        /* 0x38, the pieces in production: the running jobs (he_FindEvent 0xb) add
                                           their in / out amounts here while they add the same to AISLOT.pending */
    int amount;                         /* 0x3c, of the good in the workshop's store (gm_ListAmount 0x50aaf0,
                                           gm_ListAmountById 0x50ab20; 0 = the store holds none) */
    int capacity;                       /* 0x40, how many more of the good the workshop's store can take
                                           (gm_FreeCapacity 0x50ab80) */
    int sellAmount;                     /* 0x44, the units of the entry to sell this round (ai_MeisterTrade sets it from amount, 0 = none; the log "Verkauf von %li Einheiten") */
    int rank;                           /* 0x48, position after sorting (0x21 until ranked) */
    int buyAmount;                      /* 0x4c, how many to buy (min of the entry's amount and its space) */
    int unk50;                          /* 0x50 */
    unsigned int flags;                 /* 0x54 */
} AIENTRY;

typedef struct AISLOT {                 /* 0x40 bytes, ai_Slots[128]: a good the workshop needs */
    int item;                           /* 0x00 */
    int entry;                          /* 0x04, index into ai_Entries */
    union {
        AIBLD *unk08;                   /* 0x08, where to buy it */
        AIBLD *bld;                     /* 0x08 */
    };
    void *store;                        /* 0x0c */
    int unk10;                          /* 0x10 */
    int make;                           /* 0x14, the pieces to make / buy now (clamped to capacity) */
    int sellAmount;                     /* 0x18, the units of the slot's good this round sells (set from amount, counted down as the sales go out) */
    float price;                        /* 0x1c, a price (gm_GoodsPrice, 90% for a good the workshop makes) */
    float saving;                       /* 0x20, sort key of the buy list: the base price (gm_ItemPrice) minus the
                                           price here (<= 0: nothing to gain, the trade stops) */
    int pending;                        /* 0x24, the pieces already on their way (subtracted with amount from a need) */
    int amount;                         /* 0x28, the pieces of the good in the workshop's store */
int capacity;                    /* 0x2c, the upper bound of pieces (make is clamped to it) */
    int rank;                           /* 0x30, position after sorting (0x81 until ranked) */
    int buyAmount;                      /* 0x34, how many to buy: min (capacity, the entry's other store, budget / 4 / price) */
    int unk38;                          /* 0x38 */
    unsigned int flags;                 /* 0x3c */
} AISLOT;

typedef struct AIMSG {                  /* 0xf8 bytes, given to cm_SendLongMessage */
    int unk00;
    unsigned char type;                 /* 0x04 */
    char pad05[3];
    int to;                             /* 0x08, the receiving player's netId (G8MSG's 'to'; every sender
                                           writes the building owner's netId) */
    int fromId;                         /* 0x0c, the sending building's id */
    int nIds;                           /* 0x10, how many of ids[] are set (msg.ids[msg.nIds]++) */
    int unk14;                          /* 0x14, only ever written 0 (hst.c: -1), no reader names it */
    char pad18[0x10];
    AITIME time;                        /* 0x28 */
    unsigned char flags;                /* 0x36, the message flags (1 / 2 / 9; he.md's message record) */
    char pad37;
    int ids[8];                         /* 0x38 */
    union {
        int arg[11];                    /* 0x58 */
        unsigned short itemId;          /* 0x58, the good a buy / move message is about (the 16-bit stores) */
        struct {                        /* the trade kinds */
            int car;                    /* 0x58 */
            int n;                      /* 0x5c */
            int ch;                     /* 0x60 */
            int dst[6];                 /* 0x64, 0..2 sold, 3..5 bought */
            union {
                unsigned short item[6]; /* 0x7c */
                struct {
                    int pad7c[2];
                    int unk84;          /* 0x84 */
                } s;
            } u;
        };
        struct {                        /* the rank message 0x69 (gv_CheckRankUp) */
            unsigned char rank;         /* 0x58, the new rank (AICHAR.b00d) */
            char pad59[3];
            int ch;                     /* 0x5c, the netId of the character who rose */
        } rankUp;
        struct {                        /* the pamphlet message 0x35 (0x4aa940) */
            AITIME when;                /* 0x58, the game time it was sent */
            char pad66[2];
            int about;                  /* 0x68, the netId of the character it is about */
            char pad6c;
            unsigned char variant;      /* 0x6d, a random text variant 0..0x1c */
        } pamphlet;
        struct {                        /* the grave lease offer 0x92 to a player (gv_GraveyardTurn; he_Unk5728d0 answers it with gv_OrderExtendGrave) */
            int bld;                    /* 0x58, the graveyard building's id */
            int size;                   /* 0x5c, the grave size */
            unsigned char entry;        /* 0x60, the entry of that size */
            char pad61[3];
            int price;                  /* 0x64, the lease price (gv_LeasePrice) */
            int years;                  /* 0x68, the years the lease is extended by (10) */
        } lease;
        struct {                        /* the graveyard work 0x8d on a grave (ai_GraveCare; he_HasGraveWork finds its event) */
            int pad58;
            int size;                   /* 0x5c, the grave's size (list) */
            unsigned char index;        /* 0x60, its index in that list */
        } graveWork;
        struct {                        /* the study message 0x5f ("Studium", ai_ExecuteStudy) */
            int ch;                     /* 0x58, the netId of the character who pays */
            int pad5c;
            int child;                  /* 0x60, the child's value from the decide callback (s1->arg2) */
            unsigned char subject;      /* 0x64, the offer chosen (s2->ref) */
            unsigned char level;        /* 0x65, the offer's second value (s2->arg2) */
        } study;
    };
    union {
        int amount[3];                  /* 0x88, of the bought goods */
        struct {
            char pad88;
            char unk89;                 /* 0x89 */
        };
    };
    char pad94[9];
    unsigned char unk9d;                /* 0x9d */
    char pad9e[2];
    int unka0;                          /* 0xa0 */
    char pada4[0xc];
    unsigned char extraFlag;            /* 0xb0, 1 in the explosion message 0x4f (gv_ExplodeShot); the meaning is not known */
    char padb1[0x47];
} AIMSG;

typedef char ai_AssertAIMSG[sizeof(AIMSG) == 0xf8 ? 1 : -1];

typedef struct AIINFO {                 /* given to ai_SendJobOrders */
    int type;                           /* 0x00 */
    AICHAR *ch;                         /* 0x04 */
    AIENTRY *e;                         /* 0x08 */
    AIOBJ *b;                           /* 0x0c */
    AIOBJ *c;                           /* 0x10 */
    int n1;                             /* 0x14 */
    int n2;                             /* 0x18 */
    int z1;                             /* 0x1c */
    int z2;                             /* 0x20 */
} AIINFO;

typedef struct AIINFOP {                /* an AIINFO with the shuffled entry indices behind it (one local) */
    int type;                           /* 0x00 */
    AICHAR *ch;                         /* 0x04 */
    AIENTRY *e;                         /* 0x08 */
    AIOBJ *b;                           /* 0x0c */
    AIOBJ *c;                           /* 0x10 */
    int n1;                             /* 0x14 */
    int n2;                             /* 0x18 */
    int z1;                             /* 0x1c */
    int z2;                             /* 0x20 */
    int perm[32];                       /* 0x24 */
} AIINFOP;

typedef struct AIMAP {
    int w;                              /* 0x00, width = height */
    char pad04[0x10];
    unsigned char *cells;               /* 0x14 */
    char pad18[0x1a4c];
    char names[8][0x40];                /* 0x1a64 */
} AIMAP;

typedef struct AIMAPREF {               /* 0x3e0 bytes */
    AIMAP *map;                         /* 0x00 */
    char pad04[0x3dc];
} AIMAPREF;

typedef struct AIPLANT {                /* a partial G15TREE (rd.h), what veg_FindType returns */
    unsigned short id;                  /* 0x00, G15TREE.id (the plant type, 0 ends the table) */
    char pad02[0x40];
    unsigned char size;                 /* 0x42, G15TREE.size (its size in cells) */
} AIPLANT;

typedef struct AIWATCH {                /* 0xf895a0, what the watched building's masters did */
    AIBLD *bld;                         /* 0x000 */
    AIBLD *target;                      /* 0x004 */
    AICHAR *ch;                         /* 0x008 */
    AICHAR *owner;                      /* 0x00c */
    int budget;                         /* 0x010 */
    int n;                              /* 0x014 */
    int nslots;                         /* 0x018 */
    int nentries;                       /* 0x01c */
    int cnt[4];                         /* 0x020 */
    int pad030[4];
    AIENTRY ent[4];                     /* 0x040 */
    AIENTRY entries[4];                 /* 0x1a0 */
    AISLOT slots[16];                   /* 0x300 */
} AIWATCH;

typedef struct AIGRID {                 /* 24 bytes, ai_Grid[8][8] */
    unsigned short a;                   /* 0x00 */
    unsigned short b;                   /* 0x02 */
    char used;                          /* 0x04 */
    char pad05[0x13];
} AIGRID;

typedef struct AIPOS {                  /* filled by gv_GetTableRecord */
    char pad00[0x18];
    int unk18;                          /* 0x18 */
    int unk1c;
    int unk20;
} AIPOS;

typedef struct AIRECIPE {               /* 0xb8 bytes, filled by gm_StallGoodsList / gm_BuildingGoodsList */
    int n;                              /* 0x00 */
    int unk04;                          /* 0x04 */
    int max[16];                        /* 0x08 */
    unsigned short item[16];            /* 0x48 */
    int amount[16];                     /* 0x68 */
    int pada8[4];
} AIRECIPE;

typedef struct AIFIN {                  /* what gm_PlayerRecord finds for a character */
    char pad00[0x28];
    int spyPaid;                        /* 0x28, the money the player paid to the cabinet's spies (ai_CabinetSpyWindow adds each payment) */
    char pad2c[0x14];
    int upgradeSpent;                   /* 0x40, the money the record has paid for workshop upgrades (ai_BuyUpgrade) */
    int renovationSpent;                /* 0x44, the money it has paid for renovations (ai_BuyUpgrade, am.c 0x497ee0) */
    char pad48[0x10];
    float roundFactors[3];              /* 0x58, set to 0.5, 1, 1 at every round change (main.c 0x5384e0); their reader is not known yet */
    char pad64[0x24];
    float camAngle;                     /* 0x88, the player's camera angle factor (sv_CameraAngleFactor; sv.c) */
    float camPos[3];                    /* 0x8c, the player's camera position, restored with the view (sv.c) */
    char pad98[4];
    float camRot[3];                    /* 0x9c, its rotation (D3OBJECT.rotation) */
} AIFIN;

typedef struct AIMAPENT {               /* 24 bytes, the AI's view of the vegetation records at AIBLD.plants (rd.h
                                           G15PLANT, the same 0x18-byte layout; ai.c includes rd.h now, so the readers
                                           can take G15PLANT) */
    int id;                             /* 0x00, G15PLANT.id */
    int unk04;                          /* 0x04 */
    unsigned char x;                    /* 0x08, cell */
    unsigned char z;                    /* 0x09 */
    unsigned short type;                /* 0x0a, G15PLANT.type, the plant type (0 removes it) */
    char size;                          /* 0x0c, size in cells */
    char state;                         /* 0x0d, growth state; -1 free (veg_FindPlantId skips those) */
    char pad0e[2];
    int unk10;                          /* 0x10, -1 for a free slot (veg_SetPlant); no role name (no other reader) */
    int obj;                            /* 0x14, its 3D object */
} AIMAPENT;

#define AI_KIND(t) (((int (*)(int))gm_BuildingClass)(t))   /* the call zero-extends the byte (xor ecx,ecx; mov cl) */

extern char ai_Time[14];                 /* 0x770ae0, the game clock: an AITIME (14 bytes), defined by anm.c as char[14]; the units that read its fields use their ai_TimeT view */
extern char ai_Chars[768][0x21c];        /* 0x770b80, the characters: AICHAR records, defined by ahm.c as char[768][0x21c]; the units that read their fields use their ai_CharsT view */
extern char *ai_BTypes;                 /* 0x7e3f30, the building types: AIBTYPE records (ahm.c defines char *; ai_BTypesT views) */
extern char *ai_Blds;                   /* 0x889f74, the buildings: AIBLD records (f3gm.c defines char *; ai_BldsT views) */
extern char *ai_Goods;                  /* 0x91aaf0, the goods: AIGOOD records, 0x41 bytes each (f3gm.c defines char *; ai_GoodsT views) */
extern int ai_NSlots;                   /* 0xf86a28 */
extern int ai_NEntries;                 /* 0xf86a2c */
extern AISLOT ai_Slots[128];            /* 0xf86a40 */
extern AIENTRY ai_Entries[32];          /* 0xf88a40 */
extern AIWATCH ai_Watch;                /* 0xf895a0 */
#ifndef DATA_VIEWS_H
extern AIMAPREF ai_MapRefs[];         /* 0xfd7c4c, @original: rd.h declares G15MAP * (its rd_BuildRoad 0x4bcc20 loads the first record's map: [0xfd7c4c], then [eax] and [eax+0x1a64]); ai.c indexes the 0x3e0-byte records (ai_MapRefs[bld].map) */
#endif
extern const unsigned int ai_BuildingTypeMasks[4];  /* 0x5eed70, bit masks of building types (two 64 bit masks) */
extern float ai_DifficultyValue;              /* 0x5fef04 */
extern float ai_DifficultyScale;              /* 0x5fef08 */
extern unsigned char ai_NumPlayers;    /* 0x62489c, the number of players */
extern char ai_AmbushSpots[8][32];        /* 0x625720, "sp_AUFLAUERLEGEN_NORDEN" ... */
extern AICHAR *ai_Partner;            /* 0x6f0468 */
extern int ai_TradeMsgCount;                /* 0x6f046c, the trade messages sent since the last reset */
extern float ai_Rating;               /* 0x6f1ba0, the smoothed game rating (statistics out[5]; the running rating ai_RatingBase follows it) */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
extern AIBLD *ai_MarketBuilding;             /* 0x6f1bc8, the market building (gm_BuildQuery(1, 5, 10)); @original: f3snd.h declares G16BLDVIEW * (f3snd.c reads its +0x4c / object fields); ai.c reads the AIBLD fields (->id) */
extern unsigned short *ai_PlayerChar;    /* 0x6f1bf8 */
extern char *ai_PlayerBuilding;          /* 0x6f2308, the building the player is in: an AIBLD (sv.c defines char *; ai_PlayerBuildingT views) */
extern char *ai_FallbackBuilding;        /* 0x6f230c, the second current building, used when 0x6f2308 is 0 (sv.c defines char *) */
extern int ai_CurrentPlayer;                /* 0x6f2590, the player index whose turn the local round loop simulates */
extern int ai_Difficulty;                 /* 0x6f23fc, the game difficulty 0 .. 4 */
extern AISTORE gm_Offers[];          /* 0x7e4b20 */
extern AIGRID ai_Grid[8][8];            /* 0x91ae60 */
extern char *cm_FieldObject;              /* 0xa68d2c */
#ifndef DATA_VIEWS_H
extern unsigned int ai_ActionFlags[][37]; /* 0xf83f50, 0x94 byte entries, flags first */
#endif
extern int ai_TradeMsgOff;              /* 0xf86a24, read-only in every source: nonzero suppresses the trade
                                           message and its count (never set in this build) */
extern AIOBJ *ai_Carts[16];             /* 0xf89540, the master's kind-0x1d cart objects without a running
                                           job (ai_Unk483a60 fills them, ai_NumCarts counts them) */
extern int ai_UpdateStart;              /* 0xf89580, the character index the per-frame update share starts at */
extern AITIME ai_LastUpdateTime;        /* 0xf89584, the game time of the last per-frame AI update */
extern float ai_CountCarry;             /* 0xf89594, the fractional character count carried to the next frame */
extern int ai_WeaponList2[10];          /* 0xf8a0a0, goods ids of the second weapon list (0-terminated; ends before 0xf8a0c8) */
extern AICHAR *ai_LoggedMaster;            /* 0xf8a0c8, the master whose decisions are logged */
extern int ai_NumCarts;                 /* 0xf8a0cc, how many of ai_Carts are set (0 .. 16) */
extern int ai_WeaponList[];           /* 0xf8a0e0, goods ids of the first weapon list (0-terminated) */
extern int ai_NetPlayer;                /* 0x1460af4, the player index this client runs (the host sets it, cm.c message type 7) */

/* this unit, in address order */
int ai_Init(void);                                                  /* 0x47d9e0 */
void ai_UpdateNeeds(void);                                                 /* 0x47da80 */
void ai_GrowNeeds(AICHAR *c);                                            /* 0x47dab0 */
int ai_WorkLimit(void);                                                  /* 0x47dd00, the AI's work limit from the rating */
int ai_WorkLoad(void);                                                   /* 0x47dd30, the current AI work load */
int ai_WorksForMaster(AICHAR *c);                                             /* 0x47dd70 */
int ai_IsLocalMaster(AICHAR *c);                                             /* 0x47dd90, the master this machine runs */
AICHAR *ai_FindPartner(AICHAR *c);                                         /* 0x47ddf0 */
void ai_MasterUpdate(AICHAR *c);                                            /* 0x47e360 */
void ai_UpdateCharacters(void);                                                 /* 0x47e560 */
int ai_CanAct(AICHAR *c);                                             /* 0x47e7f0 */
int ai_IsLocalWorker(AICHAR *c);                                             /* 0x47e840 */
int ai_IsLocalJob(AICHAR *c);                                             /* 0x47e8b0 */
void ai_ResetJob(AICHAR *c);                                            /* 0x47e940 */
void ai_Unk47e990(AICHAR *c);                                            /* 0x47e990 */
void ai_PlanSlots(AICHAR *ch, AIOBJ *b);                                 /* 0x47ec20 */
void ai_SendRemoveEmptyGoods(AIOBJ *stall);                                          /* 0x47ed60 */
void ai_EvaluateProduction(AICHAR *ch);                                           /* 0x47edb0 */
void ai_EvaluateProductionFlags(AICHAR *ch);                                           /* 0x47f700 */
void ai_EvaluateProductionCategory2(AICHAR *ch);                                           /* 0x4801f0 */
void ai_CalcMeisterWache(AICHAR *ch);                                    /* 0x480e70, the guard master */
int ai_CalcAngriff(AICHAR *ch, int n);                                   /* 0x4817a0, spy on a building, attack it */
void ai_CalcMeisterDiebe(AICHAR *ch);                                    /* 0x481ef0, the thieves' master */
void ai_CalcMeisterRaeuber(AICHAR *ch);                                  /* 0x482f00, the robber master */
void ai_CalcMeisterBank(AICHAR *ch);                                           /* 0x4836a0, the bank master's money balance */
void ai_Unk483a60(AICHAR *ch);                                           /* 0x483a60 */
void ai_StartBuildingWork(AICHAR *ch);                                           /* 0x483d30 */
void ai_EndWorkerPose(AICHAR *ch);                                           /* 0x483ee0 */
void ai_Unk483fc0(AICHAR *c);                                            /* 0x483fc0 */
void ai_Unk484a60(AICHAR *c);                                            /* 0x484a60 */
void ai_UpdateProduction(AICHAR *ch, AIOBJ *a, AIOBJ *b);                       /* 0x4854f0 */
void ai_FillSlotTable(AICHAR *ch, AIOBJ *obj);                               /* 0x486190 */
void ai_BuildTradeOffers(AICHAR *ch, AIOBJ *store, int f);                      /* 0x4864b0 */
void ai_CalcEntryCost(AIENTRY *e);                                           /* 0x487040 */
int ai_HasMaterials(AICHAR *ch, AIENTRY *e, int f);                         /* 0x487180 */
int ai_StartProduction(AICHAR *ch, AIENTRY *e);                                /* 0x487410 */
void ai_AddEquipmentNeeds(AICHAR *ch, AIOBJ *obj, int v);                        /* 0x487780 */
void ai_UpdateStaff(AICHAR *ch);                                           /* 0x487b40 */
void ai_Unk487ea0(AICHAR *ch);                                           /* 0x487ea0 */
void ai_BuyUpgrade(AICHAR *ch);                                           /* 0x487fb0 */
void ai_BuySlots(AIOBJ *b, AICHAR *ch, int n);                          /* 0x4884b0 */
void ai_BuyCapacity(AIOBJ *b, AICHAR *ch, int n);                          /* 0x488740 */
void ai_FreeJobCharacters(AICHAR *ch, int n);                                    /* 0x4888c0 */
void ai_SendJobOrders(AIINFO *info);                                         /* 0x4889f0 */
int ai_CheckFreeThieves(AICHAR *ch);                                            /* 0x489300, a thief master: free characters, what they know */
int ai_Unk489480(AICHAR *ch);                                            /* 0x489480, the same for three skills */
void ai_SendFreeThieves(AICHAR *ch);                                           /* 0x489660, sends the free thieves out */
void ai_SendFreeSpies(AICHAR *ch);                                           /* 0x489870, sends the free spies out */
void ai_MeisterBewaffnen(AICHAR *ch, AIOBJ *obj);                        /* 0x489a70, hands a weapon out of a slot */
void ai_MeisterCollectTrans(AICHAR *ch);                                 /* 0x489cb0, finds / buys the carts */
void ai_UnloadCarts(AICHAR *c);                                            /* 0x48a380 */
int ai_MeisterTrade(AICHAR *ch, AIOBJ *b);                               /* 0x48a4d0, a master's cart trades */
int ai_MeisterTradeAllgemein(AICHAR *ch, AIOBJ *b);                      /* 0x48bc80 */
int ai_MeisterTradeFernEinkauf(AICHAR *ch, AIOBJ *obj);                  /* 0x48d010, the long-distance buyer */
int ai_MeisterTradeFernEinkauf2(AICHAR *ch, AIOBJ *obj);                 /* 0x48d810, its variant that sells entry 0 first
                                                                            (same log strings) */

/* callees outside this unit */
int util_Random(unsigned short n);                                       /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
struct G15TREE * veg_FindType(int type);                                         /* 0x4bc5a0 */
struct G15PLANT * veg_FindPlantId(struct G15PLANT *p, int id);                /* 0x4bc670, the plant record with the id (rd.c) */
void cm_BeginFieldBlock(char *o, int id);                             /* 0x4c3ba0, starts a message about o */
int cm_AddFieldDelta(int size, int n, short field, void *v);                /* 0x4c3c00, adds a field */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                  /* 0x4c3d00 */
void cm_BeginGroup(const char *s);                                        /* 0x4c4540 */
void cm_EndGroup(const char *s);                                        /* 0x4c4580 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                 /* 0x4c47e0 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);         /* 0x4c4880, cm.c */
void cm_SendFieldDeltas(void);                                                 /* 0x4c49a0, sends the message */
int cm_SendRelations(int a, int b, int c, int d);                  /* 0x4c4b00 */
int cm_SendLongMessage(char *msg);                                           /* 0x4c4b70 */
char * ai_Unk4cd9d0(int id);                                              /* 0x4cd9d0 */
int gv_EntryIndex(char *r, int *e);                                        /* 0x4ef6b0 */
int ord_GetGridCell(char *p, int *x, int *y);                               /* 0x4f8300 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                       /* 0x4fe140 */
void gm_SetTime(struct GCLOCK *c, unsigned char hour, unsigned min, unsigned sec);                   /* 0x4fe1e0, the gm_SetTime of f3gm.c in the AITIME view */
int gm_MinutesBetween(AITIME *a, AITIME *b);                                  /* 0x4fe280, minutes between two times */
int gm_FindObjectInst(int id);                                              /* 0x4fe980 */
int gm_FindById(int *bld, int *obj, int *chr, int id);           /* 0x4fea50 */
unsigned short * gm_QueryNext(void);                                               /* 0x4ffcc0, next of gm_Query */
char * gm_Query(void *list, int n, ...);                               /* 0x4ffe90, n (key, value) pairs */
int gm_FindMarketObject(int id);                                            /* 0x500ec0 */
unsigned char * gm_BuildNext(void);                                               /* 0x501030, next of gm_BuildQuery */
unsigned char * gm_BuildQuery(int n, ...);                                         /* 0x501190, n (key, value) filters; key 6 takes no value */
int gm_BuildingClass(int type);    /* 0x501d80, the class 0 .. 9 of ai_BTypes[type].kind: kinds 1 .. 0x20 index the byte table 0x501e14 and jump through 0x501dec (1..9), anything else is 0; ai.c dispatches its master / workshop logic on it, f3init.c's init_Betrieb stocks the classes 1 / 4 */
char * gm_FindBuildingById(int id);                                             /* 0x502020 */
short * gm_FindChar(int id);                                            /* 0x505df0 */
int gm_CharLevel(unsigned short id, int again);                              /* 0x505e60, f3gm.c */
short * gm_PlayerRecord(char *ch);                                         /* 0x506430 */
float gm_ItemPrice(int item, unsigned char n);                           /* 0x5087f0 */
float gm_GoodsPrice(unsigned id, unsigned char p);                           /* 0x5089c0, a price */
int gm_EquipmentValue(unsigned char *b, int unused);                                     /* 0x509b10 */
int gm_ListAmount(short *l, char *o);                                      /* 0x50aaf0 */
float gm_Liking(unsigned a, unsigned b, int c);                                 /* 0x50c550 */
char * he_NextEvent(void);                                             /* 0x53e300, next of he_FindEvent */
char * he_FindEvent(int n, ...);                                         /* 0x53e400, n (key, value) pairs */

#endif
