/* ai.c -- the AI unit (0x47d9e0..0x48e070): the character update (needs / mood, 0x47dab0), the partner search
   and the per-frame loop over the 768 characters (0x47e560), the dispatcher that runs a master's evaluation by
   building kind (0x47e990), and the masters' evaluations: the production masters (0x47edb0, 0x47f700, 0x4801f0,
   0x483fc0, 0x484a60), ai_CalcMeisterWache (0x480e70), ai_CalcAngriff (0x4817a0), ai_CalcMeisterDiebe (0x481ef0),
   ai_CalcMeisterRaeuber (0x482f00), the bank master (0x4836a0), the entry / slot tables (0x4854f0 .. 0x487ea0),
   the purchases and the order dispatcher (0x487fb0 .. 0x489870), ai_MeisterBewaffnen (0x489a70),
   ai_MeisterCollectTrans (0x489cb0), ai_MeisterTrade / ai_MeisterTradeAllgemein (0x48a4d0, 0x48bc80) and
   ai_MeisterTradeFernEinkauf (0x48d010, 0x48d810). Module tag "ai_" (messages "ai_Calc...(): ...").
   TU evidence: the unit's .data starts at 0x5fef04 (0x5fef00 belongs to the blit code before it) and its string
   literals 0x5fef0c..0x5ffd18 follow the function order without a new data block; 0x5ffd1c starts the next obj.
   Names are ours except the ones taken from the strings.
   Open: ai_types.h's AIMSG / AIJOB declarations are shared with sv_t.c, whose tutorial builders flip their VC6
   tie-break when a member is added (debt ai_TutorialInitChapter2, ai_TutorialInitChapter3); ai_Unk483a60's slot match
   was lost the same way (debt, to AIMSG.payment).
   Open: AIOBJ.unk1c (a building id in the trade code, a count / level elsewhere) - its rename across units is undecided.
   unit of rea 8cef7d5b (src/game/ai.c); boundaries in decl/text.dcl */
extern int gm_PlayerRecords[688];
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int SOCKET;
#include "net.h"
#include "ai.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define he_MessageBoxQuickjumpI ((void (*)(int, int, char *, int, int, int, int, int, int))he_MessageBoxQuickjump)  /* 0x53dee0 (he.c: char fl): @original this unit builds the flag as an int (byte-matched ai_MsgTakeBuildingGoodsPct5to19Type33 / ai_MsgTakeBuildingGoodsPct5to24Type34) */
#define gm_CharLevelS ((int (*)(unsigned short, int))gm_CharLevel)  /* 0x505e60 (f3gm.c: unsigned short id, so the cast is now the prototype itself): @original this unit pushes the id as 16 bits (byte-matched ai_MasterExamPassed / ai_EvalFest / ai_RelationGain) */
#define cm_SendRemoveGoodsI ((void (*)(int, int))cm_SendRemoveGoods)  /* 0x4c4960 (cm.h: short b): @original ai.c pushes the second argument as an int (byte-matched ai_SendRemoveEmptyGoods) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_ExecuteBuyRoom / ai_ExecuteBuyMarketObject) */
#include "d3_types.h"
#include "am_types.h"                /* AIACT */
#include "ahm.h"
#include "am.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "f3init.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "ls.h"
#include "rd.h"
#include "sv.h"
#include "tr.h"
#include "mission.h"
#include "sv_t.h"
#include "lc.h"
#include "ch.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */

/* 0x47d9e0, the AI module's init at the start of a game: resets the character update cycle (start index, last
   update time at day 0 06:00, count carry), loads the action table (returns 0 when that fails) and seeds the two
   weapon lists */

int ai_Init(void)
{
    ai_Update.start = 0;
    ai_Update.last.year = 0;
    gm_SetTime(&ai_Update.last, 6, 0, 0);
    ai_Update.carry = 0;
    if (!ai_LoadActionTable())
        return 0;
    ai_WeaponList[5] = 0;
    ai_WeaponList2[4] = 0;
    ai_WeaponList[1] = 0x154;
    ai_WeaponList[0] = 0x156;
    ai_WeaponList[2] = 0x158;
    ai_WeaponList[3] = 0x172;
    ai_WeaponList[4] = 0x16e;
    ai_WeaponList2[0] = 0x174;
    ai_WeaponList2[1] = 0x176;
    ai_WeaponList2[2] = 0x15e;
    ai_WeaponList2[3] = 0x160;
    return 1;
}
/* 0x47da80, updates the needs of every character in use */

void ai_UpdateNeeds(void)
{
    AICHAR *c;

    c = ai_CharsT;
    do {
        if (c->id != 0xffff)
            ai_GrowNeeds(c);
        c++;
    } while ((int)c < (int)(ai_CharsT + 768));
}
/* 0x47dab0, lets the 13 needs of a character grow (value = value + a - b * value, kept in 0..1000) and
   switches its current need to the strongest one */

void ai_GrowNeeds(AICHAR *c)
{
    float *p;
    union {
        float f;
        int i;
    } best;
    int i, bi;

    c->mood[13][2] = min(max(0.0, (c->mood[13][0] + c->mood[13][2]) - c->mood[13][1] * c->mood[13][2]), 1000.0);
    best.f = 0.0;
    bi = best.i;
    for (i = 0; i < 13; i++) {
        if (i != c->sel) {
            if (i == 1)
                c->mood[1][2] = 0;
            else if (i == 0) {
                if (c->money <= 6)
                    goto grow;
                c->mood[0][2] = 0;
            } else if (i == 2) {
                if (c->bld2 == 0 && c->kind != 3)
                    goto grow;
                c->mood[2][2] = 0;
            } else if (i == 3) {
                c->mood[3][2] = min(max(0.0, ((c->mood[3][0]) * c->mood3Rate + c->mood[3][2]) - c->mood[3][1] * c->mood[3][2]), 1000.0);
            } else {
grow:
                p = &c->mood[i][2];
                *p = min(max(0.0, (c->mood[i][0] + *p) - c->mood[i][1] * *p), 1000.0);
            }
        }
        if (i == c->sel) {
            if ((2.0 - c->mood[13][2] * 0.002) * c->mood[i][2] >= best.f) {
                bi = i;
                best.f = (2.0 - c->mood[13][2] * 0.002) * c->mood[i][2];
            }
        } else if (c->mood[i][2] >= best.f) {
            best.f = c->mood[i][2];
            bi = i;
        }
    }
    if (bi != c->sel) {
        p = &c->mood[c->sel][2];
        if (*p >= 1000.0f)
            *p = *p - 50.0f;
        c->sel = (unsigned char)bi;
        c->mood[13][2] = 0;
    }
}
/* 0x47dd00, the AI's work limit for a frame, from the smoothed game rating: rating * 0.006, clamped to 5..16
   (the callers compare the current load ai_WorkLoad against it) */

int ai_WorkLimit(void)
{
    int n;

    n = (int)(ai_Rating * 0.006);
    if (n < 5)
        return 5;
    if (n > 0x10)
        n = 0x10;
    return n;
}
/* 0x47dd30, the current AI work load: the number of players times the trade messages sent since the last
   reset, plus the open trade jobs (kind 0xb whose flags byte is still 0) */

int ai_WorkLoad(void)
{
    int n;
    AIJOB *j;

    n = 0;
    for (j = he_FindEvent(1, 0, 0xb); j; j = he_NextEvent()) {
        if (j->flags == 0)
            n++;
    }
    return ai_NumPlayers * ai_TradeMsgCount + n;
}
/* 0x47dd70, a character kind that works for a master (2..5) */

int ai_WorksForMaster(AICHAR *c)
{
    char k;

    k = c->kind;
    if (k != 2 && k != 5 && k != 4 && k != 3)
        return 0;
    return 1;
}
/* 0x47dd90, a master that this machine runs (multiplayer: ids are shared out by modulo) */

int ai_IsLocalMaster(AICHAR *c)
{
    if (!ai_WorksForMaster(c))
        return 0;
    if (ai_NumPlayers) {
        if (net_Conn.s != -1) {
            if (c->netId % ai_NumPlayers != ai_NetPlayer)
                return 0;
        } else if (c->netId % ai_NumPlayers != ai_CurrentPlayer)
            return 0;
    }
    return 1;
}
/* 0x47ddf0, looks for a partner (a character that suits this one) and sends the messages about it; returns
   the partner when the two have met enough, else 0
   @deep Read whole against the original 0x47ddf0..0x47e356 and fixed: the candidate filter (officeKind /
   secondOffice, not jobId / prevOffice), the feud relation sum computed before util_Random, the partner
   part as one exit return p with p = 0 on the no-partner paths (the original's xor edi,edi / mov eax,edi
   tails). Every call, constant (128.0, the level factor, 34.0, -0x32 - rnd(0x20), the difficulty clamp
   2d+0x23 / 2d+0xf), field (0x210 / 0x214..0x218) and branch matches. Left: (1) allocation in the
   candidate loop: the original keeps c->id in bx under the -1 constant ebx (or ebx,-1 at entry; the
   prover's Ins16(0xffffffff, id) push), the kind basis in ebp, the RelationMemory row offset in the dead
   parameter slot [esp+0x28] and spills gm_CharLevel(i) to [esp+0x1c]; ours keeps i in ebp, o in ebx, id
   and the row offset in frame slots (frame 0x1c vs 0x14, the slots hunks); (2) block layout: the original
   places the b216<=b215 block (AddFieldValues with ebx=1 pushes) and the relation>0 block after the reset
   block at the end, each with its own epilogue, where ours lays them inline and cross-jumps their common
   a=1 / AddFieldDelta(0x218) / SendFieldDeltas tail (calls: cm_AddFieldDelta ours x12 vs x13,
   cm_SendFieldDeltas x4 vs x5). (size: 429 vs 445 instructions, explained: 16 instructions: the original's
   two separate b216 / relation>0 tails (an AddFieldDelta + SendFieldDeltas + xor edi / mov eax,edi
   epilogue copy, ~14 instructions) that ours cross-jumps into one, and its spill of gm_CharLevel(i) /
   row-offset reloads in the loop) */

AICHAR *ai_FindPartner(AICHAR *c)
{
    AICHAR *o, *p;
    int i, best, d;
    float f, bestf;
    unsigned char a, z;
    unsigned short id;
    int m1;

    if (c->partner == -1) {
        best = -1;
        bestf = 128.0f;
        if (!(c->officeKind || c->secondOffice || c->kind == 5))
            return 0;
        for (i = 0; i < 768; i++) {
            o = &ai_CharsT[i];
            id = c->id;
            if (i != id && o->kind == 6 || o->kind == 7 || ai_WorksForMaster(o) && (o->officeKind || o->secondOffice)) {
                d = gm_CharLevelS(i, 0);
                d = d - gm_CharLevelS(id, 0);
                d = abs(d);
                f = (10.0f - (float)d * 0.5f) * 0.1f;
                if (ai_RelationMemory[i][c->id] * f < bestf && gm_Liking(i, c->id, 1) < 34.0f) {
                    bestf = ai_RelationMemory[i][c->id] * f;
                    best = i;
                }
            }
        }
        if (best == -1)
            return 0;
        d = ai_Relations[best][c->id] / 2 + ai_RelationMemory[best][c->id];
        if (d < -0x32 - util_RandomU(0x20)) {
            a = 0;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(1, 1, 0x215, &a);
            d = abs((int)bestf);
            a = (unsigned char)(d / 3 + 1);
            a = max(min(a, ai_Difficulty * 2 + 0x23), ai_Difficulty * 2 + 0xf);
            cm_AddFieldDelta(1, 1, 0x216, &a);
            cm_AddFieldDelta(4, 1, 0x210, &ai_CharsT[best].netId);
            cm_AddFieldDelta(1, 1, 0x214, &ai_RelationMemory[best][c->id]);
            cm_SendFieldDeltas();
            cm_SendRelations(ai_CharsT[best].netId, c->netId, 0, 2);
            ai_ShowFeudNews(c, &ai_CharsT[best]);
        }
        return 0;
    }
    p = gm_FindChar(c->partner);
    if ((c->officeKind || c->secondOffice || c->kind == 5) && p &&
        (p->secondOffice || p->officeKind || p->kind == 6 || p->kind == 7 || p->kind == 5)
        && c->b215 <= c->b216 + 5) {
        if (c->b216 <= c->b215) {
            /* the search has run its steps: it ends without a partner */
            a = 1;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldValues(1, 1, 0x215, &a);
            a = 1;
            cm_AddFieldDelta(1, 1, 0x218, &a);
            cm_SendFieldDeltas();
            p = 0;
        } else if (ai_RelationMemory[p->id][c->id] + c->relationBias > 0) {
            /* the partner remembers the character well: the search jumps to its last step */
            a = c->b216 + 1;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(1, 1, 0x215, &a);
            a = 1;
            cm_AddFieldDelta(1, 1, 0x218, &a);
            cm_SendFieldDeltas();
            p = 0;
        } else if (c->b215 <= 3) {
            /* one or two steps further; past the third the two have met */
            a = util_RandomU(2) + 1;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldValues(1, 1, 0x215, &a);
            if (c->b215 + a > 3)
                cm_AddFieldDelta(1, 1, 0x218, &c->b217);
            cm_SendFieldDeltas();
            if (c->b215 + a > 3)
                ai_ShowPartnerNews(c, p);
            else
                p = 0;
        }
    } else {
        /* no partner (any more): the search starts over */
        a = 0;
        z = 0;
        m1 = -1;
        cm_BeginFieldBlock(c, c->netId);
        cm_AddFieldDelta(1, 1, 0x215, &a);
        cm_AddFieldDelta(1, 1, 0x216, &a);
        cm_AddFieldDelta(4, 1, 0x210, &m1);
        cm_AddFieldDelta(1, 1, 0x214, &z);
        a = 1;
        cm_AddFieldDelta(1, 1, 0x218, &a);
        cm_SendFieldDeltas();
        if (p) {
            cm_SendRelations(c->partner, c->netId, 0x3c, 2);
            ai_ShowPartingNews(c, p);
        }
        p = 0;
    }
    return p;
}
/* 0x47e360, a master of this machine looks for a free workshop of its own that needs hiring, then lets its
   partner logic run
   @deep byte; readability left: Q1 no evidenced name (the role is a local master's per-frame scan of its own buildings that announces a master hire - its sprintf 'stellt Meister fuer %s ein' - then runs the partner logic; no string or text id names it), Q2 AICHAR.health (32 uses, truthy / > 0 tests only, no reader names it) and AICHAR.heldRounds (2 uses, 0x47e360 skips its post-partner action refresh and 0x4bb6c0's sender skips the character), Q3 0x7d5f82 = ai_Chars + 768 * 0x21c + 2 loop bound, still a literal (an extern would need an evidenced name for the one-past-the-end kind field). Named on the way: ai_FindPartner, ai_ChooseAction, ai_ExecuteAction, ai_FillNearRing, cm_SendMessage3d, ai_BuildingTypeMasks, ai_Partner, ai_ActionFlags, gm_FindChar, cm_AddFieldDelta, AICHAR.act / mask, AITIME.hour. gm_BuildingClass (0x501d80) is in no source and has no Ghidra export: read by disassembly, it maps ai_BTypes[type].kind 1..0x20 to the category 1..9 (jump table 0x501e14, default 0) - a role name (gm_BuildingClass) needs the defining unit, which is undrafted. */

void ai_MasterUpdate(AICHAR *c)
{
    AIBLD *b;
    AICHAR *p;
    AIBTYPE *bt;
    char buf[1024];
    unsigned int lo, hi;
    int n, found, v;
    unsigned int sel;

    if (ai_IsLocalMaster(c) && (c->flags1cc & 0x3000) == 0) {
        for (n = 0x100, b = ai_BldsT; n; n--, b++) {
            if (b->type) {
                lo = 0;
                hi = 0;
                bt = &ai_BTypesT[b->type];
                if ((unsigned char)bt->kind > 0x1f)
                    hi = 1 << ((unsigned char)bt->kind - 0x20);
                else
                    lo = 1 << (unsigned char)bt->kind;
                if (b->owner == c->id && !(b->flags5a & 1) && ((lo & ai_BuildingTypeMasks[2]) || (hi & ai_BuildingTypeMasks[3]))) {
                    found = 0;
                    p = ai_CharsT;
                    while (1) {
                        if ((int)&p->kind >= (unsigned)&gm_PlayerRecordBytes)
                            break;
                        if (p->id != 0xffff && p->bld == b && (p->kind == 1 || p->kind == 2))
                            found = 1;
                        p++;
                        if (found)
                            break;
                    }
                    if (gm_BuildingClass(b->type) != 6 && !found) {
                        v = b->type;
                        cm_SendMessage3d(b, 1, ai_BTypesT[v].title2, ai_BTypesT[v].jobBytes);
                        sprintf(buf, "%li:%02li - %s stellt Meister f\xfc" "r '%s' ein", ai_Time.hour,
                                ai_Time.min, c->name, b->name);
                    }
                }
            }
        }
        if (c->health > 0) {
            ai_Partner = ai_FindPartner(c);
            ai_FillNearRing(c);
            sel = c->act;
            if (!c->heldRounds) {
                if (ai_ChooseAction(c->id) && (c->act != sel || (ai_ActionFlags[c->act][0] & 0x200)))
                    ai_ExecuteAction(c->id);
            }
            ai_Partner = 0;
        }
    }
}
/* 0x47e560, the per-frame loop: runs the update of a share of the 768 characters (a count that follows from
   the game time passed since the last run) */

void ai_UpdateCharacters(void)
{
    AITIME now;
    AICHAR *c;
    int n, count, idx, whole, t;
    float f;
    char buf[1024];

    now = ai_Time;
    n = gm_MinutesBetween(&ai_Update.last, &now);
    f = n * 12.8f;
    whole = (int)f;
    ai_DifficultyValue = (float)ai_Difficulty;
    ai_DifficultyScale = ai_DifficultyValue * 0.0625f + 0.5f;
    if (n > 0 && ai_Time.hour <= 0x16 && ai_Time.hour >= 6) {
        if (ai_Update.last.year < now.year) {
            ai_Update.last = now;
            return;
        }
        if (n > 0x3c) {
            ai_Update.last.hour++;
            count = 0x300;
        } else {
            ai_Update.last = now;
            ai_Update.carry = (f - (float)whole) + ai_Update.carry;
            count = whole;
            if (ai_Update.carry >= 1.0f) {
                ai_Update.carry = ai_Update.carry - 1.0f;
                count = whole + 1;
            }
        }
        sprintf(buf, "Player count: %i, Starting with: %i Count: %i Deltatime: %i", count, ai_Update.start, count, n);
        idx = ai_Update.start;
        for (; count != 0; count--) {
            if (ai_CharsT[idx].id != 0xffff) {
                c = &ai_CharsT[idx];
                ai_UnloadCarts(c);
                if (ai_Time.hour > 6 && ai_Time.hour < 0x16 && !(ai_GameFlags & 0x80) &&
                    (c->kind == 5 || (((unsigned char)c->id ^ (unsigned char)ai_Time.hour) & 1) == 0))
                    ai_MasterUpdate(c);
                if ((ai_CanAct(c) && c->v024 != 0) || (c->flags1cc & 0x3000) != 0) {
                    whole = c->v024;
                    whole = -whole - (int)(whole * -0.89f);
                    cm_BeginFieldBlock(c, c->netId);
                    cm_AddFieldValues(4, 1, 0x24, &whole);
                    cm_SendFieldDeltas();
                }
            }
            idx = (idx + 1) % 0x300;
        }
        ai_Update.start = idx;
        if (idx == 0x300) {
            ai_Update.start = 0;
            ai_Update.last.hour++;
            ai_Update.last.min = 0;
            ai_Update.last.sec = 0;
            ai_Update.carry = 0.0f;
        }
        sprintf(buf, "Next Player: %i", ai_Update.start);
    }
}
/* 0x47e7f0, the character can act now (this machine runs it, and no flag 0x3000) */

int ai_CanAct(AICHAR *c)
{
    if ((net_Conn.s == -1 || c->kind == 6 || c->netId % ai_NumPlayers == ai_NetPlayer) && (c->flags1cc & 0x3000) == 0)
        return 1;
    return 0;
}
/* 0x47e840, whether this machine runs this worker (kinds 0 / 1): the building owner's net id is this
   machine's share of the players (multiplayer); a single player runs every worker */

int ai_IsLocalWorker(AICHAR *c)
{
    unsigned short id;
    AICHAR *o;

    if (c->kind != 0 && c->kind != 1)
        return 0;
    o = c;
    if (c->bld != 0 && (id = c->bld->owner) != 0xffff)
        o = &ai_CharsT[id];
    if (net_Conn.s != -1 && o->netId % ai_NumPlayers != ai_NetPlayer)
        return 0;
    return 1;
}
/* 0x47e8b0, whether this machine runs this character's job: kinds 1 / 2 with a job in an owned
   building whose owner is not kind 7, and (multiplayer) the owner's net id is this machine's share */

int ai_IsLocalJob(AICHAR *c)
{
    unsigned short id;

    if (c->kind != 1 && c->kind != 2)
        return 0;
    if (c->jobId == 0)
        return 0;
    if (c->bld == 0)
        return 0;
    id = c->bld->owner;
    if (id == 0xffff)
        return 0;
    if (ai_CharsT[id].kind == 7)
        return 0;
    if (net_Conn.s != -1 && ai_CharsT[id].kind != 6 && ai_CharsT[id].netId % ai_NumPlayers != ai_NetPlayer)
        return 0;
    return 1;
}
/* 0x47e940, resets the job state of a working character (kinds 1 / 2 with a job): clears the 16 bytes
   at 0x1b4 (the flags, budget, product and target) and sets the job flag bit 0 */

void ai_ResetJob(AICHAR *c)
{
    unsigned int *f;

    if ((c->kind == 1 || c->kind == 2) && c->jobId) {
        memset(&c->flags1b4, 0, 16);
        c->target = -1;
        c->flags1b4 |= 1;
    }
}
/* 0x47e990, runs the evaluation of a master: which function depends on the kind of its building @deep byte; readability left: Q1/Q2 the seven master-evaluation callees are undefined names - 0x47edb0 (the default "evaluates the production of a master's workshop"), 0x47f700 (the same for building kinds 0xe / 8 / 0x20), 0x4801f0 (the same for building category 2), 0x483fc0 (kind 0x1d), 0x484a60 (kind 0x1f), 0x483a60 (no role comment, kind 9), 0x4836a0 (the bank master's money balance) - the five production variants differ only by the AIENTRY flags their bodies test and the kind-to-profession table that would name them is in no source; 0x501d80 same (no source, no Ghidra export). Named on the way: 0x6f2308 -> ai_PlayerBuilding (ai.h), 0x6f230c -> ai_FallbackBuilding (the f3snd.c 2308-then-230c fallback), 0xf8a0c8 -> ai_LoggedMaster (ai.h "the master whose decisions are logged"). */

void ai_RunMasterEvaluation(AICHAR *c)
{
    char buf[256];
    AIWATCH *w;
    int t;
    char k;

    if (ai_Time.hour >= 6 && (ai_Time.hour != 6 || ai_Time.min >= 0x1e) &&
        ai_IsLocalJob(c) && (c->flags1b4 & 1)) {
        if (c == ai_LoggedMaster) {
            sprintf(buf, "Evaluation von Meister %s in %s beginnt...", ai_LoggedMaster->name,
                    ai_LoggedMaster->bld->name);
            sprintf(buf, "Aktuelles Budget: %li", ai_LoggedMaster->budget);
            sprintf(buf, "Tats\xe4" "chlich verf\xfc" "gbares Budget: %li",
                    gm_GetGoodsValue(&ai_CharsT[c->bld->owner]));
        }
        if (c->bld->owner == c->id)
            c->budget = gm_GetGoodsValue(c);
        if (c->bld == ai_PlayerBuildingT || c->bld == ai_FallbackBuildingT) {
            w = &ai_Watch;
            w->bld = c->bld;
            w->ch = c;
            w->owner = &ai_CharsT[c->bld->owner];
            w->budget = c->budget;
        }
        if (gm_BuildingClass(c->bld->type) != 1 && gm_BuildingClass(c->bld->type) != 4) {
            if (gm_BuildingClass(c->bld->type) == 2) {
                ai_EvaluateProductionCategory2(c);
                return;
            }
            k = ai_BTypesT[c->bld->type].kind;
            if (k == 0x13) {
                ai_CalcMeisterWache(c);
                return;
            }
            if (k == 4) {
                ai_CalcMeisterDiebe(c);
                return;
            }
            if (k == 0x10) {
                ai_CalcMeisterRaeuber(c);
                return;
            }
            if (k == 5) {
                ai_CalcMeisterBank(c);
                return;
            }
            if (k != 9)
                return;
            ai_Unk483a60(c);
            return;
        }
        k = ai_BTypesT[c->bld->type].kind;
        if (k != 0xe && k != 8 && k != 0x20) {
            if (k == 0x1d)
                ai_EvaluateProductionGraveyard(c);
            else if (k == 0x1f)
                ai_EvaluateProductionGypsies(c);
            else
                ai_EvaluateProduction(c);
        } else
            ai_EvaluateProductionFlags(c);
    }
}
/* 0x47ec20, the master's open slots: how much of an item to make */

void ai_PlanSlots(AICHAR *ch, AIOBJ *b)
{
    int i, n, m;
    float f;
    AISLOT *s;
    AIOBJ *x;
    AIOBJ *o;

    for (i = 0; i < ai_NSlots; i++) {
        s = &ai_Slots[i];
        if (!(s->flags & 0x20) && !(s->flags & 0x200) && s->pending == 0 && s->amount < gm_ListStock(b, s->item) / 4) {
            f = gm_GoodsPrice(s->item, ai_OwnPlayer) / gm_ItemPrice(s->item, 100);
            if (f < 0.8f) {
                n = s->capacity;
                s->bld = 0;
                m = (int)((1.0f - f) * n);
                s->make = m;
                if (m >= n)
                    m = n;
                s->make = m;
                x = gm_FindMarketObject(s->item);
                if (x && (o = gm_Query(x->list, 1, 0, s->item)) != 0) {
                    if (gm_ListAmount(x, o) - (int)(gm_ListStock(x, o->id) * (unsigned)o->cnt >> 2) >= 5)
                        s->bld = ai_MarketBuilding;
                }
            }
        }
    }
}
/* 0x47ed60, after a master's production choice: sends message 0x14 (cm_SendRemoveGoods) for every workshop
   entry that is not being produced (flags bit 8) and of which the workshop's store holds none (amount 0),
   so the clients drop the empty good from the stall's store (0x4c7d80); stall is the workshop's stall object */

void ai_SendRemoveEmptyGoods(AIOBJ *stall)
{
    int i;

    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].amount == 0 && !(ai_Entries[i].flags & 8))
            cm_SendRemoveGoodsI(stall->objId, ai_Entries[i].item);
    }
}
/* 0x47edb0, evaluates the production of a master's workshop @deep B3 far draft (1608/2272 bytes, 702
   instructions, 547 differing; code 720 / regs 76 / slots 42 / fill 9 hunks). Read against 0x47edb0 (Ghidra
   + disassembly), every region compared (the two hourly branch chains, the 32-permuted entry loops, the
   human-player block, the ai_Watch block, the job loop, the two AIINFO loops and the tail): calls,
   arguments, constants, offsets and conditions equal. spell.py leads tried: dup-tail 47ee06 fixed by the
   if/else-if rewrite of the human-player block (1618 -> 1608, kept); reload 47edb7 / 47ee6c / 47ef54 /
   47efae / 47f4bc and dup-tail 47f09a / 47f1a8 / 47f39d / 47f53f name no concrete source form (locals vs
   re-reads, arm duplication of long tails); push-pop 47f120 is mid-function, no guard to move. What is
   left is register allocation: the original keeps ch in ebx and reloads it, ours keeps it in ebp -
   match-deep's slotmap / listing / c2prio territory. Readability done (Q1..Q7 pass): callees
   ai_UpdateProduction / ai_UpdateStaff / ai_BuySlots / ai_BuyCapacity / ai_BuyUpgrade / ai_HasMaterials /
   ai_StartProduction / ai_FreeJobCharacters / ai_SendJobOrders / ai_DrawSettingGood / sv_MoveToGroup /
   gm_FreeCapacity, fields AIENTRY.capacity / amount, AIGOOD.batch, AIBLD.objList, AICHAR.health / title /
   model, AITASK.bldId */

