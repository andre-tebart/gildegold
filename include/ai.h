struct AITIME;
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
#include "ai_bld.h"                  /* AIBLD */

/* AIGOOD: ai_types.h */

/* AIBTYPE: ai_bld.h */
#pragma pack(pop)

#include "ai_types.h"                /* AITIME, AISTORE, AIOBJ */

#include "ai_char.h"                 /* AITASK, AICHAR */

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
    float margin;                       /* 0x34, sort key and profit per unit: (price - cost) / AIGOOD.workTime */
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
    int cleared10;                      /* 0x10, cleared with every new slot (ai_UpdateProduction, ai_FillSlotTable,
                                           ai_BuildTradeOffers); no reader in the source (reasoned: unused) */
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

typedef struct AIWATCH {                /* 0xb00 bytes at 0xf895a0, what the watched building's masters did */
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
    char pad700[0x400];                 /* 0x700 .. 0xaff, no reader known (cm.c's daily clear covers 0xb00 bytes) */
} AIWATCH;

typedef struct AIGRID {                 /* 24 bytes, a cell of the statistics grid ai_Grid[8][8] */
    unsigned short a;                   /* 0x00, the sum over the 3 x 3 neighbourhood */
    unsigned short b;                   /* 0x02, the sum over the cell itself */
    unsigned char used;                 /* 0x04, the number of objects of the cell (ord.c reads it unsigned: and eax,0xff) */
    char pad05[3];
    int average;                        /* 0x08, the average equipment value of the cell's objects (0x4f8360) */
    float halfWeightSum;                /* 0x0c, the sum of half the weights w of 0x4f8920 (w * 0.5f) */
    float weightSum;                    /* 0x10, the sum of the weights w of 0x4f8920 */
    float buildingCount;                /* 0x14, 0x4f8ae0 adds 1.0 per building object of the cell */
} AIGRID;

typedef struct AIPOS {                  /* filled by gv_GetTableRecord */
    char pad00[0x18];
    int unk18;                          /* 0x18 */
    int unk1c;
    int unk20;
} AIPOS;

