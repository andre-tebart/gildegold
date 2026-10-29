/* 0x43fe00, starts the terrain texture system: the colour tables, the cache of n entries, the detail level,
   mipmapping, the map shift and the jitter / uv tables */

void d3fl_Init(unsigned int n, unsigned int detail, char mips, char b)
{
    dd_MakeColorTables(d3d_TexRMask, d3d_TexGMask, d3d_TexBMask, d3fl_ColorTab[0]);
    d3fl_InitCache(n);
    d3fl_SetDetail(detail);
    d3fl_Mips = mips;
    d3fl_MapShift = b;
    d3fl_UV[0][0] = 0.0f;
    d3fl_UV[0][1] = 0.0f;
    d3fl_UV[0][2] = 1.0f;
    d3fl_UV[0][3] = 1.0f;
    d3fl_UV[0][4] = 0.0f;
    d3fl_UV[0][5] = 1.0f;
    d3fl_UV[1][0] = 0.0f;
    d3fl_UV[1][1] = 0.0f;
    d3fl_UV[1][2] = 1.0f;
    d3fl_UV[1][3] = 0.0f;
    d3fl_UV[1][4] = 1.0f;
    d3fl_UV[1][5] = 1.0f;
    d3fl_MakeJitter(d3fl_Jitter);
}
