/* f3gm -- the character income / statistics screen, the character names and heads, the richest character, homes,
   character creation, the icon names and the character count (0x4f9e30 .. 0x4fe140); the name is the old whole
   unit's, the allocation tags "f3_gm:" are f3gmt's. Its .data block is the head tables gm_BoyHead 0x6118a8 ..
   gm_BirthChance with its strings 0x61774c .. 0x617818 ("misc\statistics_taxes" ..), its .rdata literals end at
   0x5ee158, its .bss is 0x6f1b90 .. 0x6f1bb4; f3gmt (0x4fe140, the game clock and the tables) follows.
   Several Open lines below were written for the whole unit and now concern f3gmt's functions.
   Open: many callees are still declared K&R in this file (amt_HasTownObject must stay K&R: the typed prototype
   costs gm_Unk4fa4b0 a byte).
   Open: gm_GetWorth is int in every header now; the older callers still go through the K&R cast I50A640.
   Open: gm_IsBankrupt calls ai_LiquidateAssets (0x5b92e0: netId, character, amount, message) without a prototype;
   its definition in lc.c is still a one-parameter skeleton.
   Open: ai.h is not includable here, so the character / building / object records are read through gv.h's views
   ORDCHAR / ORDBLD / ORDOBJ, the job record through GMJOB (AIJOB, next to AITIME), the long message through
   f3gm_types.h's GMMSG (AIMSG), the crime records through GM_CRIMES (cm_types.h CRIME over cm.c's char
   ai_Crimes[]), the goods prototypes through GMGOOD (AIGOOD, GM_GOOD), the goods records of the
   stock blocks through GMBLOCK / GMSTOCK (f3gm_types.h, GM_BLOCK; f3init.c / hst.c / ls.c use them too),
   the building types through GMBTYPE (AIBTYPE, GM_BTYPE; older functions still by
   byte offset), the kind 0x23 events through GMEVENT23, or by byte offset (the rest, GM_GOODS).
   Open: gm_QueryNext returns unsigned short * in every header though the cursor is an ORDOBJ *.
   Open: head: cm_SendExSellObject returns its message's slot handle (gm_SyncMarket waits on it), cm.h declares
   it void; this unit calls it through the cast cm_SendExSellObjectH.
   Open: the head tables (gm_BoyHead .. gm_SpyHeads, gm_OwnNameHeads) are GMHEAD records but still defined as
   unsigned arrays; their readers cast them.
   Open: not included yet (dcp headers conflicts f3gm HEADER): ai.h anm.h cm.h d3_interface.h d3_tools.h
   gfx.h hmt.h hst.h rt.h d3_pose.h mission.h; held by own declarations that differ from the owner's:
   gm_MinutesBetween, gm_ClockSeason, gm_CompareTime; calls whose argument count differs from the
   definition (read the original's pushes): gm_Liking.
   Open: gm_AssignBuilding is defined with unsigned short master / unsigned own, but its callers' pushes (f3init's
   init caller, gm_TakeOverCharacter) zero-extend one word for both: int parameters; this unit calls it through the
   cast gm_AssignBuildingI until the definition and headers are settled.
   Open: gm_TitlePeriodServed writes an AITIME through its int *out (he.c, main.c, lc.c declare it so in
   their own prototypes); the typed parameter waits for those units to include f3gm.h.
   unit of rea 8cef7d5b (src/game/f3gm.c); boundaries in decl/text.dcl */
struct CHARACTER;
struct CHTRANSPORT;
struct AICHAR;
#include "d2_types.h"
#include "f3gm_types.h"
#include "txt_views.h"
#include "d2.h"
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
/* The stock area is ONE 0xd4d0-byte object at 0x7d6a60 (five blocks of 0x2a90: four header ints, then
   0x55 records of 0x80 bytes). The other names are views into it: a separate definition would reserve
   its own memory and the block writer/reader (gm_LoadTables, ls_ReadPlayerSlots, ls_WritePlayerSlots) would then
   write over whatever the COMMON linker placed inside the 0xd4d0-byte span. */
#define init_StockBlocks (*(char (*)[0xd4c0])((char *)gm_StockBlocks + 0x10))  /* 0x7d6a70, the records of block 0 */
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
/* the building type i's byte +0x21 (ai.h AIBTYPE.extensions, the room upgrades it takes): GMBTYPE keeps it in its pad,
   because naming the field in f3gm_types.h moves sv_t's ai_TutorialInitChapter3 off byte (declaration-set tie-break) */
#define GM_BTYPE_EXTENSIONS(i) (((unsigned char *)GM_BTYPE(i))[0x21])
#define GM_OBJ_PROTO_MASK 0xffffff  /* the prototype reference's index bits of a scene object's pickId (D3OBJECT +0x48) */

#include "am_types.h"
/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "d3_types.h"
extern unsigned char he_Events[339968];
#include "amt_types.h"

#include "ai_types.h"
extern int gm_PlayerRecords[688];
extern int gm_TreeNodeCount;
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);
void gm_FreeObjectExtra(unsigned short *o);
unsigned short gm_ObjectOwnerNo(unsigned short *o);
void gm_UpdatePrices(unsigned p, void *t);

struct AITIME;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */
struct D3SURF;  /* d3_engine.h typedef; file-scope tag for the d3if_SelectTextureSet prototype */


extern int gm_NetMsgCount;
extern int gm_RichestYear;
extern int gm_RichestWorth;
extern ORDCHAR *gm_RichestChar;
/* The five income sums are the last int of the five 17-int rows of gm_IncomeHistory (0x91b580, 0x154
   bytes): the rows start 0x44 apart, so these aliases sit at the end of each row. They are views, not
   separate globals - a separate definition would reserve its own memory here and the 0x154-byte
   ord_ResetStatistics memset would zero whatever the COMMON linker put inside it (it hit ai_BTypes). */
#define ai_Unk91b648 (gm_IncomeHistory + 0xc8)   /* 0x91b648, row 2 */
#define ai_Unk91b68c (gm_IncomeHistory + 0x10c)  /* 0x91b68c, row 3 */
#define gm_IncomeRows (*(int (*)[5][17])&gm_IncomeHistory)  /* 0x91b580, the five rows of 17 ints; [row][16] is the current sum */
#include "f3gm_carry.h"            /* GMCARRYSPEC, GMCARRYREC, gm_CarryRecs */
#pragma pack(push, 1)
/* an entry of gm_CollectCarried's list (0x1e bytes) */
typedef struct GMCARRYENTRY {
    int ref;                            /* 0x00, the record's ref */
    GMCARRYREC *rec;                    /* 0x04, the record */
    char *data;                         /* 0x08, the record's data */
    int value;                          /* 0x0c, the record's value when it has one */
    GMCARRYSPEC spec;                   /* 0x10 */
} GMCARRYENTRY;

