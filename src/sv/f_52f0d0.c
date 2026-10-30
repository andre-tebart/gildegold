/* 0x52f0d0, like 0x52eee0 (the flags come from 0x4ce170): moves the second corner with the mouse (clamped to the view) and flags (+0x188) the characters of the table 0x7603a0 whose screen position (three float conversions, the
   operands are lost: placeholders) lies inside the band; ROUGH */

void sv_Unk52f0d0(void)
{
    unsigned fl;
    int mx, my, m, x0, x1, y0, y1, px, py, c, *p, g;

    if (SV_I((unsigned)&ai_Unk760440) != 0) {
        mx = SV_S((unsigned)&dm_L);
        m = SV_I((unsigned)&d3_Viewport);
        if (m <= mx)
            m = mx;
        SV_I((unsigned)&full_data_76044c) = SV_I(((unsigned)&d3_Viewport + 0x8)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0x8)) - 1 && (SV_I((unsigned)&full_data_76044c) = mx, mx < SV_I((unsigned)&d3_Viewport)))
            SV_I((unsigned)&full_data_76044c) = SV_I((unsigned)&d3_Viewport);
        my = SV_S(((unsigned)&dm_L + 0x2));
        m = SV_I(((unsigned)&d3_Viewport + 0x4));
        if (m <= my)
            m = my;
        SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0xc)) - 1;
        if (m < SV_I(((unsigned)&d3_Viewport + 0xc)) - 1 && (SV_I((unsigned)&full_data_760450) = SV_I(((unsigned)&d3_Viewport + 0x4)), SV_I(((unsigned)&d3_Viewport + 0x4)) <= my))
            SV_I((unsigned)&full_data_760450) = my;
        x0 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_76044c) <= SV_I((unsigned)&full_data_760444))
            x0 = SV_I((unsigned)&full_data_76044c);
        y0 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760450) <= SV_I((unsigned)&full_data_760448))
            y0 = SV_I((unsigned)&full_data_760450);
        x1 = SV_I((unsigned)&full_data_760444);
        if (SV_I((unsigned)&full_data_760444) <= SV_I((unsigned)&full_data_76044c))
            x1 = SV_I((unsigned)&full_data_76044c);
        y1 = SV_I((unsigned)&full_data_760448);
        if (SV_I((unsigned)&full_data_760448) <= SV_I((unsigned)&full_data_760450))
            y1 = SV_I((unsigned)&full_data_760450);
        p = (int *)(unsigned)&ai_Unk7603a0;
        do {
            c = *p;
            if (c != 0 && *(char *)(c + 8) != 0) {
                fl = ai_Unk4ce170(ai_PlayerId, c);
                *(char *)(c + 0x188) = 0;
                if ((fl & 0x800) != 0 && (g = *(int *)(c + 0x184)) != 0 && *(char **)(g + 0x88) == ai_Unk5fb5b0 && *(int *)(*(int *)(g + 0x34) + 0x1cc) != 0) {
                    px = (int)0.0f;             /* the screen position of the character (float operands lost) */
                    py = (int)0.0f;
                    if (x0 <= px && px <= x1 && y0 <= py && py <= y1)
                        *(char *)(c + 0x188) = 1;
                }
            }
            p++;
        } while ((int)p < (unsigned)((unsigned)ai_Unk7603a0 + 0x80));
    }
}
