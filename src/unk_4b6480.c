/* unk_4b6480.c -- message callbacks: each one writes text 0x12e7 + type for a character and sends it (0x53dda0);
   called through the table ai_Unk602570 by 0x4bb6c0. Its .data (the table) follows unk_4b39f0.c's literals; unit
   start and name are guesses (no module tag; could be the missions' tail). Names are ours. Ends before 0x4bb810,
   anm.c's first user of "anm:animals".
   Open: 0x4bb720 (posts random message types as AIMSG 0x42) / 0x4bb7d0 (LCG, seed 0x6f0730) may be anm.c's.
   unit of rea 8cef7d5b (src/game/unk_4b6480.c); boundaries in decl/text.dcl */
#include <string.h>
#include <windows.h>
#include "unk_4b6480.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "he.h"
#include "hst.h"
#include "io_text.h"
#include "main.h"
#include "sv.h"
#include "tr.h"
#include "unk_58bbd0.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define he_MessageBoxQuickjumpI ((void (*)(int, int, char *, int, int, int, int, int, int))he_MessageBoxQuickjump)  /* 0x53dee0 (he.c: char fl): @original this unit builds the flag as an int (byte-matched ai_Unk4baa80 / ai_Unk4baba0) */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_Unk4b86c0) */
#define cm_SendExSetHEI ((void (*)(int, void *))cm_SendExSetHE)  /* 0x4c4c60 (cm.h: unsigned char k): @original the callers push the key as an int (byte-matched ai_Unk4b8c80) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_ExecuteBuyRoom / ai_ExecuteBuyMarketObject) */
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_600c00[];
extern GAPDATA full_data_600c01[];

/* 0x4b6480, message 0: c gets a reward of 1..3 % of 0x50a640 */

int ai_MsgRewardPct1to3(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100 ? 0x100 < strlen(buf) : 0, 0x603, 0);
    return 0;
}
/* 0x4b6570, message 1: as 0x4b6480 (2..4 %), not for c->officeKind == 0x0e */

int ai_MsgRewardPct2to4(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0x0e)
        return 0x400;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6660, message 2: c gets 2..5 % of the worth of the first character (kind 3 / 4) that applied for an
   open office (state 3, applicants taken) of c's office column; the text gets the office's 0x253 + kind.
   The goto labels give the original's block layout (the shared 0x400 exit before the found body). */

int ai_MsgOfficeApplicantPayment(G14MSG *m)
{
    AICHAR *c, *p;
    int i, j, n, v;
    double k;
    G14REC r[6];
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0)
        goto none;
    n = amt_KindColumn(c->officeKind);
    if (n == 0 || n >= 7)
        goto none;
    n = amt_GetColumnOffices(n, 6, r);
    if (n == 0)
        goto none;
    for (i = 0, p = 0; i < n; i++) {
        if (p != 0)
            goto found;
        if (r[i].state == 3 && r[i].cnt != 0) {
            for (j = 0; j < 768; j++) {
                if (ai_CharsT[j].appliedOffice == r[i].kind && (ai_CharsT[j].kind == 3 || ai_CharsT[j].kind == 4)) {
                    p = &ai_CharsT[j];
                    break;
                }
            }
            if (p != 0)
                goto found;
        }
    }
none:
    return 0x400;
