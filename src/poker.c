/* poker.c -- the card poker game of the tavern against two NPCs (0x4a7140..): the game record 0xf83654,
   the deck 0xf83660 and the two opponents 0xf837e0. The am unit ends before it (its last .data string is
   "%sgebaeude/*%s_%c.ogr" 0x600478; this unit's block starts with the hand names 0x600490). Names are ours.
   Then the leisure actions (0x4a8760..0x4aa430), the "aip_Exec*" private AI actions and actions 0x44..0x47
   (registration 0x4abed0, the last function); one .data block 0x600490..0x600f60 in function order.
   Open: possible second unit from 0x4aa580 (3-parameter callbacks, "aip_Exec*"); codegen is neutral, only the
   single .data block keeps it one file.
   Open: the leisure callbacks take the character as AIPER (am.h) or AICHAR (ai.h, preferred: it names the fields);
   the two views wait for the am.h / ai.h unification (am.c's Open line).
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
        g = 0;
        maxn = 0;
        pairs = 0;
        maxr = 0;
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
   @deep Read whole against the original and rewritten: both counting loops in the original's index form
   (deck[k+i] by suits, deck[i+j] by ranks: the strength-reduced jl loop ends now equal), int counters
   (signed jle), cnt[g++], the strategy chain with pairs <= 1 / dealt - maxr <= 1 as the original's
   constants, the column check over deck[j*4+first]. The two counting loops are instruction-equal up to
   registers. Remaining is allocation: the original keeps s in ebp and dealt in dl for the whole body
   (frame 0x28), ours gives s ebx and spills it to [esp+0x18] and dealt to [esp+0x24] (frame 0x30), so ours
   reloads them (asmdiff: -mov ebp,[esp+0x18] / -cmp edi,ebp vs +cmp [eax-4],ebp; -mov byte ptr
   [esp+0x24],bl vs +mov dl,[esi+0xb5]; -mov esi,[esp+0x18] / -inc esi / -mov [esp+0x18],esi vs +inc ebp),
   and pairs / maxn swap register and slot in the first loop (-inc edi / -cmp ecx,[esp+0x14] vs +inc
   [esp+0x14] / +cmp ecx,edi). */

