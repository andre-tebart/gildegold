/* 0x45fa80 */

void tex_Init(int count, int maxhicol, int mips, unsigned int cache)
{
    d3_TexCount = count;
    ts_Palette = 0;
    ts_MaxHiCol = maxhicol;
    ts_Mips = mips;
    ts_HiColCount = 0;
    ts_TexUsed = 0;
    d3_Textures = mem_Alloc(count * sizeof(TSTEXTURE), "d3_ts:InitTextures(tex)");
    if (maxhicol > 0)
        ts_HiCol = mem_Alloc(maxhicol * sizeof(TSHICOL), "d3_ts:InitTextures(hicol)");
    else
        ts_HiCol = 0;
    memset(ts_ScrollU, 0, sizeof(ts_ScrollU));
    memset(ts_ScrollV, 0, sizeof(ts_ScrollV));
    ts_NextGroup = 0;
    ts_LastTime = d3_Time;
    ts_InitCache(cache);
}
