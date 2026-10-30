/* Declarations of hmt.c (unk_4ac140.c until layout 2026-10-22; 0x4ac140.., the table of 51 records at 0x5ed1e8 and its helpers). Names are ours. */
#ifndef UNK_4AC140_H
#define UNK_4AC140_H

struct AMTREC;
struct AICHAR;
#include "am.h"
#include "vfs.h"

typedef struct HMTREC {                 /* 0x2c bytes, a person (p_%i) or an event (k_%i) of historical.ini */
    int idx;                            /* 0x00, running number (events first) */
    int begin;                          /* 0x04 */
    int end;                            /* 0x08 */
    char vname[0x10];                   /* 0x0c */
    char nname[0x10];                   /* 0x1c */
} HMTREC;
typedef struct HMTFILE {                /* historical.dfn after the magic */
    int np;                             /* 0x00, "pcnt" */
    int nk;                             /* 0x04, "kcnt" */
    HMTREC *p;                          /* 0x08 */
    HMTREC *k;                          /* 0x0c */
} HMTFILE;
typedef struct HMTOBJ {                 /* 0x18 bytes, filled by 0x4e6ec0 */
    int obj;                            /* 0x00 */
    int id;                             /* 0x04, a character id or -1 */
    char pad08[0x10];
} HMTOBJ;
typedef struct HMTCAND {                /* a candidate of the election 0x4ad1c0 */
    AICHAR *c;
    float score;
} HMTCAND;

