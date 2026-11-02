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

#endif