/* what gm_CollectCarried collects for a person (0x21d bytes; main.c MAINCARRY reads the head of it) */
typedef struct GMCARRY {
    int units;                          /* 0x000, the 0xe-byte units */
    char goods[0x2d];                   /* 0x004, main.c: the goods list of the balance page */
    int capacity;                       /* 0x031, the free capacity */
    GMCARRYENTRY entries[16];           /* 0x035 */
    int valueSum;                       /* 0x215, minus the values of the records with one */
    int count;                          /* 0x219, the entries used */
} GMCARRY;
#pragma pack(pop)
typedef char gm_AssertGMCARRY[sizeof(GMCARRY) == 0x21d ? 1 : -1];

int gm_IsUnderAge(unsigned short id);         /* 0x4fd7e0 */

#define CH(i) (ai_Chars[(unsigned short)(i)])
#define REC_RATE(rec) ((rec)[6])

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
#include "sv_t.h"
#include "lc.h"
#include "d3_engine.h"
#include "ch.h"
#include "cm_types.h"
#include "net.h"
#include "rd.h"
#include "ai.h"
#include "anm.h"
#include "d3_interface.h"
#include "d3_tools.h"
#include "hmt.h"
#include "hst.h"
#include "d3_pose.h"
#include "mission.h"
#include "f3gm.h"
void *ai_ProdSetting(int param_1);  /* 0x5a8560 */
#define ai_ProdSettingK ((int (*)(void *))ai_ProdSetting)  /* 0x5a8560 (lc.c: void *ai_ProdSetting(int bld)): @original this unit's callers pass the building pointer (gm_CreateBuilding 0x501676 pushes the record, as lc.c's own callers cast it); the result is unused */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original gm_Shuffled reads the 16-bit result of the 16-bit parameter's call (movzx bp,dl push, mov si,ax / and 0xffff), as anm.c's callers */
#define gm_AssignBuildingI ((void (*)(unsigned char *, int, int))gm_AssignBuilding)  /* @original 0x502b60 called with int place / own: gm_TakeOverCharacter zero-extends the word load once for both (as f3init's byte-matched init caller does, see its prototype) */
extern struct D2COLKEY d2_TransparentColour;  /* 0x6f0454, shp.c (the type in shp.h) */
#define d2_TransparentBytes ((unsigned char *)&d2_TransparentColour)  /* 0x6f0454, @original: unk_LoadIconNames stores the key byte by byte (mov byte [0x6f0455], 0xff; [0x6f0454] and [0x6f0456] = bl), not through the D2COLKEY word gb */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define GM_PLAYERCH (*(ORDCHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar as the character record it points to */
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
        io_PrintText("Runde %i:   %T$N", ai_Time.year - 0x10 + i, ((int *)gm_IncomeHistory)[i]);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(gm_StatisticsScreen, 0x677c7) != 0);
    d2_CloseView(f);
}
/* 0x4f9ec0, the income rating 0 .. 1: a weighted sum over the income kinds 9 .. 0xf of the table 0x4f2720
   (weights 0.3 0.25 0.12 0.1 0.05 0.08 0.1), each value normalized by its record's range:
   sum of (1 - (v - lo) / (hi - lo)) * weight; the original's return adds the last term x once more */

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
    return sum + x;
}
/* 0x4f9f60, income kind 1 of the character id (rate of the record 9 * 0.01 * the character's int at +0x19c, not below 0): stores kind 1, the amount (+8 + 4k) and the record's rate (+0x20 + 4k) in the first free slot k of the 6 of out;
   flag 1 sends the amount to the player, flag 2 adds it to the sum 0x91b5c0. Returns 1 (slot found) or 0 */

int gm_IncomeKind1(unsigned id, GMINCOME *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;
    ORDBLD *b;
    ORDOBJ *o;

    gv_GetTableRecord(9, rec);
    for (k = 0; k < 6; k++) {
        if (out->kind[k] == 0)
            break;
    }
    if (k >= 6)
        return 0;
    cnt = ORD_CHARS[(unsigned short)id].wage;
    x = rec[6] * 0.01f;
    x = x * cnt;
    if (!(x > 0.0f))
        x = 0.0f;
    n = (int)x;
    if (cnt > 0) {
        out->kind[k] = 1;
        out->amount[k] = n;
        out->rate[k] = rec[6];
        if (flags & 1) {
            b = (ORDBLD *)gm_FirstBuilding(1);
            o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
            cm_SendMoveGoodsUnchecked(o->objId, ORD_CHARS[(unsigned short)id].netId, 0, n);
            gm_NetMsgCount++;
        }
        if (flags & 2)
            gm_IncomeRows[0][16] += n;
    }
    return 1;
}
/* 0x4fa060, income kind 2 (the record 10, the int at +0x19c; sum 0x91b604): a copy of 0x4f9f60 */

