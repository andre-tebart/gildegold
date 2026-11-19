/* poker.c -- the card poker game of the tavern against two NPCs (0x4a7140..0x4a8760): the game record 0xf83654,
   the deck 0xf83660 and the two opponents 0xf837e0. The am unit ends before it (its last .data string is
   "%sgebaeude/*%s_%c.ogr" 0x600478); this unit's .data block is the hand names 0x600490, then its strings (the
   NPC models 0x600d90..0x600e10, 0x4a7140's); the next object (aip) starts with ai_LeisureActions. Names are ours.
   Open: G10DIST / G10DREC duplicate amt.h's AMTKIND / AMTREC (what amt_GetKind / amt_GetColumnOffices fill);
   the raised callbacks use the amt.h types, the rest still pass the G10 views.
   unit of rea 8cef7d5b (src/game/unk_4a7140.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poker.h"
#define util_RandomU ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */

#define PARTNER_FLAGS_SEX0 0xf00502  /* the partner-chain flags 0x4a8ad0 asks for a character with sex 0 (reasoned: the other sex; the low byte 2 against 1) */
#define PARTNER_FLAGS_SEX1 0xf00501  /* ditto for sex != 0 */
#define PARTNER_FLAGS_ANY 0xf00500  /* the same flags with the low byte 0, either sex (0x4a8f20; reasoned) */
#define PARTNER_FLAGS_ANY_OFFICE 0xf00d00  /* PARTNER_FLAGS_ANY | 0x800: either sex and an office holder (0x5c33c0: a candidate whose +0x166 officeKind / +0x169 secondOffice is set), 0x4a95d0 */
#define SUITOR_FLAGS_SEX0 0xc0140a  /* the candidate flags 0x4a91a0 passes ai_FindCandidates for a character with sex 0 (reasoned: the other sex, as PARTNER_FLAGS_SEX0) */
#define SUITOR_FLAGS_SEX1 0xc01409  /* ditto for sex != 0 */
#define AIPER_F1CC_DISTRUST 0x400000  /* the flags1cc bit that costs a candidate 0.2 trust in 0x4aad30 (reasoned name: its only reader) */
/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "cm_types.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char): @original the byte-matched callers here read the result as an int (no `and eax,0xff` after the call): 0x4ab7f0, and 0x4ab3e0 / 0x4abd00 compare it as an int */
#include "gv.h"
#include "he.h"
#include "tr.h"
#include "lc.h"


/* 0x4a7140, starts a game of character p in g: the stake from p's money, the deck, two random opponents */

int ai_PokerStart(AIPER *p, G10GAME *g)
{
    double d;
    int i, j, k;
    G10CARD *c;
    G10NPC *n;

    ai_PokerTable = 0;
    if (p == 0 || p->kind != 6 || g == 0)
        return 0;
    memset(ai_PokerOpponents, 0, sizeof(ai_PokerOpponents));
    ai_PokerTable = g;
    memset(g, 0, sizeof(G10GAME));
    ai_PokerTable->seat[0].who = p;
    k = gm_ConvertMoney(gm_SumListAmounts((void *)p->inventory, ai_OwnPlayer), ai_OwnPlayer);
    d = k;
    if (10000.0 > d)
        d = 10000.0;
    ai_PokerTable->money = gm_PlayerFactor((int)(d / (log10(d) + 1.0) * 0.1f), ai_OwnPlayer);
    ai_PokerTable->stake = ai_PokerTable->money / (util_RandomU(3) + 7);
    ai_PokerTable->stake2 = ai_PokerTable->stake;
    ai_PokerTable->winner = 4;
    for (k = 0; k < 6; k++) {
        for (j = 0; j < 4; j++) {
            ai_PokerDeck[k * 4 + j].suit = k;
            ai_PokerDeck[k * 4 + j].rank = j;
        }
    }
    for (i = 0; i < 2; i++) {
        n = &ai_PokerOpponents[i];
        if (i != 0)
            n->female = ai_PokerOpponents[0].female == 0;
        else
            n->female = (unsigned char)util_RandomU(2);
        n->variant = (unsigned char)util_RandomU(4) + 1;
        strncpy(n->nname, txt_FamilyNames[util_RandomU(0x95)], 15);
        if (n->female == 0) {
            strncpy(n->vname, txt_MaleFirstNames[util_RandomU(0xbf)], 15);
            switch (util_RandomU(4)) {
            case 0:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("buerger_MANN") + 0x639;
                break;
            case 1:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("Handwerker_MANN") + 0x639;
                break;
            case 2:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("buerger3_DICKER") + 0x639;
                break;
            case 3:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("schneider1_MANN") + 0x639;
                break;
            }
        } else {
            strncpy(n->vname, txt_FemaleFirstNames[util_RandomU(0x70)], 15);
            switch (util_RandomU(4)) {
            case 0:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("buergerin_frau") + 0x639;
                break;
            case 1:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("buergerin2_frau") + 0x639;
                break;
            case 2:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("minerin_frau") + 0x639;
                break;
            case 3:
                ai_PokerTable->seat[i + 1].model = gm_HeadIconNumber("arbeiterin_frau") + 0x639;
                break;
            }
        }
        n->kind = 10;
        ai_PokerTable->seat[i + 1].who = n;
    }
    return ai_PokerDealRound(g);
}
/* 0x4a7430, deals a new round: all cards back, the three seats cleared */

