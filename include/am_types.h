/* The item group record of f3gm.c's table ai_Unk61a348 (am.h's G7GRP) and the action record of am.c's
   ai_Unkf83ec0 (AIACT), for the units that declare or define them without the rest of am.h: types only.
   am.h includes it. */
#ifndef AM_TYPES_H
#define AM_TYPES_H

#pragma pack(push, 1)
typedef struct G7GRP {                  /* 0x15 bytes, the item groups at 0x61a348 */
    unsigned char key;                  /* 0x00 */
    unsigned short items[10];           /* 0x01 */
} G7GRP;

typedef struct AIACTW {                 /* 8 bytes */
    unsigned char k;                    /* 0x0, a need index */
    char pad1[3];
    float w;                            /* 0x4 */
} AIACTW;

typedef struct AIACT {                  /* 0x94 bytes, an AI action (table at 0xf83ec0) */
    unsigned char id;                   /* 0x00 */
    char name[0x23];                    /* 0x01 */
    void *eval;                         /* 0x24 */
    void *decide;                       /* 0x28 */
    void *exec;                         /* 0x2c */
    AIACTW w1[4];                       /* 0x30 */
    char pad50[0x20];
    AIACTW w2[4];                       /* 0x70 */
    unsigned int flags;                 /* 0x90 */
} AIACT;
#pragma pack(pop)

#endif
