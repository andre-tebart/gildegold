/* amt_ai.c -- am's second object, 0x4a3a90..0x4a7140: the ring of characters near a character (ai_NearRing and its
   cursor), group prices, the desires by name (tag "amt_GetDesireByName()" 0x600364), the eval callbacks of the moods /
   needs / guilt, the build site and building code (tag "amt_ExecGebaeudeBauen()" 0x60042c), the partner searches
   (chains, random walks over ai_CharWalkSteps) and the AI cannon shot. A unit of its own by its data: its named
   .rdata ai_CharWalkSteps..ai_NearRingCursor (0x5ed100) follows amr's literals 0x5ed048..0x5ed0fc (named constants
   come first in an object), its .data block ai_NearRingCursorState 0x60034c follows amr's strings (last user 0x4a3390)
   and its strings are used from 0x4a47cf on; the first user of its data is ai_NextNearRingEntry 0x4a3a90. The name
   is a guess from the "amt_" tags (amt.c, the offices, is another unit). Names are ours.
   unit 0x004a3a90..0x004a7140, split from am; boundaries in decl/text.dcl */
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "am.h"
#include "amt_ai.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "cm_types.h"
/* ai_DecideBuildBusiness (0x498c70, byte) tests flags1c8 & AI_CHF_BUILT: the original's
   `test dword [edi+0x1c8], 0x1000000` at 0x498fc3. 0x1000000 is an absolute constant (as are
   0xffffff / 0xff0000) and must not be written as symbol arithmetic: `&txt_Keys + 0x181c0` spells
   it only while txt_Keys keeps its original address 0xfe7e40; our link puts it at 0x6d0448, so the
   sum was 0x6e8608 and the test read an unrelated bit. */

#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char), @original: the byte-matched callers (ai_DecideDarkCorner, ai_DecideBuildBusiness) read the result without the unsigned char mask (no `and eax,0xff` after the call) */
#define AI_REPAIR_KINDS 0x7c43b4  /* the building kinds ai_DecideRepairBuilding repairs (one bit per AIBTYPE.kind) */

#include "hst.h"
#include "poker.h"
#include "mission.h"
extern int ai_PlayerCountYear;
G6CURSOR ai_NearRingCursorState = { ai_NearRing, 1 };  /* 0x60034c, the next entry of ai_NearRing and the stride */
extern int ai_NumPlayerChars;
extern AGCH ai_DuelOpponent;
G9SHOT ai_CannonShot = {0};  /* 0x6f0698 */
int ai_NearRingCount = 0;  /* 0x6f070c, the number of entries */
extern int ai_WriteActionTable;
extern char ai_DataPath[608];
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */
G6RING ai_NearRing[32];  /* 0xf83c20 */
extern char ai_ActionIniPath[260];
extern int ai_EvalDepth;
extern AIACT ai_Actions[0x49];
/* ai_Unkf84c40: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84c60: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84cd4: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84cf4: view of ai_Actions (tools/data_views.py) */
extern int ai_EvalStack[73];

#include "ls.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "sv.h"
#include "tr.h"
#include "lc.h"
#include "f3gm.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "d3_interface.h"
#include "d3_tools.h"
#include "d3_pose.h"
#include "menu.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3t_GetObjectBoundsV ((void (*)(void *, void *, void *))d3t_GetObjectBounds)  /* 0x413150 (d3_tools.c: D3VECTOR *, D3VECTOR *, D3OBJECT *): this unit's untyped view */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define gv_OrderMoveGraveI ((void (*)(AGCH *, int, void *, int, int, int))gv_OrderMoveGrave)  /* 0x4f0e80 (gv.c: int *c, char *o): @original the byte-matched ai_ExecHandleGrave passes its AGCH record and s1 as they are */
#define gm_CharLevelS ((int (*)(unsigned short, int))gm_CharLevel)  /* 0x505e60 (f3gm.c: unsigned short id, so the cast is now the prototype itself): @original this unit pushes the id as 16 bits (byte-matched ai_MasterExamPassed / ai_EvalFest / ai_RelationGain) */
#define amt_GetKindB ((int (*)(unsigned char, void *))amt_GetKind)  /* 0x4e6f80 (amt.h: int k): @original ai_PickLikedColumn passes (unsigned char)k without widening */
#define AI_CHAIN_BEATUP 0xf00500  /* the partner-chain flags (ai_PickPartnerChain) of action 0x18 "Verpruegeln" (ai_DecideBeatUp); the bits are not named yet */
#define AI_CHAIN_USEREPUTATION 0xf00500  /* the partner-chain flags of action 0x1c "UseObject (Ansehen)" (ai_DecideUseReputationObject), as AI_CHAIN_BEATUP; the bits are not named yet */
#define AI_CHAIN_MEAN 0xf00500  /* the partner-chain flags of action 0x21 "UseObject (Gemeinheit)" (ai_DecideUseMeanObject; | 0x800 for the good 0x29a, | 0x10000 for 0x177), as AI_CHAIN_BEATUP */
#define AI_CHAIN_SPY 0xf00500  /* the partner-chain flags of action 0x25 (spying, 0x49c570), as AI_CHAIN_BEATUP */
#define AI_CHF_BUILT 0x1000000  /* flags1c8 bit an execute callback sets after it built a building (ai_ExecBuildBuilding); ai_DecideBuildBusiness tests it */
#define AI_CHF_STAMMTISCH 0x2000000  /* flags1c8 bit ai_ExecuteStammtisch sets after a regulars' table decision; ai_DecideStammtisch skips a character that has it */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the four player slots */

/* dcp unit split: the declarations of am before the moved functions */
#undef AM_WRITE
#undef AM_READ

const int ai_CharWalkSteps[16] = { 1, 5, 7, 11, 13, 17, 19, 23, 745, 749, 751, 755, 757, 761, 763, 767 };  /* 0x5ed100, random steps */
const int ai_RandomStrides[16] = { 1, 3, 5, 7, 11, 13, 17, 19, 237, 239, 243, 245, 249, 251, 253, 255 };  /* 0x5ed140, random steps */
const int ai_RingStrides[11] = { 1, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };  /* 0x5ed180, the strides of the near ring cursor */
const int ai_RingStrideCount = 11;  /* 0x5ed1ac, the entries of ai_RingStrides (the name a guess from the value and the place); no reader in the surviving code: ai_FillNearRing scans from the immediate 0xb */
G6CURSOR *const ai_NearRingCursor = &ai_NearRingCursorState;  /* 0x5ed1b0, the code reaches the cursor through this constant pointer (mov reg, [0x5ed1b0]) */

/* 0x4a3a90, the next entry of the ring */

G6RING *ai_NextNearRingEntry(void)
{
    G6RING *r = ai_NearRingCursor->next;

    if (ai_NearRingCount == 0)
        return ai_NearRing;
    ai_NearRingCursor->next = &ai_NearRing[(unsigned int)(ai_NearRingCursor->next - ai_NearRing + ai_NearRingCursor->step) % ai_NearRingCount];
    return r;
}

/* the office holders of column col into the ring: each holder (not p) that passes cond, listed with flags f;
   G7_ADD_ANY fills without a bound (at most 3 * 6 entries), G7_ADD stops when the ring is full. A statement
   (do .. while (0)): its exit block orders the predecessors of the g test behind it, which keeps g in edi from
   the up test to the down test (vc6-matching "Live range splitting") */
