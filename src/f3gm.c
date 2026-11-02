/* The game data unit (0x4f9e30 .. 0x50d0b0; 0x50d100 starts f3init.c, tag "f3_init:"): name from the allocation
   tags "f3_gm:lpGebaeudePROT" / "f3_gm:lpObjekteINST" of the building and object tables (0x4fe570). One .data
   string block runs through it in function order (0x61774c "misc\statistics_taxes" .. "%1G %A" 0x61ac98 ..
   "_NACHRICHTEN_HS_67" 0x61ade0), so the character income / statistics code, the game clock, the building and
   object tables, the job / title tables, the character helpers, prices and worth and the standing / family
   helpers are one unit.
   Open: many callees are still declared K&R in this file (amt_HasTownObject must stay K&R: the typed prototype
   costs gm_Unk4fa4b0 a byte).
   Open: gm_GetWorth is a float in gv.h, the unit reads it as an int (I50A640).
   Open: gm_GetGebaeudeHauptraum (0x501ce0) is drafted void, but its callers use the result as a pointer.
   Open: not included yet (dcp headers conflicts f3gm HEADER): ai.h anm.h ch.h cm.h d2.h d3_engine.h
   d3_interface.h d3_tools.h gfx.h hmt.h hst.h rt.h unk_458810.h unk_4b39f0.h; held by own declarations that
   differ from the owner's: d2_Objs, gm_MinutesBetween, gm_ClockSeason, gm_CompareTime; calls whose argument
   count differs from the definition (read the original's pushes): d3if_SelectScene, gm_Liking, gm_TitleNumber,
   d3t_SceneIndex.
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

#include "am_types.h"
extern G7GRP gm_BuildingItemGroups[];           /* 0x61a348 */
/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "d3_types.h"
extern D3SCENE d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it */
extern D3SCENE d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it */
extern D3SCENE d3_Scenes[];               /* 0xfd7ba0: ai_Unk5fb5b0 points at it */
extern GAPDATA full_data_91f6e0[];
extern unsigned char he_Events[339968];
#include "amt_types.h"
extern GAPDATA full_data_7585f4[];

#include "ai_types.h"
extern AISTORE gm_Offers[];            /* 0x7e4b20 */
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
int gm_TitleJob(int i, int p);
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
#define gm_IncomeRows (*(int (*)[5][17])&gm_IncomeHistory)  /* 0x91b580, the five rows of 17 ints; [row][16] is the current sum */

int gm_GetGoodsValue(char *c);              /* 0x509e60 */
int gm_TitleGroup();                     /* 0x504ad0 */
char * gm_PlayerSeat(int p);                     /* 0x504a80 */
int gm_PlayerFactor(int a, unsigned char k);                     /* 0x5085f0 */
int unk_Unk4fd7e0();                    /* 0x4fd7e0 */

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
void *ai_Unk5a8560(int param_1);  /* 0x5a8560 */
#define ai_Unk5a8560K ((int (*)())ai_Unk5a8560)  /* 0x5a8560: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
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

int unk_Unk4f9f60(unsigned id, char *out, unsigned char flags)
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
                    ai_Unk6f1b98++;
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

int unk_Unk4fa060(unsigned id, char *out, unsigned char flags)
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
                    ai_Unk6f1b98++;
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
/* 0x4fa2a0, income kind 4 (the record 0xc, the sum of the ints at +0x1a0 and +0x1a4; sum 0x91b68c). ROUGH */

int unk_Unk4fa2a0(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;
    char *c;

    gv_GetTableRecord(0xc, rec);
    gm_Query(*(int *)((char *)gm_FirstBuilding(1) + 0x5d), 2, 6, 0, 0x115);
    c = CH(id);
    cnt = *(int *)(c + 0x1a4) + *(int *)(c + 0x1a0);
    x = (float)(rec[6] * 0.01f * *(int *)(c + 0x1a4));
    if (!(x > 0))
        x = 0;
    n = (int)x;
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            if (cnt > 0) {
                out[k] = 4;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                *(int *)(out + 8 + k * 4) = n;
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b68c += n;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4fa3d0, income kind 5 (the record 0xb, the int at +0x19c; sum 0x91b6d0); the message goes to all players (-1). ROUGH */

int unk_Unk4fa3d0(unsigned id, char *out, unsigned char flags)
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
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b6d0 += n;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4fa4b0, collects the incomes of the character id into the 0x38-byte record out (cleared first): 0 when the slot is free / not active (+8) / 0x4fd7e0 says so; the incomes 1 or 2 (by 0x4e9400 of the character's job at +0x164), 5, 3 (only when +0x1a8 > 0) and 4. Returns 1 */

int unk_Unk4fa4b0(unsigned id, int *out, unsigned char flags)
{
    char *c;
    int i;

    c = CH(id);
    for (i = 0; i < 0xe; i++)
        out[i] = 0;
    if (*(short *)c == -1)
        return 0;
    if (c[8] == 0)
        return 0;
    if (unk_Unk4fd7e0(id) != 0)
        return 0;
    if (amt_HasTownObject(gm_TitleGroup(*(int *)(c + 0x164))) == 0)
        unk_Unk4f9f60(id, (char *)out, flags);
    else
        unk_Unk4fa060(id, (char *)out, flags);
    unk_Unk4fa3d0(id, (char *)out, flags);
    if (*(int *)(c + 0x1a8) > 0)
        gm_IncomeKind3(id, (GMINCOME *)out, flags);
    unk_Unk4fa2a0(id, (char *)out, flags);
    return 1;
}

struct AMTKIND; int amt_GetKind(int k, struct AMTKIND *out);                   /* 0x4e6f80 */
float gm_AddHealth();                  /* 0x4fca40 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
float gm_Mix(int a, int b, int c);                     /* 0x504e20 */
void ai_BeginNeedBlock(char *p);                     /* 0x4c3e70 */
int ai_AddNeedDelta(int k, float v);                     /* 0x4c3eb0 */
void ai_SendNeedBlock(void);                     /* 0x4c4a20 */
void unk_Unk4fb0e0();                   /* 0x4fb0e0 */
int gm_FindRichest(short *id, int *rate);
#define I50A640 ((int (*)())gm_GetWorth)

/* 0x4fa580, the daily upkeep of the characters (kind 0, active, with a building at +0x170): rate = 0x5085f0 (0xe - the record 5 rate, 0x6f1bc5); a character whose 0x509e60 is negative: with a master (+0x16c) 0x4c4b00 (-10) tells the master player; without a master and partner (+0x5c = -1) and a debt above twice the rate 0x4fca40 (i, -5000); else (or not negative) a building of kind 1 gets 0x4c4830 (the rate) for the player. Flushes the messages every 0x40. ROUGH */
void unk_Unk4fa580(void)
{
    char *p;
    char *b;
    char *rec;
    int rate, i, v, d;
    int r[9];

    rec = (char *)gm_PlayerSeat(1);
    gv_GetTableRecord(5, r);
    rate = gm_PlayerFactor(0xe - r[6], ai_OwnPlayer);
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        p = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (*(short *)p != -1 && p[8] != 0 && p[2] == 0 && *(unsigned char **)(p + 0x170) != 0) {
            b = ai_BTypes + **(unsigned char **)(p + 0x170) * 0x24d;
            v = gm_GetGoodsValue(p);
            if (v < 0) {
                if (*(int *)(p + 0x16c) != 0) {
                    cm_SendRelations(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), *(int *)(p + 4), -10, 1);
                    ai_Unk6f1b98++;
                    goto A;
                }
                d = v < 0 ? -v : v;
                if (*(int *)(p + 0x5c) != -1 || d <= rate * 2)
                    goto A;
                gm_AddHealth(i, -5000);
            } else {
            A:
                if (*b != 1)
                    continue;
                cm_SendMoveGoodsUnchecked(*(int *)(rec + 2), *(int *)(p + 4), 0, rate);
            }
            ai_Unk6f1b98++;
        }
    }
    ai_Unk6f1b98 = 0;
}
/* 0x4fa700, the wage figures of the character id for the report: out[0] = the byte +0x166, out[1] = +0x169, out[2] = the kind 0x23 .. 0x27 by the highest of the flag bits 0x200 .. 0x2000 of the int +0x1cc, out[4] / [8] / [0xc] = the three amounts (0x4e6f80 of the kinds, 0 when the character is no active person); flag != 0 sends the amounts to the player (and for kind 3 a random extra, 0x4c5f60). Returns 1 when amounts were computed. ROUGH */

int gm_CharacterWages(unsigned short id, int flag, unsigned char *out)  /* the callers pass a 16-bit id and the original call site loads it without an extension (0x4f5a80: mov cx,[esi]) */
{
    char *c;
    unsigned f;
    int res;
    int tmp[3];
    int r[9];
    unsigned char b;

    res = 0;
    c = ai_Chars[id];
    out[0] = c[0x166];
    out[1] = c[0x169];
    f = *(unsigned *)(c + 0x1cc);
    if (f & 0x1000)
        b = 0x26;
    else if (f & 0x2000)
        b = 0x27;
    else if (f & 0x800)
        b = 0x25;
    else if (f & 0x400)
        b = 0x24;
    else
        b = (f & 0x200) ? 0x23 : 0;
    out[2] = b;
    *(int *)(out + 4) = 0;
    *(int *)(out + 8) = 0;
    *(int *)(out + 0xc) = 0;
    if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0 && (c[0x166] != 0 || c[0x169] != 0 || (f & 0xe00) != 0)) {
        gv_GetTableRecord(0xf, r);
        *(int *)(out + 8) = (int)amt_GetKind(c[0x169], tmp);
        *(int *)(out + 4) = (int)amt_GetKind(c[0x166], tmp);
        *(int *)(out + 0xc) = (int)amt_GetKind(out[2], tmp);
        res = 1;
        if (flag != 0) {
            char *bl = (char *)gm_FirstBuilding(1);
            char *o;
            if (bl == 0)
                return 0;
            o = (char *)gm_Query(*(int *)(bl + 0x5d), 2, 6, 0, 0x115);
            if (o == 0)
                return 0;
            if (*(int *)(out + 4) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 4));
                ai_Unk6f1b98++;
            }
            if (*(int *)(out + 8) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 8));
                ai_Unk6f1b98++;
            }
            if (*(int *)(out + 0xc) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 0xc));
                ai_Unk6f1b98++;
            }
            if (c[2] == 3) {
                unsigned short rr = util_RandomU((tmp[2] & 0xff) + 2);
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, (rr + (tmp[2] & 0xff)) * 0x1900);
                cm_SendMoneyDelta(*(int *)(c + 4), r[2] & 0xff);
                ai_Unk6f1b98 += 2;
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

    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        gm_CharacterWages(i, 1, out);
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
    }
    ai_Unk6f1b98 = 0;
}
/* 0x4faa00, for each active character: the three ints at +0x1d8 .. summed with the 0x50a640 value of the character (the best one is found by 0x4fad60), written through 0x4c3c00 (4, 1, ..) to +0x1d4 .. and +0x1e0, and the float messages 0x4c4ac0 (0x1e4) / 0x4c3eb0. ROUGH, the float code is far off */

void unk_Unk4faa00(void)
{
    short id;
    int top;
    char *c;
    int i, sum, a, loc;
    float cnt, fv, q;
    double lim;
    int *pi;

    gm_FindRichest(&id, &top);
    ai_Unk6f1b98 = 0;
    for (c = ai_Chars[0]; (int)c < (unsigned)((unsigned)ai_Chars + 0x65400); c += 0x21c) {
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0) {
            sum = 0;
            cnt = 0.0f;
            a = I50A640(*(short *)c);
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            pi = (int *)(c + 0x1d8);
            for (i = 0; i < 3; i++) {
                loc = *pi;
                sum += loc;
                cnt = (float)((int)cnt + (loc != 0));
                cm_AddFieldDelta(4, 1, 0x1d4 + i * 4 + (int)c, &loc);
                pi++;
            }
            cnt = (float)((a + sum) / ((int)cnt + 1));
            cm_AddFieldDelta(4, 1, 0x1e0 + (int)c, &a);
            fv = (float)(int)cnt / (float)a;
            if (fv >= 1.0f)
                lim = 1.0;
            else
                lim = (double)fv;
            q = 1.0f - (float)a / (float)top;
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
            cm_SendFloatDelta(*(int *)(c + 4), 0x1e4, -(*(float *)(c + 0x1e4) - ((float)lim + q) * 0.5));
            q = -(*(float *)(c + 0xb4) - *(float *)(c + 0xb4) * 0.95);
            ai_BeginNeedBlock(c);
            ai_AddNeedDelta(3, q);
            ai_SendNeedBlock();
            ai_Unk6f1b98++;
        }
    }
    ai_Unk6f1b98 = 0;
}
/* 0x4fabe0, the end of the day for the incomes: shifts the 17-entry history of each of the five sums (0x91b580 / 0x91b5c4 / 0x91b608 / 0x91b64c / 0x91b690, 0x40 bytes) one place and clears the sums 0x91b5c0 / 0x91b604 / 0x91b648 / 0x91b68c / 0x91b6d0; runs 0x4fa4b0 (i, out, flags) for the 0x300 characters (clearing +0x198 / +0x19c); with flag 1 also 0x4fb0e0 and a list of up to 8 objects of the group (1, 5, 7) (0x501190 / 0x501030); then clears the ints +0x4d and (for kinds 11 .. 13) +0x69 in the 256 building records. ROUGH */

void gm_EndDayIncomes(unsigned flags)
{
    unsigned char *o;
    int *c;
    int i, k, j;
    unsigned v[8];
    int ent[14];

    memmove(gm_IncomeHistory, gm_IncomeHistory + 4, 0x40);
    memmove(gm_IncomeHistory + 0x44, gm_IncomeHistory + 0x48, 0x40);
    memmove(gm_IncomeHistory + 0x88, gm_IncomeHistory + 0x8c, 0x40);
    memmove(gm_IncomeHistory + 0xcc, gm_IncomeHistory + 0xd0, 0x40);
    memmove(gm_IncomeHistory + 0x110, gm_IncomeHistory + 0x114, 0x40);
    *(int *)ai_Unk91b5c0 = 0;
    *(int *)ai_Unk91b604 = 0;
    *(int *)ai_Unk91b648 = 0;
    *(int *)ai_Unk91b68c = 0;
    *(int *)ai_Unk91b6d0 = 0;
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        unk_Unk4fa4b0(i, ent, flags);
        c = (int *)(ai_Chars[i] + 0x198);
        c[1] = 0;
        c[0] = 0;
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
    }
    if (flags & 1) {
        unk_Unk4fb0e0();
        o = (unsigned char *)gm_BuildQuery(1, 5, 7);
        if (o != 0) {
            k = 0;
            do {
                if (k > 0x1f)
                    break;
                j = k;
                if (*(short *)(o + 0x27) != -1) {
                    *(unsigned char **)((char *)ent + k) = o;
                    j = k + 4;
                    *(unsigned *)((char *)v + k) = *(unsigned char *)(ai_BTypes + *o * 0x24d + 0x247);
                }
                o = (unsigned char *)gm_BuildNext();
                k = j;
            } while (o != 0);
        }
    }
    for (k = 0; k < 0xa900; k += 0xa9) {
        *(int *)(ai_Blds + k + 0x4d) = 0;
        if ((unsigned char)ai_Blds[k] > 10 && (unsigned char)ai_Blds[k] < 0xe)
            *(int *)(ai_Blds + k + 0x69) = 0;
    }
}
/* 0x4fad60, finds the character with the highest 0x50a640 value (of the active persons of kind < 10, +8 set), caches it in 0x6f1bac / 0x617730 until the clock 0x770ae0 passes 0x61772c or the cached record is gone; out: its id (-1 when none) and the value (0xc80 when none). Returns 1 when found */

int gm_FindRichest(short *id, int *rate)
{
    int best, bi, i, v;
    unsigned char *p;

    best = -100000;
    bi = 0xffff;
    if (ai_Unk6f1bac == 0 || ai_Unk61772c < *(int *)ai_Time || *ai_Unk6f1bac == -1 || (char)ai_Unk6f1bac[4] == 0) {
        ai_Unk61772c = *(int *)ai_Time;
        i = 0;
        for (p = (unsigned char *)ai_Chars[0] + 2; (int)p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            if (*(short *)(p - 2) != -1 && *p < 10 && p[6] != 0) {
                v = I50A640(i);
                if (best < v) {
                    best = v;
                    bi = i;
                }
            }
            i++;
        }
        if (bi == 0xffff) {
            ai_Unk6f1bac = 0;
            ai_Unk617730 = 0xc80;
            goto none;
        }
        ai_Unk6f1bac = (short *)ai_Chars[bi];
        ai_Unk617730 = best;
    }
    if (ai_Unk6f1bac != 0) {
        *id = *ai_Unk6f1bac;
        *rate = ai_Unk617730;
        return 1;
    }
none:
    *id = -1;
    *rate = ai_Unk617730;
    return 0;
}
/* 0x4fae50, a leader (the record b, its character at +0x27) takes a share from the other characters: with the chance 0.25 * the other character's byte +0x1b0 * (1 + 0.0125 * the 0x5092e0 skill of b) (times 0.8 / 0.7 / 0.6 when the groups differ, byte +0xc against the record 2's rate), the other pays 0x4c4830 (-1, .., about the 0x50a640 value * 160 / the best value): the loyalty float at +0x1d0 moves by 0.05 (same group) or -0.05 (0x4c4ac0), the master gets 0x4c4b00. out[0] = the number of payers, out[1] = the sum; out[2] = an object of the leader. Returns 1, or 0 when b has no character. ROUGH, the amount (the lost __ftol argument) is a guess */

int unk_Unk4fae50(char *b, int *out)
{
    int n, sum, i, a, v, top;
    short id;
    int r[9];
    char *c;
    char *u;
    float f, p, lo;
    unsigned uid;

    sum = 0;
    n = 0;
    gm_FindRichest(&id, &top);
    if (*(unsigned short *)(b + 0x27) == 0xffff)
        return 0;
    uid = *(unsigned short *)(b + 0x27);
    gm_GetSkill(ai_Chars[uid], 4);
    gv_GetTableRecord(2, r);
    f = (float)gm_BuildingSkill(b, 9, 1) * 0.0125f + 1.0f;
    u = ai_Chars[uid];
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[2] != 6 && (unsigned char)c[2] != 7 && (unsigned char)c[2] != 8 && (unsigned char)c[2] < 10 && *(int *)(c + 4) != *(int *)(u + 4)) {
            if (c[0xc] == u[0xc]) {
                p = (float)(unsigned char)c[0x1b0] * 0.25f * f;
                lo = 0.05f;
            } else {
                p = (float)(unsigned char)c[0x1b0] * 0.25f * f;
                if (r[6] == 2)
                    p = (float)(p * 0.8);
                else if ((unsigned char)u[0xc] == r[6])
                    p = (float)(p * 0.7);
                else
                    p = (float)(p * 0.6);
                lo = -0.05f;
            }
            {
                float x = util_RandomF2();
                if (x + x <= p) {
                    a = I50A640(*(short *)c);
                    if (0.0 < ((float)a / (float)top) * 160.0)
                        a = I50A640(*(short *)c);
                    v = (int)(((float)a / (float)top) * 160.0);
                    sum += v;
                    cm_SendMoveGoodsUnchecked(-1, *(int *)(c + 4), ai_OwnPlayer, v);
                    n++;
                    if (*(float *)(c + 0x1d0) < 0.95 && 0.05 < *(float *)(c + 0x1d0))
                        cm_SendFloatDelta(*(int *)(c + 4), 0x1d0, lo);
                    if (out[2] != 0) {
                        gm_Mix(u, c, 4);
                        cm_SendRelations(*(int *)(out[2] + 4), *(int *)(c + 4), (int)lo, 1);
                    }
                }
            }
        }
    }
    out[0] = n;
    out[1] = sum;
    return 1;
}
/* 0x4fb0e0, every fourth day (the clock int % 4 == 0): by the record 2 (rate 2: a random loyalty shift for the characters above 0xb, 0x4c3c00 (1, 1, 0xc, ..); other rates: 0x4f2e30 (2, ..) and a loyalty drop of 0.1) over the characters of kinds 1 .. 9 except 6 / 7 / 8. ROUGH */

void unk_Unk4fb0e0(void)
{
    int r[9];
    int i, t, m;
    char *c;
    unsigned loc;

    if (*(int *)ai_Time % 4 != 0)
        return;
    gv_GetTableRecord(2, r);
    if (r[6] == 2) {
        ai_Unk6f1b98 = 0;
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[2] != 0 && c[2] != 6 && c[2] != 7 && c[2] != 8 && *(unsigned short *)(c + 10) > 0xb && *(float *)(c + 0x1d0) < util_RandomF2() * 0.2) {
                loc = c[0xc] == 0;
                cm_SendFloatDelta(*(int *)(c + 4), 0x1d0, (float)(util_RandomF2() * 0.5 + 0.25));
                ai_Unk6f1b98++;
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                cm_AddFieldDelta(1, 1, 0xc, &loc);
                cm_SendFieldDeltas();
                ai_Unk6f1b98++;
                if (ai_Unk6f1b98 > 0x40) {
                    sv_PumpNet();
                    ai_Unk6f1b98 = 0;
                }
            }
        }
        ai_Unk6f1b98 = 0;
        return;
    }
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[2] != 0 && c[2] != 6 && c[2] != 7 && c[2] != 8 && *(unsigned short *)(c + 10) > 0xb && (c[0xc] != 0) != (r[6] != 0)) {
            t = -1;
            if (*(int *)(c + 0x170) != 0)
                t = *(int *)(*(int *)(c + 0x170) + 1);
            m = gv_Accuse(2, r[6] == 0, *(int *)(c + 4), -1, t);
            if (m != -1)
                ai_Unk6f1b98++;
            if (*(float *)(c + 0x1d0) > 0.1)
                cm_SendFloatDelta(*(int *)(c + 4), 0x1d0, -0.1f);
            ai_Unk6f1b98++;
            if (ai_Unk6f1b98 > 0x40) {
                sv_PumpNet();
                ai_Unk6f1b98 = 0;
            }
        }
    }
    ai_Unk6f1b98 = 0;
}

/* the head tables (records of 0x44 bytes: +0 the id, +4 the name, +0x34 four head numbers): 0x6118a8 / 0x6118f0 (the two default entries), 0x611938 (0x23 records, kind 0), 0x612288, 0x612bd8 (0x5f records, kind 0) / 0x614518 (0x5f, kind 1), 0x617398 (3 records), the history 0x91ac40 (8 records) */
extern unsigned ai_Unk6118a8[], ai_Unk6118f0[], ai_Unk611938[], ai_Unk612288[], ai_Unk612bd8[], ai_Unk614518[], ai_Unk617398[], ai_Unk91ac40[];  /* 0x6118a8, 0x6118f0, 0x611938, 0x612288, 0x612bd8, 0x614518, 0x617398, 0x91ac40 */
int d3_CurrentScene = 0;  /* 0x633444 */
char ai_Unk6f0454[3] = {0};  /* 0x6f0454 .. 0x6f0456 (unk_LoadIconNames writes [0..2]) */
int ai_Unk6f1b90 = 0;  /* 0x6f1b90, the number of icon names */
char * ai_Unk6f1b94 = 0;  /* 0x6f1b94, the icon names ( */
int ai_Unk6f1b98 = 0;  /* 0x6f1b98, the number of queued messages ( */
short * ai_Unk6f1bac = 0;  /* 0x6f1bac */
int ai_Unk6f1bb0 = 0;  /* 0x6f1bb0 */
void unk_Unk4fb620(unsigned *rec);
int ch_CurrentGroup = -1;  /* 0x5f1294 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */

/* 0x4fb330, the record of the table kind (0: 0x612bd8, 1: 0x614518) with the id: prepared by 0x4fb620 (the head numbers); the first free record when the id is not found (0x5f records of 0x44 bytes); 0 for another kind */
unsigned *gm_NameRecord(int kind, int id)
{
    unsigned *t;
    int i;

    i = 0;
    if (kind == 0)
        t = ai_Unk612bd8;
    else if (kind == 1)
        t = ai_Unk614518;
    else
        return 0;
    do {
        if (t[i * 0x11] == 0)
            break;
        if (t[i * 0x11] == (unsigned)id) {
            unk_Unk4fb620(t + i * 0x11);
            return t + i * 0x11;
        }
        i++;
    } while (i < 0x5f);
    return t + i * 0x11;
}
/* 0x4fb390, the name record (head table entry) of the character id: a character with the job (+0x164) 0 and +0x20 above +10 gets the default record (0x6118a8 / 0x6118f0 by +9); a character with its own name (+0x1f4) and the int +0x18c gets a copy of it in the history 0x91ac40 (8 records, 0x6f1bb0 the next one; the four head numbers = the int - 0x639); kind 0x11: one of the three records 0x617398; else the record of its job (+0x164) or of its title (+0x165) in the tables by +9 (0x612bd8 / 0x614518, 0x611938 / 0x612288). ROUGH */

unsigned *gm_CharNameRecord(int id)
{
    unsigned *p, *q;
    unsigned char *c;
    unsigned char k;
    char *s;
    int i, n, v;
    unsigned rec;

    p = 0;
    i = 0;
    c = (unsigned char *)ai_Chars[id];
    k = c[0x164];
    if (k != 0 || !((float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20))) {
        s = (char *)c + 0x1f4;
        if (*s != 0 && (v = *(int *)(c + 0x18c)) != -1) {
            ai_Unk6f1bb0 = (ai_Unk6f1bb0 + 1) % 8;
            n = ai_Unk6f1bb0 * 0x44;
            do {
                *((char *)ai_Unk91ac40 + n + 4 + (s - ((char *)c + 0x1f4))) = *s;
            } while (*s++ != 0);
            v -= 0x639;
            *(int *)((char *)ai_Unk91ac40 + n + 0x34) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x38) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x3c) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x40) = v;
            return (unsigned *)((char *)ai_Unk91ac40 + n);
        }
        if (c[2] == 0x11)
            return ai_Unk617398 + (util_RandomU(3) & 0xffff) * 0x11;
        if (k != 0) {
            if (c[9] == 0) {
                p = ai_Unk612bd8;
                q = p;
            } else {
                q = p;
                if (c[9] == 1) {
                    p = ai_Unk614518;
                    q = p;
                }
            }
            do {
                if (*p == 0)
                    break;
                if (*p == k) {
                    unk_Unk4fb620(q + i * 0x11);
                    return q + i * 0x11;
                }
                i++;
                p += 0x11;
            } while (i < 0x5f);
            return q + i * 0x11;
        }
        if (c[0x165] != 0) {
            if (c[9] == 0) {
                q = ai_Unk611938;
                p = q;
            } else {
                q = p;
                if (c[9] == 1) {
                    q = ai_Unk612288;
                    p = q;
                }
            }
            do {
                if (*q == 0) {
                    if (i < 0x23)
                        return p + i * 0x11;
                    break;
                }
                if (*q == c[0x165]) {
                    unk_Unk4fb620(p + i * 0x11);
                    return p + i * 0x11;
                }
                i++;
                q += 0x11;
            } while (i < 0x23);
        }
        if (c[9] == 0)
            return ai_Unk611938;
        if (c[9] == 1)
            p = ai_Unk612288;
    } else {
        p = ai_Unk6118a8;
        if (c[9] != 0)
            return ai_Unk6118f0;
    }
    return p;
}
/* 0x4fb5a0, picks the variant of the object o (+0x184 its model record): byte +0x23a of the model = the id of o (+4) modulo the variant count of the model class (+0x1e4, or & 3 when 5 or more) unless the model is the shared one 0xfd7ba0; when the class is the current one (0x5f1294) 0x446f30 refreshes it. ROUGH */

