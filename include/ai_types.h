/* ai.h's game time record, for the units that need it without the rest of ai.h (unk_4b39f0_types.h's
   G13MIS): types only. ai.h does not include this header but keeps the same definition under the same
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
