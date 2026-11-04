/* The job record of f3gm.c's table 0x61a688 (gm_GetJobTalents returns it by value), for the units that use it
   without the rest of f3gm.h: types only. f3gm.h includes it. */
#ifndef F3GM_TYPES_H
#define F3GM_TYPES_H

typedef struct JOBTALENTS {             /* 6 bytes, one per job at 0x61a688 */
    unsigned char skills[5];            /* 0x0, the five skill bytes a character of the job starts with (0x507690 ages them into a new character's +0x80..+0x84, the player profile copies them into its talents) */
    unsigned char rank;                 /* 0x5, 1..4 in the table: a guess from the values and the rank byte +0xd */
} JOBTALENTS;

typedef struct GMHEAD {                 /* 0x44 bytes, a head record (gm_CharNameRecord's tables 0x6118a8 .. 0x617398) */
    int key;                            /* 0x00, the job / title id the record belongs to, 0 = the end of a table */
    char name[0x30];                    /* 0x04, the model name, "<name>_<suffix>" (bergmann3_MANN) */
    int heads[4];                       /* 0x34, the icon numbers of the four heads, 0xff = not resolved yet (gm_ResolveHeadIcons) */
} GMHEAD;

typedef struct GMINCOME {               /* 0x38 bytes, a character's incomes (0x4fa4b0 clears it, the kind functions fill the first free entry) */
    unsigned char kind[6];              /* 0x00, the income kind of each entry (1 .. 5), 0 = free */
    char pad06[2];
    int amount[6];                      /* 0x08, the amount */
    int rate[6];                        /* 0x20, the rate in percent it was computed with (AIDEC.cur) */
} GMINCOME;

#include "ai_types.h"

#pragma pack(push, 1)
/* a goods prototype (ai.h AIGOOD, 0x41 bytes, ai_Goods) as this unit reads it: ai.h is not includable here */
typedef struct GMGOOD {
    char type;                          /* 0x00 */
    char name[0x20];                    /* 0x01 */
    unsigned char cls;                  /* 0x21, the goods class (gm_ClassBuilding's index) */
    unsigned int workTime;              /* 0x22, AIGOOD.workTime, the production time of one unit */
    unsigned short need[4];             /* 0x26 */
    unsigned short in[4];               /* 0x2e, the goods it is made of */
    unsigned short batch;               /* 0x36, AIGOOD.batch, the amount one production run makes */
    int price;                          /* 0x38, the cached price of 100 pieces / 32 (gm_ItemPrice), 0: not computed yet */
    char pad3c[4];
    unsigned char b40;                  /* 0x40 */
} GMGOOD;
#pragma pack(pop)
typedef char gm_AssertGMGOOD[sizeof(GMGOOD) == 0x41 ? 1 : -1];
#define GM_GOOD(i) ((GMGOOD *)ai_Goods + (i))  /* the goods prototype i of ai_Goods (char *) */

#pragma pack(push, 1)
/* a building type (ai.h AIBTYPE, 0x24d bytes, ai_BTypes) as this unit reads it: ai.h is not includable here */
typedef struct GMBTYPE {
    char kind;                          /* 0x000 */
    char pad001[0x21];
    unsigned char objCount;             /* 0x022, the entries of objs used (gm_LayoutTree) */
    unsigned short objs[0x40];          /* 0x023, the objects the type can hold, goods ids (bit 15 = flag) */
    unsigned short parents[0x40][2];    /* 0x0a3, the two goods each of objs needs first (gm_LayoutTree's tree) */
    char pad1a3[0x8c];
    char title;                         /* 0x22f, the title rank that suits the type's workers (ORDCHAR.title: +0.3 each, gm_StaffFactor) */
    char pad230[0x17];
    unsigned char level;                /* 0x247, the type's level 1 .. 3 (ai.h AIBTYPE.level / cap247) */
    unsigned char maxLevel;             /* 0x248, the highest level of the type's line (ai.h AIBTYPE.maxLevel) */
    int baseValue;                      /* 0x249, the type's own value, x 0xf00 the base of gm_EquipmentValue (reasoned: the worth before the equipment) */
} GMBTYPE;
#pragma pack(pop)
typedef char gm_AssertGMBTYPE[sizeof(GMBTYPE) == 0x24d ? 1 : -1];
#define GM_BTYPE(i) ((GMBTYPE *)ai_BTypes + (i))  /* the building type i of ai_BTypes (char *) */

