/* Declarations of f3gm.c (0x4f9e30 .. 0x50d0b0): the game data part (clock, building / object tables and
   queries, character helpers, prices). The unit still declares most of its own functions in the .c; these are
   the ones other units call. Names are ours. */
#ifndef F3GM_H
#define F3GM_H

#include "f3gm_types.h"

unsigned char *gm_BuildNext(void);              /* 0x501030, the next building of the query */
unsigned char *gm_BuildQuery(int n, ...);       /* 0x501190, n (kind, value) filters; 6 takes no value */
int gm_BuildingMainObject(unsigned char *bld);  /* 0x501b50, the main object of the building by its class (an AIOBJ, ai.c +0x2 objId) */
short *gm_FindChar(int id);                     /* 0x505df0, the character with the id, 0 when none */
int gm_FindById(int *bld, int *obj, int *chr, int id);  /* 0x4fea50, what has the id: 1 building, 2 object, 3 character */
int gm_FindObjectProto(char *name);             /* 0x4fe900, the number of the object prototype with the given name (case-insensitive), 0 when none */
int gm_FindObjectInst(int id);                  /* 0x4fe980, the object instance with the id (int +2) */
char *gm_FindBuildingById(int id);              /* 0x502020, the building with the id (+1) */
void gm_ResetStock(void);                       /* 0x50cc60, the stock of the type 0x1e buildings' stalls to 2 */
int gm_TitleNumber(int group, int rank);        /* 0x5043d0, the title number of the rank in the group */
int gm_TitleGroup(int n);                       /* 0x504ad0, the title group of the title n */
int gm_TitleJobGroup(int n);                    /* 0x504c00, the job group of a job-pair number n */
int gm_TitleJob(int i, int p);       /* 0x5044b0, the job byte of the character i in the title group p, 0 when none */
int gm_IsType10(unsigned char *b);              /* 0x5024f0, 1 for the town market */
int gm_IsHomeType(unsigned char *b);            /* 0x502520, 1 for a home type */
char *gm_FindCharacterByMaster(int m);          /* 0x50a810, the first kind 6 / 7 / 1 / 2 character with a job whose master (+0x16c) is m */
void gm_ShowSlotObjects(int unused, int scene, int c);  /* 0x502a60, the scene objects of the character's five slots on */
void gm_AssignBuilding(unsigned char *b, unsigned short place, unsigned own);  /* 0x502b60 */

void gm_ClearBuildings(void);                   /* 0x4fe7f0, clears the 0x100 records of ai_Blds */
float gm_Mix2(float u, float v);                /* 0x504e80 */
void gm_ClearObjects(void);                     /* 0x4fe830, returns the instances of live owners and clears the 0x5008-record table */

int gm_LoadTables(char *root);                                       /* 0x4fe570 */
void unk_LoadIconNames(void);                                         /* 0x4fde90 */
void gm_ResetCharacters(void);                                        /* 0x503e80 */
void gm_FreeIconNames(void);                                          /* 0x4fdfb0 */
void gm_Free(void);                                                   /* 0x4fe9c0 */
int gm_TitlesLeft(int n);                                             /* 0x504ce0 */
int gm_CollectCarried(int id, int *out);                                   /* 0x50af00 */
int gm_IsBankrupt(unsigned short *p, int n);                                  /* 0x50a6c0, p: an AICHAR */

