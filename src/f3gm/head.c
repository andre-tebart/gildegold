/* The game data unit (0x4f9e30 .. 0x5085b0 and on): tag "f3_gm:lpGebaeudePROT" / "f3_gm:lpObjekteINST" (the allocation tags of the building and object tables loaded from A_Geb.dat / A_Obj.dat, 0x4fe570). One .data string block runs through it in function order (0x61774c "misc\statistics_taxes" .. the head icons "%s_HEAD%i_ICON.bmp" .. "Character_Names.dat" 0x617818 .. the tag strings 0x61a9e0 .. 0x61aa38 .. "buy building" 0x61ab78 .. "%1G %A" 0x61ac98), so the character income / statistics code (0x4f9e30 .. 0x4fe140), the game clock (0x4fe140 ..), the building and object tables, the job / title tables and the character helpers are one unit (joined layout 2026-10-23 from unk_4f9e30.c and f3gm.c). Drafts of layout 2026-10-23.
   unit of rea 8cef7d5b (src/game/f3gm.c); boundaries in decl/text.dcl, notes in src/f3gm/open.md */
struct CHARACTER;
struct CHTRANSPORT;
struct AICHAR;
extern char ai_PlayerSlots[][0x2f4];
extern char d2_Wins[];
extern char d2_Views[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "di_input.h"
#include "data_views.h"
extern char ai_Unk91f700[];
#define GAPDATA __declspec(align(1)) unsigned char
extern char ai_Unk7d6a60[];                 /* 0x7d6a60, the five 0x2a90-byte stock blocks (one per player slot) */
/* The stock area is ONE 0xd4d0-byte object at 0x7d6a60 (five blocks of 0x2a90: four header ints, then
   0x55 records of 0x80 bytes). The other names are views into it: a separate definition would reserve
   its own memory and the block writer/reader (gm_LoadTables, unk_Unk51f080, unk_Unk51b340) would then
   write over whatever the COMMON linker placed inside the 0xd4d0-byte span. */
#define init_StockBlocks (*(char (*)[0xd4c0])((char *)ai_Unk7d6a60 + 0x10))  /* 0x7d6a70, the records of block 0 */
#define ai_Unk7d6a64 (*(int (*)[3])((char *)ai_Unk7d6a60 + 4))  /* 0x7d6a64, the rest of block 0's header */
/* 0x7d6a90 .. 0x7d6aa8 are views into stock block 0 (the record fields the price code reads), not
   separate COMMON symbols: they start inside init_StockBlocks[0] and are indexed with the same
   0x2a90 stride as the blocks. */
#define ai_Unk7d6a90 (*(char (*)[4])((char *)init_StockBlocks + 0x20))      /* 0x7d6a90 */
#define ai_Unk7d6a94 (*(char (*)[12])((char *)init_StockBlocks + 0x24))     /* 0x7d6a94 */
#define ai_Unk7d6aa0 (*(char (*)[8])((char *)init_StockBlocks + 0x30))      /* 0x7d6aa0 */
#define ai_Unk7d6aa8 (*(char (*)[54408])((char *)init_StockBlocks + 0x38))  /* 0x7d6aa8 */

extern char ai_Unk61a348[];
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it (d3.h types it D3SCENE[]) */
extern char d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it (d3.h types it D3SCENE[]) */
extern unsigned char he_Events;
extern char d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it (d3.h types it D3SCENE[]) */
extern unsigned char txt_Keys;
extern GAPDATA full_data_91f6e0[];
extern unsigned char he_Events;
extern unsigned char amt_Table;
extern GAPDATA full_data_7585f4[];
extern unsigned char he_QuickList;

extern int gm_Offers[528];
extern int gm_PlayerRecords[688];
extern int ai_Unk7d6a40;
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);
struct CHARACTER * ch_Create();
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);
void ch_SetVisible();
unsigned short *gm_CreateObject(int id, unsigned proto, int n);
char *gm_FindCharacterByMaster(int m);
void gm_FreeObjectExtra(unsigned short *o);
void gm_ShowBuildingObjects(unsigned char *b);
void gm_ResetCharacters(void);
unsigned char gm_Unk5044b0(int i, int p);
unsigned short gm_Unk50a0b0(unsigned short *o);
void gm_UpdatePrices(unsigned p, void *t);

struct GCLOCK;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */
struct D3SURF;  /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */

unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */

extern int ai_Unk6f1b98;
extern int ai_Unk61772c;
extern int ai_Unk617730;
extern short * ai_Unk6f1bac;
/* The five income sums are the last int of the five 17-int rows of gm_IncomeHistory (0x91b580, 0x154
   bytes): the rows start 0x44 apart, so these aliases sit at the end of each row. They are views, not
   separate globals - a separate definition would reserve its own memory here and the 0x154-byte
   ord_ResetStatistics memset would zero whatever the COMMON linker put inside it (it hit ai_BTypes). */
#define ai_Unk91b5c0 (gm_IncomeHistory + 0x40)   /* 0x91b5c0, the current sum of row 0 */
#define ai_Unk91b604 (gm_IncomeHistory + 0x84)   /* 0x91b604, row 1 */
#define ai_Unk91b648 (gm_IncomeHistory + 0xc8)   /* 0x91b648, row 2 */
#define ai_Unk91b68c (gm_IncomeHistory + 0x10c)  /* 0x91b68c, row 3 */
#define ai_Unk91b6d0 (gm_IncomeHistory + 0x150)  /* 0x91b6d0, row 4 */

void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);                     /* 0x4c4830 */
int gm_GetGoodsValue();                     /* 0x509e60 */
int gm_TitleGroup();                     /* 0x504ad0 */
int amt_HasTownObject();                 /* 0x4e9400 */
char * gm_PlayerSeat(int p);                     /* 0x504a80 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
int unk_Unk4fd7e0();                    /* 0x4fd7e0 */

#define CH(i) (ai_Chars[(unsigned short)(i)])
#define REC_RATE(rec) ((rec)[6])

#include "time_view.h"

