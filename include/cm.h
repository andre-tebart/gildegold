struct D3PARTSYS;
struct D3COLOR;
struct D3TERRAIN;
struct D3SM;
struct AMTAPP;
struct AMTMSG;
struct CHACTION;
struct CHARACTER;
struct D3HMAP;
struct D3IOOBJ;
struct D3TRAIN;
struct G15PLANT;
struct SBBANK;
struct SBSAMPLE;
struct GCLOCK;
struct D2FADE;
struct AMTREC;
struct D2SURF;
struct AMTKIND;
struct D3SKY;
struct D3SKYLAYER;
struct D3VECTOR;
struct D3OBJECT;
struct D3QNODE;
struct D3SURF;
/* Declarations of cm.c (0x4c3180..): the game messages sent to / received from the server (net.c): message
   sizes by type, the send ring, the receive list, split long messages (type 7), field blocks. Its .data block starts at
   0x60bd80 (0x302, 2, -1 globals, the handler table 0x60bd98). Names are ours. */
#ifndef CM_H
#define CM_H
struct AICHAR;
struct D3FLMAP;
typedef struct G18POP { float t; int v; int h; char *c; int kind; } G18POP;      /* 0x14 bytes, a floating number */
typedef struct G18LOG { char text[0x40]; char name[0x24]; int x; int id; } G18LOG;      /* 0x6c bytes, a chat line */

#pragma pack(push, 1)
#include "cm_types.h"                /* G17MSG, G17STAT */
typedef struct G17FIELDS {              /* 0x7c bytes, a field block (0xa68c20, 0xa68ca0) */
    int id;                             /* 0x00 */
    unsigned char n;                    /* 0x04, fields */
    unsigned char buf[0x77];            /* 0x05, size, count, offset (short), values */
} G17FIELDS;
#pragma pack(pop)

typedef char ai_AssertG17MSG[sizeof(G17MSG) == 0x99 ? 1 : -1];

extern G17MSG cm_RecvPool[0x2000];     /* 0x9367e0, the free receive records */
extern int cm_NextNetStep;                /* 0xa687e4, d3_Time of the start */
extern unsigned char ai_Unka68800[];    /* 0xa68800, a long message to split (ai_Unka68d30 bytes) */
extern G17MSG *cm_RecvFree;            /* 0xa68c00, the free list */
extern unsigned int cm_GroupEndSeq;       /* 0xa68c04 */
extern int ai_Unka68c08;                /* 0xa68c08, bytes used in ai_Unka68c20.buf */
extern G17FIELDS ai_Unka68c20;          /* 0xa68c20 */
extern G17FIELDS ai_Unka68ca0;          /* 0xa68ca0 */
extern int ai_Unka68d20;                /* 0xa68d20, bytes used in ai_Unka68ca0.buf */
extern G17MSG *cm_RecvQueue;            /* 0xa68d24, the received messages */
extern char *cm_FieldObject;              /* 0xa68d2c, the object of the field block (fields are offsets into it) */
extern int ai_Unka68d30;                /* 0xa68d30, length of ai_Unka68800 */
extern char *ai_Unka68d34;              /* 0xa68d34 */
extern unsigned char ai_Unka68d40[];    /* 0xa68d40, a joined long message */
extern G17MSG cm_SendRing[0x8000];     /* 0xa69140, the send ring */
extern G17STAT cm_SendStates[0x8000];    /* 0xf31160 */
extern G17MSG *cm_SendQueue;            /* 0xf81160, the messages to send */
extern unsigned int ai_Unkf81164;       /* 0xf81164, length of ai_Unka68d40 */
extern int cm_LastSeq;       /* 0xf81168, the last sequence number */
extern G17MSG ai_Unkf81180;             /* 0xf81180, the message being received */
extern unsigned int cm_GroupFirstSeq;       /* 0xf8121c */
extern unsigned int cm_SpeedLevel;               /* 0x60bd84, = 2 (0..4) */
extern int cm_LastCharId;                /* 0x60bd88, = -1 */
extern int cm_LastBuildingId;                /* 0x60bd8c, = -1 */
extern int cm_LastObjectId;                /* 0x60bd90, = -1 */
extern int ai_Unk60bd94;                /* 0x60bd94, = -1 */
extern int (*ai_Unk60bd98[0x6a])(G17MSG *m, G17STAT *s);  /* 0x60bd98, the handler of each message type */
extern int (*ai_Unk60bf40[0x6a])(G17MSG *m);             /* 0x60bf40, the check of each type (groups 5..6) */
extern unsigned int ai_Unka68d1c;       /* 0xa68d1c, "cm_LastRequCount" */
extern int cm_LastSyncCount;               /* 0xa687e0, "cm_LastSyncCount" (net.h) */
extern int ai_GameOver;                /* 0x6f259c */
extern float ai_DifficultyValue;              /* 0x5fef04 */
extern float ai_DifficultyScale;              /* 0x5fef08 */
extern int ai_Unk6f0cd0;                /* 0x6f0cd0, received messages */
extern unsigned short ai_PlayerId;     /* 0x6f25b8, the player */
extern volatile unsigned long d3_Time;  /* 0x6333cc */
extern int ai_NextCrimeId;                /* 0x6f1b88 */
extern char ai_Unk625820[][0x20];       /* 0x625820, names ("HE_NULL", ..) */
extern char ai_Time[];                /* 0x770ae0, AITIME (ai.h) */

#pragma pack(push, 1)
typedef struct G17B7 { char b[7]; } G17B7;      /* blocks copied as one */
typedef struct G17B9 { char b[9]; } G17B9;
typedef struct G17B10 { char b[10]; } G17B10;
typedef struct G17B12 { char b[12]; } G17B12;
typedef struct G17B15 { char b[15]; } G17B15;
typedef struct G17T14 {                 /* 14 bytes copied as one (an AITIME) */
    int a, b, c;
    short d;
} G17T14;
#pragma pack(pop)

int gm_FindById(int *bld, int *obj, int *chr, int id);                             /* 0x4fea50, out: three object views */
void main_GameLoop(int ctl);                                                              /* 0x53bb20 */
short * gm_FindChar(int id);                                                      /* 0x505df0, ai.h: AICHAR * */
int gm_CompareTime(char *t, char *now);                                            /* 0x4fe210, AITIME * */
char *gm_FindBuildingById(int id);                                                      /* 0x502020, ai.h: AIBLD * */
int gm_IsHomeType(unsigned char *bld);                                                     /* 0x502520 */
void gm_GetGebaeudeHauptraum(unsigned char *bld);                                                   /* 0x501ce0, ai.h: AIOBJ * */
char * gm_Query(void *list, int n, ...);                                        /* 0x4ffe90, ai.h: AIOBJ * */
short *unk_Unk4fda70(char *o, unsigned a, unsigned b);                             /* 0x4fda70 */
char *unk_Unk4fd820(int unused, char *o);                                   /* 0x4fd820 */
char * gm_FindGoods(int p, unsigned id);                                            /* 0x4ffc40, ai.h: int * */
extern int *ai_Unk6f0d90;               /* 0x6f0d90 */
extern char ai_Chars[][0x21c];      /* 0x770b80, the characters (ai.h AICHAR) */
int gm_SumSlotAmounts(void *list, unsigned char i);                                     /* 0x508660 */
int gm_FreeCapacity(short *l, int item, int max);                                    /* 0x50ab80 */
unsigned short * gm_ObjectBuildingOwner(unsigned short *p);                                                     /* 0x50a050 */
int gm_Unk50ad70(char *p, int item, int max);                                    /* 0x50ad70 */
unsigned char * gm_BuildQuery(int n, ...);                                                  /* 0x501190, ai.h: AIBLD * */
int amt_CheckApplication(struct AMTAPP *m);                                                       /* 0x4e6360 */
int amt_CheckOfficeMessage(struct AMTMSG *m);                                                       /* 0x4e6710 */
extern signed char ai_Relations[0x300][0x300];  /* 0x7e5360, relations between the characters */
extern signed char ai_RelationMemory[0x300][0x300];  /* 0x889f80 */
extern int ai_NextEventId;                /* 0x6f2618 */
int he_IsMessageKindFiltered(int id);                                                        /* 0x575640 */
unsigned char * he_Unk53e040(unsigned char *m);                                                   /* 0x53e040, AIMSG * */
char * he_FindEvent(int n, ...);                                                  /* 0x53e400, ai.h: AIJOB * */
extern int ai_NextObjectId;                /* 0x61a654 */
unsigned char * gm_CreateBuilding(unsigned char kind, unsigned short owner);                    /* 0x501580 */
unsigned short gm_Unk507690(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8); /* 0x507690 */
short * gm_PlayerRecord(char *ch);                                                    /* 0x506430, ai.h: AIFIN *(AICHAR *) */
unsigned gm_Unk5064d0(int a, int b);                                                  /* 0x5064d0 */
int unk_Unk4f2540(void *a, unsigned char **o, int *e);                                    /* 0x4f2540 */
int gv_EntryIndex(char *r, int *e);                                       /* 0x4ef6b0 */
int unk_Unk4f01d0(int *g, char *o, int v, int b);                     /* 0x4f01d0 */
int gm_FreeBuilding(char *b);                                                       /* 0x5012e0 */
void unk_Unk503c60(int c, int mode);                                     /* 0x503c60 */
int gm_TakeObjekt(int id, int good, int n);                                       /* 0x500a40 */
int gm_AddObjekt(int id, int proto, int n);                                       /* 0x500970, hmt.h: void * n */
int gm_AddNegative(int id, int good, int n);                                       /* 0x500ae0 */
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, the players (a good id at +0x54) */
extern char *ai_Goods;                  /* 0x91aaf0, ai.h: AIGOOD * (0x41 bytes each, the kind at 0) */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
int gm_ListAmount(short *l, char *o);                                              /* 0x50aaf0 */
int gm_ListStock(short *l, int item);                                             /* 0x50ab50 */
int gm_RemoveObject(int *list, unsigned item);                                          /* 0x4ffff0 */
int gm_RemoveObjectFromBuilding(int b, int id);                                            /* 0x503480 */
unsigned short * unk_Unk5031b0(unsigned char *b, unsigned n);                                           /* 0x5031b0 */
/* 0x500190 */
void txt_Format(char *buf, int id, ...);                                       /* 0x5176d0 */
struct SBSAMPLE *sb_PlaySample(struct SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay); /* 0x426fa0, sblib.c (this header's units: sb_PlaySampleI) */
float gm_ItemPrice(int item, unsigned char n);                                   /* 0x5087f0 */
int gm_Unk50a0f0(int y);                                                     /* 0x50a0f0 */
void util_Seed(unsigned int seed);                                               /* 0x42ec90, tr.c */
extern int cm_NetFlags;                /* 0x6f2594, flags */
extern unsigned cm_LoadSize;           /* 0xa68d28, size of the load buffer */
extern char *cm_LoadBuf;              /* 0xf31140, the load buffer ("cm_LoadBuf") */
extern unsigned cm_LoadReceived;           /* 0xf81220, bytes in it */
extern unsigned short *ai_PlayerChar;    /* 0x6f1bf8, the player's character (ai.c; this header's units add byte offsets through ai_PlayerCharC) */