void ai_PokerPickStrategies(void)
{
    int cnt[6];
    int s, g, j, i, k, pairs, first;
    int n, maxn, maxr;
    unsigned char dealt;

    first = 0;
    for (s = 1; s < 3; s++) {
        maxr = 0;
        maxn = 0;
        pairs = 0;
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
    int s, j, suitN, suit, m, mySuit, any;
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
                        m = ai_PokerFindPair(st->card, 0, 7, &any);
                        if (ai_PokerTable->dealt < 5 && m == 3 && util_RandomU(4) == 1)
                            st->foldedAt = ai_PokerTable->phase;
                    }
                    break;
                case 3:
                    if (st->bold == 0) {
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
   and rank (count m, rank r)
   @stuck constant-0 web, not text order. The original keeps 0 in no callee-saved register: cnt[1..4]
   stored from eax (the xor eax,eax that also zero-extends dealt), cnt[5..9] from ecx, cnt[0] and *g / *r
   as immediates (mov dword ptr [esi], 0), frame 0x28 + push ebx/esi; ours makes one zero web over the
   init, *g = 0 and *r = 0 and gives it edi for the whole function (push edi / xor edi,edi / mov
   [esp+..],edi / mov [esi],edi). Probes: with *g and *r stored as 7 the init block takes a scratch zero
   (eax, no edi) and drops to 52 ins, so the *g / *r zeros join the web; with only *r = 7 still edi. Tried
   this round (dcp try, all draft 66 ins unless said): *g/*r = 0L, (long)0, '\0', (char)0, (short)0, 0u (=
   control: typed zeros fold into one web); chained zero groups (1..4, 5..9, c0 separate, reversed, all
   ten, pairs; new code, 66 ins each); k = 0 copied into the stores (= control); dealt load first / after
   the init / read in the for test (= control); *g = 0 before *n = cnt[4] (66, worse hunks); two arrays
   rank[4] / suit[6] with explicit stores (= control). Not yet: a candidate that outranks the zero for
   edi/ebp cannot help (the original leaves edi and ebp free), so the zero must leave the global allocator:
   compare d3sndw_Update (0 in ebx up to the main loop, immediates after, unsolved). Hunks: init block (35
   ins), every frame offset +4 from the extra push, mov [esi],edi vs mov dword ptr [esi],0 twice.
   (tiebreak: insensitive) prior: Read whole against the original: zeroes the 10 counters (ranks 0..3,
   suits 4..9) in the original's store order, dealt as an int (zero-extended byte, signed cmp eax,2 / jle),
   counts cards 2..dealt-1 (the loop is instruction-equal: lea esi,[eax-2], deck[k].rank / deck[k].suit+4),
   then the two max scans with the original's compare direction (cmp ecx,ebx / jle, now equal). Remaining
   is the zero constant's allocation only: ours keeps one 0 in edi for the whole function (push edi / xor
   edi,edi, the 10 stores and both *g = 0 / *r = 0 from edi, pop edi), the original zeroes with eax (xor
   eax,eax that also zero-extends dealt) and ecx before the pushes, stores cnt[0] and *g / *r as immediates
   (asmdiff: -xor edi,edi / -mov [esp+..],edi vs +xor eax,eax / +xor ecx,ecx / +mov [esp+..],eax|ecx / +mov
   dword ptr [esp+8],0; -mov [esi],edi vs +mov dword ptr [esi],0), which shifts every frame offset by the
   extra push. Tried: chained zero assignments, int cnt[10] = {0} (rep stosd), two arrays rank[4] / suit[6]
   (zeroing loops: rep stosd), for vs do/while card loop. */

void ai_PokerOpenCounts(G10SEAT *s, int *n, int *g, int *m, int *r)
{
    int cnt[10];                        /* cnt[0..3] per rank, cnt[4..9] per suit */
    int i, k, dealt;

    cnt[1] = 0;
    cnt[5] = 0;
    cnt[2] = 0;
    cnt[6] = 0;
    cnt[3] = 0;
    cnt[4] = 0;
    dealt = ai_PokerTable->dealt;
    cnt[7] = 0;
    cnt[8] = 0;
    cnt[0] = 0;
    cnt[9] = 0;
    for (i = 2; i < dealt; i++) {
        k = s->card[i];
        cnt[ai_PokerDeck[k].rank]++;
        cnt[ai_PokerDeck[k].suit + 4]++;
    }
    *n = cnt[4];
    *g = 0;
    for (i = 1; i < 6; i++) {
        if (cnt[i + 4] > *n) {
            *n = cnt[i + 4];
            *g = i;
        }
    }
    *m = cnt[0];
    *r = 0;
    for (i = 1; i < 4; i++) {
        if (cnt[i] > *m) {
            *m = cnt[i];
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
    kind = 0;
    b = -1;
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
                a = g;
                maxn = n;
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
   @deep Read the whole original 0x4a8190..0x4a84da against the C: rating loop (int seat counter reused as
   the winner, as the original's esi), the seat-0/1 chain, both 8-case switches (tables 0x4a84dc / 0x4a84fc
   case by case), the seat-2 comparison and every winHand store / return path correspond; compare operand
   order fixed. Left: the original keeps ai_PokerTable in edi and the seat offset in ebx (ours swapped:
   regs), and it gives each 'winHand = kind[2]; return 2' its own epilogue copy (5 copies at 0x4a83ae,
   0x4a83d3, 0x4a840b, 0x4a843f, 0x4a8460) where VC6 merges ours into one block, plus the first switch's
   shared tails (case 1/8 and 2/6 r1 compares, case 4 / both-empty high-card call) merged in the other
   direction. Tried: w = 2 / break with a common tail (252 ins, worse), the high-card if-assign (kept).
   (size: 276 vs 296 instructions, explained: 276 vs 296: the original duplicates the 8-instruction 'store
   kind[2]; mov eax,2; epilogue' block four more times than ours (32 instructions), ours keeps two extra
   r1-compare tails in the first switch) */

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
            ai_PokerTable->winHand = kind[2];
            return 2;
        }
        switch (k) {
        case 1:
            if (g1[2] >= g1[w]) {
                if (g1[2] > g1[w]) {
                    ai_PokerTable->winHand = kind[2];
                    return 2;
                }
                if (r1[w] <= r1[2]) {
                    ai_PokerTable->winHand = kind[2];
                    return 2;
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
                        ai_PokerTable->winHand = kind[2];
                        return 2;
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
                ai_PokerTable->winHand = kind[2];
                return 2;
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
    unsigned short n;

    if (ai_PokerTable->turn != 0) {
        if (ai_PokerTable->seat[ai_PokerTable->turn].bold != 0)
            n = ai_PokerTable->money - ai_PokerTable->stake;
        else
            n = ai_PokerTable->money - ai_PokerTable->stake >> 1;
        ai_PokerTable->stake2 = util_RandomU(n) + ai_PokerTable->stake;
    }
}
/* 0x4a8760, decide callback: character p (or the one of the ctx record, kind 2) picks a partner in its district
   (a character with flag 1 within 33 units, else one of a higher rank within 40..71), into m (kind 7) */

int ai_DecideOfficePartner(int a, AICHAR *p, int *ctx, int d, AIARG *m)
{
    G10DIST info, info2;
    G10DREC rec[6];
    AICHAR *who[6];
    AICHAR *o, *c;
    int k, id, n, cnt, i, j, r;

    o = 0;
    if (ctx[0] == 1) {
        if (p->appliedOffice != 0)
            return 0;
        id = p->netId;
        k = p->officeKind;
    } else {
        if (ctx[0] != 2)
            return 0;
        c = (AICHAR *)ctx[0x87];
        if (c == 0)
            return 0;
        k = c->officeKind;
        id = c->netId;
    }
    if (amt_GetKind(k, &info) == 0)
        return 0;
    n = amt_GetColumnOffices(info.id, 6, rec);
    i = 0;
    cnt = 0;
    if (n > 0) {
        for (; i < n; i++) {
            if (rec[i].k == 2) {
                cnt++;
                if (rec[i].id == id)
                    return 0;
            }
        }
        if (cnt > 2)
            return 0;
    }
    for (i = 0; i < n; i++)
        who[i] = (AICHAR *)gm_FindChar(rec[i].id);
    for (i = n; i < 6; i++)
        who[i] = 0;
    r = util_RandomU(6);
    for (j = 6; j-- != 0;) {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < 33.0f && rec[r].k == 1) {
            o = who[r];
            if (o != 0 && p->relatives[0] != o->netId) {
                m->kind = 7;
                m->ref = o->netId;
                return 1;
            }
            break;
        }
        r = (r + 1) % 6;
    }
    if (info.lvl == 3 || info.lvl == 6 || info.lvl == 9 || p->sel != 7)
        return 0;
    r = util_RandomU(6);
    for (j = 6; j-- != 0;) {
        c = who[r];
        if (c != 0 && id != c->netId) {
            if (amt_GetKind(c->officeKind, &info2) == 0)
                return 0;
            if (info2.lvl > info.lvl && rec[r].k == 1
                && gm_Liking(c->id, p->id, 1) < (float)util_RandomU(0x20) + 40.0f) {
                o = who[r];
                break;
            }
        }
        r = (r + 1) % 6;
    }
    if (o != 0 && p->relatives[0] != o->netId) {
        m->kind = 7;
        m->ref = o->netId;
        return 1;
    }
    return 0;
}
/* 0x4a8a50, decide callback: the decision record ctx[1] proposes a value (its fn) within min..max, into m
   (kind 0x11) */

int ai_DecideTableValue(int a, AIPER *p, int *ctx, int d, AIARG *m)
{
    AIDEC dec;
    int v;

    if (gv_GetTableRecord(ctx[1], &dec) != 0) {
        if (((int (*)(AIPER *, int, int *))dec.fn)(p, (unsigned char)dec.id, &v) != 0 && v != dec.cur
            && v >= dec.min && v <= dec.max) {
            m->kind = 0x11;
            m->ref = (unsigned char)dec.id;
            m->arg = v;
            return 1;
        }
    }
    return 0;
}
/* 0x4a8ad0, decide callback: p (money >= 3) picks a character of the other sex (ai_PickPartnerChain), into m
   (kind 7) with a random count 1..5 */

int ai_DecidePartnerOtherSex(int a, AICHAR *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if (p->money < 3)
        return 0;
    flags = p->sex != 0 ? PARTNER_FLAGS_SEX1 : PARTNER_FLAGS_SEX0;
    if (ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    m->kind = 7;
    m->ref = ai_CharsT[id].netId;
    /* a count 1..5: a roll of 0..money/2 - 1 when it is above 1, else 1 (the macro rolls twice, as the original) */
    m->arg = min(util_RandomU(p->money / 2) > 1 ? util_RandomU(p->money / 2) : 1, 5);
    return 1;
}
/* 0x4a8bf0, eval callback: one in four */

int ai_EvalOneInFour(void)
{
    return util_RandomU(4) == 0;
}
/* 0x4a8c10, eval callback: always */

int ai_AlwaysTrue(void)
{
    return 1;
}
/* 0x4a8c20, eval callback: true one time in four when it is p's slot (netId & 3 = year % 4 and netId & 7 =
   hour % 8), so each character gets its turn spread over the years and hours */

int ai_EvalOwnTimeSlot(int a, AIPER *p)
{
    int id;

    id = p->netId;
    if ((id & 3) == ai_Time.year % 4 && (id & 7) == ai_Time.hour % 8)
        return util_RandomU(4) == 0;
    return 0;
}
/* 0x4a8c80, decide callback: two characters near p (0x4a58d0), into m (kind 7) */

int ai_DecideTwoNearChars(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags[2];
    unsigned short id[2];

    flags[0] = PARTNER_FLAGS_ANY;
    flags[1] = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 2, flags, 0, (float)util_RandomU(0x40), id) == 0)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id[0]].netId;
    m->arg = ((AIPER *)ai_CharsT)[id[1]].netId;
    return 1;
}
/* 0x4a8d20, decide callback: p (money >= 3) picks a character within 32..49 units, into m (kind 7) */

int ai_DecideNearChar(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if (p->money < 3)
        return 0;
    flags = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 1, &flags, 0, (float)util_RandomU(0x12) + 32.0f, &id) == 0)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a8db0, decide callback: a building for action 4 of ai_Partner (or action 6 of p, rated 32..63), then from a
   random start the first living character of kind 0 whose building (bld) it is, into m (kind 7, netId, building) */

int ai_DecideBuildingResident(int a, AIPER *p, int c, int d, AIARG *m)
{
    int k, i, n;
    AIBLD *b;
    AICHAR *o;
    union { __int64 all; struct { unsigned int lo; int hi; } h; } mask;   /* the class mask, set by halves: VC6 keeps
                                                                          them in esi / edi for both calls */

    mask.h.lo = 0x807d4190;
    mask.h.hi = 1;
    if (ai_Partner != 0)
        ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.all, 0, 100.0f, &k);
    if (k == -1) {
        if (ai_PickActionBuilding(p, 6, mask.all, 0, (float)util_RandomU(0x20) + 32.0f, &k) == 0)
            return 0;
    }
    b = gm_BuildQuery(1, 1, k);
    if (b == 0)
        return 0;
    i = util_RandomU(0x300);
    n = 0x300;
    o = 0;
    while (n != 0) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].kind == 0) {
            o = &ai_CharsT[i];
            break;
        }
        n--;
        i = (i + 1) % 0x300;
    }
    if (o == 0)
        return 0;
    m->kind = 7;
    m->ref = o->netId;
    m->arg = b->id;
    return 1;
}
/* 0x4a8ee0, eval callback: two in three if p's jobId is of kind 7, else one in four */

int ai_EvalTitleGroup7(int a, AIPER *p)
{
    if (gm_TitleGroup(p->jobId) == 7 && util_RandomU(3) != 0)
        return 1;
    return util_RandomU(4) == 0;
}
/* 0x4a8f20, decide callback: a chain of two characters p rates in 33..77+roll (ai_PickPartnerChain, either sex)
   that do not both like each other above 60, into m (kind 7, both netIds) */

int ai_DecideUnfriendlyPair(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags[2];
    unsigned short id[2];
    float l1, l2;

    flags[0] = PARTNER_FLAGS_ANY;
    flags[1] = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 2, flags, 33.0f, (float)util_RandomU(0x18) + 77.0f, id) == 0)
        return 0;
    l1 = gm_Liking(id[0], id[1], 1);
    l2 = gm_Liking(id[1], id[0], 1);
    if (l1 > 60.0f && l2 > 60.0f)
        return 0;
    m->kind = 7;
    m->ref = ai_CharsT[id[0]].netId;
    m->arg = ai_CharsT[id[1]].netId;
    return 1;
}
/* 0x4a9030, eval callback: a character p with money >= 2 that owns graves in use (gv_CountOwnedGraves):
   their count, at most money / 2, into m->unk10 */