/* an event of the kind 0x23 (he_FindEvent(1, 0, 0x23)) as gm_BuildingEvents / gm_Unk50a150 read it */
typedef struct GMEVENT23 {
    char pad000[0xac];
    int bldId;                          /* 0x0ac, the building's id (ORDBLD.id) */
    int charId;                         /* 0x0b0, the character's netId (ORDCHAR.netId) */
} GMEVENT23;


/* the long message (ai.h AIMSG, 0xf8 bytes, cm_SendLongMessage) as this unit fills it: ai.h is not includable here */
typedef struct GMMSG {
    int unk00;                          /* 0x00 */
    unsigned char type;                 /* 0x04, 0x72: a character died */
    char pad05[3];
    int to;                             /* 0x08, the netId the message is about (AIMSG.to) */
    int fromId;                         /* 0x0c, -1: no building */
    char pad10[0x18];
    AITIME time;                        /* 0x28, the game time */
    unsigned char flags;                /* 0x36 */
    char pad37[0x21];
    int arg0;                           /* 0x58, AIMSG.arg[0]: the character's netId again */
    int arg1;                           /* 0x5c, AIMSG.arg[1] */
    char pad60[0x98];
} GMMSG;

/* the character statistics gm_CharacterStatistics (0x91b500, 0x68 bytes) as gm_CountCharacters (0x4fdfd0) fills
   them: per age bucket (gm_CharAge / 10, clamped to 0 .. 7) the count and the average health / base health */
typedef struct GMCHARSTATS {
    int count;                          /* 0x00, the living persons of kind < 10 */
    int ageCount[8];                    /* 0x04, the persons per age bucket */
    float underAge;                     /* 0x24, the share of the persons under age (gm_IsUnderAge), in percent */
    float health[8];                    /* 0x28, per age bucket: gm_CharacterHealth summed, then in percent of baseHealth */
    float baseHealth[8];                /* 0x48, per age bucket: gm_CharacterBaseHealth summed, then the average */
} GMCHARSTATS;

/* the payload of a character's skill-bonus event (he_FindEvent (2, 2, character, 0, 0x82)): the two skill
   numbers of the event and the two float bonuses gm_GetSkill adds for them */
typedef struct GMSKILLEV {
    char pad00[0xac];
    unsigned char skill1;               /* 0xac, the first skill number (0 .. 4) */
    unsigned char skill2;               /* 0xad, the second skill number */
    char pad0ae[2];
    float bonus1;                       /* 0xb0, the bonus added for skill1 */
    float bonus2;                       /* 0xb4, the bonus added for skill2 */
} GMSKILLEV;

/* a disease kind's record (12 bytes, ten of them at 0x617850, gm_GetDisease copies one out): kinds 1 .. 9 are the
   bit fields of a character's word +0x2c (gm_CatchDisease) */
typedef struct GMDISEASE {
    int kind;                           /* 0x00, the record's own kind (0 .. 9) */
    float cost;                         /* 0x04, the health a catch costs per point of severity */
    int range;                          /* 0x08, the severity is util_Random(range) */
} GMDISEASE;

/* a player's goods record (0x80 bytes; 0x55 of them at 0x7d6a70 + player * 0x2a90, gm_StockBlocks + 0x10; the
   template gm_DefaultStock has the same layout) */
typedef struct GMSTOCK {
    unsigned short item;                /* 0x00, the goods prototype (an ai_Goods index) */
    char pad02[2];
    int amount;                         /* 0x04, the amount the player holds */
    float rate;                         /* 0x08, the demand per unit of the rating (player 0) or of the block's demand index (gm_GoodsDemand) */
    int v00c;                           /* 0x0c, copied from the template (gm_InitPlayerRecord); 0, 40, 80 or -1 there */
    int daily[4];                       /* 0x10, four counters gm_UpdatePrices clears every day */
    float demand;                       /* 0x20, the amount the market wants (gm_GoodsDemand) */
    float mean;                         /* 0x24, the mean amount */
    unsigned char offer;                /* 0x28, another player's offer counter: random 0 .. 0xff, then + 0x18 .. 0x1f per market sync; up to 0x80 the good is offered (gm_SyncMarket) */
    char pad29[3];
    float f2c;                          /* 0x2c, a float sent with the other deltas of message 0x40 (cm_SendGoodsDelta; meaning unknown) */
    int baseStock;                      /* 0x30, the default stock (from the template; gm_UpdatePrices gives it to the players other than the own one as +0x34) */
    int stock;                          /* 0x34, the stock of the kinds 0x17 / 0x25 (gm_GoodsStock) */
    float price;                        /* 0x38, the stored price (gm_ItemPrice of the item, 100) */
    unsigned short marketKind;          /* 0x3c, the kind of the market object (0x92 .. 0x97, 0x2db) that holds the good (gm_SyncMarket) */
    char pad3e[2];
    unsigned char demandIsStock;        /* 0x40, set: gm_UpdatePrices takes the stock as the demand */
    char pad41[0x3f];
} GMSTOCK;