int ai_Unk4c3180(G17MSG *m);                                                     /* 0x4c3180 */
void ai_InitStates(void);                                                         /* 0x4c3480 */
void ai_ResetMessages(void);                                                         /* 0x4c3560 */
void ai_Unk4c3600(void);                                                         /* 0x4c3600 */
int ai_Unk4c3630(void);                                                          /* 0x4c3630 */
int cm_FlushSendQueue(void);                                                          /* 0x4c3680 */
int ai_Unk4c3720(void);                                                          /* 0x4c3720 */
int ai_Unk4c3850(G17MSG *m);                                                     /* 0x4c3850 */
unsigned int ai_Unk4c38e0(G17MSG *m);                                            /* 0x4c38e0 */
int ai_Unk4c39c0(G17MSG *src);                                                   /* 0x4c39c0 */
int cm_GetSlotState(int h);                                                         /* 0x4c3ae0 */
int cm_GetSlotValue(int h);                                                         /* 0x4c3b10 */
void cm_BeginSendGroup(void);                                                         /* 0x4c3b30 */
void cm_EndSendGroup(void);                                                         /* 0x4c3b40 */
int cm_SendGroupState(void);                                                          /* 0x4c3b50 */
void cm_BeginFieldBlock(char *o, int id);                                              /* 0x4c3ba0 */
int cm_AddFieldDelta(int size, int n, short field, void *v); /* 0x4c3c00 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v); /* 0x4c3d00 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v); /* 0x4c3d90 */
void ai_Unk4c3e70(char *p);                                                      /* 0x4c3e70 */
int ai_Unk4c3eb0(unsigned char k, float v);                                        /* 0x4c3eb0 */
void cm_SetSpeedLevel(unsigned int v);                                               /* 0x4c3ef0 */
void ai_Unk4c3f20(void);                                                         /* 0x4c3f20 */
void ai_Unk4c3f50(void);                                                         /* 0x4c3f50 */
int ai_Unk4c3f80(void);                                                          /* 0x4c3f80 */
int cm_SendPlayerId(void);                                                         /* 0x4c3f90 */
int cm_ReceiveMessages(void);                                                          /* 0x4c3fd0 */
G17MSG *ai_Unk4c4030(char type, int timeout);                                    /* 0x4c4030 */
int ai_Unk4c4080(G17MSG *src);                                                   /* 0x4c4080 */
void ai_Unk4c4130(G17MSG *m);                                                    /* 0x4c4130 */
int cm_ExecCommands(void);                                                          /* 0x4c4190, "cm_ExecCommand" */
int ai_Unk4c43c0(G17MSG *grp);                                                   /* 0x4c43c0 */
int ai_Unk4c4450(int a, int id);                                                 /* 0x4c4450 */
int ai_Unk4c4480(unsigned short len, void *p);                                   /* 0x4c4480 */
void cm_SendLoadSize(int v);                                                        /* 0x4c44d0 */
int cm_SendLoadBlock(void *p);                                                      /* 0x4c4500 */
void cm_BeginGroup(const char *s);                                                /* 0x4c4540 */
void cm_EndGroup(const char *s);                                                /* 0x4c4580 */
int cm_SendMessage10(unsigned char n, int b, void *p, short d);                      /* 0x4c45c0 */
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h); /* 0x4c4650 */
int cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2); /* 0x4c46d0 */
void ai_Unk4c4770(int a, int b);                                                 /* 0x4c4770 */
void ai_Unk4c47b0(short a);                                                      /* 0x4c47b0 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                         /* 0x4c47e0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);                         /* 0x4c4830 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);         /* 0x4c4880 */
void ai_Unk4c48e0(int a, short b, int c);                                        /* 0x4c48e0 */
void ai_Unk4c4920(int a, int b, int c);                                          /* 0x4c4920 */
void cm_SendRemoveGoods(int a, short b);                                               /* 0x4c4960 */
void cm_SendFieldDeltas(void);                                                         /* 0x4c49a0 */
void cm_SendFieldValues(void);                                                         /* 0x4c49e0 */
void ai_Unk4c4a20(void);                                                         /* 0x4c4a20 */
void cm_SendSetField(int a, int b, int c, int d, int e);                            /* 0x4c4a60 */
void ai_Unk4c4ac0(int a, int b, int c);                                          /* 0x4c4ac0 */
void cm_SendRelations(int a, int b, int c, int d);                                   /* 0x4c4b00 */
int cm_SendLongMessage(char *msg);                                                    /* 0x4c4b70, AIMSG (am.h, 0xf8 bytes) */
int ai_Unk4c4bc0(char *msg, unsigned int len, void *extra);                      /* 0x4c4bc0 */
void cm_SendExSetHE(unsigned char k, char *p);                                     /* 0x4c4c60 */
int cm_SendTime(char *t);                                                       /* 0x4c4d40 */
int cm_SendExSysMessage(unsigned char id, void *msg);                               /* 0x4c4db0 */
void cm_SendKillCharacter(int a, int b);                                                 /* 0x4c4e00 */
void ai_Unk4c4e40(void *p);                                                      /* 0x4c4e40 */
void ai_Unk4c4e90(int a, int b);                                                 /* 0x4c4e90 */
void cm_SendProofExecuted(int a, int b);                                                 /* 0x4c4ed0 */
void ai_Unk4c4f10(int a, int b, int c);                                          /* 0x4c4f10 */
void ai_Unk4c4f50(int id, int b);                                                /* 0x4c4f50, "cm_RequestSendCutInfo" */
int ai_Unk4c4fa0(void *p);                                                      /* 0x4c4fa0 */
void ai_Unk4c4fd0(int a, int b);                                                 /* 0x4c4fd0 */
void ai_Unk4c5010(int a, int b, int c);                                          /* 0x4c5010 */
void cm_SetPlant(int a, unsigned char b, unsigned char c, short d, unsigned char e, int f); /* 0x4c5050 */
void ai_Unk4c50b0(int a, int b, short c);                                        /* 0x4c50b0 */
void cm_SendChrWalkTo(int a, int b, int c, int d);                                   /* 0x4c5100 */
void ai_Unk4c5150(int a, int b, int c, const char *s);                           /* 0x4c5150 */
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);                           /* 0x4c51b0, "cm_RequestChrMoveToUniverse" */
void cm_SendChrStop(int a);                                                        /* 0x4c5230 */
void ai_Unk4c5260(int a, int b);                                                 /* 0x4c5260 */
void ai_Unk4c52a0(int a, int b, int c);                                          /* 0x4c52a0 */
int cm_SendChrGotoBuilding(int id, int bld, int c, const char *s, int f, const char *s2);  /* 0x4c52e0 */
void ai_Unk4c5410(int a, const char *s, unsigned char c);                        /* 0x4c5410 */
void cm_SendChrUseGates(int a, int b, int c);                                          /* 0x4c5460 */
void cm_SendOwnership(int a, int b, int c);                                          /* 0x4c54a0 */
void ai_Unk4c54e0(int a);                                                        /* 0x4c54e0 */
void ai_Unk4c5510(int a);                                                        /* 0x4c5510 */
void cm_SendBoss(int a, int b);                                                 /* 0x4c5540 */
void ai_Unk4c5580(int a, int b, int c, int d);                                   /* 0x4c5580 */
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);                        /* 0x4c55d0 */
void ai_Unk4c5690(int a, const char *s);                                         /* 0x4c5690 */
void ai_Unk4c56d0(void *p, unsigned char k);                                     /* 0x4c56d0 */
void cm_SendGoodsDelta(char *p, unsigned char k);                                     /* 0x4c5710 */
void cm_SendMessage41(void *p);                                                      /* 0x4c5800 */
void ai_Unk4c5840(int a);                                                        /* 0x4c5840 */
void ai_Unk4c5870(int a);                                                        /* 0x4c5870 */
int cm_SendApplication(char *p);                                                       /* 0x4c58a0 */
int cm_SendOfficeRecord(char *p);                                                      /* 0x4c58e0 */
void ai_Unk4c5930(char *p);                                                      /* 0x4c5930 */
void ai_Unk4c5970(int a, int b, int c);                                          /* 0x4c5970 */
void ai_Unk4c59b0(int a, unsigned char b);                                       /* 0x4c59b0 */
void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s); /* 0x4c59f0 */
void ai_Unk4c5a70(int a);                                                        /* 0x4c5a70 */
void cm_SendChat(void *p);                                                      /* 0x4c5aa0 */
int cm_RequestCreateGeb(int a, int k, float *pos, int *v);                              /* 0x4c5ae0, "cm_RequestCreateGeb" */
void ai_Unk4c5b90(int a);                                                        /* 0x4c5b90 */
void ai_Unk4c5bc0(const char *s, int a, int b, const char *s2);                  /* 0x4c5bc0 */
void ai_Unk4c5c40(int a, void *p);                                               /* 0x4c5c40 */
void ai_Unk4c5ca0(int a, void *p);                                               /* 0x4c5ca0 */
void ai_Unk4c5d00(int a, int b, int c);                                          /* 0x4c5d00 */
void cm_SendScriptMessage(void *p);                                                      /* 0x4c5d40 */
void ai_Unk4c5d80(int *p);                                                       /* 0x4c5d80 */
void ai_Unk4c5dc0(char *o, int k, void *p);                                      /* 0x4c5dc0 */
void cm_SendMessage56(void *p);                                                      /* 0x4c5e80 */
void ai_Unk4c5ec0(int a);                                                        /* 0x4c5ec0 */
void ai_Unk4c5ef0(int a);                                                        /* 0x4c5ef0 */
void ai_Unk4c5f20(void *p);                                                      /* 0x4c5f20 */
void cm_SendMoneyDelta(int a, int b);                                                 /* 0x4c5f60 */
void ai_Unk4c5fa0(int a, int b, int c);                                          /* 0x4c5fa0 */
void cm_SendTradeMessage(char *p);                                                      /* 0x4c5fe0 */
void ai_Unk4c6020(int a, int b, int c);                                          /* 0x4c6020 */
void ai_Unk4c6060(char *o, void *p);                                             /* 0x4c6060 */
void cm_SendGraveOrder(void *p);                                                      /* 0x4c60b0 */
void ai_Unk4c60f0(void *p);                                                      /* 0x4c60f0 */
void ai_Unk4c6130(void *p);                                                      /* 0x4c6130 */
void cm_SendCreateAlchemist(void *p);                                                      /* 0x4c6170 */
void cm_SendCommand(void *p);                                                      /* 0x4c61b0 */
void ai_Unk4c61f0(int *p, char *q);                                              /* 0x4c61f0 */
void ai_Unk4c6230(int *p);                                                       /* 0x4c6230 */
int cm_SendShots(void *p);                                                      /* 0x4c6260 */
void ai_Unk4c62a0(char *o, char *pts);                                           /* 0x4c62a0, rt.c's route points */
void ai_Unk4c6320(char *o);                                                      /* 0x4c6320 */
int ai_Unk4c6350(G17MSG *m);                                                     /* 0x4c6350, check of 0xf */
int ai_Unk4c63e0(G17MSG *m);                                                     /* 0x4c63e0, check of 0x11 */
int ai_Unk4c65b0(G17MSG *m);                                                     /* 0x4c65b0, check of 0x16 */
int ai_Unk4c66c0(G17MSG *m);                                                     /* 0x4c66c0, check of 0x19 */
int ai_Unk4c6750(G17MSG *m);                                                     /* 0x4c6750, check of 0x38 */
int ai_Unk4c6780(G17MSG *m);                                                     /* 0x4c6780, check of 0x44 */
int ai_Unk4c67a0(G17MSG *m);                                                     /* 0x4c67a0, check of 0x45 */
int ai_Unk4c67c0(G17MSG *m);                                                     /* 0x4c67c0, check of 0x53 */
int ai_Unk4c6870(G17MSG *m);                                                     /* 0x4c6870, check of 0x54 */
int ai_Unk4c69c0(G17MSG *m);                                                     /* 0x4c69c0, check of 0x59 */
int ai_Unk4c6a20(G17MSG *m, G17STAT *s);                                         /* 0x4c6a20, handler of 0, 3, 5..7 */
int ai_Unk4c6a50(G17MSG *m, G17STAT *s);                                         /* 0x4c6a50, handler of 1 */
int ai_Unk4c6a60(G17MSG *m, G17STAT *s);                                         /* 0x4c6a60, handler of 4 */
int ai_Unk4c6a90(G17MSG *m, G17STAT *s);                                         /* 0x4c6a90, handler of 8: starts a load buffer */
int ai_Unk4c6ad0(G17MSG *m, G17STAT *s);                                         /* 0x4c6ad0, handler of 9: appends to it */
int ai_Unk4c6b20(G17MSG *m, G17STAT *s);                                         /* 0x4c6b20, handler of 10 */
int ai_Unk4c6bf0(G17MSG *m, G17STAT *s);                                         /* 0x4c6bf0, handler of 0xb */
int ai_Unk4c6cd0(G17MSG *m, G17STAT *s);                                         /* 0x4c6cd0, handler of 0xc */
int ai_Unk4c6e10(G17MSG *m, G17STAT *s);                                         /* 0x4c6e10, handler of 0xd */
int ai_Unk4c6ec0(G17MSG *m, G17STAT *s);                                         /* 0x4c6ec0, handler of 0xe */
int ai_Unk4c6f60(G17MSG *m, G17STAT *s);                                         /* 0x4c6f60, handler of 0xf */
int ai_Unk4c7010(G17MSG *m, G17STAT *s);                                         /* 0x4c7010, handler of 0x10 */
int ai_Unk4c70b0(G17MSG *m, G17STAT *s);                                         /* 0x4c70b0, handler of 0x11, "cm_ExSellObjekt" */
int ai_Unk4c78c0(G17MSG *m, G17STAT *s);                                         /* 0x4c78c0, handler of 0x12 */
int ai_Unk4c7c30(G17MSG *m, G17STAT *s);                                         /* 0x4c7c30, handler of 0x13 */
int ai_Unk4c7d80(G17MSG *m, G17STAT *s);                                         /* 0x4c7d80, handler of 0x14 */
int ai_Unk4c7e20(G17MSG *m, G17STAT *s);                                         /* 0x4c7e20, handler of 0x15 */
int ai_Unk4c7e90(G17MSG *m, G17STAT *s);                                         /* 0x4c7e90, handler of 0x16 */
int ai_Unk4c7f70(G17MSG *m, G17STAT *s);                                         /* 0x4c7f70, handler of 0x17 */
int ai_Unk4c8050(G17MSG *m, G17STAT *s);                                         /* 0x4c8050, handler of 0x18 */
int ai_Unk4c8110(G17MSG *m, G17STAT *s);                                         /* 0x4c8110, handler of 0x19 */
int ai_Unk4c81d0(G17MSG *m, G17STAT *s);                                         /* 0x4c81d0, handler of 0x1a */
int ai_Unk4c8240(G17MSG *m, G17STAT *s);                                         /* 0x4c8240, handler of 0x1b */
int ai_Unk4c85c0(G17MSG *m, G17STAT *s);                                         /* 0x4c85c0, handler of 0x1c */
int ai_Unk4c87a0(G17MSG *m, G17STAT *s);                                         /* 0x4c87a0, handler of 0x1d, "cm_ExSetHE" */
struct RTPOINT;
int rt_PointCount(struct RTPOINT *p);                                             /* 0x4bdaa0, rt.h */


