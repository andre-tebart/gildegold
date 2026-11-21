/* amr.c -- the AI actions (tag "am_RegisterMethod(): Methode '%s' ..." 0x5ffde8), 0x492fd0..0x4a3a90: the random
   duel opponent and the dice duel (ag_Pairs / ag_Chance), the torture instruments, the action table loader,
   am_RegisterMethod, the registration of actions 0x01..0x3b (0x4a16f0) and their decide (int, character, flag, out1,
   out2 -> action kind or 0), execute (character, arg1, arg2) and evaluate callbacks. A unit of its own by its .rdata:
   am.c's literals 0x5ecec0..0x5ecf18 (first users 0x48e231..0x49271d, in am.c's function order) precede this unit's
   named constants ag_Pairs 0x5ecf18, ag_Chance, ai_TortureInstruments (named constants come first in an object), and
   its literals 0x5ed048..0x5ed100 are first used from 0x493283 on. It has no plain .data block: its strings follow
   am.c's (from 0x5ffdb4, first user 0x494f52) and end before amt_ai's block 0x60034c. The am_ tag is in this unit;
   the name am stays with the first part (it owns am.h). Names are ours.
   unit 0x00492fd0..0x004a3a90, split from am; boundaries in decl/text.dcl */
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "am.h"
#include "amr.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "cm_types.h"
/* ai_DecideBuildBusiness (0x498c70, byte) tests flags1c8 & AI_CHF_BUILT: the original's
   `test dword [edi+0x1c8], 0x1000000` at 0x498fc3. 0x1000000 is an absolute constant (as are
   0xffffff / 0xff0000) and must not be written as symbol arithmetic: `&txt_Keys + 0x181c0` spells
   it only while txt_Keys keeps its original address 0xfe7e40; our link puts it at 0x6d0448, so the
   sum was 0x6e8608 and the test read an unrelated bit. */

#define AI_REPAIR_KINDS 0x7c43b4  /* the building kinds ai_DecideRepairBuilding repairs (one bit per AIBTYPE.kind) */

#include "hst.h"
#include "poker.h"
#include "mission.h"
extern int ai_PlayerCountYear;
extern int ai_NumPlayerChars;
AGCH ai_DuelOpponent = {0};  /* 0x6f0478 */
extern char ai_DataPath[608];
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */
char ai_ActionIniPath[260];  /* 0xf83da0, the ini file of the actions */
int ai_EvalDepth;  /* 0xf83ea4, the depth of the evaluation stack */
AIACT ai_Actions[0x49];  /* 0xf83ec0, the registered actions (0x49 of them: the init clears 0x49 * 0x94 bytes) */
/* ai_Unkf84c40: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84c60: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84cd4: view of ai_Actions (tools/data_views.py) */
/* ai_Unkf84cf4: view of ai_Actions (tools/data_views.py) */
int ai_EvalStack[73];  /* 0xf86900, the action ids on the evaluation stack (73 deep) */

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
#include "amt_ai.h"
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define AI_CHAIN_BEATUP 0xf00500  /* the partner-chain flags (ai_PickPartnerChain) of action 0x18 "Verpruegeln" (ai_DecideBeatUp); the bits are not named yet */
#define AI_CHAIN_USEREPUTATION 0xf00500  /* the partner-chain flags of action 0x1c "UseObject (Ansehen)" (ai_DecideUseReputationObject), as AI_CHAIN_BEATUP; the bits are not named yet */
#define AI_CHAIN_MEAN 0xf00500  /* the partner-chain flags of action 0x21 "UseObject (Gemeinheit)" (ai_DecideUseMeanObject; | 0x800 for the good 0x29a, | 0x10000 for 0x177), as AI_CHAIN_BEATUP */
#define AI_CHAIN_SPY 0xf00500  /* the partner-chain flags of action 0x25 (spying, 0x49c570), as AI_CHAIN_BEATUP */
#define AI_CHF_BUILT 0x1000000  /* flags1c8 bit an execute callback sets after it built a building (ai_ExecBuildBuilding); ai_DecideBuildBusiness tests it */
#define AI_CHF_STAMMTISCH 0x2000000  /* flags1c8 bit ai_ExecuteStammtisch sets after a regulars' table decision; ai_DecideStammtisch skips a character that has it */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the four player slots */

/* ---- 0x492fd0..0x494020 (was unk_490830.c): a random character generator, a dice duel between two
   characters (struct AGDUEL), two helpers that pick a character for a message ---- */
/* 0x5ecf18, the pairs (a, b) the duel accepts for a side (9 records) */
static const int ag_Pairs[9][2] = {
    {0, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}
};

/* 0x5ecf60, five rows of cumulative chances */
static const float ag_Chance[5][6] = {
    {0.2f, 0.3787f, 0.5444f, 0.711f, 0.8999f, 1.0f},
    {0.19f, 0.3565f, 0.5222f, 0.6898f, 0.8666f, 1.0f},
    {0.1678f, 0.3343f, 0.5f, 0.6676f, 0.8444f, 1.0f},
    {0.1456f, 0.311f, 0.4888f, 0.6442f, 0.8222f, 1.0f},
    {0.1234f, 0.2998f, 0.4666f, 0.622f, 0.8111f, 1.0f}
};
const AIREC10 ai_TortureInstruments[7] = { { 0, 10, 20.0f, 40.0f }, { 1, 13, 40.0f, 60.0f }, { 2, 16, 60.0f, 80.0f }, { 3, 19, 80.0f, 100.0f }, { 4, 23, 100.0f, 120.0f }, { 5, 27, 120.0f, 140.0f }, { 6, 32, 140.0f, 160.0f } };  /* 0x5ecfd8 */

/* 0x492fd0, makes the random duel opponent ai_DuelOpponent for the duel kind k (1, 2, 4): draws a class 1..5 from the cumulative table row of k and a second class from a row near it, gives the opponent a random sex and first name and five stats in the range of the first class; returns (second class << 8) | first class */
unsigned int ai_MakeDuelOpponent(unsigned int k)
{
    float t[15] = {0.2f, 0.55f, 0.85f, 0.95f, 1.0f, 0.1f, 0.4f, 0.7f, 0.9f, 1.0f, 0.05f, 0.25f, 0.55f, 0.8f, 1.0f};
    float r;
    int idx, i, base, mul;
    unsigned int v, ret, cls;
    char c;

    r = util_RandomF();
    if (k == 1)
        idx = 0;
    else
        idx = (k != 2) + 1;
    /* @original: cls and ret (and base / mul in the switch) are set only on a hit; every row ends at 1.0f,
       so the draws always hit. VC6 packs them into the dead k slot */
    for (i = 0; i < 5; i++) {
        if (r <= t[idx * 5 + i]) {
            cls = i + 1;
            break;
        }
    }
    if (idx == 1)
        v = util_Random(3);
    else {
        if (idx == 0)
            v = util_Random(2);
        else
            v = idx - util_Random(2);
    }
    r = util_RandomF();
    for (i = 0; i < 5; i++) {
        if (r <= t[v * 5 + i]) {
            ret = (i + 1) * 0x100;
            break;
        }
    }
    memset(&ai_DuelOpponent, 0, sizeof(ai_DuelOpponent));
    ai_DuelOpponent.sex = (unsigned char)util_Random(2);
    if (ai_DuelOpponent.sex == 0)
        strncpy(ai_DuelOpponent.name, txt_MaleFirstNames[util_Random(0xbf)], 15);
    else
        strncpy(ai_DuelOpponent.name, txt_FemaleFirstNames[util_Random(0x70)], 15);
    switch (cls) {
    case 1:
        base = 0x15;
        mul = 0x2a;
        break;
    case 2:
        base = 0x15;
        mul = 0x3f;
        break;
    case 3:
        base = 0x3f;
        mul = 0x3f;
        break;
    case 4:
        base = 0x69;
        mul = 0x3f;
        break;
    case 5:
        base = 0xa8;
        mul = 0x54;
        break;
    }
    for (i = 0; i < 5; i++) {
        c = (char)util_Random(mul);
        ai_DuelOpponent.stat[i] = c + (char)base + 1;
    }
    ai_DuelOpponent.kind = 10;
    return ret | cls;
}
/* 0x493200, starts a duel between p and a fresh opponent of kind k (1, 2 or 4): the opponent
   (ai_MakeDuelOpponent), the stake and its raise from p's worth (gm_PlayerFactor), the starting side
   from the player record's first bias, phase 1; 0 for another character kind or a bad k */

int ai_DuelStart(AGCH *p, AGDUEL *d, int k)
{
    AGFIN *fin;
    unsigned int stats;
    int m, r;
    double x;
    float old;

    fin = (AGFIN *)gm_PlayerRecord((AICHAR *)p);
    if (p == 0 || p->kind != 6 || d == 0 || (k != 1 && k != 2 && k != 4))
        return 0;
    d->stake = 0;
    stats = ai_MakeDuelOpponent(k);
    d->total = 0;
    m = gm_ConvertMoney(gm_GetWorth(p->id), ai_OwnPlayer);
    x = m;
    if (10000.0 > x)
        x = 10000.0;
    r = gm_PlayerFactor((int)((x / (log10(x) + 1.0) * 0.10000000149011612) * (k * 0.25f)), ai_OwnPlayer);
    d->stake = r;
    d->raiseAmount = (int)(r * 0.1f);
    d->kind1 = 0;
    d->reserved2c = 0;
    d->kind2 = 0;
    d->reserved50 = 0;
    if (fin->firstBias < util_RandomF()) {
        d->side1 = p;
        d->stats1 = 0xffff;
        d->side2 = &ai_DuelOpponent;
        d->stats2 = stats;
        fin->firstBias = util_RandomF() * 0.11f + fin->firstBias + 0.2f;
    } else {
    d->side2 = p;
    d->stats2 = 0xffff;
    d->side1 = &ai_DuelOpponent;
    d->stats1 = stats;
    old = fin->firstBias;
    fin->firstBias = (old - 0.2f) - util_RandomF() * 0.11f;
    }
    d->phase = 1;
    return 1;
}
/* 0x493390, 1 when (the kind of side c, k) is a pair the duel accepts */

int ai_DuelAcceptAction(AGDUEL *d, AGCH *c, int k)
{
    int v;
    const int *q;

    if (c == d->side1)
        v = d->kind1;
    else if (c == d->side2)
        v = d->kind2;
    else
        return 0;
    q = &ag_Pairs[0][1];
    while (q[-1] != v || *q != k) {
        q += 2;
        if ((int)q >= (int)&ag_Pairs[9][1])
            return 0;
    }
    return 1;
}
/* 0x4933d0, accepts the action k for side c as ai_DuelAcceptAction does and sets it as the side's kind (kind1 / kind2) */

int ai_DuelSetAction(AGDUEL *d, AGCH *c, int k)
{
    int *q;
    int i;

    if (c == d->side1)
        q = &d->kind1;
    else if (c == d->side2)
        q = &d->kind2;
    else
        return 0;
    i = 0;
    while (ag_Pairs[i][0] != *q || ag_Pairs[i][1] != k) {
        i++;
        if (i >= 9)
            return 0;
    }
    *q = ag_Pairs[i][1];
    return 1;
}
/* 0x493420, a throw of the side c in the duel d: k != 0 throws three dice (faces from the ag_Chance row of skill 2, then a skill bonus of 0..3 pips added to one, the lowest or each die by a random way, unless all three are equal); k == 0 adds one die, its face from the row plus -1..1 by skill 0 and the sum so far, kept in 1..6. As in the original, t / n are not set when c is
   neither side, and w when no face row entry is reached (VC6 reads their stack homes there). */

void ai_DuelThrow(AGDUEL *d, AGCH *c, int k)
{
    unsigned char *t, *n, lvl;
    float f, r, roll;
    int i, j, sum, w, m, best, a, b, g, extra, h;
    short sv;

    if (c == d->side1) {
        t = d->t1;
        n = &d->n1;
    } else if (c == d->side2) {
        t = d->t2;
        n = &d->n2;
    }
    f = gm_GetSkill(c, 2);
    lvl = (int)(f * 100.0f) / 20;
    if (lvl >= 4)
        lvl = 4;
    if (k != 0) {
        h = util_Random(3);
        *n = 3;
        if (f > 0.8888f) {
            sv = util_Random(0x10);
            extra = sv ? 2 : 3;
        } else if (f > 0.5555f) {
            sv = util_Random(0x10);
            extra = sv ? 1 : 2;
        } else if (f > 0.3333f) {
            sv = util_Random(4);
            extra = sv ? 0 : 1;
        } else {
            sv = util_Random(8);
            extra = sv ? 0 : 1;
        }
        roll = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (roll <= ag_Chance[lvl][i]) {
                a = i + 1;
                break;
            }
        }
        roll = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (roll <= ag_Chance[lvl][i]) {
                b = i + 1;
                break;
            }
        }
        roll = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (roll <= ag_Chance[lvl][i]) {
                g = i + 1;
                break;
            }
        }
        t[1] = (unsigned char)b;
        t[0] = (unsigned char)a;
        t[2] = (unsigned char)g;
        if (a == b && a == g)
            return;
        if (extra == 0)
            return;
        if (h == 0) {
            for (i = 0; i < 3; i++, extra--) {
                if (extra == 0)
                    return;
                t[i] = (unsigned char)__min(6, t[i] + 1);
            }
            return;
        }
        if (h == 1) {
            best = -1;
            m = 6;
            for (i = 0; i < 3; i++) {
                if (m > t[i]) {
                    best = i;
                    m = t[i];
                }
            }
            if (best <= -1)
                return;
            t[best] = (unsigned char)__min(6, t[best] + extra);
            return;
        }
        j = util_Random(3);
        t[j] = (unsigned char)__min(6, t[j] + extra);
        return;
    }
    f = gm_GetSkill(c, 0);
    r = util_RandomF();
    i = 0;
    sum = 0;
    for (; i < *n; i++)
        sum += t[i];
    if (f > 0.66) {
        if (sum > 12)
            best = -1;
        else if (sum < 6)
            best = 1;
        else
            best = util_Random(3) - 1;
    } else if (f > 0.44) {
        if (sum > 13)
            best = -1;
        else if (sum < 5)
            best = 1;
        else
            best = util_Random(3) - 1;
    } else {
        if (sum > 14)
            best = -1;
        else if (sum < 4)
            best = 1;
        else
            best = util_Random(3) - 1;
    }
    for (i = 0; i < 6; i++) {
        if (r <= ag_Chance[lvl][i]) {
            w = __max(__min(best + i + 1, 6), 1);
            break;
        }
    }
    t[*n] = (unsigned char)w;
    (*n)++;
}
/* 0x4937a0, side c acts on the duel with k: 1 pays the stake and throws three dice, 2 pays the raise and adds a die, 3 pays the raise only, 4 only records the kind; 0 when the duel rejects the action */

int ai_DuelAct(AGDUEL *d, AGCH *c, int k)
{
    if (ai_DuelAcceptAction(d, c, k) == 0)
        return 0;
    switch (k) {
    case 1:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->stake);
        d->total += d->stake;
        ai_DuelThrow(d, c, 1);
        ai_DuelSetAction(d, c, k);
        return 1;
    case 2:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->raiseAmount);
        d->total += d->raiseAmount;
        ai_DuelThrow(d, c, 0);
        ai_DuelSetAction(d, c, k);
        return 1;
    case 3:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->raiseAmount);
        d->total += d->raiseAmount;
        ai_DuelSetAction(d, c, k);
        return 1;
    case 4:
        ai_DuelSetAction(d, c, k);
        return 1;
    default:
        return 0;
    }
}
/* 0x493890, the next action of the AI duel opponent c (kind 10) for ai_DuelAct: state 0 throws (1); states 1 / 2 take one more die (2) while the points short of 17 are 6 or more, else stop (3) or another die by a chance from the shortfall and its class; state 3 answers 3 or 4 by its sum against the other side's dice; 0 for any other side or state */

int ai_DuelOpponentAction(AGDUEL *d, AGCH *c)
{
    unsigned char b1, b2;
    unsigned char *t;
    int u, me, i, sum, s;
    float f;

    if (c->kind == 10) {
        if (c == d->side1) {
            s = d->kind1;
            u = d->stats1 & 0xff;
            b1 = d->n1;
            t = d->t1;
            b2 = d->n2;
            me = 0;
        } else {
            if (c != d->side2)
                return 0;
            s = d->kind2;
            u = d->stats2 & 0xff;
            b1 = d->n2;
            t = d->t2;
            b2 = d->n1;
            me = 1;
        }
        for (i = 0, sum = 0; i < b1; i++)
            sum += t[i];
        switch (s) {
        case 0:
            return 1;
        case 1:
        case 2:
            sum = 0x11 - sum;
            if (sum >= 6)
                return 2;
            if (sum == 0)
                return 3;
            f = ((1.0f - u * 0.2f) * 0.25f + 1.0f) * (sum * 0.16666667f);
            if (f > util_RandomF())
                return 2;
            break;
        case 3:
            f = (sum + me) * 0.05882353f;
            if (f >= 1.0f)
                return 3;
            if (6 - u >= f - b2 * 3.5f)
                break;
            return 4;
        default:
            return 0;
        }
        return 3;
    }
    return 0;
}
/* 0x493a40, advances the duel to its next phase: phase 2 compares the sides' triples, phases 3 and 4 the dice sums (over 17 or kind 4 ends the side's turn) */

void ai_DuelNextPhase(AGDUEL *d)
{
    int ph;
    int i;
    int s1 = 0;
    int s2 = 0;
    int sum1 = 0;
    int sum2 = 0;

    ph = d->phase;
    if (ph == 1) {
        d->phase = 2;
        return;
    }
    if (ph == 2) {
        if (d->t1[0] == d->t1[1] && d->t1[0] == d->t1[2])
            s1 = d->t1[2] + d->t1[1] + d->t1[0];
        if (d->t2[0] == d->t2[1] && d->t2[0] == d->t2[2])
            s2 = d->t2[2] + d->t2[1] + d->t2[0];
        if (s1 > s2) {
            d->phase = 5;
            return;
        }
        if (s2 > 0) {
            d->phase = 6;
            return;
        }
        if (d->kind1 == 4) {
            d->phase = 6;
            return;
        }
        d->phase = d->kind2 == 4 ? 5 : 3;
        return;
    } else {
        for (i = 0; i < d->n1; i++)
            sum1 += d->t1[i];
        for (i = 0; i < d->n2; i++)
            sum2 += d->t2[i];
        if (ph == 3) {
            if (sum1 > 17 || d->kind1 == 4) {
                d->phase = 6;
                return;
            }
            if (d->kind1 == 3 && d->kind2 == 3) {
                d->phase = (sum1 <= sum2) + 5;
                return;
            }
            d->phase = 4;
            return;
        }
        if (ph == 4) {
            if (sum2 > 17 || d->kind2 == 4) {
                d->phase = 5;
                return;
            }
            if (d->kind1 == 3 && d->kind2 == 3) {
                d->phase = (sum1 <= sum2) + 5;
                return;
            }
            d->phase = 3;
        }
        return;
    }
}
/* 0x493be0, sets object o's target (field 0x1e) for its owner p: an office holder (sel 7) picks a colleague of his office column he dislikes, sel 10 a partner from the random walk; without one the target is cleared to -1 (always for sel 3 / 4, else two times in three) */

int ai_ChooseObjectTarget(AGCH *p, AIOBJ *o)
{
    AMTREC offices[6];
    AMTKIND kind;
    AGCH *q;
    int r, n, cnt, i;
    char v;

    q = 0;
    r = util_Random(3);
    if (p == 0 || o == 0 || p->sel == 0 || p->money < 2)
        return 0;
    if (p->sel == 7) {
        float liking;

        if (p->officeKind == 0)
            goto release;
        if (amt_GetKind(p->officeKind, &kind) == 0)
            return 0;
        cnt = amt_GetColumnOffices(kind.col, 6, offices);
        if (cnt == 0)
            return 0;
        i = util_Random(cnt);
        n = cnt;
        do {
            if (p->netId != offices[i].owner && (q = AG_CH(offices[i].owner)) != 0) {
                liking = gm_Liking(p->id, q->id, 1);
                if (liking < util_Random(30) + 40.0f)
                    break;
                q = 0;
            }
            n--;
            i = (i + 1) % cnt;
        } while (n != 0);
    } else if (p->sel == 10) {
        int flags;
        unsigned short id;

        flags = 0xc00;
        if (ai_FindPartnersRandomWalk((AIPER *)p, 1, &flags, 0.0f, util_Random(30) + 40.0f, &id) == 0)
            goto release;
        q = (AGCH *)&ai_CharsT[id];
    } else
        goto release;
    if (q != 0) {
        v = 1;
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &q->netId);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = (char)util_Random(8);
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        v = (char)util_Random(8);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
        return 1;
    }
release:
    if (p->sel != 3 && p->sel != 4 && r == 0)
        return 0;
    {
        int target;

        v = 1;
        target = -1;  /* no target */
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &target);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = -1;
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
    }
    return 1;
}
/* 0x493ea0, the successor of p for the kill-player event (he_RunKillPlayer): an alive adult (above 11) child of
   p's children (relatives[3..7]) picked at random, else p's spouse for a kind other than 5 (a living, adult
   kind 1..9 without the flag 0x40000); 0 when none, and for a kind 5 p with children but no candidate the
   spouse's and the children's links to p are cleared in the field messages */

AGCH *ai_FindSuccessor(AGCH *p)
{
    AGCH *list[5];
    AGCH *q, *s, *found;
    int n, m, i, k;
    int *lp;

    found = 0;
    m = 0;
    n = 0;
    lp = p->children;
    for (i = 0; i < 5; i++, lp++) {
        q = AG_CH(*lp);
        if (q != 0) {
            n++;
            if (q->health > 0 && q->age > 11)
                list[m++] = q;
        }
    }
    if (m != 0)
        found = list[util_Random(m)];
    if (p->kind == 5) {
        if (found != 0)
            return found;
        if (n != 0) {
            int neg;

            q = AG_CH(p->spouse);
            neg = -1;
            if (q != 0) {
                cm_BeginFieldBlock(q, q->netId);
                cm_AddFieldDelta(4, 1, 0x5c, &neg);
                cm_SendFieldDeltas();
            }
            for (i = 0; i < 5; i++) {
                s = AG_CH(p->children[i]);
                if (s != 0) {
                    if (s->parent1 == p->netId)
                        k = 1;
                    else if (s->parent2 != p->netId)
                        continue;
                    else
                        k = 2;
                    cm_BeginFieldBlock(s, s->netId);
                    cm_AddFieldDelta(4, 1, k * 4 + 0x5c, &neg);
                    cm_SendFieldDeltas();
                }
            }
        }
    } else {
        q = AG_CH(p->spouse);
        if (q != 0 && q->health > 0 && q->kind > 1 && q->kind < 10 && (q->flags1c8 & 0x40000) == 0)
            return q;
        if (found != 0 && found->kind > 1 && found->kind < 10)
            return found;
    }
    return 0;
}

/* ---- 0x494020..0x494e70 (was unk_494020.c): decision / rating callbacks that compare two characters
   (closeness, rank, random rolls), a plan's tries, pair and group ratings ---- */

/* 0x494020, decides whether plan q is worth it for the character a (c18): score 3 or below no, 18 or above
   yes, else the character's life + health against a limit of ai_PlanLevel times 190 / 90 / 70 by its
   mood[11][0], then its liking against c1c's and c14's, and a coin toss */
int ai_PlanWorth(AIPLAN *q)
{
    AIC4 *a, *b, *c;
    unsigned int n;
    float lim, x, d, sum, rank;

    if (q->score <= 3.0f)
        return 0;
    if (q->score >= 18.0f)
        return 1;
    n = ai_PlanLevel(q);
    a = AI_C4(q->c18);
    b = AI_C4(q->c1c);
    c = AI_C4(q->c14);
    sum = (float)a->health + a->life;
    rank = a->mood[11][0] - 2.0f;
    if (rank > 0.66f)
        lim = (float)(n * 190);
    else if (rank > 0.33f)
        lim = (float)(n * 90);
    else
        lim = (float)(n * 70);
    if (lim > sum)
        return 1;
    x = gm_Liking(a->id, b->id, 1) * 0.01f;
    d = gm_Liking(c->id, b->id, 1) * 0.01f;
    if (x * 1.1f > d * 0.9f && x < d)
        return 0;
    return util_Random(2);
}
/* 0x4941b0, the AI accuser's answer to the trial's torture question (request kind 1 of the trial's ask-AI callback
   cm_TrialAskAI): a coin toss, 1 to torture the accused */

int ai_DecideTorture(AIPLAN *q)
{
    return util_Random(2) != 0;
}
/* 0x4941d0, the AI accuser's pick of the torture instrument (request kind 2 of cm_TrialAskAI): a random 0 .. 6, the
   index of the seven instruments (plan.instrument) */

int ai_DecideTortureInstrument(AIPLAN *q)
{
    return util_Random(7);
}
/* 0x4941e0, the torturer's mercy in the trial (request kind 3 of the trial's ask-AI callback cm_TrialAskAI):
   the torturer (c28) gives in (1) when he likes the accused (c18) by more than 20 over the accuser (c14), goes on
   (0) when he likes the accuser by more than 20 more, and tosses a coin between; with no torturer 1 */

int ai_DecideTortureMercy(AIPLAN *q)
{
    AIC4 *a, *b, *c;
    float x, y;

    c = AI_C4(q->c28);
    if (!c)
        return 1;
    a = AI_C4(q->c18);
    b = AI_C4(q->c14);
    x = gm_Liking(a->id, c->id, 1);
    y = gm_Liking(b->id, c->id, 1);
    if (x - y > 20.0f)
        return 1;
    if (y - x > 20.0f)
        return 0;
    return util_Random(2);
}
/* 0x4942a0, a level from the score: score / 4, clamped to 2..6 */

int ai_PlanLevel(AIPLAN *q)
{
    return (int)min(max(q->score * 0.25f, 2.0f), 6.0f);
}
/* 0x494300, rolls the plan's tries on the record of q->instrument: each try's amount v = base + spread * a random
   (halved when q->answer is set) adds up; a total reaching q->amount (a quarter of the time reduced by 25%) or a try
   above 0.33 of it (shrinking 20% a try) succeeds; q->penalty is the negated total */

int ai_PlanRollTries(AIPLAN *q)
{
    AIREC10 *t;
    float f, lim, scale, acc, v, sum;
    int i;

    q->success = 0;
    t = ai_FindInstrumentRecord(q->instrument);
    f = q->amount;
    lim = f * 0.33f;
    i = 0;
    acc = 0.0f;
    for (; i < q->tries; i++) {
        scale = 0.5f;
        if (q->answer == 0)
            scale = 1.0f;
        v = cm_RandomFloat() * t->spread * scale + t->base;
        sum = acc + v;
        if (sum >= f) {
            acc = v + acc;
            if (cm_Random(4))
                acc = acc * 0.75;
            q->success = 1;
            break;
        }
        if (v > lim) {
            acc = acc + lim;
            q->success = 1;
            break;
        }
        lim = lim * 0.8f;
        acc = sum;
    }
    q->penalty = -acc;
    return q->success;
}
/* 0x494410, the vote of a trial judge or assessor (request kind 4 of the trial's ask-AI callback cm_TrialAskAI):
   1 for the accused when his liking of and mix with the voter (c18) outweigh the accuser's (c14) plus the plan's
   score^2 * 0.0025, 0 otherwise; the accused's item 0x22e tips it and a torture raises his side, while a confession
   (success) or a guilty plea gives no vote for him */

int ai_DecideAcquittal(AIC4 *ch, AIPLAN *q)
{
    AIC4 *a, *b;
    float x, y, c, d;

    if (q->success != 1 && q->plea != 1) {
        a = AI_C4(q->c18);
        b = AI_C4(q->c14);
        x = gm_Liking(a->id, ch->id, 1) * 0.01f;
        y = gm_Liking(b->id, ch->id, 1) * 0.01f;
        if (ai_InventoryHas(a, 0x22e)) {
            x = x + 0.1f;
            y = y - 0.1f;
        }
        x = min(max(0.0f, x), 1.0f);
        y = min(max(0.0f, y), 1.0f);
        c = gm_Mix(a, ch, 4);
        d = gm_Mix(b, ch, 4);
        d = d + q->score * q->score * 0.0025f;
        if (q->tortured) {
            x = x * 1.1f;
            y = y * 0.9f;
        }
        if (c + x > d + y)
            return 1;
    }
    return 0;
}
/* 0x4945c0, the rank (0..7) of the plan's score rounded minus one: 4..5 -> 1, 6..7 -> 2 .. 14..16 -> 6, more -> 7; one less
   for a character with flag 0x22e
   @deep deep s34: two hunks. (1) regs: the original pops n (or r) before q: n / r esi, q edi. c2trace: q#1
   18 (block 0, P 3, three refs), r#1 17 (seven case stores at P 1, -1 through the ai_InventoryHas block,
   r-- and return), n#1 16 (def +6, n++ +2, switch +8); tie keys n 1c > q 1b, r 3f > q 1b, so n +2 or r +1
   or q -2 decides it. flip.py --want 'mov edi, _q': only constant changes keep the count (0x22e -> 7, r =
   1 -> 0, the clamp's 0 -> 1: a constant used twice becomes a candidate and raises P of r's blocks) - no
   ordinary form found yet. A float local t = q->score gives the registers (q 2 refs) but the x87 code
   differs (fpu 14: the original reads q->score twice). = control: n reused as the rank, the clamp into a
   named r / k, n = n < 0 ? 0 : n, n >= 0 ? n : 0, n = n + 1, r = r - 1, a break after default; worse: if
   (n < 0) n = 0 (17 / 98 bytes), n > 0 ? n : 0, return r - 1 / ?: forms (22), c first or between (M8), fl
   first, fl double (M10), (float)floor. (2) order: fstp fl after push edx in the original (both before
   gm_FindChar); unchanged by the t local. climb round 1: only the int t (meaning change). prior: read
   against the original: same statements, same switch (tables equal), same calls and constants; renamed
   AIPLAN.unk04 to score. Left is allocation: esi/edi swapped (original q in edi, n in esi; ours q in esi,
   n in edi) which moves 'mov reg,[esp+arg]' after 'push edi' in the original, plus the fstp [esp+0x20]
   scheduled after 'push edx' (0x4945f3..0x4945f7). Same instruction count. */