int ai_PokerDealRound(G10GAME *g)
{
    G10SEAT *s;
    int i;

    if (g == 0)
        return 0;
    g->left = 0x18;
    g->dealt = 0;
    g->pot = 0;
    g->phase = 0;
    g->turn = util_RandomU(3);
    g->winHand = 0;
    for (i = 0; i < 24; i++) {
        ai_PokerDeck[i].owner = 3;
        ai_PokerDeck[i].shown = 0;
    }
    s = g->seat;
    i = 3;
    do {
        memset(s->card, 0xff, sizeof(s->card));
        s->bet = 0;
        s->strategy = 0;
        s->swapped = -1;
        s->bold = util_RandomU(5) == 1;
        s->strategy = 0;
        s->foldedAt = -1;
        s++;
    } while (--i);
    return 1;
}
/* 0x4a74e0, one step of the game: returns the button mask ai_PokerGame shows (1 first button, 8 second, 0x10 fourth, 0x20 bet slider) or 0 */

unsigned short ai_PokerStep(void)
{
    switch (ai_PokerTable->phase) {
    case 0:
        ai_PokerTable->phase = 1;
        return ai_PokerTable->turn != 0 ? 0x10 : 0x21;
    case 1:
        ai_PokerAnte();
        ai_PokerTable->phase++;
        ai_PokerDealThree();
        ai_PokerPickStrategies();
        ai_PokerNpcSetStake();
        return 0;
    case 2:
        ai_PokerTable->phase = 3;
        return (ai_PokerTable->turn != 0 ? 0 : 0x20) | 9;
    case 3:
        ai_PokerPickStrategies();
        ai_PokerNpcFold();
        ai_PokerTable->phase++;
        ai_PokerDealFourth();
        ai_PokerPayStakes();
        return 0;
    case 4:
        ai_PokerTable->phase = 5;
        ai_PokerPickStrategies();
        ai_PokerNpcSetStake();
        return (ai_PokerTable->turn != 0 ? 0 : 0x20) | 9;
    case 5:
        ai_PokerNpcFold();
        ai_PokerPayStakes();
        ai_PokerTable->phase++;
        return 0x1c;
    case 6:
        ai_PokerNpcExchange();
        ai_PokerTable->phase++;
        return 0x10;
    case 7:
        ai_PokerDealFifth();
        ai_PokerTable->phase++;
        break;
    case 8:
        ai_PokerPickStrategies();
        ai_PokerTable->phase++;
        return (ai_PokerTable->turn != 0 ? 0 : 0x20) | 9;
    case 9:
        ai_PokerNpcFold();
        ai_PokerPayStakes();
        ai_PokerTable->phase++;
        return 0;
    case 10:
        ai_PokerTable->phase = 11;
        ai_PokerEndRound();
        return 0x10;
    }
    return 0;
}
/* 0x4a7650, finds a pair: the first suit (the deck's cards 4 by 4, suit 0..5) other than skip of which seat 2 holds
   two or more cards (only open ones if shown): the suit into out, the count as the result, 0 if none. The seat loop
   runs 1..2 and keeps only seat 2's counts; hand is not read */

int ai_PokerFindPair(int *hand, int shown, int skip, int *out)
{
    int cnt[6];
    int s, g, i, k, n, best;

    for (s = 1; s < 3; s++) {
        best = 0;
        g = 0;
        for (k = 0; k < 24; k += 4) {
            n = 0;
            for (i = 0; i < 4; i++) {
                if (ai_PokerDeck[k + i].owner == s && (shown == 0 || ai_PokerDeck[k + i].shown != 0))
                    n++;
            }
            if (n > best)
                best = n;
            cnt[g++] = n;
        }
    }
    for (g = 0; g < 6; g++) {
        if (g != skip && cnt[g] > 1) {
            *out = g;
            return cnt[g];
        }
    }
    return 0;
}
/* 0x4a76d0, the highest card of a hand (group, then rank): of cards 0..4, or 2..4 if high */

