/* The route point record of rt.c (RTPOINT), for the units that pass route lists without the rest of rt.h: types
   only. rt.h includes it. */
#ifndef RT_TYPES_H
#define RT_TYPES_H

typedef struct RTPOINT {                /* 0x40 bytes ("rt_point"), a route point in a list of at most 8 */
    int cartId;                         /* 0x00 */
    int buildingId;                     /* 0x04, -1 when new */
    int loadSlots[6];                   /* 0x08, three {good, amount} pairs */
    int unloadSlots[6];                 /* 0x20, three {good, amount} pairs */
    struct RTPOINT *prev;               /* 0x38 */
    struct RTPOINT *next;               /* 0x3c */
} RTPOINT;

#endif
