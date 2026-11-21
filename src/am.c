/* am.c -- the AI upgrade decisions and the law callbacks, 0x48e070..0x492fd0: the masters' building upgrade
   ("building_upgrade" 0x5ffd20) decide / execute / eval callbacks of the action kinds 0x40..0x42 and the decision
   callbacks of the AI decision table gv_Decisions (gs.c, AIDEC.fn). Starts with its first global ai_PlayerCountYear
   (-1 at 0x5ffd1c) right after the AI unit's strings; its strings end at 0x5ffdb4 and its literals (0x5ecec0..0x5ecf18)
   precede amr.c's named constants ag_Pairs.. (the AI actions with the am_RegisterMethod tag are amr.c; the name am
   stays here with am.h). Names are ours.
   Open: am.h keeps the joined parts' character views (AIPX / AGCH / AIC4 / G5CH / AIPER), message views of ai.h's
   AIMSG (AIXMSG / G8MSG) and duplicate callee prototypes / cast macros; they need unifying with ai.h's types.
   Open: AIJOBX (am.h) duplicates ai.h's AIJOB (its id / bldId / price fields are named from it); the AIJOB
   union member for the building offer's price flips ai_Unk483a60's byte match by the declaration set.
   Open: the use callers read am_types.h's G9DOMIZIL weight (+0xc) as g[3] through a float view; naming that
   field moved sv_t's ai_TutorialInitChapter3 (shape -> draft, a declaration-set tie-break).
   Open: not included yet (dcp headers conflicts am HEADER): txt.h.
   unit of rea 8cef7d5b (src/game/am.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "am.h"
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
int ai_PlayerCountYear = -1;                  /* 0x5ffd1c */
int ai_NumPlayerChars = 0;  /* 0x6f0470, number of characters of kind 5, 6, 7 */
extern char ai_DataPath[608];
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */

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
/* 0x48e070, the class 0x1e building of the character p (a master, kind 3 excepted) in *out: 2 it has
   one, 1 it is a master without one (kind 5, or kind 2 / 4 with rank above 6), 0 neither. It also recounts
   the characters of the player kinds 5 / 6 / 7 (ai_NumPlayerChars) once per ai_Time.year. */

int ai_FindMasterBuilding(AIPX *p, int *out)
{
    int i, bld;

    if (ai_PlayerCountYear < ai_Time.year) {
        ai_NumPlayerChars = 0;
        for (i = 0; i < 768; i++) {
            AIPX *c = &((AIPX *)ai_CharsT)[i];

            if (c->kind == 6 || c->kind == 7 || c->kind == 5)
                ai_NumPlayerChars++;
        }
        ai_PlayerCountYear = ai_Time.year;
    }
    if (p->kind != 3) {
        bld = (int)gm_BuildQuery(2, 5, 0x1e, 4, p->id);
        if (out)
            *out = bld;
        if (bld)
            return 2;
        if (p->kind == 5)
            return 1;
        if ((p->kind == 2 || p->kind == 4) && p->rank > 6)
            return 1;
    }
    return 0;
}
/* 0x48e120, decide of "Build Domizil" (0x40): the master's building needs an upgrade he can nearly pay */

int ai_DecideBuildDomizil(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld;
    int cost, lim, m, t;

    if (ai_FindMasterBuilding(p, (int *)&bld) && !b && (a == 0x40 || a == 0) && !he_FindEvent(2, 0, 0xd, 2, p->id)) {
        if (bld) {
            t = bld->type;
            if ((unsigned char)ai_BTypesT[t].level >= 3)
                return 0;
            if (bld->flags5a & 0x41)
                return 0;
            s2.ref = t + 1;
            s2.arg = 1;
            s1.kind = 4;
            s1.ref = bld->id;
            s1.arg = 1;
            cost = (int)(gm_BuildingKindValue(t + 1) * 0.3f);
            if (!ai_CanBuildType(a, p, 0, &s2, &s1, 0))
                return 0;
            m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
            lim = (int)(m * 0.95f);
            if (lim < cost) {
                if ((float)(cost - lim) / cost < 0.25f) {
                    o1->arg = cost - lim;
                    o1->kind = 0x12;
                    return ai_DecideTakeCredit(0x40, p, 0, o1, o2);
                }
                return 0;
            }
            *o1 = s2;
            *o2 = s1;
            goto ok;
        }
        s1.kind = 5;
        s1.ref = 0x4b;
        s1.arg = 1;
        if (!ai_CanBuildType(a, p, 0, &s1, &s2, 1))
            return 0;
        cost = gm_BuildingKindCost(p, s1.ref);
        m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
        lim = (int)(m * 0.95f);
        if (lim < cost) {
            if ((float)(cost - lim) / cost < 0.25f) {
                o1->arg = cost - lim;
                o1->kind = 0x12;
                return ai_DecideTakeCredit(0x40, p, 0, o1, o2);
            }
            return 0;
        }
        *o1 = s1;
        *o2 = s2;
        goto ok;
    }
    return 0;
ok:
    return 0x40;
}
/* 0x48e370, execute of action 0x40 "Build Domizil": on a building (kind 4) pays 0.3 of the value of the building kind s1->ref for the upgrade with message 0x1e, else builds (ai_ExecBuildBuilding) */

int ai_ExecBuildDomizil(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    AIMSG msg;
    int v;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.time = ai_Time;
        msg.car = s2->ref;
        cm_BeginGroup("building_upgrade");
        v = gm_BuildingKindValue(s1->ref);
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(v * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x40;
    }
    return ai_ExecBuildBuilding(p, s1, s2) ? 0x40 : 0;
}
/* 0x48e470, the decide of the AI method "UseSecretRoom" (0x41): a character of kind 3 takes no part; a
   master (its class 0x1e building found by p->id) without the cabinet object 0x266 wants one built
   (ai_ChooseSubAction, 0.22 of the weight), else one of the four payment ways ai_DecideSecretRoom0 .. 3
   (chosen by util_RandomU(4)) pays; 0 when none applies. */

int ai_DecideSecretRoom(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld;
    int r;

    if (p->kind == 3)
        return 0;
    if (b)
        return 0;
    if (a != 0x41 && a != 0)
        return 0;
    bld = gm_BuildQuery(2, 5, 0x1e, 4, p->id);
    if (!bld)
        return 0;
    if (!gm_Query(bld->objList, 2, 6, 0, 0x266)) {
        s1.kind = 2;
        s1.ref = 0x266;
        s1.arg = 1;
        s1.fvalue = 0.22f;
        s2.kind = 4;
        s2.ref = bld->id;
        s2.arg = 1;
        r = ai_ChooseSubAction(0x41, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_EventOnBuilding(bld->id))
        return 0;
    switch (util_RandomU(4)) {
    case 0:
        r = ai_DecideSecretRoom0(p, &s1, &s2);
        break;
    case 1:
        r = ai_DecideSecretRoom1(p, &s1, &s2);
        break;
    case 2:
        r = ai_DecideSecretRoom2(p, &s1, &s2);
        break;
    case 3:
        r = ai_DecideSecretRoom3(p, &s1, &s2);
        break;
    default:
        return 0;
    }
    if (!r)
        return 0;
    *o1 = s1;
    *o2 = s2;
    o1->ref2 = bld->id;
    return 0x41;
}
/* 0x48e620, decide helper: a character to pay for the upgrade, level 0 */

int ai_DecideSecretRoom0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, lim, cost, m;
    AIPX *c;

    if (p->money < 0x10)
        return 0;
    flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500 (bits 8, 10, 20..23) */
    if (!ai_PickPartnerChain(p, 1, &flags, 0, util_RandomU(0x20) + 25.0f, &cid))
        return 0;
    c = &((AIPX *)ai_CharsT)[cid];
    m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
    lim = (int)(m * 0.25f);
    {
        int s1v = gm_GetWorth(p->id);
        cost = (int)((gm_GetWorth(c->id) + s1v) * 0.02f);
    }
    if (lim < cost)
        return 0;
    s2->arg = cost;
    s1->arg = 0;
    s2->ref2 = c->netId;
    s2->arg2 = 0;
    s1->arg2 = 0;
    return 1;
}
/* 0x48e730, decide helper: pay in three steps, level 1 */

