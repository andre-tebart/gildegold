/* 0x4be940, the dialog "misc/routing_edit_point" for route point p of the list rt_EditRoute: the goods to load
   (loadSlots) and to unload (unloadSlots), up to three each (by the cart type), with their amounts; a button deletes the
   point and rebuilds the marks on window win
   @deep behaviour: frame 0xb4 vs the original 0xd0 (7 ints of locals are missing: the original spills two more dword locals around +0x58/+0xdc and keeps a copy at +0x18) and the stores are scheduled differently; the draft was made from the disassembly (Ghidra lost the frame, notes/units/rt.md) - the locals have to be reread from the original's frame, then the register/order pass. Q2/Q4 stay open */

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
    sv_Unk52e350(d2_LastWindow, 0, btns, 2, ai_Unk144ad3c, ai_Unk144ad44);
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
    *(int *)(d2_Objs + o * 0x2ec + 0x5c) = 1;
    *(short *)(d2_Objs + o * 0x2ec + 0x14) = 0xcd;
    d2_WinOpenPic(0x6e, 0x88, *bt + 0x4ae, v2);
    o = d2_CreateWinText(0x6e, 0x70, ai_Unk144ad2c, v2);
    *(int *)(d2_Objs + o * 0x2ec + 0x5c) = 1;
    *(short *)(d2_Objs + o * 0x2ec + 0x14) = 0xcd;
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
            *(int *)(d2_Objs + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_Objs + o * 0x2ec + 0x1c0) = 3;
            o = d2_WinOpenPic(x, 0x90, 0x525, v2);
            objs2[i] = o;
            *(int *)(d2_Objs + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_Objs + o * 0x2ec + 0x1c0) = 3;
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
                d2_SetObjValue(ctl1[i][1], 1, a, p->loadSlots[i * 2 + 1] < 1 ? 1 : p->loadSlots[i * 2 + 1]);
            }
            if (ctl1[i][2] != -1)
                d2_SetObjValue(ctl1[i][2], 0, a, d2_GetObjValue(ctl1[i][1]));
        }
        for (i = 0; i < 3; i++) {
            if (ctl2[i][1] == -1 && p->unloadSlots[i * 2] != 0) {
                rt_Unk4be790(bt, (int)s, p->unloadSlots[i * 2], i, 0, 0x6e, v2, ctl2[i]);
                d2_SetObjValue(ctl2[i][1], 1, a, p->unloadSlots[i * 2 + 1] < 1 ? 1 : p->unloadSlots[i * 2 + 1]);
            }
            if (ctl2[i][2] != -1)
                d2_SetObjValue(ctl2[i][2], 0, a, d2_GetObjValue(ctl2[i][1]));
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
