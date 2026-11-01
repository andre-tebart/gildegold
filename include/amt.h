struct AMTKIND;
struct VFSFILE;
struct AIPER;
/* Declarations of amt.c (0x4e5cd0.., the offices; tags "amt_"). Names are ours. */
#ifndef AMT_H
#define AMT_H

/* copy of am.h's AI_ZERO (the headers do not compile together) */
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

#include "amt_types.h"               /* AMTREC */
extern AMTREC amt_Table[42];            /* 0x925120 */
typedef struct AMTKIND {                /* 0x18 bytes, a kind of office (the table 0x60ed58, 30 records) */
    int id;                             /* 0x00, the kind */
    int col;                            /* 0x04, the column of the rate table */
    unsigned char level;                /* 0x08 */
    char pad09[3];
    int salary;                         /* 0x0c */
    int two;                            /* 0x10, the office takes two persons */
    int pad14;
} AMTKIND;
extern AMTKIND amt_Kinds[40];           /* 0x60ed58 */
extern const float amt_Pay[];                 /* 0x5edfbc */
extern float amt_Rates[];               /* 0x60ec90, 7 floats per row */
/* views of the character record and of the kind record as amt_Rating reads them (type debt) */
typedef struct AMTCHARVIEW {            /* a character record amt.c reads by offset (0x21c bytes, as ai_Chars) */
    unsigned short id;                  /* 0x000, 0xffff when the record is not in use */
    unsigned char kind;                 /* 0x002, 6 / 7 the player characters, 0x13 the applicants the office events create */
    char pad003;
    int netId;                          /* 0x004, compared with the owner of an office */
    unsigned char health;               /* 0x008, > 0 = alive (ai.h AICHAR.health) */
    char pad009[0x15b];
    unsigned char rank;                 /* 0x164, the office rank: 1 .. 6 and 0x28 .. 0x2d are the deputy candidates */
    char pad165;
    unsigned char officeKind;           /* 0x166, the kind of the office held (0 = none) */
    unsigned char prevOffice;           /* 0x167, the office held before the current one */
    unsigned char appliedOffice;        /* 0x168, the office applied for (0 = none) */
    unsigned char secondOffice;         /* 0x169, the second office (the x14 / deputy) */
    char pad16a[2];
    void *object;                       /* 0x16c, the building record the character is at (gm_FindById of the object) */
    char pad170[0x24];
    int money;                          /* 0x194, the character's money (the office price is taken off it) */
    char pad198[0x30];
    unsigned int powerFlags;            /* 0x1c8, the office-power bits already used: 0x4000000 / 0x8000000 / 0x10000000 = the first /
                                           second / third bonus step (cleared with cm_SendSetField(cid, 0x1c8, 4, bit, 0)) */
    unsigned int status;                /* 0x1cc, the status bits (0x400 .. 0x2000 select the rate table columns) */
    char pad1d0[0x4c];
} AMTCHARVIEW;
#define AMT_STATUS(c) (((AMTCHARVIEW *)(c))->status)
#define AMT_NETID(c) (((AMTCHARVIEW *)(c))->netId)
#define AMT_KIND(c) (((AMTCHARVIEW *)(c))->kind)
#define AMT_POWER(c) (((AMTCHARVIEW *)(c))->powerFlags)
typedef struct AMTGOODS {              /* a 0x80-byte goods record (gm_FindGoods / the price table 0x4ffa10) */
    char pad000[0x20];
    float amount;                       /* 0x20, the amount (0x4ffa10 weighs it against the mean) */
    float mean;                         /* 0x24 */
    char pad028[0x10];
    float price;                        /* 0x38, the price 0x4ffa10 computes */
    char pad03c[0x44];
} AMTGOODS;
#pragma pack(push, 1)
typedef struct AMTBLDVIEW {             /* a building record as gm_BuildQuery (f3gm.c 0x501190) returns it: the AIBLD of ai.h, which amt.c includes now: the view can give way to AIBLD) */
    unsigned char type;                 /* 0x00, index into ai_BTypes */
    int id;                             /* 0x01, the building's own id (AIBLD.id) */
    char pad005[0x22];
    unsigned short owner;               /* 0x27, the character id (AIBLD.owner) */
    char pad029[0x31];
    unsigned char flags5a;              /* 0x5a, bit 0 = flagged (AIBLD.flags5a) */
} AMTBLDVIEW;
#pragma pack(pop)
typedef union AMTFLOATBITS { float f; int i; } AMTFLOATBITS;    /* a float read as its bit pattern */
#define AMT_BITS(p) (((AMTFLOATBITS *)(p))->i)
typedef struct AMTKINDF {               /* an AMTKIND's +0x14 dword read as a float */
    char pad014[0x14];
    float value;
} AMTKINDF;
int amt_FindOffice(char *c, struct AMTKIND *kind, struct AMTREC *office);  /* 0x4e6030 */
int amt_GetOfficeRate(char *c, int x, float *out);  /* 0x4e60b0 */

