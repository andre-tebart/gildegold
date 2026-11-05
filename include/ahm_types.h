/* The alchemist record of ahm.c and its places, for the units that read it without the rest of ahm.h: types only.
   ahm.h includes it. */
#ifndef AHM_TYPES_H
#define AHM_TYPES_H

typedef struct AHMSLOT {                /* 8 bytes, a place of the record at +0x65 of the object (6 from +0xc): a good and its value */
    int id;
    float v;
} AHMSLOT;

typedef struct AHMREC {                 /* 0x3c bytes, the record the alchemist object keeps at +0x65 ("ahm_AllocAhmInfo") */
    int id;                             /* +0x0, the alchemist character id (0x4eaa50) */
    float value;                        /* +0x4, 0 .. 1, added to by an order (0x4ecdb0) */
    unsigned char share[4];             /* +0x8, 0 .. 252, the four shares / 252 (0x4eab20) */
    AHMSLOT slots[6];                   /* +0xc, the places */
} AHMREC;

#endif
