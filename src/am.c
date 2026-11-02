/* am.c -- the AI "methods" unit (tag "am_RegisterMethod(): Methode '%s' ..." 0x5ffde8), 0x48e070..0x4a7140:
   the action table loader, am_RegisterMethod, the registration of actions 0x01..0x3b (0x4a16f0) and their
   decide (int, character, flag, out1, out2 -> action kind or 0), execute (character, arg1, arg2) and evaluate
   callbacks, the decision callbacks of the AI decision table 0x611258 (29 records of 0x24 bytes, callback at
   +0x20), the partner / building finders and the AI cannon shot. Starts with its first global ai_Unk5ffd1c
   (-1 at 0x5ffd1c) right after the AI unit's strings; its one .data string block (in function order) ends with
   "%sgebaeude/*%s_%c.ogr" 0x600478, the next block is unk_4a7140.c's. Names are ours.
   Open: am.h keeps the joined parts' character views (AIPX / AGCH / AIC4 / G5CH / AIPER) and duplicate callee
   prototypes / cast macros; they need unifying with ai.h's types.
   Open: not included yet (dcp headers conflicts am HEADER): txt.h; held by own declarations that differ from
   the owner's: txt_Keys.
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
extern unsigned char txt_Keys;  /* @original ai_Unk498c70 (0x498c70, byte) tests flags1c8 & 0x1000000 as &txt_Keys + 0x181c0 of this byte view: the char[0x4800][0x50] spelling, a view macro or the literal flip its add operands at +0x4cc (shape) */

#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char), @original: the byte-matched callers (ai_Unk49ed10, ai_Unk498c70) read the result without the unsigned char mask (no `and eax,0xff` after the call) */

#include "hst.h"
#include "unk_4a7140.h"
#include "unk_4b39f0.h"
int ai_Unk5ffd1c = -1;                  /* 0x5ffd1c */
G6RING * ai_Unk60034c = ai_Unkf83c20;  /* 0x60034c, the next entry */
int ai_Unk600350 = 1;  /* 0x600350, the step */
int ai_Unk6f0470 = 0;  /* 0x6f0470, number of characters of kind 5, 6, 7 */
AGCH ai_Unk6f0478 = {0};  /* 0x6f0478 */
G9SHOT ai_Unk6f0698 = {0};  /* 0x6f0698 */
unsigned int ai_Unk6f070c = 0;  /* 0x6f070c, the number of entries */
int ai_Unk6f2464 = 0;  /* 0x6f2464, set: write the action table */
char ai_DataPath[608] = {0};  /* 0x75dca0, the data path */
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */
G6RING ai_Unkf83c20[32] = {0};  /* 0xf83c20 */
char ai_Unkf83da0[260] = {0};  /* 0xf83da0, the ini file of the actions */
int ai_Unkf83ea4 = 0;  /* 0xf83ea4, the depth of the evaluation stack */
AIACT ai_Unkf83ec0[0x49] = {0};  /* 0xf83ec0, the registered actions (0x49 of them: the init clears 0x49 * 0x94 bytes) */
/* ai_Unkf84c40: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84c60: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cd4: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cf4: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf86894: view of ai_Unkf83ec0 (+0x29d4, in the last action record) */
/* ai_Unkf868b4: view of ai_Unkf83ec0 (+0x29f4, in the last action record) */
int ai_Unkf86900[73] = {0};  /* 0xf86900, the actions being evaluated */

#include "time_view.h"
#include "ls.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "sv.h"
#include "tr.h"
#include "unk_58bbd0.h"
#include "f3gm.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "d3_interface.h"
#include "d3_tools.h"
#include "unk_458810.h"
#include "unk_5d33b0.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define d3t_GetObjectBoundsV ((void (*)(void *, void *, void *))d3t_GetObjectBounds)  /* 0x413150 (d3_tools.c: D3VECTOR *, D3VECTOR *, D3OBJECT *): this unit's untyped view */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define gv_OrderMoveGraveI ((void (*)(AGCH *, int, void *, int, int, int))gv_OrderMoveGrave)  /* 0x4f0e80 (gv.c: int *c, char *o): @original the byte-matched ai_Unk490b90 passes its AGCH record and s1 as they are */
#define gm_CharLevelS ((int (*)(unsigned short, int))gm_CharLevel)  /* 0x505e60 (f3gm.c: unsigned short id, so the cast is now the prototype itself): @original this unit pushes the id as 16 bits (byte-matched ai_Unk494a30 / ai_Unk494ac0 / ai_Unk494d40) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_Unk497c80 / ai_Unk49c880) */
#define amt_GetKindB ((int (*)(unsigned char, void *))amt_GetKind)  /* 0x4e6f80 (amt.h: int k): @original ai_Unk4a05e0 passes (unsigned char)k without widening */
/* 0x48e070, the building of a master (kind != 3) in *out; 2: has one, 1: has a good one, 0: none */

int ai_Unk48e070(AIPX *p, int *out)
{
    int i, n, bld;

    if (ai_Unk5ffd1c < ai_TimeT.year) {
        ai_Unk6f0470 = 0;
        for (i = 0; i < 768; i++) {
            AIPX *c = &((AIPX *)ai_CharsT)[i];

            if (c->kind == 6 || c->kind == 7 || c->kind == 5)
                ai_Unk6f0470++;
        }
        ai_Unk5ffd1c = ai_TimeT.year;
    }
    if (p->kind != 3) {
        bld = (int)gm_BuildQuery(2, 5, 0x1e, 4, p->id);
        if (out)
            *out = bld;
        if (bld)
            return 2;
        if (p->kind == 5)
            return 1;
        if ((p->kind == 2 || p->kind == 4) && p->unk00d > 6)
            return 1;
    }
    return 0;
}
/* 0x48e120, decide kind 0x40: the master's building needs an upgrade he can nearly pay
   @deep read in full against the original 0x48e120..0x48e368: every statement, callee, constant (0.3f,
   0.95f, 0.25f) and field matches (93 + 47 + 20 equal instructions). Left: the original cross-jumps the
   two 'borrow' tails: the else branch (gm_Unk509dc0 cost) computes need in ecx (mov ecx, edi / sub ecx,
   eax), stores o1->arg and jumps (jmp 0x48e264) into the first branch's tail (mov ecx, [esp+0x50] / mov
   byte [eax], 0x12 / push .. / call ai_Unk49eec0); ours keeps both tails inline with the o1 stores after
   the pushes ('- mov dword ptr [eax + 8], edi / - mov byte ptr [eax], 0x12' after 'push 0x40'). Tried:
   kind before arg (no change), a shared goto label for kind + call (100 ins differ), for arg + kind + call
   (62 ins differ, both tails fully merged): reverted. Block layout / tail merging, not meaning. */

