/* The item group record of f3gm.c's table ai_Unk61a348 (am.h's G7GRP) and the action record of am.c's
   ai_Actions (AIACT), for the units that declare or define them without the rest of am.h: types only.
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

typedef struct AIPLAN {                 /* 0x50 bytes, a plan record the callbacks 0x494020 .. 0x494410 look at; the trial
                                           (cm.c 0x4db700) hands one to its scene party as the request */
    int mode;                           /* 0x00, of the trial: what the members are asked (2 .. 5) */
    float score;                        /* 0x04, the plan's score (rated 3..18 by 0x494020, ranked by 0x4945c0) */
    int unk08;                          /* 0x08 */
    float unk0c;                        /* 0x0c */
    unsigned char b10;                  /* 0x10 */
    unsigned char crimes;               /* 0x11, of the trial: the number of its crimes */
    char pad12[2];
    union {
        struct {
            int c14;                    /* 0x14, character ids */
            int c18;                    /* 0x18 */
            int c1c;                    /* 0x1c */
            int c20;                    /* 0x20 */
            int c24;                    /* 0x24 */
            int c28;                    /* 0x28 */
        };
        int ids[6];                     /* 0x14 */
    };
    int rating2c;                       /* 0x2c, of the trial: ai_Unk4947a0 (the accuser, the accused) */
    int rating30;                       /* 0x30, of the trial: ai_Unk494830 (the accuser, the accused) */
    int unk34;                          /* 0x34 */
    int unk38;                          /* 0x38, key into the table at 0x5ecfd8 */
    int unk3c;                          /* 0x3c, tries */
    int unk40;                          /* 0x40 */
    int unk44;                          /* 0x44 */
    int unk48;                          /* 0x48, result */
    float unk4c;                        /* 0x4c, result */
} AIPLAN;
typedef char am_AssertAIPLAN[sizeof(AIPLAN) == 0x50 ? 1 : -1];
#endif