void gm_SelectModelVariant(char *o)
{
    unsigned v;
    char *m;
    int cls;

    v = 0;
    if (o != 0 && (m = *(char **)(o + 0x184)) != 0) {
        cls = *(int *)(*(int *)(m + 0x34) + 0x1ec);
        if (*(char **)(m + 0x88) != (char *)(unsigned)&d3_Scenes) {
            v = *(unsigned *)(*(int *)(cls + 0x104) + 0x1e4);
            if ((int)v < 5)
                v = *(unsigned *)(o + 4) % v;
            else
                v = *(unsigned *)(o + 4) & 3;
            m[0x23a] = (char)v;
        }
        if (ch_CurrentGroup == *(int *)(m + 0x2c))
            d3if_SelectTextureSet(*(int *)(m + 0x34), cls + 0xf4, v, 1);
    }
}
/* 0x4fb620, fills the four head numbers (+0x34 .. +0x40) of a head table record whose first is still 0xff: from the name (+4) split at the last '_' into name and suffix, the icon "%s_HEAD%i_%s_ICON.bmp" (or "%s_HEAD%i_ICON.bmp") is looked up in the icon names 0x6f1b94 (0x30 bytes each, 0x6f1b90 of them) for head 0 .. 3 */

void unk_Unk4fb620(unsigned *rec)
{
    char name[128], suffix[128], icon[128];
    char *u;
    int h, i, *out;

    out = (int *)(rec + 0xd);
    if (*out != 0xff)
        return;
    strcpy(name, (char *)(rec + 1));
    u = strrchr(name, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    for (h = 0; h < 4; h++, out++) {
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
        *out = 0xff;
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                *out = i;
                goto next;
            }
        }
        sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
        *out = 0xff;
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                *out = i;
                break;
            }
        }
    next:;
    }
}
/* 0x4fb770, the number of the first head icon (of the heads 0 .. 3) that exists for the head name (split at the last '_' as in 0x4fb620; "zigeunermeister" uses the form without the suffix), 0 when none. ROUGH */

int gm_HeadIconNumber(char *name)
{
    char buf[128], suffix[128], icon[128];
    char *u;
    int h, i, r;

    strcpy(buf, name);
    u = strrchr(buf, '_');
    if (u != 0) {
        strcpy(suffix, u + 1);
        *u = 0;
    }
    r = 0xff;
    for (h = 0; h < 4; h++) {
        if (r != 0xff)
            return r;
        sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", buf, h, suffix);
        if (strstr(buf, "zigeunermeister") != 0)
            sprintf(icon, "%s_HEAD%i_ICON.bmp", buf, h);
        for (i = 0; i < ai_Unk6f1b90; i++) {
            if (ai_Unk6f1b94[i * 0x30] == 0)
                break;
            if (_stricmp(icon, ai_Unk6f1b94 + i * 0x30) == 0) {
                r = i;
                break;
            }
        }
    }
    if (r != 0xff)
        return r;
    return 0;
}

extern char * ai_PlayerBuilding;  /* 0x6f2308 */
extern char * ai_FallbackBuilding;  /* 0x6f230c */
struct CHARACTER * ch_Create();                     /* 0x403390 */
int gm_IsHomeType();                     /* 0x502520 */
int init_ObjectSceneUniverse();                     /* 0x511dc0 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
unsigned *gm_CharNameRecord(int id);

/* 0x4fb890, chooses the head icon number of the character c (+0x18c = icon index + 0x639; id at +0, number at +4). Without a model (+0x184) or icon names: the head numbers of the name record (0x4fb390, filled as 0x4fb620 does, "zigeunermeister" without the suffix), the character takes number (its id % count of the valid ones); returns id / count. With a model: the first model part name (0x40 bytes each, the count at +0x1e0 of its class) containing "kopf" / "KOPF" / "HEAD" / "head" whose icon "%s_ICON.bmp" exists. Otherwise +0x18c = 0x639. ROUGH */
int gm_ChooseHeadIcon(unsigned short *c)
{
    char name[128], suffix[128], icon[128];
    char *u, *part;
    unsigned *rec;
    int *m;
    int *q;
    int i, j, h, n, found, cls, cnt, stride;
    unsigned k;

    found = 0;
    i = 0;
    if (c == 0)
        return 0;
    m = *(int **)(c + 0xc2);
    if (m == 0 || ai_Unk6f1b94 == 0) {
        rec = gm_CharNameRecord(*c);
        if (rec[0xd] == 0xff) {
            strcpy(name, (char *)(rec + 1));
            u = strrchr(name, '_');
            if (u != 0) {
                strcpy(suffix, u + 1);
                *u = 0;
            }
            q = (int *)(rec + 0xd);
            *q = 0;
            for (h = 0; h < 4; h++, q++) {
                sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
                if (strstr(name, "zigeunermeister") != 0)
                    sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
                *q = 0xff;
                for (j = 0; j < ai_Unk6f1b90; j++) {
                    if (ai_Unk6f1b94[j * 0x30] == 0)
                        break;
                    if (_stricmp(icon, ai_Unk6f1b94 + j * 0x30) == 0) {
                        *q = j;
                        break;
                    }
                }
            }
        }
        for (k = 0; k < 4; k++) {
            if (rec[0xd + k] == 0xff)
                break;
        }
        if (k != 0) {
            *(int *)(c + 0xc6) = rec[0xd + *(unsigned *)(c + 2) % k] + 0x639;
            return *(unsigned *)(c + 2) / k;
        }
    } else {
        if ((char *)m[0x22] == (char *)(unsigned)&d3_Scenes)
            return (int)m;
        cls = *(int *)(*(int *)(m[0xd] + 0x1ec) + 0x104);
        stride = *(unsigned char *)(*(int *)(m[0xd] + 0x1ec) + 0x289);
        n = *(int *)(cls + 0x1e0);
        if (n > 0) {
            do {
                if (found != 0)
                    return found;
                part = (char *)((n * stride + i) * 0x40 + *(int *)(cls + 0x204));
                if (*part != 0 && (strstr(part, "kopf") != 0 || strstr(part, "KOPF") != 0 || strstr(part, "HEAD") != 0 || strstr(part, "head") != 0)) {
                    sprintf(icon, "%s_ICON.bmp", part);
                    for (j = 0; j < ai_Unk6f1b90; j++) {
                        if (ai_Unk6f1b94[j * 0x30] == 0)
                            break;
                        if (_stricmp(icon, ai_Unk6f1b94 + j * 0x30) == 0) {
                            found = 1;
                            *(int *)(c + 0xc6) = j + 0x639;
                            break;
                        }
                    }
                }
                n = *(int *)(cls + 0x1e0);
                i++;
            } while (i < n);
            if (found != 0)
                return found;
        }
    }
    *(int *)(c + 0xc6) = 0x639;
    return 0;
}
/* 0x4fbb80, creates the model of the character c (+0x184) in the scene model: at the position p (0x403390 with the name of the head record 0x4fb390 (c), 0x20-byte zero vector when p is 0), named "sp_<number>", linked back to c (+0x200 of its object, +0x48 = id | 0x3000000), copies the name to c + 500 and chooses the head (0x4fb5a0 / 0x4fb890); the position comes from the dummy of the object obj when a dummy name is given (0x449690 / 0x458a20); scene 0x633444 is selected again at the end. Returns the model. ROUGH */

int unk_Unk4fbb80(unsigned short *c, int scene, int *p, int *ori, int obj, char *dummy)
{
    int saved;
    int zero[4];
    int pos[3];
    int it;
    char *name;
    char *s;
    int *m;
    int *ori0;

    saved = d3_CurrentScene;
    ori0 = ori;
    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    zero[3] = 0;
    if (p == 0)
        p = zero;
    if (ori == 0)
        ori = zero;
    if (scene < 0)
        return 0;
    d3if_SelectScene(scene, 1);
    if (dummy != 0 && (it = d3_FindObject(obj, 0x100, 0, dummy)) != 0) {
        d3_PointToWorld(it, it + 0x4c, pos);
    } else {
        pos[0] = p[0];
        pos[1] = p[1];
        pos[2] = p[2];
    }
    {
        unsigned *rec = gm_CharNameRecord(*c);
        if (rec != 0) {
            name = (char *)(rec + 1);
            s = name;
            m = (int *)ch_Create(name, pos);
            *(int **)(c + 0xc2) = m;
            if (m != 0) {
                sprintf((char *)m[0xd], "sp_%i", *(int *)(c + 2));
                *(unsigned short **)(*(int *)(*(int *)(c + 0xc2) + 0x34) + 0x200) = c;
                *(unsigned *)(*(int *)(*(int *)(c + 0xc2) + 0x34) + 0x48) = *c | 0x3000000;
                if (ori0 != 0)
                    d3_SetObjectRot(*(int *)(*(int *)(c + 0xc2) + 0x34), ori0);
                do {
                    *((char *)c + 500 + (s - name)) = *s;
                } while (*s++ != 0);
                gm_SelectModelVariant((char *)c);
                gm_ChooseHeadIcon(c);
            }
        }
    }
    d3if_SelectScene(saved, 1);
    return *(int *)(c + 0xc2);
}
/* 0x4fbd10, puts the character id (0x505df0, it must have no model yet) on the stage at the object of the group (1, 1, a) of its building (the group of the mission object when b is -1): its scene (0x502520 / 0x511dc0 / 0x511e60), the dummy "dummy_TUER" (or the object own dummy 0x406790, else "dummy_EINGANG"), the model by 0x4fbb80, the animation "bewegung/gehen"; stores the building (+0x2c) and b (+0x30) in the model and hides it (0x402480) when the shown building (0x6f2308 / 0x6f230c) is another. Returns the model or 0. ROUGH */

int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5)
{
    int saved;
    char *c;
    int o, o2, sc;
    char *dummy;
    int uVar7;

    saved = d3_CurrentScene;
    o2 = 0;
    c = gm_FindChar(id);
    if (c == 0)
        return 0;
    if (*(int *)(c + 0x184) != 0)
        return 0;
    o = gm_BuildQuery(1, 1, a);
    if (o == 0)
        return 0;
    if (b != -1) {
        o2 = (int)gm_Query(*(int *)(o + 0x5d), 1, 1, b);
        if (o2 == 0)
            return 0;
    }
    sc = gm_IsHomeType(o);
    if (sc == 0) {
        if (o2 == 0) {
            sc = 0;
            goto go;
        }
        sc = init_ObjectSceneUniverse(o2);
    } else
        sc = init_BuildingSceneUniverse(o);
    if (sc < 0)
        return 0;
go:
    d3if_SelectScene(sc, 1);
    if (o2 == 0) {
        uVar7 = *(int *)(o + 0x61);
        dummy = "dummy_TUER";
        p5 = 0;
        p4 = 0;
    } else {
        dummy = (char *)ch_FindDummy();
        if (dummy == 0) {
            dummy = "dummy_EINGANG";
            uVar7 = 0;
        } else {
            uVar7 = 0;
        }
    }
    unk_Unk4fbb80((unsigned short *)c, sc, p4, p5, uVar7, dummy);
    if (*(int *)(c + 0x184) != 0)
        ch_PreloadAnims(*(int *)(c + 0x184), 1, "bewegung/gehen");
    d3if_SelectScene(saved, 1);
    if (*(int *)(c + 0x184) == 0)
        return 0;
    *(int *)(*(int *)(c + 0x184) + 0x2c) = a;
    *(int *)(*(int *)(c + 0x184) + 0x30) = b;
    if ((ai_PlayerBuilding != 0 && ai_PlayerBuilding != o) || (ai_FallbackBuilding != 0 && ai_FallbackBuilding != o))
        ch_SetVisible(*(int *)(c + 0x184), 0);
    return *(int *)(c + 0x184);
}

extern char * cm_FieldObject;                /* 0xa68d2c */
extern char ch_EmptyName[];             /* 0x631090 */
int gm_Unk5075c0();                     /* 0x5075c0 */
void cm_SendKillCharacter(int a, int b);                     /* 0x4c4e00 */
int gv_DecayRaisedDead();                     /* 0x4f1660 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
void unk_Unk4fccc0();                   /* 0x4fccc0 */

/* the 0xf8-byte message 0x72 (kind at +4, the character id at +8 and +0x58, -1 at +0xc, the clock 0x770ae0 at +0x28 .. +0x34, 2 at +0x36) of 0x4fbe90 */
#define G_MSG72(m, c)                                                  \
    memset(m, 0, 0xf8);                                                \
    *(int *)((m) + 8) = *(int *)((c) + 4);                             \
    *(int *)((m) + 0x28) = *(int *)ai_Time;                       \
    *(int *)((m) + 0x2c) = *(int *)(ai_Time + 4);                 \
    (m)[4] = 0x72;                                                     \
    *(int *)((m) + 0xc) = -1;                                          \
    (m)[0x36] = 2;                                                     \
    *(int *)((m) + 0x30) = *(int *)(ai_Time + 8);                 \
    *(short *)((m) + 0x34) = *(short *)(ai_Time + 0xc);           \
    *(int *)((m) + 0x58) = *(int *)((m) + 8);                          \
    cm_SendLongMessage(m)

/* 0x4fbe90, one daily step of the character c (the value is the new health, -1 for no step): free slots (id -1) -1; the kind 0xf (0x5075c0 true: message 0x4c4e00 (id, 1)); kind 0x14 (0x4f1660 == 1: removed through the message 0x72, the message 0x414a to the master player); then the flags word +0x2c is compacted (each field of 1 .. 4 bits is halved: 1 -> 0, n -> n | n >> 1 of the field's upper bits) and sent (0x4c4a60 (id, 0x2c, 4, flags, -1)); a character with flag 2 (+0x1cc) loses 0x11 + random 0x10 of its float +0x1c and dies (message 0x72) below 0; otherwise the health float +0x1c moves toward +0x10 (a share of +0x20 by the age (+10) / the 0x5092e0 skill of the master's building, +0x14 as the target) and when it plus the int +0x24 falls below 0 the character dies (a master gets the message 0x1c51 / 0x4080 with "NACHRICHTEN\DOMIZIL_05"; with flag 1 and kinds other than 6 / 7 it is reset to 100 and the 0x1cc flags = 2); else 0x4fccc0 and the age (+10) + 1 is stored. ROUGH, the float code is guessed */
float unk_Unk4fbe90(unsigned short *c)
{
    char m[0xf8];
    char buf[512];
    unsigned f, g;
    char b;
    float h, lo, t, fl;
    double d;
    int a;
    char *p = (char *)c;
    char *mst;
    int k;

    if (*c == 0xffff)
        return -1.0f;
    if (p[2] == 0x0f) {
        if (gm_Unk5075c0(c) != 0) {
            cm_SendKillCharacter(*(int *)(p + 4), 1);
            ai_Unk6f1b98++;
        }
        return -1.0f;
    }
    if (p[2] == 0x14) {
        if (gv_DecayRaisedDead(c) == 1) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldValue(1, 1, (char *)p - cm_FieldObject + 8, &b);
            cm_SendFieldValues();
            G_MSG72(m, p);
            mst = *(char **)(p + 0x16c);
            if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
                txt_Format(buf, 0x414a, *c, mst);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
            }
        }
        return 0.0f;
    }
    f = *(unsigned *)(p + 0x2c);
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
                f = (f & 0xfe7fffff) | ((f >> 1) & ((unsigned)&ai_Relations + 0x1aca0));
        }
        g = (f >> 0x19) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xe1ffffff;
            else
                f = (f & 0xe1ffffff) | ((f >> 1) & 0xe000000);
        }
        cm_SendSetField(*(int *)(p + 4), 0x2c, 4, f, -1);
        ai_Unk6f1b98++;
    }
    if (p[0x1cc] & 2) {
        fl = (float)((util_RandomU(0x10) & 0xffff) + 0x11);
        t = *(float *)(p + 0x1c) - (float)(int)fl;
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
        if (t <= 0.0f) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldValue(1, 1, (char *)p - cm_FieldObject + 8, &b);
            cm_SendFieldValues();
            ai_Unk6f1b98++;
            G_MSG72(m, p);
            ai_Unk6f1b98++;
            mst = *(char **)(p + 0x16c);
            if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
                txt_Format(buf, 0x1c51, mst, *c);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
                ai_Unk6f1b98++;
            }
        }
        return t;
    }
    fl = (float)*(unsigned short *)(p + 10);
    h = fl;
    if (h > *(float *)(p + 0x20)) {
        lo = *(float *)(p + 0x14);
        if (*(int *)(p + 0x170) != 0) {
            a = gm_BuildingSkill(*(int *)(p + 0x170), 8, 1);
            lo = (float)a * 0.0125f * -3.0f + lo;
        }
        t = (*(float *)(p + 0x1c) - lo) + lo / *(float *)(p + 0x1c);
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
    } else {
        t = ((*(float *)(p + 0x1c) - *(float *)(p + 0x10)) / *(float *)(p + 0x20)) * h + *(float *)(p + 0x10);
    }
    fl = (float)*(int *)(p + 0x24) + t;
    if (fl <= 0.0f) {
        b = 0;
        if ((p[0x1cc] & 1) != 0 && p[2] != 6 && p[2] != 7) {
            d = 0.0;
            t = 100.0f;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldDelta(4, 1, 0x1c, &t);
            cm_AddFieldDelta(4, 1, 0x24, &d);
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
            cm_SendSetField(*(int *)(p + 4), 0x1cc, 4, 2, 0);
            ai_Unk6f1b98++;
            return t;
        }
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldValue(1, 1, (char *)p - cm_FieldObject + 8, &b);
        cm_SendFieldValues();
        ai_Unk6f1b98++;
        G_MSG72(m, p);
        ai_Unk6f1b98++;
        mst = *(char **)(p + 0x16c);
        if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
            if ((p[0x1cc] & 4) == 0) {
                txt_Format(buf, 0x1c51, mst, *c);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
            } else {
                txt_Format(buf, 0x4080, p + 0x30, p + 0x40, *(unsigned short *)(p + 10));
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, "_NACHRICHTEN_DOMIZIL_05");
            }
            ai_Unk6f1b98++;
        }
        return 0.0f;
    }
    unk_Unk4fccc0(c);
    if (fl <= 0.0f)
        d = 0.0;
    else
        d = (double)fl;
    k = *(unsigned short *)(p + 10) + 1;
    cm_BeginFieldBlock(c, *(int *)(p + 4));
    cm_AddFieldDelta(2, 1, 10, &k);
    cm_SendFieldDeltas();
    ai_Unk6f1b98++;
    return (float)d;
}

void ai_Unk4c5690(int a, const char *s);                     /* 0x4c5690 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
float gm_CharacterHealth(short *c);
float unk_Unk4fc890(short *c);
void unk_Unk4fd120(void);

/* 0x4fc6f0, ages the health float (+0x1c) of the character c to the age (+10 = age): above the lifetime float (+0x20) by an iteration of v = (v - k) + k / v with k = +0x14 (as many times as the years over), otherwise towards the float +0x18; the change goes to 0x4c4ac0 (id, 0x1c, ..), the age to the character through 0x4c3c00 (2, 1, 10, &age). ROUGH, the number of iterations is a guess */
void gm_AgeCharacter(char *c, unsigned short age)
{
    float k, v, d;
    int n;

    if (c != 0) {
        if ((float)(age & 0xffff) > *(float *)(c + 0x20)) {
            k = *(float *)(c + 0x14);
            v = *(float *)(c + 0x1c);
            n = (int)((float)(age & 0xffff) - *(float *)(c + 0x20));
            if (n > 0) {
                do {
                    n--;
                    v = (v - k) + k / v;
                } while (n != 0);
            }
            d = v - *(float *)(c + 0x1c);
        } else {
            d = *(float *)(c + 0x18) - *(float *)(c + 0x1c);
        }
        cm_SendFloatDelta(*(int *)(c + 4), 0x1c, d);
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldDelta(2, 1, 10, &age);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
    }
}
/* 0x4fc7a0, the age the character c dies at: the aging steps (v = (v - k) + k / v, k = +0x14) until its life value +0x1c falls to 1.0, added to max(+0x20, the age +0xa) (the draft below stops after the loop; the original counts the steps and adds them) */

int gm_ExpectedDeathAge(char *c)
{
    float v;

    for (v = *(float *)(c + 0x1c); 1.0 < v; v = (v - *(float *)(c + 0x14)) + *(float *)(c + 0x14) / v)
        ;
    return (int)v;
}
/* 0x4fc820, the health of the character c now (-1 for a free slot or an inactive one): the +0x1c float when the age (+10) is above the lifetime float (+0x20), else the line from +0x10 to +0x1c over the lifetime, plus the int +0x24, not below 0 */

float gm_CharacterHealth(short *c)
{
    float f;
    float a;

    if (*c == -1)
        return -1.0f;
    if (((char *)c)[8] == 0)
        return -1.0f;
    a = (float)*(unsigned short *)((char *)c + 10);
    if (a > *(float *)((char *)c + 0x20))
        f = *(float *)((char *)c + 0x1c);
    else
        f = ((*(float *)((char *)c + 0x1c) - *(float *)((char *)c + 0x10)) / *(float *)((char *)c + 0x20)) * a + *(float *)((char *)c + 0x10);
    f = (float)(f + *(int *)((char *)c + 0x24));
    if (f <= 0.0f)
        f = 0.0f;
    return f;
}
/* 0x4fc890, the same without the int +0x24 and the floor */

float unk_Unk4fc890(short *c)
{
    float a;

    if (*c == -1)
        return -1.0f;
    if (((char *)c)[8] == 0)
        return -1.0f;
    a = (float)*(unsigned short *)((char *)c + 10);
    if (a <= *(float *)((char *)c + 0x20))
        return ((*(float *)((char *)c + 0x1c) - *(float *)((char *)c + 0x10)) / *(float *)((char *)c + 0x20)) * a + *(float *)((char *)c + 0x10);
    return *(float *)((char *)c + 0x1c);
}
/* 0x4fc8e0, health now / health of the character c without the extra (0 for a free slot or an inactive one) */

float gm_HealthRatio(short *c)
{
    if (*c == -1)
        return 0.0f;
    if (((char *)c)[8] == 0)
        return 0.0f;
    return (float)(gm_CharacterHealth(c) / unk_Unk4fc890(c));
}
/* 0x4fc920, the daily step of all characters: the kind 0x12 ones get the byte 8 at +2 (0x4c3c00 (1, 1, 2)); every character of kind < 10 or 0x14 that is active runs 0x4fbe90 and, when the lifetime float (+0x20) - 1 equals the age and it has a model (+0x184), the call 0x4c5690 (id, "stallbursche_MANN" / "magd_FRAU" by the byte +9); then 0x4fd120 */

void gm_DailyCharacters(void)
{
    unsigned loc;
    char *c;
    int i;

    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (c[2] == 0x12) {
            loc = 8;
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(1, 1, 2, &loc);
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (((unsigned char)c[2] < 10 || c[2] == 0x14) && c[8] != 0) {
            unk_Unk4fbe90((unsigned short *)c);
            loc = *(unsigned short *)(c + 10);
            if (*(float *)(c + 0x20) - 1.0 == (float)loc && *(int *)(c + 0x184) != 0) {
                char *name;
                if (c[9] == 0)
                    name = "stallbursche_MANN";
                else
                    name = "magd_FRAU";
                ai_Unk4c5690(*(int *)(c + 4), name);
                ai_Unk6f1b98++;
            }
        }
    }
    unk_Unk4fd120();
    ai_Unk6f1b98 = 0;
}
/* 0x4fca40, changes the health of the character id by d (kind 0x14: the float +0x1c directly, dies below 0 through the message 0x72; others: the health of 0x4fc820's formula plus d, the int +0x24 = d through 0x4c3d00): -1 for a free slot, kind 10 or an inactive one; returns the new health, 0 when the character died. ROUGH */

float gm_AddHealth(unsigned id, int d)
{
    char m[0xf8];
    char b;
    char *c;
    float a, f, fc, lim;

    id &= 0xffff;
    c = ai_Chars[id];
    if (*(short *)c == -1 || c[2] == 10 || c[8] == 0)
        return -1.0f;
    if (c[2] == 0x14) {
        a = (float)d;
        fc = *(float *)(c + 0x1c) + a;
        if (fc <= 0.0f) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldValue(1, 1, c - cm_FieldObject + 8, &b);
            cm_SendFieldValues();
            G_MSG72(m, c);
            return 0.0f;
        }
        cm_SendFloatDelta(*(int *)(c + 4), 0x1c, a);
        return fc;
    }
    a = (float)*(unsigned short *)(c + 10);
    lim = *(float *)(c + 0x20);
    if (a > lim)
        f = *(float *)(c + 0x1c);
    else
        f = ((*(float *)(c + 0x1c) - *(float *)(c + 0x10)) / lim) * a + *(float *)(c + 0x10);
    f = *(int *)(c + 0x24) + d + f;
    if (f <= 0.0f)
        f = 0.0f;
    fc = f;
    if (f <= 0.0f) {
        b = 0;
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValue(1, 1, c - cm_FieldObject + 8, &b);
        cm_SendFieldValues();
        G_MSG72(m, c);
        return 0.0f;
    }
    cm_BeginFieldBlock(c, *(int *)(c + 4));
    cm_AddFieldValues(4, 1, 0x24, &d);
    cm_SendFieldDeltas();
    return fc;
}
/* 0x4fd090, 1 when the character c (not a player, kind 6 / 7) may be removed: no other player (kind 6 / 7) has it as one of the three direct relations (+0x5a, +0x5e, +0x62) and, when the five slots from +0x66 hold it, they are all filled */

int unk_Unk4fd090(char *c)
{
    int id, n, found;
    char *p;
    int *q;
    int k;

    if (c[2] == 6 || c[2] == 7)
        return 0;
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
            for (k = 5; k != 0; k--) {
                if (*q == id) {
                    found = 1;
                    n++;
                } else if (*q != -1)
                    n++;
                q++;
            }
            if (found && n < 5)
                return 0;
        }
    }
    return 1;
}

extern float ai_Unk617734[6];
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);                     /* 0x4c4650 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
int cm_GetSlotValue(int h);                     /* 0x4c3b10 */
int cm_SendNewParty(void *p);                     /* 0x4c4fa0 */
int gm_TitleNumber();                     /* 0x5043d0 */
int ai_WorksForMaster(struct AICHAR *c);                     /* 0x47dd70 */

