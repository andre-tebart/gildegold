/* unk_4a7140.c -- the card poker game of the tavern against two NPCs (0x4a7140..): the game record 0xf83654,
   the deck 0xf83660 and the two opponents 0xf837e0. The am unit ends before it (its last .data string is
   "%sgebaeude/*%s_%c.ogr" 0x600478; this unit's block starts with the hand names 0x600490). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4a7140.c); boundaries in decl/text.dcl, notes in notes/units/unk_4a7140.md */
#include "txt_views.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4a7140.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */

/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "cm_types.h"
extern G17MSG ai_Unka69140[];


/* 0x4a7140, starts a game of character p in g: the stake from p's money, the deck, two random opponents */

int ai_PokerStart(AIPER *p, G10GAME *g)
{
    double d;
    int i, j, k;
    G10CARD *c;
    G10NPC *n;

    ai_Unkf83654 = 0;
    if (p == 0 || *((unsigned char *)p + 2) != 6 || g == 0)
        return 0;
    memset(ai_Unkf837e0, 0, sizeof(ai_Unkf837e0));
    ai_Unkf83654 = g;
    memset(g, 0, sizeof(G10GAME));
    ai_Unkf83654->seat[0].who = p;
    k = gm_ConvertMoney(gm_Unk509e30(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer), ai_OwnPlayer);
    d = k;
    if (!(10000.0 <= d))
        d = 10000.0;
    ai_Unkf83654->money = gm_PlayerFactor((int)(d / (log10(d) + 1.0) * 0.1), ai_OwnPlayer);
    ai_Unkf83654->stake = ai_Unkf83654->money / (util_RandomU(3) + 7);
    ai_Unkf83654->stake2 = ai_Unkf83654->stake;
    ai_Unkf83654->unkb8 = 4;
    k = 0;
    c = ai_Unkf83660;
    do {
        for (j = 0; j < 4; j++) {
            c->rank = j;
            c->suit = k;
            c++;
        }
        k++;
    } while (c < &ai_Unkf83660[24]);
    for (i = 0; i < 2; i++) {
        n = &ai_Unkf837e0[i];
        if (i == 0)
            n->female = (unsigned char)util_RandomU(2);
        else
            n->female = ai_Unkf837e0[0].female == 0;
        n->unk0d = (unsigned char)util_RandomU(4) + 1;
        strncpy(n->nname, ai_Unk143b178[util_RandomU(0x95)], 15);
        if (n->female == 0) {
            strncpy(n->vname, ai_Unk143ac5c[util_RandomU(0xbf)], 15);
            switch (util_RandomU(4)) {
            case 0:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buerger_MANN") + 0x639;
                break;
            case 1:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("Handwerker_MANN") + 0x639;
                break;
            case 2:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buerger3_DICKER") + 0x639;
                break;
            case 3:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("schneider1_MANN") + 0x639;
                break;
            }
        } else {
            strncpy(n->vname, ai_Unk143af70[util_RandomU(0x70)], 15);
            switch (util_RandomU(4)) {
            case 0:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buergerin_frau") + 0x639;
                break;
            case 1:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buergerin2_frau") + 0x639;
                break;
            case 2:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("minerin_frau") + 0x639;
                break;
            case 3:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("arbeiterin_frau") + 0x639;
                break;
            }
        }
        n->kind = 10;
        ai_Unkf83654->seat[i + 1].who = n;
    }
    return ai_Unk4a7430(g);
}
/* 0x4a7430, deals a new round: all cards back, the three seats cleared */

int ai_Unk4a7430(G10GAME *g)
{
    G10SEAT *s;
    int i;

    if (g == 0)
        return 0;
    g->left = 0x18;
    g->unkb5 = 0;
    g->unka0 = 0;
    g->phase = 0;
    g->unkb0 = util_RandomU(3);
    g->unkbc = 0;
    for (i = 0; i < 24; i++) {
        ai_Unkf83660[i].owner = 3;
        ai_Unkf83660[i].shown = 0;
    }
    s = g->seat;
    i = 3;
    do {
        s->card[0] = -1;
        s->card[1] = -1;
        s->card[2] = -1;
        s->card[3] = -1;
        s->card[4] = -1;
        s->unk1c = 0;
        s->unk2c = 0;
        s->unk24 = -1;
        s->unk20 = util_RandomU(5) == 1;
        s->unk2c = 0;
        s->unk28 = -1;
        s++;
    } while (--i);
    return 1;
}
/* 0x4a74e0, one step of the game: returns the dialog to show (0x10 / 0x21 / 9 / 0x29 / 0x1c) or 0 */

int ai_PokerStep(void)
{
    switch (ai_Unkf83654->phase) {
    case 0:
        ai_Unkf83654->phase = 1;
        return ai_Unkf83654->unkb0 != 0 ? 0x10 : 0x21;
    case 1:
        ai_Unk4a8650();
        ai_Unkf83654->phase++;
        ai_Unk4a8520();
        ai_Unk4a7ac0();
        ai_Unk4a86f0();
        return 0;
    case 2:
        ai_Unkf83654->phase = 3;
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 3:
        ai_Unk4a7ac0();
        ai_Unk4a7c20();
        ai_Unkf83654->phase++;
        ai_Unk4a8590();
        ai_Unk4a7a20();
        return 0;
    case 4:
        ai_Unkf83654->phase = 5;
        ai_Unk4a7ac0();
        ai_Unk4a86f0();
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 5:
        ai_Unk4a7c20();
        ai_Unk4a7a20();
        ai_Unkf83654->phase++;
        return 0x1c;
    case 6:
        ai_Unk4a77e0();
        ai_Unkf83654->phase++;
        return 0x10;
    case 7:
        ai_Unk4a85e0();
        ai_Unkf83654->phase++;
        break;
    case 8:
        ai_Unk4a7ac0();
        ai_Unkf83654->phase++;
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 9:
        ai_Unk4a7c20();
        ai_Unk4a7a20();
        ai_Unkf83654->phase++;
        return 0;
    case 10:
        ai_Unkf83654->phase = 11;
        ai_Unk4a8690();
        return 0x10;
    }
    return 0;
}
/* 0x4a7650, the first group (of 4 cards) other than skip of which seat 2 holds two or more cards (only shown ones
   if shown): its index into out, the count as the result, 0 if none */

int ai_Unk4a7650(int *hand, int shown, int skip, int *out)
{
    int cnt[6];
    int s, g, i, n, best;
    G10CARD *c;
    int *q;

    for (s = 1; s < 3; s++) {
        best = 0;
        q = cnt;
        c = ai_Unkf83660;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s && (shown == 0 || c->shown != 0))
                    n++;
                c++;
            }
            if (best < n)
                best = n;
            *q++ = n;
        } while (c < &ai_Unkf83660[24]);
    }
    for (g = 0; g == skip || cnt[g] < 2;) {
        if (++g > 5)
            return 0;
    }
    *out = g;
    return cnt[g];
}
/* 0x4a76d0, the highest card of a hand (group, then rank): of cards 0..4, or 2..4 if high */

int ai_Unk4a76d0(int *hand, int high)
{
    unsigned int i;
    int best, k, g, r;

    i = high != 0 ? 2 : 0;
    best = hand[i];
    if (best < 0)
        return -1;
    if (hand[i + 1] >= 0) {
        g = ai_Unkf83660[best].suit;
        r = ai_Unkf83660[best].rank;
        for (i++; (int)i < 5; i++) {
            k = hand[i];
            if (k < 0)
                return best;
            if (g < ai_Unkf83660[k].suit || (ai_Unkf83660[k].suit == g && r < ai_Unkf83660[k].rank)) {
                g = ai_Unkf83660[k].suit;
                r = ai_Unkf83660[k].rank;
                best = k;
            }
        }
    }
    return best;
}
/* 0x4a7760, deals a card from the deck to owner (the first one when 10 or fewer are left, else a random one) */

int ai_Unk4a7760(int owner)
{
    unsigned int i;

    if (ai_Unkf83654->left == 0)
        return -1;
    if (ai_Unkf83654->left < 11) {
        for (i = 0; i < 24; i++)
            if (ai_Unkf83660[i].owner == 3)
                break;
    } else {
        do
            i = util_RandomU(24);
        while (ai_Unkf83660[i].owner != 3);
    }
    ai_Unkf83660[i].owner = owner;
    ai_Unkf83654->left--;
    return i;
}
/* 0x4a77e0, the NPCs exchange a card by their strategy (seat->unk2c): keep pairs (1), the most common rank (2) */

void ai_Unk4a77e0(void)
{
    int cnt[6];
    int s, g, j, i, n, k;
    int pairs, maxn, first, second, maxr, bestr;
    G10CARD *c, *q;
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
        c = ai_Unkf83660;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s)
                    n++;
                c++;
            }
            if (n > 1)
                pairs++;
            if (maxn < n) {
                second = first;
                maxn = n;
                first = g;
            }
            cnt[g] = n;
            g++;
        } while (c < &ai_Unkf83660[24]);
        j = 0;
        c = ai_Unkf83660;
        do {
            n = 0;
            q = c;
            for (i = 6; i != 0; i--) {
                if (q->owner == s)
                    n++;
                q += 4;
            }
            if (maxr < n) {
                maxr = n;
                bestr = j;
            }
            cnt[j] = n;
            c++;
            j++;
        } while (c < &ai_Unkf83660[4]);
        st = &ai_Unkf83654->seat[s];
        switch (st->unk2c) {
        case 0:
            k = util_RandomU(3);
            break;
        case 1:
            if (pairs == 1) {
                if (ai_Unkf83660[st->card[0]].suit != first)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first)
                    k = 1;
            } else if (pairs == 2) {
                if (ai_Unkf83660[st->card[0]].suit != first && ai_Unkf83660[st->card[0]].suit != second)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first && ai_Unkf83660[st->card[1]].suit != second)
                    k = 1;
            }
            break;
        case 2:
            if (ai_Unkf83660[st->card[0]].rank != bestr)
                k = 0;
            else if (ai_Unkf83660[st->card[1]].rank != bestr)
                k = 1;
            break;
        case 3:
            if (pairs == 1) {
                if (ai_Unkf83660[st->card[0]].suit == first)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first)
                    k = 1;
            }
            break;
        }
        if (k >= 0)
            ai_Unk4a79d0(st, k);
    }
}
/* 0x4a79d0, seat s puts card k (0 or 1) away and draws a new one in its place */

