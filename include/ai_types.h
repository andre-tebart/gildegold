/* ai.h's records other units need without the rest of ai.h: the game time (unk_4b39f0_types.h's G13MIS)
   and the offer row (f3gm.c's gm_Offers): types only. ai.h does not include this header but keeps the same definition under the same
   guard: any header ai.h includes (an empty one too) flips ai_Unk483a60 (0x483a60, byte) to shape in
   ai.c (its obj / ok stack slots 0x10 / 0x18 swap), a guard does not. Change both copies together. */
#ifndef AI_TIME_TYPE
#define AI_TIME_TYPE
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
#endif

#ifndef AI_STORE_TYPE
#define AI_STORE_TYPE
typedef struct AISTORE {                /* 24 bytes, a stock list per building, gm_Offers[] */
    struct AIBLD *bld;                  /* 0x00 */
    int unk04;                          /* 0x04 */
    int goodsObj;                       /* 0x08, the goods object of the offer (the f3gm.c note of 0x7e4b20) */
    int count;                          /* 0x0c */
    int unk10;                          /* 0x10 */
    int unk14;                          /* 0x14 */
} AISTORE;
#endif
