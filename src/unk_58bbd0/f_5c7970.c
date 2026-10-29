/* 0x5c7970, the statistics chart window (1422 B, ROUGH): the flag mask param_1 selects the five bar charts
   0x5c72d0 / 0x5c7460 / 0x5c7800 / 0x5c7560 / 0x5c76b0 (bits 1 .. 0x10), each called with the ten values
   (param_2 .. param_11) and the two screen words (height - 1, width) of the screen record 0xf8f120 +
   param_12 * 0x3c4; the frame 0x4764f0 (0x2d, 8, 0x2d, width - 0x13 / height - 0x3b); the grid lines
   (0x28, i, 0x2d, i) of the four rows; the axis labels of the buffer (0x5176d0 with 0x62d43c) and the
   control records 0xf8f080 + id * 0x2ec (+0x14 = 0x23, +0x60 = 1) placed by 0x473650. The tail (rough):
   the time label of ai_Time 0x770ae0 via 0x4fe2c0 and the fifteen rows of labels (0x5176d0 with 0x5fe7d0,
   0x46ea10, 0x473650, 0x4764f0). The window busy flag 0x6ed97c is saved and restored. ROUGH SKELETON: the
   label / control section of the second half is only sketched. */

int d2_TextWidth();

void ai_Unk5c7970(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6,
                  int param_7, int param_8, int param_9, int param_10, int param_11, int param_12)
{
    char old;
    int ret;
    int h;
    int w;
    int scale;
    int x;
    int i;
    int c;
    int id;
    char buf[0x64];
    float f;

    old = *(char *)(unsigned)&d2_PenSize;
    ret = 0;
    d2_ClearPaintbox();
    d2_RemoveWindowObjects(param_12, 1);
    *(char *)(unsigned)&d2_PenSize = 1;
    h = *(short *)((unsigned)&ai_Unkf8f128 + param_12 * 0x3c4);
    w = *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + param_12 * 0x3c4);
    scale = (h - 0x69) / 16;
    if (param_1 & 1)
        ret = ai_Unk5c72d0(param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9,
                           param_10, param_11, h - 1, w);
    *(char *)(unsigned)&d2_PenSize = 1;
    if (param_1 & 2)
        ai_Unk5c7460(param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10,
                     param_11, h - 1, w);
    if (param_1 & 4)
        ai_Unk5c7800(param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10,
                     param_11, h - 1, w);
    if (param_1 & 8)
        ai_Unk5c7560(param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10,
                     param_11, h - 1, w);
    if (param_1 & 0x10)
        ai_Unk5c76b0(param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10,
                     param_11, h - 1, w);
    *(char *)(unsigned)&d2_PenSize = 0;
    d2_PlotLine(0x2d, 8, 0x2d, w - 0x13, 0x10, 0x10, 0x10);
    d2_PlotLine(0x2d, w - 0x13, h - 0x3b, w - 0x13, 0x10, 0x10, 0x10);
    f = (float)param_2 - *(float *)(unsigned)&ai_Unk5eb81c;
    for (i = 0; i < 4; i++) {
        c = (int)((float)i * 0.25f * f + (8.0f));
        d2_PlotLine(0x28, c, 0x2d, c, 0x10, 0x10, 0x10);
    }
    i = 0;
    for (x = 0x64; x > 0; x -= 0x19) {
        txt_Format(buf, (char *)(unsigned)&full_data_62d43c, x);
        c = (int)((float)i * 0.25f * f + *(float *)(unsigned)&ai_Unk5eb988);
        id = d2_CreateWinText(3, c);
        *(unsigned short *)(*(int *)(unsigned)&d2_Objs + id * 0x2ec + 0x14) = 0x23;
        *(int *)(*(int *)(unsigned)&d2_Objs + id * 0x2ec + 0x60) = 1;
        i++;
    }
    if (ret) {
        d2_PlotLine(h - 0x3c, 8, h - 0x3c, w - 0x13, 0x10, 0x10, 0x10);
        d2_PlotLine(h - 0x37, 8, h - 0x3c, 8, 0x10, 0x10, 0x10);
        d2_PlotLine(h - 0x37, (w - 0x1e) / 2 + 8, h - 0x3c, (w - 0x1e) / 2 + 8, 0x10, 0x10, 0x10);
        /* the labels of the chart values: 0x5176d0 (0x5fe7d0) + 0x46ea10 into the 0xf8f080 records,
           0x5fe7d0 twice with the half width, then the remaining rows as in 0x5c7c23 .. 0x5c7cea */
        txt_Format(buf, (char *)(unsigned)"%i", 0);
        d2_TextWidth(buf, 0x55);
        d2_CreateWinText(5, h - 0x3c, (w - 0x1e) / 2 + 5);
    }
    /* the date label: the 14 byte ai_Time 0x770ae0 copied to a local HETIME and formatted by 0x4fe2c0,
       then the fifteen rows (0x5c7db9 .. 0x5c7e78): 0x4fe2c0 per row, 0x473650 (0x2d + i * scale, ...)
       0x4764f0, 0x5176d0 (0x5fe7d0) / 0x46ea10, and the last row of 0x5c7e7e */
    *(char *)(unsigned)&d2_PenSize = old;
}