extern int ai_Unk1460ae0;  /* 0x1460ae0 */
extern int ai_NetPlayer;  /* 0x1460af4 */
void ai_UpdateNeeds(void);  /* 0x47da80 */
void ai_UpdateCharacters(void);  /* 0x47e560 */
void ai_Unk47e990(struct AICHAR *c);  /* 0x47e990 */
void ai_Unk4b3bd0(int owner, int type);  /* 0x4b3bd0 */
int ai_Unk4b44e0();  /* 0x4b44e0 */
int cm_PartyById();  /* 0x4e50a0 */
extern G28REC ai_Unk925520[96];  /* 0x925520, the scene party records (cm.c) */
int ai_Unk4e5150();  /* 0x4e5150 */
int ai_Unk4e51b0();  /* 0x4e51b0 */
int ai_Unk4e5210();  /* 0x4e5210 */
int unk_Unk4f3640();  /* 0x4f3640 */
int unk_Unk4f36b0();  /* 0x4f36b0 */
int unk_Unk4f3780();  /* 0x4f3780 */
int unk_Unk4f37e0();  /* 0x4f37e0 */
void ord_AddGridAmount();  /* 0x4f8460 */
int unk_Unk4f86a0();  /* 0x4f86a0 */
void ord_AdvanceRatingBase(void);  /* 0x4f9c00, ord.c */
void unk_Unk4fdfd0(void);  /* 0x4fdfd0 */
char * he_NextEvent(void);  /* 0x53e300 */
void he_Unk53ed70();  /* 0x53ed70 */
void ai_Unk5a9170(unsigned short *param_1, int param_2);  /* 0x5a9170 */
int ai_Unk5ba750();  /* 0x5ba750 */
void ai_ApplyOptions(void);  /* 0x5d5e90 */
extern const int ai_Unk5edc10[];  /* 0x5edc10 */
extern char *ai_Unk5fb5b0;  /* 0x5fb5b0 */
extern int ai_Unk60bd80;  /* 0x60bd80 */
extern int ai_NextPartyId;  /* 0x61a658 */
extern int ai_Unk624570;  /* 0x624570 */
extern int ai_Unk624574;  /* 0x624574 */
extern int ai_Unk624578;  /* 0x624578 */
extern char ai_MissionMode;  /* 0x624580 */
extern unsigned char ai_NumPlayers;  /* 0x62489c */
extern int ai_MissionPartner;  /* 0x6248a0, the netId of the other player in a network mission (-1 none; set by message 8, cleared by 10) */
extern int ai_Unk631084;  /* 0x631084 */
extern int * ai_Unk633738;  /* 0x633738 */
extern int ai_TradeMsgCount;  /* 0x6f046c */
extern int cm_NoticeCount;  /* 0x6f0cd4 */
extern int ai_GameFlags;  /* 0x6f23f8 */
extern int cm_RequestedSaveSlot;  /* 0x6f25c8 */
extern char cm_RequestedSaveName[];  /* 0x6f25d0 */
extern unsigned int ai_MissionSeed;  /* 0x75e120, the random seed of the missions (as unk_4b39f0.h) */
extern int ai_TimeCopy[4];  /* 0x75e130, the clock copy (14 bytes) */
extern int cm_SaveAcks[][2];  /* 0x76fa20 */
extern int ai_Unk91b6e0[0x800][2];  /* 0x91b6e0 */
extern char ai_Unk91f700[];  /* 0x91f700 */
extern char ai_Unk92cae0[16][0x35c];  /* 0x92cae0 */
extern int cm_DayEndRequest;  /* 0xf31144 */
extern int hst_SeedPending;  /* 0xf83650 */
extern char ai_Unkf895a0[0xb00];  /* 0xf895a0 */
struct D3SM; struct D3TERRAIN; void d3sm_Build(struct D3SM *h, struct D3TERRAIN *t);  /* 0x45e620, d3_sm.c: void d3sm_Build(D3SM *, D3TERRAIN *) */
void hmt_SetPendingElection(int v);  /* 0x4ad1b0 */
int tb_IsFunctionOff();
int tb_SetFunctionOff();

