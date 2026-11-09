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
#include "d3.h"

int rt_MarkGroup = -1;                  /* 0x602a90 */
int ai_MapScrollEnabled = 1;  /* 0x62c26c */
extern RTMARK rt_Marks[8];
/* txt_InfoPanelButtons: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PanelBtnCart: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteLoadHead: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteUnloadHead: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteEditPoint: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteAddPoint: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteSettings: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteOk: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteDeletePoint: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_RouteNew: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolLevel0: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolLevel1: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolLevel2: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolLevel3: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolLevel4: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_PatrolToggle: view of txt_Ptrs (build/mk_txt_views.py) */
struct D2SURF *rt_MapSaved = 0;  /* 0x6f074c, rt_EditCartRoute's copy of rt_MapSurface, blitted back at its end */
int rt_MarkCount = 0;  /* 0x6f0750, the marks used */
int rt_PickedBuilding = 0;  /* 0x6f0754, the building shown */
RTPOINT * rt_EditRoute = 0;  /* 0x6f0758, the route being edited */
struct D2SURF *rt_MapSurface = 0;  /* 0x6f26ac, the screen surface */
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
/* 0x4bdb30, puts the mark of the route point p (the picture 0x7bb + its index in list) on the town map window win
   at its building (RTPOINT.buildingId: the building object's height map cell, ai_MapCellToScreen around the own
   player's town origin) and adds it to the radio group rt_MarkGroup (made with the first mark); returns its
   control, -1 when the building or its cell is not found. cart is not read */
int rt_PutRouteMark(LCMAPWIN *win, int cart, RTPOINT *list, RTPOINT *p)
{
    D3SM *hmap;
    int originX;
    int originY;
    AIBLD *b;
    float pos[4];           /* d3_PointToWorld writes the angles after the position */
    LCMAPMARK mark;
    float y;
    int o;
    int index;

    hmap = (D3SM *)d3_Scenes[0].hmap;
    originX = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townX;
    originY = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].townY;
    b = (AIBLD *)gm_FindBuildingById(p->buildingId);
    if (b == 0)
        return -1;
    d3_PointToWorld((D3OBJECT *)b->object, &((D3OBJECT *)b->object)->pos, (D3VECTOR *)pos);
    if (!d3_GetHeightAt(hmap, (D3VECTOR *)pos, &mark.cellX, &mark.cellZ, &y))
        return -1;
    ai_MapCellToScreen(&mark, originX, originY);
    index = rt_PointIndex(list, p);
    o = d2_WinOpenPic((int)mark.x - 0x20, (int)mark.y - 0x20, index + 0x7bb, win->screen);
    rt_Marks[rt_MarkCount].obj = o;
    d2_Objs[o].checkable = 1;
    if (rt_MarkGroup == -1)
        rt_MarkGroup = d2_CreateRadioGroup(1, rt_Marks[rt_MarkCount].obj);
    else
        d2_RadioAdd(rt_MarkGroup, rt_Marks[rt_MarkCount].obj);
    rt_Marks[rt_MarkCount].p = p;
    return rt_Marks[rt_MarkCount++].obj;
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
/* 0x4bdcd0, the dialog "misc\routing_choose_good_enhanced" (title text 0x7d): the goods of the building b to pick
   from, four to a row, 0x40 apart: of a market (type kind 0x1c) the goods its stall 0x1dc holds more than one of,
   of a warehouse (kind 9) the goods its store 0x116 holds any of, else the goods gm_BuildKindObjects lists that
   its object 0xff holds more than one of (with their names); then "none" (picture 0x527, the entry 0). Runs the
   frame loop until a pick or a right click; returns the goods picked (0: none), def when nothing was picked.
   x is not read */
