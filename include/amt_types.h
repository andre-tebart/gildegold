/* The office record of amt's table amt_Table, the office kind record (amt_Kinds) and the candidate entry of the
   candidate lists (amt_GetOfficeCandidates), for the units that declare the table without the rest of
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

typedef struct AMTCAND {                /* 12 bytes: a kind of office, its rate for a character, its level */
    int kind;
    float rate;
    unsigned char level;
    char pad09[3];
} AMTCAND;

#pragma pack(push, 1)
typedef struct AMTAPP {                 /* an application: the person, the office(s) and the kind (7 bytes), the
                                           data of message 0x44 (cm_SendApplication, amt_ApplyForOffice) */
    int id;                             /* 0x00, the character's netId */
    unsigned char o1, o2;               /* 0x04, the first office and the second (0xff: none) */
    unsigned char kind;                 /* 0x06, the office kind */
} AMTAPP;
typedef struct AMTMSG {                 /* the office message of amt_SendOfficeMessage (0x4e6680): 15 bytes, the data
                                           of message 0x45 (cm_SendOfficeRecord) */
    unsigned char office;               /* 0x00, the office */
    int a;                              /* 0x01, the first character's netId or -1 */
    unsigned char d, e;                 /* 0x05, the kind and the code */
    int b;                              /* 0x07, the second character's netId or -1 */
    char pad[4];                        /* 0x0b */
} AMTMSG;
#pragma pack(pop)

#endif
