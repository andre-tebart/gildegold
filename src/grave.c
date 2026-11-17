/* grave.c -- the graveyard (0x4ef3c0..0x4f2720): grave counts and entries, the graveyard record
   (GVYARD, "gv_AllocGraveyardField"), the graveyard orders, grave moves and raisings, the seances (see gv.c's lead
   comment for the records). Names are ours; the file name is a guess.
   Start: gv.c's cannon / effect part ends with its .rdata (ai_CannonBalls 0x5edfe0, then the literals first used
   by 0x4edf80 .. 0x4ef260); this unit's named constants gv_SaleShares 0x5ee034.. follow; in .data its allocation
   tags 0x61120c..0x611251 follow gv.c's "kugel" 0x611204. End: gs.c's .data block gv_Decisions 0x611258 and its
   named .rdata gv_RankWorth 0x5ee0f8 follow this unit's strings and literals.
   unit split from gv (dcp unit split); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "grave.h"
#include "am.h"                       /* the decision callbacks ai_Decide* of gv_Decisions (AIDEC.fn) */
#include "d3_types.h"                 /* D3COLOR, for d3_tools.h */
#include "d3_tools.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define gv_PlayerAiChar (*(AICHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) as the AICHAR it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define gv_CrimesT (*(CRIME (*)[0x200])&ai_Crimes)  /* 0x91f700, ai_Crimes (cm.c: char[0x5a00]) as its 0x200 CRIME records */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define GAPDATA __declspec(align(1)) unsigned char
#define gv_EffectsT (*(GVEFFECT (*)[8])&gv_Effects)  /* 0x6f1008, gv_Effects (char[0xb80]) as the 8 GVEFFECT slots */
#define gv_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
typedef struct GVPLAYERREC {           /* 0x2f4 bytes, a player's slot record (ai_PlayerSlots, indexed by ai_OwnPlayer) */
    char pad00[0x54];
    unsigned short currency;            /* 0x54, the currency good, as gm_AddNegative / gm_AddObjekt take it */
    char pad56[0x29e];
} GVPLAYERREC;
#define gv_PlayerRecs (*(GVPLAYERREC (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (char[4][0x2f4]) as the player records */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_91f6e3[];
extern GAPDATA full_data_91f6e4[];
#include "amt_types.h"
#include "ls.h"
#include "ahm.h"
#include "ai.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "d3_snd.h"
#include "f3gm.h"
#include "he.h"
#include "hst.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "sv.h"
#include "tr.h"
#include "mission.h"
#include "lc.h"
#include "vfs.h"
#include "d3_interface.h"
#include "d3_pose.h"
#include "d3_engine.h"
#include "menu.h"
#include "net.h"
extern int gm_PlayerRecords[688];

extern const float gv_SaleShares[];
extern const float gv_GraveWear[];

#define gv_AvatarsT (*(GVAVATAR (*)[0x20])&unk_Avatars)  /* 0x60f310, unk_Avatars (char[0x20][0xf4]) as the GVAVATAR records they are */
#define gv_FindEntryCb(cb) ((int (*)(void *, void *))(cb))  /* @original: gv_FindEntry's callback is int (*)() (gv.h, its callbacks take differing pointer types); the call pushes two pointers (0x4ef8df push esi, 0x4ef8e0 push ebx) and reads an int (0x4ef8e6 test eax) */

/* dcp unit split: the declarations of gv before the moved functions */
typedef struct GVSHOTMSG {              /* 0x80 bytes, the data of message 0x67 (cm_SendShots): a cannon ball fired at a building; the receiver (0x4eee00) starts it as a part of the effect slot */
    int id;                             /* 0x00, the building's id, the effect slot's id */
    float v;                            /* 0x04, taken off the slot's fade value of the mode (only the first ball of a salvo carries it, then 0) */
    int mode;                           /* 0x08, 0 one ball, 1 two, 2 three */
    union {
        SHOTREC shot;                   /* 0x0c, the ball as sent (gv_FireShots) */
        GVEFFPART part;                 /* the same record as the slot's part it becomes (0x4eee00) */
    };
    int pad74[3];
} GVSHOTMSG;

/* 0x4ef3c0, the number of small (0x44-byte) graves of the graveyard building o by its type's level (AIBTYPE +0x247) and the
   sizes in use (GVYARD.open): level 1: 8, 16 with open[1]; level 2: 12 / 24 with open[1], + 8 with open[2]; else 16 / 32 with
   open[1], + 16 with open[2], + 8 with open[3] */

int gv_GraveCount(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    char *bt;
    GVYARD *g;
    int n;

    bt = ai_BTypes + bld->type + bld->type * 0x24c;
    g = bld->yard;
    if (((AIBTYPE *)bt)->level == 1) {
        n = 8;
        if (g->open[1] == 1)
            return 0x10;
    } else if (((AIBTYPE *)bt)->level == 2) {
        n = 0xc;
        if (g->open[1] == 1)
            n = 0x18;
        if (g->open[2] == 1)
            n = n + 8;
    } else {
        n = 0x10;
        if (g->open[1] == 1)
            n = 0x20;
        if (g->open[2] == 1)
            n = n + 0x10;
        if (g->open[3] == 1)
            n = n + 8;
    }
    return n;
}
/* 0x4ef440, how many of the graves of gv_GraveCount are in use (GVYARD.graves, GVGRAVE.id not -1) */

int gv_GravesInUse(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    GVGRAVE *g;
    int n, cnt;

    n = gv_GraveCount(o);
    cnt = 0;
    if (n == 0)
        return 0;
    if (n > 0) {
        g = bld->yard->graves;
        do {
            if (g->id != -1)
                cnt++;
            g++;
            n--;
        } while (n != 0);
    }
    return cnt;
}
/* 0x4ef470, the number of grave entries (GVYARD.entries, 0x14 bytes each) the graveyard building o has in use, as gv_GraveCount
   by its type's level and GVYARD.open: level 1: 8, 12 with open[1]; level 2: 12 / 18 with open[1], + 2 with open[2]; else
   16 / 24 with open[1], + 4 with open[2], + 2 with open[3] (each open size adds its entries, gv_AllocGraveyard) */

int gv_EntryCount(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    char *bt;
    GVYARD *g;
    int n;

    bt = ai_BTypes + bld->type + bld->type * 0x24c;
    g = bld->yard;
    if (((AIBTYPE *)bt)->level == 1) {
        n = 8;
        if (g->open[1] == 1)
            return 0xc;
    } else if (((AIBTYPE *)bt)->level == 2) {
        n = 0xc;
        if (g->open[1] == 1)
            n = 0x12;
        if (g->open[2] == 1)
            n = n + 2;
    } else {
        n = 0x10;
        if (g->open[1] == 1)
            n = 0x18;
        if (g->open[2] == 1)
            n = n + 4;
        if (g->open[3] == 1)
            n = n + 2;
    }
    return n;
}
/* 0x4ef4f0, 1 unless the part i (0 .. 3) of the record r is missing (the pointer at +8 + 4 * i is 0 or the flag byte at +0x18 + i is 0) */

int gv_SizeOpen(char *r, int i)
{
    GVYARD *g;

    g = (GVYARD *)r;
    if (i == 0) {
        if (g->lists[0] == 0 || g->open[0] == 0)
            return 0;
    } else if (i == 1) {
        if (g->lists[1] == 0 || g->open[1] == 0)
            return 0;
    } else if (i == 2) {
        if (g->lists[2] == 0 || g->open[2] == 0)
            return 0;
    } else if (i == 3 && (g->lists[3] == 0 || g->open[3] == 0))
        return 0;
    return 1;
}
/* 0x4ef570, the entry n (0x14 bytes) of the list i (0 .. 3, the grave size) of the graveyard record r, when that size is open
   (gv_SizeOpen) and n is below the size's entry count for the record's kind (list 0: 8 / 12 / 16, list 1: 4 / 6 / 8,
   list 2: - / 2 / 4, list 3: - / - / 2 for kind 1 / 2 / 3); 0 when r->kind is not 1 .. 3 or n is out of range */