int ai_PlanRank(AIPLAN *q)
{
    int n, r;
    float fl;
    AIC4 *c;

    n = (int)(q->score - 1.0f);
    fl = (float)(q->score - floor(q->score));
    c = AI_C4(q->c18);
    if (fl >= 0.5f)
        n++;
    switch (n < 0 ? 0 : n) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 0;
    case 4:
    case 5:
        r = 1;
        break;
    case 6:
    case 7:
        r = 2;
        break;
    case 8:
    case 9:
        r = 3;
        break;
    case 10:
    case 11:
        r = 4;
        break;
    case 12:
    case 13:
        r = 5;
        break;
    case 14:
    case 15:
    case 16:
        r = 6;
        break;
    default:
        r = 7;
    }
    if (ai_InventoryHas(c, 0x22e))
        r--;
    return r;
}
/* 0x4946c0, the sentence a trial judge picks (request kind 5 of the trial's ask-AI callback cm_TrialAskAI): the
   plan's rank raised by 0 / 1 / 2 by the judge's liking of the accused against thresholds scaled by record 0's
   current law value (k = 0.7 + (5 - cur) * 0.1), 5 -> 4 for a judge without an office, 7 -> 6 under rank 2,
   clamped to 0 .. 9 */

int ai_DecideSentence(AIC4 *ch, AIPLAN *q)
{
    int n;
    float k, d;
    AIDEC dec;
    AIC4 *c;

    n = ai_PlanRank(q);
    gv_GetTableRecord(0, (int *)&dec);
    k = (5.0f - (float)dec.cur) * 0.1f + 0.7f;
    c = AI_C4(q->c18);
    d = gm_Liking(c->id, ch->id, 1);
    if (!(d > k * 66.666f)) {
        if (d > k * 33.333f)
            n += 1;
        else
            n += 2;
    }
    if (n == 5) {
        if (c->officeKind == 0)
            n = 4;
    } else if (n == 7 && c->rank < 2)
        n = 6;
    n = n <= 0 ? 0 : n;
    if (n >= 9)
        n = 9;
    return n;
}
/* 0x4947a0, a rating from the wealth of two characters: x / (log10 x + 1) * 0.1 * 32, x at least 10000 */

int ai_WealthRating(AIC4 *a, AIC4 *b)
{
    int x, y;
    double d;

    x = gm_GetWorth(a->id);
    y = gm_GetWorth(b->id);
    d = gm_ConvertMoney(y, ai_OwnPlayer) + gm_ConvertMoney(x, ai_OwnPlayer);
    if (10000.0 > d)
        d = 10000.0;
    return (int)((d / (log10(d) + 1.0) * 0.1f) * 32.0);
}
/* 0x494830, the same with 0.05 */

int ai_WealthRatingHalf(AIC4 *a, AIC4 *b)
{
    int x, y;
    double d;

    x = gm_GetWorth(a->id);
    y = gm_GetWorth(b->id);
    d = gm_ConvertMoney(y, ai_OwnPlayer) + gm_ConvertMoney(x, ai_OwnPlayer);
    if (10000.0 > d)
        d = 10000.0;
    return (int)((d / (log10(d) + 1.0) * 0.05f) * 32.0);
}
/* 0x4948c0, the record of the torture-instrument table ai_TortureInstruments with this key */

AIREC10 *ai_FindInstrumentRecord(int key)
{
    int i;

    for (i = 0; i < 7; i++)
        if (ai_TortureInstruments[i].key == key)
            return &ai_TortureInstruments[i];
    return 0;
}
/* 0x4948f0, rolls the three study offers (price, two subject indexes) of a character for the university: the price
   is its worth * 0.025 split by the map distance from player slot 0's place to slots 1 .. 3, whose offer it is;
   the first subject is fixed (4, 3, 2) and the second one of three (0 / 3 / 1, 1 / 2 / 0, 0 / 4 / 1) */

void ai_RollStudyOffers(AIC4 *ch, int *out)
{
    int t[3][4] = {{4, 0, 3, 1}, {3, 1, 2, 0}, {2, 0, 4, 1}};
    float f[3];
    float m;
    int i, k, v;

    v = gm_GetWorth(ch->id);
    k = (int)(v * 0.025f);
    m = 0.0f;
    for (i = 0; i < 3; i++) {
        f[i] = gm_MapDistance(ai_PlayerSlotsT[0].mapX, ai_PlayerSlotsT[0].mapY, ai_PlayerSlotsT[i + 1].mapX,
                              ai_PlayerSlotsT[i + 1].mapY);
        if (f[i] > m)
            m = f[i];
        out[i * 3 + 1] = t[i][0];
        out[i * 3 + 2] = t[i][util_Random(3) + 1];
    }
    for (i = 0; i < 3; i++) {
        f[i] = 1.0f / m * f[i];
        out[i * 3] = (int)(k * f[i]);
    }
}
/* 0x494a30, whether the examinee b passes the guild master a's master exam (the event he_RunFeastExam): no when
   the master's partner is the examinee, else 1 when the examinee's liking plus 2.5 per level he is above the
   master reaches a random * 75 */

int ai_MasterExamPassed(AIC4 *a, AIC4 *b)
{
    float t, s;
    int x, y;

    if (a->netId == (unsigned int)b->partner)
        return 0;
    x = gm_CharLevel(a->id, 0);
    y = gm_CharLevel(b->id, 0);
    t = (float)(y - x) * 2.5f;
    s = gm_Liking(a->id, b->id, 1) + t;
    if (s < util_RandomF() * 75.0f)
        return 0;
    return 1;
}
/* 0x494ac0, evaluates a feast: a host and up to four guests. Each guest's relation d (-2 .. 2) to the
   host is the host's level less a third and a quarter plus (5 - the menu pick) + (5 - the musician
   pick), less the guest's level, clamped to -2 .. 2; when send is set the relation change goes out
   (cm_SendRelations, a random strength by d). The feast's cost (menu and musicians 0 .. 5: the texts
   _MN_FEST_ESSEN / _MN_FEST_MUSIKER, the feedback's %T) goes to g->cost. Returns 0, or -1 .. -6 for a
   bad group */

int ai_EvalFest(AIGRP *g, int send)
{
    int i, lv, base, d, v, w, menuPenalty, musicPenalty;

    if (!g)
        return -1;
    if (!g->leader)
        return -2;
    if (g->essen > 5)
        return -3;
    if (g->musik > 5)
        return -4;
    if (g->n == 0 || g->n > 4)
        return -5;
    musicPenalty = 5 - g->musik;
    menuPenalty = 5 - g->essen;
    lv = gm_CharLevel(g->leader->id, 0);
    base = lv - lv / 3 - lv / 4 + menuPenalty + musicPenalty;
    for (i = 0; i < g->n; i++) {
        if (!g->mem[i].ch)
            return -6;
        d = gm_CharLevel(g->mem[i].ch->id, 0);
        g->mem[i].d = max(min(base - d, 2), -2);
    }
    for (i = 0; i < g->n; i++) {
        switch (g->mem[i].d) {
        case -2:
            v = -0x10 - util_Random(0x10);
            break;
        case -1:
            v = -8 - util_Random(8);
            break;
        case 0:
            v = util_Random(8) + 8;
            break;
        case 1:
            v = util_Random(0x10) + 0x10;
            break;
        case 2:
            v = util_Random(0x10) + 0x18;
        }
        if (send)
            cm_SendRelations(g->leader->netId, g->mem[i].ch->netId, v, 1);
    }
    w = gm_GetWorth(g->leader->id);
    g->cost = (int)((float)(10 - g->musik - g->essen) * w * g->n * 0.001f);
    if (send)
        cm_SendMoveGoods(-1, g->leader->netId, ai_OwnPlayer, g->cost);
    return 0;
}
/* 0x494d40, the favor (Gunst) a payment of n buys the payer a with the target b: (a's level - b's
   level + 10) * n, the relation change cm_SendRelations carries (the bewitch window's "Gunst",
   ai_BewitchOrder) */

int ai_RelationGain(AIC4 *payer, AIC4 *target, int n)
{
    int payerLevel, targetLevel;

    payerLevel = gm_CharLevel(payer->id, 0);
    targetLevel = gm_CharLevel(target->id, 0);
    return (int)((float)((payerLevel - targetLevel + 10) * n * 3) * 0.33333334f);
}
/* 0x494d90, whether the attacker a wins the brawl against b: the mixed skill of the two (gm_Mix 3) plus
   a random roll, less 0.25 while b's order 0xd holds or plus 0.1 for b's order 0xa (the order that
   applied goes to *out, 0x3b for none); 1 when the score passes 1.0 */

int ai_BrawlWin(AIC4 *attacker, AIC4 *defender, int *out)
{
    float sum, modifier;
    int appliedOrder;
    int msg[0x88];

    sum = gm_Mix(attacker, defender, 3) + util_RandomF();
    modifier = 0.0f;
    appliedOrder = 0x3b;
    memset(msg, 0, sizeof(msg));
    msg[0] = 1;
    if (ord_DispatchOrder(0xd, defender, msg) & 2) {
        modifier = -0.25f;
        appliedOrder = 0xd;
    } else if (ord_DispatchOrder(10, defender, msg) & 2) {
        modifier = 0.1f;
        appliedOrder = 10;
    }
    if (out)
        *out = appliedOrder;
    if (sum + modifier > 1.0f)
        return 1;
    return 0;
}
/* the action table file is written and read field by field; a failed transfer closes the file and fails */
#define AM_WRITE(p, size) if (vfs_fwrite(p, size, 1, f) <= 0) { vfs_fclose(f); return 0; }
#define AM_READ(p, size) if (vfs_fread(p, size, 1, f) <= 0) { vfs_fclose(f); return 0; }

/* 0x494e70, loads the action table from /gamedata/ai/ai_data_2.dfn (or, with ai_WriteActionTable set, registers
   the actions from ai_method_addon.ini and writes the table) */
int ai_LoadActionTable(void)
{
    unsigned int n;
    int hdr[2];
    G5ACT r;
    unsigned int i;
    int k;

    n = 0x49;
    memset(ai_Actions, 0, 0x49 * sizeof(G5ACT));
    hdr[0] = 0x6144466e;
    if (ai_WriteActionTable != 0) {
        void *f;

        hdr[1] = 0x49;
        GetModuleFileNameA(main_Instance, ai_ActionIniPath, 0x104);
        strrchr(ai_ActionIniPath, 0x5c)[1] = 0;
        strcat(ai_ActionIniPath, "ai_method_addon.ini");
        if (ai_RegisterBaseActions(ai_WriteActionTable) == 0)
            return 0;
        if (ai_RegisterGraveActions(ai_WriteActionTable) == 0)
            return 0;
        if (ai_RegisterBuildingActions(ai_WriteActionTable) == 0)
            return 0;
        if (ai_RegisterSocialActions(ai_WriteActionTable) == 0)
            return 0;
        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "wb", vfs_Root);
        if (f == 0)
            return 0;
        AM_WRITE(&hdr[0], 4);
        AM_WRITE(&hdr[1], 4);
        for (k = 0; k < 0x49; k++) {
            AM_WRITE(&G5_ACTS[k].id, 1);
            AM_WRITE(G5_ACTS[k].name, 0x20);
            AM_WRITE(&G5_ACTS[k].w1[0].k, 1);
            AM_WRITE(&G5_ACTS[k].w1[0].w, 4);
            AM_WRITE(&G5_ACTS[k].w1[1].k, 1);
            AM_WRITE(&G5_ACTS[k].w1[1].w, 4);
            AM_WRITE(&G5_ACTS[k].w1[2].k, 1);
            AM_WRITE(&G5_ACTS[k].w1[2].w, 4);
            AM_WRITE(&G5_ACTS[k].w1[3].k, 1);
            AM_WRITE(&G5_ACTS[k].w1[3].w, 4);
            AM_WRITE(&G5_ACTS[k].w2[0].k, 1);
            AM_WRITE(&G5_ACTS[k].w2[0].w, 4);
            AM_WRITE(&G5_ACTS[k].w2[1].k, 1);
            AM_WRITE(&G5_ACTS[k].w2[1].w, 4);
            AM_WRITE(&G5_ACTS[k].w2[2].k, 1);
            AM_WRITE(&G5_ACTS[k].w2[2].w, 4);
            AM_WRITE(&G5_ACTS[k].w2[3].k, 1);
            AM_WRITE(&G5_ACTS[k].w2[3].w, 4);
        }
        vfs_fclose(f);
        return 1;
    } else {
        void *f;

        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "rb", vfs_Root);
        if (f == 0)
            return 0;
        if (ai_RegisterBaseActions(ai_WriteActionTable) == 0 || ai_RegisterGraveActions(ai_WriteActionTable) == 0 ||
            ai_RegisterBuildingActions(ai_WriteActionTable) == 0 || ai_RegisterSocialActions(ai_WriteActionTable) == 0) {
            vfs_fclose(f);
            return 0;
        }
        AM_READ(hdr, 4);
        if (hdr[0] != 0x6144466e)
            return 0;
        AM_READ(&n, 4);
        for (i = 0; i < n; i++) {
            AM_READ(&r.id, 1);
            AM_READ(r.name, 0x20);
            AM_READ(&r.w1[0].k, 1);
            AM_READ(&r.w1[0].w, 4);
            AM_READ(&r.w1[1].k, 1);
            AM_READ(&r.w1[1].w, 4);
            AM_READ(&r.w1[2].k, 1);
            AM_READ(&r.w1[2].w, 4);
            AM_READ(&r.w1[3].k, 1);
            AM_READ(&r.w1[3].w, 4);
            AM_READ(&r.w2[0].k, 1);
            AM_READ(&r.w2[0].w, 4);
            AM_READ(&r.w2[1].k, 1);
            AM_READ(&r.w2[1].w, 4);
            AM_READ(&r.w2[2].k, 1);
            AM_READ(&r.w2[2].w, 4);
            AM_READ(&r.w2[3].k, 1);
            AM_READ(&r.w2[3].w, 4);
            memmove(r.w3, r.w1, 0x20);
            if (r.id < 0x49) {
                G5_ACTS[r.id].id = r.id;
                strcpy(G5_ACTS[r.id].name, r.name);
                memcpy(G5_ACTS[r.id].w1, r.w1, 0x20);
                memcpy(G5_ACTS[r.id].w2, r.w2, 0x20);
                memcpy(G5_ACTS[r.id].w3, r.w3, 0x20);
            }
        }
        vfs_fclose(f);
        return 1;
    }
}
#undef AM_WRITE
#undef AM_READ
/* 0x495520, registers an action into ai_Actions: its short / long desire weights come from ai_method_addon.ini when ini is set, else they are cleared */

int am_RegisterMethod(AIACT *a, int ini)
{
    G5ACT *act = (G5ACT *)a;
    char key[64];
    char val[128];
    int i, found;

    if (act->id > 0 && act->id < 0x49) {
        if (ini != 0) {
            for (i = 0; i < 4; i++) {
                sprintf(key, "ShortDesire%li", i + 1);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_ActionIniPath) != 0) {
                    act->w1[i].k = (unsigned char)ai_GetDesireByName(val);
                    sprintf(key, "ShortChange%li", i + 1);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_ActionIniPath);
                    act->w1[i].w = (float)atof(val);
                } else {
                    act->w1[i].k = 0xff;
                    act->w1[i].w = 0.0f;
                }
                sprintf(key, "LongDesire%li", i + 1);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_ActionIniPath) != 0) {
                    act->w2[i].k = (unsigned char)ai_GetDesireByName(val);
                    sprintf(key, "LongChange%li", i + 1);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_ActionIniPath);
                    act->w2[i].w = (float)atof(val);
                } else {
                    act->w2[i].k = 0xff;
                    act->w2[i].w = 0.0f;
                }
            }
            /* an action without any need it changes is reported (the message is only formatted);
               the compare with -1 of the zero-extended kind is always true, as in the original */
            found = 0;
            for (i = 0; !found && i < 4; i++) {
                if ((act->w1[i].k != -1 && act->w1[i].w != 0.0f) ||
                    (act->w2[i].k != -1 && act->w2[i].w != 0.0f))
                    found = 1;
            }
            if (!found)
                sprintf(val, "am_RegisterMethod(): Methode '%s' hat keinerlei Auswirkungen!!!", act->name);
            memmove(act->w3, act->w1, 0x20);
        } else {
            for (i = 0; i < 4; i++) {
                act->w1[i].k = 0xff;
                act->w1[i].w = 0.0f;
                act->w2[i].k = 0xff;
                act->w2[i].w = 0.0f;
                act->w3[i].k = 0xff;
                act->w3[i].w = 0.0f;
            }
        }
        G5_ACTS[act->id] = *act;
    }
    return 1;
}
/* 0x495770, pushes an action onto the evaluation stack */

void ai_PushAction(int k)
{
    ai_EvalStack[ai_EvalDepth] = k;
    ai_EvalDepth++;
}
/* 0x495790, pops the evaluation stack */

void ai_PopAction(void)
{
    ai_EvalDepth--;
}
/* 0x4957a0, whether an action is on the evaluation stack */

int ai_ActionOnStack(int k)
{
    int i;

    for (i = 0; i < ai_EvalDepth; i++) {
        if (ai_EvalStack[i] == k)
            return 1;
    }
    return 0;
}
/* 0x4957d0, chooses the next action of character i: the best short-term and the best long-term rating */

int ai_ChooseAction(int i)
{
    float best1, best2, a, b;
    int k;
    unsigned int id2, id1;
    int r1, r2, r;
    AIARG s2, s1, b2s1, b1s2, b1s1, b2s2;
    G5ACT *act;
    G5CH *p;

    p = &G5_CHARS[i];
    best1 = -1e30f;
    best2 = -1e30f;
    id1 = 0;
    id2 = 0;
    r1 = 0;
    r2 = 0;
    b1s1.kind = 0;
    b2s1.kind = 0;
    b1s2.kind = 0;
    b2s2.kind = 0;
    for (k = 0; k < 0x49; k++) {
        act = &G5_ACTS[k];
        if (act->id != 0 && act->id != p->act && (p->mask & act->flags) != 0) {
            s1.kind = 0;
            s1.ref = 0;
            s1.arg = 0;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = act->eval != 0 ? act->eval(&a, &b, k, p, 0, &s1, &s2) : 0;
            /* @original: decide is tested twice; VC6 drops the repeated test, but the third reference bases
               the action pointer on &act->decide as in the original */
            if (r != 0 && (a >= best1 || b >= best2) && act->decide != 0)
                r = act->decide != 0 ? act->decide(0, p, 0, &s1, &s2) : 0;
            else
                r = 0;
            if (r != 0) {
                if (a >= best1) {
                    b1s1 = s1;
                    best1 = a;
                    id1 = act->id;
                    b1s2 = s2;
                    r1 = r;
                }
                if (b >= best2) {
                    best2 = b;
                    id2 = act->id;
                    b2s1 = s1;
                    b2s2 = s2;
                    r2 = r;
                }
            }
        }
    }
    if (best1 != 0.0f && best1 >= best2) {
        p->prev = (unsigned char)id1;
        p->arg1 = b1s1;
        p->act = (unsigned char)r1;
        p->arg2 = b1s2;
    } else {
        p->prev = (unsigned char)id2;
        p->arg1 = b2s1;
        p->act = (unsigned char)r2;
        p->arg2 = b2s2;
    }
    return p->act;
}
/* 0x495ce0, executes the current action of character i and applies its need weights */

int ai_ExecuteAction(int i)
{
    G5CH *p = &G5_CHARS[i];
    unsigned char msg;
    int k;

    if (p->act != 0 && G5_ACTS[p->act].exec != 0 && G5_ACTS[p->act].exec(p, &p->arg1, &p->arg2) != 0) {
        ai_BeginNeedBlock((struct AICHAR *)p);
        for (k = 0; k < 4; k++) {
            if (G5_ACTS[p->prev].w1[k].k < 14)
                ai_AddNeedDelta(G5_ACTS[p->prev].w1[k].k, G5_ACTS[p->prev].w1[k].w * -0.5f);
        }
        for (k = 0; k < 4; k++) {
            if (G5_ACTS[p->act].w1[k].k < 14)
                ai_AddNeedDelta(G5_ACTS[p->act].w3[k].k, G5_ACTS[p->act].w3[k].w * -0.5f);
        }
        if (p->prev == p->act) {
            ai_AddNeedDelta(13, 500.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)p->partner) {
                msg = 2;
                cm_BeginFieldBlock((AIPER *)p, p->netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        } else {
            ai_AddNeedDelta(13, 250.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)p->partner) {
                msg = 1;
                cm_BeginFieldBlock((AIPER *)p, p->netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        }
        ai_SendNeedBlock();
        memmove(G5_ACTS[p->act].w3, G5_ACTS[p->act].w1, 0x20);
        return 1;
    }
    if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)p->partner) {
        msg = 3;
        cm_BeginFieldBlock((AIPER *)p, p->netId);
        cm_AddFieldValues(1, 1, 0x215, &msg);
        cm_SendFieldDeltas();
    }
    return 0;
}

/* ---- 0x495ee0..0x497490 ---- */

/* 0x495ee0, rolls the daily flags of every kind-3 character: bit 1 of flags1e8 (0x1e8) is always set,
   bits 2, 4, 8 and 0x10 are drawn at random, and the word is sent to the players (cm_BeginFieldBlock /
   cm_AddFieldDelta); bits 2, 4 and 8 gate that character's leisure actions (am.c 0x4960b0, 0x4965b0) */
void ai_RollCharFlags(void)
{
    AIPER *c;
    unsigned int f;
    int i;

    f = 1;
    for (i = 0; i < 768; i++) {
        c = (AIPER *)&ai_CharsT[i];
        if (c->kind == 3) {
            f |= util_Random(2) ? 2 : 0;
            f |= util_Random(3) ? 4 : 0;
            f |= util_Random(3) ? 8 : 0;
            f |= util_Random(2) ? 0x10 : 0;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(4, 1, 0x1e8, &f);
            cm_SendFieldDeltas();
        }
    }
}
/* 0x495fa0, decide of the dummy action "DummyRechtschaffenheit" (6): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 6, else 0 */

int ai_DecideDummyRighteousness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 6;
}
/* 0x495fc0, execute of the dummy action "DummyRechtschaffenheit" (6): returns the action id 6, no other effect */

int ai_ExecuteDummyRighteousness(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 6;
}
/* 0x495fd0, decide of the dummy action "DummySicherheit" (7): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 7, else 0 */

int ai_DecideDummySafety(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 7;
}
/* 0x495ff0, execute of the dummy action "DummySicherheit" (7): returns the action id 7, no other effect */

int ai_ExecuteDummySafety(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 7;
}
/* 0x496000, decide of the dummy action "DummyFortpflanzung" (8): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 8, else 0 */

int ai_DecideDummyProcreation(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 8;
}
/* 0x496020, execute of the dummy action "DummyFortpflanzung" (8): returns the action id 8, no other effect */

int ai_ExecuteDummyProcreation(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 8;
}
/* 0x496030, decide of the dummy action "DummyUnversehrtheit" (1): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 1, else 0 */

int ai_DecideDummyHealth(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b == 0;
}
/* 0x496050, decide of the dummy action "DummyBeruf" (2): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 2, else 0 */

int ai_DecideDummyJob(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 2;
}
/* 0x496070, execute of the dummy action "DummyBeruf" (2): returns the action id 2, no other effect */

int ai_ExecuteDummyJob(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 2;
}
/* 0x496080, decide of the dummy action "DummyVergnuegen" (3): with no sub-action running (a == 0)
   and b == 0 it offers its own action id 3, else 0 */

int ai_DecideDummyPleasure(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 3;
}
/* 0x4960a0, execute of the dummy action "DummyVergnuegen" (3): returns the action id 3, no other effect */

int ai_ExecuteDummyPleasure(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 3;
}
/* 0x4960b0, decide of action 4: a random affordable skill below 252 to train (o1 kind 0xe, ref the skill, arg2 the price), else first the building it needs */

