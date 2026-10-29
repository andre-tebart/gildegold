/* 0x4bf5b0, the route editor for cart c on the town map window (0x5a78a0 / 0x5a7cd0 / 0x5a82f0): a copy of c's
   route (one point at c's building if it has none), a mark per point, buttons to edit the selected point, to add
   the building picked on the map and for the settings (0x4bf080); at the end the route goes to c (0x4c62a0), or
   with fewer than two points c's route mode is reset
   @deep behaviour + Q4: the local map window view is short - the original memsets 0x27dc where our int m[0x9ea] view gives 0x27b8 (the +0x24 shifts every later local) and reads m+0x27a4/+0xdc/+0xe0 through it; the typed map window struct (rt.h type debt) is the fix, then the 306 code hunks can be reread. Far draft from the disassembly (Ghidra lost the frame, notes/units/rt.md) */

void rt_Unk4bf5b0(char *c)
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
    strcpy((char *)&m[0x17], ai_Unk144ad24);
    ai_Unk5a78a0(m);
    ai_Unk5a6cb0(m);
    rt_Unk6f074c = d2_CopySurface(ai_Unk6f26ac);
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
    sv_Unk52e4b0(d2_LastWindow, 5, btns, 3, ai_Unk144ad30, ai_Unk144ad34, ai_Unk144ad38);
    while (sv_RunFrame(rt_Unk4bf5b0, 0x657c7)) {
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
    ai_Unk5a82f0(m);
    if (rt_Unk6f074c != 0) {
        d2_BlitPart(0, 0, *(int *)(ai_Unk6f26ac + 4), *(int *)(ai_Unk6f26ac + 8), rt_Unk6f074c, 0, 0, ai_Unk6f26ac);
        d2_FreeSurface(rt_Unk6f074c);
        rt_Unk6f074c = 0;
    }
    if (rt_MarkGroup != -1) {
        d2_RadioClear(rt_MarkGroup);
        rt_MarkGroup = -1;
    }
    rt_FreePoints(rt_EditRoute);
}
