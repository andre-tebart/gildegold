/* 0x5c7560, the third bar chart (384 B, ROUGH): the bars 0x4764f0 (0x2d + i * the scale, the float of
   0x91ab14 + i * 0x14) with the colour 0xee / 0xee / 0x10 and the scale (param_6 - 0x69) / 16; the last
   bar at (scale * 3 + 9) * 5 */

void ai_Unk5c7560(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6,
                  int param_7, int param_8, int param_9, int param_10, int param_11, int param_12)
{
    int scale;
    int x;
    unsigned int *p;

    scale = (param_11 - 0x69) / 16;
    x = 0x2d;
    for (p = (unsigned int *)((unsigned)&ord_StatisticsHistory + 0x14); (int)p < (unsigned)((unsigned)ord_StatisticsHistory + 0x140); p += 5) {
        d2_PlotLine(x, (int)*(float *)((char *)p - 4));
        x += scale;
    }
    d2_PlotLine((scale * 3 + 9) * 5, (int)*(float *)((char *)(unsigned)&ai_Unk91ac40 - 4));
}
