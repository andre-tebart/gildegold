/* 0x5cd2e0, the espionage overview order handler (282 B): param_1 is the message record (the AIMSG: +2
   the type 6 / 7, +4 the handle, +0x194 the amount). Type 7 returns 0; other types: the amount < 4
   returns 0x20, else the close 0x4c5f60 (-4) and the overview 0x5ccc10 (the message, 0x42ec00 (6) &
   0xffff), return 0x10. Type 6: the window 0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 with the caption
   0x515960 (0x1df6, 4); click 0 (0x520f90 (4)) closes 0x4c5f60 (-4) and runs 0x5ccc10 (0x42ec00 (6) &
   0xffff), return 1; click 1 sets 0x6f22a0; the pump 0x5309a0 (this, 0x657c7) and the end 0x4758f0. */

int ai_EspionageOverviewHandler(int param_1, int *m)  /* m: the handler's second argument (unused here): the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int win;
    int ret;
    int v;

    if (*(char *)(param_1 + 2) != 6) {
        if (*(char *)(param_1 + 2) == 7)
            return 0;
        if (*(int *)(param_1 + 0x194) < 4)
            return 0x20;
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffc);
        ai_Unk5ccc10(param_1, util_Random(6) & 0xffff);
        return 0x10;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d64c);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1df6, 4);
    *(int *)(unsigned)&d2_ClickedId = -1;
    ret = 0;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                if (sv_CheckMoney(param_1, 4) != 0) {
                    cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffc);
                    ai_Unk5ccc10(param_1, util_Random(6) & 0xffff);
                    ret = 1;
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_EspionageOverviewHandler, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