int ai_Unk48e120(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld;
    int cost, lim, m, need, t;

    if (ai_Unk48e070(p, (int *)&bld) && !b && (a == 0x40 || a == 0) && !he_FindEvent(2, 0, 0xd, 2, p->id)) {
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
            if (!ai_Unk4a5140(a, p, 0, &s2, &s1, 0))
                return 0;
            m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
            lim = (int)(m * 0.95f);
            if (lim < cost) {
                need = cost - lim;
                if ((float)need / cost < 0.25f) {
                    o1->arg = need;
                    o1->kind = 0x12;
                    return ai_Unk49eec0(0x40, p, 0, o1, o2);
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
        if (!ai_Unk4a5140(a, p, 0, &s1, &s2, 1))
            return 0;
        cost = gm_Unk509dc0(p, s1.ref);
        m = gm_SumListAmounts(p->inventory, ai_OwnPlayer);
        lim = (int)(m * 0.95f);
        if (lim < cost) {
            need = cost - lim;
            if ((float)need / cost < 0.25f) {
                o1->arg = need;
                o1->kind = 0x12;
                return ai_Unk49eec0(0x40, p, 0, o1, o2);
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
/* 0x48e370, execute of action 0x40 "Build Domizil": on a building (kind 4) pays 0.3 of its value for the upgrade with message 0x1e, else builds (ai_ExecBuildBuilding)
   @deep read the whole function against the original: equal instruction count (66/66), same statements,
   calls, arguments and branch layout; the remaining hunks are allocation only: the original keeps p in esi
   and s2 in edx (loaded before sub esp, reused in the else branch push edx, reloaded from [esp+0x110] for
   gm_BuildingKindValue), ours keeps s2 in esi and p in edi; the ai_Time copy is scheduled ecx/edx/ecx/dx
   vs ours ecx/edx/eax/cx. Tried the early-return form (if kind != 4 return ...): 56 ins worse, reverted. */

int ai_ExecBuildDomizil(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    AIMSG msg;
    int cost;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.to = p->netId;
        msg.time = ai_TimeT;
        msg.type = 0x1e;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.car = s2->ref;
        cm_BeginGroup("building_upgrade");
        cost = (int)(gm_BuildingKindValue(s2->ref) * 0.3f);
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, cost);
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x40;
    }
    return ai_ExecBuildBuilding(p, s1, s2) ? 0x40 : 0;
}
/* 0x48e470, decide kind 0x41: a master without a workshop level object wants one built, otherwise one of the
   four ways to pay (0x48e620 .. 0x48e9f0) */

int ai_Unk48e470(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
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
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.ref = bld->id;
        s2.arg = 1;
        r = ai_Unk495a50(0x41, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_Unk56f170(bld->id))
        return 0;
    switch (util_RandomU(4)) {
    case 0:
        r = ai_Unk48e620(p, &s1, &s2);
        break;
    case 1:
        r = ai_Unk48e730(p, &s1, &s2);
        break;
    case 2:
        r = ai_Unk48e830(p, &s1, &s2);
        break;
    case 3:
        r = ai_Unk48e9f0(p, &s1, &s2);
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

int ai_Unk48e620(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, lim, cost, m;
    AIPX *c;

    if (p->money < 0x10)
        return 0;
    flags = ((unsigned)&cm_SendRing + 0x4973c0);
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

int ai_Unk48e730(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, t, sel, a[3];
    AIPX *c;

    flags = ((unsigned)&cm_SendRing + 0x4973c0);
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

int ai_Unk48e830(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, sel, sel2, sum, a[6];
    float f;
    AIPX *c;

    flags = ((unsigned)&cm_SendRing + 0x4973c0);
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

int ai_Unk48e9f0(AIPX *p, AIARGX *s1, AIARGX *s2)
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
/* 0x48eab0, execute dispatcher of 0x48e470: by the way to pay in s1->unk10 */

int ai_Unk48eab0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    switch (s1->arg2) {
    case 0:
        return ai_ExecSecretRoom0(p, s1, s2) ? 0x41 : 0;
    case 1:
        return ai_ExecSecretRoom1(p, s1, s2) ? 0x41 : 0;
    case 2:
        return ai_Unk48edc0(p, s1, s2) ? 0x41 : 0;
    case 3:
        return ai_Unk48ef10(p, s1, s2) ? 0x41 : 0;
    }
    return 0;
}
/* 0x48eb50, execute of "UseSecretRoom" (0x41), way 0: pays 16 and the amount of s2, message 0x99 to the character of s1
   @deep read the whole function against the original: same statements, calls and arguments; restructured
   the building choice as the original's flow (bld170 stores directly; bld16c or the ai_PickActionBuilding
   result merged into one msg.car store). Left: allocation and scheduling only: the ai_Time copy is
   scheduled ecx/edx/ecx/dx (ours edx/ecx/eax/cx, msg.type store moved), msg.ch is stored from ecx before
   add esp 0x18 (ours edx after), bld170->id goes through edx and is stored in the branch where ours loads
   eax and jumps to the shared store (jmp +0xdf: ours to +0x116, the original's to +0x122); the ternary
   form was worse (56 ins), reverted. */

int ai_ExecSecretRoom0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    AIPX *c;
    int v, car;

    c = (AIPX *)gm_FindChar(s1->ref2);
    if (!c)
        return 0;
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->arg);
    gm_SendMoneyDelta(p->netId, -16);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.time = ai_TimeT;
    msg.fromId = s1->ref2;
    msg.type = 0x99;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.dst[0] = -1;
    msg.n = msg.fromId;
    msg.dst[1] = 4;
    msg.dst[2] = 0;
    if (c->bld170)
        msg.car = c->bld170->id;
    else {
        if (c->bld16c)
            car = c->bld16c->id;
        else {
            v = -1;
            m.s.lo = 0x7b4c4a0;
            m.s.hi = 0;
            ((int (*)(AIPX *, int, __int64, int, float, int *))ai_PickActionBuilding)(c, 0, m.q, 0, 100.0f, &v);
            car = v;
        }
        msg.car = car;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48ec90, execute of "UseSecretRoom" (0x41), way 1: pays the amount of s1 in money, message 0x99 to the character of s1
   @deep read the whole function against the original: same statements, calls and arguments; put the
   message fields in the original's store order (to, time, type, fromId, flags, ch, n = s1->ref2,
   dst[0..2]). Left: allocation and scheduling: the ai_Time copy goes ecx/edx/ecx/dx where ours goes
   edx/ecx/eax/cx, s2 is loaded into edx before msg.flags (ours eax after), and our compiler tail-merges
   one building-id store (bld170 branch: mov eax,[eax+1]; jmp to the shared store) where the original
   stores in each of the three branches (je at +0xc8: ours to +0xcf, the original's to +0xd4). */

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
    msg.time = ai_TimeT;
    msg.type = 0x99;
    msg.fromId = s1->ref2;
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
        ((int (*)(AIPX *, int, __int64, int, float, int *))ai_PickActionBuilding)(c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48edc0, execute, way 2: pays the amount of s2 and the amount of s1 */

int ai_Unk48edc0(AIPX *p, AIARGX *s1, AIARGX *s2)
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
    TMX(msg) = NOWX;
    msg.fromId = s1->ref2;
    msg.type = 0x99;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.dst[1] = 2;
    msg.dst[2] = s2->arg2;
    msg.dst[0] = -1;
    msg.n = msg.fromId;
    if (c->bld170)
        msg.car = c->bld170->id;
    else if (c->bld16c)
        msg.car = c->bld16c->id;
    else {
        v = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ((int (*)(AIPX *, int, __int64, int, float, int *))ai_PickActionBuilding)(c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48ef10, execute, way 3: pays the amount of s2, message to nobody */

int ai_Unk48ef10(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    int v;

    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->arg);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    TMX(msg) = NOWX;
    msg.fromId = s1->ref2;
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
        ((int (*)(AIPX *, int, __int64, int, float, int *))ai_PickActionBuilding)(p, 0, m.q, 0, 100.0f, &v);
    msg.car = v;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x48f010, decide kind 0x42: a master without a second level object wants one, or a building to pair
   with his own */

int ai_Unk48f010(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
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
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.ref = bld->id;
        s2.arg = 1;
        r = ai_Unk495a50(0x42, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (!gv_FindEffect(bld->id) && !(p->flags1cc & 0x80000000)) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x80000000, 0);
        unk_Unk4eea70(bld);
        return 0;
    }
    m.s.lo = 0xe07c43b4;
    m.s.hi = 1;
    if (!((int (*)(AIPX *, int, __int64, int, float, int *))ai_PickActionBuilding)(p, 6, m.q, 0, (float)(util_RandomU(0x20) + 0x20), &v))
        return 0;
    gm_FindById((int)&b2, 0, 0, v);
    if (!b2)
        return 0;
    if (!ai_Unk4a67a0(p, bld, b2))
        return 0;
    o1->kind = 4;
    o1->ref = bld->id;
    o2->kind = 4;
    o2->ref = b2->id;
    return 0x42;
}
/* 0x48f1e0, execute of 0x48f010 */

int ai_Unk48f1e0(AIPX *p, AIARGX *s1, AIARGX *s2)
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
    return ai_Unk4a6ae0(p, x) ? 0x42 : 0;
}
/* 0x48f250, finds the building of a character: 2 found, 1 it has none but needs one, 0 nothing to do */

int ai_Unk48f250(AIPX *p, AIBLD **out)
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
        if ((p->kind == 2 || p->kind == 4) && p->unk00d > 6)
            return 1;
    }
    return 0;
}
/* 0x48f2b0, decide of action 0x43 */

int ai_Unk48f2b0(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    struct { AIARG s1, s2; } L;
    AIBLD *bl;
    int v, lim, d, t;

    if (!ai_Unk48f250(p, &bl) || b)
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
        if (!ai_Unk4a5140(a, p, 0, &L.s2, &L.s1, 0))
            return 0;
        lim = (int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            d = v - lim;
            if (!((float)d / v < 0.25f))
                return 0;
            o1->arg = d;
            o1->kind = 0x12;
            return ai_Unk49eec0(0x43, p, 0, o1, o2);
        }
        *o1 = L.s2;
        *o2 = L.s1;
    } else {
        L.s2.kind = 5;
        L.s2.ref = 0x54;
        L.s2.arg = 1;
        if (!ai_Unk4a5140(a, p, 0, &L.s1, &L.s2, 1))
            return 0;
        v = gm_Unk509dc0(p, L.s1.ref);
        lim = (int)(gm_SumListAmounts(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            d = v - lim;
            if (!((float)d / v < 0.25f))
                return 0;
            o1->arg = d;
            o1->kind = 0x12;
            return ai_Unk49eec0(0x43, p, 0, o1, o2);
        }
        *o1 = L.s1;
        *o2 = L.s2;
    }
    return 0x43;
}
/* 0x48f500, execute of action 0x43 */

int ai_Unk48f500(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    int v;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.time = ai_TimeT;
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
/* 0x48f600, decide of action 0x48 */

int ai_Unk48f600(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIRECX rec[15];
    int st[3];
    int r;

    if (b)
        goto fail;
    if (a != 0x48 && a != 0)
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
    ai_Unk4a5760(p, 15, rec, st);
    if (st[0] == 6 && !st[2])
        goto fail;
    if (st[1] == 15)
        goto fail;
    if (st[0] < 6) {
        if (!st[2]) {
            if (ai_TimeT.year < 0x14)
                goto fail;
            if (p->unk00d <= 4)
                goto fail;
            if (ai_TimeT.year % 8 != (p->netId & 7))
                goto fail;
            if (p->flags1cc & 0x40000000)
                goto fail;
            r = util_RandomU(15);
            while (rec[r].unk04)
                r = (r + 1) % 15;
            o1->kind = 2;
            o1->arg = 1;
            o1->ref = rec[r].item;
            o1->unk14 = 0.05f;
            o1->arg2 = 1;
            o2->kind = 0;
            return 0x48;
        }
    } else if (!st[2])
        goto fail;
    r = util_RandomU(15);
    while (rec[r].unk04 < 1)
        r = (r + 1) % 15;
    o1->kind = 1;
    o1->arg = 1;
    o1->ref = rec[r].unk08;
    o1->unk14 = 0.05f;
    o1->arg2 = 3;
    o2->kind = 0;
    o2->ref = -1;
    o2->arg = -1;
    if (r < 6) {
        unsigned short id;
        int flags;

        flags = ((unsigned)&cm_SendRing + 0x4973c0);
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

        out = -1;
        if (ai_Partner) {
            AI_4A63F0_7(ai_Partner, 4, 0xa0744380, 1, 0, 100.0f, &out);
            if (out != -1)
                goto done;
        }
        if (!AI_4A63F0_7(p, 6, 0xa0744380, 1, 0, util_RandomU(0x20) + 16.0f, &out))
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
/* 0x48f990, execute of action 0x48 */

int ai_Unk48f990(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bl;
    AIOBJ *o, *q;
    AIGDX *g;
    AIUSE u;

    if (s1->kind == 2) {
        if (s1->arg2 == 1) {
            cm_SendExSellObjectI(p->netId, -1, s1->ref, 1, ai_OwnPlayer, 0);
            cm_SendSetField(p->netId, 0x1cc, 4, 0x40000000, 0);
            return 0x48;
        }
        if (s1->arg2 == 2 && s2->kind == 4) {
            bl = AI_501190_5(2, 1, s2->ref2, 5, 0x1e);
            if (bl) {
                o = gm_Query(bl->objList, 2, 6, 0, 0x2a);
                if (o) {
                    q = gm_Query(o->unk14, 2, 6, 0, s1->ref);
                    if (q && q->unk0e > 1) {
                        cm_SendExSellObjectI(p->netId, o->objId, s1->ref, 1, ai_OwnPlayer, 0);
                        return 0x48;
                    }
                }
            }
        }
    } else if (s1->kind == 1) {
        o = gm_Query(p->inventory, 1, 1, s1->ref);
        if (!o)
            return 0;
        g = ai_Unk5ba620(o->id);
        if (g) {
            u.obj = o;
            u.unk04 = s2->ref;
            u.unk08 = s2->arg;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    ai_Unkf868b4 = ai_Unkf86894 * g->unk0c;
                    return 0x1c;
                }
                if (u.unk0c == -1)
                    ai_Unkf868b4 = ai_Unkf86894 * g->unk0c * -0.5f;
                return 0x1c;
            }
        }
    }
    return 0;
}
/* 0x48fb70, registers five actions (0x40..0x43, 0x48) */

int ai_Unk48fb70(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x40;
    strcpy(act.name, "Build Domizil");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48e120;
    act.exec = ai_ExecBuildDomizil;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x41;
    strcpy(act.name, "UseSecretRoom");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48e470;
    act.exec = ai_Unk48eab0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x42;
    strcpy(act.name, "UseCannonTower");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f010;
    act.exec = ai_Unk48f1e0;
    act.flags = 0x1c1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x43;
    strcpy(act.name, "BuildSculpture");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f2b0;
    act.exec = ai_Unk48f500;
    act.flags = 3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x48;
    strcpy(act.name, "UseAlchemistryItems");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f600;
    act.exec = ai_Unk48f990;
    act.flags = 0x1c3;
    return ai_Unk495520(&act, a) != 0;
}
/* 0x48feb0, a random character of the building (kind 0x14, not busy with a grave action) */

AIPX *ai_Unk48feb0(AIBLD *b)
{
    unsigned short list[8];
    int n, i;

    n = 0;
    for (i = 0; i < 768; i++) {
        if (n >= 8)
            break;
        if (AIC[i].id != 0xffff && AIC[i].kind == 0x14 && AIC[i].bld == b && AIC[i].unk008 > 0 &&
            AIC[i].unk180 != -0x78 && AIC[i].unk180 != -0x70 && AIC[i].unk180 != -0x77 &&
            AIC[i].unk180 != -0x76)
            list[n++] = i;
    }
    if (n < 2)
        return 0;
    return &AIC[list[util_RandomU(n)]];
}
/* 0x48ff50, decide of method 0x88 */

int ai_Unk48ff50(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c, *q;
    unsigned short id;
    int flags, x;

    o1->arg2 = 0;
    o1->ref2 = b->id;
    flags = 0;
    if (p->money < 4)
        return 0;
    if (!gm_Query(b->objList, 2, 0, 0x1fc, 6))
        return 0;
    c = ai_Unk48feb0(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x88, 3, b->id))
        return 0;
    flags = ((unsigned)&cm_SendRing + 0x4973c0);
    if (!ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &id))
        return 0;
    q = &AIC[id];
    if (q->bld2)
        o2->arg = q->bld2->id;
    else if (q->bld)
        o2->arg = q->bld->id;
    else {
        x = -1;
        AI_4A63F0_7(p, 0, 0x7b4c4a0, 0, 0, 100.0f, &x);
        if (x == -1)
            return 0;
        o2->arg = x;
    }
    o2->ref2 = q->netId;
    o2->ref = c->netId;
    o2->arg2 = 4;
    return 1;
}
/* 0x4900c0, execute of method 0x88 */

int ai_Unk4900c0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x88;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[1] = o->ref2;
    msg.arg[2] = o->ref;
    msg.ids[0] = o->ref;
    msg.arg[3] = o->arg2;
    msg.arg[0] = o->arg;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490170, decide of method 0x90 */

int ai_Unk490170(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    unsigned short u1, u2;
    unsigned int n;
    int x;

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
    if (ai_Partner) {
        AI_4A63F0_7(ai_Partner, 4, 0xa07c43b4, 1, 0, 100.0f, &x);
        if (x != -1)
            goto done;
    }
    if (!AI_4A63F0_7(p, 6, 0xa07c43b4, 1, 0, util_RandomU(0x20) + 19.0f, &x))
        return 0;
done:
    o2->arg = u2;
    o2->ref = u1;
    o2->ref2 = x;
    return 1;
}
/* 0x4902b0, execute of method 0x90 */

int ai_Unk4902b0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x90;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[1] = o->ref;
    msg.arg[2] = o->arg;
    msg.arg[0] = o->ref2;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490350, decide of method 0x89 */

int ai_Unk490350(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c, *q;
    unsigned short id;
    int flags, x;

    o1->arg2 = 2;
    o1->ref2 = b->id;
    flags = 0;
    if (!gm_Query(b->objList, 2, 0, 0x204, 6))
        return 0;
    c = ai_Unk48feb0(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x89, 3, b->id))
        return 0;
    flags = ((unsigned)&cm_SendRing + 0x4973c0);
    if (!ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &id))
        return 0;
    q = &AIC[id];
    o2->ref2 = q->netId;
    o2->ref = c->netId;
    o2->arg2 = 4;
    if (q->bld2) {
        o2->arg = q->bld2->id;
        return 1;
    }
    if (q->bld == 0) {
        x = -1;
        AI_4A63F0_7(&AIC[ai_PlayerId], 0, 0x7b4c4a0, 0, 0, 100.0f, &x);
        if (x == -1)
            return 0;
    } else
        x = q->bld->id;
    o2->arg = x;
    return 1;
}
/* 0x4904c0, execute of method 0x89 */

int ai_Unk4904c0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x89;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[1] = o->ref2;
    msg.arg[2] = o->ref;
    msg.ids[0] = o->ref;
    msg.arg[3] = o->arg2;
    msg.arg[0] = o->arg;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490570, decide of method 0x8a */

int ai_Unk490570(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c;

    o1->arg2 = 3;
    o1->ref2 = b->id;
    if (!gm_Query(b->objList, 2, 0, 0x1fb, 6))
        return 0;
    c = ai_Unk48feb0(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x8a, 3, b->id))
        return 0;
    o2->ref = c->netId;
    return util_RandomU(3) == 0;
}
/* 0x490600, execute of method 0x8a */

int ai_Unk490600(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x8a;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[0] = o->ref;
    msg.ids[0] = o->ref;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
/* 0x490690, decide of "Graveyard Owner" (0x3d) */

int ai_Unk490690(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG s1[4], s2[4];
    int r[4] = { 0, 0, 0, 0 };
    int v, x, i, n, fl = 0x20000000;

    if (b)
        return 0;
    if (a != 0x3d && a != 0)
        return 0;
    if (p->kind == 3)
        return 0;
    if (!AI_5044B0_US(p->id, 0xd))
        return 0;
    if (!AI_4A63F0_7(p, 4, fl, 0, 0, 100.0f, &x))
        return 0;
    AI_4FEA50_4(&v, 0, 0, x);
    if (!v)
        return 0;
    r[0] = ai_Unk48ff50(p, (AIBLD *)v, &s1[0], &s2[0]);
    r[1] = ai_Unk490170(p, (AIBLD *)v, &s1[1], &s2[1]);
    r[2] = ai_Unk490350(p, (AIBLD *)v, &s1[2], &s2[2]);
    r[3] = ai_Unk490570(p, (AIBLD *)v, &s1[3], &s2[3]);
    i = util_RandomU(4);
    for (n = 4; n; n--) {
        if (r[i]) {
            *o1 = s1[i];
            *o2 = s2[i];
            return 0x3d;
        }
        i = (i + 1) % 4;
    }
    return 0;
}
/* 0x490830, the execute callback of "Graveyard Owner" */

int ai_Unk490830(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->unk0c);
    if (s1 != 0) {
        switch (a->kind) {
        case 0:
            return ai_Unk4900c0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 1:
            return ai_Unk4902b0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 2:
            return ai_Unk4904c0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 3:
            return ai_Unk490600((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        }
    }
    return 0;
}
/* 0x4908d0, the decide callback of "Handle Grave" */

int ai_Unk4908d0(int a, AGCH *p, int b, AGARG *o1, AGARG *o2)
{
    AIBLD *bld;
    AGCH *q;
    AIBLD *list[8];
    int *r, *r2;
    int n, i, k, v, m, t2;

    r = 0;
    if (b != 0 || a != 0 || p->unk050 == 0x7fff)
        return 0;
    if (ai_TimeT.hour % 6 != p->netId % 6)
        return 0;
    if (ai_TimeT.year % 3 != p->netId % 3)
        return 0;
    if (p->kind != 5) {
        q = AG_CH(p->unk05c);
        if (q != 0) {
            if (q->kind == 5)
                return 0;
            if (q->unk009 == 0 && ai_WorksForMaster((AICHAR *)q) != 0)
                return 0;
        }
    }
    memset(list, 0, sizeof(list));
    bld = ai_BldsT;
    n = 0;
    for (i = 0; i < 256; i++, bld = (AIBLD *)((char *)bld + 0xa9)) {
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
                v = unk_Unk4f2440(p, *r, *r2, &i);
                m = gm_CharCurrency(p, ai_OwnPlayer);
                if ((int)(m * 0.22f) <= v)
                    return 0;
                o1->kind = 5;
                o1->unk0c = bld->id;
                o1->unk04 = *r2;
                o1->unk08 = v;
                o2->unk04 = *r;
                o2->kind = AG_4EF6B0(bld->unk65, r);
                return 0x3e;
            }
        }
        return 0;
    }
    if (n == 0)
        return 0;
    if (util_RandomU(8) + p->unk00a < 0x23)
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
    o1->unk0c = bld->id;
    o1->unk04 = *r2;
    o1->unk08 = v;
    return 0x3e;
}
/* 0x490b90, the execute callback of "Handle Grave" */

int ai_Unk490b90(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->unk0c);
    if (s1 != 0) {
        if (a->kind == 4) {
            gv_OrderReserveGrave(p, s1, a->unk04, 10, a->unk08);
            return 0x3e;
        }
        if (a->kind == 5) {
            gv_OrderMoveGraveI(p, a->unk04, s1, s2->unk04, s2->kind, a->unk08);
            return 0x3e;
        }
    }
    return 0;
}
/* 0x490c10, registers three actions */

int ai_Unk490c10(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x3d;
    strcpy(act.name, "Graveyard Owner");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk490690;
    act.exec = ai_Unk490830;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3e;
    strcpy(act.name, "Handle Grave");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4908d0;
    act.exec = ai_Unk490b90;
    act.flags = 1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3f;
    strcpy(act.name, "Use Seance");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_SeanceDecide;
    act.exec = ai_SeanceExec;
    act.flags = 1;
    return ai_Unk495520(&act, a) != 0;
}
/* 0x490e00, decision 0 */

int ai_Unk490e00(AIPER *p, int a, int *out)
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
/* 0x490fc0, decision 1 */

int ai_Unk490fc0(AIPER *p, int a, int *out)
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
    n = 40 - p->unk00a;
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
/* 0x491200, decision 2 */

int ai_Unk491200(AIPER *p, int a, int *out)
{
    AIDEC d;

    gv_GetTableRecord(2, &d);
    if (p->unk00c == 0 && d.cur == 1) {
        if (p->unk1d0 < 0.4f)
            *out = 2;
        else
            *out = 0;
        return 1;
    }
    if (p->unk00c == 1 && d.cur == 0) {
        if (p->unk1d0 < 0.4f) {
            *out = 2;
            return 1;
        }
        *out = 1;
        return 1;
    }
    if (d.cur == 2 && p->unk1d0 > 0.8f) {
        *out = p->unk00c != 0;
        return 1;
    }
    return 0;
}
/* 0x4912e0, decision 3 */

int ai_Unk4912e0(AIPER *p, int a, int *out)
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
/* 0x491430, decision 4 */

int ai_Unk491430(AIPER *p, int a, int *out)
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
/* 0x491560, decision 5 */

int ai_Unk491560(AIPER *p, int a, int *out)
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
/* 0x4915f0, decision 6 */

int ai_Unk4915f0(AIPER *p, int a, int *out)
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
/* 0x491650, decision 7 */

int ai_Unk491650(AIPER *p, int a, int *out)
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
/* 0x4916d0, decision 9 */

int ai_Unk4916d0(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_TimeT.year >= 4) {
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
        m = gm_SumListAmounts(o->unk14, ai_OwnPlayer);
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
/* 0x491860, decision 10 */

int ai_Unk491860(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    k = 0;
    if (ai_TimeT.year >= 10) {
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
            m = gm_SumListAmounts(o->unk14, ai_OwnPlayer);
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
/* 0x491a40, decision 11 */

int ai_Unk491a40(AIPER *p, int a, int *out)
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
/* 0x491b20, decision 12 */

int ai_Unk491b20(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_TimeT.year >= 4) {
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
/* 0x491bf0, decision 13 */

int ai_Unk491bf0(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x;
    int k, n;

    x = max(min((3.0f - p->mood[3][0]) * (p->unk01c / 700.0f), 1.0f), 0.0f);
    gv_GetTableRecord(13, &d);
    k = d.max - d.min;
    n = (int)(k * x + d.min);
    if (abs(n - d.cur) > 3) {
        *out = n;
        return 1;
    }
    return 0;
}
/* 0x491cb0, decision 14 */

int ai_Unk491cb0(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_TimeT.year >= 4) {
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
/* 0x491d80, decision 15 */

int ai_Unk491d80(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    int m, k, n;

    if (ai_TimeT.year < 4)
        return 0;
    gv_GetTableRecord(15, &d);
    ord_BuildingCapacityRatio();
    b = gm_FirstBuilding(1);
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    m = gm_SumListAmounts(o->unk14, ai_OwnPlayer);
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
/* 0x491e80, decision 16 */

int ai_Unk491e80(AIPER *p, int a, int *out)
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
/* 0x491fb0, decision 17 */

int ai_Unk491fb0(AIPER *p, int a, int *out)
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
/* 0x4920c0, decision 18 */

int ai_Unk4920c0(AIPER *p, int a, int *out)
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
/* 0x4921d0, decision 19 */

int ai_Unk4921d0(AIPER *p, int a, int *out)
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
/* 0x4922e0, decision 20 */

int ai_Unk4922e0(AIPER *p, int a, int *out)
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
/* 0x492410, decision 21 */

int ai_Unk492410(AIPER *p, int a, int *out)
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
/* 0x492540, decision 22 */

int ai_Unk492540(AIPER *p, int a, int *out)
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
/* 0x492680, decision 23 */

int ai_Unk492680(AIPER *p, int a, int *out)
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
/* 0x4927c0, decision 24 */

int ai_Unk4927c0(AIPER *p, int a, int *out)
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
/* 0x492900, decision 25 */

int ai_Unk492900(AIPER *p, int a, int *out)
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
/* 0x492a40, decision 26 */

int ai_Unk492a40(AIPER *p, int a, int *out)
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
/* 0x492b80, decision 8: a graveyard (building types 0x4b..0x4d) */

int ai_Unk492b80(AIPER *p, int a, int *out)
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
/* 0x492d50, decision 27 */

int ai_Unk492d50(AIPER *p, int a, int *out)
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
/* 0x492e90, decision 28 */

int ai_Unk492e90(AIPER *p, int a, int *out)
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
const AIREC10 ai_Unk5ecfd8[7] = { { 0, 10, 20.0f, 40.0f }, { 1, 13, 40.0f, 60.0f }, { 2, 16, 60.0f, 80.0f }, { 3, 19, 80.0f, 100.0f }, { 4, 23, 100.0f, 120.0f }, { 5, 27, 120.0f, 140.0f }, { 6, 32, 140.0f, 160.0f } };  /* 0x5ecfd8 */
const int ai_Unk5ed100[16] = { 1, 5, 7, 11, 13, 17, 19, 23, 745, 749, 751, 755, 757, 761, 763, 767 };  /* 0x5ed100, random steps */
const int ai_Unk5ed140[16] = { 1, 3, 5, 7, 11, 13, 17, 19, 237, 239, 243, 245, 249, 251, 253, 255 };  /* 0x5ed140, random steps */
const int ai_Unk5ed180[] = { 1, 3, 5, 7, 11, 13, 17, 19, 23, 29 };  /* 0x5ed180, ring steps */
const int ai_Unk5ed1a8 = 31;  /* 0x5ed1a8, the last of the ring sizes before it */
const G9BALL ai_Unk5edfe0[3] = { { 2000.0f, 3000.0f, 0.0599999987f }, { 2450.0f, 3850.0f, 0.0649999976f }, { 3600.0f, 5950.0f, 0.075000003f } };  /* 0x5edfe0 */
const float ai_Unk5eee08 = 7874.0f;  /* 0x5eee08, 7874 */

/* 0x492fd0, makes a random character in ai_Unk6f0478 from kind k, returns its stats | k */
unsigned int ai_Unk492fd0(unsigned int k)
{
    float t[15] = {0.2f, 0.55f, 0.85f, 0.95f, 1.0f, 0.1f, 0.4f, 0.7f, 0.9f, 1.0f, 0.05f, 0.25f, 0.55f, 0.8f, 1.0f};
    float r;
    int idx, i, base, mul;
    unsigned int v, ret;
    char c;

    r = util_RandomF();
    if (k == 1)
        idx = 0;
    else
        idx = (k != 2) + 1;
    for (i = 0; i < 5; i++) {
        if (r <= t[idx * 5 + i]) {
            k = i + 1;
            break;
        }
    }
    if (idx == 1)
        v = util_RandomU(3);
    else {
        v = util_RandomU(2);
        if (idx == 0)
            v = v & 0xffff;
        else
            v = idx - (v & 0xffff);
    }
    r = util_RandomF();
    ret = k;
    for (i = 0; i < 5; i++) {
        if (r <= t[v * 5 + i]) {
            ret = (i + 1) * 0x100;
            break;
        }
    }
    memset(&ai_Unk6f0478, 0, sizeof(ai_Unk6f0478));
    ai_Unk6f0478.unk009 = (unsigned char)util_RandomU(2);
    if (ai_Unk6f0478.unk009 == 0)
        strncpy(ai_Unk6f0478.name, txt_MaleFirstNames[util_RandomU(0xbf)], 15);
    else
        strncpy(ai_Unk6f0478.name, txt_FemaleFirstNames[util_RandomU(0x70)], 15);
    switch (k) {
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
    default:
        base = k;
        mul = k;
    }
    for (i = 0; i < 5; i++) {
        c = (char)util_RandomU(mul);
        ai_Unk6f0478.stat[i] = c + (char)base + 1;
    }
    ai_Unk6f0478.kind = 10;
    return ret | k;
}
/* 0x493200, starts a duel between p and a fresh opponent of kind k (1, 2 or 4) */

int ai_Unk493200(AGCH *p, AGDUEL *d, int k)
{
    AGFIN *fin;
    unsigned int stats;
    int m, r;
    double x;
    float old;

    fin = (AGFIN *)gm_PlayerRecord((AICHAR *)p);
    if (p == 0 || p->kind != 6 || d == 0 || (k != 1 && k != 2 && k != 4))
        return 0;
    d->unk00 = 0;
    stats = ai_Unk492fd0(k);
    d->total = 0;
    m = gm_ConvertMoney(gm_GetWorth(p->id), ai_OwnPlayer);
    x = m;
    if (10000.0 > x)
        x = 10000.0;
    r = gm_PlayerFactor((int)((x / (log10(x) + 1.0) * 0.10000000149011612) * (k * 0.25f)), ai_OwnPlayer);
    d->unk00 = r;
    d->unk04 = (int)(r * 0.1f);
    d->kind1 = 0;
    d->unk2c = 0;
    d->kind2 = 0;
    d->unk50 = 0;
    if (fin->unk58 < util_RandomF()) {
        d->side1 = p;
        d->unk14 = 0xffff;
        d->side2 = &ai_Unk6f0478;
        d->unk38 = stats;
        fin->unk58 = util_RandomF() * 0.11f + fin->unk58 + 0.2f;
    } else {
    d->side2 = p;
    d->unk38 = 0xffff;
    d->side1 = &ai_Unk6f0478;
    d->unk14 = stats;
    old = fin->unk58;
    fin->unk58 = (old - 0.2f) - util_RandomF() * 0.11f;
    }
    d->phase = 1;
    return 1;
}
/* 0x493390, 1 when (the kind of side c, k) is a pair the duel accepts */

int ai_Unk493390(AGDUEL *d, AGCH *c, int k)
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
/* 0x4933d0, as 0x493390, and sets the kind of side c */

int ai_Unk4933d0(AGDUEL *d, AGCH *c, int k)
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
/* 0x493420, one side gets a die (k == 0) or a roll of dice (k != 0) */

void ai_Unk493420(AGDUEL *d, AGCH *c, int k)
{
    unsigned char *t, *n, lvl;
    float f, r;
    int i, j, sum, w, best, a, b, g, extra;
    short h, sv;

    if (c == d->side1) {
        t = d->t1;
        n = &d->n1;
    } else {
        t = (unsigned char *)c;
        n = (unsigned char *)c;
        if (c == d->side2) {
            t = d->t2;
            n = &d->n2;
        }
    }
    f = gm_GetSkill(c, 2);
    lvl = (int)(f * 100.0f) / 20;
    if (lvl >= 4)
        lvl = 4;
    if (k != 0) {
        h = util_RandomU(3);
        *n = 3;
        if (f > 0.8888f) {
            sv = util_RandomU(0x10);
            extra = 3 - (sv != 0);
        } else if (f > 0.5555f) {
            sv = util_RandomU(0x10);
            extra = 2 - (sv != 0);
        } else {
            sv = util_RandomU(f > 0.3333f ? 4 : 8);
            extra = sv == 0;
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
                a = i + 1;
                break;
            }
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
                b = i + 1;
                break;
            }
        }
        r = util_RandomF();
        for (i = 0; i < 6; i++) {
            if (r <= ag_Chance[lvl][i]) {
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
            for (i = 0; i < 3; i++) {
                if (extra == 0)
                    return;
                w = t[i] + 1;
                if (w > 6)
                    w = 6;
                t[i] = (unsigned char)w;
                extra--;
            }
            return;
        }
        if (h != 1) {
            j = util_RandomU(3);
            w = t[j] + extra;
            if (w > 6)
                w = 6;
            t[j] = (unsigned char)w;
            return;
        }
        best = -1;
        lvl = 6;
        for (i = 0; i < 3; i++) {
            if (t[i] < lvl) {
                best = i;
                lvl = t[i];
            }
        }
        if (best < 0)
            return;
        w = t[best] + extra;
        if (w > 6)
            w = 6;
        t[best] = (unsigned char)w;
        return;
    }
    f = gm_GetSkill(c, 0);
    r = util_RandomF();
    sum = 0;
    for (i = 0; i < *n; i++)
        sum += t[i];
    if (f <= 0.66) {
        if (f <= 0.44) {
            if (sum <= 0xe)
                goto roll6;
            best = -1;
        } else if (sum <= 0xd) {
            goto roll5;
        } else
            best = -1;
    } else {
        if (sum <= 0xc)
            goto roll4;
        best = -1;
    }
    goto pick;
roll4:
    if (sum < 6)
        best = 1;
    else
        best = util_RandomU(3) - 1;
    goto pick;
roll5:
    if (sum < 5)
        best = 1;
    else
        best = util_RandomU(3) - 1;
    goto pick;
roll6:
    if (sum < 4)
        best = 1;
    else
        best = util_RandomU(3) - 1;
pick:
    for (j = 0; j < 6; j++) {
        if (r <= ag_Chance[lvl][j])
            break;
    }
    if (j >= 6) {
        t[*n] = (unsigned char)w;
        (*n)++;
        return;
    }
    w = best + 1 + j;
    if (w >= 6)
        w = 6;
    else if (w < 2)
        w = 1;
    t[*n] = (unsigned char)w;
    (*n)++;
}
/* 0x4937a0, side c acts on the duel with k (1 and 2 roll dice, 3 and 4 only change the kind) */

int ai_Unk4937a0(AGDUEL *d, AGCH *c, int k)
{
    if (ai_Unk493390(d, c, k) == 0)
        return 0;
    switch (k) {
    case 1:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk00);
        d->total += d->unk00;
        ai_Unk493420(d, c, 1);
        ai_Unk4933d0(d, c, k);
        return 1;
    case 2:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk04);
        d->total += d->unk04;
        ai_Unk493420(d, c, 0);
        ai_Unk4933d0(d, c, k);
        return 1;
    case 3:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk04);
        d->total += d->unk04;
        ai_Unk4933d0(d, c, k);
        return 1;
    case 4:
        ai_Unk4933d0(d, c, k);
        return 1;
    default:
        return 0;
    }
}
/* 0x493890, what side c does next in the duel (0 none, 1 ... 4) */

int ai_Unk493890(AGDUEL *d, AGCH *c)
{
    unsigned char b1, b2;
    unsigned char *t;
    unsigned int u;
    int me, n, i, sum, s;
    float f, g;

    if (c->kind == 10) {
        if (c == d->side1) {
            s = d->kind1;
            u = d->unk14;
            b1 = d->n1;
            t = d->t1;
            b2 = d->n2;
            me = 0;
        } else {
            if (c != d->side2)
                return 0;
            s = d->kind2;
            u = d->unk38;
            b1 = d->n2;
            t = d->t2;
            b2 = d->n1;
            me = 1;
        }
        n = b1;
        i = 0;
        sum = 0;
        if (n > 0) {
            do {
                sum += t[i];
                i++;
            } while (i < n);
        }
        switch (s) {
        case 0:
            return 1;
        case 1:
        case 2:
            sum = 0x11 - sum;
            if (sum > 5)
                return 2;
            if (sum == 0)
                return 3;
            f = ((1.0f - (u & 0xff) * 0.2f) * 0.25f + 1.0f) * (sum * 0.16666667f);
            if (util_RandomF() < f)
                return 2;
            break;
        case 3:
            f = (sum + me) * 0.05882353f;
            if (1.0f <= f)
                return 3;
            g = (float)(int)(6 - (u & 0xff));
            f = f - b2 * 3.5f;
            if (f > g)
                return 4;
            break;
        default:
            return 0;
        }
        return 3;
    }
    return 0;
}
/* 0x493a40, the next phase of the duel */

void ai_Unk493a40(AGDUEL *d)
{
    int ph, i, n, sum1, sum2, s1, s2;

    ph = d->phase;
    i = 0;
    s1 = 0;
    s2 = 0;
    sum2 = 0;
    sum1 = 0;
    if (ph == 1) {
        d->phase = 2;
        return;
    }
    if (ph == 2) {
        if (d->t1[0] == d->t1[1] && d->t1[0] == d->t1[2])
            s1 = d->t1[2] + d->t1[1] + d->t1[0];
        if (d->t2[0] == d->t2[1] && d->t2[0] == d->t2[2])
            s2 = d->t2[2] + d->t2[1] + d->t2[0];
        if (s1 <= s2) {
            if (s2 > 0) {
                d->phase = 6;
                return;
            }
            if (d->kind1 == 4) {
                d->phase = 6;
                return;
            }
            d->phase = (d->kind2 == 4) * 2 + 3;
            return;
        }
    } else {
        n = d->n1;
        if (n > 0) {
            do {
                sum1 += d->t1[i];
                i++;
            } while (i < n);
        }
        i = 0;
        n = d->n2;
        if (n > 0) {
            do {
                sum2 += d->t2[i];
                i++;
            } while (i < n);
        }
        if (ph == 3) {
            if (sum1 < 0x12 && d->kind1 != 4) {
                if (d->kind1 == 3 && d->kind2 == 3) {
                    d->phase = (sum1 <= sum2) + 5;
                    return;
                }
                d->phase = 4;
                return;
            }
            d->phase = 6;
            return;
        }
        if (ph != 4)
            return;
        if (sum2 < 0x12 && d->kind2 != 4) {
            if (d->kind1 == 3 && d->kind2 == 3) {
                d->phase = (sum1 <= sum2) + 5;
                return;
            }
            d->phase = 3;
            return;
        }
    }
    d->phase = 5;
}
/* 0x493be0, a message that makes o's owner (character p) take a victim or let go */

int ai_Unk493be0(AGCH *p, AIOBJ *o)
{
    struct {
        int a;
        int id;
        int c[4];
    } rec[6];
    int info[6];
    AGCH *q, *w;
    int r, nn, cnt, i;
    unsigned char v;

    q = 0;
    r = util_RandomU(3);
    if (p == 0 || o == 0 || p->sel == 0 || p->money < 2)
        return 0;
    if (p->sel == 7) {
        if (p->officeKind != 0) {
            float d;
            int x;

            if (amt_GetKindB(p->officeKind, info) == 0)
                return 0;
            nn = amt_GetColumnOffices(info[1], 6, rec);
            if (nn == 0)
                return 0;
            cnt = nn;
            i = util_RandomU(nn);
            do {
                if (p->netId != rec[i].id && (w = AG_CH(rec[i].id)) != 0) {
                    d = gm_Liking(p->id, w->id, 1);
                    x = util_RandomU(30);
                    if (d < x + 40.0f) {
                        q = w;
                        break;
                    }
                }
                nn--;
                i = (i + 1) % cnt;
            } while (nn != 0);
        }
    } else if (p->sel == 10) {
        int flags;
        int x;

        flags = 0xc00;
        x = util_RandomU(30);
        if (ai_Unk4a5b50(p, 1, &flags, 0, x + 40.0f, (unsigned int *)&x) != 0)
            q = (AGCH *)&ai_CharsT[x & 0xffff];
    }
    if (q != 0) {
        v = 1;
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &q->netId);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = (unsigned char)util_RandomU(8);
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        v = (unsigned char)util_RandomU(8);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
        return 1;
    }
    if (p->sel != 3 && p->sel != 4 && r == 0)
        return 0;
    {
        int neg;

        v = 1;
        neg = -1;
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &neg);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = 0xff;
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
    }
    return 1;
}
/* 0x493ea0, picks a character for p (its watchers, listed at 0x68) */

AGCH *ai_Unk493ea0(AGCH *p)
{
    AGCH *list[5];
    AGCH *q, *s, *found;
    int n, m, i, k;
    int *lp;

    found = 0;
    m = 0;
    n = 0;
    lp = p->list;
    for (i = 0; i < 5; i++, lp++) {
        q = AG_CH(*lp);
        if (q != 0) {
            n++;
            if (q->unk008 > 0 && q->unk00a > 11)
                list[m++] = q;
        }
    }
    if (m != 0)
        found = list[util_RandomU(m)];
    if (p->kind == 5) {
        if (found != 0)
            return found;
        if (n != 0) {
            int neg;

            q = AG_CH(p->unk05c);
            neg = -1;
            if (q != 0) {
                cm_BeginFieldBlock(q, q->netId);
                cm_AddFieldDelta(4, 1, 0x5c, &neg);
                cm_SendFieldDeltas();
            }
            for (i = 0; i < 5; i++) {
                s = AG_CH(p->list[i]);
                if (s != 0) {
                    if (s->unk060 == p->netId)
                        k = 1;
                    else if (s->unk064 != p->netId)
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
        q = AG_CH(p->unk05c);
        if (q != 0 && q->unk008 > 0 && q->kind > 1 && q->kind < 10 && (q->flags1c8 & 0x40000) == 0)
            return q;
        if (found != 0 && found->kind > 1 && found->kind < 10)
            return found;
    }
    return 0;
}

/* ---- 0x494020..0x494e70 (was unk_494020.c): decision / rating callbacks that compare two characters
   (closeness, rank, random rolls), a plan's tries, pair and group ratings ---- */

/* 0x494020, decides whether a plan of a character is worth it */
int ai_Unk494020(AIPLAN *q)
{
    AIC4 *a, *b, *c;
    unsigned int n;
    float lim, x, d, sum, rank;

    if (q->unk04 <= 3.0f)
        return 0;
    if (q->unk04 >= 18.0f)
        return 1;
    n = ai_Unk4942a0(q);
    a = AI_C4(q->c18);
    b = AI_C4(q->c1c);
    c = AI_C4(q->c14);
    sum = (float)a->unk024 + a->unk01c;
    rank = a->unk10c - 2.0f;
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
    return util_RandomU(2);
}
/* 0x4941b0 */

int ai_Unk4941b0(void)
{
    return util_RandomU(2) != 0;
}
/* 0x4941d0 */

int ai_Unk4941d0(void)
{
    return util_RandomU(7);
}
/* 0x4941e0 */

int ai_Unk4941e0(AIPLAN *q)
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
    return util_RandomU(2);
}
/* 0x4942a0, a level from the score: score / 4, clamped to 2..6 */

int ai_Unk4942a0(AIPLAN *q)
{
    return (int)min(max(q->unk04 * 0.25f, 2.0f), 6.0f);
}
/* 0x494300, rolls the plan's tries: sets unk48 on success, unk4c to the negated sum */

int ai_Unk494300(AIPLAN *q)
{
    AIREC10 *t;
    float f, lim, scale, acc, v, sum;
    int i;

    q->unk48 = 0;
    t = ai_Unk4948c0(q->unk38);
    f = q->unk0c;
    lim = f * 0.33f;
    i = 0;
    acc = 0.0f;
    for (; i < q->unk3c; i++) {
        scale = 0.5f;
        if (q->unk40 == 0)
            scale = 1.0f;
        v = ai_Unk4e5360() * t->unk0c * scale + t->unk08;
        sum = acc + v;
        if (sum >= f) {
            acc = v + acc;
            if (ai_Unk4e52f0(4))
                acc = acc * 0.75;
            q->unk48 = 1;
            break;
        }
        if (v > lim) {
            acc = acc + lim;
            q->unk48 = 1;
            break;
        }
        lim = lim * 0.8f;
        acc = sum;
    }
    q->unk4c = -acc;
    return q->unk48;
}
/* 0x494410, decides whether a plan between two characters loses against a third */

int ai_Unk494410(AIC4 *ch, AIPLAN *q)
{
    AIC4 *a, *b;
    float x, y, c, d;

    if (q->unk48 != 1 && q->unk08 != 1) {
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
        d = d + q->unk04 * q->unk04 * 0.0025f;
        if (q->unk44) {
            x = x * 1.1f;
            y = y * 0.9f;
        }
        if (c + x > d + y)
            return 1;
    }
    return 0;
}
/* 0x4945c0, the rank (0..7) of the score: 4..5 -> 1, 6..7 -> 2 .. 14..16 -> 6, more -> 7; one less
   for a character with flag 0x22e */

int ai_Unk4945c0(AIPLAN *q)
{
    int n, r;
    float fl;
    AIC4 *c;

    n = (int)(q->unk04 - 1.0f);
    fl = (float)(q->unk04 - floor(q->unk04));
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
/* 0x4946c0, the rank of 0x4945c0 raised by how close the character is (0..9) */

int ai_Unk4946c0(AIC4 *ch, AIPLAN *q)
{
    int n;
    float k, d;
    AIPOS pos;
    AIC4 *c;

    n = ai_Unk4945c0(q);
    gv_GetTableRecord(0, &pos);
    k = (5.0f - (float)pos.unk18) * 0.1f + 0.7f;
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
    } else if (n == 7 && c->unk00d < 2)
        n = 6;
    n = n <= 0 ? 0 : n;
    if (n >= 9)
        n = 9;
    return n;
}
/* 0x4947a0, a rating from the wealth of two characters: x / (log10 x + 1) * 0.1 * 32, x at least 10000 */

int ai_Unk4947a0(AIC4 *a, AIC4 *b)
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

int ai_Unk494830(AIC4 *a, AIC4 *b)
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
/* 0x4948c0, the record of the table at 0x5ecfd8 with this key */

AIREC10 *ai_Unk4948c0(int key)
{
    int i;

    for (i = 0; i < 7; i++)
        if (ai_Unk5ecfd8[i].key == key)
            return &ai_Unk5ecfd8[i];
    return 0;
}
/* 0x4948f0, rolls the three (value, option, option) triples of a character */

void ai_Unk4948f0(AIC4 *ch, int *out)
{
    int t[3][4] = {{4, 0, 3, 1}, {3, 1, 2, 0}, {2, 0, 4, 1}};
    float f[3];
    float m;
    int i, k, v;

    v = gm_GetWorth(ch->id);
    k = (int)(v * 0.025f);
    m = 0.0f;
    for (i = 0; i < 3; i++) {
        f[i] = gm_MapDistance(ai_Unk7e3f80, ai_Unk7e3f84, ai_Unk7e4274[i].a, ai_Unk7e4274[i].b);
        if (f[i] > m)
            m = f[i];
        out[i * 3 + 1] = t[i][0];
        out[i * 3 + 2] = t[i][util_RandomU(3) + 1];
    }
    for (i = 0; i < 3; i++) {
        f[i] = 1.0f / m * f[i];
        out[i * 3] = (int)(k * f[i]);
    }
}
/* 0x494a30, whether a character is not worth a second look at another (rank and distance against a roll) */

int ai_Unk494a30(AIC4 *a, AIC4 *b)
{
    float t, s;
    int x, y;

    if (a->netId == (unsigned int)b->partner)
        return 0;
    x = gm_CharLevelS(a->id, 0);
    y = gm_CharLevelS(b->id, 0);
    t = (float)(y - x) * 2.5f;
    s = gm_Liking(a->id, b->id, 1) + t;
    if (s < util_RandomF() * 75.0f)
        return 0;
    return 1;
}
/* 0x494ac0, rates a group of a leader and up to four members: the relation d (-2..2) of each member
   to the leader sends a message with a random strength (when send is set), the group's own value goes
   to unk2c. Returns 0, or -1 .. -6 for a bad group */

int ai_Unk494ac0(AIGRP *g, int send)
{
    int i, lv, base, d, v, w, p, r;

    if (!g)
        return -1;
    if (!g->leader)
        return -2;
    if (g->unk24 > 5)
        return -3;
    if (g->unk28 > 5)
        return -4;
    if (g->n == 0 || g->n > 4)
        return -5;
    r = 5 - g->unk28;
    p = 5 - g->unk24;
    lv = gm_CharLevelS(g->leader->id, 0);
    base = lv - lv / 3 - lv / 4 + p + r;
    for (i = 0; i < g->n; i++) {
        if (!g->mem[i].ch)
            return -6;
        d = gm_CharLevelS(g->mem[i].ch->id, 0);
        g->mem[i].d = max(min(base - d, 2), -2);
    }
    for (i = 0; i < g->n; i++) {
        switch (g->mem[i].d) {
        case -2:
            v = -0x10 - util_RandomU(0x10);
            break;
        case -1:
            v = -8 - util_RandomU(8);
            break;
        case 0:
            v = util_RandomU(8) + 8;
            break;
        case 1:
            v = util_RandomU(0x10) + 0x10;
            break;
        case 2:
            v = util_RandomU(0x10) + 0x18;
        }
        if (send)
            cm_SendRelations(g->leader->netId, g->mem[i].ch->netId, v, 1);
    }
    w = gm_GetWorth(g->leader->id);
    g->unk2c = (int)((float)(10 - g->unk28 - g->unk24) * w * g->n * 0.001f);
    if (send)
        cm_SendMoveGoods(-1, g->leader->netId, ai_OwnPlayer, g->unk2c);
    return 0;
}
/* 0x494d40, a rating from the rank difference of two characters and a count */

int ai_Unk494d40(AIC4 *a, AIC4 *b, int n)
{
    int x, y;

    x = gm_CharLevelS(a->id, 0);
    y = gm_CharLevelS(b->id, 0);
    return (int)((float)((x - y + 10) * n * 3) * 0.33333334f);
}
/* 0x494d90, whether a character takes a risk: closeness plus a roll plus a bonus from two flags
   (0xd: -0.25, 0xa: +0.1), the flag that applied goes to *out */

int ai_Unk494d90(AIC4 *a, AIC4 *b, int *out)
{
    float sum, f;
    int k;
    int q[0x88];

    sum = gm_Mix(a, b, 3) + util_RandomF();
    f = 0.0f;
    k = 0x3b;
    memset(q, 0, sizeof(q));
    q[0] = 1;
    if (ord_DispatchOrder(0xd, b, q) & 2) {
        f = -0.25f;
        k = 0xd;
    } else if (ord_DispatchOrder(10, b, q) & 2) {
        f = 0.1f;
        k = 10;
    }
    if (out)
        *out = k;
    if (sum + f > 1.0f)
        return 1;
    return 0;
}
/* 0x494e70, loads the action table from /gamedata/ai/ai_data_2.dfn (or, with ai_Unk6f2464 set, registers
   the actions from ai_method_addon.ini and writes the table) */

int ai_LoadActionTable(void)
{
    unsigned int n;
    int hdr[2];
    G5ACT r;
    G5ACT *act;
    void *f;
    unsigned int i;

    memset(ai_Unkf83ec0, 0, 0x49 * sizeof(G5ACT));
    n = 0x49;
    hdr[0] = 0x6144466e;
    if (ai_Unk6f2464 == 0) {
        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "rb", vfs_Root);
        if (f == 0)
            return 0;
        if (ai_Unk4a16f0(ai_Unk6f2464) != 0 && ai_Unk490c10(ai_Unk6f2464) != 0 &&
            ai_Unk48fb70(ai_Unk6f2464) != 0 && ai_Unk4abed0(ai_Unk6f2464) != 0 &&
            vfs_fread(hdr, 4, 1, f) != 0) {
            if (hdr[0] != 0x6144466e)
                return 0;
            if (vfs_fread(&n, 4, 1, f) != 0) {
                for (i = 0; i < n; i++) {
                    if (vfs_fread(&r.id, 1, 1, f) == 0 || vfs_fread(r.name, 0x20, 1, f) == 0 ||
                        vfs_fread(&r.w1[0].k, 1, 1, f) == 0 || vfs_fread(&r.w1[0].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[1].k, 1, 1, f) == 0 || vfs_fread(&r.w1[1].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[2].k, 1, 1, f) == 0 || vfs_fread(&r.w1[2].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[3].k, 1, 1, f) == 0 || vfs_fread(&r.w1[3].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[0].k, 1, 1, f) == 0 || vfs_fread(&r.w2[0].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[1].k, 1, 1, f) == 0 || vfs_fread(&r.w2[1].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[2].k, 1, 1, f) == 0 || vfs_fread(&r.w2[2].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[3].k, 1, 1, f) == 0 || vfs_fread(&r.w2[3].w, 4, 1, f) == 0)
                        goto fail;
                    memcpy(r.w3, r.w1, 0x20);
                    if (r.id < 0x49) {
                        act = &G5_ACTS[r.id];
                        act->id = r.id;
                        strcpy(act->name, r.name);
                        memcpy(act->w1, r.w1, 0x20);
                        memcpy(act->w2, r.w2, 0x20);
                        memcpy(act->w3, r.w3, 0x20);
                    }
                }
            }
            vfs_fclose(f);
            return 1;
        }
    } else {
        hdr[1] = 0x49;
        GetModuleFileNameA(main_Instance, ai_Unkf83da0, 0x104);
        strrchr(ai_Unkf83da0, '\\')[1] = 0;
        strcat(ai_Unkf83da0, "ai_method_addon.ini");
        if (ai_Unk4a16f0(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk490c10(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk48fb70(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk4abed0(ai_Unk6f2464) == 0)
            return 0;
        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "wb", vfs_Root);
        if (f == 0)
            return 0;
        if (vfs_fwrite(&hdr[0], 4, 1, f) != 0 && vfs_fwrite(&hdr[1], 4, 1, f) != 0) {
            for (act = G5_ACTS; vfs_fwrite(&act->id, 1, 1, f) != 0 && vfs_fwrite(act->name, 0x20, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[0].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[0].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[1].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[1].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[2].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[2].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[3].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[3].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[0].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[0].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[1].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[1].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[2].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[2].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[3].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[3].w, 4, 1, f) != 0; ) {
                act++;
                if (act >= G5_ACTS + 0x49) {
                    vfs_fclose(f);
                    return 1;
                }
            }
        }
    }
fail:
    vfs_fclose(f);
    return 0;
}
/* 0x495520, registers an action: its need weights come from the ini file when ini is set */

int ai_Unk495520(AIACT *a, int ini)
{
    G5ACT *act = (G5ACT *)a;
    char key[64];
    char val[128];
    int i, found;

    if (act->id != 0 && act->id < 0x49) {
        if (ini == 0) {
            for (i = 0; i < 4; i++) {
                act->w1[i].k = 0xff;
                act->w1[i].w = 0.0f;
                act->w2[i].k = 0xff;
                act->w2[i].w = 0.0f;
                act->w3[i].k = 0xff;
                act->w3[i].w = 0.0f;
            }
        } else {
            for (i = 1; i <= 4; i++) {
                sprintf(key, "ShortDesire%li", i);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0) == 0) {
                    act->w1[i - 1].k = 0xff;
                    act->w1[i - 1].w = 0.0f;
                } else {
                    act->w1[i - 1].k = (unsigned char)ai_Unk4a47c0(val);
                    sprintf(key, "ShortChange%li", i);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0);
                    act->w1[i - 1].w = (float)atof(val);
                }
                sprintf(key, "LongDesire%li", i);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0) == 0) {
                    act->w2[i - 1].k = 0xff;
                    act->w2[i - 1].w = 0.0f;
                } else {
                    act->w2[i - 1].k = (unsigned char)ai_Unk4a47c0(val);
                    sprintf(key, "LongChange%li", i);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0);
                    act->w2[i - 1].w = (float)atof(val);
                }
            }
            found = 0;
            for (i = 0; !found; i++) {
                if (i > 3) {
                    sprintf(val, "am_RegisterMethod(): Methode '%s' hat keinerlei Auswirkungen!!!", act->name);
                    break;
                }
                if ((act->w1[i].k != 0xff && act->w1[i].w != 0.0f) ||
                    (act->w2[i].k != 0xff && act->w2[i].w != 0.0f))
                    found = 1;
            }
            memcpy(act->w3, act->w1, 0x20);
        }
        G5_ACTS[act->id] = *act;
    }
    return 1;
}
/* 0x495770, pushes an action onto the evaluation stack */

