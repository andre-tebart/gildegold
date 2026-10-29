/* 0x5c72d0, a bar chart of the 0x91ab10 table (393 B, ROUGH): the thresholds 500 / 1000 / 2000 / 4000 /
   8000 / 10000 / 15000 / 20000 / 50000; the maximum of the table entries 0x91ab10 + i * 0x14 (5 dwords);
   the value param_6 clamped up to the smallest threshold above it; the bars 0x4764f0 (0x2d + i * the
   scale, the float field of the entry) with the scale (param_6 - 0x69) / 16; the last bar at
   (scale * 3 + 9) * 5. Returns the clamped value. ROUGH: the float arguments of 0x4764f0 are lost in
   Ghidra (the __ftol pairs) */

int ai_Unk5c72d0(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6,
                 int param_7, int param_8, int param_9, int param_10, int param_11, int param_12)
{
    int limits[9];
    int max;
    int scale;
    int v;
    int i;
    int x;
    int *p;

    limits[0] = 500;
    limits[1] = 1000;
    limits[2] = 2000;
    limits[3] = 4000;
    limits[4] = 8000;
    limits[5] = 10000;
    limits[6] = 15000;
    limits[7] = 20000;
    limits[8] = 50000;
    scale = (param_11 - 0x69) / 16;
    max = -1;
    for (p = (int *)((unsigned)&ord_StatisticsHistory + 0x10); p < (int *)((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x10); p += 5) {
        if (max < *p)
            max = *p;
    }
    v = max;
    for (i = 0; i < 8; i++) {
        if (v < limits[i]) {
            v = limits[i];
            break;
        }
    }
    x = 0x2d;
    for (p = (int *)((unsigned)&ord_StatisticsHistory + 0x10); p < (int *)((unsigned)&ord_StatisticsHistory + 0x13c); p += 5) {
        d2_PlotLine(x, (int)*(float *)((char *)p - 4));
        x += scale;
    }
    d2_PlotLine((scale * 3 + 9) * 5, (int)*(float *)((char *)((unsigned)&ord_StatisticsHistory + 0x13c) - 4));
    return v;
}