int ai_DecideSecretRoom1(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, t, sel, a[3];
    AIPX *c;

    flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500 (bits 8, 10, 20..23) */
    if (!ai_PickPartnerChain(p, 1, &flags, 0, util_RandomU(0x20) + 25.0f, &cid))
        return 0;
    c = &((AIPX *)ai_CharsT)[cid];
    t = amt_GetKindLevel(((unsigned char *)c)[0x166]) / 3 + 2;
    a[0] = t + 3;
    a[1] = t + 5;
    a[2] = t + 8;
    if (p->money >= a[2])
        sel = 2;
    else if (p->money >= a[1])
        sel = 1;
    else if (p->money >= a[0])
        sel = 0;
    else
        return 0;
    s1->arg = a[sel];
    s2->ref2 = c->netId;
    s2->arg2 = sel;
    s1->arg2 = 1;
    return 1;
}
/* 0x48e830, decide helper: pay in three steps, level 2 */

int ai_DecideSecretRoom2(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, sel, sel2, sum, a[6];
    float f;
    AIPX *c;

    flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500 (bits 8, 10, 20..23) */
    if (!ai_PickPartnerChain(p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &cid))
        return 0;
    c = &((AIPX *)ai_CharsT)[cid];
    f = (float)(amt_GetKindLevel(((unsigned char *)c)[0x166]) / 3 + 2);
    a[0] = (int)f;
    a[1] = (int)(f * 1.5f);
    a[2] = (int)(f + f);
    if (p->money >= a[2])
        sel = 2;
    else if (p->money >= a[1])
        sel = 1;
    else if (p->money >= a[0])
        sel = 0;
    else
        return 0;
    gm_SumListAmounts(p->inventory, ai_OwnPlayer);
    sum = gm_GetWorth(p->id) + gm_GetWorth(c->id);
    a[3] = (int)(sum * 0.01f);
    a[4] = (int)(sum * 0.015f);
    a[5] = (int)(sum * 0.02f);
    if (sum >= a[2])
        sel2 = 2;
    else if (sum >= a[1])
        sel2 = 1;
    else if (sum >= a[0])
        sel2 = 0;
    else
        return 0;
    if (sel2 >= sel)
        sel2 = sel;
    s2->arg = a[sel2 + 3];
    s1->arg = a[sel2];
    s2->ref2 = c->netId;
    s2->arg2 = sel2;
    s1->arg2 = 2;
    return 1;
}
/* 0x48e9f0, decide helper: level 3, the master pays from his own purse */

int ai_DecideSecretRoom3(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    int s, m, lim, sel, a[3];

    s = gm_GetWorth(p->id);
    m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
    lim = (int)(m * 0.25f);
    a[0] = (int)(s * 0.01f);
    a[1] = (int)(s * 0.015f);
    a[2] = (int)(s * 0.02f);
    if (lim > a[2])
        sel = 2;
    else if (lim > a[1])
        sel = 1;
    else if (lim > a[0])
        sel = 0;
    else
        return 0;
    s1->arg2 = 3;
    s2->arg2 = sel;
    s2->arg = a[sel];
    return 1;
}
/* 0x48eab0, execute of "UseSecretRoom" (0x41): the way to pay in s1->arg2 picks ai_ExecSecretRoom0..3 */

int ai_ExecSecretRoom(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    switch (s1->arg2) {
    case 0:
        return ai_ExecSecretRoom0(p, s1, s2) ? 0x41 : 0;
    case 1:
        return ai_ExecSecretRoom1(p, s1, s2) ? 0x41 : 0;
    case 2:
        return ai_ExecSecretRoom2(p, s1, s2) ? 0x41 : 0;
    case 3:
        return ai_ExecSecretRoom3(p, s1, s2) ? 0x41 : 0;
    }
    return 0;
}
/* 0x48eb50, execute of "UseSecretRoom" (0x41), way 0: pays 16 and the amount of s2, message 0x99 to the character of s1 */

