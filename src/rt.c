/* rt.c -- routing: route point lists ("rt_point"), the routing dialogs ("misc\routing_choose_good_enhanced",
   "misc/routing_edit_point", "misc\routing_settings"), "rt_load". Start: its .data block 0x602a90 begins with a -1
   global right after rd.c's literals. Names are ours.
   unit of rea 8cef7d5b (src/game/rt.c); boundaries in decl/text.dcl */
#include "d2_types.h"
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include "rt.h"
#include "di_input.h"
#include "data_views.h"
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original rt's calls push 4 arguments (as 139 of the original's call sites do) */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, int, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c, unsigned char e): @original rt.c pushes the third argument as an int (byte-matched rt_UnloadAtPoint) */
#include "d3_types.h"
#include "ahm.h"
#include "cm.h"
#include "f3gm.h"
#include "he.h"
#include "hst.h"
#include "io_text.h"
#include "ls.h"
#include "mem.h"
#include "sv.h"
#include "lc.h"
#include "vfs.h"
#include "ch.h"
#include "d2.h"
#include "d3_sm.h"
#include "gfx.h"
#include "d3_pose.h"

int rt_MarkGroup = -1;                  /* 0x602a90 */
int ai_Unk62c26c = 1;  /* 0x62c26c */
extern RTMARK rt_Marks[8];
/* ai_Unk143a588: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnCart: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad28: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad2c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad30: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad34: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad38: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad3c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad44: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad4c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad54: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad58: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad5c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad60: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad64: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad78: view of txt_Ptrs (build/mk_txt_views.py) */
int rt_Unk6f074c = 0;  /* 0x6f074c, the saved screen of */
int rt_MarkCount = 0;  /* 0x6f0750, the marks used */
int rt_PickedBuilding = 0;  /* 0x6f0754, the building shown */
RTPOINT * rt_EditRoute = 0;  /* 0x6f0758, the route being edited */
char * rt_MapSurface = 0;  /* 0x6f26ac, the screen surface */
/* ai_Unk7e422c: view of ai_PlayerSlots (tools/data_views.py) */
RTMARK rt_Marks[8] = {0};  /* 0xf812a0 */

/* 0x4bd950, appends a new point to list p (0 if it has 8 already) */
RTPOINT *rt_AddPoint(RTPOINT *p)
{
    RTPOINT *q;
    RTPOINT *n;
    int i;

    i = 0;
    q = 0;
    if (p != 0) {
        do {
            q = p;
            i++;
            p = p->next;
        } while (p != 0);
        if (i >= 8)
            return 0;
    }
    n = mem_Alloc(0x40, "rt_point");
    memset(n, 0, 0x40);
    if (q != 0)
        q->next = n;
    n->prev = q;
    n->buildingId = -1;
    return n;
}
/* 0x4bd9b0, unlinks and frees point p */

int rt_RemovePoint(RTPOINT *p)
{
    if (p->prev != 0)
        p->prev->next = p->next;
    if (p->next != 0)
        p->next->prev = p->prev;
    mem_Free(p);
    return 1;
}
/* 0x4bd9e0, frees list p */

void rt_FreePoints(RTPOINT *p)
{
    RTPOINT *n;

    while (p != 0) {
        n = p;
        p = p->next;
        mem_Free(n);
    }
}
/* 0x4bda00, a copy of list p */

RTPOINT *rt_CopyPoints(RTPOINT *p)
{
    RTPOINT *n;
    RTPOINT *first;

    n = 0;
    first = 0;
    for (; p != 0; p = p->next) {
        n = rt_AddPoint(n);
        if (first == 0)
            first = n;
        memcpy(n->loadSlots, p->loadSlots, sizeof(n->loadSlots));
        n->buildingId = p->buildingId;
        memcpy(n->unloadSlots, p->unloadSlots, sizeof(n->unloadSlots));
        n->cartId = p->cartId;
    }
    return first;
}
/* 0x4bda60, the index of q in list p (the length if not in it) */

int rt_PointIndex(RTPOINT *p, RTPOINT *q)
{
    int i;

    i = 0;
    for (; p != 0 && q != p; p = p->next)
        i++;
    return i;
}
/* 0x4bda80, point n of list p */

RTPOINT *rt_PointAt(RTPOINT *p, int n)
{
    int i;

    for (i = 0; p != 0 && n != i; i++)
        p = p->next;
    return p;
}
/* 0x4bdaa0, the length of list p */

int rt_PointCount(RTPOINT *p)
{
    int i;

    i = 0;
    for (; p != 0; p = p->next)
        i++;
    return i;
}
/* 0x4bdac0, clears the radio group rt_MarkGroup */

void rt_ClearMarkGroup(void)
{
    if (rt_MarkGroup != -1)
        d2_RadioClear(rt_MarkGroup);
    rt_MarkGroup = -1;
}
/* 0x4bdae0, puts the marks into the radio group rt_MarkGroup */

void rt_BuildMarkGroup(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1) {
            if (rt_MarkGroup == -1)
                rt_MarkGroup = d2_CreateRadioGroup(1, m->obj);
            else
                d2_RadioAdd(rt_MarkGroup, m->obj);
        }
    }
}
/* 0x4bdb30, a mark (picture 0x7bb + the index of p in list) on window win at building p->buildingId; -1 if it has none @deep behaviour: every statement read against the disassembly (0x502020, 0x458a20, 0x45f170, 0x5a6b70, 0x4731d0, the mark bookkeeping) is already in the C; the 151 code hunks are stack slots: our frame 0x20 vs the original's 0x28, v at +0x24 vs +0x14, pt at +0x1c vs +0x24, px/pz swapped - VC6 local allocation (declaration order/sizes) and deferred arg cleanup, not a meaning error. Q1/Q2 (gm_FindBuildingById, ai_Unk5a6b70, ai_Unk7e422c, ai_Unkfd7c50, d2_WinOpenPic) stay open */