#define G7_ADD_BODY(col, f, cond, guard)                                             \
    do {                                                                             \
        n = amt_GetColumnOfficesOwned((col), 6, list);                               \
        for (j = 0; j < n; j++) {                                                    \
            guard                                                                    \
            if ((cond) && (o = (G5CH *)gm_FindChar(list[j].owner)) != 0 && o != p) { \
                ai_NearRing[cnt].flags = (f);                                        \
                ai_NearRing[cnt].dist = gm_Liking(o->id, p->id, 1);                  \
                ai_NearRing[cnt].who = o;                                            \
                cnt++;                                                               \
            }                                                                        \
        }                                                                            \
    } while (0)
#define G7_ADD_ANY(col, f, cond) G7_ADD_BODY(col, f, cond, ;)
#define G7_ADD(col, f, cond) G7_ADD_BODY(col, f, cond, if (cnt >= 32) break;)
#define G7_LEVEL_NEAR (abs(amt_GetKindLevel(list[j].kind) - g) <= 1)

/* 0x4a3af0, fills the ring ai_NearRing with the characters near character p: without an office the holders of
   low offices (level < 3) of columns 1..3; with one the holders of its own column (0x10), of the neighbour
   columns (1), of the columns one level up or down (0x40 / 0x80, level within 1); then the applicants for the
   office p applies for (2), the other applicants (4) and, for a kind 5 character, the characters of kind 5..7 (1);
   kind 6 / 7 entries get (4 - difficulty) * 5 added to their liking; then sets the cursor to a random entry with
   a random stride from ai_RingStrides below the count, changed by one when it divides the count */
void ai_FillNearRing(AICHAR *c)
{
    G5CH *p = (G5CH *)c;
    AMTKIND kind;
    AMTREC list[6];
    int cnt, n, j, i, g, step;
    unsigned int f;
    unsigned char k;
    G5CH *o;
    const int *sz;

    cnt = 0;
    ai_NearRingCount = 0;
    memset(ai_NearRing, 0, 32 * sizeof(G6RING));
    if (p->officeKind == 0) {
        G7_ADD_ANY(1, 0x40, amt_GetKindLevel(list[j].kind) < 3);
        G7_ADD_ANY(2, 0x40, amt_GetKindLevel(list[j].kind) < 3);
        G7_ADD_ANY(3, 0x40, amt_GetKindLevel(list[j].kind) < 3);
    } else {
        int side[2];
        int up[2];
        int down[3];

        side[0] = 0;
        side[1] = 0;
        up[0] = 0;
        up[1] = 0;
        down[0] = 0;
        down[1] = 0;
        down[2] = 0;
        g = amt_GetKindLevel(p->officeKind);
        amt_GetKind(p->officeKind, &kind);
        if (kind.col > 0 && kind.col < 4) {
            if (kind.col == 1) {
                side[0] = 2;
                side[1] = 3;
            } else if (kind.col == 2) {
                side[0] = 1;
                side[1] = 3;
            } else {
                side[0] = 1;
                side[1] = 2;
            }
            if (g == 3) {
                up[0] = 4;
                up[1] = 5;
            }
        } else if (kind.col > 3 && kind.col < 6) {
            side[0] = (kind.col == 4) + 4;
            if (g == 6) {
                up[0] = 6;
            } else if (g == 4) {
                down[0] = 1;
                down[1] = 2;
                down[2] = 3;
            }
        } else if (g == 7) {
            down[0] = 4;
            down[1] = 5;
        }
        G7_ADD(kind.col, 0x10, 1);
        G7_ADD(side[0], 1, 1);
        G7_ADD(side[1], 1, 1);
        if (g == 3 || g == 6) {
            G7_ADD(up[0], 0x40, G7_LEVEL_NEAR);
            G7_ADD(up[1], 0x40, G7_LEVEL_NEAR);
        }
        if (g == 4 || g == 7) {
            G7_ADD(down[0], 0x80, G7_LEVEL_NEAR);
            G7_ADD(down[1], 0x80, G7_LEVEL_NEAR);
            G7_ADD(down[2], 0x80, G7_LEVEL_NEAR);
        }
    }
    if (p->appliedOffice != 0) {
        char *rivals[6];
        int nrivals;

        nrivals = amt_GetOfficeApplicants(p->appliedOffice, 6, rivals);
        for (j = 0; j < nrivals; j++) {
            if ((G5CH *)rivals[j] == p)
                continue;
            for (i = 0; i < cnt; i++) {
                if ((G5CH *)rivals[j] == ai_NearRing[i].who) {
                    ai_NearRing[i].flags |= 2;
                    break;
                }
            }
            if (i < cnt)
                continue;
            if (cnt < 32) {
                ai_NearRing[cnt].flags = 2;
                /* the original reads o, the last holder found above, not the applicant */
                ai_NearRing[cnt].dist = gm_Liking(o->id, p->id, 1);
                ai_NearRing[cnt].who = (G5CH *)rivals[j];
                cnt++;
            }
        }
    }
    if (p->appliedOffice != 0 || p->kind == 5) {
        for (j = 0; j < 768; j++) {
            o = &G5_CHARS[j];
            f = 0;
            if (p == o || o->id == 0xffff)
                continue;
            if ((o->kind == 6 || o->kind == 7 || o->kind == 5) && p->kind == 5)
                f = 1;
            if (o->appliedOffice == p->appliedOffice)
                f |= 4;
            if (f == 0)
                continue;
            for (i = 0; i < cnt; i++) {
                if (o == ai_NearRing[i].who) {
                    ai_NearRing[i].flags |= f;
                    break;
                }
            }
            if (i < cnt)
                continue;
            if (cnt < 32) {
                ai_NearRing[cnt].flags = f;
                ai_NearRing[cnt].dist = gm_Liking(o->id, p->id, 1);
                ai_NearRing[cnt].who = o;
                cnt++;
            }
        }
    }
    step = (4 - ai_Difficulty) * 5;
    for (i = 0; i < cnt; i++) {
        k = ai_NearRing[i].who->kind;
        if (k == 6 || k == 7)
            ai_NearRing[i].dist = (float)step + ai_NearRing[i].dist;
    }
    ai_NearRingCount = cnt;
    if (cnt > 3) {
        g = 11;
        for (sz = &ai_RingStrides[10]; g != 0; g--, sz--) {
            if (*sz < cnt)
                break;
        }
        ai_NearRingCursor->next = &ai_NearRing[util_RandomU(cnt)];
        ai_NearRingCursor->step = ai_RingStrides[util_RandomU(g)];
        if (ai_NearRingCursor->step > 1 && ai_NearRingCount % ai_NearRingCursor->step == 0)
            ai_NearRingCursor->step += (util_RandomU(2) != 0 ? 2 : 0) - 1;
    } else {
        ai_NearRingCursor->next = ai_NearRing;
        ai_NearRingCursor->step = 1;
    }
}
/* 0x4a4680, the current prices of the items of group m->key against their base prices */

