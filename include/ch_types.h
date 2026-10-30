/* The motion record of ch's table ch_Motions, for the units that declare the table without the rest of ch.h:
   types only. ch.h includes it. */
#ifndef CH_TYPES_H
#define CH_TYPES_H

typedef struct CHMOTION {           /* 0x4c bytes */
    char pad00[0x25];
    char name[0x27];                /* 0x25 */
} CHMOTION;

#endif
