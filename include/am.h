/* Declarations of am.c, the AI "methods" unit 0x48e070..0x4a7140 (its parts below, each marked with the header it was). 0x490c10..0x492fd0: the registration of three AI actions (0x490c10) and the
   decision callbacks of the AI decision table at 0x611258 (29 AIDEC records, gv.h, the callback at +0x20).
   ai.h is included for what it has; AIPER is our view of a character (0x21c bytes, ai_Chars at 0x770b80).
   Names are ours. */
#ifndef AM_H
#define AM_H
struct SHOTREC;

struct AMTREC;
struct D3OBJECT;
struct D3VECTOR;
struct AMTCAND;
#include "ai.h"
#include "vfs.h"
struct AIARG;
struct AIBLD;
struct AIC4;
struct AIPER;
struct AIDEC;
struct AMTKIND;
struct G5CH;
struct G9REC;

#pragma pack(push, 1)
typedef struct AIPER {                  /* a character, 0x21c bytes (ai_Chars[768]) */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002 */
    char pad003[0x1];
    int netId;                         /* 0x004 */
    char pad008[0x2];
    unsigned short age;                 /* 0x00a, the age in years (children above 11, below 40; 0x4fc7a0 projects the age at death from it) */
    unsigned char faith;                /* 0x00c, 0 / 1, the character's faith class (ai.h AICHAR.b00c;
                                           gm_ShiftLoyalty flips it, ai_DecideStateReligion reads it) */
    unsigned char rank;                  /* 0x00d, the rank (ai.h AICHAR.b00d: gv_CheckRankUp sends it as the new rank) */
    char pad00e[0xe];
    float unk01c;                       /* 0x01c */
    char pad020[0x60];
    unsigned char skills[8];            /* 0x080, the five skill bytes (0..252; ord.c ORDCHAR stats, ord_RatingBytes their maxima) */
    float mood[14][3];                  /* 0x088, (a, b, value) */
    unsigned char sel;                  /* 0x130 */
    char pad131[0x33];
    unsigned char jobId;               /* 0x164 */
    char pad165[0x1];
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none (ai.h AICHAR) */
    unsigned char unk167;               /* 0x167 */
    unsigned char appliedOffice;        /* 0x168, the office kind applied for, 0 = none (ai.h AICHAR) */
    char pad169[0x7];
    AIBLD *bld2;                        /* 0x170 */
    char pad174[4];
    int inventory;                      /* 0x178, the goods inventory (AICHAR.inventory, a G8 object list) */
    char pad17c[0x18];
    int money;                         /* 0x194 */
    char pad198[0x30];
    unsigned int flags1c8;              /* 0x1c8 */
    unsigned int flags1cc;              /* 0x1cc */
    float loyalty;                      /* 0x1d0, 0 .. 1, the character's loyalty (gm_ShiftLoyalty sends it as field
                                           0x1d0, gm_HandOutMoney moves it; ai_DecideStateReligion reads it) */
    char pad1d4[0x14];
    unsigned int flags1e8;              /* 0x1e8 */
    char pad1ec[0x30];
} AIPER;

#include "am_types.h"                /* G7GRP, AIACTW, AIACT */
#pragma pack(pop)

typedef char ai_AssertAIPER[sizeof(AIPER) == 0x21c ? 1 : -1];
typedef char ai_AssertAIACT[sizeof(AIACT) == 0x94 ? 1 : -1];

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

/* the same with the destination named in the asm (a local or `offset name`): no pointer temporary */
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


int ai_RegisterGraveActions(int a);                                        /* 0x490c10 */
int ai_DecideLawSeverity(AIPER *p, int a, int *out);                    /* 0x490e00, decision 0 */
int ai_DecideLaw1(AIPER *p, int a, int *out);                    /* 0x490fc0, decision 1 */
int ai_DecideStateReligion(AIPER *p, int a, int *out);                    /* 0x491200, decision 2 */
int ai_DecideLaw3(AIPER *p, int a, int *out);                    /* 0x4912e0, decision 3 */
int ai_DecideLaw4(AIPER *p, int a, int *out);                    /* 0x491430, decision 4 */
int ai_DecideUpkeepRate(AIPER *p, int a, int *out);                    /* 0x491560, decision 5 */
int ai_DecideBuildingCost(AIPER *p, int a, int *out);                    /* 0x4915f0, decision 6 */
int ai_DecideLaw7(AIPER *p, int a, int *out);                    /* 0x491650, decision 7 */
int ai_Unk4916d0(AIPER *p, int a, int *out);                    /* 0x4916d0, decision 9 */
int ai_Unk491860(AIPER *p, int a, int *out);                    /* 0x491860, decision 10 */
int ai_Unk491a40(AIPER *p, int a, int *out);                    /* 0x491a40, decision 11 */
int ai_Unk491b20(AIPER *p, int a, int *out);                    /* 0x491b20, decision 12 */
int ai_Unk491bf0(AIPER *p, int a, int *out);                    /* 0x491bf0, decision 13 */
int ai_Unk491cb0(AIPER *p, int a, int *out);                    /* 0x491cb0, decision 14 */
int ai_Unk491d80(AIPER *p, int a, int *out);                    /* 0x491d80, decision 15 */
int ai_Unk491e80(AIPER *p, int a, int *out);                    /* 0x491e80, decision 16 */
int ai_Unk491fb0(AIPER *p, int a, int *out);                    /* 0x491fb0, decision 17 */
int ai_Unk4920c0(AIPER *p, int a, int *out);                    /* 0x4920c0, decision 18 */
int ai_Unk4921d0(AIPER *p, int a, int *out);                    /* 0x4921d0, decision 19 */
int ai_Unk4922e0(AIPER *p, int a, int *out);                    /* 0x4922e0, decision 20 */
int ai_Unk492410(AIPER *p, int a, int *out);                    /* 0x492410, decision 21 */
int ai_Unk492540(AIPER *p, int a, int *out);                    /* 0x492540, decision 22 */
int ai_Unk492680(AIPER *p, int a, int *out);                    /* 0x492680, decision 23 */
int ai_Unk4927c0(AIPER *p, int a, int *out);                    /* 0x4927c0, decision 24 */
int ai_Unk492900(AIPER *p, int a, int *out);                    /* 0x492900, decision 25 */
int ai_Unk492a40(AIPER *p, int a, int *out);                    /* 0x492a40, decision 26 */
int ai_Unk492b80(AIPER *p, int a, int *out);                    /* 0x492b80, decision 8 */
int ai_Unk492d50(AIPER *p, int a, int *out);                    /* 0x492d50, decision 27 */
int ai_Unk492e90(AIPER *p, int a, int *out);                    /* 0x492e90, decision 28 */

int am_RegisterMethod(AIACT *act, int a);                            /* 0x495520, registers an action */
int ai_EvalMoodWeights(float *a, float *b, int i, AIPER *p, int f, void *x, void *y);  /* 0x4a49a0 */
char * amt_GetOfficeHolder(int k);                            /* 0x4e7510 */
int amt_GetOwnedOfficePay(void);                                         /* 0x4ea990 */
unsigned char ord_GetRatingByte(int k);                              /* 0x4f97a0 */
float ord_BuildingCapacityRatio(void);                                       /* 0x4f97c0 */
int gm_TitleGroup(int k);                                        /* 0x504ad0 */
float gm_GetSkill(char *c, int skill);                            /* 0x504ec0 */
int gm_SumListAmounts(void *list, unsigned char p);                       /* 0x509e30 */
unsigned char * gm_FirstBuilding(int n);                                     /* 0x50b010 */


/* 0x495ee0..0x497490 (was unk_495ee0.h) */
typedef struct AIARG {                  /* 0x18 bytes, what a decide callback fills for its execute callback */
    unsigned char kind;                 /* 0x00 */
    char pad01[3];
    int ref;                            /* 0x04, what the kind refers to: a building id, an item, a character's netId (kind 7) */
    unsigned int arg;                   /* 0x08, a second value of the kind: a count (kind 7 at 0x4a8ad0: 1..5), -1 = none */
    int ref2;                           /* 0x0c, a second reference: a character's netId (the helper at 0x4ab3e0, the sender msg.fromId) */
    int arg2;                           /* 0x10, a third value of the kind, passed on in the action's message (msg.arg[3] at 0x48ff40): a count (0x4a9030: graves), a step */
    float fvalue;                       /* 0x14, a float value of the kind: the share 0..1 of a money amount to use (0x4a.. callbacks scale by it), a weight factor, a count */
} AIARG;