void ai_EvaluateProduction(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, n1, n2, found;

    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    ai_UpdateProduction(ch, a, b);
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    found = 0;
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_GoodsT[e->item].batch && ai_HasMaterials(ch, e, 1)) {
                        ai_Entries[i].flags |= 8;
                        ai_StartProduction(ch, e);
                        ch->prod = (unsigned short)e->item;
                        goto done;
                    }
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].item == ch->prod) {
                e = &ai_Entries[i];
                if (e->capacity >= ai_GoodsT[e->item].batch && ai_HasMaterials(ch, e, 1)) {
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, e);
                    ch->prod = (unsigned short)e->item;
                    found = 1;
                }
                break;
            }
        }
        ch->flags1b4 &= ~0x100;
        if (found)
            goto done;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_GoodsT[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount) {
                        if (ai_HasMaterials(ch, e, 1)) {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_GoodsT[e->item].name);
                            goto done;
                        } else if (ai_HasMaterials(ch, e, 0)) {
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                    ai_GoodsT[e->item].name);
                            goto done;
                        }
                    }
                }
                break;
            }
        }
    }
    for (i = 0; i < 32; i++)
        info.perm[i] = i;
    i = 0;
    do {
        r = util_RandomU(0x20);
        t = info.perm[i];
        i++;
        info.perm[i - 1] = info.perm[r];
        info.perm[r] = t;
    } while (i < 32);
    for (i = 0; i < ai_NEntries; i++) {
        e = &ai_Entries[info.perm[i]];
        g = &ai_GoodsT[e->item];
        if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c && b->lvl1d >= e->need20
            && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
            && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount && ai_HasMaterials(ch, e, 1)) {
            k = info.perm[i];
            ai_Entries[k].flags |= 8;
            e = &ai_Entries[k];
            ai_StartProduction(ch, e);
            ch->prod = (unsigned short)e->item;
            sprintf(buf, "Produktion von %s starten", ai_GoodsT[e->item].name);
            goto done;
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        e = &ai_Entries[info.perm[i]];
        g = &ai_GoodsT[e->item];
        if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c && b->lvl1d >= e->need20
            && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
            && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount && ai_HasMaterials(ch, e, 0)) {
            e = &ai_Entries[info.perm[i]];
            ai_StartProduction(ch, e);
            ch->prod = (unsigned short)e->item;
            sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_GoodsT[e->item].name);
            goto done;
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
    if (ai_Watch.bld == ch->bld) {
        for (j = 0; ai_Watch.nslots < 4; j++) {
            if (ai_NEntries <= j)
                break;
            ai_Watch.ent[ai_Watch.nslots] = ai_Entries[j];
            for (k = 0; k < 4; k++) {
                if (ai_Entries[j].slot[k] >= 0) {
                    ai_Watch.slots[ai_Watch.cnt[ai_Watch.nslots] + ai_Watch.nslots * 4] =
                        ai_Slots[ai_Entries[j].slot[k]];
                    ai_Watch.cnt[ai_Watch.nslots]++;
                }
            }
            ai_Watch.nslots++;
        }
    }
    ai_FreeJobCharacters(ch, 4);
    n2 = 0;
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_CharsT[j].health && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10 && ai_CharsT[j].bld == ch->bld
            && ai_CharsT[j].title && ai_CharsT[j].model && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            n2 = info.n2 + 1;
            info.n2 = n2;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = n2;
    n1 = 0;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            n1++;
        if (ai_Entries[i].flags & 0x10)
            n1++;
        info.n1 = n1;
    }
    if (n2 <= info.n1)
        info.n1 = n2;
    if (n2 != 0 && info.n1 != 0) {
        info.c = 0;
        info.z2 = 0;
        info.z1 = 0;
        info.ch = ch;
        info.b = b;
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Entries[j].flags & 8) {
                info.type = 4;
                info.e = &ai_Entries[j];
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
/* 0x47f700, evaluates the production of a master's workshop, the variant with the 0x20 / 0x200 entry flags @deep read the whole function against the original 0x47f700..0x4801f4 (the production setup calls, the 8 / 13 o'clock and other-hour entry search, the drawn-setting block, the permutation, both random-pick loops, the flag clean-up, the watch copy, the job characters, the counts and the job orders); fixed: the entry fields read through ai_Entries[i] / ai_Entries[k] (capacity, amount, need1c, need20, flags) where the original indexes the array ([esi + ai_Entries+0x40] etc.) and only the item and the call arguments through e, the job-order loop tests 0x10 first (if (flags & 0x10) {0x20: type 0x14, else 0x200: type 0x15} else if (flags & 8): type 4, the original's test al,0x10 / je). 542 -> 337 differing, ours 791 vs 808 instructions. Left: cross-jumping and layout: ours merges the three ai_SendJobOrders calls of the job-order loop into one tail (type stored, then a shared push / call / watch check, 0x48006b..0x4800f0) where the original keeps three call sites and joins only at the watch copy (0x480162 / 0x48017b); the success block of the second random loop goes out of line after the function body in ours ('- jne 0x480075' vs '+ je 0x47fdc5'); tried info.type before info.e: identical. The loops keep i in a slot / ebp and ch reloaded from [esp+0x1c0] in the original (ebx reused as a temporary). (size: 791 vs 808 instructions, explained: the 17 missing instructions are the two extra ai_SendJobOrders call sites of the original (lea / push / mov / call / reload of ai_Watch.bld and ch->bld / compare: about 8 each) that VC6 cross-jumped into one in ours) */

void ai_EvaluateProductionFlags(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, n1, n2, found;

    found = 0;
    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    ai_UpdateProduction(ch, a, b);
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                        && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                        ai_Entries[i].flags |= 8;
                        ai_StartProduction(ch, &ai_Entries[i]);
                        ch->prod = (unsigned short)ai_Entries[i].item;
                        goto done;
                    }
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].item == ch->prod) {
                if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                    && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, &ai_Entries[i]);
                    ch->prod = (unsigned short)ai_Entries[i].item;
                    found = 1;
                }
                break;
            }
        }
        ch->flags1b4 &= ~0x100;
        if (found)
            goto done;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (ai_Entries[i].capacity >= ai_GoodsT[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[i].amount) {
                        if (!ai_HasMaterials(ch, e, 1)) {
                            if (ai_HasMaterials(ch, e, 0)) {
                                ai_StartProduction(ch, e);
                                ch->prod = (unsigned short)e->item;
                                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                        ai_GoodsT[e->item].name);
                                goto done;
                            }
                        } else {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_GoodsT[e->item].name);
                            goto done;
                        }
                    }
                }
                break;
            }
        }
    }
    for (i = 0; i < 32; i++)
        info.perm[i] = i;
    i = 0;
    do {
        r = util_RandomU(0x20);
        t = info.perm[i];
        i++;
        info.perm[i - 1] = info.perm[r];
        info.perm[r] = t;
    } while (i < 32);
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x20) && !(ai_Entries[k].flags & 0x200)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 1)) {
                k = info.perm[i];
                ai_Entries[k].flags |= 8;
                e = &ai_Entries[k];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x20) && !(ai_Entries[k].flags & 0x200)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 0)) {
                e = &ai_Entries[info.perm[i]];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
    if (0 < ai_NEntries) {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].flags & 8) {
                for (k = 0; k < 4; k++) {
                    if (ai_Slots[ai_Entries[i].slot[k]].amount < ai_GoodsT[ai_Entries[i].item].need[k])
                        ai_Entries[i].flags &= ~8;
                }
            }
        }
        for (i = 0; i < ai_NEntries; i++) {
            if ((ai_Entries[i].flags & 0x20) || (ai_Entries[i].flags & 0x200))
                ai_Entries[i].flags &= ~8;
        }
    }
    for (i = 0; i < ai_NSlots; i++) {
        if (((ai_Slots[i].flags & 0x20) || (ai_Slots[i].flags & 0x200)) && (ai_Slots[i].flags & 0x10)
            && ai_Slots[i].entry >= 0)
            ai_Entries[ai_Slots[i].entry].flags |= 0x10;
    }
    if (ai_Watch.bld == ch->bld) {
        for (j = 0; ai_Watch.nslots < 4; j++) {
            if (ai_NEntries <= j)
                break;
            ai_Watch.ent[ai_Watch.nslots] = ai_Entries[j];
            for (k = 0; k < 4; k++) {
                if (ai_Entries[j].slot[k] >= 0) {
                    ai_Watch.slots[ai_Watch.cnt[ai_Watch.nslots] + ai_Watch.nslots * 4] =
                        ai_Slots[ai_Entries[j].slot[k]];
                    ai_Watch.cnt[ai_Watch.nslots]++;
                }
            }
            ai_Watch.nslots++;
        }
    }
    ai_FreeJobCharacters(ch, 4);
    n2 = 0;
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_CharsT[j].health && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10 && ai_CharsT[j].bld == ch->bld
            && ai_CharsT[j].title && ai_CharsT[j].model && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id
            && (!ai_CharsT[j].job || (ai_CharsT[j].job->kind != 0x14 && ai_CharsT[j].job->kind != 0x15))) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            n2 = info.n2 + 1;
            info.n2 = n2;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = n2;
    n1 = 0;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            n1++;
        if (ai_Entries[i].flags & 0x10)
            n1++;
        info.n1 = n1;
    }
    if (n2 <= info.n1)
        info.n1 = n2;
    if (n2 == 0 || info.n1 == 0) {
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    info.c = 0;
    info.z2 = 0;
    info.z1 = 0;
    info.ch = ch;
    info.b = b;
    for (j = 0; j < ai_NEntries; j++) {
        e = &ai_Entries[j];
        if (e->flags & 0x10) {
            if (e->flags & 0x20) {
                info.e = e;
                info.type = 0x14;
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            } else if (e->flags & 0x200) {
                info.e = e;
                info.type = 0x15;
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        } else if (e->flags & 8) {
            info.e = e;
            info.type = 4;
            ai_SendJobOrders((AIINFO *)&info);
            if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                ai_Watch.entries[ai_Watch.nentries] = *info.e;
                ai_Watch.nentries++;
            }
        }
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
/* 0x4801f0, evaluates the production of a master's workshop of the building category 2 (the dispatcher
   0x47e990 comes here when gm_BuildingClass(type) == 2): buys slots / capacity / upgrades, starts the entries'
   production, sends the job orders and finishes with the trade (ai_MeisterCollectTrans / ai_MeisterTrade);
   the same production steps as ai_EvaluateProduction for this category's AIENTRY flags
   @deep B3 far draft (970/975 instructions, 447 differing; diffclass 708 code / 42 regs / 38 slots / 5 fill): every hunk read against build/ghidra/4801f0.c and tools/asmdiff - no missing / extra statement, constant, callee or field offset; the whole diff is the c/ch register swap (ours c=ebp ch=ebx, original c=ebx ch=ebp) with the [esp+0x10]/[0x1c]/[0x20]/[0x24] slot rotation and the reloads it causes, i.e. allocation / scheduling. spell.py: the reload (19x) and dup-tail (6x) entries all describe that same allocation; no meaning-class lead (wrong type / signedness / offset / width) exists to try one edit on, so the last-bytes pass leaves nothing to measure. Q1 named by role (0x47e990 dispatches here on gm_BuildingClass(type) == 2). Q2 gap: AIMSG.unk14 is only ever written 0 by the ai.c senders (hst.c -1) and has no reader that names it. */

void ai_EvaluateProductionCategory2(AICHAR *ch)
{
    AIOBJ *a, *b, *c, *o, *o2;
    AIJOB *job;
    AIGOOD *g;
    AIMAP *map;
    AIPLANT *plant;
    void *x;
    AIINFO info;
    AIMSG msg;
    int i, j, k, n, id, found, v71;
    int r, y, idx;

    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    c = gm_Query(ch->bld->objList, 1, 0, 0xfe);
    if (!c)
        return;
    ai_UpdateProduction(ch, a, b);
    for (i = 0; i < ai_NEntries; i++) {
        for (k = 0; k < 4; k++) {
            if (ai_Entries[i].slot[k] >= 0 && ai_Slots[ai_Entries[i].slot[k]].entry >= 0) {
                ai_Entries[ai_Slots[ai_Entries[i].slot[k]].entry].flags |= 4;
                for (j = 0; j < ai_NSlots; j++) {
                    if (ai_Entries[ai_Slots[ai_Entries[i].slot[k]].entry].item == ai_Slots[j].item)
                        ai_Slots[j].flags |= 4;
                }
            }
        }
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 10);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    o = a;
    if (!(ch->flags1c8 & 0x10) && ai_BTypesT[ch->bld->type].kind != 11) {
        for (o = gm_Query(a->list, 1, 4, 0x17); o; o = gm_QueryNext()) {
            if (o->count == 1) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 9;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.nIds = 0;
                msg.reserved14 = 0;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[0] = ch->bld->id;
                msg.arg[1] = a->objId;
                msg.arg[2] = c->objId;
                cm_SendLongMessage(&msg);
                break;
            }
        }
        ch->flags1c8 |= 0x10;
    }
    found = 0;
    job = he_FindEvent(2, 0, 9, 3, ch->bld->id);
    if (job) {
        if (ch->job != job)
            sv_MoveToGroup(ch->bld, 0, ch->id, job);
        found = 1;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6) {
        id = ai_DrawSettingGood(ch->bld);
        if (o) {
            for (i = 0; i < 32; i++) {
                if (ai_Entries[i].item == id) {
                    if (b->lvl1c < ai_Entries[i].need1c || b->lvl1d < ai_Entries[i].need20)
                        break;
                    if (ai_Entries[i].capacity < ai_GoodsT[ai_Entries[i].item].batch)
                        break;
                    x = gm_FindMarketObject(ai_Entries[i].item);
                    if (!x)
                        break;
                    r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
                    g = &ai_GoodsT[ai_Entries[i].item];
                    if (r < g->batch + ai_Entries[i].amount)
                        break;
                    if (g->type == 0x17) {
                        o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                        if (!o2 || o2->count < ai_GoodsT[ai_Entries[i].item].batch)
                            break;
                    } else if (!ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                        if (ai_HasMaterials(ch, &ai_Entries[i], 0)) {
                            ai_StartProduction(ch, &ai_Entries[i]);
                            goto check;
                        }
                        break;
                    }
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, &ai_Entries[i]);
                    goto check;
                }
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (!(ai_Entries[i].flags & 4) && b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20
            && ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
            && (x = gm_FindMarketObject(ai_Entries[i].item)) != 0) {
            r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
            g = &ai_GoodsT[ai_Entries[i].item];
            if (r >= g->batch + ai_Entries[i].amount) {
                if (g->type == 0x17) {
                    o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                    if (!o2 || o2->count < ai_GoodsT[ai_Entries[i].item].batch)
                        continue;
                } else if (!ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                    continue;
                }
                ai_Entries[i].flags |= 8;
                ai_StartProduction(ch, &ai_Entries[i]);
                break;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            break;
        if (!(ai_Entries[i].flags & 4) && b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20
            && ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
            && (x = gm_FindMarketObject(ai_Entries[i].item)) != 0) {
            r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
            g = &ai_GoodsT[ai_Entries[i].item];
            if (r >= g->batch + ai_Entries[i].amount) {
                if (g->type == 0x17) {
                    ai_Entries[i].flags |= 0x10;
                    break;
                }
                if (ai_HasMaterials(ch, &ai_Entries[i], 0)) {
                    ai_StartProduction(ch, &ai_Entries[i]);
                    break;
                }
            }
        }
    }
check:
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8) {
            g = &ai_GoodsT[ai_Entries[i].item];
            if (g->type == 0x17) {
                if (ai_BTypesT[ch->bld->type].kind == 11) {
                    ai_Entries[i].flags |= 0x10;
                } else {
                    o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                    if (!o2 || o2->count < ai_GoodsT[ai_Entries[i].item].batch)
                        ai_Entries[i].flags = ai_Entries[i].flags & ~8 | 0x10;
                }
            } else {
                for (k = 0; k < 4; k++) {
                    if (ai_Slots[ai_Entries[i].slot[k]].amount < g->need[k])
                        ai_Entries[i].flags &= ~8;
                }
            }
        }
    }
    ai_FreeJobCharacters(ch, 4);
    ai_FreeJobCharacters(ch, 8);
    ai_FreeJobCharacters(ch, 3);
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_CharsT[j].health > 0 && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10
            && ai_CharsT[j].bld == ch->bld && ai_CharsT[j].title && ai_CharsT[j].model
            && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id && (!ai_CharsT[j].job || ai_CharsT[j].job->kind != '(')
            && (!found || ch != &ai_CharsT[j])) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            info.n2++;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = info.n2;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            info.n1++;
        if ((ai_Entries[i].flags & 0x10) && ai_BTypesT[ch->bld->type].kind != 11)
            info.n1++;
    }
    if (info.n1 >= info.n2)
        info.n1 = info.n2;
    if (info.n2 && info.n1) {
        info.b = b;
        info.c = c;
        info.ch = ch;
        info.z2 = 0;
        info.z1 = 0;
        if (ai_BTypesT[ch->bld->type].kind == 11) {
            if (ai_Time.hour % 2 == 0) {
                ch->flags1b4 &= ~0x20;
            } else if (!(ch->flags1b4 & 0x20)) {
                map = ((AIMAPREF *)&ai_MapRefs)[init_BuildingSceneUniverse(ch->bld)].map;
                if (!map)
                    return;
                v71 = ch->bld->plants;
                for (j = 0; j < 8; j++) {
                    if (strstr(map->names[j], "PFLANZBAR"))
                        idx = j;
                }
                for (i = 0; i < ai_NEntries * 4; i++) {
                    r = util_RandomU(ai_NEntries);
                    if (ai_GoodsT[ai_Entries[r].item].type == 0x17)
                        break;
                }
                if (ai_GoodsT[ai_Entries[r].item].type == 0x17) {
                    plant = veg_FindType(ai_Entries[r].item);
                    n = 0;
                    for (y = 0; n < 4; y++) {
                        if (y >= map->w)
                            break;
                        for (i = 0; n < 4 && i < map->w; i++) {
                            if (map->cells[map->w * y + i] == idx && veg_Fits(v71, i, y, plant->size)) {
                                cm_SetPlant(ch->bld->id, i, y, plant->id, 0, -1);
                                n++;
                            }
                        }
                    }
                }
                ch->flags1b4 |= 0x20;
            }
        }
        for (i = 0; i < ai_NEntries; i++) {
            if ((ai_Entries[i].flags & 0x10) && ai_BTypesT[ch->bld->type].kind != 11) {
                info.type = 8;
                info.e = &ai_Entries[i];
                ai_SendJobOrders(&info);
            }
            if (ai_Entries[i].flags & 8) {
                if (ai_GoodsT[ai_Entries[i].item].type == 0x17) {
                    if (ai_BTypesT[ch->bld->type].kind != 11) {
                        info.type = 3;
                        info.e = &ai_Entries[i];
                        ai_SendJobOrders(&info);
                        if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                            ai_Watch.entries[ai_Watch.nentries] = *info.e;
                            ai_Watch.nentries++;
                        }
                    } else {
                        info.type = 0x28;
                        info.e = &ai_Entries[i];
                        ai_SendJobOrders(&info);
                        if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                            ai_Watch.entries[ai_Watch.nentries] = *info.e;
                            ai_Watch.nentries++;
                        }
                    }
                } else {
                    info.type = 4;
                    info.e = &ai_Entries[i];
                    ai_SendJobOrders(&info);
                    if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                        ai_Watch.entries[ai_Watch.nentries] = *info.e;
                        ai_Watch.nentries++;
                    }
                }
            }
        }
        ai_SendRemoveEmptyGoods(b);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
/* 0x480e70, the guard master: the workshop's slots, equipment and trade first (its workshop object of kind 0x2a), the
   staff, slots, capacity, upgrade and thieves; then, at most once in the hours past 6 whose (netId + hour) is a
   multiple of 3 (flags1b4 0x20 marks it, cleared outside them), it counts his idle guards (title, no job, standing
   in his building) and, when at least half are free, picks one of: an attack (ai_CalcAngriff), a patrol in the
   town (message 0x43 to the best-rated grid cell's building of kind 6 he likes best, up to four guards), a check of
   a workshop there (message 100, one guard) or a patrol of the surroundings (message 0x65 to one of the eight
   ai_AmbushSpots, up to six guards, at least two)
   @deep Read whole against the original 0x480e70..0x481794 and rewritten statement for statement (702 vs
   702 instructions): the dispatch, the four branches, every call, constant, string, message field (types
   0x43 / 100 / 0x65, flags, arg[] / ids[] slots, patrolRoll), the member searches and the -1 fills (rep
   stosd) match. Left, all allocation / layout: (1) the town-patrol member loop: the original keeps ch in
   ebx and 0xffff in ebp (cmp word [eax-0x165], bp), ours reloads ch from the parameter and compares the
   immediate (+0x4d2..+0x4ba, 15 + 3 lines); the workshop check holds ch in edi and guard in ebx where ours
   swaps them (+0x64e, +0x6f8); (2) the shared send tail: the original's town patrol (cmp [msg.arg+0x18],
   -1) jumps into the workshop check's je / cm_SendLongMessage / epilogue at +0x767, ours keeps it inline
   at +0x4da and cross-jumps the check into it (tail 38, the 7 branch-target lines); (3) the grid best-cell
   compare computes the cell's value before the best's (ours mov si,[eax] vs the original mov si,[ebx],
   +0x2a7 / +0x52c) and the gx / gy slot pair of ord_GetGridCell is swapped (+0x2ff, +0x5a1); (4) the gx =
   gy = 8 store order (order hunks). */

void ai_CalcMeisterWache(AICHAR *ch)
{
    AIOBJ *b, *o;
    AIBLD *sel, *bl;
    AIGRID *best, *cell;
    AIMSG msg;
    char buf[1024];
    char *name;
    int gx, gy;             /* the grid cell of a building (ord_GetGridCell) */
    int i, j, c, k, n, cnt, total, idle, half, found, guard;
    int r;
    unsigned char kind;

    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (b) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, b);
        ai_AddEquipmentNeeds(ch, b, ch->bld->budget);
        ai_MeisterBewaffnen(ch, b);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 10);
    ai_BuyCapacity(b, ch, 10);
    ai_BuyUpgrade(ch);
    ai_CheckFreeThieves(ch);
    ai_SendFreeThieves(ch);
    if (ai_Time.hour <= 6 || (ch->netId + ai_Time.hour) % 3 != 0) {
        ch->flags1b4 &= ~0x20;
        return;
    }
    if (ch->flags1b4 & 0x20)
        return;
    ch->flags1b4 |= 0x20;
    total = 0;
    idle = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].health > 0 && ai_CharsT[i].id != 0xffff && ai_CharsT[i].kind != 10 && ai_CharsT[i].bld == ch->bld
            && ai_CharsT[i].title && ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
            total++;
            if (!ai_CharsT[i].job)
                idle++;
        }
    }
    if (ai_Watch.bld == ch->bld) {
        ai_Watch.n = idle;
        if (ch->target != -1)
            ai_Watch.target = gm_FindBuildingById(ch->target);
    }
    half = min(total >> 1, total - (total >> 1));
    if (idle < half) {
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    kind = ai_CharsT[ch->bld->owner].kind;
    if (kind != 6 && kind != 7) {
        if (!(util_RandomF() > 0.85) || idle < 3 || ai_Time.year < 7 - ai_Difficulty)
            r = util_RandomU(3);
        else
            goto attack;
    } else {
        r = util_RandomU(3);
        if (r != 0 && ch->target != -1 && util_RandomU(2) != 0)
            goto city;
    }
    if (r == 4) {
attack:
        if (ai_CalcAngriff(ch, idle) != 0)
            goto umland;
        goto city;
    }
    if (r == 0) {
city:
        /* a patrol in the town: the best-rated grid cell, there the building of kind 6 whose owner he likes best */
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s m\xf6" "chte in der Stadt patroullieren", ch->name);
        gx = 8;
        gy = 8;
        best = &ai_Grid[0][0];
        for (j = 0; j < 8; j++) {
            cell = &ai_Grid[0][j];
            for (i = 8; i; i--) {
                if (!best->used || (cell->used && (cell->a >> 3) + cell->b > (best->a >> 3) + best->b))
                    best = cell;
                cell += 8;
            }
        }
        sel = 0;
        bl = gm_BuildQuery(1, 6);
        if (!bl)
            goto umland;
        do {
            if (bl->object && ord_GetGridCell(bl->object + 0x4c, &gx, &gy) && best == &ai_Grid[gx][gy]) {
                if (!sel)
                    sel = bl;
                else if (bl->owner != 0xffff && gm_Opinion(ch->id, bl->owner) > gm_Opinion(ch->id, sel->owner))
                    sel = bl;
            }
            bl = gm_BuildNext();
        } while (bl);
        if (!sel)
            goto umland;
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s patroulliert bei '%s'", ch->name, sel->name);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x43;
        msg.to = ai_CharsT[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 2;
        msg.arg[0] = sel->id;
        msg.arg[1] = ch->bld->id;
        msg.patrolRoll = util_RandomU(4) + 2;
        n = min(idle, 4);
        for (k = 0, c = 0; k < n && c < 768; c++) {
            if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
                && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
                msg.arg[6 + k++] = ai_CharsT[c].netId;
        }
        for (; k < 4; k++)
            msg.arg[6 + k] = -1;
        if (msg.arg[6] == -1)
            return;
        cm_SendLongMessage(&msg);
        return;
    }
    if (r == 1) {
        /* a check of a workshop: as the town patrol, but the building he likes least (one not flagged 1) */
        name = ch->name;
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s m\xf6" "chte Betrieb pr\xfc" "fen", name);
        gx = 8;
        gy = 8;
        best = &ai_Grid[0][0];
        for (j = 0; j < 8; j++) {
            cell = &ai_Grid[0][j];
            for (i = 8; i; i--) {
                if (!best->used || (cell->used && (cell->a >> 3) + cell->b > (best->a >> 3) + best->b))
                    best = cell;
                cell += 8;
            }
        }
        sel = 0;
        bl = gm_BuildQuery(1, 6);
        if (!bl)
            goto umland;
        do {
            if (bl->object && ord_GetGridCell(bl->object + 0x4c, &gx, &gy) && best == &ai_Grid[gx][gy]) {
                if (!sel)
                    sel = bl;
                else if (bl->owner != 0xffff && !(bl->flags5a & 1)
                         && gm_Opinion(ch->id, bl->owner) < gm_Opinion(ch->id, sel->owner))
                    sel = bl;
            }
            bl = gm_BuildNext();
        } while (bl);
        if (!sel)
            goto umland;
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s \xfc" "berpr\xfc" "ft '%s'", name, sel->name);
        memset(&msg, 0, sizeof(msg));
        msg.type = 100;
        msg.to = ai_CharsT[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[1] = sel->id;
        msg.arg[2] = ch->bld->id;
        o = gm_GetGebaeudeHauptraum(ch->bld);
        if (!o) {
            sprintf(buf, "ai_CalcMeisterWache() : gm_GetGebaeudeHauptraum() failed !!!");
            return;
        }
        msg.arg[3] = o->objId;
        guard = -1;
        msg.arg[0] = guard;
        /* @original found is never set before the search: the original reads the stack slot it shares with name
           (nonzero), so the search never runs and the check is never sent */
        for (c = 0; !found && c < 768; c++) {
            if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
                && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
                guard = ai_CharsT[c].netId;
                found++;
                msg.arg[0] = guard;
            }
        }
        if (guard == -1)
            return;
        cm_SendLongMessage(&msg);
        return;
    }
    if (r < 2)
        return;
umland:
    /* a patrol of the surroundings: one of the eight ambush spots, up to six guards (at least two) */
    sprintf(buf, "ai_CalcMeisterWache(): Meister %s patroulliert im Umland", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x65;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 2;
    strcpy((char *)&msg.arg[0], ai_AmbushSpots[util_RandomU(8)]);
    msg.arg[8] = ch->bld->id;
    o = gm_GetGebaeudeHauptraum(ch->bld);
    if (!o) {
        sprintf(buf, "ai_CalcMeisterWache() : gm_GetGebaeudeHauptraum() failed !!!");
        return;
    }
    msg.arg[9] = o->objId;
    n = min(idle, 6);
    for (cnt = 0, k = 0, c = 0; cnt < n && c < 768; c++) {
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title && !ai_CharsT[c].job
            && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
            msg.ids[k++] = ai_CharsT[c].netId;
            cnt++;
        }
    }
    for (; k < 6; k++)
        msg.ids[k] = -1;
    if (msg.ids[0] != -1 && cnt >= 2)
        cm_SendLongMessage(&msg);
}

/* the master's free people (no job, at home) into msg.ids, -1 for the rest */
#define AI_FILL_IDS(n)                                                                                      \
    k = 0;                                                                                                  \
    for (c = 0; k < (n); c++) {                                                                             \
        if (c >= 768)                                                                                       \
            break;                                                                                          \
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title                    \
            && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)  \
            msg.ids[k++] = ai_CharsT[c].netId;                                                              \
    }                                                                                                       \
    for (; k < 8; k++)                                                                                      \
        msg.ids[k] = -1;

