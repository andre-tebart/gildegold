/* Declarations of unk_4adf40.c (0x4adf40..). Names are ours. */
#ifndef UNK_4ADF40_H
#define UNK_4ADF40_H

#include "am.h"

typedef struct G12ROW {                 /* 0x44 bytes, hst_Lists[4] (one list of text entries) */
    int inUse;                          /* 0x00, 1 while the list holds entries (0x4b1ee0 clears it, a [_SET_ / [_USE_ command needs it) */
    struct {
        int id;
        int v;
    } e[8];                             /* 0x04 */
} G12ROW;
extern G12ROW hst_Lists[4];  /* 0xf83540 */
typedef struct G12ARG {                 /* 8 bytes, the arguments of a text */
    int unk00;
    int id;                             /* 0x04, a character id */
} G12ARG;                                                   /* 0xf83540 */

void ai_SplitTokenFormat(char *src, char *dst, int *n1, int *n2);                       /* 0x4adf40 */
int ai_NotInLists(int id);                                                        /* 0x4adfb0 */
int ai_IsLivingCharacter(unsigned short id);                                             /* 0x4adff0 */
int ai_IsLivingPlayer(unsigned short id);                                             /* 0x4ae030 */
int ai_IsLivingNonPlayer(unsigned short id);                                             /* 0x4ae080 */
int ai_IsLivingHuman(unsigned short id);                                             /* 0x4ae0d0 */
int ai_IsLivingInhabitant(unsigned short id);                                             /* 0x4ae120 */
int ai_TokenMayor(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4ae160 */
int ai_TokenBishop(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4ae280 */
int ai_TokenRandomGuildMaster(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4ae3a0 */
int ai_TokenRandomOfficeHolderFemale(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4ae600 */
int ai_TokenRandomOfficeHolder(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4ae900 */
int ai_TokenRandomOfficePerson(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4aec00 */
int ai_TokenRichestInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4aef10 */
int ai_TokenBestInn(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af090 */
int ai_TokenMoney(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af1b0 */
int ai_TokenCityTreasury(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af2c0 */
int ai_TokenRandomChurchProfession(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af3f0 */
int ai_TokenRandomRich(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af6c0 */
int ai_TokenRandomNpcInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4af8d0 */
int ai_TokenRandomMerchant(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4afa20 */
int ai_TokenRandomPlayer(int mode, G12ARG *args, int k, char *fmt, char *out);           /* 0x4afc30 */
int ai_CommandFest(int a, int b, char *fmt);                                     /* 0x4afdd0 */
int ai_Unk4afe80(void);                                                          /* 0x4afe80 */
int ai_Unk4aff30(void);                                                          /* 0x4aff30 */
int ai_CommandFire(void);                                                        /* 0x4affc0 */
int ai_CommandTornado(void);                                                     /* 0x4b0050 */
int ai_CommandCityTreasury(int a, int b, char *fmt);                             /* 0x4b00e0 */
int ai_CommandWealthTax(int a, int b, char *fmt);                                /* 0x4b02a0 */
int ai_CommandWealth(G12ARG *args, int k, char *fmt);                            /* 0x4b0360 */
int ai_CommandOfficeStanding(G12ARG *args, int k, char *fmt);                    /* 0x4b04a0 */
int ai_CommandKillPlayer(G12ARG *args, int k);                                   /* 0x4b05e0 */
int ai_CommandExecution(G12ARG *args, int k, char *fmt);                         /* 0x4b0640 */
int ai_CommandPrison(G12ARG *args, int k, char *fmt);                            /* 0x4b08e0 */
int ai_CommandOfficeInventoryPlus(int a, int b, char *fmt);                      /* 0x4b0950 */
int ai_CommandLaw(int a, int b, char *fmt);                                      /* 0x4b09f0 */
int ai_CommandLawChange(int s, int b, char *fmt);                                /* 0x4b0b50 */
int ai_CommandFaithChange(int a, int b, char *fmt);                               /* 0x4b0d40 */
int ai_CommandRiot(void);                                                        /* 0x4b0fd0 */
int ai_Unk4b1060(int a, int b, char *fmt);                                       /* 0x4b1060 */
float gm_CharacterHealth(short *c);                                              /* 0x4fc820, the health of the character c now (-1 for a free slot / an inactive one) */
float gm_AddHealth(unsigned short id, int v);                                    /* 0x4fca40, @original: f3gm.c defines `unsigned id, int d`; the byte-matched ai_CommandKillPlayer (hst.c) pushes c->id as 16 bits */
#endif
