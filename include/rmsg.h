/* Declarations of rmsg.c (0x4b6480..0x4bb720): 69 message callbacks (text 0x12e7 + type), table 0x602570,
   dispatcher 0x4bb6c0. Names are ours. */
#ifndef RMSG_H
#define RMSG_H

#include "am.h"

typedef struct G14MSG {                 /* the argument of the callbacks */
    int id;                             /* 0x00, a character id */
    unsigned short type;                /* 0x04, 0..0x44, the table index; text 0x12e7 + type */
} G14MSG;

struct AMTREC;                          /* amt.c's office record (amt.h, which does not compile with am.h); the
                                           callbacks read this 0x18-byte view of it */
typedef struct G14REC {
    char pad00[8];
    unsigned char kind;                 /* 0x08, the office kind (AMTREC.kind) */
    char pad09[3];
    int cnt;                            /* 0x0c, applicants taken (AMTREC.cnt) */
    unsigned char state;                /* 0x10, 3 = open for applications (AMTREC.state) */
    char pad11[3];
    unsigned char x14;                  /* 0x14, text 0x253 + x14 (the low byte of AMTREC.x14) */
    char pad15[3];
} G14REC;


void gm_ClockToDate(struct AITIME *c, unsigned char *out);                                            /* 0x4fe2c0 */
#define G14_4FEA50(a, o, b, k) ((int (*)(int, void *, int, int))gm_FindById)(a, o, b, k)  /* object out 2nd */
#define G14_5087F0(a, b) ((float (*)(int, unsigned char))gm_ItemPrice)(a, b)  /* ai.h: int n */


extern unsigned int ai_RandomSeed;       /* 0x6f0730, random seed of the messages */

typedef int (*G14CB)(G14MSG *m);
extern G14CB ai_MsgCallbacks[0x45];        /* 0x602570 */
int ai_PostRandomMessages(int n, int id);   /* 0x4bb720, posts 0..n random messages (AIMSG 0x42) for the character id */

#endif