int ai_EvalGraveCount(int a, AIPER *p, int c, AIARG *m)
{
    int n, k;

    if (p->money < 2)
        return 0;
    n = gv_CountOwnedGraves(p->netId);
    if (n <= 0)
        return 0;
    k = p->money / 2;
    if (k >= n)
        k = n;
    m->arg2 = k;
    return 1;
}
/* 0x4a9080, decide callback: one of the owners p counts most often (gv_RankCulprits, at least 2..5 times),
   walked from the first, that is a living character (not of kind 15) and holds none of the office kinds
   13 / 27 / 15 / 21, into m (kind 7, its netId); none when p itself holds office kind 13 */

int ai_DecideAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m)
{
    int cnt[32];
    int ids[32];
    int n, i, j, lim;
    AICHAR *o;
    unsigned char k;

    lim = util_RandomU(4) + 2;
    if (p->officeKind != 13) {
        n = gv_RankCulprits(ids, cnt, p->id);
        if (n != 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= lim) {
                    o = (AICHAR *)gm_FindChar(ids[i]);
                    if (o != 0 && o->kind != 15 && o->health != 0) {
                        k = o->officeKind;
                        if (k != 13 && k != 27 && k != 15 && k != 21) {
                            m->kind = 7;
                            m->ref = o->netId;
                            return 1;
                        }
                    }
                }
                j--;
                i = (i + 1) % n;
            } while (j != 0);
        }
    }
    return 0;
}
/* 0x4a9150, eval callback: p with money >= 8 in its year of four (netId % 4 = the year % 4), one in four */

int ai_EvalOwnYearTurn(int a, AICHAR *p)
{
    if (p->money >= 8 && p->netId % 4 == ai_Time.year % 4 && util_RandomU(4) == 0)
        return 1;
    return 0;
}
/* 0x4a91a0, decide callback: in its year of four (netId % 4 = the year % 4), an unmarried p (relatives[0] no
   character) that is in no event 0x41 (he_FindEvent, either side +0xac / +0xb0) picks one of up to 12
   characters of the other sex 30..100 units away (ai_FindCandidates), into m (kind 7) */

int ai_DecideSuitor(int a, AICHAR *p, int c, int d, AIARG *m)
{
    unsigned short ids[12];
    AIJOB *r;
    unsigned int n;

    if (ai_Time.year % 4 == p->netId % 4 && gm_FindChar(p->relatives[0]) == 0) {
        for (r = (AIJOB *)he_FindEvent(1, 0, 0x41); r != 0; r = (AIJOB *)he_NextEvent()) {
            if (r->partner2 == p->netId)
                return 0;
            if (r->partner1 == p->netId)
                return 0;
        }
        n = ai_FindCandidates((AIPER *)p, 12, p->sex != 0 ? SUITOR_FLAGS_SEX1 : SUITOR_FLAGS_SEX0, 30.0f, 100.0f, ids);
        if (n != 0) {
            m->kind = 7;
            m->ref = ai_CharsT[ids[util_RandomU(n)]].netId;
            return 1;
        }
    }
    return 0;
}
/* 0x4a9280, decide callback: as ai_DecideAcquaintance with an owner counted more often (at least 5..12 times) */

int ai_DecideCloseAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m)
{
    int cnt[32];
    int ids[32];
    int n, i, j, lim;
    AICHAR *o;
    unsigned char k;

    lim = util_RandomU(8) + 5;
    if (p->officeKind != 13) {
        n = gv_RankCulprits(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= lim) {
                    o = (AICHAR *)gm_FindChar(ids[i]);
                    if (o != 0 && o->kind != 15 && o->health != 0) {
                        k = o->officeKind;
                        if (k != 13 && k != 27 && k != 15 && k != 21) {
                            m->kind = 7;
                            m->ref = o->netId;
                            return 1;
                        }
                    }
                }
                j--;
                i = (i + 1) % n;
            } while (j != 0);
        }
    }
    return 0;
}
/* 0x4a9350, decide callback: p (money >= 2, not flagged 0x100) picks an owner it counts at least 4 times
   (gv_RankCulprits) not flagged 0x200 whose office kind has a level of 4 or more, into m (kind 7) */

int ai_DecideOfficeAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m)
{
    AMTKIND kind;
    int cnt[32];
    int ids[32];
    int n, i, j;
    AICHAR *o;

    if (p->money >= 2 && (p->flags1c8 & 0x100) == 0) {
        n = gv_RankCulprits(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= 4) {
                    o = (AICHAR *)gm_FindChar(ids[i]);
                    if (o != 0 && (o->flags1c8 & 0x200) == 0) {
                        if (amt_GetKind(o->officeKind, &kind) != 0 && kind.level >= 4) {
                            m->kind = 7;
                            m->ref = o->netId;
                            return 1;
                        }
                    }
                }
                j--;
                i = (i + 1) % n;
            } while (j != 0);
        }
    }
    return 0;
}
/* 0x4a9420, decide callback: p (money >= 2) picks a character within 40..71 units that 0x553d60 accepts, into
   m (kind 7) */

int ai_DecideAcceptedChar(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if (p->money < 2)
        return 0;
    flags = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 1, &flags, 0, (float)util_RandomU(0x20) + 40.0f, &id) == 0)
        return 0;
    if (he_NoEvent4bPair(p, &((AIPER *)ai_CharsT)[id]) == 0)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a94e0, decide callback: p (money > 2, not flagged 0x400) picks a character within 18..49 units, not flagged
   0x400, by chance after 0x504e20, into m (kind 7) */

int ai_DecideUnflaggedChar(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if (p->money >= 3 && (p->flags1c8 & 0x400) == 0) {
        flags = PARTNER_FLAGS_ANY;
        if (ai_PickPartnerChain(p, 1, &flags, 0, (float)util_RandomU(0x20) + 18.0f, &id) != 0
            && (((AIPER *)ai_CharsT)[id].flags1c8 & 0x400) == 0
            && util_RandomF() + gm_Mix(p, &((AIPER *)ai_CharsT)[id], 3) >= 1.0f) {
            m->kind = 7;
            m->ref = ((AIPER *)ai_CharsT)[id].netId;
            return 1;
        }
    }
    return 0;
}
/* 0x4a95d0, decide callback: p (not flagged 0x80000) picks a character 20..63 units away who holds an office,
   into m (kind 7) */

int ai_DecideFarChar(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x80000) != 0)
        return 0;
    flags = PARTNER_FLAGS_ANY_OFFICE;
    if (ai_PickPartnerChain(p, 1, &flags, 20.0f, (float)util_RandomU(0x20) + 32.0f, &id) == 0)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a9660, decide callback: p (not flagged 0x40000, money >= 8) picks a character within 18..33 units who
   holds an office, into m (kind 7) */

int ai_DecideCloseChar(int a, AIPER *p, int c, int d, AIARG *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x40000) != 0)
        return 0;
    if (p->money < 8)
        return 0;
    flags = PARTNER_FLAGS_ANY_OFFICE;
    if (ai_PickPartnerChain(p, 1, &flags, 0, (float)util_RandomU(0x10) + 18.0f, &id) == 0)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a9700, decide callback: p (not flagged 0x100, money >= 4) picks one of the holders of the offices of
   column 4 (amt_GetColumnOffices, up to 6), starting at a random one, that is not the character of ctx (p itself
   for ctx kind 1, the one at ctx + 0x21c for kind 2) and that p likes less than 16..31, nor p's spouse
   (relatives[0]), into m (kind 7); 0x20 when p cannot */

int ai_DecideColumn4Officer(int a, AICHAR *p, int *ctx, int d, AIARG *m)
{
    AMTREC rec[6];
    AICHAR *who[6];
    AICHAR *o;
    int id, n, i;
    unsigned int r;

    if ((p->flags1c8 & 0x100) != 0)
        return 0x20;
    if (p->money < 4)
        return 0x20;
    if (ctx[0] == 1) {
        id = p->netId;
    } else {
        if (ctx[0] != 2)
            return 0;
        if (ctx[0x87] == 0)
            return 0;
        id = ((AICHAR *)ctx[0x87])->netId;
    }
    n = amt_GetColumnOffices(4, 6, rec);
    for (i = 0; i < n; i++)
        who[i] = (AICHAR *)gm_FindChar(rec[i].owner);
    for (i = n; i < 6; i++)
        who[i] = 0;
    r = util_RandomU(6);
    i = 6;
    do {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < (float)util_RandomU(0x10) + 16.0f) {
            o = who[r];
            if (o == 0)
                return 0;
            if (p->relatives[0] == o->netId)
                return 0;
            m->kind = 7;
            m->ref = o->netId;
            return 1;
        }
        i--;
        r = (int)(r + 1) % 6;
    } while (i != 0);
    return 0;
}
/* 0x4a9880, decide callback: as ai_DecideColumn4Officer for the holders of the offices of column 5 */