void ai_Unk4a79d0(G10SEAT *s, int k)
{
    int o, n;

    if (k < 2) {
        o = ai_Unkf83660[s->card[k]].owner;
        ai_Unkf83660[s->card[k]].owner = 4;
        s->unk24 = s->card[k];
        n = ai_Unk4a7760(o);
        s->card[k] = n;
        ai_Unkf83660[n].shown = 0;
    }
}
/* 0x4a7a20, every seat still in (unk28 < 0) pays the stake into the pot (0xa0); the player's from p's money */

void ai_Unk4a7a20(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (ai_Unkf83654->seat[i].unk28 < 0) {
            ai_Unkf83654->unka0 += ai_Unkf83654->stake2;
            if (i == 0) {
                gm_PlayerFactor(ai_Unkf83654->stake2, ai_OwnPlayer);
                cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), ai_OwnPlayer,
                             ai_Unkf83654->stake2);
            }
            ai_Unkf83654->seat[i].unk1c += ai_Unkf83654->stake2;
        }
    }
}
/* 0x4a7ac0, picks each NPC's strategy (seat->unk2c) from its hand: 3 nothing yet, 2 go for the rank, 1 pairs,
   0 random */

void ai_Unk4a7ac0(void)
{
    unsigned int cnt[6];
    int s, j, i, pairs, first;
    unsigned int n, maxn, maxr;
    unsigned char b5;
    G10CARD *c, *q;
    unsigned int *pc;

    first = 0;
    for (s = 1; s < 3; s++) {
        maxr = 0;
        maxn = 0;
        pairs = 0;
        c = ai_Unkf83660;
        pc = cnt;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s)
                    n++;
                c++;
            }
            if ((int)n > 1)
                pairs++;
            if ((int)maxn < (int)n)
                maxn = n;
            *pc++ = n;
        } while (c < &ai_Unkf83660[24]);
        j = 0;
        c = ai_Unkf83660;
        do {
            n = 0;
            q = c;
            for (i = 6; i != 0; i--) {
                if (q->owner == s)
                    n++;
                q += 4;
            }
            if ((int)maxr < (int)n) {
                maxr = n;
                first = j;
            }
            cnt[j] = n;
            c++;
            j++;
        } while (c < &ai_Unkf83660[4]);
        b5 = ai_Unkf83654->unkb5;
        if (b5 < 5 && pairs < 2 && (int)maxn < 2) {
            ai_Unkf83654->seat[s].unk2c = 3;
        } else if (b5 == maxr) {
            ai_Unkf83654->seat[s].unk2c = 2;
        } else {
            if (ai_Unkf83654->phase < 6 && (int)(b5 - maxr) < 2) {
                for (j = 0; j < 6; j++)
                    if (ai_Unkf83660[j * 4 + first].owner != s && ai_Unkf83660[j * 4 + first].shown == 0)
                        break;
                if (j == 6) {
                    ai_Unkf83654->seat[s].unk2c = 2;
                    continue;
                }
            }
            if (b5 == 5 && (int)maxn < 2)
                ai_Unkf83654->seat[s].unk2c = 0;
            else
                ai_Unkf83654->seat[s].unk2c = 1;
        }
    }
}
/* 0x4a7c20, each NPC decides whether to fold (seat->unk28 = the phase) against the open cards of the others */

