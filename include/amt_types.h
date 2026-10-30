/* The office record of amt's table amt_Table, for the units that declare the table without the rest of
   amt.h: types only. amt.h includes it. */
#ifndef AMT_TYPES_H
#define AMT_TYPES_H

typedef struct AMTREC {                 /* 0x18 bytes, one office of the table 0x925120 */
    unsigned char idx;                  /* 0x00 */
    char pad01[3];
    int owner;                          /* 0x04, character id or -1 */
    unsigned char kind;                 /* 0x08 */
    char pad09[3];
    int cnt;                            /* 0x0c, applicants taken (< 4) */
    unsigned char state;                /* 0x10, 3 = open for applications */
    char pad11[3];
    int x14;                            /* 0x14, -1 */
} AMTREC;

#endif