int ai_DecideColumn5Officer(int a, AICHAR *p, int *ctx, int d, AIARG *m)
{
    AMTREC rec[6];
    AICHAR *who[6];
    AICHAR *o;
    int id, n, i;
    unsigned int r;

    if ((p->flags1c8 & 0x100) != 0)
        return 0x20;
    if (p->money < 4)
        return 0x20;
    if (ctx[0] == 1) {
        id = p->netId;
    } else {
        if (ctx[0] != 2)
            return 0;
        if (ctx[0x87] == 0)
            return 0;
        id = ((AICHAR *)ctx[0x87])->netId;
    }
    n = amt_GetColumnOffices(5, 6, rec);
    for (i = 0; i < n; i++)
        who[i] = (AICHAR *)gm_FindChar(rec[i].owner);
    for (i = n; i < 6; i++)
        who[i] = 0;
    r = util_RandomU(6);
    i = 6;
    do {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < (float)util_RandomU(0x10) + 16.0f) {
            o = who[r];
            if (o == 0)
                return 0;
            if (p->relatives[0] == o->netId)
                return 0;
            m->kind = 7;
            m->ref = o->netId;
            return 1;
        }
        i--;
        r = (int)(r + 1) % 6;
    } while (i != 0);
    return 0;
}

/* the callbacks above as 0x4a9a00 calls them */
#define G10_CB(fn) ((int (*)(int, AIPER *, int *, AIARG *, AIARG *))(fn))


/* 0x4a9a00, decide callback of the leisure actions: picks one of the actions of the table 0x600e10 that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback
   @deep read whole against the original and reconciled: branch order (in->kind==0 first), the two index
   loops (jl), the side choice (int side gives the and eax,0xffff), direct returns per case. Left is
   allocation: n1 lives in [esp+0x10] and n2 in ebx where the original has n1 in ebx, n2 in [esp+0x10]
   (asmdiff: '- mov dword ptr [esp + eax*4 + 0x234], edx / inc eax / mov [esp+0x10],eax' vs '+ mov [esp +
   ebx*4 + 0x234], edx / inc ebx'; '- xor ebx, ebx' vs '+ mov dword ptr [esp + 0x10], 0'); our side is
   stored into the dead slot (- mov dword ptr [esp + 0x10], eax) where the original tests eax; the switch
   index is lea eax,[edi-0x12] where the original does add edi,-0x12 with xor eax,eax hoisted for the
   default return. Tried: init order, int casts on the random, a ternary side, r variable in the switch
   (worse) */
int ai_DecideLeisureAction(int a, AIPER *p, int c, AIARG *in, AIARG *m)
{
    int ctx[0x88];
    int l1[19], l2[19];
    int n1, n2, i, k, kind, side;
    AIJOB *j;

    memset(ctx, 0, sizeof(ctx));
    n1 = 0;
    n2 = 0;
    ctx[0x87] = 0;
    if ((p->flags1c8 & 0x100) != 0) {
        for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
            if (j->partner1 == p->netId) {
                ctx[0x87] = (int)gm_FindChar(j->partner2);
                break;
            }
        }
    }
    if (in->kind == 0) {
        ctx[0] = 1;
        for (i = 0; i < 19; i++) {
            if ((ord_DispatchOrder(ai_LeisureActions[i], p, ctx) & 2) != 0)
                l1[n1++] = ai_LeisureActions[i];
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (i = 0; i < 19; i++) {
                if ((ord_DispatchOrder(ai_LeisureActions[i], p, ctx) & 2) != 0)
                    l2[n2++] = ai_LeisureActions[i];
            }
        }
        if (n1 == 0) {
            if (n2 == 0)
                return 0;
            side = 1;
        } else if (n2 == 0) {
            side = 0;
        } else {
            side = util_RandomU(2);
        }
        if (side == 0) {
            ctx[0] = 1;
            k = l1[util_RandomU(n1)];
            kind = 3;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        } else {
            if (n2 == 0)
                return 0;
            ctx[0] = 2;
            k = l2[util_RandomU(n2)];
            kind = 4;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        }
        in->kind = 0x10;
        in->ref = k;
        in->arg = kind;
    } else {
        if (in->kind != 0x10)
            return 0;
        k = in->ref;
        ctx[0] = 1;
        if ((ord_DispatchOrder(k, p, ctx) & 2) != 0) {
            in->arg = 3;
        } else {
            ctx[0] = 2;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
            in->arg = 4;
        }
    }
    switch (k) {
    case 0x12:
        return G10_CB(ai_DecideOfficePartner)(a, p, ctx, in, m);
    case 0x13:
    case 0x14:
        return G10_CB(ai_DecideTableValue)(a, p, ctx, in, m);
    case 0x1c:
        return G10_CB(ai_DecidePartnerOtherSex)(a, p, ctx, in, m);
    case 0x1d:
        return G10_CB(ai_EvalOneInFour)(a, p, ctx, in, m);
    case 0x21:
        return G10_CB(ai_AlwaysTrue)(a, p, ctx, in, m);
    case 0x24:
        return G10_CB(ai_EvalOwnTimeSlot)(a, p, ctx, in, m);
    case 0x25:
        return G10_CB(ai_DecideTwoNearChars)(a, p, ctx, in, m);
    case 0x27:
        return G10_CB(ai_DecideNearChar)(a, p, ctx, in, m);
    case 0x28:
        return G10_CB(ai_DecideBuildingResident)(a, p, ctx, in, m);
    case 0x29:
        return G10_CB(ai_EvalTitleGroup7)(a, p, ctx, in, m);
    case 0x2a:
        return G10_CB(ai_DecideUnfriendlyPair)(a, p, ctx, in, m);
    case 0x2b:
        return G10_CB(ai_EvalGraveCount)(a, p, ctx, in, m);
    case 0x2c:
        return G10_CB(ai_DecideAcquaintance)(a, p, ctx, in, m);
    case 0x2d:
        return G10_CB(ai_EvalOwnYearTurn)(a, p, ctx, in, m);
    case 0x31:
        return G10_CB(ai_DecideFarChar)(a, p, ctx, in, m);
    case 0x32:
        return G10_CB(ai_DecideCloseChar)(a, p, ctx, in, m);
    case 0x35:
        return G10_CB(ai_DecideColumn4Officer)(a, p, ctx, in, m);
    case 0x37:
        return G10_CB(ai_DecideColumn5Officer)(a, p, ctx, in, m);
    }
    return 0;
}
/* 0x4a9fa0, check callback of the leisure actions: whether p may still do the action in (kind 0x10) with the
   targets of out (0x4f81c0 without flags 0x60) */

int ai_CheckLeisureAction(AIPER *p, AIARG *in, AIARG *out)
{
    int ctx[0x88];
    AIJOB *j;

    if (in->kind == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && in->arg == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->partner1 == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->partner2);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = in->arg;
        switch (in->ref) {
        case 0x13:
        case 0x14:
            ctx[1] = out->ref;
            ctx[2] = out->arg;
            break;
        case 0x1c:
            ctx[0x85] = out->ref;
            ctx[1] = out->arg;
            break;
        case 0x27:
            ctx[0x85] = out->ref;
            break;
        case 0x25:
        case 0x28:
        case 0x2a:
            ctx[0x85] = out->ref;
            ctx[0x86] = out->arg;
            break;
        case 0x2b:
            ctx[1] = in->arg2;
            break;
        case 0x1d:
        case 0x21:
        case 0x24:
        case 0x29:
        case 0x2d:
            break;
        case 0x12:
        case 0x2c:
        case 0x31:
        case 0x32:
        case 0x35:
        case 0x37:
            ctx[0x85] = out->ref;
            break;
        default:
            return 0;
        }
        return (ord_DispatchOrder(in->ref, p, ctx) & 0x60) == 0;
    }
    return 0;
}
/* 0x4aa120, decide callback of the visit actions: as 0x4a9a00 with the actions of the table 0x600e5c that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback
   @deep read whole against the original, rewritten as ai_DecideLeisureAction (in->kind==0 first, index
   loops with jl, int side for the coin, direct returns per case); instruction count 247 vs 249. Left is
   allocation: the original keeps in in ebx (+ mov ebx, dword ptr [esp + 0x25c]) and n1 in ebp, n2 in
   [esp+0x10]; ours n1 in ebx, n2 in ebp and in reloaded (- mov eax, dword ptr [esp + 0x25c] / mov byte ptr
   [eax], 0x10); our side is stored in the slot (- mov dword ptr [esp + 0x10], eax); the default return
   value is eax set before the switch jump (+ xor eax, eax / + mov eax, dword ptr [esp + 0x10]) where ours
   has a separate return 0 (table entries +0x4/+0x8/+0x14). Tried: r variable over the switch (213 ins,
   worse) */