int rt_Unk4bdb30(char *win, int b, RTPOINT *list, RTPOINT *p)
{
    void *h;
    int x, z;
    char *bld;
    float v[4];
    float pt[2];
    int px, pz;
    float y;
    int o;
    int k;

    h = ai_Unkfd7c50;
    x = *(int *)(ai_Unk7e422c + ai_OwnPlayer * 0x2f4);
    z = *(int *)(ai_Unk7e422c + 4 + ai_OwnPlayer * 0x2f4);
    bld = gm_FindBuildingById(p->buildingId);
    if (bld == 0)
        return -1;
    d3_PointToWorld(*(char **)(bld + 0x61), *(char **)(bld + 0x61) + 0x4c, v);
    if (!d3_GetHeightAt(h, v, &px, &pz, &y))
        return -1;
    ai_Unk5a6b70(pt, x, z);
    o = d2_WinOpenPic((int)pt[0] - 0x20, (int)pt[1] - 0x20, rt_PointIndex(list, p) + 0x7bb, *(int *)(win + 8));
    rt_Marks[rt_MarkCount].obj = o;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x48) = 1;
    if (rt_MarkGroup == -1)
        rt_MarkGroup = d2_CreateRadioGroup(1, rt_Marks[rt_MarkCount].obj);
    else
        d2_RadioAdd(rt_MarkGroup, rt_Marks[rt_MarkCount].obj);
    rt_Marks[rt_MarkCount].p = p;
    k = rt_MarkCount++;
    return rt_Marks[k].obj;
}
/* 0x4bdc90, the slider value of mark m */

void rt_MarkValue(RTMARK *m)
{
    d2_GetObjValue(m->obj);
}
/* 0x4bdca0, 0x4bdc90 for each mark */

void rt_ReadMarkValues(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1)
            rt_MarkValue(m);
    }
}
/* 0x4bdcd0, the dialog "misc\routing_choose_good_enhanced": the goods of building b to pick from (its stock of a
   market (type 0x1c, more than 1) / warehouse (type 9), else the last list with the stock of b), plus "none"
   (picture 0x527); returns the good picked, def if none
   @deep behaviour: the diff read end to end is one instruction stream shifted (ours 0xf bytes shorter): register allocation (ebx/ebp/esi), argument spills and stack slots (the original keeps b in [esp+0xc] and reads it at the end, ours keeps it in esi), tail nop fill - no statement differs; the 497 code hunks are the misalignment this causes. Needs the deep slot/register steering. Q2 (17 callees/globals) and Q4 (23 raw accesses, D2OBJ/d2_Objs view) stay open */

int rt_Unk4bdcd0(unsigned char *b, int x, int def)
{
    char *t;
    int w;
    char *s;
    unsigned short *g;
    char *e;
    unsigned int n;
    int i;
    int o;
    int ret;
    int objs[10];

    t = ai_BTypes + *b * 0x24d;
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 4);
    d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 2);
    if (*t == 0x1c) {
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1dc, 6);
        n = 0;
        if (s != 0) {
            for (g = gm_Query(*(int *)(s + 0x14), 1, 5); g != 0; g = gm_QueryNext()) {
                if (*(int *)(g + 7) > 1) {
                    o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, *g + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    gm_KindObjs[n] = *g;
                    n++;
                }
            }
        }
    } else if (*t == 9) {
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x116, 6);
        n = 0;
        if (s != 0) {
            for (g = gm_Query(*(int *)(s + 0x14), 1, 5); g != 0; g = gm_QueryNext()) {
                if (*(int *)(g + 7) > 0) {
                    o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, *g + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    gm_KindObjs[n] = *g;
                    n++;
                }
            }
        }
    } else {
        gm_BuildKindObjects(b);
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0xff, 6);
        n = 0;
        for (i = 0; i < gm_KindObjCount; i++) {
            e = gm_Query(*(int *)(s + 0x14), 1, 0, gm_KindObjs[n]);
            if (e != 0 && *(int *)(e + 0xe) > 1) {
                o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, gm_KindObjs[n] + 0xdf, d2_LastWindow);
                objs[n] = o;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 0x23;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1cc) = 4;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1d0) = 0xf;
                sprintf(d2_ObjsC + o * 0x2ec + 0xbc, "%s", *(char **)(txt_ObjectNames + gm_KindObjs[n] * 8));
                n++;
            }
        }
    }
    gm_KindObjCount = (unsigned short)n;
    gm_KindObjs[n] = 0;
    o = d2_WinOpenPic((int)n % 4 * 0x40 + 7, (int)n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[n] = o;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
    ret = def;
    while (sv_RunFrame(rt_Unk4bdcd0, 0x657c7)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < gm_KindObjCount + 1; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = gm_KindObjs[i];
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
/* 0x4be180, the dialog "misc\routing_choose_good_enhanced" with the goods of route p (three slots per point,
   loadSlots[2 * k] with a count above 0); returns the good picked, 0 if none
   @deep behaviour: the whole diff is the prologue (17 code hunks, 10 tail nops, 167 equal instructions): the original loads p into eax before the register saves and moves it to esi after the test, stores n's 0 as an immediate; ours loads p straight into esi and stores ebx - same statements, VC6 register allocation/scheduling. Declaration initializers (int n = 0) tried, no change -> deep steers this. Q2 (10 callees/globals) and Q4 (5 raw accesses) stay open */

int rt_Unk4be180(int x, RTPOINT *p)
{
    int n;
    int ret;
    int k;
    int *s;
    int w;
    int ok;
    int i;
    int o;
    int objs[10];
    int goods[24];

    n = 0;
    ret = 0;
    for (; p != 0; p = p->next) {
        s = p->loadSlots;
        for (k = 3; k != 0; k--) {
            if (s[0] != 0 && s[1] > 0)
                goods[n++] = s[0];
            s += 2;
        }
    }
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 4);
    ok = d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 2);
    for (i = 0; i < n; i++) {
        o = d2_WinOpenPic(i % 4 << 6, i / 4 << 6, goods[i] + 0xdf, d2_LastWindow);
        objs[i] = o;
        *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
        *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
        *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
    }
    goods[i] = 0;
    o = d2_WinOpenPic(i % 4 * 0x40 + 7, i / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[i] = o;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
    while (sv_RunFrame(rt_Unk4be180, 0x657c7)) {
        if (dm_L.rup != 0 || ret != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < n + 1; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = goods[i];
                    break;
                }
            }
        }
    }
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
/* 0x4be3b0, the dialog "misc\routing_choose_good_enhanced" for building b with seven tabs (stock kinds 0x92..0x97,
   0x2db; text "$Z%2N3$A%1G"): the goods of the tab picked; returns the good picked, def if none
   @deep behaviour: frame 0x1d0 vs the original 0x2e4 (0x114 = 69 ints): the local arrays are too small/misordered (ours kinds/objs/goods/tabs land at +0x34/0x50/0x120/0x128, the original's at +0x44/0x54/0x104/0x1d0) and the s==0 arm differs in which value is pushed ([eax+0x14] vs [emp+0x5d] alignment) - the diff was read end to end; a real reread of the locals and that branch is needed. Q2/Q4 stay open */