struct CHARACTER * ch_FindByName();  /* 0x402a70 */
int ch_StopActions();  /* 0x405610 */
int ch_StandUp();  /* 0x405ad0 */
struct CHACTION * ch_WalkToObject();  /* 0x40a900 */
struct CHACTION * ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);  /* 0x40b2f0, ch.h ch_AddAction */
struct G15TREE * veg_FindType(int type);  /* 0x4bc5a0 */
struct G15PLANT * veg_FindPlantAt();  /* 0x4bc5f0 */
int veg_Fits(struct G15PLANT *p, int x, int z, int size);  /* 0x4bc710 */
struct G15PLANT * veg_SetPlant();  /* 0x4bc7c0 */
void veg_LoadModel();  /* 0x4bc940 */
void gm_SelectModelVariant(char *o);  /* 0x4fb5a0, f3gm.c: the model variant of the object o */
int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5);  /* 0x4fbd10 */
int gm_FindObjectInst(int id);  /* 0x4fe980 */
int gm_RemoveOwned();  /* 0x5000c0 */
extern char ai_Unk60c4a0[20];  /* 0x60c4a0 */
extern int veg_NextId;  /* 0x6f0748 */
extern char *ai_BTypes;  /* 0x7e3f30 */
#ifndef DATA_VIEWS_H
extern char init_SceneDirty[];  /* 0xfd7f75 */
#endif

int he_Unk555210();  /* 0x555210 */
#ifndef DATA_VIEWS_H
extern char ai_Unkfd7c50[];  /* 0xfd7c50 */
#endif

int ch_QueueAnim();  /* 0x4057a0 */
char * vfs_GetPath();  /* 0x42fca0 */
char d3_SetLightKeyCB(struct D3OBJECT *obj, float *time);  /* 0x449da0 */
struct D3OBJECT * d3_LoadGroup();  /* 0x44c3f0 */
float d3_VecAngle();  /* 0x4539f0 */
void d3_DirToWorld();  /* 0x458810 */
struct GVYARD *gv_GrowGraveyard(unsigned char *o);  /* 0x4eff50 */
int ord_GetKindRecord();  /* 0x4f8890 */
int gm_ClockSeason();  /* 0x4fe350 */
void gm_RefreshGoodsLists(unsigned p);  /* 0x4ff190 */
void gm_AssignBuilding(unsigned char *b, unsigned short place, unsigned own);  /* 0x502b60 */
void ai_Unk58feb0(int obj, int target);  /* 0x58feb0 */
extern const char ai_AxisPair[];  /* 0x5ebb74 */
extern const double ai_Unk5edc28;  /* 0x5edc28 */
extern struct VFSDIR *vfs_Root;  /* 0x6333fc */
extern char ai_DataPath[];  /* 0x75dca0 */

void ai_ResetJob(struct AICHAR *c);  /* 0x47e940 */
int unk_Unk4f3910();  /* 0x4f3910 */
void unk_Unk4fda10(char *c, char *b);  /* 0x4fda10 */
char *unk_Unk4fdc10(unsigned char *b);  /* 0x4fdc10 */
short *unk_Unk4fddb0(unsigned char flags, int p2, int p3, unsigned char p4, unsigned char p5, char *p6);  /* 0x4fddb0 */
unsigned short * gm_QueryNext(void);  /* 0x4ffcc0 */
int gm_BuildingMainObject(unsigned char *b);  /* 0x501b50 */
void gm_Unk5034b0(int o);  /* 0x5034b0 */
extern char *sv_SelectedObject;  /* 0x75ec60 */
typedef struct G17B16 { char b[16]; } G17B16;

int amt_AcceptApplication();  /* 0x4e6430 */
int amt_ExecuteOfficeMessage();  /* 0x4e67a0, amt.h */
int unk_Unk4f27d0();  /* 0x4f27d0 */
void ord_LoadStatistics(void *p);  /* 0x4f9020 */
int gm_ChooseHeadIcon(unsigned short *c);  /* 0x4fb890 */
void gm_SetJob(int i, unsigned n);  /* 0x504790 */
extern char ch_EmptyName[];  /* 0x631090, "" (ch.h) */

int rd_BuildRoad();  /* 0x4bcc20 */
int ai_BuildingName();  /* 0x58f640 */
int ai_VgFilter();  /* 0x58f920 */
int lc_GetGebaeudeBauplatz();  /* 0x58fc40 (unk_58bbd0.c) */
extern int ai_Unk5f8b10;  /* 0x5f8b10 */
extern char cm_ChatLog[];  /* 0x75fba0 */

void ai_Unk4cc720(char *c, float *a, float *b, int x);                              /* 0x4cc720 */
void he_Unk541b40();  /* 0x541b40 */
void ai_Unk5a8c50(unsigned short *param_1, char *param_2, int param_3, int param_4);  /* 0x5a8c50 */

int ch_Delete();  /* 0x4028a0 */
void ai_Unk4d6ba0(char *f);  /* 0x4d6ba0 */
void ai_Unk4d6f60(char *f);  /* 0x4d6f60 */
void ai_Unk4d6f90(char *f);  /* 0x4d6f90 */
void ai_Unk4d7000(char *f);  /* 0x4d7000 */
void ai_Unk4d70a0(char *f);  /* 0x4d70a0 */
void ai_Unk4d7150(char *r);  /* 0x4d7150 */
int ai_Unk4d7360(char *rec, char *c, char *g);  /* 0x4d7360 */
int ai_InventoryHas(void *ch, int id);  /* 0x5ba6b0 */
void ai_Unk4d7190(char *f);  /* 0x4d7190 */
void ai_Unk4e5180();  /* 0x4e5180 */
void amt_ClearOfficesIfAll();  /* 0x4e6e20 */
int ord_HandleMessageRecord();  /* 0x4f9710 */
void ai_GameFrame(void);  /* 0x591520 */

int amt_ExecuteSwapMessage();  /* 0x4e6c50 */
unsigned short ahm_CreateAlchemist(int *p);  /* 0x4ec500, ahm.c (cm.c reads it through ahm_CreateAlchemistI) */
int gv_BuryCharacter();  /* 0x4f0240 */
int gv_ReserveGrave();  /* 0x4f0490 */
int gv_ExtendGrave();  /* 0x4f0770 */
int unk_Unk4f0970();  /* 0x4f0970 */
int unk_Unk4f0af0();  /* 0x4f0af0 */
int unk_Unk4f0c80();  /* 0x4f0c80 */
int unk_Unk4f0dd0();  /* 0x4f0dd0 */
int unk_Unk4f0ed0();  /* 0x4f0ed0 */
unsigned unk_Unk4f1200(char *p);  /* 0x4f1200 */

void rt_FreePoints();  /* 0x4bd9e0 */
struct RTPOINT * rt_CopyPoints();  /* 0x4bda00 */
int ahm_HasRecord();  /* 0x4eaad0 */
int ahm_SetShare();  /* 0x4eca00 */
int ahm_AddPlace();  /* 0x4ecb30 */
int ahm_RemovePlace();  /* 0x4ecbc0 */
int ahm_SetPlaceValue();  /* 0x4ecd10 */
int ahm_AddValue();  /* 0x4ecdb0 */
int ahm_DeleteAlchemist();  /* 0x4ece60 */
int gv_ReserveEffect();  /* 0x4eeaa0 */
int gv_StopEffect();  /* 0x4eeb00 */
int gv_StartShot();  /* 0x4eee00 */

char *ai_Unk4cd350();  /* 0x4cd350 */
char *ai_Unk4cd380(int id);  /* 0x4cd380 */
void ai_Unk4cd3c0(int id, int x, char *text);  /* 0x4cd3c0 */
G18LOG *ai_Unk4cd480();  /* 0x4cd480 */
G18LOG *ai_Unk4cd4c0();  /* 0x4cd4c0 */
void cm_LobbyClearChat();  /* 0x4cd500 */
int cm_LobbyAllReady();  /* 0x4cd510 */
void ai_Unk4cd580(int a, int b);  /* 0x4cd580 */
unsigned int *cm_LobbyPlayerList();  /* 0x4cd5d0 */
char *cm_LobbyFindPlayer(int id);  /* 0x4cd5e0 */
CMLOBBYPLAYER *cm_LobbyMyPlayer();  /* 0x4cd5f0 */
int *cm_LobbyState();  /* 0x4cd600 */
int cm_LobbyRequestList();  /* 0x4cd610 */
int cm_LobbySetReady(int v);  /* 0x4cd650 */
int cm_LobbyLeave();  /* 0x4cd6a0 */
int cm_LobbySendChat(int to, char *text);  /* 0x4cd6e0 */
int cm_LobbyJoin(CMLOBBYPLAYER *p);  /* 0x4cd760 */
int cm_LobbyUpdatePlayer(CMLOBBYPLAYER *p);  /* 0x4cd7c0 */
void ai_Unk4cd820(int v);  /* 0x4cd820 */
int ai_Unk4cd870(G17MSG *m, G17STAT *s);  /* 0x4cd870 */
char *ai_Unk4cd9d0(int id);  /* 0x4cd9d0 */
char *ai_Unk4cda00(char *c);  /* 0x4cda00 */
char *ai_Unk4cda50(char *c);  /* 0x4cda50 */
char *ai_Unk4cdac0(char *c);  /* 0x4cdac0 */
void ai_Unk4cdb20(char *c);  /* 0x4cdb20 */
char ai_Unk4cdb80(char *c);  /* 0x4cdb80 */
float ai_Unk4cdcf0(char *c);  /* 0x4cdcf0 */
void cm_UpdateHandObject(char *c);  /* 0x4cde20 */
int *ai_Unk4cdf00(char *rec, char *c);  /* 0x4cdf00 */
int *ai_Unk4cdf80(char *c);  /* 0x4cdf80 */
int ai_Unk4cdfe0(int x, int y);  /* 0x4cdfe0 */
int ai_Unk4ce040(int a, int b, int *ox, int *oy);  /* 0x4ce040 */
char *ai_Unk4ce130(int id);  /* 0x4ce130 */
int ai_LedSelectBits(int idx, char *c);  /* 0x4ce170 */
float ai_Unk4ce1b0(char *c, char *o);  /* 0x4ce1b0 */
float ai_Unk4ce200(char *c, int x, int y);  /* 0x4ce200 */
int ai_Unk4ce270(char *a, char *b);  /* 0x4ce270 */
char *ai_Unk4ce290(char *p);  /* 0x4ce290 */
void ch_SetHandObject();  /* 0x406c60 */
int util_Random(unsigned short n);  /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
char d3sm_GetVertex(struct D3SM *h, int x, int z, struct D3VECTOR *out);                              /* 0x45f100, d3_sm.c (this unit's callers read an int: d3sm_GetVertexI) */
float gm_HealthRatio();  /* 0x4fc8e0 */
float gm_GetSkill(char *c, int skill);  /* 0x504ec0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);  /* 0x53dda0 */
extern int ai_Unk60c7b0[2];  /* 0x60c7b0 */
extern char ai_Unk60c7c0[];  /* 0x60c7c0 */
extern int ai_Unk6f0cd8;  /* 0x6f0cd8 */
extern int ai_Unk6f0cdc;  /* 0x6f0cdc */
extern char *ai_Unk6f0d04;  /* 0x6f0d04 */
extern int gm_AllowAll;  /* 0x6f2448 */
extern char ai_Unk930600[];  /* 0x930600 */
extern G18LOG ai_Unk935da0[15];  /* 0x935da0 */
extern int ai_Unk9363f4;  /* 0x9363f4 */
extern int ai_Unk936400;  /* 0x936400 */
extern int ai_Unk936404;  /* 0x936404 */
extern int ai_Unk936408;  /* 0x936408 */
extern unsigned int ai_Unk936420[];  /* 0x936420, the count then the eight player states (0x24 bytes; ai_Unk936424
                                        is a view at +4) */
