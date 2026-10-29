/* 0x5c7460, the second bar chart (640 B, ROUGH): the maximum of the floats 0x91ab0c + i * 0x14, the
   bars 0x4764f0 (0x2d + i * the scale, the float) with the colour 0x10 / 0xee / 0x10 and the scale
   (param_6 - 0x69) / 16; the last bar at (scale * 3 + 9) * 5 */

void ai_Unk5c7460(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6,
                  int param_7, int param_8, int param_9, int param_10, int param_11, int param_12)
{
    float max;
    int scale;
    int x;
    float *p;

    max = -1.0f;
    for (p = (float *)((unsigned)&ord_StatisticsHistory + 0xc); (int)p < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0xc); p += 5) {
        if (max < *p)
            max = *p;
    }
    scale = (param_11 - 0x69) / 16;
    x = 0x2d;
    for (p = (float *)((unsigned)&ord_StatisticsHistory + 0xc); (int)p < ((unsigned)&ord_StatisticsHistory + 0x138); p += 5) {
        d2_PlotLine(x, (int)*p);
        x += scale;
    }
    d2_PlotLine((scale * 3 + 9) * 5, (int)*(float *)((char *)((unsigned)&ord_StatisticsHistory + 0x138) - 4));
}