void ai_Unk4a7c20(void)
{
    int s, j, n, r, m, k, y;
    int x8, xc;
    G10SEAT *st;

    for (s = 1; s < 3; s++) {
        st = &ai_Unkf83654->seat[s];
        if (st->unk2c == 0 && st->unk20 == 0 && ai_Unkf83654->unkb5 > 3)
            st->unk28 = ai_Unkf83654->phase;
        for (j = 0; j < 2; j++) {
            if (j != s && ai_Unkf83654->seat[j].unk28 < 0) {
                ai_Unk4a7e70(&ai_Unkf83654->seat[j], &n, &r, &x8, &xc);
                switch (st->unk2c) {
                case 0:
                    if (st->unk20 == 0) {
                        if (n > 1 || util_RandomU(4) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    } else if (n > 2) {
                        st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 1:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &k);
                        if (ai_Unkf83654->phase > 5 && m <= n && (m < n || k < r))
                            st->unk28 = ai_Unkf83654->phase;
                    } else {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (m < n && ai_Unkf83654->phase > 5 && ai_Unkf83654->unkb5 > 4)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 2:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (ai_Unkf83654->unkb5 < 5 && m == 3 && util_RandomU(4) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 3:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (ai_Unkf83654->unkb5 < 5 && m == 3 && util_RandomU(5) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                }
            }
        }
    }
}
/* 0x4a7e70, the open cards (2 .. unkb5-1) of seat s: the most common group (count, group) and rank (count, rank) */

void ai_Unk4a7e70(G10SEAT *s, int *n, int *g, int *m, int *r)
{
    int cnt[10];
    int i, k;
    int *p;

    cnt[1] = 0;
    cnt[5] = 0;
    cnt[2] = 0;
    cnt[6] = 0;
    cnt[3] = 0;
    cnt[4] = 0;
    cnt[7] = 0;
    cnt[8] = 0;
    cnt[0] = 0;
    cnt[9] = 0;
    if (ai_Unkf83654->unkb5 > 2) {
        i = ai_Unkf83654->unkb5 - 2;
        p = &s->card[2];
        do {
            k = *p++;
            cnt[ai_Unkf83660[k].rank]++;
            cnt[ai_Unkf83660[k].suit + 4]++;
        } while (--i);
    }
    *n = cnt[4];
    *g = 0;
    for (i = 1; i < 6; i++) {
        if (*n < cnt[i + 4]) {
            *n = cnt[i + 4];
            *g = i;
        }
    }
    *m = cnt[0];
    *r = 0;
    for (i = 1; i < 4; i++) {
        if (*m < cnt[i]) {
            *m = cnt[i];
            *r = i;
        }
    }
}
/* 0x4a7f50, rates the hand of owner: 0 nothing, 1 pair, 2 two pairs, 3 three, 4 straight, 5 full house, 6 flush,
   7 poker, 8 straight flush (the names at 0x600490); the groups into g1 / g2, the highest ranks into r1 / r2 */

char ai_Unk4a7f50(int owner, int *g1, int *g2, int *r1, int *r2)
{
    int cnt[6];
    int rcnt[4];
    int g, r, n, i;
    int pairs, rpairs, maxn, maxr, bestr, kind, a, b;
    G10CARD *c, *q, *q2;

    pairs = 0;
    rpairs = 0;
    maxn = 0;
    maxr = 0;
    bestr = 0;
    kind = 0;
    g = 0;
    c = ai_Unkf83660;
    b = -1;
    a = 0;
    do {
        n = 0;
        for (i = 4; i != 0; i--) {
            if (c->owner == owner)
                n++;
            c++;
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
        g++;
    } while (c < &ai_Unkf83660[24]);
    r = 0;
    c = ai_Unkf83660;
    do {
        n = 0;
        q = c;
        for (i = 6; i != 0; i--) {
            if (q->owner == owner)
                n++;
            q += 4;
        }
        rcnt[r] = n;
        if (n > 1)
            rpairs++;
        if (maxr < n) {
            maxr = n;
            bestr = r;
        }
        c++;
        r++;
    } while (c < &ai_Unkf83660[4]);
    if (rpairs == 1 && maxr == 5) {
        kind = 6;
        *r1 = bestr;
    }
    if (pairs == 0) {
        if (cnt[0] == 0 || cnt[5] == 0)
            return (kind == 6) * 4 + 4;
        if (kind == 6)
            return 6;
    } else if (pairs == 1) {
        if (maxn == 4) {
            *g1 = a;
            return 7;
        }
        if (maxn == 3) {
            *g1 = a;
            return 3;
        }
        if (maxn == 2) {
            *g1 = a;
            q = &ai_Unkf83660[a * 4 + 3];
            for (r = 3; r >= 0; r--) {
                if (q->owner == owner) {
                    *r1 = r;
                    return 1;
                }
                q--;
            }
            return 1;
        }
    } else if (pairs == 2) {
        if (maxn == 3) {
            *g1 = a;
            *g2 = b;
            return 5;
        }
        if (maxn == 2) {
            *g1 = a;
            *g2 = b;
            q2 = &ai_Unkf83660[b * 4 + 3];
            q = &ai_Unkf83660[a * 4 + 3];
            for (r = 3; r >= 0; r--) {
                if (q->owner == owner)
                    *r1 = r;
                if (q2->owner == owner)
                    *r2 = r;
                q--;
                q2--;
            }
            return 2;
        }
    }
    return 0;
}
/* 0x4a8190, the showdown: the winning seat (seat 0 against 1, the better of them against 2), its hand into unkbc */

int ai_Unk4a8190(void)
{
    int g1[3], r1[3], kind[3], out[3], g2[3];
    int r2[3];
    unsigned int i, w;
    int k, a, b;

    kind[0] = 0;
    kind[2] = 0;
    kind[1] = 0;
    out[0] = 0;
    out[2] = 0;
    out[1] = 0;
    for (i = 0; i < 3; i++) {
        if (ai_Unkf83654->seat[i].unk28 < 0) {
            kind[i] = ai_Unk4a7f50(i, &g1[i], &g2[i], &r1[i], &r2[i]);
            ai_Unkf83654->seat[i].unk30 = kind[i];
        } else {
            out[i] = 1;
        }
    }
    if (out[0] != 0) {
        w = 1;
    } else if (out[1] != 0) {
        w = 0;
    } else if (kind[0] == 0) {
        if (kind[1] == 0)
            w = ai_Unk4a76d0(ai_Unkf83654->seat[0].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
        else
            w = 1;
    } else if (kind[1] == 0 || kind[1] < kind[0]) {
        w = 0;
    } else if (kind[0] < kind[1]) {
        w = 1;
    } else {
        switch (kind[0]) {
        case 1:
            if (g1[0] > g1[1])
                w = 0;
            else if (g1[0] < g1[1])
                w = 1;
            else
                w = r1[0] <= r1[1];
            break;
        case 2:
            if (g1[1] < g1[0])
                w = 0;
            else if (g1[0] < g1[1])
                w = 1;
            else if (g2[1] < g2[0])
                w = 0;
            else if (g2[1] <= g2[0])
                w = r1[0] <= r1[1];
            else
                w = 1;
            break;
        case 3:
        case 7:
            w = g1[0] <= g1[1];
            break;
        case 4:
            w = ai_Unk4a76d0(ai_Unkf83654->seat[0].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
            break;
        case 5:
            r1[0] = g1[0];
            r1[1] = g1[1];
            w = r1[0] <= r1[1];
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
        ai_Unkf83654->unkbc = kind[w];
        return w;
    }
    k = kind[w];
    if (k == 0) {
        if (kind[2] != 0) {
            ai_Unkf83654->unkbc = kind[2];
            return 2;
        }
        a = ai_Unk4a76d0(ai_Unkf83654->seat[w].card, 0);
        b = ai_Unk4a76d0(ai_Unkf83654->seat[1].card, 0);
        if (b < a)
            return w;
        return 1;
    }
    if (kind[2] != 0 && kind[2] >= k) {
        if (k < kind[2]) {
            ai_Unkf83654->unkbc = kind[2];
            return 2;
        }
        switch (k) {
        case 1:
            if (g1[w] <= g1[2]) {
                if (g1[w] < g1[2] || r1[w] <= r1[2]) {
                    ai_Unkf83654->unkbc = kind[2];
                    return 2;
                }
            }
            break;
        case 2:
            if (g1[w] <= g1[2]) {
                if (g1[w] < g1[2]) {
                    ai_Unkf83654->unkbc = kind[2];
                    return 2;
                }
                if (g2[w] <= g2[2]) {
                    if (g2[w] < g2[2] || r1[w] <= r1[2]) {
                        ai_Unkf83654->unkbc = kind[2];
                        return 2;
                    }
                }
            }
            break;
        case 3:
        case 5:
        case 7:
            if (g1[w] <= g1[2]) {
                ai_Unkf83654->unkbc = kind[2];
                return 2;
            }
            break;
        case 4:
            if (ai_Unk4a76d0(ai_Unkf83654->seat[w].card, 0) <= ai_Unk4a76d0(ai_Unkf83654->seat[2].card, 0))
                w = 2;
            ai_Unkf83654->unkbc = kind[w];
            return w;
        case 6:
        case 8:
            if (r1[w] <= r1[2]) {
                ai_Unkf83654->unkbc = kind[2];
                return 2;
            }
            break;
        default:
            return w;
        }
    }
    ai_Unkf83654->unkbc = k;
    return w;
}
/* 0x4a8520, deals three cards to each seat (the first two hidden, the third open) */

void ai_Unk4a8520(void)
{
    int k, i;

    for (k = 0; k < 3; k++) {
        for (i = 0; i < 3; i++) {
            ai_Unkf83654->seat[i].card[k] = ai_Unk4a7760(i);
            if (k > 1)
                ai_Unkf83660[ai_Unkf83654->seat[i].card[k]].shown = 1;
            else
                ai_Unkf83660[ai_Unkf83654->seat[i].card[k]].shown = 0;
        }
    }
    ai_Unkf83654->unkb5 += 3;
}
/* 0x4a8590, deals the fourth card to each seat, open */

void ai_Unk4a8590(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_Unkf83654->seat[i].card[3] = ai_Unk4a7760(i);
        ai_Unkf83660[ai_Unkf83654->seat[i].card[3]].shown = 1;
    }
    ai_Unkf83654->unkb5++;
}
/* 0x4a85e0, deals the fifth card to each seat, open */

void ai_Unk4a85e0(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_Unkf83654->seat[i].card[4] = ai_Unk4a7760(i);
        ai_Unkf83660[ai_Unkf83654->seat[i].card[4]].shown = 1;
    }
    ai_Unkf83654->unkb5++;
}
/* 0x4a8630, the player exchanges card k */

void ai_PokerExchange(int k)
{
    ai_Unk4a79d0(&ai_Unkf83654->seat[0], k);
}
/* 0x4a8650, the ante: three stakes into the pot, the player's from p's money */

void ai_Unk4a8650(void)
{
    ai_Unkf83654->unka0 = ai_Unkf83654->stake2 * 3;
    cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), ai_OwnPlayer, ai_Unkf83654->stake2);
}
/* 0x4a8690, ends the round: the winner into unkb8, the pot to the player if he won, all cards back */

void ai_Unk4a8690(void)
{
    int i;

    ai_Unkf83654->unkb8 = (short)ai_Unk4a8190();
    if (ai_Unkf83654->unkb8 == 0)
        cm_SendMoveGoodsUnchecked(*(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), -1, ai_OwnPlayer,
                     ai_Unkf83654->unka0);
    for (i = 0; i < 24; i++) {
        ai_Unkf83660[i].owner = 3;
        ai_Unkf83660[i].shown = 0;
    }
}
/* 0x4a86f0, the NPC whose turn it is (unkb0) sets the stake: up to half the margin above the stake, or all of it
   if it bluffs */

void ai_Unk4a86f0(void)
{
    int n;

    if (ai_Unkf83654->unkb0 != 0) {
        n = ai_Unkf83654->seat[ai_Unkf83654->unkb0].unk20;
        if (n == 0)
            n = ai_Unkf83654->money - ai_Unkf83654->stake >> 1;
        else
            n = (unsigned short)(ai_Unkf83654->money - ai_Unkf83654->stake);
        ai_Unkf83654->stake2 = util_RandomU(n) + ai_Unkf83654->stake;
    }
}
/* 0x4a8760, decide callback: character p (or the one of the ctx record, kind 2) picks a partner in its district
   (a character with flag 1 within 33 units, else one of a higher rank within 40..71), into m (kind 7) */

int ai_Unk4a8760(int a, unsigned char *p, int *ctx, int d, unsigned char *m)
{
    G10DIST info, info2;
    G10DREC rec[6];
    unsigned char *who[6];
    unsigned char *o;
    unsigned char k;
    int id, n, cnt, i, j;
    unsigned int r;

    i = 0;
    o = 0;
    if (ctx[0] == 1) {
        if (p[0x168] != 0)
            return 0;
        id = *(int *)(p + 4);
        k = p[0x166];
    } else {
        if (ctx[0] != 2)
            return 0;
        o = (unsigned char *)ctx[0x87];
        if (o == 0)
            return 0;
        k = o[0x166];
        id = *(int *)(o + 4);
        o = 0;
    }
    if (amt_GetKind(k, &info) != 0) {
        n = amt_GetColumnOffices(info.id, 6, rec);
        j = 0;
        cnt = 0;
        if (n > 0) {
            for (j = 0; j < n; j++) {
                if (rec[j].k == 2) {
                    cnt++;
                    if (rec[j].id == id)
                        return 0;
                }
            }
            if (cnt > 2)
                return 0;
        }
        if (n > 0) {
            for (i = 0; i < n; i++)
                who[i] = (unsigned char *)gm_FindChar(rec[i].id);
        }
        if (n < 6)
            memset(&who[n], 0, (6 - n) * sizeof(who[0]));
        r = util_RandomU(6);
        for (j = 6; j != 0; j--) {
            if (who[r] != 0 && id != *(int *)(who[r] + 4)
                && gm_Liking(*(short *)who[r], *(short *)p, 1) < 33.0f && rec[r].k == 1) {
                o = who[r];
                if (o != 0 && *(int *)(p + 0x5c) != *(int *)(o + 4)) {
                    m[0] = 7;
                    *(int *)(m + 4) = *(int *)(o + 4);
                    return 1;
                }
                break;
            }
            r = (int)(r + 1) % 6;
        }
        if (info.lvl != 3 && info.lvl != 6 && info.lvl != 9 && p[0x130] == 7) {
            r = util_RandomU(6);
            for (j = 6; j != 0; j--) {
                if (who[r] != 0 && id != *(int *)(who[r] + 4)) {
                    if (amt_GetKind(who[r][0x166], &info2) == 0)
                        return 0;
                    if (info.lvl < info2.lvl && rec[r].k == 1
                        && gm_Liking(*(short *)who[r], *(short *)p, 1) < (float)util_RandomU(0x20) + 40.0f) {
                        o = who[r];
                        break;
                    }
                }
                r = (int)(r + 1) % 6;
            }
            if (o != 0 && *(int *)(p + 0x5c) != *(int *)(o + 4)) {
                m[0] = 7;
                *(int *)(m + 4) = *(int *)(o + 4);
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4a8a50, decide callback: the decision record ctx[1] proposes a value (its fn) within min..max, into m
   (kind 0x11) */

int ai_Unk4a8a50(int a, AIPER *p, int *ctx, int d, unsigned char *m)
{
    AIDEC dec;
    int v;

    if (gv_GetTableRecord(ctx[1], &dec) != 0) {
        if (((int (*)(AIPER *, int, int *))dec.fn)(p, (unsigned char)dec.unk00, &v) != 0 && v != dec.cur
            && v >= dec.min && v <= dec.max) {
            m[0] = 0x11;
            *(unsigned int *)(m + 4) = (unsigned char)dec.unk00;
            *(int *)(m + 8) = v;
            return 1;
        }
    }
    return 0;
}
/* 0x4a8ad0, decide callback: p (unk194 >= 3) picks a character of the other sex (0x4a58d0), into m (kind 7) with
   a random count 1..5 */

int ai_Unk4a8ad0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 3)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c2) - (((unsigned char *)p)[9] != 0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    if (util_RandomU(p->unk194 / 2) > 1 && util_RandomU(p->unk194 / 2) > 4) {
        *(int *)(m + 8) = 5;
        return 1;
    }
    if (util_RandomU(p->unk194 / 2) > 1) {
        *(unsigned int *)(m + 8) = util_RandomU(p->unk194 / 2);
        return 1;
    }
    *(int *)(m + 8) = 1;
    return 1;
}
/* 0x4a8bf0, eval callback: one in four */

int ai_Unk4a8bf0(void)
{
    return util_RandomU(4) == 0;
}
/* 0x4a8c10, eval callback: always */

int ai_AlwaysTrue(void)
{
    return 1;
}
/* 0x4a8c20, eval callback: on p's day (id & 3 = day % 4, id & 7 = the hour & 7), one in four */

int ai_Unk4a8c20(int a, AIPER *p)
{
    if ((p->netId & 3) == ai_TimeT.year % 4 && (p->netId & 7) == (ai_TimeT.hour & 7))
        return util_RandomU(4) == 0;
    return 0;
}
/* 0x4a8c80, decide callback: two characters near p (0x4a58d0), into m (kind 7) */

int ai_Unk4a8c80(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags[2];
    unsigned short id[2];

    flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    flags[1] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 2, flags, 0, (float)util_RandomU(0x40), id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id[0]].netId;
    *(int *)(m + 8) = ((AIPER *)ai_CharsT)[id[1]].netId;
    return 1;
}
/* 0x4a8d20, decide callback: p (unk194 >= 3) picks a character within 32..49 units, into m (kind 7) */

int ai_Unk4a8d20(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 3)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x12) + 32.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a8db0, decide callback: a building of the character 0x6f0468 (or one near p), and a random character whose
   building (0x16c) it is, into m (kind 7) */

int ai_Unk4a8db0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int k;
    unsigned int i;
    int n;
    AIBLD *b;

    if (ai_Partner != 0)
        ai_Unk4a63f0(AI_6F0468, 4, 0x1807d4190, 0, 100.0f, &k);
    if (k == -1) {
        if (ai_Unk4a63f0(p, 6, 0x1807d4190, 0, (float)util_RandomU(0x20) + 32.0f, &k) == 0)
            return 0;
    }
    b = gm_BuildQuery(1, 1, k);
    if (b == 0)
        return 0;
    i = util_RandomU(0x300);
    n = 0x300;
    while (((AIPER *)ai_CharsT)[i].id == 0xffff
           || *(AIBLD **)((unsigned char *)&((AIPER *)ai_CharsT)[i] + 0x16c) != b
           || ((AIPER *)ai_CharsT)[i].kind != 0) {
        if (--n == 0)
            return 0;
        i = (int)(i + 1) % 0x300;
    }
    if (&((AIPER *)ai_CharsT)[i] == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[i].netId;
    *(int *)(m + 8) = b->id;
    return 1;
}
/* 0x4a8ee0, eval callback: two in three if p's jobId is of kind 7, else one in four */

int ai_Unk4a8ee0(int a, AIPER *p)
{
    if (gm_TitleGroup(p->jobId) == 7 && util_RandomU(3) != 0)
        return 1;
    return util_RandomU(4) == 0;
}
/* 0x4a8f20, decide callback: two characters 33..100 units from p of which one is within 60 units of the other,
   into m (kind 7) */

int ai_Unk4a8f20(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags[2];
    unsigned short id[2];

    flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    flags[1] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (G10_4A58D0(p, 2, flags, 33.0f, (float)util_RandomU(0x18) + 77.0f, id) == 0)
        return 0;
    if (gm_Liking(id[0], id[1], 1) > 60.0f && gm_Liking(id[1], id[0], 1) > 60.0f)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id[0]].netId;
    *(int *)(m + 8) = ((AIPER *)ai_CharsT)[id[1]].netId;
    return 1;
}
/* 0x4a9030, eval callback: p (unk194 >= 2) with n > 0 of 0x4f39b0: the count, at most unk194 / 2, into m->0x10 */

int ai_Unk4a9030(int a, AIPER *p, int c, unsigned char *m)
{
    int n, k;

    if (p->unk194 < 2)
        return 0;
    n = unk_Unk4f39b0(p->netId);
    if (n < 1)
        return 0;
    k = p->unk194 / 2;
    if (n <= k)
        k = n;
    *(int *)(m + 0x10) = k;
    return 1;
}
/* 0x4a9080, decide callback: a character p knows well enough (0x4f3c50, at least 2..5) that is free (not of kind
   15, not in the places 13 / 27 / 15 / 21), into m (kind 7) */

int ai_Unk4a9080(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int cnt[32];
    int ids[32];
    unsigned short r;
    int n, i, j;
    AIPER *o;
    unsigned char k;

    r = util_RandomU(4);
    if (p->unk166 != 13) {
        n = unk_Unk4f3c50(ids, cnt, p->id);
        if (n != 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= r + 2) {
                    o = AI_505DF0(ids[i]);
                    if (o != 0 && o->kind != 15 && ((unsigned char *)o)[8] != 0) {
                        k = o->unk166;
                        if (k != 13 && k != 27 && k != 15 && k != 21) {
                            m[0] = 7;
                            *(int *)(m + 4) = o->netId;
                            return 1;
                        }
                    }
                }
                i = (i + 1) % n;
            } while (--j);
        }
    }
    return 0;
}
/* 0x4a9150, eval callback: p (unk194 > 7) on its day (id & 3 = day % 4), one in four */