void amt_InitTable(void);               /* 0x4e5cd0 */
extern const float amt_TabA[];                /* 0x5edc90, 15 rows of 10 floats (id, 9 columns by the rank) */
extern const float amt_TabB[];                /* 0x5edef0, 5 rows of 10 */
float amt_Rating(char *a, char *b, int mode, int flag);  /* 0x4e5e50 */

#pragma pack(push, 1)
typedef struct AMTAPP {                 /* an application: the person, the office(s) and the kind (7 bytes) */
    int id;
    unsigned char o1, o2, kind;
} AMTAPP;
typedef struct AMTMSG {                 /* the message of 0x4e6680: 11 bytes, sent as the 15-byte G17B15 (its frame in 0x4e6680 reserves 16) */
    unsigned char office;
    int a;
    unsigned char d, e;
    int b;
    char pad[4];
} AMTMSG;
typedef struct AMTSET {                 /* the office message of 0x4e67a0 (12 bytes) */
    unsigned char idx;                  /* 0x00, the office (compared with AMTREC.idx as a byte) */
    int owner;                          /* 0x01, the new owner or -1 */
    unsigned char state;                /* 0x05, the new state */
    char code;                          /* 0x06, unused here */
    int x14;                            /* 0x07, the second character or -1 */
} AMTSET;
typedef struct AMTTRADE {               /* the swap message (10 bytes) */
    int id;
    unsigned char a, b;
    int v;
} AMTTRADE;
#pragma pack(pop)
typedef struct AMTCAND {                /* 12 bytes: a kind of office, its rate for a character, its level */
    int kind;
    float rate;
    unsigned char level;
    char pad09[3];
} AMTCAND;
typedef struct AMTRANGE {               /* 0x1c bytes, the ranges of 0x4e78f0 */
    short s0, s1;
    int i2, i3, i4, i5, i6, i7;
} AMTRANGE;
#pragma pack(push, 1)
typedef struct AMTRANGEW {               /* an AMTRANGE's first pair read as the two overlapping dwords amt_CreateApplicant (0x4e7aa0) rolls its age from */
    int word0;                           /* 0x0, s0 | s1 << 16 */
    int word1;                           /* 0x2, s1 | (the low half of i2) << 16 */
} AMTRANGEW;
#pragma pack(pop)
extern char ch_EmptyName[];             /* 0x631090 */
extern int d3_CurrentScene;                /* 0x633444 */
extern char ai_DataPath[];             /* 0x75dca0 */
extern char ai_Time[];             /* 0x770ae0 */
void err_Message(char *text);                     /* 0x4222b0 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
char * he_FindEvent(int n, ...);                     /* 0x53e400 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
void cm_SendMoneyDelta(int a, int b);                     /* 0x4c5f60 */
int sv_MessageBox(int text, unsigned flags); /* 0x521150, sv.c */
extern int amt_Flip;                    /* 0x60f118, 2 at the start */
extern unsigned char amt_PriceRows[];   /* 0x61a348, the guild-house price table: 37 rows of 0x15 bytes (the building kind, 10 goods ids) */
#define AMT_CHARS_END (&ai_CharInts[0x300][0])  /* 0x7d5f84, the word past the last character record */
int util_Random(unsigned short n);      /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);  /* 0x4c4830 */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
void cm_BeginFieldBlock(char *o, int id);     /* 0x4c3ba0 */
int cm_AddFieldDelta(int size, int n, short field, void *v);  /* 0x4c3c00 */
void cm_SendFieldDeltas(void);                /* 0x4c49a0 */
void cm_SendRelations(int a, int b, int c, int d);                     /* 0x4c4b00 */
int cm_GetSlotState(int h);                     /* 0x4c3ae0 */
/* ai_TimeHour (0x770ae4, the clock's hour) is no object: time_view.h views it in ai_Time + 4 */
int amt_ApplyForOffice(char *c, unsigned int kind);  /* 0x4e6180 */
int amt_CheckApplication(struct AMTAPP *m);           /* 0x4e6360 */
int amt_AcceptApplication(AMTAPP *m);           /* 0x4e6430 */
int amt_SendOfficeFree(char a);              /* 0x4e65e0 */
int amt_SendOfficeCandidates(char *c, char *x);    /* 0x4e6600 */
int amt_SendOfficeOwner(char a, char *b, unsigned char c);  /* 0x4e6650 */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);  /* 0x4e6680 */
int amt_CheckOfficeMessage(struct AMTMSG *m);  /* 0x4e6710 */

