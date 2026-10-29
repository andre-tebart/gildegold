/* 0x5c7f00, the statistics panel window "panel_statistik_stadt" 0x62d440 (1520 B, ROUGH SKELETON): the gate
   0x5763c0 and the state step 0x576040 (0xc, 0, 0, 0); the window 0x4741f0 / 0x476f80 / 0x475600 and the
   page setup 0x515960 (0x1f0e with the player record 0x7e3f60 + 0x6f1bc5 * 0x2f4 and the clock 0x770ae0,
   0x1f14 .. 0x1f22 with the value / rank texts); the chart values of 0x4f9e00 and the charts 0x5c7970 with
   the flag word 0x6f2f88 (the screen 0x6ed938); the five legend rows (0x4731d0, the text 0x5176d0, the
   control 0x473650 with the record 0xf8f080 + id * 0x2ec (+0x48 / +0x4c = 1), the toggle 0x475cb0, and ten
   colour lines 0x4764f0 per row from the 14-byte colour table 0x33 / 0x10 / 0xee); the pump 0x5309a0 and
   the flag rebuild 0x475980 over the five ids. ROUGH: the label section (0x4f9e00 args, 0x5176d0 ids) and
   the pump arguments are sketched from the pseudocode. */

float unk_Unk4f9e00(int *o);

void ai_Unk5c7f00(void)
{
    unsigned char colors[14];
    int vals[10];
    int ids[5];
    char buf[0x100];
    int win;
    int i;
    int k;
    int id;
    int y;
    int h;
    int flag;
    float f;

    colors[0] = 0x33;
    colors[1] = 0x33;
    colors[2] = 0x10;
    colors[3] = 0xee;
    colors[4] = 0x10;
    colors[5] = 0xee;
    colors[6] = 0x10;
    colors[7] = 0x10;
    colors[8] = 0xee;
    colors[9] = 0xee;
    colors[10] = 0;
    colors[11] = 0xee;
    colors[12] = 0x10;
    colors[13] = 0xee;
    if (*(unsigned short *)(unsigned)&full_data_6f2f88 == 0)
        *(unsigned short *)(unsigned)&full_data_6f2f88 = 1;
    if (!ai_Unk5763c0(0x10))
        return;
    ai_Unk576040(0xc, 0, 0, 0);
    win = d2_OpenForm((char *)(unsigned)&full_data_62d440, 0, 0);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f0e, ((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4, (unsigned)&ai_Time);
    f = unk_Unk4f9e00(vals);
    d2_SetForm(win, 2);
    io_PrintText(0x1f14, (int)*(float *)(unsigned)&ai_RatingBase);
    io_PrintText(0x1f15, min(4, (int)((f + *(double *)(unsigned)&full_rdata_5eb8a8) * (2.5))) + 0x1f16);
    io_PrintText(0x1f1b, min(4, (int)(f * *(float *)(unsigned)&ai_Unk5eb988)) + 0x1f1c);
    io_PrintText(0x1f21, (int)f);
    io_PrintText(0x1f22, (int)f);
    d2_SetForm(win, 3);
    ai_Unk5c7970(*(unsigned short *)(unsigned)&full_data_6f2f88, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5],
                 vals[6], vals[7], vals[8], vals[9], *(int *)(unsigned)&d2_LastWindow);
    d2_SetForm(win, 4);
    h = *(short *)((unsigned)&ai_Unkf8f128 + *(int *)(unsigned)&d2_LastWindow * 0x3c4);
    for (i = 0; i < 5; i++) {
        id = d2_WinOpenPic(5, i * 0x1e + 5, i + 0x1f25, *(int *)(unsigned)&d2_LastWindow);
        txt_Format(buf, i + 0x1f25);
        y = d2_CreateWinText(*(int *)(unsigned)&d2_LastWindow, 0x19, i * 0x1e + 4);
        *(int *)((unsigned)&d2_Objs + id * 0x2ec + 0x4c) = 1;
        *(int *)((unsigned)&d2_Objs + id * 0x2ec + 0x48) = 1;
        d2_SetObjValue(id, (*(unsigned short *)(unsigned)&full_data_6f2f88 & (1 << i)) != 0);
        for (k = 0; k < 10; k++) {
            d2_PlotLine(h - 1, y + k, h - 0xb, y + k, colors[i * 3], colors[i * 3 + 1],
                         colors[i * 3 + 2]);
        }
        ids[i] = id;
    }
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            *(unsigned short *)(unsigned)&full_data_6f2f88 = 0;
            for (i = 0; i < 5; i++) {
                if (d2_GetObjValue(ids[i]))
                    *(unsigned short *)(unsigned)&full_data_6f2f88 |= (1 << i);
            }
            d2_SetForm(win, 3);
            ai_Unk5c7970(*(unsigned short *)(unsigned)&full_data_6f2f88, vals[0], vals[1], vals[2], vals[3], vals[4],
                         vals[5], vals[6], vals[7], vals[8], vals[9], *(int *)(unsigned)&d2_LastWindow);
        }
        if (*(int *)((unsigned)&dm_L + 0x20))
            *(int *)(unsigned)&sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Unk5c7f00, 0x657c7) != 0);
    d2_CloseView(win);
    ai_Unk576040(0xc, 0, 1, 0);
}