int ai_Unk4a9150(int a, AIPER *p)
{
    if (p->unk194 >= 8 && ai_TimeT.year % 4 == (p->netId & 3) && util_RandomU(4) == 0)
        return 1;
    return 0;
}
/* 0x4a91a0, decide callback: on p's day, p without a partner (0x5c) and not in a list 0x53e400 record (0xac /
   0xb0) picks one of up to 12 characters of the other sex 30..100 units away (0x4a5ef0), into m (kind 7) */

int ai_Unk4a91a0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    unsigned short ids[12];
    AIJOB *r;
    unsigned int n;

    if (ai_TimeT.year % 4 == (p->netId & 3) && gm_FindChar(*(int *)((unsigned char *)p + 0x5c)) == 0) {
        for (r = he_FindEvent(1, 0, 0x41); r != 0; r = he_NextEvent()) {
            if (r->unkb0 == p->netId)
                return 0;
            if (r->unkac == p->netId)
                return 0;
        }
        n = ai_Unk4a5ef0(p, 12, ((unsigned)&ai_Unka69140 + 0x1982ca) - (((unsigned char *)p)[9] != 0), 30.0f, 100.0f, ids);
        if (n != 0) {
            m[0] = 7;
            *(int *)(m + 4) = ((AIPER *)ai_CharsT)[ids[util_RandomU(n)]].netId;
            return 1;
        }
    }
    return 0;
}
/* 0x4a9280, decide callback: as 0x4a9080 with a closer acquaintance (at least 5..12) */

int ai_Unk4a9280(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int cnt[32];
    int ids[32];
    unsigned short r;
    int n, i, j;
    AIPER *o;
    unsigned char k;

    r = util_RandomU(8);
    if (p->unk166 != 13) {
        n = unk_Unk4f3c50(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= r + 5) {
                    o = AI_505DF0(ids[i]);
                    if (o != 0 && o->kind != 15 && ((unsigned char *)o)[8] != 0) {
                        k = o->unk166;
                        if (k != 13 && k != 27 && k != 15 && k != 21) {
                            m[0] = 7;
                            *(int *)(m + 4) = o->netId;
                            return 1;
                        }
                    }
                }
                i = (i + 1) % n;
            } while (--j);
        }
    }
    return 0;
}
/* 0x4a9350, decide callback: p (unk194 > 1, not flagged 0x100) picks an acquaintance (more than 3) not flagged
   0x200 whose district level is above 3, into m (kind 7) */