/* declared in other units' headers before (moved to the owner) */
struct ORDCHAR; int gm_CatchUpperDisease(struct ORDCHAR *c);  /* 0x5056a0, the character catches one of the upper diseases 5 .. 8; the kind or 0 */
struct ORDCHAR; int gm_CureDisease(struct ORDCHAR *c);  /* 0x505840, cures one disease of the character; the kind or 0 */
struct ORDCHAR; int gm_CureMinorDisease(struct ORDCHAR *c);  /* 0x5059d0, cures one of the minor diseases 2 .. 5 of the character; the kind or 0 */
struct ORDCHAR; int gm_CureUpperDisease(struct ORDCHAR *c);  /* 0x505ad0, cures one of the diseases 6, 7, 8, 3 of the character; the kind or 0 */
int gm_CurseTargetCount(char *b);  /* 0x50d0b0 */
int gm_Season(char *d);  /* 0x4fe330 */
struct ORDCHAR; int gm_CatchMinorDisease(struct ORDCHAR *c);  /* 0x5054f0, the character catches one of the minor diseases 2 .. 5; the kind or 0 */
int gm_HeadIconNumber(char *name);  /* 0x4fb770 */
int gm_FindRichest(short *id, int *rate);  /* 0x4fad60 */
void gm_Shuffle(unsigned n, int *out);  /* 0x505d30 */
int gm_SexForJob(int x);  /* 0x503f30 */
extern GMBLOCK gm_StockBlocks[];             /* 0x7d6a60, the five stock blocks (one per player slot) */
extern unsigned short gm_KindObjCount;  /* 0x76fdc4 */
extern unsigned gm_KindObjs[732];  /* 0x889400 */
int gm_BuildKindObjects(char *b);  /* 0x500b90 */
int gm_BathObjectCount(char *b);  /* 0x50cf60 */
int gm_CurseObjectCount(char *b);  /* 0x50cfd0 */
int gm_ObjectBuilding(int o);  /* 0x50a0f0 */
int gm_TakeableCount(char *c, int proto, int n);  /* 0x50ad70 */
void gm_ChangeBuildingState(unsigned char *b, int d);  /* 0x50cad0 */
int gm_ListWorkers(unsigned char *b, int *out);  /* 0x502eb0, the worker lists of the building b (out[1 ..], out[0x19 ..]); returns their number */
int gm_TitleNumberGroup(int n);  /* 0x504350, the group of the title number n */
int gm_HasBuilding(int p);  /* 0x5049c0 */
void gm_EndDayIncomes(unsigned flags);  /* 0x4fabe0, the end of the day for the income histories */
void gm_DailyCharacters(void);  /* 0x4fc920, the daily step of all characters */
void gm_UpdatePlayers(void);  /* 0x4feb30 */
void gm_CharacterUpkeep(void);  /* 0x4fa580, the daily upkeep of the characters */
void gm_PayAllCharacterWages(void);  /* 0x4fa9b0 */
void gm_UpdateWorthMood(void);  /* 0x4faa00 */
float gm_JobWage(char *c, int k, int a, int b);  /* 0x508740, the wage of a job (a) or title (b) of the building kind k for the character c */
int gm_BuildingBalance(unsigned char *b, unsigned id, int *out);  /* 0x508d80, the income / cost summary of the building b for the owner id */
unsigned *gm_NameRecord(int kind, int id);  /* 0x4fb330, the name record (0x44 bytes, the name at +4) with the id in the table of the sex kind (0 / 1), or the first free one */
unsigned *gm_CharNameRecord(int id);  /* 0x4fb390 */
int gm_CreateCharModel(unsigned short *c, int scene, int *p, int *ori, int obj, char *dummy);  /* 0x4fbb80 */
float gm_HealthRatio(short *c);  /* 0x4fc8e0 */
struct ORDBLD; struct ORDBLD *gm_FindHome(int unused, struct ORDBLD *o);  /* 0x4fd820, a home with room for the building o (the nearest or least full) */
void gm_MoveIntoHome(char *c, char *b);  /* 0x4fda10 */
struct ORDCHAR; struct ORDBLD; struct ORDCHAR *gm_FindApplicant(struct ORDBLD *o, unsigned title, unsigned job);  /* 0x4fda70 */
char *gm_HomeObject(unsigned char *b);  /* 0x4fdc10 */
short *gm_SpawnCharacter(unsigned char flags, int p2, int p3, unsigned char p4, unsigned char p5, char *p6);  /* 0x4fddb0 */
void gm_CountCharacters(void);  /* 0x4fdfd0 */
unsigned short *gm_AllocRaum(unsigned char *b, unsigned n);  /* 0x5031b0 */
int gm_BuildingGoodsList17(char *b, unsigned *out, int kind);  /* 0x5099e0 */
extern char gm_CharacterStatistics[128];  /* 0x91b500 */
int gm_CharacterWages(unsigned short id, int flag, unsigned char *out);  /* 0x4fa700 */
int gm_TakeObjekt(int id, int proto, int n);  /* 0x500a40 */
void gm_FreeObject(int o);  /* 0x5034b0 */
int gm_SkillPoints(char *c, int skill);  /* 0x504e00 */
int gm_HasAdultRelative(char *c, int *n);  /* 0x506480 */
unsigned gm_TakeOverCharacter(int a, int b);  /* 0x5064d0 */
struct ORDCHAR; void gm_ForfeitEstate(struct ORDCHAR *c, char *out, int flag);  /* 0x506e70, the character c forfeits its estate after a lost fight; the announcement into out */
struct ORDBLD; unsigned short gm_CreateCharacter(int type, int a2, int a3, unsigned short a4, struct ORDBLD *a5, unsigned a6, int a7, int a8);  /* 0x507690 */
unsigned short *gm_ObjectBuildingOwner(unsigned short *o);  /* 0x50a050 */
int gm_ListObjectsAfter(unsigned char *b, unsigned short *proto, int *out);  /* 0x508cc0, counts (and with out lists) the objects b lists after proto */
int gm_RemoveOwned(int *list, int id);  /* 0x5000c0 */
void gm_SelectModelVariant(char *o);  /* 0x4fb5a0 */
void gm_SetJob(int i, unsigned n);  /* 0x504790 */
void gm_ShuffledBytes(unsigned char n, unsigned char *out);  /* 0x505be0 */
int gm_FreeBuilding(char *b);  /* 0x5012e0 */
int gm_RemoveObjectFromBuilding(int b, int id);  /* 0x503480 */
unsigned char *gm_CreateBuilding(unsigned char k, unsigned short own);  /* 0x501580 */
extern int gm_AllowAll;  /* 0x6f2448 */
float gm_AddHealth(unsigned short id, int d);  /* 0x4fca40 */
int gm_AddNegative(int id, int proto, int n);  /* 0x500ae0 */
float gm_CharacterHealth(short *c);  /* 0x4fc820 */
extern int ch_CurrentGroup;  /* 0x5f1294 */
float gm_GoodsValue(int p, unsigned id);  /* 0x4ffbf0 */
void gm_Shuffled(unsigned char n, int *out);  /* 0x505ca0 */
int gm_BuildingKindCost(int c, int kind);  /* 0x509dc0 */
int gm_SumListAmounts(void *list, unsigned char p);  /* 0x509e30 */
int gm_CharCurrency(char *c, char p);  /* 0x509ee0 */
unsigned short *gm_ObjectOwnerChar(unsigned short *o);  /* 0x509ff0 */
int gm_GetCreditOffers(int b, unsigned short *p, unsigned cnt, int *out);  /* 0x50a250 */
int gm_ExpectedDeathAge(char *c);  /* 0x4fc7a0 */
int gm_MayTakeJob(int k, int c);  /* 0x5020e0 */
void gm_SellBuilding(int a, int b, int c, int d, int e);  /* 0x502fd0 */
char *gm_PlayerSeat(int p);  /* 0x504a80 */
void gm_SendMoneyDelta(int a, int b);  /* 0x50cc50 */
int gm_JobBuildingType(int x);  /* 0x504050 */
float gm_MapDistance(int a, int b, int c, int d);  /* 0x4ffc90 */
float gm_Mix(int a, int b, int c);  /* 0x504e20 */
int gm_PlayerFactor(int a, unsigned char k);  /* 0x5085f0 */
unsigned char *gm_FirstBuilding(int n);  /* 0x50b010 */
int gm_FirstTitleNumber(int x);  /* 0x504290 */
float gm_GetSkill(char *c, int skill);  /* 0x504ec0 */
int gm_GetWorth(unsigned short id);  /* 0x50a640 */
int gm_BuildingKindValue(int kind);  /* 0x509d30 */
int gm_ConvertMoney(int a, unsigned char k);  /* 0x5085b0 */
int gm_BuildingJob(int t);  /* 0x5048e0 */
int gm_StallGoodsList(char *c, unsigned *out);  /* 0x50aa00 */
int gm_SumSlotAmounts(void *list, unsigned char p);  /* 0x508660 */
int gm_CharLevel(unsigned short id, int again);  /* 0x505e60, the level of the character id (its job's kind level + 1, raised to the parent's) */
int gm_CharPrevLevel(unsigned short id, int again);  /* 0x505f30, gm_CharLevel for the previous office (+0x167) */
struct ORDCHAR; struct GMDATE; void gm_BirthDate(struct ORDCHAR *c, struct GMDATE *out);  /* 0x506000, the birthday of the character c (its birth year, a month and day from its first name) */
struct ORDCHAR; struct GMDATE; int gm_DeathDate(struct ORDCHAR *c, struct GMDATE *out);  /* 0x5060d0, the day of death of the character c; 0 for a living one */
struct GMRANK; void gm_RankPlayers(int *count, struct GMRANK *out);  /* 0x506180, the ranking of the up to 8 players into out, *count of them */
float gm_JobCost(char *c, int k, int a, int b);  /* 0x50cd40 */
unsigned short *gm_QueryNext(void);  /* 0x4ffcc0 */
int gm_OwnerWorth(char *b, int flag);  /* 0x509c80 */
short *gm_PlayerRecord(char *c);  /* 0x506430 */
int gm_Opinion(int a, int b);  /* 0x50c520 */
extern int gm_NOffers;  /* 0x7e3f3c */
float gm_ObjectValue(int n);  /* 0x5086b0 */
int gm_ListStock(short *l, int id);  /* 0x50ab50 */
int gm_ListAmountById(char *l, int id);  /* 0x50ab20 */
int gm_BuildingClass(int kind);  /* 0x501d80 */
int gm_BuildingGoodsList(char *b, unsigned *out, int kind);  /* 0x5098a0 */
int gm_BuildingLevel(char *c);  /* 0x508d70 */
int gm_BuildingRank(unsigned char *b);  /* 0x5090b0 */
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag);  /* 0x5092e0 */
float gm_CharAge(unsigned short id);  /* 0x505e30, the age (+0x0a) of the character id */
float gm_StaffFactor(unsigned char *b, int n, int *ids);  /* 0x50cdc0, the staff factor of the building b with the persons ids[0 .. n - 1] */
int gm_CollectOffers(int goods);  /* 0x500c20 */
int gm_EquipmentValue(unsigned char *b, int level);  /* 0x509b10 */
char *gm_FindGoods(int p, unsigned id);  /* 0x4ffc40 */
int gm_FindMarketObject(int id);  /* 0x500ec0 */
int gm_FreeCapacity(short *l, int id, int n);  /* 0x50ab80 */
void *gm_GetGebaeudeHauptraum(unsigned char *b);  /* 0x501ce0 */
int gm_GetGoodsValue(char *c);  /* 0x509e60 */
float gm_GoodsPrice(unsigned id, unsigned char p);  /* 0x5089c0 */
float gm_ItemPrice(int item, unsigned char n);  /* 0x5087f0 */
float gm_Liking(unsigned a, unsigned b, int c);  /* 0x50c550 */
int gm_ListAmount(short *l, char *o);  /* 0x50aaf0 */
extern struct AIOBJ *ai_Objects;  /* 0x7e3f34 */
extern int ai_GameFlags;  /* 0x6f23f8 */
extern char * ai_Goods;  /* 0x91aaf0 */
int gm_RemoveObject(int *list, unsigned id);  /* 0x4ffff0 */
void gm_KillSpieler(int c, int mode);  /* 0x503c60 */
extern char * ai_Blds;  /* 0x889f74 */
extern int ai_Difficulty;  /* 0x6f23fc */
extern int ai_CharCount;  /* 0x6f1bc0 */
extern int ai_NextObjectId;  /* 0x61a654 */
int gm_AddObjekt(int id, int proto, int n);  /* 0x500970 */
void gm_ChooseHeadIcon(unsigned short *c);  /* 0x4fb890 */
int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5);  /* 0x4fbd10 */
char *gm_Query(void *list, int n, ...);  /* 0x4ffe90 */
void gm_RefreshGoodsLists(unsigned p);  /* 0x4ff190 */
JOBTALENTS gm_GetJobTalents(int job);  /* 0x503ee0, the record of the job (0x5f and above: the first) */

#endif