typedef struct AIJOBX {                 /* a job (0x53e400 / 0x53e300 list) */
    char pad00[4];
    int unk04;                          /* 0x04 */
    char pad08[0xa4];
    int unkac;                          /* 0xac, a building id */
    union {
        int unkb0;                      /* 0xb0 */
        int price;                      /* 0xb0, of a building offer (kind 0x1b, +0xac the building) the asking price (0x497fa0 sets it against two thirds of the buyer's money) */
    };
    int unkb4;                          /* 0xb4 */
    int unkb8;                          /* 0xb8 */
    int unkbc;                          /* 0xbc */
} AIJOBX;

#pragma pack(push, 1)
typedef struct AIXMSG {                 /* 0xf8 bytes, an AIMSG (ai.h) as these callbacks fill it */
    int unk00;                          /* 0x00 */
    unsigned char type;                 /* 0x04 */
    char pad05[3];
    int to;                             /* 0x08, an AIMSG's receiving player netId */
    int fromId;                         /* 0x0c, an AIMSG's sending building id */
    char pad10[0x26];
    unsigned char flags;                /* 0x36, an AIMSG's message flags */
    char pad37[0x23];
    unsigned char unk5a;                /* 0x5a */
    char pad5b[0x9d];
} AIXMSG;
#pragma pack(pop)

typedef char ai_AssertAIXMSG[sizeof(AIXMSG) == 0xf8 ? 1 : -1];

extern AIACT ai_Actions[];            /* 0xf83ec0, the registered actions */
#define AI_PARTNER ((AIPER *)ai_Partner)  /* ai.h declares it AICHAR * */

