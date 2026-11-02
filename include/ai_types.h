/* ai.h's records other units need without the rest of ai.h: the game time (unk_4b39f0_types.h's G13MIS)
   and the offer row (f3gm.c's gm_Offers): types only. ai.h includes it. */
#ifndef AI_TYPES_H
#define AI_TYPES_H
#pragma pack(push, 2)
typedef struct AITIME {                 /* 14 bytes: the game time ai_Time, the AI's last-run time ai_LastUpdateTime */
    int year;                           /* 0x0, the year (0x578 + it is the calendar year) */
    unsigned short hour;                /* 0x4, the hour */
    union {
        struct {
            short unk6;                 /* 0x6 */
            int unk8;                   /* 0x8 */
            short unkc;                 /* 0xc */
        };
        struct {
            int min;                    /* 0x6, the minute */
            int xy;                     /* 0xa */
        };
    };
} AITIME;
#pragma pack(pop)

typedef struct AISTORE {                /* 24 bytes, a stock list per building, gm_Offers[] */
    struct AIBLD *bld;                  /* 0x00 */
    int market;                         /* 0x04, the market / stall object holding the goods (f3gm.c gm_CollectOffers) */
    int goodsObj;                       /* 0x08, the goods object of the offer (the f3gm.c note of 0x7e4b20) */
    int count;                          /* 0x0c, the supply: the stall's amount less a quarter of its stock times the goods' +0x1c, not below 0 */
    float price;                        /* 0x10, the goods price times the building's price factor; gm_CollectOffers sorts by it, highest first */
    int stored;                         /* 0x14, 1 when the building's store object (0xff) holds the goods */
} AISTORE;

#endif