extern CMNETGAME cm_NetGame;  /* 0x936460, the network game */
extern char ai_Unk9364e0[];  /* 0x9364e0 */
extern int ai_Unk9367d0;  /* 0x9367d0 */
extern int ai_Unk9367d4;  /* 0x9367d4 */

int ai_Unk4ce300();  /* 0x4ce300 */
char *ai_Unk4ce5d0(float *p, int x, int y);  /* 0x4ce5d0 */
int ai_Unk4d3390();  /* 0x4d3390 */
unsigned int ai_Unk4e52f0();  /* 0x4e52f0 */
extern int ai_Unk6f0cec;  /* 0x6f0cec */
extern int ai_Unk6f0d14;  /* 0x6f0d14 */
extern char ai_Unk935a00[];  /* 0x935a00 */

int ai_Unk4ce830();  /* 0x4ce830 */
int ai_Unk4ceb70(char *c);  /* 0x4ceb70 */
void ai_Unk4ceb80(int id);  /* 0x4ceb80 */
void ai_Unk4cec00();  /* 0x4cec00 */
void ai_Unk4cec30(int a, char *c);  /* 0x4cec30 */
void ai_Unk4ced80(int a);  /* 0x4ced80 */
void d3fl_MakeDivide();  /* 0x441300 */
struct D3OBJECT; struct D3RECT; char d3if_ScreenBox(struct D3OBJECT *obj, struct D3RECT *r);  /* 0x4486e0 */
int d2_WinOpenSlider();  /* 0x46a960 */
int d2_CreateWindow();  /* 0x472390 */
int d2_RemoveWindow();  /* 0x472ce0 */
int d2_WinOpenPic(short x, short y, int pic, int win);  /* 0x4731d0 */
void d2_SetObjValue();  /* 0x475cb0 */
int gm_SkillPoints();  /* 0x504e00 */
extern int d2_LastWindow;  /* 0x6ed938 */
extern int cm_SkipEvents;  /* 0x6f0d8c */
extern int ai_Unk935d20[];  /* 0x935d20 */

void ai_Unk4cedc0(char *c, int v, int kind);  /* 0x4cedc0 */
void ai_Unk4cee20();  /* 0x4cee20 */
void ai_Unk4cef80();  /* 0x4cef80 */
void ai_Unk4cefa0();  /* 0x4cefa0 */
void ai_Unk4cefe0();  /* 0x4cefe0 */
void d2_MoveObj(short x, short y, int obj);  /* 0x46cda0 */
void d2_RemoveObj();  /* 0x46e710 */
int d2_CreateScreenText(short x, short y, char *text);  /* 0x473860 */
void d2_CloseView(int view);  /* 0x4758f0 */
void d2_EnableObject();  /* 0x475aa0 */
extern int ai_Unk60cb60;  /* 0x60cb60 */
extern G18POP ai_Unk9304c0[16];  /* 0x9304c0 */

void ai_Unk4cf000();  /* 0x4cf000 */
char d3if_ShowTree(struct D3OBJECT *obj, char show);  /* 0x447300 */
float d3_YawTo();  /* 0x448ac0 */
extern int ai_Unk60cb5c;  /* 0x60cb5c */
extern char *ai_Unk6f0d74;  /* 0x6f0d74 */

void cm_DrawScrollArrows();  /* 0x4cf370 */
void * d2_GetPic();  /* 0x4690b0 */
int d2_DrawShape(int x, int y, void *surf, void *pic, unsigned char i);  /* 0x47c6f0 */
void d2_SetClipRect(int x0, int y0, int x1, int y1);  /* 0x47cb40 */
extern float ai_Unk5eb7fc;  /* 0x5eb7fc */
extern float ai_Unk5eb81c;  /* 0x5eb81c */
extern const float ai_Unk5ebba4[];  /* 0x5ebba4 */
extern const float ai_Unk5edc50;  /* 0x5edc50 */
extern float d2_ScreenScale;  /* 0x5fe708 */
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c defines it) */
extern short d2_ScreenH;  /* 0xfb9070 */
extern short d2_ScreenW;  /* 0xfb9072 */

struct D3OBJECT * d3if_Pick(float x, float y, int mask, int one);  /* 0x448630 */
struct D3SM; char d3sm_Pick(struct D3SM *h, int sx, int sy, int *px, int *pz);  /* 0x45f2b0 */
char d3if_Show(struct D3OBJECT *obj, char show);  /* 0x4471b0 */
void ai_Unk4cf670();  /* 0x4cf670 */
extern char *ai_Unk5fb5b0;  /* 0x5fb5b0 */
extern float ai_Unk5ebb68;  /* 0x5ebb68 */
extern int ai_Unk6f0d08;  /* 0x6f0d08 */
extern char *ai_Unk6f0ce0;  /* 0x6f0ce0 */
extern char *ai_Unk6f0cf0;  /* 0x6f0cf0 */
extern char *ai_PickedObject;  /* 0x6f22e8 */
extern int ai_PickedChar;  /* 0x6f22f8 */

void ai_Unk4cf9c0();  /* 0x4cf9c0 */
void d2_SetObjText(int obj, char *text);  /* 0x4734d0 */
void d2_ScaleObject(int obj, int a);  /* 0x475e80 */
void d2_AddPointerItem(int id, int amount);  /* 0x476b90 */
void d2_ClearPointerItems(void);  /* 0x476c70 */
extern int d2_ClickedId;  /* 0xf8a668 */
extern char ai_Unk930400[];  /* 0x930400 */
extern struct D2OBJ *d2_Objs;  /* 0xf8f080, 0x2ec bytes each (d2.c); this header's unit adds byte offsets through d2_ObjsC */

void ai_Unk4cfc10();  /* 0x4cfc10 */
void ai_Unk4cfe70();  /* 0x4cfe70 */
int d2_CreatePicObj();  /* 0x46cd80 */
int d2_CreateWindow();  /* 0x472390 */
int d2_CreateWinText(short x, short y, char *text, int win);  /* 0x473650 */

void ai_Unk4cfeb0();  /* 0x4cfeb0 */
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);  /* 0x4c27d0 */
extern int ai_ClickedObject;  /* 0x6f22e4 */
extern int ai_Unk6f0d24;  /* 0x6f0d24 */
extern int ai_Unk6f0d28;  /* 0x6f0d28 */

int ai_Unk4cffd0(char *rec, char *c, char *tgt, int x, int z);  /* 0x4cffd0 */
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);  /* 0x45f170 */




int ai_Unk4d0200(char *rec, char *c, char *tgt);  /* 0x4d0200 */

int ai_Unk4d02d0(char *rec, char *c, char *tgt);  /* 0x4d02d0 */
int ai_Unk4d0380(char *rec, char *c, char *tgt, unsigned char a);  /* 0x4d0380 */

int ai_Unk4d0460(char *rec, char *c, int a);  /* 0x4d0460 */
void ai_Unk4d0520(char *rec, char *c);  /* 0x4d0520 */
int ai_Unk4d2170(char *c, int cx, int cy, int r, int *ox, int *oy);  /* 0x4d2170 */

char ai_Unk4d0570(char *rec, char *c);  /* 0x4d0570 */


void ai_Unk4d0740(char *rec);  /* 0x4d0740 */
int snd_PlayVoice(int id, int bank, char *text, int var);  /* 0x4c2760 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);  /* 0x446f30 */
extern int ai_Unk6f0d34;  /* 0x6f0d34 */
extern int ai_Unk6f0d38;  /* 0x6f0d38 */

void ai_Unk4d08c0();  /* 0x4d08c0 */
char *ai_Unk4d0920(char *c, int a);  /* 0x4d0920 */
int unk_Unk4fbb80();  /* 0x4fbb80 */
extern const double ai_Unk5eb9b8;  /* 0x5eb9b8 */

void ai_Unk4d0a30();  /* 0x4d0a30 */
char ai_Unk4d0a80(char *o);  /* 0x4d0a80 */
extern int ai_Unk6f0d58;  /* 0x6f0d58 */
extern char *ai_Unk935980[];  /* 0x935980 */

void ai_Unk4d0ca0();  /* 0x4d0ca0 */
int ai_Unk4d0cd0(int *a);  /* 0x4d0cd0 */
void ai_Unk4d0cf0(char *rec, unsigned char a);  /* 0x4d0cf0 */
char ai_Unk4d0f80(char *o);  /* 0x4d0f80 */
float gm_AddHealth(unsigned id, int d);  /* 0x4fca40 */