/* 0x4817a0, the guard master's attack (ai_CalcMeisterWache): keeps his target building ch->target while it stands
   and belongs to another owner, else picks a new one among the buildings of the kinds 0x13 / 4 / 0x10 of other
   owners (not the player's, not flagged 1, no protection event) by the owner's worth times a random share of
   (8 - rank) * how little guarded its grid cell is; then, unless a report of his (object 0xca) knows the building
   well enough (reportKnow >= 50 + a random 0..49), sends up to eight idle guards to spy on it (message 0x40),
   else, with at least three, to attack it (message 0x49, the target cleared) - both only when none of his spy /
   break-in / attack events (0x40 / 0x3f / 0x49) runs on it. Returns 1 when he acted (or one runs), 0 when there is
   no target or too few guards
   @deep Read whole against the original 0x4817a0..0x481ee5 and rewritten: the target keep / drop, the grid
   maximum (i = j = 8 stored first, !(max > v)), the 256-building countdown scan with its filters and the
   worth * random * (8 - rank) * guard weight, the report search (o->reportBld, reportKnow < rnd(0x32) +
   0x32 spies again), the three event searches 0x40 / 0x3f / 0x49 on targetBld, both messages (0x40 / 0x49,
   flags 1, arg[0] / arg[1], up to min(n, 8) ids, -1 fill) and the four texts all match; the prover follows
   both sides through the target choice up to the spy message. Left is block layout: the original lays out
   [spy searches][spy message][report test][attack][return 0 inline after the kind test at
   +0x2d8][unfinished / spying sharing one sprintf tail at +0x725]; ours moves the spy message block
   (+0x634) and the return 0 (+0x765) to the end and keeps the two text tails apart (sprintf x5 vs x4), so
   the instructions after +0x3c4 align against other blocks (the code hunks; the M9 lines are the same four
   strings crossed by that order). Tried: the structured if (!o || know < rnd) form, the
   found-break-inside-the-loop form and the goto report: form - all three lay the spy message out last.
   (size: 7 of the instruction difference are shared / inline tails) (references: M9 at +0x616: our literal
   "ai_CalcAngriff(): Meister %s hat schon eine Spio" where the original's field refers to
   "ai_CalcAngriff(): Meister %s hat aber noch eine " at 0x5ff2c0 (and 1 more), explained: the M9 literal
   lines pair the same four format strings of the original in another block order (spy / attack /
   unfinished / spying are all present on both sides); the sprintf count differs by the original's merged
   text tail) (size: 569 vs 552 instructions, explained: 10 instructions: the original's cross-jumped
   sprintf tail of the two text exits (one push / call / add / epilogue copy fewer) and the inline return
   0) */
int ai_CalcAngriff(AICHAR *ch, int n)
{
    AIOBJ *o;
    AIBLD *bb, *best;
    AIJOB *job;
    AIBTYPE *bt;
    char *name;
    int i, j, k, c, r, t;
    float max, v, w, bestw;
    AIMSG msg;
    char buf[1024];

    name = ch->name;
    sprintf(buf, "ai_CalcAngriff(): Meister %s m\xf6" "chte angreifen", name);
    if (ch->target != -1) {
        best = gm_FindBuildingById(ch->target);
        if (!best)
            ch->target = -1;
        else if (best->owner == ch->bld->owner)
            ch->target = -1;
    }
    if (ch->target == -1) {
        /* the grid cell value most guarded, to scale the others by */
        max = 0.0f;
        i = 8;
        j = 8;
        for (c = 0; c < 8; c++) {
            for (k = 0; k < 8; k++) {
                v = ai_Grid[k][c].a * 0.125f + ai_Grid[k][c].b;
                if (!(max > v))
                    max = v;
            }
        }
        if (max == 0.0f)
            max = 1.0f;
        best = 0;
        bb = ai_BldsT;
        for (t = 256; t; t--, bb++) {
            if (!bb->type)
                continue;
            bt = &ai_BTypesT[bb->type];
            if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                && bb->owner != *ai_PlayerChar
                && (bt->kind == 0x13 || bt->kind == 4 || bt->kind == 0x10)
                && he_NoProtectionEvent(bb)) {
                if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                    w = 0.0f;
                else
                    w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                w = (8.0f - gm_BuildingRank(bb)) * w * 0.5;
                w = util_RandomU(100) * w * 0.01;
                if (!best) {
                    bestw = w;
                    best = bb;
                } else if ((float)gm_OwnerWorth(bb, 0) * w > (float)gm_OwnerWorth(best, 0) * bestw) {
                    bestw = w;
                    best = bb;
                }
            }
        }
        ch->target = best->id;
    }
    if (!best || (ai_BTypesT[best->type].kind != 0x13 && ai_BTypesT[best->type].kind != 4
                  && ai_BTypesT[best->type].kind != 0x10))
        return 0;
    for (o = gm_Query(ch->bld->objList, 1, 0, 0xca); o; o = gm_QueryNext())
        if (o->reportBld == best->id)
            goto report;
spy:
    /* no report on the building, or one that knows too little: spy on it (again) */
    for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto spying;
    for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto spying;
    for (job = he_FindEvent(2, 0, 0x49, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto spying;
    sprintf(buf, "ai_CalcAngriff(): Meister %s spioniert bei '%s'", name, best->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x40;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[0] = best->id;
    msg.arg[1] = ch->bld->id;
    t = min(n, 8);
    for (k = 0, c = 0; k < t && c < 768; c++) {
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
            && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
            msg.ids[k++] = ai_CharsT[c].netId;
    }
    for (; k < 8; k++)
        msg.ids[k] = -1;
    cm_SendLongMessage(&msg);
    return 1;

report:
    /* his report on the building: it must know it well enough (reportKnow 50 + a random 0..49), else spy again */
    if (o->reportKnow < util_RandomU(0x32) + 0x32)
        goto spy;
    if (n < 3)
        return 0;
    for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto unfinished;
    for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto unfinished;
    for (job = he_FindEvent(2, 0, 0x49, 3, ch->bld->id); job; job = he_NextEvent())
        if (job->targetBld == best->id)
            goto unfinished;
    sprintf(buf, "ai_CalcAngriff(): Meister %s greift an '%s' ", name, best->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x49;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[0] = best->id;
    msg.arg[1] = ch->bld->id;
    t = min(n, 8);
    for (k = 0, c = 0; k < t && c < 768; c++) {
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
            && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
            msg.ids[k++] = ai_CharsT[c].netId;
    }
    for (; k < 8; k++)
        msg.ids[k] = -1;
    if (msg.ids[0] != -1) {
        cm_SendLongMessage(&msg);
        ch->target = -1;
    }
    return 1;

unfinished:
    sprintf(buf, "ai_CalcAngriff(): Meister %s hat aber noch eine Spionage bei '%s' nicht beendet", name, best->name);
    return 1;
spying:
    sprintf(buf, "ai_CalcAngriff(): Meister %s hat schon eine Spionage bei '%s' laufen", name, best->name);
    return 1;
}
/* 0x481ef0, the thieves' master: equips and trades for his guild (the first goods-type-2 object of the building),
   hires and buys, frees idle thieves; then once per third hour (after 6:00, flag 0x20 of flags1b4) with at least half
   of his thieves idle picks a job by random: a kidnapping (message 0x3c, the richest owner by worth and the grid's
   guard weight, only while the guild holds no hostage), a break-in on his target building (message 0x3f, once a report
   knows enough, else a spying job 0x40), an attack (ai_CalcAngriff) or, by default, pickpocketing (message 0x61)
   @deep Read whole against the original 0x481ef0..0x482ef3 and fixed: the burgle scan as the original's
   countdown with bt = &ai_BTypes[type] taken before the owner tests (1195 -> 1181 instructions, equal to
   the original's 1181), the report search as a break-search followed by the report test (the found body
   now right after the loop as in the original), the report test as !(know < rnd(20) + guard * 2) (and
   eax,0x4100 / je as the original), target = best ? best->id : -1 with the goto pick threaded (the target
   = -1 block after the report loop as in the original), min() for the thief counts (the kidnap count
   min(n, rnd(3) + 1) calls util_Random twice as the original), the hostage test named (AIBLD.hostage).
   Statements, constants, callees, strings and field offsets all match; both sides make the same calls
   block for block. Left is block layout and allocation: the original lays the kidnap branch (r == 1) out
   split - its 0.95 test, sprintf and the first grid iteration up to the max store inline at +0x32f, the
   rest (the grid loop latch, the 256-building class scan, the kidnap message) after the r == 3 call to
   ai_CalcAngriff at +0xacf..+0xead; ours keeps the whole branch inline (the code hunks at +0x3af / +0x519
   / +0xe9a, 548 lines). Tried: goto pick for the 0.95 test (identical code), a switch on r (worse, 890
   code lines), the goto report / if (o) / nested-if forms of the report test (identical code). Further:
   obj in edi vs ebp (the jne / jge at +0xc9 / +0xd6 reach the original's reload of obj at +0x10f), edi = 8
   hoisted at +0x18b in the original. (references: M8 at +0xbba: ours refers to ai_Time (0x770ae0), the
   original's field to 0x770ae4 (ai_TimeHourW) (and 1 more), explained: the two M8 lines are the msg.time =
   ai_Time copy of the break-in message: both sides load ai_Time, ai_TimeHourW, 0x770ae8 and the word
   0x770aec in that order and store them to msg.time (+0x28 .. +0x34); the copy is scheduled one load
   earlier in the original (mov eax, [ai_Time] before the fromId store), so the alignment pairs each load
   with its neighbour (crossed: both sides refer to both)) */
void ai_CalcMeisterDiebe(AICHAR *ch)
{
    AIOBJ *o, *obj;
    AIBLD *bb, *best;
    AIJOB *job;
    AIBTYPE *bt;
    unsigned short *item;
    char *name;
    int i, j, k, c, n, all, half, r, t;
    float max, v, w, bestw;
    AIMSG msg;
    char buf[1024];

    obj = 0;
    o = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
    for (; !obj && o; o = gm_QueryNext()) {
        if (gm_FindById(0, &item, 0, o->ownerId) && ai_GoodsT[*item].type == 2)
            obj = o;
    }
    if (obj) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, obj);
        ai_AddEquipmentNeeds(ch, obj, ch->bld->budget);
        ai_MeisterBewaffnen(ch, obj);
        if (ai_Watch.bld == ch->bld) {
            for (c = 0; ai_Watch.nslots < 4; c++) {
                if (c >= ai_NSlots)
                    break;
                ai_Watch.slots[ai_Watch.nslots] = ai_Slots[c];
                ai_Watch.nslots++;
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, obj);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(obj, ch, 10);
    ai_BuyCapacity(obj, ch, 10);
    ai_BuyUpgrade(ch);
    if (gm_Query(ch->bld->objList, 2, 6, 0, 0x60))
        ai_CheckFreeThieves(ch);
    ai_SendFreeThieves(ch);
    if (ai_Time.hour <= 6 || (ch->netId + ai_Time.hour) % 3 != 0) {
        ch->flags1b4 &= ~0x20;
        return;
    }
    if (ch->flags1b4 & 0x20)
        return;
    ch->flags1b4 |= 0x20;
    all = 0;
    n = 0;
    for (c = 0; c < 768; c++) {
        if (ai_CharsT[c].health > 0 && ai_CharsT[c].id != 0xffff && ai_CharsT[c].kind != 10
            && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title && ai_CharsT[c].model
            && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
            all++;
            if (!ai_CharsT[c].job)
                n++;
        }
    }
    if (ai_Watch.bld == ch->bld) {
        ai_Watch.n = n;
        if (ch->target != -1)
            ai_Watch.target = gm_FindBuildingById(ch->target);
    }
    half = min(all >> 1, all - (all >> 1));
    if (n < half) {
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 || ai_CharsT[ch->bld->owner].kind == 7) {
        r = util_RandomU(2);
        if (r && ch->target != -1 && util_RandomU(2))
            goto burgle;
    } else {
        if (util_RandomF() > 0.85 && n >= 3 && ai_Time.year >= 7 - ai_Difficulty)
            goto other;
        r = util_RandomU(3);
    }
    if (r == 1) {
        if (!(util_RandomF() < 0.95)) {
            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte entf\xfc" "hren", ch->name);
            max = 0.0f;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                    if (!(max > v))
                        max = v;
                }
            }
            if (max == 0.0f)
                max = 1.0f;
            best = 0;
            bb = ai_BldsT;
            for (t = 0; t < 256; t++, bb++) {
                if (bb->type) {
                    r = gm_BuildingClass(bb->type);
                    if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1) && r != 3 && r != 5
                        && r != 0
                        && (!ai_CharsT[bb->owner].bld || ai_CharsT[bb->owner].bld->owner != ch->bld->owner)
                        && ai_CharsT[bb->owner].kind <= 7) {
                        if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                            w = 0.0f;
                        else
                            w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                        w = util_RandomU(100) * w * 0.01;
                        if (!best) {
                            bestw = w;
                            best = bb;
                        } else if ((float)gm_GetGoodsValue(&ai_CharsT[bb->owner]) * w
                                   > (float)gm_GetGoodsValue(&ai_CharsT[best->owner]) * bestw) {
                            bestw = w;
                            best = bb;
                        }
                    }
                }
            }
            if (gm_Query(ch->bld->objList, 1, 0, 0x65) && best && ch->bld->hostage == -1) {
                if (he_FindEvent(2, 0, 0x3c, 3, ch->bld->id))
                    return;
                sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s entf\xfc" "hrt %s aus '%s'", ch->name,
                        ai_CharsT[best->owner].name, best->name);
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x3c;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[0] = ai_CharsT[best->owner].netId;
                msg.arg[1] = best->id;
                msg.arg[2] = ch->bld->id;
                t = min(n, util_RandomU(3) + 1);
                AI_FILL_IDS(t)
                if (msg.ids[0] != -1) {
                    cm_SendLongMessage(&msg);
                    ch->target = -1;
                }
                return;
            }
        }
    } else if (r == 0) {
burgle:
        name = ch->name;
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte einbrechen", name);
        if (ch->target != -1) {
            best = gm_FindBuildingById(ch->target);
            if (!best)
                ch->target = -1;
            else if (best->owner == ch->bld->owner)
                ch->target = -1;
        }
        if (ch->target == -1) {
            max = 0.0f;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                    if (!(max > v))
                        max = v;
                }
            }
            if (max == 0.0f)
                max = 1.0f;
            best = 0;
            bb = ai_BldsT;
            for (t = 256; t; t--, bb++) {
                if (!bb->type)
                    continue;
                bt = &ai_BTypesT[bb->type];
                if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                    && bb->owner != *ai_PlayerChar && !gm_IsHomeType(bb) && bt->kind != '!'
                    && he_NoProtectionEvent(bb)) {
                    if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                        w = 0.0f;
                    else
                        w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                    r = gm_BuildingRank(bb);
                    w = (8.0f - r) * w * 0.5;
                    w = util_RandomU(100) * w * 0.01;
                    if (!best) {
                        bestw = w;
                        best = bb;
                    } else if ((float)gm_OwnerWorth(bb, obj != 0) * w > (float)gm_OwnerWorth(best, obj != 0) * bestw) {
                        bestw = w;
                        best = bb;
                    }
                }
            }
            if (best)
                ch->target = best->id;
            else
                ch->target = -1;
        }
        if (!best)
            goto pick;
        for (o = gm_Query(ch->bld->objList, 1, 0, 0xca); o; o = gm_QueryNext())
            if (o->reportBld == best->id)
                break;
        /* a report on the building that knows enough (reportKnow 0x5f, or a random 0 .. 19 plus twice its guard): break in */
        if (o && (o->reportKnow >= 0x5f || !((float)o->reportKnow < util_RandomU(20) + o->reportGuard * 2.0f))) {
            for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent())
                if (job->targetBld == best->id)
                    goto unfinished;
            for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent())
                if (job->targetBld == best->id)
                    goto unfinished;
            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s bricht bei '%s' ein", name, best->name);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x3f;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
            msg.flags = 1;
            msg.arg[0] = best->id;
            msg.arg[1] = ch->bld->id;
            t = min(n, 8);
            AI_FILL_IDS(t)
            if (msg.ids[0] != -1) {
                cm_SendLongMessage(&msg);
                ch->target = -1;
            }
            return;
        unfinished:
            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat aber noch eine Spionage bei '%s' nicht beendet",
                    name, best->name);
            return;
        }
        for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent())
            if (job->targetBld == best->id)
                goto spying;
        for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent())
            if (job->targetBld == best->id)
                goto spying;
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s spioniert bei '%s'", name, best->name);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x40;
        msg.to = ai_CharsT[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[0] = best->id;
        msg.arg[1] = ch->bld->id;
        t = min(n, 8);
        AI_FILL_IDS(t)
        cm_SendLongMessage(&msg);
        return;
    spying:
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat schon eine Spionage bei '%s' laufen", name, best->name);
        return;
    } else if (r == 3) {
other:
        ai_CalcAngriff(ch, n);
    } else if (r <= 1)
        return;
pick:
    sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte Taschen dieben", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x61;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[10] = ai_CharsT[ch->bld->owner].netId;
    t = min(n, 8);
    AI_FILL_IDS(t)
    if (msg.ids[0] != -1)
        cm_SendLongMessage(&msg);
}
/* 0x482f00, ai_CalcMeisterRaeuber: the robber master (the Wache message names the guard master's room) @deep B3 draft, 207 code / 12 slots hunks after the strcpy fix (was 321 / 26): every hunk of tools/asmdiff was read against build/ghidra/482f00.c - statements, constants, callees and field offsets match. Last-bytes pass: the manual copy loop `src[i] / ((char *)msg.arg)[i]` was the meaning-class lead - the original's asm is VC6's inlined strcpy (sub ecx,eax / mov dl,[eax] / mov [ecx+eax],dl / inc eax / test dl,dl); replacing it by strcpy((char *)msg.arg, src) dropped the function from 321 code / 26 slots to 207 / 12 and its asm diff from 211 to 139 lines. The same idiom was applied to ai_CalcMeisterWache (byte-neutral there, its pointer-loop form already compiles as the inline copy). What is left is allocation / block order (the ebp/esi roles, [esp+0x1c..0x44] rotation, the best/rank block emitted before vs after the compare) = match-deep. Q2 gaps: ord_GetGridCell (ord.c's ord_GetGridCell; unit unnamed, @named_by guess), AIBLD.budget (a building amount used as the purchase limit, scaled by 1/64000 in ai.c - no source names it). */

void ai_CalcMeisterRaeuber(AICHAR *ch)
{
    AIOBJ *obj, *room;
    AIBLD *bb, *best;
    AIBTYPE *bt;
    int i, j, k, c, n, all, half, r, t;
    volatile int z;
    float max, v, w, bestw;
    char *src;
    AIMSG msg;
    char buf[1024];

    z = 0;
    obj = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
    if (obj) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, obj);
        ai_AddEquipmentNeeds(ch, obj, ch->bld->budget);
        ai_MeisterBewaffnen(ch, obj);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, obj);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(obj, ch, 10);
    ai_BuyCapacity(obj, ch, 10);
    ai_BuyUpgrade(ch);
    ai_CheckFreeThieves(ch);
    ai_SendFreeThieves(ch);
    if (ai_Time.hour <= 6 || (ch->netId + ai_Time.hour) % 3 != 0) {
        ch->flags1b4 &= ~0x20;
        return;
    }
    if (ch->flags1b4 & 0x20)
        return;
    ch->flags1b4 |= 0x20;
    all = 0;
    n = 0;
    for (c = 0; c < 768; c++) {
        if (ai_CharsT[c].health > 0 && ai_CharsT[c].id != 0xffff && ai_CharsT[c].kind != 10
            && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title && ai_CharsT[c].model
            && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
            all++;
            if (!ai_CharsT[c].job)
                n++;
        }
    }
    if (ai_Watch.bld == ch->bld) {
        ai_Watch.n = n;
        if (ch->target != -1)
            ai_Watch.target = gm_FindBuildingById(ch->target);
    }
    half = min(all >> 1, all - (all >> 1));
    if (n < half) {
        sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 || ai_CharsT[ch->bld->owner].kind == 7) {
        r = util_RandomU(2);
        if (r && ch->target != -1 && util_RandomU(2))
            goto lauer;
    } else {
        if (util_RandomF() > 0.85 && n >= 3 && ai_Time.year >= 7 - ai_Difficulty)
            goto attack;
        r = util_RandomU(4);
    }
    if (r == 3) {
attack:
        ai_CalcAngriff(ch, n);
    } else if (r == 0) {
        if (!(util_RandomF() < 0.75)) {
            sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s m\xf6" "chte Geld sch\xfctzen", ch->name);
            max = 0.0f;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                    if (!(max > v))
                        max = v;
                }
            }
            if (max == 0.0f)
                max = 1.0f;
            best = 0;
            bb = ai_BldsT;
            for (t = 0; t < 256; t++, bb++) {
                if (bb->type) {
                    bt = &ai_BTypesT[bb->type];
                    if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                        && bb->owner != *ai_PlayerChar && !gm_IsHomeType(bb) && bt->kind != '!') {
                        if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                            w = 0.0f;
                        else
                            w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                        w = util_RandomU(100) * w * 0.01;
                        if (best) {
                            if (!((float)gm_GetGoodsValue(&ai_CharsT[bb->owner]) * w > (float)gm_GetGoodsValue(&ai_CharsT[best->owner]) * bestw))
                                continue;
                        }
                        bestw = w;
                        best = bb;
                    }
                }
            }
            ch->target = best->id;
            sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s sch\xfctzt Geld bei '%s'", ch->name, best->name);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x62;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
            msg.flags = 2;
            msg.arg[0] = -1;
            msg.arg[1] = best->id;
            msg.arg[4] = ch->bld->id;
            room = gm_GetGebaeudeHauptraum(ch->bld);
            if (!room) {
                sprintf(buf, "ai_CalcMeisterWache() : gm_GetGebaeudeHauptraum() failed !!!");
                return;
            }
            msg.arg[5] = room->objId;
            k = z;
            for (c = 0; k == 0; c++) {
                if (c >= 768)
                    break;
                if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
                    && !ai_CharsT[c].job && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
                    msg.arg[0] = ai_CharsT[c].netId;
                    k++;
                }
            }
            if (msg.arg[0] == -1)
                return;
            cm_SendLongMessage(&msg);
            return;
        }
    } else if (r < 1)
        return;
lauer:
    sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s m\xf6" "chte sich auf die Lauer legen", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x48;
    msg.to = ai_CharsT[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 2;
    src = ai_AmbushSpots[util_RandomU(8)];
    strcpy((char *)msg.arg, src);
    if (n > 7)
        n = 8;
    k = 0;
    t = 0;
    for (c = 0; t < n; c++) {
        if (k >= 4 || c >= 768)
            break;
        if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title && !ai_CharsT[c].job
            && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id) {
            msg.ids[t++] = ai_CharsT[c].netId;
            k++;
        }
    }
    for (i = t; i < 8; i++)
        msg.ids[i] = -1;
    if (msg.ids[0] != -1 && k > 1)
        cm_SendLongMessage(&msg);
}
/* 0x4836a0, the bank master's money balance: moves money between the four banks / the town @deep byte, Q-only handover: Q2 only, after this session's readability work (Q1 named by role - the bank master of 0x47e990's kind-5 dispatch, its comment already described it; ai_Unk508660 -> gm_SumSlotAmounts with the f3gm.c definition). Q2 evidence gaps left, all three read in the callers, callees and data: cm_FieldObject (the object the cm_AddFieldDelta addresses are formed against; no source names it), AIPOS.unk18 (the table record 0xe field the building's 0x65 value is steered towards within +-2, clamped to >= 4; no reader names it), AIBLD.unk65 (the value moved by +-1 per event and written as field delta 0x65; no source names it). */

void ai_CalcMeisterBank(AICHAR *ch)
{
    AIPOS p;
    AIJOB *job;
    AIBLD *b;
    int i, n, v, d, m, sum;

    if (ch->flags1c8 & 8)
        return;
    if (!gv_GetTableRecord(0xe, &p))
        return;
    n = 0;
    for (job = he_FindEvent(2, 0, 0x23, 3, ch->bld->id); job; job = he_NextEvent())
        n++;
    if (abs(ch->bld->unk65 - p.unk18) > 3) {
        if (ch->bld->unk65 > p.unk18)
            v = ch->bld->unk65 - 1;
        else
            v = ch->bld->unk65 + 1;
        cm_BeginFieldBlock(ch->bld, ch->bld->id);
        cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
        cm_SendFieldDeltas();
    } else if (n == 0) {
        if (ch->bld->unk65 > p.unk18 - 2) {
            v = ch->bld->unk65 - util_RandomU(3) - 1;
            if (p.unk18 - 2 > v)
                v = p.unk18 - 2;
            if (v < 4)
                v = 4;
            cm_BeginFieldBlock(ch->bld, ch->bld->id);
            cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
            cm_SendFieldDeltas();
        }
    } else if (n > 3 && ch->bld->unk65 < p.unk18) {
        v = util_RandomU(3) + 1 + ch->bld->unk65;
        if (v >= p.unk18 + 3)
            v = p.unk18 + 3;
        cm_BeginFieldBlock(ch->bld, ch->bld->id);
        cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
        cm_SendFieldDeltas();
    }
    sum = 0;
    for (i = 1; i < 4; i++) {
        v = gm_SumSlotAmounts(ch->bld->objList, i);
        m = ch->budget;
        if (v < m * 3 / 20) {
            d = m * 5 / 20 - v;
            sum += d;
            cm_BeginGroup("Bankmeister");
            cm_SendMoveGoods(ch->bld->id, -1, i, d);
            cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)(d * 1.03));
            cm_EndGroup("Bankmeister");
        } else if (v > m * 6 / 20) {
            d = v - m * 4 / 20;
            sum -= d;
            cm_BeginGroup("Bankmeister");
            cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, ai_OwnPlayer, d);
            cm_SendMoveGoods(-1, ch->bld->id, i, (int)(d * 1.03));
            cm_EndGroup("Bankmeister");
        }
    }
    v = gm_SumSlotAmounts(ch->bld->objList, ai_OwnPlayer);
    m = ch->budget;
    if (!(v < m * 1.5) || m - sum <= 0) {
        b = ch->bld;
        if (ai_CharsT[b->owner].kind != 6 && ai_CharsT[b->owner].kind != 7 && v > m * 2)
            cm_SendMoveGoods(ai_CharsT[b->owner].netId, b->id, ai_OwnPlayer, (int)(v - m * 2.25));
    } else {
        b = ch->bld;
        cm_SendMoveGoods(b->id, ai_CharsT[b->owner].netId, ai_OwnPlayer, (int)((m - sum) * 0.8));
    }
    ch->flags1c8 |= 8;
}
/* 0x483a60, the kind-9 master's trade step (0x47e990 dispatches here on ai_BTypes[type].kind == 9): in every
   third hour it buys slots / capacity, collects the building's kind 0x1d objects without a running job into
   ai_Carts, lets ai_BuildTradeOffers decide the trade and runs ai_MeisterTradeFernEinkauf or ...2 by the ratio
   of the first slot and entry; a watched building gets its slots copied
   @deep byte, Q-only handover: Q6 description added this session (kind-9 trade step: buys slots / capacity, collects the free kind 0x1d objects into ai_Carts, ai_BuildTradeOffers decides the trade, then ai_MeisterTradeFernEinkauf or ...2 by the slot/entry ratio). Q1 no evidenced name: 0x47e990 dispatches here on ai_BTypes[type].kind == 9 only, the body has no string / text id, and no caller names the profession. Q2 evidence gaps: ai_BuildTradeOffers (the trade decision builder, no comment or named reader), ai_Carts / ai_NumCarts (the object list and its count this function fills; the reader is ai_BuildTradeOffers only), AIOBJ.unk1c (a building id here, but the rename across units is undecided against the other unk1c fields), AIOBJ.unk20 / AISLOT.unk34 / AIENTRY.unk4c / AIBTYPE.level (the slot and entry amounts, cap247 is only an unsigned alias), AIJOB.unkac (union id). */