int ai_DecideTrainSkill(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int count, k, v, id, r;

    count = 5;
    ids[0] = 0x3a;
    ids[1] = 0x1a;
    ids[2] = 0x17;
    ids[3] = 0x18;
    ids[4] = 0x19;
    if (b)
        return 0;
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.ref = 0;
        s1.arg = 0;
        s1.fvalue = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_ChooseSubAction(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    if (o1->kind == 0xe) {
        memset(q, 0, sizeof(q));
        k = o1->ref;
        q[0] = 1;
        f = (ord_DispatchOrder(ids[k], p, q) & 2) ? -1.0f : 0.0f;
        if (p->skills[k] < 42)
            v = 1;
        else if (p->skills[k] < 84)
            v = 2;
        else if (p->skills[k] < 126)
            v = 4;
        else if (p->skills[k] < 168)
            v = 6;
        else if (p->skills[k] < 210)
            v = 9;
        else
            v = 12;
        v = (int)(v + f);
        if (v < p->money)
            return 0;
        if (p->skills[k] == 252.0f)
            return 0;
        o1->arg2 = v;
        return 4;
    }
    for (i = 0; i < 5; i++) {
        memset(q, 0, sizeof(q));
        q[0] = 1;
        valid[i] = 1;
        cnt[i] = ord_GetRatingByte(i);
        if (!cnt[i])
            cnt[i] = 252;
        f = (ord_DispatchOrder(ids[i], p, q) & 2) ? -1.0f : 0.0f;
        if (p->skills[i] < 42)
            n[i] = 1;
        else if (p->skills[i] < 84)
            n[i] = 2;
        else if (p->skills[i] < 126)
            n[i] = 4;
        else if (p->skills[i] < 168)
            n[i] = 6;
        else if (p->skills[i] < 210)
            n[i] = 9;
        else
            n[i] = 12;
        n[i] = (int)(n[i] + f);
        if (n[i] > p->money) {
            valid[i] = 0;
            count--;
        } else {
            fb = p->skills[i];
            if (fb == 252.0f) {
                valid[i] = 0;
                count--;
            } else
                w[i] = fb / cnt[i];
        }
    }
    if (!count)
        return 0;
    k = util_Random(5);
    while (!valid[k])
        k = (k + 1) % 5;
    if (p->kind == 3) {
        if (k == 4) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (k == 3) {
            if (!(p->flags1e8 & 8))
                return 0;
        } else if (k == 2 && !(p->flags1e8 & 2))
            return 0;
    } else if (k > 1) {
        if (k == 4)
            id = 0x15;
        else
            id = k == 3 ? 0x17 : 0x14;
        if (!gm_Query(p->bld2->objList, 1, 0, id)) {
            AIARG s1, s2;

            s1.kind = 2;
            s1.ref = id;
            s1.arg = 1;
            s1.fvalue = 0.44f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_ChooseSubAction(4, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    }
    o1->kind = 0xe;
    o1->ref = k;
    o1->arg2 = n[k];
    return 4;
}
/* 0x4965b0, execute of 0x4960b0 */

int ai_ExecuteTrainSkill(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->arg2);
    m.flags = 1;
    m.type = 0x60;
    m.to = p->netId;
    m.fromId = -1;
    m.skill = (unsigned char)s1->ref;
    cm_SendLongMessage((AIMSG *)&m);
    return 4;
}
/* 0x496610, decide of action 5: one of the two skills its job rates highest (gm_GetJobTalents), affordable and below 252, to train (o1 kind 0xe, ref the skill, arg2 the price); else the building it needs, or the sub-actions 0xe (no job) / 0xf (none affordable) */

int ai_DecideTrainJobSkill(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    JOBTALENTS jt;
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int best, second, ka, kb, k, v, id, r, sum;


    best = 0;
    second = 0;
    ids[0] = 0x3a;
    ids[1] = 0x1a;
    ids[2] = 0x17;
    ids[3] = 0x18;
    ids[4] = 0x19;
    if (b)
        return 0;
    if (p->jobId == 0) {
        AIARG s1, s2;

        s2.kind = 0;
        s1.kind = 0xe;
        s1.ref = 0;
        s1.arg = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_ChooseSubAction(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    jt = gm_GetJobTalents(p->jobId);
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.ref = 0;
        s1.arg = 0;
        s1.fvalue = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_ChooseSubAction(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    ka = util_Random(5);
    kb = util_Random(5);
    for (i = 0; i < 5; i++) {
        memset(q, 0, sizeof(q));
        v = jt.skills[i];
        q[0] = 1;
        if (v > best) {
            if (best > 0) {
                kb = ka;
                second = best;
            }
            ka = i;
            best = v;
        } else if (v > second) {
            kb = i;
            second = v;
        }
        valid[i] = 1;
        cnt[i] = ord_GetRatingByte(i);
        if (!cnt[i])
            cnt[i] = 252;
        f = (ord_DispatchOrder(ids[i], p, q) & 2) ? -1.0f : 0.0f;
        if (p->skills[i] < 42)
            n[i] = 1;
        else if (p->skills[i] < 84)
            n[i] = 2;
        else if (p->skills[i] < 126)
            n[i] = 4;
        else if (p->skills[i] < 168)
            n[i] = 6;
        else if (p->skills[i] < 210)
            n[i] = 9;
        else
            n[i] = 12;
        n[i] = (int)(n[i] + f);
        if (n[i] > p->money)
            valid[i] = 0;
        else {
            fb = p->skills[i];
            if (fb == 252.0f)
                valid[i] = 0;
            else
                w[i] = fb / cnt[i];
        }
    }
    sum = valid[kb] + valid[ka];
    if (sum == 0) {
        AIARG s1, s2;

        s1.kind = 0xf;
        s1.ref = 0;
        s1.arg = 0;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_ChooseSubAction(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    k = ka;
    if (sum == 2) {
        if (!util_Random(2))
            k = kb;
    } else if (!valid[ka])
        k = kb;
    if (p->kind == 3) {
        if (k == 4) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (k == 3) {
            if (!(p->flags1e8 & 8))
                return 0;
        } else if (k == 2 && !(p->flags1e8 & 2))
            return 0;
    } else if (k > 1) {
        if (k == 4)
            id = 0x15;
        else
            id = k == 3 ? 0x17 : 0x14;
        if (!gm_Query(p->bld2->objList, 1, 0, id)) {
            AIARG s1, s2;

            s1.kind = 2;
            s1.ref = id;
            s1.arg = 1;
            s1.fvalue = 0.44f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_ChooseSubAction(5, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    }
    o1->kind = 0xe;
    o1->ref = k;
    o1->arg2 = n[k];
    return 5;
}
/* 0x496ab0, execute of 0x496610 */

int ai_ExecuteTrainJobSkill(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->arg2);
    m.to = p->netId;
    m.fromId = p->netId;
    m.flags = 1;
    m.type = 0x60;
    m.skill = (unsigned char)s1->ref;
    cm_SendLongMessage((AIMSG *)&m);
    return 5;
}
/* 0x496b10, evaluate: without an office application, the least liked (gm_Liking) of the three characters +0xb4 / +0xb8 / +0xbc of the character's event 0x2c (no judge at +0xb4) becomes the target (o1 kind 7) and the action's weights count against the moods; no such event or office kind 0xd: the guilt or mood weights */

int ai_EvalLeastLikedOfEvent(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;
    AIACT *act;
    AIJOBX *job;
    AIPER *c[3];
    float dist[3], best;
    int x, y, j, bi;

    if (f) {
        *a = -1.0e30f;
        *b = -1.0e30f;
        return 0;
    }
    act = &ai_Actions[i];
    *a = 0.0f;
    *b = 0.0f;
    /* @original the gotos: as guards with one shared fallback the original places the ai_EvalMoodWeights
       return in front of the epilogue and keeps i in ebx; the nested if / flag forms do not */
    if (p->appliedOffice)
        goto weigh;
    if (p->officeKind == 0xd)
        goto fallback;
    x = ai_JudgeCatholic ? ai_JudgeCatholic->netId : -1;
    y = ai_JudgeProtestant ? ai_JudgeProtestant->netId : -1;
    for (job = (AIJOBX *)he_FindEvent(1, 0, 0x2c); job; job = (AIJOBX *)he_NextEvent())
        if (job->partner == p->netId)
            break;
    if (!job || job->guests[0] == x || job->guests[0] == y)
        goto fallback;
    dist[0] = 100.0f;
    dist[1] = 100.0f;
    dist[2] = 100.0f;
    best = 100.0f;
    bi = -1;
    c[0] = gm_FindPer(job->guests[0]);
    c[1] = gm_FindPer(job->guests[1]);
    c[2] = gm_FindPer(job->guests[2]);
    if (c[0] && c[0] != AI_PARTNER)
        dist[0] = gm_Liking(p->id, c[0]->id, 1);
    if (c[1] && c[1] != AI_PARTNER)
        dist[1] = gm_Liking(p->id, c[1]->id, 1);
    if (c[2] && c[2] != AI_PARTNER)
        dist[2] = gm_Liking(p->id, c[2]->id, 1);
    for (j = 0; j < 3; j++) {
        if (dist[j] < best) {
            bi = j;
            best = dist[j];
        }
    }
    if (bi == -1)
        goto fallback;
    o1->kind = 7;
    o1->ref = c[bi]->netId;
    o1->arg = 1;
weigh:
    for (j = 0; j < 4; j++) {
        if (act->w1[j].w > 0.0f) {
            *a += act->w1[j].k == p->sel ? p->mood[act->w1[j].k][2] * act->w1[j].w * 0.02f : p->mood[act->w1[j].k][2] * act->w1[j].w * 0.01f;
        }
        if (act->w2[j].w > 0.0f) {
            *b += act->w2[j].k == p->sel ? p->mood[act->w2[j].k][2] * act->w2[j].w * 0.02f : p->mood[act->w2[j].k][2] * act->w2[j].w * 0.01f;
        }
    }
    return 1;
fallback:
    gv_GetTableRecord(16, &d);
    if (d.cur)
        return ai_EvalGuiltWeights(a, b, i, p, &d);
    return ai_EvalMoodWeights(a, b, i, p, 0, o1, o2);
}
/* 0x496e40, decide: talk a companion into money (action 9) */

int ai_DecideBribe(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIPER *list[6];
    AIPER *c;
    float g, best, hi;
    int m, lim, n, j, bi, lo, hi2, r;

    if (b)
        return 0;
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.ref = 0;
        s1.arg = 0;
        s1.fvalue = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_ChooseSubAction(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if ((p->flags1e8 & 0x400) && util_Random(8))
        return 0;
    if (p->kind == 3) {
        if (!(p->flags1e8 & 2))
            return 0;
    } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
        AIARG s1, s2;

        s1.kind = 2;
        s1.ref = 0x14;
        s1.arg = 1;
        s1.fvalue = 0.22f;
        s2.kind = 4;
        s2.ref = p->bld2->id;
        s2.arg = 1;
        r = ai_ChooseSubAction(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    m = gm_CharCurrency(p, ai_OwnPlayer);
    lim = (int)(m * 0.33f);
    if (o1->kind == 0) {
        if (p->appliedOffice) {
            bi = -1;
            best = 100.0f;
            n = amt_GetOfficeApplicants(p->appliedOffice, 6, list);
            if (n <= 0)
                return 0;
            for (j = 0; j < n; j++) {
                if (list[j] != AI_PARTNER) {
                    if ((g = gm_Liking(p->id, list[j]->id, 1)) < best) {
                        best = g;
                        bi = j;
                    }
                }
            }
            if (bi < 0)
                return 0;
            c = list[bi];
        } else {
            unsigned short cid;
            int flags;

            if (p->flags1cc & 0x1c000)
                return 0;
            flags = 0x10000500;
            if (!ai_PickPartnerChain(p, 1, &flags, 0, util_Random(25) + 10.0, &cid))
                return 0;
            c = (AIPER *)&ai_CharsT[cid];
        }
        if (!c)
            return 0;
        o1->kind = 7;
        o1->ref = c->netId;
        o1->arg = 1;
        lo = (int)max(abs(gm_GetWorth(p->id) + gm_GetWorth(c->id)) / 1275.0f, 1600.0f);
        if (lo > lim)
            return 0;
        hi2 = min(lim, lo + 320000);
        o2->kind = 0x12;
        o2->ref = 0;
        o2->arg = (int)(util_RandomF() * (hi2 - lo) + lo);
        o2->arg2 = (int)(o2->arg * 500.0f / (gm_GetWorth(p->id) + gm_GetWorth(c->id)));
        return 9;
    }
    if (o1->kind == 7) {
        c = gm_FindPer(o1->ref);
        if (!c || c == AI_PARTNER)
            return 0;
        hi2 = (int)max(lim * 0.01f, 3200.0f);
        hi = min(lim * 0.15f, 160000.0f);
        o2->kind = 0x12;
        o2->ref = 0;
        o2->arg = (int)(util_RandomF() * ((int)hi - hi2) + hi2);
        o2->arg2 = (int)(o2->arg * 500.0f / (gm_GetWorth(p->id) + gm_GetWorth(c->id)));
        return 9;
    }
    return 0;
}
/* 0x4972d0, execute of 0x496e40: pays the companion, with a message to the player */

int ai_ExecuteBribe(AIPER *p, AIARG *s1, AIARG *s2)
{
    char buf[0x400];
    AIPER *c;
    float k;

    if (s1->kind != 7)
        return 0;
    if (s2->kind != 0x12)
        return 0;
    c = gm_FindPer(s1->ref);
    if (!c)
        return 0;
    cm_SendMoveGoods(c->netId, p->netId, ai_OwnPlayer, s2->arg);
    if (p->flags1cc & 0x800)
        k = 6.0f;
    else
        k = 4.0f;
    cm_SendRelations(p->netId, c->netId, (int)((s2->arg2 + 1) * k), 1);
    cm_SendSetField(p->netId, 0x1e8, 4, 0x400, 0);
    if (c->kind == 6 || c->kind == 7) {
        txt_Format(buf, 0x1751, p->id, s2->arg);
        he_ShowNews(c->netId, -1, buf, 0, 0x5fb, "_NACHRICHTEN_HS_22");
    }
    return 9;
}
/* 0x4973f0, evaluate: the default rating, ten times for a character that held an office
   (prevOffice) and holds none now (officeKind 0) */

int ai_EvalApplyForOffice(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    float k;

    if (f) {
        *a = -1.0e30f;
        *b = -1.0e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    if (p->prevOffice && !p->officeKind)
        k = 10.0f;
    else
        k = 1.0f;
    ai_EvalMoodWeights(a, b, i, p, 0, o1, o2);
    *a *= k;
    *b *= k;
    return 1;
}
/* 0x497490, decide of action 10: below rank 2 first the sub-action 13 (o1 kind 13), else from hour 13, without an application and with 3 years left to live, apply for a random office of amt_GetOfficeCandidates (o1 kind 10, ref the office), or one time in four a leisure action (0x27) */

int ai_DecideApplyForOffice(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int list[6];
    int r, k;

    if (f == 0) {
        if (p->rank < 2) {
            AIARG s1;
            AIARG s2;

            s1.kind = 13;
            s1.ref = 2;
            s1.arg = 1;
            s1.fvalue = 0.44f;
            AI_ZERO(&s2, sizeof(s2));
            r = ai_ChooseSubAction(10, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        } else if (ai_Time.hour >= 13 && p->appliedOffice == 0 && gm_ExpectedDeathAge(p) - p->age >= 3) {
            k = amt_GetOfficeCandidates(p, 2, list);
            if (k == 0) {
                if (util_Random(4) == 0) {
                    AIARG s1;
                    AIARG s2;

                    s2.kind = 0x10;
                    s2.ref = 0x12;
                    s2.arg = 0;
                    if (ai_DecideLeisureAction(a, p, 0, &s2, &s1) != 0) {
                        *o1 = s2;
                        *o2 = s1;
                        return 0x27;
                    }
                }
            } else {
                r = list[util_Random(k) * 3];
                if (r != 0 && r <= 0x1b) {
                    o1->ref = r;
                    o1->kind = 10;
                    return 10;
                }
            }
        }
    }
    return 0;
}
/* 0x497630, execute callback of 0x497490's kind 10 */

int ai_ExecuteApplyForOffice(G5CH *p, AIARG *s1)
{
    if (s1->kind != 10)
        return 0;
    return amt_ApplyForOffice(p, s1->ref) ? 10 : 0;
}
/* 0x497660, decide of action 11: whether the good o1 (kind 2) can be made where o2 points (a store object, a building by its type's recipe list, or a character) and its owner can pay gm_GoodsPrice (11, else 0) */

int ai_DecideProduceGood(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIOBJ *obj;
    AIBLD *bld;
    AIPER *c, *owner;
    AIBTX *t;
    int i, v;
    unsigned short k;
    AIGOODX *g;

    if (b != 1 || o1->kind != 2 || AI_GOODX[o1->ref].noSale || AI_GOODX[o1->ref].type == 2)
        return 0;
    obj = 0;
    bld = 0;
    c = 0;
    if (o2->kind == 1) {
        obj = (AIOBJ *)gm_FindObjectInst(o2->ref);
        if (!obj)
            return 0;
        g = &AI_GOODX[obj->id];
        if (g->type == 2 || g->type == 6) {
            if (gm_Query(obj->list, 1, 0, o1->ref))
                return 0;
            bld = gm_FindBuildingById(obj->ownerId);
            if (!bld)
                return 0;
            t = &AI_BTX[bld->type];
            for (i = 0; i < t->n; i++)
                if ((t->k[i] & 0x7fff) == obj->id)
                    break;
            if (i >= t->n)
                return 0;
            for (i++; i < t->n; i++)
                if ((t->k[i] & 0x7fff) == o1->ref)
                    break;
            if (i >= t->n)
                return 0;
            if (t->p[i].a && !gm_Query(obj->list, 1, 0, t->p[i].a))
                return 0;
            k = t->p[i].b;
            if (k && !gm_Query(obj->list, 1, 0, k))
                return 0;
        }
    } else if (o2->kind == 4) {
        bld = gm_FindBuildingById(o2->ref);
        if (!bld)
            return 0;
        if (gm_Query(bld->objList, 1, 0, o1->ref))
            return 0;
        t = &AI_BTX[bld->type];
        for (i = 0; i < t->n; i++)
            if ((t->k[i] & 0x7fff) == o1->ref)
                break;
        if (i >= t->n)
            return 0;
        if (t->p[i].a && !gm_Query(bld->objList, 1, 0, t->p[i].a))
            return 0;
        k = t->p[i].b;
        if (k && !gm_Query(bld->objList, 1, 0, k))
            return 0;
    } else if (o2->kind == 7) {
        c = gm_FindPer(o2->ref);
        if (!c)
            return 0;
    } else
        return 0;
    if (a != 0x39) {
        if (obj)
            owner = gm_ObjectOwnerChar(obj);
        else if (bld) {
            if (bld->owner == 0xffff)
                return 0;
            owner = (AIPER *)&ai_CharsT[bld->owner];
        } else if (c)
            owner = c;
        else
            return 0;
        if (!owner)
            return 0;
        v = gm_CharCurrency(owner, ai_OwnPlayer);
        o1->arg2 = 0;
    } else {
        v = o2->arg2;
        o1->arg2 = 0x39;
    }
    if (o1->fvalue > 0.0f && o1->fvalue < 1.0f)
        v = (int)(v * o1->fvalue);
    return v < (int)gm_GoodsPrice(o1->ref, ai_OwnPlayer) ? 0 : 11;
}
/* 0x4979d0, execute of action 11 (ai_DecideProduceGood): orders the good from the object / building / character o2 names at its price (action 0x39: pays the object s1->ref2 first) */

int ai_ExecuteProduceGood(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIOBJ *o, *it;
    int id, price;

    if (s2->kind == 1) {
        o = (AIOBJ *)gm_FindObjectInst(s2->ref);
        if (!o)
            return 0;
        id = o->objId;
    } else if (s2->kind == 4) {
        AIBLD *bld = gm_FindBuildingById(s2->ref);
        if (!bld)
            return 0;
        id = bld->id;
    } else if (s2->kind == 7) {
        AIPER *c = gm_FindPer(s2->ref);
        if (!c)
            return 0;
        id = c->netId;
    } else
        id = -1;
    if (id == -1)
        return 0;
    if (s1->kind != 2)
        return 0;
    if (s1->arg2 == 0x39) {
        price = (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer);
        gm_FindById(0, (unsigned short **)&it, 0, s1->ref2);
        if (!it)
            return 0;
        cm_SendMoveGoods(-1, it->objId, ai_OwnPlayer, price);
        cm_SendExSellObject(id, -1, s1->ref, s1->arg, ai_OwnPlayer, 0);
        return 0x39;
    }
    cm_SendExSellObject(id, -1, s1->ref, s1->arg, ai_OwnPlayer, (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 11;
}
/* 0x497af0, decide: sell a good (o1, kind 2) to a building (o2, kind 4) with room for it (action 14) */

int ai_DecideBuyRoom(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIBLD *bld;
    AIBTX *t;
    AIOBJ *o;
    AIJOB *j;
    AIPER *c;
    int v, n;

    if (p->kind == 3)
        return 0;
    if (b != 1)
        return 0;
    if (o1->kind != 2)
        return 0;
    if (AI_GOODX[o1->ref].type != 2)
        return 0;
    if (o2->kind != 4)
        return 0;
    bld = gm_FindBuildingById(o2->ref);
    if (!bld)
        return 0;
    if (bld->owner == 0xffff)
        return 0;
    c = (AIPER *)&ai_CharsT[bld->owner];
    v = gm_CharCurrency(c, ai_OwnPlayer);
    if (o1->fvalue > 0.0f && o1->fvalue < 1.0f)
        v = (int)(v * o1->fvalue);
    if (v < (int)gm_GoodsPrice(o1->ref, ai_OwnPlayer))
        return 0;
    t = &AI_BTX[bld->type];
    n = 0;
    for (o = gm_Query(bld->objList, 1, 4, 2); o; o = gm_QueryNext())
        if (o->id != 0xfd)
            n++;
    if (n >= t->maxObjects)
        return 0;
    for (j = he_FindEvent(2, 0, 0x1f, 3, bld->id); j; j = he_NextEvent())
        n++;
    return n >= t->maxObjects ? 0 : 14;
}
/* 0x497c80, execute of 0x497af0 */

int ai_ExecuteBuyRoom(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bld;

    bld = gm_FindBuildingById(s2->ref);
    if (!bld)
        return 0;
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObject(bld->id, -1, s1->ref, s1->arg, ai_OwnPlayer, (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 14;
}
/* 0x497cf0, decide: renovate the character's worst building (action 15) */

int ai_DecideRepairBuilding(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIBLDX *bl, *best;
    float lim, f;
    int i, d, v;

    if (o1->kind == 0) {
        d = util_Random(0x24) + 0x23;
        best = 0;
        bl = AI_BLDX;
        for (i = 0; i < 0x100; i++, bl++) {
            if (bl->type && bl->masterId == p->id && !(bl->flags5a & 0x45)
                && ((1 << ai_BTypesT[bl->type].kind) & AI_REPAIR_KINDS) && bl->level < d) {
                d = bl->level;
                best = bl;
            }
        }
        if (!best)
            return 0;
        lim = gm_CharCurrency(p, ai_OwnPlayer) * 0.95f;
        v = d < 0x32 ? 50 : 30;
        f = ai_RepairCost(best, v);
        if (f == 0.0f || f > lim)
            return 0;
        o1->kind = 4;
        o1->ref = best->id;
        o1->arg = 1;
        o1->arg2 = v;
        o1->fvalue = f;
        o2->kind = 7;
        o2->ref = p->netId;
        o2->arg = (int)lim;
        o2->arg2 = 1;
        return 15;
    }
    if (o1->kind == 4) {
        bl = AI_BLDX;
        for (i = 0; i < 0x100; i++, bl++) {
            if (bl->type && bl->id == o1->ref) {
                if (bl->flags5a & 5)
                    return 0;
                f = ai_RepairCost(bl, o1->arg2);
                if (f == 0.0f || f > o2->arg)
                    return 0;
                o1->fvalue = f;
                return 15;
            }
        }
    }
    return 0;
}
/* 0x497ee0, execute of 0x497cf0: pays for the renovation */

int ai_ExecuteRepairBuilding(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIFINX *fin;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x1c;
    msg.to = p->netId;
    msg.fromId = -1;
    msg.flags = 1;
    msg.arg[0] = s1->ref;
    msg.arg[1] = s1->arg2;
    cm_BeginGroup("renovieren");
    cm_SendMoveGoods(-1, s2->ref, ai_OwnPlayer, (int)s1->fvalue);
    cm_SendLongMessage(&msg);
    cm_EndGroup("renovieren");
    fin = (AIFINX *)gm_PlayerRecord((AICHAR *)p);
    if (fin)
        fin->renovationSpent = (int)(fin->renovationSpent + s1->fvalue);
    return 15;
}
/* 0x497fa0, decide of action 16: buy an offered building (event 0x1b, kinds 4..7 as its orders allow, o1 kind 0 or kind 5 naming the kind) when two thirds of the money cover the price (o1 kind 4, 16), else a credit for the difference (ai_DecideTakeCredit) when the price is low against the equipment value
   @deep size queue s19: the return tails' polarity (if (o1->kind == 0) return 0; return 16; and the kind 5
   twin, its return 0 falling to the final one; 5 combinations) = control, raw 1309 each: the unmerged
   credit tail (the original's kind 5 copy jmps into the kind 0 copy's fmul) is the whole gap, as the prior
   note explains. prior: Kept: bt = &ai_BTypesT[..] between lo = 0 and hi = 0 (climb swap@27: 109 -> 63 ins
   / 401 bytes). Main hunk: the original's kind 5 credit tail jumps into the kind 0 copy at its fmul -0.5
   (jmp 0x4982c7), ours keeps two copies (432 vs 409 ins). Tail merge research (python
   tools/c2dag/c2tail.py src/am.c ai_DecideBuyBuilding --tries --il merge; notes/vc6-matching.md 'How VC6
   merges tails'): the two credit tails ARE compared by the pair merger (FUN_1074633c, both jmps to the
   exit label) and are equal for 20 instructions back to fidiv [r] (C2DAG_TRYDUMP=1: 'pair equal 20'), but
   both copies are entered by fall-through, so the merge needs a size above 22 and this x87 / call run
   measures 20 ('pair bytes 20'): no pair merge with this tail, in ours or the original. The original's
   shape (kind 5 jumps INTO the kind 0 copy, the kept copy has the epilogue) is cross-jumping (xjump, no
   size limit; its reg... */

int ai_DecideBuyBuilding(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    int ok[4];
    AIBLDX *bl;
    AIJOB *j;
    AIBTYPE *bt;
    unsigned int lo, hi;
    int i, n, lim, v, r;

    if (p->kind == 3)
        return 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (b == 1) {
        if (o1->kind != 5 && o1->kind != 4)
            return 0;
    }
    if (he_FindEvent(2, 0, 0xd, 2, p->id))
        return 0;
    bl = AI_BLDX;
    for (i = 0; i < 0x100; i++, bl++) {
        if (bl->type) {
            lo = 0;
            bt = &ai_BTypesT[bl->type];
            hi = 0;
            if ((unsigned char)bt->kind > 0x1f)
                hi = 1 << ((unsigned char)bt->kind - 0x20);
            else
                lo = 1 << (unsigned char)bt->kind;
            if (bl->owner == p->id && ((lo & ai_BuildingTypeMasks[2]) || (hi & ai_BuildingTypeMasks[3])))
                goto found;
        }
    }
    return 0;
found:
    lim = (int)(gm_CharCurrency(p, ai_OwnPlayer) * 0.66f);
    r = ord_DispatchOrder(1, p, buf);
    if (r == 2 || r == 4)
        ok[0] = 1;
    else
        ok[0] = 0;
    r = ord_DispatchOrder(5, p, buf);
    if (r == 2 || r == 4)
        ok[1] = 1;
    else
        ok[1] = 0;
    r = ord_DispatchOrder(9, p, buf);
    if (r == 2 || r == 4)
        ok[2] = 1;
    else
        ok[2] = 0;
    r = ord_DispatchOrder(0xc, p, buf);
    if (r == 2 || r == 4)
        ok[3] = 1;
    else
        ok[3] = 0;
    if (p->bld2) {
        n = AI_BTX[p->bld2->type].level;
        for (i = 0; i <= n - 1; i++)
            ok[i] = 0;
    }
    if (o1->kind == 0) {
        for (bl = (AIBLDX *)gm_BuildQuery(1, 6); o1->kind == 0 && bl; bl = AI_BUILDNEXT()) {
            if (ai_BTypesT[bl->type].kind == 2 && (bl->flags5a & 2)
                && ((bl->type == 4 && ok[0]) || (bl->type == 5 && ok[1]) || (bl->type == 6 && ok[2])
                    || (bl->type == 7 && ok[3]))) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->u.id == bl->id) {
                        v = ((AIJOBX *)j)->price;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v - lim;
                                return ai_DecideTakeCredit(0x10, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 4;
                        o1->ref = bl->id;
                        o1->arg = 1;
                        break;
                    }
                }
            }
        }
        if (o1->kind != 0)
            return 16;
        return 0;
    }
    if (o1->kind == 5) {
        if (o1->ref == 4 && !ok[0])
            return 0;
        if (o1->ref == 5 && !ok[1])
            return 0;
        if (o1->ref == 6 && !ok[2])
            return 0;
        if (o1->ref == 7 && !ok[3])
            return 0;
        for (bl = (AIBLDX *)gm_BuildQuery(1, 6); o1->kind == 5 && bl; bl = AI_BUILDNEXT()) {
            if ((unsigned char)ai_BTypesT[bl->type].kind == o1->ref) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->u.id == bl->id) {
                        v = ((AIJOBX *)j)->price;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v - lim;
                                return ai_DecideTakeCredit(0x10, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 4;
                        o1->ref = bl->id;
                        o1->arg = 1;
                        break;
                    }
                }
            }
        }
        if (o1->kind != 5)
            return 16;
        return 0;
    }
    return 0;
}
/* 0x498470, execute of action 16 (ai_DecideBuyBuilding): buys the building s1->ref at the price of its offer (event 0x1b) through gm_SellBuilding, 16; 0 without an offer */

int ai_ExecuteBuyBuilding(G5CH *p, AIARG *s1)
{
    AIJOB *j;

    if (s1->kind != 4)
        return 0;
    for (j = he_FindEvent(1, 0, 0x1b); j != 0; j = he_NextEvent()) {
        if (j->u.id == s1->ref)
            break;
    }
    if (j == 0)
        return 0;
    gm_SellBuilding(s1->ref, p->netId, ((AIJOBX *)j)->price, (AIJOBX *)j, 1);
    return 0x10;
}
/* 0x4984d0, decide callback: buy or upgrade the character's house (a building of kind 2) when it owns a building
   of the house-owner kinds (ai_BuildingTypeMasks[2..3]); short of money below a quarter of the price it decides
   to take a credit instead (called directly by 0x498c70) */

int ai_DecideBuildResidence(int a, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    int i;
    int levels[4];
    AIARG s1;
    AIARG s2;
    AIBLD *b;
    AIBLD *h;
    AIBTYPE *bt;
    unsigned int lo, hi;
    int money, cost, j;
    unsigned char t;

    levels[0] = 7;
    levels[1] = 6;
    levels[2] = 5;
    levels[3] = 4;
    if (p->kind == 3)
        return 0;
    if (f == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (f == 1 && o1->kind != 5 && o1->kind != 4) {
        return 0;
    }
    if (he_FindEvent(2, 0, 0xd, 2, p->id) != 0)
        return 0;
    b = ai_BldsT;
    for (i = 0; i < 0x100; i++, b++) {
        if (b->type != 0) {
            bt = &ai_BTypesT[b->type];
            lo = 0;
            hi = 0;
            if ((unsigned char)bt->kind > 0x1f)
                hi = 1 << ((unsigned char)bt->kind - 0x20);
            else
                lo = 1 << (unsigned char)bt->kind;
            if (b->owner == p->id && ((lo & ai_BuildingTypeMasks[2]) || (hi & ai_BuildingTypeMasks[3])))
                goto found;
        }
    }
    /* @original the not-found return 0 directly behind the loop and the match jumping past it (a break plus a
       test of i, or a found flag, lays out the shared returns differently) */
    return 0;
found:
    money = (int)(gm_CharCurrency((char *)p, ai_OwnPlayer) * 0.66f);
    s2.kind = 0;
    s2.ref = 0;
    s2.arg = 0;
    if (o1->kind == 0) {
        h = p->bld2;
        if (h != 0 && ai_BTypesT[h->type].kind == 2 && (h->flags5a & 0x41) == 0) {
            t = ai_BTypesT[h->type].cap247;
            if (t < 2 || t == 3) {
                int price;

                /* the next level of the house: levels[] runs from the top (7) down, so the type one above
                   is the entry before the house's (for type 7 the original reads the slot below the array) */
                s1.kind = 5;
                for (j = 0; levels[j] != h->type; j++)
                    ;
                s1.ref = levels[j - 1];
                s1.arg = 1;
                s2.kind = 4;
                s2.ref = h->id;
                s2.arg = 1;
                if (ai_CanBuildType(a, p, f, &s1, &s2, 0) == 0)
                    return 0;
                price = gm_BuildingKindCost((int)p, s1.ref);
                if (money < price) {
                    if ((float)(price - money) / price < 0.25f) {
                        o1->arg = price - money;
                        o1->kind = 0x12;
                        return ai_DecideTakeCredit(0x11, p, f, o1, o2);
                    }
                    return 0;
                }
                *o1 = s1;
                *o2 = s2;
                return 0x11;
            }
            if (t == 5)
                return 0;
        }
        s1.kind = 5;
        s1.ref = 6;
        s1.arg = 1;
        if (ai_CanBuildType(a, p, f, &s1, &s2, 1) != 0) {
            cost = gm_BuildingKindCost((int)p, s1.ref);
            if (money < cost) {
                if ((float)(cost - money) / cost < 0.25f) {
                    o1->arg = cost - money;
                    o1->kind = 0x12;
                    return ai_DecideTakeCredit(0x11, p, f, o1, o2);
                }
            } else {
                *o1 = s1;
                *o2 = s2;
                return 0x11;
            }
        }
        /* no house of level 6 within reach: the smallest one (type 4) */
        s1.kind = 5;
        s1.ref = 4;
        s1.arg = 1;
        if (ai_CanBuildType(a, p, f, &s1, &s2, 1) == 0)
            return 0;
        cost = gm_BuildingKindCost((int)p, s1.ref);
        if (money < cost) {
            if ((float)(cost - money) / cost < 0.25f) {
                o1->arg = cost - money;
                o1->kind = 0x12;
                return ai_DecideTakeCredit(0x11, p, f, o1, o2);
            }
            return 0;
        }
    } else {
        int price;

        if (o1->kind != 5)
            return 0;
        h = p->bld2;
        s1 = *o1;
        if (h != 0) {
            t = h->type;
            if (ai_BTypesT[h->type].kind != 2)
                return 0;
            if (o1->ref - t != 1)
                return 0;
            if (t == 5 || t == 7)
                return 0;
            s1.ref = o1->ref;
            s1.arg = 1;
            s2.kind = 4;
            s2.ref = h->id;
            s2.arg = 1;
            price = (int)(gm_BuildingKindValue(s1.ref) * 0.3f);
            if (ai_CanBuildType(a, p, f, &s1, &s2, 0) == 0)
                return 0;
            if (money < price) {
                if ((float)(price - money) / price < 0.25f) {
                    o1->kind = 0x12;
                    o1->arg = price - money;
                    return ai_DecideTakeCredit(0x11, p, f, o1, o2);
                }
                return 0;
            }
            *o1 = s1;
            *o2 = s2;
            return 0x11;
        } else {
            if (ai_BTypesT[o1->ref].kind != 2)
                return 0;
            if (ai_CanBuildType(a, p, f, &s1, &s2, 1) == 0)
                return 0;
            if (p->kind != 5)
                return 0;
            price = gm_BuildingKindCost((int)p, s1.ref);
            if (money < price) {
                if ((float)(price - money) / price < 0.25f) {
                    o1->kind = 0x12;
                    o1->arg = price - money;
                    return ai_DecideTakeCredit(0x11, p, f, o1, o2);
                }
                return 0;
            }
        }
    }
    *o1 = s1;
    *o2 = s2;
    return 0x11;
}
/* 0x498a80, execute callback of "Wohnsitz bauen" (0x11): pays 0.3 of the new type's value and sends the upgrade of the house (message group "building_upgrade"), or builds the new house (ai_ExecBuildBuilding) */

int ai_ExecuteBuildResidence(G5CH *p, AIARG *s1, AIARG *s2)
{
    AIMSG m;

    if (s2->kind == 4) {
        /* s2 is the house to upgrade (kind 4, its building id), s1 the new type */
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.fromId = s2->ref;
        m.flags = 1;
        m.time = ai_Time;
        m.arg[0] = s2->ref;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage(&m);
        cm_EndGroup("building_upgrade");
        return 0x11;
    }
    return ai_ExecBuildBuilding((AIPER *)p, s1, s2) ? 0x11 : 0;
}
/* 0x498b80, decide callback of "Wohnsitz upgraden (Credits)" (0x12): when the character's house can make a
   good it does not hold yet (ai_FindGoodToMake), chooses the sub-action 0x15 for it (s1: the good, s2: the cart
   that carries it or the house itself, weight 0.44) */

int ai_DecideUpgradeResidence(int a, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int item, cart;
    int r;

    if (p->kind == 3)
        return 0;
    if (f == 0 && o1->kind != 0)
        return 0;
    if (p->bld2 == 0 || o1->kind != 0)
        return 0;
    if (ai_FindGoodToMake(&item, &cart, p->bld2, 0) == 0)
        return 0;
    s1.kind = 2;
    s1.ref = item;
    s1.arg = 1;
    if (cart != -1) {
        s2.kind = 1;
        s2.ref = cart;
    } else {
        s2.kind = 4;
        s2.ref = p->bld2->id;
    }
    s2.arg = 1;
    s2.fvalue = 0.44f;
    r = ai_ChooseSubAction(0x15, p->id, 1, &s1, &s2);
    if (r == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x498c70, decide: build up the character's workshops (action 0x13) */

int ai_DecideBuildBusiness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG t1, t2, s1, s2;
    AIBTX *bt;
    AIBLDX *bl;
    AIJOBX *j;
    int i, tot, cnt, ones, sel, maxn, lim, k, n, v, r, d, h;

    tot = 0;
    ones = 0;
    cnt = 0;
    maxn = (p->kind == 5) + 2;
    sel = 0;
    if (p->kind == 3)
        return 0;
    if (a != 0)
        return 0;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (b == 1 && o1->kind != 5)
        return 0;
    lim = (int)(gm_CharCurrency(p, ai_OwnPlayer) * 0.66f);
    s2.kind = 0;
    s2.ref = 0;
    s2.arg = 0;
    for (i = 0; i < 0x100; i++) {
        if (AI_BLDX[i].type && AI_BLDX[i].owner == p->id) {
            bt = &AI_BTX[AI_BLDX[i].type];
            k = gm_BuildingJob(bt->kind);
            if (k > 0 && k < 0x10) {
                tot += bt->level;
                cnt++;
                if (bt->level == 1)
                    ones++;
            }
        }
    }
    if (tot && (!p->bld2 || (p->bld2->type != 7
            && (float)(AI_BTX[p->bld2->type].level - AI_BTX[4].level + 1) / tot < 0.33f))) {
        AI_ZERO(&t1, sizeof(t1));
        AI_ZERO(&t1, sizeof(t1));
        r = ai_DecideBuildResidence(0x13, p, 0, &t1, &t2);
        if (r) {
            *o1 = t1;
            *o2 = t2;
            return r;
        }
    }
    if (0 != o1->kind)
        return 0;
    for (i = 0; i < 0x100; i++) {
        bl = &AI_BLDX[i];
        if (bl->type && bl->owner == p->id && !(bl->flags5a & 0x41)) {
            bt = &AI_BTX[bl->type];
            if (bt->level < bt->maxLevel) {
                k = gm_BuildingJob(bt->kind);
                r = gm_TitleJob(p->id, k);
                if (r) {
                    n = gm_TitlesLeft(r);
                    n = n / 2 + n % 2;
                    if (n > bt->level && k != 7) {
                        int v1, d1;

                        v1 = (int)(gm_BuildingKindValue(bl->type + 1) * 0.3f);
                        if (lim < v1) {
                            if ((float)(v1 - lim) / v1 < 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v1 - lim;
                                return ai_DecideTakeCredit(0x13, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 5;
                        o1->ref = bl->type + 1;
                        o1->arg = 1;
                        o2->kind = 4;
                        o2->ref = bl->id;
                        o2->arg = 1;
                        return 0x13;
                    }
                    if (k == 7 && n > bt->level)
                        sel = bl->type + 1;
                }
            }
        }
    }
    if (p->flags1c8 & AI_CHF_BUILT)
        return 0;
    if (cnt > 3)
        return 0;
    if (ones > 0 && !sel) {
        if (ones > maxn)
            return 0;
        if (ai_Time.year % 8 != (unsigned int)p->netId % 8)
            return 0;
    }
    if (!sel) {
        sel = gm_FirstTitleNumber(p->jobId);
        if (!sel)
            return 0;
    }
    s1.kind = 5;
    s1.ref = sel;
    s1.arg = 1;
    s2.kind = 0;
    s2.ref = 0;
    s2.arg = 0;
    if (ai_CanBuildType(0, p, b, &s1, &s2, 1)) {
        int v2, d2;

        v2 = gm_BuildingKindCost(p, s1.ref);
        if (lim < v2) {
            if ((float)(v2 - lim) / v2 < 0.25f) {
                o1->kind = 0x12;
                o1->arg = v2 - lim;
                return ai_DecideTakeCredit(0, p, b, o1, o2);
            }
            return 0;
        }
    } else {
        k = gm_JobBuildingType(gm_TitleGroup(p->jobId));
        n = gm_TitlesLeft(p->jobId);
        h = n % 2 + n / 2;
        if (k == 0x10)
            h = 3;
        s1.kind = 0;
        s2.kind = 0;
        for (bl = AI_501190_3(1, 5, k); s1.kind == 0 && bl; bl = AI_BUILDNEXT()) {
            if (AI_BTX[bl->type].level <= h && (bl->flags5a & 2)) {
                for (j = AI_JOBS1B(); j; j = AI_JOBNEXT()) {
                    if (j->bldId == bl->id) {
                        v = j->price;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            lim = v - lim;
                            if ((float)lim / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = lim;
                                return ai_DecideTakeCredit(0x13, p, b, o1, o2);
                            }
                            return 0;
                        }
                        s1.kind = 4;
                        s1.ref = bl->id;
                        s1.arg = 1;
                        s2.kind = 8;
                        s2.ref = j->id;
                        break;
                    }
                }
            }
        }
        if (s1.kind != 4 || s2.kind != 8)
            return 0;
    }
    *o1 = s1;
    *o2 = s2;
    return 0x13;
}
/* 0x499290, execute of 0x498c70: pays a job, upgrades a building or runs 0x4a5320 */

int ai_ExecuteBuildBusiness(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIJOBX *j;

    if (s1->kind == 4 && s2->kind == 8) {
        j = (AIJOBX *)he_FindEvent(1, 1, s2->ref);
        if (!j)
            return 0;
        gm_SellBuilding(s1->ref, p->netId, j->price, j, 1);
        return 0x13;
    }
    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.time = ai_Time;
        msg.arg[0] = s2->ref;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x13;
    }
    if (ai_ExecBuildBuilding(p, s1, s2)) {
        cm_SendSetField(p->netId, 0x1c8, 4, AI_CHF_BUILT, 0);
        return 0x13;
    }
    return 0;
}
/* 0x499400, decide callback of "Betrieb upgraden (Credits)" (0x15): picks one of the character's businesses
   (ai_PickActionBuilding, building kinds of the 64 bit mask 0x1a07d7bb0) that can make a good it does not hold yet
   (ai_FindGoodToMake) and chooses the sub-action 0x15 for it (s1: the good, s2: its cart or the building) */

int ai_DecideUpgradeBusiness(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG s1, s2;
    AIBLD *y;
    LARGE_INTEGER m;
    int x, z, w, r;

    if (a != 0)
        return 0;
    if (p->kind == 3)
        return 0;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (o1->kind == 0) {
        /* nothing */
    } else
        return 0;
    m.LowPart = 0xa07d7bb0;
    m.HighPart = 1;
    if (!ai_PickActionBuilding(p, 4, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (!ai_FindGoodToMake(&w, &z, y, 0))
        return 0;
    s1.kind = 2;
    s1.ref = w;
    s1.arg = 1;
    s1.fvalue = 0.44f;
    if (z != -1) {
        s2.kind = 1;
        s2.ref = z;
        s2.fvalue = 0.44f;
        s2.arg = 1;
    } else {
        s2.kind = 4;
        s2.ref = y->id;
        s2.fvalue = 0.44f;
        s2.arg = 1;
    }
    r = ai_ChooseSubAction(0x15, p->id, 1, &s1, &s2);
    if (!r)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x499520, decide: action 0x16 when 0x4f81c0 rates kind 2 so */

int ai_DecideVisitMedicus(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];

    if (a)
        return 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}
/* 0x499580, execute of 0x499520 */

int ai_ExecuteVisitMedicus(AIPER *p, AIARG *s1, AIARG *s2)
{
    int buf[0x88];

    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}
/* 0x4995d0, evaluate: decision record 0x18, the default rating without it */

int ai_EvalSabotage(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x18, &d);
    if (d.cur && (p->kind != 5 || !(p->flags1c8 & 2)))
        return ai_EvalGuiltWeights(a, b, i, p, &d);
    return ai_EvalMoodWeights(a, b, i, p, f, o1, o2);
}
/* 0x499660, decide: sabotage a rival's building (action 0x17) */

int ai_DecideSabotage(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    AIDEC d;
    AIARG s1, s2;
    AIBLDX *y;
    AIPER *ch;
    LARGE_INTEGER mask;
    float g;
    int m, x, v, r;

    m = gm_CharCurrency(p, ai_OwnPlayer);
    if (!b) {
        if (p->bld2 == 0 && p->kind != 3) {
            s1.kind = 0;
            s1.ref = 0;
            s1.arg = 0;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_ChooseSubAction(0x17, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        if (p->kind == 3) {
            if (!(p->flags1e8 & 2))
                return 0;
        } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
            s1.kind = 2;
            s1.ref = 0x14;
            s1.arg = 1;
            s1.fvalue = 0.22f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_ChooseSubAction(0x17, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        gv_GetTableRecord(0x18, &d);
        if (d.cur && !util_Random(4) && !ai_Partner)
            return 0;
        if (o1->kind == 0) {
            mask.LowPart = 0x344015;
            mask.HighPart = 1;
            x = -1;
            if ((ai_Partner && (ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &x), x != -1))
                || ai_PickActionBuilding(p, 6, mask.QuadPart, 0, util_Random(0x20) + 19.0f, &x)) {
                gm_FindById((int)&y, 0, 0, x);
                if (y && y->owner != 0xffff && he_NoEvent88Pair(&ai_CharsT[y->owner], (AICHAR *)p)) {
                    memset(buf, 0, sizeof(buf));
                    buf[0] = 1;
                    v = (int)(gm_EquipmentValue(y, 100) * 0.05f + gm_GetWorth(p->id) * 0.02f);
                    ch = (AIPER *)&ai_CharsT[y->owner];
                    if (ord_DispatchOrder(0x1b, p, buf) & 2)
                        v /= 2;
                    if (!(m * 0.44f < v)) {
                        g = gm_Mix(p, ch, 2);
                        if (!(util_RandomF() + g >= 1.0f)) {
                            s1.kind = 0xe;
                            s1.ref = 2;
                            s1.arg = 1;
                            s1.fvalue = 0.0f;
                            s2.kind = 0;
                            s2.ref = 0;
                            s2.arg = 0;
                            r = ai_ChooseSubAction(0x17, p->id, 0, &s1, &s2);
                            if (r) {
                                *o1 = s1;
                                *o2 = s2;
                            }
                            return r;
                        }
                        if (he_NoEvent4bPair((AIPER *)&ai_CharsT[y->owner], p)
                            && he_NoEvent2ePair((AIPER *)&ai_CharsT[y->owner], p) && he_NoProtectionEvent((AIBLD *)y)
                            && he_NoEvent88Pair(&ai_CharsT[y->owner], (AICHAR *)p)) {
                            o1->kind = 4;
                            o1->ref = y->id;
                            o1->arg = 1;
                            o1->arg2 = v;
                            o1->fvalue = (float)(d.cur * 2 + 1);
                            return 0x17;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x499a90, execute of 0x499660 */

int ai_ExecuteSabotage(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIBLD *bl;

    if (s1->kind != 4)
        return 0;
    bl = gm_FindBuildingById(s1->ref);
    if (!bl)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x19;
    msg.to = p->netId;
    if (p->bld2)
        msg.fromId = p->bld2->id;
    else
        msg.fromId = -1;
    msg.flags = 1;
    msg.arg[0] = bl->id;
    msg.arg[9] = s1->arg2;
    cm_BeginGroup("sabotage");
    cm_SendLongMessage(&msg);
    cm_EndGroup("sabotage");
    ai_Actions[0x17].w3[0].w = ai_Actions[0x17].w1[0].w * s1->fvalue;
    ai_Actions[0x17].w3[1].w = ai_Actions[0x17].w1[1].w * s1->fvalue;
    ai_Actions[0x17].w3[2].w = ai_Actions[0x17].w1[2].w * s1->fvalue;
    ai_Actions[0x17].w3[3].w = ai_Actions[0x17].w1[3].w * s1->fvalue;
    return 0x17;
}
/* 0x499b80, evaluate: decision record 0x15, the default rating without it */

int ai_EvalBeatUp(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x15, &d);
    if (d.cur && (p->kind != 5 || !(p->flags1c8 & 2)))
        return ai_EvalGuiltWeights(a, b, i, p, &d);
    return ai_EvalMoodWeights(a, b, i, p, f, o1, o2);
}
/* 0x499c10, decide callback of "Verpruegeln" (0x18): when the character can afford it (currency * 0.22 above
   0.0085 of its worth, halved by order 0x1b), first a house (sub-action 0x18 with type 4) or a good 0x14 in it,
   then a partner (ai_PickPartnerChain) with money whom the mix roll lets it beat up (o1 kind 7, its netId) or
   else the sub-action 0x18 with kind 0xe */

int ai_DecideBeatUp(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    AIDEC d;
    AIARG s1, s2;
    AIPER *c;
    float g, t;
    int m, flags, v, r;

    m = gm_CharCurrency(p, ai_OwnPlayer);
    if (!b) {
        memset(buf, 0, sizeof(buf));
        buf[0] = 1;
        v = (int)(gm_GetWorth(p->id) * 0.0085f);
        if (ord_DispatchOrder(0x1b, p, buf) & 2)
            v /= 2;
        if (!(m * 0.22f < v)) {
            if (p->bld2 == 0 && p->kind != 3) {
                s1.kind = 5;
                s1.ref = 4;
                s1.arg = 1;
                s2.kind = 0;
                s2.ref = 0;
                s2.arg = 0;
                r = ai_ChooseSubAction(0x18, p->id, 1, &s1, &s2);
                if (r) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            if (p->kind == 3) {
                if (!(p->flags1e8 & 2))
                    return 0;
            } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
                s1.kind = 2;
                s1.ref = 0x14;
                s1.arg = 1;
                s1.fvalue = 0.22f;
                s2.kind = 4;
                s2.ref = p->bld2->id;
                s2.arg = 1;
                r = ai_ChooseSubAction(0x18, p->id, 1, &s1, &s2);
                if (r) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            gv_GetTableRecord(0x15, &d);
            if (!d.cur || util_Random(3) || ai_Partner) {
                if (o1->kind == 0) {
                    /* @original cid is a local of this block: its slot is shared with g (the frame), so its
                       scope ends before the gm_Mix call */
                    {
                        unsigned short cid;

                        flags = AI_CHAIN_BEATUP;
                        t = util_Random(0x20) + 32.0f;
                        if (!ai_PickPartnerChain(p, 1, &flags, 0, t, &cid))
                            return 0;
                        c = (AIPER *)&ai_CharsT[cid];
                    }
                    if (c->money >= 2) {
                        g = gm_Mix(p, c, 3);
                        if (!(util_RandomF() + g >= 1.0f)) {
                            s1.kind = 0xe;
                            s1.ref = 3;
                            s1.arg = 1;
                            s1.fvalue = 0.0f;
                            s2.kind = 0;
                            s2.ref = 0;
                            s2.arg = 0;
                            r = ai_ChooseSubAction(0x18, p->id, 0, &s1, &s2);
                            if (r) {
                                *o1 = s1;
                                *o2 = s2;
                            }
                            return r;
                        }
                        if (he_NoEvent4bPair(c, p) && he_NoEvent2ePair(c, p)) {
                            o1->kind = 7;
                            o1->ref = c->netId;
                            o1->arg = 1;
                            o1->fvalue = (float)(d.cur * 2 + 1);
                            o1->arg2 = v;
                            return 0x18;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x499f20, execute of 0x499c10 */

int ai_ExecuteBeatUp(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIPER *c;

    if (s1->kind != 7)
        return 0;
    c = gm_FindPer(s1->ref);
    if (!c)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x1a;
    msg.to = p->netId;
    if (p->bld2)
        msg.fromId = p->bld2->id;
    else
        msg.fromId = -1;
    msg.flags = 1;
    msg.u.s.cost = s1->arg2;
    msg.ch = c->netId;
    msg.arg[0] = ord_RandomObjectId(c);
    ai_Actions[0x18].w3[0].w = ai_Actions[0x18].w1[0].w * s1->fvalue;
    ai_Actions[0x18].w3[1].w = ai_Actions[0x18].w1[1].w * s1->fvalue;
    ai_Actions[0x18].w3[2].w = ai_Actions[0x18].w1[2].w * s1->fvalue;
    ai_Actions[0x18].w3[3].w = ai_Actions[0x18].w1[3].w * s1->fvalue;
    cm_BeginGroup("pruegel");
    cm_SendLongMessage(&msg);
    cm_EndGroup("pruegel");
    return 0x18;
}
/* 0x49a020, decide: action 0x19 for a character with flag 0x4000 and little money */

int ai_DecideCitizenship(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int m;

    if (b != 1) {
        if (b != 0)
            return 0;
    } else if (o1->kind != 0xd || o1->ref != 2)
        return 0;
    {
        if (p->rank < 2 && (p->flags1c8 & 0x4000)) {
            m = gm_CharCurrency(p, ai_OwnPlayer);
            if (m >= 0 && !(m * 0.44f < 16000.0f)) {
                o1->kind = 0xd;
                o1->ref = 2;
                o1->arg = 1;
                return 0x19;
            }
        }
    }
    return 0;
}
/* 0x49a0a0, execute callback of "Buergerrecht" (0x19): pays 16000 to the object 0x115 of the building of kind 0xf,
   raises the character to rank 2 (field 0xd) with flag 0x4000, and a character of kind 5 tells every councillor
   (kind 6 / 7) about it with a rank message 0x69 dated 1..4 hours (and 0..49 minutes) ahead */

int ai_ExecuteCitizenship(G5CH *p)
{
    AIMSG m;
    AIBLD *b;
    AIOBJ *o;
    AITIME t;
    int i, n;
    int rank;

    b = (AIBLD *)gm_BuildQuery(1, 5, 0xf);
    rank = 2;
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    cm_SendMoveGoodsUnchecked(o->objId, p->netId, 0, 16000);
    cm_BeginFieldBlock((AIPER *)p, p->netId);
    cm_AddFieldDelta(1, 1, 0xd, &rank);
    cm_SendFieldDeltas();
    cm_SendSetField(p->netId, 0x1c8, 4, 0, 0x4000);
    t = ai_Time;
    gm_AddTime(&t, util_Random(4) + 1, util_Random(8) * 7, 0);
    if (((AIPER *)p)->kind == 5) {
        n = 0;
        for (i = 0; i < 768; i++) {
            if (n >= (int)ai_NumPlayers)
                break;
            if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7) {
                AI_ZERO(&m, 0xf8);
                m.to = ai_CharsT[i].netId;
                m.type = 0x69;
                m.fromId = -1;
                m.flags = 9;
                m.time = t;
                m.rankUp.rank = 2;
                m.rankUp.ch = p->netId;
                cm_SendLongMessage(&m);
                n++;
            }
        }
    }
    return 0x19;
}
/* 0x49a290, decide callback of "ApsSparen" (0x1a): the sub-action 0x1a choice is taken unless it lowers a need
   (a weight of kind 0 below 0 in its short- or long-term block); without a choice, or when refused, it returns 0x1a
   with o1 / o2 untouched */

int ai_DecideSaveAps(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    G5ACT *act;
    int r, k;

    if (f != 0)
        return 0;
    s1.arg = 1;
    s2.arg = 1;
    s1.kind = 0;
    s1.ref = 0;
    s2.kind = 0;
    s2.ref = 0;
    r = ai_ChooseSubAction(0x1a, p->id, 0, &s1, &s2);
    if (r == 0)
        goto refuse;
    act = &G5_ACTS[r];
    for (k = 0; k < 4; k++) {
        if (act->w1[k].k == 0 && act->w1[k].w < 0.0f)
            goto refuse;
    }
    for (k = 0; k < 4; k++) {
        if (act->w2[k].k == 0 && act->w2[k].w < 0.0f)
            goto refuse;
    }
    *o1 = s1;
    *o2 = s2;
    return r;
refuse:
    return 0x1a;
}
/* 0x49a380, execute callback 0x1a */

int ai_ExecuteSaveAps(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0x1a;
}
/* 0x49a390, decide callback of "UseObject (Military)" (0x1b): for one of the character's buildings of the kinds in
   mask 0x90010 (ai_PickActionBuilding) whose workers average at least 6 - a random 0..3 in AICHAR.b1c5, the good
   0x161: with fewer than six goods and none usable, the sub-action 0x1c to get it from the character (weight
   0.05; k is a or 0x1c, and only a = 0 gets this far), else with a usable one the action 0x1b on it (o1 kind 1,
   its inventory entry; o2 the building) */

int ai_DecideUseMilitaryObject(int a, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    int k;
    int id;
    int st[3];
    G9REC rec;
    AIARG s1;
    AIARG s2;
    AIBLD *b;
    int i, n, sum, r;
    LARGE_INTEGER mask;

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    mask.LowPart = 0x90010;
    mask.HighPart = 0;
    if (ai_PickActionBuilding(p, 4, mask.QuadPart, 0, 100.0f, &id) == 0)
        return 0;
    gm_FindById((int *)&b, 0, 0, id);
    if (b == 0)
        return 0;
    n = 0;
    sum = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].bld == b) {
            n++;
            sum += ai_CharsT[i].b1c5;
        }
    }
    if (util_Random(4) + (float)sum / n < 6.0f)
        return 0;
    memset(&rec, 0, sizeof(rec));
    rec.item = 0x161;
    ai_FindWantedItems((G5CH *)p, 1, &rec, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 1)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            AIARG s1, s2;

            s1.ref = rec.item;
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.fvalue = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    s1.kind = 1;
    s1.ref = rec.entryId;
    s1.arg = 1;
    s2.kind = 4;
    s2.ref = b->id;
    *o1 = s1;
    s2.arg = -1;
    s2.arg2 = 0x1b;
    *o2 = s2;
    return 0x1b;
}
/* 0x49a5b0, execute callback 0x1b: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseMilitaryObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = s2->ref;
        u.arg = -1;
        u.effect = 0;
        u.handled = 0;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x1b].w3[0].w = G5_ACTS[0x1b].w1[0].w * g[3];
                return 0x1b;
            }
            if (u.effect == -1)
                G5_ACTS[0x1b].w3[0].w = G5_ACTS[0x1b].w1[0].w * g[3] * -0.5f;
            return 0x1b;
        }
    }
    return 0;
}
/* 0x49a690, decide callback of "UseObject (Ansehen)" (0x1c): of twelve reputation goods (0x155..0x2b6), with fewer
   than six goods and none of them usable, the sub-action 2 to get one fitting the office level (weight 0.05; o2
   the character itself); else with a usable one the action 0x1c on a random one (o1 kind 1, its inventory entry):
   goods 9 and 10 with a partner of the other / the same sex (ai_PickPartnerChain, o2 kind 7), good 8 through the
   sub-action 3 */

int ai_DecideUseReputationObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    AIARG s1;
    AIARG s2;
    int st[3];
    G9REC recs[12];
    AMTKIND kind;

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x155;
    recs[1].item = 0x15d;
    recs[2].item = 0x157;
    recs[3].item = 0x15f;
    recs[4].item = 0x159;
    recs[5].item = 0x165;
    recs[6].item = 0x166;
    recs[7].item = 0x16b;
    recs[8].item = 0x170;
    recs[9].item = 0x167;
    recs[10].item = 0x229;
    recs[11].item = 0x2b6;
    ai_FindWantedItems(p, 12, recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 12)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            if (amt_GetKind(p->officeKind, &kind) == 0)
                i = 7;
            else if (kind.level < 4)
                i = 0;
            else
                i = kind.level < 7 ? 2 : 4;
            while (recs[i].value != 0)
                i = (i + 1) % 12;
            s1.ref = recs[i].item;
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.fvalue = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_Random(12);
    while (recs[i].value < 1)
        i = (i + 1) % 12;
    s2.kind = 1;
    s2.ref = recs[i].entryId;
    s2.arg = 1;
    s1.kind = 0;
    s1.ref = -1;
    s1.arg = -1;
    s1.arg2 = 0x1c;
    if (i == 9) {
        int flags;
        unsigned short id;

        flags = (p->sex != 0 ? 1 : 2) | AI_CHAIN_USEREPUTATION;
        if (ai_PickPartnerChain((AIPER *)p, 1, &flags, 10.0f, (float)util_Random(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.ref = ai_CharsT[id].netId;
        s1.arg = -1;
    } else if (i == 10) {
        int flags;
        unsigned short id;

        flags = (p->sex != 1 ? 1 : 2) | AI_CHAIN_USEREPUTATION;
        if (ai_PickPartnerChain((AIPER *)p, 1, &flags, 10.0f, (float)util_Random(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.ref = ai_CharsT[id].netId;
        s1.arg = -1;
    } else if (i == 8) {
        r = ai_ChooseSubAction(k, p->id, 3, &s2, &s1);
        if (r != 0) {
            *o1 = s2;
            *o2 = s1;
        }
        return r;
    }
    *o1 = s2;
    *o2 = s1;
    return 0x1c;
}
/* 0x49aa80, execute callback 0x1c: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseReputationObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.effect = 0;
        u.handled = 0;
        u.target = s2->ref;
        u.arg = s2->arg;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x1c].w3[0].w = G5_ACTS[0x1c].w1[0].w * g[3];
                return 0x1c;
            }
            if (u.effect == -1)
                G5_ACTS[0x1c].w3[0].w = G5_ACTS[0x1c].w1[0].w * g[3] * -0.5f;
            return 0x1c;
        }
    }
    return 0;
}
/* 0x49ab60, decide callback of "UseObject (APS)" (0x1d), as ai_DecideUseReputationObject for the two goods 0x173
   and 0x15b: with fewer than six goods and neither usable, the sub-action 2 to get a random missing one; else the
   action 0x1d on a random usable one (o1 kind 1, its inventory entry), good 0x173 through the sub-action 4 */

int ai_DecideUseApsObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G9REC recs[2];

    k = a;
    if (a == 0)
        k = 0x1d;
    if (f == 0) {
        memset(recs, 0, sizeof(recs));
        recs[0].item = 0x173;
        recs[1].item = 0x15b;
        ai_FindWantedItems(p, 2, recs, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != 2) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    i = util_Random(2);
                    while (recs[i].value != 0)
                        i = (i + 1) % 2;
                    s1.kind = 2;
                    s1.ref = recs[i].item;
                    s1.arg = 1;
                    s2.arg = 1;
                    s2.ref = p->netId;
                    s1.fvalue = 0.05f;
                    s2.kind = 7;
                    r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
                    if (r != 0) {
                        *o1 = s1;
                        *o2 = s2;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            i = util_Random(2);
            while (recs[i].value < 1)
                i = (i + 1) % 2;
            s2.kind = 1;
            s2.ref = recs[i].entryId;
            s2.arg = 1;
            s1.kind = 0;
            s1.ref = -1;
            s1.arg = -1;
            s1.arg2 = 0x1d;
            if (i == 0) {
                r = ai_ChooseSubAction(k, p->id, 4, &s2, &s1);
                if (r != 0) {
                    *o1 = s2;
                    *o2 = s1;
                }
                return r;
            }
            *o1 = s2;
            *o2 = s1;
            return 0x1d;
        }
    }
    return 0;
}
/* 0x49ad80, execute callback 0x1d: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseApsObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = -1;
        u.arg = -1;
        u.effect = 0;
        u.handled = 0;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x1d].w3[0].w = G5_ACTS[0x1d].w1[0].w * g[3];
                return 0x1d;
            }
            if (u.effect == -1)
                G5_ACTS[0x1d].w3[0].w = -(G5_ACTS[0x1d].w1[0].w * g[3] * 0.5f);
            return 0x1d;
        }
    }
    return 0;
}
/* 0x49ae50, decide callback of "UseObject (Bildung)" (0x1e), as ai_DecideUseReputationObject for the four goods
   0x168, 0x16c, 0x297 and 0x2b7: with fewer than six goods and none usable, the sub-action 2 to get a random missing
   one; else the action 0x1e on a random usable one (o1 kind 1, its inventory entry) */

int ai_DecideUseEducationObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G9REC recs[4];

    k = a;
    if (a == 0)
        k = 0x1e;
    if (f == 0) {
        memset(recs, 0, sizeof(recs));
        recs[0].item = 0x168;
        recs[1].item = 0x16c;
        recs[2].item = 0x297;
        recs[3].item = 0x2b7;
        ai_FindWantedItems(p, 4, recs, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != 4) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    i = util_Random(4);
                    while (recs[i].value != 0)
                        i = (i + 1) % 4;
                    s1.kind = 2;
                    s1.ref = recs[i].item;
                    s1.arg = 1;
                    s2.arg = 1;
                    s2.ref = p->netId;
                    s1.fvalue = 0.05f;
                    s2.kind = 7;
                    r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
                    if (r != 0) {
                        *o1 = s1;
                        *o2 = s2;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            i = util_Random(4);
            while (recs[i].value < 1)
                i = (i + 1) % 4;
            s1.kind = 1;
            s1.ref = recs[i].entryId;
            s1.arg = 1;
            *o1 = s1;
            s2.kind = 0;
            s2.ref = -1;
            s2.arg = -1;
            s2.arg2 = 0x1e;
            *o2 = s2;
            return 0x1e;
        }
    }
    return 0;
}
/* 0x49b050, execute callback 0x1e: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseEducationObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = -1;
        u.arg = -1;
        u.effect = 0;
        u.handled = 0;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x1e].w3[0].w = G5_ACTS[0x1e].w1[0].w * g[3];
                return 0x1e;
            }
            if (u.effect == -1)
                G5_ACTS[0x1e].w3[0].w = -(G5_ACTS[0x1e].w1[0].w * g[3]);
            return 0x1e;
        }
    }
    return 0;
}
/* 0x49b120, decide callback of "UseObject (Unversehrtheit)" (0x1f): the good 0x17e while the character has an
   ailment, else (one time in three) the good 0x175; with fewer than six goods and it not usable, the sub-action 2
   to get it (weight 0.05; o2 the character itself), else with it usable the sub-action 4 on it (o1 kind 1, its
   inventory entry) */

int ai_DecideUseHealthObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, r;
    int st[3];
    G9REC rec;
    AIARG s1;
    AIARG s2;

    k = a;
    if (a == 0)
        k = 0x1f;
    if (f == 0) {
        memset(&rec, 0, sizeof(rec));
        if (p->ailments != 0)
            rec.item = 0x17e;
        else if (util_Random(3) != 0)
            return 0;
        else
            rec.item = 0x175;
        ai_FindWantedItems(p, 1, &rec, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != 1) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    s1.ref = rec.item;
                    s2.ref = p->netId;
                    s1.kind = 2;
                    s1.arg = 1;
                    s1.fvalue = 0.05f;
                    s2.kind = 7;
                    s2.arg = 1;
                    r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
                    if (r != 0) {
                        *o1 = s1;
                        *o2 = s2;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            s1.ref = -1;
            s1.arg = -1;
            s2.ref = rec.entryId;
            s2.kind = 1;
            s2.arg = 1;
            s1.kind = 0;
            s1.arg2 = 0x1f;
            r = ai_ChooseSubAction(k, p->id, 4, &s2, &s1);
            if (r != 0) {
                *o1 = s2;
                *o2 = s1;
            }
            return r;
        }
    }
    return 0;
}
/* 0x49b2b0, execute callback 0x1f: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseHealthObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = -1;
        u.arg = -1;
        u.effect = 0;
        u.handled = 0;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x1f].w3[0].w = G5_ACTS[0x1f].w1[0].w * g[3];
                return 0x1f;
            }
            if (u.effect == -1)
                G5_ACTS[0x1f].w3[0].w = -(G5_ACTS[0x1f].w1[0].w * g[3]);
            return 0x1f;
        }
    }
    return 0;
}
/* 0x49b380, decide callback of "UseObject (Dynastie)" (0x20): the gifts 0x162..0x164 for a relative (3..7) younger
   than 40, else, with no relatives[0], the good 0x167 for the partner of the character's pair event 0x41 or the
   wedding ring 0x170; with fewer than six goods and none usable, the sub-action 2 to get a random missing one
   (weight 0.05; o2 the character itself), else the action 0x20 with a random usable one (o1 kind 1, its inventory
   entry) for the relative / the partner (o2 kind 7), the ring through the sub-action 3 */

int ai_DecideUseDynastyObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    AIPER *w;
    AIPER *c;
    AIJOB *j;
    int st[3];
    AIARG s1;
    AIARG s2;
    G9REC recs[3];

    k = a;
    if (a == 0)
        k = 0x20;
    if (f == 0 && a == 0) {
        for (i = 3; i < 8; i++) {
            c = gm_FindPer(p->relatives[i]);
            if (c != 0 && c->age < 40)
                break;
        }
        for (i = 3; i < 8; i++) {
            c = gm_FindPer(p->relatives[i]);
            if (c != 0 && c->age < 40)
                break;
        }
        memset(recs, 0, sizeof(recs));
        if (c != 0) {
            recs[0].item = 0x162;
            recs[1].item = 0x163;
            recs[2].item = 0x164;
            n = 3;
        } else {
            if (p->relatives[0] != -1)
                return 0;
            for (j = (AIJOB *)he_FindEvent(1, 0, 0x41); j != 0; j = (AIJOB *)he_NextEvent()) {
                if (j->partner1 == (int)p->netId)
                    break;
            }
            if (j != 0) {
                w = gm_FindPer(j->partner2);
                if (w == 0)
                    return 0;
                recs[0].item = 0x167;
                recs[1].item = 0;
                recs[2].item = 0;
                n = 1;
            } else {
                recs[0].item = 0x170;       /* w stays unset: only the 0x167 case reads it */
                recs[1].item = 0;
                recs[2].item = 0;
                n = 1;
            }
        }
        ai_FindWantedItems(p, n, recs, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != n) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    i = util_Random(n);
                    while (recs[i].value != 0)
                        i = (i + 1) % n;
                    s2.kind = 2;
                    s2.ref = recs[i].item;
                    s1.ref = p->netId;
                    s2.arg = 1;
                    s2.fvalue = 0.05f;
                    s1.kind = 7;
                    s1.arg = 1;
                    r = ai_ChooseSubAction(k, p->id, 2, &s2, &s1);
                    if (r != 0) {
                        *o1 = s2;
                        *o2 = s1;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            i = util_Random(n);
            while (recs[i].value < 1)
                i = (i + 1) % n;
            s2.kind = 1;
            s2.arg = 1;
            s2.ref = recs[i].entryId;
            s2.fvalue = 0.05f;
            switch (recs[i].item) {
            case 0x170:
                s1.kind = 0;
                s1.arg = -1;
                s1.ref = -1;
                break;
            case 0x167:
                s1.ref = w->netId;
                s1.kind = 7;
                s1.arg = -1;
                break;
            case 0x162:
            case 0x163:
            case 0x164:
                s1.ref = c->netId;
                s1.kind = 7;
                s1.arg = -1;
                break;
            default:
                return 0;
            }
            s1.arg2 = 0x20;
            if (recs[i].item == 0x170) {
                r = ai_ChooseSubAction(k, p->id, 3, &s2, &s1);
                if (r != 0) {
                    *o1 = s2;
                    *o2 = s1;
                }
                return r;
            }
            *o1 = s2;
            *o2 = s1;
            return 0x20;
        }
    }
    return 0;
}
/* 0x49b6f0, execute callback of "UseObject (Dynastie)" (0x20): uses the good s1->ref of the character's inventory
   on the target s2->ref (ai_UseGood); when it took effect, the action's short-term weight w3[0] becomes w1[0]
   times the good's effect weight, when it backfired minus half of that; 0x20 when used, else 0 */

int ai_ExecuteUseDynastyObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = (AIOBJ *)gm_Query((void *)p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = s2->ref;
        u.arg = -1;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x20].w3[0].w = G5_ACTS[0x20].w1[0].w * g[3];
                return 0x20;
            }
            if (u.effect == -1)
                G5_ACTS[0x20].w3[0].w = -(G5_ACTS[0x20].w1[0].w * g[3] * 0.5f);
            return 0x20;
        }
    }
    return 0;
}
/* 0x49b7c0, decide callback of "UseObject (Gemeinheit)" (0x21): of twelve mean goods (0x169..0x29c), with fewer than
   six goods and none usable, the sub-action 2 to get a random missing one (weight 0.05; o2 the character itself;
   k is 0x1c, not 0x21, as in the original); else a random usable one (o1 kind 1, its inventory entry) against
   partners (ai_PickPartnerChain): 0x169 / 0x16d two characters (o2 kind 7, both netIds), 0x177 the building of
   one (kind 4), 0x178 a building of the leader or the character (ai_PickActionBuilding, kind 4), the others one
   character (kind 7); all but the first two goods go through the sub-action 4 */

int ai_DecideUseMeanObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    unsigned int flags[2];
    unsigned short id[2];
    int v;
    LARGE_INTEGER mask;
    G9REC *rp;
    AIARG s1;
    AIARG s2;
    int st[3];
    G9REC recs[12];

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f == 0 && a == 0) {
        memset(recs, 0, sizeof(recs));
        recs[0].item = 0x169;
        recs[1].item = 0x16a;
        recs[2].item = 0x177;
        recs[3].item = 0x178;
        recs[4].item = 0x17d;
        recs[5].item = 0x16d;
        recs[6].item = 0x17b;
        recs[7].item = 0x22c;
        recs[8].item = 0x22b;
        recs[9].item = 0x22d;
        recs[10].item = 0x29a;
        recs[11].item = 0x29c;
        ai_FindWantedItems(p, 12, recs, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != 12) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    i = util_Random(12);
                    while (recs[i].value != 0)
                        i = (i + 1) % 12;
                    s1.ref = recs[i].item;
                    s1.kind = 2;
                    s1.arg = 1;
                    s1.fvalue = 0.05f;
                    s2.kind = 7;
                    s2.ref = p->netId;
                    s2.arg = 1;
                    r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
                    if (r != 0) {
                        *o1 = s1;
                        *o2 = s2;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            i = util_Random(12);
            while (recs[i].value < 1)
                i = (i + 1) % 12;
            s1.kind = 1;
            s1.arg = 1;
            s1.fvalue = 0.05f;
            s1.ref = recs[i].entryId;
            rp = &recs[i];
            if (rp->item == 0x169 || rp->item == 0x16d) {
                flags[0] = AI_CHAIN_MEAN;
                flags[1] = AI_CHAIN_MEAN;
                n = 2;
            } else if (rp->item == 0x29a) {
                flags[0] = AI_CHAIN_MEAN | 0x800;
                n = 1;
            } else {
                flags[0] = AI_CHAIN_MEAN;
                n = 1;
            }
            if (rp->item == 0x177)
                flags[0] |= 0x10000;
            if (rp->item == 0x178) {
                mask.LowPart = 0xa07c4390;
                mask.HighPart = 1;
                v = -1;
                if (!(ai_Partner && (ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &v), v != -1))
                    && !ai_PickActionBuilding((AIPER *)p, 6, mask.QuadPart, 0, (float)util_Random(0x40), &v))
                    return 0;
            } else if (ai_PickPartnerChain((AIPER *)p, n, (int *)flags, 0.0f, (float)util_Random(0x40), id) == 0) {
                return 0;
            }
            switch (rp->item) {
            case 0x169:
            case 0x16d:
                s2.ref = ai_CharsT[id[0]].netId;
                s2.arg = ai_CharsT[id[1]].netId;
                s2.kind = 7;
                break;
            case 0x16a:
                s2.arg = -1;
                s2.ref = ai_CharsT[id[0]].netId;
                s2.kind = 7;
                break;
            case 0x177:
                s2.kind = 4;
                s2.ref = ai_CharsT[id[0]].bld2->id;
                s2.arg = -1;
                break;
            case 0x178:
                s2.kind = 4;
                s2.ref = v;
                s2.arg = -1;
                break;
            case 0x17b:
            case 0x17d:
            case 0x22b:
            case 0x22c:
            case 0x22d:
            case 0x29a:
            case 0x29c:
                s2.arg = -1;
                s2.ref = ai_CharsT[id[0]].netId;
                s2.kind = 7;
                break;
            default:
                return 0;
            }
            s2.arg2 = 0x21;
            if (i > 1) {
                r = ai_ChooseSubAction(k, p->id, 4, &s1, &s2);
                if (r != 0) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            *o1 = s1;
            *o2 = s2;
            return 0x21;
        }
    }
    return 0;
}
/* 0x49bc50, execute callback 0x21: uses the object s1->unk04 of the character's list, scales four needs */

int ai_ExecuteUseMeanObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = s2->ref;
        u.arg = s2->arg;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x21].w3[0].w = G5_ACTS[0x21].w1[0].w * g[3];
                G5_ACTS[0x21].w3[1].w = G5_ACTS[0x21].w1[1].w * g[3];
                G5_ACTS[0x21].w3[2].w = G5_ACTS[0x21].w1[2].w * g[3];
                G5_ACTS[0x21].w3[3].w = G5_ACTS[0x21].w1[3].w * g[3];
                return 0x21;
            }
            if (u.effect == -1) {
                G5_ACTS[0x21].w3[0].w = -(G5_ACTS[0x21].w1[0].w * g[3] * 0.5f);
                G5_ACTS[0x21].w3[1].w = -(G5_ACTS[0x21].w1[1].w * g[3] * 0.5f);
                G5_ACTS[0x21].w3[2].w = -(G5_ACTS[0x21].w1[2].w * g[3] * 0.5f);
                G5_ACTS[0x21].w3[3].w = -(G5_ACTS[0x21].w1[3].w * g[3] * 0.5f);
            }
            return 0x21;
        }
    }
    return 0;
}
/* 0x49bd90, decide callback of "UseObject (Sicherheit)" (0x22), as ai_DecideUseReputationObject for ten goods
   (0x17c..0x2b3): with fewer than six goods and none usable, the sub-action 2 to get a random missing one (weight
   0.05; o2 the character itself); else the action 0x22 on a random usable one (o1 kind 1, its inventory entry),
   good 0x17c through the sub-action 4, 0x171 / 0x16f / 0x15c through the sub-action 3 */

int ai_DecideUseSafetyObject(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    G9REC recs[10];

    k = a;
    if (a == 0)
        k = 0x22;
    if (f == 0 && a == 0) {
        memset(recs, 0, sizeof(recs));
        recs[0].item = 0x17c;
        recs[1].item = 0x171;
        recs[2].item = 0x16f;
        recs[3].item = 0x15c;
        recs[4].item = 0x15a;
        recs[5].item = 0x22e;
        recs[6].item = 0x22a;
        recs[7].item = 0x2b4;
        recs[8].item = 0x2b5;
        recs[9].item = 0x2b3;
        ai_FindWantedItems(p, 10, recs, st);
        if ((st[0] != 6 || st[2] != 0) && st[1] != 10) {
            if (st[0] < 6) {
                if (st[2] == 0) {
                    AIARG s1;
                    AIARG s2;

                    i = util_Random(10);
                    while (recs[i].value != 0)
                        i = (i + 1) % 10;
                    s1.ref = recs[i].item;
                    s1.kind = 2;
                    s1.arg = 1;
                    s1.fvalue = 0.05f;
                    s2.kind = 7;
                    s2.ref = p->netId;
                    s2.arg = 1;
                    r = ai_ChooseSubAction(k, p->id, 2, &s1, &s2);
                    if (r != 0) {
                        *o1 = s1;
                        *o2 = s2;
                    }
                    return r;
                }
            } else if (st[2] == 0) {
                return 0;
            }
            {
                AIARG s1;
                AIARG s2;

                i = util_Random(10);
                while (recs[i].value < 1)
                    i = (i + 1) % 10;
                s1.ref = recs[i].entryId;
                s1.kind = 1;
                s1.arg = 1;
                s2.kind = 0;
                s2.ref = -1;
                s2.arg = -1;
                s2.arg2 = 0x22;
                if (i == 0) {
                    r = ai_ChooseSubAction(k, p->id, 4, &s1, &s2);
                } else if (i < 4) {
                    r = ai_ChooseSubAction(k, p->id, 3, &s1, &s2);
                } else {
                    *o1 = s1;
                    *o2 = s2;
                    return 0x22;
                }
                if (r != 0) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
        }
    }
    return 0;
}
/* 0x49c000, execute callback 0x22: uses the object s1->unk04 of the character's list */

int ai_ExecuteUseSafetyObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = -1;
        u.arg = -1;
        u.effect = 0;
        u.handled = 0;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x22].w3[0].w = G5_ACTS[0x22].w1[0].w * g[3];
                return 0x22;
            }
            if (u.effect == -1)
                G5_ACTS[0x22].w3[0].w = G5_ACTS[0x22].w1[0].w * g[3] * -0.5f;
            return 0x22;
        }
    }
    return 0;
}
/* 0x49c0d0, eval callback for the goods actions 0x1c..0x22: decision 0x11 decides the rating */

int ai_EvalUsePerfume(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    if (o2->arg2 >= 0x1c && o2->arg2 <= 0x22) {
        gv_GetTableRecord(0x11, (AIPOS *)&d);
        if (d.cur != 0)
            return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
        return ai_EvalMoodWeights(a, b, i, (AIPER *)p, f, o1, o2);
    }
    return 0;
}
/* 0x49c170, decide callback 0x23 */

int ai_DecideUsePerfume(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 3)
        return 0x23;
    return 0;
}
/* 0x49c190, execute callback 0x23: uses the object, the needs of action s2->unk10 scale those of 0x23 */

int ai_ExecuteUsePerfume(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = s2->ref;
        u.arg = s2->arg;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x23].w3[0].w = G5_ACTS[s2->arg2].w1[0].w * g[3];
                G5_ACTS[0x23].w3[1].w = G5_ACTS[s2->arg2].w1[1].w * g[3];
                G5_ACTS[0x23].w3[2].w = G5_ACTS[s2->arg2].w1[2].w * g[3];
                G5_ACTS[0x23].w3[3].w = G5_ACTS[s2->arg2].w1[3].w * g[3];
                return 0x23;
            }
            if (u.effect == -1) {
                G5_ACTS[0x23].w3[0].w = -(G5_ACTS[s2->arg2].w1[0].w * g[3] * 0.5f);
                G5_ACTS[0x23].w3[1].w = -(G5_ACTS[s2->arg2].w1[1].w * g[3] * 0.5f);
                G5_ACTS[0x23].w3[2].w = -(G5_ACTS[s2->arg2].w1[2].w * g[3] * 0.5f);
                G5_ACTS[0x23].w3[3].w = -(G5_ACTS[s2->arg2].w1[3].w * g[3] * 0.5f);
            }
            return 0x23;
        }
    }
    return 0;
}
/* 0x49c320, eval callback for the goods actions 0x1c..0x22: decision 0x12 decides the rating */

int ai_EvalUseTrank(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    if (o2->arg2 >= 0x1c && o2->arg2 <= 0x22) {
        gv_GetTableRecord(0x12, (AIPOS *)&d);
        if (d.cur != 0)
            return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
        return ai_EvalMoodWeights(a, b, i, (AIPER *)p, f, o1, o2);
    }
    return 0;
}
/* 0x49c3c0, decide callback 0x24 */

int ai_DecideUseTrank(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 4)
        return 0x24;
    return 0;
}
/* 0x49c3e0, execute callback 0x24: uses the object, the needs of action s2->unk10 scale those of 0x24 */

int ai_ExecuteUseTrank(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(p->inventory, 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)ai_FindUsableGood(o->id);
    if (g != 0) {
        u.obj = o;
        u.target = s2->ref;
        u.arg = s2->arg;
        if (ai_UseGood(p, &u) == 1) {
            if (u.effect == 1) {
                G5_ACTS[0x24].w3[0].w = G5_ACTS[s2->arg2].w1[0].w * g[3];
                G5_ACTS[0x24].w3[1].w = G5_ACTS[s2->arg2].w1[1].w * g[3];
                G5_ACTS[0x24].w3[2].w = G5_ACTS[s2->arg2].w1[2].w * g[3];
                G5_ACTS[0x24].w3[3].w = G5_ACTS[s2->arg2].w1[3].w * g[3];
                return 0x24;
            }
            if (u.effect == -1) {
                G5_ACTS[0x24].w3[0].w = -(G5_ACTS[s2->arg2].w1[0].w * g[3] * 0.5f);
                G5_ACTS[0x24].w3[1].w = -(G5_ACTS[s2->arg2].w1[1].w * g[3] * 0.5f);
                G5_ACTS[0x24].w3[2].w = -(G5_ACTS[s2->arg2].w1[2].w * g[3] * 0.5f);
                G5_ACTS[0x24].w3[3].w = -(G5_ACTS[s2->arg2].w1[3].w * g[3] * 0.5f);
            }
            return 0x24;
        }
    }
    return 0;
}
/* 0x49c570, decide callback 0x25: spying on a character (0x49c700 sends "spionage"), if the character can pay for it */

int ai_DecideSpy(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned int flags;
    unsigned short id;
    int r, money, v, x;
    float w;
    G5CH *c;

    flags = AI_CHAIN_SPY;
    r = util_Random(3);
    money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (f != 0)
        return 0;
    if (ai_PickPartnerChain((AIPER *)p, 1, (int *)&flags, 0, (float)util_Random(0x19) + 10.0, &id) == 0)
        return 0;
    c = &G5_CHARS[id];
    w = gm_GetWorth(p->id) * 0.005f;
    x = (int)(gm_GetWorth(c->id) * 0.005f + w);
    v = (int)(x * ((float)r * 0.33f + 1.0f));
    if (money * 0.22f < v)
        return 0;
    if (he_FindEvent(2, 0, 0x18, 2, (int)p->id) != 0)
        return 0;
    o1->kind = 7;
    o1->ref = c->netId;
    o1->arg = 1;
    o1->arg2 = v;
    o2->kind = 0;
    o2->ref = 0;
    o2->arg = 0;
    o2->arg2 = 0;
    return 0x25;
}
/* 0x49c700, execute callback 0x25: sends the "spionage" message */

int ai_ExecuteSpy(G5CH *p, AIARG *s1)
{
    AIMSG m;

    if (s1->kind != 7)
        return 0;
    m.type = 0x18;
    m.flags = 2;
    m.to = p->netId;
    m.fromId = -1;
    if (p->bld2)
        m.arg[0] = p->bld2->id;
    else
        m.arg[0] = -1;
    m.arg[6] = s1->ref;
    m.amount[2] = s1->arg2;
    cm_BeginGroup("spionage");
    cm_SendLongMessage(&m);
    cm_EndGroup("spionage");
    return 0x25;
}
/* 0x49c790, callback 0x0c: buy the good s1->unk04 at the place that sells it, if the character can pay
   (s1->fvalue in (0, 1) is the share of the money to use) */

int ai_DecideBuyMarketObject(int a, G5CH *p, int f, AIARG *s1, AIARG *o)
{
    AIOBJ *bld;
    AIOBJ *obj;
    int money;

    if (f == 2 && s1->kind == 2) {
        bld = (AIOBJ *)gm_FindMarketObject(s1->ref);
        if (bld != 0) {
            obj = gm_Query(bld->list, 1, 0, s1->ref);
            if (obj != 0 && gm_ListAmount((short *)bld, (char *)obj) >= 1) {
                money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
                if (s1->fvalue > 0.0f && s1->fvalue < 1.0f)
                    money = (int)(money * s1->fvalue);
                if (money >= (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer)) {
                    o->kind = 1;
                    o->ref = bld->objId;
                    o->arg = 1;
                    return 0xc;
                }
            }
        }
    }
    return 0;
}
/* 0x49c880, execute callback 0x0c: the character buys the good s1->unk04 from s2->unk04 at its price */

int ai_ExecuteBuyMarketObject(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObject(p->netId, s2->ref, s1->ref, s1->arg, ai_OwnPlayer,
                 (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 0xc;
}
/* 0x49c8e0, decide callback 0x26 "Ablass Kaufen": on its year / hour slot, a character guilty enough buys an
   indulgence in a building (1, 5, 7) of another owner; the price depends on its worth, guilt and the owner's
   liking (half with the good 0x2c6), or it pays with its money field when that is 4 or more */

int ai_DecideBuyIndulgence(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int guilt, graves, i, money, w, c;
    float worth;
    AIBLDX *b;
    AIBLDX *list[8];

    if (f == 0 && (p->netId & 3) == ai_Time.year % 4 && (p->netId & 7) == (int)ai_Time.hour % 8) {
        guilt = gv_CulpritGuilt(p->netId);
        graves = gv_CountOwnedGraves(p->netId);
        if (util_Random(8) + 8 <= guilt) {
            i = 0;
            b = AI_501190_3(1, 5, 7);
            if (b != 0) {
                do {
                    if (b->owner != 0xffff && b->owner != p->id && !(b->flags5a & 1))
                        list[i++] = b;
                } while ((b = AI_BUILDNEXT()) != 0);
                if (i != 0) {
                    b = list[util_Random(i)];
                    if (p->money < 4) {
                        /* the price: the character's worth, its guilt and the owner's liking, clamped */
                        money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
                        worth = gm_GetWorth(p->id) * 0.005f;
                        w = (int)((float)((200.0f - gm_Liking(p->id, b->owner, 1)) * 0.01f) * guilt * worth);
                        w = (int)min(max(w * 0.01, 3200.0), 320000.0);
                        if (ai_InventoryHas(p, 0x2c6))
                            w /= 2;
                        if (!(w > money * 0.44f)) {
                            if (graves <= 1)
                                graves = 1;
                            o1->kind = 4;
                            o1->ref = b->id;
                            o1->arg2 = w;
                            o1->fvalue = (float)graves;
                            o2->kind = 0x12;
                            return 0x26;
                        }
                    } else {
                        c = p->money / 2;
                        c += c & 1;
                        w = c / 2;
                        if (w > graves) {
                            w = graves;
                            c = graves * 2;
                        }
                        o1->kind = 4;
                        o1->ref = b->id;
                        o1->arg2 = c;
                        o1->fvalue = (float)w;
                        o2->kind = 0xf;
                        return 0x26;
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x49cb60, execute callback 0x26: pays the rent (s2->kind 0x12, with a message) or takes over the rooms
   (s2->kind 0xf) of the building s1->unk04 */

int ai_ExecuteBuyIndulgence(G5CH *p, AIARG *s1, AIARG *s2)
{
    AIBLDX *b;
    AIOBJ *o;
    int n, d;

    if (s1->kind != 4)
        return 0;
    b = AI_501190_3(1, 1, s1->ref);
    if (b == 0 || b->owner == 0xffff)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0xf2);
    if (s2->kind == 0x12) {
        n = s1->arg2;
        d = (int)((float)n / log10(n));
        cm_BeginFieldBlock((AIPER *)p, p->netId);
        cm_AddFieldValues(4, 1, 0x1ac, &s1->arg2);
        cm_SendFieldDeltas();
        cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s1->arg2);
        cm_SendMoveGoodsUnchecked(ai_CharsT[b->owner].netId, -1, ai_OwnPlayer, d);
        gv_RetireCrimesOf(p->netId, (int)s1->fvalue);
        ai_ShowIndulgenceNews(p, (char *)&ai_CharsT[b->owner], (int)b, d, s1->arg2);
        G5_ACTS[0x26].w3[0].w = G5_ACTS[0x26].w1[0].w * s1->fvalue;
        G5_ACTS[0x26].w3[1].w = G5_ACTS[0x26].w1[1].w * s1->fvalue;
        G5_ACTS[0x26].w3[2].w = G5_ACTS[0x26].w1[2].w * s1->fvalue;
        return 0x26;
    }
    if (s2->kind == 0xf && o != 0) {
        gv_RetireCrimesOf(p->netId, (int)s1->fvalue);
        gm_SendMoneyDelta(p->netId, -s1->arg2);
        gm_SendMoneyDelta(ai_CharsT[b->owner].netId, 1);
        ai_ShowPaymentNews(p, (char *)&ai_CharsT[b->owner], (int)b);
        return 0x26;
    }
    return 0;
}
/* 0x49cd30, decide callback 0x27: what 0x4a9a00 decides */

int ai_DecideUseOffice(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    s1.kind = 0;
    s1.ref = 0;
    s1.arg = 0;
    if (ai_DecideLeisureAction(0, p, 0, &s1, &s2) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x27;
}
/* 0x49cdb0, execute callback 0x27 */

int ai_ExecuteUseOffice(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckLeisureAction(p, s1, s2) ? 0x27 : 0;
}
/* 0x49cdd0, eval callback 0x27: decision 0x13 decides the rating */

int ai_EvalThreaten(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x13, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x49ce50, decide callback 0x28 "Drohen": first a house (sub-action kind 5) or the good 0x15 for it,
   else threaten a little-known character (ai_DecideLittleKnownChar): directly when random + gm_Mix >= 1,
   else through the sub-action (kind 4, ref 3) first */

int ai_DecideThreaten(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    float g;
    AIPER *c;
    int r;

    if (f == 0) {
        if (p->bld2 == 0 && p->kind != 3) {
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_ChooseSubAction(0x28, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        if (p->kind == 3) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x15)) {
            s1.kind = 2;
            s1.ref = 0x15;
            s1.arg = 1;
            s1.fvalue = 0.44f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_ChooseSubAction(0x28, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        memset(&s1, 0, sizeof(s1));
        memset(&s2, 0, sizeof(s2));
        if (ai_DecideLittleKnownChar(p, &s1, &s2) == 0)
            return 0;
        c = (AIPER *)&ai_CharsT[s2.arg2];
        g = gm_Mix((AIPER *)p, c, 4);
        if (!(util_RandomF() + g >= 1.0f)) {
            s1.kind = 4;
            s1.ref = 3;
            s1.arg = 1;
            s1.fvalue = 0.0f;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_ChooseSubAction(0x28, p->id, 0, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        *o1 = s1;
        *o2 = s2;
        return 0x28;
    }
    return 0;
}
/* 0x49d030, execute callback 0x28 */

int ai_ExecuteThreaten(G5CH *p, AIARG *s1, AIARG *s2)
{
    return aip_ExecDrohen(p, s1, s2) ? 0x28 : 0;
}
/* 0x49d050, eval callback 0x28: decision 3 decides the rating; none while f is set */

int ai_EvalPamphlets(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(3, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d0f0, decide callback 0x29 "Pamphlete": as 0x28 (house first), then the character
   ai_PickPamphletTarget finds; flag 0x100 of flags1e8 makes it rare (1 in 4) */

int ai_DecidePamphlets(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIARG t1;
    AIARG t2;
    int r;

    if (f == 0 && (!(p->flags1e8 & 0x100) || util_Random(4) == 0)) {
        if (p->bld2 == 0 && p->kind != 3) {
            /* no house: get one first */
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_ChooseSubAction(0x28, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        } else {
            if (p->kind == 3) {
                /* kind 3 only with flag 4 */
                if (!(p->flags1e8 & 4))
                    return 0;
            } else if (gm_Query(p->bld2->objList, 2, 6, 0, 0x15) == 0) {
                /* a house without the good 0x15: buy it for the house */
                s1.kind = 2;
                s1.ref = 0x15;
                s1.arg = 1;
                s1.fvalue = 0.44f;
                s2.kind = 4;
                s2.ref = p->bld2->id;
                s2.arg = 1;
                r = ai_ChooseSubAction(0x29, p->id, 1, &s1, &s2);
                if (r != 0) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            memset(&t1, 0, sizeof(t1));
            memset(&t2, 0, sizeof(t2));
            if (ai_PickPamphletTarget(p, &t1, &t2) == 0)
                return 0;
            *o1 = t1;
            *o2 = t2;
            return 0x29;
        }
    }
    return 0;
}
/* 0x49d280, execute callback 0x29: sets flag 0x100 of +0x1e8 */

int ai_ExecutePamphlets(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (aip_ExecPamphlet(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x100, 0);
        return 0x29;
    }
    return 0;
}
/* 0x49d2d0, decide callback 0x2a "Anschwaerzen": as 0x28 (house first), then the pair of characters
   ai_DecideCharPair finds; flag 0x200 of flags1e8 makes it rare (1 in 8) */

int ai_DecideDenounce(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int r;

    if (f == 0 && (!(p->flags1e8 & 0x200) || util_Random(8) == 0)) {
        if (p->bld2 == 0 && p->kind != 3) {
            /* no house: get one first */
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_ChooseSubAction(0x28, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        } else {
            if (p->kind == 3) {
                /* kind 3 only with flag 4 */
                if (!(p->flags1e8 & 4))
                    return 0;
            } else if (gm_Query(p->bld2->objList, 2, 6, 0, 0x15) == 0) {
                /* a house without the good 0x15: buy it for the house */
                s1.kind = 2;
                s1.ref = 0x15;
                s1.arg = 1;
                s1.fvalue = 0.44f;
                s2.kind = 4;
                s2.ref = p->bld2->id;
                s2.arg = 1;
                r = ai_ChooseSubAction(0x28, p->id, 1, &s1, &s2);
                if (r != 0) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            memset(&s1, 0, sizeof(s1));
            memset(&s2, 0, sizeof(s2));
            if (ai_DecideCharPair(p, &s1, &s2) == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return 0x2a;
        }
    }
    return 0;
}
/* 0x49d460, execute callback 0x2a: sets flag 0x200 of +0x1e8 */

int ai_ExecuteDenounce(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (aip_ExecAnschwaerzen(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x200, 0);
        return 0x2a;
    }
    return 0;
}
/* 0x49d4b0, eval callback 0x2a: decision 4 decides the rating; none while f is set */

int ai_EvalInsult(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(4, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d550, decide callback 0x2b: what 0x4aa120 finds for the kind 0x10 / 8 */

int ai_DecideInsult(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (f == 0) {
        memset(&s1, 0, sizeof(s1));
        memset(&s2, 0, sizeof(s2));
        s1.arg = 0;
        s1.kind = 0x10;
        s1.ref = 8;
        if (ai_DecideVisitAction(a, p, 0, &s1, &s2) != 0) {
            *o1 = s1;
            *o2 = s2;
            return 0x2b;
        }
    }
    return 0;
}
/* 0x49d5e0, execute callback 0x2b */

int ai_ExecuteInsult(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2b : 0;
}
/* 0x49d600, eval callback 0x2b: decision 0x16 decides the rating; none while f is set */

int ai_EvalBlackmail(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x16, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_EvalGuiltWeights(a, b, i, (AIPER *)p, &d);
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d6a0, decide callback 0x2c: what 0x4aa120 finds for the kind 0x10 / 4 */

int ai_DecideBlackmail(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (f == 0) {
        memset(&s1, 0, sizeof(s1));
        memset(&s2, 0, sizeof(s2));
        s1.arg = 0;
        s1.kind = 0x10;
        s1.ref = 4;
        if (ai_DecideVisitAction(a, p, 0, &s1, &s2) != 0) {
            *o1 = s1;
            *o2 = s2;
            return 0x2c;
        }
    }
    return 0;
}
/* 0x49d730, execute callback 0x2c */

int ai_ExecuteBlackmail(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2c : 0;
}
/* 0x49d750, decide callback 0x2d: what 0x4aa120 finds */

int ai_DecideUsePrivileges(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;

    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    s1.kind = 0;
    s1.ref = 0;
    s1.arg = 0;
    if (ai_DecideVisitAction(0, p, 0, &s1, &s2) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x2d;
}
/* 0x49d7d0, execute callback 0x2d */

int ai_ExecuteUsePrivileges(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2d : 0;
}
/* 0x49d7f0, eval callback 0x2e "Buergermeister" (only for the holder of office 0xf): rated on the current need
   and three times higher when statistic 3 of ord_GetStatistics is above (mood[10][0] - 2) * 0.4 */

int ai_EvalMayor(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    float d[10];

    if (p->officeKind != 0xf) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    ord_GetStatistics(d);
    if (d[3] > (float)((p->mood[10][0] - 2.0) * 0.4f)) {
        ai_EvalCurrentNeed(a, b, i, (AIPER *)p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x49d8d0, decide callback 0x2e "Buergermeister" (only for the holder of office 0xf), with the money of the
   list in object 0x115 of the first building of kind 1: when statistic 3 of ord_GetStatistics is above
   (mood[10][0] - 2) * 0.4, upgrade a free building of type 1 to type 2 or of type 2 to type 3 (o1 kind 4,
   ref 2 / 3, o2 the building),
   else build type 1 (ai_CanBuildType); otherwise renovate the lowest-level building of kind 1 (level < 0x50,
   sub-action 0xf) */

int ai_DecideMayor(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *best, *rec, *b;
    AIBLD *spare[3];     /* per building type 1..3 (index type - 1): the last one not flagged 0x45 */
    AIOBJ *o;
    unsigned char k;
    unsigned short fl;
    char lvl;
    int count[3];        /* per building type 1..3: how many */
    int n, money, v;
    float d[10];

    best = 0;
    spare[0] = 0;
    spare[1] = 0;
    count[0] = 0;
    count[1] = 0;
    if (a != 0 || f != 0 || p->officeKind != 0xf)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(o->list, ai_OwnPlayer);
    ord_GetStatistics(d);
    rec = ai_BldsT;
    for (n = 0x100; n != 0; n--, rec++) {
        k = rec->type;
        if (k == 0 || ai_BTypesT[rec->type].kind != 1)
            continue;
        fl = rec->flags5aw;     /* the original tests fl again below (no threading of the first test) */
        if (rec->flags5aw & 0x45)
            continue;
        lvl = rec->level;
        if (lvl < 0x50 && (best == 0 || best->level > lvl))
            best = rec;
        if (k == 3)
            continue;
        if (k == 2) {
            count[1]++;
            if (!(fl & 0x45))
                spare[1] = rec;
        } else {
            count[0]++;
            if (!(fl & 0x45))
                spare[0] = rec;
        }
    }
    if (d[3] > (float)((p->mood[10][0] - 2.0) * 0.4f)) {
        if (count[0] != 0 && spare[0] != 0) {
            o1->kind = 4;
            o1->ref = 2;
            if (gm_BuildingKindValue(2) * 0.4f > money)
                return 0;
            o1->arg = 1;
            o2->kind = 4;
            o2->ref = spare[0]->id;
            o2->arg = 1;
            return 0x2e;
        }
        if (count[1] != 0 && spare[1] != 0) {
            o1->kind = 4;
            o1->ref = 3;
            if (gm_BuildingKindValue(3) * 0.4f > money)
                return 0;
            o1->arg = 1;
            o2->kind = 4;
            o2->ref = spare[1]->id;
            o2->arg = 1;
            return 0x2e;
        }
        {
        AIARG s1;
        AIARG s2;

            s1.kind = 5;
            s1.ref = 1;
            if (gm_BuildingKindValue(1) * 1.2f > money)
                return 0;
            s1.arg = 1;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            if (ai_CanBuildType(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return 0x2e;
        }
    }
    if (best == 0)
        return 0;
    {
        AIARG s1;
        AIARG s2;

    lvl = best->level;
    if (lvl - util_Random(0x20) >= 0x42)
        return 0;
    s1.ref = best->id;
    s1.kind = 4;
    s1.arg = 1;
    if (lvl < 0x21)
        s1.arg2 = 0x32;
    else
        s1.arg2 = lvl < 0x42 ? 30 : 10;
    s2.ref = o->objId;
    s2.arg = money;
    s2.arg2 = 1;
    s2.kind = 1;
    v = ai_ChooseSubAction(0xf, p->id, 0, &s1, &s2);
    if (v == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return v;
    }
}
/* 0x49dc10, execute callback 0x2e: upgrades ("upgr_arbeiter_unterk", s2->kind 4) or builds
   ("bau_arbeiter_unterk") the workers' lodging, paid to 0x504a80(1) */

int ai_ExecuteMayor(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIOBJ *c;

    c = (AIOBJ *)gm_PlayerSeat(1);
    if (c == 0 || p->officeKind != 0xf)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.fromId = s2->ref;
        m.flags = 1;
        m.time = ai_Time;
        m.arg = s2->ref;
        cm_BeginGroup("upgr_arbeiter_unterk");
        cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("upgr_arbeiter_unterk");
        return 0x2e;
    }
    cm_BeginGroup("bau_arbeiter_unterk");
    if (ai_ExecBuildBuilding((AIPER *)ai_PlayerChar, s1, s2) != 0) {
        cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, gm_BuildingKindValue(s1->ref));
        cm_EndGroup("bau_arbeiter_unterk");
        return 0x2e;
    }
    cm_EndGroup("bau_arbeiter_unterk");
    return 0;
}
/* 0x49dd90, decide callback 0x15 (characters with +0x166 == 0xf): up to four tries to find a good to sell
   (as 0x499400 in am.c) */

int ai_DecideMayorUpgrade(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    LARGE_INTEGER m;
    int n, x, w, z, r;

    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 6 && ((unsigned char *)p)[0x130] != 9)
        return 0;
    for (n = 4; n != 0; n--) {
        m.LowPart = 2;
        m.HighPart = 0;
        if (!ai_PickActionBuilding((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
            continue;
        gm_FindById((int)&y, 0, 0, x);
        if (y == 0)
            continue;
        if (!ai_FindGoodToMake(&w, &z, y, 0))
            continue;
        s1.kind = 2;
        s1.ref = w;
        s1.arg = 1;
        if (z != -1) {
            s2.kind = 1;
            s2.ref = z;
        } else {
            s2.kind = 4;
            s2.ref = y->id;
        }
        s2.arg = 1;
        s2.fvalue = 0.44f;
        r = ai_ChooseSubAction(0x15, p->id, 1, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    return 0;
}
/* 0x49def0, decide callback of "Buergermeister Rathaus" (0x31, characters with +0x166 == 0xf): for the town
   hall found by ai_PickActionBuilding (mask 0x8000): raise its level (when low), make and sell a good there,
   or upgrade it (type 0x24 -> 0x25, 0x25 -> 0x26) when the statistic and the money in its object 0x115 allow */

int ai_DecideMayorTownHall(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    AIOBJ *o;
    LARGE_INTEGER m;
    float d[10];
    int x, w, z, r, money, act, v;
    char lvl;

    m.LowPart = 0x8000;
    m.HighPart = 0;
    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 7 && ((unsigned char *)p)[0x130] != 6)
        return 0;
    if (!ai_PickActionBuilding((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (y == 0)
        return 0;
    o = gm_Query(y->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(o->list, ai_OwnPlayer);
    if (!(y->flags5a & 4) && y->level < util_Random(0x40) + 0x25) {
        s1.kind = 4;
        s1.ref = y->id;
        s1.arg = 1;
        lvl = y->level;
        if (lvl < 0x21)
            s1.arg2 = 0x32;
        else
            s1.arg2 = lvl < 0x42 ? 30 : 10;
        s2.kind = 1;
        s2.ref = o->objId;
        s2.arg = money;
        s2.arg2 = 1;
        r = ai_ChooseSubAction(0xf, p->id, 0, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    if (ai_FindGoodToMake(&w, &z, y, 0)) {
        s1.kind = 2;
        s1.ref = w;
        s1.arg = 1;
        if (z != -1) {
            s2.kind = 1;
            s2.ref = z;
        } else {
            s2.kind = 4;
            s2.ref = y->id;
        }
        s2.arg = 1;
        s2.fvalue = 0.44f;
        r = ai_ChooseSubAction(0x15, p->id, 1, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    if (y->flags5a & 0x41)
        return 0;
    ord_GetStatistics(d);
    if (y->type == 0x24) {
        act = 0x25;
        if (util_Random(0x200) + d[6] < 1250.0f)
            return 0;
    } else if (y->type == 0x25) {
        act = 0x26;
        if (util_Random(0x400) + d[6] < 5000.0f)
            return 0;
    } else
        return 0;
    if (gm_BuildingKindValue(act) * 0.4f > money)
        return 0;
    o1->kind = 4;
    o1->ref = act;
    o1->arg = 1;
    o1->arg2 = 0;
    o2->kind = 4;
    o2->ref = y->id;
    o2->arg2 = 0;
    o2->arg = 1;
    return 0x31;
}
/* 0x49e230, execute callback 0x31: upgrades the town hall ("upgr_rathaus"), paid to 0x504a80(1) */

int ai_ExecuteMayorTownHall(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIOBJ *c;

    c = (AIOBJ *)gm_PlayerSeat(1);
    if (c == 0)
        return 0;
    if (p->officeKind != 0xf)
        return 0;
    if (s2->kind != 4 && s1->arg2 != 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.to = p->netId;
    m.fromId = s2->ref;
    m.flags = 1;
    m.time = ai_Time;
    m.arg = s2->ref;
    cm_BeginGroup("upgr_rathaus");
    cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_rathaus");
    return 0x31;
}
/* 0x49e350, decide callback of "Buergermeister Kerker" (0x3c, characters with +0x166 == 0xf): as
   ai_DecideMayorTownHall for the prison found with mask 0x40, paid from the seat object of player 1
   (gm_PlayerSeat); upgrades type 0x11 -> 0x12, 0x12 -> 0x13 */

int ai_DecideMayorPrison(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    AIOBJ *o;
    LARGE_INTEGER m;
    float d[10];
    int x, w, z, r, money, act;
    char lvl;

    gm_PlayerSeat(1);
    m.LowPart = 0x40;
    m.HighPart = 0;
    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 7 && ((unsigned char *)p)[0x130] != 6)
        return 0;
    if (!ai_PickActionBuilding((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (y == 0)
        return 0;
    o = (AIOBJ *)gm_PlayerSeat(1);
    if (o == 0)
        return 0;
    money = gm_SumListAmounts(o->list, ai_OwnPlayer);
    if (!(y->flags5a & 4) && y->level < util_Random(0x40) + 0x25) {
        s1.kind = 4;
        s1.ref = y->id;
        s1.arg = 1;
        lvl = y->level;
        if (lvl < 0x21)
            s1.arg2 = 0x32;
        else
            s1.arg2 = lvl < 0x42 ? 30 : 10;
        s2.kind = 1;
        s2.ref = o->objId;
        s2.arg = money;
        s2.arg2 = 1;
        r = ai_ChooseSubAction(0xf, p->id, 0, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    if (ai_FindGoodToMake(&w, &z, y, 0)) {
        s1.kind = 2;
        s1.ref = w;
        s1.arg = 1;
        if (z != -1) {
            s2.kind = 1;
            s2.ref = z;
        } else {
            s2.kind = 4;
            s2.ref = y->id;
        }
        s2.arg = 1;
        s2.fvalue = 0.44f;
        r = ai_ChooseSubAction(0x15, p->id, 1, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    if (y->flags5a & 0x41)
        return 0;
    ord_GetStatistics(d);
    if (y->type == 0x11) {
        act = 0x12;
        if (util_Random(0x200) + d[6] < 1250.0f)
            return 0;
    } else if (y->type == 0x12) {
        act = 0x13;
        if (util_Random(0x400) + d[6] < 5000.0f)
            return 0;
    } else
        return 0;
    if (gm_BuildingKindValue(act) * 0.4f > money)
        return 0;
    o1->kind = 4;
    o1->ref = act;
    o1->arg = 1;
    o1->arg2 = 0;
    o2->kind = 4;
    o2->ref = y->id;
    o2->arg = 1;
    o2->arg2 = 0;
    return 0x3c;
}
/* 0x49e680, execute callback 0x3c: upgrades ("upgr_kerker"), paid to 0x504a80(1) */

int ai_ExecuteMayorPrison(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIOBJ *c;

    c = (AIOBJ *)gm_PlayerSeat(1);
    if (c == 0)
        return 0;
    if (p->officeKind != 0xf)
        return 0;
    if (s2->kind != 4 && s1->arg2 != 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.to = p->netId;
    m.fromId = s2->ref;
    m.flags = 1;
    m.time = ai_Time;
    m.arg = s2->ref;
    cm_BeginGroup("upgr_kerker");
    cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_kerker");
    return 0x3c;
}
/* 0x49e7a0, decide callback of "Baumeister" (0x2f, characters with +0x166 == 0xa, from year 4 on, when the
   object 0x115 of building 1 holds (rand(3000) + 1000) * 32): raise the level of a building of kind 3; else
   (1 in 8) upgrade a free building of kind 3 to type 9, one of type 9 to type 10, or build type 8 */

int ai_DecideMasterBuilder(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *best[3], *b, *rec;           /* best / cnt by type - 8: [0] type 8, [1] type 9 ([2], type 10, unused) */
    AIOBJ *o;
    char lvl;
    int cnt[3];
    int i, money, r;

    best[0] = 0;
    best[1] = 0;
    cnt[0] = 0;
    cnt[1] = 0;
    if (ai_Time.year < 4 || f != 0 || ((unsigned char *)p)[0x166] != 0xa || a != 0)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(o->list, ai_OwnPlayer);
    if (money < (util_Random(3000) + 1000) * 32)
        return 0;
    rec = ai_BldsT;
    for (i = 0; i < 0x100; i++, rec++) {
        if (rec->type == 0 || ai_BTypesT[rec->type].kind != 3 || (rec->flags5a & 4))
            continue;
        lvl = rec->level;
        if (lvl < util_Random(0x40) + 0x25) {
            AIARG s1;
            AIARG s2;

            s1.kind = 4;
            s1.ref = rec->id;
            s1.arg = 1;
            if (lvl < 0x21)
                s1.arg2 = 0x32;
            else
                s1.arg2 = lvl < 0x42 ? 30 : 10;
            s2.ref = o->objId;
            s2.arg = money;
            s2.kind = 1;
            s2.arg2 = 1;
            r = ai_ChooseSubAction(0xf, p->id, 0, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        }
        if (rec->type == 0xa)
            continue;
        if (rec->type == 9) {
            cnt[1]++;
            if (!(rec->flags5a & 0x41))
                best[1] = rec;
        } else {
            cnt[0]++;
            if (!(rec->flags5a & 0x41))
                best[0] = rec;
        }
    }
    if (util_Random(8) != 0)
        return 0;
    if (cnt[0] != 0 && best[0] != 0) {
        o1->kind = 4;
        o1->ref = 9;
        o1->arg = 1;
        o2->kind = 4;
        o2->ref = best[0]->id;
        o2->arg = 1;
        return 0x2f;
    }
    if (cnt[1] != 0 && best[1] != 0) {
        o1->kind = 4;
        o1->ref = 0xa;
        o1->arg = 1;
        o2->kind = 4;
        o2->ref = best[1]->id;
        o2->arg = 1;
        return 0x2f;
    }
    {
        AIARG s1;
        AIARG s2;

        s1.ref = 8;
        s1.arg = 1;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        if (ai_CanBuildType(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
            return 0;
        *o1 = s1;
        *o2 = s2;
        return 0x2f;
    }
}
/* 0x49ea90, execute callback of "Baumeister" (0x2f): upgrades the well ("upgr_brunnen", s2->kind 4: pays 0.3
   of the new type's value, sends the upgrade message 0x1e) or builds it ("bau_brunnen"), paid from the seat
   object of player 1; then doubles weight s1->arg2 (1..3) of the action's second weight set
   @deep deep s26: climb round 1 (42 mutants): nothing. = control: n in a block of its own, n set inside
   the if from a re-read s1->arg2, nested ifs, 2.0f * w, w + w, return through n. flip.py --want 'mov
   eax,[esi+16]': only swapping n = s1->arg2 with its test (meaning) and moving the else's return 0 out.
   c2trace: s1#1 (4 refs, prio 32) takes esi first, n#1 (prio 18, avail eax..edi) then takes esi (n's def
   is s1's last use); the original's eax for n means n was served before s1 (n's priority above s1's) or s1
   lived past n's def. Left: n in esi vs eax (4 lines, 4 bytes). prior: read whole against the original:
   the guards, the upgrade message (AIMSG fromId / flags / arg[0]), both payment paths, the failure path
   placed last (if (build) {...} else {end group; return 0}) and the weight doubling all match, 105/105
   instructions. Left is one register choice: n = s1->arg2 goes to esi (s1's register, dead) in ours, to
   eax in the original: '- mov esi, dword ptr [esi + 0x10] / test esi, esi / cmp esi, 4 / fstp [esi*8 + X]'
   vs '+ mov eax, dword ptr [esi + 0x10] / test eax, eax / cmp eax, 4 / fstp [eax*8 + X]'; reading s1->arg2
   without a local gives the same code. */

int ai_ExecuteMasterBuilder(G5CH *p, AIARG *s1, AIARG *s2)
{
    AIMSG m;
    AIOBJ *c;
    int n;

    c = (AIOBJ *)gm_PlayerSeat(1);
    if (c == 0 || ((unsigned char *)p)[0x166] != 0xa)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.fromId = s2->ref;
        m.flags = 1;
        m.time = ai_Time;
        m.arg[0] = s2->ref;
        cm_BeginGroup("upgr_brunnen");
        cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage(&m);
        cm_EndGroup("upgr_brunnen");
    } else {
        cm_BeginGroup("bau_brunnen");
        if (ai_ExecBuildBuilding((AIPER *)ai_PlayerChar, s1, s2) != 0) {
            cm_SendMoveGoods(-1, c->objId, ai_OwnPlayer, gm_BuildingKindValue(s1->ref));
            cm_EndGroup("bau_brunnen");
        } else {
            cm_EndGroup("bau_brunnen");
            return 0;
        }
    }
    n = s1->arg2;
    if (n > 0 && n < 4)
        G5_ACTS[0x2f].w3[n].w = G5_ACTS[0x2f].w1[0].w * 2.0f;
    return 0x2f;
}
/* 0x49ec10, eval callback of "Dunkle Ecke" (0x32): with a title job 4 (gm_TitleJob) that has more than two
   titles left, rated as the current need times 6 / 2 when ai_FindDarkCornerTarget finds a building (its pick
   into o1 / o2), else 0; without one, the mood weights from hour 20 on */

int ai_EvalDarkCorner(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    *a = -1e30f;
    *b = -1e30f;
    if (f != 0)
        return 0;
    q = gm_TitleJob(p->id, 4);
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (ai_FindDarkCornerTarget(p, &s1, &s2) != 0) {
            ai_EvalCurrentNeed(a, b, i, (AIPER *)p, 0, o1, o2);
            *o1 = s1;
            *o2 = s2;
            *a *= 6.0f;
            *b *= 2.0f;
            return 1;
        }
        return 0;
    }
    if (ai_Time.hour < 0x14)
        return 0;
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49ed10, decide callback 0x32: what 0x4aaef0 finds (when 0x5044b0(p, 4) has more than two entries),
   else what 0x4ab080 finds */

int ai_DecideDarkCorner(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    q = gm_TitleJob(p->id, 4);
    if (a != 0x32 && a != 0x25 && a != 0)
        return 0;
    if (f != 0)
        return 0;
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (o1->kind == 4 && o2->kind == 1)
            return 0x32;
        if (ai_FindDarkCornerTarget(p, &s1, &s2) != 0) {
            *o1 = s1;
            *o2 = s2;
            return 0x32;
        }
    }
    if (ai_DecideBuyDunkleEcke(p, &s1, &s2) != 0) {
        *o1 = s1;
        *o2 = s2;
        return 0x32;
    }
    return 0;
}
/* 0x49ee20, execute callback of "Dunkle Ecke" (0x32): s1 kind 4 with s2 kind 1 runs the message 0x74 on that
   building (ai_ExecMessage74); s1 kind 0x13 with s2 kind 4 sets bit 0x2000000 of field 0x1cc and buys the dark
   corner (ai_ExecBuyDunkleEcke); s1 kind 0 only sets that bit */

int ai_ExecuteDarkCorner(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (s1->kind == 4 && s2->kind == 1) {
        if (ai_ExecMessage74(p, s1, s2) != 0)
            return 0x32;
    } else if (s1->kind == 0x13 && s2->kind == 4) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        if (ai_ExecBuyDunkleEcke(p, s1, s2) != 0)
            return 0x32;
    } else if (s1->kind == 0) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        return 0x32;
    }
    return 0;
}
#define AI_WORKSHOP_KINDS 0x744180  /* mask of the building kinds (1 << kind) ai_DecidePriceManipulation counts as workshops: 7, 8, 14, 18, 20, 21, 22 */
#define AI_WORKSHOP_KINDS_HI 0  /* the same mask's kinds 32..63: none */
#define AI_CREDIT_KINDS 0x7d4390  /* mask of the building kinds (1 << kind) a credit is taken for: 4, 7, 8, 9, 14, 16, 18..22 */
/* 0x49eec0, decide callback of "Kredit Nehmen" (0x33; also asked by 0x13 / 0x10 / 0x11 with o1 kind 0x12, the sum
   needed): unless the character already runs more credits (events 0x23) than a random 0..9, picks one of its own
   buildings of the kinds AI_CREDIT_KINDS whose budget is below the hour-based limit of its class (for 0x33 only)
   and a kind-5 building of someone else, then the best of that building's three gm_GetCreditOffers */

int ai_DecideTakeCredit(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int lim[4];
    G8OFFER rec[3];
    AIDEC d;
    AIBLD *own, *b;
    int i, n, h, j, best;
    float m;

    lim[0] = ai_RandomStrides[util_Random(0x10)];
    if (((unsigned char *)p)[2] == 3)
        return 0;
    if (a != 0x33 && a != 0x13 && a != 0x10 && a != 0x11)
        return 0;
    n = 0;
    if (he_FindEvent(2, 0, 0x23, 2, p->id) != 0) {
        do
            n++;
        while (he_NextEvent() != 0);
    }
    if (util_Random(10) < n + 1)
        return 0;
    if (a == 0x33) {
        h = min(max(ai_Time.hour - 8, 0), 10);
        m = (10 - h) * 0.1f;
        m = m * m;
        lim[1] = (int)(m * 6400.0f);
        lim[2] = (int)(m * 12800.0f);
        lim[3] = (int)(m * 25600.0f);
        i = 0x100;
        j = util_Random(0x100);
        for (;;) {
            own = &ai_BldsT[j];
            if (own->type != 0 && own->owner == p->id && ((1 << ai_BTypesT[own->type].kind) & AI_CREDIT_KINDS)
                && own->budget < lim[ai_BTypesT[own->type].cap247])
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        i = 0x100;
        j = util_Random(0x100);
        for (;;) {
            b = &ai_BldsT[j];
            if (b->type != 0 && b->owner != p->id && ai_BTypesT[b->type].kind == 5)
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        n = gm_GetCreditOffers(b, p, 3, rec);
        if (n <= 0)
            return 0;
        gv_GetTableRecord(0xe, &d);
        for (i = 0; i < n; i++)
            ((float *)lim)[i + 1] = (float)(d.cur - 2) / rec[i].c * (rec[i].b * 0.14285715f) * rec[i].a;
        m = 0.0f;
        best = -1;
        for (i = 0; i < n; i++) {
            if (m < ((float *)lim)[i + 1]) {
                m = ((float *)lim)[i + 1];
                best = i;
            }
        }
        if (best < 0 && best > 2)
            return 0;
        o1->kind = 4;
        o1->ref = b->id;
        o1->arg = own->id;
        o2->kind = 0x14;
        o2->ref = rec[best].a;
        o2->arg = rec[best].b;
        o2->arg2 = rec[best].c;
        return 0x33;
    }
    if (o1->kind != 0x12)
        return 0;
    i = 0x100;
    j = util_Random(0x100);
    for (;;) {
        b = &ai_BldsT[j];
        if (b->type != 0 && b->owner != p->id && ai_BTypesT[b->type].kind == 5)
            break;
        i--;
        j = (j + lim[0]) % 0x100;
        if (i == 0)
            return 0;
    }
    n = gm_GetCreditOffers(b, p, 3, rec);
    if (n <= 0)
        return 0;
    gv_GetTableRecord(0xe, &d);
    for (i = 0; i < n; i++)
        ((float *)lim)[i + 1] = (unsigned int)rec[i].a > o1->arg
            ? (float)(d.cur - 2) / rec[i].c * (rec[i].b * 0.14285715f) * rec[i].a : -1.0f;
    m = 0.0f;
    best = -1;
    for (i = 0; i < n; i++) {
        if (m < ((float *)lim)[i + 1]) {
            m = ((float *)lim)[i + 1];
            best = i;
        }
    }
    if (best < 0 || best > 2)
        return 0;
    o1->kind = 4;
    o1->ref = b->id;
    o1->arg = -1;
    o2->kind = 0x14;
    o2->ref = rec[best].a;
    o2->arg = rec[best].b;
    o2->arg2 = rec[best].c;
    return 0x33;
}
/* 0x49f370, execute callback 0x33: the AI credit ("AI Kredit"): message 0x23 from the lender's building s1->unk04
   to the owner of s1->unk08, the offer s2 (0x14) attached; the money is paid; always returns 0 */

int ai_ExecuteTakeCredit(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIBLD *a;
    AIBLD *b;
    int v;

    if (s1->kind != 4 || s2->kind != 0x14)
        return 0;
    gm_FindById((int *)&a, 0, 0, s1->ref);
    if (a == 0)
        return 0;
    gm_FindById((int *)&b, 0, 0, s1->arg);
    memset(&m, 0, sizeof(m));
    m.type = 0x23;
    m.to = ai_CharsT[a->owner].netId;
    m.fromId = a->id;
    m.time = ai_Time;
    m.flags = 0x12;
    m.arg = a->id;
    m.from = p->netId;
    m.credit.amount = s2->ref;
    m.credit.term = s2->arg;
    m.credit.interest = s2->arg2;
    v = s2->ref;
    cm_BeginGroup("AI Kredit");
    cm_SendMoveGoods(p->netId, a->id, ai_OwnPlayer, s2->ref);
    cm_SendLongMessage((AIMSG *)&m);
    if (b != 0) {
        cm_BeginFieldBlock((AIPER *)b, b->id);
        cm_AddFieldValues(4, 1, (char *)b - cm_FieldObject + 0x39, &v);
        cm_SendFieldDeltas();
    }
    cm_EndGroup("AI Kredit");
    return 0;
}
/* 0x49f4f0, eval callback 0x34: only for a kind-5 character without 0x169; with 3..5 entries in its list 0x164
   a random chance of the plain rating, with 6 or more the doubled / tripled rating */

int ai_EvalChangeProfession(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int n;

    *a = -100.0f;
    *b = -100.0f;
    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    if (((unsigned char *)p)[2] != 5 || ((unsigned char *)p)[0x169] != 0)
        return 0;
    n = gm_TitlesLeft(((unsigned char *)p)[0x164]);
    if (n < 3)
        return 0;
    if (n < 6) {
        if (util_Random(n - 1) == 0)
            return 0;
        return ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
    }
    ai_EvalCurrentNeed(a, b, i, (AIPER *)p, 0, o1, o2);
    *a += *a;
    *b *= 3.0f;
    return 1;
}
/* 0x49f5c0, decide callback of "Change Profession" (0x34): a kind-5 character (money field >= 4, currency / 32
   >= 5000, no event 0x24 for it) picks a random title class r (1..15) of which its player record (+0x75) has
   no titles left and fewer than 1..4 buildings run that job, and builds the first building of that class's
   title (gm_TitleNumber / gm_FirstTitleNumber) when it can pay for it; o1->arg2 is the title */

int ai_DecideChangeProfession(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int cnt[17];
    int own[17];
    AIFINX *q;
    AIJOB *j;
    AIBTYPE *bt;
    int money, k, n, r, c, x, y;

    q = (AIFINX *)gm_PlayerRecord((AICHAR *)p);
    if (p->kind != 5 || p->money < 4 || q == 0)
        return 0;
    money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (money / 32 < 5000)
        return 0;
    for (j = he_FindEvent(1, 0, 0x24); j != 0; j = he_NextEvent()) {
        if (j->ownerCh == (int)p->netId)
            return 0;
    }
    gm_TitlesLeft(((unsigned char *)p)[0x164]);
    for (k = 1; k < 16; k++) {
        cnt[k] = 0;
        own[k] = gm_TitlesLeft(q->jobByGroup[k]);
    }
    for (k = 0; k < 0x100; k++) {
        if (ai_BldsT[k].type != 0) {
            bt = &ai_BTypesT[ai_BldsT[k].type];
            cnt[gm_BuildingJob((unsigned char)bt->kind)]++;
        }
    }
    n = util_Random(4) + 1;
    r = util_Random(0x10);
    for (c = 0x10;; ) {
        if (r != 0 && own[r] == 0 && cnt[r] < n)
            break;
        r = (r + 1) % 0x10;
        if (--c == 0)
            return 0;
    }
    if (r == -1)
        return 0;
    x = gm_TitleNumber(r, 1);
    y = gm_FirstTitleNumber(x);
    if (y == 0)
        return 0;
    s1.ref = y;
    s1.kind = 5;
    s1.arg = 1;
    s2.kind = 0;
    s2.ref = 0;
    s2.arg = 0;
    if (ai_CanBuildType(a, p, f, &s1, &s2, 1) == 0)
        return 0;
    if (money < gm_BuildingKindValue(s1.ref))
        return 0;
    *o1 = s1;
    o1->arg2 = x;
    *o2 = s2;
    return 0x34;
}
/* 0x49f7f0, execute callback of "Change Profession" (0x34): builds the building ai_DecideChangeProfession
   chose, marks the character AI_CHF_BUILT, makes the chosen title class (s1->arg2) its job and books the
   money delta 8 */

int ai_ExecuteChangeProfession(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (ai_ExecBuildBuilding((AIPER *)p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1c8, 4, AI_CHF_BUILT, 0);
        cm_SendSetJob(p->netId, s1->arg2);
        gm_SendMoneyDelta(p->netId, 8);
        return 0x34;
    }
    return 0;
}
/* 0x49f850, decide callback of "Stammtisch" (0x35), the regulars' table: finds up to four kind-0x37 buildings
   of other owners with a table object 0x12d (G5STAMM). A member of one ('leav' it): alone there 1 in 4, else
   when its mean liking of the others / 100 is above 0.666 and a random + 1/6 stays below it; not a member:
   'join' a table with a free seat 1 in 4 (o2 = its owner); 'noop' (kind 0) otherwise */

int ai_DecideStammtisch(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl[4];
    G5STAMM *ob[4];
    int free[4];
    AIBLD *b;
    G5STAMM *o;
    AICHAR *c;
    int found, step, i, j, n, k, m, v;
    float sum;

    step = ai_RandomStrides[util_Random(0x10)];
    found = -1;
    if (p->flags1c8 & AI_CHF_STAMMTISCH)
        return 0;
    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    n = 0;
    i = 0x100;
    j = util_Random(i);
    while (n < 4 && i != 0) {
        b = &ai_BldsT[j];
        if (b->type == 0x37 && b->owner != 0xffff && b->owner != p->id) {
            o = (G5STAMM *)gm_Query(b->objList, 2, 6, 0, 0x12d);
            if (o != 0) {
                bl[n] = b;
                ob[n] = o;
                n++;
            }
        }
        i--;
        j = (j + step) % 0x100;
    }
    if (n == 0) {
        if (util_RandomF() > 0.33f)
            goto noop; /* @original one noop tail shared with the join path; written out here it is scheduled apart */
        return 0;
    }
    for (k = 0; k < n; k++) {
        free[k] = 0;
        for (m = 0; m < 4; m++) {
            v = ob[k]->member[m];
            free[k] |= v == -1;
            if (found == -1)
                found = v == (int)p->netId ? k : -1;
        }
    }
    if (found != -1) {
        sum = 0.0f;
        k = 0;
        for (m = 0; m < 4; m++) {
            c = gm_FindChar(ob[found]->member[m]);
            if (c != 0 && c != (AICHAR *)p) {
                k++;
                sum = gm_Liking(p->id, c->id, 1) + sum;
            }
        }
        if (k == 0 && util_Random(4) == 0) {
            o1->kind = 4;
            o1->ref = bl[found]->id;
            o1->arg = 'leav';
            return 0x35;
        }
        sum = sum / (k * 100);
        if (sum > 0.666f) {
            if (util_RandomF() + 0.166667f < sum) {
                o1->kind = 4;
                o1->ref = bl[found]->id;
                o1->arg = 'leav';
                return 0x35;
            }
            o1->kind = 0;
            o1->ref = -1;
            o1->arg = 'noop';
            return 0x35;
        }
        return 0;
    }
    j = util_Random(n);
    for (i = n; i != 0; ) {
        if (free[j] == 1) {
            found = j;
            break;
        }
        i--;
        j = (j + 1) % n;
    }
    if (found == -1)
        return 0;
    if (util_RandomF() > 0.75f) {
        o1->kind = 4;
        o1->ref = bl[found]->id;
        o1->arg = 'join';
        o2->kind = 0x12;
        o2->ref = ai_CharsT[bl[found]->owner].netId;
        o2->arg = 0;
        return 0x35;
    }
noop:
    o1->kind = 0;
    o1->ref = -1;
    o1->arg = 'noop';
    return 0x35;
}
/* 0x49fb80, execute callback of "Stammtisch" (0x35): sends the 'join' / 'leav' request ai_DecideStammtisch chose
   (cm_SendTableRequest) and marks the character AI_CHF_STAMMTISCH ('noop' only marks it) */

int ai_ExecuteStammtisch(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8TABLE r;

    if (s1->arg == 'join') {
        r.tag = s1->arg;
        r.ch = p->netId;
        r.bld = s1->ref;
        cm_BeginGroup("stammtisch join AI-Plr");
        cm_SendTableRequest((G17TABLEMSG *)&r);
        cm_SendSetField(p->netId, 0x1c8, 4, AI_CHF_STAMMTISCH, 0);
        cm_EndGroup("stammtisch join AI-Plr");
        return 0x35;
    }
    if (s1->arg == 'leav') {
        r.tag = s1->arg;
        r.ch = p->netId;
        r.bld = s1->ref;
        cm_BeginGroup("stammtisch leave AI-Plr");
        cm_SendTableRequest((G17TABLEMSG *)&r);
        cm_SendSetField(p->netId, 0x1c8, 4, AI_CHF_STAMMTISCH, 0);
        cm_EndGroup("stammtisch leave AI-Plr");
        return 0x35;
    }
    if (s1->arg == 'noop') {
        cm_SendSetField(p->netId, 0x1c8, 4, AI_CHF_STAMMTISCH, 0);
        return 0x35;
    }
    return 0;
}
/* 0x49fc90, decide callback of "Zunft Preismanipulation" (0x36): a mayor whose bonus step is 1 counts, per job
   group 1..7, the workshop levels (AI_WORKSHOP_KINDS, cap247) of all owners and its own, and the best type; scores
   each group by its prices (ai_GroupPrices) and its share, then pushes the prices of the lowest-scored group
   (o1->ref = group, o1->arg2 = -1 down / +1 up) */

int ai_DecidePriceManipulation(int a, G5CH *p, int f, AIARG *o1)
{
    G7MKT m[7];
    float score[7];
    unsigned int best[7];
    int all[7];
    int own[7];
    AIBLD *b;
    AIBTYPE *bt;
    LARGE_INTEGER k;
    unsigned int mlo, mhi;
    unsigned char hit;
    int j, g, i, lo, hi, d;
    float v;

    mlo = AI_WORKSHOP_KINDS;
    mhi = AI_WORKSHOP_KINDS_HI;
    if (a == 0 && f == 0 && amt_MayorBonusStep(p) == 1) {
        AI_ZERO(own, sizeof(own));
        AI_ZERO(all, sizeof(all));
        AI_ZERO(best, sizeof(best));
        for (j = 0; j < 0x100; j++) {
            b = &ai_BldsT[j];
            if (b->type != 0 && b->owner != 0xffff) {
                bt = &ai_BTypesT[b->type];
                k.LowPart = 0;
                k.HighPart = 0;
                if ((unsigned char)bt->kind > 0x1f)
                    k.HighPart = 1 << ((unsigned char)bt->kind - 0x20);
                else
                    k.LowPart = 1 << (unsigned char)bt->kind;
                hit = (k.LowPart & mlo) != 0 || (k.HighPart & mhi) != 0;
                if (hit && (g = gm_BuildingJob((unsigned char)bt->kind)) != 0 && g <= 7) {
                    g--;
                    if (b->owner == p->id)
                        own[g] += bt->cap247;
                    all[g] += bt->cap247;
                    if (best[g] == 0)
                        best[g] = b->type;
                    else if (bt->cap247 > ai_BTypesT[best[g]].cap247)
                        best[g] = b->type;
                }
            }
        }
        lo = -1;
        hi = -1;
        for (i = 0; i < 7; i++) {
            score[i] = -1.0f;
            m[i].key = best[i];
            m[i].n = 0;
            all[i] = all[i] < 15 ? all[i] : 15;
            own[i] = own[i] < 15 ? own[i] : 15;
            if (all[i] > 0 && ai_GroupPrices(&m[i]) != 0) {
                v = ((float)all[i] + 1.0f - (float)own[i]) / (float)all[i]
                    * ((16.0f - (float)own[i]) * 0.06666667f) * m[i].avg;
                score[i] = v;
                if (lo == -1)
                    lo = i;
                else if (v < score[lo])
                    lo = 1;             /* sic: the original stores 1, not i */
                if (hi == -1)
                    hi = i;
                else if (v > score[hi])
                    hi = i;
            }
        }
        if (lo != -1 && hi != -1 && lo != hi) {
            d = 0;
            if (m[lo].avg > 1.0f || own[lo] == 0) {
                if (m[hi].avg < 1.0f)
                    return 0;
                if (own[hi] * 2 + 1 >= all[hi])
                    return 0;
                d = -1;
            }
            if (m[hi].avg < 1.0f || own[hi] * 2 + 1 >= all[hi]) {
                if (m[lo].avg > 1.0f || own[lo] == 0)
                    return 0;
                d = 1;
            }
            if (d == 0)
                d = util_Random(4) - 1;
            if (d != -1 && d != 1)
                return 0;
            o1->kind = 0x15;
            o1->ref = lo + 1;
            o1->arg2 = d;
            return 0x36;
        }
    }
    return 0;
}
/* 0x4a0040, execute callback 0x36: hands the request (3, s1->unk04, s1->unk10) of s1 (kind 0x15) to 0x4e9660 */

int ai_ExecutePriceManipulation(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind == 0x15) {
        r.kind = 3;
        r.a = s1->ref;
        r.b = s1->arg2;
        amt_ExecutePriceMessage(p, &r);
        return 0x36;
    }
    return 0;
}
/* 0x4a0090, decide callback 0x37: when 0x4e94e0 allows it and the character can pay a price of
   max(0x50a640 * 0.01, 160) (at most 22 % of its money), a building (0x4a63f0 kind 6) whose object list some
   character points to */

int ai_DecideGuildRiot(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    LARGE_INTEGER m;
    int v, n, b, y, i;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (amt_CanUseMayorPower2(p) != 1)
        return 0;
    v = (int)max(gm_GetWorth(p->id) * 0.01f, 160.0f);
    if (v > (int)(gm_CharCurrency((AIPER *)p, ai_OwnPlayer) * 0.22f))
        return 0;
    m.LowPart = 0x807d4190;
    m.HighPart = 1;
    if (!ai_PickActionBuilding((AIPER *)p, 6, m.QuadPart, 0, (float)(util_Random(0x20) + 0x20), &b))
        return 0;
    gm_FindById((int)&y, 0, 0, b);
    if (y == 0)
        return 0;
    n = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].bld == (AIBLD *)y)
            n++;
    }
    if (n == 0)
        return 0;
    o1->arg2 = v;
    o1->kind = 4;
    o1->ref = b;
    return 0x37;
}
/* 0x4a01f0, execute callback 0x37: hands the request (2, s1->unk04, s1->unk10) of s1 (kind 4) to 0x4e9a00 */

int ai_ExecuteGuildRiot(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind == 4) {
        r.kind = 2;
        r.a = s1->ref;
        r.b = s1->arg2;
        amt_ExecuteShareMessage(p, &r);
        return 0x37;
    }
    return 0;
}
/* 0x4a0240, the character p likes least among its rivals: without an office the holders of an office whose
   kind (of p's previous office) has level <= 1; with one the other applicants for its applied office (kept
   only when the liking stays <= 8 * applicants + 52); else a partner-chain pick (0xc00, 25.0) */

G5CH *ai_PickLeastLikedRival(G5CH *p)
{
    G5CH *best = 0;
    G5CH *o;
    float d, bd;
    int n, i;
    AIPER *list[5];
    AMTKIND kind;
    AMTREC office;

    if (p->officeKind == 0) {
        amt_GetKind(p->prevOffice, &kind);
        for (i = 0; i < 768; i++) {
            o = &G5_CHARS[i];
            if (o->officeKind != 0 && amt_FindOffice((char *)o, &kind, &office) != 0 && kind.level <= 1) {
                if (best == 0) {
                    best = o;
                    bd = gm_Liking(p->id, o->id, 1);
                } else {
                    d = gm_Liking(p->id, o->id, 1);
                    if (d < bd) {
                        bd = d;
                        best = o;
                    }
                }
            }
        }
        return best;
    }
    if (p->appliedOffice != 0 && (n = amt_GetOfficeApplicants(p->appliedOffice, 5, (char **)list)) != 0) {
        for (i = 0; i < n; i++) {
            if (list[i] != (AIPER *)p) {
                if (best == 0) {
                    best = (G5CH *)list[i];
                    bd = gm_Liking(p->id, best->id, 1);
                } else {
                    d = gm_Liking(p->id, list[i]->id, 1);
                    if (d < bd) {
                        best = (G5CH *)list[i];
                        bd = d;
                    }
                }
            }
        }
        if (best != 0) {
            if (!((float)n * 8.0f + 52.0f < bd))
                return best;
            best = 0;
        }
    }
    {
        int flags = 0xc00;
        unsigned short id;

        if (ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, 25.0f, &id) != 0)
            return &G5_CHARS[id];
    }
    return best;
}
/* 0x4a0410, an office column (1..6) whose three or more holders p likes on average below 66, else 0: the order
   runs up (level 1..3 of p's applied / held / previous office kind), from 5 (4..6) or down (7..9), then each
   half is shuffled six times */

int ai_PickDislikedColumn(G5CH *p)
{
    int order[6];
    unsigned int k;
    AMTKIND kind;
    AMTREC list[6];
    float sum;
    int i, n, t, a, b;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (p->appliedOffice != 0)
        k = p->appliedOffice;
    else if (p->officeKind != 0)
        k = p->officeKind;
    else if (p->prevOffice != 0)
        k = p->prevOffice;
    if (k != 0) {
        if (amt_GetKind(k, &kind) == 0)
            return 0;
        i = ((int)kind.level - 1) / 3;
        if (i == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (i == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        a = util_Random(3);
        b = util_Random(3);
        t = order[a];
        order[a] = order[b];
        order[b] = t;
        a = util_Random(3) + 3;
        b = util_Random(3) + 3;
        t = order[a];
        order[a] = order[b];
        order[b] = t;
    }
    for (i = 0; i < 5; i++) {
        n = amt_GetColumnOfficesOwned(order[i], 6, list);
        if (n >= 3) {
            sum = 0.0f;
            for (t = 0; t < n; t++)
                sum += gm_Liking(p->id, ((AICHAR *)gm_FindChar(list[t].owner))->id, 1);
            if (sum / (float)n < 66.0f)
                return order[i];
        }
    }
    return 0;
}
/* 0x4a05e0: one of the six quarters (1..6) whose residents (0x4e7040) like each other (mean of 0x50c550 both ways
   over 33), tried in an order that depends on the character's home (0x168 / 0x166 / 0x167, 0x4e6f80) and is
   shuffled within 1..3 and 4..6; 0 when none */

int ai_PickLikedColumn(G5CH *p)
{
    int order[6];
    AMTKIND kind;
    G8SEAT seat[6];
    AICHAR *c1, *c2;
    unsigned int k;
    int i, j, n, d, cnt, t, x, y;
    float sum;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (p->appliedOffice != 0)
        k = p->appliedOffice;
    else if (p->officeKind != 0)
        k = p->officeKind;
    else if (p->prevOffice != 0)
        k = p->prevOffice;
    if (k != 0) {
        if (amt_GetKind(k, &kind) == 0)
            return 0;
        t = (kind.level - 1) / 3;
        if (t == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (t == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        x = util_Random(3);
        y = util_Random(3);
        t = order[x];
        order[x] = order[y];
        order[y] = t;
        x = util_Random(3) + 3;
        y = util_Random(3) + 3;
        t = order[x];
        order[x] = order[y];
        order[y] = t;
    }
    for (d = 0; d < 5; d++) {
        cnt = 0;
        sum = 0.0f;
        n = amt_GetColumnOfficesOwned(order[d], 6, seat);
        if (n >= 3) {
            for (i = 0; i < n - 1; i++) {
                if (seat[i].id == (int)p->netId || seat[i].id == -1)
                    continue;
                c1 = gm_FindChar(seat[i].id);
                for (j = i + 1; j < n && c1 != 0; j++) {
                    if (seat[j].id == (int)p->netId || seat[j].id == -1)
                        continue;
                    c2 = gm_FindChar(seat[j].id);
                    if (c2 == 0)
                        continue;
                    sum = gm_Liking(c2->id, c1->id, 1)
                        + (gm_Liking(c1->id, c2->id, 1) + sum);
                    cnt += 2;
                }
            }
            if (cnt != 0 && sum / cnt > 33.0f)
                return order[d];
        }
    }
    return 0;
}
/* 0x4a0890, decide callback 0x38 (0x4e95a0 must allow it): by the character's office 0x169: 0x1e someone
   (0x4a0240, kind 7), 0x20 / 0x1f / 0x21 a quarter (0x4a0410 / 0x4a05e0, kind 0x16) */

int ai_DecideGuildLevel3(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c;
    unsigned char k;
    int n;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (amt_CanUseMayorPower3(p) != 1)
        return 0;
    k = ((unsigned char *)p)[0x169];
    if (k == 0x1e) {
        c = ai_PickLeastLikedRival(p);
        if (c == 0)
            return 0;
        o1->kind = 7;
        o1->ref = c->netId;
        o1->arg2 = 1;
        return 0x38;
    }
    if (k == 0x20)
        n = ai_PickDislikedColumn(p);
    else if (k == 0x1f || k == 0x21)
        n = ai_PickLikedColumn(p);
    else
        return 0;
    if (n == 0)
        return 0;
    o1->ref = n;
    o1->kind = 0x16;
    o1->arg2 = 4;
    return 0x38;
}
/* 0x4a0940, execute callback 0x38: hands the request (s1->unk10, s1->unk04, 0) of s1 (kind 7 / 0x16) to
   0x4e9ce0 */

int ai_ExecuteGuildLevel3(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind != 7 && s1->kind != 0x16)
        return 0;
    r.kind = s1->arg2;
    r.a = s1->ref;
    r.b = 0;
    amt_ExecuteMayorPowerMessage(p, &r);
    return 0x38;
}
/* 0x4a0990, eval callback 0x38: by the office 0x169 (0x1e..0x21) the office building (kind 0x17..0x1a); rated
   three times higher while its level 0x5c is below 50 + a random 0..15 */

int ai_EvalUpgradeGuild(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl;
    unsigned char k;

    *a = -100.0f;
    *b = -100.0f;
    k = ((unsigned char *)p)[0x169];
    if (k < 0x1e && k > 0x21)
        return 0;
    if (k == 0x1e)
        bl = gm_BuildQuery(1, 5, 0x17);
    else if (k == 0x1f)
        bl = gm_BuildQuery(1, 5, 0x18);
    else if (k == 0x20)
        bl = gm_BuildQuery(1, 5, 0x19);
    else
        bl = gm_BuildQuery(1, 5, 0x1a);
    if (bl == 0)
        return 0;
    if (bl->level < util_Random(0x10) + 0x32) {
        ai_EvalCurrentNeed(a, b, i, (AIPER *)p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_EvalMoodWeights(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x4a0a70, decide callback of "Upgrade Zunft" (0x39) for a mayor (second office 0x1e..0x21, the guild hall
   building 0x17..0x1a): repair a run-down hall (sub-action 0xf, 50 / 30 / 10 points), else make a good for it
   (ai_FindGoodToMake, sub-action 0x39), else build the next type of its line; when the treasury (object 0x142)
   cannot pay, a credit to it (o1 kind 1, o2 kind 0x12) from the character's money above 80000 (a quarter, half
   for the new type)
   @deep 108 ins (register research). The priority is not linear in savings: prio = sum over blocks of
   refcost*P*w where referenced, -P*w per block lived through unreferenced, P = distinct values referenced
   in the block (c2trace --prio '^_(o1|money|o|lvl)$'). Tree (goto): o1 43 < money 49 (o1 +36 credit block
   P6, +56 new-type block P7, +30 end credit P5, -65 over blocks 2..27 as a live parameter). Two copies
   (build/c2dag/trace/am_two_fs_cse_c2.c): the second credit copy adds +30, o1 69 > o 60 > lvl 48 > money
   47; the original needs o1 below all four (bl esi, o ebp, money edi, ebx lvl): 23 points less, i.e. o1's
   three credit stores in blocks with fewer distinct values, or o1 living through more referencing-free
   blocks. Tried this round, = control: an empty statement or label before credit:, a label after the good
   path's goto, do..while (0) around the credit tests (block shape levers that fixed ai_FillNearRing do not
   move this layout). Next: --prio on each two-copy spelling, look for one whose copies have P <= 4 in the
   o1 store blocks. prior: Session 2 kept (108 ins, frame 0x64): literal 1 for o1->arg2 (no c), 'amt +
   0x20' written twice in both credit blocks (the 0.5 credit: amt in [esp+0x10], amt+0x20 in esi = a CSE;
   drops the money / n spill slots, money gets edi). Left: block layout + the allocation it implies. By the
   DFS block order (jump edge first, RPO) the original's 0.25 credit inline right after the repair test,
   with the good path's 'jl' back to its first instruction, needs the good path to have NO edge to the
   credit at block-order time: two full credit copies, cross-jumped after allocation (a goto from the good
   path, the credit text in the good path, goto good, if/else spellings, a goto into the stores after a
   partial copy (cut1..3, with block or function-scope AIARGs): all place the credit after the good path or
   break the frame). But every two-copy spelling (two_fs/two_blk x inc/cse, a __inline helper, a c=1
   variab... */

int ai_DecideUpgradeGuild(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl;
    AIBTYPE *t;
    AIOBJ *o;
    unsigned char k;
    char lvl;
    int money, need, amt, n, m;

    if (f != 0 || o1->kind != 0 || a != 0)
        return 0;
    k = p->secondOffice;
    if (k < 0x1e && k > 0x21)
        return 0;
    if (k == 0x1e)
        bl = gm_BuildQuery(1, 5, 0x17);
    else if (k == 0x1f)
        bl = gm_BuildQuery(1, 5, 0x18);
    else if (k == 0x20)
        bl = gm_BuildQuery(1, 5, 0x19);
    else
        bl = gm_BuildQuery(1, 5, 0x1a);
    if (bl == 0)
        return 0;
    o = (AIOBJ *)gm_Query(bl->objList, 2, 6, 0, 0x142);
    money = gm_SumListAmounts((void *)o->list, ai_OwnPlayer);
    lvl = bl->level;
    if (lvl < util_Random(0x10) + 0x32 && !(bl->flags5a & 4)) {
        AIARG s1;
        AIARG s2;

        s1.kind = 4;
        s1.ref = bl->id;
        s1.arg = 1;
        if (lvl < 0x21)
            need = 0x32;
        else
            need = lvl < 0x42 ? 30 : 10;
        s2.ref = o->objId;
        s2.arg2 = 1;
        s1.arg2 = need;
        s2.kind = 1;
        s2.arg = money;
        n = ai_ChooseSubAction(0xf, p->id, 0, &s1, &s2);
        if (n != 0) {
            *o1 = s1;
            *o2 = s2;
            return n;
        }
        amt = ai_RepairCost(bl, need);
        if (amt > money) {
credit:
            m = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
            if (m < 80000)
                return 0;
            if ((int)((m - 80000) * 0.25f) < amt + 0x20)
                return 0;
            o1->kind = 1;
            o1->ref = o->objId;
            o1->arg2 = 1;
            o2->arg = amt + 0x20;
            o2->kind = 0x12;
            return 0x39;
        }
    }
    if (ai_FindGoodToMake(&f, &a, bl, 0) != 0) {
        AIARG b1;
        AIARG b2;

        b1.kind = 2;
        b1.ref = f;
        b1.arg = 1;
        b1.ref2 = o->objId;
        b1.arg2 = 0x39;
        b2.arg2 = money;
        if (a != -1) {
            b2.kind = 1;
            b2.ref = a;
        } else {
            b2.kind = 4;
            b2.ref = bl->id;
        }
        b2.arg = 1;
        b2.fvalue = 0.0f;
        amt = (int)gm_GoodsPrice(f, ai_OwnPlayer);
        if (amt < money)
            goto credit;
        n = ai_ChooseSubAction(0x39, p->id, 1, &b1, &b2);
        if (n == 0)
            return 0;
        *o1 = b1;
        *o2 = b2;
        return n;
    }
    t = &ai_BTypesT[bl->type];
    if (bl->flags5a & 0x45)
        return 0;
    if (t->cap247 >= t->maxLevel)
        return 0;
    n = bl->type + 1;
    amt = (int)(gm_BuildingKindValue(n) * 0.4f);
    if (amt < money) {
        o1->kind = 4;
        o1->ref = n;
        o1->arg = 1;
        o1->ref2 = bl->id;
        o2->kind = 1;
        o2->ref = o->objId;
        o2->arg = 1;
        return 0x39;
    }
    m = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (m < 80000)
        return 0;
    if ((int)((m - 80000) * 0.5f) < amt + 0x20)
        return 0;
    o1->kind = 1;
    o1->ref = o->objId;
    o1->arg2 = 1;
    o2->arg = amt + 0x20;
    o2->kind = 0x12;
    return 0x39;
}
/* 0x4a0e60, execute callback 0x39: pays the credit (s1 kind 1, s2 kind 0x12) into the treasury s1->unk04, or
   upgrades the guild hall ("upgr_ai_zunfthaus", s1 kind 4, s2 kind 1) for 30 % of the price of type s1->unk04 */

int ai_ExecuteUpgradeGuild(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIOBJ *o;
    AIBLD *b;

    if (s1->kind == 1 && s2->kind == 0x12 && s2->arg > 0) {
        gm_FindById(0, (int *)&o, 0, s1->ref);
        if (o == 0)
            return 0;
        cm_SendMoveGoods(o->objId, p->netId, ai_OwnPlayer, s2->arg);
        return 0x39;
    }
    if (s1->kind != 4 || s2->kind != 1)
        return 0;
    gm_FindById((int *)&b, 0, 0, s1->ref2);
    if (b == 0)
        return 0;
    gm_FindById(0, (int *)&o, 0, s2->ref);
    if (o == 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.fromId = s1->ref2;
    m.to = p->netId;
    m.flags = 1;
    m.time = ai_Time;
    m.arg = s1->ref2;
    cm_BeginGroup("upgr_ai_zunfthaus");
    cm_SendMoveGoods(-1, o->objId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_ai_zunfthaus");
    return 0x39;
}
/* 0x4a1000, eval callback: not for kind-5 characters' partners of kind 5 (or busy, 0x47dd70); the oldest (0x0a) of
   the five children 0x68.. who is grown up (0x0a + 4 >= f020), has no job (jobId 0x164) and no job event 0x5e; rated
   only when that child is old enough */

int ai_EvalApprenticeship(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    int m, w;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    if (p->kind != 5) {
        c = (G5CH *)gm_FindChar(p->relatives[0]);
        if (c != 0) {
            if (c->kind == 5)
                return 0;
            if (c->sex == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(p->relatives[m + 3]);
        if (c != 0 && c->health > 0
            && (float)(c->age + 4) >= c->f020
            && c->jobId == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->age > best->age))
            best = c;
    }
    if (best == 0)
        return 0;
    *a = 0.0f;
    *b = 0.0f;
    ai_EvalMoodWeights(a, b, i, (AIPER *)p, 0, o1, o2);
    w = (float)(best->age + 4) >= best->f020 ? 1 : 0;
    *a = w * *a;
    *b = w * *b;
    return 1;
}
/* 0x4a1170, decide callback 0x3a: the oldest grown-up child without a job (as in 0x4a1000) gets a building class
   (o2 kind 0x17): a random one the dynasty has none of (0x506430 + 0x74, kind 5), the family's (0x504ad0) or
   a random 1..15; the character pays 1.5 % of 0x50a640 and needs six times that */

int ai_DecideApprenticeship(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    AIFINX *fin;
    unsigned char k;
    int m, price, q, n;             /* m: the relatives' index, then the random title group */

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (p->kind != 5) {
        c = (G5CH *)gm_FindChar(p->relatives[0]);
        if (c != 0) {
            k = c->kind;
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (c->sex == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(p->relatives[m + 3]);
        if (c != 0 && c->health > 0
            && (float)(c->age + 4) >= c->f020
            && c->jobId == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->age > best->age))
            best = c;
    }
    if (best == 0)
        return 0;
    price = (int)(gm_GetWorth(p->id) * 0.015f);
    if (gm_CharCurrency((AIPER *)p, ai_OwnPlayer) < price * 6)
        return 0;
    q = util_Random(0xf) + 1;
    if (p->kind == 5) {
        fin = (AIFINX *)gm_PlayerRecord((AICHAR *)p);
        m = util_Random(0x10);
        for (n = 0x10; n != 0; n--) {
            if (m != 0 && fin->jobByGroup[m] == 0) {
                q = m;
                break;
            }
            m = (m + 1) % 0x10;
        }
    } else if (p->jobId != 0) {
        q = gm_TitleGroup(p->jobId);
    }
    o1->kind = 7;
    o1->ref = best->netId;
    o2->kind = 0x17;
    o2->ref = gm_TitleNumber(q, 1);
    o2->arg2 = price;
    return 0x3a;
}
/* 0x4a1360, execute callback 0x3a: message 0x5e to the child s1->unk04 (kind 7): the building class s2->unk04
   (kind 0x17) and the price s2->unk10 */

int ai_ExecuteApprenticeship(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;

    if (s1->kind == 7 && s2->kind == 0x17) {
        m.to = s1->ref;
        m.flags = 0x12;
        m.arg = p->netId;
        m.type = 0x5e;
        m.fromId = -1;
        m.bldClass = (unsigned char)s2->ref;
        m.word[0] = s2->arg2;
        cm_SendLongMessage((AIMSG *)&m);
        return 0x3a;
    }
    return 0;
}
/* 0x4a13e0, decide callback 0x3b: the oldest married child (0x164, not 0x10000 in 0x1c8) gets one of three
   offers of 0x4948f0 (o2 kind 0xe), the best or the worst by the child's ratio 0x80.. to the family's 0x503ee0
   values; the character needs ten times the price
   @deep deep s43: the addressing order is a CSE effect: with rec[n].c unnamed (a CSE temp, no sk) VC6
   addresses it [edi+esi+0x80] as the original, but then (a) in one sum s[n] = b/va + c/vb VC6 puts the c
   term first (fpu 22, 8 ins, every operand order / parens the same code), (b) as s[n] = b/va; s[n] += c/vb
   the order is right but the strength-reduced rec pointer is based on .c (lea edx,[esp+0x2c], [edx-4])
   instead of .b (6 ins): the IV base seems to follow the last-referenced field. Tried this session: sk
   named in vb only / s only (mixes), b named (m or sk) 14, a G8OFFER *r loop pointer (= the index forms),
   va/vb as term temps (va = b/va; s = va + c/vb: 6), s[n] = s[n] + .. (6), the max statements interleaved
   with the terms (84), reversed index spellings *(sk + p), sk[p], raw byte offsets, (float) casts: all =
   control. climb --base v0s0f2 2 rounds: back to the control (named t). Open: a form where the b term is
   first on the FPU and the last rec reference is .b with .c as a CSE. prior: deep s23: the c index as a
   named local sk (climb local op) and the sum b/va first: 8 -> 2 ins. Left: best->skills[sk] addressed
   [esi+edi+0x80] where the original has [edi+esi+0x80] (index register first). sk as
   unsigned/long/short/char, *(skills + sk), parens, both indexes named, 30 orders of the va / sk / vb
   statements x sum forms, sk = .. inside the max argument: no lever or worse; flip.py: only sk first
   (worse elsewhere) and type changes flip it. */

int ai_DecideStudy(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    JOBTALENTS x;
    float s[3];
    G8OFFER rec[3];
    G5CH *c, *best;
    unsigned char k;
    int m, n;
    float va, vb;
    int sk;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (p->kind != 5) {
        c = (G5CH *)gm_FindChar(p->relatives[0]);
        if (c != 0) {
            k = c->kind;
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (c->sex == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(p->relatives[m + 3]);
        if (c != 0 && c->health > 0 && c->jobId != 0
            && !(c->flags1c8 & 0x10000)
            && (best == 0 || c->age > best->age))
            best = c;
    }
    if (best == 0)
        return 0;
    ai_RollStudyOffers((AIC4 *)p, (int *)rec);
    x = gm_GetJobTalents(best->jobId);
    for (n = 0; n < 3; n++) {
        va = max((float)x.skills[rec[n].b], 1.0f);
        sk = rec[n].c;
        vb = max((float)x.skills[sk], 1.0f);
        s[n] = best->skills[rec[n].b] / va + best->skills[sk] / vb;
    }
    if (util_Random(2) != 0) {
        m = s[0] > s[1] ? 0 : 1;
        if (!(s[m] > s[2]))
            m = 2;
    } else {
        m = s[0] < s[1] ? 0 : 1;
        if (!(s[m] < s[2]))
            m = 2;
    }
    if (gm_CharCurrency((AIPER *)p, ai_OwnPlayer) < rec[m].a * 10)
        return 0;
    o1->kind = 7;
    o1->ref = best->netId;
    o1->arg2 = rec[m].a;
    o2->kind = 0xe;
    o2->ref = rec[m].b;
    o2->arg2 = rec[m].c;
    return 0x3b;
}
/* 0x4a1640, execute callback of "Studium" (0x3b): sends the study message 0x5f (AIMSG.study: the offer s2,
   kind 0xe, for the child s1, kind 7, paid by p) to the character gm_FindChar(0) finds and sets flag 0x10000 of
   that character's field 0x1c8 (the decide callback skips a child that has it; the original passes 0, not the
   child) */

int ai_ExecuteStudy(G5CH *p, AIARG *s1, AIARG *s2)
{
    AIMSG m;
    AICHAR *c;

    if (s1->kind != 7 || s2->kind != 0xe)
        return 0;
    c = gm_FindChar(0);
    if (c == 0)
        return 0;
    m.flags = 0x12;
    m.type = 0x5f;
    m.to = c->netId;
    m.study.ch = p->netId;
    m.study.subject = (unsigned char)s2->ref;
    m.fromId = -1;
    m.study.level = (unsigned char)s2->arg2;
    m.study.child = s1->arg2;
    cm_SendLongMessage(&m);
    cm_SendSetField(c->netId, 0x1c8, 4, 0x10000, 0);
    return 0x3b;
}
/* 0x4a16f0, registers the actions 0x01..0x3b (eval / decide / execute callbacks of this run and of am.c) */

int ai_RegisterBaseActions(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x1;
    strcpy(act.name, "DummyUnversehrtheit");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummyHealth;
    act.exec = ai_AlwaysTrue;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2;
    strcpy(act.name, "DummyBeruf");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummyJob;
    act.exec = ai_ExecuteDummyJob;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3;
    strcpy(act.name, "DummyVergnuegen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummyPleasure;
    act.exec = ai_ExecuteDummyPleasure;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x4;
    strcpy(act.name, "Bildung");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideTrainSkill;
    act.exec = ai_ExecuteTrainSkill;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x5;
    strcpy(act.name, "Bildung (Beruf)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideTrainJobSkill;
    act.exec = ai_ExecuteTrainJobSkill;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x6;
    strcpy(act.name, "DummyRechtschaffenheit");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummyRighteousness;
    act.exec = ai_ExecuteDummyRighteousness;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x7;
    strcpy(act.name, "DummySicherheit");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummySafety;
    act.exec = ai_ExecuteDummySafety;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x8;
    strcpy(act.name, "DummyFortpflanzung");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDummyProcreation;
    act.exec = ai_ExecuteDummyProcreation;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x9;
    strcpy(act.name, "Bestechung");
    act.eval = ai_EvalLeastLikedOfEvent;
    act.decide = ai_DecideBribe;
    act.exec = ai_ExecuteBribe;
    act.flags = 0x19;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xa;
    strcpy(act.name, "Amtsbewerbung");
    act.eval = ai_EvalApplyForOffice;
    act.decide = ai_DecideApplyForOffice;
    act.exec = ai_ExecuteApplyForOffice;
    act.flags = 0x1b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xb;
    strcpy(act.name, "Objekt kaufen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideProduceGood;
    act.exec = ai_ExecuteProduceGood;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xd;
    strcpy(act.name, "Objekt kaufen (AP)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_BuyObjectApDecide;
    act.exec = ai_BuyObjectApExec;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xe;
    strcpy(act.name, "Raum kaufen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuyRoom;
    act.exec = ai_ExecuteBuyRoom;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xf;
    strcpy(act.name, "Repair Building");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideRepairBuilding;
    act.exec = ai_ExecuteRepairBuilding;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x10;
    strcpy(act.name, "Wohnsitz kaufen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuyBuilding;
    act.exec = ai_ExecuteBuyBuilding;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x11;
    strcpy(act.name, "Wohnsitz bauen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuildResidence;
    act.exec = ai_ExecuteBuildResidence;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x12;
    strcpy(act.name, "Wohnsitz upgraden (Credits)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUpgradeResidence;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x13;
    strcpy(act.name, "Betrieb bauen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuildBusiness;
    act.exec = ai_ExecuteBuildBusiness;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x14;
    strcpy(act.name, "Betrieb upgraden (AP)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_UpgradeBusinessApDecide;
    act.exec = ai_UpgradeBusinessApExec;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x15;
    strcpy(act.name, "Betrieb upgraden (Credits)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUpgradeBusiness;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x16;
    strcpy(act.name, "Medicus ausfsuchen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideVisitMedicus;
    act.exec = ai_ExecuteVisitMedicus;
    act.flags = 0x3b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x17;
    strcpy(act.name, "Sabotieren");
    act.eval = ai_EvalSabotage;
    act.decide = ai_DecideSabotage;
    act.exec = ai_ExecuteSabotage;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x18;
    strcpy(act.name, "Verpruegeln");
    act.eval = ai_EvalBeatUp;
    act.decide = ai_DecideBeatUp;
    act.exec = ai_ExecuteBeatUp;
    act.flags = 0x1c9;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x19;
    strcpy(act.name, "Buergerrecht");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideCitizenship;
    act.exec = ai_ExecuteCitizenship;
    act.flags = 0x1b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1a;
    strcpy(act.name, "ApsSparen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideSaveAps;
    act.exec = ai_ExecuteSaveAps;
    act.flags = 0x1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1b;
    strcpy(act.name, "UseObject (Military)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseMilitaryObject;
    act.exec = ai_ExecuteUseMilitaryObject;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1c;
    strcpy(act.name, "UseObject (Ansehen)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseReputationObject;
    act.exec = ai_ExecuteUseReputationObject;
    act.flags = 0x19;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1d;
    strcpy(act.name, "UseObject (APS)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseApsObject;
    act.exec = ai_ExecuteUseApsObject;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1e;
    strcpy(act.name, "UseObject (Bildung)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseEducationObject;
    act.exec = ai_ExecuteUseEducationObject;
    act.flags = 0x3b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1f;
    strcpy(act.name, "UseObject (Unversehrtheit)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseHealthObject;
    act.exec = ai_ExecuteUseHealthObject;
    act.flags = 0x3f;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x20;
    strcpy(act.name, "UseObject (Dynastie)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseDynastyObject;
    act.exec = ai_ExecuteUseDynastyObject;
    act.flags = 0x17;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x21;
    strcpy(act.name, "UseObject (Gemeinheit)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseMeanObject;
    act.exec = ai_ExecuteUseMeanObject;
    act.flags = 0xc9;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x22;
    strcpy(act.name, "UseObject (Sicherheit)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseSafetyObject;
    act.exec = ai_ExecuteUseSafetyObject;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x23;
    strcpy(act.name, "Use Parfuem");
    act.eval = ai_EvalUsePerfume;
    act.decide = ai_DecideUsePerfume;
    act.exec = ai_ExecuteUsePerfume;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x24;
    strcpy(act.name, "Use Trank");
    act.eval = ai_EvalUseTrank;
    act.decide = ai_DecideUseTrank;
    act.exec = ai_ExecuteUseTrank;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x25;
    strcpy(act.name, "Spionage");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideSpy;
    act.exec = ai_ExecuteSpy;
    act.flags = 0x69;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xc;
    strcpy(act.name, "Objekt Kaufen (Markt)");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuyMarketObject;
    act.exec = ai_ExecuteBuyMarketObject;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x26;
    strcpy(act.name, "Ablass Kaufen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuyIndulgence;
    act.exec = ai_ExecuteBuyIndulgence;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x27;
    strcpy(act.name, "UseAmt");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseOffice;
    act.exec = ai_ExecuteUseOffice;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x28;
    strcpy(act.name, "Drohen");
    act.eval = ai_EvalThreaten;
    act.decide = ai_DecideThreaten;
    act.exec = ai_ExecuteThreaten;
    act.flags = 0x5d;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x29;
    strcpy(act.name, "Pamphlete");
    act.eval = ai_EvalPamphlets;
    act.decide = ai_DecidePamphlets;
    act.exec = ai_ExecutePamphlets;
    act.flags = 0x5d;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2a;
    strcpy(act.name, "Anschwaerzen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideDenounce;
    act.exec = ai_ExecuteDenounce;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2b;
    strcpy(act.name, "Beleidigen");
    act.eval = ai_EvalInsult;
    act.decide = ai_DecideInsult;
    act.exec = ai_ExecuteInsult;
    act.flags = 0xc9;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2c;
    strcpy(act.name, "Erpressen");
    act.eval = ai_EvalBlackmail;
    act.decide = ai_DecideBlackmail;
    act.exec = ai_ExecuteBlackmail;
    act.flags = 0xc9;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2d;
    strcpy(act.name, "UsePrivs");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUsePrivileges;
    act.exec = ai_ExecuteUsePrivileges;
    act.flags = 0xff;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2e;
    strcpy(act.name, "Buergermeister");
    act.eval = ai_EvalMayor;
    act.decide = ai_DecideMayor;
    act.exec = ai_ExecuteMayor;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2f;
    strcpy(act.name, "Baumeister");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideMasterBuilder;
    act.exec = ai_ExecuteMasterBuilder;
    act.flags = 0x3b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x30;
    strcpy(act.name, "Buergermeister upgrade");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideMayorUpgrade;
    act.exec = sv_ReturnZero;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x31;
    strcpy(act.name, "Buergermeister Rathaus");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideMayorTownHall;
    act.exec = ai_ExecuteMayorTownHall;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3c;
    strcpy(act.name, "Buergermeister Kerker");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideMayorPrison;
    act.exec = ai_ExecuteMayorPrison;
    act.flags = 0x39;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x32;
    strcpy(act.name, "Dunkle Ecke");
    act.eval = ai_EvalDarkCorner;
    act.decide = ai_DecideDarkCorner;
    act.exec = ai_ExecuteDarkCorner;
    act.flags = 0x69;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x33;
    strcpy(act.name, "Kredit Nehmen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideTakeCredit;
    act.exec = ai_ExecuteTakeCredit;
    act.flags = 0x7;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x34;
    strcpy(act.name, "Change Profession");
    act.eval = ai_EvalChangeProfession;
    act.decide = ai_DecideChangeProfession;
    act.exec = ai_ExecuteChangeProfession;
    act.flags = 0x7;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x35;
    strcpy(act.name, "Stammtisch");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideStammtisch;
    act.exec = ai_ExecuteStammtisch;
    act.flags = 0x19;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x36;
    strcpy(act.name, "Zunft Preismanipulation");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecidePriceManipulation;
    act.exec = ai_ExecutePriceManipulation;
    act.flags = 0x3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x37;
    strcpy(act.name, "Zunft Aufruhr");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideGuildRiot;
    act.exec = ai_ExecuteGuildRiot;
    act.flags = 0x43;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x38;
    strcpy(act.name, "Zunft Level 3");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideGuildLevel3;
    act.exec = ai_ExecuteGuildLevel3;
    act.flags = 0x4b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x39;
    strcpy(act.name, "Upgrade Zunft");
    act.eval = ai_EvalUpgradeGuild;
    act.decide = ai_DecideUpgradeGuild;
    act.exec = ai_ExecuteUpgradeGuild;
    act.flags = 0x1b;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3a;
    strcpy(act.name, "Berufsausbildung");
    act.eval = ai_EvalApprenticeship;
    act.decide = ai_DecideApprenticeship;
    act.exec = ai_ExecuteApprenticeship;
    act.flags = 0x1f;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3b;
    strcpy(act.name, "Studium");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideStudy;
    act.exec = ai_ExecuteStudy;
    act.flags = 0x13;
    return am_RegisterMethod(&act, a) != 0;
}

/* ---- out of address order: 0x495a50 belongs between 0x4957d0 and 0x495ce0. Defined above its callers, the
   draft's register use changes 0x496e40 / 0x499660 (matched) and 0x496610 / 0x499c10; move it back once it
   matches (layout 2026-10-22). ---- */

/* 0x495a50, rates the actions that lead to action kind for character id (recursion guarded by the stack) */
int ai_ChooseSubAction(int kind, int id, int c, AIARG *o1, AIARG *o2)
{
    float best1, best2, a, b;
    int k, r, r1, r2;
    AIARG s1, s2, b1s1, b2s1, b1s2, b2s2;
    G5ACT *act;
    G5CH *p;

    best1 = -1e30f;
    best2 = -1e30f;
    p = &G5_CHARS[id];
    r1 = 0;
    r2 = 0;
    b1s1.kind = 0;
    b2s1.kind = 0;
    b1s2.kind = 0;
    b2s2.kind = 0;
    if (ai_ActionOnStack(kind) != 0)
        return 0;
    if (p->mask == 0)
        p->mask = 1;
    for (k = 0; k < 0x49; k++) {
        act = &G5_ACTS[k];
        if (act->id != 0 && act->id != p->act && ai_ActionOnStack(act->id) == 0) {
            ai_PushAction(G5_ACTS[k].id);
            s1 = *o1;
            s2 = *o2;
            if (act->eval != 0)
                act->eval(&a, &b, k, p, c, &s1, &s2);
            if ((a >= best1 || b >= best2) && act->decide != 0) {
                r = act->decide(kind, p, c, &s1, &s2);
                if (r != 0) {
                    if (a >= best1) {
                        b1s1 = s1;
                        best1 = a;
                        r1 = r;
                        b1s2 = s2;
                    }
                    if (b >= best2) {
                        best2 = b;
                        b2s1 = s1;
                        b2s2 = s2;
                        r2 = r;
                    }
                }
            }
            ai_PopAction();
        }
    }
    if (best1 != 0.0f && best1 >= best2) {
        *o1 = b1s1;
        *o2 = b1s2;
        return r1;
    }
    *o1 = b2s1;
    *o2 = b2s2;
    return r2;
}