int gv_ListEntry(char *r, int i, unsigned char n)
{
    GVYARD *g = (GVYARD *)r;
    int kind;

    kind = g->kind;
    if (kind < 1 || kind > 3)
        return 0;
    if (i == 0) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1 && n >= 8)
            return 0;
        if (kind == 2 && n >= 0xc)
            return 0;
        if (kind == 3 && n >= 0x10)
            return 0;
        return (int)&g->lists[0][n];
    }
    if (i == 1) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1 && n >= 4)
            return 0;
        if (kind == 2 && n >= 6)
            return 0;
        if (kind == 3 && n >= 8)
            return 0;
        return (int)&g->lists[1][n];
    }
    if (i == 2) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1)
            return 0;
        if (kind == 2 && n >= 2)
            return 0;
        if (kind == 3 && n >= 4)
            return 0;
        return (int)&g->lists[2][n];
    }
    if (i == 3) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1)
            return 0;
        if (kind == 2)
            return 0;
        if (kind == 3 && n >= 2)
            return 0;
        return (int)&g->lists[3][n];
    }
    return 0;
}
/* 0x4ef6b0, the index of the entry e (GVENTRY, its size the list 0 .. 3) within its list of the graveyard record r (GVYARD.lists); -1 for another list */

int gv_EntryIndex(char *r, int *e)
{
    GVYARD *g = (GVYARD *)r;
    GVENTRY *en = (GVENTRY *)e;

    if (en->size == 0)
        return en - g->lists[0];
    if (en->size == 1)
        return en - g->lists[1];
    if (en->size == 2)
        return en - g->lists[2];
    if (en->size == 3)
        return en - g->lists[3];
    return -1;
}
/* 0x4ef710, 1 when the entry b is held by the character a (GVENTRY.w08 == AICHAR.w050) and the holder is not 0x7fff */

int gv_EntryHeldBy(char *a, char *b)
{
    AICHAR *ch = (AICHAR *)a;
    GVENTRY *e = (GVENTRY *)b;
    unsigned short v;

    v = ch->w050;
    if (e->w08 == v && v != 0x7fff)
        return 1;
    return 0;
}
/* 0x4ef740, 1 when the +4 words of a and b are equal */

int gv_EntrySameId(int *a, int *b)
{
    return b[1] == a[1];
}
/* 0x4ef760, 1 when the +4 word of b is -1 */

int gv_EntryIsFree(int *a, int *b)
{
    return b[1] == -1;
}
/* 0x4ef770, 1 when the +4 word of b is not -1 */

int gv_EntryIsUsed(int *a, int *b)
{
    return b[1] != -1;
}
/* 0x4ef780, 1 when the id a[0] is in the table (+0x10, 0x44 bytes per entry, gv_EntryCounts[b[0]] of them) of the entry b */

int gv_EntryHasId(int *a, int *b)
{
    int i, n;
    int *p;

    i = 0;
    n = gv_EntryCounts[b[0]];
    if (n > 0) {
        p = (int *)b[4];
        do {
            if (*p == a[0])
                return 1;
            i++;
            p += 0x11;
        } while (i < n);
    }
    return 0;
}
/* 0x4ef7c0, the first entry (GVENTRY) of the list l (the grave size 0 .. 3; 0xff: all entries, GVYARD.entries) of the
   graveyard building o after the entry `after` (0: from the start) for which cb (a, entry) is true (no cb: any); the list
   ends after gv_ListEntries[l][level] entries (0xff: gv_EntryCount); 0 when none or the size is not open (gv_SizeOpen).
   after walks as the cursor (the original keeps the two in one register) */

unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l)
{
    AIBLD *bld = (AIBLD *)o;
    char *bt;
    GVYARD *g;
    GVENTRY *end;

    bt = ai_BTypes + bld->type + bld->type * 0x24c;
    g = bld->yard;
    if (l != 0xff && gv_SizeOpen((char *)g, l) == 0)
        return 0;
    if (l == 0)
        end = g->lists[0] + gv_ListEntries[0][((AIBTYPE *)bt)->cap247];
    else if (l == 1)
        end = g->lists[1] + gv_ListEntries[1][((AIBTYPE *)bt)->cap247];
    else if (l == 2)
        end = g->lists[2] + gv_ListEntries[2][((AIBTYPE *)bt)->cap247];
    else if (l == 3)
        end = g->lists[3] + gv_ListEntries[3][((AIBTYPE *)bt)->cap247];
    else if (l == 0xff)
        end = g->entries + gv_EntryCount(o);
    else
        return 0;
    if (after == 0) {
        if (l == 0)
            after = (unsigned)g->lists[0];
        else if (l == 1)
            after = (unsigned)g->lists[1];
        else if (l == 2)
            after = (unsigned)g->lists[2];
        else if (l == 3)
            after = (unsigned)g->lists[3];
        else
            after = (unsigned)g->entries;
    } else
        after += sizeof(GVENTRY);
    for (; after < (unsigned)end; after += sizeof(GVENTRY)) {
        if (cb == 0 || gv_FindEntryCb(cb)(a, (void *)after) != 0)
            return after;
    }
    return 0;
}
/* 0x4ef900, the first entry of the list 0xff of the object o whose +4 word is the callback's (0x4ef780) match for the entry e; 0 when e is 0 or e[0] is -1 */

unsigned gv_LoadGraveyard(unsigned char *o, int *e)
{
    if (e != 0 && e[0] != -1)
        return gv_FindEntry(e, o, 0, gv_EntryHasId, 0xff);
    return 0;
}
/* 0x4ef930, a random used grave (GVYARD.graves, first word not -1) of the graveyard building o, searched cyclically from a
   random start among its gv_GraveCount small graves; 0 when none or o is no graveyard (kind 0x1d) of level 1 .. 3 */

GVGRAVE *gv_RandomGrave(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *g;
    int n, k, i;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    g = bld->yard;
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        n = gv_GraveCount(o);
        i = util_RandomU(n);
        k = n;
        if (n != 0) {
            do {
                if (g->graves[i].id != -1)
                    return &g->graves[i];
                k--;
                i = (i + 1) % n;
            } while (k != 0);
        }
    }
    return 0;
}
/* 0x4ef9c0, buries up to n new dead persons in the free entries of the smallest graves (list 0, gv_EntryIsFree) of the
   graveyard building o (kind 0x1d, level 1 .. 3): each grave gets a new object id, random traits, values and year, a random
   family and first name by sex and an icon from its gm_NameRecord; returns how many were buried (0 for no graveyard)
   @deep Read in full against the original (all 185 instructions reconciled): statements, field stores, the
   traits fpu loop, the icon do-while and the name strncpys match instruction for instruction. Tried: early
   return 0 first, the whole body inside the kind/level if (kept), i = 0 before the if (worse, fpu hunks).
   Remaining only the callee-saved register pushes/pops: the original pushes ebx/esi/edi in the prologue
   (+0x0b..+0x10) and pops them on the return-0 path (+0x241: pop edi; pop esi; xor eax,eax; pop ebx), ours
   pushes them after the kind/level check (+0x46) and returns 0 without pops; the [esp+0x10] vs [esp+0x1c]
   load of n is the same slot shifted by those pushes. Prologue placement is allocation, not meaning. */

int gv_BuryNewDead(unsigned char *o, int n)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVENTRY *e;
    unsigned *rec;
    int i, k, r;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        for (i = 0; i < n; i++) {
            e = (GVENTRY *)gv_FindEntry(0, o, 0, gv_EntryIsFree, 0);
            if (e == 0)
                break;
            e->grave->id = ai_NextObjectId;
            ai_NextObjectId++;
            e->id = e->grave->id;
            e->w08 = 0x7fff;
            e->until = (unsigned short)(ai_Time.year + 5);
            e->b0c = 0xff;
            for (k = 0; k < 5; k++)
                e->grave->traits[k] = (unsigned char)(int)((float)util_RandomU(8) * 21.0f + 42.0f);
            e->grave->v2c = util_RandomU(9) + 1;
            e->grave->v30 = util_RandomU(2) + 1;
            e->grave->nameId = util_RandomU(0x5e) + 1;
            e->grave->year = ai_Time.year - util_RandomU(4) - 1;
            e->grave->sex = util_RandomU(2);
            strncpy(e->grave->family, txt_FamilyNames[util_RandomU(0x95)], 0xf);
            rec = gm_NameRecord(e->grave->sex, e->grave->nameId);
            e->grave->icon = 0xff;
            r = util_RandomU(4);
            k = 4;
            do {                                    /* one of the record's four words +0x34 that is set, from a random one */
                if (rec[0xd + r] != 0xff) {
                    e->grave->icon = rec[0xd + r] + 0x639;
                    break;
                }
                k--;
                r = (r + 1) % 4;
            } while (k != 0);
            if (e->grave->sex == 0)
                strncpy(e->grave->first, txt_MaleFirstNames[util_RandomU(0xbf)], 0xf);
            else
                strncpy(e->grave->first, txt_FemaleFirstNames[util_RandomU(0x70)], 0xf);
        }
        return i;
    }
    return 0;
}
/* 0x4efc10, allocates the graveyard record (GVYARD, "gv_AllocGraveyardField") of the graveyard building o (kind 0x1d, level
   1 .. 3) with its entries (gv_EntryTotals, "gv_Alloc:_all_graves") and graves (gv_GraveTotals, all free,
   "gv_Alloc:_all_zombies"); only the smallest size is open. The entries are laid out size by size, each pointing at its
   grave (0x44 << size bytes): level 1: 8 / 4, level 2: 12 / 6 / 2, level 3: 16 / 8 / 4 / 2 graves of the sizes 0 .. 3.
   Stores it as AIBLD.yard and returns it; 0 when out of memory or o is no graveyard
   @deep Read in full against the original (all 266 instructions reconciled, same count): the kind/level
   check, the three allocations with their memsets, the free marks, the lists/open setup and the per-level
   entry loops (gr advanced before e, as the original) match instruction for instruction. Remaining: (1)
   the register pair esi/ebp swapped (the original keeps g in esi and the type record in ebp; ours the
   reverse, which also makes ours 9 bytes larger through [ebp] encodings); with --regs esi=ebp,ebp=esi only
   (2) is left: the out-of-memory cleanup block (free entries, free graves, free g, return 0: 18
   instructions) that the original places inline after the graves allocation (jne to the init at +0x131)
   and ours at the end (+0x313); tried the cleanup behind a goto to the init and an entries && graves test
   before it (same or worse, VC6 lays the block out the same). Block placement and allocation only. */