found:
    k = util_RandomU(4) + 2.0;
    v = (int)(gm_GetWorth(p->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, p->id, c->id, r[i].kind + 0x253, v);
    cm_SendMoveGoodsUnchecked(m->id, p->netId, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6830, message: c pays 1..3 % of 0x50a640 (not for officeKind 0x0e) */

int ai_MsgPayPct1to3(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0x0e)
        return 0x400;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6920, message: c pays 1..4 % (not if 0x504ad0 of jobId is 7) */

int ai_MsgPayPct1to4(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (gm_TitleGroup(c->jobId) == 7)
        return 0x400;
    k = util_RandomU(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6a20, message 5: 2..5 %, with a number 3..8 in the text */

int ai_MsgRewardPct2to5(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(4) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, util_RandomU(6) + 3, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6b10, message 6 and 8: 1..3 %, the same body as message 0 (the two table entries share it) */

int ai_MsgRewardPct1to3Type68(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6bf0, message: one of up to 12 buildings of the 0x501190(1, 5, 0x16) list is chosen (0x501190(1, 1, id)),
   c pays 2..6 % */

int ai_MsgPayForBuilding(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, v;
    double k;
    int ids[12];
    char buf[0x400];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 5, 0x16); b != 0 && n < 12; b = gm_BuildNext())
        ids[n++] = b->id;
    if (n == 0)
        return 0x400;
    gm_BuildQuery(1, 1, ids[util_RandomU(n)]);
    k = util_RandomU(5) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6d30, message: c pays 2..4 % */

int ai_MsgPayPct2to4(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6e10, message 10: c pays (1..2) * 1.25 % */

int ai_MsgPayPct125(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(2) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.0125);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b6ef0, message 11: c pays (2..4) / 120 */

int ai_MsgPayPctOver120(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.008333333333333333);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}

void ai_MsgNoOp(struct AICHAR *c);

/* 0x4b6fd0, message: c pays 1..3 % (not for officeKind 0x0e; with item 0x290 only 0x4b70f0, which does nothing) */
int ai_MsgPayPct1to3Type0c(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0x0e)
        return 0x400;
    if (ai_InventoryHas(c, 0x290)) {
        ai_MsgNoOp(c);
        return 0x400;
    }
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b70f0, does nothing */

void ai_MsgNoOp(AICHAR *c)
{
}
/* 0x4b7100, message 0x0d: c pays 2..4 % of its worth (0x50a640), skipped with item 0x290 */

int ai_MsgPayPct2to4Type0d(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(3) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7200, message 0x0e: c pays 1..2 % of its worth (0x50a640), skipped with item 0x290 and when the amount is above the worth */

int ai_MsgPayPct1to2Type0e(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(2) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    if (v > gm_GetWorth(c->id))
        return 0;
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7310, message 0x0f: c pays 1..4 % of its worth (0x50a640), skipped with item 0x290 and for title group 7 (gm_TitleGroup of jobId) */

int ai_MsgPayPct1to4Type0f(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (gm_TitleGroup(c->jobId) == 7)
        return 0x400;
    k = util_RandomU(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7430, message 0x10: c pays 1..4 % of its worth (0x50a640) when it holds an office (officeKind or secondOffice), skipped with item 0x290 */

int ai_MsgPayPct1to4OfficeType10(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0 && c->secondOffice == 0)
        return 0x400;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7550, message 0x11: c pays 1..3 % of its worth (0x50a640), with the building ai_PickActionBuilding finds (kind
   bit 0x400000) and its gm_FindById result in the text; skipped with item 0x290 and without a building */

int ai_MsgPayPct1to3BuildingType11(G14MSG *m)
{
    AICHAR *c;
    double k;
    LARGE_INTEGER mask;
    int v, r, o;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    mask.LowPart = 1 << 22;  /* 0x400000: the building kind bit of the filter */
    mask.HighPart = 0;
    if (!ai_PickActionBuilding((AIPER *)c, 6, mask.QuadPart, 0, 100.0f, &o))
        return 0x400;
    if (!AI_4FEA50_4(&r, 0, 0, o))
        return 0x400;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, r, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b76b0, message: v = 1..2 % of 0x50a640 (not with item 0x290; types 0x12 and 0x13) */

int ai_MsgPayPct1to2Type1213(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(2) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b77b0, message: v = 1..3 %, a number 2..16 in the text (not with item 0x290) */

int ai_Unk4b77b0(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, util_RandomU(0xf) + 2, v);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b78c0, message: a character p found by 0x4a5b50 (flags 0x600c00, 0..100) gets 0x4c4b00 4..10 from c */

int ai_Unk4b78c0(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    fl = (unsigned)&full_data_600c00;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_RandomU(4) * 2 + 4;
    cm_SendRelations(c->netId, p->netId, n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b79c0, message: as 0x4b78c0 (33..100, 6..14) */

int ai_Unk4b79c0(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    fl = (unsigned)&full_data_600c00;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 33.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_RandomU(5) * 2 + 6;
    cm_SendRelations(c->netId, p->netId, n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7ac0, message: as 0x4b78c0 (33..100, 6..18) */

int ai_Unk4b7ac0(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    fl = (unsigned)&full_data_600c01;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 33.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_RandomU(7) * 2 + 6;
    cm_SendRelations(c->netId, p->netId, n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b7bc0, message: the holders of up to 16 buildings of the 0x501190(1, 5, 7) list (each once) get 0x4c4b00
   6..12 from c; v = 2..5 % of 0x50a640 (not with item 0x290) */

int ai_Unk4b7bc0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, i, j, r, v;
    double k;
    unsigned int ids[16];
    char buf[0x400];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 5, 7); b != 0 && n < 16; b = gm_BuildNext()) {
        if (*(unsigned short *)((char *)b + 0x27) != 0xffff)
            ids[n++] = ai_CharsT[*(unsigned short *)((char *)b + 0x27)].netId;
    }
    if (n == 0)
        return 0x400;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ids[i] == ids[j])
                ids[j] = -1;
        }
    }
    for (i = 0; i < n; i++) {
        if (ids[i] != -1) {
            r = util_RandomU(4) * 2 + 6;
            cm_SendRelations(c->netId, ids[i], r, 1);
        }
    }
    k = util_RandomU(4) + 2.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
/* 0x4b7d80, message for c with secondOffice 0x1e..0x21: every character (not kind 6 / 7) of the same 0x504ad0 group gets
   0x4c4b00 1..8 from c; v = 1..4 % of 0x50a640, text 0x2a3 + secondOffice - 0x1e (not with item 0x290) */

int ai_Unk4b7d80(G14MSG *m)
{
    AICHAR *c;
    int g, i, s, v;
    double k;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->secondOffice >= 0x1e && c->secondOffice <= 0x21 && (g = gm_TitleGroup(c->jobId)) != 0
        && !ai_InventoryHas(c, 0x290)) {
        for (i = 0; i < 768; i++) {
            if (ai_CharsT[i].kind != 6 && ai_CharsT[i].kind != 7 && g == gm_TitleGroup(ai_CharsT[i].jobId))
                cm_SendRelations(c->netId, ai_CharsT[i].netId, util_RandomU(8) + 1, 1);
        }
        k = util_RandomU(4) + 1.0;
        v = (int)(gm_GetWorth(c->id) * k * 0.01);
        cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
        if (c->secondOffice == 0x1e)
            s = 0;
        else if (c->secondOffice == 0x1f)
            s = 1;
        else if (c->secondOffice == 0x20)
            s = 2;
        else if (c->secondOffice == 0x21)
            s = 3;
        else
            return 0x400;
        txt_Format(buf, m->type + 0x12e7, s + 0x2a3, v);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        return 0;
    }
    return 0x400;
}
/* 0x4b7f40, message: as 0x4b78c0, negative (0..100, -6..-14; not with item 0x290) */

int ai_Unk4b7f40(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    fl = (unsigned)&full_data_600c00;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_RandomU(5) * 2 + 6;
    cm_SendRelations(c->netId, p->netId, -n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b8060, message: as 0x4b78c0, negative (0..100, -10..-38; not with item 0x290) */

int ai_Unk4b8060(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    fl = (unsigned)&full_data_600c00;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    if (p == 0)
        return 0x400;
    n = util_RandomU(15) * 2 + 10;
    cm_SendRelations(c->netId, p->netId, -n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b81a0, message: as 0x4b78c0, negative (0..100, -4..-18; not with item 0x290) */

int ai_Unk4b81a0(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    fl = (unsigned)&full_data_600c00;
    if (!ai_FindPartnersRandomWalk((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_RandomU(8) * 2 + 4;
    cm_SendRelations(c->netId, p->netId, -n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b82c0, message: one of c's buildings (0x501190(1, 3, id), up to 8 of kind 1 / 2) and one of up to 6 characters
   working there (title set) are chosen; half the time a text and 0x4fca40 of p by -(0.1 .. 0.4 of p's 0x1c float)
   (not with item 0x290) */

int ai_Unk4b82c0(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, x, y;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k != 0)
        p = a[util_RandomU(k)];
    if (b == 0 || p == 0)
        return 0x400;
    if (util_RandomU(2)) {
        txt_Format(buf, m->type + 0x12e7, b, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title, p->id);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        x = (int)(*(float *)((char *)p + 0x1c) * 0.3);
        y = (int)(*(float *)((char *)p + 0x1c) * 0.1);
        gm_AddHealthS(p->id, -(util_RandomU(x) + y));
    }
    return 0;
}
/* 0x4b84b0, message: as 0x4b82c0, the worker chosen from more than 2; 0x4fca40 of p by -(p's 0x1c float + 1) (not
   with item 0x290)
   @deep behaviour draft (5 -> 1 close after the sibling idiom): 10 differing instructions in one hunk. The 0x230/0x20d ternary spelling of its sibling ai_Unk4b82c0 is applied and took 54 -> 11; the rest is the while (util_Random(k) == s) test: the original pushes k (push edi) before loading p = a[s] and merges the cleanup of the previous util_Random argument into add esp,0x8 after the test call, ours does add esp,4 / and esi / mov ebp / push edi / call. Read: the loop, s, p and the two calls are the same statements (p = a[s] before the loop matches the original's load order), no meaning error. spell.py: no hunk matches the menu ('no menu rule; meaning check first'), the fill hunk is only the tail. Pure argument-push scheduling/instruction order - match-deep's scheduling work. */

int ai_Unk4b84b0(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, s;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k > 2) {
        s = util_RandomU(k);
        p = a[s];
        while (util_RandomU(k) == s)
            ;
    }
    if (b == 0 || p == 0)
        return 0x400;
    if (util_RandomU(2)) {
        txt_Format(buf, m->type + 0x12e7, b, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title,
                     p->id, p->id);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        gm_AddHealthS(p->id, (int)-(*(float *)((char *)p + 0x1c) + 1.0f));
    }
    return 0;
}
/* 0x4b86c0, message: as 0x4b82c0, but needs any worker (title set) in the chosen building; 0x4fca40 of c by
   -(0.05 .. 0.30 of c's 0x1c float) (not with item 0x290) */

int ai_Unk4b86c0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, i, w, x, y;
    void *a[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    w = -1;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            w = i;
    }
    if (w == -1 || b == 0)
        return 0x400;
    if (util_RandomU(2)) {
        txt_Format(buf, m->type + 0x12e7, b);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        x = (int)(*(float *)((char *)c + 0x1c) * 0.25);
        y = (int)(*(float *)((char *)c + 0x1c) * 0.05);
        gm_AddHealthS(c->id, -(util_RandomU(x) + y));
    }
    return 0;
}
/* 0x4b8870, message: c (money at least 2..4) loses that much 0x50cc50 (not with item 0x290) */

int ai_Unk4b8870(G14MSG *m)
{
    AICHAR *c;
    int s;
    char buf[0x200];

    s = util_RandomU(3) + 2;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (c->money < s)
        return 0x400;
    txt_Format(buf, m->type + 0x12e7, s);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    gm_SendMoneyDelta(c->netId, -s);
    return 0;
}
/* 0x4b8950, message: c gains 3..5 0x50cc50 (the text gets 0x5085f0(1, ai_OwnPlayer) and the amount) */

int ai_Unk4b8950(G14MSG *m)
{
    AICHAR *c;
    int s;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    s = util_RandomU(3) + 3;
    txt_Format(buf, m->type + 0x12e7, gm_PlayerFactor(1, ai_OwnPlayer), s);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    gm_SendMoneyDelta(c->netId, s);
    return 0;
}
/* 0x4b8a10, message: one of up to 8 jobs (0x53e400(1, 0, 0xf)) of c's objects whose ends (0xb0 / 0xb4, via
   0x4fea50) are 0x502520 buildings and one of kind 0x47; the object's byte 0x12 drops to 60..94 % (0x4c3c00), the
   text names the player of the 0x47 end (not with item 0x290) */

int ai_Unk4b8a10(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, r;
    unsigned char v;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        o = gm_FindObjectInst(*(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    r = util_RandomU(n);
    j = js[r];
    o = os[r];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    v = (unsigned char)(((unsigned char *)o)[0x12] * ((util_RandomU(0x23) + 0x3c) * 0.01f));
    cm_BeginFieldBlock(o, *(int *)(o + 2));
    cm_AddFieldDelta(1, 1, 0x12, &v);
    cm_SendFieldDeltas();
    if (*(char *)w == 0x47)
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0]);
    else
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4b8c80, message: as 0x4b8a10, but the chosen job is stamped with the time (0x52), 0x4c5010 / 0x4c4c60 are told,
   and the text gets the value s of the object and its 0x4ffe90 (2, 5, 6) goods (0x5087f0) */

int ai_MsgJobTime(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIOBJ *e;
    AIBLD *w, *u;
    int n, r, s;
    AIJOB *js[8];
    char *os[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        o = gm_FindObjectInst(*(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    r = util_RandomU(n);
    o = os[r];
    j = js[r];
    s = (int)G14_5087F0(*(unsigned short *)o, o[0x12]);
    for (e = gm_Query(*(int *)(o + 0x14), 2, 5, 6); e != 0; e = gm_QueryNext())
        s = (int)(G14_5087F0(*(unsigned short *)e, o[0x12]) * *(int *)((char *)e + 0xe) + s);
    *(AITIME *)((char *)j + 0x52) = ai_Time;
    cm_SendMoveObject(-1, -1, *(int *)(o + 2));
    cm_SendExSetHEI(-1, j);
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        if (*(char *)w == 0x47)
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], s);
        else
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)], s);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
/* 0x4b8f70, message: one of up to 8 jobs (0x53e400(1, 0, 0xf)) of c's objects (0x4fea50 of 0xac) whose ends are
   0x502520 buildings, one of kind 0x47; the text names the player of that end */

int ai_Unk4b8f70(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n;
    char *os[8];
    AIJOB *js[8];
    char buf[0x800];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if (*(char *)w == 0x47 || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (*(char *)w == 0x47)
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], ai_PlayerSlots[0]);
    else
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)],
                     ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    return 0;
}
/* 0x4b9180, message: c pays 1..2 % of 0x50a640; then as 0x4b8f70, but the text names the player of the second end
   (not with item 0x290) */

int ai_Unk4b9180(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, v;
    float k;
    AIJOB *js[8];
    char *os[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    k = (util_RandomU(2) + 1.0f) * 0.01f;
    v = (int)(gm_GetWorth(c->id) * k);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        if (*(char *)w == 0x47)
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0]);
        else
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)u + 0x65)], v);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
/* 0x4b9420, message: as 0x4b8f70 for jobs between two buildings without kind 0x47; the text gets both ends and
   2..4 (not with item 0x290) */

int ai_Unk4b9420(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, s;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w != 0x47) || *(char *)u != 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    s = util_RandomU(3) + 2;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        txt_Format(buf, m->type + 0x12e7, w, u, s);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
/* 0x4b9630, message: as 0x4b9420 (any check against item 0x290 missing) */
int ai_Unk4b9630(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, s;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if (*(char *)w != 0x47 || *(char *)u != 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    s = util_RandomU(3) + 2;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        txt_Format(buf, m->type + 0x12e7, w, u, s);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
/* 0x4b9810, message without effect (the unused table slots) */

int ai_Unk4b9810(G14MSG *m)
{
    return 0x400;
}
/* 0x4b9820, message: as 0x4b8f70, the text also gets tomorrow's date (not with item 0x290) */

int ai_Unk4b9820(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n;
    AITIME t;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((u != 0 && w != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    t = ai_Time;
    t.year++;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        if (*(char *)w == 0x47)
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], &t,
                         ai_PlayerSlots[0]);
        else
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)], &t,
                         ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
/* 0x4b9ab0, message: as 0x4b8f70, the text also gets 2..4 */

int ai_Unk4b9ab0(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, s;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if (*(char *)w == 0x47 || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    s = util_RandomU(3) + 2;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (*(char *)w == 0x47)
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], s, ai_PlayerSlots[0]);
    else
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)], s,
                     ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    return 0;
}
/* 0x4b9cd0, message 0x2b: one worker p (kind 0, title set) of one of c's category-1 buildings b is picked, and
   when a 0..49 roll beats gm_Liking(c, p, 1) (the worker does not like c, and a building's AIMSG type 0x31
   stops whoever it names) c is told (text 0x12e7 + 0x2b) and p is told to stop working: an AIMSG type 0x31
   (fromId b->id, to c, arg[0] p, the receive side cm.c 0x4c9c10 runs ch_StopActions on "sp_<p>", ch.c's stop of
   the action). Skipped when c holds the item 0x290. As 0x4b84b0 (which hurts the worker instead).
   @deep behaviour draft (42 -> 34 code hunks before the revert): the whole body reads the same as the original;
   the one differing region is the AIMSG fill after he_ShowNews (instruction 150 on), i.e. the tail of
   memset / fromId / to / time / type / flags / arg. Tried: (1) hoisting the fromId load into a local before the
   memset - the original loads [ebp+1] before the rep stos; it matched that one instruction (150 -> 155 equal)
   but grew the bytes (1414 -> 1447 differing, WORSE) and the rest stayed as a different schedule, so it was
   reverted; (2) spell.py's width lead (16-bit vs 32-bit read of the same slot) - both sides read `mov ax` and
   store a WORD (msg.time's last word, 0x34), so the widths already agree; the 16-bit store's disp differs by 4
   only because the original sinks it past `push &msg`. Nothing left in base's means: it is the store / load
   schedule and the register choice of the time copy (original 0x30 before 0x28, ours ascending). */

int ai_MsgWorkerStop(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i;
    float f;
    void *a[8];
    AIMSG msg;
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(b->type) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind == 0 && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k > 2)
        p = a[util_RandomU(k)];
    if (b == 0 || p == 0)
        return 0x400;
    f = gm_Liking(c->id, p->id, 1);
    if (util_RandomU(0x32) > f) {
        txt_Format(buf, m->type + 0x12e7, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title, p->id, b);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        memset(&msg, 0, sizeof(msg));
        msg.fromId = b->id;
        msg.to = c->netId;
        msg.time = ai_Time;
        msg.type = 0x31;
        msg.flags = 1;
        msg.arg[0] = p->netId;
        cm_SendLongMessage(&msg);
        return 0;
    }
    return 0x400;
}
/* 0x4b9f30, message: a worker (kind 0) of one of c's buildings of kind 1 / 2 is named in the text */

int ai_Unk4b9f30(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, t;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    k = 0;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_CharsT[i].kind == 0 && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            a[k++] = &ai_CharsT[i];
    }
    if (k > 1)
        p = a[util_RandomU(k)];
    if (b == 0 || p == 0)
        return 0x400;
    t = (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title;
    txt_Format(buf, m->type + 0x12e7, t, p->id, p->id, t);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4ba0c0, message: one of c's buildings of kind 1 / 2 is picked, and when at least 2 of its kind-0 workers (up
   to k231 + k232 of the building type) all have a gm_Liking (c, worker, 1) of at least a 0x19..0x2c roll, an
   AIMSG type 0x4a names the building (id, fromId, n), c, the object gm_BuildingMainObject (0x501b50) gives and
   the news text id, to the local player ai_PlayerId; the receive side (cm.c 0x4cbb90) finds that object and tears
   it down (f3gm.c 0x5034b0). The three 0x400 guards share one exit: the goto none keeps that block last, as in
   the original.
   @deep behaviour reached (10 order hunks: 168/168 instructions, spell.py 'same instructions, other order') - byte needs the scheduler (the order of the b->id load, the type / flags stores and the ai_PlayerId lea). Raised here: the frame (0x128 -> 0x124) and the FP compare via the liking operand order (gm_Liking(...) < r: fild after the call, fcompp, no float spill), the shared 0x400 exit via goto none, msg.n = msg.fromId (one b->id load). Q1: no name evidence - the gate is the workers' liking, but the 0x4a receive side (0x4cbb90 -> 0x5034b0) tears the building's object down and the text 0x1315 is not in the exe, so the message's wording cannot be stated. Q4: the one left is the 16-bit text-id store over AIMSG.dst[0] (0x64); the textId union arm in ai.h was built and regressed ai_Unk483a60 byte -> 5 differing (its slots, the same failure ai.c's note records for 0x60), reverted - a shared-header decision. */

int ai_Unk4ba0c0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    AIBTYPE *t;
    AIOBJ *e;
    int n, k, i, r;
    void *a[8];
    AIMSG msg;

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(b->type);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        goto none;
    k = 0;
    r = util_RandomU(0x14) + 0x19;
    b = a[util_RandomU(n)];
    t = &ai_BTypesT[b->type];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 0 && ai_CharsT[i].bld == b) {
            if (gm_Liking(c->id, i, 1) < r)
                goto none;
            if (++k >= t->k231 + t->k232)
                goto none;
        }
    }
    if (k >= 2) {
        e = gm_BuildingMainObject(b);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x4a;
        msg.fromId = b->id;
        msg.flags = 1;
        msg.n = msg.fromId;
        msg.to = ai_CharsT[ai_PlayerId].netId;
        msg.time = ai_Time;
        /* the text id at 0x64 is the only 16-bit store of AIMSG (a word over dst[0]); a textId union arm in
           ai.h regresses ai_Unk483a60's slots (ai.c's note), so the shared-header view is match-deep's */
        *(unsigned short *)&msg.dst[0] = m->type + 0x12e7;
        msg.car = c->netId;
        msg.ch = e->objId;
        cm_SendLongMessage(&msg);
        return 0;
    }
none:               /* the guards share this one 0x400 exit, as in the original */
    return 0x400;
}
/* 0x4ba2d0, message: the first worker of kind 1 in one of c's buildings of kind 1 gains 5..14 of skill 0x81 (up to
   0xfc) through 0x4c6020 */

int ai_Unk4ba2d0(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, i, s;
    void *a[8];
    char buf[0x100];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(*(unsigned char *)b) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 1 && ai_CharsT[i].bld == b) {
            p = &ai_CharsT[i];
            break;
        }
    }
    if (b == 0 || p == 0)
        return 0x400;
    s = 0xfc - p->sk81;
    if (s <= 0)
        return 0x400;
    cm_SendAddSkill(p->netId, 1, (char)min(s, util_RandomU(10) + 5));
    txt_Format(buf, m->type + 0x12e7, 0x16b8, p->id, b);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4ba470, message: as 0x4ba2d0, the worker loses 5..14 of skill 0x81 (not with item 0x290) */

int ai_Unk4ba470(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, i, s;
    void *a[8];
    char buf[0x100];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(*(unsigned char *)b) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 1 && ai_CharsT[i].bld == b) {
            p = &ai_CharsT[i];
            break;
        }
    }
    if (b == 0 || p == 0)
        return 0x400;
    s = p->sk81;
    if (s <= 0)
        return 0x400;
    cm_SendAddSkill(p->netId, 1, -(char)min(s, util_RandomU(10) + 5));
    txt_Format(buf, m->type + 0x12e7, p->id, b, p->id, 0x16b8);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4ba630, message: one of c's buildings of kind 1 / 2 gets item 0x134 / 0x135 (0x4c4880); the text names one of
   up to 16 characters (not kind 6 / 7, below 10) of one half of ai_Chars, else c (the reverse direction finds
   nobody); sent with 0x53dee0
   @deep behaviour: 112 code hunks + 4 regs, 188/192 instructions, far. Read against the original: the statements match (the building loop, the 0x180/0x300 half pick with d = -1, the kind/id filter, the 16-entry cap, p = c fallback, cm_SendExSellObject, the text and he_MessageBoxQuickjump's 8 arguments). The root difference is allocation, not meaning: the original keeps k in ebp (xor ebp,ebp at the entry, inc ebp / cmp ebp,0x10 in the loop) and uses the zeroed ebp for its NULL compares (cmp eax,ebp), ours keeps k in [esp+0xc] and reloads it; that also drags p (original spills it to [esp+0x10], ours keeps it in edi) and the entry block (the original's zero store to c's slot before the call). Leads tried: spell.py has 'no menu rule; meaning check first' on the two entry hunks and reload leads on 4ba74f / 4ba7d4 / 4ba84f (the original re-reads, ours keeps the value - i.e. its spills, not a dropped local in our source; b->id is already re-read at both uses); adding p = 0 (the sibling idiom) compiled identically and was reverted. Q1: no name evidence - the text of message 0x31 (0x1318) is not in the exe and the good / object ids 0x134 / 0x135 it sells are unnamed, so the event's wording cannot be stated. Q4 is fixed (b->type, b->id) and Q2 by the he_MessageBoxQuickjump rename. */

int ai_Unk4ba630(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, lo, hi, d;
    void *a[16];
    char buf[0x100];

    k = 0;
    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(b->type);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_RandomU(n)];
    d = 1;
    if (util_RandomU(2)) {
        lo = 0;
        hi = 0x180;
    } else {
        lo = 0x180;
        hi = 0x300;
    }
    if (util_RandomU(2)) {
        hi = 0;
        lo = 0x180;
        d = -1;
    }
    for (i = lo; i < hi; i += d) {
        if (ai_CharsT[i].kind != 6 && ai_CharsT[i].kind != 7 && ai_CharsT[i].kind < 10 && ai_CharsT[i].id != 0xffff) {
            a[k++] = &ai_CharsT[i];
            if (k == 16)
                break;
        }
    }
    if (k != 0)
        p = a[util_RandomU(k)];
    else
        p = c;
    if (b == 0 || p == 0)
        return 0x400;
    cm_SendExSellObjectI(b->id, -1, util_RandomU(2) + 0x134, 1, ai_OwnPlayer, 0);
    txt_Format(buf, m->type + 0x12e7, p->id);
    he_MessageBoxQuickjumpI(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, b->id, -1, 0, 0);
    return 0;
}
/* 0x4ba840, takes a share f of the goods (0x4ffe90 (1, 5) list, more than 1 each) of the object (2, 6, 0, 0x2a) of
   one of c's buildings of kind 1 / 2 (0x4c4880 to -1); *id = the building; 0: none */

int ai_Unk4ba840(AICHAR *c, int *id, float f)
{
    AIBLD *b;
    AIOBJ *o, *e;
    int n, k, i, v;
    void *a[12];

    n = 0;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n != 0) {
        k = 0;
        b = a[util_RandomU(n)];
        *id = *(int *)((char *)b + 1);
        o = gm_Query(*(int *)((char *)b + 0x5d), 2, 6, k, 0x2a);
        if (o != 0) {
            for (e = gm_Query(*(int *)((char *)o + 0x14), 1, 5); e != 0 && k < 12; e = gm_QueryNext())
                a[k++] = e;
            if (k != 0) {
                for (i = 0; i < k; i++) {
                    e = a[i];
                    v = *(int *)((char *)e + 0xe);
                    if (v > 1) {
                        v = (int)(v * f);
                        if (v < 1)
                            v = 1;
                        cm_SendExSellObjectI(-1, *(int *)((char *)o + 2), *(unsigned short *)e, v, ai_OwnPlayer, 0);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4ba970, message: 0x4ba840 takes 13..25 % of the goods of one of c's buildings; the text names its 0x501190
   (1, 1, id) entry and the share in % (not with item 0x290) */

int ai_Unk4ba970(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_RandomU(0xd) + 13.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, b, (int)(f * 100.0f));
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        return 0;
    }
    return 0x400;
}
/* 0x4baa80, message: as 0x4ba970 (5..19 %), sent with 0x53dee0 and the building */

int ai_Unk4baa80(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_RandomU(0xf) + 5.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, b, (int)(f * 100.0f));
        he_MessageBoxQuickjumpI(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, *(int *)((char *)b + 1), -1, 0, 0);
        return 0;
    }
    return 0x400;
}
/* 0x4baba0, message: as 0x4baa80 (5..24 %, the share first in the text) */

int ai_Unk4baba0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_RandomU(0x14) + 5.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, (int)(f * 100.0f), b);
        he_MessageBoxQuickjumpI(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, *(int *)((char *)b + 1), -1, 0, 0);
        return 0;
    }
    return 0x400;
}
/* 0x4bacc0, message 0x38: unless a 0..255 roll is below c's luck (skill 0x82) and unless c holds item 0x290, one
   of up to 6 goods of c's inventory (gm_Query (1, 5), not the goods of type 9) is lost: cm_SendExSellObject with
   -1 as the buyer (cm.c 0x4c70b0 moves the item from data[4] = c to data[0] = -1) and the news text 0x9a8 + 2 *
   the good names it */