unsigned int rt_Unk4be3b0(char *b, int x, unsigned int def)
{
    int first;
    int last;
    int kinds[7];
    int objs[0x30];
    unsigned int goods[0x30];
    int tabs[7];
    unsigned int ret;
    int w;
    int ok;
    int g;
    int i;
    int y;
    char *s;
    unsigned short *e;
    int n;
    int o;

    ret = def;
    first = 1;
    last = -1;
    kinds[0] = 0x95;
    kinds[1] = 0x92;
    kinds[2] = 0x93;
    kinds[3] = 0x94;
    kinds[4] = 0x97;
    kinds[5] = 0x96;
    kinds[6] = 0x2db;
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 1);
    io_PrintText((int)"$Z%2N3$A%1G", *(unsigned short *)(b + 0x27), b);
    d2_SetForm(w, 4);
    ok = d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 3);
    g = d2_CreateRadioGroup(0);
    for (i = 0, y = 4; y < 0xd6; i++, y += 0x1e) {
        o = sv_AddPanelButton(0, y, ai_Unk143a588[i]);
        tabs[i] = o;
        *(void **)(d2_ObjsC + o * 0x2ec + 0x2e8) = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, kinds[i]);
        d2_RadioAdd(g, o);
    }
    d2_RadioSelect(g, 0);
    d2_ClearInput();
    for (;;) {
        if (dm_L.rup != 0 || ret != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (d2_RadioSel[g * 0x23] != last || first) {
            d2_SetForm(w, 2);
            d2_RemoveWindowObjects(d2_GetViewWin(w, 2), 0);
            for (i = 0; i < 0x30; i++)
                objs[i] = -1;
            n = 0;
            s = *(char **)(d2_ObjsC + tabs[d2_RadioSel[g * 0x23]] * 0x2ec + 0x2e8);
            if (s == 0)
                s = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xff);
            else
                s = gm_Query(*(int *)(s + 0x14), 1, 0, 0xff);
            if (s != 0) {
                for (e = gm_Query(*(int *)(s + 0x14), 1, 5); e != 0; e = gm_QueryNext()) {
                    o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, *e + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    goods[n] = *e;
                    n++;
                }
            }
            goods[n] = 0;
            o = d2_WinOpenPic(n % 4 * 0x40 + 7, n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
            objs[n] = o;
            first = 0;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
            last = d2_RadioSel[g * 0x23];
        }
        if (d2_ClickedId != -1) {
            for (i = 0; i < 0x30; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = goods[i];
                    break;
                }
            }
        }
        if (!sv_RunFrame(rt_Unk4be3b0, 0x657c7))
            break;
    }
    d2_RadioClear(g);
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
/* 0x4be790, the controls of one good slot (k 0..2) of a route point: a slider (0x50ab50(a, good) steps) and the
   good's picture with its name; out = {-1, picture, slider}; returns the picture
   @deep behaviour: frame matches (0x24) and every statement corresponds (offs inits, the slider 0x50ab50, d2_WinOpenSlider(x-8, y+0x1d, 0x2f, 0, s, 0x6a, 0x2d, win), d2_WinOpenInputIcon(x, y+0x22, good+0xdf, 0x8a, win), the out[] stores and the d2_Objs fields); the 102 code hunks are register allocation and scheduling (ebp/ebx/eax swaps, the two open calls interleaved, offs stores reordered) - misalignment, no statement differs. Q2 (txt_ObjectNames, gm_ListStock, d2_SetObjFont) and Q4 (4) stay open */

int rt_Unk4be790(unsigned short *bt, int a, int good, int k, int x, int y, int win, int *out)
{
    int offs[9];
    int s;
    int o;

    offs[1] = 0;
    offs[2] = 0;
    offs[5] = 0;
    offs[0] = 0x51;
    offs[3] = 0x30;
    offs[4] = 0x71;
    offs[6] = 0x11;
    offs[7] = 0x52;
    offs[8] = 0x93;
    s = gm_ListStock(a, good);
    x = offs[(*bt - 0x134) * 3 + k] + 0x6e + x;
    s = d2_WinOpenSlider(x - 8, y + 0x1d, 0x2f, 0, s, 0x6a, 0x2d, win);
    o = d2_WinOpenInputIcon(x, y + 0x22, good + 0xdf, 0x8a, win);
    d2_SetObjFont(o, 0x54);
    out[0] = -1;
    out[1] = o;
    out[2] = s;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 0x22;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1cc) = 4;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d0) = 0xf;
    sprintf(d2_ObjsC + o * 0x2ec + 0xbc, "%s", *(char **)(txt_ObjectNames + good * 8));
    return o;
}
/* 0x4be8e0, removes the three controls of out (0x4be790) */

void rt_RemoveControls(int *out)
{
    if (out[0] != -1)
        d2_RemoveObj(out[0]);
    if (out[1] != -1)
        d2_RemoveObj(out[1]);
    if (out[2] != -1)
        d2_RemoveObj(out[2]);
    out[0] = -1;
    out[1] = -1;
    out[2] = -1;
}
/* 0x4be930, no controls in out */