GVYARD *gv_AllocGraveyard(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *g;
    GVENTRY *e;
    GVGRAVE *gr;
    int i;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        g = (GVYARD *)mem_Alloc(sizeof(GVYARD), "gv_AllocGraveyardField");
        if (g == 0)
            return 0;
        memset(g, 0, sizeof(GVYARD));
        g->id = bld->id;
        g->kind = bt->cap247;
        g->entries = (GVENTRY *)mem_Alloc(gv_EntryTotals[bt->cap247] * sizeof(GVENTRY), "gv_Alloc:_all_graves");
        if (g->entries != 0) {
            memset(g->entries, 0, gv_EntryTotals[bt->cap247] * sizeof(GVENTRY));
            g->graves = (GVGRAVE *)mem_Alloc(gv_GraveTotals[bt->cap247] * sizeof(GVGRAVE), "gv_Alloc:_all_zombies");
            if (g->graves != 0) {
                memset(g->graves, 0, gv_GraveTotals[bt->cap247] * sizeof(GVGRAVE));
                for (i = 0; i < gv_GraveTotals[bt->cap247]; i++)
                    g->graves[i].id = -1;
                e = g->entries;
                gr = g->graves;
                g->lists[0] = e;
                g->open[0] = 1;
                g->open[1] = 0;
                g->open[2] = 0;
                g->open[3] = 0;
                if (g->kind == 1) {
                    g->lists[1] = e + 8;
                    g->lists[2] = 0;
                    g->lists[3] = 0;
                    for (i = 0; i < 8; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 4; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                } else if (g->kind == 2) {
                    g->lists[1] = e + 12;
                    g->lists[2] = e + 18;
                    g->lists[3] = 0;
                    for (i = 0; i < 12; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 6; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 2; i++, gr += 4, e++) {
                        e->size = 2; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                } else {
                    g->lists[1] = e + 16;
                    g->lists[2] = e + 24;
                    g->lists[3] = e + 28;
                    for (i = 0; i < 16; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 8; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 4; i++, gr += 4, e++) {
                        e->size = 2; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 2; i++, gr += 8, e++) {
                        e->size = 3; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                }
                bld->yard = g;
                return g;
            }
        }
        if (g->entries != 0) {
            mem_Free(g->entries);
            g->entries = 0;
        }
        if (g->graves != 0) {
            mem_Free(g->graves);
            g->graves = 0;
        }
        mem_Free(g);
    }
    return 0;
}
/* 0x4eff50, moves the graveyard building o to the record of its new level (the type's level one above GVYARD.kind, which
   is below 3): allocates the larger record (gv_AllocGraveyard), copies the graves of the old sizes and the entries' state
   (all but their grave pointers) over, takes the open flags and frees the old record (gv_FreeGraveyard); returns the new
   record, or 0 (the old one stays) when o does not qualify or there is no memory
   @deep this pass: the counts as one int n[4] (the original's 0x10-byte frame with n[2] at +8; VC6 keeps
   n[0], n[1] in registers) and set in index order n[0], n[1], n[2] before the memcpys (60 -> 8 ins, shape).
   Left (sib 26): the four disp stores of the first copy loop address [base + esi + d] in the original ('+ mov
   dword ptr [edx+esi+4],ecx'), ours [esi + base + d]; the loops over lists[1] / lists[2] agree. Tried for
   loop 0 only: (p + i)->f, (*nw->lists)[i], &p[i], i[p], *(p + i), a (GVENTRY *) cast, char * arithmetic, on
   nw, old or both; old->lists[0] without != 0, n[0] > i, ++i, a block-local bound: all = control; n[3] as the
   index (45 ins), unsigned i (14 ins); every declaration order (= control). (tiebreak: insensitive) prior:
   Read in full against the original (all 188 instructions reconciled): the qualification test, the alloc /
   restore, the per-level counts and grave memcpys (rep movsd 0x88 / 0xcc / 0xcc / 0x88), the three
   field-by-field entry copy loops, the open flags and the free match statement for statement. Tried separate
   loop indices per list (no change) and s/d grave pointers set before the level test (worse: 146 code).
   Frame, allocation and scheduling only. */

GVYARD *gv_GrowGraveyard(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *old, *nw;
    int n[4], i;                            /* the entry count of each size (as lists[4]; [3] unused) */

    old = bld->yard;
    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && old != 0 && old->kind < 3 && bt->cap247 - old->kind == 1) {
        bld->yard = 0;
        nw = gv_AllocGraveyard(o);
        if (nw == 0) {
            bld->yard = old;
            return 0;
        }
        if (bt->cap247 == 2) {                  /* from level 1: 8 graves of size 0, 4 of size 1 */
            n[0] = 8;
            n[1] = 4;
            n[2] = 0;
            memcpy(nw->lists[0]->grave, old->lists[0]->grave, 8 * sizeof(GVGRAVE));
            memcpy(nw->lists[1]->grave, old->lists[1]->grave, 4 * 2 * sizeof(GVGRAVE));
        } else {                                /* from level 2: 12 / 6 / 2 graves of the sizes 0 .. 2 */
            n[0] = 12;
            n[1] = 6;
            n[2] = 2;
            memcpy(nw->lists[0]->grave, old->lists[0]->grave, 12 * sizeof(GVGRAVE));
            memcpy(nw->lists[1]->grave, old->lists[1]->grave, 6 * 2 * sizeof(GVGRAVE));
            memcpy(nw->lists[2]->grave, old->lists[2]->grave, 2 * 4 * sizeof(GVGRAVE));
        }
        if (old->lists[0] != 0) {
            for (i = 0; i < n[0]; i++) {
                nw->lists[0][i].size = old->lists[0][i].size;
                nw->lists[0][i].id = old->lists[0][i].id;
                nw->lists[0][i].w08 = old->lists[0][i].w08;
                nw->lists[0][i].until = old->lists[0][i].until;
                nw->lists[0][i].b0c = old->lists[0][i].b0c;
            }
        }
        if (old->lists[1] != 0) {
            for (i = 0; i < n[1]; i++) {
                nw->lists[1][i].size = old->lists[1][i].size;
                nw->lists[1][i].id = old->lists[1][i].id;
                nw->lists[1][i].w08 = old->lists[1][i].w08;
                nw->lists[1][i].until = old->lists[1][i].until;
                nw->lists[1][i].b0c = old->lists[1][i].b0c;
            }
        }
        if (old->lists[2] != 0) {
            for (i = 0; i < n[2]; i++) {
                nw->lists[2][i].size = old->lists[2][i].size;
                nw->lists[2][i].id = old->lists[2][i].id;
                nw->lists[2][i].w08 = old->lists[2][i].w08;
                nw->lists[2][i].until = old->lists[2][i].until;
                nw->lists[2][i].b0c = old->lists[2][i].b0c;
            }
        }
        nw->open[0] = old->open[0];
        nw->open[1] = old->open[1];
        nw->open[2] = old->open[2];
        nw->open[3] = old->open[3];
        gv_FreeGraveyard((int *)old);
        return bld->yard;
    }
    return 0;
}
/* 0x4f0180, frees a graveyard record g: its graves (+0x1c), zombies (+0x20) and itself */

void gv_FreeGraveyard(int *g)
{
    if (g != 0) {
        if (g[7] != 0) {
            mem_Free((void *)g[7]);
            g[7] = 0;
        }
        if (g[8] != 0) {
            mem_Free((void *)g[8]);
            g[8] = 0;
        }
        mem_Free(g);
    }
}
/* 0x4f01d0, the message 0x4c60b0 type 0 (graveyard id +4 of g, object id +1 of o, value v, byte b); -1 when g or o is 0 or b is -1 */

int gv_OrderBury(int *g, char *o, int v, int b)
{
    AICHAR *ch = (AICHAR *)g;
    AIBLD *bld = (AIBLD *)o;
    GVORDER m;

    if (g != 0 && o != 0 && b != -1) {
        m.chr = ch->netId;
        m.type = 0;
        m.bld = bld->id;
        m.entry = (unsigned char)b;
        m.size = v;
        m.years = 0;
        m.price = 0;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0240, graveyard order (p: +4 the character, +8 the graveyard building, +0xc the grave size, +0x10 the entry): buries
   the dead character in the graves of that entry, in the first free grave or else the one buried longest ago (GVGRAVE.year):
   its id, names, skills, offices, sex and portrait and the burial year (ai_Time); returns 0, or a negative reason: -1 no
   building or one without owner or the player's own, -2 no graveyard, -3 no graveyard record, -4 the size is not open,
   -6 no character, -7 no such entry or its w08 is not the character's AICHAR.w050 */

int gv_BuryCharacter(int *p)
{
    AIBLD *bld;
    GVYARD *g;
    AICHAR *c;
    GVENTRY *e;
    GVGRAVE *lst, *best, *q;
    int i, list;
    char *s, *d;
    char ch;

    gm_FindById((int *)&bld, 0, 0, p[2]);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, p[3]) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, p[1]);
    if (c == 0)
        return -6;
    list = p[3];
    e = (GVENTRY *)gv_ListEntry((char *)g, list, (unsigned char)p[4]);
    if (e == 0 || e->w08 != c->w050)
        return -7;
    lst = e->grave;
    best = lst;
    q = lst;
    for (i = 0; i < gv_EntryCounts[list]; i++, q++) {
        if (best->id == -1)
            break;
        if (q->id == -1) {
            best = &lst[i];
            break;
        }
        if (best->year > q->year)
            best = q;
    }
    best->id = c->netId;
    s = c->first;
    d = best->first;
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    s = c->family;
    d = best->family;
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    memcpy(best->traits, c->skills, sizeof(best->traits));
    best->v2c = c->prevOffice;
    best->v30 = c->b00d;
    best->nameId = c->jobId;
    best->sex = c->sex;
    best->year = ai_Time.year;
    best->icon = c->picture;
    return 0;
}
/* 0x4f0430, sends graveyard order type 1 (GVORDER, cm_SendGraveOrder): reserve a grave of the size `size` of the graveyard
   building o for the living character c (c[1]: its AICHAR.netId; the entry is left to the handler, 0xff) for `years` years at
   the price `price`; returns the sender's result, -1 when c or o is 0 */

int gv_OrderReserveGrave(int *c, char *o, int size, short years, int price)
{
    GVORDER m;

    if (c != 0 && o != 0) {
        m.chr = c[1];
        m.bld = ((AIBLD *)o)->id;
        m.size = size;
        m.type = 1;
        m.entry = 0xff;
        m.years = years;
        m.price = price;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0490, graveyard order type 1 (GVORDER m, sent by gv_OrderReserveGrave): reserves the first free entry of the size
   m->size of the graveyard building m->bld for the living character m->chr: the entry gets the character's netId and w050,
   runs until ai_Time.year + m->years, its graves are freed; with a price the buyer pays it (gm_AddNegative), the owner gets
   the size's share of it (gv_SaleShares, gm_AddObjekt) and an owner that is a player character (kind 6) other than the
   buyer is told (ai_NotifySale). Returns 0, or a negative reason as gv_BuryCharacter (-5 the size is out of range, -7 no
   free entry) */

int gv_ReserveGrave(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c, *owner;
    GVENTRY *e;
    int i;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    if (gv_SizeOpen((char *)bld->yard, m->size) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    if (m->size >= 0 && m->size <= 3) {
        e = (GVENTRY *)gv_FindEntry(c, (unsigned char *)bld, 0, gv_EntryIsFree, m->size);
        if (e == 0)
            return -7;
        e->id = c->netId;
        e->w08 = c->w050;
        e->until = m->years + (unsigned short)ai_Time.year;
        e->b0c = 0xff;
        for (i = 0; i < gv_EntryCounts[m->size]; i++)
            e->grave[i].id = -1;
        if (m->price > 0) {
            gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
            gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                         (int)(m->price * gv_SaleShares[m->size]));
            owner = &gv_CharsT[bld->owner];
            if (c != owner && gv_CharsT[bld->owner].kind == 6)
                ai_NotifySale((int)owner, (unsigned short *)c, (int)bld, m->size, m->price,
                              (int)(m->price * gv_SaleShares[m->size]));
        }
        return 0;
    }
    return -5;
}
/* 0x4f0700, the message 0x4c60b0 type 2 (graveyard id +4 of g, object id +1 of o, value v, byte b, short s, int x); -1 when g or o is 0 or b is -1 */

int gv_OrderExtendGrave(int *g, char *o, int v, int b, short s, int x)
{
    AICHAR *ch = (AICHAR *)g;
    AIBLD *bld = (AIBLD *)o;
    GVORDER m;

    if (g != 0 && o != 0 && b != -1) {
        m.chr = ch->netId;
        m.bld = bld->id;
        m.size = v;
        m.entry = (unsigned char)b;
        m.type = 2;
        m.years = s;
        m.price = x;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0770, graveyard order type 2 (GVORDER m): extends the lease of the entry m->entry of the size m->size of the graveyard
   building m->bld by m->years (GVENTRY.until); with a price the character m->chr pays it (gm_AddNegative) and the owner
   gets the size's share of it (gv_SaleShares, gm_AddObjekt). Returns 0, or a negative reason as gv_BuryCharacter (-7 no
   such entry) */

int gv_ExtendGrave(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;
    GVENTRY *e;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, m->size) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    e = (GVENTRY *)gv_ListEntry((char *)g, m->size, m->entry);
    if (e == 0)
        return -7;
    e->until += m->years;
    if (m->price > 0) {
        gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
        gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                     (int)(m->price * gv_SaleShares[m->size]));
    }
    return 0;
}
/* 0x4f0900, sends graveyard order type 3 (GVORDER, cm_SendGraveOrder): sets the holder of the entry `entry` of the size
   `size` of the graveyard building o to the character c (c[1]: its AICHAR.netId), or frees the entry when c is 0 (netId
   -1); returns the sender's result, -1 when o is 0 or entry is -1 */

int gv_OrderSetGraveHolder(int *c, char *o, int size, int entry)
{
    GVORDER m;
    int chr;

    if (o != 0 && entry != -1) {
        chr = c == 0 ? -1 : c[1];
        m.chr = chr;
        m.bld = ((AIBLD *)o)->id;
        m.type = 3;
        m.size = size;
        m.entry = (unsigned char)entry;
        m.years = 0;
        m.price = 0;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0970, graveyard order type 3 (GVORDER m, sent by gv_OrderSetGraveHolder): makes the character m->chr the holder of
   the entry m->entry of the size m->size of the graveyard building m->bld (GVENTRY.id its netId, GVENTRY.w08 its
   AICHAR.w050), or frees the entry when m->chr is -1 (-1 / 0xffff). Returns 0, or a negative reason as gv_BuryCharacter
   (-6 no such character, -7 no such entry) */

int gv_SetGraveHolder(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;
    GVENTRY *e;
    int id;
    unsigned short key;

    id = -1;
    key = 0xffff;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, m->size) == 0)
        return -4;
    if (m->chr != -1) {
        gm_FindById(0, 0, (int *)&c, m->chr);
        if (c == 0)
            return -6;
        id = m->chr;
        key = c->w050;
    }
    e = (GVENTRY *)gv_ListEntry((char *)g, m->size, m->entry);
    if (e == 0)
        return -7;
    e->w08 = key;
    e->id = id;
    return 0;
}
/* 0x4f0a90, sends graveyard order type 4 (GVORDER, cm_SendGraveOrder): opens the grave sizes up to `size` of the graveyard
   building o for the character c (c[1]: its AICHAR.netId, -1 when c is 0); returns the sender's result, -1 when o is 0 */

int gv_OrderOpenSize(int *c, char *o, int size)
{
    GVORDER m;
    int chr;

    if (o == 0)
        return -1;
    chr = c == 0 ? -1 : c[1];
    m.chr = chr;
    m.bld = ((AIBLD *)o)->id;
    m.type = 4;
    m.size = size;
    m.entry = 0;
    m.years = 0;
    m.price = 0;
    return ((int (*)(void *))cm_SendGraveOrder)(&m);
}
/* 0x4f0af0, graveyard order type 4 (GVORDER m, sent by gv_OrderOpenSize): opens the grave sizes 1 .. m->size (GVYARD.open)
   of the graveyard building m->bld, when that size's list exists and the yard's level (GVYARD.kind) reaches it. Returns 0,
   -8 for a size other than 1 .. 3, -7 when the size cannot be opened, otherwise a negative reason as gv_BuryCharacter
   @deep read whole against the original: every check, constant and branch matches (kind >= 3 / >= 2
   signed, the -7 / -8 tails, if-chain on m->size; a switch is further off). Remaining: the original
   cross-jumps size 1's 'open[1] = 1; return 0' into size 2's tail (+ 0x4f0bfe jne into 0x4f0be7, its pop
   edi after the stores at 0x4f0bea) while ours schedules pop edi before size 2's stores (- 0x4f0be4 pop
   edi, order 6) so no common tail exists and size 1 keeps its own copy (- je / mov [esi+0x19], cl / pop
   edi / xor / pop / pop / ret, code 8). Tried: goto into the shared tail (same code), switch (worse).
   Scheduling / tail merge only */

int gv_OpenSize(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    if (m->size == 3) {
        if (g->lists[3] != 0 && g->kind >= 3) {
            g->open[3] = 1;
            g->open[2] = 1;
            g->open[1] = 1;
            return 0;
        }
    } else if (m->size == 2) {
        if (g->lists[2] != 0 && g->kind >= 2) {
            g->open[2] = 1;
            g->open[1] = 1;
            return 0;
        }
    } else if (m->size == 1) {
        if (g->lists[1] != 0) {
            g->open[1] = 1;
            return 0;
        }
    } else {
        return -8;
    }
    return -7;
}
/* 0x4f0c20, the message 0x4c60b0 type 5 (graveyard id -1, object id +1 of o, value v, byte b, int x); -1 when o is 0 or b is -1 */

int gv_OrderAdjustEntry(char *o, int v, int b, int x)
{
    AIBLD *bld = (AIBLD *)o;
    GVORDER m;

    if (o != 0 && b != -1) {
        m.bld = bld->id;
        m.entry = (unsigned char)b;
        m.type = 5;
        m.chr = -1;
        m.size = v;
        m.years = 0;
        m.price = x;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0c80, graveyard order type 5 (GVORDER m, sent by gv_OrderAdjustEntry): adds m->price (here a signed delta) to the
   byte GVENTRY.b0c of the entry m->entry of the size m->size of the graveyard building m->bld, clamped to 0 .. 0xff.
   Returns 0, or a negative reason as gv_BuryCharacter (-7 no such entry) */

int gv_AdjustEntry(GVORDER *m)
{
    AIBLD *bld;
    GVENTRY *e;
    int size;
    int v;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    size = m->size;
    if (gv_SizeOpen((char *)bld->yard, size) == 0)
        return -4;
    e = (GVENTRY *)gv_ListEntry((char *)bld->yard, size, m->entry);
    if (e == 0)
        return -7;
    v = e->b0c + m->price;
    v = v <= 0 ? 0 : v;
    if (v >= 0xff)
        v = 0xff;
    e->b0c = (unsigned char)v;
    return 0;
}
/* 0x4f0d70, the message 0x4c60b0 type 6 (graveyard id -1, object id +1 of o, value x at +0x13); -1 when o is 0 */

int gv_OrderNewDead(char *o, int x)
{
    AIBLD *bld = (AIBLD *)o;
    GVORDER m;

    if (o == 0)
        return -1;
    m.size = 0;
    m.entry = 0;
    m.years = 0;
    m.type = 6;
    m.chr = -1;
    m.bld = bld->id;
    m.price = x;
    return ((int (*)(void *))cm_SendGraveOrder)(&m);
}
/* 0x4f0dd0, graveyard order type 6 (p: +8 the object id, +0x13 the number): makes that many new dead (0x4ef9c0); 0 when some were made, else -4 */

int gv_AddNewDead(unsigned char *p)
{
    GVORDER *m = (GVORDER *)p;
    AIBLD *bld;
    int n;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || ai_Chars[bld->owner] == ai_PlayerCharC)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    n = gv_BuryNewDead((unsigned char *)bld, m->price);
    return (n > 0) - 1 & 0xfffffffc;
}
/* 0x4f0e80, sends a grave move (GVMOVE, message 0x61, cm_SendGraveMove): the character c (c[1]: its AICHAR.netId) moves the
   entry `entry` of the size `from` of the graveyard building o to a free entry of the size `to` for the price `price`; returns the message's slot handle (cm_GetSlotState), -1 when it was not sent */

int gv_OrderMoveGrave(int *c, int to, char *o, int from, int entry, int price)
{
    GVMOVE m;

    m.chr = c[1];
    m.bld = ((AIBLD *)o)->id;
    m.from = from;
    m.entry = (unsigned char)entry;
    m.to = to;
    m.price = price;
    return cm_SendGraveMove(&m);
}
/* 0x4f0ed0, runs a grave move (GVMOVE m, message 0x61, sent by gv_OrderMoveGrave): the entry m->entry of the size m->from
   of the graveyard building m->bld, held by the character m->chr (GVENTRY.w08 == AICHAR.w050), moves to the first free
   entry of the size m->to (gv_FindEntry, gv_EntryIsFree): its dead (GVGRAVE records in use) are packed into the free
   records of the new entry and cleared at the old one; the holder goes over, the new lease runs to ai_Time.year + 10.
   With a price the character pays it and the owner gets the target size's share (gv_SaleShares). Returns 0, or a
   negative reason as gv_BuryCharacter (-6 no such character, -7 no such held entry or no free one) */

int gv_MoveGrave(GVMOVE *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVENTRY *src;
    GVENTRY *dst;
    GVGRAVE *s;
    GVGRAVE *send;
    GVGRAVE *d;
    GVGRAVE *dend;
    int from;
    int n;

    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    from = m->from;
    if (gv_SizeOpen((char *)bld->yard, from) == 0)
        return -4;
    src = (GVENTRY *)gv_ListEntry((char *)bld->yard, from, m->entry);
    if (src == 0 || src->w08 != c->w050)
        return -7;
    s = src->grave;
    n = gv_EntryCounts[from];
    dst = (GVENTRY *)gv_FindEntry(c, (unsigned char *)bld, 0, gv_EntryIsFree, m->to);
    if (dst == 0)
        return -7;
    d = dst->grave;
    dend = d + gv_EntryCounts[m->to];
    send = s + n;
    for (; d < dend; d++) {
        if (d->id == -1) {
            for (; s < send; s++) {
                if (s->id != -1) {
                    *d = *s;
                    memset(s, 0, sizeof(GVGRAVE));
                    s->id = -1;
                    break;
                }
            }
            if (s >= send)
                break;
        }
    }
moved:
    dst->id = src->id;
    src->id = -1;
    dst->w08 = src->w08;
    src->w08 = 0xffff;
    dst->until = (unsigned short)(ai_Time.year + 10);
    dst->b0c = 0xff;
    src->until = 0xff;
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->objId;
    if (m->price > 0) {
        gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
        gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                     (int)(m->price * gv_SaleShares[m->to]));
    }
    return 0;
}
/* 0x4f1170, sends a raising of a dead person (GVRAISE, message 0x62, cm_SendRaiseDead): the dead GVGRAVE.id `grave` of the
   entry `entry` of the size `size` of the graveyard building o, as kind `kind` (0x1b / 0x1c); first files gv_Accuse (0x1b,
   1, the owner's netId, -1, the building's id). Returns the sender's result, -1 when the building has no owner, is the
   player's own or entry is -1 */

int gv_OrderRaiseDead(char *o, int size, int entry, int grave, int kind)
{
    unsigned short owner;
    GVRAISE m;

    owner = ((AIBLD *)o)->owner;
    if (owner != 0xffff && owner != *ai_PlayerChar && entry != -1) {
        m.bld = ((AIBLD *)o)->id;
        m.size = size;
        m.entry = (unsigned char)entry;
        m.grave = grave;
        m.kind = kind;
        gv_Accuse(0x1b, 1, gv_CharsT[owner].netId, -1, m.bld);
        return ((int (*)(void *))cm_SendRaiseDead)(&m);
    }
    return -1;
}
/* 0x4f1200, runs a raising of a dead person (GVRAISE m, message 0x62, sent by gv_OrderRaiseDead): the dead GVGRAVE m->grave
   of the entry m->entry of the size m->size of the graveyard building m->bld becomes a living character of kind 0x14 in
   the first free ai_Chars slot (or one of kind 0xf, as ahm_CreateAlchemist): the building is its home, its title m->kind,
   its skills half the grave's traits, its names and sex the grave's, f014 = 1000 / (random * 4 + 8), its relations to
   all others random -0x20 .. 0x1f; it is placed at the building. The grave record is cleared; when the entry's id was
   this dead person's, it passes to the first other dead of the entry (none: the entry is freed). Returns the new
   character's id, 0xffff (no such entry or grave, no free slot) or a negative reason as gv_BuryCharacter (-6 another
   kind, -1 no owner or the player's own building, -2 not a graveyard, -3 no GVYARD, -4 the size not open) */

unsigned gv_RaiseDead(GVRAISE *m)
{
    AIBLD *bld;
    GVENTRY *e;
    GVGRAVE *q, *z;
    AICHAR *c;
    int size, i, j, n, slot;
    float r;

    if (m->kind != 0x1b && m->kind != 0x1c)
        return (unsigned)-6;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return (unsigned)-1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return (unsigned)-2;
    if (bld->yard == 0)
        return (unsigned)-3;
    size = m->size;
    if (gv_SizeOpen((char *)bld->yard, size) == 0)
        return (unsigned)-4;
    e = (GVENTRY *)gv_ListEntry((char *)bld->yard, size, m->entry);
    if (e == 0)
        return 0xffff;
    i = 0;
    if (gv_EntryCounts[size] > 0) {
        do {
            q = &e->grave[i];
            if (q->id == m->grave)
                break;
            i++;
        } while (i < gv_EntryCounts[size]);
        if (q->id == m->grave) {
            for (slot = 0; slot < 0x300; slot++)
                if (gv_CharsT[slot].id == 0xffff)
                    break;
            if (slot == 0x300) {
                for (slot = 0; slot < 0x300; slot++)
                    if (gv_CharsT[slot].kind == 0x0f)
                        break;
                if (slot == 0x300)
                    return 0xffff;
            }
            if (gv_CharsT[slot].kind == 0x0f)
                ai_CharCount--;
            c = &gv_CharsT[slot];
            memset(c, 0, sizeof(AICHAR));
            gv_CharsT[slot].netId = -1;
            gv_CharsT[slot].id = 0xffff;
            for (i = 0; i < 8; i++)
                gv_CharsT[slot].relatives[i] = -1;
            ai_CharCount++;
            c->netId = ai_NextObjectId;
            c->id = (unsigned short)slot;
            c->kind = 0x14;
            c->bld2 = bld;
            c->bld = bld;
            c->v1f0 = 0;
            c->partner = -1;
            c->b215 = 0;
            c->b216 = 0;
            c->b217 = 0;
            c->mask = 1;
            ai_NextObjectId++;
            c->mood3Rate = 1.0f;
            c->health = 100;
            c->w00a = 0x10;
            c->jobId = 0;
            c->officeKind = 0;
            c->title = (char)m->kind;
            c->ownName[0] = 0;
            c->picture = -1;
            c->partyId = -1;
            c->v190 = 4;
            c->money = 0;
            c->b00d = 1;
            c->w050 = 0x7fff;
            for (j = 0; j < 5; j++)
                c->skills[j] = q->traits[j] >> 1;
            c->sex = (unsigned char)q->sex;
            strcpy(c->first, q->first);
            strcpy(c->family, q->family);
            c->f010 = 0;
            r = util_RandomF();
            c->f018 = 0;
            c->f01c = 1000.0f;
            c->f020 = 0;
            c->v024 = 0;
            c->w028 = 0;
            c->v02c = 0;
            c->f014 = 1000.0f / (r * 4.0f + 8.0f);
            for (j = 0; j < 0x300; j++) {
                ai_Relations[j][c->id] = (signed char)util_RandomU(0x40) - 0x20;
                ai_RelationMemory[j][c->id] = 0;
                ai_Relations[c->id][j] = (signed char)util_RandomU(0x40) - 0x20;
                ai_RelationMemory[c->id][j] = 0;
            }
            gm_PlaceOnStage(c->netId, bld->id, -1, 0, 0);
            gm_ChooseHeadIcon(&c->id);
            if (q->id == e->id) {
                n = 0;
                if (gv_EntryCounts[m->size] > 0) {
                    z = e->grave;
                    do {
                        if (z->id != -1 && z->id != q->id) {
                            e->id = z->id;
                            goto cleared;
                        }
                        n++;
                        z++;
                    } while (n < gv_EntryCounts[m->size]);
                }
                e->id = -1;
                e->w08 = 0xffff;
            }
        cleared:
            memset(q, 0, sizeof(GVGRAVE));
            q->id = -1;
            return c->id;
        }
    }
    return 0xffff;
}
/* 0x4f15e0, takes a random grave (0x4ef930) of the object o and its list entry (0x4ef900) and sends it with 0x4f1170 (type 0x1c); -1 when 0x4f1d50 (o, 0) is not 1 or nothing is found */

int gv_SendGrave(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    GVGRAVE *z;
    GVENTRY *e;
    int k;

    if (gv_RoomForRaised(o, 0) != 1)
        return -1;
    z = gv_RandomGrave(o);
    if (z == 0)
        return -1;
    e = (GVENTRY *)gv_LoadGraveyard(o, (int *)z);
    if (e == 0)
        return -1;
    k = gv_EntryIndex((char *)bld->yard, (int *)e);
    if (k == -1)
        return -1;
    return gv_OrderRaiseDead((char *)o, e->size, k, z->id, 0x1c);
}
/* 0x4f1660, the daily decay of a raised dead person c (kind 0x14, gv_RaiseDead; the daily character step of f3gm.c
   0x4fbe90 removes it when this returns 1): its life value f01c loses f014 less 50 * gv_PreserveRate of its graveyard
   (cm_SendFloatDelta), all of it (-1000) when the graveyard is gone, has no owner or is the player's own. Returns 1
   when the life value is used up, 0 while some is left and for other or dead characters */

int gv_DecayRaisedDead(AICHAR *c)
{
    AIBLD *bld;
    float loss, rest;

    if (c != 0 && c->kind == 0x14 && c->health != 0) {
        bld = c->bld;
        if (bld == 0 || bld->owner == 0xffff || bld->owner == *ai_PlayerChar) {
            cm_SendFloatDelta(c->netId, 0x1c, -1000.0f);
        } else {
            loss = c->f014 - gv_PreserveRate((char *)bld) * 50.0f;
            rest = c->f01c - loss;
            cm_SendFloatDelta(c->netId, 0x1c, -loss);
            if (rest <= 0.0f)
                return 1;
            return 0;
        }
        return 1;
    }
    return 0;
}
/* 0x4f16f0, a rate of the object o: the sum of 10 / 10 / 10 / 15 / 10 / 10 / 15 for the items 0x1ff, 0x202, 0x208, 0x20d, 0x214, 0x216, 0x219 in its list, times 0.0125 */

float gv_PreserveRate(char *o)
{
    AIBLD *bld = (AIBLD *)o;
    int sum;

    sum = 0;
    if (gm_Query(bld->objList, 2, 6, 0, 0x1ff) != 0)
        sum = 10;
    if (gm_Query(bld->objList, 2, 6, 0, 0x202) != 0)
        sum += 10;
    if (gm_Query(bld->objList, 2, 6, 0, 0x208) != 0)
        sum += 10;
    if (gm_Query(bld->objList, 2, 6, 0, 0x20d) != 0)
        sum += 15;
    if (gm_Query(bld->objList, 2, 6, 0, 0x214) != 0)
        sum += 10;
    if (gm_Query(bld->objList, 2, 6, 0, 0x216) != 0)
        sum += 10;
    if (gm_Query(bld->objList, 2, 6, 0, 0x219) != 0)
        sum += 15;
    return (float)sum * 0.0125f;
}
/* 0x4f17f0, writes the graveyard record of the object o to the file f: +0, +4 (4 each), the four flag bytes +0x18 .. +0x1b, the zombie records (0x44 bytes: 4, 0x10, 0x10, 5, then six ints) of the
   kind's table 0x5ee074, then the graves (+0 +4 +8 short +10 short +0xc byte, and the index of the zombie record at +0x10) of the table 0x5ee064; 1 when all was written */

int gv_WriteGraveyard(void *f, unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    GVYARD *g;
    GVGRAVE *z;
    GVENTRY *e;
    int i, n, idx;

    g = bld->yard;
    if (g == 0)
        return 0;
    if (vfs_fwrite(&g->id, 4, 1, f) == 0)
        return 0;
    if (vfs_fwrite(&g->kind, 4, 1, f) == 0)
        return 0;
    if (vfs_fwrite(&g->open[0], 1, 1, f) == 0)
        return 0;
    if (vfs_fwrite(&g->open[1], 1, 1, f) == 0)
        return 0;
    if (vfs_fwrite(&g->open[2], 1, 1, f) == 0)
        return 0;
    if (vfs_fwrite(&g->open[3], 1, 1, f) == 0)
        return 0;
    z = g->graves;
    i = 0;
    if (gv_GraveTotals[g->kind] > 0) {
        do {
            if (vfs_fwrite(&z->id, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(z->first, 0x10, 1, f) == 0)
                return 0;
            if (vfs_fwrite(z->family, 0x10, 1, f) == 0)
                return 0;
            if (vfs_fwrite(z->traits, 5, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->v2c, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->v30, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->nameId, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->sex, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->icon, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&z->year, 4, 1, f) == 0)
                return 0;
            i++;
            z++;
        } while (i < gv_GraveTotals[g->kind]);
    }
    e = g->entries;
    i = 0;
    if (gv_EntryTotals[g->kind] > 0) {
        do {
            if (vfs_fwrite(&e->size, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&e->id, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&e->w08, 2, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&e->until, 2, 1, f) == 0)
                return 0;
            if (vfs_fwrite(&e->b0c, 1, 1, f) == 0)
                return 0;
            idx = e->grave - g->graves;
            if (vfs_fwrite(&idx, 4, 1, f) == 0)
                return 0;
            i++;
            e++;
        } while (i < gv_EntryTotals[g->kind]);
    }
    return 1;
}
/* 0x4f1aa0, reads the graveyard record of 0x4f17f0 from the file f for the object o (allocated by 0x4efc10): the same fields in the same order; the graves' zombie pointers are rebuilt from the saved indexes; 1 when all was read */

int gv_ReadGraveyard(void *f, unsigned char *o)
{
    GVYARD *g;
    GVGRAVE *z;
    GVENTRY *e;
    int i, idx;

    g = gv_AllocGraveyard(o);
    if (g == 0)
        return 0;
    if (ls_Read(&g->id, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&g->kind, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&g->open[0], 1, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&g->open[1], 1, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&g->open[2], 1, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&g->open[3], 1, 1, (int)f) == 0)
        return 0;
    z = g->graves;
    i = 0;
    if (gv_GraveTotals[g->kind] > 0) {
        do {
            if (ls_Read(&z->id, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(z->first, 0x10, 1, (int)f) == 0)
                return 0;
            if (ls_Read(z->family, 0x10, 1, (int)f) == 0)
                return 0;
            if (ls_Read(z->traits, 5, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->v2c, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->v30, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->nameId, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->sex, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->icon, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&z->year, 4, 1, (int)f) == 0)
                return 0;
            i++;
            z++;
        } while (i < gv_GraveTotals[g->kind]);
    }
    i = 0;
    e = g->entries;
    if (gv_EntryTotals[g->kind] > 0) {
        do {
            if (ls_Read(&e->size, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&e->id, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&e->w08, 2, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&e->until, 2, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&e->b0c, 1, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&idx, 4, 1, (int)f) == 0)
                return 0;
            e->grave = g->graves + idx;
            e++;
            i++;
        } while (i < gv_EntryTotals[g->kind]);
    }
    return 1;
}
/* 0x4f1d50, counts the raised dead persons of the graveyard building o (kind 0x14 characters whose bld is o) into *n
   (n may be 0); returns 1 while they are fewer than the building type's places k231 + k232, 0 when it is full, -1 for
   a building that is no graveyard (kind 0x1d) or has no GVYARD */

int gv_RoomForRaised(unsigned char *o, int *n)
{
    AIBLD *bld;
    AIBTYPE *t;
    AICHAR *c;
    int places;
    int cnt;

    bld = (AIBLD *)o;
    t = (AIBTYPE *)(ai_BTypes + bld->type * 0x24d);
    if (t->kind != 0x1d)
        return -1;
    if (bld->yard == 0)
        return -1;
    places = t->k232 + t->k231;
    cnt = 0;
    for (c = gv_CharsT; (int)&c->bld < (int)&gv_CharsT[768].bld; c++) {
        if (c->id != 0xffff && c->kind == 0x14 && c->bld == bld)
            cnt++;
    }
    if (n != 0)
        *n = cnt;
    return cnt < places;
}
/* 0x4f1de0, the turn step of the graveyard building o (main.c): walks its used grave entries (gv_FindEntry with
   gv_EntryIsUsed, all sizes); after 0x40 orders it waits for the last one's answer (cm_GetSlotState). An entry whose
   holder is gone is freed (gv_OrderSetGraveHolder) when no dead lies in it, else its care b0c wears down by
   b0c / 255 * gv_GraveWear[size] * (random + 1) / 2 (gv_OrderAdjustEntry, on every other turn by the entry's parity);
   a held entry wears the same way. With an owner that is not the player: a holder other than the owner whose grave
   is in bad care (below 0.66) loses relation to the owner (cm_SendRelations -5 .. -20, by the year and the holder's
   netId); when the lease runs out (GVENTRY.until == the year) it is renewed for 10 years (gv_OrderExtendGrave): free
   for the owner, offered to a player (message 0x92, AIMSG.lease), paid by a character with enough money (0.44 of
   gm_CharCurrency above gv_LeasePrice), else the entry is freed. At the end the graveyard gets 1 .. 2 new dead while
   fewer than 4 graves are in use (gv_OrderNewDead), and a player owner is told the paid leases (ai_TellGravesPaid)
   and the angered holders (ai_TellGraveNeglect) */

void gv_GraveyardTurn(char *o)
{
    AIBLD *bld;
    AICHAR *pl;
    AICHAR *c;
    GVENTRY *e;
    GVGRAVE *g;
    GVYARD *yard;
    int h, rounds, nrel, paid, sum;
    int k, t, size, price, share;
    float f;

    bld = (AIBLD *)o;
    pl = 0;
    rounds = 0;
    nrel = 0;
    paid = 0;
    sum = 0;
    h = -1;
    if (bld->owner != 0xffff && bld->owner != *ai_PlayerChar)
        pl = &gv_CharsT[bld->owner];
    e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, 0, gv_EntryIsUsed, 0xff);
    while (e != 0) {
        if (rounds >= 0x40 && h != -1) {
            while (1) {
                if (cm_GetSlotState(h) != 0)
                    break;
                cm_FlushSendQueue();
                cm_ReceiveMessages();
                cm_ExecCommands();
            }
            rounds = 0;
            h = -1;
        }
        c = (AICHAR *)gm_FindChar(e->id);
        if (c == 0) {
            size = e->size;
            for (k = 0; k < gv_EntryCounts[size]; k++) {
                g = &e->grave[k];
                if (g->id != -1) {
                    f = (util_RandomF() + 1.0f) * 0.5f;
                    t = (int)(f * (gv_GraveWear[e->size] * ((float)e->b0c / 255.0f)));
                    if (t <= 0 || ((gv_EntryIndex((char *)bld->yard, (int *)e) ^ ai_Time.year) & 1) != 0)
                        goto next;
                    h = gv_OrderAdjustEntry(o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), -t);
                    goto counted;
                }
            }
            h = gv_OrderSetGraveHolder(0, o, size, gv_EntryIndex((char *)bld->yard, (int *)e));
        } else {
            AIMSG m;

            f = (util_RandomF() + 1.0f) * 0.5f;
            size = e->size;
            t = (int)(f * (gv_GraveWear[size] * ((float)e->b0c / 255.0f)));
            if (t > 0 && ((gv_EntryIndex((char *)(yard = bld->yard), (int *)e) ^ ai_Time.year) & 1) == 0) {
                h = gv_OrderAdjustEntry(o, size, gv_EntryIndex((char *)yard, (int *)e), -t);
                rounds++;
            }
            if (pl == 0)
                goto next;
            if (c != pl && ai_Time.year % 4 == (c->netId & 3)
                && (float)e->b0c / 255.0f < 0.66f) {
                h = cm_SendRelations(pl->netId, c->netId, -5 - util_RandomU(0x10), 1);
                rounds++;
                nrel++;
            }
            if (e->until != ai_Time.year)
                goto next;
            h = price = gv_LeasePrice(&c->id, e->b0c, e->size, 0, &share);
            if (c == pl) {
                h = gv_OrderExtendGrave((int *)c, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), 10, 0);
            } else if (c->kind == 6 || c->kind == 7) {
                memset(&m, 0, sizeof(m));
                m.to = c->netId;
                m.fromId = bld->id;
                m.type = 0x92;
                m.time = ai_Time;
                m.flags = 9;
                m.lease.bld = bld->id;
                m.lease.size = e->size;
                m.lease.entry = (unsigned char)gv_EntryIndex((char *)bld->yard, (int *)e);
                m.lease.price = price;
                m.lease.years = 10;
                h = cm_SendLongMessage((char *)&m);
            } else if ((int)(gm_CharCurrency((char *)c, ai_OwnPlayer) * 0.44f) > price) {
                h = gv_OrderExtendGrave((int *)c, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), 10, price);
                rounds++;
                paid++;
                sum += share;
                goto next;
            } else {
                h = gv_OrderSetGraveHolder(0, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e));
            }
        }
    counted:
        rounds++;
    next:
        e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, (unsigned)e, gv_EntryIsUsed, 0xff);
    }
    if (gv_GravesInUse((unsigned char *)bld) < 4)
        gv_OrderNewDead(o, util_RandomU(2) + 1);
    if (pl != 0 && (pl->kind == 6 || pl->kind == 7)) {
        if (paid > 0)
            ai_TellGravesPaid((int)pl, (int)bld, paid, sum);
        if (nrel != 0)
            ai_TellGraveNeglect((unsigned short *)pl, (int)bld);
    }
}
/* 0x4f22a0, the entry of the list 0xff of the object o (0x4ef7c0 with 0x4ef770) whose byte +0xc is below m and the smallest of them; 0 when the owner is the player or has no item 0x1f8 (list 2, key 4) or none qualifies */