int ai_Unk4a9350(int a, AIPER *p, int c, int d, unsigned char *m)
{
    G10DIST info;
    int cnt[32];
    int ids[32];
    int n, i, j;
    AIPER *o;

    if (p->unk194 > 1 && (p->flags1c8 & 0x100) == 0) {
        n = unk_Unk4f3c50(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] > 3) {
                    o = AI_505DF0(ids[i]);
                    if (o != 0 && (o->flags1c8 & 0x200) == 0) {
                        if (amt_GetKind(o->unk166, &info) != 0 && info.lvl > 3) {
                            m[0] = 7;
                            *(int *)(m + 4) = o->netId;
                            return 1;
                        }
                    }
                }
                i = (i + 1) % n;
            } while (--j);
        }
    }
    return 0;
}
/* 0x4a9420, decide callback: p (unk194 >= 2) picks a character within 40..71 units that 0x553d60 accepts, into
   m (kind 7) */

int ai_Unk4a9420(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 < 2)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x20) + 40.0f, &id) == 0)
        return 0;
    if (he_Unk553d60(p, &((AIPER *)ai_CharsT)[id]) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a94e0, decide callback: p (unk194 > 2, not flagged 0x400) picks a character within 18..49 units, not flagged
   0x400, by chance after 0x504e20, into m (kind 7) */

int ai_Unk4a94e0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 >= 3 && (p->flags1c8 & 0x400) == 0) {
        flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
        if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x20) + 18.0f, &id) != 0
            && (((AIPER *)ai_CharsT)[id].flags1c8 & 0x400) == 0
            && util_RandomF() + gm_Mix(p, &((AIPER *)ai_CharsT)[id], 3) >= 1.0f) {
            m[0] = 7;
            *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
            return 1;
        }
    }
    return 0;
}
/* 0x4a95d0, decide callback: p (not flagged 0x80000) picks a character 20..63 units away, into m (kind 7) */

int ai_Unk4a95d0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x80000) != 0)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x497bc0);
    if (G10_4A58D0(p, 1, &flags, 20.0f, (float)util_RandomU(0x20) + 32.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a9660, decide callback: p (not flagged 0x40000, unk194 >= 8) picks a character within 18..33 units, into m
   (kind 7) */

int ai_Unk4a9660(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if ((p->flags1cc & 0x40000) != 0)
        return 0;
    if (p->unk194 < 8)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x497bc0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x10) + 18.0f, &id) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4a9700, decide callback: p (not flagged 0x100, unk194 >= 4) picks a character of district 4 within 16..31 units,
   not its partner (0x5c), into m (kind 7); 0x20 if p cannot */

int ai_Unk4a9700(int a, AIPER *p, int *ctx, int d, unsigned char *m)
{
    G10DREC rec[6];
    AIPER *who[6];
    AIPER *o;
    int id, n, i;
    unsigned int r;

    if ((p->flags1c8 & 0x100) != 0)
        return 0x20;
    if (p->unk194 < 4)
        return 0x20;
    if (ctx[0] == 1) {
        id = p->netId;
    } else {
        if (ctx[0] != 2)
            return 0;
        if (ctx[0x87] == 0)
            return 0;
        id = ((AIPER *)ctx[0x87])->netId;
    }
    n = amt_GetColumnOffices(4, 6, rec);
    for (i = 0; i < n; i++)
        who[i] = AI_505DF0(rec[i].id);
    if (n < 6)
        memset(&who[n], 0, (6 - n) * sizeof(who[0]));
    r = util_RandomU(6);
    i = 6;
    do {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < (float)util_RandomU(0x10) + 16.0f) {
            o = who[r];
            if (o == 0)
                return 0;
            if (*(int *)((unsigned char *)p + 0x5c) == o->netId)
                return 0;
            m[0] = 7;
            *(int *)(m + 4) = o->netId;
            return 1;
        }
        r = (int)(r + 1) % 6;
    } while (--i);
    return 0;
}
/* 0x4a9880, decide callback: p (not flagged 0x100, unk194 >= 4) picks a character of district 5 within 16..31 units,
   not its partner (0x5c), into m (kind 7); 0x20 if p cannot */

int ai_Unk4a9880(int a, AIPER *p, int *ctx, int d, unsigned char *m)
{
    G10DREC rec[6];
    AIPER *who[6];
    AIPER *o;
    int id, n, i;
    unsigned int r;

    if ((p->flags1c8 & 0x100) != 0)
        return 0x20;
    if (p->unk194 < 4)
        return 0x20;
    if (ctx[0] == 1) {
        id = p->netId;
    } else {
        if (ctx[0] != 2)
            return 0;
        if (ctx[0x87] == 0)
            return 0;
        id = ((AIPER *)ctx[0x87])->netId;
    }
    n = amt_GetColumnOffices(5, 6, rec);
    for (i = 0; i < n; i++)
        who[i] = AI_505DF0(rec[i].id);
    if (n < 6)
        memset(&who[n], 0, (6 - n) * sizeof(who[0]));
    r = util_RandomU(6);
    i = 6;
    do {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < (float)util_RandomU(0x10) + 16.0f) {
            o = who[r];
            if (o == 0)
                return 0;
            if (*(int *)((unsigned char *)p + 0x5c) == o->netId)
                return 0;
            m[0] = 7;
            *(int *)(m + 4) = o->netId;
            return 1;
        }
        r = (int)(r + 1) % 6;
    } while (--i);
    return 0;
}

/* the callbacks above as 0x4a9a00 calls them */
#define G10_CB(fn) ((int (*)(int, AIPER *, int *, unsigned char *, unsigned char *))(fn))

/* 0x4a9a00, decide callback of the leisure actions: picks one of the actions of the table 0x600e10 that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback */
int ai_Unk4a9a00(int a, AIPER *p, int c, unsigned char *in, unsigned char *m)
{
    int ctx[0x88];
    int l1[19], l2[19];
    int n1, n2, k, kind;
    int *t;
    AIJOB *j;

    memset(ctx, 0, sizeof(ctx));
    n1 = 0;
    n2 = 0;
    ctx[0x87] = 0;
    if ((p->flags1c8 & 0x100) != 0) {
        for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
            if (j->unkac == p->netId) {
                ctx[0x87] = (int)gm_FindChar(j->unkb0);
                break;
            }
        }
    }
    if (in[0] != 0) {
        if (in[0] != 0x10)
            return 0;
        k = *(int *)(in + 4);
        ctx[0] = 1;
        if ((ord_DispatchOrder(k, p, ctx) & 2) == 0) {
            ctx[0] = 2;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
            *(int *)(in + 8) = 4;
        } else {
            *(int *)(in + 8) = 3;
        }
    } else {
        ctx[0] = 1;
        for (t = ai_Unk600e10; t < &ai_Unk600e10[19]; t++) {
            if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                l1[n1++] = *t;
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (t = ai_Unk600e10; t < &ai_Unk600e10[19]; t++) {
                if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                    l2[n2++] = *t;
            }
        }
        if (n1 == 0 && n2 == 0)
            return 0;
        if (n1 == 0 || (n2 != 0 && util_RandomU(2) != 0)) {
            ctx[0] = 2;
            k = l2[util_RandomU(n2)];
            kind = 4;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        } else {
            ctx[0] = 1;
            kind = 3;
            k = l1[util_RandomU(n1)];
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        }
        in[0] = 0x10;
        *(int *)(in + 4) = k;
        *(int *)(in + 8) = kind;
    }
    switch (k) {
    case 0x12:
        return G10_CB(ai_Unk4a8760)(a, p, ctx, in, m);
    case 0x13:
    case 0x14:
        return G10_CB(ai_Unk4a8a50)(a, p, ctx, in, m);
    default:
        return 0;
    case 0x1c:
        return G10_CB(ai_Unk4a8ad0)(a, p, ctx, in, m);
    case 0x1d:
        return G10_CB(ai_Unk4a8bf0)(a, p, ctx, in, m);
    case 0x21:
        return G10_CB(ai_AlwaysTrue)(a, p, ctx, in, m);
    case 0x24:
        return G10_CB(ai_Unk4a8c20)(a, p, ctx, in, m);
    case 0x25:
        return G10_CB(ai_Unk4a8c80)(a, p, ctx, in, m);
    case 0x27:
        return G10_CB(ai_Unk4a8d20)(a, p, ctx, in, m);
    case 0x28:
        return G10_CB(ai_Unk4a8db0)(a, p, ctx, in, m);
    case 0x29:
        return G10_CB(ai_Unk4a8ee0)(a, p, ctx, in, m);
    case 0x2a:
        return G10_CB(ai_Unk4a8f20)(a, p, ctx, in, m);
    case 0x2b:
        return G10_CB(ai_Unk4a9030)(a, p, ctx, in, m);
    case 0x2c:
        return G10_CB(ai_Unk4a9080)(a, p, ctx, in, m);
    case 0x2d:
        return G10_CB(ai_Unk4a9150)(a, p, ctx, in, m);
    case 0x31:
        return G10_CB(ai_Unk4a95d0)(a, p, ctx, in, m);
    case 0x32:
        return G10_CB(ai_Unk4a9660)(a, p, ctx, in, m);
    case 0x35:
        return G10_CB(ai_Unk4a9700)(a, p, ctx, in, m);
    case 0x37:
        return G10_CB(ai_Unk4a9880)(a, p, ctx, in, m);
    }
}
/* 0x4a9fa0, check callback of the leisure actions: whether p may still do the action in (kind 0x10) with the
   targets of out (0x4f81c0 without flags 0x60) */

