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

#include "hmt_types.h"
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
void hmt_DecideElection(void);                                                        /* 0x4ad580 */
void hmt_Unk4ad7a0(void);                                                        /* 0x4ad7a0 */
extern const int hmt_Unk5ee114;                                                        /* 0x5ee114, 8000000 */
void hmt_Create(int type, int unk, unsigned short v, struct AICHAR *c);                 /* 0x4adbb0 */
void hmt_NextEvent(void);                                                        /* 0x4ade60 */
void hmt_NextPerson(void);                                                       /* 0x4adec0 */
int hmt_RandomHeadIcon(void);                                                         /* 0x4adf20 */
#ifndef DATA_VIEWS_H
extern char ai_Unkf42400;                                                        /* 0xf42400 */
#endif
int hmt_Load(void *f, int version);                                              /* 0x4ada80, version: the save game's (ls_LoadGame passes ls_Version; unread) */
int hmt_Save(VFSFILE *f);                                                        /* 0x4adb20 */
extern char ch_EmptyName[];                                                      /* 0x631090, "" */
#endif