typedef struct G11REC {                 /* 0x30 bytes, a record of the table at 0x5ed1e8 (51: 7 talents, 21 professions, 23 events) */
    int kind;                           /* 0x00, 0 talent (id 1..4 = the skill at +0x80 + id), 1 profession (id 1..12), 2 historical event */
    unsigned short id;                  /* 0x04 */
    short pad06;
    int textSet;                        /* 0x08, 0 / 1: selects the text block 0x1e2b or 0x1e35 (+n) */
    char pad0c[0x24];                   /* 0x0c a text id, 0x0e the name string (e.g. "KAMPF_1", "PEST") */
} G11REC;
extern const G11REC hmt_Records[51];                                                  /* 0x5ed1e8 */
unsigned short hmt_RecordCount(void);                                               /* 0x4ac140 */
unsigned short hmt_RandomRecord(short k);                                            /* 0x4ac150 */
unsigned short hmt_RandomTextOffset(unsigned short k);                                   /* 0x4ac170 */
unsigned short hmt_RecordTextId(unsigned short k, unsigned short n);                 /* 0x4ac1a0 */
G11REC *hmt_GetRecord(unsigned short k);                                          /* 0x4ac1e0 */
int hmt_GetPrice(AICHAR *p);                                                     /* 0x4ac200, 500 * p's worth / the richest's, 1600..16000; 0 above p's goods value */
void hmt_ApplyRecord(AICHAR *p, int d, unsigned short k);                           /* 0x4ac2f0 */
extern HMTFILE hmt_File;                                                         /* 0x600f60, {-1, -1, 0, 0} */
extern int hmt_CurrentPerson;                                                    /* 0x6f0710, idx of the person in play, -1 for none */
extern int hmt_Person;                                                           /* 0x6f0714, idx of the person to appear next */
extern int hmt_CurrentEvent;                                                     /* 0x6f0718, idx of the event in play, -1 for none */
extern int hmt_Event;                                                            /* 0x6f071c, idx of the event to appear next */
extern int hmt_PendingElection;                                                  /* 0x6f0720, the election to decide (0..2), -1 for none */
int hmt_CreateBinaryFile(void);                                                  /* 0x4ac440 */
int hmt_WriteBinaryFile(HMTFILE *h);                                            /* 0x4ac8a0 */
void hmt_Init(void);                                                             /* 0x4aca90 */
int hmt_ReadBinaryFile(HMTFILE *h);                                              /* 0x4acae0 */
void hmt_Reset(void);                                                            /* 0x4acdc0 */
int hmt_Exit(void);                                                              /* 0x4acde0 */
void hmt_Update(void);                                                           /* 0x4ace40 */
HMTREC *hmt_Find(int idx, int *kind);                                            /* 0x4ad010 */
void hmt_Begin(void);                                                            /* 0x4ad0a0 */
int hmt_GetElectionObjType(void);                                                         /* 0x4ad190 */
void hmt_SetPendingElection(int v);                                                       /* 0x4ad1b0 */
extern const int hmt_ElectionObjTypes[3];                                                     /* 0x5edb78, .rdata: object type per election */
extern const unsigned int hmt_ElectionGroupBits[3];                                            /* 0x5edb84, .rdata: candidates' group bits */
extern const unsigned int hmt_ElectionExcludedBits[3];                                            /* 0x5edb90, .rdata: excluded group bits */
extern const int hmt_ElectionOfficeBits[3];                                                     /* 0x5edb9c, .rdata: the office */
void hmt_Election(void);                                                         /* 0x4ad1c0 */
void hmt_SendElectionMessage(int v);                                                       /* 0x4ad3b0 */
float hmt_Score(AICHAR *c, void *obj, int type, int twice);                      /* 0x4ad3e0 */
void hmt_ClearOffices(void);                                                        /* 0x4ad510 */
void hmt_Unk4ad580(void);                                                        /* 0x4ad580 */
void hmt_Unk4ad7a0(void);                                                        /* 0x4ad7a0 */
extern const int hmt_Unk5ee114;                                                        /* 0x5ee114, 8000000 */
void gm_Shuffle(unsigned n, int *out);                                              /* 0x505d30, defined elsewhere (shuffles ids) */
int gm_Unk503f30(int id);                                                        /* 0x503f30, defined elsewhere */
int cm_GetSlotState(int h);                                                         /* 0x4c3ae0, defined elsewhere */
void sv_PumpNet(void);                                                         /* 0x52fe00, defined elsewhere */
int cm_GetSlotValue(int h);                                                     /* 0x4c3b10, defined elsewhere */
void hmt_Create(int type, int unk, unsigned short v, struct AICHAR *c);                 /* 0x4adbb0 */
void hmt_NextEvent(void);                                                        /* 0x4ade60 */
void hmt_NextPerson(void);                                                       /* 0x4adec0 */
int hmt_RandomHeadIcon(void);                                                         /* 0x4adf20 */
void amt_SetEventOwner(int id);                                                       /* 0x4e6b40, defined elsewhere */
void amt_SetPersonOwner(int id);                                                       /* 0x4e6b70, defined elsewhere */
int gm_AddObjekt(int id, int proto, int n);                            /* 0x500970, defined elsewhere */
#ifndef DATA_VIEWS_H
extern char ai_Unkf42400;                                                        /* 0xf42400 */
#endif
int hmt_Load(void *f);                                                           /* 0x4ada80 */
int hmt_Save(VFSFILE *f);                                                        /* 0x4adb20 */
int ls_Read(void *buf, int size, int n, int f);                             /* 0x51c920, ls.c: true when the fread returned a positive count */
char * amt_GetSuperiorOfficeHolder(int type);                                                    /* 0x4e7550, defined elsewhere */
void cm_SendExSysMessage(unsigned char id, void *msg);                                            /* 0x4c4db0, defined elsewhere */
void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);  /* 0x4c4650, defined elsewhere */
int amt_GetFirstOffice(unsigned int k, struct AMTREC *out);                                         /* 0x4e6ec0, defined elsewhere */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);                /* 0x4e6680, defined elsewhere */
void cm_SendKillCharacter(int a, int b);                                                /* 0x4c4e00, defined elsewhere */
extern char ch_EmptyName[];                                                      /* 0x631090, "" */
int gm_FindRichest(short *id, int *rate);                                          /* 0x4fad60, defined elsewhere: the id and worth of the richest active character */
#endif