int ai_DecideVisitAction(int a, AIPER *p, int c, AIARG *in, AIARG *m)
{
    int ctx[0x88];
    int l1[3], l2[3];
    int n1, n2, i, k, kind, side;
    AIJOB *j;

    memset(ctx, 0, sizeof(ctx));
    n1 = 0;
    n2 = 0;
    ctx[0x87] = 0;
    if ((p->flags1c8 & 0x100) != 0) {
        for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
            if (j->partner1 == p->netId) {
                ctx[0x87] = (int)gm_FindChar(j->partner2);
                break;
            }
        }
    }
    if (in->kind == 0) {
        ctx[0] = 1;
        for (i = 0; i < 3; i++) {
            if ((ord_DispatchOrder(ai_VisitActions[i], p, ctx) & 2) != 0)
                l1[n1++] = ai_VisitActions[i];
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (i = 0; i < 3; i++) {
                if ((ord_DispatchOrder(ai_VisitActions[i], p, ctx) & 2) != 0)
                    l2[n2++] = ai_VisitActions[i];
            }
        }
        if (n1 == 0) {
            if (n2 == 0)
                return 0;
            side = 1;
        } else if (n2 == 0) {
            side = 0;
        } else {
            side = util_RandomU(2);
        }
        if (side == 0) {
            ctx[0] = 1;
            k = l1[util_RandomU(n1)];
            kind = 3;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        } else {
            ctx[0] = 2;
            k = l2[util_RandomU(n2)];
            kind = 4;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        }
        in->kind = 0x10;
        in->ref = k;
        in->arg = kind;
    } else {
        if (in->kind != 0x10)
            return 0;
        k = in->ref;
        ctx[0] = 1;
        if ((ord_DispatchOrder(k, p, ctx) & 2) != 0) {
            in->arg = 3;
        } else {
            ctx[0] = 2;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
            in->arg = 4;
        }
    }
    switch (k) {
    case 0:
        return G10_CB(ai_DecideSuitor)(a, p, ctx, in, m);
    case 3:
        return G10_CB(ai_DecideCloseAcquaintance)(a, p, ctx, in, m);
    case 4:
        return G10_CB(ai_DecideOfficeAcquaintance)(a, p, ctx, in, m);
    case 7:
        return G10_CB(ai_DecideAcceptedChar)(a, p, ctx, in, m);
    case 8:
        return G10_CB(ai_DecideUnflaggedChar)(a, p, ctx, in, m);
    }
    return 0;
}
/* 0x4aa430, check callback of the visit actions (0x4aa120): as 0x4a9fa0; prints the action name into a debug text
   that nobody reads */

int ai_CheckVisitAction(AICHAR *p, AIARG *in, AIARG *out)
{
    char name[64];
    int ctx[0x88];
    char text[256];
    AIJOB *j;

    if (in->kind == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && in->arg == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->partner1 == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->partner2);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = in->arg;
        switch (in->ref) {
        case 0:
            sprintf(name, "prva_WERBUNG");
            ctx[0x85] = out->ref;
            break;
        case 3:
            sprintf(name, "prva_ANKLAGE_ERHEBEN");
            ctx[0x85] = out->ref;
            break;
        case 4:
            sprintf(name, "prva_ERPRESSUNG");
            ctx[0x85] = out->ref;
            break;
        case 7:
            sprintf(name, "prva_FURCHT_EINFLOESSEN");
            ctx[0x85] = out->ref;
            break;
        case 8:
            sprintf(name, "prva_BELEIDIGUNG");
            ctx[0x85] = out->ref;
            break;
        default:
            return 0;
        }
        sprintf(text, "NPC %s <%i> w\xe4hlt %s", p->name, p->id, name);
        return (ord_DispatchOrder(in->ref, (char *)p, ctx) & 0x60) == 0;
    }
    return 0;
}
/* 0x4aa580, decide callback: p (money >= 1) picks a character within 0..63 units that he_NoEvent35Pair /
   he_NoEvent88Pair accept, with fewer than 4 pamphlets spread (by chance), into m (kind 7) */

