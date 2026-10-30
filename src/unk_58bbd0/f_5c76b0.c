/* 0x5c76b0, a bar chart of the 0x91ab08 / 0x91ab1c float pairs (325 B): the rectangles 0x4764f0 (x, y1,
   x + scale, y2, 0xee, 0x10, 0xee) between the consecutive floats 0x91ab08 + i * 0x14 and
   0x91ab1c + i * 0x14, each clamped at 0 (max(0.0, ..)); the height (float)(param_12 - 0x1e), the baseline
   (float)(param_12 - 0x16) - value * height; x = 0x2d + i * scale, the scale (param_11 - 0x69) / 16 (param_11 /
   param_12 are the two screen words); the last bar x = (scale * 3 + 9) * 5 .. scale * 16 + 0x2d with the
   value 0x91ac34 and param_4. The ten leading parameters are the chart values copied by the caller.
   ROUGH: the float arguments of 0x5de518 are lost in Ghidra; a matching draft needs the max() spelling. */

void ai_Unk5c76b0(int p1, int p2, int p3, float v4, int p5, int p6, int p7, int p8, int p9, int p10,
                  int p11, int p12)
{
    float h;
    float base;
    float *p;
    int scale;
    int x;
    float a;
    float b;

    h = (float)(p12 - 0x1e);
    base = (float)(p12 - 0x16);
    scale = (p11 - 0x69) / 16;
    x = 0x2d;
    p = (float *)((unsigned)&ord_StatisticsHistory + 0x1c);
    do {
        a = *(float *)((char *)p - 0x14);
        b = *p;
        d2_PlotLine(x, (int)(base - max(0.0, a) * h), x + scale, (int)(base - max(0.0, b) * h),
                     0xee, 0x10, 0xee);
        p = (float *)((char *)p + 0x14);
        x += scale;
    } while ((int)p < ((unsigned)((unsigned)ord_StatisticsHistoryB + 0x140) + 0x8));
    a = *(float *)((unsigned)&ord_StatisticsHistory + 0x134);
    d2_PlotLine((scale * 3 + 9) * 5, (int)(base - max(0.0, a) * h), scale * 16 + 0x2d,
                 (int)(base - max(0.0, v4) * h), 0xee, 0x10, 0xee);
}