int ai_GroupPrices(G7MKT *m)
{
    int i, k, n;
    float sum, r;

    for (i = 0; i < 0x25; i++) {
        if (gm_BuildingItemGroups[i].key == m->key)
            break;
    }
    if (i >= 0x25)
        return 0;
    n = 0;
    for (k = 0; k < 10; k++) {
        if (gm_BuildingItemGroups[i].items[k] != 0)
            m->items[n++] = gm_BuildingItemGroups[i].items[k];
    }
    m->n = n;
    if (n == 0)
        return 0;
    m->hi = 0;
    m->lo = 0;
    sum = 0.0f;
    for (k = 0; k < m->n; k++) {
        m->price[k] = (int)gm_GoodsPrice(m->items[k], ai_OwnPlayer);
        m->base[k] = (int)gm_ItemPrice(m->items[k], 100);
        r = (float)m->price[k] / (float)m->base[k];
        m->ratio[k] = r;
        sum += r;
        if (m->ratio[m->hi] < r)
            m->hi = k;
        if (m->ratio[k] < m->ratio[m->lo])
            m->lo = k;
    }
    m->avg = sum / (float)m->n;
    return 1;
}
/* 0x4a47c0, amt_GetDesireByName */

int ai_GetDesireByName(char *s)
{
    char buf[128];

    if (strcmpi(s, "APS") == 0)
        return 0;
    if (strcmpi(s, "UNVERSEHRTHEIT") == 0)
        return 1;
    if (strcmpi(s, "WOHNUNG") == 0)
        return 2;
    if (strcmpi(s, "GELD") == 0)
        return 3;
    if (strcmpi(s, "BERUF") == 0)
        return 4;
    if (strcmpi(s, "VERGNUEGEN") == 0)
        return 5;
    if (strcmpi(s, "ANSEHEN") == 0)
        return 6;
    if (strcmpi(s, "AMT") == 0)
        return 7;
    if (strcmpi(s, "BILDUNG") == 0)
        return 8;
    if (strcmpi(s, "RECHTSCHAFFENHEIT") == 0)
        return 9;
    if (strcmpi(s, "GEMEINHEIT") == 0)
        return 10;
    if (strcmpi(s, "SICHERHEIT") == 0)
        return 11;
    if (strcmpi(s, "FORTPFLANZUNG") == 0)
        return 12;
    if (strcmpi(s, "TRAEGHEIT") == 0)
        return 13;
    sprintf(buf, "amt_GetDesireByName(): Unbekanntes Bed\xfc" "rfnis: %s", s);
    return -1;
}
/* 0x4a49a0, eval callback: the action's weights against the moods */

int ai_EvalMoodWeights(float *a, float *b, int i, AIPER *p, int f, void *x, void *y)
{
    G5ACT *act;
    float d1, d2;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    act = &G5_ACTS[i];
    if (act->flags & G7_MASK(p)) {
        d1 = 40.0f;
        d2 = 50.0f;
        found = 1;
    } else {
        d1 = 50.0f;
        d2 = 100.0f;
        found = 0;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        *a = (act->w1[k].k == p->sel ? act->w1[k].w / d1 * p->mood[act->w1[k].k][2] : act->w1[k].w / d2 * p->mood[act->w1[k].k][2]) + *a;
        *b = (act->w2[k].k == p->sel ? act->w2[k].w / d1 * p->mood[act->w2[k].k][2] : act->w2[k].w / d2 * p->mood[act->w2[k].k][2]) + *b;
    }
    return found;
}
/* 0x4a4ae0, eval callback: only the weights of the current need count */

int ai_EvalCurrentNeed(float *a, float *b, int i, AIPER *p)
{
    G5ACT *act;
    float d1;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    act = &G5_ACTS[i];
    if (act->flags & G7_MASK(p)) {
        d1 = 40.0f;
        found = 1;
    } else {
        d1 = 50.0f;
        found = 0;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        *a = (act->w1[k].k == p->sel ? act->w1[k].w / d1 * p->mood[act->w1[k].k][2] : 0.0f) + *a;
        *b = (act->w2[k].k == p->sel ? act->w2[k].w / d1 * p->mood[act->w2[k].k][2] : 0.0f) + *b;
    }
    return found;
}
/* 0x4a4c10, eval callback with the character's rank and the decision's value */

int ai_EvalGuiltWeights(float *a, float *b, int i, AIPER *p, AIDEC *dec)
{
    G5ACT *act;
    float v, rank, d1, d2;
    int n, k, found;

    if (dec == 0)
        v = 1.0f;
    else
        v = (float)dec->guilt;
    n = gv_CulpritGuilt(p->netId);
    rank = ((float)n + v) * (p->mood[11][0] - 2.0f);
    *a = 0.0f;
    *b = 0.0f;
    act = &G5_ACTS[i];
    if (act->flags & G7_MASK(p)) {
        d1 = 40.0f;
        d2 = 50.0f;
        found = 1;
    } else {
        d1 = 50.0f;
        d2 = 100.0f;
        found = 0;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k >= 4)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        if (act->w1[k].w > 0.0f)
            *a = (act->w1[k].k == p->sel ? p->mood[act->w1[k].k][2] * act->w1[k].w / (d1 + rank) : p->mood[act->w1[k].k][2] * act->w1[k].w / (d2 + rank)) + *a;
        else
            *a = (act->w1[k].k == p->sel ? act->w1[k].w / d1 : act->w1[k].w / d2) * p->mood[act->w1[k].k][2] + *a;
        if (act->w2[k].w > 0.0f)
            *b = (act->w2[k].k == p->sel ? p->mood[act->w2[k].k][2] * act->w2[k].w / (d1 + rank) : p->mood[act->w2[k].k][2] * act->w2[k].w / (d2 + rank)) + *b;
        else
            *b = (act->w2[k].k == p->sel ? act->w2[k].w / d1 : act->w2[k].w / d2) * p->mood[act->w2[k].k][2] + *b;
    }
    return found;
}
/* 0x4a4e30, a building's value for a stake of k (10 / 30 / 50) */

int ai_RepairCost(AIBLD *b, int k)
{
    int v = (int)(gm_EquipmentValue(b, 100) * 0.005f);

    if (k == 50)
        return v * 4;
    if (k == 30)
        return v * 2;
    if (k != 10)
        v = 0;
    return v;
}
/* 0x4a4e70, picks one of the build-site objects (ai_FindBauplaetze) of building kind name: the nearest (with noise) to the character's
   house (2 of 3 times) or to the market building, else a random one; 0 when none
   @deep deep s38: the original's frame (0x478) has two 4-byte slots more than ours, at [esp+0x20] (no
   instruction touches it) and [esp+0x24] (the fst of the (float)sqrt value before fmulp, in both loops);
   ours packs that spill into dx's slot (c2frame: tmp@538 / tmp@558 join dx, whose life ends in the sum).
   So in the original the spill conflicts with dx / dy / dz / best and a fifth small local exists that has
   a slot but no code left (references removed by a late pass?). Tried: a named dist = (float)sqrt(..)
   inside the product (= control), dx / dy / dz as block locals of the loop bodies (56 ins, frame
   unchanged), the double sqrt without the (float) cast (6 ins / 362 bytes: the frame stays 0x470, fpu
   hunks). Open: which local the empty slot is. prior: read in full against the original and reconciled
   (both nearest-site loops, the random fallback, the scene save/restore); the vectors are 16-byte float[4]
   slots as in the original's frame (q at 0x28, o at 0x38, buf at 0x48). Left is the frame and allocation:
   the original gives the (float)sqrt spill its own slot (fst [esp+0x24]) where ours reuses dz's [esp+0x10]
   and swaps best/dz (slots 0x10<->0x14), so the frame is 0x478 vs ours 0x470 and every list[] access
   shifts by 8 (code hunks at 0x4a4f37, 0x4a4fc0, 0x4a502a, 0x4a50b3, 0x4a50f3, 0x4a5129: [esp+esi*4+0x88]
   vs ours +0x80); the fallback counter is ebx in the original, ecx in ours (0x4a50e0: + mov ebx,edi before
   the call / - mov ecx,edi after it) */