void ai_Unk483a60(AICHAR *ch)
{
    AIOBJ *obj, *o;
    AIJOB *job;
    AIOBJ *it;
    float f;
    int i, ok, c;

    if ((ch->netId + ai_Time.hour) % 3 == 0) {
        if (!(ch->flags1b4 & 0x20)) {
            obj = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
            if (!obj)
                return;
            ai_BuySlots(obj, ch, 0x14);
            ai_BuyCapacity(obj, ch, 0x32);
            ok = 1;
            it = ai_Objects;
            for (i = 0; ok && i < 0x5008; it++, i++) {
                if (it->id && ai_GoodsT[it->id].type == 0x1d && it->unk1c == ch->bld->id && it->id == 0x136)
                    ok = 0;
            }
            ai_NumCarts = 0;
            for (o = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); o; o = gm_QueryNext()) {
                if (o->unk1c == ch->bld->id) {
                    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
                        if (job->unkac == o->objId)
                            break;
                    }
                    if (!job && ai_NumCarts < 0x10)
                        ai_Carts[ai_NumCarts++] = o;
                }
            }
            if (ai_NumCarts == 0) {
                ai_MeisterCollectTrans(ch);
                return;
            }
            ch->flags1b4 |= 0x20;
            ai_BuildTradeOffers(ch, obj, ok);
            if (ai_BTypesT[ch->bld->type].level == 1)
                f = ch->bld->budget * 1.5625e-05f;
            else if (ai_BTypesT[ch->bld->type].level == 2)
                f = ch->bld->budget * 7.8125e-06f;
            else if (ai_BTypesT[ch->bld->type].level == 3)
                f = ch->bld->budget * 3.90625e-06f;
            f = f * 0.5f + 0.25f;
            if (ai_NSlots != 0) {
                if (ai_NEntries != 0) {
                    if (ai_Slots[0].buyAmount * ai_Slots[0].saving * f > (1.0f - f) * ai_Entries[0].buyAmount * ai_Entries[0].margin)
                        ai_MeisterTradeFernEinkauf(ch, obj);
                    else
                        ai_MeisterTradeFernEinkauf2(ch, obj);
                } else
                    ai_MeisterTradeFernEinkauf(ch, obj);
            } else if (ai_NEntries != 0)
                ai_MeisterTradeFernEinkauf2(ch, obj);
            if (ai_Watch.bld == ch->bld) {
                for (c = 0; ai_Watch.nslots < 4; c++) {
                    if (c >= ai_NSlots)
                        break;
                    ai_Watch.slots[ai_Watch.nslots] = ai_Slots[c];
                    ai_Watch.nslots++;
                }
            }
        }
    } else
        ch->flags1b4 &= ~0x20;
}
/* 0x483d30, starts a work on a building (the event 0x8d) for the kind-0x1d master at 7 / 12 / 18 o'clock
   when none runs yet: picks the work entry below a random threshold (gv_FindWornEntry) and a free worker
   of the building, then sends message 0x8d with the entry's value, its list index and the worker's netId */

void ai_StartBuildingWork(AICHAR *ch)
{
    AIMSG msg;
    AICHAR *f;
    GVENTRY *p;
    int c;

    if ((short)ai_Time.hour != 7 && (short)ai_Time.hour != 0xc && (short)ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x800;
        return;
    }
    if (!(ch->flags1b4 & 0x800)) {
        ch->flags1b4 |= 0x800;
        if (!he_FindEvent(2, 0, 0x8d, 3, ch->bld->id)) {
            p = gv_FindWornEntry(ch->bld, (char)util_RandomU(0x40) - 0x80);
            if (p) {
                f = 0;
                for (c = 0; !f; c++) {
                    if (c >= 768) {
                        if (!f)
                            return;
                        break;
                    }
                    if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
                        && (!ai_CharsT[c].job || ai_CharsT[c].job->kind == 4 || ai_CharsT[c].job->kind == (char)0x8c)
                        && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
                        f = &ai_CharsT[c];
                }
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x8d;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.graveWork.size = p->size;
                msg.graveWork.index = gv_EntryIndex((char *)ch->bld->yard, (int *)p);
                msg.ids[0] = f->netId;
                cm_SendLongMessage(&msg);
            }
        }
    }
}
/* 0x483ee0, the worker step of the kind-0x1d master (the graveyard: 0x47e990 dispatches 0x483fc0 for the
   building kind 0x1d), run at 7 / 12 / 18 o'clock and once per hour (flag bit 0x1000, cleared in every other
   hour): of the characters standing in the master's building it takes the first with the worker title 0x1c (the
   two titles 0x1b / 0x1c that he.c's work events 0x8c / 0x8d scale differently and that sv.c 0x52b500 poses as
   "faulenzen"), at work (+0x1b4 bit 0) and with its model in the building, and with a 25% chance ends that
   character's role pose (ai_EndCharExtra(f, 0)), which sv.c then builds again from the current role */

void ai_EndWorkerPose(AICHAR *ch)
{
    AICHAR *f;
    int c;

    f = 0;
    if ((short)ai_Time.hour != 7 && (short)ai_Time.hour != 0xc && (short)ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x1000;
        return;
    }
    if (!(ch->flags1b4 & 0x1000)) {
        ch->flags1b4 |= 0x1000;
        for (c = 0; !f; c++) {
            if (c >= 768) {
                if (!f)
                    return;
                break;
            }
            if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title == 0x1c
                && ai_CharsT[c].kind == 0x14 && (ai_CharsT[c].flags1b4 & 1) && ai_CharsT[c].model
                && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
                f = &ai_CharsT[c];
        }
        if (util_RandomF() > 0.75f)
            ai_EndCharExtra(f, 0);
    }
}
/* 0x483fc0, evaluates the production of a graveyard (building kind 0x1d): offers graves (ai_OfferGrave), then as
   ai_EvaluateProduction picks the good to make (the current one at 8 / 13 o'clock, the player's drawn setting, a
   random entry with or without its materials), skipping the entries flagged 0x400: those lose flag 8 and get job
   orders of type 0x8c when a slot flagged 0x400 | 0x10 marks them 0x10, the others of type 4 for flag 8
   @deep Read whole against the original 0x483fc0..0x484a56 and fixed: the entry tests on ai_Entries[i] /
   ai_Entries[k] with the entry field on the left (capacity >= batch, lvl1c >= need1c, lvl1d >= need20,
   gm_FreeCapacity >= batch + amount; the original's mov eax,[esi+ai_Entries+0x40] / cmp eax,edx / jl), the
   two 0x400 / flag-8 loops without the 0 < ai_NEntries wrapper (each reloads and tests ai_NEntries), the
   watch loop's j >= ai_NEntries, health > 0 (jbe), info.n2++ / info.n1++ counted in the struct (the
   original keeps n1 in edx and stores it after the loop), info.n1 = min(info.n1, info.n2), the job-order
   loop as if (flags & 0x10) { if (flags & 0x400) type 0x8c } else if (flags & 8) type 4 with type stored
   before e and the watch copy from *info.e, the info fields stored ch, b, c, z2, z1. 470 -> 138 differing,
   every call, constant and field the same. Left is allocation: the goods-record address of the random
   loops (+0x454 / +0x555: ours add ebp,edx / mov eax,ecx / add eax,ebp / mov dl,[eax]; the original add
   ecx,edx / lea edx,[eax+ecx] / mov cl,[eax+ecx], item in eax vs ecx), the zero register compare at +0x1e8
   (test esi,esi vs cmp esi,ebp), the watch copy's register / scheduling order at +0x7e9 (mov ebx,4 / lea
   before mov ecx,0x16), and 144 regs lines; the prover stops at +0x562 on the register-swapped compare of
   the same fields. */

