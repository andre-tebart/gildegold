/* 0x4bf080, the dialog "misc\routing_settings" for cart c: a 5-step slider (c->unk31), a button that starts a new
   route at c's building (marks on window win), a button that toggles c->unk33; changes go out as AI updates
   @deep behaviour: frame 0x10 vs the original 0x34 (9 dwords of locals missing, the original spills several around +0x14..+0x18 and +0x68) and the argument setup differs (the original reloads the 4 args at +0x74..+0x7c into locals before the call); far draft from the disassembly (Ghidra lost the frame, notes/units/rt.md). Locals must be reread from the frame first, then registers/order. Q2/Q4 stay open */

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
    d2_SetObjValue(sl, 0, 4, *(unsigned char *)(c + 0x31));
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
