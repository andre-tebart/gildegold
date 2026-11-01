/* Declarations of unk_4a7140.c (0x4a7140.., the card poker game against two NPCs: the .data block "Naechste
   Karte", "Paar" .. "Royal Flush", the NPC models "arbeiterin_frau" .. "buerger_MANN"). Names are ours. */
#ifndef UNK_4A7140_H
#define UNK_4A7140_H

struct AMTREC;
#include "am.h"
struct AIPER;
struct AMTKIND;

#pragma pack(push, 1)
typedef struct G10NPC {                 /* 0x21c bytes, the two opponents at 0xf837e0 (character-like records) */
    char pad00[2];
    unsigned char kind;                 /* 0x02, 10 */
    char pad03[6];
    unsigned char female;               /* 0x09 */
    char pad0a[3];
    unsigned char variant;              /* 0x0d, 1..4, rolled for each opponent at the start; no reader found (reasoned guess: a variant of the opponent's play or looks) */
    char pad0e[0x22];
    char vname[16];                     /* 0x30 */
    char nname[16];                     /* 0x40 */
    char pad50[0x1cc];
} G10NPC;
#pragma pack(pop)
typedef struct G10SEAT {                /* 0x34 bytes; seat 0 is the player, 1 and 2 the NPCs */
    void *who;                          /* 0x00, AIPER (seat 0) / G10NPC */
    int model;                          /* 0x04, 0x639 + the model's index (0x4fb770) */
    int card[5];                        /* 0x08, indices into the deck, -1 = none */
    int bet;                            /* 0x1c, what the seat has put into the pot this round (+= stake2) */
    int bold;                           /* 0x20, 1 for one NPC in five (rolled per round): folds only with a clearly better open hand (reasoned name) */
    int swapped;                        /* 0x24, the card the seat gave away in the exchange, -1 = none */
    int foldedAt;                       /* 0x28, the phase the seat folded in, -1 = still playing */
    int strategy;                       /* 0x2c, the NPC's exchange strategy (0x4a77e0: 1 keep pairs, 2 the most common rank, 3 ..) */
    int unk30;                          /* 0x30 */
} G10SEAT;
typedef struct G10GAME {                /* 0xc0 bytes, the game the caller hands to 0x4a7140 (0xf83654) */
    int phase;                          /* 0x00 */
    G10SEAT seat[3];                    /* 0x04 */
    int pot;                            /* 0xa0, the money in the pot (+= stake2 per bet; paid to the winner at the round end) */
    int stake;                          /* 0xa4 */
    int money;                          /* 0xa8 */
    int stake2;                         /* 0xac */
    int turn;                           /* 0xb0, 0..2, the seat whose turn it is (0x4a86f0: that NPC sets the stake; 0 = the player's dialog); rolled per round */
    unsigned char left;                 /* 0xb4, cards left in the deck */
    unsigned char dealt;                /* 0xb5, the cards dealt this round (+3 at the deal, +1 per further card) */
    short pad;
    short winner;                       /* 0xb8, the winning seat of the round (ai_Unk4a8190 at the round end; 0 = the player), 4 = none yet (ai_PokerStart) */
    short padba;
    int winHand;                        /* 0xbc, the hand kind of the round's winner */
} G10GAME;
typedef struct G10CARD {                /* 16 bytes, the deck at 0xf83660 (24 cards) */
    int suit;                           /* 0x0 */
    int rank;                           /* 0x4, 0..3 */
    int owner;                          /* 0x8, the seat holding it, 3 = in the deck */
    int shown;                          /* 0xc */
} G10CARD;
typedef struct G10DIST {                /* 0x18 bytes, a district as 0x4e6f80 fills it */
    int unk00;                          /* 0x00 */
    int id;                             /* 0x04 */
    unsigned char lvl;                  /* 0x08 */
    char pad09[15];
} G10DIST;
typedef struct G10DREC {                /* 0x18 bytes, a district's character as 0x4e6fc0 lists it */
    int unk00;                          /* 0x00 */
    int id;                             /* 0x04 */
    int unk08;                          /* 0x08 */
    int unk0c;                          /* 0x0c */
    unsigned char k;                    /* 0x10 */
    char pad11[7];
} G10DREC;
extern G10GAME *ai_PokerTable;                                                    /* 0xf83654 */
extern G10CARD ai_PokerDeck[24];                                                 /* 0xf83660 */
extern G10NPC ai_PokerOpponents[2];                                                   /* 0xf837e0 */



