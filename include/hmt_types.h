/* hmt_types.h -- hmt.c's record type, readable without hmt.h (he.c reads the records of its events) */
#ifndef HMT_TYPES_H
#define HMT_TYPES_H

typedef struct G11REC {                 /* 0x30 bytes, a record of the table at 0x5ed1e8 (51: 7 talents, 21 professions, 23 events) */
    int kind;                           /* 0x00, 0 talent (id 1..4 = the skill at +0x80 + id), 1 profession (id 1..12), 2 historical event */
    unsigned short id;                  /* 0x04 */
    short pad06;
    int textSet;                        /* 0x08, 0 / 1: selects the text block 0x1e2b or 0x1e35 (+n); the bard's script
                                           gets it as its argument (he_RunBard) */
    char pad0c[0x24];                   /* 0x0c a text id, 0x0e the name string (e.g. "KAMPF_1", "PEST") */
} G11REC;

#endif