int ai_PokerHighCard(int *hand, int high)
{
    int i, j, best, g, r;

    i = high != 0 ? 2 : 0;
    best = hand[i];
    if (best < 0)
        return -1;
    j = i + 1;
    if (hand[j] < 0)
        return best;
    g = ai_PokerDeck[best].suit;
    r = ai_PokerDeck[best].rank;
    for (; j < 5; j++) {
        if (hand[j] < 0)
            break;
        if (ai_PokerDeck[hand[j]].suit > g) {
            best = hand[j];
            g = ai_PokerDeck[best].suit;
            r = ai_PokerDeck[best].rank;
        } else if (ai_PokerDeck[hand[j]].suit == g && ai_PokerDeck[hand[j]].rank > r) {
            best = hand[j];
            r = ai_PokerDeck[best].rank;
        }
    }
    return best;
}
/* 0x4a7760, deals a card from the deck to owner (the first one when 10 or fewer are left, else a random one) */

int ai_PokerDrawCard(int owner)
{
    int i;

    if (ai_PokerTable->left == 0)
        return -1;
    if (ai_PokerTable->left > 10) {
        do
            i = util_RandomU(24);
        while (ai_PokerDeck[i].owner != 3);
    } else {
        for (i = 0; i < 24; i++)
            if (ai_PokerDeck[i].owner == 3)
                break;
    }
    ai_PokerDeck[i].owner = owner;
    ai_PokerTable->left--;
    return i;
}
/* 0x4a77e0, the NPCs exchange a card by their strategy (seat->strategy): keep pairs (1), the most common rank (2) */

void ai_PokerNpcExchange(void)
{
    int cnt[6];
    int s, g, j, i, n, k;
    int pairs, maxn, first, second, maxr, bestr;
    int c0;
    G10SEAT *st;

    first = 0;
    second = 0;
    bestr = 0;
    k = -1;
    for (s = 1; s < 3; s++) {
        maxn = 0;
        pairs = 0;
        maxr = 0;
        g = 0;
        for (c0 = 0; c0 < 24; c0 += 4) {
            n = 0;
            for (i = 0; i < 4; i++) {
                if (ai_PokerDeck[c0 + i].owner == s)
                    n++;
            }
            if (n > 1)
                pairs++;
            if (n > maxn) {
                second = first;
                maxn = n;
                first = g;
            }
            cnt[g] = n;
            g++;
        }
        for (j = 0; j < 4; j++) {
            n = 0;
            for (i = 0; i < 24; i += 4) {
                if (ai_PokerDeck[i + j].owner == s)
                    n++;
            }
            if (n > maxr) {
                maxr = n;
                bestr = j;
            }
            cnt[j] = n;
        }
        st = &ai_PokerTable->seat[s];
        switch (st->strategy) {
        case 0:
            k = util_RandomU(3);
            break;
        case 1:
            if (pairs == 1) {
                if (ai_PokerDeck[st->card[0]].suit != first)
                    k = 0;
                else if (ai_PokerDeck[st->card[1]].suit != first)
                    k = 1;
            } else if (pairs == 2) {
                if (ai_PokerDeck[st->card[0]].suit != first && ai_PokerDeck[st->card[0]].suit != second)
                    k = 0;
                else if (ai_PokerDeck[st->card[1]].suit != first && ai_PokerDeck[st->card[1]].suit != second)
                    k = 1;
            }
            break;
        case 2:
            if (ai_PokerDeck[st->card[0]].rank != bestr)
                k = 0;
            else if (ai_PokerDeck[st->card[1]].rank != bestr)
                k = 1;
            break;
        case 3:
            if (pairs == 1) {
                if (ai_PokerDeck[st->card[0]].suit == first)
                    k = 0;
                else if (ai_PokerDeck[st->card[1]].suit != first)
                    k = 1;
            }
            break;
        }
        if (k >= 0)
            ai_PokerSwapCard(st, k);
    }
}
/* 0x4a79d0, seat s puts card k (0 or 1) away and draws a new one in its place */