GVENTRY *gv_FindWornEntry(AIBLD *bld, unsigned char m)
{
    GVENTRY *best;
    GVENTRY *e;

    best = 0;
    if (bld->owner != 0xffff && bld->owner != gv_PlayerAiChar->id
        && gm_Query(bld->objList, 2, 4, 0x1f8, 6) != 0) {
        for (e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, 0, gv_EntryIsUsed, 0xff); e != 0;
             e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, (int *)e, gv_EntryIsUsed, 0xff)) {
            if (e->b0c < m && (best == 0 || e->b0c < best->b0c))
                best = e;
        }
        return best;
    }
    return 0;
}
/* 0x4f2330, the price of a grave lease for the character c (its id) in a grave of the size kind: its worth in the player's
   money (gm_GetWorth, gm_ConvertMoney) clamped to 10000 .. 1000000, log10 of it squared * 100, times 0.25 / 0.5 / 0.75 for the
   sizes 0 .. 2; unless flag, also times (0.3 + 0.7 * care / 255) * 0.75 with care the entry's GVENTRY.b0c. Returns the price
   in the player's terms (gm_PlayerFactor) and stores the owner's share of it (gv_SaleShares[kind]) in *out */

int gv_LeasePrice(unsigned short *c, unsigned char care, int kind, int flag, int *out)
{
    double x, v;
    int r;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    x = max(min(1000000.0, x), 10000.0);
    v = log10(x);
    v = v * v * 100.0;
    if (kind == 0)
        v *= 0.25;
    else if (kind == 1)
        v *= 0.5;
    else if (kind == 2)
        v *= 0.75;
    if (flag == 0)
        v = (0.7f * (care / 255.0) + 0.3f) * (v * 0.75);
    r = gm_PlayerFactor((int)v, ai_OwnPlayer);
    *out = (int)(r * gv_SaleShares[kind]);
    return r;
}
/* 0x4f2440, the cost difference of two kinds: 0x4f2330 (c, 100, k1, 1) minus half of the one of k2; *out the difference times the factor of k2 */