void ai_Unk495770(int k)
{
    ai_Unkf86900[ai_Unkf83ea4] = k;
    ai_Unkf83ea4++;
}
/* 0x495790, pops the evaluation stack */

void ai_Unk495790(void)
{
    ai_Unkf83ea4--;
}
/* 0x4957a0, whether an action is on the evaluation stack */

int ai_Unk4957a0(int k)
{
    int i;

    for (i = 0; i < ai_Unkf83ea4; i++) {
        if (ai_Unkf86900[i] == k)
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
    p = &G5_CHARS[i];
    k = 0;
    for (act = G5_ACTS; act < G5_ACTS + 0x49; act++, k++) {
        if (act->id != 0 && act->id != p->act && (act->flags & p->mask) != 0) {
            s1.kind = 0;
            s1.ref = 0;
            s1.arg = 0;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            if (act->eval != 0 && act->eval(&a, &b, k, p, 0, &s1, &s2) != 0 && (best1 <= a || best2 <= b) &&
                act->decide != 0 && (r = act->decide(0, p, 0, &s1, &s2)) != 0) {
                if (best1 <= a) {
                    b1s1 = s1;
                    best1 = a;
                    id1 = act->id;
                    b1s2 = s2;
                    r1 = r;
                }
                if (best2 <= b) {
                    best2 = b;
                    id2 = act->id;
                    b2s1 = s1;
                    b2s2 = s2;
                    r2 = r;
                }
            }
        }
    }
    if (best1 == 0.0f || best1 < best2) {
        p->prev = (unsigned char)id2;
        p->arg1 = b2s1;
        p->act = (unsigned char)r2;
        p->arg2 = b2s2;
    } else {
        p->prev = (unsigned char)id1;
        p->arg1 = b1s1;
        p->act = (unsigned char)r1;
        p->arg2 = b1s2;
    }
    return p->act;
}
/* 0x495ce0, executes the current action of character i and applies its need weights */

int ai_ExecuteAction(int i)
{
    G5CH *p = &G5_CHARS[i];
    G5ACT *act;
    unsigned char msg;
    int k;

    if (p->act != 0 && G5_ACTS[p->act].exec != 0 && G5_ACTS[p->act].exec(p, &p->arg1, &p->arg2) != 0) {
        ai_Unk4c3e70(p);
        for (k = 0; k < 4; k++) {
            act = &G5_ACTS[p->prev];
            if (act->w1[k].k < 14)
                ai_Unk4c3eb0(act->w1[k].k, act->w1[k].w * -0.5f);
        }
        for (k = 0; k < 4; k++) {
            act = &G5_ACTS[p->act];
            if (act->w1[k].k < 14)
                ai_Unk4c3eb0(act->w3[k].k, act->w3[k].w * -0.5f);
        }
        if (p->prev == p->act) {
            ai_Unk4c3eb0(13, 500.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
                msg = 2;
                cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        } else {
            ai_Unk4c3eb0(13, 250.0f);
            if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
                msg = 1;
                cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
                cm_AddFieldValues(1, 1, 0x215, &msg);
                cm_SendFieldDeltas();
            }
        }
        ai_Unk4c4a20();
        memcpy(G5_ACTS[p->act].w3, G5_ACTS[p->act].w1, 0x20);
        return 1;
    }
    if (ai_Partner != 0 && ai_Partner->netId == (unsigned int)G5_CHARS[i].partner) {
        msg = 3;
        cm_BeginFieldBlock((AIPER *)p, G5_CHARS[i].netId);
        cm_AddFieldValues(1, 1, 0x215, &msg);
        cm_SendFieldDeltas();
    }
    return 0;
}

/* ---- 0x495ee0..0x497490 ---- */

/* 0x495ee0, sends each kind-3 character random flags (field 0x1e8) */
void ai_Unk495ee0(void)
{
    AIPER *c;
    unsigned int f;
    int i;

    f = 1;
    for (i = 0; i < 768; i++) {
        c = (AIPER *)&ai_CharsT[i];
        if (c->kind == 3) {
            f |= util_RandomU(2) ? 2 : 0;
            f |= util_RandomU(3) ? 4 : 0;
            f |= util_RandomU(3) ? 8 : 0;
            f |= util_RandomU(2) ? 0x10 : 0;
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldDelta(4, 1, 0x1e8, &f);
            cm_SendFieldDeltas();
        }
    }
}
/* 0x495fa0 */

int ai_Unk495fa0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 6;
}
/* 0x495fc0 */

int ai_Unk495fc0(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 6;
}
/* 0x495fd0 */

int ai_Unk495fd0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 7;
}
/* 0x495ff0 */

int ai_Unk495ff0(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 7;
}
/* 0x496000 */

int ai_Unk496000(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 8;
}
/* 0x496020 */

int ai_Unk496020(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 8;
}
/* 0x496030 */

int ai_Unk496030(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b == 0;
}
/* 0x496050 */

int ai_Unk496050(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 2;
}
/* 0x496070 */

int ai_Unk496070(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 2;
}
/* 0x496080 */

int ai_Unk496080(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    if (a)
        return 0;
    return b ? 0 : 3;
}
/* 0x4960a0 */

int ai_Unk4960a0(AIPER *p, AIARG *s1, AIARG *s2)
{
    return 3;
}
/* 0x4960b0, decide: one of five needs (action 4) */

int ai_Unk4960b0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int count, k, v, id, r;
    unsigned char c;

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
        s1.unk14 = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    if (o1->kind == 0xe) {
        k = o1->ref;
        memset(q, 0, sizeof(q));
        q[0] = 1;
        f = (ord_DispatchOrder(ids[k], p, q) & 2) ? -1.0f : 0.0f;
        c = p->unk080[k];
        if (c < 42)
            v = 1;
        else if (c < 84)
            v = 2;
        else if (c < 126)
            v = 4;
        else if (c < 168)
            v = 6;
        else if (c < 210)
            v = 9;
        else
            v = 12;
        v = (int)(v + f);
        if (v < p->money)
            return 0;
        if (c == 252.0f)
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
        c = p->unk080[i];
        if (c < 42)
            n[i] = 1;
        else if (c < 84)
            n[i] = 2;
        else if (c < 126)
            n[i] = 4;
        else if (c < 168)
            n[i] = 6;
        else if (c < 210)
            n[i] = 9;
        else
            n[i] = 12;
        n[i] = (int)(n[i] + f);
        if (n[i] > p->money) {
            valid[i] = 0;
            count--;
        } else {
            fb = c;
            if (fb == 252.0f) {
                valid[i] = 0;
                count--;
            } else
                w[i] = fb / cnt[i];
        }
    }
    if (!count)
        return 0;
    k = util_RandomU(5);
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
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
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

int ai_Unk4965b0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->arg2);
    m.flags = 1;
    m.type = 0x60;
    m.to = p->netId;
    m.fromId = -1;
    m.unk5a = (unsigned char)s1->ref;
    cm_SendLongMessage((AIMSG *)&m);
    return 4;
}
/* 0x496610, decide: the strongest of five needs (action 5) */