int ai_PickBuildSite(AIPER *p, int name)
{
    int scene = d3_CurrentScene;
    char buf[64];
    int list[256];
    float o[4];                         /* the positions d3_PointToWorld writes: 16-byte slots in the original's frame */
    float q[4];
    float dx, dy, dz, d, best;
    struct D3OBJECT *obj;
    int n, i, r;
    unsigned int k;

    d3if_SelectScene(0, 1);
    if (ai_BuildingName(name, buf) != 0) {
        memset(list, 0, sizeof(list));
        n = ai_FindBauplaetze(buf, list);
        if (n != 0) {
            if (p->bld2 != 0 && AI_BX(p->bld2)->object != 0 && util_RandomU(3) != 0) {
                obj = AI_BX(p->bld2)->object;
                d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((D3OBJECT *)list[i], &((D3OBJECT *)list[i])->pos, (D3VECTOR *)q);
                        dx = q[0] - o[0];
                        dy = q[1] - o[1];
                        dz = q[2] - o[2];
                        d = (float)sqrt(dx * dx + dy * dy + dz * dz) * (util_RandomF() * 0.5f + 0.75f);
                        if (d < best) {
                            best = d;
                            r = list[i];
                        }
                    }
                }
                d3if_SelectScene(scene, 1);
                return r;
            }
            if (ai_MarketBuilding != 0 && (obj = AI_BX(ai_MarketBuilding)->object) != 0) {
                d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((D3OBJECT *)list[i], &((D3OBJECT *)list[i])->pos, (D3VECTOR *)q);
                        dx = q[0] - o[0];
                        dy = q[1] - o[1];
                        dz = q[2] - o[2];
                        d = (float)sqrt(dx * dx + dy * dy + dz * dz) * (util_RandomF() * 0.5f + 0.75f);
                        if (d < best) {
                            best = d;
                            r = list[i];
                        }
                    }
                }
                d3if_SelectScene(scene, 1);
                return r;
            }
            k = util_RandomU(n);
            for (i = n; i != 0; i--) {
                if (list[k] != 0) {
                    d3if_SelectScene(scene, 1);
                    return list[k];
                }
                k = (int)(k + 1) % n;
            }
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}
/* 0x4a5140: may character p build the building type s1->unk04 (s1 kind 5)? A type of class 2 must rank above
   the one it has (0x170) and pass 0x4f81c0 (2 or 4) for types 4..7; other types ask gm_MayTakeJob. With f, s2 becomes
   the plot (kind 0xc) that 0x4a4e70 finds */

int ai_CanBuildType(int a, AIPER *p, int b, AIARG *s1, AIARG *s2, int f)
{
    int q[0x88];
    AIBLD *h;
    int k, r;

    memset(q, 0, sizeof(q));
    q[0] = 1;
    if (s1->kind != 5)
        return 0;
    k = s1->ref;
    if (ai_BTypesT[k].kind == 2) {
        h = p->bld2;
        if (h != 0 && ai_BTypesT[h->type].cap247 >= ai_BTypesT[k].cap247 && ai_BTypesT[h->type].kind == 2)
            return 0;
        if (k == 4) {
            r = ord_DispatchOrder(1, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 5) {
            r = ord_DispatchOrder(5, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 6) {
            r = ord_DispatchOrder(9, p, q);
            if (r != 2 && r != 4)
                return 0;
        } else if (k == 7) {
            r = ord_DispatchOrder(0xc, p, q);
            if (r != 2 && r != 4)
                return 0;
        }
    } else if (gm_MayTakeJob(k, p->id) == 0) {
        return 0;
    }
    if (f == 0)
        return 1;
    r = ai_PickBuildSite(p, s1->ref);
    if (r == 0)
        return 0;
    s2->ref = r;
    s2->kind = 0xc;
    s2->arg = 1;
    return 1;
}
/* 0x4a5320, amt_ExecGebaeudeBauen: builds the building type s1->unk04 (kind 5) on the plot s2->unk04 (kind 0xc):
   finds its model group "gb_<NAME>" (with a "_A" variant), message 0xd with the price, place and group path */

int ai_ExecBuildBuilding(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG m;
    char name[0x104];
    char path[0x104];
    char found[0x100];
    int plot, alt, ok;

    if (s1->kind != 5)
        return 0;
    if (s2->kind != 0xc)
        return 0;
    plot = s2->ref;
    strcpy(name, "gb_");
    strcat(name, ai_BTypesT[s1->ref].name);
    strupr(name + 2);
    ok = 0;
    alt = 0;
    for (;;) {
        if (alt)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, name, (char)(alt + '@'));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, name);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0) {
            sprintf(m.buildPath, "%s", path);
            ok = 1;
            break;
        }
        if (alt)
            break;
        alt = 1;
    }
    if (!ok) {
        sprintf(name, "amt_ExecGebaeudeBauen(): Keine Group f\xfcr %s gefunden", ai_BTypesT[s1->ref].name);
        return 0;
    }
    m.to = p->netId;
    m.fromId = p->netId;
    m.type = 0xd;
    m.time = ai_Time;
    m.arg[0] = p->netId;
    m.flags = 2;
    m.arg[1] = s1->ref;
    m.arg[2] = gm_BuildingKindCost(p, s1->ref);
    ai_ObjectPosition(m.buildPos, plot);
    ai_ObjectAngle(m.buildRot, plot);
    cm_SendLongMessage(&m);
    return 1;
}
/* 0x4a5540: the first good of building type b->type that b's store does not hold yet while it holds both of its
   two ingredients (slot 0x40 + 2i, 0x41 + 2i); goods of kind 2 / 6 need a free cart (list 4) or one carrying it.
   *item gets the good, *obj the cart's id (-1 for none) */

