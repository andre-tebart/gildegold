/* 0x5c7800, the sibling bar chart (325 B): first the maximum of the floats 0x91ab0c + i * 0x14 (the caller's
   scale value; computed and not read again), then the same rectangles 0x4764f0 (x, y1, x + scale, y2, 0xee,
   0x10, 0x10) between the floats 0x91ab04 + i * 0x14 and 0x91ab18 + i * 0x14 clamped at 0, x = 0x2d + i *
   scale, the scale / the height / the baseline as in 0x5c76b0; the last bar x = (scale * 3 + 9) * 5 ..
   scale * 16 + 0x2d with the value 0x91ac30 and param_4. ROUGH. */

void ai_Unk5c7800(int p1, int p2, int p3, float v4, int p5, int p6, int p7, int p8, int p9, int p10,
                  int p11, int p12)
{
    float maxv;
    float h;
    float base;
    float *p;
    int scale;
    int x;
    float a;
    float b;

    maxv = -1.0f;
    for (p = (float *)((unsigned)&ord_StatisticsHistory + 0xc); (int)p < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0xc); p += 5) {
        if (maxv < *p)
            maxv = *p;
    }
    h = (float)(p12 - 0x1e);
    base = (float)(p12 - 0x16);
    scale = (p11 - 0x69) / 16;
    x = 0x2d;
    p = (float *)((unsigned)&ord_StatisticsHistory + 0x18);
    do {
        a = *(float *)((char *)p - 0x14);
        b = *p;
        d2_PlotLine(x, (int)(base - max(0.0, a) * h), x + scale, (int)(base - max(0.0, b) * h),
                     0xee, 0x10, 0x10);
        p = (float *)((char *)p + 0x14);
        x += scale;
    } while ((int)p < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
    a = *(float *)((unsigned)&ord_StatisticsHistory + 0x130);
    d2_PlotLine((scale * 3 + 9) * 5, (int)(base - max(0.0, a) * h), scale * 16 + 0x2d,
                 (int)(base - max(0.0, v4) * h), 0xee, 0x10, 0x10);
}