void ai_Unk495ee0(void);                                                        /* 0x495ee0 */
int ai_DecideDummyRighteousness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x495fa0 */
int ai_ExecuteDummyRighteousness(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x495fc0 */
int ai_DecideDummySafety(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x495fd0 */
int ai_ExecuteDummySafety(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x495ff0 */
int ai_DecideDummyProcreation(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496000 */
int ai_ExecuteDummyProcreation(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x496020 */
int ai_DecideDummyHealth(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496030 */
int ai_DecideDummyJob(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496050 */
int ai_ExecuteDummyJob(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x496070 */
int ai_DecideDummyPleasure(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496080 */
int ai_ExecuteDummyPleasure(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x4960a0 */
int ai_DecideTrainSkill(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x4960b0 */
int ai_ExecuteTrainSkill(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x4965b0 */
int ai_DecideTrainJobSkill(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496610 */
int ai_ExecuteTrainJobSkill(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x496ab0 */
int ai_EvalLeastLikedOfEvent(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2); /* 0x496b10 */
int ai_DecideBribe(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x496e40 */
int ai_ExecuteBribe(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x4972d0 */
int ai_EvalApplyForOffice(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2); /* 0x4973f0 */

int ai_ChooseSubAction(int kind, int id, int c, AIARG *s1, AIARG *s2);                /* 0x495a50 */
int ai_EvalGuiltWeights(float *a, float *b, int i, AIPER *p, struct AIDEC *d);                /* 0x4a4c10 */
int ai_PickPartnerChain(AIPER *p, int a, int *flags, float b, float f, unsigned short *id); /* 0x4a58d0 */
void cm_SendSetField(int a, int b, int c, int d, int e);                   /* 0x4c4a60 */
void cm_SendMoneyDelta(int a, int b);                                               /* 0x4c5f60 */
int amt_GetOfficeApplicants(int k, int n, char **out);                                   /* 0x4e76b0 */
int ord_DispatchOrder(int id, char *c, int *m);                                     /* 0x4f81c0 */
#define gm_FindPer(id) ((AIPER *)gm_FindChar(id))  /* ai.h: returns AICHAR * */
int gm_CharCurrency(char *c, char p);                                    /* 0x509ee0 */
int gm_GetWorth(unsigned short id);                                            /* 0x50a640 */
/* ai.h declares he_FindEvent with five arguments; 0x496b10 calls it with three */
#define AI_53E400_3(a, b, k) ((AIJOBX *(*)(int, int, int, ...))he_FindEvent)(a, b, k)

/* 0x497660..0x49a0a0 (round 22, hv4): more action callbacks */
#pragma pack(push, 1)
typedef struct AIBTX {                  /* 0x24d bytes, an AIBTYPE (ai.h) as these callbacks read it */
    unsigned char kind;                 /* 0x000 */
    char pad001[0x20];
    unsigned char unk21;                /* 0x021, a limit */
    unsigned char n;                    /* 0x022, entries in k */
    unsigned short k[64];               /* 0x023, goods (low 15 bits) */
    struct {
        unsigned short a;               /* 0x0a3 */
        unsigned short b;               /* 0x0a5 */
    } p[64];                            /* 0x0a3, per entry of k: two more goods */
    char pad1a3[0xa4];
    unsigned char unk247;               /* 0x247, a count (level) */
    unsigned char unk248;               /* 0x248, its maximum */
    char pad249[4];
} AIBTX;

typedef struct AIBLDX {                 /* 0xa9 bytes, an AIBLD (ai.h) as these callbacks read it */
    unsigned char type;                 /* 0x00 */
    int id;                             /* 0x01 */
    char pad05[0x20];
    unsigned short masterId;            /* 0x25, a character id (the building's master) */
    unsigned short owner;               /* 0x27 */
    char pad29[0x31];
    unsigned char flags5a;              /* 0x5a */
    char pad5b;
    signed char level;                  /* 0x5c */
    int objList;                        /* 0x5d, an AIBLD's object-instance list */
    struct D3OBJECT *object;            /* 0x61, its 3D object in scene 0 (ai.h AIBLD.object) */
    char pad65[0x44];
} AIBLDX;
#define AI_BX(b) ((AIBLDX *)(b))        /* an AIBLD * (incomplete here) read as its AIBLDX view */

typedef struct AIFINX {                 /* what gm_PlayerRecord finds for a character (ai.h AIFIN) */
    char pad00[0x44];
    int renovationSpent;                /* 0x44 */
} AIFINX;

typedef struct AIGOODX {                /* 0x41 bytes, an AIGOOD (ai.h) as these callbacks read it */
    char type;                          /* 0x00 */
    char pad01[0x3f];
    unsigned char unk40;                /* 0x40 */
} AIGOODX;
#pragma pack(pop)

typedef char ai_AssertAIBTX[sizeof(AIBTX) == 0x24d ? 1 : -1];
typedef char ai_AssertAIBLDX[sizeof(AIBLDX) == 0xa9 ? 1 : -1];
typedef char ai_AssertAIGOODX[sizeof(AIGOODX) == 0x41 ? 1 : -1];

#ifndef DATA_VIEWS_H
extern float ai_Unkf84c40[8];           /* 0xf84c40, four (value, pad) pairs */
#endif
#ifndef DATA_VIEWS_H
extern float ai_Unkf84c60[8];           /* 0xf84c60, the same scaled by 0x499a90 */
#endif
#ifndef DATA_VIEWS_H
extern float ai_Unkf84cd4[8];           /* 0xf84cd4, four (value, pad) pairs */
#endif
#ifndef DATA_VIEWS_H
extern float ai_Unkf84cf4[8];           /* 0xf84cf4, the same scaled by 0x499f20 */
#endif

#define AI_BTX ((AIBTX *)ai_BTypes)
#define AI_GOODX ((AIGOODX *)ai_Goods)
#define AI_BLDX ((AIBLDX *)ai_Blds)
/* ai.h declares gm_BuildQuery with two arguments, am.h above with three */
#define AI_501190_2(a, b) ((AIBLDX *(*)(int, int))gm_BuildQuery)(a, b)
#define AI_BUILDNEXT() ((AIBLDX *)gm_BuildNext())
#define AI_501190_3(a, b, c) ((AIBLDX *)gm_BuildQuery(a, b, c))
#define AI_JOBS1B() ((AIJOBX *)he_FindEvent(1, 0, 0x1b))
#define AI_JOBNEXT() ((AIJOBX *)he_NextEvent())

int ai_DecideProduceGood(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x497660 */
int ai_ExecuteProduceGood(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x4979d0 */
int ai_DecideBuyRoom(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x497af0 */
int ai_ExecuteBuyRoom(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x497c80 */
int ai_DecideRepairBuilding(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x497cf0 */
int ai_ExecuteRepairBuilding(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x497ee0 */
int ai_DecideBuyBuilding(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x497fa0 */
int ai_DecideBuildResidence(int a, struct AIPER *p, int f, struct AIARG *o1, struct AIARG *o2);                 /* 0x4984d0 */
int ai_DecideBuildBusiness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x498c70 */
int ai_ExecuteBuildBusiness(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x499290 */
int ai_DecideUpgradeBusiness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x499400 */
int ai_DecideVisitMedicus(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x499520 */
int ai_ExecuteVisitMedicus(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x499580 */
int ai_EvalSabotage(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2); /* 0x4995d0 */
int ai_DecideSabotage(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x499660 */
int ai_ExecuteSabotage(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x499a90 */
int ai_EvalBeatUp(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2); /* 0x499b80 */
int ai_DecideBeatUp(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x499c10 */
int ai_ExecuteBeatUp(AIPER *p, AIARG *s1, AIARG *s2);                               /* 0x499f20 */
int ai_DecideCitizenship(int a, AIPER *p, int b, AIARG *o1, AIARG *o2);                 /* 0x49a020 */

int ai_DecideTakeCredit(int a, struct G5CH *p, int f, struct AIARG *o1, struct AIARG *o2);             /* 0x49eec0 */
int ai_RepairCost(struct AIBLD *b, int k);                                           /* 0x4a4e30 */
int ai_CanBuildType(int a, struct AIPER *p, int b, struct AIARG *s1, struct AIARG *s2, int f);         /* 0x4a5140 */
int ai_ExecBuildBuilding(struct AIPER *p, struct AIARG *s1, struct AIARG *s2);                               /* 0x4a5320 */
int ai_FindGoodToMake(int *item, int *obj, AIBLD *bld, int f);                       /* 0x4a5540 */
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);    /* 0x4a63f0 */
void gm_SellBuilding(int a, int b, int c, int d, int e);                     /* 0x502fd0 */
int gm_JobBuildingType(int k);                                                        /* 0x504050 */
int gm_FirstTitleNumber(int k);                                                        /* 0x504290 */
/* 0x5044b0 */
int gm_BuildingJob(int k);                                                        /* 0x5048e0 */
int gm_TitlesLeft(int k);                                                        /* 0x504ce0 */
float gm_Mix(int a, int b, int c);                                  /* 0x504e20 */
int gm_BuildingKindValue(int k);                                                        /* 0x509d30 */
int gm_BuildingKindCost(int c, int kind);                                              /* 0x509dc0 */
int ord_RandomObjectId(char *c);                                                     /* 0x4f8700 */
unsigned short *gm_ObjectOwnerChar(unsigned short *o);                                                  /* 0x509ff0 */

/* ---- 0x48e070..0x490b90 (was unk_48e070.h) ---- */
/* unk_490830.h (the AGCH / AGARG views of 0x490830..0x490b90); it calls ai_ExecGraveyardOwner0 .. ai_ExecGraveyardOwner3 and cm_SendMoveGoodsUnchecked through the prototypes below (AIPX / AIBLD, the definitions') */
/* ---- was unk_490830.h ---- */
#pragma pack(push, 1)
typedef struct AGCH {                   /* a character, 0x21c bytes */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002 */
    char pad003;
    unsigned int netId;                /* 0x004, the id */
    unsigned char unk008;               /* 0x008 */
    unsigned char sex;                  /* 0x009, 0 male / 1 female (0x492fd0 picks the first-name list by it) */
    unsigned short age;                 /* 0x00a, the age in years (children above 11, below 40; 0x4fc7a0 projects the age at death from it) */
    char pad00c[0x24];
    char name[0x20];                    /* 0x030 */
    unsigned short w050;                /* 0x050, the grave slot word (AICHAR.w050, gv_BuryCharacter compares it); 0x7fff = no grave */
    char pad052[0xa];
    int spouse;                         /* 0x05c, the netId of relatives[0], the spouse (ai_char.h AICHAR.relatives) */
    int unk060;                         /* 0x060 */
    int unk064;                         /* 0x064 */
    int list[5];                        /* 0x068 */
    char pad07c[4];
    unsigned char stat[8];              /* 0x080 */
    char pad088[0xa8];
    unsigned char sel;                  /* 0x130 */
    char pad131[0x35];
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none (ai.h AICHAR) */
    char pad167[0x2d];
    int money;                         /* 0x194 */
    char pad198[0x30];
    unsigned int flags1c8;              /* 0x1c8 */
    char pad1cc[0x50];
} AGCH;

typedef struct AGARG {                  /* what a decide callback fills for the execute callback */
    int unk00;                          /* 0x00 */
    int entry;                          /* 0x04, a grave entry number (gv_FindEntry's result) */
    int price;                          /* 0x08, the move / lease price the decision checked */
    int bldId;                          /* 0x0c, the building's id, found again with gm_FindById (ai_ExecGraveyardOwner, ai_ExecHandleGrave) */
    int kind;                           /* 0x10 */
} AGARG;

typedef struct AGDUEL {                 /* a dice duel between two characters (0x493200) */
    int unk00;                          /* 0x00 */
    int unk04;                          /* 0x04 */
    int phase;                          /* 0x08 */
    AGCH *side1;                        /* 0x0c */
    unsigned char n1;                   /* 0x10 */
    char pad11[3];
    unsigned int stats1;                /* 0x14, an AI opponent's classes (ai_MakeDuelOpponent: second << 8 | first), 0xffff for a player's side */
    unsigned char t1[0x10];             /* 0x18 */
    int kind1;                          /* 0x28 */
    int unk2c;                          /* 0x2c */
    AGCH *side2;                        /* 0x30 */
    unsigned char n2;                   /* 0x34 */
    char pad35[3];
    unsigned int stats2;                /* 0x38, as stats1 for side2 */
    unsigned char t2[0x10];             /* 0x3c */
    int kind2;                          /* 0x4c */
    int unk50;                          /* 0x50 */
    int total;                          /* 0x54 */
} AGDUEL;
#pragma pack(pop)

typedef struct AGFIN {                  /* what 0x506430 finds for a character */
    char pad00[0x58];
    float unk58;                        /* 0x58 */
} AGFIN;

typedef char ai_AssertAGCH[sizeof(AGCH) == 0x21c ? 1 : -1];
typedef char ai_AssertAGDUEL[sizeof(AGDUEL) == 0x58 ? 1 : -1];

extern AGCH ai_DuelOpponent;               /* 0x6f0478 */



int ai_ExecGraveyardOwner(AGCH *p, AGARG *s1, AGARG *s2);                /* 0x490830 */
int ai_DecideHandleGrave(int a, AGCH *p, int b, AGARG *o1, AGARG *o2);  /* 0x4908d0 */
int ai_ExecHandleGrave(AGCH *p, AGARG *s1, AGARG *s2);                /* 0x490b90 */
unsigned int ai_MakeDuelOpponent(unsigned int k);                      /* 0x492fd0 */
int ai_Unk493200(AGCH *p, AGDUEL *d, int k);                    /* 0x493200 */
int ai_Unk493390(AGDUEL *d, AGCH *c, int k);                    /* 0x493390 */
int ai_Unk4933d0(AGDUEL *d, AGCH *c, int k);                    /* 0x4933d0 */
void ai_DuelThrow(AGDUEL *d, AGCH *c, int k);                   /* 0x493420 */
int ai_Unk4937a0(AGDUEL *d, AGCH *c, int k);                    /* 0x4937a0 */
int ai_DuelOpponentAction(AGDUEL *d, AGCH *c);                           /* 0x493890 */
void ai_DuelNextPhase(AGDUEL *d);                                   /* 0x493a40 */
int ai_ChooseObjectTarget(AGCH *p, AIOBJ *o);                            /* 0x493be0 */
AGCH *ai_Unk493ea0(AGCH *p);                                    /* 0x493ea0 */

int gm_CharCurrency(char *c, char p);                     /* 0x509ee0 */
int gm_GetWorth(unsigned short id);                            /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k);                       /* 0x5085b0 */
int gm_PlayerFactor(int a, unsigned char k);                       /* 0x5085f0 */
float gm_GetSkill(char *c, int skill);                             /* 0x504ec0 */
int amt_GetKind(int k, struct AMTKIND *out);                             /* 0x4e6f80 */
int amt_GetColumnOffices(int col, int max, struct AMTREC *out);             /* 0x4e6fc0 */
/* ai.h declares these with other types */
#define AG_CH(id) ((AGCH *)gm_FindChar(id))

#pragma pack(push, 1)
typedef struct AIPX {                   /* a character, 0x21c bytes (ai_Chars[768]) */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002 */
    char pad003;
    int netId;                         /* 0x004 */
    unsigned char health;               /* 0x008, > 0 = alive (ai_char.h AICHAR.health) */
    char pad009[4];
    unsigned char rank;                  /* 0x00d, the rank (ai.h AICHAR.b00d: gv_CheckRankUp sends it as the new rank) */
    char pad00e[0x15e];
    union { AIBLD *bld; AIBLD *bld16c; };   /* 0x16c */
    union { AIBLD *bld2; AIBLD *bld170; };  /* 0x170 */
    char pad174[4];
    int inventory;                      /* 0x178 */
    char pad17c[4];
    signed char jobKind;                /* 0x180, the kind of the job (ai_char.h AICHAR.jobKind); the grave jobs 0x88,
                                           0x8a, 0x90 are negative as signed chars here */
    char pad181[0x13];
    int money;                         /* 0x194 */
    char pad198[0x34];
    unsigned int flags1cc;              /* 0x1cc */
    char pad1d0[0x4c];
} AIPX;

typedef struct AIGDX {                  /* what 0x5ba620 returns */
    char pad00[0xc];
    float unk0c;                        /* 0x0c */
} AIGDX;

typedef struct AIRECX {                  /* 16 bytes, filled by 0x4a5760 */
    int item;                           /* 0x0, the wanted item */
    int value;                          /* 0x4, the inventory entry's value when usable, -1 unusable, 0 not found */
    int id;                             /* 0x8, the inventory entry's id, -1 not found */
    int entry;                          /* 0xc, the inventory entry */
} AIRECX;

typedef struct AIUSE {                  /* 20 bytes, given to 0x5cf8a0 */
    AIOBJ *obj;                         /* 0x00 */
    int target;                         /* 0x04, the target for the good's use callback (the table +0x14): a reference of the action's second argument */
    int arg;                            /* 0x08, a value for the use callback, -1 none */
    int effect;                         /* 0x0c, set by the callee: 1 the use took effect (0x175 only one time in three), 0 not; the use callback may set -1 (backfired) */
    int handled;                        /* 0x10, 0 by the callee, set by a use callback that reported the use itself (no text then) */
} AIUSE;
#pragma pack(pop)

typedef char ai_AssertAIPX[sizeof(AIPX) == 0x21c ? 1 : -1];

#define AIC ((AIPX *)ai_Chars)
/* ai.h declares gm_FindById for another use; here it fills an int and takes three more */
#define AI_4FEA50_4(a, b, c, d) ((int (*)(int *, int, int, int))gm_FindById)(a, b, c, d)
#define AI_501190_5(a, b, c, d, e) ((AIBLD *(*)(int, int, int, int, int))gm_BuildQuery)(a, b, c, d, e)

typedef AIARG AIARGX;                   /* 0x48e070..0x48f250 spell it so */


extern int ai_PlayerCountYear;                /* 0x5ffd1c, the game day the count below was made on (-1 at start) */
extern int ai_NumPlayerChars;                /* 0x6f0470, number of characters of kind 5, 6, 7 */

int ai_AimCannon(struct AIPER *p, struct AIBLD *b, struct AIBLD *t);                        /* 0x4a67a0 */
int ai_FireCannon(struct AIPER *p, struct AIBLD *b);                                    /* 0x4a6ae0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);                 /* 0x4c4830 */
int amt_GetKindLevel(int v);                                                 /* 0x4e7c40 */
void gm_SendMoneyDelta(int a, int b);                                        /* 0x50cc50 */

/* this unit, in address order */
int ai_FindMasterBuilding(AIPX *p, int *out);                                     /* 0x48e070 */
int ai_DecideBuildDomizil(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2);         /* 0x48e120 */
int ai_ExecBuildDomizil(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48e370 */
int ai_DecideSecretRoom(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2);         /* 0x48e470 */
int ai_DecideSecretRoom0(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48e620 */
int ai_DecideSecretRoom1(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48e730 */
int ai_DecideSecretRoom2(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48e830 */
int ai_DecideSecretRoom3(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48e9f0 */
int ai_ExecSecretRoom(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48eab0 */
int ai_ExecSecretRoom0(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48eb50 */
int ai_ExecSecretRoom1(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48ec90 */
int ai_ExecSecretRoom2(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48edc0 */
int ai_ExecSecretRoom3(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48ef10 */
int ai_DecideCannonTower(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2);         /* 0x48f010 */
int ai_ExecCannonTower(AIPX *p, AIARGX *s1, AIARGX *s2);                       /* 0x48f1e0 */

extern unsigned short ai_PlayerId;     /* 0x6f25b8 */
#ifndef DATA_VIEWS_H
extern float ai_Unkf86894;              /* 0xf86894 */
extern float ai_Unkf868b4;              /* 0xf868b4 */
#endif

int ai_FindOwnBuilding(AIPX *p, AIBLD **out);                                         /* 0x48f250 */
int ai_DecideBuildSculpture(int a, AIPX *p, int b, AIARG *o1, AIARG *o2);                  /* 0x48f2b0 */
int ai_ExecBuildSculpture(AIPX *p, AIARG *s1, AIARG *s2);                                /* 0x48f500 */
int ai_DecideUseAlchemyItems(int a, AIPX *p, int b, AIARG *o1, AIARG *o2);                  /* 0x48f600 */
int ai_ExecUseAlchemyItems(AIPX *p, AIARG *s1, AIARG *s2);                                /* 0x48f990 */
int ai_RegisterBuildingActions(int a);                                                        /* 0x48fb70 */
AIPX *ai_PickRandomBuildingChar(AIBLD *b);                                                   /* 0x48feb0 */
int ai_DecideGraveyardOwner0(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2);                      /* 0x48ff50 */
int ai_ExecGraveyardOwner0(AIPX *p, AIBLD *b, int unused, AIARG *o);                      /* 0x4900c0 */
int ai_DecideGraveyardOwner1(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2);                      /* 0x490170 */
int ai_ExecGraveyardOwner1(AIPX *p, AIBLD *b, int unused, AIARG *o);                      /* 0x4902b0 */
int ai_DecideGraveyardOwner2(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2);                      /* 0x490350 */
int ai_ExecGraveyardOwner2(AIPX *p, AIBLD *b, int unused, AIARG *o);                      /* 0x4904c0 */
int ai_DecideGraveyardOwner3(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2);                      /* 0x490570 */
int ai_ExecGraveyardOwner3(AIPX *p, AIBLD *b, int unused, AIARG *o);                      /* 0x490600 */
int ai_DecideGraveyardOwner(int a, AIPX *p, int b, AIARG *o1, AIARG *o2);                  /* 0x490690 */

/* callees outside this file */
int ai_CanBuildType(int a, struct AIPER *p, int b, struct AIARG *s1, struct AIARG *s2, int f);           /* 0x4a5140 */
int ai_ExecBuildBuilding(struct AIPER *p, struct AIARG *s1, struct AIARG *s2);                                /* 0x4a5320 */
void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);                         /* 0x4a5760 */
/* am.h declares 0x4a63f0 with an __int64 mask; 0x48ff50..0x490350 pass it as two ints */
/* am.h: id is an int; 0x490690 passes an unsigned short */
#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)
int ai_DecideTakeCredit(int a, struct G5CH *p, int f, struct AIARG *o1, struct AIARG *o2);                  /* 0x49eec0 */
int gm_BuildingKindValue(int a);                                                        /* 0x509d30 */
int gm_BuildingKindCost(int c, int kind);                                               /* 0x509dc0 */
int ai_FindUsableGood(int param_1);                                         /* 0x5ba620 */

/* ---- 0x494020..0x494e70 (was unk_494020.h) ---- */
#pragma pack(push, 1)
typedef struct AIC4 {                   /* 0x21c bytes, ai_Chars[] with the fields these functions use */
    unsigned short id;                  /* 0x000 */
    char pad002[2];
    unsigned int netId;                /* 0x004 */
    char pad008[5];
    unsigned char rank;                  /* 0x00d, the rank (ai.h AICHAR.b00d: gv_CheckRankUp sends it as the new rank) */
    char pad00e[0xe];
    float unk01c;                       /* 0x01c */
    char pad020[4];
    int unk024;                         /* 0x024 */
    char pad028[0xe4];
    float unk10c;                       /* 0x10c */
    char pad110[0x56];
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none (ai.h AICHAR) */
    char pad167[0xa9];
    int partner;                        /* 0x210 */
    char pad214[8];
} AIC4;

typedef struct AIGRP {                  /* a group: a leader and up to four members (0x494ac0) */
    AIC4 *leader;                       /* 0x00 */
    struct {
        AIC4 *ch;                       /* 0x04 */
        int d;                          /* 0x08 */
    } mem[4];
    unsigned int unk24;                 /* 0x24, 0..5 */
    unsigned int unk28;                 /* 0x28, 0..5 */
    int unk2c;                          /* 0x2c, result */
    unsigned char n;                    /* 0x30, member count, 1..4 */
    char pad31[3];
} AIGRP;
#pragma pack(pop)

typedef struct AIREC10 {                /* 16 bytes, the table at 0x5ecfd8 (7 records) */
    int key;                            /* 0x0 */
    int unk04;                          /* 0x4 */
    float unk08;                        /* 0x8 */
    float unk0c;                        /* 0xc */
} AIREC10;

typedef struct AIREC2F4 {               /* 0x2f4 bytes, the table at 0x7e4274 (3 records) */
    int a;                              /* 0x000 */
    int b;                              /* 0x004 */
    char pad008[0x2ec];
} AIREC2F4;

extern const AIREC10 ai_Unk5ecfd8[7];         /* 0x5ecfd8 */

#define AI_C4(id) ((AIC4 *)gm_FindChar(id))

double cm_RandomFloat(void);                                       /* 0x4e5360, a random value */
unsigned int cm_Random(unsigned int n);                                        /* 0x4e52f0, a random test */
float gm_MapDistance(int a, int b, int c, int d);                 /* 0x4ffc90 */
float gm_Mix(int a, int b, int c);                    /* 0x504e20 */
int gm_ConvertMoney(int a, unsigned char b);                       /* 0x5085b0 */
int gm_GetWorth(unsigned short id);                            /* 0x50a640 */
int ord_DispatchOrder(int id, char *c, int *m);           /* 0x4f81c0 */

int ai_Unk494020(AIPLAN *q);                                    /* 0x494020 */
int ai_Unk4941b0(void);                                         /* 0x4941b0 */
int ai_Unk4941d0(void);                                         /* 0x4941d0 */
int ai_Unk4941e0(AIPLAN *q);                                    /* 0x4941e0 */
int ai_PlanLevel(AIPLAN *q);                                    /* 0x4942a0 */
int ai_PlanRollTries(AIPLAN *q);                                    /* 0x494300 */
int ai_Unk494410(AIC4 *ch, AIPLAN *q);                          /* 0x494410 */
int ai_PlanRank(AIPLAN *q);                                    /* 0x4945c0 */
int ai_Unk4946c0(AIC4 *ch, AIPLAN *q);                          /* 0x4946c0 */
int ai_WealthRating(AIC4 *a, AIC4 *b);                             /* 0x4947a0 */
int ai_WealthRatingHalf(AIC4 *a, AIC4 *b);                             /* 0x494830 */
AIREC10 *ai_Unk4948c0(int key);                                 /* 0x4948c0 */
void ai_Unk4948f0(struct AIC4 *ch, int *out);                          /* 0x4948f0 */
int ai_Unk494a30(AIC4 *a, AIC4 *b);                             /* 0x494a30 */
int ai_Unk494ac0(AIGRP *g, int send);                           /* 0x494ac0 */
int ai_Unk494d40(AIC4 *a, AIC4 *b, int n);                      /* 0x494d40 */
int ai_Unk494d90(AIC4 *a, AIC4 *b, int *out);                   /* 0x494d90 */

/* ---- 0x494e70..0x4984d0 (was unk_494e70.h) ---- */
#pragma pack(push, 1)
typedef struct G5CH {                   /* a character, 0x21c bytes (ai_Chars[768] at 0x770b80) */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002, (AIPER.kind) */
    char pad003;
    unsigned int netId;                /* 0x004 */
    char pad008;
    unsigned char sex;                  /* 0x009, 0 male / 1 female */
    unsigned short age;                 /* 0x00a, the age in years (children above 11, below 40; 0x4fc7a0 projects the age at death from it) */
    char pad00c;
    unsigned char rank;                  /* 0x00d, the rank (ai.h AICHAR.b00d: gv_CheckRankUp sends it as the new rank) */
    char pad00e[0x1e];
    unsigned int ailments;              /* 0x02c, bit fields of counters (4,4,4,2,3,3,3,2,4 bits, the dump 0x4e5980) the
                                           daily update 0x4fbe90 runs down; reasoned: the character's ailments, as
                                           "UseObject (Unversehrtheit)" wants the good 0x17e while one is set */
    char pad030[0x2c];
    int relatives[8];                   /* 0x05c, the netIds of the character's relatives (ai.h AICHAR), -1 none */
    char pad07c[0xc];
    float mood[14][3];                  /* 0x088, (a, b, value) */
    unsigned char sel;                  /* 0x130, the current need */
    unsigned char prev;                 /* 0x131, the previous action */
    unsigned char act;                  /* 0x132, the current action */
    char pad133;
    AIARG arg1;                         /* 0x134 */
    AIARG arg2;                         /* 0x14c */
    char pad164[2];
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none */
    unsigned char prevOffice;           /* 0x167, the office kind held before the current one (ai.h AICHAR) */
    unsigned char appliedOffice;        /* 0x168, the office kind applied for, 0 = none (ai.h AICHAR) */
    unsigned char secondOffice;         /* 0x169, the second office kind (ai.h AICHAR): 0x1e .. 0x21 the mayor's powers */
    char pad16a[0x6];
    AIBLDX *bld2;                       /* 0x170, its house (AIPER.bld2) */
    char pad174[4];
    int inventory;                      /* 0x178, the goods inventory (AICHAR.inventory, a G8 object list) */
    char pad17c[0x18];
    int money;                          /* 0x194, (AIPER.money) */
    char pad198[0x30];
    unsigned int flags1c8;              /* 0x1c8, (AIPER.flags1c8): 0x2000000 keeps it from the regulars' table (ai_DecideStammtisch) */
    char pad1cc[0x1c];
    unsigned int flags1e8;              /* 0x1e8, (AIPER.flags1e8) */
    unsigned int goodsMask;             /* 0x1ec, (AICHAR.unk1ec): a good whose Domizil entry mask (+0x10) shares a bit is not usable (0x4a5760); reasoned name */
    char pad1f0[0x20];
    int partner;                        /* 0x210 */
    char pad214[4];
    unsigned char mask;                 /* 0x218, the action groups it may use */
    char pad219[3];
} G5CH;

typedef struct G5STAMM {                /* the regulars' table, the object 0x12d of a kind-0x37 building (ai_DecideStammtisch) */
    char pad00[0x1c];
    int member[4];                      /* 0x1c, the members' netIds, -1 = a free seat */
} G5STAMM;

typedef struct G5ACT {                  /* 0x94 bytes, am.h's AIACT with the third weight block */
    unsigned char id;                   /* 0x00 */
    char name[0x23];                    /* 0x01 */
    int (*eval)(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2);  /* 0x24 */
    int (*decide)(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                    /* 0x28 */
    int (*exec)(G5CH *p, AIARG *s1, AIARG *s2);                                    /* 0x2c */
    AIACTW w1[4];                       /* 0x30, short-term needs */
    AIACTW w3[4];                       /* 0x50, a copy of w1 */
    AIACTW w2[4];                       /* 0x70, long-term needs */
    unsigned char flags;                /* 0x90, the action's groups */
    char pad91[3];
} G5ACT;
#pragma pack(pop)

typedef char ai_AssertG5CH[sizeof(G5CH) == 0x21c ? 1 : -1];
typedef char ai_AssertG5ACT[sizeof(G5ACT) == 0x94 ? 1 : -1];

#define G5_CHARS ((G5CH *)ai_Chars)
#define G5_ACTS ((G5ACT *)ai_Actions)

extern int ai_Unkf83ea4;                /* 0xf83ea4, the depth of the evaluation stack */
extern int ai_Unkf86900[];              /* 0xf86900, the actions being evaluated */
extern char ai_ActionIniPath[];             /* 0xf83da0, the ini file of the actions */
extern int ai_WriteActionTable;               /* 0x6f2464, set: write the action table */

int ai_LoadActionTable(void);                                         /* 0x494e70 */
/* 0x495520 am_RegisterMethod is declared in am.h */
void ai_PushAction(int k);                                       /* 0x495770 */
void ai_PopAction(void);                                        /* 0x495790 */
int ai_ActionOnStack(int k);                                        /* 0x4957a0 */
/* ai_ExecuteAction: ai.h */                                         /* 0x495ce0 */
int ai_DecideApplyForOffice(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);  /* 0x497490 */
int ai_ExecuteApplyForOffice(G5CH *p, AIARG *s1);                            /* 0x497630 */
int amt_ApplyForOffice(char *c, unsigned int kind);                                /* 0x4e6180 */
int ai_ExecuteBuyBuilding(G5CH *p, AIARG *s1);                            /* 0x498470 */

int ai_GetDesireByName(char *s);                                      /* 0x4a47c0, a need name -> index */
void ai_BeginNeedBlock(char *p);                                     /* 0x4c3e70 */
int ai_AddNeedDelta(int k, float v);                    /* 0x4c3eb0 */
void ai_SendNeedBlock(void);                                        /* 0x4c4a20 */
int gm_ExpectedDeathAge(char *c);                                      /* 0x4fc7a0 */
int amt_GetOfficeCandidates(char *c, int n, struct AMTCAND *out);                     /* 0x4e7150 */
extern void *main_Instance;              /* 0x75e140, the module handle */
int ai_RegisterBuildingActions(int a);                                                        /* 0x48fb70 */
int ai_RegisterBaseActions(int a);                                                        /* 0x4a16f0 */

/* ---- small functions 0x4984d0..0x4a3af0 (was unk_4984d0.h) ---- */
typedef struct G6RING {                 /* 12 bytes, the ring at 0xf83c20 (ai_FillNearRing fills it) */
    G5CH *who;                          /* 0x0 */
    float dist;                         /* 0x4 */
    unsigned int flags;                 /* 0x8, why the character is listed */
} G6RING;

extern G6RING ai_NearRing[];           /* 0xf83c20 */
typedef struct G6CURSOR {               /* 8 bytes, the walk over ai_NearRing */
    G6RING *next;                       /* 0x0, the entry ai_NextNearRingEntry returns next */
    int step;                           /* 0x4, the stride (coprime to the count, ai_FillNearRing) */
} G6CURSOR;
extern G6CURSOR ai_NearRingCursorState;          /* 0x60034c */
extern G6CURSOR *const ai_NearRingCursor;        /* 0x5ed1b0, points to ai_NearRingCursorState */
extern int ai_NearRingCount;                /* 0x6f070c, the number of entries */

int ai_ExecuteDarkCorner(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49ee20 */
G5CH *ai_PickLeastLikedRival(G5CH *p);                                                    /* 0x4a0240 */
G6RING *ai_NextNearRingEntry(void);                                                     /* 0x4a3a90 */
int ai_PickDislikedColumn(G5CH *p);                                                      /* 0x4a0410 */
int ai_DecidePriceManipulation(int a, G5CH *p, int f, AIARG *o1);                             /* 0x49fc90 */
int amt_MayorBonusStep(char *c);                                                      /* 0x4e94a0 */
int ai_DecideBuildResidence(int a, struct AIPER *p, int f, struct AIARG *o1, struct AIARG *o2);                 /* 0x4984d0 */
int ai_CanBuildType(int a, struct AIPER *p, int b, struct AIARG *s1, struct AIARG *s2, int f);           /* 0x4a5140 */
int gm_BuildingKindCost(int c, int kind);                                               /* 0x509dc0 */
int gm_BuildingKindValue(int k);                                                        /* 0x509d30 */
int ai_DecideTakeCredit(int a, struct G5CH *p, int f, struct AIARG *o1, struct AIARG *o2);                  /* 0x49eec0 */
#define G6_509EE0(p, k) ((float (*)(AIPER *, unsigned char))gm_CharCurrency)((AIPER *)(p), (k))  /* am.h: returns int */
int ai_ExecuteBuildResidence(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x498a80 */
int ai_DecideUpgradeResidence(int a, AIPER *p, int f, AIARG *o1, AIARG *o2);                 /* 0x498b80 */
int amt_GetColumnOfficesOwned(int col, int max, struct AMTREC *out);                                      /* 0x4e7040 */

int amt_GetKind(int k, struct AMTKIND *out);                             /* 0x4e6f80 */
int amt_FindOffice(char *c, struct AMTKIND *kind, struct AMTREC *office);                                    /* 0x4e6030 */

/* ---- 0x49a0a0..0x4a16f0 (was unk_49a0a0.h) ---- */
#pragma pack(push, 1)
typedef struct G8MSG {                  /* 0xf8 bytes, an AIMSG (ai.h) as these callbacks fill it */
    int unk00;                          /* 0x00 */
    unsigned char type;                 /* 0x04 */
    char pad05[3];
    unsigned int to;                    /* 0x08 */
    int unk0c;                          /* 0x0c */
    char pad10[0x18];
    AITIME time;                        /* 0x28 */
    unsigned char unk36;                /* 0x36 */
    char pad37[0x21];
    unsigned char unk58;                /* 0x58 */
    char pad59[3];
    unsigned int from;                  /* 0x5c */
    int unk60;                          /* 0x60 */
    int unk64;                          /* 0x64 */
    char pad68[4];
    int unk6c;                          /* 0x6c */
    char pad70[0x88];
} G8MSG;
#pragma pack(pop)

typedef char ai_AssertG8MSG[sizeof(G8MSG) == 0xf8 ? 1 : -1];

typedef struct G8USE {                  /* 20 bytes, given to 0x5cf8a0 (unk_48e070.h: AIUSE) */
    AIOBJ *obj;                         /* 0x00 */
    int target;                         /* 0x04, the target for the good's use callback (the table +0x14): a reference of the action's second argument */
    int arg;                            /* 0x08, a value for the use callback, -1 none */
    int effect;                         /* 0x0c, set by the callee: 1 the use took effect (0x175 only one time in three), 0 not; the use callback may set -1 (backfired) */
    int handled;                        /* 0x10, 0 by the callee, set by a use callback that reported the use itself (no text then) */
} G8USE;

typedef struct G8OFFER {                /* 12 bytes, filled by 0x50a250 */
    int a;                              /* 0x0 */
    int b;                              /* 0x4 */
    int c;                              /* 0x8 */
} G8OFFER;

typedef struct G8TABLE {                /* 12 bytes, a regulars' table request (0x4c5d80) */
    unsigned int tag;                   /* 0x0, 'join' / 'leav' */
    unsigned int ch;                    /* 0x4 */
    int bld;                            /* 0x8 */
} G8TABLE;

typedef struct G8REQ {                  /* 20 bytes, given to 0x4e9660 */
    int kind;                           /* 0x00 */
    int a;                              /* 0x04 */
    int b;                              /* 0x08 */
    int unk0c;                          /* 0x0c */
    int unk10;                          /* 0x10 */
} G8REQ;

typedef struct G8SEAT {                 /* 24 bytes, filled by 0x4e7040 */
    int unk00;                          /* 0x00 */
    int id;                             /* 0x04, a character id or -1 */
    int unk08[4];                       /* 0x08 */
} G8SEAT;

typedef struct G8REC {                  /* 16 bytes, filled by 0x4a5760 (unk_48e070.h: AIRECX) */
    int item;                           /* 0x0 */
    int unk04;                          /* 0x4 */
    int unk08;                          /* 0x8 */
    int unk0c;                          /* 0xc */
} G8REC;


int ai_ExecuteCitizenship(G5CH *p);                                                      /* 0x49a0a0 */
int ai_DecideSaveAps(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49a290 */
int ai_ExecuteSaveAps(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49a380 */
int ai_DecideUseMilitaryObject(int a, AIPER *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49a390 */
/* unk_48e070.h declares ai_FindWantedItems with its AIPX / AIRECX views (unify at the join) */
void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);                           /* 0x4a5760 */
#define G8_4FEA50(a, b, c, d) ((int (*)(int *, int, int, int))gm_FindById)(a, b, c, d)  /* ai.h: other types */
int ai_ExecuteUseMilitaryObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49a5b0 */
/* 0x5ba620 with an int id (the callers here zero-extend; unk_48e070.h: unsigned short) */
#define ai_FindUsableGoodP(id) ((void *(*)(int))ai_FindUsableGood)(id)
int ai_DecideUseReputationObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49a690 */
int ai_ExecuteUseReputationObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49aa80 */
int ai_DecideUseApsObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49ab60 */
int ai_ExecuteUseApsObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49ad80 */
int ai_DecideUseEducationObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49ae50 */
int ai_ExecuteUseEducationObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49b050 */
int ai_DecideUseHealthObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49b120 */
int ai_ExecuteUseHealthObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49b2b0 */
int ai_DecideUseDynastyObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49b380 */
int ai_ExecuteUseDynastyObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49b6f0 */
int ai_DecideUseMeanObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49b7c0 */
int ai_ExecuteUseMeanObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49bc50 */
int ai_DecideUseSafetyObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49bd90 */
int ai_DecideUsePerfume(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49c170 */
int ai_DecideUseTrank(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49c3c0 */
int ai_ExecuteUseSafetyObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49c000 */
int ai_EvalUsePerfume(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49c0d0 */
int ai_EvalUseTrank(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49c320 */
int ai_DecideSpy(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49c570 */
int ai_ExecuteSpy(G5CH *p, AIARG *s1);                                            /* 0x49c700 */
int ai_ExecuteUsePerfume(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49c190 */
int ai_ExecuteUseTrank(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49c3e0 */
int ai_DecideBuyMarketObject(int a, G5CH *p, int f, AIARG *s1, AIARG *o);                   /* 0x49c790 */
int ai_ExecuteBuyMarketObject(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49c880 */
int ai_DecideBuyIndulgence(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49c8e0 */
int ai_ExecuteBuyIndulgence(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49cb60 */
int ai_DecideUseOffice(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49cd30 */
int ai_ExecuteUseOffice(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49cdb0 */
int ai_EvalThreaten(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49cdd0 */
int ai_DecideThreaten(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49ce50 */
int ai_ExecuteThreaten(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d030 */
int ai_EvalPamphlets(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49d050 */
int ai_DecidePamphlets(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d0f0 */
int ai_ExecutePamphlets(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d280 */
int ai_DecideDenounce(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d2d0 */
int ai_ExecuteDenounce(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d460 */
int ai_EvalInsult(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49d4b0 */
int ai_DecideInsult(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d550 */
int ai_ExecuteInsult(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d5e0 */
int ai_EvalBlackmail(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49d600 */
int ai_DecideBlackmail(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d6a0 */
int ai_ExecuteBlackmail(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d730 */
int ai_DecideUsePrivileges(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d750 */
int ai_ExecuteUsePrivileges(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49d7d0 */
float ord_GetStatistics(int *o);                                                   /* 0x4f9e00 */
int ai_EvalCurrentNeed(float *a, float *b, int i, AIPER *p);                          /* 0x4a4ae0, as in unk_4a3af0.h */
#define AI_EvalCurrentNeed7(a, b, i, p, f, o1, o2) ((int (*)(float *, float *, int, G5CH *, int, AIARG *, AIARG *))ai_EvalCurrentNeed)(a, b, i, p, f, o1, o2)  /* declared with 4 parameters */
int ai_EvalMayor(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49d7f0 */
int ai_DecideMayor(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49d8d0 */
char * gm_PlayerSeat(int p);                                                      /* 0x504a80 */
int ai_ExecuteMayor(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49dc10 */
int ai_DecideMayorUpgrade(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49dd90 */
int ai_DecideMayorTownHall(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49def0 */
int ai_ExecuteMayorTownHall(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49e230 */
int ai_DecideMayorPrison(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49e350 */
int ai_ExecuteMayorPrison(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49e680 */
int ai_DecideMasterBuilder(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49e7a0 */
int ai_ExecuteMasterBuilder(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49ea90 */
int ai_EvalDarkCorner(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49ec10 */
int ai_DecideDarkCorner(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49ed10 */
extern const int ai_RandomStrides[16];                                                   /* 0x5ed140, random steps */
int gm_GetCreditOffers(int b, unsigned short *p, unsigned cnt, int *out);                       /* 0x50a250 */
int ai_DecideTakeCredit(int a, struct G5CH *p, int f, struct AIARG *o1, struct AIARG *o2);                  /* 0x49eec0 */
int ai_ExecuteTakeCredit(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49f370 */
int ai_EvalChangeProfession(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x49f4f0 */
int gm_TitleNumber(int r, int n);                                                 /* 0x5043d0 */
int ai_DecideChangeProfession(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49f5c0 */
void cm_SendSetJob(int id, int job);                                      /* 0x4c59b0 */
void gm_SendMoneyDelta(int a, int b);                                      /* 0x50cc50, unk_48e070.h: int id */
int ai_ExecuteChangeProfession(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49f7f0 */
int ai_DecideStammtisch(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x49f850 */
void cm_SendTableRequest(int *p);                                                  /* 0x4c5d80 */
int ai_ExecuteStammtisch(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x49fb80 */
int amt_ExecutePriceMessage(char *c, int *m);                                           /* 0x4e9660 */
int ai_ExecutePriceManipulation(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a0040 */
int amt_CanUseMayorPower2(char *c);                                                      /* 0x4e94e0 */
int ai_DecideGuildRiot(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x4a0090 */
int amt_ExecuteShareMessage(short *c, int *m);                                           /* 0x4e9a00 */
int ai_ExecuteGuildRiot(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a01f0 */
int ai_Unk4a05e0(G5CH *p);                                                      /* 0x4a05e0 */
int amt_CanUseMayorPower3(char *c);                                                      /* 0x4e95a0 */
int ai_DecideGuildLevel3(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x4a0890 */
int amt_ExecuteMayorPowerMessage(char *c, int *m);                                           /* 0x4e9ce0 */
int ai_ExecuteGuildLevel3(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a0940 */
int ai_EvalUpgradeGuild(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x4a0990 */
int ai_DecideUpgradeGuild(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x4a0a70 */
int ai_ExecuteUpgradeGuild(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a0e60 */
int ai_EvalApprenticeship(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2); /* 0x4a1000 */
int ai_DecideApprenticeship(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x4a1170 */
int ai_ExecuteApprenticeship(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a1360 */
void ai_Unk4948f0(struct AIC4 *ch, int *out);                                         /* 0x4948f0, unk_494020.h: AIC4 * */
#define G8_4948F0(p, r) ai_Unk4948f0((p), (int *)(r))
int ai_DecideStudy(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);                  /* 0x4a13e0 */
int ai_ExecuteStudy(G5CH *p, AIARG *s1, AIARG *s2);                                /* 0x4a1640 */
int ai_SeanceDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);    /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_SeanceExec(G5CH *p, AIARG *s1, AIARG *s2);                   /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_BuyObjectApDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);  /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_BuyObjectApExec(G5CH *p, AIARG *s1, AIARG *s2);  /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_UpgradeBusinessApDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2);  /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_UpgradeBusinessApExec(G5CH *p, AIARG *s1, AIARG *s2);  /* unlinked: LINK folds it (/OPT:ICF) into sv_ReturnZero */
int ai_RegisterBaseActions(int a);                                                        /* 0x4a16f0 */
#define G8_4C4830(a, b, k, d) ((void (*)(int, unsigned int, unsigned char, int))cm_SendMoveGoodsUnchecked)(a, b, k, d)  /* above: c is int */
/* 0x4c4830 with an int c (unk_48e070.h: unsigned char) */
#define G8_4C4830I(a, b, c, d) ((void (*)(int, unsigned int, int, int))cm_SendMoveGoodsUnchecked)(a, b, c, d)

/* ---- 0x4a3af0..0x4a7140 (was unk_4a3af0.h) ---- */
#define amt_GetKindLevel ai_Unk4e7c40_g9      /* k is an unsigned char here */

typedef struct G7MKT {                  /* 0xb4 bytes, the prices of an item group (0x4a4680) */
    unsigned int key;                   /* 0x00 */
    int n;                              /* 0x04 */
    int items[10];                      /* 0x08 */
    int base[10];                       /* 0x30 */
    int price[10];                      /* 0x58 */
    float ratio[10];                    /* 0x80 */
    float avg;                          /* 0xa8 */
    int hi;                             /* 0xac */
    int lo;                             /* 0xb0 */
} G7MKT;

extern G7GRP gm_BuildingItemGroups[];            /* 0x61a348 */

int ai_GroupPrices(G7MKT *m);
int ai_RepairCost(struct AIBLD *b, int k);                                              /* 0x4a4e30 */
int ai_PickBuildSite(AIPER *p, int name);                                           /* 0x4a4e70 */
extern int d3_CurrentScene;             /* 0x633444 */
char d3if_SelectScene(int index, char keep);                                        /* 0x4478e0 */
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                              /* 0x458a20 */                                                     /* 0x4a4680 */

#define G7_MASK(p) (((G5CH *)(p))->mask)

int amt_GetKindLevel(int k);                                              /* 0x4e7c40 */
extern const int ai_RingStrides[11];          /* 0x5ed180, the strides of the near ring cursor */
int ai_EvalCurrentNeed(float *a, float *b, int i, AIPER *p);                          /* 0x4a4ae0 */

int gm_MayTakeJob(int k, int c);                                      /* 0x5020e0 */
/* 0x4a5140 ai_CanBuildType is declared in am.h */
extern char ai_DataPath[];                                                      /* 0x75dca0, the data path */
/* 0x4a5320 ai_ExecBuildBuilding is declared in am.h */
typedef struct G9REC {                  /* 16 bytes, filled by 0x4a5760 (unk_49a0a0.h G8REC, unk_48e070.h AIRECX) */
    int item;                           /* 0x0, the wanted item */
    int value;                          /* 0x4, the inventory entry's value at +0x0e when usable, else -1 */
    int entryId;                        /* 0x8, the inventory entry's id (+0x02), -1 none */
    int entry;                          /* 0xc, the inventory entry itself */
} G9REC;
void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);                          /* 0x4a5760, declared int elsewhere */
/* 0x4a58d0 ai_PickPartnerChain is declared in am.h */
int ai_PickPartnerInRange(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5a30 */
extern const int ai_CharWalkSteps[16];                                                    /* 0x5ed100, random steps */
int ai_FindPartnersRandomWalk(AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id); /* 0x4a5b50, unk_490830.h: other view */
int ai_PickPartnerRandomWalk(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
unsigned int ai_FindCandidates(AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out); /* 0x4a5ef0 */
unsigned int ai_PickPartnerChainLevel(AIPER *p, unsigned int a, unsigned int fl, float lo, float hi, int d, unsigned short *id); /* 0x4a61d0 */
extern const int ai_RandomStrides[16];                                                    /* 0x5ed140, random steps */
/* 0x4a63f0 ai_PickActionBuilding is declared in am.h */
int ai_PickActionBuildings(AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out); /* 0x4a6570 */
void d3t_GetObjectBounds(struct D3VECTOR *mn, struct D3VECTOR *mx, struct D3OBJECT *obj);             /* 0x413150, d3_tools.c */
int ai_BuildingAimPoints(AIBLD *x, float *a, AIBLD *y, float *b);                        /* 0x4a66f0 */
typedef struct G9SHOT {                 /* 0x74 bytes, the AI's cannon shot at 0x6f0698 (0x4a67a0) */
    int who;                            /* 0x00, the shooter's id */
    int bld;                            /* 0x04, the cannon building's id */
    float a[4];                         /* 0x08, start */
    float b[4];                         /* 0x18 */
    float d[4];                         /* 0x28 */
    float c[4];                         /* 0x38 */
    float ang;                          /* 0x48, the elevation */
    int charge;                         /* 0x4c */
    int k;                              /* 0x50, the elevation in percent of the range */
    int kind;                           /* 0x54, the cannon type */
    int par[5];                         /* 0x58, from 0x5dc1a0 */
    int cost;                           /* 0x6c */
    int x;                              /* 0x70 */
} G9SHOT;
typedef struct G9BALL {                 /* 12 bytes, the table at 0x5edfe0 (3 cannon types) */
    float a;                            /* 0x0, 2000 / 2450 / 3600 */
    float b;                            /* 0x4, 3000 / 3850 / 5950 */
    float c;                            /* 0x8, 0.06 / 0.065 / 0.075 */
} G9BALL;
typedef struct G9CANNON {               /* what 0x4eded0 finds for a building */
    char pad00[8];
    float charge[4];                    /* 0x08 */
} G9CANNON;
typedef struct G9KIND {                 /* filled by 0x4ef000 */
    int unk00;                          /* 0x0 */
    int level;                          /* 0x4 */
    int unk08[6];
} G9KIND;
extern G9SHOT ai_CannonShot;                                                      /* 0x6f0698 */
extern const G9BALL ai_CannonBalls[3];                                                   /* 0x5edfe0 */
extern const float ai_IronDensity;                                                       /* 0x5eee08, 7874 */
int ai_SphereVolume(float radius, float size, float *out);                                   /* 0x5dc1a0 */
struct SPHEREVOLUME; void ai_PositionShare(float a, float b, struct SPHEREVOLUME *rec, float *out);                        /* 0x5dc210 */
int ai_SolveCannonAim(float *a, float *b, float *rng, float *x, float *y, float *ang, float *vb, float *vc, float *vd); /* 0x5dd100 */
int ai_AimCannon(struct AIPER *p, struct AIBLD *b, struct AIBLD *t);                                  /* 0x4a67a0 */
int ai_InitShot(struct SHOTREC *shot, struct D3VECTOR *pos, struct D3VECTOR *dir, float charge, float size, float density); /* 0x5dc240 */
int ai_ScatterShot(struct SHOTREC *shot, float sideSpread, struct D3VECTOR *side, float otherSpread, struct D3VECTOR *other, float speedSpread);    /* 0x5dc3a0 */
int ai_FireCannon(struct AIPER *p, struct AIBLD *b);                                            /* 0x4a6ae0 */
#undef amt_GetKindLevel

/* declared in other units' headers before (moved to the owner) */
int ai_ChooseAction(int i);  /* 0x4957d0 */
int ai_ExecuteAction(int i);  /* 0x495ce0 */
void ai_FillNearRing(AICHAR *c);  /* 0x4a3af0 */

#endif