int ai_FindGoodToMake(int *item, int *obj, AIBLD *b, int f)
{
    AIBTYPE *t;
    AIOBJ *o, *cart;
    int list, n, i, found;
    AIGOOD *g;

    list = b->objList;
    t = &ai_BTypesT[b->type];
    cart = 0;
    for (i = 0; i < t->nslots; i++) {
        g = &ai_GoodsT[t->slot[i] & 0x7fff];
        if (g->type == '!')
            continue;
        if (g->type == 2 || g->type == 6) {
            found = 0;
            n = 0;
            cart = 0;
            for (o = gm_Query(b->objList, 1, 4, 2); o != 0; o = gm_QueryNext()) {
                if (o->id != 0xfd)
                    n++;
                if (o->id == (t->slot[i] & 0x7fff)) {
                    list = o->list;
                    found = 1;
                    cart = o;
                    break;
                }
            }
            if (n >= (unsigned char)t->name[0x20] || found)
                continue;
            o = gm_Query(b->objList, 1, 4, 6);
            while (1) {
                if (!o)
                    break;
                if (o->id == (t->slot[i] & 0x7fff)) {
                    list = o->list;
                    found = 1;
                    cart = o;
                    break;
                }
                o = gm_QueryNext();
            }
            if (n >= (unsigned char)t->name[0x20] || found)
                continue;
        }
        if (gm_Query(list, 1, 0, t->slot[i] & 0x7fff) == 0
            && (t->slot[0x40 + 2 * i] == 0 || gm_Query(list, 1, 0, t->slot[0x40 + 2 * i]) != 0)
            && (t->slot[0x41 + 2 * i] == 0 || gm_Query(list, 1, 0, t->slot[0x41 + 2 * i]) != 0)) {
            *item = t->slot[i] & 0x7fff;
            if (cart != 0) {
                *obj = cart->objId;
                return 1;
            }
            *obj = -1;
            return 1;
        }
    }
    return 0;
}
/* 0x4a5760: for n wanted items rec[i].item, finds them in the character's inventory list (goods of kind 9
   skipped): rec[i].entryId the entry's id, rec[i].entry the entry, rec[i].value its count when usable (its
   Domizil entry's mask shares no bit with the character's goodsMask and ai_InventoryHas refuses), else -1.
   st[0] counts the entries, st[1] the unusable, st[2] the usable ones */

void ai_FindWantedItems(G5CH *p, int n, G9REC *rec, int *st)
{
    AIOBJ *e;
    G9REC *r;
    G9DOMIZIL *d;
    int i;

    for (r = rec, i = n; i > 0; i--, r++) {
        r->entryId = -1;
        r->value = 0;
    }
    st[1] = 0;
    st[2] = 0;
    st[0] = 0;
    for (e = (AIOBJ *)p->inventory; e != 0; e = e->next) {
        if (ai_GoodsT[e->id].type == 9)
            continue;
        st[0]++;
        for (i = 0; i < n; i++) {
            if (e->id != rec[i].item)
                continue;
            d = (G9DOMIZIL *)ai_FindUsableGoodP(e->id);
            if (d == 0) {
                rec[i].value = -1;
                st[1]++;
                rec[i].entry = 0;
                rec[i].entryId = -1;
            } else if (!(d->mask & p->goodsMask) && ai_InventoryHas(p, e->id) == 0) {
                st[2]++;
                rec[i].value = e->count;
                rec[i].entryId = e->objId;
                rec[i].entry = (int)e;
            } else {
                rec[i].value = -1;
                st[1]++;
                rec[i].entryId = e->objId;
                rec[i].entry = (int)e;
            }
            break;
        }
    }
    for (i = 0; i < n; i++) {
        if (rec[i].entryId == -1 && (d = (G9DOMIZIL *)ai_FindUsableGoodP(rec[i].item)) != 0 && (d->mask & p->goodsMask))
            st[1]++;
    }
}
/* 0x4a58d0: picks a partner chain of a (< 4) characters for an action with the flags flags[i]: the leader
   ai_Partner first when ai_PassesPartnerFilter accepts it, then for each further slot the next near-ring entry whose
   rating lies in [b, f] and that ai_PassesPartnerFilter accepts with the chain so far; ids into id[] */

int ai_PickPartnerChain(AIPER *p, int a, int *flags, float b, float f, unsigned short *id)
{
    unsigned int q[0x14];
    G6RING *e;
    unsigned int i, n;
    int found;

    if ((unsigned int)a > 3)
        return 0;
    memset(q, 0, sizeof(q));
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    i = 0;
    if (ai_Partner != 0 && !(q[1] & 0x10000000)) {
        q[16] = 0;
        q[1] = flags[0];
        if (ai_PassesPartnerFilter(p->id, AI_PARTNER->id, q, 0) != 0) {
            i = 1;
            id[0] = AI_PARTNER->id;
        }
    }
    for (; i < (unsigned int)a; i++) {
        q[1] = flags[i];
        if (i != 0)
            ((unsigned short *)&q[7])[i] = id[i - 1];
        else
            ((unsigned short *)&q[7])[0] = p->id;
        found = 0;
        n = ai_NearRingCount;
        for (e = ai_NextNearRingEntry(); !found; n--, e = ai_NextNearRingEntry()) {
            if (n == 0) {
                if (!found)
                    return 0;
                break;
            }
            if (e->who == 0)
                return 0;
            if (!(e->dist < b) && !(e->dist > f)
                && ai_PassesPartnerFilter(p->id, e->who->id, q, 0) != 0) {
                found = 1;
                id[i] = e->who->id;
            }
        }
    }
    return 1;
}
/* 0x4a5a30: one partner for an action with the flags fl and the chain ids[n]: the leader ai_Partner when
   ai_PassesPartnerFilter accepts it, else the first near-ring entry rated in [lo, hi] that it accepts; into *id */

int ai_PickPartnerInRange(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    G6RING *e;
    unsigned int len;
    int k;

    memset(q, 0, sizeof(q));
    len = n * 2;
    if (len > 0x20)
        len = 0x20;
    memcpy(&q[7], ids, len);
    q[5] = 2;
    if (ai_Partner != 0 && !(q[1] & 0x10000000)) {
        q[16] = 0;
        q[1] = fl;
        if (ai_PassesPartnerFilter(p->id, AI_PARTNER->id, q, 0) != 0) {
            *id = AI_PARTNER->id;
            return 1;
        }
    }
    q[16] = 0;
    k = ai_NearRingCount;
    do {
        e = ai_NextNearRingEntry();
        if (k == 0 || e->who == 0)
            return 0;
        if (!(e->dist < lo) && !(e->dist > hi)
            && ai_PassesPartnerFilter(p->id, e->who->id, q, 0) != 0) {
            *id = e->who->id;
            return 1;
        }
        k--;
    } while (1);
}
/* 0x4a5b50: as ai_PickPartnerChain, but the partners come from a random walk over all 768 characters (ai_CharWalkSteps);
   the rating window [lo, hi] (0x50c550) only counts when it is narrower than 0..100 */