int rt_ChooseGoodsDialog(AIBLD *b, int x, int def)
{
    int ret;
    AIOBJ *store;
    int objs[10];
    AIBTYPE *t;
    int w;
    AIOBJ *item;
    int n;
    int i;
    int o;

    ret = def;
    t = &((AIBTYPE *)ai_BTypes)[b->type];
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 4);
    d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 2);
    if (t->kind == 0x1c) {
        store = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x1dc, 6);
        n = 0;
        if (store != 0) {
            for (item = (AIOBJ *)gm_Query((void *)store->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
                if (item->count > 1) {
                    o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, item->id + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    d2_Objs[o].interactive = 1;
                    d2_Objs[o].style = 3;
                    d2_Objs[o].icon = 0x525;
                    gm_KindObjs[n] = item->id;
                    n++;
                }
            }
        }
    } else if (t->kind == 9) {
        store = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0x116, 6);
        n = 0;
        if (store != 0) {
            for (item = (AIOBJ *)gm_Query((void *)store->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
                if (item->count >= 1) {
                    o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, item->id + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    d2_Objs[o].interactive = 1;
                    d2_Objs[o].style = 3;
                    d2_Objs[o].icon = 0x525;
                    gm_KindObjs[n] = item->id;
                    n++;
                }
            }
        }
    } else {
        gm_BuildKindObjects((char *)b);
        store = (AIOBJ *)gm_Query((void *)b->objList, 2, 0, 0xff, 6);
        n = 0;
        for (i = 0; i < gm_KindObjCount; i++) {
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, gm_KindObjs[n]);
            if (item != 0 && item->count > 1) {
                o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, gm_KindObjs[n] + 0xdf, d2_LastWindow);
                objs[n] = o;
                d2_Objs[o].interactive = 1;
                d2_Objs[o].style = 0x23;
                d2_Objs[o].icon = 0x525;
                d2_Objs[o].captionX = 4;
                d2_Objs[o].captionY = 0xf;
                sprintf(d2_Objs[o].caption, "%s", (char *)txt_ObjectNames[gm_KindObjs[n]][0]);
                n++;
            }
        }
    }
    gm_KindObjCount = (unsigned short)n;
    gm_KindObjs[n] = 0;
    o = d2_WinOpenPic(n % 4 * 0x40 + 7, n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[n] = o;
    d2_Objs[o].interactive = 1;
    d2_Objs[o].style = 3;
    while (sv_RunFrame(rt_ChooseGoodsDialog, 0x657c7)) {
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
   loadSlots[2 * k] with a count above 0); returns the good picked, 0 if none */

int rt_ChooseRouteGood(int x, RTPOINT *p)
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
        d2_Objs[o].interactive = 1;
        d2_Objs[o].style = 3;
        d2_Objs[o].icon = 0x525;
    }
    goods[i] = 0;
    o = d2_WinOpenPic(i % 4 * 0x40 + 7, i / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[i] = o;
    d2_Objs[o].interactive = 1;
    d2_Objs[o].style = 3;
    while (sv_RunFrame(rt_ChooseRouteGood, 0x657c7)) {
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
/* 0x4be3b0, the dialog "misc\routing_choose_good_enhanced" for building b with seven tabs (txt_InfoPanelButtons;
   each keeps the building's store of stock kind 0x95, 0x92, 0x93, 0x94, 0x97, 0x96, 0x2db in its object's user
   field; title "$Z%2N3$A%1G" with the owner): the goods the store of the tab picked holds (the building's own
   object 0xff when it has none), four to a row, then "none" (picture 0x527). Runs until a pick, a right click or
   the button "ok" (text 0x7d); returns the good picked, def if none. x is not read */

unsigned int rt_ChooseTabGood(AIBLD *b, int x, unsigned int def)
{
    int first;
    int last;
    int kinds[7];
    int objs[0x30];
    unsigned int goods[0x30];
    int tabs[0x4c];     /* seven used; the size is computed from the original's frame (0x2e4, tabs last above goods) */
    unsigned int ret;
    int w;
    int ok;
    int g;
    int i;
    int y;
    AIOBJ *s;
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
    io_PrintText((int)"$Z%2N3$A%1G", b->owner, b);
    d2_SetForm(w, 4);
    ok = d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 3);
    g = d2_CreateRadioGroup(0);
    for (i = 0, y = 4; y < 0xd6; i++, y += 0x1e) {
        o = sv_AddPanelButton(0, y, txt_InfoPanelButtons[i]);
        tabs[i] = o;
        d2_Objs[o].user = gm_Query((void *)b->objList, 2, 6, 0, kinds[i]);
        d2_RadioAdd(g, o);
    }
    d2_RadioSelect(g, 0);
    d2_ClearInput();
    for (;;) {
        if (dm_L.rup != 0 || ret != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (d2_Radios[g].sel != last || first) {
            d2_SetForm(w, 2);
            d2_RemoveWindowObjects(d2_GetViewWin(w, 2), 0);
            for (i = 0; i < 0x30; i++)
                objs[i] = -1;
            n = 0;
            s = (AIOBJ *)d2_Objs[tabs[d2_Radios[g].sel]].user;
            if (s != 0)
                s = (AIOBJ *)gm_Query((void *)s->list, 1, 0, 0xff);
            else
                s = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xff);
            if (s != 0) {
                for (e = (unsigned short *)gm_Query((void *)s->list, 1, 5); e != 0; e = gm_QueryNext()) {
                    o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, *e + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    d2_Objs[o].interactive = 1;
                    d2_Objs[o].style = 3;
                    d2_Objs[o].icon = 0x525;
                    goods[n] = *e;
                    n++;
                }
            }
            goods[n] = 0;
            o = d2_WinOpenPic(n % 4 * 0x40 + 7, n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
            objs[n] = o;
            first = 0;
            d2_Objs[o].interactive = 1;
            d2_Objs[o].style = 3;
            last = d2_Radios[g].sel;
        }
        if (d2_ClickedId != -1) {
            for (i = 0; i < 0x30; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = goods[i];
                    break;
                }
            }
        }
        if (!sv_RunFrame(rt_ChooseTabGood, 0x657c7))
            break;
    }
    d2_RadioClear(g);
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
/* 0x4be790, the controls of one good slot (k 0..2) of a route point: a slider (gm_ListStock(a, good) steps) and the
   good's picture with its name; out = {-1, picture, slider}; returns the picture */

int rt_OpenSlotControls(AIOBJ *cart, int a, int good, int k, int x, int y, int win, int *out)
{
    int offs[9];
    int s;
    int o;
    int px;
    int py;

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
    px = offs[(cart->id - 0x134) * 3 + k] + 0x6e + x;
    py = y + 0x22;
    s = d2_WinOpenSlider(px - 8, py - 5, 0x2f, 0, s, 0x6a, 0x2d, win);
    o = d2_WinOpenInputIcon(px, py, good + 0xdf, 0x8a, win);
    d2_SetObjFont(o, 0x54);
    out[0] = -1;
    out[1] = o;
    out[2] = s;
    d2_Objs[o].style = 0x22;
    d2_Objs[o].captionX = 4;
    d2_Objs[o].captionY = 0xf;
    sprintf(d2_Objs[o].caption, "%s", (char *)txt_ObjectNames[good][0]);
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
   @deep read whole against the original: fixed the delete-point loop (p walks the list, ends 0), the btns
   copy into ok/del locals, btns[8] (frame 0xd0 now equal), the typed AIBLD/AIOBJ/AIBTYPE/D2OBJ accesses.
   Left: allocation only - after gm_ListStock the original holds -1 in two registers (+0x147 'or edx,-1'
   then objs1 stores from edx, objs2 from eax; ours one 'or eax,-1' for both), so the rt_InitControls loop
   picks ecx/eax instead of eax/ecx (regs eax->edx edx->ecx), and two order hunks at +0x4e8/+0x523 (mov
   edi,1/2 scheduled before vs after mov ebx,ecx). Tried chained assignment of objs1/objs2 (worse). */

void rt_EditPointDialog(RTPOINT *p, int *win)
{
    int cnt[3];
    int *sel;
    int *selc;
    AIBLD *bld;
    AIOBJ *cart;
    AIBTYPE *t;
    int w;
    int v2;
    int btns[8];
    int ok;
    int del;
    int n;
    AIOBJ *s;
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

    cnt[0] = 1;
    cnt[1] = 2;
    cnt[2] = 3;
    sel = 0;
    selc = 0;
    bld = (AIBLD *)gm_FindBuildingById(p->buildingId);
    cart = (AIOBJ *)gm_FindObjectInst(p->cartId);
    if (cart == 0 || bld == 0)
        return;
    d2_EnableViewObjects(*win, 0);
    t = &((AIBTYPE *)ai_BTypes)[bld->type];
    w = d2_OpenForm(0, 0, "misc/routing_edit_point");
    d2_CentreViewWindows(w);
    v2 = d2_GetViewWin(w, 2);
    d2_GetViewWin(w, 1);
    d2_SetForm(w, 1);
    io_PrintText((int)"$Z%2N3$A%1G", bld->owner, bld);
    d2_SetForm(w, 3);
    sv_ButtonRow(d2_LastWindow, 0, btns, 2, txt_RouteOk, txt_RouteDeletePoint);
    ok = btns[0];
    del = btns[1];
    n = cnt[cart->id - 0x134];
    s = (AIOBJ *)gm_Query((void *)cart->list, 1, 0, 0x1dd);
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
    d2_WinOpenPic(0x6e, 0x1a, cart->id + 0x4ae, v2);
    o = d2_CreateWinText(0x6e, 2, txt_RouteLoadHead, v2);
    d2_Objs[o].centred = 1;
    d2_Objs[o].w = 0xcd;
    d2_WinOpenPic(0x6e, 0x88, cart->id + 0x4ae, v2);
    o = d2_CreateWinText(0x6e, 0x70, txt_RouteUnloadHead, v2);
    d2_Objs[o].centred = 1;
    d2_Objs[o].w = 0xcd;
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
            x = offs[(cart->id - 0x134) * 3 + i] + 0x6e;
            o = d2_WinOpenPic(x, 0x22, 0x525, v2);
            objs1[i] = o;
            d2_Objs[o].interactive = 1;
            d2_Objs[o].style = 3;
            o = d2_WinOpenPic(x, 0x90, 0x525, v2);
            objs2[i] = o;
            d2_Objs[o].interactive = 1;
            d2_Objs[o].style = 3;
        }
    }
    while (sv_RunFrame(rt_EditPointDialog, 0x657c7)) {
        if (dm_L.rup != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (rt_EditRoute == 0 || del == -1 || rt_PointCount(rt_EditRoute) > 1)
            d2_SetObjEnabled(del, 1);
        else
            d2_SetObjEnabled(del, 0);
        for (i = 0; i < 3; i++) {
            if (ctl1[i][1] == -1 && p->loadSlots[i * 2] != 0) {
                rt_OpenSlotControls(cart, (int)s, p->loadSlots[i * 2], i, 0, 0, v2, ctl1[i]);
                d2_SetObjValue4(ctl1[i][1], 1, a, p->loadSlots[i * 2 + 1] < 1 ? 1 : p->loadSlots[i * 2 + 1]);
            }
            if (ctl1[i][2] != -1)
                d2_SetObjValue4(ctl1[i][2], 0, a, d2_GetObjValue(ctl1[i][1]));
        }
        for (i = 0; i < 3; i++) {
            if (ctl2[i][1] == -1 && p->unloadSlots[i * 2] != 0) {
                rt_OpenSlotControls(cart, (int)s, p->unloadSlots[i * 2], i, 0, 0x6e, v2, ctl2[i]);
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
                    if (t->kind == 10)
                        g = rt_ChooseTabGood(bld, 1, old);
                    else
                        g = rt_ChooseGoodsDialog(bld, 1, old);
                    *sel = g;
                    if (old != g)
                        rt_RemoveControls(selc);
                } else
                    *sel = rt_ChooseRouteGood((int)cart, rt_EditRoute);
                d2_EnableViewObjects(w, 1);
                sel = 0;
            }
            if (d2_PointerObj == del) {
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
                for (p = rt_EditRoute; p != 0; p = p->next)
                    rt_PutRouteMark((LCMAPWIN *)win, (int)cart, rt_EditRoute, p);
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
/* 0x4bf080, the dialog "misc\routing_settings" for cart c (title text 0x42e2): a 5-step slider for c->patrolLevel
   (txt_PatrolLevel0..4), the button txt_RouteNew that starts a new route at c's building (its mark on the town map
   window win), the button txt_PatrolToggle[c->patrolOn] that toggles c->patrolOn (text 0x42ec + patrolOn shows the
   state); changes go out as field updates */

void rt_CartSettingsDialog(AIOBJ *c, char *win)
{
    int w;
    int sl;
    int ok;
    int renew;
    int toggle;
    int btns[10];     /* three used; the size is computed from the original's frame (0x34) */
    unsigned char v;
    int i;

    w = d2_OpenForm(0, 0, "misc\\routing_settings");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 1);
    io_PrintText(0x42e2);
    sl = d2_GetWinObj(w, 2, 0);
    d2_SetForm(w, 4);
    io_PrintText(c->patrolOn + 0x42ec);
    d2_SetForm(w, 3);
    sv_ButtonRow(d2_LastWindow, 0, btns, 3, txt_RouteOk, txt_RouteNew, txt_PatrolToggle[c->patrolOn]);
    ok = btns[0];
    renew = btns[1];
    toggle = btns[2];
    d2_SetCaption(toggle, txt_PatrolToggle[c->patrolOn]);
    d2_SetObjValue4(sl, 0, 4, c->patrolLevel);
    d2_SetSliderFlags(sl, 0x82);
    d2_SetList(sl, 5, txt_PatrolLevel0, txt_PatrolLevel1, txt_PatrolLevel2, txt_PatrolLevel3, txt_PatrolLevel4);
    while (sv_RunFrame(rt_CartSettingsDialog, 0x657c7)) {
        if (dm_L.rup != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        d2_SetForm(w, 4);
        io_PrintText(c->patrolOn + 0x42ec);
        d2_SetCaption(toggle, txt_PatrolToggle[c->patrolOn]);
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == renew) {
                if (rt_EditRoute != 0)
                    rt_FreePoints(rt_EditRoute);
                for (i = 0; i < 8; i++) {
                    if (rt_Marks[i].obj != -1)
                        rt_RemoveMark(i);
                }
                rt_MarkCount = 0;
                rt_ClearMarkGroup();
                rt_EditRoute = rt_AddPoint(0);
                rt_EditRoute->buildingId = c->bldId;
                rt_EditRoute->cartId = c->objId;
                rt_PutRouteMark((LCMAPWIN *)win, (int)c, rt_EditRoute, rt_EditRoute);
                d2_RadioSelect(rt_MarkGroup, 0);
                sv_PopRequest = 2;
            }
            if (d2_ClickedId != -1 && d2_PointerObj == toggle) {
                v = c->patrolOn == 0;
                cm_BeginFieldBlock((char *)c, c->objId);
                cm_AddFieldValue(1, 1, (char *)c - cm_FieldObject + 0x33, &v);
                cm_SendFieldValues();
            }
        }
    }
    if ((unsigned int)d2_GetObjValue(sl) != c->patrolLevel) {
        v = (unsigned char)d2_GetObjValue(sl);
        cm_BeginFieldBlock((char *)c, c->objId);
        cm_AddFieldValue(1, 1, (char *)c - cm_FieldObject + 0x31, &v);
        cm_SendFieldValues();
    }
    d2_CloseView(w);
    di_ClearButtons();
}
/* 0x4bf360, gives mark m the picture of its point's index (0x7bb + n) */

void rt_SetMarkPic(RTMARK *m)
{
    int n;

    n = rt_PointIndex(rt_EditRoute, m->p);
    if (d2_Objs[m->obj].pic != n + 0x7bb)
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
/* 0x4bf440, on town map window m: the marks of the buildings other than rt_PickedBuilding plain, its mark
   highlighted; then the marks of route p's points highlighted (found[] collects their buildings, unread) */

void rt_HighlightRoute(RTPOINT *p, LCMAPWIN *m)
{
    int i;
    LCMAPMARK *mk;
    AIBLD *found[8];
    AIBLD **f;
    RTPOINT *q;

    for (i = 0; i < m->nMarks; i++) {
        mk = &m->marks[i];
        if (mk->bld != 0) {
            if ((int)mk->bld != rt_PickedBuilding) {
                d2_Objs[mk->ctrl].style |= 1;
                d2_Objs[mk->ctrl].highlighted = 0;
            } else {
                d2_Objs[mk->ctrl].style &= ~1;
                d2_Objs[mk->ctrl].highlighted = 1;
            }
        }
    }
    q = p;
    if (q != 0) {
        f = found;
        do {
            for (i = 0, mk = m->marks; i < m->nMarks; i++, mk++) {
                if (mk->bld != 0 && mk->bld->id == q->buildingId) {
                    d2_Objs[m->marks[i].ctrl].highlighted = 1;
                    d2_Objs[m->marks[i].ctrl].style &= ~1;
                    *f++ = m->marks[i].bld;
                    break;
                }
            }
            q = q->next;
        } while (q != 0);
    }
}
/* 0x4bf5b0, the route editor for cart c on the town map window (ai_OpenTownMap / ai_UpdateTownMap /
   ai_CloseMapWindow; title txt_PanelBtnCart): a copy of c's route (one point at c's building if it has none, and
   c->patrolOn set), a mark per point, the tab buttons txt_RouteEditPoint (rt_EditPointDialog on the selected
   point), txt_RouteAddPoint (adds the building picked on the map) and txt_RouteSettings (rt_CartSettingsDialog); at
   the end the route goes to c (cm_SendSetRoute), or with fewer than two points c's route is cleared and
   c->patrolOn reset. rt_MapSurface is saved before and restored after */

void rt_EditCartRoute(AIOBJ *c)
{
    unsigned char v;
    int add;
    int settings;
    int btns[9];      /* three used; the size is computed from the original's frame (0x27dc) */
    LCMAPWIN m;
    RTPOINT *q;
    int edit;
    int i, k;
    int id;

    rt_PickedBuilding = 0;
    if (c == 0)
        return;
    rt_EditRoute = rt_CopyPoints(c->route);
    memset(m.req, 0, sizeof(m.req));
    m.req[1] = 0x400;
    m.req[4] = 0;
    m.req[3] = 0x40000001;
    m.req[2] = 0xb07d7fb0;
    m.flags = 0x10;
    m.req[0x13] = 0x723;
    strcpy(m.name, txt_PanelBtnCart);
    ai_OpenTownMap(&m);
    ai_BuildTownMap(&m);
    rt_MapSaved = d2_CopySurface(rt_MapSurface);
    rt_MarkCount = 0;
    for (i = 0; i < 8; i++) {
        rt_Marks[i].obj = -1;
        rt_Marks[i].p = 0;
    }
    if (rt_EditRoute == 0) {
        v = 1;
        cm_BeginFieldBlock((char *)c, c->objId);
        cm_AddFieldValue(1, 1, (char *)c - cm_FieldObject + 0x33, &v);
        cm_SendFieldValues();
        rt_EditRoute = rt_AddPoint(0);
        rt_EditRoute->buildingId = c->bldId;
        rt_EditRoute->cartId = c->objId;
        rt_PutRouteMark(&m, (int)c, rt_EditRoute, rt_EditRoute);
        d2_RadioSelect(rt_MarkGroup, 0);
        rt_PickedBuilding = 0;
    } else {
        for (q = rt_EditRoute; q != 0; q = q->next)
            rt_PutRouteMark(&m, (int)c, rt_EditRoute, q);
    }
    d2_SetForm(m.view, 2);
    sv_TabButtonRow(d2_LastWindow, 5, btns, 3, txt_RouteEditPoint, txt_RouteAddPoint, txt_RouteSettings);
    edit = btns[0];
    add = btns[1];
    settings = btns[2];
    while (sv_RunFrame(rt_EditCartRoute, 0x657c7)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        ai_MapScrollEnabled = d2_FocusedInput == 0;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == settings)
                rt_CartSettingsDialog(c, (char *)&m);
            else if (d2_PointerObj == edit) {
                if (rt_MarkGroup != -1 && d2_Radios[rt_MarkGroup].sel != -1)
                    rt_EditPointDialog(rt_Marks[d2_Radios[rt_MarkGroup].sel].p, (int *)&m);
            } else if (d2_PointerObj == add && rt_PickedBuilding != 0) {
                q = rt_AddPoint(rt_EditRoute);
                q->buildingId = ((AIBLD *)rt_PickedBuilding)->id;
                q->cartId = c->objId;
                rt_PutRouteMark(&m, (int)c, rt_EditRoute, q);
                d2_RadioSelect(rt_MarkGroup, rt_MarkCount - 1);
                rt_PickedBuilding = 0;
            }
        }
        ai_UpdateTownMap(&m);
        if (dm_L.lclick != 0 && d2_PointerObj != -1) {
            for (i = 0; i < m.nMarks; i++) {
                if (m.marks[i].ctrl == d2_PointerObj) {
                    rt_PickedBuilding = (int)m.marks[i].bld;
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
        if (rt_PickedBuilding != 0) {
            d2_SetObjEnabled(add, 1);
            d2_SetObjEnabled(edit, 0);
        } else {
            d2_SetObjEnabled(add, 0);
            d2_SetObjEnabled(edit, 1);
        }
        rt_HighlightRoute(rt_EditRoute, &m);
        rt_UpdateMarkPics();
        rt_ReadMarkValues();
    }
    if (rt_PointCount(rt_EditRoute) > 1)
        cm_SendSetRoute(c, rt_EditRoute);
    else {
        v = 0;
        cm_SendClearRoute(c);
        cm_BeginFieldBlock((char *)c, c->objId);
        cm_AddFieldValue(1, 1, (char *)c - cm_FieldObject + 0x33, &v);
        cm_SendFieldValues();
    }
    ai_CloseMapWindow((int *)&m);
    if (rt_MapSaved != 0) {
        d2_BlitPart(0, 0, rt_MapSurface->w, rt_MapSurface->h, rt_MapSaved, 0, 0, rt_MapSurface);
        d2_FreeSurface(rt_MapSaved);
        rt_MapSaved = 0;
    }
    if (rt_MarkGroup != -1) {
        d2_RadioClear(rt_MarkGroup);
        rt_MarkGroup = -1;
    }
    rt_FreePoints(rt_EditRoute);
}
/* 0x4bfa40, cart c unloads at its current route point (c->unk2d): the goods of the point's unloadSlots slots go from the
   cart into the building's stock (market 0x1c: its stock 0x1db, warehouse 10: 0x500ec0), bought by player pl at
   the building's price when it is not the cart owner's */

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

    if (((AIOBJ *)c)->route == 0)
        return;
    q = rt_PointAt(((AIOBJ *)c)->route, ((AIOBJ *)c)->routeIdx);
    b = gm_FindBuildingById(q->buildingId);
    if (b == 0)
        return;
    t = ai_BTypes + ((AIBLD *)b)->type * 0x24d;
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
                st = gm_Query(((AIBLD *)b)->objList, 2, 0, 0x1db, 6);
            else
                st = bi.stock;
            g = gm_Query(((AIOBJ *)ci.stock)->list, 1, 0, s[0]);
            if (g != 0) {
                n = s[1];
                if (n >= gm_ListAmount(ci.stock, g))
                    n = gm_ListAmount(ci.stock, g);
                n = gm_FreeCapacity(st, s[0], n);
                if (n > 0) {
                    price = 0;
                    if ((short)((AIBLD *)b)->owner != *o) {
                        if (*t == 10)
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * 1.1f);
                        else
                            price = (int)(gm_GoodsPrice(*g, ai_OwnPlayer) * ((AIBLD *)b)->priceFactor);
                    }
                    if (gm_CharCurrency(o, pl) >= price * n)
                        cm_SendExSellObjectI(((AIOBJ *)st)->objId, ((AIOBJ *)ci.stock)->objId, s[0], n, pl, price);
                }
            }
        }
        s += 2;
    }
}
/* 0x4bfc60, cart c loads at its current route point: the goods of the point's loadSlots slots go from the building's
   stock (market: 0x1dc) into the cart, paid by player pl (a message when pl cannot afford it) */

void rt_LoadAtPoint(char *c, char pl)
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
/* 0x4bff40, saves the routes of all carts (0x4ffe90(0, 2, 7, 4, 0x1d)): the number of points, then each point */

int rt_SaveRoutes(void *f)
{
    AIOBJ *c;
    RTPOINT *q;
    int n;

    n = 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        if (c->route != 0)
            n += rt_PointCount(c->route);
    }
    if (vfs_fwrite(&n, 4, 1, f) <= 0)
        return 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        if (c->route != 0) {
            for (q = c->route; q != 0; q = q->next) {
                if (vfs_fwrite(&q->cartId, 4, 1, f) <= 0)
                    return 0;
                if (vfs_fwrite(&q->buildingId, 4, 1, f) <= 0)
                    return 0;
                if (vfs_fwrite(q->loadSlots, 0x18, 1, f) <= 0)
                    return 0;
                if (vfs_fwrite(q->unloadSlots, 0x18, 1, f) <= 0)
                    return 0;
            }
        }
    }
    return 1;
}
/* 0x4c0040, loads the routes (0x4bff40) into the carts (their id in cartId, consecutive points per cart) */

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