void ai_Unk4d0fa0(char *rec);  /* 0x4d0fa0 */
int snd_LoadVoiceBank();  /* 0x4c2830 */
struct SBBANK * sb_LoadSampleBank();  /* 0x426950 */
int ai_Unk4d4050();  /* 0x4d4050 */
void ai_Unk4e2e60();  /* 0x4e2e60 */
void ai_Unk4e3650(void);  /* 0x4e3650 */
void d3sky_Init(struct D3SKY *s, char build);  /* 0x45d690 */
struct D3IOOBJ * d3io_LoadObjectRef();  /* 0x44e190 */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);  /* 0x45eeb0, d3_sm.h */
void gm_ShuffledBytes(unsigned char n, unsigned char *out);  /* 0x505be0 */
int gm_BuildingSkill();  /* 0x5092e0 */
extern int ai_Unk633dd4;  /* 0x633dd4 */
extern int ai_Unk6f0ce8;  /* 0x6f0ce8 */
extern int ai_Unk6f0cf4;  /* 0x6f0cf4 */
extern int ai_Unk6f0d0c;  /* 0x6f0d0c */
extern int ai_Unk6f0d10;  /* 0x6f0d10 */
extern int ai_Unk6f0d18;  /* 0x6f0d18 */
extern int ai_Unk6f0d1c;  /* 0x6f0d1c */
extern int ai_Unk6f0d20;  /* 0x6f0d20 */
extern int ai_Unk6f0d2c;  /* 0x6f0d2c */
extern int ai_Unk6f0d30;  /* 0x6f0d30 */
extern int ai_Unk6f0d3c;  /* 0x6f0d3c */
extern int ai_Unk6f0d40;  /* 0x6f0d40 */
extern int ai_Unk6f0d44;  /* 0x6f0d44 */
extern int ai_Unk6f0d48;  /* 0x6f0d48 */
extern int ai_Unk6f0d50;  /* 0x6f0d50 */
extern int ai_Unk6f0d54;  /* 0x6f0d54 */
extern int ai_Unk6f0d5c;  /* 0x6f0d5c */
extern float ai_Unk935d00;  /* 0x935d00 */
extern float ai_Unk60cb58;  /* 0x60cb58 */
extern const double ai_Unk5edc58;  /* 0x5edc58 */
extern double ai_Unk5eb8b8;  /* 0x5eb8b8 */
extern float ai_Unk5eb878;  /* 0x5eb878 */

void ai_Unk4d1a10(char *rec);  /* 0x4d1a10 */
void ai_Unk4d1c20(char *side, int id);  /* 0x4d1c20 */
int ai_Unk4e3060();  /* 0x4e3060 */
int sb_UnloadSampleBank();  /* 0x426ba0 */
void d3io_ReleaseObject();  /* 0x44e2c0 */
int ai_Unk4e30c0();  /* 0x4e30c0 */
void d2_CopyCaption(char *caption);  /* 0x478870 */
char ch_FindFreeCell(struct D3SM *h, struct D3VECTOR *from, struct D3VECTOR *to, int x, int z, int *px, int *pz, int max, int flags); /* 0x4072a0, ch.h (cm.c calls it through ch_FindFreeCellK) */

void ai_Unk4d1e00();  /* 0x4d1e00 */
void ai_Unk4d1eb0();  /* 0x4d1eb0 */
char *ai_Unk4d1f10(char *c);  /* 0x4d1f10 */
void snd_PlaySpecialTrack(char *name);  /* 0x4c23f0 */
extern int snd_MusicEnabled;  /* 0x6f2578 */
extern int sv_FrameMask;  /* 0x760638 */
extern int ai_Unk6f0d4c;  /* 0x6f0d4c */
extern char ai_Time[];  /* 0x770ae0 */

float ai_Unk4d2010(char *c, int a, int b, float limit);  /* 0x4d2010 */
extern const double ai_Unk5edc60;  /* 0x5edc60 */

int ai_Unk4d22d0(char *c, int cx, int cy, int r, int *ox, int *oy);  /* 0x4d22d0 */
char ai_Unk4d2450(char *o, int *list);  /* 0x4d2450 */
char *ai_Unk4d2490(char *c);  /* 0x4d2490 */
char ai_Unk4d2550(char *o, char *c);  /* 0x4d2550 */
char *ai_Unk4d25a0(char *c);  /* 0x4d25a0 */
char ai_Unk4d2650(char *o);  /* 0x4d2650 */
char *ai_Unk4d2680(char *c);  /* 0x4d2680 */
extern int ai_Unk6f0d60;  /* 0x6f0d60 */
extern char *ai_Unk930380[];  /* 0x930380 */
extern float ai_Unk5eb788;  /* 0x5eb788 */
extern float ai_Unk5eb988;  /* 0x5eb988 */
extern double ai_Unk5eb990;  /* 0x5eb990 */
extern double ai_Unk5eb7b0;  /* 0x5eb7b0 */
extern double ai_Unk5ebbe8;  /* 0x5ebbe8 */

void ai_BuildFloatTable();  /* 0x4d27a0 */
extern float ai_Unk9300a0[0xb4];  /* 0x9300a0 */
extern double ai_Unk5ece60;  /* 0x5ece60 */

float *ai_Unk4d2c00(int side);  /* 0x4d2c00 */
extern float ai_Unk5ebc1c;  /* 0x5ebc1c */
extern const float ai_Unk5ecebc;  /* 0x5ecebc */
extern float ai_Unk5eb94c;  /* 0x5eb94c */

float ai_Unk4d2e30(char *c, int type);  /* 0x4d2e30 */
void ai_Unk4d2f80(int side);  /* 0x4d2f80 */
float util_RandomF();  /* 0x42ec20 */
double ai_Unk4e5360();  /* 0x4e5360 */
extern float ai_Unk5eb780;  /* 0x5eb780 */
extern double ai_Unk5eb9b0;  /* 0x5eb9b0 */

void ai_Unk4d3120(char *rec, char *c);  /* 0x4d3120 */
void ai_Unk4d3300(char *c);  /* 0x4d3300 */
extern float ai_Unk5eb7dc;  /* 0x5eb7dc */
extern const double ai_Unk5edc68;  /* 0x5edc68 */

int ai_Unk4d3390(char *c);  /* 0x4d3390 */
struct CHACTION * ch_AddActionCB(struct CHARACTER *ch, int (*func2)(struct CHACTION *), unsigned char frame, unsigned int type, ...); /* 0x40b400, ch.h (CHACTFUNC func2; cm.c calls it through ch_AddActionCBK) */
void d3_SetWorldPosRot();  /* 0x449920 */
extern double ai_Unk5edbd0;  /* 0x5edbd0 */
extern double ai_Unk5eb8e0;  /* 0x5eb8e0 */

void ai_Unk4d3550(char *ev);  /* 0x4d3550 */
void ai_Unk4d3980(char *ev);  /* 0x4d3980 */
void ai_Unk4d3a30(char *ev);  /* 0x4d3a30 */
void ai_Unk4d3ac0(char *ev);  /* 0x4d3ac0 */
void ai_Unk4d3ae0(char *ev);  /* 0x4d3ae0 */
char *ai_Unk4d3b30(char *rec);  /* 0x4d3b30 */
int ai_Unk4d3d10(char *a, char *b, char *d, char *t);  /* 0x4d3d10 */
int ai_Unk4d4050(unsigned char *b);  /* 0x4d4050 */
int ai_Unk4d4090(int x, int y, int mode);  /* 0x4d4090 */
void ai_Unk4d4340(int *ids, int x, int y);  /* 0x4d4340 */
int ai_Unk4d4540(char *a);  /* 0x4d4540 */
void ai_Unk4d4d70(char *a);  /* 0x4d4d70 */
int ai_Unk4d60b0(char *a);  /* 0x4d60b0 */
void ai_Unk4d6930(char *r);  /* 0x4d6930 */
void ai_Unk4d6970(char *r);  /* 0x4d6970 */
void ai_Unk4d6a50();  /* 0x4d6a50 */
void ai_Unk4d6b70();  /* 0x4d6b70 */
extern int ai_Unkfa5f00;  /* 0xfa5f00 */
extern int ai_Unkfa5f04;  /* 0xfa5f04 */
int d2_WinCreateSlider();  /* 0x46bf10 */
void d2_CenterObj();  /* 0x46c1d0 */
extern int ai_Unk6f0d84;  /* 0x6f0d84 */
extern int ai_Unk6f0d80;  /* 0x6f0d80 */
extern int ai_Unk6f0d88;  /* 0x6f0d88 */
extern int ai_Unk60cb64;  /* 0x60cb64 */
extern int ai_Unk60cb68;  /* 0x60cb68 */
extern int ai_Unk6f0d64;  /* 0x6f0d64 */

extern int ai_Unk60d368;  /* 0x60d368 */
void ai_Unk4d63e0(char *a);  /* 0x4d63e0 */
unsigned int ai_Unk4e52c0();  /* 0x4e52c0 */
void ai_Unk4e52a0();  /* 0x4e52a0 */
void ai_Unk4e3830();  /* 0x4e3830 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);  /* 0x449de0 */
extern int ai_Unk60d364;  /* 0x60d364 */
extern int ai_Unk6f244c;  /* 0x6f244c */
void ai_Unk4e48b0();  /* 0x4e48b0 */



int gm_TitleGroup();  /* 0x504ad0 */
int gm_BuildingJob();  /* 0x5048e0 */
void gm_ChangeBuildingState(unsigned char *b, int d);  /* 0x50cad0 */

int d2_OpenForm();  /* 0x4741f0 */
void d2_PlaceMainWindow();  /* 0x475850 (int view, unsigned char flags), K&R */
int d2_SetForm(int view, int n);  /* 0x475f70 */
int d2_GetWinObj(int view, int idx, int k);  /* 0x475b80 */
int io_PrintText(char *fmt, ...);  /* 0x515960 */
int ai_Unk4e5470();  /* 0x4e5470 */
int gm_StallGoodsList(char *c, unsigned *out);  /* 0x50aa00 */
extern int ai_Unk6f0cf8;  /* 0x6f0cf8 */
extern int ai_Unk6f0cfc;  /* 0x6f0cfc */
extern struct D2WIN *d2_LastWindowPtr;  /* 0x6ed980, window created last (d2.c); cm.c adds byte offsets through d2_LastWindowPtrC */

