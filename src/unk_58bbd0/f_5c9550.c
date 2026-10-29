/* 0x5c9550, the blackmail building listener "privillegien_erpressung" 0x62d4e0 (1082 B, ROUGH): a message
   record param_1 (the AIMSG: +0 the id, +2 the type, +4 the handle) and param_2 (the building: +0x214 the
   target id). Types 6 / 7 open the window 0x4741f0 and its texts 0x515960 (0x1d55) / 0x472270; the candidate
   list 0x4f3dd0 (out ids / ranks / keys of the target's characters 0x505df0), the rows 0x5be1d0 /
   0x5be3b0 (0x32, y, screen, 0x56) and 0x5bd190 (the text 0x62cd9c "%i. %i (%s)", h - 0xf, y + 0x2d, 200,
   0x55); a click 0x520f90 (2) either warns 0x521150 (0x1441704) / 0x1d52 or calls the blackmail handler
   0x5c9290 (the recurse flag 0x92), setting 0x6f22a0. Other types: the rank 0x4f4080 against 0x42ec00 (8)
   pays 0x4c4b00 (0xffffffe6) and returns 0x10, else the clock record 0x770ae0 .. with 0x4fe140 (0x60) /
   0x4c4b70; no target returns 0x60. The pump 0x5309a0 and the end 0x4758f0. ROUGH SKELETON: the candidate
   records (stride 0xe ints: +0 the character, +0xb the key, +0xc = 1, +0xd the rank) by offset. */

int ai_BlackmailBuildingListener(unsigned short *param_1, int param_2)  /* @original: the callers push (c, m) (0x4f5190); the draft body reads its own offsets of the two params */
{
    int ids[32];
    int ranks[32];
    int keys[32];
    int hits[11 * 0xe];
    char names[64];
    char text[512];
    int time[4];
    int rec[0x3e];
    int win;
    int h;
    int i;
    int n;
    int cnt;
    int y;
    int id;
    int ret;
    unsigned int r;
    int *p;
    int target;

    if (*(char *)((char *)param_1 + 2) != 6 && *(char *)((char *)param_1 + 2) != 7) {
        target = (int)gm_FindChar(*(int *)((char *)param_2 + 0x214));
        if (target == 0)
            return 0x60;
        ids[0] = unk_Unk4f4080(*(int *)(target + 4), names, *param_1);
        r = util_Random(8) & 0xffff;
        if (ids[0] < (int)r) {
            cm_SendRelations(*(int *)((char *)param_1 + 4), *(int *)(target + 4), 0xffffffe6, 1);
            return 0x10;
        }
        time[0] = *(int *)(unsigned)&ai_Time;
        time[1] = *(int *)(unsigned)&ai_TimeHour;
        time[2] = *(int *)((unsigned)&ai_Time + 0x8);
        *(short *)&time[3] = *(short *)((unsigned)&ai_Time + 0xc);
        gm_AddTime(time, 0x60, 0, 0);
        for (i = 0; i < 0x3e; i++)
            rec[i] = 0;
        rec[1] = *(int *)((char *)param_1 + 4);
        rec[4] = *(int *)(unsigned)&ai_Time;
        rec[5] = *(int *)(unsigned)&ai_TimeHour;
        rec[6] = *(int *)((unsigned)&ai_Time + 0x8);
        *(short *)((char *)rec + 0x1a) = *(short *)((unsigned)&ai_Time + 0xc);
        rec[7] = *(int *)(target + 4);
        *(char *)&rec[8] = 0x45;
        *(char *)&rec[9] = 2;
        *(short *)((char *)rec + 0x26) = *(short *)((unsigned)&ai_Time + 0xc);
        rec[10] = rec[1];
        rec[11] = rec[1];
        cm_SendLongMessage(rec);
        return 0x10;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d4e0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1d55);
    d2_SetForm(win, 2);
    id = *(int *)(unsigned)&d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, *(int *)(unsigned)&d2_LastWindow, id, 0x767);
    cnt = unk_Unk4f3dd0(ids, ranks, keys, *param_1);
    n = 0;
    for (i = 0; i < cnt; i++) {
        h = (int)gm_FindChar(ids[i]);
        if (h != 0) {
            p = hits + n * 0xe;
            p[0] = h;
            p[0xb] = keys[i];
            p[0xd] = ranks[i];
            ai_AmountRowStatus(p);
            p[0xc] = 1;
            n++;
        }
    }
    d2_SetForm(win, 2);
    if (n == 0) {
        io_PrintText(0x1d50);
    } else if (n > 0) {
        y = 10;
        for (i = 0; i < n; i++) {
            p = hits + i * 0xe;
            txt_Format(names, (char *)(unsigned)&full_data_62cd9c, p[0], p[2], (p[0] != 1) + 0x1748);
            p[-7] = 0x32;
            p[-6] = y;
            ai_AmountRow(0x32, y, *(int *)(unsigned)&d2_LastWindow, 0x56, p - 0xb, 1);
            ai_Unk5bd190(names, *(short *)((unsigned)&ai_Unkf8f128 + *(int *)(unsigned)&d2_LastWindow * 0x3c4) - 0xf, y + 0x2d, 200,
                         0x55);
            y += 0x69;
        }
    }
    ret = 0x20;
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1 && n > 0) {
            for (i = 0; i < n; i++) {
                if (*(int *)(unsigned)&ai_Unk5fe710 == hits[i * 0xe + 1]) {
                    if (sv_CheckMoney(param_1, 2) != 0) {
                        if ((*(unsigned int *)((char *)hits[i * 0xe] + 0xe4) & 0x200) == 0) {
                            if (*(int *)(hits + i * 0xe + 4) < 3) {
                                sv_MessageBox((char *)((unsigned)&ai_MessageFilterTexts + 0x204), 0);
                            } else {
                                ret = (int)gm_FindChar(ids[i]);
                                if (ai_Unk5c9290(param_1, (unsigned short *)ret) != 0)
                                    *(int *)(unsigned)&sv_PopRequest = 1;
                                ret = 0x92;
                            }
                        } else {
                            txt_Format(text, (char *)0x1d52, *(short *)hits[i * 0xe]);
                            sv_MessageBox(text, 0);
                        }
                    }
                    break;
                }
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_BlackmailBuildingListener, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