void ai_EvaluateProductionGraveyard(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, found;

    found = 0;
    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    ai_UpdateProduction(ch, a, b);
    ai_OfferGrave(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    ai_CheckFreeStaff(ch);
    ai_SendFreeSpies(ch);
    ai_EndWorkerPose(ch);
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                        && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                        ai_Entries[i].flags |= 8;
                        ai_StartProduction(ch, &ai_Entries[i]);
                        ch->prod = (unsigned short)ai_Entries[i].item;
                        goto done;
                    }
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].item == ch->prod) {
                if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                    && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, &ai_Entries[i]);
                    ch->prod = (unsigned short)ai_Entries[i].item;
                    found = 1;
                }
                break;
            }
        }
        ch->flags1b4 &= ~0x100;
        if (found)
            goto done;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (ai_Entries[i].capacity >= ai_GoodsT[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[i].amount) {
                        if (!ai_HasMaterials(ch, e, 1)) {
                            if (ai_HasMaterials(ch, e, 0)) {
                                ai_StartProduction(ch, e);
                                ch->prod = (unsigned short)e->item;
                                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                        ai_GoodsT[e->item].name);
                                goto done;
                            }
                        } else {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_GoodsT[e->item].name);
                            goto done;
                        }
                    }
                }
                break;
            }
        }
    }
    for (i = 0; i < 32; i++)
        info.perm[i] = i;
    i = 0;
    do {
        r = util_RandomU(0x20);
        t = info.perm[i];
        i++;
        info.perm[i - 1] = info.perm[r];
        info.perm[r] = t;
    } while (i < 32);
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 1)) {
                k = info.perm[i];
                ai_Entries[k].flags |= 8;
                e = &ai_Entries[k];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 0)) {
                e = &ai_Entries[info.perm[i]];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8) {
            for (k = 0; k < 4; k++) {
                if (ai_Slots[ai_Entries[i].slot[k]].amount < ai_GoodsT[ai_Entries[i].item].need[k])
                    ai_Entries[i].flags &= ~8;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 0x400)
            ai_Entries[i].flags &= ~8;
    }
    for (i = 0; i < ai_NSlots; i++) {
        if ((ai_Slots[i].flags & 0x400) && (ai_Slots[i].flags & 0x10) && ai_Slots[i].entry >= 0)
            ai_Entries[ai_Slots[i].entry].flags |= 0x10;
    }
    if (ai_Watch.bld == ch->bld) {
        for (j = 0; ai_Watch.nslots < 4; j++) {
            if (j >= ai_NEntries)
                break;
            ai_Watch.ent[ai_Watch.nslots] = ai_Entries[j];
            for (k = 0; k < 4; k++) {
                if (ai_Entries[j].slot[k] >= 0) {
                    ai_Watch.slots[ai_Watch.cnt[ai_Watch.nslots] + ai_Watch.nslots * 4] =
                        ai_Slots[ai_Entries[j].slot[k]];
                    ai_Watch.cnt[ai_Watch.nslots]++;
                }
            }
            ai_Watch.nslots++;
        }
    }
    ai_FreeJobCharacters(ch, 4);
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_CharsT[j].health > 0 && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10 && ai_CharsT[j].bld == ch->bld
            && ai_CharsT[j].title && ai_CharsT[j].model && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id
            && (!ai_CharsT[j].job || ai_CharsT[j].job->kind == 4)) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            info.n2++;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = info.n2;
    ai_StartBuildingWork(ch);
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            info.n1++;
        if (ai_Entries[i].flags & 0x10)
            info.n1++;
    }
    info.n1 = min(info.n1, info.n2);
    if (info.n2 != 0 && info.n1 != 0) {
        info.ch = ch;
        info.b = b;
        info.c = 0;
        info.z2 = 0;
        info.z1 = 0;
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Entries[j].flags & 0x10) {
                if (ai_Entries[j].flags & 0x400) {
                    info.type = 0x8c;
                    info.e = &ai_Entries[j];
                    ai_SendJobOrders((AIINFO *)&info);
                    if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                        ai_Watch.entries[ai_Watch.nentries] = *info.e;
                        ai_Watch.nentries++;
                    }
                }
            } else if (ai_Entries[j].flags & 8) {
                info.type = 4;
                info.e = &ai_Entries[j];
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
/* 0x484a60, evaluates the production of a gypsy camp (building kind 0x1f): as ai_EvaluateProductionGraveyard
   without the grave offer and the staff / spy / pose steps (ai_UpdateStaff instead): picks the good to make, skips
   the entries flagged 0x400 (job orders of type 0x8c when a slot flagged 0x400 | 0x10 marks them 0x10), the others
   get job orders of type 4 for flag 8
   @deep Read whole against the original 0x484a60..0x485473 and rewritten as ai_EvaluateProductionGraveyard
   (its twin, read the same way): the entry tests on ai_Entries[i] / [k] with the field on the left, the
   0x400 / flag-8 loops each testing ai_NEntries, j >= ai_NEntries in the watch loop, health > 0, info.n2++
   / info.n1++ in the struct, min(info.n1, info.n2), the job-order loop's 0x10 / 0x400 / 8 tests with type
   before e and the watch copy from *info.e, info stored ch, b, c, z2, z1; the call list is the original's
   (ai_UpdateStaff, no ai_StartBuildingWork). 569 -> 328 differing, every call, constant and field the
   same. Left is block layout and allocation: the original keeps the first random loop's success block
   inline (+0x4c7..+0x50b) and lays the second loop's success (StartProduction / 'obwohl Rohstoffe fehlen')
   out of line inside the watch loop (+0x77a, jmp back to done at +0x679), ours the other way round (the
   code hunks at +0x4c7 and +0x602); ch lives in ebx in the original and is reloaded from [esp+0x1c0] by
   ours in the job-order loop where ours keeps j in ebx (+0x999 / +0x9ce / +0xa0e), the goods-record
   address of the random loops (+0x43a: add ecx,edx / lea edx,[eax+ecx] vs ours add eax,edx / mov edx,eax),
   the zero register compare at +0x1d6. Same layout quirk as ai_EvaluateProductionFlags 0x47f700. */

void ai_EvaluateProductionGypsies(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, found;

    found = 0;
    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    ai_UpdateProduction(ch, a, b);
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                        && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                        ai_Entries[i].flags |= 8;
                        ai_StartProduction(ch, &ai_Entries[i]);
                        ch->prod = (unsigned short)ai_Entries[i].item;
                        goto done;
                    }
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].item == ch->prod) {
                if (ai_Entries[i].capacity >= ai_GoodsT[ai_Entries[i].item].batch
                    && ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, &ai_Entries[i]);
                    ch->prod = (unsigned short)ai_Entries[i].item;
                    found = 1;
                }
                break;
            }
        }
        ch->flags1b4 &= ~0x100;
        if (found)
            goto done;
    }
    if (ai_CharsT[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (ai_Entries[i].capacity >= ai_GoodsT[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[i].amount) {
                        if (!ai_HasMaterials(ch, e, 1)) {
                            if (ai_HasMaterials(ch, e, 0)) {
                                ai_StartProduction(ch, e);
                                ch->prod = (unsigned short)e->item;
                                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                        ai_GoodsT[e->item].name);
                                goto done;
                            }
                        } else {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_GoodsT[e->item].name);
                            goto done;
                        }
                    }
                }
                break;
            }
        }
    }
    for (i = 0; i < 32; i++)
        info.perm[i] = i;
    i = 0;
    do {
        r = util_RandomU(0x20);
        t = info.perm[i];
        i++;
        info.perm[i - 1] = info.perm[r];
        info.perm[r] = t;
    } while (i < 32);
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 1)) {
                k = info.perm[i];
                ai_Entries[k].flags |= 8;
                e = &ai_Entries[k];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || ai_Entries[k].amount < 5)
                && b->lvl1c >= ai_Entries[k].need1c && b->lvl1d >= ai_Entries[k].need20
                && ai_Entries[k].capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + ai_Entries[k].amount
                && ai_HasMaterials(ch, e, 0)) {
                e = &ai_Entries[info.perm[i]];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8) {
            for (k = 0; k < 4; k++) {
                if (ai_Slots[ai_Entries[i].slot[k]].amount < ai_GoodsT[ai_Entries[i].item].need[k])
                    ai_Entries[i].flags &= ~8;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 0x400)
            ai_Entries[i].flags &= ~8;
    }
    for (i = 0; i < ai_NSlots; i++) {
        if ((ai_Slots[i].flags & 0x400) && (ai_Slots[i].flags & 0x10) && ai_Slots[i].entry >= 0)
            ai_Entries[ai_Slots[i].entry].flags |= 0x10;
    }
    if (ai_Watch.bld == ch->bld) {
        for (j = 0; ai_Watch.nslots < 4; j++) {
            if (j >= ai_NEntries)
                break;
            ai_Watch.ent[ai_Watch.nslots] = ai_Entries[j];
            for (k = 0; k < 4; k++) {
                if (ai_Entries[j].slot[k] >= 0) {
                    ai_Watch.slots[ai_Watch.cnt[ai_Watch.nslots] + ai_Watch.nslots * 4] =
                        ai_Slots[ai_Entries[j].slot[k]];
                    ai_Watch.cnt[ai_Watch.nslots]++;
                }
            }
            ai_Watch.nslots++;
        }
    }
    ai_FreeJobCharacters(ch, 4);
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_CharsT[j].health > 0 && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10 && ai_CharsT[j].bld == ch->bld
            && ai_CharsT[j].title && ai_CharsT[j].model && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id
            && (!ai_CharsT[j].job || ai_CharsT[j].job->kind == 4)) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            info.n2++;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = info.n2;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            info.n1++;
        if (ai_Entries[i].flags & 0x10)
            info.n1++;
    }
    info.n1 = min(info.n1, info.n2);
    if (info.n2 != 0 && info.n1 != 0) {
        info.ch = ch;
        info.b = b;
        info.c = 0;
        info.z2 = 0;
        info.z1 = 0;
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Entries[j].flags & 0x10) {
                if (ai_Entries[j].flags & 0x400) {
                    info.type = 0x8c;
                    info.e = &ai_Entries[j];
                    ai_SendJobOrders((AIINFO *)&info);
                    if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                        ai_Watch.entries[ai_Watch.nentries] = *info.e;
                        ai_Watch.nentries++;
                    }
                }
            } else if (ai_Entries[j].flags & 8) {
                info.type = 4;
                info.e = &ai_Entries[j];
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
/* 0x4854f0, builds the workshop's production tables: an entry per good the production object a can make (its
   goods listed more than once, up to 32), a slot per input good of the entries (up to 128, gives up when full);
   for the building kinds 0xe / 8 / 0x20 the goods 0x1c4 .. 0x1c7 / 0x2af / 0x2b0 get flag 0x24 and 0x1c1 .. 0x1c3
   flag 0x204, for the kinds 0x1d / 0x1f the goods 0x225 .. 0x227 / 0x293 .. 0x295 flag 0x404 (entries added for
   them); then the store b's amounts and free capacities, the pieces the running jobs (events 0xb, 0x14 / 0x15,
   0x8c) bring, the costs, the entries sorted by margin, flag 0x40 for a slot that is also an entry, the ranks
   @deep Read whole against the original 0x4854f0..0x486181: the entry / slot table build, the three kind
   groups (0xe / 8 / 0x20: 0x24 and 0x204, 0x1d / 0x1f: 0x404) with their entry creation, the slot-entry
   links, amounts and capacities, the pending sums of the events 0xb / 0x14 / 0x15 / 0x8c, the costs, the
   margin sort, the 0x40 flags and the ranks all match statement for statement (every call, constant and
   field; 33 differing of 933). Readability done here: AIENTRY need1c / need20 for the unk1c / unk20
   stores, AISLOT.unk10 -> cleared10 (only ever cleared), the description. Left are two layout / scheduling
   hunks: (1) +0x61: the inline memset of the new entry's four slots - ours lea ebp,[ecx+ai_Entries+4] /
   mov [ebp..ebp+0xc],eax, the original mov [ecx+0xf88a44..0xf88a50],eax with the ai_NEntries store
   between; tried individual slot[k] = -1 stores (the -1 is hoisted out of the loop into esi, 106
   differing), memset(.., -1, 16), memset(&..slot, 0xff, 16), memset(&slot[0], 0xff, sizeof(int) * 4): all
   identical to the current; (2) +0x322..+0x51b: the kind 0xe / 8 / 0x20 group - the original falls into
   the 0x24 block after the 0x1c7 .. 0x2b0 compares (jne to the 0x1c1 tests) and lays the 0x204 block after
   it, ours jumps to the 0x24 block laid out after the 0x204 one (je 0x485956); tried continue, else if and
   the inverted != chain: identical code. */

void ai_UpdateProduction(AICHAR *ch, AIOBJ *a, AIOBJ *b)
{
    AIOBJ *it;
    AIOBJ *o;
    AIJOB *job;
    void *p;
    AIRECIPE rec;
    AIENTRY tmp;
    unsigned short g;
    char kind;
    int i, j, k, m, found;

    ai_NEntries = 0;
    for (it = gm_Query(a->list, 1, 5); it; it = gm_QueryNext()) {
        if (it->count > 1 && ai_NEntries < 32) {
            ai_Entries[ai_NEntries].item = it->id;
            memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
            ai_Entries[ai_NEntries].need1c = 0;
            ai_Entries[ai_NEntries].need20 = 0;
            ai_Entries[ai_NEntries].cost = -1e10f;
            ai_Entries[ai_NEntries].pending = 0;
            ai_Entries[ai_NEntries].amount = 0;
            ai_Entries[ai_NEntries].capacity = 0;
            ai_Entries[ai_NEntries].sellAmount = 0;
            ai_Entries[ai_NEntries].rank = 0x21;
            ai_Entries[ai_NEntries].flags = 0;
            ai_NEntries++;
        }
    }
    ai_NSlots = 0;
    for (i = 0; i < ai_NEntries; i++) {
        for (k = 0; k < 4; k++) {
            if (ai_GoodsT[ai_Entries[i].item].in[k]) {
                for (j = 0; j < ai_NSlots; j++) {
                    if (ai_Slots[j].item == ai_GoodsT[ai_Entries[i].item].in[k]) {
                        ai_Entries[i].slot[k] = j;
                        break;
                    }
                }
                if (ai_Entries[i].slot[k] == -1) {
                    if (ai_NSlots >= 128)
                        return;
                    ai_Slots[ai_NSlots].item = ai_GoodsT[ai_Entries[i].item].in[k];
                    ai_Slots[ai_NSlots].entry = -1;
                    ai_Slots[ai_NSlots].bld = 0;
                    ai_Slots[ai_NSlots].cleared10 = 0;
                    ai_Slots[ai_NSlots].make = 0;
                    ai_Slots[ai_NSlots].sellAmount = 0;
                    ai_Slots[ai_NSlots].price = -1e10f;
                    ai_Slots[ai_NSlots].pending = 0;
                    ai_Entries[i].slot[k] = ai_NSlots;
                    ai_Slots[ai_NSlots].amount = 0;
                    ai_Slots[ai_NSlots].rank = 0x21;
                    ai_Slots[ai_NSlots].flags = 0;
                    ai_NSlots++;
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 14 || kind == 8 || kind == 32) {
        for (j = 0; j < ai_NSlots; j++) {
            if (ai_Slots[j].item == 0x1c7 || ai_Slots[j].item == 0x1c4 || ai_Slots[j].item == 0x1c5 || ai_Slots[j].item == 0x1c6 || ai_Slots[j].item == 0x2af || ai_Slots[j].item == 0x2b0) {
                ai_Slots[j].flags |= 0x24;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x24;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].need1c = 0;
                    ai_Entries[ai_NEntries].need20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x24;
                    ai_NEntries++;
                }
            } else if (ai_Slots[j].item == 0x1c1 || ai_Slots[j].item == 0x1c2 || ai_Slots[j].item == 0x1c3) {
                ai_Slots[j].flags |= 0x204;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x204;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].need1c = 0;
                    ai_Entries[ai_NEntries].need20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x204;
                    ai_NEntries++;
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 29 || kind == 31) {
        for (j = 0; j < ai_NSlots; j++) {
            if (ai_Slots[j].item == 0x225 || ai_Slots[j].item == 0x226 || ai_Slots[j].item == 0x227 || ai_Slots[j].item == 0x293
                || ai_Slots[j].item == 0x294 || ai_Slots[j].item == 0x295) {
                ai_Slots[j].flags |= 0x404;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x404;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].need1c = 0;
                    ai_Entries[ai_NEntries].need20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x404;
                    ai_NEntries++;
                }
            }
        }
    }
    for (j = 0; j < ai_NSlots; j++) {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Slots[j].item == ai_Entries[i].item) {
                ai_Slots[j].entry = i;
                break;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        o = gm_Query(b->list, 1, 0, ai_Entries[i].item);
        if (!o)
            ai_Entries[i].amount = 0;
        else
            ai_Entries[i].amount = gm_ListAmount(b, o);
        ai_Entries[i].capacity = gm_FreeCapacity(b, ai_Entries[i].item, 1000000);
    }
    for (j = 0; j < ai_NSlots; j++) {
        o = gm_Query(b->list, 1, 0, ai_Slots[j].item);
        if (!o) {
            ai_Slots[j].amount = 0;
        } else {
            ai_Slots[j].amount = gm_ListAmount(b, o);
            if (!ai_Slots[j].amount)
                cm_SendRemoveGoodsI(b->objId, ai_Slots[j].item);
        }
        ai_Slots[j].capacity = gm_FreeCapacity(b, ai_Slots[j].item, 1000000);
    }
    for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (!p)
            continue;
        gm_StallGoodsList(p, &rec);
        for (k = 0; k < rec.n; k++) {
            g = rec.item[k];
            if (!g)
                continue;
            found = 0;
            for (m = 0; !found && m < 3; m++) {
                if (job->in[m] == g)
                    found = 1;
            }
            if (!found) {
                for (i = 0; i < ai_NEntries; i++) {
                    if (g == ai_Entries[i].item)
                        ai_Entries[i].pending += rec.amount[k];
                }
                for (j = 0; j < ai_NSlots; j++) {
                    if (g == ai_Slots[j].item)
                        ai_Slots[j].pending += rec.amount[k];
                }
            }
        }
        for (m = 0; m < 3; m++) {
            if (job->out[m]) {
                for (i = 0; i < ai_NEntries; i++) {
                    if (job->out[m] == ai_Entries[i].item)
                        ai_Entries[i].pending += job->amount[m];
                }
                for (j = 0; j < ai_NSlots; j++) {
                    if (job->out[m] == ai_Slots[j].item)
                        ai_Slots[j].pending += job->amount[m];
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 8 || kind == 14) {
        for (job = he_FindEvent(2, 0, 0x14, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.hi)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    if (ai_BTypesT[ch->bld->type].kind == 8) {
        for (job = he_FindEvent(2, 0, 0x15, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.hi)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 29 || kind == 31) {
        for (job = he_FindEvent(2, 0, 0x8c, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.lo)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_CalcEntryCost(&ai_Entries[i]);
    for (i = 0; i < ai_NEntries; i++) {
        for (j = i; j < ai_NEntries; j++) {
            if (ai_Entries[i].margin < ai_Entries[j].margin) {
                tmp = ai_Entries[i];
                ai_Entries[i] = ai_Entries[j];
                ai_Entries[j] = tmp;
            }
        }
    }
    for (j = 0; j < ai_NSlots; j++) {
        if (ai_Slots[j].entry != -1) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Slots[j].item == ai_Entries[i].item) {
                    ai_Slots[j].flags |= 0x40;
                    ai_Entries[i].flags |= 0x40;
                    ai_Slots[j].entry = i;
                }
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_Entries[i].rank = i;
}
/* 0x486190, the workshop's slot table: the goods of two fixed lists, and what the running jobs bring in and out
   @deep Read whole against the original 0x486190..0x4864ab and rewritten: the two weapon-list loops as
   index loops over ai_WeaponList[k] / ai_WeaponList2[k] (the original walks both with ebp as k, the second
   also reading ai_WeaponList[k] for the price), the slot count n = ai_NSlots kept across the search and
   the new slot's fields stored as ai_Slots[n] (the original's [eax + ai_Slots + off] stores before the
   gm_GoodsPrice call, [esi + ...] after), amount / capacity stored at ai_Slots[ai_NSlots] after their
   calls and n = ++ai_NSlots (the original's mov ecx,[ai_NSlots] / inc ecx / mov [ai_NSlots],ecx), the
   event 0xb part with id unsigned short (mov di,word), the input search as !found && k < 3 (found tested
   after the body). Every call, constant and field matches. Left is allocation: the original keeps n in ecx
   and copies it to esi for the post-call stores, the zero in esi and both the loop pointer (ebp) and its
   copy (edi) in registers; ours keeps n in esi, the zero in edx and spills the k pointer to [esp+0x10]
   (frame 0xbc vs 0xb8, the frame / slots hunks); in the job part the original reloads ai_NSlots per item
   (+0x28f) where ours hoists it into ebp before the item loop, and the job pointer / item registers are
   rotated (ebp/ebx, di/si). */

void ai_FillSlotTable(AICHAR *ch, AIOBJ *obj)
{
    AIJOB *job;
    AIRECIPE rec;
    void *p;
    unsigned short id;
    int i, j, k, n, found;

    n = ai_NSlots;
    for (k = 0; ai_WeaponList[k] != 0; k++) {
        for (i = 0; i < n; i++) {
            if (ai_Slots[i].item == ai_WeaponList[k])
                break;
        }
        if (i < n)
            continue;
        ai_Slots[n].item = ai_WeaponList[k];
        ai_Slots[n].entry = -1;
        ai_Slots[n].bld = 0;
        ai_Slots[n].cleared10 = 0;
        ai_Slots[n].make = 0;
        ai_Slots[n].sellAmount = 0;
        ai_Slots[n].price = gm_GoodsPrice(ai_WeaponList[k], ai_OwnPlayer);
        ai_Slots[n].pending = 0;
        ai_Slots[n].amount = 0;
        ai_Slots[n].rank = 0x81;
        ai_Slots[n].flags = 0;
        ai_Slots[ai_NSlots].amount = gm_ListAmountById(obj, ai_WeaponList[k]);
        ai_Slots[ai_NSlots].capacity = gm_FreeCapacity(obj, ai_WeaponList[k], 1000000);
        n = ++ai_NSlots;
    }
    for (k = 0; ai_WeaponList2[k] != 0; k++) {
        for (i = 0; i < n; i++) {
            if (ai_Slots[i].item == ai_WeaponList2[k])
                break;
        }
        if (i < n)
            continue;
        ai_Slots[n].item = ai_WeaponList2[k];
        ai_Slots[n].entry = -1;
        ai_Slots[n].bld = 0;
        ai_Slots[n].cleared10 = 0;
        ai_Slots[n].make = 0;
        ai_Slots[n].sellAmount = 0;
        ai_Slots[n].price = gm_GoodsPrice(ai_WeaponList[k], ai_OwnPlayer);
        ai_Slots[n].pending = 0;
        ai_Slots[n].amount = 0;
        ai_Slots[n].rank = 0x81;
        ai_Slots[n].flags = 0;
        ai_Slots[ai_NSlots].amount = gm_ListAmountById(obj, ai_WeaponList2[k]);
        ai_Slots[ai_NSlots].capacity = gm_FreeCapacity(obj, ai_WeaponList2[k], 1000000);
        n = ++ai_NSlots;
    }
    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (!p)
            continue;
        gm_StallGoodsList(p, &rec);
        for (i = 0; i < rec.n; i++) {
            id = rec.item[i];
            if (id != 0) {
                found = 0;
                for (k = 0; !found && k < 3; k++) {
                    if (job->in[k] == id)
                        found = 1;
                }
                if (!found) {
                    for (j = 0; j < ai_NSlots; j++) {
                        if (id == ai_Slots[j].item)
                            ai_Slots[j].pending += rec.amount[i];
                    }
                }
            }
        }
        for (k = 0; k < 3; k++) {
            if (job->out[k] != 0) {
                for (j = 0; j < ai_NSlots; j++) {
                    if (job->out[k] == ai_Slots[j].item)
                        ai_Slots[j].pending += job->amount[k];
                }
            }
        }
    }
}
/* 0x4864b0, builds the master's two ranked trade offer tables (ai_Slots = what to buy where, ai_Entries = what
   the workshop makes / sells, both sorted best first): both counts are cleared first; with f == 0 every shop
   (the buildings 0x47 and their 0x1db store object) contributes the goods its gm_FindGoods record knows, with the
   difference to the item price (gm_ItemPrice) as the ai_Slots sort key; every building of the kind 2 (its 0x2a
   store object) contributes to both tables, with the margin (goods price - item price) as the ai_Entries sort key;
   the running jobs (he_FindEvent 0xb) add their in goods against the entries' stock (AIENTRY.pending) and the
   slots' pending pieces and their out goods likewise; then each slot's amount = min (its capacity, the entry's
   other-store capacity, budget / 4 / price), each entry's amount = min (amount, the other store's capacity), and
   both tables are selection-sorted by amount * sort key, the result positions kept in AIENTRY.rank /
   AISLOT.rank
   @deep Read whole against the original 0x4864b0..0x487035 and fixed: the shop and market price
   differences kept as one float in the FPU across the search (margin = gm_GoodsPrice - gm_ItemPrice, the
   original's fsub [esp+0x18] / fcom / fstp), the slot limit as i >= 0x80 (cmp 0x80 / jge), the kind 2
   price as gm_GoodsPrice * bld->priceRate * 0.01f (fimul [bld+0x45] / fmul dword 0.01), the building walk
   as do { ...; k++; bld = &ai_BldsT[k]; } while (k < 256) (the original's latch off += 0xa9 / lea bld),
   the input search as !found && k < 3, the entry buyAmount as min(buyerCapacity, amount). Readability:
   AIBLD.playerSlot (the kind 0x47 shop's player, was unk65), AIBLD.saved45 -> priceRate (its only reader),
   AISLOT.unk38 -> sellerAmount, AIENTRY.unk50 -> buyerCapacity, an AISLOT temporary instead of the AIENTRY
   cast. 276 -> 152 differing, 862 = 862 instructions, every call, constant and field the same. Left is
   scheduling and allocation: the order of the record stores in the four fill blocks (+0x115, +0x2f2,
   +0x371, +0x520: item / bld / fstp interleaved otherwise), the gm_Query argument pushes of the kind 2
   store at +0x43a, the not-found saving = -1e6 tail the original shares at +0x7ad, and slot rotations of
   the float temporaries ([esp+0x14] / [0x1c], [0x18] / [0x1c]). (size: 3 of the instruction difference are
   shared / inline tails) (references: M8 at +0x30a: ours refers to ai_Entries+0x4 (0xf88a44), the
   original's field to 0xf88a54 (ai_Entries+0x14) (and 1 more), explained: the two M8 lines are the new
   entry's stores in the shop part (+0x2f2..+0x33b): both sides store item, slot[0..3] = -1, margin, bld,
   store, sellAmount, cost, pending, rank, flags at the same fields; VC6 schedules our fstp margin before
   the slot stores and the item after them, the original item / slots / margin, so the alignment pairs each
   store with another field of the same record (moving the margin assignment anywhere in the source block
   gives identical code)) */

void ai_BuildTradeOffers(AICHAR *ch, AIOBJ *store, int f)
{
    AIBLD *bld;
    AIOBJ *o, *o2;
    AIOBJ *it, *it2;
    int *q;
    AIJOB *job;
    void *p;
    AIRECIPE rec;
    AIENTRY etmp;
    AISLOT stmp;
    int i, j, k, m, found;
    float price, diff;

    ai_NEntries = 0;
    ai_NSlots = 0;
    if (f == 0) {
        for (bld = gm_BuildQuery(1, 0, 0x47); bld; bld = gm_BuildNext()) {
            o = gm_Query(bld->objList, 2, 0, 0x1db, 6);
            if (o) {
                for (it = gm_Query(o->list, 1, 5); it; it = gm_QueryNext()) {
                    q = gm_FindGoods(bld->playerSlot, it->id);
                    if (q && q[1]) {
                        price = gm_GoodsPrice(it->id, (unsigned char)bld->playerSlot);
                        diff = gm_ItemPrice(it->id, 100) - price;
                        i = 0;
                        if (ai_NSlots > 0) {
                            for (; i < ai_NSlots; i++) {
                                if (ai_Slots[i].item == it->id) {
                                    if (i < ai_NSlots && diff < ai_Slots[i].saving)
                                        goto next1;
                                    break;
                                }
                            }
                            if (i >= 0x80)
                                goto next1;
                        }
                        ai_Slots[i].saving = diff;
                        ai_Slots[i].item = it->id;
                        ai_Slots[i].bld = bld;
                        ai_Slots[i].entry = -1;
                        ai_Slots[i].store = o;
                        ai_Slots[i].cleared10 = 0;
                        ai_Slots[i].make = 0;
                        ai_Slots[i].sellAmount = 0;
                        ai_Slots[i].price = price;
                        ai_Slots[i].pending = 0;
                        ai_Slots[i].rank = 0x81;
                        ai_Slots[i].flags = 0x80;
                        ai_Slots[i].amount = gm_ListAmountById(store, it->id);
                        ai_Slots[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                        ai_Slots[i].buyAmount = 0;
                        ai_Slots[i].sellerAmount = gm_ListAmountById(o, it->id);
                        if (i == ai_NSlots)
                            ai_NSlots++;
                        it2 = gm_Query(o->list, 1, 5);
                        while (it2 && it2 != it)
                            it2 = gm_QueryNext();
                    }
                next1:;
                }
                o2 = gm_Query(bld->objList, 2, 0, 0x1db, 6);
                if (o2) {
                    for (it = gm_Query(o2->list, 1, 5); it; it = gm_QueryNext()) {
                        q = gm_FindGoods(bld->playerSlot, it->id);
                        if (q && q[1]) {
                            diff = gm_ItemPrice(it->id, 100);
                            price = gm_GoodsPrice(it->id, (unsigned char)bld->playerSlot) - diff;
                            i = 0;
                            if (ai_NEntries > 0) {
                                for (; i < ai_NEntries; i++) {
                                    if (ai_Entries[i].item == it->id) {
                                        if (i < ai_NEntries && price < ai_Entries[i].margin)
                                            goto next2;
                                        break;
                                    }
                                }
                            }
                            if (ai_NEntries < 0x20) {
                                ai_Entries[i].item = it->id;
                                ai_Entries[i].slot[0] = -1;
                                ai_Entries[i].slot[1] = -1;
                                ai_Entries[i].slot[2] = -1;
                                ai_Entries[i].slot[3] = -1;
                                ai_Entries[i].margin = price;
                                ai_Entries[i].bld = bld;
                                ai_Entries[i].store = o2;
                                ai_Entries[i].sellAmount = 0;
                                ai_Entries[i].cost = 0;
                                ai_Entries[i].pending = 0;
                                ai_Entries[i].rank = 0x21;
                                ai_Entries[i].flags = 0x100;
                                ai_Entries[i].amount = gm_ListAmountById(store, it->id);
                                ai_Entries[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                                ai_Entries[i].buyAmount = 0;
                                ai_Entries[i].buyerCapacity = gm_FreeCapacity(o2, it->id, 1000000);
                                if (i == ai_NEntries)
                                    ai_NEntries++;
                                it2 = gm_Query(o->list, 1, 5);
                                while (it2 && it2 != it)
                                    it2 = gm_QueryNext();
                            }
                        }
                    next2:;
                    }
                }
            }
        }
    }
    k = 0;
    bld = ai_BldsT;
    do {
        if (AI_KIND(bld->type) == 2) {
            o = gm_Query(bld->objList, 2, 0, 0x2a, 6);
            if (o) {
                for (it = gm_Query(o->list, 1, 5); it; it = gm_QueryNext()) {
                    q = gm_FindGoods(ai_OwnPlayer, it->id);
                    if (q && q[1]) {
                        price = gm_GoodsPrice(it->id, ai_OwnPlayer) * bld->priceRate * 0.01f;
                        diff = gm_ItemPrice(it->id, 100) - price;
                        i = 0;
                        if (ai_NSlots > 0) {
                            for (; i < ai_NSlots; i++) {
                                if (ai_Slots[i].item == it->id) {
                                    if (i < ai_NSlots && diff < ai_Slots[i].saving)
                                        goto next3;
                                    break;
                                }
                            }
                            if (i >= 0x80)
                                goto next3;
                        }
                        ai_Slots[i].saving = diff;
                        ai_Slots[i].bld = bld;
                        ai_Slots[i].item = it->id;
                        ai_Slots[i].entry = -1;
                        ai_Slots[i].store = o;
                        ai_Slots[i].cleared10 = 0;
                        ai_Slots[i].make = 0;
                        ai_Slots[i].sellAmount = 0;
                        ai_Slots[i].price = price;
                        ai_Slots[i].pending = 0;
                        ai_Slots[i].rank = 0x81;
                        ai_Slots[i].flags = 0;
                        ai_Slots[i].amount = gm_ListAmountById(store, it->id);
                        ai_Slots[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                        ai_Slots[i].buyAmount = 0;
                        ai_Slots[i].sellerAmount = gm_ListAmountById(o, it->id);
                        if (i == ai_NSlots)
                            ai_NSlots++;
                        it2 = gm_Query(o->list, 1, 5);
                        while (it2 && it2 != it)
                            it2 = gm_QueryNext();
                    }
                next3:;
                }
            }
        }
        k++;
        bld = &ai_BldsT[k];
    } while (k < 256);
    for (i = 0; i < ai_NSlots; i++) {
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Slots[i].item == ai_Entries[j].item)
                break;
        }
        if (j < ai_NEntries)
            continue;
        p = gm_FindMarketObject(ai_Slots[i].item);
        if (p == 0) {
            ai_Slots[i].saving = -1000000.0f;
        } else {
            price = gm_ItemPrice(ai_Slots[i].item, 100);
            diff = gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer);
            if (ai_NEntries < 0x20) {
                ai_Entries[ai_NEntries].item = ai_Slots[i].item;
                ai_Entries[ai_NEntries].slot[0] = -1;
                ai_Entries[ai_NEntries].slot[1] = -1;
                ai_Entries[ai_NEntries].slot[2] = -1;
                ai_Entries[ai_NEntries].slot[3] = -1;
                ai_Entries[ai_NEntries].bld = ai_MarketBuilding;
                ai_Entries[ai_NEntries].store = p;
                ai_Entries[ai_NEntries].sellAmount = 0;
                ai_Entries[ai_NEntries].cost = 0;
                ai_Entries[ai_NEntries].margin = diff - price;
                ai_Entries[ai_NEntries].pending = 0;
                ai_Entries[ai_NEntries].rank = 0x21;
                ai_Entries[ai_NEntries].flags = 0;
                ai_Entries[ai_NEntries].amount = gm_ListAmountById(store, ai_Slots[i].item);
                ai_Entries[ai_NEntries].capacity = gm_FreeCapacity(store, ai_Slots[i].item, 1000000);
                ai_Entries[ai_NEntries].buyAmount = 0;
                ai_Entries[ai_NEntries].buyerCapacity = gm_FreeCapacity(p, ai_Slots[i].item, 1000000);
                ai_NEntries++;
            }
        }
    }
    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (p) {
            gm_StallGoodsList(p, &rec);
            for (i = 0; i < rec.n; i++) {
                if (rec.item[i]) {
                    found = 0;
                    for (k = 0; !found && k < 3; k++) {
                        if (job->in[k] == rec.item[i])
                            found = 1;
                    }
                    if (!found) {
                        for (m = 0; m < ai_NEntries; m++) {
                            if (rec.item[i] == ai_Entries[m].item)
                                ai_Entries[m].pending += rec.amount[i];
                        }
                        for (m = 0; m < ai_NSlots; m++) {
                            if (rec.item[i] == ai_Slots[m].item)
                                ai_Slots[m].pending += rec.amount[i];
                        }
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                if (job->out[k]) {
                    for (m = 0; m < ai_NEntries; m++) {
                        if (job->out[k] == ai_Entries[m].item)
                            ai_Entries[m].pending += job->amount[k];
                    }
                    for (m = 0; m < ai_NSlots; m++) {
                        if (job->out[k] == ai_Slots[m].item)
                            ai_Slots[m].pending += job->amount[k];
                    }
                }
            }
        }
    }
    for (i = 0; i < ai_NSlots; i++) {
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Slots[i].item == ai_Entries[j].item) {
                k = ai_Slots[i].capacity < ai_Entries[j].buyerCapacity ? ai_Slots[i].capacity : ai_Entries[j].buyerCapacity;
                ai_Slots[i].buyAmount = k;
                ai_Slots[i].buyAmount = (int)min((float)ai_Slots[i].buyAmount, (float)(ch->budget / 4) / ai_Slots[i].price);
            }
        }
    }
    for (j = 0; j < ai_NEntries; j++)
        ai_Entries[j].buyAmount = min(ai_Entries[j].buyerCapacity, ai_Entries[j].amount);
    for (i = 0; i < ai_NSlots; i++) {
        for (j = i; j < ai_NSlots; j++) {
            if ((float)ai_Slots[i].buyAmount * ai_Slots[i].saving < (float)ai_Slots[j].buyAmount * ai_Slots[j].saving) {
                stmp = ai_Slots[i];
                ai_Slots[i] = ai_Slots[j];
                ai_Slots[j] = stmp;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        for (j = i; j < ai_NEntries; j++) {
            if ((float)ai_Entries[i].buyAmount * ai_Entries[i].margin < (float)ai_Entries[j].buyAmount * ai_Entries[j].margin) {
                etmp = ai_Entries[i];
                ai_Entries[i] = ai_Entries[j];
                ai_Entries[j] = etmp;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_Entries[i].rank = i;
    for (i = 0; i < ai_NSlots; i++)
        ai_Slots[i].rank = i;
}
/* 0x487040, the production cost of an entry (a good the workshop makes), once per entry (the guard: only while
   AIENTRY.unk24 is still the -1e10 default): per needed good (the four AIENTRY.slot indices into ai_Slots) the
   slot price (gm_GoodsPrice, 90% for a good the workshop produces itself - the slot has an entry) times the need
   amount, summed and divided by the good's batch = the material cost in AIENTRY.unk28; plus the object value
   (gm_ObjectValue) in AIENTRY.unk2c = the cost AIENTRY.unk24; the sale price (gm_GoodsPrice) in AIENTRY.unk30;
   the margin per unit (price - cost) / AIGOOD.workTime in AIENTRY.margin */

void ai_CalcEntryCost(AIENTRY *e0)
{
    AIENTRY *e = (AIENTRY *)e0;
    AISLOT *s;
    int k;

    if (!(e->cost > -1e10)) {
        e->materialCost = 0;
        for (k = 0; k < 4; k++) {
            if (e->slot[k] >= 0) {
                s = &ai_Slots[e->slot[k]];
                if (!(s->price > -1e10)) {
                    if (s->entry != -1)
                        s->price = gm_GoodsPrice(s->item, ai_OwnPlayer) * 0.9;
                    else
                        s->price = gm_GoodsPrice(s->item, ai_OwnPlayer);
                }
                e->materialCost = ai_GoodsT[e->item].need[k] * s->price + e->materialCost;
            }
        }
        e->materialCost = e->materialCost / ai_GoodsT[e->item].batch;
        e->objectValue = gm_ObjectValue(e->item);
        e->cost = e->objectValue + e->materialCost;
        e->price = gm_GoodsPrice(e->item, ai_OwnPlayer);
        e->margin = (e->price - e->cost) / ai_GoodsT[e->item].workTime;
    }
}
/* 0x487180, whether the workshop can get the materials of entry e, 1 when every needed good is covered: per
   slot of the entry, when the slot's amount is below the need, f == 0 requires it in the workshop's own store
   (the slot capacity 0 counts as covered, so does a recursive check of the producing entry); with f != 0 and a
   good the workshop does not produce itself (the pending pieces 0 and none of the special items 0x1c1 .. 0x1c7 /
   0x2af / 0x2b0) it re-collects the offers of the good (gm_CollectOffers) and takes the first foreign offer (the
   master's own building, the kind-2 warehouses and offers without a goods object skipped) whose supply covers the
   need and whose buy amount - half the entry capacity, at least 1, above half the slot capacity minus the pending
   pieces, below the slot capacity - pending - 1 and the budget / price - is at least the need
   @deep B3 behaviour draft: 43 code hunks + 9 fill, all after instruction 88 - a stack-slot / block-order difference (the original keeps the row pointer of gm_Offers[j] in a slot and initializes the loop counter with a constant 0 store, ours zeroes through eax and keeps the row base in ebp: [esp+0x14]/[esp+0x18] roles swapped at instruction 179), no meaning-class lead left: statements, constants (the special item list 0x1c1 .. 0x1c7 / 0x2af / 0x2b0), callees and field offsets all match and the Q rules are clean after this session's work. Q2 done: gm_CollectOffers -> gm_CollectOffers (unified with the f3gm.c definition), gm_Offers -> gm_Offers, gm_NOffers -> gm_NOffers (the f3gm.c evidence: the offer table of 6-int rows and its count), AISTORE.unk08 -> goodsObj (the goods object of an offer), AISLOT.unk28 -> the amount member (the union alias removed). Q6 done (description above the definition). */

int ai_HasMaterials(AICHAR *ch, AIENTRY *e, int f)
{
    AISLOT *s;
    AISTORE *row;
    int k, j, found, cnt, v, m;

    for (k = 0; k < 4; k++) {
        if (e->slot[k] >= 0) {
            s = &ai_Slots[e->slot[k]];
            if (s->amount < ai_GoodsT[e->item].need[k]) {
                if (f != 0 || s->capacity == 0)
                    return 0;
                if (s->pending == 0 && s->item != 0x1c7 && s->item != 0x1c4 && s->item != 0x1c5 && s->item != 0x1c6 &&
                    s->item != 0x2af && s->item != 0x2b0 && s->item != 0x1c1 && s->item != 0x1c2 &&
                    s->item != 0x1c3) {
                    if (s->entry != -1) {
                        found = ai_HasMaterials(ch, &ai_Entries[s->entry], 0);
                    } else {
                        gm_CollectOffers(s->item);
                        j = found = 0;
                        do {
                            if (j >= gm_NOffers)
                                break;
                            row = &gm_Offers[j];
                            if (row->bld != ch->bld && AI_KIND(row->bld->type) != 2 && row->goodsObj != 0) {
                                cnt = row->count;
                                if (cnt >= ai_GoodsT[e->item].need[k]) {
                                    if (row->bld->owner != ch->bld->owner) {
                                        m = e->capacity >> 1;
                                        if (m < 1)
                                            m = 1;
                                        v = ai_GoodsT[e->item].need[j] * m - s->amount - s->pending;
                                        v = max(v, (s->capacity >> 1) - s->pending);
                                        v = min(s->capacity - s->pending - 1, v);
                                        v = (int)min((float)ch->budget / s->price, (float)v);
                                        v = min(cnt, v);
                                        if (v >= ai_GoodsT[e->item].need[j])
                                            found = 1;
                                    } else {
                                        found = 1;
                                    }
                                }
                            }
                            j++;
                        } while (!found);
                    }
                    if (!found)
                        return 0;
                }
            }
        }
    }
    return 1;
}
/* 0x487410, starts the production the workshop entry e needs, returns 1: per slot of the entry the slot gets the
   entry's rank and flag 2 (selected); a special item (a good the workshop makes itself, 0x1c1 .. 0x1c7 / 0x2af /
   0x2b0) gets flag 0x10 when its entry's store has room (4 x pending + amount below the capacity) and the slot
   holds / expects less than 4 x the needed amount; a slot assigned to an entry whose store has room starts that
   entry's production (recursively) and flags the entry 8 when the store holds a whole batch; a slot without an
   entry whose pending + stock is below 4 x the need collects the goods' offers (gm_CollectOffers) and scans
   them: an offer of a foreign building (not the master's, not a kind-2 warehouse) with a goods object that
   supplies at least the need becomes the slot's bld, an own one fills the slot's make to the capacity, the else
   buy amount - half the entry capacity at least 1, at least half the slot capacity minus the pending, below the
   capacity - pending - 1, at most the budget / price and the offer's count - goes into make when it covers the
   need
   @deep behaviour draft, 2 code hunks only, both the block layout of the item chain: the original keeps the special-item block inline after the 15 je (0x4874cb..0x48751b) and puts the entry!=-1 and the offer-scan blocks OUT of line after the loop epilogue/ret (0x48754a, 0x4875e7), ours emits the entry block inline and the special block at $L4532 before the loop tail; 8 slots hunks are the k / j / row slot choice (k 0x18 vs 0x10, j 0x10 vs 0x18, row 0x14 vs 0x1c). Read whole against tools/disasm.py 0x487410 0x370, Ghidra build/ghidra/487410.c and the VC6 listing; statements, constants, callees, field offsets and the item list order all match. Last-bytes pass, one edit one build each, all measured byte-identical (183 asm diff lines, 129 code / 68 slots hunks unchanged): (1) positive == || chain with the special block as then-branch and the entry/offer blocks as else-if, (2) nested else (if (special) {} else { if (entry != -1) .. else if (..) }), (3) special block + continue then a separate if / else if. spell.py branch / dup-tail / reload / slots entries: dup-tail 4877ac is the padding + next function in the window; slots swap of the k / j declaration order tried, identical. Readability done: Q2 AIENTRY.unk38 -> pending, Q6 description. Left: allocation / scheduling / block ordering and the stack slot choice = match-deep */

int ai_StartProduction(AICHAR *ch, AIENTRY *e)
{
    AISLOT *s;
    AIENTRY *e2;
    AISTORE *row;
    int k, j, cnt, v, m;

    for (k = 0; k < 4; k++) {
        if (e->slot[k] >= 0) {
            s = &ai_Slots[e->slot[k]];
            s->rank = s->rank < e->rank ? s->rank : e->rank;
            s->flags |= 2;
            if (s->item != 0x1c7 && s->item != 0x1c4 && s->item != 0x1c5 && s->item != 0x1c6 &&
                s->item != 0x2af && s->item != 0x2b0 && s->item != 0x1c1 && s->item != 0x1c2 &&
                s->item != 0x1c3 && s->item != 0x225 && s->item != 0x226 && s->item != 0x227 &&
                s->item != 0x293 && s->item != 0x294 && s->item != 0x295) {
                if (s->entry != -1) {
                    ai_Entries[s->entry].rank = ai_Entries[s->entry].rank < e->rank ? ai_Entries[s->entry].rank : e->rank;
                    ai_Entries[s->entry].flags |= 2;
                    if ((ai_Entries[s->entry].pending + ai_Entries[s->entry].amount) * 4 < ai_Entries[s->entry].capacity) {
                        if (ai_Entries[s->entry].capacity > ai_GoodsT[ai_Entries[s->entry].item].batch)
                            ai_Entries[s->entry].flags |= 8;
                        ai_StartProduction(ch, &ai_Entries[s->entry]);
                    }
                } else if (s->pending + s->amount < ai_GoodsT[e->item].need[k] * 4) {
                    gm_CollectOffers(s->item);
                    j = 0;
                    while (1) {
                        if (j >= gm_NOffers)
                            break;
                        row = &gm_Offers[j];
                        if (row->bld != ch->bld && AI_KIND(row->bld->type) != 2 && row->goodsObj != 0) {
                            cnt = row->count;
                            if (cnt >= ai_GoodsT[e->item].need[k]) {
                                s->bld = row->bld;
                                if (row->bld->owner == ch->bld->owner) {
                                    s->make = s->capacity;
                                    j++;
                                    continue;
                                }
                                m = e->capacity >> 1;
                                if (m < 1)
                                    m = 1;
                                v = ai_GoodsT[e->item].need[k] * m - s->pending - s->amount;
                                v = max(v, (s->capacity >> 1) - s->pending);
                                v = min(s->capacity - s->pending - 1, v);
                                v = (int)min((float)ch->budget / s->price, (float)v);
                                v = min(cnt, v);
                                if (v >= ai_GoodsT[e->item].need[k])
                                    s->make = max(s->make, v);
                            }
                        }
                        j++;
                    }
                }
            } else {
                if ((ai_Entries[s->entry].pending + ai_Entries[s->entry].amount) * 4 < ai_Entries[s->entry].capacity &&
                    s->amount + s->pending < ai_GoodsT[e->item].need[k] * 4)
                    s->flags |= 0x10;
            }
        }
    }
    return 1;
}
/* 0x487780, adds the equipment the building's characters need (their object list at 0x178, queried for the
   prototype kind 0xc, matched against ai_WeaponList / ai_WeaponList2) to the slot table with the mark
   `make` and the flag 2, then reduces the marks until make * price fits the building's limit (0x39) @deep Read whole against the original and fixed: the inner slot searches as for (j = 0; !found && j < n; j++) (cmp j,n / jge, the found pre-test), the need reduction as make -= pending + amount then a clamp at 0 (the original's two stores of make), the offer filter count >= 1 (cmp 1 / jl); the !found tests after the weapon-list loops moved out of the break branch. Left are optimizer decisions no source variant changed (do-while with the test inside or after the loop, for(;;) with a top break, for with the bound: all compile to the same object): ours folds the found tests VC6 kept in the original (+0x487855 test edx,edx / jne, +0x48788b test edx,edx / jne, +0x487965 test eax,eax / jne: found is provably 0 there), rebases the walk pointer (ours [edi] / [edi+4] from ai_WeaponList, the original [edi-4] / [edi] from &ai_WeaponList[1]), turns the offer loop into a countdown (ours test ecx / jle .. dec ecx / jne, the original xor ecx / cmp ecx,edx / jge .. inc ecx / jmp), and allocates the final list loops' id / count in ebp / edi where the original has edi / ebp. */

void ai_AddEquipmentNeeds(AICHAR *ch, AIOBJ *obj, int limit)
{
    unsigned short *it;
    int *p;
    int i, j, k, n, sum, id, found;
    unsigned int u;

    n = ai_NSlots;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title != 0) {
            it = (unsigned short *)gm_Query(ai_CharsT[i].inventory, 1, 4, 0xc);
            n = ai_NSlots;
            if (it == 0) {
                for (j = 0; j < n; j++) {
                    if (ai_Slots[j].item == ai_WeaponList[0]) {
                        ai_Slots[j].make++;
                        ai_Slots[j].flags |= 2;
                        break;
                    }
                }
            } else {
                found = 0;
                p = &ai_WeaponList[1];
                do {
                    if (p[-1] == 0 || (id = *p) == 0)
                        break;
                    u = *it;
                    p[-1] = u;
                    if (u != 0) {
                        for (j = 0; !found && j < n; j++) {
                            if (ai_Slots[j].item == id) {
                                ai_Slots[j].make++;
                                ai_Slots[j].flags |= 2;
                                found = 1;
                            }
                        }
                    }
                    p++;
                } while (!found);
                if (!found) {
                    p = &ai_WeaponList2[1];
                    do {
                        if (p[-1] == 0 || (id = *p) == 0)
                            break;
                        u = *it;
                        p[-1] = u;
                        if (u != 0) {
                            for (j = 0; !found && j < n; j++) {
                                if (ai_Slots[j].item == id) {
                                    ai_Slots[j].make++;
                                    ai_Slots[j].flags |= 2;
                                    found = 1;
                                }
                            }
                        }
                        p++;
                    } while (!found);
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (ai_Slots[i].make != 0) {
            found = 0;
            p = ai_WeaponList;
            do {
                if (*p == 0)
                    break;
                if (ai_Slots[i].item == *p) {
                    ai_Slots[i].make -= ai_Slots[i].pending + ai_Slots[i].amount;
                    ai_Slots[i].make = ai_Slots[i].make < 0 ? 0 : ai_Slots[i].make;
                    found = 1;
                }
                p++;
            } while (!found);
            if (!found) {
                p = ai_WeaponList2;
                do {
                    if (*p == 0)
                        break;
                    if (ai_Slots[i].item == *p) {
                        ai_Slots[i].make -= ai_Slots[i].pending + ai_Slots[i].amount;
                        ai_Slots[i].make = ai_Slots[i].make < 0 ? 0 : ai_Slots[i].make;
                        found = 1;
                    }
                    p++;
                } while (!found);
            }
            if (ai_Slots[i].make != 0) {
                gm_CollectOffers(ai_Slots[i].item);
                for (k = 0;; k++) {
                    if (k >= gm_NOffers)
                        break;
                    if (gm_Offers[k].goodsObj != 0 && gm_Offers[k].count >= 1)
                        ai_Slots[i].bld = gm_Offers[k].bld;
                }
                n = ai_NSlots;
            }
            if (ai_Slots[i].bld == 0)
                ai_Slots[i].make = 0;
        }
    }
    sum = 0;
    for (p = ai_WeaponList; *p != 0; p++) {
        id = *p;
        for (j = 0; j < ai_NSlots; j++) {
            if (id == ai_Slots[j].item && ai_Slots[j].make != 0) {
                if (ai_Slots[j].make > 0) {
                    do {
                        if (!((float)ai_Slots[j].make * ai_Slots[j].price + (float)sum > (float)limit))
                            break;
                    } while (--ai_Slots[j].make > 0);
                }
                sum = (int)(ai_Slots[j].make * ai_Slots[j].price + sum);
            }
        }
    }
    for (p = ai_WeaponList2; *p != 0; p++) {
        id = *p;
        for (j = 0; j < ai_NSlots; j++) {
            if (id == ai_Slots[j].item && ai_Slots[j].make != 0) {
                if (ai_Slots[j].make > 0) {
                    do {
                        if (!((float)ai_Slots[j].make * ai_Slots[j].price + (float)sum > (float)limit))
                            break;
                    } while (--ai_Slots[j].make > 0);
                }
                sum = (int)(ai_Slots[j].make * ai_Slots[j].price + sum);
            }
        }
    }
}
/* 0x487b40, the staff update of the building ch->bld at the clock's hours 7, 12 and 18 (once each: flags1b4 bit
   0x40, cleared at any other hour): for a building of a player (owner kind 6 / 7) or one whose flags1b4 bit 2 is
   set, it tells the owner (message 0x32 from the building) the titled characters of the building whose job days
   (v174) have reached 4, counts the building's titled characters plus the nonzero ware counters of its kind 6
   sales-tour event, and hires a title-1 worker (message 6 from the building, arg[1] 1, arg[0] the type's title byte
   k230) when the count is below the type's k231 + k232 - the existing staff (flags1c8 bit 8), the master's budget
   and a random 0x48 roll permitting; the hire costs gm_JobCost(owner, type, -1, 1), taken from the master's budget */

void ai_UpdateStaff(AICHAR *ch)
{
    AIMSG msg;
    AIBTYPE *bt;
    AICHAR *oc;
    AIJOB *job;
    int *jp;
    unsigned int fl;
    int i, n, k, v;
    unsigned int t;

    if (ai_Time.hour != 7 && ai_Time.hour != 0xc && ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x40;
        return;
    }
    fl = ch->flags1b4;
    if (!(fl & 0x40)) {
        fl |= 0x40;
        ch->flags1b4 = fl;
        if ((ai_CharsT[ch->bld->owner].kind != 6 && ai_CharsT[ch->bld->owner].kind != 7) || (fl & 2)) {
            bt = &ai_BTypesT[ch->bld->type];
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title != 0 &&
                    ai_CharsT[i].title == (char)bt->k230 && ai_CharsT[i].v174 >= 4) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x32;
                    msg.to = ai_CharsT[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    msg.arg[0] = ai_CharsT[i].netId;
                    cm_SendLongMessage(&msg);
                }
            }
            n = 0;
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title != 0)
                    n++;
            }
            job = he_FindEvent(2, 0, 6, 3, ch->bld->id);
            if (job != 0) {
                jp = job->tourCounters;
                for (k = 0; k < 12; k++) {
                    if (*jp != 0)
                        n++;
                    jp++;
                }
            }
            t = ch->bld->type;
            if (n < ai_BTypesT[t].k231 + ai_BTypesT[t].k232) {
                oc = &ai_CharsT[ch->bld->owner];
                v = (int)gm_JobCost(oc, t, -1, 1);
                if (n != 0) {
                    if (ch->flags1c8 & 8)
                        return;
                    if (ch->budget < v)
                        return;
                    if (util_RandomU(0x48) < (0x12 - n) * 2)
                        return;
                }
                if (job == 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 6;
                    msg.to = ai_CharsT[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.flags = 1;
                    msg.time = ai_Time;
                    msg.arg[1] = 1;
                    gm_IsHomeType(ch->bld);
                    msg.amount[1] = -1;
                    ((unsigned char *)&msg.arg[0])[1] = 1;
                    ((unsigned char *)&msg.arg[0])[0] = ai_BTypesT[ch->bld->type].k230;
                    cm_SendLongMessage(&msg);
                    ch->budget -= v;
                }
            }
        }
    }
}
/* 0x487ea0, the undertaker's production step: at the clock's hours 7, 12 and 18 (once each: flags1b4 bit 0x40,
   cleared at any other hour), for a building of a player (owner kind 6 / 7) or one whose flags1b4 bit 2 is set, it
   offers a grave of the building (gv_SendGrave of a random grave) when the building's titled characters are fewer
   than the type's k231 + k232 staff limits; an existing titled staff (flags1c8 bit 8, unless there is none) and a
   random 0x48 roll against (0x12 - the count) * 2 can cancel the offer */

void ai_OfferGrave(AICHAR *ch)
{
    AIBLD *bld;
    unsigned int fl;
    int i, n;

    if (ai_Time.hour == 7 || ai_Time.hour == 0xc || ai_Time.hour == 0x12) {
        fl = ch->flags1b4;
        if (!(fl & 0x40)) {
            bld = ch->bld;
            fl |= 0x40;
            ch->flags1b4 = fl;
            if ((ai_CharsT[bld->owner].kind != 6 && ai_CharsT[bld->owner].kind != 7) || (fl & 2)) {
                n = 0;
                for (i = 0; i < 768; i++) {
                    if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == bld && ai_CharsT[i].title != 0)
                        n++;
                }
                if (n < ai_BTypesT[bld->type].k231 + ai_BTypesT[bld->type].k232) {
                    if (n == 0 || (!(ch->flags1c8 & 8) && util_RandomU(0x48) >= (0x12 - n) * 2))
                        gv_SendGrave(ch->bld);
                }
            }
        }
    } else {
        ch->flags1b4 &= ~0x40;
    }
}
/* 0x487fb0, buys a renovation of the building or an upgrade of the workshop @deep behaviour draft, now near: 52 code / 10 regs / 6 order / 6 fill hunks = 48 differing instructions of 361 (baseline 99). This session's fix: the two slot loops were a do-while over a hand-walked p with a separate i - VC6 spilled BOTH (frame 0x200 vs 0x1fc) and the loop counter went to the frame; rewritten as for (i = 0; i < bt->nslots; i++) with bt->slot[i] (index only) the frame matched and 99 -> 48 differing instructions in one edit (code 151 -> 52, every slots hunk gone). Tried and reverted / identical, one edit one build each: all-c->bld (81 code hunks, worse), all-bld (377, worse), int n (identical), dropping the redundant AICHAR *c = (AICHAR *)ch alias (identical, the cleaner ch form kept). What is left, all read against tools/disasm.py 0x487fb0 0x4c0 and build/ghidra/487fb0.c: the register pair of the building pointer (original edi, ours ebp; knock-on hunks at 0x487fc0 zero register, 0x488060 the lim registers, 0x4881c3 test vs cmp against the zero register, 0x488217 the re-zero, 0x48824c / 0x4882e3 the ai_Goods[n] address form), the store order at 0x488101 and the 3-instruction dup-tail at 0x4884aa; every statement, constant, callee, field offset and the float block (0.005 * 0.005, max 128, budget * 0.75) match Ghidra. Readability done this session: Q2 gm_PlayerRecord -> gm_PlayerRecord (unified with the f3gm.c definition), gm_BuildingLevel -> gm_BuildingLevel (f3gm.c definition renamed too; it returns the building's level byte +0x5c), gm_EquipmentValue -> gm_EquipmentValue (the f3gm.c gm_EquipmentValue definition and every ai_ alias), AIBLD.unk25 -> masterId (am.c 0x497cf0 looks the AI's buildings up by it, the upgrade message goes to that character), AIBLD.level -> level (0..100, renovation threshold, 100 - level in the renovation message), AIFIN.unk40 / unk44 -> upgradeSpent / renovationSpent; Q4 *(unsigned short *)&msg2.arg[0] = ret -> the new AIMSG.itemId union view; Q6 already there */

void ai_BuyUpgrade(AICHAR *ch)
{
    AIBLD *bld;
    AIBTYPE *bt;
    AIOBJ *o;
    AIFIN *fin;
    AIMSG msg, msg2;
    unsigned short *p;
    float f;
    unsigned int flags;
    int i, cost, ret;
    unsigned int n;
    unsigned short rv;
    int lim;
    int first;

    i = 0;
    ret = 0;
    first = 0xfd;
    if (ch->flags1c8 & 8)
        return;
    if (ai_Time.hour != 7 && ai_Time.hour != 0xc && ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x400;
        return;
    }
    flags = ch->flags1b4;
    if (flags & 0x400)
        return;
    bld = ch->bld;
    flags |= 0x400;
    ch->flags1b4 = flags;
    if (ai_CharsT[bld->owner].kind != 6 && ai_CharsT[bld->owner].kind != 7)
        return;
    if (!(flags & 8))
        return;
    rv = util_RandomU(0x14);
    lim = rv + 0x4b;
    if (gm_BuildingLevel((AIBLD *)bld) < lim) {
        f = (float)gm_EquipmentValue(ch->bld, 100) * 0.005f;
        f = f * 0.005f;
        if (128.0f > f)
            f = 128.0f;
        if ((float)ch->budget * 0.75f > f) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x1c;
            msg.fromId = -1;
            msg.flags = 1;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.arg[0] = ch->bld->id;
            msg.arg[1] = 100 - ch->bld->level;
            cm_BeginGroup("renovieren");
            cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)f);
            cm_SendLongMessage(&msg);
            cm_EndGroup("renovieren");
            fin = gm_PlayerRecord(&ai_CharsT[ch->bld->owner]);
            if (fin)
                fin->renovationSpent = (int)(fin->renovationSpent + f);
            ch->budget = (int)(ch->budget - f);
        }
    }
    bt = &ai_BTypesT[ch->bld->type];
    if (bt->nslots > 0) {
        for (i = 0; i < bt->nslots; i++) {
            if (ret)
                goto found;
            if ((bt->slot[i] & 0x7fff) != 0) {
                n = bt->slot[i] & 0x7fff;
                if (ai_GoodsT[n].type == 2 || ai_GoodsT[n].type == 6)
                    first = n;
                else if (first != 0xfd && !gm_Query(ch->bld->objList, 2, 6, 0, n))
                    ret = bt->slot[i] & 0x7fff;
            }
        }
        if (ret)
            goto found;
    }
    i = 0;
    first = 0;
    if (bt->nslots <= 0)
        return;
    for (i = 0; i < bt->nslots; i++) {
        if (ret)
            goto found;
        if ((bt->slot[i] & 0x7fff) != 0) {
            n = bt->slot[i] & 0x7fff;
            if (ai_GoodsT[n].type == 2 || ai_GoodsT[n].type == 6)
                first = n;
            else if (first == 0xfd && !gm_Query(ch->bld->objList, 2, 6, 0, n))
                ret = bt->slot[i] & 0x7fff;
        }
    }
    if (!ret)
        return;
found:
    o = gm_Query(ch->bld->objList, 1, 0, first);
    if (o) {
        cost = (int)gm_ItemPrice(ret, 100);
        if (ch->budget > cost * 3) {
            memset(&msg2, 0, sizeof(msg2));
            msg2.type = 2;
            msg2.to = ai_CharsT[ch->bld->masterId].netId;
            msg2.fromId = ch->bld->id;
            msg2.time = ai_Time;
            msg2.flags = 1;
            msg2.itemId = ret;
            msg2.arg[1] = o->objId;
            msg2.arg[3] = 1;
            fin = gm_PlayerRecord(&ai_CharsT[ch->bld->masterId]);
            if (fin)
                fin->upgradeSpent += cost;
            cm_BeginGroup("upgrade_meister_ai");
            cm_SendMoveGoods(-1, ai_CharsT[ch->bld->masterId].netId, ai_OwnPlayer, cost);
            cm_SendLongMessage(&msg2);
            cm_EndGroup("upgrade_meister_ai");
            ch->budget -= cost;
        }
    }
    ch->flags1c8 |= 8;
}
/* 0x4884b0, buys more slots of the building */

void ai_BuySlots(AIOBJ *b, AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIJOB *job;
    AIBLD *bld;
    AIMSG msg;
    char buf[1024];
    int a, d, ma, mb, x, y;
    int cnt, r;

    if (b && !(c->flags1c8 & 8) && (int)util_RandomU(100) >= n) {
        a = b->lvl1c;
        d = b->lvl1d;
        for (job = he_FindEvent(2, 0, 0x12, 3, c->bld->id); job; job = he_NextEvent()) {
            a += job->slotOrder1c;
            d += job->slotOrder1d;
        }
        bld = c->bld;
        mb = ai_BTypesT[bld->type].min1;
        y = mb - d;
        ma = ai_BTypesT[bld->type].min0;
        x = ma - a;
        if ((0 < x || 0 < y) && c->budget >= 24000) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x12;
            msg.to = ai_CharsT[bld->owner].netId;
            msg.fromId = bld->id;
            msg.flags = 1;
            msg.time = ai_Time;
            msg.arg[0] = b->objId;
            msg.arg[1] = x != 0;
            msg.arg[2] = y != 0;
            cnt = msg.arg[2] + msg.arg[1];
            msg.arg[3] = 1;
            cm_BeginGroup("AI-Slots");
            cm_SendLongMessage(&msg);
            cm_SendMoveGoods(-1, ai_CharsT[c->bld->owner].netId, ai_OwnPlayer, 8000);
            switch (ls_GetLanguage()) {
            case 0:
                if (cnt == 1)
                    txt_Format(buf, 0x1bbc, c->id, c->bld);
                else
                    txt_Format(buf, 0x1bbd, c->id, c->bld, cnt);
                break;
            case 1:
                if (cnt == 1)
                    txt_Format(buf, 0x1bbc, c->id, c->bld);
                else
                    txt_Format(buf, 0x1bbd, c->id, c->bld, cnt);
                break;
            }
            he_MessageBoxQuickjumpI(ai_CharsT[c->bld->owner].netId, ai_CharsT[c->bld->owner].netId, buf, 0, 0x60b,
                         c->bld->id, -1, 0, 0);
            c->budget += cnt * -8000;
            cm_EndGroup("AI-Slots");
            c->flags1c8 |= 8;
        }
    }
}
/* 0x488740, buys a capacity upgrade */

void ai_BuyCapacity(AIOBJ *b, AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIMSG msg;

    if (b && !(c->flags1c8 & 8)) {
        if ((int)util_RandomU(100) >= n) {
            if (!he_FindEvent(2, 0, 0x13, 3, c->bld->id) && b->count < 3 && c->budget >= 0x9600) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x13;
                msg.to = ai_CharsT[c->bld->owner].netId;
                msg.fromId = c->bld->id;
                msg.flags = 1;
                msg.time = ai_Time;
                msg.arg[0] = b->objId;
                msg.arg[1] = 1;
                msg.arg[3] = 1;
                cm_BeginGroup("AI-kapazit\344t");
                cm_SendLongMessage(&msg);
                cm_SendMoveGoods(-1, ai_CharsT[c->bld->owner].netId, ai_OwnPlayer, 0x3200);
                c->budget += -0x3200;
                cm_EndGroup("AI-kapazit\344t");
                c->flags1c8 |= 8;
            }
        }
    }
}
/* 0x4888c0, frees the characters of the jobs of one kind whose entries are done */

void ai_FreeJobCharacters(AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIJOB *job;
    int i, found;

    for (job = he_FindEvent(2, 0, n, 3, c->bld->id); job; job = he_NextEvent()) {
        found = 0;
        if (n == 4) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 8))
                    found = 1;
            }
        } else if (n == 8) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 0x10))
                    found = 1;
            }
        } else if (n == 3) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 8))
                    found = 1;
            }
        }
        if (found) {
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].job == job)
                    sv_MoveToGroup(c->bld, 0, ai_CharsT[i].id, 0);
            }
        }
    }
}
/* 0x4889f0, sends the master's order of one kind (info->type) for the building: 0x28 is a harvest order (the
   plants of the building's vegetation map are counted down, the free grown records get one harvest job each; a
   full char table or no free plant record ends the round), 0x14 / 0x15 are goods orders (the entry's item and
   the building's main object go into the message, the spare characters of the building are collected), 0x8c and
   the trade kinds 3 / 4 / 8 name the carts' objects (info->b / info->c) and the building in the message.
   Side effects: info->z2 counts what was sent (also across calls), info->z1 is set as the round flag; the order
   message goes as one cm_SendLongMessage (0x28: one per free character, then the round ends).
   @deep Read whole against the original 0x4889f0..0x4892f4 and rewritten: every worker loop as for (i = 0;
   info->z2 < total && i < 768; i++) (the original's z2 test before the loop and at the bottom, i < 768 at
   the top), the harvest plant search as !found && k < 64 over the G15PLANT records with the plant id taken
   before the memset, msg.nIds / reserved14 stored before the time, the 0x8c count with base = info->z2
   loaded before the division, the trade kinds tested on info->type itself (the original reloads it and
   does not thread the 4 / 8 / 3 tests), the goods orders' count / item through the new AIMSG.goodsOrder
   view (was the *(unsigned char *) / *((unsigned short *) + 1) casts on arg[0]), AIJOB.harvestPlant for
   unkb8. 327 -> 130 differing; every call, constant and field the same. Left: the original keeps a third
   send site for 0x8c (ours cross-jumps it), the 0x14 / found-default total keeps its add per branch in the
   original (ours shares the final add ebp,eax: +0x22e, +0x82c; z2 + n2 % n + n2 / n spelled either way
   compiles identical), the zero register (ebx / ebp) choices and the message stores' scheduling (+0x138 ..
   +0x1b9, +0x725 ..). */