int ai_Unk4a9fa0(AIPER *p, unsigned char *in, unsigned char *out)
{
    int ctx[0x88];
    AIJOB *j;

    if (in[0] == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && *(int *)(in + 8) == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->unkac == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->unkb0);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = *(int *)(in + 8);
        switch (*(int *)(in + 4)) {
        case 0x12:
        case 0x2c:
        case 0x31:
        case 0x32:
        case 0x35:
        case 0x37:
            ctx[0x85] = *(int *)(out + 4);
            break;
        case 0x13:
        case 0x14:
            ctx[1] = *(int *)(out + 4);
            ctx[2] = *(int *)(out + 8);
            break;
        default:
            return 0;
        case 0x1c:
            ctx[0x85] = *(int *)(out + 4);
            ctx[1] = *(int *)(out + 8);
            break;
        case 0x1d:
        case 0x21:
        case 0x24:
        case 0x29:
        case 0x2d:
            break;
        case 0x25:
        case 0x28:
        case 0x2a:
            ctx[0x85] = *(int *)(out + 4);
            ctx[0x86] = *(int *)(out + 8);
            break;
        case 0x27:
            ctx[0x85] = *(int *)(out + 4);
            break;
        case 0x2b:
            ctx[1] = *(int *)(in + 0x10);
            break;
        }
        return (ord_DispatchOrder(*(int *)(in + 4), p, ctx) & 0x60) == 0;
    }
    return 0;
}
/* 0x4aa120, decide callback of the visit actions: as 0x4a9a00 with the actions of the table 0x600e5c that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback */

int ai_Unk4aa120(int a, AIPER *p, int c, unsigned char *in, unsigned char *m)
{
    int ctx[0x88];
    int l1[3], l2[3];
    int n1, n2, k, kind;
    int *t;
    AIJOB *j;

    memset(ctx, 0, sizeof(ctx));
    n1 = 0;
    n2 = 0;
    ctx[0x87] = 0;
    if ((p->flags1c8 & 0x100) != 0) {
        for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
            if (j->unkac == p->netId) {
                ctx[0x87] = (int)gm_FindChar(j->unkb0);
                break;
            }
        }
    }
    if (in[0] != 0) {
        if (in[0] != 0x10)
            return 0;
        k = *(int *)(in + 4);
        ctx[0] = 1;
        if ((ord_DispatchOrder(k, p, ctx) & 2) == 0) {
            ctx[0] = 2;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
            *(int *)(in + 8) = 4;
        } else {
            *(int *)(in + 8) = 3;
        }
    } else {
        ctx[0] = 1;
        for (t = ai_Unk600e5c; t < &ai_Unk600e5c[3]; t++) {
            if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                l1[n1++] = *t;
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (t = ai_Unk600e5c; t < &ai_Unk600e5c[3]; t++) {
                if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                    l2[n2++] = *t;
            }
        }
        if (n1 == 0 && n2 == 0)
            return 0;
        if (n1 == 0 || (n2 != 0 && util_RandomU(2) != 0)) {
            ctx[0] = 2;
            k = l2[util_RandomU(n2)];
            kind = 4;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        } else {
            ctx[0] = 1;
            kind = 3;
            k = l1[util_RandomU(n1)];
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        }
        in[0] = 0x10;
        *(int *)(in + 4) = k;
        *(int *)(in + 8) = kind;
    }
    switch (k) {
    case 0:
        return G10_CB(ai_Unk4a91a0)(a, p, ctx, in, m);
    default:
        return 0;
    case 3:
        return G10_CB(ai_Unk4a9280)(a, p, ctx, in, m);
    case 4:
        return G10_CB(ai_Unk4a9350)(a, p, ctx, in, m);
    case 7:
        return G10_CB(ai_Unk4a9420)(a, p, ctx, in, m);
    case 8:
        return G10_CB(ai_Unk4a94e0)(a, p, ctx, in, m);
    }
}
/* 0x4aa430, check callback of the visit actions (0x4aa120): as 0x4a9fa0; prints the action name into a debug text
   that nobody reads */

int ai_Unk4aa430(AIPER *p, unsigned char *in, unsigned char *out)
{
    char name[64];
    int ctx[0x88];
    char text[256];
    char *s;
    AIJOB *j;

    if (in[0] == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && *(int *)(in + 8) == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->unkac == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->unkb0);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = *(int *)(in + 8);
        switch (*(int *)(in + 4)) {
        case 0:
            s = "prva_WERBUNG";
            break;
        default:
            return 0;
        case 3:
            s = "prva_ANKLAGE_ERHEBEN";
            break;
        case 4:
            s = "prva_ERPRESSUNG";
            break;
        case 7:
            s = "prva_FURCHT_EINFLOESSEN";
            break;
        case 8:
            s = "prva_BELEIDIGUNG";
            break;
        }
        sprintf(name, s);
        ctx[0x85] = *(int *)(out + 4);
        sprintf(text, "NPC %s <%i> w\xe4hlt %s", (char *)p + 0x30, p->id, name);
        return (ord_DispatchOrder(*(int *)(in + 4), p, ctx) & 0x60) == 0;
    }
    return 0;
}
/* 0x4aa580, decide callback: p (unk194 >= 1) picks a character within 0..63 units that 0x553db0 / 0x570ea0
   accept, with fewer than 4 (0x553db0's count, by chance), into m (kind 7) */

int ai_Unk4aa580(AIPER *p, int c, unsigned char *m)
{
    int k;
    int flags;
    unsigned short id;

    k = 0;
    if (p->unk194 < 1)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_RandomU(0x40), &id) == 0)
        return 0;
    if (he_Unk553db0(p, &((AIPER *)ai_CharsT)[id], &k) == 0)
        return 0;
    if (he_Unk570ea0(&((AIPER *)ai_CharsT)[id], p) == 0)
        return 0;
    if (k > 3)
        return 0;
    if (util_RandomF() < k * 0.25f)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    return 1;
}
/* 0x4aa6a0, decide callback: p (unk194 > 1) picks a random acquaintance within 20..80 units that 0x553d10
   accepts and it knows little (fewer than 2..5), into m (kind 7, its id at 0x10); in gets a count and a kind */

int ai_Unk4aa6a0(AIPER *p, unsigned char *in, unsigned char *m)
{
    int cnt[32];
    int ids[32];
    int n, j;
    unsigned int i;
    AIPER *o;

    if (p->unk194 > 1 && (n = unk_Unk4f3c50(ids, cnt, p->id)) != 0) {
        i = util_RandomU(n);
        j = n;
        do {
            o = AI_505DF0(ids[i]);
            if (o == 0)
                return 0;
            if (he_Unk553d10(p, o) == 0)
                return 0;
            if (gm_Liking(p->id, o->id, 1) < (double)util_RandomU(0x3d) + 20.0
                && cnt[i] < util_RandomU(4) + 2) {
                m[0] = 7;
                *(int *)(m + 4) = o->netId;
                *(unsigned int *)(m + 0x10) = o->id;
                *(unsigned int *)(in + 8) = util_RandomU(cnt[i]) + 1;
                *(unsigned int *)(in + 0x10) = util_RandomU(3);
                return 1;
            }
            i = (int)(i + 1) % n;
        } while (--j);
    }
    return 0;
}
/* 0x4aa800, decide callback: p (unk194 >= 3) picks a character within 33..48 units and a second one 50..100 units
   from both, by chance after 0x504e20, into m (kind 7) */

int ai_Unk4aa800(AIPER *p, int c, unsigned char *m)
{
    unsigned short ids[4];
    unsigned short id, id2;
    float f;

    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    if (p->unk194 < 3)
        return 0;
    ids[0] = p->id;
    if (ai_Unk4a5a30(p, ((unsigned)&ai_Unka69140 + 0x4973c0), 4, ids, 0.0f, (float)util_RandomU(0x10) + 33.0f, &id) == 0)
        return 0;
    ids[1] = id;
    if (ai_Unk4a5a30(p, ((unsigned)&ai_Unka69140 + 0x4973c0), 4, ids, 50.0f, 100.0f, &id2) == 0)
        return 0;
    m[0] = 7;
    *(int *)(m + 4) = ((AIPER *)ai_CharsT)[id].netId;
    f = gm_Mix(p, &((AIPER *)ai_CharsT)[id], 4);
    if (util_RandomF() + f < 1.0f)
        return 0;
    *(int *)(m + 8) = ((AIPER *)ai_CharsT)[id2].netId;
    return 1;
}
/* 0x4aa940, execute callback "aip_ExecPamphlet": p spreads a pamphlet about the character of m (message 0x35) */

int ai_Unk4aa940(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;
    AIPER *o;

    if (m[0] != 7)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    gm_BuildQuery(1, 5, 10);
    *(AITIME *)&msg.arg[0] = ai_TimeT;
    msg.to = p->netId;
    msg.type = 0x35;
    msg.flags = 2;
    msg.fromId = msg.to;
    ((unsigned char *)&msg)[0x6d] = (unsigned char)util_RandomU(0x1d);
    msg.arg[4] = o->netId;
    cm_BeginGroup("aip_ExecPamphlet");
    cm_SendLongMessage(&msg);
    cm_EndGroup("aip_ExecPamphlet");
    return 1;
}
/* 0x4aaa10, execute callback "aip_ExecDrohen": p threatens the character of m (message 0x2e, by chance after
   0x504e20) */