int ai_PickPamphletTarget(AIPER *p, int c, AIARG *m)
{
    int k;
    int flags;
    unsigned short id;

    k = 0;
    if (p->money < 1)
        return 0;
    flags = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    if (he_NoEvent35Pair(p, &((AIPER *)ai_CharsT)[id], &k) == 0)
        return 0;
    if (he_NoEvent88Pair(&ai_CharsT[id], (AICHAR *)p) == 0)
        return 0;
    if (k > 3)
        return 0;
    if (util_RandomF() < k * 0.25f)
        return 0;
    m->kind = 7;
    m->ref = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4aa6a0, decide callback: p (money > 1) picks a random acquaintance within 20..80 units that 0x553d10
   accepts and it knows little (fewer than 2..5), into m (kind 7, its id at 0x10); in gets a count and a kind */

int ai_DecideLittleKnownChar(AICHAR *p, AIARG *in, AIARG *m)
{
    int cnt[32];
    int ids[32];
    int n, j;
    unsigned int i;
    AICHAR *o;

    if (p->money >= 2 && (n = gv_RankCulprits(ids, cnt, p->id)) != 0) {
        i = util_RandomU(n);
        j = n;
        do {
            o = (AICHAR *)gm_FindChar(ids[i]);
            if (o == 0)
                return 0;
            if (he_NoEvent2ePair((int)p, (int)o) == 0)
                return 0;
            if (gm_Liking(p->id, o->id, 1) < (double)util_RandomU(0x3d) + 20.0
                && cnt[i] < util_RandomU(4) + 2) {
                m->kind = 7;
                m->ref = o->netId;
                m->arg2 = o->id;
                in->arg = util_RandomU(cnt[i]) + 1;
                in->arg2 = util_RandomU(3);
                return 1;
            }
            j--;
            i = (int)(i + 1) % n;
        } while (j != 0);
    }
    return 0;
}
/* 0x4aa800, decide callback: p (money >= 3) picks a character within 33..48 units and a second one 50..100 units
   from both, by chance after 0x504e20, into m (kind 7) */

int ai_DecideCharPair(AIPER *p, int c, AIARG *m)
{
    unsigned short ids[4];
    unsigned short id, id2;
    float f;

    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    if (p->money < 3)
        return 0;
    ids[0] = p->id;
    if (ai_PickPartnerInRange(p, PARTNER_FLAGS_ANY, 4, ids, 0.0f, util_RandomU(0x10) + 33.0, &id) == 0)
        return 0;
    ids[1] = id;
    if (ai_PickPartnerInRange(p, PARTNER_FLAGS_ANY, 4, ids, 50.0f, 100.0f, &id2) == 0)
        return 0;
    m->kind = 7;
    m->ref = ai_CharsT[id].netId;
    f = gm_Mix((int)p, (int)&ai_CharsT[id], 4);
    if (!(util_RandomF() + f >= 1.0f))
        return 0;
    m->arg = ai_CharsT[id2].netId;
    return 1;
}
/* 0x4aa940, execute callback "aip_ExecPamphlet": p spreads a pamphlet about the character of m (message 0x35) */

int aip_ExecPamphlet(AIPER *p, int c, AIARG *m)
{
    AIMSG msg;
    AICHAR *o;

    if (m->kind != 7)
        return 0;
    o = (AICHAR *)gm_FindChar(m->ref);
    if (o == 0)
        return 0;
    gm_BuildQuery(1, 5, 10);
    msg.type = 0x35;
    msg.to = p->netId;
    msg.fromId = msg.to;
    msg.flags = 2;
    msg.pamphlet.when = ai_Time;
    msg.pamphlet.variant = (unsigned char)util_RandomU(0x1d);
    msg.pamphlet.about = o->netId;
    cm_BeginGroup("aip_ExecPamphlet");
    cm_SendLongMessage(&msg);
    cm_EndGroup("aip_ExecPamphlet");
    return 1;
}
/* 0x4aaa10, execute callback "aip_ExecDrohen": p threatens the character of m (message 0x2e, by chance after
   0x504e20) */

int aip_ExecDrohen(AIPER *p, int c, AIARG *m)
{
    AIMSG msg;
    AIPER *o;

    if (m->kind != 7)
        return 0;
    o = gm_FindPer(m->ref);
    if (o == 0)
        return 0;
    cm_BeginGroup("aip_ExecDrohen");
    if (util_RandomF() + gm_Mix(p, o, 4) >= 1.0f) {
        msg.type = 0x2e;
        msg.to = o->netId;
        msg.fromId = -1;
        msg.flags = 2;
        msg.arg[1] = p->netId;
        msg.arg[0] = o->netId;
        cm_SendLongMessage(&msg);
    }
    cm_SendMoneyDelta(p->netId, -2);
    cm_EndGroup("aip_ExecDrohen");
    return 1;
}
/* 0x4aaae0, execute callback "aip_ExecAnschwaerzen": p denounces the character of m to the one at m + 8
   (message 0x2f) */

int aip_ExecAnschwaerzen(AIPER *p, int c, AIARG *m)
{
    AIMSG msg;

    msg.to = m->arg;
    msg.arg[1] = p->netId;
    msg.arg[0] = m->ref;
    msg.arg[2] = m->arg;
    msg.type = 0x2f;
    msg.fromId = -1;
    msg.flags = 1;
    msg.time = ai_Time;
    cm_BeginGroup("aip_ExecAnschwaerzen");
    cm_SendLongMessage(&msg);
    cm_SendMoneyDelta(p->netId, -3);
    cm_EndGroup("aip_ExecAnschwaerzen");
    return 1;
}
/* 0x4aab90, which of b and c character p trusts more (liking 0.66, skill 4 0.34): 1 b, 0 c, 2 neither (within
   5); *k 1 when the likings of b and c (each +0.2 with item 0x2be, after the ratings) differ by more than half
   as much as their skills, else 2 */

int ai_TrustedOfTwo(AIPER *p, AIPER *b, AIPER *c, int *k)
{
    float db, wb, dc, wc, x1, x2;

    *k = 0;
    if (p == c)
        return 0;
    if (p == b)
        return 1;
    db = gm_Liking(b->id, p->id, 1);
    wb = gm_GetSkill(b, 4);
    dc = gm_Liking(c->id, p->id, 1);
    wc = gm_GetSkill(c, 4);
    x1 = db * 0.66 + wb * 0.34;
    x2 = dc * 0.66 + wc * 0.34;
    if (ai_InventoryHas(b, 0x2be) != 0)
        db += 0.2f;
    if (ai_InventoryHas(c, 0x2be) != 0)
        dc += 0.2f;
    if (abs((int)(db - dc)) * 0.66 > abs((int)(wb - wc)) * 0.33)
        *k = 1;
    else
        *k = 2;
    if (x2 < x1 - 5.0f)
        return 1;
    if (x1 < x2 - 5.0f)
        return 0;
    return 2;
}
/* 0x4aad30, of the four candidates ids the one character p trusts most for district k (liking + the office rating,
   item 0x2be +0.2, flag AIPER_F1CC_DISTRUST -0.2); a tie is broken at random; -1 if none
   @deep Read whole 0x4aad30..0x4aaeee against the C: the scoring loop (gm_FindChar, amt_GetOffice rate,
   gm_Liking, item 0x2be +0.2, flags1cc 0x400000 -0.2, -1000), the best/tie loop (c[i] tested in both
   branches as the original's second test edx), bi<0 / tie checks, the signed (r+1)%4 tie-break loop
   (do-while with the counter decremented before the step: dec edx / test edx) all correspond; every
   instruction between the blocks is equal. Left: the original gives each return -1 its own 'pop edi; or
   eax,-1; pop esi; add esp,0x38; ret' copy (0x4aad55, 0x4aae6f, 0x4aaed8 plus the loop end) where VC6
   cross-jumps ours into one block (- je X / jl X to the shared tail, + jne/jge over an inline epilogue),
   the same epilogue-duplication quirk as ai_PokerShowdown 0x4a8190. (size: 147 vs 162 instructions,
   explained: 146 vs 162: the original's three extra inline 'pop edi; or eax,-1; pop esi; add esp,0x38;
   ret' return -1 copies (15 instructions) and the test edx,edx the dec placement needs) */

int ai_TrustedOfFour(unsigned char k, AIPER *p, int *ids, int *out)
{
    float sc[4];
    AIPER *c[4];
    AMTREC info;
    float f, best;
    int tie, bi, i, r;

    *out = 0;
    if (amt_GetOffice(k, &info) == 0)
        return -1;
    for (i = 0; i < 4; i++) {
        c[i] = gm_FindPer(ids[i]);
        if (c[i] == 0 || amt_GetOfficeRate(c[i], info.kind, &f) == 0) {
            sc[i] = -1000.0f;
        } else {
            sc[i] = gm_Liking(c[i]->id, p->id, 1) + f;
            if (ai_InventoryHas(c[i], 0x2be) != 0)
                sc[i] += 0.2f;
            if ((c[i]->flags1cc & AIPER_F1CC_DISTRUST) != 0)
                sc[i] -= 0.2f;
        }
    }
    best = -10.0f;
    tie = 0;
    bi = -1;
    for (i = 0; i < 4; i++) {
        if (c[i] != 0 && sc[i] > best) {
            best = sc[i];
            tie = 0;
            bi = i;
        } else if (c[i] != 0 && best == sc[i]) {
            tie = 1;
        }
    }
    if (bi < 0)
        return -1;
    if (!tie)
        return c[bi]->netId;
    r = util_RandomU(4);
    i = 4;
    do {
        if (sc[r] == best && c[r] != 0) {
            if (r >= 0)
                return c[r]->netId;
            return -1;
        }
        i--;
        r = (r + 1) % 4;
    } while (i != 0);
    return -1;
}
/* 0x4aaef0, decide callback: a building of p (type with cap > 1) with a free workplace object (lists 0x120 / 300)
   and no job 0x74 yet, into a (kind 4) and b (kind 1) */

int ai_FindDarkCornerTarget(AIPER *p, AIARG *a, AIARG *b)
{
    AIBLD *bld;
    AIOBJ *o, *o2;

    for (bld = gm_BuildQuery(2, 4, p->id, 5, 0x16); bld != 0; bld = gm_BuildNext()) {
        if (ai_BTypesT[bld->type].cap247 > 1 && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
            && (o2 = gm_Query(o->list, 1, 0, 300)) != 0 && he_FindEvent(2, 0, 0x74, 3, bld->id) == 0) {
            a->kind = 4;
            a->ref = bld->id;
            b->kind = 1;
            b->ref = o->objId;
            return 1;
        }
    }
    return 0;
}
/* 0x4aafb0, execute callback of 0x49ee20 (a of kind 4, b of kind 1): message 0x74 to p from the character
   a->ref (arg[1]) about b->ref (arg[2]); p pays 1 (gm_SendMoneyDelta -1) */

int ai_ExecMessage74(AIPER *p, AIARG *a, AIARG *b)
{
    AIMSG msg;

    AI_ZERO(&msg, sizeof(msg));
    msg.type = 0x74;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = a->ref;
    msg.time = ai_Time;
    msg.arg[1] = a->ref;
    msg.arg[2] = b->ref;
    cm_SendLongMessage(&msg);
    gm_SendMoneyDelta(p->netId, -1);
    return 1;
}
/* 0x4ab080, decide callback of 'buy dunkle ecke' (0x4ab2c0): from hour 21, when hour % 3 == netId % 3, p (not
   flagged 0x2000000) takes a random other owner's shop (type cap > 1, objects 0x120 / 300) and tries its three
   records from a random one: the first whose value (gv_EvidencePrice) is at most 5 percent of p's goods value
   goes into a (kind 0x13: record, price) and b (kind 4: the building); none when random(12) exceeds the office
   level of the record's character; half of the time it only looks (a and b cleared, still 1) */

int ai_DecideBuyDunkleEcke(AIPER *p, AIARG *a, AIARG *b)
{
    struct {
        AIBLD *bld;
        AIOBJ *obj;
    } shops[8], *s;
    AMTKIND kind;
    int money, n, tries;
    int price;
    unsigned int x;
    short who;
    unsigned int k;
    AIBLD *bld;
    AIOBJ *o;

    if ((p->flags1cc & 0x2000000) == 0 && ai_Time.hour >= 21
        && ai_Time.hour % 3 == (unsigned int)p->netId % 3) {
        money = gm_GetGoodsValue((AICHAR *)p);
        memset(shops, 0, sizeof(shops));
        n = 0;
        bld = gm_BuildQuery(1, 5, 0x16);
        if (bld != 0) {
            do {
                if (bld->owner != 0xffff && bld->owner != p->id && ai_BTypesT[bld->type].cap247 > 1
                    && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
                    && (o = gm_Query(o->list, 1, 0, 300)) != 0) {
                    shops[n].bld = bld;
                    shops[n].obj = o;
                    n++;
                }
                bld = gm_BuildNext();
            } while (bld != 0);
            if (n != 0) {
                s = &shops[util_RandomU(n)];
                k = util_RandomU(3);
                bld = s->bld;
                o = s->obj;
                tries = 3;
                do {
                    if (gv_EvidencePrice(o->recs[k], &ai_CharsT[bld->owner], &price, &who, &x) != 0
                        && !(money * 0.05f < price)) {
                        amt_GetKind(ai_CharsT[(unsigned short)who].officeKind, &kind);
                        if (util_RandomU(12) > kind.level)
                            return 0;
                        if (util_RandomF() < 0.5f) {
                            memset(a, 0, sizeof(*a));
                            memset(b, 0, sizeof(*b));
                            return 1;
                        }
                        a->kind = 0x13;
                        a->ref = o->recs[k];
                        a->arg2 = price;
                        b->kind = 4;
                        b->ref = bld->id;
                        return 1;
                    }
                    tries--;
                    k = (int)(k + 1) % 3;
                } while (tries != 0);
            }
        }
    }
    return 0;
}
/* 0x4ab2c0, execute callback "buy dunkle ecke": p buys the record a->ref for a->arg2 in the shop b->ref if its
   kind 300 object still holds it: the script message 'buy ' (shop, p, record, price), the proof executed, the
   price to the shop's owner */

int ai_ExecBuyDunkleEcke(AIPER *p, AIARG *a, AIARG *b)
{
    int rec[5];
    AIBLD *bld;
    AIOBJ *o;
    int *q;
    int i;

    bld = gm_BuildQuery(1, 1, b->ref);
    if (bld == 0)
        return 0;
    o = gm_Query(bld->objList, 1, 0, 0x120);
    if (o == 0)
        return 0;
    o = gm_Query(o->list, 1, 0, 300);
    if (o == 0)
        return 0;
    i = 0;
    q = o->recs;
    do {
        if (*q == a->ref)
            break;
        i++;
        q++;
    } while (i < 3);
    if (i == 3)
        return 0;
    rec[0] = 0x62757920;    /* 'buy ' */
    rec[2] = bld->id;
    rec[1] = p->netId;
    rec[3] = a->ref;
    rec[4] = a->arg2;
    cm_BeginGroup("buy dunkle ecke");
    cm_SendScriptMessage(rec);
    cm_SendProofExecuted(a->ref, p->netId);
    cm_SendMoveGoodsUnchecked(((AIPER *)ai_CharsT)[bld->owner].netId, p->netId, 0, a->arg2);
    cm_EndGroup("buy dunkle ecke");
    return 1;
}
/* 0x4ab3e0, decide callback of action 0x44 "Blutrache": p (money >= 5, not flagged 8, not of kind 3) with its
   best building of kind 0x1f holding object 0x27c picks a victim within 25..57 units and up to four helpers
   (ai_PickPartnerChainLevel) by their weight (office level / 3 + 2): a helper is taken when the running sum
   with its weight reaches p's money (s + w >= money, as the original compares); the sum into m->arg2, the
   victim and helpers into m / m2 (kind 7)
   @deep Read whole 0x4ab3e0..0x4ab6b0 against the C: the early checks (flag 8, money >= 5, a 0x44/0, c,
   gm_TitleJob 0xe int result, kind 3), the 0x100-building walk with gm_BathObjectCount, flags = 0 at the
   top (its address goes to ai_PickPartnerChainF, the original zeroes it in p's arg slot),
   PARTNER_FLAGS_ANY, the 25+random(32) radius, o's office level /3+2 (read through o),
   ai_PickPartnerChainLevel, the helper loop and the AIARG stores all correspond; 222 vs 222 instructions,
   frame 0x1c equal. Left: register choice in the helper loop and the tail - the original forms s + w with
   lea ecx, [eax + esi] and loads p->money into eax after it (ours: mov ecx, [ebp+0x194] first, add eax,
   esi in place), the who[] address in eax vs edx, and the tail loads o into ecx / m into eax where ours
   swaps them; plus the order hunk xor ebx / xor edi. Tried: t = s + (w), w then s + w / s += w, p->money
   <= s + t, for (i = 0, cnt = 0; ...) - no change. */

int ai_DecideBlutrache(int a, AIPER *p, int c, AIARG *m, AIARG *m2)
{
    unsigned short ids[4];
    AIPER *who[4];
    int v, i, n, cnt, s, t;
    int bestv, k;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_TitleJobI(p->id, 0xe);
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    best = 0;
    bestv = 0;
    flags = 0;
    who[0] = 0;
    who[1] = 0;
    who[2] = 0;
    who[3] = 0;
    if ((p->flags1cc & 8) != 0 || p->money < 5)
        return 0;
    if ((a != 0x44 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0x100; i != 0; i--) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x27c) != 0) {
            if (best == 0) {
                best = b;
                bestv = gm_BathObjectCount(b);
            } else {
                k = gm_BathObjectCount(b);
                if (k > bestv) {
                    best = b;
                    bestv = k;
                }
            }
        }
        b++;
    }
    if (best == 0 || bestv == 0)
        return 0;
    flags = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
        return 0;
    o = &((AIPER *)ai_CharsT)[id];
    s = amt_GetKindLevel(o->officeKind) / 3 + 2;
    if (s + 2 > p->money)
        return 0;
    n = ai_PickPartnerChainLevel(p, 4, flags, 25.0f, 75.0f, bestv, ids);
    if (n == 0)
        return 0;
    cnt = 0;
    for (i = 0; i < n; i++) {
t = amt_GetKindLevel(ai_CharsT[ids[i]].officeKind) / 3 + 2;
        if (s + t >= p->money) {
            s += t;
            who[cnt++] = &((AIPER *)ai_CharsT)[ids[i]];
        }
    }
    if (cnt == 0)
        return 0;
    m->ref = o->netId;
    m->ref2 = who[0]->netId;
    if (cnt > 1)
        m->arg = who[1]->netId;
    if (cnt > 2)
        m2->ref2 = who[2]->netId;
    if (cnt > 3)
        m2->arg = who[3]->netId;
    m->arg2 = s;
    m->kind = 7;
    return 0x44;
}
/* 0x4ab6b0, execute callback of action 0x44 "Blutrache": p pays the cost, is flagged (0x1cc bit 3), the victim
   loses standing with every helper by their relation (0x504e20), and 0x5a9470 starts the feud */

