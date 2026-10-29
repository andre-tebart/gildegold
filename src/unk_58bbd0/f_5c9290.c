/* 0x5c9290, the blackmail privilege handler "privillegien_erpressung2" 0x62d4c4 (695 B, ROUGH): two
   records param_1 (the AIMSG: +0 the id, +2 the type, +4 the handle, +0x1c8 the flags) and param_2 (the
   target: +0 the id, +4 the handle); null or type not 6 / 7 -> 0. The window 0x4741f0 / 0x476f80 /
   0x475600, the texts 0x515960 (0x1d57 with the target id four times and 2, 0x1d58); the rank 0x4f4080
   (target handle, out, id). Click 0: 0x42ec00 (8) against the rank -> the message 0x4c4b00 (handles,
   0xffffffe6, 1), the close 0x4c5f60 (-2) and the text 0x5176d0 (0x1d59) / 0x521150, return 0; else the
   clock record 0x770ae0 .. into the 0x1d8 record with 0x4fe140 (0x60) and 0x4c4b70, the wait 0x4c3ae0,
   the reply 0x4c3b10 (+0x70), the flag wait and the text 0x1d5a, return 1. Click 1 -> 0. The pump
   0x5309a0 and the end 0x4758f0. ROUGH: the record fields by offset. */

int unk_Unk4f4080();

int ai_Unk5c9290(unsigned short *param_1, unsigned short *param_2)
{
    char buf[256];
    char name[128];
    int time[4];
    int rec[0x3e];
    int win;
    int h;
    int i;
    int rank;
    int ret;
    unsigned int r;

    if (param_1 == 0 || param_2 == 0)
        return 0;
    if (*(char *)((char *)param_1 + 2) != 6 && *(char *)((char *)param_1 + 2) != 7)
        return 0;
    rank = unk_Unk4f4080(*(int *)((char *)param_2 + 4), name, *param_1);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d4c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d57, *param_2, *param_2, *param_2, *param_2, 2);
    io_PrintText(0x1d58);
    *(int *)(unsigned)&d2_ClickedId = -1;
    ret = 0;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                r = util_Random(8) & 0xffff;
                if (rank < (int)r) {
                    cm_SendRelations(*(int *)((char *)param_1 + 4), *(int *)((char *)param_2 + 4),
                                 0xffffffe6, 1);
                    cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffffe);
                    txt_Format(buf, (char *)0x1d59, *param_2);
                    sv_MessageBox(buf, 0);
                    ret = 0;
                    *(int *)(unsigned)&sv_PopRequest = 1;
                } else {
                    time[0] = *(int *)(unsigned)&ai_Time;
                    time[1] = *(int *)(unsigned)&ai_TimeHour;
                    time[2] = *(int *)((unsigned)&ai_Time + 0x8);
                    *(short *)&time[3] = *(short *)((unsigned)&ai_Time + 0xc);
                    gm_AddTime(time, 0x60, 0, 0);
                    for (i = 0; i < 0x3e; i++)
                        rec[i] = 0;
                    rec[0] = 0;                    /* +0 */
                    rec[1] = *(int *)((char *)param_1 + 4);   /* +4 */
                    rec[4] = *(int *)(unsigned)&ai_Time;     /* +0x10 */
                    rec[5] = *(int *)(unsigned)&ai_TimeHour;
                    rec[6] = *(int *)((unsigned)&ai_Time + 0x8);
                    rec[7] = *(int *)((char *)param_2 + 4);   /* +0x1c */
                    *(char *)&rec[8] = 0x45;       /* +0x20 */
                    *(char *)&rec[9] = 2;          /* +0x24 */
                    *(short *)((char *)rec + 0x26) = *(short *)((unsigned)&ai_Time + 0xc);
                    rec[10] = rec[1];              /* +0x28 */
                    rec[11] = rec[1];              /* +0x2c */
                    h = cm_SendLongMessage(rec);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                    }
                    h = cm_GetSlotValue(h);
                    if ((*(unsigned int *)((char *)param_1 + 0x1c8) & 0x100) == 0) {
                        do {
                            if (*(int *)((char *)h + 0x70) == -1)
                                break;
                            sv_PumpNet();
                        } while ((*(unsigned int *)((char *)param_1 + 0x1c8) & 0x100) == 0);
                        if ((*(unsigned int *)((char *)param_1 + 0x1c8) & 0x100) != 0)
                            goto answered;
                    } else {
answered:
                        txt_Format(buf, (char *)0x1d5a, *param_2, time);
                        sv_MessageBox(buf, 0);
                    }
                    ret = 1;
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                ret = 0;
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_Unk5c9290, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