void ai_SendJobOrders(AIINFO *info)
{
    AIMSG msg;
    AIJOB *job;
    AIOBJ *o;
    G15PLANT *map, *pl, *found;
    int type, n, total, base, i, k, v;

    type = info->type;
    if (type == 0x28) {
        n = info->n1;
        map = (G15PLANT *)info->ch->bld->plants;
        if (n <= 1 || info->z1 != 0) {
            total = info->n2 / n;
            base = info->z2;
        } else {
            base = info->n2 / n;
            total = info->z2 + info->n2 % n;
        }
        total += base;
        for (job = he_FindEvent(2, 0, 0x28, 3, info->ch->bld->id); job; job = he_NextEvent()) {
            pl = veg_FindPlantId(map, job->harvestPlant);
            if (pl && pl->type == info->e->item)
                total--;
        }
        for (i = 0; info->z2 < total && i < 768; i++) {
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == info->ch->bld && ai_CharsT[i].title && !ai_CharsT[i].job
                && ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                found = 0;
                pl = map;
                for (k = 0; !found && k < 64; k++, pl++) {
                    if (pl->state > 1 && pl->fallId == -1)
                        found = pl;
                }
                if (!found)
                    return;
                k = found->id;
                memset(&msg, 0, sizeof(msg));
                msg.type = info->type;
                msg.to = ai_CharsT[info->ch->bld->owner].netId;
                msg.fromId = info->ch->bld->id;
                msg.nIds = 1;
                msg.reserved14 = 0;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[3] = k;
                msg.arg[0] = info->ch->bld->id;
                msg.arg[1] = ai_CharsT[i].netId;
                cm_SendLongMessage(&msg);
                info->z2++;
            }
        }
    } else if (type == 0x14) {
        n = info->n1;
        if (n <= 1 || info->z1 != 0)
            total = info->n2 / n + info->z2;
        else
            total = info->z2 + info->n2 / n + info->n2 % n;
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_CharsT[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.time = ai_Time;
        msg.nIds = 0;
        msg.reserved14 = 0;
        msg.flags = 1;
        msg.goodsOrder.item = info->e->item;
        o = (AIOBJ *)gm_BuildingMainObject(info->ch->bld);
        msg.arg[1] = o->objId;
        for (i = 0; info->z2 < total && i < 768; i++) {
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == info->ch->bld && ai_CharsT[i].title && !ai_CharsT[i].job
                && ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                msg.ids[msg.nIds] = ai_CharsT[i].netId;
                msg.nIds++;
                info->z2++;
            }
        }
        msg.goodsOrder.count = (unsigned char)msg.nIds;
        if (msg.nIds != 0 && msg.goodsOrder.item != 0) {
            cm_SendLongMessage(&msg);
            info->z1++;
            return;
        }
    } else if (type == 0x15) {
        n = info->n1;
        if (n <= 1 || info->z1 != 0) {
            total = info->n2 / n;
            base = info->z2;
        } else {
            base = info->n2 / n;
            total = info->z2 + info->n2 % n;
        }
        total += base;
        o = gm_Query(info->ch->bld->objList, 2, 6, 0, 0xdd);
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_CharsT[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.nIds = 0;
        msg.time = ai_Time;
        msg.reserved14 = 0;
        msg.flags = 1;
        msg.goodsOrder.item = info->e->item;
        if (o) {
            msg.arg[2] = o->objId;
        } else {
            o = (AIOBJ *)gm_BuildingMainObject(info->ch->bld);
            if (o)
                msg.arg[2] = o->objId;
            else
                msg.arg[2] = -1;
        }
        for (i = 0; info->z2 < total && i < 768; i++) {
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == info->ch->bld && ai_CharsT[i].title && !ai_CharsT[i].job
                && ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                msg.ids[msg.nIds] = ai_CharsT[i].netId;
                msg.nIds++;
                info->z2++;
            }
        }
        msg.goodsOrder.count = (unsigned char)msg.nIds;
        if (msg.nIds != 0 && msg.goodsOrder.item != 0) {
            cm_SendLongMessage(&msg);
            info->z1++;
            return;
        }
    } else if (type == 0x8c) {
        gm_Query(info->ch->bld->objList, 1, 0, 0xfd);
        n = info->n1;
        base = info->z2;
        if (n <= 1 || info->z1 != 0)
            total = info->n2 / n + base;
        else
            total = info->n2 % n + base + info->n2 / n;
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_CharsT[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.time = ai_Time;
        msg.nIds = 0;
        msg.reserved14 = 0;
        msg.flags = 1;
        msg.itemId = info->e->item;
        if (base < total) {
            for (i = 0; info->z2 < total && i < 768; i++) {
                if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == info->ch->bld && ai_CharsT[i].title
                    && !ai_CharsT[i].job && ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                    msg.ids[msg.nIds] = ai_CharsT[i].netId;
                    msg.nIds++;
                    info->z2++;
                }
            }
            if (msg.nIds != 0 && msg.itemId != 0) {
                cm_SendLongMessage(&msg);
                info->z1++;
                return;
            }
        }
    } else {
        for (job = he_FindEvent(2, 0, type, 3, info->ch->bld->id); job; job = he_NextEvent()) {
            if ((info->type == 4 && job->u.w.lo == (unsigned int)info->e->item) ||
                (info->type == 8 && job->u.w.lo == (unsigned int)info->e->item) ||
                (info->type == 3 && job->u.w.lo == (unsigned int)info->e->item)) {
                n = info->n1;
                if (n <= 1 || info->z1 != 0)
                    total = info->n2 / n + info->z2;
                else
                    total = info->z2 + info->n2 / n + info->n2 % n;
                k = info->type;
                v = 0;
                if (k == 8 || k == 3)
                    v = (int)gm_Query(info->ch->bld->objList, 1, 0, 0x37);
                else if (k == 4)
                    v = gm_BuildingMainObject(info->ch->bld);
                for (i = 0; info->z2 < total && i < 768; i++) {
                    if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == info->ch->bld && ai_CharsT[i].title
                        && !ai_CharsT[i].job) {
                        sv_MoveToGroup(info->ch->bld, v, ai_CharsT[i].id, job);
                        info->z2++;
                    }
                }
                goto done;
            }
        }
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_CharsT[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.time = ai_Time;
        msg.nIds = 0;
        msg.reserved14 = 0;
        msg.flags = 1;
        if (info->type == 4) {
            msg.arg[1] = -1;
            msg.itemId = info->e->item;
            msg.arg[4] = info->ch->bld->id;
            msg.arg[5] = info->b->objId;
        }
        if (info->type == 8) {
            msg.itemId = info->e->item;
            msg.arg[1] = info->ch->bld->id;
            msg.arg[2] = info->c->objId;
        }
        if (info->type == 3) {
            msg.itemId = info->e->item;
            msg.arg[3] = info->ch->bld->id;
            msg.arg[5] = info->b->objId;
            msg.arg[4] = info->c->objId;
        }
        cm_SendLongMessage(&msg);
    }
done:
    info->z1++;
}
/* 0x489300, a thief master: are there free characters, and what do they know */