void rt_InitControls(int *out)
{
    out[0] = -1;
    out[1] = -1;
    out[2] = -1;
}
/* 0x4be940, the dialog "misc/routing_edit_point" for route point p of the list rt_EditRoute: the goods to load
   (loadSlots) and to unload (unloadSlots), up to three each (by the cart type), with their amounts; a button deletes the
   point and rebuilds the marks on window win
   @deep behaviour: frame 0xb4 vs the original 0xd0 (7 ints of locals are missing: the original spills two more dword locals around +0x58/+0xdc and keeps a copy at +0x18) and the stores are scheduled differently; the draft was made from the disassembly (Ghidra lost the frame) - the locals have to be reread from the original's frame, then the register/order pass. Q2/Q4 stay open */

void rt_Unk4be940(RTPOINT *p, int *win)
{
    int cnt[3];
    int *sel;
    int *selc;
    char *bld;
    unsigned short *bt;
    char *t;
    int w;
    int v2;
    int btns[2];
    int ok;
    int n;
    char *s;
    int a;
    int objs1[3];
    int objs2[3];
    int ctl1[3][3];
    int ctl2[3][3];
    int offs[9];
    int o;
    int i;
    int x;
    int kind;
    int old;
    int g;
    RTPOINT *q;

    cnt[0] = 1;
    cnt[1] = 2;
    cnt[2] = 3;
    sel = 0;
    selc = 0;
    bld = gm_FindBuildingById(p->buildingId);
    bt = gm_FindObjectInst(p->cartId);
    if (bt == 0 || bld == 0)
        return;
    d2_EnableViewObjects(*win, 0);
    t = ai_BTypes + *(unsigned char *)bld * 0x24d;
    w = d2_OpenForm(0, 0, "misc/routing_edit_point");
    d2_CentreViewWindows(w);
    v2 = d2_GetViewWin(w, 2);
    d2_GetViewWin(w, 1);
    d2_SetForm(w, 1);
    io_PrintText((int)"$Z%2N3$A%1G", *(unsigned short *)(bld + 0x27), bld);
    d2_SetForm(w, 3);
    sv_ButtonRow(d2_LastWindow, 0, btns, 2, ai_Unk144ad3c, ai_Unk144ad44);
    ok = btns[0];
    n = cnt[*bt - 0x134];
    s = gm_Query(*(int *)((char *)bt + 0x14), 1, 0, 0x1dd);
    a = gm_ListStock((int)s, 0);
    objs1[0] = -1;
    objs1[1] = -1;
    objs1[2] = -1;
    objs2[0] = -1;
    objs2[1] = -1;
    objs2[2] = -1;
    for (i = 0; i < 3; i++) {
        rt_InitControls(ctl1[i]);
        rt_InitControls(ctl2[i]);
    }
    d2_WinOpenPic(0x6e, 0x1a, *bt + 0x4ae, v2);
    o = d2_CreateWinText(0x6e, 2, ai_Unk144ad28, v2);
    *(int *)(d2_ObjsC + o * 0x2ec + 0x5c) = 1;
    *(short *)(d2_ObjsC + o * 0x2ec + 0x14) = 0xcd;
    d2_WinOpenPic(0x6e, 0x88, *bt + 0x4ae, v2);
    o = d2_CreateWinText(0x6e, 0x70, ai_Unk144ad2c, v2);
    *(int *)(d2_ObjsC + o * 0x2ec + 0x5c) = 1;
    *(short *)(d2_ObjsC + o * 0x2ec + 0x14) = 0xcd;
    if (n > 0) {
        offs[0] = 0x51;
        offs[1] = 0;
        offs[2] = 0;
        offs[3] = 0x30;
        offs[4] = 0x71;
        offs[5] = 0;
        offs[6] = 0x11;
        offs[7] = 0x52;
        offs[8] = 0x93;
        for (i = 0; i < n; i++) {
            x = offs[(*bt - 0x134) * 3 + i] + 0x6e;
            o = d2_WinOpenPic(x, 0x22, 0x525, v2);
            objs1[i] = o;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
            o = d2_WinOpenPic(x, 0x90, 0x525, v2);
            objs2[i] = o;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
        }
    }
    while (sv_RunFrame(rt_Unk4be940, 0x657c7)) {
        if (dm_L.rup != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (rt_EditRoute == 0 || btns[1] == -1 || rt_PointCount(rt_EditRoute) > 1)
            d2_SetObjEnabled(btns[1], 1);
        else
            d2_SetObjEnabled(btns[1], 0);
        for (i = 0; i < 3; i++) {
            if (ctl1[i][1] == -1 && p->loadSlots[i * 2] != 0) {
                rt_Unk4be790(bt, (int)s, p->loadSlots[i * 2], i, 0, 0, v2, ctl1[i]);
                d2_SetObjValue4(ctl1[i][1], 1, a, p->loadSlots[i * 2 + 1] < 1 ? 1 : p->loadSlots[i * 2 + 1]);
            }
            if (ctl1[i][2] != -1)
                d2_SetObjValue4(ctl1[i][2], 0, a, d2_GetObjValue(ctl1[i][1]));
        }
        for (i = 0; i < 3; i++) {
            if (ctl2[i][1] == -1 && p->unloadSlots[i * 2] != 0) {
                rt_Unk4be790(bt, (int)s, p->unloadSlots[i * 2], i, 0, 0x6e, v2, ctl2[i]);
                d2_SetObjValue4(ctl2[i][1], 1, a, p->unloadSlots[i * 2 + 1] < 1 ? 1 : p->unloadSlots[i * 2 + 1]);
            }
            if (ctl2[i][2] != -1)
                d2_SetObjValue4(ctl2[i][2], 0, a, d2_GetObjValue(ctl2[i][1]));
        }
        if (d2_ClickedId != -1) {
            kind = 0;
            for (i = 0; i < 3; i++) {
                if (objs1[i] != -1 && objs1[i] == d2_PointerObj) {
                    kind = 1;
                    sel = &p->loadSlots[i * 2];
                    selc = ctl1[i];
                }
            }
            for (i = 0; i < 3; i++) {
                if (objs2[i] != -1 && objs2[i] == d2_PointerObj) {
                    kind = 2;
                    sel = &p->unloadSlots[i * 2];
                    selc = ctl2[i];
                }
            }
            if (sel != 0) {
                d2_EnableViewObjects(w, 0);
                if (kind == 1) {
                    old = *sel;
                    if (*t == 10)
                        g = rt_Unk4be3b0(bld, 1, old);
                    else
                        g = rt_Unk4bdcd0((unsigned char *)bld, 1, old);
                    *sel = g;
                    if (old != g)
                        rt_RemoveControls(selc);
                } else
                    *sel = rt_Unk4be180((int)bt, rt_EditRoute);
                d2_EnableViewObjects(w, 1);
                sel = 0;
            }
            if (d2_PointerObj == btns[1]) {
                g = rt_FindMark(p);
                if (g != -1)
                    rt_RemoveMark(g);
                rt_ClearMarkGroup();
                rt_BuildMarkGroup();
                if (p == rt_EditRoute)
                    rt_EditRoute = p->next;
                rt_RemovePoint(p);
                for (i = 0; i < 8; i++) {
                    if (rt_Marks[i].obj != -1)
                        rt_RemoveMark(i);
                }
                rt_MarkCount = 0;
                rt_ClearMarkGroup();
                p = rt_EditRoute;
                if (rt_EditRoute != 0) {
                    for (q = rt_EditRoute; q != 0; q = q->next)
                        rt_Unk4bdb30((char *)win, (int)bt, rt_EditRoute, q);
                    p = 0;
                }
                d2_RadioSelect(rt_MarkGroup, 0);
                sv_PopRequest = 1;
            }
        }
    }
    if (p != 0) {
        for (i = 0; i < 3; i++) {
            if (ctl1[i][1] != -1)
                p->loadSlots[i * 2 + 1] = d2_GetObjValue(ctl1[i][1]);
        }
        for (i = 0; i < 3; i++) {
            if (ctl2[i][1] != -1)
                p->unloadSlots[i * 2 + 1] = d2_GetObjValue(ctl2[i][1]);
        }
    }
    d2_CloseView(w);
    d2_EnableViewObjects(*win, 1);
}
/* 0x4bf080, the dialog "misc\routing_settings" for cart c: a 5-step slider (c->unk31), a button that starts a new
   route at c's building (marks on window win), a button that toggles c->unk33; changes go out as AI updates
   @deep behaviour: frame 0x10 vs the original 0x34 (9 dwords of locals missing, the original spills several around +0x14..+0x18 and +0x68) and the argument setup differs (the original reloads the 4 args at +0x74..+0x7c into locals before the call); far draft from the disassembly (Ghidra lost the frame). Locals must be reread from the frame first, then registers/order. Q2/Q4 stay open */

void rt_Unk4bf080(char *c, char *win)
{
    int w;
    int sl;
    int btns[3];
    unsigned char v;

    w = d2_OpenForm(0, 0, "misc\\routing_settings");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 1);
    io_PrintText(0x42e2);
    sl = d2_GetWinObj(w, 2, 0);
    d2_SetForm(w, 4);
    io_PrintText(*(unsigned char *)(c + 0x33) + 0x42ec);
    d2_SetForm(w, 3);
    ai_Unk52e350x(d2_LastWindow, 0, btns, 3, ai_Unk144ad3c, ai_Unk144ad4c, ai_Unk144ad78[*(unsigned char *)(c + 0x33)]);
    d2_SetCaption(btns[2], ai_Unk144ad78[*(unsigned char *)(c + 0x33)]);
    d2_SetObjValue4(sl, 0, 4, *(unsigned char *)(c + 0x31));
    d2_SetSliderFlags(sl, 0x82);
    d2_SetList(sl, 5, ai_Unk144ad54, ai_Unk144ad58, ai_Unk144ad5c, ai_Unk144ad60, ai_Unk144ad64);
    while (sv_RunFrame(rt_Unk4bf080, 0x657c7)) {
        int i;

        if (dm_L.rup != 0 || d2_ClickedId != -1 && d2_PointerObj == btns[0])
            sv_PopRequest = 1;
        d2_SetForm(w, 4);
        io_PrintText(*(unsigned char *)(c + 0x33) + 0x42ec);
        d2_SetCaption(btns[2], ai_Unk144ad78[*(unsigned char *)(c + 0x33)]);
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == btns[1]) {
                if (rt_EditRoute != 0)
                    rt_FreePoints(rt_EditRoute);
                for (i = 0; i < 8; i++) {
                    if (rt_Marks[i].obj != -1)
                        rt_RemoveMark(i);
                }
                rt_MarkCount = 0;
                rt_ClearMarkGroup();
                rt_EditRoute = rt_AddPoint(0);
                rt_EditRoute->buildingId = *(int *)(c + 0x1c);
                rt_EditRoute->cartId = *(int *)(c + 2);
                rt_Unk4bdb30(win, (int)c, rt_EditRoute, rt_EditRoute);
                d2_RadioSelect(rt_MarkGroup, 0);
                sv_PopRequest = 2;
            }
            if (d2_ClickedId != -1 && d2_PointerObj == btns[2]) {
                v = *(char *)(c + 0x33) == 0;
                cm_BeginFieldBlock(c, *(int *)(c + 2));
                cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x33, &v);
                cm_SendFieldValues();
            }
        }
    }
    if ((unsigned int)d2_GetObjValue(sl) != *(unsigned char *)(c + 0x31)) {
        v = (unsigned char)d2_GetObjValue(sl);
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x31, &v);
        cm_SendFieldValues();
    }
    d2_CloseView(w);
    di_ClearButtons();
}
/* 0x4bf360, gives mark m the picture of its point's index (0x7bb + n) @deep Q4 only: the one raw access reads D2OBJ.pic (+8) through rt.h's char* d2_Objs view; a typed field needs d2.h in rt.c, whose declarations conflict with rt.h's views (d2_WinOpenPic int vs short), the recorded rt.h type debt */