void ai_PokerSwapCard(G10SEAT *s, int k)
{
    int o, n;

    if (k < 2) {
        o = ai_PokerDeck[s->card[k]].owner;
        ai_PokerDeck[s->card[k]].owner = 4;
        s->swapped = s->card[k];
        n = ai_PokerDrawCard(o);
        s->card[k] = n;
        ai_PokerDeck[n].shown = 0;
    }
}
/* 0x4a7a20, every seat still in (foldedAt < 0) pays the stake into the pot; the player's from p's money */

void ai_PokerPayStakes(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (ai_PokerTable->seat[i].foldedAt < 0) {
            ai_PokerTable->pot += ai_PokerTable->stake2;
            if (i == 0) {
                gm_PlayerFactor(ai_PokerTable->stake2, ai_OwnPlayer);
                cm_SendMoveGoodsUnchecked(-1, ((AIPER *)ai_PokerTable->seat[0].who)->netId, ai_OwnPlayer,
                             ai_PokerTable->stake2);
            }
            ai_PokerTable->seat[i].bet += ai_PokerTable->stake2;
        }
    }
}
/* 0x4a7ac0, picks each NPC's strategy (seat->strategy) from its hand: 3 nothing yet, 2 go for the rank, 1 pairs,
   0 random
   @deep orchestrator s18: dealt is back to unsigned char (the original's byte compares prove a char; a wider
   type only for the size is not kept, as for d3t_MoveObjectPath); the init order stays. size queue s18: dealt
   as unsigned short with the inits pairs / maxn / maxr / g (pk round: 4
   dealt types x 24 init orders): 79 -> 24 ins, raw 380 -> 349, padded size (352). Left: only the table /
   dealt registers swapped (ours table edx, dealt esi via movzx si; the original table esi, dealt dl, byte
   compares): the original allocates dealt before the table (prio: c2trace --prio gives the table 6, the
   uchar dealt -38: dealt pays the j loop's blocks (-42) and its compares at 474 / 477 go through the
   zero-extension CSE temp, so they do not count for it). Tried s18, no lever: dealt not a local
   (ai_PokerTable->dealt at each use) x 24 init orders: 70 ins, frame 0x2c. Next: something that ranks the
   char dealt above the table (more direct references to dealt in the chain, or fewer table references),
   then the uchar back. prior: byte-register class of dealt. Meaning re-read against the original (loops,
   deck[k+i] / deck[i+j] owner at +8, shown at +0xc, seat[s].strategy at +0x30 + s*0x34, the strategy
   chain): equal. The original keeps the unsigned char dealt in dl (cmp dl,5 / mov eax,edx; and eax,0xff)
   and ai_PokerTable in esi, s in ebp, maxr ebx, maxn edi in the first loop (stored to [esp+0x18] after
   it), pairs at [esp+0x14] (inc dword ptr), frame 0x28. Ours puts the table in edx, so dealt takes bl and
   evicts s (spilled to [esp+0x18], dealt to [esp+0x24], frame 0x30). Levers (dcp try, uchar unless said):
   init order of pairs/maxn/maxr/g, 23 orders: 79 ins each, four codegens (pairs, maxn, maxr, g gives the
   original's first-loop roles only with a wider dealt); dealt as int: 33-37 ins, unsigned short + that
   init order: 24 ins (frame 0x28 and the loops equal, dealt in esi via movzx si and the table in edx: the
   table/... */

void ai_PokerPickStrategies(void)
{
    int cnt[6];
    int s, g, j, i, k, pairs, first;
    int n, maxn, maxr;
    unsigned char dealt;

    first = 0;
    for (s = 1; s < 3; s++) {
        pairs = 0;
        maxn = 0;
        maxr = 0;
        g = 0;
        for (k = 0; k < 24; k += 4) {
            n = 0;
            for (i = 0; i < 4; i++) {
                if (ai_PokerDeck[k + i].owner == s)
                    n++;
            }
            if (n > 1)
                pairs++;
            if (n > maxn)
                maxn = n;
            cnt[g++] = n;
        }
        for (j = 0; j < 4; j++) {
            n = 0;
            for (i = 0; i < 24; i += 4) {
                if (ai_PokerDeck[i + j].owner == s)
                    n++;
            }
            if (n > maxr) {
                maxr = n;
                first = j;
            }
            cnt[j] = n;
        }
        dealt = ai_PokerTable->dealt;
        if (dealt < 5 && pairs <= 1 && maxn < 2) {
            ai_PokerTable->seat[s].strategy = 3;
        } else if (dealt == maxr) {
            ai_PokerTable->seat[s].strategy = 2;
        } else {
            if (ai_PokerTable->phase < 6 && dealt - maxr <= 1) {
                for (j = 0; j < 6; j++)
                    if (ai_PokerDeck[j * 4 + first].owner != s && ai_PokerDeck[j * 4 + first].shown == 0)
                        break;
                if (j == 6) {
                    ai_PokerTable->seat[s].strategy = 2;
                    continue;
                }
            }
            if (dealt == 5 && maxn < 2)
                ai_PokerTable->seat[s].strategy = 0;
            else
                ai_PokerTable->seat[s].strategy = 1;
        }
    }
}
/* 0x4a7c20, each NPC decides whether to fold (seat->foldedAt = the phase) against the open cards of the others */

