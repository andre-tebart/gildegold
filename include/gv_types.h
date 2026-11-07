/* The order statistics records of gv.h (ord.c's ord_Statistics, ord_StatisticsHistory, ord_MessageRecord), the wage record ORDWAGES (gm_CharacterWages) and the AI
   decision record AIDEC (gv_GetTableRecord) and a graveyard's list entry GVENTRY (gv_ListEntry, he.c's graveyard work), for the units that declare them without the rest of gv.h: types only. gv.h includes it. */
#ifndef GV_TYPES_H
#define GV_TYPES_H

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

typedef struct GVENTRY {                /* 0x14 bytes, an entry of a graveyard's lists (gv_AllocGraveyard) */
    int size;                           /* 0x00, the list (grave size) 0 .. 3 it belongs to */
    int id;                             /* 0x04, -1 when free */
    unsigned short w08;                 /* 0x08, 0xffff when free (0x7fff for a new dead person) */
    unsigned short until;               /* 0x0a, ai_Time.year + 5 at the burial (a guess: the year the grave runs out) */
    unsigned char b0c;                  /* 0x0c, 0xff for a new dead person */
    char pad0d[3];
    struct GVGRAVE *grave;              /* 0x10, its grave record (0x44 << size bytes) in GVYARD.graves */
} GVENTRY;

/* the order statistics (ord.c 0x4f82a0 ..): the last statistics, the history of the rounds and the message
   event record */
typedef struct ORDSTATS {               /* 0x28 bytes at 0x91b4c0 (ord_Statistics): the ten statistics
                                           ord_ComputeStatistics computes (out[0..9]) */
    float col[4];                       /* 0x00, out[0] the income rating, [1] the share without the first
                                           object, [2] the combined rating, [3] the share without the second
                                           object */
    int chars;                          /* 0x10, out[4], the number of characters counted (an int) */
    float rating;                       /* 0x14, out[5], the rating ai_Rating follows */
    float base;                         /* 0x18, out[6], ai_RatingBase (ord_GetStatistics) */
    int sumC, sumA;                     /* 0x1c, 0x20, out[7], out[8], the kind sums (ints; he_RunPopulation
                                           rates (sumA - chars) / sumA) */
    float score;                        /* 0x24, out[9], the overall score */
} ORDSTATS;
typedef struct ORDSTATROUND {           /* one 0x14-byte round of the statistics history ord_StatisticsHistory */
    float noFirst;                      /* 0x00, out[1] of 0x4f8ae0: the share of the kind 0/1 characters without
                                           the first object (1 - n0 / cnt) */
    float combined;                     /* 0x04, out[2]: the combined rating and grid score */
    float noSecond;                     /* 0x08, out[3]: the share without the second object (1 - n1 / cnt) */
    float score;                        /* 0x0c, out[9]: the overall score the report's 0.55 / 0.45 use */
    int base;                           /* 0x10, ai_RatingBase of the round, truncated (0x4f88c0) */
} ORDSTATROUND;
typedef struct ORDMSGREC {              /* 0x24 bytes at 0x91b480 (ord_MessageRecord): the record the messages
                                           0x4c5e80 carry (0x4f9220 reads the fields) */
    int enabled;                        /* 0x00, 0 nothing, 1 the event runs, -1 no event (0x4f82a0) */
    int count;                          /* 0x04, the messages sent so far */
    int time;                           /* 0x08, the clock's first int (ai_Time.year) */
    float base;                         /* 0x0c, the accumulator's base */
    float scale;                        /* 0x10, the accumulator's random scale */
    float value;                        /* 0x14, the accumulator */
    float limit;                        /* 0x18, the limit the accumulator is compared to */
    int phase;                          /* 0x1c, 0 idle, 1 / 2 / 3 running */
    int stage;                          /* 0x20, 0 .. 3, picked on every message */
} ORDMSGREC;

typedef struct AIDEC {                  /* 0x24 bytes, a record of the AI decision table 0x611258 (29 records, gv_GetTableRecord copies one) */
    unsigned char id;                   /* 0x00, the record's own index 0 .. 0x1c (CRIME.law of a crime against it) */
    unsigned char valueText;            /* 0x01, how gv_TableRecordText shows cur: 0 the number, 1 / 2 / 3 a name of the texts 0x13dd / 0x13e6 / 0x13ec */
    short pad02;
    int min;                            /* 0x04 */
    int max;                            /* 0x08 */
    int guilt;                          /* 0x0c, the weight of a crime against it (summed per culprit by gv_CulpritGuilt: the indulgence price, ai_EvalGuiltWeights) */
    int crimeCount;                     /* 0x10, the CRIME.count a crime against it starts with (gv_Accuse) */
    int rule;                           /* 0x14, low byte: how gv_Accuse compares cur with a deed's value (1 .. 7: broken unless ==, unless !=, ..) */
    int cur;                            /* 0x18, the current value */
    unsigned char office;               /* 0x1c, the office kind (AICHAR.officeKind) the decision belongs to (gv_FindTableRecord) */
    unsigned char officeNo;             /* 0x1d, the decision's number within that office (1, 2, ..) */
    char b1e;                           /* 0x1e, -1 or 0 .. 0x11 in the table */
    char pad1f;
    void *fn;                           /* 0x20 */
} AIDEC;

typedef struct ORDCREDIT {          /* 12 bytes, a credit offer of a bank (gm_GetCreditOffers) */
    int amount;                     /* 0x0, the sum lent; -1 = the slot is taken */
    int term;                       /* 0x4, 2 .. 7 */
    int interest;                   /* 0x8 */
} ORDCREDIT;

#pragma pack(push, 1)
typedef struct GVDECMSG {               /* 9 bytes, the data of message 0x46 (cm_SendDecisionValue): an AI decision
                                           record's new value (gv_SetTableRecord, stored by the handler 0x4f27d0) */
    int id;                             /* 0x00, the character's netId (AICHAR.netId), -1 for none */
    unsigned char rec;                  /* 0x04, the record index (0 .. 0x1c) */
    int value;                          /* 0x05, the new AIDEC.cur, clamped to min .. max */
} GVDECMSG;
#pragma pack(pop)

#endif