int ai_ExecBlutrache(AIPER *p, AIARG *m, AIARG *m2)
{
    AIPER *who[4];
    AIPER *o;
    int cost, n, i, d;

    cost = m->arg2;
    if (m2 == 0 || p == 0)
        return 0;
    o = gm_FindPer(m->ref);
    if (o == 0)
        return 0;
    who[0] = gm_FindPer(m->ref2);
    if (who[0] == 0)
        return 0;
    n = 1;
    if ((who[n] = gm_FindPer(m->arg)) == 0)
        n++;
    if ((who[n] = gm_FindPer(m2->ref2)) == 0)
        n++;
    if ((who[n] = gm_FindPer(m2->arg)) == 0)
        n++;
    gm_SendMoneyDelta(p->netId, -cost);
    cm_SendSetField(p->netId, 0x1cc, 4, 8, 0);
    for (i = 0; i < n; i++) {
        d = 50 - (int)(gm_Mix(p, who[i], 4) * -50.0f);
        cm_SendRelations(o->id, who[i]->id, -d, 1);
    }
    ai_GypsyDeliveryText(o, n, who);
    return 0x44;
}
/* 0x4ab7f0, decide callback of action 0x45 "Verfluchen": p (not flagged 0x10, not of kind 3) with its best
   building of kind 0x1f holding object 0x28a curses a character within 25..57 units with a strength it can
   afford (32000 per level from a quarter of its money), into m (kind 7) */

