/* The place record of ahm.c's alchemist records, for the units that read it without the rest of ahm.h: types only.
   ahm.h includes it. */
#ifndef AHM_TYPES_H
#define AHM_TYPES_H

typedef struct AHMSLOT {                /* 8 bytes, a place of the record at +0x65 of the object (6 from +0xc): a good and its value */
    int id;
    float v;
} AHMSLOT;

#endif
