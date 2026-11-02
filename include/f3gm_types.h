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
    char pad5c[0x9c];
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

/* a player's goods record (0x80 bytes; 0x55 of them at 0x7d6a70 + player * 0x2a90, gm_StockBlocks + 0x10; the
   template gm_DefaultStock has the same layout): f3init.c's INIT_STOCK, hst.c's G12STOCK */
typedef struct GMSTOCK {
    unsigned short item;                /* 0x00, the goods prototype (an ai_Goods index) */
    char pad02[2];
    int amount;                         /* 0x04, the amount the player holds */
    char pad08[4];
    int v00c;                           /* 0x0c, copied from the template (gm_InitPlayerRecord); 0, 40, 80 or -1 there */
    char pad10[0x10];
    float demand;                       /* 0x20, the amount the market wants (gm_GoodsDemand) */
    float mean;                         /* 0x24, the mean amount */
    unsigned char offer;                /* 0x28, another player's offer counter: random 0 .. 0xff, then + 0x18 .. 0x1f per market sync; up to 0x80 the good is offered (gm_SyncMarket) */
    char pad29[7];
    int baseStock;                      /* 0x30, the default stock (from the template; gm_UpdatePrices gives it to the players other than the own one as +0x34) */
    int stock;                          /* 0x34, the stock of the kinds 0x17 / 0x25 (gm_GoodsStock) */
    float price;                        /* 0x38, the stored price (gm_ItemPrice of the item, 100) */
    unsigned short marketKind;          /* 0x3c, the kind of the market object (0x92 .. 0x97, 0x2db) that holds the good (gm_SyncMarket) */
    char pad3e[0x42];
} GMSTOCK;

#endif