int ai_DecideVerfluchen(int a, AIPER *p, int c, AIARG *m)
{
    int v, i, k, r, money;
    int bestv, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_TitleJobI(p->id, 0xe);
    bestv = 0;
    best = 0;
    flags = (unsigned int)best;
    if ((p->flags1cc & 0x10) != 0)
        return 0;
    if ((a != 0x45 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0x100; i != 0; i--) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x28a) != 0) {
            if (best == 0) {
                best = b;
                bestv = gm_CurseObjectCount(b);
            } else {
                n = gm_CurseObjectCount(b);
                if (n > bestv) {
                    best = b;
                    bestv = n;
                }
            }
        }
        b++;
    }
    if (best == 0 || bestv == 0)
        return 0;
    flags = PARTNER_FLAGS_ANY;
    if (ai_PickPartnerChain(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
        return 0;
    o = &((AIPER *)ai_CharsT)[id];
    money = (int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.25f);
    gm_GetWorth(p->id);
    for (k = bestv - 1; k > 0; k--) {
        if (money > (k > 1 ? k : 1) * 32000)
            break;
    }
    if (k == 0)
        return 0;
    r = util_RandomU(k + 1);
    m->ref = o->netId;
    k = r > 1 ? r : 1;
    m->ref2 = r;
    m->arg2 = k * 32;
    m->kind = 7;
    return 0x45;
}
/* 0x4ab9f0, execute callback of action 0x45 "Verfluchen": p is flagged (0x1cc bit 4), pays, and the curse goes
   out (message 0x9d: strength, victim) */

int ai_ExecVerfluchen(AIPER *p, AIARG *m)
{
    AIMSG msg;

    cm_SendSetField(p->netId, 0x1cc, 4, 0x10, 0);
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, -m->arg2);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.time = ai_Time;
    msg.type = 0x9d;
    msg.fromId = -1;
    msg.flags = 2;
    msg.arg[0] = m->ref2;
    msg.arg[1] = m->ref;
    cm_SendLongMessage(&msg);
    return 0x45;
}
/* 0x4abac0, decide callback of action 0x46 "Theater": p (money > 1, not of kind 3) picks its theatre (a building
   of kind 0x1f holding object 0x27f, no curse event on it; the bigger one, cap 2: 7, else 4), a roll r below that
   size capped by its money (2: 4, 3: 7, 4 and more: 10) and from r a play length (below 4: 2, below 7: 3, else
   4), into m (kind 7: the building, r, the length) */

int ai_DecideTheater(int a, AIPER *p, int c, AIARG *m)
{
    int v, k, n, i;
    int r;
    AIBLD *b, *best;
    AIBTYPE *bt;

    v = gm_TitleJobI(p->id, 0xe);
    k = 0;
    if ((a == 0x46 || a == 0) && c == 0 && v != 0 && p->kind != 3 && p->money >= 2) {
        best = 0;
        b = ai_BldsT;
        for (i = 0x100; i != 0; i--) {
            if (b->type != 0 && b->owner == p->id) {
                bt = &ai_BTypesT[b->type];
                if (bt->kind == 0x1f && gm_Query(b->objList, 2, 6, 0, 0x27f) != 0 && ai_FindCurseEvent(b) == 0) {
                    if (best == 0) {
                        k = bt->cap247 != 2 ? 4 : 7;
                        best = b;
                    } else {
                        n = bt->cap247 != 2 ? 4 : 7;
                        if (n > k) {
                            k = n;
                            best = b;
                        }
                    }
                }
            }
            b++;
        }
        if (best != 0 && k != 0) {
            if (p->money >= 4) {
                if (k >= 10)
                    k = 10;
            } else if (p->money >= 3) {
                if (k >= 7)
                    k = 7;
            } else if (k >= 4) {
                k = 4;
            }
            r = util_RandomU(k);
            if (r < 4)
                k = 2;
            else
                k = (r >= 7) + 3;
            m->ref = best->id;
            m->ref2 = r;
            m->arg2 = k;
            m->kind = 7;
            return 0x46;
        }
    }
    return 0;
}
/* 0x4abc60, execute callback of action 0x46 "Theater": p pays m->arg2 and puts on the play in building m->ref
   (message 0xa0, length m->ref2) */

int ai_ExecTheater(AIPER *p, AIARG *m)
{
    AIMSG msg;

    gm_SendMoneyDelta(p->netId, -m->arg2);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.fromId = m->ref;
    msg.type = 0xa0;
    msg.time = ai_Time;
    msg.flags = 2;
    msg.arg[0] = m->ref2;
    cm_SendLongMessage(&msg);
    return 0x46;
}
/* 0x4abd00, decide callback of action 0x47 "Ehrerbietung": p (money >= 2, not flagged 0x20, not of kind 3) with a
   building of kind 0x1f holding object 0x276 picks a character within 25..57 units to honour, into m (kind 7, a
   cost 2..7); returns 0x45 (sic) */

int ai_DecideEhrerbietung(int a, AIPER *p, int c, AIARG *m)
{
    int v, i, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b;
    AIPER *o;

    v = gm_TitleJobI(p->id, 0xe);
    flags = 0;
    if ((p->flags1cc & 0x20) != 0 || p->money < 2)
        return 0;
    if ((a != 0x47 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0; i < 0x100; i++, b++) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x276) != 0) {
            flags = PARTNER_FLAGS_ANY;
            if (ai_PickPartnerChain(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
                return 0;
            o = &((AIPER *)ai_CharsT)[id];
            n = util_RandomU(p->money < 8 ? p->money : 8);
            if (n <= 2)
                n = 2;
            m->arg2 = n;
            m->ref = o->netId;
            m->ref2 = 0;
            m->kind = 7;
            return 0x45;
        }
    }
    return 0;
}
/* 0x4abe60, execute callback of action 0x47 "Ehrerbietung": p is flagged (0x1cc bit 5), pays m->arg2 and wins
   five times that in standing with the character of m->ref */

int ai_ExecEhrerbietung(AIPER *p, AIARG *m)
{
    AIPER *o;

    o = gm_FindPer(m->ref);
    if (o == 0)
        return 0;
    cm_SendSetField(p->netId, 0x1cc, 4, 0x20, 0);
    gm_SendMoneyDelta(p->netId, -m->arg2);
    cm_SendRelations(p->id, o->id, m->arg2 * 5, 1);
    return 0x47;
}
/* 0x4abed0, registers the actions 0x44 "Blutrache", 0x45 "Verfluchen", 0x46 "Theater" and 0x47 "Ehrerbietung" */

int ai_RegisterSocialActions(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x44;
    strcpy(act.name, "Blutrache");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBlutrache;
    act.exec = ai_ExecBlutrache;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x45;
    strcpy(act.name, "Verfluchen");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideVerfluchen;
    act.exec = ai_ExecVerfluchen;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x46;
    strcpy(act.name, "Theater");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideTheater;
    act.exec = ai_ExecTheater;
    act.flags = 1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x47;
    strcpy(act.name, "Ehrerbietung");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideEhrerbietung;
    act.exec = ai_ExecEhrerbietung;
    act.flags = 3;
    return am_RegisterMethod(&act, a) != 0;
}

/* the poker unit's one .rdata constant: it precedes the unit's literals (77.0f at 0x5ed1c0, ...), after am's */
const int ai_CardBack = 0x18;  /* 0x5ed1bc, the picture offset of the card back (d2_Pics[0x775 + ai_CardBack], ai_PokerSeatPanel) */
G10GAME * ai_PokerTable = 0;  /* 0xf83654 */
G10CARD ai_PokerDeck[24] = {0};  /* 0xf83660 */
G10NPC ai_PokerOpponents[2] = {0};  /* 0xf837e0 */
int ai_LeisureActions[19] = { 18, 19, 20, 28, 29, 33, 36, 37, 39, 40, 41, 42, 43, 44, 45, 49, 50, 53, 55 };  /* 0x600e10, the leisure actions */
int ai_VisitActions[3] = { 0, 3, 7 };  /* 0x600e5c */