int gv_MovePrice(unsigned short *c, int k1, int k2, int *out)
{
    int a, b, d;

    a = gv_LeasePrice(c, 100, k1, 1, &d);
    b = gv_LeasePrice(c, 100, k2, 1, &d);
    d = b - a / 2;
    *out = (int)(d * gv_SaleShares[k2]);
    return d;
}
/* 0x4f24a0, the price the plunder event 0x8a (he.c 0x5707a0) demands from the character c (its id): its worth in the player's
   money (gm_GetWorth, gm_ConvertMoney) clamped to 10000 .. 100000, x / (log10 x + 1) * 0.1, in the player's terms
   (gm_PlayerFactor) */

int gv_ProtectionPrice(unsigned short *c)
{
    double x;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    x = max(min(100000.0, x), 10000.0);
    return gm_PlayerFactor((int)(x / (log10(x) + 1.0) * 0.1f), ai_OwnPlayer);
}
/* 0x4f2540, finds the grave entry held by the character a (gv_EntryHeldBy, any size) in the first graveyard building
   (kind 0x1d) of the 256 that has one: *o = the building, *e = the entry, 1; 0 (both 0) when none has */

int gv_FindHeldEntry(void *a, unsigned char **o, int *e)
{
    int i, r;
    AIBLD *b;

    *o = 0;
    *e = 0;
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0 && ((AIBTYPE *)(ai_BTypes + b->type * 0x24d))->kind == 0x1d) {
            r = gv_FindEntry(a, (unsigned char *)b, 0, gv_EntryHeldBy, 0xff);
            if (r != 0) {
                *o = (unsigned char *)b;
                *e = r;
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4f25d0, finds the grave entry reserved for the character a (gv_EntrySameId: its id is a's netId, any size) in the
   first graveyard building (kind 0x1d) of the 256 that has one: *o = the building, *e = the entry, 1; 0 (both 0) when none
   has */

int gv_FindReservedEntry(void *a, unsigned char **o, int *e)
{
    int i, r;
    AIBLD *b;

    *o = 0;
    *e = 0;
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0 && ((AIBTYPE *)(ai_BTypes + b->type * 0x24d))->kind == 0x1d) {
            r = gv_FindEntry(a, (unsigned char *)b, 0, gv_EntrySameId, 0xff);
            if (r != 0) {
                *o = (unsigned char *)b;
                *e = r;
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4f2660, a rate of the object o: 30 (item 0x20f) + 10 (0x21f) + 15 (0x222) + 25 (0x223), times 0.0125 * 0.5 + 0.5 */

float gv_SeanceChance(char *o)
{
    AIBLD *bld = (AIBLD *)o;
    float x;

    x = 0.0f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x20f) != 0)
        x = 30.0f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x21f) != 0)
        x = x + 10.0f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x222) != 0)
        x = x + 15.0f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x223) != 0)
        x = x + 25.0f;
    x = x * 0.0125f;
    x = x * 0.5f;     /* in steps: one expression folds 0.5 * y + 0.5 into (y + 1) * 0.5 */
    return x + 0.5f;
}

/* the graveyard's named constants, this unit's first .rdata (0x5ee034..) */
const float gv_SaleShares[] = { 0.800000012f, 0.699999988f, 0.600000024f, 0.5f };  /* 0x5ee034, the owner's share of a grave sale per size */
const float gv_GraveWear[] = { 15.0f, 12.5f, 10.0f, 7.5f };  /* 0x5ee044, the care a grave entry of each size loses per step (gv_GraveyardTurn: times (random + 1) / 2 and the care / 255) */
const int gv_EntryCounts[] = { 1, 2, 4, 8 };  /* 0x5ee054, entries per kind */
const int gv_EntryTotals[] = { 0, 12, 20, 30 };  /* 0x5ee064 */
const int gv_GraveTotals[] = { 0, 16, 32, 64 };  /* 0x5ee074 */
const int gv_ListEntries[4][4] = { { 0, 8, 12, 16 }, { 0, 4, 6, 8 }, { 0, 0, 2, 4 }, { 0, 0, 0, 2 } };  /* 0x5ee084 */