int ai_Unk4d3ea0(char *a, char *b, char *d, int unused, float f);  /* 0x4d3ea0 */
float gm_GoodsPrice(unsigned id, unsigned char p);  /* 0x5089c0, a price */
int gm_GetGoodsValue(char *ch);  /* 0x509e60 */
struct D3HMAP * ch_GetHMap();  /* 0x402140 */
int ai_Unk405810();  /* 0x405810 */
void ch_SetVisible();  /* 0x402480 */

extern int ai_Unk92bec0;  /* 0x92bec0 */
void ai_Unk4d76a0(int *rec);  /* 0x4d76a0 */

extern int ai_Unk6f0ce4;  /* 0x6f0ce4, fight paused */

extern int ai_Unk6f0d7c;  /* 0x6f0d7c */
struct MSXTRACK * msx_PlayAmbient(char *name, int vol);  /* 0x4243f0 */
void ai_Unk4d88c0(char *a);  /* 0x4d88c0 */
void ai_Unk4d8b10(char *a);  /* 0x4d8b10 */

extern int ai_Unk92ca80[16];  /* 0x92ca80 */
extern unsigned int cm_FightStart;  /* 0x6f0da4, fight start (d3_Time) */
void * d2_SetFont(int font);  /* 0x476030 */
void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags);  /* 0x46f1a0 */
void d2_AddDirty(int x, int y, int w, int h);  /* 0x468eb0 */
extern int d2_TextDrawnW;  /* 0x6ed968 */
extern int d2_TextDrawnH;  /* 0x6ed96c */
void ai_Unk4d8b30();  /* 0x4d8b30 */
void cm_DrawFightTime();  /* 0x4d8b50 */

int ai_Unk4e3250();  /* 0x4e3250 */
void ai_Unk4d8c40();  /* 0x4d8c40 */

int d2_RemoveWindowObjects();  /* 0x472d20 */
void d2_SetObjFont(int obj, int font);  /* 0x4760b0 */


/* ---- declarations of cut.c (joined) ---- */
/* Declarations of cut.c (the cutscene code 0x4d8c70..: council sessions, duels, executions; tag "cut_MainDuell()"). Names are ours. */

extern int ai_Unk6f0d6c;  /* 0x6f0d6c */
extern int ai_Unk6f0d68;  /* 0x6f0d68 */
extern int ai_Unk6f0d70;  /* 0x6f0d70 */
void ai_Unk4d8c70(int unused, char *o);  /* 0x4d8c70 */
void ai_Unk4d8cb0(int a, int b, int c);  /* 0x4d8cb0 */

typedef struct G22ENT {                 /* 0x18 bytes, one participant of a session (amt_GetColumnOffices) */
    int unk00;
    int id;                             /* 0x04, character id */
    int unk08;
    int flag;                           /* 0x0c */
    char kind;                          /* 0x10, 2 = candidate, 3 = voter */
    char pad[7];
} G22ENT;
#define G22_4C5580 ((int (*)())ai_Unk4c5580)  /* cut.c calls it without arguments */
extern char *ai_Unk6ed918;  /* 0x6ed918 */
int ai_Unk4e3270();  /* 0x4e3270 */
int ai_Unk4e3620();  /* 0x4e3620 */
int amt_GetColumnOffices(int col, int max, struct AMTREC *out);  /* 0x4e6fc0 */
int amt_IsColumnDecided();  /* 0x4e83b0 */
void amt_ElectOffices();  /* 0x4e7c60 */
int ai_Unk4e8530();  /* 0x4e8530 */
int amt_CanTakeOffice();  /* 0x4e75c0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
void d2_CloseView(int view);  /* 0x4758f0 */
int ai_AmountRowStatus();  /* 0x5be1d0 */
int ai_AmountRow();  /* 0x5be3b0 */
int ai_Unk4e32d0();  /* 0x4e32d0 */
int ai_Unk4e3570();  /* 0x4e3570 */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);  /* 0x4e6680 */
unsigned * gm_CharNameRecord();  /* 0x4fb390 */
int ai_Unk4e2c30();  /* 0x4e2c30 */
int ai_Unk4e2c70();  /* 0x4e2c70 */
int ai_Unk4e3290();  /* 0x4e3290 */
int ai_Unk4e2d40();  /* 0x4e2d40 */
int ai_Unk4e3640();  /* 0x4e3640 */
void amt_UpdateOffices();  /* 0x4e85d0 */
void ai_CampaignStep(int a, int b, int c, int d);  /* 0x576040 */
void ai_Unk4d8e20(char *a);  /* 0x4d8e20 */
extern int ai_Unk60d360;  /* 0x60d360, the frame wait of the dialogs (0x5309a0) */
extern int d2_ClickedId;  /* 0xf8a668 */
int d2_GetWinObj();  /* 0x475b80 */
int ai_Unk4e53e0();  /* 0x4e53e0 */
int ai_TrustedOfTwo();  /* 0x4aab90 */
int ai_TrustedOfFour();  /* 0x4aad30 */
int amt_GetOffice(unsigned char i, struct AMTREC *out);  /* 0x4e6f30, amt.c (this unit calls it through amt_GetOfficeI) */
int amt_GetKind(int k, struct AMTKIND *out);  /* 0x4e6f80 */
int amt_GetOfficeApplicants();  /* 0x4e76b0 */

int ai_Unk4da9e0(char *a, char *b, char *c, int *out);  /* 0x4da9e0 */
int ai_Unk4dab50(int a, char *b, int *c, int *out);  /* 0x4dab50 */
void ai_Unk4dada0(char *r, int *m);  /* 0x4dada0 */

extern int ai_Unk6f0d94;  /* 0x6f0d94 */
extern int ai_Unk92bca4;  /* 0x92bca4 */
extern unsigned char ai_Unk6f0da8[];  /* 0x6f0da8, +2 / +3: the left / right insult won, +4 / +5 the shot won */
int ai_Unk4e50e0();  /* 0x4e50e0 */
int ai_Unk4e3420();  /* 0x4e3420 */
void ai_Unk4de4b0(float a, char *o);  /* 0x4de4b0 */
int ai_Unk4de530(char *c, int dmg);  /* 0x4de530 */
void ai_Unk4de900(char *a, char *b);  /* 0x4de900 */
extern int ai_Unk60d370;  /* 0x60d370 */
extern int ai_Unk6f0db0;  /* 0x6f0db0 */
extern int ai_Unk92bcc0[];  /* 0x92bcc0 */
void gm_Shuffled(unsigned char n, int *out);  /* 0x505ca0 */
void ai_Unk4de5e0(int n);  /* 0x4de5e0 */
int ai_Unk4de610(int n);  /* 0x4de610 */
void ai_Unk4de640(char *o);  /* 0x4de640 */
extern unsigned char ai_Unk6f0daa;  /* 0x6f0daa */
extern int ai_Unk92bca0;  /* 0x92bca0 */
int amt_FindOffice();  /* 0x4e6030 */
void ai_Unk4debe0(char *m);  /* 0x4debe0 */
int ai_Unk4e36a0();  /* 0x4e36a0 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);  /* 0x44a170 */
int ai_Unk4e2bf0();  /* 0x4e2bf0 */
int ai_Unk4e32b0();  /* 0x4e32b0 */
int sb_IsPlaying();  /* 0x426f20 */
int ai_Unk4e2d90();  /* 0x4e2d90 */
void ai_Unk4ded10(char *m);  /* 0x4ded10 */



int gv_Accuse();  /* 0x4f2e30 */
int ai_Unk4dfa20(char *m);  /* 0x4dfa20 */
void ai_Unk4dfcb0(char *m);  /* 0x4dfcb0 */
void ai_Unk4dfdd0(char *m);  /* 0x4dfdd0 */
void ai_Unk4dff80(char *m);  /* 0x4dff80 */
void ai_Unk4dfff0(char *m);  /* 0x4dfff0 */
int ai_Unk4e0010(int scene);  /* 0x4e0010 */

int ai_Unk4e0040(char *m);  /* 0x4e0040 */

extern char sv_SkyHeavy[];  /* 0x623ac0, 76-byte sky records: Sky_Schwer / Sky_Mittel / Sky_Schoen */
int ai_Unk4e32f0();  /* 0x4e32f0 */
int ai_Unk4e33d0();  /* 0x4e33d0 */
int ai_Unk4e33f0();  /* 0x4e33f0 */
int ai_Unk4e3390();  /* 0x4e3390 */
void ai_Unk4e01e0(char *m);  /* 0x4e01e0 */
int ai_Unk4e0580(char *m);  /* 0x4e0580 */
void sb_Stop();  /* 0x427290 */
int ai_Unk4e06d0(int *s);  /* 0x4e06d0 */
int ai_Unk4e3490();  /* 0x4e3490 */
int ai_Unk4e2cc0();  /* 0x4e2cc0 */
int ai_Unk4e3270();  /* 0x4e3270 */
int ai_Unk4e32b0();  /* 0x4e32b0 */
int sb_IsPlaying();  /* 0x426f20 */
int ai_Unk4e0770(char *m);  /* 0x4e0770 */

int ai_Unk4e0e90(char *m);  /* 0x4e0e90 */
extern struct D2INPUT *d2_FocusedInput;  /* 0x6ed9dc, the focused input (as d2.h / rt.h) */
extern int ai_Unk6f2438;  /* 0x6f2438 */
void snd_ClearQueue(void);  /* 0x4c01b0 */
int ai_Unk4e3290();  /* 0x4e3290 */
void d2_CentreViewWindows(int view);  /* 0x475600 */
void d2_SetObjMax();  /* 0x475f40 */
void d2_FocusInput();  /* 0x478320 */
int d2_GetObjValue();  /* 0x475980 */
int ai_Unk4e2bf0();  /* 0x4e2bf0 */
int ai_Unk4e32d0();  /* 0x4e32d0 */
int ai_Unk4e0f70(char *m);  /* 0x4e0f70 */
int ai_Unk4e1220(char *m);  /* 0x4e1220 */
float gm_CharacterHealth();  /* 0x4fc820 */
int gm_Unk506480();  /* 0x506480 */
void gm_Unk506e70(unsigned short *c, char *out, int flag);  /* 0x506e70 */
void ai_FamilyTreeWindow(int param_1, unsigned char param_2, int param_3, int param_4);  /* 0x5c4150 */
int ai_Unk4e1250(char *m);  /* 0x4e1250 */
/* ai_TimeHour (0x770ae4, the clock's hour) is no object: time_view.h views it in ai_Time + 4 */