int ai_CheckFreeThieves(AICHAR *ch)
{
    AICHAR *c = (AICHAR *)ch;
    int i, found;

    if (c->flags1c8 & 4)
        return 0;
    found = 0;
    c->flags1c8 |= 4;
    i = 0;
    while (1) {
        if (i >= 768)
            break;
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title && ai_CharsT[i].job == 0 && !(ai_CharsT[i].flags1c8 & 4) &&
            ((float)ai_CharsT[i].sk83 < 168.0f || (float)ai_CharsT[i].sk82 < 168.0f))
            found = 1;
        i++;
        if (found)
            break;
    }
    if (found && util_RandomU(0x80) < 0x20)
        return 0;
    if (!found) {
        if (((float)ai_CharsT[i].sk83 < 252.0f || (float)ai_CharsT[i].sk82 < 252.0f) && util_RandomU(0x80) > 0x60)
            return 0;
    }
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title && ai_CharsT[i].job == 0 && !(ai_CharsT[i].flags1c8 & 4))
            ai_CharsT[i].flags1b4 |= 0x10;
    }
    return 1;
}
/* 0x489480, the three-skill master's free-character check: it marks the building's titled characters whose job is
   none or of the kinds 4 / 0x8c (a building or a helper job) and whose skills sk81, sk82 or sk84 are below 168 with
   the free mark (flags1b4 bit 0x10) that the worker orders read, unless a 0x80 roll cancels it (below 0x60 when one
   is found, above 0x20 when none is); 1 when the marks were set, 0 when the master already checked (flags1c8 bit 4)
   or the roll did not pass */

int ai_CheckFreeStaff(AICHAR *ch)
{
    AICHAR *c = (AICHAR *)ch;
    int i, found;

    if (c->flags1c8 & 4)
        return 0;
    found = 0;
    c->flags1c8 |= 4;
    i = 0;
    while (1) {
        if (i >= 768)
            break;
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title &&
            (ai_CharsT[i].job == 0 || ai_CharsT[i].job->kind == 4 || ai_CharsT[i].job->kind == -0x74) && !(ai_CharsT[i].flags1c8 & 4) &&
            ((float)ai_CharsT[i].sk81 < 168.0f || (float)ai_CharsT[i].sk82 < 168.0f || (float)ai_CharsT[i].sk84 < 168.0f))
            found = 1;
        i++;
        if (found)
            break;
    }
    if (found && util_RandomU(0x80) < 0x60)
        return 0;
    if (!found) {
        if (((float)ai_CharsT[i].sk81 < 252.0f || (float)ai_CharsT[i].sk82 < 252.0f ||
             (float)ai_CharsT[i].sk84 < 252.0f) && util_RandomU(0x80) > 0x20)
            return 0;
    }
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title &&
            (ai_CharsT[i].job == 0 || ai_CharsT[i].job->kind == 4 || ai_CharsT[i].job->kind == -0x74) && !(ai_CharsT[i].flags1c8 & 4))
            ai_CharsT[i].flags1b4 |= 0x10;
    }
    return 1;
}
/* 0x489660, sends the free thieves out */

void ai_SendFreeThieves(AICHAR *ch)
{
    AICHAR *c = (AICHAR *)ch;
    AIOBJ *o;
    AIMSG msg;
    int i, n;
    char k;
    AIBTYPE *bt;

    n = 2;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title && ai_CharsT[i].job && ai_CharsT[i].job->kind == 0x16)
            n--;
    }
    if (0 < n) {
        bt = &ai_BTypesT[c->bld->type];
        k = ((volatile AIBTYPE *)bt)->kind;
        if (k == 4)
            o = gm_Query(c->bld->objList, 2, 6, 0, 0x60);
        else if (k == 0x13)
            o = gm_Query(c->bld->objList, 2, 6, 0, 0x145);
        else
            o = 0;
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x16;
        msg.to = ai_CharsT[c->bld->owner].netId;
        msg.fromId = c->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        if (o)
            msg.arg[2] = o->objId;
        else
            msg.arg[2] = -1;
        i = 0;
        while (1) {
            if (i >= 768)
                break;
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title && (ai_CharsT[i].flags1b4 & 0x10)) {
                if (ai_CharsT[i].job) {
                    if (ai_CharsT[i].job->kind == 0x16)
                        ai_CharsT[i].flags1b4 &= ~0x10;
                } else {
                    if (ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                        if (ai_CharsT[i].sk83 < ai_CharsT[i].sk82) {
                            msg.arg[1] = ai_CharsT[i].netId;
                            msg.thiefClass = 3;
                            cm_SendLongMessage(&msg);
                            n--;
                        } else {
                            msg.arg[1] = ai_CharsT[i].netId;
                            msg.thiefClass = 2;
                            cm_SendLongMessage(&msg);
                            n--;
                        }
                        ai_CharsT[i].flags1c8 |= 4;
                    }
                }
            }
            i++;
            if (n <= 0)
                break;
        }
    }
}
/* 0x489870, sends the free spies out: every character of the building with the espionage job kind 0x8f (he.c
   0x571ac0) or marked free (flags1b4 bit 0x10) is counted; when one is free and the building's main object
   0x211 exists, each spare character (no job, its model in this building) loses the free mark, gets the used
   mark (flags1c8 bit 4) and one message 0x8f goes to the owner with the spy class in arg[1] (1: sk81 < sk82,
   else 2, 4 when sk82 >= sk84) and its netId in arg[0].
   @deep behaviour draft: 62 code hunks, 38 differing asm lines of 150 instructions; the body was read against tools/asmdiff.py (every statement, constant, callee and field offset equals the original: the n = 1 count over the 0x8f-job characters, the kind 0x1d main-object 0x211 gate, the flags1b4 bit 0x10 free mark, the sk81 / sk82 / sk84 classes 1 / 2 / 4, flags1c8 bit 4). What is left is the prologue register pair (original: ebp = the parameter, ebx = n; ours: ebx = the parameter, ebp = n; spell.py 48987c push-pop) and the reloads of [ch+0x16c] (spell.py 4898b5 / 489924 / 4899a5 reload, the original re-reads ch->bld at each use while ours keeps it). Tried, byte-identical (kept where cleaner): dropping the AICHAR *c alias for the parameter, swapping the i / n declaration order. Readability: Q1 cleared with the evidenced name ai_SendFreeSpies (the job kind 0x8f is he.c's espionage event 0x571ac0, the same band as ai_SendFreeThieves), Q6 description rewritten. */

void ai_SendFreeSpies(AICHAR *ch)
{
    AIOBJ *o;
    AIMSG msg;
    int i, n;

    n = 1;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title && ai_CharsT[i].job && ai_CharsT[i].job->kind == -0x71)
            n--;
    }
    if (0 < n && ai_BTypesT[ch->bld->type].kind == 0x1d && (o = gm_Query(ch->bld->objList, 2, 6, 0, 0x211)) != 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x8f;
        msg.to = ai_CharsT[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[2] = o->objId;
        i = 0;
        while (1) {
            if (i >= 768)
                break;
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title && (ai_CharsT[i].flags1b4 & 0x10)) {
                if (ai_CharsT[i].job) {
                    if (ai_CharsT[i].job->kind == -0x71)
                        ai_CharsT[i].flags1b4 &= ~0x10;
                } else {
                    if (ai_CharsT[i].model && ai_CharsT[i].model->bldId == ai_CharsT[i].bld->id) {
                        ai_CharsT[i].flags1b4 &= ~0x10;
                        if (ai_CharsT[i].sk81 < ai_CharsT[i].sk82) {
                            msg.arg[0] = ai_CharsT[i].netId;
                            msg.arg[1] = 1;
                            cm_SendLongMessage(&msg);
                        } else {
                            msg.arg[1] = 2;
                            msg.arg[0] = ai_CharsT[i].netId;
                            if (ai_CharsT[i].sk82 >= ai_CharsT[i].sk84)
                                msg.arg[1] = 4;
                            cm_SendLongMessage(&msg);
                        }
                        n--;
                        ai_CharsT[i].flags1c8 |= 4;
                    }
                }
            }
            i++;
            if (n <= 0)
                break;
        }
    }
}

#define SELL_FMT "cm_RequestSellObjekt(%i, %i, %i, %i, %i, %i)"

/* 0x489a70, a master hands a weapon out of a slot to a character of his building */
void ai_MeisterBewaffnen(AICHAR *ch, AIOBJ *b)
{
    AICHAR *c;
    AISLOT *sel, *q;
    unsigned short *p;
    int *t;
    int i, j, found;
    char buf[1024];

    i = 0;
    do {
        c = &ai_CharsT[i];
        if (c->id != 0xffff && c->bld == ch->bld && c->title && c->model && c->model->bldId == c->bld->id) {
            sel = NULL;
            p = (unsigned short *)gm_Query(c->inventory, 1, 4, 12);
            if (!p)
                return;
            found = 0;
            q = &ai_Slots[1];
            t = &ai_WeaponList[1];
            while (!found) {
                if (!t[-1] || !t[0])
                    break;
                t[-1] = *p;
                if (t[-1] != 0) {
                    j = 0;
                    while (!found) {
                        if (j >= ai_NSlots)
                            break;
                        if (ai_Slots[j].item == t[0]) {
                            sel = q;
                            found = 1;
                        }
                        j++;
                    }
                }
                t++;
                q++;
            }
            if (!found) {
                q = &ai_Slots[1];
                t = &ai_WeaponList2[1];
                while (!found) {
                    if (!t[-1] || !t[0])
                        break;
                    t[-1] = *p;
                    if (t[-1] != 0) {
                        j = 0;
                            while (!found) {
                            if (j >= ai_NSlots)
                                break;
                            if (ai_Slots[j].item == t[0]) {
                                sel = q;
                                found = 1;
                            }
                            j++;
                            }
                    }
                    t++;
                    q++;
                }
            }
            if (sel && sel->amount && ai_FindWeaponRecord(sel->item) && ai_GoodFitsBuilding(ch, sel->item)) {
                sprintf(buf, "Meister %s bewaffnet %s mit %s", ch->name, c->name, ai_GoodsT[sel->item].name);
                sprintf(buf, SELL_FMT, c->netId, b->objId, sel->item, 1, ai_OwnPlayer, 0);
                cm_SendExSellObjectI(c->netId, b->objId, sel->item, 1, ai_OwnPlayer, 0);
                sel->amount--;
            }
        }
        i++;
    } while (i < 768);
}
/* 0x489cb0, the transport master's collection check, run in every odd hour: it first (the flags1b4 bit 0x80
   guard, set and cleared each call) walks the 0x5008 object records for the goods objects of kind 0x1d that
   belong to this building, counts them (n) and the 0x136 transporters (n136), and for each cart that no job 11
   (building) or 15 (owner) already targets it walks the cart's +6 owner chain (gm_FindById) up to a building;
   a cart that belongs to another building gets the move message 0xf to that building. Then, for a kind-9 master,
   it buys a transporter (0x134 / 0x135 / 0x136) when the counts, the cap247 limit, the budget (twice the item
   price), the 1-in-0x2ee roll and the flags1c8 bit 8 allow it, and sets flags1c8 bit 8; on an even hour the
   flag 0x80 is cleared only.
   @deep Read whole against the original 0x489cb0..0x48a372 and rewritten: the odd-hour body as one if with
   the flag-0x80 clear as its else, the cart walk over the 0x5008 objects (type, goods kind 0x1d,
   AIOBJ.bldId of the cart, the n / n136 counts, the two job searches 11 / 15, the owner chain as the
   original's do { if (v == -1) ... } while (!own)), the move message 0xf (to, time, car, fromId -1, n =
   own->id, ch, AIMSG.moveArgA0 -1, was unka0), the kind-9 purchases in the original's order (n == n136:
   0x134; no 0x136 yet: 0x136; else 0x135 / 0x136 by util_Random(8) < 4) and the other kinds' 0x134 / 0x135
   with the owner-kind 6 / 7 flag 4 test; the misplaced braces of the old text are gone. Every call,
   constant, string and field matches. Left is allocation and cross-jumping: the original keeps n in edi
   (spilled to [esp+0x1c] around the owner chain) and compares against 0 with test, ours keeps n in a slot
   and 0 in ebp (cmp cx,bp / push ebp at +0x8e .. +0x142); the original keeps four cm_SendExSellObject
   sites (the 0x136 block owns the shared push / call / 'Um %li:%li' tail that the 0x135 and random blocks
   jump into), ours cross-jumps one more (calls: ours x3, the original x4; 472 vs 483 instructions), and
   the record stores of the move message are scheduled otherwise (+0x1d0). */


void ai_MeisterCollectTrans(AICHAR *ch)
{
    AIMSG msg;
    AIOBJ *t;
    AIJOB *job;
    AIBLD *own;
    AIOBJ *q;
    AIBTYPE *bt;
    char buf[1024];
    int i, n, n136, v, item;

    if (ai_Time.hour % 2) {
        if (ch->flags1b4 & 0x80)
            return;
        ch->flags1b4 |= 0x80;
        sprintf(buf, "ai_MeisterCollectTrans(): Check um %li:%li Uhr", ai_Time.hour, ai_Time.min);
        n = 0;
        n136 = 0;
        t = (AIOBJ *)ai_Objects;
        i = 0x5008;
        do {
            if (t->type && ai_GoodsT[t->type].type == 0x1d && t->bldId == ch->bld->id) {
                n++;
                if (t->type == 0x136)
                    n136++;
                /* a cart a unload (11) or collect (15) job already moves is left alone */
                for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
                    if (job->u.id == t->objId)
                        goto next;
                }
                for (job = he_FindEvent(2, 0, 15, 2, ch->bld->owner); job; job = he_NextEvent()) {
                    if (job->u.id == t->objId)
                        goto next;
                }
                /* the building the cart stands in: up its owner chain */
                v = t->ownerId;
                own = NULL;
                do {
                    if (v == -1) {
                        if (!own)
                            goto next;
                        break;
                    }
                    if (!gm_FindById((int)&own, (int *)&q, 0, v))
                        v = -1;
                    else if (q)
                        v = q->ownerId;
                } while (!own);
                if (own != ch->bld) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xf;
                    msg.flags = 2;
                    msg.to = ai_CharsT[ch->bld->owner].netId;
                    msg.time = ai_Time;
                    msg.car = t->objId;
                    msg.fromId = -1;
                    msg.n = own->id;
                    msg.ch = ch->bld->id;
                    msg.moveArgA0 = -1;
                    cm_SendLongMessage(&msg);
                    sprintf(buf, "ai_MeisterCollectTrans(): Verlorener Transporter wird von %s nach %s geschickt",
                            own->name, ch->bld->name);
                }
            }
        next:
            t++;
        } while (--i);
        bt = &ai_BTypesT[ch->bld->type];
        if (bt->kind == 9) {
            if (n == n136) {
                cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x134, 100));
                cm_SendExSellObjectI(ch->bld->id, -1, 0x134, 1, ai_OwnPlayer, 0);
                sprintf(buf, "ai_MeisterCollectTrans(): Transporter f\xfcr %s gekauft", ch->bld->name);
                return;
            }
            if (!n136) {
                if (ch->flags1c8 & 8)
                    return;
                if (n >= bt->cap247)
                    return;
                if (util_RandomU(0x2ee) >= 2)
                    return;
                if (ch->budget <= (int)gm_ItemPrice(0x136, 100) * 2)
                    return;
                cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x136, 100));
                cm_SendExSellObjectI(ch->bld->id, -1, 0x136, 1, ai_OwnPlayer, 0);
            } else {
                if (ch->flags1c8 & 8)
                    return;
                if (n >= bt->cap247)
                    return;
                if (util_RandomU(0x2ee) >= 2)
                    return;
                item = 0x136 - (util_RandomU(8) < 4);
                if (ch->budget <= (int)gm_ItemPrice(item, 100) * 2)
                    return;
                cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(item, 100));
                cm_SendExSellObjectI(ch->bld->id, -1, item, 1, ai_OwnPlayer, 0);
            }
        } else {
            if (n == 0) {
                if ((ai_CharsT[ch->bld->owner].kind == 6 || ai_CharsT[ch->bld->owner].kind == 7) && !(ch->flags1b4 & 4))
                    return;
                cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x134, 100));
                cm_SendExSellObjectI(ch->bld->id, -1, 0x134, 1, ai_OwnPlayer, 0);
                sprintf(buf, "ai_MeisterCollectTrans(): Transporter f\xfcr %s gekauft", ch->bld->name);
                return;
            }
            if ((ai_CharsT[ch->bld->owner].kind == 6 || ai_CharsT[ch->bld->owner].kind == 7) && !(ch->flags1b4 & 4))
                return;
            if (ch->flags1c8 & 8)
                return;
            if (n >= bt->cap247)
                return;
            if (util_RandomU(0x2ee) >= 2)
                return;
            if (ch->budget <= (int)gm_ItemPrice(0x135, 100) * 2)
                return;
            cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x135, 100));
            cm_SendExSellObjectI(ch->bld->id, -1, 0x135, 1, ai_OwnPlayer, 0);
        }
        sprintf(buf, "ai_MeisterCollectTrans(): Um %li:%li Uhr zus\xe4tzlichen Transporter f\xfcr %s gekauft",
                ai_Time.hour, ai_Time.min, ch->bld->name);
        ch->flags1c8 |= 8;
    } else {
        ch->flags1b4 &= ~0x80;
    }
}
/* 0x48a380, the unload order of the stolen goods carts of a master's building */

void ai_UnloadCarts(AICHAR *ch)
{
    AIOBJ *car;
    AIMSG msg;
    int id, fill;

    if (ai_IsLocalJob(ch) && (ch->flags1b4 & 1)) {
        for (car = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); car;
             car = gm_QueryNext()) {
            if (car->fill < util_RandomU(0x20) + 0x28) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x10;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.flags = 1;
                msg.time = ai_Time;
                gm_AddTime(&msg.time, 0, 0, 10);
                id = car->objId;
                fill = car->fill;
                msg.dst[0] = 5;
                msg.car = id;
                msg.u.s.cost = 0;
                msg.n = 100 - fill;
                cm_SendLongMessage(&msg);
            }
        }
    }
}
/* 0x48a4d0, the trade master's round (returns 1; a building of type 0x116 goes to ai_MeisterTradeAllgemein):
   on the odd hour it plans the sells - the slots whose goods the store holds enough of get a sellAmount (half for
   a kind-22 master), the entries get theirs from the price ratio of the boss / the money shortage / a full
   store - and sets the flags1b4 bit 0x200 (cleared on the even hour); then it refreshes the two goods lists of
   the building (gm_BuildingGoodsList, kind 1 / 0) and resets the field +0x1c of the goods objects of the buy
   slots (and of the kinds 13 / 11 / 12 entries). Every cart of the building (gm_Query kind 3, the 0x1d goods,
   the owner) is then handled: a cart below a third full gets the reload message 0x10 (10 seconds ahead); a full
   cart without a running job 11 is unloaded - the goods are sold to the market stalls (cm_SendExSellObject, the
   sellAmount counts down) and the buys of the slots with a make plan are collected into one message 11 (up to
   three sells and three buys), sent unless the ai_TradeMsgOff gate is set (then ai_TradeMsgCount counts it).
   The gm_Query lists are walked to their end (the second loop drains the cursor). */

int ai_MeisterTrade(AICHAR *ch, AIOBJ *b)
{
    AIOBJ *car, *market, *o;
    AISLOT *s;
    AIJOB *job;
    AIRECIPE stA, rec, stB;
    AIMSG msg;
    char buf[1024];
    unsigned short it;
    float f, g;
    int i, j, k, m, n, v, lim, found, nsell, nbuy, sold;

    if (b->type == 0x116)
        return ai_MeisterTradeAllgemein(ch, b);
    for (i = 0; i < ai_NSlots; i++) {
        if (!(ai_Slots[i].flags & 6) && (ai_Slots[i].flags & 8)) {
            v = ai_Slots[i].amount;
            if (v > gm_ListStock(b, ai_Slots[i].item) / 3) {
                if (ai_BTypesT[ch->bld->type].kind == 22)
                    ai_Slots[i].sellAmount = v / 2;
                else
                    ai_Slots[i].sellAmount = v;
            }
        }
    }
    if (ai_Time.hour % 2) {
        if (!(ch->flags1b4 & 0x200)) {
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6) && (!(ai_Entries[i].flags & 8) || ai_Entries[i].amount > 3)) {
                    if (ai_CharsT[ch->bld->owner].kind == 6) {
                        f = ai_SettingFactor(ch->bld);
                        g = gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        if (f <= g / gm_ItemPrice(ai_Entries[i].item, 100)) {
                            ai_Entries[i].sellAmount = ai_Entries[i].amount;
                            if (ch == ai_LoggedMaster)
                                sprintf(buf, "Verkauf von %li Einheiten %s, weil der Preis f\xfcr den Boss ok ist",
                                        ai_Entries[i].amount, ai_GoodsT[ai_Entries[i].item].name);
                        }
                    } else {
                        g = gm_ItemPrice(ai_Entries[i].item, 100);
                        f = g / gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        if (f < 1.0f && f <= util_RandomF() * 1.25f + 0.25) {
                            ai_Entries[i].sellAmount = ai_Entries[i].amount;
                            if (ch == ai_LoggedMaster)
                                sprintf(buf, "Verkauf wegen guter Gewinnspanne von %li Einheiten %s",
                                        ai_Entries[i].sellAmount, ai_GoodsT[ai_Entries[i].item].name);
                        }
                    }
                }
            }
            if (ch->budget < 3200 || gm_GetGoodsValue(&ai_CharsT[ch->bld->owner]) < 8000) {
                v = min(ch->budget, gm_GetGoodsValue(&ai_CharsT[ch->bld->owner]));
                for (i = 0; v < 32000 && i < ai_NEntries; i++) {
                    if (!(ai_Entries[i].flags & 6)) {
                        k = (int)gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        n = 32000 / k;
                        if (n < 1)
                            n = 1;
                        ai_Entries[i].sellAmount = min(n, ai_Entries[i].amount);
                        v += n * k;
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s", ai_Entries[i].sellAmount,
                                    ai_GoodsT[ai_Entries[i].item].name);
                    }
                }
            }
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6)) {
                    if (ai_Entries[i].amount > gm_ListStock(b, ai_Entries[i].item) * 3 / 4) {
                        ai_Entries[i].sellAmount = max(5, ai_Entries[i].amount / 4);
                        ai_Entries[i].sellAmount = min(ai_Entries[i].sellAmount, ai_Entries[i].amount);
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s wegen \xfc" "berf\xfc" "lltem Lager",
                                    ai_Entries[i].sellAmount, ai_GoodsT[ai_Entries[i].item].name);
                    }
                }
            }
        }
        ch->flags1b4 |= 0x200;
    } else {
        ch->flags1b4 &= ~0x200;
    }
    gm_BuildingGoodsList(ch->bld, &stA, 1);
    gm_BuildingGoodsList(ch->bld, &stB, 0);
    for (j = 0; j < ai_NSlots; j++) {
        if ((ai_Slots[j].flags & 6) && (o = gm_Query(b->list, 1, 0, ai_Slots[j].item)) != NULL) {
            if (o->unk1c != 0) {
                v = 0;
                cm_BeginFieldBlock(o, o->objId);
                cm_AddFieldDelta(4, 1, (char *)o - cm_FieldObject + 0x1c, &v);
                cm_SendFieldDeltas();
            }
        }
    }
    k = ai_BTypesT[ch->bld->type].kind;
    if (k == 13 || k == 11 || k == 12) {
        for (i = 0; i < ai_NEntries; i++) {
            if (!(ai_Entries[i].flags & 6) && (o = gm_Query(b->list, 1, 0, ai_Entries[i].item)) != NULL
                && o->unk1c != 0) {
                v = 0;
                cm_BeginFieldBlock(o, o->objId);
                cm_AddFieldDelta(4, 1, (char *)o - cm_FieldObject + 0x1c, &v);
                cm_SendFieldDeltas();
            }
        }
    }
    for (car = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); car;
         car = gm_QueryNext()) {
        if (car->unk1c == ch->bld->id) {
            if (car->fill < 0x21) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x10;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.flags = 1;
                msg.time = ai_Time;
                gm_AddTime(&msg.time, 0, 0, 10);
                msg.car = car->objId;
                msg.dst[0] = 5;
                msg.u.s.cost = 0;
                msg.n = 100 - car->fill;
                cm_SendLongMessage(&msg);
                continue;
            }
            for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
                if (job->u.id == car->objId)
                    break;
            }
            if (!job) {
                memset(&msg, 0, sizeof(msg));
                nsell = 0;
                nbuy = 0;
                if (ai_WorkLoad() > ai_WorkLimit())
                    msg.unk9d = 1;
                for (k = 0; k < 3; k++) {
                    msg.u.item[k] = 0;
                    msg.dst[k] = -1;
                    msg.u.item[k + 3] = 0;
                    msg.dst[k + 3] = -1;
                }
                gm_StallGoodsList(car, &rec);
                market = gm_Query(car->list, 1, 0, 0x1dd);
                if (!market)
                    market = gm_Query(car->list, 1, 0, 0x1dd);
                sold = 0;
                for (i = 0; i < rec.n; i++) {
                    if (!rec.item[i])
                        continue;
                    sprintf(buf, "ai_MeisterTrade(): Von Auto in \"%s\" %li Einheiten %s entladen", ch->bld->name,
                            rec.amount[i], ai_GoodsT[rec.item[i]].name);
                    s = NULL;
                    for (m = 0; !s && m < 128; m++) {
                        if (ai_Slots[m].item == rec.item[i])
                            s = &ai_Slots[m];
                    }
                    if (s && (s->flags & 2)) {
                        found = 0;
                        for (j = 0; !found && j < stA.n; j++) {
                            if (stA.item[j] == rec.item[i]) {
                                sprintf(buf, "ai_MeisterTrade(): %s ist in \"%s\" schon vorhanden",
                                        ai_GoodsT[rec.item[i]].name, ch->bld->name);
                                n = min(rec.amount[i], stA.max[j] - stA.amount[j]);
                                if (n) {
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                }
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (!stA.item[j]) {
                                sprintf(buf, "ai_MeisterTrade(): \"%s\" hat leeren Slot f\xfcr %s", ch->bld->name,
                                        ai_GoodsT[rec.item[i]].name);
                                n = gm_ListStock(b, rec.item[i]);
                                if (rec.amount[i] < n)
                                    n = rec.amount[i];
                                sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.item[j] = rec.item[i];
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (found)
                                continue;
                            for (m = 0; !found && m < 128; m++) {
                                if (ai_Slots[m].item == stA.item[j]
                                    && (!(ai_Slots[m].flags & 2) || ai_Slots[m].rank > s->rank)) {
                                    sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf aus Lager in \"%s\" von %li Einheiten %s",
                                            ch->bld->name, stA.amount[j], ai_GoodsT[stA.item[j]].name);
                                    sprintf(buf, SELL_FMT, -1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                            (double)(int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    cm_SendExSellObjectI(-1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                                 (int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    ch->budget += gm_GoodsPrice(stA.item[j], ai_OwnPlayer);
                                    stA.item[j] = 0;
                                    stA.amount[j] = 0;
                                    n = gm_ListStock(b, rec.item[i]);
                                    if (rec.amount[i] < n)
                                        n = rec.amount[i];
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    rec.amount[i] -= n;
                                    if (!rec.amount[i])
                                        rec.item[i] = 0;
                                    stA.item[j] = rec.item[i];
                                    stA.amount[j] += n;
                                    found = 1;
                                }
                            }
                        }
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
                for (i = 0; nsell < 3 && i < stA.n; i++) {
                    it = stA.item[i];
                    if (!it)
                        continue;
                    found = 0;
                    for (j = 0; !found && j < ai_NEntries; j++) {
                        if (it == ai_Entries[j].item)
                            found = 1;
                    }
                    for (j = 0; !found && j < ai_NSlots; j++) {
                        if (it == ai_Slots[j].item)
                            found = 1;
                    }
                    if (!found) {
                        lim = gm_ListStock(market, it);
                        for (j = 0; stA.amount[i] && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == stA.item[i]) {
                                n = stA.amount[i];
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = stA.item[i];
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                sold = 1;
                                stA.amount[i] -= n;
                                rec.item[j] = stA.item[i];
                            }
                        }
                        if (!stA.amount[i]) {
                            cm_SendRemoveGoodsI(b->objId, stA.item[i]);
                            stA.item[i] = 0;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < stB.n; i++) {
                    it = stB.item[i];
                    if (!it)
                        continue;
                    found = 0;
                    for (j = 0; !found && j < ai_NEntries; j++) {
                        if (it == ai_Entries[j].item)
                            found = 1;
                    }
                    for (j = 0; !found && j < ai_NSlots; j++) {
                        if (it == ai_Slots[j].item)
                            found = 1;
                    }
                    if (!found) {
                        lim = gm_ListStock(market, it);
                        for (j = 0; stB.amount[i] && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == stB.item[i]) {
                                n = stB.amount[i];
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, stB.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, stB.item[i], n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = stB.item[i];
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                sold = 1;
                                stB.amount[i] -= n;
                                rec.item[j] = stB.item[i];
                            }
                        }
                        if (!stB.amount[i]) {
                            cm_SendRemoveGoodsI(b->objId, stB.item[i]);
                            stB.item[i] = 0;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NEntries; i++) {
                    if (!ai_Entries[i].sellAmount)
                        continue;
                    lim = gm_ListStock(market, ai_Entries[i].item);
                    for (j = 0; ai_Entries[i].sellAmount && j < rec.n; j++) {
                        if (!rec.item[j] || rec.item[j] == ai_Entries[i].item) {
                            n = ai_Entries[i].sellAmount;
                            if (lim < n)
                                n = lim;
                            sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            msg.u.item[nsell] = ai_Entries[i].item;
                            msg.dst[nsell] = ai_MarketBuilding->id;
                            nsell++;
                            ai_Entries[i].sellAmount -= n;
                            sold = 1;
                            rec.item[j] = ai_Entries[i].item;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].sellAmount)
                        continue;
                    if (!gm_FindMarketObject(ai_Slots[i].item)) {
                        sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf in \"%s\" von %li Einheiten %s, weil kein Marktlager",
                                ch->bld->name, ai_Slots[i].sellAmount, ai_GoodsT[ai_Slots[i].item].name);
                        sprintf(buf, SELL_FMT, -1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                     (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        ai_Slots[i].sellAmount = 0;
                    } else {
                        lim = gm_ListStock(market, ai_Slots[i].item);
                        for (j = 0; ai_Slots[i].sellAmount && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == ai_Slots[i].item) {
                                n = ai_Slots[i].sellAmount;
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = ai_Slots[i].item;
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                ai_Slots[i].sellAmount -= n;
                                sold = 1;
                                rec.item[j] = ai_Slots[i].item;
                            }
                        }
                    }
                }
                for (i = 0; nbuy < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].make)
                        continue;
                    n = gm_ListStock(market, ai_Slots[i].item);
                    if (ai_Slots[i].bld) {
                        msg.u.item[nbuy + 3] = ai_Slots[i].item;
                        msg.dst[nbuy + 3] = ai_Slots[i].bld->id;
                        msg.amount[nbuy] = min(n, ai_Slots[i].make);
                        nbuy++;
                        ai_Slots[i].make = 0;
                        sold = 1;
                    }
                }
                if (sold) {
                    if (ch == ai_LoggedMaster) {
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k])
                                sprintf(buf, "Verkauf von %s", ai_GoodsT[msg.u.item[k]].name);
                        }
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k + 3])
                                sprintf(buf, "Einkauf von %li Einheiten %s", msg.amount[k],
                                        ai_GoodsT[msg.u.item[k + 3]].name);
                        }
                    }
                    n = msg.dst[0];
                    if (n == -1)
                        n = msg.dst[3];
                    msg.type = 11;
                    msg.to = ai_CharsT[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.time = ai_Time;
                    msg.flags = 1;
                    msg.car = car->objId;
                    msg.n = n;
                    msg.ch = ch->netId;
                    if (!ai_TradeMsgOff) {
                        cm_SendLongMessage(&msg);
                        ai_TradeMsgCount++;
                    }
                }
            }
        }
        for (o = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); o != car;
             o = gm_QueryNext())
            ;
    }
    return 1;
}
/* 0x48bc80, the trade round of a building of type 0x116 (ai_MeisterTrade forwards here; returns 1): as
   ai_MeisterTrade it plans the sells on the odd hour - the slots whose store holds enough get a sellAmount, the
   entries theirs by the price margin f < 1 with a 1.25 roll, by the money shortage and by the full store - under
   the flags1b4 bit 0x200 gate, then handles the building's carts: a cart below a third full gets the reload
   message 0x10 (10 seconds ahead); a full one without a running job 11 is unloaded against the market - the stall
   goods are sold to the market stalls (cm_SendExSellObject) and the buys collected into one message 11, sent
   unless the ai_TradeMsgOff gate is set. */

