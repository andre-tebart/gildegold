/* The item group record of f3gm.c's table gm_BuildingItemGroups (G7GRP) and the action record of am.c's
   ai_Actions (AIACT), the Domizil table entry (G9DOMIZIL, he.c reads its duration), for the units that declare
   or define them without the rest of am.h: types only.
   am.h includes it. */
#ifndef AM_TYPES_H
#define AM_TYPES_H

typedef struct G9DOMIZIL {              /* 0x1c bytes, an entry of the Domizil table 0x62c420 (ai_FindUsableGood returns it) */
    int id;                             /* 0x00, the good, 0 ends the table */
    int minutes;                        /* 0x04, the duration of its effect in minutes (he.c 0x54fb50 adds it to the clock
                                           with gm_AddTime's minute argument), 0: none */
    char pad08[8];
    unsigned int mask;                  /* 0x10, tested against the character's goodsMask (0x1ec) */
    char pad14[8];
} G9DOMIZIL;

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
    int plea;                           /* 0x08, of the trial: 1 when the accused pleaded guilty (ai_Unk494410 skips the roll then) */
    float amount;                       /* 0x0c, the scale of the tries' sum (ai_PlanRollTries); of the trial 1000, then the accused's plea amount */
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
    int rating2c;                       /* 0x2c, of the trial: ai_WealthRating (the accuser, the accused) */
    int rating30;                       /* 0x30, of the trial: ai_WealthRatingHalf (the accuser, the accused) */
    int rating34;                       /* 0x34, reasoned guess: a third rating beside 0x2c / 0x30 (the trial clears it) */
    int instrument;                     /* 0x38, key into the table at 0x5ecfd8 (ai_PlanRollTries); of the trial the torture instrument 0 .. 6 */
    int tries;                          /* 0x3c, the number of rolls (ai_PlanLevel) */
    int answer;                         /* 0x40, of the trial the accused's answer under torture; 0 changes the roll (ai_PlanRollTries) */
    int tortured;                       /* 0x44, of the trial: 1 when the accuser chose the torture */
    int success;                        /* 0x48, 1 when a roll succeeded (ai_PlanRollTries): the accused confesses */
    float penalty;                      /* 0x4c, the negated sum of the rolls (ai_PlanRollTries); the trial takes it as the accused's health loss */
} AIPLAN;
typedef char am_AssertAIPLAN[sizeof(AIPLAN) == 0x50 ? 1 : -1];
#endif