/* 0x4fccc0, the yearly love / marriage step of the character c (kind < 10, active, byte +9 = 1, +0x1b1 = 0, +0x1c8 bit 0x40000 clear; its partner o = 0x505df0 (+0x5c)): a float +0x7c above 1.0 is pulled down by 0x4c4ac0 (-0.5 * random - the value) and the pair gets the event 0x4c4650 (8, ..) with a message 7 (0x4c4fa0, text number 0x12 ..); otherwise (the partner active, the age +10 below 0x27) the number of the relatives at +0x68 .. (4 .. 8; a player (6 / 7) or a priest (5) among them ends it) sets the float +0x7c to (random + 1) * the table value 0x617734 [count]. ROUGH */
void unk_Unk4fccc0(char *c)
{
    int clk[4];
    int msg[0x45];
    char *o, *q2, *mate, *p;
    int *q;
    int arg, t, cnt, cnt2, k, h, n, bld;
    float fv;
    char kind;

    if ((unsigned char)c[2] > 9)
        return;
    if (c[8] == 0)
        return;
    if (c[9] != 1)
        return;
    if (c[0x1b1] != 0)
        return;
    if ((*(unsigned *)(c + 0x1c8) & 0x40000) != 0)
        return;
    o = gm_FindChar(*(int *)(c + 0x5c));
    if (1.0 < *(float *)(c + 0x7c)) {
        cm_SendFloatDelta(*(int *)(c + 4), 0x7c, (float)(util_RandomF2() * -0.5 - *(float *)(c + 0x7c)));
        ai_Unk6f1b98++;
        if (o == 0)
            return;
        memcpy(clk, ai_Time, 14);
        arg = 2;
        gm_AddTime(clk, 8, 0, 0);
        *(short *)(clk + 1) = util_RandomU(4) + 8;
        bld = *(int *)(c + 0x170);
        *(short *)((char *)clk + 6) = 0;
        *(short *)(clk + 2) = 0;
        t = bld == 0 ? -1 : *(int *)(bld + 1);
        mate = 0;
        if (o[2] == 6 || o[2] == 7) {
            cnt = 0;
            q = (int *)(o + 0x68);
            for (k = 4; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            if (cnt == 0)
                arg = (unsigned char)o[9];
            mate = o;
            if (*(int *)(o + 0x170) == 0)
                t = bld == 0 ? -1 : *(int *)(bld + 1);
            else
                t = *(int *)(*(int *)(o + 0x170) + 1);
        } else if (c[2] == 6 || c[2] == 7) {
            cnt = 0;
            q = (int *)(c + 0x68);
            for (k = 4; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            if (cnt == 0)
                arg = (unsigned char)c[9];
            mate = c;
            if (bld == 0) {
                if (*(int *)(o + 0x170) == 0)
                    t = -1;
                else
                    t = *(int *)(*(int *)(o + 0x170) + 1);
            } else
                t = *(int *)(bld + 1);
        }
        h = ((int (*)(int, void *, void *, int, int, int, int, int))cm_SendCreateCharacter)(8, *(int *)(o + 4), *(int *)(c + 4), 0, t, 0, 0, arg);
        n = cm_GetSlotState(h);
        while (n == 0) {
            sv_PumpNet();
            n = cm_GetSlotState(h);
        }
        if (cm_GetSlotState(h) == 2)
            return;
        p = (char *)cm_GetSlotValue(h);
        if (mate != 0) {
            arg = 0x12;
            memset(msg, 0, sizeof(msg));
            ((char *)msg)[8] = 7;
            msg[3] = *(int *)(mate + 4);
            msg[5] = -1;
            msg[6] = clk[0];
            msg[7] = *(int *)((char *)clk + 4);
            msg[8] = *(int *)((char *)clk + 8);
            *(short *)(msg + 9) = *(short *)((char *)clk + 12);
            ((char *)msg)[0x26] = (char)0xa0;
            ((char *)msg)[0x30] = 2;
            msg[0xd] = msg[3];
            msg[0xe] = *(int *)(p + 4);
            cm_BeginFieldBlock(p, msg[0xe]);
            cm_AddFieldDelta(1, 1, 2, &arg);
            cm_SendFieldDeltas();
            cm_SendNewParty(msg);
            ai_Unk6f1b98++;
            return;
        }
    }
    if (o != 0 && o[8] != 0 && *(unsigned short *)(c + 10) < 0x27) {
        cnt = 0;
        q = (int *)(c + 0x68);
        for (k = 3; k < 8; k++, q++) {
            if (*q != -1) {
                q2 = gm_FindChar(*q);
                kind = q2[2];
                if (kind == 6 || kind == 7 || kind == 5)
                    return;
                if (kind != 0x0f)
                    cnt++;
            }
        }
        if (cnt != 4) {
            if (o[2] == 6 || o[2] == 7) {
                cnt2 = 0;
                q = (int *)(o + 0x68);
                for (k = 3; k < 8; k++, q++) {
                    if (*q != -1) {
                        q2 = gm_FindChar(*q);
                        kind = q2[2];
                        if (kind == 6 || kind == 7 || kind == 5)
                            return;
                        if (kind != 0x0f)
                            cnt2++;
                    }
                }
                if (cnt2 == 5)
                    return;
            }
            fv = (float)((util_RandomF2() + 1.0) * ai_Unk617734[cnt]);
            cm_SendFloatDelta(*(int *)(c + 4), 0x7c, fv);
            ai_Unk6f1b98++;
        }
    }
}
/* 0x4fd120, the yearly population step: counts the active characters (n); every kind 3 (child?) character that is old enough (age +10 at least 0x2c + random 4, or n >= 652.8) with no job / title bytes (+0x164, +0x166, +0x167) and removable (0x4fd090) is removed by 0x4fca40 (id, ..); the others (age above 1): kind 8 (no 0x4fd7e0, partners 0x5e / 0x62 gone) get the event messages 3 / 2 and a payment 0x4c4830 ((random + 0x800) * the mark * 0x20); the other aging characters 0x47dd70 (counted; relatives +0x1ee); then a random new arrival (0x4c4650 (0x10 ..) and a gift of (random + 10000) * 0x40) and, below 652.8 characters, up to (0x28 - the events) / 2 newcomers (0x4c4650 (3, .., 0x14 ..), random wage 0x400 .. + 0x400). ROUGH */

void unk_Unk4fd120(void)
{
    int n, events, kids, i, t, m;
    char *c;
    char *a, *b;
    int h, r, loc[2];
    unsigned v;

    n = 0;
    events = 0;
    kids = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0)
            n++;
    }
    m = n;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0 && c[2] == 3 && ((util_RandomU(4) & 0xffff) + 0x2c <= *(unsigned short *)(c + 10) || (float)m >= 652.80005f) && c[0x164] == 0 && c[0x166] == 0 && c[0x167] == 0 && unk_Unk4fd090(c) != 0) {
            gm_AddHealth(*(unsigned short *)c, (int)0);
            n--;
            m = n;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[8] > 1) {
            if ((float)m <= 652.80005f) {
            L1:
                if (c[2] == 8 && unk_Unk4fd7e0(*(unsigned short *)c) == 0) {
                    a = gm_FindChar(*(int *)(c + 0x5e));
                    b = gm_FindChar(*(int *)(c + 0x62));
                    if ((a == 0 || a[2] == 0x0f) && (b == 0 || b[2] == 0x0f)) {
                        loc[0] = 3;
                        loc[1] = 2;
                        cm_BeginFieldBlock(c, *(int *)(c + 4));
                        cm_AddFieldValue(1, 1, c - cm_FieldObject + 2, &loc[0]);
                        if ((unsigned char)c[0xd] < 2)
                            cm_AddFieldValue(1, 1, c - cm_FieldObject + 0xd, &loc[1]);
                        cm_SendFieldValues();
                        v = util_RandomU(0x800);
                        cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, ((v & 0xffff) + 0x800) * loc[1] * 0x20);
                        events++;
                    }
                    continue;
                }
            } else if (c[2] == 8) {
                a = gm_FindChar(*(int *)(c + 0x5e));
                b = gm_FindChar(*(int *)(c + 0x62));
                if ((a == 0 || a[2] == 0x0f) && (b == 0 || b[2] == 0x0f)) {
                    if (unk_Unk4fd090(c) != 0) {
                        gm_AddHealth(*(unsigned short *)c, 0);
                        m--;
                    }
                    continue;
                }
                goto L1;
            }
            if (ai_WorksForMaster(c) != 0) {
                events++;
                if (*(int *)(c + 0x1f0) != 0)
                    kids++;
            }
        }
    }
    if (kids < 6 && util_RandomU(4) == 0) {
        h = ((int (*)(int, int, int, int))cm_SendCreateCharacter)(0x10, -1, -1, util_RandomU(4) + 0x10);
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        if (cm_GetSlotState(h) == 1) {
            events++;
            c = (char *)cm_GetSlotValue(h);
            r = ((util_RandomU(0x2000) & 0xffff) + 10000) * 0x40;
            cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, r);
        }
    }
    if ((float)m <= 652.80005f) {
        if (events < 0x1e)
            t = (0x28 - events) / 2;
        else if (events < 0x28)
            t = (util_RandomU(2) & 0xffff) + 1;
        else
            t = 0;
        while (t > 0) {
            v = util_RandomU(2);
            v = util_RandomU((v & 0xffff) + 1);
            h = ((int (*)(int, int, int, int, int, int))cm_SendCreateCharacter)(3, -1, -1, 0x14, -1, gm_TitleNumber((v & 0xffff) + 1));
            if (h != -1) {
                while (cm_GetSlotState(h) == 0)
                    sv_PumpNet();
            }
            if (cm_GetSlotState(h) == 1) {
                loc[0] = (util_RandomU(0xc) & 0xffff) + 10;
                c = (char *)cm_GetSlotValue(h);
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, ((util_RandomU(0x400) & 0xffff) + 0x400) * 0x40);
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x167, &loc[0]);
                cm_SendFieldValues();
            }
            t--;
        }
    }
}

extern char d2_GfxDir[256];
/* txt_MaleFirstNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FemaleFirstNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FamilyNames: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_Unk6f1bb4 = 0;  /* 0x6f1bb4 */
extern char gm_CharacterStatistics[128];
char * ai_Unk60c1ec = (void *)((unsigned)&he_Events + 0xc234);  /* 0x60c1ec, the memory pool tag */
unsigned short gm_Unk507690(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8);                     /* 0x507690 */
int gm_Unk503f30();                     /* 0x503f30 */
int gm_Unk503f50();                     /* 0x503f50 */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
float gm_CharAge(unsigned short id);  /* 0x505e30 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void mem_Free(void *p);                    /* 0x422620 */