void rt_SetMarkPic(RTMARK *m)
{
    int n;

    n = rt_PointIndex(rt_EditRoute, m->p);
    if (*(int *)(d2_ObjsC + m->obj * 0x2ec + 8) != n + 0x7bb)
        d2_SetObjPic(m->obj, n + 0x7bb);
}
/* 0x4bf3b0, 0x4bf360 for each mark */

void rt_UpdateMarkPics(void)
{
    RTMARK *m;

    for (m = rt_Marks; (int)m < (int)(rt_Marks + 8); m++) {
        if (m->obj != -1)
            rt_SetMarkPic(m);
    }
}
/* 0x4bf3e0, the mark of point p, -1 if none */

int rt_FindMark(RTPOINT *p)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (rt_Marks[i].obj != -1 && rt_Marks[i].p == p)
            return i;
    }
    return -1;
}
/* 0x4bf410, removes mark i */

void rt_RemoveMark(int i)
{
    d2_RemoveObj(rt_Marks[i].obj);
    rt_Marks[i].obj = -1;
    rt_Marks[i].p = 0;
}
/* 0x4bf440, on map window m: the building buttons (0x18 bytes each from 0xdc, count at 0x27a4) of buildings other
   than rt_PickedBuilding off; then those of route p's points on
   @deep behaviour: the if/else was checked against the raw disassembly (equal arm: +0x1c0 &= ~1, +0x2c = 1; not-equal: |= 1, = 0 - matches the C) and the rest is scheduling (order 8, regs 8: mov edi,0x1 placement, the mark loop's ebp/eax reload at the end). Q4 (12 raw accesses through the char* d2_Objs view and the map window m+0xdc/+0x27a4) needs typed views - rt.h type debt */