void ai_PokerNpcFold(void)
{
    int s, j, suitN, suit, m, mySuit;
    int rankN, rank;
    G10SEAT *st, *o;

    for (s = 1; s < 3; s++) {
        st = &ai_PokerTable->seat[s];
        if (st->strategy == 0 && st->bold == 0 && ai_PokerTable->dealt >= 4)
            st->foldedAt = ai_PokerTable->phase;
        for (j = 0; j < 2; j++) {
            if (j != s && (o = &ai_PokerTable->seat[j])->foldedAt < 0) {
                ai_PokerOpenCounts(o, &suitN, &suit, &rankN, &rank);
                switch (st->strategy) {
                case 0:
                    if (st->bold != 0) {
                        if (suitN > 2)
                            st->foldedAt = ai_PokerTable->phase;
                    } else if (suitN > 1 || util_RandomU(4) == 1) {
                        st->foldedAt = ai_PokerTable->phase;
                    }
                    break;
                case 1:
                    if (st->bold != 0) {
                        int any;

                        m = ai_PokerFindPair(st->card, 0, 7, &any);
                        if (m < suitN && ai_PokerTable->phase >= 6 && ai_PokerTable->dealt >= 5)
                            st->foldedAt = ai_PokerTable->phase;
                    } else {
                        m = ai_PokerFindPair(st->card, 0, 7, &mySuit);
                        if (ai_PokerTable->phase >= 6 && m <= suitN && (m < suitN || mySuit < suit))
                            st->foldedAt = ai_PokerTable->phase;
                    }
                    break;
                case 2:
                    if (st->bold == 0) {
                        int any;

                        m = ai_PokerFindPair(st->card, 0, 7, &any);
                        if (ai_PokerTable->dealt < 5 && m == 3 && util_RandomU(4) == 1)
                            st->foldedAt = ai_PokerTable->phase;
                    }
                    break;
                case 3:
                    if (st->bold == 0) {
                        int any;

                        m = ai_PokerFindPair(st->card, 0, 7, &any);
                        if (ai_PokerTable->dealt < 5 && m == 3 && util_RandomU(5) == 1)
                            st->foldedAt = ai_PokerTable->phase;
                    }
                    break;
                }
            }
        }
    }
}
/* 0x4a7e70, counts the open cards (2 .. dealt-1) of seat s per suit and rank: the most common suit (count n, suit g)
   and rank (count m, rank r) */

void ai_PokerOpenCounts(G10SEAT *s, int *n, int *g, int *m, int *r)
{
    int rcnt[4] = {0};
    int cnt[6] = {0};
    int i, k, dealt;

    dealt = ai_PokerTable->dealt;
    for (i = 2; i < dealt; i++) {
        k = s->card[i];
        rcnt[ai_PokerDeck[k].rank]++;
        cnt[ai_PokerDeck[k].suit]++;
    }
    *n = cnt[0];
    *g = 0;
    for (i = 1; i < 6; i++) {
        if (cnt[i] > *n) {
            *n = cnt[i];
            *g = i;
        }
    }
    *m = rcnt[0];
    *r = 0;
    for (i = 1; i < 4; i++) {
        if (rcnt[i] > *m) {
            *m = rcnt[i];
            *r = i;
        }
    }
}
/* 0x4a7f50, rates the hand of owner: 0 nothing, 1 pair, 2 two pairs, 3 three, 4 straight, 5 full house, 6 flush,
   7 poker, 8 straight flush (the names at 0x600490); the groups into g1 / g2, the highest ranks into r1 / r2 */