/* 0x4fd5c0, picks two characters (kinds 0 .. 5, active, old enough (age +10 not below the float +0x20), with a chance by the number of the relatives at +0x68 (table 0x60 0x4b 0x28 0x17 9 0 of 256) against random 1 .. 256; 16 tries each, the range shrinking by 3/4 on a free slot) and starts the event 0x507690 (0, id1, id2, param, 0, 0, 0, 2). Returns its result. ROUGH */
unsigned unk_Unk4fd5c0(int param)
{
    int tab[6];
    int tries, cnt, k, range;
    unsigned short a, b;
    unsigned r;
    int *q;
    char *c;

    tab[0] = 0x60;
    tab[1] = 0x4b;
    tab[2] = 0x28;
    tab[3] = 0x17;
    tab[4] = 9;
    tab[5] = 0;
    range = 0x300;
    for (tries = 0; tries < 0x10; tries++) {
        a = util_RandomU(range);
        c = ai_Chars[a];
        r = a * 0xf;
        if (*(short *)c == -1) {
            range = (range * 3) >> 2;
            r = range;
        } else if ((unsigned char)c[2] > 5 || (float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20) || c[8] == 0) {
            a = 0xffff;
            r = 0;
        } else {
            q = (int *)(c + 0x68);
            cnt = 0;
            for (k = 5; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            r = (util_RandomU(0x100) & 0xffff) + 1;
            if ((int)r < tab[cnt])
                break;
        }
    }
    if (a == 0xffff)
        return r & 0xffff0000;
    for (tries = 0; tries < 0x10; tries++) {
        b = util_RandomU(range);
        c = ai_Chars[b];
        if (*(short *)c == -1)
            range = (range * 3) >> 2;
        else if ((unsigned char)c[2] > 5 || (float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20) || c[8] == 0)
            b = 0xffff;
        else {
            q = (int *)(c + 0x68);
            cnt = 0;
            for (k = 5; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            r = (util_RandomU(0x100) & 0xffff) + 1;
            if ((int)r < tab[cnt])
                break;
        }
        r = tries + 1;
    }
    if (b != 0xffff)
        return gm_Unk507690(0, *(int *)(ai_Chars[a] + 4), *(int *)(ai_Chars[b] + 4), param, 0, 0, 0, 2);
    return r & 0xffff0000;
}
/* 0x4fd7e0, 1 when the character id is younger than its lifetime float (+0x20): age (+10) below it */

int unk_Unk4fd7e0(unsigned id)
{
    if ((float)*(unsigned short *)(ai_Chars[id & 0xffff] + 10) < *(float *)(ai_Chars[id & 0xffff] + 0x20))
        return 1;
    return 0;
}
/* 0x4fd820, finds a free home among the buildings of the group (1, 5, 1) (kind capacity 0x4f8890 above the occupancy byte +0x65, flag +0x5a bit 0 clear): the nearest to the building of the object o (+0x61) when both have one, else the one with the lowest occupancy ratio; none: a random one of the group (1, 5, 0x16) with the flag clear. ROUGH */

char *unk_Unk4fd820(int unused, char *o)
{
    float best, d, dx, dy, dz;
    char *b, *pick;
    unsigned cap, pickcap, c2;
    int pos[3], pos2[3];
    int list[16];
    int n, k;
    int s[4], s2[4];
    float v1[3], v2[3];

    pick = 0;
    best = 1e+08f;
    ord_GetKindRecord(1, (int *)&cap);
    pickcap = cap;
    b = (char *)gm_BuildQuery(1, 5, 1);
    if (b != 0) {
        do {
            ord_GetKindRecord(*(unsigned char *)b, (int *)s);
            c2 = s[0];
            if ((unsigned short)(unsigned char)b[0x65] < (unsigned short)c2 && (b[0x5a] & 1) == 0) {
                if (o == 0 || *(int *)(o + 0x61) == 0 || *(int *)(b + 0x61) == 0) {
                    if (pick == 0) {
                        pickcap = c2;
                        pick = b;
                    } else {
                        cap = pickcap & 0xffff;
                        if ((float)(unsigned char)b[0x65] / (float)(c2 & 0xffff) < (float)(unsigned char)pick[0x65] / (float)cap)
                            pick = b;
                    }
                } else {
                    d3_PointToWorld(*(int *)(b + 0x61), *(int *)(b + 0x61) + 0x4c, v1);
                    d3_PointToWorld(*(int *)(o + 0x61), *(int *)(o + 0x61) + 0x4c, v2);
                    dx = v1[0] - v2[0];
                    dy = v1[1] - v2[1];
                    dz = v1[2] - v2[2];
                    d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    if (d < best) {
                        pickcap = c2;
                        pick = b;
                        best = d;
                    }
                }
            }
            b = (char *)gm_BuildNext();
        } while (b != 0);
        if (pick != 0)
            return pick;
    }
    b = (char *)gm_BuildQuery(1, 5, 0x16);
    n = 0;
    memset(list, 0, sizeof(list));
    for (; b != 0 && (b[0x5a] & 1) == 0 && n < 0x10; n++) {
        list[n] = (int)b;
        b = (char *)gm_BuildNext();
    }
    return (char *)list[util_RandomU(n) & 0xffff];
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
/* 0x4fda70, finds a free grown character (not flagged 0x2000 at +0x1cc, kind < 2, not married (+0x5e..), by job (+0x164 = param_3) and, with param_2 = 0, the group of 0x503f30 (param_3); with param_2: the title byte +0x165 = param_2 or 0, the age float (+0x1c) not above the age, 0x503f50) whom the master (the record at +0x27 of o = param_1, or none) is not in debt with (0x50c520 > -0x1e), flags it 0x2000 and returns it. ROUGH */

short *unk_Unk4fda70(char *o, unsigned a, unsigned b)
{
    int i, g;
    char *c;

    if (a == 0) {
        if (b != 0) {
            g = gm_Unk503f30(b & 0xff);
            for (i = 0; i < 0x300; i++) {
                c = ai_Chars[i];
                if (*(short *)c != -1 && (*(unsigned *)(c + 0x1c8) & 0x2000) == 0 && (unsigned char)c[2] < 2) {
                    if ((unsigned char)c[0x164] == b && *(int *)(c + 0x16c) == 0 && (g == 2 || (unsigned char)c[9] == g)) {
                        if (*(short *)(o + 0x27) == -1 || gm_Opinion(*(short *)(o + 0x27), i) > -0x1e) {
                            *(unsigned *)(c + 0x1c8) |= 0x2000;
                            return (short *)c;
                        }
                    }
                }
            }
            return 0;
        }
    } else {
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (*(short *)c != -1 && (*(unsigned *)(c + 0x1c8) & 0x2000) == 0 && (unsigned char)c[2] < 2) {
                if (((unsigned char)c[0x165] == a || c[0x165] == 0) && *(int *)(c + 0x16c) == 0) {
                    if (*(float *)(c + 0x20) <= (float)*(unsigned short *)(c + 10) && (unsigned char)c[0x164] == b) {
                        g = gm_Unk503f50(a);
                        if (g == 2 || (unsigned char)c[9] == g) {
                            if (*(short *)(o + 0x27) == -1 || gm_Opinion(*(short *)(o + 0x27), i) > -0x1e) {
                                *(unsigned *)(c + 0x1c8) |= 0x2000;
                                return (short *)c;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x4fdc10, the object of the group a building kind supplies for the building record b (kind 1: 0x12, 2: 0x13, 0x1d: 0xfd, 0x16: 0x120), through 0x4ffe90 (its object id, 1, 0, n) */

char *unk_Unk4fdc10(unsigned char *b)
{
    char k;

    k = ai_BTypes[*b * 0x24d];
    if (k == 1)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x12);
    if (k == 2)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x13);
    if (k == 0x1d)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
    if (k == 0x16)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x120);
    return 0;
}
/* 0x4fdcb0, the home of the character c: its building (+0x170) with the object of 0x4fdc10 (out3 = its id, -1 without) and out2 = the building id (+1); without one the next building (starting at 0x6f1bb4, 256 records of 0xa9 bytes) of kind 3 / 0x16 / 0x0f. Returns the building, 0 when none */

char *unk_Unk4fdcb0(char *c, int *out2, int *out3)
{
    unsigned i;
    unsigned char k;
    char *o;
    int n;

    i = ai_Unk6f1bb4;
    if (*(int *)(c + 0x170) != 0) {
        o = unk_Unk4fdc10(*(unsigned char **)(c + 0x170));
        if (o == 0)
            *out3 = -1;
        else
            *out3 = *(int *)(o + 2);
        *out2 = *(int *)(*(int *)(c + 0x170) + 1);
        return *(char **)(c + 0x170);
    }
    for (n = 0xff; n != 0; n--) {
        k = ai_Blds[i * 0xa9];
        if (k != 0 && (ai_BTypes[k * 0x24d] == 3 || ai_BTypes[k * 0x24d] == 0x16 || ai_BTypes[k * 0x24d] == 0x0f)) {
            ai_Unk6f1bb4 = i + 1;
            *out3 = -1;
            *out2 = *(int *)(ai_Blds + i * 0xa9 + 1);
            return ai_Blds + i * 0xa9;
        }
        i = (i + 1) % 256;
    }
    return 0;
}
/* 0x4fddb0, creates a character: flags bit 0 = 0x4fd5c0 (a pair born from the group), else 0x507690 (p2, -1, -1, p3, 0, p4, p5, 2); bit 1: stores the job bytes p4 / p5 (+0x164 / +0x165); bit 2: puts it on the stage of the building p6 (0x4fbd10). Returns the character or 0 */

short *unk_Unk4fddb0(unsigned char flags, int p2, int p3, unsigned char p4, unsigned char p5, char *p6)
{
    unsigned id;
    char *o;

    if ((flags & 1) == 0) {
        id = gm_Unk507690(p2, -1, -1, p3, 0, p4, p5, 2);
        id &= 0xffff;
    } else {
        id = unk_Unk4fd5c0(p3);
        id &= 0xffff;
        if (id == 0xffff)
            return 0;
        if (flags & 2) {
            ai_Chars[id][0x165] = p5;
            ai_Chars[id][0x164] = p4;
        }
    }
    if (id == 0xffff)
        return 0;
    if ((flags & 4) != 0 && p6 != 0 && (o = unk_Unk4fdc10((unsigned char *)p6)) != 0)
        gm_PlaceOnStage(*(int *)(ai_Chars[id] + 4), *(int *)(p6 + 1), *(int *)(o + 2), 0, 0);
    return (short *)ai_Chars[id];
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
    ai_Unk6f1b90 = 0;
    sprintf(path, "%sCharacter_Names.dat", d2_GfxDir);
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            ai_Unk6f1b90++;
        } while ((f->_flag & 0x10) == 0);
        fclose(f);
    }
    ai_Unk6f1b94 = (char *)mem_Alloc(ai_Unk6f1b90 * 0x30, &ai_Unk60c1ec);
    ai_Unk6f1b90 = 0;
    f = fopen(path, "rb");
    if (f != 0) {
        do {
            fread(rec, 0x80, 1, f);
            s = rec;
            d = ai_Unk6f1b94 + ai_Unk6f1b90 * 0x30;
            ai_Unk6f1b90++;
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
    if (ai_Unk6f1b94 != 0) {
        mem_Free(ai_Unk6f1b94);
        ai_Unk6f1b94 = 0;
    }
}
/* 0x4fdfd0, the character statistics (0x91b500, 0x68 bytes): [0] the number of active persons of kind < 10, a histogram of 0x505e30 (i) / 10 (clamped, [1 ..]), the float [9] the share of children below their lifetime (0x4fd7e0, in percent at the end), the sums of the healths 0x4fc820 / 0x4fc890 per kind ([10 ..] / [18 ..]) turned into averages. ROUGH */

void unk_Unk4fdfd0(void)
{
    int *s = (int *)gm_CharacterStatistics;
    float *f = (float *)gm_CharacterStatistics;
    float v;
    int i, k;
    char *c;

    memset(gm_CharacterStatistics, 0, 0x68);
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0 && (unsigned char)c[2] < 10) {
            s[0]++;
            v = gm_CharAge(i) * 0.1f;
            if (v >= 7.0f)
                v = 6.0f;
            if (v < 0.0f)
                v = 0.0f;
            k = (int)v;
            s[1 + k]++;
            if (unk_Unk4fd7e0(i) != 0)
                f[9] += 1.0f;
            f[10 + (unsigned char)c[2]] = gm_CharacterHealth((short *)c) + f[10 + (unsigned char)c[2]];
            f[18 + (unsigned char)c[2]] = unk_Unk4fc890((short *)c) + f[18 + (unsigned char)c[2]];
        }
    }
    for (i = 18; i < 26; i++) {
        f[i - 8] = (f[i - 8] / f[i]) * 100.0f;
        f[i] = f[i] / (float)s[i - 17];
    }
    f[9] = (f[9] / (float)s[0]) * 100.0f;
}

#pragma pack(push, 1)
typedef struct GCLOCK {                 /* the 14-byte game clock (0x770ae0) */
    int day;
    unsigned short hour;
    int min;
    int sec;
} GCLOCK;
#pragma pack(pop)

extern int ai_Unk875360[32];
extern int ai_Unk919f80[732];
extern char * ai_Goods;
extern unsigned ai_Unk91ac40[136];
extern struct AIOBJ *ai_Objects;
extern int ai_Unk6178c8[];
extern char ai_Unk61aa38[], ai_Unk61aa20[], ai_Unk61aa00[], ai_Unk61a9ec[];  /* 0x61aa38, 0x61aa20, 0x61aa00, 0x61a9ec */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void util_Randomize();                     /* 0x42ec80 */

/* 0x4fe140, adds hours / minutes / seconds (any sign for the hours) to the clock c, carrying into the days (the minutes and seconds only upward) */
void gm_AddTime(GCLOCK *c, int hours, int mins, int secs)
{
    unsigned h;
    int t;
    int d;

    secs = c->sec + secs;
    t = c->min + secs / 0x3c;
    c->sec = secs % 0x3c;
    mins = t + mins;
    c->min = t;
    c->hour = (short)c->hour + (short)(mins / 0x3c);
    c->min = mins % 0x3c;
    h = c->hour + hours;
    if (0x17 < (int)h) {
        d = h / 0x18;
        h = h % 0x18;
        c->day = c->day + d;
    }
    if ((int)h < 0) {
        t = (-h - 1) / 0x18 + 1;
        c->day = c->day - t;
        c->hour = (short)h + (short)t * 0x18;
        return;
    }
    c->hour = (short)h;
}
/* 0x4fe1e0, sets the time of day of the clock c (hour, minute, second) */

void gm_SetTime(GCLOCK *c, unsigned char hour, unsigned min, unsigned sec)
{
    c->hour = hour;
    c->sec = sec & 0xff;
    c->min = min & 0xff;
}
/* 0x4fe210, compares two clocks: -1 when a is earlier, 1 when later, 0 when equal */

int gm_CompareTime(GCLOCK *a, GCLOCK *b)
{
    int x, y;
    int da = a->day, db = b->day;

    if (da < db)
        return -1;
    if (db < da)
        return 1;
    x = a->sec + (a->min + a->hour * 0x3c) * 0x3c;
    y = b->sec + (b->min + b->hour * 0x3c) * 0x3c;
    if (x < y)
        return -1;
    return y < x;
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
/* 0x4fe370, builds the building lookup tables: 0x875360 (the first building of the object classes 6 .. 16, fixed numbers) and 0x919f80 (for every object: the building of its class that follows from the kind runs: the first building of the class plus (the class length * the position of this building in its run of equal kinds / the run length)); objects without a building get the first building of their class. ROUGH */

void gm_InitBuildingTables(void)
{
    int run, len, i, j, k, w, u, base, cnt, v;
    char cur, kind;
    char *p, *q;
    unsigned short *wp;

    memset(ai_Unk875360, 0, 0x64);
    for (i = 0; i < 0x2dc; i++)
        ai_Unk919f80[i] = 0x58;
    run = 0;
    len = 0;
    ai_Unk875360[8] = 0x2f;
    ai_Unk875360[7] = 0x32;
    ai_Unk875360[6] = 0x29;
    ai_Unk875360[10] = 0x21;
    ai_Unk875360[14] = 0x1e;
    ai_Unk875360[15] = 0x1f;
    ai_Unk875360[16] = 0x20;
    ai_Unk875360[11] = 0x17;
    ai_Unk875360[12] = 0x35;
    ai_Unk875360[9] = 0x14;
    cur = 'X';
    p = ai_BTypes;
    for (i = 0; i < 0x58; i++, p += 0x24d) {
        kind = *p;
        if (kind == cur) {
            run++;
        } else {
            run = 0;
            len = 0;
            j = i;
            for (q = p; (cur = kind, j < 0x58 && *q == kind); q += 0x24d) {
                len++;
                j++;
            }
        }
        if (p[0x22] != 0) {
            wp = (unsigned short *)(p + 0x23);
            for (k = 0; k < (unsigned char)p[0x22]; k++, wp++) {
                if (*wp != 0xffff) {
                    u = *wp & 0x7fff;
                    base = ai_Unk875360[(unsigned char)ai_Goods[u * 0x41 + 0x21]];
                    q = ai_BTypes + base * 0x24d;
                    kind = *q;
                    cnt = 0;
                    for (j = base; j < 0x58 && *q == kind; q += 0x24d) {
                        cnt++;
                        j++;
                    }
                    v = base + (cnt * run) / len;
                    if (v < ai_Unk919f80[u])
                        ai_Unk919f80[u] = v;
                }
            }
        }
    }
    for (i = 0; i < 0x2dc; i++) {
        if (ai_Unk919f80[i] == 0x58)
            ai_Unk919f80[i] = ai_Unk875360[(unsigned char)ai_Goods[0x21 + i * 0x41]];
    }
}
/* 0x4fe570, loads the building and object tables of the data path root: resets the player records at 0x7d6a64 (stride 0xaa4 ints, each gets the 0xaa0 ints 0x6178c8), allocates the building prototypes (0xca78 = 0x58 * 0x24d, "f3_gm:lpGebaeudePROT") and instances (0xa900, "..INST"), reads "<root>A_Geb.dat" (0x58 records of 0x24d bytes), the object prototypes (0xb9dc = 0x2dc * 0x41, "..PROT"), instances (0x14f218) and "<root>A_Obj.dat"; builds the lookup tables (0x4fe370, 0x503e80) and adds the count of the flagged (bit 15) objects of the kinds 2 / 6 (except 0xfd) to each building's byte +0x21. Returns 0, or -1 .. -6 for the failed step. ROUGH */

int gm_LoadTables(char *root)
{
    char path[512];
    char *p;
    FILE *f;
    int i, n, cnt;
    unsigned short *w;
    unsigned short u;
    char k;
    int *dst;

    p = ai_Unk7e3ff8;
    dst = ai_Unk7d6a64;
    do {
        dst[-1] = *(int *)(p - 0x50);
        dst[0] = 1000;
        dst[1] = *(int *)p;
        dst[2] = 1000;
        memcpy(dst + 3, ai_Unk6178c8, 0xaa0 * 4);
        p += 0xbd * 4;
        dst += 0xaa4;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk7e3ff8 + 0xb28) + 0x39c));
    ai_BTypes = (char *)mem_Alloc(0xca78, ai_Unk61aa38);
    if (ai_BTypes == 0)
        return -1;
    ai_Blds = (char *)mem_Alloc(0xa900, ai_Unk61aa20);
    memset(ai_Blds, 0, 0xa900);
    if (ai_Blds == 0)
        return -2;
    sprintf(path, "%s%s", root, "A_Geb.dat");
    f = fopen(path, "rb");
    if (f == 0)
        return -3;
    fread(ai_BTypes, 0x24d, 0x58, f);
    fclose(f);
    ai_Goods = (char *)mem_Alloc(0xb9dc, ai_Unk61aa00);
    if (ai_Goods == 0)
        return -4;
    ai_Objects = (struct AIOBJ *)mem_Alloc(0x14f218, ai_Unk61a9ec);
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
                k = ai_Goods[(u & 0x7fff) * 0x41];
                if ((k == 2 || k == 6) && (u & 0x7fff) != 0xfd && (short)u < 0)
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
int gm_Unk4ff950();                     /* 0x4ff950 */
int gm_Unk4ff8c0();                     /* 0x4ff8c0 */
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
/* 0x4fe830, runs through the object instances (0x5008 records of 0x43 bytes: word +0 the prototype, int +6 the owner id): the owner (found by 0x4fea50 as a building / object / character) gets the instance back (0x4ffff0 on its list at +0x5d / +0x14 / +0x178), an instance without a live owner is freed (0x500080); then the table is cleared */

void gm_ClearObjects(void)
{
    int i, t;
    int b, o, c;

    i = 0;
    do {
        if (*(short *)((char *)ai_Objects + i) != 0) {
            gm_FindById(&b, &o, &c, *(int *)((char *)ai_Objects + i + 6));
            if (b == 0) {
                if (o != 0) {
                    t = o + 0x14;
                    goto have;
                }
                if (c != 0) {
                    t = c + 0x178;
                    goto have;
                }
            } else {
                t = b + 0x5d;
            have:
                if (t != 0) {
                    gm_RemoveObject(t, *(short *)((char *)ai_Objects + i));
                    goto next;
                }
            }
            gm_FreeObjectExtra((char *)ai_Objects + i);
            *(short *)((char *)ai_Objects + i) = 0;
            f3gm_ObjectCount--;
        }
    next:
        i += 0x43;
        if (i > 0x14f217) {
            memset(ai_Objects, 0, 0x14f218);
            return;
        }
    } while (1);
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
    char k;

    k = *(char *)(ai_Goods + n * 0x40 + n);
    if (k != 1 && k != 0x1a && k != 0x0b && k != 0x1c && k != 0x1b && k != 3 && k != 4)
        return 0;
    return 1;
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
    short *p;
    char *b;
    int i;

    if (ai_Goods != 0) {
        if (bld != 0)
            *bld = 0;
        if (obj != 0)
            *obj = 0;
        if (chr != 0) {
            *chr = 0;
            p = (short *)ai_Chars[0];
            i = 0;
            do {
                if (*p != -1 && *(int *)(p + 2) == id) {
                    *chr = (int)p;
                    return 3;
                }
                p += 0x10e;
                i++;
            } while (i < 0x300);
        }
        if (bld != 0) {
            i = 0;
            b = ai_Blds;
            do {
                if (*b != 0 && *(int *)(b + 1) == id) {
                    *bld = (int)b;
                    return 1;
                }
                i++;
                b += 0xa9;
            } while (i < 0x100);
        }
        if (obj != 0 && (i = 0, p = (short *)ai_Objects, 0 < f3gm_ObjectCount)) {
            do {
                if (*p != 0) {
                    if (*(int *)(p + 1) == id) {
                        *obj = (int)p;
                        return 2;
                    }
                    i++;
                }
                p = (short *)((char *)p + 0x43);
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
/* 0x4feb60, resets the goods records of the player p (0x53 records of 0x80 bytes from 0x7d6aa0 + p * 0x2a90, the template 0x6178f8): copies the first int and the one 0x24 bytes before, then by the kind of its object prototype (+0x30 the prototype number): kinds 0x17 / 0x25 take 0x4ff950 (p, i), the others the float of 0x4ff8c0 (p, i); +8 = 0x5087f0 (prototype, 100); the float at -0x10 is copied to -0xc. ROUGH */

void gm_InitPlayerRecord(int p)
{
    int i;
    char *rec;
    int *src;
    char *pc;
    float v;

    i = 0;
    src = ai_Unk6178f8;
    rec = ai_Unk7d6aa0 + p * 0x2a90;
    do {
        pc = ai_Goods + *(unsigned short *)(rec - 0x30) * 0x41;
        *(int *)(rec - 0x24) = src[-9];
        *(int *)rec = src[0];
        if (*pc == 0x17 || *pc == 0x25) {
            *(int *)(rec + 4) = gm_Unk4ff950(p, i);
        } else {
            *(float *)(rec - 0x10) = (float)gm_Unk4ff8c0(p, i);
        }
        v = gm_ItemPrice(*(unsigned short *)(rec - 0x30), 100);
        *(float *)(rec + 8) = v;
        *(int *)(rec - 0xc) = *(int *)(rec - 0x10);
        src += 0x20;
        i++;
        rec += 0x80;
    } while ((int)src < ((unsigned)((unsigned)ai_Unk6178f8 + 0x2a50) + 0x30));
}

float gm_Unk4ffa10();                   /* 0x4ffa10 */
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
            return gm_Unk4ffa10(p, i);
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
/* 0x4fffc0, the first free record of the object instance table (0x5008 records of 0x43 bytes), 0 when it is full */

short *gm_FindFreeObject(void)
{
    int i;
    short *p;

    i = 0;
    p = (short *)ai_Objects;
    while (i <= 0x5007) {
        if (*p == 0)
            return p;
        i++;
        p = (short *)((char *)p + 0x43);
    }
    return 0;
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
/* 0x4ff450, a random market shake: with chance 1 / 2, 0 .. 1 times (0x2 draws) a random goods record of the player 0x6f1bc5 (0x54 + 1 records) that is in use gets a copy (0x80 bytes) with the float at +0x20 copied to +0x24 halved or raised by 1.5 (and the value 0x5087f0 (id, 100) likewise at +0x38), handed to 0x4c5710 (copy, player). ROUGH */

void gm_RandomMarketShake(void)
{
    int rec[0x20];
    unsigned n, k;
    int *g;
    float v;
    int i;

    if (util_RandomU(2) != 0) {
        n = util_RandomU(2) & 0xffff;
        for (; n != 0; n--) {
            k = util_RandomU(0x54) & 0xffff;
            g = (int *)(init_StockBlocks + ai_OwnPlayer * 0x2a90 + k * 0x80);
            if (*(short *)g != 0 && g[1] != 0) {
                for (i = 0; i < 0x20; i++)
                    rec[i] = g[i];
                if (util_RandomU(1) == 0) {
                    *(float *)(rec + 9) = *(float *)(rec + 8) * 0.5f;
                    v = (float)(gm_ItemPrice(*(short *)g, 100) * 0.5);
                } else {
                    *(float *)(rec + 9) = *(float *)(rec + 8) * 1.5f;
                    v = (float)(gm_ItemPrice(*(short *)g, 100) * 1.5);
                }
                *(float *)(rec + 0xe) = v;
                cm_SendGoodsDelta(rec, ai_OwnPlayer);
            }
        }
    }
}

extern int ai_Unk61a67c;
extern int ai_Unk61a680;
extern short ai_Unk61a684;
#define ALIGN1 __declspec(align(1))
extern ALIGN1 char ai_Unk61a686[550];
extern char ai_Unk6f1be0, ai_Unk6f1be8, ai_Unk6f1be9, ai_Unk6f1bea;  /* 0x6f1be0, 0x6f1be8, 0x6f1be9, 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
extern int ai_Unk6f1be4;
extern unsigned short * ai_Unk6f1bf4;
extern int ai_Unk7e3f38;
extern int ai_Unk8753e0[20488];
int gm_FindMarketObject(int id);                     /* 0x500ec0 */
int gm_FindPlayerBuilding();                     /* 0x501000 */

/* 0x4ffa10, the price of the goods record n of the player p (0x80 bytes each from 0x7d6a70 + p * 0x2a90): the base price 0x5087f0 (id, 100) when the player has no market object (0x500ec0, or for another player the object of 0x501000 with the group (2, 0, 0x1db, 6)); else the market's stock (the int at +0xe of the goods object of the market object (+0x14 list, id)) against the record's wanted amount (+0x20) and mean (+0x24) gives a factor limited to 0.25 .. 5.0, the price moves 0.4 of the way from the stored price (+0x38) to base * factor and is at least 512.0 (or the base when that is above 512). ROUGH */
float gm_Unk4ffa10(int p, int n)
{
    char *rec;
    int off;
    unsigned id;
    char *pr;
    int mk;
    int m2;
    float stock, cur;
    double f;
    float r;

    off = p * 0x2a90 + n * 0x80;
    id = *(unsigned short *)(init_StockBlocks + n * 0x80 + p * 0x2a90);
    pr = ai_Goods + id * 0x41;
    mk = gm_FindMarketObject(id);
    if (mk == 0 && p != ai_OwnPlayer && (m2 = gm_FindPlayerBuilding(p)) != 0)
        mk = (int)gm_Query(*(int *)(m2 + 0x5d), 2, 0, 0x1db, 6);
    stock = 0.0f;
    if (mk == 0) {
        r = gm_ItemPrice(id, 100);
    } else {
        m2 = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, id);
        if (m2 != 0)
            stock = (float)*(int *)(m2 + 0xe);
        cur = *(float *)(ai_Unk7d6aa8 + off);
        if (stock <= *(float *)(ai_Unk7d6a90 + off) || *pr == 0x17)
            f = 1.0;
        else if (1.0 <= stock)
            f = (double)*(float *)(ai_Unk7d6a90 + off) / (double)stock;
        else
            f = (double)*(float *)(ai_Unk7d6a90 + off) / 1.0;
        f = f * ((double)*(float *)(ai_Unk7d6a94 + off) / (double)*(float *)(ai_Unk7d6a90 + off));
        if (f <= 0.25)
            f = 0.25;
        if (5.0 <= f)
            f = 5.0;
        r = (float)(((float)f * gm_ItemPrice(id, 100) - cur) * 0.4 + *(float *)(ai_Unk7d6aa8 + off));
        if (*(float *)(ai_Unk7d6aa8 + off) <= 0.0f) {
            r = gm_ItemPrice(id, 100);
            if (512.0 <= r)
                return gm_ItemPrice(id, 100);
            return 512.0f;
        }
    }
    return r;
}
/* 0x4ffcc0, the next match of the object query set up by 0x4ffe90: walks the instance list from the current record (0x6f1bf4; with the scan flag through the whole table by 0x43-byte steps, else along the next pointers at +0x3f, with the child stack 0x8753e0 when descending), tests the query (prototype 0x61a67c, owner id 0x61a680, 0x50a0b0 value 0x61a684, prototype kind 0x61a686) and returns the last match (the first with flag 0x6f1bea), 0 when none. ROUGH */

unsigned short *gm_QueryNext(void)
{
    unsigned short *p, *q, *res;
    int m, sp;
    short s;

    p = ai_Unk6f1bf4;
    res = 0;
    m = 0;
    if (ai_Unk6f1bf4 == 0)
        return 0;
    if (ai_Unk6f1be0 != 0) {
        ai_Unk6f1be0 = 0;
        if (ai_Unk6f1be9 != 0 && *(int *)(ai_Unk6f1bf4 + 10) != 0) {
            ai_Unk8753e0[ai_Unk7e3f38] = *(int *)(ai_Unk6f1bf4 + 10);
            ai_Unk7e3f38++;
        }
    }
    sp = ai_Unk7e3f38;
    if (ai_Unk6f1be4 < f3gm_ObjectCount) {
        do {
            if (p == 0)
                return res;
            if (m)
                return res;
            q = ai_Unk6f1bf4;
            while (ai_Unk6f1bf4 = p, *p == 0) {
                q = (unsigned short *)((char *)ai_Unk6f1bf4 + 0x43);
                p = q;
            }
            if (ai_Unk6f1bea != 0)
                m = 1;
            if (ai_Unk61a67c == 0x7fff || *ai_Unk6f1bf4 == ai_Unk61a67c) {
                if (ai_Unk61a67c != 0x7fff)
                    m = 1;
                if (ai_Unk61a680 != -1) {
                    if (*(int *)(ai_Unk6f1bf4 + 1) != ai_Unk61a680)
                        goto no;
                    m = 1;
                }
                if (ai_Unk61a684 != -1) {
                    s = (short)gm_Unk50a0b0(ai_Unk6f1bf4);
                    sp = ai_Unk7e3f38;
                    if (s != ai_Unk61a684)
                        goto no;
                    m = 1;
                }
                if (ai_Unk61a686[0] != -1)
                    m = ai_Goods[*ai_Unk6f1bf4 * 0x41] == ai_Unk61a686[0];
            } else {
            no:
                m = 0;
            }
            if (ai_Unk6f1be8 == 0) {
                if (m)
                    res = ai_Unk6f1bf4;
                p = *(unsigned short **)((char *)ai_Unk6f1bf4 + 0x3f);
                ai_Unk6f1bf4 = p;
                if (p == 0) {
                    if (ai_Unk6f1be9 != 0 && sp != 0) {
                        p = (unsigned short *)ai_Unk8753e0[sp - 1];
                        sp--;
                        ai_Unk6f1bf4 = p;
                        ai_Unk7e3f38 = sp;
                        if (*(int *)(p + 10) != 0) {
                            ai_Unk8753e0[sp] = *(int *)(p + 10);
                            sp++;
                            ai_Unk7e3f38 = sp;
                        }
                    }
                } else if (*(int *)(p + 10) != 0 && ai_Unk6f1be9 != 0) {
                    ai_Unk8753e0[sp] = *(int *)(p + 10);
                    sp++;
                    ai_Unk7e3f38 = sp;
                }
            } else {
                if (m)
                    res = ai_Unk6f1bf4;
                p = (unsigned short *)((char *)ai_Unk6f1bf4 + 0x43);
                ai_Unk6f1be4++;
                ai_Unk6f1bf4 = p;
            }
        } while (ai_Unk6f1be4 < f3gm_ObjectCount);
    }
    return res;
}
/* 0x4ffe90, starts an object query over the list (or, with the flag 7, over the whole instance table) and returns the first match (0x4ffcc0): n criteria follow as (kind, value) pairs: kind 0 = prototype, 1 = owner id, 3 = the 0x50a0b0 value, 4 = the prototype kind (each with a value), 5 = stop at the first match, 6 = descend into the children, 7 = scan the whole table (without value). Not set criteria match everything; 0 without a table or list. Used as gm_Query (list, 2, 6, 0, 0x115) .. all over the exe */

char *gm_Query(void *list, int n, ...)
{
    int *ap;
    char scan, kids;
    int i;

    if (ai_Goods != 0) {
        scan = 0;
        kids = 0;
        ai_Unk61a67c = 0x7fff;
        ai_Unk61a680 = -1;
        ai_Unk61a684 = -1;
        ai_Unk61a686[0] = (char)0xff;
        ai_Unk6f1bea = 0;
        ai_Unk6f1be8 = 0;
        ai_Unk6f1be9 = 0;
        ai_Unk6f1be0 = 1;
        ai_Unk6f1be4 = 0;
        ai_Unk7e3f38 = 0;
        if (n > 0) {
            ap = &n + 1;
            for (i = n; i != 0; i--) {
                switch ((char)*ap) {
                case 0:
                    ai_Unk61a67c = (unsigned short)ap[1];
                    ap += 2;
                    break;
                case 1:
                    ai_Unk61a680 = ap[1];
                    ap += 2;
                    break;
                case 3:
                    ai_Unk61a684 = (short)ap[1];
                    ap += 2;
                    break;
                case 4:
                    ai_Unk61a686[0] = (char)ap[1];
                    ap += 2;
                    break;
                case 5:
                    ai_Unk6f1bea = 1;
                    ap += 1;
                    break;
                case 6:
                    kids = 1;
                    ap += 1;
                    break;
                case 7:
                    scan = 1;
                    ap += 1;
                    break;
                default:
                    ap += 1;
                }
            }
            ai_Unk6f1be8 = scan;
            ai_Unk6f1be9 = kids;
            if (scan != 0) {
                if (kids != 0) {
                    ai_Unk6f1be0 = 1;
                    ai_Unk6f1be4 = 0;
                    ai_Unk7e3f38 = 0;
                    return 0;
                }
                ai_Unk6f1bf4 = (unsigned short *)ai_Objects;
                return (char *)gm_QueryNext();
            }
        }
        if (list != 0) {
            ai_Unk6f1bf4 = (unsigned short *)list;
            return (char *)gm_QueryNext();
        }
    }
    return 0;
}

extern struct AIBLD *ai_MarketBuilding;                /* 0x6f1bc8, the market building (group 1, 5, 10) */
extern char * cm_FieldObject;                /* 0xa68d2c */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
extern char gm_StockBlocks[0xd4d0];
unsigned ai_Unk6118a8[] = { 0, 1735292266, 1430937445, 4540238, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118a8 */
unsigned ai_Unk6118f0[] = { 0, 1684365677, 1852139619, 1161907551, 1162363716, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x6118f0 */
unsigned ai_Unk611938[] = { 12, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1735550306, 1852727661, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1735550306, 1852727661, 1095589682, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1684955496, 1802659191, 1597272677, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1684955496, 1802659191, 1597141605, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1684955496, 1802659191, 1597338213, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1684955496, 1802659191, 1597207141, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1735550306, 1852727661, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701409360, 1919251571, 1414875999, 17748, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1601462881, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1919251810, 846357863, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1650813284, 1431002932, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1650813284, 1312902495, 12878, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1969578354, 1735550306, 1818588005, 1398760812, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1969578354, 1819436386, 1819437157, 1600613993, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1751344486, 1601332596, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1802728813, 1852142693, 1969775475, 1702524005, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 2053926760, 1818583398, 1601332588, 1145851731, 21569, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1818326131, 1920295532, 1701340019, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1852139844, 1264546405, 1413563730, 21077, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 1298084466, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 1298084210, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1684955496, 1802659191, 1298100837, 5131841, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x611938 */
unsigned ai_Unk612288[] = { 14, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1684955496, 1802659191, 1852404325, 1380343602, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1684955496, 1802659191, 1852404325, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1668505975, 1634887272, 1380343669, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1701409350, 1718577252, 1751476083, 1768320105, 1380343662, 3364161, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 33, 1852334963, 1701079397, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1852334963, 1701079397, 829319538, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x612288 */
unsigned ai_Unk612bd8[] = { 33, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 1684825446, 1700950625, 1919251561, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1953655159, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1953655159, 1095589683, 20046, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1953655159, 1229217586, 1380272963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 13, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 14, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 15, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 16, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 17, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 18, 1752002404, 1601335909, 1414813003, 69, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1601332583, 1313751373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1111836996, 1431002930, 4543572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1969578354, 1836213602, 1953720677, 1398764133, 1094995023, 84, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1768318575, 1919248762, 1280267103, 5521732, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 863135079, 1128875103, 5391691, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 76, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 77, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 78, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 79, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 80, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 81, 1919640910, 1851878767, 1162698612, 1163154249, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 83, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 84, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 85, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 86, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 87, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 88, 1701276026, 1919250037, 1936287085, 1601332596, 1313751373, 51, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 1147089778, 1162560329, 82, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 82, 1936943479, 1668509285, 1952866664, 1601332588, 1414813003, 12869, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1700950625, 1919251561, 1312902495, 78, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x612bd8 */
unsigned ai_Unk614518[] = { 33, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 32, 1684825446, 1700950625, 1919251561, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 31, 2053926760, 1818583398, 1769104748, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 46, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 47, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 48, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 49, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 50, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 51, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 64, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 65, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 66, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 67, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 68, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 69, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 58, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 59, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 60, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 61, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 62, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 63, 1684955496, 1802659191, 1852404325, 1380343603, 21825, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 70, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 71, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 72, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 73, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 74, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 75, 1684496749, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 34, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 35, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 36, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 37, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 38, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 39, 1919251810, 1769104743, 1180644206, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 19, 1702191426, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 20, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 21, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 22, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 23, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 24, 1702191458, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 1, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 2, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 3, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 4, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 5, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 6, 1701276026, 1919250037, 1180659305, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 40, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 41, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 42, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 43, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 44, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 45, 1701734765, 846096754, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 52, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 53, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 54, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 55, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 56, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 57, 1919251810, 1769104743, 1380343662, 21825, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 25, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 26, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 27, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 28, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 29, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 30, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 7, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 8, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 9, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 10, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 11, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 12, 1919251810, 1769104743, 1180643950, 5587282, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 89, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 90, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 91, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 92, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 93, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 94, 1852334963, 1701079397, 862873970, 1095911007, 85, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1701734765, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1095589746, 3296846, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1920229744, 1701411433, 1601071474, 1430344262, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x614518 */
unsigned ai_Unk617398[] = { 0, 1869181043, 1162698606, 1297434948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 1869181043, 1298084462, 1430864965, 77, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0 };  /* 0x617398 */
int ai_Unk61772c = -1;  /* 0x61772c */
int ai_Unk617730 = -1000000;  /* 0x617730 */
float ai_Unk617734[6] = { 0.240015402f, 0.222200006f, 0.125f, 0.0555500016f, 0.0138499998f, 0.0f };  /* 0x617734, a float per number of children / relatives */
float gm_DayStartHours[] = { 8.0f, 7.0f, 8.0f, 9.0f };  /* 0x617830 / */
float gm_DayEndHours[] = { 20.0f, 21.0f, 20.0f, 19.0f };  /* 0x617840 */
int ai_Unk617850[] = { 0, 0, 1, 1, 1109393408, 16, 2, 1097859072, 16, 3, 1101004800, 16, 4, 1092616192, 4, 5, 0, 8, 6, 1090519040, 8, 7, 1084227584, 8, 8, 1077936128, 4, 9, 1094713344, 16 };  /* 0x617850, records of 3 ints ( */
/* ai_Unk7e3ff8: view of ai_PlayerSlots (tools/data_views.py) */
int ai_Unk6178c8[] = { 468, 0, 1084227584, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x6178c8 */
int ai_Unk6178f8[] = { 15, 0, 0, 146, 18, 1094713344, 21, 1094713344, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 341, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 342, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 343, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 344, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 345, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 346, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 469, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 146, 21, 0, 14, 1086324736, 8, 1094713344, 22, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 347, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 348, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 349, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 351, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 352, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 470, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 147, 18, 1086324736, 21, 1086324736, 20, 1086324736, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 353, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 354, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 355, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 356, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 357, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 358, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 471, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 148, 18, 1086324736, 21, 1086324736, 20, 1086324736, 7, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 365, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 366, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 367, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 368, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 369, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 370, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 472, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 148, 18, 1088421888, 20, 1086324736, 7, 1084227584, 14, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 371, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 372, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 373, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 374, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 375, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 376, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 473, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 147, 14, 1088421888, 8, 1088421888, 22, 1088421888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 377, 0, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 378, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 379, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 380, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 381, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 382, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 464, 0, 1112014848, -1, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 149, 18, 1104150528, 21, 1090519040, 14, 1082130432, 8, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 465, 0, 1106247680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1092616192, 21, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 466, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1073741824, 21, 1073741824, 20, 1090519040, 7, 1082130432, 0, 0, 0, 0, 0, 0, 0, 0, 467, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 149, 18, 1073741824, 20, 1073741824, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 458, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 21, 1104150528, 7, 1108869120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 459, 0, 1097859072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 21, 1105199104, 7, 1104150528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 460, 0, 1101004800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 18, 1090519040, 21, 1101004800, 20, 1082130432, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 461, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1099956224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 462, 0, 1109393408, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1105199104, 8, 1073741824, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 463, 0, 1103626240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 149, 20, 1090519040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 474, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 151, 7, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 359, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 360, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 361, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 362, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 363, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 364, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 455, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 150, 14, 1095761920, 8, 1095761920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 456, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 22, 1110966272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 457, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 22, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 686, 0, 1092616192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 150, 32, 1114636288, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1103626240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 1090519040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 552, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 731, 29, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 553, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 554, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 555, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 556, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 557, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 558, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 689, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 150, 32, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 690, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 691, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 692, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 693, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 694, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 695, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 662, 0, 1112014848, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 731, 32, 1094713344, 31, 1094713344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 663, 0, 1086324736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 664, 0, 1077936128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 665, 0, 1073741824, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666, 0, 1069547520, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 667, 0, 1067030938, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 668, 0, 1065353216, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x6178f8 */
G7GRP gm_BuildingItemGroups[37] = { { 41, { 0, 0, 468, 341, 342, 0, 0, 0, 0, 0 } }, { 42, { 0, 0, 468, 341, 342, 344, 343, 0, 0, 0 } }, { 43, { 0, 0, 468, 0, 341, 342, 344, 343, 345, 346 } }, { 50, { 469, 0, 0, 347, 348, 0, 0, 0, 0, 0 } }, { 51, { 469, 0, 347, 348, 349, 350, 0, 0, 0, 0 } }, { 52, { 469, 0, 347, 348, 349, 350, 352, 351, 0, 0 } }, { 33, { 471, 0, 0, 0, 365, 366, 0, 0, 0, 0 } }, { 34, { 471, 0, 0, 0, 365, 366, 367, 368, 0, 0 } }, { 35, { 471, 0, 0, 0, 365, 366, 367, 368, 369, 370 } }, { 23, { 472, 0, 0, 371, 372, 0, 0, 0, 0, 0 } }, { 24, { 472, 0, 0, 371, 372, 373, 375, 0, 0, 0 } }, { 25, { 472, 0, 0, 371, 372, 373, 375, 376, 374, 0 } }, { 47, { 470, 0, 0, 353, 354, 0, 0, 0, 0, 0 } }, { 48, { 470, 0, 0, 353, 354, 355, 356, 0, 0, 0 } }, { 49, { 470, 0, 0, 353, 354, 355, 356, 357, 358, 0 } }, { 53, { 0, 0, 0, 0, 473, 377, 378, 0, 0, 0 } }, { 54, { 0, 0, 0, 473, 377, 378, 379, 380, 0, 0 } }, { 55, { 0, 0, 0, 473, 377, 378, 379, 380, 381, 382 } }, { 20, { 474, 0, 0, 0, 359, 360, 0, 0, 0, 0 } }, { 21, { 474, 0, 0, 0, 359, 360, 361, 362, 0, 0 } }, { 22, { 474, 0, 0, 0, 359, 360, 361, 362, 363, 364 } }, { 72, { 552, 0, 0, 0, 553, 554, 0, 0, 0, 0 } }, { 73, { 552, 0, 0, 0, 553, 554, 555, 556, 0, 0 } }, { 74, { 552, 0, 0, 0, 553, 554, 555, 556, 557, 558 } }, { 31, { 463, 461, 462, 0, 0, 0, 0, 0, 0, 0 } }, { 75, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 76, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 77, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { 81, { 689, 0, 0, 0, 690, 691, 0, 0, 0, 0 } }, { 82, { 689, 0, 0, 0, 690, 691, 692, 693, 0, 0 } }, { 83, { 689, 0, 0, 0, 690, 691, 692, 693, 694, 695 } }, { 78, { 662, 663, 664, 0, 0, 0, 0, 0, 0, 0 } }, { 79, { 662, 663, 664, 665, 666, 0, 0, 0, 0, 0 } }, { 80, { 662, 663, 664, 665, 666, 667, 668, 0, 0, 0 } }, { 32, { 464, 465, 466, 467, 0, 0, 0, 0, 0, 0 } }, { 30, { 458, 459, 460, 0, 0, 0, 0, 0, 0, 0 } }, { 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };  /* 0x61a348, 37 item groups: a building kind and up to ten object prototypes */
int ai_NextObjectId = 1;  /* 0x61a654, the next object id */
unsigned char ai_Unk61a66c = 0xff;  /* 0x61a66c, the building query: kind class number ( */
int ai_Unk61a670 = -1;  /* 0x61a670, the query: id (-1 = any) */
short ai_Unk61a674 = 32767;  /* 0x61a674 / */
short ai_Unk61a676 = 32767;  /* 0x61a676, the query: the words + */
char ai_Unk61a678 = -1;  /* 0x61a678, the query: building type (-1 = any) */
int ai_Unk61a67c = 32767;  /* 0x61a67c, the query: object prototype ( */
int ai_Unk61a680 = -1;  /* 0x61a680, the query: owner id (-1 = any) */
short ai_Unk61a684 = -1;  /* 0x61a684, the query: */
ALIGN1 char ai_Unk61a686[550] = { 255, 0, 0, 0, 0, 0, 0, 0, 105, 105, -25, -25, 105, 4, 63, 63, -67, -67, 63, 3, 63, 63, 126, 126, 42, 2, 63, 42, 84, 84, 21, 2, 63, 21, 84, 84, 21, 1, 42, 21, 84, 84, 21, 1, -109, 105, -109, 105, -67, 4, 105, 63, 105, 63, -109, 3, 105, 42, 84, 63, 105, 2, 105, 21, 63, 42, 63, 2, 105, 21, 63, 21, 63, 1, 84, 21, 63, 21, 63, 1, 105, 126, -109, 105, -46, 4, 63, 84, 105, 63, -88, 3, 63, 63, 84, 63, 126, 2, 63, 63, 63, 21, 84, 2, 42, 63, 63, 21, 84, 1, 21, 63, 63, 21, 84, 1, -46, -88, 105, 105, 105, 4, -88, 126, 63, 63, 63, 3, 126, 105, 63, 42, 63, 2, 84, 84, 63, 21, 42, 2, 84, 84, 63, 21, 21, 1, 84, 84, 42, 21, 21, 1, -25, 105, 105, 105, -109, 4, -67, 63, 63, 63, 105, 3, -109, 63, 63, 42, 84, 2, 126, 21, 42, 42, 63, 2, 126, 21, 42, 42, 42, 1, 105, 21, 42, 42, 42, 1, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105, 63, 63, 63, 2, 84, 105, 42, 21, 42, 2, 84, 105, 21, 21, 42, 1, 84, 84, 21, 21, 42, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 42, 21, 84, 126, 21, 2, 42, 21, 84, 105, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 63, 21, 105, 84, 21, 2, 42, 21, 105, 84, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -67, -88, 105, 105, 126, 4, -109, 126, 63, 63, 84, 3, 105, 105, 63, 42, 84, 2, 84, 105, 42, 21, 42, 2, 84, 84, 42, 21, 42, 1, 84, 84, 21, 21, 42, 1, -109, -109, -67, 105, 105, 4, 105, 105, -109, 63, 63, 3, 84, 84, 105, 42, 63, 2, 63, 84, 84, 21, 21, 2, 63, 63, 84, 21, 21, 1, 63, 63, 84, 21, 21, 1, 0, 0, 0, 0, 0, 1, -67, -109, -109, 105, 105, 4, -109, 105, 105, 63, 63, 3, 105, 84, 84, 42, 63, 2, 84, 63, 63, 21, 21, 2, 84, 63, 63, 21, 21, 1, 84, 63, 63, 21, 21, 1, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105 };  /* 0x61a686, the query: prototype kind (-1 = any); the
                                   table at +2 is ai_Unk61a688 (the same storage, one block as in the original) */
/* ---- layout 2026-10-23: the value / price helpers 0x5085b0 .. 0x509ee0 (ROUGH drafts) ---- */
char ai_Unk61a8ac[] = "?*T\002iT*\025*\002TT*\025*\001TT\025\025*\001";  /* 0x61a8ac, a table of ints indexed by the word + */
char ai_Unk61a9ec[] = "f3_gm:lpObjekteINST";  /* 0x61a9ec */
char ai_Unk61aa00[20] = { 102, 51, 95, 103, 109, 58, 108, 112, 79, 98, 106, 101, 107, 116, 101, 80, 82, 79, 84, 0 };  /* 0x61aa00 */
char ai_Unk61aa20[] = "f3_gm:lpGebaeudeINST";  /* 0x61aa20 */
char ai_Unk61aa38[24] = { 102, 51, 95, 103, 109, 58, 108, 112, 71, 101, 98, 97, 101, 117, 100, 101, 80, 82, 79, 84, 0, 0, 0, 0 };  /* 0x61aa38 */
int ai_NextCharNumber = 0;  /* 0x6f1bb8 */
int ai_NextPlayerNumber = 0;  /* 0x6f1bbc */
int ai_CharCount = 0;  /* 0x6f1bc0 */
char * ai_Unk6f1bd0 = 0;  /* 0x6f1bd0 */
char ai_Unk6f1bd4 = 0;  /* 0x6f1bd4 */
char ai_Unk6f1bd5 = 0;  /* 0x6f1bd5, stop at the first, first call */
int ai_Unk6f1bd8 = 0;  /* 0x6f1bd8, the number of records tested */
int f3gm_ObjectCount = 0;  /* 0x6f1bdc, the number of object instances in use */
char ai_Unk6f1be0 = 0;  /* 0x6f1be0 */
int ai_Unk6f1be4 = 0;  /* 0x6f1be4, the number of records scanned */
char ai_Unk6f1be8 = 0;  /* 0x6f1be8 */
char ai_Unk6f1be9 = 0;  /* 0x6f1be9 */
char ai_Unk6f1bea = 0;  /* 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
/* 0x500190 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);                     /* 0x4c4880 */
void ai_Unk4c56d0(void *p, unsigned char k);                     /* 0x4c56d0 */
void cm_BeginSendGroup(void);                     /* 0x4c3b30 */
void cm_EndSendGroup(void);                     /* 0x4c3b40 */
int cm_SendGroupState();                     /* 0x4c3b50 */
int gm_FindPlayerBuilding();                     /* 0x501000 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */

/* the goods record n (0x80 bytes) of the player p */
#define GM_GOODS(p, n) (init_StockBlocks + (p) * 0x2a90 + (n) * 0x80)

/* 0x4ff190, refreshes the goods lists of the player 0x6f1bc5 (only for the own player): clears the flag (+4) of the 0x55 goods records, marks the goods the buildings of the town (group (1, 6), not kind 10) hold in their stock (the object 0xff of each: 0x4ffe90 query) and the goods their prototypes need (the four words at +0x2e of the prototype), puts the goods that are marked on the market (+0x3c) but not yet in its stock there (0x500190), and counts the market objects 0x92 .. 0x97 / 0x2db: the byte +0x1c of the one of kind 0x11 gets the count. ROUGH */
void gm_RefreshGoodsLists(unsigned p)
{
    int list[8];
    char *g, *g2;
    unsigned char *b;
    int o, o2, i, j, k, cnt;
    int *lp;
    unsigned short id;

    list[0] = 0x92;
    list[1] = 0x93;
    list[2] = 0x94;
    list[3] = 0x95;
    list[4] = 0x96;
    list[5] = 0x97;
    list[6] = 0x2db;
    list[7] = 0;
    ai_MarketBuilding = (struct AIBLD *)gm_BuildQuery(1, 5, 10);
    if (ai_MarketBuilding != 0 && p == ai_OwnPlayer) {
        for (i = 0; i < 0x55; i++)
            *(int *)(GM_GOODS(p, i) + 4) = 0;
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (ai_BTypes[*b * 0x24d] != 10) {
                o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xff);
                if (o != 0) {
                    for (i = 0; i < 0x55; i++) {
                        g = GM_GOODS(p, i);
                        id = *(unsigned short *)g;
                        if (id != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, id) != 0) {
                            *(unsigned short *)(g + 4) = 1;
                            *(unsigned short *)(g + 6) = 0;
                            for (j = 0; j < 0x55; j++) {
                                g2 = GM_GOODS(p, j);
                                for (k = 0; k < 4; k++) {
                                    if (*(unsigned short *)g2 == *(unsigned short *)(ai_Goods + id * 0x41 + 0x2e + k * 2)) {
                                        *(unsigned short *)(g2 + 4) = 1;
                                        *(unsigned short *)(g2 + 6) = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        for (i = 0; i < 0x55; i++) {
            g = GM_GOODS(p, i);
            o = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, *(unsigned short *)(g + 0x3c));
            if (o != 0) {
                o = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff);
                if (*(int *)(g + 4) != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, *(unsigned short *)g) == 0)
                    gm_CreateObject(*(int *)(o + 2), *(unsigned short *)g, 1);
            }
        }
        lp = list;
        k = 0x92;
        do {
            cnt = 0;
            o = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, k);
            if (o != 0 && (o2 = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff)) != 0) {
                o2 = (int)gm_Query(*(int *)(o2 + 0x14), 1, 5);
                while (o2 != 0) {
                    cnt++;
                    o2 = (int)gm_QueryNext();
                }
                o = (int)gm_Query(*(int *)(o + 0x14), 1, 4, 0x11);
                if (o != 0 && (unsigned char)*(char *)(o + 0x1c) != cnt)
                    *(char *)(o + 0x1c) = (char)cnt;
            }
            k = lp[1];
            lp++;
        } while (k != 0);
    }
}

void gm_SyncMarket(unsigned p);
int gm_Unk4ff8c0();
int gm_Unk4ff950();

/* 0x4ff550, the daily price index update of the player p at the clock t: the calendar date of 0x4fe2c0 (year, month number) is looked up in the two piecewise linear tables of the player (9 points of (value, month) at +0x60 / +0x50 of the slot 0x7e3f40 + p * 0x2f4 each), interpolated into the indices [0] / [2] of the player's record (0x7d6a60, times the factor [1] / [3] / 1000); then the 0x55 goods records: the float +0x20 moves halfway to 0x4ff8c0, the own player gets 0x4ff950 into +0x34, others copy +0x30; +0x38 = the price 0x4ffa10; the counters +0x10 .. +0x1c are cleared; at the end of the calendar the market (0x4fec10, 0x4ff450) runs. ROUGH */
void gm_UpdatePrices(unsigned p, void *t)
{
    unsigned char d[10];   /* gm_ClockToDate writes 10 bytes: out[0..5] and the int at +6 */
    int month, n, k, a, x0, x1;
    int *pl = (int *)ai_PlayerSlots[p];
    int *rec = gm_StockBlocks + p * 0xaa4;
    int i;
    char *g;
    float f;

    gm_ClockToDate(t, d);
    month = *(short *)(d + 2);
    for (n = 0; n < 9; n++)
        if (pl[0x64 / 4 + 2 * n + 2] <= pl[0x64 / 4 + 2 * n])
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (month <= pl[0x64 / 4 + 2 * k])
            break;
    if (k < n - 1) {
        x0 = pl[0x64 / 4 + 2 * k];
        if (month == x0)
            rec[0] = (((int)(char)d[1] + (month - x0) * 0xc) * (pl[0x70 / 4 + 2 * k] - pl[0x68 / 4 + 2 * k])) / ((pl[0x6c / 4 + 2 * k] - x0) * 0xc) + pl[0x68 / 4 + 2 * k];
        else if (k < 1)
            rec[0] = pl[0x68 / 4];
        else {
            x1 = pl[0x5c / 4 + 2 * k];
            rec[0] = (((int)(char)d[1] + (month - x1) * 0xc) * (pl[0x68 / 4 + 2 * k] - pl[0x60 / 4 + 2 * k])) / ((x0 - x1) * 0xc) + pl[0x60 / 4 + 2 * k];
        }
    } else
        rec[0] = pl[0x60 / 4 + 2 * n];
    rec[0] = (rec[1] * rec[0]) / 1000;
    for (n = 0; n < 9; n++)
        if (pl[0xb4 / 4 + 2 * n + 2] <= pl[0xb4 / 4 + 2 * n])
            break;
    if (n >= 9)
        n = 9;
    for (k = 0; k < n; k++)
        if (month <= pl[0xb4 / 4 + 2 * k])
            break;
    if (k < n - 1) {
        x0 = pl[0xb4 / 4 + 2 * k];
        if (month == x0)
            a = (((int)(char)d[1] + (month - x0) * 0xc) * (pl[0xc0 / 4 + 2 * k] - pl[0xb8 / 4 + 2 * k])) / ((pl[0xbc / 4 + 2 * k] - x0) * 0xc) + pl[0xb8 / 4 + 2 * k];
        else if (k < 1)
            a = pl[0xb8 / 4];
        else {
            x1 = pl[0xac / 4 + 2 * k];
            a = (((int)(char)d[1] + (month - x1) * 0xc) * (pl[0xb8 / 4 + 2 * k] - pl[0xb0 / 4 + 2 * k])) / ((x0 - x1) * 0xc) + pl[0xb0 / 4 + 2 * k];
        }
        rec[2] = a;
    } else
        rec[2] = pl[0xb0 / 4 + 2 * n];
    rec[2] = (rec[3] * rec[2]) / 1000;
    for (i = 0; i < 0x55; i++) {
        g = GM_GOODS(p, i);
        if (ai_Goods[*(unsigned short *)g * 0x41] != 0x17 && ai_Goods[*(unsigned short *)g * 0x41] != 0x25) {
            a = gm_Unk4ff8c0(p, i);
            *(float *)(g + 0x20) = ((float)a - *(float *)(g + 0x20)) * 0.5f + *(float *)(g + 0x20);
        }
        if (p == ai_OwnPlayer)
            *(int *)(g + 0x34) = gm_Unk4ff950(p, i);
        else
            *(int *)(g + 0x34) = *(int *)(g + 0x30);
        f = gm_Unk4ffa10(p, i);
        *(float *)(g + 0x38) = f;
        if (g[0x40] != 0)
            *(float *)(g + 0x20) = (float)*(int *)(g + 0x34);
        *(int *)(g + 0x10) = 0;
        *(int *)(g + 0x14) = 0;
        *(int *)(g + 0x1c) = 0;
        *(int *)(g + 0x18) = 0;
    }
    if (ai_Unk1460ae0 == -1) {
        gm_SyncMarket(p);
        gm_RandomMarketShake();
    }
}

#define GM_WAIT(h) while (cm_GetSlotState(h) == 0) sv_PumpNet()

/* 0x4fec10, the market sync of the player p: for the own player (0x6f1bc5) the goods lists are rebuilt as in 0x4ff190 (the flag +4 marks goods in stock or needed), then every goods record is offered to / requested from the market building (0x4c4880 messages: a marked good that is not in stock is requested (1), an unmarked one in stock is sold with its stock (+0xe)) and sent to the player (0x4c56d0); the market object counts (0x92 .. 0x97, 0x2db) are written back to the byte +0x1c of the object of kind 0x11. For another player: the goods of its two market objects (0x1db / 0x1dc) are randomly marked (a byte at +0x28 of the record grows by random 0x18 + 0 .. 7 per call; below 0x81 means offered) and sent (0x4c3b30 .. 0x4c3b50). ROUGH */
void gm_SyncMarket(unsigned p)
{
    int list[8];
    unsigned rec[0x20];
    char *g, *g2;
    unsigned char *b;
    int o, o2, i, j, k, cnt, m, h, a1;
    int *lp;
    unsigned short id;
    unsigned char c28;

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
    if (p == ai_OwnPlayer) {
        for (i = 0; i < 0x55; i++)
            *(int *)(GM_GOODS(p, i) + 4) = 0;
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (ai_BTypes[*b * 0x24d] != 10) {
                o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xff);
                if (o != 0) {
                    for (i = 0; i < 0x55; i++) {
                        g = GM_GOODS(p, i);
                        id = *(unsigned short *)g;
                        if (id != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, id) != 0) {
                            *(int *)(g + 4) = 1;
                            for (j = 0; j < 0x55; j++) {
                                g2 = GM_GOODS(p, j);
                                for (k = 0; k < 4; k++)
                                    if (*(unsigned short *)g2 == *(unsigned short *)(ai_Goods + id * 0x41 + 0x2e + k * 2))
                                        *(int *)(g2 + 4) = 1;
                            }
                        }
                    }
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        for (i = 0; i < 0x55; i++) {
            g = GM_GOODS(p, i);
            memcpy(rec, GM_GOODS(p, i), 0x80);
            o = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, *(unsigned short *)(g + 0x3c));
            if (o != 0) {
                o2 = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff);
                if (o2 == 0) {
                    h = ((int (*)(void *, int, int, int, int, int))cm_SendExSellObject)(*(int *)(o + 2), -1, 0xff, 1, ai_OwnPlayer, 0);
                    GM_WAIT(h);
                    o2 = cm_GetSlotValue(h);
                }
                if (*(int *)(g + 4) == 0) {
                    m = (int)gm_Query(*(int *)(o2 + 0x14), 1, 0, *(unsigned short *)g);
                    if (m != 0) {
                        h = ((int (*)(int, void *, void *, void *, int, int))cm_SendExSellObject)(-1, *(int *)(o2 + 2), *(unsigned short *)g, *(int *)(m + 0xe), ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                } else {
                    m = (int)gm_Query(*(int *)(o2 + 0x14), 1, 0, *(unsigned short *)g);
                    if (m == 0) {
                        h = ((int (*)(void *, int, void *, int, int, int))cm_SendExSellObject)(*(int *)(o2 + 2), -1, *(unsigned short *)g, 1, ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                }
                ai_Unk4c56d0(rec, p);
            }
        }
        lp = list;
        k = 0x92;
        do {
            cnt = 0;
            o = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, k);
            if (o != 0 && (o2 = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff)) != 0) {
                o2 = (int)gm_Query(*(int *)(o2 + 0x14), 1, 5);
                while (o2 != 0) {
                    cnt++;
                    o2 = (int)gm_QueryNext();
                }
                o = (int)gm_Query(*(int *)(o + 0x14), 1, 4, 0x11);
                if (o != 0 && (int)(unsigned char)*(char *)(o + 0x1c) < cnt) {
                    cnt -= (unsigned char)*(char *)(o + 0x1c);
                    cm_BeginFieldBlock(o, *(int *)(o + 2));
                    cm_AddFieldValues(1, 1, ((char *)o - cm_FieldObject) + 0x1c, &cnt);
                    cm_SendFieldDeltas();
                }
            }
            k = lp[1];
            lp++;
        } while (k != 0);
        return;
    }
    o = gm_FindPlayerBuilding(p);
    if (o != 0 && (a1 = (int)gm_Query(*(int *)(o + 0x5d), 2, 0, 0x1db, 6)) != 0 && (o2 = (int)gm_Query(*(int *)(o + 0x5d), 2, 0, 0x1dc, 6)) != 0) {
        cm_BeginSendGroup();
        for (i = 0; i < 0x55; i++) {
            memcpy(rec, GM_GOODS(p, i), 0x80);
            if ((short)rec[0] != 0) {
                c28 = *((unsigned char *)rec + 0x28);
                if (gm_Query(*(int *)(a1 + 0x14), 1, 0, rec[0] & 0xffff) == 0 && gm_Query(*(int *)(o2 + 0x14), 1, 0, rec[0] & 0xffff) == 0) {
                    rec[1] = 0;
                } else if (c28 == 0) {
                    c28 = (unsigned char)util_RandomU(0xff);
                    rec[1] = c28 < 0x81;
                } else {
                    c28 = c28 + (char)util_RandomU(8) + 0x18;
                    rec[1] = c28 < 0x81;
                }
                *((unsigned char *)rec + 0x28) = c28;
                ai_Unk4c56d0(rec, p);
            }
        }
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0)
            sv_PumpNet();
    }
}
/* 0x4ff8c0, the amount of the goods record i (+8 a float rate) for the player p: 0 for the goods kinds 0x17 / 0x25; for player 0 the float 0x6f1b9c * the rate * 0.0025 + 0.5, for the others the player's first int (price index, 0x7d6a60) * the rate * 0.0005 */

int gm_Unk4ff8c0(int p, int i)
{
    char *g;
    char k;

    g = GM_GOODS(p, i);
    k = ai_Goods[*(unsigned short *)g * 0x41];
    if (k != 0x17 && k != 0x25) {
        if (p == 0)
            return (int)(*(float *)&ai_RatingBase * *(float *)(g + 8) * 0.0025 + 0.5);
        return (int)((float)gm_StockBlocks[p * 0xaa4] * *(float *)(g + 8) * 0.0005);
    }
    return 0;
}
/* 0x4ff950, the stock of the goods record i of the player p: the sum over the four buildings (the group (1, 5, number) of the bytes at +0x40 + 8k, each with the float at +4 of its slot added once per match) times 0.5, scaled by the float +0x24 / +0x20 when +0x24 is not 0; 0 when the first building byte is 0. ROUGH */

int gm_Unk4ff950(int p, int i)
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

extern short ai_Unk61a674, ai_Unk61a676;/* 0x61a674 / 0x61a676, the query: the words +0x25 / +0x27 (0x7fff = any) */
extern char ai_Unk6f1bd4, ai_Unk6f1bd5; /* 0x6f1bd4, 0x6f1bd5, stop at the first, first call */
char * ai_Unk6f1bf0 = 0;  /* 0x6f1bf0, the current building */
extern unsigned ai_Unk889400[732];
extern unsigned short ai_Unk76fdc4;
void rt_FreePoints();                     /* 0x4bd9e0 */
/* 0x500190 */
int ai_Unk5a86c0();                     /* 0x5a86c0 */

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
    int b, o, c, r;
    int *list;
    char buf[512];

    if (proto != 0) {
        if (gm_FindById(&b, &o, &c, id) != 0) {
            if (b == 0) {
                if (o == 0) {
                    if (c == 0)
                        return 0;
                    list = (int *)(c + 0x178);
                } else
                    list = (int *)(o + 0x14);
            } else
                list = (int *)(b + 0x5d);
            r = (int)gm_Query(*list, 1, 0, proto);
            if (r != 0) {
                *(int *)(r + 0xe) += n;
                return r;
            }
            return gm_CreateObject(id, proto, n);
        }
        sprintf(buf, "gm_AddObjekt(): Parent not found. ObjProt:%i Menge:%i", proto, n);
    }
    return 0;
}
/* 0x500a40, takes the amount n of the prototype proto from the owner id: 1 when it had enough (the instance is removed when it drops to 0 or below), else 0 */

int gm_TakeObjekt(int id, int proto, int n)
{
    int b, o, c, r, left;
    int *list;

    if (gm_FindById(&b, &o, &c, id) != 0) {
        if (b == 0) {
            if (o == 0) {
                if (c == 0)
                    return 0;
                list = (int *)(c + 0x178);
            } else
                list = (int *)(o + 0x14);
        } else
            list = (int *)(b + 0x5d);
        r = (int)gm_Query(*list, 1, 0, proto);
        if (r != 0 && n <= *(int *)(r + 0xe)) {
            left = *(int *)(r + 0xe) - n;
            *(int *)(r + 0xe) = left;
            if (left < 1)
                gm_RemoveObject(list, proto);
            return 1;
        }
    }
    return 0;
}
/* 0x500ae0, like 0x500a40 but creates a missing instance with 0 first and removes it when the amount is exactly 0 (a negative stock is possible) */

int gm_AddNegative(int id, int proto, int n)
{
    int b, o, c, r, left;
    int *list;

    if (gm_FindById(&b, &o, &c, id) == 0)
        return 0;
    if (b == 0) {
        if (o == 0) {
            if (c == 0)
                return 0;
            list = (int *)(c + 0x178);
        } else
            list = (int *)(o + 0x14);
    } else
        list = (int *)(b + 0x5d);
    r = (int)gm_Query(*list, 1, 0, proto);
    if (r == 0) {
        r = gm_CreateObject(id, proto, 1);
        *(int *)(r + 0xe) = 0;
    }
    left = *(int *)(r + 0xe) - n;
    *(int *)(r + 0xe) = left;
    if (left == 0)
        gm_RemoveObject(list, proto);
    return 1;
}
/* 0x500b90, looks the building kind of the building b up in the table 0x61a348 (0x1e records of 0x15 bytes: a kind byte and ten object words) and collects the non-zero words into 0x889400 (count 0x76fdc4); 1 when the kind is not there (or no tables), else 0 */

int gm_BuildKindObjects(char *b)
{
    char *t;
    int i, k;
    unsigned short *w;

    if (ai_Goods == 0)
        return 1;
    i = 0;
    t = (char *)gm_BuildingItemGroups;
    do {
        if (*t == *b) {
            if (i * 0x15 == -(unsigned)&gm_BuildingItemGroups)
                return 1;
            ai_Unk76fdc4 = 0;
            w = gm_BuildingItemGroups[i].items;
            for (k = 10; k != 0; k--, w++) {
                if (*w != 0)
                    ai_Unk889400[ai_Unk76fdc4++] = *w;
            }
            return 0;
        }
        t += 0x15;
        i++;
    } while ((int)t < ((unsigned)&gm_BuildingItemGroups + 0x309));
    return 1;
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
/* 0x501030, the next building matching the query of 0x501190 (type 0x61a66c, id 0x61a670, words +0x25 / +0x27, building type 0x61a678; the flag 0x6f1bd4 stops at the first match), walking the building table from 0x6f1bf0; 0 when done */

unsigned char *gm_BuildNext(void)
{
    int m;
    unsigned char *end, *p;

    m = 0;
    end = (unsigned char *)ai_Blds + 0xa900;
    if (ai_BTypes == 0)
        return 0;
    if (ai_Unk6f1bd8 > 0x100)
        return 0;
    if (ai_Unk6f1bd5 == 0) {
        do {
            ai_Unk6f1bf0 += 0xa9;
            if ((unsigned char *)ai_Unk6f1bf0 >= end)
                return 0;
        } while (*ai_Unk6f1bf0 == 0);
    } else {
        ai_Unk6f1bd5 = 0;
        if ((unsigned char *)ai_Unk6f1bf0 >= end) {
            ai_Unk6f1bd5 = 0;
            return 0;
        }
        do {
            if (*ai_Unk6f1bf0 != 0)
                break;
            ai_Unk6f1bf0 += 0xa9;
        } while ((unsigned char *)ai_Unk6f1bf0 < end);
    }
    p = (unsigned char *)ai_Unk6f1bf0;
    if (p >= end)
        return 0;
    do {
        if (ai_Unk6f1bd4 != 0)
            m = 1;
        if (ai_Unk61a66c == 0xff || *p == ai_Unk61a66c) {
            if (ai_Unk61a66c != 0xff)
                m = 1;
            if (ai_Unk61a670 != -1) {
                if (*(int *)(p + 1) != ai_Unk61a670)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a674 != 0x7fff) {
                if (*(short *)(p + 0x25) != ai_Unk61a674)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a676 != 0x7fff) {
                if (*(short *)(p + 0x27) != ai_Unk61a676)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a678 != -1)
                m = ai_BTypes[*p * 0x24d] == ai_Unk61a678;
        } else {
        no:
            m = 0;
        }
        ai_Unk6f1bd8++;
        if (!m) {
            do {
                p += 0xa9;
                if (p >= end)
                    goto done;
                ai_Unk6f1bf0 = (char *)p;
            } while (*p == 0);
        }
        if (p >= end) {
            if (!m) {
            done:
                p = 0;
                ai_Unk6f1bf0 = 0;
            }
            return p;
        }
        if (m)
            return p;
    } while (1);
}
/* 0x501190, starts a building query and returns the first match (0x501030): n criteria follow as (kind, value) pairs: 0 = type class (byte, 0x61a66c), 1 = id, 2 = a string (copied to *0x6f1bd0, unused), 3 / 4 = the words +0x25 / +0x27, 5 = the building type, 6 = stop at the first (no value). Used all over the exe as gm_BuildQuery (1, 5, 10) .. */

unsigned char *gm_BuildQuery(int n, ...)
{
    int *ap;
    int i;
    char *s, *d;
    char c;

    if (ai_BTypes != 0) {
        ai_Unk61a66c = 0xff;
        ai_Unk61a678 = (char)0xff;
        ai_Unk61a670 = -1;
        ai_Unk6f1bd0 = 0;
        ai_Unk61a674 = 0x7fff;
        ai_Unk61a676 = 0x7fff;
        ai_Unk6f1bd5 = 1;
        ai_Unk6f1bd8 = 0;
        ai_Unk6f1bd4 = 0;
        if (n > 0) {
            ap = &n;
            for (i = n; i != 0; i--) {
                switch ((char)ap[1]) {
                case 0:
                    ai_Unk61a66c = (unsigned char)ap[2];
                    ap += 2;
                    break;
                case 1:
                    ai_Unk61a670 = ap[2];
                    ap += 2;
                    break;
                case 2:
                    s = (char *)ap[2];
                    ap += 2;
                    d = ai_Unk6f1bd0;
                    do {
                        c = *s++;
                        *d++ = c;
                    } while (c != 0);
                    break;
                case 3:
                    ai_Unk61a674 = (short)ap[2];
                    ap += 2;
                    break;
                case 4:
                    ai_Unk61a676 = (short)ap[2];
                    ap += 2;
                    break;
                case 5:
                    ai_Unk61a678 = (char)ap[2];
                    ap += 2;
                    break;
                case 6:
                    ai_Unk6f1bd4 = 1;
                    ap += 1;
                    break;
                default:
                    ap += 1;
                }
            }
        }
        ai_Unk6f1bf0 = ai_Blds;
        return gm_BuildNext();
    }
    return 0;
}
/* 0x5012b0, the first free record of the building table (0x100 records of 0xa9 bytes), 0 when it is full */

char *gm_FindFreeBuilding(void)
{
    char *end, *p;

    end = ai_Blds + 0xa900;
    p = ai_Blds;
    if (p < end) {
        do {
            if (*p == 0)
                return p;
            p += 0xa9;
        } while (p < end);
    }
    return 0;
}
/* 0x5012e0, frees the building b: a player's building (+0x27 a character of kind 6) tells 0x5a86c0 (its id); by building type: 0x1e frees the block at +0x71, 0x48 .. 0x4a the block at +0x65 through 0x4f0180, 0x4b .. 0x4d through the heap free (0x422620) and clears +0x65; the characters (0x770cec / 0x770cf0 per record) that point at it are cleared; its list at +0x5d is freed (0x500140) and the building type byte set to 0. Returns 0, -1 / -2 for no building / a free one. ROUGH */

int gm_FreeBuilding(char *b)
{
    int *q;
    char k;

    if (b == 0)
        return -1;
    if (*b == 0)
        return -2;
    if (*(unsigned short *)(b + 0x27) != 0xffff && ai_Chars[*(unsigned short *)(b + 0x27)][2] == 6)
        ai_Unk5a86c0(*(int *)(b + 1));
    k = *b;
    if (k == 0x1e) {
        if (*(int *)(b + 0x71) != 0) {
            mem_Free(*(int *)(b + 0x71));
            *(int *)(b + 0x71) = 0;
        }
        *(int *)(b + 0x71) = 0;
    } else if (k == 'H' || k == 'I' || k == 'J') {
        gv_FreeGraveyard(*(int *)(b + 0x65));
        *(int *)(b + 0x65) = 0;
    } else if (k == 'K' || k == 'L' || k == 'M') {
        mem_Free(*(int *)(b + 0x65));
        *(int *)(b + 0x65) = 0;
    }
    q = (int *)(ai_Chars[0] + 0x16c);
    do {
        if ((char *)q[1] == b)
            q[1] = 0;
        if ((char *)q[0] == b)
            q[0] = 0;
        q += 0x87;
    } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x16a));
    if (*(int *)(b + 0x5d) != 0)
        gm_FreeList((int *)(b + 0x5d));
    *b = 0;
    return 0;
}

extern int gm_NOffers;
int gm_ListStock();                     /* 0x50ab50 */
int gm_ListAmount();                     /* 0x50aaf0 */
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

/* 0x500190, creates an object instance of the prototype proto with the amount n in the list of the owner id (a building, an object or a character, found by 0x4fea50): appends it to the owner's list (the owner list head: +0x5d building, +0x14 object, +0x178 character), takes a free record (0x4fffc0), fills it (+0 prototype, +2 a new id, +6 the owner id, +0xa the owner's number, +0xe the amount, +0x12 0x64 = 100, +0x3f next 0) and counts it (0x6f1bdc); a few prototypes get their own start values (0xb8, 0x12c, 0x12d, 0x15e / 0x160, 0x174 / 0x176, 0x24b / 0x256 / 0x266) and the three transport carts 0x134 / 0x135 / 0x136 get a model (cart or horse, 0x403390 at the dummy "dummy_TRANSPORTER") with a load object (ob_KARREN_*). Returns the instance, 0 on failure. ROUGH */
unsigned short *gm_CreateObject(int id, unsigned proto, int n)
{
    int b, o, c;
    int it, it2;
    int *list;
    int last;
    unsigned short *r;
    unsigned short *q;
    char *pr;
    char buf[1024];
    int pos[3], zero3[3];
    int saved, sc, m, ld;
    char *name;
    short s;
    int *hd;

    b = 0;
    o = 0;
    c = 0;
    last = 0;
    r = 0;
    pr = ai_Goods;
    if (ai_Goods == 0)
        return 0;
    if (id == -1)
        return 0;
    if (gm_FindById(&b, &o, &c, id) == 0)
        return 0;
    if (b == 0) {
        if (o == 0) {
            if (c == 0)
                return 0;
            list = (int *)(c + 0x178);
        } else
            list = (int *)(o + 0x14);
    } else
        list = (int *)(b + 0x5d);
    if (list != 0) {
        it = *list;
        if (*list == 0) {
            r = (unsigned short *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            *list = (int)r;
            pr = ai_Goods;
        } else {
            do {
                last = it;
                it = *(int *)(last + 0x3f);
            } while (*(int *)(last + 0x3f) != 0);
            r = (unsigned short *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            *(unsigned short **)(last + 0x3f) = r;
            pr = ai_Goods;
        }
    }
    pr = pr + proto * 0x41;
    *r = (unsigned short)proto;
    sprintf(buf, "----> ObjID: %i ObjProt: %i", ai_NextObjectId, proto & 0xffff);
    *(int *)(r + 1) = ai_NextObjectId;
    ai_NextObjectId++;
    if (b == 0) {
        if (o == 0) {
            if (c != 0)
                *(int *)(r + 5) = *(int *)(c + 4);
        } else
            *(int *)(r + 5) = *(int *)(o + 10);
    } else
        *(int *)(r + 5) = *(int *)(b + 1);
    *(int *)(r + 3) = id;
    *(int *)(r + 7) = n;
    *(char *)(r + 9) = 'd';
    *((char *)r + 0x13) = 0;
    r[10] = 0;
    r[0xb] = 0;
    *(int *)((char *)r + 0x3f) = 0;
    f3gm_ObjectCount++;
    memset(r + 0xe, 0, 0x1d);
    if ((int)proto > 0x160) {
        switch (proto) {
        case 0x174:
        case 0x176:
            r[0x10] = 6;
            r[0x11] = 0;
            break;
        case 0x24b:
        case 0x256:
        case 0x266:
            r[0xe] = 0;
            r[0xf] = 0;
            r[0x10] = 0;
            r[0x11] = 0;
        }
    } else if (proto == 0x160 || proto == 0x15e) {
        r[0x10] = 0xf;
        r[0x11] = 0;
    } else {
        switch (proto) {
        case 0xb8:
            *(char *)(r + 0xe) = 6;
            break;
        case 300:
            r[0xe] = 0xffff;
            r[0xf] = 0xffff;
            r[0x10] = 0xffff;
            r[0x11] = 0xffff;
            r[0x12] = 0xffff;
            r[0x13] = 0xffff;
            *(int *)(r + 0x14) = *(int *)ai_Time - 1;
            break;
        case 0x12d:
            r[0xe] = 0xffff;
            r[0xf] = 0xffff;
            r[0x10] = 0xffff;
            r[0x11] = 0xffff;
            r[0x12] = 0xffff;
            r[0x13] = 0xffff;
            r[0x14] = 0xffff;
            r[0x15] = 0xffff;
            break;
        case 0x134:
        case 0x135:
        case 0x136:
            if (b == 0) {
                if (o == 0) {
                    if (c != 0)
                        *(int *)(r + 5) = *(int *)(c + 4);
                } else {
                    it = gm_ObjectBuildingOwner(o);
                    *(int *)(r + 5) = *(int *)(it + 4);
                }
            } else
                *(int *)(r + 5) = *(int *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 4);
            m = gm_FindById(&b, &o, &c, id);
            it = id;
            while (m != 0 && b == 0) {
                if (o != 0)
                    it = *(int *)(o + 6);
                m = gm_FindById(&b, &o, &c, it);
            }
            *(char *)(r + 0x14) = 0;
            *((char *)r + 0x31) = 2;
            if (b == 0) {
                r[0xe] = 0xffff;
                r[0xf] = 0xffff;
            } else
                *(int *)(r + 0xe) = *(int *)(b + 1);
            it = (int)gm_CreateObject(*(int *)(r + 1), 0x1dd, 1);
            *(unsigned *)(it + 0xe) = *r - 0x133;
            *(char *)(it + 0x1c) = (char)*r - 0x33;
            it = (int)gm_BuildQuery(1, 1, id);
            ld = it;
            sc = 0;
            if (it != 0 && gm_IsHomeType(it) != 0)
                sc = init_BuildingSceneUniverse(it);
            saved = d3_CurrentScene;
            d3if_SelectScene(sc, 1);
            if (gm_IsHomeType(it) == 0)
                sc = *(int *)(it + 0x61);
            else
                sc = 0;
            m = d3_FindObject(sc, 0x100, 0, "dummy_TRANSPORTER");
            if (m == 0) {
                if (*(int *)(it + 0x61) == 0) {
                    pos[0] = pos[1] = pos[2] = 0;
                } else {
                    pos[0] = *(int *)(*(int *)(it + 0x61) + 0x4c);
                    pos[1] = *(int *)(*(int *)(it + 0x61) + 0x50);
                    pos[2] = *(int *)(*(int *)(it + 0x61) + 0x54);
                }
            } else
                d3_PointToWorld(m, m + 0x4c, pos);
            if (proto == 0x136)
                name = "pferd_PFERD";
            else {
                s = util_RandomU(3);
                if (s == 0)
                    name = "arbeiter_KARREN";
                else if (s == 1)
                    name = "holzfaeller_KARREN";
                else
                    name = "handwerker_KARREN";
            }
            it2 = ch_Create(name, pos);
            *(int *)((char *)r + 0x3b) = it2;
            ch_PreloadAnims(it2, 1, "bewegung/karren_ziehen");
            init_HookObject(*(int *)(it2 + 0x34), r, 1);
            sprintf((char *)*(int *)(it2 + 0x34), "sp_%i", *(int *)(r + 1));
            if (proto == 0x134)
                name = "ob_KARREN_klein";
            else if (proto == 0x135)
                name = "ob_KARREN_mittel";
            else
                name = "ob_KARREN_GROSS";
            m = d3_CreateObject(0, pos, zero3, name);
            if (m != 0) {
                *(unsigned char *)(m + 0x212) = *(unsigned char *)(m + 0x212) & 0xf7 | 4;
                *(unsigned char *)(m + 0x211) = *(unsigned char *)(m + 0x211) & 0xfd;
                ch_AttachTransport(it2, m);
                *(int *)(m + 0x224) = 1;
                init_HookObject(m, r, 1);
            }
            if (ld == 0) {
                *(int *)(it2 + 0x30) = -1;
                sc = -1;
            } else if (gm_IsHomeType(ld) == 0) {
                ch_SetVisible(it2, 0);
                *(int *)(it2 + 0x30) = -1;
                sc = -1;
            } else {
                sc = *(int *)(ld + 1);
                *(int *)(it2 + 0x30) = -1;
            }
            *(int *)(it2 + 0x2c) = sc;
            r[0x12] = 0;
            r[0x13] = 0;
            d3if_SelectScene(saved, 1);
            break;
        }
    }
    if (*pr == 0x17 || *pr == ' ' || *pr == '%') {
        r[0xe] = 0;
        r[0xf] = 0;
    }
    return r;
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
/* 0x500c20, collects the offers for the goods id from all buildings (the first 0x58 of the table; each entry of 6 ints at 0x7e4b20: building, market object, goods object, the supply (0x50aaf0 minus a quarter of 0x50ab50 * the object's int +0x1c, not below 0), the price float, flag) and sorts them by price, highest first (the price = 0x5089c0 (goods, 0x6f1bc5) * the float at +0x49 of the building). Count in 0x7e3f3c. Returns 0, 1 without the tables. ROUGH */

int gm_CollectOffers(int goods)
{
    int n, k, i, j, cnt;
    char *b;
    int *e;
    int mk, s, o, a, q, cc;
    unsigned v;
    int f;
    unsigned id;
    int tmp[6];

    if (ai_BTypes == 0)
        return 1;
    n = 0;
    gm_NOffers = 0;
    b = ai_Blds;
    for (k = 0x58; k != 0; k--, b += 0xa9) {
        if (*b != 0) {
            f = 0;
            e = ((int *)gm_Offers) + n * 6;
            e[0] = 0;
            e[2] = 0;
            e[5] = 0;
            if (b == (char *)ai_MarketBuilding) {
                mk = gm_FindMarketObject(goods);
                n = gm_NOffers;
                if (mk != 0 && (o = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, goods)) != 0) {
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[1] = mk;
                    e[2] = o;
                    id = *(unsigned short *)o;
                    goto add;
                }
            } else {
                s = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xff);
                n = gm_NOffers;
                if (s != 0 && (o = (int)gm_Query(*(int *)(s + 0x14), 1, 0, goods)) != 0) {
                    f = 1;
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[5] = 1;
                }
                mk = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x2a);
                n = gm_NOffers;
                if (mk != 0 && (o = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, goods)) != 0) {
                    e = ((int *)gm_Offers) + gm_NOffers * 6;
                    e[0] = (int)b;
                    e[1] = mk;
                    e[2] = o;
                    id = *(unsigned short *)o;
                add:
                    a = gm_ListStock(mk, id);
                    q = *(int *)(o + 0x1c);
                    cc = gm_ListAmount(mk, o);
                    v = cc - ((unsigned)(a * q) >> 2);
                    ((int *)gm_Offers)[gm_NOffers * 6 + 3] = v;
                    ((int *)gm_Offers)[gm_NOffers * 6 + 3] = v & ((int)v < 0) - 1;
                    n = gm_NOffers + 1;
                    gm_NOffers++;
                } else if (f) {
                    gm_NOffers++;
                    n = gm_NOffers;
                }
            }
        }
    }
    cnt = n;
    for (i = 0; i < cnt; i++)
        *(float *)(((int *)gm_Offers) + i * 6 + 4) = (float)(gm_GoodsPrice(goods, ai_OwnPlayer) * *(float *)(((int *)gm_Offers)[i * 6] + 0x49));
    cnt = gm_NOffers;
    for (i = 1; i <= cnt; i++) {
        for (j = i; j < cnt; j++) {
            if (*(float *)(((int *)gm_Offers) + (i - 1) * 6 + 4) < *(float *)(((int *)gm_Offers) + j * 6 + 4)) {
                memcpy(tmp, ((int *)gm_Offers) + (i - 1) * 6, 24);
                memcpy(((int *)gm_Offers) + (i - 1) * 6, ((int *)gm_Offers) + j * 6, 24);
                memcpy(((int *)gm_Offers) + j * 6, tmp, 24);
            }
        }
    }
    return 0;
}

int gm_IsHomeType(unsigned char *b);
int gm_IsType10(unsigned char *b);
int gm_BuildingClass(int kind);

/* 0x501400, stocks a new building b with the objects its building kind needs (the table 0x61a348, one record of 0x15 bytes per kind: the kind byte and ten object numbers): the container object 0xff (query (2, 6, 0, 0xff), created by 0x500190 when missing) gets each listed object with the amount 2 when it has none */
void gm_StockBuildingObjects(char *b)
{
    short *w;
    int o, o2, k;
    char *t;

    w = (short *)gm_BuildingItemGroups[0].items;
    do {
        if (*b == *((char *)w - 1)) {
            o = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xff);
            if (o != 0 || (o = gm_CreateObject(*(int *)(b + 1), 0xff, 1)) != 0) {
                short *p = w;
                for (k = 10; k != 0; k--, p++) {
                    if (*p != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, *p) == 0)
                        gm_CreateObject(*(int *)(o + 2), *p, 2);
                }
            }
        }
        w = (short *)((char *)w + 0x15);
    } while ((int)w < ((unsigned)&gm_BuildingItemGroups + 0x30a));
}
/* 0x5014b0, sets the capacity bytes of the building b objects from its kind record (+0x23d / +0x23e / +0x23f): the object 0x2a gets +0x1c = the first (at least 2) and +0x1d = the second (at least 2), or just the third when the first two are 0; the object 0x116 gets the third in +0x1c in that case */

void gm_SetCapacities(unsigned char *b)
{
    char *k;
    int o;
    unsigned char c;

    k = ai_BTypes + *b * 0x24d;
    o = (int)gm_Query(*(int *)(b + 0x5d), 2, 0, 0x2a, 6);
    if (o != 0) {
        c = k[0x23d];
        if (c == 0 && k[0x23e] == 0 && k[0x23f] != 0) {
            *(char *)(o + 0x1c) = k[0x23f];
        } else {
            if (c < 2)
                c = 2;
            *(unsigned char *)(o + 0x1c) = c;
            c = k[0x23e];
            if (c < 2)
                c = 2;
            *(unsigned char *)(o + 0x1d) = c;
        }
    }
    o = (int)gm_Query(*(int *)(b + 0x5d), 2, 0, 0x116, 6);
    if (o != 0 && k[0x23d] == 0 && k[0x23e] == 0 && k[0x23f] != 0)
        *(char *)(o + 0x1c) = k[0x23f];
}

int gv_BuryNewDead();                     /* 0x4ef9c0 */
void gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */
int gm_BuildingClass();                     /* 0x501d80 */
/* ai_BuildingNames: view of txt_Ptrs (build/mk_txt_views.py) */
extern char ai_Time[];             /* 0x770ae0 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
unsigned short * unk_Unk5031b0(unsigned char *b, unsigned n);

/* 0x501580, creates a building of the type k for the owner o (the next free instance, 0x5012b0): the id (0x61a654 counter), the owner / place words, the default values (rank 100, 0x7d00 ..), a name from the names of its type that no building has yet (up to 12 candidates of 0x20 characters: "Building name too long: %s" is the error text), the objects of its prototype (0x500190 / 0x5031b0 for the kinds 2 / 6), the main object (0x501ce0) and the per-type extras: dates (the clock at +0x69 .. +0x75) for the types 4 .. 7, an empty owner for 0xb .. 0xd, the counters of 0xe .. 0x10, the object 0x1b5 for 0x14 .. 0x16, the "f3_gm:PlantMap" of 0x1e (0x600 bytes of 0x18-byte cells), 0x4efc10 / 0x4ef9c0 for 0x48 .. 0x4a, "AHM_INFO" (0x3c bytes) for 0x4b .. 0x4d. ROUGH */
unsigned char *gm_CreateBuilding(unsigned char k, unsigned short own)
{
    unsigned char *b;
    char *base, *proto;
    char dbg[0x100];
    char names[12][0x40];
    char cand[0x100];
    char err[0x200];
    int i, n, j, found, any;
    unsigned short *w;
    char *src, *u;
    short *s;
    int *q;
    int ch;
    char c;
    char *bl;

    base = ai_BTypes;
    if (base == 0)
        return 0;
    any = 0;
    b = (unsigned char *)gm_FindFreeBuilding();
    if (b == 0)
        return 0;
    *b = k;
    proto = base + k * 0x24d;
    sprintf(dbg, "----->GebID= %i GebProt= %i", ai_NextObjectId, k);
    *(int *)(b + 1) = ai_NextObjectId;
    ai_NextObjectId++;
    *(unsigned short *)(b + 0x25) = own;
    *(unsigned short *)(b + 0x27) = own;
    for (i = 0x29; i <= 0x2f; i++)
        b[i] = 0;
    for (i = 0x5d; i <= 0x64; i++)
        b[i] = 0;
    *(int *)(b + 0x30) = 0;
    *(int *)(b + 0x39) = 0x7d00;
    *(int *)(b + 0x3d) = 100;
    *(int *)(b + 0x41) = 2;
    *(int *)(b + 0x45) = 100;
    *(int *)(b + 0x49) = 0;
    *(float *)(b + 0x4b) = 1.0f;
    b[0x5c] = 100;
    *(int *)(b + 0x95) = -1;
    if (own != 0xffff && ai_Chars[own][2] == 6)
        ai_Unk5a8560K(b);
    *(unsigned *)(b + 0x99) = 0;
    for (i = 0x9d; i <= 0xa8; i++)
        b[i] = 0;
    *(int *)(b + 0xa5) = -1;
    *(unsigned *)(b + 0x99) |= 1;
    memset(b + 0x65, 0, 0x30);
    n = 0;
    if (**(char **)(&ai_BuildingNames[k * 0xe]) == 0) {
        b[5] = ch_EmptyName[0];
    } else {
        strcpy((char *)b + 5, *(char **)(&ai_BuildingNames[k * 0xe - 2]));
    }
    for (i = 0; i < 0xc; i++) {
        src = *(char **)(&ai_BuildingNames[k * 0xe + i]);
        if (*src != 0) {
            strcpy(cand, src);
            bl = ai_Blds;
            for (j = 0; j < 0x100; j++, bl += 0xa9) {
                if (*bl != 0 && strcmp(bl + 5, cand) == 0)
                    break;
            }
            if (j < 0x100)
                continue;
            if (strlen(cand) < 0x20) {
                strcpy(names[n], cand);
                n++;
            } else
                sprintf(err, "Building name too long: %s", cand);
        }
    }
    if (n != 0) {
        j = util_RandomU(n) & 0xffff;
        strcpy((char *)b + 5, names[j]);
    }
    w = (unsigned short *)(proto + 0x23);
    for (i = 0; i < 0x40; i++, w++) {
        if (*w == 0)
            break;
        if (*w != 0xffff) {
            c = ai_Goods[(*w & 0x7fff) * 0x41];
            if (c == 2 || c == 6)
                any = 1;
            if ((short)*w < 0) {
                if (c == 2 || c == 6) {
                    any = 1;
                    unk_Unk5031b0(b, *w & 0x7fff);
                } else if (any == 0)
                    gm_CreateObject(*(int *)(b + 1), *w & 0x7fff, 1);
            }
        }
    }
    s = (short *)gm_Query(*(void **)(b + 0x5d), 1, 4, 2);
    while (s != 0) {
        if (*s != 0xfd) {
            *(short *)(b + 0x29) = *s;
            break;
        }
        s = (short *)gm_QueryNext();
    }
    s = (short *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
    if (s != 0)
        *(short *)(b + 0x29) = *s;
    if (gm_IsHomeType(b) != 0)
        *(int *)(b + 0x30) = 0x1388;
    switch (k) {
    case 4:
    case 5:
    case 6:
    case 7:
        *(int *)(b + 0x69) = *(int *)ai_Time;
        *(int *)(b + 0x6d) = *(int *)(ai_Time + 4);
        *(int *)(b + 0x71) = *(int *)(ai_Time + 8);
        *(short *)(b + 0x75) = *(short *)(ai_Time + 12);
        if (own != 0xffff) {
            *(int *)(b + 0x65) = *(int *)(ai_Chars[own] + 4);
            break;
        }
    case 0xb:
    case 0xc:
    case 0xd:
        *(int *)(b + 0x65) = -1;
        break;
    case 0xe:
    case 0xf:
    case 0x10:
        *(int *)(b + 0x65) = 10;
        *(int *)(b + 0x69) = 3;
        *(int *)(b + 0x6d) = 5;
        break;
    case 0x14:
    case 0x15:
    case 0x16:
        q = (int *)gm_CreateObject(*(int *)(b + 1), 0x1b5, 1);
        if (q != 0) {
            *((char *)q + 0x22) = 0;
            *((char *)q + 0x1c) = -1;
            *((char *)q + 0x1d) = -1;
            *(int *)((char *)q + 0x1e) = -1;
        }
        break;
    case 0x1e:
        *(void **)(b + 0x71) = mem_Alloc(0x600, "f3_gm:PlantMap");
        for (i = 0; i < 0x600; i += 0x18) {
            *((char *)*(void **)(b + 0x71) + i + 0xd) = -1;
            *(int *)((char *)*(void **)(b + 0x71) + i + 0x14) = 0;
        }
    case 0x1f:
    case 0x20:
        *(int *)(b + 0x30) = 100;
        *(float *)(b + 0x49) = 0.8f;
        break;
    case 0x35:
    case 0x36:
    case 0x37:
        *(int *)(b + 0x65) = 0;
        *(int *)(b + 0x69) = 0;
        *(int *)(b + 0x6d) = 0;
        break;
    case 0x47:
        *(int *)(b + 0x30) = 0x13880;
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        *(int *)(b + 0x65) = (int)gv_AllocGraveyard(b);
        gv_BuryNewDead(b, 3);
        break;
    case 0x4b:
    case 0x4c:
    case 0x4d:
        q = (int *)mem_Alloc(0x3c, "AHM_INFO");
        *(int **)(b + 0x65) = q;
        *q = -1;
    }
    if (*b == 0x26) {
        b[0x29] = 0xe;
        b[0x2a] = 1;
    }
    gm_SetCapacities(b);
    gm_BuildingClass(*b);
    gm_StockBuildingObjects(b);
    return b;
}
/* 0x501b50, the main object of the building b by its building class (0x501d80): classes 1 / 7 / 8: by type 0x1d the object 0x205, 0x16 the 0x12e, 4 the 0x54, 0x1f the 0xfd; class 4: 0xf7; homes (0x502520): 0xfd; else the first object of the kind 2 (not 0xfd) in the building's prototype list. ROUGH */

int gm_BuildingMainObject(unsigned char *b)
{
    int cls, i, o, id;
    char *k;
    unsigned short *w;
    unsigned u;

    o = 0;
    cls = gm_BuildingClass(*b);
    k = ai_BTypes + *b * 0x24d;
    if (cls != 1 && cls != 7 && cls != 8) {
        if (cls == 4)
            return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xf7);
        if (gm_IsHomeType(b) == 0) {
            w = (unsigned short *)(k + 0x23);
            i = 0;
            while (*w == 0xffff || (u = *w & 0x7fff, ai_Goods[u * 0x41] != 2) || u == 0xfd) {
                i++;
                w++;
                if (i > 0x3f)
                    return 0;
            }
            return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, u);
        }
        u = 0xfd;
        goto done;
    }
    if (*k == 0x1d) {
        id = 0x205;
    use_list:
        o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, id);
        if (o != 0)
            return o;
    } else {
        if (*k == 0x16) {
            id = 0x12e;
            goto use_id;
        }
        if (*k == 4) {
            id = 0x54;
            goto use_id;
        }
        if (*k == 0x1f) {
            id = 0xfd;
            goto use_list;
        }
    }
    goto rest;
use_id:
    o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, id);
    if (o != 0)
        return o;
rest:
    w = (unsigned short *)(k + 0x23);
    i = 0;
    while (*w == 0xffff || (u = *w & 0x7fff, ai_Goods[u * 0x41] != 2) || u == 0xfd) {
        i++;
        w++;
        if (i > 0x3f)
            return o;
    }
done:
    return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, u);
}
/* 0x501ce0, queries the main object of the building b for the side effect (the object 0xfd of a home, the 0x54 of a type 4 building, else walks the kind 2 objects while they are 0xfd) */

void gm_GetGebaeudeHauptraum(unsigned char *b)
{
    char *k;
    short *s;

    gm_BuildingClass(*b);
    k = ai_BTypes + *b * 0x24d;
    if (gm_IsHomeType(b) != 0) {
        gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        return;
    }
    if (*k != 4) {
        s = (short *)gm_Query(*(int *)(b + 0x5d), 1, 4, 2);
        while (s != 0 && *s == 0xfd)
            s = (short *)gm_QueryNext();
        return;
    }
    gm_Query(*(int *)(b + 0x5d), 1, 0, 0x54);
}
/* 0x501d80, the class of a building kind (the type byte of the kind record): 3 for 1 / 3 / 6 / 0xf, 6 for 2 / 0x1e, 8 for 4 / 5 / 9, 4 for 7, 1 for 8 / 0xe / 0x12 / 0x14 .. 0x16 / 0x1d / 0x1f / 0x20, 2 for 0xb .. 0xd, 7 for 0x13, 5 for 0x17 .. 0x1a, 9 for 0x21, else 0 */

int gm_BuildingClass(int kind)
{
    switch (ai_BTypes[kind * 0x24d]) {
    case 1:
    case 3:
    case 6:
    case 0xf:
        return 3;
    case 2:
    case 0x1e:
        return 6;
    case 4:
    case 5:
    case 9:
        return 8;
    case 7:
        return 4;
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
    default:
        return 0;
    case 0xb:
    case 0xc:
    case 0xd:
        return 2;
    case 0x13:
        return 7;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        return 5;
    case 0x21:
        return 9;
    }
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
/* 0x501f60, like 0x501e40 for buildings without an owner (+0x27 = -1), taking the positions directly from the objects (+0x4c .. +0x54) */

unsigned char *gm_NearestFreeBuilding(unsigned char *b, unsigned kind)
{
    float best, d, dx, dy, dz;
    unsigned char *r, *p;
    int o, bo;

    best = 1e+08f;
    r = 0;
    p = (unsigned char *)gm_BuildQuery(1, 6);
    while (p != 0) {
        if (gm_IsHomeType(p) == 0 && gm_IsType10(p) == 0 && (o = *(int *)(p + 0x61)) != 0 && p != b && *p == kind && *(short *)(p + 0x27) == -1) {
            bo = *(int *)(b + 0x61);
            dx = *(float *)(o + 0x4c) - *(float *)(bo + 0x4c);
            dy = *(float *)(o + 0x50) - *(float *)(bo + 0x50);
            dz = *(float *)(o + 0x54) - *(float *)(bo + 0x54);
            d = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (d < best) {
                r = p;
                best = d;
            }
        }
        p = gm_BuildNext();
    }
    return r;
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

int gm_BuildingJob();                     /* 0x5048e0 */
int gm_ListBuildingsOfType();                     /* 0x5020a0 */
extern int gm_AllowAll;

/* 0x5020e0, whether the character c may take the job of the building kind k: 0 no, 1 yes, 2 yes in a neighbouring building of the same type (0x5020a0 lists them) ; by the building's class (0x501d80): 1 / 7 / 8 the jobs of the kind (the six bytes at +0x223) match the character's job number (0x5044b0), 2 never, 3 by the character's kind (0xf: kinds 1 / 0xf / 6, 0xa: kind 3), 4 own list only, 6 by the rank byte at +0x9 of the character (limits 0 / 3 / 4 / 5 by the kind) giving 1 or 2. With the flag 0x6f2448 every person (kind 6 / 7) may. ROUGH */
#define JOB_FIND(t0, base, v, code)                                                  \
    do {                                                                             \
        t = (unsigned char *)(base) + 0x223;                                         \
        do {                                                                         \
            if (*t == 0)                                                             \
                break;                                                               \
            if ((v) == *t)                                                           \
                return code;                                                         \
            t++;                                                                     \
        } while ((int)(t - ((unsigned char *)(base) + 0x223)) < 6);                  \
    } while (0)

/* 0x5020e0, whether character c may take the job of building type k: 0 no, 1 yes, 2 yes in a neighbouring building of that type */
int gm_MayTakeJob(int k, int c)
{
    int cls, i, n;
    char *p, *ch;
    int job;
    unsigned v;
    unsigned char *t;
    int list[8];
    unsigned char lim;

    cls = gm_BuildingClass(k);
    p = ai_BTypes + k + k * 0x24c;
    job = gm_BuildingJob(*p);
    v = gm_TitleJob(c, job);
    if (c == 0xffff)
        return 0;
    ch = ai_Chars[c];
    if (gm_AllowAll != 0 && (ch[2] == 6 || ch[2] == 7))
        return 1;
    if (cls == 1) {
        JOB_FIND(t, p, v, 1);
        n = gm_ListBuildingsOfType(*p, list);
        i = 0;
        if (0 < n) {
            do {
                JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                i++;
                if (n <= i)
                    return 0;
            } while (1);
        }
    } else {
        if (cls == 2)
            return 0;
        if (cls == 3) {
            if (ch[2] == 0xf) {
                if (*p == 1)
                    return 1;
                if (*p == 0xf)
                    return 1;
                if (*p == 6)
                    return 1;
            }
            if (ch[2] == 10 && *p == 3)
                return 1;
            return 0;
        }
        if (cls == 7) {
            if (*p == 0x13) {
                JOB_FIND(t, p, v, 1);
                n = gm_ListBuildingsOfType(0x13, list);
                i = 0;
                if (0 < n) {
                    do {
                        JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                        i++;
                        if (n <= i)
                            return 0;
                    } while (1);
                }
            }
        } else if (cls == 4) {
            t = (unsigned char *)(p + 0x223);
            do {
                if (*t == 0)
                    return 0;
                if (v == *t)
                    return 1;
                t++;
            } while ((int)(t - (unsigned char *)(p + 0x223)) < 6);
        } else {
            if (cls == 6) {
                lim = 0;
                switch (k) {
                case 4:
                    return ((unsigned char)ch[9] == 0) + 1;
                case 5:
                    return ((unsigned char)ch[9] < 3) + 1;
                case 6:
                case 0x4b:
                case 0x4c:
                case 0x4d:
                    return ((unsigned char)ch[9] < 4) + 1;
                case 7:
                    lim = 5;
                }
                return ((unsigned char)ch[9] < lim) + 1;
            }
            if (cls != 8)
                return 1;
            JOB_FIND(t, p, v, 1);
            n = gm_ListBuildingsOfType(*p, list);
            i = 0;
            if (0 < n) {
                do {
                    JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                    i++;
                    if (n <= i)
                        return 0;
                } while (1);
            }
        }
    }
    return 0;
}
/* 0x5024f0, 1 when the building b is of the type 10 (the town market) */

int gm_IsType10(unsigned char *b)
{
    return ai_BTypes[*b * 0x24d] == 10;
}
/* 0x502520, 1 when the building b is a home (the types 0xb, 0xc, 0xd, 0x10, 0x1c, 0x1d, 0x1f) */

int gm_IsHomeType(unsigned char *b)
{
    char k;

    if (b == 0)
        return 0;
    k = *(char *)(ai_BTypes + (unsigned)*b * 0x24d);
    if (k != 0x0b && k != 0x0d && k != 0x0c && k != 0x10 && k != 0x1c && k != 0x1d && k != 0x1f)
        return 0;
    return 1;
}

int gm_JobBuildingType(int x);
extern unsigned short * sv_PanelShown;                /* 0x6f2310 */
/* ai_Unk61a688: view of ai_Unk61a686 (tools/data_views.py) */
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
void he_Unk5538b0(unsigned short *p);                     /* 0x5538b0 */
void he_Unk568aa0(int ev);                     /* 0x568aa0 */
void d3io_NoopCallback();                     /* 0x44ee40 */
int amt_ClearOffices();                     /* 0x4e6d80 */
void unk_Unk503c60(int c, int mode);                     /* 0x503c60 */
extern char *ai_Unk5fb5b0;  /* 0x5fb5b0 (d3_DefaultScene, now defined in d3_engine.c) */

/* 0x502570, the string-table number of the object o (0x60e + n) for the object kinds 2 / 6: a table of (n, object) pairs (0x13 -> 0x54, 2 -> 0x15, 4 -> 0x18 ..) searched for the prototype number of the object; -1 when it is not in it. ROUGH */
int gm_GoodPicture(unsigned short *o)
{
    int t[0x6d] = { 0x1, 0x54, 0x2, 0x15, 0x4, 0x18, 0xd, 0x14, 0x9, 0x16, 0x18, 0x17, 0xf, 0x37, 0x0, 0x12, 0x19, 0x9, 0x1, 0xcf, 0x20, 0xd6, 0x1b, 0xdd, 0x1f, 0x120, 0xa, 0x12e, 0x1c, 0x10e, 0x1d, 0x110, 0x1, 0x114, 0x1e, 0x139, 0x1e, 0x138, 0x1, 0x137, 0x1d, 0xf2, 0xe, 0xe6, 0x21, 0xe5, 0x21, 0xf0, 0xb, 0xf7, 0x1c, 0xb7, 0x15, 0xba, 0x1a, 0xbb, 0x5, 0x4d, 0x7, 0x5c, 0x14, 0x60, 0x12, 0x65, 0x1a, 0x145, 0x2, 0x146, 0x15, 0x14c, 0x16, 0x74, 0x20, 0x85, 0x20, 0x118, 0x20, 0x9b, 0x20, 0xa5, 0x1c, 0xf3, 0x22, 0x205, 0x23, 0x21b, 0x24, 0x211, 0x25, 0x276, 0x26, 0x28a, 0x27, 0x27f, 0x28, 0x24b, 0x29, 0x256, 0x2a, 0x266, 0x9, 0x2c, 0x20, 0x2d, 0x20, 0x33, 0x20, 0x0, -1 };
    char k;
    int i;
    unsigned v;

    k = ai_Goods[*o * 0x41];
    if (k == 2 || k == 6) {
        i = 0;
        v = 0x13;
        do {
            if (v == *o)
                return t[i * 2] + 0x60e;
            v = t[i * 2 + 1];
            i++;
        } while (v != 0);
    }
    return -1;
}
/* 0x5029f0, callback of 0x438d40 for 0x502a60: collects the objects whose prototype (low 24 bits of +0x48) is of the kind 0x21 and has
   its top byte 1 into list (0x40 objects, then their keys, then the count); true while there is room */

char gm_Unk5029f0(char *o, int *list)
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
/* 0x502a60, for the building object o of the player in the building b with the character c at the scene 0x6f2310: the five slots of the character (bytes +0x80 .. of the record 0x770c00 + c * 0x21c) give the numbers 0x17f + (the scaled value) + 6k, 0 for an empty slot; the scene's objects (0x438d40 with the callback 0x5029f0 into a 0x40-entry list) whose key matches one of them are switched on (0x447300 (obj, 1)), the others off. ROUGH */

void gm_ShowSlotObjects(int unused, int scene, int c)
{
    int want[5];
    int list[0x40 * 2 + 1];
    int i, k, n, v;
    char *s;

    if (sv_PanelShown == scene) {
        k = 0;
        s = ai_Chars[c] + 0x80;
        i = 0;
        do {
            v = (int)(float)0;
            v = v + 0x17f + k;
            if (*s == 0)
                v = 0;
            want[i] = v;
            k += 6;
            s++;
            i++;
        } while (k < 0x1e);
        memset(list, 0, sizeof(list));
        d3_ForEachObject(ai_Unk5fb5b0, 0, 0xc0, gm_Unk5029f0, list);
        n = list[0x40 * 2];
        for (i = 0; i < n; i++) {
            d3if_ShowTree(list[i], 0);
            for (k = 0; k < 5; k++)
                if (list[i + 0x40] == want[k])
                    d3if_ShowTree(list[i], 1);
        }
    }
}

/* ---- the character and building life cycle 0x502b60 .. 0x504510 (ROUGH drafts, written out of address order at the end of the file) ---- */
/* ai_Unk91f716: view of ai_Crimes (tools/data_views.py) */
extern int ai_Evidence[0x800][2];              /* 0x91b6e0 .. 0x91f6e0: pairs of ints */
extern int ai_Unk770ae0_day;            /* the clock's day (0x770ae0) */
#ifndef TIME_VIEW_H
extern unsigned short ai_TimeHour;     /* 0x770ae4, the clock's hour (unaligned view) */
#endif
extern char ai_Unk770ce4[];             /* 0x770ce4: the field +0x164 of the character records (stride 0x21c) */
int ai_Unk5a86c0();                     /* 0x5a86c0 */
void sv_Unk5286a0(char *b);                     /* 0x5286a0 */
void sv_Unk5282f0(char *b);                     /* 0x5282f0 */
void ch_Unk402540();                     /* 0x402540 */
int ch_Delete();                     /* 0x4028a0 */
void ai_ShowEvidenceVoidNews(int param_1, unsigned short *param_2);                     /* 0x5a9430 */
int gm_FreeList();                     /* 0x500140 */
int gm_TitlesLeft();                     /* 0x504ce0 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int gm_CompareTime();                     /* 0x4fe210 */
int gm_MinutesBetween();                     /* 0x4fe280 */
int d3t_SceneIndex();                     /* 0x411be0 */
double _CIfmod();                     /* 0x5de73a: a CRT call */
extern int ai_CharCount;  /* 0x6f1bc0 */
extern int f3gm_ObjectCount;  /* 0x6f1bdc */

/* 0x502b60, hands the building b over to the owner (character slot) own, at the place a: the old owner (a kind 6 character) loses it (0x5a86c0), the new one (kind 6) gets it (0x5a8560); the person living in it is cleaned up, the objects of kind 0x1d (graveyard) in its list get the owner, and the scene objects are refreshed (0x5286a0 / 0x5282f0). ROUGH */
void gm_AssignBuilding(unsigned char *b, unsigned short place, unsigned own)
{
    char *p;
    unsigned short o;
    char *bl;
    int per, k;
    unsigned short *w;
    int cnt, scn, ob, owid;
    char t;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    *(unsigned short *)(b + 0x25) = place;
    if (own != 0xffff) {
        o = *(unsigned short *)(b + 0x27);
        if (o != own) {
            if (o != 0xffff && ai_Chars[o][2] == 6)
                ai_Unk5a86c0(*(int *)(b + 1));
            if (ai_Chars[own][2] == 6)
                ai_Unk5a8560K(b);
        }
        o = *(unsigned short *)(b + 0x27);
        if (o != 0xffff && *(unsigned char **)(ai_Chars[o] + 0x170) == b)
            *(int *)(ai_Chars[o] + 0x170) = 0;
        *(unsigned short *)(b + 0x27) = own;
        if (*p == 2) {
            gm_ShowBuildingObjects(b);
            if (*p == 2) {
                bl = *(char **)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x170);
                if (bl == 0 || (unsigned char)*bl <= *b)
                    *(unsigned char **)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x170) = b;
            }
        }
        t = ai_Chars[own][2];
        if (t == 6 || t == 7) {
            per = gm_FindCharacterByMaster(b);
            if (per != 0) {
                *(char *)(per + 0x165) = 0;
                *(unsigned *)(per + 0x1b4) = *(unsigned *)(per + 0x1b4) & 0xfffffffe;
                if (*p == 5 || *p == 9)
                    *(int *)(per + 0x16c) = 0;
            }
        }
        cnt = 0x5008;
        w = (unsigned short *)ai_Objects;
        do {
            if (*w != 0 && ai_Goods[*w * 0x41] == 0x1d && *(int *)(w + 0xe) == *(int *)(b + 1)) {
                *(int *)(w + 5) = *(int *)(ai_Chars[own] + 4);
                if (*(int *)(w + 10) != 0)
                    *(int *)(*(int *)(w + 10) + 10) = *(int *)(ai_Chars[own] + 4);
                scn = d3_CurrentScene;
                ob = *(int *)((char *)w + 0x3b);
                if (*(int *)(ai_Chars[own] + 0x54) == 0 || ((t = ai_Chars[own][2]) != 5 && t != 6 && t != 7))
                    owid = 0;
                else
                    owid = *(int *)(ai_Chars[own] + 0x54) - 0x599;
                k = d3t_SceneIndex(*(int *)(ob + 0x88), 1);
                d3if_SelectScene(k);
                if (*(int *)(*(int *)(ob + 0x34) + 0x1cc) == 0)
                    ch_Unk402540(ob);
                d3if_SelectTextureSet(**(int **)(ob + 0x124), *(int *)(**(int **)(ob + 0x124) + 0x1cc), owid, 1);
                d3if_SelectScene(scn, 1);
            }
            scn = d3_CurrentScene;
            w = (unsigned short *)((char *)w + 0x43);
            cnt--;
        } while (cnt != 0);
        if (ai_Chars[own] != ai_PlayerCharC && *(int *)(b + 0x61) != 0 && ((t = ai_Chars[own][2]) == 6 || t == 7 || t == 5)) {
            d3if_SelectScene(0, 1);
            sv_Unk5286a0(b);
            sv_Unk5282f0(b);
            d3if_SelectScene(scn, 1);
            return;
        }
        d3if_SelectScene(0, 1);
        sv_Unk5286a0(b);
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
/* 0x502eb0, sorts the characters that work in the building b into the lists of an output record and returns their number: out[0] = their number, out[1 ..] (0x18 words, -1 filled) the workers of the first job class (type bytes +0x22f), out[0xd ..] (0xc) of the second (+0x230), out[0x19 ..] (0xc) the others (not the building's own two special jobs +0x223 / +0x224), out[0x25] = the one of the special jobs. ROUGH */

int gm_ListWorkers(unsigned char *b, int *out)
{
    int i, n, j;
    char *k;
    char *c;
    unsigned *p1, *p2, *p3;

    k = ai_BTypes + *b * 0x24d;
    p1 = (unsigned *)(out + 1);
    out[0] = 0;
    for (j = 0; j < 0x18; j++)
        p1[j] = 0xffff;
    p3 = (unsigned *)(out + 0x19);
    for (j = 0; j < 0xc; j++)
        p3[j] = 0xffff;
    out[0x25] = 0xffff;
    p2 = (unsigned *)(out + 0xd);
    n = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(unsigned char **)(c + 0x170) == b) {
            if (k[0x22f] == c[0x165]) {
                *p1++ = *(unsigned short *)c;
            } else if (k[0x230] == c[0x165]) {
                *p2++ = *(unsigned short *)c;
            } else if (k[0x223] == 0 || k[0x224] == 0 || (k[0x223] != c[0x164] && k[0x224] != c[0x164])) {
                *p3++ = *(unsigned short *)c;
            } else {
                out[0x25] = i;
            }
            n++;
        }
    }
    out[0] = n;
    return n;
}
/* 0x502fd0, the sale of the building with the id a to the player b for the price c (the sale record d, flag e): marks "buy building" in the message log (0x4c4540 .. 0x4c4580), pays the former owner (a message 0x17e6 with "_NACHRICHTEN_HS_30", 0x4c4830 the price) or the bank (0x4c47e0), hands the ownership over (0x4c54a0, and for the type 2 with e the 0x4c5540), stamps the sale record d one hour ahead (0x4fe140 +1 second, 0x4c4c60) and sets the building's owner field (0x4c4a60 (id, 0x5a, 2, 0, 2)). ROUGH */

void gm_SellBuilding(int a, int b, int c, int d, int e)
{
    unsigned char *bl;
    short *ch;
    char *k;
    char buf[256];
    int u4, u5;

    bl = (unsigned char *)gm_BuildQuery(1, 1, a);
    ch = (short *)gm_FindChar(b);
    k = ai_BTypes + *bl * 0x24d;
    cm_BeginGroup("buy building");
    if (*(short *)(bl + 0x25) == -1) {
        cm_SendMoveGoods(-1, b, ai_OwnPlayer, c);
        cm_SendOwnership(*(int *)(bl + 1), *(int *)(ch + 2), *(int *)(ch + 2));
        if (e == 0 || *k != 2)
            goto L1;
        u4 = *(int *)(bl + 1);
        u5 = *(int *)(ch + 2);
    } else {
        txt_Format(buf, 0x17e6, bl, *ch, c);
        he_MessageBoxQuickjump(*(int *)(ai_Chars[*(unsigned short *)(bl + 0x25)] + 4), -1, buf, 0, 0x5ff, *(int *)(bl + 1), -1, 0, "_NACHRICHTEN_HS_30");
        cm_SendMoveGoodsUnchecked(*(int *)(ai_Chars[*(unsigned short *)(bl + 0x25)] + 4), b, ai_OwnPlayer, c);
        cm_SendOwnership(*(int *)(bl + 1), *(int *)(ch + 2), *(int *)(ch + 2));
        if (e == 0 || *k != 2)
            goto L1;
        u4 = *(int *)(bl + 1);
        u5 = *(int *)(ch + 2);
    }
    cm_SendBoss(u5, u4);
L1:
    if (d != 0) {
        *(int *)(d + 0x52) = *(int *)ai_Time;
        *(int *)(d + 0x56) = *(int *)(ai_Time + 4);
        *(int *)(d + 0x5a) = *(int *)(ai_Time + 8);
        *(short *)(d + 0x5e) = *(short *)(ai_Time + 0xc);
        gm_AddTime((GCLOCK *)(d + 0x52), 0, 0, 1);
        *(int *)(d + 0x84) = ((int (*)(int, int))cm_SendExSetHE)(-1, d);
    }
    cm_SendSetField(*(int *)(bl + 1), 0x5a, 2, 0, 2);
    cm_EndGroup("buy building");
}
/* 0x5031b0, "gm_AllocRaum": creates the building b's (kind) object n with all its sub-objects (the object words after it up to the next kind 2 / 6 object that are marked negative), sets the counts of its first kind 0x2a / 0x116 objects (the bytes at +0x23d / +0x23e / +0x23f of the building prototype, at least 2 / 6), and for the market stalls 0x92 .. 0x97 / 0x2db the store room ob_LAGERFLAECHE_ALLGEMEIN with the player's goods. ROUGH */

unsigned short *unk_Unk5031b0(unsigned char *b, unsigned n)
{
    unsigned short *o, *s;
    int i, bi, k, pl, cap;
    char *bp;
    unsigned short w;
    char cl;
    char *g;
    int *gp;
    unsigned char c1, c2;
    int *goods;
    int *tab;

    if (ai_Goods == 0 || ai_BTypes == 0 || f3gm_ObjectCount + 0x40 > 0x5007)
        return 0;
    o = (unsigned short *)gm_CreateObject(*(int *)(b + 1), n, 1);
    i = 0;
    bp = ai_BTypes + (unsigned)*b * 0x24d;
    k = (int)(bp + 0x23);
    w = *(unsigned short *)(bp + 0x23);
    while ((w & 0x7fff) != n) {
        k += 2;
        i++;
        w = *(unsigned short *)k;
    }
    while (i++, i < 0x40) {
        w = *(unsigned short *)(bp + 0x23 + i * 2);
        if (w == 0)
            break;
        if (w != 0xffff) {
            cl = ai_Goods[(w & 0x7fff) * 0x41];
            if (cl == 2 || cl == 6)
                i = 0x40;
            else if ((short)w < 0)
                gm_CreateObject(*(int *)(o + 1), w & 0x7fff, 1);
        }
    }
    g = gm_Query(*(int *)(o + 10), 1, 0, 0x2a);
    if (g != 0) {
        *(int *)(g + 0xe) = 2;
        *(int *)(g + 6) = *(int *)(b + 1);
        c1 = (unsigned char)bp[0x23d];
        if (c1 == 0 && bp[0x23e] == 0 && bp[0x23f] != 0)
            g[0x1c] = bp[0x23f];
        else {
            if (c1 < 2)
                c1 = 2;
            g[0x1c] = c1;
            c2 = (unsigned char)bp[0x23e];
            if (c2 < 2)
                c2 = 2;
            g[0x1d] = c2;
        }
    }
    g = gm_Query(*(int *)(o + 10), 2, 0, 0x116, 6);
    if (g != 0) {
        *(int *)(g + 0xe) = 2;
        *(int *)(g + 6) = *(int *)(o + 1);
        c1 = (unsigned char)bp[0x23d];
        if (c1 == 0 && bp[0x23e] == 0 && (c2 = (unsigned char)bp[0x23f]) != 0) {
            if (c2 < 6)
                g[0x1c] = 6;
            else
                g[0x1c] = c2;
        } else {
            if (c1 < 6)
                c1 = 6;
            g[0x1c] = c1;
            c2 = (unsigned char)bp[0x23e];
            if (c2 < 6)
                c2 = 6;
            g[0x1d] = c2;
        }
    }
    w = *o;
    if ((0x91 < w && w < 0x98) || w == 0x2db) {
        pl = ai_OwnPlayer;
        g = gm_Query(*(int *)(o + 10), 1, 0, 0x116);
        if (g == 0)
            err_Message("gm_AllocRaum(): ob_LAGERFLAECHE_ALLGEMEIN is missing in ob_MARKTSTAND!");
        else {
            g[0x1c] = 0xc;
            g[0x1d] = 0;
            *(int *)(g + 0xe) = 3;
        }
        bi = gm_CreateObject(*(int *)(o + 1), 0xff, 1);
        cap = 0;
        goods = ai_Unk6178c8;
        gp = (int *)(init_StockBlocks + 4 + pl * 0x2a90);
        do {
            if (*(unsigned short *)(gp + 0xe) == *o && *gp != 0 && cap < 0x10) {
                gm_CreateObject(*(int *)(bi + 2), *(unsigned short *)goods, 1);
                cap++;
            }
            goods += 0x10;
            gp += 0x20;
        } while ((int)goods < (unsigned)((unsigned)ai_Unk6178c8 + 0x2a80));
    }
    return o;
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
    int saved, r, m, sc;

    saved = d3_CurrentScene;
    r = *(int *)(o + 0x24);
    if (r != 0 && (*(unsigned char *)(r + 0x78) & 2) != 0)
        *(int *)(r + 0x84) = ((int (*)(int, int))cm_SendExSetHE)(-1, r);
    m = *(int *)(o + 0x3b);
    if (m != 0) {
        sc = d3t_SceneIndex(*(int *)(m + 0x88), 1);
        d3if_SelectScene(sc);
        ch_Delete(m);
        d3if_SelectScene(saved, 1);
    }
    r = gm_Unk50a0f0(o);
    if (r != 0)
        gm_RemoveOwned((int *)(r + 0x5d), *(int *)(o + 2));
}

int ai_Unk587b70();                     /* 0x587b70 */
extern int gm_AllowAll;                /* 0x6f2448 */
int gm_BuildingJob();                     /* 0x5048e0 */
int gm_TitleGroup();                     /* 0x504ad0 */

/* 0x503530, the permission bits of the character p1 in the building b (or, without a building, in the object o / the character c): a bit set of what he may do there (0x1 enter, 0x4 .. 0x30 use, 0x40 .., 0x800 .., 0x2000 .., 0x4000, 0x5000 ..: the exact meaning of each bit is open) by the building type: a case per type, owner (+0x27) and place (+0x25) words compared with p1, the job list (six bytes at +0x223) searched for the job number of the character (0x5044b0). With the flag 0x6f2448 everything is allowed (-1, -2 for the types 3 / 0x21). ROUGH */
unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c)
{
    char *p;
    unsigned v, jn, w;
    char t, cc;
    int r, jb;
    unsigned char *q;
    unsigned short ob;

    v = 0;
    p = 0;
    if (b == 0) {
        if (c == 0) {
            if (o == 0)
                goto fin;
            ob = *o;
            if (ai_Goods[ob * 0x41] == 6) {
                if (ob < 0x91 || ob > 0x97)
                    v = 0xffffffff;
                else
                    v = 0x4001;
            } else if (ob < 0x91 || ob > 0x97)
                v = (-(unsigned)(ob != 0xfd) & 0xffffbffe) + 0x4001;
            else
                v = 1;
        } else if (*(int *)(c + 0x16c) != 0 && *(unsigned short *)(*(int *)(c + 0x16c) + 0x27) == p1)
            v = 0x800;
        goto dflt;
    }
    p = ai_BTypes + (unsigned)*b * 0x24d;
    jb = gm_BuildingJob(*p);
    jn = gm_TitleJob(p1, jb);
    t = *p;
    if (t == 0x1d)
        return ai_Unk587b70(ai_Chars[p1], b, o);
    switch (t) {
    case 1:
    case 3:
    case 0xf:
        if (t != 3)
            v = 0x5081;
        cc = ai_Chars[p1][0x166];
        if (cc == 0xf || cc == 0xe || cc == 0xd)
            v |= 0x502;
        if (cc == 0xf || cc == 0xe || cc == 0xd)
            v |= 0x200;
        if (t == 3)
            r = cc == 10;
        else
            r = cc == 0xf;
        if (r)
            v |= 0x30;
        if (o == 0)
            goto fin;
        if (*o == 0x114 && cc == 0)
            v &= 0xffffbffe;
    case 0x1b:
    case 0x1d:
        break;
    case 2:
    case 0x1e:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x3c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5661;
        break;
    case 4:
    case 0x13:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x50bd;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x44e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
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
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x10bd;
        if (*(unsigned short *)(b + 0x27) == p1) {
            w = v | 0x460;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0) {
                set:
                    v = w | 0x4000;
                    goto dflt;
                }
                if (jn == *q) {
                    w = v | 0x2660;
                    goto set;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
            v |= 0x4460;
        }
        break;
    case 6:
        v = 0x5001;
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x503d;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5660;
        if (ai_Chars[p1][0x166] == 0xf)
            v |= 0x30;
        break;
    case 7:
        v = 1;
        if (o != 0 && (*o == 0xe6 || *o == 0xe5))
            v = 0x1001;
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x103c;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x4460;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        } else {
            if (o == 0)
                goto fin;
            if (*o != 0xf3)
                v |= 0x5000;
        }
        break;
    case 9:
        v = 1;
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x403d;
        if (*(unsigned short *)(b + 0x27) == p1) {
            v |= 0x54e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v |= 0x2200;
                    goto dflt;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        }
        break;
    case 10:
        v = 1;
        break;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0x11:
    case 0x1f:
        if (o == 0 || *(unsigned short *)(b + 0x27) == p1)
            v = 0x81;
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x403c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x5670;
        if (o == 0)
            goto fin;
        if (*o == 0xfd)
            v |= 0x4001;
        break;
    case 0x10:
        if (*(unsigned short *)(b + 0x27) == p1) {
            v = 0x54e1;
            q = (unsigned char *)(p + 0x223);
            do {
                if (*q == 0)
                    break;
                if (jn == *q) {
                    v = 0x76e1;
                    break;
                }
                q++;
            } while ((int)(q - ((unsigned char *)p + 0x223)) < 6);
        }
        if (*(unsigned short *)(b + 0x25) == p1)
            v |= 0x3c;
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        v = 0x5081;
        if ((t == 0x17 && ai_Chars[p1][9] == 0x1e) || (t == 0x18 && ai_Chars[p1][9] == 0x1f) ||
            (t == 0x19 && ai_Chars[p1][9] == 0x20) || (t == 0x1a && ai_Chars[p1][9] == 0x21))
            v = 0x52f1;
        break;
    case 0x1c:
        r = gm_TitleGroup(*(int *)(ai_Chars[p1] + 0x164));
        if (r == 8 || gm_BuildQuery(2, 5, 9, 4, *(short *)ai_Chars[p1]) != 0)
            v = 1;
        break;
    case 0x21:
        if (*(unsigned short *)(b + 0x25) == p1)
            v = 0x3c;
        if (*(unsigned short *)(b + 0x27) == p1)
            v |= 0x620;
    }
dflt:
    if (o != 0) {
        ob = *o;
        if (ob == 0x27f || ob == 0x21b || ob == 0x28a || ob == 0x120)
            v |= 0x5000;
        if (ob == 0x12e)
            v |= 0x4000;
    }
fin:
    if (gm_AllowAll != 0) {
        v = 0xffffffff;
        if (b == 0)
            return 0xffffffff;
        if (*p == 3 || *p == 0x21)
            v = 0xfffffffe;
    }
    if (b != 0 && *(short *)(b + 0x27) == -1)
        v = 0;
    return v;
}
/* 0x503c20, releases what hangs on the character c (not for a free slot): 0x524260 (0, 0, id, 0), 0x5538b0, 0x568aa0, 0x44ee40, 0x4e6d80 */

void gm_Unk503c20(short *c)
{
    if (c != 0 && *c != -1) {
        sv_MoveToGroup(0, 0, *c, 0);
        he_Unk5538b0(c);
        he_Unk568aa0(c);
        d3io_NoopCallback(c);
        amt_ClearOffices(c);
    }
}
/* 0x503c60, "gm_KillSpieler": removes the character c: with mode 0 only its data is held (kind 0xf = dead; the clock stamped at +0x28 / +0x48; the party records / pairs of the player cleaned up), else it is removed for good (the slot freed, the number of living (0x6f1bc0) decremented, the references in the other characters' relative lists (+0x5c ..) cleared). ROUGH */

void unk_Unk503c60(int c, int mode)
{
    char buf[0x200];
    char *p;
    int id, i, cnt;
    char *peer;
    int *q, *r;
    unsigned char *b;
    int k;

    id = *(int *)(ai_Chars[c] + 4);
    p = ai_Chars[c];
    if (p[2] != 0xf) {
        *(int *)(p + 8) = 0;
        *(unsigned short *)(p + 0x28) = *(unsigned short *)ai_Time;
        gm_Unk503c20(p);
        p[2] = 0xf;
        peer = 0;
        cnt = 0;
        for (i = 0; i < 0x300; i++) {
            if (ai_Chars[i][2] == 6) {
                peer = ai_Chars[i];
                break;
            }
        }
        q = (int *)(ai_Unk91f716);
        do {
            if (*q == *(int *)(ai_Chars[c] + 4)) {
                k = *(int *)((char *)q - 0x16);
                r = ai_Evidence;
                do {
                    if (r[1] == k) {
                        if (peer != 0 && *(int *)(peer + 4) == *r)
                            cnt++;
                        r[1] = -1;
                        *r = -1;
                    }
                    r += 2;
                } while ((int)r < ((unsigned)ai_Evidence + 0x4000));
                q[-1] = -1;
                *q = -1;
                *(int *)((char *)q - 0x16) = -1;
            }
            q = (int *)((char *)q + 0x2d);
        } while ((int)q < ((unsigned)((unsigned)ai_Unk91f716 + 0x59fe) + 0x2));
        if (cnt != 0 && peer != 0 && peer != p)
            ai_ShowEvidenceVoidNews(peer, p);
    }
    gm_FreeList(ai_Chars[c] + 0x178);
    if (*(int *)(ai_Chars[c] + 0x184) != 0) {
        ch_Delete(*(int *)(ai_Chars[c] + 0x184));
        *(int *)(ai_Chars[c] + 0x184) = 0;
    }
    b = *(unsigned char **)(ai_Chars[c] + 0x170);
    if (b != 0) {
        if (ai_BTypes[*b * 0x24d] == 1 && b[0x65] != 0)
            b[0x65]--;
        *(int *)(ai_Chars[c] + 0x170) = 0;
    }
    *(int *)(ai_Chars[c] + 0x16c) = 0;
    if (mode == 0) {
        sprintf(buf, "gm_KillSpieler: Killing Player <id = %i>, holding data", id);
    } else {
        if (*(short *)p != -1)
            ai_CharCount--;
        q = (int *)(ai_Chars[0] + 0x5c);
        do {
            i = 8;
            r = q;
            do {
                if (*r == *(int *)(ai_Chars[c] + 4))
                    *r = -1;
                r++;
                i--;
            } while (i != 0);
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x5a));
        memset(p, 0, 0x21c);
        *(short *)p = -1;
        *(int *)(ai_Chars[c] + 4) = -1;
        r = (int *)(ai_Chars[c] + 0x5c);
        for (i = 8; i != 0; i--)
            *r++ = -1;
        if (mode == 1)
            sprintf(buf, "gm_KillSpieler: Killing Player <id = %i> permanently", id);
    }
}
/* 0x503e80, resets the character state: 0x503c60 (i, 2) for all 0x300 characters, clears the 0x2b-int records from 0x7d5f80 to 0x7d6a40 (first word -1) and the three counters 0x6f1bc0 / 0x6f1bb8 / 0x6f1bbc */


void gm_ResetCharacters(void)
{
    int i;
    int *p, *p2, *q;

    i = 0;
    do {
        unk_Unk503c60(i, 2);
        i++;
    } while (i < 0x300);
    p = gm_PlayerRecords;
    do {
        p2 = p + 0x2b;
        q = p;
        for (i = 0x2b; i != 0; i--)
            *q++ = 0;
        *(short *)p = (short)0xffff;
        p = p2;
    } while ((int)p2 < (unsigned)((unsigned)gm_PlayerRecords + 0xac0));
    ai_CharCount = 0;
    ai_NextCharNumber = 0;
    ai_NextPlayerNumber = 0;
}
#define gm_JobTalents (*(JOBTALENTS (*)[0x5b])((char *)&ai_Unk61a686 + 0x2))  /* 0x61a688, the job records */

/* 0x503ee0, the talents record of the job, by value (the hidden result pointer; jobs 0x5f and above: the first record) */

JOBTALENTS gm_GetJobTalents(int job)
{
    if (job >= 0x5f)
        return gm_JobTalents[0];
    return gm_JobTalents[job];
}
/* 0x503f30, 0 when the kind number of 0x504ad0 (x) is 10 .. 14, else 2 */

int gm_Unk503f30(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k >= 10 && k <= 14)
        return 0;
    return 2;
}
/* 0x503f50, 0 for the kind numbers 1 .. 4, 7 .. 0xc and 0xf .. 0x18, else 2 */

int gm_Unk503f50(int x)
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
    default:
        return 2;
    }
}
/* 0x503fa0, the building type of the job of the character kind x (0x504ad0 maps x to the job number first; 0 for none) */

int gm_BuildingTypeOfJob(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k != 0)
        return gm_JobBuildingType(k);
    return 0;
}
/* 0x504050, the building type of the job number x (1 -> 0x12, 2 -> 0x14, 3 -> 0x15, 4 -> 0x16, 5 -> 0xe, 6 -> 8, 7 -> 7, 8 -> 9, 9 -> 5, 10 -> 0x13, 0xb -> 4, 0xc -> 0x10, 0xd -> 0x1d, else 0) */

int gm_JobBuildingType(int x)
{
    switch (x) {
    case 1:
        return 0x12;
    case 2:
        return 0x14;
    case 3:
        return 0x15;
    case 4:
        return 0x16;
    case 5:
        return 0xe;
    case 6:
        return 8;
    case 7:
        return 7;
    case 8:
        return 9;
    case 9:
        return 5;
    case 10:
        return 0x13;
    case 0xb:
        return 4;
    case 0xc:
        return 0x10;
    case 0xd:
        return 0x1d;
    default:
        return 0;
    }
}


unsigned short * ai_Unk6f1bf4 = 0;  /* 0x6f1bf4, the current record */
int ai_PermsDirty = 0;  /* 0x6f2318 */
unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */
short * gm_PlayerRecord(char *c);                     /* 0x506430 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */


/* 0x504100, the two small numbers a / b of a job number (the job tables of the characters): 2 -> 1, 0; 4 -> 1, 2; 5 -> 1, 0; 6 -> 0, 1; 7 -> 4, 1; 8 -> 0, 3; 9 -> 0, 4; 10 -> 3, 2; 11 -> 2, 3; 12 -> 3, 2; 13 -> 2, 0; others 1, 0. Returns 1 */
int gm_JobSlots(int job, int *a, int *b)
{
    switch (job) {
    default:
        *a = 1;
        *b = 0;
        return 1;
    case 2:
        *a = 1;
        *b = 0;
        return 1;
    case 4:
        *a = 1;
        *b = 2;
        return 1;
    case 5:
        *a = 1;
        *b = 0;
        return 1;
    case 6:
        *a = 0;
        *b = 1;
        return 1;
    case 7:
        *a = 4;
        *b = 1;
        return 1;
    case 8:
        *a = 0;
        *b = 3;
        return 1;
    case 9:
        *a = 0;
        *b = 4;
        return 1;
    case 0xa:
        *a = 3;
        *b = 2;
        return 1;
    case 0xb:
        *a = 2;
        *b = 3;
        return 1;
    case 0xc:
        *a = 3;
        *b = 2;
        return 1;
    case 0xd:
        *a = 2;
        *b = 0;
        return 1;
    }
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
    if (0 < n && n < 7)
        return 7;
    if (0x27 < n && n < 0x2e)
        return 7;
    if (0x22 < n && n < 0x28)
        return 2;
    if (0x2d < n && n < 0x34)
        return 3;
    if (0x39 < n && n < 0x40)
        return 4;
    if (0x3f < n && n < 0x46)
        return 5;
    if (0xc < n && n < 7)
        return 6;
    return 0;
}
/* 0x5043d0, the title number of the rank r in the job group a: the last title number of the group (1 -> 0x33, 2 -> 0x3f, 3 -> 0x45 .. 0xf -> 0x5e) minus (r - 1); 0 for group 0 or an unknown one */


int gm_TitleNumber(int a, int r)
{
    if (a != 0) {
        r = r - 1;
        switch (a) {
        case 1:
            return 0x33 - r;
        case 2:
            return 0x3f - r;
        case 3:
            return 0x45 - r;
        case 4:
            return 0x4b - r;
        case 5:
            return 0x27 - r;
        case 6:
            return 0x18 - r;
        case 7:
            return 0x12 - r;
        case 8:
            return 0x1e - r;
        case 9:
            return 0xc - r;
        case 0xa:
            return 0x39 - r;
        case 0xb:
            return 6 - r;
        case 0xc:
            return 0x2d - r;
        case 0xd:
            return 0x51 - r;
        case 0xe:
            return 0x58 - r;
        case 0xf:
            return 0x5e - r;
        }
    }
    return 0;
}
/* 0x5044b0, the job byte (+0x164) of the character i when its group (0x504ad0) is p, else the byte of the job record of the character (0x506430, +0x74 + p) or 0 */


int gm_TitleJob(int i, int p)
{
    int k, r;

    k = gm_TitleGroup(ai_Chars[i][0x164]);
    if (k == p)
        return ai_Chars[i][0x164];
    r = gm_PlayerRecord(ai_Chars[i]);
    if (r == 0)
        return 0;
    return *(unsigned char *)(r + 0x74 + p);
}
/* 0x504510, the date a character n (the player slot of 0x770b80) next gets its portion: from the character's kind (0x504ce0 must be 1 .. 5, 0x504ad0 gives a class 8 .. 12 mapped to 0 .. 4: 11 -> 2, 12 / 10 -> 3, 8 -> 0, 9 -> 4, else 1) 0x504ec0 gives a period; the date is computed from the clock 0x770ae0 (day, hour part 0x770ae4, with the fraction 6 .. 21 of the day: before 7 the hour + 6, above 21 the next day), compared with the character's own date +0xcf4 (0x4fe210), and when later it is written to out (3 words and a short, 0x4fe280 / 0x4fe140 add the fraction in ticks of 0x18 per day). Returns the fraction as a float (0.0 when the character has no portion kind, 1.0 when the date is not later). ROUGH: all float operands are lost in the pseudocode (the __ftol calls) */

float gm_Unk504510(int n, int *out)
{
    int k, cls, period;
    int d[4], e[4];
    unsigned short hour;
    unsigned t;
    float f;
    int c;

    k = gm_TitlesLeft(ai_Chars[n][0x164]);
    if (k == 0 || k > 5)
        return 0.0f;
    cls = gm_TitleGroup(ai_Chars[n][0x164]);
    if (cls == 0xb)
        period = 2;
    else if (cls == 0xc)
        period = 3;
    else if (cls == 10)
        period = 3;
    else if (cls == 8)
        period = 0;
    else if (cls == 9)
        period = 4;
    else
        period = 1;
    f = gm_GetSkill(ai_Chars[n], period);
    (void)(int)f;                                /* lost */
    memset(d, 0, sizeof(d));
    memset(e, 0, sizeof(e));
    d[0] = (int)f;                               /* lost */
    hour = (unsigned short)(int)f;               /* lost */
    e[0] = hour;
    if ((float)hour <= 21.0f) {
        if ((float)hour < 7.0f)
            e[0] = hour + 6;
    } else {
        d[0]++;
        e[0] = (short)(int)f;                    /* lost */
    }
    t = *(unsigned *)(ai_Time + 4);
    c = *(int *)(ai_Chars[n] + 0x174);
    e[1] = (short)t;
    e[3] = c;
    if (gm_CompareTime(e + 3, d) < 0) {
        gm_MinutesBetween(e + 0, e + 3);
        gm_MinutesBetween(e + 0, d);
        gm_AddTime(d, (t & 0xffff) + *(int *)ai_Time * 0x18, 0, 0);
        f = (float)((int (*)(struct GCLOCK *, int, int, int))gm_AddTime)(d, c * -0x18 - (t & 0xffff), 0, 0);
        out[0] = d[0];
        out[1] = e[0];
        out[2] = e[2];
        *(short *)(out + 3) = (short)e[3];
        return f;
    }
    return 1.0f;
}
/* 0x504790, sets the job byte (+0x164) of the character i to n: a changed job clears the int +0x174; for a player or priest (kinds 5 / 6 / 7) the old job is stored in the job record (0x506430, +0x74 + the group of the old job) and the new one when it is lower or the slot empty (flag 0x6f2318) */


void gm_SetJob(int i, unsigned n)
{
    char *c;
    int r, g;
    unsigned char b;
    char k;

    c = ai_Chars[i];
    if (n != (unsigned char)c[0x164])
        *(int *)(c + 0x174) = 0;
    r = gm_PlayerRecord(c);
    if (r != 0 && ((k = c[2], k == 6 || k == 7) || k == 5)) {
        g = gm_TitleGroup((unsigned char)c[0x164]);
        *(char *)(g + 0x74 + r) = c[0x164];
        g = gm_TitleGroup(n);
        b = *(unsigned char *)(g + 0x74 + r);
        if ((int)n < (int)b || b == 0)
            *(char *)(g + 0x74 + r) = (char)n;
        ai_PermsDirty = 1;
    }
    c[0x164] = (char)n;
}
/* 0x504840, drops the character c one title rank (+0x165 - 1; 0xc for a master of kind 0x20, 0xb for kind 0x1f) and clears its int +0x174, both sent through 0x4c3d90. Returns -1 */

int gm_DropRank(char *c)
{
    char *m;
    int v;

    m = *(char **)(c + 0x16c);
    if (m != 0) {
        if (*m == ' ') {
            v = 0xc;
            goto send;
        }
        if (m != 0 && *m == 0x1f) {
            v = 0xb;
            goto send;
        }
    }
    v = (unsigned char)c[0x165] - 1;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 4));
    cm_AddFieldValue(1, 1, 0x165, &v);
    cm_SendFieldValues();
    v = 0;
    cm_BeginFieldBlock(c, *(int *)(c + 4));
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
/* 0x5049e0, the seat object of the building class: class 5 by the byte +9 of the selected character (0x1e .. 0x21 -> the buildings 2 .. 5, the object 0x142); class 3 the object 0x115 of the building 1. 0 else */

int gm_Unk5049e0(char *b)
{
    int cls, p;
    char k;

    cls = gm_BuildingClass((unsigned char)*b);
    if (cls == 5) {
        k = ai_Chars[ai_PlayerId][9];
        if (k == 0x1e)
            p = 2;
        else if (k == 0x1f)
            p = 3;
        else if (k == ' ')
            p = 4;
        else {
            if (k != '!')
                return 0;
            p = 5;
        }
        cls = (int)gm_FirstBuilding(p);
        if (cls != 0)
            return (int)gm_Query(*(int *)(cls + 0x5d), 2, 6, 0, 0x142);
    } else if (cls == 3) {
        cls = (int)gm_FirstBuilding(1);
        return (int)gm_Query(*(int *)(cls + 0x5d), 2, 6, 0, 0x115);
    }
    return 0;
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
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
        return 0xe;
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
        return 0xf;
    default:
        return 0;
    }
}
/* 0x504c00, the job group of a job-pair number n (two numbers per group: 1 .. 2 -> 0xb, 3 .. 4 -> 7, ..) */