int ai_FindPartnersRandomWalk(AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win;
    float r;

    step = ai_CharWalkSteps[util_RandomU(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = flags[0];
        if (ai_PassesPartnerFilter(p->id, AI_PARTNER->id, q, 0) != 0) {
            i = 1;
            id[0] = AI_PARTNER->id;
        }
    }
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    for (; i < a; i++) {
        q[1] = flags[i];
        if (i != 0)
            ((unsigned short *)&q[7])[i] = id[i - 1];
        else
            ((unsigned short *)&q[7])[0] = p->id;
        n = 0x300;
        found = 0;
        c = util_RandomU(0x300);
        while (!found) {
            if (n == 0) {
                if (!found)
                    return 0;
                break;
            }
            if (ai_PassesPartnerFilter(p->id, (unsigned short)c, q, 0) != 0) {
                if (!win || (r = gm_Liking(c, p->id, 1), !(r < lo) && !(r > hi))) {
                    found = 1;
                    id[i] = (unsigned short)c;
                }
            }
            n--;
            c = (int)(c + step) % 0x300;
        }
    }
    return 1;
}
/* 0x4a5d30: as ai_PickPartnerInRange, but the partner comes from a random walk over all 768 characters (as
   ai_FindPartnersRandomWalk) */

int ai_PickPartnerRandomWalk(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int len, c;
    int step, k, found, win;
    float r;

    step = ai_CharWalkSteps[util_RandomU(0x10)];
    memset(q, 0, sizeof(q));
    win = 0;
    len = n * 2;
    memcpy(&q[7], ids, len > 0x20 ? 0x20 : len);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = fl;
        if (ai_PassesPartnerFilter(p->id, AI_PARTNER->id, q, 0) != 0) {
            *id = AI_PARTNER->id;
            return 1;
        }
    }
    q[1] = fl;
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    found = 0;
    k = 0x300;
    c = util_RandomU(0x300);
    while (!found) {
        if (k == 0) {
            if (!found)
                return 0;
            break;
        }
        if (ai_PassesPartnerFilter(p->id, (unsigned short)c, q, 0) != 0) {
            if (!win || (r = gm_Liking(c, p->id, 1), !(r < lo) && !(r > hi))) {
                found = 1;
                *id = (unsigned short)c;
            }
        }
        k--;
        c = (int)(c + step) % 0x300;
    }
    return 1;
}
/* 0x4a5ef0: up to n candidates for an action with the flags fl, from two random walks over all 768 characters
   (no kinds 8 / 7 / 5 / 9 / 0x14; first the ones with a jobId (+0x164) other than 'R', then those without),
   accepted by ai_PassesPartnerFilter, with a previous-office level (gm_CharPrevLevel) within 5 of p's and, when
   the window is narrower than 0..100, a liking in [lo, hi]; ids into out[], returns the number */

unsigned int ai_FindCandidates(AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out)
{
    unsigned int q[0x14];
    unsigned int cnt;
    int i, step, k, rank, win;
    unsigned char t;
    float r;

    step = ai_CharWalkSteps[util_RandomU(0x10)];
    win = 0;
    memset(q, 0, sizeof(q));
    memset(&q[7], 0xff, 8 * 4);
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    q[5] = 2;
    q[1] = fl;
    rank = gm_CharPrevLevel(p->id, 0);
    cnt = 0;
    k = 0x300;
    i = util_RandomU(0x300);
    while (cnt < n && k != 0) {
        t = ai_CharsT[i].kind;
        if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && ai_CharsT[i].jobId != 0 && ai_CharsT[i].jobId != 'R'
            && ai_PassesPartnerFilter(p->id, (unsigned short)i, q, 0) != 0
            && abs(rank - gm_CharPrevLevel((unsigned short)i, 0)) < 6
            && (!win || (r = gm_Liking(i, p->id, 1), !(r < lo) && !(r > hi)))) {
            out[cnt++] = (unsigned short)i;
        }
        k--;
        i = (i + step) % 0x300;
    }
    k = 0x300;
    i = util_RandomU(0x300);
    while (cnt < n && k != 0) {
        t = ai_CharsT[i].kind;
        if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && ai_CharsT[i].jobId == 0
            && ai_PassesPartnerFilter(p->id, (unsigned short)i, q, 0) != 0
            && abs(rank - gm_CharPrevLevel((unsigned short)i, 0)) < 6
            && (!win || (r = gm_Liking(i, p->id, 1), !(r < lo) && !(r > hi)))) {
            out[cnt++] = (unsigned short)i;
        }
        k--;
        i = (step + i) % 0x300;
    }
    return cnt;
}
/* 0x4a61d0: a partner chain of up to a characters for an action with the flags fl: the leader ai_Partner first,
   then for each slot a random walk over the 768 characters for one ai_PassesPartnerFilter accepts with the chain so
   far, with a liking in [lo, hi] when that window is narrower than 0..100 and an office level (officeKind,
   amt_GetKindLevel) at most d away from p's; ids into id[], returns the number found */

