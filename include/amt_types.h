/* The office record of amt's table amt_Table and the office kind record (amt_Kinds), for the units that declare the table without the rest of
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

typedef struct AMTKIND {                /* 0x18 bytes, a kind of office (the table 0x60ed58, 30 records) */
    int id;                             /* 0x00, the kind */
    int col;                            /* 0x04, the column of the rate table */
    unsigned char level;                /* 0x08 */
    char pad09[3];
    int salary;                         /* 0x0c */
    int two;                            /* 0x10, the office takes two persons */
    int pad14;
} AMTKIND;

#endif
