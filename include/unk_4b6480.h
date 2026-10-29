/* Declarations of unk_4b6480.c (0x4b6480..0x4bb720): 69 message callbacks (text 0x12e7 + type), table 0x602570,
   dispatcher 0x4bb6c0. Names are ours. */
#ifndef UNK_4B6480_H
#define UNK_4B6480_H

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

int amt_KindColumn(int k);                                                       /* 0x4e6f10, the office column of the kind k */
int amt_GetColumnOffices(int col, int max, struct AMTREC *out);                  /* 0x4e6fc0, up to max offices of the column col */
float gm_AddHealth(unsigned short id, int d);                                  /* 0x4fca40, @original: f3gm.c defines `unsigned id, int d`; the byte-matched ai_Unk4b86c0 (unk_4b6480.c) pushes p->id as 16 bits */

int gm_Unk5054f0(AICHAR *c);                                                     /* 0x5054f0 */
int gm_Unk5056a0(AICHAR *c);                                                     /* 0x5056a0 */
int gm_Unk5059d0(AICHAR *c);                                                     /* 0x5059d0 */
int gm_Unk505ad0(AICHAR *c);                                                     /* 0x505ad0 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                                            /* 0x4fe2c0 */
int gm_Season(unsigned char *d);                                             /* 0x4fe330, the season of a calendar record (f3gm.c's gm_Season) */
void ai_Unk4c6020(int a, int b, int c);                                /* 0x4c6020 */
void ai_Unk4c5010(int a, int b, int c);                                         /* 0x4c5010 */
void cm_SendExSetHE(int a, void *j);                                              /* 0x4c4c60, @original: the callers push the key as an int (byte-matched ai_Unk4b8c80), against cm.h's unsigned char view */                                               /* 0x4c4c60 */
#define G14_4FEA50(a, o, b, k) ((int (*)(int, void *, int, int))gm_FindById)(a, o, b, k)  /* object out 2nd */
#define G14_5087F0(a, b) ((float (*)(int, unsigned char))gm_ItemPrice)(a, b)  /* ai.h: int n */

extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, per player (hst.c: starts with a name) */

extern unsigned int ai_Unk6f0730;       /* 0x6f0730, random seed of the messages */

typedef int (*G14CB)(G14MSG *m);
extern G14CB ai_Unk602570[0x45];        /* 0x602570 */

#endif