int ai_MeisterTradeAllgemein(AICHAR *ch, AIOBJ *b)
{
    AIOBJ *car, *market, *o;
    AISLOT *s;
    AIJOB *job;
    AIRECIPE stA, rec;
    AIMSG msg;
    char buf[1024];
    unsigned short it;
    float f, g;
    int i, j, k, m, n, v, lim, found, nsell, nbuy, sold;

    for (i = 0; i < ai_NSlots; i++) {
        if (!(ai_Slots[i].flags & 6) && (ai_Slots[i].flags & 8)) {
            v = ai_Slots[i].amount;
            if (v > gm_ListStock(b, ai_Slots[i].item) / 3)
                ai_Slots[i].sellAmount = v;
        }
    }
    if (ai_Time.hour % 2) {
        if (!(ch->flags1b4 & 0x200)) {
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6)
                    && (!(ai_Entries[i].flags & 8) || ai_Entries[i].amount > gm_ListStock(b, ai_Entries[i].item) / 3)) {
                    f = gm_ItemPrice(ai_Entries[i].item, 100);
                    f = f / gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                    if (f < 1.0f && f <= util_RandomF() * 1.25f) {
                        ai_Entries[i].sellAmount = ai_Entries[i].amount;
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Verkauf wegen guter Gewinnspanne von %li Einheiten %s",
                                    ai_Entries[i].sellAmount, ai_GoodsT[ai_Entries[i].item].name);
                    }
                }
            }
            if (ch->budget < 3200 || gm_GetGoodsValue(&ai_CharsT[ch->bld->owner]) < 8000) {
                v = min(ch->budget, gm_GetGoodsValue(&ai_CharsT[ch->bld->owner]));
                for (i = 0; v < 32000 && i < ai_NEntries; i++) {
                    if (!(ai_Entries[i].flags & 6)) {
                        k = (int)gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        n = 32000 / k;
                        if (n < 1)
                            n = 1;
                        ai_Entries[i].sellAmount = min(n, ai_Entries[i].amount);
                        v += n * k;
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s", ai_Entries[i].sellAmount,
                                    ai_GoodsT[ai_Entries[i].item].name);
                    }
                }
            }
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6)) {
                    if (ai_Entries[i].amount > gm_ListStock(b, ai_Entries[i].item) * 3 / 4) {
                        ai_Entries[i].sellAmount = max(5, ai_Entries[i].amount / 4);
                        ai_Entries[i].sellAmount = min(ai_Entries[i].sellAmount, ai_Entries[i].amount);
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s wegen \xfc" "berf\xfc" "lltem Lager",
                                    ai_Entries[i].sellAmount, ai_GoodsT[ai_Entries[i].item].name);
                    }
                }
            }
        }
        ch->flags1b4 |= 0x200;
    } else {
        ch->flags1b4 &= ~0x200;
    }
    gm_BuildingGoodsList(ch->bld, &stA, 0);
    for (car = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); car;
         car = gm_QueryNext()) {
        if (car->unk1c == ch->bld->id) {
            if (car->fill < 0x21) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x10;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.flags = 1;
                msg.time = ai_Time;
                gm_AddTime(&msg.time, 0, 0, 10);
                msg.car = car->objId;
                msg.dst[0] = 5;
                msg.u.s.cost = 0;
                msg.n = 100 - car->fill;
                cm_SendLongMessage(&msg);
                continue;
            }
            for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
                if (job->u.id == car->objId)
                    break;
            }
            if (!job) {
                memset(&msg, 0, sizeof(msg));
                nsell = 0;
                nbuy = 0;
                if (ai_WorkLoad() > ai_WorkLimit())
                    msg.unk9d = 1;
                for (k = 0; k < 3; k++) {
                    msg.u.item[k] = 0;
                    msg.dst[k] = -1;
                    msg.u.item[k + 3] = 0;
                    msg.dst[k + 3] = -1;
                }
                gm_StallGoodsList(car, &rec);
                market = gm_Query(car->list, 1, 0, 0x1dd);
                sold = 0;
                for (i = 0; i < rec.n; i++) {
                    if (!rec.item[i])
                        continue;
                    sprintf(buf, "ai_MeisterTradeAllgemein(): Von Auto in \"%s\" %li Einheiten %s entladen", ch->bld->name,
                            rec.amount[i], ai_GoodsT[rec.item[i]].name);
                    s = NULL;
                    for (m = 0; !s && m < 128; m++) {
                        if (ai_Slots[m].item == rec.item[i])
                            s = &ai_Slots[m];
                    }
                    if (s && (s->flags & 2)) {
                        found = 0;
                        for (j = 0; !found && j < stA.n; j++) {
                            if (stA.item[j] == rec.item[i]) {
                                sprintf(buf, "ai_MeisterTradeAllgemein(): %s ist in \"%s\" schon vorhanden",
                                        ai_GoodsT[rec.item[i]].name, ch->bld->name);
                                n = min(rec.amount[i], stA.max[j] - stA.amount[j]);
                                if (n) {
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                }
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (!stA.item[j]) {
                                sprintf(buf, "ai_MeisterTradeAllgemein(): \"%s\" hat leeren Slot f\xfcr %s", ch->bld->name,
                                        ai_GoodsT[rec.item[i]].name);
                                n = gm_ListStock(b, rec.item[i]);
                                if (rec.amount[i] < n)
                                    n = rec.amount[i];
                                sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.item[j] = rec.item[i];
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (found)
                                continue;
                            for (m = 0; !found && m < 128; m++) {
                                if (ai_Slots[m].item == stA.item[j]
                                    && (!(ai_Slots[m].flags & 2) || ai_Slots[m].rank > s->rank)) {
                                    sprintf(buf, "ai_MeisterTradeAllgemein(): Fake-Verkauf aus Lager in \"%s\" von %li Einheiten %s",
                                            ch->bld->name, stA.amount[j], ai_GoodsT[stA.item[j]].name);
                                    sprintf(buf, SELL_FMT, -1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                            (double)(int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    cm_SendExSellObjectI(-1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                                 (int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    ch->budget += gm_GoodsPrice(stA.item[j], ai_OwnPlayer);
                                    stA.item[j] = 0;
                                    stA.amount[j] = 0;
                                    n = gm_ListStock(b, rec.item[i]);
                                    if (rec.amount[i] < n)
                                        n = rec.amount[i];
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    rec.amount[i] -= n;
                                    if (!rec.amount[i])
                                        rec.item[i] = 0;
                                    stA.item[j] = rec.item[i];
                                    stA.amount[j] += n;
                                    found = 1;
                                }
                            }
                        }
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTradeAllgemein(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
                for (i = 0; nsell < 3 && i < stA.n; i++) {
                    it = stA.item[i];
                    if (!it)
                        continue;
                    found = 0;
                    for (j = 0; !found && j < ai_NEntries; j++) {
                        if (it == ai_Entries[j].item)
                            found = 1;
                    }
                    for (j = 0; !found && j < ai_NSlots; j++) {
                        if (it == ai_Slots[j].item)
                            found = 1;
                    }
                    if (!found) {
                        lim = gm_ListStock(market, it);
                        for (j = 0; stA.amount[i] && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == stA.item[i]) {
                                n = stA.amount[i];
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = stA.item[i];
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                sold = 1;
                                stA.amount[i] -= n;
                                rec.item[j] = stA.item[i];
                            }
                        }
                        if (!stA.amount[i]) {
                            cm_SendRemoveGoodsI(b->objId, stA.item[i]);
                            stA.item[i] = 0;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NEntries; i++) {
                    if (!ai_Entries[i].sellAmount)
                        continue;
                    lim = gm_ListStock(market, ai_Entries[i].item);
                    for (j = 0; ai_Entries[i].sellAmount && j < rec.n; j++) {
                        if (!rec.item[j] || rec.item[j] == ai_Entries[i].item) {
                            n = ai_Entries[i].sellAmount;
                            if (lim < n)
                                n = lim;
                            sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            msg.u.item[nsell] = ai_Entries[i].item;
                            msg.dst[nsell] = ai_MarketBuilding->id;
                            nsell++;
                            ai_Entries[i].sellAmount -= n;
                            sold = 1;
                            rec.item[j] = ai_Entries[i].item;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].sellAmount)
                        continue;
                    if (!gm_FindMarketObject(ai_Slots[i].item)) {
                        sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf in \"%s\" von %li Einheiten %s, weil kein Marktlager",
                                ch->bld->name, ai_Slots[i].sellAmount, ai_GoodsT[ai_Slots[i].item].name);
                        sprintf(buf, SELL_FMT, -1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                     (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        ai_Slots[i].sellAmount = 0;
                    } else {
                        lim = gm_ListStock(market, ai_Slots[i].item);
                        for (j = 0; ai_Slots[i].sellAmount && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == ai_Slots[i].item) {
                                n = ai_Slots[i].sellAmount;
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = ai_Slots[i].item;
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                ai_Slots[i].sellAmount -= n;
                                sold = 1;
                                rec.item[j] = ai_Slots[i].item;
                            }
                        }
                    }
                }
                for (i = 0; nbuy < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].make)
                        continue;
                    n = gm_ListStock(market, ai_Slots[i].item);
                    if (ai_Slots[i].bld) {
                        msg.u.item[nbuy + 3] = ai_Slots[i].item;
                        msg.dst[nbuy + 3] = ai_Slots[i].bld->id;
                        msg.amount[nbuy] = min(n, ai_Slots[i].make);
                        nbuy++;
                        ai_Slots[i].make = 0;
                        sold = 1;
                    }
                }
                if (sold) {
                    if (ch == ai_LoggedMaster) {
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k])
                                sprintf(buf, "Verkauf von %s", ai_GoodsT[msg.u.item[k]].name);
                        }
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k + 3])
                                sprintf(buf, "Einkauf von %li Einheiten %s", msg.amount[k],
                                        ai_GoodsT[msg.u.item[k + 3]].name);
                        }
                    }
                    n = msg.dst[0];
                    if (n == -1)
                        n = msg.dst[3];
                    msg.type = 11;
                    msg.to = ai_CharsT[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.time = ai_Time;
                    msg.flags = 1;
                    msg.car = car->objId;
                    msg.n = n;
                    msg.ch = ch->netId;
                    if (!ai_TradeMsgOff) {
                        cm_SendLongMessage(&msg);
                        ai_TradeMsgCount++;
                    }
                }
            }
        }
        for (o = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); o != car;
             o = gm_QueryNext())
            ;
    }
    return 1;
}
/* 0x48d010, ai_MeisterTradeFernEinkauf: the long-distance buyer sends every cart that is in the world to a
   market store (unloads it, then orders the purchases of the slots / entries) */

int ai_MeisterTradeFernEinkauf(AICHAR *ch, AIOBJ *b)
{
    AIMSG msg;
    AIRECIPE stA, rec;
    AIOBJ *car, *market;
    AISLOT *hit, *sp;
    char *bl;
    AISLOT *sl;
    char buf[1024];
    int exp[4];
    int k, i, m, n, nA, nB, amt, d, v;

    nA = 0;
    nB = 0;
    if (ai_Slots[0].saving <= 0 || ai_Slots[0].buyAmount == 0)
        return 0;
    gm_BuildingGoodsList(ch->bld, &stA, 0);
    for (k = 0; k < ai_NumCarts; k++) {
        if (nB > 3)
            __asm int 3
        car = ai_Carts[k];
        if (car->fill < 0x21) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x10;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.flags = 1;
            msg.time = ai_Time;
            gm_AddTime(&msg.time, 0, 0, 10);
            msg.car = car->objId;
            msg.dst[0] = 5;
            msg.u.s.cost = 0;
            msg.n = 100 - car->fill;
            cm_SendLongMessage(&msg);
        } else {
            memset(&msg, 0, sizeof(msg));
            if (ai_WorkLoad() > ai_WorkLimit())
                msg.unk9d = 1;
            for (i = 0; i < 3; i++) {
                msg.u.item[i] = 0;
                msg.dst[i] = -1;
                msg.u.item[i + 3] = 0;
                msg.dst[i + 3] = -1;
            }
            gm_StallGoodsList(car, &rec);
            market = gm_Query(car->list, 1, 0, 0x1dd);
            for (i = 0; i < rec.n; i++) {
                if (rec.item[i]) {
                    sprintf(buf, "ai_MeisterTradeFernEinkauf(): Von Auto in \"%s\" %li Einheiten %s entladen",
                            ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                    hit = NULL;
                    sp = ai_Slots;
                    while (!hit) {
                        if ((int)sp >= (int)&ai_Slots[128])
                            break;
                        if (sp->item == rec.item[i])
                            hit = sp;
                        sp++;
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTradeFernEinkauf(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
            }
            exp[0] = 0;
            exp[1] = 0;
            exp[2] = 0;
            nB = 0;
            exp[3] = 0;
            i = 0;
            if (ai_Slots[0].item) {
                amt = gm_ListStock(market, ai_Slots[0].item);
                msg.u.item[3] = ai_Slots[0].item;
                msg.dst[3] = ai_Slots[0].bld->id;
                if (amt >= ai_Slots[0].buyAmount)
                    amt = ai_Slots[0].buyAmount;
                msg.amount[0] = amt;
                nB = 1;
                i = 1;
            }
            sl = &ai_Slots[i];
            do {
                if (i >= ai_NSlots || !(sl->saving <= 0))
                    break;
                if (msg.dst[3] == sl->bld->id && sl->buyAmount) {
                    amt = gm_ListStock(market, sl->item);
                    bl = (char *)sl->bld;
                    if (bl) {
                        msg.u.item[nB + 3] = sl->item;
                        msg.dst[nB + 3] = *(int *)(bl + 1);
                        if (amt >= sl->buyAmount)
                            amt = sl->buyAmount;
                        msg.amount[nB] = amt;
                        if (*bl == 'G')
                            exp[*(int *)(bl + 0x65)] = (int)(amt * sl->price + exp[*(int *)(bl + 0x65)]);
                        nB++;
                    }
                }
                i++;
                sl++;
            } while (nB < 3);
            for (i = 1; i < 4; i++) {
                v = gm_SumSlotAmounts(ai_CharsT[ch->bld->owner].inventory, i);
                if (v < exp[i]) {
                    d = exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, i, d);
                    cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                } else if (v > exp[i] * 2) {
                    d = v - exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, ai_OwnPlayer, d);
                    cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, i, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                }
            }
            i = 0;
            if (nA < 3) {
                for (; i < ai_NEntries && ai_Entries[i].margin <= 0; i++) {
                    if (msg.dst[3] == ai_Entries[i].bld->id && ai_Entries[i].buyAmount) {
                        amt = gm_ListStock(market, ai_Entries[i].item);
                        for (m = 0; m < rec.n; m++) {
                            if (!rec.item[m]) {
                                n = amt;
                                if (ai_Entries[i].buyAmount < n)
                                    n = ai_Entries[i].buyAmount;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nA] = ai_Entries[i].item;
                                msg.dst[nA] = ai_Entries[i].bld->id;
                                nA++;
                                rec.item[m] = ai_Entries[i].item;
                            }
                        }
                    }
                    if (nA >= 3)
                        break;
                }
            }
            n = msg.dst[0];
            if (n == -1)
                n = msg.dst[3];
            msg.type = 0xb;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
            msg.flags = 1;
            msg.car = car->objId;
            msg.n = n;
            msg.ch = ch->netId;
            if (!ai_TradeMsgOff) {
                cm_SendLongMessage(&msg);
                ai_TradeMsgCount++;
            }
        }
    }
    return 1;
}
/* 0x48d810, ai_MeisterTradeFernEinkauf, the variant that sells entry 0 first (the entries are searched before
   the slots): the kind-9 master's long-distance purchase - for every cart in ai_Carts, a free one (fill < 0x21)
   gets the unload order (message 0x10); a loaded one first unloads its goods at the market (a fake sale at the
   market's price credits the budget), then buys entry 0 and up to two more entries whose building is dst[3],
   then the slots (up to three buys per player, the expected value exp[] per player from the banks of the type
   'G'), and sends the trade message 0xb (unless ai_TradeMsgOff) */

int ai_MeisterTradeFernEinkauf2(AICHAR *ch, AIOBJ *b)
{
    AIMSG msg;
    AIRECIPE stA, rec;
    AIOBJ *car, *market;
    AISLOT *hit, *sp;
    AIBLD *bl;
    AISLOT *sl;
    char buf[1024];
    int exp[4];
    int k, i, m, n, nA, nB, amt, d, v;

    nA = 0;
    nB = 0;
    if (ai_Entries[0].margin <= 0 || ai_Entries[0].buyAmount == 0)
        return 0;
    gm_BuildingGoodsList(ch->bld, &stA, 0);
    for (k = 0; k < ai_NumCarts; k++) {
        car = ai_Carts[k];
        if (car->fill < 0x21) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x10;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.flags = 1;
            msg.time = ai_Time;
            gm_AddTime(&msg.time, 0, 0, 10);
            msg.car = car->objId;
            msg.dst[0] = 5;
            msg.u.s.cost = 0;
            msg.n = 100 - car->fill;
            cm_SendLongMessage(&msg);
        } else {
            memset(&msg, 0, sizeof(msg));
            if (ai_WorkLoad() > ai_WorkLimit())
                msg.unk9d = 1;
            for (i = 0; i < 3; i++) {
                msg.u.item[i] = 0;
                msg.dst[i] = -1;
                msg.u.item[i + 3] = 0;
                msg.dst[i + 3] = -1;
            }
            gm_StallGoodsList(car, &rec);
            market = gm_Query(car->list, 1, 0, 0x1dd);
            for (i = 0; i < rec.n; i++) {
                if (rec.item[i]) {
                    sprintf(buf, "ai_MeisterTradeFernEinkauf(): Von Auto in \"%s\" %li Einheiten %s entladen",
                            ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                    hit = NULL;
                    sp = ai_Slots;
                    while (!hit) {
                        if ((int)sp >= (int)&ai_Slots[128])
                            break;
                        if (sp->item == rec.item[i])
                            hit = sp;
                        sp++;
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTradeFernEinkauf(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_GoodsT[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
            }
            amt = gm_ListStock(market, ai_Entries[0].item);
            n = ai_Entries[0].buyAmount;
            if (amt < n)
                n = amt;
            sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[0].item, n, ai_OwnPlayer, 0);
            if (amt >= ai_Entries[0].buyAmount)
                amt = ai_Entries[0].buyAmount;
            cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[0].item, amt, ai_OwnPlayer, 0);
            msg.u.item[nA] = ai_Entries[0].item;
            msg.dst[nA] = ai_Entries[0].bld->id;
            nA++;
            rec.item[0] = ai_Entries[0].item;
            i = 1;
            if (nA < 3) {
                do {
                    if (i >= ai_NEntries || !(ai_Entries[i].margin <= 0))
                        break;
                    if (msg.dst[3] == ai_Entries[i].bld->id && ai_Entries[i].buyAmount) {
                        amt = gm_ListStock(market, ai_Entries[i].item);
                        for (m = 0; m < rec.n; m++) {
                            if (!rec.item[m]) {
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nA] = ai_Entries[i].item;
                                msg.dst[nA] = ai_Entries[i].bld->id;
                                nA++;
                                rec.item[m] = ai_Entries[i].item;
                            }
                        }
                    }
                    i++;
                } while (nA < 3);
            }
            exp[0] = 0;
            exp[1] = 0;
            exp[2] = 0;
            exp[3] = 0;
            i = 0;
            if (nB < 3) {
                sl = ai_Slots;
                do {
                    if (i >= ai_NSlots || !(sl->saving <= 0))
                        break;
                    if (msg.dst[0] == sl->bld->id && sl->buyAmount) {
                        amt = gm_ListStock(market, sl->item);
                        bl = sl->bld;
                        if (bl) {
                            msg.u.item[nB + 3] = sl->item;
                            msg.dst[nB + 3] = bl->id;
                            if (amt >= sl->buyAmount)
                                amt = sl->buyAmount;
                            msg.amount[nB] = amt;
                            if (bl->type == 'G')
                                exp[bl->unk65] = (int)(amt * sl->price + exp[bl->unk65]);
                            nB++;
                        }
                    }
                    i++;
                    sl++;
                } while (nB < 3);
            }
            for (i = 1; i < 4; i++) {
                v = gm_SumSlotAmounts(ai_CharsT[ch->bld->owner].inventory, i);
                if (v < exp[i]) {
                    d = exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, i, d);
                    cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                } else if (v > exp[i] * 2) {
                    d = v - exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, ai_OwnPlayer, d);
                    cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, i, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                }
            }
            n = msg.dst[0];
            if (n == -1)
                n = msg.dst[3];
            msg.type = 0xb;
            msg.to = ai_CharsT[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
            msg.flags = 1;
            msg.car = car->objId;
            msg.n = n;
            msg.ch = ch->netId;
            if (!ai_TradeMsgOff) {
                cm_SendLongMessage(&msg);
                ai_TradeMsgCount++;
            }
        }
    }
    return 1;
}

AICHAR * ai_Partner = 0;  /* 0x6f0468 */
int ai_TradeMsgCount = 0;  /* 0x6f046c, the trade messages sent since the last reset */
float ai_Rating = 0;  /* 0x6f1ba0, the smoothed game rating (statistics out[5]; the running rating ai_RatingBase follows it) */
AIBLD * ai_MarketBuilding = 0;  /* 0x6f1bc8, the market building (gm_BuildQuery(1, 5, 10)); @original: f3snd.h declares G16BLDVIEW * (f3snd.c reads its + */
unsigned short * ai_PlayerChar = 0;  /* 0x6f1bf8 */
int ai_CurrentPlayer = 0;  /* 0x6f2590, the player index whose turn the local round loop simulates */
AIGRID ai_Grid[8][8] = {0}; /* 0x91ae60 */
/* ai_ActionFlags: view of ai_Actions (tools/data_views.py) */
int ai_TradeMsgOff = 0;  /* 0xf86a24, read-only in every source: nonzero suppresses the trade
                                           message and its count (never set in this build) */
int ai_NSlots = 0;  /* 0xf86a28 */
int ai_NEntries = 0;  /* 0xf86a2c */
AISLOT ai_Slots[128] = {0};  /* 0xf86a40 */
AIENTRY ai_Entries[32] = {0};  /* 0xf88a40 */
AIOBJ * ai_Carts[16] = {0};  /* 0xf89540, the master's kind- */
AIUPDATE ai_Update = {0};  /* 0xf89580, the per-frame character update cycle */
int ai_WeaponList2[10] = {0};  /* 0xf8a0a0, goods ids of the second weapon list (0-terminated; ends before */
AICHAR * ai_LoggedMaster = 0;  /* 0xf8a0c8, the master whose decisions are logged */
int ai_NumCarts = 0;  /* 0xf8a0cc, how many of ai_Carts are set (0 .. 16) */
int ai_WeaponList[10] = {0};  /* 0xf8a0e0, goods ids of the first weapon list (0-terminated) */
int ai_NetPlayer = 0;  /* 0x1460af4, the player index this client runs (the host sets it, cm.c message type 7) */
const unsigned int ai_BuildingTypeMasks[6] = { 2692498336, 1, 3766305712, 1, 0xf82806f, 2 };  /* 0x5eed70, bit masks of building classes (three 64 bit masks: [2..3] the house-owner classes, [4..5] the classes lc.c's ai_PassesMapPickFilter greys out under its 0x40000000 flag) */
float ai_DifficultyValue = 1.0f;  /* 0x5fef04 */
float ai_DifficultyScale = 0.5f;  /* 0x5fef08 */
char ai_AmbushSpots[8][32] = { "sp_AUFLAUERLEGEN_NORDEN", "sp_AUFLAUERLEGEN_OSTEN", "sp_AUFLAUERLEGEN_SUEDEN", "sp_AUFLAUERLEGEN_WESTEN", "sp_AUFLAUERLEGEN_STADT_1", "sp_AUFLAUERLEGEN_STADT_2", "sp_AUFLAUERLEGEN_STADT_3", "sp_AUFLAUERLEGEN_STADT_4" };  /* 0x625720, "sp_AUFLAUERLEGEN_NORDEN" ... */