int ai_Unk4aaa10(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;
    AIPER *o;

    if (m[0] != 7)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
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

int ai_Unk4aaae0(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;

    msg.to = *(int *)(m + 8);
    msg.arg[0] = *(int *)(m + 4);
    msg.arg[1] = p->netId;
    msg.time = ai_TimeT;
    msg.type = 0x2f;
    msg.fromId = -1;
    msg.flags = 1;
    msg.arg[2] = msg.to;
    cm_BeginGroup("aip_ExecAnschwaerzen");
    cm_SendLongMessage(&msg);
    cm_SendMoneyDelta(p->netId, -3);
    cm_EndGroup("aip_ExecAnschwaerzen");
    return 1;
}
/* 0x4aab90, which of b and c character p trusts more (distance 0.66, 0x504ec0 0.34, item 0x2be +0.2): 1 b, 0 c,
   2 neither; *k 2 if the difference is small, else 1 (the middle part is a rough draft) */

int ai_TrustedOfTwo(AIPER *p, AIPER *b, AIPER *c, int *k)
{
    float db, wb, dc, wc, x1, x2, fb, fc;

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
    fb = wb;
    if (ai_InventoryHas(b, 0x2be) != 0)
        fb += 0.2f;
    fc = wc;
    if (ai_InventoryHas(c, 0x2be) != 0)
        fc += 0.2f;
    if (abs((int)(fb - fc)) * 0.66 <= abs((int)(x1 - x2)) * 0.33)
        *k = 2;
    else
        *k = 1;
    if (x2 < x1 - 5.0f)
        return 1;
    if (x1 < x2 - 5.0f)
        return 0;
    return 2;
}
/* 0x4aad30, of the four candidates ids the one character p trusts most for district k (distance + 0x4e60b0, item
   0x2be +0.2, flag 0x400000 -0.2); a tie is broken at random; -1 if none */

int ai_TrustedOfFour(int k, AIPER *p, int *ids, int *out)
{
    float sc[4];
    AIPER *c[4];
    G10DIST info;
    float f, best;
    int tie, bi, i;
    unsigned int r;

    *out = 0;
    if (amt_GetOfficeI(k, &info) == 0)
        return -1;
    for (i = 0; i < 4; i++) {
        c[i] = AI_505DF0(ids[i]);
        if (c[i] == 0 || amt_GetOfficeRate(c[i], (unsigned char)info.lvl, &f) == 0) {
            sc[i] = -1000.0f;
        } else {
            sc[i] = gm_Liking(c[i]->id, p->id, 1) + f;
            if (ai_InventoryHas(c[i], 0x2be) != 0)
                sc[i] += 0.2f;
            if ((c[i]->flags1cc & 0x400000) != 0)
                sc[i] -= 0.2f;
        }
    }
    best = -10.0f;
    tie = 0;
    bi = -1;
    for (i = 0; i < 4; i++) {
        if (c[i] != 0) {
            if (sc[i] > best) {
                best = sc[i];
                tie = 0;
                bi = i;
            } else if (c[i] != 0 && best == sc[i]) {
                tie = 1;
            }
        }
    }
    if (bi < 0)
        return -1;
    if (!tie)
        return c[bi]->netId;
    r = util_RandomU(4);
    for (i = 4; i != 0; i--) {
        if (sc[r] == best && c[r] != 0) {
            if ((int)r >= 0)
                return c[r]->netId;
            return -1;
        }
        r = (r + 1) % 4;
    }
    return -1;
}
/* 0x4aaef0, decide callback: a building of p (type with cap > 1) with a free workplace object (lists 0x120 / 300)
   and no job 0x74 yet, into a (kind 4) and b (kind 1) */

int ai_Unk4aaef0(AIPER *p, unsigned char *a, unsigned char *b)
{
    AIBLD *bld;
    AIOBJ *o, *o2;

    for (bld = gm_BuildQuery(2, 4, p->id, 5, 0x16); bld != 0; bld = gm_BuildNext()) {
        if (ai_BTypesT[bld->type].cap247 > 1 && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
            && (o2 = gm_Query(o->list, 1, 0, 300)) != 0 && he_FindEvent(2, 0, 0x74, 3, bld->id) == 0) {
            a[0] = 4;
            *(int *)(a + 4) = bld->id;
            b[0] = 1;
            *(int *)(b + 4) = o->objId;
            return 1;
        }
    }
    return 0;
}
/* 0x4aafb0, execute callback: message 0x74 from p to o about the object at m */

int ai_Unk4aafb0(AIPER *p, AIPER *o, unsigned char *m)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x74;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = o->netId;
    msg.time = ai_TimeT;
    msg.arg[1] = *(int *)(m + 4);
    msg.arg[0] = msg.fromId;
    cm_SendLongMessage(&msg);
    gm_SendMoneyDelta(p->netId, -1);
    return 1;
}
/* 0x4ab080, decide callback: at certain hours p (not flagged 0x2000000) picks one of the goods of another
   owner's shop (type cap > 1, objects 0x120 / 300) that costs at most 5 percent of its money, into a (kind
   0x13: item, price) and b (kind 4: the building); half of the time it only looks (a and b cleared) */

int ai_Unk4ab080(AIPER *p, int *a, int *b)
{
    int list[16];
    G10DIST info;
    int money, n, tries, item;
    int price, id2, x;
    unsigned int r, k;
    AIBLD *bld;
    AIOBJ *o;
    int i;

    if ((p->flags1cc & 0x2000000) == 0 && ai_TimeT.hour > 0x14
        && ai_TimeT.hour % 3 == (unsigned int)p->netId % 3) {
        money = gm_GetGoodsValue((AICHAR *)p);
        memset(list, 0, sizeof(list));
        n = 0;
        bld = gm_BuildQuery(1, 5, 0x16);
        if (bld != 0) {
            do {
                if (bld->owner != 0xffff && bld->owner != p->id && ai_BTypesT[bld->type].cap247 > 1
                    && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
                    && (o = gm_Query(o->list, 1, 0, 300)) != 0) {
                    list[n * 2] = (int)bld;
                    list[n * 2 + 1] = (int)o;
                    n++;
                }
                bld = gm_BuildNext();
            } while (bld != 0);
            if (n != 0) {
                r = util_RandomU(n);
                k = util_RandomU(3);
                bld = (AIBLD *)list[r * 2];
                o = (AIOBJ *)list[r * 2 + 1];
                tries = 3;
                do {
                    item = *(int *)((unsigned char *)o + 0x1c + k * 4);
                    if (unk_Unk4f4d30(item, &((AIPER *)ai_CharsT)[bld->owner], &price, &id2, &x) != 0
                        && (float)price <= money * 0.05f) {
                        amt_GetKind(((AIPER *)ai_CharsT)[(unsigned short)id2].unk166, &info);
                        if (info.lvl < util_RandomU(12))
                            return 0;
                        if (util_RandomF() >= 0.5) {
                            *(unsigned char *)a = 0x13;
                            a[1] = item;
                            a[4] = price;
                            *(unsigned char *)b = 4;
                            b[1] = bld->id;
                            return 1;
                        }
                        for (i = 6; i != 0; i--)
                            *a++ = 0;
                        for (i = 6; i != 0; i--)
                            *b++ = 0;
                        return 1;
                    }
                    k = (int)(k + 1) % 3;
                } while (--tries);
            }
        }
    }
    return 0;
}
/* 0x4ab2c0, execute callback "buy dunkle ecke": p buys item a[1] for a[4] in the shop b if it still sells it */

int ai_Unk4ab2c0(AIPER *p, int *a, int *b)
{
    int rec[5];
    AIBLD *bld;
    AIOBJ *o;
    int *q;
    int i;

    bld = gm_BuildQuery(1, 1, b[1]);
    if (bld == 0)
        return 0;
    o = gm_Query(bld->objList, 1, 0, 0x120);
    if (o == 0)
        return 0;
    o = gm_Query(o->list, 1, 0, 300);
    if (o == 0)
        return 0;
    i = 0;
    q = (int *)((unsigned char *)o + 0x1c);
    rec[3] = a[1];
    do {
        if (*q == rec[3])
            break;
        i++;
        q++;
    } while (i < 3);
    if (i == 3)
        return 0;
    rec[0] = 0x62757920;
    rec[2] = bld->id;
    rec[1] = p->netId;
    rec[4] = a[4];
    cm_BeginGroup("buy dunkle ecke");
    ai_Unk4c5d40(rec);
    cm_SendProofExecuted(a[1], p->netId);
    cm_SendMoveGoodsUnchecked(((AIPER *)ai_CharsT)[bld->owner].netId, p->netId, 0, a[4]);
    cm_EndGroup("buy dunkle ecke");
    return 1;
}
/* 0x4ab3e0, decide callback of action 0x44 "Blutrache": p (unk194 >= 5, not flagged 8, not of kind 3) with its
   best building of kind 0x1f holding object 0x27c picks a victim within 25..57 units and up to four helpers
   (0x4a61d0) it can afford by their weight (0x4e7c40 / 3 + 2), into m / m2 (kind 7) */