void rt_HighlightRoute(RTPOINT *p, char *m)
{
    int i;
    int *e;
    int found[8];
    int *f;

    for (i = 0, e = (int *)(m + 0xdc); i < *(int *)(m + 0x27a4); i++, e += 6) {
        if (e[1] != 0) {
            if (e[1] == rt_PickedBuilding) {
                *(unsigned int *)(d2_ObjsC + e[0] * 0x2ec + 0x1c0) &= ~1;
                *(int *)(d2_ObjsC + e[0] * 0x2ec + 0x2c) = 1;
            } else {
                *(unsigned int *)(d2_ObjsC + e[0] * 0x2ec + 0x1c0) |= 1;
                *(int *)(d2_ObjsC + e[0] * 0x2ec + 0x2c) = 0;
            }
        }
    }
    if (p != 0) {
        f = found;
        do {
            for (i = 0, e = (int *)(m + 0xe0); i < *(int *)(m + 0x27a4); i++, e += 6) {
                if (*e != 0 && *(int *)(*e + 1) == p->buildingId) {
                    *(int *)(d2_ObjsC + *(int *)(m + 0xdc + i * 0x18) * 0x2ec + 0x2c) = 1;
                    *(unsigned int *)(d2_ObjsC + *(int *)(m + i * 0x18 + 0xdc) * 0x2ec + 0x1c0) &= ~1;
                    *f++ = *(int *)(m + i * 0x18 + 0xe0);
                    break;
                }
            }
            p = p->next;
        } while (p != 0);
    }
}
/* 0x4bf5b0, the route editor for cart c on the town map window (0x5a78a0 / 0x5a7cd0 / 0x5a82f0): a copy of c's
   route (one point at c's building if it has none), a mark per point, buttons to edit the selected point, to add
   the building picked on the map and for the settings (0x4bf080); at the end the route goes to c (0x4c62a0), or
   with fewer than two points c's route mode is reset
   @deep behaviour + Q4: the local map window view is short - the original memsets 0x27dc where our int m[0x9ea] view gives 0x27b8 (the +0x24 shifts every later local) and reads m+0x27a4/+0xdc/+0xe0 through it; the typed map window struct (rt.h type debt) is the fix, then the 306 code hunks can be reread. Far draft from the disassembly (Ghidra lost the frame) */