int ai_PokerStart(AIPER *p, G10GAME *g);                                          /* 0x4a7140 */
int ai_PokerDealRound(G10GAME *g);                                                    /* 0x4a7430 */
unsigned short ai_PokerStep(void);                                                        /* 0x4a74e0 */
int ai_PokerFindPair(int *hand, int shown, int skip, int *out);                         /* 0x4a7650 */
int ai_Unk4a76d0(int *hand, int high);                                           /* 0x4a76d0 */
int ai_Unk4a7760(int owner);                                                     /* 0x4a7760 */
void ai_PokerNpcExchange(void);                                                         /* 0x4a77e0 */
void ai_Unk4a79d0(G10SEAT *s, int k);                                            /* 0x4a79d0 */
void ai_PokerPayStakes(void);                                                         /* 0x4a7a20 */
void ai_PokerPickStrategies(void);                                                         /* 0x4a7ac0 */
void ai_PokerNpcFold(void);                                                         /* 0x4a7c20 */
void ai_Unk4a7e70(G10SEAT *s, int *n, int *g, int *m, int *r);                 /* 0x4a7e70 */
char ai_Unk4a7f50(int owner, int *g1, int *g2, int *r1, int *r2);              /* 0x4a7f50 */
int ai_Unk4a8190(void);                                                          /* 0x4a8190 */
void ai_PokerDealThree(void);                                                         /* 0x4a8520 */
void ai_PokerDealFourth(void);                                                         /* 0x4a8590 */
void ai_PokerDealFifth(void);                                                         /* 0x4a85e0 */
void ai_PokerExchange(int k);                                                        /* 0x4a8630 */
void ai_PokerAnte(void);                                                         /* 0x4a8650 */
void ai_PokerEndRound(void);                                                         /* 0x4a8690 */
void ai_PokerNpcSetStake(void);                                                         /* 0x4a86f0 */
int ai_Unk4a8760(int a, unsigned char *p, int *ctx, int d, unsigned char *m);   /* 0x4a8760 */
int ai_Unk4a8a50(int a, AIPER *p, int *ctx, int d, unsigned char *m);           /* 0x4a8a50 */
int ai_Unk4a8ad0(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a8ad0 */
int ai_Unk4a8bf0(void);                                                          /* 0x4a8bf0 */
int ai_AlwaysTrue(void);                                                          /* 0x4a8c10 */
int ai_Unk4a8c20(int a, AIPER *p);                                               /* 0x4a8c20 */
int ai_Unk4a8c80(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a8c80 */
int ai_Unk4a8d20(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a8d20 */
int ai_Unk4a8db0(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a8db0 */
int ai_Unk4a8ee0(int a, AIPER *p);                                               /* 0x4a8ee0 */
/* am.h declares ai_Unk4a58d0 with an int b (really a float) */
#define G10_4A58D0(p, a, fl, b, f, id) ((int (*)(AIPER *, int, int *, float, float, unsigned short *))ai_Unk4a58d0)(p, a, fl, b, f, id)
int ai_Unk4a8f20(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a8f20 */
int ai_Unk4a9030(int a, AIPER *p, int c, unsigned char *m);                     /* 0x4a9030 */
int ai_Unk4a9080(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a9080 */
int ai_Unk4a9150(int a, AIPER *p);                                               /* 0x4a9150 */
int ai_Unk4a91a0(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a91a0 */
int ai_Unk4a9280(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a9280 */
int ai_Unk4a9350(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a9350 */
int ai_Unk4a9420(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a9420 */
int ai_Unk4a94e0(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a94e0 */
int ai_Unk4a95d0(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a95d0 */
int ai_Unk4a9660(int a, AIPER *p, int c, int d, unsigned char *m);              /* 0x4a9660 */
int ai_Unk4a9700(int a, AIPER *p, int *ctx, int d, unsigned char *m);           /* 0x4a9700 */
int ai_Unk4a9880(int a, AIPER *p, int *ctx, int d, unsigned char *m);           /* 0x4a9880 */
extern int ai_Unk600e10[19];                                                     /* 0x600e10, the leisure actions */
int ai_Unk4a9a00(int a, struct AIPER *p, int c, unsigned char *in, unsigned char *m);  /* 0x4a9a00 */
int ai_Unk4a9fa0(struct AIPER *p, unsigned char *in, unsigned char *out);              /* 0x4a9fa0 */
extern int ai_Unk600e5c[3];                                                      /* 0x600e5c */
int ai_Unk4aa120(int a, struct AIPER *p, int c, unsigned char *in, unsigned char *m);  /* 0x4aa120 */
int ai_Unk4aa430(struct AIPER *p, unsigned char *in, unsigned char *out);              /* 0x4aa430 */
int ai_Unk4aa580(struct AIPER *p, int c, unsigned char *m);                             /* 0x4aa580 */
int ai_Unk4aa6a0(struct AIPER *p, unsigned char *in, unsigned char *m);                 /* 0x4aa6a0 */
int ai_Unk4aa800(struct AIPER *p, int c, unsigned char *m);                             /* 0x4aa800 */
int ai_Unk4aa940(struct AIPER *p, int c, unsigned char *m);                             /* 0x4aa940 */
int ai_Unk4aaa10(struct AIPER *p, int c, unsigned char *m);                             /* 0x4aaa10 */
int ai_Unk4aaae0(struct AIPER *p, int c, unsigned char *m);                             /* 0x4aaae0 */
int ai_TrustedOfTwo(AIPER *p, AIPER *b, AIPER *c, int *k);                          /* 0x4aab90 */
int ai_TrustedOfFour(int k, AIPER *p, int *ids, int *out);                           /* 0x4aad30 */
int ai_Unk4aaef0(struct AIPER *p, unsigned char *a, unsigned char *b);                  /* 0x4aaef0 */
int ai_Unk4aafb0(struct AIPER *p, struct AIPER *o, unsigned char *m);                          /* 0x4aafb0 */
int ai_Unk4ab080(struct AIPER *p, int *a, int *b);                                      /* 0x4ab080 */
int ai_Unk4ab2c0(struct AIPER *p, int *a, int *b);                                      /* 0x4ab2c0 */
int ai_Unk4ab3e0(int a, AIPER *p, int c, unsigned char *m, unsigned char *m2);  /* 0x4ab3e0 */
int ai_Unk4ab6b0(AIPER *p, unsigned char *m, unsigned char *m2);                 /* 0x4ab6b0 */
int ai_Unk4ab7f0(int a, AIPER *p, int c, unsigned char *m);                     /* 0x4ab7f0 */
int ai_Unk4ab9f0(AIPER *p, unsigned char *m);                                    /* 0x4ab9f0 */
int ai_Unk4abac0(int a, AIPER *p, int c, unsigned char *m);                     /* 0x4abac0 */
int ai_Unk4abc60(AIPER *p, unsigned char *m);                                    /* 0x4abc60 */
int ai_Unk4abd00(int a, AIPER *p, int c, unsigned char *m);                     /* 0x4abd00 */
int ai_Unk4abe60(AIPER *p, unsigned char *m);                                    /* 0x4abe60 */
int ai_Unk4abed0(int a);                                                         /* 0x4abed0 */
#endif
