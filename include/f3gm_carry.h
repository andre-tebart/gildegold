/* f3gm.c's carried-goods records (0x100 of 0xa4 bytes at 0x74e1e0, inside ls.c's he_QuickList) for the units that
   use them (f3gm.c, he.c): types and the view only. */
#ifndef F3GM_CARRY_H
#define F3GM_CARRY_H
#pragma pack(push, 1)
/* the 0xe-byte goods description a carried-goods record holds and gm_CollectCarried copies whole (ls.c saves it
   as one block of 0xe bytes) */
typedef struct GMCARRYSPEC {
    int v[3];
    short w;
} GMCARRYSPEC;

/* one of the 0x100 carried-goods records at 0x74e1e0 (0xa4 bytes; he.c counts +4 down, ls.c saves the fields
   +4 / +8 / +0xc / +0x10 / +0x14 (0xe bytes) / +0x22 (0x80 bytes) of each used one) */
typedef struct GMCARRYREC {
    int ref;                            /* 0x00, nonzero: the record is in use (ls.c saves the word at ref + 4) */
    unsigned char ttl;                  /* 0x04, counted down by he.c 0x53eea0, the record expires at 0 */
    char pad05[3];
    int hasValue;                       /* 0x08, nonzero: value counts */
    int owner;                          /* 0x0c, the id of the person who carries it */
    int value;                          /* 0x10 */
    GMCARRYSPEC spec;                   /* 0x14 */
    char data[0x80];                    /* 0x22 */
    char padA2[2];
} GMCARRYREC;
#pragma pack(pop)
typedef char gm_AssertGMCARRYREC[sizeof(GMCARRYREC) == 0xa4 ? 1 : -1];
#define gm_CarryRecs (*(GMCARRYREC (*)[0x100])((char *)&he_QuickList + 0x6a0))  /* 0x74e1e0, the 0x100 carried-goods records inside ls.c's he_QuickList */
#endif