int gm_IncomeKind2(unsigned id, GMINCOME *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;
    ORDBLD *b;
    ORDOBJ *o;

    gv_GetTableRecord(10, rec);
    for (k = 0; k < 6; k++) {
        if (out->kind[k] == 0)
            break;
    }
    if (k >= 6)
        return 0;
    cnt = ORD_CHARS[(unsigned short)id].wage;
    x = rec[6] * 0.01f;
    x = x * cnt;
    if (!(x > 0.0f))
        x = 0.0f;
    n = (int)x;
    if (cnt > 0) {
        out->kind[k] = 2;
        out->amount[k] = n;
        out->rate[k] = rec[6];
        if (flags & 1) {
            b = (ORDBLD *)gm_FirstBuilding(1);
            o = (ORDOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
            cm_SendMoveGoodsUnchecked(o->objId, ORD_CHARS[(unsigned short)id].netId, 0, n);
            gm_NetMsgCount++;
        }
        if (flags & 2)
            gm_IncomeRows[1][16] += n;
    }
    return 1;
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
    x = rate * 0.01f;
    x = x * base;
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
/* 0x4fa3d0, income kind 5 of the character id: the rate of the record 0xb percent of the character's wage at
   0x19c into the first free entry of out; flag 1 sends the amount from the character to all players (-1), flag 2
   adds it to the income sum of row 4 */

int gm_IncomeKind5(unsigned id, GMINCOME *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(0xb, rec);
    for (k = 0; k < 6; k++) {
        if (out->kind[k] == 0)
            break;
    }
    if (k >= 6)
        return 0;
    cnt = ORD_CHARS[(unsigned short)id].wage;
    x = rec[6] * 0.01f;
    x = x * cnt;
    if (!(x > 0.0f))
        x = 0.0f;
    n = (int)x;
    if (cnt > 0) {
        out->kind[k] = 5;
        out->amount[k] = n;
        out->rate[k] = rec[6];
        if (flags & 1) {
            cm_SendMoveGoodsUnchecked(-1, ORD_CHARS[(unsigned short)id].netId, 0, n);
            gm_NetMsgCount++;
        }
        if (flags & 2)
            gm_IncomeRows[4][16] += n;
    }
    return 1;
}
/* 0x4fa4b0, collects the incomes of the character id into the record out (cleared first): none (0) for a free
   record, one not in use (health 0) or one under age (gm_IsUnderAge); else the kind 1 or 2 (2 when the title group
   of the character's job has a town object, amt_HasTownObject), 5, 3 (only when v1a8 is positive) and 4. Returns 1
   @deep near-byte sweep s22: the zero fill before i (loop / memset), id &= 0xffff: 36-45 ins (the frame of
   the prologue moves the wrong way); memset after i = control. Callees IncomeKind1/2/5 now byte: no change
   here. prior: Read whole against the original: statement structure equal (branch targets shift only by
   the one-instruction size difference). Tried: index loop vs memset (identical code), the index
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
        gm_IncomeKind2(id, out, flags);
    else
        gm_IncomeKind1(id, out, flags);
    gm_IncomeKind5(id, out, flags);
    if (ORD_CHARS[i].v1a8 > 0)
        gm_IncomeKind3(id, out, flags);
    gm_IncomeKind4(id, out, flags);
    return 1;
}

struct AMTKIND; int amt_GetKind(int k, struct AMTKIND *out);                   /* 0x4e6f80 */
float gm_AddHealth(unsigned short id, int d);  /* 0x4fca40 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
float gm_Mix(int a, int b, int c);                     /* 0x504e20 */
void ai_BeginNeedBlock(struct AICHAR *p);                     /* 0x4c3e70 */
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
    rate = 0xe - rec.cur;
    rate = gm_PlayerFactor(rate, ai_OwnPlayer);
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
   @deep deep s33: the hunk is the add's destination of (r + kind.level): the original adds into r's
   register (and eax,0xffff / and ecx,0xff / mov edx,netId / add eax,ecx), ours into level's (add ecx,eax),
   and the netId load and the next call's registers rotate from it. The same pattern as ai_HatredWindow's
   worth sum (the original's destination is the operand computed first, ours the later one). Tried:
   kind.level + r, r * 0x1900 + kind.level * 0x1900 (= control), r += kind.level as statements (21) or in
   the call's statement (23), the whole roll inline (18). prior: Read whole against the original and
   rewritten (ORDWAGES / ORDCHAR / AMTKIND / AIDEC typed): the kind chain into a local, the index form
   ORD_CHARS[id] (the original folds ai_Chars into the displacements), the wage order job2 / job / kind,
   the money delta of AMTKIND.level (was the column), the random extra via a temp. 207 = 207 instructions,
   statement structure equal (the je/jae targets differ by the one byte of the end). Remaining: hunk
   0x4fa95e (code 6) is the same three instructions reordered with the commutative add's destination
   swapped: ours add ecx,eax / lea eax,[ecx+ecx*4] / mov ecx,[esi+netId], the original mov edx,[esi+netId]
   / add eax,ecx / lea eax,[eax+eax*4]; hunk 0x4fa974 (regs 14) is the rotation eax->ecx ecx->edx edx->eax
   that follows from it through the two sends. Tried (r + level) and (level + r), an int and an unsigned
   short temp, the one-expression form: no change. Allocation / scheduling of the kind-3 tail only. */

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

void gm_PayAllCharacterWages(void)
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
        ai_BeginNeedBlock(c);
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
   and, for the kinds 11 .. 13, its daySum. */

void gm_EndDayIncomes(unsigned flags)
{
    ORDBLD *b;
    int i, n;
    GMBTYPE *bt;
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
                bt = GM_BTYPE(b->type);
                levels[n] = bt->level;
                /* @original the level is stored twice: VC6 drops the repeated store after allocation and
                   leaves its address (a dead lea ecx,[ebx+ecx*4]) behind, as the original has it */
                levels[n] = bt->level;
                u.list[n] = b;
                n++;
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
   placed after the stores instead of before. Tried: ai_Time.year > gm_RichestYear (worse), if/goto layouts. */

int gm_FindRichest(short *id, int *worth)
{
    int best, bi, i, v;

    best = -100000;
    bi = 0xffff;
    if (gm_RichestChar == 0 || gm_RichestYear < ai_Time.year || gm_RichestChar->id == 0xffff
        || gm_RichestChar->health == 0) {
        gm_RichestYear = ai_Time.year;
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
   out[2] is the partner: the tavern run handler he_RunTavern passes its hired helper there (0 for none). */

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
/* 0x4fb0e0, every fourth year (ai_Time.year % 4 == 0), over the characters in use of kind 1 .. 9 but not 6 / 7 / 8 and
   w00a >= 12: when the decision 2's value is 2, each whose loyalty is below a random 0 .. 0.2 flips its b00c and gets
   0.25 .. 0.75 added to the loyalty; else each whose b00c differs from (value != 0) is accused (gv_Accuse 2) and
   loses 0.1 loyalty while above 0.1. Pumps the net every 0x40 messages. */

void gm_ShiftLoyalty(void)
{
    AIDEC rec;
    int t, m, i, faith, other;
    ORDCHAR *c;
    unsigned flip;

    if (ai_Time.year % 4 != 0)
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
int gm_IconNameCount = 0;  /* 0x6f1b90, the number of icon names */
char * gm_IconNames = 0;  /* 0x6f1b94, the icon names ( */
int gm_NetMsgCount = 0;  /* 0x6f1b98, the number of queued messages ( */
float ai_RatingBase = 0;  /* 0x6f1b9c, the running rating (it moves towards ai.h's ai_Rating) */
float ai_Rating = 0;  /* 0x6f1ba0, the smoothed game rating (statistics out[5]; the running rating ai_RatingBase follows it) */
int gm_UnreadBss[2] = {0};  /* 0x6f1ba4, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
ORDCHAR *gm_RichestChar = 0;  /* 0x6f1bac, the richest character of gm_FindRichest's last search, 0 = none */
int gm_OwnNameHeadLast = 0;  /* 0x6f1bb0 */
void gm_ResolveHeadIcons(GMHEAD *rec);
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
   @deep size queue s18 (raw 576 vs 518; int k 481, ushort k 501: both too short for the padded 528).
   c2trace corrects the earlier reading: p (prio -7, 15 refs, live over the whole body) loses ebp to v (the
   picture, prio 14) and then esi to k; the TitleHeadsMale constant is no cause (probe: the fallback with
   another table: same count). The original: v in edi, p in ebp, k in bl, the boy path returning gm_BoyHead
   in eax through the final epilogue (je behind mov eax,ebp). No lever this round: child arm as two returns
   (c1/c2/c3) x k uchar/int x p/i init order (12 variants: 103-135), v not a local (picture read in the
   condition and again in the stores: = same count), climb --rounds 1 (best 125 via an
   assignment-in-expression for gm_OwnNameHeadLast, not applied). Next: what gives p a priority above v
   (fewer blocks where p lives unreferenced: p assigned only in the job / title / fallback paths, the
   own-name and spy paths returning before p is set). prior: 129 ins, unchanged; the q walks are plain p[i]
   index loops now (same code). headers sample: insensitive. Read: the original has no frame: p in ebp (0
   at entry), i in edx (0 at entry, shared by both loops), k in bl (unsigned char: test bl,bl and the
   per-compare mov ecx,ebx / and ecx,0xff), the ownName address in esi and the picture in edi (add
   edi,-0x639), the child path returns gm_BoyHead in eax through the final epilogue. Ours spills p and k
   (frame 8) because the gm_TitleHeadsMale address, used twice, takes ebx as a constant candidate (c2trace:
   k's two webs prio -2, refs 1, the compares go through hoisted temps). Tried: k as int / unsigned / char
   / unsigned short (int: no frame, p in ebp, 103 ins, but the original's byte loads and test bl,bl say
   unsigned char), p = 0 / i = 0 / k orders (one code), the child arm returning both heads directly
   (108-135), the fallback returning the tables d... */

unsigned *gm_CharNameRecord(int id)
{
    GMHEAD *p;
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
            for (; i < 0x5f; i++) {
                if (p[i].key == 0)
                    break;
                if (p[i].key == k) {
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
            for (; i < 0x23; i++) {
                if (p[i].key == 0)
                    break;
                if (p[i].key == k) {
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
    unsigned v;
    CHARACTER *m;
    D3SURF *surf;
    D3OPS *ops;

    v = 0;
    if (o != 0 && (m = ORD_CHAR(o)->model) != 0) {
        surf = (D3SURF *)m->obj->material->animCtl;
        ops = (D3OPS *)surf->info.mesh;
        if (m->scene != d3_Scenes) {
            if ((int)ops->nsets <= 4)
                v = ORD_CHAR(o)->netId % ops->nsets;
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
            if (stricmp(icon, gm_IconNames + i * 0x30) == 0) {
                *out = i;
                goto next;
            }
        }
        sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
        *out = 0xff;
        for (i = 0; i < gm_IconNameCount; i++) {
            if (gm_IconNames[i * 0x30] == 0)
                break;
            if (stricmp(icon, gm_IconNames + i * 0x30) == 0) {
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
            if (stricmp(icon, gm_IconNames + i * 0x30) == 0) {
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
                        if (stricmp(icon, gm_IconNames + j * 0x30) == 0) {
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
                    if (stricmp(icon, gm_IconNames + j * 0x30) == 0) {
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
   again at the end. Returns the model (0 when scene is negative). */

int gm_CreateCharModel(unsigned short *c, int scene, int *p, int *ori, int obj, char *dummy)
{
    int saved;
    D3VECTOR4 zero;
    int pos[4];
    int *o, *src;
    D3OBJECT *it;
    GMHEAD *rec;
    char *name, *s, *d;
    CHARACTER *m;

    saved = d3_CurrentScene;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    zero.w = 0.0f;
    src = p != 0 ? p : (int *)&zero;
    o = ori != 0 ? ori : (int *)&zero;
    if (scene < 0)
        return 0;
    d3if_SelectScene(scene, 1);
    it = 0;
    if (dummy != 0)
        it = d3_FindObject(obj, 0x100, 0, dummy);
    if (it == 0 && src == 0 && o == 0)
        goto done;
    if (it != 0)
        d3_PointToWorld(it, &it->pos, (D3VECTOR *)pos);
    else {
        pos[0] = src[0];
        pos[1] = src[1];
        pos[2] = src[2];
    }
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
int gm_MayRemoveChar(ORDCHAR *c);                     /* 0x5075c0 */
void cm_SendKillCharacter(int a, int b);                     /* 0x4c4e00 */
int gv_DecayRaisedDead();                     /* 0x4f1660 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
int cm_SendFieldValues(void);                     /* 0x4c49e0 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
int cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
void gm_PartnerStep(ORDCHAR *c);                 /* 0x4fccc0 */

/* sends the message 0x72 (the character c died) in the GMMSG m */
#define GM_SENDDEATH(m, c)                                             \
    memset(&(m), 0, sizeof(m));                                        \
    (m).type = 0x72;                                                   \
    (m).to = (c)->netId;                                               \
    (m).fromId = -1;                                                   \
    (m).flags = 2;                                                     \
    (m).time = ai_Time;                                               \
    (m).arg0 = (m).to;                                                 \
    cm_SendLongMessage((char *)&(m))

/* 0x4fbe90, the daily step of the character c; returns its new life value (-1 for no step). A free slot and a
   character of kind 0xf take no step (the latter is killed when gm_MayRemoveChar lets it go); a raised dead
   person (kind 0x14) decays (gv_DecayRaisedDead) and, used up, dies: the message 0x72, and the news 0x414a to
   the master of its building when that is a player (kind 6 / 7). Every other character halves the counters of
   v02c, then: one marked to die (flags1cc bit 2) loses 0x11 + random 0x10 of its life f01c and dies below 0
   (news 0x1c51); else its life moves toward f010 while it is younger (w00a) than f020, beyond that it loses f014
   less 3.75 % per point of the building skill 8 of its home, and when f01c + v024 is used up it dies (news
   0x1c51, or 0x4080 with its name and age for a flags1cc bit 4 one) - unless flags1cc bit 1 saves it (not a
   player): life 100, v024 0, flags1cc 2. A survivor runs gm_PartnerStep and ages by one. */

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
        if (gm_MayRemoveChar(c) != 0) {
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
        if (c->bld2 != 0) {
            float p;   /* 3.75 % less per skill point */

            p = gm_BuildingSkill((unsigned char *)c->bld2, 8, 1) * 0.0125f;
            p = p * -3.0f;
            lo += p;
        }
        t = lo / c->f01c;
        t += c->f01c - lo;
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
        if (a <= ch->f020) {
            v = ch->f018;
            cm_SendFloatDelta(ch->netId, 0x1c, v - ch->f01c);
        } else {
            v = ch->f018;
            k = ch->f014;
            n = (int)(a - ch->f020);
            if (n > 0) {
                do {
                    n--;
                    v = v - k + (k / v);
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
    float k, v, r, t;
    int n;

    n = 0;
    v = ch->f01c;
    k = ch->f014;
    while (v > 1.0f) {
        t = k / v;
        v = v - k + t;
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

extern float gm_BirthChance[6];
void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);                     /* 0x4fe140 */
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
    ORDCHAR *o, *mate, *child;
    int arg, t, kids, n, m, k, h;
    float fv;
    char kind;

    if (c->kind >= 10 || c->health == 0 || c->textVariant != 1 || c->b1b1 > 0 || (c->flags1c8 & 0x40000) != 0)
        return;
    o = ORD_CHAR(gm_FindChar(c->partner));
    if (c->pregnancy > 1.0f) {
        AITIME clk;

        cm_SendFloatDelta(c->netId, 0x7c, util_RandomF() * -0.5f - c->pregnancy);
        gm_NetMsgCount++;
        mate = 0;
        if (o == 0)
            return;
        clk = ai_Time;
        arg = 2;
        gm_AddTime((struct AITIME *)&clk, 8, 0, 0);
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
            G28REC party;

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
                int kind = 3, level = 2;
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldValue(1, 1, (char *)&c->kind - cm_FieldObject, &kind);
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
                int office = util_RandomU(0xc) + 10;
                nc = ORD_CHAR(cm_GetSlotValue(h));
                cm_SendMoveGoodsUnchecked(nc->netId, -1, ai_OwnPlayer, (util_RandomU(0x400) + 0x400) * 0x40);
                cm_BeginFieldBlock((char *)nc, nc->netId);
                cm_AddFieldValue(1, 1, (char *)&nc->prevOffice - cm_FieldObject, &office);
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
#define gm_CharStats (*(GMCHARSTATS *)&gm_CharacterStatistics)  /* 0x91b500, gm_CharacterStatistics as the record gm_CountCharacters fills: its fields read through the object (a GMCHARSTATS * local compiles baseHealth[k] += .. without the original's lea edx,[edx*4+X] before the call) */
unsigned short gm_CreateCharacter(int type, int a2, int a3, unsigned short a4, ORDBLD *a5, unsigned a6, int a7, int a8);                     /* 0x507690 */
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
    if ((float)ORD_CHARS[id].w00a < ORD_CHARS[id].f020)
        return 1;
    return 0;
}
/* 0x4fd820, finds a home with room for the building o's people: among the buildings of the query (1, 5, 1) with
   fewer people (+0x65) than their kind's capacity (ord_GetKindRecord [0]) and the flag 0x5a bit 0 clear, the
   nearest to o when o and the building both have a scene object, else the least full one (people / capacity);
   none: a random one of the first 16 of the query (1, 5, 0x16) with the flag clear. The first parameter is
   not read
   @deep size queue s16: the escape lever (kind / v1 / v2 / d declared in the blocks that use them, all 7
   subsets) is the control's code; a k = kind[0] scalar gives the original's candidate set (o loses its
   register and is reread from the argument as in the original) but the rotation pick edi / cap ebp / k ebx
   against the original's ebp / ebx / edi (88 ins); unsigned short k = control. prior: draft 90 -> 67 ins.
   Read: the original keeps the pick's capacity in ebx (loaded from the first record after the query test)
   and kind[0] in edi across the d3_PointToWorld calls, stores only the record's second int at a pick, and
   shares the int-to-float temp with the first record's first int (frame 0x74, the 8-byte record slot
   shared with a 4-byte temp, which no variant reproduces: c2frame gives the temps their own 4-byte slot).
   Kept: a scalar cap for the pick's capacity (cap = pickKind[0] after the query; cap = kind[0] at each
   pick): 90 -> 67. Tried: a def[2] copied into a scalarized pickKind (75, the [1] stores vanish), struct /
   unsigned short[4] records and struct copies (= control), cap scalars for kind[0] too (82-99), the init
   in the if / before the query, all 24 orders of the pick block (best 88 with the kind scalar). Left: the
   rotation pick edi / cap ebp / kind[0] ebx against the or... */

ORDBLD *gm_FindHome(int unused, ORDBLD *o)
{
    float best, d;
    ORDBLD *b, *pick;
    int pickKind[2], kind[2];
    int cap;
    D3VECTOR4 v1, v2;                   /* the engine's 16-byte vectors */
    ORDBLD *list[16];
    int n;

    pick = 0;
    best = 1e+08f;
    ord_GetKindRecord(1, pickKind);
    b = (ORDBLD *)gm_BuildQuery(1, 5, 1);
    cap = pickKind[0];
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
                        cap = kind[0];
                        pickKind[1] = kind[1];
                        pick = b;
                        best = d;
                    }
                } else if (pick == 0) {
                    cap = kind[0];
                    pickKind[1] = kind[1];
                    pick = b;
                } else if ((float)b->people / (unsigned short)kind[0] < (float)pick->people / (unsigned short)cap)
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
/* 0x4fda10, makes the building b the home (bld2) of the character c when b still has room for one more
   inhabitant (the kind's capacity, ord_GetKindRecord): b gains an inhabitant, the old home loses one */

void gm_MoveIntoHome(char *c, char *b)
{
    unsigned short cap[4];
    ORDCHAR *ch = ORD_CHAR(c);
    ORDBLD *bld = (ORDBLD *)b;
    ORDBLD *old;

    ord_GetKindRecord(bld->type, (int *)cap);
    if (bld->people < cap[0]) {
        old = ch->bld2;
        if (old != 0 && old->people > 0)
            old->people--;
        ch->bld2 = bld;
        bld->people++;
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
/* 0x4fddb0, creates a character: flags bit 0 = gm_CreateRandomChild(age), else gm_CreateCharacter(type, -1, -1, age, 0, job, title, 2); bit 1 (random child only): sets its jobId / title; bit 2: places it on the stage of the building bld at its gm_HomeObject. Returns the character or 0 */

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
    if (flags & 4) {
        if (bld != 0 && (o = (ORDOBJ *)gm_HomeObject((unsigned char *)bld)) != 0)
            gm_PlaceOnStage(c->netId, ((ORDBLD *)bld)->id, o->objId, 0, 0);
        return (short *)c;
    }
    return (short *)c;
}
/* 0x4fde90, loads the icon names: counts the 0x80-byte records of "<root>Character_Names.dat" (0x6f1b90 of them), allocates 0x30 bytes per name (0x6f1b94) and copies each name; resets the three flag bytes at 0x6f0454 */

void unk_LoadIconNames(void)
{
    char path[260], rec[260];
    char *s, *d;
    FILE *f;
    char ch;

    d2_TransparentBytes[1] = (char)0xff;
    d2_TransparentBytes[0] = 0;
    d2_TransparentBytes[2] = 0;
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
    gm_IconNames = (char *)mem_Alloc(gm_IconNameCount * 0x30, "tmp");
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

/* 0x4fdfd0, fills the character statistics gm_CharacterStatistics: the living persons of kind < 10, per age bucket (gm_CharAge / 10, clamped to 0 .. 7) their count, health in percent of the base health and average base health, and the share under age in percent */

void gm_CountCharacters(void)
{
    ORDCHAR *c;
    int i, k;

    memset(gm_CharacterStatistics, 0, sizeof(GMCHARSTATS));
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id != 0xffff && c->health > 0 && c->kind < 10) {
            gm_CharStats.count++;
            k = (int)GM_MAX(GM_MIN(gm_CharAge(i) * 0.1f, 7.0f), 0.0f);
            gm_CharStats.ageCount[k]++;
            if (gm_IsUnderAge(i) != 0)
                gm_CharStats.underAge += 1.0f;
            gm_CharStats.health[k] += gm_CharacterHealth((short *)c);
            gm_CharStats.baseHealth[k] += gm_CharacterBaseHealth((short *)c);
        }
    }
    for (k = 0; k < 8; k++) {
        gm_CharStats.health[k] = gm_CharStats.health[k] / gm_CharStats.baseHealth[k] * 100.0f;
        gm_CharStats.baseHealth[k] = gm_CharStats.baseHealth[k] / (float)gm_CharStats.ageCount[k];
    }
    gm_CharStats.underAge = gm_CharStats.underAge / (float)gm_CharStats.count * 100.0f;
}


/* a job record (ai.h AIJOB, he_NextEvent's list) as gm_SellBuilding and gm_FreeObject stamp it */
#pragma pack(push, 1)
typedef struct GMJOB {
    char pad00[0x44];
    AITIME start;                       /* 0x44, the start time (gm_UpdateTechTree's progress) */
    AITIME end;                         /* 0x52, the end time (ai.h AIJOB.end) */
    char pad60[0x18];
    unsigned char opts;                 /* 0x78, the event's options (ai.h AIJOB.opts); bit 2 = posted when due */
    char pad79[0xb];
    int msgSlot;                        /* 0x84, the slot handle of its HE message (cm_SendExSetHE) */
    char pad88[0x24];
    unsigned short good;                /* 0xac, of an order (event kind 2): the goods it makes */
    char padAe[2];
    int objId;                          /* 0xb0, of an order: the ORDOBJ.objId of the workshop it is for */
    int research;                       /* 0xb4, of a research job (event kind 0xc): the goods researched */
} GMJOB;
#pragma pack(pop)

extern int gm_ClassBuilding[32];
extern int gm_GoodBuilding[732];
extern char * ai_Goods;
extern unsigned gm_OwnNameHeads[136];
extern struct AIOBJ *ai_Objects;
extern int gm_DefaultStock[];
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void util_Randomize();                     /* 0x42ec80 */

/* dcp unit split: the functions of f3gmt the rest of f3gm uses */
void gm_SyncMarket(unsigned p);
int gm_GoodsDemand(int p, int i);
int gm_GoodsStock(int p, int i);
int gm_FreeList(int *list);
int gm_ListBuildingsOfType(unsigned t, int *out);
int gm_CharSumAmounts(char *c);
unsigned gm_TreeNodeRank(int p);
int gm_LayoutTree(unsigned char *bld, unsigned short *item, int w, int h);
/* dcp unit split: the declarations that moved to f3gmt (copies: drop what f3gm does not use) */
extern int f3gm_ObjectCount;
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
int gm_RemoveObject();                     /* 0x4ffff0 */
int gm_FindById(int *bld, int *obj, int *chr, int id);
int gm_GoodsStock(int p, int i);                   /* 0x4ff950 */
int gm_GoodsDemand(int p, int i);                   /* 0x4ff8c0 */
float gm_ItemPrice(int item, unsigned char n); /* 0x5087f0 */
int gm_FreeList(int *list);                     /* 0x500140 */
void cm_SendGoodsDelta(struct GMSTOCK *p, unsigned char k);                     /* 0x4c5710 */
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);                     /* 0x4c4880 */
void cm_SendGoodsRecord(void *p, unsigned char k);                     /* 0x4c56d0 */
void cm_BeginSendGroup(void);                     /* 0x4c3b30 */
void cm_EndSendGroup(void);                     /* 0x4c3b40 */
int cm_SendGroupState();                     /* 0x4c3b50 */
int gm_FindPlayerBuilding(int p);                   /* 0x501000 */
void gm_ClockToDate(struct AITIME *c, unsigned char *out);                     /* 0x4fe2c0 */
void gm_SyncMarket(unsigned p);
int gm_GoodsDemand(int p, int i);
int gm_GoodsStock(int p, int i);
extern int gm_QueryType;
extern int gm_QueryOwner;
extern unsigned short gm_QueryOwnerNo;
#define ALIGN1 __declspec(align(1))
extern ALIGN1 char gm_QueryKind[550];
extern char gm_QueryFirst, gm_QueryScanAll, gm_QueryChildren, gm_QueryStopFirst;  /* 0x6f1be0, 0x6f1be8, 0x6f1be9, 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
extern int gm_QueryScanned;
extern ORDOBJ *gm_QueryCursor;
extern int gm_QueryDepth;
extern int gm_QueryChildStack[20488];
int gm_FindMarketObject(int id);                     /* 0x500ec0 */
int gm_FindPlayerBuilding(int p);                   /* 0x501000 */
extern struct AIBLD *ai_MarketBuilding;                /* 0x6f1bc8, the market building (group 1, 5, 10) */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
extern unsigned short gm_BQMaster, gm_BQOwner;  /* 0x61a674 / 0x61a676 */
extern char gm_BQStopFirst, gm_BQFirstCall; /* 0x6f1bd4, 0x6f1bd5, stop at the first, first call */
extern unsigned gm_KindObjs[732];
extern unsigned short gm_KindObjCount;
void rt_FreePoints(struct RTPOINT *p);                     /* 0x4bd9e0 */
extern int gm_NOffers;
int gm_ListStock(short *l, int id);     /* 0x50ab50 */
int gm_ListAmount(short *l, char *o);   /* 0x50aaf0 */
float gm_GoodsPrice(unsigned id, unsigned char p);                   /* 0x5089c0 */
unsigned short * gm_ObjectBuildingOwner(unsigned short *p);                     /* 0x50a050 */
void init_HookObject(struct D3OBJECT *o, void *p, int kind);                     /* 0x50d380 */
struct CHTRANSPORT * ch_AttachTransport();                     /* 0x4034e0 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */
struct CHARACTER * ch_Create();                     /* 0x403390 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
int gm_IsHomeType();                     /* 0x502520 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
int gm_IsHomeType(unsigned char *b);
int gm_IsType10(unsigned char *b);
int gm_BuildingClass(int kind);
int gv_BuryNewDead();                     /* 0x4ef9c0 */
void *gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */
int gm_BuildingClass();                     /* 0x501d80 */
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
unsigned short * gm_AllocRaum(unsigned char *b, unsigned n);
int gm_BuildingJob(int t);                   /* 0x5048e0 */
int gm_ListBuildingsOfType();                     /* 0x5020a0 */
extern int gm_AllowAll;
int gm_JobBuildingType(int x);
extern unsigned short * sv_PanelShown;                /* 0x6f2310 */
extern int ai_CharCount, ai_NextCharNumber, ai_NextPlayerNumber;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                     /* 0x447300 */
void cm_BeginGroup(const char *s);                     /* 0x4c4540 */
int cm_EndGroup(const char *s);                     /* 0x4c4580 */
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
void cm_SendBoss(int a, int b);                     /* 0x4c5540 */
int cm_SendExSetHE(unsigned char k, char *p);                     /* 0x4c4c60 */
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);                     /* 0x53dee0 */
int d3t_SceneIndex();                     /* 0x411be0 */
int ch_Delete();                     /* 0x4028a0 */
int gm_ObjectBuilding(int y);                     /* 0x50a0f0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);                     /* 0x524260 */
void he_EndPersonEvents(unsigned short *p);                     /* 0x5538b0 */
void he_EndKind23Events(int ev);                     /* 0x568aa0 */
void d3io_NoopCallback();                     /* 0x44ee40 */
int amt_ClearOffices();                     /* 0x4e6d80 */
void gm_KillSpieler(int c, int mode);                     /* 0x503c60 */
extern struct D3SCENE *d3_DefaultScene;  /* 0x5fb5b0 (as d3.h / anm.h) */
extern int ai_Evidence[0x800][2];              /* 0x91b6e0 .. 0x91f6e0: pairs of ints */
extern int ai_Unk770ae0_day;            /* the clock's day (0x770ae0) */
#ifndef TIME_VIEW_H
#endif
extern char ai_Unk770ce4[];             /* 0x770ce4: the field +0x164 of the character records (stride 0x21c) */
void sv_RemoveBanners(char *b);                     /* 0x5286a0 */
void sv_CreateBanners(char *b);                     /* 0x5282f0 */
void ch_InflateModel();                     /* 0x402540 */
int ch_Delete();                     /* 0x4028a0 */
void ai_ShowEvidenceVoidNews(unsigned short *toldRec, unsigned short *culpritRec);     /* 0x5a9430 */
int gm_FreeList(int *list);                     /* 0x500140 */
int gm_TitlesLeft(int n);                     /* 0x504ce0 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int gm_CompareTime();                     /* 0x4fe210 */
int gm_MinutesBetween();                     /* 0x4fe280 */
int d3t_SceneIndex();                     /* 0x411be0 */
double _CIfmod();                     /* 0x5de73a: a CRT call */
extern int ai_CharCount;  /* 0x6f1bc0 */
extern int f3gm_ObjectCount;  /* 0x6f1bdc */
int ai_PermissionBits1d();                     /* 0x587b70 */
extern int gm_AllowAll;                /* 0x6f2448 */
int gm_BuildingJob();                     /* 0x5048e0 */
int gm_TitleGroup(int n);                /* 0x504ad0 */
unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */
short * gm_PlayerRecord(char *c);                     /* 0x506430 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
int amt_GetKind(int k, struct AMTKIND *out);                     /* 0x4e6f80 */
float gm_AddHealth(unsigned short id, int d);                   /* 0x4fca40 */
void gm_ClockToDate(struct AITIME *c, unsigned char *out);                     /* 0x4fe2c0 */
int ai_InventoryHas(void *ch, int id);                     /* 0x5ba6b0 */
float gm_Liking();                     /* 0x50c550 */
void ai_RestartMission(struct G13MIS *m);                     /* 0x4b3a90 */
void he_RecheckPersonEvents();                     /* 0x53ed40 */
int gm_EquipmentValue(unsigned char *b, int level);                     /* 0x509b10 */
int gm_CharSumAmounts(char *c);                     /* 0x509eb0 */
int cm_SendMoveObject(int a, int b, int c);                     /* 0x4c5010 */
extern const unsigned int ai_BuildingTypeMasks[6];  /* 0x5eed70 (ai.h, not includable here): [2..3] the 64-bit mask of the house-owner building kinds */
void cm_SendFloatDelta(int id, int field, float delta);                     /* 0x4c4ac0 */
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
float util_RandomSq(void);                  /* 0x42ec40 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
int gm_BuildingClass();                     /* 0x501d80 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
int gm_CharSumAmounts(char *c);                     /* 0x509eb0 */
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
unsigned char * gm_BuildNext(void);                     /* 0x501030 */
extern int ai_Difficulty;
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
extern int ai_CharCount, ai_NextCharNumber, ai_NextPlayerNumber, gm_AllowAll, ai_NextObjectId;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc, 0x6f2448, 0x61a654 */
void hmt_Create(int type, int unk, unsigned short v, struct AICHAR *c);                     /* 0x4adbb0 */
void ai_ResetJob(struct AICHAR *c);                     /* 0x47e940 */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */
char * ai_FindWeaponRecord(int id);                     /* 0x4cd9d0 */
int gm_EquipmentValue(unsigned char *b, int level);
int gm_GetWorth(unsigned short id);
int gm_ListAmount(short *l, char *o);
void d2_PlotLine(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);  /* 0x4764f0 */
void cm_SendBuildingDestroyed(int a);                     /* 0x4c5510 */
float gm_Liking();
int gm_LayoutTree();
void gm_UpdateTechTree();
typedef struct {
    int id, p1, p2;
    unsigned short rank, pad;
    int x, y, z;
    int l[5];
    unsigned char flag;
} RTNODE;       /* 0x34 bytes, the nodes of the research tree layout 0x76fde0 */
extern int gm_TreeRanks;
unsigned gm_TreeNodeRank();                /* 0x50b2f0 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */
unsigned gm_BoyHead[] = { 0, 1735292266, 1430937445, 4540238, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118a8 */
unsigned gm_GirlHead[] = { 0, 1684365677, 1852139619, 1161907551, 1162363716, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118f0 */
unsigned gm_TitleHeadsMale[] = { 12, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1684955496, 1802659191, 1597272677, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1684955496, 1802659191, 1597141605, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1684955496, 1802659191, 1597207141, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1735550306, 1852727661, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701409360, 1919251571, 1414875999, 17748, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1601462881, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1650813284, 1431002932, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1650813284, 1312902495, 12878, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1969578354, 1735550306, 1818588005, 1398760812, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1969578354, 1819436386, 1819437157, 1600613993, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1751344486, 1601332596, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1802728813, 1852142693, 1969775475, 1702524005, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 1298084466, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 1298084210, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x611938 */
unsigned gm_TitleHeadsFemale[] = { 14, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1684955496, 1802659191, 1852404325, 1380343602, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1684955496, 1802659191, 1852404325, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1668505975, 1634887272, 1380343669, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 829319538, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x612288 */
unsigned gm_JobHeadsMale[] = { 33, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 76, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 77, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 78, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 79, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 80, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 81, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 83, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 84, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 85, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 86, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 87, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 88, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 82, 1936943479, 1668509285, 1952866664, 1601332588, 1414813003, 12869, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x612bd8 */
unsigned gm_JobHeadsFemale[] = { 33, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 2053926760, 1818583398, 1769104748, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191426, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x614518 */
unsigned gm_SpyHeads[] = { 0, 1869181043, 1162698606, 1297434948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x617398 */
char ord_KindTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 50, 0, 0, 0, 0, 0, 12, 0, 75, 0, 0, 0, 0, 0, 20, 0, 125, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 3, 0, 0, 0, 15, 0, 0, 0, 9, 0, 0, 0, 25, 0, 0, 0, 16, 0, 0, 0, 20, 0, 6, 0, 20, 0, 0, 0, 30, 0, 9, 0, 60, 0, 0, 0, 45, 0, 12, 0, 110, 0, 0, 0, 30, 0, 0, 0, 10, 0, 0, 0, 45, 0, 0, 0, 30, 0, 0, 0, 70, 0, 0, 0, 60, 0, 0, 0, 10, 0, 0, 0, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0, 25, 0, 0, 0, 60, 0, 0, 0, 30, 0, 3, 0, 15, 0, 0, 0, 45, 0, 6, 0, 50, 0, 0, 0, 80, 0, 9, 0, 120, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 0, 0, 15, 0, 0, 0, 50, 0, 0, 0, 35, 0, 0, 0, 75, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 100, 0, 0, 0, 20, 0, 0, 0, 30, 0, 12, 0, 30, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 8, 0, 20, 0, 0, 0, 50, 0, 10, 0, 40, 0, 0, 0, 75, 0, 12, 0, 80, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 4, 0, 25, 0, 0, 0, 50, 0, 5, 0, 40, 0, 0, 0, 80, 0, 6, 0, 70, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };  /* 0x617468, the kind records ord.c reads (ord_GetKindRecord, ord_ComputeStatistics): f3gm's by place, in its .data block between gm_SpyHeads and gm_RichestYear (the name keeps the users' prefix) */
int gm_RichestYear = -1;  /* 0x61772c, the game year (ai_Time.year) of gm_FindRichest's last search */
int gm_RichestWorth = -1000000;  /* 0x617730, the worth of gm_RichestChar (0xc80 when none was found) */
float gm_BirthChance[6] = { 0.240015402f, 0.222200006f, 0.125f, 0.0555500016f, 0.0138499998f, 0.0f };  /* 0x617734, a float per number of children / relatives */
extern int gm_PlayerRecords[688];
extern int gm_ClassBuilding[32];
char * ai_Blds;  /* 0x889f74, the buildings ( */
signed char ai_RelationMemory[768][768];  /* 0x889f80 */
int gm_GoodBuilding[732];  /* 0x919f80 */
char * ai_Goods;  /* 0x91aaf0, the object prototypes ( */
extern unsigned gm_OwnNameHeads[136];
extern char gm_CharacterStatistics[128];
extern char d2_GfxDir[256];