void rt_EditCartRoute(char *c)
{
    char v;
    int btns[3];
    int m[0x9ea];
    RTPOINT *q;
    int i, k;
    int id;

    rt_PickedBuilding = 0;
    if (c == 0)
        return;
    rt_EditRoute = rt_CopyPoints(*(RTPOINT **)(c + 0x29));
    memset(&m[3], 0, 0x50);
    m[4] = 0x400;
    m[7] = 0;
    m[6] = 0x40000001;
    m[5] = 0xb07d7fb0;
    m[1] = 0x10;
    m[0x16] = 0x723;
    strcpy((char *)&m[0x17], txt_PanelBtnCart);
    ai_Unk5a78a0(m);
    ai_Unk5a6cb0(m);
    rt_Unk6f074c = d2_CopySurface(rt_MapSurface);
    rt_MarkCount = 0;
    for (i = 0; i < 8; i++) {
        rt_Marks[i].obj = -1;
        rt_Marks[i].p = 0;
    }
    if (rt_EditRoute == 0) {
        v = 0;
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x33, &v);
        cm_SendFieldValues();
        rt_EditRoute = rt_AddPoint(0);
        rt_EditRoute->buildingId = *(int *)(c + 0x1c);
        rt_EditRoute->cartId = *(int *)(c + 2);
        rt_Unk4bdb30((char *)m, (int)c, rt_EditRoute, rt_EditRoute);
        d2_RadioSelect(rt_MarkGroup, 0);
        rt_PickedBuilding = 0;
    } else {
        for (q = rt_EditRoute; q != 0; q = q->next)
            rt_Unk4bdb30((char *)m, (int)c, rt_EditRoute, q);
    }
    d2_SetForm(m[0], 2);
    sv_TabButtonRow(d2_LastWindow, 5, btns, 3, ai_Unk144ad30, ai_Unk144ad34, ai_Unk144ad38);
    while (sv_RunFrame(rt_EditCartRoute, 0x657c7)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        ai_Unk62c26c = d2_FocusedInput == 0;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == btns[2])
                rt_Unk4bf080(c, (char *)m);
            else if (d2_PointerObj == btns[0]) {
                if (rt_MarkGroup != -1 && d2_RadioSel[rt_MarkGroup * 0x23] != -1)
                    rt_Unk4be940(rt_Marks[d2_RadioSel[rt_MarkGroup * 0x23]].p, m);
            } else if (d2_PointerObj == btns[1] && rt_PickedBuilding != 0) {
                q = rt_AddPoint(rt_EditRoute);
                q->buildingId = *(int *)(rt_PickedBuilding + 1);
                q->cartId = *(int *)(c + 2);
                rt_Unk4bdb30((char *)m, (int)c, rt_EditRoute, q);
                d2_RadioSelect(rt_MarkGroup, rt_MarkCount - 1);
                rt_PickedBuilding = 0;
            }
        }
        ai_Unk5a7cd0(m);
        if (dm_L.lclick != 0 && d2_PointerObj != -1) {
            for (i = 0; i < m[0x27a4 / 4]; i++) {
                if (m[0xdc / 4 + i * 6] == d2_PointerObj) {
                    rt_PickedBuilding = m[0xe0 / 4 + i * 6];
                    for (k = 0; k < 8; k++) {
                        if (rt_Marks[k].p != 0 && (id = rt_Marks[k].p->buildingId) != -1
                            && rt_PickedBuilding == (int)gm_FindBuildingById(id)) {
                            d2_RadioSelect(rt_MarkGroup, k);
                            rt_PickedBuilding = 0;
                        }
                    }
                    break;
                }
            }
        }
        if (rt_PickedBuilding == 0)
            d2_SetObjEnabled(btns[1], 0);
        else
            d2_SetObjEnabled(btns[1], 1);
        d2_SetObjEnabled(btns[0], rt_PickedBuilding == 0);
        rt_HighlightRoute(rt_EditRoute, (char *)m);
        rt_UpdateMarkPics();
        rt_ReadMarkValues();
    }
    if (rt_PointCount(rt_EditRoute) > 1)
        ai_Unk4c62a0(c, rt_EditRoute);
    else {
        v = 0;
        ai_Unk4c6320(c);
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x33, &v);
        cm_SendFieldValues();
    }
    ai_CloseMapWindow(m);
    if (rt_Unk6f074c != 0) {
        d2_BlitPart(0, 0, *(int *)(rt_MapSurface + 4), *(int *)(rt_MapSurface + 8), rt_Unk6f074c, 0, 0, rt_MapSurface);
        d2_FreeSurface(rt_Unk6f074c);
        rt_Unk6f074c = 0;
    }
    if (rt_MarkGroup != -1) {
        d2_RadioClear(rt_MarkGroup);
        rt_MarkGroup = -1;
    }
    rt_FreePoints(rt_EditRoute);
}
/* 0x4bfa40, cart c unloads at its current route point (c->unk2d): the goods of the point's unloadSlots slots go from the
   cart into the building's stock (market 0x1c: its stock 0x1db, warehouse 10: 0x500ec0), bought by player pl at
   the building's price when it is not the cart owner's
   @deep readability only, byte now: Q4 (10 raw accesses - AIBLD +0x27/+0x49/+0x5d/+0x65, AIOBJ +0x14, RTINV stock, d2_Objs view need typed fields; rt.h's views and the ai.h AIBLD pads) and Q2 (gm_Query/4ffcc0 stock lists, gm_ObjectBuildingOwner/50aa00/50ab80/50aaf0 stock and cart accessors, gm_GoodsPrice price, gm_CharCurrency money, cm_SendExSellObject transfer) - the stock/cart API needs names first */

void rt_UnloadAtPoint(char *c, int pl)
{
    RTPOINT *q;
    char *b;
    char *t;
    short *o;
    char *st;
    unsigned short *g;
    int *s;
    int k;
    int n;
    int price;
    RTINV ci;
    RTINV bi;

    if (*(RTPOINT **)(c + 0x29) == 0)
        return;
    q = rt_PointAt(*(RTPOINT **)(c + 0x29), *(int *)(c + 0x2d));
    b = gm_FindBuildingById(q->buildingId);
    if (b == 0)
        return;
    t = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (*t != 10 && *t != 0x1c && !gm_BuildingGoodsList(b, &bi, 0))
        return;
    o = gm_ObjectBuildingOwner(c);
    if (!gm_StallGoodsList(c, &ci))
        return;
    s = q->unloadSlots;
    for (k = 3; k != 0; k--) {
        if (s[0] != 0) {
            if (*t == 10)
                st = gm_FindMarketObject(s[0]);
            else if (*t == 0x1c)
                st = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1db, 6);
            else
                st = bi.stock;
            g = gm_Query(*(int *)(ci.stock + 0x14), 1, 0, s[0]);
            if (g != 0) {
                n = s[1];
                if (n >= gm_ListAmount(ci.stock, g))
                    n = gm_ListAmount(ci.stock, g);
                n = gm_FreeCapacity(st, s[0], n);
                if (n > 0) {
                    price = 0;
                    if (*(short *)(b + 0x27) != *o) {
                        if (*t == 10)
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * 1.1f);
                        else
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * *(float *)(b + 0x49));
                    }
                    if (gm_CharCurrency(o, pl) >= price * n)
                        cm_SendExSellObjectI(*(int *)(st + 2), *(int *)(ci.stock + 2), s[0], n, pl, price);
                }
            }
        }
        s += 2;
    }
}
/* 0x4bfc60, cart c loads at its current route point: the goods of the point's loadSlots slots go from the building's
   stock (market: 0x1dc) into the cart, paid by player pl (a message when pl cannot afford it)
   @deep behaviour: the price factors are fixed (1.1f / *(float*)(b+0x49)); what remains is the local frame: the original zeroes each RTINV local as rep stos 0x2d dwords (0xb4) plus a separate dword store at +0x15c/+0x24 where we zero 0x2e dwords, and the stores/registers differ (67 code hunks, slots 16, order 8). The RTINV zeroing seam suggests the original's local is not the plain RTINV view (rt.h type debt). Q2/Q4 stay open */

