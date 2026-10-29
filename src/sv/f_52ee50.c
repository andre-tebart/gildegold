/* 0x52ee50, starts the rubber band selection (0x760440 = 1): both corners (0x760444 / 0x760448 and 0x76044c / 0x760450) are the mouse position clamped to the view rectangle 0xfc2930 .. 0xfc293c */

void sv_Unk52ee50(void)
{
    int mx, my, m;

    mx = SV_S((unsigned)&dm_L);
    m = SV_I((unsigned)&d3_Viewport);
    if (m <= mx)
        m = mx;
    SV_I((unsigned)&full_data_760444) = SV_I(((unsigned)&d3_Viewport + 0x8)) - 1;
    if (m < SV_I(((unsigned)&d3_Viewport + 0x8)) - 1 && (SV_I((unsigned)&full_data_760444) = mx, mx < SV_I((unsigned)&d3_Viewport)))
        SV_I((unsigned)&full_data_760444) = SV_I((unsigned)&d3_Viewport);
    my = SV_S(((unsigned)&dm_L + 0x2));
    m = SV_I(((unsigned)&d3_Viewport + 0x4));
    if (m <= my)
        m = my;
    SV_I((unsigned)&full_data_760448) = SV_I(((unsigned)&d3_Viewport + 0xc)) - 1;
    if (m < SV_I((unsigned)&full_data_760448) && (SV_I((unsigned)&full_data_760448) = my, my < SV_I(((unsigned)&d3_Viewport + 0x4))))
        SV_I((unsigned)&full_data_760448) = SV_I(((unsigned)&d3_Viewport + 0x4));
    SV_I((unsigned)&full_data_76044c) = SV_I((unsigned)&full_data_760444);
    SV_I((unsigned)&ai_Unk760440) = 1;
    SV_I((unsigned)&full_data_760450) = SV_I((unsigned)&full_data_760448);
}