int ai_MsgLoseGood(G14MSG *m)
{
    AICHAR *c;
    AIOBJ *e;
    int n;
    void *a[6];
    char buf[0x100];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (util_RandomU(0x100) < (unsigned short)c->sk82)
        return 0x400;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (e = gm_Query(c->inventory, 1, 5); e != 0 && n < 6; e = gm_QueryNext()) {
        if (ai_GoodsT[e->id].type != 9)
            a[n++] = e;
    }
    if (n == 0)
        return 0x400;
    e = a[util_RandomU(n)];
    cm_SendExSellObjectI(-1, c->netId, e->id, 1, ai_OwnPlayer, 0);
    txt_Format(buf, m->type + 0x12e7, e->id * 2 + 0x9a8);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bae20, message (if 0..255 is not above c's skill 0x82): c gets 1..3 % of 0x50a640 */

int ai_Unk4bae20(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x800];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (util_RandomU(0x100) > (unsigned short)c->sk82)
        return 0x400;
    k = util_RandomU(3) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(m->id, -1, ai_OwnPlayer, v);
    return 0;
}
/* 0x4baf30, message: c gains 8..24 of one of the skills 0x80..0x84 (up to 0xfc) through 0x4c6020 */

int ai_Unk4baf30(G14MSG *m)
{
    AICHAR *c;
    int s, d;
    char buf[0x100];

    s = util_RandomU(5);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    d = 0xfc - ((unsigned char *)c)[0x80 + s];
    if (d <= 0)
        return 0x400;
    cm_SendAddSkill(c->netId, s, (char)min(d, util_RandomU(0x11) + 8));
    txt_Format(buf, m->type + 0x12e7, s + 0x16b7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb030, message: c loses 6..17 of one of the skills 0x80..0x84 (not with item 0x290) */

int ai_Unk4bb030(G14MSG *m)
{
    AICHAR *c;
    int s, d;
    char buf[0x100];

    s = util_RandomU(5);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    d = ((unsigned char *)c)[0x80 + s];
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (d <= 0)
        return 0x400;
    cm_SendAddSkill(c->netId, s, -(char)min(d, util_RandomU(0xc) + 6));
    txt_Format(buf, m->type + 0x12e7, s + 0x16b7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb150, message for c with field 0x2c set: c pays 1..4 % of 0x50a640 */

int ai_Unk4bb150(G14MSG *m)
{
    AICHAR *c;
    double k;
    int v;
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (*(int *)((char *)c + 0x2c) == 0)
        return 0x400;
    k = util_RandomU(4) + 1.0;
    v = (int)(gm_GetWorth(c->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
/* 0x4bb250, message for c with field 0x2c set: the text 0x4c + the season (gm_Season) of the calendar record
   gm_ClockToDate makes of the game clock
   @deep byte reached (the meaning fix: the calendar record is 10 bytes (f3gm.c gm_ClockToDate's out), not an int - the frame was 8 bytes short and the calls wrote past it; gm_ClockToDate(&t, d) with unsigned char d[10], then gm_Season(d)). Q1: no name evidence - the text is 0x4c + the season and the gate is AICHAR+0x2c != 0, a field with no role name anywhere (ai_Unk4bb150 has the same gate), so the message cannot be named. Q2: gm_ClockToDate / gm_Season are f3gm.c's gm_ClockToDate / gm_Season (same addresses) but declared locally in 5 TUs (sv.c and main.c even pass an int clock and a K&R form) - the project-wide rename needs the canonical prototypes in f3gm.h plus those callers fixed (a cross-unit shared-header decision). Q4: AICHAR+0x2c is in pad028[8] with no evidenced role; naming it unk2c would only move the debt. */

int ai_Unk4bb250(G14MSG *m)
{
    AICHAR *c;
    AITIME t;
    unsigned char d[10];                /* the calendar record gm_ClockToDate fills (f3gm.c's d[10]) */
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (*(int *)((char *)c + 0x2c) == 0)
        return 0x400;
    t = ai_Time;
    gm_ClockToDate(&t, d);
    txt_Format(buf, m->type + 0x12e7, gm_Season(d) + 0x4c);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb330, message if 0x5054f0 of c: c pays 0x5085f0(1..5, ai_OwnPlayer) (not with item 0x290) */

int ai_Unk4bb330(G14MSG *m)
{
    AICHAR *c;
    int v;
    char buf[0x400];

    v = gm_PlayerFactor(util_RandomU(5) + 1, ai_OwnPlayer);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (!gm_CatchMinorDisease(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
    return 0;
}
/* 0x4bb430, message if 0x5056a0 of c (not with item 0x290) */

int ai_Unk4bb430(G14MSG *m)
{
    AICHAR *c;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    if (!gm_CatchUpperDisease(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb4f0, message if 0x5059d0 of c */

int ai_Unk4bb4f0(G14MSG *m)
{
    AICHAR *c;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!gm_CureMinorDisease(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb590, message if 0x505ad0 of c */

int ai_Unk4bb590(G14MSG *m)
{
    AICHAR *c;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!gm_CureUpperDisease(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb630, message only (types 0x42..0x44) */

int ai_Unk4bb630(G14MSG *m)
{
    AICHAR *c;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
/* 0x4bb6c0, sends message m (from the 8th day on): -1 without m, 0x400 without its character or with heldRounds set,
   -2 for an unknown type */

int ai_Unk4bb6c0(G14MSG *m)
{
    AICHAR *c;

    if (ai_Time.year < 8)
        return 0;
    if (m == 0)
        return -1;
    c = gm_FindChar(m->id);
    if (c == 0 || c->heldRounds > 0)
        return 0x400;
    if (m->type < 0x45)
        return ai_Unk602570[m->type](m);
    return -2;
}

int sv_ReturnZero(void);                                                          /* 0x52a040, defined elsewhere */

G14CB ai_Unk602570[0x45] = {                                                     /* 0x602570 */
    ai_MsgRewardPct1to3, ai_MsgRewardPct2to4, ai_MsgOfficeApplicantPayment, ai_MsgPayPct1to3,
    ai_MsgPayPct1to4, ai_MsgRewardPct2to5, ai_MsgRewardPct1to3Type68, ai_MsgPayForBuilding,
    ai_MsgRewardPct1to3Type68, ai_MsgPayPct2to4, ai_MsgPayPct125, ai_MsgPayPctOver120,
    ai_MsgPayPct1to3Type0c, ai_MsgPayPct2to4Type0d, ai_MsgPayPct1to2Type0e, ai_MsgPayPct1to4Type0f,
    ai_MsgPayPct1to4OfficeType10, ai_MsgPayPct1to3BuildingType11, ai_MsgPayPct1to2Type1213, ai_MsgPayPct1to2Type1213,
    ai_Unk4b77b0, ai_Unk4b78c0, ai_Unk4b79c0, ai_Unk4b7ac0,
    ai_Unk4b7bc0, ai_Unk4b7d80, ai_Unk4b7f40, ai_Unk4b8060,
    ai_Unk4b81a0, ai_Unk4b82c0, ai_Unk4b84b0, ai_Unk4b86c0,
    ai_Unk4b8870, ai_Unk4b8950, ai_Unk4b8a10, ai_MsgJobTime,
    ai_Unk4b8f70, ai_Unk4b9180, ai_Unk4b9420, ai_Unk4b9630,
    ai_Unk4b9810, ai_Unk4b9820, ai_Unk4b9ab0, ai_MsgWorkerStop,
    ai_Unk4b9f30, sv_ReturnZero, ai_Unk4ba0c0, ai_Unk4ba2d0,
    ai_Unk4ba470, ai_Unk4ba630, ai_Unk4ba970, ai_Unk4baa80,
    ai_Unk4baba0, ai_Unk4b9810, ai_Unk4b9810, ai_Unk4b9810,
    ai_MsgLoseGood, ai_Unk4bae20, ai_Unk4baf30, ai_Unk4bb030,
    ai_Unk4bb150, ai_Unk4bb250, ai_Unk4bb330, ai_Unk4bb430,
    ai_Unk4bb4f0, ai_Unk4bb590, ai_Unk4bb630, ai_Unk4bb630,
    ai_Unk4bb630
};
unsigned int ai_Unk6f0730 = 0;  /* 0x6f0730, random seed of the messages */

unsigned int ai_Unk4bb7d0(unsigned int n);

__declspec(dllimport) unsigned long __stdcall timeGetTime(void);

/* 0x4bb720, posts 0..n random messages for character id (AIMSG type 0x42 with the message type in n); returns the
   number; reseeds ai_Unk6f0730 with timeGetTime() */
int ai_PostRandomMessages(int n, int id)
{
    int k, i;
    AIMSG msg;

    k = ai_Unk4bb7d0(n + 1);
    ai_Unk6f0730 = timeGetTime();
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x42;
    msg.to = id;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 1;
    for (i = k; i > 0; i--) {
        *(unsigned short *)&msg.n = ai_Unk4bb7d0(0x45);
        msg.car = id;
        cm_SendLongMessage(&msg);
    }
    return k;
}
/* 0x4bb7d0, a random number below n (LCG ai_Unk6f0730; 0 for n 0) */

unsigned int ai_Unk4bb7d0(unsigned int n)
{
    if (n == 0)
        return 0;
    ai_Unk6f0730 = ai_Unk6f0730 * 0x41c64e6d + 0x3039;
    return (*((unsigned short *)&ai_Unk6f0730 + 1) % 0x7fffu) % n;
}