void rt_Unk4bfc60(char *c, char pl)
{
    RTPOINT *q;
    short *o;
    char *b;
    char *t;
    char *st;
    unsigned short *g;
    int *s;
    int k;
    int n;
    int price;
    RTINV ci;
    char name[0x80];
    RTINV bi;
    char buf[0x400];

    memset(&bi, 0, sizeof(bi));
    memset(&ci, 0, sizeof(ci));
    if (*(RTPOINT **)(c + 0x29) == 0)
        return;
    q = rt_PointAt(*(RTPOINT **)(c + 0x29), *(int *)(c + 0x2d));
    o = gm_ObjectBuildingOwner(c);
    b = gm_FindBuildingById(q->buildingId);
    if (b == 0)
        return;
    t = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (*t != 10 && *t != 0x1c && !gm_BuildingGoodsList(b, &bi, 0) || !gm_StallGoodsList(c, &ci))
        return;
    s = q->loadSlots;
    for (k = 3; k != 0; k--) {
        if (s[0] != 0) {
            price = 0;
            if (*t == 10)
                st = gm_FindMarketObject(s[0]);
            else if (*t == 0x1c)
                st = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1dc, 6);
            else
                st = bi.stock;
            g = gm_Query(*(int *)(st + 0x14), 1, 0, s[0]);
            if (g != 0) {
                n = s[1];
                if (n >= gm_ListAmount(st, g))
                    n = gm_ListAmount(st, g);
                n = gm_FreeCapacity(ci.stock, s[0], n);
                if (*(short *)(b + 0x27) != *o) {
                    if (*t == 10)
                        price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * 1.1f);
                    else if (*t == 0x1c)
                        price = (int)(gm_GoodsPrice(*g, b[0x65]) * 1.1f);
                    else
                        price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * *(float *)(b + 0x49));
                }
                if (n > 0) {
                    if (gm_CharCurrency(o, pl) < n * price) {
                        io_TownDisplayName((char *)ai_PlayerSlots + pl * 0x2f4, name);
                        txt_Format(buf, 0x42f0, name, 0xc14, name, pl);
                        he_ShowNews(*(int *)(o + 2), -1, buf, 0, 0x5ef, ch_EmptyName);
                    }
                    cm_SendExSellObjectI(*(int *)(ci.stock + 2), *(int *)(st + 2), s[0], n, pl, price);
                }
            }
        }
        s += 2;
    }
}
/* 0x4bff40, saves the routes of all carts (0x4ffe90(0, 2, 7, 4, 0x1d)): the number of points, then each point @deep behaviour: all 15 code hunks are the five vfs_fwrite tests (ours test eax,eax; je return 0 from 'if (!vfs_fwrite(...))', the original test eax,eax; ja body from a '> 0' test with jump-to-body layout) plus the q null test the original keeps separate (mov eax,[eax+0x29]; test; je before moving esi); the statements and the saved order (cartId, buildingId, loadSlots 0x18, unloadSlots 0x18) are the original's. Rewriting the five guards as '> 0' needs the restructured layout - deep. Q2 (gm_Query/4ffcc0, vfs_fwrite) and Q4 (c+0x29 raw) stay open */

int rt_SaveRoutes(void *f)
{
    char *c;
    RTPOINT *q;
    int n;

    n = 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        if (*(RTPOINT **)(c + 0x29) != 0)
            n += rt_PointCount(*(RTPOINT **)(c + 0x29));
    }
    if (!vfs_fwrite(&n, 4, 1, f))
        return 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        for (q = *(RTPOINT **)(c + 0x29); q != 0; q = q->next) {
            if (!vfs_fwrite(&q->cartId, 4, 1, f))
                return 0;
            if (!vfs_fwrite(&q->buildingId, 4, 1, f))
                return 0;
            if (!vfs_fwrite(q->loadSlots, 0x18, 1, f))
                return 0;
            if (!vfs_fwrite(q->unloadSlots, 0x18, 1, f))
                return 0;
        }
    }
    return 1;
}
/* 0x4c0040, loads the routes (0x4bff40) into the carts (their id in cartId, consecutive points per cart) @deep behaviour: 168 code hunks, all shifts of the same stream - frame 0xc vs the original 0x8 (one extra int local), esi/ebx/ebp register allocation throughout and the load loop's pointer arithmetic (add esi,0x8 vs lea esi,[ebx+0x8]); the statements (n = fread; buf = mem_Alloc(n << 6, "rt_load"); the four freads per point; the cart chain via rt_AddPoint with cartId/buildingId/slots) match the disassembly. Needs frame/register steering. Q2 (ls_Read, gm_FindObjectInst) and Q4 (c+0x29 / c+2 raw) stay open */

int rt_LoadRoutes(void *f)
{
    RTPOINT *buf;
    RTPOINT *r;
    RTPOINT *q;
    char *c;
    int i;
    int n;

    if (!ls_Read(&n, 4, 1, f))
        return 0;
    buf = mem_Alloc(n << 6, "rt_load");
    for (i = 0; i < n; i++) {
        if (!ls_Read(&buf[i].cartId, 4, 1, f))
            return 0;
        if (!ls_Read(&buf[i].buildingId, 4, 1, f))
            return 0;
        if (!ls_Read(buf[i].loadSlots, 0x18, 1, f))
            return 0;
        if (!ls_Read(buf[i].unloadSlots, 0x18, 1, f))
            return 0;
    }
    for (i = 0; i < n; ) {
        r = &buf[i];
        c = gm_FindObjectInst(r->cartId);
        *(RTPOINT **)(c + 0x29) = 0;
        do {
            q = rt_AddPoint(*(RTPOINT **)(c + 0x29));
            if (*(RTPOINT **)(c + 0x29) == 0)
                *(RTPOINT **)(c + 0x29) = q;
            q->buildingId = r->buildingId;
            q->cartId = r->cartId;
            i++;
            memcpy(q->loadSlots, r->loadSlots, sizeof(q->loadSlots));
            memcpy(q->unloadSlots, r->unloadSlots, sizeof(q->unloadSlots));
            r++;
            if (i >= n)
                goto done;
        } while (r != 0 && *(int *)(c + 2) == r->cartId);
    }
done:
    mem_Free(buf);
    return 1;
}