unsigned int ai_PickPartnerChainLevel(AIPER *p, unsigned int a, unsigned int fl, float lo, float hi, int d, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win, home;
    float r;

    step = ai_CharWalkSteps[util_RandomU(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[1] = fl;
        q[16] = 0;
        if (ai_PassesPartnerFilter(p->id, ai_Partner->id, q, 0) != 0) {
            i = 1;
            id[0] = ai_Partner->id;
        }
    }
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    home = amt_GetKindLevel(p->officeKind);
    for (; i < a; i++) {
        q[1] = fl;
        if (i != 0)
            ((unsigned short *)&q[7])[i] = id[i - 1];
        else
            ((unsigned short *)&q[7])[0] = p->id;
        n = 0x300;
        found = 0;
        c = util_RandomU(0x300);
        while (!found) {
            if (n == 0) {
                if (!found)
                    return i;
                break;
            }
            if (ai_PassesPartnerFilter(p->id, (unsigned short)c, q, 0) != 0
                && (!win || (r = gm_Liking(c, p->id, 1), !(r < lo) && !(r > hi)))
                && abs(home - amt_GetKindLevel(ai_CharsT[c].officeKind)) <= d) {
                found = 1;
                id[i] = (unsigned short)c;
            }
            n--;
            c = (int)(c + step) % 0x300;
        }
    }
    return i;
}
/* 0x4a63f0: a building for p's action a: a random walk over the 256 buildings for one that ai_PassesBuildingFilter
   accepts with the kind mask (bit 63 forced: no dead or homeless buildings) and, when [lo, hi] is narrower than
   0..100, whose owner's liking (gm_Liking) equals the filter record's liking minimum (+0x44, as ai_PassesPartnerFilter
   reads it): the original compares both bounds against that float, which it never sets (lo and hi only switch the
   test on); its id into *out (-1 for none), returns 1 when found */

int ai_PickActionBuilding(AIPER *p, int a, __int64 mask, float lo, float hi, int *out)
{
    unsigned int q[0x14];
    AIBLD *b;
    int step, n, found, win, j;
    float r;

    step = ai_RandomStrides[util_RandomU(0x10)];
    win = 0;
    q[2] = (unsigned int)mask;
    found = 0;
    q[5] = 0;
    q[3] = (unsigned int)((unsigned __int64)mask >> 32) | 0x80000000;
    q[4] = a;
    *out = -1;
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    n = 0x100;
    j = util_RandomU(0x100);
    while (!found) {
        if (n == 0) {
            if (!found)
                return 0;
            break;
        }
        b = &ai_BldsT[j];
        if (b->type != 0 && ai_PassesBuildingFilter(p, q, b) != 0
            && (!win || (r = gm_Liking(b->owner, p->id, 1), !(r < ((float *)q)[0x11]) && !(r > ((float *)q)[0x11])))) {
            found = 1;
            *out = b->id;
        }
        n--;
        j = (j + step) % 0x100;
    }
    return 1;
}
/* 0x4a6570: ai_PickActionBuilding collecting up to n buildings: a random walk over the 256 buildings, the ones
   ai_PassesBuildingFilter accepts with the kind mask (bit 63 forced) and, when [lo, hi] is narrower than 0..100,
   whose owner's liking equals the filter record's unset minimum (+0x44, both bounds, as in ai_PickActionBuilding);
   ids into out[], returns the number */

int ai_PickActionBuildings(AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out)
{
    unsigned int q[0x14];
    AIBLD *b;
    int step, k, cnt, win, j, max;
    float r;

    step = ai_RandomStrides[util_RandomU(0x10)];
    cnt = 0;
    win = 0;
    q[5] = 0;
    q[2] = (unsigned int)mask;
    q[3] = (unsigned int)((unsigned __int64)mask >> 32) | 0x80000000;
    q[4] = a;
    if ((lo > 0.0 || hi < 100.0) && lo < hi)
        win = 1;
    k = 0x100;
    j = util_RandomU(0x100);
    max = n;
    while (cnt < max && k != 0) {
        b = &ai_BldsT[j];
        if (b->type != 0 && ai_PassesBuildingFilter(p, q, b) != 0
            && (!win || (r = gm_Liking(b->owner, p->id, 1), !(r < ((float *)q)[0x11]) && !(r > ((float *)q)[0x11])))) {
            out[cnt] = b->id;
            cnt++;
        }
        k--;
        j = (j + step) % 0x100;
    }
    return cnt;
}
/* 0x4a66f0: the positions of two buildings' models (0x61) in scene 0: a (raised by 0.8 of its height), b; -1
   when one is missing or they are the same */

int ai_BuildingAimPoints(AIBLD *x, float *a, AIBLD *y, float *b)
{
    int scene = d3_CurrentScene;
    float mn[4];
    float mx[4];

    if (x != 0 && y != 0 && x->object != 0 && y->object != 0 && x != y) {
        d3if_SelectScene(0, 1);
        d3_PointToWorld((void *)x->object, x->object + 0x4c, a);
        d3t_GetObjectBoundsV((void *)mn, (void *)mx, (void *)x->object);
        a[1] = (mx[1] - mn[1]) * 0.8f + a[1];
        d3_PointToWorld((void *)y->object, y->object + 0x4c, b);
        d3if_SelectScene(scene, 1);
        return 0;
    }
    return -1;
}
/* 0x4a67a0: aims p's cannon in building b at building t: a random cannon type r (gv_BuildingCannon / gv_CannonKind,
   p's money must reach its level), a ballistic solution (ai_SphereVolume of the iron ball, ai_PositionShare,
   ai_SolveCannonAim) within -15..45 degrees, its charge (gv_SalvoCharge) within the cannon's limit and its cost
   (gv_CannonCost) within a quarter of p's goods; the shot goes into ai_CannonShot, 1 when aimed */

int ai_AimCannon(AIPER *p, AIBLD *b, AIBLD *t)
{
    G9CANNON *cn;
    G9KIND kind;
    float pa[4];
    float pb[4];
    float rng[4];
    float v94[2];
    float vb[4];
    float vc[4];
    float vd[4];
    float x5c, x48;
    int par[5];
    int r, r2, n, cost, x, k;
    float f, ang;

    ai_CannonShot.who = -1;
    ai_CannonShot.bld = -1;
    cn = gv_FindEffect(b->id);
    if (cn == 0)
        return 0;
    if (ai_BuildingAimPoints(b, pa, t, pb) != 0)
        return 0;
    gv_BuildingCannon(b, &r2, &r);
    r = util_RandomU(r + 1);
    gv_CannonKind(r, &kind);
    if (p->money < kind.level)
        return 0;
    f = gv_CannonItemFactor(b);
    r2 = util_RandomU(r2 + 1);
    ai_SphereVolume(ai_IronDensity, ai_CannonBalls[r].c, par);
    ai_PositionShare(ai_CannonBalls[r].a, f * ai_CannonBalls[r].b, par, v94);
    rng[0] = v94[0];
    rng[1] = v94[1];
    rng[2] = -0.2617994f;
    rng[3] = 0.7853982f;
    if (ai_SolveCannonAim(pa, pb, rng, &x5c, &x48, &ang, vb, vc, vd) != 0)
        return 0;
    k = (int)((ang - rng[0]) * 100.0f / (rng[1] - rng[0]));
    if (gv_SalvoCharge(r, k, r2 + 1) > cn->charge[r2])
        return 0;
    n = gv_CannonCost(p, r, k, r2 + 1, &x, &cost);
    if (n == 0)
        return 0;
    if ((int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.25f) < cost)
        return 0;
    ai_CannonShot.a[0] = pa[0];
    ai_CannonShot.a[1] = pa[1];
    ai_CannonShot.a[2] = pa[2];
    ai_CannonShot.b[0] = vb[0];
    ai_CannonShot.b[1] = vb[1];
    ai_CannonShot.b[2] = vb[2];
    ai_CannonShot.c[0] = vc[0];
    ai_CannonShot.c[1] = vc[1];
    ai_CannonShot.c[2] = vc[2];
    ai_CannonShot.d[0] = vd[0];
    ai_CannonShot.d[1] = vd[1];
    ai_CannonShot.d[2] = vd[2];
    ai_CannonShot.who = p->netId;
    ai_CannonShot.bld = b->id;
    ai_CannonShot.x = x;
    ai_CannonShot.cost = cost;
    ai_CannonShot.k = k;
    ai_CannonShot.ang = ang;
    ai_CannonShot.charge = r2 + 1;
    ai_CannonShot.kind = r;
    memcpy(ai_CannonShot.par, par, sizeof(par));
    return 1;
}
/* 0x4a6ae0, execute callback of the cannon shot: fires the 1..3 balls ai_AimCannon planned into ai_CannonShot: one
   ball from 50 units along the barrel b; two from 40 units, 5 to either side along d; three from 40 units, one 5
   along c, two 3 back along c and 5 to either side along d. Each ball: ai_InitShot, ai_ScatterShot by the
   cannon's spread (gv_CannonScatter); then gv_FireShots with the charge, 1 when fired
   @deep x87 research: cases 2/3 copied the sum as a CSE, not as a copy of the stored vector: case 2 v3[i]
   = v1[i] + a[i]; then v2[i] = v1[i] + a[i]; case 3 v4[i] = v1[i] + a[i]; v3[i] = ..; v1[i] = ..; (301 ->
   206, frame 0x190 = the original's). The [L50] slot is the z CSE temp: c2fpu.py shows temp#.. x / y OK on
   the FPU stack, z NO (its live range does not nest with the v1 / v4 pieces), so it lives in memory (fst
   [L50], copied with mov). Left: (1) operand order of the y / z sums (original case 2: fld a1; fadd [v1y]
   but fld [v1z]; fadd a2; case 3: v1 first for both; ours a first everywhere): no lever in 30 parenthesis
   / (float) cast spellings or 40 textual swaps (internal rank); (2) slots: v1 / v2 counts 32 / 31
   (c2frame) sort them before u / v (dens 2000 / 1937 > 1750); the original has u / v at 0x10 / 0x14, so v1
   / v2 have <= 27 references there; case 1 as one expression per component (v1[i] = a[i] + b[i] * 50)
   halves the slot hunks (106 -> 62) but adds fpu (224 ins). Next: which spelling has 5 fewer v1 and 4
   fewer v2 references with case 1's code. prior: this session (no variant kept, tree as found, 301 ins):
   the gv_FireShots argument order (x, cost) is right: the original pushes +0x6c (cost) as the 8th argument
   and +0x70 (x) as the 7th; the M8 at +0x52/+0x58 is only the load order (cost into edi, spilled to
   [esp+0x54]; x in ebp). Case 1 matches but for slots. Cases 2/3 (original, locals after the 4 pushes): v1
   = b*40 keeps x on the FPU (never stored), stores y right after its fmul (fstp [L1c]) before z is loaded,
   then x + a.x as fadd [a] on that product (ours fld a; fadd st(2), keeping it); the z sum is also stored
   to an extra 4-byte slot [L50] (fst) and copied as an integer into v3.z (case 2) / v1.z (case 3): that
   slot is the original's 4 extra frame bytes (0x190 vs 0x18c); in case 2 the d*-5 products x/y stay on the
   FPU (only z stored), i.e. case 2's product vector is not address-taken there (dead stores dropped) while
   cases 1/3 pass ... */

int ai_FireCannon(AIPER *p, AIBLD *b)
{
    float u, v;
    float v1[4], v2[4], v3[4], v4[4];
    float f, c, g;
    char s1[104], s2[104], s3[104];
    int cost, x;
    const G9BALL *bl;

    if (ai_CannonShot.who == p->netId && ai_CannonShot.bld == b->id) {
        gv_CannonScatter(b, &u, &v);
        g = gv_CannonItemFactor(b);
        cost = ai_CannonShot.cost;
        x = ai_CannonShot.x;
        bl = &ai_CannonBalls[ai_CannonShot.kind];
        c = bl->c;
        f = (g * bl->b - bl->a) * (ai_CannonShot.k * 0.01f) + bl->a;
        if (ai_CannonShot.charge == 1) {
            v1[0] = ai_CannonShot.b[0] * 50.0f;
            v1[1] = ai_CannonShot.b[1] * 50.0f;
            v1[2] = ai_CannonShot.b[2] * 50.0f;
            v1[0] = ai_CannonShot.a[0] + v1[0];
            v1[1] = ai_CannonShot.a[1] + v1[1];
            v1[2] = ai_CannonShot.a[2] + v1[2];
            ai_InitShot(s1, v1, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_ScatterShot(s1, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            gv_FireShots(b, p, s1, 0, 0, gv_SalvoCharge(ai_CannonShot.kind, ai_CannonShot.k, 1), x, cost);
            return 1;
        }
        if (ai_CannonShot.charge == 2) {
            v1[0] = ai_CannonShot.b[0] * 40.0f;
            v1[1] = ai_CannonShot.b[1] * 40.0f;
            v1[2] = ai_CannonShot.b[2] * 40.0f;
            v3[0] = v1[0] + ai_CannonShot.a[0];
            v3[1] = v1[1] + ai_CannonShot.a[1];
            v3[2] = v1[2] + ai_CannonShot.a[2];
            v2[0] = v1[0] + ai_CannonShot.a[0];
            v2[1] = v1[1] + ai_CannonShot.a[1];
            v2[2] = v1[2] + ai_CannonShot.a[2];
            v1[0] = ai_CannonShot.d[0] * 5.0f;
            v1[1] = ai_CannonShot.d[1] * 5.0f;
            v1[2] = ai_CannonShot.d[2] * 5.0f;
            v2[0] = v1[0] + v2[0];
            v2[1] = v1[1] + v2[1];
            v2[2] = v1[2] + v2[2];
            v1[0] = ai_CannonShot.d[0] * -5.0f;
            v1[1] = ai_CannonShot.d[1] * -5.0f;
            v1[2] = ai_CannonShot.d[2] * -5.0f;
            v3[0] = v1[0] + v3[0];
            v3[1] = v1[1] + v3[1];
            v3[2] = v1[2] + v3[2];
            ai_InitShot(s2, v2, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_InitShot(s1, v3, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_ScatterShot(s2, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            ai_ScatterShot(s1, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            gv_FireShots(b, p, s2, s1, 0, gv_SalvoCharge(ai_CannonShot.kind, ai_CannonShot.k, 2), x, cost);
            return 1;
        }
        if (ai_CannonShot.charge == 3) {
            v1[0] = ai_CannonShot.b[0] * 40.0f;
            v1[1] = ai_CannonShot.b[1] * 40.0f;
            v1[2] = ai_CannonShot.b[2] * 40.0f;
            v4[0] = v1[0] + ai_CannonShot.a[0];
            v4[1] = v1[1] + ai_CannonShot.a[1];
            v4[2] = v1[2] + ai_CannonShot.a[2];
            v3[0] = v1[0] + ai_CannonShot.a[0];
            v3[1] = v1[1] + ai_CannonShot.a[1];
            v3[2] = v1[2] + ai_CannonShot.a[2];
            v1[0] = v1[0] + ai_CannonShot.a[0];
            v1[1] = v1[1] + ai_CannonShot.a[1];
            v1[2] = v1[2] + ai_CannonShot.a[2];
            v2[0] = ai_CannonShot.c[0] * 5.0f;
            v2[1] = ai_CannonShot.c[1] * 5.0f;
            v2[2] = ai_CannonShot.c[2] * 5.0f;
            v1[0] = v2[0] + v1[0];
            v1[1] = v2[1] + v1[1];
            v1[2] = v2[2] + v1[2];
            v2[0] = ai_CannonShot.c[0] * -3.0f;
            v2[1] = ai_CannonShot.c[1] * -3.0f;
            v2[2] = ai_CannonShot.c[2] * -3.0f;
            v4[0] = v2[0] + v4[0];
            v4[1] = v2[1] + v4[1];
            v4[2] = v2[2] + v4[2];
            v3[0] = v2[0] + v3[0];
            v3[1] = v2[1] + v3[1];
            v3[2] = v2[2] + v3[2];
            v2[0] = ai_CannonShot.d[0] * 5.0f;
            v2[1] = ai_CannonShot.d[1] * 5.0f;
            v2[2] = ai_CannonShot.d[2] * 5.0f;
            v4[0] = v2[0] + v4[0];
            v4[1] = v2[1] + v4[1];
            v4[2] = v2[2] + v4[2];
            v2[0] = ai_CannonShot.d[0] * -5.0f;
            v2[1] = ai_CannonShot.d[1] * -5.0f;
            v2[2] = ai_CannonShot.d[2] * -5.0f;
            v3[0] = v2[0] + v3[0];
            v3[1] = v2[1] + v3[1];
            v3[2] = v2[2] + v3[2];
            ai_InitShot(s3, v4, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_InitShot(s1, v3, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_InitShot(s2, v1, ai_CannonShot.b, f, c, ai_IronDensity);
            ai_ScatterShot(s3, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            ai_ScatterShot(s1, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            ai_ScatterShot(s2, u, ai_CannonShot.d, u, ai_CannonShot.c, v);
            gv_FireShots(b, p, s3, s1, s2, gv_SalvoCharge(ai_CannonShot.kind, ai_CannonShot.k, 3), x, cost);
            return 1;
        }
    }
    return 0;
}
