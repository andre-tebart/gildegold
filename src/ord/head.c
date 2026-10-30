/* Order handlers of the characters (the table 0x611768: one handler per order, 0x4f4e20 .. 0x4f8140), the dispatcher 0x4f81c0, the privilege dialog 0x4f8200 and the grid statistics that follow (0x4f82a0 ..). Split from gv.c (layout 2026-10-22): 0x4f5d50 matches only when 0x4f2870 is not defined above it in the same file.
   unit of rea 8cef7d5b (src/game/ord.c); boundaries in decl/text.dcl, notes in src/ord/open.md */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "di_input.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define d3sm_GetVertexI ((int (*)())d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched unk_Unk4f86a0 (ord.c): no 0f be c0 after the call) */
extern char d3_Scenes[];

/* full rebuild: the symbols whose original addresses the code above spells literally */

#define ORD_FLAG_BLOCKED 0x400000  /* the privilege flag that blocks the misappropriation order (0x4f6b40) */

/* the clock's hour as am.h / cm.h / amt.h declare it: a view of gv.h's ai_Time (0x770ae0 + 4), so it is not
   an object of its own in the data area */
#define ai_TimeHour (*(unsigned short *)((char *)&ai_Time + 0x4))  /* 0x770ae4 */

/* the engine's generic unaligned block copy (D3_COPY in d3.h, which needs the DirectX headers): size and
   destination through temps, the source named directly in the asm (0x4f9190 copies the record 0x91b480) */
#define ORD_CAT(a, b) a##b
#define ORD_LABEL(name, n) ORD_CAT(name, n)
#define ORD_COPY(dst, src, size) {                                  \
    int _cn = (size); void *_cd = (dst);                            \
    __asm mov edi, _cd                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copy_tail, __LINE__)                    \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copy_tail, __LINE__): add ecx, ebx          \
    __asm rep movsb                                                 \
}
/* both operands through temps (0x4f9220 copies the local record into the message out) */
#define ORD_COPY3(dst, src, size) {                                 \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);         \
    __asm mov edi, _cd                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copy3_tail, __LINE__)                   \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copy3_tail, __LINE__): add ecx, ebx         \
    __asm rep movsb                                                 \
}
/* both operands named directly in the asm (0x4f9710 copies the message into the record 0x91b480) */
#define ORD_COPY_AT(dvar, svar, size) {                             \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copyat_tail, __LINE__)                  \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copyat_tail, __LINE__): add ecx, ebx        \
    __asm rep movsb                                                 \
}

/* the statistics record views of 0x4f9020 (unit-private: gv.h's ORDCHAR is the shared character view) */
typedef struct ORDSTATS {               /* 0x28 bytes at 0x91b4c0 (ord_StatisticsRecord) */
    int col[5];                         /* 0x00, the five statistic columns */
    float rating;                       /* 0x14, ai_Rating (0x6f1ba0) */
    float base;                         /* 0x18, ai_RatingBaseF (0x6f1b9c) */
    int v1c, v20, v24;                  /* 0x1c, 0x20, 0x24 */
} ORDSTATS;
#pragma pack(push, 1)
typedef struct ORDSTIME {               /* the 0x0e-byte game clock: ai_Time 0x770ae0 and the stamp 0x91b470 */
    int year;                           /* 0x00 */
    int hourWord;                       /* 0x04, the hour in the low word (the minute's low word at +6) */
    int minuteWord;                     /* 0x08 */
    unsigned short stamp;               /* 0x0c */
} ORDSTIME;
#pragma pack(pop)
typedef struct ORDSAVE {                /* the saved record of 0x4f9020 (15 words) */
    int col[5];                         /* 0x00 */
    float rating;                       /* 0x14 */
    float base;                         /* 0x18 */
    int v1c, v20, v24;                  /* 0x1c .. 0x24 */
    int rate;                           /* 0x28, the bits of ord_RatingRate */
    int clock[3];                       /* 0x2c, the clock stamp */
    unsigned short stamp;               /* 0x38 */
} ORDSAVE;
typedef struct ORDCHARKIND {            /* the character record seen from its kind byte */
    unsigned char kind;                 /* 0x00 (ORDCHAR + 0x02) */
    char pad01[5];
    unsigned char health;               /* 0x06 (ORDCHAR + 0x08) */
    char pad07[4];
    unsigned char level;                /* 0x0b (ORDCHAR + 0x0d) */
    char pad0c[0x72];
    unsigned char stats[5];             /* 0x7e (ORDCHAR + 0x80), the five skill bytes 0x4f9020 averages */
    char pad83[0x21c - 0x83];
} ORDCHARKIND;
#define ORD_CHARKINDS ((ORDCHARKIND *)((char *)ai_Chars + 2))
typedef struct ORDSTATROUND {           /* one 0x14-byte round of the statistics history 0x4f88c0 keeps */
    float noFirst;                      /* 0x00, out[1] of 0x4f8ae0: the share of the kind 0/1 characters without
                                           the first object (1 - n0 / cnt) */
    float combined;                     /* 0x04, out[2]: the combined rating and grid score */
    float noSecond;                     /* 0x08, out[3]: the share without the second object (1 - n1 / cnt) */
    float score;                        /* 0x0c, out[9]: the overall score the report's 0.55 / 0.45 use */
    float base;                         /* 0x10, ai_RatingBaseF of the round */
} ORDSTATROUND;
extern ORDSTATS ord_Statistics;
unsigned char ai_OwnPlayer = 0;  /* 0x6f1bc5: defined here because ord.c does not read it (the ai data area) */
ORDSTIME ord_StatisticsClock = {0}; /* 0x91b470, the same bytes as ord_StatisticsTime */
ORDSTATS ord_Statistics = {0}; /* 0x91b4c0, the same bytes as ord_StatisticsRecord */
extern ORDSTIME ord_GameClock;           /* 0x770ae0, the same bytes as ai_Time */
extern ORDSTATROUND ord_StatisticsRounds[16]; /* 0x91ab00, the same bytes as ord_StatisticsHistory */
extern int ord_RatingRateBits;           /* 0x91aaf4, the bits of ord_RatingRate */