int ai_PokerRateHand(int owner, int *g1, int *g2, int *r1, int *r2)
{
    int cnt[6];
    int rcnt[4];
    int g, r, n, i;
    int pairs, rpairs, maxn, maxr, bestr, kind, a, b;

    pairs = 0;
    rpairs = 0;
    maxn = 0;
    maxr = 0;
    bestr = 0;
    b = -1;
    kind = 0;
    a = 0;
    for (g = 0; g < 6; g++) {
        n = 0;
        for (i = 0; i < 4; i++) {
            if (ai_PokerDeck[g * 4 + i].owner == owner)
                n++;
        }
        cnt[g] = n;
        if (n > 1) {
            pairs++;
            if (n > maxn) {
                b = a;
                maxn = n;
                a = g;
            } else if (b < 0) {
                b = g;
            }
        }
    }
    for (r = 0; r < 4; r++) {
        n = 0;
        for (g = 0; g < 6; g++) {
            if (ai_PokerDeck[g * 4 + r].owner == owner)
                n++;
        }
        rcnt[r] = n;
        if (n > 1)
            rpairs++;
        if (n > maxr) {
            maxr = n;
            bestr = r;
        }
    }
    if (rpairs == 1 && maxr == 5) {
        kind = 6;
        *r1 = bestr;
    }
    if (pairs == 0) {
        if (cnt[0] == 0 || cnt[5] == 0)
            return kind == 6 ? 8 : 4;
        if (kind == 6)
            return 6;
    } else if (pairs == 1 && maxn == 4) {
        *g1 = a;
        return 7;
    } else if (pairs == 2 && maxn == 3) {
        *g1 = a;
        *g2 = b;
        return 5;
    } else if (pairs == 1 && maxn == 3) {
        *g1 = a;
        return 3;
    } else if (pairs == 2 && maxn == 2) {
        *g1 = a;
        *g2 = b;
        for (r = 3; r >= 0; r--) {
            if (ai_PokerDeck[a * 4 + r].owner == owner)
                *r1 = r;
            if (ai_PokerDeck[b * 4 + r].owner == owner)
                *r2 = r;
        }
        return 2;
    } else if (pairs == 1 && maxn == 2) {
        *g1 = a;
        for (r = 3; r >= 0; r--) {
            if (ai_PokerDeck[a * 4 + r].owner == owner) {
                *r1 = r;
                return 1;
            }
        }
        return 1;
    }
    return 0;
}
/* 0x4a8190, the showdown: the winning seat (seat 0 against 1, the better of them against 2), its hand into winHand;
   two empty hands against an empty seat 2 compare the high card of the winner with seat 1's (as the original does)
   @deep meaning re-read against 0x4a8190..0x4a84da (both switches case by case, the both-empty high-card path,
   every winHand store / return): no difference. The merged 'winHand = kind[2]; return 2' tail: VC6 cross-jumps every
   copy of that statement pair into one block (also with break-form cases, = control); writing four of the nine sites
   as 'w = 2; winHand = kind[w]; return w' (a 511-mask sweep, m14a: kind[2]>k, case 1 r1, case 2 r1, case 6/8) keeps
   separate epilogue copies as the original: 230 -> 195 ins, but those copies reload kind[2] from [esp+0x30] where
   the original stores ebp (the register-cached kind[2]); 'w = 2; winHand = kind[2]; return w' folds back (=
   control). Left: the table pointer in ebx and the seat offset in edi (the original: edi / ebx, loaded after the
   pushes, out[0] stored from the zero offset register), the store-before-pop order that follows from it, and the
   case 6/8 + 3/5/7 r1 / g1 compares the original shares (jle into one return-2 copy). Tried: headers sample
   insensitive (32, one codegen); the 720 orders of the six zero inits (two codegens, 230/232, never the register
   swap); the rating branch inverted (worse), hand store chained / through k, w = 0 hoisted, the seat field through
   (&seat[w])->: = control (tiebreak: insensitive, 32 samples one codegen) prior: Read the whole original
   0x4a8190..0x4a84da against the C: rating loop (int seat counter reused as the winner, as the original's esi), the
   seat-0/1 chain, both 8-case switches (tables 0x4a84dc / 0x4a84fc case by case), the seat-2 comparison and every
   winHand store / return path correspond; compare operand order fixed. Left: the original keeps ai_PokerTable in edi
   and the seat offset in ebx (ours swapped: regs), and it gives each 'winHand = kind[2]; return 2' its own epilogue
   copy (5 copies at 0x4a83ae, 0x4a83d3, 0x4a840b, 0x4a843f, 0x4a8460) where VC6 merges ours into one block, plus the
   first switch's shared tails (case 1/8 and 2/6 r1 compares, case 4 / both-empty high-card call) merged in the other
   direction. Tried: w = 2 / break with a common tail (252 ins, worse), the high-card if-assign (kept). (size: 276 vs
   296 instructions, explained: 276 vs 296: the original duplicates the 8-instruction 'store kind[2]; mov eax,2;
   epilogue' block four more times than ours (32 instructions), ours keeps two extra r1-compare tails in the first
   switch) */