int ai_ExecSecretRoom0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    AIPX *c;
    int v;

    c = (AIPX *)gm_FindChar(s1->ref2);
    if (!c)
        return 0;
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->arg);
    gm_SendMoneyDelta(p->netId, -16);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.type = 0x99;
    msg.fromId = s1->ref2;
    msg.time = ai_Time;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.dst[0] = -1;
    msg.n = msg.fromId;
    msg.dst[1] = 4;
    msg.dst[2] = 0;
    if (c->bld170)
        msg.car = c->bld170->id;
    else if (c->bld16c)
        msg.car = c->bld16c->id;
    else {
        v = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ai_PickActionBuilding((AIPER *)c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48ec90, execute of "UseSecretRoom" (0x41), way 1: pays the amount of s1 in money, message 0x99 to the character of s1 */

int ai_ExecSecretRoom1(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    AIPX *c;
    int v;

    c = (AIPX *)gm_FindChar(s1->ref2);
    if (!c)
        return 0;
    gm_SendMoneyDelta(p->netId, -(int)s1->arg);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.type = 0x99;
    msg.fromId = s1->ref2;
    msg.time = ai_Time;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.n = s1->ref2;
    msg.dst[0] = -1;
    msg.dst[1] = 1;
    msg.dst[2] = s2->arg2;
    if (c->bld170)
        msg.car = c->bld170->id;
    else if (c->bld16c)
        msg.car = c->bld16c->id;
    else {
        v = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ai_PickActionBuilding((AIPER *)c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48edc0, execute of "UseSecretRoom" (0x41), way 2: pays the goods amount of s2 and the money of s1, message 0x99 to the character of s1 */

int ai_ExecSecretRoom2(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    AIPX *c;
    int v;

    c = (AIPX *)gm_FindChar(s1->ref2);
    if (!c)
        return 0;
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->arg);
    gm_SendMoneyDelta(p->netId, -(int)s1->arg);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.type = 0x99;
    msg.fromId = s1->ref2;
    msg.time = ai_Time;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.n = s1->ref2;
    msg.dst[0] = -1;
    msg.dst[1] = 2;
    msg.dst[2] = s2->arg2;
    if (c->bld170)
        msg.car = c->bld170->id;
    else if (c->bld16c)
        msg.car = c->bld16c->id;
    else {
        v = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ai_PickActionBuilding((AIPER *)c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48ef10, execute of "UseSecretRoom" (0x41), way 3: pays the goods amount of s2, message 0x99 to no character (ch -1), building by ai_PickActionBuilding */

int ai_ExecSecretRoom3(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    int v;

    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->arg);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.fromId = s1->ref2;
    msg.time = ai_Time;
    msg.dst[2] = s2->arg2;
    msg.type = 0x99;
    msg.flags = 2;
    msg.ch = -1;
    msg.dst[0] = -1;
    msg.dst[1] = 3;
    v = -1;
    msg.n = msg.fromId;
    m.s.lo = 0x7b4c4a0;
    m.s.hi = 0;
    ai_PickActionBuilding((AIPER *)p, 0, m.q, 0, 100.0f, &v);
    msg.car = v;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48f010, decide kind 0x42: a master without a second level object wants one, or a building to pair
   with his own */

int ai_DecideCannonTower(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld, *b2;
    int r, v;
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;

    if (p->kind == 3)
        return 0;
    if (a != 0x42 && a != 0)
        return 0;
    bld = gm_BuildQuery(2, 5, 0x1e, 4, p->id);
    if (!bld)
        return 0;
    if (!gm_Query(bld->objList, 2, 6, 0, 0x256)) {
        s1.kind = 2;
        s1.ref = 0x256;
        s1.arg = 1;
        s1.fvalue = 0.22f;
        s2.kind = 4;
        s2.ref = bld->id;
        s2.arg = 1;
        r = ai_ChooseSubAction(0x42, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (!gv_FindEffect(bld->id) && !(p->flags1cc & 0x80000000)) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x80000000, 0);
        gv_RequestEffectLoad(bld);
        return 0;
    }
    m.s.lo = 0xe07c43b4;
    m.s.hi = 1;
    if (!ai_PickActionBuilding((AIPER *)p, 6, m.q, 0, (float)(util_RandomU(0x20) + 0x20), &v))
        return 0;
    gm_FindById((int)&b2, 0, 0, v);
    if (!b2)
        return 0;
    if (!ai_AimCannon(p, bld, b2))
        return 0;
    o1->kind = 4;
    o1->ref = bld->id;
    o2->kind = 4;
    o2->ref = b2->id;
    return 0x42;
}
/* 0x48f1e0, execute of "UseCannonTower" (0x42): fires the shot from the building of s1->ref at the one of s2->ref */

int ai_ExecCannonTower(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    AIBLD *x, *y;

    if (s1->kind != 4 || s2->kind != 4)
        return 0;
    gm_FindById((int)&x, 0, 0, s1->ref);
    if (!x)
        return 0;
    gm_FindById((int)&y, 0, 0, s2->ref);
    if (!y)
        return 0;
    return ai_FireCannon(p, x) ? 0x42 : 0;
}
/* 0x48f250, finds the building of a character: 2 found, 1 it has none but needs one, 0 nothing to do */

int ai_FindOwnBuilding(AIPX *p, AIBLD **out)
{
    AIBLD *b;

    if (p->kind != 3) {
        b = AI_501190_5(2, 5, 0x21, 4, p->id);
        if (out)
            *out = b;
        if (b)
            return 2;
        if (p->kind == 5)
            return 1;
        if ((p->kind == 2 || p->kind == 4) && p->rank > 6)
            return 1;
    }
    return 0;
}
/* 0x48f2b0, decide of action 0x43 "BuildSculpture": upgrades the own building (type + 1, level below 3) or builds type 0x54 on a plot; short of money (below half the inventory) by less than a quarter, it takes a credit (ai_DecideTakeCredit) */

int ai_DecideBuildSculpture(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    struct { AIARG s1, s2; } L;
    AIBLD *bl;
    int v, lim, t;

    if (!ai_FindOwnBuilding(p, &bl) || b)
        return 0;
    if (a != 0x43 && a != 0)
        return 0;
    if (he_FindEvent(2, 0, 0xd, 2, p->id))
        return 0;
    if (bl) {
        if ((unsigned char)ai_BTypesT[bl->type].level >= 3 || (bl->flags5a & 0x41))
            return 0;
        t = bl->type + 1;
        L.s2.ref = t;
        L.s2.arg = 1;
        L.s1.kind = 4;
        L.s1.ref = bl->id;
        L.s1.arg = 1;
        v = (int)(gm_BuildingKindValue(t) * 0.3f);
        if (!ai_CanBuildType(a, p, 0, &L.s2, &L.s1, 0))
            return 0;
        lim = (int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            if ((float)(v - lim) / v < 0.25f) {
                o1->arg = v - lim;
                o1->kind = 0x12;
                return ai_DecideTakeCredit(0x43, p, 0, o1, o2);
            }
            return 0;
        }
        *o1 = L.s2;
        *o2 = L.s1;
    } else {
        L.s1.kind = 5;
        L.s1.ref = 0x54;
        L.s1.arg = 1;
        if (!ai_CanBuildType(a, p, 0, &L.s1, &L.s2, 1))
            return 0;
        v = gm_BuildingKindCost(p, L.s1.ref);
        lim = (int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            if ((float)(v - lim) / v < 0.25f) {
                o1->arg = v - lim;
                o1->kind = 0x12;
                return ai_DecideTakeCredit(0x43, p, 0, o1, o2);
            }
            return 0;
        }
        *o1 = L.s1;
        *o2 = L.s2;
    }
    return 0x43;
}
/* 0x48f500, execute of action 0x43 */

int ai_ExecBuildSculpture(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    int v;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.time = ai_Time;
        msg.arg[0] = s2->ref;
        cm_BeginGroup("building_upgrade");
        v = gm_BuildingKindValue(s1->ref);
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(v * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x43;
    }
    return ai_ExecBuildBuilding(p, s1, s2) ? 0x43 : 0;
}
/* 0x48f600, decide of action 0x48 "UseAlchemistryItems": of the 15 alchemy items 0x2b8..0x2c6 the character holds, uses a random usable one (on a partner chain for the first six, on a building for the seventh), or wants a random missing one bought (year 20+, rank above 4, one year in eight) */

int ai_DecideUseAlchemyItems(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIRECX rec[15];
    int st[3];
    int r;

    if (b || (a != 0x48 && a != 0))
        goto fail;
    memset(rec, 0, sizeof(rec));
    rec[0].item = 0x2b8;
    rec[1].item = 0x2bb;
    rec[2].item = 0x2bc;
    rec[3].item = 0x2bf;
    rec[4].item = 0x2c2;
    rec[5].item = 0x2c3;
    rec[6].item = 0x2bd;
    rec[7].item = 0x2c1;
    rec[8].item = 0x2c4;
    rec[9].item = 0x2c5;
    rec[10].item = 0x2c6;
    rec[11].item = 0x2ba;
    rec[12].item = 0x2c0;
    rec[13].item = 0x2be;
    rec[14].item = 0x2b9;
    ai_FindWantedItems(p, 15, rec, st);
    if (st[0] == 6 && !st[2])
        goto fail;
    if (st[1] == 15)
        goto fail;
    if (st[0] < 6) {
        if (!st[2]) {
            if (ai_Time.year < 0x14)
                goto fail;
            if (p->rank <= 4)
                goto fail;
            if (ai_Time.year % 8 != (unsigned int)p->netId % 8)
                goto fail;
            if (p->flags1cc & 0x40000000)
                goto fail;
            r = util_RandomU(15);
            while (rec[r].value)
                r = (r + 1) % 15;
            o1->kind = 2;
            o1->arg = 1;
            o1->ref = rec[r].item;
            o1->fvalue = 0.05f;
            o1->arg2 = 1;
            o2->kind = 0;
            return 0x48;
        }
    } else if (!st[2])
        goto fail;
    r = util_RandomU(15);
    while (rec[r].value < 1)
        r = (r + 1) % 15;
    o1->kind = 1;
    o1->arg = 1;
    o1->ref = rec[r].id;
    o1->fvalue = 0.05f;
    o1->arg2 = 3;
    o2->kind = 0;
    o2->ref = -1;
    o2->arg = -1;
    if (r < 6) {
        unsigned short id;
        int flags;

        flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500 (bits 8, 10, 20..23) */
        if (!ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 10.0f, &id))
            goto fail;
        o2->kind = 7;
        o2->ref = AIC[id].netId;
        o2->arg = -1;
        o2->arg2 = 0x48;
        return 0x48;
    }
    if (r == 6) {
        int out;
        LARGE_INTEGER mask;

        mask.LowPart = 0xa0744380;
        mask.HighPart = 1;
        out = -1;
        if (ai_Partner) {
            ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &out);
            if (out != -1)
                goto done;
        }
        if (!ai_PickActionBuilding((AIPER *)p, 6, mask.QuadPart, 0, util_RandomU(0x20) + 16.0f, &out))
            goto fail;
    done:
        o2->kind = 4;
        o2->ref = out;
        o2->arg = -1;
        o2->arg2 = 0x48;
        return 0x48;
    }
    if (r == 7 && p->money < 3)
        goto fail;
    o2->arg2 = 0x48;
    return 0x48;
fail:
    return 0;
}
/* 0x48f990, execute of "UseAlchemistryItems" (0x48): sells the item, or uses it on s2->ref; when it took
   effect, the action's short-term weight w3[0] becomes w1[0] times the good's effect weight (g[3], the
   Domizil record's +0xc), minus half when it backfired; 0x48 when sold, 0x1c (the mood change) when used, else 0 */

int ai_ExecUseAlchemyItems(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bl;
    AIOBJ *o, *q;
    float *g;
    AIUSE u;

    if (s1->kind == 2) {
        if (s1->arg2 == 1) {
            cm_SendExSellObject(p->netId, -1, s1->ref, 1, ai_OwnPlayer, 0);
            cm_SendSetField(p->netId, 0x1cc, 4, 0x40000000, 0);
            return 0x48;
        }
        if (s1->arg2 == 2 && s2->kind == 4) {
            bl = AI_501190_5(2, 1, s2->ref2, 5, 0x1e);
            if (bl) {
                o = gm_Query(bl->objList, 2, 6, 0, 0x2a);
                if (o) {
                    q = gm_Query(o->list, 2, 6, 0, s1->ref);
                    if (q && q->count > 1) {
                        cm_SendExSellObject(p->netId, o->objId, s1->ref, 1, ai_OwnPlayer, 0);
                        return 0x48;
                    }
                }
            }
        }
    } else if (s1->kind == 1) {
        o = gm_Query(p->inventory, 1, 1, s1->ref);
        if (!o)
            return 0;
        g = (float *)ai_FindUsableGoodP(o->id);
        if (g) {
            u.obj = o;
            u.target = s2->ref;
            u.arg = s2->arg;
            u.effect = 0;
            u.handled = 0;
            if (ai_UseGood(p, &u) == 1) {
                if (u.effect == 1) {
                    ai_Actions[0x48].w3[0].w = ai_Actions[0x48].w1[0].w * g[3];
                    return 0x1c;
                }
                if (u.effect == -1)
                    ai_Actions[0x48].w3[0].w = ai_Actions[0x48].w1[0].w * g[3] * -0.5f;
                return 0x1c;
            }
        }
    }
    return 0;
}
/* 0x48fb70, registers five actions (0x40..0x43, 0x48) */

int ai_RegisterBuildingActions(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x40;
    strcpy(act.name, "Build Domizil");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuildDomizil;
    act.exec = ai_ExecBuildDomizil;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x41;
    strcpy(act.name, "UseSecretRoom");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideSecretRoom;
    act.exec = ai_ExecSecretRoom;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x42;
    strcpy(act.name, "UseCannonTower");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideCannonTower;
    act.exec = ai_ExecCannonTower;
    act.flags = 0x1c1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x43;
    strcpy(act.name, "BuildSculpture");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideBuildSculpture;
    act.exec = ai_ExecBuildSculpture;
    act.flags = 3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x48;
    strcpy(act.name, "UseAlchemistryItems");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideUseAlchemyItems;
    act.exec = ai_ExecUseAlchemyItems;
    act.flags = 0x1c3;
    return am_RegisterMethod(&act, a) != 0;
}
/* 0x48feb0, a random character of the building (kind 0x14, not busy with a grave action) */

AIPX *ai_PickRandomBuildingChar(AIBLD *b)
{
    unsigned short list[8];
    int n, i;

    n = 0;
    for (i = 0; i < 768; i++) {
        if (n >= 8)
            break;
        if (AIC[i].id != 0xffff && AIC[i].kind == 0x14 && AIC[i].bld == b && AIC[i].health > 0 &&
            AIC[i].jobKind != -0x78 && AIC[i].jobKind != -0x70 && AIC[i].jobKind != -0x77 &&
            AIC[i].jobKind != -0x76)
            list[n++] = i;
    }
    if (n < 2)
        return 0;
    return &AIC[list[util_RandomU(n)]];
}
/* 0x48ff50, way 0 of the "Graveyard Owner" decide (ai_DecideGraveyardOwner): with money and a 0x1fc object in the graveyard, picks one of its characters and a partner chain for message 0x88 and a building of the partner */

int ai_DecideGraveyardOwner0(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c, *q;
    unsigned short id;
    int flags;

    flags = 0;
    o1->arg2 = 0;
    o1->ref2 = b->id;
    if (p->money < 4)
        return 0;
    if (!gm_Query(b->objList, 2, 0, 0x1fc, 6))
        return 0;
    c = ai_PickRandomBuildingChar(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x88, 3, b->id))
        return 0;
    flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500 (bits 8, 10, 20..23) */
    if (!ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &id))
        return 0;
    q = &AIC[id];
    if (q->bld2)
        o2->arg = q->bld2->id;
    else if (q->bld)
        o2->arg = q->bld->id;
    else {
        union { struct { unsigned int lo, hi; } s; __int64 q; } m;
        int x;

        x = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ai_PickActionBuilding((AIPER *)p, 0, m.q, 0, 100.0f, &x);
        if (x == -1)
            return 0;
        o2->arg = x;
    }
    o2->ref2 = q->netId;
    o2->ref = c->netId;
    o2->arg2 = 4;
    return 1;
}
/* 0x4900c0, way 0 of the "Graveyard Owner" execute: sends message 0x88 (flags 2) from the building to the character with the way's AIARG (ref2, ref, arg2, arg) */

int ai_ExecGraveyardOwner0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x88;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_Time;
    msg.arg[1] = o->ref2;
    msg.arg[2] = o->ref;
    msg.arg[3] = o->arg2;
    msg.arg[0] = o->arg;
    msg.ids[0] = o->ref;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490170, way 1 of the "Graveyard Owner" decide (ai_DecideGraveyardOwner): two random factors 2..4 whose product the money must cover, a 0x211 object in the graveyard and no event 0x90 of it pending; picks a building (mask 0x1a07c43b4) of the partner, else of its own; o2 = the two factors (arg, ref) and the building (ref2) */

int ai_DecideGraveyardOwner1(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    unsigned int u1, u2;
    unsigned int n;
    int x;
    LARGE_INTEGER mask;

    u1 = util_RandomU(3);
    u2 = util_RandomU(3);
    o1->arg2 = 1;
    o1->ref2 = b->id;
    n = (u2 + 2) * (u1 + 2);
    if (n > (unsigned int)p->money)
        return 0;
    if (!gm_Query(b->objList, 2, 0, 0x211, 6))
        return 0;
    if (he_FindEvent(2, 0, 0x90, 3, b->id))
        return 0;
    x = -1;
    mask.LowPart = 0xa07c43b4;
    mask.HighPart = 1;
    if (ai_Partner) {
        ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &x);
        if (x != -1)
            goto done;
    }
    if (!ai_PickActionBuilding((AIPER *)p, 6, mask.QuadPart, 0, util_RandomU(0x20) + 19.0f, &x))
        return 0;
done:
    o2->arg = u2;
    o2->ref = u1;
    o2->ref2 = x;
    return 1;
}
/* 0x4902b0, way 1 of the "Graveyard Owner" execute: sends message 0x90 (flags 2) from the building to the character with the way's AIARG (ref2, ref, arg) */

int ai_ExecGraveyardOwner1(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x90;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_Time;
    msg.arg[1] = o->ref;
    msg.arg[2] = o->arg;
    msg.arg[0] = o->ref2;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490350, way 2 of the "Graveyard Owner" decide (ai_DecideGraveyardOwner): with a 0x204 object in the graveyard and no event 0x89 of it pending, picks one of its characters and a partner chain for message 0x89 and a building of the partner (else one of the player's, mask 0x7b4c4a0) */

int ai_DecideGraveyardOwner2(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIPX *c, *q;
    int flags;
    unsigned short id;

    flags = 0;
    o1->arg2 = 2;
    o1->ref2 = b->id;
    if (!gm_Query(b->objList, 2, 0, 0x204, 6))
        return 0;
    c = ai_PickRandomBuildingChar(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x89, 3, b->id))
        return 0;
    flags = (0xf << 20) | (5 << 8);  /* the partner flags 0xf00500, as in way 0 */
    if (!ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &id))
        return 0;
    q = &AIC[id];
    o2->ref2 = q->netId;
    o2->ref = c->netId;
    o2->arg2 = 4;
    if (q->bld2)
        o2->arg = q->bld2->id;
    else if (q->bld)
        o2->arg = q->bld->id;
    else {
        int x;

        x = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ai_PickActionBuilding((AIPER *)&AIC[ai_PlayerId], 0, m.q, 0, 100.0f, &x);
        if (x == -1)
            return 0;
        o2->arg = x;
    }
    return 1;
}
/* 0x4904c0, way 2 of the "Graveyard Owner" execute: sends message 0x89 (flags 1) from the building to the character with the way's AIARG (ref2, ref, arg2, arg) */

int ai_ExecGraveyardOwner2(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x89;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_Time;
    msg.arg[1] = o->ref2;
    msg.arg[2] = o->ref;
    msg.arg[3] = o->arg2;
    msg.arg[0] = o->arg;
    msg.ids[0] = o->ref;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490570, decide of method 0x8a */

int ai_DecideGraveyardOwner3(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c;

    o1->arg2 = 3;
    o1->ref2 = b->id;
    if (!gm_Query(b->objList, 2, 0, 0x1fb, 6))
        return 0;
    c = ai_PickRandomBuildingChar(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x8a, 3, b->id))
        return 0;
    o2->ref = c->netId;
    return util_RandomU(3) == 0;
}
/* 0x490600, way 3 of the "Graveyard Owner" execute: sends message 0x8a (flag 1) from the building to the character with the way's AIARG (ref) */

int ai_ExecGraveyardOwner3(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x8a;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_Time;
    msg.arg[0] = o->ref;
    msg.ids[0] = o->ref;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490690, decide of the AI method "Graveyard Owner" (0x3d): for a character with a job in the title group 0xd and a graveyard (a building of mask 0x20000000), runs its four ways and takes the first that can act from a random start; returns 0x3d with o1 / o2 of that way, else 0 */

int ai_DecideGraveyardOwner(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG s1[4], s2[4];
    int r[4] = { 0, 0, 0, 0 };
    int v, x, i, n;
    LARGE_INTEGER mask;

    mask.LowPart = 0x20000000;
    mask.HighPart = 0;
    if (b)
        return 0;
    if (a != 0x3d && a != 0)
        return 0;
    if (p->kind == 3)
        return 0;
    if (!gm_TitleJob(p->id, 0xd))
        return 0;
    if (!ai_PickActionBuilding((AIPER *)p, 4, mask.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById(&v, 0, 0, x);
    if (!v)
        return 0;
    r[0] = ai_DecideGraveyardOwner0(p, (AIBLD *)v, &s1[0], &s2[0]);
    r[1] = ai_DecideGraveyardOwner1(p, (AIBLD *)v, &s1[1], &s2[1]);
    r[2] = ai_DecideGraveyardOwner2(p, (AIBLD *)v, &s1[2], &s2[2]);
    r[3] = ai_DecideGraveyardOwner3(p, (AIBLD *)v, &s1[3], &s2[3]);
    i = util_RandomU(4);
    n = 4;
    do {
        if (r[i]) {
            *o1 = s1[i];
            *o2 = s2[i];
            return 0x3d;
        }
        n--;
        i = (i + 1) % 4;
    } while (n != 0);
    return 0;
}
/* 0x490830, the execute callback of "Graveyard Owner" */

int ai_ExecGraveyardOwner(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->bldId);
    if (s1 != 0) {
        switch (a->kind) {
        case 0:
            return ai_ExecGraveyardOwner0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 1:
            return ai_ExecGraveyardOwner1((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 2:
            return ai_ExecGraveyardOwner2((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 3:
            return ai_ExecGraveyardOwner3((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        }
    }
    return 0;
}
/* 0x4908d0, the decide callback of "Handle Grave" */

int ai_DecideHandleGrave(int a, AGCH *p, int b, AGARG *o1, AGARG *o2)
{
    AIBLD *bld;
    AGCH *q;
    AIBLD *list[8];
    int *r, *r2;
    int n, i, k, v, m, t2;

    r = 0;
    if (b != 0 || a != 0 || p->w050 == 0x7fff)
        return 0;
    if (ai_Time.hour % 6 != p->netId % 6)
        return 0;
    if (ai_Time.year % 3 != p->netId % 3)
        return 0;
    if (p->kind != 5) {
        q = AG_CH(p->spouse);
        if (q != 0) {
            if (q->kind == 5)
                return 0;
            if (q->sex == 0 && ai_WorksForMaster((AICHAR *)q) != 0)
                return 0;
        }
    }
    memset(list, 0, sizeof(list));
    bld = ai_BldsT;
    n = 0;
    for (i = 0; i < 256; i++, bld++) {
        if (bld->type != 0 && (bld->flags5a & 1) == 0 && ai_BTypesT[bld->type].kind == 0x1d) {
            if (n < 8)
                list[n++] = bld;
            r = gv_FindEntry(p, bld, 0, gv_EntryHeldBy, 0xff);
            if (r != 0)
                break;
        }
    }
    if (r != 0) {
        if (*r == 3)
            return 0;
        for (k = 3; k > *r; k--) {
            r2 = gv_FindEntry(0, bld, 0, gv_EntryIsFree, k);
            if (r2 != 0) {
                v = gv_MovePrice(p, *r, *r2, &i);
                m = gm_CharCurrency(p, ai_OwnPlayer);
                if ((int)(m * 0.22f) <= v)
                    return 0;
                o1->kind = 5;
                o1->bldId = bld->id;
                o1->entry = *r2;
                o1->price = v;
                o2->entry = *r;
                o2->kind = gv_EntryIndex((char *)bld->yard, r);
                return 0x3e;
            }
        }
        return 0;
    }
    if (n == 0)
        return 0;
    if (util_RandomU(8) + p->age < 0x23)
        return 0;
    bld = list[util_RandomU(n)];
    r2 = gv_FindEntry(0, bld, 0, gv_EntryIsFree, 0xff);
    if (r2 == 0)
        return 0;
    {
        int t3;

        v = gv_LeasePrice(p, 0xff, *r2, 1, &t3);
        m = gm_CharCurrency(p, ai_OwnPlayer);
    }
    if ((int)(m * 0.22f) < v)
        return 0;
    o1->kind = 4;
    o1->bldId = bld->id;
    o1->entry = *r2;
    o1->price = v;
    return 0x3e;
}
/* 0x490b90, the execute callback of "Handle Grave": reserves the grave ai_DecideHandleGrave picked (kind 4), or moves the grave with its second entry (kind 5) */

int ai_ExecHandleGrave(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->bldId);
    if (s1 != 0) {
        if (a->kind == 4) {
            gv_OrderReserveGrave(p, s1, a->entry, 10, a->price);
            return 0x3e;
        }
        if (a->kind == 5) {
            gv_OrderMoveGrave((int *)p, a->entry, (char *)s1, s2->entry, s2->kind, a->price);
            return 0x3e;
        }
    }
    return 0;
}
/* 0x490c10, registers three actions */

int ai_RegisterGraveActions(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x3d;
    strcpy(act.name, "Graveyard Owner");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideGraveyardOwner;
    act.exec = ai_ExecGraveyardOwner;
    act.flags = 0x1c3;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3e;
    strcpy(act.name, "Handle Grave");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_DecideHandleGrave;
    act.exec = ai_ExecHandleGrave;
    act.flags = 1;
    if (!am_RegisterMethod(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3f;
    strcpy(act.name, "Use Seance");
    act.eval = ai_EvalMoodWeights;
    act.decide = ai_SeanceDecide;
    act.exec = ai_SeanceExec;
    act.flags = 1;
    return am_RegisterMethod(&act, a) != 0;
}
/* 0x490e00, the callback of law 0 (RECHTSPRECHUNG_HAERTE, the severity of jurisprudence): the office holder raises or lowers it from its title group, skill 4, moods 9..11 and its guilt */

int ai_DecideLawSeverity(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n, r;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    r = gv_CulpritGuilt(p->netId);
    if (t == 0xb || t == 0xc)
        x += 6.0f;
    else if (t == 9 || t == 7)
        x += 2.0f;
    else
        x += 4.0f;
    x = r / x;
    gv_GetTableRecord(0, &d);
    if (x > 1.0f) {
        if (w > u && d.cur < 2) {
            *out = util_RandomU(2) + d.cur + 1;
            return 1;
        }
        if (x > 1.4f) {
            *out = 4;
            return 1;
        }
    } else if (u > v && d.cur > 2) {
        *out = d.cur - util_RandomU(3) - 1;
        return 1;
    }
    return 0;
}
/* 0x490fc0, the callback of law record 1 (AIDEC.fn, office 0x18 no 2, a boolean law): raises it when the office holder's skill, age and guilt say so, or the office 0x11 holder dislikes the character, and lowers it when that holder likes it */

int ai_DecideLaw1(AIPER *p, int a, int *out)
{
    AIDEC d;
    unsigned short *o;
    float f, x, u, v, w, g;
    int t, n, r;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    r = gv_CulpritGuilt(p->netId);
    if (t == 0xb || t == 0xc)
        x += 7.0f;
    else if (t == 9 || t == 1 || t == 3 || t == 2)
        x += 3.0f;
    else
        x += 5.0f;
    n = 40 - p->age;
    x = r / (n * 0.1f + x);
    gv_GetTableRecord(1, &d);
    if (x > 1.0f && d.cur == 0) {
        if (x > 1.4f || w > u) {
            *out = 1;
            return 1;
        }
    } else if (x < 0.5f && d.cur == 1 && (v > u || p->officeKind == 0x11)) {
        *out = 0;
        return 1;
    } else {
        o = amt_GetOfficeHolder(0x11);
        if (o) {
            g = gm_Liking(*o, p->id, 1);
            if (d.cur == 1) {
                if (g > 0.75f) {
                    *out = 0;
                    return 1;
                }
            } else if (d.cur == 0 && g < 0.25f) {
                *out = 1;
                return 1;
            }
        }
    }
    return 0;
}
/* 0x491200, the callback of law record 2 (AIDEC.fn, office 0x1a the bishop no 1, a three-state law: 0 faith 0, 1 faith 1, 2 tolerance): the bishop proposes its own faith when the state religion is the other one and it is loyal (loyalty >= 0.4), tolerance when it is not (below 0.4), and its own faith again from tolerance when it is loyal (above 0.8); otherwise it leaves the law */

int ai_DecideStateReligion(AIPER *p, int a, int *out)
{
    AIDEC d;

    gv_GetTableRecord(2, &d);
    if (p->faith == 0 && d.cur == 1) {
        if (p->loyalty < 0.4f)
            *out = 2;
        else
            *out = 0;
        return 1;
    }
    if (p->faith == 1 && d.cur == 0) {
        if (p->loyalty < 0.4f) {
            *out = 2;
            return 1;
        }
        *out = 1;
        return 1;
    }
    if (d.cur == 2 && p->loyalty > 0.8f) {
        *out = p->faith != 0;
        return 1;
    }
    return 0;
}
/* 0x4912e0, the callback of law record 3 (AIDEC.fn, office 0x12 no 1, a boolean law): the normalized skill 4 against the rating (x, 1.1 for the title group 7, 0.9 for 0xb / 0xc) raises the law (cur 0 -> 1) when it is below ai_FloatOne and mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when it is above and mood 10 outweighs mood 9; the tribute event accuses against it (gv_Accuse(3, 1, ...)) */

int ai_DecideLaw3(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 7)
        x *= 1.1f;
    else if (t == 0xb || t == 0xc)
        x *= 0.9f;
    gv_GetTableRecord(3, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x491430, the callback of law record 4 (AIDEC.fn, office 0x16 no 1, a boolean law): the normalized skill 3 against the rating, always x 1.1, raises the law (cur 0 -> 1) when it is below ai_FloatOne and mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when it is above and mood 10 outweighs mood 9 */

int ai_DecideLaw4(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int n;

    gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 3);
    n = ord_GetRatingByte(3);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    x *= 1.1f;
    gv_GetTableRecord(4, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x491560, the callback of law record 5 (AIDEC.fn, office 0x0e no 1, a 0..4 law, the upkeep rate gm_CharacterUpkeep reads): the rate is (3 - the growth of need 3) x 5, at most 4, and at most 1 for the title groups 8 / 9 */

int ai_DecideUpkeepRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n, t;

    n = min((int)((3.0f - p->mood[3][0]) * 5.0f), 4);
    t = gm_TitleGroup(p->jobId);
    if ((t == 8 || t == 9) && n > 1)
        n = 1;
    gv_GetTableRecord(5, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
/* 0x4915f0, the callback of law record 6 (AIDEC.fn, office 0x0a no 1, a 0..4 law, the building cost factor gm_BuildingKindCost reads): (the growth of need 3 - 2) x 5, at most 4; every step below 4 makes a new building 10% more expensive */

int ai_DecideBuildingCost(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n;

    n = min((int)((p->mood[3][0] - 2.0f) * 5.0f), 4);
    gv_GetTableRecord(6, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
/* 0x491650, the callback of law record 7 (AIDEC.fn, office 0x0b no 1, a 1..4 law, the ranking event's minimum wealth): (the growth of need 3 - 2) x 5, at most 4, at least 1; he_PostEvent67 scales the event 0x67 threshold by (5 - cur) x 0.1 + 0.7 */

int ai_DecideLaw7(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n;

    gm_TitleGroup(p->jobId);
    n = min((int)((p->mood[3][0] - 2.0f) * 5.0f), 4);
    n = max(1, n);
    gv_GetTableRecord(7, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
/* 0x4916d0, the callback of law record 9 (AIDEC.fn, office 0x0f the mayor no 1, a rate 8 .. 0x12, the income tax gm_IncomeKind1 charges): from the 4th year the tax rate is 22 / 44 / 66% of the range above the minimum by the building capacity ratio, one lower unless the title group is 1 / 2 / 3 / 5 and a random 1 .. 3 higher when the seat's goods are below a quarter of the office pay; the rate moves by a random 1 .. 3 when the current value is more than 3 away */

int ai_DecideIncomeTaxRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_Time.year >= 4) {
        gv_GetTableRecord(9, &d);
        f = ord_BuildingCapacityRatio();
        k = d.max - d.min;
        if (f < 0.33f)
            n = (int)(k * 0.22f + d.min);
        else if (f < 0.66)
            n = (int)(k * 0.44f + d.min);
        else
            n = (int)(k * 0.66f + d.min);
        if (t != 1 && t != 2 && t != 3 && t != 5)
            n--;
        b = gm_FirstBuilding(1);
        o = gm_Query(b->objList, 2, 6, 0, 0x115);
        m = gm_SumListAmounts(o->list, ai_OwnPlayer);
        if (m < (int)(amt_GetOwnedOfficePay() * 0.25f) * 3)
            n += util_RandomU(3) + 1;
        if (d.cur - 3 > n) {
            *out = d.cur - util_RandomU(3) - 1;
            *out = max(d.min, *out);
            return 1;
        }
        if (d.cur + 3 < n) {
            *out = util_RandomU(3) + d.cur + 1;
            *out = min(d.max, *out);
            return 1;
        }
    }
    return 0;
}
/* 0x491860, the callback of law record 10 (AIDEC.fn, office 0x0f the mayor no 2, a rate 6 .. 0x12, the wage tax of the title groups with a town object: gm_CharacterIncomes picks gm_IncomeKind2, record 10, over record 9 for them): from the 10th year and while a guild building (kind 0x17 .. 0x1a) exists, the rate is 22 / 44 / 66% of the range above the minimum by the building capacity ratio, one lower for the title groups 1 / 2 / 3 / 5 and a random 1 .. 3 higher when the seat's goods are below the office pay x 3; the rate moves by a random 1 .. 2 when the current value is more than 3 away */

int ai_DecideGuildTaxRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    k = 0;
    if (ai_Time.year >= 10) {
        if (gm_BuildQuery(1, 5, 0x17))
            k = 1;
        if (gm_BuildQuery(1, 5, 0x18))
            k++;
        if (gm_BuildQuery(1, 5, 0x19))
            k++;
        if (gm_BuildQuery(1, 5, 0x1a))
            k++;
        if (k) {
            gv_GetTableRecord(10, &d);
            f = ord_BuildingCapacityRatio();
            k = d.max - d.min;
            if (f < 0.33f)
                n = (int)(k * 0.22f + d.min);
            else if (f < 0.66)
                n = (int)(k * 0.44f + d.min);
            else
                n = (int)(k * 0.66f + d.min);
            if (t == 1 || t == 2 || t == 3 || t == 5)
                n--;
            b = gm_FirstBuilding(1);
            o = gm_Query(b->objList, 2, 6, 0, 0x115);
            m = gm_SumListAmounts(o->list, ai_OwnPlayer);
            if (m < amt_GetOwnedOfficePay() * 3)
                n += util_RandomU(3) + 1;
            if (d.cur - 3 > n) {
                *out = d.cur - util_RandomU(2) - 1;
                *out = max(d.min, *out);
                return 1;
            }
            if (d.cur + 3 < n) {
                *out = util_RandomU(2) + d.cur + 1;
                *out = min(d.max, *out);
                return 1;
            }
        }
    }
    return 0;
}
/* 0x491a40, the callback of law record 11 (AIDEC.fn, office 0x15, a church office: hst.c's BISCHOF token takes 0x15 / 0x1a, a rate 6 .. 0xa): the record's rate is the percent of a character's wage that gm_IncomeKind5 (f3gm.c 0x4fa3d0) takes and gives to all players; from the worth mood (mood 3, gm_UpdateWorthMood) the rate rises with the mood for the title group 7 and falls with it for the other groups, clamped between the record's min and max; it moves only when it is above (group 7) / below (the others) the current value */

int ai_DecideChurchTaxRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, n, k;

    t = gm_TitleGroup(p->jobId);
    gv_GetTableRecord(11, &d);
    k = d.max - d.min;
    if (t == 7) {
        n = (int)((p->mood[3][0] - 2.0) * k + d.min);
        n = min(max(d.min, n), d.max);
        if (n > d.cur) {
            *out = n;
            return 1;
        }
    } else {
        n = (int)((3.0f - p->mood[3][0]) * k + d.min);
        n = min(max(d.min, n), d.max);
        if (n < d.cur) {
            *out = n;
            return 1;
        }
    }
    return 0;
}
/* 0x491b20, the callback of law record 12 (AIDEC.fn, office 0x0c, a rate 5 .. 0xb): the record's rate is the percent of a character's trade sum (v1a0 + v1a4) that gm_IncomeKind4 (f3gm.c 0x4fa2a0) takes; from the 4th year the rate is pushed down to min + 0 .. 2 when it is above a third of the range for the title group 8, and up to max - 0 .. 2 when it is below two thirds of the range for the other groups */

int ai_DecideTradeTaxRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_Time.year >= 4) {
        gv_GetTableRecord(12, &d);
        k = d.max - d.min;
        if (t == 8) {
            if (d.cur > (int)(k * 0.33f + d.min)) {
                *out = util_RandomU(3) + d.min;
                return 1;
            }
        } else if (d.cur < (int)(k * 0.66f + d.min)) {
            *out = d.max - util_RandomU(3);
            return 1;
        }
    }
    return 0;
}
/* 0x491bf0, the callback of law record 13 (AIDEC.fn, office 0x0e no 2, a rate 0xa .. 0x19): the record's rate is the percent of an inheritance (ORDCHAR.v1a8, the worth of a taken-over character, gm_TakeOverCharacter) that gm_IncomeKind3 (f3gm.c 0x4fa160) takes; it scales the range above the minimum by the worth mood (3 - mood 3) and the deciding character's life (f01c / 700), clamped 0 .. 1, and moves only when it is more than 3 away from the current value */

int ai_DecideInheritanceTaxRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x;
    int k, n;

    x = max(min((3.0f - p->mood[3][0]) * (p->f01c / 700.0f), 1.0f), 0.0f);
    gv_GetTableRecord(13, &d);
    k = d.max - d.min;
    n = (int)(k * x + d.min);
    if (abs(n - d.cur) > 3) {
        *out = n;
        return 1;
    }
    return 0;
}
/* 0x491cb0, the callback of law record 14 (AIDEC.fn, office 0x10 no 1, a rate 0xa .. 0x19): the record's current value is the bank's rate (ai_CalcMeisterBank, ai.c 0x4836a0, steers a bank's v65 toward the record's +0x18; ai_DecideTakeCredit takes cur - 2 into its offer rating); from the 4th year the rate is pushed up to max - 0 .. 5 when it is below 55% of the range for the title group 9, and down to min + 0 .. 5 when it is above 45% for the other groups */

int ai_DecideBankRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_Time.year >= 4) {
        gv_GetTableRecord(14, &d);
        k = d.max - d.min;
        if (t == 9) {
            if (d.cur < (int)(k * 0.55f + d.min)) {
                *out = d.max - util_RandomU(6);
                return 1;
            }
        } else if (d.cur > (int)(k * 0.45f + d.min)) {
            *out = util_RandomU(6) + d.min;
            return 1;
        }
    }
    return 0;
}
/* 0x491d80, the callback of law record 15 (AIDEC.fn, office 0x19 no 1, a 0 .. 4 law): the record's cur indexes amt_Pay (1.1, 1.05, 1.0, 0.95, 0.9), the factor of an office's salary that amt_GetOwnedOfficePay (amt.c 0x4ea990) and gm_CharacterWages (f3gm.c) read; from the 4th year the target factor is 4 when the town's stock (the first building's 0x115 object) is below 3 x the office pay, 0 when it is above 12 x, and 3 / 2 / 1 between (3x / 6x / 8x); the factor moves one step towards it, and only when it is at least 2 away */

int ai_DecideOfficePayRate(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    int m, k, n;

    if (ai_Time.year < 4)
        return 0;
    gv_GetTableRecord(15, &d);
    ord_BuildingCapacityRatio();
    b = gm_FirstBuilding(1);
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    m = gm_SumListAmounts(o->list, ai_OwnPlayer);
    k = amt_GetOwnedOfficePay();
    if (m < k * 3)
        n = 4;
    else if (m > k * 12)
        n = 0;
    else if (m > k * 8)
        n = 1;
    else if (m > k * 6)
        n = 2;
    else
        n = 3;
    if (abs(d.cur - n) < 2)
        return 0;
    if (n > d.cur)
        n = d.cur + 1;
    else if (n < d.cur)
        n = d.cur - 1;
    *out = max(min(n, d.max), d.min);
    return 1;
}
/* 0x491e80, the callback of law record 16 (AIDEC.fn, office 0x0d no 1, a boolean law): the fallback of 0x496b10 takes its cur != 0 to evaluate an action with the character's guilt (ai_EvalGuiltWeights, gv_CulpritGuilt) instead of the mood weights; the growth of needs 6 / 7 (the larger, times 1.2 for the title groups 0xb / 0xc, 0.8 for 7, else 1.1) against 0.5 turns it on when mood 9 outweighs mood 11, and off when mood 11 outweighs mood 9 */

int ai_DecideGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x, u, w;
    int t;

    t = gm_TitleGroup(p->jobId);
    x = max(p->mood[6][0] - 2.0, p->mood[7][0] - 2.0);
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb || t == 0xc)
        x *= 1.2f;
    else if (t == 7)
        x *= 0.8f;
    else
        x *= 1.1f;
    gv_GetTableRecord(16, &d);
    if (x < 0.5f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 0.5 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x491fb0, the callback of law record 17 (AIDEC.fn, office 0x19 no 2, a boolean law): ai_EvalUsePerfume (0x49c0d0, the eval callback of the goods actions 0x1c .. 0x22) takes its cur != 0 to evaluate those actions with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the growth of need 10 - 2 (times 1.5 for the title groups 5 / 6, 0.9 for 7) against 0.5 turns it on when mood 9 outweighs mood 11, and off when mood 11 outweighs mood 9 */

int ai_DecideGoodsGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x, u, w;
    int t;

    t = gm_TitleGroup(p->jobId);
    x = (float)(p->mood[10][0] - 2.0);
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 5 || t == 6)
        x *= 1.5f;
    else if (t == 7)
        x *= 0.9f;
    gv_GetTableRecord(17, &d);
    if (x < 0.5f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 0.5 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x4920c0, the callback of law record 18 (AIDEC.fn, office 0x12 no 1, a boolean law): ai_EvalUseTrank (0x49c320, the eval callback of the goods actions 0x1c .. 0x22 of the potion action 0x23) takes its cur != 0 to evaluate those actions with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the growth of need 10 - 2 (times 1.5 for the title groups 5 / 6, 0.9 for 7) against 0.5 turns it on when mood 9 outweighs mood 11, and off when mood 11 outweighs mood 9 */

int ai_DecideTrankGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x, u, w;
    int t;

    t = gm_TitleGroup(p->jobId);
    x = (float)(p->mood[10][0] - 2.0);
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 5 || t == 6)
        x *= 1.5f;
    else if (t == 7)
        x *= 0.9f;
    gv_GetTableRecord(18, &d);
    if (x < 0.5f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 0.5 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x4921d0, the callback of law record 19 (AIDEC.fn, office 0x13 no 1, a boolean law): ai_EvalThreaten (0x49cdd0, the eval callback of action 0x27 "Drohen") takes its cur != 0 to evaluate that action with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the normalized skill 4 against its rating (always x0.8) against 1 turns it on when mood 9 outweighs mood 11, and off when mood 11 outweighs mood 9 */

int ai_DecideThreatenGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, w;
    int n;

    gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    x *= 0.8f;
    gv_GetTableRecord(19, &d);
    if (x < 1.0f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x4922e0, the callback of law record 20 (AIDEC.fn, office 0x13 no 2, a boolean law): no reader of its cur is in the exe outside the table; the normalized skill 4 against its rating (always x0.8) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw20(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int n;

    gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 3);
    n = ord_GetRatingByte(3);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    x *= 1.1f;
    gv_GetTableRecord(20, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492410, the callback of law record 21 (AIDEC.fn, office 0x11 no 1, a boolean law): ai_EvalBeatUp (0x499b80, the eval callback of action 0x18 "Verpruegeln"; ai_DecideBeatUp also reads cur as the fight's value cur * 2 + 1) takes its cur != 0 to evaluate that action with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the normalized skill 3 against its rating (always x1.1) against 1 raises the law when mood 11 outweighs mood 9, and lowers it when mood 10 outweighs mood 9 */

int ai_DecideBeatUpGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int n;

    gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 3);
    n = ord_GetRatingByte(3);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    x *= 1.1f;
    gv_GetTableRecord(21, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492540, the callback of law record 22 (AIDEC.fn, office 0x14 no 1, a boolean law): ai_EvalBlackmail (0x49d600, the eval callback of action 0x2c "Erpressen") takes its cur != 0 to evaluate that action with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the normalized skill 4 against its rating (x1.1 for the title groups 0xb / 0xc, else x0.9) against 1 raises the law when mood 11 outweighs mood 9, and lowers it when mood 10 outweighs mood 9 */

int ai_DecideBlackmailGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb || t == 0xc)
        x *= 1.1f;
    else
        x *= 0.9f;
    gv_GetTableRecord(22, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492680, the callback of law record 23 (AIDEC.fn, office 0x17 no 1, a boolean law): no reader of its cur is in the exe outside the table; the normalized skill 2 against its rating (x1.3 for title group 0xb, else x0.9) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw23(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb)
        x *= 1.3f;
    else
        x *= 0.9f;
    gv_GetTableRecord(23, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x4927c0, the callback of law record 24 (AIDEC.fn, office 0x16 no 2, a boolean law): ai_EvalSabotage (0x4995d0, the eval callback of action 0x17, sabotage; ai_DecideSabotage also reads cur for its 1-in-4 skip) takes its cur != 0 to evaluate that action with the character's guilt (ai_EvalGuiltWeights) instead of the mood weights; the normalized skill 2 against its rating (x1.1 for the title groups 0xb / 0xc, else x1) against 1 raises the law when mood 11 outweighs mood 9, and lowers it when mood 10 outweighs mood 9 */

int ai_DecideSabotageGuiltWeights(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb || t == 0xc)
        x *= 1.1f;
    gv_GetTableRecord(24, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492900, the callback of law record 25 (AIDEC.fn, office 0x17 no 2, a boolean law): no reader of its cur is in the exe outside the table; the normalized skill 3 against its rating (x1.3 for title group 0xc, else x0.9) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw25(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 3);
    n = ord_GetRatingByte(3);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xc)
        x *= 1.3f;
    else
        x *= 0.9f;
    gv_GetTableRecord(25, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492a40, the callback of law record 26 (AIDEC.fn, office 0x14 no 2, a boolean law): no reader of its cur is in the exe outside the table; the normalized skill 2 against its rating (x1.3 for title group 0xb, else x0.9) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw26(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb)
        x *= 1.3f;
    else
        x *= 0.9f;
    gv_GetTableRecord(26, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492b80, the callback of law record 8 (AIDEC.fn, office 0x1b no 1, a boolean law): no reader of its cur is in the exe outside the table; it needs a graveyard (the building types 0x4b..0x4d) and then decides for a player character (kind 5) by whether it owns one (own with cur 1 -> 0, otherwise with cur 0 -> 1), else by the normalized skill 2 against its rating (x1.2 for kind 3, else x0.8) against 1: mood 11 over mood 9 raises the law, mood 10 over mood 9 lowers it */

int ai_DecideLaw8(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    float f, x, u, v, w;
    int n, i, found, own;

    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    found = 0;
    own = 0;
    b = ai_BldsT;
    for (i = 0; i < 256; i++, b++) {
        if (b->type == 0x4b || b->type == 0x4c || b->type == 0x4d) {
            found++;
            if (p->kind != 5)
                break;
            if (b->owner == p->id) {
                own = 1;
                break;
            }
        }
    }
    if (!found)
        return 0;
    gv_GetTableRecord(8, &d);
    if (p->kind == 5) {
        if (own) {
            if (d.cur == 1) {
                *out = 0;
                return 1;
            }
        } else if (d.cur == 0) {
            *out = 1;
            return 1;
        }
        return 0;
    }
    if (p->kind == 3)
        x *= 1.2f;
    else
        x *= 0.8f;
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492d50, the callback of law record 27 (AIDEC.fn, office 0x1a no 2, a boolean law): the graveyard's work (he_AllocDecay) and raise-dead (gv_OrderRaiseDead) accuse against it (gv_Accuse(0x1b, ...)); the normalized skill 2 against its rating (x1.4 for the title group 0xd, else x0.9) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw27(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xd)
        x *= 1.4f;
    else
        x *= 0.9f;
    gv_GetTableRecord(27, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
/* 0x492e90, the callback of law record 28 (AIDEC.fn, office 0x1b no 2, a boolean law): the event 0x9d, the disease of a figure (he_AllocGypsyApproach), accuses against it (gv_Accuse(0x1c, ...)); the normalized skill 2 against its rating (x1.4 for the title group 0xe, else x0.9) against 1 raises the law (cur 0 -> 1) when mood 11 outweighs mood 9, and lowers it (cur 1 -> 0) when mood 10 outweighs mood 9 */

int ai_DecideLaw28(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xe)
        x *= 1.4f;
    else
        x *= 0.9f;
    gv_GetTableRecord(28, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}


/* dcp unit split: the declarations that moved to amr (copies: drop what am does not use) */
#undef AM_WRITE
#undef AM_READ

/* unlinked: the "Use Seance" decide callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_RegisterGraveActions points */
int ai_SeanceDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    return 0;
}
/* unlinked: the "Use Seance" execute callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_RegisterGraveActions points */
int ai_SeanceExec(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0;
}
/* unlinked: the "Objekt kaufen (AP)" decide callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040;
   two distinct symbols, as the original stores both callbacks as immediates (one symbol is loaded once into eax) */
int ai_BuyObjectApDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    return 0;
}
/* unlinked: the "Objekt kaufen (AP)" execute callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040 */
int ai_BuyObjectApExec(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0;
}
/* unlinked: the "Betrieb upgraden (AP)" decide callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040;
   two distinct symbols, as the original stores both callbacks as immediates (one symbol is loaded once into eax) */
int ai_UpgradeBusinessApDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    return 0;
}
/* unlinked: the "Betrieb upgraden (AP)" execute callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040 */
int ai_UpgradeBusinessApExec(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0;
}