int ai_Unk496610(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIX6 r6;
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int best, second, ka, kb, v, id, r, sum;
    unsigned char c;

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
        r = ai_Unk495a50(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    gm_Unk503ee0(&r6, p->jobId);
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.ref = 0;
        s1.arg = 0;
        s1.unk14 = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    ka = util_RandomU(5);
    kb = util_RandomU(5);
    for (i = 0; i < 5; i++) {
        memset(q, 0, sizeof(q));
        v = r6.v[i];
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
        c = p->unk080[i];
        if (c < 42)
            n[i] = 1;
        else if (c < 84)
            n[i] = 2;
        else if (c < 126)
            n[i] = 4;
        else if (c < 168)
            n[i] = 6;
        else if (c < 210)
            n[i] = 9;
        else
            n[i] = 12;
        n[i] = (int)(n[i] + f);
        if (n[i] > p->money)
            valid[i] = 0;
        else {
            fb = c;
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
        r = ai_Unk495a50(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (sum == 2) {
        if (!util_RandomU(2))
            ka = kb;
    } else if (!valid[ka])
        ka = kb;
    if (p->kind == 3) {
        if (ka == 4) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (ka == 3) {
            if (!(p->flags1e8 & 8))
                return 0;
        } else if (ka == 2 && !(p->flags1e8 & 2))
            return 0;
    } else if (ka > 1) {
        if (ka == 4)
            id = 0x15;
        else
            id = ka == 3 ? 0x17 : 0x14;
        if (!gm_Query(p->bld2->objList, 1, 0, id)) {
            AIARG s1, s2;

            s1.kind = 2;
            s1.ref = id;
            s1.arg = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_Unk495a50(5, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    }
    o1->kind = 0xe;
    o1->ref = ka;
    o1->arg2 = n[ka];
    return 5;
}
/* 0x496ab0, execute of 0x496610 */

int ai_Unk496ab0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->arg2);
    m.to = p->netId;
    m.fromId = p->netId;
    m.flags = 1;
    m.type = 0x60;
    m.unk5a = (unsigned char)s1->ref;
    cm_SendLongMessage((AIMSG *)&m);
    return 5;
}
/* 0x496b10, evaluate: the needs an action serves, or a companion near the job's partners */

int ai_Unk496b10(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
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
    act = &ai_Unkf83ec0[i];
    *a = 0.0f;
    *b = 0.0f;
    if (!p->unk168) {
        if (p->officeKind == 0xd)
            goto fallback;
        x = ai_JudgeCatholic ? ai_JudgeCatholic->netId : -1;
        y = ai_JudgeProtestant ? ai_JudgeProtestant->netId : -1;
        for (job = AI_53E400_3(1, 0, 0x2c); job; job = (AIJOBX *)he_NextEvent())
            if (job->unkb0 == p->netId)
                break;
        if (!job || job->unkb4 == x || job->unkb4 == y)
            goto fallback;
        dist[0] = 100.0f;
        dist[1] = 100.0f;
        dist[2] = 100.0f;
        best = 100.0f;
        bi = -1;
        c[0] = gm_FindPer(job->unkb4);
        c[1] = gm_FindPer(job->unkb8);
        c[2] = gm_FindPer(job->unkbc);
        if (c[0] && c[0] != AI_6F0468)
            dist[0] = gm_Liking(p->id, c[0]->id, 1);
        if (c[1] && c[1] != AI_6F0468)
            dist[1] = gm_Liking(p->id, c[1]->id, 1);
        if (c[2] && c[2] != AI_6F0468)
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
    }
    for (j = 0; j < 4; j++) {
        if (act->w1[j].w > 0.0f) {
            if (act->w1[j].k == p->sel)
                *a += p->mood[act->w1[j].k][2] * act->w1[j].w * 0.02f;
            else
                *a += p->mood[act->w1[j].k][2] * act->w1[j].w * 0.01f;
        }
        if (act->w2[j].w > 0.0f) {
            if (act->w2[j].k == p->sel)
                *b += p->mood[act->w2[j].k][2] * act->w2[j].w * 0.02f;
            else
                *b += p->mood[act->w2[j].k][2] * act->w2[j].w * 0.01f;
        }
    }
    return 1;
fallback:
    gv_GetTableRecord(16, &d);
    if (d.cur)
        return ai_Unk4a4c10(a, b, i, p, &d);
    return ai_Unk4a49a0(a, b, i, p, 0, o1, o2);
}
/* 0x496e40, decide: talk a companion into money (action 9) */

int ai_Unk496e40(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
        s1.unk14 = 0.66f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_Unk495a50(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if ((p->flags1e8 & 0x400) && util_RandomU(8))
        return 0;
    if (p->kind == 3) {
        if (!(p->flags1e8 & 2))
            return 0;
    } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
        AIARG s1, s2;

        s1.kind = 2;
        s1.ref = 0x14;
        s1.arg = 1;
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.ref = p->bld2->id;
        s2.arg = 1;
        r = ai_Unk495a50(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    m = gm_CharCurrency(p, ai_OwnPlayer);
    lim = (int)(m * 0.33f);
    if (o1->kind == 0) {
        if (p->unk168) {
            bi = -1;
            best = 100.0f;
            n = amt_GetOfficeApplicants(p->unk168, 6, list);
            if (n <= 0)
                return 0;
            for (j = 0; j < n; j++) {
                if (list[j] != AI_6F0468) {
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
            if (!ai_PickPartnerChain(p, 1, &flags, 0, util_RandomU(25) + 10.0, &cid))
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
        if (!c || c == AI_6F0468)
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

int ai_Unk4972d0(AIPER *p, AIARG *s1, AIARG *s2)
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
/* 0x4973f0, evaluate: the default rating, ten times for a character with 0x167 set and 0x166 clear */

int ai_Unk4973f0(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    float k;

    if (f) {
        *a = -1.0e30f;
        *b = -1.0e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    if (p->unk167 && !p->officeKind)
        k = 10.0f;
    else
        k = 1.0f;
    ai_Unk4a49a0(a, b, i, p, 0, o1, o2);
    *a *= k;
    *b *= k;
    return 1;
}
/* 0x497490, decide callback */

int ai_Unk497490(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int list[6];
    AIARG s2;
    AIARG s1;
    int r, k;

    if (f == 0) {
        if (p->unk00d < 2) {
            s1.kind = 13;
            s1.ref = 2;
            s1.arg = 1;
            s1.unk14 = 0.44f;
            memset(&s2, 0, sizeof(s2));
            r = ai_Unk495a50(10, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        } else if (ai_TimeHour > 12 && p->unk168 == 0 && unk_Unk4fc7a0(p) - p->unk00a > 2) {
            k = amt_GetOfficeCandidates(p, 2, list);
            if (k == 0) {
                if (util_RandomU(4) == 0) {
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
                r = list[util_RandomU(k) * 3];
                if (r != 0 && r < 0x1c) {
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

int ai_Unk497630(G5CH *p, AIARG *s1)
{
    if (s1->kind != 10)
        return 0;
    return amt_ApplyForOffice(p, s1->ref) ? 10 : 0;
}
/* 0x497660, decide: whether a good (o1, kind 2) can be made from what o2 holds (action 11) */

int ai_Unk497660(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIOBJ *obj;
    AIBLD *bld;
    AIPER *c;
    AIBTX *t;
    int i, n, v;
    unsigned short k;

    if (b == 1 && o1->kind == 2 && !AI_GOODX[o1->ref].unk40 && AI_GOODX[o1->ref].type != 2) {
        c = 0;
        bld = 0;
        obj = 0;
        if (o2->kind == 1) {
            obj = (AIOBJ *)gm_FindObjectInst(o2->ref);
            if (!obj)
                return 0;
            if (AI_GOODX[obj->id].type == 2 || AI_GOODX[obj->id].type == 6) {
                if (!gm_Query(obj->unk14, 1, 0, o1->ref) && (bld = gm_FindBuildingById(obj->unk06)) != 0) {
                    t = &AI_BTX[bld->type];
                    n = t->n;
                    for (i = 0; i < n; i++)
                        if ((t->k[i] & 0x7fff) == obj->id)
                            break;
                    if (i < n) {
                        for (i++; i < n; i++)
                            if ((t->k[i] & 0x7fff) == o1->ref)
                                break;
                        if (i < n && (!t->p[i].a || gm_Query(obj->unk14, 1, 0, t->p[i].a))) {
                            k = t->p[i].b;
                            if (!k || gm_Query(obj->unk14, 1, 0, k))
                                goto rate;
                        }
                    }
                }
                return 0;
            }
        } else if (o2->kind == 4) {
            bld = gm_FindBuildingById(o2->ref);
            if (bld && !gm_Query(bld->objList, 1, 0, o1->ref)) {
                t = &AI_BTX[bld->type];
                n = t->n;
                for (i = 0; i < n; i++)
                    if ((t->k[i] & 0x7fff) == o1->ref)
                        break;
                if (i < n && (!t->p[i].a || gm_Query(bld->objList, 1, 0, t->p[i].a))) {
                    k = t->p[i].b;
                    if (!k || gm_Query(bld->objList, 1, 0, k))
                        goto rate;
                }
            }
            return 0;
        } else if (o2->kind == 7) {
            c = gm_FindPer(o2->ref);
            if (!c)
                return 0;
        } else
            return 0;
rate:
        if (a != 0x39) {
            if (obj)
                c = gm_ObjectOwnerChar(obj);
            else if (bld) {
                if (bld->owner == 0xffff)
                    return 0;
                c = (AIPER *)&ai_CharsT[bld->owner];
            } else if (!c)
                return 0;
            if (!c)
                return 0;
            v = gm_CharCurrency(c, ai_OwnPlayer);
            o1->arg2 = 0;
        } else {
            v = o2->arg2;
            o1->arg2 = 0x39;
        }
        if (o1->unk14 > 0.0f && o1->unk14 < 1.0f)
            v = (int)(v * o1->unk14);
        return v < (int)gm_GoodsPrice(o1->ref, ai_OwnPlayer) ? 0 : 11;
    }
    return 0;
}
/* 0x4979d0, execute of 0x497660: hands the good over (buys it for action 0x39) */

int ai_Unk4979d0(AIPER *p, AIARG *s1, AIARG *s2)
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
        return 0;
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
        cm_SendExSellObjectI(id, -1, s1->ref, s1->arg, ai_OwnPlayer, 0);
        return 0x39;
    }
    cm_SendExSellObjectI(id, -1, s1->ref, s1->arg, ai_OwnPlayer, (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 11;
}
/* 0x497af0, decide: sell a good (o1, kind 2) to a building (o2, kind 4) with room for it (action 14) */

int ai_Unk497af0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
    if (o1->unk14 > 0.0f && o1->unk14 < 1.0f)
        v = (int)(v * o1->unk14);
    if (v < (int)gm_GoodsPrice(o1->ref, ai_OwnPlayer))
        return 0;
    t = &AI_BTX[bld->type];
    n = 0;
    for (o = gm_Query(bld->objList, 1, 4, 2); o; o = gm_QueryNext())
        if (o->id != 0xfd)
            n++;
    if (n >= t->unk21)
        return 0;
    for (j = he_FindEvent(2, 0, 0x1f, 3, bld->id); j; j = he_NextEvent())
        n++;
    return n >= t->unk21 ? 0 : 14;
}
/* 0x497c80, execute of 0x497af0 */

int ai_Unk497c80(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bld;

    bld = gm_FindBuildingById(s2->ref);
    if (!bld)
        return 0;
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObjectI(bld->id, -1, s1->ref, s1->arg, ai_OwnPlayer, (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 14;
}
/* 0x497cf0, decide: renovate the character's worst building (action 15) */

int ai_Unk497cf0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIBLDX *bl, *best;
    float lim, f;
    int i, d, v;

    if (o1->kind == 0) {
        d = util_RandomU(0x24) + 0x23;
        best = 0;
        bl = AI_BLDX;
        for (i = 0; i < 0x100; i++, bl++) {
            if (bl->type && bl->masterId == p->id && !(bl->flags5a & 0x45)
                && ((1 << ai_BTypesT[bl->type].kind) & ((unsigned)&ai_Unk770d8c + 0x53628)) && bl->level < d) {
                d = bl->level;
                best = bl;
            }
        }
        if (!best)
            return 0;
        lim = gm_CharCurrency(p, ai_OwnPlayer) * 0.95f;
        v = d < 0x32 ? 50 : 30;
        f = ai_Unk4a4e30(best, v);
        if (f == 0.0f || f > lim)
            return 0;
        o1->kind = 4;
        o1->ref = best->id;
        o1->arg = 1;
        o1->arg2 = v;
        o1->unk14 = f;
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
                f = ai_Unk4a4e30(bl, o1->arg2);
                if (f == 0.0f || f > o2->arg)
                    return 0;
                o1->unk14 = f;
                return 15;
            }
        }
    }
    return 0;
}
/* 0x497ee0, execute of 0x497cf0: pays for the renovation */

int ai_Unk497ee0(AIPER *p, AIARG *s1, AIARG *s2)
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
    cm_SendMoveGoods(-1, s2->ref, ai_OwnPlayer, (int)s1->unk14);
    cm_SendLongMessage(&msg);
    cm_EndGroup("renovieren");
    fin = (AIFINX *)gm_PlayerRecord((AICHAR *)p);
    if (fin)
        fin->renovationSpent = (int)(fin->renovationSpent + s1->unk14);
    return 15;
}
/* 0x497fa0, decide: pay off a building's job when it is cheap enough (action 16) */

int ai_Unk497fa0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
            hi = 0;
            bt = &ai_BTypesT[bl->type];
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
        n = AI_BTX[p->bld2->type].unk247;
        for (i = 0; i <= n - 1; i++)
            ok[i] = 0;
    }
    if (o1->kind == 0) {
        for (bl = AI_501190_2(1, 6); o1->kind == 0 && bl; bl = AI_501030()) {
            if (ai_BTypesT[bl->type].kind == 2 && (bl->flags5a & 2)
                && ((bl->type == 4 && ok[0]) || (bl->type == 5 && ok[1]) || (bl->type == 6 && ok[2])
                    || (bl->type == 7 && ok[3]))) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->unkac == bl->id) {
                        v = ((AIJOBX *)j)->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v - lim;
                                return ai_Unk49eec0(0x10, p, b, o1, o2);
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
        for (bl = AI_501190_2(1, 6); o1->kind == 5 && bl; bl = AI_501030()) {
            if ((unsigned char)ai_BTypesT[bl->type].kind == o1->ref) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->unkac == bl->id) {
                        v = ((AIJOBX *)j)->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v - lim;
                                return ai_Unk49eec0(0x10, p, b, o1, o2);
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
/* 0x498470, execute callback: hands the job s1->unk04 over */

int ai_Unk498470(G5CH *p, AIARG *s1)
{
    AIJOB *j;

    if (s1->kind != 4)
        return 0;
    for (j = he_FindEvent(1, 0, 0x1b); j != 0; j = he_NextEvent()) {
        if (((int *)j)[0xac / 4] == s1->ref) {
            gm_Unk502fd0(s1->ref, p->netId, ((int *)j)[0xb0 / 4], (AIJOBX *)j, 1);
            return 0x10;
        }
    }
    return 0;
}
/* 0x4984d0, decide callback: buy or upgrade a house (called directly by 0x498c70) */

int ai_Unk4984d0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned int up[5];
    AIARG s1;
    AIARG s2;
    unsigned char *b;
    unsigned char *h;
    unsigned int lo32, hi32;
    int money, cost, j;
    unsigned char t;

    up[1] = 7;
    up[2] = 6;
    up[3] = 5;
    up[4] = 4;
    if (((unsigned char *)p)[2] == 3)
        return 0;
    if (f == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (f == 1 && o1->kind != 5 && o1->kind != 4) {
        return 0;
    }
    if (he_FindEvent(2, 0, 0xd, 2, (int)(short)p->id) != 0)
        return 0;
    b = (unsigned char *)ai_BldsT;
    for (up[0] = 0; ; ) {
        if (b[0] != 0) {
            t = ((unsigned char *)ai_BTypesT)[b[0] * 0x24d];
            lo32 = 0;
            hi32 = 0;
            if (t < 0x20)
                lo32 = 1 << t;
            else
                hi32 = 1 << (t - 0x20);
            if (*(short *)(b + 0x27) == (short)p->id && ((lo32 & 0xe07d43b0) != 0 || (hi32 & 1) != 0)) {
                money = (int)G6_509EE0(p, ai_OwnPlayer);
                s2.kind = 0;
                s2.ref = 0;
                s2.arg = 0;
                if (o1->kind == 0) {
                    h = *(unsigned char **)((char *)p + 0x170);
                    if (h != 0 && ((unsigned char *)ai_BTypesT)[h[0] * 0x24d] == 2 && (h[0x5a] & 0x41) == 0) {
                        t = ((unsigned char *)ai_BTypesT)[h[0] * 0x24d + 0x247];
                        if (t < 2 || t == 3) {
                            s1.kind = 5;
                            j = 0;
                            if (h[0] != 7) {
                                do
                                    j++;
                                while (up[j + 1] != h[0]);
                            }
                            s1.ref = up[j];
                            s1.arg = 1;
                            s2.kind = 4;
                            s2.ref = *(int *)(h + 1);
                            s2.arg = 1;
                            if (ai_Unk4a5140(a, p, f, &s1, &s2, 0) == 0)
                                return 0;
                            cost = gm_Unk509dc0(p, s1.ref);
                            if (cost <= money)
                                goto take;
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->arg = cost - money;
                            o1->kind = 0x12;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                        if (t == 5)
                            return 0;
                    }
                    s1.kind = 5;
                    s1.ref = 6;
                    s1.arg = 1;
                    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) != 0) {
                        cost = gm_Unk509dc0(p, s1.ref);
                        if (cost <= money)
                            goto take;
                        if ((float)(cost - money) / (float)cost < 0.25f) {
                            o1->arg = cost - money;
                            o1->kind = 0x12;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    }
                    s1.kind = 5;
                    s1.ref = 4;
                    s1.arg = 1;
                    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
                        return 0;
                    cost = gm_Unk509dc0(p, s1.ref);
                    if (money < cost) {
                        if ((float)(cost - money) / (float)cost >= 0.25f)
                            return 0;
                        o1->arg = cost - money;
                        o1->kind = 0x12;
                        return ai_Unk49eec0(0x11, p, f, o1, o2);
                    }
                } else {
                    if (o1->kind != 5)
                        return 0;
                    h = *(unsigned char **)((char *)p + 0x170);
                    s1 = *o1;
                    if (h == 0) {
                        if (((unsigned char *)ai_BTypesT)[o1->ref * 0x24d] != 2)
                            return 0;
                        if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
                            return 0;
                        if (((unsigned char *)p)[2] != 5)
                            return 0;
                        cost = gm_Unk509dc0(p, s1.ref);
                        if (money < cost) {
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->kind = 0x12;
                            o1->arg = cost - money;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    } else {
                        t = h[0];
                        if (((unsigned char *)ai_BTypesT)[t * 0x24d] != 2)
                            return 0;
                        s1.ref = o1->ref;
                        if (s1.ref - t != 1)
                            return 0;
                        if (t == 5)
                            return 0;
                        if (t == 7)
                            return 0;
                        s1.arg = 1;
                        s2.kind = 4;
                        s2.ref = *(int *)(h + 1);
                        s2.arg = 1;
                        cost = (int)gm_BuildingKindValue(s1.ref);
                        if (ai_Unk4a5140(a, p, f, &s1, &s2, 0) == 0)
                            return 0;
                        if (money < cost) {
                            if ((float)(cost - money) / (float)cost >= 0.25f)
                                return 0;
                            o1->kind = 0x12;
                            o1->arg = cost - money;
                            return ai_Unk49eec0(0x11, p, f, o1, o2);
                        }
                    }
                }
take:
                *o1 = s1;
                *o2 = s2;
                return 0x11;
            }
        }
        up[0]++;
        b += 0xa9;
        if ((int)up[0] > 0xff)
            return 0;
    }
}
/* 0x498a80, execute callback: upgrades a building (message "building_upgrade") */

int ai_Unk498a80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G6MSG m;

    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.unk08 = p->netId;
        m.time = ai_TimeT;
        m.type = 0x1e;
        m.unk0c = s2->ref;
        m.unk36 = 1;
        m.unk58 = s2->ref;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("building_upgrade");
        return 0x11;
    }
    return ai_ExecBuildBuilding((AIPER *)p, s1, s2) ? 0x11 : 0;
}
/* 0x498b80, decide callback: upgrade the character's building */

int ai_Unk498b80(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int item, obj;
    AIBLD *b;
    int r;

    if (((unsigned char *)p)[2] != 3 && (f != 0 || o1->kind == 0)) {
        b = *(AIBLD **)((char *)p + 0x170);
        if (b != 0 && o1->kind == 0 && ai_Unk4a5540(&item, &obj, b, 0) != 0) {
            s1.kind = 2;
            s1.ref = item;
            s1.arg = 1;
            if (obj != -1) {
                s2.kind = 1;
                s2.ref = obj;
            } else {
                s2.kind = 4;
                s2.ref = *(int *)((char *)b + 1);
            }
            s2.arg = 1;
            s2.unk14 = 0.44f;
            r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        }
    }
    return 0;
}
/* 0x498c70, decide: build up the character's workshops (action 0x13) */

int ai_Unk498c70(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
                tot += bt->unk247;
                cnt++;
                if (bt->unk247 == 1)
                    ones++;
            }
        }
    }
    if (tot && (!p->bld2 || (p->bld2->type != 7
            && (float)(AI_BTX[p->bld2->type].unk247 - AI_BTX[4].unk247 + 1) / tot < 0.33f))) {
        AI_ZERO(&t1, sizeof(t1));
        AI_ZERO(&t1, sizeof(t1));
        r = ai_Unk4984d0(0x13, p, 0, &t1, &t2);
        if (r) {
            *o1 = t1;
            *o2 = t2;
            return r;
        }
    }
    if (o1->kind != 0)
        return 0;
    for (i = 0; i < 0x100; i++) {
        bl = &AI_BLDX[i];
        if (bl->type && bl->owner == p->id && !(bl->flags5a & 0x41)) {
            bt = &AI_BTX[bl->type];
            if (bt->unk247 < bt->unk248) {
                k = gm_BuildingJob(bt->kind);
                r = gm_TitleJobI(p->id, k);
                if (r) {
                    n = gm_TitlesLeft(r);
                    n = n / 2 + n % 2;
                    if (n > bt->unk247 && k != 7) {
                        int v1, d1;

                        v1 = (int)(gm_BuildingKindValue(bl->type + 1) * 0.3f);
                        if (lim < v1) {
                            if ((float)(v1 - lim) / v1 < 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = v1 - lim;
                                return ai_Unk49eec0(0x13, p, b, o1, o2);
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
                    if (k == 7 && n > bt->unk247)
                        sel = bl->type + 1;
                }
            }
        }
    }
    if (p->flags1c8 & ((unsigned)&txt_Keys + 0x181c0))
        return 0;
    if (cnt > 3)
        return 0;
    if (ones > 0 && !sel) {
        if (ones > maxn)
            return 0;
        if (ai_TimeT.year % 8 != (unsigned int)p->netId % 8)
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
    if (ai_Unk4a5140(0, p, b, &s1, &s2, 1)) {
        int v2, d2;

        v2 = gm_Unk509dc0(p, s1.ref);
        if (lim < v2) {
            if ((float)(v2 - lim) / v2 < 0.25f) {
                o1->kind = 0x12;
                o1->arg = v2 - lim;
                return ai_Unk49eec0(0, p, b, o1, o2);
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
        for (bl = AI_501190_3(1, 5, k); s1.kind == 0 && bl; bl = AI_501030()) {
            if (AI_BTX[bl->type].unk247 <= h && (bl->flags5a & 2)) {
                for (j = AI_JOBS1B(); j; j = AI_JOBNEXT()) {
                    if (j->unkac == bl->id) {
                        v = j->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            lim = v - lim;
                            if ((float)lim / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->arg = lim;
                                return ai_Unk49eec0(0x13, p, b, o1, o2);
                            }
                            return 0;
                        }
                        s1.kind = 4;
                        s1.ref = bl->id;
                        s1.arg = 1;
                        s2.kind = 8;
                        s2.ref = j->unk04;
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

int ai_Unk499290(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIJOBX *j;

    if (s1->kind == 4 && s2->kind == 8) {
        j = (AIJOBX *)he_FindEvent(1, 1, s2->ref);
        if (!j)
            return 0;
        gm_Unk502fd0(s1->ref, p->netId, j->unkb0, j, 1);
        return 0x13;
    }
    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->ref;
        msg.flags = 1;
        msg.time = ai_TimeT;
        msg.arg[0] = s2->ref;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x13;
    }
    if (ai_ExecBuildBuilding(p, s1, s2)) {
        cm_SendSetField(p->netId, 0x1c8, 4, 0x1000000, 0);
        return 0x13;
    }
    return 0;
}
/* 0x499400, decide: find a good to sell (action 0x15) */

int ai_Unk499400(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
    if (!ai_Unk4a5540(&w, &z, y, 0))
        return 0;
    s1.kind = 2;
    s1.ref = w;
    s1.arg = 1;
    s1.unk14 = 0.44f;
    if (z != -1) {
        s2.kind = 1;
        s2.ref = z;
    } else {
        s2.kind = 4;
        s2.ref = y->id;
    }
    s2.arg = 1;
    s2.unk14 = 0.44f;
    r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
    if (!r)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x499520, decide: action 0x16 when 0x4f81c0 rates kind 2 so */

int ai_Unk499520(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];

    if (a)
        return 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}
/* 0x499580, execute of 0x499520 */

int ai_Unk499580(AIPER *p, AIARG *s1, AIARG *s2)
{
    int buf[0x88];

    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}
/* 0x4995d0, evaluate: decision record 0x18, the default rating without it */

int ai_Unk4995d0(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x18, &d);
    if (d.cur && (p->kind != 5 || !(p->flags1c8 & 2)))
        return ai_Unk4a4c10(a, b, i, p, &d);
    return ai_Unk4a49a0(a, b, i, p, f, o1, o2);
}
/* 0x499660, decide: sabotage a rival's building (action 0x17) */

int ai_Unk499660(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
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
            r = ai_Unk495a50(0x17, p->id, 1, &s1, &s2);
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
            s1.unk14 = 0.22f;
            s2.kind = 4;
            s2.ref = p->bld2->id;
            s2.arg = 1;
            r = ai_Unk495a50(0x17, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        gv_GetTableRecord(0x18, &d);
        if (d.cur && !util_RandomU(4) && !ai_Partner)
            return 0;
        if (o1->kind == 0) {
            mask.LowPart = 0x344015;
            mask.HighPart = 1;
            x = -1;
            if ((ai_Partner && (ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &x), x != -1))
                || ai_PickActionBuilding(p, 6, mask.QuadPart, 0, util_RandomU(0x20) + 19.0f, &x)) {
                gm_FindById((int)&y, 0, 0, x);
                if (y && y->owner != 0xffff && he_Unk570ea0((AIPER *)&ai_CharsT[y->owner], p)) {
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
                            s1.unk14 = 0.0f;
                            s2.kind = 0;
                            s2.ref = 0;
                            s2.arg = 0;
                            r = ai_Unk495a50(0x17, p->id, 0, &s1, &s2);
                            if (r) {
                                *o1 = s1;
                                *o2 = s2;
                            }
                            return r;
                        }
                        if (he_Unk553d60((AIPER *)&ai_CharsT[y->owner], p)
                            && he_NoEvent2ePair((AIPER *)&ai_CharsT[y->owner], p) && he_Unk553cc0((AIBLD *)y)
                            && he_Unk570ea0((AIPER *)&ai_CharsT[y->owner], p)) {
                            o1->kind = 4;
                            o1->ref = y->id;
                            o1->arg = 1;
                            o1->arg2 = v;
                            o1->unk14 = (float)(d.cur * 2 + 1);
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

int ai_Unk499a90(AIPER *p, AIARG *s1, AIARG *s2)
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
    ai_Unkf84c60[0] = ai_Unkf84c40[0] * s1->unk14;
    ai_Unkf84c60[2] = ai_Unkf84c40[2] * s1->unk14;
    ai_Unkf84c60[4] = ai_Unkf84c40[4] * s1->unk14;
    ai_Unkf84c60[6] = ai_Unkf84c40[6] * s1->unk14;
    return 0x17;
}
/* 0x499b80, evaluate: decision record 0x15, the default rating without it */

int ai_Unk499b80(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x15, &d);
    if (d.cur && (p->kind != 5 || !(p->flags1c8 & 2)))
        return ai_Unk4a4c10(a, b, i, p, &d);
    return ai_Unk4a49a0(a, b, i, p, f, o1, o2);
}
/* 0x499c10, decide: beat up a character (action 0x18) */

int ai_Unk499c10(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    AIDEC d;
    AIARG s1, s2;
    AIPER *c;
    unsigned short cid;
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
                r = ai_Unk495a50(0x18, p->id, 1, &s1, &s2);
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
                s1.unk14 = 0.22f;
                s2.kind = 4;
                s2.ref = p->bld2->id;
                s2.arg = 1;
                r = ai_Unk495a50(0x18, p->id, 1, &s1, &s2);
                if (r) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            gv_GetTableRecord(0x15, &d);
            if (!d.cur || util_RandomU(3) || ai_Partner) {
                if (o1->kind == 0) {
                    flags = ((unsigned)&cm_SendRing + 0x4973c0);
                    t = util_RandomU(0x20) + 32.0f;
                    if (ai_PickPartnerChain(p, 1, &flags, 0, t, &cid)) {
                        c = (AIPER *)&ai_CharsT[cid];
                        if (c->money >= 2) {
                            g = gm_Mix(p, c, 3);
                            if (!(util_RandomF() + g >= 1.0f)) {
                                s1.kind = 0xe;
                                s1.ref = 3;
                                s1.arg = 1;
                                s1.unk14 = 0.0f;
                                s2.kind = 0;
                                s2.ref = 0;
                                s2.arg = 0;
                                r = ai_Unk495a50(0x18, p->id, 0, &s1, &s2);
                                if (r) {
                                    *o1 = s1;
                                    *o2 = s2;
                                }
                                return r;
                            }
                            if (he_Unk553d60(c, p) && he_NoEvent2ePair(c, p)) {
                                o1->kind = 7;
                                o1->ref = c->netId;
                                o1->arg = 1;
                                o1->arg2 = v;
                                o1->unk14 = (float)(d.cur * 2 + 1);
                                return 0x18;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x499f20, execute of 0x499c10 */

int ai_Unk499f20(AIPER *p, AIARG *s1, AIARG *s2)
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
    msg.u.s.unk84 = s1->arg2;
    msg.ch = c->netId;
    msg.arg[0] = unk_Unk4f8700(c);
    ai_Unkf84cf4[0] = ai_Unkf84cd4[0] * s1->unk14;
    ai_Unkf84cf4[2] = ai_Unkf84cd4[2] * s1->unk14;
    ai_Unkf84cf4[4] = ai_Unkf84cd4[4] * s1->unk14;
    ai_Unkf84cf4[6] = ai_Unkf84cd4[6] * s1->unk14;
    cm_BeginGroup("pruegel");
    cm_SendLongMessage(&msg);
    cm_EndGroup("pruegel");
    return 0x18;
}
/* 0x49a020, decide: action 0x19 for a character with flag 0x4000 and little money */

int ai_Unk49a020(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int m;

    if (b != 1) {
        if (b != 0)
            return 0;
    } else if (o1->kind != 0xd || o1->ref != 2)
        return 0;
    {
        if (p->unk00d < 2 && (p->flags1c8 & 0x4000)) {
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
/* 0x49a0a0, execute callback: a public announcement, every councillor (kind 6 / 7) is told */

int ai_Unk49a0a0(G5CH *p)
{
    G8MSG m;
    AIBLD *b;
    AIOBJ *o;
    unsigned char *c;
    AITIME t;
    int n;
    int v;

    b = gm_BuildQuery(1, 5, 0xf);
    v = 2;
    if (b == 0)
        return 0;
    o = gm_Query(*(int *)((char *)b + 0x5d), 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    G8_4C4830I(*(int *)((char *)o + 2), p->netId, 0, 16000);
    cm_BeginFieldBlock((AIPER *)p, p->netId);
    cm_AddFieldDelta(1, 1, 0xd, &v);
    cm_SendFieldDeltas();
    cm_SendSetField(p->netId, 0x1c8, 4, 0, 0x4000);
    t = ai_TimeT;
    gm_AddTime(&t, util_RandomU(4) + 1, util_RandomU(8) * 7, 0);
    if (((unsigned char *)p)[2] == 5) {
        n = 0;
        for (c = (unsigned char *)ai_CharsT + 2; c < (unsigned char *)(ai_CharsT + 768) + 2; c += 0x21c) {
            if (n >= (int)ai_NumPlayers)
                return 0x19;
            if (c[0] == 6 || c[0] == 7) {
                AI_ZERO(&m, 0xf8);
                m.to = *(unsigned int *)(c + 2);
                m.time = t;
                m.from = p->netId;
                m.type = 0x69;
                m.unk0c = -1;
                m.unk36 = 9;
                m.unk58 = 2;
                cm_SendLongMessage((AIMSG *)&m);
                n++;
            }
        }
    }
    return 0x19;
}
/* 0x49a290, decide callback 0x1a: only actions that do not raise need 0 (APS) */

int ai_Unk49a290(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
    r = ai_Unk495a50(0x1a, p->id, 0, &s1, &s2);
    if (r == 0)
        return 0;
    act = &G5_ACTS[r];
    for (k = 0; k < 4; k++) {
        if (act->w1[k].k == 0 && act->w1[k].w > 0.0f)
            return 0;
    }
    for (k = 0; k < 4; k++) {
        if (act->w2[k].k == 0 && act->w2[k].w > 0.0f)
            return 0;
    }
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49a380, execute callback 0x1a */

int ai_Unk49a380(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0x1a;
}
/* 0x49a390, decide callback: a building of type 0x161 for the character's workshop, if its workers are
   not too tired */

int ai_Unk49a390(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k;
    int b;
    float x;
    int st[3];
    int rec[4];
    AIARG s1;
    AIARG s2;
    int o;
    int n, sum, r;
    unsigned char *c;

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    if (ai_PickActionBuilding((AIPER *)p, 4, 0x90010, 0, 100.0f, &b) == 0)
        return 0;
    G8_4FEA50(&o, 0, 0, b);
    if (o == 0)
        return 0;
    n = 0;
    sum = 0;
    for (c = (unsigned char *)ai_CharsT; c < (unsigned char *)(ai_CharsT + 768); c += 0x21c) {
        if (*(int *)(c + 0x16c) == o) {
            n++;
            sum += c[0x1c5];
        }
    }
    x = (float)util_RandomU(4);
    if (x + (float)sum / (float)n > 6.0f)
        return 0;
    rec[1] = 0;
    rec[2] = 0;
    rec[3] = 0;
    rec[0] = 0x161;
    ai_Unk4a5760(p, 1, rec, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 1)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            s1.ref = rec[0];
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    s1.kind = 1;
    s1.ref = rec[2];
    s1.arg = 1;
    *o1 = s1;
    s2.kind = 4;
    s2.ref = *(int *)((char *)o + 1);
    s2.arg = -1;
    s2.arg2 = 0x1b;
    *o2 = s2;
    return 0x1b;
}
/* 0x49a5b0, execute callback 0x1b: uses the object s1->unk04 of the character's list */

int ai_Unk49a5b0(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = s2->ref;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1b].w3[0].w = G5_ACTS[0x1b].w1[0].w * g[3];
                    return 0x1b;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1b].w3[0].w = G5_ACTS[0x1b].w1[0].w * g[3] * -0.5f;
                return 0x1b;
            }
        }
    }
    return 0;
}
/* 0x49a690, decide callback 0x1c: one of twelve goods (0x155..0x2b6) the character lacks */

int ai_Unk49a690(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    unsigned int flags;
    unsigned short id;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[12];
    int info[4];

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
    ai_Unk4a5760(p, 12, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 12)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            if (amt_GetKindB(((unsigned char *)p)[0x166], info) == 0)
                i = 7;
            else if ((unsigned char)info[2] < 4)
                i = 0;
            else
                i = (unsigned char)info[2] < 7 ? 2 : 4;
            while (recs[i].unk04 != 0)
                i = (i + 1) % 12;
            s1.ref = recs[i].item;
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(12);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 12;
    s2.kind = 1;
    s2.ref = recs[i].unk08;
    s2.arg = 1;
    s1.kind = 0;
    s1.ref = -1;
    s1.arg = -1;
    s1.arg2 = 0x1c;
    if (i == 9) {
        flags = (((unsigned char *)p)[9] != 0 ? 1 : 2) | ((unsigned)&cm_SendRing + 0x4973c0);
        if (G8_4A58D0(p, 1, &flags, 10.0f, (float)util_RandomU(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.ref = ai_CharsT[id].netId;
        s1.arg = -1;
    } else if (i == 10) {
        flags = (((unsigned char *)p)[9] != 1 ? 1 : 2) | ((unsigned)&cm_SendRing + 0x4973c0);
        if (G8_4A58D0(p, 1, &flags, 10.0f, (float)util_RandomU(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.ref = ai_CharsT[id].netId;
        s1.arg = -1;
    } else if (i == 8) {
        r = ai_Unk495a50(k, p->id, 3, &s2, &s1);
        if (r == 0)
            return r;
        *o1 = s2;
        *o2 = s1;
        return r;
    }
    *o1 = s2;
    *o2 = s1;
    return 0x1c;
}
/* 0x49aa80, execute callback 0x1c: uses the object s1->unk04 of the character's list */

int ai_Unk49aa80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk0c = 0;
            u.unk10 = 0;
            u.unk04 = s2->ref;
            u.unk08 = s2->arg;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1c].w3[0].w = G5_ACTS[0x1c].w1[0].w * g[3];
                    return 0x1c;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1c].w3[0].w = G5_ACTS[0x1c].w1[0].w * g[3] * -0.5f;
                return 0x1c;
            }
        }
    }
    return 0;
}
/* 0x49ab60, decide callback 0x1d: one of two goods (0x173, 0x15b) */

int ai_Unk49ab60(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[2];

    k = a;
    if (a == 0)
        k = 0x1d;
    if (f != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x173;
    recs[1].item = 0x15b;
    ai_Unk4a5760(p, 2, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 2)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(2);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 2;
            s1.kind = 2;
            s1.ref = recs[i].item;
            s1.arg = 1;
            s2.arg = 1;
            s2.ref = p->netId;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(2);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 2;
    s2.kind = 1;
    s2.arg = 1;
    s2.ref = recs[i].unk08;
    s1.kind = 0;
    s1.ref = -1;
    s1.arg = -1;
    s1.arg2 = 0x1d;
    if (i == 0) {
        r = ai_Unk495a50(k, p->id, 4, &s2, &s1);
        if (r == 0)
            return r;
        *o1 = s2;
        *o2 = s1;
        return r;
    }
    *o1 = s2;
    *o2 = s1;
    return 0x1d;
}
/* 0x49ad80, execute callback 0x1d: uses the object s1->unk04 of the character's list */

int ai_Unk49ad80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = -1;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1d].w3[0].w = G5_ACTS[0x1d].w1[0].w * g[3];
                    return 0x1d;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1d].w3[0].w = -(G5_ACTS[0x1d].w1[0].w * g[3] * 0.5f);
                return 0x1d;
            }
        }
    }
    return 0;
}
/* 0x49ae50, decide callback 0x1e: one of four goods (0x168, 0x16c, 0x297, 0x2b7) */

int ai_Unk49ae50(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[4];

    k = a;
    if (a == 0)
        k = 0x1e;
    if (f != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x168;
    recs[1].item = 0x16c;
    recs[2].item = 0x297;
    recs[3].item = 0x2b7;
    ai_Unk4a5760(p, 4, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 4)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(4);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 4;
            s1.kind = 2;
            s1.ref = recs[i].item;
            s1.arg = 1;
            s2.arg = 1;
            s2.ref = p->netId;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(4);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 4;
    s1.kind = 1;
    s1.arg = 1;
    s1.ref = recs[i].unk08;
    *o1 = s1;
    s2.kind = 0;
    s2.ref = -1;
    s2.arg = -1;
    s2.arg2 = 0x1e;
    *o2 = s2;
    return 0x1e;
}
/* 0x49b050, execute callback 0x1e: uses the object s1->unk04 of the character's list */

int ai_Unk49b050(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = -1;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1e].w3[0].w = G5_ACTS[0x1e].w1[0].w * g[3];
                    return 0x1e;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1e].w3[0].w = -(G5_ACTS[0x1e].w1[0].w * g[3]);
                return 0x1e;
            }
        }
    }
    return 0;
}
/* 0x49b120, decide callback 0x1f: good 0x17e (or now and then 0x175) */

int ai_Unk49b120(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, r;
    int st[3];
    G8REC rec;
    AIARG s1;
    AIARG s2;

    k = a;
    if (a == 0)
        k = 0x1f;
    if (f != 0)
        return 0;
    rec.item = 0;
    rec.unk04 = 0;
    rec.unk08 = 0;
    rec.unk0c = 0;
    if (*(int *)((char *)p + 0x2c) != 0)
        rec.item = 0x17e;
    else if (util_RandomU(3) != 0)
        return 0;
    else
        rec.item = 0x175;
    ai_Unk4a5760(p, 1, (int *)&rec, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 1)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            s1.ref = rec.item;
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    s1.ref = -1;
    s1.arg = -1;
    s2.ref = rec.unk08;
    s2.kind = 1;
    s2.arg = 1;
    s1.kind = 0;
    s1.arg2 = 0x1f;
    r = ai_Unk495a50(k, p->id, 4, &s2, &s1);
    if (r == 0)
        return r;
    *o1 = s2;
    *o2 = s1;
    return r;
}
/* 0x49b2b0, execute callback 0x1f: uses the object s1->unk04 of the character's list */

int ai_Unk49b2b0(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = -1;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x1f].w3[0].w = G5_ACTS[0x1f].w1[0].w * g[3];
                    return 0x1f;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x1f].w3[0].w = -(G5_ACTS[0x1f].w1[0].w * g[3]);
                return 0x1f;
            }
        }
    }
    return 0;
}
/* 0x49b380, decide callback 0x20: gifts for a young child (0x162..0x164), a fiancee (0x167) or a wedding
   ring (0x170) */

int ai_Unk49b380(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    int *rel;
    AIPER *c;
    AIPER *w;
    AIJOB *j;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[3];

    k = a;
    if (a == 0)
        k = 0x20;
    if (f != 0 || a != 0)
        return 0;
    rel = (int *)((char *)p + 0x5c);
    for (i = 3; i < 8; i++) {
        c = gm_FindPer(rel[i]);
        if (c != 0 && c->unk00a < 0x28)
            break;
    }
    for (i = 3; i < 8; i++) {
        c = gm_FindPer(rel[i]);
        if (c != 0 && c->unk00a < 0x28)
            break;
    }
    memset(recs, 0, sizeof(recs));
    if (c != 0) {
        recs[0].item = 0x162;
        recs[1].item = 0x163;
        recs[2].item = 0x164;
        n = 3;
    } else {
        if (rel[0] != -1)
            return 0;
        for (j = he_FindEvent(1, 0, 0x41); j != 0; j = he_NextEvent()) {
            if (((int *)j)[0xac / 4] == (int)p->netId)
                break;
        }
        if (j != 0) {
            w = gm_FindPer(((int *)j)[0xb0 / 4]);
            if (w == 0)
                return 0;
            recs[0].item = 0x167;
        } else {
            recs[0].item = 0x170;
        }
        recs[1].item = 0;
        recs[2].item = 0;
        n = 1;
    }
    ai_Unk4a5760(p, n, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == n)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(n);
            while (recs[i].unk04 != 0)
                i = (i + 1) % n;
            s2.kind = 2;
            s2.ref = recs[i].item;
            s1.ref = p->netId;
            s2.arg = 1;
            s2.unk14 = 0.05f;
            s1.kind = 7;
            s1.arg = 1;
            r = ai_Unk495a50(k, p->id, 2, &s2, &s1);
            goto done;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(n);
    while (recs[i].unk04 < 1)
        i = (i + 1) % n;
    s2.kind = 1;
    s2.arg = 1;
    s2.ref = recs[i].unk08;
    s2.unk14 = 0.05f;
    switch (recs[i].item) {
    case 0x170:
        s1.kind = 0;
        s1.arg = -1;
        s1.ref = -1;
        break;
    case 0x167:
        s1.kind = 7;
        s1.arg = -1;
        s1.ref = w->netId;
        break;
    case 0x162:
    case 0x163:
    case 0x164:
        s1.kind = 7;
        s1.arg = -1;
        s1.ref = c->netId;
        break;
    default:
        return 0;
    }
    s1.arg2 = 0x20;
    if (recs[i].item != 0x170) {
        *o1 = s2;
        *o2 = s1;
        return 0x20;
    }
    r = ai_Unk495a50(k, p->id, 3, &s2, &s1);
done:
    if (r == 0)
        return r;
    *o1 = s2;
    *o2 = s1;
    return r;
}
/* 0x49b6f0, execute callback 0x20: uses the object s1->unk04 of the character's list */

int ai_Unk49b6f0(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)G8_5BA620(*(unsigned short *)o);
    if (g == 0)
        return 0;
    u.obj = o;
    u.unk04 = s2->ref;
    u.unk08 = -1;
    u.unk0c = 0;
    u.unk10 = 0;
    if (ai_Unk5cf8a0(p, &u) != 1)
        return 0;
    if (u.unk0c == 1) {
        G5_ACTS[0x20].w3[0].w = G5_ACTS[0x20].w1[0].w * g[3];
        return 0x20;
    }
    if (u.unk0c == -1)
        G5_ACTS[0x20].w3[0].w = -(G5_ACTS[0x20].w1[0].w * g[3] * 0.5f);
    return 0x20;
}
/* 0x49b7c0, decide callback 0x21: one of twelve goods (0x169..0x29c), some given to one or two characters */

int ai_Unk49b7c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    unsigned int flags[2];
    unsigned short id[2];
    int v;
    LARGE_INTEGER mask;
    G8REC *rp;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[12];

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
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
    ai_Unk4a5760(p, 12, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 12)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(12);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 12;
            s1.ref = recs[i].item;
            s2.ref = p->netId;
            s1.kind = 2;
            s1.arg = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.arg = 1;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(12);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 12;
    s1.kind = 1;
    s1.arg = 1;
    s1.unk14 = 0.05f;
    rp = &recs[i];
    s1.ref = recs[i].unk08;
    if (rp->item == 0x169 || rp->item == 0x16d) {
        flags[0] = ((unsigned)&cm_SendRing + 0x4973c0);
        flags[1] = ((unsigned)&cm_SendRing + 0x4973c0);
        n = 2;
    } else {
        n = 1;
        if (rp->item == 0x29a)
            flags[0] = ((unsigned)&cm_SendRing + 0x497bc0);
        else
            flags[0] = ((unsigned)&cm_SendRing + 0x4973c0);
    }
    if (rp->item == 0x177)
        flags[0] |= 0x10000;
    if (rp->item == 0x178) {
        mask.LowPart = 0xa07c4390;
        mask.HighPart = 1;
        v = -1;
        if (!(ai_Partner && (ai_PickActionBuilding((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &v), v != -1))
            && !ai_PickActionBuilding((AIPER *)p, 6, mask.QuadPart, 0, (float)util_RandomU(0x40), &v))
            return 0;
    } else if (G8_4A58D0(p, n, flags, 0.0f, (float)util_RandomU(0x40), id) == 0) {
        return 0;
    }
    switch (rp->item) {
    case 0x169:
    case 0x16d:
        s2.ref = ai_CharsT[id[0]].netId;
        s2.arg = ai_CharsT[id[1]].netId;
        s2.kind = 7;
        break;
    case 0x177:
        s2.kind = 4;
        s2.ref = *(int *)((char *)ai_CharsT[id[0]].bld2 + 1);
        s2.arg = -1;
        break;
    case 0x178:
        s2.kind = 4;
        s2.ref = v;
        s2.arg = -1;
        break;
    case 0x16a:
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
    r = 0x21;
    if (i > 1) {
        r = ai_Unk495a50(k, p->id, 4, &s1, &s2);
        if (r == 0)
            return r;
    }
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49bc50, execute callback 0x21: uses the object s1->unk04 of the character's list, scales four needs */

int ai_Unk49bc50(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = s2->ref;
            u.unk08 = s2->arg;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x21].w3[0].w = G5_ACTS[0x21].w1[0].w * g[3];
                    G5_ACTS[0x21].w3[1].w = G5_ACTS[0x21].w1[1].w * g[3];
                    G5_ACTS[0x21].w3[2].w = G5_ACTS[0x21].w1[2].w * g[3];
                    G5_ACTS[0x21].w3[3].w = G5_ACTS[0x21].w1[3].w * g[3];
                    return 0x21;
                }
                if (u.unk0c == -1) {
                    G5_ACTS[0x21].w3[0].w = -(G5_ACTS[0x21].w1[0].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[1].w = -(G5_ACTS[0x21].w1[1].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[2].w = -(G5_ACTS[0x21].w1[2].w * g[3] * 0.5f);
                    G5_ACTS[0x21].w3[3].w = -(G5_ACTS[0x21].w1[3].w * g[3] * 0.5f);
                }
                return 0x21;
            }
        }
    }
    return 0;
}
/* 0x49bd90, decide callback 0x22: one of ten goods (0x17c..0x2b3) */

int ai_Unk49bd90(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[10];

    k = a;
    if (a == 0)
        k = 0x22;
    if (f != 0 || a != 0)
        return 0;
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
    ai_Unk4a5760(p, 10, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 10)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(10);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 10;
            s1.ref = recs[i].item;
            s1.arg = 1;
            s2.arg = 1;
            s1.kind = 2;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.ref = p->netId;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(10);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 10;
    s1.kind = 1;
    s1.arg = 1;
    s2.kind = 0;
    s1.ref = recs[i].unk08;
    s2.ref = -1;
    s2.arg = -1;
    s2.arg2 = 0x22;
    if (i == 0) {
        r = ai_Unk495a50(k, p->id, 4, &s1, &s2);
    } else if (i < 4) {
        r = ai_Unk495a50(k, p->id, 3, &s1, &s2);
    } else {
        *o1 = s1;
        *o2 = s2;
        return 0x22;
    }
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49c000, execute callback 0x22: uses the object s1->unk04 of the character's list */

int ai_Unk49c000(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o != 0) {
        g = (float *)G8_5BA620(*(unsigned short *)o);
        if (g != 0) {
            u.obj = o;
            u.unk04 = -1;
            u.unk08 = -1;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    G5_ACTS[0x22].w3[0].w = G5_ACTS[0x22].w1[0].w * g[3];
                    return 0x22;
                }
                if (u.unk0c == -1)
                    G5_ACTS[0x22].w3[0].w = G5_ACTS[0x22].w1[0].w * g[3] * -0.5f;
                return 0x22;
            }
        }
    }
    return 0;
}
/* 0x49c0d0, eval callback for the goods actions 0x1c..0x22: decision 0x11 decides the rating */

int ai_Unk49c0d0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    if (o2->arg2 >= 0x1c && o2->arg2 <= 0x22) {
        gv_GetTableRecord(0x11, (AIPOS *)&d);
        if (d.cur != 0)
            return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
        return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
    }
    return 0;
}
/* 0x49c170, decide callback 0x23 */

int ai_Unk49c170(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 3)
        return 0x23;
    return 0;
}
/* 0x49c190, execute callback 0x23: uses the object, the needs of action s2->unk10 scale those of 0x23 */

int ai_Unk49c190(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)G8_5BA620(*(unsigned short *)o);
    if (g == 0)
        return 0;
    u.obj = o;
    u.unk04 = s2->ref;
    u.unk08 = s2->arg;
    if (ai_Unk5cf8a0(p, &u) != 1)
        return 0;
    if (u.unk0c == 1) {
        G5_ACTS[0x23].w3[0].w = G5_ACTS[s2->arg2].w1[0].w * g[3];
        G5_ACTS[0x23].w3[1].w = G5_ACTS[s2->arg2].w1[1].w * g[3];
        G5_ACTS[0x23].w3[2].w = G5_ACTS[s2->arg2].w1[2].w * g[3];
        G5_ACTS[0x23].w3[3].w = G5_ACTS[s2->arg2].w1[3].w * g[3];
        return 0x23;
    }
    if (u.unk0c == -1) {
        G5_ACTS[0x23].w3[0].w = -(G5_ACTS[s2->arg2].w1[0].w * g[3] * 0.5f);
        G5_ACTS[0x23].w3[1].w = -(G5_ACTS[s2->arg2].w1[1].w * g[3] * 0.5f);
        G5_ACTS[0x23].w3[2].w = -(G5_ACTS[s2->arg2].w1[2].w * g[3] * 0.5f);
        G5_ACTS[0x23].w3[3].w = -(G5_ACTS[s2->arg2].w1[3].w * g[3] * 0.5f);
    }
    return 0x23;
}
/* 0x49c320, eval callback for the goods actions 0x1c..0x22: decision 0x12 decides the rating */

int ai_Unk49c320(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    if (o2->arg2 >= 0x1c && o2->arg2 <= 0x22) {
        gv_GetTableRecord(0x12, (AIPOS *)&d);
        if (d.cur != 0)
            return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
        return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
    }
    return 0;
}
/* 0x49c3c0, decide callback 0x24 */

int ai_Unk49c3c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 4)
        return 0x24;
    return 0;
}
/* 0x49c3e0, execute callback 0x24: uses the object, the needs of action s2->unk10 scale those of 0x24 */

int ai_Unk49c3e0(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8USE u;
    AIOBJ *o;
    float *g;

    o = gm_Query(*(int *)((char *)p + 0x178), 1, 1, s1->ref);
    if (o == 0)
        return 0;
    g = (float *)G8_5BA620(*(unsigned short *)o);
    if (g == 0)
        return 0;
    u.obj = o;
    u.unk04 = s2->ref;
    u.unk08 = s2->arg;
    if (ai_Unk5cf8a0(p, &u) != 1)
        return 0;
    if (u.unk0c == 1) {
        G5_ACTS[0x24].w3[0].w = G5_ACTS[s2->arg2].w1[0].w * g[3];
        G5_ACTS[0x24].w3[1].w = G5_ACTS[s2->arg2].w1[1].w * g[3];
        G5_ACTS[0x24].w3[2].w = G5_ACTS[s2->arg2].w1[2].w * g[3];
        G5_ACTS[0x24].w3[3].w = G5_ACTS[s2->arg2].w1[3].w * g[3];
        return 0x24;
    }
    if (u.unk0c == -1) {
        G5_ACTS[0x24].w3[0].w = -(G5_ACTS[s2->arg2].w1[0].w * g[3] * 0.5f);
        G5_ACTS[0x24].w3[1].w = -(G5_ACTS[s2->arg2].w1[1].w * g[3] * 0.5f);
        G5_ACTS[0x24].w3[2].w = -(G5_ACTS[s2->arg2].w1[2].w * g[3] * 0.5f);
        G5_ACTS[0x24].w3[3].w = -(G5_ACTS[s2->arg2].w1[3].w * g[3] * 0.5f);
    }
    return 0x24;
}
/* 0x49c570, decide callback 0x25: spying on a character (0x49c700 sends "spionage"), if the character can pay for it */

int ai_Unk49c570(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned int flags;
    unsigned short id;
    int r, money, v, x;
    G5CH *c;

    flags = ((unsigned)&cm_SendRing + 0x4973c0);
    r = util_RandomU(3);
    money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (f != 0)
        return 0;
    if (ai_PickPartnerChain((AIPER *)p, 1, (int *)&flags, 0, (float)util_RandomU(0x19) + 10.0, &id) == 0)
        return 0;
    c = &G5_CHARS[id];
    x = (int)(gm_GetWorth(c->id) * 0.005f + gm_GetWorth(p->id) * 0.005f);
    v = (int)(x * ((float)r * 0.33f + 1.0f));
    if (!((float)v <= money * 0.22f))
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

int ai_Unk49c700(G5CH *p, AIARG *s1)
{
    G8MSG m;
    unsigned char *h;

    if (s1->kind != 7)
        return 0;
    m.type = 0x18;
    m.unk36 = 2;
    m.to = p->netId;
    m.unk0c = -1;
    if ((h = *(unsigned char **)((char *)p + 0x170)) != 0)
        *(int *)&m.unk58 = *(int *)(h + 1);
    else
        *(int *)&m.unk58 = -1;
    *(int *)((char *)&m + 0x70) = s1->ref;
    *(int *)((char *)&m + 0x90) = s1->arg2;
    cm_BeginGroup("spionage");
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("spionage");
    return 0x25;
}
/* 0x49c790, callback 0x0c: buy the good s1->unk04 at the place that sells it, if the character can pay
   (s1->unk14 in (0, 1) is the share of the money to use) */

int ai_Unk49c790(int a, G5CH *p, int f, AIARG *s1, AIARG *o)
{
    unsigned char *bld;
    AIOBJ *obj;
    int money;

    if (f == 2 && s1->kind == 2) {
        bld = (unsigned char *)gm_FindMarketObject(s1->ref);
        if (bld != 0) {
            obj = gm_Query(*(int *)(bld + 0x14), 1, 0, s1->ref);
            if (obj != 0 && gm_ListAmount(bld, obj) >= 1) {
                money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
                if (s1->unk14 > 0.0f && s1->unk14 < 1.0f)
                    money = (int)(money * s1->unk14);
                if (money >= (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer)) {
                    o->kind = 1;
                    o->ref = *(int *)(bld + 2);
                    o->arg = 1;
                    return 0xc;
                }
            }
        }
    }
    return 0;
}
/* 0x49c880, execute callback 0x0c: the character buys the good s1->unk04 from s2->unk04 at its price */

int ai_Unk49c880(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObjectI(p->netId, s2->ref, s1->ref, s1->arg, ai_OwnPlayer,
                 (int)gm_GoodsPrice(s1->ref, ai_OwnPlayer));
    return 0xc;
}
/* 0x49c8e0, decide callback 0x26: on its day / hour slot, the character rents rooms (?) in a building
   (1, 5, 7) of another owner; the price depends on its fame */

int ai_Unk49c8e0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int n1, n2, i, money, w, c;
    float x, t;
    unsigned char *b;
    unsigned char *list[10];

    if (f != 0)
        return 0;
    if ((p->netId & 3) != ai_TimeT.year % 4)
        return 0;
    if ((p->netId & 7) != (int)ai_TimeT.hour % 8)
        return 0;
    n1 = gv_CulpritGuilt(p->netId);
    n2 = gv_CountOwnedGraves(p->netId);
    if (util_RandomU(8) + 8 > n1)
        return 0;
    i = 0;
    for (b = (unsigned char *)gm_BuildQuery(1, 5, 7); b != 0; b = (unsigned char *)gm_BuildNext()) {
        if (*(unsigned short *)(b + 0x27) != 0xffff && *(unsigned short *)(b + 0x27) != p->id
            && !(b[0x5a] & 1))
            list[i++] = b;
    }
    if (i == 0)
        return 0;
    b = list[util_RandomU(i)];
    if (*(int *)((char *)p + 0x194) < 4) {
        money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
        x = gm_GetWorth(p->id) * 0.005f;
        w = (int)((200.0f - gm_Liking(p->id, *(unsigned short *)(b + 0x27), 1)) * 0.01f * n1 * x);
        t = w * 0.01;
        if (t > 3200.0) {
            if (!(t < 320000.0))
                t = 320000.0;
        } else
            t = 3200.0;
        w = (int)t;
        if (ai_InventoryHas(p, 0x2c6))
            w /= 2;
        if (money * 0.44f < w)
            return 0;
        if (n2 <= 1)
            n2 = 1;
        o1->kind = 4;
        o1->ref = *(int *)(b + 1);
        o1->arg2 = w;
        o1->unk14 = (float)n2;
        o2->kind = 0x12;
        return 0x26;
    }
    c = *(int *)((char *)p + 0x194) / 2;
    c += c & 1;
    w = c / 2;
    if (w > n2) {
        w = n2;
        c = n2 * 2;
    }
    o1->kind = 4;
    o1->ref = *(int *)(b + 1);
    o1->arg2 = c;
    o1->unk14 = (float)w;
    o2->kind = 0xf;
    return 0x26;
}
/* 0x49cb60, execute callback 0x26: pays the rent (s2->kind 0x12, with a message) or takes over the rooms
   (s2->kind 0xf) of the building s1->unk04 */

int ai_Unk49cb60(G5CH *p, AIARG *s1, AIARG *s2)
{
    unsigned char *b;
    AIOBJ *o;
    int n, d;

    if (s1->kind != 4)
        return 0;
    b = (unsigned char *)gm_BuildQuery(1, 1, s1->ref);
    if (b == 0 || *(unsigned short *)(b + 0x27) == 0xffff)
        return 0;
    o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xf2);
    if (s2->kind == 0x12) {
        n = s1->arg2;
        d = (int)(n / log10(n));
        cm_BeginFieldBlock((AIPER *)p, p->netId);
        cm_AddFieldValues(4, 1, 0x1ac, &s1->arg2);
        cm_SendFieldDeltas();
        G8_4C4830(-1, p->netId, ai_OwnPlayer, s1->arg2);
        G8_4C4830(ai_CharsT[*(unsigned short *)(b + 0x27)].netId, -1, ai_OwnPlayer, d);
        gv_RetireCrimesOf(p->netId, (int)s1->unk14);
        ai_Unk5a8b80(p, &ai_CharsT[*(unsigned short *)(b + 0x27)], b, d, s1->arg2);
        G5_ACTS[0x26].w3[0].w = G5_ACTS[0x26].w1[0].w * s1->unk14;
        G5_ACTS[0x26].w3[1].w = G5_ACTS[0x26].w1[1].w * s1->unk14;
        G5_ACTS[0x26].w3[2].w = G5_ACTS[0x26].w1[2].w * s1->unk14;
        return 0x26;
    }
    if (s2->kind == 0xf && o != 0) {
        gv_RetireCrimesOf(p->netId, (int)s1->unk14);
        gm_SendMoneyDelta(p->netId, -s1->arg2);
        gm_SendMoneyDelta(ai_CharsT[*(unsigned short *)(b + 0x27)].netId, 1);
        ai_Unk5a8c00(p, &ai_CharsT[*(unsigned short *)(b + 0x27)], b);
        return 0x26;
    }
    return 0;
}
/* 0x49cd30, decide callback 0x27: what 0x4a9a00 decides */

int ai_Unk49cd30(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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

int ai_Unk49cdb0(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckLeisureAction(p, s1, s2) ? 0x27 : 0;
}
/* 0x49cdd0, eval callback 0x27: decision 0x13 decides the rating */

int ai_Unk49cdd0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x13, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x49ce50, decide callback 0x28: get a house (kind 5) or the good 0x15 for it, else what 0x4aa6a0 finds,
   if the other character (0x504e20) is willing */

int ai_Unk49ce50(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned char *h;
    AIARG s1;
    AIARG s2;
    float g;
    int r;

    if (f != 0)
        return 0;
    h = *(unsigned char **)((char *)p + 0x170);
    if (h == 0) {
        if (((unsigned char *)p)[2] != 3) {
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
    } else if (((unsigned char *)p)[2] != 3) {
        if (gm_Query(*(int *)(h + 0x5d), 2, 6, 0, 0x15) == 0) {
            s1.kind = 2;
            s1.ref = 0x15;
            s1.arg = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.ref = *(int *)(h + 1);
            s2.arg = 1;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
        goto find;
    }
    if (!(((unsigned char *)p)[0x1e8] & 4))
        return 0;
find:
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (ai_DecideLittleKnownChar(p, &s1, &s2) == 0)
        return 0;
    g = gm_Mix((AIPER *)p, (AIPER *)&ai_CharsT[s2.arg2], 4);
    if (util_RandomF() + g < 1.0f) {
        s1.kind = 4;
        s1.ref = 3;
        s1.arg = 1;
        s1.unk14 = 0.0f;
        s2.kind = 0;
        s2.ref = 0;
        s2.arg = 0;
        r = ai_Unk495a50(0x28, p->id, 0, &s1, &s2);
        goto done;
    }
    *o1 = s1;
    *o2 = s2;
    return 0x28;
done:
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49d030, execute callback 0x28 */

int ai_Unk49d030(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_Unk4aaa10(p, s1, s2) ? 0x28 : 0;
}
/* 0x49d050, eval callback 0x28: decision 3 decides the rating; none while f is set */

int ai_Unk49d050(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d0f0, decide callback 0x29: as 0x28 (house first), then what 0x4aa580 finds; flag 0x100 of +0x1e8
   makes it rare */

int ai_Unk49d0f0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned char *h;
    AIARG s1;
    AIARG s2;
    AIARG t1;
    AIARG t2;
    int r;

    if (f != 0)
        return 0;
    if ((*(int *)((char *)p + 0x1e8) & 0x100) && util_RandomU(4) != 0)
        return 0;
    h = *(unsigned char **)((char *)p + 0x170);
    if (h == 0) {
        if (((unsigned char *)p)[2] != 3) {
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
    } else if (((unsigned char *)p)[2] != 3) {
        if (gm_Query(*(int *)(h + 0x5d), 2, 6, 0, 0x15) == 0) {
            s1.kind = 2;
            s1.ref = 0x15;
            s1.arg = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.ref = *(int *)(h + 1);
            s2.arg = 1;
            r = ai_Unk495a50(0x29, p->id, 1, &s1, &s2);
            goto done;
        }
        goto find;
    }
    if (!(((unsigned char *)p)[0x1e8] & 4))
        return 0;
find:
    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));
    if (ai_Unk4aa580(p, &t1, &t2) == 0)
        return 0;
    *o1 = t1;
    *o2 = t2;
    return 0x29;
done:
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49d280, execute callback 0x29: sets flag 0x100 of +0x1e8 */

int ai_Unk49d280(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (aip_ExecPamphlet(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x100, 0);
        return 0x29;
    }
    return 0;
}
/* 0x49d2d0, decide callback 0x2a: as 0x28 (house first), then what 0x4aa800 finds; flag 0x200 of +0x1e8
   makes it rare */

int ai_Unk49d2d0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned char *h;
    AIARG s1;
    AIARG s2;
    int r;

    if (f != 0)
        return 0;
    if ((*(int *)((char *)p + 0x1e8) & 0x200) && util_RandomU(8) != 0)
        return 0;
    h = *(unsigned char **)((char *)p + 0x170);
    if (h == 0) {
        if (((unsigned char *)p)[2] != 3) {
            s1.arg = 1;
            s1.kind = 5;
            s1.ref = 4;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
    } else if (((unsigned char *)p)[2] != 3) {
        if (gm_Query(*(int *)(h + 0x5d), 2, 6, 0, 0x15) == 0) {
            s1.kind = 2;
            s1.ref = 0x15;
            s1.arg = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.ref = *(int *)(h + 1);
            s2.arg = 1;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
        goto find;
    }
    if (!(((unsigned char *)p)[0x1e8] & 4))
        return 0;
find:
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (ai_DecideCharPair(p, &s1, &s2) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x2a;
done:
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49d460, execute callback 0x2a: sets flag 0x200 of +0x1e8 */

int ai_Unk49d460(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (aip_ExecAnschwaerzen(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x200, 0);
        return 0x2a;
    }
    return 0;
}
/* 0x49d4b0, eval callback 0x2a: decision 4 decides the rating; none while f is set */

int ai_Unk49d4b0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d550, decide callback 0x2b: what 0x4aa120 finds for the kind 0x10 / 8 */

int ai_Unk49d550(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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

int ai_Unk49d5e0(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2b : 0;
}
/* 0x49d600, eval callback 0x2b: decision 0x16 decides the rating; none while f is set */

int ai_Unk49d600(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49d6a0, decide callback 0x2c: what 0x4aa120 finds for the kind 0x10 / 4 */

int ai_Unk49d6a0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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

int ai_Unk49d730(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2c : 0;
}
/* 0x49d750, decide callback 0x2d: what 0x4aa120 finds */

int ai_Unk49d750(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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

int ai_Unk49d7d0(G5CH *p, AIARG *s1, AIARG *s2)
{
    return ai_CheckVisitAction(p, s1, s2) ? 0x2d : 0;
}
/* 0x49d7f0, eval callback (characters with +0x166 == 0xf): rated three times higher when 0x4f9e00 is above
   the character's value +0x100 */

int ai_Unk49d7f0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    float d[10];

    if (((unsigned char *)p)[0x166] != 0xf) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    ord_GetStatistics(d);
    if ((*(float *)((char *)p + 0x100) - 2.0) * 0.4f < d[3]) {
        G8_4A4AE0(a, b, i, p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x49d8d0, decide callback 0x2e (characters with +0x166 == 0xf): buy a building of kind 1 (type 2) or
   kind 1 (type 3) when 0x4f9e00 is above the value +0x100, else work on the weakest building (+0x5c < 0x50) */

int ai_Unk49d8d0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *best, *recA, *recB, *r, *b, *rec;
    AIOBJ *o;
    unsigned short fl;
    unsigned char k;
    char lvl;
    int cntA, cntB, n, money, v;
    AIARG s1;
    AIARG s2;
    float d[10];

    best = 0;
    recA = 0;
    recB = 0;
    cntA = 0;
    cntB = 0;
    if (a != 0 || f != 0 || ((unsigned char *)p)[0x166] != 0xf)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(*(int *)((char *)o + 0x14), ai_OwnPlayer);
    ord_GetStatistics(d);
    rec = ai_BldsT;
    for (n = 0x100; n != 0; n--, rec++) {
        k = rec->type;
        if (k == 0 || ai_BTypesT[k].kind != 1)
            continue;
        fl = *(unsigned short *)&rec->flags5a;
        if (fl & 0x45)
            continue;
        lvl = rec->level;
        if (lvl < 0x50 && (best == 0 || best->level > lvl))
            best = rec;
        if (k == 3)
            continue;
        if (k == 2) {
            cntB++;
            if (!(fl & 0x45))
                recB = rec;
        } else {
            cntA++;
            if (!(fl & 0x45))
                recA = rec;
        }
    }
    if ((*(float *)((char *)p + 0x100) - 2.0) * 0.4f < d[3]) {
        if (cntA != 0 && recA != 0) {
            r = recA;
            o1->kind = 4;
            o1->ref = 2;
            v = gm_BuildingKindValue(2);
        } else if (cntB != 0 && recB != 0) {
            r = recB;
            o1->kind = 4;
            o1->ref = 3;
            v = gm_BuildingKindValue(3);
        } else {
            s1.kind = 5;
            s1.ref = 1;
            v = gm_BuildingKindValue(1);
            if (money < v * 1.2f)
                return 0;
            s1.arg = 1;
            s2.kind = 0;
            s2.ref = 0;
            s2.arg = 0;
            if (ai_Unk4a5140(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return 0x2e;
        }
        if (money < v * 0.4f)
            return 0;
        o1->arg = 1;
        o2->kind = 4;
        o2->ref = r->id;
        o2->arg = 1;
        return 0x2e;
    }
    if (best == 0)
        return 0;
    lvl = best->level;
    if (lvl - util_RandomU(0x20) >= 0x42)
        return 0;
    s1.ref = best->id;
    s1.kind = 4;
    s1.arg = 1;
    if (lvl < 0x21)
        s1.arg2 = 0x32;
    else
        s1.arg2 = lvl >= 0x42 ? 10 : 30;
    s2.ref = *(int *)((char *)o + 2);
    s2.arg = money;
    s2.arg2 = 1;
    s2.kind = 1;
    v = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
    if (v == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return v;
}
/* 0x49dc10, execute callback 0x2e: upgrades ("upgr_arbeiter_unterk", s2->kind 4) or builds
   ("bau_arbeiter_unterk") the workers' lodging, paid to 0x504a80(1) */

int ai_Unk49dc10(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0 || ((unsigned char *)p)[0x166] != 0xf)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.unk0c = s2->ref;
        m.unk36 = 1;
        m.time = ai_TimeT;
        *(int *)&m.unk58 = s2->ref;
        cm_BeginGroup("upgr_arbeiter_unterk");
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("upgr_arbeiter_unterk");
        return 0x2e;
    }
    cm_BeginGroup("bau_arbeiter_unterk");
    if (ai_ExecBuildBuilding((AIPER *)ai_PlayerChar, s1, s2) != 0) {
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, gm_BuildingKindValue(s1->ref));
        cm_EndGroup("bau_arbeiter_unterk");
        return 0x2e;
    }
    cm_EndGroup("bau_arbeiter_unterk");
    return 0;
}
/* 0x49dd90, decide callback 0x15 (characters with +0x166 == 0xf): up to four tries to find a good to sell
   (as 0x499400 in am.c) */

int ai_Unk49dd90(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        if (!ai_Unk4a5540(&w, &z, y, 0))
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
        s2.unk14 = 0.44f;
        r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    return 0;
}
/* 0x49def0, decide callback 0x31 (characters with +0x166 == 0xf): for a building found by 0x4a63f0 (mask
   0x8000): work on it (+0x5c low), sell its goods, or upgrade it (type 0x24 -> 0x25, 0x25 -> 0x26) */

int ai_Unk49def0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    AIOBJ *o;
    LARGE_INTEGER m;
    float d[10];
    int x, w, z, r, money, act, v;
    char lvl;

    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 7 && ((unsigned char *)p)[0x130] != 6)
        return 0;
    m.LowPart = 0x8000;
    m.HighPart = 0;
    if (!ai_PickActionBuilding((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (y == 0)
        return 0;
    o = gm_Query(y->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(*(int *)((char *)o + 0x14), ai_OwnPlayer);
    if (!(y->flags5a & 4) && y->level < util_RandomU(0x40) + 0x25) {
        s1.kind = 4;
        s1.ref = y->id;
        s1.arg = 1;
        lvl = y->level;
        if (lvl < 0x21)
            s1.arg2 = 0x32;
        else
            s1.arg2 = lvl >= 0x42 ? 10 : 30;
        s2.kind = 1;
        s2.ref = *(int *)((char *)o + 2);
        s2.arg = money;
        s2.arg2 = 1;
        r = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
        if (r != 0)
            goto done;
    }
    if (ai_Unk4a5540(&w, &z, y, 0)) {
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
        s2.unk14 = 0.44f;
        r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
        if (r != 0)
            goto done;
    }
    if (y->flags5a & 0x41)
        return 0;
    ord_GetStatistics(d);
    if (y->type == 0x24) {
        act = 0x25;
        if (util_RandomU(0x200) + d[6] < 1250.0f)
            return 0;
    } else if (y->type == 0x25) {
        act = 0x26;
        if (util_RandomU(0x400) + d[6] < 5000.0f)
            return 0;
    } else
        return 0;
    v = gm_BuildingKindValue(act);
    if (money < v * 0.4f)
        return 0;
    o1->kind = 4;
    o1->ref = act;
    o1->arg = 1;
    o1->arg2 = 0;
    o2->kind = 4;
    o2->arg2 = 0;
    o2->ref = y->id;
    o2->arg = 1;
    return 0x31;
done:
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49e230, execute callback 0x31: upgrades the town hall ("upgr_rathaus"), paid to 0x504a80(1) */

int ai_Unk49e230(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0)
        return 0;
    if (((unsigned char *)p)[0x166] != 0xf)
        return 0;
    if (s2->kind != 4 && s1->arg2 != 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.to = p->netId;
    m.unk0c = s2->ref;
    m.unk36 = 1;
    m.time = ai_TimeT;
    *(int *)&m.unk58 = s2->ref;
    cm_BeginGroup("upgr_rathaus");
    cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_rathaus");
    return 0x31;
}
/* 0x49e350, decide callback 0x3c (characters with +0x166 == 0xf): as 0x31 for a building found with mask 0x40;
   upgrades type 0x11 -> 0x12, 0x12 -> 0x13 */

int ai_Unk49e350(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    unsigned char *o;
    LARGE_INTEGER m;
    float d[10];
    int x, w, z, r, money, act, v;
    char lvl;

    gm_PlayerSeat(1);
    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 7 && ((unsigned char *)p)[0x130] != 6)
        return 0;
    m.LowPart = 0x40;
    m.HighPart = 0;
    if (!ai_PickActionBuilding((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (y == 0)
        return 0;
    o = (unsigned char *)gm_PlayerSeat(1);
    if (o == 0)
        return 0;
    money = gm_SumListAmounts(*(int *)(o + 0x14), ai_OwnPlayer);
    if (!(y->flags5a & 4) && y->level < util_RandomU(0x40) + 0x25) {
        s1.kind = 4;
        s1.ref = y->id;
        s1.arg = 1;
        lvl = y->level;
        if (lvl < 0x21)
            s1.arg2 = 0x32;
        else
            s1.arg2 = lvl >= 0x42 ? 10 : 30;
        s2.kind = 1;
        s2.ref = *(int *)(o + 2);
        s2.arg = money;
        s2.arg2 = 1;
        r = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
        if (r != 0)
            goto done;
    }
    if (ai_Unk4a5540(&w, &z, y, 0)) {
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
        s2.unk14 = 0.44f;
        r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
        if (r != 0)
            goto done;
    }
    if (y->flags5a & 0x41)
        return 0;
    ord_GetStatistics(d);
    if (y->type == 0x11) {
        act = 0x12;
        if (util_RandomU(0x200) + d[6] < 1250.0f)
            return 0;
    } else if (y->type == 0x12) {
        act = 0x13;
        if (util_RandomU(0x400) + d[6] < 5000.0f)
            return 0;
    } else
        return 0;
    v = gm_BuildingKindValue(act);
    if (money < v * 0.4f)
        return 0;
    o1->kind = 4;
    o1->ref = act;
    o1->arg = 1;
    o1->arg2 = 0;
    o2->kind = 4;
    o2->arg2 = 0;
    o2->ref = y->id;
    o2->arg = 1;
    return 0x3c;
done:
    *o1 = s1;
    *o2 = s2;
    return r;
}
/* 0x49e680, execute callback 0x3c: upgrades ("upgr_kerker"), paid to 0x504a80(1) */

int ai_Unk49e680(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0)
        return 0;
    if (((unsigned char *)p)[0x166] != 0xf)
        return 0;
    if (s2->kind != 4 && s1->arg2 != 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.to = p->netId;
    m.unk0c = s2->ref;
    m.unk36 = 1;
    m.time = ai_TimeT;
    *(int *)&m.unk58 = s2->ref;
    cm_BeginGroup("upgr_kerker");
    cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_kerker");
    return 0x3c;
}
/* 0x49e7a0, decide callback 0x2f (characters with +0x166 == 0xa, from day 4 on, rich enough): work
   on a building of kind 3, else build type 9 / 10 (1 in 8), else what 0x4a5140 finds */

int ai_Unk49e7a0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *recA, *recB, *b, *rec;
    unsigned char *o;
    unsigned char k;
    char lvl;
    int cntA, cntB, i, money, r;
    AIARG s1;
    AIARG s2;

    recA = 0;
    recB = 0;
    cntA = 0;
    cntB = 0;
    if (ai_TimeT.year < 4 || f != 0 || ((unsigned char *)p)[0x166] != 0xa || a != 0)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = (unsigned char *)gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(*(int *)(o + 0x14), ai_OwnPlayer);
    if (money < (util_RandomU(3000) + 1000) * 32)
        return 0;
    rec = ai_BldsT;
    for (i = 0; i < 0x100; i++, rec++) {
        k = rec->type;
        if (k == 0 || ai_BTypesT[k].kind != 3 || (rec->flags5a & 4))
            continue;
        lvl = rec->level;
        if (lvl < util_RandomU(0x40) + 0x25) {
            s1.kind = 4;
            s1.ref = rec->id;
            s1.arg = 1;
            if (lvl < 0x21)
                s1.arg2 = 0x32;
            else
                s1.arg2 = lvl >= 0x42 ? 10 : 30;
            s2.arg = money;
            s2.ref = *(int *)(o + 2);
            s2.kind = 1;
            s2.arg2 = 1;
            r = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        }
        if (rec->type == 0xa)
            continue;
        if (rec->type == 9) {
            cntB++;
            if (!(rec->flags5a & 0x41))
                recB = rec;
        } else {
            cntA++;
            if (!(rec->flags5a & 0x41))
                recA = rec;
        }
    }
    if (util_RandomU(8) != 0)
        return 0;
    if (cntA != 0 && recA != 0) {
        o1->kind = 4;
        o1->ref = 9;
        o1->arg = 1;
        o2->kind = 4;
        o2->ref = recA->id;
        o2->arg = 1;
        return 0x2f;
    }
    if (cntB != 0 && recB != 0) {
        o1->kind = 4;
        o1->ref = 0xa;
        o1->arg = 1;
        o2->kind = 4;
        o2->ref = recB->id;
        o2->arg = 1;
        return 0x2f;
    }
    s1.ref = 8;
    s1.arg = 1;
    s2.kind = 0;
    s2.ref = 0;
    s2.arg = 0;
    if (ai_Unk4a5140(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x2f;
}
/* 0x49ea90, execute callback 0x2f: upgrades ("upgr_brunnen", s2->kind 4) or builds ("bau_brunnen"), paid to
   0x504a80(1); doubles need s1->unk10 (1..3) */

int ai_Unk49ea90(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;
    int n;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0 || ((unsigned char *)p)[0x166] != 0xa)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.unk0c = s2->ref;
        m.unk36 = 1;
        m.time = ai_TimeT;
        *(int *)&m.unk58 = s2->ref;
        cm_BeginGroup("upgr_brunnen");
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("upgr_brunnen");
    } else {
        cm_BeginGroup("bau_brunnen");
        if (ai_ExecBuildBuilding((AIPER *)ai_PlayerChar, s1, s2) == 0) {
            cm_EndGroup("bau_brunnen");
            return 0;
        }
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, gm_BuildingKindValue(s1->ref));
        cm_EndGroup("bau_brunnen");
    }
    n = s1->arg2;
    if (n > 0 && n < 4)
        G5_ACTS[0x2f].w3[n].w = G5_ACTS[0x2f].w1[0].w * 2.0f;
    return 0x2f;
}
/* 0x49ec10, eval callback 0x32: rated 6 / 2 times higher when 0x5044b0(p, 4) has more than two entries and
   0x4aaef0 finds something; else only from hour 20 on */

int ai_Unk49ec10(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    *a = -1e30f;
    *b = -1e30f;
    if (f != 0)
        return 0;
    q = gm_TitleJobI(p->id, 4);
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (ai_Unk4aaef0(p, &s1, &s2) == 0)
            return 0;
        G8_4A4AE0(a, b, i, p, 0, o1, o2);
        *o1 = s1;
        *o2 = s2;
        *a *= 6.0f;
        *b *= 2.0f;
        return 1;
    }
    if (ai_TimeT.hour < 0x14)
        return 0;
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
/* 0x49ed10, decide callback 0x32: what 0x4aaef0 finds (when 0x5044b0(p, 4) has more than two entries),
   else what 0x4ab080 finds */

int ai_Unk49ed10(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int q;

    q = gm_TitleJobI(p->id, 4);
    if (a != 0x32 && a != 0x25 && a != 0)
        return 0;
    if (f != 0)
        return 0;
    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    if (q != 0 && gm_TitlesLeft(q) > 2) {
        if (o1->kind == 4 && o2->kind == 1)
            return 0x32;
        if (ai_Unk4aaef0(p, &s1, &s2) != 0) {
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
/* 0x49ee20, execute callback */

int ai_Unk49ee20(G5CH *p, AIARG *s1, AIARG *s2)
{
    unsigned char k = s1->kind;

    if (k == 4 && s2->kind == 1) {
        if (ai_ExecMessage74(p, s1, s2) != 0)
            return 0x32;
    } else if (k == 0x13 && s2->kind == 4) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        if (ai_ExecBuyDunkleEcke(p, s1, s2) != 0)
            return 0x32;
    } else if (k == 0) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        return 0x32;
    }
    return 0;
}
/* 0x49eec0, decide callback 0x33 (also for 0x13 / 0x10 / 0x11): when fewer jobs (0x53e400 key 0x23) run than a
   random 0..9, picks one of the character's own buildings below its stock limit (hour-based) and a kind-5
   building of someone else, then the best of three offers of 0x50a250 */

int ai_Unk49eec0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int lim[4];
    G8OFFER rec[3];
    AIDEC d;
    AIBLD *own, *b;
    unsigned char k;
    int i, n, h, j, best;
    float m;

    lim[0] = ai_Unk5ed140[util_RandomU(0x10)];
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
    if (util_RandomU(10) < n + 1)
        return 0;
    if (a == 0x33) {
        h = ai_TimeT.hour - 8;
        if (h <= 0 || h >= 10)
            h = h > 0 ? 10 : 0;
        m = (10 - h) * 0.1f;
        m = m * m;
        lim[1] = (int)(m * 6400.0f);
        lim[2] = (int)(m * 12800.0f);
        lim[3] = (int)(m * 25600.0f);
        j = util_RandomU(0x100);
        for (i = 0x100;; ) {
            own = &ai_BldsT[j];
            k = own->type;
            if (k != 0 && own->owner == p->id && ((1 << ai_BTypesT[k].kind) & ((unsigned)&ai_Unk770d8c + 0x63604))
                && own->budget < lim[ai_BTypesT[k].cap247])
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        j = util_RandomU(0x100);
        for (i = 0x100;; ) {
            b = &ai_BldsT[j];
            k = b->type;
            if (k != 0 && b->owner != p->id && ai_BTypesT[k].kind == 5)
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        n = gm_Unk50a250(b, p, 3, rec);
        if (n <= 0)
            return 0;
        gv_GetTableRecord(0xe, &d);
        for (i = 0; i < n; i++)
            ((float *)lim)[i + 1] = rec[i].b * 0.14285715f * ((float)(d.cur - 2) / rec[i].c) * rec[i].a;
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
    j = util_RandomU(0x100);
    for (i = 0x100;; ) {
        b = &ai_BldsT[j];
        k = b->type;
        if (k != 0 && b->owner != p->id && ai_BTypesT[k].kind == 5)
            break;
        i--;
        j = (j + lim[0]) % 0x100;
        if (i == 0)
            return 0;
    }
    n = gm_Unk50a250(b, p, 3, rec);
    if (n <= 0)
        return 0;
    gv_GetTableRecord(0xe, &d);
    for (i = 0; i < n; i++) {
        if (o1->arg < (unsigned int)rec[i].a)
            ((float *)lim)[i + 1] = (float)(d.cur - 2) / rec[i].c * (rec[i].b * 0.14285715f) * rec[i].a;
        else
            ((float *)lim)[i + 1] = -1.0f;
    }
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

int ai_Unk49f370(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIBLD *a;
    AIBLD *b;
    int v;

    if (s1->kind != 4 || s2->kind != 0x14)
        return 0;
    G8_4FEA50((int *)&a, 0, 0, s1->ref);
    if (a == 0)
        return 0;
    G8_4FEA50((int *)&b, 0, 0, s1->arg);
    memset(&m, 0, sizeof(m));
    m.type = 0x23;
    m.to = ai_CharsT[a->owner].netId;
    m.unk0c = a->id;
    m.time = ai_TimeT;
    m.unk36 = 0x12;
    *(int *)&m.unk58 = a->id;
    m.from = p->netId;
    m.unk60 = s2->ref;
    m.unk64 = s2->arg;
    m.unk6c = s2->arg2;
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

int ai_Unk49f4f0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        if (util_RandomU(n - 1) == 0)
            return 0;
        return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
    }
    G8_4A4AE0(a, b, i, p, 0, o1, o2);
    *a += *a;
    *b *= 3.0f;
    return 1;
}
/* 0x49f5c0, decide callback 0x34: a kind-5 character (0x194 >= 4, money / 32 >= 5000, no job 0x24 of its own)
   builds a building of a random class (0x5043d0) it has fewer than 1..4 of and none of in its list */

int ai_Unk49f5c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int cnt[15];
    int own[15];
    unsigned char *q, *l;
    AIJOB *j;
    AIBLD *b;
    int money, k, n, r, c, x, y;

    q = (unsigned char *)gm_PlayerRecord((AICHAR *)p);
    if (((unsigned char *)p)[2] != 5 || *(int *)((unsigned char *)p + 0x194) < 4 || q == 0)
        return 0;
    money = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (money / 32 < 5000)
        return 0;
    for (j = he_FindEvent(1, 0, 0x24); j != 0; j = he_NextEvent()) {
        if (j->unkac == (int)p->netId)
            return 0;
    }
    gm_TitlesLeft(((unsigned char *)p)[0x164]);
    memset(cnt, 0, sizeof(cnt));
    l = q + 0x75;
    for (k = 0; k < 15; k++, l++)
        own[k] = gm_TitlesLeft(*l);
    for (b = ai_BldsT, k = 0; k < 0x100; k++, b++) {
        if (b->type != 0)
            cnt[gm_BuildingJob(ai_BTypesT[b->type].kind) - 1]++;
    }
    n = util_RandomU(4) + 1;
    r = util_RandomU(0x10);
    for (c = 0x10; c != 0; c--) {
        if (r != 0 && own[r - 1] == 0 && cnt[r - 1] < n)
            break;
        r = (r + 1) % 0x10;
    }
    if (c == 0 || r == -1)
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
    if (ai_Unk4a5140(a, p, f, &s1, &s2, 1) == 0)
        return 0;
    if (money < gm_BuildingKindValue(s1.ref))
        return 0;
    *o1 = s1;
    o1->arg2 = x;
    *o2 = s2;
    return 0x34;
}
/* 0x49f7f0, execute callback 0x34: builds what 0x49f5c0 chose (0x4a5320), sets flag 0x1000000 of the
   character's field 0x1c8 and records the building class s1->unk10 */

int ai_Unk49f7f0(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (ai_ExecBuildBuilding((AIPER *)p, s1, s2) == 0)
        return 0;
    cm_SendSetField(p->netId, 0x1c8, 4, 0x1000000, 0);
    ai_Unk4c59b0(p->netId, s1->arg2);
    gm_SendMoneyDelta(p->netId, 8);
    return 0x34;
}
/* 0x49f850, decide callback 0x35, the regulars' table ("stammtisch"): up to four kind-0x37 buildings of other
   owners with a table object 0x12d; a member leaves ('leav') when it likes the others too little, else joins
   ('join') a table with a free seat; 'noop' (kind 0) otherwise */

int ai_Unk49f850(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl[4];
    unsigned char *ob[4];
    int free[4];
    AIBLD *b;
    unsigned char *o;
    AICHAR *c;
    int found, step, i, j, n, k, m, v;
    float sum;

    found = -1;
    step = ai_Unk5ed140[util_RandomU(0x10)];
    if (*(int *)((unsigned char *)p + 0x1c8) & 0x2000000)
        return 0;
    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    n = 0;
    j = util_RandomU(0x100);
    for (i = 0x100; i != 0; ) {
        b = &ai_BldsT[j];
        if (b->type == 0x37 && b->owner != 0xffff && b->owner != p->id) {
            o = (unsigned char *)gm_Query(b->objList, 2, 6, 0, 0x12d);
            if (o != 0) {
                bl[n] = b;
                ob[n] = o;
                n++;
            }
        }
        i--;
        j = (j + step) % 0x100;
        if (n >= 4)
            break;
    }
    if (n == 0) {
        if (util_RandomF() > 0.33f)
            goto noop;
        return 0;
    }
    for (k = 0; k < n; k++) {
        free[k] = 0;
        for (m = 0; m < 4; m++) {
            v = ((int *)(ob[k] + 0x1c))[m];
            free[k] |= v == -1;
            if (found == -1)
                found = v == (int)p->netId ? k : -1;
        }
    }
    if (found != -1) {
        k = 0;
        sum = 0.0f;
        for (m = 0; m < 4; m++) {
            c = gm_FindChar(((int *)(ob[found] + 0x1c))[m]);
            if (c != 0 && c != (AICHAR *)p) {
                k++;
                sum = gm_Liking(p->id, *(unsigned short *)c, 1) + sum;
            }
        }
        if (k == 0 && util_RandomU(4) == 0) {
            o1->kind = 4;
            o1->ref = bl[found]->id;
            o1->arg = 'leav';
            return 0x35;
        }
        sum = sum / (k * 100);
        if (sum <= 0.666f)
            return 0;
        if (util_RandomF() + 0.166667f > sum)
            goto noop;
        o1->kind = 4;
        o1->ref = bl[found]->id;
        o1->arg = 'leav';
        return 0x35;
    }
    j = util_RandomU(n);
    for (i = n; i != 0; i--) {
        if (free[j] == 1)
            break;
        j = (j + 1) % n;
    }
    if (i == 0 || j == -1)
        return 0;
    if (util_RandomF() > 0.75f)
        goto noop;
    o1->kind = 4;
    o1->ref = bl[j]->id;
    o1->arg = 'join';
    o2->kind = 0x12;
    o2->ref = ai_CharsT[bl[j]->owner].netId;
    o2->arg = 0;
    return 0x35;
noop:
    o1->kind = 0;
    o1->ref = -1;
    o1->arg = 'noop';
    return 0x35;
}
/* 0x49fb80, execute callback 0x35: sends the join / leave request of 0x49f850 (0x4c5d80) and sets flag 0x2000000
   of the character's field 0x1c8 ('noop' only sets the flag) */

int ai_Unk49fb80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8TABLE r;

    if (s1->arg == 'join') {
        r.tag = s1->arg;
        r.bld = s1->ref;
        r.ch = p->netId;
        cm_BeginGroup("stammtisch join AI-Plr");
        ai_Unk4c5d80(&r);
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        cm_EndGroup("stammtisch join AI-Plr");
        return 0x35;
    }
    if (s1->arg == 'leav') {
        r.tag = s1->arg;
        r.bld = s1->ref;
        r.ch = p->netId;
        cm_BeginGroup("stammtisch leave AI-Plr");
        ai_Unk4c5d80(&r);
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        cm_EndGroup("stammtisch leave AI-Plr");
        return 0x35;
    }
    if (s1->arg == 'noop') {
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        return 0x35;
    }
    return 0;
}
/* 0x49fc90, decide callback: trade in the item group where the character's share of the workshops is lowest */

int ai_Unk49fc90(int a, G5CH *p, int f, AIARG *o1)
{
    G7MKT m[7];
    float score[7];

    unsigned int best[7];
    int all[7];
    int own[7];
    unsigned char *b;
    unsigned char *bt;
    unsigned int lo32;
    int hi32;
    int off, g, i, lo, hi, d;
    float v;

    if (a == 0 && f == 0 && amt_Unk4e94a0(p) == 1) {
        memset(own, 0, sizeof(own));
        memset(all, 0, sizeof(all));
        memset(best, 0, sizeof(best));
        for (off = 0; off < 0xa900; off += 0xa9) {
            b = (unsigned char *)ai_BldsT + off;
            if (b[0] != 0 && *(short *)(b + 0x27) != -1) {
                bt = (unsigned char *)ai_BTypesT + b[0] * 0x24d;
                lo32 = 0;
                hi32 = 0;
                if (bt[0] < 0x20)
                    lo32 = 1 << bt[0];
                else
                    hi32 = 1 << (bt[0] - 0x20);
                if ((lo32 & ((unsigned)&he_Events + 0x49640)) != 0 && (g = gm_BuildingJob(bt[0])) != 0 && g < 8) {
                    g--;
                    if (*(short *)(b + 0x27) == (short)p->id)
                        own[g] += bt[0x247];
                    all[g] += bt[0x247];
                    if (best[g] == 0)
                        best[g] = b[0];
                    else if (((unsigned char *)ai_BTypesT)[best[g] * 0x24d + 0x247] < bt[0x247])
                        best[g] = b[0];
                }
            }
        }
        hi = -1;
        lo = -1;
        for (i = 0; i < 7; i++) {
            score[i] = -1.0f;
            m[i].key = best[i];
            m[i].n = 0;
            if (all[i] > 14)
                all[i] = 15;
            if (own[i] > 14)
                own[i] = 15;
            if (all[i] > 0 && ai_Unk4a4680(&m[i]) != 0) {
                v = (16.0f - (float)own[i]) * 0.06666667f * (((float)all[i] + 1.0f - (float)own[i]) / (float)all[i]) * m[i].avg;
                score[i] = v;
                if (lo == -1 || v < score[lo])
                    lo = i;
                if (hi == -1 || score[hi] < v)
                    hi = i;
            }
        }
        if (lo != -1 && hi != -1 && lo != hi) {
            d = 0;
            if (m[lo].avg > 1.0f || own[lo] == 0) {
                if (m[hi].avg < 1.0f)
                    return 0;
                if (all[hi] <= own[hi] * 2 + 1)
                    return 0;
                d = -1;
            }
            if (m[hi].avg < 1.0f || all[hi] <= own[hi] * 2 + 1) {
                if (m[lo].avg <= 1.0f && own[lo] != 0) {
                    d = 1;
                    goto set;
                }
            } else {
                if (d == 0)
                    d = util_RandomU(4) - 1;
                if (d == -1 || d == 1) {
set:
                    o1->kind = 0x15;
                    o1->ref = lo + 1;
                    o1->arg2 = d;
                    return 0x36;
                }
            }
        }
    }
    return 0;
}
/* 0x4a0040, execute callback 0x36: hands the request (3, s1->unk04, s1->unk10) of s1 (kind 0x15) to 0x4e9660 */

int ai_Unk4a0040(G5CH *p, AIARG *s1, AIARG *s2)
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

int ai_Unk4a0090(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
    if (!ai_PickActionBuilding((AIPER *)p, 6, m.QuadPart, 0, (float)(util_RandomU(0x20) + 0x20), &b))
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

int ai_Unk4a01f0(G5CH *p, AIARG *s1, AIARG *s2)
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
/* 0x4a0240, the nearest partner of character p */

G5CH *ai_Unk4a0240(G5CH *p)
{
    G5CH *best = 0;
    G5CH *o;
    float d, bd;
    unsigned int n;
    int i;
    AIPER *list[5];
    char key[8];
    unsigned char info[16];
    char out[24];
    unsigned short id;
    int flags;

    if (((unsigned char *)p)[0x166] == 0) {
        amt_GetKindB(((unsigned char *)p)[0x167], key);
        for (o = G5_CHARS; o < G5_CHARS + 768; o++) {
            if (((unsigned char *)o)[0x166] != 0 && amt_FindOffice(o, key, out) != 0 && info[0] < 2) {
                if (best == 0) {
                    bd = gm_Liking(p->id, o->id, 1);
                    best = o;
                } else {
                    d = gm_Liking(p->id, o->id, 1);
                    if (d < bd) {
                        bd = d;
                        best = o;
                    }
                }
            }
        }
    } else {
        if (p->unk168 != 0 && (n = amt_GetOfficeApplicants((char)p->unk168, 5, list)) != 0) {
            for (i = 0; i < (int)n; i++) {
                o = (G5CH *)list[i];
                if (o != p) {
                    if (best == 0) {
                        bd = gm_Liking(p->id, o->id, 1);
                        best = o;
                    } else {
                        d = gm_Liking(p->id, o->id, 1);
                        if (d < bd) {
                            best = (G5CH *)list[i];
                            bd = d;
                        }
                    }
                }
            }
            if (best != 0 && bd <= (float)(int)n * 8.0f + 52.0f)
                return best;
        }
        best = 0;
        flags = 0xc00;
        if (ai_PickPartnerChain((AIPER *)p, 1, &flags, 0, 25.0f, &id) != 0)
            return &G5_CHARS[id];
    }
    return best;
}
/* 0x4a0410, a district for character p: few neighbours closer than 66 */

int ai_Unk4a0410(G5CH *p)
{
    int order[6];
    unsigned int k;
    struct { char pad[8]; unsigned int kind; char pad2[12]; } info;
    struct { int unk00; int id; char pad[16]; } list[6];
    float sum;
    int i, n, t, a, b;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (p->unk168 != 0)
        k = p->unk168;
    else if (((unsigned char *)p)[0x166] != 0)
        k = ((unsigned char *)p)[0x166];
    else if (((unsigned char *)p)[0x167] != 0)
        k = ((unsigned char *)p)[0x167];
    if (k != 0) {
        if (amt_GetKindB((unsigned char)k, &info) == 0)
            return 0;
        i = ((int)(info.kind & 0xff) - 1) / 3;
        if (i == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (i == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        a = util_RandomU(3);
        b = util_RandomU(3);
        t = order[a];
        order[a] = order[b];
        order[b] = t;
        a = util_RandomU(3);
        b = util_RandomU(3) + 3;
        t = order[a + 3];
        order[a + 3] = order[b];
        order[b] = t;
    }
    for (i = 0; i <= 4; i++) {
        n = amt_GetColumnOfficesOwned(order[i], 6, list);
        if (n > 2) {
            sum = 0.0f;
            for (t = 0; t < n; t++)
                sum += gm_Liking(p->id, ((AICHAR *)gm_FindChar(list[t].id))->id, 1);
            if (sum / (float)n < 66.0f)
                return order[i];
        }
    }
    return 0;
}
/* 0x4a05e0: one of the six quarters (1..6) whose residents (0x4e7040) like each other (mean of 0x50c550 both ways
   over 33), tried in an order that depends on the character's home (0x168 / 0x166 / 0x167, 0x4e6f80) and is
   shuffled within 1..3 and 4..6; 0 when none */

int ai_Unk4a05e0(G5CH *p)
{
    int order[6];
    unsigned char info[0x18];
    G8SEAT seat[6];
    AICHAR *c1, *c2;
    unsigned int k;
    int i, j, n, d, cnt, t, x, y;
    float sum;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (((unsigned char *)p)[0x168] != 0)
        k = ((unsigned char *)p)[0x168];
    else if (((unsigned char *)p)[0x166] != 0)
        k = ((unsigned char *)p)[0x166];
    else if (((unsigned char *)p)[0x167] != 0)
        k = ((unsigned char *)p)[0x167];
    if (k != 0) {
        if (amt_GetKindB((unsigned char)k, info) == 0)
            return 0;
        t = (info[8] - 1) / 3;
        if (t == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (t == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        x = util_RandomU(3);
        y = util_RandomU(3);
        t = order[x];
        order[x] = order[y];
        order[y] = t;
        x = util_RandomU(3) + 3;
        y = util_RandomU(3) + 3;
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
                    sum = gm_Liking(*(unsigned short *)c2, *(unsigned short *)c1, 1)
                        + (gm_Liking(*(unsigned short *)c1, *(unsigned short *)c2, 1) + sum);
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

int ai_Unk4a0890(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
        c = ai_Unk4a0240(p);
        if (c == 0)
            return 0;
        o1->kind = 7;
        o1->ref = c->netId;
        o1->arg2 = 1;
        return 0x38;
    }
    if (k == 0x20)
        n = ai_Unk4a0410(p);
    else if (k == 0x1f || k == 0x21)
        n = ai_Unk4a05e0(p);
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

int ai_Unk4a0940(G5CH *p, AIARG *s1, AIARG *s2)
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

int ai_Unk4a0990(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
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
    if (bl->level < util_RandomU(0x10) + 0x32) {
        G8_4A4AE0(a, b, i, p, f, o1, o2);
        *a *= 3.0f;
        *b *= 3.0f;
        return 1;
    }
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
/* 0x4a0a70, decide callback 0x39: the office building (kind 0x17..0x1a by the office 0x169): upgrade it (0x495a50
   kind 0xf), buy an item for it (0x4a5540) or build the next type; when the treasury (object 0x142) cannot pay,
   a credit (o1 kind 1, o2 kind 0x12) from the character's money over 80000 */

int ai_Unk4a0a70(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *bl;
    AIBTYPE *t;
    unsigned char *o;
    unsigned char k;
    char lvl;
    int money, need, amt, n, m, c;

    if (f != 0 || o1->kind != 0 || a != 0)
        return 0;
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
    o = (unsigned char *)gm_Query(bl->objList, 2, 6, 0, 0x142);
    money = gm_SumListAmounts(*(int *)(o + 0x14), ai_OwnPlayer);
    lvl = bl->level;
    if (lvl < util_RandomU(0x10) + 0x32 && !(bl->flags5a & 4)) {
        s1.kind = 4;
        s1.ref = bl->id;
        s1.arg = 1;
        if (lvl < 0x21)
            need = 0x32;
        else
            need = lvl >= 0x42 ? 10 : 30;
        s2.ref = *(int *)(o + 2);
        s2.arg2 = 1;
        s1.arg2 = need;
        s2.kind = 1;
        s2.arg = money;
        n = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
        if (n != 0) {
            *o1 = s1;
            *o2 = s2;
            return n;
        }
        n = ai_Unk4a4e30((AIBLDX *)bl, need);
        if (n > money) {
            amt = n;
            c = 1;
            goto credit;
        }
    }
    if (ai_Unk4a5540(&f, &a, bl, 0) != 0) {
        s1.kind = 2;
        s1.ref = f;
        s1.arg = 1;
        s1.ref2 = *(int *)(o + 2);
        s1.arg2 = 0x39;
        s2.arg2 = money;
        if (a != -1) {
            s2.kind = 1;
            s2.ref = a;
        } else {
            s2.kind = 4;
            s2.ref = bl->id;
        }
        s2.arg = 1;
        *(int *)&s2.unk14 = 0;
        amt = (int)gm_GoodsPrice(f, ai_OwnPlayer);
        c = 1;
        if (amt < money) {
credit:
            m = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
            if (m < 80000)
                return 0;
            amt += 0x20;
            if ((int)((m - 80000) * 0.25f) < amt)
                return 0;
            o1->kind = 1;
            o1->ref = *(int *)(o + 2);
            o1->arg2 = c;
            o2->arg = amt;
            o2->kind = 0x12;
            return 0x39;
        }
        n = ai_Unk495a50(0x39, p->id, 1, &s1, &s2);
        if (n == 0)
            return 0;
        *o1 = s1;
        *o2 = s2;
        return n;
    }
    t = &ai_BTypesT[bl->type];
    if (bl->flags5a & 0x45)
        return 0;
    if (t->cap247 >= ((unsigned char *)t)[0x248])
        return 0;
    n = bl->type + 1;
    amt = (int)(gm_BuildingKindValue(n) * 0.4f);
    if (amt < money) {
        o1->kind = 4;
        o1->ref = n;
        o1->arg = 1;
        o1->ref2 = bl->id;
        o2->kind = 1;
        o2->ref = *(int *)(o + 2);
        o2->arg = 1;
        return 0x39;
    }
    m = gm_CharCurrency((AIPER *)p, ai_OwnPlayer);
    if (m < 80000)
        return 0;
    amt += 0x20;
    if ((int)((m - 80000) * 0.5f) < amt)
        return 0;
    o1->kind = 1;
    o1->ref = *(int *)(o + 2);
    o1->arg2 = 1;
    o2->arg = amt;
    o2->kind = 0x12;
    return 0x39;
}
/* 0x4a0e60, execute callback 0x39: pays the credit (s1 kind 1, s2 kind 0x12) into the treasury s1->unk04, or
   upgrades the guild hall ("upgr_ai_zunfthaus", s1 kind 4, s2 kind 1) for 30 % of the price of type s1->unk04 */

int ai_Unk4a0e60(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *o;
    AIBLD *b;

    if (s1->kind == 1 && s2->kind == 0x12 && s2->arg > 0) {
        G8_4FEA50(0, (int)&o, 0, s1->ref);
        if (o == 0)
            return 0;
        cm_SendMoveGoods(*(int *)(o + 2), p->netId, ai_OwnPlayer, s2->arg);
        return 0x39;
    }
    if (s1->kind != 4 || s2->kind != 1)
        return 0;
    G8_4FEA50((int *)&b, 0, 0, s1->ref2);
    if (b == 0)
        return 0;
    G8_4FEA50(0, (int)&o, 0, s2->ref);
    if (o == 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.unk0c = s1->ref2;
    m.to = p->netId;
    m.unk36 = 1;
    m.time = ai_TimeT;
    *(int *)&m.unk58 = s1->ref2;
    cm_BeginGroup("upgr_ai_zunfthaus");
    cm_SendMoveGoods(-1, *(int *)(o + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->ref) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_ai_zunfthaus");
    return 0x39;
}
/* 0x4a1000, eval callback: not for kind-5 characters' partners of kind 5 (or busy, 0x47dd70); the oldest (0x0a) of
   the five children 0x68.. who is grown up (0x0a + 4 >= 0x20), unmarried (0x164) and has no job 0x5e; rated
   only when that child is old enough */

int ai_Unk4a1000(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    int m, w;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            if (((unsigned char *)c)[2] == 5)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0
            && (float)(c->unk00a + 4) >= *(float *)((unsigned char *)c + 0x20)
            && ((unsigned char *)c)[0x164] == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    *a = 0.0f;
    *b = 0.0f;
    ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
    w = (float)(best->unk00a + 4) >= *(float *)((unsigned char *)best + 0x20) ? 1 : 0;
    *a = w * *a;
    *b = w * *b;
    return 1;
}
/* 0x4a1170, decide callback 0x3a: the oldest grown-up unmarried child (as in 0x4a1000) gets a building class
   (o2 kind 0x17): a random one the dynasty has none of (0x506430 + 0x74, kind 5), the family's (0x504ad0) or
   a random 1..15; the character pays 1.5 % of 0x50a640 and needs six times that */

int ai_Unk4a1170(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    G5CH *c, *best;
    unsigned char *fin;
    unsigned char k;
    int m, price, q, r, n;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            k = ((unsigned char *)c)[2];
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0
            && (float)(c->unk00a + 4) >= *(float *)((unsigned char *)c + 0x20)
            && ((unsigned char *)c)[0x164] == 0 && he_FindEvent(2, 2, c->id, 0, 0x5e) == 0
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    price = (int)(gm_GetWorth(p->id) * 0.015f);
    if (gm_CharCurrency((AIPER *)p, ai_OwnPlayer) < price * 6)
        return 0;
    q = util_RandomU(0xf) + 1;
    if (((unsigned char *)p)[2] == 5) {
        fin = (unsigned char *)gm_PlayerRecord((AICHAR *)p);
        r = util_RandomU(0x10);
        for (n = 0x10; n != 0; n--) {
            if (r != 0 && fin[r + 0x74] == 0) {
                q = r;
                break;
            }
            r = (r + 1) % 0x10;
        }
    } else if (((unsigned char *)p)[0x164] != 0) {
        q = gm_TitleGroup(((unsigned char *)p)[0x164]);
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

int ai_Unk4a1360(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;

    if (s1->kind == 7 && s2->kind == 0x17) {
        m.to = s1->ref;
        m.unk36 = 0x12;
        *(int *)&m.unk58 = p->netId;
        m.type = 0x5e;
        m.unk0c = -1;
        *(unsigned char *)&m.from = (unsigned char)s2->ref;
        m.unk60 = s2->arg2;
        cm_SendLongMessage((AIMSG *)&m);
        return 0x3a;
    }
    return 0;
}
/* 0x4a13e0, decide callback 0x3b: the oldest married child (0x164, not 0x10000 in 0x1c8) gets one of three
   offers of 0x4948f0 (o2 kind 0xe), the best or the worst by the child's ratio 0x80.. to the family's 0x503ee0
   values; the character needs ten times the price */

int ai_Unk4a13e0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIX6 x;
    float s[3];
    G8OFFER rec[3];
    G5CH *c, *best;
    unsigned char k;
    int m, n;
    float va, vb;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            k = ((unsigned char *)c)[2];
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0 && ((unsigned char *)c)[0x164] != 0
            && !(*(int *)((unsigned char *)c + 0x1c8) & 0x10000)
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    G8_4948F0(p, rec);
    gm_Unk503ee0(&x, ((unsigned char *)best)[0x164]);
    for (n = 0; n < 3; n++) {
        va = max((float)x.v[rec[n].b], 1.0f);
        vb = max((float)x.v[rec[n].c], 1.0f);
        s[n] = ((unsigned char *)best)[0x80 + rec[n].c] / vb + ((unsigned char *)best)[0x80 + rec[n].b] / va;
    }
    if (util_RandomU(2) != 0) {
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
/* 0x4a1640, execute callback 0x3b: message 0x5f about the offer s2 (kind 0xe) for the child s1 (kind 7), to
   character 0; sets flag 0x10000 of its field 0x1c8 */

int ai_Unk4a1640(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AICHAR *c;

    if (s1->kind != 7 || s2->kind != 0xe)
        return 0;
    c = gm_FindChar(0);
    if (c == 0)
        return 0;
    m.unk36 = 0x12;
    m.type = 0x5f;
    m.to = ((G5CH *)c)->netId;
    ((unsigned char *)&m)[0x64] = (unsigned char)s2->ref;
    *(int *)&m.unk58 = p->netId;
    m.unk0c = -1;
    ((unsigned char *)&m)[0x65] = (unsigned char)s2->arg2;
    m.unk60 = s1->arg2;
    cm_SendLongMessage((AIMSG *)&m);
    cm_SendSetField(((G5CH *)c)->netId, 0x1c8, 4, 0x10000, 0);
    return 0x3b;
}
/* 0x4a16f0, registers the actions 0x01..0x3b (eval / decide / execute callbacks of this run and of am.c) */

int ai_Unk4a16f0(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x1;
    strcpy(act.name, "DummyUnversehrtheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496030;
    act.exec = ai_AlwaysTrue;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2;
    strcpy(act.name, "DummyBeruf");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496050;
    act.exec = ai_Unk496070;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3;
    strcpy(act.name, "DummyVergnuegen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496080;
    act.exec = ai_Unk4960a0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x4;
    strcpy(act.name, "Bildung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4960b0;
    act.exec = ai_Unk4965b0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x5;
    strcpy(act.name, "Bildung (Beruf)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496610;
    act.exec = ai_Unk496ab0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x6;
    strcpy(act.name, "DummyRechtschaffenheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk495fa0;
    act.exec = ai_Unk495fc0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x7;
    strcpy(act.name, "DummySicherheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk495fd0;
    act.exec = ai_Unk495ff0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x8;
    strcpy(act.name, "DummyFortpflanzung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496000;
    act.exec = ai_Unk496020;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x9;
    strcpy(act.name, "Bestechung");
    act.eval = ai_Unk496b10;
    act.decide = ai_Unk496e40;
    act.exec = ai_Unk4972d0;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xa;
    strcpy(act.name, "Amtsbewerbung");
    act.eval = ai_Unk4973f0;
    act.decide = ai_Unk497490;
    act.exec = ai_Unk497630;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xb;
    strcpy(act.name, "Objekt kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497660;
    act.exec = ai_Unk4979d0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xd;
    strcpy(act.name, "Objekt kaufen (AP)");
    act.eval = ai_Unk4a49a0;
    act.decide = sv_ReturnZero;
    act.exec = sv_ReturnZero;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xe;
    strcpy(act.name, "Raum kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497af0;
    act.exec = ai_Unk497c80;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xf;
    strcpy(act.name, "Repair Building");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497cf0;
    act.exec = ai_Unk497ee0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x10;
    strcpy(act.name, "Wohnsitz kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497fa0;
    act.exec = ai_Unk498470;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x11;
    strcpy(act.name, "Wohnsitz bauen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4984d0;
    act.exec = ai_Unk498a80;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x12;
    strcpy(act.name, "Wohnsitz upgraden (Credits)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk498b80;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x13;
    strcpy(act.name, "Betrieb bauen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk498c70;
    act.exec = ai_Unk499290;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x14;
    strcpy(act.name, "Betrieb upgraden (AP)");
    act.eval = ai_Unk4a49a0;
    act.decide = sv_ReturnZero;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x15;
    strcpy(act.name, "Betrieb upgraden (Credits)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk499400;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x16;
    strcpy(act.name, "Medicus ausfsuchen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk499520;
    act.exec = ai_Unk499580;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x17;
    strcpy(act.name, "Sabotieren");
    act.eval = ai_Unk4995d0;
    act.decide = ai_Unk499660;
    act.exec = ai_Unk499a90;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x18;
    strcpy(act.name, "Verpruegeln");
    act.eval = ai_Unk499b80;
    act.decide = ai_Unk499c10;
    act.exec = ai_Unk499f20;
    act.flags = 0x1c9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x19;
    strcpy(act.name, "Buergerrecht");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a020;
    act.exec = ai_Unk49a0a0;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1a;
    strcpy(act.name, "ApsSparen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a290;
    act.exec = ai_Unk49a380;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1b;
    strcpy(act.name, "UseObject (Military)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a390;
    act.exec = ai_Unk49a5b0;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1c;
    strcpy(act.name, "UseObject (Ansehen)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a690;
    act.exec = ai_Unk49aa80;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1d;
    strcpy(act.name, "UseObject (APS)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49ab60;
    act.exec = ai_Unk49ad80;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1e;
    strcpy(act.name, "UseObject (Bildung)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49ae50;
    act.exec = ai_Unk49b050;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1f;
    strcpy(act.name, "UseObject (Unversehrtheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b120;
    act.exec = ai_Unk49b2b0;
    act.flags = 0x3f;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x20;
    strcpy(act.name, "UseObject (Dynastie)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b380;
    act.exec = ai_Unk49b6f0;
    act.flags = 0x17;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x21;
    strcpy(act.name, "UseObject (Gemeinheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b7c0;
    act.exec = ai_Unk49bc50;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x22;
    strcpy(act.name, "UseObject (Sicherheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49bd90;
    act.exec = ai_Unk49c000;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x23;
    strcpy(act.name, "Use Parfuem");
    act.eval = ai_Unk49c0d0;
    act.decide = ai_Unk49c170;
    act.exec = ai_Unk49c190;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x24;
    strcpy(act.name, "Use Trank");
    act.eval = ai_Unk49c320;
    act.decide = ai_Unk49c3c0;
    act.exec = ai_Unk49c3e0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x25;
    strcpy(act.name, "Spionage");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c570;
    act.exec = ai_Unk49c700;
    act.flags = 0x69;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xc;
    strcpy(act.name, "Objekt Kaufen (Markt)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c790;
    act.exec = ai_Unk49c880;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x26;
    strcpy(act.name, "Ablass Kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c8e0;
    act.exec = ai_Unk49cb60;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x27;
    strcpy(act.name, "UseAmt");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49cd30;
    act.exec = ai_Unk49cdb0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x28;
    strcpy(act.name, "Drohen");
    act.eval = ai_Unk49cdd0;
    act.decide = ai_Unk49ce50;
    act.exec = ai_Unk49d030;
    act.flags = 0x5d;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x29;
    strcpy(act.name, "Pamphlete");
    act.eval = ai_Unk49d050;
    act.decide = ai_Unk49d0f0;
    act.exec = ai_Unk49d280;
    act.flags = 0x5d;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2a;
    strcpy(act.name, "Anschwaerzen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49d2d0;
    act.exec = ai_Unk49d460;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2b;
    strcpy(act.name, "Beleidigen");
    act.eval = ai_Unk49d4b0;
    act.decide = ai_Unk49d550;
    act.exec = ai_Unk49d5e0;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2c;
    strcpy(act.name, "Erpressen");
    act.eval = ai_Unk49d600;
    act.decide = ai_Unk49d6a0;
    act.exec = ai_Unk49d730;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2d;
    strcpy(act.name, "UsePrivs");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49d750;
    act.exec = ai_Unk49d7d0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2e;
    strcpy(act.name, "Buergermeister");
    act.eval = ai_Unk49d7f0;
    act.decide = ai_Unk49d8d0;
    act.exec = ai_Unk49dc10;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2f;
    strcpy(act.name, "Baumeister");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49e7a0;
    act.exec = ai_Unk49ea90;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x30;
    strcpy(act.name, "Buergermeister upgrade");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49dd90;
    act.exec = sv_ReturnZero;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x31;
    strcpy(act.name, "Buergermeister Rathaus");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49def0;
    act.exec = ai_Unk49e230;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3c;
    strcpy(act.name, "Buergermeister Kerker");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49e350;
    act.exec = ai_Unk49e680;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x32;
    strcpy(act.name, "Dunkle Ecke");
    act.eval = ai_Unk49ec10;
    act.decide = ai_Unk49ed10;
    act.exec = ai_Unk49ee20;
    act.flags = 0x69;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x33;
    strcpy(act.name, "Kredit Nehmen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49eec0;
    act.exec = ai_Unk49f370;
    act.flags = 0x7;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x34;
    strcpy(act.name, "Change Profession");
    act.eval = ai_Unk49f4f0;
    act.decide = ai_Unk49f5c0;
    act.exec = ai_Unk49f7f0;
    act.flags = 0x7;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x35;
    strcpy(act.name, "Stammtisch");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49f850;
    act.exec = ai_Unk49fb80;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x36;
    strcpy(act.name, "Zunft Preismanipulation");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49fc90;
    act.exec = ai_Unk4a0040;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x37;
    strcpy(act.name, "Zunft Aufruhr");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a0090;
    act.exec = ai_Unk4a01f0;
    act.flags = 0x43;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x38;
    strcpy(act.name, "Zunft Level 3");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a0890;
    act.exec = ai_Unk4a0940;
    act.flags = 0x4b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x39;
    strcpy(act.name, "Upgrade Zunft");
    act.eval = ai_Unk4a0990;
    act.decide = ai_Unk4a0a70;
    act.exec = ai_Unk4a0e60;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3a;
    strcpy(act.name, "Berufsausbildung");
    act.eval = ai_Unk4a1000;
    act.decide = ai_Unk4a1170;
    act.exec = ai_Unk4a1360;
    act.flags = 0x1f;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3b;
    strcpy(act.name, "Studium");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a13e0;
    act.exec = ai_Unk4a1640;
    act.flags = 0x13;
    return ai_Unk495520(&act, a) != 0;
}
/* 0x4a3a90, the next entry of the ring */

G6RING *ai_Unk4a3a90(void)
{
    G6RING *r = ai_Unk60034c;

    if (ai_Unk6f070c == 0)
        return ai_Unkf83c20;
    ai_Unk60034c = &ai_Unkf83c20[(unsigned int)(ai_Unk60034c - ai_Unkf83c20 + ai_Unk600350) % ai_Unk6f070c];
    return r;
}

/* one district's characters into the ring: the list of district k, entries passing cond, with flags f */
#define G7_ADD(k, f, cond)                                                          \
    n = amt_GetColumnOfficesOwned((k), 6, &info);                                                \
    for (j = 0; j < n; j++) {                                                       \
        if (r > ai_Unkf83c20 + 31)                                                  \
            break;                                                                  \
        if ((cond) && (o = (G5CH *)gm_FindChar(info.e[j].id)) != 0 && o != p) {    \
            r->flags = (f);                                                         \
            r->dist = gm_Liking(o->id, p->id, 1);                                \
            r->who = o;                                                             \
            cnt++;                                                                  \
            r++;                                                                    \
        }                                                                           \
    }

/* 0x4a3af0, fills the ring 0xf83c20 with the characters near character p (district, family, same group) */
void ai_FillNearRing(AICHAR *c)
{
    G5CH *p = (G5CH *)c;
    struct { int unk00; struct { int id; unsigned char k; char pad[19]; } e[6]; } info;
    int kind[2];
    int dist[3];
    int k2, k3, k4;
    unsigned char k1;
    int cnt, n, j, i, g, step;
    unsigned int f;
    G5CH *o;
    AIPER *fam[6];
    G6RING *r;
    int *sz;

    cnt = 0;
    ai_Unk6f070c = 0;
    memset(ai_Unkf83c20, 0, 32 * sizeof(G6RING));
    r = ai_Unkf83c20;
    if (((unsigned char *)p)[0x166] == 0) {
        G7_ADD(1, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
        G7_ADD(2, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
        G7_ADD(3, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
    } else {
        k1 = 0;
        k2 = 0;
        k3 = 0;
        k4 = 0;
        dist[0] = 0;
        dist[1] = 0;
        dist[2] = 0;
        g = amt_GetKindLevel(((unsigned char *)p)[0x166]);
        amt_GetKindB(((unsigned char *)p)[0x166], kind);
        if (kind[1] >= 1 && kind[1] <= 3) {
            k2 = 3;
            if (kind[1] == 1) {
                k1 = 2;
            } else {
                k1 = 1;
                if (kind[1] != 2)
                    k2 = 2;
            }
            if (g == 3) {
                k3 = 4;
                k4 = 5;
            }
        } else if (kind[1] >= 4 && kind[1] <= 5) {
            k1 = (kind[1] == 4) + 4;
            if (g == 6) {
                k3 = 6;
            } else if (g == 4) {
                dist[0] = 1;
                dist[1] = 2;
                dist[2] = 3;
            }
        } else if (g == 7) {
            dist[0] = 4;
            dist[1] = 5;
        }
        G7_ADD(kind[1], 0x10, 1)
        G7_ADD(k1, 1, 1)
        G7_ADD(k2, 1, 1)
        if (g == 3 || g == 6) {
            G7_ADD(k3, 0x40, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(k4, 0x40, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
        }
        if (g == 4 || g == 7) {
            G7_ADD(dist[0], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(dist[1], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(dist[2], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
        }
    }
    if (p->unk168 != 0) {
        n = amt_GetOfficeApplicants((char)p->unk168, 6, fam);
        for (j = 0; j < n; j++) {
            if ((G5CH *)fam[j] != p) {
                for (i = 0; i < cnt; i++) {
                    if (ai_Unkf83c20[i].who == (G5CH *)fam[j]) {
                        ai_Unkf83c20[i].flags |= 2;
                        break;
                    }
                }
                if (i < cnt)
                    continue;
                if (r <= ai_Unkf83c20 + 31) {
                    r->flags = 2;
                    r->dist = gm_Liking(o->id, p->id, 1);
                    cnt++;
                    r->who = (G5CH *)fam[j];
                    r++;
                }
            }
        }
    }
    if (p->unk168 != 0 || ((unsigned char *)p)[2] == 5) {
        for (o = G5_CHARS; o < G5_CHARS + 768; o++) {
            f = 0;
            if (o != p && o->id != 0xffff) {
                k1 = ((unsigned char *)o)[2];
                if ((k1 == 6 || k1 == 7 || k1 == 5) && ((unsigned char *)p)[2] == 5)
                    f = 1;
                if (((unsigned char *)o)[0x168] == p->unk168)
                    f |= 4;
                if (f != 0) {
                    for (i = 0; i < cnt; i++) {
                        if (ai_Unkf83c20[i].who == o) {
                            ai_Unkf83c20[i].flags |= f;
                            break;
                        }
                    }
                    if (i < cnt)
                        continue;
                    if (r <= ai_Unkf83c20 + 31) {
                        r->flags = f;
                        r->dist = gm_Liking(o->id, p->id, 1);
                        r->who = o;
                        cnt++;
                        r++;
                    }
                }
            }
        }
    }
    step = (4 - ai_Difficulty) * 5;
    for (i = 0; i < cnt; i++) {
        k1 = ((unsigned char *)ai_Unkf83c20[i].who)[2];
        if (k1 == 6 || k1 == 7)
            ai_Unkf83c20[i].dist = (float)step + ai_Unkf83c20[i].dist;
    }
    ai_Unk6f070c = cnt;
    if (cnt < 4) {
        ai_Unk60034c = ai_Unkf83c20;
        ai_Unk600350 = 1;
        return;
    }
    g = 11;
    for (sz = &ai_Unk5ed1a8; g != 0; sz--, g--) {
        if (*sz < cnt)
            break;
    }
    ai_Unk60034c = &ai_Unkf83c20[util_RandomU(cnt)];
    ai_Unk600350 = ai_Unk5ed180[util_RandomU(g)];
    if (ai_Unk600350 > 1 && ai_Unk6f070c % ai_Unk600350 == 0)
        ai_Unk600350 += (util_RandomU(2) != 0 ? 2 : 0) - 1;
}
/* 0x4a4680, the current prices of the items of group m->key against their base prices */

int ai_Unk4a4680(G7MKT *m)
{
    G7GRP *g;
    int i, k, n;
    float sum, r;

    i = 0;
    for (g = ai_Unk61a348; g->key != m->key; g++) {
        i++;
        if (g + 1 > ai_Unk61a348 + 0x25)
            return 0;
    }
    if (i > 0x24)
        return 0;
    n = 0;
    for (k = 0; k < 10; k++) {
        if (ai_Unk61a348[i].items[k] != 0)
            m->items[n++] = ai_Unk61a348[i].items[k];
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

int ai_Unk4a47c0(char *s)
{
    char buf[128];

    if (_strcmpi(s, "APS") == 0)
        return 0;
    if (_strcmpi(s, "UNVERSEHRTHEIT") == 0)
        return 1;
    if (_strcmpi(s, "WOHNUNG") == 0)
        return 2;
    if (_strcmpi(s, "GELD") == 0)
        return 3;
    if (_strcmpi(s, "BERUF") == 0)
        return 4;
    if (_strcmpi(s, "VERGNUEGEN") == 0)
        return 5;
    if (_strcmpi(s, "ANSEHEN") == 0)
        return 6;
    if (_strcmpi(s, "AMT") == 0)
        return 7;
    if (_strcmpi(s, "BILDUNG") == 0)
        return 8;
    if (_strcmpi(s, "RECHTSCHAFFENHEIT") == 0)
        return 9;
    if (_strcmpi(s, "GEMEINHEIT") == 0)
        return 10;
    if (_strcmpi(s, "SICHERHEIT") == 0)
        return 11;
    if (_strcmpi(s, "FORTPFLANZUNG") == 0)
        return 12;
    if (_strcmpi(s, "TRAEGHEIT") == 0)
        return 13;
    sprintf(buf, "amt_GetDesireByName(): Unbekanntes Bed\xfc" "rfnis: %s", s);
    return -1;
}
/* 0x4a49a0, eval callback: the action's weights against the moods */

int ai_Unk4a49a0(float *a, float *b, int i, AIPER *p, int f, void *x, void *y)
{
    G5ACT *act = &G5_ACTS[i];
    float d1, d2, d;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    if ((act->flags & G7_MASK(p)) == 0) {
        d1 = 50.0f;
        d2 = 100.0f;
        found = 0;
    } else {
        d1 = 40.0f;
        d2 = 50.0f;
        found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        d = d2;
        if (act->w1[k].k == p->sel)
            d = d1;
        *a = act->w1[k].w / d * p->mood[act->w1[k].k][2] + *a;
        d = d2;
        if (act->w2[k].k == p->sel)
            d = d1;
        *b = act->w2[k].w / d * p->mood[act->w2[k].k][2] + *b;
    }
    return found;
}
/* 0x4a4ae0, eval callback: only the weights of the current need count */

int ai_Unk4a4ae0(float *a, float *b, int i, AIPER *p)
{
    G5ACT *act = &G5_ACTS[i];
    float d1;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    if ((act->flags & G7_MASK(p)) == 0) {
        d1 = 50.0f;
        found = 0;
    } else {
        d1 = 40.0f;
        found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
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

int ai_Unk4a4c10(float *a, float *b, int i, AIPER *p, AIDEC *dec)
{
    G5ACT *act;
    float v, rank, d1, d2, t;
    int n, k, found;
    unsigned char c;

    if (dec == 0)
        v = 1.0f;
    else
        v = (float)dec->guilt;
    n = gv_CulpritGuilt(p->netId);
    rank = p->mood[11][0];
    *a = 0.0f;
    *b = 0.0f;
    act = &G5_ACTS[i];
    rank = (rank - 2.0f) * ((float)n + v);
    if ((act->flags & G7_MASK(p)) == 0) {
        d1 = 50.0f;
        d2 = 100.0f;
        found = 0;
    } else {
        d1 = 40.0f;
        d2 = 50.0f;
        found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        c = act->w1[k].k;
        if (act->w1[k].w <= 0.0f)
            t = act->w1[k].w / (c == p->sel ? d1 : d2) * p->mood[c][2];
        else if (c == p->sel)
            t = p->mood[c][2] * act->w1[k].w / (d1 + rank);
        else
            t = p->mood[c][2] * act->w1[k].w / (d2 + rank);
        *a = t + *a;
        c = act->w2[k].k;
        if (act->w2[k].w <= 0.0f)
            t = act->w2[k].w / (c == p->sel ? d1 : d2) * p->mood[c][2];
        else if (c == p->sel)
            t = p->mood[c][2] * act->w2[k].w / (d1 + rank);
        else
            t = p->mood[c][2] * act->w2[k].w / (d2 + rank);
        *b = t + *b;
    }
    return found;
}
/* 0x4a4e30, a building's value for a stake of k (10 / 30 / 50) */

int ai_Unk4a4e30(AIBLD *b, int k)
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
/* 0x4a4e70, picks one of the scene objects listed under name: the nearest (with noise) to the character's
   building or the current object, else a random one */

int ai_Unk4a4e70(AIPER *p, int name)
{
    int scene = d3_CurrentScene;
    char buf[64];
    int list[256];
    float o[6];
    float q[6];
    float dx, dy, dz, d, best;
    int n, i, r, obj;
    unsigned int k;

    d3if_SelectScene(0, 1);
    if (ai_BuildingName(name, buf) != 0) {
        memset(list, 0, sizeof(list));
        n = ai_FindBauplaetze(buf, list);
        if (n != 0) {
            if (p->bld2 != 0 && *(int *)((char *)p->bld2 + 0x61) != 0 && util_RandomU(3) != 0) {
                obj = *(int *)((char *)p->bld2 + 0x61);
                d3_PointToWorld((void *)obj, (char *)obj + 0x4c, o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((void *)list[i], (char *)list[i] + 0x4c, q);
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
            if (ai_MarketBuilding != 0 && (obj = *(int *)((char *)ai_MarketBuilding + 0x61)) != 0) {
                d3_PointToWorld((void *)obj, (char *)obj + 0x4c, o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((void *)list[i], (char *)list[i] + 0x4c, q);
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
   the one it has (0x170) and pass 0x4f81c0 (2 or 4) for types 4..7; other types ask 0x5020e0. With f, s2 becomes
   the plot (kind 0xc) that 0x4a4e70 finds */

int ai_Unk4a5140(int a, AIPER *p, int b, AIARG *s1, AIARG *s2, int f)
{
    int q[0x88];
    unsigned char *h;
    int k, r;

    memset(q, 0, sizeof(q));
    q[0] = 1;
    if (s1->kind != 5)
        return 0;
    k = s1->ref;
    if (ai_BTypesT[k].kind == 2) {
        h = *(unsigned char **)((unsigned char *)p + 0x170);
        if (h != 0 && ai_BTypesT[k].cap247 <= ai_BTypesT[*h].cap247 && ai_BTypesT[*h].kind == 2)
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
    } else if (unk_Unk5020e0(k, *(unsigned short *)p) == 0) {
        return 0;
    }
    if (f == 0)
        return 1;
    r = ai_Unk4a4e70(p, s1->ref);
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
    int plot, alt;

    if (s1->kind != 5)
        return 0;
    if (s2->kind != 0xc)
        return 0;
    plot = s2->ref;
    strcpy(name, "gb_");
    strcat(name, ai_BTypesT[s1->ref].pad001);
    _strupr(name + 2);
    alt = 0;
    for (;;) {
        if (alt)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, name, alt + '@');
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, name);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0)
            break;
        if (alt) {
            sprintf(name, "amt_ExecGebaeudeBauen(): Keine Group f\xfcr %s gefunden", ai_BTypesT[s1->ref].pad001);
            return 0;
        }
        alt = 1;
    }
    sprintf((char *)&m + 0x9c, "%s", path);
    m.time = ai_TimeT;
    m.to = *(int *)((unsigned char *)p + 4);
    m.arg[1] = s1->ref;
    m.type = 0xd;
    m.flags = 2;
    m.fromId = m.to;
    m.arg[0] = m.to;
    m.arg[2] = gm_Unk509dc0(p, m.arg[1]);
    ai_ObjectPosition((char *)&m + 0x80, plot);
    ai_ObjectAngle((char *)&m + 0x90, plot);
    cm_SendLongMessage(&m);
    return 1;
}
/* 0x4a5540: the first good of building type b->type that b's store does not hold yet while it holds both of its
   two ingredients (slot 0x40 + 2i, 0x41 + 2i); goods of kind 2 / 6 need a free cart (list 4) or one carrying it.
   *item gets the good, *obj the cart's id (-1 for none) */

int ai_Unk4a5540(int *item, int *obj, AIBLD *b, int f)
{
    AIBTYPE *t;
    unsigned short *s;
    short *in;
    AIOBJ *o, *cart;
    int list, n, i, found;
    char k;

    list = b->objList;
    t = &ai_BTypesT[b->type];
    cart = 0;
    i = 0;
    if (t->nslots == 0)
        return 0;
    in = (short *)&t->slot[0x41];
    s = t->slot;
    do {
        k = ai_GoodsT[*s & 0x7fff].type;
        if (k != '!') {
            if (k == 2 || k == 6) {
                n = 0;
                found = 0;
                cart = 0;
                for (o = gm_Query(b->objList, 1, 4, 2); o != 0; o = gm_QueryNext()) {
                    if (o->id != 0xfd)
                        n++;
                    if (o->id == (*s & 0x7fff)) {
                        list = *(int *)((unsigned short *)o + 10);
                        found = 1;
                        cart = o;
                        break;
                    }
                }
                if (n < (unsigned char)t->pad001[0x20] && !found) {
                    for (o = gm_Query(b->objList, 1, 4, 6); o != 0; o = gm_QueryNext()) {
                        if (o->id == (*s & 0x7fff)) {
                            list = *(int *)((unsigned short *)o + 10);
                            found = 1;
                            cart = o;
                            break;
                        }
                    }
                    if (n < (unsigned char)t->pad001[0x20] && !found)
                        goto check;
                }
            } else {
check:
                if (gm_Query(list, 1, 0, *s & 0x7fff) == 0
                    && (in[-1] == 0 || gm_Query(list, 1, 0, in[-1]) != 0)
                    && (in[0] == 0 || gm_Query(list, 1, 0, in[0]) != 0)) {
                    *item = t->slot[i] & 0x7fff;
                    if (cart != 0) {
                        *obj = *(int *)((unsigned short *)cart + 1);
                        return 1;
                    }
                    *obj = -1;
                    return 1;
                }
            }
        }
        in += 2;
        i++;
        s++;
    } while (i < t->nslots);
    return 0;
}
/* 0x4a5760: for n wanted items rec[i].item, finds them in the character's inventory list (0x178, next at 0x3f;
   goods of kind 9 skipped): rec[i].unk08 the entry's id, rec[i].unk0c the entry, rec[i].unk04 its 0x0e value when
   usable (0x5ba620 class matches 0x1ec or 0x5ba6b0 refuses), else -1. st[0] counts the entries, st[1] the
   unusable, st[2] the usable ones */

void ai_Unk4a5760(G5CH *p, int n, G9REC *rec, int *st)
{
    unsigned short *e;
    G9REC *r;
    unsigned char *c;
    int i, v;

    if (n > 0) {
        for (r = rec, i = n; i != 0; i--, r++) {
            r->unk08 = -1;
            r->unk04 = 0;
        }
    }
    st[1] = 0;
    st[2] = 0;
    st[0] = 0;
    for (e = *(unsigned short **)((unsigned char *)p + 0x178); e != 0; e = *(unsigned short **)((unsigned char *)e + 0x3f)) {
        if (ai_GoodsT[*e].type == 9)
            continue;
        st[0]++;
        for (i = 0, r = rec; i < n; i++, r++) {
            if (*e != (unsigned int)r->item)
                continue;
            c = (unsigned char *)G8_5BA620(*e);
            if (c == 0) {
                v = -1;
                r = &rec[i];
                r->unk04 = -1;
                st[1]++;
                r->unk0c = 0;
            } else {
                if (!(*(unsigned int *)(c + 0x10) & *(unsigned int *)((unsigned char *)p + 0x1ec))
                    && ai_InventoryHas(p, *e) == 0) {
                    st[2]++;
                    rec[i].unk04 = *(int *)(e + 7);
                    rec[i].unk08 = *(int *)(e + 1);
                    rec[i].unk0c = (int)e;
                    break;
                }
                r = &rec[i];
                r->unk04 = -1;
                st[1]++;
                v = *(int *)(e + 1);
                r->unk0c = (int)e;
            }
            r->unk08 = v;
            break;
        }
    }
    for (; n > 0; n--, rec++) {
        if (rec->unk08 == -1 && (c = (unsigned char *)G8_5BA620(rec->item)) != 0
            && (*(unsigned int *)(c + 0x10) & *(unsigned int *)((unsigned char *)p + 0x1ec)))
            st[1]++;
    }
}
/* 0x4a58d0: picks a partner chain of a (< 4) characters for an action with the flags flags[i]: the leader
   ai_Partner first when 0x5c33c0 accepts it, then for each further slot the next ring entry (0x4a3a90) whose
   rating lies in [b, f] and that 0x5c33c0 accepts with the chain so far; ids into id[] */

int ai_PickPartnerChain(AIPER *p, int a, int *flags, int b, float f, unsigned short *id)
{
    unsigned int q[0x14];
    G6RING *e;
    unsigned int i, n;
    int found, ret;

    ret = 0;
    if ((unsigned int)a < 4) {
        memset(q, 0, sizeof(q));
        i = 0;
        memset(&q[7], 0xff, 8 * 4);
        q[5] = 2;
        if (ai_Partner != 0 && !(q[1] & 0x10000000)) {
            q[16] = 0;
            q[1] = flags[0];
            if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
                i = 1;
                id[0] = *(unsigned short *)ai_Partner;
            }
        }
        for (; i < (unsigned int)a; i++) {
            q[1] = flags[i];
            if (i == 0)
                *(unsigned short *)&q[7] = *(unsigned short *)p;
            else
                ((unsigned short *)&q[7])[i] = id[i - 1];
            found = 0;
            e = ai_Unk4a3a90();
            for (n = ai_Unk6f070c; !found; n--, e = ai_Unk4a3a90()) {
                if (n == 0) {
                    if (!found)
                        return 0;
                    break;
                }
                if (e->who == 0)
                    return 0;
                if (*(float *)&b <= e->dist && e->dist <= f
                    && ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)e->who, q, 0) != 0) {
                    found = 1;
                    id[i] = *(unsigned short *)e->who;
                }
            }
        }
        ret = 1;
    }
    return ret;
}
/* 0x4a5a30: one partner for an action with the flags fl and the chain ids[n]: the leader ai_Partner when
   0x5c33c0 accepts it, else the first ring entry (0x4a3a90) rated in [lo, hi] that it accepts; into *id */

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
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            *id = *(unsigned short *)ai_Partner;
            return 1;
        }
    }
    q[16] = 0;
    for (k = ai_Unk6f070c; ; k--) {
        e = ai_Unk4a3a90();
        if (k == 0 || e->who == 0)
            break;
        if (lo <= e->dist && e->dist <= hi
            && ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)e->who, q, 0) != 0) {
            *id = *(unsigned short *)e->who;
            return 1;
        }
    }
    return 0;
}
/* 0x4a5b50: as 0x4a58d0, but the partners come from a random walk over all 768 characters (steps 0x5ed100);
   the rating window [lo, hi] (0x50c550) only counts when it is narrower than 0..100 */

int ai_Unk4a5b50(AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win;
    float r;

    step = ai_Unk5ed100[util_RandomU(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = flags[0];
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            i = 1;
            id[0] = *(unsigned short *)ai_Partner;
        }
    }
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    for (; i < a; i++) {
        q[1] = flags[i];
        if (i == 0)
            *(unsigned short *)&q[7] = *(unsigned short *)p;
        else
            ((unsigned short *)&q[7])[i] = id[i - 1];
        n = 0x300;
        found = 0;
        c = util_RandomU(0x300);
        while (!found) {
            if (n == 0) {
                if (!found)
                    return 0;
                break;
            }
            if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0) {
                if (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi)) {
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
/* 0x4a5d30: as 0x4a5a30, but the partner comes from a random walk over all 768 characters (as 0x4a5b50) */

int ai_Unk4a5d30(AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int len, c;
    int step, k, found, win;
    float r;

    step = ai_Unk5ed100[util_RandomU(0x10)];
    memset(q, 0, sizeof(q));
    win = 0;
    len = n * 2;
    if (len > 0x20)
        len = 0x20;
    memcpy(&q[7], ids, len);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[16] = 0;
        q[1] = fl;
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            *id = *(unsigned short *)ai_Partner;
            return 1;
        }
    }
    q[1] = fl;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    k = 0x300;
    found = 0;
    c = util_RandomU(0x300);
    while (!found) {
        if (k == 0) {
            if (found)
                return 1;
            return 0;
        }
        if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0) {
            if (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi)) {
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
   (no kinds 8 / 7 / 5 / 9 / 0x14; first the married ones (0x164 set, not 'R'), then the unmarried), accepted by
   0x5c33c0, of a rank (0x505f30) within 5 of p's and, when the window is narrower than 0..100, rated in
   [lo, hi]; ids into out[], returns the number */

unsigned int ai_FindCandidates(AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out)
{
    unsigned int q[0x14];
    unsigned char *c;
    unsigned int cnt, i;
    int step, k, rank, win;
    char t;
    float r;

    step = ai_Unk5ed100[util_RandomU(0x10)];
    memset(q, 0, sizeof(q));
    memset(&q[7], 0xff, 8 * 4);
    win = 0;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    q[5] = 2;
    q[1] = fl;
    rank = gm_Unk505f30(*(unsigned short *)p, 0);
    k = 0x300;
    cnt = 0;
    i = util_RandomU(0x300);
    if (n != 0) {
        do {
            if (k == 0)
                break;
            c = (unsigned char *)&ai_CharsT[i];
            t = c[2];
            if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && c[0x164] != 0 && c[0x164] != 'R'
                && ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)i, q, 0) != 0
                && abs(rank - gm_Unk505f30(i, 0)) < 6
                && (!win || (r = gm_Liking(i, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
                cnt++;
                out[cnt - 1] = (unsigned short)i;
            }
            k--;
            i = (int)(i + step) % 0x300;
        } while (cnt < n);
    }
    i = util_RandomU(0x300);
    for (k = 0x300; cnt < n && k != 0; k--) {
        c = (unsigned char *)&ai_CharsT[i];
        t = c[2];
        if (t != 8 && t != 7 && t != 5 && t != 9 && t != 0x14 && c[0x164] == 0
            && ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)i, q, 0) != 0
            && abs(rank - gm_Unk505f30(i, 0)) < 6
            && (!win || (r = gm_Liking(i, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
            cnt++;
            out[cnt - 1] = (unsigned short)i;
        }
        i = (int)(step + i) % 0x300;
    }
    return cnt;
}
/* 0x4a61d0: as 0x4a5b50 with one flag set fl for every slot, and the partners' quarter (0x166, 0x4e7c40) at most
   d away from p's; returns the number found */

unsigned int ai_PickPartnerChainLevel(AIPER *p, unsigned int a, unsigned int fl, float lo, float hi, int d, unsigned short *id)
{
    unsigned int q[0x14];
    unsigned int i, c;
    int step, n, found, win, home;
    float r;

    step = ai_Unk5ed100[util_RandomU(0x10)];
    i = 0;
    memset(q, 0, sizeof(q));
    win = 0;
    memset(&q[7], 0xff, 8 * 4);
    q[5] = 2;
    if (ai_Partner != 0) {
        q[1] = fl;
        q[16] = 0;
        if (ai_Unk5c33c0(*(unsigned short *)p, *(unsigned short *)ai_Partner, q, 0) != 0) {
            i = 1;
            id[0] = *(unsigned short *)ai_Partner;
        }
    }
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    home = amt_GetKindLevel(((unsigned char *)p)[0x166]);
    for (; i < a; i++) {
        q[1] = fl;
        if (i == 0)
            *(unsigned short *)&q[7] = *(unsigned short *)p;
        else
            ((unsigned short *)&q[7])[i] = id[i - 1];
        n = 0x300;
        found = 0;
        c = util_RandomU(0x300);
        while (!found) {
            if (n == 0) {
                if (!found)
                    return i;
                break;
            }
            if (ai_Unk5c33c0(*(unsigned short *)p, (unsigned short)c, q, 0) != 0
                && (!win || (r = gm_Liking(c, *(unsigned short *)p, 1), lo <= r && r <= hi))
                && abs(home - amt_GetKindLevel(((unsigned char *)&ai_CharsT[c])[0x166])) <= d) {
                found = 1;
                id[i] = (unsigned short)c;
            }
            n--;
            c = (int)(c + step) % 0x300;
        }
    }
    return i;
}
/* 0x4a63f0: a building for p's action a: a random walk over the 256 buildings for one that 0x5c37a0 accepts
   with the class mask (bit 63 forced) and, when the window is narrower than 0..100, whose owner p rates
   (0x50c550) in [lo, hi]; its id into *out (-1 for none). am.h declares lo as int (c) */

int ai_PickActionBuilding(AIPER *p, int a, __int64 mask, int c, float f, int *out)
{
    unsigned int q[0x12];
    AIBLD *b;
    int step, n, found, win;
    unsigned int j;
    float r;

    step = ai_Unk5ed140[util_RandomU(0x10)];
    q[2] = (unsigned int)mask;
    q[4] = a;
    found = 0;
    q[3] = (unsigned int)(mask >> 32) | 0x80000000;
    win = 0;
    q[5] = 0;
    *out = -1;
    if ((*(float *)&c > 0.0f || f < 100.0f) && *(float *)&c < f)
        win = 1;
    n = 0x100;
    j = util_RandomU(0x100);
    while (!found) {
        if (n == 0) {
            if (found)
                return 1;
            return 0;
        }
        b = &ai_BldsT[j];
        if (b->type != 0 && ai_Unk5c37a0(p, q, b) != 0
            && (!win || (r = gm_Liking(b->owner, *(unsigned short *)p, 1), *(float *)&c <= r && r <= f))) {
            found = 1;
            *out = b->id;
        }
        n--;
        j = (j + step) % 0x100;
    }
    return 1;
}
/* 0x4a6570: as 0x4a63f0, but collects up to n buildings into out[]; returns the number */

int ai_Unk4a6570(AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out)
{
    unsigned int q[0x12];
    AIBLD *b;
    int step, k, cnt, win;
    unsigned int j;
    float r;

    step = ai_Unk5ed140[util_RandomU(0x10)];
    cnt = 0;
    q[4] = a;
    q[3] = (unsigned int)(mask >> 32) | 0x80000000;
    win = 0;
    q[5] = 0;
    q[2] = (unsigned int)mask;
    if ((lo > 0.0f || hi < 100.0f) && lo < hi)
        win = 1;
    k = 0x100;
    j = util_RandomU(0x100);
    if (n != 0) {
        do {
            if (k == 0)
                return cnt;
            b = &ai_BldsT[j];
            if (b->type != 0 && ai_Unk5c37a0(p, q, b) != 0
                && (!win || (r = gm_Liking(b->owner, *(unsigned short *)p, 1), lo <= r && r <= hi))) {
                out[cnt] = b->id;
                cnt++;
            }
            k--;
            j = (j + step) % 0x100;
        } while (cnt < n);
    }
    return cnt;
}
/* 0x4a66f0: the positions of two buildings' models (0x61) in scene 0: a (raised by 0.8 of its height), b; -1
   when one is missing or they are the same */

int ai_Unk4a66f0(AIBLD *x, float *a, AIBLD *y, float *b)
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
/* 0x4a67a0: aims p's cannon in building b at building t: the cannon type r (0x4ef170 / 0x4ef000, p's 0x194 must
   reach its level), a ballistic solution (0x5dc1a0 / 0x5dc210 / 0x5dd100) within -15..45 degrees, its strength
   (0x4eef50) within the charge limit of the cannon and its cost (0x4ef090) within a quarter of the money; the
   shot goes into ai_Unk6f0698 */

int ai_Unk4a67a0(AIPER *p, AIBLD *b, AIBLD *t)
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

    ai_Unk6f0698.who = -1;
    ai_Unk6f0698.bld = -1;
    cn = gv_FindEffect(b->id);
    if (cn == 0)
        return 0;
    if (ai_Unk4a66f0(b, pa, t, pb) != 0)
        return 0;
    gv_BuildingCannon(b, &r2, &r);
    r = util_RandomU(r + 1);
    gv_CannonKind(r, &kind);
    if (*(int *)((unsigned char *)p + 0x194) < kind.level)
        return 0;
    f = unk_Unk4ef330(b);
    r2 = util_RandomU(r2 + 1);
    ai_SphereVolume(ai_Unk5eee08, ai_Unk5edfe0[r].c, par);
    ai_PositionShare(ai_Unk5edfe0[r].a, f * ai_Unk5edfe0[r].b, par, v94);
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
    if ((int)(gm_SumListAmounts(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer) * 0.25f) < cost)
        return 0;
    ai_Unk6f0698.a[0] = pa[0];
    ai_Unk6f0698.a[1] = pa[1];
    ai_Unk6f0698.a[2] = pa[2];
    ai_Unk6f0698.b[0] = vb[0];
    ai_Unk6f0698.b[1] = vb[1];
    ai_Unk6f0698.b[2] = vb[2];
    ai_Unk6f0698.c[0] = vc[0];
    ai_Unk6f0698.c[1] = vc[1];
    ai_Unk6f0698.c[2] = vc[2];
    ai_Unk6f0698.d[0] = vd[0];
    ai_Unk6f0698.d[1] = vd[1];
    ai_Unk6f0698.d[2] = vd[2];
    ai_Unk6f0698.who = *(int *)((unsigned char *)p + 4);
    ai_Unk6f0698.bld = b->id;
    ai_Unk6f0698.cost = cost;
    ai_Unk6f0698.charge = r2 + 1;
    ai_Unk6f0698.x = x;
    ai_Unk6f0698.ang = ang;
    ai_Unk6f0698.kind = r;
    ai_Unk6f0698.k = k;
    memcpy(ai_Unk6f0698.par, par, sizeof(par));
    return 1;
}
/* 0x4a6ae0, execute callback of the cannon shot: fires the 1..3 balls the decide callback 0x4a67a0 planned
   into ai_Unk6f0698 (one ball from 50 units along the barrel, two or three side by side from 40) */

int ai_Unk4a6ae0(AIPER *p, AIBLD *b)
{
    float u, v;
    float pa[3], pb[3], pc[3];
    float f, c, g, s;
    float x0, x1, x2;
    char s1[104], s2[104], s3[104];
    int cost, x;

    if (ai_Unk6f0698.who == *(int *)((unsigned char *)p + 4) && ai_Unk6f0698.bld == b->id) {
        gv_CannonScatter(b, &u, &v);
        g = unk_Unk4ef330(b);
        cost = ai_Unk6f0698.cost;
        x = ai_Unk6f0698.x;
        c = ai_Unk5edfe0[ai_Unk6f0698.kind].c;
        f = ai_Unk6f0698.k * 0.01f * (g * ai_Unk5edfe0[ai_Unk6f0698.kind].b - ai_Unk5edfe0[ai_Unk6f0698.kind].a)
            + ai_Unk5edfe0[ai_Unk6f0698.kind].a;
        if (ai_Unk6f0698.charge == 1) {
            pa[0] = ai_Unk6f0698.a[0] + ai_Unk6f0698.b[0] * 50.0f;
            pa[1] = ai_Unk6f0698.a[1] + ai_Unk6f0698.b[1] * 50.0f;
            pa[2] = ai_Unk6f0698.a[2] + ai_Unk6f0698.b[2] * 50.0f;
            ai_InitShot(s1, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            gv_FireShots(b, p, s1, 0, 0, gv_SalvoCharge(ai_Unk6f0698.kind, ai_Unk6f0698.k, 1), x, cost);
            return 1;
        }
        if (ai_Unk6f0698.charge == 2) {
            x0 = ai_Unk6f0698.b[0] * 40.0f + ai_Unk6f0698.a[0];
            x1 = ai_Unk6f0698.a[1] + ai_Unk6f0698.b[1] * 40.0f;
            x2 = ai_Unk6f0698.b[2] * 40.0f + ai_Unk6f0698.a[2];
            pa[0] = ai_Unk6f0698.d[0] * 5.0f + x0;
            pa[1] = ai_Unk6f0698.d[1] * 5.0f + x1;
            pa[2] = ai_Unk6f0698.d[2] * 5.0f + x2;
            pb[0] = ai_Unk6f0698.d[0] * -5.0f + x0;
            pb[1] = ai_Unk6f0698.d[1] * -5.0f + x1;
            pb[2] = ai_Unk6f0698.d[2] * -5.0f + x2;
            ai_InitShot(s2, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s1, pb, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s2, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            gv_FireShots(b, p, s2, s1, 0, gv_SalvoCharge(ai_Unk6f0698.kind, ai_Unk6f0698.k, 2), x, cost);
            return 1;
        }
        if (ai_Unk6f0698.charge == 3) {
            x0 = ai_Unk6f0698.b[0] * 40.0f + ai_Unk6f0698.a[0];
            x1 = ai_Unk6f0698.b[1] * 40.0f + ai_Unk6f0698.a[1];
            x2 = ai_Unk6f0698.b[2] * 40.0f + ai_Unk6f0698.a[2];
            pc[0] = ai_Unk6f0698.c[0] * 5.0f + x0;
            pc[1] = ai_Unk6f0698.c[1] * 5.0f + x1;
            pc[2] = ai_Unk6f0698.c[2] * 5.0f + x2;
            s = ai_Unk6f0698.c[0] * -3.0f;
            pa[0] = ai_Unk6f0698.d[0] * 5.0f + x0 + s;
            pa[1] = ai_Unk6f0698.d[1] * 5.0f + ai_Unk6f0698.c[1] * -3.0f + x1;
            pa[2] = ai_Unk6f0698.d[2] * 5.0f + ai_Unk6f0698.c[2] * -3.0f + x2;
            pb[0] = ai_Unk6f0698.d[0] * -5.0f + s + x0;
            pb[1] = ai_Unk6f0698.d[1] * -5.0f + ai_Unk6f0698.c[1] * -3.0f + x1;
            pb[2] = ai_Unk6f0698.d[2] * -5.0f + ai_Unk6f0698.c[2] * -3.0f + x2;
            ai_InitShot(s3, pa, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s1, pb, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_InitShot(s2, pc, ai_Unk6f0698.b, f, c, ai_Unk5eee08);
            ai_ScatterShot(s3, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s1, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            ai_ScatterShot(s2, u, ai_Unk6f0698.d, u, ai_Unk6f0698.c, v);
            gv_FireShots(b, p, s3, s1, s2, gv_SalvoCharge(ai_Unk6f0698.kind, ai_Unk6f0698.k, 3), x, cost);
            return 1;
        }
    }
    return 0;
}

/* ---- out of address order: 0x495a50 belongs between 0x4957d0 and 0x495ce0. Defined above its callers, the
   draft's register use changes 0x496e40 / 0x499660 (matched) and 0x496610 / 0x499c10; move it back once it
   matches (layout 2026-10-22). ---- */

/* 0x495a50, rates the actions that lead to action kind for character id (recursion guarded by the stack) */
int ai_Unk495a50(int kind, int id, int c, AIARG *o1, AIARG *o2)
{
    float best1, best2, a, b;
    int k, r, r1, r2;
    AIARG s1, s2, b1s1, b2s1, b1s2, b2s2;
    G5ACT *act;
    G5CH *p;

    best1 = -1e30f;
    best2 = -1e30f;
    r1 = 0;
    r2 = 0;
    b1s1.kind = 0;
    b2s1.kind = 0;
    b1s2.kind = 0;
    b2s2.kind = 0;
    p = &G5_CHARS[id];
    if (ai_Unk4957a0(kind) != 0)
        return 0;
    if (p->mask == 0)
        p->mask = 1;
    k = 0;
    for (act = G5_ACTS; act < G5_ACTS + 0x49; act++, k++) {
        if (act->id != 0 && act->id != p->act && ai_Unk4957a0(act->id) == 0) {
            ai_Unk495770(act->id);
            s1 = *o1;
            s2 = *o2;
            if (act->eval != 0)
                act->eval(&a, &b, k, p, c, &s1, &s2);
            if ((best1 <= a || best2 <= b) && act->decide != 0) {
                r = act->decide(kind, p, c, &s1, &s2);
                if (r != 0) {
                    if (best1 <= a) {
                        b1s1 = s1;
                        best1 = a;
                        b1s2 = s2;
                        r1 = r;
                    }
                    if (best2 <= b) {
                        best2 = b;
                        b2s1 = s1;
                        b2s2 = s2;
                        r2 = r;
                    }
                }
            }
            ai_Unk495790();
        }
    }
    if (best1 == 0.0f || best1 < best2) {
        *o1 = b2s1;
        *o2 = b2s2;
        return r2;
    }
    *o1 = b1s1;
    *o2 = b1s2;
    return r1;
}
/* unlinked: the "Use Seance" decide callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_Unk490c10 points */
int ai_SeanceDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    return 0;
}
/* unlinked: the "Use Seance" execute callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_Unk490c10 points */
int ai_SeanceExec(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0;
}
