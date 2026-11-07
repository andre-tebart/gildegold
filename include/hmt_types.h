/* hmt_types.h -- hmt.c's record type, readable without hmt.h (he.c reads the records of its events) */
#ifndef HMT_TYPES_H
#define HMT_TYPES_H

typedef struct G11REC {                 /* 0x30 bytes, a record of the table at 0x5ed1e8 (51: 7 talents, 21 professions, 23 events) */
    int kind;                           /* 0x00, 0 talent (id 1..4 = the skill at +0x80 + id), 1 profession (id 1..12), 2 historical event */
    unsigned short id;                  /* 0x04 */
    short pad06;
    int textSet;                        /* 0x08, 0 / 1: selects the text block 0x1e2b or 0x1e35 (+n); the bard's script
                                           gets it as its argument (he_RunBard) */
    unsigned short textId;              /* 0x0c, the first of its three texts (0x1e60 ..): the title, the offer
                                           (textId + 1, ai_BardWindow) and the poem (textId + 2) */
    char name[0x22];                    /* 0x0e, the name (e.g. "KAMPF_1", "PEST"), the mp3 file of its poem */
} G11REC;

#endif