int ai_PokerShowdown(void)
{
    int g1[3], r1[3], kind[3], out[3], g2[3];
    int r2[3];
    int w, k;    /* w: the seat, first the loop over the seats, then the winner */

    kind[0] = 0;
    kind[2] = 0;
    kind[1] = 0;
    out[0] = 0;
    out[2] = 0;
    out[1] = 0;
    for (w = 0; w < 3; w++) {
        if (ai_PokerTable->seat[w].foldedAt < 0) {
            kind[w] = ai_PokerRateHand(w, &g1[w], &g2[w], &r1[w], &r2[w]);
            ai_PokerTable->seat[w].hand = kind[w];
        } else {
            out[w] = 1;
        }
    }
    if (out[0] != 0) {
        w = 1;
    } else if (out[1] != 0) {
        w = 0;
    } else if (kind[0] == 0) {
        if (kind[1] == 0)
            w = ai_PokerHighCard(ai_PokerTable->seat[0].card, 0) <= ai_PokerHighCard(ai_PokerTable->seat[1].card, 0);
        else
            w = 1;
    } else if (kind[1] == 0) {
        w = 0;
    } else if (kind[1] < kind[0]) {
        w = 0;
    } else if (kind[1] > kind[0]) {
        w = 1;
    } else {
        switch (kind[0]) {
        case 1:
            if (g1[1] < g1[0])
                w = 0;
            else if (g1[1] > g1[0])
                w = 1;
            else
                w = r1[0] <= r1[1];
            break;
        case 2:
            if (g1[1] < g1[0])
                w = 0;
            else if (g1[1] > g1[0])
                w = 1;
            else if (g2[1] < g2[0])
                w = 0;
            else if (g2[1] > g2[0])
                w = 1;
            else
                w = r1[0] <= r1[1];
            break;
        case 3:
        case 7:
            w = g1[0] <= g1[1];
            break;
        case 4:
            w = ai_PokerHighCard(ai_PokerTable->seat[0].card, 0) <= ai_PokerHighCard(ai_PokerTable->seat[1].card, 0);
            break;
        case 5:
            w = g1[0] <= g1[1];
            break;
        case 6:
            w = r1[0] <= r1[1];
            break;
        case 8:
            w = r1[0] <= r1[1];
            break;
        }
    }
    if (out[2] != 0) {
        ai_PokerTable->winHand = kind[w];
        return w;
    }
    if (kind[w] == 0 && kind[2] == 0) {
        if (ai_PokerHighCard(ai_PokerTable->seat[w].card, 0) <= ai_PokerHighCard(ai_PokerTable->seat[1].card, 0))
            w = 1;
        return w;
    }
    k = kind[w];
    if (k == 0) {
        ai_PokerTable->winHand = kind[2];
        return 2;
    }
    if (kind[2] != 0 && kind[2] >= k) {
        if (kind[2] > k) {
            w = 2;
            ai_PokerTable->winHand = kind[w];
            return w;
        }
        switch (k) {
        case 1:
            if (g1[2] >= g1[w]) {
                if (g1[2] > g1[w]) {
                    ai_PokerTable->winHand = kind[2];
                    return 2;
                }
                if (r1[w] <= r1[2]) {
                    w = 2;
                    ai_PokerTable->winHand = kind[w];
                    return w;
                }
            }
            break;
        case 2:
            if (g1[2] >= g1[w]) {
                if (g1[2] > g1[w]) {
                    ai_PokerTable->winHand = kind[2];
                    return 2;
                }
                if (g2[2] >= g2[w]) {
                    if (g2[2] > g2[w]) {
                        ai_PokerTable->winHand = kind[2];
                        return 2;
                    }
                    if (r1[w] <= r1[2]) {
                        w = 2;
                        ai_PokerTable->winHand = kind[w];
                        return w;
                    }
                }
            }
            break;
        case 3:
        case 5:
        case 7:
            if (g1[w] <= g1[2]) {
                ai_PokerTable->winHand = kind[2];
                return 2;
            }
            break;
        case 4:
            if (ai_PokerHighCard(ai_PokerTable->seat[w].card, 0) <= ai_PokerHighCard(ai_PokerTable->seat[2].card, 0))
                w = 2;
            ai_PokerTable->winHand = kind[w];
            return w;
        case 6:
        case 8:
            if (r1[w] <= r1[2]) {
                w = 2;
                ai_PokerTable->winHand = kind[w];
                return w;
            }
            break;
        default:
            return w;
        }
    }
    ai_PokerTable->winHand = k;
    return w;
}
/* 0x4a8520, deals three cards to each seat (the first two hidden, the third open) */

