/* The order statistics records of gv.h (ord.c's ord_Statistics, ord_StatisticsHistory, ord_MessageRecord), for the
   units that declare them without the rest of gv.h: types only. gv.h includes it. */
#ifndef GV_TYPES_H
#define GV_TYPES_H

/* the order statistics (ord.c 0x4f82a0 ..): the last statistics, the history of the rounds and the message
   event record */
typedef struct ORDSTATS {               /* 0x28 bytes at 0x91b4c0 (ord_Statistics): the ten statistics
                                           ord_ComputeStatistics computes (out[0..9]) */
    float col[5];                       /* 0x00, out[0] the income rating, [1] the share without the first
                                           object, [2] the combined rating, [3] the share without the second
                                           object, [4] */
    float rating;                       /* 0x14, out[5], the rating ai_Rating follows */
    float base;                         /* 0x18, out[6], ai_RatingBase (ord_GetStatistics) */
    float v1c, v20;                     /* 0x1c, 0x20, out[7], out[8] */
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

#endif
