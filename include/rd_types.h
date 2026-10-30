/* The building-site record of rd's ai_Bauplaetze (0xf812e4, defined by f3init), for the units that read
   the sites without rd.h: types only. rd.h includes it. */
#ifndef RD_TYPES_H
#define RD_TYPES_H

typedef struct G15BK {                  /* 0x60 bytes, a building site (ai_Bauplaetze[ai_NBauplaetze]) */
    char name[0x40];                    /* 0x00 */
    float x0, y0, z0;                   /* 0x40, the minimum corner */
    char pad4c[4];
    float x1, y1, z1;                   /* 0x50, the maximum corner */
    char pad5c[4];
} G15BK;

#endif