int ai_Unk4ab3e0(int a, AIPER *p, int c, unsigned char *m, unsigned char *m2)
{
    unsigned short ids[4];
    AIPER *who[4];
    int v, i, n, cnt, s, t;
    int bestv, k;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    best = 0;
    bestv = 0;
    who[0] = 0;
    who[1] = 0;
    who[2] = 0;
    who[3] = 0;
    if ((p->flags1cc & 8) != 0 || p->unk194 < 5)
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
        b = (AIBLD *)((unsigned char *)b + 0xa9);
    }
    if (best == 0 || bestv == 0)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (G10_4A58D0(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
        return 0;
    o = &((AIPER *)ai_CharsT)[id];
    s = amt_GetKindLevel(o->unk166) / 3 + 2;
    if (s + 2 > p->unk194)
        return 0;
    n = ai_Unk4a61d0(p, 4, flags, 25.0f, 75.0f, bestv, ids);
    if (n == 0)
        return 0;
    cnt = 0;
    for (i = 0; i < n; i++) {
        t = amt_GetKindLevel(((AIPER *)ai_CharsT)[ids[i]].unk166) / 3 + 2 + s;
        if (t >= p->unk194) {
            s = t;
            who[cnt++] = &((AIPER *)ai_CharsT)[ids[i]];
        }
    }
    if (cnt == 0)
        return 0;
    *(int *)(m + 4) = o->netId;
    *(int *)(m + 0xc) = who[0]->netId;
    if (cnt > 1)
        *(int *)(m + 8) = who[1]->netId;
    if (cnt > 2)
        *(int *)(m2 + 0xc) = who[2]->netId;
    if (cnt > 3)
        *(int *)(m2 + 8) = who[3]->netId;
    *(int *)(m + 0x10) = s;
    m[0] = 7;
    return 0x44;
}
/* 0x4ab6b0, execute callback of action 0x44 "Blutrache": p pays the cost, is flagged (0x1cc bit 3), the victim
   loses standing with every helper by their relation (0x504e20), and 0x5a9470 starts the feud */

int ai_Unk4ab6b0(AIPER *p, unsigned char *m, unsigned char *m2)
{
    AIPER *who[4];
    AIPER *o;
    int cost, n, i;

    cost = *(int *)(m + 0x10);
    if (m2 == 0 || p == 0)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    who[0] = AI_505DF0(*(int *)(m + 0xc));
    if (who[0] == 0)
        return 0;
    n = 1;
    if ((who[n] = AI_505DF0(*(int *)(m + 8))) == 0)
        n++;
    if ((who[n] = AI_505DF0(*(int *)(m2 + 0xc))) == 0)
        n++;
    if ((who[n] = AI_505DF0(*(int *)(m2 + 8))) == 0)
        n++;
    gm_SendMoneyDelta(p->netId, -cost);
    cm_SendSetField(p->netId, 0x1cc, 4, 8, 0);
    for (i = 0; i < n; i++)
        cm_SendRelations(o->id, who[i]->id, -(50 - (int)(gm_Mix(p, who[i], 4) * -50.0f)), 1);
    ai_GypsyDeliveryText(o, n, who);
    return 0x44;
}
/* 0x4ab7f0, decide callback of action 0x45 "Verfluchen": p (not flagged 0x10, not of kind 3) with its best
   building of kind 0x1f holding object 0x28a curses a character within 25..57 units with a strength it can
   afford (32000 per level from a quarter of its money), into m (kind 7) */

int ai_Unk4ab7f0(int a, AIPER *p, int c, unsigned char *m)
{
    int v, i, k, r, money;
    int bestv, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
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
        b = (AIBLD *)((unsigned char *)b + 0xa9);
    }
    if (best == 0 || bestv == 0)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (G10_4A58D0(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
        return 0;
    o = &((AIPER *)ai_CharsT)[id];
    money = (int)(gm_Unk509e30(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer) * 0.25f);
    gm_GetWorth(p->id);
    for (k = bestv - 1; k > 0; k--) {
        if (money > (k > 1 ? k : 1) * 32000)
            break;
    }
    if (k == 0)
        return 0;
    r = util_RandomU(k + 1);
    *(int *)(m + 4) = o->netId;
    k = r > 1 ? r : 1;
    *(int *)(m + 0xc) = r;
    *(int *)(m + 0x10) = k * 32;
    m[0] = 7;
    return 0x45;
}
/* 0x4ab9f0, execute callback of action 0x45 "Verfluchen": p is flagged (0x1cc bit 4), pays, and the curse goes
   out (message 0x9d: strength, victim) */

int ai_Unk4ab9f0(AIPER *p, unsigned char *m)
{
    AIMSG msg;

    cm_SendSetField(p->netId, 0x1cc, 4, 0x10, 0);
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, -*(int *)(m + 0x10));
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.time = ai_TimeT;
    msg.type = 0x9d;
    msg.fromId = -1;
    msg.flags = 2;
    msg.arg[0] = *(int *)(m + 0xc);
    msg.arg[1] = *(int *)(m + 4);
    cm_SendLongMessage(&msg);
    return 0x45;
}
/* 0x4abac0, decide callback of action 0x46 "Theater": p (unk194 > 1, not of kind 3) picks its theatre (a building
   of kind 0x1f holding object 0x27f, not busy by 0x5741d0; the bigger one, cap 2: 7, else 4) and a play length
   by its rank, into m (kind 7) */

int ai_Unk4abac0(int a, AIPER *p, int c, unsigned char *m)
{
    int v, k, n, i;
    unsigned int r;
    AIBLD *b, *best;
    AIBTYPE *bt;

    v = gm_Unk5044b0(p->id, 0xe);
    k = 0;
    if ((a == 0x46 || a == 0) && c == 0 && v != 0 && p->kind != 3 && p->unk194 > 1) {
        best = 0;
        b = ai_BldsT;
        for (i = 0x100; i != 0; i--) {
            if (b->type != 0 && b->owner == p->id) {
                bt = &ai_BTypesT[b->type];
                if (bt->kind == 0x1f && gm_Query(b->objList, 2, 6, 0, 0x27f) != 0 && ai_FindCurseEvent(b) == 0) {
                    if (best == 0) {
                        n = bt->cap247 != 2 ? 4 : 7;
                    } else {
                        n = bt->cap247 != 2 ? 4 : 7;
                        if (n <= k)
                            goto next;
                    }
                    k = n;
                    best = b;
                }
            }
next:
            b = (AIBLD *)((unsigned char *)b + 0xa9);
        }
        if (best != 0 && k != 0) {
            if (p->unk194 < 4) {
                if (p->unk194 < 3) {
                    if (k > 3)
                        k = 4;
                } else if (k > 6) {
                    k = 7;
                }
            } else if (k > 9) {
                k = 10;
            }
            r = util_RandomU(k);
            if (r < 4)
                k = 2;
            else
                k = (r > 6) + 3;
            *(int *)(m + 4) = best->id;
            *(unsigned int *)(m + 0xc) = r;
            *(int *)(m + 0x10) = k;
            m[0] = 7;
            return 0x46;
        }
    }
    return 0;
}
/* 0x4abc60, execute callback of action 0x46 "Theater": p pays and puts on the play in building m + 4
   (message 0xa0, length m + 0xc) */

int ai_Unk4abc60(AIPER *p, unsigned char *m)
{
    AIMSG msg;

    gm_SendMoneyDelta(p->netId, -*(int *)(m + 0x10));
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.fromId = *(int *)(m + 4);
    msg.time = ai_TimeT;
    msg.type = 0xa0;
    msg.flags = 2;
    msg.arg[0] = *(int *)(m + 0xc);
    cm_SendLongMessage(&msg);
    return 0x46;
}
/* 0x4abd00, decide callback of action 0x47 "Ehrerbietung": p (unk194 >= 2, not flagged 0x20, not of kind 3) with a
   building of kind 0x1f holding object 0x276 picks a character within 25..57 units to honour, into m (kind 7, a
   cost 2..7); returns 0x45 (sic) */

int ai_Unk4abd00(int a, AIPER *p, int c, unsigned char *m)
{
    int v, i, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
    flags = 0;
    if ((p->flags1cc & 0x20) != 0 || p->unk194 < 2)
        return 0;
    if ((a != 0x47 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0; i < 0x100; i++) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x276) != 0) {
            flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
            if (G10_4A58D0(p, 1, (int *)&flags, 0.0f, (float)util_RandomU(0x20) + 25.0f, &id) == 0)
                return 0;
            o = &((AIPER *)ai_CharsT)[id];
            n = util_RandomU(p->unk194 < 8 ? p->unk194 : 8);
            if (n <= 2)
                n = 2;
            *(int *)(m + 0x10) = n;
            *(int *)(m + 4) = o->netId;
            *(int *)(m + 0xc) = 0;
            m[0] = 7;
            return 0x45;
        }
        b = (AIBLD *)((unsigned char *)b + 0xa9);
    }
    return 0;
}
/* 0x4abe60, execute callback of action 0x47 "Ehrerbietung": p is flagged (0x1cc bit 5), pays m + 0x10 and wins
   five times that in standing with the character of m */

int ai_Unk4abe60(AIPER *p, unsigned char *m)
{
    AIPER *o;

    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    cm_SendSetField(p->netId, 0x1cc, 4, 0x20, 0);
    gm_SendMoneyDelta(p->netId, -*(int *)(m + 0x10));
    cm_SendRelations(p->id, o->id, *(int *)(m + 0x10) * 5, 1);
    return 0x47;
}
/* 0x4abed0, registers the actions 0x44 "Blutrache", 0x45 "Verfluchen", 0x46 "Theater" and 0x47 "Ehrerbietung" */

int ai_Unk4abed0(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x44;
    strcpy(act.name, "Blutrache");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4ab3e0;
    act.exec = ai_Unk4ab6b0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x45;
    strcpy(act.name, "Verfluchen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4ab7f0;
    act.exec = ai_Unk4ab9f0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x46;
    strcpy(act.name, "Theater");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4abac0;
    act.exec = ai_Unk4abc60;
    act.flags = 1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x47;
    strcpy(act.name, "Ehrerbietung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4abd00;
    act.exec = ai_Unk4abe60;
    act.flags = 3;
    return ai_Unk495520(&act, a) != 0;
}

G10GAME * ai_Unkf83654 = 0;  /* 0xf83654 */
G10CARD ai_Unkf83660[24] = {0};  /* 0xf83660 */
G10NPC ai_Unkf837e0[2] = {0};  /* 0xf837e0 */
int ai_Unk600e10[19] = { 18, 19, 20, 28, 29, 33, 36, 37, 39, 40, 41, 42, 43, 44, 45, 49, 50, 53, 55 };  /* 0x600e10, the leisure actions */
int ai_Unk600e5c[3] = { 0, 3, 7 };  /* 0x600e5c */
