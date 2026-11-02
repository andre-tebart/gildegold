/* The game data unit (0x4f9e30 .. 0x50d0b0; 0x50d100 starts f3init.c, tag "f3_init:"): name from the allocation
   tags "f3_gm:lpGebaeudePROT" / "f3_gm:lpObjekteINST" of the building and object tables (0x4fe570). One .data
   string block runs through it in function order (0x61774c "misc\statistics_taxes" .. "%1G %A" 0x61ac98 ..
   "_NACHRICHTEN_HS_67" 0x61ade0), so the character income / statistics code, the game clock, the building and
   object tables, the job / title tables, the character helpers, prices and worth and the standing / family
   helpers are one unit.
   Open: many callees are still declared K&R in this file (amt_HasTownObject must stay K&R: the typed prototype
   costs gm_Unk4fa4b0 a byte).
   Open: gm_GetWorth is int in every header now; the older callers still go through the K&R cast I50A640.
   Open: ai.h is not includable here, so the character / building / object records are read through gv.h's views
   ORDCHAR / ORDBLD / ORDOBJ, the job record through GMJOB (AIJOB, next to GCLOCK), the long message through
   f3gm_types.h's GMMSG (AIMSG), the crime records through GM_CRIMES (cm_types.h CRIME over cm.c's char
   ai_Crimes[]), the goods records of the
   stock blocks through GMBLOCK / GMSTOCK (f3gm_types.h, GM_BLOCK; f3init.c / hst.c hold their own copies
   INIT_STOCK / G12STOCK), or by byte offset (the rest, GM_GOODS, the building type records of ai_BTypes).
   Open: data: gm_QueryKind (0x61a686) is defined as 550 bytes, but only its first byte is the query kind; the
   job table gm_JobTalents (0x61a688) is a view inside it.
   Open: gm_QueryNext returns unsigned short * in every header though the cursor is an ORDOBJ *.
   Open: data: gm_DefaultStock (0x6178c8) is defined as 12 ints and ai_Unk6178f8 follows it, but gm_LoadTables
   copies 0xaa0 ints and gm_InitPlayerRecord walks 0x55 GMSTOCK records from it: one 0x2a80-byte template.
   Open: head: cm_SendExSellObject returns its message's slot handle (gm_SyncMarket waits on it), cm.h declares
   it void; this unit calls it through the cast cm_SendExSellObjectH.
   Open: the head tables (gm_BoyHead .. gm_SpyHeads, gm_OwnNameHeads) are GMHEAD records but still defined as
   unsigned arrays; their readers cast them.
   Open: not included yet (dcp headers conflicts f3gm HEADER): ai.h anm.h cm.h d2.h d3_interface.h d3_tools.h
   gfx.h hmt.h hst.h rt.h unk_458810.h unk_4b39f0.h; held by own declarations that differ from the owner's:
   d2_Objs, gm_MinutesBetween, gm_ClockSeason, gm_CompareTime; calls whose argument count differs from the
   definition (read the original's pushes): gm_Liking.
   Open: data: gm_KindObjCount (0x76fdc4) is defined '= 0' as D5 asks, but gm_BuildKindObjects is byte-identical
   only with a tentative definition (or none in this TU): its definition form / owner is unsettled.
   Open: gm_TitlePeriodServed writes a GCLOCK through its int *out (he.c, main.c, unk_58bbd0.c declare it so in
   their own prototypes); the typed parameter waits for those units to include f3gm.h.
   unit of rea 8cef7d5b (src/game/f3gm.c); boundaries in decl/text.dcl */
struct CHARACTER;
struct CHTRANSPORT;
struct AICHAR;
extern char ai_PlayerSlots[4][0x2f4];
#include "d2_types.h"
#include "f3gm_types.h"
extern D2WIN d2_Wins[];
extern D2VIEW d2_Views[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "di_input.h"
#include "data_views.h"
#include "amt.h"
extern char ai_Crimes[];
#define GAPDATA __declspec(align(1)) unsigned char
extern char gm_StockBlocks[];                 /* 0x7d6a60, the five 0x2a90-byte stock blocks (one per player slot) */
/* The stock area is ONE 0xd4d0-byte object at 0x7d6a60 (five blocks of 0x2a90: four header ints, then
   0x55 records of 0x80 bytes). The other names are views into it: a separate definition would reserve
   its own memory and the block writer/reader (gm_LoadTables, ls_ReadPlayerSlots, ls_WritePlayerSlots) would then
   write over whatever the COMMON linker placed inside the 0xd4d0-byte span. */
#define init_StockBlocks (*(char (*)[0xd4c0])((char *)gm_StockBlocks + 0x10))  /* 0x7d6a70, the records of block 0 */
#define ai_Unk7d6a64 (*(int (*)[3])((char *)gm_StockBlocks + 4))  /* 0x7d6a64, the rest of block 0's header */
/* 0x7d6a90 .. 0x7d6aa8 are views into stock block 0 (the record fields the price code reads), not
   separate COMMON symbols: they start inside init_StockBlocks[0] and are indexed with the same
   0x2a90 stride as the blocks. */
#define ai_Unk7d6a90 (*(char (*)[4])((char *)init_StockBlocks + 0x20))      /* 0x7d6a90 */
#define ai_Unk7d6a94 (*(char (*)[12])((char *)init_StockBlocks + 0x24))     /* 0x7d6a94 */
#define ai_Unk7d6aa0 (*(char (*)[8])((char *)init_StockBlocks + 0x30))      /* 0x7d6aa0 */
#define ai_Unk7d6aa8 (*(char (*)[54408])((char *)init_StockBlocks + 0x38))  /* 0x7d6aa8 */
#define GM_BLOCK(p) ((GMBLOCK *)gm_StockBlocks + (p))  /* the stock block of the player p */
#define GM_TABLE_A(p) ((GMPOINT *)(ai_PlayerSlots[p] + 0x64))  /* the player slot's first yearly table */
#define GM_TABLE_B(p) ((GMPOINT *)(ai_PlayerSlots[p] + 0xb4))  /* its second yearly table */

#include "am_types.h"
extern G7GRP gm_BuildingItemGroups[];           /* 0x61a348 */
/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "d3_types.h"
extern GAPDATA full_data_91f6e0[];
extern unsigned char he_Events[339968];
#include "amt_types.h"
extern GAPDATA full_data_7585f4[];

#include "ai_types.h"
extern AISTORE gm_Offers[];            /* 0x7e4b20 */
extern int gm_PlayerRecords[688];
extern int ai_Unk7d6a40;
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);
unsigned short *gm_CreateObject(int id, int proto, int n);
char *gm_FindCharacterByMaster(int m);
void gm_FreeObjectExtra(unsigned short *o);
void gm_ShowBuildingObjects(unsigned char *b);
void gm_ResetCharacters(void);
int gm_TitleJob(int i, int p);
unsigned short gm_ObjectOwnerNo(unsigned short *o);
void gm_UpdatePrices(unsigned p, void *t);

struct GCLOCK;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */
struct D3SURF;  /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */

unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */

extern int gm_NetMsgCount;
extern int gm_RichestYear;
extern int gm_RichestWorth;
extern ORDCHAR *gm_RichestChar;
/* The five income sums are the last int of the five 17-int rows of gm_IncomeHistory (0x91b580, 0x154
   bytes): the rows start 0x44 apart, so these aliases sit at the end of each row. They are views, not
   separate globals - a separate definition would reserve its own memory here and the 0x154-byte
   ord_ResetStatistics memset would zero whatever the COMMON linker put inside it (it hit ai_BTypes). */
#define ai_Unk91b5c0 (gm_IncomeHistory + 0x40)   /* 0x91b5c0, the current sum of row 0 */
#define ai_Unk91b604 (gm_IncomeHistory + 0x84)   /* 0x91b604, row 1 */
#define ai_Unk91b648 (gm_IncomeHistory + 0xc8)   /* 0x91b648, row 2 */
#define ai_Unk91b68c (gm_IncomeHistory + 0x10c)  /* 0x91b68c, row 3 */
#define ai_Unk91b6d0 (gm_IncomeHistory + 0x150)  /* 0x91b6d0, row 4 */
#define gm_IncomeRows (*(int (*)[5][17])&gm_IncomeHistory)  /* 0x91b580, the five rows of 17 ints; [row][16] is the current sum */

int gm_GetGoodsValue(char *c);              /* 0x509e60 */
int gm_TitleGroup(int n);                /* 0x504ad0 */
char * gm_PlayerSeat(int p);                     /* 0x504a80 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
int gm_IsUnderAge(unsigned short id);         /* 0x4fd7e0 */

#define CH(i) (ai_Chars[(unsigned short)(i)])
#define REC_RATE(rec) ((rec)[6])

#include "time_view.h"
#include "io_text.h"
#include "ls.h"
#include "ahm.h"
#include "d3_io_a.h"
#include "err.h"
#include "f3init.h"
#include "he.h"
#include "main.h"
#include "mem.h"
#include "sv.h"
#include "tr.h"
#include "txt.h"
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "d3_engine.h"
#include "ch.h"
#include "cm_types.h"
#include "net.h"
#include "rd.h"
void *ai_ProdSetting(int param_1);  /* 0x5a8560 */
#define ai_ProdSettingK ((int (*)())ai_ProdSetting)  /* 0x5a8560: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
/* 0x4f9e30, the statistics screen: opens the form "misc\statistics_taxes", prints one line per round
   ("Runde %i:   %T$N": the game time - 0x10 + i and the round's value of the income history, 17 of them),
   then waits in the sv_RunFrame loop until a mouse event pops the request */

void gm_StatisticsScreen(void)
{
    int f;
    int i;

    f = d2_OpenForm(0, 0, "misc\\statistics_taxes");
    d2_CentreViewWindows(f);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(f, 0);
    for (i = 0; i < 0x11; i++)
        io_PrintText("Runde %i:   %T$N", ai_TimeYear - 0x10 + i, ((int *)gm_IncomeHistory)[i]);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(gm_StatisticsScreen, 0x677c7) != 0);
    d2_CloseView(f);
}
/* 0x4f9ec0, the income rating 0 .. 1: a weighted sum over the income kinds 9 .. 0xf of the table 0x4f2720
   (weights 0.3 0.25 0.12 0.1 0.05 0.08 0.1), each value normalized by its record's range:
   sum of (1 - (v - lo) / (hi - lo)) * weight
   @deep behaviour: 47/49 instructions, 13 differing - 9 fpu + 2 code + 6 fill. The whole delta is the FPU-stack form of the accumulation: the original computes the term (st0), rotates it to st1 (fstp st(1)), loads the sum (fld [sum]), adds (fadd st,st(1)) and stores (fstp [sum]), leaving the term on the stack so the loop tail pops it (fstp st(0)) and the final exit returns sum + the last term (fadd [sum]); ours does fadd [sum] / fstp [sum] / fstp st(0) and returns the sum alone (fld [sum]) - the 2 code instructions are the loop entry jmp / fstp st(0) that follow from it. The value expression (fild/fsub/fdivp/fsubr 1.0/fmul w[i-9]) is identical. Meaning-class leads that MATCHED and are kept: the loop bound is i <= 0xf (cmp 0xf / jle, was i < 0x10) and the term is a named local x with sum = sum + x (17 -> 13 differing, 48171 -> 48168 bytes in the unit). Measured and NOT it (byte-identical): sum = (float)(sum + ...), sum += ..., the swapped operand order, dropping the (float) cast. Q1/Q5/Q6 done this session (gm_IncomeRating, ROUGH gone, description rewritten); Q2..Q4, Q7 pass. */

float gm_IncomeRating(void)
{
    float w[7];
    float sum;
    float x;
    int rec[9];
    int i;

    w[0] = 0.3f;
    w[1] = 0.25f;
    w[2] = 0.12f;
    w[3] = 0.1f;
    w[4] = 0.05f;
    w[5] = 0.08f;
    w[6] = 0.1f;
    sum = 0.0f;
    for (i = 9; i <= 0xf; i++) {
        gv_GetTableRecord(i, rec);
        x = (1.0 - ((double)rec[6] - rec[1]) / ((double)rec[2] - rec[1])) * w[i - 9];
        sum = sum + x;
    }
    return sum;
}
/* 0x4f9f60, income kind 1 of the character id (rate of the record 9 * 0.01 * the character's int at +0x19c, not below 0): stores kind 1, the amount (+8 + 4k) and the record's rate (+0x20 + 4k) in the first free slot k of the 6 of out;
   flag 1 sends the amount to the player, flag 2 adds it to the sum 0x91b5c0. Returns 1 (slot found) or 0 */

int gm_IncomeKind1(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(9, rec);
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            cnt = *(int *)(CH(id) + 0x19c);
            x = (float)(rec[6] * 0.01f);
            x = (float)(x * cnt);
            if (!(x > 0))
                x = 0;
            n = (int)x;
            if (cnt > 0) {
                out[k] = 1;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    gm_NetMsgCount++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b5c0 += n;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4fa060, income kind 2 (the record 10, the int at +0x19c; sum 0x91b604): a copy of 0x4f9f60 */

int gm_IncomeKind2(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(10, rec);
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            cnt = *(int *)(CH(id) + 0x19c);
            x = (float)(rec[6] * 0.01f);
            x = (float)(x * cnt);
            if (!(x > 0))
                x = 0;
            n = (int)x;
            if (cnt > 0) {
                out[k] = 2;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    gm_NetMsgCount++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b604 += n;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4fa160, the income kind 3 of the character id: the rate of the decision 0xd percent of the character's v1a8
   into the first free entry of out (0 when out is full; no entry, but 1, when v1a8 is not positive); flags 1 moves
   the amount from the character to the player's object of the first building when the character's goods are
   worth it, flags 2 clears v1a8 and adds the amount to the income sum of row 2 */

int gm_IncomeKind3(unsigned id, GMINCOME *out, unsigned char flags)
{
    int rate;
    AIDEC rec;
    int k, n, base;
    float x;
    ORDBLD *b;
    ORDOBJ *o;

    gv_GetTableRecord(0xd, (int *)&rec);
    rate = rec.cur;
    b = (ORDBLD *)gm_FirstBuilding(1);
    gm_Query((void *)b->objList, 2, 6, 0, 0x115);
    for (k = 0; k < 6; k++) {
        if (out->kind[k] == 0)
            break;
    }
    if (k >= 6)
        return 0;
    id &= 0xffff;
    base = ORD_CHARS[id].v1a8;
    x = (float)(rate * 0.01f);
    x = (float)(x * base);
    if (!(x > 0))
        x = 0;
    n = (int)x;
    if (base > 0) {
        out->kind[k] = 3;
        out->amount[k] = n;
        out->rate[k] = rate;
        if (flags & 1) {
            b = (ORDBLD *)gm_FirstBuilding(1);
            o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
            if (gm_GetGoodsValue((char *)&ORD_CHARS[id]) >= n)
                cm_SendMoveGoodsUnchecked(o->objId, ORD_CHARS[id].netId, ai_OwnPlayer, n);
        }
        if (flags & 2) {
            ORD_CHARS[id].v1a8 = 0;
            gm_IncomeRows[2][16] += n;
        }
    }
    return 1;
}
/* 0x4fa2a0, the income kind 4 of the character id: the rate of the decision 0xc percent of the character's trade
   sums v1a0 + v1a4 into the first free entry of out (0 when out is full; no entry, but 1, when the sum is not
   positive); flags 1 moves the amount from the character to the object of the first building (one more net
   message), flags 2 adds it to the income sum of row 3 */

int gm_IncomeKind4(unsigned id, GMINCOME *out, unsigned char flags)
{
    int rate;
    AIDEC rec;
    int k, n, base;
    float x;
    ORDBLD *b;
    ORDOBJ *o;

    gv_GetTableRecord(0xc, (int *)&rec);
    rate = rec.cur;
    b = (ORDBLD *)gm_FirstBuilding(1);
    gm_Query((void *)b->objList, 2, 6, 0, 0x115);
    id &= 0xffff;
    base = ORD_CHARS[id].v1a0 + ORD_CHARS[id].v1a4;
    x = (float)(rate * 0.01f);
    x = (float)(x * base);
    if (!(x > 0))
        x = 0;
    n = (int)x;
    for (k = 0; k < 6; k++) {
        if (out->kind[k] == 0)
            break;
    }
    if (k >= 6)
        return 0;
    if (base > 0) {
        out->kind[k] = 4;
        out->rate[k] = rate;
        out->amount[k] = n;
        if (flags & 1) {
            b = (ORDBLD *)gm_FirstBuilding(1);
            o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
            cm_SendMoveGoodsUnchecked(o->objId, ORD_CHARS[id].netId, 0, n);
            gm_NetMsgCount++;
        }
        if (flags & 2)
            gm_IncomeRows[3][16] += n;
    }
    return 1;
}
/* 0x4fa3d0, income kind 5 (the record 0xb, the int at +0x19c; sum 0x91b6d0); the message goes to all players (-1). ROUGH */

int gm_IncomeKind5(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(0xb, rec);
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            cnt = *(int *)(CH(id) + 0x19c);
            x = (float)(rec[6] * 0.01f);
            x = (float)(x * cnt);
            if (!(x > 0))
                x = 0;
            n = (int)x;
            if (cnt > 0) {
                out[k] = 5;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    cm_SendMoveGoodsUnchecked(-1, *(int *)(CH(id) + 4), 0, n);
                    gm_NetMsgCount++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b6d0 += n;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4fa4b0, collects the incomes of the character id into the record out (cleared first): none (0) for a free
   record, one not in use (health 0) or one under age (gm_IsUnderAge); else the kind 1 or 2 (2 when the title group
   of the character's job has a town object, amt_HasTownObject), 5, 3 (only when v1a8 is positive) and 4. Returns 1
   @deep Read whole against the original: statement structure equal (branch targets shift only by the
   one-instruction size difference). Tried: index loop vs memset (identical code), the index
   ORD_CHARS[(unsigned short)id] at each use (out in ebx, id in edi, ebp pushed late: allocation), a
   character pointer c (lea ebp,[..+ai_Chars] where the original folds the base into [esi+ai_Chars+off]:
   wrong form, reverted), the index temp i = (unsigned short)id (kept: 11 of 84 differ). Remaining hunk at
   +0x5: ours mov edx,ebx / and edx,0xffff / mov ecx,0xe / xor eax,eax / rep stosd / lea eax,[edx+edx*2],
   the original mov eax,ebx / and eax,0xffff / lea eax,[eax+eax*2] scheduled before xor eax,eax / rep
   stosd: the same instructions with the index in edx instead of eax and the memset moved before it;
   because of that order ours repeats xor eax,eax at the first return 0 (the original reuses the zero eax
   of the stosd), the one extra instruction. Scheduling / allocation of the prologue only. */

int gm_CharacterIncomes(unsigned id, GMINCOME *out, unsigned char flags)
{
    int i, k;

    i = (unsigned short)id;
    for (k = 0; k < 0xe; k++)
        ((int *)out)[k] = 0;
    if (ORD_CHARS[i].id == 0xffff)
        return 0;
    if (ORD_CHARS[i].health == 0)
        return 0;
    if (gm_IsUnderAge(id) != 0)
        return 0;
    if (amt_HasTownObject(gm_TitleGroup(ORD_CHARS[i].jobId)) != 0)
        gm_IncomeKind2(id, (char *)out, flags);
    else
        gm_IncomeKind1(id, (char *)out, flags);
    gm_IncomeKind5(id, (char *)out, flags);
    if (ORD_CHARS[i].v1a8 > 0)
        gm_IncomeKind3(id, out, flags);
    gm_IncomeKind4(id, out, flags);
    return 1;
}

struct AMTKIND; int amt_GetKind(int k, struct AMTKIND *out);                   /* 0x4e6f80 */
float gm_AddHealth(unsigned short id, int d);  /* 0x4fca40 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
float gm_Mix(int a, int b, int c);                     /* 0x504e20 */
void ai_BeginNeedBlock(char *p);                     /* 0x4c3e70 */
int ai_AddNeedDelta(int k, float v);                     /* 0x4c3eb0 */
void ai_SendNeedBlock(void);                     /* 0x4c4a20 */
void gm_ShiftLoyalty();                   /* 0x4fb0e0 */
int gm_FindRichest(short *id, int *rate);
#define I50A640 ((int (*)())gm_GetWorth)
extern char * cm_FieldObject;                /* 0xa68d2c */

/* 0x4fa580, the upkeep of the characters: rate = gm_PlayerFactor (0xe - the decision 5's value, the own player); for
   every character in use of the kind 0 with a home (bld2): in debt (gm_GetGoodsValue negative), one without a
   building or partner whose debt is above twice the rate loses 5000 health, one with a building lowers its relation
   to the building's owner by 10; then a home of the building kind 1 pays the rate to the player's seat object
   (gm_PlayerSeat (1)). Pumps the net after every 0x40 messages */
void gm_CharacterUpkeep(void)
{
    ORDOBJ *seat;
    AIDEC rec;
    ORDCHAR *c;
    char *b;
    int rate, i, v;

    seat = (ORDOBJ *)gm_PlayerSeat(1);
    gv_GetTableRecord(5, (int *)&rec);
    rate = gm_PlayerFactor(0xe - rec.cur, ai_OwnPlayer);
    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
        if (c->id == 0xffff || c->health == 0 || c->kind != 0 || c->bld2 == 0)
            continue;
        b = ai_BTypes + c->bld2->type * 0x24d;
        v = gm_GetGoodsValue((char *)c);
        if (v < 0) {
            if (c->bld == 0 && c->partner == -1 && abs(v) > rate * 2) {
                gm_AddHealth(i, -5000);
                gm_NetMsgCount++;
                continue;
            }
            if (c->bld != 0) {
                cm_SendRelations(ORD_CHARS[c->bld->owner].netId, c->netId, -10, 1);
                gm_NetMsgCount++;
            }
        }
        if (*b == 1) {
            cm_SendMoveGoodsUnchecked(seat->objId, c->netId, 0, rate);
            gm_NetMsgCount++;
        }
    }
    gm_NetMsgCount = 0;
}
/* 0x4fa700, the wages of the character id: out gets the two profession ids, the wage kind 0x23 .. 0x27 by the
   highest privilege flag 0x200 .. 0x2000 (0x1000 first), and for a person in use (kind below 10, alive) with a
   profession or a privilege the three wages: the AMTKIND salary of each times 32 times amt_Pay of the decision
   0xf's value. flag != 0 pays them (the player's seat object must exist): one goods move each from the character,
   and for the kind 3 a random extra ((0 .. level + 1) + level) * 0x1900 and a money delta of the level.
   Returns 1 when the wages were computed
   @deep Read whole against the original and rewritten (ORDWAGES / ORDCHAR / AMTKIND / AIDEC typed): the
   kind chain into a local, the index form ORD_CHARS[id] (the original folds ai_Chars into the
   displacements), the wage order job2 / job / kind, the money delta of AMTKIND.level (was the column), the
   random extra via a temp. 207 = 207 instructions, statement structure equal (the je/jae targets differ by
   the one byte of the end). Remaining: hunk 0x4fa95e (code 6) is the same three instructions reordered
   with the commutative add's destination swapped: ours add ecx,eax / lea eax,[ecx+ecx*4] / mov
   ecx,[esi+netId], the original mov edx,[esi+netId] / add eax,ecx / lea eax,[eax+eax*4]; hunk 0x4fa974
   (regs 14) is the rotation eax->ecx ecx->edx edx->eax that follows from it through the two sends. Tried
   (r + level) and (level + r), an int and an unsigned short temp, the one-expression form: no change.
   Allocation / scheduling of the kind-3 tail only. */

int gm_CharacterWages(unsigned short id, int flag, unsigned char *rec_)  /* the callers pass a 16-bit id and the original call site loads it without an extension (0x4f5a80: mov cx,[esi]) */
{
    int res;
    AMTKIND kind;
    AIDEC rec;
    float pay;
    ORDBLD *b;
    int f, k;
    unsigned short r;
    ORDWAGES *out;

    out = (ORDWAGES *)rec_;
    res = 0;
    out->job = ORD_CHARS[id].job;
    out->job2 = ORD_CHARS[id].job2;
    f = ORD_CHARS[id].flags1cc;
    if (f & 0x1000)
        k = 0x26;
    else if (f & 0x2000)
        k = 0x27;
    else if (f & 0x800)
        k = 0x25;
    else if (f & 0x400)
        k = 0x24;
    else
        k = (f & 0x200) ? 0x23 : 0;
    out->kind = k;
    out->amount = 0;
    out->amount2 = 0;
    out->amount3 = 0;
    if (ORD_CHARS[id].id != 0xffff && ORD_CHARS[id].kind < 10 && ORD_CHARS[id].health > 0 && (ORD_CHARS[id].job != 0 || ORD_CHARS[id].job2 != 0 || (ORD_CHARS[id].flags1cc & 0xe00) != 0)) {
        gv_GetTableRecord(0xf, (int *)&rec);
        pay = amt_Pay[rec.cur];
        amt_GetKind(ORD_CHARS[id].job2, &kind);
        out->amount2 = (int)(kind.salary * 32 * pay);
        amt_GetKind(ORD_CHARS[id].job, &kind);
        out->amount = (int)(kind.salary * 32 * pay);
        amt_GetKind(out->kind, &kind);
        out->amount3 = (int)(kind.salary * 32 * pay);
        res = 1;
        if (flag != 0) {
            b = (ORDBLD *)gm_FirstBuilding(1);
            if (b == 0)
                return 0;
            if (gm_Query((void *)b->objList, 2, 6, 0, 0x115) == 0)
                return 0;
            if (out->amount != 0) {
                cm_SendMoveGoodsUnchecked(ORD_CHARS[id].netId, -1, 0, out->amount);
                gm_NetMsgCount++;
            }
            if (out->amount2 != 0) {
                cm_SendMoveGoodsUnchecked(ORD_CHARS[id].netId, -1, 0, out->amount2);
                gm_NetMsgCount++;
            }
            if (out->amount3 != 0) {
                cm_SendMoveGoodsUnchecked(ORD_CHARS[id].netId, -1, 0, out->amount3);
                gm_NetMsgCount++;
            }
            if (ORD_CHARS[id].kind == 3) {
                r = util_Random((unsigned short)(kind.level + 2));
                cm_SendMoveGoodsUnchecked(ORD_CHARS[id].netId, -1, 0, (r + kind.level) * 0x1900);
                cm_SendMoneyDelta(ORD_CHARS[id].netId, kind.level);
                gm_NetMsgCount += 2;
            }
        }
    }
    return res;
}
/* 0x4fa9b0, runs 0x4fa700 (i, 1, out) for all 0x300 characters, flushing the messages every 0x40 */

void unk_Unk4fa9b0(void)
{
    unsigned char out[16];
    int i;

    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        gm_CharacterWages(i, 1, out);
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
    }
    gm_NetMsgCount = 0;
}
/* 0x4faa00, the worth mood of the characters: for every character in use (kind below 10, alive) shifts the worth
   history down (worth[0 .. 2] = worth[1 .. 3]) and stores the current worth (gm_GetWorth) in worth[3], as one field
   block; moves mood3Rate halfway to the mean of min (the history's average / the worth, 1) and 1 - the worth / the
   richest character's (gm_FindRichest); lowers the mood 3 value by 5 percent. Pumps the net after every 0x40
   messages */

void gm_UpdateWorthMood(void)
{
    short id;
    int top;
    ORDCHAR *c;
    int i, k, sum, cnt, avg, a, v;
    float q, lim;

    gm_FindRichest(&id, &top);
    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
        if (c->id == 0xffff || c->kind >= 10 || !(c->health > 0))
            continue;
        sum = 0;
        cnt = 0;
        a = gm_GetWorth(c->id);
        cm_BeginFieldBlock((char *)c, c->netId);
        for (k = 0; k < 3; k++) {
            v = c->worth[k + 1];
            sum += v;
            cnt += v != 0;
            cm_AddFieldDelta(4, 1, (short)((char *)&c->worth[k] - cm_FieldObject), &v);
        }
        avg = (a + sum) / (cnt + 1);
        cm_AddFieldDelta(4, 1, (short)((char *)&c->worth[3] - cm_FieldObject), &a);
        lim = (float)avg / (float)a < 1.0 ? (float)avg / (float)a : 1.0;
        q = 1.0f - (float)a / top;
        cm_SendFieldDeltas();
        gm_NetMsgCount++;
        cm_SendFloatDelta(c->netId, 0x1e4, -(c->mood3Rate - (float)((lim + q) * 0.5)));
        q = -(c->mood[3][2] - c->mood[3][2] * 0.95);
        ai_BeginNeedBlock((char *)c);
        ai_AddNeedDelta(3, q);
        ai_SendNeedBlock();
        gm_NetMsgCount++;
    }
    gm_NetMsgCount = 0;
}
/* 0x4fabe0, the end of the day for the incomes: shifts the 17-round history of each of the five income sums one
   place and clears the current sums; settles every character's incomes (gm_CharacterIncomes) and clears its
   v198 / wage; with flag 1 also shifts the loyalties (gm_ShiftLoyalty) and lists up to 8 owned buildings of the
   query (1, 5, 7) with their type levels (the list is not used further); then clears every building's roundSum
   and, for the kinds 11 .. 13, its daySum.
   @deep Read whole against the original: the five memmoves, the sum clears, the character loop (i < 0x300
   strength-reduced to the +0x198 pointer), the twin flags&1 tests (esi = flags & 1 reused), the 8-entry
   owned-building list sharing the incomes buffer (union, frame equal), the building loop (11..13 unsigned)
   all equal. One hunk left at 0x4facdf: the type-level load: the original keeps the BTypes base in ebx
   (xor edx,edx; mov ebx,[ai_BTypes]; mov dl,[eax]; add ebx,edx; lea ecx,[edx+edx*2] .. mov
   dl,[ebx+ecx*4+0x247]) plus a dead lea ecx,[ebx+ecx*4]; ours uses eax/ecx/edx for the same ops (mov
   cl,[eax]; lea eax,[ecx+ecx*2] ... mov cl,[eax+edx*4+0x247]). Tried: a temp pointer t before/after the
   list store, levels[n++], a struct-typed index (same code): register choice and the materialised dead
   address only. */

void gm_EndDayIncomes(unsigned flags)
{
    ORDBLD *b;
    int i, n;
    char *t;
    unsigned levels[8];
    union {                     /* one buffer: the original's frame holds the list where the incomes were */
        GMINCOME inc;
        ORDBLD *list[8];
    } u;

    memmove(gm_IncomeRows[0], gm_IncomeRows[0] + 1, 16 * sizeof(int));
    memmove(gm_IncomeRows[1], gm_IncomeRows[1] + 1, 16 * sizeof(int));
    memmove(gm_IncomeRows[2], gm_IncomeRows[2] + 1, 16 * sizeof(int));
    memmove(gm_IncomeRows[3], gm_IncomeRows[3] + 1, 16 * sizeof(int));
    memmove(gm_IncomeRows[4], gm_IncomeRows[4] + 1, 16 * sizeof(int));
    gm_IncomeRows[0][16] = 0;
    gm_IncomeRows[1][16] = 0;
    gm_IncomeRows[2][16] = 0;
    gm_IncomeRows[3][16] = 0;
    gm_IncomeRows[4][16] = 0;
    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        gm_CharacterIncomes(i, &u.inc, flags);
        ORD_CHARS[i].wage = 0;
        ORD_CHARS[i].v198 = 0;
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
    }
    if (flags & 1)
        gm_ShiftLoyalty();
    if (flags & 1) {
        b = (ORDBLD *)gm_BuildQuery(1, 5, 7);
        n = 0;
        while (b != 0 && n < 8) {
            if (b->owner != 0xffff) {
                t = ai_BTypes + b->type * 0x24d;
                u.list[n] = b;
                levels[n++] = (unsigned char)t[0x247];
            }
            b = (ORDBLD *)gm_BuildNext();
        }
    }
    for (i = 0; i < 0x100; i++) {
        ((ORDBLD *)ai_Blds)[i].roundSum = 0;
        if (((ORDBLD *)ai_Blds)[i].type >= 11 && ((ORDBLD *)ai_Blds)[i].type <= 13)
            ((ORDBLD *)ai_Blds)[i].daySum = 0;
    }
}
/* 0x4fad60, finds the richest character (the highest gm_GetWorth of the used records of kind < 10 that are alive),
   searching again only when the cached one is gone or the game year has advanced; out: its id (-1 when none) and
   its worth (0xc80 when none). Returns 1 when one was found
   @deep Read whole against the original: the cache test (char 0 / year advanced / id 0xffff / health 0),
   the 0x300 loop (id, kind < 10, health > 0, gm_GetWorth), the else-goto block order (now equal: branch
   targets agree, sizes 71/71) and both returns. Left: diffclass regs hunks eax<->ecx (gm_RichestChar in
   ecx, the year in eax at entry; asmdiff shows the same instructions rotated) and two code hunks that are
   scheduling of the same instructions: at 0x4fae00 the original sets eax=1 before the stores and loads the
   worth pointer after pop ebp ([esp+0xc]) where ours loads both out-pointers first; at 0x4fae37 pop ebx
   placed after the stores instead of before. Tried: ai_TimeYear > gm_RichestYear (worse), if/goto layouts. */

int gm_FindRichest(short *id, int *worth)
{
    int best, bi, i, v;

    best = -100000;
    bi = 0xffff;
    if (gm_RichestChar == 0 || gm_RichestYear < ai_TimeYear || gm_RichestChar->id == 0xffff
        || gm_RichestChar->health == 0) {
        gm_RichestYear = ai_TimeYear;
        for (i = 0; i < 0x300; i++) {
            if (ORD_CHARS[i].id != 0xffff && ORD_CHARS[i].kind < 10 && ORD_CHARS[i].health > 0) {
                v = gm_GetWorth(i);
                if (v > best) {
                    best = v;
                    bi = i;
                }
            }
        }
        if (bi != 0xffff) {
            gm_RichestWorth = best;
            gm_RichestChar = &ORD_CHARS[bi];
        } else {
            gm_RichestChar = 0;
            gm_RichestWorth = 0xc80;
            goto none;
        }
    }
    if (gm_RichestChar != 0) {
        *id = gm_RichestChar->id;
        *worth = gm_RichestWorth;
        return 1;
    }
none:
    *id = -1;
    *worth = gm_RichestWorth;
    return 0;
}
/* 0x4fae50, the owner of the building b hands out money to the other characters (in use, of kind < 10 but not 6 / 7 / 8,
   not the owner): each one with the chance b1b0 * 0.25 * (1 + 0.0125 * the building's skill 9), times 0.8 / 0.7 / 0.6
   when its b00c differs from the owner's (the decision 2's value 2 / equal to the owner's / else), gets
   (max(its worth / the richest worth * 160, 0) + 160) * that chance; its loyalty moves by +0.05 (same b00c) or -0.05
   while inside 0.05 .. 0.95, and with a partner in out[2] the relation is sent too. out[0] = the number of characters
   paid, out[1] = the sum (the caller takes it from the owner). Returns 1, or 0 when b has no owner.
   out[2] is read past the caller's two ints (he_Unk542590 declares int out[2]), as the original does. */

int gm_HandOutMoney(ORDBLD *b, int *out)
{
    int n, sum, i, a, v, top;
    short id;
    AIDEC rec;
    ORDCHAR *c;
    ORDCHAR *u;
    float f, p, lo;

    sum = 0;
    n = 0;
    gm_FindRichest(&id, &top);
    if (b->owner == 0xffff)
        return 0;
    u = &ORD_CHARS[b->owner];
    gm_GetSkill((char *)u, 4);
    gv_GetTableRecord(2, (int *)&rec);
    f = (float)gm_BuildingSkill((unsigned char *)b, 9, 1) * 0.0125f + 1.0f;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->kind != 6 && c->kind != 7 && c->kind != 8 && c->kind < 10 && c->netId != u->netId) {
            p = (float)c->b1b0 * 0.25f;
            if (c->b00c == u->b00c) {
                p *= f;
                lo = 0.05f;
            } else {
                if (rec.cur == 2)
                    p = (float)((double)p * f * 0.8);
                else if (u->b00c == rec.cur)
                    p = (float)((double)p * f * 0.7);
                else
                    p = (float)((double)p * f * 0.6);
                lo = -0.05f;
            }
            if (util_RandomF() * 2.0f <= p) {
                v = (int)((((float)gm_GetWorth(c->id) / (float)top * 160.0f > 0.0f
                            ? (float)gm_GetWorth(c->id) / (float)top * 160.0f : 0.0f) + 160.0f) * p);
                sum += v;
                cm_SendMoveGoodsUnchecked(-1, c->netId, ai_OwnPlayer, v);
                n++;
                if (c->loyalty < 0.95f && c->loyalty > 0.05f)
                    cm_SendFloatDelta(c->netId, 0x1d0, lo);
                if (out[2] != 0) {
                    a = (int)(gm_Mix((int)u, (int)c, 4) * 10.0f + 1.0f);
                    cm_SendRelations(((ORDCHAR *)out[2])->netId, c->netId, a, 1);
                }
            }
        }
    }
    out[0] = n;
    out[1] = sum;
    return 1;
}
/* 0x4fb0e0, every fourth year (ai_TimeYear % 4 == 0), over the characters in use of kind 1 .. 9 but not 6 / 7 / 8 and
   w00a >= 12: when the decision 2's value is 2, each whose loyalty is below a random 0 .. 0.2 flips its b00c and gets
   0.25 .. 0.75 added to the loyalty; else each whose b00c differs from (value != 0) is accused (gv_Accuse 2) and
   loses 0.1 loyalty while above 0.1. Pumps the net every 0x40 messages. */

void gm_ShiftLoyalty(void)
{
    AIDEC rec;
    int t, m, i, faith, other;
    ORDCHAR *c;
    unsigned flip;

    if (ai_TimeYear % 4 != 0)
        return;
    gv_GetTableRecord(2, (int *)&rec);
    if (rec.cur == 2) {
        gm_NetMsgCount = 0;
        for (i = 0; i < 0x300; i++) {
            c = &ORD_CHARS[i];
            if (c->id != 0xffff && c->kind < 10 && c->kind > 0 && c->kind != 6 && c->kind != 7 && c->kind != 8
                && c->w00a >= 12 && util_RandomF() * 0.2f > c->loyalty) {
                flip = c->b00c == 0;
                cm_SendFloatDelta(c->netId, 0x1d0, util_RandomF() * 0.5f + 0.25f);
                gm_NetMsgCount++;
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldDelta(1, 1, 0xc, &flip);
                cm_SendFieldDeltas();
                gm_NetMsgCount++;
                if (gm_NetMsgCount > 0x40) {
                    sv_PumpNet();
                    gm_NetMsgCount = 0;
                }
            }
        }
        gm_NetMsgCount = 0;
        return;
    }
    if (rec.cur == 0) {
        faith = 0;
        other = 1;
    } else {
        faith = 1;
        other = 0;
    }
    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->kind < 10 && c->kind > 0 && c->kind != 6 && c->kind != 7 && c->kind != 8
            && c->w00a >= 12 && c->b00c != faith) {
            t = -1;
            if (c->bld2 != 0)
                t = c->bld2->id;
            m = gv_Accuse(2, other, c->netId, -1, t);
            if (m != -1)
                gm_NetMsgCount++;
            if (c->loyalty > 0.1f)
                cm_SendFloatDelta(c->netId, 0x1d0, -0.1f);
            gm_NetMsgCount++;
            if (gm_NetMsgCount > 0x40) {
                sv_PumpNet();
                gm_NetMsgCount = 0;
            }
        }
    }
    gm_NetMsgCount = 0;
}

/* the head tables (records of 0x44 bytes: +0 the id, +4 the name, +0x34 four head numbers): 0x6118a8 / 0x6118f0 (the two default entries), 0x611938 (0x23 records, kind 0), 0x612288, 0x612bd8 (0x5f records, kind 0) / 0x614518 (0x5f, kind 1), 0x617398 (3 records), the history 0x91ac40 (8 records) */
extern unsigned gm_BoyHead[], gm_GirlHead[], gm_TitleHeadsMale[], gm_TitleHeadsFemale[], gm_JobHeadsMale[], gm_JobHeadsFemale[], gm_SpyHeads[], gm_OwnNameHeads[];  /* 0x6118a8, 0x6118f0, 0x611938, 0x612288, 0x612bd8, 0x614518, 0x617398, 0x91ac40 */
int d3_CurrentScene = 0;  /* 0x633444 */
char ai_Unk6f0454[3] = {0};  /* 0x6f0454 .. 0x6f0456 (unk_LoadIconNames writes [0..2]) */
int gm_IconNameCount = 0;  /* 0x6f1b90, the number of icon names */
char * gm_IconNames = 0;  /* 0x6f1b94, the icon names ( */
int gm_NetMsgCount = 0;  /* 0x6f1b98, the number of queued messages ( */
ORDCHAR *gm_RichestChar = 0;  /* 0x6f1bac, the richest character of gm_FindRichest's last search, 0 = none */
int gm_OwnNameHeadLast = 0;  /* 0x6f1bb0 */
void gm_ResolveHeadIcons(GMHEAD *rec);
int ch_CurrentGroup = -1;  /* 0x5f1294 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */

/* 0x4fb330, the record of the table kind (0: 0x612bd8, 1: 0x614518) with the id: prepared by 0x4fb620 (the head numbers); the first free record when the id is not found (0x5f records of 0x44 bytes); 0 for another kind */
unsigned *gm_NameRecord(int kind, int id)
{
    unsigned *t;
    int i;

    i = 0;
    if (kind == 0)
        t = gm_JobHeadsMale;
    else if (kind == 1)
        t = gm_JobHeadsFemale;
    else
        return 0;
    do {
        if (t[i * 0x11] == 0)
            break;
        if (t[i * 0x11] == (unsigned)id) {
            gm_ResolveHeadIcons(t + i * 0x11);
            return t + i * 0x11;
        }
        i++;
    } while (i < 0x5f);
    return t + i * 0x11;
}
/* 0x4fb390, the head record of the character id: a child (jobId 0, w00a below f020) gets the boy / girl head; a
   character with its own name and a picture gets a copy in the next record of the ring gm_OwnNameHeads (the four
   head numbers = the picture - 0x639); a spy (kind 0x11) one of the three spy heads; else the record of its job,
   or of its title, in the table of its sex (resolving its head icons), the table's first record when the title is
   not listed.
   @deep Read whole against the original: the child test (jobId 0, (float)w00a < f020) with boy/girl, the
   own-name ring copy (index (n+1)%8, inline strcpy loop, picture-0x639 into the four heads), the spy pick
   util_RandomU(3), the job loop (0x5f, key 0 ends, found -> gm_ResolveHeadIcons) and the title loop (0x23,
   i<0x23 after a key-0 end) and the sex fallback all have the original's statements, compares and
   constants; 199 vs 196 instructions. What remains is allocation and its block layout: ours spills p and k
   into an 8-byte frame (sub esp,8; mov [esp+0x10],edx; mov [esp+0x1c],bl; reloads mov ecx,[esp+0x10] / mov
   esi,[esp+0x1c] in both loops) and keeps gm_TitleHeadsMale in ebx, where the original holds p in ebp and
   k in bl with no frame (the fild temp in the argument slot); with the different register set VC6 places
   the job loop's found block after the title code (the branch-target pairs at +0x11d/+0x127 and the tail
   returns), and the remaining regs hunks are edi->esi / ebp->edi rotations. Tried: pointer c vs
   ORD_CHARS[id] indexing, separate t for the title, q pointer vs p[i], if/else nesting of the job/title
   parts, goto found, no s temp, v += -0x639 (the original's add edi,0xfffff9c7 stays unexplained: sub in
   ours). */

unsigned *gm_CharNameRecord(int id)
{
    GMHEAD *p, *q;
    unsigned char k;
    int i, v;

    p = 0;
    i = 0;
    k = ORD_CHARS[id].jobId;
    if (k != 0 || !((float)ORD_CHARS[id].w00a < ORD_CHARS[id].f020)) {
        if (ORD_CHARS[id].ownName[0] != 0 && (v = ORD_CHARS[id].picture) != -1) {
            gm_OwnNameHeadLast = (gm_OwnNameHeadLast + 1) % 8;
            strcpy(((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast].name, ORD_CHARS[id].ownName);
            v -= 0x639;
            ((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast].heads[0] = v;
            ((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast].heads[1] = v;
            ((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast].heads[2] = v;
            ((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast].heads[3] = v;
            return (unsigned *)&((GMHEAD *)gm_OwnNameHeads)[gm_OwnNameHeadLast];
        }
        if (ORD_CHARS[id].kind == 0x11)
            return (unsigned *)((GMHEAD *)gm_SpyHeads + (util_RandomU(3) & 0xffff));
        if (k != 0) {
            if (ORD_CHARS[id].textVariant == 0)
                p = (GMHEAD *)gm_JobHeadsMale;
            else if (ORD_CHARS[id].textVariant == 1)
                p = (GMHEAD *)gm_JobHeadsFemale;
            for (q = p; i < 0x5f; i++, q++) {
                if (q->key == 0)
                    break;
                if (q->key == k) {
                    gm_ResolveHeadIcons(&p[i]);
                    return (unsigned *)&p[i];
                }
            }
            return (unsigned *)&p[i];
        }
        k = ORD_CHARS[id].title;
        if (k != 0) {
            if (ORD_CHARS[id].textVariant == 0)
                p = (GMHEAD *)gm_TitleHeadsMale;
            else if (ORD_CHARS[id].textVariant == 1)
                p = (GMHEAD *)gm_TitleHeadsFemale;
            for (q = p; i < 0x23; i++, q++) {
                if (q->key == 0)
                    break;
                if (q->key == k) {
                    gm_ResolveHeadIcons(&p[i]);
                    return (unsigned *)&p[i];
                }
            }
            if (i < 0x23)
                return (unsigned *)&p[i];
        }
        if (ORD_CHARS[id].textVariant == 0)
            p = (GMHEAD *)gm_TitleHeadsMale;
        else if (ORD_CHARS[id].textVariant == 1)
            p = (GMHEAD *)gm_TitleHeadsFemale;
    } else {
        p = (GMHEAD *)gm_BoyHead;
        if (ORD_CHARS[id].textVariant != 0)
            return gm_GirlHead;
    }
    return (unsigned *)p;
}
/* 0x4fb5a0, picks the texture set of the character o's model: its id modulo the set count of the body mesh (or
   id & 3 when there are 5 or more), unless the model is in the first scene (d3_Scenes[0]); when the model belongs to
   the current group (ch_CurrentGroup) the set is selected on the body at once. */

void gm_SelectModelVariant(char *o)
{
    unsigned v, n;
    CHARACTER *m;
    D3SURF *surf;

    v = 0;
    if (o != 0 && (m = ORD_CHAR(o)->model) != 0) {
        surf = (D3SURF *)m->obj->material->animCtl;
        n = ((D3OPS *)surf->info.mesh)->nsets;
        if (m->scene != d3_Scenes) {
            if ((int)n <= 4)
                v = ORD_CHAR(o)->netId % n;
            else
                v = ORD_CHAR(o)->netId & 3;
            m->texSet = (unsigned char)v;
        }
        if (ch_CurrentGroup == m->group)
            d3if_SelectTextureSet(m->obj, surf, (unsigned char)v, 1);
    }
}
/* 0x4fb620, fills the four head numbers (+0x34 .. +0x40) of a head table record whose first is still 0xff: from the name (+4) split at the last '_' into name and suffix, the icon "%s_HEAD%i_%s_ICON.bmp" (or "%s_HEAD%i_ICON.bmp") is looked up in the icon names 0x6f1b94 (0x30 bytes each, 0x6f1b90 of them) for head 0 .. 3 */

void gm_ResolveHeadIcons(GMHEAD *rec)
{
    char name[128], suffix[128], icon[128];
    char *u;
    int h, i, *out;

    out = rec->heads;
    if (*out != 0xff)
        return;
    strcpy(name, rec->name);
    u = strrchr(name, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    for (h = 0; h < 4; h++, out++) {
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
        *out = 0xff;
        for (i = 0; i < gm_IconNameCount; i++) {
            if (gm_IconNames[i * 0x30] == 0)
                break;
            if (_stricmp(icon, gm_IconNames + i * 0x30) == 0) {
                *out = i;
                goto next;
            }
        }
        sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
        *out = 0xff;
        for (i = 0; i < gm_IconNameCount; i++) {
            if (gm_IconNames[i * 0x30] == 0)
                break;
            if (_stricmp(icon, gm_IconNames + i * 0x30) == 0) {
                *out = i;
                break;
            }
        }
    next:;
    }
}
/* 0x4fb770, the number of the first head icon (of the heads 0 .. 3) that exists for the head name (split at its
   last '_' into name and suffix as in gm_ResolveHeadIcons; names with "zigeunermeister" use the form without the
   suffix), 0 when none. */

int gm_HeadIconNumber(char *name)
{
    char buf[128], icon[128], suffix[128];
    char *u;
    int h, i, r;

    r = 0xff;
    strcpy(buf, name);
    u = strrchr(buf, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    for (h = 0; h < 4; h++) {
        if (r != 0xff)
            break;
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", buf, h, suffix);
        if (strstr(buf, "zigeunermeister") != 0)
            sprintf(icon, "%s_HEAD%i_ICON.bmp", buf, h);
        for (i = 0; i < gm_IconNameCount; i++) {
            if (gm_IconNames[i * 0x30] == 0)
                break;
            if (_stricmp(icon, gm_IconNames + i * 0x30) == 0) {
                r = i;
                break;
            }
        }
    }
    if (r == 0xff)
        return 0;
    return r;
}

extern char * ai_PlayerBuilding;  /* 0x6f2308 */
extern char * ai_FallbackBuilding;  /* 0x6f230c */
struct CHARACTER * ch_Create();                     /* 0x403390 */
int gm_IsHomeType(unsigned char *b);                     /* 0x502520 */
int init_ObjectSceneUniverse();                     /* 0x511dc0 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
unsigned *gm_CharNameRecord(int id);

/* 0x4fb890, chooses the portrait of the character c (its picture = the icon index + 0x639). With a model outside the
   first scene and the icon names loaded: the first texture name of the body's current texture set containing
   "kopf" / "KOPF" / "HEAD" / "head" whose "%s_ICON.bmp" is an icon. Without them: one of the head icons of its head
   record (resolved as gm_ResolveHeadIcons does, "zigeunermeister" without the suffix), picked by its id modulo the
   number of resolved heads. Else the picture 0x639. */

void gm_ChooseHeadIcon(unsigned short *c)
{
    char name[128], icon[128], suffix[128];
    char *u, *part;
    GMHEAD *rec;
    CHARACTER *m;
    D3SURF *surf;
    D3OPS *ops;
    int i, j, h, k, found, set;

    found = 0;
    i = 0;
    if (c == 0)
        return;
    m = ORD_CHAR(c)->model;
    if (m != 0 && gm_IconNames != 0) {
        if (m->scene == d3_Scenes)
            return;
        surf = (D3SURF *)m->obj->material->animCtl;
        ops = (D3OPS *)surf->info.mesh;
        set = surf->texSet;
        if (ops->count > 0) {
            do {
                if (found != 0)
                    return;
                part = ops->texnames + (ops->count * set + i) * 0x40;
                if (*part != 0 && (strstr(part, "kopf") != 0 || strstr(part, "KOPF") != 0
                                   || strstr(part, "HEAD") != 0 || strstr(part, "head") != 0)) {
                    sprintf(icon, "%s_ICON.bmp", part);
                    for (j = 0; j < gm_IconNameCount; j++) {
                        if (gm_IconNames[j * 0x30] == 0)
                            break;
                        if (_stricmp(icon, gm_IconNames + j * 0x30) == 0) {
                            found = 1;
                            ORD_CHAR(c)->picture = j + 0x639;
                            break;
                        }
                    }
                }
                i++;
            } while (i < ops->count);
            if (found != 0)
                return;
        }
    } else {
        rec = (GMHEAD *)gm_CharNameRecord(*c);
        k = 0;
        if (rec->heads[0] == 0xff) {
            strcpy(name, rec->name);
            u = strrchr(name, '_');
            if (u != 0) {
                strcpy(suffix, u + 1);
                *u = 0;
            }
            rec->heads[0] = 0;
            for (h = 0; h < 4; h++) {
                sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
                if (strstr(name, "zigeunermeister") != 0)
                    sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
                rec->heads[h] = 0xff;
                for (j = 0; j < gm_IconNameCount; j++) {
                    if (gm_IconNames[j * 0x30] == 0)
                        break;
                    if (_stricmp(icon, gm_IconNames + j * 0x30) == 0) {
                        rec->heads[h] = j;
                        break;
                    }
                }
            }
        }
        for (; k < 4; k++) {
            if (rec->heads[k] == 0xff)
                break;
        }
        if (k != 0) {
            ORD_CHAR(c)->picture = rec->heads[ORD_CHAR(c)->netId % k] + 0x639;
            return;
        }
    }
    ORD_CHAR(c)->picture = 0x639;
}
/* 0x4fbb80, creates the model of the character c (its model, ch_Create with the model name of its head record)
   in the scene scene: at the dummy named dummy of the object group obj when there is one (in world
   coordinates), else at p (the zero vector when p is 0; no model at all when p and ori are both 0), named
   "sp_<netId>", its body linked back to c (owner, pickId = id | 0x3000000) and turned to ori when given; copies
   the model name to c's own head name and chooses the variant and the head icon. The current scene is selected
   again at the end. Returns the model (0 when scene is negative).
   @deep Read whole against the original and Ghidra; meaning matches (same 130 instructions, calls, args,
   fields, loop). Left: (1) allocation: ours keeps ori in its arg slot ([esp+0x44] reload at +0x91) and
   caches 0 in ebx (xor ebx,ebx; cmp reg,ebx / push ebx / mov [zero],ebx), the original keeps p'=edi,
   o'=ebx, ori=ebp, c=esi and so uses test reg,reg / push 0 / mov [..],0 immediates - every code hunk but
   one is that substitution (cmp eax,ebx vs test eax,eax; ebp->ebx, eax->ebp regs). Tried o = ori ternary
   vs if, rot/o naming, declaration order. (2) block layout: the original places the d3_PointToWorld block
   after the epilogue (jne +0x8b to +0x174, jmp back to +0xb1), ours inline; tried if/else both polarities,
   else-if chain, nested if with goto placed, goto world label after the return - VC6 emits the identical
   layout (dist 62 ins every time), so it is not reachable from the statement structure tried. */

int gm_CreateCharModel(unsigned short *c, int scene, int *p, int *ori, int obj, char *dummy)
{
    int saved;
    int zero[4];
    int pos[4];
    int *o;
    D3OBJECT *it;
    GMHEAD *rec;
    char *name, *s, *d;
    CHARACTER *m;

    saved = d3_CurrentScene;
    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    zero[3] = 0;
    if (p == 0)
        p = zero;
    o = ori != 0 ? ori : zero;
    if (scene < 0)
        return 0;
    d3if_SelectScene(scene, 1);
    if (dummy != 0 && (it = d3_FindObject(obj, 0x100, 0, dummy)) != 0)
        d3_PointToWorld(it, &it->pos, (D3VECTOR *)pos);
    else if (p != 0 || o != 0) {
        pos[0] = p[0];
        pos[1] = p[1];
        pos[2] = p[2];
    } else
        goto done;
    rec = (GMHEAD *)gm_CharNameRecord(*c);
    if (rec != 0) {
        name = rec->name;
        m = ch_Create(name, (D3VECTOR *)pos);
        ORD_CHAR(c)->model = m;
        if (m != 0) {
            sprintf(m->obj->name, "sp_%i", ORD_CHAR(c)->netId);
            ORD_CHAR(c)->model->obj->owner = c;
            ORD_CHAR(c)->model->obj->pickId = *c | 0x3000000;
            if (ori != 0)
                d3_SetObjectRot(ORD_CHAR(c)->model->obj, (D3VECTOR *)ori);
            s = name;
            d = ORD_CHAR(c)->ownName;
            while ((*d++ = *s++) != 0)
                ;
            gm_SelectModelVariant((char *)c);
            gm_ChooseHeadIcon(c);
        }
    }
done:
    d3if_SelectScene(saved, 1);
    return (int)ORD_CHAR(c)->model;
}
/* 0x4fbd10, puts the character id (it must have no model yet) on the stage of its building, the building of
   the query (1, 1, a): with b != -1 at the object of the query (1, 1, b) over the building's object list,
   in the building's scene (a home type) or the object's (else the scene 0). The model (gm_CreateCharModel)
   stands at the dummy "dummy_TUER" of the building's object, or at the object's own dummy (ch_FindDummy,
   else "dummy_EINGANG") at p turned to ori; it preloads "bewegung/gehen", gets a and b as its group and
   script group, and is hidden when the shown building (ai_PlayerBuilding / ai_FallbackBuilding) is another.
   Returns the model or 0. */

int gm_PlaceOnStage(int id, int a, int b, int *p, int *ori)
{
    int saved;
    ORDCHAR *c;
    ORDBLD *o;
    unsigned short *o2;
    int sc;
    D3OBJECT *d;

    o2 = 0;
    saved = d3_CurrentScene;
    c = ORD_CHAR(gm_FindChar(id));
    if (c != 0 && c->model == 0) {
        o = (ORDBLD *)gm_BuildQuery(1, 1, a);
        if (o != 0) {
            if (b != -1) {
                o2 = (unsigned short *)gm_Query((void *)o->objList, 1, 1, b);
                if (o2 == 0)
                    return 0;
            }
            if (gm_IsHomeType((unsigned char *)o))
                sc = init_BuildingSceneUniverse((unsigned char *)o);
            else if (o2 != 0)
                sc = init_ObjectSceneUniverse(o2);
            else
                sc = 0;
            if (sc >= 0) {
                d3if_SelectScene(sc, 1);
                if (o2 != 0) {
                    if ((d = ch_FindDummy()) != 0)
                        gm_CreateCharModel((unsigned short *)c, sc, p, ori, 0, d->name);
                    else
                        gm_CreateCharModel((unsigned short *)c, sc, p, ori, 0, "dummy_EINGANG");
                } else
                    gm_CreateCharModel((unsigned short *)c, sc, 0, 0, (int)o->object, "dummy_TUER");
                if (c->model != 0)
                    ch_PreloadAnims(c->model, 1, "bewegung/gehen");
                d3if_SelectScene(saved, 1);
                if (c->model != 0) {
                    c->model->group = a;
                    c->model->scriptGroup = b;
                    if ((ai_PlayerBuilding != 0 && ai_PlayerBuilding != (char *)o)
                        || (ai_FallbackBuilding != 0 && ai_FallbackBuilding != (char *)o))
                        ch_SetVisible(c->model, 0);
                    return (int)c->model;
                }
            }
        }
    }
    return 0;
}

extern char ch_EmptyName[];             /* 0x631090 */
int gm_MayRemoveChar(char *c);                     /* 0x5075c0 */
void cm_SendKillCharacter(int a, int b);                     /* 0x4c4e00 */
int gv_DecayRaisedDead();                     /* 0x4f1660 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
void gm_PartnerStep(ORDCHAR *c);                 /* 0x4fccc0 */

#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (gv.h: char[]) as the AITIME it is */
/* sends the message 0x72 (the character c died) in the GMMSG m */
#define GM_SENDDEATH(m, c)                                             \
    memset(&(m), 0, sizeof(m));                                        \
    (m).to = (c)->netId;                                               \
    (m).time = ai_TimeT;                                               \
    (m).type = 0x72;                                                   \
    (m).fromId = -1;                                                   \
    (m).flags = 2;                                                     \
    (m).arg0 = (m).to;                                                 \
    cm_SendLongMessage((char *)&(m))
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag);  /* 0x5092e0 */

/* 0x4fbe90, the daily step of the character c; returns its new life value (-1 for no step). A free slot and a
   character of kind 0xf take no step (the latter is killed when gm_MayRemoveChar lets it go); a raised dead
   person (kind 0x14) decays (gv_DecayRaisedDead) and, used up, dies: the message 0x72, and the news 0x414a to
   the master of its building when that is a player (kind 6 / 7). Every other character halves the counters of
   v02c, then: one marked to die (flags1cc bit 2) loses 0x11 + random 0x10 of its life f01c and dies below 0
   (news 0x1c51); else its life moves toward f010 while it is younger (w00a) than f020, beyond that it loses f014
   less 3.75 % per point of the building skill 8 of its home, and when f01c + v024 is used up it dies (news
   0x1c51, or 0x4080 with its name and age for a flags1cc bit 4 one) - unless flags1cc bit 1 saves it (not a
   player): life 100, v024 0, flags1cc 2. A survivor runs gm_PartnerStep and ages by one.
   @deep Reconstructed and read whole against the original (633 vs 634 ins, all
   calls/fields/branches/strings matched: message 0x72 fill, nine counter halvings, the kind 0x14 /
   flags1cc bit 2 / life paths, the 0x414a / 0x1c51 / 0x4080 news, the double 0.0 compares, short age with
   its top init). Left: (1) fpu at +0x533..+0x552: the original computes skill*0.0125f, fmul -3.0f (literal
   0x5ed1b4), fadd [lo], fld st(0) and keeps lo's copy (fstp st(0) after); our VC6 turns the same
   expression into fmul 3.0 / fsubr [lo] / fxch - tried x*0.0125f*-3.0f+lo (folds to 0.0375), lo += ..., lo
   + ..., -3.0f*(...), a float k=-3.0f, (float)(x*0.0125f) casts, lo - ...*3.0f: none keeps fmul -3 + fadd;
   (2) order hunks +0xf3 / +0x3f9 / +0x6a0: the AITIME struct copy of the message (time+8 / time+0xc stored
   before vs after type/fromId/flags) and the counter halving's and/or order. The one 'branch targets
   differ' is the je at +0x514 whose target is +0x53d on both sides (the join after the skill block, inside
   the fpu hunk). */

float gm_DailyCharacterStep(unsigned short *id)
{
    GMMSG m;
    char buf[512];
    unsigned f, g;
    char b;
    float h, lo, t, fl;
    double d;
    short age;
    ORDCHAR *c = ORD_CHAR(id);
    ORDBLD *mst;

    age = 0;
    if (c->id == 0xffff)
        return -1.0f;
    if (c->kind == 0x0f) {
        if (gm_MayRemoveChar((char *)c) != 0) {
            cm_SendKillCharacter(c->netId, 1);
            gm_NetMsgCount++;
        }
        return -1.0f;
    }
    if (c->kind == 0x14) {
        if (gv_DecayRaisedDead((struct AICHAR *)c) == 1) {
            b = 0;
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValue(1, 1, (char *)&c->health - cm_FieldObject, &b);
            cm_SendFieldValues();
            GM_SENDDEATH(m, c);
            mst = c->bld;
            if (mst != 0 && mst->owner != 0xffff && (ORD_CHARS[mst->owner].kind == 6 || ORD_CHARS[mst->owner].kind == 7)) {
                txt_Format(buf, 0x414a, c->id, mst);
                he_ShowNews(ORD_CHARS[c->bld->owner].netId, -1, buf, 0, 0x60a, ch_EmptyName);
            }
        }
        return 0.0f;
    }
    f = c->v02c;
    if (f != 0) {
        if ((f & 0xf) != 0) {
            if ((f & 0xf) == 1)
                f &= 0xfffffff0;
            else
                f = (f & 0xfffffff0) | ((f >> 1) & 7);
        }
        g = (f >> 4) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xffffff0f;
            else
                f = (f & 0xffffff0f) | ((f >> 1) & 0x70);
        }
        g = (f >> 8) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xfffff0ff;
            else
                f = (f & 0xfffff0ff) | ((f >> 1) & 0x700);
        }
        g = (f >> 0xc) & 3;
        if (g != 0) {
            if (g == 1)
                f &= 0xffffcfff;
            else
                f = (f & 0xffffcfff) | ((f >> 1) & 0x1000);
        }
        g = (f >> 0xe) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xfffe3fff;
            else
                f = (f & 0xfffe3fff) | ((f >> 1) & 0xc000);
        }
        g = (f >> 0x11) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xfff1ffff;
            else
                f = (f & 0xfff1ffff) | ((f >> 1) & 0x60000);
        }
        g = (f >> 0x14) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xff8fffff;
            else
                f = (f & 0xff8fffff) | ((f >> 1) & 0x300000);
        }
        g = (f >> 0x17) & 3;
        if (g != 0) {
            if (g == 1)
                f &= 0xfe7fffff;
            else
                f = (f & 0xfe7fffff) | ((f >> 1) & (1 << 23));
        }
        g = (f >> 0x19) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xe1ffffff;
            else
                f = (f & 0xe1ffffff) | ((f >> 1) & 0xe000000);
        }
        cm_SendSetField(c->netId, 0x2c, 4, f, -1);
        gm_NetMsgCount++;
    }
    if (c->flags1cc & 2) {
        t = c->f01c - ((util_RandomU(0x10) & 0xffff) + 0x11);
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        gm_NetMsgCount++;
        if (t <= 0.0f) {
            b = 0;
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValue(1, 1, (char *)&c->health - cm_FieldObject, &b);
            cm_SendFieldValues();
            gm_NetMsgCount++;
            GM_SENDDEATH(m, c);
            gm_NetMsgCount++;
            mst = c->bld;
            if (mst != 0 && mst->owner != 0xffff && (ORD_CHARS[mst->owner].kind == 6 || ORD_CHARS[mst->owner].kind == 7)) {
                txt_Format(buf, 0x1c51, mst, c->id);
                he_ShowNews(ORD_CHARS[c->bld->owner].netId, -1, buf, 0, 0x60a, ch_EmptyName);
                gm_NetMsgCount++;
            }
        }
        return t;
    }
    fl = (float)c->w00a;
    h = fl;
    if (h <= c->f020) {
        t = ((c->f01c - c->f010) / c->f020) * h + c->f010;
    } else {
        lo = c->f014;
        if (c->bld2 != 0)
            lo = (gm_BuildingSkill((unsigned char *)c->bld2, 8, 1) * 0.0125f) * -3.0f + lo;
        t = lo / c->f01c + (c->f01c - lo);
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        gm_NetMsgCount++;
    }
    fl = (float)c->v024 + t;
    if (fl <= 0.0) {
        b = 0;
        if ((c->flags1cc & 1) != 0 && c->kind != 6 && c->kind != 7) {
            int zero = 0;


            t = 100.0f;
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldDelta(4, 1, 0x1c, &t);
            cm_AddFieldDelta(4, 1, 0x24, &zero);
            cm_SendFieldDeltas();
            gm_NetMsgCount++;
            cm_SendSetField(c->netId, 0x1cc, 4, 2, 0);
            gm_NetMsgCount++;
            return t;
        }
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValue(1, 1, (char *)&c->health - cm_FieldObject, &b);
        cm_SendFieldValues();
        gm_NetMsgCount++;
        GM_SENDDEATH(m, c);
        gm_NetMsgCount++;
        mst = c->bld;
        if (mst != 0 && mst->owner != 0xffff && (ORD_CHARS[mst->owner].kind == 6 || ORD_CHARS[mst->owner].kind == 7)) {
            if (c->flags1cc & 4) {
                txt_Format(buf, 0x4080, c->first, c->family, c->w00a);
                he_ShowNews(ORD_CHARS[c->bld->owner].netId, -1, buf, 0, 0x60a, "_NACHRICHTEN_DOMIZIL_05");
            } else {
                txt_Format(buf, 0x1c51, mst, c->id);
                he_ShowNews(ORD_CHARS[c->bld->owner].netId, -1, buf, 0, 0x60a, ch_EmptyName);
            }
            gm_NetMsgCount++;
        }
        return 0.0f;
    }
    gm_PartnerStep(c);
    if (fl > 0.0)
        d = (double)fl;
    else
        d = 0.0;
    age = c->w00a + 1;
    cm_BeginFieldBlock((char *)c, c->netId);
    cm_AddFieldDelta(2, 1, 10, &age);
    cm_SendFieldDeltas();
    gm_NetMsgCount++;
    return (float)d;
}

void cm_SendCharModel(int a, const char *s);                     /* 0x4c5690 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
float gm_CharacterHealth(short *c);
float gm_CharacterBaseHealth(short *c);
void gm_PopulationStep(void);

/* 0x4fc6f0, ages the character c to the age: its life f01c becomes f018 when the age is not above its f020,
   else f018 run through v = f014 / v + (v - f014) once per year above f020; the change goes out as a float
   delta of the field 0x1c, the age as the delta of w00a (0xa). */
void gm_AgeCharacter(char *c, unsigned short age)
{
    ORDCHAR *ch;
    float a, k, v;
    int n;

    ch = ORD_CHAR(c);
    if (ch != 0) {
        a = (float)age;
        v = ch->f018;
        if (a <= ch->f020) {
            cm_SendFloatDelta(ch->netId, 0x1c, v - ch->f01c);
        } else {
            k = ch->f014;
            n = (int)(a - ch->f020);
            if (n > 0) {
                do {
                    n--;
                    v = k / v + (v - k);
                } while (n != 0);
            }
            cm_SendFloatDelta(ch->netId, 0x1c, v - ch->f01c);
        }
        cm_BeginFieldBlock(c, ch->netId);
        cm_AddFieldDelta(2, 1, 10, &age);
        cm_SendFieldDeltas();
        gm_NetMsgCount++;
    }
}
/* 0x4fc7a0, the age the character c dies at: its age w00a plus the number of aging steps
   (v = f014 / v + (v - f014), as gm_AgeCharacter) its life f01c takes to fall to 1.0, plus the years it still
   has below f020 (none when it is older). */

int gm_ExpectedDeathAge(char *c)
{
    ORDCHAR *ch = ORD_CHAR(c);
    float k, v, r;
    int n;

    n = 0;
    v = ch->f01c;
    k = ch->f014;
    while (v > 1.0f) {
        v = k / v + (v - k);
        n++;
    }
    r = ch->f020 - (float)ch->w00a;
    if (!(r > 0.0f))
        r = 0.0f;
    return (int)((float)(ch->w00a + n) + r);
}
/* 0x4fc820, the life of the character c now (-1 for a free slot or a dead one): f01c once its age w00a is
   above f020, before that the line from f010 to f01c over the years up to f020; plus v024, not below 0 */

float gm_CharacterHealth(short *c)
{
    ORDCHAR *ch = ORD_CHAR(c);
    float a, f;

    if (ch->id == 0xffff)
        return -1.0f;
    if (ch->health == 0)
        return -1.0f;
    a = (float)ch->w00a;
    if (a <= ch->f020)
        f = ((ch->f01c - ch->f010) / ch->f020) * a + ch->f010;
    else
        f = ch->f01c;
    f = f + ch->v024;
    return f > 0.0 ? f : 0.0;
}
/* 0x4fc890, the life of the character c by its age alone: gm_CharacterHealth without v024 and the floor (-1 for
   a free slot or a dead one) */

float gm_CharacterBaseHealth(short *c)
{
    ORDCHAR *ch = ORD_CHAR(c);
    float a;

    if (ch->id == 0xffff)
        return -1.0f;
    if (ch->health == 0)
        return -1.0f;
    a = (float)ch->w00a;
    if (a <= ch->f020)
        return ((ch->f01c - ch->f010) / ch->f020) * a + ch->f010;
    return ch->f01c;
}
/* 0x4fc8e0, the share of its life the character c has now: gm_CharacterHealth / gm_CharacterBaseHealth (0 for a
   free slot or a dead one) */

float gm_HealthRatio(short *c)
{
    ORDCHAR *ch = ORD_CHAR(c);

    if (ch->id == 0xffff)
        return 0.0f;
    if (ch->health == 0)
        return 0.0f;
    return gm_CharacterHealth(c) / gm_CharacterBaseHealth(c);
}
/* 0x4fc920, the daily step of all characters: every kind 0x12 character gets the kind byte 8 (a field delta of
   +2); then every active character of a kind below 10 or 0x14 runs gm_DailyCharacterStep, and one whose age
   w00a has reached f020 - 1 and that has a model gets the model "stallbursche_MANN" / "magd_FRAU" by its sex;
   then the population step */

void gm_DailyCharacters(void)
{
    ORDCHAR *c;
    int i, v;

    gm_NetMsgCount = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
        if (ai_Chars[i][2] == 0x12) {     /* c->kind, read through the record */
            v = 8;
            cm_BeginFieldBlock(ai_Chars[i], c->netId);
            cm_AddFieldDelta(1, 1, 2, &v);
            cm_SendFieldDeltas();
            gm_NetMsgCount++;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (gm_NetMsgCount > 0x40) {
            sv_PumpNet();
            gm_NetMsgCount = 0;
        }
        if ((c->kind < 10 || c->kind == 0x14) && c->health != 0) {
            gm_DailyCharacterStep((unsigned short *)c);
            v = c->w00a;
            if ((float)v == c->f020 - 1.0f && c->model != 0) {
                if (c->textVariant == 0)
                    cm_SendCharModel(c->netId, "stallbursche_MANN");
                else
                    cm_SendCharModel(c->netId, "magd_FRAU");
                gm_NetMsgCount++;
            }
        }
    }
    gm_PopulationStep();
    gm_NetMsgCount = 0;
}
/* 0x4fca40, changes the life of the character id by d and returns the new life value (-1 for a free slot, a
   kind 10 character or a dead one; 0 when the change kills it: health 0 and the death message 0x72). A raised
   dead person (kind 0x14) has its life f01c moved by d; every other character has v024 moved by d, its life
   being f01c (or the line from f010 over the years up to f020) plus v024, not below 0 */

float gm_AddHealth(unsigned short id, int d)
{
    GMMSG m;
    unsigned char b;
    ORDCHAR *c;
    float a, f;

    c = &ORD_CHARS[id];
    if (c->id == 0xffff || c->kind == 10 || c->health == 0)
        return -1.0f;
    if (c->kind != 0x14) {
        a = (float)c->w00a;
        if (a <= c->f020)
            f = ((c->f01c - c->f010) / c->f020) * a + c->f010;
        else
            f = c->f01c;
        f = (float)c->v024 + d + f;
        f = f > 0.0 ? f : 0.0;
        if (f <= 0.0) {
            b = 0;
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValue(1, 1, (char *)&c->health - cm_FieldObject, &b);
            cm_SendFieldValues();
            GM_SENDDEATH(m, c);
            return 0.0f;
        }
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValues(4, 1, 0x24, &d);
        cm_SendFieldDeltas();
        return f;
    }
    a = (float)d;
    f = c->f01c;
    f += a;
    if (f <= 0.0) {
        b = 0;
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValue(1, 1, (char *)&c->health - cm_FieldObject, &b);
        cm_SendFieldValues();
        GM_SENDDEATH(m, c);
        return 0.0f;
    }
    cm_SendFloatDelta(c->netId, 0x1c, a);
    return f;
}
/* 0x4fd090, 1 when the character c may be removed: it is no player (kind 6 / 7), no other player has it as one
   of the relatives[0..2] (the partner and the parents), and no other player with fewer than
   5 children has it among the children (relatives[3..7]) */

int gm_IsNoPlayerKin(ORDCHAR *c)
{
    ORDCHAR *p;
    int id, i, k, n, found;

    if (c->kind == 6 || c->kind == 7)
        return 0;
    id = c->netId;
    for (i = 0; i < 0x300; i++) {
        p = &ORD_CHARS[i];
        found = 0;
        n = 0;
        if (p->netId != id && (p->kind == 6 || p->kind == 7)) {
            if (p->relatives[0] == id || p->relatives[1] == id || p->relatives[2] == id)
                return 0;
            for (k = 3; k < 8; k++) {
                if (p->relatives[k] == id) {
                    found |= 1;
                    n++;
                } else if (p->relatives[k] != -1)
                    n++;
            }
            if (found && n < 5)
                return 0;
        }
    }
    return 1;
}

extern float gm_BirthChance[6];
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);                     /* 0x4c4650 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
int cm_GetSlotValue(int h);                     /* 0x4c3b10 */
int cm_SendNewParty(void *p);                     /* 0x4c4fa0 */
int gm_TitleNumber(int group, int rank);                     /* 0x5043d0 */
int ai_WorksForMaster(struct AICHAR *c);                     /* 0x47dd70 */

/* 0x4fccc0, the birth step of the character c, a woman (sex 1, kind below 10, alive, b1b1 0, no flag 0x40000)
   with the partner o: a pregnancy above 1.0 is reset below 0 and, with the partner there, a child (kind 8) of
   the two is created at the house of the father or the mother; when one of the parents is a player (kind 6 / 7)
   the child is made kind 0x12 and gets a baptism party (kind 7, at 8 .. 11 o'clock the next 8 hours on) held
   by that parent. Otherwise, the partner alive and c at most 38 years old, a new pregnancy value is drawn from
   gm_BirthChance by the number of c's children, unless a child is a player or a kind 5 character, c has 4
   children or the player partner has 5
   @deep read in full against the original and reconstructed (relatives[3..7] indexing, the party G28REC,
   util_RandomF, b1b1 > 0, w00a > 0x26, the member forwarded from party.members[1]); statement structure
   and branch targets equal, size 971 = 971. Left: (1) frame 0x12c vs 0x128: the original packs the spill
   of the child count n (ebx, stored at +0x2e1 / reloaded at +0x372) and the float temp of the last
   cm_SendFloatDelta into the slot of the address-taken arg ([esp+0x10]); ours gives arg its own slot
   [esp+0x14] (tried: n as arg itself, fv inlined, a separate delta variable: all worse); (2) regs:
   c/o/mate rotate ebp<->ebx through the whole function (original keeps 0 in ebx as mate's start and c in
   edi, reloading c from the parameter in the second part); (3) the party fill hunk at +0x23f: the same 13
   stores, the time words 8/0xc and host/members[0] scheduled around the two pushes (tried 11 statement
   orders, this is the closest) */
void gm_PartnerStep(ORDCHAR *c)
{
    G28REC party;
    AITIME clk;
    ORDCHAR *o, *mate, *child;
    int arg, t, kids, n, m, k, h;
    float fv;
    char kind;

    if (c->kind >= 10 || c->health == 0 || c->textVariant != 1 || c->b1b1 > 0 || (c->flags1c8 & 0x40000) != 0)
        return;
    o = ORD_CHAR(gm_FindChar(c->partner));
    if (c->pregnancy > 1.0f) {
        cm_SendFloatDelta(c->netId, 0x7c, util_RandomF() * -0.5f - c->pregnancy);
        gm_NetMsgCount++;
        mate = 0;
        if (o == 0)
            return;
        clk = ai_TimeT;
        arg = 2;
        gm_AddTime((struct GCLOCK *)&clk, 8, 0, 0);
        clk.hour = util_Random(4) + 8;
        clk.min = 0;
        t = c->bld2 != 0 ? c->bld2->id : -1;
        if (o->kind == 6 || o->kind == 7) {
            kids = 0;
            for (k = 3; k < 7; k++)
                if (o->relatives[k] != -1)
                    kids++;
            if (kids == 0)
                arg = o->textVariant;
            if (o->bld2 != 0)
                t = o->bld2->id;
            else
                t = c->bld2 != 0 ? c->bld2->id : -1;
            mate = o;
        } else if (c->kind == 6 || c->kind == 7) {
            kids = 0;
            for (k = 3; k < 7; k++)
                if (c->relatives[k] != -1)
                    kids++;
            if (kids == 0)
                arg = c->textVariant;
            if (c->bld2 != 0)
                t = c->bld2->id;
            else
                t = o->bld2 != 0 ? o->bld2->id : -1;
            mate = c;
        }
        h = cm_SendCreateCharacter(8, o->netId, c->netId, 0, t, 0, 0, arg);
        while (cm_GetSlotState(h) == 0)
            sv_PumpNet();
        if (cm_GetSlotState(h) == 2)
            return;
        child = ORD_CHAR(cm_GetSlotValue(h));
        if (mate != 0) {
            memset(&party, 0, sizeof(party));
            arg = 0x12;
            party.host = mate->netId;
            party.time = clk;
            party.members[0] = party.host;
            party.members[1] = child->netId;
            party.kind = 7;
            party.flags = 0xa0;
            party.thing = -1;
            party.used = 2;
            cm_BeginFieldBlock((char *)child, party.members[1]);
            cm_AddFieldDelta(1, 1, 2, &arg);
            cm_SendFieldDeltas();
            cm_SendNewParty(&party);
            gm_NetMsgCount++;
            return;
        }
    }
    if (o == 0 || o->health == 0 || c->w00a > 0x26)
        return;
    n = 0;
    for (k = 3; k < 8; k++) {
        if (c->relatives[k] != -1) {
            kind = ORD_CHAR(gm_FindChar(c->relatives[k]))->kind;
            if (kind == 6 || kind == 7 || kind == 5)
                return;
            if (kind != 0xf)
                n++;
        }
    }
    if (n == 4)
        return;
    if (o->kind == 6 || o->kind == 7) {
        m = 0;
        for (k = 3; k < 8; k++) {
            if (o->relatives[k] != -1) {
                kind = ORD_CHAR(gm_FindChar(o->relatives[k]))->kind;
                if (kind == 6 || kind == 7 || kind == 5)
                    return;
                if (kind != 0xf)
                    m++;
            }
        }
        if (m == 5)
            return;
    }
    fv = (util_RandomF() + 1.0f) * gm_BirthChance[n];
    cm_SendFloatDelta(c->netId, 0x7c, fv);
    gm_NetMsgCount++;
}
/* 0x4fd120, the yearly population step. n counts the characters in use. A child (kind 3) without a profession or an
   office applied for that gm_IsNoPlayerKin lets go is removed (its whole life taken) when it is old enough (44 + random 4
   years) or when n has reached 652.8. Then, for every character with health above 1: over 652.8 characters a
   kind 8 one whose parents (relatives[1], [2]) are gone (none or kind 0xf) is removed the same way; else a
   grown-up (gm_IsUnderAge 0) kind 8 one with both parents gone becomes kind 3 with level 2 (if lower) and gets
   (random 0x800 + 0x800) * 2 * 32; every other one that works for a master counts as settled (and in kids when
   its v1f0 is set). Fewer than 6 of those: one time in 4 a newcomer of kind 0x10 arrives with
   (random 0x2000 + 10000) * 64. Up to 652.8 characters, (40 - settled) / 2 (below 30 settled; below 40: 1 .. 2)
   adults of kind 3 arrive with a random title, (random 0x400 + 0x400) * 64 and prevOffice 10 .. 21. */

void gm_PopulationStep(void)
{
    ORDCHAR *c, *a, *b, *nc;
    int settled, n, kids, i, t, h;
    int v, level;

    settled = 0;
    kids = 0;
    n = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->health != 0)
            n++;
    }
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->health != 0 && c->kind == 3
            && (c->w00a >= util_RandomU(4) + 0x2c || !((float)n < 652.80005f))
            && c->job == 0 && c->appliedOffice == 0 && c->job2 == 0 && gm_IsNoPlayerKin(c) != 0) {
            gm_AddHealth(c->id, (int)-(c->f01c + 1.0f));
            n--;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id == 0xffff || c->health <= 1)
            continue;
        if ((float)n > 652.80005f && c->kind == 8) {
            a = ORD_CHAR(gm_FindChar(c->relatives[1]));
            b = ORD_CHAR(gm_FindChar(c->relatives[2]));
            if ((a == 0 || a->kind == 0xf) && (b == 0 || b->kind == 0xf)) {
                if (gm_IsNoPlayerKin(c) != 0) {
                    gm_AddHealth(c->id, (int)-(c->f01c + 1.0f));
                    n--;
                }
                continue;
            }
        }
        if (c->kind == 8 && gm_IsUnderAge(c->id) == 0) {
            a = ORD_CHAR(gm_FindChar(c->relatives[1]));
            b = ORD_CHAR(gm_FindChar(c->relatives[2]));
            if ((a == 0 || a->kind == 0xf) && (b == 0 || b->kind == 0xf)) {
                v = 3;
                level = 2;
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldValue(1, 1, (char *)&c->kind - cm_FieldObject, &v);
                if (c->level < 2)
                    cm_AddFieldValue(1, 1, (char *)&c->level - cm_FieldObject, &level);
                cm_SendFieldValues();
                cm_SendMoveGoodsUnchecked(c->netId, -1, ai_OwnPlayer, (util_RandomU(0x800) + 0x800) * level * 0x20);
                settled++;
            }
            continue;
        }
        if (ai_WorksForMaster((struct AICHAR *)c) != 0) {
            settled++;
            if (c->v1f0 != 0)
                kids++;
        }
    }
    if (kids < 6 && util_RandomU(4) == 0) {
        h = cm_SendCreateCharacter(0x10, -1, -1, util_Random(4) + 0x10, -1, 0, 0, 0);
        while (h != -1 && cm_GetSlotState(h) == 0)
            sv_PumpNet();
        if (cm_GetSlotState(h) == 1) {
            settled++;
            nc = ORD_CHAR(cm_GetSlotValue(h));
            cm_SendMoveGoodsUnchecked(nc->netId, -1, ai_OwnPlayer, (util_RandomU(0x2000) + 10000) * 0x40);
        }
    }
    if (!((float)n > 652.80005f)) {
        t = settled < 0x1e ? (0x28 - settled) / 2 : settled < 0x28 ? util_RandomU(2) + 1 : 0;
        for (i = 0; i < t; i++) {
            h = cm_SendCreateCharacter(3, -1, -1, 0x14, -1, gm_TitleNumber(util_RandomU(0xc) + 1, util_RandomU(2) + 1), 0, 2);
            while (h != -1 && cm_GetSlotState(h) == 0)
                sv_PumpNet();
            if (cm_GetSlotState(h) == 1) {
                v = util_RandomU(0xc) + 10;
                nc = ORD_CHAR(cm_GetSlotValue(h));
                cm_SendMoveGoodsUnchecked(nc->netId, -1, ai_OwnPlayer, (util_RandomU(0x400) + 0x400) * 0x40);
                cm_BeginFieldBlock((char *)nc, nc->netId);
                cm_AddFieldValue(1, 1, (char *)&nc->prevOffice - cm_FieldObject, &v);
                cm_SendFieldValues();
            }
        }
    }
}

extern char d2_GfxDir[256];
/* txt_MaleFirstNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FemaleFirstNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FamilyNames: view of txt_Ptrs (build/mk_txt_views.py) */
int gm_HomeSearchStart = 0;  /* 0x6f1bb4 */
extern char gm_CharacterStatistics[128];
char * ai_Unk60c1ec = (void *)((unsigned)&he_Events + 0xc234);  /* 0x60c1ec, the memory pool tag */
unsigned short gm_CreateCharacter(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8);                     /* 0x507690 */
int gm_SexForJob(int x);                   /* 0x503f30 */
int gm_SexForTitle(int x);                   /* 0x503f50 */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
float gm_CharAge(unsigned short id);  /* 0x505e30 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void mem_Free(void *p);                    /* 0x422620 */

/* 0x4fd5c0, creates the child (aged age) of two random parents and returns its id (0 when no parents are
   found). A parent is a character of a kind below 6, alive and grown up (w00a not below f020), taken with the
   chance chance[its number of children] in 256; each parent gets 16 draws, the range of the draw shrinking by
   a quarter on every free slot hit */
unsigned short gm_CreateRandomChild(int age)
{
    int chance[6] = { 0x60, 0x4b, 0x28, 0x17, 9, 0 };
    int tries, n, k, range;
    unsigned short a, b;

    range = 0x300;
    for (tries = 0; tries < 0x10; tries++) {
        a = util_RandomU(range);
        if (ORD_CHARS[a].id == 0xffff)
            range = range * 3 / 4;
        else if (ORD_CHARS[a].kind < 6 && !((float)ORD_CHARS[a].w00a < ORD_CHARS[a].f020) && ORD_CHARS[a].health != 0) {
            n = 0;
            for (k = 3; k < 8; k++)
                if (ORD_CHARS[a].relatives[k] != -1)
                    n++;
            if (util_RandomU(0x100) + 1 < chance[n])
                break;
        } else
            a = 0xffff;
    }
    if (a == 0xffff)
        return 0;
    for (tries = 0; tries < 0x10; tries++) {
        b = util_RandomU(range);
        if (ORD_CHARS[b].id == 0xffff)
            range = range * 3 / 4;
        else if (ORD_CHARS[b].kind < 6 && !((float)ORD_CHARS[b].w00a < ORD_CHARS[b].f020) && ORD_CHARS[b].health != 0) {
            n = 0;
            for (k = 3; k < 8; k++)
                if (ORD_CHARS[b].relatives[k] != -1)
                    n++;
            if (util_RandomU(0x100) + 1 < chance[n])
                break;
        } else
            b = 0xffff;
    }
    if (b == 0xffff)
        return 0;
    return gm_CreateCharacter(0, ORD_CHARS[a].netId, ORD_CHARS[b].netId, age, 0, 0, 0, 2);
}
/* 0x4fd7e0, 1 when the character id is younger than its lifetime float (+0x20): age (+10) below it */

int gm_IsUnderAge(unsigned short id)
{
    if ((float)*(unsigned short *)(ai_Chars[id] + 10) < *(float *)(ai_Chars[id] + 0x20))
        return 1;
    return 0;
}
/* 0x4fd820, finds a home with room for the building o's people: among the buildings of the query (1, 5, 1) with
   fewer people (+0x65) than their kind's capacity (ord_GetKindRecord [0]) and the flag 0x5a bit 0 clear, the
   nearest to o when o and the building both have a scene object, else the least full one (people / capacity);
   none: a random one of the first 16 of the query (1, 5, 0x16) with the flag clear. The first parameter is
   not read
   @deep read in full against the original and reconstructed (ORDBLD people 0x65 / flags5a / object as
   D3OBJECT, the 16-byte D3VECTOR4 positions subtracted in place and summed z, y, x, the ratio compare, the
   fallback list of 16 under if (pick == 0) as the original's block order shows); same statements, branch
   targets and calls, 160 = 160 instructions. Left is allocation: the original keeps the pick in ebp, the
   pick's capacity (pickKind[0]) promoted to ebx and the kind's capacity in edi, rereads o from the
   parameter, and overlaps the int-to-float conversion temp with pickKind[0]'s slot (frame 0x74); ours
   keeps the constant 1 in ebx and o in ebp, pickKind in memory and the temp in its own slot (frame 0x78),
   so every code / regs / slots hunk is one of these registers or slots (tried: pickKind as an 8-byte
   struct copy instead of the array: identical code; o as a local copy of a char * parameter: identical) */

ORDBLD *gm_FindHome(int unused, ORDBLD *o)
{
    float best, d;
    ORDBLD *b, *pick;
    int pickKind[2], kind[2];
    D3VECTOR4 v1, v2;                   /* the engine's 16-byte vectors */
    ORDBLD *list[16];
    int n;

    pick = 0;
    best = 1e+08f;
    ord_GetKindRecord(1, pickKind);
    b = (ORDBLD *)gm_BuildQuery(1, 5, 1);
    if (b != 0) {
        do {
            ord_GetKindRecord(b->type, kind);
            if ((unsigned short)b->people < (unsigned short)kind[0] && (b->flags5a & 1) == 0) {
                if (o != 0 && o->object != 0 && b->object != 0) {
                    d3_PointToWorld(b->object, &b->object->pos, (D3VECTOR *)&v1);
                    d3_PointToWorld(o->object, &o->object->pos, (D3VECTOR *)&v2);
                    v1.x -= v2.x;
                    v1.y -= v2.y;
                    v1.z -= v2.z;
                    d = (float)sqrt(v1.z * v1.z + v1.y * v1.y + v1.x * v1.x);
                    if (d < best) {
                        pickKind[0] = kind[0];
                        pickKind[1] = kind[1];
                        pick = b;
                        best = d;
                    }
                } else if (pick == 0) {
                    pickKind[0] = kind[0];
                    pickKind[1] = kind[1];
                    pick = b;
                } else if ((float)b->people / (unsigned short)kind[0] < (float)pick->people / (unsigned short)pickKind[0])
                    pick = b;
            }
            b = (ORDBLD *)gm_BuildNext();
        } while (b != 0);
    }
    if (pick == 0) {
        b = (ORDBLD *)gm_BuildQuery(1, 5, 0x16);
        memset(list, 0, sizeof(list));
        for (n = 0; b != 0 && (b->flags5a & 1) == 0 && n < 0x10; n++) {
            list[n] = b;
            b = (ORDBLD *)gm_BuildNext();
        }
        return list[util_RandomU(n)];
    }
    return pick;
}
/* 0x4fda10, moves the character c into the building b (+0x170) when b (+0x65 its occupancy) has room (0x4f8890 capacity): the old building loses one */

void unk_Unk4fda10(char *c, char *b)
{
    unsigned short cap[4];
    char *old;

    ord_GetKindRecord(*(unsigned char *)b, (int *)cap);
    if ((unsigned char)b[0x65] < cap[0]) {
        old = *(char **)(c + 0x170);
        if (old != 0 && (unsigned char)old[0x65] > 0)
            old[0x65]--;
        *(char **)(c + 0x170) = b;
        b[0x65]++;
    }
}
/* 0x4fda70, finds the applicant the building o hires for the job (and, when title is not 0, the title): a
   character in use, not yet taken (flags1c8 0x2000), of kind 0 / 1, with the job, no building, of the sex the
   job / title needs (gm_SexForJob / gm_SexForTitle, 2 = either), with the title or none and grown up (w00a
   not below f020) when a title is asked for, and one the master of o (its owner, 0xffff = none) does not
   dislike (gm_Opinion above -30); marks it taken and returns it, 0 when there is none (or neither title nor
   job is given) */

ORDCHAR *gm_FindApplicant(ORDBLD *o, unsigned title, unsigned job)
{
    int i, sex;

    if (title != 0) {
        for (i = 0; i < 0x300; i++) {
            if (ORD_CHARS[i].id != 0xffff && (ORD_CHARS[i].flags1c8 & 0x2000) == 0 && ORD_CHARS[i].kind <= 1
                && ((unsigned char)ORD_CHARS[i].title == title || ORD_CHARS[i].title == 0) && ORD_CHARS[i].bld == 0
                && (float)ORD_CHARS[i].w00a >= ORD_CHARS[i].f020 && ORD_CHARS[i].jobId == job) {
                sex = gm_SexForTitle(title);
                if ((sex == 2 || ORD_CHARS[i].textVariant == sex)
                    && (o->owner == 0xffff || gm_Opinion(o->owner, i) > -0x1e))
                    goto found;
            }
        }
    } else if (job != 0) {
        sex = gm_SexForJob((unsigned char)job);
        for (i = 0; i < 0x300; i++) {
            if (ORD_CHARS[i].id != 0xffff && (ORD_CHARS[i].flags1c8 & 0x2000) == 0 && ORD_CHARS[i].kind <= 1 && ORD_CHARS[i].jobId == job && ORD_CHARS[i].bld == 0
                && (sex == 2 || ORD_CHARS[i].textVariant == sex)
                && (o->owner == 0xffff || gm_Opinion(o->owner, i) > -0x1e))
                goto found;
        }
    }
    return 0;
found:
    ORD_CHARS[i].flags1c8 |= 0x2000;
    return &ORD_CHARS[i];
}
/* 0x4fdc10, the object of the home building b where an inhabitant is placed on stage (reasoned: the callers place characters there): by the kind (kind 1: 0x12, 2: 0x13, 0x1d: 0xfd, 0x16: 0x120), through 0x4ffe90 (its object id, 1, 0, n) */

char *gm_HomeObject(unsigned char *b)
{
    ORDBLD *bld = (ORDBLD *)b;
    char *o = 0;
    char *type;

    type = ai_BTypes + bld->type * 0x24d;
    if (*type == 1)
        o = gm_Query((void *)bld->objList, 1, o, 0x12);
    else if (*type == 2)
        o = gm_Query((void *)bld->objList, 1, 0, 0x13);
    else if (*type == 0x1d)
        o = gm_Query((void *)bld->objList, 1, 0, 0xfd);
    else if (*type == 0x16)
        o = gm_Query((void *)bld->objList, 1, 0, 0x120);
    return o;
}
/* 0x4fdcb0, the home of the character c: its own building bld2 with the object of gm_HomeObject (out3 = its id, -1 without) and out2 = the building id; without one the next building of ai_Blds (round-robin from gm_HomeSearchStart) of kind 3 / 0x16 / 0x0f. Returns the building, 0 when none */

char *gm_CharacterHome(char *c, int *out2, int *out3)
{
    ORDCHAR *ch = (ORDCHAR *)c;
    ORDOBJ *o;
    char *type;
    int i;
    int n;

    n = 0xff;
    if (ch->bld2 != 0) {
        o = (ORDOBJ *)gm_HomeObject((unsigned char *)ch->bld2);
        if (o != 0)
            *out3 = o->objId;
        else
            *out3 = -1;
        *out2 = ch->bld2->id;
        return (char *)ch->bld2;
    }
    i = gm_HomeSearchStart;
    for (; n != 0; n--) {
        if (((ORDBLD *)ai_Blds)[i].type != 0) {
            type = ai_BTypes + ((ORDBLD *)ai_Blds)[i].type * 0x24d;
            if (*type == 3 || *type == 0x16 || *type == 0x0f) {
                gm_HomeSearchStart = i + 1;
                *out3 = -1;
                *out2 = ((ORDBLD *)ai_Blds)[i].id;
                return (char *)&((ORDBLD *)ai_Blds)[i];
            }
        }
        i = (i + 1) % 256;
    }
    return 0;
}
/* 0x4fddb0, creates a character: flags bit 0 = gm_CreateRandomChild(age), else gm_CreateCharacter(type, -1, -1, age, 0, job, title, 2); bit 1 (random child only): sets its jobId / title; bit 2: places it on the stage of the building bld at its gm_HomeObject. Returns the character or 0
   @deep statements read against the original 0x4fddb0 and reconciled (random-child / gm_CreateCharacter
   branches, the 0xffff checks, title then jobId stores, the flags&4 home-object placement); every
   remaining asmdiff line is callee-saved register placement: the original pushes edi in the prologue (+0x8
   'push edi', 'pop edi' before the shared return 0) and esi only inside the flags&4 branch (+0x50 'push
   esi' / +0x7e 'pop esi'), ours pushes esi+edi together after the 0xffff check (+0x3c/+0x3d); the [esp+N]
   differences are the shift that follows. Tried: combined vs nested flags&4 condition, unsigned short id,
   local declaration orders, no c temporary (worse) */

short *gm_SpawnCharacter(unsigned char flags, int type, int age, unsigned char job, unsigned char title, char *bld)
{
    unsigned id;
    ORDCHAR *c;
    ORDOBJ *o;

    if (flags & 1) {
        id = gm_CreateRandomChild(age);
        id &= 0xffff;
        if (id != 0xffff && (flags & 2)) {
            ORD_CHARS[id].title = title;
            ORD_CHARS[id].jobId = job;
        }
    } else {
        id = gm_CreateCharacter(type, -1, -1, age, 0, job, title, 2);
        id &= 0xffff;
    }
    if (id == 0xffff)
        return 0;
    c = &ORD_CHARS[id];
    if ((flags & 4) && bld != 0 && (o = (ORDOBJ *)gm_HomeObject((unsigned char *)bld)) != 0)
        gm_PlaceOnStage(c->netId, ((ORDBLD *)bld)->id, o->objId, 0, 0);
    return (short *)c;
}
/* 0x4fde90, loads the icon names: counts the 0x80-byte records of "<root>Character_Names.dat" (0x6f1b90 of them), allocates 0x30 bytes per name (0x6f1b94) and copies each name; resets the three flag bytes at 0x6f0454 */

void unk_LoadIconNames(void)
{
    char path[260], rec[260];
    char *s, *d;
    FILE *f;
    char ch;

    ai_Unk6f0454[1] = (char)0xff;
    ai_Unk6f0454[0] = 0;
    ai_Unk6f0454[2] = 0;
    gm_IconNameCount = 0;
    sprintf(path, "%sCharacter_Names.dat", d2_GfxDir);
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            gm_IconNameCount++;
        } while ((f->_flag & 0x10) == 0);
        fclose(f);
    }
    gm_IconNames = (char *)mem_Alloc(gm_IconNameCount * 0x30, &ai_Unk60c1ec);
    gm_IconNameCount = 0;
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            s = rec;
            d = gm_IconNames + gm_IconNameCount * 0x30;
            gm_IconNameCount++;
            do {
                ch = *s++;
                *d++ = ch;
            } while (ch != 0);
        } while ((f->_flag & 0x10) == 0);
        fclose(f);
    }
}
/* 0x4fdfb0, frees the icon names */

void gm_FreeIconNames(void)
{
    if (gm_IconNames != 0) {
        mem_Free(gm_IconNames);
        gm_IconNames = 0;
    }
}
#define GM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define GM_MAX(a, b) ((a) > (b) ? (a) : (b))

/* 0x4fdfd0, fills the character statistics gm_CharacterStatistics: the living persons of kind < 10, per age bucket (gm_CharAge / 10, clamped to 0 .. 7) their count, health in percent of the base health and average base health, and the share under age in percent
   @deep whole function read against the original 0x4fdfd0 and reconciled (the [0, 7] clamp of
   gm_CharAge*0.1 as MAX(MIN(..)) with its four calls, the per-age-bucket health / base health sums indexed
   by the bucket, the averaging loop, the index loop strength-reduced to the original's pointer + jl); the
   one remaining hunk is the addressing of baseHealth[k]: the original reuses k's dead register, '+ lea
   edx, [edx*4 + 0x91b548]' before the call then 'fadd/fstp dword ptr [edx]', ours keeps 'fadd/fstp dword
   ptr [edx*4 + X]' (same memory, 3 bytes). Tried: += vs x = f() + x, a pointer temporary, (&health[k])[8],
   k unsigned / char (worse) */

void gm_CountCharacters(void)
{
    GMCHARSTATS *st = (GMCHARSTATS *)gm_CharacterStatistics;
    ORDCHAR *c;
    int i, k;

    memset(gm_CharacterStatistics, 0, sizeof(GMCHARSTATS));
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->health > 0 && c->kind < 10) {
            st->count++;
            k = (int)GM_MAX(GM_MIN(gm_CharAge(i) * 0.1f, 7.0f), 0.0f);
            st->ageCount[k]++;
            if (gm_IsUnderAge(i) != 0)
                st->underAge += 1.0f;
            st->health[k] += gm_CharacterHealth((short *)c);
            st->baseHealth[k] += gm_CharacterBaseHealth((short *)c);
        }
    }
    for (k = 0; k < 8; k++) {
        st->health[k] = st->health[k] / st->baseHealth[k] * 100.0f;
        st->baseHealth[k] = st->baseHealth[k] / (float)st->ageCount[k];
    }
    st->underAge = st->underAge / (float)st->count * 100.0f;
}

#pragma pack(push, 1)
typedef struct GCLOCK {                 /* the 14-byte game clock (0x770ae0) */
    int day;
    unsigned short hour;
    int min;
    int sec;
} GCLOCK;
#pragma pack(pop)
#define GM_NOW (*(GCLOCK *)&ai_Time)  /* 0x770ae0, the game clock ai_Time as a GCLOCK */

/* a job record (ai.h AIJOB, he_NextEvent's list) as gm_SellBuilding stamps it */
#pragma pack(push, 1)
typedef struct GMJOB {
    char pad00[0x52];
    GCLOCK end;                         /* 0x52, the end time (ai.h AIJOB.end) */
    char pad60[0x24];
    int msgSlot;                        /* 0x84, the slot handle of its HE message (cm_SendExSetHE) */
} GMJOB;
#pragma pack(pop)

extern int gm_ClassBuilding[32];
extern int gm_GoodBuilding[732];
extern char * ai_Goods;
extern unsigned gm_OwnNameHeads[136];
extern struct AIOBJ *ai_Objects;
extern int gm_DefaultStock[];
extern char gm_TagGebaeudePROT[], gm_TagGebaeudeINST[], gm_TagObjektePROT[], gm_TagObjekteINST[];  /* 0x61aa38, 0x61aa20, 0x61aa00, 0x61a9ec */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void util_Randomize();                     /* 0x42ec80 */

/* 0x4fe140, adds hours / minutes / seconds (any sign for the hours) to the clock c, carrying into the days (the minutes and seconds only upward)
   @deep read against the original 0x4fe140 and reconciled: the read-modify-write form of sec / min / hour
   (the original keeps the intermediate store c->min = min + sec/60, which only the compound form
   reproduces), h signed with unsigned /24 and h -= d*24 (the mul by 0xaaaaaaab), the negative-hour borrow.
   Remaining is register allocation only: the original holds the new minute and the day in edi (push edi;
   '+ mov edi, [ecx+6]; add edi, eax; mov [ecx+6], edi', '+ mov edi, [ecx]; add edi, edx; mov [ecx], edi',
   pop edi in both returns), ours uses esi then reloads 'mov esi, 0x3c' and does the day updates through
   eax; ours=55 vs original=57 are the push/pop edi. Tried: separate t temporary, secs += c->sec, %24
   instead of -= d*24 (worse) */
void gm_AddTime(GCLOCK *c, int hours, int mins, int secs)
{
    int h;
    int d;

    c->sec += secs;
    c->min += c->sec / 60;
    c->sec %= 60;
    c->min += mins;
    c->hour += c->min / 60;
    c->min %= 60;
    h = c->hour + hours;
    if (h >= 24) {
        d = (unsigned)h / 24;
        c->day += d;
        h -= d * 24;
    }
    if (h < 0) {
        d = (unsigned)(-1 - h) / 24 + 1;
        c->day -= d;
        c->hour = h + d * 24;
    } else
        c->hour = h;
}
/* 0x4fe1e0, sets the time of day of the clock c (hour, minute, second) */

void gm_SetTime(GCLOCK *c, unsigned char hour, unsigned min, unsigned sec)
{
    c->sec = sec & 0xff;
    c->hour = hour;
    c->min = min & 0xff;
}
/* 0x4fe210, compares two clocks: -1 when a is earlier, 1 when later, 0 when equal */

int gm_CompareTime(GCLOCK *a, GCLOCK *b)
{
    int x, y;

    if (a->day < b->day)
        return -1;
    if (a->day > b->day)
        return 1;
    x = a->sec + (a->min + a->hour * 60) * 60;
    y = b->sec + (b->min + b->hour * 60) * 60;
    if (x < y)
        return -1;
    return x > y;
}
/* 0x4fe280, the minutes from clock a to clock b (days, hours and minutes; the seconds are not used) */

int gm_MinutesBetween(GCLOCK *a, GCLOCK *b)
{
    return ((((b->day - a->day) * 0x18 - a->hour) + b->hour) * 0x3c - a->min) + b->min;
}
/* 0x4fe2c0, converts the clock c into the calendar record out: byte 0 = 1, byte 1 = season (day % 4) * 3 + 1, word at +2 = year (day + 0x578), byte 4 = hour, byte 5 = minute, int at +6 = the seconds field */

void gm_ClockToDate(GCLOCK *c, unsigned char *out)
{
    int r;

    *(short *)(out + 2) = (short)c->day + 0x578;
    r = c->day % 4;
    out[0] = 1;
    out[1] = (char)r * 3 + 1;
    out[4] = (char)c->hour;
    out[5] = (char)c->min;
    *(int *)(out + 6) = c->sec;
}
/* 0x4fe310, a forward to 0x4fe140 with the first two arguments left out (the second is unused) */

void gm_AddTime2(GCLOCK *c, int unused, int hours, int mins, int secs)
{
    gm_AddTime(c, hours, mins, secs);
}
/* 0x4fe330, the season (word at +2 modulo 4) of a calendar record */

int gm_Season(char *d)
{
    return *(short *)(d + 2) % 4;
}
/* 0x4fe350, the season of the clock c (day % 4) */

int gm_ClockSeason(GCLOCK *c)
{
    return c->day % 4;
}
/* 0x4fe370, builds the building lookup tables: gm_ClassBuilding (the first building type of the goods classes 6 .. 16, fixed numbers) and gm_GoodBuilding (for every good in a building type's slot list: the first building type of its class plus the class run length * the position of this building type in its run of equal kinds / that run's length, the smallest such); goods in no slot list get the first building type of their class
   @deep read against the original 0x4fe370 and reconciled: the two fills, the fixed class entries, cur as
   an int starting at 0x58, the kind runs (kind != cur first, run++ in the else), the slot loop with the
   goods class lookup, the class run count and the min store, the final pointer loop (equal). The
   frame-shifted branch targets are the same statements at moved offsets. What remains is allocation: the
   original keeps 0 in edx for the run / len / k stores (ours edi), holds p in edi and i in ebp with stack
   copies [esp+0x18] / [esp+0x24] (the extra 4-byte frame slot, frame 0x1c vs 0x18), and strength-reduces
   the slot index to a pointer in ebx ('+ lea ebx, [edi + 0x23]' / '+ add ebx, 2'; ours 'mov ax, [ebx +
   eax*2 + 0x23]'), which also moves the base + cnt*run/len sum to ecx. Tried: a slot pointer stepped in
   the for (worse, 137 differing), the index form (119) */

void gm_InitBuildingTables(void)
{
    int run, len, i, j, k, u, base, cnt, v;
    int cur, kind, kind2;
    unsigned char *p, *q;
    unsigned short *slots;

    memset(gm_ClassBuilding, 0, 0x64);
    for (i = 0; i < 0x2dc; i++)
        gm_GoodBuilding[i] = 0x58;
    run = 0;
    len = 0;
    gm_ClassBuilding[8] = 0x2f;
    gm_ClassBuilding[7] = 0x32;
    gm_ClassBuilding[6] = 0x29;
    gm_ClassBuilding[10] = 0x21;
    gm_ClassBuilding[14] = 0x1e;
    gm_ClassBuilding[15] = 0x1f;
    gm_ClassBuilding[16] = 0x20;
    gm_ClassBuilding[11] = 0x17;
    gm_ClassBuilding[12] = 0x35;
    gm_ClassBuilding[9] = 0x14;
    cur = 0x58;
    p = (unsigned char *)ai_BTypes;
    for (i = 0; i < 0x58; i++, p += 0x24d) {
        kind = *p;
        if (kind != cur) {
            run = 0;
            cur = kind;
            len = 0;
            for (j = i, q = p; j < 0x58 && *q == kind; j++, q += 0x24d)
                len++;
        } else {
            run++;
        }
        slots = (unsigned short *)(p + 0x23);
        for (k = 0; k < p[0x22]; k++) {
            if (slots[k] != 0xffff) {
                u = slots[k] & 0x7fff;
                base = gm_ClassBuilding[(unsigned char)ai_Goods[u * 0x41 + 0x21]];
                q = (unsigned char *)ai_BTypes + base * 0x24d;
                kind2 = *q;
                cnt = 0;
                for (j = base; j < 0x58 && *q == kind2; j++, q += 0x24d)
                    cnt++;
                v = base + cnt * run / len;
                if (v < gm_GoodBuilding[u])
                    gm_GoodBuilding[u] = v;
            }
        }
    }
    for (i = 0; i < 0x2dc; i++) {
        if (gm_GoodBuilding[i] == 0x58)
            gm_GoodBuilding[i] = gm_ClassBuilding[(unsigned char)ai_Goods[i * 0x41 + 0x21]];
    }
}
/* 0x4fe570, loads the building and object tables of the data path root: resets the player records at 0x7d6a64 (stride 0xaa4 ints, each gets the 0xaa0 ints 0x6178c8), allocates the building prototypes (0xca78 = 0x58 * 0x24d, "f3_gm:lpGebaeudePROT") and instances (0xa900, "..INST"), reads "<root>A_Geb.dat" (0x58 records of 0x24d bytes), the object prototypes (0xb9dc = 0x2dc * 0x41, "..PROT"), instances (0x14f218) and "<root>A_Obj.dat"; builds the lookup tables (0x4fe370, 0x503e80) and adds the count of the flagged (bit 15) objects of the kinds 2 / 6 (except 0xfd) to each building's byte +0x21. Returns 0, or -1 .. -6 for the failed step.
   @deep read whole against the original: stock-block init loop, allocations, both file reads, the
   per-building count loop (now id = u & 0x7fff kept, bit 15 as (u & 0x8000): test dh,dh matches). Left:
   (1) the stock-block loop's induction pointer is biased +8 in ours (mov eax, 0x7d6a68; stores
   [eax-8],[eax-4],[eax],[eax+4]; lea edi,[eax+8]) vs +4 in the original (0x7d6a64; [eax-4]..[eax+8]; lea
   edi,[eax+0xc]) - same stores, same order, only the IV bias; tried blk at +0 / +4, an index loop over i,
   &blk[4]: same bias. (2) k = ai_Goods[id*0x41]: ours folds mov cl,[eax+ebx], the original mov ecx,eax;
   add ecx,ebx; mov cl,[ecx] (same address); tried (ai_Goods+id*0x40)[id], (ai_Goods+id)[id*0x40], an int
   cast: unchanged. Rest is sib swap and a 2-reg rotation. */

int gm_LoadTables(char *root)
{
    char path[512];
    FILE *f;
    int i, n, cnt, id;
    unsigned short *w;
    unsigned short u;
    char k;
    int *slot;
    int *blk;

    /* every stock block: its header from the player slot (+0x68, +0xb8) and the factors 1000, its records from the template */
    slot = (int *)(ai_PlayerSlots[0] + 0xb8);
    blk = (int *)gm_StockBlocks;
    do {
        blk[0] = slot[-0x14];
        blk[1] = 1000;
        blk[2] = slot[0];
        blk[3] = 1000;
        memcpy(blk + 4, gm_DefaultStock, 0xaa0 * 4);
        slot += 0x2f4 / 4;
        blk += 0x2a90 / 4;
    } while ((int)slot < (int)(ai_PlayerSlots[0] + 0xb8 + 5 * 0x2f4));
    ai_BTypes = (char *)mem_Alloc(0xca78, gm_TagGebaeudePROT);
    if (ai_BTypes == 0)
        return -1;
    ai_Blds = (char *)mem_Alloc(0xa900, gm_TagGebaeudeINST);
    memset(ai_Blds, 0, 0xa900);
    if (ai_Blds == 0)
        return -2;
    sprintf(path, "%s%s", root, "A_Geb.dat");
    f = fopen(path, "rb");
    if (f == 0)
        return -3;
    fread(ai_BTypes, 0x24d, 0x58, f);
    fclose(f);
    ai_Goods = (char *)mem_Alloc(0xb9dc, gm_TagObjektePROT);
    if (ai_Goods == 0)
        return -4;
    ai_Objects = (struct AIOBJ *)mem_Alloc(0x14f218, gm_TagObjekteINST);
    if (ai_Objects == 0)
        return -5;
    memset(ai_Objects, 0, 0x14f218);
    sprintf(path, "%s%s", root, "A_Obj.dat");
    f = fopen(path, "rb");
    if (f == 0)
        return -6;
    fread(ai_Goods, 0x41, 0x2dc, f);
    fclose(f);
    gm_InitBuildingTables();
    gm_ResetCharacters();
    for (i = 0; i < 0xca78; i += 0x24d) {
        cnt = 0;
        w = (unsigned short *)(ai_BTypes + i + 0x23);
        for (n = 0x40; n != 0; n--, w++) {
            u = *w;
            if (u != 0xffff) {
                id = u & 0x7fff;
                k = ai_Goods[id * 0x41];
                if ((k == 2 || k == 6) && id != 0xfd && (u & 0x8000) != 0)
                    cnt++;
            }
        }
        ai_BTypes[i + 0x21] += (char)cnt;
    }
    util_Randomize();
    return 0;
}

extern int f3gm_ObjectCount;
extern int ai_Unk6178f8[];
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
int gm_FreeBuilding();                     /* 0x5012e0 */
int gm_RemoveObject();                     /* 0x4ffff0 */
int gm_FindById(int *bld, int *obj, int *chr, int id);
int gm_GoodsStock(int p, int i);                   /* 0x4ff950 */
int gm_GoodsDemand(int p, int i);                   /* 0x4ff8c0 */
float gm_ItemPrice(int item, unsigned char n); /* 0x5087f0 */

/* 0x4fe7f0, frees every building of the instance table (0x5012e0) and clears the table (0x100 records of 0xa9 bytes) */
void gm_ClearBuildings(void)
{
    int i;

    i = 0;
    do {
        if (ai_Blds[i] != 0)
            gm_FreeBuilding(ai_Blds + i);
        i += 0xa9;
    } while (i < 0xa900);
    memset(ai_Blds, 0, 0xa900);
}
/* 0x4fe830, runs through the object instances (0x5008 records of 0x43 bytes: word +0 the prototype, int +6 the owner id): the owner (found by 0x4fea50 as a building / object / character) gets the instance back (0x4ffff0 on its list at +0x5d / +0x14 / +0x178), an instance without a live owner is freed (0x500080); then the table is cleared
   @deep read whole against the original: the loop is for (i = 0; i < 0x5008*0x43; i += 0x43) (jl
   0x14f218), the owner list is b+0x5d / o+0x14 / c+0x178 in that order, the id is passed unsigned
   (ORDOBJ.type, gv.h, now with ownerId +6). Branch structure equal. Left: register allocation only:
   rotations eax<->esi, edx<->esi (base/index swaps), and the free call's argument: ours add eax, esi; push
   eax, the original lea ecx, [esi + eax]; push ecx (same value in another register; ecx->edx follows at
   the reload). Tried i + (char *)ai_Objects, &->type, an int cast: unchanged. */

void gm_ClearObjects(void)
{
    int i, list;
    int b, o, c;

    for (i = 0; i < 0x5008 * 0x43; i += 0x43) {
        if (((ORDOBJ *)((char *)ai_Objects + i))->type != 0) {
            gm_FindById(&b, &o, &c, ((ORDOBJ *)((char *)ai_Objects + i))->ownerId);
            if (b != 0)
                list = b + 0x5d;
            else if (o != 0)
                list = o + 0x14;
            else if (c != 0)
                list = c + 0x178;
            else
                list = 0;
            if (list != 0) {
                gm_RemoveObject(list, ((ORDOBJ *)((char *)ai_Objects + i))->type);
            } else {
                gm_FreeObjectExtra((unsigned short *)((char *)ai_Objects + i));
                ((ORDOBJ *)((char *)ai_Objects + i))->type = 0;
                f3gm_ObjectCount--;
            }
        }
    }
    memset(ai_Objects, 0, 0x14f218);
}
/* 0x4fe900, the number of the object prototype with the given name (case-insensitive; the name is at +1 of each 0x41-byte record), 0 when none */

int gm_FindObjectProto(char *name)
{
    int n, i;

    n = 0;
    i = 0;
    do {
        if (_stricmp(name, &ai_Goods[i + 1]) == 0)
            return n;
        i += 0x41;
        n++;
    } while (i < 0xb9dc);
    return 0;
}
/* 0x4fe940, 1 when the object prototype n is a building part of the kinds 1, 3, 4, 0xb, 0x1a, 0x1b, 0x1c */

int gm_IsBuildingKind(int n)
{
    char *g;

    g = ai_Goods + n * 0x41;
    if (*g == 1 || *g == 0x1a || *g == 0x0b || *g == 0x1c || *g == 0x1b || *g == 3 || *g == 4)
        return 1;
    return 0;
}
/* 0x4fe980, the object instance with the id (int at +2) in the instance table, 0 when none */

int gm_FindObjectInst(int id)
{
    int i;
    short *p;

    i = 0;
    p = (short *)ai_Objects;
    while (*p == 0 || *(int *)(p + 1) != id) {
        i++;
        p = (short *)((char *)p + 0x43);
        if (i >= 0x5008)
            return 0;
    }
    return (int)ai_Objects + i * 0x43;
}
/* 0x4fe9c0, frees the buildings of the group (1, 6) and the four tables (building prototypes, building instances, object prototypes, object instances) */

void gm_Free(void)
{
    int o;

    o = gm_BuildQuery(1, 6);
    while (o != 0) {
        gm_FreeBuilding(o);
        o = gm_BuildNext();
    }
    if (ai_BTypes != 0) {
        mem_Free(ai_BTypes);
        ai_BTypes = 0;
    }
    if (ai_Blds != 0) {
        mem_Free(ai_Blds);
        ai_Blds = 0;
    }
    if (ai_Goods != 0) {
        mem_Free(ai_Goods);
        ai_Goods = 0;
    }
    if (ai_Objects != 0) {
        mem_Free(ai_Objects);
        ai_Objects = 0;
    }
}
/* 0x4fea50, finds what has the id: a character (0x300 records, int +4; stored in *chr, returns 3), a building (*bld, returns 1) or an object instance (*obj, returns 2); each result pointer is cleared first; 0 when nothing (or the prototype table is not loaded) */

int gm_FindById(int *bld, int *obj, int *chr, int id)
{
    ORDCHAR *c;
    ORDBLD *b;
    ORDOBJ *o;
    int i;

    if (ai_Goods != 0) {
        if (bld != 0)
            *bld = 0;
        if (obj != 0)
            *obj = 0;
        if (chr != 0) {
            *chr = 0;
            c = ORD_CHARS;
            i = 0;
            do {
                if (c->id != 0xffff && c->netId == id) {
                    *chr = (int)c;
                    return 3;
                }
                c++;
                i++;
            } while (i < 0x300);
        }
        if (bld != 0) {
            i = 0;
            b = (ORDBLD *)ai_Blds;
            do {
                if (b->type != 0 && b->id == id) {
                    *bld = (int)b;
                    return 1;
                }
                i++;
                b++;
            } while (i < 0x100);
        }
        if (obj != 0 && (i = 0, o = (ORDOBJ *)ai_Objects, 0 < f3gm_ObjectCount)) {
            do {
                if (o->type != 0) {
                    if (o->objId == id) {
                        *obj = (int)o;
                        return 2;
                    }
                    i++;
                }
                o = (ORDOBJ *)((char *)o + 0x43);
            } while (i < f3gm_ObjectCount);
        }
    }
    return 0;
}
/* 0x4feb30, runs 0x4ff550 (player i, the clock 0x770ae0) for every used player slot (0x2f4 bytes each, from 0x7e3f40) */

void gm_UpdatePlayers(void)
{
    char *p;
    int i;

    i = 0;
    p = ai_PlayerSlots[0];
    do {
        if (*p == 0)
            return;
        gm_UpdatePrices(i, ai_Time);
        p += 0x2f4;
        i++;
    } while ((int)p < (int)((unsigned)ai_PlayerSlots + 0xbd0));
}
/* 0x4feb60, resets the 0x55 goods records of the player p from the template gm_DefaultStock: copies v00c and the base stock, sets the stock (gm_GoodsStock) of the goods of the kinds 0x17 / 0x25 or else the demand (gm_GoodsDemand), the price (gm_ItemPrice of the item, 100), and the mean to the demand */

void gm_InitPlayerRecord(int p)
{
    int i;
    GMSTOCK *g;
    GMSTOCK *t;
    char *pc;

    for (i = 0; i < 0x55; i++) {
        g = (GMSTOCK *)(init_StockBlocks + p * 0x2a90) + i;
        t = &((GMSTOCK *)gm_DefaultStock)[i];
        pc = ai_Goods + g->item * 0x41;
        g->v00c = t->v00c;
        g->baseStock = t->baseStock;
        if (*pc == 0x17 || *pc == 0x25)
            g->stock = gm_GoodsStock(p, i);
        else
            g->demand = (float)gm_GoodsDemand(p, i);
        g->price = gm_ItemPrice(g->item, 100);
        g->mean = g->demand;
    }
}

float gm_StockPrice();                   /* 0x4ffa10 */
int gm_FreeList();                     /* 0x500140 */
void cm_SendGoodsDelta(char *p, unsigned char k);                     /* 0x4c5710 */

/* 0x4ffbf0, the 0x4ffa10 value of the goods record of the player p with the word at +0 equal to id (0x55 records of 0x80 bytes), -1 when there is none */
float gm_GoodsValue(int p, unsigned id)
{
    int i;
    unsigned short *q;

    i = 0;
    q = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*q == id)
            return gm_StockPrice(p, i);
        i++;
        q += 0x40;
    } while (i < 0x55);
    return -1.0f;
}
/* 0x4ffc40, the goods record of the player p with the word at +0 equal to id, 0 when none */

char *gm_FindGoods(int p, unsigned id)
{
    int i;
    unsigned short *q;

    i = 0;
    q = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*q == id)
            return init_StockBlocks + i * 0x80 + p * 0x2a90;
        i++;
        q += 0x40;
    } while (i < 0x55);
    return 0;
}
/* 0x4ffc90, the distance of the points (a, b) and (c, d) times 2.90816330909729 (the map scale) */

float gm_MapDistance(int a, int b, int c, int d)
{
    return (float)(sqrt(fabs((double)a - (double)c) * fabs((double)a - (double)c) + fabs((double)b - (double)d) * fabs((double)b - (double)d)) * 2.90816330909729);
}
/* 0x4fffc0, the first free record (prototype 0) of the object instance table ai_Objects (0x5008 records of 0x43 bytes), 0 when it is full */

short *gm_FindFreeObject(void)
{
    int i;
    ORDOBJ *p, *res;

    res = 0;
    i = 0;
    p = (ORDOBJ *)ai_Objects;
    while (res == 0 && i < 0x5008) {
        if (p->type != 0) {
            i++;
            p++;
        } else
            res = p;
    }
    return (short *)res;
}
/* 0x4ffff0, removes the object instance with the prototype id from the singly linked list whose head pointer is *list (the next pointer sits at +0x3f of each node): frees its extra part (0x500140 of the pointer at +0x14), the node itself (0x500080), relinks, marks the record free and decrements the count 0x6f1bdc. Returns 0, -1 for an empty list, -2 when not found */

int gm_RemoveObject(int *list, unsigned id)
{
    unsigned short *prev, *cur;
    int n;

    if (list == 0)
        return -1;
    if ((unsigned short *)*list == 0)
        return -1;
    n = 0;
    prev = 0;
    cur = (unsigned short *)*list;
    do {
        if (*cur == id) {
            if (*(int *)(cur + 10) != 0)
                gm_FreeList(cur + 10);
            gm_FreeObjectExtra(cur);
            if (n == 0)
                *list = *(int *)((char *)cur + 0x3f);
            else
                *(int *)((char *)prev + 0x3f) = *(int *)((char *)cur + 0x3f);
            *cur = 0;
            f3gm_ObjectCount--;
            return 0;
        }
        n++;
        prev = cur;
        cur = *(unsigned short **)((char *)cur + 0x3f);
    } while (cur != 0);
    return -2;
}
/* 0x4ff450, a random market shake: with chance 1 / 2, 0 .. 1 times (0x2 draws) a random goods record of the own player (of the first 0x54) that is in use and held gets a copy whose mean is the demand raised by 1.5 or halved, and whose price is the item's price (gm_ItemPrice, 100) scaled the same; the copy goes to cm_SendGoodsDelta for the own player */

void gm_RandomMarketShake(int unused)
{
    GMSTOCK rec;
    int n;
    unsigned k;
    GMSTOCK *g;

    if (util_RandomU(2) != 0) {
        for (n = util_RandomU(2) & 0xffff; n > 0; n--) {
            k = util_RandomU(0x54) & 0xffff;
            g = (GMSTOCK *)(init_StockBlocks + ai_OwnPlayer * 0x2a90) + k;
            if (g->item != 0 && g->amount != 0) {
                rec = *g;
                if (util_RandomU(1) != 0) {
                    rec.mean = (float)(rec.demand * 1.5);
                    rec.price = (float)(gm_ItemPrice(g->item, 100) * 1.5);
                } else {
                    rec.mean = (float)(rec.demand * 0.5);
                    rec.price = (float)(gm_ItemPrice(g->item, 100) * 0.5);
                }
                cm_SendGoodsDelta((char *)&rec, ai_OwnPlayer);
            }
        }
    }
}

extern int gm_QueryType;
extern int gm_QueryOwner;
extern unsigned short gm_QueryOwnerNo;
#define ALIGN1 __declspec(align(1))
extern ALIGN1 char gm_QueryKind[550];
extern char gm_QueryFirst, gm_QueryScanAll, gm_QueryChildren, gm_QueryStopFirst;  /* 0x6f1be0, 0x6f1be8, 0x6f1be9, 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
extern int gm_QueryScanned;
extern ORDOBJ *gm_QueryCursor;
extern int gm_QueryDepth;
extern int gm_QueryStack[20488];
int gm_FindMarketObject(int id);                     /* 0x500ec0 */
int gm_FindPlayerBuilding(int p);                   /* 0x501000 */

/* 0x4ffa10, the price of the goods record n of the player p: the base price (gm_ItemPrice, 100) when no market object holds the good (gm_FindMarketObject, for another player the kind 0x1db object of its building, gm_FindPlayerBuilding); else a factor mean / demand, times demand / the market's count of the good (at least 1) when that count is above the demand and the good is not of the kind 0x17, clamped to 0.25 .. 5.0, and the price moves 0.4 of the way from the stored price to the base price times the factor; a record without a stored price (<= 0) gets the base price, at least 512
   @deep read the whole function against 0x4ffa10..0x4ffbe6 and reconciled every statement (the market
   lookup, the count, the mean/demand ratio kept in st, the ternary factor with the float-cast division by
   max(count,1), the 0.25 / 5.0 clamps, the (float)f * base - cur step, the 512 floor). Tried: if-clamps,
   nested min(max()) in one expression, the clamp on f itself (all worse), the 512 test as >= (worse), mk
   == 0 nested or as an early return (same). Two hunks remain, both optimizer layout with equal meaning:
   (1) ours threads the 0.25 path past the 5.0 compare ('fld 0.25; fstp qword [esp+0x14]; jmp', 0x4ffb3b)
   where the original runs it through 'fcom 5.0' (0x4ffb3b); (2) ours cross-jumps the two identical 'push
   0x64; push esi; call gm_ItemPrice; add esp,8; epilogue' tails into one, the original keeps them apart
   (0x4ffbb8..0x4ffbc8, its third ret): together the 8 missing instructions */
float gm_StockPrice(int p, int n)
{
    unsigned id;
    char *good;
    int mk;
    int b;
    ORDOBJ *o;
    float count, cur, ratio, r;
    double f, t;

    id = GM_BLOCK(p)->goods[n].item;
    good = ai_Goods + id * 0x41;
    mk = gm_FindMarketObject(id);
    if (mk == 0 && p != ai_OwnPlayer && (b = gm_FindPlayerBuilding(p)) != 0)
        mk = (int)gm_Query((void *)((ORDBLD *)b)->objList, 2, 0, 0x1db, 6);
    count = 0.0f;
    if (mk == 0)
        return gm_ItemPrice(id, 100);
    o = (ORDOBJ *)gm_Query((void *)((ORDOBJ *)mk)->list, 1, 0, id);
    if (o != 0)
        count = (float)o->count;
    ratio = GM_BLOCK(p)->goods[n].mean / GM_BLOCK(p)->goods[n].demand;
    cur = GM_BLOCK(p)->goods[n].price;
    t = ratio * (count > GM_BLOCK(p)->goods[n].demand && *good != 0x17 ? (float)(GM_BLOCK(p)->goods[n].demand / (count < 1.0 ? 1.0 : count)) : 1.0f);
    t = t > 0.25 ? t : 0.25;
    f = t < 5.0 ? t : 5.0;
    r = ((float)f * gm_ItemPrice(id, 100) - cur) * 0.4f + GM_BLOCK(p)->goods[n].price;
    if (GM_BLOCK(p)->goods[n].price <= 0.0f) {
        if (gm_ItemPrice(id, 100) < 512.0f)
            return 512.0f;
        return gm_ItemPrice(id, 100);
    }
    return r;
}
/* 0x4ffcc0, the next match of the object query gm_Query set up: from the cursor record it walks the whole instance table (gm_QueryScanAll, skipping empty records, counting them in gm_QueryScanned) or the next pointers of the list (with gm_QueryChildren the child lists go on gm_QueryStack and are walked when a list ends), tests each record against the set criteria (prototype, owner id, the owner's character number gm_ObjectOwnerNo, the prototype kind) and returns the last record that matched before the first match ends the walk (or, with gm_QueryStopFirst, the first record looked at); 0 when none
   @deep read the whole function against 0x4ffcc0..0x4ffe83 and reconciled every statement: the first-step
   push of the cursor's child list, the scanned < count loop with the cursor / match break, the
   empty-record skip, the four criteria in the original's order (prototype, objId at +2, the owner number
   as an unsigned short against 0xffff: the imm16 'cmp bp, 0xffff' now matches, the prototype kind byte
   with sete), the scan-all step and the list step with the child stack (gm_QueryDepth is the global the
   original caches in edi and reloads after the call). Tried: the criteria as a goto chain, as an if/else
   with the no: label, as nested ifs with three else m = 0 (worse, 473 bytes), the no: block placed after a
   continue (identical object: VC6 normalizes it), the null check before the res / m init (worse). What
   remains: (1) the original's mismatch block 'xor cl,cl; jmp 0x4ffddb' sits out of line at 0x4ffe02 (the
   jne at +0xb2 / +0xc4 and the jmp at +0x141 that diffclass reports as branch targets), ours keeps it
   inline after the kind test: a block placement VC6 picks the same for every source order tried; (2) ours
   keeps the zeroed m register (xor edx,edx) as a constant 0 and compares with it ('cmp eax, edx', 'cmp cl,
   dl', 'mov byte [gm_QueryFirst], dl'), the original zeroes m in cl alone and uses test / an immediate 0
   (0x4ffcc5..0x4ffcf4); (3) the rest is the register rotation res ebx/ebp, m cl/dl, the stack depth
   edi/esi, the goods base esi/ebx (diffclass: regs 58) (size: 2 of the instruction difference are shared /
   inline tails) */

unsigned short *gm_QueryNext(void)
{
    ORDOBJ *res;
    char m;

    res = 0;
    m = 0;
    if (gm_QueryCursor == 0)
        return 0;
    if (gm_QueryFirst != 0) {
        gm_QueryFirst = 0;
        if (gm_QueryChildren != 0 && gm_QueryCursor->list != 0)
            gm_QueryStack[gm_QueryDepth++] = gm_QueryCursor->list;
    }
    while (gm_QueryScanned < f3gm_ObjectCount) {
        if (gm_QueryCursor == 0 || m)
            break;
        while (gm_QueryCursor->type == 0)
            gm_QueryCursor++;
        if (gm_QueryStopFirst != 0)
            m = 1;
        if (gm_QueryType == 0x7fff || gm_QueryCursor->type == gm_QueryType) {
            if (gm_QueryType != 0x7fff)
                m = 1;
            if (gm_QueryOwner != -1) {
                if (gm_QueryCursor->objId != gm_QueryOwner)
                    goto no;
                m = 1;
            }
            if (gm_QueryOwnerNo != 0xffff) {
                if (gm_ObjectOwnerNo((unsigned short *)gm_QueryCursor) != gm_QueryOwnerNo)
                    goto no;
                m = 1;
            }
            if (gm_QueryKind[0] != -1)
                m = ai_Goods[gm_QueryCursor->type * 0x41] == gm_QueryKind[0];
        } else {
        no:
            m = 0;
        }
        if (gm_QueryScanAll != 0) {
            if (m)
                res = gm_QueryCursor;
            gm_QueryCursor++;
            gm_QueryScanned++;
        } else {
            if (m)
                res = gm_QueryCursor;
            gm_QueryCursor = gm_QueryCursor->next;
            if (gm_QueryCursor != 0) {
                if (gm_QueryCursor->list != 0 && gm_QueryChildren != 0)
                    gm_QueryStack[gm_QueryDepth++] = gm_QueryCursor->list;
            } else if (gm_QueryChildren != 0 && gm_QueryDepth != 0) {
                gm_QueryCursor = (ORDOBJ *)gm_QueryStack[--gm_QueryDepth];
                if (gm_QueryCursor->list != 0)
                    gm_QueryStack[gm_QueryDepth++] = gm_QueryCursor->list;
            }
        }
    }
    return (unsigned short *)res;
}
/* 0x4ffe90, starts an object query over the list (or, with the flag 7, over the whole instance table) and returns the first match (gm_QueryNext): n criteria follow as (kind, value) pairs: kind 0 = prototype, 1 = owner id, 3 = the owner's character number (gm_ObjectOwnerNo), 4 = the prototype kind (each with a value), 5 = stop at the first match, 6 = descend into the children, 7 = scan the whole table (without value). Not set criteria match everything; 0 without the goods table or a list, and for 6 together with 7. Used as gm_Query (list, 2, 6, 0, 0x115) .. all over the exe
   @deep read the whole function against 0x4ffe90..0x4fff9b and reconciled every statement: the reset of
   the query state in the original's store order, the argument walk (ap = &n, pre-incremented, switch on
   the low byte as unsigned char: the 8-entry jump table is equal), the per-kind value reads (word / dword
   / word / byte), the flags, the scan-all exit (0 with 6 and 7 together, else the instance table) and the
   list start, both as tail calls of gm_QueryNext. Tried: the ai_Goods test as an early return or wrapping
   the body, the list test both ways, scan / kids as char or unsigned char: the object is the same. Left is
   allocation: ours takes esi as the zero register (push esi at entry, 'xor esi,esi; cmp eax,esi', the
   stores of 0 from esi) and keeps -1 in eax ('or eax,-1', gm_QueryOwner and gm_QueryKind[0] from eax /
   al), the original uses eax as the zero register and immediates for the -1 stores, and pushes esi only
   inside the n > 0 block (0x4ffef7 / 0x4fff6d): that is the 3 extra instructions and the 1-byte shift
   diffclass reports for the three jumps to the return-0 block (+0x108 vs +0x107); the zero stores of the
   flags come from bl / dl / al by the same choice (diffclass: regs, slots) */

char *gm_Query(void *list, int n, ...)
{
    int *ap;
    char scan, kids;
    int i;

    if (ai_Goods != 0) {
    scan = 0;
    kids = 0;
    gm_QueryType = 0x7fff;
    gm_QueryOwner = -1;
    gm_QueryOwnerNo = 0xffff;
    gm_QueryKind[0] = (char)0xff;
    gm_QueryStopFirst = 0;
    gm_QueryScanAll = scan;
    gm_QueryChildren = kids;
    gm_QueryFirst = 1;
    gm_QueryScanned = 0;
    gm_QueryDepth = 0;
    if (n > 0) {
        ap = &n;
        for (i = n; i != 0; i--) {
            ap++;
            switch ((unsigned char)*ap) {
            case 0:
                ap++;
                gm_QueryType = (unsigned short)*ap;
                break;
            case 1:
                gm_QueryOwner = ap[1];
                ap++;
                break;
            case 3:
                gm_QueryOwnerNo = (unsigned short)ap[1];
                ap++;
                break;
            case 4:
                gm_QueryKind[0] = (char)ap[1];
                ap++;
                break;
            case 5:
                gm_QueryStopFirst = 1;
                break;
            case 6:
                kids = 1;
                break;
            case 7:
                scan = 1;
                break;
            }
        }
        gm_QueryScanAll = scan;
        gm_QueryChildren = kids;
        if (scan != 0) {
            if (kids != 0)
                return 0;
            gm_QueryCursor = (ORDOBJ *)ai_Objects;
            return (char *)gm_QueryNext();
        }
    }
    if (list != 0) {
        gm_QueryCursor = (ORDOBJ *)list;
        return (char *)gm_QueryNext();
    }
    }
    return 0;
}

extern struct AIBLD *ai_MarketBuilding;                /* 0x6f1bc8, the market building (group 1, 5, 10) */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
extern char gm_StockBlocks[0xd4d0];
unsigned gm_BoyHead[] = { 0, 1735292266, 1430937445, 4540238, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118a8 */
unsigned gm_GirlHead[] = { 0, 1684365677, 1852139619, 1161907551, 1162363716, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118f0 */
unsigned gm_TitleHeadsMale[] = { 12, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1684955496, 1802659191, 1597272677, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1684955496, 1802659191, 1597141605, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1684955496, 1802659191, 1597207141, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1735550306, 1852727661, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701409360, 1919251571, 1414875999, 17748, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1601462881, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1650813284, 1431002932, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1650813284, 1312902495, 12878, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1969578354, 1735550306, 1818588005, 1398760812, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1969578354, 1819436386, 1819437157, 1600613993, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1751344486, 1601332596, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1802728813, 1852142693, 1969775475, 1702524005, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 1298084466, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 1298084210, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x611938 */
unsigned gm_TitleHeadsFemale[] = { 14, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1684955496, 1802659191, 1852404325, 1380343602, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1684955496, 1802659191, 1852404325, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1668505975, 1634887272, 1380343669, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 829319538, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x612288 */
unsigned gm_JobHeadsMale[] = { 33, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 76, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 77, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 78, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 79, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 80, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 81, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 83, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 84, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 85, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 86, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 87, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 88, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 82, 1936943479, 1668509285, 1952866664, 1601332588, 1414813003, 12869, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x612bd8 */
unsigned gm_JobHeadsFemale[] = { 33, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 2053926760, 1818583398, 1769104748, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191426, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x614518 */
unsigned gm_SpyHeads[] = { 0, 1869181043, 1162698606, 1297434948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x617398 */
int gm_RichestYear = -1;  /* 0x61772c, the game year (ai_TimeYear) of gm_FindRichest's last search */
int gm_RichestWorth = -1000000;  /* 0x617730, the worth of gm_RichestChar (0xc80 when none was found) */
float gm_BirthChance[6] = { 0.240015402f, 0.222200006f, 0.125f, 0.0555500016f, 0.0138499998f, 0.0f };  /* 0x617734, a float per number of children / relatives */
float gm_DayStartHours[] = { 8.0f, 7.0f, 8.0f, 9.0f };  /* 0x617830 / */
float gm_DayEndHours[] = { 20.0f, 21.0f, 20.0f, 19.0f };  /* 0x617840 */
int ai_Unk617850[] = { 0, 0, 1, 1, 1109393408, 16, 2, 1097859072, 16, 3, 1101004800, 16, 4, 1092616192, 4, 5, 0, 8, 6, 1090519040, 8, 7, 1084227584, 8, 8, 1077936128, 4, 9, 1094713344, 16 };  /* 0x617850, records of 3 ints ( */
/* ai_Unk7e3ff8: view of ai_PlayerSlots (tools/data_views.py) */
int gm_DefaultStock[] = { 468, 0, 1084227584, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x6178c8 */
int ai_Unk6178f8[] = { 15, 0, 0, 146, 18, 1094713344, 21, 1094713344, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 341, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 342, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 343, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 344, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 345, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 346, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 469, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 146, 21, 0, 14, 1086324736, 8, 1094713344, 22, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 347, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 348, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 349, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 351, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 352, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 470, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 147, 18, 1086324736, 21, 1086324736, 20, 1086324736, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 353, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 354, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 355, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 356, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 357, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 358, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 471, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 148, 18, 1086324736, 21, 1086324736, 20, 1086324736, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 365, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 366, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 367, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 368, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 369, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 370, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 472, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 148, 18, 1088421888, 20, 1086324736, 7, 1084227584, 14, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 371, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 372, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 373, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 374, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 375, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 376, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 473, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 147, 14, 1088421888, 8, 1088421888, 22, 1088421888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 377, 0, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 378, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 379, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 380, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 381, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 382, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 464, 0, 1112014848, -1, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 149, 18, 1104150528, 21, 1090519040, 14, 1082130432, 8, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 465, 0, 1106247680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1092616192, 21, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 466, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1073741824, 21, 1073741824, 20, 1090519040, 7, 1082130432, 0, 0, 0, 0, 0, 0, 0, 0, 467, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1073741824, 20, 1073741824, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 458, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 21, 1104150528, 7, 1108869120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 459, 0, 1097859072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 21, 1105199104, 7, 1104150528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 460, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 18, 1090519040, 21, 1101004800, 20, 1082130432, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 461, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1099956224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 462, 0, 1109393408, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1105199104, 8, 1073741824, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 463, 0, 1103626240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1090519040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 474, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 151, 7, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 359, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 360, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 361, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 362, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 363, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 364, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 455, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 150, 14, 1095761920, 8, 1095761920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 456, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 22, 1110966272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 457, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 22, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 686, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 32, 1114636288, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1103626240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 1090519040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 552, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 731, 29, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 553, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 554, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 555, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 556, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 557, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 558, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 689, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 150, 32, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 690, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 691, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 692, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 693, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 694, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 695, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 662, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 731, 32, 1094713344, 31, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 663, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 664, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 665, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 667, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 668, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x6178f8 */
G7GRP gm_BuildingItemGroups[37] = { { 41, { 0, 0, 468, 341, 342, 0, 0, 0, 0, 0 } }, { 42, { 0, 0, 468, 341, 342, 344, 343, 0, 0, 0 } }, { 43, { 0, 0, 468, 0, 341, 342, 344, 343, 345, 346 } }, { 50, { 469, 0, 0, 347, 348, 0, 0, 0, 0, 0 } }, { 51, { 469, 0, 347, 348, 349, 350, 0, 0, 0, 0 } }, { 52, { 469, 0, 347, 348, 349, 350, 352, 351, 0, 0 } }, { 33, { 471, 0, 0, 0, 365, 366, 0, 0, 0, 0 } }, { 34, { 471, 0, 0, 0, 365, 366, 367, 368, 0, 0 } }, { 35, { 471, 0, 0, 0, 365, 366, 367, 368, 369, 370 } }, { 23, { 472, 0, 0, 371, 372, 0, 0, 0, 0, 0 } }, { 24, { 472, 0, 0, 371, 372, 373, 375, 0, 0, 0 } }, { 25, { 472, 0, 0, 371, 372, 373, 375, 376, 374, 0 } }, { 47, { 470, 0, 0, 353, 354, 0, 0, 0, 0, 0 } }, { 48, { 470, 0, 0, 353, 354, 355, 356, 0, 0, 0 } }, { 49, { 470, 0, 0, 353, 354, 355, 356, 357, 358, 0 } }, { 53, { 0, 0, 0, 0, 473, 377, 378, 0, 0, 0 } }, { 54, { 0, 0, 0, 473, 377, 378, 379, 380, 0, 0 } }, { 55, { 0, 0, 0, 473, 377, 378, 379, 380, 381, 382 } }, { 20, { 474, 0, 0, 0, 359, 360, 0, 0, 0, 0 } }, { 21, { 474, 0, 0, 0, 359, 360, 361, 362, 0, 0 } }, { 22, { 474, 0, 0, 0, 359, 360, 361, 362, 363, 364 } }, { 72, { 552, 0, 0, 0, 553, 554, 0, 0, 0, 0 } }, { 73, { 552, 0, 0, 0, 553, 554, 555, 556, 0, 0 } }, { 74, { 552, 0, 0, 0, 553, 554, 555, 556, 557, 558 } }, { 31, { 463, 461, 462, 0, 0, 0, 0, 0, 0, 0 } }, { 75, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 76, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 77, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 81, { 689, 0, 0, 0, 690, 691, 0, 0, 0, 0 } }, { 82, { 689, 0, 0, 0, 690, 691, 692, 693, 0, 0 } }, { 83, { 689, 0, 0, 0, 690, 691, 692, 693, 694, 695 } }, { 78, { 662, 663, 664, 0, 0, 0, 0, 0, 0, 0 } }, { 79, { 662, 663, 664, 665, 666, 0, 0, 0, 0, 0 } }, { 80, { 662, 663, 664, 665, 666, 667, 668, 0, 0, 0 } }, { 32, { 464, 465, 466, 467, 0, 0, 0, 0, 0, 0 } }, { 30, { 458, 459, 460, 0, 0, 0, 0, 0, 0, 0 } }, { 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };  /* 0x61a348, 37 item groups: a building kind and up to ten object prototypes */
int ai_NextObjectId = 1;  /* 0x61a654, the next object id */
unsigned char gm_BQType = 0xff;  /* 0x61a66c, the building query: the building type (ORDBLD.type, 0xff = any) */
int gm_BQId = -1;  /* 0x61a670, the query: id (-1 = any) */
unsigned short gm_BQMaster = 32767;  /* 0x61a674, the query: the master id (ORDBLD.masterId, 0x7fff = any) */
unsigned short gm_BQOwner = 32767;  /* 0x61a676, the query: the owner (ORDBLD.owner, 0x7fff = any) */
unsigned char gm_BQClass = 0xff;  /* 0x61a678, the query: the class of the type (the first byte of its ai_BTypes record, 0xff = any) */
int gm_QueryType = 32767;  /* 0x61a67c, the query: object prototype ( */
int gm_QueryOwner = -1;  /* 0x61a680, the query: owner id (-1 = any) */
unsigned short gm_QueryOwnerNo = 0xffff;  /* 0x61a684, the query: the owner character number gm_ObjectOwnerNo must give (0xffff = any) */
ALIGN1 char gm_QueryKind[550] = { 255, 0, 0, 0, 0, 0, 0, 0, 105, 105, -25, -25, 105, 4, 63, 63, -67, -67, 63, 3, 63, 63, 126, 126, 42, 2, 63, 42, 84, 84, 21, 2, 63, 21, 84, 84, 21, 1, 42, 21, 84, 84, 21, 1, -109, 105, -109, 105, -67, 4, 105, 63, 105, 63, -109, 3, 105, 42, 84, 63, 105, 2, 105, 21, 63, 42, 63, 2, 105, 21, 63, 21, 63, 1, 84, 21, 63, 21, 63, 1, 105, 126, -109, 105, -46, 4, 63, 84, 105, 63, -88, 3, 63, 63, 84, 63, 126, 2, 63, 63, 63, 21, 84, 2, 42, 63, 63, 21, 84, 1, 21, 63, 63, 21, 84, 1, -46, -88, 105, 105, 105, 4, -88, 126, 63, 63, 63, 3, 126, 105, 63, 42, 63, 2, 84, 84, 63, 21, 42, 2, 84, 84, 63, 21, 21, 1, 84, 84, 42, 21, 21, 1, -25, 105, 105, 105, -109, 4, -67, 63, 63, 63, 105, 3, -109, 63, 63, 42, 84, 2, 126, 21, 42, 42, 63, 2, 126, 21, 42, 42, 42, 1, 105, 21, 42, 42, 42, 1, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105, 63, 63, 63, 2, 84, 105, 42, 21, 42, 2, 84, 105, 21, 21, 42, 1, 84, 84, 21, 21, 42, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 42, 21, 84, 126, 21, 2, 42, 21, 84, 105, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 63, 21, 105, 84, 21, 2, 42, 21, 105, 84, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -67, -88, 105, 105, 126, 4, -109, 126, 63, 63, 84, 3, 105, 105, 63, 42, 84, 2, 84, 105, 42, 21, 42, 2, 84, 84, 42, 21, 42, 1, 84, 84, 21, 21, 42, 1, -109, -109, -67, 105, 105, 4, 105, 105, -109, 63, 63, 3, 84, 84, 105, 42, 63, 2, 63, 84, 84, 21, 21, 2, 63, 63, 84, 21, 21, 1, 63, 63, 84, 21, 21, 1, 0, 0, 0, 0, 0, 1, -67, -109, -109, 105, 105, 4, -109, 105, 105, 63, 63, 3, 105, 84, 84, 42, 63, 2, 84, 63, 63, 21, 21, 2, 84, 63, 63, 21, 21, 1, 84, 63, 63, 21, 21, 1, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105 };  /* 0x61a686, the query: prototype kind (-1 = any); the
                                   table at +2 is ai_Unk61a688 (the same storage, one block as in the original) */
/* ---- layout 2026-10-23: the value / price helpers 0x5085b0 .. 0x509ee0 (ROUGH drafts) ---- */
char ai_Unk61a8ac[] = "?*T\002iT*\025*\002TT*\025*\001TT\025\025*\001";  /* 0x61a8ac, a table of ints indexed by the word + */
char gm_TagObjekteINST[] = "f3_gm:lpObjekteINST";  /* 0x61a9ec */
char gm_TagObjektePROT[20] = { 102, 51, 95, 103, 109, 58, 108, 112, 79, 98, 106, 101, 107, 116, 101, 80, 82, 79, 84, 0 };  /* 0x61aa00 */
char gm_TagGebaeudeINST[] = "f3_gm:lpGebaeudeINST";  /* 0x61aa20 */
char gm_TagGebaeudePROT[24] = { 102, 51, 95, 103, 109, 58, 108, 112, 71, 101, 98, 97, 101, 117, 100, 101, 80, 82, 79, 84, 0, 0, 0, 0 };  /* 0x61aa38 */
int ai_NextCharNumber = 0;  /* 0x6f1bb8 */
int ai_NextPlayerNumber = 0;  /* 0x6f1bbc */
int ai_CharCount = 0;  /* 0x6f1bc0 */
char * gm_BQName = 0;  /* 0x6f1bd0 */
char gm_BQStopFirst = 0;  /* 0x6f1bd4 */
char gm_BQFirstCall = 0;  /* 0x6f1bd5, stop at the first, first call */
int gm_BQTested = 0;  /* 0x6f1bd8, the number of records tested */
int f3gm_ObjectCount = 0;  /* 0x6f1bdc, the number of object instances in use */
char gm_QueryFirst = 0;  /* 0x6f1be0 */
int gm_QueryScanned = 0;  /* 0x6f1be4, the number of records scanned */
char gm_QueryScanAll = 0;  /* 0x6f1be8 */
char gm_QueryChildren = 0;  /* 0x6f1be9 */
char gm_QueryStopFirst = 0;  /* 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
/* 0x500190 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);                     /* 0x4c4880 */
void cm_SendGoodsRecord(void *p, unsigned char k);                     /* 0x4c56d0 */
void cm_BeginSendGroup(void);                     /* 0x4c3b30 */
void cm_EndSendGroup(void);                     /* 0x4c3b40 */
int cm_SendGroupState();                     /* 0x4c3b50 */
int gm_FindPlayerBuilding(int p);                   /* 0x501000 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */

/* the goods record n (0x80 bytes) of the player p */
#define GM_GOODS(p, n) (init_StockBlocks + (p) * 0x2a90 + (n) * 0x80)

/* 0x4ff190, refreshes the goods lists of the own player p (as the first part of gm_SyncMarket): clears the amount flag of the 0x55 goods records, flags the goods a building of the town (group (1, 6), not kind 10) holds in its store (its 0xff object) and the goods those need (the four words at +0x2e of the ai_Goods record), creates the flagged goods missing in the store of their market object (marketKind) there (gm_CreateObject), and sets the level byte +0x1c of the kind 0x11 object of the market objects 0x92 .. 0x97 / 0x2db to their goods count */
void gm_RefreshGoodsLists(unsigned p)
{
    int list[8];
    GMSTOCK *stock, *g, *g2;
    unsigned char *b;
    ORDOBJ *o, *o2;
    char *gp;
    int i, j, k, cnt;
    int *lp;

    list[0] = 0x92;
    list[1] = 0x93;
    list[2] = 0x94;
    list[3] = 0x95;
    list[4] = 0x96;
    list[5] = 0x97;
    list[6] = 0x2db;
    list[7] = 0;
    ai_MarketBuilding = (struct AIBLD *)gm_BuildQuery(1, 5, 10);
    if (ai_MarketBuilding == 0)
        return;
    stock = (GMSTOCK *)(init_StockBlocks + p * 0x2a90);
    if (p != ai_OwnPlayer)
        return;
    for (i = 0; i < 0x55; i++)
        stock[i].amount = 0;
    b = (unsigned char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (ai_BTypes[*b * 0x24d] != 10) {
            o = (ORDOBJ *)gm_Query(((ORDBLD *)b)->objList, 1, 0, 0xff);
            for (i = 0; o != 0 && i < 0x55; i++) {
                g = &stock[i];
                if (g->item != 0 && gm_Query(o->list, 1, 0, g->item) != 0) {
                    g->amount = 1;
                    gp = ai_Goods + g->item * 0x41;
                    for (j = 0; j < 0x55; j++) {
                        g2 = &stock[j];
                        for (k = 0; k < 4; k++)
                            if (g2->item == ((unsigned short *)(gp + 0x2e))[k])
                                g2->amount = 1;
                    }
                }
            }
        }
        b = (unsigned char *)gm_BuildNext();
    }
    for (i = 0; i < 0x55; i++) {
        g = &stock[i];
        o = (ORDOBJ *)gm_Query(((ORDBLD *)ai_MarketBuilding)->objList, 1, 0, g->marketKind);
        if (o != 0) {
            o = (ORDOBJ *)gm_Query(o->list, 1, 0, 0xff);
            if (g->amount != 0 && gm_Query(o->list, 1, 0, g->item) == 0)
                gm_CreateObject(o->objId, g->item, 1);
        }
    }
    for (lp = list; *lp != 0; lp++) {
        cnt = 0;
        o = (ORDOBJ *)gm_Query(((ORDBLD *)ai_MarketBuilding)->objList, 1, 0, *lp);
        if (o != 0 && (o2 = (ORDOBJ *)gm_Query(o->list, 1, 0, 0xff)) != 0) {
            o2 = (ORDOBJ *)gm_Query(o2->list, 1, 5);
            while (o2 != 0) {
                cnt++;
                o2 = (ORDOBJ *)gm_QueryNext();
            }
            o = (ORDOBJ *)gm_Query(o->list, 1, 4, 0x11);
            if (o != 0 && o->lvl1c != cnt)
                o->lvl1c = (unsigned char)cnt;
        }
    }
}

void gm_SyncMarket(unsigned p);
int gm_GoodsDemand(int p, int i);
int gm_GoodsStock(int p, int i);

/* 0x4ff550, the daily price update of the player p at the clock t: the date's year and month are interpolated in the player slot's two yearly tables (GMPOINT, at +0x64 and +0xb4) into the block's two indices, each scaled by its factor / 1000; then for the 0x55 goods records the demand moves halfway to gm_GoodsDemand (not for the kinds 0x17 / 0x25), the stock is gm_GoodsStock for the own player and the base stock for the others, the price is gm_StockPrice, a record with the byte +0x40 set takes its stock as demand, and the four counters +0x10 .. +0x1c are cleared; without a network game the market syncs (gm_SyncMarket) and shakes (gm_RandomMarketShake)
   @deep read the whole function against the original 0x4ff550..0x4ff8b2 and reconciled every statement:
   date / yearly table interpolation of both GMPOINT tables, the block indices and factors (GMBLOCK), the
   goods loop (demand, stock, price, demandIsStock, the daily counters), the net_Conn.s gate. Tried:
   else-if chain (kept), the inner three cases nested under if (k < n - 1) (worse, 64 ins), a temporary for
   table A like table B (worse, 245 ins). What remains is CSE placement in table A: the original computes
   p*17 (mov eax,ebp; shl eax,4; add eax,ebp) once before 'cmp ebx,edi; jne' and reuses it in all three
   inner cases, ours recomputes it in the k>0 case (0x4ff660..0x4ff665) and in the k<=0 case
   (0x4ff6b1..0x4ff6bc): those 6 instructions are the whole size difference; the rest is ebx/ebp rotation
   and the [esp+0x10]/[esp+0x24] slot swap (diffclass: slots), plus the folded 'mov al,[eax+edx]' vs 'add
   eax,edx; mov al,[eax]' of the ai_Goods kind byte */
void gm_UpdatePrices(unsigned p, void *t)
{
    GMDATE d;
    int year, n, k, a;
    int i;
    char kind;

    gm_ClockToDate(t, (unsigned char *)&d);
    year = d.year;
    for (n = 0; n < 9; n++)
        if (GM_TABLE_A(p)[n].year >= GM_TABLE_A(p)[n + 1].year)
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (GM_TABLE_A(p)[k].year >= year)
            break;
    if (k >= n - 1)
        GM_BLOCK(p)->demandIndex = GM_TABLE_A(p)[n - 1].value;
    else if (year == GM_TABLE_A(p)[k].year)
        GM_BLOCK(p)->demandIndex = (d.month + (year - GM_TABLE_A(p)[k].year) * 12) * (GM_TABLE_A(p)[k + 1].value - GM_TABLE_A(p)[k].value) / ((GM_TABLE_A(p)[k + 1].year - GM_TABLE_A(p)[k].year) * 12) + GM_TABLE_A(p)[k].value;
    else if (k > 0)
        GM_BLOCK(p)->demandIndex = (d.month + (year - GM_TABLE_A(p)[k - 1].year) * 12) * (GM_TABLE_A(p)[k].value - GM_TABLE_A(p)[k - 1].value) / ((GM_TABLE_A(p)[k].year - GM_TABLE_A(p)[k - 1].year) * 12) + GM_TABLE_A(p)[k - 1].value;
    else
        GM_BLOCK(p)->demandIndex = GM_TABLE_A(p)[0].value;
    GM_BLOCK(p)->demandIndex = GM_BLOCK(p)->demandFactor * GM_BLOCK(p)->demandIndex / 1000;
    for (n = 0; n < 9; n++)
        if (GM_TABLE_B(p)[n].year >= GM_TABLE_B(p)[n + 1].year)
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (GM_TABLE_B(p)[k].year >= year)
            break;
    if (k >= n - 1)
        GM_BLOCK(p)->indexB = GM_TABLE_B(p)[n - 1].value;
    else {
        if (year == GM_TABLE_B(p)[k].year)
            a = (d.month + (year - GM_TABLE_B(p)[k].year) * 12) * (GM_TABLE_B(p)[k + 1].value - GM_TABLE_B(p)[k].value) / ((GM_TABLE_B(p)[k + 1].year - GM_TABLE_B(p)[k].year) * 12) + GM_TABLE_B(p)[k].value;
        else if (k > 0)
            a = (d.month + (year - GM_TABLE_B(p)[k - 1].year) * 12) * (GM_TABLE_B(p)[k].value - GM_TABLE_B(p)[k - 1].value) / ((GM_TABLE_B(p)[k].year - GM_TABLE_B(p)[k - 1].year) * 12) + GM_TABLE_B(p)[k - 1].value;
        else
            a = GM_TABLE_B(p)[0].value;
        GM_BLOCK(p)->indexB = a;
    }
    GM_BLOCK(p)->indexB = GM_BLOCK(p)->factorB * GM_BLOCK(p)->indexB / 1000;
    for (i = 0; i < 0x55; i++) {
        kind = ai_Goods[GM_BLOCK(p)->goods[i].item * 0x41];
        if (kind != 0x17 && kind != 0x25) {
            a = gm_GoodsDemand(p, i);
            GM_BLOCK(p)->goods[i].demand = (float)(((float)a - GM_BLOCK(p)->goods[i].demand) * 0.5 + GM_BLOCK(p)->goods[i].demand);
        }
        if (p == ai_OwnPlayer)
            GM_BLOCK(p)->goods[i].stock = gm_GoodsStock(p, i);
        else
            GM_BLOCK(p)->goods[i].stock = GM_BLOCK(p)->goods[i].baseStock;
        GM_BLOCK(p)->goods[i].price = gm_StockPrice(p, i);
        if (GM_BLOCK(p)->goods[i].demandIsStock != 0)
            GM_BLOCK(p)->goods[i].demand = (float)GM_BLOCK(p)->goods[i].stock;
        GM_BLOCK(p)->goods[i].daily[0] = 0;
        GM_BLOCK(p)->goods[i].daily[1] = 0;
        GM_BLOCK(p)->goods[i].daily[3] = 0;
        GM_BLOCK(p)->goods[i].daily[2] = 0;
    }
    if ((int)net_Conn.s == -1) {
        gm_SyncMarket(p);
        gm_RandomMarketShake(p);
    }
}

#define GM_WAIT(h) while (cm_GetSlotState(h) == 0) sv_PumpNet()
#define cm_SendExSellObjectH ((int (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880: it returns the slot handle of its message (ai_Unk4c39c0's result stays in eax), which cm.h's void prototype drops; @original gm_SyncMarket pushes the goods id zero-extended (int), as ai.c's cm_SendExSellObjectI */

/* 0x4fec10, the market sync of the player p (group 1, 5, 10 is the market building ai_MarketBuilding): for the own player the amount flag of every goods record is set when a building other than kind 10 holds the good in its store (0xff object) or needs it for one it holds (the four needs at +0x2e of its ai_Goods record); then every record is synced with the market object of its marketKind (its store created first with cm_SendExSellObject when missing): a flagged good the store lacks is requested (one), an unflagged one it has is sold with its count; each record goes to the server (cm_SendGoodsRecord), and the goods count of the market objects 0x92 .. 0x97, 0x2db raises the level byte +0x1c of their kind 0x11 object. For another player: the goods its two market objects (0x1db / 0x1dc) hold get the offer counter (random, then + 0x18 .. 0x1f per call; offered up to 0x80) and are sent in one group */
void gm_SyncMarket(unsigned p)
{
    int list[8];
    GMSTOCK rec;
    GMSTOCK *stock, *g, *g2;
    unsigned char *b;
    ORDOBJ *o, *o2, *m, *a1;
    char *gp;
    int i, j, k, cnt, h;
    int *lp;

    list[0] = 0x92;
    list[1] = 0x93;
    list[2] = 0x94;
    list[3] = 0x95;
    list[4] = 0x96;
    list[5] = 0x97;
    list[6] = 0x2db;
    list[7] = 0;
    ai_MarketBuilding = (struct AIBLD *)gm_BuildQuery(1, 5, 10);
    if (ai_MarketBuilding == 0)
        return;
    stock = (GMSTOCK *)(init_StockBlocks + p * 0x2a90);
    if (p == ai_OwnPlayer) {
        for (i = 0; i < 0x55; i++)
            stock[i].amount = 0;
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (ai_BTypes[*b * 0x24d] != 10) {
                o = (ORDOBJ *)gm_Query(((ORDBLD *)b)->objList, 1, 0, 0xff);
                for (i = 0; o != 0 && i < 0x55; i++) {
                    {
                        g = &stock[i];
                        if (g->item != 0 && gm_Query(o->list, 1, 0, g->item) != 0) {
                            g->amount = 1;
                            gp = ai_Goods + g->item * 0x41;
                            for (j = 0; j < 0x55; j++) {
                                g2 = &stock[j];
                                for (k = 0; k < 4; k++)
                                    if (g2->item == ((unsigned short *)(gp + 0x2e))[k])
                                        g2->amount = 1;
                            }
                        }
                    }
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        for (i = 0; i < 0x55; i++) {
            g = (GMSTOCK *)(init_StockBlocks + p * 0x2a90) + i;
            rec = stock[i];
            o = (ORDOBJ *)gm_Query(((ORDBLD *)ai_MarketBuilding)->objList, 1, 0, g->marketKind);
            if (o != 0) {
                o2 = (ORDOBJ *)gm_Query(o->list, 1, 0, 0xff);
                if (o2 == 0) {
                    h = cm_SendExSellObjectH(o->objId, -1, 0xff, 1, ai_OwnPlayer, 0);
                    GM_WAIT(h);
                    o2 = (ORDOBJ *)cm_GetSlotValue(h);
                }
                if (g->amount != 0) {
                    m = (ORDOBJ *)gm_Query(o2->list, 1, 0, g->item);
                    if (m == 0) {
                        h = cm_SendExSellObjectH(o2->objId, -1, g->item, 1, ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                } else {
                    m = (ORDOBJ *)gm_Query(o2->list, 1, 0, g->item);
                    if (m != 0) {
                        h = cm_SendExSellObjectH(-1, o2->objId, g->item, m->count, ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                }
                cm_SendGoodsRecord(&rec, p);
            }
        }
        for (lp = list; *lp != 0; lp++) {
            cnt = 0;
            o = (ORDOBJ *)gm_Query(((ORDBLD *)ai_MarketBuilding)->objList, 1, 0, *lp);
            if (o != 0 && (o2 = (ORDOBJ *)gm_Query(o->list, 1, 0, 0xff)) != 0) {
                o2 = (ORDOBJ *)gm_Query(o2->list, 1, 5);
                while (o2 != 0) {
                    cnt++;
                    o2 = (ORDOBJ *)gm_QueryNext();
                }
                o = (ORDOBJ *)gm_Query(o->list, 1, 4, 0x11);
                if (o != 0 && (int)o->lvl1c < cnt) {
                    cnt -= o->lvl1c;
                    cm_BeginFieldBlock((char *)o, o->objId);
                    cm_AddFieldValues(1, 1, (char *)&o->lvl1c - cm_FieldObject, &cnt);
                    cm_SendFieldDeltas();
                }
            }
        }
        return;
    }
    o = (ORDOBJ *)gm_FindPlayerBuilding(p);
    if (o != 0 && (a1 = (ORDOBJ *)gm_Query(((ORDBLD *)o)->objList, 2, 0, 0x1db, 6)) != 0 && (o2 = (ORDOBJ *)gm_Query(((ORDBLD *)o)->objList, 2, 0, 0x1dc, 6)) != 0) {
        cm_BeginSendGroup();
        for (i = 0; i < 0x55; i++) {
            rec = stock[i];
            if (rec.item != 0) {
                m = (ORDOBJ *)gm_Query(a1->list, 1, 0, rec.item);
                if (m == 0)
                    m = (ORDOBJ *)gm_Query(o2->list, 1, 0, rec.item);
                if (m != 0) {
                    if (rec.offer == 0) {
                        rec.offer = (unsigned char)util_RandomU(0xff);
                        rec.amount = rec.offer <= 0x80;
                    } else {
                        rec.offer += util_RandomU(8) + 0x18;
                        rec.amount = rec.offer <= 0x80;
                    }
                } else
                    rec.amount = 0;
                cm_SendGoodsRecord(&rec, p);
            }
        }
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0)
            sv_PumpNet();
    }
}
/* 0x4ff8c0, the market demand of the goods record i of the player p: 0 for the goods kinds 0x17 / 0x25; for player 0 the rating (ai_RatingBaseF) times the record's rate * 0.0025, rounded; for the others the block's demand index times the rate * 0.0005 */

int gm_GoodsDemand(int p, int i)
{
    char *good;

    good = ai_Goods + GM_BLOCK(p)->goods[i].item * 0x41;
    if (*good != 0x17 && *good != 0x25) {
        if (p == 0)
            return (int)(ai_RatingBaseF * GM_BLOCK(0)->goods[i].rate * 0.0025 + 0.5);
        return (int)(GM_BLOCK(p)->demandIndex * GM_BLOCK(p)->goods[i].rate * 0.0005);
    }
    return 0;
}
/* 0x4ff950, the stock of the goods record i of the player p: the sum over the four buildings (the group (1, 5, number) of the bytes at +0x40 + 8k, each with the float at +4 of its slot added once per match) times 0.5, scaled by the float +0x24 / +0x20 when +0x24 is not 0; 0 when the first building byte is 0. ROUGH */

int gm_GoodsStock(int p, int i)
{
    int sum;
    char *g;
    char *s;
    int k, o;

    sum = 0;
    g = GM_GOODS(p, i);
    s = g + 0x40;
    if (*s == 0)
        return 0;
    for (k = 4; k != 0; k--, s += 8) {
        if (*s != 0 && (o = gm_BuildQuery(1, 5, (unsigned char)*s)) != 0) {
            do {
                sum = (int)((float)sum + *(float *)(s + 4));
                o = gm_BuildNext();
            } while (o != 0);
        }
    }
    sum = (int)(sum * 0.5);
    if (*(float *)(g + 0x24) != 0.0f)
        sum = (int)(*(float *)(g + 0x24) / *(float *)(g + 0x20) * sum);
    return sum;
}

extern unsigned short gm_BQMaster, gm_BQOwner;  /* 0x61a674 / 0x61a676 */
extern char gm_BQStopFirst, gm_BQFirstCall; /* 0x6f1bd4, 0x6f1bd5, stop at the first, first call */
ORDBLD *gm_BQCursor = 0;  /* 0x6f1bf0, the current building of the query */
extern unsigned gm_KindObjs[732];
extern unsigned short gm_KindObjCount;
void rt_FreePoints();                     /* 0x4bd9e0 */
/* 0x500190 */

/* 0x500080, an object instance of the prototype kind 0x1d frees the record at +0x29 (0x4bd9e0) */
void gm_FreeObjectExtra(unsigned short *o)
{
    if (ai_Goods[*o * 0x41] == 0x1d && *(int *)((char *)o + 0x29) != 0) {
        rt_FreePoints(*(int *)((char *)o + 0x29));
        *(int *)((char *)o + 0x29) = 0;
    }
}
/* 0x5000c0, removes the object instance with the owner id (+2) from the list *list: like 0x4ffff0 but by the id field */

int gm_RemoveOwned(int *list, int id)
{
    unsigned short *prev, *cur;
    int n;

    n = 0;
    prev = 0;
    if ((unsigned short *)*list == 0)
        return -1;
    cur = (unsigned short *)*list;
    do {
        if (*(int *)(cur + 1) == id) {
            if (*(int *)(cur + 10) != 0)
                gm_FreeList(cur + 10);
            gm_FreeObjectExtra(cur);
            if (n == 0)
                *list = *(int *)((char *)cur + 0x3f);
            else
                *(int *)((char *)prev + 0x3f) = *(int *)((char *)cur + 0x3f);
            *cur = 0;
            f3gm_ObjectCount--;
            return 0;
        }
        n++;
        prev = cur;
        cur = *(unsigned short **)((char *)cur + 0x3f);
    } while (cur != 0);
    return -2;
}
/* 0x500140, frees a whole list of object instances (and, recursively, the lists they hold at +0x14) */

int gm_FreeList(int *list)
{
    int o;

    if (list != 0) {
        o = *list;
        if (o == 0)
            return -1;
        do {
            *list = *(int *)(o + 0x3f);
            if (*(int *)(o + 0x14) != 0)
                gm_FreeList((int *)(o + 0x14));
            gm_FreeObjectExtra((unsigned short *)o);
            o = *list;
        } while (o != 0);
    }
    return 0;
}
/* 0x500970, adds the amount n of the object prototype proto to the owner id (a building / object / character found by 0x4fea50): an existing instance gets +n (+0xe), else 0x500190 creates one; an unknown owner writes the message "gm_AddObjekt(): Parent not found. ObjProt:%i Menge:%i" (the buffer is not shown). Returns the instance or 0 */

int gm_AddObjekt(int id, int proto, int n)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    int *list;
    ORDOBJ *r;
    char buf[512];

    if (proto == 0)
        return 0;
    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, id) == 0) {
        sprintf(buf, "gm_AddObjekt(): Parent not found. ObjProt:%i Menge:%i", proto, n);
        return 0;
    }
    if (b != 0)
        list = &b->objList;
    else if (o != 0)
        list = &o->list;
    else if (c != 0)
        list = &c->inventory;
    else
        return 0;
    r = (ORDOBJ *)gm_Query((void *)*list, 1, 0, proto);
    if (r == 0)
        return (int)gm_CreateObject(id, proto, n);
    r->count += n;
    return (int)r;
}
/* 0x500a40, takes the amount n of the prototype proto from the owner id: 1 when it had enough (the instance is removed when it drops to 0 or below), else 0 */

int gm_TakeObjekt(int id, int proto, int n)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    int *list;
    ORDOBJ *r;

    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, id) == 0)
        return 0;
    if (b != 0)
        list = &b->objList;
    else if (o != 0)
        list = &o->list;
    else if (c != 0)
        list = &c->inventory;
    else
        return 0;
    r = (ORDOBJ *)gm_Query((void *)*list, 1, 0, proto);
    if (r == 0 || r->count < n)
        return 0;
    r->count -= n;
    if (r->count <= 0)
        gm_RemoveObject(list, proto);
    return 1;
}
/* 0x500ae0, like 0x500a40 but creates a missing instance with 0 first and removes it when the amount is exactly 0 (a negative stock is possible) */

int gm_AddNegative(int id, int proto, int n)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    int *list;
    ORDOBJ *r;

    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, id) == 0)
        return 0;
    if (b != 0)
        list = &b->objList;
    else if (o != 0)
        list = &o->list;
    else if (c != 0)
        list = &c->inventory;
    else
        return 0;
    r = (ORDOBJ *)gm_Query((void *)*list, 1, 0, proto);
    if (r == 0) {
        r = (ORDOBJ *)gm_CreateObject(id, proto, 1);
        r->count = 0;
    }
    r->count -= n;
    if (r->count == 0)
        gm_RemoveObject(list, proto);
    return 1;
}
/* 0x500b90, looks the kind of the building b up in gm_BuildingItemGroups (37 records: a kind byte and ten object prototypes) and collects its non-zero prototypes into the list gm_KindObjs (count gm_KindObjCount); 1 when the kind is not there (or no tables), else 0
   @deep the statements match the original (the group lookup, the null test and the first 23 instructions
   are equal); the one remaining hunk at +0x4b is VC6 keeping the global gm_KindObjCount in dx through the
   copy loop (xor dx,dx / mov [cnt],dx / mov di,dx / inc dx / store at exit) where ours reads and
   increments it in memory. Verified: with the definition of gm_KindObjCount removed from f3gm.c, or
   written as a tentative definition 'unsigned short gm_KindObjCount;', the function is byte-identical (0
   ins / 0 bytes); the initialised definition '= 0' that D5 requires blocks the promotion. So the remaining
   difference is the global's definition form / owner (D5 vs the bytes), not this function's statements;
   the branch targets differ only by the shorter loop body. */

int gm_BuildKindObjects(char *b)
{
    G7GRP *g;
    int i, k;

    if (ai_Goods == 0)
        return 1;
    g = 0;
    for (i = 0; i < 37; i++) {
        if (gm_BuildingItemGroups[i].key == ((ORDBLD *)b)->type) {
            g = &gm_BuildingItemGroups[i];
            break;
        }
    }
    if (g == 0)
        return 1;
    gm_KindObjCount = 0;
    for (k = 0; k < 10; k++) {
        if (g->items[k] != 0)
            gm_KindObjs[gm_KindObjCount++] = g->items[k];
    }
    return 0;
}
/* 0x501000, the building of the type 0x47 (group (1, 0, 0x47)) whose int at +0x65 is the player p, 0 when none */

int gm_FindPlayerBuilding(int p)
{
    int b;

    b = gm_BuildQuery(1, 0, 0x47);
    while (b != 0) {
        if (*(int *)(b + 0x65) == p)
            return b;
        b = gm_BuildNext();
    }
    return 0;
}
/* 0x501030, the next building of the query gm_BuildQuery (0x501190) started: walks the cursor gm_BQCursor over the used records of ai_Blds and returns the first that matches every set criterion (type, id, master, owner, the class of its type; gm_BQStopFirst takes any); 0 when the table ends (the cursor is cleared) or more than 0x100 records were tested
   @deep read against the original statement by statement: the two early returns, the first-call /
   next-record walks of the cursor, the criteria chain (type, id, masterId +0x25, owner +0x27, the class
   byte of ai_BTypes, sete), the tested counter, the advance and the end / match returns all match (equal
   compares, operands and loop-invariant loads into edi / bp / bl). Left: the original saves ebx / ebp /
   edi only before the criteria loop (0x5010a4 push ebx .. push edi; the early returns pop esi only, the
   first-call walk jumps to the shared end check 0x50109c), ours pushes all four in the prolog and so
   duplicates every early return with four pops; and the 'm = 0' block (xor cl,cl; jmp) sits after the
   match return in the original, inline in ours. Tried the criteria as a goto chain, as an if/else with the
   no label in the else, with the no block after the loop's continue, and as nested ifs with separate m = 0
   (56 ins, worse): VC6 lays the first three out identically, so the block placement follows from the late
   register saves, not from the statements (size: 2 of the instruction difference are shared / inline
   tails) */

unsigned char *gm_BuildNext(void)
{
    char m;
    ORDBLD *end;

    m = 0;
    end = (ORDBLD *)ai_Blds + 0x100;
    if (ai_BTypes == 0)
        return 0;
    if (gm_BQTested > 0x100)
        return 0;
    if (gm_BQFirstCall) {
        gm_BQFirstCall = 0;
        while (gm_BQCursor < end && gm_BQCursor->type == 0)
            gm_BQCursor++;
    } else {
        do
            gm_BQCursor++;
        while (gm_BQCursor < end && gm_BQCursor->type == 0);
    }
    if (gm_BQCursor >= end)
        return 0;
    for (;;) {
        if (gm_BQStopFirst)
            m = 1;
        if (gm_BQType == 0xff || gm_BQCursor->type == gm_BQType) {
            if (gm_BQType != 0xff)
                m = 1;
            if (gm_BQId != -1) {
                if (gm_BQCursor->id != gm_BQId)
                    goto no;
                m = 1;
            }
            if (gm_BQMaster != 0x7fff) {
                if (gm_BQCursor->masterId != gm_BQMaster)
                    goto no;
                m = 1;
            }
            if (gm_BQOwner != 0x7fff) {
                if (gm_BQCursor->owner != gm_BQOwner)
                    goto no;
                m = 1;
            }
            if (gm_BQClass != 0xff)
                m = (unsigned char)ai_BTypes[gm_BQCursor->type * 0x24d] == gm_BQClass;
        } else {
        no:
            m = 0;
        }
        gm_BQTested++;
        if (!m) {
            do
                gm_BQCursor++;
            while (gm_BQCursor < end && gm_BQCursor->type == 0);
        }
        if (gm_BQCursor >= end) {
            if (!m)
                gm_BQCursor = 0;
            return (unsigned char *)gm_BQCursor;
        }
        if (m)
            return (unsigned char *)gm_BQCursor;
    }
}
/* 0x501190, starts a building query and returns its first match (gm_BuildNext): resets the criteria, then reads n (kind, value) pairs: 0 the type, 1 the id, 2 a name (copied to gm_BQName, never tested), 3 the master id, 4 the owner, 5 the class of the type, 6 stop at the first (no value); the cursor starts at ai_Blds. Used all over the exe (gm_BuildQuery(1, 5, 10) ..)
   @deep read against the original: the reset of the nine criteria, the (kind, value) walk with a
   pre-incremented argument pointer, the seven cases (switch tables equal, strcpy intrinsic for the name)
   and the tail call to gm_BuildNext all match; gm_BQClass is unsigned char like gm_BQType (the original
   stores one 0xff register into both). The one hunk left (+0xc, 10 lines) is the same five instructions
   scheduled and allocated differently: the original loads 0xff into cl, stores type / class, then loads n
   into ecx; ours loads n into ecx first and takes al for 0xff (its short mov [mem], al form makes ours 2
   bytes smaller). Tried the class / type stores chained and the copy locals reordered: no change */

unsigned char *gm_BuildQuery(int n, ...)
{
    int *ap;
    int i;

    if (ai_BTypes == 0)
        return 0;
    gm_BQType = 0xff;
    gm_BQClass = 0xff;
    gm_BQId = -1;
    gm_BQName = 0;
    gm_BQMaster = 0x7fff;
    gm_BQOwner = 0x7fff;
    gm_BQFirstCall = 1;
    gm_BQTested = 0;
    gm_BQStopFirst = 0;
    if (n > 0) {
        ap = &n;
        for (i = n; i != 0; i--) {
            switch ((unsigned char)*++ap) {
            case 0:
                gm_BQType = (unsigned char)*++ap;
                break;
            case 1:
                gm_BQId = *++ap;
                break;
            case 2:
                strcpy(gm_BQName, (char *)*++ap);
                break;
            case 3:
                gm_BQMaster = (unsigned short)*++ap;
                break;
            case 4:
                gm_BQOwner = (unsigned short)*++ap;
                break;
            case 5:
                gm_BQClass = (unsigned char)*++ap;
                break;
            case 6:
                gm_BQStopFirst = 1;
                break;
            }
        }
    }
    gm_BQCursor = (ORDBLD *)ai_Blds;
    return gm_BuildNext();
}
/* 0x5012b0, the first free record of the building table (0x100 records of 0xa9 bytes), 0 when it is full */

char *gm_FindFreeBuilding(void)
{
    ORDBLD *end, *b;

    end = (ORDBLD *)ai_Blds + 0x100;
    for (b = (ORDBLD *)ai_Blds; b < end; b++) {
        if (b->type == 0)
            break;
    }
    if (b >= end)
        return 0;
    return (char *)b;
}
/* 0x5012e0, frees the building b: a player's building (its owner a character of kind 6) frees its production setting (ai_FreeProdSetting); by type, 0x1e frees its plant records, 0x48 .. 0x4a its graveyard record, 0x4b .. 0x4d its heap block; every character living or working in it loses it, its object list is freed and the type set to 0 (free). Returns 0, -1 for no building, -2 for one already free */

int gm_FreeBuilding(char *p)
{
    ORDBLD *b = (ORDBLD *)p;
    int i;

    if (b == 0)
        return -1;
    if (b->type == 0)
        return -2;
    if (b->owner != 0xffff && ORD_CHARS[b->owner].kind == 6)
        ai_FreeProdSetting(b->id);
    switch (b->type) {
    case 0x1e:
        if (b->plants != 0) {
            mem_Free(b->plants);
            b->plants = 0;
        }
        b->plants = 0;
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        gv_FreeGraveyard((int *)b->block65);
        b->block65 = 0;
        break;
    case 0x4b:
    case 0x4c:
    case 0x4d:
        mem_Free(b->block65);
        b->block65 = 0;
        break;
    }
    for (i = 0; i < 0x300; i++) {
        if (ORD_CHARS[i].bld2 == b)
            ORD_CHARS[i].bld2 = 0;
        if (ORD_CHARS[i].bld == b)
            ORD_CHARS[i].bld = 0;
    }
    if (b->objList != 0)
        gm_FreeList(&b->objList);
    b->type = 0;
    return 0;
}

extern int gm_NOffers;
int gm_ListStock(short *l, int id);     /* 0x50ab50 */
int gm_ListAmount(short *l, char *o);   /* 0x50aaf0 */
float gm_GoodsPrice(unsigned id, unsigned char p);                   /* 0x5089c0 */
unsigned short * gm_ObjectBuildingOwner(unsigned short *p);                     /* 0x50a050 */
void init_HookObject(char *o, unsigned short *p, int kind);                     /* 0x50d380 */
struct CHTRANSPORT * ch_AttachTransport();                     /* 0x4034e0 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */
struct CHARACTER * ch_Create();                     /* 0x403390 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
int gm_IsHomeType();                     /* 0x502520 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */

/* 0x500190, creates an object instance of the prototype proto with the amount n in the list of the holder id (a building, an object or a character, gm_FindById): takes a free record (gm_FindFreeObject) and appends it to the holder's list (ORDBLD.objList, ORDOBJ.list or the character's list at +0x178), fills it (the prototype, a new object id ai_NextObjectId, the holder id, the holder's own id, the amount, fill 100) and counts it (f3gm_ObjectCount); a few prototypes get their own start values (0xb8, 300, 0x12d, 0x15e / 0x160, 0x174 / 0x176, 0x24b / 0x256 / 0x266), the goods kinds 0x17 / 0x20 / 0x25 clear +0x1c, and the three carts 0x134 / 0x135 / 0x136 get the id of their building, a load object (prototype 0x1dd), a model (a cart man or the horse, ch_Create at the building's "dummy_TRANSPORTER") pulling the cart object ob_KARREN_klein / _mittel / _GROSS (textured by the owner character's v054). Returns the instance, 0 on failure
   @deep read the whole function against 0x500190..0x5007a4 and reconciled every statement: the holder
   lookup and its list (ORDBLD.objList / ORDOBJ.list / the character's +0x178), the tail walk, the record
   fill in the original's order, the 0x1f-byte clear (rep stosd 7 + stosw + stosb, now equal), the one
   switch on proto as an int (jg / the two byte tables equal), the case order 300 / 0x12d / 0xb8 / carts,
   the cart branch (holder netId, the walk up to the building, the 0x1dd load object, the scene, the
   dummy_TRANSPORTER position, the four ch_Create blocks, the three d3_CreateObject blocks, the texture set
   from the owner character's v054, the group by the home building). Tried: a switch vs an if chain for the
   random model (the if chain matches block for block), the home tail as if/else-if (matches block for
   block), set as char / unsigned char / a cast sum, the dummy ternary with the cast outside. Left is
   allocation: the original keeps ai_Goods cached in edi and the list in ebp, so last (and later the
   texture set) lives in the stack slot [esp+0x1c] (0x50023f / 0x500259), ours keeps the list in edi and
   last in a register - that is the frame 0x430 vs 0x434 and the 8-byte shift of the three jumps to the
   ch_Create call (+0x24e vs +0x256); the pr computation 'add edi,ecx; add eax,edi' vs 'add eax,ebp; add
   eax,ebx' and the sprintf argument loads are the same values in other registers (0x500276); the id / n
   loads before the stores (0x5002e6), 'test ebp' vs 'mov ecx,[b]; test ecx' (0x500394) and 'mov edi, id'
   vs 'mov edi, ebp' (0x5003de) follow from b and id in other registers; the texture byte: ours takes the
   flags of 'add dl,0x67', the original adds 'test al,al' (0x5006a7); the rest diffclass explains (regs 80,
   slots 44, frame 76, order 22) */
unsigned short *gm_CreateObject(int id, int proto, int n)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    ORDOBJ *last;
    ORDOBJ *r;
    int *list;
    char *pr;
    char buf[1024];
    float pos[4];
    float rot[3];
    ORDOBJ *it;
    ORDOBJ *load;
    unsigned char *home;
    int scene, saved, holder;
    D3OBJECT *dummy;
    D3OBJECT *cart;
    CHARACTER *model;
    ORDCHAR *owner;
    char *name;
    unsigned char set;
    int found;
    int group;
    int pick;

    b = 0;
    o = 0;
    c = 0;
    last = 0;
    r = 0;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    pos[3] = 0;
    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    pr = ai_Goods;
    if (ai_Goods == 0 || id == -1 || gm_FindById(&b, &o, &c, id) == 0)
        return 0;
    if (b != 0)
        list = &b->objList;
    else if (o != 0)
        list = &o->list;
    else if (c != 0)
        list = (int *)((char *)c + 0x178);
    else
        return 0;
    if (list != 0) {
        it = (ORDOBJ *)*list;
        if (it != 0) {
            do {
                last = it;
                it = it->next;
            } while (it != 0);
            r = (ORDOBJ *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            last->next = r;
        } else {
            r = (ORDOBJ *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            *list = (int)r;
        }
        pr = ai_Goods;
    }
    pr = pr + proto * 0x41;
    r->type = (unsigned short)proto;
    sprintf(buf, "----->ObjID= %i ObjProt= %i", ai_NextObjectId, proto & 0xffff);
    r->objId = ai_NextObjectId;
    ai_NextObjectId++;
    if (b != 0)
        r->holderId = b->id;
    else if (o != 0)
        r->holderId = o->holderId;
    else if (c != 0)
        r->holderId = c->netId;
    r->ownerId = id;
    r->count = n;
    r->fill = 100;
    r->b13 = 0;
    r->list = 0;
    r->next = 0;
    f3gm_ObjectCount++;
    memset(r->v1c, 0, 0x1f);
    switch (proto) {
    case 300:
        r->v1c[0] = -1;
        r->v1c[1] = -1;
        r->v1c[2] = -1;
        r->v1c[3] = ai_TimeT.year - 1;
        break;
    case 0x12d:
        r->v1c[0] = -1;
        r->v1c[1] = -1;
        r->v1c[2] = -1;
        r->v1c[3] = -1;
        break;
    case 0xb8:
        r->lvl1c = 6;
        break;
    case 0x134:
    case 0x135:
    case 0x136:
        if (b != 0)
            r->holderId = ORD_CHARS[b->owner].netId;
        else if (o != 0)
            r->holderId = ((ORDCHAR *)gm_ObjectBuildingOwner((unsigned short *)o))->netId;
        else if (c != 0)
            r->holderId = c->netId;
        holder = id;
        found = gm_FindById(&b, &o, &c, holder);
        while (found != 0 && b == 0) {
            if (o != 0)
                holder = o->ownerId;
            found = gm_FindById(&b, &o, &c, holder);
        }
        r->cart28 = 0;
        r->b31 = 2;
        if (b != 0)
            r->cartBld = b->id;
        else
            r->cartBld = -1;
        load = (ORDOBJ *)gm_CreateObject(r->objId, 0x1dd, 1);
        load->count = r->type - 0x133;
        load->lvl1c = (unsigned char)r->type - 0x33;
        home = gm_BuildQuery(1, 1, id);
        scene = 0;
        if (home != 0 && gm_IsHomeType(home) != 0)
            scene = init_BuildingSceneUniverse(home);
        saved = d3_CurrentScene;
        d3if_SelectScene(scene, 1);
        dummy = d3_FindObject(gm_IsHomeType(home) != 0 ? 0 : (int)((ORDBLD *)home)->object, 0x100, 0, "dummy_TRANSPORTER");
        if (dummy != 0)
            d3_PointToWorld(dummy, &dummy->pos, (D3VECTOR *)pos);
        else if (((ORDBLD *)home)->object != 0) {
            pos[0] = ((ORDBLD *)home)->object->pos.x;
            pos[1] = ((ORDBLD *)home)->object->pos.y;
            pos[2] = ((ORDBLD *)home)->object->pos.z;
        } else {
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0;
        }
        if (proto != 0x136) {
            pick = util_RandomU(3) & 0xffff;
            if (pick == 0)
                model = ch_Create("arbeiter_KARREN", (D3VECTOR *)pos);
            else if (pick == 1)
                model = ch_Create("holzfaeller_KARREN", (D3VECTOR *)pos);
            else
                model = ch_Create("handwerker_KARREN", (D3VECTOR *)pos);
        } else
            model = ch_Create("pferd_PFERD", (D3VECTOR *)pos);
        r->model = model;
        ch_PreloadAnims(model, 1, "bewegung/karren_ziehen");
        init_HookObject((char *)model->obj, (unsigned short *)r, 1);
        sprintf(model->obj->name, "sp_%i", r->objId);
        if (proto == 0x134)
            cart = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ob_KARREN_klein");
        else if (proto == 0x135)
            cart = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ob_KARREN_mittel");
        else if (proto == 0x136)
            cart = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ob_KARREN_GROSS");
        else
            cart = 0;
        if (cart != 0) {
            cart->stateFlags = cart->stateFlags & 0xf7 | 4;
            cart->objectFlags = cart->objectFlags & 0xfd;
            ch_AttachTransport(model, cart);
            owner = c;
            if (owner == 0 && b != 0 && b->owner != 0xffff)
                owner = &ORD_CHARS[b->owner];
            if (owner != 0 && owner->v054 != 0 && (owner->kind == 5 || owner->kind == 6 || owner->kind == 7)) {
                set = (unsigned char)owner->v054 + 0x67;
                if (set != 0)
                    d3if_SelectTextureSet(cart, (struct D3SURF *)cart->info, set, 1);
            }
            cart->mask = 1;
            init_HookObject((char *)cart, (unsigned short *)r, 1);
        }
        if (home == 0) {
            model->scriptGroup = -1;
            group = -1;
        } else if (gm_IsHomeType(home) == 0) {
            ch_SetVisible(model, 0);
            model->scriptGroup = -1;
            group = -1;
        } else if (gm_IsHomeType(home) != 0) {
            group = ((ORDBLD *)home)->id;
            model->scriptGroup = -1;
        } else {
            model->scriptGroup = -1;
            group = -1;
        }
        model->group = group;
        r->cart24 = 0;
        d3if_SelectScene(saved, 1);
        break;
    case 0x15e:
    case 0x160:
        r->v1c[1] = 0xf;
        break;
    case 0x174:
    case 0x176:
        r->v1c[1] = 6;
        break;
    case 0x24b:
    case 0x256:
    case 0x266:
        r->v1c[0] = 0;
        r->v1c[1] = 0;
        break;
    }
    if (*pr == 0x17 || *pr == ' ' || *pr == '%')
        r->v1c[0] = 0;
    return (unsigned short *)r;
}
/* 0x500ec0, the market object of the town market building (0x6f1bc5's market 0x6f1bc8) that holds the goods id: looks through its objects 0x92 .. 0x97 / 0x2db for one with the goods id (query (2, 6, 0, id)) and returns its 0x116 object, 0 when none */

int gm_FindMarketObject(int id)
{
    int l[8];
    int o, i;

    if (ai_MarketBuilding != 0) {
        l[0] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x92);
        l[1] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x93);
        l[2] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x94);
        l[3] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x95);
        l[4] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x96);
        l[5] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x97);
        l[6] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x2db);
        l[7] = 0;
        i = 0;
        for (o = l[0]; o != 0; o = l[i]) {
            if (gm_Query(*(int *)(o + 0x14), 2, 6, 0, id) != 0)
                return (int)gm_Query(*(int *)(l[i] + 0x14), 1, 0, 0x116);
            i++;
        }
    }
    return 0;
}
/* 0x500c20, collects the offers of the goods id from the 0x58 buildings into gm_Offers (count gm_NOffers): the market building offers what its market object (gm_FindMarketObject) holds, any other building flags the goods in its store object (0xff) and offers what its stall (0x2a) holds; the supply is the stall's amount (gm_ListAmount) less a quarter of its stock (gm_ListStock) times the goods' +0x1c, not below 0; each offer's price is gm_GoodsPrice(goods, ai_OwnPlayer) times the building's price factor, and the offers are sorted by price, highest first. Returns 0, 1 without the tables
   @deep read against the original statement by statement: the building walk, the three row clears, the
   market branch (gm_FindMarketObject, gm_Query of its list), the store-flag branch (0xff), the stall
   branch (0x2a), the supply (gm_ListAmount - (unsigned)(gm_ListStock * o->v1c[0]) >> 2, clamped by the
   sets/dec/and pair after a first store), the price loop and the selection sort with the struct swap all
   match (equal call sequences, operands, constants and field offsets). What is left: VC6 cross-jumps the
   identical supply tail of the two branches (push edi .. mov [gm_NOffers], edx) into the stall copy in the
   original (the market copy ends 'push ecx; jmp 0x500d84') but into the market copy in ours (ours 'jmp
   0x500cdb'); tried a goto into the stall copy, a shared tail after the if/else (both lay the market block
   out of line: 199 ins) and the branches swapped (78 ins). The other hunks follow from it: the zero
   register esi rematerialized at the loop end in the original (0x500de3 xor esi,esi) vs a jmp over it at
   our loop head, stored = 1 as an immediate vs ebx, cmp edx,esi vs test edx,edx */

int gm_CollectOffers(int goods)
{
    ORDBLD *b;
    ORDOBJ *s, *mk, *o;
    AISTORE tmp;
    int k, i, j, f;

    if (ai_BTypes == 0)
        return 1;
    gm_NOffers = 0;
    b = (ORDBLD *)ai_Blds;
    for (k = 0x58; k != 0; k--, b++) {
        if (b->type == 0)
            continue;
        f = 0;
        gm_Offers[gm_NOffers].bld = 0;
        gm_Offers[gm_NOffers].goodsObj = 0;
        gm_Offers[gm_NOffers].stored = 0;
        if (b == (ORDBLD *)ai_MarketBuilding) {
            mk = (ORDOBJ *)gm_FindMarketObject(goods);
            if (mk != 0 && (o = (ORDOBJ *)gm_Query((void *)mk->list, 1, 0, goods)) != 0) {
                gm_Offers[gm_NOffers].bld = (struct AIBLD *)b;
                gm_Offers[gm_NOffers].market = (int)mk;
                gm_Offers[gm_NOffers].goodsObj = (int)o;
                gm_Offers[gm_NOffers].count = gm_ListAmount((short *)mk, (char *)o)
                    - ((unsigned)(gm_ListStock((short *)mk, o->type) * o->v1c[0]) >> 2);
                gm_Offers[gm_NOffers].count = gm_Offers[gm_NOffers].count < 0 ? 0 : gm_Offers[gm_NOffers].count;
                gm_NOffers++;
            }
        } else {
            s = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0xff);
            if (s != 0 && gm_Query((void *)s->list, 1, 0, goods) != 0) {
                f = 1;
                gm_Offers[gm_NOffers].bld = (struct AIBLD *)b;
                gm_Offers[gm_NOffers].stored = 1;
            }
            mk = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
            if (mk != 0 && (o = (ORDOBJ *)gm_Query((void *)mk->list, 1, 0, goods)) != 0) {
                gm_Offers[gm_NOffers].bld = (struct AIBLD *)b;
                gm_Offers[gm_NOffers].market = (int)mk;
                gm_Offers[gm_NOffers].goodsObj = (int)o;
                gm_Offers[gm_NOffers].count = gm_ListAmount((short *)mk, (char *)o)
                    - ((unsigned)(gm_ListStock((short *)mk, o->type) * o->v1c[0]) >> 2);
                gm_Offers[gm_NOffers].count = gm_Offers[gm_NOffers].count < 0 ? 0 : gm_Offers[gm_NOffers].count;
                gm_NOffers++;
            } else if (f)
                gm_NOffers++;
        }
    }
    for (i = 0; i < gm_NOffers; i++)
        gm_Offers[i].price = gm_GoodsPrice(goods, ai_OwnPlayer) * ((ORDBLD *)gm_Offers[i].bld)->priceFactor;
    for (i = 0; i < gm_NOffers; i++) {
        for (j = i + 1; j < gm_NOffers; j++) {
            if (gm_Offers[j].price > gm_Offers[i].price) {
                tmp = gm_Offers[i];
                gm_Offers[i] = gm_Offers[j];
                gm_Offers[j] = tmp;
            }
        }
    }
    return 0;
}

int gm_IsHomeType(unsigned char *b);
int gm_IsType10(unsigned char *b);
int gm_BuildingClass(int kind);

/* 0x501400, stocks a new building b with the objects its building kind needs (gm_BuildingItemGroups: the kind byte and ten object prototypes): the container object 0xff (query (2, 6, 0, 0xff), created by gm_CreateObject when missing) gets each listed object with the amount 2 when it has none */
void gm_StockBuildingObjects(char *p)
{
    ORDBLD *b = (ORDBLD *)p;
    ORDOBJ *o;
    int i, k;

    for (i = 0; i < 37; i++) {
        if (b->type == gm_BuildingItemGroups[i].key) {
            o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0xff);
            if (o != 0 || (o = (ORDOBJ *)gm_CreateObject(b->id, 0xff, 1)) != 0) {
                for (k = 0; k < 10; k++) {
                    if (gm_BuildingItemGroups[i].items[k] != 0 && gm_Query((void *)o->list, 1, 0, gm_BuildingItemGroups[i].items[k]) == 0)
                        gm_CreateObject(o->objId, gm_BuildingItemGroups[i].items[k], 2);
                }
            }
        }
    }
}
/* 0x5014b0, sets the capacities of the objects of building b from its kind record (bytes +0x23d / +0x23e / +0x23f): the object 0x2a gets cap[0] = the first, cap[1] = the second (each at least 2), or cap[0] = the third when the first two are 0; the object 0x116 gets the third in cap[0] in that case */

void gm_SetCapacities(unsigned char *p)
{
    ORDBLD *b = (ORDBLD *)p;
    unsigned char *k;
    ORDOBJ *o;

    k = (unsigned char *)ai_BTypes + b->type * 0x24d;
    o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 0, 0x2a, 6);
    if (o != 0) {
        if (k[0x23d] == 0 && k[0x23e] == 0 && k[0x23f] != 0) {
            o->cap[0] = k[0x23f];
        } else {
            o->cap[0] = k[0x23d] < 2 ? 2 : k[0x23d];
            o->cap[1] = k[0x23e] < 2 ? 2 : k[0x23e];
        }
    }
    o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 0, 0x116, 6);
    if (o != 0 && k[0x23d] == 0 && k[0x23e] == 0 && k[0x23f] != 0)
        o->cap[0] = k[0x23f];
}

int gv_BuryNewDead();                     /* 0x4ef9c0 */
void *gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */
int gm_BuildingClass();                     /* 0x501d80 */
/* ai_BuildingNames: view of txt_Ptrs (build/mk_txt_views.py) */
extern char ai_Time[];             /* 0x770ae0 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
unsigned short * gm_AllocRaum(unsigned char *b, unsigned n);

/* 0x501580, creates a building of the kind k for the owner own (a character slot, 0xffff none) in the next free instance (gm_FindFreeBuilding): a new id (ai_NextObjectId), the owner and master words, the start values (budget 0x7d00, wage rate 100, level 100, price factor 1.0, no smoke effect), a player's production setting (ai_ProdSetting), a name picked at random from the up to 12 names of its kind that no building has yet (the kind's own name when none is free; "Building name too long: %s" is the unused error text), the objects of its prototype (the rooms of the kinds 2 / 6 by gm_AllocRaum, the others while no room came before), its main room and the per-kind extras: the creation time and the owner's netId for the kinds 4 .. 7, -1 for 0xb .. 0xd, 10 / 3 / 5 for 0xe .. 0x10, the object 0x1b5 for 0x14 .. 0x16, the "f3_gm:PlantMap" of 0x1e, the price factor 0.8 of 0x1e .. 0x20, the graveyard of 0x48 .. 0x4a, the "AHM_INFO" block of 0x4b .. 0x4d; then the capacities and the kind's stock (gm_SetCapacities, gm_StockBuildingObjects). Returns the building, 0 when there are no tables or no free instance
   @deep Read whole against the original 0x501580..0x501b4e and rewritten on typed ORDBLD / ORDOBJ fields
   (statements, constants, callees, the AI_ZERO asm clear of +0x65, the case order with the 4..7 else -1,
   the 0x1b5 object bytes, the word stores of 0x35..0x37 all match; switch tables equal). What is left is
   allocation: the original keeps base (ai_BTypes) in edi from the prologue (push edi first, so both early
   returns pop edi; ours keeps it in edx and pushes edi only before the asm block, popping it before the
   0x26 test: hunks +0x9 / +0x2d / +0x35 / +0x102 / +0x516 / +0x53e), it holds any in edi and w in the slot
   [ebp-4] shared with n (ours any in [ebp-0x14], w in edi: hunks +0x2bc / +0x2ea / +0x31a / +0x33c /
   +0x356), keeps 0 in ebx and re-zeroes it after the ai_Goods temp (ours zero in edi: +0x383, cmp eax,ebx
   vs edi), and the arrays sit at other frame slots (err/names swapped: +0x18b / +0x2d7; declaration order
   tried, all 24 permutations unchanged at 147); the ai_Goods index id*0x41 as mov ecx,eax; add ecx,ebx vs
   [eax+ebx] is the same expression in another register (an explicit id local tried, unchanged).
   Instruction count 422 vs 431: the 2 early-return pop edi, the late xor ebx,ebx and that 3-instruction
   lookup. */
unsigned char *gm_CreateBuilding(unsigned char k, unsigned short own)
{
    ORDBLD *b;
    char *base;
    unsigned char *proto;
    char *name;
    int *info;
    char dbg[0x100];
    char names[12][0x40];
    char cand[0x100];
    char err[0x200];
    int i, j, n, any, id;
    unsigned short *w;
    unsigned short *s;
    ORDOBJ *o;
    char c;

    base = ai_BTypes;
    if (base == 0)
        return 0;
    any = 0;
    b = (ORDBLD *)gm_FindFreeBuilding();
    if (b == 0)
        return 0;
    b->type = k;
    proto = (unsigned char *)base + k * 0x24d;
    sprintf(dbg, "----->GebID= %i GebProt= %i", ai_NextObjectId, k);
    b->id = ai_NextObjectId;
    ai_NextObjectId++;
    b->masterId = own;
    b->owner = own;
    b->mainRoom = 0;
    b->v2b = 0;
    b->b2f = 0;
    b->objList = 0;
    b->object = 0;
    b->v30 = 0;
    b->budget = 0x7d00;
    b->wageRate = 100;
    b->workRate = 2;
    b->v45 = 100;
    b->priceFactor = 1.0f;
    b->level = 100;
    b->smokeEffect = -1;
    if (own != 0xffff && ORD_CHARS[own].kind == 6)
        ai_ProdSettingK(b);
    memset(&b->flags99, 0, 0x10);
    b->flags99 |= 1;
    b->va5 = -1;
    AI_ZERO(&b->v65, 0x30);
    n = 0;
    name = (char *)ai_BuildingNames[k * 0xe];
    if (*name != 0)
        strcpy(b->name, (char *)ai_BuildingNames[k * 0xe - 2]);
    else
        b->name[0] = ch_EmptyName[0];
    for (i = 0; i < 0xc; i++) {
        name = (char *)ai_BuildingNames[k * 0xe + i];
        if (*name != 0) {
            strcpy(cand, (char *)ai_BuildingNames[k * 0xe + i]);
            for (j = 0; j < 0x100; j++) {
                if (((ORDBLD *)ai_Blds)[j].type != 0 && strcmp(((ORDBLD *)ai_Blds)[j].name, cand) == 0)
                    break;
            }
            if (j < 0x100)
                continue;
            if (strlen(cand) < 0x20)
                strcpy(names[n++], cand);
            else
                sprintf(err, "Building name too long: %s", cand);
        }
    }
    if (n != 0)
        strcpy(b->name, names[util_RandomU(n)]);
    w = (unsigned short *)(proto + 0x23);
    for (i = 0; i < 0x40; i++, w++) {
        if (*w == 0)
            break;
        if (*w != 0xffff) {
            id = *w & 0x7fff;
            c = ai_Goods[id * 0x41];
            if (c == 2 || c == 6)
                any = 1;
            if (*w & 0x8000) {
                if (c == 2 || c == 6) {
                    any = 1;
                    gm_AllocRaum((unsigned char *)b, id);
                } else if (any == 0)
                    gm_CreateObject(b->id, id, 1);
            }
        }
    }
    s = (unsigned short *)gm_Query((void *)b->objList, 1, 4, 2);
    while (s != 0) {
        if (*s != 0xfd) {
            b->mainRoom = *s;
            break;
        }
        s = (unsigned short *)gm_QueryNext();
    }
    s = (unsigned short *)gm_GetGebaeudeHauptraum((unsigned char *)b);
    if (s != 0)
        b->mainRoom = *s;
    if (gm_IsHomeType((unsigned char *)b) != 0)
        b->v30 = 0x1388;
    switch (k) {
    case 4:
    case 5:
    case 6:
    case 7:
        b->created = ai_TimeT;
        if (own != 0xffff)
            b->v65 = ORD_CHARS[own].netId;
        else
            b->v65 = -1;
        break;
    case 0x14:
    case 0x15:
    case 0x16:
        o = (ORDOBJ *)gm_CreateObject(b->id, 0x1b5, 1);
        if (o != 0) {
            o->v1b5d = 0;
            o->v1b5a = -1;
            o->v1b5b = -1;
            o->v1b5c = -1;
        }
        break;
    case 0x47:
        b->v30 = 0x13880;
        break;
    case 0x1e:
        b->plants = (struct G15PLANT *)mem_Alloc(0x600, "f3_gm:PlantMap");
        for (i = 0; i < 0x40; i++) {
            b->plants[i].state = -1;
            b->plants[i].obj = 0;
        }
    case 0x1f:
    case 0x20:
        b->v30 = 100;
        b->priceFactor = 0.8f;
        break;
    case 0x35:
    case 0x36:
    case 0x37:
        b->v65 = 0;
        b->w69[0] = 0;
        b->w69[1] = 0;
        b->v6d = 0;
        break;
    case 0xb:
    case 0xc:
    case 0xd:
        b->v65 = -1;
        break;
    case 0xe:
    case 0xf:
    case 0x10:
        b->v65 = 10;
        b->daySum = 3;
        b->v6d = 5;
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        b->block65 = (int)gv_AllocGraveyard((unsigned char *)b);
        gv_BuryNewDead((unsigned char *)b, 3);
        break;
    case 0x4b:
    case 0x4c:
    case 0x4d:
        info = (int *)mem_Alloc(0x3c, "AHM_INFO");
        b->block65 = (int)info;
        *info = -1;
    }
    if (b->type == 0x26)
        b->mainRoom = 0x10e;
    gm_SetCapacities((unsigned char *)b);
    gm_BuildingClass(b->type);
    gm_StockBuildingObjects((char *)b);
    return (unsigned char *)b;
}
/* 0x501b50, the main object of the building b, chosen by its building class (gm_BuildingClass): the classes 1 / 7 / 8 look by kind for the object 0x205 (kind 0x1d), 0x12e (0x16), 0x54 (4) or 0xfd (0x1f); class 4 takes the object 0xf7, a home (gm_IsHomeType) the 0xfd; otherwise (or when that object is missing) the first room of the kind's prototype list (an object of goods class 2 other than 0xfd). Returns the object, 0 when there is none
   @deep Read whole against the original 0x501b50..0x501cdd and rewritten on ORDBLD: the two prototype-list
   loops are two loops in the original too (one shared loop tried: 280 vs 397 bytes, reverted), the kind
   chain 0x1d / 0x16 / 4 / 0x1f is an if-chain in source order, and the 1/7/8 path loops only when o == 0.
   What is left is where o lives: the original keeps o in the stack slot [esp+0x10] (push ecx; mov
   [esp+0x10],0; mov [esp+0x10],eax after the shared query; mov eax,[esp+0x10] at both loop ends), ours
   keeps it in ebx (xor ebx,ebx reused as the push 0 of the 0xf7 query, mov ebx,eax / test ebx,ebx, mov
   eax,ebx and xor eax,eax at the loop ends); with ebx taken, ours reloads ai_Goods into ebp inside the
   second loop (mov ebp,[ai_Goods] at +0x133) where the original hoists it into edi (+0x137) and uses ebx
   for id*0x40. The three differing branch targets are those same epilogues: the original's if (o != 0)
   return o jumps to the query-tail epilogue (eax is o there), ours to the ebx epilogue; offsets shift by
   the 4-byte prologue. Declaration order and int vs pointer tried, unchanged at 29. */

int gm_BuildingMainObject(unsigned char *p)
{
    ORDBLD *b = (ORDBLD *)p;
    int cls, i;
    char *o;
    unsigned char *k;
    unsigned short *w;
    unsigned id;

    o = 0;
    cls = gm_BuildingClass(b->type);
    k = (unsigned char *)ai_BTypes + b->type * 0x24d;
    if (cls != 1 && cls != 7 && cls != 8) {
        if (cls == 4)
            return (int)gm_Query((void *)b->objList, 1, 0, 0xf7);
        if (gm_IsHomeType((unsigned char *)b) != 0)
            return (int)gm_Query((void *)b->objList, 1, 0, 0xfd);
        w = (unsigned short *)(k + 0x23);
        for (i = 0; i < 0x40; i++, w++) {
            if (*w != 0xffff) {
                id = *w & 0x7fff;
                if (ai_Goods[id * 0x41] == 2 && id != 0xfd)
                    return (int)gm_Query((void *)b->objList, 1, 0, id);
            }
        }
        return (int)o;
    }
    if (*k == 0x1d)
        o = gm_Query((void *)b->objList, 1, 0, 0x205);
    else if (*k == 0x16)
        o = gm_Query((void *)b->objList, 1, 0, 0x12e);
    else if (*k == 4)
        o = gm_Query((void *)b->objList, 1, 0, 0x54);
    else if (*k == 0x1f)
        o = gm_Query((void *)b->objList, 1, 0, 0xfd);
    if (o == 0) {
        w = (unsigned short *)(k + 0x23);
        for (i = 0; i < 0x40; i++, w++) {
            if (*w != 0xffff) {
                id = *w & 0x7fff;
                if (ai_Goods[id * 0x41] == 2 && id != 0xfd)
                    return (int)gm_Query((void *)b->objList, 1, 0, id);
            }
        }
    }
    return (int)o;
}
/* 0x501ce0, the main room of the building b ("gm_GetGebaeudeHauptraum", ai.c's error texts): the object 0xfd of a home (gm_IsHomeType), the 0x54 of a kind-type 4 building, else the first object of the goods class 2 in its list that is not the 0xfd; 0 when there is none */

void *gm_GetGebaeudeHauptraum(unsigned char *p)
{
    ORDBLD *b = (ORDBLD *)p;
    unsigned char *k;
    unsigned short *o;

    gm_BuildingClass(b->type);
    k = (unsigned char *)ai_BTypes + b->type * 0x24d;
    if (gm_IsHomeType((unsigned char *)b) != 0)
        return gm_Query((void *)b->objList, 1, 0, 0xfd);
    if (*k == 4)
        return gm_Query((void *)b->objList, 1, 0, 0x54);
    o = (unsigned short *)gm_Query((void *)b->objList, 1, 4, 2);
    while (o != 0 && *o == 0xfd)
        o = gm_QueryNext();
    return o;
}
/* 0x501d80, the class of a building kind (the type byte of the kind record): 3 for 1 / 3 / 6 / 0xf, 6 for 2 / 0x1e, 8 for 4 / 5 / 9, 4 for 7, 1 for 8 / 0xe / 0x12 / 0x14 .. 0x16 / 0x1d / 0x1f / 0x20, 2 for 0xb .. 0xd, 7 for 0x13, 5 for 0x17 .. 0x1a, 9 for 0x21, else 0 */

int gm_BuildingClass(int kind)
{
    switch ((unsigned char)ai_BTypes[kind * 0x24d]) {
    case 0xb:
    case 0xc:
    case 0xd:
        return 2;
    case 0x13:
        return 7;
    case 4:
    case 5:
    case 9:
        return 8;
    case 8:
    case 0xe:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1d:
    case 0x1f:
    case 0x20:
        return 1;
    case 1:
    case 3:
    case 6:
    case 0xf:
        return 3;
    case 7:
        return 4;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        return 5;
    case 2:
    case 0x1e:
        return 6;
    case 0x21:
        return 9;
    }
    return 0;
}
/* 0x501e40, the nearest other building of the type kind to the building b (both need an object at +0x61; not a home (0x502520), not a type-10 (0x5024f0) one): by the squared-root distance of the positions 0x458a20 gives, 0 when none */

unsigned char *gm_NearestBuilding(unsigned char *b, unsigned kind)
{
    float best, d;
    unsigned char *r, *p, *k;
    int o;
    float v1[3], v2[3];

    r = 0;
    best = 1e+08f;
    if (b != 0 && *(int *)(b + 0x61) != 0) {
        p = (unsigned char *)gm_BuildQuery(1, 6);
        while (p != 0) {
            k = (unsigned char *)ai_BTypes + *p * 0x24d;
            if (gm_IsHomeType(p) == 0 && gm_IsType10(p) == 0 && (o = *(int *)(p + 0x61)) != 0 && p != b && *k == kind) {
                d3_PointToWorld(o, o + 0x4c, v1);
                d3_PointToWorld(*(int *)(b + 0x61), *(int *)(b + 0x61) + 0x4c, v2);
                d = (float)sqrt((v1[0] - v2[0]) * (v1[0] - v2[0]) + (v1[1] - v2[1]) * (v1[1] - v2[1]) + (v1[2] - v2[2]) * (v1[2] - v2[2]));
                if (d < best) {
                    r = p;
                    best = d;
                }
            }
            p = gm_BuildNext();
        }
        return r;
    }
    return 0;
}
/* 0x501f60, the building of the kind kind nearest to the building b (by the scene positions of their objects) among those without an owner, as gm_NearestBuilding (0x501e40) but owner 0xffff only: of the query (1, 6), not a home (gm_IsHomeType), not of type 10 (gm_IsType10), with a scene object, not b itself. Returns it, 0 when there is none */

unsigned char *gm_NearestFreeBuilding(unsigned char *p, unsigned kind)
{
    ORDBLD *b = (ORDBLD *)p;
    float best, d, dx, dy, dz;
    ORDBLD *r, *q;
    struct D3OBJECT *o;

    best = 1e+08f;
    r = 0;
    q = (ORDBLD *)gm_BuildQuery(1, 6);
    while (q != 0) {
        if (gm_IsHomeType((unsigned char *)q) == 0 && gm_IsType10((unsigned char *)q) == 0 && (o = q->object) != 0
            && q != b && q->type == kind && q->owner == 0xffff) {
            dx = o->pos.x - b->object->pos.x;
            dy = o->pos.y - b->object->pos.y;
            dz = o->pos.z - b->object->pos.z;
            d = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (d < best) {
                r = q;
                best = d;
            }
        }
        q = (ORDBLD *)gm_BuildNext();
    }
    return (unsigned char *)r;
}
/* 0x502020, the building with the id (+1) in the building table, 0 when none */

char *gm_FindBuildingById(int id)
{
    int i;
    char *p;

    i = 0;
    p = ai_Blds;
    while (*p == 0 || *(int *)(p + 1) != id) {
        i++;
        p += 0xa9;
        if (i >= 0x100)
            return 0;
    }
    return ai_Blds + i * 0xa9;
}
/* 0x502060, the number of the building kind with the name (case-insensitive; the name is at +1 of each 0x24d-byte record), 0 when none */

int gm_FindBuildingKind(char *name)
{
    int n, i;

    n = 0;
    i = 0;
    do {
        if (_stricmp(name, (char *)(i + 1 + (int)ai_BTypes)) == 0)
            return n;
        i += 0x24d;
        n++;
    } while (i < 0xca78);
    return 0;
}
/* 0x5020a0, writes the numbers of all building kinds with the type byte t into out and returns their count */

int gm_ListBuildingsOfType(unsigned t, int *out)
{
    int i, k, n;

    n = 0;
    k = 0;
    i = 0;
    do {
        if ((unsigned char)ai_BTypes[i] == t) {
            out[n] = k;
            n++;
        }
        i += 0x24d;
        k++;
    } while (i < 0xca78);
    return n;
}

int gm_BuildingJob(int t);                   /* 0x5048e0 */
int gm_ListBuildingsOfType();                     /* 0x5020a0 */
extern int gm_AllowAll;

/* the job list of a building kind record (the six job numbers at +0x223, 0-terminated): returns code from the
   caller when the job number v is in it */
#define JOB_FIND(rec, v, code)                                                       \
    for (j = 0; j < 6; j++) {                                                        \
        if ((rec)[0x223 + j] == 0)                                                   \
            break;                                                                   \
        if ((v) == (rec)[0x223 + j])                                                 \
            return code;                                                             \
    }

/* 0x5020e0, whether the character c may take a job in the building kind k: 0 no, 1 yes, 2 yes in another building of the same type (gm_ListBuildingsOfType); by the kind's class (gm_BuildingClass): 1 / 7 (type 0x13 only) / 8 the job number of c for the kind (gm_TitleJob) is in the kind's job list, 2 never, 3 by the character's profession (0xf: types 1 / 0xf / 6, 0xa: type 3), 4 the kind's own list only, 6 by the character's level against a limit per kind (1 / 3 / 4 / 5), giving 1 or 2; with gm_AllowAll every person (kind 6 / 7) may */
int gm_MayTakeJob(int k, int c)
{
    int cls, i, j, n;
    unsigned char *p, *q;
    ORDCHAR *ch;
    int v;
    int list[8];
    int lim;

    cls = gm_BuildingClass(k);
    p = (unsigned char *)ai_BTypes + k * 0x24d;
    v = gm_TitleJob(c, gm_BuildingJob(*p));
    if (c == 0xffff)
        return 0;
    ch = &ORD_CHARS[c];
    if (gm_AllowAll != 0 && (ch->kind == 6 || ch->kind == 7))
        return 1;
    if (cls == 1) {
        JOB_FIND(p, v, 1);
        n = gm_ListBuildingsOfType(*p, list);
        for (i = 0; i < n; i++) {
            q = (unsigned char *)ai_BTypes + list[i] * 0x24d;
            JOB_FIND(q, v, 2);
        }
        return 0;
    }
    if (cls == 2)
        return 0;
    if (cls == 3) {
        if (ch->job == 0xf && (*p == 1 || *p == 0xf || *p == 6))
            return 1;
        if (ch->job == 10 && *p == 3)
            return 1;
        return 0;
    }
    if (cls == 7) {
        if (*p != 0x13)
            return 0;
        JOB_FIND(p, v, 1);
        n = gm_ListBuildingsOfType(0x13, list);
        for (i = 0; i < n; i++) {
            q = (unsigned char *)ai_BTypes + list[i] * 0x24d;
            JOB_FIND(q, v, 2);
        }
        return 0;
    }
    if (cls == 4) {
        JOB_FIND(p, v, 1);
        return 0;
    }
    if (cls == 6) {
        lim = 0;
        switch (k) {
        case 4:
            lim = 1;
            break;
        case 5:
            lim = 3;
            break;
        case 6:
        case 0x4b:
        case 0x4c:
        case 0x4d:
            lim = 4;
            break;
        case 7:
            lim = 5;
            break;
        }
        return (ch->level < lim) + 1;
    }
    if (cls == 8) {
        JOB_FIND(p, v, 1);
        n = gm_ListBuildingsOfType(*p, list);
        for (i = 0; i < n; i++) {
            q = (unsigned char *)ai_BTypes + list[i] * 0x24d;
            JOB_FIND(q, v, 2);
        }
        return 0;
    }
    return 1;
}
/* 0x5024f0, 1 when the building b is of the type 10 (the town market) */

int gm_IsType10(unsigned char *b)
{
    return ai_BTypes[*b * 0x24d] == 10;
}
/* 0x502520, 1 when the building b is a home (the types 0xb, 0xc, 0xd, 0x10, 0x1c, 0x1d, 0x1f) */

int gm_IsHomeType(unsigned char *b)
{
    char *k;

    if (b == 0)
        return 0;
    k = ai_BTypes + *b * 0x24d;
    if (*k == 0x0b || *k == 0x0d || *k == 0x0c || *k == 0x10 || *k == 0x1c || *k == 0x1d || *k == 0x1f)
        return 1;
    return 0;
}

int gm_JobBuildingType(int x);
extern unsigned short * sv_PanelShown;                /* 0x6f2310 */
/* ai_Unk61a688: view of gm_QueryKind (tools/data_views.py) */
extern int ai_CharCount, ai_NextCharNumber, ai_NextPlayerNumber;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                     /* 0x447300 */
void cm_BeginGroup(const char *s);                     /* 0x4c4540 */
void cm_EndGroup(const char *s);                     /* 0x4c4580 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
void cm_SendBoss(int a, int b);                     /* 0x4c5540 */
void cm_SendExSetHE(unsigned char k, char *p);                     /* 0x4c4c60 */
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);                     /* 0x53dee0 */
int d3t_SceneIndex();                     /* 0x411be0 */
int ch_Delete();                     /* 0x4028a0 */
int gm_Unk50a0f0(int y);                     /* 0x50a0f0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);                     /* 0x524260 */
void he_EndPersonEvents(unsigned short *p);                     /* 0x5538b0 */
void he_EndKind23Events(int ev);                     /* 0x568aa0 */
void d3io_NoopCallback();                     /* 0x44ee40 */
int amt_ClearOffices();                     /* 0x4e6d80 */
void gm_KillSpieler(int c, int mode);                     /* 0x503c60 */
extern struct D3SCENE *d3_DefaultScene;  /* 0x5fb5b0 (as d3.h / anm.h) */

/* 0x502570, the string-table number (0x60e + n) of the picture of the good o (an object of the kinds 2 / 6): the pairs (prototype number, n) of a local table (0x13 -> 1, 0x54 -> 2, 0x15 -> 4 ..), searched for the object's prototype number up to the 0 entry; -1 when it is not in it or the object is of another kind */
int gm_GoodPicture(unsigned short *o)
{
    int t[55][2] = {
        { 0x13, 0x1 }, { 0x54, 0x2 }, { 0x15, 0x4 }, { 0x18, 0xd }, { 0x14, 0x9 }, { 0x16, 0x18 },
        { 0x17, 0xf }, { 0x37, 0x0 }, { 0x12, 0x19 }, { 0x9, 0x1 }, { 0xcf, 0x20 }, { 0xd6, 0x1b },
        { 0xdd, 0x1f }, { 0x120, 0xa }, { 0x12e, 0x1c }, { 0x10e, 0x1d }, { 0x110, 0x1 }, { 0x114, 0x1e },
        { 0x139, 0x1e }, { 0x138, 0x1 }, { 0x137, 0x1d }, { 0xf2, 0xe }, { 0xe6, 0x21 }, { 0xe5, 0x21 },
        { 0xf0, 0xb }, { 0xf7, 0x1c }, { 0xb7, 0x15 }, { 0xba, 0x1a }, { 0xbb, 0x5 }, { 0x4d, 0x7 },
        { 0x5c, 0x14 }, { 0x60, 0x12 }, { 0x65, 0x1a }, { 0x145, 0x2 }, { 0x146, 0x15 }, { 0x14c, 0x16 },
        { 0x74, 0x20 }, { 0x85, 0x20 }, { 0x118, 0x20 }, { 0x9b, 0x20 }, { 0xa5, 0x1c }, { 0xf3, 0x22 },
        { 0x205, 0x23 }, { 0x21b, 0x24 }, { 0x211, 0x25 }, { 0x276, 0x26 }, { 0x28a, 0x27 }, { 0x27f, 0x28 },
        { 0x24b, 0x29 }, { 0x256, 0x2a }, { 0x266, 0x9 }, { 0x2c, 0x20 }, { 0x2d, 0x20 }, { 0x33, 0x20 },
        { 0x0, -1 }
    };
    char *g;
    int i;

    g = ai_Goods + *o * 0x41;
    if (*g == 2 || *g == 6) {
        i = 0;
        do {
            if (t[i][0] == *o)
                return t[i][1] + 0x60e;
            i++;
        } while (t[i][0] != 0);
    }
    return -1;
}
/* 0x5029f0, callback of 0x438d40 for 0x502a60: collects the objects whose prototype (low 24 bits of +0x48) is of the kind 0x21 and has
   its top byte 1 into list (0x40 objects, then their keys, then the count); true while there is room */

char gm_CollectSlotObject(char *o, int *list)
{
    int key;

    key = *(int *)(o + 0x48) & ((unsigned)&txt_Keys + 0x181bf);
    if (o[0x4b] == 1 && ai_Goods[key * 0x40 + key] == 0x21) {
        list[list[0x80] + 0x40] = key;
        list[list[0x80]] = (int)o;
        list[0x80]++;
    }
    return list[0x80] < 0x40;
}
/* 0x502a60, when the panel shown (sv_PanelShown) is the scene: the five slot bytes of the character c (+0x80 of its record) give the wanted keys 0x17f + 6 k + (int)(slot / 42 - 0.5), 0 for an empty slot; the default scene's objects of goods kind 0x21 (gm_CollectSlotObject) are switched off (d3if_ShowTree (obj, 0)) and those whose key is wanted on again */
void gm_ShowSlotObjects(int unused, int scene, int c)
{
    int want[5];
    int list[0x40 * 2 + 1];
    int i, k, v;
    unsigned char *s;

    if (sv_PanelShown == scene) {
        s = (unsigned char *)ai_Chars[c] + 0x80;
        for (k = 0, i = 0; k < 0x1e; k += 6, s++, i++) {
            v = (int)((float)(*s / 42.0f) - 0.5f) + 0x17f + k;
            if (*s == 0)
                v = 0;
            want[i] = v;
        }
        memset(list, 0, sizeof(list));
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, gm_CollectSlotObject, list);
        for (i = 0; i < list[0x40 * 2]; i++) {
            d3if_ShowTree(list[i], 0);
            for (k = 0; k < 5; k++)
                if (list[i + 0x40] == want[k])
                    d3if_ShowTree(list[i], 1);
        }
    }
}

/* ---- the character and building life cycle 0x502b60 .. 0x504510 (ROUGH drafts, written out of address order at the end of the file) ---- */
#define GM_CRIMES ((CRIME *)ai_Crimes)  /* 0x91f700, ai_Crimes (cm.c, char[]) as its 0x200 CRIME records */
extern int ai_Evidence[0x800][2];              /* 0x91b6e0 .. 0x91f6e0: pairs of ints */
extern int ai_Unk770ae0_day;            /* the clock's day (0x770ae0) */
#ifndef TIME_VIEW_H
extern unsigned short ai_TimeHour;     /* 0x770ae4, the clock's hour (unaligned view) */
#endif
extern char ai_Unk770ce4[];             /* 0x770ce4: the field +0x164 of the character records (stride 0x21c) */
void sv_RemoveBanners(char *b);                     /* 0x5286a0 */
void sv_CreateBanners(char *b);                     /* 0x5282f0 */
void ch_InflateModel();                     /* 0x402540 */
int ch_Delete();                     /* 0x4028a0 */
void ai_ShowEvidenceVoidNews(int param_1, unsigned short *param_2);                     /* 0x5a9430 */
int gm_FreeList();                     /* 0x500140 */
int gm_TitlesLeft(int n);                     /* 0x504ce0 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int gm_CompareTime();                     /* 0x4fe210 */
int gm_MinutesBetween();                     /* 0x4fe280 */
int d3t_SceneIndex();                     /* 0x411be0 */
double _CIfmod();                     /* 0x5de73a: a CRT call */
extern int ai_CharCount;  /* 0x6f1bc0 */
extern int f3gm_ObjectCount;  /* 0x6f1bdc */

/* 0x502b60, hands the building b to the owner own (a character slot, 0xffff: only the master is set) with the master character master: a player owner (kind 6) loses the building's production setting (ai_FreeProdSetting), a player new owner gets one (ai_ProdSetting); the old owner's home (bld2) link is cleared; a home (type 2) shows its objects (gm_ShowBuildingObjects) and becomes the new owner's home unless it has one of a higher type; for a person owner (kind 6 / 7) the character working there under the master (gm_FindCharacterByMaster) loses title and flag, and its building for the types 5 / 9; every cart (goods kind 0x1d) of the building gets the owner's netId and its model the owner's texture set (v054 - 0x599 for the kinds 5 / 6 / 7); last the banners are removed in scene 0 (sv_RemoveBanners) and, for another character of kind 5 / 6 / 7 than the player's with a model, made again (sv_CreateBanners) */
void gm_AssignBuilding(unsigned char *b, unsigned short master, unsigned own)
{
    ORDBLD *bd;
    unsigned char *p;
    unsigned short o;
    ORDBLD *home;
    ORDCHAR *per;
    ORDCHAR *ch;
    ORDOBJ *w;
    CHARACTER *ob;
    int cnt, scn, owid;
    char t;

    bd = (ORDBLD *)b;
    p = (unsigned char *)ai_BTypes + bd->type * 0x24d;
    bd->masterId = master;
    if (own != 0xffff) {
        o = bd->owner;
        if (o != own) {
            if (o != 0xffff && ORD_CHARS[o].kind == 6)
                ai_FreeProdSetting(bd->id);
            if (ORD_CHARS[own].kind == 6)
                ai_ProdSettingK(b);
        }
        o = bd->owner;
        if (o != 0xffff && ORD_CHARS[o].bld2 == bd)
            ORD_CHARS[o].bld2 = 0;
        bd->owner = own;
        if (*p == 2) {
            gm_ShowBuildingObjects(b);
            if (*p == 2) {
                home = ORD_CHARS[bd->owner].bld2;
                if (home == 0 || home->type <= bd->type)
                    ORD_CHARS[bd->owner].bld2 = bd;
            }
        }
        t = ORD_CHARS[own].kind;
        if (t == 6 || t == 7) {
            per = (ORDCHAR *)gm_FindCharacterByMaster((int)b);
            if (per != 0) {
                per->title = 0;
                per->flags1b4 &= ~1u;
                if (*p == 5 || *p == 9)
                    per->bld = 0;
            }
        }
        cnt = 0x5008;
        w = (ORDOBJ *)ai_Objects;
        do {
            if (w->type != 0 && ai_Goods[w->type * 0x41] == 0x1d && w->cartBld == bd->id) {
                w->holderId = ORD_CHARS[own].netId;
                if (w->list != 0)
                    ((ORDOBJ *)w->list)->holderId = ORD_CHARS[own].netId;
                scn = d3_CurrentScene;
                ob = w->model;
                if (ORD_CHARS[own].v054 == 0 || ((t = ORD_CHARS[own].kind) != 5 && t != 6 && t != 7))
                    owid = 0;
                else
                    owid = ORD_CHARS[own].v054 - 0x599;
                d3if_SelectScene(d3t_SceneIndex(ob->scene), 1);
                if (ob->obj->info == 0)
                    ch_InflateModel(ob);
                d3if_SelectTextureSet(*ob->transport, (struct D3SURF *)(*ob->transport)->info, owid, 1);
                d3if_SelectScene(scn, 1);
            }
            w = (ORDOBJ *)((char *)w + 0x43);
            cnt--;
        } while (cnt != 0);
        ch = &ORD_CHARS[own];
        if ((char *)ch != ai_PlayerCharC && bd->object != 0 && ((t = ch->kind) == 6 || t == 7 || t == 5)) {
            scn = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            sv_RemoveBanners(b);
            sv_CreateBanners(b);
            d3if_SelectScene(scn, 1);
            return;
        }
        scn = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        sv_RemoveBanners(b);
        d3if_SelectScene(scn, 1);
    }
}
/* 0x502e30, for a building of the type 2: runs 0x502a60 for every object of the kind 2 in its list (up to 32), with the building's owner (+0x27) */


void gm_ShowBuildingObjects(unsigned char *b)
{
    int l[32];
    int n, i, o;

    n = 0;
    if (ai_BTypes[*b * 0x24d] == 2) {
        o = (int)gm_Query(*(int *)(b + 0x5d), 1, 4, 2);
        while (o != 0) {
            l[n] = o;
            n++;
            o = (int)gm_QueryNext();
        }
        for (i = 0; i < n; i++)
            gm_ShowSlotObjects((int)b, l[i], *(unsigned short *)(b + 0x27));
    }
}
/* 0x502eb0, lists the characters working in the building b (their building, bld, is b) into out and returns their number (also out[0]): out[1 ..] (0xc) those with the title of the kind's first job rank (+0x22f), out[0xd ..] (0xc) of the second (+0x230), out[0x19 ..] (0xc) the others, out[0x25] the one whose job is one of the kind's two jobs (+0x223 / +0x224, both set); the lists are filled with 0xffff
   @deep Read whole against the original and rewritten (ORDCHAR walk, c->bld at +0x16c - the draft compared
   bld2 +0x170, a meaning fix; the three 0xffff-filled lists, the special job test): the statement
   structure and every operand match, block order equal (bld test, rank 1 -> p1, rank 2 -> p2, the
   0x223/0x224 job test -> out[0x25], else p3, n++). What is left is the allocation of the six live values:
   the original keeps p3 in ebp, out in edx and n in out's dead argument slot ([esp+0x24], inc dword ptr),
   k / p1 / p2 in slots ([esp+0x18] / [esp+0x10] / [esp+0x14]); ours keeps k in ebp, out in esi, p1 in ebx
   with a slot copy (mov [esp+0x18], ebx / mov ebx, [esp+0x18]: the 3 extra instructions), n / p3 in slots
   and p2 in the argument slot. The c->bld compare (cmp [ecx+7], edx vs mov eax, [esi+7]; cmp eax, ecx) and
   the rank loads (mov al, [ebp+0x22f]; cmp al, dl vs cmp [ecx+0x22f], al) follow from that choice. Tried:
   pointer loop (worse, jb), index loop (kept), declaration orders, unsigned list pointers, n kept in
   out[0], the p3 branch written twice (worse) */
int gm_ListWorkers(unsigned char *b, int *out)
{
    int i, n, j;
    unsigned char *k;
    ORDCHAR *c;
    int *p1, *p2, *p3;

    k = (unsigned char *)ai_BTypes + *b * 0x24d;
    p1 = out + 1;
    out[0] = 0;
    for (j = 0; j < 0x18; j++)
        p1[j] = 0xffff;
    p3 = out + 0x19;
    for (j = 0; j < 0xc; j++)
        p3[j] = 0xffff;
    out[0x25] = 0xffff;
    p2 = out + 0xd;
    n = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->bld == (ORDBLD *)b) {
            if (k[0x22f] == (unsigned char)c->title)
                *p1++ = c->id;
            else if (k[0x230] == (unsigned char)c->title)
                *p2++ = c->id;
            else if (k[0x223] != 0 && k[0x224] != 0 && (k[0x223] == c->jobId || k[0x224] == c->jobId))
                out[0x25] = i;
            else
                *p3++ = c->id;
            n++;
        }
    }
    out[0] = n;
    return n;
}
/* 0x502fd0, the sale of the building with the id id to the character with the netId buyer for price (the job record job, the building offer, may be 0; boss: also make the buyer the boss of a home): in the message group "buy_building" the buyer pays the master of the building (a message 0x17e6 "_NACHRICHTEN_HS_30" to the master, cm_SendMoveGoodsUnchecked) or, without a master, the bank (cm_SendMoveGoods), the ownership goes to the buyer (cm_SendOwnership) and for a home (type 2) with boss set the boss too (cm_SendBoss); the job's end is stamped now + 1 second and its HE message sent (cm_SendExSetHE, the slot kept in the job), and the building's field 0x5a set (cm_SendSetField (id, 0x5a, 2, 0, 2)) */
void gm_SellBuilding(int id, int buyer, int price, int job, int boss)
{
    ORDBLD *bl;
    ORDCHAR *ch;
    unsigned char *k;
    GMJOB *j;
    char buf[256];

    bl = (ORDBLD *)gm_BuildQuery(1, 1, id);
    ch = (ORDCHAR *)gm_FindChar(buyer);
    k = (unsigned char *)ai_BTypes + bl->type * 0x24d;
    cm_BeginGroup("buy_building");
    if (bl->masterId != 0xffff) {
        txt_Format(buf, 0x17e6, bl, ch->id, price);
        he_MessageBoxQuickjump(ORD_CHARS[bl->masterId].netId, -1, buf, 0, 0x5ff, bl->id, -1, 0, "_NACHRICHTEN_HS_30");
        cm_SendMoveGoodsUnchecked(ORD_CHARS[bl->masterId].netId, buyer, ai_OwnPlayer, price);
        cm_SendOwnership(bl->id, ch->netId, ch->netId);
        if (boss != 0 && *k == 2)
            cm_SendBoss(ch->netId, bl->id);
    } else {
        cm_SendMoveGoods(-1, buyer, ai_OwnPlayer, price);
        cm_SendOwnership(bl->id, ch->netId, ch->netId);
        if (boss != 0 && *k == 2)
            cm_SendBoss(ch->netId, bl->id);
    }
    j = (GMJOB *)job;
    if (j != 0) {
        j->end = GM_NOW;
        gm_AddTime(&j->end, 0, 0, 1);
        j->msgSlot = ((int (*)(int, int))cm_SendExSetHE)(-1, job);
    }
    cm_SendSetField(bl->id, 0x5a, 2, 0, 2);
    cm_EndGroup("buy_building");
}
/* 0x5031b0, "gm_AllocRaum": creates the room object n of the building b with its sub-objects: the object list of the building kind (the words at +0x23, 0x40 of them) after n, up to the next room (goods kind 2 / 6) or the 0 end, the words with the top bit set; the room's first object 0x2a gets the count 2, the building as owner and the capacities +0x23d / +0x23e of the kind (at least 2; the kind's +0x23f alone when only it is set), its object 0x116 (kind 6) the same with the room as owner and at least 6; a market stall (0x92 .. 0x97, 0x2db) also makes its store room ob_LAGERFLAECHE_ALLGEMEIN (capacity 0xc, count 3) and an object 0xff holding up to 0x10 goods of the template gm_DefaultStock that the own player's stock records give to this stall kind. Returns the room, 0 without tables or room for 0x40 more objects */
unsigned short *gm_AllocRaum(unsigned char *b, unsigned n)
{
    ORDOBJ *o, *g;
    unsigned short *bi;
    unsigned char *bp;
    unsigned short *q;
    char *k;
    unsigned short w;
    GMSTOCK *s, *t;
    int i, j, cnt;

    if (ai_Goods == 0 || ai_BTypes == 0 || f3gm_ObjectCount + 0x40 >= 0x5008)
        return 0;
    o = (ORDOBJ *)gm_CreateObject(((ORDBLD *)b)->id, n, 1);
    bp = (unsigned char *)ai_BTypes + *b * 0x24d;
    i = 0;
    for (q = (unsigned short *)(bp + 0x23); (*q & 0x7fff) != n; q++)
        i++;
    for (i++; i < 0x40; i++) {
        w = ((unsigned short *)(bp + 0x23))[i];
        if (w == 0)
            break;
        if (w != 0xffff) {
            k = ai_Goods + (w & 0x7fff) * 0x41;
            if (*k == 2 || *k == 6)
                i = 0x40;
            else if (w & 0x8000)
                gm_CreateObject(o->objId, w & 0x7fff, 1);
        }
    }
    g = (ORDOBJ *)gm_Query((void *)o->list, 1, 0, 0x2a);
    if (g != 0) {
        g->ownerId = ((ORDBLD *)b)->id;
        g->count = 2;
        if (bp[0x23d] == 0 && bp[0x23e] == 0 && bp[0x23f] != 0)
            g->cap[0] = bp[0x23f];
        else {
            g->cap[0] = bp[0x23d] < 2 ? 2 : bp[0x23d];
            g->cap[1] = bp[0x23e] < 2 ? 2 : bp[0x23e];
        }
    }
    g = (ORDOBJ *)gm_Query((void *)o->list, 2, 0, 0x116, 6);
    if (g != 0) {
        g->ownerId = o->objId;
        g->count = 2;
        if (bp[0x23d] == 0 && bp[0x23e] == 0 && bp[0x23f] != 0)
            g->cap[0] = bp[0x23f] < 6 ? 6 : bp[0x23f];
        else {
            g->cap[0] = bp[0x23d] < 6 ? 6 : bp[0x23d];
            g->cap[1] = bp[0x23e] < 6 ? 6 : bp[0x23e];
        }
    }
    if ((o->type >= 0x92 && o->type <= 0x97) || o->type == 0x2db) {
        s = (GMSTOCK *)(init_StockBlocks + ai_OwnPlayer * 0x2a90);
        g = (ORDOBJ *)gm_Query((void *)o->list, 1, 0, 0x116);
        if (g != 0) {
            g->cap[0] = 0xc;
            g->cap[1] = 0;
            g->count = 3;
        } else
            err_Message("gm_AllocRaum(): ob_LAGERFLAECHE_ALLGEMEIN is missing in ob_MARKTSTAND!");
        bi = gm_CreateObject(o->objId, 0xff, 1);
        cnt = 0;
        t = (GMSTOCK *)gm_DefaultStock;
        for (j = 0; j < 0x55; j++) {
            if (s[j].marketKind == o->type && s[j].amount != 0 && cnt < 0x10) {
                gm_CreateObject(((ORDOBJ *)bi)->objId, t[j].item, 1);
                cnt++;
            }
        }
    }
    return (unsigned short *)o;
}
/* 0x503480, removes the object instance with the id from the list of the building b (0x4ffff0): 0 when it was there, -3 otherwise */


int gm_RemoveObjectFromBuilding(int b, int id)
{
    int r;

    r = gm_RemoveObject((int *)(b + 0x5d), id);
    return (r != -2) ? 0 : -3;
}
/* 0x5034b0, tears down the extra parts of the object o: its record at +0x24 (flag 2 at +0x78: 0x4c4c60 message), its model at +0x3b (0x4028a0 in the scene 0x411be0 gives), and unlinks it from the owner's list (0x50a0f0, 0x5000c0 by id). ROUGH */

void gm_Unk5034b0(int o)
{
    int saved, r, m;

    saved = d3_CurrentScene;
    r = *(int *)(o + 0x24);
    if (r != 0 && (*(unsigned char *)(r + 0x78) & 2) != 0)
        *(int *)(r + 0x84) = ((int (*)(int, int))cm_SendExSetHE)(-1, r);
    m = *(int *)(o + 0x3b);
    if (m != 0) {
        d3if_SelectScene(d3t_SceneIndex(*(int *)(m + 0x88)), 1);
        ch_Delete(m);
        d3if_SelectScene(saved, 1);
    }
    r = gm_Unk50a0f0(o);
    if (r != 0)
        gm_RemoveOwned((int *)(r + 0x5d), *(int *)(o + 2));
}

int ai_PermissionBits1d();                     /* 0x587b70 */
extern int gm_AllowAll;                /* 0x6f2448 */
int gm_BuildingJob();                     /* 0x5048e0 */
int gm_TitleGroup(int n);                /* 0x504ad0 */

/* 0x503530, the permission bits of the character p in the building b (or, without a building, for the object o or the character c): what he may do there, by the class byte of the building's type record (a case per class: the master (+0x25) and the owner (+0x27) compared with p, the six job bytes at +0x223 of the type record searched for the job of p in the type (gm_TitleJob); class 0x1d is ai_PermissionBits1d). Bits seen: 0x1 enter, 0x800 led by p (c's building is owned by p), 0x4000 / 0x5000 / 0x2200 granted per object and job; the exact meaning of each bit is open. With gm_AllowAll everything is allowed (-1, -2 for the classes 3 / 0x21); a building without owner (0xffff) allows nothing
   @deep Read whole against the original 0x503530..0x503bbf and rewritten (ORDBLD / ORDOBJ / ORDCHAR typed;
   switch on *btype in the original's case order 0x21, 2/0x1e, 9, 4, 5.., 0x17.., 0xb.., 0x10, 1/3/f, 0x13,
   6, 0x1c->10, 0x1b/0x1d, 7; the job scans as index loops btype[0x223 + i] (the original's lea
   ecx,[edx+esi] test is VC6's replacement of i); job2 (+0x169) for the 0x17 .. 0x1a classes; the char
   pointer ch for 1/3/f, 6, 0x1c). Tables equal, ours=491=original's, every instruction matched: the 2
   remaining hunks are two blocks placed elsewhere, nothing else. (1) the join block 'mov ebx,[esp+0x1c]' +
   the object test (0x5039ee..0x503a2f, 17 ins) is the allocator's reload of b (ebx is the switch index):
   VC6 places it after the 0x17 case (ours +0x289) where the original has it after case 10 (+0x4be). (2)
   the cross-jumped tail 'or ebp,0x2200; jmp' shared by the cases 9/4/0x13/7: the original keeps the copy
   of 0x13 (+0x427), ours that of case 7 (its last user). Tried: case 7 as a goto block after the join (38 ins,
   the same two placements move elsewhere), the 0x17 test as an else-if chain (60), cls as a char local
   (45). */
unsigned gm_PermissionBits(unsigned p, unsigned char *b, unsigned short *o, int c)
{
    ORDBLD *bld = (ORDBLD *)b;
    ORDOBJ *obj = (ORDOBJ *)o;
    unsigned char *btype;
    int i;
    unsigned v;
    unsigned short ot;
    int job;
    unsigned char prof;
    ORDCHAR *ch;

    v = 0;
    btype = 0;
    if (bld != 0) {
        btype = (unsigned char *)ai_BTypes + bld->type * 0x24d;
        job = gm_TitleJob(p, gm_BuildingJob(*btype));
        if (*btype == 0x1d)
            return ai_PermissionBits1d(ai_Chars[p], (char *)bld, (char *)obj);
        switch (*btype) {
        case 0x21:
            if (bld->masterId == p)
                v = 0x3c;
            if (bld->owner == p)
                v |= 0x620;
            break;
        case 2:
        case 0x1e:
            if (bld->masterId == p)
                v = 0x3c;
            if (bld->owner == p)
                v |= 0x5661;
            break;
        case 9:
            v = 1;
            if (bld->masterId == p)
                v = 0x403d;
            if (bld->owner == p) {
                v |= 0x54e1;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v |= 0x2200;
                        break;
                    }
                    i++;
                } while (i < 6);
            }
            break;
        case 4:
            if (bld->masterId == p)
                v = 0x50bd;
            if (bld->owner == p) {
                v |= 0x44e1;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v |= 0x2200;
                        break;
                    }
                    i++;
                } while (i < 6);
            }
            break;
        case 5:
        case 8:
        case 0xe:
        case 0x12:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x20:
            v = 0x81;
            if (bld->masterId == p)
                v = 0x10bd;
            if (bld->owner == p) {
                v |= 0x460;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v |= 0x2200;
                        break;
                    }
                    i++;
                } while (i < 6);
                v |= 0x4000;
            }
            break;
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1a:
            v = 0x5081;
            if ((*btype == 0x17 && ORD_CHARS[p].job2 == 0x1e) || (*btype == 0x18 && ORD_CHARS[p].job2 == 0x1f) ||
                (*btype == 0x19 && ORD_CHARS[p].job2 == 0x20) || (*btype == 0x1a && ORD_CHARS[p].job2 == 0x21))
                v = 0x52f1;
            break;
        case 0xb:
        case 0xc:
        case 0xd:
        case 0x11:
        case 0x1f:
            if (obj == 0 || bld->owner == p)
                v = 0x81;
            if (bld->masterId == p)
                v |= 0x403c;
            if (bld->owner == p)
                v |= 0x5670;
            if (obj == 0)
                goto fin;
            if (obj->type == 0xfd)
                v |= 0x4001;
            break;
        case 0x10:
            if (bld->owner == p) {
                v = 0x54e1;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v = 0x76e1;
                        break;
                    }
                    i++;
                } while (i < 6);
            }
            if (bld->masterId == p)
                v |= 0x3c;
            break;
        case 1:
        case 3:
        case 0xf:
            ch = &ORD_CHARS[p];
            if (*btype != 3)
                v = 0x5081;
            prof = ch->job;
            if (prof == 0xf || prof == 0xe || prof == 0xd)
                v |= 0x502;
            if (prof == 0xf || prof == 0xe || prof == 0xd)
                v |= 0x200;
            if (*btype == 3) {
                if (prof == 10)
                    v |= 0x30;
            } else if (prof == 0xf)
                v |= 0x30;
            if (obj == 0)
                goto fin;
            if (obj->type == 0x114 && prof == 0)
                v &= 0xffffbffe;
            break;
        case 0x13:
            if (bld->masterId == p)
                v = 0x50bd;
            if (bld->owner == p) {
                v |= 0x44e1;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v |= 0x2200;
                        break;
                    }
                    i++;
                } while (i < 6);
            }
            break;
        case 6:
            ch = &ORD_CHARS[p];
            v = 0x5001;
            if (bld->masterId == p)
                v = 0x503d;
            if (bld->owner == p)
                v |= 0x5660;
            if (ch->job == 0xf)
                v |= 0x30;
            break;
        case 0x1c:
            ch = &ORD_CHARS[p];
            if (gm_TitleGroup(ch->jobId) != 8 && gm_BuildQuery(2, 5, 9, 4, ch->id) == 0)
                break;
        case 10:
            v = 1;
            break;
        case 0x1b:
        case 0x1d:
            break;
        case 7:
            v = 1;
            if (obj != 0 && (obj->type == 0xe6 || obj->type == 0xe5))
                v = 0x1001;
            if (bld->masterId == p)
                v |= 0x103c;
            if (bld->owner == p) {
                v |= 0x4460;
                i = 0;
                do {
                    if (btype[0x223 + i] == 0)
                        break;
                    if (job == btype[0x223 + i]) {
                        v |= 0x2200;
                        break;
                    }
                    i++;
                } while (i < 6);
            } else {
                if (obj == 0)
                    goto fin;
                if (obj->type != 0xf3)
                    v |= 0x5000;
            }
            break;
        }
    } else if (c != 0) {
        if (((ORDCHAR *)c)->bld != 0 && ((ORDCHAR *)c)->bld->owner == p)
            v = 0x800;
    } else {
        if (obj == 0)
            goto fin;
        ot = obj->type;
        if (ai_Goods[ot * 0x41] == 6) {
            if (ot >= 0x91 && ot <= 0x97)
                v = 1;
            else
                v = 0xffffffff;
            v |= 0x4000;
        } else if (ot >= 0x91 && ot <= 0x97)
            v = 1;
        else
            v = ot == 0xfd ? 0x4001 : 0xffffffff;
    }
    if (obj != 0) {
        ot = obj->type;
        if (ot == 0x27f || ot == 0x21b || ot == 0x28a || ot == 0x120)
            v |= 0x5000;
        if (ot == 0x12e)
            v |= 0x4000;
    }
fin:
    if (gm_AllowAll != 0) {
        v = 0xffffffff;
        if (bld != 0 && (*btype == 3 || *btype == 0x21))
            v = 0xfffffffe;
    }
    if (bld != 0 && bld->owner == 0xffff)
        v = 0;
    return v;
}
/* 0x503c20, releases what hangs on the character c before it dies or its slot is freed (nothing for a free slot, id 0xffff): moves it out of its group (sv_MoveToGroup), ends the events it takes part in (he_EndPersonEvents, he_EndKind23Events), the empty callback 0x44ee40 and gives up its offices (amt_ClearOffices) */

void gm_ReleaseCharacter(ORDCHAR *c)
{
    if (c != 0 && c->id != 0xffff) {
        sv_MoveToGroup(0, 0, c->id, 0);
        he_EndPersonEvents(&c->id);
        he_EndKind23Events((int)c);
        d3io_NoopCallback(c);
        amt_ClearOffices(c);
    }
}
/* 0x503c60, "gm_KillSpieler": kills the character c. Unless it is dead already (kind 0xf): its health is cleared,
   the day stamped at +0x28, what hangs on it released (gm_ReleaseCharacter), its kind set to 0xf, and every crime
   record it is the culprit of is freed together with the evidence pairs of that crime (when the player's character
   held some, ai_ShowEvidenceVoidNews tells him). Then always: its inventory is freed, its model deleted, a home of
   the class 1 loses an inhabitant, the two building links are cleared. mode 0 keeps the record ("holding data");
   otherwise the record is removed for good: the count of the living (ai_CharCount) drops, its netId is cleared in
   every relative list, the record zeroed and freed (id 0xffff, netId -1, no relatives); mode 1 logs "permanently"
   @deep Read whole against the original 0x503c60..0x503e7b and rewritten (ORDCHAR / CRIME typed, the crime
   and relative scans as index loops: the original's signed jl ends; mode != 0 first, the 'holding data'
   log as the else). Structure equal, ours=165 original=166; the remaining hunks are allocation: (1) the
   original spills ebp (c * 0x21c) to [esp+0x18] (frame 0x20c vs ours 0x208: the one extra instruction, mov
   [esp+0x18],ebp, and its reload mov ebp,[esp+0x18] in the evidence loop) because it uses ebp for
   peer->netId (mov ebp,[edi+4]; cmp ebp,[eax]) where ours takes ebx; (2) the original keeps ch->netId in
   ebx across the crime loop (mov ebx,[esi+4]; cmp [ecx],ebx) where ours reloads it per record (mov
   eax,[esi+4]; mov edx,[ecx]; cmp edx,eax); (3) cmp ebx,edi (edi = 0 cached) vs test ebx,ebx for mode; (4)
   mov word [esi],0xffff scheduled after the second lea/mov ecx,8. Tried: ch->netId in a local before the
   loop (VC6 propagates it back, 15 ins), netId (the stack local) as the compare (19). */
void gm_KillSpieler(int c, int mode)
{
    char buf[0x200];
    ORDCHAR *ch;
    ORDCHAR *peer;
    int netId;
    int crime;
    int cnt;
    int i;
    int j;

    ch = &ORD_CHARS[c];
    netId = ORD_CHARS[c].netId;
    if (ch->kind != 0xf) {
        ch->health = 0;
        ch->w028 = (short)GM_NOW.day;
        gm_ReleaseCharacter(ch);
        ch->kind = 0xf;
        peer = 0;
        cnt = 0;
        for (i = 0; i < 0x300; i++) {
            if (ORD_CHARS[i].kind == 6) {
                peer = &ORD_CHARS[i];
                break;
            }
        }
        for (i = 0; i < 0x200; i++) {
            if (GM_CRIMES[i].culprit == ch->netId) {
                crime = GM_CRIMES[i].id;
                for (j = 0; j < 0x800; j++) {
                    if (ai_Evidence[j][1] == crime) {
                        if (peer != 0 && peer->netId == ai_Evidence[j][0])
                            cnt++;
                        ai_Evidence[j][1] = -1;
                        ai_Evidence[j][0] = -1;
                    }
                }
                GM_CRIMES[i].victim = -1;
                GM_CRIMES[i].culprit = -1;
                GM_CRIMES[i].id = -1;
            }
        }
        if (cnt != 0 && peer != 0 && peer != ch)
            ai_ShowEvidenceVoidNews((int)peer, (unsigned short *)ch);
    }
    gm_FreeList(&ch->inventory);
    if (ch->model != 0) {
        ch_Delete(ch->model);
        ch->model = 0;
    }
    if (ch->bld2 != 0) {
        if (ai_BTypes[ch->bld2->type * 0x24d] == 1 && ch->bld2->people > 0)
            ch->bld2->people--;
        ch->bld2 = 0;
    }
    ch->bld = 0;
    if (mode != 0) {
        if (ch->id != 0xffff)
            ai_CharCount--;
        for (j = 0; j < 0x300; j++) {
            for (i = 0; i < 8; i++) {
                if (ORD_CHARS[j].relatives[i] == ch->netId)
                    ORD_CHARS[j].relatives[i] = -1;
            }
        }
        memset(ch, 0, 0x21c);
        ch->id = 0xffff;
        ORD_CHARS[c].netId = -1;
        for (i = 0; i < 8; i++)
            ORD_CHARS[c].relatives[i] = -1;
        if (mode == 1)
            sprintf(buf, "gm_KillSpieler: Killing Player <id = %i> permanently", netId);
    } else {
        sprintf(buf, "gm_KillSpieler: Killing Player <id = %i>, holding data", netId);
    }
}
/* 0x503e80, resets the character state: kills every one of the 0x300 characters for good (gm_KillSpieler mode 2), clears the 16 player records gm_PlayerRecords (0x2b ints each, the first word -1: free) and the three counters ai_CharCount / ai_NextCharNumber / ai_NextPlayerNumber */
void gm_ResetCharacters(void)
{
    int i;
    short *rec;

    for (i = 0; i < 0x300; i++)
        gm_KillSpieler(i, 2);
    for (i = 0; i < 0x10; i++) {
        rec = (short *)&gm_PlayerRecords[i * 0x2b];
        memset(rec, 0, 0x2b * sizeof(int));
        rec[0] = -1;
    }
    ai_CharCount = 0;
    ai_NextCharNumber = 0;
    ai_NextPlayerNumber = 0;
}
#define gm_JobTalents (*(JOBTALENTS (*)[0x5b])((char *)&gm_QueryKind + 0x2))  /* 0x61a688, the job records */

/* 0x503ee0, the talents record of the job, by value (the hidden result pointer; jobs 0x5f and above: the first record) */

JOBTALENTS gm_GetJobTalents(int job)
{
    if (job >= 0x5f)
        return gm_JobTalents[0];
    return gm_JobTalents[job];
}
/* 0x503f30, the sex the job x needs: 0 when its group (gm_TitleGroup) is 10 .. 14, else 2 = either (reasoned
   guess: 0 men only; gm_FindApplicant compares it with the sex byte) */

int gm_SexForJob(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k >= 10 && k <= 14)
        return 0;
    return 2;
}
/* 0x503f50, the sex the title x needs: 0 for the titles 1 .. 4, 7 .. 0xc and 0xf .. 0x18, 2 = either for the
   titles 5, 6, 0xd, 0xe, 0x19, 0x1a (named in the switch: its table runs to 0x1a) and any other (reasoned guess:
   0 men only, as gm_SexForJob) */

int gm_SexForTitle(int x)
{
    switch (x) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0xc:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
        return 0;
    case 5:
    case 6:
    case 0xd:
    case 0xe:
    case 0x19:
    case 0x1a:
        return 2;
    }
    return 2;
}
/* 0x503fa0, the building type of the job of the title x (gm_TitleGroup maps x to its job group first): the same
   table as gm_JobBuildingType, written out here (1 -> 0x12, 2 -> 0x14, 3 -> 0x15, 4 -> 0x16, 5 -> 0xe, 6 -> 8,
   7 -> 7, 8 -> 9, 9 -> 5, 10 -> 0x13, 0xb -> 4, 0xc -> 0x10, 0xd -> 0x1d; the groups 0xe .. 0x10 and none: 0) */
int gm_BuildingTypeOfJob(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k != 0) {
        switch (k) {
        case 0xb:
            return 4;
        case 9:
            return 5;
        case 7:
            return 7;
        case 6:
            return 8;
        case 8:
            return 9;
        case 5:
            return 0xe;
        case 0xc:
            return 0x10;
        case 1:
            return 0x12;
        case 10:
            return 0x13;
        case 2:
            return 0x14;
        case 3:
            return 0x15;
        case 4:
            return 0x16;
        case 0xd:
            return 0x1d;
        case 0xe:
        case 0xf:
        case 0x10:
            return 0;
        }
    }
    return 0;
}
/* 0x504050, the building type of the job group x (1 -> 0x12, 2 -> 0x14, 3 -> 0x15, 4 -> 0x16, 5 -> 0xe, 6 -> 8,
   7 -> 7, 8 -> 9, 9 -> 5, 10 -> 0x13, 0xb -> 4, 0xc -> 0x10, 0xd -> 0x1d; the groups 0xe .. 0x10 and any other: 0);
   the cases are listed by building type */
int gm_JobBuildingType(int x)
{
    switch (x) {
    case 0xb:
        return 4;
    case 9:
        return 5;
    case 7:
        return 7;
    case 6:
        return 8;
    case 8:
        return 9;
    case 5:
        return 0xe;
    case 0xc:
        return 0x10;
    case 1:
        return 0x12;
    case 10:
        return 0x13;
    case 2:
        return 0x14;
    case 3:
        return 0x15;
    case 4:
        return 0x16;
    case 0xd:
        return 0x1d;
    case 0xe:
    case 0xf:
    case 0x10:
        return 0;
    }
    return 0;
}


ORDOBJ *gm_QueryCursor = 0;  /* 0x6f1bf4, the current record */
int ai_PermsDirty = 0;  /* 0x6f2318 */
unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */
short * gm_PlayerRecord(char *c);                     /* 0x506430 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */


/* 0x504100, the two small numbers a / b of the job group job (the job tables of the characters; reasoned guess: two
   staff counts of its building, the cases are listed by the building type as in gm_JobBuildingType): 2 -> 1, 0;
   4 -> 1, 2; 5 -> 1, 0; 6 -> 0, 1; 7 -> 4, 1; 8 -> 0, 3; 9 -> 0, 4; 10 -> 3, 2; 11 -> 2, 3; 12 -> 3, 2; 13 -> 2, 0;
   1, 3, 0xe .. 0x10 and any other 1, 0. Returns 1
   @deep Read whole against the original 0x504100..0x50424c: one switch over the job groups 1 .. 0x10
   (table of 16), the case bodies in the original's order (11, 9, 7, 6, 8, 5, 12, 10, 2, 4, 13, as
   gm_JobBuildingType: by building type), the groups 1, 3, 0xe .. 0x10 as explicit cases on the default
   body (the table runs to 0x10); every body's values checked against the original's stores. What remains
   is VC6's merging of identical case blocks, decided on the final code: the original gives each case its
   own block with its own registers and scheduling (job 2: eax/ecx, stores then mov eax,1; job 5: ecx/edx,
   mov eax,1 first; job 10: ecx/edx, mov eax,1 first; job 12: eax/ecx, mov eax,1 last), so none are
   identical and none merge; ours gives job 10 the same registers as job 12 and the jobs 2 / 5 the same as
   the 1, 0 block, so they are cross-jumped (the table entries +0x4, +0x10, +0x24 point to the shared copy;
   ours=71 vs 77 instructions: the missing copies, 6 ins each). Tried: every case as break with one return
   1 after the switch (same merge), the default first (44 ins). */
int gm_JobSlots(int job, int *a, int *b)
{
    switch (job) {
    case 0xb:
        *a = 2;
        *b = 3;
        break;
    case 9:
        *a = 0;
        *b = 4;
        break;
    case 7:
        *a = 4;
        *b = 1;
        break;
    case 6:
        *a = 0;
        *b = 1;
        break;
    case 8:
        *a = 0;
        *b = 3;
        break;
    case 5:
        *a = 1;
        *b = 0;
        break;
    case 0xc:
        *a = 3;
        *b = 2;
        break;
    case 0xa:
        *a = 3;
        *b = 2;
        break;
    case 2:
        *a = 1;
        *b = 0;
        break;
    case 4:
        *a = 1;
        *b = 2;
        break;
    case 0xd:
        *a = 2;
        *b = 0;
        break;
    case 1:
    case 3:
    case 0xe:
    case 0xf:
    case 0x10:
        *a = 1;
        *b = 0;
        break;
    default:
        *a = 1;
        *b = 0;
        break;
    }
    return 1;
}
/* 0x504290, the first title number of the job group that 0x504ad0 gives for x (1 -> 0x29, 2 -> 0x2f, 3 -> 0x32 .. 0xf -> 0x51), 0 for none */


int gm_FirstTitleNumber(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k != 0) {
        switch (k) {
        case 1:
            return 0x29;
        case 2:
            return 0x2f;
        case 3:
            return 0x32;
        case 4:
            return 0x35;
        case 5:
            return 0x21;
        case 6:
            return 0x17;
        case 7:
            return 0x14;
        case 8:
            return 0x1a;
        case 9:
            return 0xe;
        case 0xa:
            return 0x2c;
        case 0xb:
            return 0xb;
        case 0xc:
            return 0x27;
        case 0xd:
            return 0x48;
        case 0xe:
            return 0x4e;
        case 0xf:
            return 0x51;
        }
    }
    return 0;
}
/* 0x504350, the group of the title number n (1 .. 6 and 0x28 .. 0x2d: 7; 0x23 .. 0x27: 2; 0x2e .. 0x33: 3; 0x3a .. 0x3f: 4; 0x40 .. 0x45: 5), 0 else */


int gm_TitleNumberGroup(int n)
{
    if (n >= 1 && n <= 6)
        return 7;
    if (n >= 0x28 && n <= 0x2d)
        return 7;
    if (n >= 0x23 && n <= 0x27)
        return 2;
    if (n >= 0x2e && n <= 0x33)
        return 3;
    if (n >= 0x3a && n <= 0x3f)
        return 4;
    if (n >= 0x40 && n <= 0x45)
        return 5;
    if (n >= 0xd && n <= 6)     /* never true: the original's range is empty */
        return 6;
    return 0;
}
/* 0x5043d0, the title number of the rank r in the job group a: the last title number of the group (1 -> 0x33, 2 -> 0x3f, 3 -> 0x45 .. 0xf -> 0x5e) minus (r - 1); 0 for group 0 or an unknown one */


int gm_TitleNumber(int a, int r)
{
    /* the cases are in the order of their title numbers (the original's case bodies lie so) */
    if (a != 0) {
        r = r - 1;
        switch (a) {
        case 0xb:
            return 6 - r;
        case 9:
            return 0xc - r;
        case 7:
            return 0x12 - r;
        case 6:
            return 0x18 - r;
        case 8:
            return 0x1e - r;
        case 5:
            return 0x27 - r;
        case 0xc:
            return 0x2d - r;
        case 1:
            return 0x33 - r;
        case 0xa:
            return 0x39 - r;
        case 2:
            return 0x3f - r;
        case 3:
            return 0x45 - r;
        case 4:
            return 0x4b - r;
        case 0xd:
            return 0x51 - r;
        case 0xf:
            return 0x5e - r;
        case 0xe:
            return 0x58 - r;
        case 0x10:
            return 0;
        }
    }
    return 0;
}
/* 0x5044b0, the job byte (+0x164) of the character i when its group (0x504ad0) is p, else the byte of the job record of the character (0x506430, +0x74 + p) or 0 */


int gm_TitleJob(int i, int p)
{
    unsigned char *rec;     /* the job record: a job byte per group from +0x74 */

    if (gm_TitleGroup((unsigned char)ai_Chars[i][0x164]) == p)
        return (unsigned char)ai_Chars[i][0x164];
    rec = (unsigned char *)gm_PlayerRecord(ai_Chars[i]);
    if (rec == 0)
        return 0;
    return rec[0x74 + p];
}
/* 0x504510, the share of its title period the character n has served in its job: the period is (16 * the titles
   left (gm_TitlesLeft, 1 .. 5) + 72) hours over (1 + the skill of its job group, at least 0.25: group 11 -> skill 2,
   12 / 10 -> 3, 8 -> 0, 9 -> 4, else 1), its hour of day moved into 7 .. 21; the time served is the job's days
   (+0x174, cleared by a job change) at the clock's hour. Served less than the period: returns served / period in
   minutes and writes to *out (a GCLOCK; the callers declare int *) the date the period completes (now + period -
   served); else returns 1.0; 0.0 when the character has no title left in 1 .. 5 */

float gm_TitlePeriodServed(int n, int *out)
{
    GCLOCK period, served, zero;
    GCLOCK *when;
    double h;
    int k, group, hours, days;
    unsigned short hour;
    float f, share;

    k = gm_TitlesLeft(ORD_CHARS[n].jobId);
    if (k == 0 || k >= 6)
        return 0.0f;
    group = gm_TitleGroup(ORD_CHARS[n].jobId);
    if (group == 0xb)
        f = gm_GetSkill(ai_Chars[n], 2);
    else if (group == 0xc)
        f = gm_GetSkill(ai_Chars[n], 3);
    else if (group == 0xa)
        f = gm_GetSkill(ai_Chars[n], 3);
    else if (group == 8)
        f = gm_GetSkill(ai_Chars[n], 0);
    else if (group == 9)
        f = gm_GetSkill(ai_Chars[n], 4);
    else
        f = gm_GetSkill(ai_Chars[n], 1);
    f = f > 0.25f ? f : 0.25f;
    memset(&period, 0, sizeof(period));
    memset(&served, 0, sizeof(served));
    memset(&zero, 0, sizeof(zero));
    hours = (int)((k * 16.0f + 72.0f) / (f + 1.0f));
    period.day = (int)((h = hours) * (1.0 / 24.0));
    period.hour = (unsigned short)(int)fmod(h, 24.0);
    if (period.hour > 21.0f) {
        period.day++;
        period.hour = (unsigned short)(int)(period.hour - 21.0f + 7.0f);
    } else if (period.hour < 7.0f)
        period.hour += 6;
    days = ORD_CHARS[n].jobDays;
    hour = GM_NOW.hour;
    served.day = days;
    served.hour = hour;
    if (gm_CompareTime(&served, &period) < 0) {
        share = (float)((double)gm_MinutesBetween(&zero, &served) / (double)gm_MinutesBetween(&zero, &period));
        gm_AddTime(&period, hour + GM_NOW.day * 24, 0, 0);
        gm_AddTime(&period, -hour - days * 24, 0, 0);
        when = (GCLOCK *)out;
        *when = period;
        return share;
    }
    return 1.0f;
}
/* 0x504790, sets the job byte (+0x164) of the character i to n: a changed job clears the int +0x174; for a player or priest (kinds 5 / 6 / 7) the old job is stored in the job record (0x506430, +0x74 + the group of the old job) and the new one when it is lower or the slot empty (flag 0x6f2318) */


void gm_SetJob(int i, unsigned n)
{
    unsigned char *rec;     /* the job record: a job byte per group from +0x74 */
    int k;

    if (n != ORD_CHARS[i].jobId)
        ORD_CHARS[i].jobDays = 0;
    rec = (unsigned char *)gm_PlayerRecord(ai_Chars[i]);
    if (rec != 0 && (ORD_CHARS[i].kind == 6 || ORD_CHARS[i].kind == 7 || ORD_CHARS[i].kind == 5)) {
        rec[0x74 + gm_TitleGroup(ORD_CHARS[i].jobId)] = ORD_CHARS[i].jobId;
        k = gm_TitleGroup(n);
        if ((int)n < rec[0x74 + k] || rec[0x74 + k] == 0)
            rec[0x74 + k] = (unsigned char)n;
        ai_PermsDirty = 1;
    }
    ORD_CHARS[i].jobId = (unsigned char)n;
}
/* 0x504840, drops the character c one title rank (title - 1; 0xc when its building is of type 0x20, 0xb for type 0x1f) and clears its jobDays, both sent as field values (cm_AddFieldValue). Returns -1 */

int gm_DropRank(ORDCHAR *c)
{
    int v;

    v = 0;
    if (c->bld != 0 && c->bld->type == 0x20)
        v = 0xc;
    else if (c->bld != 0 && c->bld->type == 0x1f)
        v = 0xb;
    else
        v = (unsigned char)c->title - 1;
    cm_BeginFieldBlock((char *)c, c->netId);
    cm_AddFieldValue(1, 1, 0x165, &v);
    cm_SendFieldValues();
    v = 0;
    cm_BeginFieldBlock((char *)c, c->netId);
    cm_AddFieldValue(4, 1, 0x174, &v);
    cm_SendFieldValues();
    return -1;
}
/* 0x5048e0, the job group of a building type (the inverse of 0x504050; 4 -> 0xb, 5 -> 9, 7 -> 7, 8 -> 6, 9 -> 8, 0xe -> 5, 0x10 -> 0xc, 0x12 -> 1, 0x13 -> 10, 0x14 -> 2, 0x15 -> 3, 0x16 -> 4, 0x1d -> 0xd, 0x1f -> 0xe, 0x20 -> 0xf, else 0) */


int gm_BuildingJob(int t)
{
    switch (t) {
    case 4:
        return 0xb;
    case 5:
        return 9;
    case 7:
        return 7;
    case 8:
        return 6;
    case 9:
        return 8;
    case 0xe:
        return 5;
    case 0x10:
        return 0xc;
    case 0x12:
        return 1;
    case 0x13:
        return 0xa;
    case 0x14:
        return 2;
    case 0x15:
        return 3;
    case 0x16:
        return 4;
    case 0x1d:
        return 0xd;
    case 0x1f:
        return 0xe;
    case 0x20:
        return 0xf;
    default:
        return 0;
    }
}
/* 0x5049c0, 1 when the player p has a building (0x50b010 (p) is not 0) */


int gm_HasBuilding(int p)
{
    return gm_FirstBuilding(p) != 0;
}
/* 0x5049e0, the treasury object of the building b's class: class 5 the object 0x142 (the treasury) of the building 2 .. 5 chosen by the player's job2 (0x1e .. 0x21), class 3 the object 0x115 of the building 1; 0 else */

char *gm_ClassTreasury(ORDBLD *b)
{
    int cls;
    char *r;
    ORDBLD *h;

    r = 0;
    cls = gm_BuildingClass(b->type);
    if (cls == 5) {
        if (ORD_CHARS[ai_PlayerId].job2 == 0x1e)
            h = (ORDBLD *)gm_FirstBuilding(2);
        else if (ORD_CHARS[ai_PlayerId].job2 == 0x1f)
            h = (ORDBLD *)gm_FirstBuilding(3);
        else if (ORD_CHARS[ai_PlayerId].job2 == 0x20)
            h = (ORDBLD *)gm_FirstBuilding(4);
        else if (ORD_CHARS[ai_PlayerId].job2 == 0x21)
            h = (ORDBLD *)gm_FirstBuilding(5);
        else
            h = 0;
        if (h != 0)
            r = gm_Query((void *)h->objList, 2, 6, 0, 0x142);
    } else if (cls == 3) {
        h = (ORDBLD *)gm_FirstBuilding(1);
        r = gm_Query((void *)h->objList, 2, 6, 0, 0x115);
    }
    return r;
}
/* 0x504a80, the object 0x115 (player 1) or 0x142 (the others) of the building of the player p */

char *gm_PlayerSeat(int p)
{
    char *b;

    b = gm_FirstBuilding(p);
    if (p == 1)
        return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
    return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x142);
}
/* 0x504ad0, the job group (0 .. 0x10) of a title number n (six numbers per group: 1 .. 6 -> 0xb, 7 .. 12 -> 9, 13 .. 18 -> 7, 19 .. 24 -> 6, 25 .. 30 -> 8, 31 .. 33 -> 0x10, 34 .. 39 -> 5, 40 .. 45 -> 0xc, 46 .. 51 -> 1, 52 .. 57 -> 10, 58 .. 63 -> 2, 64 .. 69 -> 3, 70 .. 75 -> 4, 76 .. 81 -> 0xd, 83 .. 88 -> 0xe, 89 .. 94 -> 0xf), 0 for others */


int gm_TitleGroup(int n)
{
    switch (n) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return 0xb;
    case 7:
    case 8:
    case 9:
    case 0xa:
    case 0xb:
    case 0xc:
        return 9;
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
        return 7;
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
        return 6;
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
        return 8;
    case 0x1f:
    case 0x20:
    case 0x21:
        return 0x10;
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
        return 5;
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
        return 0xc;
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
        return 1;
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
        return 10;
    case 0x3a:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
        return 2;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
        return 3;
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
        return 4;
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
        return 0xd;
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
        return 0xf;
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
        return 0xe;
    }
    return 0;
}
/* 0x504c00, the job group of a job-pair number n (two numbers per group: 1 .. 2 -> 0xb, 3 .. 4 -> 7, ..) */


int gm_TitleJobGroup(int n)
{
    switch (n) {
    case 0:
        return 0;
    case 1:
    case 2:
        return 0xb;
    case 3:
    case 4:
        return 7;
    case 5:
    case 6:
        return 6;
    case 7:
    case 8:
        return 8;
    case 0xd:
    case 0xe:
        return 5;
    case 0xf:
    case 0x10:
        return 0xc;
    case 0x11:
    case 0x12:
        return 1;
    case 0x13:
    case 0x14:
        return 10;
    case 0x15:
    case 0x16:
        return 2;
    case 0x17:
    case 0x18:
        return 3;
    case 0x19:
    case 0x1a:
        return 4;
    case 0x1b:
    case 0x1c:
        return 0xd;
    case 0x1f:
    case 0x20:
        return 0xe;
    case 0x21:
    case 0x22:
        return 0xf;
    }
    return 0;
}
/* 0x504ce0, the number of titles left in the group of the title number n: the end of its group of six minus n (n in 1 .. 6: 7 - n, 7 .. 12: 13 - n, ..), 0 for others */


int gm_TitlesLeft(int n)
{
    int r;

    r = 0;
    if (0 < n && n < 7)
        return 7 - n;
    if (6 < n && n < 0xd)
        return 0xd - n;
    if (0x21 < n && n < 0x28)
        return 0x28 - n;
    if (0x27 < n && n < 0x2e)
        return 0x2e - n;
    if (0x2d < n && n < 0x34)
        return 0x34 - n;
    if (0x39 < n && n < 0x40)
        return 0x40 - n;
    if (0x3f < n && n < 0x46)
        return 0x46 - n;
    if (0x45 < n && n < 0x4c)
        return 0x4c - n;
    if (0xc < n && n < 0x13)
        return 0x13 - n;
    if (0x33 < n && n < 0x3a)
        return 0x3a - n;
    if (0x18 < n && n < 0x1f)
        return 0x1f - n;
    if (0x12 < n && n < 0x19)
        return 0x19 - n;
    if (0x4b < n && n < 0x52)
        return 0x52 - n;
    if (0x58 < n && n < 0x5f)
        return 0x5f - n;
    if (0x52 < n && n < 0x59)
        r = 0x59 - n;
    return r;
}
/* 0x504e00, the 0x504ec0 value of (a, b) times 252, truncated (a tail through __ftol) */


int gm_SkillPoints(char *c, int skill)
{
    return (int)(gm_GetSkill(c, skill) * 252.0f);
}
/* 0x504e20, mixes the two 0x504ec0 values u = f (a, c) and v = f (b, c): ((u * u - v) * 0.4 + 0.6 + (u - v * v) * 0.6 + 0.4) * 0.5 */

float gm_Mix(int a, int b, int c)
{
    float u, v;

    u = gm_GetSkill(a, c);
    v = gm_GetSkill(b, c);
    return (float)((((double)u * u - v) * 0.4 + 0.6 + ((double)u - (double)v * v) * 0.6 + 0.4) * 0.5);
}
/* 0x504e80, the same mix for two floats */

float gm_Mix2(float u, float v)
{
    return (float)((((double)u * u - v) * 0.4 + 0.6 + ((double)u - (double)v * v) * 0.6 + 0.4) * 0.5);
}

int amt_GetKind(int k, struct AMTKIND *out);                     /* 0x4e6f80 */
float gm_AddHealth(unsigned short id, int d);                   /* 0x4fca40 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */

int ai_InventoryHas(void *ch, int id);                     /* 0x5ba6b0 */
int he_NoEvent9bFor();                     /* 0x56f3b0 */

/* 0x504ec0, the skill value (0 .. 1) of the character c for the skill number 0 .. 4 (-1 when there is none): the base byte (+0x80 + skill) / 252, plus the bonuses of the character's items (the group (2, 2) records 0x53e400: float +0xb0 when the first skill byte (+0xac) is the skill, +0xb4 for the second (+0xad); halved when 0x5ba6b0 (c, 0x2c4) is 1), plus 0x5ba6b0 (c, 0x15a) / 6 for the skill 3, minus a penalty from the bit fields of the word +0x2c (a jump table over the skills: fields at bits 4 / 8 / 12 / 17 / 20 / 25 times 0.0333 / 0.0555 / 0.1667), plus 0.5 when the skill's flag (+0x1cd: bit 2 / 3 / 4 for the skills 2 / 3 / 4) is set; clamped to 0 .. 1 and scaled by 0.5 (0.75) when the character is no ruler (0x56f3b0) and 0x5ba6b0 (c, 0x2c4) is 0 (else). ROUGH */
float gm_GetSkill(char *c, int skill)
{
    float s, acc, r;
    unsigned f;
    char *it;
    int v;

    r = 0.0f;
    if (c == 0 || skill >= 5)
        return -1.0f;
    s = (unsigned char)c[0x80 + skill] * 0.003968254f;
    if (skill != 1) {
        acc = 0.0f;
        it = (char *)he_FindEvent(2, 2, *(unsigned short *)c, 0, 0x82);
        while (it != 0) {
            if ((unsigned char)it[0xac] == skill)
                acc = acc + *(float *)(it + 0xb0);
            else if ((unsigned char)it[0xad] == skill)
                acc = acc + *(float *)(it + 0xb4);
            it = (char *)he_NextEvent();
        }
        v = ai_InventoryHas(c, 0x2c4);
        if (v == 1)
            acc = acc * 0.5f;
        s = acc + s;
    }
    if (skill == 3) {
        v = ai_InventoryHas(c, 0x15a);
        s = v * 0.1666667f + s;
    } else if (skill > 4)
        goto after;
    f = *(unsigned *)(c + 0x2c);
    switch (skill) {
    case 0:
        r = ((f >> 20) & 7) * 0.0555f + ((f >> 4) & 0xf) * 0.03333333432674408;
        break;
    case 1:
        r = ((f >> 8) & 0xf) * 0.03333333432674408 + ((f >> 20) & 7) * 0.0555f;
        break;
    case 2:
        r = ((f >> 17) & 7) * 0.055500001654028894 + ((f >> 25) & 0xf) * 0.03333333432674408;
        break;
    case 3:
        r = ((f >> 8) & 0xf) * 0.055500001654028894 + ((f >> 25) & 0xf) * 0.03333333432674408;
        break;
    case 4:
        r = ((f >> 12) & 3) * 0.1666667f + ((f >> 4) & 0xf) * 0.03333333432674408;
        break;
    }
after:
    s = s - r;
    if (skill == 4 ? (*(unsigned *)(c + 0x1cc) & 0x400) != 0 : skill == 3 ? (*(unsigned *)(c + 0x1cc) & 0x200) != 0 : skill == 2 ? (*(unsigned *)(c + 0x1cc) & 0x800) != 0 : 0)
        s = s + 0.5f;
    if (1.0f > s) {
        if (s <= 0.0f)
            s = 0.0f;
    } else
        s = 1.0f;
    if (he_NoEvent9bFor(c) == 0) {
        v = ai_InventoryHas(c, 0x2c4);
        if (v == 0)
            return s * 0.5f;
        return s * 0.75f;
    }
    return s;
}
/* 0x5051d0, the record n (3 ints; kinds above 9: the first one) of the table 0x617850 into out; 1 for n <= 9, else 0 */

int gm_Unk5051d0(int n, int *out)
{
    if (9 < n) {
        out[0] = ai_Unk617850[0];
        out[1] = ai_Unk617850[1];
        out[2] = ai_Unk617850[2];
        return 0;
    }
    out[0] = ai_Unk617850[n * 3];
    out[1] = ai_Unk617850[n * 3 + 1];
    out[2] = ai_Unk617850[n * 3 + 2];
    return 1;
}
/* 0x505230, lets the character c (words +0x2c: eight bit fields of 4 / 4 / 2 / 3 / 3 / 3 / 2 / 4 bits at 4, 8, 12, 14, 17, 20, 23, 25) change one of its empty fields: a random start field (0 .. 7), the first field from there whose bits are all 0; a random value (0 .. the record limit of 0x5051d0 (field + 2)) is xored in, the new word is sent (0x4c3c00 (4, 1, 0x2c)) and the health changes by the cost (0x4fca40 (id, -the float of the record)); *kind = the field number + 2, *health = the new health. 1 when a field changed, else 0. ROUGH */

int gm_Unk505230(short *c, unsigned *kind, float *health)
{
    unsigned w, f, v, s, x;
    int free_[8];
    int i, rec[3], cost;
    float h;

    w = *(unsigned *)(c + 0x16);
    free_[0] = (w & 0xf0) == 0;
    free_[1] = (w & 0xf00) == 0;
    free_[2] = (w & 0x3000) == 0;
    free_[3] = (w & 0x1c000) == 0;
    free_[4] = (w & 0xe0000) == 0;
    free_[5] = (w & ((unsigned)&he_Events + 0x54c0)) == 0;
    free_[6] = (w & 0x1800000) == 0;
    free_[7] = (w & 0x1e000000) == 0;
    f = util_RandomU(8) & 0xffff;
    i = 8;
    while (free_[f] != 1) {
        i--;
        f = (f + 1) % 8;
        if (i == 0)
            return 0;
    }
    if ((int)f < 0 || 7 < (int)f)
        return 0;
    s = *(unsigned *)(c + 0x16);
    f = f + 2;
    if (gm_Unk5051d0(f, rec) == 0)
        return 0;
    v = util_RandomU(rec[2]) & 0xffff;
    cost = (int)(float)rec[0];
    if (kind != 0)
        *kind = -(v != 0) & f;
    if (v == 0)
        return 0;
    if (f == 2)
        x = (v << 4 ^ s) & 0xf0;
    else if (f == 3)
        x = (v << 8 ^ s) & 0xf00;
    else if (f == 4)
        x = (v << 0xc ^ s) & 0x3000;
    else if (f == 5)
        x = (v << 0xe ^ s) & 0x1c000;
    else if (f == 6)
        x = (v << 0x11 ^ s) & 0xe0000;
    else if (f == 7)
        x = (v << 0x14 ^ s) & ((unsigned)&he_Events + 0x54c0);
    else if (f == 8)
        x = (v << 0x17 ^ s) & 0x1800000;
    else if (f == 9)
        x = (v << 0x19 ^ s) & 0x1e000000;
    else
        goto send;
    s = s ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &s);
    cm_SendFieldDeltas();
    h = gm_AddHealth(*c, -cost);
    if (health == 0)
        return 1;
    *health = h;
    return 1;
}
/* 0x505440, lets the character c (when the low 4 bits of +0x2c are 0) get a random first field value (record 1 of 0x5051d0), sent as in 0x505230, and pays its cost (0x4fca40). Returns the new health or 0. ROUGH */

float gm_Unk505440(short *c)
{
    unsigned w, v, nw;
    int rec[3];
    int cost;

    w = *(unsigned *)(c + 0x16);
    if ((w & 0xf) != 0)
        return 0.0f;
    if (gm_Unk5051d0(1, rec) == 0)
        return 0.0f;
    v = util_RandomU(rec[2]);
    if ((v & 0xffff) == 0)
        return 0.0f;
    nw = ((v & 0xffff) ^ w) & 0xf ^ w;
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &nw);
    cm_SendFieldDeltas();
    cost = (int)(float)rec[0];
    return gm_AddHealth(*c, -cost);
}
/* 0x505ca0, fills out[0 .. n-1] with 0 .. n-1 and shuffles it with (n / 2 + max (1, n / 4)) random swaps of different places (n > 1) */

void gm_Shuffled(unsigned char n, int *out)
{
    int i, cnt, swaps;
    unsigned a, b;
    int t;
    unsigned m;

    i = 0;
    if (n != 0) {
        do {
            out[i] = i;
            i++;
        } while (i < (int)n);
    }
    if (1 < n) {
        if ((n & 0xfc) < 5)
            m = 1;
        else
            m = n >> 2;
        cnt = (n >> 1) + m;
        swaps = 0;
        if (cnt != 0) {
            do {
                a = util_RandomU(n);
                b = util_RandomU(n) & 0xffff;
                if (a != b) {
                    t = out[a];
                    out[a] = out[b];
                    out[b] = t;
                    swaps++;
                }
            } while (swaps < cnt);
        }
    }
}
/* 0x505d30, shuffles the n ints at out in place (n / 2 + max (1, n / 4) swaps of different places) */

void gm_Shuffle(unsigned n, int *out)
{
    unsigned m, a, b;
    int cnt, swaps, t;

    if (n >= 2) {
        m = n >> 2;
        if (m <= 1)
            m = 1;
        swaps = 0;
        cnt = (n >> 1) + m;
        if (0 < cnt) {
            do {
                a = util_RandomU(n);
                b = util_RandomU(n) & 0xffff;
                if (a != b) {
                    t = out[a];
                    out[a] = out[b];
                    out[b] = t;
                    swaps++;
                }
            } while (swaps < cnt);
        }
    }
}
/* 0x505da0, a random age for the value n: for 0 a value of 0x15 .. 0x3e, below 42 n + (0 .. 5), else n + (0 .. 10) */

char gm_RandomAge(unsigned char n)
{
    char c;
    int u;

    if (n == 0) {
        c = (char)util_RandomU(0x2a);
        return c + 0x15;
    }
    u = 0xb;
    if ((float)n < 42.0f)
        u = 6;
    c = (char)util_RandomU(u);
    return n + c;
}
/* 0x505df0, the character with the id (int +4) in the character table, 0 when none */

short *gm_FindChar(int id)
{
    int *p;
    int i;

    i = 0;
    p = (int *)(ai_Chars[0] + 4);
    while ((short)p[-1] == -1 || *p != id) {
        p += 0x87;
        i++;
        if ((int)p >= (int)((unsigned)&ai_Chars + 0x65404))
            return 0;
    }
    return (short *)ai_Chars[i];
}
/* 0x505e30, the age (word +10) of the character with the number id as a float */

float gm_CharAge(unsigned short id)
{
    return (float)*(unsigned short *)(ai_Chars[id] + 10);
}
/* 0x505e60, the level of the character id (0 for a free slot or an inactive one, 1 when 0x4e6f80 has no entry for its job byte +0x166): the record's byte + 1; below the maximum 10 and with no flag it is raised to the level of the master (+0x1a0, 0x505df0, once) when that is higher */

int gm_CharLevel(unsigned short id, int again)
{
    int r, k, l2;
    short *m;
    int rec[4];

    id &= 0xffff;
    if (*(int *)(ai_Chars[id] + 4) == -1 || ai_Chars[id][8] == 0)
        return 0;
    r = amt_GetKind(ai_Chars[id][0x166], rec);
    if (r == 0)
        return 1;
    k = (rec[2] & 0xff) + 1;
    if ((char)rec[2] == 10)
        return k;
    if (again != 0)
        return k;
    m = gm_FindChar(*(int *)(ai_Chars[id] + 0x60));
    if (m != 0) {
        l2 = gm_CharLevel(*m, 1);
        if (k <= l2)
            return gm_CharLevel(*m, 1);
    }
    return k;
}
/* 0x505f30, gm_CharLevel for the previous office (+0x167): its level, raised to the master's when that is higher */

int gm_CharPrevLevel(unsigned short id, int again)
{
    int r, k, l2;
    short *m;
    int rec[4];

    id &= 0xffff;
    if (*(int *)(ai_Chars[id] + 4) == -1 || ai_Chars[id][8] == 0)
        return 0;
    r = amt_GetKind(ai_Chars[id][0x167], rec);
    if (r == 0)
        return 1;
    k = (rec[2] & 0xff) + 1;
    if ((char)rec[2] == 10)
        return k;
    if (again != 0)
        return k;
    m = gm_FindChar(*(int *)(ai_Chars[id] + 0x60));
    if (m != 0) {
        l2 = gm_CharPrevLevel(*m, 1);
        if (k <= l2)
            return gm_CharPrevLevel(*m, 1);
    }
    return k;
}
/* 0x506000, the birthday of the character c as a calendar record (month byte, day byte, year word) in out: the birth date (the clock minus the age +10, an inactive character from the word +0x28), the month = ((x >> 3) % 13 + the birth day % 11) % 12 + 1 and the day = (x >> 5) % 28 + 1 with x = the int +0x30 xor the year */

void gm_BirthDate(char *c, unsigned *out)
{
    int clk[4];
    unsigned char d[16];
    int day;
    unsigned x;

    ((int *)clk)[1] = *(int *)(ai_Time + 4);
    ((int *)clk)[2] = *(int *)(ai_Time + 8);
    day = *(int *)ai_Time;
    if (c[8] == 0)
        day = *(short *)(c + 0x28);
    day = day - *(unsigned short *)(c + 10);
    clk[0] = day;
    gm_ClockToDate(clk, d);
    x = *(unsigned *)(c + 0x30) ^ *(unsigned short *)(d + 2);
    d[1] = (char)(((x >> 3) % 0xd + day % 0xb) % 0xc) + 1;
    d[0] = (char)((x >> 5) % 0x1c) + 1;
    out[0] = *(unsigned *)d;
    out[1] = *(unsigned *)(d + 4);
    *(short *)(out + 2) = *(short *)(d + 8);
}
/* 0x5060d0, the birthday of an inactive character c (word +0x28 the birth day) into out; 0 for an active one, 1 else (month = ((x >> 3) % 7 + day % 19) % 12 + 1) */

int gm_BirthDate2(char *c, unsigned *out)
{
    short day;
    int clk[4];
    unsigned char d[16];
    unsigned x;

    if (c[8] != 0)
        return 0;
    day = *(short *)(c + 0x28);
    clk[0] = day;
    gm_ClockToDate(clk, d);
    x = *(unsigned *)(c + 0x30) ^ *(unsigned short *)(d + 2);
    d[1] = (char)(((x >> 3) % 7 + day % 0x13) % 0xc) + 1;
    d[0] = (char)((x >> 5) % 0x1c) + 1;
    out[0] = *(unsigned *)d;
    out[1] = *(unsigned *)(d + 4);
    *(short *)(out + 2) = *(short *)(d + 8);
    return 1;
}

/* the bit fields of the word +0x2c of a character (the skill levels): field k starts at the bit FIELD_SHIFT[k] with the mask FIELD_MASK[k] (4, 4, 2, 3, 3, 3, 2, 4 bits) */

/* 0x5054f0, raises one of the four fields 0 .. 3 (records 2 .. 5 of 0x5051d0) of the character c that is empty (a random start), by a random amount up to the record's limit (+0x4 scaled by 0x5051d0); the word is sent (0x4c3c00 (4, 1, 0x2c)) and the cost paid (0x4fca40). Returns the record number or 0. ROUGH */
unsigned gm_Unk5054f0(short *c)
{
    unsigned w, f, x, v;
    unsigned tab[8];
    int i, cost;
    int rec[3];

    w = *(unsigned *)(c + 0x16);
    tab[0] = 2;
    tab[1] = 3;
    tab[2] = 4;
    tab[3] = 5;
    tab[4] = (w & 0xf0) == 0;
    tab[5] = (w & 0xf00) == 0;
    tab[6] = (w & 0x3000) == 0;
    tab[7] = (w & 0x1c000) == 0;
    f = util_RandomU(4) & 0xffff;
    i = 4;
    while (tab[f + 4] != 1) {
        i--;
        f = (f + 1) % 4;
        if (i == 0)
            return 0;
    }
    f = tab[f];
    if (gm_Unk5051d0(f, rec) == 0)
        return 0;
    v = util_RandomU((int)(float)rec[2]) & 0xffff;
    if (v == 0)
        return 0;
    cost = (int)(float)rec[0];
    w = *(unsigned *)(c + 0x16);
    if (f == 2)
        x = (v << 4 ^ w) & 0xf0;
    else if (f == 3)
        x = (v << 8 ^ w) & 0xf00;
    else if (f == 4)
        x = (v << 0xc ^ w) & 0x3000;
    else {
        if (f != 5)
            goto send;
        x = (v << 0xe ^ w) & 0x1c000;
    }
    w = w ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &w);
    cm_SendFieldDeltas();
    gm_AddHealth(*c, -cost);
    return f;
}
/* 0x5056a0, like 0x5054f0 for the fields 3 .. 6 (records 5 .. 8) */

unsigned gm_Unk5056a0(short *c)
{
    unsigned w, f, x, v;
    unsigned tab[8];
    int i, cost;
    int rec[3];

    w = *(unsigned *)(c + 0x16);
    tab[0] = 5;
    tab[1] = 6;
    tab[2] = 7;
    tab[3] = 8;
    tab[4] = (w & 0x1c000) == 0;
    tab[5] = (w & 0xe0000) == 0;
    tab[6] = (w & ((unsigned)&he_Events + 0x54c0)) == 0;
    tab[7] = (w & 0x1800000) == 0;
    f = util_RandomU(4) & 0xffff;
    i = 4;
    while (tab[f + 4] != 1) {
        i--;
        f = (f + 1) % 4;
        if (i == 0)
            return 0;
    }
    f = tab[f];
    if (gm_Unk5051d0(f, rec) == 0)
        return 0;
    v = util_RandomU((int)(float)rec[2]) & 0xffff;
    if (v == 0)
        return 0;
    cost = (int)(float)rec[0];
    w = *(unsigned *)(c + 0x16);
    if (f == 5 || f == 8)
        x = (v << 0x17 ^ w) & 0x1800000;
    else if (f == 6)
        x = (v << 0x11 ^ w) & 0xe0000;
    else if (f == 7)
        x = (v << 0x14 ^ w) & ((unsigned)&he_Events + 0x54c0);
    else
        goto send;
    w = w ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &w);
    cm_SendFieldDeltas();
    gm_AddHealth(*c, -cost);
    return f;
}
/* 0x505840, clears one set field of the word +0x2c of the character c (a random start field of 8, the first that is not 0) and sends the word; returns the record number (2 .. 9) or 0 */

int gm_Unk505840(char *c)
{
    unsigned w, f;
    int tab[8];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xf0) != 0;
    tab[1] = (w & 0xf00) != 0;
    tab[2] = (w & 0x3000) != 0;
    tab[3] = (w & 0x1c000) != 0;
    tab[4] = (w & 0xe0000) != 0;
    tab[5] = (w & ((unsigned)&he_Events + 0x54c0)) != 0;
    tab[6] = (w & 0x1800000) != 0;
    tab[7] = (w & 0x1e000000) != 0;
    f = util_RandomU(8) & 0xffff;
    i = 8;
    do {
        if (tab[f] == 1) {
            if (f == 0xffffffff)
                return 0;
            w = *(unsigned *)(c + 0x2c);
            if (f == 0) {
                w &= 0xffffff0f;
                r = 2;
            } else if (f == 1) {
                w &= 0xfffff0ff;
                r = 3;
            } else if (f == 2) {
                w &= 0xffffcfff;
                r = 4;
            } else if (f == 3) {
                w &= 0xfffe3fff;
                r = 5;
            } else if (f == 4) {
                w &= 0xfff1ffff;
                r = 6;
            } else if (f == 5) {
                w &= 0xff8fffff;
                r = 7;
            } else if (f == 6) {
                w &= 0xfe7fffff;
                r = 8;
            } else if (f == 7) {
                w &= 0xe1ffffff;
                r = 9;
            }
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(4, 1, 0x2c, &w);
            cm_SendFieldDeltas();
            return r;
        }
        i--;
        f = (f + 1) % 8;
    } while (i != 0);
    return 0;
}
/* 0x5059d0, clears one set field of the first four (a random start field of 4); returns the record number (2 .. 5) or 0 */

int gm_Unk5059d0(char *c)
{
    unsigned w, f;
    int tab[4];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xf0) != 0;
    tab[1] = (w & 0xf00) != 0;
    tab[2] = (w & 0x3000) != 0;
    tab[3] = (w & 0x1c000) != 0;
    f = util_RandomU(4) & 0xffff;
    i = 4;
    do {
        if (tab[f] == 1) {
            if (f == 0xffffffff)
                return 0;
            w = *(unsigned *)(c + 0x2c);
            if (f == 0) {
                w &= 0xffffff0f;
                r = 2;
            } else if (f == 1) {
                w &= 0xfffff0ff;
                r = 3;
            } else if (f == 2) {
                w &= 0xffffcfff;
                r = 4;
            } else if (f == 3) {
                w &= 0xfffe3fff;
                r = 5;
            }
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(4, 1, 0x2c, &w);
            cm_SendFieldDeltas();
            return r;
        }
        i--;
        f = (f + 1) % 4;
    } while (i != 0);
    return 0;
}
/* 0x505ad0, clears one set field of the fields 4 / 5 / 6 / 1 (a random start of 4); returns the record number (6 / 7 / 8 / 3) or 0 */

int gm_Unk505ad0(char *c)
{
    unsigned w, f;
    int tab[4];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xe0000) != 0;
    tab[1] = (w & ((unsigned)&he_Events + 0x54c0)) != 0;
    tab[2] = (w & 0x1800000) != 0;
    tab[3] = (w & 0xf00) != 0;
    f = util_RandomU(4) & 0xffff;
    i = 4;
    do {
        if (tab[f] == 1) {
            if (f == 0xffffffff)
                return 0;
            w = *(unsigned *)(c + 0x2c);
            if (f == 0) {
                w &= 0xfff1ffff;
                r = 6;
            } else if (f == 1) {
                w &= 0xff8fffff;
                r = 7;
            } else if (f == 2) {
                w &= 0xfe7fffff;
                r = 8;
            } else if (f == 3) {
                w &= 0xfffff0ff;
                r = 3;
            }
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(4, 1, 0x2c, &w);
            cm_SendFieldDeltas();
            return r;
        }
        i--;
        f = (f + 1) % 4;
    } while (i != 0);
    return 0;
}
/* 0x505be0, fills the bytes out[0 .. n-1] with 0 .. n-1 and shuffles them (as 0x505ca0 for bytes) */

void gm_ShuffledBytes(unsigned char n, unsigned char *out)
{
    unsigned char i, cnt, swaps, a, b, t, m;
    unsigned char *p;

    i = 0;
    p = out;
    if (n != 0) {
        do {
            *p = i;
            i++;
            p++;
        } while (i < n);
    }
    if (1 < n) {
        if ((n & 0xfc) < 5)
            m = 1;
        else
            m = n >> 2;
        swaps = 0;
        cnt = (n >> 1) + m;
        if (cnt != 0) {
            do {
                a = (unsigned char)util_RandomU(n);
                b = (unsigned char)util_RandomU(n);
                if (a != b) {
                    t = out[a];
                    out[a] = out[b];
                    swaps++;
                    out[b] = t;
                }
            } while (swaps < cnt);
        }
    }
}

float gm_Liking();                     /* 0x50c550 */
void ai_RestartMission(struct G13MIS *m);                     /* 0x4b3a90 */
void he_Unk53ed40();                     /* 0x53ed40 */
char ai_Unk61ac40[] = "player %i: sum:%f points:%i verm\366gen:%i vermoegen_trend:%f ansehen:%f talente:%f amt:%f";  /* 0x61ac40, a log format */

/* 0x506180, the ranking of the players: collects the ids of up to 8 characters of the kinds 5 / 6 / 7 into out (records of 0x24 bytes, the id first; *count of them), and fills per record: +0x18 the sum of the five skill bytes (+0x80) / 1260, +0xc the byte +0xd / 9, +0x10 the two 0x4e6f80 levels (jobs +0x166 / +0x169) / 13, +0x14 the mean opinion (0x50c550 over the other players and townsmen) / 100, +4 the 0x50a640 value (at least 1; the maximum in 0x506180's local), +8 = that / the maximum, +0x1c the weighted sum (0.25 * +0x18 + 0.3 * +8 + 0.15 * (+0xc + +0x10 + +0x14)), +0x20 its truncation; the line is formatted with 0x61ac40. ROUGH, the lost float arguments are guessed */
void gm_Unk506180(int *count, unsigned short *out)
{
    int n, i, k, mx, sum, cnt, j, v;
    char *c;
    unsigned short *r;
    float *f;
    float acc;
    int tmp[4];
    unsigned u1;
    char buf[256];
    unsigned short idx;

    n = 0;
    mx = 0;
    idx = 0;
    r = out;
    c = ai_Chars[0] + 2;
    do {
        if (7 < n)
            break;
        if (*c == 6 || *c == 7 || *c == 5) {
            *r = idx;
            n++;
            r += 0x12;
        }
        c += 0x21c;
        idx++;
    } while ((int)c < (unsigned)((unsigned)ai_Chars + 0x65402));
    *count = n;
    if (n != 0) {
        r = out;
        for (i = 0; i < *count; i++, r += 0x12) {
            c = ai_Chars[*r];
            sum = 0;
            for (k = 0; k < 5; k++)
                sum += (unsigned char)c[0x80 + k];
            *(float *)(r + 0xc) = (float)sum * 0.0007936508f;
            *(float *)(r + 6) = (float)(unsigned char)c[0xd] * 0.11111111f;
            amt_GetKind(c[0x166], tmp);
            u1 = tmp[2] & 0xff;
            amt_GetKind(c[0x169], tmp);
            *(float *)(r + 8) = ((float)(tmp[2] & 0xff) + (float)u1) * 0.07692308f;
            cnt = 0;
            acc = 0.0f;
            for (j = 0; j < 0x300; j++) {
                char *o = ai_Chars[j];
                if ((unsigned)j != *r && *(short *)o != -1 && (o[2] == 6 || o[2] == 7 || o[2] == 5 || o[2] == 4)) {
                    acc = (float)(gm_Liking(*r, j) + acc);
                    cnt++;
                }
            }
            *(float *)(r + 10) = (float)((acc * 0.01) / (float)cnt);
            v = (int)gm_GetWorth(*r);
            if (v < 2)
                v = 1;
            else
                v = (int)gm_GetWorth(*r);
            *(int *)(r + 2) = v;
            if (mx < v)
                mx = v;
        }
        f = (float *)(out + 6);
        for (i = 0; i < *count; i++, f += 9) {
            float t = f[-2];
            float q = (float)(int)t / (float)mx;
            f[-1] = q;
            f[4] = f[3] * 0.25f + q * 0.3f + (f[0] + f[1] + f[2]) * 0.15f;
            f[5] = (float)(int)f[4];
            sprintf(buf, ai_Unk61ac40, i, (double)f[5], f[4], t, (double)q, (double)f[2], (double)f[3], (double)f[1]);
        }
    }
}
/* 0x506430, the record (0x56 words each, 0x7d5f80 ..) of a player-like character c (kinds 5 / 6 / 7 whose word +0x50 has the top bit set; the low 4 bits give the slot), 0 else */

short *gm_PlayerRecord(char *c)
{
    char k;
    int u;

    k = c[2];
    if ((k == 6 || k == 7 || k == 5) && (short)*(unsigned short *)(c + 0x50) < 0 && (u = *(unsigned short *)(c + 0x50) & 0xf, u < 0x10))
        return (short *)gm_PlayerRecords + u * 0x56;
    return 0;
}
/* 0x506480, counts the active relatives (the five ids from +0x68, 0x505df0, +8 set) of the character c into *n and returns 1 when one of them is older than 0xb */

int gm_Unk506480(char *c, int *n)
{
    int r, cnt, k;
    short *p;
    int *q;

    r = 0;
    cnt = 0;
    q = (int *)(c + 0x68);
    for (k = 5; k != 0; k--, q++) {
        p = gm_FindChar(*q);
        if (p != 0 && ((char *)p)[8] != 0 && (cnt++, 0xb < *(unsigned short *)((char *)p + 10)))
            r = 1;
    }
    if (n != 0)
        *n = cnt;
    return r;
}
/* 0x5064d0, swaps the two characters a and b (0x505df0): runs 0x4b3a90 for a, clears the scene links (0x502b60 for the buildings of class 2 owned by a), swaps the 0x21c-byte records (a keeping its slot number, the byte at +9 of the first, the job bytes fixed up with the job tables 0x504ad0 / 0x5044b0 / 0x504ce0, the name copied or the icon looked up 0x4fb330), re-points the object lists (+0x178) at the new ids, swaps all references to a / b in the relation ids (+0x5c .. +0x7c of every character), adds the kind 9 objects of a's list to a's id (0x500970), restores the money (+0x1a8) and the state words and re-registers (0x503c60 (id, 0)). Returns a's slot or -1 / -2 when a / b is unknown. ROUGH */

unsigned gm_Unk5064d0(int a, int b)
{
    unsigned short *ca, *cb;
    char tmpa[0x21c];
    char tmpb[0x21c];
    unsigned sa, sb;
    int money, i, k, off;
    int *q;
    unsigned short *o;

    ca = (unsigned short *)gm_FindChar(a);
    if (ca == 0)
        return -1;
    ai_RestartMission(ca);
    for (i = 0; i < 0x100; i++) {
        char *bl = ai_Blds + i * 0xa9;
        if (*bl != 0 && gm_BuildingClass((unsigned char)*bl) == 2 && *(unsigned short *)(bl + 0x27) == *ca)
            gm_AssignBuilding((int)bl, *(unsigned short *)ai_PlayerCharC, *(unsigned short *)ai_PlayerCharC);
    }
    sa = *ca;
    money = (int)gm_GetWorth(sa);
    cb = (unsigned short *)gm_FindChar(b);
    if (cb == 0)
        return -2;
    he_Unk53ed40(ca);
    he_EndPersonEvents(ca);
    he_EndPersonEvents(cb);
    sb = *cb;
    if (ca[0x28] != cb[0x28])
        cb[0x28] = ca[0x28];
    amt_ClearOffices(ca);
    memcpy(tmpb, cb, 0x21c);
    *(unsigned short *)tmpb = (unsigned short)sa;
    *(int *)(tmpb + 4) = *(int *)(ca + 2);
    *(int *)(tmpb + 0x170) = *(int *)(ca + 0xb8);
    memcpy(tmpa, ca, 0x21c);
    *(unsigned short *)tmpa = *cb;
    *(int *)(tmpa + 4) = *(int *)(cb + 2);
    ai_Chars[sa][0] = tmpb[0];
    memcpy(ai_Chars[sa], tmpb, 0x21c);
    memcpy(ai_Chars[sb], tmpa, 0x21c);
    for (o = *(unsigned short **)(ai_Chars[sa] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f))
        *(int *)((char *)o + 6) = *(int *)(ai_Chars[sa] + 4);
    for (o = *(unsigned short **)(ai_Chars[sb] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f))
        *(int *)((char *)o + 6) = *(int *)(ai_Chars[sb] + 4);
    for (k = 0; k < 0x300; k++) {
        q = (int *)(ai_Chars[k] + 0x5c);
        for (i = 0; i < 8; i++) {
            if (q[i] == a)
                q[i] = b;
            else if (q[i] == b)
                q[i] = a;
        }
    }
    for (o = *(unsigned short **)(ai_Chars[sa] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f)) {
        if (*o != 0 && ai_Goods[*o * 0x41] == 9)
            gm_AddObjekt(*(int *)(ai_Chars[sa] + 4), *o, *(int *)((char *)o + 0xe));
    }
    off = sa * 0x21c;
    *(int *)(ai_Chars[0] + off + 0x1a8) = money;
    *(int *)(ai_Chars[0] + off + 0x194) = 4;
    *(int *)(ai_Chars[0] + off + 0x1c8) = 0;
    gm_KillSpieler(sa, 0);
    return sa;
}

int gm_EquipmentValue(unsigned char *b, int unused);                     /* 0x509b10 */
int gm_Unk509eb0();                     /* 0x509eb0 */
void ai_Unk4c5010(int a, int b, int c);                     /* 0x4c5010 */
void cm_SendFloatDelta(int id, int field, float delta);                     /* 0x4c4ac0 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
float util_RandomSq(void);                  /* 0x42ec40 */

int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
int gm_BuildingClass();                     /* 0x501d80 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
int gm_Unk509eb0();                     /* 0x509eb0 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
unsigned char * gm_BuildNext(void);                     /* 0x501030 */

/* 0x506890, the reaction to an order of the character a for the character b (the target): a's order record (0x4f2540) is answered (0x4f01d0; for b of the same group 0x4f0900); every object of a (0x501190 (1, 4, id) / 0x501030
   iteration) is told to b (message 0x4c54a0; for the player's own character also a message 0x1b and 0x31 to the other players with an interest), the objects are collected (up to 0x240) and the result is put into the
   player's text (0x1c4a .. 0x1c4e; a list of at most 6 objects plus "and n more" 0xb6) and sent (0x53dda0) to the player(s) concerned. ROUGH */
void gm_Unk506890(char *a, char *b)
{
    char *objs[0x240];
    char t1[0x800];
    char t2[0x1000];
    char m[0xf8];
    char *o;
    int *e;
    char *s;
    char *pl;
    int n, flag, k, i, j, v;

    n = 0;
    flag = 0;
    if (gv_FindHeldEntry(a, (unsigned char **)&o, (int *)&e) != 0) {
        unk_Unk4f01d0((int *)a, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
        if (b != 0 && b != ai_PlayerCharC && e[1] == *(int *)(a + 4))
            gv_OrderSetGraveHolder((int *)b, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
    }
    s = (char *)gm_BuildQuery(1, 4, *(unsigned short *)a);
    pl = ai_PlayerCharC;
    while (s != 0) {
        k = gm_BuildingClass((unsigned char)s[0]);
        if (k == 2)
            cm_SendOwnership(*(int *)(s + 1), *(int *)(pl + 4), *(int *)(pl + 4));
        else {
            cm_SendOwnership(*(int *)(s + 1), *(int *)(b + 4), *(int *)(b + 4));
            flag = 1;
        }
        pl = ai_PlayerCharC;
        if (b == pl && k != 2 && (s[0x5a] & 1) == 0) {
            memset(m, 0, 0xf8);
            m[4] = 0x1b;
            *(int *)(m + 8) = *(int *)(pl + 4);
            *(int *)(m + 0xc) = -1;
            m[0x36] = 2;
            *(int *)(m + 0x58) = *(int *)(s + 1);
            *(int *)(m + 0x5c) = gm_EquipmentValue(s, (char)s[0x5c]);
            cm_SendLongMessage(m);
            for (i = 0; (char *)((unsigned)&ai_CharInts + i * 0x21c) < (char *)((unsigned)&gm_PlayerRecordBytes + 0x2); i++) {
                char *c = ai_Chars[i];
                if (*(char **)(c + 0x168 + 4) == s && *(int *)(c + 4) != *(int *)(a + 4)) {
                    memset(m, 0, 0xf8);
                    m[4] = 0x31;
                    *(int *)(m + 8) = *(int *)(pl + 4);
                    *(int *)(m + 0xc) = *(int *)(s + 1);
                    *(int *)(m + 0x28) = *(int *)ai_Time;
                    *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
                    *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
                    *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                    m[0x36] = 1;
                    *(int *)(m + 0x58) = *(int *)(c + 4);
                    cm_SendLongMessage(m);
                }
            }
            pl = ai_PlayerCharC;
        }
        objs[n++] = s;
        s = (char *)gm_BuildNext();
    }
    if (b != pl) {
        v = gm_Unk509eb0(a);
        cm_SendMoveGoods(*(int *)(b + 4), -1, ai_OwnPlayer, v);
        v = gm_GetWorth(*(unsigned short *)a);
        cm_BeginFieldBlock(b, *(int *)(b + 4));
        ((int (*)(int, int, int))cm_AddFieldValues)(4, 1, (b - ai_Chars[0]) + 0x1a8);
        if (flag && (b[2] == 3 || b[2] == 8 || b[2] == 9))
            cm_AddFieldDelta(1, 1, (b - ai_Chars[0]) + 2, &v);
        cm_SendFieldDeltas();
        pl = ai_PlayerCharC;
    }
    if (b[2] != 6 && b[2] != 7) {
        if ((a[2] == 6 || a[2] == 7 || a[2] == 5) && b == pl) {
            for (i = 0; i < 0x300; i++) {
                char *c = ai_Chars[i];
                if ((c[2] == 6 || c[2] == 7) && *(int *)(c + 4) != *(int *)(a + 4)) {
                    txt_Format(t1, 0x1c4d, *(unsigned short *)a);
                    he_ShowNews(*(int *)(c + 4), -1, t1, 0, 0x5fe, "_NACHRICHTEN_HS_67");
                }
            }
            return;
        }
        for (i = 0; i < 0x300; i++) {
            for (j = 0; j < 8; j++) {
                char *c = ai_Chars[i];
                if ((c[2] == 6 || c[2] == 7) && *(int *)(c + 0x58 + j * 4) == *(int *)(a + 4)) {
                    txt_Format(t1, 0x1c4e, *(unsigned short *)a);
                    he_ShowNews(*(int *)(c + 4), -1, t1, 0, 0x5f4, "_NACHRICHTEN_HS_67");
                    i = 0x300;
                }
            }
        }
        return;
    }
    txt_Format(t2, 0x1c4a, *(unsigned short *)a);
    if (n != 0) {
        txt_Format(t1, 0x1c4b);
        strcat(t2, t1);
        for (i = 0; i < n; i++) {
            if (i >= 6) {
                if (i < n) {
                    txt_Format(t1, 0xb6, n - i);
                    strcat(t2, t1);
                }
                break;
            }
            if ((objs[i][0x5a] & 1) == 0)
                txt_Format(t1, "%1G$A", objs[i]);
            strcat(t2, t1);
        }
    }
    txt_Format(t1, 0x1c4c, gm_Unk509eb0(a));
    strcat(t2, t1);
    he_ShowNews(*(int *)(b + 4), -1, t2, 1, 0x5ff, "_NACHRICHTEN_HS_67");
}
/* 0x506e70, the inheritance of the dead character c (the heirs are the characters of the group (1, 3, id) i.e. buildings it owns: those of the kinds in the mask 0xe07d43b0 / the kind 0x20 go to the best claimant by 0x509b10, the others of kind 2 to everyone): messages 0x4c54a0 (hand over), the message 0x1b to the new owner and 0x31 to the tenants, the money (0x509eb0 of the dead goes to the player 0x6f1bf8), the objects of its list (+0x178) by 0x4c5010; when flag is set the character is reset to a new person (the fields +8 .. +0x1b0 are sent through 0x4c3c00 / 0x4c3d90: age 0x10, 0x100 = 100 .., health floats), and the text of the announcement is built into out from the formats 0x1c4f (name, money), "%1G %A" per heir and 0x1c50 / 0xb6 (the rest). ROUGH */

void gm_Unk506e70(unsigned short *c, char *out, int flag)
{
    unsigned char *own;
    unsigned char *best, *second;
    unsigned char *heirs[0x80];
    int n, bestv, v, i, k;
    unsigned char *b;
    unsigned mask, mask2;
    unsigned char kind;
    int *q;
    int money;
    char buf[2048];
    int msg[0x3e];
    int tmp[0x10];

    own = *(unsigned char **)(c + 0xb8);
    n = 0;
    best = 0;
    bestv = 0;
    b = (unsigned char *)gm_BuildQuery(1, 3, *c);
    while (b != 0) {
        mask = 0;
        mask2 = 0;
        kind = ai_BTypes[*b * 0x24d];
        if (kind < 0x20)
            mask = 1 << (kind & 0x1f);
        else
            mask2 = 1 << ((kind - 0x20) & 0x1f);
        if ((mask & 0xe07d43b0) == 0 && (mask2 & 1) == 0) {
            if (b != own && mask == 4) {
            take:
                heirs[n] = b;
                n++;
                second = b;
                b = best;
                goto next;
            }
        } else {
            v = gm_EquipmentValue(b, (int)(char)b[0x5c]);
            if (v <= bestv)
                goto take;
            second = 0;
            bestv = v;
            if (best != 0) {
                heirs[n] = best;
                n++;
                second = best;
            }
        next:
            best = b;
            if (second != 0) {
                cm_SendOwnership(*(int *)(second + 1), *(int *)(ai_PlayerCharC + 4), *(int *)(ai_PlayerCharC + 4));
                memset(msg, 0, sizeof(msg));
                ((char *)msg)[4] = 0x1b;
                msg[2] = *(int *)(ai_PlayerCharC + 4);
                msg[3] = -1;
                ((char *)msg)[0x36] = 2;
                msg[0x58 / 4] = *(int *)(second + 1);
                msg[0x5c / 4] = gm_EquipmentValue(second, (int)(char)second[0x5c]);
                cm_SendLongMessage(msg);
                q = (int *)(ai_Chars[0] + 4);
                do {
                    if (q[0x5a] == (int)second && *q != *(int *)(c + 2)) {
                        memset(msg, 0, sizeof(msg));
                        ((char *)msg)[4] = 0x31;
                        msg[2] = *(int *)(ai_PlayerCharC + 4);
                        msg[3] = *(int *)(second + 1);
                        msg[0x28 / 4] = *(int *)ai_Time;
                        msg[0x30 / 4] = *(int *)(ai_Time + 8);
                        msg[0x2c / 4] = *(int *)(ai_Time + 4);
                        ((char *)msg)[0x36] = 1;
                        *(short *)((char *)msg + 0x34) = *(short *)(ai_Time + 0xc);
                        msg[0x58 / 4] = *q;
                        cm_SendLongMessage(msg);
                    }
                    q += 0x87;
                } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x2));
            }
        }
        b = gm_BuildNext();
    }
    money = gm_Unk509eb0(c);
    cm_SendMoveGoods(*(int *)(ai_PlayerCharC + 4), -1, ai_OwnPlayer, money);
    k = (int)gm_Query(*(int *)(c + 0xbc), 1, 5);
    while (k != 0) {
        ai_Unk4c5010(*(int *)(c + 2), -1, *(int *)(k + 2));
        k = (int)gm_QueryNext();
    }
    cm_SendMoveGoods(*(int *)(c + 2), -1, ai_OwnPlayer, gm_PlayerFactor(0x5dc, ai_OwnPlayer));
    if (flag != 0) {
        char z2, z3;
        int age, st;

        z2 = 100;
        age = 0x10;
        st = 3;
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldDelta(1, 1, (char *)c - cm_FieldObject + 8, &z2);
        cm_AddFieldDelta(2, 1, (char *)c - cm_FieldObject + 10, &age);
        cm_AddFieldDelta(4, 1, (char *)c - cm_FieldObject + 0x194, &st);
        z3 = 0;
        cm_AddFieldDelta(1, 1, (char *)c - cm_FieldObject + 0x1b0, &z3);
        cm_AddFieldDelta(1, 1, (char *)c - cm_FieldObject + 0x1b1, &z3);
        cm_SendSetField(*(int *)(c + 2), 0x1c8, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
        cm_SendFieldDeltas();
        cm_SendFloatDelta(*(int *)(c + 2), 0x7c, -*(float *)(c + 0x3e));
        cm_SendFloatDelta(*(int *)(c + 2), 0x14, (float)(util_RandomSq() * 7.0 + 30.0 - *(float *)(c + 10)));
        cm_SendFloatDelta(*(int *)(c + 2), 0x1c, 1000.0f - *(float *)(c + 0xe));
        cm_SendFloatDelta(*(int *)(c + 2), 0x18, 1000.0f - *(float *)(c + 0xc));
        cm_SendFloatDelta(*(int *)(c + 2), 0x20, 12.0f - *(float *)(c + 0x10));
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        for (i = 0; i < 5; i++) {
            z3 = 0;
            cm_AddFieldValue(1, 1, (char *)c + i - cm_FieldObject + 0x80, &z3);
        }
        cm_SendFieldValues();
    }
    txt_Format(out, 0x1c4f, *c, gm_PlayerFactor(0x5dc, ai_OwnPlayer));
    if (best != 0) {
        txt_Format(buf, "%1G$A", best);   /* 0x61ac98: the pooled format literal */
        strcat(out, buf);
    }
    if (own != 0) {
        txt_Format(buf, "%1G$A", own);
        strcat(out, buf);
    }
    if (n != 0) {
        txt_Format(buf, 0x1c50);
        strcat(out, buf);
        i = 0;
        if (0 < n) {
            while (i < 6) {
                txt_Format(buf, "%1G$A", heirs[i]);
                strcat(out, buf);
                i++;
                if (n <= i)
                    return;
            }
            if (i < n) {
                txt_Format(buf, 0xb6, n - i);
                strcat(out, buf);
            }
        }
    }
}
/* 0x5075c0, 1 when the character c (not a player of kind 6 / 7) may be given up for the party table: like 0x4fd090 (no player has it as a direct relation or among a full set of five), then, when fewer than 0x20 character slots are free (0x300 - 0x6f1bc0), the table at 0x91f725 (records of 0x2d bytes, the id at -0x13) must not hold it as an active entry; 0 otherwise */

int gm_MayRemoveChar(char *c)
{
    int id, n, found, k;
    char *p;
    int *q;

    if (c[2] != 6 && c[2] != 7) {
        id = *(int *)(c + 4);
        for (p = ai_Chars[0] + 2; (int)p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            found = 0;
            n = 0;
            if (*(int *)(p + 2) != id && (*p == 6 || *p == 7)) {
                if (*(int *)(p + 0x5a) == id)
                    return 0;
                if (*(int *)(p + 0x5e) == id)
                    return 0;
                if (*(int *)(p + 0x62) == id)
                    return 0;
                q = (int *)(p + 0x66);
                for (k = 5; k != 0; k--, q++) {
                    if (*q == id) {
                        found = 1;
                        n++;
                    } else if (*q != -1)
                        n++;
                }
                if (found && n < 5)
                    return 0;
            }
        }
        if (0x300 - ai_CharCount < 0x20)
            return 1;
        p = (char *)((unsigned)&ai_Unk91f716 + 0xf);
        while (*(int *)(p - 0x13) != id || *(int *)p == 0) {
            p += 0x2d;
            if ((int)p > ((unsigned)((unsigned)ai_Unk91f716 + 0x5a0a) + 0x4))
                return 1;
        }
    }
    return 0;
}

extern int ai_Difficulty;
/* txt_ObjectNames: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_GameFlags = 0;  /* 0x6f23f8 */
extern float gm_DayStartHours[], gm_DayEndHours[];  /* 0x617830 / 0x617840 */
char * ahm_OwnerChar();                     /* 0x4eaa50 */
int ahm_ObjectValue();                     /* 0x4ec3d0 */
int gm_ListAmount();                     /* 0x50aaf0 */
int gm_ListStock();                     /* 0x50ab50 */
int gm_ClockSeason();                     /* 0x4fe350 */
int gm_CompareTime();                     /* 0x4fe210 */
int gv_GetTableRecord(int i, int *out);                     /* 0x4f2720 */
float gm_ItemPrice(int item, unsigned char n);
int gm_SumListAmounts(void *list, unsigned char p);

#define PLW54(k) (*(unsigned short *)(ai_PlayerSlots[(unsigned char)(k)] + 0x54))
#define PLTAB(i) (((int *)ai_Unk61a8ac)[i])
#define OPROTO(i) (ai_Goods + (unsigned)(i) * 0x41)

extern int ai_CharCount, ai_NextCharNumber, ai_NextPlayerNumber, gm_AllowAll, ai_NextObjectId;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc, 0x6f2448, 0x61a654 */

extern char ai_Unk61a8c8[];  /* 0x61a8c8 */
const float ai_Unk5ee160[476] = { 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.20000005f, 2.70000005f, 2.5f, 3.0f, 2.5f, 3.0f, 2.4000001f, 2.70000005f, 2.29999995f, 2.9000001f, 2.20000005f, 2.79999995f, 2.0f, 3.0f, 16.0f, 24.0f, 2.4000001f, 3.0f, 2.5f, 3.0f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.5f, 3.0f, 2.0f, 2.5f, 2.5f, 3.0f, 2.5f, 3.0f, 2.5f, 3.0f, 2.20000005f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.20000005f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.9000001f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.20000005f, 2.70000005f, 2.0f, 2.79999995f, 2.0999999f, 2.5999999f, 2.20000005f, 2.70000005f, 2.20000005f, 2.70000005f, 2.20000005f, 2.70000005f, 2.29999995f, 2.79999995f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 16.0f, 24.0f, 2.5f, 2.9000001f, 2.4000001f, 2.9000001f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.0f, 2.9000001f, 2.0f, 2.5999999f, 2.5999999f, 2.9000001f, 2.29999995f, 2.79999995f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.29999995f, 2.79999995f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 16.0f, 24.0f, 2.5f, 2.9000001f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 2.9000001f, 2.0999999f, 2.9000001f, 2.0999999f, 2.79999995f, 2.0999999f, 2.9000001f, 2.5f, 2.79999995f, 2.5999999f, 2.9000001f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.29999995f, 3.0f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.0f, 2.9000001f, 2.29999995f, 2.79999995f, 2.0f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.0999999f, 2.70000005f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.0f, 2.79999995f, 2.0999999f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.0999999f, 2.70000005f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.0f, 2.79999995f, 2.0999999f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 16.0f, 24.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };  /* 0x5ee160 */
void hmt_Create(int type, int unk, unsigned short v, struct AICHAR *c);                     /* 0x4adbb0 */
void ai_ResetJob(struct AICHAR *c);                     /* 0x47e940 */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */

#define CF(o, T) (*(T *)(c + (o)))

/* 0x507690, creates a character of the type `type` (the slot is the first free one, id -1; with none left, the first of type 0xf is reused) and returns its id (0xffff when full): clears the record (0x21c bytes), sets
   type / ids (+4 a running number 0x61a654) / defaults (+8 = 100, +0x1e4 = 1.0, +0x190 = 4), the kind byte +0x164 = a6 and +0x165 = a7, the age +0xa = a4, the player avatar for type 0x10 (0x4ed170: names, skills, floats
   copied from the avatar record), the home objects (+0x16c / +0x170 from the building a5 by its class), and either rolls the 14 float skills (+0x88 + 12 * i: value, 0.001 * value, 900 + rand) from the table 0x5ee160
   by the kind (0x504ad0), or inherits them from the two parents a2 / a3 (a random mix, the parents' child slots +0x5c ..) and picks names (0x143ac5c / 0x143af70 / 0x143b178, 0xf characters) and the portrait
   (+0x50). ROUGH */
unsigned short gm_CreateCharacter(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8)
{
    int idx, i, n, kind, w;
    char *c;
    unsigned short *p1;
    unsigned short *p2;
    char *av;
    float f;
    unsigned short r, rr;
    char buf[0x400];

    idx = 0;
    while (*(short *)ai_Chars[idx] != -1) {
        idx++;
        if (idx >= 0x300)
            break;
    }
    if (idx == 0x300) {
        idx = 0;
        while (ai_Chars[idx][2] != 0xf) {
            idx++;
            if (idx >= 0x300)
                break;
        }
    }
    c = ai_Chars[idx];
    if (c[2] == 0xf)
        ai_CharCount--;
    if (idx == 0x300)
        return 0xffff;
    memset(c, 0, 0x21c);
    ai_CharCount++;
    CF(4, int) = -1;
    CF(0, unsigned short) = 0xffff;
    for (i = 0; i < 8; i++)
        CF(0x5c + i * 4, int) = -1;
    CF(0, unsigned short) = (unsigned short)idx;
    c[2] = (char)type;
    CF(0x170, int) = 0;
    CF(0x16c, int) = 0;
    CF(0x1f0, int) = 0;
    CF(0x210, int) = -1;
    c[0x215] = 0;
    c[0x216] = 0;
    c[0x217] = 0;
    c[0x218] = 1;
    sprintf(buf, "-----> SpID = %i, playermode = %i", ai_NextObjectId, type);
    CF(4, int) = ai_NextObjectId;
    ai_NextObjectId++;
    c[0x164] = (char)a6;
    CF(0x18c, int) = -1;
    CF(0x20c, int) = -1;
    CF(0x1e4, float) = 1.0f;
    c[8] = 100;
    CF(0xa, unsigned short) = (unsigned short)a4;
    c[0x166] = 0;
    c[0x165] = (char)a7;
    CF(0x1f4, char) = 0;
    CF(0x190, int) = 4;
    CF(0x194, int) = 0;
    c[0xd] = 1;
    kind = c[2];
    if (kind == 3 || kind == 0x10 || kind == 0x13) {
        CF(0x1e8, int) = 1;
        c[0xd] = 2;
    }
    if (kind == 0x10) {
        av = gv_AllocAvatar();
        c[2] = 3;
        if (av != 0) {
            r = util_RandomU(8);
            CF(0x54, int) = 0x59a;
            CF(0x1f0, int) = *(int *)av;
            c[0x164] = av[0xd4];
            c[9] = av[0xd5];
            c[0xc] = av[0xd6];
            strcpy(c + 0x30, av + 4);
            strcpy(c + 0x40, av + 0x14);
            strcpy(c + 0x1f4, av + 0xd7);
            CF(0x80, int) = *(int *)(av + 0x24);
            c[0x84] = av[0x28];
            memcpy(c + 0x88, av + 0x2c, 0xa8);
            CF(0x1d0, float) = util_RandomF() * 0.5f + 0.25f;
            CF(0x18c, int) = gm_HeadIconNumber(av + 0xd7) + 0x639;
            CF(0x1c, float) = 1000.0f;
            CF(0x50, unsigned short) = (unsigned short)ai_NextCharNumber++;
            CF(0x20, float) = 12.0f;
            CF(0x14, float) = util_RandomSq() * 7.0f + 30.0f;
            rr = util_RandomU((int)CF(0x14, float));
            CF(0x18, float) = CF(0x1c, float);
            CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
            if (r < 2)
                c[0x217] = 0x58;
            else if (r < 4)
                c[0x217] = 0x5c;
            c[0x217] = (char)((((r > 5) - 1) & 0x86) + 0x3c);
            if (CF(0x20, float) < (float)CF(0xa, unsigned short)) {
                n = (int)CF(0x14, float);
                f = CF(0x20, float);
                for (; n > 0; n--)
                    f = (f - CF(0x14, float)) + CF(0x14, float) / f;
                if (f <= 1.0f)
                    f = 1.0f;
                CF(0x1c, float) = f;
                return CF(0, unsigned short);
            }
            return CF(0, unsigned short);
        }
    }
    if (c[2] == 0x15 || c[2] == 0x16) {
        hmt_Create(c[2], a2, a4, c);
        return CF(0, unsigned short);
    }
    if (a5 != 0) {
        kind = (unsigned char)ai_BTypes[a5[0] * 0x24d];
        if (kind == 1)
            CF(0x170, unsigned char *) = a5;
        else if (kind > 3 && kind < 0x17 && kind != 10 && kind != 0xf && kind != 0x11)
            CF(0x16c, unsigned char *) = a5;
    }
    c[0xd] = (c[0x166] != 0) + 1;
    if (type != 6 && type != 7 && type < 10)
        c[0x1b0] = (char)util_RandomU(8);
    if (a8 == 2)
        a8 = util_RandomU(2);
    c[9] = (char)a8;
    if (type < 4 || type == 0xb) {
        if (a7 == 0) {
            if (a6 == 0)
                goto skip;
            w = gm_SexForJob(a6);
        } else
            w = gm_SexForTitle(a7);
        if (w != 2)
            c[9] = (char)w;
    }
skip:
    CF(0x54, int) = 0x59a;
    if (gm_AllowAll != 0) {
        CF(0x194, int) = 10;
        CF(0x190, int) = 10;
    }
    p1 = (unsigned short *)gm_FindChar(a2);
    p2 = (unsigned short *)gm_FindChar(a3);
    if (p1 == 0 || p2 == 0) {
        c[0x164] = (char)a6;
        c[0x166] = 0;
        c[0x165] = (char)a7;
        w = gm_TitleGroup(a6 & 0xff);
        r = util_RandomU(8);
        if (c[2] == 3) {
            if (w == 0xb || w == 0xc) {
                if (r > 2)
                    c[0x217] = (char)(((r > 5) - 1 & 0x9a) - 0x3e);
                else
                    c[0x217] = 0x58;
            } else if (r < 3)
                c[0x217] = 0x3c;
            else
                c[0x217] = (char)((r < 6) * 4 + 0x58);
        } else if (w == 0xb || w == 0xc)
            c[0x217] = (char)((r > 3) * 8 + 0x4e);
        else if (r < 3)
            c[0x217] = 0x3e;
        else
            c[0x217] = (char)((r < 6) * 8 + 0x4e);
        for (i = 0; i < 14; i++) {
            float lo = ai_Unk5ee160[w * 0x1c + i * 2], hi = ai_Unk5ee160[w * 0x1c + i * 2 + 1];
            CF(0x88 + i * 12, float) = (hi - lo) * util_RandomF() + lo;
            rr = util_RandomU(10);
            CF(0x90 + i * 12, float) = (float)rr + 900.0f;
            CF(0x8c + i * 12, float) = CF(0x88 + i * 12, float) * 0.001f;
        }
        f = (util_RandomF() + 2.0f) * 8.0f;
        CF(0x124, float) = f;
        CF(0x128, float) = f * 0.001f;
        for (i = 0; i < 0x300; i++) {
            ai_RelationMemory[i][CF(0, unsigned short)] = 0;
            ai_Relations[i][CF(0, unsigned short)] = (char)(util_RandomU(0x40) - 0x20);
            ai_Relations[CF(0, unsigned short)][i] = (char)(util_RandomU(0x40) - 0x20);
            ai_RelationMemory[CF(0, unsigned short)][i] = 0;
        }
        c[0xc] = (char)util_RandomU(2);
        CF(0x1d0, float) = util_RandomF() * 0.5f + 0.25f;
        for (i = 0; i < 5; i++)
            c[0x80 + i] = (char)gm_RandomAge(ai_Unk61a688[(unsigned char)c[0x164] * 6 + i]);
        if (type != 6 && type != 7)
            c[0xd] = ai_Unk61a688[(unsigned char)c[0x164] * 6 + 4];
        for (i = 0; i < 8; i++)
            CF(0x5c + i * 4, int) = -1;
        CF(0x64, int) = a3;
        c[0x58] = 1;
        if (type == 6 || type == 7 || type == 5) {
            if (ai_NextPlayerNumber > 0xf)
                return 0xffff;
            CF(0x50, unsigned short) = (unsigned short)ai_NextPlayerNumber | 0x8000;
            ai_NextPlayerNumber++;
        } else if (type < 10)
            CF(0x50, unsigned short) = (unsigned short)ai_NextCharNumber++;
        else
            CF(0x50, unsigned short) = 0x7fff;
        CF(0x1c, float) = 1000.0f;
        CF(0x20, float) = 12.0f;
        CF(0x14, float) = util_RandomSq() * 7.0f + 30.0f;
        rr = util_RandomU((int)CF(0x14, float));
        CF(0x18, float) = CF(0x1c, float);
        CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
        strncpy(c + 0x40, txt_FamilyNames[util_RandomU(0x95)], 0xf);
    } else {
        c[0xc] = (char)p1[6];
        CF(0x1d0, float) = *(float *)(p1 + 0xe8);
        c[0x217] = *((char *)p1 + 0x217);
        for (i = 0; i < 14; i++) {
            f = util_RandomF();
            CF(0x88 + i * 12, float) = f * *(float *)((char *)p1 + 0x88 + i * 12) + (1.0f - f) * *(float *)((char *)p2 + 0x88 + i * 12);
            rr = util_RandomU(10);
            CF(0x90 + i * 12, float) = (float)rr + 900.0f;
            CF(0x8c + i * 12, float) = CF(0x88 + i * 12, float) * 0.001f;
        }
        f = (util_RandomF() + 2.0f) * 8.0f;
        CF(0x124, float) = f;
        CF(0x128, float) = f * 0.001f;
        CF(0x60, int) = a2;
        CF(0x64, int) = a3;
        CF(0x68, int) = -1;
        CF(0x6c, int) = -1;
        CF(0x70, int) = -1;
        CF(0x74, int) = -1;
        CF(0x78, int) = -1;
        CF(0x7c, int) = 0;
        CF(0x50, unsigned short) = p1[0x28];
        c[0x58] = (char)p1[0x2c] + 1;
        CF(0x54, int) = *(int *)(p1 + 0x2a);
        CF(0x1c, float) = 1000.0f;
        CF(0x20, float) = 12.0f;
        CF(0x14, float) = (*(float *)(p2 + 10) + *(float *)(p1 + 10)) * 0.2f + (util_RandomSq() * 7.0f + 30.0f) * 0.6f;
        rr = util_RandomU((int)CF(0x14, float));
        CF(0x18, float) = CF(0x1c, float);
        CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
        strcpy(c + 0x40, (char *)(p1 + 0x20));
    }
    if (c[9] == 0)
        strncpy(c + 0x30, txt_MaleFirstNames[util_RandomU(0xbf)], 0xf);
    else
        strncpy(c + 0x30, txt_FemaleFirstNames[util_RandomU(0x70)], 0xf);
    if (CF(0x20, float) < (float)CF(0xa, unsigned short)) {
        n = (int)CF(0x14, float);
        f = CF(0x20, float);
        for (; n > 0; n--)
            f = (f - CF(0x14, float)) + CF(0x14, float) / f;
        if (f <= 1.0f)
            f = 1.0f;
        CF(0x1c, float) = f;
    }
    gm_ChooseHeadIcon(c);
    CF(0x1b4, int) = 0;
    CF(0x1b8, int) = 0;
    CF(0x1bc, int) = 0;
    CF(0x1c0, int) = 0;
    ai_ResetJob(c);
    kind = c[2];
    if (kind == 3 || kind == 4 || kind == 2 || kind == 0x13)
        c[0x82 + (util_RandomU(3) & 0xffff)] += 0x2a;
    sprintf(buf, "-----> PlayerID = %i, PlayerName = %s", ai_NextObjectId, c + 0x30);
    return (unsigned short)idx;
}
/* 0x5085b0, a / the divisor of the player slot k, rounded */

int gm_ConvertMoney(int a, unsigned char k)
{
    return (int)((double)a / PLTAB(PLW54(k)) + 0.5);
}
/* 0x5085f0, a * the factor of the player slot k */

int gm_PlayerFactor(int a, unsigned char k)
{
    return PLTAB(PLW54(k)) * a;
}
/* 0x508620 */

int gm_Unk508620(int a, int i)
{
    return PLTAB(i) * a;
}
/* 0x508640 */

int gm_Unk508640(int a, int i)
{
    return a / PLTAB(i);
}
/* 0x508660, the sum of the amounts of the objects of the list (prototype kind 9, group 1) whose prototype word is the one of the player slot p */

int gm_SumSlotAmounts(void *list, unsigned char p)
{
    int s;
    unsigned short *o;

    s = 0;
    o = (unsigned short *)gm_Query(list, 1, 4, 9);
    if (o) {
        do {
            if (PLW54(p) == *o)
                s += *(int *)(o + 7);
            o = (unsigned short *)gm_QueryNext();
        } while (o);
    }
    return s;
}
/* 0x5086b0, the value of the object n: the base values of its building (two bytes at +0x233) times its amount per unit. ROUGH */

float gm_ObjectValue(int n)
{
    float f;
    int i;
    char *p;

    f = 0.0f;
    i = 0;
    do {
        f += (unsigned char)(ai_BTypes + gm_GoodBuilding[n] * 0x24d)[0x233 + i] * 896.0f * 0.0013888888888888887;
        i++;
    } while (i < 2);
    p = OPROTO(n);
    return f * *(int *)(p + 0x22) / *(unsigned short *)(p + 0x36);
}
/* 0x508740, the wage of a job (a) or title (b) of the building kind k for the character c, reduced for the kinds 6 / 7 */

float gm_JobWage(char *c, int k, int a, int b)
{
    float f;
    char *p;

    f = 0.0f;
    p = ai_BTypes + k * 0x24d;
    if (a == -1) {
        if (b != -1)
            f = (unsigned char)p[0x233 + b] * 0x380;
    } else
        f = (unsigned char)p[0x229 + a] * 0x380;
    if (c[2] == 6 || c[2] == 7)
        f = (1.0 - (2 - ai_Difficulty) * 0.25) * f;
    return f;
}
/* 0x5087f0, the price of the object item for n pieces: the prototype's base value (4.0 without a building, else the building's two values) times the amount per unit (+0x22 / +0x36), the sub-objects (four words at +0x2e, weights at +0x26) added; cached in the prototype word +0x38 (value / 32). ROUGH */

float gm_ItemPrice(int item, unsigned char n)
{
    char *p;
    float s, f;
    int i;
    unsigned short *w;
    char buf[0x100];
    unsigned char *bp;

    p = OPROTO(item);
    if (*(int *)(p + 0x38) == 0) {
        if (gm_GoodBuilding[item] == 0)
            f = 4.0f;
        else {
            bp = (unsigned char *)(ai_BTypes + gm_GoodBuilding[item] * 0x24d);
            f = 0.0f;
            i = 0;
            do {
                f += bp[0x233 + i] * 896.0f * 0.0013888888888888887;
                i++;
            } while (i < 2);
        }
        s = (float)(f * 0.5 * ((double)*(unsigned *)(p + 0x22) / ((*(unsigned short *)(p + 0x36) == 0) + *(unsigned short *)(p + 0x36))));
        if (*p == 0x17)
            return s * 2.2f;
        i = 0;
        w = (unsigned short *)(p + 0x2e);
        do {
            if (*w == 0)
                break;
            if (*w == 0xffff)
                sprintf(buf, "eval_ObjektPreis2Credits:Unterobjekt f\xfcr '%s' nicht vorhanden!!!", (char *)txt_ObjectNames[item][0]);
            else
                s = (float)(gm_ItemPrice(*w, n) * w[-4] / *(unsigned short *)(p + 0x36) + s);
            i++;
            w++;
        } while (i < 4);
        *(int *)(p + 0x38) = (int)(s * 0.06875f);
        return n * s * 0.01f * 2.2f;
    }
    f = (*(int *)(p + 0x38) << 5) * n * 0.01f;
    if (p[0x21] == 3)
        f *= 1.5f;
    return f;
}
/* 0x5089c0, the price of the goods id for the player slot p: the goods record's value (+0x38) or the base price */

float gm_GoodsPrice(unsigned id, unsigned char p)
{
    int i;
    unsigned short *r;

    i = 0;
    r = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*r == id)
            return *(float *)(init_StockBlocks + 0x38 + p * 0x2a90 + i * 0x80);
        i++;
        r += 0x40;
    } while (i < 0x55);
    return gm_ItemPrice(id, 100);
}
/* 0x508a20, a health factor of the character c: 1 + hp / 1008 above 42 */

float gm_HealthFactor(int c)
{
    float f;
    unsigned char *p;

    p = (unsigned char *)ai_Chars + c * 0x21c;
    if (p == 0)
        return 1.0f;
    f = p[0x80];
    if (f <= 42.0f)
        return 1.0f;
    return f * 0.000992063492063492 + 1.0;
}
/* 0x508a80, formats the amount v for the player slot k into out: "%i%c" below 1000, else with a '.' every three digits ("%s%c", the unit char 0x11 at the end), "0%c" for zero, a '-' in front for negative values. ROUGH */

void gm_FormatAmount(int v, unsigned char k, char *out)
{
    int n, i, j, e;
    char a[0x40], b[0x80];
    char *q;

    n = gm_ConvertMoney(v < 0 ? -v : v, k);
    if (v < 0) {
        if (n != 0) {
            if (n < 1000) {
                sprintf(out, "-%i%c", n, 0x11);
                return;
            }
            _itoa(n, a, 10);
            e = (int)((strlen(a) - 1) / 3.0f) + strlen(a);
            j = -1;
            i = strlen(a) - 1;
            if (i >= 0) {
                q = b + e;
                do {
                    *q = a[i];
                    q--;
                    if (i != 0 && j > 0 && (j + 1) % 3 == 0)
                        *q-- = '.';
                    j++;
                    i--;
                } while (i >= 0);
            }
            b[e] = 0;
            sprintf(out, "-%s%c", b, 0x11);
            return;
        }
    } else {
        if (n > 999) {
            _itoa(n, a, 10);
            e = (int)((strlen(a) - 1) / 3.0f) + strlen(a);
            j = -1;
            i = strlen(a) - 1;
            if (i >= 0) {
                q = b + e;
                do {
                    *q = a[i];
                    q--;
                    if (i != 0 && j > 0 && (j + 1) % 3 == 0)
                        *q-- = '.';
                    j++;
                    i--;
                } while (i >= 0);
            }
            b[e] = 0;
            sprintf(out, "%s%c", b, 0x11);
            return;
        }
        if (n != 0) {
            sprintf(out, "%i%c", n, 0x11);
            return;
        }
    }
    sprintf(out, "0%c", 0x11);
}
/* 0x508cc0, the objects a building b (kind b[0]) lists after the object word *proto in its object words (+0x23): counts them up to the first kind 2 / 6 object (kind 0x21 not counted), stores their ids at out + 4 * n */

int gm_ListObjectsAfter(unsigned char *b, unsigned short *proto, int *out)
{
    char *bp;
    int n, i, cnt;
    unsigned short *w;
    unsigned id;
    char k;

    bp = ai_BTypes + *b * 0x24d;
    n = 0;
    i = 0;
    cnt = (unsigned char)bp[0x22];
    if (cnt != 0) {
        w = (unsigned short *)(bp + 0x23);
        do {
            if ((*w & 0x7fff) == *proto)
                break;
            i++;
            w++;
        } while (i < cnt);
    }
    if (cnt <= i)
        return 0;
    i++;
    if (i < 0x40) {
        w = (unsigned short *)(bp + 0x23 + i * 2);
        while (*w != 0) {
            id = *w & 0x7fff;
            k = ai_Goods[id * 0x41];
            if (k == 2)
                return n;
            if (k == 6)
                return n;
            if (k != 0x21)
                n++;
            if (out)
                out[n] = id;
            i++;
            w++;
            if (i > 0x3f)
                return n;
        }
    }
    return n;
}
/* 0x508d70 */

int gm_BuildingLevel(char *c)
{
    return c[0x5c];
}
/* 0x508d80, the income / cost summary (0x15 ints, out[3] wages, out[0x10] cost, out[0x11] income, out[0x14] profit) of the building b for the owner id; 0 when both are zero. ROUGH */

int gm_BuildingBalance(unsigned char *b, unsigned id, int *out)
{
    char *own;
    char *bp;
    unsigned char *c;
    int i, k, w, in, t, r, u, n, found;
    int *pi;
    char *o;

    for (i = 0; i < 0x15; i++)
        out[i] = 0;
    w = 0;
    in = 0;
    bp = ai_BTypes + *b * 0x24d;
    t = 0;
    if (id == *(unsigned short *)(b + 0x27)) {
        own = ai_Chars[id];
        if (*bp == 0x1e) {
            n = w;
            if (ahm_OwnerChar(b) != 0) {
                n = ahm_ObjectValue(b);
                out[6] = n;
            }
            w = n;
        } else {
            c = (unsigned char *)ai_Chars[0];
            do {
                if (*(unsigned char **)(c + 0x16c) == b) {
                    found = 0;
                    k = 0;
                    pi = out + 6;
                    do {
                        if (bp[k + 0x223] != 0 && bp[k + 0x223] == (char)c[0x164] && (char)c[2] != 0x14) {
                            r = (int)gm_JobWage(own, *b, k, -1);
                            found = 1;
                            in += r;
                            *pi += r;
                        }
                        k++;
                        pi++;
                    } while (k < 6);
                    k = 0;
                    pi = out + 4;
                    do {
                        if (bp[k + 0x22f] != 0 && bp[k + 0x22f] == (char)c[0x165] && (char)c[2] != 0x14) {
                            r = (int)gm_JobWage(own, *b, -1, k);
                            found = 1;
                            in += r;
                            *pi += r;
                        }
                        k++;
                        pi++;
                    } while (k < 2);
                    if (!found && (char)c[2] != 0x14) {
                        r = (int)gm_JobWage(own, *b, -1, 0);
                        in += r;
                        out[4] += r;
                    }
                }
                c += 0x21c;
                w = in;
            } while ((int)c < (unsigned)((unsigned)ai_Chars + 0x65400));
        }
        out[3] = w;
        out[0x12] = *(int *)(b + 0x4d);
        if (*bp == 5) {
            out[10] = *(int *)(b + 0x55);
            t = *(int *)(b + 0x55);
        }
        if (*bp == 7) {
            o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0xe8);
            if (o) {
                r = gm_SumListAmounts(*(void **)(o + 0x14), ai_OwnPlayer);
                u = out[0xc];
                out[0xc] = u + r;
                t = t + u + r;
            }
            o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0xf0);
            if (o) {
                r = gm_SumListAmounts(*(void **)(o + 0x14), ai_OwnPlayer);
                u = out[0xf];
                out[0xf] = u + r;
                t = t + u + r;
            }
            k = *(int *)(b + 0x65);
            r = out[0xd];
            u = out[0xe];
            out[0xd] = r + k;
            i = *(int *)(b + 0x69);
            out[0xe] = u + i;
            t = t + r + k + u + i;
        } else if (*bp == 0x16) {
            t += *(int *)(b + 0x6d);
            out[0x13] = *(int *)(b + 0x6d);
        }
    }
    out[0x10] = w;
    out[0x11] = t;
    out[0x14] = t - w;
    if (w == 0 && t == 0)
        return 0;
    return 1;
}
/* 0x509060, up to three town objects (query group 2: kinds 7, 4 = 0x1d) whose owner word (+0x1c) is the id of the character c; returns the count and stores them in out */

int gm_Unk509060(char *c, int *out)
{
    int n;
    char *o;

    n = 0;
    o = gm_Query(0, 2, 7, 4, 0x1d);
    while (o && n < 3) {
        if (*(int *)(o + 0x1c) == *(int *)(c + 1)) {
            if (out)
                out[n] = (int)o;
            n++;
        }
        o = (char *)gm_QueryNext();
    }
    return n;
}
/* 0x5090b0, a rank byte (+0x247) of the building b */

int gm_BuildingRank(unsigned char *b)
{
    char *p;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    switch (*p) {
    case 2:
    case 5:
    case 7:
        return (unsigned char)p[0x247];
    default:
        return (unsigned char)p[0x247];
    }
}
/* 0x509120, 1 when the rank of c is below the limit of the building b (rank 1: < 2, 2: < 3, 3: <= 7, else <= 0) */

int gm_Unk509120(unsigned char *b, void *c)
{
    char *p;
    int r, lim;

    lim = 0;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    r = gm_BuildingRank(c);
    if (p[0x247] == 1)
        return r < 2;
    if (p[0x247] == 2)
        return r < 3;
    if (p[0x247] == 3)
        lim = 7;
    return r <= lim;
}
/* 0x5091a0, the value of the building b: out[0] the price of its owner's estate (0x50a640 of the owner capped at 2048000, 7 % for the kind 2 else 3 %), out[1] the sum of the prices of the objects of its first kind 0x2a object. ROUGH */

int gm_Unk5091a0(unsigned char *b, int *out)
{
    float f, tot;
    char *p, *l, *o;
    int v;
    unsigned short id;

    f = 0.0f;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    id = *(unsigned short *)(b + 0x27);
    if (id != 0xffff) {
        f = I50A640(id);
        if (f > 2048000.0f)
            f = 2048000.0f;
    }
    if (*p == 2)
        f *= 0.07f;
    else
        f *= 0.03f;
    if (f < 0.0f)
        f = 0.0f;
    out[0] = (int)f;
    tot = 0.0f;
    l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l) {
        o = gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)(gm_ListAmount(l, o) * 0.3);
            if (v < 1)
                v = 1;
            tot = gm_ItemPrice(*(unsigned short *)o, 100) * v + tot;
            o = (char *)gm_QueryNext();
        }
    }
    out[1] = (int)tot;
    return 0;
}

/* 0x509300, in the building's object words (+0x23) the objects with the class byte (+0x1a3) k that exist in the building's list: the sum of their bytes at +0x1e3; with flag the sum is scaled by the factor of 0x508d70 (0.01) */
/* 0x5092e0 */
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag)
{
    char *p;
    int i, s;
    unsigned short *w;

    i = 0;
    s = 0;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    w = (unsigned short *)(p + 0x23);
    do {
        if ((*w & 0x7fff) != 0 && (unsigned char)p[i + 0x1a3] == k) {
            if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, *w & 0x7fff))
                s += (unsigned char)p[i + 0x1e3];
        }
        i++;
        w++;
    } while (i < 0x40);
    if (flag)
        return (int)(gm_BuildingLevel((char *)b) * 0.01 * s + 0.5);
    return s;
}
/* 0x5093a0, the same sum over the building kind p without asking for the instance */

int gm_Unk5093a0(int kind, unsigned k)
{
    char *p;
    int i, s;
    unsigned short *w;

    i = 0;
    p = ai_BTypes + kind * 0x24d;
    s = 0;
    w = (unsigned short *)(p + 0x23);
    do {
        if ((*w & 0x7fff) != 0 && (unsigned char)p[s + 0x1a3] == k)
            i += (unsigned char)p[s + 0x1e3];
        s++;
        w++;
    } while (s < 0x40);
    return i;
}

#define GM_CLAMPMIN(m, lo, hi) ((float)(m) < (lo) ? (lo) : ((float)(m) > (hi) ? (hi) : (float)(m)))

/* 0x509400, the working minutes between the clocks a and b of the building b (the day runs from the season's start to its end: 0x617830 / 0x617840 hours); the whole day (0x4fe280) for the night shift (0x6f23f8 < 0) and the kind 0x1d. ROUGH */
int gm_Unk509400(GCLOCK *a, GCLOCK *b, unsigned char *bld)
{
    float lo, hi, m, c;
    int s, acc, x, y, d0, d1, n;
    GCLOCK t;

    s = gm_ClockSeason(a);
    lo = gm_DayStartHours[s];
    hi = gm_DayEndHours[s];
    if ((char)ai_GameFlags < 0)
        return gm_MinutesBetween(a, b);
    if (bld != 0 && ai_BTypes[*bld * 0x24d] == 0x1d)
        return gm_MinutesBetween(a, b);
    if (gm_CompareTime(a, b) > 0)
        return 0;
    t = *a;
    d0 = a->day;
    d1 = b->day;
    if (d0 < d1) {
        m = (float)(t.hour * 60 + t.min);
        c = GM_CLAMPMIN(m, lo * 60.0f, hi * 60.0f);
        x = (int)c;
        acc = (int)(hi * 60.0f - x);
        t.day = d0 + 1;
        gm_SetTime(&t, (int)lo, 0, 0);
        d0 = t.day;
    } else
        acc = 0;
    if (d0 < d1) {
        n = d1 - d0;
        do {
            acc = (int)(acc + (hi - lo) * 60.0f);
            n--;
        } while (n != 0);
    }
    m = (float)(t.hour * 60 + t.min);
    y = (int)GM_CLAMPMIN((float)(b->hour * 60 + b->min), lo * 60.0f, hi * 60.0f);
    x = (int)GM_CLAMPMIN(m, lo * 60.0f, hi * 60.0f);
    return (acc - x) + y;
}
/* 0x5096a0, the same with the fixed day 360 .. 1380 minutes (6:00 to 23:00, 1020 a day) */

int gm_Unk5096a0(GCLOCK *a, GCLOCK *b)
{
    float m;
    int acc, x, y, d0, d1, n;
    GCLOCK t;

    if (gm_CompareTime(a, b) > 0)
        return 0;
    t = *a;
    d0 = a->day;
    d1 = b->day;
    if (d0 < d1) {
        m = (float)(t.hour * 60 + t.min);
        x = (int)GM_CLAMPMIN(m, 360.0f, 1380.0f);
        acc = (int)(1380.0f - x);
        t.day = d0 + 1;
        gm_SetTime(&t, 6, 0, 0);
        d0 = t.day;
    } else
        acc = 0;
    if (d0 < d1) {
        n = d1 - d0;
        do {
            acc = (int)(acc + 1020.0f);
            n--;
        } while (n != 0);
    }
    y = (int)GM_CLAMPMIN((float)(b->hour * 60 + b->min), 360.0f, 1380.0f);
    m = (float)(t.hour * 60 + t.min);
    x = (int)GM_CLAMPMIN(m, 360.0f, 1380.0f);
    return (acc - x) + y;
}
/* 0x5098a0, the goods list of the building b: out (0x2e ints) gets the max count (out[0]), the number of entries (out[1]), per entry the goods id (words at +0x48), its stock (+8 ints: 0x50ab50) and its price (+0x68: 0x50aaf0); out[0x2d] the building's market object. kind 0: the objects that are not 0x17 / 0x25; else those. 0 when it has no such object (0xfd). */

int gm_BuildingGoodsList(char *b, unsigned *out, int kind)
{
    char k;
    short *l;
    unsigned short *o;
    unsigned v;
    int i;

    for (i = 0; i < 0x2e; i++)
        out[i] = 0;
    if (gm_Query(*(void **)(b + 0x5d), 1, 0, 0xfd) == 0)
        return 0;
    l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l == 0 && (l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x116)) == 0)
        return 0;
    if (kind == 0) {
        v = *(unsigned char *)((char *)l + 0x1d);
        out[0] = v;
        if (*l == 0x116) {
            if (v <= *(unsigned char *)(l + 0xe))
                v = *(unsigned char *)(l + 0xe);
            out[0] = v;
        }
    } else {
        if (*l == 0x116)
            return 0;
        v = *(unsigned char *)(l + 0xe);
        out[0] = v;
    }
    o = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 5);
    while (o) {
        k = ai_Goods[*o * 0x41];
        if ((((k == 0x17 || k == 0x25) && kind != 0) || ((k != 0x17 && k != 0x25) && kind == 0)) || *l == 0x116) {
            v = gm_ListAmount(l, o);
            out[out[1] + 0x1a] = v;
            v = gm_ListStock(l, *o);
            out[out[1] + 2] = v;
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
        }
        o = (unsigned short *)gm_QueryNext();
    }
    out[0x2d] = (unsigned)l;
    return 1;
}
/* 0x5099e0, the same list with the other loop shape (the kinds 0x17 only when kind != 0, the others only when kind == 0) */

int gm_Unk5099e0(char *b, unsigned *out, int kind)
{
    short *l;
    unsigned short *o;
    unsigned v;
    int i;

    for (i = 0; i < 0x2e; i++)
        out[i] = 0;
    if (gm_Query(*(void **)(b + 0x5d), 1, 0, 0xfd) == 0)
        return 0;
    l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l == 0 && (l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x116)) == 0)
        return 0;
    if (kind == 0) {
        v = *(unsigned char *)((char *)l + 0x1d);
        out[0] = v;
        if (*l == 0x116) {
            if (v <= *(unsigned char *)(l + 0xe))
                v = *(unsigned char *)(l + 0xe);
            out[0] = v;
        }
    } else {
        if (*l == 0x116)
            return 0;
        v = *(unsigned char *)(l + 0xe);
        out[0] = v;
    }
    o = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 5);
    do {
        if (o == 0) {
            out[0x2d] = (unsigned)l;
            return 1;
        }
        if (ai_Goods[*o * 0x41] == 0x17) {
            if (kind != 0) {
                v = gm_ListAmount(l, o);
                out[out[1] + 0x1a] = v;
                v = gm_ListStock(l, *o);
                out[out[1] + 2] = v;
                *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
                out[1]++;
            }
        } else if (kind == 0) {
            v = gm_ListAmount(l, o);
            out[out[1] + 0x1a] = v;
            v = gm_ListStock(l, *o);
            out[out[1] + 2] = v;
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
        }
        o = (unsigned short *)gm_QueryNext();
    } while (1);
}
/* 0x509b10, walks the building b's object words from +0x25: for a kind 2 / 6 object found in the instance, the price (0x5087f0) of the next one that is not 2 / 6 is added up. ROUGH */

int gm_EquipmentValue(unsigned char *b, int unused)
{
    char k;
    int found, i;
    unsigned id;
    unsigned short *l, *o, *w;
    float tot;

    l = 0;
    found = 0;
    i = 0;
    tot = 0.0f;
    w = (unsigned short *)(ai_BTypes + (unsigned)*b * 0x24d + 0x25);
    do {
        if (w[-1] == 0)
            break;
        id = w[-1] & 0x7fff;
        if (found) {
            if (l != 0) {
                o = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 0, id);
                if (o != 0)
                    tot += gm_ItemPrice(*o, *(unsigned char *)(o + 9));
                if (i < 0x3f && ((k = ai_Goods[(*w & 0x7fff) * 0x41]) == 2 || k == 6))
                    found = 0;
            }
        } else {
            k = ai_Goods[id * 0x41];
            if (k == 2 || k == 6) {
                l = (unsigned short *)gm_Query(*(void **)(b + 0x5d), 1, 0, id);
                if (l != 0) {
                    tot += gm_ItemPrice(*l, *(unsigned char *)(l + 9));
                    if (i < 0x3f && (k = ai_Goods[(*w & 0x7fff) * 0x41]) != 2 && k != 6)
                        found = 1;
                }
            }
        }
        i++;
        w++;
    } while (i < 0x40);
    return (int)tot;
}
/* 0x509c80, the worth of the building b's owner: the value of the character's objects (0x509e60) for a person, plus (with flag) the prices of the building's first kind 0x2a object's goods (0x5089c0 for the player 0x6f1bc5) */

int gm_OwnerWorth(char *b, int flag)
{
    unsigned short id;
    int v;
    char *l;
    unsigned short *o;

    v = 0;
    id = *(unsigned short *)(b + 0x27);
    if (id != 0xffff && (unsigned char)ai_Chars[id][2] < 10)
        v = gm_GetGoodsValue(ai_Chars[id]);
    if (flag != 0 && (l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a)) != 0) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)gm_GoodsPrice(*o, ai_OwnPlayer);
            o = (unsigned short *)gm_QueryNext();
        }
    }
    return v;
}
/* 0x509d30, the sum of the prices of the building kind's negative object words (+0x23, high bit set) */

int gm_BuildingKindValue(int kind)
{
    unsigned short w;
    unsigned short *p;
    int i;
    float tot;

    tot = 0.0f;
    p = (unsigned short *)(ai_BTypes + kind * 0x24d + 0x23);
    i = 0;
    do {
        w = *p;
        if (w == 0)
            break;
        if ((short)w < 0)
            tot += gm_ItemPrice((unsigned char)ai_Goods[(w & 0x7fff) * 0x41], 100);
        i++;
        p++;
    } while (i < 0x40);
    return (int)tot;
}
/* 0x509dc0 */

int gm_BuildingKindCost(int unused, int kind)
{
    char buf[0x24];
    int v;

    v = gm_BuildingKindValue(kind);
    gv_GetTableRecord(6, buf);
    return v;
}
/* 0x509e30, the sum of the amounts of the objects in the list (prototype kind 9, group 1) */

int gm_SumListAmounts(void *list, unsigned char p)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(list, 1, 4, 9);
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}
/* 0x509e60, the amount of the character c's objects of the prototype word of the player 0x6f1bc5 */

int gm_GetGoodsValue(char *c)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 0, PLW54(ai_OwnPlayer));
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}
/* 0x509eb0 */

int gm_Unk509eb0(char *c)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 4, 9);
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}
/* 0x509ee0, the amount of the character c's object of the prototype word of the player p */

int gm_CharCurrency(char *c, char p)
{
    char *o;

    o = gm_Query(*(void **)(c + 0x178), 1, 0, PLW54(p));
    if (o)
        return *(int *)(o + 0xe);
    return 0;
}

/* ---- layout 2026-10-23: the character / building worth helpers 0x509f20 .. 0x50af00 (ROUGH drafts) ---- */
char * ai_FindWeaponRecord(int id);                     /* 0x4cd9d0 */
int gm_EquipmentValue(unsigned char *b, int unused);
int gm_GetWorth(unsigned short id);
int gm_ListAmount(short *l, char *o);

/* 0x509f20, the amount of the objects (prototype kind 9) of the character with the id of slot c, or -1 when it does not exist */
int gm_Unk509f20(int c)
{
    char *p;
    int s;
    char *o;

    p = gm_FindChar(*(int *)(ai_Chars[c] + 4));
    if (p != 0) {
        s = 0;
        o = gm_Query(*(void **)(p + 0x178), 1, 4, 9);
        while (o) {
            s += *(int *)(o + 0xe);
            o = (char *)gm_QueryNext();
        }
        return s;
    }
    return -1;
}
/* 0x509f80, the price of the goods of the building b's first kind 0x2a object (the last one counts) */

int gm_Unk509f80(char *b)
{
    int v;
    char *l;
    unsigned short *o;

    v = 0;
    l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)gm_ItemPrice(*o, *(unsigned char *)(o + 9));
            o = (unsigned short *)gm_QueryNext();
        }
    }
    return v;
}
/* 0x509ff0, the character that lives in the building the object o is in (word +0x25 of the building), or o's own character */

unsigned short *gm_ObjectOwnerChar(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o == 0) {
        if (b != 0 && *(unsigned short *)(b + 0x25) != 0xffff)
            return (unsigned short *)ai_Chars[*(unsigned short *)(b + 0x25)];
        o = 0;
    }
    return o;
}
/* 0x50a050, the same with the owner word (+0x27) */

unsigned short *gm_ObjectBuildingOwner(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o == 0) {
        if (b != 0 && *(unsigned short *)(b + 0x27) != 0xffff)
            return (unsigned short *)ai_Chars[*(unsigned short *)(b + 0x27)];
        o = 0;
    }
    return o;
}
/* 0x50a0b0, the character number of the owner of the object o */

unsigned short gm_ObjectOwnerNo(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o != 0)
        return *o;
    if (b != 0)
        return *(unsigned short *)(b + 0x27);
    return 0xffff;
}
/* 0x50a0f0, walks up the owner chain of the object o (the id at +6) to the first character */

int gm_Unk50a0f0(int o)
{
    int r, id, bld, chr;

    id = *(int *)(o + 6);
    while (1) {
        if (id == -1 || (r = gm_FindById(&o, &bld, &chr, id)) == 0)
            return 0;
        if (o != 0)
            return o;
        if (chr != 0)
            break;
        if (bld != 0)
            id = *(int *)(bld + 6);
        chr = 0;
        o = 0;
    }
    return 0;
}
/* 0x50a150, the (up to 9) group (1, 0, 0x23) items of the character slot c (the owner id at +0xb0), optionally of the party p (+0xac) */

int gm_Unk50a150(int c, int p, int *out)
{
    int n;
    char *it;

    n = 0;
    it = (char *)he_FindEvent(1, 0, 0x23);
    while (it != 0 && n < 9) {
        if (*(int *)(it + 0xb0) == *(int *)(ai_Chars[c] + 4) && (p == 0 || *(int *)(it + 0xac) == *(int *)(p + 1))) {
            out[n] = (int)it;
            n++;
        }
        it = (char *)he_NextEvent();
    }
    return n;
}
/* 0x50a1c0, the same for the buildings of the owner o (+0x27 = o), optionally one building b and one character c */

int gm_Unk50a1c0(unsigned o, int b, int c, int *out)
{
    int n;
    char *it, *bl;

    n = 0;
    it = (char *)he_FindEvent(1, 0, 0x23);
    while (it != 0 && n < 9) {
        bl = (char *)gm_BuildQuery(1, 1, *(int *)(it + 0xac));
        if (bl != 0 && *(unsigned short *)(bl + 0x27) == o && (b == 0 || b == (int)bl) &&
            (c == 0xffff || *(int *)(ai_Chars[c] + 4) == *(int *)(it + 0xb0))) {
            n++;
            out[n - 1] = (int)it;
        }
        it = (char *)he_NextEvent();
    }
    return n;
}
/* 0x50a250, hands the owner of the building b the person p's wage entries: up to `cnt` triples (value, kind 2 .. 7, amount) written to out (0x0 means the building has no owner / p is no person / cnt is 0; the first 3 triples of a kind 6 person's own record +0x71 are reused when they are still valid below the owner's budget 0x508660, else the whole table is built anew: from the random numbers (0x42ec00(0x1e), (6)) and the per-class tables 0x4f81c0 (0xf / 0xb) and the wage 0x50a640; a kind 6 person's table is stored back at +0x71, a kind 6 / 7 person employed by a non-6 / 7 owner is rated with 0x50c550). Returns 3 when written, 0 else. ROUGH: the float operands (the __ftol calls) are lost */

int gm_GetCreditOffers(int b, unsigned short *p, unsigned cnt, int *out)
{
    int budget, i, r, t, v;
    int *src, *dst;
    int tmp[0x88];
    char buf[36];
    unsigned k;
    int all = 0;

    if (p != 0 && b != 0 && *(short *)(b + 0x27) != -1 && cnt != 0) {
        budget = gm_SumSlotAmounts(*(int *)(b + 0x5d), ai_OwnPlayer);
        if (*((char *)p + 2) == 6) {
            if (*(int *)ai_Time < 6) {
                if (ai_Chars[*(unsigned short *)(b + 0x27)][2] != 6
                    && ai_Chars[*(unsigned short *)(b + 0x27)][2] != 7
                    && (float)budget < (float)gm_GetWorth(*p) * 0.2f) {
                    budget = (int)(float)budget;     /* lost */
                    cm_SendMoveGoods(*(int *)(b + 1), -1, ai_OwnPlayer, (int)(float)budget);
                }
            }
            src = (int *)(b + 0x71);
            dst = out;
            for (i = 3; i != 0; i--) {
                v = *src;
                if (v < 1) {
                    if (v == -1) {
                        all = -1;
                    }
                    *dst = -1;
                } else if (budget <= v) {
                    all = -1;
                    *dst = -1;
                } else {
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst[2] = src[2];
                    all = -1;
                }
                src += 3;
                dst += 3;
            }
            if (all == -1)
                return 3;
        }
        r = gm_Unk50a1c0(*(unsigned short *)(b + 0x27), 0, 0xffff, buf);
        if (r < 9) {
            memset(out, 0, cnt * 12);
            gm_GetWorth(*p);
            memset(tmp, 0, sizeof(tmp));
            tmp[0] = 1;
            k = ord_DispatchOrder(0xf, p, tmp);
            if ((k & 2) == 0)
                ord_DispatchOrder(0xb, p, tmp);
            for (k = 0; k < cnt; k++) {
                util_RandomU(0x1e);
                v = (int)(float)0;               /* lost */
                if (k == 0 || k == 1)
                    v = (int)(float)0;           /* lost */
                t = util_RandomU(6);
                r = (int)(float)0;               /* lost */
                if (ai_Chars[*(unsigned short *)(b + 0x27)][2] != 6
                    && ai_Chars[*(unsigned short *)(b + 0x27)][2] != 7
                    && (*((char *)p + 2) == 6 || *((char *)p + 2) == 7)) {
                    gm_Liking(*p, *(unsigned short *)(b + 0x27), 1);
                    r = (int)(float)0;           /* lost */
                }
                out[k * 3 + 2] = r;
                out[k * 3] = v;
                out[k * 3 + 1] = (unsigned short)t + 2;
            }
            if (*((char *)p + 2) == 6) {
                dst = (int *)(b + 0x71);
                for (i = 3; i != 0; i--) {
                    dst[0] = out[0];
                    dst[1] = out[1];
                    dst[2] = out[2];
                    out += 3;
                    dst += 3;
                }
            }
            return 3;
        }
    }
    return 0;
}
/* 0x50a640, the worth of the character id: its objects (0x509e60) plus the buildings it owns (query kind 3 of the owner): 0x509b10 and 0x509f80 per building; -1 for a free slot */


int gm_GetWorth(unsigned short id)
{
    int v, a, b;
    unsigned char *bl;

    if (id > 0x2ff)
        return -1;
    if (*(short *)ai_Chars[id] != -1) {
        v = gm_GetGoodsValue(ai_Chars[id]);
        bl = gm_BuildQuery(1, 3, id);
        while (bl != 0) {
            a = gm_EquipmentValue(bl, (char)bl[0x5c]);
            b = gm_Unk509f80((char *)bl);
            v = v + a + b;
            bl = gm_BuildNext();
        }
        return v;
    }
    return -1;
}
/* 0x50a6c0, true when the character p (a person of the kinds 6 / 7 or another) is too far from its goal after n more: the pseudocode lost the float operands (0x509e60, 0x50a640, the two __ftol); the person of the kinds 6 / 7 is reported to its owner through 0x5b92e0 / 0x53dda0 (text 0x42c3 = ...) when the ratio is above 0.07 (above 0.2 for the others). ROUGH */

int gm_IsBankrupt(unsigned short *p, int n)
{
    int a, b;
    char msg[2048];
    float ratio;
    int r;

    a = gm_GetGoodsValue(p);
    if (0.0f < (float)(a + n))
        return 0;
    gm_GetWorth(*p);
    if (*((char *)p + 2) == 6 || *((char *)p + 2) == 7) {
        a = (int)gm_GetGoodsValue(p);                /* lost: float operand of the first __ftol */
        b = (int)gm_GetGoodsValue(p);                /* lost */
        ratio = (float)(b < 0 ? -b : b) / (float)a;
        if (0.07f < ratio) {
            txt_Format(msg, 0x42c3);
            r = ai_Unk5b92e0(*(int *)(p + 2), p, a, msg);
            he_ShowNews(*(int *)(p + 2), *(int *)(p + 2), msg, 1, 0x5ff, "_NACHRICHTEN_HS_69");
            return r <= a / 2;
        }
    } else {
        b = (int)gm_GetGoodsValue(p);                /* lost */
        ratio = (float)(b < 0 ? -b : b) / (float)a;
        if (0.2f < ratio)
            return 1;
    }
    return 0;
}
/* 0x50a810, the first kind 6 / 7 / 1 / 2 character with a job whose master (+0x16c) is m */


char *gm_FindCharacterByMaster(int m)
{
    char *p;
    int i;
    char k;

    if (m == 0)
        return 0;
    i = 0;
    p = ai_Chars[0] + 2;
    while (p[0x162] == 0 || *(int *)(p + 0x16a) != m || ((k = *p) != 6 && k != 7 && k != 1 && k != 2)) {
        p += 0x21c;
        i++;
        if ((int)p > ((unsigned)((unsigned)ai_Chars + 0x65400) + 0x1))
            return 0;
    }
    return ai_Chars[i];
}
/* 0x50a880, sorts the n ids by their names (0x5143d0 gets the name of an id) */

void gm_Unk50a880(int n, int *ids, int flag)
{
    int i, j, t;
    char a[0x80], b[0x80];

    if (flag == 0 && n != 1 && n - 1 >= 0) {
        for (i = 1;; i++) {
            for (j = i; j < n; j++) {
                io_CharacterName(ids[i - 1], 1, 1, a);
                io_CharacterName(ids[j], 1, 1, b);
                if (strcmp(a, b) == 1) {
                    t = ids[i - 1];
                    ids[i - 1] = ids[j];
                    ids[j] = t;
                }
            }
            if (!(i < n - 1))
                break;
        }
    }
}
/* 0x50a970, the effect of a kind of event (kinds 1 .. 3 start with a float 0): the pseudocode lost both float operands. ROUGH placeholder */

void gm_Unk50a970(int a, int kind)
{
    float z = 0.0f;

    if (kind == 1 || kind == 2 || kind == 3)
        (void)(int)z;
    (void)(int)z;
}
/* 0x50aa00, the list of the stall object 0x1dd of the object c: out[0] the capacity, out[1] the number of entries, per entry the amount (+0x68), the stock (0x50ab50) .., out[0x2a] / [0x2b] the sums, out[0x2c] the last price. ROUGH */


int gm_StallGoodsList(char *c, unsigned *out)
{
    int i;
    char *l;
    unsigned short *o;
    unsigned v;

    for (i = 0; i < 0x2e; i++)
        out[i] = 0;
    l = gm_Query(*(void **)(c + 0x14), 1, 0, 0x1dd);
    if (l != 0) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        out[0] = (unsigned char)l[0x1c];
        while (o) {
            out[out[1] + 0x1a] = *(unsigned *)(o + 7);
            v = gm_ListStock((short *)l, *o);
            out[out[1] + 2] = v;
            out[0x2a] += out[out[1] + 2];
            out[0x2b] += *(unsigned *)(o + 7);
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
            out[0x2c] = (int)gm_ItemPrice(*o, 100);
            o = (unsigned short *)gm_QueryNext();
        }
        out[0x2d] = (unsigned)l;
        return 1;
    }
    return 0;
}
/* 0x50aaf0, the amount of the object o in the list object l: minus one for the stall objects 0x2a / 0x116 / 0x1db / 0x1dc (the stall itself) */

int gm_ListAmount(short *l, char *o)
{
    short k;

    k = *l;
    if (k != 0x2a && k != 0x116 && k != 0x1db && k != 0x1dc)
        return *(int *)(o + 0xe);
    return *(int *)(o + 0xe) - 1;
}
/* 0x50ab20, the amount (0x50aaf0) of the object id in the list of l */

int gm_ListAmountById(char *l, int id)
{
    char *o;

    o = gm_Query(*(void **)(l + 0x14), 1, 0, id);
    if (o == 0)
        return 0;
    return gm_ListAmount((short *)l, o);
}
/* 0x50ab50, the stock a list object l holds: (amount + 2) * 5 for the stall 0x1dd, 0x50 for 3, else 0x14 per amount */

int gm_ListStock(short *l, int id)
{
    int a;

    a = *(int *)(l + 7);
    if (*l == 0x1dd) {
        a += 2;
        return a * 5;
    }
    if (a == 3)
        return 0x50;
    return a * 0x14;
}
/* 0x50ab80, how many of the object id the list object l can still take (at most n), by the free places of its kind (0x50a0f0 gets its owner, 0x501d80 the class of the owner's building). ROUGH */

int gm_FreeCapacity(short *l, int id, int n)
{
    char k;
    unsigned char *o;
    int cls, cap, cnt, in;
    int pre;
    unsigned short *w;
    char *p;
    int t;

    o = (unsigned char *)gm_Unk50a0f0((int)l);
    if (o == 0)
        return 0;
    cls = gm_BuildingClass(*o);
    cap = gm_ListStock(l, id);
    in = (int)gm_Query(*(void **)(l + 10), 1, 0, id);
    if (in == 0) {
        k = ai_Goods[id * 0x41];
        if (cls != 2)
            pre = (k == 0x17 || k == 0x25);
        else
            pre = (k == 0x17);
        cnt = 0;
        if (*l == 0x2a) {
            w = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 5);
            while (w != 0) {
                p = ai_Goods + *w * 0x41;
                k = *p;
                if (cls != 2) {
                    if (pre) {
                        if (k == 0x17 || k == 0x25)
                            cnt++;
                    } else if (k != 0x17 && k != 0x25)
                        cnt++;
                } else {
                    if (pre) {
                        if (k == 0x17)
                            cnt++;
                    } else if (k != 0x17)
                        cnt++;
                }
                w = (unsigned short *)gm_QueryNext();
            }
            if (pre) {
                if (cnt >= *(unsigned char *)(l + 0xe))
                    return 0;
            } else if (cnt >= *(unsigned char *)((char *)l + 0x1d))
                return 0;
        } else {
            t = (int)gm_Query(*(void **)(l + 10), 1, 5);
            while (t != 0) {
                cnt++;
                t = (int)gm_QueryNext();
            }
            if (cnt >= *(unsigned char *)(l + 0xe))
                return 0;
        }
        if (cap < n)
            n = cap;
    } else {
        t = cap - gm_ListAmount(l, (char *)in);
        if (t <= n)
            return t;
    }
    return n;
}
/* 0x50acf0, the same for the stall list of the object l */

int gm_Unk50acf0(char *l, int id, int n)
{
    int cap, in, cnt, t;

    cap = gm_ListStock((short *)l, id);
    in = (int)gm_Query(*(void **)(l + 0x14), 1, 0, id);
    if (in == 0) {
        cnt = 0;
        t = (int)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (t != 0) {
            cnt++;
            t = (int)gm_QueryNext();
        }
        if (*(unsigned char *)(l + 0x1c) <= cnt)
            return 0;
        if (cap < n)
            n = cap;
    } else {
        cap = cap - *(int *)(in + 0xe);
        if (cap <= n)
            return cap;
    }
    return n;
}
/* 0x50ad70, how many of the object id the character c may take (n wanted, at most 3 of one kind, at most 5 kinds); only the kinds 8 0xc 0x12 0x19 0x22 0x20 0x27 count (kind 9: n), none of the 0x179 / 0x17a and none that the town forbids (0x4cd9d0) for the kinds 6 / 7 */

int gm_Unk50ad70(char *c, int proto, int n)
{
    unsigned char k;
    int found, cnt, v;
    int tab[8] = { 8, 0xc, 0x12, 0x19, 0x22, 0x20, 0x27, 0 };
    int *t;
    int u;
    char *o;
    unsigned short *w;

    v = 0;
    found = 0;
    k = ai_Goods[proto * 0x41];
    if (k == 9)
        return n;
    u = 8;
    t = tab;
    do {
        if (u == k) {
            found = 1;
            break;
        }
        u = *++t;
    } while (u != 0);
    if (((c[2] == 6 || c[2] == 7) && ai_FindWeaponRecord(proto) != 0) || proto == 0x179 || proto == 0x17a || !found)
        return 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 0, proto);
    if (o == 0) {
        cnt = 0;
        w = (unsigned short *)gm_Query(*(void **)(c + 0x178), 1, 5);
        if (w != 0) {
            do {
                if (ai_Goods[*w * 0x41] != 9)
                    cnt++;
                w = (unsigned short *)gm_QueryNext();
            } while (w != 0);
            if (cnt > 5)
                return v < 0 ? 0 : v;
        }
        v = 3;
        if (3 > n)
            v = n;
        return v < 0 ? 0 : v;
    } else {
        ai_FindWeaponRecord(proto);
        if (3 < *(int *)(o + 0xe) + n) {
            v = 3 - *(int *)(o + 0xe);
            return v < 0 ? 0 : v;
        }
    }
    v = n;
    return v < 0 ? 0 : v;
}

int ai_Difficulty = 0;  /* 0x6f23fc */
int gm_AllowAll = 0;  /* 0x6f2448 */
unsigned short gm_KindObjCount = 0;  /* 0x76fdc4 */
int ai_Unk76fdc8 = 0;  /* 0x76fdc8 */
/* ---- layout 2026-10-23: the standing / family helpers 0x50b010 .. 0x50d0b0 (ROUGH drafts) ---- */
char ai_Unk76fde0[3328] = {0};  /* 0x76fde0, the family records ( */
int ai_Unk770af0 = 0;  /* 0x770af0, the window of the family tree */
/* ai_Unkfb1030: view of d2_Views (tools/data_views.py) */
/* ai_Unkf8f128: view of d2_Wins (tools/data_views.py) */
void d2_PlotLine();                     /* 0x4764f0 */
void ai_Unk4c5510(int a);                     /* 0x4c5510 */
float gm_Liking();
int gm_Unk50b3a0();
int gm_Unk50b9f0();

/* 0x50af00, collects what the person `id` carries into the record out (0x218 bytes: the first word counts the 0xe-byte units, +0x31 the free capacity, +0x41 .. the entries of 0x1e bytes built from the goods list 0x74e1f4 (0x29 words each, owner at -2 words) that belong to the person, the entry count +0x219 and the sum +0x215). The person's own character record is searched with 0x501190 / 0x501030 (0x502eb0 gives its fields; one with the id equal adds 0xe to the first word and to +0x31). Returns 0. ROUGH */
int gm_CollectCarried(int id, int *out)
{
    int *p, *e;
    int rec;
    char buf[148];
    int c;

    memset(out, 0, 0x21c);
    rec = (int)gm_BuildQuery(1, 4, id);
    for (;;) {
        if (rec == 0)
            break;
        gm_ListWorkers(rec, buf);
        c = *(int *)(buf + 0x94);
        if (c != 0xffff && c == id) {
            *out += 0xe;
            *(int *)((char *)out + 0x31) += 0xe;
            break;
        }
        rec = (int)gm_BuildNext();
    }
    p = (int *)((unsigned)&he_QuickList + 0x6b4);
    e = (int *)((char *)out + 0x41);
    do {
        if (p[-5] != 0 && p[-2] == id) {
            e[-3] = p[-5];
            if (p[-3] != 0)
                *e = p[-1];
            e[1] = *p;
            e[2] = p[1];
            e[3] = p[2];
            *(short *)(e + 4) = *(short *)(p + 3);
            e[-1] = (int)p + 0xe;
            if (p[-3] != 0)
                *(int *)((char *)out + 0x215) -= p[-1];
            e[-2] = (int)(p - 5);
            e = (int *)((char *)e + 0x1e);
            *(int *)((char *)out + 0x219) += 1;
        }
        p += 0x29;
    } while ((int)p < ((unsigned)&he_QuickList + 0xaab4));
    return 0;
}
/* 0x50b010, the first building of the class n (1 .. 5: the types 0xf, 0x17 .. 0x1a); 0 for 6 / 7 */


unsigned char *gm_FirstBuilding(int n)
{
    unsigned char *r;

    r = 0;
    switch (n) {
    case 1:
        r = gm_BuildQuery(1, 5, 0xf);
        return r;
    case 2:
        r = gm_BuildQuery(1, 5, 0x17);
        return r;
    case 3:
        r = gm_BuildQuery(1, 5, 0x18);
        return r;
    case 4:
        r = gm_BuildQuery(1, 5, 0x19);
        return r;
    case 5:
        r = gm_BuildQuery(1, 5, 0x1a);
        break;
    case 6:
    case 7:
        return 0;
    }
    return r;
}
/* 0x50b090, the class number (out) of the building kind k: 1 for 0xf, 2 .. 5 for 0x17 .. 0x1a (returns 1), 6 / 7 / 7 for 7 / 4 / 0x10 (returns 0) */

int gm_ResearchClass(int k, int *out)
{
    int r;

    r = 1;
    switch (ai_BTypes[k * 0x24d]) {
    case 4:
        *out = 7;
        return 0;
    case 7:
        *out = 6;
        return 0;
    case 0xf:
        *out = 1;
        return r;
    case 0x10:
        *out = 7;
        break;
    case 0x17:
        *out = 2;
        return r;
    case 0x18:
        *out = 3;
        return r;
    case 0x19:
        *out = 4;
        return r;
    case 0x1a:
        *out = 5;
        return r;
    }
    return 0;
}
/* 0x50b170, the rank (1 = best) of the character c among the living persons by the score at +0x1ac (plus d) */

int gm_Unk50b170(int c, int d)
{
    int r, i;
    char *p;

    r = 1;
    if (c >= 0 && c < 0x300) {
        i = 0;
        p = ai_Chars[0];
        do {
            if (*(int *)(ai_Chars[c] + 0x1ac) + d <= *(int *)(p + 0x1ac) && *(short *)p != -1 && (unsigned char)p[2] < 10 && i != c)
                r++;
            p += 0x21c;
            i++;
        } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65400));
        return r;
    }
    return 0;
}
/* 0x50b1e0, the five best characters by the score at +0x1ac: ids[0 .. 4] (0xffff = none), sc[0 .. 4] */

void gm_Unk50b1e0(unsigned *ids, int *sc)
{
    int i, j;
    char *p;

    for (i = 0; i < 5; i++) {
        ids[i] = 0xffff;
        sc[i] = 0;
    }
    p = ai_Chars[0];
    do {
        if (*(short *)p != -1 && (unsigned char)p[2] < 10 && sc[4] < *(int *)(p + 0x1ac)) {
            sc[4] = *(int *)(p + 0x1ac);
            ids[4] = *(unsigned short *)p;
            for (j = 3; j >= 0; j--) {
                if (*(int *)(p + 0x1ac) <= sc[j] || (unsigned char)p[2] > 9)
                    break;
                sc[j + 1] = sc[j];
                ids[j + 1] = ids[j];
                sc[j] = *(int *)(p + 0x1ac);
                ids[j] = *(unsigned short *)p;
            }
        }
        p += 0x21c;
    } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65400));
}
/* 0x50b290, whether a random number plus 0.15 is below c * (the weight of 0x50c550) * 0.01 / d */

int gm_Unk50b290(int a, int b, int c, int d)
{
    float f, r;

    f = gm_Liking(a, b, 1);
    r = util_RandomF();
    if (r + 0.15 < (double)(c * f * 0.01 / d))
        return 1;
    return 0;
}
/* 0x50b2f0, the generation of the family record p: 0 without parents, else one more than the older parent's (cached in the word at +0xc) */

unsigned gm_Unk50b2f0(int p)
{
    unsigned a, b, i;
    int n, j;
    int *q;

    n = ai_Unk7d6a40;
    a = 0;
    b = 0;
    if (*(unsigned short *)(ai_Unk76fde0 + p * 0x34 + 0xc) != 0xffff)
        return *(unsigned short *)(ai_Unk76fde0 + p * 0x34 + 0xc);
    i = 0;
    if (*(int *)(ai_Unk76fde0 + p * 0x34 + 4) != 0) {
        if (0 < ai_Unk7d6a40) {
            q = (int *)ai_Unk76fde0;
            do {
                if (*(int *)(ai_Unk76fde0 + p * 0x34 + 4) == *q) {
                    a = gm_Unk50b2f0(i) + 1;
                    break;
                }
                i++;
                q += 0xd;
            } while ((int)i < ai_Unk7d6a40);
        }
        if (*(int *)(ai_Unk76fde0 + p * 0x34 + 8) == 0)
            return a;
        j = 0;
        if (0 < n) {
            q = (int *)ai_Unk76fde0;
            do {
                if (*(int *)(ai_Unk76fde0 + p * 0x34 + 8) == *q) {
                    b = gm_Unk50b2f0(j) + 1;
                    break;
                }
                j++;
                q += 0xd;
            } while (j < n);
        }
        i = a;
        if ((int)a <= (int)b)
            i = b;
    }
    return i;
}

typedef struct {
    int id, p1, p2;
    unsigned short rank, pad;
    int x, y, z;
    int l[5];
    unsigned char flag;
} RTNODE;       /* 0x34 bytes, the nodes of the research tree layout 0x76fde0 */
#define NODES ((RTNODE *)ai_Unk76fde0)
extern int ai_Unk889f70;
unsigned short ai_Unk770b00[64] = {0};  /* 0x770b00 */
unsigned gm_Unk50b2f0();                /* 0x50b2f0 */

/* 0x50b3a0, lays out the tree of the items that follow the item *b in the list of the building a (record +0x22 the count, +0x23 the ids (& 0x7fff), +0xa5 pairs of parents): the nodes (until an item of class 2 / 6) go into
   0x76fde0 (id, the two parents, rank = 0x50b2f0, x / y unset, links -1), are bubble-sorted by rank, the rank starts go to 0x770b00, then the x positions are spread over the width w (averages of the parents' x,
   spreading of equal x, a minimum distance of 0x50, a shift when the tree leaves the area), and the y positions over the height h (0x60 per rank at most); 1 when there is nothing to lay out, else 0. ROUGH */
int gm_Unk50b3a0(unsigned short *a, unsigned short *b, int w, int h)
{
    char *rec;
    int n, i, j, k, lv, cnt, ranks, step, sum, num, lo, hi, d;
    unsigned short *ids;
    unsigned short rmax;
    unsigned s, e, t, u;
    int again;
    RTNODE tmp;
    int *pp;

    rec = ai_BTypes + (unsigned char)*(char *)a * 0x24d;
    n = 0;
    cnt = (unsigned char)rec[0x22];
    ids = (unsigned short *)(rec + 0x23);
    if (cnt != 0) {
        do {
            if ((ids[n] & 0x7fff) == *b)
                break;
            n++;
        } while (n < cnt);
    }
    if (cnt <= n)
        return 1;
    ai_Unk7d6a40 = 0;
    n++;
    for (; n < (unsigned char)rec[0x22]; n++) {
        char cls = *(char *)(ai_Goods + (ids[n] & 0x7fff) * 0x41);
        RTNODE *r;
        if (cls == 2 || cls == 6)
            break;
        r = &NODES[ai_Unk7d6a40];
        r->id = ids[n] & 0x7fff;
        r->p1 = *(unsigned short *)(rec + 0xa3 + n * 4);
        r->p2 = *(unsigned short *)(rec + 0xa5 + n * 4);
        r->rank = 0xffff;
        r->x = 0;
        r->y = 0;
        for (k = 0; k < 5; k++)
            r->l[k] = -1;
        r->flag = 0xff;
        ai_Unk7d6a40++;
    }
    cnt = ai_Unk7d6a40;
    if (cnt == 0)
        return 1;
    rmax = 0;
    for (i = 0; i < cnt; i++) {
        NODES[i].rank = (unsigned short)gm_Unk50b2f0(i);
        if (rmax <= NODES[i].rank)
            rmax = NODES[i].rank;
    }
    ai_Unk889f70 = rmax + 1;
    do {
        again = 0;
        for (i = 0; i < cnt; i++)
            for (j = i + 1; j < cnt; j++)
                if (NODES[j].rank < NODES[i].rank) {
                    tmp = NODES[i];
                    NODES[i] = NODES[j];
                    NODES[j] = tmp;
                    again = 1;
                }
    } while (again);
    lv = 0;
    ai_Unk770b00[0] = 0;
    for (i = 0; i < cnt; i++) {
        if (lv < NODES[i].rank) {
            lv++;
            ai_Unk770b00[lv] = (unsigned short)i;
        }
    }
    ranks = ai_Unk889f70;
    ai_Unk770b00[ranks] = (unsigned short)cnt;
    t = ai_Unk770b00[1];
    step = w / (int)(t * 2);
    if (t != 0) {
        d = step - 0x18;
        for (i = 0; i < (int)t; i++) {
            NODES[i].x = d;
            d += step * 2;
        }
    }
    for (lv = 1; lv < ranks; lv++) {
        s = ai_Unk770b00[lv];
        e = ai_Unk770b00[lv + 1];
        for (i = s; i < (int)e; i++) {
            sum = 0;
            num = 0;
            for (j = 0; j < (int)s; j++) {
                if (NODES[j].id == NODES[i].p1 || NODES[j].id == NODES[i].p2) {
                    sum += NODES[j].x;
                    num++;
                }
            }
            if (num != 0)
                sum /= num;
            NODES[i].x = sum;
        }
        do {
            again = 0;
            for (i = s; i < (int)e; i++)
                for (j = i + 1; j < (int)e; j++)
                    if (NODES[j].x < NODES[i].x) {
                        tmp = NODES[i];
                        NODES[i] = NODES[j];
                        NODES[j] = tmp;
                        again = 1;
                    }
        } while (again);
        for (i = s; i < (int)e; i++) {
            u = i;
            for (j = i; j < (int)e; j++)
                if (NODES[i].x != NODES[j].x)
                    break;
            if (i + 1 < j) {
                num = j - i;
                d = (w / (int)t >> (lv - 1)) + (num * 3 + 3) * -8;
                if (num > 0) {
                    k = 0;
                    for (u = 0; (int)u < num; u++) {
                        NODES[i + u].x += k / (num - 1) - (d >> 1);
                        k += d;
                    }
                }
            }
        }
        for (i = s; i < (int)e; i++)
            for (j = i + 1; j < (int)e; j++) {
                lo = NODES[i].x;
                if (NODES[j].x - lo < 0x50) {
                    d = (lo - NODES[j].x + 0x50) >> 1;
                    NODES[i].x = lo - d;
                    NODES[j].x += d;
                }
            }
    }
    lo = w + 1;
    hi = -1;
    for (lv = 0; lv < ranks; lv++)
        for (i = ai_Unk770b00[lv]; i < ai_Unk770b00[lv + 1]; i++) {
            if (NODES[i].x <= lo)
                lo = NODES[i].x;
            if (hi <= NODES[i].x)
                hi = NODES[i].x;
        }
    if (lo - 0x18 < 0 || w <= hi + 0x60) {
        d = (int)((w - (hi - lo)) * 0.5);
        for (lv = 0; lv < ranks; lv++)
            for (i = ai_Unk770b00[lv]; i < ai_Unk770b00[lv + 1]; i++)
                NODES[i].x = (int)(NODES[i].x - lo) + d;
    }
    step = ranks * 0x60;
    if (h - 0x10 <= step)
        step = h - 0x10;
    for (i = 0; i < ai_Unk7d6a40; i++)
        NODES[i].y = (int)((unsigned)NODES[i].rank * step) / ranks + 0x10;
    return 0;
}

extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */
extern int d2_LastWindow;  /* 0x6ed938 */
int d2_RemoveWindow();                     /* 0x472ce0 */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);                     /* 0x472aa0 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */
#define WIDGET(i) (d2_Objs + (i) * 0x2ec)

/* 0x50b9f0, (re)builds the widgets of the nodes laid out by 0x50b3a0 for the building a and the player record b: per node the state 0 (not available) .. 3 (in progress) is derived (class 6 items: researched /
   queued objects 0x53e400 (2, 0, 0xc, 3, owner), parents known through 0x4ffe90; other items: the same with the order list kind 2), and when it differs from the node's last state (+0x30) the old widgets (+0x1c ..
   +0x2c) are freed (0x472ce0) and new ones created (0x472aa0 / 0x4731d0 with the images 0x52b / 0x529 / 0x52a / 0x4fb / 0x4fc, the label 0x54 and the text 0x515960 (item * 2 + 0x9a8)); state 3 nodes get their
   progress (+0x1e4 of the widget = the elapsed time over the duration, 0x4fe280). ROUGH */
int gm_Unk50b9f0(char *a, short *b)
{
    int i, k, cnt, state, par, w, cls, img, sub;
    char *o;
    int saved;
    int parent;
    RTNODE *nd;

    parent = *(int *)(ai_Unkfb1030 + ai_Unk770af0 * 0x2ac);
    for (i = 0; i < ai_Unk7d6a40; i++) {
        nd = &NODES[i];
        state = 0;
        cls = *(char *)(ai_Goods + nd->id * 0x41);
        o = 0;
        if (cls == 6) {
            if (gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0) {
                state = 1;
                for (o = (char *)he_FindEvent(2, 0, 0xc, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb4) == nd->id) {
                        state = 3;
                        break;
                    }
            } else {
                cnt = 0;
                for (k = 0; k < i; k++) {
                    RTNODE *q = &NODES[k];
                    if ((q->id == nd->p1 || q->id == nd->p2)
                        && (gm_Query(*(int *)(a + 0x5d), 1, 0, q->id) != 0 || gm_Query(*(int *)(b + 10), 1, 0, q->id) != 0)
                        && *(int *)(WIDGET(q->l[2]) + 0x50) == 0)
                        cnt++;
                }
                if (cnt == 0)
                    par = nd->p1;
                else if (cnt == 1)
                    par = nd->p2;
                else
                    par = cnt - 2;
                if (par == 0)
                    state = 2;
            }
        } else if (gm_Query(*(int *)(b + 10), 1, 0, nd->id) != 0)
            state = 1;
        else if (*b == 0xfd && gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0)
            state = 1;
        else {
            cnt = 0;
            for (k = 0; k < i; k++) {
                RTNODE *q = &NODES[k];
                if ((q->id == nd->p1 || q->id == nd->p2)
                    && (gm_Query(*(int *)(a + 0x5d), 1, 0, q->id) != 0 || gm_Query(*(int *)(b + 10), 1, 0, q->id) != 0))
                    cnt++;
            }
            if (cnt == 0)
                par = nd->p1 == 0;
            else if (cnt == 1)
                par = nd->p2 == 0;
            else
                par = cnt == 2;
            if (par) {
                state = 2;
                for (o = (char *)he_FindEvent(2, 0, 2, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb0) == *(int *)(b + 2) && *(unsigned short *)(o + 0xac) == nd->id) {
                        state = 3;
                        break;
                    }
            }
        }
        if (state != nd->flag) {
            saved = d2_LastWindow;
            if (nd->l[0] != -1) {
                d2_RemoveWindow(nd->l[0]);
                nd->l[0] = nd->l[1] = nd->l[2] = nd->l[3] = -1;
            }
            if (nd->l[4] != -1) {
                d2_RemoveWindow(nd->l[4]);
                nd->l[4] = -1;
            }
            w = d2_CreateChildWindow(nd->x - 4, nd->y - 4, 0x36, 0x36, 0x400, parent);
            nd->l[0] = w;
            if (state == 0) {
                nd->l[1] = d2_WinOpenPic(0, 0, 0x52b, w);
                nd->l[2] = d2_WinOpenPic(3, 3, 0x4fb, nd->l[0]);
            } else {
                img = state == 2 ? 0x52a : 0x529;
                nd->l[1] = d2_WinOpenPic(0, 0, img, w);
                sub = nd->id < 0x2dc ? nd->id + 0xdf : 0xdf;
                nd->l[2] = d2_WinOpenPic(3, 3, sub, nd->l[0]);
                if (state == 1)
                    *(int *)(WIDGET(nd->l[2]) + 0x54) = 1;
                *(int *)(WIDGET(nd->l[2]) + 0x4c) = 1;
                d2_LastWindow = saved;
                if (state == 3)
                    nd->l[3] = d2_WinOpenPic(3, 3, 0x4fc, nd->l[0]);
                nd->l[4] = d2_CreateChildWindow(nd->x - 0x18, nd->y + 0x32, 0x60, 0x11, 0x4510, parent);
                *(int *)(ai_Unkf8f128 + 0x274 + nd->l[4] * 0x3c4) = 0x54;
                *(short *)(ai_Unkf8f128 + 0xc + nd->l[4] * 0x3c4) = 1;
                *(short *)(ai_Unkf8f128 + 0xe + nd->l[4] * 0x3c4) = 1;
                if (nd->id < 0x2dc)
                    io_PrintText(nd->id * 2 + 0x9a8);
                if (state == 3)
                    *(int *)(WIDGET(nd->l[2]) + 0x54) = 1;
            }
            d2_LastWindow = saved;
            nd->flag = (unsigned char)state;
        }
        if (nd->flag == 3) {
            if (cls == 6) {
                if (gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0)
                    for (o = (char *)he_FindEvent(2, 0, 0xc, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                        if (*(int *)(o + 0xb4) == nd->id)
                            goto progress;
            } else {
                for (o = (char *)he_FindEvent(2, 0, 2, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb0) == *(int *)(b + 2) && *(unsigned short *)(o + 0xac) == nd->id)
                        goto progress;
            }
            *(float *)(WIDGET(nd->l[2]) + 0x1e4) = 0;
            continue;
        progress:
            k = gm_MinutesBetween(o + 0x44, ai_Time);
            *(float *)(WIDGET(nd->l[2]) + 0x1e4) = (float)gm_MinutesBetween(o + 0x44, o + 0x52) ? (float)k / (float)gm_MinutesBetween(o + 0x44, o + 0x52) : 0.0f;
        }
    }
    return 0;
}
/* 0x50c310, a line of the family tree (the colour 0x58, the widths 0x1f / 0x14) */

void gm_Unk50c310(int a, int b, int c, int d)
{
    d2_PlotLine(a, b, c, d, 0x58, 0x1f, 0x14);
}
/* 0x50c340, opens the family tree window for the character (the form name 0x4741f0), prints the title ("$[%s$]", 0x19: the name text 0x48e + 0xe * kind or 0x9a8 + 2 * id), draws the boxes (0x50b3a0) and the lines between the records (0x50b9f0 positions them), returns the window or -1. ROUGH */

int gm_Unk50c340(int form, unsigned char *p, unsigned short *q)
{
    int i, j, w, t, n;
    int *r;
    int a, b, c, d;

    ai_Unk770af0 = d2_OpenForm(0, 0, form);
    d2_CentreViewWindows(ai_Unk770af0);
    ai_Unk76fdc8 = ai_ClockMs;
    w = *(int *)(ai_Unkfb1030 + ai_Unk770af0 * 0x2ac);
    d2_SetForm(0, 1);
    if (*q == 0xfd)
        t = *p * 0xe + 0x48e;
    else
        t = *q * 2 + 0x9a8;
    io_PrintText("$[%s$]", 0x19, t);
    d2_SetForm(0, 3);
    n = gm_Unk50b3a0(p, q, (int)*(short *)(ai_Unkf8f128 + w * 0x3c4), (int)*(short *)(ai_Unkf8f128 + 2 + w * 0x3c4));
    if (n != 0) {
        d2_CloseView(ai_Unk770af0);
        return -1;
    }
    gm_Unk50b9f0(p, q);
    i = 0;
    if (0 < ai_Unk7d6a40) {
        j = 0;
        do {
            if (0 < j) {
                r = (int *)(ai_Unk76fde0 + 0x14);
                t = i;
                do {
                    if (r[-5] == *(int *)(ai_Unk76fde0 + 4 + j)) {
                        a = *(int *)(ai_Unk76fde0 + 0x14 + j);
                        b = *(int *)(ai_Unk76fde0 + 0x10 + j);
                        c = *r;
                        d = r[-1];
                        gm_Unk50c310(d + 0x18, c + 0x30, b + 0x18, a + 0x18);
                    } else if (r[-5] == *(int *)(ai_Unk76fde0 + 8 + j)) {
                        a = *(int *)(ai_Unk76fde0 + 0x14 + j);
                        b = *(int *)(ai_Unk76fde0 + 0x10 + j);
                        c = *r;
                        d = r[-1];
                        gm_Unk50c310(d + 0x18, c + 0x30, b + 0x18, a + 0x18);
                    }
                    r += 0xd;
                    t--;
                } while (t != 0);
            }
            i++;
            j += 0x34;
        } while (i < ai_Unk7d6a40);
    }
    return ai_Unk770af0;
}
/* 0x50c4d0, closes the family tree window */

void gm_Unk50c4d0(void)
{
    d2_CloseView(ai_Unk770af0);
}
/* 0x50c4e0 */

void gm_Unk50c4e0(int a)
{
    unsigned char *b;
    char *o;

    b = gm_FirstBuilding(1);
    o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x115);
    cm_SendMoveGoodsUnchecked(-1, *(int *)(o + 2), ai_OwnPlayer, a);
}
/* 0x50c520, the opinion of the character a of the character b (0x7f for oneself) */

int gm_Opinion(int a, int b)
{
    if (a == b)
        return 0x7f;
    return ai_Relations[a][b];
}

int gm_PlayerRecords[688] = {0};  /* 0x7d5f80, the 16 player records ( */
int ai_Unk7d6a40 = 0;  /* 0x7d6a40, the number of family records */
char gm_StockBlocks[0xd4d0] = {0}; /* 0x7d6a60, the five stock blocks (five rows of four header ints plus 0x55 records of 0x80 bytes); a tentative definition, so the COMMON linker reserves the whole span and no other global lands inside it. init_StockBlock is the same object (data.csv alias). */
struct AIOBJ *ai_Objects = 0;  /* 0x7e3f34, the object instances (AIOBJ, ai.h) */
int gm_QueryDepth = 0;  /* 0x7e3f38, the depth of the child stack */
int gm_NOffers = 0;  /* 0x7e3f3c, the number of offers */
AISTORE gm_Offers[88] = {0};  /* 0x7e4b20, the offers (ai.h's AISTORE rows of 6 ints: building, market, goods object, supply, price float, flag) */
int gm_ClassBuilding[32] = {0};  /* 0x875360 */
int gm_QueryStack[20488] = {0};  /* 0x8753e0, the child stack (the first word of */
unsigned gm_KindObjs[732] = {0};  /* 0x889400 */
int ai_Unk889f70 = 0;  /* 0x889f70 */
char * ai_Blds = 0;  /* 0x889f74, the buildings ( */
int gm_GoodBuilding[732] = {0};  /* 0x919f80 */
char * ai_Goods = 0;  /* 0x91aaf0, the object prototypes ( */
unsigned gm_OwnNameHeads[136] = {0};  /* 0x91ac40 */
char gm_CharacterStatistics[128] = {0};  /* 0x91b500, the character statistics ( */
char d2_GfxDir[256] = {0}; /* 0xf8ef80, the game root path */
/* 0x50c550, how much the person a likes the person b, 0 .. 100 (100 for the same person, 50 when one of them is unknown (0xffff)): the sum of the liking of a's two bound persons (0x5092e0 of the words +0x170 and, for a person with a kind 4 spouse at +0xce6, of 0x50b010(1), and of the guild head of the faith byte 0x1e .. 0x21 -> the persons 0x17 .. 0x1a), the stored mutual rating of the two (0x7e5360 matrix, scaled to 0 .. 100), the best building of the class 0x21 the person a lives in (the byte +0x247, up to 3), the rating of 0x4e5e50, the bonuses per kind of b's job group (0x5ba6b0 of the goods 0x155 .. 0x166: 10 / 7 / 8 / 15 times the count), the kinds 0x2b6 (+5 when b has a spouse, +5 when b has the flags 0x3e00), the owner link (0x16b, times 7), minus 3 per level of the bits 14..16 (a different faith byte) and bits 23..24 of a's flags +0x1ac. ROUGH: converted from the pseudocode, the long float expression is lost in part */
float gm_Liking(unsigned a, unsigned b, int c)
{
    float sum = 0.0f;
    int ra, rb, k;
    char m;
    unsigned char best = 0, kind;
    unsigned i;
    unsigned char *bp;
    char *cls;
    unsigned char sp[16];
    float f;
    unsigned fl;
    int w;
    unsigned char *ca = (unsigned char *)ai_Chars[a];
    unsigned char *cb = (unsigned char *)ai_Chars[b];

    if (a == b)
        return 100.0f;
    if (a == 0xffff || b == 0xffff)
        return 50.0f;
    if (*(int *)(ca + 0x170) != 0)
        sum = (float)gm_BuildingSkill(*(int *)(ca + 0x170), 4, 1);
    if (ca[0x166] != 0) {
        char rec[0x20];

        amt_GetKind(ca[0x166], rec);
        if (*(int *)(rec + 4) == 4) {
            ra = gm_FirstBuilding(1);
            if (ra != 0)
                sum = (float)gm_BuildingSkill(ra, 4, 1) + sum;
        }
    }
    kind = ca[0x169];
    if (kind > 0x1d && kind < 0x22) {
        int g = 0;

        switch (kind) {
        case 0x1e:
            g = 0x17;
            break;
        case 0x1f:
            g = 0x18;
            break;
        case 0x20:
            g = 0x19;
            break;
        case 0x21:
            g = 0x1a;
        }
        ra = gm_BuildQuery(1, 5, g);
        if (ra != 0 && (*(unsigned char *)(ra + 0x5a) & 1) == 0)
            sum = (float)gm_BuildingSkill(ra, 4, 1) + sum;
    }
    m = *((char *)(unsigned)&ai_Relations + b + a * 0x300);
    i = 0;
    bp = (unsigned char *)ai_Blds;
    do {
        if (*bp != 0 && *(short *)(bp + 0x27) == *(short *)ai_Chars[a]) {
            cls = ai_BTypes + (unsigned)*bp * 0x24d;
            if (*cls == '!' && best < (unsigned char)cls[0x247])
                best = cls[0x247];
            if (best > 2)
                break;
        }
        i++;
        bp += 0xa9;
    } while (i < 0x100);
    f = (float)amt_Rating(ai_Chars[a], ai_Chars[b], best, c);
    sum = f + ((float)(m + 0x7f) * 0.003921569f * 100.0f + sum);
    if (cb[0x166] != 0) {
        unsigned char jk;
        char rec[0x20];

        amt_GetKind(cb[0x166], rec);
        jk = *(unsigned char *)(rec + 8);
        if (jk == 0 || jk > 3) {
            if (jk < 4 || jk > 7) {
                if (jk >= 8 && jk <= 9) {
                    sum += (float)ai_InventoryHas(ai_Chars[a], 0x165) * 7.0f + (float)ai_InventoryHas(ai_Chars[a], 0x159) * 7.0f;
                    sum += (float)ai_InventoryHas(ai_Chars[a], 0x166) * 15.0f;
                }
            } else {
                sum += (float)ai_InventoryHas(ai_Chars[a], 0x157) * 10.0f;
                sum += (float)ai_InventoryHas(ai_Chars[a], 0x15f) * 8.0f;
            }
        } else {
            sum += (float)ai_InventoryHas(ai_Chars[a], 0x155) * 10.0f;
            sum += (float)ai_InventoryHas(ai_Chars[a], 0x15d) * 10.0f;
        }
    }
    w = ai_InventoryHas(ai_Chars[a], 0x2b6);
    if (w != 0) {
        if (cb[0x166] != 0)
            sum += 5.0f;
        if ((*(unsigned *)(cb + 0x1cc) & 0x3e00) != 0)
            sum += 5.0f;
    }
    if (*(int *)(cb + 0x16c) != 0 && *(unsigned short *)(*(int *)(cb + 0x16c) + 0x27) == a)
        sum += (float)ai_InventoryHas(ai_Chars[a], 0x16b) * 7.0f;
    fl = *(unsigned *)(ca + 0x2c) >> 0xe & 7;
    if (fl != 0 && ca[9] != cb[9])
        sum -= (float)(fl * 3);
    fl = *(unsigned *)(ca + 0x2c) >> 0x17 & 3;
    if (fl != 0)
        sum -= (float)(fl * 3);
    if (sum < 100.0f) {
        if (0.0f < sum)
            return sum;
        return 0.0f;
    }
    return 100.0f;
}
/* 0x50c980, the mean opinion (0x50c550 for the persons of the list ids[0 .. n - 1] about the character c) * 0.01; 0.5 without any */


float gm_Unk50c980(int c, int n, int *ids)
{
    int cnt, i, id;
    float s;
    unsigned short *p;

    cnt = 0;
    i = 0;
    s = 0.0f;
    if (0 < n) {
        do {
            id = ids[i];
            if (id != -1 && (p = (unsigned short *)gm_FindChar(id)) != 0) {
                s = (float)(gm_Liking(c, *p, 0) + s);
                cnt++;
            }
            i++;
        } while (i < n);
        if (cnt != 0)
            return s * 0.01 / cnt;
    }
    return 0.5f;
}
/* 0x50ca10, tells every character that has the flag byte at +0x166 the message a about b */

void gm_Unk50ca10(unsigned short a, int b)
{
    int i;
    char *p;

    i = 0;
    p = ai_Chars[0] + 0x166;
    do {
        if (*p != 0)
            cm_SendRelations(a, i, b, 1);
        p += 0x21c;
        i++;
    } while ((int)p < (int)((unsigned)&ai_Chars + 0x65566));
}
/* 0x50ca50, changes the state byte (+0x12) of the town object id by d (not below 0) and sends the record */

void gm_Unk50ca50(int id, int d)
{
    char buf[0x80];
    char v;
    char *o;

    v = d;
    o = gm_Query(0, 2, 7, 1, id);
    if (o == 0) {
        sprintf(buf, "gm_ChangeObjektZustand():Could not find Object-ID: %i", id);
        return;
    }
    if ((unsigned char)o[0x12] + d < 0)
        v = 0;
    cm_BeginFieldBlock(o, id);
    cm_AddFieldValues(1, 1, 0x12, &v);
    cm_SendFieldDeltas();
}
/* 0x50cad0, changes the state byte (+0x5c) of the building b by d (kept in 0 .. 100) and sends it; when a building of a kind that matters drops to 0, every person is told (HS_66 for the owner, HS_67 for the others) */

void gm_ChangeBuildingState(unsigned char *b, int d)
{
    unsigned char k;
    char v;
    int *q;
    char buf1[0x400], buf2[0x400];

    v = d;
    k = b[0x5c];
    if (d + (char)k < 0)
        v = -k;
    if (100 < d + (char)k)
        v = 100 - k;
    cm_BeginFieldBlock(b, *(int *)(b + 1));
    cm_AddFieldValues(1, 1, 0x5c, &v);
    cm_SendFieldDeltas();
    if (*b != 0 && (int)v + (char)b[0x5c] < 1 && (k = ai_BTypes[*b * 0x24d]) != 0xf && k != 0x1c && k != 0x1b && k != 10 && k != 6 && k < 0x17 && k != 0x10) {
        q = (int *)(ai_Chars[0] + 4);
        do {
            if (*((char *)q - 2) == 6 || *((char *)q - 2) == 7) {
                if (*(short *)(q - 1) == *(short *)(b + 0x27)) {
                    txt_Format(buf1, 0x1c53, b);
                    he_ShowNews(*q, -1, buf1, 0, 0x5fe, "_NACHRICHTEN_HS_66");
                } else {
                    txt_Format(buf2, 0x1c52, *(short *)(b + 0x27));
                    he_ShowNews(*q, -1, buf2, 0, 0x5fe, "_NACHRICHTEN_HS_67");
                }
            }
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x2));
        ai_Unk4c5510(*(int *)(b + 1));
    }
}
/* 0x50cc50, forwards to 0x4c5f60 */

void gm_SendMoneyDelta(int a, int b)
{
    cm_SendMoneyDelta(a, b);
}
/* 0x50cc60, sets the amount of every object in the first object (0xff) of the buildings of the type 0x1e to 2 */

void gm_ResetStock(void)
{
    unsigned char *b;
    char *l, *o;

    b = gm_BuildQuery(1, 0, 0x1e);
    while (b != 0) {
        l = gm_Query(*(void **)(b + 0x5d), 1, 0, 0xff);
        if (l != 0) {
            o = gm_Query(*(void **)(l + 0x14), 1, 5);
            while (o != 0) {
                if (*(int *)(o + 0xe) != 2)
                    *(int *)(o + 0xe) = 2;
                o = (char *)gm_QueryNext();
            }
        }
        b = gm_BuildNext();
    }
}
/* 0x50ccd0, 1 when the character c (flag +0x1b1) has no building of the type 4 (a home) of his id (+0x65) */

int gm_Unk50ccd0(char *c)
{
    int i;
    unsigned char *b;

    if (c[0x1b1] != 0) {
        i = 0;
        b = (unsigned char *)ai_Blds;
        while (*b == 0 || ai_BTypes[*b * 0x24d] != 4 || *(int *)(b + 0x65) != *(int *)(c + 4)) {
            b += 0xa9;
            i++;
            if (i > 0xff)
                return 1;
        }
    }
    return 0;
}
/* 0x50cd40, the wage of 0x508740 times 3 (9 for the jobs 10 .. 12 of the building kind k) */

float gm_Unk50cd40(char *c, int k, int a, int b)
{
    int j;

    j = gm_BuildingJob((unsigned char)ai_BTypes[k * 0x24d]);
    if (j != 10 && j != 0xb && j != 0xc)
        return gm_JobWage(c, k, a, b) * 3.0f;
    return gm_JobWage(c, k, a, b) * 9.0f;
}
/* 0x50cdc0, the standing of the building b with the persons ids[0 .. n - 1]: the mean of their 0x4fc8e0 weights (+0.3 for those whose title matches the building's), added to the owner's factor. ROUGH */

float gm_StaffFactor(unsigned char *b, int n, int *ids)
{
    char *p, *c;
    int i, cnt, id;
    float a, s, m, f, t;

    p = ai_BTypes + *b * 0x24d;
    i = 0;
    a = 0.0f;
    cnt = 0;
    s = 0.0f;
    m = 0.0f;
    gm_BuildingSkill(b, 1, 1);
    c = gm_FindCharacterByMaster((int)b);
    if (c == 0)
        c = ai_Chars[*(unsigned short *)(b + 0x27)];
    if (c[2] == 6 || c[2] == 7)
        f = (unsigned char)c[0x81] * 0.000992063492063492;
    else
        f = 0.0f;
    if (0 < n) {
        do {
            id = ids[i];
            if (id != -1 && (c = gm_FindChar(id)) != 0) {
                s = (float)(gm_HealthRatio((short *)c) + s);
                if (p[0x22f] == c[0x165])
                    m = m + 0.3;
                cnt++;
            }
            i++;
        } while (i < n);
        if (cnt != 0) {
            a = s * (1.0f / (float)cnt);
            m = (1.0f / (float)cnt) * m;
        }
    }
    t = gm_Unk50c980(*(unsigned short *)(b + 0x27), n, ids);
    f = (float)(f + 0);
    if ((b[0x5b] & 0x10) != 0)
        f = f + 0.1;
    return (m + a) * ((t - 0.5) * 0.25 + f + (*(int *)(b + 0x41) - 2) * 0.1);
}
/* 0x50cf60, the number of the kitchen / bath objects (0x27a, 0x27b, 0x292) the building b has: 1 + .. */

int gm_BathObjectCount(char *b)
{
    int n;

    n = 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27a) != 0)
        n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27b) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x292) != 0)
        n = n + 1;
    return n;
}
/* 0x50cfd0, the same with 0x274 / 0x27e / 0x28e / 0x291 (2 + ..) */

int gm_CurseObjectCount(char *b)
{
    int n;

    n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x274) != 0)
        n = 3;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27e) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x28e) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x291) != 0)
        n = n + 1;
    return n;
}
/* 0x50d060, 0x285 / 0x288 (1 + ..) */

int gm_Unk50d060(char *b)
{
    int n;

    n = 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x285) != 0)
        n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x288) != 0)
        n = n + 1;
    return n;
}
/* 0x50d0b0, 0x284 (4 -> 7) / 0x289 (+ 3) */

int gm_CurseTargetCount(char *b)
{
    int n;

    n = 4;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x284) != 0)
        n = 7;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x289) != 0)
        n = n + 3;
    return n;
}