int ai_Unk494020();  /* 0x494020 */
int ai_Unk4941b0();  /* 0x4941b0 */
int ai_Unk4941d0();  /* 0x4941d0 */
int ai_Unk4941e0();  /* 0x4941e0 */
int ai_Unk494410();  /* 0x494410 */
int ai_Unk4946c0();  /* 0x4946c0 */
int gm_GetWorth(unsigned short id);  /* 0x50a640 */
int ai_Unk4945c0();  /* 0x4945c0 */
int unk_Unk4f4420();  /* 0x4f4420 */
void d2_SetObjEnabled(int obj, int on);  /* 0x475e40 */
extern char *ai_Unk6332b4;  /* 0x6332b4 */
int ai_Unk4e2bd0();  /* 0x4e2bd0 */
int ai_Unk4e2dc0();  /* 0x4e2dc0 */
int ai_Unk4e2e00();  /* 0x4e2e00 */
void snd_EndSpecialTrack(void);  /* 0x4c2480 */
extern int cm_SceneRain;  /* 0x6f0dbc */
extern int ai_Unk6f0db4;  /* 0x6f0db4 */
extern int ai_Unk6f0db8;  /* 0x6f0db8 */
struct D3SKY; struct D3SKYLAYER; void d3sky_DeleteLayer(struct D3SKY *s, struct D3SKYLAYER *l);  /* 0x45d800 */
void d3sky_Destroy();  /* 0x45daa0 */
int ai_Unk4e37a0();  /* 0x4e37a0 */
int ai_Unk4e3420();  /* 0x4e3420 */
int ai_Unk4e3750();  /* 0x4e3750 */
int ai_Unk4e3800();  /* 0x4e3800 */
int ai_Unk4e37a0();  /* 0x4e37a0 */
struct D3SKY; struct D3SKY *d3sky_Create(float horizon, float fogstart, float fogscale);  /* 0x45da30 */
/* d3sky_CreateLayer (0x45d880): declared in cm's head after d3.h (its D3TEXFLAGS parameter is passed by value) */
void d3sky_SetLayerSpeed(struct D3SKY *s, struct D3SKYLAYER *l, float speed);  /* 0x45d960 */
void d3sky_FadeLayer(struct D3SKY *s, struct D3SKYLAYER *l, unsigned char alpha, float time);  /* 0x45d9a0 */
int ai_Unk4e32f0();  /* 0x4e32f0 */
int ai_Unk4e3570();  /* 0x4e3570 */
int ai_Unk4e3490();  /* 0x4e3490 */
extern char *ai_Unk6f0da0;  /* 0x6f0da0, the fade 0x4e3060 started */
int ai_Unk4e36a0();  /* 0x4e36a0 */
extern char *ai_Unk1468ae0[0x300];  /* 0x1468ae0, the objects */
int tb_SetFunctionOff();  /* 0x42ea10 */
int ai_Unk4e3fe0();  /* 0x4e3fe0 */
int ai_Unk4e4020();  /* 0x4e4020 */
extern int ai_Unk6f0d9c;  /* 0x6f0d9c, the scene nesting depth */
extern int ai_Unk60d36c;  /* 0x60d36c, -1 or the object waited for (0x4284a0) */
extern int ai_Unk5f8438;  /* 0x5f8438 */
extern int d3t_CamMoving;  /* 0x6310e4 */
void d3sm_Destroy();  /* 0x45ef60 */
void d3if_Reset();  /* 0x4473f0 */
int ai_Unk4e3210();  /* 0x4e3210 */
struct CHARACTER * ch_Create();  /* 0x403390 */
int ai_Unk4e3230();  /* 0x4e3230 */
int ai_Unk4e1700(char *m);  /* 0x4e1700 */
int ai_Unk4e19a0(char *m);  /* 0x4e19a0 */


int gv_GetTableRecord(int i, int *out);  /* 0x4f2720 */
int unk_Unk4f3580();  /* 0x4f3580 */
float gm_Liking();  /* 0x50c550 */
void unk_Unk4f48f0();  /* 0x4f48f0 */
void unk_Unk4f4460();  /* 0x4f4460 */
int ai_Unk494830();  /* 0x494830 */
int ai_Unk494300();  /* 0x494300 */
int ai_Unk4947a0();  /* 0x4947a0 */
int sb_IsPlaying(struct SBSAMPLE *s);  /* 0x426f20 */
void msx_StopAmbient(struct MSXTRACK *t);  /* 0x4244b0 */

int ai_Unk5bd060();  /* 0x5bd060 */
int gm_ConvertMoney(int a, unsigned char k);  /* 0x5085b0 */
int gm_PlayerFactor(int a, unsigned char k);  /* 0x5085f0 */
int d2_GetObjValue();  /* 0x475980 */
void d2_SetObjValue();  /* 0x475cb0 */
void ai_Unk4e2910();  /* 0x4e2910 */

int d2_ClearSurface();  /* 0x4796c0 */
void d2_BlitPart(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst);  /* 0x479480 */
void d2_ReleaseFade(struct D2FADE *f);  /* 0x476860 */
struct D2FADE * d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags);  /* 0x4767d0 */
char d3_InflateScene(unsigned int flags);  /* 0x43deb0 */
extern char *ai_Unk6ed92c;  /* 0x6ed92c */
extern unsigned int ai_Unk63349c;  /* 0x63349c */
extern unsigned int ai_Unk6f0d98;  /* 0x6f0d98 */

char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));  /* 0x44be60 */
void d3fl_SetDetail();  /* 0x43f8a0 */
void d3fl_SetShift(struct D3FLMAP *map, unsigned char shift, char light);  /* 0x446dc0, d3_fl.c */
void d3if_Prerender(char map);  /* 0x447860 */
#ifndef DATA_VIEWS_H
extern char ai_Unk6f35ca;  /* 0x6f35ca */
#endif

extern int ai_Unk6f0d78;  /* 0x6f0d78 */
extern int ai_Unk92c504[];  /* 0x92c504, the scene kind table (5 ints per kind) */
extern int ai_Unk92c508[];  /* 0x92c508 */
extern char ai_Unk92c50c[];  /* 0x92c50c */
extern char ai_Unk60c6c0[];  /* 0x60c6c0 */


void ai_Unk4e4060();  /* 0x4e4060 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);  /* 0x4fe140 */
#ifndef DATA_VIEWS_H
extern char ai_Unk925538[];  /* 0x925538, the scene party records (0x114 bytes, +0x18 used) */
#endif


extern int ai_Unk92c510[];  /* 0x92c510, the answer callbacks of the kind table (5 ints per kind) */

int ai_Unk4e4f40();  /* 0x4e4f40 */
char ai_RunPendingParties(int pl);  /* 0x4e4f80, runs the pending parties of the player pl */
void ai_DebugDumpPerson(char *c);  /* 0x4e5bf0, the debug dump of a person */
int ai_Unk4e5050();  /* 0x4e5050 */
int ai_Unk4e5120();  /* 0x4e5120 */


extern unsigned int ai_Unk92c4e0;  /* 0x92c4e0, the random seed of the scenes */
extern int ai_ClockMs;  /* 0x6333cc, a clock */
extern int ai_Unk6f0dc0;  /* 0x6f0dc0 */

extern int ai_Unk60d374;  /* 0x60d374 */

int d2_CreateRadioGroup(int n, ...);  /* 0x46c490 */
void d2_RadioAdd(int g, int obj);  /* 0x46c560 */
void d2_RadioSelect(int g, int sel);  /* 0x46c630 */
void d2_MenuNavigate(int g);  /* 0x46c6c0 */
void d2_RaiseViewWindows(int view);  /* 0x4741a0 */
void d2_CentreViewWindows(int view);  /* 0x475600 */
void d2_RadioClear(int g);  /* 0x46c600 */
extern char ai_Unk6f0dc4;  /* 0x6f0dc4 */

extern int ai_Unk92c500[];  /* 0x92c500, the scene kind table: run, callbacks, flag, check */
void d3io_NoopCallback(void *t);  /* 0x44ee40 */

extern char ai_Unk60eae0[][24];  /* 0x60eae0, the names of the NPC kinds */









extern float cm_SpeedFactor;  /* 0x5f1290 */
extern int ai_Unk631064;  /* 0x631064 */
extern int ai_Unk925510;  /* 0x925510 */
extern int ai_Unk92cac0;  /* 0x92cac0 */
extern char ai_Unk92c600[];  /* 0x92c600, 0x480 bytes of scene state */
extern unsigned char d3_FogByDistance;  /* 0x6334a5 */
extern char d2_CaptionCopy[0x80];  /* 0xf8aae0, the d2 caption / status text buffer */
extern unsigned short ai_Unk92bee0[];  /* 0x92bee0 */
int tb_OpenFunction();  /* 0x42e970 */
int tb_CloseFunction();  /* 0x42e9d0 */
void d2_SetPointerSprite(int a, int pic);  /* 0x476f80 */

/* copy of am.h's AI_ZERO (the headers do not compile together) */
/* The engine's inline-asm clear (d3.h D3_ZERO), byte stores up to dword alignment, dwords, the rest. */
#define AI_ZERO_CAT(a, b) a##b
#define AI_ZERO_LABEL(n) AI_ZERO_CAT(ai_zero_tail, n)
#define AI_ZERO(dst, size) {                                        \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}



/* the same with the destination named in the asm (a global: use `offset name`) */
#define AI_ZERO_AT(var, size) {                                     \
    int _zn = (size);                                               \
    __asm mov edi, var                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}

#endif