void ai_PokerDealThree(void)
{
    int k, i;

    for (k = 0; k < 3; k++) {
        for (i = 0; i < 3; i++) {
            ai_PokerTable->seat[i].card[k] = ai_PokerDrawCard(i);
            if (k > 1)
                ai_PokerDeck[ai_PokerTable->seat[i].card[k]].shown = 1;
            else
                ai_PokerDeck[ai_PokerTable->seat[i].card[k]].shown = 0;
        }
    }
    ai_PokerTable->dealt += 3;
}
/* 0x4a8590, deals the fourth card to each seat, open */

void ai_PokerDealFourth(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_PokerTable->seat[i].card[3] = ai_PokerDrawCard(i);
        ai_PokerDeck[ai_PokerTable->seat[i].card[3]].shown = 1;
    }
    ai_PokerTable->dealt++;
}
/* 0x4a85e0, deals the fifth card to each seat, open */

void ai_PokerDealFifth(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_PokerTable->seat[i].card[4] = ai_PokerDrawCard(i);
        ai_PokerDeck[ai_PokerTable->seat[i].card[4]].shown = 1;
    }
    ai_PokerTable->dealt++;
}
/* 0x4a8630, the player exchanges card k */

void ai_PokerExchange(int k)
{
    ai_PokerSwapCard(&ai_PokerTable->seat[0], k);
}
/* 0x4a8650, the ante: three stakes into the pot, the player's from p's money */

void ai_PokerAnte(void)
{
    ai_PokerTable->pot = ai_PokerTable->stake2 * 3;
    cm_SendMoveGoodsUnchecked(-1, ((AIPER *)ai_PokerTable->seat[0].who)->netId, ai_OwnPlayer, ai_PokerTable->stake2);
}
/* 0x4a8690, ends the round: the winner into winner, the pot to the player if he won, all cards back */

void ai_PokerEndRound(void)
{
    int i;

    ai_PokerTable->winner = (short)ai_PokerShowdown();
    if (ai_PokerTable->winner == 0)
        cm_SendMoveGoodsUnchecked(((AIPER *)ai_PokerTable->seat[0].who)->netId, -1, ai_OwnPlayer,
                     ai_PokerTable->pot);
    for (i = 0; i < 24; i++) {
        ai_PokerDeck[i].owner = 3;
        ai_PokerDeck[i].shown = 0;
    }
}
/* 0x4a86f0, the NPC whose turn it is (turn) sets the stake: up to half the margin above the stake, or all of it
   if it bluffs */

void ai_PokerNpcSetStake(void)
{
    if (ai_PokerTable->turn != 0) {
        if (ai_PokerTable->seat[ai_PokerTable->turn].bold != 0)
            ai_PokerTable->stake2 = util_RandomU(ai_PokerTable->money - ai_PokerTable->stake) + ai_PokerTable->stake;
        else
            ai_PokerTable->stake2 = util_RandomU(ai_PokerTable->money - ai_PokerTable->stake >> 1) + ai_PokerTable->stake;
    }
}

/* the poker unit's one .rdata constant: it precedes the unit's literals (77.0f at 0x5ed1c0, ...), after am's */
const int ai_CardBack = 0x18;  /* 0x5ed1bc, the picture offset of the card back (d2_Pics[0x775 + ai_CardBack], ai_PokerSeatPanel) */
char ai_PokerHandNames[9][0x100] = { "H\xf6" "chste Karte", "Paar", "2 Paar", "Dreier", "Strasse", "Fullhouse", "Flush", "Poker", "Royal Flush" };  /* 0x600490, the hand names by ai_PokerRateHand's rank 0 .. 8; no reader in the image: the first datum of the unit's .data block, after am's strings */
G10GAME * ai_PokerTable;  /* 0xf83654 */
G10CARD ai_PokerDeck[24];  /* 0xf83660 */
G10NPC ai_PokerOpponents[2];  /* 0xf837e0 */