typedef struct AIFIN {                  /* what gm_PlayerRecord finds for a character */
    unsigned short w00;                 /* 0x00, the character's AICHAR.w050 (cm.c 0x4c6cd0 copies it) */
    char family[0x10];                  /* 0x02, the family name (cm.c 0x4c6cd0 sets it with the character's) */
    char pad12[2];                      /* 0x12 */
    int incomeA;                        /* 0x14, a round income part (the round report prints it with text 0x200d, then adds it
                                           with incomeB to missionMoney[0] and clears it; the kind is a reasoned guess open) */
    int incomeB;                        /* 0x18, a round income part (text 0x200c, rolled into missionMoney[0]) */
    int lootTakings;                    /* 0x1c, the loot value a won fight / raid took from the loser's goods
                                           (cm_LootStock and cm_LootCarts add it to the winner's record) */
    int incomeC;                        /* 0x20, a round income part (text 0x2010, rolled into missionMoney[2]) */
    int madeValue;                      /* 0x24, the price value of the goods its workshops made (cm.c 0x4c78c0 adds it
                                           with the building's AIBLD.roundSums[0]) */
    int spyPaid;                        /* 0x28, the money the player paid to the cabinet's spies (ai_CabinetSpyWindow adds each payment) */
    int missionMoney[5];                /* 0x2c .. 0x3f, the money statistics: the round report (main_RoundReport) adds the
                                           round parts in ([0] incomeA + incomeB, [1] lootTakings, [2] incomeC, [3]
                                           madeValue, [4] the character's AICHAR.v1a4 + v1a0); read by the mission conditions, converted with
                                           gm_ConvertMoney: 0x38 by the types 1 / 6 / 14 / 22 / 32, 0x2c + 0x34 by
                                           20 / 25 / 36, 0x30 by 26 / 37, 0x3c by 27 */
    int upgradeSpent;                   /* 0x40, the money the record has paid for workshop upgrades (ai_BuyUpgrade) */
    int renovationSpent;                /* 0x44, the money it has paid for renovations (ai_BuyUpgrade, am.c 0x497ee0) */
    int loansRepaid;                    /* 0x48, the loan sums the character has paid back at a money lender (ai_RepayLoan adds each) */
    int costA;                          /* 0x4c, a round cost part (the round report prints it with text 0x201a and clears it) */
    int cartSpent;                      /* 0x50, the money the character has paid for new carts (lc.c ai_BuyCartWindow adds each price) */
    int costB;                          /* 0x54, a round cost part (text 0x2015, cleared by the round report) */
    float roundFactors[3];              /* 0x58, set to 0.5, 1, 1 at every round change (main.c 0x5384e0); [0] the bias for who starts a dice duel
                                           (ai_DuelStart, am.h AGFIN.firstBias), [1] the chance of a tavern dice invitation (ai_TavernDiceWindow) */
    int saved64[2];                     /* 0x64, two ints the save game keeps (ls_ReadPlayerRecords from version 0x10014);
                                           no other reader known */
    int officeSentences;                /* 0x6c, the sentences 4 / 6 / 8 its character passed as the judge of a trial on an
                                           office holder (cm.c 0x4db700) */
    int saved70;                        /* 0x70, an int the save game keeps (from version 0x10018); no other reader known */
    unsigned char jobByGroup[0x11];     /* 0x74, one job id per title group (am.h AIFINX.jobByGroup; gm_SetJob fills it) */
    char pad85[3];
    float camAngle;                     /* 0x88, the player's camera angle factor (sv_CameraAngleFactor; sv.c) */
    float camPos[3];                    /* 0x8c, the player's camera position, restored with the view (sv.c) */
    char pad98[4];
    float camRot[3];                    /* 0x9c, its rotation (D3OBJECT.rotation) */
    int savedA8;                        /* 0xa8, an int the save game keeps; no other reader known */
} AIFIN;                                /* 0xac bytes, the 16 records of gm_PlayerRecords */

typedef struct AIMAPENT {               /* 24 bytes, the AI's view of the vegetation records at AIBLD.plants (rd.h
                                           G15PLANT, the same 0x18-byte layout; ai.c includes rd.h now, so the readers
                                           can take G15PLANT) */
    int id;                             /* 0x00, G15PLANT.id */
    int bldId;                          /* 0x04, G15PLANT.bldId, the building whose map holds it */
    unsigned char x;                    /* 0x08, cell */
    unsigned char z;                    /* 0x09 */
    unsigned short type;                /* 0x0a, G15PLANT.type, the plant type (0 removes it) */
    char size;                          /* 0x0c, size in cells */
    char state;                         /* 0x0d, growth state; -1 free (veg_FindPlantId skips those) */
    char pad0e[2];
    int fallId;                         /* 0x10, G15PLANT.fallId, -1 for a plain plant (veg_SetPlant) */
    int obj;                            /* 0x14, its 3D object */
} AIMAPENT;

#define AI_KIND(t) (((int (*)(int))gm_BuildingClass)(t))   /* the call zero-extends the byte (xor ecx,ecx; mov cl) */

extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
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
extern const unsigned int ai_BuildingTypeMasks[6];  /* 0x5eed70, bit masks of building classes (three 64 bit masks) */
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
typedef struct AIUPDATE {               /* 0x18 bytes, the per-frame character update cycle (ai_Update): the save
                                           writes and reads it as one block (ls.c, vfs_fwrite at 0x5191ad) */
    int start;                          /* 0x00, the character index the per-frame update share starts at */
    AITIME last;                        /* 0x04, the game time of the last per-frame AI update */
    float carry;                        /* 0x14, the fractional character count carried to the next frame */
} AIUPDATE;
extern AIUPDATE ai_Update;              /* 0xf89580 */
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
void ai_RunMasterEvaluation(AICHAR *c);                                            /* 0x47e990 */
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
void ai_EvaluateProductionGraveyard(AICHAR *c);                                            /* 0x483fc0 */
void ai_EvaluateProductionGypsies(AICHAR *c);                                            /* 0x484a60 */
void ai_UpdateProduction(AICHAR *ch, AIOBJ *a, AIOBJ *b);                       /* 0x4854f0 */
void ai_FillSlotTable(AICHAR *ch, AIOBJ *obj);                               /* 0x486190 */
void ai_BuildTradeOffers(AICHAR *ch, AIOBJ *store, int f);                      /* 0x4864b0 */
void ai_CalcEntryCost(AIENTRY *e);                                           /* 0x487040 */
int ai_HasMaterials(AICHAR *ch, AIENTRY *e, int f);                         /* 0x487180 */
int ai_StartProduction(AICHAR *ch, AIENTRY *e);                                /* 0x487410 */
void ai_AddEquipmentNeeds(AICHAR *ch, AIOBJ *obj, int v);                        /* 0x487780 */
void ai_UpdateStaff(AICHAR *ch);                                           /* 0x487b40 */
void ai_OfferGrave(AICHAR *ch);                                           /* 0x487ea0 */
void ai_BuyUpgrade(AICHAR *ch);                                           /* 0x487fb0 */
void ai_BuySlots(AIOBJ *b, AICHAR *ch, int n);                          /* 0x4884b0 */
void ai_BuyCapacity(AIOBJ *b, AICHAR *ch, int n);                          /* 0x488740 */
void ai_FreeJobCharacters(AICHAR *ch, int n);                                    /* 0x4888c0 */
void ai_SendJobOrders(AIINFO *info);                                         /* 0x4889f0 */
int ai_CheckFreeThieves(AICHAR *ch);                                            /* 0x489300, a thief master: free characters, what they know */
int ai_CheckFreeStaff(AICHAR *ch);                                            /* 0x489480, a three-skill master: free characters (job none / 4 / 0x8c) */
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
int util_Random(unsigned short n);                                       /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, cm_FightLoop ..) call util_RandomU, a cast in their head */
struct G15TREE * veg_FindType(int type);                                         /* 0x4bc5a0 */
struct G15PLANT * veg_FindPlantId(struct G15PLANT *p, int id);                /* 0x4bc670, the plant record with the id (rd.c) */
void cm_BeginFieldBlock(char *o, int id);                             /* 0x4c3ba0, starts a message about o */
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);                /* 0x4c3c00, adds a field */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                  /* 0x4c3d00 */
void cm_BeginGroup(const char *s);                                        /* 0x4c4540 */
int cm_EndGroup(const char *s);                                        /* 0x4c4580 */
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);                 /* 0x4c47e0 */
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);         /* 0x4c4880, cm.c */
int cm_SendFieldDeltas(void);                                                 /* 0x4c49a0, sends the message */
int cm_SendRelations(int a, int b, int c, int d);                  /* 0x4c4b00 */
int cm_SendLongMessage(char *msg);                                           /* 0x4c4b70 */
char * ai_FindWeaponRecord(int id);                                              /* 0x4cd9d0 */
int gv_EntryIndex(char *r, int *e);                                        /* 0x4ef6b0 */
int ord_GetGridCell(char *p, int *x, int *y);                               /* 0x4f8300 */
void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);                       /* 0x4fe140 */
void gm_SetTime(struct AITIME *c, unsigned char hour, unsigned min, unsigned sec);                   /* 0x4fe1e0, f3gm.c: sets the time of day of the clock c */
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
int gm_EquipmentValue(unsigned char *b, int level);                                     /* 0x509b10 */
int gm_ListAmount(short *l, char *o);                                      /* 0x50aaf0 */
float gm_Liking(unsigned a, unsigned b, int c);                                 /* 0x50c550 */
char * he_NextEvent(void);                                             /* 0x53e300, next of he_FindEvent */
char * he_FindEvent(int n, ...);                                         /* 0x53e400, n (key, value) pairs */

#endif
