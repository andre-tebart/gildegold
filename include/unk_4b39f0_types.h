/* The mission record of unk_4b39f0's table ai_Missions, for the units that declare the table without the
   rest of unk_4b39f0.h: types only. unk_4b39f0.h includes it. */
#ifndef UNK_4B39F0_TYPES_H
#define UNK_4B39F0_TYPES_H

#include "ai_types.h"                /* AITIME */

typedef struct G13MIS {                 /* 0x24 bytes, ai_Missions[0x80], the running missions */
    int type;                           /* 0x00, 0: free */
    int owner;                          /* 0x04, a character id */
    AITIME time;                        /* 0x08, when it was given */
    short pad16;
    int unk18;                          /* 0x18 */
    int unk1c;                          /* 0x1c */
    unsigned char unk20;                /* 0x20 */
    char pad21[3];
} G13MIS;

#endif
