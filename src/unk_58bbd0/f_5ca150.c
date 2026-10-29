/* 0x5ca150, the law / privilege window "privillegien_gesetz" 0x62d518 (1019 B, ROUGH): param_1 is the
   message record (AIMSG: +2 the type, +4 the handle, +0x194 the amount), param_2 a record (dword +0: 3
   (the message's id) or 4 (with +0x21c the owner), +4 the law id, +8 the value, +0x214 the mode 1 / 2 (the
   flags 0x80000 / 0x100000)). The law info 0x4f2720 (id, out: the category byte and the three bounds
   min / max / current) and the setter 0x4f2750. Type 6 opens the window 0x4741f0 with the text 0x515960
   (0x1ba9) and one of four button pairs 0x515960 (0x13d0 / 0x13da / 0x13e3 / 0x13e9 with the category
   0x13fd + law * 5 and the current value offsets 0x13dd / 0x13ec) / 0x475b80; the slider 0x475cb0
   (min, max, current), the state 0x475980 / 0x475e40; a click (0x520f90 (2)) applies 0x4f2750 (the value -
   the current), waits 0x4c3ae0, closes 0x4c5f60 (-2) and reopens the carrier 0x4c4a60 (owner handle, 0x1c8,
   4, the flag). Other types check the bounds and apply the change, return 0 / 0x20 / 0x60 / 0x80. The pump
   0x5309a0 and the end 0x4758f0. ROUGH: the info record by offset. */

int gv_SetTableRecord(short *c, int i, int v);   /* 0x4f2750 */

int ai_LawPrivilegeWindow(int param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    unsigned char info[16];
    int ctrls[2];
    int win;
    int owner;
    int apply;
    int flag;
    int h;
    int t;
    int v;
    int i;
    int cat;
    int cur;
    int minv;
    int maxv;
    int base;
    int ret;

    ret = 0;
    if (*param_2 == 3) {
        owner = param_1;
    } else {
        if (*param_2 != 4)
            return 0x60;
        owner = param_2[0x87];
        if (owner == 0)
            return 0x60;
    }
    if (param_2[0x85] == 1)
        flag = 0x80000;
    else if (param_2[0x85] == 2)
        flag = 0x100000;
    else
        return 0x60;
    if (*(char *)(param_1 + 2) != 6) {
        if (*(int *)(param_1 + 0x194) < 2)
            return 0x20;
        if (gv_GetTableRecord(param_2[1], info) == 0)
            return 0x60;
        cur = *(int *)(info + 4);
        minv = *(int *)(info + 8);
        maxv = *(int *)(info + 0xc);
        if (param_2[2] == cur || maxv < param_2[2] || param_2[2] < minv)
            return 0x60;
        gv_SetTableRecord(param_1, info[0], param_2[2]);
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffe);
        cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, flag, 0);
        if (*(char *)(param_1 + 2) != 6)
            return 0;
    }
    for (i = 0; i < 16; i++)
        info[i] = 0;
    if (gv_GetTableRecord(param_2[1], info) == 0)
        return 0;
    cat = info[1];
    cur = *(int *)(info + 4);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d518);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1ba9);
    base = 0;
    apply = 1;
    switch (cat) {
    case 0:
        t = io_PrintText(0x13d0, info[0] * 5 + 0x13fd, 0x13d3);
        ctrls[0] = d2_GetWinObj(win, 1, t);
        ctrls[1] = d2_GetWinObj(win, 1, t + 1);
        d2_SetObjValue(ctrls[0], minv, maxv, cur);
        base = 0;
        break;
    case 1:
        t = io_PrintText(0x13da, info[0] * 5 + 0x13fd, 0x13dd, cur + 0x13dd);
        ctrls[0] = d2_GetWinObj(win, 1, t);
        ctrls[1] = d2_GetWinObj(win, 1, t + 1);
        base = 0x13dd;
        break;
    case 2:
        t = io_PrintText(0x13e3, info[0] * 5 + 0x13fd, info[0] * 5 + 0x13fd, 0x13e7 - cur);
        ctrls[0] = d2_GetWinObj(win, 1, t);
        ctrls[1] = -1;
        apply = 0;
        break;
    case 3:
        t = io_PrintText(0x13e9, 0x13ec, cur + 0x13ec);
        ctrls[0] = d2_GetWinObj(win, 1, t);
        ctrls[1] = d2_GetWinObj(win, 1, t + 1);
        base = 0x13ec;
        break;
    default:
        return 0;
    }
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (apply) {
            v = d2_GetObjValue(ctrls[0]);
            d2_SetObjEnabled(ctrls[1], (v - base == cur) || ctrls[1] == -1 ? 0 : 1);
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                if (sv_CheckMoney((char *)param_1, 2) != 0) {
                    if (cat == 2) {
                        v = 1;
                        base = *(int *)(info + 4);
                    } else {
                        v = d2_GetObjValue(ctrls[0]);
                    }
                    ret = 0x80;
                    h = ((int (*)(int, int, int))gv_SetTableRecord)(param_1, info[0], v - base);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffe);
                        cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, flag, 0);
                    }
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (sv_RunFrame(ai_LawPrivilegeWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