int amt_ExecuteOfficeMessage(AMTSET *p);           /* 0x4e67a0 */
void amt_SetEventOwner(int id);             /* 0x4e6b40 */
void amt_SetPersonOwner(int id);             /* 0x4e6b70 */
int amt_SendOfficeSwap(char *requester, char *b, char *c);  /* 0x4e6ba0 */
int amt_ExecuteSwapMessage(AMTTRADE *m);         /* 0x4e6c50 */
int amt_ClearOffices(char *c);             /* 0x4e6d80 */
void amt_ClearOfficesIfAll(char *c, int all);   /* 0x4e6e20 */
int amt_GetFirstOffice(unsigned int k, AMTREC *out);  /* 0x4e6ec0 */
int amt_KindColumn(int k);                /* 0x4e6f10 */
int amt_GetOffice(unsigned char i, AMTREC *out); /* 0x4e6f30, @original: cm.c 0x4dab50 and unk_4a7140.c 0x4aad30 pass their int index raw (push ebp / push edx, no zero-extension) where a caller compiled against this unsigned char parameter zero-extends; the original TUs were compiled against an int parameter */
int amt_GetKind(int k, struct AMTKIND *out);  /* 0x4e6f80 */
int amt_GetColumnOffices(int col, int max, AMTREC *out);  /* 0x4e6fc0 */

int amt_GetColumnOfficesOwned(int col, int max, struct AMTREC *out);  /* 0x4e7040 */
int amt_GetColumn7Offices(int all, int max, AMTREC *out);  /* 0x4e70f0 */
int amt_GetOfficeCandidates(char *c, int n, struct AMTCAND *out);   /* 0x4e7150 */
int amt_GetOfficeCandidatesFull(char *c, int n, AMTCAND *out, int *full);  /* 0x4e7320 */
char *amt_GetOfficeHolder(int k);       /* 0x4e7510 */
char *amt_GetSuperiorOfficeHolder(int k);             /* 0x4e7550 */
int amt_CanTakeOffice(char *c, int k);      /* 0x4e75c0 */
int amt_GetOfficeApplicants(int k, int n, char **out);  /* 0x4e76b0 */
int amt_GetEligibleOffices(char *c, int n, int *out);  /* 0x4e7770 */

int amt_GetOpenOffice(int k);           /* 0x4e78a0 */
int amt_GetCreateRanges(int k, AMTRANGE *r);  /* 0x4e78f0 */
int amt_CreateApplicant(int k);         /* 0x4e7aa0 */
int amt_InitApplicant(char *c, int k);      /* 0x4e7b60 */
int amt_GetKindLevel(int k);      /* 0x4e7c40, int not unsigned int: the original's callers use the result signed (f3init 0x50e5a0 divides it with sar, am.c 0x4a3af0 compares with jge) */
void amt_ElectOffices(int unused, int n, AMTREC *recs);  /* 0x4e7c60 */

int amt_CheckOfficeColumns(void);                /* 0x4e8160 */
int amt_IsColumnDecided(int col);       /* 0x4e83b0 */
int amt_Unk4e8530(int col, AMTREC *r);  /* 0x4e8530 */
void amt_UpdateOffices(void);           /* 0x4e85d0 */
int amt_HasColumn7Holder(void);         /* 0x4e86e0 */
void amt_SendOfficeNews(unsigned short *p, int v);  /* 0x4e8730 */
void amt_SendOfficeLost(unsigned short *p, int v);  /* 0x4e87e0 */
void amt_ElectDeputies(void);            /* 0x4e88a0 */
void amt_ElectKind22(void);            /* 0x4e8c20 */

void amt_CalcZuenfte(unsigned int kind);  /* 0x4e8dc0 */
void amt_CheckOffices(void);               /* 0x4e9260 */
int amt_HasTownObject(int k);               /* 0x4e9400 */
int amt_Unk4e94a0(char *c);             /* 0x4e94a0 */
int amt_CanUseMayorPower2(char *c);             /* 0x4e94e0 */
int amt_CanUseMayorPower3(char *c);             /* 0x4e95a0 */

int amt_ExecutePriceMessage(char *c, int *m);     /* 0x4e9660 */
int amt_ExecuteShareMessage(short *c, int *m);    /* 0x4e9a00 */

int amt_ExecuteMayorPowerMessage(char *c, int *m);  /* 0x4e9ce0 */

void amt_ChangeDistrictOpinion(char *c, int d, int v);  /* 0x4ea550 */
float amt_GetDistrictLiking(short *c, int d);  /* 0x4ea5c0 */
int amt_SaveAemter(void *f);            /* 0x4ea690 */
int amt_LoadAemter(void *f, int version);  /* 0x4ea7a0 */
void amt_TellOpenOffices(char *c);            /* 0x4ea8e0 */
void amt_Unk4eaa00(unsigned int k);     /* 0x4eaa00 */

int amt_GetOwnedOfficePay(void);        /* 0x4ea990 */

#endif