int gm_TitleJobGroup(int n)
{
    switch (n) {
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
    default:
        return 0;
    }
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
float gm_AddHealth(unsigned id, int d);                   /* 0x4fca40 */
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
    he_Unk5538b0(ca);
    he_Unk5538b0(cb);
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
    unk_Unk503c60(sa, 0);
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

int gm_Unk5075c0(char *c)
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
unsigned short gm_Unk507690(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8)
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
            w = gm_Unk503f30(a6);
        } else
            w = gm_Unk503f50(a7);
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
        f += (unsigned char)(ai_BTypes + ai_Unk919f80[n] * 0x24d)[0x233 + i] * 896.0f * 0.0013888888888888887;
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
        if (ai_Unk919f80[item] == 0)
            f = 4.0f;
        else {
            bp = (unsigned char *)(ai_BTypes + ai_Unk919f80[item] * 0x24d);
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

unsigned short gm_Unk50a0b0(unsigned short *o)
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
unsigned short ai_Unk76fdc4 = 0;  /* 0x76fdc4 */
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
int ai_Unk7e3f38 = 0;  /* 0x7e3f38, the depth of the child stack */
int gm_NOffers = 0;  /* 0x7e3f3c, the number of offers */
AISTORE gm_Offers[88] = {0};  /* 0x7e4b20, the offers (ai.h's AISTORE rows of 6 ints: building, market, goods object, supply, price float, flag) */
int ai_Unk875360[32] = {0};  /* 0x875360 */
int ai_Unk8753e0[20488] = {0};  /* 0x8753e0, the child stack (the first word of */
unsigned ai_Unk889400[732] = {0};  /* 0x889400 */
int ai_Unk889f70 = 0;  /* 0x889f70 */
char * ai_Blds = 0;  /* 0x889f74, the buildings ( */
int ai_Unk919f80[732] = {0};  /* 0x919f80 */
char * ai_Goods = 0;  /* 0x91aaf0, the object prototypes ( */
unsigned ai_Unk91ac40[136] = {0};  /* 0x91ac40 */
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