/* a player's stock block (0x2a90 bytes, five of them at gm_StockBlocks 0x7d6a60): two seasonal indices in
   permille (gm_UpdatePrices interpolates them from the player slot's tables and scales them by their factors),
   then the goods records */
typedef struct GMBLOCK {
    int demandIndex;                    /* 0x00, the demand index (gm_GoodsDemand scales the demand by it) */
    int demandFactor;                   /* 0x04, its factor in permille */
    int indexB;                         /* 0x08, the index of the second table (no other reader known) */
    int factorB;                        /* 0x0c, its factor in permille */
    GMSTOCK goods[0x55];                /* 0x10 */
} GMBLOCK;

/* a point of a player slot's yearly tables (ten of them at ai_PlayerSlots[p] + 0x64 and + 0xb4, increasing
   years until the first that is not; gm_UpdatePrices interpolates between them by month) */
typedef struct GMPOINT {
    int year;
    int value;
} GMPOINT;

/* a player slot (0x2f4 bytes, four of them at ai_PlayerSlots 0x7e3f40, declared char[4][0x2f4]) as far as known */
typedef struct GMPLAYERSLOT {
    char name[0x40];                    /* 0x00, the player's name (ls_WriteSaveHeader copies it into LSHDR.player); the
                                           size is reasoned: up to mapX */
    int map[2];                         /* 0x40, the player's place on the map, x / y (am.c 0x4948f0 measures gm_MapDistance
                                           from it to the other slots' places; the save header keeps both) */
    unsigned char b48;                  /* 0x48, saved in the save header (LSHDR.playerByte); the meaning is unknown */
    char pad049[0xb];
    unsigned short moneyGood;           /* 0x54, the good the player's money is counted in (cm.c 0x4c70b0 pays a sale's price
                                           with gm_AddObjekt / gm_TakeObjekt of it) */
    char pad056[0xe];
    GMPOINT tableA[10];                 /* 0x64, the yearly tables gm_UpdatePrices interpolates */
    GMPOINT tableB[10];                 /* 0xb4 */
    char pad104[0x1f0];
} GMPLAYERSLOT;

/* a player's line of the ranking gm_RankPlayers fills (0x24 bytes; the names from its log format 0x61ac40) */
typedef struct GMRANK {
    unsigned short id;                  /* 0x00, the character index */
    short pad02;
    int worth;                          /* 0x04, "vermoegen": gm_GetWorth, at least 1 */
    float worthShare;                   /* 0x08, "vermoegen_trend": worth / the highest worth of the ranked */
    float title;                        /* 0x0c, the character's level / 9 */
    float office;                       /* 0x10, "amt": the kind levels of its two jobs / 13 */
    float reputation;                   /* 0x14, "ansehen": the mean liking of the other players and townsmen / 100 */
    float talents;                      /* 0x18, "talente": the sum of the five skills / 1260 */
    float sum;                          /* 0x1c, the weighted sum of the five values */
    int points;                         /* 0x20, sum * the highest worth / 10000 */
} GMRANK;

/* the calendar record gm_ClockToDate fills (10 bytes) */
typedef struct GMDATE {
    unsigned char one;                  /* 0x0, always 1 */
    char month;                         /* 0x1, the first month of the season: season * 3 + 1 */
    short year;                         /* 0x2, day + 0x578 */
    unsigned char hour;                 /* 0x4 */
    unsigned char min;                  /* 0x5 */
    unsigned char sec[4];               /* 0x6, the clock's seconds field (an unaligned int) */
} GMDATE;

#endif
