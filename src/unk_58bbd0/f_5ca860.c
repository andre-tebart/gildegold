/* 0x5ca860, the "privillegien\unterschlagung" 0x62d538 window (932 B): param_1 is the message record (the
   AIMSG by word offsets: +0 the id, +2 the type 6, +4 the handle, +0x194 the amount), param_2 the request
   (dword +0: 3 (the message) or 4 (with +0x21c the owner), +0x214 the target id). The owner id into the
   0x4fa700 record (id, 0, the record); the price (int)((float)((0x42ec00 (0xb0) & 0xffff) + 0x19) *
   0x5eceb0 * the record +4). Types other than 6: the amount < 4 returns 0x20; else 0x4c4830 (the handle,
   -1, 0, the price), 0x4c5f60 (-4) and 0x4c4a60 (owner +4, 0x1c8, 4, 0x400000), return 0. Type 6: the
   window 0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 with the text 0x515960 (0x1dc2, the id); a click
   0x520f90 (4) pays, closes, waits 0x4c3ae0 / 0x52fe00, shows 0x5176d0 (0x1dc3, the price) / 0x521150 and
   sets 0x6f22a0; the pump 0x5309a0 (this, 0x657c7) and the end 0x4758f0, return 0x90. ROUGH. */

int ai_MisappropriationWindow(unsigned short *param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int rec[2];
    char text[256];
    int owner;
    int price;
    int win;
    int ret;
    int h;

    rec[0] = 0;
    rec[1] = 0;
    ret = 0;
    if (*param_2 == 3) {
        owner = (int)param_1;
    } else {
        if (*param_2 != 4)
            return 0x60;
        owner = param_2[0x87];
        if (owner == 0)
            return 0x60;
    }
    gm_CharacterWages(*(unsigned short *)owner, 0, (unsigned char *)rec);
    price = (int)((float)((util_Random(0xb0) & 0xffff) + 0x19) * (0.00499999989f) * rec[1]);
    if (*(char *)((char *)param_1 + 2) != 6) {
        if (*(int *)((char *)param_1 + 0x194) < 4)
            return 0x20;
        cm_SendMoveGoodsUnchecked(*(int *)((char *)param_1 + 4), 0xffffffff, 0, price);
        cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffffc);
        cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, 0x400000, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d538);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1dc2, *(unsigned short *)param_1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                if (sv_CheckMoney(param_1, 4) != 0) {
                    cm_SendMoveGoodsUnchecked(*(int *)((char *)param_1 + 4), 0xffffffff, 0, price);
                    cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffffc);
                    h = cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, 0x400000, 0);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    txt_Format(text, (char *)0x1dc3, price);
                    sv_MessageBox(text, 0);
                    ret = 0x90;
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_MisappropriationWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
